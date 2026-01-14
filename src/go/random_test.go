package main

import (
	"testing"
)

func TestNumberRange(t *testing.T) {
	t.Run("both_zero", func(t *testing.T) {
		result := numberRange(0, 0)
		if result != 0 {
			t.Errorf("numberRange(0, 0) = %d, want 0", result)
		}
	})

	t.Run("same_value", func(t *testing.T) {
		result := numberRange(5, 5)
		if result != 5 {
			t.Errorf("numberRange(5, 5) = %d, want 5", result)
		}
	})

	t.Run("range_1_to_10", func(t *testing.T) {
		for i := 0; i < 100; i++ {
			result := numberRange(1, 10)
			if result < 1 || result > 10 {
				t.Errorf("numberRange(1, 10) = %d, want value in [1, 10]", result)
			}
		}
	})

	t.Run("negative_range", func(t *testing.T) {
		for i := 0; i < 100; i++ {
			result := numberRange(-5, 5)
			if result < -5 || result > 5 {
				t.Errorf("numberRange(-5, 5) = %d, want value in [-5, 5]", result)
			}
		}
	})
}

func TestNumberPercent(t *testing.T) {
	for i := 0; i < 100; i++ {
		result := numberPercent()
		if result < 1 || result > 100 {
			t.Errorf("numberPercent() = %d, want value in [1, 100]", result)
		}
	}
}

func TestNumberDoor(t *testing.T) {
	for i := 0; i < 100; i++ {
		result := numberDoor()
		if result < 0 || result > 5 {
			t.Errorf("numberDoor() = %d, want value in [0, 5]", result)
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
				result := numberBits(tt.width)
				if result < 0 || result > tt.max {
					t.Errorf("numberBits(%d) = %d, want value in [0, %d]", tt.width, result, tt.max)
				}
			}
		})
	}
}

func TestDice(t *testing.T) {
	t.Run("zero_size", func(t *testing.T) {
		result := dice(3, 0)
		if result != 0 {
			t.Errorf("dice(3, 0) = %d, want 0", result)
		}
	})

	t.Run("size_one", func(t *testing.T) {
		result := dice(5, 1)
		if result != 5 {
			t.Errorf("dice(5, 1) = %d, want 5", result)
		}
	})

	t.Run("3d6", func(t *testing.T) {
		for i := 0; i < 100; i++ {
			result := dice(3, 6)
			if result < 3 || result > 18 {
				t.Errorf("dice(3, 6) = %d, want value in [3, 18]", result)
			}
		}
	})
}

func TestNumberFuzzy(t *testing.T) {
	for i := 0; i < 100; i++ {
		input := 10
		result := numberFuzzy(input)
		if result < 9 || result > 11 {
			t.Errorf("numberFuzzy(10) = %d, want value in [9, 11]", result)
		}
	}

	t.Run("minimum_value", func(t *testing.T) {
		for i := 0; i < 100; i++ {
			result := numberFuzzy(1)
			if result < 1 {
				t.Errorf("numberFuzzy(1) = %d, want value >= 1", result)
			}
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
		{0, 100, 200, 100},
		{16, 100, 200, 150},
		{32, 100, 200, 200},
	}

	for _, tt := range tests {
		t.Run("", func(t *testing.T) {
			result := interpolate(tt.level, tt.value00, tt.value32)
			if result != tt.expected {
				t.Errorf("interpolate(%d, %d, %d) = %d, want %d",
					tt.level, tt.value00, tt.value32, result, tt.expected)
			}
		})
	}
}

