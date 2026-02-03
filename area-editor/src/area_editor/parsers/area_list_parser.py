"""
Parser for area.lst file.

The area.lst file contains a list of area files to load, one per line.
Lines starting with '#' are comments. The file ends with a '$' line.
"""
from pathlib import Path
from typing import List, Optional


def parse_area_list_file(filepath: Optional[str] = None) -> List[str]:
    """Parse area.lst file and return list of area file paths.

    Args:
        filepath: Path to area.lst file. If None, uses path from config or searches default locations.

    Returns:
        List of area file paths (relative to areas_directory)
    """
    if filepath is None:
        # Get path from config
        from area_editor.config import config
        filepath_str = config.get("paths.areas_file")

        # If areas_file is None, return empty list (auto-load disabled)
        if filepath_str is None:
            return []

        # Use configured path (can be absolute or relative to cwd)
        filepath = Path(filepath_str)
        if not filepath.is_absolute():
            filepath = Path.cwd() / filepath
    
    area_files = []
    
    try:
        with open(filepath, 'r') as f:
            for line in f:
                line = line.strip()
                
                # Skip empty lines
                if not line:
                    continue
                
                # End of file marker
                if line == '$':
                    break
                
                # Skip comments
                if line.startswith('#'):
                    continue
                
                # Add area file to list
                area_files.append(line)
    
    except FileNotFoundError:
        print(f"Warning: area.lst not found at {filepath}")
        return []
    
    return area_files


def get_area_files() -> List[str]:
    """Get list of area files to auto-load.
    
    Returns:
        List of area file paths (relative to areas_directory)
    """
    return parse_area_list_file()

