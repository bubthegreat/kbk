# AI Agent Quick Start Guide - KBK Go Migration

**For AI agents continuing the C-to-Go migration work**

---

## 🚀 Quick Start (5 minutes)

### 1. Understand the Project
- **What:** Migrating a 126,000 line C MUD server to Go incrementally
- **How:** Using CGO to create hybrid C+Go executable
- **Status:** Phase 1 complete (20 utility functions migrated)
- **Next:** Phase 2 - Data structures and bit manipulation

### 2. Read the Docs
1. **Start here:** `docs/GO_MIGRATION_PLAN.md` (comprehensive plan)
2. **Technical details:** `docs/GO_MIGRATION_ARCHITECTURE.md` (architecture)
3. **This file:** Quick reference for common tasks

### 3. Verify Setup
```bash
# Check Go tests pass
cd src/go && go test -v ./...

# Check build works
cd src/go && ./build.sh
cd src && make clean && make
```

If all green ✅ - you're ready to start!

---

## 📋 Common Tasks

### Task: Migrate a New Function

**Example: Migrating `is_set()` from bit.c**

1. **Find the C function:**
```bash
grep -n "is_set" src/bit.c
```

2. **Create Go implementation in `src/go/bits.go`:**
```go
// Internal implementation (pure Go, testable)
func isSet(flags int64, bit int64) bool {
    return (flags & bit) != 0
}

// Exported for C (CGO wrapper)
//export IsSet
func IsSet(flags C.long, bit C.long) C.int {
    if isSet(int64(flags), int64(bit)) {
        return 1  // true in C
    }
    return 0  // false in C
}
```

3. **Write tests in `src/go/bits_test.go`:**
```go
func TestIsSet(t *testing.T) {
    // Test bit is set
    if !isSet(0b1010, 0b0010) {
        t.Error("Expected bit to be set")
    }
    
    // Test bit is not set
    if isSet(0b1010, 0b0100) {
        t.Error("Expected bit to not be set")
    }
}
```

4. **Run tests:**
```bash
cd src/go && go test -v ./...
```

5. **Build and verify:**
```bash
cd src/go && ./build.sh
cd src && make clean && make
```

6. **Update docs:**
- Add to "Current Status Tracking" in `GO_MIGRATION_PLAN.md`
- Mark checkbox in "Appendix A: Function Inventory"

---

### Task: Run Tests

```bash
# All tests
cd src/go && go test -v ./...

# Specific test
cd src/go && go test -v -run TestIsSet

# With coverage
cd src/go && go test -v -cover ./...

# With race detection
cd src/go && go test -race ./...
```

---

### Task: Add a New Go File

**Example: Creating `bits.go` for bit manipulation functions**

1. **Create the file:**
```bash
vim src/go/bits.go
```

2. **Add package and imports:**
```go
package main

import "C"

// Bit manipulation functions for KBK MUD
```

3. **Add functions (internal + exported pairs)**

4. **Create test file:**
```bash
vim src/go/bits_test.go
```

5. **Add tests:**
```go
package main

import "testing"

func TestBitFunctions(t *testing.T) {
    // Tests here
}
```

6. **Update `main.go` comment to document new file**

---

### Task: Fix a Failing Test

1. **Identify the failure:**
```bash
cd src/go && go test -v ./...
```

2. **Read the error message carefully**

3. **Fix the internal Go function** (not the CGO wrapper)

4. **Re-run tests:**
```bash
cd src/go && go test -v -run TestFailingFunction
```

5. **If still failing after 3 attempts, STOP and ask the human**

---

### Task: Benchmark Performance

1. **Add benchmark to test file:**
```go
func BenchmarkIsSet(b *testing.B) {
    for i := 0; i < b.N; i++ {
        isSet(0b11111111, 0b00000010)
    }
}
```

2. **Run benchmark:**
```bash
cd src/go && go test -bench=BenchmarkIsSet -benchmem
```

3. **Compare with C baseline** (if available)

4. **If >20% slower, investigate and optimize**

---

## 🎯 What to Migrate Next

### Recommended Order (Phase 2)

1. **bit.c functions** (4-6 functions, 2-4 hours)
   - `is_set()`, `set_bit()`, `remove_bit()`, `toggle_bit()`
   - Pure functions, easy to test
   - Create `src/go/bits.go` and `src/go/bits_test.go`

2. **const.c lookups** (10-15 functions, 3-5 hours)
   - `flag_string()`, `flag_value()`
   - Similar to existing lookup.go
   - Add to `src/go/lookup.go` or create `src/go/flags.go`

3. **handler.c utilities** (5-10 functions, 4-6 hours)
   - `get_curr_stat()`, `can_carry_n()`, `can_carry_w()`
   - Calculations, no side effects
   - Create `src/go/character.go`

---

## ⚠️ Red Flags - When to STOP

**STOP and ask the human if you encounter:**

- ❌ Tests failing after 3 fix attempts
- ❌ Performance >20% worse than C
- ❌ Function modifies global state extensively
- ❌ Function requires complex C struct manipulation
- ❌ Anything in `comm.c` or the `nanny()` function
- ❌ Memory leaks detected
- ❌ Segmentation faults

---

## 📚 Code Patterns

### Pattern: Simple Function
```go
// Internal (pure Go, testable)
func myFunction(a, b int) int {
    return a + b
}

// Exported (CGO wrapper)
//export MyFunction
func MyFunction(a C.int, b C.int) C.int {
    return C.int(myFunction(int(a), int(b)))
}
```

### Pattern: String Function
```go
// Internal
func myStringFunc(s string) string {
    return strings.ToUpper(s)
}

// Exported
//export MyStringFunc
func MyStringFunc(s *C.char) *C.char {
    if s == nil {
        return nil
    }
    result := myStringFunc(C.GoString(s))
    return C.CString(result)  // Caller MUST free!
}
```

### Pattern: Boolean Function
```go
// Internal
func myCheck(value int) bool {
    return value > 0
}

// Exported
//export MyCheck
func MyCheck(value C.int) C.int {
    if myCheck(int(value)) {
        return 1  // true
    }
    return 0  // false
}
```

---

## 🔍 Useful Commands

```bash
# Find a C function
grep -rn "function_name" src/*.c

# View a C file at specific line
vim src/filename.c +123

# Count lines in C files
find src -name "*.c" | xargs wc -l

# Check Go code quality
cd src/go && go vet ./...

# Format Go code
cd src/go && go fmt ./...

# View test coverage in browser
cd src/go && go test -coverprofile=coverage.out ./...
cd src/go && go tool cover -html=coverage.out
```

---

## 📝 Documentation Updates

After completing work, update these sections in `GO_MIGRATION_PLAN.md`:

1. **Current Status Tracking** - Add migrated functions
2. **Phase Status** - Update completion percentage  
3. **Appendix B: Change Log** - Add entry with date and summary
4. **Appendix A: Function Inventory** - Check off completed functions

**Example update:**
```markdown
### 2026-01-15 - Phase 2 Started
- ✅ Migrated bit.c functions (4 functions)
- ✅ Created bits_test.go with 100% coverage
- ⚠️ Note: Used int64 for bit flags (C uses long)
- **Time:** 3 hours actual vs 2.5 estimated
- **Next:** const.c lookup functions
```

---

## 🎓 Learning Resources

- **CGO Basics:** https://pkg.go.dev/cmd/cgo
- **Go Testing:** https://pkg.go.dev/testing
- **Existing Examples:** Look at `src/go/random.go`, `strings.go`, `lookup.go`

---

## ✅ Checklist for Each Function

- [ ] Found C function and understood its behavior
- [ ] Created internal Go function (lowercase, pure Go types)
- [ ] Created exported CGO wrapper (uppercase, C types)
- [ ] Wrote comprehensive tests
- [ ] All tests pass: `go test -v ./...`
- [ ] Built successfully: `./build.sh && make`
- [ ] Updated GO_MIGRATION_PLAN.md
- [ ] No performance regression (if critical path)

---

**Remember:** 
- Test internal functions, not CGO wrappers
- Always check pointers for nil
- Document memory ownership (who frees what)
- When in doubt, ask the human!

**Good luck! 🚀**

