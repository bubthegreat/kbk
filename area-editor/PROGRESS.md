# KBK Area Editor - Development Progress

# Guidelines for AI Agents

1. When adding features, ensure that test.are is updated with a new room that supports that new feature.  The rooms should follow minimalistic standards that still follow a fantasy theme.  The main theme of the test area should follow the Legend of the Red Dragon MUD theme.  
3. When exceptions are found they shoudl be fixed without removing functionality that already existed without explicit user direction to do so.
4. When fixing exceptions, add new rooms that have specific conditions that cause the exception.  
5. When adding new features or updating existing ones, they should be added to a FEATURES.md file.  It should outline the existing features in a way that ensures AI knows which things that we have to make sure we keep when adjusting code. The format should be as follows:
    - Feature Name
    - Description of feature
    - How to test the feature
    - Any additional notes
    - End
6. When adding new they should be added in the standard uv format with uv add and use uv to regenerate the lock file.  Do not edit the lock file directly.
7. When adding new features, add new tests to ensure that the feature works as expected.  The tests should be added to the tests folder in the standard python unittest format.  The tests should be able to be run with uv run pytest -v.  If new dependencies are needed for the tests, add them with uv add and regenerate the lock file.  Do not edit the lock file directly.  Dependencies that are added specifically for testing and development and not use should be added with the --dev flag.

## Completed Tasks

### Phase 1: Foundation ✅
- [x] Set up Python project structure with UV package manager
- [x] Installed DearPyGui and development dependencies (ruff, pytest, pytest-xdist, pytest-cov)
- [x] Created basic UI scaffold with main window
- [x] Implemented menu bar with placeholder callbacks
- [x] Created area tree panel structure
- [x] Created editor panel structure
- [x] Created properties panel structure
- [x] Created status bar component
- [x] Defined data models for Area, Room, Object, Mobile

### Phase 2: File I/O and Parsing ✅
- [x] Created parser module structure
- [x] Implemented base parser class with utility methods
  - read_line(), read_word(), read_string(), read_number()
  - Support for tilde-terminated strings
  - Support for bit flag notation (e.g., "1|2|4")
- [x] Created main AreParser class
- [x] Implemented AREA section parser (#AREADATA)
  - Parses area name, builders, vnum range, security
- [x] Implemented ROOMS section parser (#ROOMDATA)
  - Parses room name, description, flags, sector type
  - Parses exits/doors with descriptions, keywords, locks
  - Parses extra descriptions (EDESC)
- [x] Implemented OBJECTS section parser (#OBJDATA)
  - Parses object name, descriptions, material
  - Parses TYPE line with item type and values
  - Parses EXTRA, WEAR, stats (LEVEL, WEIGHT, COST, COND)
  - Parses affects (Affect lines)
  - Parses extra descriptions (EDESC)
  - Handles LIMIT and RESTR fields
- [x] Implemented MOBILES section parser (#MOBDATA)
  - Parses mobile name, descriptions, race
  - Parses all flag types (ACT, AFF, OFF, IMM, RES, VULN)
  - Parses stats (LEVEL, ALIGN, GROUP, HROLL, etc.)
  - Parses dice notation (HDICE, MDICE, DDICE)
  - Parses AC, POS, SEX, GOLD, FORM, PARTS, SIZE
  - Parses DMOD, AMOD, QUEST, WSPEC fields
- [x] Added comprehensive parser tests with pytest
  - Tests for base parser utilities
  - Tests for area, room, mobile, and object parsing
  - All tests passing on limbo.are
- [ ] Implement RESETS section parser
- [ ] Implement SHOPS section parser
- [ ] Implement SPECIALS section parser
- [ ] Implement HELPS section parser

## Project Structure

```
area-editor/
├── src/
│   └── area_editor/
│       ├── __init__.py
│       ├── main.py                    # Application entry point
│       ├── models/                    # Data models
│       │   ├── __init__.py
│       │   ├── area.py               # Area model with vnum management
│       │   ├── room.py               # Room model with exits and extra descriptions
│       │   ├── object.py             # Object model with affects
│       │   └── mobile.py             # Mobile model
│       ├── parsers/                   # .are file parsers
│       │   ├── __init__.py
│       │   ├── base_parser.py        # Base parser with utilities
│       │   └── are_parser.py         # Main .are file parser
│       └── ui/                        # UI components
│           ├── __init__.py
│           ├── main_window.py        # Main window layout
│           ├── menu_bar.py           # Menu bar with File/Edit/View/Tools/Help
│           ├── area_tree.py          # Tree view for areas
│           ├── editor_panel.py       # Main editor panel
│           ├── properties_panel.py   # Properties sidebar
│           └── status_bar.py         # Status bar
├── tests/
│   └── test_parser.py                # Parser tests
├── pyproject.toml                    # Project configuration
├── README.md                         # Project documentation
├── PROGRESS.md                       # This file
└── uv.lock                           # Dependency lock file
```

## Key Features Implemented

### Data Models
- **Area**: Manages area metadata, vnum ranges, and collections of rooms/objects/mobiles
- **Room**: Supports exits in 6 directions, extra descriptions, room flags, sector types
- **Object**: Supports item types, value fields, extra descriptions, affects
- **Mobile**: Supports all mobile properties including stats, flags, dice notation

### Parser
- **BaseParser**: Provides low-level parsing utilities for the .are file format
  - Handles tilde-terminated strings
  - Supports bit flag notation with | operator
  - Line-by-line reading with position tracking
- **AreParser**: Coordinates parsing of different sections
  - Currently parses #AREADATA section
  - Framework in place for other sections

### UI Components
- **MainWindow**: Three-panel layout (tree, editor, properties) with menu and status bar
  - Implements open_area_file() to load and parse .are files
  - Coordinates between all UI components
- **MenuBar**: File, Edit, View, Tools, Help menus
  - File dialog for opening .are files
  - Connected to parser for loading areas
- **AreaTree**: Tree view for navigating areas
  - Populates from loaded Area data
  - Shows hierarchical structure (Area > Rooms/Objects/Mobiles)
  - Click handlers for item selection
- **EditorPanel**: Main editing area
  - Shows different editors based on selection
  - Room editor, Object editor, Mobile editor, Area editor
  - Currently read-only (edit functionality to be added)
- **PropertiesPanel**: Quick properties view (placeholder)
- **StatusBar**: Status messages and file info
  - Shows loading status and errors
  - Displays current file and item counts
- **AppState**: Global application state manager
  - Manages current area and file path
  - Tracks selected item and modification status

## Recent Bug Fixes

1. **Letter-Coded Lock Values**: Fixed parser to handle letter-coded lock values (e.g., 'AB', 'C') in room exits. Converts letter codes to bit values (A=1, B=2, C=4, etc.).

2. **Room Exit Display**: Fixed AttributeError when displaying room exits. Changed iteration from `for exit in room.exits:` to `for direction, exit in room.exits.items()` to properly access Exit objects.

3. **Selection Highlighting**: Fixed issue where multiple items could be highlighted simultaneously in the area tree. Now properly deselects previous item when new item is selected.

## Known Issues

1. **DearPyGui Library Compatibility**: The application requires system libraries (GLIBCXX_3.4.30) that may not be available on all systems. See README.md for installation instructions.

2. **Parser Format**: The current parser is designed for the newer .are format (with #AREADATA, #MOBDATA, etc.). The older format (with #AREA, #MOBILES, etc.) is not yet supported.

### Phase 3: UI Integration (In Progress) 🔄
- [x] Created application state manager (app_state.py)
  - Manages current area, file path, and selection state
  - Provides methods for loading areas and selecting items
  - Tracks modification status
- [x] Implemented file open dialog
  - Added file dialog to menu bar with .are file filter
  - Connected to File > Open Area menu item
  - Integrated parser to load area files
- [x] Connected parser to UI
  - MainWindow.open_area_file() method loads and parses files
  - Updates status bar with loading progress and results
  - Populates area tree with loaded data
- [x] Populated area tree with loaded data
  - AreaTree.populate_from_area() creates tree structure
  - Shows area info, rooms, objects, and mobiles
  - Displays item counts and vnums
- [x] Added area tree selection handling
  - Click callbacks on tree items
  - Updates app_state with selected item
  - Triggers editor panel updates
  - Properly deselects previous item when new item is selected
  - Tracks all selectable items for proper state management
- [x] Display selected item details in editor panel
  - Room editor shows name, description, exits, extra descriptions
  - **Clickable room links in exits** - click to navigate to connected rooms
  - Object editor shows descriptions, type, stats, affects
  - Mobile editor shows descriptions, race, level, stats
  - Area editor shows area info and statistics
- [x] Updated status bar with file info
  - Shows loaded file name
  - Displays item counts (rooms, objects, mobiles)
  - Shows modification status with asterisk
- [x] Add edit functionality to editor panels
  - **Room editor**: Editable name and description fields, displays extra descriptions with keywords and text
  - **Object editor**: Editable short and long description fields, displays extra descriptions with keywords and text
  - **Mobile editor**: Editable short and long description fields
  - **Area editor**: Editable area name field
  - All changes tracked with `app_state.mark_modified()`
  - Status bar shows asterisk (*) when file is modified
  - Real-time updates to data models on field changes
- [x] Implemented properties panel
  - **Room properties**: Shows exit count, extra descriptions, sector, flags, exit directions
  - **Object properties**: Shows type, level, weight, cost, material, extra descriptions, affects
  - **Mobile properties**: Shows race, level, alignment, combat stats (hit dice, damage, AC)
  - **Area properties**: Shows VNUM range, security, builders, content counts
  - Updates automatically when items are selected
  - Provides quick reference information alongside the main editor
  - **Note**: Extra descriptions are being parsed correctly (arkham.are: 46/67 objects, 4/174 rooms have EDESCs)
- [x] Implemented keyboard navigation
  - **Up Arrow (↑)**: Navigate to previous room in VNUM order
  - **Down Arrow (↓)**: Navigate to next room in VNUM order
  - Only active when a room is selected
  - Wraps around (last room → first room, first room → last room)
  - Updates tree selection, editor panel, and properties panel automatically
  - **Note**: Hotkeys work globally when not typing in a text field
  - **Bug fix**: Fixed TypeError when accessing app_state selection (was trying to unpack Room object)
- [x] Implemented MUD Terminal Simulator
  - **"Open Terminal" button** in room editor opens a MUD-like terminal
  - **Movement commands**: north (n), south (s), east (e), west (w), up (u), down (d)
  - **Look command**: look (shows current room), look <target> (looks at specific things)
  - **Extra descriptions**: Can look at room extra descriptions by keyword
  - **Room display**: Shows room name, description, and available exits
  - **Auto-show room**: Room is displayed automatically when terminal opens
  - **Auto-scroll**: Terminal automatically scrolls to bottom when new output is added
  - **Help command**: Shows available commands
  - **Quit command**: Closes the terminal window
  - **Command repeat**: Press Enter on empty input to repeat last command
  - **Auto-focus**: Input field stays focused after each command for continuous typing
  - **Auto-select**: Previous command stays highlighted - type to replace, Enter to repeat
  - **Color-coded output**: Room names (cyan), exits (gold), commands (green), errors (red), etc.
  - **Selectable text**: All terminal output is selectable for copy/paste
  - Provides a player's-eye view of the area for testing navigation and descriptions
  - Simplified simulator - no combat, timing, or lag mechanics
- [ ] Add area creation functionality
- [ ] Implement basic validation
- [ ] Implement .are file writer (to save changes)

## Next Steps

### Immediate (Phase 3 Continuation)
1. ~~Add edit functionality to editor panels (currently read-only)~~ ✅ DONE
2. Implement .are file writer to save changes
3. Add area creation functionality
4. Implement basic validation

### Short Term (Phase 4)
1. Build out full room editor UI with all fields
2. Build out full object editor UI with all fields
3. Build out full mobile editor UI with all fields
4. Add real-time validation

### Medium Term (Phases 5-6)
1. Implement RESETS section parser and editor
2. Implement SHOPS section parser and editor
3. Implement SPECIALS section parser and editor
4. Add map visualization
5. Add reference finding and validation

## Testing

Run tests with:
```bash
cd area-editor
uv run pytest -v
```

Current test coverage:
- Base parser utilities: ✅
- Area section parsing: ✅
- Room parsing: ✅
- Object parsing: ✅
- Mobile parsing: ✅

## Dependencies

- **dearpygui**: 2.1.1 - Immediate mode GUI framework
- **pytest**: 9.0.2 - Testing framework
- **pytest-xdist**: 3.8.0 - Parallel test execution
- **pytest-cov**: 7.0.0 - Code coverage
- **ruff**: 0.14.13 - Linting and formatting

## Notes

- The project uses UV for package management instead of pip
- All code follows modern Python practices with type hints
- Data models use dataclasses for clean, maintainable code
- Parser is designed to be extensible for future format changes

