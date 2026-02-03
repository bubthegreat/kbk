"""
Main entry point for the KBK Area Editor application.
"""
import argparse
import sys
from pathlib import Path
import dearpygui.dearpygui as dpg
from area_editor.ui.main_window import MainWindow
from area_editor.config import config


def main():
    """Initialize and run the Area Editor application."""
    # Parse command line arguments
    parser = argparse.ArgumentParser(
        description="KBK Area Editor - A GUI editor for MUD area files",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  area-editor                           # Start with no file loaded
  area-editor --file tests/test.are     # Load test.are on startup
  area-editor -f ../area/limbo.are      # Load limbo.are on startup
        """
    )
    parser.add_argument(
        '-f', '--file',
        type=str,
        metavar='PATH',
        help='Area file (.are) to load on startup'
    )

    args = parser.parse_args()

    # Validate file path if provided
    file_to_load = None
    if args.file:
        file_path = Path(args.file)
        if not file_path.exists():
            print(f"Error: File not found: {args.file}", file=sys.stderr)
            sys.exit(1)
        if not file_path.is_file():
            print(f"Error: Not a file: {args.file}", file=sys.stderr)
            sys.exit(1)
        if not file_path.suffix == '.are':
            print(f"Warning: File does not have .are extension: {args.file}", file=sys.stderr)
        file_to_load = str(file_path.resolve())

    # Create DearPyGui context
    dpg.create_context()

    # Load window size from config
    window_width, window_height = config.get_window_size()

    # Configure viewport
    # Note: vsync=False helps prevent freezing on some Linux systems with MESA/ZINK
    # x_pos=0, y_pos=0 ensures the viewport starts at the correct position
    # This is critical for proper mouse coordinate mapping
    dpg.create_viewport(
        title="KBK Area Editor",
        width=window_width,
        height=window_height,
        x_pos=0,
        y_pos=0,
        min_width=1024,
        min_height=768,
        vsync=False,  # Disable vsync to prevent freezing during resize/maximize
        resizable=True
    )

    # Setup DearPyGui
    dpg.setup_dearpygui()

    # Apply font scaling based on saved preference or auto-detect
    if config.is_font_scale_auto():
        # Auto-detect appropriate font scale based on resolution
        # Baseline: 1920x1080 = 2.0x scale (doubled for better readability)
        # Scale proportionally based on the geometric mean of width and height ratios
        # This ensures fonts scale appropriately for different aspect ratios
        baseline_width = 1920.0
        baseline_height = 1080.0

        width_ratio = window_width / baseline_width
        height_ratio = window_height / baseline_height

        # Use geometric mean to balance width and height scaling
        # This prevents over-scaling on ultra-wide or over-scaling on tall screens
        import math
        base_scale = math.sqrt(width_ratio * height_ratio)

        # Double the scale factor for better readability
        font_scale = base_scale * 2.0

        # Clamp to reasonable range (0.75x to 3.0x)
        font_scale = max(0.75, min(3.0, font_scale))

        # Round to nearest 0.25 for cleaner scaling
        font_scale = round(font_scale * 4) / 4

        config.set_font_scale(font_scale, auto=True)
    else:
        # Use manually set font scale
        font_scale = config.get_font_scale()

    dpg.set_global_font_scale(font_scale)

    # Create main window
    main_window = MainWindow()
    main_window.create()

    # Show viewport
    dpg.show_viewport()

    # Set as primary window - this makes it fill the viewport automatically
    dpg.set_primary_window(main_window.window_id, True)

    # Load file if specified on command line
    if file_to_load:
        main_window.open_area_file(file_to_load)
    else:
        # Auto-load areas from area.lst if configured
        from area_editor.parsers.area_list_parser import get_area_files
        area_files = get_area_files()

        if area_files:
            # Get areas directory from config
            areas_dir_str = config.get("paths.areas_directory")

            if areas_dir_str is None:
                # Search default locations relative to current working directory
                search_dirs = [
                    Path.cwd() / "area",  # From repo root
                    Path.cwd(),  # From area directory itself
                    Path.cwd() / "../area",  # From area-editor directory
                ]

                for search_dir in search_dirs:
                    if search_dir.exists() and search_dir.is_dir():
                        areas_dir = search_dir
                        break
                else:
                    print("Warning: Could not find area directory")
                    areas_dir = Path.cwd() / "area"  # Default fallback
            else:
                # Use configured path (can be absolute or relative to cwd)
                areas_dir = Path(areas_dir_str)
                if not areas_dir.is_absolute():
                    areas_dir = Path.cwd() / areas_dir

            # Load each area file
            for area_file in area_files:
                area_path = areas_dir / area_file
                if area_path.exists() and area_path.suffix == '.are':
                    try:
                        main_window.open_area_file(str(area_path.resolve()))
                    except Exception as e:
                        print(f"Warning: Failed to load {area_file}: {e}")

    # Start DearPyGui
    dpg.start_dearpygui()

    # Save window size before cleanup
    viewport_width = dpg.get_viewport_width()
    viewport_height = dpg.get_viewport_height()
    config.set_window_size(viewport_width, viewport_height)
    config.save()

    # Cleanup
    dpg.destroy_context()


if __name__ == "__main__":
    main()

