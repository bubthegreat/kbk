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

/* for cabals 
 *
 * NEVER set the max number to 0, ever, ever, ever or we won't boot until you change it
 * back. This goes all the way back to math class when you leanred about dividing with zero
 */

const struct cabal_type cabal_table[MAX_CABAL] =
{
    /*  name,		who entry,	death room,	      max ppl,independent, open/closed */
    /* independent should be FALSE if is a real cabal */
    {   "",             "",              "",	ROOM_VNUM_ALTAR,       
    	1,	1,	TRUE , FALSE },
    {   "ancient",      "[ANCIENT] ",    "Masters of Intrigue",	
    	ROOM_VNUM_ANCIENT,     3801,	0,	0,	50,	FALSE, FALSE, 	TRUE },
    	
    {   "knight",       "[KNIGHT] ",     "Knights of Thera",	
    	ROOM_VNUM_KNIGHT,      4502,	0,	0,	50,	FALSE, FALSE, 	TRUE },
    	
    {   "arcana",       "[ARCANA] ",     "Masters of the Five Magics",	
    	ROOM_VNUM_ARCANA,      5801,	0,	0,	50,	FALSE, FALSE, 	TRUE  },
    	
    {   "rager",        "[RAGER] ",      "Battleragers, Haters of Magic",	
    	ROOM_VNUM_RAGER,       5701,	0,	0,	50,	FALSE, FALSE,	TRUE },
    	
    {   "outlaw",       "[OUTLAW] ",     "Outlaw, Barons of Chaos",	
    	ROOM_VNUM_OUTLAW,      9902,	0,	0,	50,	FALSE, FALSE,	TRUE  },
    	
    {   "empire",       "[EMPIRE] ",     "Empire, Subjugator of Nations",	
    	ROOM_VNUM_ALTAR,       8101,	0,	0,	50,	FALSE, FALSE,	TRUE },
    	
    {   "bounty",	"[BOUNTY] ",	"Bounty Hunters",	
    	ROOM_VNUM_ALTAR,       8800,	0,	0,	50,	FALSE, FALSE,	TRUE  },

    {   "sylvan",	"[SYLVAN] ",	"Sylvan Warders of the Glades",	
    	ROOM_VNUM_ALTAR,       8900,	0,	0,	50,	FALSE, FALSE,	TRUE },

    {
	"enforcer",	"[ENFORCER] ",	"Enforcers of Justice",
	ROOM_VNUM_ALTAR,	4521,	0,	0,	50,	FALSE, FALSE, 	TRUE }
};


const struct cabal_restr_type cabal_restr_table[MAX_CABAL] = {
	{	// None
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		ALIGN_NONE,
		ETHOS_NONE
	},

	{	// Ancient
		{1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
		{0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0},
		ALIGN_E,
		ETHOS_ANY
	},

	{	// Knight
		{1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
		{0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
		ALIGN_G,
		ETHOS_ANY,
	},

	{	// Arcana
		{0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0},
		{0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0},
		ALIGN_ANY,
		ETHOS_ANY,
	},

	{	// Rager
		{1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0},
		ALIGN_ANY,
		ETHOS_ANY
	},

	{	// Outlaw
		{1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0},
		ALIGN_ANY,
		ETHOS_C
	},

	{	// Empire
		{1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
		{0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0},
		ALIGN_E,
		ETHOS_L
	}, 

	{	// Bounty
		{1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
		ALIGN_ANY,
		ETHOS_ANY
	},

	{	// Sylvan
		{1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1},
		{0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
		ALIGN_GN,
		ETHOS_NC
	},

	{	// Enforcer
		{1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
		ALIGN_ANY,
		ETHOS_L
	}
};

const struct cabal_message cabal_messages[MAX_CABAL] =
{
	// login, logout, entry message //
    { 	"", "", "" },
    { 	"%s has joined the Hunt.", 
	"%s has left from the Hunt.",
	"May the darkness conceal you." },
    { 	"The Code of Honor gleams as %s enters the Castle.",
	"Farewell %s, the crusades await your return.",
	"Greetings, brother Knight." },
    { 	"The Orb of Magic hums briefly as %s enters the lands.",
	"The crystal of power dims briefly as %s leaves the lands.",
	"Greetings, Master of Magic." },
    { 	"WAAAAAAAAAAAAAZZZZZZZZAAAAAAAAA WARRIOR OF THE VILLAGE?!?!!?!?",
	"Farewell %s. May your strength return to us soon.",
	"Welcome, great warrior." },
    { 	"The encampment of Outlaws grows as %s enters the realm.",
	"%s disbands from the outlaw encampments for a while.",
	"Greetings, chaotic one." },
    { 	"Imperial power grows as %s enters our realm.",
	"Our conquest is yet assured though %s leaves us.",
	"Greetings, citizen." },
    {   "%s has joined the watch.",
        "%s has left the watch.",
        "Hail, Hunter." },
    {   "Welcome, %s, protector of the wilderness.",
	"The Grove will remain steadfast in your absence, %s.",
	"Nature welcomes you."},
    {	"The law grows stronger with the arrival of %s.",
	"%s signs off from duty and leaves the chaotic streets.",
	"Greetings, Enforcer of the Law."
    }
};

const struct hometown_type hometown_table[MAX_HOMETOWN] =
{
	/* City Name		Recall	Pit	Align Restrict		Ethos Restrict	Race/Class Restrict */
	{"",			0,	0,	ALIGN_NONE,		ETHOS_NONE,		0	},
	{"Midgaard",		3054,	3010,	ALIGN_ANY,		ETHOS_ANY,		0	},
	{"New Thalos",		9609,	9603,	ALIGN_ANY,		ETHOS_ANY,		0	},
	{"Arkham",		768,	700,	ALIGN_E,		ETHOS_ANY,		0	},
	{"Naiadia",		20463,	20425,	ALIGN_ANY,		ETHOS_ANY,		0	},
	{"Asgard",		1213,	1200,	ALIGN_NONE,		ETHOS_NONE,		0	},

};

/* for position */
const struct position_type position_table[] =
{
    {	"dead",			"dead"	},
    {	"mortally wounded",	"mort"	},
    {	"incapacitated",	"incap"	},
    {	"stunned",		"stun"	},
    {	"sleeping",		"sleep"	},
    {	"resting",		"rest"	},
    {   "sitting",		"sit"   },
    {	"fighting",		"fight"	},
    {	"standing",		"stand"	},
    {	NULL,			NULL	}
};

/* for sex */
const struct sex_type sex_table[] =
{
   {	"none"		},
   {	"male"		},
   {	"female"	},
   {	"either"	},
   {	NULL		}
};

/* for sizes */
const struct size_type size_table[] =
{
    {	"tiny"		},
    {	"small" 	},
    {	"medium"	},
    {	"large"		},
    {	"huge", 	},
    {	"giant" 	},
    {	NULL		}
};

/* various flag tables */
const struct flag_type act_flags[] =
{
    {	"npc",			A,	FALSE	},
    {	"sentinel",		B,	TRUE	},
    {	"scavenger",		C,	TRUE	},
    {	"inner_guardian",	D,	TRUE	},
    {	"aggressive",		F,	TRUE	},
    {	"stay_area",		G,	TRUE	},
    {	"wimpy",		H,	TRUE	},
    {	"pet",			I,	TRUE	},
    {	"train",		J,	TRUE	},
    {	"practice",		K,	TRUE	},
    {   "no_track",		L,	TRUE	},
    {	"undead",		O,	TRUE	},
    {	"cleric",		Q,	TRUE	},
    {	"mage",			R,	TRUE	},
    {	"thief",		S,	TRUE	},
    {	"warrior",		T,	TRUE	},
    {	"noalign",		U,	TRUE	},
    {	"nopurge",		V,	TRUE	},
    {	"outdoors",		W,	TRUE	},
    {	"indoors",		Y,	TRUE	},
    {   "outer_guardian",	Z,	TRUE	},
    {	"healer",		aa,	TRUE	},
    {	"gain",			bb,	TRUE	},
    {	"update_always",	cc,	TRUE	},
    {	"changer",		dd,	TRUE	},
    {	NULL,			0,	FALSE	}
};

const struct flag_type plr_flags[] =
{
    {	"npc",			A,	FALSE	},
    {	"autoassist",		C,	FALSE	},
    {	"autoexit",		D,	FALSE	},
    {	"autoloot",		E,	FALSE	},
    {	"autosac",		F,	FALSE	},
    {	"autogold",		G,	FALSE	},
    {	"autosplit",		H,	FALSE	},
    {   "color",                I,      FALSE   },
    {   "betrayer",             K,      TRUE    },
    {   "heroimm",              M,      TRUE    },
    {	"holylight",		N,	FALSE	},
    {	"empowered",		O,	FALSE	},
    {	"can_loot",		P,	FALSE	},
    {	"nosummon",		Q,	FALSE	},
    {	"nofollow",		R,	FALSE	},
    {	"permit",		U,	TRUE	},
    {	"moron",		V,	FALSE	},
    {	"log",			W,	FALSE	},
    {	"deny",			X,	FALSE	},
    {	"freeze",		Y,	FALSE	},
    {	"thief",		Z,	FALSE	},
    {	"killer",		aa,	FALSE	},
    {	NULL,			0,	0	}
};

const struct flag_type affect_flags[] =
{
    {   "blind",                AFF_BLIND,      	TRUE    },
    {   "invisible",            AFF_INVISIBLE,      	TRUE    },
    {   "detect_evil",          AFF_DETECT_EVIL,      	TRUE    },
    {   "detect_invis",         AFF_DETECT_INVIS,      	TRUE    },
    {   "detect_magic",         AFF_DETECT_MAGIC,      	TRUE    },
    {   "detect_hidden",        AFF_DETECT_HIDDEN,      TRUE    },
    {   "detect_good",          AFF_DETECT_GOOD,      	TRUE    },
    {   "sanctuary",            AFF_SANCTUARY,      	TRUE    },
    {   "faerie_fire",          AFF_FAERIE_FIRE,      	TRUE    },
    {   "infrared",             AFF_INFRARED,      	TRUE    },
    {   "curse",                AFF_CURSE,      	TRUE    },
    {   "poison",               AFF_POISON,      	TRUE    },
    {   "protect_evil",         AFF_PROTECT_EVIL,      	TRUE    },
    {   "protect_good",         AFF_PROTECT_GOOD,      	TRUE    },
    {   "sneak",                AFF_SNEAK,      	TRUE    },
    {   "hide",                 AFF_HIDE,      		TRUE    },
    {   "sleep",                AFF_SLEEP,      	TRUE    },
    {   "charm",                AFF_CHARM,      	TRUE    },
    {   "flying",               AFF_FLYING,      	TRUE    },
    {   "pass_door",            AFF_PASS_DOOR,      	TRUE    },
    {   "haste",                AFF_HASTE,      	TRUE    },
    {   "calm",                 AFF_CALM,      		TRUE    },
    {   "plague",               AFF_PLAGUE,      	TRUE    },
    {   "weaken",               AFF_WEAKEN,      	TRUE    },
    {   "dark_vision",          AFF_DARK_VISION,      	TRUE    },
    {   "berserk",              AFF_BERSERK,     	TRUE    },
    {   "waterbreathing",       AFF_WATERBREATHING,	TRUE    },
    {   "regeneration",         AFF_REGENERATION,     	TRUE    },
    {   "slow",                 AFF_SLOW,     		TRUE    },
    {   "camouflage",           AFF_CAMOUFLAGE,     	TRUE    },
    {   NULL,                   0,      		0       }
};

const struct flag_type off_flags[] =
{
    {   "area_attack",		OFF_AREA_ATTACK,	TRUE    },
    {   "backstab",             OFF_BACKSTAB,      	TRUE    },
    {   "bash",                 OFF_BASH,      		TRUE    },
    {   "berserk",              OFF_BERSERK,      	TRUE    },
    {   "disarm",               OFF_DISARM,      	TRUE    },
    {   "dodge",                OFF_DODGE,      	TRUE    },
    {   "fade",                 OFF_FADE,      		TRUE    },
    {   "fast",                 OFF_FAST,      		TRUE    },
    {   "kick",                 OFF_KICK,      		TRUE    },
    {   "dirt_kick",            OFF_KICK_DIRT,      	TRUE    },
    {   "parry",                OFF_PARRY,     	 	TRUE    },
    {   "rescue",               OFF_RESCUE,      	TRUE    },
    {   "tail",                 OFF_TAIL,      		TRUE    },
    {   "trip",                 OFF_TRIP,      		TRUE    },
    {   "crush",                OFF_CRUSH,      	TRUE    },
    {   "assist_all",           ASSIST_ALL,      	TRUE    },
    {   "assist_align",         ASSIST_ALIGN,      	TRUE    },
    {   "assist_race",          ASSIST_RACE,      	TRUE    },
    {   "assist_players",       ASSIST_PLAYERS,      	TRUE    },
    {   "assist_guard",         ASSIST_GUARD,      	TRUE    },
    {   "assist_vnum",          ASSIST_VNUM,      	TRUE    },
    {	"static_tracking",	STATIC_TRACKING,	TRUE	},
    {	"spam_murder",		SPAM_MURDER,		TRUE	},
    {	"attack_evil",		OFF_ATTACK_EVIL,	TRUE	},
    {	"attack_good",		OFF_ATTACK_GOOD,	TRUE	},
    {	NULL,			0,			0	}
};

const struct flag_type imm_flags[] =
{
    {	"summon",		A,	TRUE	},
    {	"charm",		B,	TRUE	},
    {	"magic",		C,	TRUE	},
    {	"weapon",		D,	TRUE	},
    {	"bash",			E,	TRUE	},
    {	"pierce",		F,	TRUE	},
    {	"slash",		G,	TRUE	},
    {	"fire",			H,	TRUE	},
    {	"cold",			I,	TRUE	},
    {	"lightning",		J,	TRUE	},
    {	"acid",			K,	TRUE	},
    {	"poison",		L,	TRUE	},
    {	"negative",		M,	TRUE	},
    {	"holy",			N,	TRUE	},
    {	"energy",		O,	TRUE	},
    {	"mental",		P,	TRUE	},
    {	"disease",		Q,	TRUE	},
    {	"drowning",		R,	TRUE	},
    {	"light",		S,	TRUE	},
    {	"sound",		T,	TRUE	},
    {	"wood",			X,	TRUE	},
    {	"silver",		Y,	TRUE	},
    {	"iron",			Z,	TRUE	},
    {	NULL,			0,	0	}
};

const struct flag_type form_flags[] =
{
    {	"edible",		FORM_EDIBLE,		TRUE	},
    {	"poison",		FORM_POISON,		TRUE	},
    {	"magical",		FORM_MAGICAL,		TRUE	},
    {	"instant_decay",	FORM_INSTANT_DECAY,	TRUE	},
    {	"other",		FORM_OTHER,		TRUE	},
    {	"animal",		FORM_ANIMAL,		TRUE	},
    {	"sentient",		FORM_SENTIENT,		TRUE	},
    {	"undead",		FORM_UNDEAD,		TRUE	},
    {	"construct",		FORM_CONSTRUCT,		TRUE	},
    {	"mist",			FORM_MIST,		TRUE	},
    {	"intangible",		FORM_INTANGIBLE,	TRUE	},
    {	"biped",		FORM_BIPED,		TRUE	},
    {	"centaur",		FORM_CENTAUR,		TRUE	},
    {	"insect",		FORM_INSECT,		TRUE	},
    {	"spider",		FORM_SPIDER,		TRUE	},
    {	"crustacean",		FORM_CRUSTACEAN,	TRUE	},
    {	"worm",			FORM_WORM,		TRUE	},
    {	"blob",			FORM_BLOB,		TRUE	},
    {	"mammal",		FORM_MAMMAL,		TRUE	},
    {	"bird",			FORM_BIRD,		TRUE	},
    {	"reptile",		FORM_REPTILE,		TRUE	},
    {	"snake",		FORM_SNAKE,		TRUE	},
    {	"dragon",		FORM_DRAGON,		TRUE	},
    {	"amphibian",		FORM_AMPHIBIAN,		TRUE	},
    {	"fish",			FORM_FISH ,		TRUE	},
    {	"cold_blood",		FORM_COLD_BLOOD,	TRUE	},
    {	NULL,			0,			0	}
};

const struct flag_type part_flags[] =
{
    {	"head",			PART_HEAD,		TRUE	},
    {	"arms",			PART_ARMS,		TRUE	},
    {	"legs",			PART_LEGS,		TRUE	},
    {	"heart",		PART_HEART,		TRUE	},
    {	"brains",		PART_BRAINS,		TRUE	},
    {	"guts",			PART_GUTS,		TRUE	},
    {	"hands",		PART_HANDS,		TRUE	},
    {	"feet",			PART_FEET,		TRUE	},
    {	"fingers",		PART_FINGERS,		TRUE	},
    {	"ear",			PART_EAR,		TRUE	},
    {	"eye",			PART_EYE,		TRUE	},
    {	"long_tongue",		PART_LONG_TONGUE,	TRUE	},
    {	"eyestalks",		PART_EYESTALKS,		TRUE	},
    {	"tentacles",		PART_TENTACLES,		TRUE	},
    {	"fins",			PART_FINS,		TRUE	},
    {	"wings",		PART_WINGS,		TRUE	},
    {	"tail",			PART_TAIL,		TRUE	},
    {	"claws",		PART_CLAWS,		TRUE	},
    {	"fangs",		PART_FANGS,		TRUE	},
    {	"horns",		PART_HORNS,		TRUE	},
    {	"scales",		PART_SCALES,		TRUE	},
    {	"tusks",		PART_TUSKS,		TRUE	},
    {	NULL,			0,			0	}
};

const struct flag_type comm_flags[] =
{
    {	"quiet",		COMM_QUIET,		TRUE	},
    {   "deaf",			COMM_DEAF,		TRUE	},
    {   "nowiz",		COMM_NOWIZ,		TRUE	},
    {   "noauction",            COMM_NOAUCTION,         TRUE    },
    {   "noquestion",		COMM_NOQUESTION,	TRUE	},
    {   "nocabal",              COMM_NOCABAL,           TRUE    },
    {   "noquote",		COMM_NOQUOTE,		TRUE	},
    {   "shoutsoff",		COMM_SHOUTSOFF,		TRUE	},
    {   "compact",		COMM_COMPACT,		TRUE	},
    {   "brief",		COMM_BRIEF,		TRUE	},
    {   "prompt",		COMM_PROMPT,		TRUE	},
    {   "combine",		COMM_COMBINE,		TRUE	},
    {   "telnet_ga",		COMM_TELNET_GA,		TRUE	},
    {   "show_affects",		COMM_SHOW_AFFECTS,	TRUE	},
    {   "noemote",		COMM_NOEMOTE,		FALSE	},
    {   "noshout",		COMM_NOSHOUT,		FALSE	},
    {   "notell",		COMM_NOTELL,		FALSE	},
    {   "nochannels",		COMM_NOCHANNELS,	FALSE	},
    {   "snoop_proof",		COMM_SNOOP_PROOF,	FALSE	},
    {   "afk",			COMM_AFK,		TRUE	},
    {	"nosocket",		COMM_NOSOCKET,		FALSE	},
    { 	"noglobal",		COMM_NOGLOBAL,		TRUE	},
    {	"allcabal",		COMM_ALL_CABALS,	TRUE	},
    {	NULL,			0,			0	}
};

/***************************** OLC TABLES BELOW **********************************/

const struct flag_type area_flags[] =
 {
     {	"none",			AREA_NONE,		FALSE	},
     {	"changed",		AREA_CHANGED,		TRUE	},
     {	"added",		AREA_ADDED,		TRUE	},
     {	"loading",		AREA_LOADING,		FALSE	},
     {	"closed",		AREA_CLOSED,		TRUE	},
     {	NULL,			0,			0	}
 };



const struct flag_type sex_flags[] =
 {
     {	"male",			SEX_MALE,		TRUE	},
     {	"female",		SEX_FEMALE,		TRUE	},
     {	"neutral",		SEX_NEUTRAL,		TRUE	},
     {   "random",               3,                      TRUE    },   /* ROM */
     {	"none",			SEX_NEUTRAL,		TRUE	},
     {	NULL,			0,			0	}
 };

const struct flag_type direction_table [] =
{
	{"north",	DIR_NORTH,	TRUE },
	{"east",	DIR_EAST,	TRUE },
	{"south",  	DIR_SOUTH, 	TRUE },
	{"west",	DIR_WEST,	TRUE },
	{"up",		DIR_UP,		TRUE },
	{"down",	DIR_DOWN,	TRUE },
	{NULL,		0,			TRUE },
};

const struct flag_type exit_flags[] =
 {
    {   "door",			EX_ISDOOR,		TRUE    },
    {	"closed",		EX_CLOSED,		TRUE	},
    {	"locked",		EX_LOCKED,		TRUE	},
    {	"pickproof",		EX_PICKPROOF,		TRUE	},
    {   "nopass",		EX_NOPASS,		TRUE	},
    {   "easy",			EX_EASY,		TRUE	},
    {   "hard",			EX_HARD,		TRUE	},
    {	"infuriating",		EX_INFURIATING,		TRUE	},
    {	"noclose",		EX_NOCLOSE,		TRUE	},
    {	"nolock",		EX_NOLOCK,		TRUE	},
    {	"nobash",		EX_NOBASH,		TRUE	},
    {	"nonobvious",		EX_NONOBVIOUS,		TRUE	},
    {	"current",		EX_CURRENT,		TRUE	},
    {	NULL,			0,			0	}
 };



const struct flag_type door_resets[] =
 {
     {	"open and unlocked",	0,		TRUE	},
     {	"closed and unlocked",	1,		TRUE	},
     {	"closed and locked",	2,		TRUE	},
     {	NULL,			0,		0	}
 };



const struct flag_type room_flags[] =
 {
     {	"dark",			ROOM_DARK,		TRUE	},
     {	"no_mob",		ROOM_NO_MOB,		TRUE	},
     {	"indoors",		ROOM_INDOORS,		TRUE	},
     {	"private",		ROOM_PRIVATE,		TRUE    },
     {	"safe",			ROOM_SAFE,		TRUE	},
     {	"solitary",		ROOM_SOLITARY,		TRUE	},
     {	"pet_shop",		ROOM_PET_SHOP,		TRUE	},
     {	"no_recall",		ROOM_NO_RECALL,		TRUE	},
     {  "no_summon",		ROOM_NO_SUMMON,		TRUE	},
     {	"imp_only",		ROOM_IMP_ONLY,		TRUE    },
     {	"gods_only",	        ROOM_GODS_ONLY,		TRUE    },
     {	"heroes_only",		ROOM_HEROES_ONLY,	TRUE	},
     {	"newbie_only",		ROOM_NEWBIES_ONLY,	TRUE	},
     {	"law",			ROOM_LAW,		TRUE	},
     {  "no_where",		ROOM_NOWHERE,		TRUE	},
     {  "no_gate",		ROOM_NO_GATE,		TRUE	},
     {  "no_consecrate",	ROOM_NO_CONSECRATE,	TRUE	},
     {  "low_only",		ROOM_LOW_ONLY,		TRUE	},
     {  "no_magic",		ROOM_NO_MAGIC,		TRUE	},
     {  "consecrated",		ROOM_CONSECRATED,	TRUE 	}, 
    {	"no_newbie",		ROOM_NO_NEWBIE,		TRUE	},
     {	NULL,			0,			0	}
 };



const struct flag_type sector_flags[] =
 {
    {	"inside",	SECT_INSIDE,		TRUE	},
    {	"city",		SECT_CITY,		TRUE	},
    {	"field",	SECT_FIELD,		TRUE	},
    {	"forest",	SECT_FOREST,		TRUE	},
    {	"hills",	SECT_HILLS,		TRUE	},
    {	"mountain",	SECT_MOUNTAIN,		TRUE	},
    {	"swim",		SECT_WATER_SWIM,	TRUE	},
    {	"noswim",	SECT_WATER_NOSWIM,	TRUE	},
    {  	"unused",	SECT_UNUSED,		TRUE	},
    {	"air",		SECT_AIR,		TRUE	},
    {	"desert",	SECT_DESERT,		TRUE	},
    {	"road",		SECT_ROAD,		TRUE	},
    {	"underwater",	SECT_UNDERWATER,	TRUE	},
    {	NULL,		0,			0	}
 };



const struct flag_type type_flags[] =
 {
     {	"light",		ITEM_LIGHT,		TRUE	},
     {	"scroll",		ITEM_SCROLL,		TRUE	},
     {	"wand",			ITEM_WAND,		TRUE	},
     {	"staff",		ITEM_STAFF,		TRUE	},
     {	"weapon",		ITEM_WEAPON,		TRUE	},
     {	"treasure",		ITEM_TREASURE,		TRUE	},
     {	"armor",		ITEM_ARMOR,		TRUE	},
     {	"potion",		ITEM_POTION,		TRUE	},
     {	"furniture",		ITEM_FURNITURE,		TRUE	},
     {	"trash",		ITEM_TRASH,		TRUE	},
     {	"container",		ITEM_CONTAINER,		TRUE	},
     {	"drinkcontainer",	ITEM_DRINK_CON,		TRUE	},
     {	"key",			ITEM_KEY,		TRUE	},
     {	"food",			ITEM_FOOD,		TRUE	},
     {	"money",		ITEM_MONEY,		TRUE	},
     {	"boat",			ITEM_BOAT,		TRUE	},
     {	"npccorpse",		ITEM_CORPSE_NPC,	TRUE	},
     {	"pc corpse",		ITEM_CORPSE_PC,		FALSE	},
     {	"fountain",		ITEM_FOUNTAIN,		TRUE	},
     {	"pill",			ITEM_PILL,		TRUE	},
     {	"protect",		ITEM_PROTECT,		TRUE	},
     {	"map",			ITEM_MAP,		TRUE	},
     {   "portal",		ITEM_PORTAL,		TRUE	},
     {   "warpstone",		ITEM_WARP_STONE,	TRUE	},
     {	"roomkey",		ITEM_ROOM_KEY,		TRUE	},
     { 	"gem",			ITEM_GEM,		TRUE	},
     {	"jewelry",		ITEM_JEWELRY,		TRUE	},
     {	"jukebox",		ITEM_JUKEBOX,		TRUE	},
     { "instrument",		ITEM_INSTRUMENT,	TRUE	},
     {	NULL,			0,			0	}
 };


const struct flag_type extra_flags[] =
 {
     {	"glow",			ITEM_GLOW,		TRUE	},
     {	"hum",			ITEM_HUM,		TRUE	},
     {	"dark",			ITEM_DARK,		TRUE	},
     {	"lock",			ITEM_LOCK,		TRUE	},
     {	"evil",			ITEM_EVIL,		TRUE	},
     {	"invis",		ITEM_INVIS,		TRUE	},
     {	"magic",		ITEM_MAGIC,		TRUE	},
     {	"nodrop",		ITEM_NODROP,		TRUE	},
     {	"bless",		ITEM_BLESS,		TRUE	},
     {	"antigood",		ITEM_ANTI_GOOD,		TRUE	},
     {	"antievil",		ITEM_ANTI_EVIL,		TRUE	},
     {	"antineutral",		ITEM_ANTI_NEUTRAL,	TRUE	},
     {	"noremove",		ITEM_NOREMOVE,		TRUE	},
     {	"inventory",		ITEM_INVENTORY,		TRUE	},
     {	"nopurge",		ITEM_NOPURGE,		TRUE	},
     {	"rotdeath",		ITEM_ROT_DEATH,		TRUE	},
     {	"visdeath",		ITEM_VIS_DEATH,		TRUE	},
     {	"meltdrop",		ITEM_MELT_DROP,		TRUE	},
     {	"hadtimer",		ITEM_HAD_TIMER,		TRUE	},
     {	"sellextract",		ITEM_SELL_EXTRACT,	TRUE	},
     {	"burnproof",		ITEM_BURN_PROOF,	TRUE	},
     {	"nouncurse",		ITEM_NOUNCURSE,		TRUE	},
     {  "metal",            	ITEM_METAL,	        TRUE    },
     {  "nolocate",		ITEM_NOLOCATE,		TRUE    },
     {  "nodisarm",		ITEM_NODISARM,		TRUE	},
     {  "corpsenoanim",		CORPSE_NO_ANIMATE,	TRUE	},
     {  "hidden",		ITEM_HIDDEN,		TRUE	},
     {  "antineutraleth",	ITEM_ANTI_NEUT,		TRUE	},
     {  "antichaotic",		ITEM_ANTI_CHAOTIC,	TRUE	},
     {	"maledicted",		ITEM_MALEDICTED,	TRUE	},
     {	"fixed",		ITEM_FIXED,		TRUE	},
     {	NULL,			0,			0	}
 };

const struct flag_type restrict_flags[] =
{
	{ "human_only",		HUMAN_ONLY,		TRUE	},
    	{ "dwarf_only",		DWARF_ONLY,		TRUE	},
    	{ "elf_only",		ELF_ONLY,		TRUE	},
    	{ "drow_only",		DARK_ONLY,		TRUE	},
    	{ "giant_only",		GIANT_ONLY,		TRUE	},
	{ "centuar_only",	CENTAUR_ONLY,		TRUE	},
	{ "troll_only",		TROLL_ONLY,		TRUE	},
	{ "draconian_only",	DRACONIAN_ONLY,		TRUE	},
	{ "gnome_only",		GNOME_ONLY,		TRUE	},
	{ "warrior_only",	WARRIOR_ONLY,		TRUE	},
	{ "thief_only",		THIEF_ONLY,		TRUE	},
	{ "zealot_only",	ZEALOT_ONLY,		TRUE	},
	{ "AP_only",		ANTI_PALADIN_ONLY,	TRUE	},
	{ "bard_only",		BARD_ONLY,		TRUE	},
	{ "ranger_only",	RANGER_ONLY,		TRUE	},
	{ "channeler_only",	CHANNELER_ONLY,		TRUE	},
	{ "assassin_only",	ASSASSIN_ONLY,		TRUE	},
	{ "necromancer_only",	NECROMANCER_ONLY,	TRUE	},
	{ "elementalist_only",	ELEMENTALIST_ONLY,	TRUE	},
	{ "healer_only",	HEALER_ONLY,		TRUE	},
	{ "rager_only",		RAGER_ONLY,		TRUE	},
	{ "ancient_only",	ANCIENT_ONLY,		TRUE	},
	{ "sylvan_only",	SYLVAN_ONLY,		TRUE    },
	{ "mage_only",		MAGE_ONLY,		TRUE	},
	{ "arcana_only",	ARCANA_ONLY,		TRUE	},
	{ "knight_only",	KNIGHT_ONLY,		TRUE	},
	{ "outlaw_only",	OUTLAW_ONLY,		TRUE	},
	{ "bounty_only",	BOUNTY_ONLY,		TRUE	},
	{ "newbie_only",	NEWBIE_ONLY,		TRUE	},
      	{ "paladin_only",	PALADIN_ONLY,		TRUE	},
	{ NULL,			0,			0	}
};

const struct flag_type wear_flags[] =
{
     {	"take",			ITEM_TAKE,		TRUE	},
     {	"finger",		ITEM_WEAR_FINGER,	TRUE	},
     {	"neck",			ITEM_WEAR_NECK,		TRUE	},
     {	"body",			ITEM_WEAR_BODY,		TRUE	},
     {	"head",			ITEM_WEAR_HEAD,		TRUE	},
     {	"legs",			ITEM_WEAR_LEGS,		TRUE	},
     {	"feet",			ITEM_WEAR_FEET,		TRUE	},
     {	"hands",		ITEM_WEAR_HANDS,	TRUE	},
     {	"arms",			ITEM_WEAR_ARMS,		TRUE	},
     {	"shield",		ITEM_WEAR_SHIELD,	TRUE	},
     {	"about",		ITEM_WEAR_ABOUT,	TRUE	},
     {	"waist",		ITEM_WEAR_WAIST,	TRUE	},
     {	"wrist",		ITEM_WEAR_WRIST,	TRUE	},
     {	"wield",		ITEM_WIELD,		TRUE	},
     {	"hold",			ITEM_HOLD,		TRUE	},
     {  "nosac",		ITEM_NO_SAC,		TRUE	},
     {  "brand",		ITEM_WEAR_BRAND,	TRUE	},
     {  "strap",		ITEM_WEAR_STRAPPED,	TRUE    },
     {	NULL,			0,			0	}
};

 /*
  * Used when adding an affect to tell where it goes.
  * See addaffect and delaffect in act_olc.c
  */
 const struct flag_type apply_flags[] =
 {
     {	"none",			APPLY_NONE,		TRUE	},
     {	"strength",		APPLY_STR,		TRUE	},
     {	"dexterity",		APPLY_DEX,		TRUE	},
     {	"intelligence",		APPLY_INT,		TRUE	},
     {	"wisdom",		APPLY_WIS,		TRUE	},
     {	"constitution",		APPLY_CON,		TRUE	},
     {	"sex",			APPLY_SEX,		TRUE	},
     {	"class",		APPLY_CLASS,		TRUE	},
     {	"level",		APPLY_LEVEL,		TRUE	},
     {	"age",			APPLY_AGE,		TRUE	},
     {	"height",		APPLY_HEIGHT,		TRUE	},
     {	"weight",		APPLY_WEIGHT,		TRUE	},
     {	"mana",			APPLY_MANA,		TRUE	},
     {	"hp",			APPLY_HIT,		TRUE	},
     {	"move",			APPLY_MOVE,		TRUE	},
     {	"gold",			APPLY_GOLD,		TRUE	},
     {	"experience",		APPLY_EXP,		TRUE	},
     {	"ac",			APPLY_AC,		TRUE	},
     {	"hitroll",		APPLY_HITROLL,		TRUE	},
     {	"damroll",		APPLY_DAMROLL,		TRUE	},
     {	"saves",		APPLY_SAVES,		TRUE	},
     {	"savingpara",		APPLY_SAVING_PARA,	TRUE	},
     {	"savingrod",		APPLY_SAVING_ROD,	TRUE	},
     {	"savingpetri",		APPLY_SAVING_PETRI,	TRUE	},
     {	"savingbreath",		APPLY_SAVING_BREATH,	TRUE	},
     {	"savingspell",		APPLY_SAVING_SPELL,	TRUE	},
     {	"spellaffect",		APPLY_SPELL_AFFECT,	FALSE	},
     {	"dam_mod",		APPLY_DAM_MOD,		TRUE	},
     {  "attack_mod",		APPLY_ATTACK_MOD,	TRUE	},
     {  "enhancedDamMod",	APPLY_ENHANCED_MOD,	TRUE	},
	 {  "regen",		APPLY_REGENERATION,  TRUE    },
     {	"size",			APPLY_SIZE,		TRUE	},
     {	NULL,			0,			0	}
 };



 /*
  * What is seen.
  */
const struct flag_type wear_loc_strings[] =
 {
     {	"in the inventory",	WEAR_NONE,		TRUE	},
     {	"as a light",		WEAR_LIGHT,		TRUE	},
     {	"on the left finger",	WEAR_FINGER_L,		TRUE	},
     {	"on the right finger",	WEAR_FINGER_R,		TRUE	},
     {	"around the neck (1)",	WEAR_NECK_1,		TRUE	},
     {	"around the neck (2)",	WEAR_NECK_2,		TRUE	},
     {	"on the body",		WEAR_BODY,		TRUE	},
     {	"over the head",	WEAR_HEAD,		TRUE	},
     {	"on the legs",		WEAR_LEGS,		TRUE	},
     {	"on the feet",		WEAR_FEET,		TRUE	},
     {	"on the hands",		WEAR_HANDS,		TRUE	},
     {	"on the arms",		WEAR_ARMS,		TRUE	},
     {	"as a shield",		WEAR_SHIELD,		TRUE	},
     {	"about the shoulders",	WEAR_ABOUT,		TRUE	},
     {	"around the waist",	WEAR_WAIST,		TRUE	},
     {	"on the left wrist",	WEAR_WRIST_L,		TRUE	},
     {	"on the right wrist",	WEAR_WRIST_R,		TRUE	},
     {	"wielded",		WEAR_WIELD,		TRUE	},
     {	"held in the hands",	WEAR_HOLD,		TRUE	},
     {  "dual wielded",		WEAR_DUAL_WIELD,	TRUE    },
     {	NULL,			0	      , 	0	}
 };


const struct flag_type wear_loc_flags[] =
 {
     {	"none",		WEAR_NONE,		TRUE	},
     {	"light",	WEAR_LIGHT,		TRUE	},
     {	"lfinger",	WEAR_FINGER_L,		TRUE	},
     {	"rfinger",	WEAR_FINGER_R,		TRUE	},
     {	"neck1",	WEAR_NECK_1,		TRUE	},
     {	"neck2",	WEAR_NECK_2,		TRUE	},
     {	"body",		WEAR_BODY,		TRUE	},
     {	"head",		WEAR_HEAD,		TRUE	},
     {	"legs",		WEAR_LEGS,		TRUE	},
     {	"feet",		WEAR_FEET,		TRUE	},
     {	"hands",	WEAR_HANDS,		TRUE	},
     {	"arms",		WEAR_ARMS,		TRUE	},
     {	"shield",	WEAR_SHIELD,		TRUE	},
     {	"about",	WEAR_ABOUT,		TRUE	},
     {	"waist",	WEAR_WAIST,		TRUE	},
     {	"lwrist",	WEAR_WRIST_L,		TRUE	},
     {	"rwrist",	WEAR_WRIST_R,		TRUE	},
     {	"wielded",	WEAR_WIELD,		TRUE	},
     {	"hold",		WEAR_HOLD,		TRUE	},
     {	"dualwielded",	WEAR_DUAL_WIELD,	TRUE	},
     {	NULL,		0,		0	}
 };

const struct flag_type container_flags[] =
 {
     {	"closeable",		1,		TRUE	},
     {	"pickproof",		2,		TRUE	},
     {	"closed",		4,		TRUE	},
     {	"locked",		8,		TRUE	},
     {	"puton",		16,		TRUE	},
     {	NULL,			0,		0	}
 };

 /*****************************************************************************
                       ROM - specific tables:
  ****************************************************************************/




const struct flag_type ac_type[] =
 {
     {   "pierce",        AC_PIERCE,            TRUE    },
     {   "bash",          AC_BASH,              TRUE    },
     {   "slash",         AC_SLASH,             TRUE    },
     {   "exotic",        AC_EXOTIC,            TRUE    },
     {   NULL,              0,                    0       }
 };


const struct flag_type size_flags[] =
 {
     {   "tiny",          SIZE_TINY,            TRUE    },
     {   "small",         SIZE_SMALL,           TRUE    },
     {   "medium",        SIZE_MEDIUM,          TRUE    },
     {   "large",         SIZE_LARGE,           TRUE    },
     {   "huge",          SIZE_HUGE,            TRUE    },
     {   "giant",         SIZE_GIANT,           TRUE    },
     {   NULL,              0,                    0       },
 };


const struct flag_type weapon_class[] =
 {
     {   "exotic",	WEAPON_EXOTIC,		TRUE    },
     {   "sword",	WEAPON_SWORD,		TRUE    },
     {   "dagger",	WEAPON_DAGGER,		TRUE    },
     {   "spear",	WEAPON_SPEAR,		TRUE    },
     {   "mace",	WEAPON_MACE,		TRUE    },
     {   "axe",		WEAPON_AXE,		TRUE    },
     {   "flail",	WEAPON_FLAIL,		TRUE    },
     {   "whip",	WEAPON_WHIP,		TRUE    },
     {   "polearm",	WEAPON_POLEARM,		TRUE    },
     {	 "staff",	WEAPON_STAFF,		TRUE	},
     {   NULL,		0,			0       }
 };


const struct flag_type weapon_type2[] =
 {
     {   "flaming",       WEAPON_FLAMING,       TRUE    },
     {   "frost",         WEAPON_FROST,         TRUE    },
     {   "vampiric",      WEAPON_VAMPIRIC,      TRUE    },
     {   "sharp",         WEAPON_SHARP,         TRUE    },
     {   "vorpal",        WEAPON_VORPAL,        TRUE    },
     {   "twohands",     WEAPON_TWO_HANDS,     TRUE     },
     {	"shocking",	 WEAPON_SHOCKING,      TRUE     },
     {	"poison",	WEAPON_POISON,		TRUE	},
     {	"leader",	WEAPON_LEADER,		TRUE	},
     {   NULL,              0,                    0       }
 };

const struct flag_type res_flags[] =
 {
     {	"summon",	 RES_SUMMON,		TRUE	},
     {   "charm",         RES_CHARM,            TRUE    },
     {   "magic",         RES_MAGIC,            TRUE    },
     {   "weapon",        RES_WEAPON,           TRUE    },
     {   "bash",          RES_BASH,             TRUE    },
     {   "pierce",        RES_PIERCE,           TRUE    },
     {   "slash",         RES_SLASH,            TRUE    },
     {   "fire",          RES_FIRE,             TRUE    },
     {   "cold",          RES_COLD,             TRUE    },
     {   "lightning",     RES_LIGHTNING,        TRUE    },
     {   "acid",          RES_ACID,             TRUE    },
     {   "poison",        RES_POISON,           TRUE    },
     {   "negative",      RES_NEGATIVE,         TRUE    },
     {   "holy",          RES_HOLY,             TRUE    },
     {   "energy",        RES_ENERGY,           TRUE    },
     {   "mental",        RES_MENTAL,           TRUE    },
     {   "disease",       RES_DISEASE,          TRUE    },
     {   "drowning",      RES_DROWNING,         TRUE    },
     {   "light",         RES_LIGHT,            TRUE    },
     {	"sound",	RES_SOUND,		TRUE	},
     {	"wood",		RES_WOOD,		TRUE	},
     {	"silver",	RES_SILVER,		TRUE	},
     {	"iron",		RES_IRON,		TRUE	},
     {   NULL,          0,            0    }
 };


const struct flag_type vuln_flags[] =
 {
     {	"summon",	 VULN_SUMMON,		TRUE	},
     {	"charm",	VULN_CHARM,		TRUE	},
     {   "magic",         VULN_MAGIC,           TRUE    },
     {   "weapon",        VULN_WEAPON,          TRUE    },
     {   "bash",          VULN_BASH,            TRUE    },
     {   "pierce",        VULN_PIERCE,          TRUE    },
     {   "slash",         VULN_SLASH,           TRUE    },
     {   "fire",          VULN_FIRE,            TRUE    },
     {   "cold",          VULN_COLD,            TRUE    },
     {   "lightning",     VULN_LIGHTNING,       TRUE    },
     {   "acid",          VULN_ACID,            TRUE    },
     {   "poison",        VULN_POISON,          TRUE    },
     {   "negative",      VULN_NEGATIVE,        TRUE    },
     {   "holy",          VULN_HOLY,            TRUE    },
     {   "energy",        VULN_ENERGY,          TRUE    },
     {   "mental",        VULN_MENTAL,          TRUE    },
     {   "disease",       VULN_DISEASE,         TRUE    },
     {   "drowning",      VULN_DROWNING,        TRUE    },
     {   "light",         VULN_LIGHT,           TRUE    },
     {	"sound",	 VULN_SOUND,		TRUE	},
     {   "wood",          VULN_WOOD,            TRUE    },
     {   "silver",        VULN_SILVER,          TRUE    },
     {   "iron",          VULN_IRON,            TRUE    },
     {   NULL,              0,                    0       }
 };

 const struct flag_type position_flags[] =
 {
     {   "dead",           POS_DEAD,            FALSE   },
     {   "mortal",         POS_MORTAL,          FALSE   },
     {   "incap",          POS_INCAP,           FALSE   },
     {   "stunned",        POS_STUNNED,         FALSE   },
     {   "sleeping",       POS_SLEEPING,        TRUE    },
     {   "resting",        POS_RESTING,         TRUE    },
     {   "sitting",        POS_SITTING,         TRUE    },
     {   "fighting",       POS_FIGHTING,        FALSE   },
     {   "standing",       POS_STANDING,        TRUE    },
     {   NULL,              0,                    0       }
 };

const struct flag_type portal_flags[] =
{
     {   "normal_exit",	  GATE_NORMAL_EXIT,	TRUE	},
     {	"no_curse",	  GATE_NOCURSE,		TRUE	},
     {   "go_with",	  GATE_GOWITH,		TRUE	},
     {   "buggy",	  GATE_BUGGY,		TRUE	},
     {	"random",	  GATE_RANDOM,		TRUE	},
     {   NULL,		  0,			0	}
 };

const struct flag_type furniture_flags[] =
{
     {   "stand_at",	  STAND_AT,		TRUE	},
     {	"stand_on",	  STAND_ON,		TRUE	},
     {	"stand_in",	  STAND_IN,		TRUE	},
     {	"sit_at",	  SIT_AT,		TRUE	},
     {	"sit_on",	  SIT_ON,		TRUE	},
     {	"sit_in",	  SIT_IN,		TRUE	},
     {	"rest_at",	  REST_AT,		TRUE	},
     {	"rest_on",	  REST_ON,		TRUE	},
     {	"rest_in",	  REST_IN,		TRUE	},
     {	"sleep_at",	  SLEEP_AT,		TRUE	},
     {	"sleep_on",	  SLEEP_ON,		TRUE	},
     {	"sleep_in",	  SLEEP_IN,		TRUE	},
     {	"put_at",	  PUT_AT,		TRUE	},
     {	"put_on",	  PUT_ON,		TRUE	},
     {	"put_in",	  PUT_IN,		TRUE	},
     {	"put_inside",	  PUT_INSIDE,		TRUE	},
     {	NULL,		  0,			0	}
 };

const struct flag_type     apply_types [] =
{
        {	"affects",	TO_AFFECTS,	TRUE	},
 	{	"object",	TO_OBJECT,	TRUE	},
 	{	"immune",	TO_IMMUNE,	TRUE	},
 	{	"resist",	TO_RESIST,	TRUE	},
 	{	"vuln",		TO_VULN,	TRUE	},
 	{	"weapon",	TO_WEAPON,	TRUE	},
 	{	NULL,		0,		TRUE	}
};

const struct bit_type bitvector_type [] =
{
    	{    	affect_flags,  	"affect"        },
 	{	apply_flags,	"apply"		},
 	{	imm_flags,	"imm"		},
 	{	res_flags,	"res"		},
 	{	vuln_flags,	"vuln"		},
 	{	weapon_type2,	"weapon"	}
};

const struct empire_type empire_table[MAX_EMPIRE] =
{
    /*  name,		who entry  */
    {   "anathema",	"[ANATHEMA] "	},
    {   "nonempire",	""	},
    {   "prebloodoath",	""	},
    {   "bloodoath",	"[BLOODOATH] "	},
    {   "citizen",	"[CITIZEN] "	},
    {   "leader",	"[LEADER] "	},
    {   "emperor",	"[EMPEROR] "	},
    {   "immortal",	""		}

};

const struct color_type color_table[MAX_COLORS] =
{
	/* Color name	Associated Ascii Value */
	{"uncolored",	"\x01B[0m" 		},
	{"red",			"\x01B[0;31m" 	},
    {"green",		"\x01B[0;32m" 	},
    {"yellow",		"\x01B[0;33m" 	},
	{"blue",		"\x01B[0;34m" 	},
	{"magenta",		"\x01B[0;35m" 	},
	{"cyan",		"\x01B[0;36m" 	},
	{"lightgrey",	"\x01B[0;37m" 	},
	{"darkgrey",	"\x01B[1;30m" 	},
	{"lightred",	"\x01B[1;31m" 	},
	{"lightgreen",	"\x01B[1;32m" 	},
	{"lightyellow",	"\x01B[1;33m" 	},
	{"lightblue",	"\x01B[1;34m" 	},
	{"lightmagenta","\x01B[1;35m" 	},
	{"lightcyan",	"\x01B[1;36m" 	},
	{"white",		"\x01B[1;37m" 	},
};
const struct color_event_type color_event[MAX_EVENTS] =
{
	/* Event Name		Default Color   Minimum Level */
        {"prompt",		"uncolored",	1  },
		{"channels",	"darkgrey",		1  },
		{"tells",		"lightgreen",	1  },
		{"grouptells",	"magenta",		1  },
		{"speech",		"lightyellow",	1  },
		{"damage",		"lightred",	1  },
		{"yells",		"lightcyan",	1  },
		{"immtalk",		"lightblue",	52 },
		{"wiznet",		"uncolored",	52 },
		{"prays",		"lightred",	53 },
};

const struct flag_type extended_flags[] =
{
    {	"cheap_shot",		OFF_CHEAPSHOT,		TRUE	},
    {	"parting_blow",		OFF_PARTING_BLOW,	TRUE	},
    {	"assassin",		OFF_ASSASSIN,		TRUE	},
    {	"sword_spec",		OFF_SWORD_SPEC,		TRUE	},
    {	"axe_spec",		OFF_AXE_SPEC,		TRUE	},
    {	"hth_spec",		OFF_HTH_SPEC,		TRUE	},
    {	"dagger_spec",		OFF_DAGGER_SPEC,	TRUE	},
    {	"spear_spec",		OFF_SPEAR_SPEC,		TRUE	},
    {	"mace_spec",		OFF_MACE_SPEC,		TRUE	},
    {	"whip_spec",		OFF_WHIP_SPEC,		TRUE	},
    {	"polearm_spec",		OFF_POLEARM_SPEC,	TRUE	},
    {	NULL,			0,			FALSE	}
};

/* New Affect Types */
const struct flag_type aftype_table[] =
{
    {   "AFT_SPELL",    AFT_SPELL,              TRUE    },
    {   "AFT_SKILL",    AFT_SKILL,              TRUE    },
    {   "AFT_POWER",    AFT_POWER,              TRUE    },
    {   "AFT_MALADY",   AFT_MALADY,             TRUE    },
    {   "AFT_COMMUNE",  AFT_COMMUNE,    	TRUE    },
    {   "AFT_INVIS",    AFT_INVIS,              TRUE    },
    {	NULL,		0,			FALSE	},
};

const struct display_type apply_locations[] =
{
    {   "none",			APPLY_NONE,			"none"			},
    {	"str",			APPLY_STR,     		"strength"			},
    {	"dex",   		APPLY_DEX,     		"dexterity"			},
    { 	"int",	 		APPLY_INT,    	 	"intelligence"			},
    {   "wis",   		APPLY_WIS,     		"wisdom"			},
    {   "con",   		APPLY_CON,     		"constitution"			},
    {	"sex",   		APPLY_SEX,     		"sex"				},
    {	"class", 		APPLY_CLASS,   		"class"				},
    {   "age",  	 	APPLY_AGE,     		"age"				},
    {   "height",		APPLY_HEIGHT,     	"none"				},
    {   "weight",		APPLY_WEIGHT,     	"none"				},
    {   "mana",  		APPLY_MANA,     	"mana"				},
    {   "hp",			APPLY_HIT,     		"health"			},
    {   "move",  		APPLY_MOVE,     	"moves"				},
    {   "gold",	 		APPLY_GOLD,     	"none"				},
    {   "exp",	 		APPLY_EXP,     		"none"				},
    {	"ac",			APPLY_AC,     		"armor class"			},
    {   "hitroll",		APPLY_HITROLL,   	"hitroll"  			},
    {	"damroll",		APPLY_DAMROLL,      "damroll"				},
    {	"saves",		APPLY_SAVES,        "saves"				},
    {	"spara",		APPLY_SAVING_PARA,  "save vs paralysis"			},
    {	"srod",			APPLY_SAVING_ROD,   "save vs rod"			},
    {	"spetri",		APPLY_SAVING_PETRI, "save vs petrification"		},
    {	"sbreath",		APPLY_SAVING_BREATH,"save vs breath"			},
    {   "svspell",		APPLY_SAVING_SPELL, "save vs spell" 			},
    {	"affect",		APPLY_SPELL_AFFECT, "none"				},
    {	"regen",		APPLY_REGENERATION, "regeneration"  			},
    {	"dam_mod",		APPLY_DAM_MOD,		"% damage received" 		},
    {	"enhancedDamMod",	APPLY_ENHANCED_MOD,	"% damage dealt" 		},
    {   "attack_mod",		APPLY_ATTACK_MOD,	"number of attacks"		},
    {	"size",			APPLY_SIZE,		"size"				},
    {   NULL,			0,					FALSE		},
};

const struct flag_type area_type_table[] =
{
	{"normal",		ARE_NORMAL,		TRUE },
	{"cabal",		ARE_CABAL,		TRUE },
	{"quest",		ARE_QUEST,		TRUE },
	{"city",		ARE_CITY,		TRUE },
	{"unopened",		ARE_UNOPENED,		TRUE },
	{NULL,			0,			TRUE },
};

const struct bounty_type bounty_table[MAX_BOUNTY] =
{
    /* rank name,	extitle,		credits required */
    {   "neophyte",	"Neophyte Hunter", 		0	},
    {   "hunter", 	"Veteran Hunter", 		200	},
    {   "master",	"Master Hunter",		500	},
    {	"sensei",	"Elder Sensei of the Hunters",	1000	},
};

const struct quest_reward quest_reward_table[MAX_QUEST_REWARDS] =
{
    /*  keyword,		price,		type
	description,
	classes,
	races,
	aligns, ethos,
	function that performs the reward

    Class array layout (listed in groups of 5):
		{ warrior, thief, zealot, paladin, anti-paladin,
		  ranger, druid, channeler, assassin, necromancer,
		  elementalist, bard, healer }

    Race array layout (listed in groups of 5):
		{ human, dwarf, elf, svirfnebli, dark-elf,
       		  centaur, troll, cloud, gnome, draconian,
		  ethereal, changeling, arial, fire, duergar,
		  storm, lich, angel, wood-elf, illithid, 
		  vuuhu, ruugrah, malefisti } */

    //ARMOR
    {   "warrior set boots",		2500,	REWARD_ARMOR, SET_ITEM, SET_WARRIOR,
	OBJ_VNUM_QUEST_STORE_WARRIOR_BOOTS,
	"Boots fashioned for the strongest of warriors. Grant the March skill.",		
	{1,0,0,0,0,0,0,0,0,0,0,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_warrior_boots	},
	
    {   "warrior set gauntlets",		2500,	REWARD_ARMOR, SET_ITEM, SET_WARRIOR, 
	OBJ_VNUM_QUEST_STORE_WARRIOR_GAUNTLETS,
	"Gloves fashioned for the strongest of warriors.",		
	{1,0,0,0,0,0,0,0,0,0,0,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_warrior_gauntlets	},

    {   "warrior set plate",		2500,	REWARD_ARMOR, SET_ITEM, SET_WARRIOR,
	OBJ_VNUM_QUEST_STORE_WARRIOR_PLATE,
	"Platemail fashioned for the strongest of warriors.",		
	{1,0,0,0,0,0,0,0,0,0,0,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_warrior_plate	},

    {   "channeler set helm",		2500,	REWARD_ARMOR, SET_ITEM, SET_CHANNELER,
	OBJ_VNUM_QUEST_STORE_CHANNELER_HELM,
	"A crest designed to allow clearer and more focused thought. Grants the Mana\nConduit skill.",		
	{0,0,0,0,0,0,0,1,0,0,0,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_channeler_helm	},

    {   "channeler set necklace",		2500,	REWARD_ARMOR, SET_ITEM, SET_CHANNELER,
	OBJ_VNUM_QUEST_STORE_CHANNELER_NECK,
	"A necklace of magical beads. Grants the Mana Disruption spell.",		
	{0,0,0,0,0,0,0,1,0,0,0,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_channeler_neck	},

    {   "channeler set robe",		2500,	REWARD_ARMOR, SET_ITEM, SET_CHANNELER,
	OBJ_VNUM_QUEST_STORE_CHANNELER_ROBE,
	"A robe sewn of ethereal fibers. Grants the Mental Clarity skill.",		
	{0,0,0,0,0,0,0,1,0,0,0,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_channeler_robe	},

    {   "channeler set ankh",		2500,	REWARD_ARMOR, SET_ITEM, SET_CHANNELER,
	OBJ_VNUM_QUEST_STORE_CHANNELER_ANKH,
	"A magical artifact from an ethereal realm. Can be invoked for a Displacement\neffect.",		
	{0,0,0,0,0,0,0,1,0,0,0,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_channeler_ankh	},

    {   "thief set belt",		2500,	REWARD_ARMOR, SET_ITEM, SET_THIEF,
	OBJ_VNUM_QUEST_STORE_THIEF_BELT,
	"An ethereal belt of pouches. Grants the Plant Explosives skill.",		
	{0,1,0,0,0,0,0,0,0,0,0,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_thief_belt	},

    {   "thief set boots",		2500,	REWARD_ARMOR, SET_ITEM, SET_THIEF,
	OBJ_VNUM_QUEST_STORE_THIEF_BOOTS,
	"A pair of lightweight blackened boots. Grant the Advanced Tripwire skill.",		
	{0,1,0,0,0,0,0,0,0,0,0,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_thief_boots	},

    {   "thief set robe",		2500,	REWARD_ARMOR, SET_ITEM, SET_THIEF,
	OBJ_VNUM_QUEST_STORE_THIEF_ROBE,
	"A concealing robe forged from the essence of shadows. Grants the Shadowstep skill.",		
	{0,1,0,0,0,0,0,0,0,0,0,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_thief_robe	},
	
	{   "healer set ring",		2500,	REWARD_ARMOR, SET_ITEM, SET_HEALER,
	OBJ_VNUM_QUEST_STORE_HEALER_RING,
	"A holy ring crafted from translucent gold. Grants the Resurrect spell.",		
	{0,0,0,0,0,0,0,0,0,0,0,0,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_healer_ring	},
	
	{   "healer set sleeves",		2500,	REWARD_ARMOR, SET_ITEM, SET_HEALER,
	OBJ_VNUM_QUEST_STORE_HEALER_SLEEVES,
	"Silver-laced armguards from the heavens. Grants the Divine Transferrance spell.",		
	{0,0,0,0,0,0,0,0,0,0,0,0,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_healer_sleeves	},
	
	{   "healer set bracelet",		2500,	REWARD_ARMOR, SET_ITEM, SET_HEALER,
	OBJ_VNUM_QUEST_STORE_HEALER_BRACELET,
	"A blessed bracelet of pure aquamarine. Grants the Greater Regeneration skill.",		
	{0,0,0,0,0,0,0,0,0,0,0,0,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_healer_bracelet	},
	
	{   "healer set statue",		2500,	REWARD_ARMOR, SET_ITEM, SET_HEALER,
	OBJ_VNUM_QUEST_STORE_HEALER_STATUE,
	"A statuette imbued with the essence of a goddess.",		
	{0,0,0,0,0,0,0,0,0,0,0,0,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_healer_statue	},
	
	{   "elementalist set helm",		2500,	REWARD_ARMOR, SET_ITEM, SET_ELEMENTALIST,
	OBJ_VNUM_QUEST_STORE_ELEMENTALIST_HELM,
	"A halo of frozen flame. Grants the Shatter Armor spell.",		
	{0,0,0,0,0,0,0,0,0,0,1,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_elementalist_helm	},
	
	{   "elementalist set neck",		2500,	REWARD_ARMOR, SET_ITEM, SET_ELEMENTALIST,
	OBJ_VNUM_QUEST_STORE_ELEMENTALIST_NECK,
	"A cloak of pure flowing water. Grants the Elemental Affinity skill.",		
	{0,0,0,0,0,0,0,0,0,0,1,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_elementalist_neck	},
	
	{   "elementalist set belt",		2500,	REWARD_ARMOR, SET_ITEM, SET_ELEMENTALIST,
	OBJ_VNUM_QUEST_STORE_ELEMENTALIST_BELT,
	"A cord of sparking electricity. Grants the Hurricane spell.",		
	{0,0,0,0,0,0,0,0,0,0,1,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_elementalist_belt	},
	
	{   "elementalist set ankh",		2500,	REWARD_ARMOR, SET_ITEM, SET_ELEMENTALIST,
	OBJ_VNUM_QUEST_STORE_ELEMENTALIST_FLAME,
	"A pure, flickering flame.",		
	{0,0,0,0,0,0,0,0,0,0,1,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_elementalist_flame	},

    //WEAPONS
    {   "warrior set weapon",		2500,	REWARD_WEAPON, SET_ITEM, SET_WARRIOR,
	OBJ_VNUM_QUEST_STORE_WARRIOR_WEAPON,
	"A weapon fashioned for the strongest of warriors. This weapon's type can \nbe converted by saying the phrase, \"Victory to the <weapon type>!\"",		
	{1,0,0,0,0,0,0,0,0,0,0,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_warrior_weapon	},

     {   "thief set weapon",		2500,	REWARD_WEAPON, SET_ITEM, SET_THIEF,
	OBJ_VNUM_QUEST_STORE_THIEF_DAGGER,
	"A lightweight dagger forged from shadow.",		
	{0,1,0,0,0,0,0,0,0,0,0,0,0},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_thief_dagger	},

    //BOOSTS
    {   "hp boost",			1000,	REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Maximum HP raised by 10.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_hp_boost		},
    
    {   "mana boost",			1000,	REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Maximum Mana raised by 30.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_mana_boost		},

    {   "move boost",			1000,	REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Maximum Moves raised by 30.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_move_boost		},	

    {   "attack modifier",			10000,	REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"One extra attack per round.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_attack_modifier	},

    {   "damage reduction",			4000,	REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Damage received reduced by 1%.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_damage_reduction	},

    {   "damage enhancement",			2000, REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Damage dealt increased by 1%.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_damage_enhancer	},

    {   "remove fire vuln",			5000, REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Strips fire vulnerability.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_remove_vuln_fire	},

   {   "remove cold vuln",			5000, REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Strips cold vulnerability.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_remove_vuln_cold	},

    {   "remove lightning vuln",		5000, REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Strips lightning vulnerability.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_remove_vuln_lightning	},

    {   "remove holy vuln",		5000, REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Strips holy vulnerability.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_remove_vuln_holy	},

    {   "remove negative vuln",		5000, REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Strips negative vulnerability.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_remove_vuln_negative	},

    {   "remove mental vuln",		5000, REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Strips mental vulnerability.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_remove_vuln_mental	},

    {   "remove bash vuln",		5000, REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Strips bashing vulnerability.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_remove_vuln_bash	},

    {   "remove iron vuln",		5000, REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Strips iron vulnerability.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_remove_vuln_iron	},

    {   "remove drowning vuln",		5000, REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Strips drowning vulnerability.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_remove_vuln_drowning	},

    {   "remove light vuln",		5000, REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Strips light vulnerability.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_remove_vuln_light	},

    {   "add slash resistance",		7500, REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Grants resistance to slashing attacks.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_add_resist_slash	},

    {   "add pierce resistance",		7500, REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Grants resistance to piercing attacks.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_add_resist_pierce	},

    {   "add bash resistance",		7500, REWARD_BOOST, NOT_SET_ITEM, SET_NONE, 0,
	"Grants resistance to bashing attacks.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_add_resist_bash	},
	


    //MISCELLANEOUS
    {   "pill of recall",		250, REWARD_MISCELLANEOUS, NOT_SET_ITEM, SET_NONE, 0,
	"Buys one (1) pill of recall.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_pill_recall	},

    {   "pill of serum",		250, REWARD_MISCELLANEOUS, NOT_SET_ITEM, SET_NONE, 0,
	"Buys one (1) poison antidote.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_pill_antidote	},

     {  "pill of purifying",	250, REWARD_MISCELLANEOUS, NOT_SET_ITEM, SET_NONE, 0,
	"Buys one (1) plague-counteracting pill.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_pill_purifier	},

     {  "pill of holy water",	250, REWARD_MISCELLANEOUS, NOT_SET_ITEM, SET_NONE, 0,
	"Buys one (1) pill filled with holy water, used for removing curses.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_pill_holy_water	},

     {  "pill of teleportation",	250, REWARD_MISCELLANEOUS, NOT_SET_ITEM, SET_NONE, 0,
	"Buys one (1) teleportation pill.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_pill_teleport	},

     {  "pill of flight",	250, REWARD_MISCELLANEOUS, NOT_SET_ITEM, SET_NONE, 0,
	"Buys one (1) pill of flying.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_pill_flying	},

     {  "pill of pass-door",	250, REWARD_MISCELLANEOUS, NOT_SET_ITEM, SET_NONE, 0,
	"Buys one (1) pill of pass door.",
	{1,1,1,1,1,1,1,1,1,1,1,1,1},	
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	ALIGN_ANY, ETHOS_ANY,
	quest_reward_pill_pass_door	}

};

const struct set_type set_type_table[MAX_SET_TYPES] =
{
    {   SET_NONE, 	NULL,			NULL	 	},
    {   SET_WARRIOR,	"Victory (Warrior)",     check_set_item_warrior  	},
    {   SET_CHANNELER,  "Consciousness (Channeler)",   check_set_item_channeler },
    {   SET_THIEF,  	"Imperception (Thief)",   check_set_item_thief },
	{	SET_HEALER,		"Devotion (Healer)",	check_set_item_healer },
	{	SET_ELEMENTALIST,"Control (Elementalist)",	check_set_item_elementalist },
};

const struct sector_type sector_table[SECT_MAX] =
{
    {	"inside",	SECT_INSIDE,		1,	1,	"civilized"	},
    {	"city",		SECT_CITY,		1,	1,	"civilized"	},
    {	"field",	SECT_FIELD,		2,	1,	"uncivilized"	},
    {	"forest",	SECT_FOREST,		3,	2,	"uncivilized"	},
    {	"hills",	SECT_HILLS,		5,	2,	"uncivilized"	},
    {	"mountains",	SECT_MOUNTAIN,		6,	3,	"uncivilized"	},
    {	"water",	SECT_WATER_SWIM,	3,	1,	"uncivilized"	},
    {	"water",	SECT_WATER_NOSWIM,	3,	1,	"uncivilized"	},
    {	"unused",	SECT_UNUSED,		1,	1,	"civilized"	},
    {	"air",		SECT_AIR,		4,	2,	"uncivilized"	},
    {	"desert",	SECT_DESERT,		5,	4,	"uncivilized"	},
    {	"road",		SECT_ROAD,		2,	1,	"civilized"	},
    {	"underwater",	SECT_UNDERWATER,	4,	2,	"underwater"	}
};
