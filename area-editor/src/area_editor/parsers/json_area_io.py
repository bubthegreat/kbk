"""Read/write areas in the rom24 (PyRom) JSON folder format.

rom24 stores each area as a folder of JSON files (``area.json``, ``rooms.json``,
``mobiles.json``, ``objects.json``, ``resets.json``, ``shops.json``). Each record
is a machine serialization wrapped as ``{"__class__/<module>.<Class>": {...}}``
and carries encodings the editor does not model (bit-flag references, instance
bookkeeping).

Strategy: **preserve-raw**. On load, map only the fields the editor edits into
its models and stash the original inner dict (``_rom24_raw``) plus its class key
(``_rom24_class``) on each model. On save, overlay the edited fields onto that
raw dict and re-wrap it — so untouched fields (door flags, instance ids, etc.)
round-trip losslessly.
"""
import json
import os

from area_editor.models import Area, Room, Object, Mobile, Reset, Shop
from area_editor.models.room import Exit

# rom24 exit flags are ROM letter codes: A=1 (door) ... F=32 (pickproof).
EX_ISDOOR = 1
EX_PICKPROOF = 32

_SECTIONS = ("area.json", "rooms.json", "mobiles.json", "objects.json", "resets.json", "shops.json")


def is_json_area(path):
    """True if ``path`` is a rom24 JSON area folder (has area.json)."""
    return os.path.isdir(path) and os.path.isfile(os.path.join(path, "area.json"))


def _inner(blob):
    """Split a {"__class__/...": inner} wrapper into (class_key, inner_dict)."""
    key = next(iter(blob))
    return key, blob[key]


def _wrap(class_key, inner):
    return {class_key: inner}


def _read(area_dir, fname):
    with open(os.path.join(area_dir, fname), "r") as fp:
        return json.load(fp)


def _write(area_dir, fname, data):
    with open(os.path.join(area_dir, fname), "w") as fp:
        json.dump(data, fp, indent=2, sort_keys=True)


def _bit_bits(bit_blob):
    """Read the integer value out of a serialized rom24 bit.Bit."""
    if not isinstance(bit_blob, dict):
        return 0
    _, inner = _inner(bit_blob)
    return inner.get("bits", 0)


def _exit_locks(bits):
    if bits & EX_PICKPROOF:
        return 2
    if bits & EX_ISDOOR:
        return 1
    return 0


# --------------------------------------------------------------------------- #
# Load
# --------------------------------------------------------------------------- #
def load_area_json(area_dir):
    area = Area()
    akey, ainner = _inner(_read(area_dir, "area.json"))
    area._rom24_class = akey
    area._rom24_raw = ainner
    area.name = ainner.get("name", "")
    area.credits = ainner.get("credits", "")
    area.min_vnum = ainner.get("min_vnum", 0)
    area.max_vnum = ainner.get("max_vnum", 0)
    area.filename = os.path.basename(os.path.normpath(area_dir))
    area._json_dir = os.path.abspath(area_dir)  # remembered so Save can round-trip

    for blob in _read(area_dir, "rooms.json"):
        rkey, ri = _inner(blob)
        room = Room(vnum=ri.get("vnum", 0))
        room.name = ri.get("name", "")
        room.description = ri.get("description", "")
        room.room_flags = ri.get("room_flags", 0)
        room.sector_type = ri.get("sector_type", 0)
        room._rom24_class = rkey
        room._rom24_raw = ri
        for i, eblob in enumerate(ri.get("exit") or []):
            if not eblob:
                continue
            ekey, ei = _inner(eblob)
            bits = _bit_bits(ei.get("exit_info"))
            key = ei.get("key", -1)
            ex = Exit(
                direction=i,
                description=ei.get("description", ""),
                keywords=ei.get("keyword", ""),
                locks=_exit_locks(bits),
                key_vnum=key if isinstance(key, int) and key >= 0 else 0,
                to_room=ei.get("to_room_vnum") or 0,
            )
            ex._rom24_class = ekey
            ex._rom24_raw = ei
            room.exits[i] = ex
        area.rooms[room.vnum] = room

    for blob in _read(area_dir, "mobiles.json"):
        mkey, mi = _inner(blob)
        mob = Mobile(vnum=mi.get("vnum", 0))
        mob.keywords = mi.get("name", "")
        mob.short_description = mi.get("short_descr", "")
        mob.long_description = mi.get("long_descr", "")
        mob.description = mi.get("description", "")
        mob.level = mi.get("level", 1)
        mob.race = mi.get("_race", mi.get("race", "human"))
        mob._rom24_class = mkey
        mob._rom24_raw = mi
        area.mobiles[mob.vnum] = mob

    for blob in _read(area_dir, "objects.json"):
        okey, oi = _inner(blob)
        obj = Object(vnum=oi.get("vnum", 0))
        obj.keywords = oi.get("name", "")
        obj.short_description = oi.get("short_descr", "")
        obj.long_description = oi.get("description", "")
        obj.item_type = oi.get("item_type", "")
        obj.type_values = [str(v) for v in (oi.get("value") or [])]
        obj.level = oi.get("level", 0)
        obj.weight = oi.get("weight", 0)
        obj.cost = oi.get("cost", 0)
        obj._rom24_class = okey
        obj._rom24_raw = oi
        area.objects[obj.vnum] = obj

    for blob in _read(area_dir, "resets.json"):
        rkey, ri = _inner(blob)
        reset = Reset(
            command=ri.get("command", ""),
            arg1=ri.get("arg1", 0),
            arg2=ri.get("arg2", 0),
            arg3=ri.get("arg3", 0),
            arg4=ri.get("arg4", 0),
        )
        reset._rom24_class = rkey
        reset._rom24_raw = ri
        area.resets.append(reset)

    for blob in _read(area_dir, "shops.json"):
        skey, si = _inner(blob)
        shop = Shop(keeper=si.get("keeper", 0))
        shop.profit_buy = si.get("profit_buy", 100)
        shop.profit_sell = si.get("profit_sell", 100)
        shop.open_hour = si.get("open_hour", 0)
        shop.close_hour = si.get("close_hour", 23)
        shop._rom24_class = skey
        shop._rom24_raw = si
        area.shops.append(shop)

    return area


# --------------------------------------------------------------------------- #
# Save
# --------------------------------------------------------------------------- #
def _raw(model, default_class):
    raw = dict(getattr(model, "_rom24_raw", {}) or {})
    cls = getattr(model, "_rom24_class", default_class)
    return raw, cls


def save_area_json(area, area_dir):
    os.makedirs(area_dir, exist_ok=True)

    ai, akey = _raw(area, "__class__/rom24.world_classes.Area")
    ai["name"] = area.name
    ai["credits"] = area.credits
    ai["min_vnum"] = area.min_vnum
    ai["max_vnum"] = area.max_vnum
    _write(area_dir, "area.json", _wrap(akey, ai))

    rooms = []
    for vnum in sorted(area.rooms):
        room = area.rooms[vnum]
        ri, rkey = _raw(room, "__class__/rom24.handler_room.Room")
        ri["name"] = room.name
        ri["description"] = room.description
        ri["room_flags"] = room.room_flags
        ri["sector_type"] = room.sector_type
        exit_list = ri.get("exit") or [None] * 6
        for i, ex in room.exits.items():
            ei, ekey = _raw(ex, "__class__/rom24.world_classes.Exit")
            ei["description"] = ex.description
            ei["keyword"] = ex.keywords
            ei["to_room_vnum"] = ex.to_room
            while len(exit_list) <= i:
                exit_list.append(None)
            exit_list[i] = _wrap(ekey, ei)
        ri["exit"] = exit_list
        rooms.append(_wrap(rkey, ri))
    _write(area_dir, "rooms.json", rooms)

    mobs = []
    for vnum in sorted(area.mobiles):
        mob = area.mobiles[vnum]
        mi, mkey = _raw(mob, "__class__/rom24.handler_npc.Npc")
        mi["name"] = mob.keywords
        mi["short_descr"] = mob.short_description
        mi["long_descr"] = mob.long_description
        mi["description"] = mob.description
        mi["level"] = mob.level
        mobs.append(_wrap(mkey, mi))
    _write(area_dir, "mobiles.json", mobs)

    objs = []
    for vnum in sorted(area.objects):
        obj = area.objects[vnum]
        oi, okey = _raw(obj, "__class__/rom24.handler_item.Items")
        oi["name"] = obj.keywords
        oi["short_descr"] = obj.short_description
        oi["description"] = obj.long_description
        objs.append(_wrap(okey, oi))
    _write(area_dir, "objects.json", objs)

    _write(
        area_dir,
        "resets.json",
        [_wrap(*reversed(_raw(r, "__class__/rom24.world_classes.Reset"))) for r in area.resets],
    )
    _write(
        area_dir,
        "shops.json",
        [_wrap(*reversed(_raw(s, "__class__/rom24.world_classes.Shop"))) for s in area.shops],
    )
