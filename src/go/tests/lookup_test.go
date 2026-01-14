package main

import (
	"testing"
	"unsafe"
)

/*
#include <stdlib.h>
*/
import "C"

func TestDirectionLookup(t *testing.T) {
	tests := []struct {
		input    string
		expected int
	}{
		{"n", 0},
		{"north", 0},
		{"N", 0},
		{"NORTH", 0},
		{"e", 1},
		{"east", 1},
		{"s", 2},
		{"south", 2},
		{"w", 3},
		{"west", 3},
		{"u", 4},
		{"up", 4},
		{"d", 5},
		{"down", 5},
		{"invalid", -1},
		{"", -1},
	}

	for _, tt := range tests {
		t.Run(tt.input, func(t *testing.T) {
			cstr := C.CString(tt.input)
			defer C.free(unsafe.Pointer(cstr))

			result := DirectionLookup(cstr)
			if int(result) != tt.expected {
				t.Errorf("DirectionLookup(%q) = %d, want %d", tt.input, result, tt.expected)
			}
		})
	}

	// Test nil pointer
	t.Run("nil", func(t *testing.T) {
		result := DirectionLookup(nil)
		if result != -1 {
			t.Errorf("DirectionLookup(nil) = %d, want -1", result)
		}
	})
}

func TestPositionLookup(t *testing.T) {
	tests := []struct {
		input    string
		expected int
	}{
		{"dead", 0},
		{"d", 0},
		{"DEAD", 0},
		{"mortally wounded", 1},
		{"mort", 1},
		{"m", 1},
		{"incapacitated", 2},
		{"incap", 2},
		{"stunned", 3},
		{"stun", 3},
		{"sleeping", 4},
		{"sleep", 4},
		{"resting", 5},
		{"rest", 5},
		{"sitting", 6},
		{"sit", 6},
		{"fighting", 7},
		{"fight", 7},
		{"standing", 8},
		{"stand", 8},
		{"invalid", -1},
		{"", -1},
	}

	for _, tt := range tests {
		t.Run(tt.input, func(t *testing.T) {
			cstr := C.CString(tt.input)
			defer C.free(unsafe.Pointer(cstr))

			result := PositionLookup(cstr)
			if int(result) != tt.expected {
				t.Errorf("PositionLookup(%q) = %d, want %d", tt.input, result, tt.expected)
			}
		})
	}

	// Test nil pointer
	t.Run("nil", func(t *testing.T) {
		result := PositionLookup(nil)
		if result != -1 {
			t.Errorf("PositionLookup(nil) = %d, want -1", result)
		}
	})
}

func TestSexLookup(t *testing.T) {
	tests := []struct {
		input    string
		expected int
	}{
		{"none", 0},
		{"n", 0},
		{"male", 1},
		{"m", 1},
		{"MALE", 1},
		{"female", 2},
		{"f", 2},
		{"either", 3},
		{"e", 3},
		{"invalid", -1},
		{"", -1},
	}

	for _, tt := range tests {
		t.Run(tt.input, func(t *testing.T) {
			cstr := C.CString(tt.input)
			defer C.free(unsafe.Pointer(cstr))

			result := SexLookup(cstr)
			if int(result) != tt.expected {
				t.Errorf("SexLookup(%q) = %d, want %d", tt.input, result, tt.expected)
			}
		})
	}
}

func TestSizeLookup(t *testing.T) {
	tests := []struct {
		input    string
		expected int
	}{
		{"tiny", 0},
		{"t", 0},
		{"small", 1},
		{"s", 1},
		{"medium", 2},
		{"m", 2},
		{"large", 3},
		{"l", 3},
		{"huge", 4},
		{"h", 4},
		{"giant", 5},
		{"g", 5},
		{"invalid", -1},
		{"", -1},
	}

	for _, tt := range tests {
		t.Run(tt.input, func(t *testing.T) {
			cstr := C.CString(tt.input)
			defer C.free(unsafe.Pointer(cstr))

			result := SizeLookup(cstr)
			if int(result) != tt.expected {
				t.Errorf("SizeLookup(%q) = %d, want %d", tt.input, result, tt.expected)
			}
		})
	}
}
