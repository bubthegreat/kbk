"""
Main .are file parser.
"""
from typing import Optional
from pathlib import Path
from area_editor.models.area import Area
from area_editor.parsers.base_parser import BaseParser


class AreParser(BaseParser):
    """Parser for .are files."""
    
    def __init__(self, filepath: str):
        """Initialize the parser with a file path."""
        self.filepath = Path(filepath)
        self.area = Area()
        self.area.filename = self.filepath.name
        self.file_handle = None
    
    def parse(self) -> Optional[Area]:
        """Parse the .are file and return an Area object."""
        try:
            with open(self.filepath, 'r', encoding='utf-8', errors='ignore') as f:
                self.file_handle = f
                super().__init__(f)

                # Read through the file looking for sections
                while True:
                    raw_line = self.file.readline()
                    if not raw_line:  # EOF
                        break

                    self.line_number += 1
                    line = raw_line.strip()

                    if line.startswith('#AREADATA'):
                        self._parse_area_section()
                    elif line.startswith('#MOBDATA'):
                        self._parse_mobiles_section()
                    elif line.startswith('#OBJDATA'):
                        self._parse_objects_section()
                    elif line.startswith('#ROOMDATA'):
                        self._parse_rooms_section()
                    elif line.startswith('#RESETS'):
                        self._parse_resets_section()
                    elif line.startswith('#SHOPS'):
                        self._parse_shops_section()
                    elif line.startswith('#SPECIALS'):
                        self._parse_specials_section()
                    elif line.startswith('#HELPS'):
                        self._parse_helps_section()
                    elif line.startswith('#$'):
                        break

                return self.area

        except Exception as e:
            print(f"Error parsing {self.filepath}: {e}")
            import traceback
            traceback.print_exc()
            return None
    
    def _parse_area_section(self):
        """Parse the #AREADATA section."""
        print(f"Parsing AREADATA section at line {self.line_number}")
        
        while True:
            line = self.read_line()
            if not line or line.strip() == 'End':
                break
            
            parts = line.split(None, 1)
            if len(parts) < 2:
                continue
            
            keyword = parts[0]
            value = parts[1]
            
            if keyword == 'Name':
                self.area.name = value.rstrip('~')
            elif keyword == 'Builders':
                self.area.author = value.rstrip('~')
            elif keyword == 'VNUMs':
                vnums = value.split()
                if len(vnums) >= 2:
                    self.area.min_vnum = int(vnums[0])
                    self.area.max_vnum = int(vnums[1])
            elif keyword == 'Security':
                self.area.security = int(value)
            elif keyword == 'Credits':
                # Parse credits line like "{ 5 35} Author    Area Name~"
                if value.startswith('{'):
                    # Extract level range and name
                    pass
    
    def _parse_mobiles_section(self):
        """Parse the #MOBDATA section."""
        print(f"Parsing MOBDATA section at line {self.line_number}")
        from area_editor.models.mobile import Mobile

        while True:
            raw_line = self.file.readline()
            if not raw_line:  # EOF
                break

            self.line_number += 1
            line = raw_line.strip()

            if line.startswith('#0'):
                break

            if line.startswith('#'):
                # Parse mobile vnum
                vnum = int(line[1:])
                mobile = Mobile(vnum=vnum)

                # Parse mobile data
                while True:
                    line = self.read_line()
                    if not line or line.strip() == 'End':
                        break

                    parts = line.split(None, 1)
                    if len(parts) < 1:
                        continue

                    keyword = parts[0]

                    if keyword == 'NAME':
                        # Read tilde-terminated keywords
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        mobile.keywords = self.read_string()

                    elif keyword == 'SHORT':
                        # Read tilde-terminated short description
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        mobile.short_description = self.read_string()

                    elif keyword == 'LONG':
                        # Read tilde-terminated long description
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        mobile.long_description = self.read_string()

                    elif keyword == 'DESCR':
                        # Read tilde-terminated description
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        mobile.description = self.read_string()

                    elif keyword == 'RACE':
                        # Read tilde-terminated race
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        mobile.race = self.read_string()

                    elif keyword == 'ACT':
                        # Read act flags
                        mobile.act_flags = parts[1] if len(parts) > 1 else ""

                    elif keyword == 'AFF':
                        # Read affected flags
                        mobile.affected_flags = parts[1] if len(parts) > 1 else ""

                    elif keyword == 'OFF':
                        # Read offensive flags
                        mobile.offensive_flags = parts[1] if len(parts) > 1 else ""

                    elif keyword == 'IMM':
                        # Read immunity flags
                        mobile.immunity_flags = parts[1] if len(parts) > 1 else ""

                    elif keyword == 'RES':
                        # Read resistance flags
                        mobile.resistance_flags = parts[1] if len(parts) > 1 else ""

                    elif keyword == 'VULN':
                        # Read vulnerability flags
                        mobile.vulnerability_flags = parts[1] if len(parts) > 1 else ""

                    elif keyword == 'WSPEC':
                        # Read weapon spec
                        mobile.weapon_spec = int(parts[1]) if len(parts) > 1 else 0

                    elif keyword == 'ALIGN':
                        # Read alignment
                        mobile.alignment = int(parts[1]) if len(parts) > 1 else 0

                    elif keyword == 'GROUP':
                        # Read group
                        mobile.group = int(parts[1]) if len(parts) > 1 else 0

                    elif keyword == 'LEVEL':
                        # Read level
                        mobile.level = int(parts[1]) if len(parts) > 1 else 1

                    elif keyword == 'HROLL':
                        # Read hitroll
                        mobile.hitroll = int(parts[1]) if len(parts) > 1 else 0

                    elif keyword == 'ENHA':
                        # Read enhancement
                        mobile.enhancement = parts[1] if len(parts) > 1 else "100.00%"

                    elif keyword == 'MATER':
                        # Read tilde-terminated material
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        mobile.material = self.read_string()

                    elif keyword == 'HDICE':
                        # Read hit dice
                        mobile.hit_dice = parts[1] if len(parts) > 1 else "1d1+0"

                    elif keyword == 'MDICE':
                        # Read mana dice
                        mobile.mana_dice = parts[1] if len(parts) > 1 else "1d1+0"

                    elif keyword == 'DDICE':
                        # Read damage dice
                        mobile.damage_dice = parts[1] if len(parts) > 1 else "1d1+0"

                    elif keyword == 'REGEN':
                        # Read regen
                        mobile.regen = int(parts[1]) if len(parts) > 1 else 0

                    elif keyword == 'DTYPE':
                        # Read damage type
                        mobile.damage_type = parts[1] if len(parts) > 1 else ""

                    elif keyword == 'AC':
                        # Read armor class (4 values)
                        if len(parts) > 1:
                            ac_values = parts[1].split()
                            if len(ac_values) >= 4:
                                mobile.ac_pierce = int(ac_values[0])
                                mobile.ac_bash = int(ac_values[1])
                                mobile.ac_slash = int(ac_values[2])
                                mobile.ac_exotic = int(ac_values[3])

                    elif keyword == 'POS':
                        # Read position (2 values)
                        if len(parts) > 1:
                            pos_values = parts[1].split()
                            if len(pos_values) >= 2:
                                mobile.start_position = pos_values[0]
                                mobile.default_position = pos_values[1]

                    elif keyword == 'SEX':
                        # Read sex
                        mobile.sex = parts[1] if len(parts) > 1 else "none"

                    elif keyword == 'GOLD':
                        # Read gold
                        mobile.gold = int(parts[1]) if len(parts) > 1 else 0

                    elif keyword == 'FORM':
                        # Read form flags
                        mobile.form = parts[1] if len(parts) > 1 else ""

                    elif keyword == 'PARTS':
                        # Read parts flags
                        mobile.parts = parts[1] if len(parts) > 1 else ""

                    elif keyword == 'SIZE':
                        # Read size
                        mobile.size = parts[1] if len(parts) > 1 else "medium"

                    elif keyword == 'DMOD':
                        # Read damage modifier
                        mobile.damage_modifier = int(parts[1]) if len(parts) > 1 else 0

                    elif keyword == 'AMOD':
                        # Read armor modifier
                        mobile.armor_modifier = int(parts[1]) if len(parts) > 1 else 0

                    elif keyword == 'QUEST':
                        # Read quest
                        mobile.quest = int(parts[1]) if len(parts) > 1 else 0

                # Add mobile to area
                self.area.mobiles[vnum] = mobile
                print(f"  Parsed mobile #{vnum}: {mobile.short_description}")
    
    def _parse_objects_section(self):
        """Parse the #OBJDATA section."""
        print(f"Parsing OBJDATA section at line {self.line_number}")
        from area_editor.models.object import Object, ObjectAffect, ExtraDescription

        while True:
            raw_line = self.file.readline()
            if not raw_line:  # EOF
                break

            self.line_number += 1
            line = raw_line.strip()

            if line.startswith('#0'):
                break

            if line.startswith('#'):
                # Parse object vnum
                vnum = int(line[1:])
                obj = Object(vnum=vnum)

                # Parse object data
                while True:
                    line = self.read_line()
                    if not line or line.strip() == 'End':
                        break

                    parts = line.split(None, 1)
                    if len(parts) < 1:
                        continue

                    keyword = parts[0]

                    if keyword == 'NAME':
                        # Read tilde-terminated keywords
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        obj.keywords = self.read_string()

                    elif keyword == 'SHORT':
                        # Read tilde-terminated short description
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        obj.short_description = self.read_string()

                    elif keyword == 'DESCR':
                        # Read tilde-terminated long description
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        obj.long_description = self.read_string()

                    elif keyword == 'MAT':
                        # Read tilde-terminated material
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        obj.material = self.read_string()

                    elif keyword == 'TYPE':
                        # Read item type and values
                        # Format: TYPE <item_type> <value1> <value2> ...
                        if len(parts) > 1:
                            type_parts = parts[1].split(None, 1)
                            obj.item_type = type_parts[0] if type_parts else ""
                            if len(type_parts) > 1:
                                # Store remaining values as a list
                                obj.type_values = type_parts[1].split()

                    elif keyword == 'EXTRA':
                        # Read extra flags (can be 2, 3, or 4 values)
                        if len(parts) > 1:
                            extra_parts = parts[1].split()
                            if len(extra_parts) >= 1:
                                obj.extra_flags = int(extra_parts[0])
                            if len(extra_parts) >= 2:
                                obj.extra_value2 = int(extra_parts[1])
                            if len(extra_parts) >= 3:
                                obj.extra_value3 = int(extra_parts[2])
                            if len(extra_parts) >= 4:
                                obj.extra_value4 = int(extra_parts[3])

                    elif keyword == 'WEAR':
                        # Read wear flags
                        obj.wear_flags = parts[1] if len(parts) > 1 else ""

                    elif keyword == 'LEVEL':
                        # Read level
                        obj.level = int(parts[1]) if len(parts) > 1 else 0

                    elif keyword == 'WEIGHT':
                        # Read weight
                        obj.weight = int(parts[1]) if len(parts) > 1 else 0

                    elif keyword == 'COST':
                        # Read cost
                        obj.cost = int(parts[1]) if len(parts) > 1 else 0

                    elif keyword == 'COND':
                        # Read condition
                        obj.condition = int(parts[1]) if len(parts) > 1 else 100

                    elif keyword == 'LIMIT':
                        # Read limit
                        obj.limit = int(parts[1]) if len(parts) > 1 else 0

                    elif keyword == 'RESTR':
                        # Read restriction
                        obj.restriction = parts[1] if len(parts) > 1 else ""

                    elif keyword == 'Affect':
                        # Read affect
                        # Format: Affect <location> <type> <modifier> <bitvector>
                        if len(parts) > 1:
                            affect_parts = parts[1].split()
                            if len(affect_parts) >= 4:
                                # Bitvector can be a number or a letter code
                                try:
                                    bitvector = int(affect_parts[3])
                                except ValueError:
                                    # It's a letter code, store as 0 for now
                                    # (we'd need a flag lookup table to convert properly)
                                    bitvector = 0

                                affect = ObjectAffect(
                                    location=affect_parts[0],
                                    apply_type=int(affect_parts[1]),
                                    modifier=int(affect_parts[2]),
                                    bitvector=bitvector
                                )
                                obj.affects.append(affect)

                    elif keyword == 'EDESC':
                        # Read extra description
                        # Format: EDESC <keywords>~
                        #         <description>~
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        keywords = self.read_string()

                        self.read_line()
                        self.current_line = self.current_line.strip()
                        description = self.read_string()

                        extra_desc = ExtraDescription(
                            keywords=keywords,
                            description=description
                        )
                        obj.extra_descriptions.append(extra_desc)

                # Add object to area
                self.area.objects[vnum] = obj
                print(f"  Parsed object #{vnum}: {obj.short_description}")
    
    def _parse_rooms_section(self):
        """Parse the #ROOMDATA section."""
        print(f"Parsing ROOMDATA section at line {self.line_number}")
        from area_editor.models.room import Room, Exit, ExtraDescription

        while True:
            raw_line = self.file.readline()
            if not raw_line:  # EOF
                break

            self.line_number += 1
            line = raw_line.strip()

            if line.startswith('#0'):
                break

            if line.startswith('#'):
                # Parse room vnum
                vnum = int(line[1:])
                room = Room(vnum=vnum)

                # Parse room data
                while True:
                    line = self.read_line()
                    if not line or line.strip() == 'End':
                        break

                    parts = line.split(None, 1)
                    if len(parts) < 1:
                        continue

                    keyword = parts[0]

                    if keyword == 'NAME':
                        # Read tilde-terminated name
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        room.name = self.read_string()

                    elif keyword == 'DESCR':
                        # Read tilde-terminated description
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        room.description = self.read_string()

                    elif keyword == 'FLAGS':
                        # Read room flags
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        room.room_flags = self.read_number()

                    elif keyword == 'Sect':
                        # Read sector type
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        room.sector_type = self.read_number()

                    elif keyword == 'DOOR':
                        # Read door/exit data
                        # Format: DOOR <direction>
                        #         <description>~
                        #         <keywords>~
                        #         <locks> <key_vnum> <to_room>
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        direction = self.read_number()

                        # Read description (tilde-terminated)
                        self.read_line()
                        self.current_line = self.current_line.strip()
                        description = self.read_string()

                        # Read keywords (tilde-terminated)
                        self.read_line()
                        self.current_line = self.current_line.strip()
                        keywords = self.read_string()

                        # Read locks, key_vnum, to_room
                        self.read_line()
                        locks = self.read_number()
                        key_vnum = self.read_number()
                        to_room = self.read_number()

                        # Create exit
                        exit_obj = Exit(
                            direction=direction,
                            description=description,
                            keywords=keywords,
                            locks=locks,
                            key_vnum=key_vnum,
                            to_room=to_room
                        )
                        room.exits[direction] = exit_obj

                    elif keyword == 'EDESC':
                        # Read extra description
                        # Format: EDESC <keywords>~
                        #         <description>~
                        self.current_line = parts[1] if len(parts) > 1 else ""
                        keywords = self.read_string()

                        self.read_line()
                        self.current_line = self.current_line.strip()
                        description = self.read_string()

                        extra_desc = ExtraDescription(
                            keywords=keywords,
                            description=description
                        )
                        room.extra_descriptions.append(extra_desc)

                # Add room to area
                self.area.rooms[vnum] = room
                print(f"  Parsed room #{vnum}: {room.name}")
    
    def _parse_resets_section(self):
        """Parse the #RESETS section."""
        print(f"Parsing RESETS section at line {self.line_number}")
        # TODO: Implement reset parsing
        pass
    
    def _parse_shops_section(self):
        """Parse the #SHOPS section."""
        print(f"Parsing SHOPS section at line {self.line_number}")
        # TODO: Implement shop parsing
        pass
    
    def _parse_specials_section(self):
        """Parse the #SPECIALS section."""
        print(f"Parsing SPECIALS section at line {self.line_number}")
        # TODO: Implement specials parsing
        pass
    
    def _parse_helps_section(self):
        """Parse the #HELPS section."""
        print(f"Parsing HELPS section at line {self.line_number}")
        # TODO: Implement helps parsing
        pass

