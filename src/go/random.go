package main

import "C"
import (
	"math/rand"
	"time"
)

var rng *rand.Rand

func init() {
	// Initialize random number generator with current time and process ID
	// This matches the behavior of init_mm() in the C code
	rng = rand.New(rand.NewSource(time.Now().UnixNano()))
}

// NumberRange generates a random number between from and to (inclusive).
// Matches the C function: int number_range(int from, int to)
//
//export NumberRange
func NumberRange(from C.int, to C.int) C.int {
	fromInt := int(from)
	toInt := int(to)

	// Handle edge cases matching C implementation
	if fromInt == 0 && toInt == 0 {
		return 0
	}

	rangeSize := toInt - fromInt + 1
	if rangeSize <= 1 {
		return from
	}

	// Generate random number in range
	return C.int(fromInt + rng.Intn(rangeSize))
}

// NumberPercent generates a percentile roll (1-100).
// Matches the C function: int number_percent(void)
//
//export NumberPercent
func NumberPercent() C.int {
	return C.int(1 + rng.Intn(100))
}

// NumberDoor generates a random door direction (0-5).
// Matches the C function: int number_door(void)
//
//export NumberDoor
func NumberDoor() C.int {
	return C.int(rng.Intn(6))
}

// NumberBits generates a random number with the specified number of bits.
// Matches the C function: int number_bits(int width)
//
//export NumberBits
func NumberBits(width C.int) C.int {
	if width <= 0 {
		return 0
	}
	mask := (1 << uint(width)) - 1
	return C.int(rng.Int31() & int32(mask))
}

// Dice rolls dice with the specified number and size (e.g., 3d6).
// Matches the C function: int dice(int number, int size)
//
//export Dice
func Dice(number C.int, size C.int) C.int {
	numInt := int(number)
	sizeInt := int(size)

	// Handle edge cases matching C implementation
	switch sizeInt {
	case 0:
		return 0
	case 1:
		return number
	}

	sum := 0
	for i := 0; i < numInt; i++ {
		sum += 1 + rng.Intn(sizeInt)
	}

	return C.int(sum)
}

// NumberFuzzy adds a small random variation to a number.
// The number can be decreased by 1, stay the same, or increased by 1.
// Matches the C function: int number_fuzzy(int number)
//
//export NumberFuzzy
func NumberFuzzy(number C.int) C.int {
	numInt := int(number)

	// Use 2-bit random number (0-3) to determine fuzz
	switch rng.Intn(4) {
	case 0:
		numInt--
	case 3:
		numInt++
	}

	// Ensure result is at least 1
	if numInt < 1 {
		return 1
	}

	return C.int(numInt)
}

// Interpolate performs simple linear interpolation.
// Matches the C function: int interpolate(int level, int value_00, int value_32)
//
//export Interpolate
func Interpolate(level C.int, value00 C.int, value32 C.int) C.int {
	levelInt := int(level)
	v00 := int(value00)
	v32 := int(value32)

	result := v00 + (levelInt * (v32 - v00) / 32)
	return C.int(result)
}

// UMin returns the minimum of two integers.
// Matches the C macro: UMIN(a, b)
//
//export UMin
func UMin(a C.int, b C.int) C.int {
	if a < b {
		return a
	}
	return b
}

// UMax returns the maximum of two integers.
// Matches the C macro: UMAX(a, b)
//
//export UMax
func UMax(a C.int, b C.int) C.int {
	if a > b {
		return a
	}
	return b
}

// URange clamps a value between a minimum and maximum.
// Matches the C macro: URANGE(a, b, c)
//
//export URange
func URange(min C.int, val C.int, max C.int) C.int {
	if val < min {
		return min
	}
	if val > max {
		return max
	}
	return val
}

