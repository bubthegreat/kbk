"""
Parser for materials.lst file.
"""
from pathlib import Path
from typing import List, Optional


def parse_materials_file(filepath: Optional[str] = None) -> List[str]:
    """Parse materials.lst file and return list of material names.

    Args:
        filepath: Path to materials.lst file. If None, uses path from config or searches default locations.

    Returns:
        List of material names (without the ~ suffix)
    """
    if filepath is None:
        # Get path from config
        from area_editor.config import config
        filepath_str = config.get("paths.materials_file")

        if filepath_str is None:
            # Search default locations relative to current working directory
            search_paths = [
                Path.cwd() / "area" / "materials.lst",  # From repo root
                Path.cwd() / "materials.lst",  # From area directory
                Path.cwd() / "../area/materials.lst",  # From area-editor directory
            ]

            for search_path in search_paths:
                if search_path.exists():
                    filepath = search_path
                    break
            else:
                # No file found, will use default list
                filepath = None
        else:
            # Use configured path (can be absolute or relative to cwd)
            filepath = Path(filepath_str)
            if not filepath.is_absolute():
                filepath = Path.cwd() / filepath
    
    materials = []

    # If no filepath found, return default list
    if filepath is None:
        print(f"Warning: materials.lst not found in default locations, using default materials")
        return [
            'iron', 'steel', 'wood', 'leather', 'cloth', 'gold', 'silver',
            'mithril', 'adamantite', 'bronze', 'copper', 'stone', 'bone',
            'glass', 'ceramic', 'plastic', 'flesh', 'plant', 'food'
        ]

    try:
        with open(filepath, 'r') as f:
            in_data_section = False
            
            for line in f:
                line = line.strip()
                
                # Skip empty lines
                if not line:
                    continue
                
                # Check for start of data section
                if line.startswith('MATS'):
                    in_data_section = True
                    continue
                
                # Skip lines before MATS section
                if not in_data_section:
                    continue
                
                # Parse material line
                # Format: material_name~ <stats...>
                parts = line.split()
                if parts:
                    material_name = parts[0]
                    # Remove trailing ~ if present
                    if material_name.endswith('~'):
                        material_name = material_name[:-1]
                    materials.append(material_name)
    
    except FileNotFoundError:
        # If file not found, return a default list
        print(f"Warning: materials.lst not found at {filepath}, using default materials")
        materials = [
            'iron', 'steel', 'wood', 'leather', 'cloth', 'gold', 'silver',
            'mithril', 'adamantite', 'bronze', 'copper', 'stone', 'bone',
            'glass', 'ceramic', 'plastic', 'flesh', 'plant', 'food'
        ]
    
    return materials


def get_materials() -> List[str]:
    """Get list of available materials.
    
    Returns:
        List of material names
    """
    return parse_materials_file()

