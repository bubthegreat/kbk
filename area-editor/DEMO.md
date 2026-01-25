# KBK Area Editor - Demo Guide

## Running the Application

To start the Area Editor:

```bash
cd area-editor
uv run python -m area_editor.main
```

## Features Implemented

### 1. File Loading
- Click **File > Open Area...** to open the file dialog
- Navigate to an `.are` file (e.g., `../area/limbo.are`)
- The parser will load and parse the file
- Status bar shows loading progress and results

### 2. Area Tree Navigation
Once a file is loaded, the Area Explorer shows:
- **Area Info**: Click to see area details
- **Rooms**: Expandable list of all rooms with vnums
- **Objects**: Expandable list of all objects with vnums
- **Mobiles**: Expandable list of all mobiles with vnums

### 3. Item Viewing
Click on any item in the tree to view its details in the Editor panel:

**Room View:**
- Name and description
- Sector type and room flags
- List of exits with destinations
- Extra descriptions

**Object View:**
- Short and long descriptions
- Item type and material
- Level, weight, and cost
- List of affects

**Mobile View:**
- Short and long descriptions
- Race, level, and alignment
- Hit dice and damage dice
- Hitroll

**Area View:**
- Area name
- VNUM range
- Security level
- Statistics (room/object/mobile counts)

### 4. Status Bar
The status bar at the bottom shows:
- Current operation status (loading, errors, success)
- File information (name, item counts)
- Modification status (asterisk when modified)

## Example Workflow

1. Start the application
2. Click **File > Open Area...**
3. Select `limbo.are` from the `area` directory
4. Wait for "Area loaded successfully" message
5. Expand the **Rooms** section in the tree
6. Click on **#1: The Void**
7. View room details in the Editor panel
8. Try clicking on objects and mobiles to see their details

## Current Limitations

- **Read-only**: The editor panels currently only display data, editing is not yet implemented
- **No saving**: File saving functionality is not yet implemented
- **No creation**: Creating new areas/rooms/objects/mobiles is not yet implemented
- **Limited sections**: Only AREADATA, ROOMS, OBJECTS, and MOBILES sections are parsed

## Next Steps

The next phase of development will add:
1. Edit functionality to all editor panels
2. File saving (.are writer)
3. Area/room/object/mobile creation
4. Validation and error checking

