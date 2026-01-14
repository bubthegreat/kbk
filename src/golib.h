/***************************************************************************
 *  KBK Go Library Interface                                               *
 *  This header provides C declarations for Go-implemented functions       *
 *                                                                         *
 *  This file is a wrapper around the auto-generated libkbkgo.h            *
 *  It provides a cleaner interface and documentation for C code.          *
 ***************************************************************************/

#ifndef GOLIB_H
#define GOLIB_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include the auto-generated Go header */
#include "libkbkgo.h"

/*
 * Lookup Functions
 * These functions provide table lookups for various game constants
 */

/* Convert direction string to numeric value (0-5 for n,e,s,w,u,d)
 * Returns -1 if direction is not recognized
 */
extern int DirectionLookup(char *dir);

/* Look up position by name (dead, sleeping, standing, etc.)
 * Returns position index or -1 if not found
 */
extern int PositionLookup(char *name);

/* Look up sex by name (none, male, female, either)
 * Returns sex index or -1 if not found
 */
extern int SexLookup(char *name);

/* Look up size by name (tiny, small, medium, large, huge, giant)
 * Returns size index or -1 if not found
 */
extern int SizeLookup(char *name);

/*
 * Random Number Functions
 * These functions provide random number generation for game mechanics
 */

/* Generate random number in range [from, to] inclusive */
extern int NumberRange(int from, int to);

/* Generate percentile roll (1-100) */
extern int NumberPercent(void);

/* Generate random door direction (0-5) */
extern int NumberDoor(void);

/* Generate random number with specified number of bits */
extern int NumberBits(int width);

/* Roll dice (e.g., Dice(3, 6) rolls 3d6) */
extern int Dice(int number, int size);

/* Add small random variation to a number (-1, 0, or +1) */
extern int NumberFuzzy(int number);

/* Linear interpolation between two values based on level (0-32) */
extern int Interpolate(int level, int value_00, int value_32);

/* Return minimum of two integers */
extern int UMin(int a, int b);

/* Return maximum of two integers */
extern int UMax(int a, int b);

/* Clamp value between min and max */
extern int URange(int min, int val, int max);

/*
 * String Utility Functions
 * These functions provide string manipulation and comparison
 */

/* Case-insensitive string comparison
 * Returns 0 if strings are equal, 1 if different
 * Note: This matches C convention where TRUE means different
 */
extern int StrCmp(char *astr, char *bstr);

/* Case-insensitive prefix check
 * Returns 0 if astr is a prefix of bstr, 1 if not
 */
extern int StrPrefix(char *astr, char *bstr);

/* Case-insensitive suffix check
 * Returns 0 if astr is a suffix of bstr, 1 if not
 */
extern int StrSuffix(char *astr, char *bstr);

/* Case-insensitive substring check
 * Returns 0 if astr is found in bstr, 1 if not
 */
extern int StrInfix(char *astr, char *bstr);

/* Capitalize first letter, lowercase the rest
 * Returns pointer to static buffer (do not free)
 */
extern char *Capitalize(char *str);

/* Replace all tilde (~) characters with dashes (-)
 * Modifies string in place
 */
extern void SmashTilde(char *str);

#ifdef __cplusplus
}
#endif

#endif /* GOLIB_H */

