/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/***************************************************************************
*	ROM 2.4 is copyright 1993-1996 Russ Taylor			   *
*	ROM has been brought to you by the ROM consortium		   *
*	    Russ Taylor (rtaylor@pacinfo.com)				   *
*	    Gabrielle Taylor (gtaylor@pacinfo.com)			   *
*	    Brian Moore (rom@rom.efn.org)				   *
*	By using this code, you have agreed to follow the terms of the	   *
*	ROM license, in the file Tartarus/doc/rom.license                  *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

#include "include.h"

#if !defined(macintosh)
extern	int	_filbuf		args( (FILE *) );
#endif

int getpid();
time_t time(time_t *tloc);


void printRaceInfo();

/* externals for counting purposes */
extern	OBJ_DATA	*obj_free;
extern	CHAR_DATA	*char_free;
extern  DESCRIPTOR_DATA *descriptor_free;
extern	PC_DATA		*pcdata_free;
extern  AFFECT_DATA	*affect_free;

/*
 * Globals.
 */
 
char * flag_name_lookup args((long bitv, const struct flag_type *flag_table));
HELP_DATA *		help_first;
HELP_DATA *		help_last;

HELP_AREA *had_list;

SHOP_DATA *		shop_first;
SHOP_DATA *		shop_last;

char			bug_buf		[2*MAX_INPUT_LENGTH];
CHAR_DATA *		char_list;
char *			help_greeting;
char			log_buf		[2*MAX_INPUT_LENGTH];
KILL_DATA		kill_table	[MAX_LEVEL];
OBJ_DATA *		object_list;
MAT_DATA *		mat_list;
TIME_INFO_DATA		time_info;
WEATHER_DATA		weather_info;

sh_int			obsidian_prince_timer;

ROOM_INDEX_DATA *       top_affected_room;

NOTE_DATA *		note_free;
NOTE_DATA *		note_list;

sh_int			gsn_aura;
sh_int			gsn_divine_aura;
sh_int			gsn_backstab;
sh_int			gsn_plant_explosive;
sh_int			gsn_advanced_tripwire;
sh_int			gsn_shadowstep;
sh_int			gsn_veil_of_the_opaque;
sh_int			gsn_disguise;
sh_int			gsn_improved_blackjack;
sh_int			gsn_bulwark_of_blades;
sh_int			gsn_coalesce_resistance;
sh_int			gsn_blade_mastery;
sh_int			gsn_divine_transferrance;
sh_int			gsn_statue_sacrifice;
sh_int			gsn_greater_regeneration;
sh_int			gsn_resurrect;
sh_int                  gsn_detect_hidden;
sh_int			gsn_dodge;
sh_int                  gsn_blizzard;
sh_int                  gsn_icy_wind;
sh_int                  gsn_electric_storm;
sh_int			gsn_envenom;
sh_int			gsn_hide;
sh_int			gsn_peek;
sh_int			gsn_pick_lock;
sh_int			gsn_sneak;
sh_int			gsn_steal;
sh_int			gsn_silence;
sh_int			gsn_disarm;
sh_int			gsn_enhanced_damage;
sh_int                  gsn_kick;
sh_int			gsn_crescent_kick;
sh_int			gsn_owaza;
sh_int			gsn_shuto_strike;
sh_int			gsn_seiken_strike;
sh_int			gsn_sukuinage;
sh_int			gsn_axe_kick;
sh_int			gsn_heightened_awareness;
sh_int			gsn_unarmed_defense;
sh_int			gsn_unarmed_offense;
sh_int			gsn_bindwounds;
sh_int			gsn_tigerclaw;
sh_int			gsn_backfist;
sh_int			gsn_scissor_kick;
sh_int			gsn_parry;
sh_int			gsn_rescue;
sh_int			gsn_second_attack;
sh_int			gsn_third_attack;
sh_int			gsn_fourth_attack;

sh_int			gsn_blindness;
sh_int			gsn_charm_person;
sh_int			gsn_curse;
sh_int			gsn_invis;
sh_int			gsn_mass_invis;
sh_int			gsn_poison;
sh_int			gsn_plague;
sh_int			gsn_sleep;
sh_int			gsn_sanctuary;
sh_int			gsn_stoneskin;
sh_int			gsn_shield;
sh_int			gsn_fly;
/* new gsn's */

sh_int  		gsn_axe;
sh_int  		gsn_dagger;
sh_int  		gsn_flail;
sh_int  		gsn_mace;
sh_int  		gsn_polearm;
sh_int			gsn_shield_block;
sh_int  		gsn_spear;
sh_int  		gsn_sword;
sh_int  		gsn_whip;

sh_int  		gsn_bash;
sh_int                  gsn_bearcharge;
sh_int  		gsn_berserk;
sh_int  		gsn_dirt;
sh_int  		gsn_hand_to_hand;
sh_int  		gsn_trip;
sh_int                  gsn_asscounter;
sh_int                  gsn_counter;
sh_int                  gsn_legbreak;
sh_int                  gsn_armbreak;
sh_int  		gsn_fast_healing;
sh_int  		gsn_haggle;
sh_int  		gsn_lore;
sh_int  		gsn_meditation;

sh_int  		gsn_scrolls;
sh_int  		gsn_staves;
sh_int  		gsn_wands;
sh_int  		gsn_recall;
sh_int			gsn_icy_shards;
sh_int			gsn_crush;
sh_int			gsn_ground_control;
sh_int			gsn_righteousness;
sh_int			gsn_absorb;
sh_int 			gsn_camouflage;
sh_int 			gsn_acute_vision;
sh_int			gsn_ambush;
sh_int 			gsn_laying_hands;
sh_int 			gsn_circle_stab;
sh_int 			gsn_empower;
sh_int 			gsn_circle;
sh_int			gsn_dual_backstab;
sh_int 			gsn_power_word_stun;
sh_int			gsn_psi_blast; //Illithids
sh_int			gsn_skin;
sh_int			gsn_camp;
sh_int			gsn_resistance;
sh_int                  gsn_bloodthirst;
sh_int                  gsn_shroud;
sh_int			gsn_dark_shroud;
sh_int                  gsn_blitz;
sh_int                  gsn_shackles;
sh_int			gsn_bloody_shackles;
sh_int			gsn_mimic;
sh_int			gsn_morph_weapon;
sh_int			gsn_liquify;
sh_int			gsn_breath_morph;
sh_int			gsn_animate_dead;
sh_int			gsn_barkskin;
sh_int			gsn_animal_call;
sh_int			gsn_shadowgate;
sh_int			gsn_eye_of_the_predator;
sh_int			gsn_lunge;
sh_int			gsn_blackjack;
sh_int			gsn_dual_wield;
sh_int			gsn_trance;
sh_int			gsn_wanted;
sh_int			gsn_cleave;
sh_int			gsn_herb;
sh_int			gsn_bandage;
sh_int			gsn_deathstrike;
sh_int			gsn_protective_shield;
sh_int			gsn_consecrate;
sh_int			gsn_timestop;
sh_int			gsn_timestop_done;
sh_int			gsn_true_sight;
sh_int			gsn_butcher;
sh_int			gsn_shadowstrike;
sh_int			gsn_fireshield;
sh_int			gsn_iceshield;
sh_int			gsn_vanish;
sh_int			gsn_riot;
sh_int			gsn_embalm;
sh_int			gsn_dark_dream;
sh_int			gsn_revolt;
sh_int			gsn_acid_spit;
sh_int			gsn_bear_call;
sh_int			gsn_trophy;
sh_int			gsn_tail;
sh_int			gsn_spellbane;
sh_int			gsn_request;
sh_int			gsn_endure;
sh_int			gsn_follow_through;
sh_int			gsn_nerve;
sh_int			gsn_palm;
sh_int			gsn_throw;
sh_int			gsn_poison_dust;
sh_int			gsn_blindness_dust;
sh_int			gsn_cblow;
sh_int			gsn_roll;
sh_int			gsn_block;
sh_int			gsn_garrotte;
sh_int			gsn_warcry;
sh_int			gsn_chimera_lion;
sh_int			gsn_chimera_goat;
sh_int			gsn_strange_form;
sh_int			gsn_downstrike;
sh_int			gsn_enlist;
sh_int			gsn_shadowplane;
sh_int			gsn_blackjack_timer;
sh_int			gsn_garrotte_timer;
sh_int			gsn_tame;
sh_int			gsn_find_water;
sh_int			gsn_track;
sh_int			gsn_shield_cleave;
sh_int			gsn_spellcraft;
sh_int			gsn_demand;
sh_int			gsn_tertiary_wield;
sh_int			gsn_breath_fire;
sh_int			gsn_awareness;
sh_int			gsn_forest_blending;
sh_int			gsn_gaseous_form;
sh_int			gsn_rear_kick;
sh_int			gsn_regeneration;
sh_int			gsn_parrot;
sh_int			gsn_door_bash;
sh_int			gsn_wolverine_brand;
sh_int			gsn_flame_scorch;
sh_int			gsn_mutilated_left_arm;
sh_int			gsn_mutilated_left_hand;
sh_int			gsn_mutilated_left_leg;
sh_int			gsn_mutilated_right_arm;
sh_int			gsn_mutilated_right_leg;
sh_int			gsn_mutilated_right_hand;
sh_int			gsn_blood_tide;
sh_int			gsn_flail_arms;
sh_int			gsn_shattered_bone;
sh_int			gsn_banshee_call;
sh_int			gsn_soulbind;
sh_int			gsn_earthfade;
sh_int			gsn_forget;
sh_int			gsn_earthbind;
sh_int			gsn_divine_touch;
sh_int			gsn_grounding;
sh_int			gsn_shock_sphere;
sh_int			gsn_forage;
sh_int			gsn_fwood;
sh_int			gsn_carve;
sh_int			gsn_assassinate;
sh_int			gsn_defend;
sh_int			gsn_intimidate;
sh_int			gsn_escape;
sh_int			gsn_moving_ambush;
sh_int			gsn_pugil;
sh_int			gsn_staff;
sh_int			gsn_evaluation;
sh_int			gsn_enhanced_damage_two;
sh_int			gsn_protection_heat_cold;
sh_int 			gsn_tactics;
sh_int			gsn_lash;
sh_int			gsn_prevent_healing;
sh_int			gsn_regeneration;
sh_int			gsn_undead_drain;
sh_int			gsn_iron_resolve;
sh_int			gsn_quiet_movement;
sh_int			gsn_atrophy;
sh_int                  gsn_flurry;
sh_int                  gsn_drum;
sh_int                  gsn_armbreak;
sh_int                  gsn_siton;
sh_int			gsn_knife;
sh_int			gsn_disperse;
sh_int			gsn_decapitate;
sh_int			gsn_lightwalk;
sh_int			gsn_bobbit;
sh_int gsn_spike;
sh_int gsn_starvation;
sh_int gsn_dehydrated;
sh_int gsn_black_death;
sh_int gsn_black_scythe;
sh_int gsn_spore;
sh_int gsn_barbarian_strike;
sh_int gsn_barbarian_bone_tear;
sh_int gsn_hold_person; /* Hold person -Detlef */
sh_int gsn_wraithform;
sh_int gsn_garble;
sh_int gsn_impale;
sh_int			gsn_unholy_bless;
sh_int gsn_defiance;
sh_int gsn_confuse; /* Confuse for Outlaw -Detlef */
sh_int gsn_executioner;
sh_int gsn_haunting;
sh_int gsn_darkforge; // For Ancient -- Ceial
sh_int gsn_darkforge_sword;
sh_int gsn_darkforge_dagger;
sh_int gsn_darkforge_whip;
sh_int gsn_bind;
sh_int gsn_cheap_shot;
sh_int gsn_deafen; /* Deafen for AP -Detlef */
sh_int gsn_questdodge; /* Quest dodge -Detlef */
sh_int gsn_divine_intervention; /* Divine intervention - Dioxide */
sh_int gsn_ranger_staff; /* Ranger staff skill - Dioxide */
sh_int gsn_creep; /* creep skill - Dioxide */
sh_int gsn_dragonsword;
sh_int gsn_dragonplate;
sh_int gsn_unholy_timer;
sh_int gsn_honorable_combat;
sh_int gsn_chaos_blade;
sh_int gsn_aristaeia;
sh_int gsn_dark_blessing;
sh_int gsn_rage_of_dioxide;
sh_int gsn_artic_fury;
sh_int gsn_merth;
sh_int gsn_spin;
sh_int gsn_enhanced_damage_three;
sh_int gsn_pummel;
sh_int gsn_gouge;
sh_int gsn_stun;
sh_int gsn_stab;
sh_int gsn_stalker;
sh_int gsn_zealotism;
sh_int gsn_parting_blow;
sh_int gsn_dream_blossom;
sh_int gsn_randomizer;
sh_int gsn_body_of_steel; /*tat affect -Orduno*/
sh_int gsn_body_of_iron; /*tat affect -Orduno*/
sh_int gsn_body_of_wood; /*tat affect -Orduno*/
sh_int gsn_body_of_clay; /*tat affect -Orduno*/
sh_int gsn_body_of_stone; /*tat affect -Orduno*/
sh_int gsn_body_of_diamond; /*tat affect -Orduno*/
sh_int gsn_rage; /*dhaytons tat -Orduno*/
sh_int gsn_curse_of_the_dagger; /*dhaytons tat Orduno*/
sh_int gsn_shieldbash;  /* Shield bash for Paladins -- Dioxide */
sh_int gsn_hacksaw;  /* Hacksaw for Outlaw -- Dioxide */
sh_int gsn_boneshatter;  /* Quest boneshatter -- Dioxide */
sh_int gsn_pincer;  /* Quest pincer -- Graelik */
sh_int gsn_spiderhands; /* Spiderhands -- Graelik */
sh_int gsn_chargeset; /* Quest chargeset -- Dioxide */
sh_int gsn_shove; /* Quest shove -- Dioxide */
sh_int gsn_devour; /* Quest devour -- Dioxide */
sh_int gsn_strip; /* Quest strip -- Graelik */
sh_int gsn_headclap; /* Quest headclap -- Dioxide */
sh_int gsn_noheadclap; // Lets put a damper on headclap a bit -- Ceial
sh_int gsn_choke; /* Quest choke --Dioxide */
sh_int gsn_rot; /* quest rot -- xanth */
sh_int gsn_rot_recovery;
sh_int gsn_eagle_eyes; /* Eagle eyes -- Dioxide */
sh_int gsn_cutoff; /* Cutoff -- Dioxide */
sh_int gsn_incinerate;
sh_int gsn_integrate;
sh_int gsn_chameleon;
sh_int gsn_insect_swarm;
sh_int gsn_change_sex;
sh_int gsn_forest_friend;
sh_int gsn_slaves;
sh_int gsn_phase;
sh_int gsn_flourintine;
sh_int gsn_ironhands;
sh_int gsn_overhead;
sh_int gsn_archery;
/*block of new elementalist spells*/
sh_int gsn_self_immolation;
sh_int gsn_shield_of_bubbles;
sh_int gsn_shield_of_dust;
sh_int gsn_shield_of_flames;
sh_int gsn_shield_of_frost;
sh_int gsn_shield_of_lightning;
sh_int gsn_shield_of_wind;
sh_int gsn_flames;
sh_int gsn_static_charge;
sh_int gsn_blizzard;
/*illusionist type stuff*/
sh_int gsn_displacement;
sh_int gsn_nullify_sound;
sh_int gsn_snare;
sh_int gsn_stance;
sh_int gsn_transform;
sh_int gsn_group_gate;
sh_int gsn_group_recall;
sh_int gsn_group_sanctuary;
sh_int gsn_march;
sh_int gsn_critical_strike;
sh_int gsn_valiant_wrath;
sh_int gsn_weaponmeld;
sh_int gsn_mana_shield;
sh_int gsn_lich_elixir_affect;
sh_int gsn_age_death;
sh_int gsn_soul_channel;
sh_int gsn_darkheart;
sh_int gsn_anti_magic_shell;
sh_int gsn_dark_essence;
sh_int gsn_ritual_of_abominations;
sh_int gsn_snaretimer;
sh_int gsn_phat_blunt;
sh_int gsn_whirl;
sh_int gsn_nightfist;
sh_int gsn_centurion_call;
sh_int gsn_imperial_training;
sh_int gsn_incandescense;
sh_int gsn_scorch;
sh_int gsn_bite;
sh_int gsn_abites;
sh_int gsn_throw_knife;
sh_int gsn_poison_flower;
sh_int gsn_pie;
sh_int gsn_weaponbreaker;
sh_int gsn_barrier;
sh_int gsn_crushing_blow;
sh_int gsn_hex;
sh_int gsn_black_circle;
sh_int gsn_maehslin;
sh_int gsn_volley;
sh_int gsn_soften;
sh_int gsn_duo;
sh_int gsn_protection;

sh_int gsn_forge;
sh_int	gsn_wire_delay;
sh_int	gsn_trip_wire;
sh_int	gsn_balance;
sh_int	gsn_gag;
sh_int  gsn_healing_sleep;
sh_int  gsn_sunray;
sh_int  gsn_armor_of_thorns;
sh_int  gsn_cranial;
sh_int  gsn_entwine;
sh_int  gsn_legsweep;
sh_int  gsn_rising_pheonix_kick;
sh_int  gsn_double_spin_kick;
sh_int  gsn_charge;
sh_int	gsn_dash;
sh_int	gsn_restrike;
sh_int	gsn_cross_slice;
sh_int  gsn_icelance;
sh_int	gsn_bleed;
sh_int  gsn_blade_barrier;
sh_int  gsn_detect_illusion;
sh_int  gsn_counterspell;
sh_int  gsn_control_flight;
sh_int  gsn_control_speed;
sh_int  gsn_control_phase;
sh_int  gsn_control_skin;
sh_int  gsn_kotegaeshi;
sh_int  gsn_spire_of_flames;
sh_int  gsn_worldbind;
sh_int  gsn_hellfire;
sh_int  gsn_chop;
sh_int	gsn_weapontrip;
sh_int	gsn_dblow;
sh_int  gsn_spell_evasion;
sh_int  gsn_bloodlust;
sh_int  gsn_ward_timer;
sh_int  gsn_ormthrak;
sh_int  gsn_paladin_tech_counter_pc;
sh_int  gsn_paladin_tech_counter_npc;
sh_int  gsn_swordplay;
sh_int  gsn_drowning;
sh_int  gsn_water_breathing;
sh_int  gsn_hobble;
sh_int	gsn_zornath;
sh_int  gsn_ward_diminution;
sh_int  gsn_intrigue;
sh_int  gsn_tracerah;
sh_int  gsn_cloak_form;
sh_int  gsn_vampiric_touch;
sh_int  gsn_unholy_affinity;
sh_int  gsn_darkshout;
sh_int  gsn_beast_timer;
sh_int  gsn_credit_timer;
sh_int  gsn_pathfinding;
sh_int  gsn_plant_growth;
sh_int  gsn_spores;
sh_int  gsn_carapace;
sh_int  gsn_bearform;
sh_int  gsn_herbal_medicine;
sh_int  gsn_treeform;
sh_int  gsn_spike_growth;
sh_int  gsn_enlarge;
sh_int  gsn_reduce;
sh_int  gsn_shield_of_purity;
sh_int  gsn_shield_of_redemption;
sh_int  gsn_shield_of_justice;
sh_int  gsn_shield_of_truth;
sh_int	gsn_dragsnare;
sh_int	gsn_wilderness_fam;
sh_int  gsn_charge_weapon;
sh_int  gsn_dent;
sh_int  gsn_feint;
sh_int  gsn_sigil_pain;
sh_int  gsn_crescent;
sh_int  gsn_intercept;
sh_int  gsn_maelstrom;
sh_int  gsn_bastion_of_elements;
sh_int	gsn_hurricane;
sh_int	gsn_elemental_affinity;
sh_int	gsn_shatter_armor;
sh_int	gsn_vorpal_bolt;
sh_int  gsn_exude_health;
sh_int  gsn_vitality;
sh_int  gsn_dragon_might;

/* GSNS */

/*
 * Locals.
 */
MOB_INDEX_DATA *	mob_index_hash		[MAX_KEY_HASH];
OBJ_INDEX_DATA *	obj_index_hash		[MAX_KEY_HASH];
ROOM_INDEX_DATA *	room_index_hash		[MAX_KEY_HASH];
char *			string_hash		[MAX_KEY_HASH];

AREA_DATA *		area_first;
AREA_DATA *		area_last;
AREA_DATA *             current_area;

char *			string_space;
char *			top_string;
char			str_empty	[1];

sh_int			count_data[40000];

int			top_affect;
int			top_area;
int			top_ed;
long			top_exit;
long			top_help;
long			top_mob_index;
long			top_obj_index;
long			top_reset;
long			top_room;
long			top_shop;
long                    top_vnum_room;  /* OLC */
long                    top_vnum_mob;   /* OLC */
long                    top_vnum_obj;   /* OLC */
int 			mobile_count = 0;
int			newmobs = 0;
int			newobjs = 0;

/*
 * Memory management.
 * Increase MAX_STRING if you have too.
 * Tune the others only if you understand what you're doing.
 */
#define                 MAX_STRING      7500000	/* Original was 1483120 and then 3593120 */
#define			MAX_PERM_BLOCK	131072
#define			MAX_MEM_LIST	12

void *			rgFreeList	[MAX_MEM_LIST];
const int		rgSizeList	[MAX_MEM_LIST]	=
{
    16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384, 32768-64
};

int			nAllocString;
int			sAllocString;
int			nAllocPerm;
int			sAllocPerm;

extern bool fCopyOver;

char * munch(char *str)
{
	char buf[MSL];
	int i;
	sprintf(buf,"%s",str);
	for(i=0; buf[i]; i++)
		if(buf[i] == '\n')
		{
			buf[i] = ' ';
			buf[i+1] = '\n';
			i+=1;
		}
        return str_dup(buf);
}

/*
 * Semi-locals.
 */
bool			fBootDb;
FILE *			fpArea;
FILE *			fpMaterials;
char			strArea[MAX_INPUT_LENGTH];

int 	race_occurance[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int 	cabal_occurance[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int 	class_occurance[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int 	sex_occurance[] = { 0, 0, 0 };
int 	num_pfiles = 0;
int 	average_played = 0;
int 	tempalign = 0;
int 	align_occurance[] = { 0, 0, 0 };
int 	disregard = 0;
int 	char_hours = 0;
int 	char_max_hp = 0;
int 	most_hours = 0;
char 	most_hours_player[MAX_STRING_LENGTH];
char 	temp_player_name[MAX_STRING_LENGTH];
char *	plr_act_flags;
int 	totalhrsplayed = 0;
int 	temp_level = 0;
float 	temp_percentage = 0;
sh_int 	cabal_members[MAX_CABAL];
sh_int 	cabal_max[MAX_CABAL];

/*
 * Local booting procedures.
*/


void	load_bans		args( ( void ) );
void    init_mm         	args( ( void ) );
void	load_area		args( ( FILE *fp ) );
void    new_load_area   	args( ( FILE *fp ) );   /* OLC */
void 	load_helps 		args ((FILE * fp, char *fname));
void	load_old_mob		args( ( FILE *fp ) );
void 	load_mobiles		args( ( FILE *fp ) );
void	load_old_obj		args( ( FILE *fp ) );
void 	load_objects		args( ( FILE *fp ) );
void	load_resets		args( ( FILE *fp ) );
void	load_rooms		args( ( FILE *fp ) );
void	load_shops		args( ( FILE *fp ) );
void 	load_socials		args( ( FILE *fp ) );
void	load_specials		args( ( FILE *fp ) );
void	load_cabal_items	args( ( void ) );
void	fix_exits		args( ( void ) );
void    load_improgs    	args( ( FILE *fp ) );
void	reset_area		args( ( AREA_DATA * pArea ) );
void 	fread_room 		args( ( FILE *fp, long vnum ));
void 	fread_mobile 		args( ( FILE *fp, long vnum ));
void 	fread_object 		args( ( FILE *fp, long vnum ));
void 	load_objects_new	args( ( FILE *fp ) );
void 	load_mobiles_new 	args( ( FILE *fp ) );
void 	load_rooms_new 		args( ( FILE *fp ) );
int		load_materials		args( ( FILE *fp ) );
void 	logItemFix		args( (void) );

/*
 * Big mama top level function.
 */
void boot_db()
{
    FILE *fp;
    char tempbuf[MSL], buf[MSL];

    obsidian_prince_timer=45;

    /*
     * Init some data space stuff.
     */
    {
	if ( ( string_space = calloc( 1, MAX_STRING ) ) == NULL )
	{
	    bug( "Boot_db: can't alloc %d string space.", MAX_STRING );
		exit( 1 );
	}
	top_string	= string_space;
	fBootDb		= TRUE;
    }

    /*
     * Init random number generator.
     */
    {
        init_mm( );
    }

    /*
     * Set time and weather.
     */
    {
	long lhour, lday, lmonth;

	lhour		= (current_time - 650336715)
			/ (PULSE_TICK / PULSE_PER_SECOND);
	time_info.hour	= lhour  % 24;
	lday		= lhour  / 24;
	time_info.day	= lday   % 35;
	lmonth		= lday   / 35;
	time_info.month	= lmonth % 17;
	time_info.year	= lmonth / 17;

	     if ( time_info.hour <  5 ) weather_info.sunlight = SUN_DARK;
	else if ( time_info.hour <  6 ) weather_info.sunlight = SUN_RISE;
	else if ( time_info.hour < 19 ) weather_info.sunlight = SUN_LIGHT;
	else if ( time_info.hour < 20 ) weather_info.sunlight = SUN_SET;
	else                            weather_info.sunlight = SUN_DARK;

	weather_info.change	= 0;
	weather_info.mmhg	= 960;
	if ( time_info.month >= 7 && time_info.month <=12 )
	    weather_info.mmhg += number_range( 1, 50 );
	else
	    weather_info.mmhg += number_range( 1, 80 );

	     if ( weather_info.mmhg <=  980 ) weather_info.sky = SKY_LIGHTNING;
	else if ( weather_info.mmhg <= 1000 ) weather_info.sky = SKY_RAINING;
	else if ( weather_info.mmhg <= 1020 ) weather_info.sky = SKY_CLOUDY;
	else                                  weather_info.sky = SKY_CLOUDLESS;

    }

    /* room_affected_data */
    top_affected_room = NULL;

    /*
     * Assign gsn's for skills which have them.
     */

    {
	int sn;
        for ( sn = 0; sn < MAX_SKILL; sn++ )
	{

            if ( skill_table[sn].pgsn != NULL )
		*skill_table[sn].pgsn = sn;
	}
    }

    init_mysql();
    /*
     * Read in all the area files.
     */
    {

	FILE *fpList;

	if ( ( fpList = fopen( AREA_LIST, "r" ) ) == NULL )
	{
	    perror( AREA_LIST );
	    exit( 1 );
	}

	if ( ( fpMaterials = fopen( MATERIAL_LIST, "r" ) ) == NULL ) {
		perror ( MATERIAL_LIST );
		exit( 1 );
	}
	else {
		int matcount = load_materials(fpMaterials);
		char matbuf[MSL];
		sprintf(matbuf,"%d materials loaded.",matcount);
		log_string(matbuf);
	}
		
	for ( ; ; )
	{
	    strcpy( strArea, fread_word( fpList ) );
	    if ( strArea[0] == '$' )
		break;
	  
	    if ( strArea[0] == '-' )
	    {
		fpArea = stdin;
	    }
	    else
	    {
		if ( ( fpArea = fopen( strArea, "r" ) ) == NULL )
		{
		    perror( strArea );
		    exit( 1 );
		}
	    }
            current_area = NULL;

	    for ( ; ; )
	    {
		char *word;

		if ( fread_letter( fpArea ) != '#' )
		{
		    bug( "Boot_db: # not found.", 0 );
		    exit( 1 );
		}
		word = fread_word( fpArea );
		     if ( word[0] == '$'               )                 break;
		else if ( !str_cmp( word, "AREA"     ) ) { load_area    (fpArea); }
  /* OLC */     else if ( !str_cmp( word, "AREADATA" ) ) { new_load_area(fpArea); }
		else if ( !str_cmp( word, "HELPS"    ) ) { load_helps   (fpArea, strArea); }
		else if ( !str_cmp( word, "MOBOLD"   ) ) { load_old_mob (fpArea); }
		else if ( !str_cmp( word, "MOBILES"  ) ) { load_mobiles (fpArea); }
		else if ( !str_cmp( word, "OBJOLD"   ) ) { load_old_obj (fpArea); }
	  	else if ( !str_cmp( word, "OBJECTS"  ) ) { load_objects (fpArea); }
		else if ( !str_cmp( word, "RESETS"   ) ) { load_resets  (fpArea); }
		else if ( !str_cmp( word, "ROOMS"    ) ) { load_rooms   (fpArea); }
		else if ( !str_cmp( word, "SHOPS"    ) ) { load_shops   (fpArea); }
		else if ( !str_cmp( word, "SOCIALS"  ) ) { load_socials (fpArea); }
		else if ( !str_cmp( word, "SPECIALS" ) ) { load_specials(fpArea); }
		else if ( !str_cmp( word, "IMPROGS"  ) ) { load_improgs (fpArea); }
                else if ( !str_cmp( word, "ROOMDATA"  ) ) { load_rooms_new (fpArea); }
                else if ( !str_cmp( word, "MOBDATA" ) ) { load_mobiles_new(fpArea); }
                else if ( !str_cmp( word, "OBJDATA"  ) ) { load_objects_new (fpArea); }
		else
		{
		    bug( "Boot_db: bad section name.", 0 );
		    exit( 1 );
		}
	    }
	    if ( fpArea != stdin )
		fclose( fpArea );
	    fpArea = NULL;
	}
	fclose( fpList );
    }
    {
	fBootDb  = FALSE;	/* Exit boot status before limit load */
    }

    {
	FILE *fpChar_list;
	FILE *fpChar;
	char strPlr[MAX_INPUT_LENGTH];
	char chkbuf[MAX_STRING_LENGTH];
	char pbuf[100];

        log_string("Loading object counts off players now...");
	sprintf(pbuf,"ls %s%s > %s",PLAYER_DIR,"*.plr",PLAYER_LIST);
	system(pbuf);

	if ((fpChar_list = fopen( PLAYER_LIST, "r")) == NULL)
	{
		perror(PLAYER_LIST);
		exit(1);
	}

	for (; ;)
	{
	 strcpy(strPlr, fread_word(fpChar_list) );
	log_string(strPlr);
	 sprintf(chkbuf,"%s%s",PLAYER_DIR,"Zzz.plr");
	 if (!str_cmp(strPlr,chkbuf))
	 break;
	 if ( (	fpChar = fopen(strPlr, "r") ) == NULL)
	 {
		perror(strPlr);
		exit(1);
	 }
	num_pfiles++;
	for (; ;)
	{
	char letter;
	char *word;
	long vnum;
	char objbuf[MAX_STRING_LENGTH];
	OBJ_INDEX_DATA *pObjIndex;

	letter = fread_letter(fpChar);
	if (letter != '#')
		continue;
	word = fread_word(fpChar);

	if (!str_cmp(word,"End"))
		break;

	// Read player stats.
	if ( !str_cmp( word, "PLAYER"))
		{
			for ( ; ; )
			{
			word   = fread_word( fpChar );
			if(!str_cmp(word,"Name"))
				strcpy(temp_player_name,fread_string( fpChar ));
			if(!str_cmp(word,"Levl"))
				temp_level=fread_number( fpChar );
			if(!str_cmp(word,"Race"))
				race_occurance[race_lookup(fread_string(fpChar))]++;
			if(!str_cmp(word,"Cabal"))
				cabal_members[cabal_lookup(fread_string(fpChar))]++;
			if(!str_cmp(word,"Cla"))
				class_occurance[fread_number(fpChar)]++;
			if(!str_cmp(word,"HMV"))
			{
				disregard	= fread_number(fpChar);
				char_max_hp	= fread_number(fpChar);
				disregard	= fread_number(fpChar);
				disregard	= fread_number(fpChar);
				disregard	= fread_number(fpChar);
				disregard	= fread_number(fpChar);
			}
			if(!str_cmp(word,"Plyd"))
			{
				char_hours=(fread_number(fpChar)/3600);
				totalhrsplayed+=char_hours;
			}
			if(!str_cmp(word,"Act"))
			{
				plr_act_flags=fread_word(fpChar);
				if(char_hours>most_hours && temp_level<52 && !strstr(plr_act_flags,"M"))
				{
					most_hours=char_hours;
					strcpy(most_hours_player,temp_player_name);
					smash_tilde(most_hours_player);
				}
			}
			if(!str_cmp(word,"Alig"))
			{
				if((tempalign=fread_number(fpChar))==-1000)
					align_occurance[0]++;
				if(tempalign==0)
					align_occurance[1]++;
				if(tempalign==1000)
					align_occurance[2]++;
			}
			if(!str_cmp(word,"TSex"))
			{
				sex_occurance[fread_number(fpChar)]++;
				break;
			}
			}
		}
	if ( !str_cmp( word, "O") || !str_cmp( word, "OBJECT"))
		{
		word = fread_word(fpChar);
		if (!str_cmp(word, "Vnum"))
			{
			vnum = fread_number(fpChar);
			if ( (get_obj_index(vnum)) == NULL)
			{
			bug("Bad obj vnum in limits: %d",vnum); 
				}
			else
				{
				pObjIndex = get_obj_index(vnum);
				pObjIndex->limcount++;

		if (pObjIndex->limtotal < 15  && pObjIndex->limtotal > 0)
			{
			sprintf(objbuf,"%s, %ld.",pObjIndex->short_descr,vnum);
			}
		}
		}
		}
	}
	
	fclose(fpChar);
	fpChar = NULL;
	}
	fclose( fpChar_list);
	//log_string("Object Limits Loaded.");
    }

    /*
     * Fix up exits.
     * Declare db booting over.
     * Reset all areas once.
     * Load up the songs, notes and ban files.
     *
     * Log strings added to help debugging.
     */
    {

	fix_exits( );
	log_string("Exits Fixed.");

	fBootDb	= FALSE;
        convert_objects( );           /* ROM OLC */

	area_update( );
	log_string("Area Update.");

	load_cabal_items();
	log_string("Cabal Items: Loaded.");

	logItemFix();
	log_string("Created item fix log.");

	printRaceInfo();
	log_string("Created race info file.");

	log_string("Loading bans.");
	// load_bans();
	log_string("Bans loaded.");

	pruneDatabase();

	fp = fopen(LOGIN_SCREEN_FILE, "r");
	
	//Printed carriage return to prevent use of strcat with unitialized buffer
	sprintf(buf, "\r");
	while(fgets(tempbuf,200,fp))
	{
		strcat(buf,tempbuf);
		strcat(buf,"\r");
	}
	
	fclose(fp);
	chop(buf);
	chop(buf);
	help_greeting = str_dup(buf);
	log_string("Login screen loaded.");

	num_pfiles	= 20;

    }

    return;
}



/*
 * Snarf an 'area' header line.
 */
void load_area( FILE *fp )
{
    AREA_DATA *pArea;

    pArea		= alloc_perm( sizeof(*pArea) );
/*  pArea->reset_first	= NULL;
    pArea->reset_last	= NULL; */
    pArea->file_name	= fread_string(fp);

    pArea->area_flags   = AREA_LOADING;         /* OLC */
    pArea->security     = 9;                    /* OLC */ /* 9 -- Hugin */
    pArea->builders     = str_dup( "None" );    /* OLC */
    pArea->vnum         = top_area;             /* OLC */

    pArea->name		= fread_string( fp );
    pArea->credits	= fread_string( fp );
    pArea->min_vnum	= fread_number(fp);
    pArea->max_vnum	= fread_number(fp);
    pArea->age		= 15;
    pArea->nplayer	= 0;
    pArea->empty	= FALSE;

    if ( !area_first )
	area_first = pArea;
    if ( area_last )
    {
	area_last->next = pArea;
        REMOVE_BIT(area_last->area_flags, AREA_LOADING);        /* OLC */
    }
    area_last	= pArea;
    pArea->next	= NULL;
    current_area = pArea;

    top_area++;
    return;
}

/*
 * OLC
 * Use these macros to load any new area formats that you choose to
 * support on your MUD.  See the new_load_area format below for
 * a short example.
 */
#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )                \
                if ( !str_cmp( word, literal ) )    \
                {                                   \
                    field  = value;                 \
                    fMatch = TRUE;                  \
                    break;                          \
                                }

#define SKEY( string, field )                       \
                if ( !str_cmp( word, string ) )     \
                {                                   \
                    free_string( field );           \
                    field = fread_string( fp );     \
                    fMatch = TRUE;                  \
                    break;                          \
                                }



/* OLC
 * Snarf an 'area' header line.   Check this format.  MUCH better.  Add fields
 * too.
 *
 * #AREAFILE
 * Name   { All } Locke    Newbie School~
 * Repop  A teacher pops in the room and says, 'Repop coming!'~
 * Recall 3001
 * End
 */
void new_load_area( FILE *fp )
{
    AREA_DATA *pArea;
    char      *word;
    bool      fMatch;

    pArea               	= alloc_perm( sizeof(*pArea) );
    pArea->age          	= 15;
    pArea->nplayer      	= 0;
    pArea->file_name     	= str_dup( strArea );
    pArea->vnum         	= top_area;
    pArea->name         	= str_dup( "New Area" );
    pArea->builders     	= str_dup( "" );
    pArea->security     	= 9;                    /* 9 -- Hugin */
    pArea->min_vnum        	= 0;
    pArea->max_vnum        	= 0;
    pArea->area_flags   	= 0;
    pArea->explore		= FALSE;

    for ( ; ; )
    {
       word   = feof( fp ) ? "End" : fread_word( fp );
       fMatch = FALSE;

       switch ( UPPER(word[0]) )
       {
           case 'N':
            SKEY( "Name", pArea->name );
            break;
           case 'S':
             KEY( "Security", pArea->security, fread_number( fp ) );
            break;
           case 'V':
            if ( !str_cmp( word, "VNUMs" ) )
            {
                pArea->min_vnum = fread_number( fp );
                pArea->max_vnum = fread_number( fp );
            }
            break;
           case 'E':
             if ( !str_cmp( word, "End" ) )
             {
                 fMatch = TRUE;
                 if ( area_first == NULL )
                    area_first = pArea;
                 if ( area_last  != NULL )
                    area_last->next = pArea;
                 area_last   = pArea;
                 pArea->next = NULL;
                    current_area = pArea;
                 top_area++;
                 return;
            }
            break;
           case 'B':
            SKEY( "Builders", pArea->builders );
            break;
	   case 'C':
	    SKEY( "Credits", pArea->credits );
	    break;
           case 'X':
             KEY( "Xplore", pArea->explore, fread_number( fp ) );
            break;
        }
    }
}

/*
 * Sets vnum range for area using OLC protection features.
 */
void assign_area_vnum( long vnum ) //leave it with int vnum.
{
    if ( area_last->min_vnum == 0 || area_last->max_vnum == 0 )
        area_last->min_vnum = area_last->max_vnum = vnum;
    if ( vnum != URANGE( area_last->min_vnum, vnum, area_last->max_vnum ) )
	{
        if ( vnum < area_last->min_vnum )
            area_last->min_vnum = vnum;
        else
            area_last->max_vnum = vnum;
	}
    return;
}

//Materials file read into a recursive list -Zornath
int load_materials(FILE * fp) {
	MAT_DATA *new;
	int count = 0;
	
	//Comments
	while (strcmp(fread_word(fp),"MATS")) {fread_to_eol(fp);}
	
	while (!feof(fp)) {
		new = (MAT_DATA*) alloc_perm( sizeof(*new) );
		new->name = str_dup(fread_string(fp));
		new->relative_protection_pierce = fread_number(fp);
		new->relative_protection_bash = fread_number(fp);
		new->relative_protection_slash = fread_number(fp);
		new->relative_protection_magic = fread_number(fp);
		new->relative_weight = fread_number(fp);
		//other readins here

		new->next = mat_list;
		mat_list = new;
		count++;

		fread_to_eol(fp);
	}
	
	return count;
}

/*
 * Snarf a help section.
 */
void load_helps (FILE * fp, char *fname)
{
    HELP_DATA *pHelp;
    int level;
    char *keyword;

    for (;;)
    {
        HELP_AREA *had;

        level = fread_number (fp);
        keyword = fread_string (fp);

        if (keyword[0] == '$')
            break;

        if (!had_list)
        {
            had = new_had ();
            had->filename = str_dup (fname);
            had->area = current_area;
            if (current_area)
                current_area->helps = had;
            had_list = had;
        }
        else if (str_cmp (fname, had_list->filename))
        {
            had = new_had ();
            had->filename = str_dup (fname);
            had->area = current_area;
            if (current_area)
                current_area->helps = had;
            had->next = had_list;
            had_list = had;
        }
        else
            had = had_list;

        pHelp = new_help ();
        pHelp->level = level;
        pHelp->keyword = keyword;
        pHelp->text = fread_string (fp);

	//mysql_safe_query("DELETE FROM helpfiles");
	/*mysql_safe_query("INSERT INTO helpfiles VALUES(%d, '%s', '%s', %d)",
		pHelp->level,
		pHelp->keyword,
		pHelp->text,
		pHelp->level > 51 ? 1 : 0);*/

        if (!str_cmp (pHelp->keyword, "greeting"))
            help_greeting = pHelp->text;

        if (help_first == NULL)
            help_first = pHelp;
        if (help_last != NULL)
            help_last->next = pHelp;

        help_last = pHelp;
        pHelp->next = NULL;

        if (!had->first)
            had->first = pHelp;
        if (!had->last)
            had->last = pHelp;

        had->last->next_area = pHelp;
        had->last = pHelp;
        pHelp->next_area = NULL;

        top_help++;
    }

    return;
}



/*
 * Snarf a mob section.  old style
 */
void load_old_mob( FILE *fp )
{
    MOB_INDEX_DATA *pMobIndex;
    /* for race updating */
    int race;
    char name[MAX_STRING_LENGTH];

    if ( !area_last )   /* OLC */
    {
        bug( "Load_mobiles: no #AREA seen yet.", 0 );
        exit( 1 );
    }

    for ( ; ; )
    {
	long vnum;
	char letter;
	int iHash;

	letter				= fread_letter( fp );
	if ( letter != '#' )
	{
	    bug( "Load_mobiles: # not found.", 0 );
	    exit( 1 );
	}

	vnum				= fread_number( fp );
	if ( vnum == 0 )
	    break;

	fBootDb = FALSE;
	if ( get_mob_index( vnum ) != NULL )
	{
	    bug( "Load_mobiles: vnum %d duplicated.", vnum );
	    exit( 1 );
	}
	fBootDb = TRUE;

	pMobIndex			= alloc_perm( sizeof(*pMobIndex) );
	pMobIndex->vnum			= vnum;
        pMobIndex->area                 = area_last;               /* OLC */
	pMobIndex->new_format		= FALSE;
	pMobIndex->player_name		= fread_string( fp );
	pMobIndex->short_descr		= fread_string( fp );
	pMobIndex->long_descr		= fread_string( fp );
	pMobIndex->description		= fread_string( fp );

	pMobIndex->long_descr[0]	= UPPER(pMobIndex->long_descr[0]);
	pMobIndex->description[0]	= UPPER(pMobIndex->description[0]);

	pMobIndex->act			= fread_flag( fp ) | ACT_IS_NPC;
	//pMobIndex->affected_by		= fread_flag( fp );
	pMobIndex->pShop		= NULL;
	pMobIndex->alignment		= fread_number( fp );
	letter				= fread_letter( fp );
	pMobIndex->level		= fread_number( fp );

	/*
	 * The unused stuff is for imps who want to use the old-style
	 * stats-in-files method.
	 */
					  fread_number( fp );	/* Unused */
					  fread_number( fp );	/* Unused */
					  fread_number( fp );	/* Unused */
	/* 'd'		*/		  fread_letter( fp );	/* Unused */
					  fread_number( fp );	/* Unused */
	/* '+'		*/		  fread_letter( fp );	/* Unused */
					  fread_number( fp );	/* Unused */
					  fread_number( fp );	/* Unused */
	/* 'd'		*/		  fread_letter( fp );	/* Unused */
					  fread_number( fp );	/* Unused */
	/* '+'		*/		  fread_letter( fp );	/* Unused */
					  fread_number( fp );	/* Unused */
        pMobIndex->wealth               = fread_number( fp )/20;
	pMobIndex->quest_credit_reward	= fread_number( fp );
	/* xp can't be used! */		  fread_number( fp );	/* Unused */
	pMobIndex->start_pos		= fread_number( fp );	/* Unused */
	pMobIndex->default_pos		= fread_number( fp );	/* Unused */

  	if (pMobIndex->start_pos < POS_SLEEPING)
	    pMobIndex->start_pos = POS_STANDING;
	if (pMobIndex->default_pos < POS_SLEEPING)
	    pMobIndex->default_pos = POS_STANDING;

	/*
	 * Back to meaningful values.
	 */
	pMobIndex->sex			= fread_number( fp );

    	/* compute the race BS */
   	one_argument(pMobIndex->player_name,name);

   	if (name[0] == '\0' || (race =  race_lookup(name)) == 0)
   	{
            /* fill in with blanks */
            pMobIndex->race = race_lookup("human");
            pMobIndex->off_flags = OFF_DODGE|OFF_DISARM|OFF_TRIP|ASSIST_VNUM;
            pMobIndex->imm_flags = 0;
            pMobIndex->res_flags = 0;
            pMobIndex->vuln_flags = 0;
            pMobIndex->form = FORM_EDIBLE|FORM_SENTIENT|FORM_BIPED|FORM_MAMMAL;
            pMobIndex->parts = PART_HEAD|PART_ARMS|PART_LEGS|PART_HEART|
                               PART_BRAINS|PART_GUTS;
    	}
    	else
    	{
            pMobIndex->race = race;
            pMobIndex->off_flags = OFF_DODGE|OFF_DISARM|OFF_TRIP|ASSIST_RACE|
                                   race_table[race].off;
            pMobIndex->imm_flags = race_table[race].imm;
            pMobIndex->res_flags = race_table[race].res;
            pMobIndex->vuln_flags = race_table[race].vuln;
            pMobIndex->form = race_table[race].form;
            pMobIndex->parts = race_table[race].parts;
    	}

	if ( letter != 'S' )
	{
	    bug( "Load_mobiles: vnum %d non-S.", vnum );
	    exit( 1 );
	}

	convert_mobile( pMobIndex );                           /* ROM OLC */

	iHash			= vnum % MAX_KEY_HASH;
	pMobIndex->next		= mob_index_hash[iHash];
	mob_index_hash[iHash]	= pMobIndex;
	top_mob_index++;
        top_vnum_mob = top_vnum_mob < vnum ? vnum : top_vnum_mob;  /* OLC */
        assign_area_vnum( vnum );                                  /* OLC */
	kill_table[URANGE(0, pMobIndex->level, MAX_LEVEL-1)].number++;
    }

    return;
}

/*
 * Snarf an obj section.  old style
 */
void load_old_obj( FILE *fp )
{
    OBJ_INDEX_DATA *pObjIndex;

    if ( !area_last )   /* OLC */
    {
        bug( "Load_objects: no #AREA seen yet.", 0 );
        exit( 1 );
    }

    for ( ; ; )
    {
	long vnum;
	char letter;
	int iHash;

	letter				= fread_letter( fp );
	if ( letter != '#' )
	{
	    bug( "Load_objects: # not found.", 0 );
	    exit( 1 );
	}

	vnum				= fread_number( fp );
	if ( vnum == 0 )
	    break;

	fBootDb = FALSE;
	if ( get_obj_index( vnum ) != NULL )
	{
	    bug( "Load_objects: vnum %d duplicated.", vnum );
	    exit( 1 );
	}
	fBootDb = TRUE;

	pObjIndex			= alloc_perm( sizeof(*pObjIndex) );
	pObjIndex->vnum			= vnum;
        pObjIndex->area                 = area_last;
	pObjIndex->new_format		= FALSE;
	pObjIndex->reset_num	 	= 0;
	pObjIndex->name			= fread_string( fp );
	pObjIndex->short_descr		= fread_string( fp );
	pObjIndex->description		= fread_string( fp );
	/* Action description */	  fread_string( fp );

	pObjIndex->short_descr[0]	= LOWER(pObjIndex->short_descr[0]);
	pObjIndex->description[0]	= UPPER(pObjIndex->description[0]);
	pObjIndex->material		= find_material_by_name("none");

	pObjIndex->item_type		= fread_number( fp );
	//pObjIndex->extra_flags		= fread_flag( fp );
	load_old_bits(&pObjIndex->extra_flags, fp);
	pObjIndex->wear_flags		= fread_flag( fp );
	pObjIndex->value[0]		= fread_number( fp );
	pObjIndex->value[1]		= fread_number( fp );
	pObjIndex->value[2]		= fread_number( fp );
	pObjIndex->value[3]		= fread_number( fp );
	pObjIndex->value[4]		= 0;
	pObjIndex->level		= 0;
	pObjIndex->condition 		= 100;
	pObjIndex->weight		= fread_number( fp );
	pObjIndex->cost			= fread_number( fp );	/* Unused */
	/* Cost per day */		  fread_number( fp );


	if (pObjIndex->item_type == ITEM_WEAPON)
	{
	    if (is_name("two",pObjIndex->name)
	    ||  is_name("two-handed",pObjIndex->name)
            ||  is_name("spear",pObjIndex->name)
            ||  is_name("staff",pObjIndex->name)
            ||  is_name("polearm",pObjIndex->name)
            ||  is_name("halberd",pObjIndex->name)
            ||  is_name("spike",pObjIndex->name)
	    ||  is_name("claymore",pObjIndex->name))
		SET_BIT(pObjIndex->value[4],WEAPON_TWO_HANDS);
	}

 

	for ( ; ; )
	{
	    char letter;

	    letter = fread_letter( fp );

	    if ( letter == 'A' )
	    {
		AFFECT_DATA *paf;

		paf			= alloc_perm( sizeof(*paf) );
		paf->where		= TO_OBJECT;
		paf->type		= -1;
		paf->level		= 20; /* RT temp fix */
		paf->duration		= -1;
		paf->location		= fread_number( fp );
		paf->modifier		= fread_number( fp );
		paf->bitvector		= 0;
		paf->next		= pObjIndex->affected;
		pObjIndex->affected	= paf;
		top_affect++;
	    }
	    else if ( letter == 'C' )
                pObjIndex->cabal = fread_number( fp );
	    else if (letter == 'L' )
		pObjIndex->limtotal = fread_number( fp );
	    else if (letter == 'R')
		pObjIndex->restrict_flags = fread_flag( fp );
	    else if ( letter == 'E' )
	    {
		EXTRA_DESCR_DATA *ed;

		ed			= alloc_perm( sizeof(*ed) );
		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		ed->next		= pObjIndex->extra_descr;
		pObjIndex->extra_descr	= ed;
		top_ed++;
	    }

	    else
	    {
		ungetc( letter, fp );
		break;
	    }
	}

        /* fix armors */
        if (pObjIndex->item_type == ITEM_ARMOR)
        {
            pObjIndex->value[1] = pObjIndex->value[0];
            pObjIndex->value[2] = pObjIndex->value[1];
        }

	/*
	 * Translate spell "slot numbers" to internal "skill numbers."
	 */
	switch ( pObjIndex->item_type )
	{
	case ITEM_PILL:
	case ITEM_POTION:
	case ITEM_SCROLL:
	    pObjIndex->value[1] = slot_lookup( pObjIndex->value[1] );
	    pObjIndex->value[2] = slot_lookup( pObjIndex->value[2] );
	    pObjIndex->value[3] = slot_lookup( pObjIndex->value[3] );
	    pObjIndex->value[4] = slot_lookup( pObjIndex->value[4] );
	    break;

	case ITEM_STAFF:
	case ITEM_WAND:
	    pObjIndex->value[3] = slot_lookup( pObjIndex->value[3] );
	    break;
	}

	iHash			= vnum % MAX_KEY_HASH;
	pObjIndex->next		= obj_index_hash[iHash];
	obj_index_hash[iHash]	= pObjIndex;
	top_obj_index++;
        top_vnum_obj = top_vnum_obj < vnum ? vnum : top_vnum_obj;   /* OLC */
        assign_area_vnum( vnum );                                   /* OLC */
    }

    return;
}

/*
 * Adds a reset to a room.  OLC
 * Similar to add_reset in olc.c
 */
void new_reset( ROOM_INDEX_DATA *pR, RESET_DATA *pReset )
{
    RESET_DATA *pr;

    if ( !pR )
       return;

    pr = pR->reset_last;

    if ( !pr )
    {
        pR->reset_first = pReset;
        pR->reset_last  = pReset;
    }
    else
    {
        pR->reset_last->next = pReset;
        pR->reset_last       = pReset;
        pR->reset_last->next = NULL;
    }

    top_reset++;
    return;
}

/*
 * Snarf a reset section.
 */
void load_resets( FILE *fp )
{
    int         iLastRoom = 0;
    int         iLastObj  = 0;

    if ( !area_last )
    {
	bug( "Load_resets: no #AREA seen yet.", 0 );
	exit( 1 );
    }

    for ( ; ; )
    {
		RESET_DATA *pReset;
		ROOM_INDEX_DATA *pRoomIndex;
		EXIT_DATA *pexit;
		char letter;
		OBJ_INDEX_DATA *temp_index;

		if ( ( letter = fread_letter( fp ) ) == 'S' )
		    break;

		if ( letter == '*' )
		{
		    fread_to_eol( fp );
		    continue;
		}

		pReset		= (RESET_DATA *)alloc_perm( sizeof(*pReset) );
		pReset->command	= letter;
		/* if_flag */	  fread_number( fp );
		pReset->arg1	= fread_number( fp );
		pReset->arg2	= fread_number( fp );
		pReset->arg3	= (letter == 'G' || letter == 'R')
				    ? 0 : fread_number( fp );
		pReset->arg4	= (letter == 'P' || letter == 'M')
				    ? fread_number(fp) : 0;
				  fread_to_eol( fp );

		/*
		 * Validate parameters.
		 * We're calling the index functions for the side effect.
		 */
		switch ( letter )
		{
			default:
			    bug( "Load_resets: bad command '%c'.", letter );
			    exit( 1 );
			    break;

			case 'M':
			    //get_mob_index  ( pReset->arg1 );
		            if ( ( pRoomIndex = get_room_index ( pReset->arg3 ) ) )
		            {
		                new_reset( pRoomIndex, pReset );
		                iLastRoom = pReset->arg3;
		            }
			    break;

			case 'O':
			    temp_index = get_obj_index  ( pReset->arg1 );
			    temp_index->reset_num++;
		            if ( ( pRoomIndex = get_room_index ( pReset->arg3 ) ) )
		            {
		                new_reset( pRoomIndex, pReset );
		                iLastObj = pReset->arg3;
		            }
			    break;

			case 'P':
			    temp_index = get_obj_index  ( pReset->arg1 );
			    temp_index->reset_num++;
		            if ( ( pRoomIndex = get_room_index ( iLastObj ) ) )
		            {
		                new_reset( pRoomIndex, pReset );
		            }
			    break;

			case 'G':
			case 'E':
			    temp_index = get_obj_index  ( pReset->arg1 );
			    temp_index->reset_num++;
		            if ( ( pRoomIndex = get_room_index ( iLastRoom ) ) )
		            {
		                new_reset( pRoomIndex, pReset );
		                iLastObj = iLastRoom;
		            }
			    break;

			case 'D':
			    pRoomIndex = get_room_index( pReset->arg1 );

			    if ( pReset->arg2 < 0
			    ||   pReset->arg2 > (MAX_DIR - 1)
		            || !pRoomIndex
			    || !( pexit = pRoomIndex->exit[pReset->arg2] )
			    || !IS_SET( pexit->rs_flags, EX_ISDOOR ) )
			    {
				bug( "Load_resets: 'D': exit %d not door.", pReset->arg2 );
				exit( 1 );
			    }

		            switch ( pReset->arg3 )
			    {
		                default:
				bug( "Load_resets: 'D': bad 'locks': %d.", pReset->arg3 );
		                case 0: break;
		                case 1: SET_BIT( pexit->rs_flags, EX_CLOSED );
					SET_BIT( pexit->exit_info, EX_CLOSED ); break;
		                case 2: SET_BIT( pexit->rs_flags, EX_CLOSED | EX_LOCKED );
		                	SET_BIT( pexit->exit_info, EX_CLOSED | EX_LOCKED ); break;
			    }

			    break;

			case 'R':
			    pRoomIndex		= get_room_index( pReset->arg1 );

			    if ( pReset->arg2 < 0 || pReset->arg2 > MAX_DIR )
			    {
				bug( "Load_resets: 'R': bad exit %d.", pReset->arg2 );
				exit( 1 );
			    }

		            if ( pRoomIndex )
		                new_reset( pRoomIndex, pReset );

			    break;
		}
    }

    return;
}

/*
 * Snarf a room section.
 */
void load_rooms( FILE *fp )
{
    ROOM_INDEX_DATA *pRoomIndex;

    if ( area_last == NULL )
    {
	bug( "Load_resets: no #AREA seen yet.", 0 );
	exit( 1 );
    }

    for ( ; ; )
    {
	long vnum;
	char letter;
	int door;
	int iHash;

	letter				= fread_letter( fp );
	if ( letter != '#' )
	{
	    bug( "Load_rooms: # not found.", 0 );
	    exit( 1 );
	}

	vnum				= fread_number( fp );
	if ( vnum == 0 )
	    break;

	fBootDb = FALSE;
	if ( get_room_index( vnum ) != NULL )
	{
	    bug( "Load_rooms: vnum %d duplicated.", vnum );
	    exit( 1 );
	}
	fBootDb = TRUE;

	pRoomIndex			= alloc_perm( sizeof(*pRoomIndex) );
	pRoomIndex->owner		= str_dup("");
	pRoomIndex->move_progs		= FALSE;
	pRoomIndex->people		= NULL;
	pRoomIndex->contents		= NULL;
	pRoomIndex->extra_descr		= NULL;
	pRoomIndex->area		= area_last;
	pRoomIndex->vnum		= vnum;
	pRoomIndex->name		= fread_string( fp );
	pRoomIndex->description		= fread_string( fp );

	/* Area number */		  fread_number( fp );
	pRoomIndex->room_flags		= fread_flag( fp );

	pRoomIndex->sector_type		= fread_number( fp );
	pRoomIndex->light		= 0;
	pRoomIndex->cabal		= 0;

	for ( door = 0; door <= 5; door++ )
	    pRoomIndex->exit[door] = NULL;

	/* defaults */
	pRoomIndex->heal_rate = 100;
	pRoomIndex->mana_rate = 100;

	for ( ; ; )
	{
	    letter = fread_letter( fp );

	    if ( letter == 'S' )
		break;

	    if ( letter == 'H') /* healing room */
		pRoomIndex->heal_rate = fread_number(fp);

	    else if ( letter == 'M') /* mana room */
		pRoomIndex->mana_rate = fread_number(fp);

	    else if ( letter == 'G') /* Guilds...added by Ceran */
		pRoomIndex->guild = fread_number(fp);

           else if ( letter == 'C') /* Cabal */
	   {
                if (pRoomIndex->cabal)
	  	{
                    bug("Load_rooms: duplicate cabal fields.",0);
		    exit(1);
		}
                pRoomIndex->cabal = cabal_lookup(fread_string(fp));
	    }
	    else if ( letter == 'D' )
   	    {
     		EXIT_DATA *pexit;
     		door = fread_number (fp );
     		if ( door < 0 || door > 5 )
     		{
       			bug( "Fread_rooms: vnum %d has bad door number.", vnum );
       			exit( 1 );
 		}
     		pexit                = alloc_perm( sizeof(*pexit) ); 
		pexit->description   = fread_string( fp );
		pexit->keyword       = fread_string( fp );
		pexit->exit_info     = 0;
		pexit->rs_flags      = fread_flag( fp );
		pexit->orig_door     = door;
		pexit->key           = fread_number( fp );
		pexit->u1.vnum       = fread_number( fp );
		pRoomIndex->exit[door] = pexit;
		top_exit++;
            }
	    else if ( letter == 'E' )
	    {
		EXTRA_DESCR_DATA *ed;

		ed			= alloc_perm( sizeof(*ed) );
		ed->keyword		= fread_string( fp );
		ed->description		= fread_string( fp );
		ed->next		= pRoomIndex->extra_descr;
		pRoomIndex->extra_descr	= ed;
		top_ed++;
	    }

	    else if (letter == 'O')
	    {
		if (pRoomIndex->owner[0] != '\0')
		{
		    bug("Load_rooms: duplicate owner.",0);
		    exit(1);
		}

		pRoomIndex->owner = fread_string(fp);
	    }
            else if ( letter == 'C' )
            {
                pRoomIndex->cabal = cabal_lookup(fread_word(fp));
            }
	    else
	    {
		bug( "Load_rooms: vnum %d has flag not 'DES'.", vnum );
		exit( 1 );
	    }
	}

	iHash			= vnum % MAX_KEY_HASH;
	pRoomIndex->next	= room_index_hash[iHash];
	room_index_hash[iHash]	= pRoomIndex;
	top_room++;
        top_vnum_room = top_vnum_room < vnum ? vnum : top_vnum_room; /* OLC */
        assign_area_vnum( vnum );                                    /* OLC */
    }

    return;
}



/*
 * Snarf a shop section.
 */
void load_shops( FILE *fp )
{
    SHOP_DATA *pShop;

    for ( ; ; )
    {
	MOB_INDEX_DATA *pMobIndex;
	int iTrade;

	pShop			= alloc_perm( sizeof(*pShop) );
	pShop->keeper		= fread_number( fp );
	if ( pShop->keeper == 0 )
	    break;
	for ( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
	    pShop->buy_type[iTrade]	= fread_number( fp );
	pShop->profit_buy	= fread_number( fp );
	pShop->profit_sell	= fread_number( fp );
	pShop->open_hour	= fread_number( fp );
	pShop->close_hour	= fread_number( fp );
				  fread_to_eol( fp );
	pMobIndex		= get_mob_index( pShop->keeper );
	pMobIndex->pShop	= pShop;

	if ( shop_first == NULL )
	    shop_first = pShop;
	if ( shop_last  != NULL )
	    shop_last->next = pShop;

	shop_last	= pShop;
	pShop->next	= NULL;
	top_shop++;
    }

    return;
}


/*
 * Snarf spec proc declarations.
 */
void load_specials( FILE *fp )
{
    for ( ; ; )
    {
	MOB_INDEX_DATA *pMobIndex;
	char letter;

	switch ( letter = fread_letter( fp ) )
	{
	default:
	    bug( "Load_specials: letter '%c' not *MS.", letter );
	    exit( 1 );

	case 'S':
	    return;

	case '*':
	    break;

	case 'M':
	    pMobIndex		= get_mob_index	( fread_number ( fp ) );
	    pMobIndex->spec_fun	= spec_lookup	( fread_word   ( fp ) );
	    if ( pMobIndex->spec_fun == 0 )
	    {
		bug( "Load_specials: 'M': vnum %d.", pMobIndex->vnum );
		exit( 1 );
	    }
	    break;
	}

	fread_to_eol( fp );
    }
}

/* LOAD CABAL ITEMS */

void load_cabal_items(void)
{
	FILE *fp;
	OBJ_DATA *obj;
	CHAR_DATA *mob;
	int inum, vnum;

	fp=fopen(CABAL_ITEMS_FILE,"r");
	if(!fp)
		return;
	while((inum = fread_number(fp)) != -1)
	{
		vnum = fread_number(fp);
		obj = create_object(get_obj_index(cabal_table[inum].item_vnum),0);
		if(!vnum)
		{
			for(mob = char_list; mob; mob = mob->next)
				if(mob->cabal == inum && IS_CABAL_GUARD(mob))
					obj_to_char(obj, mob);
			continue;
		}
		for(mob = char_list; mob; mob = mob->next)
			if(mob->pIndexData->vnum == vnum)
				obj_to_char(obj, mob);
	}
	fclose(fp);
}
/*
 * Translate all room exits from virtual to real.
 * Has to be done after all rooms are read in.
 * Check for bad reverse exits.
 */
void fix_exits( void )
{
    extern const sh_int rev_dir [];
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    EXIT_DATA *pexit_rev;
    int iHash;
    int door;

    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
	for ( pRoomIndex  = room_index_hash[iHash];
	      pRoomIndex != NULL;
	      pRoomIndex  = pRoomIndex->next )
	{
	    bool fexit;

	    fexit = FALSE;
	    for ( door = 0; door <= 5; door++ )
	    {
		if ( ( pexit = pRoomIndex->exit[door] ) != NULL )
		{
		    if ( pexit->u1.vnum <= 0
		    || get_room_index(pexit->u1.vnum) == NULL)
			pexit->u1.to_room = NULL;
		    else
		    {
		   	fexit = TRUE;
			pexit->u1.to_room = get_room_index( pexit->u1.vnum );
		    }
		}
	    }
	    if (!fexit)
		SET_BIT(pRoomIndex->room_flags,ROOM_NO_MOB);
	}
    }

    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
	for ( pRoomIndex  = room_index_hash[iHash];
	      pRoomIndex != NULL;
	      pRoomIndex  = pRoomIndex->next )
	{
	    for ( door = 0; door <= 5; door++ )
	    {
		if ( ( pexit     = pRoomIndex->exit[door]       ) != NULL
		&&   ( to_room   = pexit->u1.to_room            ) != NULL
		&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
		&&   pexit_rev->u1.to_room != pRoomIndex
		&&   (pRoomIndex->vnum < 1200 || pRoomIndex->vnum > 1299))
		{
		    sprintf( buf, "Fix_exits: %ld:%d -> %ld:%d -> %ld.",
			pRoomIndex->vnum, door,
			to_room->vnum,    rev_dir[door],
			(pexit_rev->u1.to_room == NULL)
			    ? 0 : pexit_rev->u1.to_room->vnum );
/*		    bug( buf, 0 ); */
		}
	    }
	}
    }

    return;
}


/*
 * Repopulate areas periodically.
 */
void area_update( void )
{
    AREA_DATA *pArea;
    char buf[MAX_STRING_LENGTH];

    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {

	if ( ++pArea->age < 3 )
	    continue;

	/*
	 * Check age and reset.
	 * Note: Mud School resets every 3 minutes (not 15).
	 */
	if ( (!pArea->empty && (pArea->nplayer == 0 || pArea->age >= 15) && !pArea->explore)
	||    (pArea->age >= 31 && !pArea->explore)
	||	(pArea->explore && pArea->nplayer == 0 && pArea->age >= 15))
	{
	    ROOM_INDEX_DATA *pRoomIndex;

	    reset_area( pArea );
	    sprintf(buf,"%s has just been reset.",pArea->name);
	    wiznet(buf,NULL,NULL,WIZ_RESETS,0,0);
	    pArea->age = number_range( 0, 3 );
	    pRoomIndex = get_room_index( ROOM_VNUM_SCHOOL );
	    if ( pRoomIndex != NULL && pArea == pRoomIndex->area )
		pArea->age = 15 - 2;
	    else if ( (pRoomIndex = get_room_index(9401)) != NULL
	&& pRoomIndex->area == pArea)
		pArea->age = 15-2;
	    else if ( (pRoomIndex = get_room_index(23100)) != NULL
	&& pRoomIndex->area == pArea)
		pArea->age = 15-2;
	    else if ( (pRoomIndex = get_room_index(26000)) != NULL
	&& pRoomIndex->area == pArea)
		pArea->age = 15-2;
	    else if (pArea->nplayer == 0)
		pArea->empty = TRUE;
	}
    }

    return;
}



 /* OLC
  * Reset one room.  Called by reset_area and olc.
 */
void reset_room( ROOM_INDEX_DATA *pRoom )
{
    RESET_DATA *pReset;
     CHAR_DATA   *pMob;
    CHAR_DATA *mob;
     OBJ_DATA    *pObj;
     CHAR_DATA   *LastMob = NULL;
     OBJ_DATA    *LastObj = NULL;
     int iExit;
      int level = 0;
    bool last;
//    int level;

     if ( !pRoom )
         return;

     pMob        = NULL;
     last        = FALSE;

     for ( iExit = 0;  iExit < MAX_DIR;  iExit++ )
      {
         EXIT_DATA *pExit;
         if ( ( pExit = pRoom->exit[iExit] )
 	  /*  && !IS_SET( pExit->exit_info, EX_BASHED )   ROM OLC */ )
         {
             pExit->exit_info = pExit->rs_flags;
             if ( ( pExit->u1.to_room != NULL )
               && ( ( pExit = pExit->u1.to_room->exit[rev_dir[iExit]] ) ) )
             {
                 /* nail the other side */
                 pExit->exit_info = pExit->rs_flags;
             }
         }
     }

     for ( pReset = pRoom->reset_first; pReset != NULL; pReset = pReset->next )
    {
	MOB_INDEX_DATA *pMobIndex;
	OBJ_INDEX_DATA *pObjIndex;
	OBJ_INDEX_DATA *pObjToIndex;
         ROOM_INDEX_DATA *pRoomIndex;
 	char buf[MAX_STRING_LENGTH];
 	int count,limit=0;

	switch ( pReset->command )
	{
	default:
                 bug( "Reset_room: bad command %c.", pReset->command );
	    break;

	case 'M':
             if ( !( pMobIndex = get_mob_index( pReset->arg1 ) ) )
	    {
                 bug( "Reset_room: 'M': bad vnum %d.", pReset->arg1 );
		continue;
	    }

	    if ( ( pRoomIndex = get_room_index( pReset->arg3 ) ) == NULL )
	    {
		bug( "Reset_area: 'R': bad vnum %d.", pReset->arg3 );
		continue;
	    }
	    if ( pMobIndex->count >= pReset->arg2 )
	    {
		last = FALSE;
		break;
	    }
 /* */

	    count = 0;
	    for (mob = pRoomIndex->people; mob != NULL; mob = mob->next_in_room)
		if (mob->pIndexData == pMobIndex)
		{
		    count++;
		    if (count >= pReset->arg4)
		    {
		    	last = FALSE;
		    	break;
		    }
		}

	    if (count >= pReset->arg4)
		break;

 /* */

             pMob = create_mobile( pMobIndex );

             /*
              * Some more hard coding.
              */
             if ( room_is_dark( pRoom ) )
                 SET_BIT(pMob->affected_by, AFF_INFRARED);

	    /*
	     * Check for pet shop.
	     */
	    {
		ROOM_INDEX_DATA *pRoomIndexPrev;

                 pRoomIndexPrev = get_room_index( pRoom->vnum - 1 );
                 if ( pRoomIndexPrev
		&&   IS_SET(pRoomIndexPrev->room_flags, ROOM_PET_SHOP) )
                     SET_BIT( pMob->act, ACT_PET);
	    }

             char_to_room( pMob, pRoom );

             LastMob = pMob;
             level  = URANGE( 0, pMob->level - 2, LEVEL_HERO - 1 ); /* -1 ROM */
	    last  = TRUE;
	    break;

	case 'O':
             if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
	    {
                 bug( "Reset_room: 'O' 1 : bad vnum %d", pReset->arg1 );
                 sprintf (buf,"%ld %ld %ld %ld",pReset->arg1, pReset->arg2, pReset->arg3,
                 pReset->arg4 );
 		bug(buf,1);
		continue;
	    }

             if ( !( pRoomIndex = get_room_index( pReset->arg3 ) ) )
	    {
                 bug( "Reset_room: 'O' 2 : bad vnum %d.", pReset->arg3 );
                 sprintf (buf,"%ld %ld %ld %ld",pReset->arg1, pReset->arg2, pReset->arg3,
                 pReset->arg4 );
 		bug(buf,1);
		continue;
	    }

             if ( pRoom->area->nplayer > 0
               || count_obj_list( pObjIndex, pRoom->contents ) > 0 )
	    {
		last = FALSE;
		break;
	    }

             pObj = create_object( pObjIndex,              /* UMIN - ROM OLC */
 				  UMIN(number_fuzzy( level ), LEVEL_HERO -1) );
             pObj->cost = 0;
             obj_to_room( pObj, pRoom );
	    last = TRUE;
	    break;

	case 'P':
             if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
	    {
                 bug( "Reset_room: 'P': bad vnum %d.", pReset->arg1 );
		continue;
	    }

             if ( !( pObjToIndex = get_obj_index( pReset->arg3 ) ) )
	    {
                 bug( "Reset_room: 'P': bad vnum %d.", pReset->arg3 );
		continue;
	    }

            if (pReset->arg2 > 50) /* old format */
                limit = 6;
            else if (pReset->arg2 == -1) /* no limit */
                limit = 999;
            else
                limit = pReset->arg2;

             if ( pRoom->area->nplayer > 0
               || ( LastObj = get_obj_type( pObjToIndex ) ) == NULL
               || ( LastObj->in_room == NULL && !last)
               || ( pObjIndex->count >= limit /* && number_range(0,4) != 0 */ )
	       || ( pObjIndex->limcount >= pObjIndex->limtotal && pObjIndex->limtotal > 0 )
		|| ( count = count_obj_list( pObjIndex, LastObj->contains ) ) > pReset->arg4  )

	    {
		last = FALSE;
		break;
	    }
 				                /* lastObj->level  -  ROM */

	    while (count < pReset->arg4)
	    {
             pObj = create_object( pObjIndex, number_fuzzy( LastObj->level ) );
             obj_to_obj( pObj, LastObj );
		count++;
  		/*if (pObjIndex->count >= limit)
		    break;*/
		if (pObjIndex->count >= limit
		&& pObjIndex->limcount >= pObjIndex->limtotal
		&& pObjIndex->limtotal > 0)
		break;


	    }

	    /* fix object lock state! */
 	    LastObj->value[1] = LastObj->pIndexData->value[1];
	    last = TRUE;
	    break;

	case 'G':
	case 'E':
             if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
	    {
                 bug( "Reset_room: 'E' or 'G': bad vnum %d.", pReset->arg1 );
		continue;
	    }

	    if ( !last )
		break;

             if ( !LastMob )
	    {
                 bug( "Reset_room: 'E' or 'G': null mob for vnum %d.",
		    pReset->arg1 );
		last = FALSE;
		break;
	    }

             if ( LastMob->pIndexData->pShop )   /* Shop-keeper? */
	    {
		int olevel = 0,i,j;

		if (!pObjIndex->new_format)
		    switch ( pObjIndex->item_type )
		{
                 default:                olevel = 0;                      break;
		case ITEM_PILL:
		case ITEM_POTION:
		case ITEM_SCROLL:
		    olevel = 53;
		    for (i = 1; i < 5; i++)
		    {
			if (pObjIndex->value[i] > 0)
			{
		    	    for (j = 0; j < MAX_CLASS; j++)
			    {
				olevel = UMIN(olevel,
				         skill_table[pObjIndex->value[i]].
						     skill_level[j]);
			    }
			}
		    }

		    olevel = UMAX(0,(olevel * 3 / 4) - 2);
		    break;

		case ITEM_WAND:		olevel = number_range( 10, 20 ); break;
		case ITEM_STAFF:	olevel = number_range( 15, 25 ); break;
		case ITEM_ARMOR:	olevel = number_range(  5, 15 ); break;
		case ITEM_WEAPON:	olevel = number_range(  5, 15 ); break;
		case ITEM_TREASURE:	olevel = number_range( 10, 20 ); break;
		/*}*/

 #if 0 /* envy version */
                 case ITEM_WEAPON:       if ( pReset->command == 'G' )
                                             olevel = number_range( 5, 15 );
                                         else
                                             olevel = number_fuzzy( level );
 #endif /* envy version */

                   break;
	    }

                 pObj = create_object( pObjIndex, olevel );
 		set_bit( &pObj->extra_flags, ITEM_INVENTORY );  /* ROM OLC */

#if 0 /* envy version */
                 if ( pReset->command == 'G' )
                     SET_BIT( pObj->extra_flags, ITEM_INVENTORY );
#endif /* envy version */

             }
 	    else   /* ROM OLC else version */
	    {
 		int limit;
		if (pReset->arg2 > 50) /* old format */
		    limit = 6;
 		else if ( pReset->arg2 == -1 || pReset->arg2 == 0 )  /* no limit */
		    limit = 999;
		else
		    limit = pReset->arg2;

// 		if ( pObjIndex->count < limit || number_range(0,4) == 0 )

	if ( pObjIndex->limcount >= pObjIndex->limtotal
	&& pObjIndex->limtotal > 0 && FALSE )
	{
		break;
	}
		else
		if ( pObjIndex->count < limit || number_range(0,4) == 0 )


		{
 		    pObj = create_object( pObjIndex,
 			   UMIN( number_fuzzy( level ), LEVEL_HERO - 1 ) );
  		    /* error message if it is too high */
 		    if (pObj->level > LastMob->level + 3
 		    ||  (pObj->item_type == ITEM_WEAPON
  		    &&   pReset->command == 'E'
 		    &&   pObj->level < LastMob->level -5 && pObj->level < 45));
  			 /* fprintf(stderr,
  			    "Err: obj %s (%d) -- %d, mob %s (%d) -- %d\n",
 			    pObj->short_descr,pObj->pIndexData->vnum,pObj->level,
 			    LastMob->short_descr,LastMob->pIndexData->vnum,LastMob->level);*/
		}
		else
		    break;
		    }

#if 0 /* envy else version */
             else
	    {
                 pObj = create_object( pObjIndex, number_fuzzy( level ) );
	    }
#endif /* envy else version */

             obj_to_char( pObj, LastMob );
             if ( pReset->command == 'E' )
                 equip_char( LastMob, pObj, pReset->arg3 );
	    last = TRUE;
	    break;

         case 'D':
             break;

	case 'R':
             if ( !( pRoomIndex = get_room_index( pReset->arg1 ) ) )
	    {
                 bug( "Reset_room: 'R': bad vnum %d.", pReset->arg1 );
		continue;
	    }

	    {
                 EXIT_DATA *pExit;
		int d0;
		int d1;

		for ( d0 = 0; d0 < pReset->arg2 - 1; d0++ )
		{
		    d1                   = number_range( d0, pReset->arg2-1 );
                     pExit                = pRoomIndex->exit[d0];
		    pRoomIndex->exit[d0] = pRoomIndex->exit[d1];
                     pRoomIndex->exit[d1] = pExit;
		}
	    }
	    break;
	}
    }

    return;
}

 /* OLC
  * Reset one area.
  */
 void reset_area( AREA_DATA *pArea )
 {
     ROOM_INDEX_DATA *pRoom;
     long  vnum;

     for ( vnum = pArea->min_vnum; vnum <= pArea->max_vnum; vnum++ )
     {
         if ( ( pRoom = get_room_index(vnum) ) )
            reset_room(pRoom); 
    }
    return;
}



/*
 * Create an instance of a mobile.
 */
CHAR_DATA *create_mobile( MOB_INDEX_DATA *pMobIndex )
{
    CHAR_DATA *mob;
    int i;
    AFFECT_DATA af;

    mobile_count++;

    if ( pMobIndex == NULL )
    {
	bug( "Create_mobile: NULL pMobIndex.", 0 );
	exit( 1 );
    }

    mob = new_char();

    mob->pIndexData	= pMobIndex;

    mob->name		= str_dup( pMobIndex->player_name );    /* OLC */
	mob->original_name	= str_dup ( pMobIndex->player_name );
    mob->short_descr	= str_dup( pMobIndex->short_descr );    /* OLC */
	mob->original_name	= str_dup ( pMobIndex->player_name );
    mob->long_descr	= str_dup( pMobIndex->long_descr );     /* OLC */
    mob->description	= str_dup( pMobIndex->description );    /* OLC */
    mob->id		= get_mob_id();
    mob->spec_fun	= pMobIndex->spec_fun;
    mob->prompt		= NULL;
    mob->progtypes      = pMobIndex->progtypes;

/*
    if (pMobIndex->wealth == 0)
    {
	mob->gold   = 0;
    }
    else
    {
	long wealth;

	wealth = number_range(pMobIndex->wealth/2, 3 * pMobIndex->wealth/2);
	mob->gold = number_range(wealth/200,wealth/100);
    }
*/

    if (pMobIndex->quest_credit_reward <= 0)
    {
       mob->quest_credits = 0;
    }
    else
    {
       mob->quest_credits = pMobIndex->quest_credit_reward;
    }



    if (pMobIndex->new_format)
    /* load in new style */
    {
	/* read from prototype */
 	mob->group		= pMobIndex->group;
	mob->act 		= pMobIndex->act;
	mob->affected_by	= pMobIndex->affected_by;
	mob->alignment		= pMobIndex->alignment;
	mob->level		= pMobIndex->level;
	mob->hitroll		= pMobIndex->hitroll;
	mob->numAttacks		= pMobIndex->numAttacks;
	mob->damroll		= pMobIndex->damage[DICE_BONUS];
	mob->enhancedDamMod	= pMobIndex->enhancedDamMod;
	mob->regen_rate		= pMobIndex->regen_rate;
	mob->max_hit		= dice(pMobIndex->hit[DICE_NUMBER],
				       pMobIndex->hit[DICE_TYPE])
				  + pMobIndex->hit[DICE_BONUS];
	mob->hit		= mob->max_hit;
	mob->max_mana		= dice(pMobIndex->mana[DICE_NUMBER],
				       pMobIndex->mana[DICE_TYPE])
				  + pMobIndex->mana[DICE_BONUS];
	mob->mana		= mob->max_mana;
	mob->damage[DICE_NUMBER]= pMobIndex->damage[DICE_NUMBER];
	mob->damage[DICE_TYPE]	= pMobIndex->damage[DICE_TYPE];
	mob->dam_type		= pMobIndex->dam_type;
        if (mob->dam_type == 0)
    	    switch(number_range(1,3))
            {
                case (1): mob->dam_type = 3;        break;  /* slash */
                case (2): mob->dam_type = 7;        break;  /* pound */
                case (3): mob->dam_type = 11;       break;  /* pierce */
            }
	for (i = 0; i < 4; i++)
	    mob->armor[i]	= pMobIndex->ac[i];
	mob->off_flags		= pMobIndex->off_flags;
	mob->imm_flags		= pMobIndex->imm_flags;
	mob->res_flags		= pMobIndex->res_flags;
	mob->vuln_flags		= pMobIndex->vuln_flags;
	mob->start_pos		= pMobIndex->start_pos;
	mob->default_pos	= pMobIndex->default_pos;
	mob->sex		= pMobIndex->sex;
        if (mob->sex == 3) /* random sex */
            mob->sex = number_range(1,2);
	mob->race		= pMobIndex->race;
	mob->form		= pMobIndex->form;
	mob->parts		= pMobIndex->parts;
	mob->size		= pMobIndex->size;
	if (pMobIndex->material)
		mob->material           = str_dup(pMobIndex->material);
	else
		mob->material = str_dup("none");
     	mob->dam_mod		= pMobIndex->dam_mod;
	mob->extended_flags	= pMobIndex->extended_flags;
	
	/* computed on the spot */

    	for (i = 0; i < MAX_STATS; i ++)
            mob->perm_stat[i] = UMIN(25,11 + mob->level/4);

        if (IS_SET(mob->act,ACT_WARRIOR))
        {
            mob->perm_stat[STAT_STR] += 3;
            mob->perm_stat[STAT_INT] -= 1;
            mob->perm_stat[STAT_CON] += 2;
        }

        if (IS_SET(mob->act,ACT_THIEF))
        {
            mob->perm_stat[STAT_DEX] += 3;
            mob->perm_stat[STAT_INT] += 1;
            mob->perm_stat[STAT_WIS] -= 1;
        }

        if (IS_SET(mob->act,ACT_CLERIC))
        {
            mob->perm_stat[STAT_WIS] += 3;
            mob->perm_stat[STAT_DEX] -= 1;
            mob->perm_stat[STAT_STR] += 1;
        }

        if (IS_SET(mob->act,ACT_MAGE))
        {
            mob->perm_stat[STAT_INT] += 3;
            mob->perm_stat[STAT_STR] -= 1;
            mob->perm_stat[STAT_DEX] += 1;
        }

        if (IS_SET(mob->off_flags,OFF_FAST))
            mob->perm_stat[STAT_DEX] += 2;

        mob->perm_stat[STAT_STR] += mob->size - SIZE_MEDIUM;
        mob->perm_stat[STAT_CON] += (mob->size - SIZE_MEDIUM) / 2;

	/* let's get some spell action */

	if (IS_AFFECTED(mob,AFF_SANCTUARY))
	{
        init_affect(&af);
	    af.where	 = TO_AFFECTS;
	    af.type      = skill_lookup("sanctuary");
        af.aftype    = AFT_COMMUNE;
	    af.level     = mob->level;
	    af.duration  = -1;
	    af.location  = APPLY_NONE;
		
		char msg_buf[MSL];
		sprintf(msg_buf,"reduces damage taken by %d",DAM_REDUX_SANC);
		af.affect_list_msg	= str_dup(msg_buf);
	    af.modifier  = 0;
	    af.bitvector = 0;
	}

	if (IS_AFFECTED(mob,AFF_HASTE))
	{
	    init_affect(&af);
	    af.where	 = TO_AFFECTS;
	    af.aftype 	= AFT_SPELL;
	    af.type      = skill_lookup("haste");
    	    af.level     = mob->level;
      	    af.duration  = -1;
    	    af.location  = APPLY_DEX;
    	    af.modifier  = 1 + (mob->level >= 18) + (mob->level >= 25) +
			   (mob->level >= 32);
    	    af.bitvector = AFF_HASTE;
			
			char msg_buf[MSL];
			sprintf(msg_buf,"increases likelihood of attacks and dexterity by %d",af.modifier);
			af.affect_list_msg = str_dup(msg_buf);
    	    affect_to_char( mob, &af );
	}

	if (IS_AFFECTED(mob,AFF_PROTECT_EVIL))
	{
	    init_affect(&af);
	    af.where	 = TO_AFFECTS;
	af.aftype = AFT_COMMUNE;
	    af.type	 = skill_lookup("protection evil");
	    af.level	 = mob->level;
	    af.duration	 = -1;
	    af.location	 = APPLY_SAVES;
	    af.modifier	 = -1;
	    af.bitvector = AFF_PROTECT_EVIL;
		char msg_buf[MSL];
		sprintf(msg_buf,"reduces damage taken from evil opponents by %d",DAM_REDUX_PROTECTION_EVIL);
		af.affect_list_msg = str_dup(msg_buf);
	    affect_to_char(mob,&af);
	}

        if (IS_AFFECTED(mob,AFF_PROTECT_GOOD))
        {
            init_affect(&af);
	    af.where	 = TO_AFFECTS;
	af.aftype = AFT_SPELL;
            af.type      = skill_lookup("protection good");
            af.level     = mob->level;
            af.duration  = -1;
            af.location  = APPLY_SAVES;
            af.modifier  = -1;
            af.bitvector = AFF_PROTECT_GOOD;
			char msg_buf[MSL];
			sprintf(msg_buf,"reduces damage taken from good opponents by %d",DAM_REDUX_PROTECTION_GOOD);
			af.affect_list_msg = str_dup(msg_buf);
            affect_to_char(mob,&af);
        }
    }

    else /* read in old format and convert */
    {
	mob->act		= pMobIndex->act;
	mob->affected_by	= pMobIndex->affected_by;
	mob->alignment		= pMobIndex->alignment;
	mob->level		= pMobIndex->level;
	mob->hitroll		= pMobIndex->hitroll;
	mob->numAttacks		= pMobIndex->numAttacks;
	mob->damroll		= 0;
	mob->enhancedDamMod	= pMobIndex->enhancedDamMod;
	mob->regen_rate		= pMobIndex->regen_rate;
	mob->max_hit		= mob->level * 8 + number_range(
					mob->level * mob->level/4,
					mob->level * mob->level);
	mob->max_hit *= .9;
	mob->hit		= mob->max_hit;
	mob->max_mana		= 100 + dice(mob->level,10);
	mob->mana		= mob->max_mana;
	switch(number_range(1,3))
	{
	    case (1): mob->dam_type = 3; 	break;  /* slash */
	    case (2): mob->dam_type = 7;	break;  /* pound */
	    case (3): mob->dam_type = 11;	break;  /* pierce */
	}
	for (i = 0; i < 3; i++)
	    mob->armor[i]	= interpolate(mob->level,100,-100);
	mob->armor[3]		= interpolate(mob->level,100,0);
	mob->race		= pMobIndex->race;
	mob->off_flags		= pMobIndex->off_flags;
	mob->imm_flags		= pMobIndex->imm_flags;
	mob->res_flags		= pMobIndex->res_flags;
	mob->vuln_flags		= pMobIndex->vuln_flags;
	mob->start_pos		= pMobIndex->start_pos;
	mob->default_pos	= pMobIndex->default_pos;
	mob->sex		= pMobIndex->sex;
	mob->form		= pMobIndex->form;
	mob->parts		= pMobIndex->parts;
	mob->size		= SIZE_MEDIUM;
	mob->material		= str_dup("");

        for (i = 0; i < MAX_STATS; i ++)
            mob->perm_stat[i] = 11 + mob->level/4;
    }

    mob->position = mob->start_pos;
    mob->cabal		= mob->pIndexData->cabal;

    /* link the mob to the world list */
    mob->next		= char_list;
    char_list		= mob;
    pMobIndex->count++;
    return mob;
}

/* duplicate a mobile exactly -- except inventory */
void clone_mobile(CHAR_DATA *parent, CHAR_DATA *clone)
{
    int i;
    AFFECT_DATA *paf;

    if ( parent == NULL || clone == NULL || !IS_NPC(parent))
	return;

    /* start fixing values */
    clone->name 	= str_dup(parent->name);
    clone->version	= parent->version;
    clone->short_descr	= str_dup(parent->short_descr);
    clone->long_descr	= str_dup(parent->long_descr);
    clone->description	= str_dup(parent->description);
    clone->group	= parent->group;
    clone->sex		= parent->sex;
    clone->class	= parent->class;
    clone->race		= parent->race;
    clone->level	= parent->level;
    clone->trust	= 0;
    clone->timer	= parent->timer;
    clone->wait		= parent->wait;
    clone->hit		= parent->hit;
    clone->max_hit	= parent->max_hit;
    clone->mana		= parent->mana;
    clone->max_mana	= parent->max_mana;
    clone->move		= parent->move;
    clone->max_move	= parent->max_move;
    clone->gold		= parent->gold;
    clone->quest_credits = parent->quest_credits;
    clone->exp		= parent->exp;
    clone->act		= parent->act;
    clone->comm		= parent->comm;
    clone->imm_flags	= parent->imm_flags;
    clone->res_flags	= parent->res_flags;
    clone->vuln_flags	= parent->vuln_flags;
    clone->invis_level	= parent->invis_level;
    clone->affected_by	= parent->affected_by;
    clone->position	= parent->position;
    clone->practice	= parent->practice;
    clone->train	= parent->train;
    clone->saving_throw	= parent->saving_throw;
    clone->alignment	= parent->alignment;
    clone->hitroll	= parent->hitroll;
    clone->numAttacks 	= parent->numAttacks;
    clone->damroll	= parent->damroll;
    clone->enhancedDamMod = parent->enhancedDamMod;
	clone->regen_rate	= parent->regen_rate;
    clone->wimpy	= parent->wimpy;
    clone->form		= parent->form;
    clone->parts	= parent->parts;
    clone->size		= parent->size;
    clone->material	= str_dup(parent->material);
    clone->off_flags	= parent->off_flags;
    clone->dam_type	= parent->dam_type;
    clone->start_pos	= parent->start_pos;
    clone->default_pos	= parent->default_pos;
    clone->spec_fun	= parent->spec_fun;
    clone->dam_mod	= parent->dam_mod;

    for (i = 0; i < 4; i++)
    	clone->armor[i]	= parent->armor[i];

    for (i = 0; i < MAX_STATS; i++)
    {
	clone->perm_stat[i]	= parent->perm_stat[i];
	clone->mod_stat[i]	= parent->mod_stat[i];
    }

    for (i = 0; i < 3; i++)
	clone->damage[i]	= parent->damage[i];

    /* now add the affects */
    for (paf = parent->affected; paf != NULL; paf = paf->next)
        affect_to_char(clone,paf);

}




/*
 * Create an instance of an object.
 */
OBJ_DATA *create_object( OBJ_INDEX_DATA *pObjIndex, int level )
{
    AFFECT_DATA *paf;
    OBJ_DATA *obj;
    int i;

    if ( pObjIndex == NULL )
    {
	bug( "Create_object: NULL pObjIndex.", 0 );
	exit( 1 );
    }

    obj = new_obj();

    obj->pIndexData	= pObjIndex;
    obj->in_room	= NULL;
    obj->enchanted	= FALSE;
    obj->ohp		= 0;

    if (pObjIndex->new_format)
	obj->level = pObjIndex->level;
    else
	obj->level		= UMAX(0,level);
    obj->wear_loc	= -1;

	obj->owner = str_dup("none");
	obj->corpse_of = str_dup("none");
    obj->name		= str_dup( pObjIndex->name );           /* OLC */
    obj->short_descr	= str_dup( pObjIndex->short_descr );    /* OLC */
    obj->description	= str_dup( pObjIndex->description );    /* OLC */
    obj->material	= pObjIndex->material;
    obj->item_type	= pObjIndex->item_type;
    obj->extra_flags	= dup_bitmask(&pObjIndex->extra_flags);
    obj->wear_flags	= pObjIndex->wear_flags;
    obj->value[0]	= pObjIndex->value[0];
    obj->value[1]	= pObjIndex->value[1];
    obj->value[2]	= pObjIndex->value[2];
    obj->value[3]	= pObjIndex->value[3];
    obj->value[4]	= pObjIndex->value[4];
    obj->weight		= pObjIndex->weight;
   obj->progtypes      = pObjIndex->progtypes;

    if (level == -1 || pObjIndex->new_format)
	obj->cost	= pObjIndex->cost;
    else
    	obj->cost	= number_fuzzy( 10 )
			* number_fuzzy( level ) * number_fuzzy( level );
    obj->condition	= pObjIndex->condition;
    /*
     * Mess with object properties.
     */
    switch ( obj->item_type )
    {
    default:
	bug( "Read_object: vnum %d bad type.", pObjIndex->vnum );
	break;

    case ITEM_LIGHT:
	if (obj->value[2] == 999)
		obj->value[2] = -1;
	break;

    case ITEM_FURNITURE:
    case ITEM_TRASH:
    case ITEM_CONTAINER:
    case ITEM_DRINK_CON:
    case ITEM_KEY:
    case ITEM_FOOD:
    case ITEM_BOAT:
    case ITEM_CORPSE_NPC:
    case ITEM_CORPSE_PC:
    case ITEM_FOUNTAIN:
    case ITEM_MAP:
    case ITEM_CLOTHING:
    case ITEM_PORTAL:
	if (!pObjIndex->new_format)
	    obj->cost /= 5;
	break;

    case ITEM_TREASURE:
    case ITEM_INSTRUMENT:
    case ITEM_WARP_STONE:
    case ITEM_ROOM_KEY:
    case ITEM_GEM:
    case ITEM_JEWELRY:
    case ITEM_SKELETON:
	break;

    case ITEM_JUKEBOX:
	for (i = 0; i < 5; i++)
	   obj->value[i] = -1;
	break;

    case ITEM_SCROLL:
	if (level != -1 && !pObjIndex->new_format)
	    obj->value[0]	= number_fuzzy( obj->value[0] );
	break;

    case ITEM_WAND:
    case ITEM_STAFF:
	if (level != -1 && !pObjIndex->new_format)
	{
	    obj->value[0]	= number_fuzzy( obj->value[0] );
	    obj->value[1]	= number_fuzzy( obj->value[1] );
	    obj->value[2]	= obj->value[1];
	}
	if (!pObjIndex->new_format)
	    obj->cost *= 2;
	break;

    case ITEM_WEAPON:
	if (level != -1 && !pObjIndex->new_format)
	{
	    obj->value[1] = number_fuzzy( number_fuzzy( 1 * level / 4 + 2 ) );
	    obj->value[2] = number_fuzzy( number_fuzzy( 3 * level / 4 + 6 ) );
	}
	break;

    case ITEM_ARMOR:
	if (level != -1 && !pObjIndex->new_format)
	{
	    obj->value[0]	= number_fuzzy( level / 5 + 3 );
	    obj->value[1]	= number_fuzzy( level / 5 + 3 );
	    obj->value[2]	= number_fuzzy( level / 5 + 3 );
	}
	break;

    case ITEM_POTION:
    case ITEM_PILL:
	if (level != -1 && !pObjIndex->new_format)
	    obj->value[0] = number_fuzzy( number_fuzzy( obj->value[0] ) );
	break;

    case ITEM_MONEY:
	if (!pObjIndex->new_format)
	    obj->value[0]	= obj->cost;
	break;
    }

    for (paf = pObjIndex->affected; paf != NULL; paf = paf->next)
	if ( paf->location == APPLY_SPELL_AFFECT )
	    affect_to_obj(obj,paf);

    obj->next		= object_list;
    object_list		= obj;
    pObjIndex->limcount += 1;

    return obj;
}

/* duplicate an object exactly -- except contents */
void clone_object(OBJ_DATA *parent, OBJ_DATA *clone)
{
    int i;
    AFFECT_DATA *paf;
    EXTRA_DESCR_DATA *ed,*ed_new;

    if (parent == NULL || clone == NULL)
	return;

    /* start fixing the object */
    clone->name 	= str_dup(parent->name);
    clone->short_descr 	= str_dup(parent->short_descr);
    clone->description	= str_dup(parent->description);
    clone->item_type	= parent->item_type;
    clone->extra_flags	= parent->extra_flags;
    clone->wear_flags	= parent->wear_flags;
    clone->weight	= parent->weight;
    clone->cost		= parent->cost;
    clone->level	= parent->level;
    clone->condition	= parent->condition;
    clone->material	= parent->material;
    clone->timer	= parent->timer;
    clone->ohp		= parent->ohp;

    for (i = 0;  i < 5; i ++)
	clone->value[i]	= parent->value[i];

    /* affects */
    clone->enchanted	= parent->enchanted;

    for (paf = parent->affected; paf != NULL; paf = paf->next)
	affect_to_obj(clone,paf);

    /* extended desc */
    for (ed = parent->extra_descr; ed != NULL; ed = ed->next)
    {
        ed_new                  = new_extra_descr();
        ed_new->keyword    	= str_dup( ed->keyword);
        ed_new->description     = str_dup( ed->description );
        ed_new->next           	= clone->extra_descr;
        clone->extra_descr  	= ed_new;
    }

}



/*
 * Clear a new character.
 */
void clear_char( CHAR_DATA *ch )
{
    static CHAR_DATA ch_zero;
    int i;

    *ch				= ch_zero;
    ch->name			= &str_empty[0];
    ch->short_descr		= &str_empty[0];
    ch->long_descr		= &str_empty[0];
    ch->description		= &str_empty[0];
    ch->prompt                  = &str_empty[0];
    ch->logon			= current_time;
    ch->lines			= PAGELEN;
    for (i = 0; i < 4; i++)
    	ch->armor[i]		= 100;
    ch->position		= POS_STANDING;
    ch->hit			= 20;
    ch->max_hit			= 20;
    ch->mana			= 100;
    ch->max_mana		= 100;
    ch->move			= 100;
    ch->max_move		= 100;
    ch->last_fought             = NULL;
    ch->last_fight_time    	= 0;
    ch->last_fight_name         = NULL;
    ch->on			= NULL;
    return;
}

/*
 * Get an extra description from a list.
 */
char *get_extra_descr( const char *name, EXTRA_DESCR_DATA *ed )
{
    for ( ; ed != NULL; ed = ed->next )
    {
	if ( is_name( (char *) name, ed->keyword ) )
	    return ed->description;
    }
    return NULL;
}



/*
 * Translates mob virtual number to its mob index struct.
 * Hash table lookup.
 */
MOB_INDEX_DATA *get_mob_index( long vnum )
{
    MOB_INDEX_DATA *pMobIndex;

    for ( pMobIndex  = mob_index_hash[vnum % MAX_KEY_HASH];
	  pMobIndex != NULL;
	  pMobIndex  = pMobIndex->next )
    {
	if ( pMobIndex->vnum == vnum )
	    return pMobIndex;
    }

    if ( fBootDb )
    {
	bug( "Get_mob_index: bad vnum %d.", vnum );
	exit( 1 );
    }

    return NULL;
}



/*
 * Translates mob virtual number to its obj index struct.
 * Hash table lookup.
 */
OBJ_INDEX_DATA *get_obj_index( long vnum )
{
    OBJ_INDEX_DATA *pObjIndex;

    for ( 	pObjIndex  = obj_index_hash[vnum % MAX_KEY_HASH];
			pObjIndex != NULL;
			pObjIndex  = pObjIndex->next )
    {
		if ( pObjIndex->vnum == vnum )
			return pObjIndex;
    }

    if ( fBootDb )
    {
		bug( "Get_obj_index: bad vnum %d.", vnum );
		exit( 1 );
    }

    return NULL;
}



/*
 * Translates mob virtual number to its room index struct.
 * Hash table lookup.
 */
ROOM_INDEX_DATA *get_room_index( long vnum )
{
	ROOM_INDEX_DATA *pRoomIndex;

	for ( pRoomIndex  = room_index_hash[vnum % MAX_KEY_HASH]; pRoomIndex != NULL; pRoomIndex  = pRoomIndex->next )
	{
	  if ( pRoomIndex->vnum == vnum )
		return pRoomIndex;
	}

	if ( fBootDb )
	{
	  bug( "Get_room_index: bad vnum %d.", vnum );
	  exit( 1 );
	}

	return NULL;
}

/*
 * Read a letter from a file.
 */
char fread_letter( FILE *fp )
{
    char c;

    do
    {
	c = getc( fp );
    }
    while ( isspace(c) );

    return c;
}



/*
 * Read a number from a file.
 */
int fread_number( FILE *fp )
{
    int number;
    bool sign;
    char c;

    do
    {
	c = getc( fp );
    }
    while ( isspace(c) );

    number = 0;

    sign   = FALSE;
    if ( c == '+' )
    {
	c = getc( fp );
    }
    else if ( c == '-' )
    {
	sign = TRUE;
	c = getc( fp );
    }

    if ( !isdigit(c) )
    {
	bug( "Fread_number: bad format.", 0 );
	exit( 1 );
    }

    while ( isdigit(c) )
    {
	number = number * 10 + c - '0';
	c      = getc( fp );
    }

    if ( sign )
	number = 0 - number;

    if ( c == '|' )
	number += fread_number( fp );
    else if ( c != ' ' )
	ungetc( c, fp );

    return number;
}

long fread_flag( FILE *fp)
{
    int number;
    char c;
    bool negative = FALSE;

    do
    {
	c = getc(fp);
    }
    while ( isspace(c));

    if (c == '-')
    {
	negative = TRUE;
	c = getc(fp);
    }

    number = 0;

    if (!isdigit(c))
    {
	while (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
	{
	    number += flag_convert(c);
	    c = getc(fp);
	}
    }

    while (isdigit(c))
    {
	number = number * 10 + c - '0';
	c = getc(fp);
    }

    if (c == '|')
	number += fread_flag(fp);

    else if  ( c != ' ')
	ungetc(c,fp);

    if (negative)
	return -1 * number;

    return number;
}

long flag_convert(char letter )
{
    long bitsum = 0;
    char i;

    if ('A' <= letter && letter <= 'Z')
    {
	bitsum = 1;
	for (i = letter; i > 'A'; i--)
	    bitsum *= 2;
    }
    else if ('a' <= letter && letter <= 'z')
    {
	bitsum = 67108864; /* 2^26 */
	for (i = letter; i > 'a'; i --)
	    bitsum *= 2;
    }

    return bitsum;
}




/*
 * Read and allocate space for a string from a file.
 * These strings are read-only and shared.
 * Strings are hashed:
 *   each string prepended with hash pointer to prev string,
 *   hash code is simply the string length.
 *   this function takes 40% to 50% of boot-up time.
 */
char *fread_string( FILE *fp )
{
    char *plast;
    char c;

    plast = top_string + sizeof(char *);
    if ( plast > &string_space[MAX_STRING - MAX_STRING_LENGTH] )
    {
	bug( "Fread_string: MAX_STRING %d exceeded.", MAX_STRING );
	exit( 1 );
    }

    /*
     * Skip blanks.
     * Read first char.
     */
    do
    {
	c = getc( fp );
    }
    while ( isspace(c) );

    if ( ( *plast++ = c ) == '~' )
	return &str_empty[0];

    for ( ;; )
    {
        /*
         * Back off the char type lookup,
         *   it was too dirty for portability.
         *   -- Furey
         */

	switch ( *plast = getc(fp) )
	{
        default:
            plast++;
            break;

        case EOF:
	/* temp fix */
            bug( "Fread_string: EOF", 0 );
	    return NULL;
            /* exit( 1 ); */
            break;

        case '\n':
            plast++;
            *plast++ = '\r';
            break;

        case '\r':
            break;

        case '~':
            plast++;
	    {
		union
		{
		    char *	pc;
		    char	rgc[sizeof(char *)];
		} u1;
		int ic;
		int iHash;
		char *pHash;
		char *pHashPrev;
		char *pString;

		plast[-1] = '\0';
		iHash     = UMIN( MAX_KEY_HASH - 1, plast - 1 - top_string );
		for ( pHash = string_hash[iHash]; pHash; pHash = pHashPrev )
		{
		    for ( ic = 0; ic < sizeof(char *); ic++ )
			u1.rgc[ic] = pHash[ic];
		    pHashPrev = u1.pc;
		    pHash    += sizeof(char *);

		    if ( top_string[sizeof(char *)] == pHash[0]
		    &&   !strcmp( top_string+sizeof(char *)+1, pHash+1 ) )
			return pHash;
		}

		if ( fBootDb )
		{
		    pString		= top_string;
		    top_string		= plast;
		    u1.pc		= string_hash[iHash];
		    for ( ic = 0; ic < sizeof(char *); ic++ )
			pString[ic] = u1.rgc[ic];
		    string_hash[iHash]	= pString;

		    nAllocString += 1;
		    sAllocString += top_string - pString;
		    return pString + sizeof(char *);
		}
		else
		{
		    return str_dup( top_string + sizeof(char *) );
		}
	    }
	}
    }
}

char *fread_string_eol( FILE *fp )
{
    static bool char_special[256-EOF];
    char *plast;
    char c;

    if ( char_special[EOF-EOF] != TRUE )
    {
        char_special[EOF -  EOF] = TRUE;
        char_special['\n' - EOF] = TRUE;
        char_special['\r' - EOF] = TRUE;
    }

    plast = top_string + sizeof(char *);
    if ( plast > &string_space[MAX_STRING - MAX_STRING_LENGTH] )
    {
        bug( "Fread_string: MAX_STRING %d exceeded.", MAX_STRING );
        exit( 1 );
    }

    /*
     * Skip blanks.
     * Read first char.
     */
    do
    {
        c = getc( fp );
    }
    while ( isspace(c) );

    if ( ( *plast++ = c ) == '\n')
        return &str_empty[0];

    for ( ;; )
    {
        if ( !char_special[ ( *plast++ = getc( fp ) ) - EOF ] )
            continue;

        switch ( plast[-1] )
        {
        default:
            break;

        case EOF:
            bug( "Fread_string_eol  EOF", 0 );
            exit( 1 );
            break;

        case '\n':  case '\r':
            {
                union
                {
                    char *      pc;
                    char        rgc[sizeof(char *)];
                } u1;
                int ic;
                int iHash;
                char *pHash;
                char *pHashPrev;
                char *pString;

                plast[-1] = '\0';
                iHash     = UMIN( MAX_KEY_HASH - 1, plast - 1 - top_string );
                for ( pHash = string_hash[iHash]; pHash; pHash = pHashPrev )
                {
                    for ( ic = 0; ic < sizeof(char *); ic++ )
                        u1.rgc[ic] = pHash[ic];
                    pHashPrev = u1.pc;
                    pHash    += sizeof(char *);

                    if ( top_string[sizeof(char *)] == pHash[0]
                    &&   !strcmp( top_string+sizeof(char *)+1, pHash+1 ) )
                        return pHash;
                }

                if ( fBootDb )
                {
                    pString             = top_string;
                    top_string          = plast;
                    u1.pc               = string_hash[iHash];
                    for ( ic = 0; ic < sizeof(char *); ic++ )
                        pString[ic] = u1.rgc[ic];
                    string_hash[iHash]  = pString;

                    nAllocString += 1;
                    sAllocString += top_string - pString;
                    return pString + sizeof(char *);
                }
                else
                {
                    return str_dup( top_string + sizeof(char *) );
                }
            }
        }
    }
}



/*
 * Read to end of line (for comments).
 */
void fread_to_eol( FILE *fp )
{
    char c;

    do
    {
	c = getc( fp );
    }
    while ( c != '\n' && c != '\r' );

    do
    {
	c = getc( fp );
    }
    while ( c == '\n' || c == '\r' );

    ungetc( c, fp );
    return;
}



/*
 * Read one word (into static buffer).
 */
char *fread_word( FILE *fp )
{
    static char word[MAX_INPUT_LENGTH];
    char *pword;
    char cEnd;

    do
    {
	cEnd = getc( fp );
    }
    while ( isspace( cEnd ) );

    if ( cEnd == '\'' || cEnd == '"' )
    {
	pword   = word;
    }
    else
    {
	word[0] = cEnd;
	pword   = word+1;
	cEnd    = ' ';
    }

    for ( ; pword < word + MAX_INPUT_LENGTH; pword++ )
    {
	*pword = getc( fp );
	if ( cEnd == ' ' ? isspace(*pword) : *pword == cEnd )
	{
	    if ( cEnd == ' ' )
		ungetc( *pword, fp );
	    *pword = '\0';
	    return word;
	}
    }

    bug( "Fread_word: word too long.", 0 );
    bug( word, 0 );
    exit( 1 );
    return NULL;
}


/*
 * Allocate some ordinary memory,
 *   with the expectation of freeing it someday.
 */
void *alloc_mem( int sMem )
{
    void *pMem;
    int *magic;
    int iList;

    sMem += sizeof(*magic);

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
    {
        if ( sMem <= rgSizeList[iList] )
            break;
    }

    if ( iList == MAX_MEM_LIST )
    {
        bug( "Alloc_mem: size %d too large.", sMem );
        exit( 1 );
    }

    if ( rgFreeList[iList] == NULL )
    {
        pMem              = alloc_perm( rgSizeList[iList] );
    }
    else
    {
        pMem              = rgFreeList[iList];
        rgFreeList[iList] = * ((void **) rgFreeList[iList]);
    }

    magic = (int *) pMem;
    *magic = MAGIC_NUM;
    pMem += sizeof(*magic);

    return pMem;
}



/*
 * Free some memory.
 * Recycle it back onto the free list for blocks of that size.
 */
void free_mem( void *pMem, int sMem )
{
    int iList;
    int *magic;

    pMem -= sizeof(*magic);
    magic = (int *) pMem;

    if (*magic != MAGIC_NUM)
    {
/*
        bug("Attempt to recyle invalid memory of size %d.",sMem);
        bug((char*) pMem + sizeof(*magic),0);
*/
        return;
    }

    *magic = 0;
    sMem += sizeof(*magic);

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
    {
        if ( sMem <= rgSizeList[iList] )
            break;
    }

    if ( iList == MAX_MEM_LIST )
    {
        bug( "Free_mem: size %d too large.", sMem );
        exit( 1 );
    }

    * ((void **) pMem) = rgFreeList[iList];
    rgFreeList[iList]  = pMem;

    return;
}


/*
 * Allocate some permanent memory.
 * Permanent memory is never freed,
 *   pointers into it may be copied safely.
 */
void *alloc_perm( int sMem )
{
    static char *pMemPerm;
    static int iMemPerm;
    void *pMem;

    while ( sMem % sizeof(long) != 0 )
	sMem++;
    if ( sMem > MAX_PERM_BLOCK )
    {
	bug( "Alloc_perm: %d too large.", sMem );
	exit( 1 );
    }

    if ( pMemPerm == NULL || iMemPerm + sMem > MAX_PERM_BLOCK )
    {
	iMemPerm = 0;
	if ( ( pMemPerm = calloc( 1, MAX_PERM_BLOCK ) ) == NULL )
	{
	    perror( "Alloc_perm" );
	    exit( 1 );
	}
    }

    pMem        = pMemPerm + iMemPerm;
    iMemPerm   += sMem;
    nAllocPerm += 1;
    sAllocPerm += sMem;
    return pMem;
}



/*
 * Duplicate a string into dynamic memory.
 * Fread_strings are read-only and shared.
 */
char *str_dup( const char *str )
{
    char *str_new;

    if ( str[0] == '\0' )
	return &str_empty[0];

    if ( str >= string_space && str < top_string )
	return (char *) str;

    str_new = alloc_mem( strlen(str) + 1 );
    strcpy( str_new, str );
    return str_new;
}



/*
 * Free a string.
 * Null is legal here to simplify callers.
 * Read-only shared strings are not touched.
 */
void free_string( char *pstr )
{
    if ( pstr == NULL
    ||   pstr == &str_empty[0]
    || ( pstr >= string_space && pstr < top_string ) )
	return;
	if(strlen(pstr) < MAX_STRING_LENGTH)
	    free_mem( pstr, strlen(pstr) + 1 );
	else
	    free_mem( pstr, strlen(pstr));
    return;
}



void do_areas( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    AREA_DATA *pArea1;
    AREA_DATA *pArea2;
    int iArea;
    int iAreaHalf;

    if (argument[0] != '\0')
    {
	send_to_char_bw("No argument is used with this command.\n\r",ch);
	return;
    }

    iAreaHalf = (top_area + 1) / 2;
    pArea1    = area_first;
    pArea2    = area_first;
    for ( iArea = 0; iArea < iAreaHalf; iArea++ )
	pArea2 = pArea2->next;

    for ( iArea = 0; iArea < iAreaHalf; iArea++ )
    {
	sprintf( buf, "%-39s%-39s\n\r",
	    pArea1->credits, (pArea2 != NULL) ? pArea2->credits : "" );
	send_to_char_bw( buf, ch );
	pArea1 = pArea1->next;
	if ( pArea2 != NULL )
	    pArea2 = pArea2->next;
    }

    return;
}



void do_memory( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    sprintf( buf, "Affects %5d\n\r", top_affect    ); send_to_char( buf, ch );
    sprintf( buf, "Areas   %5d\n\r", top_area      ); send_to_char( buf, ch );
    sprintf( buf, "ExDes   %5d\n\r", top_ed        ); send_to_char( buf, ch );
    sprintf( buf, "Exits   %5ld\n\r", top_exit      ); send_to_char( buf, ch );
    sprintf( buf, "Helps   %5ld\n\r", top_help      ); send_to_char( buf, ch );
    sprintf( buf, "Socials %5d\n\r", social_count  ); send_to_char( buf, ch );
    sprintf( buf, "Mobs    %5ld(%d new format)\n\r", top_mob_index,newmobs );
    send_to_char( buf, ch );
    sprintf( buf, "(in use)%5d\n\r", mobile_count  ); send_to_char( buf, ch );
    sprintf( buf, "Objs    %5ld(%d new format)\n\r", top_obj_index,newobjs );
    send_to_char( buf, ch );
    sprintf( buf, "Resets  %5ld\n\r", top_reset     ); send_to_char( buf, ch );
    sprintf( buf, "Rooms   %5ld\n\r", top_room      ); send_to_char( buf, ch );
    sprintf( buf, "Shops   %5ld\n\r", top_shop      ); send_to_char( buf, ch );

    sprintf( buf, "Strings %5d strings of %7d bytes (max %d).\n\r",
	nAllocString, sAllocString, MAX_STRING );
    send_to_char( buf, ch );

    sprintf( buf, "Perms   %5d blocks  of %7d bytes.\n\r",
	nAllocPerm, sAllocPerm );
    send_to_char( buf, ch );

    printf_to_char(ch,"MySQL Connection Active: %s\n\r", mysql_ping(&conn) == 0 ? "YES" : mysql_error(&conn));
    printf_to_char(ch,"Obsidian Prince Timer  : %d\n\r", obsidian_prince_timer);

    return;
}

void do_dump( CHAR_DATA *ch, char *argument )
{
    int count,count2,num_pcs,aff_count;
    CHAR_DATA *fch;
    MOB_INDEX_DATA *pMobIndex;
    PC_DATA *pc;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    ROOM_INDEX_DATA *room;
    EXIT_DATA *exit;
    DESCRIPTOR_DATA *d;
    AFFECT_DATA *af;
    FILE *fp;
    long vnum,nMatch = 0;

    /* open file */
    fclose(fpReserve);
    fp = fopen("mem.dmp","w");

    /* report use of data structures */

    num_pcs = 0;
    aff_count = 0;

    /* mobile prototypes */
    fprintf(fp,"MobProt	%4ld (%8ld bytes)\n",
	top_mob_index, top_mob_index * (sizeof(*pMobIndex)));

    /* mobs */
    count = 0;  count2 = 0;
    for (fch = char_list; fch != NULL; fch = fch->next)
    {
	count++;
	if (fch->pcdata != NULL)
	    num_pcs++;
	for (af = fch->affected; af != NULL; af = af->next)
	    aff_count++;
    }
    for (fch = char_free; fch != NULL; fch = fch->next)
	count2++;

    fprintf(fp,"Mobs	%4d (%8d bytes), %2d free (%d bytes)\n",
	count, count * (sizeof(*fch)), count2, count2 * (sizeof(*fch)));

    /* pcdata */
    count = 0;
    for (pc = pcdata_free; pc != NULL; pc = pc->next)
	count++;

    fprintf(fp,"Pcdata	%4d (%8d bytes), %2d free (%d bytes)\n",
	num_pcs, num_pcs * (sizeof(*pc)), count, count * (sizeof(*pc)));

    /* descriptors */
    count = 0; count2 = 0;
    for (d = descriptor_list; d != NULL; d = d->next)
	count++;
    for (d= descriptor_free; d != NULL; d = d->next)
	count2++;

    fprintf(fp, "Descs	%4d (%8d bytes), %2d free (%d bytes)\n",
	count, count * (sizeof(*d)), count2, count2 * (sizeof(*d)));

    /* object prototypes */
    for ( vnum = 0; nMatch < top_obj_index; vnum++ )
        if ( ( pObjIndex = get_obj_index( vnum ) ) != NULL )
        {
	    for (af = pObjIndex->affected; af != NULL; af = af->next)
		aff_count++;
            nMatch++;
        }

    fprintf(fp,"ObjProt	%4ld (%8ld bytes)\n",
	top_obj_index, top_obj_index * (sizeof(*pObjIndex)));


    /* objects */
    count = 0;  count2 = 0;
    for (obj = object_list; obj != NULL; obj = obj->next)
    {
	count++;
	for (af = obj->affected; af != NULL; af = af->next)
	    aff_count++;
    }
    for (obj = obj_free; obj != NULL; obj = obj->next)
	count2++;

    fprintf(fp,"Objs	%4d (%8d bytes), %2d free (%d bytes)\n",
	count, count * (sizeof(*obj)), count2, count2 * (sizeof(*obj)));

    /* affects */
    count = 0;
    for (af = affect_free; af != NULL; af = af->next)
	count++;

    fprintf(fp,"Affects	%4d (%8d bytes), %2d free (%d bytes)\n",
	aff_count, aff_count * (sizeof(*af)), count, count * (sizeof(*af)));

    /* rooms */
    fprintf(fp,"Rooms	%4ld (%8ld bytes)\n",
	top_room, top_room * (sizeof(*room)));

     /* exits */
    fprintf(fp,"Exits	%4ld (%8ld bytes)\n",
	top_exit, top_exit * (sizeof(*exit)));

    fclose(fp);

    /* start printing out mobile data */
    fp = fopen("mob.dmp","w");

    fprintf(fp,"\nMobile Analysis\n");
    fprintf(fp,  "---------------\n");
    nMatch = 0;
    for (vnum = 0; nMatch < top_mob_index; vnum++)
	if ((pMobIndex = get_mob_index(vnum)) != NULL)
	{
	    nMatch++;
	    fprintf(fp,"#%-4ld %3d active %3d killed     %s\n",
		pMobIndex->vnum,pMobIndex->count,
		pMobIndex->killed,pMobIndex->short_descr);
	}
    fclose(fp);

    /* start printing out object data */
    fp = fopen("obj.dmp","w");

    fprintf(fp,"\nObject Analysis\n");
    fprintf(fp,  "---------------\n");
    nMatch = 0;
    for (vnum = 0; nMatch < top_obj_index; vnum++)
	if ((pObjIndex = get_obj_index(vnum)) != NULL)
	{
	    nMatch++;
	    fprintf(fp,"#%-4ld %3d active %3d reset     %s\n",
		pObjIndex->vnum,pObjIndex->count,
		pObjIndex->reset_num,pObjIndex->short_descr);
	}

    /* close file */
    fclose(fp);
    fpReserve = fopen( NULL_FILE, "r" );
}



/*
 * Stick a little fuzz on a number.
 */
int number_fuzzy( int number )
{
    switch ( number_bits( 2 ) )
    {
    case 0:  number -= 1; break;
    case 3:  number += 1; break;
    }

    return UMAX( 1, number );
}



/*
 * Generate a random number.
 */
int number_range( int from, int to )
{
    int power;
    int number;

    if (from == 0 && to == 0)
	return 0;

    if ( ( to = to - from + 1 ) <= 1 )
	return from;

    for ( power = 2; power < to; power <<= 1 )
	;

    while ( ( number = number_mm() & (power -1 ) ) >= to )
	;

    return from + number;
}



/*
 * Generate a percentile roll.
 */
int number_percent( void )
{
    int percent;

    while ( (percent = number_mm() & (128-1) ) > 99 )
	;

    return 1 + percent;
}



/*
 * Generate a random door.
 */
int number_door( void )
{
    int door;

    while ( ( door = number_mm() & (8-1) ) > 5)
	;

    return door;
}

int number_bits( int width )
{
    return number_mm( ) & ( ( 1 << width ) - 1 );
}




/*
 * I've gotten too many bad reports on OS-supplied random number generators.
 * This is the Mitchell-Moore algorithm from Knuth Volume II.
 * Best to leave the constants alone unless you've read Knuth.
 * -- Furey
 */


void init_mm( )
{
    srandom(time(NULL)^getpid());
    return;
}



long number_mm( void )
{
    return random() >> 6;
}


/*
 * Roll some dice.
 */
int dice( int number, int size )
{
    int idice;
    int sum;

    switch ( size )
    {
    case 0: return 0;
    case 1: return number;
    }

    for ( idice = 0, sum = 0; idice < number; idice++ )
	sum += number_range( 1, size );

    return sum;
}



/*
 * Simple linear interpolation.
 */
int interpolate( int level, int value_00, int value_32 )
{
    return value_00 + level * (value_32 - value_00) / 32;
}



/*
 * Removes the tildes from a string.
 * Used for player-entered strings that go into disk files.
 */
void smash_tilde( char *str )
{
    for ( ; *str != '\0'; str++ )
    {
	if ( *str == '~' )
	    *str = '-';
    }

    return;
}



/*
 * Compare strings, case insensitive.
 * Return TRUE if different
 *   (compatibility with historical functions).
 */
bool str_cmp( const char *astr, const char *bstr )
{
    if ( astr == NULL )
    {
	if (bstr!=NULL)
		n_logf("null astr. bstr = %s", bstr);
	else
		bug( "Str_cmp: null astr.", 0 );
	return TRUE;
    }

    if ( bstr == NULL )
    {
	if (astr!=NULL)
		n_logf("Str_cmp: null bstr. astr = %s", astr);
	else
		bug( "Str_cmp: null bstr.", 0 );
	return TRUE;
    }

    for ( ; *astr || *bstr; astr++, bstr++ )
    {
	if ( LOWER(*astr) != LOWER(*bstr) )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Compare strings, case insensitive, for prefix matching.
 * Return TRUE if astr not a prefix of bstr
 *   (compatibility with historical functions).
 */
bool str_prefix( const char *astr, const char *bstr )
{
    if ( astr == NULL )
    {
	bug( "Strn_cmp: null astr.", 0 );
	return TRUE;
    }

    if ( bstr == NULL )
    {
	bug( "Strn_cmp: null bstr.", 0 );
	return TRUE;
    }

    for ( ; *astr; astr++, bstr++ )
    {
	if ( LOWER(*astr) != LOWER(*bstr) )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns TRUE is astr not part of bstr.
 *   (compatibility with historical functions).
 */
bool str_infix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;
    int ichar;
    char c0;

    if ( ( c0 = LOWER(astr[0]) ) == '\0' )
	return FALSE;

    sstr1 = strlen(astr);
    sstr2 = strlen(bstr);

    for ( ichar = 0; ichar <= sstr2 - sstr1; ichar++ )
    {
	if ( c0 == LOWER(bstr[ichar]) && !str_prefix( astr, bstr + ichar ) )
	    return FALSE;
    }

    return TRUE;
}



/*
 * Compare strings, case insensitive, for suffix matching.
 * Return TRUE if astr not a suffix of bstr
 *   (compatibility with historical functions).
 */
bool str_suffix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;

    sstr1 = strlen(astr);
    sstr2 = strlen(bstr);
    if ( sstr1 <= sstr2 && !str_cmp( astr, bstr + sstr2 - sstr1 ) )
	return FALSE;
    else
	return TRUE;
}



/*
 * Returns an initial-capped string.
 */
char *capitalize( const char *str )
{
    static char strcap[MAX_STRING_LENGTH];
    int i;

    for ( i = 0; str[i] != '\0'; i++ )
	strcap[i] = LOWER(str[i]);
    strcap[i] = '\0';
    strcap[0] = UPPER(strcap[0]);
    return strcap;
}


/*
 * Append a string to a file.
 */
void append_file( CHAR_DATA *ch, char *file, char *str )
{
    FILE *fp;

    if ( IS_NPC(ch) || str[0] == '\0' )
	return;

    fclose( fpReserve );
    if ( ( fp = fopen( file, "a" ) ) == NULL )
    {
	perror( file );
	send_to_char( "Could not open the file!\n\r", ch );
    }
    else
    {
	fprintf( fp, "[%5ld] %s: %s\n",
	    ch->in_room ? ch->in_room->vnum : 0, ch->original_name, str );
	fclose( fp );
    }

    fpReserve = fopen( NULL_FILE, "r" );
    return;
}



/*
 * Reports a bug.
 */
void bug( const char *str, int param )
{
    char buf[MAX_STRING_LENGTH];

    if ( fpArea != NULL )
    {
	int iLine;
	int iChar;

	if ( fpArea == stdin )
	{
	    iLine = 0;
	}
	else
	{
	    iChar = ftell( fpArea );
	    fseek( fpArea, 0, 0 );
	    for ( iLine = 0; ftell( fpArea ) < iChar; iLine++ )
	    {
		while ( getc( fpArea ) != '\n' )
		    ;
	    }
	    fseek( fpArea, iChar, 0 );
	}

	sprintf( buf, "[*****] FILE: %s LINE: %d", strArea, iLine );
	log_string( buf );
	wiznet(buf,NULL,NULL,WIZ_DEBUG,0,0);
    }

    strcpy( buf, "[*****] BUG: " );
    sprintf( buf + strlen(buf), str, param );
	log_string(buf);
	wiznet(buf,NULL,NULL,WIZ_DEBUG,0,0);
    return;
}

/*
 * Writes a string to the log.
 */
void log_string( const char *str )
{
    char *strtime;

    strtime                    = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
    fprintf( stderr, "%s :: %s\n", strtime, str );
    return;
}



/*
 * This function is here to aid in debugging.
 * If the last expression in a function is another function call,
 *   gcc likes to generate a JMP instead of a CALL.
 * This is called "tail chaining."
 * It hoses the debugger call stack for that call.
 * So I make this the last call in certain critical functions,
 *   where I really need the call stack to be right for debugging!
 *
 * If you don't understand this, then LEAVE IT ALONE.
 * Don't remove any calls to tail_chain anywhere.
 *
 * -- Furey
 */
void tail_chain( void )
{
    return;
}

void do_areset args( ( CHAR_DATA *ch, char *argument ) )
{
	char buf[MAX_STRING_LENGTH];
	char arg[MAX_INPUT_LENGTH];
	int areachoice;
    AREA_DATA *pArea;

	one_argument( argument, arg );

	if (!is_number(arg))
	{
		areachoice = -1;
	}
	else
	{
		areachoice = atoi(arg);
	}

	if (arg[0] == '\0')
	{
		send_to_char("Please choose an area to reset.\n\r",ch);
		send_to_char("Syntax: areset <filename>.\n\r",ch);
		send_to_char("Syntax: areset <vnum>.\n\r",ch);
		return;
	}

    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
		if(((pArea->vnum == areachoice)
		&& ((areachoice != 0)
		|| (areachoice != -1)))
		|| (!str_cmp(arg,pArea->file_name)))
		{
			reset_area( pArea );
			pArea->age = number_range( 0, 3 );

			sprintf(buf, "Area: %s has been reset.\n\r", pArea->name);
			send_to_char(buf, ch);

			sprintf(buf,"Area: %s has just been reset by %s.\n\r",pArea->name,ch->name);
			wiznet(buf,NULL,NULL,WIZ_RESETS,0,0);
		}
	}
	return;
}


void do_force_reset(CHAR_DATA *ch,char *argument)
{
    AREA_DATA *pArea;
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *ich;

    pArea = ch->in_room->area;
    reset_area(pArea);
    pArea->age = number_range(0,3);
    sprintf(buf,"Area: %s reset.\n\r",pArea->name);
    send_to_char(buf,ch);
    sprintf(buf,"Area: %s reset by %s.\n\r",pArea->name,ch->name);

    for (ich = char_list; ich != NULL; ich = ich->next)
    {
	if (!IS_NPC(ich) && ich != ch)
	{
	if (ich->level == 60 || ich->trust == 60)
		send_to_char(buf,ich);
	}
    }

    return;
}



void do_llimit(CHAR_DATA *ch, char *argument)
    {
	FILE *fpChar_list;
	FILE *fpChar;
	char strPlr[MAX_INPUT_LENGTH];
	char catplr[MAX_INPUT_LENGTH];
	char chkbuf[MAX_STRING_LENGTH];
	char pbuf[100];
  	OBJ_INDEX_DATA *lObjIndex;
	OBJ_DATA *obj;
	int i;
	CHAR_DATA *carrier;

/* reset limit counts and recount items in game , do NOT count items on
players loaded */
	send_to_char("Reseting all counts to zero now.\n\r",ch);
	for (i = 0; i < 40000; i++)
	{
	lObjIndex = get_obj_index(i);
	if (lObjIndex == NULL)
		continue;
	lObjIndex->limcount = 0;
	}
	send_to_char("Counts set to zero....\n\r",ch);
	send_to_char("Loading all in-game counts now (excluding PC objects).\n\r",ch);
    	for (obj = object_list; obj != NULL; obj = obj->next)
	{
	carrier = obj->carried_by;
	if (carrier != NULL && !IS_NPC(carrier))
		continue;
	obj->pIndexData->limcount++;
	}
	send_to_char("Loading all pfile object counts now.\n\r",ch);

	sprintf(catplr,"%s%s",PLAYER_DIR,"*.plr");
	sprintf(pbuf,"ls %s > %s",catplr,PLAYER_LIST);
	system(pbuf);

	if ((fpChar_list = fopen( PLAYER_LIST, "r")) == NULL)
	{
	perror(PLAYER_LIST);
	exit(1);
	}

	for (; ;)
	{
	 strcpy(strPlr, fread_word(fpChar_list) );
         log_string(strPlr);
 	 sprintf(chkbuf,"%s%s",PLAYER_DIR,"Zzz.plr");
	 if (!str_cmp(strPlr,"../player/Zzz.plr"))
		break;	/* Exit if == Zzz.plr file */

	 if ( (	fpChar = fopen(strPlr, "r") ) == NULL)
	 {
		perror(strPlr);
		exit(1);
	 }

	for (; ;)
	{
	long vnum;
	char letter;
	char *word;
	OBJ_INDEX_DATA *pObjIndex;

	letter = fread_letter(fpChar);
	if (letter != '#')
		continue;
	word = fread_word(fpChar);

	if (!str_cmp(word,"End"))
		break;

	if ( !str_cmp( word, "O") || !str_cmp( word, "OBJECT"))
		{
		word = fread_word(fpChar);
		if (!str_cmp(word, "Vnum"))
			{
			vnum = fread_number(fpChar);
			if ( (get_obj_index(vnum)) == NULL)
			{
		/*	bug("Bad obj vnum in limits: %d",vnum); */
			}
			else
				{
				pObjIndex = get_obj_index(vnum);
				pObjIndex->limcount++;
				}
			}
		}
	}
	fclose(fpChar);
	fpChar = NULL;
	}

	fclose( fpChar_list);
	send_to_char("All object counts reset.\n\r",ch);
	return;
    }

void do_vlist( CHAR_DATA *ch, char *argument )
{
    char buf    [ MAX_STRING_LENGTH ];
    AREA_DATA *pArea;
    BUFFER *output;
//    int i,x,y;
    long i, x, y;

    output = new_buf();
//    x = 32768;  /* If you have vnums higher than this, make this number bigger */
    x = 65536;
    y = 0;

    sprintf( buf, "[%3s] [%-27s] (%-5s-%5s) [%-10s]\n\r",
       "Num", "Area Name", "lvnum", "uvnum", "Filename" );

    send_to_char(buf,ch);

    for ( i = area_first->vnum; i <= area_last->vnum; i++ )
    {
      for ( pArea = area_first; pArea; pArea = pArea->next )

        if ( pArea->min_vnum < x && pArea->min_vnum > y )
          x = pArea->min_vnum;

      y = x;
//      x = 32768;  /* If you have vnums higher than this, make this number bigger */
      x = 65536;
      for ( pArea = area_first; pArea; pArea = pArea->next )

        if ( y == pArea->min_vnum )
        {
	    sprintf( buf, "[%3ld] %-29.29s (%-5ld-%5ld) %-12.12s\n\r",
	       pArea->vnum,
	       pArea->name,
	       pArea->min_vnum,
	       pArea->max_vnum,
	       pArea->file_name );
	       add_buf(output,buf);
        }
    }

    page_to_char(buf_string(output),ch);
    return;
}

void fread_room( FILE *fp, long vnum )
{
	ROOM_INDEX_DATA *pRoomIndex;
	int iHash;
	int door;
	char *word;
	bool fMatch;

	pRoomIndex			= (ROOM_INDEX_DATA*) alloc_perm( sizeof(*pRoomIndex) );
	pRoomIndex->next		= NULL;
	pRoomIndex->owner		= str_dup("");
	pRoomIndex->people		= NULL;
	pRoomIndex->contents		= NULL;
	pRoomIndex->extra_descr		= NULL;
	pRoomIndex->area		= area_last;
	pRoomIndex->vnum		= vnum;
	pRoomIndex->cabal		= 0;
	pRoomIndex->light		= 0;
	pRoomIndex->move_progs          = FALSE;

	for ( door=0; door < MAX_DIR; door++ )
	  	pRoomIndex->exit[door]	=	NULL;

	/* defaults */
	pRoomIndex->heal_rate = 100;
	pRoomIndex->mana_rate = 100;

	for ( ; ; )
	{
		word   = feof( fp ) ? "End" : fread_word( fp );
		fMatch = FALSE;

		switch ( UPPER(word[0]) )
		{
			case '*': fread_to_eol( fp ); break;
		  	case 'E':
				if ( !str_cmp( word, "End" ) )
				{
			  		iHash			= vnum % MAX_KEY_HASH;
			  		pRoomIndex->next	= room_index_hash[iHash];
			  		room_index_hash[iHash]	= pRoomIndex;
			  		top_room++;
			  		top_vnum_room 		= top_vnum_room < vnum ? vnum : top_vnum_room;
			  		assign_area_vnum( vnum );
			  		return;
				}

				if ( !str_cmp( word, "EDESC" ) )
				{
			  		EXTRA_DESCR_DATA *ed;

			  		ed			= (EXTRA_DESCR_DATA *) alloc_perm( sizeof(*ed) );
			  		ed->keyword             = fread_string( fp );
			  		ed->description         = fread_string( fp );
			  		ed->next                = pRoomIndex->extra_descr;
			  		pRoomIndex->extra_descr	= ed;
			  		top_ed++;
			  		fMatch = TRUE;
				}
				break;

                        case 'C':
                                if ( !str_cmp( word, "CABAL" ) )
                                {
                                        pRoomIndex->cabal = cabal_lookup(fread_word(fp));
                                        fMatch = TRUE;
                                }
                                break;

		  	case 'D':
				SKEY( "DESCR", pRoomIndex->description );

				if ( !str_cmp( word, "DOOR" ) )
				{
					EXIT_DATA *pexit;

                			door = fread_number (fp );
                			if ( door < 0 || door > 5 )
                			{
                        			bug("Fread_rooms: vnum %d has bad door number.", vnum);
                        			exit(1);
                			}

                			pexit                	= alloc_perm( sizeof(*pexit) );
                			pexit->description   	= fread_string( fp );
                			pexit->keyword       	= fread_string( fp );
                			pexit->exit_info     	= 0;
                			pexit->rs_flags      	= fread_flag( fp );
                			pexit->orig_door     	= door;
                			pexit->key           	= fread_number( fp );
                			pexit->u1.vnum       	= fread_number( fp );
                			pRoomIndex->exit[door]	= pexit;
                			top_exit++;
					fMatch = TRUE;
				}
				break;
		  	case 'F':
				if ( !str_cmp( word, "FLAGS" ) )
				{
			  		pRoomIndex->room_flags	= fread_flag( fp );
			  		fMatch = TRUE;
				}
				break;
		  	case 'M':
				if ( !str_cmp( word, "MHRATE" ) )
				{
			  		pRoomIndex->mana_rate = fread_number(fp);
			  		pRoomIndex->heal_rate = fread_number(fp);
			  		fMatch = TRUE;
				}
				break;
		  	case 'N':
				SKEY( "NAME", pRoomIndex->name );
				break;

		  	case 'S':
				KEY( "SECT", pRoomIndex->sector_type, fread_number( fp ) );
				break;
		}
		if ( !fMatch )
		{
		  	char buf[80];
		  	sprintf( buf, "fread_rooms: incorrect word %s on vnum %ld", word, vnum );
		  	bug( buf, 0 );
		  	fread_to_eol( fp );
		}
	}
	return;
}

void fread_mobile( FILE *fp, long vnum )
{
	MOB_INDEX_DATA *pMobIndex;
	char      *word;
	bool      fMatch;
	int 	iHash;

	pMobIndex			= (MOB_INDEX_DATA*) alloc_perm( sizeof(*pMobIndex) );
	pMobIndex->vnum			= vnum;
	pMobIndex->area			= area_last;
	pMobIndex->new_format		= TRUE;
	pMobIndex->cabal		= 0;
        pMobIndex->mprogs		= NULL;
        pMobIndex->progtypes		= 0;
        pMobIndex->dam_mod		= 100;
	pMobIndex->enhancedDamMod	= 100;
	pMobIndex->regen_rate = 0;
	newmobs++;

	for ( ; ; )
	{
		word   = feof(fp) ? "End" : fread_word(fp);
		fMatch = FALSE;

		switch ( UPPER(word[0]) )
		{
		  	case '*': fread_to_eol(fp); 
				break;
		  	case 'E':
				if ( !str_cmp( word, "End" ) )
				{
			  		iHash			= vnum % MAX_KEY_HASH;
			  		pMobIndex->next		= mob_index_hash[iHash];
			  		mob_index_hash[iHash]	= pMobIndex;
			  		top_mob_index++;
			  		top_vnum_mob 		= top_vnum_mob < vnum ? vnum : top_vnum_mob; 
			  		assign_area_vnum( vnum ); 
			  		kill_table[URANGE(0, pMobIndex->level, MAX_LEVEL-1)].number++;
			  		return;
				}
				if ( !str_cmp( word, "ENHA" ))
				{
        				pMobIndex->enhancedDamMod       = fread_number( fp );
					fread_letter( fp );
					fread_number( fp );
					fread_letter( fp );
					fMatch = TRUE;
				}
				break;

		  	case 'A':
				KEY( "AMOD",			pMobIndex->numAttacks,		fread_number(fp));
				if ( !str_cmp( word, "ACT") )
				{
					pMobIndex->act = fread_flag( fp ) | ACT_IS_NPC | race_table[pMobIndex->race].act;
					fMatch = TRUE;
				}
				if ( !str_cmp( word, "AC" ) )
				{
			  		pMobIndex->ac[AC_PIERCE]	= fread_number(fp) * 10;
			  		pMobIndex->ac[AC_BASH]		= fread_number(fp) * 10;
			  		pMobIndex->ac[AC_SLASH]		= fread_number(fp) * 10;
			  		pMobIndex->ac[AC_EXOTIC]	= fread_number(fp) * 10;
			  		fMatch = TRUE;
				}
				KEY( "AFF",			pMobIndex->affected_by,		fread_flag(fp) );
				KEY( "ALIGN",			pMobIndex->alignment,		fread_number(fp) );
				break;

		  	case 'C':
				if ( !str_cmp( word, "CABAL" ) )
				{
					pMobIndex->cabal 	= cabal_lookup(fread_word(fp));
					fMatch = TRUE;
				}
				break;

		  	case 'D':
				KEY( "DMOD",	pMobIndex->dam_mod,		fread_number(fp) );
				SKEY( "DESCR",	pMobIndex->description );
				if ( !str_cmp( word, "DDice" ) )
				{
			  		pMobIndex->damage[DICE_NUMBER]	= fread_number(fp);
			  						  fread_letter(fp);
			  		pMobIndex->damage[DICE_TYPE]	= fread_number(fp);
			  						  fread_letter(fp); 
			  		pMobIndex->damage[DICE_BONUS]	= fread_number(fp);
			  		fMatch = TRUE;
				}
				KEY( "DTYPE",	pMobIndex->dam_type,	attack_lookup(fread_word(fp)) );
				break;

		  	case 'F':
				KEY( "FORM",	pMobIndex->form,	fread_flag(fp) );
				break;

		  	case 'G':
				KEY( "GOLD",	pMobIndex->wealth,	fread_number(fp) );
				KEY( "GROUP",	pMobIndex->group,	fread_number(fp) );
				break;

		  	case 'H':
				KEY( "HROLL",	pMobIndex->hitroll,	fread_number(fp) );
				if ( !str_cmp( word, "HDICE" ) )
				{
			  		pMobIndex->hit[DICE_NUMBER]	= fread_number(fp);
			  						  fread_letter(fp);
			  		pMobIndex->hit[DICE_TYPE]	= fread_number(fp);
			  						  fread_letter(fp);
			  		pMobIndex->hit[DICE_BONUS]	= fread_number(fp);
			  		fMatch = TRUE;
				}
				break;

		  	case 'I':
				KEY( "IMM",	pMobIndex->imm_flags,	fread_flag(fp) );
				break;

		  	case 'L':
				SKEY( "LONG",	pMobIndex->long_descr );
				KEY( "LEVEL",	pMobIndex->level, fread_number(fp) );
				break;

		  	case 'M':
				if ( !str_cmp( word, "MDICE" ) )
				{
			  		pMobIndex->mana[DICE_NUMBER]	= fread_number(fp);
			 						  fread_letter(fp);
			  		pMobIndex->mana[DICE_TYPE]	= fread_number(fp);
									  fread_letter(fp);
			  		pMobIndex->mana[DICE_BONUS]	= fread_number(fp);
			  		fMatch = TRUE;
				}
				if (!str_cmp( word, "MATER" ) ) {
					pMobIndex->material = fread_string(fp);
					fMatch = TRUE;
				}
				break;

		  	case 'N':
				SKEY( "NAME",			pMobIndex->player_name );
				break;

		  	case 'O':
				KEY( "OFF",			pMobIndex->off_flags,	fread_flag(fp) );
				break;

		  	case 'P':
				KEY( "PARTS",			pMobIndex->parts,	fread_flag(fp) );
				if ( !str_cmp( word, "POS" ) )
				{
			  		pMobIndex->start_pos	= position_lookup(fread_word(fp));
			  		pMobIndex->default_pos	= position_lookup(fread_word(fp));
			  		fMatch = TRUE;
				}
				break;

		  	case 'R':
				KEY( "RACE",			pMobIndex->race,	race_lookup(fread_string(fp)));
				KEY( "RES",			pMobIndex->res_flags,	fread_flag(fp) );
				KEY( "REGEN",		pMobIndex->regen_rate,	fread_number(fp) );
				break;

		  	case 'S':
				KEY( "SEX",			pMobIndex->sex,		sex_lookup(fread_word(fp)));
				SKEY( "SHORT",			pMobIndex->short_descr );
				if ( !str_cmp( word, "SIZE" ) )
				{
			  		CHECK_POS( pMobIndex->size, size_lookup(fread_word(fp)), "size" );
			  		fMatch = TRUE;
				}
				break;

		  	case 'V':
				KEY( "VULN",			pMobIndex->vuln_flags,	fread_flag(fp) );
				break;

		  	case 'Q':
				KEY( "QUEST",			pMobIndex->quest_credit_reward,	fread_number(fp) );
				break;
			case 'W':
				KEY( "WSPEC",			pMobIndex->extended_flags,	fread_flag(fp) );
				break;
		}
		if (!fMatch)
		{
		  	char buf[80];
		  	sprintf( buf, "fread_mobile: %ld incorrect: %s", vnum, word );
		  	bug(buf, 0);
		  	fread_to_eol(fp);
		}
	}
	return;
}

//Reads objects from .are file
void fread_object( FILE *fp, long vnum )
{
	OBJ_INDEX_DATA *pObjIndex;
	int iHash;
	char      *word;
	bool      fMatch;

	pObjIndex		= (OBJ_INDEX_DATA*) alloc_perm( sizeof(*pObjIndex) );
	pObjIndex->vnum		= vnum;
	pObjIndex->area		= area_last;
	pObjIndex->new_format	= TRUE;
	pObjIndex->reset_num	= 0;
        pObjIndex->limcount	= 0;
        pObjIndex->limtotal	= 0;
        pObjIndex->iprogs	= NULL;
        pObjIndex->progtypes	= 0;

	newobjs++;

	for ( ; ; )
	{
		word   = feof( fp ) ? "End" : fread_word(fp);
		fMatch = FALSE;

		switch ( UPPER(word[0]) )
		{
		  	case '*': fread_to_eol(fp); break;
		  	
			case 'E':
				if ( !str_cmp( word, "End" ) )
				{
			  		//Armor normalization procedure --Zornath
					if (pObjIndex->item_type == ITEM_ARMOR) {
						float recommend_ac_cap = 0.0;
						
						//How strong each armor wear_loc is, logically, compared to others
						if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_FINGER)) { recommend_ac_cap = 0.667; }
						if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_NECK)) { recommend_ac_cap = 0.8; }
						if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_BODY)) { recommend_ac_cap = 1.466; }
						if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_HEAD)) { recommend_ac_cap = 1.067; }
						if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_LEGS)) { recommend_ac_cap = 0.933; }
						if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_FEET)) { recommend_ac_cap = 0.933; }
						if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_HANDS)) { recommend_ac_cap = 0.933; }
						if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_ARMS)) { recommend_ac_cap = 1.067; }
						if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_SHIELD)) { recommend_ac_cap = 2.50; }
						if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_ABOUT)) { recommend_ac_cap = 1.067; }
						if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_WAIST)) { recommend_ac_cap = 0.8; }
						if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_WRIST)) { recommend_ac_cap = 0.8; }
						if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_STRAPPED)) { recommend_ac_cap = 0.5; }
						
						float relative_prot_pierce = (float)pObjIndex->material->relative_protection_pierce / (float)100;
						float relative_prot_bash = (float)pObjIndex->material->relative_protection_bash / (float)100;
						float relative_prot_slash = (float)pObjIndex->material->relative_protection_slash / (float)100;
						float relative_prot_magic = (float)pObjIndex->material->relative_protection_magic / (float)100;
						
						//Armor function is quadratic with respect to the armor level
						//y = m(x^2)    
						//Solve for m based on game constants
						float slope_pierce = (recommend_ac_cap * (AC_PER_ONE_PERCENT_DECREASE_DAMAGE * relative_prot_pierce))/(1.5 * LEVEL_HERO * LEVEL_HERO);
						float slope_bash = (recommend_ac_cap * (AC_PER_ONE_PERCENT_DECREASE_DAMAGE * relative_prot_bash))/(1.5 * LEVEL_HERO * LEVEL_HERO);
						float slope_slash = (recommend_ac_cap * (AC_PER_ONE_PERCENT_DECREASE_DAMAGE * relative_prot_slash))/(1.5 * LEVEL_HERO * LEVEL_HERO);
						float slope_magic = (recommend_ac_cap * (AC_PER_ONE_PERCENT_DECREASE_DAMAGE * relative_prot_magic))/(1.5 * LEVEL_HERO * LEVEL_HERO);
						
						//Evaluate y at x = armor level and m = slope above
						pObjIndex->value[0] = (int)(-1 * slope_pierce * (pObjIndex->level * pObjIndex->level));
						pObjIndex->value[1] = (int)(-1 * slope_bash * (pObjIndex->level * pObjIndex->level));
						pObjIndex->value[2] = (int)(-1 * slope_slash * (pObjIndex->level * pObjIndex->level));
						pObjIndex->value[3] = (int)(-1 * slope_magic * (pObjIndex->level * pObjIndex->level));
					}
					float recommend_weight = 0.3;
					//How heavy each slot is with respect to others (assuming same material)
					if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_FINGER)) { recommend_weight = 0.15; }
					if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_NECK)) { recommend_weight = 0.4; }
					if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_BODY)) { recommend_weight = 1; }
					if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_HEAD)) { recommend_weight = 0.8; }
					if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_LEGS)) { recommend_weight = 0.85; }
					if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_FEET)) { recommend_weight = 0.7; }
					if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_HANDS)) { recommend_weight = 0.6; }
					if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_ARMS)) { recommend_weight = 0.8; }
					if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_SHIELD)) { recommend_weight = 1.2; }
					if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_ABOUT)) { recommend_weight = 0.8; }
					if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_WAIST)) { recommend_weight = 0.4; }
					if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_WRIST)) { recommend_weight = 0.5; }
					if (IS_SET(pObjIndex->wear_flags,ITEM_WEAR_STRAPPED)) { recommend_weight = 0.7; }
					
					//Weights for weapons
					if (pObjIndex->item_type == ITEM_WEAPON) {
						switch (pObjIndex->value[0]) {
							case WEAPON_EXOTIC: recommend_weight = 0.6; break;
							case WEAPON_SWORD: recommend_weight = 0.5; break;
							case WEAPON_DAGGER: recommend_weight = 0.2; break;
							case WEAPON_SPEAR: recommend_weight = 0.65; break;
							case WEAPON_MACE: recommend_weight = 0.7; break;
							case WEAPON_AXE: recommend_weight = 0.7; break;
							case WEAPON_FLAIL: recommend_weight = 0.6; break;
							case WEAPON_WHIP: recommend_weight = 0.4; break;
							case WEAPON_POLEARM: recommend_weight = 0.8; break;
							case WEAPON_STAFF: recommend_weight = 0.65; break;
							case WEAPON_HAND: recommend_weight = 0.3; break;
						}
						if (IS_SET(pObjIndex->value[4],WEAPON_TWO_HANDS)) recommend_weight *= 1.5;
					}
					
					if (pObjIndex->item_type == ITEM_BOAT) recommend_weight *= 4;
					
					//Material weight ratio with respect to the weight standard(i.e. relative weight 120 means weight will be 1.2 * WEIGHT_STANDARD * recommend factor)
					float material_weight = (float)pObjIndex->material->relative_weight / (float)100;
					pObjIndex->weight = (int)(recommend_weight * (material_weight * WEIGHT_STANDARD));
					
					iHash			= vnum % MAX_KEY_HASH;
			  		pObjIndex->next		= obj_index_hash[iHash];
			  		obj_index_hash[iHash]	= pObjIndex;
			  		top_obj_index++;
			  		top_vnum_obj 	= top_vnum_obj < vnum ? vnum : top_vnum_obj;
			  		assign_area_vnum( vnum );
			  		return;
				}
				//KEY( "Extra", pObjIndex->extra_flags, fread_flag(fp) );
				if (!str_cmp(word, "Extra"))
				{
					//load_old_bits(&pObjIndex->extra_flags, fp);
					load_bitmask(&pObjIndex->extra_flags, fp);
					fMatch = TRUE;
				}
				if ( !str_cmp( word, "EDESC" ) )
				{
			  		EXTRA_DESCR_DATA *ed;

			  		ed			= (EXTRA_DESCR_DATA *) alloc_perm( sizeof(*ed) );
			  		ed->keyword		= fread_string(fp);
			  		ed->description		= fread_string(fp);
			  		ed->next		= pObjIndex->extra_descr;
			  		pObjIndex->extra_descr	= ed;
			  		top_ed++;
			  		fMatch = TRUE;
				}
				break;

		  	case 'A':
				if ( !str_cmp( word, "AFFECT" ) )
				{
					AFFECT_DATA *paf;
					char letter = fread_letter(fp);

					paf = (AFFECT_DATA*)  alloc_perm( sizeof(*paf) );

					switch (letter)
					{
				  		case 'A': paf->where = TO_AFFECTS;		break;
				  		case 'I': paf->where = TO_IMMUNE;		break;
				  		case 'R': paf->where = TO_RESIST;		break;
				  		case 'V': paf->where = TO_VULN;			break;
				  		case 'O': paf->where = TO_OBJECT;		break;
				  		default:
							bug( "Load_objects: Bad where on flag set.", 0 );
							exit( 1 );
					}

					paf->type		= -1;
					paf->level		= pObjIndex->level;
					paf->duration		= -1;
					paf->location		= fread_number(fp);
					paf->modifier		= fread_number(fp);
				
					if (paf->where == TO_OBJECT)
				  		paf->bitvector	= fread_number(fp);
					else
				  		paf->bitvector	= fread_flag(fp);
				
					paf->next		= pObjIndex->affected;
					pObjIndex->affected	= paf;
					top_affect++;
					fMatch = TRUE;
				}
				break;

		  	case 'C':
				KEY( "CABAL",	pObjIndex->cabal,	fread_number(fp) );
				KEY( "COST",	pObjIndex->cost,	fread_number(fp) );
				KEY( "COND",	pObjIndex->condition,	fread_number(fp) );
				break;

		  	case 'D':
				SKEY( "DESCR",	pObjIndex->description );
				break;

		  	case 'L':
				KEY( "LEVEL",	pObjIndex->level,		fread_number(fp) );
				KEY( "LIMIT",	pObjIndex->limtotal,	fread_number(fp) );
				break;

		  	case 'M':
				if ( !str_cmp( word, "MAT" ) )
				{
			  		char bugbuf[MSL];
					pObjIndex->material = find_material_by_name(fread_string(fp)) ;
					if (pObjIndex->material == find_material_by_name("none")) {
						sprintf(bugbuf,"%ld :: material is set to none",pObjIndex->vnum);
						bug(bugbuf,0);
					}
						
			  		fMatch = TRUE;
				}
				break;

		  	case 'N':
				SKEY( "NAME",	pObjIndex->name  );
				break;

		  	case 'R':
				KEY( "RESTR",		pObjIndex->restrict_flags,	fread_flag(fp) );
				break;

		  	case 'S':
				SKEY( "SHORT",	pObjIndex->short_descr );
				break;

		  	case 'T':
				if ( !str_cmp( word, "TYPE" ) )
				{
					CHECK_POS(pObjIndex->item_type, item_lookup(fread_word(fp)), "item_type" );

					switch(pObjIndex->item_type)
					{
				  		case ITEM_WEAPON:
							pObjIndex->value[0] = weapon_type(fread_word(fp));
							pObjIndex->value[1]	= fread_number(fp);
							pObjIndex->value[2]	= fread_number(fp);
							pObjIndex->value[3]	= attack_lookup(fread_word(fp));
							pObjIndex->value[4]	= fread_flag(fp);
							break;
				  		case ITEM_CONTAINER:
							pObjIndex->value[0]	= fread_number(fp);
							pObjIndex->value[1]	= fread_flag(fp);
							pObjIndex->value[2]	= fread_number(fp);
							pObjIndex->value[3]	= fread_number(fp);
							pObjIndex->value[4]	= fread_number(fp);
							break;
				  		case ITEM_DRINK_CON:
				  		case ITEM_FOUNTAIN:
							pObjIndex->value[0]	= fread_number(fp);
							pObjIndex->value[1]	= fread_number(fp);
							CHECK_POS(pObjIndex->value[2], liq_lookup(fread_word(fp)), "liq_lookup" );
							pObjIndex->value[3]	= fread_number(fp);
							pObjIndex->value[4]	= fread_number(fp);
							break;
				  		case ITEM_WAND:
				  		case ITEM_STAFF:
							pObjIndex->value[0]	= fread_number(fp);
							pObjIndex->value[1]	= fread_number(fp);
							pObjIndex->value[2]	= fread_number(fp);
							pObjIndex->value[3]	= skill_lookup(fread_word(fp));
							pObjIndex->value[4]	= fread_number(fp);
							break;
				  		case ITEM_POTION:
				  		case ITEM_PILL:
				  		case ITEM_SCROLL:
							pObjIndex->value[0]	= fread_number(fp);
							pObjIndex->value[1]	= skill_lookup(fread_word(fp));
							pObjIndex->value[2]	= skill_lookup(fread_word(fp));
							pObjIndex->value[3]	= skill_lookup(fread_word(fp));
							pObjIndex->value[4]	= skill_lookup(fread_word(fp));
							break;
				  		default:
							pObjIndex->value[0]	= fread_flag(fp);
							pObjIndex->value[1]	= fread_flag(fp);
							pObjIndex->value[2]	= fread_flag(fp);
							pObjIndex->value[3]	= fread_flag(fp);
							pObjIndex->value[4]	= fread_flag(fp);
							break;
					}
					fMatch = TRUE;
				}
				break;

		  	case 'W':
				KEY( "WEAR",	pObjIndex->wear_flags,	fread_flag( fp ) );
				KEY( "WEIGHT",	pObjIndex->weight,		fread_number( fp ) );
				break;
		}

		if ( !fMatch )
		{
		  	sprintf( log_buf, "fread_object: vnum %ld incorrect:  '%s'", vnum, word );
		  	bug( log_buf, 0 );
		  	fread_to_eol( fp );
		}
	}
	return;
}

void load_rooms_new( FILE *fp )
{
	if ( area_last == NULL )
	{
	  bug( "Load_rooms: no #AREA seen yet.", 0 );
	  exit( 1 );
	}

	for ( ; ; )
	{
		long vnum;
		char letter;

		letter = fread_letter( fp );
		if ( letter != '#' )
		{
		  bug( "Load_rooms: # not found.", 0 );
		  exit( 1 );
		}
		vnum = fread_number( fp );
		if ( vnum == 0 )
		  break;

		fBootDb = FALSE;
		if ( get_room_index( vnum ) != NULL )
		{
		  bug( "Load_rooms: vnum %ld duplicated.", vnum );
		  exit( 1 );
		}
		fBootDb = TRUE;

		fread_room( fp, vnum );
	}
	return;
}

void load_objects_new( FILE *fp )
{
	if ( !area_last )
	{
	  bug( "Load_objects: no #AREA seen yet.", 0 );
	  exit( 1 );
	}

	for ( ; ; )
	{
		long vnum;
		char letter;

		letter = fread_letter( fp );
		if ( letter != '#' )
		{
		  bug( "Load_objects: # not found.", 0 );
		  exit( 1 );
		}

		vnum = fread_number( fp );
		if ( vnum == 0 )
		break;

		fBootDb = FALSE;
		if ( get_obj_index( vnum ) != NULL )
		{
		  bug( "Load_objects: vnum %ld duplicated.", vnum );
		  exit( 1 );
		}
		fBootDb = TRUE;

		fread_object( fp, vnum );
	}
	return;
}

void load_mobiles_new( FILE *fp )
{
	if ( area_last == NULL )
	{
	  bug( "Load_mobiles: no #AREA seen yet.", 0 );
	  exit( 1 );
	}

	for ( ; ; )
	{
		long vnum;
		char letter;

		letter = fread_letter( fp );
		if ( letter != '#' )
		{
		  bug( "Load_mobiles: '#' not found.", 0 );
		  exit( 1 );
		}

		vnum = fread_number( fp );
		if ( vnum == 0 )
		{
		  /* bug( "found vnum zero", 0 ); */
		  break;
		}
		fBootDb = FALSE;
		if ( get_mob_index( vnum ) != NULL )
		{
		  bug( "Load_mobiles: vnum %ld duplicated.", vnum );
		  exit( 1 );
		}
		fBootDb = TRUE;

		fread_mobile( fp, vnum );
	}
	return;
}

void logItemFix(void)
{
	FILE *fp;
	OBJ_INDEX_DATA *obj;
	int i=0;

	fp = fopen(ITEM_FIX_FILE, "w");
	
	if (!fp)
		return;

	for ( obj = obj_index_hash[0]; i < MAX_KEY_HASH; obj = obj_index_hash[i], i++ )
	{
		if(obj == NULL || obj == 0)
			continue;

		if(obj->level <= 0)
			fprintf(fp, "%ld *** Item level is zero or negative.\n", obj->vnum);

		if (obj->weight <=0)
			fprintf(fp, "%ld *** Item has no weight.\n", obj->vnum);

		if (!obj->material)
			fprintf(fp, "%ld *** NULL material set!\n", obj->vnum);
		
		if (obj->material)
			if (!str_cmp(obj->material->name,"none"))
				fprintf(fp, "%ld *** No material set.\n", obj->vnum);
	}
    	fclose(fp);
	return;
}

void printRaceInfo()
{
	FILE *fp;
	int r=0, c=0;
	char *align;

	fp = fopen("raceinfo.txt", "w");

	if (fp == NULL)
	{
		n_logf("Cant open race info file!");
		return;
	}

	for (r=1;r<MAX_PC_RACE;r++) {
		fprintf(fp, "%s %s\n--------------------\n", pc_race_table[r].name, pc_race_table[r].set_race ? "[SET RACE]" : "");
		
		switch(pc_race_table[r].align)
		{
			case ALIGN_NONE: align = "none"; break;
			case ALIGN_N: align = "neutral"; break;
			case ALIGN_GN: align = "good/neutral"; break;
			case ALIGN_E: align = "evil"; break;
			case ALIGN_GE: align = "good/evil"; break;
			case ALIGN_NE: align = "neutral/evil"; break;
			case ALIGN_ANY: align = "any"; break;
			case ALIGN_G: align = "good"; break;
			default: align = "error"; 
		}
		fprintf(fp, "Align\t\t%s\n", align);
		fprintf(fp, "Affect\t\t%s\n", flag_string(affect_flags, race_table[r].aff));
		fprintf(fp, "Imm\t\t%s\n", flag_string(imm_flags, race_table[r].imm));
		fprintf(fp, "Res\t\t%s\n", flag_string(res_flags, race_table[r].res));
		fprintf(fp, "Vuln\t\t%s\n", flag_string(vuln_flags, race_table[r].vuln));
		fprintf(fp, "Classes\t\t");
		for (c=0;c<MAX_CLASS;c++)
		{
			if (pc_race_table[r].classes[c] == 1)
			{
				fprintf(fp, "%s ", class_table[c].name);
			}
		}
		fprintf(fp, "\n");
		fprintf(fp, "Size:\t\t%s\n", size_table[pc_race_table[r].size].name);
		fprintf(fp, "Str: %d Int: %d Wis: %d Dex: %d Con: %d\n", pc_race_table[r].max_stats[STAT_STR],
			pc_race_table[r].max_stats[STAT_INT], pc_race_table[r].max_stats[STAT_WIS],
			pc_race_table[r].max_stats[STAT_DEX], pc_race_table[r].max_stats[STAT_CON]);
		fprintf(fp, "\n\n\n");
	}
	fclose(fp);
	return;
}

void printCabalInfo()
{
	FILE *fp;
	int i=0, c=0, r=0, sn=0, gn=0;
	char *abuf, *ebuf;

        if ( ( fp = fopen( "cinfo.txt", "w" ) ) == NULL )
                return perror( "cinfo.txt" );

	for (i=1; i<MAX_CABAL; i++)
	{
		fprintf(fp, "%s------------------------\n", cabal_table[i].name);
		fprintf(fp, "Long Name: %s\n", cabal_table[i].long_name);
		fprintf(fp, "Races Allowed: ");
		for (r=0;r<MAX_PC_RACE;r++)
		{
			if (cabal_restr_table[i].acc_race[r])
				fprintf(fp, "%s ", pc_race_table[r].name);
		}
		fprintf(fp, "\n");
		fprintf(fp, "Classes Allowed: ");
		for (c=0;c<MAX_CLASS;c++)
		{
			if (cabal_restr_table[i].acc_class[c])
				fprintf(fp, "%s ", class_table[c].name);
		}
		fprintf(fp, "\n");
		switch (cabal_restr_table[i].acc_align)
		{
			case ALIGN_NONE: abuf = "none"; break;
			case ALIGN_G: abuf = "good"; break;
			case ALIGN_N: abuf = "neutral"; break;
			case ALIGN_GN: abuf = "good/neutral"; break;
			case ALIGN_E: abuf = "evil"; break;
			case ALIGN_GE: abuf = "good/evil"; break;
			case ALIGN_NE: abuf = "neutral/evil"; break;
			case ALIGN_ANY: abuf = "any"; break;
			default: abuf = "error";
		}
		fprintf(fp, "Align: %s\n", abuf);
		switch (cabal_restr_table[i].acc_ethos)
		{
			case ETHOS_NONE: ebuf = "none"; break;
			case ETHOS_L: ebuf = "lawful"; break;
			case ETHOS_N: ebuf = "neutral"; break;
			case ETHOS_LN: ebuf = "lawful/neutral"; break;
			case ETHOS_C: ebuf = "chaotic"; break;
			case ETHOS_LC: ebuf = "lawful/chaotic"; break;
			case ETHOS_NC: ebuf = "neutral/chaotic"; break;
			case ETHOS_ANY: ebuf = "any"; break;
			default: ebuf = "error";
		}
		fprintf(fp, "Ethos: %s\n", ebuf);
		fprintf(fp, "Powers: ");
		gn = group_lookup(cabal_table[i].name);
		for (sn=0;sn<MAX_IN_GROUP;sn++)
		{
			if (group_table[gn].spells[sn] != NULL)
				fprintf(fp, "%s ", group_table[gn].spells[sn]);
		}
		fprintf(fp, "\n\n\n");
			
	}
	fclose(fp);
	return;
}
