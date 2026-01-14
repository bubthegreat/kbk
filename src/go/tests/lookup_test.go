package main

import (
	"testing"
)

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
			result := directionLookup(tt.input)
			if result != tt.expected {
				t.Errorf("directionLookup(%q) = %d, want %d", tt.input, result, tt.expected)
			}
		})
	}

	// Test nil/empty string
	t.Run("empty", func(t *testing.T) {
		result := directionLookup("")
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
			result := positionLookup(tt.input)
			if result != tt.expected {
				t.Errorf("positionLookup(%q) = %d, want %d", tt.input, result, tt.expected)
			}
		})
	}

	// Test empty string
	t.Run("empty", func(t *testing.T) {
		result := positionLookup("")
		if result != -1 {
			t.Errorf("positionLookup(\"\") = %d, want -1", result)
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
			result := sexLookup(tt.input)
			if result != tt.expected {
				t.Errorf("sexLookup(%q) = %d, want %d", tt.input, result, tt.expected)
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
			result := sizeLookup(tt.input)
			if result != tt.expected {
				t.Errorf("sizeLookup(%q) = %d, want %d", tt.input, result, tt.expected)
			}
		})
	}
}
