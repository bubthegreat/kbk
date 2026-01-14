# KBK MUD: Go Migration Technical Architecture

**Version:** 1.0  
**Last Updated:** 2026-01-14  
**Companion to:** GO_MIGRATION_PLAN.md

---

## Overview

This document provides detailed technical architecture for the C-to-Go hybrid system. It covers build processes, memory management, type conversions, and integration patterns.

---

## Build System Architecture

### Directory Structure

```
kbk/
├── src/
│   ├── *.c                    # C source files
│   ├── *.h                    # C header files
│   ├── Makefile               # Main build file (C + Go integration)
│   └── go/
│       ├── go.mod             # Go module definition
│       ├── go.sum             # Go dependency checksums
│       ├── build.sh           # Go library build script
│       ├── main.go            # CGO entry point
│       ├── random.go          # Random number functions
│       ├── random_test.go     # Tests for random functions
│       ├── strings.go         # String manipulation functions
│       ├── strings_test.go    # Tests for string functions
│       ├── lookup.go          # Lookup table functions
│       ├── lookup_test.go     # Tests for lookup functions
│       └── libkbkgo.a         # Generated static library (gitignored)
├── docs/
│   ├── GO_MIGRATION_PLAN.md           # High-level migration plan
│   └── GO_MIGRATION_ARCHITECTURE.md   # This file
└── .github/
    └── workflows/
        └── build-and-deploy.yml       # CI/CD pipeline
```

### Build Process Flow

```
┌─────────────────────────────────────────────────────────────┐
│ 1. Build Go Library                                         │
│    cd src/go && ./build.sh                                  │
│    → Runs: go build -buildmode=c-archive -o libkbkgo.a     │
│    → Generates: libkbkgo.a (static library)                │
│    → Generates: libkbkgo.h (C header with exports)         │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────────┐
│ 2. Compile C Source Files                                   │
│    gcc -c *.c -I./go                                        │
│    → Includes libkbkgo.h for Go function declarations      │
│    → Generates: *.o object files                           │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ▼
┌─────────────────────────────────────────────────────────────┐
│ 3. Link Everything Together                                 │
│    gcc *.o libkbkgo.a -o kbk `mysql_config --libs` -lcrypt │
│    → Links C objects with Go static library                │
│    → Links MySQL and crypt libraries                       │
│    → Generates: kbk (final executable)                     │
└─────────────────────────────────────────────────────────────┘
```

### Makefile Integration

Key sections from `src/Makefile`:

```makefile
GO_LIB = libkbkgo.a

# Build Go library first
$(GO_LIB):
	cd go && ./build.sh

# Main executable depends on Go library
kbk: $(O_FILES) $(GO_LIB)
	$(CC) -o kbk $(O_FILES) $(GO_LIB) $(E_FLAGS)

# Clean includes Go artifacts
clean:
	rm -f *.o kbk
	cd go && rm -f libkbkgo.a libkbkgo.h
```

---

## CGO Integration Patterns

### Pattern 1: Simple Value Types

**Go → C Type Mappings:**

| Go Type | C Type | CGO Type | Notes |
|---------|--------|----------|-------|
| `int` | `int` | `C.int` | 32-bit on most platforms |
| `int64` | `long long` | `C.longlong` | 64-bit guaranteed |
| `bool` | `int` | `C.int` | 0=false, 1=true |
| `float64` | `double` | `C.double` | IEEE 754 |
| `string` | `char*` | `*C.char` | Requires conversion |

**Example:**
```go
//export NumberRange
func NumberRange(from C.int, to C.int) C.int {
    result := numberRange(int(from), int(to))
    return C.int(result)
}
```

### Pattern 2: String Handling

**C → Go (temporary):**
```go
//export StrCmp
func StrCmp(astr *C.char, bstr *C.char) C.int {
    // Check for NULL
    if astr == nil || bstr == nil {
        return 1
    }
    
    // Convert to Go strings (Go manages memory)
    a := C.GoString(astr)
    b := C.GoString(bstr)
    
    // Use Go string
    if strCmp(a, b) {
        return 1
    }
    return 0
}
```

**Go → C (caller must free):**
```go
//export GetSomething
func GetSomething() *C.char {
    result := "some string"
    // C.CString allocates C memory - caller MUST free!
    return C.CString(result)
}
```

**C code must free:**
```c
char *str = GetSomething();
// ... use str ...
free(str);  // MUST FREE!
```

### Pattern 3: Pointer Parameters

**Read-only pointer:**
```go
//export ProcessData
func ProcessData(data *C.int) C.int {
    if data == nil {
        return -1
    }
    // Read the value
    value := int(*data)
    // Process...
    return C.int(result)
}
```

**Modifying through pointer (use with caution):**
```go
//export ModifyValue
func ModifyValue(data *C.int) {
    if data == nil {
        return
    }
    // Modify the C memory
    *data = C.int(42)
}
```

### Pattern 4: Arrays and Slices

**C array → Go slice (read-only):**
```go
//export ProcessArray
func ProcessArray(arr *C.int, length C.int) C.int {
    if arr == nil || length <= 0 {
        return -1
    }

    // Create Go slice view of C array
    slice := (*[1 << 30]C.int)(unsafe.Pointer(arr))[:length:length]

    // Process slice
    for i := 0; i < int(length); i++ {
        value := int(slice[i])
        // ... process value ...
    }

    return 0
}
```

**Note:** This is advanced and should be used sparingly. Prefer passing individual values.

---

## Memory Management

### Memory Ownership Rules

1. **Go allocates, C frees:**
   - Use `C.CString()` to create C strings
   - C code MUST call `free()` when done
   - Document this requirement clearly

2. **C allocates, C frees:**
   - Go can read C memory via pointers
   - Go should NOT free C memory
   - C retains ownership

3. **Go allocates, Go manages:**
   - Internal Go data structures
   - Go garbage collector handles cleanup
   - Never pass Go pointers to C for long-term storage

### Memory Safety Checklist

- [ ] All `C.CString()` calls have corresponding `free()` in C
- [ ] No Go pointers stored in C structures
- [ ] No C pointers stored in Go structures (except temporarily)
- [ ] All pointer parameters checked for NULL/nil
- [ ] No pointer arithmetic on Go pointers

### Common Memory Issues

**Issue 1: Leaked C strings**
```go
// BAD - leaks memory
//export BadFunction
func BadFunction() *C.char {
    return C.CString("leak")  // C never frees this!
}
```

**Solution:**
```go
// GOOD - document that caller must free
//export GoodFunction
func GoodFunction() *C.char {
    // Caller MUST free the returned string
    return C.CString("no leak")
}
```

**Issue 2: Storing Go pointers in C**
```go
// BAD - Go pointer in C memory
type GoStruct struct {
    data []int
}

//export BadStore
func BadStore(cStruct *C.SomeStruct) {
    goData := &GoStruct{data: []int{1, 2, 3}}
    cStruct.ptr = unsafe.Pointer(goData)  // ILLEGAL!
}
```

**Solution:** Don't do this. Keep Go data in Go, C data in C.

---

## Type Conversion Reference

### Numeric Types

```go
// C to Go
cInt := C.int(42)
goInt := int(cInt)

cLong := C.long(100)
goInt64 := int64(cLong)

cFloat := C.float(3.14)
goFloat32 := float32(cFloat)

cDouble := C.double(2.718)
goFloat64 := float64(cDouble)

// Go to C
goInt := 42
cInt := C.int(goInt)

goInt64 := int64(100)
cLong := C.long(goInt64)
```

### Boolean Types

```go
// C to Go (C uses int for bool)
cBool := C.int(1)  // true
goBool := cBool != 0

// Go to C
goBool := true
cBool := C.int(0)
if goBool {
    cBool = C.int(1)
}
```

### String Types

```go
// C to Go (temporary - Go manages memory)
cStr := C.CString("hello")  // For creating C strings
goStr := C.GoString(cStr)   // For reading C strings
C.free(unsafe.Pointer(cStr))

// Go to C (caller must free)
goStr := "world"
cStr := C.CString(goStr)
// C code must call free(cStr)
```

### Struct Types (Advanced)

**Simple approach - pass individual fields:**
```go
//export ProcessCharacter
func ProcessCharacter(level C.int, hp C.int, name *C.char) C.int {
    // Convert and process
    goLevel := int(level)
    goHP := int(hp)
    goName := C.GoString(name)

    // ... process ...
    return C.int(result)
}
```

**Complex approach - pass struct pointer (use sparingly):**
```go
// In C header
typedef struct {
    int level;
    int hp;
    char name[50];
} CharData;

// In Go
//export ProcessCharacterStruct
func ProcessCharacterStruct(ch *C.CharData) C.int {
    if ch == nil {
        return -1
    }

    level := int(ch.level)
    hp := int(ch.hp)
    name := C.GoString(&ch.name[0])

    // ... process ...
    return C.int(result)
}
```

---

## Testing Strategy

### Test Organization

```
src/go/
├── random.go          # Implementation
├── random_test.go     # Tests
├── strings.go         # Implementation
├── strings_test.go    # Tests
└── ...
```

### Test Structure

```go
package main

import "testing"

// Test internal function (no CGO)
func TestNumberRange(t *testing.T) {
    tests := []struct {
        name string
        from int
        to   int
        want func(int) bool  // Validation function
    }{
        {
            name: "zero range",
            from: 0,
            to:   0,
            want: func(r int) bool { return r == 0 },
        },
        {
            name: "single value",
            from: 5,
            to:   5,
            want: func(r int) bool { return r == 5 },
        },
        {
            name: "normal range",
            from: 1,
            to:   10,
            want: func(r int) bool { return r >= 1 && r <= 10 },
        },
    }

    for _, tt := range tests {
        t.Run(tt.name, func(t *testing.T) {
            result := numberRange(tt.from, tt.to)
            if !tt.want(result) {
                t.Errorf("numberRange(%d, %d) = %d, validation failed",
                    tt.from, tt.to, result)
            }
        })
    }
}
```

### Running Tests

```bash
# Run all tests
cd src/go && go test -v ./...

# Run specific test
cd src/go && go test -v -run TestNumberRange

# Run with coverage
cd src/go && go test -v -cover ./...

# Generate coverage report
cd src/go && go test -coverprofile=coverage.out ./...
cd src/go && go tool cover -html=coverage.out
```

### Benchmarking

```go
func BenchmarkNumberRange(b *testing.B) {
    for i := 0; i < b.N; i++ {
        numberRange(1, 100)
    }
}

func BenchmarkStrCmp(b *testing.B) {
    for i := 0; i < b.N; i++ {
        strCmp("hello", "world")
    }
}
```

Run benchmarks:
```bash
cd src/go && go test -bench=. -benchmem
```

---

## CI/CD Integration

### GitHub Actions Workflow

Key sections from `.github/workflows/build-and-deploy.yml`:

```yaml
- name: Set up Go
  uses: actions/setup-go@v4
  with:
    go-version: '1.21'

- name: Run Go tests
  run: |
    cd src/go
    go test -v ./...

- name: Build Go library
  run: |
    cd src/go
    ./build.sh

- name: Build C executable
  run: |
    cd src
    make clean
    make
```

### Test Automation

Tests run automatically on:
- Every push to any branch
- Every pull request
- Manual workflow dispatch

### Build Verification

The CI pipeline verifies:
1. ✅ Go tests pass
2. ✅ Go library builds successfully
3. ✅ C code compiles with Go library
4. ✅ Final executable links correctly

---

## Debugging

### Debugging Go Code

```bash
# Run with verbose output
cd src/go && go test -v ./...

# Run with race detector
cd src/go && go test -race ./...

# Print debug info
func numberRange(from, to int) int {
    fmt.Printf("DEBUG: numberRange(%d, %d)\n", from, to)
    // ... rest of function
}
```

### Debugging CGO Integration

```bash
# Build with debug symbols
cd src/go && go build -buildmode=c-archive -gcflags="all=-N -l" -o libkbkgo.a

# Use GDB
gdb ./kbk
(gdb) break NumberRange
(gdb) run
```

### Common Issues

**Issue: "undefined reference to Go function"**
- Solution: Rebuild Go library: `cd src/go && ./build.sh`
- Verify `libkbkgo.a` exists
- Check Makefile includes `$(GO_LIB)` in link step

**Issue: "cannot use import "C" in test"**
- Solution: Test internal functions, not exported CGO wrappers
- Use lowercase function names in tests

**Issue: Segmentation fault**
- Check for NULL pointer dereferences
- Verify pointer parameters are checked for nil
- Use race detector: `go test -race`

---

## Performance Optimization

### CGO Call Overhead

CGO calls have ~100-200ns overhead. Minimize by:

1. **Batch operations:**
```go
// BAD - many CGO calls
for i := 0; i < 1000; i++ {
    result := NumberRange(1, 100)  // 1000 CGO calls
}

// GOOD - one CGO call
//export NumberRangeBatch
func NumberRangeBatch(from, to, count C.int, results *C.int) {
    slice := (*[1 << 30]C.int)(unsafe.Pointer(results))[:count:count]
    for i := 0; i < int(count); i++ {
        slice[i] = C.int(numberRange(int(from), int(to)))
    }
}
```

2. **Keep hot paths in same language:**
- If function is called millions of times per second, keep it in C
- Or migrate entire subsystem to Go

3. **Profile before optimizing:**
```bash
go test -cpuprofile=cpu.prof -bench=.
go tool pprof cpu.prof
```

### Memory Optimization

```go
// Use sync.Pool for frequently allocated objects
var stringPool = sync.Pool{
    New: func() interface{} {
        return new(strings.Builder)
    },
}

func processString(s string) string {
    sb := stringPool.Get().(*strings.Builder)
    defer func() {
        sb.Reset()
        stringPool.Put(sb)
    }()

    // Use sb...
    return sb.String()
}
```

---

## Migration Workflow

### Step-by-Step Process

1. **Identify function to migrate**
   - Choose from candidate list in GO_MIGRATION_PLAN.md
   - Verify it's appropriate for current phase

2. **Analyze C implementation**
   ```bash
   # Find function definition
   grep -n "function_name" src/*.c

   # View implementation
   vim src/filename.c +line_number
   ```

3. **Create Go implementation**
   ```bash
   # Edit appropriate .go file
   vim src/go/category.go
   ```

4. **Write tests**
   ```bash
   # Create/edit test file
   vim src/go/category_test.go
   ```

5. **Run tests**
   ```bash
   cd src/go && go test -v ./...
   ```

6. **Build and integrate**
   ```bash
   cd src/go && ./build.sh
   cd src && make clean && make
   ```

7. **Update documentation**
   - Update GO_MIGRATION_PLAN.md
   - Mark function as complete
   - Add any lessons learned

---

*End of Architecture Document*

