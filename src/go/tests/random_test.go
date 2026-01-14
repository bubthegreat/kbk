package main

import (
	"testing"
)

func TestNumberRange(t *testing.T) {
	// Test edge cases
	t.Run("both_zero", func(t *testing.T) {
		result := NumberRange(0, 0)
		if result != 0 {
			t.Errorf("NumberRange(0, 0) = %d, want 0", result)
		}
	})

	t.Run("same_value", func(t *testing.T) {
		result := NumberRange(5, 5)
		if result != 5 {
			t.Errorf("NumberRange(5, 5) = %d, want 5", result)
		}
	})

	t.Run("range_1_to_10", func(t *testing.T) {
		for i := 0; i < 100; i++ {
			result := NumberRange(1, 10)
			if result < 1 || result > 10 {
				t.Errorf("NumberRange(1, 10) = %d, want value in [1, 10]", result)
			}
		}
	})

	t.Run("negative_range", func(t *testing.T) {
		for i := 0; i < 100; i++ {
			result := NumberRange(-5, 5)
			if result < -5 || result > 5 {
				t.Errorf("NumberRange(-5, 5) = %d, want value in [-5, 5]", result)
			}
		}
	})
}

func TestNumberPercent(t *testing.T) {
	for i := 0; i < 100; i++ {
		result := NumberPercent()
		if result < 1 || result > 100 {
			t.Errorf("NumberPercent() = %d, want value in [1, 100]", result)
		}
	}
}

func TestNumberDoor(t *testing.T) {
	for i := 0; i < 100; i++ {
		result := NumberDoor()
		if result < 0 || result > 5 {
			t.Errorf("NumberDoor() = %d, want value in [0, 5]", result)
		}
	}
}

func TestNumberBits(t *testing.T) {
	tests := []struct {
		width int
		max   int
	}{
		{0, 0},
		{1, 1},
		{2, 3},
		{3, 7},
		{4, 15},
		{8, 255},
	}

	for _, tt := range tests {
		t.Run("", func(t *testing.T) {
			for i := 0; i < 50; i++ {
				result := NumberBits(C.int(tt.width))
				if result < 0 || result > C.int(tt.max) {
					t.Errorf("NumberBits(%d) = %d, want value in [0, %d]", tt.width, result, tt.max)
				}
			}
		})
	}
}

func TestDice(t *testing.T) {
	t.Run("zero_size", func(t *testing.T) {
		result := Dice(3, 0)
		if result != 0 {
			t.Errorf("Dice(3, 0) = %d, want 0", result)
		}
	})

	t.Run("size_one", func(t *testing.T) {
		result := Dice(5, 1)
		if result != 5 {
			t.Errorf("Dice(5, 1) = %d, want 5", result)
		}
	})

	t.Run("3d6", func(t *testing.T) {
		for i := 0; i < 100; i++ {
			result := Dice(3, 6)
			if result < 3 || result > 18 {
				t.Errorf("Dice(3, 6) = %d, want value in [3, 18]", result)
			}
		}
	})

	t.Run("1d20", func(t *testing.T) {
		for i := 0; i < 100; i++ {
			result := Dice(1, 20)
			if result < 1 || result > 20 {
				t.Errorf("Dice(1, 20) = %d, want value in [1, 20]", result)
			}
		}
	})
}

func TestNumberFuzzy(t *testing.T) {
	// Test that fuzzy returns value within expected range
	for i := 0; i < 100; i++ {
		input := C.int(10)
		result := NumberFuzzy(input)
		if result < 9 || result > 11 {
			t.Errorf("NumberFuzzy(10) = %d, want value in [9, 11]", result)
		}
	}

	// Test minimum value of 1
	t.Run("minimum_one", func(t *testing.T) {
		result := NumberFuzzy(1)
		if result < 1 {
			t.Errorf("NumberFuzzy(1) = %d, want value >= 1", result)
		}
	})
}

func TestInterpolate(t *testing.T) {
	tests := []struct {
		level    int
		value00  int
		value32  int
		expected int
	}{
		{0, 10, 50, 10},
		{32, 10, 50, 50},
		{16, 10, 50, 30},
		{8, 0, 100, 25},
	}

	for _, tt := range tests {
		t.Run("", func(t *testing.T) {
			result := Interpolate(C.int(tt.level), C.int(tt.value00), C.int(tt.value32))
			if result != C.int(tt.expected) {
				t.Errorf("Interpolate(%d, %d, %d) = %d, want %d",
					tt.level, tt.value00, tt.value32, result, tt.expected)
			}
		})
	}
}

func TestUMin(t *testing.T) {
	if UMin(5, 10) != 5 {
		t.Error("UMin(5, 10) should be 5")
	}
	if UMin(10, 5) != 5 {
		t.Error("UMin(10, 5) should be 5")
	}
}

func TestUMax(t *testing.T) {
	if UMax(5, 10) != 10 {
		t.Error("UMax(5, 10) should be 10")
	}
	if UMax(10, 5) != 10 {
		t.Error("UMax(10, 5) should be 10")
	}
}

func TestURange(t *testing.T) {
	if URange(0, -5, 10) != 0 {
		t.Error("URange(0, -5, 10) should be 0")
	}
	if URange(0, 5, 10) != 5 {
		t.Error("URange(0, 5, 10) should be 5")
	}
	if URange(0, 15, 10) != 10 {
		t.Error("URange(0, 15, 10) should be 10")
	}
}
