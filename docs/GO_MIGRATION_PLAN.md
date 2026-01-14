# KBK MUD: C to Go Migration Plan

**Version:** 1.0  
**Last Updated:** 2026-01-14  
**Status:** Phase 1 - Foundation Complete

---

## Executive Summary

This document outlines a comprehensive, incremental migration strategy for the KBK MUD codebase from C to Go. The migration uses a hybrid approach with CGO, allowing gradual replacement of C functions with Go implementations while maintaining 100% backward compatibility and zero downtime.

**Codebase Stats:**
- **Total C Files:** 51
- **Total Lines of C Code:** ~126,000 LOC
- **Architecture:** Traditional MUD server (DikuMUD derivative)
- **Key Dependencies:** MySQL, POSIX sockets, file I/O

---

## Migration Philosophy

### Core Principles

1. **Incremental & Safe**: Migrate one function/module at a time with full testing
2. **Zero Downtime**: Maintain production stability throughout migration
3. **Test-Driven**: Every migrated function must have comprehensive tests
4. **Reversible**: Ability to roll back any migration if issues arise
5. **Performance Neutral**: No performance degradation during migration

### Hybrid Architecture Pattern

```
┌─────────────────────────────────────────┐
│         C Main Program (comm.c)         │
│    - Network I/O                        │
│    - Game Loop                          │
│    - State Management                   │
└──────────────┬──────────────────────────┘
               │
               │ CGO Interface
               ▼
┌─────────────────────────────────────────┐
│      Go Static Library (libkbkgo.a)     │
│                                         │
│  ┌─────────────┐  ┌─────────────┐     │
│  │  Exported   │  │  Internal   │     │
│  │  Functions  │─▶│  Functions  │     │
│  │  (C types)  │  │  (Go types) │     │
│  └─────────────┘  └─────────────┘     │
│                                         │
│  - Utility Functions                   │
│  - Business Logic                      │
│  - Data Processing                     │
└─────────────────────────────────────────┘
```

### Function Migration Pattern

For each C function to migrate:

1. **Create internal Go implementation** (lowercase function name)
   - Uses pure Go types (int, string, bool)
   - Contains all business logic
   - Fully testable without CGO

2. **Create exported CGO wrapper** (uppercase function name)
   - Converts C types to Go types
   - Calls internal Go function
   - Converts Go return value to C type
   - Marked with `//export` directive

3. **Write comprehensive tests**
   - Test internal Go function (no CGO required)
   - Cover edge cases, error conditions
   - Validate against original C behavior

4. **Update C code**
   - Replace C implementation with call to Go function
   - Or keep C wrapper that calls Go function

---

## Codebase Structure Analysis

### Major Systems (by file)

| System | Files | LOC Est. | Complexity | Priority |
|--------|-------|----------|------------|----------|
| **Network/Comm** | comm.c | ~4,000 | Very High | Phase 5 |
| **Database** | db.c, db2.c, mysql.c | ~8,000 | High | Phase 4 |
| **Combat** | fight.c, morefight.c | ~6,000 | High | Phase 3 |
| **Magic/Spells** | magic.c, moremagic.c, necro.c, commune.c | ~8,000 | High | Phase 3 |
| **Skills** | skills.c, assassin.c, ranger.c | ~5,000 | Medium | Phase 3 |
| **Actions** | act_*.c (6 files) | ~10,000 | Medium | Phase 3 |
| **Handlers** | handler.c, update.c | ~8,000 | High | Phase 4 |
| **Utilities** | lookup.c, string.c, bit.c, const.c | ~3,000 | Low | **Phase 1** ✅ |
| **OLC** | olc.c, olc_act.c, olc_save.c | ~6,000 | Medium | Phase 4 |
| **Progs** | mprog.c, iprog.c, rprog.c | ~4,000 | Medium | Phase 3 |
| **Misc** | save.c, recycle.c, tables.c, etc. | ~10,000 | Medium | Phase 2-3 |

---

## Migration Phases

### Phase 1: Foundation & Utilities ✅ **COMPLETE**

**Goal:** Establish migration pattern, tooling, and migrate simple utility functions

**Status:** Complete (2026-01-14)

**Completed Work:**
- ✅ Set up Go module structure (`src/go/`)
- ✅ Configure CGO build system
- ✅ Create Makefile integration
- ✅ Establish testing framework
- ✅ Migrate utility functions:
  - `random.go`: numberRange, numberPercent, numberDoor, numberBits, dice, numberFuzzy, interpolate, uMin, uMax, uRange
  - `strings.go`: strCmp, strPrefix, strSuffix, strInfix, capitalize, smashTilde
  - `lookup.go`: directionLookup, positionLookup, sexLookup, sizeLookup
- ✅ Write comprehensive tests (100% passing)
- ✅ Update CI/CD pipeline for Go tests

**Files Modified:**
- Created: `src/go/random.go`, `src/go/strings.go`, `src/go/lookup.go`
- Created: `src/go/random_test.go`, `src/go/strings_test.go`, `src/go/lookup_test.go`
- Modified: `src/Makefile`, `.github/workflows/build-and-deploy.yml`

**Lessons Learned:**
- CGO doesn't work in test files - must use internal Go functions for testing
- Pattern of internal (lowercase) + exported (uppercase) functions works well
- CI needs Go toolchain installed separately from Docker build

**Time Invested:** ~8 hours

---

### Phase 2: Data Structures & Memory Management

**Goal:** Migrate data manipulation functions and establish patterns for complex types

**Estimated Duration:** 20-30 hours

**Target Files:**
- `bit.c` - Bit manipulation functions
- `recycle.c` - Memory pooling/recycling
- `mem.c` - Memory management utilities
- `const.c` - Constant definitions and lookups

**Key Functions to Migrate (~30-40 functions):**

From `bit.c`:
- `is_set()` - Check if bit is set
- `set_bit()` - Set a bit
- `remove_bit()` - Clear a bit
- `toggle_bit()` - Toggle a bit

From `recycle.c`:
- Object recycling functions (if applicable)
- Memory pool management

From `const.c`:
- Constant lookup tables
- Flag name conversions

**Challenges:**
- Bit manipulation in Go (use bitwise operators)
- Memory management differences (Go has GC)
- May need to create Go equivalents of C structs

**Success Criteria:**
- All bit operations migrated with tests
- Memory management patterns established
- Performance benchmarks show no regression

**Deliverables:**
- `src/go/bits.go` with internal + exported functions
- `src/go/bits_test.go` with comprehensive tests
- Documentation on bit manipulation patterns

---

### Phase 3: Game Logic & Calculations

**Goal:** Migrate self-contained game logic functions

**Estimated Duration:** 60-100 hours

**Target Systems:**

#### 3A: Combat Calculations (20-30 hours)
**Files:** `fight.c`, `morefight.c`

**Functions to Migrate (~40-60 functions):**
- Damage calculation functions
- Hit/miss determination
- Critical hit logic
- Armor class calculations
- Weapon damage calculations
- Combat modifiers

**Pattern:**
```go
// Internal implementation
func calculateDamage(attackerLevel, weaponDamage, armorClass int) int {
    // Pure Go logic
}

// Exported for C
//export CalculateDamage
func CalculateDamage(attackerLevel C.int, weaponDamage C.int, armorClass C.int) C.int {
    return C.int(calculateDamage(int(attackerLevel), int(weaponDamage), int(armorClass)))
}
```

#### 3B: Spell Effects (20-30 hours)
**Files:** `magic.c`, `moremagic.c`, `necro.c`, `commune.c`

**Functions to Migrate (~50-70 functions):**
- Spell damage calculations
- Duration calculations
- Saving throw logic
- Spell effect applications
- Mana cost calculations

#### 3C: Skill Checks (15-25 hours)
**Files:** `skills.c`, `assassin.c`, `ranger.c`

**Functions to Migrate (~30-50 functions):**
- Skill check calculations
- Success/failure determination
- Skill improvement logic
- Class-specific skill functions

**Challenges:**
- Many functions have side effects (modify game state)
- Need to separate pure calculations from state changes
- Some functions are tightly coupled to C data structures

**Strategy:**
1. Start with pure calculation functions (no side effects)
2. Extract calculation logic from stateful functions
3. Leave state modification in C initially
4. Gradually migrate state management later

---

### Phase 4: Data Persistence & I/O

**Goal:** Migrate database and file I/O operations

**Estimated Duration:** 40-60 hours

**Target Files:**
- `mysql.c` - Database operations
- `save.c` - Player save/load
- `db.c`, `db2.c` - World database loading

**Key Functions (~40-60 functions):**

From `mysql.c`:
- Database connection management
- Query execution
- Result parsing
- Transaction handling

From `save.c`:
- Player serialization
- Player deserialization
- Object save/load
- Character data persistence

**Challenges:**
- Database connections need careful management
- File I/O patterns differ between C and Go
- Character encoding issues
- Transaction safety

**Strategy:**
1. Create Go database abstraction layer
2. Migrate query functions one at a time
3. Implement comprehensive error handling
4. Add connection pooling in Go
5. Maintain C compatibility layer

**Benefits:**
- Better error handling in Go
- Connection pooling
- Prepared statements
- SQL injection protection

---

### Phase 5: Network & Core Systems

**Goal:** Migrate core server infrastructure (FINAL PHASE)

**Estimated Duration:** 80-120 hours

**Target Files:**
- `comm.c` - Network communication
- `handler.c` - Core game handlers
- `update.c` - Game loop updates
- `interp.c` - Command interpreter

**WARNING:** This is the most critical and risky phase. Do not attempt until Phases 1-4 are complete and stable.

**The Nanny Function:**
- **1,082 lines** of complex state machine code
- Handles all login/character creation
- **DO NOT MIGRATE EARLY**
- Migrate supporting functions first
- Consider rewriting in Go from scratch rather than direct translation

**Strategy:**
1. Extract utility functions from core systems first
2. Create Go versions of data structures
3. Implement parallel Go handlers
4. Gradually switch traffic to Go handlers
5. Keep C versions as fallback
6. Nanny migration should be LAST

**Success Criteria:**
- Zero downtime during migration
- Performance equal or better than C
- All edge cases handled
- Comprehensive integration tests

---

## Function Migration Checklist

Use this checklist for each function being migrated:

### Pre-Migration
- [ ] Identify function in C codebase
- [ ] Document function behavior and edge cases
- [ ] Identify all callers of the function
- [ ] Check for global state dependencies
- [ ] Verify function is suitable for migration (pure or extractable logic)

### Implementation
- [ ] Create internal Go function (lowercase, pure Go types)
- [ ] Implement business logic in Go
- [ ] Create exported CGO wrapper (uppercase, C types)
- [ ] Add type conversions in wrapper
- [ ] Add error handling

### Testing
- [ ] Write unit tests for internal function
- [ ] Test edge cases (null, zero, negative, overflow)
- [ ] Test against original C behavior
- [ ] Run tests locally: `cd src/go && go test -v ./...`
- [ ] Verify CI passes

### Integration
- [ ] Update C code to call Go function (or keep C wrapper)
- [ ] Update Makefile if needed
- [ ] Test in development environment
- [ ] Verify no memory leaks
- [ ] Check performance (benchmark if critical path)

### Documentation
- [ ] Update function documentation
- [ ] Add to migration tracking (this document)
- [ ] Note any behavioral changes
- [ ] Document any caveats or limitations

---

## Current Status Tracking

### Migrated Functions (Phase 1 Complete)

#### random.go (10 functions)
- ✅ `numberRange(from, to int) int`
- ✅ `numberPercent() int`
- ✅ `numberDoor() int`
- ✅ `numberBits(width int) int`
- ✅ `dice(number, size int) int`
- ✅ `numberFuzzy(number int) int`
- ✅ `interpolate(level, value00, value32 int) int`
- ✅ `uMin(a, b int) int`
- ✅ `uMax(a, b int) int`
- ✅ `uRange(min, val, max int) int`

#### strings.go (6 functions)
- ✅ `strCmp(a, b string) bool`
- ✅ `strPrefix(a, b string) bool`
- ✅ `strSuffix(a, b string) bool`
- ✅ `strInfix(a, b string) bool`
- ✅ `capitalize(s string) string`
- ✅ `smashTilde(s string) string`

#### lookup.go (4 functions)
- ✅ `directionLookup(dirStr string) int`
- ✅ `positionLookup(nameStr string) int`
- ✅ `sexLookup(nameStr string) int`
- ✅ `sizeLookup(nameStr string) int`

**Total Migrated:** 20 functions
**Test Coverage:** 100%
**Status:** All tests passing ✅

---

## Candidate Functions for Next Migration

### High Priority - Phase 2 Candidates

These functions are good candidates for immediate migration (low complexity, high value):

#### From `bit.c` (~10 functions, 2-4 hours)
```c
bool is_set(const struct flag_type *flag_table, long bits)
void set_bit(long *bits, long bit)
void remove_bit(long *bits, long bit)
void toggle_bit(long *bits, long bit)
```

**Rationale:** Pure functions, no side effects, widely used

#### From `const.c` (~15 functions, 3-5 hours)
```c
char *flag_string(const struct flag_type *flag_table, long bits)
long flag_value(const struct flag_type *flag_table, char *argument)
```

**Rationale:** Lookup tables, pure functions, easy to test

#### From `handler.c` - Utility subset (~10 functions, 4-6 hours)
```c
int get_curr_stat(CHAR_DATA *ch, int stat)
int get_max_train(CHAR_DATA *ch, int stat)
int can_carry_n(CHAR_DATA *ch)
int can_carry_w(CHAR_DATA *ch)
```

**Rationale:** Calculations based on character data, minimal side effects

#### From `fight.c` - Damage calculations (~8 functions, 5-8 hours)
```c
int dam_modifier(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt)
int critical_strike_modifier(CHAR_DATA *ch, CHAR_DATA *victim, int dam)
bool check_parry(CHAR_DATA *ch, CHAR_DATA *victim)
bool check_dodge(CHAR_DATA *ch, CHAR_DATA *victim)
```

**Rationale:** Core game logic, high test value, performance critical

### Medium Priority - Phase 3 Candidates

#### From `magic.c` - Spell calculations (~20 functions, 10-15 hours)
```c
int mana_cost(CHAR_DATA *ch, int min_mana, int level)
int spell_damage(int level, int dice_num, int dice_size, int bonus)
bool saves_spell(int level, CHAR_DATA *victim, int dam_type)
```

**Rationale:** Important game logic, but more complex

#### From `skills.c` - Skill checks (~15 functions, 8-12 hours)
```c
int get_skill(CHAR_DATA *ch, int sn)
void check_improve(CHAR_DATA *ch, int sn, bool success, int multiplier)
bool can_use_skill(CHAR_DATA *ch, int sn)
```

**Rationale:** Frequently called, good test candidates

---

## Technical Patterns & Best Practices

### Pattern 1: Simple Value Functions

**C Code:**
```c
int number_range(int from, int to) {
    if (from == 0 && to == 0) return 0;
    int range = to - from + 1;
    if (range <= 1) return from;
    return from + rand() % range;
}
```

**Go Migration:**
```go
// Internal implementation - pure Go
func numberRange(from, to int) int {
    if from == 0 && to == 0 {
        return 0
    }
    rangeSize := to - from + 1
    if rangeSize <= 1 {
        return from
    }
    return from + rng.Intn(rangeSize)
}

// Exported for C
//export NumberRange
func NumberRange(from C.int, to C.int) C.int {
    return C.int(numberRange(int(from), int(to)))
}
```

**Test:**
```go
func TestNumberRange(t *testing.T) {
    result := numberRange(0, 0)
    if result != 0 {
        t.Errorf("numberRange(0, 0) = %d, want 0", result)
    }

    for i := 0; i < 100; i++ {
        result := numberRange(1, 10)
        if result < 1 || result > 10 {
            t.Errorf("numberRange(1, 10) = %d, out of range", result)
        }
    }
}
```

### Pattern 2: String Functions

**C Code:**
```c
bool str_cmp(const char *astr, const char *bstr) {
    if (!astr || !bstr) return TRUE;
    return strcasecmp(astr, bstr) != 0;
}
```

**Go Migration:**
```go
// Internal - works with Go strings
func strCmp(a, b string) bool {
    return !strings.EqualFold(a, b)
}

// Exported - handles C strings
//export StrCmp
func StrCmp(astr *C.char, bstr *C.char) C.int {
    if astr == nil || bstr == nil {
        return 1 // Different (TRUE in C)
    }
    if strCmp(C.GoString(astr), C.GoString(bstr)) {
        return 1
    }
    return 0
}
```

### Pattern 3: Lookup Tables

**C Code:**
```c
const struct direction_type direction_table[] = {
    { "north", 0 },
    { "east",  1 },
    // ...
};

int direction_lookup(char *name) {
    for (int i = 0; direction_table[i].name != NULL; i++) {
        if (!str_prefix(name, direction_table[i].name))
            return direction_table[i].value;
    }
    return -1;
}
```

**Go Migration:**
```go
// Internal - use Go map or switch
func directionLookup(dirStr string) int {
    dirLower := strings.ToLower(dirStr)
    switch dirLower {
    case "n", "north":
        return 0
    case "e", "east":
        return 1
    // ...
    default:
        return -1
    }
}

//export DirectionLookup
func DirectionLookup(dir *C.char) C.int {
    if dir == nil {
        return -1
    }
    return C.int(directionLookup(C.GoString(dir)))
}
```

### Pattern 4: Functions with Side Effects

**Strategy:** Separate calculation from side effects

**C Code:**
```c
void damage(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt) {
    // Calculate damage
    dam = dam_modifier(ch, victim, dam, dt);

    // Apply damage (side effect)
    victim->hit -= dam;

    // Check death (side effect)
    if (victim->hit < 0) {
        raw_kill(victim);
    }
}
```

**Go Migration (Phase 1 - Extract calculation):**
```go
// Migrate calculation only
func damModifier(attackerLevel, victimArmor, baseDam int) int {
    // Pure calculation logic
    modified := baseDam
    // ... calculation ...
    return modified
}

//export DamModifier
func DamModifier(attackerLevel C.int, victimArmor C.int, baseDam C.int) C.int {
    return C.int(damModifier(int(attackerLevel), int(victimArmor), int(baseDam)))
}
```

**C Code (updated to use Go):**
```c
void damage(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt) {
    // Call Go for calculation
    dam = DamModifier(ch->level, victim->armor, dam);

    // Keep side effects in C for now
    victim->hit -= dam;
    if (victim->hit < 0) {
        raw_kill(victim);
    }
}
```

---

## Common Pitfalls & Solutions

### Pitfall 1: CGO in Test Files
**Problem:** Cannot use `import "C"` in `*_test.go` files
**Solution:** Test internal (lowercase) Go functions, not exported CGO wrappers

### Pitfall 2: Memory Management
**Problem:** C expects manual memory management, Go has GC
**Solution:**
- For strings returned to C: Use `C.CString()` (caller must free)
- For temporary conversions: Use `C.GoString()` (Go manages memory)
- Document memory ownership clearly

### Pitfall 3: Null Pointers
**Problem:** C code may pass NULL pointers
**Solution:** Always check for nil in exported functions
```go
//export SomeFunction
func SomeFunction(ptr *C.char) C.int {
    if ptr == nil {
        return -1 // or appropriate error value
    }
    // ... rest of function
}
```

### Pitfall 4: Global State
**Problem:** C code uses global variables extensively
**Solution:**
- Phase 1: Keep global state in C, pass as parameters to Go
- Phase 2: Gradually move state to Go
- Use Go packages to encapsulate state

### Pitfall 5: Error Handling
**Problem:** C uses return codes, Go uses error returns
**Solution:**
- For exported functions: Use C conventions (return codes)
- For internal Go: Use proper Go error handling
- Log errors appropriately

---

## Performance Considerations

### Benchmarking Strategy

For each migrated function in critical path:

1. **Baseline C performance:**
```bash
# Use gprof or similar
gcc -pg -o mud *.c
./mud
gprof mud gmon.out > analysis.txt
```

2. **Benchmark Go implementation:**
```go
func BenchmarkNumberRange(b *testing.B) {
    for i := 0; i < b.N; i++ {
        numberRange(1, 100)
    }
}
```

3. **Compare results:**
- Go should be within 10% of C performance
- If slower, profile and optimize
- Consider CGO call overhead

### CGO Call Overhead

**Important:** CGO calls have overhead (~100-200ns per call)

**Mitigation strategies:**
- Batch operations when possible
- Migrate entire subsystems, not individual tiny functions
- Keep hot path functions in same language

### Memory Usage

**Monitor:**
- Go heap size: `runtime.MemStats`
- C heap size: existing tools
- Total process memory

**Target:** No more than 10% increase in memory usage

---

## AI Agent Instructions

### For Future AI Agents Working on This Migration

This section provides specific guidance for AI agents continuing this migration work.

#### Before Starting Any Migration Work

1. **Read this entire document** to understand the migration strategy
2. **Check the "Current Status Tracking" section** to see what's already done
3. **Review the "Candidate Functions" section** for suggested next steps
4. **Verify tests pass:** `cd src/go && go test -v ./...`
5. **Check CI status** in GitHub Actions

#### When Migrating a New Function

1. **Use the "Function Migration Checklist"** (above)
2. **Follow the established patterns** (see "Technical Patterns & Best Practices")
3. **Always create internal + exported function pairs**
4. **Write tests for the internal function** (not the CGO wrapper)
5. **Update this document** with your progress

#### Updating This Document

When you complete work, update these sections:

1. **Current Status Tracking:** Add newly migrated functions
2. **Phase Status:** Update completion percentage
3. **Lessons Learned:** Add any new insights or issues
4. **Time Tracking:** Update estimated vs actual time

#### Example Update:

```markdown
### Phase 2: Data Structures & Memory Management

**Status:** In Progress (40% complete)

**Completed Work:**
- ✅ Migrated bit.c functions (4 hours actual vs 3 estimated)
- ✅ Created bits_test.go with 95% coverage
- ⚠️ Issue: Performance regression in set_bit (investigating)

**In Progress:**
- 🔄 Migrating const.c lookup functions (2/15 complete)

**Lessons Learned:**
- Bit manipulation in Go requires careful uint conversion
- Benchmark showed 15% slowdown, optimized with bit tricks
```

#### Common Tasks

**Task: Add a new migrated function**
1. Create/update the appropriate `.go` file in `src/go/`
2. Add internal function (lowercase, pure Go types)
3. Add exported function (uppercase, CGO wrapper)
4. Create/update corresponding `_test.go` file
5. Run tests: `go test -v ./...`
6. Update this document's "Current Status Tracking"

**Task: Fix a failing test**
1. Run tests to identify failure: `go test -v ./...`
2. Check if it's a logic error or type conversion issue
3. Fix the internal Go function (not the wrapper)
4. Re-run tests
5. If performance-critical, run benchmarks

**Task: Investigate performance issue**
1. Write benchmark: `func BenchmarkFunctionName(b *testing.B) { ... }`
2. Run: `go test -bench=. -benchmem`
3. Compare with C baseline
4. Profile if needed: `go test -cpuprofile=cpu.prof`
5. Optimize and re-benchmark

#### Red Flags - When to Stop and Ask

Stop and ask the human for guidance if:

- ❌ Tests are failing and you can't determine why after 3 attempts
- ❌ Performance is >20% worse than C implementation
- ❌ You encounter a function that modifies global state extensively
- ❌ The function requires complex C struct manipulation
- ❌ You're considering migrating anything in `comm.c` or the nanny function
- ❌ Memory leaks are detected
- ❌ The migration requires changing the C API significantly

#### Helpful Commands

```bash
# Run all Go tests
cd src/go && go test -v ./...

# Run tests with coverage
cd src/go && go test -v -cover ./...

# Run benchmarks
cd src/go && go test -bench=. -benchmem

# Build the Go library
cd src/go && ./build.sh

# Build the full MUD (C + Go)
cd src && make clean && make

# Check for Go issues
cd src/go && go vet ./...

# Format Go code
cd src/go && go fmt ./...
```

---

## Risk Assessment

### Low Risk Migrations (Safe to proceed)
- ✅ Pure utility functions (math, string manipulation)
- ✅ Lookup tables and constants
- ✅ Calculation functions with no side effects
- ✅ Functions with clear inputs/outputs

### Medium Risk Migrations (Proceed with caution)
- ⚠️ Functions that read global state
- ⚠️ Functions with complex C struct parameters
- ⚠️ Performance-critical hot path functions
- ⚠️ Functions with subtle edge cases

### High Risk Migrations (Requires careful planning)
- 🔴 Functions that modify global state
- 🔴 Network I/O functions
- 🔴 Database operations
- 🔴 Memory management functions
- 🔴 Signal handlers

### DO NOT MIGRATE (Until final phases)
- 🚫 `nanny()` function in comm.c
- 🚫 Main game loop
- 🚫 Socket handling
- 🚫 Core state management
- 🚫 Anything that could cause downtime

---

## Success Metrics

### Phase Completion Criteria

**Phase 1 (Foundation):** ✅ COMPLETE
- [x] 15+ utility functions migrated
- [x] 100% test coverage
- [x] CI/CD pipeline updated
- [x] Documentation complete

**Phase 2 (Data Structures):**
- [ ] 30+ functions migrated
- [ ] Bit manipulation patterns established
- [ ] 95%+ test coverage
- [ ] No performance regression

**Phase 3 (Game Logic):**
- [ ] 100+ functions migrated
- [ ] Combat calculations in Go
- [ ] Spell effects in Go
- [ ] Skill checks in Go
- [ ] Performance equal or better

**Phase 4 (Data Persistence):**
- [ ] Database layer in Go
- [ ] Save/load functions in Go
- [ ] Connection pooling implemented
- [ ] Zero data loss

**Phase 5 (Core Systems):**
- [ ] Network layer in Go
- [ ] Game loop in Go
- [ ] Command interpreter in Go
- [ ] Zero downtime migration

### Overall Project Success
- [ ] >80% of codebase in Go
- [ ] All tests passing
- [ ] Performance maintained or improved
- [ ] Production stable
- [ ] Documentation complete

---

## Timeline Estimates

### Conservative Estimate (Part-time, 10 hours/week)

| Phase | Duration | Completion Date |
|-------|----------|-----------------|
| Phase 1 | ✅ Complete | 2026-01-14 |
| Phase 2 | 3-4 weeks | 2026-02-11 |
| Phase 3 | 10-12 weeks | 2026-05-06 |
| Phase 4 | 6-8 weeks | 2026-06-24 |
| Phase 5 | 12-15 weeks | 2026-10-06 |
| **Total** | **~9 months** | **October 2026** |

### Aggressive Estimate (Full-time, 40 hours/week)

| Phase | Duration | Completion Date |
|-------|----------|-----------------|
| Phase 1 | ✅ Complete | 2026-01-14 |
| Phase 2 | 1 week | 2026-01-21 |
| Phase 3 | 3 weeks | 2026-02-11 |
| Phase 4 | 2 weeks | 2026-02-25 |
| Phase 5 | 3 weeks | 2026-03-18 |
| **Total** | **~2 months** | **March 2026** |

**Note:** These are estimates. Actual time may vary based on:
- Complexity of discovered edge cases
- Performance optimization needs
- Testing thoroughness
- Integration challenges

---

## Resources & References

### Documentation
- [CGO Documentation](https://pkg.go.dev/cmd/cgo)
- [Go Testing](https://pkg.go.dev/testing)
- [Go Performance](https://github.com/golang/go/wiki/Performance)

### Internal Documentation
- `src/go/README.md` - Go library overview
- `src/go/build.sh` - Build script
- `.github/workflows/build-and-deploy.yml` - CI/CD pipeline

### Key Files
- `src/Makefile` - Build configuration
- `src/go/go.mod` - Go module definition
- `src/merc.h` - Main C header with data structures

### Testing
- Run tests: `cd src/go && go test -v ./...`
- View coverage: `go test -cover ./...`
- Benchmarks: `go test -bench=.`

---

## Appendix A: Function Inventory

### Complete List of C Functions by File

This section will be populated as we analyze each file. For now, we have:

#### lookup.c (COMPLETE ✅)
- [x] `direction_lookup()` → `directionLookup()`
- [x] `position_lookup()` → `positionLookup()`
- [x] `sex_lookup()` → `sexLookup()`
- [x] `size_lookup()` → `sizeLookup()`

#### random.c (COMPLETE ✅)
- [x] `number_range()` → `numberRange()`
- [x] `number_percent()` → `numberPercent()`
- [x] `number_door()` → `numberDoor()`
- [x] `number_bits()` → `numberBits()`
- [x] `dice()` → `dice()`
- [x] `number_fuzzy()` → `numberFuzzy()`
- [x] `interpolate()` → `interpolate()`
- [x] `UMIN()` → `uMin()`
- [x] `UMAX()` → `uMax()`
- [x] `URANGE()` → `uRange()`

#### string.c (COMPLETE ✅)
- [x] `str_cmp()` → `strCmp()`
- [x] `str_prefix()` → `strPrefix()`
- [x] `str_suffix()` → `strSuffix()`
- [x] `str_infix()` → `strInfix()`
- [x] `capitalize()` → `capitalize()`
- [x] `smash_tilde()` → `smashTilde()`

#### bit.c (PENDING - Phase 2)
- [ ] `is_set()`
- [ ] `set_bit()`
- [ ] `remove_bit()`
- [ ] `toggle_bit()`
- [ ] Additional bit manipulation functions (TBD)

#### const.c (PENDING - Phase 2)
- [ ] `flag_string()`
- [ ] `flag_value()`
- [ ] Additional constant lookups (TBD)

*(More files to be inventoried as migration progresses)*

---

## Appendix B: Change Log

### 2026-01-14 - Phase 1 Complete
- ✅ Migrated 20 utility functions across 3 files
- ✅ Created comprehensive test suite (100% passing)
- ✅ Updated CI/CD pipeline for Go testing
- ✅ Established migration patterns and best practices
- ✅ Created this migration plan document
- **Time:** 8 hours actual
- **Next:** Begin Phase 2 (bit.c and const.c)

---

## Appendix C: Contact & Support

### For Questions or Issues

1. **Check this document first** - most questions are answered here
2. **Review existing code** - look at migrated functions for patterns
3. **Check test files** - they show expected behavior
4. **Ask the human** - if stuck or encountering red flags

### Document Maintenance

This document should be updated:
- ✅ After completing each function migration
- ✅ After completing each phase
- ✅ When discovering new patterns or issues
- ✅ When timeline estimates change
- ✅ When adding new insights or lessons learned

**Last Updated:** 2026-01-14
**Document Version:** 1.0
**Migration Status:** Phase 1 Complete, Phase 2 Ready to Start

---

*End of Migration Plan*

