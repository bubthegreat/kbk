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

/* item type list */
const struct item_type		item_table	[]	=
{
    {	ITEM_LIGHT,	"light"		},
    {	ITEM_SCROLL,	"scroll"	},
    {	ITEM_WAND,	"wand"		},
    {   ITEM_STAFF,	"staff"		},
    {   ITEM_WEAPON,	"weapon"	},
    {   ITEM_NULL6,     "null6"         },
    {   ITEM_NULL7,     "null7"         },
    {   ITEM_TREASURE,	"treasure"	},
    {   ITEM_ARMOR,	"armor"		},
    {	ITEM_POTION,	"potion"	},
    {	ITEM_CLOTHING,	"clothing"	},
    {   ITEM_FURNITURE,	"furniture"	},
    {	ITEM_TRASH,	"trash"		},
    {	ITEM_CONTAINER,	"container"	},
    {	ITEM_DRINK_CON, "drink"		},
    {	ITEM_KEY,	"key"		},
    {	ITEM_FOOD,	"food"		},
    {	ITEM_MONEY,	"money"		},
    {	ITEM_BOAT,	"boat"		},
    {	ITEM_CORPSE_NPC,"npc_corpse"	},
    {	ITEM_CORPSE_PC,	"pc_corpse"	},
    {   ITEM_FOUNTAIN,	"fountain"	},
    {	ITEM_PILL,	"pill"		},
    {	ITEM_PROTECT,	"protect"	},
    {	ITEM_MAP,	"map"		},
    {	ITEM_PORTAL,	"portal"	},
    {	ITEM_WARP_STONE,"warp_stone"	},
    {	ITEM_ROOM_KEY,	"room_key"	},
    {	ITEM_GEM,	"gem"		},
    {	ITEM_JEWELRY,	"jewelry"	},
    {   ITEM_JUKEBOX,	"jukebox"	},
    {   ITEM_SKELETON,	"skeleton"      },
    {	ITEM_INSTRUMENT,"instrument"	},
    {   0,		NULL		}
};


/* weapon selection table */
const	struct	weapon_type	weapon_table	[]	=
{
   { "sword",		OBJ_VNUM_SCHOOL_SWORD,		WEAPON_SWORD,	&gsn_sword	},
   { "mace",		OBJ_VNUM_SCHOOL_MACE,		WEAPON_MACE,	&gsn_mace 	},
   { "dagger",		OBJ_VNUM_SCHOOL_DAGGER,		WEAPON_DAGGER,	&gsn_dagger	},
   { "axe",		OBJ_VNUM_SCHOOL_AXE,		WEAPON_AXE,	&gsn_axe	},
   { "spear",		OBJ_VNUM_SCHOOL_STAFF,		WEAPON_SPEAR,	&gsn_spear	},
   { "flail",		OBJ_VNUM_SCHOOL_FLAIL,		WEAPON_FLAIL,	&gsn_flail	},
   { "whip",		OBJ_VNUM_SCHOOL_WHIP,		WEAPON_WHIP,	&gsn_whip	},
   { "polearm",		OBJ_VNUM_SCHOOL_POLEARM,	WEAPON_POLEARM,	&gsn_polearm	},
   { "staff",   	OBJ_VNUM_SCHOOL_STAFF,		WEAPON_STAFF,   &gsn_staff	},
   { "hand to hand", 	OBJ_VNUM_SCHOOL_SWORD, 		WEAPON_HAND, 	&gsn_sword	},
   { NULL,		0,				0,		NULL		}
};



/* wiznet table and prototype for future flag setting */
const   struct wiznet_type      wiznet_table    []              =
{
   {    "on",           	WIZ_ON,         	52 },
   {    "cabal",        	WIZ_CABAL,      	53 },
   {    "deaths",       	WIZ_DEATHS,     	53 },
   {	"levels",		WIZ_LEVELS,		53 },
   {	"newbies",		WIZ_NEWBIE,		55 },
   {    "prefix",		WIZ_PREFIX,		52 },
   {    "ticks",        	WIZ_TICKS,      	52 },
   {    "logins",       	WIZ_LOGINS,     	58 },
   {    "links",        	WIZ_LINKS,      	60 },
   {    "flags",		WIZ_FLAGS,		58 },
   {	"penalties",		WIZ_PENALTIES,		60 },
   {	"saccing",		WIZ_SACCING,		54 },
   {	"spam",			WIZ_SPAM,		56 },
   {    "mobdeaths",    	WIZ_MOBDEATHS,  	54 },
   {    "resets",       	WIZ_RESETS,     	54 },
   {    "sites",        	WIZ_SITES,      	60 },
   {	"load",			WIZ_LOAD,		60 },
   {	"restore",		WIZ_RESTORE,		57 },
   {	"snoops",		WIZ_SNOOPS,		60 },
   {	"switches",		WIZ_SWITCHES,		59 },
   {	"secure",		WIZ_SECURE,		60 },
   {	"looting",		WIZ_LOOTING,		55 },
   {    "debug",		WIZ_DEBUG,		60 },
   {	"mysql",		WIZ_MYSQL,		60 },
   {	"percent",		WIZ_PERCENT,		52 },
   {	NULL,			0,			0  }
};

/* attack table  -- not very organized :( */
const 	struct attack_type	attack_table	[MAX_DAMAGE_MESSAGE]	=
{
    { 	"none",			"hit",			-1,		0	},  /*  0 */
    {	"slice",		"slice", 		DAM_SLASH,	0	},
    {   "stab",			"stab",			DAM_PIERCE,	0	},
    {	"slash",		"slash",		DAM_SLASH,	0	},
    {	"whip",			"whip",			DAM_SLASH,	0	},
    {   "claw",			"claw",			DAM_SLASH,	0	},  /*  5 */
    {	"blast",		"blast",		DAM_BASH,	0	},
    {   "pound",		"pound",		DAM_BASH,	0	},
    {	"crush",		"crush",		DAM_BASH,	0	},
    {   "grep",			"grep",			DAM_SLASH,	0	},
    {	"bite",			"bite",			DAM_PIERCE,	0	},  /* 10 */
    {   "pierce",		"pierce",		DAM_PIERCE,	0	},
    {   "suction",		"suction",		DAM_BASH,	0	},
    {	"beating",		"beating",		DAM_BASH,	0	},
    {   "digestion",		"digestion",		DAM_ACID,	0	},
    {	"charge",		"charge",		DAM_BASH,	0	},  /* 15 */
    { 	"slap",			"slap",			DAM_BASH,	0	},
    {	"punch",		"punch",		DAM_BASH,	0	},
    {	"wrath",		"wrath",		DAM_HOLY,	0	},
    {	"magic",		"magic",		DAM_ENERGY,	0	},
    {   "divine",		"divine power",		DAM_HOLY,	0	},  /* 20 */
    {	"cleave",		"cleave",		DAM_SLASH,	0	},
    {	"scratch",		"scratch",		DAM_PIERCE,	0	},
    {   "peck",			"peck",			DAM_PIERCE,	0	},
    {   "peckb",		"peck",			DAM_BASH,	0	},
    {   "chop",			"chop",			DAM_SLASH,	0	},  /* 25 */
    {   "sting",		"sting",		DAM_PIERCE,	0	},
    {   "smash",	 	"smash",		DAM_BASH,	0	},
    {   "shbite",		"shocking bite",	DAM_LIGHTNING,	0	},
    {   "shockingbite",		"shocking bite",	DAM_LIGHTNING,	0	},
    {	"flbite",		"flaming bite", 	DAM_FIRE,	0	}, /* 30 */
    {	"flamingbite",		"flaming bite", 	DAM_FIRE,	0	},
    {	"frbite",		"freezing bite", 	DAM_COLD,	0	},  
    {	"freezingbite",		"freezing bite",	DAM_COLD,	0	},  
    {	"acbite",		"acidic bite", 		DAM_ACID,	0	},
    {	"acidicbite",		"acidic bite", 		DAM_ACID,	0	}, /* 35 */
    {	"chomp",		"chomp",		DAM_PIERCE,	0	},
    {  	"drain",		"life drain",		DAM_NEGATIVE,	0	},
    {   "thrust",		"thrust",		DAM_PIERCE,	0	},
    {   "slime",		"slime",		DAM_ACID,	0	}, /* 40 */
    {	"shock",		"shock",		DAM_LIGHTNING,	0	},
    {   "thwack",		"thwack",		DAM_BASH,	0	},
    {   "flame",		"flame",		DAM_FIRE,	0	},
    {   "chill",		"chill",		DAM_COLD,	0	}, /* 45 */
    {   "light",		"blast of light",	DAM_LIGHT,	0	}, 
    {   "nature", 		"force of nature",	DAM_ENERGY,    	0	},
    {	"defilement",		"defilement", 		DAM_NEGATIVE, 	0	},
    { 	"mental", 		"mental assault",	DAM_MENTAL, 	0	},
    { 	"sound", 		"sonic assault",	DAM_SOUND,	0	}, /* 50 */
    { 	"searing", 		"searing light",	DAM_LIGHT, 	0	},
    { 	"defile", 		"defilement",		DAM_NEGATIVE, 	0	},
    { 	"acidbite", 		"acidic bite", 		DAM_ACID, 	0	},
    { 	"darkwrath", 		"dark wrath", 		DAM_NEGATIVE, 	0	},
    {	"rending",		"rending gale",		DAM_WIND,	0	}, /* 55 */
    {	"asphyxiation",		"asphyxiation",		DAM_DROWNING,	0	},
    { 	"infinite", 		"infinite darkness", 	DAM_ENERGY,	-30 	},
    {   "chaotic",		"chaotic blast",	DAM_ENERGY, 	0 	},
    {   "sflame",		"searing flame",	DAM_ENERGY,	0 	},
    {   NULL,			NULL,			0,		0	}
};

/* race table */
const 	struct	race_type	race_table	[]		=
{
    { "null",		FALSE, 0, 0, 0, 0, 0, 0, 0, 0 },

    {
	"human",	TRUE,
	0,		0, 		0,
	0, 		0,		0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"dwarf",	TRUE,
	0,		AFF_INFRARED,	0,
	0,		RES_POISON|RES_DISEASE, VULN_DROWNING,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"elf",		TRUE,
        0,              AFF_SNEAK|AFF_INFRARED,	0,
	0,		RES_CHARM,	VULN_IRON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "svirfnebli",   TRUE,
        0,              AFF_DETECT_INVIS|AFF_INFRARED,	0,
        0,              RES_POISON|RES_DISEASE,	VULN_BASH,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "dark-elf",     TRUE,
        0,              AFF_SNEAK|AFF_INFRARED,	0,
        0,              RES_CHARM,	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "centaur",      TRUE,
	0,		0,		0,
        0,              RES_CHARM|RES_BASH,	0,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "troll",        TRUE,
	0,		AFF_INFRARED,	0,
        0,              RES_POISON|RES_DISEASE|RES_WEAPON,	VULN_ACID|VULN_FIRE,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"cloud",	TRUE,
	0,		AFF_FLYING,		0,
	0,		RES_WEAPON|RES_BASH,	VULN_MENTAL|VULN_HOLY,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "gnome",        TRUE,
	0,		0,		0,
        0,              RES_MENTAL|RES_CHARM,	VULN_BASH,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "draconian",    TRUE,
        0,              AFF_FLYING,	0,
        IMM_POISON,     RES_SLASH|RES_ACID|RES_DISEASE,	VULN_COLD,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "ethereal",     TRUE,
	0,		AFF_PASS_DOOR|AFF_FLYING|AFF_INFRARED,	0,
	IMM_POISON,	RES_WEAPON|RES_MENTAL|RES_BASH,	VULN_DROWNING|VULN_LIGHTNING,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"changeling",   TRUE,
	0,		0,		0,
	0,		0,		0,
	H|M|V,		A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"arial",	TRUE,
	0,		AFF_FLYING,	0,
	0,		RES_COLD,VULN_LIGHTNING|VULN_DROWNING,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"fire",		TRUE,
	0,		0,		0,
	0,		RES_WEAPON|RES_BASH|RES_FIRE,	VULN_COLD,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"duergar",	TRUE,
	0,		AFF_INFRARED|AFF_DETECT_HIDDEN,	0,
	0,		RES_POISON,	VULN_LIGHT|VULN_HOLY,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"storm",	TRUE,
	0,		AFF_WATERBREATHING,	0,
	0,		RES_WEAPON|RES_LIGHTNING|RES_DROWNING,	VULN_NEGATIVE,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "lich",         TRUE,
        0,              AFF_DARK_VISION|AFF_FLYING|AFF_PASS_DOOR|AFF_DETECT_INVIS,   0,
        IMM_CHARM|IMM_MENTAL|IMM_POISON|IMM_ENERGY|IMM_DISEASE|IMM_NEGATIVE|IMM_SLEEP|IMM_SUMMON, 0, VULN_HOLY|VULN_ACID|VULN_FIRE,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },
    {
        "angel",         TRUE,
        0,              AFF_DARK_VISION|AFF_FLYING,   0,
        IMM_CHARM|IMM_MENTAL|IMM_POISON|IMM_DISEASE|IMM_HOLY,	RES_MAGIC,	VULN_NEGATIVE,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },
    {
	"wood-elf",	TRUE,
        0,              AFF_SNEAK,	0,
	0,		RES_CHARM|RES_WOOD,	VULN_IRON,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
        "illithid",     TRUE,
        0,		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_MENTAL,	VULN_LIGHT|VULN_FIRE,
	A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"vuuhu",	TRUE,
	0,		AFF_DETECT_INVIS|AFF_FLYING,	0,
	0,		RES_NEGATIVE|RES_FIRE,	VULN_HOLY,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"ruugrah",	TRUE,
	0,		AFF_DETECT_INVIS,	0,
	0,		RES_WEAPON|RES_NEGATIVE|RES_ACID|RES_BASH,	VULN_HOLY,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"malefisti",	TRUE,
	0,		AFF_DETECT_INVIS|AFF_FLYING,	0,
	0,		RES_NEGATIVE|RES_WEAPON|RES_ACID|RES_FIRE,	VULN_HOLY,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {

	"bat", 		FALSE,
	0,		AFF_FLYING|AFF_DARK_VISION,	OFF_DODGE|OFF_FAST,
	0,		0,	VULN_LIGHT,
	A|G|V,		A|C|D|E|F|H|J|K|P
    },

    {
	"bear",		FALSE,
	0,		0,	OFF_CRUSH|OFF_DISARM|OFF_BERSERK,
	0,		RES_BASH|RES_COLD,	0,
	A|G|V,		A|B|C|D|E|F|H|J|K|U|V
    },

    {
	"cat",		FALSE,
	0,		AFF_DARK_VISION,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|U|V
    },

    {
	"centipede",	FALSE,
	0,		AFF_DARK_VISION,	0,
	0,		RES_PIERCE|RES_COLD,	VULN_BASH,
 	A|B|G|O,		A|C|K
    },

    {
	"dog",		FALSE,
	0,		0,		OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|U|V
    },

    {
	"doll",		FALSE,
	0,		0,		0,
	IMM_COLD|IMM_POISON|IMM_HOLY|IMM_NEGATIVE|IMM_MENTAL|IMM_DISEASE
	|IMM_DROWNING,	RES_BASH|RES_LIGHT,
	VULN_SLASH|VULN_FIRE|VULN_ACID|VULN_LIGHTNING|VULN_ENERGY,
	E|J|M|cc,	A|B|C|G|H|K
    },

    { 	"dragon", 	FALSE,
	0, 		AFF_INFRARED|AFF_FLYING,	0,
	0,		RES_FIRE|RES_BASH|RES_CHARM,	VULN_PIERCE|VULN_COLD,
	A|H|Z,		A|C|D|E|F|G|H|I|J|K|P|Q|U|V|X
    },

    {
	"fido",		FALSE,
	0,		0,	OFF_DODGE|ASSIST_RACE,
	0,		0,	VULN_MAGIC,
	A|B|G|V,	A|C|D|E|F|H|J|K|Q|V
    },

    {
	"fish",		FALSE,
	0,		AFF_WATERBREATHING,	OFF_DODGE,
	0,		RES_SUMMON|RES_CHARM,	0,
	A|G|V,		A|D|F|K|O|Q|X
    },

    {
	"fox",		FALSE,
	0,		AFF_DARK_VISION,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K|Q|V
    },

    {
	"goblin",	FALSE,
	0,		AFF_INFRARED,	0,
	0,		RES_DISEASE,	VULN_MAGIC,
	A|H|M|V,	A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"hobgoblin",	FALSE,
	0,		AFF_INFRARED,	0,
	0,		RES_DISEASE|RES_POISON,	0,
	A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K|Y
    },

    {
	"kobold",	FALSE,
	0,		AFF_INFRARED,	0,
	0,		RES_POISON,	VULN_MAGIC,
	A|B|H|M|V,	A|B|C|D|E|F|G|H|I|J|K|Q
    },

    {
	"lizard",	FALSE,
	0,		0,		0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|cc,	A|C|D|E|F|H|K|Q|V
    },

    {
	"modron",	FALSE,
	0,		AFF_INFRARED,		ASSIST_RACE|ASSIST_ALIGN,
	IMM_CHARM|IMM_DISEASE|IMM_MENTAL|IMM_HOLY|IMM_NEGATIVE,
			RES_FIRE|RES_COLD|RES_ACID,	0,
	H,		A|B|C|G|H|J|K
    },

    {
	"pig",		FALSE,
	0,		0,		0,
	0,		0,		0,
	A|G|V,	 	A|C|D|E|F|H|J|K
    },

    {
	"rabbit",	FALSE,
	0,		0,		OFF_DODGE|OFF_FAST,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|H|J|K
    },

    {
	"school monster",	FALSE,
	ACT_NOALIGN,		0,		0,
	IMM_CHARM|IMM_SUMMON,	0,		VULN_MAGIC,
	A|M|V,		A|B|C|D|E|F|H|J|K|Q|U
    },

    {
	"snake",	FALSE,
	0,		0,		0,
	0,		RES_POISON,	VULN_COLD,
	A|G|X|Y|cc,	A|D|E|F|K|L|Q|V|X
    },

    {
	"song bird",	FALSE,
	0,		AFF_FLYING,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|W,		A|C|D|E|F|H|K|P
    },

    {
	"water fowl",	FALSE,
	0,		AFF_WATERBREATHING|AFF_FLYING,	0,
	0,		RES_DROWNING,	0,
	A|G|W,		A|C|D|E|F|H|K|P
    },

    {
	"wolf",		FALSE,
	0,		AFF_DARK_VISION,	OFF_FAST|OFF_DODGE,
	0,		0,		0,
	A|G|V,		A|C|D|E|F|J|K|Q|V
    },

    {
	"wyvern",	FALSE,
	0,		AFF_FLYING|AFF_DETECT_INVIS|AFF_DETECT_HIDDEN,
	OFF_BASH|OFF_FAST|OFF_DODGE,
	IMM_POISON,	0,	VULN_LIGHT,
	A|B|G|Z,	A|C|D|E|F|H|J|K|Q|V|X
    },

    {
	"nymph",	FALSE,
	0,		AFF_DETECT_INVIS|AFF_INFRARED, OFF_DODGE|OFF_FAST,
	IMM_LIGHTNING, 0, VULN_SLASH,
	A|B|G,	A|C|D|E|F|H|J|K|V|X
    },

    {
        "titan",        FALSE,
        0,              AFF_REGENERATION,	0,
        0,              RES_WEAPON|RES_BASH,	0,
        A|H|M|V,        A|B|C|D|E|F|G|H|I|J|K
    },

    {
	"unique",	FALSE,
	0,		0,		0,
	0,		0,		0,
	0,		0
    },

    {
	NULL, 0, 0, 0, 0, 0, 0
    }
};

const	struct	pc_race_type	pc_race_table	[]	=
{
    { "null race", "", 0, 0, { "" }, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 13, 13, 13, 13, 13 }, { 18, 18, 18, 18, 18 }, 0, FALSE },
    {
        "human",	"Human",	ALIGN_ANY, 0,
	{ "" },
	{ 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1 },
        { 0, 0, 0, 0, 00 }, { 20, 20, 20, 20, 20 }, SIZE_MEDIUM, FALSE
    },

    {
        "dwarf",	"Dwarf",	ALIGN_GN, 250,
        { "berserk" },
        { 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 2, -2, 1, -1, 3 }, { 22, 18, 22, 16, 25 }, SIZE_MEDIUM, FALSE
    },

    {
        "elf",		" Elf ",	ALIGN_G, 400,
        { "awareness","forest blending" },
        { 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1 },
        { -2, 3, 0, 2, -2 }, { 18, 25, 20, 23, 17 }, SIZE_MEDIUM, FALSE
    },

    {
        "svirfnebli",	"Svirf", 	ALIGN_N, 250,
        { "lore" },
	{ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
        { 2, 2, 2, 2, -2 }, { 22, 18, 22, 20, 23}, SIZE_SMALL, FALSE
    },
    {
        "dark-elf",     "D-Elf",	ALIGN_E, 400,
        { "globe of darkness", "infravision" },
	{ 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1 },
        { -1, 3, -2, 2, -1 }, { 18, 24, 20, 24, 17 }, SIZE_MEDIUM, FALSE
    },

    {
        "centaur",	" Cen ",	ALIGN_G, 500,
        { "rear kick", "enhanced damage", "kick", "bash" },
	{ 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0 },
        { 2, -2, -2, 0, 2 }, { 24, 18, 20, 20, 22 }, SIZE_LARGE, FALSE
    },

    {
        "troll",	"Troll",	ALIGN_E, 450,
        { "regeneration","enhanced damage", "bash", "door bash" },
	{ 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
        { 3, -3, -2, -2, 3 }, { 24, 16, 17, 17, 24 }, SIZE_LARGE, FALSE
    },

    {
        "cloud",	"Cloud",	ALIGN_N, 500,
        { "door bash","enhanced damage", "bash" },
	{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 },
        { 3, -2, -2, -1, 3 }, { 24, 16, 17, 18, 23 }, SIZE_HUGE, FALSE
    },

    {
        "gnome",	"Gnome",	ALIGN_N, 200,
        { "staves", "wands" },
	{ 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1 },
        { -2, 3, 2, 0, -1 }, { 18, 23, 25, 20, 19 }, SIZE_SMALL, FALSE
    },

    {
        "draconian",	"Draco",	ALIGN_E, 300,
        { "breath fire" },
        { 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 2, 1, -1, 0, 0 }, { 22, 23, 21, 18, 20 }, SIZE_MEDIUM, FALSE
    },

    {
        "ethereal",	" Eth ",	ALIGN_GN, 400,
        { "" },
        { 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1 },
        { -3, 2, 2, 2, -1 }, { 17, 24, 24, 22, 17 }, SIZE_SMALL, FALSE
    },

    {
	"changeling",	"Chnge",	ALIGN_ANY, 500,
	{""},
	{ 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0 },
	{-2,3,2,3,-3}, { 17, 23, 22, 24, 15 },	SIZE_MEDIUM, TRUE
    },

    {
	"arial",	"Arial",	ALIGN_ANY, 400,
	{""},
	{ 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1 },
	{ 2, -1, -1, -1, 2 }, { 19, 23, 20, 25, 19 }, SIZE_MEDIUM, FALSE
    },

    {
        "fire",		"Fire",		ALIGN_E, 500,
        { "door bash","enhanced damage", "bash" },
	{ 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 3, -2, -2, -1, 3 }, { 25, 15, 17, 17, 24 }, SIZE_HUGE, FALSE
    },

    {
        "duergar",	"Duerg",	ALIGN_E, 250,
        { "berserk", "pick lock" },
        { 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
        { 2, -2, 1, -1, 3}, { 23, 16, 18, 21, 23 }, SIZE_MEDIUM, FALSE
    },
    {
        "storm",	"Storm",	ALIGN_G, 500,
        { "door bash","enhanced damage", "bash" },
	{ 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
        { 2, -1, -1, -1, 2 }, { 24, 17, 17, 17, 23 }, SIZE_HUGE, FALSE
    },

    {
	"lich",		"Lich",		ALIGN_E, 0,
	{"animate dead"},
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 }, { 25, 25, 25, 25, 25 }, SIZE_MEDIUM, TRUE
    },

    {
	"angel",	"Angel",	ALIGN_G, 0,
	{},
	{ 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1 },
	{ 0, 0, 0, 0, 0 }, { 25, 25, 25, 25, 25 }, SIZE_MEDIUM, TRUE
    },

    {
        "wood-elf",	"W-Elf",	ALIGN_N, 340,
        { "dispel magic","forest blending" },
        { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
        { -2, 3, 1, -2, -1 }, { 19, 24, 21, 25, 18 }, SIZE_MEDIUM, FALSE
    },

    {
	"illithid",	"Illd", 	ALIGN_E, 350,
	{ "hand to hand", "psionic blast" },
        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0 },
	{ -3, 3, 2, 0, -2 }, { 16, 25, 23, 20, 18 }, SIZE_MEDIUM, FALSE
    },

    {
	"vuuhu",	"Vuuhu",	ALIGN_E,  500,
	{ "agility" },
	{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 }, { 21, 20, 20, 25, 23 }, SIZE_SMALL, TRUE
    },

    {
	"ruugrah",	"Rugrh",	ALIGN_E,  500,
	{ "enhanced damage III" },
	{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 }, { 25, 19, 18, 23, 25 }, SIZE_HUGE, TRUE
    },

    {
	"malefisti",	"Malef",	ALIGN_E,  500,
	{ "trance", "meditation" },
	{ 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0 },
	{ 0, 0, 0, 0, 0}, { 19, 25, 25, 24, 21 }, SIZE_SMALL, TRUE
    },

};




/*
 * Class table.
 */
const	struct	class_type	class_table	[MAX_CLASS]	=
{
    {	// 0
        "warrior", "War",	ALIGN_ANY , 0, OBJ_VNUM_SCHOOL_SWORD,
        { 3022, 8818, 20408 },  75,  20,  -10,  8, 19, FALSE,
	"class basics", "class default", CLASS_FIGHTER
    },

    {	// 1
        "thief", "Thi",		ALIGN_ANY, 0, OBJ_VNUM_SCHOOL_DAGGER,
        { 3028, 8850, 20525 },  75,  20,  -4,  8, 17, FALSE,
	"class basics", "class default", CLASS_FIGHTER
    },

    { 	// 2
        "zealot", "Zea",  	ALIGN_GE, 0, OBJ_VNUM_SCHOOL_SWORD,
        { 3003, 8836, 20519 },  75,  20, -6,  10, 13, TRUE,
	"class  basics", "class default", CLASS_COMMUNER
    },

    {	// 3
        "paladin", "Pal",  	ALIGN_G, 500, OBJ_VNUM_SCHOOL_SWORD,
        { 3370, 8835, 9754, 20526 },  75,  20, -6,  8,  18, TRUE,
        "class basics", "class default", CLASS_COMMUNER
    },

    {	// 4
        "anti-paladin", "A-P",	ALIGN_E, 400, OBJ_VNUM_SCHOOL_SWORD,
        { 3368, 8820, 20515 },  75,  20, -6,  8,  18, TRUE,
        "class basics", "class default", CLASS_CASTER
    },

    {	// 5
        "ranger", "Ran",	ALIGN_ANY, 250, OBJ_VNUM_SCHOOL_SWORD,
        { 6156, 1874, 1890, 1891, 20402 },  75,  20, -8,  8,  18, FALSE,
        "class basics", "class default", CLASS_FIGHTER
    },

    {	// 6
        "druid",	"Dru",	ALIGN_N, 0, OBJ_VNUM_SCHOOL_MACE,
        { 8819, 20519 },  75,  20, 2,  7,  15, FALSE,
        "class basics", "class default", CLASS_COMMUNER
    },

    {	// 7	
        "channeler", 	"Cha",	ALIGN_ANY, 100, OBJ_VNUM_SCHOOL_DAGGER,
        { 8817, 20401 },  75,  20, 2,  7,  15, TRUE,
        "class basics", "class default", CLASS_CASTER
    },

    {	// 8
        "assassin", "Asn",	ALIGN_ANY, 0, OBJ_VNUM_SCHOOL_SWORD,
        { 3361, 8837, 20530 },  75,  20, -4,  8,  17, FALSE,
        "class basics", "class default", CLASS_FIGHTER
    },

    {	// 9
        "necromancer", "Nec",	ALIGN_E, 100, OBJ_VNUM_SCHOOL_DAGGER,
        { 3363, 8815, 20407 },  75,  20, -2,  7,  15, TRUE,
        "class basics", "class default", CLASS_CASTER
    },

    {	// 10
        "elementalist", "Ele",	ALIGN_ANY, 200, OBJ_VNUM_SCHOOL_DAGGER,
        { 8897, 20401 },  75,  20, -4,  8,  16, TRUE,
        "class basics", "class default", CLASS_CASTER
    },

    {	// 11
    	"bard", 	"Bar",	ALIGN_ANY, 200, OBJ_VNUM_SCHOOL_WHIP,
    	{ 8897, 20401 }, 75,  20, -4,  8,  11, TRUE,
    	"class basics", "class default", CLASS_SING
    },

    {	// 12
	"healer",	"Hea",	ALIGN_ANY, 300, OBJ_VNUM_SCHOOL_MACE,
	{ 3366, 8897 },	75, 20, -10, 8, 14, TRUE,
	"class basics", "class default", CLASS_COMMUNER
    }

};

/*
 * Attribute bonus tables.
 */
const	struct	str_app_type	str_app		[26]		=
{
//AC Modifier, Hit/Dam Modifier, Carry Weight Limit, Wield Weight Limit, When Strength is the Commented Value
    
    {  0, -4,   0,   0 },  /* 0  */
    {  0, -4,   3,   0 },  /* 1  */
    {  0, -2,   3,   1 },
    { 50, -1,  10,   4 },  /* 3  */
    { 40, -1,  25,   5 },
    { 30, -1,  55,   6 },  /* 5  */
    { 20,  0,  80,   7 },
    { 15,  0, 100,  9 },
    { 10,  0, 150,  14 },
    {  5,  0, 180,  19 },
    {  0,  0, 200,  22 }, /* 10  */
    {  0,  0, 215,  26 },
    {  0,  0, 230,  30 },
    {  0,  0, 230,  34 }, /* 13  */
    {  0,  1, 240,  38 },
    {  0,  1, 250,  42 }, /* 15  */
    {  0,  2, 265,  46 },
    {  0,  3, 280,  50 },
    {  0,  3, 300,  54 }, /* 18  */
    { -5,  4, 325,  58 },
    {-10,  5, 350,  62 }, /* 20  */
    {-15,  6, 400,  66 },
    {-20,  6, 450,  70 },
    {-30,  7, 500,  75 },
    {-40,  8, 550,  80 },
    {-50,  9, 600,  85 }  /* 25   */
};



const	struct	int_app_type	int_app		[26]		=
{
    {  3 },	/*  0 */
    {  5 },	/*  1 */
    {  7 },
    {  8 },	/*  3 */
    {  9 },
    { 10 },	/*  5 */
    { 11 },
    { 12 },
    { 13 },
    { 15 },
    { 17 },	/* 10 */
    { 19 },
    { 22 },
    { 25 },
    { 28 },
    { 31 },	/* 15 */
    { 34 },
    { 37 },
    { 40 },	/* 18 */
    { 44 },
    { 49 },	/* 20 */
    { 55 },
    { 60 },
    { 70 },
    { 80 },
    { 85 }	/* 25 */
};



const	struct	wis_app_type	wis_app		[26]		=
{
    { 0 },	/*  0 */
    { 0 },	/*  1 */
    { 0 },
    { 0 },	/*  3 */
    { 0 },
    { 1 },	/*  5 */
    { 1 },
    { 1 },
    { 1 },
    { 1 },
    { 1 },	/* 10 */
    { 1 },
    { 1 },
    { 1 },
    { 1 },
    { 2 },	/* 15 */
    { 2 },
    { 2 },
    { 3 },	/* 18 */
    { 3 },
    { 3 },	/* 20 */
    { 3 },
    { 4 },
    { 4 },
    { 4 },
    { 5 }	/* 25 */
};



const	struct	dex_app_type	dex_app		[26]		=
{
    {   60,0 },   /* 0 */
    {   50,0 },   /* 1 */
    {   50,0 },
    {   40,0 },
    {   30,0 },
    {   20,0 },   /* 5 */
    {   10,0 },
    {    0,0 },
    {    0,0 },
    {    0,0 },
    {    0,0 },   /* 10 */
    {    0,1 },
    {    0,1 },
    {    0,1 },
    {    0,2 },
    { - 10,2 },   /* 15 */
    { - 15,2 },
    { - 20,3 },
    { - 30,3 },
    { - 40,4 },
    { - 50,4 },   /* 20 */
    { - 60,5 },
    { - 75,6 },
    { - 90,7 },
    { -105,8 },
    { -120,9 }    /* 25 */
};


const	struct	con_app_type	con_app		[26]		=
{
    { -4, 20 },   /*  0 */
    { -3, 25 },   /*  1 */
    { -2, 30 },
    { -2, 35 },	  /*  3 */
    { -1, 40 },
    { -1, 45 },   /*  5 */
    { -1, 50 },
    {  0, 55 },
    {  0, 60 },
    {  0, 65 },
    {  0, 70 },   /* 10 */
    {  0, 75 },
    {  0, 80 },
    {  0, 85 },
    {  0, 88 },
    {  1, 90 },   /* 15 */
    {  2, 95 },
    {  2, 97 },
    {  3, 99 },   /* 18 */
    {  3, 99 },
    {  4, 99 },   /* 20 */
    {  4, 99 },
    {  5, 99 },
    {  6, 99 },
    {  7, 99 },
    {  8, 99 }    /* 25 */
};



/*
 * Liquid properties.
 * Used in world.obj.
 */
const	struct	liq_type	liq_table	[]	=
{
/*    name			color	proof, full, thirst, food, ssize */
    { "water",			"clear",	{   0, 1, 10, 0, 16 }	},
    { "beer",			"amber",	{  12, 1,  8, 1, 12 }	},
    { "red wine",		"burgundy",	{  30, 1,  8, 1,  5 }	},
    { "ale",			"brown",	{  15, 1,  8, 1, 12 }	},
    { "dark ale",		"dark",		{  16, 1,  8, 1, 12 }	},

    { "whisky",			"golden",	{ 120, 1,  5, 0,  2 }	},
    { "lemonade",		"pink",		{   0, 1,  9, 2, 12 }	},
    { "firebreather",		"boiling",	{ 190, 0,  4, 0,  2 }	},
    { "local specialty",	"clear",	{ 151, 1,  3, 0,  2 }	},
    { "slime mold juice",	"green",	{   0, 2, -8, 1,  2 }	},

    { "milk",			"white",	{   0, 2,  9, 3, 12 }	},
    { "tea",			"tan",		{   0, 1,  8, 0,  6 }	},
    { "coffee",			"black",	{   0, 1,  8, 0,  6 }	},
    { "blood",			"red",		{   0, 2, -1, 2,  6 }	},
    { "salt water",		"clear",	{   0, 1, -2, 0,  1 }	},

    { "coke",			"brown",	{   0, 2,  9, 2, 12 }	},
    { "root beer",		"brown",	{   0, 2,  9, 2, 12 }   },
    { "elvish wine",		"green",	{  35, 2,  8, 1,  5 }   },
    { "white wine",		"golden",	{  28, 1,  8, 1,  5 }   },
    { "champagne",		"golden",	{  32, 1,  8, 1,  5 }   },

    { "mead",			"honey-colored",{  34, 2,  8, 2, 12 }   },
    { "rose wine",		"pink",		{  26, 1,  8, 1,  5 }	},
    { "benedictine wine",	"burgundy",	{  40, 1,  8, 1,  5 }   },
    { "vodka",			"clear",	{ 130, 1,  5, 0,  2 }   },
    { "cranberry juice",	"red",		{   0, 1,  9, 2, 12 }	},

    { "orange juice",		"orange",	{   0, 2,  9, 3, 12 }   },
    { "absinthe",		"green",	{ 200, 1,  4, 0,  2 }	},
    { "brandy",			"golden",	{  80, 1,  5, 0,  4 }	},
    { "aquavit",		"clear",	{ 140, 1,  5, 0,  2 }	},
    { "schnapps",		"clear",	{  90, 1,  5, 0,  2 }   },

    { "icewine",		"purple",	{  50, 2,  6, 1,  5 }	},
    { "amontillado",		"burgundy",	{  35, 2,  8, 1,  5 }	},
    { "sherry",			"red",		{  38, 2,  7, 1,  5 }   },
    { "framboise",		"red",		{  50, 1,  7, 1,  5 }   },
    { "rum",			"amber",	{ 151, 1,  4, 0,  2 }	},

    { "cordial",		"clear",	{ 100, 1,  5, 0,  2 }   },
    { NULL,			NULL,		{   0, 0,  0, 0,  0 }	}
};



/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)	n

const	struct	skill_type	skill_table	[MAX_SKILL]	=
{

/*
 * Magic spells.
 */

    {
        "reserved",
        { 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
        { 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	0,			TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT( 0),	 0,	 0,
	"",			"",		"", CMD_NONE
    },


    {
	"absorb",
	{ 53, 53, 53, 53, 53, 53, 53, 33, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_absorb,		TAR_CHAR_SELF,			POS_STANDING,
	&gsn_absorb,		SLOT(551),	100,	12,
	"",			"The shimmering fields around you fades.",
	"$p's shimmering field fades.", CMD_SPELL
    },

    {
        "acid blast",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_acid_blast,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(70),	20,	24,
	"acid blast",		"!Acid Blast!", "", CMD_SPELL
    },

    {
        "acid needles",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 51, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_acidneedles,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(0),	20,	24,
	"acidic needle",	"!Acid Needles!", "", CMD_SPELL
    },

    {
	"embalm",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 15, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_embalm,		SLOT( 0),	20,	12,
	"",	"","", CMD_NONE
    },

    {
	"trace",
	{ 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_trace,		TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(731),	60,	12,
	"",	"",	"", CMD_POWER
    },

    {
	"flame scorch",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	spell_flame_scorch,	TAR_IGNORE,			POS_FIGHTING,
	&gsn_flame_scorch,	SLOT(912),	50,	12,
	"searing blue-green flame",	"",	"", CMD_NONE
    },

    {
        "soulbind",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 60, 61, 61, 61 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_soulbind,      	TAR_CHAR_DEFENSIVE, 		POS_STANDING,
        NULL,   		SLOT(913),	100,	12,
        "",     "",     "", CMD_SPELL
    },
	
	 {
        "banshee call",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 51, 61, 61, 61 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_banshee_call,      	TAR_IGNORE, 		POS_STANDING,
        &gsn_banshee_call,   		SLOT(913),	300,	36,
        "",     "You feel ready to summon more banshees.",     "", CMD_SPELL
    },

    {
        "shattered bone",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null,     	TAR_IGNORE,			POS_DEAD,
        &gsn_shattered_bone,    SLOT(0),	0,	0,
        "fragments of blasted bone",    "",     "", CMD_NONE
    },

    {
        "blood tide",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null,		TAR_IGNORE,			POS_DEAD,
        &gsn_blood_tide,        SLOT(0), 	0,      0,
        "bloody tide", "",     "", CMD_NONE
    },
/* const.c skill table stuff for flesh-cutter */
    {
        "mutilated left hand",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null,		TAR_IGNORE,			POS_DEAD,
        &gsn_mutilated_left_hand,	SLOT(0),	0,	0,
        "",     "The feeling in your mangled left hand returns.",
        "", CMD_NONE
    },

    {
        "mutilated right hand",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null,		TAR_IGNORE,			POS_DEAD,
        &gsn_mutilated_right_hand,	SLOT(0),	0,	0,
        "",     "The feeling in your mangled right hand returns.",
        "", CMD_NONE
    },

    {
        "mutilated left arm",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null,		TAR_IGNORE,			POS_DEAD,
        &gsn_mutilated_left_arm,	SLOT(0),	0,	0,
        "",     "The feeling in your mangled left arm returns.",
        "", CMD_NONE
    },

    {
        "mutilated right arm",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null,		TAR_IGNORE,			POS_DEAD,
        &gsn_mutilated_right_arm,	SLOT(0),	0,	0,
        "",     "The feeling in your mangled right arm returns.",
        "", CMD_NONE
    },

    {
        "mutilated left leg",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null,		TAR_IGNORE,			POS_DEAD,
        &gsn_mutilated_left_leg,	SLOT(0),        0,      0,
        "",     "The feeling in your mangled left leg returns.",
        "", CMD_NONE
    },

    {
        "mutilated right leg",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null,            TAR_IGNORE,			POS_DEAD,
        &gsn_mutilated_right_leg,	SLOT(0),        0,      0,
        "",     "The feeling in your mangled right leg returns.",
        "", CMD_NONE
    },

    {
	"icy wind",
	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	spell_null,		TAR_IGNORE,			POS_DEAD,
	&gsn_icy_wind,		SLOT(212),	0,	0,
	"icy wind",	"!icywind!","", CMD_SPELL
    },

    {
	"electric storm",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_electric_storm,	TAR_IGNORE,			POS_STANDING,
	&gsn_electric_storm,	SLOT(214),	200,	12,
	"",	"The storm is ready to obey your summons again.","", CMD_SPELL
    },

    {
        "deathspell",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_deathspell,       TAR_IGNORE,			POS_STANDING,
        NULL,   		SLOT(740),      150,    12,
        "explosion of negative energy",  "", "", CMD_SPELL
    },

    {
        "forest blending",
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_STANDING,
        &gsn_forest_blending,   SLOT(0),        0,      0,
        "",     "",	"", CMD_NONE
    },

    {
        "awareness",
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,		TAR_IGNORE,			POS_STANDING,
        &gsn_awareness,		SLOT(0),	0,	0,
        "",     "Your awareness of the surrounding trees lessens.", CMD_NONE
    },

    {
	"door bash",
	{ 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_door_bash,		SLOT(0),	0,	0,
	"failed door bashing",	"",	"", CMD_NONE
    },

    {
        "breath fire",
        { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,		TAR_IGNORE,			POS_FIGHTING,
        &gsn_breath_fire,       SLOT(0),        0,      0,
        "blast of flame",
        "Your body once more has retained enough internal heat to breath fire again.", CMD_NONE
    },

    {
	"regeneration",
	{ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_regeneration,	SLOT(0),	0,	0,
	"",	"",	"", CMD_SPELL
    },

    {
	"brand",
	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	spell_null,		TAR_IGNORE,			POS_DEAD,
	&gsn_wolverine_brand,	SLOT(0),	0,	0,
	"summoned demons",	"The fire coursing through your blood cools down.", "", CMD_NONE
    },

    {
	"silence",
	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	spell_null,		TAR_IGNORE,			POS_DEAD,
	&gsn_silence,		SLOT(0),	0,	0,
	"",	"You feel the words come back to you.", "", CMD_SPELL
    },

    {
	"brand",
	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	spell_null,		TAR_IGNORE,			POS_DEAD,
	&gsn_parrot,		SLOT(0),	0,	0,
	"unholy fury",	"You feel once again ready to call upon your god.", "", CMD_NONE
    },

    {
	"hunger",
	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	spell_null,		TAR_IGNORE,			POS_DEAD,
	&gsn_starvation,	SLOT(0),	0,	0,
	"starvation",	"You no longer feel the pangs of famine.", "", CMD_NONE
    },

    {
	"dehydrated",
	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	spell_null,		TAR_IGNORE,			POS_DEAD,
	&gsn_dehydrated,	SLOT(0),	0,	0,
	"dehydration",	"You no longer feel so thirsty.", "", CMD_NONE
    },

    {
        "rear kick",
        { 18, 53, 27, 23, 53, 20, 32, 32, 32, 32, 32, 20, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_rear_kick, 	SLOT(0),        0,      0,
        "rear kick",     "", "", CMD_NONE
    },

    {
	"spike",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_spike,		SLOT(0),	0,	0,
	"spike",	"", "", CMD_NONE
    },

    {
        "lore",
        { 18, 18, 53, 53, 53, 18, 53, 53, 18, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,		TAR_IGNORE,     		POS_RESTING,
        &gsn_lore,      	SLOT(0),        0,      0,
        "",     "", "", CMD_NONE
    },

    {
	"globe of darkness",
	{ 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_globe_of_darkness,TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(780),	100,	12,
	"",
	"Your power to call upon the darkness returns.", "", CMD_POWER
    },

    {
		"psionic blast",
	{ 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_psi_blast,TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_psi_blast,		SLOT(816),	50,	12,
	"psionic blast",
	"You can once again assault minds.",	"", CMD_SPELL
	},

    {
        "animate object",
        { 53, 53, 53, 53, 53, 53, 53, 35, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_animate_object,   TAR_OBJ_INV,    		POS_STANDING,
        NULL,   		SLOT(741),      100,    12,
        "",     "", 	"", CMD_SPELL
    },

    {
        "lifebane",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_lifebane,		TAR_IGNORE,			POS_FIGHTING,
        NULL,			SLOT(742),      45,     12,
        "lifebane",     "", "", CMD_SPELL
    },

    {
        "demand",
        { 53, 53, 53, 53, 30, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,		TAR_IGNORE,			POS_STANDING,
        &gsn_demand,		SLOT(0),        0,      12,
        "",     "", "", CMD_NONE
    },

    {
	"animate dead",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 45, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_animate_dead,	TAR_IGNORE,			POS_STANDING,
	&gsn_animate_dead,	SLOT(570),	100,	0,
	"",			"Your power to raise dead returns.",	"", CMD_SPELL
    },

    {
	"lesser golem",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 20, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_lesser_golem, 	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(591),	70,	24,
	"",	"You feel up to fashioning another golem.", "", CMD_SPELL
    },

    {
	"greater golem",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 33, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_greater_golem, 	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(591),	70,	24,
	"",	"You feel up to fashioning another iron golem.", "", CMD_SPELL
    },

    {
        "armor",
        { 53, 53, 53, 5, 5, 7, 1, 1, 53, 1, 1, 53, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_armor,		TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT( 1),	5,	12,
	"",			"You feel less armored.",	"", CMD_BOTH
    },

    {
        "bless",
        { 53, 53, 53, 4, 53, 53, 53, 53, 53, 53, 53, 53, 7 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_bless,		TAR_OBJ_CHAR_DEF,		POS_STANDING,
	NULL,			SLOT( 3),	5,	12,
	"",	"You feel less righteous.",
	"$p's holy aura fades.", CMD_COMMUNE
    },

    {
        "blindness",
        { 53, 53, 11, 53, 9, 53, 53, 53, 53, 12, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_blindness,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	&gsn_blindness,		SLOT( 4),	5,	12,
	"",			"You can see again.",	"", CMD_BOTH
    },

    {
        "burning hands",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_burning_hands,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT( 5),	15,	12,
	"burning hands",	"!Burning Hands!", 	"", CMD_SPELL
    },

    {
        "call lightning",
        { 53, 53, 53, 53, 53, 22, 18, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_call_lightning,	TAR_IGNORE,			POS_FIGHTING,
	NULL,			SLOT( 6),	15,	12,
	"lightning bolt",	"!Call Lightning!",	"", CMD_BOTH
    },

    {
	"call slaves",
	{ 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36, 36 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_call_slaves,	TAR_IGNORE,			POS_FIGHTING,
	&gsn_slaves,			SLOT(906),	25,	12,
	"",	"You feel as though you may call more slaves.",	"", CMD_POWER
    },

    {   "calm",
    	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 14, 53, 16 },
    	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_calm,		TAR_IGNORE,			POS_FIGHTING,
	NULL,			SLOT(509),	30,	12,
	"",			"You have lost your peace of mind.",	"", CMD_BOTH
    },

    {
        "cancellation",
        { 53, 53, 19, 53, 53, 53, 23, 18, 53, 53, 53, 53, 24 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_cancellation,	TAR_CHAR_DEFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(507),	20,	12,
	"",			"!cancellation!",	"", CMD_BOTH
    },

    {
        "cause critical",
        { 53, 53, 18, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_cause_critical,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(63),	20,	12,
	"spell",		"!Cause Critical!",	"", CMD_COMMUNE
    },

    {
        "cause light",
        { 53, 53, 3, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_cause_light,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(62),	15,	12,
	"spell",		"!Cause Light!",	"", CMD_COMMUNE
    },

    {
        "cause serious",
        { 53, 53, 12, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_cause_serious,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(64),	17,	12,
	"spell",		"!Cause Serious!",	"", CMD_COMMUNE
    },

    {
        "chain lightning",
        { 53, 53, 53, 53, 53, 53, 53, 26,53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_chain_lightning,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(500),	25,	12,
	"lightning",		"!Chain Lightning!",	"", CMD_SPELL
    },

    {
        "change sex",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_change_sex,	TAR_CHAR_DEFENSIVE,		POS_FIGHTING,
	&gsn_change_sex,	SLOT(82),	15,	12,
	"",			"Your body feels familiar again.",	"", CMD_SPELL
    },

    {
        "charm person",
        { 53, 53, 53, 53, 53, 53, 53, 50, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_charm_person,	TAR_CHAR_OFFENSIVE,		POS_STANDING,
	&gsn_charm_person,	SLOT( 7),	 80,	12,
	"",			"You feel more self-confident.",	"", CMD_SPELL
    },

    {
        "chill touch",
        { 53, 53, 53, 53, 3, 53, 53, 53, 53, 3, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_chill_touch,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT( 8),	15,	12,
	"chilling touch",	"You feel less cold.",	"", CMD_SPELL
    },

    {
        "colour spray",
        { 53, 53, 53, 53, 53, 53, 53, 16, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_colour_spray,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(10),	15,	12,
	"colour spray",		"!Colour Spray!",	"", CMD_SPELL
    },

    {
	"firestream",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_firestream,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(565),	20,	12,
	"firestream",	"",	"", CMD_SPELL
    },

    {
	"fireshield",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_fireshield,	TAR_IGNORE,			POS_STANDING,
	&gsn_fireshield,	SLOT(617),	60,	12,
	"",	"Your flaming shield dissipates.",	"", CMD_SPELL
    },

    {
	"iceshield",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_iceshield,	TAR_IGNORE,			POS_STANDING,
	&gsn_iceshield,		SLOT(618),	60,	12,
	"",	"Your frost shield dissipates.",	"", CMD_SPELL
    },

    {
	"concatenate",
	{ 53, 53, 53, 53, 53, 53, 53, 45, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_concatenate,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(568),	30,	12,
	"blast of energy",	"!Concatenate!",	"", CMD_SPELL
    },

    {
	"heavenly wrath",
	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	spell_heavenly_sceptre_frenzy,	TAR_CHAR_SELF,		POS_FIGHTING,
	NULL,			SLOT(900),	0,	0,
	"",	"The heavenly wrath bestowed upon you fades away.",	"", CMD_NONE
    },

    {
	"heavenly fire",
	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	spell_heavenly_sceptre_fire,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(901),	0,	0,
	"heavenly fire",	"You feel the power drained from you by the scetpre's heavenly fire return.",	"", CMD_NONE
    },

    {
	"wrath",
	{ 53, 53, 53, 21, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_wrath,		TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(553),	20,	17,
	"heavenly wrath",	"!wrath!",	"", CMD_COMMUNE
    },

    {
        "continual light",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_continual_light,	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(57),	 7,	12,
	"",			"!Continual Light!",	"", CMD_SPELL
    },

    {
        "control weather",
        { 53, 53, 53, 53, 53, 20, 53, 53, 53, 53, 14, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_control_weather,	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(11),	25,	12,
	"",			"!Control Weather!",	"", CMD_SPELL
    },

    {
        "create food",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_create_food,	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(12),	 5,	12,
	"",			"!Create Food!",	"", CMD_BOTH
    },

    {
        "create rose",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_create_rose,	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(511),	30, 	12,
	"",			"!Create Rose!",	"", CMD_SPELL
    },

    {
        "create spring",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_create_spring,	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(80),	20,	12,
	"",			"!Create Spring!",	"", CMD_BOTH
    },

    {
        "create water",
        { 53, 53, 53, 12, 53, 53, 53, 53, 53, 53, 8, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_create_water,	TAR_OBJ_INV,			POS_STANDING,
	NULL,			SLOT(13),	 5,	12,
	"",			"!Create Water!",	"", CMD_BOTH
    },

    {
        "cure blindness",
        { 53, 53, 13, 15, 53, 53, 53, 53, 53, 53, 53, 53, 7 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_cure_blindness,	TAR_CHAR_DEFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(14),	 5,	12,
	"",			"!Cure Blindness!",	"", CMD_COMMUNE
    },

    {
        "cure critical",
        { 53, 53, 18, 20, 53, 53, 12, 53, 53, 53, 53, 53, 9 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_cure_critical,	TAR_CHAR_DEFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(15),	20,	12,
	"",			"!Cure Critical!",	"", CMD_COMMUNE
    },

    {
        "cure disease",
        { 53, 53, 15, 18, 53, 53, 53, 53, 53, 53, 53, 53, 11 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_cure_disease,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(501),	20,	12,
	"",			"!Cure Disease!",	"", CMD_BOTH
    },

    {
        "cure light",
        { 53, 53, 3, 6, 53, 53, 53, 53, 53, 53, 53, 53, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_cure_light,	TAR_CHAR_DEFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(16),	10,	12,
	"",			"!Cure Light!",		"", CMD_COMMUNE
    },

    {
        "cure poison",
        { 53, 53, 13, 19, 53, 53, 53, 53, 53, 53, 53, 53, 10 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_cure_poison,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(43),	 5,	12,
	"",			"!Cure Poison!",	"", CMD_BOTH
    },

    {
        "cure serious",
        { 53, 53, 9, 53, 53, 53, 8, 53, 53, 53, 53, 53, 2 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_cure_serious,	TAR_CHAR_DEFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(61),	15,	12,
	"",			"!Cure Serious!",	"", CMD_COMMUNE
    },

    {
        "curse",
        { 53, 53, 20, 53, 17, 53, 53, 53, 53, 16, 53, 53, 14 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_curse,		TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	&gsn_curse,		SLOT(17),	20,	12,
	"curse",		"The curse wears off.",
	"$p is no longer impure.", CMD_BOTH
    },

    {
	"dark wrath",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_dark_wrath,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(550),	20,	12,
	"unholy dark wrath",	"!DarkWrath!", "", CMD_BOTH
    },

    {
        "demonfire",
        { 53, 53, 40, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_demonfire,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(505),	40,	12,
	"torments",		"!Demonfire!",		"", CMD_COMMUNE
    },

    {
        "detect evil",
        { 53, 53, 10, 10, 53, 53, 53, 53, 53, 53, 53, 53, 5 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_detect_evil,	TAR_CHAR_SELF,			POS_STANDING,
	NULL,			SLOT(18),	 5,	12,
	"",			"The red in your vision disappears.",	"", CMD_COMMUNE
    },

    {
        "detect good",
        { 53, 53, 10, 53, 10, 53, 53, 53, 53, 2, 53, 53, 5 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_detect_good,      TAR_CHAR_SELF,  	        POS_STANDING,
        NULL,                   SLOT(513),        5,     12,
        "",                     "The gold in your vision disappears.",	"", CMD_BOTH
    },

    {
        "detect hidden",
        { 53, 15, 53, 53, 53, 53, 53, 53, 16, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	 	TAR_IGNORE,			POS_STANDING,
	&gsn_detect_hidden,	SLOT(0),	 5,	12,
	"",			"You feel less aware of your surroundings.",
	"", CMD_NONE
    },

    {
        "detect invis",
        { 53, 53, 9, 10, 9, 53, 7, 3, 53, 8, 10, 53, 8 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_detect_invis,	TAR_CHAR_SELF,			POS_STANDING,
	NULL,			SLOT(19),	 5,	12,
	"",			"You no longer see invisible objects.",
	"", CMD_BOTH
    },

    {
        "detect magic",
        { 53, 53, 4, 53, 53, 53, 53, 2, 53, 53, 1, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_detect_magic,	TAR_CHAR_SELF,			POS_STANDING,
	NULL,			SLOT(20),	 5,	12,
	"",			"The detect magic wears off.",	"", CMD_BOTH
    },

    {
        "detect poison",
        { 53, 53, 8, 53, 53, 53, 53, 53, 53, 53, 4, 53, 8 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_detect_poison,	TAR_OBJ_INV,			POS_STANDING,
	NULL,			SLOT(21),	 5,	12,
	"",			"!Detect Poison!",	"", CMD_BOTH
    },

    {
        "dispel evil",
        { 53, 53, 15, 7, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_dispel_evil,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(22),	15,	12,
	"dispel evil",		"!Dispel Evil!",	"", CMD_COMMUNE
    },

    {
        "dispel good",
        { 53, 53, 15, 53, 7, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_dispel_good,      TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
        NULL,                   SLOT(512),      15,     12,
        "dispel good",          "!Dispel Good!",	"", CMD_BOTH
    },

    {
        "dispel magic",
        { 53, 53, 20, 53, 35, 53, 53, 17, 53, 25, 23, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_dispel_magic,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(59),	15,	12,
	"",			"!Dispel Magic!",	"", CMD_BOTH
    },

    {
        "earthquake",
        { 53, 53, 15, 53, 53, 53, 7, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_earthquake,	TAR_IGNORE,			POS_FIGHTING,
	NULL,			SLOT(23),	15,	12,
	"earthquake",		"!Earthquake!",		"", CMD_BOTH
    },

    {
        "enchant armor",
        { 53, 53, 53, 53, 53, 53, 53, 17, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_enchant_armor,	TAR_OBJ_INV,			POS_STANDING,
	NULL,			SLOT(510),	100,	24,
	"",			"!Enchant Armor!",	"", CMD_SPELL
    },

    {
        "enchant weapon",
        { 53, 53, 53, 53, 53, 53, 53, 20, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_enchant_weapon,	TAR_OBJ_INV,			POS_STANDING,
	NULL,			SLOT(24),	100,	24,
	"",			"!Enchant Weapon!",	"", CMD_SPELL
    },

    {
        "energy drain",
        { 53, 53, 25, 53, 27, 53, 53, 53, 53, 19, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_energy_drain,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(25),	50,	12,
	"energy drain",		"The cold chill in your body fades away.", "", CMD_BOTH
    },

    {
   	"evil eye",
   	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_evil_eye,		TAR_CHAR_OFFENSIVE,		POS_STANDING,
	NULL,			SLOT(559),	150,	24,
	"deathly gaze",		"!EvilEye!",		"", CMD_NONE
    },

    {
        "faerie fire",
        { 53, 53, 11, 53, 6, 53, 6, 1, 53, 3, 5, 53, 3 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_faerie_fire,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(72),	 5,	12,
	"faerie fire",		"The pink aura around you fades away.",
	"", CMD_BOTH
    },

    {
        "faerie fog",
        { 53, 53, 14, 53, 53, 53, 19, 13, 53, 17, 17, 53, 18 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_faerie_fog,	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(73),	12,	12,
	"faerie fog",		"The fog on your body fades away.", "", CMD_BOTH
    },

    {
	"unholy fire",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_unholy_fire,	TAR_IGNORE,			POS_FIGHTING,
	NULL,			SLOT(700),	50,	12,
	"unholy fire",	"",	"", CMD_SPELL
    },
    {
	"acid spit",
	{ 53, 53, 53, 53, 53, 53, 53, 53 ,53 ,53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_acid_spit,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	&gsn_acid_spit,		SLOT(701),	15,	12,
	"acidic spittle",	"The acid in your eyes washes out.", "", CMD_SPELL
    },

    {
        "fireball",
        { 53, 53, 53, 53, 22, 53, 53, 27, 53, 53, 27, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_fireball,		TAR_IGNORE,			POS_FIGHTING,
	NULL,			SLOT(26),	15,	12,
	"fireball",		"!Fireball!",		"", CMD_SPELL
    },

    {
        "fireproof",
        { 53, 53, 53, 53, 53, 53, 53, 25, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_fireproof,	TAR_OBJ_INV,			POS_STANDING,
	NULL,			SLOT(523),	10,	12,
	"",			"",	"$p's protective aura fades.", CMD_SPELL
    },

    {
        "flamestrike",
        { 53, 53, 53, 15, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_flamestrike,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(65),	20,	12,
	"flamestrike",		"!Flamestrike!",		"", CMD_BOTH
    },

    {
        "fly",
        { 53, 53, 19, 53, 53, 53, 15, 10, 53, 53, 16, 53, 17 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_fly,		TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(56),	10,	18,
	"",			"You slowly float to the ground.",	"", CMD_BOTH
    },

    {
        "frenzy",
        { 53, 53, 25, 32, 53, 53, 21, 53, 53, 53, 53, 53, 22 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_frenzy,           TAR_CHAR_DEFENSIVE,		POS_STANDING,
        NULL,                   SLOT(504),      30,     24,
        "",                     "Your rage ebbs.",	"", CMD_COMMUNE
    },

    {
        "gate",
        { 53, 53, 20, 51, 53, 53, 53, 27, 53, 53, 53, 53, 11 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_gate,		TAR_IGNORE,			POS_FIGHTING,
	NULL,			SLOT(83),	80,	12,
	"",			"!Gate!",		"", CMD_COMMUNE
    },

    {
        "giant strength",
        { 53, 53, 53, 53, 53, 53, 12, 11, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_giant_strength,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(39),	20,	12,
	"",			"You feel weaker.",	"", CMD_BOTH
    },

    {
        "harm",
        { 53, 53, 24, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_harm,		TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(27),	35,	12,
	"harm spell",		"!Harm!",		"", CMD_COMMUNE
    },

    {
        "haste",
        { 53, 53, 53, 53, 53, 53, 53, 21, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_haste,		TAR_CHAR_DEFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(502),	30,	12,
	"",			"You feel yourself slow down.",	"", CMD_SPELL
    },

    {
        "heal",
        { 53, 53, 53, 30, 53, 53, 53, 53, 53, 53, 53, 53, 17 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_heal,		TAR_CHAR_DEFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(28),	50,	12,
	"",			"!Heal!",		"", CMD_COMMUNE
    },

    {
        "heat metal",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_heat_metal,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(516), 	25,	18,
	"spell",		"!Heat Metal!",		"", CMD_SPELL
    },

    {
	"curse weapon",
	{ 53, 53, 53, 53, 28, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_curse_weapon,	TAR_OBJ_INV,			POS_STANDING,
	NULL,			SLOT(771),	75,	12,
	"",	"", "", CMD_SPELL
    },

    {
	"bless weapon",
	{ 53, 53, 53, 41, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_bless_weapon,	TAR_OBJ_INV,			POS_STANDING,
	NULL,			SLOT(621),	150,	24,
	"",	"",	"", CMD_COMMUNE
    },

    {
        "holy word",
        { 53, 53, 35, 40, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_holy_word,	TAR_IGNORE,			POS_FIGHTING,
	NULL,			SLOT(506), 	200,	24,
	"divine wrath",		"!Holy Word!",		"", CMD_COMMUNE
    },

    {
        "identify",
        { 53, 53, 14, 16, 16, 53, 16, 15, 53, 14, 15, 53, 21 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_identify,		TAR_OBJ_INV,			POS_STANDING,
	NULL,			SLOT(53),	12,	18,
	"",			"!Identify!",		"", CMD_BOTH
    },

    {
        "infravision",
        { 53, 53, 10, 53, 53, 53, 7, 9, 53, 6, 8, 53, 10 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_infravision,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(77),	 5,	18,
	"",			"You no longer see in the dark.",	"", CMD_BOTH
    },

    {
        "invisibility",
        { 53, 53, 53, 53, 12, 53, 53, 5, 53, 8, 10, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_invis,		TAR_OBJ_CHAR_DEF,		POS_STANDING,
	&gsn_invis,		SLOT(29),	 5,	12,
	"",			"You are no longer invisible.",
	"$p fades into view.", CMD_SPELL
    },

    {
        "know alignment",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_know_alignment,	TAR_CHAR_DEFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(58),	 9,	12,
	"",			"!Know Alignment!",	"", CMD_BOTH
    },

    {
	"brew",
	{ 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_brew,		TAR_OBJ_INV,			POS_STANDING,
	NULL,			SLOT(598),	35,	12,
	"",	"",	"", CMD_SPELL
    },

    {
        "lightning bolt",
        { 53, 53, 53, 53, 16, 53, 53, 9, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_lightning_bolt,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(30),	15,	12,
	"lightning bolt",	"!Lightning Bolt!",	"", CMD_SPELL
    },

    {
        "locate object",
        { 53, 53, 20, 20, 53, 53, 17, 15, 53, 20, 20, 53, 24 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_locate_object,	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(31),	20,	18,
	"",			"!Locate Object!",	"", CMD_BOTH
    },

    {
        "magic missile",
        { 53, 53, 53, 53, 53, 53, 53, 3, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_magic_missile,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(32),	15,	12,
	"magic missile",	"!Magic Missile!",	"", CMD_SPELL
    },

    {
        "mass healing",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_mass_healing,	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(508),	100,	36,
	"",			"!Mass Healing!",	"", CMD_COMMUNE
    },

    {
        "mass invis",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 30, 53, 53, 22 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_mass_invis,	TAR_IGNORE,			POS_STANDING,
	&gsn_mass_invis,	SLOT(69),	20,	24,
	"",			"You are no longer invisible.",		"", CMD_BOTH
    },

    {
        "pass door",
        { 53, 53, 36, 53, 53, 53, 26, 24, 53, 53, 25, 53, 26 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_pass_door,	TAR_CHAR_SELF,			POS_STANDING,
	NULL,			SLOT(74),	20,	12,
	"",			"You feel solid again.",	"", CMD_BOTH
    },

    {
        "plague",
        { 53, 53, 21, 53, 26, 53, 53, 53, 53, 25, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_plague,		TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	&gsn_plague,		SLOT(503),	20,	12,
	"sickness",		"Your sores vanish.",	"", CMD_BOTH
    },

    {
        "poison",
        { 53, 53, 10, 53, 27, 53, 53, 53, 53, 17, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_poison,		TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	&gsn_poison,		SLOT(33),	10,	12,
	"poison",		"You feel less sick.",
	"The poison on $p dries up.", CMD_BOTH
    },

    {
	"power word kill",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 38, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_power_word_kill,	TAR_CHAR_OFFENSIVE,		POS_STANDING,
	NULL,			SLOT(557),	150,	24,
	"word of power",	"PowerWordKill!",	"", CMD_SPELL
    },

    {
	"power word stun",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_power_word_stun,	TAR_CHAR_OFFENSIVE,		POS_STANDING,
	&gsn_power_word_stun,	SLOT(569),	100,	0,
	"word of power",	"You recover from the shock and stagger about slightly.",	"", CMD_SPELL
    },

    {
        "protection evil",
        { 53, 53, 15, 12, 53, 53, 53, 53, 53, 53, 53, 53, 8 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_protection_evil,	TAR_CHAR_SELF,			POS_STANDING,
	NULL,			SLOT(34), 	5,	12,
	"",			"You feel less protected.",	"", CMD_COMMUNE
    },

    {
        "protection good",
        { 53, 53, 15, 53, 21, 53, 53, 53, 53, 15, 53, 53, 8 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_protection_good,  TAR_CHAR_SELF,          	POS_STANDING,
        NULL,                   SLOT(514),       5,     12,
        "",                     "You feel less protected.",	"", CMD_BOTH
    },

    {
        "ray of truth",
        { 53, 53, 53, 40, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_ray_of_truth,     TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
        NULL,                   SLOT(518),      20,     12,
        "ray of truth",         "!Ray of Truth!",	"", CMD_COMMUNE
    },

    {
        "recharge",
        { 53, 53, 53, 53, 53, 53, 53, 27, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_recharge,		TAR_OBJ_INV,			POS_STANDING,
	NULL,			SLOT(517),	60,	24,
	"",			"!Recharge!",		"", CMD_SPELL
    },

    {
        "refresh",
        { 53, 53, 7, 14, 53, 53, 9, 53, 53, 53, 9, 53, 6 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_refresh,		TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(81),	12,	18,
	"refresh",		"!Refresh!",		"", CMD_BOTH
    },

    {
        "remove curse",
        { 53, 53, 20, 21, 53, 53, 53, 53, 53, 53, 53, 53, 12 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_remove_curse,	TAR_OBJ_CHAR_DEF,		POS_STANDING,
	NULL,			SLOT(35),	 5,	12,
	"",			"!Remove Curse!",	"", CMD_COMMUNE
    },

    {
        "sanctuary",
        { 53, 53, 53, 28, 53, 53, 53, 53, 53, 53, 53, 53, 21 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_sanctuary,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	&gsn_sanctuary,		SLOT(36),	75,	12,
	"",			"The white aura around your body fades.",
	"", CMD_BOTH
    },

    {
        "shield",
        { 53, 53, 53, 53, 53, 27, 53, 53, 53, 53, 25, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_shield,		TAR_CHAR_DEFENSIVE,		POS_STANDING,
	&gsn_shield,			SLOT(67),	12,	12,
	"",			"Your force shield shimmers then fades away.",
	"", CMD_BOTH
    },

    {
        "shocking grasp",
        { 53, 53, 53, 53, 53, 53, 53, 5, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_shocking_grasp,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(53),	15,	12,
	"shocking grasp",	"!Shocking Grasp!",	"", CMD_SPELL
    },

    {
        "sleep",
        { 53, 53, 53, 53, 18, 53, 53, 53, 53, 16, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_sleep,		TAR_CHAR_OFFENSIVE,		POS_STANDING,
	&gsn_sleep,		SLOT(38),	15,	12,
	"",			"You feel less tired.",	"", CMD_SPELL
    },

    {
        "slow",
        { 53, 53, 53, 53, 53, 53, 53, 23, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_slow,             TAR_CHAR_DEFENSIVE,     	POS_FIGHTING,
        NULL,                   SLOT(515),      30,     12,
        "",                     "You feel yourself speed up.",	"", CMD_SPELL
    },

    {
        "stone skin",
        { 53, 53, 53, 53, 53, 53, 53, 15, 53, 53, 26, 53, 20 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_stone_skin,	TAR_CHAR_SELF,			POS_STANDING,
	&gsn_stoneskin,			SLOT(66),	50,	12,
	"",			"Your skin feels soft again.",	"", CMD_BOTH
    },

    {
        "summon",
        { 53, 53, 15, 20, 21, 53, 53, 53, 53, 27, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_summon,		TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(40),	100,	12,
	"",			"!Summon!",		"", CMD_BOTH
    },

    {
        "teleport",
        { 53, 53, 13, 53, 14, 53, 53, 14, 53, 15, 16, 53, 19 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_teleport,		TAR_CHAR_SELF,			POS_FIGHTING,
	NULL,	 		SLOT( 2),	50,	12,
	"",			"!Teleport!",		"", CMD_BOTH
    },

    {
	"turn undead",
	{ 53, 53, 53, 21, 53, 53, 53, 53, 53, 53, 53, 53, 28 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_turn_undead,	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(552),	50,	24,
	"turning of undead",	"!TurnUndead!",		"", CMD_COMMUNE
    },

    {
	"undead drain",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_undead_drain,	TAR_CHAR_OFFENSIVE, 		POS_FIGHTING,
	&gsn_undead_drain, 	SLOT(809),	60,	12,
	"undead drain",	"You feel your body and mind restored to normal.",
	"", CMD_SPELL
    },

    {
        "ventriloquate",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_ventriloquate,	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(41),	 5,	12,
	"",			"!Ventriloquate!",	"", CMD_BOTH
    },

    {
	"team spirit",
	{ 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_team_spirit, 	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(703),	5,	12,
	"energy transference",	"",	"", CMD_SPELL
    },

    {
        "venueport",
        { 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_venueport,	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(558),	 150,	12,
	"",		"You are once again ready to venueport.",	"", CMD_SPELL
    },

    {
        "weaken",
        { 53, 53, 8, 53, 27, 53, 53, 53, 53, 29, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_weaken,		TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(68),	20,	12,
	"spell",		"You feel stronger.",	"", CMD_BOTH
    },

    {
	"web",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_web,		TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(560),	20,	12,
	"corrosive webs",
	"The sticky strands about your body dissolve.",	"", CMD_BOTH
    },

    {
        "word of recall",
        { 53, 53, 28, 31, 53, 53, 34, 28, 53, 33, 30, 53, 27 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_word_of_recall,	TAR_CHAR_DEFENSIVE, 		POS_FIGHTING,
	NULL,			SLOT(42),	 5,	12,
	"",			"!Word of Recall!",	"", CMD_BOTH
    },


    {
        "summon fire elemental",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
        spell_summon_fire_elemental,    TAR_IGNORE,     	POS_STANDING,
        NULL,           	SLOT(572),      50,     12,
        "",     "You feel up to calling another fire elemental.", "", CMD_SPELL
    },

    {
        "summon earth elemental",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
        spell_summon_earth_elemental,    TAR_IGNORE,	     	POS_STANDING,
        NULL,           	SLOT(573),      50,     12,
        "",     "You feel up to calling another earth elemental.", "", CMD_SPELL
    },

    {
        "summon air elemental",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
        spell_summon_air_elemental,    TAR_IGNORE,	     	POS_STANDING,
        NULL,           	SLOT(574),      50,     12,
        "",     "You feel up to calling another air elemental.", "", CMD_SPELL
    },

    {
        "summon water elemental",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 ,0 },
        spell_summon_water_elemental,    TAR_IGNORE,		POS_STANDING,
        NULL,           	SLOT(575),      50,     12,
        "",     "You feel up to calling another water elemental.", "", CMD_SPELL
    },

    {
        "tsunami",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
        spell_tsunami,    	TAR_IGNORE,     		POS_FIGHTING,
        NULL,           	SLOT(576),      30,     12,
        "raging tidal wave",     "!Tsunami!", "", CMD_SPELL
    },

    {
        "earthmaw",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 36, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
        spell_earthmaw,		TAR_CHAR_OFFENSIVE,     	POS_FIGHTING,
        NULL,           	SLOT(577),      30,     24,
        "earthmaw",     "!earthmaw!", "", CMD_SPELL
    },

    {
        "windwall",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 22, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0 },
        spell_windwall,    	TAR_IGNORE,     		POS_FIGHTING,
        NULL,           	SLOT(578),      20,     24,
        "air blast",     "You rub the dirt and dust from your eyes.", "", CMD_SPELL
    },

    {
        "drain",
        { 53, 53, 53, 53, 53, 53, 53, 15, 53, 53, 53, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
        spell_drain,    	TAR_OBJ_INV,     		POS_STANDING,
        NULL,           	SLOT(579),      30,     12,
        "",     "!Drain!", "", CMD_SPELL
    },

    {
        "disenchant armor",
        { 53, 53, 53, 53, 53, 53, 53, 16, 53, 53, 53, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
        spell_disenchant_armor,	TAR_OBJ_INV,			POS_STANDING,
        NULL,           	SLOT(580),      50,     36,
        "",     		"!Disenchant!", "", CMD_SPELL
    },

    {
        "disenchant weapon",
        { 53, 53, 53, 53, 53, 53, 53, 21, 53, 53, 53, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
        spell_disenchant_weapon,TAR_OBJ_INV,			POS_STANDING,
        NULL,           	SLOT(581),      50,     36,
        "",     		"!Disenchant!", "", CMD_SPELL
    },

    {
        "cone of cold",
        { 53, 53, 53, 53, 53, 53, 53, 32, 53, 53, 53, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 1, 0 ,0, 0, 0, 0 },
        spell_cone_of_cold,     TAR_IGNORE,     		POS_FIGHTING,
        NULL,                   SLOT(585),      30,     12,
        "blast of cold",          "","", CMD_SPELL
    },

    {
        "protective shield",
        { 53, 53, 16, 53, 53, 53, 53, 16, 53, 53, 18, 53, 13 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_protective_shield,TAR_CHAR_SELF,     		POS_STANDING,
        &gsn_protective_shield, SLOT(571),      75,     12,
        "",          "Your protective shield fades away.","", CMD_BOTH
    },

    {
        "timestop",
        { 53, 53, 53, 53, 53, 53, 53, 40, 53, 53, 53, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 1, 0 ,0, 0, 0, 0 },
        spell_timestop,		TAR_IGNORE,     		POS_STANDING,
        &gsn_timestop,          SLOT(586),      100,     48,
        "",          "Everything around you starts moving fast again.","", CMD_SPELL
    },

    {
        "timestop spell",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 1, 0 ,0, 0, 0, 0 },
        spell_timestop,      	TAR_IGNORE,			POS_STANDING,
        &gsn_timestop_done,     SLOT(0),	100,     48,
        "",          "You are once again able to interfere with time.","", CMD_NONE
    },

    {
        "consecrate",
        { 53, 53, 53, 38, 53, 53, 53, 53, 53, 53, 53, 53, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1 },
        spell_consecrate,      TAR_IGNORE,			POS_STANDING,
        &gsn_consecrate,                   SLOT(589),      90,     60,
        "",          "You feel holy enough to consecrate more ground.","", CMD_COMMUNE
    },

    {
        "wither",
        { 53, 53, 45, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_wither,		TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
        NULL,                   SLOT(590),      40,     12,
        "withering",          "Your skin begins to heal.","", CMD_COMMUNE
    },

    {
        "shadowstrike",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
        spell_shadowstrike,     TAR_CHAR_OFFENSIVE,		POS_STANDING,
        &gsn_shadowstrike,      SLOT(563),	100,	12,
        "",
        "The shadowy arms grasping you fade away in a smokey haze.","", CMD_SPELL
    },

    {
	"dark dream",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_SLEEPING,
	&gsn_dark_dream,	SLOT( 0),	0,	0,
	"",	"",	"", CMD_SPELL
    },

    {
	"shadowself",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_shadowself,	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(619),	20,	12,
	"",	"",	"", CMD_SPELL
    },

    {

        "nightwalk",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_nightwalk,	TAR_IGNORE,			POS_STANDING,
        NULL,			SLOT(561),      75,     12,
	"", "", "", CMD_SPELL
    },

    {
        "lightshield",
        { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_lightshield,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(813),	20,	18,
	"",			"The glow of purity surrounding you fades.", "", CMD_POWER
    },

    {
	"wrath of purity",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_wrath_of_purity,	TAR_IGNORE,		POS_FIGHTING,
	NULL,			SLOT(751),	30,	12,
	"wrath of purity",	"",	"", CMD_COMMUNE
	},

    {
	"fire and ice",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 47, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_fire_and_ice,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL, 			SLOT(810),	16,	24,
	"fire and ice",	"",	"", CMD_SPELL
    },

    {
	"utter heal",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_utter_heal,	TAR_CHAR_DEFENSIVE, 		POS_STANDING,
	NULL, 			SLOT(811),	75,	12,
	"", "", "", CMD_COMMUNE
    },

    {
	"atrophy",
	{ 53, 53, 38, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_atrophy, 		TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	&gsn_atrophy, 		SLOT(812),	25,	12,
	"atrophy",	"Your body ceases wasting away.",	"", CMD_COMMUNE
    },


/*
 * Dragon breath
 */
    {
        "acid breath",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_acid_breath,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(200),	100,	24,
	"blast of acid",	"!Acid Breath!",	"", CMD_SPELL
    },

    {
        "fire breath",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_fire_breath,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(201),	200,	24,
	"blast of flame",	"The smoke leaves your eyes.",	"", CMD_SPELL
    },

    {
        "frost breath",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_frost_breath,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(202),	125,	24,
	"blast of frost",	"!Frost Breath!",	"", CMD_SPELL
    },

    {
        "gas breath",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_gas_breath,	TAR_IGNORE,			POS_FIGHTING,
	NULL,			SLOT(203),	175,	24,
	"blast of gas",		"!Gas Breath!",		"", CMD_SPELL
    },

    {
        "lightning breath",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_lightning_breath,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(204),	150,	24,
	"blast of lightning",	"!Lightning Breath!",	"", CMD_SPELL
    },

    {
        "hold person",
        { 53, 53, 53, 53, 53, 53, 53, 37, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_hold_person,      TAR_CHAR_OFFENSIVE,   		POS_FIGHTING,
        &gsn_hold_person,       SLOT(592),       100,     24,
        "",          "The stiffness in your body wears off.","", CMD_SPELL
    },



/* combat and weapons skills */


    {
        "axe",
        { 1, 53, 1, 53, 1, 1, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_axe,            	SLOT( 0),       0,      0,
        "",                     "!Axe!",		"", CMD_NONE
    },

    {
        "dagger",
        { 1, 1, 53, 53, 1, 1, 53, 1, 1, 1, 1, 1, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_dagger,            SLOT( 0),       0,      0,
        "",                     "!Dagger!",		"", CMD_NONE
    },

    {
        "flail",
        { 1, 53, 1, 1, 1, 53, 53, 53, 53, 1, 1, 53, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_flail,            	SLOT( 0),       0,      0,
        "",                     "!Flail!",		"", CMD_NONE
    },
        {
        "flail arms",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null,     	TAR_IGNORE,     		POS_DEAD,
        &gsn_flail_arms,        SLOT(0),	0,      0,
        "flailing bone arms",   "",     "", CMD_NONE
        },

    {
	"mind sear",
	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	NULL,			SLOT(0),	0,	0,
	"mind sear",	"The burning in your mind fades away.",	"", CMD_NONE
    },

    {
        "mace",
        { 1, 1, 53, 1, 1, 1, 1, 53, 53, 53, 53, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_mace,            	SLOT( 0),       0,      0,
        "",                     "!Mace!",		"", CMD_NONE
    },

    {
        "polearm",
        { 1, 53, 1, 1, 1, 1, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_polearm,           SLOT( 0),       0,      0,
        "",                     "!Polearm!",		"", CMD_NONE
    },

    {
        "shield block",
        { 10, 14, 18, 11, 11, 12, 53, 53, 15, 53, 53, 16, 17 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_shield_block,	SLOT(0),	0,	0,
	"",			"!Shield!",		"", CMD_NONE
    },

    {
        "spear",
        { 1, 1, 53, 53, 1, 1, 53, 53, 1, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_spear,            	SLOT( 0),       0,      0,
        "",                     "!Spear!",		"", CMD_NONE
    },

    {
	"dual wield",
	{ 12, 25, 53, 53, 18, 16, 53, 53, 17, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_dual_wield,	SLOT( 0),	0,	0,
	"", "", "", CMD_NONE
    },

    {
        "sword",
        { 1, 1, 1, 1, 1, 1, 1, 53, 1, 1, 1, 1, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_sword,            	SLOT( 0),       0,      0,
        "",                     "!sword!",		"", CMD_NONE
    },

    {
        "whip",
        { 1, 1, 53, 53, 1, 1, 1, 1, 1, 1, 1, 53, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_whip,            	SLOT( 0),       0,      0,
        "",                     "!Whip!",	"", CMD_NONE
    },

    {
	"outfit",	    { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_STANDING,
	NULL,	0,	0,	24,
	"outfit",		"",	"", CMD_NONE
    },

    {
	"carve",
	{ 53, 53, 53, 53, 53, 30, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_carve,	SLOT(0),	0,	0,
	"",	"!Ranger Staff!",	"", CMD_NONE
    },

    {
	"ranger staff",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_ranger_staff,	SLOT(0),	0,	0,
	"",	"!Ranger Staff!",	"", CMD_NONE
    },
     {
        "armbreak",
        {61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_DEAD,
        &gsn_armbreak,           SLOT(0),       0,      0,
        "powerful blow",     "You feel as though your arm as mended.",  "", CMD_NONE
    },
     {
        "legbreak",
        {61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_DEAD,
        &gsn_legbreak,           SLOT( 0),       0,      0,
        "darting kick",     "The tendons in your knee seem to have healed.",     "", CMD_NONE
    },

    {
	"preparation",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 51, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_asscounter,		SLOT( 0),	0,	0,
	"",	"Your concentration falters.",	"", CMD_NONE
    },

    {
        "counter",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_FIGHTING,
        &gsn_counter,                SLOT( 0),       0,      0,
	"",	"",	"", CMD_NONE
    },
    {
        "tame",
        { 53, 53, 53, 53, 53, 25, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_tame,      	SLOT(0),        0,      0,
        "",     "", CMD_NONE
    },

    {
        "find water",
        { 53, 53, 53, 53, 53, 18, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_STANDING,
        &gsn_find_water,        SLOT(0),        0,      0,
        "",      "", CMD_NONE
     },

     {
        "shield cleave",
        { 18, 53, 53, 53, 22, 26, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_shield_cleave, 	SLOT(0),    0,      0,
        "",     "", CMD_NONE
     },

    {
        "spellcraft",
        { 53, 53, 53, 53, 53, 53, 53, 1, 53, 1, 1, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_SLEEPING,
        &gsn_spellcraft,        SLOT(0),        0,      0,
        "",     "", "", CMD_NONE
    },

    {
        "power word fear",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_power_word_fear,  TAR_CHAR_OFFENSIVE, 		POS_FIGHTING,
        NULL,   		SLOT(801),	20, 	6,
        "", "You feel the fear plaguing your mind fade away.",
        "", CMD_SPELL
    },

    {
        "decay corpse",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 15, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_decay_corpse, 	TAR_IGNORE, 			POS_STANDING,
        NULL,   		SLOT(804),	15, 	0,
        "", "", "", CMD_SPELL
    },

    {
        "preserve",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 20, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_preserve, 	TAR_OBJ_INV,    		POS_STANDING,
        NULL,   		SLOT(805),	10, 	0,
        "", "", "", CMD_SPELL
    },

    {
        "imbue regeneration",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_regeneration, 	TAR_CHAR_DEFENSIVE, 		POS_STANDING,
        NULL,   		SLOT(806),  	50, 	12,
        "", "Your body stops regenerating.",    "", CMD_COMMUNE
    },

    {
        "restoration",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 19 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_restoration,  	TAR_CHAR_DEFENSIVE, 		POS_STANDING,
        NULL,   		SLOT(807),  	50, 	12,
        "", "", "", CMD_COMMUNE
    },


    {
	"quiet movement",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE, 			POS_STANDING,
	&gsn_quiet_movement, 	SLOT(0), 	0, 	0,
	"", "You stop moving quietly.", "", CMD_NONE
    },
    {
	"iron resolve",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE, 			POS_STANDING,
	&gsn_iron_resolve, 	SLOT(0), 	0, 	0,
	"", "Your mind wanders from its resolve.", "", CMD_NONE
    },

    {
	"ambush",
	{ 53, 53, 53, 53, 53, 12, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_ambush,		SLOT( 0),	0,	24,
	"surprise attack",	"!Ambush!",	"", CMD_NONE
    },

    {
        "bearcharge",
        { 53, 53, 53, 53, 53, 12, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_STANDING,
        &gsn_bearcharge,        SLOT( 0),       0,      24,
        "bearcharge",      	"!Bearcharge!",     "", CMD_NONE
    },

    {
	"animate skeleton",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 45, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_animate_skeleton,	TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(779),	50,	12,
	"",	"You feel your power over the dead return.",	"", CMD_SPELL
    },

    {
	"mummify",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 45, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_mummify,		TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(780),	150,	24,
	"",	"You feel you power over the dead return.",	"", CMD_SPELL
    },

    {
	"moving ambush",
	{ 53, 53, 53, 53, 53, 35, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_moving_ambush,	SLOT( 0),	0,	0,
	"surprise attack",	"!Ambush!",	"", CMD_NONE
    },


        {
        "pugil",
        { 22, 53, 53, 26, 53, 24, 20, 20, 25, 53, 53, 19, 18 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_pugil,     	SLOT(0),        0,      12,
        "pugil",        "",     "", CMD_NONE
        },

        {
        "lash",
        { 15, 13, 24, 53, 53, 19, 25, 20, 18, 25, 25, 53, 19 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_lash,      	SLOT(0),        0,      12,
        "lash", "",     "", CMD_NONE
        },

        {
        "protection heat cold",
        { 53, 53, 53, 53, 53, 18, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_protection_heat_cold,	SLOT(0),0,      12,
        "",     "You no longer feel protected from the elements.", "", CMD_NONE
        },

        {
        "evaluation",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_evaluation,        SLOT(0),        0,      0,
        "",     "",     "", CMD_NONE
        },

    {
        "enhanced damage",
        { 1, 20, 29, 1, 1, 15, 53, 53, 16, 53, 53, 25, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1 },
        spell_null,             TAR_IGNORE,            		POS_FIGHTING,
        &gsn_enhanced_damage,   SLOT( 0),        0,     0,
        "",                     "!Enhanced Damage!",	"", CMD_NONE
    },


        {
        "enhanced damage II",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_enhanced_damage_two,       SLOT(0),        0,      0,
        "",     "",     "", CMD_NONE
        },

        {
        "staff",
        { 1, 53, 53, 1, 53, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_staff,     	SLOT(0),        0,      0,
        "",     "",                "", CMD_NONE
        },

        {
        "tactics",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_tactics,   	SLOT(0),        0,      0,
        "",     "",     "", CMD_NONE
        },

    {
        "backstab",
        { 53, 1, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null,             TAR_IGNORE,             	POS_STANDING,
        &gsn_backstab,          SLOT( 0),        0,     24,
        "backstab",             "!Backstab!",		"", CMD_NONE
    },

    {
	"strange form",
	{ 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60 },
	{ 0, 0, 0, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_strange_form,	SLOT( 0),	0,	0,
	"",	"You feel your normal form returning.",
	"You feel your normal form returning.", CMD_NONE
    },

    {
        "bite",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null, 		TAR_IGNORE, 			POS_FIGHTING,
        &gsn_chimera_lion,  	SLOT( 0),   	0,  	0,
        "crushing bite",    "", "", CMD_NONE
    },

    {
        "ram",
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null, 		TAR_IGNORE, 			POS_FIGHTING,
        &gsn_chimera_goat,  	SLOT( 0),   	0,  	0,
        "ramming",  "", "", CMD_NONE
    },


    {
	"tail",
	{ 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_tail,		SLOT( 0),	0,	12,
	"tail attack",	"", "", CMD_NONE
    },

    {
	"throw",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 15, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_throw,		SLOT( 0),	0,	12,
	"throw",	"",	"", CMD_NONE
    },

    {
	"endure",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 25, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_endure,		SLOT(0),	0,	12,
	"",	"You feel more susceptible to magic.",	"", CMD_NONE
    },
    {
	"palm",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_palm,		SLOT(0),	0,	12,
	"palm",	"You stop concentrating on fighting open handed.",	"", CMD_NONE
    },

    {
	"poisondust",
	{ 53, 53, 53, 53, 53, 53,53, 53, 18, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_poison_dust,	SLOT( 0),	0,	0,
	"poison",	"The poison in your blood is neutralised.", "", CMD_NONE
    },

    {
	"blindness dust",
	{ 53, 53, 53, 53, 53, 53,533, 53, 17, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_blindness_dust,	SLOT( 0),	0,	0,
	"",	"You rub the dust in your eyes away.", "", CMD_NONE
    },

    {
	"roll",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 20, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_roll,		SLOT( 0),	0,	0,
	"",	"",	"", CMD_NONE
    },

    {
	"block",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_block,		SLOT( 0),	0,	0,
	"", "", "", CMD_NONE
    },

    {
	"garrotte",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 25, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE, 			POS_STANDING,
	&gsn_garrotte,		SLOT( 0),	0,	0,
	"garrotte",		"Your throat feels better.", "", CMD_NONE
    },

    {
	"garrotte timer",
	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_garrotte_timer,	0,	0, 	0,
	"",	"You stop guarding your neck so carefully.",	"", CMD_NONE
    },

    {
	"nerve",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 19, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_nerve,		SLOT( 0),	0,	0,
	"nerve",	"Your arm loses the numbness within it.",	"", CMD_NONE
    },

    {
	"follow through",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 45, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_follow_through,	SLOT( 0),	0,	0,
	"follow kick",	"",	"", CMD_NONE
    },


    {
        "bash",
        { 12, 53, 53, 53, 15, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_bash,            	SLOT( 0),       0,      24,
        "bash",                 "!Bash!",		"", CMD_NONE
    },


    {
        "berserk",
        { 18, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_berserk,        	SLOT( 0),       0,      24,
        "",                     "You feel your pulse slow down.",	"", CMD_NONE
    },

    {
	"crush",
	{ 16, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_crush,		SLOT( 0),	0,	12,
	"body crush",	"",	"", CMD_NONE
    },

    {
	"ground control",
	{ 29, 53, 53, 53, 53, 53, 53, 53, 27, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_ground_control,	SLOT( 0),	0,	0,
	"ground control",	"",	"", CMD_NONE
    },

    {
        "circle stab",
        { 53, 14, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_circle,            SLOT( 0),       0,      18,
        "circle stab",                 "!Circlestab!",		"", CMD_NONE
    },


    {
	"spellbane",
	{ 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_spellbane,		SLOT( 0),	0,	0,
	"spellbane",	"Your aura of hatred recedes and you no longer reflect spells.",	"", CMD_POWER
    },

    {
	"warcry",
	{ 16, 53, 20, 53, 53, 20, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_warcry,		SLOT(0),	0,	0,
	"",	"The effects of your warcry die out.",	"", CMD_NONE
    },

    {
    	"deathblow",
    	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
    	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    	spell_null,            TAR_IGNORE,                      POS_FIGHTING,
    	&gsn_dblow,            SLOT( 0),        0,      0,
    	"",                    "!deathblow!",   "", CMD_POWER
    },

    {
        "dirt kicking",
        { 4, 3, 53, 53, 4, 5, 4, 53, 4, 53, 53, 2, 53 },
        { 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_dirt,		SLOT( 0),	0,	24,
	"kicked dirt",		"You rub the dirt out of your eyes.",	"", CMD_NONE
    },

    {
        "disarm",
        { 15, 17, 53, 16, 16, 17, 53, 53, 16, 53, 53, 18, 53 },
        { 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_disarm,            SLOT( 0),        0,     24,
        "",                     "!Disarm!",		"", CMD_NONE
    },

    {
        "dodge",
        { 13, 1, 53, 53, 22, 14, 53, 1, 1, 53, 5, 3, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_dodge,             SLOT( 0),        0,     0,
        "",                     "!Dodge!",		"", CMD_NONE
    },

    {
	"dual backstab",
	{ 53, 32, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_dual_backstab,	SLOT( 0),	0,	0,
	"second backstab",	"!DualBackstab!",	"", CMD_NONE
    },


    {
        "envenom",
        { 53, 10, 53, 53, 53, 53, 53, 53, 21, 53, 53, 53, 53 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 },
	spell_null,		TAR_IGNORE,	  		POS_RESTING,
	&gsn_envenom,		SLOT(0),	0,	36,
	"",			"!Envenom!",		"", CMD_NONE
    },

    {
        "hand to hand",
        { 5, 7, 11, 6, 6, 9, 3, 25, 9, 12, 5, 6, 13 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_hand_to_hand,	SLOT( 0),	0,	0,
	"",			"!Hand to Hand!",	"", CMD_NONE
    },

    {
        "kick",
        { 8, 14, 12, 53, 10, 11, 6, 53, 13, 18, 53, 7, 15 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,     	POS_FIGHTING,
        &gsn_kick,              SLOT( 0),        0,     12,
        "kick",                 "!Kick!",		"", CMD_NONE
    },

    {
        "crescent kick",
        { 53, 53, 53, 53, 53, 53, 53, 53, 44, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,     	POS_FIGHTING,
        &gsn_crescent_kick,              SLOT( 0),        0,     12,
        "crescent kick",                 "!Kick!",		"", CMD_NONE
    },

    {
        "owaza",
        { 53, 53, 53, 53, 53, 53, 53, 53, 42, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,     	POS_FIGHTING,
        &gsn_owaza,              SLOT( 0),        0,     12,
        "owaza",                 "",		"", CMD_NONE
    },

    {
    	"shuto strike",
    	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
    	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    	spell_null,		TAR_IGNORE,			POS_FIGHTING,
    	&gsn_shuto_strike,		SLOT(0),	0,	0,
    	"shuto strike",		"",	"",	CMD_NONE
    },

    {
    	"seiken strike",
    	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
    	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    	spell_null,		TAR_IGNORE,			POS_FIGHTING,
    	&gsn_seiken_strike,		SLOT(0),	0,	0,
    	"seiken strike",		"",	"",	CMD_NONE
    },

    {
    	"sukuinage",
    	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
    	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    	spell_null,		TAR_IGNORE,			POS_FIGHTING,
    	&gsn_sukuinage,		SLOT(0),	0,	0,
    	"sukuinage",		"",	"",	CMD_NONE
    },

	{
        "axe kick",
        { 53, 53, 53, 53, 53, 53, 53, 53, 32, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,     	POS_FIGHTING,
        &gsn_axe_kick,              SLOT( 0),        0,     12,
        "axe kick",                 "The pain in your shoulder subsides.",		"", CMD_NONE
    },

    {
        "backfist",
        { 53, 53, 53, 53, 53, 53, 53, 53, 47, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,     	POS_FIGHTING,
        &gsn_backfist,              SLOT( 0),        0,     12,
        "backfist",                 "",		"", CMD_NONE
    },

    {
        "heightened awareness",
        { 53, 53, 53, 53, 53, 53, 53, 53, 11, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,     	POS_FIGHTING,
        &gsn_heightened_awareness,              SLOT( 0),        0,     12,
        "",                 "Your senses seem duller.",		"", CMD_NONE
    },

		{
        "unarmed defense",
        { 53, 53, 53, 53, 53, 53, 53, 53, 14, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,     	POS_FIGHTING,
        &gsn_unarmed_defense,              SLOT( 0),        0,     12,
        "",                 "",		"", CMD_NONE
    },

		{
        "unarmed offense",
        { 53, 53, 53, 53, 53, 53, 53, 53, 14, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,     	POS_FIGHTING,
        &gsn_unarmed_offense,              SLOT( 0),        0,     12,
        "",                 "",		"", CMD_NONE
    },
	
	{
		"tigerclaw",
        { 53, 53, 53, 53, 53, 53, 53, 53, 42, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,     	POS_FIGHTING,
        &gsn_tigerclaw,              SLOT( 0),        0,     12,
        "tigerclaw",                 "Your throat feels less constricted.",		"", CMD_NONE
    },

    {
        "bindwounds",
        { 53, 53, 53, 53, 53, 53, 53, 53, 20, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,     	POS_FIGHTING,
        &gsn_bindwounds,              SLOT( 0),        0,     12,
        "",                 "You feel ready to bind some more wounds.",		"", CMD_NONE
    },

	{
        "scissor kick",
        { 53, 53, 53, 53, 53, 53, 53, 53, 14, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,     	POS_FIGHTING,
        &gsn_scissor_kick,              SLOT( 0),        0,     12,
        "scissor kick",                 "!Kick!",		"", CMD_NONE
    },

		{
        "parry",
        { 1, 11, 7, 1, 1, 1, 20, 53, 16, 18, 53, 13, 21 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_parry,             SLOT( 0),        0,     0,
        "",                     "!Parry!",		"", CMD_NONE
    },

    {
        "rescue",
        { 1, 53, 53, 5, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_rescue,            SLOT( 0),        0,     12,
        "",                     "!Rescue!",		"", CMD_NONE
    },

    {
	"downstrike",
	{ 53, 28, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_downstrike,	SLOT(0),	0,	0,
	"downstrike",	"",	"", CMD_NONE
    },

    {
        "trip",
        { 9, 5, 53, 53, 13, 11, 53, 53, 11, 53, 53, 4, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_trip,		SLOT( 0),	0,	24,
	"trip",			"!Trip!",		"", CMD_NONE
    },

    {
        "second attack",
        { 5, 12, 24, 7, 7, 8, 7, 30, 6, 21, 26, 15, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_second_attack,     SLOT( 0),        0,     0,
        "",                     "!Second Attack!",	"", CMD_NONE
    },

    {
        "third attack",
        { 19, 25, 53, 25, 24, 25, 25, 53, 23, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_third_attack,      SLOT( 0),        0,     0,
        "",                     "!Third Attack!",	"", CMD_NONE
    },

    {
        "fourth attack",
        { 30, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_fourth_attack,     SLOT( 0),        0,     0,
        "",                     "!Fourth Attack!",	"", CMD_NONE
    },


/* non-combat skills */

    {
	"acute vision",
	{ 53, 53, 53, 53, 53, 11, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_acute_vision,	SLOT( 0),	0,	12,
	"",			"Your sight dulls as your awareness fades.",	"", CMD_NONE

    },

    {
	"barkskin",
	{ 53, 53, 53, 53, 53, 18, 53 ,53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_barkskin,		SLOT( 0),	0,	12,
	"",	"The bark on your skin peels off and falls away.", "", CMD_NONE
    },

    {
	"enlist",
	{35, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_enlist,		SLOT( 0),	0,	0,
	"",	"You decide to follow your own destiny and leave your company.", "", CMD_NONE
    },

    {
	"animal call",
	{ 53, 53, 53, 53, 53, 20, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_animal_call,	SLOT( 0),	0,	36,
	"", "You feel up to calling more animals.", "", CMD_NONE
    },

    {
	"bear call",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_bear_call,		SLOT( 0),	0,	12,
	"", "You feel up to calling more bears.", "", CMD_NONE
    },

    {
	"camouflage",
	{ 53, 53, 53, 53, 53, 8, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_camouflage,	SLOT( 0),	0,	12,
	"",			"!Camouflage!",		"", CMD_NONE
    },

    {
	"camp",
	{ 53, 53, 53, 53, 53, 20, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_RESTING,
	&gsn_camp,		SLOT( 0),	0,	48,
	"",			"!Camp!",		"", CMD_NONE
    },

    {
        "fast healing",
        { 7, 12, 9, 14, 14, 15, 8, 15, 13, 14, 8, 10, 10 },
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	spell_null,		TAR_IGNORE,			POS_SLEEPING,
	&gsn_fast_healing,	SLOT( 0),	0,	0,
	"",			"!Fast Healing!",	"", CMD_NONE
    },

    {
        "haggle",
        { 14, 8, 11, 53, 53, 14, 53, 8, 9, 10, 10, 9 },
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	spell_null,		TAR_IGNORE,			POS_RESTING,
	&gsn_haggle,		SLOT( 0),	0,	0,
	"",			"!Haggle!",		"", CMD_NONE
    },

    {
        "hide",
        { 53, 1, 53, 53, 53, 53, 53, 53, 5, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_RESTING,
	&gsn_hide,		SLOT( 0),	 0,	12,
	"",			"!Hide!",		"", CMD_NONE
    },

    {
	"laying hands",
	{ 53, 53, 53, 35, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_laying_hands,	SLOT( 0),	35, 	36,
	"",			"You feel able to heal with your touch again.",
	"", CMD_NONE
    },

    {
        "meditation",
        { 16, 17, 6, 8, 8, 13, 2, 5, 12, 11, 7, 12, 6 },
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	spell_null,		TAR_IGNORE,			POS_SLEEPING,
	&gsn_meditation,	SLOT( 0),	0,	0,
	"",			"Meditation",		"", CMD_NONE
    },

    {
	"trance",
	{ 53, 53, 15, 53, 53, 53, 14, 14, 53, 14, 14, 53, 16 },
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	spell_null,		TAR_IGNORE,			POS_SLEEPING,
	&gsn_trance,		SLOT( 0),	0,	0,
	"",	"",	"", CMD_NONE
    },

    {
        "lunge",
        { 14, 53, 53, 53, 24, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_lunge,      	SLOT( 0),       0,      24,
        "lunge",        "","", CMD_NONE
    },

    {
        "black guard",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_SLEEPING,
        &gsn_blackjack_timer,   SLOT(0),	0,  0,
        "", "You stop guarding your head so carefully.",    "", CMD_NONE
    },

    {
		"head guard",
		{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		spell_null,			TAR_IGNORE,				POS_SLEEPING,
		&gsn_noheadclap,		SLOT(0),	0,	0,
		"",	"You let down your head guard.",			"",		CMD_NONE
	},

    {
        "blackjack",
        { 53, 21, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_STANDING,
        &gsn_blackjack,  	SLOT( 0),       0,      24,
        "blackjack",    "You come to with a pounding headache.", "", CMD_NONE
    },

    {
        "guard call",
        { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_guard_call,       TAR_IGNORE,     		POS_STANDING,
        NULL,   		SLOT(611),      35,     12,
        "", "You can commandeer a guard contingent once again.", "", CMD_POWER
    },

    {
        "peek",
        { 53, 6, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_peek,		SLOT( 0),	 0,	 0,
	"",			"!Peek!",		"", CMD_NONE
    },

    {
        "pick lock",
        { 53, 11, 53, 53, 53, 53, 53, 53, 13, 53, 53, 9, 53 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_pick_lock,		SLOT( 0),	 0,	12,
	"",			"!Pick!",		"", CMD_NONE
    },

    {
	"skin",
	{ 53, 53, 53, 53, 53, 22, 53, 53, 53, 53, 53, 53, 53 },
	{ 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_skin,		SLOT( 0),	0,	24,
	"",
	"You feel up to skinning and fashioning another sack.", "", CMD_NONE
    },

    {
	"butcher",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_butcher,		SLOT( 0),	0,	12,
	"",	"",	"", CMD_NONE
    },


    {
        "sneak",
        { 53, 4, 53, 53, 53, 53, 53, 53, 6, 53, 53, 30, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_sneak,		SLOT( 0),	 0,	12,
	"",			"You no longer feel stealthy.",	"", CMD_NONE
    },

    {
        "steal",
        { 53, 6, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_steal,		SLOT( 0),	 0,	24,
	"",			"!Steal!",		"", CMD_NONE
    },

    {
	"resistance",
	{ 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_resistance,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_resistance,		SLOT( 0),		60,	12,
	"",
	"You feel less resistant as your skin softens.",
	"", CMD_POWER
    },

    {
        "scrolls",
        { 53, 1, 53, 53, 1, 1, 1, 1, 53, 1, 1, 1, 53 },
        { 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_scrolls,		SLOT( 0),	0,	24,
	"",			"!Scrolls!",		"", CMD_NONE
    },

    {
        "staves",
        { 53, 1, 53, 53, 1, 1, 1, 1, 53, 1, 1, 1, 53 },
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_staves,		SLOT( 0),	0,	12,
	"",			"!Staves!",		"", CMD_NONE
    },

    {
        "wands",
        { 53, 53, 53, 53, 1, 53, 53, 1, 53, 1, 1, 53, 53 },
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_wands,		SLOT( 0),	0,	12,
	"",			"!Wands!",		"", CMD_NONE
    },

    {
        "recall",
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_recall,		SLOT( 0),	0,	12,
	"",			"You are able to recall again.","", CMD_POWER
    },

    {
	"channel",
	{ 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_channel,		TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(555),	100,	24,
	"",
	"Your mind loses its mental strength and you feel less healthy.", "", CMD_POWER
    },

    {
        "life transfer",
        { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_life_transfer, 	TAR_CHAR_DEFENSIVE,        	POS_STANDING,
        NULL,   		SLOT(609),      10,     12,
        "",     "","", CMD_SPELL
    },

    {
	"safety",
	{ 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_safety,		TAR_IGNORE,			POS_FIGHTING,
	NULL,			SLOT(690), 	75, 	24,
	"",	"",	"", CMD_POWER
    },

    {
	"badge",
	{ 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_badge,		TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(640),	75,	12,
	"",	"",	"", CMD_POWER
    },

    {
	"protectorate plate",
	{ 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_protectorate_plate,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(631),	75,	12,
	"",	"",	"", CMD_POWER
    },

    {
	"decoy",
	{ 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_decoy,		TAR_CHAR_DEFENSIVE,		POS_STANDING,
	NULL,			SLOT(632),	40,	12,
	"",	"You are informed that more decoys are ready for your use.", "", CMD_POWER
    },

    {
	"alarm",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_alarm,		TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(634),	50,	12,
	"",	"",	"", CMD_SPELL
    },


    {
	"holy fire",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_holy_fire,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(636),	20,	12,
	"holy fire",	"",	"", CMD_COMMUNE
    },

    {
	"blade barrier",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 37 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_blade_barrier,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_blade_barrier,	0,	50,	12,
	"blade barrier",	"",	"", CMD_COMMUNE
    },

    {
        "prevent healing",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_prevent_healing, TAR_CHAR_OFFENSIVE,  		POS_FIGHTING,
        &gsn_prevent_healing,  SLOT(803),	20,	0,
        "", "You feel your health returning.",  "", CMD_NONE
    },

    {
        "spiritblade",
        { 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_spiritblade,      TAR_CHAR_OFFENSIVE, 		POS_FIGHTING,
        NULL,   		SLOT(597),	30,     12,
        "spiritblade",  "", "", CMD_SPELL
    },

    {
        "iceball",
        { 53, 53, 53, 53, 29, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_iceball,  	TAR_IGNORE,     		POS_FIGHTING,
        NULL,   		SLOT(616),      20,     12,
        "iceball",      "", "", CMD_SPELL
    },

    {
        "bandage",
        { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_STANDING,
        &gsn_bandage,    	SLOT( 0),       0,     36,
        "",     "You are once more able to administer battle aid.", "", CMD_POWER
    },

    {
	"trophy",
	{ 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_RESTING,
	&gsn_trophy,		SLOT( 0),	0,	0,
	"",	"You feel up to making a new trophy.",	"", CMD_NONE
    },

    {
	"shadowplane",
	{ 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	spell_null,		TAR_IGNORE,			POS_SLEEPING,
	&gsn_shadowplane,	SLOT(0),	0,	0,
	"presence in the shadowplane",	"",	"", CMD_SPELL
    },

    {
	"request timer",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_request,		SLOT(0),	0,	0,
	"",	"Your contemplation of the good you have seen is over.",
	"", CMD_NONE
    },

    {
        "herb",
        { 53, 53, 53, 53, 53, 10, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_STANDING,
        &gsn_herb,       	SLOT( 0),       0,      12,
        "", "The herbs look more plentiful.", "", CMD_NONE
    },

    {
        "cleave",
        { 53, 53, 53, 53, 30, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,  		TAR_IGNORE,     		POS_STANDING,
        &gsn_cleave,   		SLOT(616),      0,     12,
        "cleave",      "", "", CMD_NONE
    },

    {
	"familiar",
	{ 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_familiar,		TAR_IGNORE, 			POS_FIGHTING,
	NULL,			SLOT(595),	75,	12,
	"",	"You feel up to summoning a new familiar.", "", CMD_SPELL
    },

    {
	"slave of purgatory",
	{ 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_slave_of_purgatory,	TAR_IGNORE, 		POS_FIGHTING,
	NULL,			SLOT(596),	100,	12,
	"",	"You feel your summoning power return.", "", CMD_POWER
    },

    {
	"shroud",
	{ 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_shroud,		TAR_IGNORE, 			POS_STANDING,
	&gsn_dark_shroud,	SLOT(597),	100,	12,
	"",	"You feel your shroud fades.", "", CMD_POWER
    },

    {
        "shadowgate",
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null, 		TAR_IGNORE, 			POS_STANDING,
        &gsn_shadowgate, 	SLOT( 0),	75, 	36,
        "", "You step into shadow gate","", CMD_NONE
    },

    {
        "eye of the predator",
        { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE, 			POS_STANDING,
        &gsn_eye_of_the_predator,	SLOT( 0),	50,	12,
        "", "Your predatory awareness recedes.", "", CMD_POWER
    },

    {
        "frostbolt",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_frostbolt,    	TAR_CHAR_OFFENSIVE, 		POS_FIGHTING,
        NULL, 			SLOT(780),	25,	12,
        "frostbolt",    "", "", CMD_SPELL
    },

    {
        "icelance",
        { 53, 53, 53, 53, 53, 53, 53, 23, 53, 53, 23, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_icelance, 	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
        NULL,  			SLOT(781),	24,	12,
        "icelance", "", "", CMD_SPELL
    },

    {
        "icelance wound",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        NULL, 			TAR_IGNORE,		POS_FIGHTING,
        &gsn_icelance,  	SLOT(0),	0,	0,
        "", "The icelance buried in your chest melts and the gaping wound seals.", "", CMD_NONE
    },

    {
        "earthfade",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 39, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_earthfade,    	TAR_CHAR_SELF,  		POS_STANDING,
        &gsn_earthfade, 	SLOT(782),  	100,    12,
        "", "You slowly emerge from the earth.", "", CMD_SPELL
    },

    {
        "forget",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 29, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_forget,   	TAR_CHAR_OFFENSIVE, 		POS_FIGHTING,
        &gsn_forget,    	0,	50, 	12,
        "", "Your memories return..",    "", CMD_SPELL
    },

    {
        "earthbind",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_earthbind,    	TAR_CHAR_OFFENSIVE, 		POS_FIGHTING,
        &gsn_earthbind, 	SLOT(784),  	20, 	12,
        "", "Your feet no longer feel bound to the ground.",    "", CMD_SPELL
    },

    {
        "divine touch",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_divine_touch,     TAR_CHAR_SELF,  		POS_STANDING,
        &gsn_divine_touch,   	SLOT(786), 	30, 	12,
        "", "Your hands lose their divine touch.",  "", CMD_SPELL
    },

    {
        "transfer object",
        { 53, 53, 53, 53, 53, 53, 53, 32, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_transfer_object,  TAR_IGNORE,    			POS_STANDING,
        NULL,   		SLOT(787),	30,	12,
        "", "", "", CMD_SPELL
    },

    {
        "disintegrate",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_disintegrate,  	TAR_CHAR_OFFENSIVE, 		POS_STANDING,
        NULL,   		SLOT(788),	100,	12,
        "disintegration",   "", "", CMD_SPELL
    },

    {
        "grounding",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_grounding,    	TAR_CHAR_SELF,  		POS_STANDING,
        &gsn_grounding, 	SLOT(789),	70, 	12,
        "", "You lose your grounding against electricty.",  "", CMD_SPELL
    },

    {
        "shock sphere",
        { 53, 53, 53, 53, 53, 53, 53, 18, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_shock_sphere, 	TAR_CHAR_OFFENSIVE, 		POS_FIGHTING,
        &gsn_shock_sphere,  	SLOT(790),	20, 	12,
        "shock sphere", "The ringing in your ears dies away.", "", CMD_SPELL
    },

    {
        "forage",
        { 53, 53, 53, 53, 53, 25, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null, 		TAR_IGNORE,	 		POS_STANDING,
        &gsn_forage,    	SLOT(0),	0,	0,
        "", "", "", CMD_NONE
    },

    {
	    "fwood",
	    { 53, 53, 53, 53, 53, 25, 53, 53, 53, 53, 53, 53, 53 },
	    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	    spell_null, 		TAR_IGNORE,	 		POS_STANDING,
	    &gsn_fwood,    	SLOT(0),	0,	0,
	    "", "", "", CMD_NONE
    },

    {
        "assassinate",
        { 53, 53, 53, 53, 53, 53, 53, 53, 35, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null, 		TAR_IGNORE,			POS_STANDING,
        &gsn_assassinate,    	SLOT(0),	0,	0,
        "assassination attempt", "", "", CMD_NONE
    },

    {
        "defend",
        { 29, 53, 53, 24, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null, 		TAR_IGNORE, 			POS_STANDING,
        &gsn_defend,    	SLOT(0),	0,	0,
        "", "", "", CMD_NONE
    },

    {
        "escape",
        { 53, 27, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null, 		TAR_IGNORE, 			POS_STANDING,
        &gsn_escape,    	SLOT(0),    	0,  	0,
        "", "", "", CMD_NONE
    },

    {
	"deathstrike",
	{ 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 },
	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_deathstrike,	SLOT( 0),	0,	0,
	"deathstrike",	"",	"", CMD_NONE
    },

    {
        "bloodthirst",
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_bloodthirst,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_bloodthirst,       SLOT( 0),       0,      12,
        "",                     "Your bloody rage fades away.",       "", CMD_POWER
    },

    {
        "shackles",
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_STANDING,
        &gsn_shackles,       	SLOT( 0),       0,      12,
        "",                     "Your hands break free of their bonds.", "", CMD_NONE
    },

    {
	"bloody shackles",
	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_bloody_shackles,	SLOT( 0),	0,	12,
	"bloody shackles",	"Your hands break free of their bonds.", "", CMD_NONE
    },

    {
	"flurry",
	{ 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_flurry,		SLOT( 0),	0,	15,
	"flurry",	"",	"", CMD_NONE
    },

    {
	"drum",
	{ 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_drum,		SLOT( 0),	0,	16,
	"drumming maces",	"",	"", CMD_NONE
    },

    {
        "siton",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_siton,      SLOT( 0),       0,      12,
        "immense weight",       "",     "", CMD_NONE
    },

    {
        "mental knife",
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_mental_knife,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(0),	15,	12,
	"mental knife",	"!Mental Knife!",	"", CMD_SPELL
    },

    {
        "light of heaven",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_light_of_heaven,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(0),	15,	12,
	"light of heaven",	"!Light of Heaven!",	"", CMD_COMMUNE
    },

    {
        "knife",
        { 53, 1, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null,             TAR_IGNORE,             	POS_STANDING,
        &gsn_knife,          	SLOT( 0),        0,     20,
        "knifing",             "!Knife!",           "", CMD_NONE
    },

    {
        "disperse",
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_STANDING,
        &gsn_disperse,      	SLOT( 0),       0,      0,
        "",       "",     "", CMD_POWER
    },

    {
        "scourge",
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_scourge,		TAR_IGNORE,			POS_FIGHTING,
	NULL,			SLOT(0),	75,	12,
	"scourge",	"!Scourge!",	"", CMD_SPELL
    },

    {
        "decapitate",
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_decapitate,    	SLOT( 0),       0,      18,
        "decapitation attempt",
        "Your neck feels connected to your head again.",     "", CMD_NONE
    },

    {
        "lightwalk",
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_STANDING,
        &gsn_lightwalk,    	SLOT( 0),       0,      18,
        "",       "",     "", CMD_POWER
    },

    {
        "bobbit",
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_bobbit,      	SLOT( 0),       0,      25,
        "man-wound",  "The Immortals restore your manhood.",     "", CMD_NONE
    },

    {
        "garble",
        { 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_garble,      	TAR_CHAR_OFFENSIVE,     	POS_STANDING,
        &gsn_garble,            SLOT(0),	50,	12,
        "",          "You feel you can speak again.","", CMD_POWER
    },


    {
        "wraithform",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 25, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_wraithform,      	TAR_CHAR_SELF,     		POS_STANDING,
        &gsn_wraithform,        SLOT(0),     	 50,	12,
        "",          "You slowly fade back in to the Prime Material Plane.","", CMD_SPELL
    },

    {
        "impale",
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_impale,      	SLOT( 0),       0,      50,
        "impalement wound",   "The wound on your chest feels better.", "", CMD_NONE
    },


    {
	    "haunting",
	    { 53, 53, 53, 53, 40, 53, 53, 53, 53, 53, 53, 53, 53 },
	    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	    spell_haunting,  	TAR_CHAR_OFFENSIVE,     	POS_FIGHTING,
	    &gsn_haunting,   	SLOT(654),	75, 	12,
	   "haunting", "Your mind eases as the haunting images fade.", "", CMD_SPELL
    },

    {
        "unholy bless",
        { 53, 53, 53, 53, 40, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_unholy_bless,  	TAR_OBJ_INV,     	POS_STANDING,
        &gsn_unholy_bless,   	SLOT(653),	500,	12,
        "",      "Your cursed blessing wears off.", "", CMD_SPELL
    },

    {
	"divine aura",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_divine_aura,	TAR_IGNORE,			POS_FIGHTING,
	&gsn_divine_aura,	SLOT(755),	50,	12,
	"",	"Your aura of the divine slowly fades away.",	"", CMD_BOTH
    },

    {
        "executioners grace",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_executioner,	SLOT(0),	300,	0,
	"deadly assault",
	"You no longer flow with the deadly grace of an executioner.",	"", CMD_NONE
    },

    {

        "eyes of intrigue",
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_eyes_of_intrigue, TAR_IGNORE,     		POS_STANDING,
        &gsn_intrigue,                   SLOT(801),      150,     24,
        "",                     "",   "", CMD_POWER
    },

    {
        "confuse",
        { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_confuse,  	TAR_CHAR_OFFENSIVE,     	POS_FIGHTING,
        &gsn_confuse,           SLOT(802),      50,     12,
        "",                     "You feel less confused.", "", CMD_POWER
    },

    {
	"crushing hand",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_crushing_hand,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,	SLOT(0),	20,	24,
	"crushing hand",	"",	"", CMD_SPELL
    },

    {
        "binds",
        { 53, 30, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_CHAR_OFFENSIVE,		POS_STANDING,
	&gsn_bind, 		SLOT(0),	0,	0,
	"",	"You break free of the bindings on your head and legs.",	"", CMD_NONE
    },

    {
        "cheap shot",
        { 53, 30, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_cheap_shot, 	SLOT(0),	0,	0,
	"cheap shot",	"",	"", CMD_NONE
    },

    {
        "deafen",
        { 53, 53, 53, 53, 24, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_deafen,       	TAR_CHAR_OFFENSIVE,     	POS_FIGHTING,
        &gsn_deafen,            SLOT(804),      20,     12,
        "",          "The ringing in your ears finally stops.","", CMD_SPELL
    },

    {
        "blitz",
        { 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_blitz,            	SLOT( 0),       0,      24,
        "blitz",                 "!Blitz!",		"", CMD_NONE
    },

    {
        "questdodge",
        { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_questdodge,        SLOT( 0),       0,	0,
        "",                     "!QuestDodge!",		"", CMD_NONE
    },

    {
	"agility",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_questdodge,	SLOT( 0),	0,	0,
	"",			"!Agility!",		"", CMD_NONE
    },

    {
        "divine intervention",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,       	TAR_IGNORE,     		POS_DEAD,
        &gsn_divine_intervention,	SLOT(0),	0,	0,
        "",          "You are no longer intervened by the Immortals.","", CMD_NONE
    },

    {
        "scrying",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_scrying, 		TAR_IGNORE,     		POS_STANDING,
        NULL,                   SLOT(801),      150,     12,
        "",                     "",   "", CMD_BOTH
    },

    {
	"nova",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_nova,		TAR_IGNORE,			POS_FIGHTING,
	NULL,			SLOT(0),	20,	12,
	"nova",	"",	"", CMD_SPELL
    },

    {
	"creep",
	{ 53, 53, 53, 53, 53, 15, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_creep,		SLOT(0),	0,	0,
	"",	"You feel less stealthy in the forest.",	"", CMD_NONE
    },

    {
		"darkforge",
		{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_darkforge,	TAR_IGNORE,		POS_STANDING,
	&gsn_darkforge,		SLOT(0),	30, 0,
	"",	"!darkforge!",	"",		CMD_POWER
	},

	{
        "dragonsword",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_dragonsword,	SLOT(0),	50,	0,
	"jab",	"You feel up to forging a new golden sword.",	"", CMD_POWER
    },

    {
        "dragonplate",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_dragonplate,	SLOT(0),	50,	0,
	"",	"You feel up to forging a new dragonplate.",	"", CMD_POWER
    },

    {
        "blademaster",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(0),	50,	0,
	"graceful riposte",	"",	"", CMD_NONE
    },

    {
        "prevent",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	NULL,			SLOT(0),	50,	0,
	"",	"",	"", CMD_SPELL
    },

    {
		"dark sword",
		{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		spell_null,		TAR_IGNORE,		POS_SLEEPING,
		&gsn_darkforge_sword,	SLOT(0),	0,	0,
		"dark sword",	"You can once again darkforge a sword.",	"",	CMD_NONE
	},

	{
		"dark dagger",
		{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		spell_null,		TAR_IGNORE,		POS_SLEEPING,
		&gsn_darkforge_dagger,	SLOT(0),	0,	0,
		"dark dagger",	"You can once again darkforge a dagger.",	"",	CMD_NONE
	},

	{
		"dark whip",
		{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		spell_null,		TAR_IGNORE,		POS_SLEEPING,
		&gsn_darkforge_whip,	SLOT(0),	0,	0,
		"dark whip",	"You can once again darkforge a whip.",	"",	CMD_NONE
	},

    {
        "unholy guard",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_SLEEPING,
        &gsn_unholy_timer,   	SLOT(0),	0,	0,
        "unholy torments", "Your corpse can be drained for power again.",    "", CMD_NONE
    },

    {
        "truestrike",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        NULL,   		SLOT(0),	0,	0,
        "", "",    "", CMD_NONE
    },

    {
        "honorable combat",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_STANDING,
        &gsn_honorable_combat,  SLOT(0),	0,	0,
        "doublethrust",
        "You feel ready to challenge someone to honorable combat again.",    "", CMD_POWER
    },

    {
        "chaos blade",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_chaos_blade,  	TAR_IGNORE,     		POS_STANDING,
        &gsn_chaos_blade,   	SLOT(0),	0,	0,
        "", "You are ready to make another chaos blade.",    "", CMD_POWER
    },

    {
        "aristaeia",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,  		TAR_IGNORE,     		POS_STANDING,
        &gsn_aristaeia,   	SLOT(0),	0,	0,
        "", "Your righteous fury dies down and you feel able to retreat again.",    "", CMD_NONE
    },

    {
		"dark blessing",
		{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		spell_null,			TAR_IGNORE,				POS_STANDING,
		&gsn_dark_blessing,	SLOT(0),	0,	0,
		"!dark!",	"Your dark blessing ebbs.",		"",	CMD_NONE
	},

    {
        "rage",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,  		TAR_IGNORE,     		POS_STANDING,
        &gsn_rage,   		SLOT(0),	0,	0,
        "cowardice", "Your rage fades away as your pulse slows down.",    "", CMD_NONE
    },

    {
        "rage of Dioxide",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,  		TAR_IGNORE,     		POS_STANDING,
        &gsn_rage_of_dioxide,   SLOT(0),	0,	0,
        "raging fury", "Your rage fades away as your pulse slows down.",    "", CMD_NONE
    },

    {
        "phat blunt",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,  		TAR_IGNORE,     		POS_STANDING,
        &gsn_phat_blunt,   	SLOT(0),	0,  	0,
        "phat blunt", "Your stoned fades away.",    "", CMD_NONE
    },

    {
        "body of steel",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,  		TAR_IGNORE,     		POS_STANDING,
        &gsn_body_of_steel,   	SLOT(0),	0,	0,
        "", "Your skin loses its steel texture.",    "", CMD_NONE
    },

    {
        "body of diamond",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,  		TAR_IGNORE,     		POS_STANDING,
        &gsn_body_of_diamond,   SLOT(0),	0,	0,
        "", "Your skin loses its diamond-like texture.",    "", CMD_NONE
    },

    {
        "body of stone",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,  		TAR_IGNORE,     		POS_STANDING,
        &gsn_body_of_stone,   	SLOT(0),	0,	0,
        "", "Your skin loses its stone-like texture.",    "", CMD_NONE
    },

    {
        "body of clay",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,  		TAR_IGNORE,     		POS_STANDING,
        &gsn_body_of_clay,   	SLOT(0),        0,	0,
        "", "Your skin loses its clay texture.",    "", CMD_NONE
    },

    {
        "body of wood",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,  		TAR_IGNORE,     		POS_STANDING,
        &gsn_body_of_wood,   	SLOT(0),	0,	0,
        "", "Your skin loses its wooden texture.",    "", CMD_NONE
    },

    {
        "body of iron",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,  		TAR_IGNORE,     		POS_STANDING,
        &gsn_body_of_iron,   	SLOT(0),        0,	0,
        "", "Your skin loses its abnormality.",    "", CMD_NONE
    },

    {
        "artic fury",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,  		TAR_IGNORE,     		POS_STANDING,
        &gsn_artic_fury,   	SLOT(0),	0,  	0,
        "artic fury", "{bYour artic fury fades away.{x",    "", CMD_NONE
    },

    {
        "spin",
        { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_spin,             	SLOT(0),	0,	0,
        "defensive spin",                     "!Spin!",		"", CMD_NONE
    },

    {
        "stun",
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_STANDING,
        &gsn_stun,  		SLOT(0),       0,	24,
        "stunning blow",    "You come to with a pounding headache.", "", CMD_NONE
    },

    {
        "enhanced damage III",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_enhanced_damage_three,	SLOT(0),	0,      0,
        "",     "",     "", CMD_NONE
    },

    {
        "gouge",
        { 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_STANDING,
        &gsn_gouge,  		SLOT( 0),	0,	24,
        "gouge",    "Your eye heals from the gouging and you can see again.", "", CMD_NONE
    },

    {
        "stab",
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_STANDING,
        &gsn_stab,  		SLOT( 0),	0,	24,
        "stab",    "", "", CMD_NONE
    },

    {
        "stalker",
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_stalker,     	TAR_IGNORE, 			POS_STANDING,
        &gsn_stalker,    	SLOT( 0),   	200, 	12,
        "", "You feel up to summoning another stalker.", "", CMD_POWER
    },
/*
    {
        "dream blossom",
        { 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_dream_blossom,	TAR_IGNORE, 		POS_STANDING,
        &gsn_dream_blossom,	SLOT( 0),	100,	12,
        "", "You feel as though you can summon the Dream Blossoms once more.", "", CMD_POWER
    },
*/
    {
        "randomizer",
        { 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52, 52 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_randomizer,     	TAR_IGNORE, 			POS_STANDING,
        &gsn_randomizer,    	SLOT( 0),	25,	12,
        "", "You feel your randomness regenerating.", "", CMD_POWER
    },

    {
        "zealotism",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,  		TAR_IGNORE,     		POS_STANDING,
        &gsn_zealotism,   	SLOT(0),	0,	0,
        "heavenly wrath", "Your righteous fury ebbs.",    "", CMD_NONE
    },

    {
        "parting blow",
        { 24, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_parting_blow,      SLOT( 0),       0,	23,
        "parting blow",         0,       "", CMD_NONE
    },

    {
	"divine aid",
	{ 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_divine_aid,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(553),	20,	17,
	"divine aid",	"!Divine Aid!",	"", CMD_BOTH
    },

    {
        "minion tactics",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_minion_tactics,	TAR_CHAR_DEFENSIVE,     	POS_STANDING,
        NULL,                   SLOT( 1),	100,	17,
          "",                     "You feel less tactical.",       "", CMD_SPELL
    },

    {
        "minion sanc",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_minion_sanc,   	TAR_CHAR_DEFENSIVE,     	POS_STANDING,
        NULL,                   SLOT( 1),	100,	17,
          "",                     "You are no longer surrounded by a white aura.",       "", CMD_SPELL
    },

    {
        "minion flight",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_minion_flight,  	TAR_CHAR_DEFENSIVE,     	POS_STANDING,
        NULL,                   SLOT( 1),	100,	17,
          "",                     "You slowly float to the ground.",       "", CMD_SPELL
    },

    {
        "minion sneak",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_minion_sneak,     TAR_CHAR_DEFENSIVE,  		POS_STANDING,
        NULL,                   SLOT( 1),       100,    17,
          "",                     "You trample around loudly again.",       "", CMD_SPELL
    },

    {
        "minion haste",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_minion_haste,     TAR_CHAR_DEFENSIVE,		POS_STANDING,
        NULL,                   SLOT( 1),       100,	17,
          "",                     "You slow down.",       "", CMD_SPELL
    },

    {
        "minion recall",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_minion_recall,    TAR_CHAR_DEFENSIVE,     	POS_STANDING,
        NULL,                   SLOT( 1),       100,    17,
          "",                     "You can recall again.",       "", CMD_SPELL
    },

    {
        "minion trans",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_minion_trans,     TAR_CHAR_DEFENSIVE,     	POS_STANDING,
        NULL,                   SLOT( 1),       100,	17,
          "",                     "You feel solid again.",       "", CMD_SPELL
    },

    {
	"shieldbash",
	{ 53, 53, 53, 51, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_shieldbash,       	SLOT( 0),       0,      18,
        "shieldbash",           "!Shieldbash!",		"", CMD_NONE
    },

    {
        "hacksaw",
        { 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,        	TAR_IGNORE,             	POS_STANDING,
        &gsn_hacksaw,       	SLOT( 0),       0,      12,
        "blundering mistake",               "You can hacksaw again.",       "", CMD_POWER
    },

    {
	"boneshatter",
	{ 30, 30, 30,30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_boneshatter,	SLOT( 0),	0,	25,
	"bone-shattering blow",	"Your bones mend.",	"", CMD_NONE
    },

    {
        "rejuvenate",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_rejuvenate,	TAR_CHAR_DEFENSIVE,		POS_FIGHTING,
	NULL,			SLOT(28),	80,	12,
	"",			"!Rejuvenate!",		"", CMD_COMMUNE
    },

    {
        "pincer",
        { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_pincer,    	SLOT( 0),       0,     24,
        "brutal attack",       "!pincer!",     "", CMD_NONE
    },

    {
	"spiderhands",
	{ 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_spiderhands,	TAR_CHAR_DEFENSIVE,		POS_STANDING,
	&gsn_spiderhands,	SLOT(0),	50,	17,
	"",	"The melding with your weapon fades.",	"", CMD_POWER
    },

    {
        "charge",
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     TAR_IGNORE,     POS_STANDING,
        &gsn_charge,   SLOT(0),        0,  15,
        "charge", "",    "", CMD_NONE
    },


    {
	"charge set",
	{ 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_chargeset,		SLOT( 0),	0,	25,
	"charge set",	"",	"", CMD_NONE
    },

    {
	"devour",
	{ 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_RESTING,
        &gsn_devour,            SLOT( 0),        0,     12,
        "",                     "!Devour!",		"", CMD_NONE
    },

    {
        "strip",
        { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_strip,    		SLOT( 0),       0,     0,
        "",       "!Strip!",     "", CMD_NONE
    },

    {
        "headclap",
        { 53, 53, 53, 53,53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_STANDING,
        &gsn_headclap,  SLOT( 0),       0,      0,
        "head clap",    "Your ears stop ringing.", "", CMD_NONE
    },

    {
        "choke",
        { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_choke,    		SLOT( 0),       0,     0,
        "choke",       "Your throat stops hurting.",     "", CMD_NONE
    },

	{
        "rot",
        { 53, 53, 40, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_rot,       	TAR_CHAR_OFFENSIVE,     	POS_STANDING,
        &gsn_rot,               SLOT(804),      50,     12,
        "decomposition",        "Your body stops decomposing.","", CMD_BOTH
    },

    {
        "body reconstruction",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,       	TAR_IGNORE,     	POS_FIGHTING,
        &gsn_rot_recovery,   SLOT(0),      50,     12,
        "",        "Your body completes its natural healing process.","", CMD_NONE
    },

	{
	"fatigue",
	{ 53, 53, 26, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_fatigue,	TAR_CHAR_OFFENSIVE,			POS_FIGHTING,
	NULL,                   0,        	50,      12,
	"fatigue",		"",	"",	CMD_COMMUNE
    },

    {
        "eagle eyes",
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,       	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_eagle_eyes,        SLOT(0),     	0,     0,
        "eagle eyes",          "Your eyes feel less sharp.","", CMD_BOTH
    },

    {
        "cutoff",
        { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,       	TAR_IGNORE,     		POS_FIGHTING,
        &gsn_cutoff,            SLOT(0),     	0,     0,
        "cutoff",          "!Cutoff!","", CMD_NONE
    },

    {
	"incinerate",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_incinerate,	TAR_CHAR_OFFENSIVE,		POS_STANDING,
	&gsn_incinerate,	SLOT(0),	50,	0,
	"incineration",		"You finally stop burning.",	"", CMD_SPELL
    },

    {
        "integrate style",
        { 45, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,  			POS_FIGHTING,
        &gsn_integrate,         0,           	0,    	0,
        "",  "",     "", CMD_NONE
    },

    {
        "chameleon",
        { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     		TAR_IGNORE,            POS_STANDING,
        &gsn_chameleon,         0, 		0, 	24,
        "", "You step out from your cover.", "", CMD_POWER
    },

    {
        "insectswarm",
        { 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null, 	TAR_IGNORE,         	POS_STANDING,
        &gsn_insect_swarm,  	SLOT(0),     	0, 	0,
        "insect bites", "", "", CMD_POWER
    },

    {
        "pandemonium",
        { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_pandemonium,	TAR_IGNORE,			POS_FIGHTING,
	NULL,			SLOT(0),	15,	12,
	"pandemonium",		"You feel ready to perpetrate more pandemonium", "", CMD_POWER
    },

    {
        "beast call",
        { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_beast_call,     	TAR_CHAR_OFFENSIVE, 		POS_STANDING,
        &gsn_forest_friend,     SLOT(0),        50,     12,
        "",     "The beasts of the wilds are ready to lend you their aid once more.", "", CMD_POWER
    },

    {
        "distance",
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,            	TAR_IGNORE,    			POS_STANDING,
        NULL,                  	0,	0,	0,
        "counterstrike",       "", "", CMD_NONE
    },

    {
        "flourintine",
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,            	TAR_IGNORE,    			POS_STANDING,
        &gsn_flourintine,      0,	0,	0,
        "",    "",   "", CMD_NONE
    },

    {
        "ironhands",
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,            	TAR_IGNORE,    			POS_STANDING,
        &gsn_ironhands,        0,	0,	0,
        "", "", "", CMD_NONE
    },

    {
        "overhead",
        { 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,            	TAR_IGNORE,    			POS_STANDING,
        &gsn_overhead,          0,	0,	0,
        "wild overhead attack", "", "", CMD_NONE
    },

    {
        "self immolation",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 30, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_self_immolation,	TAR_CHAR_DEFENSIVE,     	POS_STANDING,
        &gsn_self_immolation,   SLOT(219),	100,	12,
        "self immolation", "You cease to immolate.",
        "", CMD_SPELL
    },

    {
        "shield of wind",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 20, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_shield_of_wind,  	TAR_CHAR_DEFENSIVE,     	POS_STANDING,
        &gsn_shield_of_wind,    SLOT(220),	50,	12,
        "", "Your shield of winds disperses.","", CMD_SPELL
    },

    {
        "shield of dust",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 25, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_shield_of_dust, 	TAR_CHAR_DEFENSIVE,     	POS_STANDING,
        &gsn_shield_of_dust,    SLOT(221),	50,	12,
        "", "Your shield of dust crumbles.","", CMD_SPELL
    },

    {
        "shield of flames",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 30, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_shield_of_flames,	TAR_CHAR_DEFENSIVE,     	POS_STANDING,
        &gsn_shield_of_flames,	SLOT(222),	50,	12,
        "", "Your shield of flames flickers and vanishes.","", CMD_SPELL
    },

    {
        "shield of frost",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 35, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_shield_of_frost,	TAR_CHAR_DEFENSIVE,     	POS_STANDING,
        &gsn_shield_of_frost,   SLOT(223),	50,	12,
        "", "Your shield of frost melts.","", CMD_SPELL
    },

    {
        "shield of lightning",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_shield_of_lightning,	TAR_CHAR_DEFENSIVE,     POS_STANDING,
        &gsn_shield_of_lightning,       SLOT(224),	50,	12,
        "", "Your shield of lightning vanishes.","", CMD_SPELL
    },

    {
        "shield of bubbles",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 45, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_shield_of_bubbles,	TAR_CHAR_DEFENSIVE,     POS_STANDING,
        &gsn_shield_of_bubbles,         SLOT(225),       50,     12,
        "", "Your shield of bubbles pops.","", CMD_SPELL
    },

    {
        "flames",
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        spell_null,     	TAR_IGNORE,     		POS_DEAD,
        &gsn_flames,        	SLOT(0),	0,	0,
        "searing flames",   	"",	"", CMD_NONE
    },

    {
        "hydrolic ram",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 45, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_hydrolic_ram, 	TAR_CHAR_OFFENSIVE,         	POS_FIGHTING,
        NULL,  			SLOT(226),	40,	24,
        "hydrolic ram", "", "", CMD_SPELL
    },

    {
        "spontanious combustion",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_spontanious_combustion, TAR_CHAR_OFFENSIVE,     	POS_STANDING,
        NULL,  			SLOT(227),	100,	24,
        "spontanious combustion", "", "", CMD_SPELL
    },

    {
        "blizzard",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 48, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_blizzard, 	TAR_IGNORE,     	POS_STANDING,
        NULL,  			SLOT(1001),	200,	48,
        "", "", "", CMD_SPELL
    },

    {
        "blizzard room affect",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        NULL, 			TAR_IGNORE,     	POS_STANDING,
        &gsn_blizzard,  			SLOT(0),	0,	0,
        "", "The blizzard dissipates.", "", CMD_NONE
    },

    {
        "shockwave",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_shockwave, 	TAR_IGNORE, 			POS_FIGHTING,
        NULL,  			SLOT(228),	30,	24,
        "shockwave", "", "", CMD_SPELL
    },

    {
        "static charge",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 24, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_static_charge, 	TAR_CHAR_SELF,          	POS_STANDING,
        &gsn_static_charge,     SLOT(229),	150,	24,
        "static shock", "Your charge disipates.",
        "$p's hair stops standing on end.", CMD_SPELL
    },

    {
        "flicker",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_flicker, 		TAR_CHAR_SELF, 			POS_FIGHTING,
        NULL,  			SLOT(230),	35,	24,
        "", "", "", CMD_SPELL
    },

    {
        "smoke",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 37, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_smoke, 		TAR_CHAR_OFFENSIVE, 		POS_FIGHTING,
        NULL,  			SLOT(231),	15,	12,
        "", "The smoke clears your eyes", "blinks $s and wipes out the smoke", CMD_SPELL
    },

    {
        "ice slick",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 26, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_ice_slick, 	TAR_IGNORE, 			POS_STANDING,
        NULL,  			SLOT(235),	150,	48,
        "fall", "You feel ready to create another ice slick.", "", CMD_SPELL
    },

    {
        "create vacume",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_create_vacume, 	TAR_IGNORE, 			POS_STANDING,
        NULL,  			SLOT(236),	300,	PULSE_TICK,
        "asphyxiation", "You can create another vacume now.", "", CMD_SPELL
    },

    {
        "hellfire",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 40, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_hellfire, 	TAR_CHAR_OFFENSIVE, 		POS_FIGHTING,
        &gsn_hellfire,		SLOT(237),	80,	36,
        "hellfire", "The hellfire subsides", "The hellfire consuming $n subsides", CMD_SPELL
    },

    {
        "pyrotechnics",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_pyrotechnics, 	TAR_IGNORE, 			POS_FIGHTING,
        NULL,  			SLOT(238),	90,	24,
        "pyrotechnics", "", "", CMD_SPELL
    },

    {
        "ball lightning",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 45, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_ball_lightning, 	TAR_CHAR_OFFENSIVE, 		POS_FIGHTING,
        NULL,  			SLOT(239),	30,	12,
        "ball lightning", "", "", CMD_SPELL
    },

/*a group of illusionist spells*/
    {
        "nullify sound",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_nullify_sound, 	TAR_CHAR_OFFENSIVE, 		POS_FIGHTING,
        &gsn_nullify_sound,  	SLOT(232),	35,	12,
        "", "The world no longer seems so quiet.", "", CMD_SPELL
    },

    {
        "detect illusion",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_detect_illusion, 	TAR_CHAR_DEFENSIVE, 		POS_STANDING,
        &gsn_detect_illusion,	SLOT(233),	10,	12,
        "", "Your senses return to normal.", "", CMD_SPELL
    },

    {
        "displacement",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_displacement, 	TAR_CHAR_DEFENSIVE, 		POS_FIGHTING,
        &gsn_displacement,  	SLOT(234),	120,	12,
        "", "You no longer mask your true location.", "", CMD_SPELL
    },

    {
    	"snare",
    	{ 53, 53, 53, 53, 53, 40, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	 	TAR_IGNORE, 			POS_STANDING,
	&gsn_snare,	 	SLOT( 0),	75,	12,
	"",	"You free yourself from the snare.", "", CMD_NONE
    },

    {
	"snaretimer",
	{ 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	 	TAR_IGNORE, 			POS_STANDING,
	&gsn_snaretimer, 	SLOT( 0),	75,	12,
	"flames of the apocalypse",	"You feel able to construct a new snare.", ""
    },

    {
        "whirl",
        { 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_STANDING,
        &gsn_whirl,  		0,	0,	0,
        "whirl",    "The axe wounds close up.", "", CMD_NONE
    },

        {
        "nightfist",
        { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,         	TAR_IGNORE,             	POS_STANDING,
        &gsn_nightfist,      	SLOT( 0),	0,	12,
        "",                     "Your unholy power fades away.",       "", CMD_POWER
    },

    {
        "centurions",
        { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_STANDING,
        &gsn_centurion_call,    SLOT( 0),	0,	12,
        "",                     "You can call Centurions again.",       "", CMD_POWER
    },

    {
    	"shove",
    	{ 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_shove,            	SLOT( 0),       0,      12,
        "disembowelment",  	"!Shove!",		"", CMD_NONE
    },

    {
	"black shroud",
	{ 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE, 			POS_FIGHTING,
	&gsn_shroud,		SLOT(0),	100,	12,
	"",	"The black aura around your body fades.", "", CMD_NONE
    },

    {
	"imperial training",
	{ 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,             	POS_FIGHTING,
        &gsn_imperial_training,	SLOT( 0),	0,	0,
        "",	"You feel less focused on your training.",		"", CMD_POWER
    },

    {
	"looking glass",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_looking_glass,	TAR_CHAR_OFFENSIVE,		POS_STANDING,
	NULL,			SLOT(599),	50,	12,
	"", 	"You pass back through the looking glass.",	"", CMD_SPELL
    },

    {
	"vanish",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 24, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_FIGHTING,
	&gsn_vanish,		SLOT( 0),	0,	12,
	"",	"You feel ready to vanish once again.", "", CMD_NONE
    },

    {
        "wall of thorns",
        { 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_wall_of_thorns, 	TAR_IGNORE, 			POS_STANDING,
        NULL,  			SLOT(600),	150,	48,
        "A wall of thorns's thorns", "You feel ready to create another wall of thorns.",
	"", CMD_POWER
    },

    {
        "weaponbreaker",
	{ 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,     	TAR_IGNORE,     		POS_STANDING,
        &gsn_weaponbreaker,  	0,       	0,      0,
        "powerful blow",    "", "", CMD_NONE
    },

    {
        "barrier",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_barrier,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_barrier,		0,	50,	12,
	"",			"The magical barrier surrounding you fades away.",
	"", CMD_SPELL
    },

    {
	"crushing blow",
	{ 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,	POS_FIGHTING,
	&gsn_crushing_blow,	0,	0, 	0,
	"crushing blows",	"",	"", CMD_NONE
    },

    {
	"hex",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 47, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_hex,		TAR_CHAR_OFFENSIVE,	POS_STANDING,
	&gsn_hex,		0,	100,	24,
	"hex",			"",	"", CMD_SPELL
    },

    {
	"dark summons",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 49, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_black_circle,	TAR_IGNORE,		POS_STANDING,
	NULL,			0,	250,	24,
	"",			"You feel able to call your minions to you again.",
	"", CMD_SPELL
    },

    {
	"shield of the onyx vanguard",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_shield_onyx,	TAR_IGNORE,		POS_STANDING,
	NULL,			0, 	100,	24,
	"",			"The now-hardened black ooze covering your skin cracks and crumbles into dust.",
	"", CMD_POWER
    },

    {
	"mantle of maehslin",
	{ 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_maehslin,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_maehslin,		SLOT(601),	100,	12,
	"",			"The fitful violet aura around you dissipates into nothingness.",
	"", CMD_POWER
    },

    {
	"assess",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 35 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,			TAR_IGNORE,		POS_STANDING,
	NULL,	SLOT(0),	0,	12,
	"",			"",			"", CMD_NONE
    },

    {
	"cure deafness",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 11 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_cure_deafness,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(0),	10,	12,
	"",	"",	"", CMD_COMMUNE
    },

    {
	"awaken",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 20 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_awaken,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(0),	40,	24,
	"",	"",	"", CMD_COMMUNE
    },

    {
	"resist heat",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 10 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
	spell_resist_heat,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(0),	50,	12,
	"",	"You are no longer resistant to heat.",	"", CMD_COMMUNE
    },

    {
	"resist cold",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 12 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_resist_cold,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(0),	50,	12,
	"",	"You are no longer resistant to cold.",	"", CMD_COMMUNE
    },
    {
	"resist lightning",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 14 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_resist_lightning,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(0),	50,	12,
	"",	"You are no longer resistant to lightning.",	"", CMD_COMMUNE
    },
    {
	"resist mental",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 16 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_resist_mental,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(0),	50,	12,
	"",	"You are no longer resistant to mental attacks.",	"", CMD_COMMUNE
    },
    {
	"resist acid",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 16 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_resist_acid,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(0),	50,	12,
	"",	"You are no longer resistant to acid.",	"", CMD_COMMUNE
    },
    {
	"resist negative",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 20 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_resist_negative,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(0),	50,	12,
	"",	"You are no longer resistant to negative damage.",	"", CMD_COMMUNE
    },

	//Resist positive - added for use in affects, but not coded for a spell.
    {
	"resist positive",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 20 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(0),	50,	12,
	"",	"You are no longer resistant to positive damage.",	"", CMD_NONE
    },

    {
	"resist paralysis",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 30 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_resist_paralysis,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(0),	50,	12,
	"",	"You are no longer resistant to paralysis.",	"", CMD_COMMUNE
    },

    {
	"resist weapon",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 51 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_resist_weapon,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(0),	50,	12,
	"",	"You are no longer resistant to weapons.",	"", CMD_COMMUNE
    },

    {
	"remove paralysis",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 30 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_remove_paralysis,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(0),	70,	12,
	"",	"",	"", CMD_COMMUNE
    },

    {
	"group teleport",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 33 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_group_teleport,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(0),	90,	12,
	"",	"",	"", CMD_COMMUNE
    },

    {
	"cleanse",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 35 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_cleanse,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	NULL,	SLOT(0),	75,	12,
	"",	"",	"", CMD_COMMUNE
    },

    {
        "strength of faith",
	{ 53, 53, 53, 47, 53, 53, 53, 53, 53, 53, 53, 53, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_strength,          TAR_CHAR_SELF,     POS_STANDING,
        NULL,   SLOT(0),        250,     80,
        "",     "The absolute certainty in your faith begins to waver.",     "", CMD_COMMUNE
    },

    {
	"soften",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_soften,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_soften,		0,	70,	12,
	"",		"You no longer feel so frail.", "", CMD_SPELL
    },

    {
	"duo dimension",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_duo_dimension,	TAR_CHAR_SELF,		POS_STANDING,
	&gsn_duo,		0,	125,	12,
	"",		"You feel three dimensional again.", "", CMD_SPELL
    },

    {
	"fumble",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_fumble,		TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	15,	12,
	"",	"",	"",	CMD_SPELL
    },

    {
	"knock",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_knock,		TAR_IGNORE,		POS_STANDING,
	NULL,			0,	5,	12,
	"",	"",	"",	CMD_SPELL
    },

    {
	"protection",
	{ 53, 53, 53, 53, 53, 53, 53, 12, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_protection,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			0,	5,	12,
	"",	"You feel less protected.",	"",	CMD_BOTH
    },

    {
	"prismatic spray",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_prismatic_spray,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			0,	15,	12,
	"prismatic spray",	"Your prismatic spray wears off.",	"",	CMD_SPELL
    },

    {
    	"tripwire",
    	{ 53, 35, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
    	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    	spell_null,		TAR_IGNORE,		POS_FIGHTING,
    	&gsn_trip_wire,		0,	25,	0,
    	"trip wire",	"You are able to construct another tripwire.",
    	"",	CMD_NONE
    },

    {
    	"wire delay",
    	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
    	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    	spell_null,		TAR_IGNORE,		POS_FIGHTING,
    	&gsn_wire_delay,	0,	25,	0,
    	"trip wire",	"You have restrung another trap, ready for use.",
    	"", CMD_NONE
    },

    {
    	"balance",
    	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
    	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    	spell_null,		TAR_IGNORE,		POS_FIGHTING,
    	&gsn_balance,		0,	25,	0,
    	"",	"!balance!",	"", CMD_NONE
    },

    {
    	"gag",
    	{ 53, 26, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
    	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    	spell_null,		TAR_IGNORE,		POS_STANDING,
    	&gsn_gag,		0,	10,	12,
    	"gag attempt",	"You can talk again.",	"",	CMD_NONE
    },

    {
    	"dragonweapon",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
    	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    	spell_dragonweapon,	TAR_IGNORE,		POS_FIGHTING,
    	NULL,			0,	80,	18,
    	"",			"!DragonWeapon!",
    	"",	CMD_POWER
    },

    {
        "healing sleep",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 45 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_healing_sleep,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
        &gsn_healing_sleep, 	0,       400,     12,
        "", "You awaken feeling refreshed, and cured of all ailments.", "", CMD_COMMUNE
    },

    {
        "cranial",
        { 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,	POS_STANDING,
        &gsn_cranial,		0,      0,      0,
        "cranial hit",	"Your head stops throbbing.", "", CMD_NONE
    },

    {
        "entwine",
        { 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,		TAR_IGNORE,	POS_STANDING,
        &gsn_entwine,		SLOT( 0),       0,      0,
        "",	"You feel able to flee again.",	"", CMD_NONE
    },

    {
        "legsweep",
        { 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,	TAR_IGNORE,     POS_STANDING,
        &gsn_legsweep,	SLOT( 0),       0,      0,
        "legsweep",     "", "", CMD_NONE
    },

    {
        "rising pheonix kick",
        { 53, 53, 53, 53, 53, 53, 53, 53, 48, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,			TAR_IGNORE,	POS_FIGHTING,
        &gsn_rising_pheonix_kick,	SLOT( 0),        0,     12,
        "rising pheonix kick",		"!Kick!",               "", CMD_NONE
    },

    {
        "double spin kick",
        { 53, 53, 53, 53, 53, 53, 53, 53, 48, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,		TAR_IGNORE,     POS_FIGHTING,
        &gsn_double_spin_kick,	SLOT( 0),        0,     12,
        "double spin kick",	"!Kick!",               "", CMD_NONE
    },
    
    {
    	"dash",
    	{ 25, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
    	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    	spell_null,	TAR_IGNORE,	POS_STANDING,
    	&gsn_dash,	SLOT(0),	0,	0,
    	"",		"!Dash!",	"",	CMD_NONE
    },
    
    {
    	"bleed",
    	{ 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61 },
    	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    	spell_null,		TAR_CHAR_SELF,			POS_STANDING,
    	&gsn_bleed,		SLOT(0),	0,	0,
    	"bleeding",		"",	"",	CMD_NONE
    },
    
    {
	"restrike",
	{ 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_CHAR_SELF,			POS_STANDING,
	&gsn_restrike,		SLOT(0),	0,	12,
	"rapid restrike",	"",	"", CMD_NONE
    },
    
    {
    	"crossslice",
    	{ 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33 },
    	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    	spell_null,		TAR_CHAR_SELF,			POS_STANDING,
    	&gsn_cross_slice,	SLOT(0),	0,	12,
    	"slicing blade",	"Your chest wounds heal.",	"", CMD_NONE
    },

    {
        "chromatic fire",
        { 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_chromatic_fire_new,	TAR_CHAR_OFFENSIVE,             POS_FIGHTING,
        NULL,            	SLOT(0),        50,     12,
        "",	"You are no longer engulfed in the chromatic fire.","", CMD_POWER
    },

    {
        "counterspell",
        { 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,	POS_STANDING,
        &gsn_counterspell,	SLOT( 0),       0,      0,
        "",    "",        "", CMD_NONE
    },

    {
        "kotegaeshi",
        { 53, 53, 53, 53, 53, 53, 53, 53, 24, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,       	TAR_IGNORE,     POS_FIGHTING,
        &gsn_kotegaeshi, 	SLOT(0),       0,      0,
        "kotegaeshi",		"Your wrist feels better.", "", CMD_NONE
    },

    {
        "spire of flames",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_spire_of_flames,	TAR_CHAR_OFFENSIVE,             POS_FIGHTING,
        &gsn_spire_of_flames,	SLOT(800),      65,     18,
        "spire of flames",	"",		"",	CMD_SPELL
    },

    {
        "worldbind",
        { 53, 53, 53, 53, 45, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_worldbind,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
        &gsn_worldbind,   	SLOT(950),	75,     18,
        "",	"Your ties to the spiritual world recover.",	"",	CMD_NONE
    },

    {
	"icy shards",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 38, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_icy_shards,	TAR_CHAR_OFFENSIVE,		POS_FIGHTING,
	&gsn_icy_shards,			SLOT(951),	75,	36,
	"icy shards",		"",	"",	CMD_SPELL
    },

    {
        "righteousness",
        { 53, 53, 53, 47, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_righteousness,	TAR_CHAR_DEFENSIVE,             POS_STANDING,
        &gsn_righteousness,	SLOT(952),       		75,     12,
        "",                     "You feel less righteous.",
        "", CMD_COMMUNE
    },

    {
	"chop",
	{ 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_STANDING,
	&gsn_chop,		SLOT(0),			0,	0,
	"chop",			"",	"",	CMD_NONE
    },

    {
        "weapon trip",
        { 53, 32, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_FIGHTING,
        &gsn_weapontrip,	SLOT( 0),       	0,      24,
        "weapon trip",                 "",		"", 	CMD_NONE
    },

    {
	"true sight",
	{ 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_true_sight,	TAR_CHAR_SELF,			POS_STANDING,
	&gsn_true_sight,	SLOT(0),		50,	24,
	"",			"Your true vision fades.",			"",	CMD_POWER
    },

    {
        "ward of diminution",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_ward_diminution,	TAR_CHAR_SELF,	POS_STANDING,
        &gsn_ward_diminution,	SLOT(0),      100,     12,
        "",                     "Your ward of diminution crumbles.",
        "", CMD_POWER
    },

    {
        "ward of diminution",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,		TAR_IGNORE,	POS_STANDING,
        &gsn_ward_timer,	SLOT(0),      	0,     0,
        "",                     "You feel ready to summon another ward of diminution.",
        "", CMD_POWER
    },

    {
	"dark embrace",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,	POS_STANDING,
	&gsn_ormthrak,		SLOT(0),	0,	0,
	"",			"The eternal power of Ormthrak channels through your body once more.",
	"", CMD_NONE
    },

    {
	"omnipotent wrath",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,	POS_STANDING,
	&gsn_zornath,		SLOT(0),	0,	0,
	"",			"Zornath's channeled energy leaves your idle body.",
	"", CMD_NONE
    },

    {
	"quest guard",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_SLEEPING,
	&gsn_credit_timer,	SLOT(0),	0,	0,
	"",			"Your corpse is worth quest credits once again.",
	"", CMD_NONE
    },

    {
	"beast call",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_SLEEPING,
	&gsn_beast_timer,	SLOT(0),	0,	0,
	"",			"The beasts of the wilds are ready to lend you their aid once more.",
	"", CMD_NONE
    },

    {
	"eavesdrop",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_eavesdrop,	TAR_IGNORE,	POS_STANDING,
	NULL,			SLOT(0),	100,	24,
	"",	"",	"", CMD_POWER
    },

    {
	"savants armor",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
       	spell_null,        TAR_IGNORE,     POS_STANDING,
        &gsn_tracerah,     SLOT(0),        100,    24,
        "",     "",     "", CMD_NONE
    },

    {
	"cloak",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_cloak_form,	SLOT( 0), 	0,	0,
	"",	"The wind pulls your cloak away from your face.", "", CMD_POWER
    },

    {
        "pummel",
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,		TAR_IGNORE, 	POS_STANDING,
        &gsn_pummel,            SLOT(0),       	0,       0,
        "pummeling fists",    	"", 	"",	CMD_NONE
    },

    {
	"sunray",
        { 53, 53, 53, 53, 53, 53, 35, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_sunray,       TAR_CHAR_OFFENSIVE,     POS_FIGHTING,
        &gsn_sunray,   		SLOT(0),      25,     12,
        "sunray",   		"The bright spot fades from your vision and you can see again.",     "", CMD_COMMUNE
    },

    {
	"vampiric touch",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_vampiric_touch,             TAR_CHAR_OFFENSIVE,             POS_FIGHTING,
        &gsn_vampiric_touch,       SLOT( 0),       0,      12,
        "deathly touch",                     "",       "", CMD_SPELL
    },

    {
	"unholy affinity",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_unholy_affinity,		TAR_CHAR_SELF,		POS_STANDING,
	&gsn_unholy_affinity,	SLOT(0),	300,	24,
	"Placeholder",	"You feel as though you could release your weapon.", "", CMD_SPELL
    },

    {
        "conjure fire elemental",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_create_fire_elemental, TAR_IGNORE,        POS_DEAD,   
        NULL,                   SLOT(0),        0,      0,
        "",                     "",     "", CMD_NONE
    },

    {
	"aura",
	{ 53, 53, 33, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_aura,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_aura,	SLOT(O),	75,	24,
	"Placeholder",	"The magical aura fades away.",	"",	CMD_BOTH
    },

    {
	"darkshout",
        { 53, 53, 53, 53, 47, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,		POS_STANDING,
	&gsn_darkshout,	SLOT(0),	0,	0,
	"",	"Your unholy will resides.", "", CMD_NONE
    },

    {
	"pathfinding",
	{ 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,		POS_SLEEPING,
	&gsn_pathfinding, SLOT(0),	0,	0,
	"",	"",	"", CMD_NONE
    },

    {
	"enlarge",
        { 53, 53, 53, 53, 53, 53, 29, 15, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_enlarge,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_enlarge,		SLOT(0),	40,	24,
	"", "You shrink back to your normal size.", "", CMD_BOTH
    },

    {
	"reduce",
        { 53, 53, 53, 53, 53, 53, 53, 15, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_reduce,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_reduce,		SLOT(0),	40,	24,
	"", "You return to your normal size.", "", CMD_SPELL
    },

    {
	"shield of purity",
        { 53, 53, 53, 48, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_shield_of_purity,		TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_shield_of_purity,		SLOT(0),	200,	24,
	"", "You feel more vulnerable as the power of your god leaves your shield.", "", CMD_COMMUNE
    },

    {
	"shield of redemption",
        { 53, 53, 53, 48, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_shield_of_redemption,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_shield_of_redemption,	SLOT(0),	200,	24,
	"", "You feel less zealous as the power of your god leaves your shield.", "", CMD_COMMUNE
    },

    {
	"shield of justice",
        { 53, 53, 53, 48, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_shield_of_justice,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_shield_of_justice,	SLOT(0),	200,	24,
	"", "You feel less zealous as the power of your god leaves your shield.", "", CMD_COMMUNE
    },

    {
	"shield of truth",
        { 53, 53, 53, 48, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_shield_of_truth,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_shield_of_truth,	SLOT(0),	200,	24,
	"", "You feel more vulnerable as the power of your god leaves your shield.", "", CMD_COMMUNE
    },

    {
	"drive",
        { 53, 53, 53, 48, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_FIGHTING,
	NULL,	SLOT(0),	0,	0,
	"", "", "", CMD_NONE
    },

    {
	"paladin combo counter PC",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_paladin_tech_counter_pc,	SLOT(0),	0,	0,
	"", "Your combat focus against your opponents leaves you.", "", CMD_NONE
    },

    {
	"paladin combo counter NPC",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_paladin_tech_counter_npc,	SLOT(0),	0,	0,
	"", "Your combat focus against the mindless leaves you.", "", CMD_NONE
    },

    {
	"hobble",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_hobble,	SLOT(0),	0,	0,
	"", "You feel able to walk again.", "", CMD_NONE
    },

    {
	"swordplay",
	{ 53, 53, 53, 51, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_swordplay,	SLOT(0),	0,	0,
	"", "", "", CMD_NONE
    },

    {
	"drowning",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_drowning,	SLOT(0),	0,	0,
	"", "", "", CMD_NONE
    },

    {
	"water breathing",
        { 53, 53, 53, 53, 53, 53, 25, 53, 53, 53, 35, 53, 20 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_water_breathing,	TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_water_breathing,	SLOT(100),	15,	9,
	"", "You can no longer breathe underwater.", "", CMD_BOTH
    },

    {
	"darkheart",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_darkheart,	SLOT(0),	0,	0,
	"", "The blackness in your heart wavers.", "", CMD_NONE
    },

    {
	"transform",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_transform,	SLOT(0),	0,	0,
	"", "Your bones snap back into their original locations, leaving you vulnerable.", "", CMD_NONE
    },

    {
	"anti-magic shell",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_anti_magic_shell,	SLOT(0),	0,	0,
	"", "The anti-magic shell around your body fades.", "", CMD_NONE
    },

    {
	"dark essence",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_dark_essence,	SLOT(0),	0,	0,
	"", "Your natural resistance to the Light fades away.", "", CMD_NONE
    },

    {
	"soul channel",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_soul_channel,	SLOT(0),	0,	0,
	"", "You are no longer able to channel your pain through your soul.", "", CMD_NONE
    },

    {
	"ritual of abominations",
	{ 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,	TAR_IGNORE,	POS_STANDING,
	&gsn_ritual_of_abominations,	SLOT(0),	0,	0,
	"", "You feel able to fashion another phylactery from a victim's corpse.", "", CMD_NONE
    },

    {
	"clear brush",
	{ 53, 53, 53, 53, 53, 51, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null, TAR_IGNORE, POS_STANDING,
	NULL, SLOT(0), 0, 24,
	"", "You're ready to clear the brush again.", "", CMD_NONE
    },
    {
	"drag snare",
	{ 53, 53, 53, 53, 53, 51, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null, TAR_IGNORE, POS_STANDING,
	&gsn_dragsnare, SLOT(0), 0, 24,
	"", "You discard your snare.", "", CMD_NONE
    },

    {
	"maledict corpse",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 45, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_maledict_corpse,	TAR_IGNORE,	POS_STANDING,
	NULL,	SLOT(0),	300,	36,
	"", "", "", CMD_SPELL
    },
    {
	"wilderness familiarity",
	{ 53, 53, 53, 53, 53, 51, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null, TAR_IGNORE,	POS_STANDING,
	&gsn_wilderness_fam, SLOT(0), 0, 0,
	"","","", CMD_NONE
    },

    {
	"lich elixir affect",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null, TAR_IGNORE,	POS_STANDING,
	&gsn_lich_elixir_affect, SLOT(0), 0, 0,
	"","","", CMD_NONE
    },

    {
	"age death",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null, TAR_IGNORE,	POS_STANDING,
	&gsn_age_death, SLOT(0), 0, 0,
	"","","", CMD_NONE
    },

    {
	"march",
	{ 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null, TAR_IGNORE,	POS_STANDING,
	&gsn_march, SLOT(0), 0, 12,
	"","The current of wind flowing under you dies down.\nYou float to the ground.","", CMD_NONE
    },

    {
	"critical strike",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null, TAR_IGNORE,	POS_STANDING,
	&gsn_critical_strike, SLOT(0), 0, 0,
	"","The critical strike wound fades away.","", CMD_NONE
    },

    {
	"valiant wrath",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null, TAR_IGNORE,	POS_STANDING,
	&gsn_valiant_wrath, SLOT(0), 0, 0,
	"","The feeling of zeal leaves you.","", CMD_NONE
    },

    {
	"weaponmeld",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null, TAR_IGNORE,	POS_STANDING,
	&gsn_weaponmeld, SLOT(0), 0, 0,
	"","Your weapon detaches itself from your hands.","", CMD_NONE
    },

    {
	"mana conduit",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null, TAR_IGNORE,	POS_STANDING,
	NULL, SLOT(0), 		0, 	0,
	"","","", CMD_NONE
    },

    {
	"mental clarity",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null, TAR_IGNORE,	POS_STANDING,
	NULL, SLOT(0), 		0, 	0,
	"","","", CMD_NONE
    },

    {
	"mana disruption",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_mana_disruption, TAR_IGNORE,	POS_FIGHTING,
	NULL, 	SLOT(0), 	1, 	24,
	"","","", CMD_SPELL
    },

    {
	"mana shield",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_mana_shield, TAR_CHAR_SELF,	POS_STANDING,
	&gsn_mana_shield, SLOT(1005), 100, 24,
	"","You lose your focus on the mana shield surrounding you.","", CMD_SPELL
    },

	{
	"mental exhaustion",
	{ 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null, TAR_IGNORE,	POS_STANDING,
	NULL, SLOT(0), 0, 0,
	"","You feel ready to conjure another mana shield.","", CMD_NONE
    },
	
    {
	"doublecast",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_doublecast, TAR_IGNORE,	POS_FIGHTING,
	NULL, 	SLOT(0), 	50, 	36,
	"","","", CMD_SPELL
    },

    {
	"group gate",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_group_gate, TAR_IGNORE,	POS_STANDING,
	&gsn_group_gate, 	SLOT(1006), 	250, 	24,
	"","","", CMD_BOTH
    },

    {
	"group recall",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_group_recall, TAR_CHAR_DEFENSIVE,	POS_FIGHTING,
	&gsn_group_recall, 	SLOT(1007), 	200, 	24,
	"","","", CMD_BOTH
    },

    {
	"group sanctuary",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_group_sanctuary, TAR_CHAR_DEFENSIVE,	POS_STANDING,
	&gsn_group_sanctuary, 	SLOT(1008), 	350, 	24,
	"","","", CMD_BOTH
    },

    {
	"informant",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_informant,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(0),	80,	24,
	"", "", "", CMD_POWER 
    },

    {
	"hunters strength",
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_hunters_strength,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(0),	100,	12,
	"",  "Your hunter's strength wanes as your concentration wavers.",    "", CMD_POWER
    },

    {
	"hire mercenary",
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_hire_mercenary,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(0),	100,	12,
	"", "The guild will put you in touch with a new mercenary-for-hire.",    "", CMD_POWER
    },

    {
	"hunters vision",
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_hunters_vision,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(0),	75,	12,
	"", "Your predatorial sight dulls as your concentration wavers.",    "", CMD_POWER
    },

    {
	"mystic affinity",
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(0),	0,	0,
	"", "", "", CMD_POWER
    },

    {
	"charge weapon",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 38, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_charge_weapon,	TAR_OBJ_INV,		POS_STANDING,
	&gsn_charge_weapon,	SLOT(0),	85,	12,
	"",	"", "The electricity around $p fades.", CMD_SPELL
    },

    {
	"dent",
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,		POS_FIGHTING,
	&gsn_dent,		SLOT(0),	0,	0,
	"dent",		"",	"", CMD_NONE
    },
  
    {
	"hallow cry",
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_hallow_cry,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(0),	250,	24,
	"", "You are ready to cry out again.", "", CMD_POWER
    },

    {
	"sequester demon",	
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_sequester_demon,	TAR_CHAR_OFFENSIVE,	POS_STANDING,
	NULL,			SLOT(0),	350,	24,
	"", "", "", CMD_SPELL
    },

    {
	"sigil of pain",
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_sigil_pain,	TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	&gsn_sigil_pain,	SLOT(0),	200,	24,
	"", "The terrible sigil ends.", "", CMD_POWER
    },

    {
	"vigilance",
        { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_vigilance,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(0),	35,	12,
	"", "You feel less vigilant.", "", CMD_POWER
    },

    {
	"shield of order",
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_shield_order,	TAR_IGNORE,		POS_STANDING,
	NULL,			SLOT(0),	150,	12,
	"", "The immortal dieties of Enforcer are willing to provide you with another shield.", "", CMD_POWER
    },

    {
	"hand of vengeance",
        { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_hand_of_vengeance, TAR_CHAR_OFFENSIVE,	POS_FIGHTING,
	NULL,			SLOT(0),	25,	12,
	"hand of vengeance",  "", "", CMD_POWER
    },

    {
        "crescent",
        { 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        &gsn_crescent,              SLOT(0),                        0,      0,
        "crescent slash",                 "",     "",     CMD_NONE
    },

    {
        "plant explosive",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        &gsn_plant_explosive,              SLOT(0),                        0,      24,
        "", "You have crafted another explosive to plant.",     "",     CMD_NONE
    },

    {
        "advanced tripwire",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        &gsn_advanced_tripwire,              SLOT(0),                        0,      36,
        "", "", "", CMD_NONE
    },

    {
        "shadowstep",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        &gsn_shadowstep,              SLOT(0),                        0,      0,
        "", "", "", CMD_NONE
    },

    {
        "veil of the opaque",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        &gsn_veil_of_the_opaque,              SLOT(0),                        0,      0,
        "", "", "", CMD_NONE
    },

   {
        "disguise",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        &gsn_disguise,              SLOT(0),                        0,      0,
        "", "Your aged disguise is no longer convincing. You discard it.", "", CMD_NONE
    },
	
	{
        "disguise timer",
       { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        NULL,              SLOT(0),                        0,      0,
        "", "You feel up to fashioning another disguise.", "", CMD_NONE
    },

    {
        "improved blackjack",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        &gsn_improved_blackjack,              SLOT(0),                        0,      0,
        "", "", "", CMD_NONE
    },
	
	{
        "shadowfeed",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        NULL,              SLOT(0),                        0,      0,
        "", "The strength of the shadows leaves you.", "", CMD_NONE
    },
	
	{
        "coalesce resistance",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_coalesce_resistance,             TAR_CHAR_DEFENSIVE,                     POS_STANDING,
        &gsn_coalesce_resistance,              SLOT(0),                        300,      24,
        "", "Your resistances fade.", "", CMD_COMMUNE
    },
	
	{
        "stance",
        { 52, 52, 52, 52, 52, 50, 52, 52, 52, 52, 52, 52, 52 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        &gsn_stance,              SLOT(0),                        0,      24,
        "", "", "", CMD_NONE
    },
	
	{
        "bulwark of blades",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_bulwark_of_blades,             TAR_CHAR_DEFENSIVE,                     POS_STANDING,
        &gsn_bulwark_of_blades,              SLOT(0),                        150,      12,
        "bulwark of blades", "The barrier of blades around you fades away.", "", CMD_COMMUNE
    },
	
	{
        "dicing wounds",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        NULL,              SLOT(0),                        0,      0,
        "", "Your diced flesh has healed.", "", CMD_NONE
    },
	
	{
        "blade mastery",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        &gsn_blade_mastery,              SLOT(0),                        0,      0,
        "", "", "", CMD_NONE
    },
	
	{
        "divine transferrance",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_divine_transferrance,             TAR_CHAR_DEFENSIVE,                     POS_STANDING,
        &gsn_divine_transferrance,              SLOT(0),                        400,      12,
        "", "", "", CMD_COMMUNE
    },
	
	{
        "divine transferrance timer",
       { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_CHAR_DEFENSIVE,                     POS_STANDING,
        NULL,              SLOT(0),                       0,      0,
        "", "Your ability to bond with others has been restored by your faith.", "", CMD_NONE
    },
	
	{
        "sacrifice",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        &gsn_statue_sacrifice,              SLOT(0),                        0,      0,
        "", "The gods are willing to heed your sacrifices once again.", "", CMD_NONE
    },
	
	{
        "divine transferrer",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        NULL,              SLOT(0),                        0,      0,
        "", "Your divine bond breaks and fades away.", "", CMD_NONE
    },
	
	{
        "divine transferree",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        NULL,              SLOT(0),                        0,      0,
        "", "Your divine bond breaks and fades away.", "", CMD_NONE
    },
	
	{
        "greater regeneration",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        &gsn_greater_regeneration,              SLOT(0),                       0,      0,
        "", "", "", CMD_NONE
    },
	
	{
        "resurrect",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_resurrect,             TAR_IGNORE,                     POS_STANDING,
        &gsn_resurrect,              SLOT(0),                        1500,      60,
        "", "", "", CMD_BOTH
    },
	
	{
        "resurrect timer",
        { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        NULL,              SLOT(0),                        0,      0,
        "", "Your spirit has regained the strength to resurrect.", "", CMD_NONE
    },

   {
        "intercept",
        { 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_STANDING,
        &gsn_intercept,        SLOT( 0),       0,      0,
        "",                     "!intercept!",         "", CMD_NONE
    },
	
	{
        "maelstrom",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_maelstrom,             TAR_CHAR_OFFENSIVE,                     POS_FIGHTING,
        &gsn_maelstrom,        SLOT( 0),       150,      24,
        "elemental maelstrom",                     "",         "", CMD_BOTH
    },
	
	{
        "bastion of elements",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_bastion_of_elements,   TAR_CHAR_DEFENSIVE,                     POS_STANDING,
        &gsn_bastion_of_elements,        SLOT( 0),       250,      24,
        "",                     "Your bastion of the elements fades away.",         "", CMD_SPELL
    },
	
	{
        "hurricane",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_hurricane,   TAR_CHAR_OFFENSIVE,                     POS_FIGHTING,
        &gsn_hurricane,        SLOT( 0),       500,      24,
        "",                     "You feel able to summon another hurricane.",         "", CMD_SPELL
    },

	{
        "elemental affinity",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,   TAR_IGNORE,                     POS_STANDING,
        &gsn_elemental_affinity,        SLOT( 0),       0,      0,
        "",                     "",         "", CMD_NONE
    },

	{
        "shatter armor",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_shatter_armor,   TAR_CHAR_OFFENSIVE,                     POS_FIGHTING,
        &gsn_shatter_armor,        SLOT( 0),       100,      12,
        "shatter armor",                     "Your armor regains its effectiveness.",         "", CMD_BOTH
    },
	
	{
        "vorpal bolt",
        { 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_vorpal_bolt,   TAR_CHAR_OFFENSIVE,                     POS_FIGHTING,
        &gsn_vorpal_bolt,        SLOT( 0),       150,      24,
        "vorpal bolt",                     "",         "", CMD_BOTH
    },
	
	{
        "vorpal bolt arm",
         { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,   TAR_IGNORE,                     POS_FIGHTING,
        NULL,        SLOT( 0),       0,      0,
        "",                     "The burns on your arms heal and you regain your strength.",         "", CMD_NONE
    },
	
	{
        "vorpal bolt leg",
         { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,   TAR_IGNORE,                     POS_FIGHTING,
        NULL,        SLOT( 0),       0,      0,
        "",                     "The burns on your legs heal and you regain your dexterity.",         "", CMD_NONE
    },
	
	{
        "vorpal bolt head",
         { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,   TAR_IGNORE,                     POS_FIGHTING,
        NULL,        SLOT( 0),       0,      0,
        "",                     "The burns on your head heal and your mind becomes clearer.",         "", CMD_NONE
    },
	
	{
        "vorpal bolt chest",
         { 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53, 53 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,   TAR_IGNORE,                     POS_FIGHTING,
        NULL,        SLOT( 0),       0,      0,
        "",                     "The burns on your chest heal.",         "", CMD_NONE
    },

    {
	"exude health",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_exude_health,    TAR_OBJ_INV,                    POS_STANDING,
	&gsn_exude_health,                   SLOT(0),      350,    36,
	"",                     "Your magical boost of health weakens.",      "", CMD_SPELL
    },

    {
	"vitality",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_null,		TAR_IGNORE,			POS_RESTING,
	&gsn_vitality,		SLOT(0),		0,	0,
	"",			"Your added vitality fades.",	"",	CMD_NONE
    },

    {
	"dragon might",
	{ 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51 },
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        spell_null,             TAR_IGNORE,                     POS_RESTING,
        &gsn_dragon_might,	SLOT(0),                0,      0,
        "",                     "The might of the dragon-kind leaves you.",   "",     CMD_NONE
    },

    {
	"ward of magnetism",
	{ 53, 53, 53, 53, 53, 53, 53, 46, 53, 53, 53, 53, 53 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	spell_ward_of_magnetism,	TAR_CHAR_SELF,		POS_STANDING,
	NULL,			SLOT(0),	150,	24,
	"",	"Your ward of magnetism fades, leaving you able to handle metal objects again.", "", CMD_SPELL
    }

};

const   struct  group_type      group_table     [MAX_GROUP]     =
{

    {
        "class basics",       { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { "sword", "axe", "dagger", "flail", "mace", "polearm",
        "shield block", "spear", "whip", "bash", "berserk", "dirt kicking",
        "disarm", "dodge", "enhanced damage", "hand to hand", "kick",
        "parry", "rescue", "trip", "second attack", "third attack",
  	"firestream", "fireshield", "iceshield", "dark dream", "embalm",
	"shadowself", "bless weapon", "barkskin","alarm","nightwalk",
	"blade barrier", "warcry", "lash", "chain lightning",
	"circle stab", "turn undead", "aura", "concatenate", "camouflage", "ambush",
	"frostbolt",	"icelance",	"earthfade", "forget", "earthbind",
	"divine touch", "transfer object", "disintergrate",
	"grounding", "shock sphere", "forage", "fwood", "assassinate",
	"staff", "pugil", "enhanced damage II", "animate skeleton",
	"mummify", "protection heat cold", "transfer object",
	"defend", "escape", "moving ambush", "soulbind", "banshee call",
	"absorb","wrath", "wrath of purity", "dark wrath", "acute vision",
        "laying hands", "light of heaven", "zealotism",
	"dual backstab", "power word kill", "power word stun", "evil eye",
	"animate dead", "skin", "camp", "fourth attack",
	"animate object", "demand", "lifebane", "deathspell",
	"morph white dragon", "morph black dragon", "morph blue dragon",
	"morph green dragon", "morph red dragon", "weapon trip",
	"breath dragon", "summon fire elemental", "summon water elemental",
	"crush", "ground control", "palm","endure", "follow through",
	"throw", "nerve", "poisondust", "blindness dust",
        "block", "roll", "garrotte", "spike", "acid needles",
	"summon air elemental", "summon earth elemental",
	"earthmaw", "disenchant armor", "disenchant weapon","dual wield",
 	"windwall", "butcher","shadowself", "bark skin", "utter heal",
	"cone of cold", "timestop", "icestorm", "raise hell",  "consecrate",
    	"wither", "moonshine", "shadowstrike", "nightmare", "slip",
	"protective shield", "animal call", "lunge", "blackjack",
	"trance", "herb", "bearcharge", "cleave","iceball",
	"lesser golem","ranger staff", "clown mallet", "greater golem",
	"preserve", "power word fear", "prevent healing",
	"decay corpse", "imbue regeneration", "restoration",
	"undead drain", "atrophy", "fire and ice",
	"preparation","bear call", "enlist", "downstrike", "spellcraft",
	"tame", "find water", "shield cleave", 
	"curse weapon", "focus","rejuvenate",
	"recharge", "assess", "shieldbash", "righteous warfare", "cross slash",
        "fast healing", "haggle", "meditation", "recall", "backstab",
	"envenom", "camouflage", "hide", "peek", "pick lock", "sneak", "steal", "scrolls",
 	"acid blast", "armor", "bless", "icy wind", "electric storm", "blindness",
	"burning hands", "call lightning", "calm", "cancellation", "colour spray",
	"continual light", "control weather", "create food", "create rose",
	"create spring", "create water", "cure blindness", "cure critical",
	"cure disease", "cure light", "cure poison", "cure serious", "curse",
	"demonfire", "detect evil", "detect good", "detect hidden",
	"detect invis", "detect magic", "detect poison", "dispel evil",
	"dispel good", "dispel magic", "earthquake", "enchant armor",
	"enchant weapon", "energy drain", "faerie fire", "faerie fog",
	"farsight", "fireball", "fireproof", "flamestrike", "fly", "frenzy",
	"gate", "giant strength", "harm", "haste", "heal",
	"holy word", "identify", "infravision", "invisibility", "know alignment",
	"lightning bolt", "locate object", "magic missile", "mass healing",
	"mass invis", "pass door", "plague", "poison", "protection evil",
	"protection good", "ray of truth", "refresh", "remove curse",
	"sanctuary", "shield", "shocking grasp", "sleep", "slow",
	"stone skin", "summon", "teleport", "ventriloquate", "weaken",
	"word of recall", "cause critical", "cause light", "cause serious",
	"change sex", "chill touch", "sunray",
	"ice pox", "hold person", "lore","wraithform","knife",
	"cheap shot", "binds", "deafen","creep", "parting blow",
        "snare", "shield of bubbles", "charm person",
        "shield of dust","shield of flames", "shield of wind", "blizzard",
        "shield of frost", "shield of lightning", "hydrolic ram" ,
        "spontanious combustion", "shockwave", "static charge",
        "smoke", "ice slick", "rising pheonix kick", "icy shards",
        "hellfire", "ball lightning", "incandescense", "righteousness",
        "scorch", "looking glass", "vanish", "acid spit", "knock",
	"throwing knife", "poisonous flower", "siton",
	"thrknife", "barrier", "dark summons", "hex", "kotegaeshi", 
	"flight", "titans", "dew", "protection", "storms", "balmy sleep",
	"deafening pitch", "healing", "insanity", "chilliness of death", "fatigue", "rot",
	"cure deafness","awaken", "resist heat","resist cold","resist lightning",
	"resist mental","resist acid","resist negative", "resist paralysis",
	"group teleport", "remove paralysis", "assess", "cleanse", "strength of faith",
	"resist weapon", "resting", "armageddon", "hour of rising", "stonelike resolve", "phantom",
	"soften", "guarding", "divine blessing", "duo dimension",
	"fumble", "protection", "prismatic spray","gag", "owaza", "axe kick", "crescent kick",
	"scissor kick", "tigerclaw", "unarmed defense", "unarmed offense", "backfist", "carve", "haunting",
	"healing sleep", "detect illusion", "displacement", "heightened awareness", "backfist",
	"double spin kick", "dash", "bindwounds", "plant growth", "spores",
	"spire of flames", "worldbind", "aura", "darkshout",
	"enlarge", "reduce", "water breathing", "tripwire", "self immolation", "stance",
	"maledict corpse", "charge weapon", "unholy bless", "wilderness familiarity", "ward of magnetism" }
    },

    {
        "class default",      { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { "" }
    },

/* You don't need to group Cabal abilities here, the assignment of these
are now done in act_wiz.c's do_induct() function. These entries just make
it easier to see what abilities each cabal gets. (Ceran)
*/
    {
        "ancient", { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { "eye of the predator", "eyes of intrigue", "mantle of maehslin", 
	  "shadowgate", "shroud", "cloak", "eavesdrop" }
    },

    {
        "knight",    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { "lightwalk","dragonplate", "dragonweapon", 
	  "blademaster", "truestrike" }
    },

    {
        "arcana",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { "channel", "venueport", "scourge", "scrying", "counterspell" }
    },

    {
        "rager",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { "spellbane", "resistance", "bandage", "trophy", "bloodthirst", "blitz", "true sight", "hallow cry" }
    },

    {
        "enforcer", { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"guard call", "shackles", "vigilance", "shield of order", "hand of vengeance" }
    },

    {
        "outlaw",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ "decoy", "disperse", "chaos blade", "randomizer",
	  "garble", "confuse", "chromatic fire" }
    },

    {
        "empire",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { "nightfist", "shove", "devour", "centurions", 
	  "black shroud", "imperial training", "sigil of pain" }
    },

    {
        "bounty",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { "informant", "hunters strength", "hire mercenary"}
    },

    {
        "sylvan",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { "chameleon", "insectswarm", "beast call",
          "spiderhands", "wall of thorns", "pathfinding"}
    },

    {
        "hunter", {0,0,0,0,0,0,0,0,0,0,0}, 
        { "web" }
    },
	
    {
		"master", {0,0,0,0,0,0,0,0,0,0,0},
		{ "hunters vision" }
    },
	
    {
        "sensei", {0,0,0,0,0,0,0,0,0,0,0},
        { "" }
    },

/* Lich */

    {
        "lich",    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"minion tactics", "minion sanc", "minion flight", "minion sneak",
         "minion recall", "minion haste", "minion trans", "vampiric touch",
	 "ritual of abominations" }
    },

/* swarrior */
     {
        "ws hand to hand",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"ironhands", "pummel", "crushing blow", "stun"}
    },
    {
        "ws sword",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"flourintine","flurry","crossslice"}
    },
    {
        "ws dagger",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"stab","gouge","restrike"}
    },
    {
        "ws spear",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"spin","impale", "legsweep", "crescent"}
    },
    {
        "ws mace",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"boneshatter","drum", "cranial", "dent"}
    },
    {
        "ws axe",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"pincer","weaponbreaker", "whirl"}
    },
    {
        "ws flail",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"strip","choke", "entwine"}
    },
    {
        "ws whip",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"strip","choke", "entwine"}
    },
    {
        "ws polearm",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"distance","charge set", "charge", "chop"}
    },
    {
        "ws staff",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"spin", "impale", "legsweep"}
    },
	/* THIS NEXT ONE IS 23 */
    {
        "mob sword",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"flourintine","flurry","crossslice","",""}
    },
    {
        "mob axe",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"pincer","overhead","weaponbreaker","",""}
    },
    {
        "mob hand to hand",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"ironhands","pummel","stun","crushing blow",""}
    },
    {
        "mob dagger",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"stab","gouge","restrike","",""}
    },
    {
        "mob spear",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"spin","impale","legsweep","crescent",""}
    },
    {
        "mob mace",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"boneshatter","drum","cranial","",""}
    },
    {
        "mob whip",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"strip","choke","lash","",""}
    },
    {
        "mob polearm",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        {"distance","charge set","legsweep","cutoff",""}
    },

    {
        "ded shield",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { "shield of truth", "shield of justice", "shield of redemption",
          "shield of purity" }
    },

    {
        "ded sword",  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        { "swordplay", "drive" }
    },
};

