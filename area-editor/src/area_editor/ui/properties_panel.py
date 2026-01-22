"""
Properties panel showing details and quick actions for selected items.
"""
import dearpygui.dearpygui as dpg
from area_editor.models.room import Room
from area_editor.models.object import Object
from area_editor.models.mobile import Mobile
from area_editor.models.area import Area


class PropertiesPanel:
    """Properties panel showing item details and quick actions."""

    def __init__(self):
        """Initialize the properties panel."""
        self.content_group_id = None

    def create(self):
        """Create the properties panel."""
        dpg.add_text("Properties", color=(200, 200, 200))
        dpg.add_separator()

        # Content group that will be updated
        with dpg.group(tag="properties_content") as self.content_group_id:
            dpg.add_text(
                "No item selected",
                color=(120, 120, 120)
            )

    def _clear_content(self):
        """Clear the properties content."""
        if self.content_group_id and dpg.does_item_exist(self.content_group_id):
            dpg.delete_item(self.content_group_id, children_only=True)

    def show_room_properties(self, room: Room, vnum: int):
        """Show properties for a room."""
        self._clear_content()

        with dpg.group(parent=self.content_group_id):
            dpg.add_text(f"Room #{vnum}", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=5)

            # Quick stats
            dpg.add_text("Quick Info", color=(150, 150, 150))
            dpg.add_spacer(height=5)

            dpg.add_text(f"Exits: {len(room.exits)}", color=(120, 120, 120))
            dpg.add_text(f"Extra Descs: {len(room.extra_descriptions)}", color=(120, 120, 120))
            dpg.add_text(f"Sector: {room.sector_type}", color=(120, 120, 120))
            dpg.add_text(f"Flags: {room.room_flags}", color=(120, 120, 120))

            if room.exits:
                dpg.add_spacer(height=10)
                dpg.add_text("Exit Directions", color=(150, 150, 150))
                dpg.add_spacer(height=5)
                direction_names = ['North', 'East', 'South', 'West', 'Up', 'Down']
                for direction in sorted(room.exits.keys()):
                    dir_name = direction_names[direction] if direction < len(direction_names) else f"Dir{direction}"
                    dpg.add_text(f"• {dir_name}", color=(120, 120, 120))

    def show_object_properties(self, obj: Object, vnum: int):
        """Show properties for an object."""
        self._clear_content()

        with dpg.group(parent=self.content_group_id):
            dpg.add_text(f"Object #{vnum}", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=5)

            # Quick stats
            dpg.add_text("Quick Info", color=(150, 150, 150))
            dpg.add_spacer(height=5)

            dpg.add_text(f"Type: {obj.item_type}", color=(120, 120, 120))
            dpg.add_text(f"Level: {obj.level}", color=(120, 120, 120))
            dpg.add_text(f"Weight: {obj.weight}", color=(120, 120, 120))
            dpg.add_text(f"Cost: {obj.cost}", color=(120, 120, 120))
            dpg.add_text(f"Material: {obj.material}", color=(120, 120, 120))
            dpg.add_text(f"Extra Descs: {len(obj.extra_descriptions)}", color=(120, 120, 120))

            if obj.affects:
                dpg.add_spacer(height=10)
                dpg.add_text(f"Affects ({len(obj.affects)})", color=(150, 150, 150))
                dpg.add_spacer(height=5)
                for affect in obj.affects[:5]:  # Show first 5
                    dpg.add_text(f"• {affect.apply_type}: {affect.modifier:+d}", color=(120, 120, 120))
                if len(obj.affects) > 5:
                    dpg.add_text(f"  ... and {len(obj.affects) - 5} more", color=(100, 100, 100))

    def show_mobile_properties(self, mob: Mobile, vnum: int):
        """Show properties for a mobile."""
        self._clear_content()

        with dpg.group(parent=self.content_group_id):
            dpg.add_text(f"Mobile #{vnum}", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=5)

            # Quick stats
            dpg.add_text("Quick Info", color=(150, 150, 150))
            dpg.add_spacer(height=5)

            dpg.add_text(f"Race: {mob.race}", color=(120, 120, 120))
            dpg.add_text(f"Level: {mob.level}", color=(120, 120, 120))
            dpg.add_text(f"Alignment: {mob.alignment}", color=(120, 120, 120))

            dpg.add_spacer(height=10)
            dpg.add_text("Combat Stats", color=(150, 150, 150))
            dpg.add_spacer(height=5)

            dpg.add_text(f"Hit Dice: {mob.hit_dice}", color=(120, 120, 120))
            dpg.add_text(f"Damage Dice: {mob.damage_dice}", color=(120, 120, 120))
            dpg.add_text(f"Hitroll: {mob.hitroll}", color=(120, 120, 120))
            dpg.add_text(f"AC Pierce: {mob.ac_pierce}", color=(120, 120, 120))
            dpg.add_text(f"AC Bash: {mob.ac_bash}", color=(120, 120, 120))
            dpg.add_text(f"AC Slash: {mob.ac_slash}", color=(120, 120, 120))
            dpg.add_text(f"AC Exotic: {mob.ac_exotic}", color=(120, 120, 120))

    def show_area_properties(self, area: Area):
        """Show properties for an area."""
        self._clear_content()

        with dpg.group(parent=self.content_group_id):
            dpg.add_text("Area Info", color=(200, 200, 100))
            dpg.add_separator()
            dpg.add_spacer(height=5)

            # Quick stats
            dpg.add_text("Statistics", color=(150, 150, 150))
            dpg.add_spacer(height=5)

            dpg.add_text(f"VNUM Range: {area.min_vnum}-{area.max_vnum}", color=(120, 120, 120))
            dpg.add_text(f"Security: {area.security}", color=(120, 120, 120))
            dpg.add_text(f"Builders: {area.builders}", color=(120, 120, 120))

            dpg.add_spacer(height=10)
            dpg.add_text("Content", color=(150, 150, 150))
            dpg.add_spacer(height=5)

            dpg.add_text(f"Rooms: {len(area.rooms)}", color=(120, 120, 120))
            dpg.add_text(f"Objects: {len(area.objects)}", color=(120, 120, 120))
            dpg.add_text(f"Mobiles: {len(area.mobiles)}", color=(120, 120, 120))

    def clear(self):
        """Clear the properties panel."""
        self._clear_content()

        with dpg.group(parent=self.content_group_id):
            dpg.add_text(
                "No item selected",
                color=(120, 120, 120)
            )

