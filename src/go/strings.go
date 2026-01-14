package main

import "C"
import (
	"strings"
	"unicode"
	"unsafe"
)

// StrCmp performs case-insensitive string comparison.
// Returns 1 (true) if strings are different, 0 (false) if they are the same.
// This matches the C convention where str_cmp returns TRUE if different.
// Matches the C function: bool str_cmp(const char *astr, const char *bstr)
//
//export StrCmp
func StrCmp(astr *C.char, bstr *C.char) C.int {
	// Handle NULL pointers
	if astr == nil || bstr == nil {
		return 1 // Different (TRUE in C)
	}

	a := C.GoString(astr)
	b := C.GoString(bstr)

	// Case-insensitive comparison
	if strings.EqualFold(a, b) {
		return 0 // Same (FALSE in C)
	}

	return 1 // Different (TRUE in C)
}

// StrPrefix checks if astr is a case-insensitive prefix of bstr.
// Returns 1 (true) if astr is NOT a prefix of bstr, 0 (false) if it is.
// This matches the C convention.
// Matches the C function: bool str_prefix(const char *astr, const char *bstr)
//
//export StrPrefix
func StrPrefix(astr *C.char, bstr *C.char) C.int {
	// Handle NULL pointers
	if astr == nil || bstr == nil {
		return 1 // Not a prefix (TRUE in C)
	}

	a := strings.ToLower(C.GoString(astr))
	b := strings.ToLower(C.GoString(bstr))

	// Check if a is a prefix of b
	if strings.HasPrefix(b, a) {
		return 0 // Is a prefix (FALSE in C)
	}

	return 1 // Not a prefix (TRUE in C)
}

// StrSuffix checks if astr is a case-insensitive suffix of bstr.
// Returns 1 (true) if astr is NOT a suffix of bstr, 0 (false) if it is.
// This matches the C convention.
// Matches the C function: bool str_suffix(const char *astr, const char *bstr)
//
//export StrSuffix
func StrSuffix(astr *C.char, bstr *C.char) C.int {
	// Handle NULL pointers
	if astr == nil || bstr == nil {
		return 1 // Not a suffix (TRUE in C)
	}

	a := strings.ToLower(C.GoString(astr))
	b := strings.ToLower(C.GoString(bstr))

	// Check if a is a suffix of b
	if strings.HasSuffix(b, a) {
		return 0 // Is a suffix (FALSE in C)
	}

	return 1 // Not a suffix (TRUE in C)
}

// StrInfix checks if astr appears anywhere in bstr (case-insensitive).
// Returns 1 (true) if astr is NOT found in bstr, 0 (false) if it is found.
// This matches the C convention.
// Matches the C function: bool str_infix(const char *astr, const char *bstr)
//
//export StrInfix
func StrInfix(astr *C.char, bstr *C.char) C.int {
	// Handle NULL pointers
	if astr == nil || bstr == nil {
		return 1 // Not found (TRUE in C)
	}

	a := strings.ToLower(C.GoString(astr))
	b := strings.ToLower(C.GoString(bstr))

	// Check if a is contained in b
	if strings.Contains(b, a) {
		return 0 // Found (FALSE in C)
	}

	return 1 // Not found (TRUE in C)
}

// Capitalize returns a string with the first letter capitalized and the rest lowercase.
// Note: This returns a pointer to a static buffer, matching C behavior.
// The caller should not free this pointer.
// Matches the C function: char *capitalize(const char *str)
//
//export Capitalize
func Capitalize(str *C.char) *C.char {
	if str == nil {
		return C.CString("")
	}

	s := C.GoString(str)
	if len(s) == 0 {
		return C.CString("")
	}

	// Convert to lowercase
	lower := strings.ToLower(s)

	// Capitalize first character
	runes := []rune(lower)
	runes[0] = unicode.ToUpper(runes[0])

	result := string(runes)
	return C.CString(result)
}

// SmashTilde replaces all tilde characters (~) with dashes (-).
// This modifies the string in place.
// Matches the C function: void smash_tilde(char *str)
//
//export SmashTilde
func SmashTilde(str *C.char) {
	if str == nil {
		return
	}

	// Get the Go string
	s := C.GoString(str)

	// Replace tildes with dashes
	modified := strings.ReplaceAll(s, "~", "-")

	// Copy back to C string (in place modification)
	// Note: This assumes the C string has enough space
	for i := 0; i < len(modified); i++ {
		*(*C.char)(unsafe.Pointer(uintptr(unsafe.Pointer(str)) + uintptr(i))) = C.char(modified[i])
	}
	// Null terminate
	*(*C.char)(unsafe.Pointer(uintptr(unsafe.Pointer(str)) + uintptr(len(modified)))) = 0
}
