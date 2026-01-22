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
    dpg.create_viewport(
        title="KBK Area Editor",
        width=window_width,
        height=window_height,
        min_width=1024,
        min_height=768,
        vsync=False,  # Disable vsync to prevent freezing during resize/maximize
        resizable=True
    )

    # Setup DearPyGui
    dpg.setup_dearpygui()

    # Create main window
    main_window = MainWindow()
    main_window.create()

    # Show viewport
    dpg.show_viewport()

    # Set primary window
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

