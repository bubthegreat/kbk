# Buffer Overflow Warnings TODO

This file tracks buffer overflow warnings that need to be addressed. These are all related to `sprintf` calls that could potentially overflow their destination buffers.

## Priority: HIGH - Active Security Risks

### act_comm.c
- **Line 686**: `sprintf(buf, " (%s) [%d]: %s", timestamp(), level, buf2);`
  - Risk: buf2 (4607 bytes) + timestamp + formatting could exceed buf size (4608)
  - Fix: Use `snprintf(buf, sizeof(buf), ...)` instead
  
- **Line 688**: `sprintf(buf, " (%s): %s", timestamp(), buf2);`
  - Risk: Same as above
  - Fix: Use `snprintf(buf, sizeof(buf), ...)` instead

- **Line 692, 709**: `sprintf(buf2, "{g[IMM] %s%s{x\n\r", ch->original_name, buf);`
  - Risk: original_name + buf could exceed buf2 size
  - Fix: Use `snprintf(buf2, sizeof(buf2), ...)` instead

### act_enter.c
- **Lines 300-302, 306-307, 319**: Path building with sprintf
  - Risk: Concatenating direction strings could overflow
  - Fix: Use `snprintf` and check return values

- **Lines 323-326, 331-334**: Path display messages
  - Risk: Combining path string with area name/victim name
  - Fix: Use `snprintf` with proper size limits

### act_wiz.c
- **Line 2793-2795**: `sprintf(buf, "%s: '%s' %s%s%s", buf2, ...)`
  - Risk: Multiple string concatenations
  - Fix: Use `snprintf` with size check

- **Line 6256**: `sprintf(buf, "finger %s history", arg1);`
  - Risk: arg1 could be very long
  - Fix: Use `snprintf` and validate arg1 length

### ban.c
- **Lines 162-170**: Ban list formatting
  - Risk: Multiple fields being formatted together
  - Fix: Use `snprintf` with proper bounds

### comm.c
- **Lines 3234, 3254**: `sprintf(rbuf, "{c%s{x", buf);`
  - Risk: buf (4607 bytes) + color codes could exceed rbuf
  - Fix: Use `snprintf(rbuf, sizeof(rbuf), ...)` instead

### db.c
- **Line 4518**: `sprintf(pbuf, "ls %s > %s", catplr, PLAYER_LIST);`
  - Risk: catplr (255 bytes) + path could exceed pbuf (100 bytes)
  - Fix: Increase pbuf size or use `snprintf` with validation

## Priority: MEDIUM - Display/Formatting Issues

### extra.c
- **Line 782**: Site tracking string concatenation
- **Lines 864-865**: Player file path construction
- **Line 1227**: Object tracking display
- **Lines 2703, 2709**: Divine intervention messages
- **Lines 2747-2750, 2752-2755, 2766-2774**: Cabal channel messages

### morecabal.c
- **Lines 492, 495**: Duel messages
- **Lines 929, 948, 967**: Command formatting

### olc.c
- **Lines 1535, 1549, 1563**: Prog setting messages

### queststore.c
- **Lines 121**: Reward description display
- **Multiple lines (171-537)**: Quest store item formatting with credits
  - Pattern: `sprintf(buf, "%-24s %s%5d credits...", name, spacing, price);`
  - Risk: Complex spacing calculations could overflow
  - Fix: Use `snprintf` and simplify spacing logic

### string.c
- **Line 623**: `sprintf(buf2, "%2d. %s\n\r", cnt++, tmpb);`
  - Risk: tmpb could be very long
  - Fix: Use `snprintf` with size limit

### magic.c
- **Line 235**: `sprintf(buf2, "$n utters the words, '%s'.", buf);`
  - Risk: buf (4607 bytes) + message text
  - Fix: Use `snprintf` with proper size

## Recommended Approach

1. **Create a safe sprintf wrapper**: Consider creating a `safe_sprintf` macro or function that automatically uses `snprintf` with buffer size
2. **Increase buffer sizes where needed**: Some buffers (like pbuf in db.c) are too small for their intended use
3. **Validate input lengths**: Before formatting, check that input strings won't cause overflow
4. **Use string truncation**: When displaying to users, it's acceptable to truncate long strings rather than overflow

## Notes

- MAX_STRING_LENGTH appears to be 4608 bytes based on compiler warnings
- Most warnings show potential overflow of 1-30 bytes, suggesting buffers are close to correct size
- The real risk is when user input or dynamic content is involved
- Display-only overflows are lower priority than command/system call overflows (like db.c line 4518)

