package main

import (
	"testing"
)

func TestStrCmp(t *testing.T) {
	tests := []struct {
		a        string
		b        string
		expected bool // true if different
	}{
		{"hello", "hello", false},
		{"Hello", "hello", false},
		{"HELLO", "hello", false},
		{"hello", "world", true},
		{"test", "testing", true},
		{"", "", false},
		{"a", "b", true},
	}

	for _, tt := range tests {
		t.Run("", func(t *testing.T) {
			result := strCmp(tt.a, tt.b)
			if result != tt.expected {
				t.Errorf("strCmp(%q, %q) = %v, want %v", tt.a, tt.b, result, tt.expected)
			}
		})
	}
}

func TestStrPrefix(t *testing.T) {
	tests := []struct {
		prefix   string
		str      string
		expected bool // true if NOT a prefix
	}{
		{"hel", "hello", false},
		{"HEL", "hello", false},
		{"hello", "hello", false},
		{"world", "hello", true},
		{"hello", "hel", true},
		{"", "hello", false},
	}

	for _, tt := range tests {
		t.Run("", func(t *testing.T) {
			result := strPrefix(tt.prefix, tt.str)
			if result != tt.expected {
				t.Errorf("strPrefix(%q, %q) = %v, want %v", tt.prefix, tt.str, result, tt.expected)
			}
		})
	}
}

func TestStrSuffix(t *testing.T) {
	tests := []struct {
		suffix   string
		str      string
		expected bool // true if NOT a suffix
	}{
		{"lo", "hello", false},
		{"LO", "hello", false},
		{"hello", "hello", false},
		{"world", "hello", true},
		{"hello", "lo", true},
		{"", "hello", false},
	}

	for _, tt := range tests {
		t.Run("", func(t *testing.T) {
			result := strSuffix(tt.suffix, tt.str)
			if result != tt.expected {
				t.Errorf("strSuffix(%q, %q) = %v, want %v", tt.suffix, tt.str, result, tt.expected)
			}
		})
	}
}

func TestStrInfix(t *testing.T) {
	tests := []struct {
		infix    string
		str      string
		expected bool // true if NOT found
	}{
		{"ell", "hello", false},
		{"ELL", "hello", false},
		{"world", "hello", true},
		{"", "hello", false},
		{"hello", "hello", false},
	}

	for _, tt := range tests {
		t.Run("", func(t *testing.T) {
			result := strInfix(tt.infix, tt.str)
			if result != tt.expected {
				t.Errorf("strInfix(%q, %q) = %v, want %v", tt.infix, tt.str, result, tt.expected)
			}
		})
	}
}

func TestCapitalize(t *testing.T) {
	tests := []struct {
		input    string
		expected string
	}{
		{"hello", "Hello"},
		{"HELLO", "Hello"},
		{"hELLO", "Hello"},
		{"a", "A"},
		{"", ""},
		{"test string", "Test string"},
	}

	for _, tt := range tests {
		t.Run("", func(t *testing.T) {
			result := capitalize(tt.input)
			if result != tt.expected {
				t.Errorf("capitalize(%q) = %q, want %q", tt.input, result, tt.expected)
			}
		})
	}
}

func TestSmashTilde(t *testing.T) {
	tests := []struct {
		input    string
		expected string
	}{
		{"hello~world", "hello-world"},
		{"test~~string", "test--string"},
		{"no tildes", "no tildes"},
		{"~start", "-start"},
		{"end~", "end-"},
		{"", ""},
	}

	for _, tt := range tests {
		t.Run("", func(t *testing.T) {
			result := smashTilde(tt.input)
			if result != tt.expected {
				t.Errorf("smashTilde(%q) = %q, want %q", tt.input, result, tt.expected)
			}
		})
	}
}

