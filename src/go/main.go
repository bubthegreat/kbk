package main

import "C"

// This file serves as the entry point for the Go library.
// All exported functions are defined in their respective files:
// - lookup.go: DirectionLookup, PositionLookup, SexLookup, SizeLookup
// - random.go: NumberRange, NumberPercent, Dice, NumberFuzzy, etc.
// - strings.go: StrCmp, StrPrefix, StrSuffix, Capitalize, etc.

// main is required for buildmode=c-archive but is never called
func main() {}

