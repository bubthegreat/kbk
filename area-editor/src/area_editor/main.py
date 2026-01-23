"""
Main entry point for the KBK Area Editor application.
"""
import dearpygui.dearpygui as dpg
from area_editor.ui.main_window import MainWindow
from area_editor.config import config


def main():
    """Initialize and run the Area Editor application."""
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

    # Start render loop
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

