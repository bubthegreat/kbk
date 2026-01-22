"""
Main entry point for the KBK Area Editor application.
"""
import dearpygui.dearpygui as dpg
from area_editor.ui.main_window import MainWindow


def main():
    """Initialize and run the Area Editor application."""
    # Create DearPyGui context
    dpg.create_context()
    
    # Configure viewport
    dpg.create_viewport(
        title="KBK Area Editor",
        width=1600,
        height=900,
        min_width=1024,
        min_height=768
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
    
    # Cleanup
    dpg.destroy_context()


if __name__ == "__main__":
    main()

