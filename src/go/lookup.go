package main

import "C"
import (
	"strings"
)

// DirectionLookup converts a direction string to a numeric direction value.
// Returns -1 if the direction is not recognized.
// Matches the C function: int direction_lookup(char *dir)
//
//export DirectionLookup
func DirectionLookup(dir *C.char) C.int {
	if dir == nil {
		return -1
	}

	dirStr := strings.ToLower(C.GoString(dir))

	switch dirStr {
	case "n", "north":
		return 0
	case "e", "east":
		return 1
	case "s", "south":
		return 2
	case "w", "west":
		return 3
	case "u", "up":
		return 4
	case "d", "down":
		return 5
	default:
		return -1
	}
}

// Position table matching the C implementation
var positionTable = []struct {
	name      string
	shortName string
}{
	{"dead", "dead"},
	{"mortally wounded", "mort"},
	{"incapacitated", "incap"},
	{"stunned", "stun"},
	{"sleeping", "sleep"},
	{"resting", "rest"},
	{"sitting", "sit"},
	{"fighting", "fight"},
	{"standing", "stand"},
}

// PositionLookup finds a position by name with case-insensitive prefix matching.
// Returns -1 if not found.
// Matches the C function: int position_lookup(const char *name)
//
//export PositionLookup
func PositionLookup(name *C.char) C.int {
	if name == nil {
		return -1
	}

	nameStr := C.GoString(name)
	if len(nameStr) == 0 {
		return -1
	}

	nameLower := strings.ToLower(nameStr)
	firstChar := nameLower[0]

	for i, pos := range positionTable {
		if len(pos.name) > 0 && pos.name[0] == firstChar {
			if strings.HasPrefix(strings.ToLower(pos.name), nameLower) {
				return C.int(i)
			}
		}
	}

	return -1
}

// Sex table matching the C implementation
var sexTable = []struct {
	name string
}{
	{"none"},
	{"male"},
	{"female"},
	{"either"},
}

// SexLookup finds a sex value by name with case-insensitive prefix matching.
// Returns -1 if not found.
// Matches the C function: int sex_lookup(const char *name)
//
//export SexLookup
func SexLookup(name *C.char) C.int {
	if name == nil {
		return -1
	}

	nameStr := C.GoString(name)
	if len(nameStr) == 0 {
		return -1
	}

	nameLower := strings.ToLower(nameStr)
	firstChar := nameLower[0]

	for i, sex := range sexTable {
		if len(sex.name) > 0 && sex.name[0] == firstChar {
			if strings.HasPrefix(strings.ToLower(sex.name), nameLower) {
				return C.int(i)
			}
		}
	}

	return -1
}

// Size table matching the C implementation
var sizeTable = []struct {
	name string
}{
	{"tiny"},
	{"small"},
	{"medium"},
	{"large"},
	{"huge"},
	{"giant"},
}

// SizeLookup finds a size value by name with case-insensitive prefix matching.
// Returns -1 if not found.
// Matches the C function: int size_lookup(const char *name)
//
//export SizeLookup
func SizeLookup(name *C.char) C.int {
	if name == nil {
		return -1
	}

	nameStr := C.GoString(name)
	if len(nameStr) == 0 {
		return -1
	}

	nameLower := strings.ToLower(nameStr)
	firstChar := nameLower[0]

	for i, size := range sizeTable {
		if len(size.name) > 0 && size.name[0] == firstChar {
			if strings.HasPrefix(strings.ToLower(size.name), nameLower) {
				return C.int(i)
			}
		}
	}

	return -1
}

