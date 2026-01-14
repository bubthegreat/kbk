package main

import "C"
import (
	"strings"
	"unicode"
	"unsafe"
)

// strCmp is the internal implementation
// Returns true if strings are different, false if they are the same
func strCmp(a, b string) bool {
	return !strings.EqualFold(a, b)
}

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

	if strCmp(C.GoString(astr), C.GoString(bstr)) {
		return 1 // Different (TRUE in C)
	}
	return 0 // Same (FALSE in C)
}

// strPrefix is the internal implementation
// Returns true if a is NOT a prefix of b, false if it is
func strPrefix(a, b string) bool {
	return !strings.HasPrefix(strings.ToLower(b), strings.ToLower(a))
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

	if strPrefix(C.GoString(astr), C.GoString(bstr)) {
		return 1 // Not a prefix (TRUE in C)
	}
	return 0 // Is a prefix (FALSE in C)
}

// strSuffix is the internal implementation
// Returns true if a is NOT a suffix of b, false if it is
func strSuffix(a, b string) bool {
	return !strings.HasSuffix(strings.ToLower(b), strings.ToLower(a))
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

	if strSuffix(C.GoString(astr), C.GoString(bstr)) {
		return 1 // Not a suffix (TRUE in C)
	}
	return 0 // Is a suffix (FALSE in C)
}

// strInfix is the internal implementation
// Returns true if a is NOT found in b, false if it is found
func strInfix(a, b string) bool {
	return !strings.Contains(strings.ToLower(b), strings.ToLower(a))
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

	if strInfix(C.GoString(astr), C.GoString(bstr)) {
		return 1 // Not found (TRUE in C)
	}
	return 0 // Found (FALSE in C)
}

// capitalize is the internal implementation
func capitalize(s string) string {
	if len(s) == 0 {
		return ""
	}

	// Convert to lowercase
	lower := strings.ToLower(s)

	// Capitalize first character
	runes := []rune(lower)
	runes[0] = unicode.ToUpper(runes[0])

	return string(runes)
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

	result := capitalize(C.GoString(str))
	return C.CString(result)
}

// smashTilde is the internal implementation
func smashTilde(s string) string {
	return strings.ReplaceAll(s, "~", "-")
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
	modified := smashTilde(s)

	// Copy back to C string (in place modification)
	// Note: This assumes the C string has enough space
	for i := 0; i < len(modified); i++ {
		*(*C.char)(unsafe.Pointer(uintptr(unsafe.Pointer(str)) + uintptr(i))) = C.char(modified[i])
	}
	// Null terminate
	*(*C.char)(unsafe.Pointer(uintptr(unsafe.Pointer(str)) + uintptr(len(modified)))) = 0
}
