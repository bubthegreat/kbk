"""
Writer for .are files.
"""
from pathlib import Path
from typing import TextIO
from area_editor.models.area import Area
from area_editor.models.room import Room
from area_editor.models.mobile import Mobile
from area_editor.models.object import Object
from area_editor.models.reset import Reset
from area_editor.models.shop import Shop
from area_editor.models.special import Special


class AreWriter:
    """Writer for .are files."""
    
    def __init__(self, area: Area):
        """Initialize the writer with an Area object."""
        self.area = area
    
    def write(self, filepath: str | Path) -> None:
        """Write the area to a .are file."""
        filepath = Path(filepath)
        with open(filepath, 'w', encoding='utf-8') as f:
            self._write_area_section(f)
            self._write_mobiles_section(f)
            self._write_objects_section(f)
            self._write_rooms_section(f)
            self._write_specials_section(f)
            self._write_resets_section(f)
            self._write_shops_section(f)
            self._write_terminator(f)
    
    def _write_area_section(self, f: TextIO) -> None:
        """Write the #AREADATA section."""
        f.write("#AREADATA\n")
        f.write(f"Name        {self.area.name}~\n")
        f.write(f"Builders    {self.area.author}~\n")
        f.write(f"VNUMs       {self.area.min_vnum} {self.area.max_vnum}\n")
        f.write(f"Security    {self.area.security}\n")
        if self.area.min_vnum > 0:  # Only add Recall if we have a valid vnum range
            f.write(f"Recall      {self.area.min_vnum}\n")
        f.write("End\n\n")
    
    def _write_mobiles_section(self, f: TextIO) -> None:
        """Write the #MOBDATA section."""
        f.write("#MOBDATA\n")
        
        # Write mobiles in vnum order
        for vnum in sorted(self.area.mobiles.keys()):
            mob = self.area.mobiles[vnum]
            self._write_mobile(f, mob)
        
        f.write("#0\n\n")
    
    def _write_mobile(self, f: TextIO, mob: Mobile) -> None:
        """Write a single mobile."""
        f.write(f"#{mob.vnum}\n")
        f.write(f"NAME   {mob.keywords}~\n")
        f.write(f"SHORT  {mob.short_description}~\n")
        f.write(f"LONG   {mob.long_description}~\n")
        f.write("DESCR\n")
        f.write(f"{mob.description}~\n")
        f.write(f"RACE   {mob.race}~\n")
        f.write(f"ACT    {mob.act_flags}\n")
        f.write(f"AFF    {mob.affected_flags}\n")
        f.write(f"ALIGN  {mob.alignment}\n")
        f.write(f"LEVEL  {mob.level}\n")
        f.write(f"HROLL  {mob.hitroll}\n")
        f.write(f"DDICE  {mob.damage_dice}\n")
        f.write(f"MDICE  {mob.mana_dice}\n")
        f.write(f"HDICE  {mob.hit_dice}\n")
        f.write(f"GOLD   {mob.gold}\n")
        f.write(f"POS    {mob.start_position} {mob.default_position}\n")
        f.write(f"SEX    {mob.sex}\n")
        f.write("End\n\n")
    
    def _write_objects_section(self, f: TextIO) -> None:
        """Write the #OBJDATA section."""
        f.write("#OBJDATA\n")
        
        # Write objects in vnum order
        for vnum in sorted(self.area.objects.keys()):
            obj = self.area.objects[vnum]
            self._write_object(f, obj)
        
        f.write("#0\n\n")
    
    def _write_object(self, f: TextIO, obj: Object) -> None:
        """Write a single object."""
        f.write(f"#{obj.vnum}\n")
        f.write(f"NAME {obj.keywords}~\n")
        f.write(f"SHORT {obj.short_description}~\n")
        f.write(f"DESCR {obj.long_description}~\n")
        
        # TYPE line
        type_values_str = " ".join(str(v) for v in obj.type_values) if obj.type_values else "0"
        f.write(f"TYPE {obj.item_type} {type_values_str}\n")
        
        # EXTRA line
        f.write(f"EXTRA {obj.extra_flags} {obj.extra_value2} {obj.extra_value3} {obj.extra_value4} 0\n")
        
        # VALUES line (legacy format, may not be needed in new format)
        f.write("VALUES 0 0 0 0 0\n")
        
        # STATS line
        f.write(f"STATS {obj.level} {obj.weight} {obj.cost} P\n")
        
        # WEAR line
        f.write(f"WEAR {obj.wear_flags} 0 0\n")
        
        # AFFECT lines (if any)
        # TODO: Implement when we have affects in the Object model

        f.write("End\n\n")

    def _write_rooms_section(self, f: TextIO) -> None:
        """Write the #ROOMDATA section."""
        f.write("#ROOMDATA\n")

        # Write rooms in vnum order
        for vnum in sorted(self.area.rooms.keys()):
            room = self.area.rooms[vnum]
            self._write_room(f, room)

        f.write("#0\n\n")

    def _write_room(self, f: TextIO, room: Room) -> None:
        """Write a single room."""
        f.write(f"#{room.vnum}\n")
        f.write(f"NAME   {room.name}~\n")
        f.write("DESCR\n")
        f.write(f"{room.description}~\n")
        f.write(f"FLAGS  {room.room_flags}\n")
        f.write(f"Sect   {room.sector_type}\n")

        # Write exits (DOOR sections)
        for direction in sorted(room.exits.keys()):
            exit_obj = room.exits[direction]
            f.write(f"DOOR {direction}\n")
            f.write(f"{exit_obj.description}~\n")
            f.write(f"{exit_obj.keywords}~\n")
            f.write(f"{exit_obj.locks} {exit_obj.key_vnum} {exit_obj.to_room}\n")

        # Write extra descriptions (EDESC sections)
        for edesc in room.extra_descriptions:
            f.write(f"EDESC {edesc.keywords}~\n")
            f.write(f"{edesc.description}~\n")

        f.write("End\n\n")

    def _write_specials_section(self, f: TextIO) -> None:
        """Write the #SPECIALS section."""
        f.write("#SPECIALS\n")

        # Write specials if any
        for special in self.area.specials:
            # Format: M <vnum> <spec_function>
            f.write(f"M {special.vnum} {special.spec_function}\n")

        f.write("S\n\n")

    def _write_resets_section(self, f: TextIO) -> None:
        """Write the #RESETS section."""
        f.write("#RESETS\n")

        # Write resets
        for reset in self.area.resets:
            self._write_reset(f, reset)

        f.write("S\n\n")

    def _write_reset(self, f: TextIO, reset: Reset) -> None:
        """Write a single reset command."""
        # Format: <command> <if_flag> <arg1> <arg2> <arg3> <arg4>
        # if_flag is always 0 in modern ROM
        f.write(f"{reset.command} 0 {reset.arg1} {reset.arg2} {reset.arg3} {reset.arg4}\n")

    def _write_shops_section(self, f: TextIO) -> None:
        """Write the #SHOPS section."""
        f.write("#SHOPS\n")

        # Write shops
        for shop in self.area.shops:
            self._write_shop(f, shop)

        f.write("0\n\n")

    def _write_shop(self, f: TextIO, shop: Shop) -> None:
        """Write a single shop."""
        # Format: keeper buy_type0 buy_type1 buy_type2 buy_type3 buy_type4 profit_buy profit_sell open_hour close_hour
        buy_types_str = " ".join(str(bt) for bt in shop.buy_types)
        f.write(f"{shop.keeper} {buy_types_str} {shop.profit_buy} {shop.profit_sell} {shop.open_hour} {shop.close_hour}\n")

    def _write_terminator(self, f: TextIO) -> None:
        """Write the #$ terminator."""
        f.write("#$\n")

