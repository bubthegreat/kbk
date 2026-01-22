"""
Properties panel showing details and quick actions for selected items.
"""
import dearpygui.dearpygui as dpg


class PropertiesPanel:
    """Properties panel showing item details and quick actions."""
    
    def __init__(self):
        """Initialize the properties panel."""
        pass
    
    def create(self):
        """Create the properties panel."""
        dpg.add_text("Properties", color=(200, 200, 200))
        dpg.add_separator()
        
        # Placeholder content
        with dpg.group(tag="properties_content"):
            dpg.add_text(
                "No item selected",
                color=(120, 120, 120)
            )
    
    def show_room_properties(self, room_data):
        """Show properties for a room."""
        # TODO: Implement room properties display
        pass
    
    def show_object_properties(self, obj_data):
        """Show properties for an object."""
        # TODO: Implement object properties display
        pass
    
    def show_mobile_properties(self, mob_data):
        """Show properties for a mobile."""
        # TODO: Implement mobile properties display
        pass

