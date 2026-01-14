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

// numberRange is the internal implementation
func numberRange(from, to int) int {
	// Handle edge cases matching C implementation
	if from == 0 && to == 0 {
		return 0
	}

	rangeSize := to - from + 1
	if rangeSize <= 1 {
		return from
	}

	// Generate random number in range
	return from + rng.Intn(rangeSize)
}

// NumberRange generates a random number between from and to (inclusive).
// Matches the C function: int number_range(int from, int to)
//
//export NumberRange
func NumberRange(from C.int, to C.int) C.int {
	return C.int(numberRange(int(from), int(to)))
}

// numberPercent is the internal implementation
func numberPercent() int {
	return 1 + rng.Intn(100)
}

// NumberPercent generates a percentile roll (1-100).
// Matches the C function: int number_percent(void)
//
//export NumberPercent
func NumberPercent() C.int {
	return C.int(numberPercent())
}

// numberDoor is the internal implementation
func numberDoor() int {
	return rng.Intn(6)
}

// NumberDoor generates a random door direction (0-5).
// Matches the C function: int number_door(void)
//
//export NumberDoor
func NumberDoor() C.int {
	return C.int(numberDoor())
}

// numberBits is the internal implementation
func numberBits(width int) int {
	if width <= 0 {
		return 0
	}
	mask := (1 << uint(width)) - 1
	return int(rng.Int31() & int32(mask))
}

// NumberBits generates a random number with the specified number of bits.
// Matches the C function: int number_bits(int width)
//
//export NumberBits
func NumberBits(width C.int) C.int {
	return C.int(numberBits(int(width)))
}

// dice is the internal implementation
func dice(number, size int) int {
	// Handle edge cases matching C implementation
	switch size {
	case 0:
		return 0
	case 1:
		return number
	}

	sum := 0
	for i := 0; i < number; i++ {
		sum += 1 + rng.Intn(size)
	}

	return sum
}

// Dice rolls dice with the specified number and size (e.g., 3d6).
// Matches the C function: int dice(int number, int size)
//
//export Dice
func Dice(number C.int, size C.int) C.int {
	return C.int(dice(int(number), int(size)))
}

// numberFuzzy is the internal implementation
func numberFuzzy(number int) int {
	// Use 2-bit random number (0-3) to determine fuzz
	switch rng.Intn(4) {
	case 0:
		number--
	case 3:
		number++
	}

	// Ensure result is at least 1
	if number < 1 {
		return 1
	}

	return number
}

// NumberFuzzy adds a small random variation to a number.
// The number can be decreased by 1, stay the same, or increased by 1.
// Matches the C function: int number_fuzzy(int number)
//
//export NumberFuzzy
func NumberFuzzy(number C.int) C.int {
	return C.int(numberFuzzy(int(number)))
}

// interpolate is the internal implementation
func interpolate(level, value00, value32 int) int {
	return value00 + (level * (value32 - value00) / 32)
}

// Interpolate performs simple linear interpolation.
// Matches the C function: int interpolate(int level, int value_00, int value_32)
//
//export Interpolate
func Interpolate(level C.int, value00 C.int, value32 C.int) C.int {
	return C.int(interpolate(int(level), int(value00), int(value32)))
}

// uMin is the internal implementation
func uMin(a, b int) int {
	if a < b {
		return a
	}
	return b
}

// UMin returns the minimum of two integers.
// Matches the C macro: UMIN(a, b)
//
//export UMin
func UMin(a C.int, b C.int) C.int {
	return C.int(uMin(int(a), int(b)))
}

// uMax is the internal implementation
func uMax(a, b int) int {
	if a > b {
		return a
	}
	return b
}

// UMax returns the maximum of two integers.
// Matches the C macro: UMAX(a, b)
//
//export UMax
func UMax(a C.int, b C.int) C.int {
	return C.int(uMax(int(a), int(b)))
}

// uRange is the internal implementation
func uRange(min, val, max int) int {
	if val < min {
		return min
	}
	if val > max {
		return max
	}
	return val
}

// URange clamps a value between a minimum and maximum.
// Matches the C macro: URANGE(a, b, c)
//
//export URange
func URange(min C.int, val C.int, max C.int) C.int {
	return C.int(uRange(int(min), int(val), int(max)))
}
