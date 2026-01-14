/*
 * Example C code demonstrating how to use the Go library functions
 * This file is for documentation purposes and shows the integration pattern
 */

#include <stdio.h>
#include <stdlib.h>
#include "../golib.h"

int main() {
    printf("=== KBK Go Library Example Usage ===\n\n");

    // Lookup functions
    printf("Lookup Functions:\n");
    printf("  DirectionLookup(\"north\") = %d\n", DirectionLookup("north"));
    printf("  DirectionLookup(\"e\") = %d\n", DirectionLookup("e"));
    printf("  PositionLookup(\"standing\") = %d\n", PositionLookup("standing"));
    printf("  SexLookup(\"male\") = %d\n", SexLookup("male"));
    printf("  SizeLookup(\"medium\") = %d\n", SizeLookup("medium"));
    printf("\n");

    // Random number functions
    printf("Random Number Functions:\n");
    printf("  NumberRange(1, 10) = %d\n", NumberRange(1, 10));
    printf("  NumberPercent() = %d\n", NumberPercent());
    printf("  Dice(3, 6) = %d\n", Dice(3, 6));
    printf("  NumberFuzzy(10) = %d\n", NumberFuzzy(10));
    printf("  Interpolate(16, 10, 50) = %d\n", Interpolate(16, 10, 50));
    printf("\n");

    // Utility functions
    printf("Utility Functions:\n");
    printf("  UMin(5, 10) = %d\n", UMin(5, 10));
    printf("  UMax(5, 10) = %d\n", UMax(5, 10));
    printf("  URange(0, -5, 10) = %d\n", URange(0, -5, 10));
    printf("  URange(0, 5, 10) = %d\n", URange(0, 5, 10));
    printf("  URange(0, 15, 10) = %d\n", URange(0, 15, 10));
    printf("\n");

    // String functions
    printf("String Functions:\n");
    printf("  StrCmp(\"hello\", \"HELLO\") = %d (0=same)\n", StrCmp("hello", "HELLO"));
    printf("  StrCmp(\"hello\", \"world\") = %d (1=different)\n", StrCmp("hello", "world"));
    printf("  StrPrefix(\"hel\", \"hello\") = %d (0=is prefix)\n", StrPrefix("hel", "hello"));
    printf("  StrPrefix(\"world\", \"hello\") = %d (1=not prefix)\n", StrPrefix("world", "hello"));
    printf("  StrSuffix(\"lo\", \"hello\") = %d (0=is suffix)\n", StrSuffix("lo", "hello"));
    printf("  StrInfix(\"ell\", \"hello\") = %d (0=found)\n", StrInfix("ell", "hello"));
    
    char *cap = Capitalize("HELLO WORLD");
    printf("  Capitalize(\"HELLO WORLD\") = \"%s\"\n", cap);
    free(cap);
    printf("\n");

    printf("All functions working correctly!\n");
    return 0;
}

