# Area/Object Creation Tool Requirements

## Overview
This document outlines the requirements for a modern GUI-based area/object creation tool for the KBK MUD. The tool will provide a user-friendly interface for creating and editing areas, rooms, objects, and mobiles, replacing the need for in-game OLC commands or manual text file editing.

## Background
The existing OLC (Online Creation) system in KBK allows builders to create content in-game through text commands. While functional, this approach has limitations:
- Requires being connected to the running MUD
- Text-based interface can be cumbersome for complex editing
- No visual representation of area layouts
- Difficult to see relationships between rooms, objects, and mobiles
- Limited ability to bulk edit or copy/paste content

## Goals
1. Create a standalone desktop application for area creation and editing
2. Provide visual representation of area layouts and connections
3. Enable offline editing of area files
4. Maintain full compatibility with existing .are file format
5. Improve builder productivity and reduce errors
6. Support all existing OLC functionality and more

## Technology Stack
- **UI Framework**: DearPyGui (Python-based immediate mode GUI)
- **AI Context**: Context7 for DearPyGui integration
- **Language**: Python 3.x
- **Package Manager**: UV for dependency management
- **Testing**: pytest with pytest-xdist and pytest-cov
- **Code Quality**: ruff for linting and formatting

## Core Features

### 1. Area Management
- Create new areas with metadata (name, level range, author, vnums)
- Edit existing area properties
- Load and save .are files
- Validate vnum ranges and conflicts
- Area-level flags and settings

### 2. Room Editor
- Visual room creation and editing
- Room properties: name, description, flags, sector type
- Exit management (north, east, south, west, up, down)
- Extra descriptions
- Visual map view showing room connections
- Drag-and-drop room linking
- Room flag editor with checkboxes

### 3. Object Editor
- Create and edit objects with all properties
- Object types: weapons, armor, containers, potions, etc.
- Value fields that change based on object type
- Extra descriptions
- Apply (affect) management
- Material selection
- Weight, cost, and level settings
- Preview of object appearance

### 4. Mobile Editor
- Create and edit mobiles (NPCs)
- Mobile properties: level, alignment, stats
- Act flags and affected flags
- Long and short descriptions
- Shop assignment
- Special function assignment
- Mobile programs (if supported)

### 5. Reset Editor
- Visual reset command editor
- Mobile spawning with limits
- Object placement in rooms
- Object loading into containers
- Equipment assignment to mobiles
- Door state management
- Room randomization

### 6. Visual Map View
- 2D grid-based room layout
- Drag rooms to reposition
- Visual connection lines between rooms
- Color coding by sector type or flags
- Zoom and pan controls
- Mini-map for large areas

### 7. File Operations
- Open existing .are files
- Save changes to .are files
- Export to different formats (future)
- Auto-save and backup functionality
- Undo/redo support

### 8. Validation and Error Checking
- Vnum conflict detection
- Missing reference detection (invalid room vnums in exits, etc.)
- Flag validation
- Level range validation
- Required field validation
- Real-time error highlighting

### 9. Search and Navigation
- Search for rooms, objects, mobiles by vnum or name
- Quick navigation between related entities
- Filter and sort lists
- Recent items history

### 10. Import/Export
- Import from existing OLC format
- Export to .are file format
- Batch operations for multiple areas
- Template support for common patterns

## User Interface Layout

### Main Window
- Menu bar: File, Edit, View, Tools, Help
- Toolbar: Common actions (New, Open, Save, etc.)
- Left sidebar: Area tree view (Areas > Rooms/Objects/Mobiles)
- Center panel: Editor for selected item
- Right sidebar: Properties panel
- Bottom panel: Validation errors and warnings
- Status bar: Current file, vnum ranges, save status

### Editor Panels
Each entity type (Room, Object, Mobile) gets a dedicated editor panel with:
- Form fields for all properties
- Visual previews where applicable
- Related items quick links
- Validation feedback

## Data Model
The tool will parse and generate .are files following the existing format documented in `doc/area.md`:
- #AREA section
- #HELPS section
- #MOBILES section
- #OBJECTS section
- #ROOMS section
- #RESETS section
- #SHOPS section
- #SPECIALS section
- #$ terminator

## Non-Functional Requirements
- **Performance**: Load and save areas with 1000+ rooms in < 2 seconds
- **Usability**: Intuitive interface requiring minimal training
- **Reliability**: Auto-save every 5 minutes, crash recovery
- **Compatibility**: Support all existing .are file features
- **Extensibility**: Plugin architecture for future enhancements

## Future Enhancements (Out of Scope for v1.0)
- Multi-user collaborative editing
- Version control integration
- 3D visualization
- Scripting support for bulk operations
- Integration with running MUD for live testing
- Mobile program editor
- Quest chain visualization
- Loot table generator
- Balance analysis tools

---

## Implementation Checklist

This section breaks down the implementation into manageable chunks of work that can be iterated on.

### Phase 1: Foundation (COMPLETED)
- [x] Set up Python project structure with UV
- [x] Install DearPyGui and development dependencies
- [x] Create basic UI scaffold with main window
- [x] Implement menu bar with placeholder callbacks
- [x] Create area tree panel structure
- [x] Create editor panel structure
- [x] Create properties panel structure
- [x] Create status bar
- [x] Define data models for Area, Room, Object, Mobile

### Phase 2: File I/O and Parsing
- [ ] Create .are file parser
  - [ ] Parse #AREA section
  - [ ] Parse #HELPS section
  - [ ] Parse #MOBILES section
  - [ ] Parse #OBJECTS section
  - [ ] Parse #ROOMS section
  - [ ] Parse #RESETS section
  - [ ] Parse #SHOPS section
  - [ ] Parse #SPECIALS section
- [ ] Create .are file writer
  - [ ] Write #AREA section
  - [ ] Write #HELPS section
  - [ ] Write #MOBILES section
  - [ ] Write #OBJECTS section
  - [ ] Write #ROOMS section
  - [ ] Write #RESETS section
  - [ ] Write #SHOPS section
  - [ ] Write #SPECIALS section
- [ ] Implement file dialog for opening/saving
- [ ] Add file validation on load
- [ ] Implement auto-save functionality

### Phase 3: Area Management
- [ ] Implement area tree population from loaded data
- [ ] Add area creation dialog
- [ ] Add area properties editor
- [ ] Implement area-level validation
- [ ] Add search functionality in area tree
- [ ] Implement area tree filtering

### Phase 4: Room Editor
- [ ] Create room editor UI
  - [ ] Basic properties (name, description, vnum)
  - [ ] Room flags checkboxes
  - [ ] Sector type dropdown
  - [ ] Exit editor
  - [ ] Extra descriptions editor
- [ ] Implement room creation
- [ ] Implement room deletion
- [ ] Implement room duplication
- [ ] Add room validation
- [ ] Create room preview

### Phase 5: Object Editor
- [ ] Create object editor UI
  - [ ] Basic properties (keywords, descriptions, vnum)
  - [ ] Item type dropdown
  - [ ] Dynamic value fields based on item type
  - [ ] Extra flags checkboxes
  - [ ] Wear flags checkboxes
  - [ ] Extra descriptions editor
  - [ ] Affects/applies editor
- [ ] Implement object creation
- [ ] Implement object deletion
- [ ] Implement object duplication
- [ ] Add object validation
- [ ] Create object preview

### Phase 6: Mobile Editor
- [ ] Create mobile editor UI
  - [ ] Basic properties (keywords, descriptions, vnum)
  - [ ] Level and stats
  - [ ] Act flags checkboxes
  - [ ] Affected flags checkboxes
  - [ ] Alignment slider
  - [ ] Race and material dropdowns
  - [ ] Hit/damage dice editors
- [ ] Implement mobile creation
- [ ] Implement mobile deletion
- [ ] Implement mobile duplication
- [ ] Add mobile validation
- [ ] Create mobile preview

### Phase 7: Reset Editor
- [ ] Create reset command list UI
- [ ] Implement mobile spawn reset editor
- [ ] Implement object placement reset editor
- [ ] Implement equipment reset editor
- [ ] Implement door state reset editor
- [ ] Add reset validation
- [ ] Implement reset reordering

### Phase 8: Visual Map View
- [ ] Create map canvas widget
- [ ] Implement room rendering on map
- [ ] Add exit connection lines
- [ ] Implement drag-and-drop room positioning
- [ ] Add zoom and pan controls
- [ ] Implement room selection from map
- [ ] Add color coding by sector/flags
- [ ] Create mini-map widget

### Phase 9: Validation and Error Checking
- [ ] Implement vnum conflict detection
- [ ] Add missing reference detection
- [ ] Create validation panel UI
- [ ] Implement real-time validation
- [ ] Add validation error highlighting
- [ ] Create validation report generator

### Phase 10: Polish and Testing
- [ ] Write unit tests for parsers
- [ ] Write unit tests for data models
- [ ] Write integration tests
- [ ] Add keyboard shortcuts
- [ ] Implement undo/redo system
- [ ] Add tooltips and help text
- [ ] Create user documentation
- [ ] Performance optimization
- [ ] Bug fixes and refinements

### Phase 11: Advanced Features
- [ ] Implement shop editor
- [ ] Implement special function assignment
- [ ] Add help entry editor
- [ ] Create template system
- [ ] Add bulk operations
- [ ] Implement import/export utilities

