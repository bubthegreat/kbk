package main

import (
	"testing"
	"unsafe"
)

/*
#include <stdlib.h>
*/
import "C"

func TestStrCmp(t *testing.T) {
	tests := []struct {
		a        string
		b        string
		expected int // 0 = same, 1 = different
	}{
		{"hello", "hello", 0},
		{"hello", "HELLO", 0},
		{"Hello", "hello", 0},
		{"hello", "world", 1},
		{"test", "testing", 1},
		{"", "", 0},
	}

	for _, tt := range tests {
		t.Run(tt.a+"_vs_"+tt.b, func(t *testing.T) {
			ca := C.CString(tt.a)
			cb := C.CString(tt.b)
			defer C.free(unsafe.Pointer(ca))
			defer C.free(unsafe.Pointer(cb))

			result := StrCmp(ca, cb)
			if int(result) != tt.expected {
				t.Errorf("StrCmp(%q, %q) = %d, want %d", tt.a, tt.b, result, tt.expected)
			}
		})
	}

	// Test nil pointers
	t.Run("nil_pointers", func(t *testing.T) {
		if StrCmp(nil, nil) != 1 {
			t.Error("StrCmp(nil, nil) should return 1 (different)")
		}
		ca := C.CString("test")
		defer C.free(unsafe.Pointer(ca))
		if StrCmp(nil, ca) != 1 {
			t.Error("StrCmp(nil, str) should return 1 (different)")
		}
		if StrCmp(ca, nil) != 1 {
			t.Error("StrCmp(str, nil) should return 1 (different)")
		}
	})
}

func TestStrPrefix(t *testing.T) {
	tests := []struct {
		prefix   string
		str      string
		expected int // 0 = is prefix, 1 = not prefix
	}{
		{"hel", "hello", 0},
		{"HEL", "hello", 0},
		{"hello", "hello", 0},
		{"world", "hello", 1},
		{"hello", "hel", 1},
		{"", "hello", 0}, // Empty string is prefix of anything
	}

	for _, tt := range tests {
		t.Run(tt.prefix+"_in_"+tt.str, func(t *testing.T) {
			cprefix := C.CString(tt.prefix)
			cstr := C.CString(tt.str)
			defer C.free(unsafe.Pointer(cprefix))
			defer C.free(unsafe.Pointer(cstr))

			result := StrPrefix(cprefix, cstr)
			if int(result) != tt.expected {
				t.Errorf("StrPrefix(%q, %q) = %d, want %d", tt.prefix, tt.str, result, tt.expected)
			}
		})
	}
}

func TestStrSuffix(t *testing.T) {
	tests := []struct {
		suffix   string
		str      string
		expected int // 0 = is suffix, 1 = not suffix
	}{
		{"lo", "hello", 0},
		{"LO", "hello", 0},
		{"hello", "hello", 0},
		{"world", "hello", 1},
		{"hello", "lo", 1},
		{"", "hello", 0}, // Empty string is suffix of anything
	}

	for _, tt := range tests {
		t.Run(tt.suffix+"_in_"+tt.str, func(t *testing.T) {
			csuffix := C.CString(tt.suffix)
			cstr := C.CString(tt.str)
			defer C.free(unsafe.Pointer(csuffix))
			defer C.free(unsafe.Pointer(cstr))

			result := StrSuffix(csuffix, cstr)
			if int(result) != tt.expected {
				t.Errorf("StrSuffix(%q, %q) = %d, want %d", tt.suffix, tt.str, result, tt.expected)
			}
		})
	}
}

func TestStrInfix(t *testing.T) {
	tests := []struct {
		infix    string
		str      string
		expected int // 0 = found, 1 = not found
	}{
		{"ell", "hello", 0},
		{"ELL", "hello", 0},
		{"world", "hello", 1},
		{"", "hello", 0}, // Empty string is in anything
	}

	for _, tt := range tests {
		t.Run(tt.infix+"_in_"+tt.str, func(t *testing.T) {
			cinfix := C.CString(tt.infix)
			cstr := C.CString(tt.str)
			defer C.free(unsafe.Pointer(cinfix))
			defer C.free(unsafe.Pointer(cstr))

			result := StrInfix(cinfix, cstr)
			if int(result) != tt.expected {
				t.Errorf("StrInfix(%q, %q) = %d, want %d", tt.infix, tt.str, result, tt.expected)
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
		{"test string", "Test string"},
		{"", ""},
	}

	for _, tt := range tests {
		t.Run(tt.input, func(t *testing.T) {
			cinput := C.CString(tt.input)
			defer C.free(unsafe.Pointer(cinput))

			result := Capitalize(cinput)
			defer C.free(unsafe.Pointer(result))

			resultStr := C.GoString(result)
			if resultStr != tt.expected {
				t.Errorf("Capitalize(%q) = %q, want %q", tt.input, resultStr, tt.expected)
			}
		})
	}

	// Test nil pointer
	t.Run("nil", func(t *testing.T) {
		result := Capitalize(nil)
		defer C.free(unsafe.Pointer(result))
		resultStr := C.GoString(result)
		if resultStr != "" {
			t.Errorf("Capitalize(nil) = %q, want empty string", resultStr)
		}
	})
}

