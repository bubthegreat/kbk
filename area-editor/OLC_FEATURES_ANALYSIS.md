# OLC Features Analysis for Area Editor

This document analyzes ROM MUD OLC (Online Creation) features and recommends which ones to implement in the area editor for basic area creation.

## Current Implementation Status

### ✅ Already Implemented
- **dig command**: Create new rooms or link to existing rooms with bidirectional exits
- **Room navigation**: Move between rooms in terminal
- **Room viewing**: Display room name, description, exits
- **Look command**: View extra descriptions
- **File parsing**: Read .are files with rooms, objects, mobiles
- **File writing**: Save changes to .are files
- **Multi-area support**: Load and edit multiple areas

### 🔨 Recommended for Basic Area Creation

#### 1. **Exit/Door Management** (HIGH PRIORITY)
Commands from OLC: `north/south/east/west/up/down <command> <args>`

**Exit Flags** (from `src/merc.h` lines 1833-1844):
- `door` (EX_ISDOOR) - Exit has a door
- `closed` (EX_CLOSED) - Door is closed
- `locked` (EX_LOCKED) - Door is locked
- `pickproof` (EX_PICKPROOF) - Lock cannot be picked
- `nonobvious` (EX_NONOBVIOUS) - Exit is not obvious/hidden
- `nopass` (EX_NOPASS) - Cannot pass through
- `easy/hard/infuriating` - Pick difficulty

**Recommended Terminal Commands**:
```
north door          - Toggle door flag on north exit
north locked        - Toggle locked flag
north pickproof     - Toggle pickproof flag
north nonobvious    - Toggle hidden exit flag
north key <vnum>    - Set key object for locked door
north desc          - Edit door description (what you see when looking at the door)
north keywords <text> - Set door keywords (for open/close/unlock commands)
north delete        - Remove exit entirely
```

**Why**: Essential for creating locked doors, hidden passages, and basic area security.

#### 2. **Room Editing** (HIGH PRIORITY)
Commands from OLC: `name`, `desc`, `ed`

**Recommended Terminal Commands**:
```
rname <text>        - Set room name
rdesc               - Edit room description (multi-line editor)
ed add <keywords>   - Add extra description
ed edit <keywords>  - Edit existing extra description
ed delete <keywords> - Delete extra description
```

**Why**: Currently can only edit via properties panel. Terminal editing is faster for builders.

#### 3. **Room Flags** (MEDIUM PRIORITY)
Commands from OLC: Room flags toggle

**Common Room Flags** (from `src/merc.h` lines 1791-1813):
- `dark` - Room is dark (need light)
- `indoors` - Room is indoors
- `safe` - No combat allowed
- `private` - Limit number of players
- `no_recall` - Cannot recall from room
- `no_gate` - Cannot gate to/from room
- `no_summon` - Cannot summon to/from room

**Recommended Terminal Commands**:
```
rflag <flag>        - Toggle room flag
rflags              - Show current room flags
```

**Why**: Important for area design (safe rooms, dark dungeons, etc.)

#### 4. **Sector Types** (MEDIUM PRIORITY)
Commands from OLC: Sector type setting

**Common Sector Types** (from constants):
- Inside, City, Field, Forest, Hills, Mountain
- Water (swim/boat), Air (fly), Desert

**Recommended Terminal Commands**:
```
sector <type>       - Set room sector type
```

**Why**: Affects movement, weather, and gameplay mechanics.

### ❌ NOT Recommended for Initial Implementation

#### Object/Mobile Creation in Terminal
- **Why**: Complex, better suited for GUI properties panel
- **Alternative**: Use properties panel for detailed editing

#### Resets in Terminal
- **Why**: Complex relationships, better in GUI
- **Alternative**: Use properties panel for reset management

#### Special Programs (MOBPROGs, etc.)
- **Why**: Advanced feature, not needed for basic areas
- **Alternative**: Add later if needed

#### Room Affects/Spells
- **Why**: Advanced feature
- **Alternative**: Add later if needed

## Implementation Priority

### Phase 1: Door/Exit Management (HIGHEST VALUE)
1. Exit flag toggling (door, locked, pickproof, nonobvious)
2. Key assignment for locked doors
3. Door keywords and descriptions
4. Exit deletion

**Estimated Effort**: Medium
**Value**: Very High - Essential for any interesting area

### Phase 2: Room Editing
1. Room name editing
2. Room description editing
3. Extra description management

**Estimated Effort**: Medium
**Value**: High - Speeds up area building significantly

### Phase 3: Room Properties
1. Room flags
2. Sector types

**Estimated Effort**: Low
**Value**: Medium - Nice to have, but can use GUI

## Technical Notes

### Exit Flag Storage
- Stored in `Exit.locks` field (integer bitfield)
- Need to map bit positions to flag names
- Already defined in `area-editor/src/area_editor/constants.py` lines 51-66

### Door Keywords
- Stored in `Exit.keywords` field (string)
- Used for "open door", "unlock gate", etc.

### Door Descriptions
- Stored in `Exit.description` field (string)
- Shown when you look in that direction

### Key Objects
- Stored in `Exit.key_vnum` field (integer)
- References an object vnum that can unlock the door

## Recommendation Summary

**For basic area creation, implement Phase 1 (Door/Exit Management) first.**

This gives builders the ability to:
- Create locked doors with keys
- Make hidden passages
- Add door descriptions
- Control access between rooms

This is the most impactful feature for creating interesting areas with minimal implementation effort.

