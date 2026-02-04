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
            self._write_improgs_section(f)
            self._write_terminator(f)
    
    def _write_area_section(self, f: TextIO) -> None:
        """Write the #AREADATA section."""
        f.write("#AREADATA\n")
        f.write(f"Name {self.area.name}~\n")
        f.write(f"Builders {self.area.builders}~\n")
        f.write(f"VNUMs {self.area.min_vnum} {self.area.max_vnum}\n")

        # Write Credits if present (format: "{min max} Author  AreaName~")
        if self.area.credits:
            f.write(f"Credits {self.area.credits}\n")

        f.write(f"Security {self.area.security}\n")

        # Write Xplore if present
        if hasattr(self.area, 'xplore'):
            f.write(f"Xplore {self.area.xplore}\n")

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
        f.write(f"NAME  {mob.keywords}~\n")
        f.write(f"SHORT {mob.short_description}~\n")
        f.write(f"LONG  {mob.long_description}~\n")
        f.write("DESCR\n")
        f.write(f"{mob.description}~\n")
        f.write(f"RACE  {mob.race}~\n")
        f.write(f"ACT   {mob.act_flags}\n")
        f.write(f"AFF   {mob.affected_flags}\n")

        # Write OFF, IMM, RES, VULN (always write, even if 0 or empty)
        f.write(f"OFF   {mob.offensive_flags}\n")
        f.write(f"IMM   {mob.immunity_flags}\n")
        f.write(f"RES   {mob.resistance_flags}\n")
        f.write(f"VULN  {mob.vulnerability_flags}\n")

        # Write WSPEC (always write)
        f.write(f"WSPEC {mob.weapon_spec}\n")

        f.write(f"ALIGN {mob.alignment}\n")

        # Write GROUP (always write)
        f.write(f"GROUP {mob.group}\n")

        f.write(f"LEVEL {mob.level}\n")
        f.write(f"HROLL {mob.hitroll}\n")

        # Write ENHA if present
        if mob.enhancement:
            f.write(f"ENHA  {mob.enhancement}\n")

        # Write MATER if present
        if mob.material:
            f.write(f"MATER {mob.material}~\n")

        f.write(f"HDICE {mob.hit_dice}\n")
        f.write(f"MDICE {mob.mana_dice}\n")
        f.write(f"DDICE {mob.damage_dice}\n")

        # Write REGEN (always write)
        f.write(f"REGEN {mob.regen}\n")

        # Write DTYPE if present
        if mob.damage_type:
            f.write(f"DTYPE {mob.damage_type}\n")

        # Write AC if any values are non-zero
        if mob.ac_pierce or mob.ac_bash or mob.ac_slash or mob.ac_exotic:
            f.write(f"AC    {mob.ac_pierce} {mob.ac_bash} {mob.ac_slash} {mob.ac_exotic}\n")

        f.write(f"POS   {mob.start_position} {mob.default_position}\n")
        f.write(f"SEX   {mob.sex}\n")
        f.write(f"GOLD  {mob.gold}\n")

        # Write FORM if present
        if mob.form:
            f.write(f"FORM  {mob.form}\n")

        # Write PARTS if present
        if mob.parts:
            f.write(f"PARTS {mob.parts}\n")

        # Write SIZE if present
        if mob.size:
            f.write(f"SIZE  {mob.size}\n")

        # Write DMOD if present
        if mob.damage_modifier:
            f.write(f"DMOD  {mob.damage_modifier}\n")

        # Write AMOD (always write)
        f.write(f"AMOD  {mob.armor_modifier}\n")

        # Write QUEST (always write)
        f.write(f"QUEST {mob.quest}\n")

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
        f.write(f"NAME   {obj.keywords}~\n")
        f.write(f"SHORT  {obj.short_description}~\n")
        f.write("DESCR \n")
        f.write(f"{obj.long_description}~\n")

        # MAT (material) line
        f.write(f"MAT    {obj.material}~\n")

        # TYPE line
        type_values_str = " ".join(str(v) for v in obj.type_values) if obj.type_values else "0"
        f.write(f"TYPE   {obj.item_type} {type_values_str}\n")

        # EXTRA line - bitmask format: <masks> <bits> [<set> <tar_mask>]...
        # If no flags set: EXTRA 0 0
        # If flags set: EXTRA <masks> <bits> <set> <tar_mask>
        if obj.extra_flags == 0 and obj.extra_value2 == 0:
            # No flags set
            f.write(f"EXTRA  0 0\n")
        else:
            # Flags set - write all 4 values
            f.write(f"EXTRA  {obj.extra_flags} {obj.extra_value2} {obj.extra_value3} {obj.extra_value4}\n")

        # WEAR line (just the flags, no extra values)
        f.write(f"WEAR   {obj.wear_flags}\n")

        # Individual stat lines
        f.write(f"LEVEL  {obj.level}\n")
        f.write(f"WEIGHT {obj.weight}\n")
        f.write(f"COST   {obj.cost}\n")
        f.write(f"COND   {obj.condition}\n")

        # Extra descriptions (EDESC)
        for edesc in obj.extra_descriptions:
            f.write(f"EDESC {edesc.keywords}~\n")
            f.write(f"{edesc.description}~\n")

        # Affect lines (if any) - MUST come before LIMIT
        # Format: Affect <location> <apply_type> <modifier> <bitvector>
        for affect in obj.affects:
            f.write(f"Affect {affect.location} {affect.apply_type} {affect.modifier} {affect.bitvector}\n")

        # Optional LIMIT line - MUST come after Affect
        if obj.limit > 0:
            f.write(f"LIMIT  {obj.limit}\n")

        # Optional RESTR line
        if obj.restriction:
            f.write(f"RESTR  {obj.restriction}~\n")

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
        # Format varies by command type:
        # M: M 0 <mob_vnum> <max_world> <room_vnum> <max_room>
        # O: O 0 <obj_vnum> 0 <room_vnum>
        # P: P 0 <obj_vnum> <max> <container_vnum> <max_in_container>
        # G: G 0 <obj_vnum> 0  (only 3 values after command)
        # E: E 0 <obj_vnum> 0 <wear_loc>  (only 4 values after command)
        # D: D 0 <room_vnum> <door> <state>
        # R: R 0 <room_vnum> <num_exits>

        if reset.command == 'G':
            # Give: G 0 <obj_vnum> 0
            f.write(f"{reset.command} 0 {reset.arg1} {reset.arg2}\n")
        elif reset.command == 'E':
            # Equip: E 0 <obj_vnum> 0 <wear_loc>
            f.write(f"{reset.command} 0 {reset.arg1} {reset.arg2} {reset.arg3}\n")
        elif reset.command == 'O':
            # Object: O 0 <obj_vnum> 0 <room_vnum>
            f.write(f"{reset.command} 0 {reset.arg1} {reset.arg2} {reset.arg3}\n")
        elif reset.command == 'D':
            # Door: D 0 <room_vnum> <door> <state>
            f.write(f"{reset.command} 0 {reset.arg1} {reset.arg2} {reset.arg3}\n")
        elif reset.command == 'R':
            # Randomize: R 0 <room_vnum> <num_exits>
            f.write(f"{reset.command} 0 {reset.arg1} {reset.arg2}\n")
        else:
            # M, P, and others: full 5 values
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

    def _write_improgs_section(self, f: TextIO) -> None:
        """Write the #IMPROGS section."""
        f.write("#IMPROGS\n")
        # Most areas have empty IMPROGS sections, just write the end marker
        f.write("E\n\n")

    def _write_terminator(self, f: TextIO) -> None:
        """Write the #$ terminator."""
        f.write("\n\n#$\n")

