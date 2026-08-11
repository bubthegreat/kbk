"""Round-trip tests for the rom24 JSON area adapter."""
import json
import os
import shutil

import pytest

from area_editor.parsers import json_area_io

# The real converted stock area, if the rom24 checkout is present alongside.
ROM24_MIDGAARD = "/home/bub/Development/rom24/src/areas/midgaard"


def _synthetic_area(tmp_path):
    """A minimal rom24-format area folder with one room + one exit + one mob."""
    d = str(tmp_path / "demo")
    os.makedirs(d)
    room = {
        "__class__/rom24.handler_room.Room": {
            "vnum": 100,
            "name": "Test Chamber",
            "description": "A plain stone room.",
            "room_flags": 0,
            "sector_type": 1,
            "instance_id": 4242,  # untouched field that must survive round-trip
            "exit": [
                {
                    "__class__/rom24.world_classes.Exit": {
                        "description": "A door north.",
                        "keyword": "door",
                        "key": -1,
                        "to_room_vnum": 101,
                        "exit_info": {"__class__/rom24.bit.Bit": {"bits": 1, "flags_ref": "exit_flags"}},
                    }
                },
                None, None, None, None, None,
            ],
        }
    }
    mob = {"__class__/rom24.handler_npc.Npc": {"vnum": 100, "name": "rat", "short_descr": "a rat", "level": 2, "hitroll": 7}}
    with open(os.path.join(d, "area.json"), "w") as fp:
        json.dump({"__class__/rom24.world_classes.Area": {"name": "Demo", "min_vnum": 100, "max_vnum": 199, "credits": "x"}}, fp)
    with open(os.path.join(d, "rooms.json"), "w") as fp:
        json.dump([room], fp)
    with open(os.path.join(d, "mobiles.json"), "w") as fp:
        json.dump([mob], fp)
    for empty in ("objects.json", "resets.json", "shops.json"):
        with open(os.path.join(d, empty), "w") as fp:
            json.dump([], fp)
    return d


def test_load_maps_fields(tmp_path):
    area = json_area_io.load_area_json(_synthetic_area(tmp_path))
    assert area.name == "Demo"
    assert 100 in area.rooms
    room = area.rooms[100]
    assert room.name == "Test Chamber"
    assert room.sector_type == 1
    assert 0 in room.exits
    assert room.exits[0].to_room == 101
    assert room.exits[0].keywords == "door"
    assert room.exits[0].locks == 1  # EX_ISDOOR
    assert area.mobiles[100].short_description == "a rat"


def test_roundtrip_preserves_untouched_and_applies_edits(tmp_path):
    src = _synthetic_area(tmp_path)
    area = json_area_io.load_area_json(src)

    area.rooms[100].name = "Renamed Chamber"
    area.rooms[100].exits[0].to_room = 999

    out = str(tmp_path / "out")
    json_area_io.save_area_json(area, out)
    reloaded = json_area_io.load_area_json(out)

    assert reloaded.rooms[100].name == "Renamed Chamber"
    assert reloaded.rooms[100].exits[0].to_room == 999
    # Untouched machine field survived the round-trip.
    assert reloaded.rooms[100]._rom24_raw["instance_id"] == 4242
    # Untouched mob field survived.
    assert reloaded.mobiles[100]._rom24_raw["hitroll"] == 7


@pytest.mark.skipif(not os.path.isdir(ROM24_MIDGAARD), reason="rom24 checkout not present")
def test_real_midgaard_roundtrip(tmp_path):
    work = str(tmp_path / "midgaard")
    shutil.copytree(ROM24_MIDGAARD, work)
    area = json_area_io.load_area_json(work)
    assert len(area.rooms) > 50
    assert 3001 in area.rooms  # Temple of Mota
    original_name = area.rooms[3001].name
    area.rooms[3001].name = original_name + " (edited)"

    json_area_io.save_area_json(area, work)
    reloaded = json_area_io.load_area_json(work)
    assert reloaded.rooms[3001].name.endswith("(edited)")
    # Room count preserved across a full save.
    assert len(reloaded.rooms) == len(area.rooms)
