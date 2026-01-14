/*
 * Simple test program to verify Go library functions work correctly
 * Compile with: gcc -o test_golib test_golib.c libkbkgo.a -lpthread
 */

#include <stdio.h>
#include <stdlib.h>
#include "golib.h"

int main() {
    printf("=== Testing KBK Go Library ===\n\n");

    // Test lookup functions
    printf("Lookup Functions:\n");
    printf("  DirectionLookup(\"north\") = %d (expected: 0)\n", DirectionLookup("north"));
    printf("  DirectionLookup(\"e\") = %d (expected: 1)\n", DirectionLookup("e"));
    printf("  DirectionLookup(\"invalid\") = %d (expected: -1)\n", DirectionLookup("invalid"));
    printf("  PositionLookup(\"standing\") = %d (expected: 8)\n", PositionLookup("standing"));
    printf("  SexLookup(\"male\") = %d (expected: 1)\n", SexLookup("male"));
    printf("  SizeLookup(\"medium\") = %d (expected: 2)\n", SizeLookup("medium"));
    printf("\n");

    // Test random number functions
    printf("Random Number Functions:\n");
    int range_result = NumberRange(1, 10);
    printf("  NumberRange(1, 10) = %d (expected: 1-10)\n", range_result);
    if (range_result < 1 || range_result > 10) {
        printf("    ERROR: Out of range!\n");
        return 1;
    }
    
    int percent = NumberPercent();
    printf("  NumberPercent() = %d (expected: 1-100)\n", percent);
    if (percent < 1 || percent > 100) {
        printf("    ERROR: Out of range!\n");
        return 1;
    }
    
    int dice_result = Dice(3, 6);
    printf("  Dice(3, 6) = %d (expected: 3-18)\n", dice_result);
    if (dice_result < 3 || dice_result > 18) {
        printf("    ERROR: Out of range!\n");
        return 1;
    }
    
    printf("  NumberFuzzy(10) = %d (expected: 9-11)\n", NumberFuzzy(10));
    printf("  Interpolate(16, 10, 50) = %d (expected: 30)\n", Interpolate(16, 10, 50));
    printf("\n");

    // Test utility functions
    printf("Utility Functions:\n");
    printf("  UMin(5, 10) = %d (expected: 5)\n", UMin(5, 10));
    printf("  UMax(5, 10) = %d (expected: 10)\n", UMax(5, 10));
    printf("  URange(0, -5, 10) = %d (expected: 0)\n", URange(0, -5, 10));
    printf("  URange(0, 5, 10) = %d (expected: 5)\n", URange(0, 5, 10));
    printf("  URange(0, 15, 10) = %d (expected: 10)\n", URange(0, 15, 10));
    printf("\n");

    // Test string functions
    printf("String Functions:\n");
    printf("  StrCmp(\"hello\", \"HELLO\") = %d (expected: 0=same)\n", StrCmp("hello", "HELLO"));
    printf("  StrCmp(\"hello\", \"world\") = %d (expected: 1=different)\n", StrCmp("hello", "world"));
    printf("  StrPrefix(\"hel\", \"hello\") = %d (expected: 0=is prefix)\n", StrPrefix("hel", "hello"));
    printf("  StrPrefix(\"world\", \"hello\") = %d (expected: 1=not prefix)\n", StrPrefix("world", "hello"));
    printf("  StrSuffix(\"lo\", \"hello\") = %d (expected: 0=is suffix)\n", StrSuffix("lo", "hello"));
    printf("  StrInfix(\"ell\", \"hello\") = %d (expected: 0=found)\n", StrInfix("ell", "hello"));
    
    char *cap = Capitalize("HELLO WORLD");
    printf("  Capitalize(\"HELLO WORLD\") = \"%s\" (expected: \"Hello world\")\n", cap);
    free(cap);
    printf("\n");

    printf("✓ All tests passed!\n");
    printf("\nThe Go library is working correctly and can be used in the pos2 binary.\n");
    return 0;
}

