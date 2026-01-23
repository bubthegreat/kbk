# KBK Area Editor

A modern GUI-based area editor for the KBK MUD, built with DearPyGui.

## Features

- Visual area, room, object, and mobile editing
- Tree-based navigation of area contents
- Real-time validation and error checking
- Compatible with existing .are file format
- Offline editing support

## Installation

### Prerequisites

- Python 3.12 or higher
- UV package manager
- System libraries for DearPyGui (OpenGL, X11)

### Setup

```bash
# Clone the repository
cd /home/bub/Development/kbk/area-editor

# Install dependencies
uv sync

# Run the application
uv run area-editor

# Or run with a file to load on startup
uv run area-editor --file tests/test.are
uv run area-editor -f ../area/limbo.are
```

### System Library Requirements

DearPyGui requires certain system libraries. On Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install -y libgl1-mesa-glx libx11-6 libxext6 libxrender1 libxrandr2 libxinerama1 libxcursor1 libxi6
```

If you encounter `GLIBCXX` version errors, you may need to update your system's C++ standard library:

```bash
sudo apt-get install -y libstdc++6
```

## Development

### Running Tests

```bash
uv run pytest
```

### Code Quality

```bash
# Run linter
uv run ruff check .

# Format code
uv run ruff format .
```

## Project Structure

```
area-editor/
├── src/
│   └── area_editor/
│       ├── main.py              # Application entry point
│       ├── models/              # Data models
│       │   ├── area.py
│       │   ├── room.py
│       │   ├── object.py
│       │   └── mobile.py
│       ├── ui/                  # UI components
│       │   ├── main_window.py
│       │   ├── menu_bar.py
│       │   ├── area_tree.py
│       │   ├── editor_panel.py
│       │   ├── properties_panel.py
│       │   └── status_bar.py
│       └── parsers/             # .are file parsers (TODO)
├── tests/                       # Test files
└── pyproject.toml              # Project configuration
```

## Usage

### Basic Usage

1. Launch the application: `uv run area-editor`
2. Use File > Open Area to load an existing .are file
3. Navigate the area tree to select items to edit
4. Edit properties in the center and right panels
5. Save changes with File > Save

### Command Line Options

```bash
# Show help
uv run area-editor --help

# Load a specific file on startup
uv run area-editor --file tests/test.are
uv run area-editor -f ../area/limbo.are

# Start with no file loaded
uv run area-editor
```

This is especially useful for:
- **Testing**: Quickly load test files during development
- **Convenience**: Skip the File > Open dialog for frequently edited files
- **Automation**: Script the editor to open specific files

## Roadmap

See `requirements.md` in the parent directory for detailed requirements and planned features.

## License

See LICENSE file in the parent directory.
