# Unused Variable Warnings TODO

This file tracks variables that are set but never used. These should be either removed or properly utilized.

## comm.c
- **Line 3029**: `bool fColor = FALSE;` in `act_new()`
  - Set but never used
  - Action: Remove the variable declaration

## db.c
- **Line 1205**: `bool fMatch;` in `new_load_area()`
  - Set but never used
  - Action: Remove the variable declaration

## fight.c
- **Line 809**: `bool exeresult;` in `one_hit_new()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 1597**: `AFFECT_DATA *af;` in `damage_newer()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 1439**: `int originaldam;` in `damage_newer()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 1942**: `int originaldam;` in `damage_calculate()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 1941**: `bool immune;` in `damage_calculate()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 3293**: `int tmp_dt;` in `check_spin()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 3966**: `int cloak = 0;` in `raw_kill_new()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 4993**: `OBJ_DATA *secondary;` in `disarm()`
  - Declared but never used (not even set)
  - Action: Remove the variable declaration

- **Line 6213**: `bool attempt_dual;` in `do_circle_stab()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 10611**: `OBJ_DATA *secondary;` in `do_swordplay()`
  - Set but never used
  - Action: Remove the variable declaration

## handler.c
- **Line 1749**: `bool found;` in `affect_join()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 2159**: `bool status;` in `equip_char()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 4172**: `bool found;` in `affect_join_room()`
  - Set but never used
  - Action: Remove the variable declaration

## healer.c
- **Line 45**: `char *words;` in `do_heal()`
  - Set but never used
  - Action: Remove the variable declaration

## magic.c
- **Line 5365**: `int hp_dam;` in `spell_acid_breath()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 5401**: `int hp_dam;` in `spell_fire_breath()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 5471**: `int hp_dam;` in `spell_frost_breath()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 5538**: `int hp_dam;` in `spell_gas_breath()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 5583**: `int hp_dam;` in `spell_lightning_breath()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 8105**: `int acm;` in `spell_animate_object()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 8105**: `int ac;` in `spell_animate_object()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 10565**: `bool found = FALSE;` in `spell_dispel_magic()`
  - Set but never used
  - Action: Remove the variable declaration

## skills.c
- **Line 126**: `bool fAll = FALSE;` in `do_spells()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 270**: `bool fAll = FALSE;` in `do_skills()`
  - Set but never used
  - Action: Remove the variable declaration

## special.c
- **Line 964**: `char *crime;` in `spec_executioner()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 3473**: `int num;` in `spec_legionlord()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 3473**: `int level;` in `spec_legionlord()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 3471**: `char *msg;` in `spec_legionlord()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 4001**: `int imod;` in `spec_blitz()`
  - Set but never used
  - Action: Remove the variable declaration

## morefight.c
- **Line 686**: `bool exeresult;` in `do_whirl()`
  - Set but never used
  - Action: Remove the variable declaration

## extra.c
- **Line 821**: `int bcredits = 0;` in `do_finger()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 2970**: `int check;` in `do_shieldbash_templar()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 2969**: `OBJ_DATA *obj_templar;` in `do_shieldbash_templar()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 3127**: `OBJ_DATA *obj_templar;` in `do_shieldbash()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 3471**: `bool fAll = FALSE;` in `do_powers()`
  - Set but never used
  - Action: Remove the variable declaration

## moremagic.c
- **Line 2365**: `char *targetTwo;` in `spell_doublecast()`
  - Set but never used
  - Action: Remove the variable declaration

## olc_act.c
- **Line 3359**: `EXTRA_DESCR_DATA *ped = NULL;` in `oedit_ed()`
  - Set but never used
  - Action: Remove the variable declaration

## olc_save.c
- **Line 572**: `OBJ_INDEX_DATA *pLastObj;` in `save_resets()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 966**: `FILE *fp;` in `do_asave()`
  - Set but never used
  - Action: Remove the variable declaration

## commune.c
- **Line 45**: `bool fAll = FALSE;` in `do_supps()`
  - Set but never used
  - Action: Remove the variable declaration

- **Line 193**: `AFFECT_DATA af;` in `do_commune()`
  - Set but never used
  - Action: Remove the variable declaration

## mprog.c
- **Line 233**: `bool found = FALSE;` in `death_prog_outer_guardian()`
  - Set but never used
  - Action: Remove the variable declaration

## newbit.c
- **Line 163**: `int found = 1;` in `free_bitmask()`
  - Set but never used
  - Action: Remove the variable declaration

## note.c
- **Line 278**: `bool fAll;` in `parse_note()`
  - Set but never used
  - Action: Remove the variable declaration

## Recommended Approach

1. Review each variable to ensure it's truly unused and not part of incomplete functionality
2. Remove the variable declarations
3. Check if any assignments to these variables have side effects that need to be preserved
4. Recompile to ensure no new errors are introduced

