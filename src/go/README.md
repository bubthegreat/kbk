# KBK Go Library

This directory contains Go implementations of isolated utility functions from the KBK MUD server.
These functions are designed to be called from C code using CGO, enabling a gradual migration
from C to Go while maintaining full backward compatibility.

## Architecture

The hybrid compilation approach uses CGO to create a static library that can be linked with
the existing C codebase. This allows us to:

1. Migrate isolated, pure functions to Go incrementally
2. Maintain 100% functionality parity with the C implementation
3. Add comprehensive unit tests for migrated functions
4. Improve code quality and safety over time

## Migrated Functions

### Lookup Functions (lookup.go)
- `DirectionLookup` - Convert direction strings to numeric values
- `PositionLookup` - Look up position values by name
- `SexLookup` - Look up sex values by name
- `SizeLookup` - Look up size values by name

### Random Number Functions (random.go)
- `NumberRange` - Generate random number in range
- `NumberPercent` - Generate percentile roll (1-100)
- `Dice` - Roll dice (e.g., 3d6)
- `NumberFuzzy` - Add fuzz to a number

### String Utilities (strings.go)
- `StrCmp` - Case-insensitive string comparison
- `StrPrefix` - Case-insensitive prefix matching
- `StrSuffix` - Case-insensitive suffix matching
- `Capitalize` - Capitalize first letter

## Building

The Go code is compiled into a static library (`libkbkgo.a`) and header file (`kbkgo.h`)
that are linked with the C code during the main build process.

```bash
cd src/go
go build -buildmode=c-archive -o ../libkbkgo.a
```

## Testing

Run the comprehensive test suite:

```bash
cd src/go
go test -v ./...
```

## Future Migration Candidates

Based on analysis of the codebase, good candidates for future migration include:

1. **Bit manipulation functions** (bit.c) - flag operations
2. **More lookup functions** (lookup.c) - cabal, empire, hometown lookups
3. **String formatting** (string.c) - format_string, string_proper, etc.
4. **File I/O utilities** - fread_number, fread_string, etc.
5. **Mathematical utilities** - interpolate, number_bits, etc.

## Design Principles

1. **No side effects** - Migrated functions should be pure or have minimal side effects
2. **Clear interfaces** - C-compatible function signatures
3. **Comprehensive tests** - Every function must have unit tests
4. **Performance** - Go implementations should match or exceed C performance
5. **Safety** - Leverage Go's memory safety and error handling

