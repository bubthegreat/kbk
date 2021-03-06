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
*	ROM license, in the file Rom24/doc/rom.license			   *
***************************************************************************/

/***************************************************************************
*       Tartarus code is copyright (C) 1997-1998 by Daniel Graham          *
*	In using this code you agree to comply with the Tartarus license   *
*       found in the file /Tartarus/doc/tartarus.doc                       *
***************************************************************************/

#define args( list )				list

#define DECLARE_DO_FUN( fun )			DO_FUN    fun
#define DECLARE_SPEC_FUN( fun )			SPEC_FUN  fun
#define DECLARE_SPELL_FUN( fun )		SPELL_FUN fun
#define DECLARE_AFF_FUN( fun )			AFF_FUN  fun
#define DECLARE_RAFF_FUN( fun )			RAFF_FUN  fun

#define DECLARE_IPROG_FUN_WEAR(fun) 		IPROG_FUN_WEAR fun
#define DECLARE_IPROG_FUN_REMOVE(fun)		IPROG_FUN_REMOVE fun
#define DECLARE_IPROG_FUN_GET(fun) 		IPROG_FUN_GET fun
#define DECLARE_IPROG_FUN_DROP(fun)		IPROG_FUN_DROP fun
#define DECLARE_IPROG_FUN_SAC(fun) 		IPROG_FUN_SAC fun
#define DECLARE_IPROG_FUN_ENTRY(fun) 		IPROG_FUN_ENTRY fun
#define DECLARE_IPROG_FUN_GIVE(fun) 		IPROG_FUN_GIVE fun
#define DECLARE_IPROG_FUN_GREET(fun) 		IPROG_FUN_GREET fun
#define DECLARE_IPROG_FUN_FIGHT(fun) 		IPROG_FUN_FIGHT fun
#define DECLARE_IPROG_FUN_DEATH(fun) 		IPROG_FUN_DEATH fun
#define DECLARE_IPROG_FUN_SPEECH(fun) 		IPROG_FUN_SPEECH fun
#define DECLARE_IPROG_FUN_PULSE(fun) 		IPROG_FUN_PULSE fun
#define DECLARE_IPROG_FUN_INVOKE(fun) 		IPROG_FUN_INVOKE fun

#define DECLARE_MPROG_FUN_ATTACK( fun )	    	MPROG_FUN_ATTACK fun
#define DECLARE_MPROG_FUN_BRIBE( fun )      	MPROG_FUN_BRIBE fun
#define DECLARE_MPROG_FUN_ENTRY( fun )      	MPROG_FUN_ENTRY fun
#define DECLARE_MPROG_FUN_GREET( fun )      	MPROG_FUN_GREET fun
#define DECLARE_MPROG_FUN_GIVE( fun )       	MPROG_FUN_GIVE fun
#define DECLARE_MPROG_FUN_FIGHT( fun )      	MPROG_FUN_FIGHT fun
#define DECLARE_MPROG_FUN_DEATH( fun )      	MPROG_FUN_DEATH fun
#define DECLARE_MPROG_FUN_PULSE( fun )      	MPROG_FUN_PULSE fun
#define DECLARE_MPROG_FUN_SPEECH( fun )     	MPROG_FUN_SPEECH fun
#define DECLARE_MPROG_FUN_MOVE( fun )       	MPROG_FUN_MOVE fun
#define DECLARE_MPROG_FUN_REQUEST( fun )	MPROG_FUN_MOVE fun

#define DECLARE_RPROG_FUN_SPEECH( fun )		RPROG_FUN_SPEECH fun
#define DECLARE_RPROG_FUN_ENTRY( fun )		RPROG_FUN_ENTRY fun

/* system calls */
int remove();
int system();



/*
 * Short scalar types.
 * Diavolo reports AIX compiler has bugs with short types.
 */
#if	!defined(FALSE)
#define FALSE	 0
#endif

#if	!defined(TRUE)
#define TRUE	 1
#endif

#if	defined(_AIX)
#if	!defined(const)
#define const
#endif
typedef int				sh_int;
typedef int				bool;
#define unix
#else
typedef short   int			sh_int;
typedef unsigned char			bool;
#endif

/* ea */
#define MSL MAX_STRING_LENGTH
#define MIL MAX_INPUT_LENGTH

/*
 * Structure types.
 */
typedef struct	affect_data		AFFECT_DATA;
typedef struct	area_data		AREA_DATA;
typedef struct	ban_data		BAN_DATA;
typedef struct 	buf_type	 	BUFFER;
typedef struct	char_data		CHAR_DATA;
typedef struct	descriptor_data		DESCRIPTOR_DATA;
typedef struct	exit_data		EXIT_DATA;
typedef struct	extra_descr_data	EXTRA_DESCR_DATA;
typedef struct	help_data		HELP_DATA;
typedef struct	help_area_data		HELP_AREA;
typedef struct	kill_data		KILL_DATA;
typedef struct	mem_data		MEM_DATA;
typedef struct	mob_index_data		MOB_INDEX_DATA;
typedef struct	obj_data		OBJ_DATA;
typedef struct	obj_index_data		OBJ_INDEX_DATA;
typedef struct	mat_data		MAT_DATA;
typedef struct	pc_data			PC_DATA;
typedef struct  gen_data		GEN_DATA;
typedef struct	reset_data		RESET_DATA;
typedef struct	room_index_data		ROOM_INDEX_DATA;
typedef struct	shop_data		SHOP_DATA;
typedef struct	time_info_data		TIME_INFO_DATA;
typedef struct	weather_data		WEATHER_DATA;
typedef struct  mprog_code              MPROG_CODE;
typedef struct  room_affect_data        ROOM_AFFECT_DATA;
typedef struct  old_char		OLD_CHAR;
typedef struct  iprog_data              IPROG_DATA;
typedef struct  mprog_data              MPROG_DATA;
typedef struct  rprog_data		RPROG_DATA;
typedef struct	note_data		NOTE_DATA;

/*
 * Function types.
 */
typedef	void DO_FUN		args( (CHAR_DATA *ch, char *argument ) );
typedef bool SPEC_FUN		args( (CHAR_DATA *ch ) );
typedef void SPELL_FUN		args( (int sn, int level, CHAR_DATA *ch, void *vo, int target ) );
typedef void RAFF_FUN		args( (ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *raf ) );
typedef void AFF_FUN		args( (CHAR_DATA *ch, AFFECT_DATA *af ) );
typedef void IPROG_FUN_WEAR 	args( (OBJ_DATA *obj, CHAR_DATA *ch));
typedef void IPROG_FUN_REMOVE 	args( (OBJ_DATA *obj, CHAR_DATA *ch));
typedef void IPROG_FUN_GET 	args( (OBJ_DATA *obj, CHAR_DATA *ch));
typedef void IPROG_FUN_DROP 	args( (OBJ_DATA *obj, CHAR_DATA *ch));
typedef bool IPROG_FUN_SAC	args( (OBJ_DATA *obj, CHAR_DATA *ch));
typedef void IPROG_FUN_ENTRY 	args( (OBJ_DATA *obj));
typedef bool IPROG_FUN_GIVE 	args( (OBJ_DATA *obj, CHAR_DATA *from, CHAR_DATA *to));
typedef void IPROG_FUN_GREET 	args( (OBJ_DATA *obj, CHAR_DATA *ch));
typedef void IPROG_FUN_FIGHT 	args( (OBJ_DATA *obj, CHAR_DATA *ch));
typedef bool IPROG_FUN_DEATH 	args( (OBJ_DATA *obj, CHAR_DATA *ch));
typedef void IPROG_FUN_SPEECH 	args( (OBJ_DATA *obj, CHAR_DATA *ch, char *speech));
typedef void IPROG_FUN_PULSE 	args( (OBJ_DATA *obj, bool isTick));
typedef void IPROG_FUN_INVOKE 	args( (OBJ_DATA *obj, CHAR_DATA *ch, char *argument));

typedef void MPROG_FUN_ATTACK 	args( (CHAR_DATA *mob, CHAR_DATA *attacker ) );
typedef void MPROG_FUN_BRIBE 	args( (CHAR_DATA *mob, CHAR_DATA *ch, int amount ) );
typedef void MPROG_FUN_ENTRY 	args( (CHAR_DATA *mob ) );
typedef void MPROG_FUN_GREET 	args( (CHAR_DATA *mob, CHAR_DATA *ch ) );
typedef void MPROG_FUN_GIVE 	args( (CHAR_DATA *mob, CHAR_DATA *ch, OBJ_DATA *obj ));
typedef void MPROG_FUN_FIGHT 	args( (CHAR_DATA *mob, CHAR_DATA *victim ));
typedef bool MPROG_FUN_DEATH 	args( (CHAR_DATA *mob, CHAR_DATA *killer ) );
typedef void MPROG_FUN_PULSE 	args( (CHAR_DATA *mob ) );
typedef void MPROG_FUN_SPEECH 	args( (CHAR_DATA *mob, CHAR_DATA *ch, char *speech));
typedef bool MPROG_FUN_MOVE 	args( (CHAR_DATA *ch, CHAR_DATA *mob, ROOM_INDEX_DATA *from, int direction));
typedef bool MPROG_FUN_REQUEST 	args( (CHAR_DATA *ch, CHAR_DATA *mob, OBJ_DATA *obj));

typedef void RPROG_FUN_SPEECH 	args( (ROOM_INDEX_DATA *room, CHAR_DATA *ch, char *speech));
typedef void RPROG_FUN_ENTRY 	args( (ROOM_INDEX_DATA *room, CHAR_DATA *ch));

typedef void FUNC args((CHAR_DATA *ch));
typedef void SET_ITEM_PROG args((CHAR_DATA *ch, int arg2, bool arg3));

/*
 * String and memory management parameters.
 */
#define	MAX_KEY_HASH		 1024
#define MAX_STRING_LENGTH	 4608
#define MAX_INPUT_LENGTH	  256
#define PAGELEN			   20


/*
 * Game parameters.
 * Increase the max'es if you add more of something.
 * Adjust the pulse numbers to suit yourself.

 */
#define MAX_SOCIALS		  	256
#define MAX_SKILL                 	638
#define MAX_GROUP		   	50
#define MAX_IN_GROUP              	500
#define MAX_ALIAS		   	10
#define MAX_CLASS                  	13
#define MAX_PC_RACE                	24
#define MAX_CABAL                 	10
#define MAX_DAMAGE_MESSAGE	   	63
#define MAX_LEVEL		   	60
#define LEVEL_HERO		   	(MAX_LEVEL - 9)
#define LEVEL_IMMORTAL		   	(MAX_LEVEL - 8)
#define MAX_WEAPON                 	10
#define MAX_VNUM		   	99999
#define MAX_FORM		   	6
#define MAX_HOMETOWN	 	   	6
#define MAX_QUEST_REWARDS		46
#define QUEST_CREDITS_PER_PK	5
#define MAX_SET_TYPES			6
#define GOLD_PER_POUND 		300

#define PULSE_PER_SECOND	    	4
#define PULSE_VIOLENCE		  	( 3 * PULSE_PER_SECOND)
#define PULSE_MOBILE		  	( 4 * PULSE_PER_SECOND)
#define PULSE_TICK		  	(30 * PULSE_PER_SECOND)
#define PULSE_AREA		  	(120 * PULSE_PER_SECOND)
#define PULSE_TRACK		  	(20 * PULSE_PER_SECOND)
#define PULSE_RIOT		  	( 2 * PULSE_PER_SECOND)
#define PULSE_RAFFECT             	( 3 * PULSE_MOBILE)
#define PULSE_IPROG_PULSE	  	PULSE_PER_SECOND

#define IMPLEMENTOR			MAX_LEVEL
#define	CREATOR				(MAX_LEVEL - 1)
#define SUPREME				(MAX_LEVEL - 2)
#define DEITY				(MAX_LEVEL - 3)
#define GOD				(MAX_LEVEL - 4)
#define IMMORTAL			(MAX_LEVEL - 5)
#define DEMI				(MAX_LEVEL - 6)
#define ANGEL				(MAX_LEVEL - 7)
#define AVATAR				(MAX_LEVEL - 8)
#define HERO				LEVEL_HERO

#define	CON_DIE_BOUND			3

#define CLEAR			"\x01B[0m"
#define C_RED			"[0;31m"	/* Normal Colours	*/
#define C_GREEN			"[0;32m"
#define C_YELLOW		"[0;33m"
#define C_BLUE			"[0;34m"
#define C_MAGENTA		"[0;35m"
#define C_CYAN			"[0;36m"
#define C_WHITE			"[0;37m"
#define C_D_GREY        	"[1;30m"	/* Light Colors	*/
#define C_B_RED			"[1;31m"
#define C_B_GREEN		"[1;32m"
#define C_B_YELLOW		"[1;33m"
#define C_B_BLUE		"[1;34m"
#define C_B_MAGENTA		"[1;35m"
#define C_B_CYAN		"[1;36m"
#define C_B_WHITE		"[1;37m"
#define BG_BLUE			"\e[1;44m"
#define C_UNDERLINE		"\x01B[4m"
#define	C_REVERSE		"\x01B[7m"
#define	C_FLASH			"\x01B[5m"

#define MAX_COLORS		20
#define MAX_EVENTS		10

/*
 * Site ban structure.
 */

#define BAN_SUFFIX		1
#define BAN_PREFIX		2
#define BAN_NEWBIES		3
#define BAN_ALL			4
#define BAN_PERMIT		5
#define BAN_PERMANENT		6

struct	ban_data
{
    BAN_DATA *	next;
    bool	valid;
    sh_int	ban_flags;
    sh_int	level;
    char *	name;
    char *	reason; // newban
    char *	banner; // newban
};

struct buf_type
{
    BUFFER *    next;
    bool        valid;
    sh_int      state;  /* error state of the buffer */
    sh_int      size;   /* size in k */
    char *      string; /* buffer's string */
};



/*
 * Time and weather stuff.
 */
#define SUN_DARK		    0
#define SUN_RISE		    1
#define SUN_LIGHT		    2
#define SUN_SET			    3

#define SKY_CLOUDLESS		    0
#define SKY_CLOUDY		    1
#define SKY_RAINING		    2
#define SKY_LIGHTNING		    3

struct	time_info_data
{
    int		hour;
    int		day;
    int		month;
    int		year;
};

struct	weather_data
{
    int		mmhg;
    int		change;
    int		sky;
    int		sunlight;
};

/* big batch of iprog shit */
#define IPROG_WEAR                (A)
#define IPROG_REMOVE              (B)
#define IPROG_DROP                (C)
#define IPROG_SAC                 (D)
#define IPROG_GIVE                (E)
#define IPROG_GREET               (F)
#define IPROG_FIGHT               (G)
#define IPROG_DEATH               (H)
#define IPROG_SPEECH              (I)
#define IPROG_ENTRY               (J)
#define IPROG_GET                 (K)
#define IPROG_PULSE               (L)
#define IPROG_INVOKE		  (M)

#define MPROG_BRIBE               (A)
#define MPROG_ENTRY               (B)
#define MPROG_GREET               (C)
#define MPROG_GIVE                (D)
#define MPROG_FIGHT               (E)
#define MPROG_DEATH               (F)
#define MPROG_PULSE               (G)
#define MPROG_SPEECH              (H)
#define MPROG_ATTACK              (I)
#define MPROG_MOVE                (J)
#define MPROG_REQUEST		(K)

#define RPROG_SPEECH		  (A)
#define RPROG_ENTRY		(B)

struct  improg_type
{
    char *	type;
    char * 	name;
    void *	function;
};

struct iprog_data
{
  IPROG_FUN_WEAR *wear_prog;
  IPROG_FUN_REMOVE *remove_prog;
  IPROG_FUN_GET *get_prog;
  IPROG_FUN_DROP *drop_prog;
  IPROG_FUN_SAC *sac_prog;
  IPROG_FUN_ENTRY *entry_prog;
  IPROG_FUN_GIVE *give_prog;
  IPROG_FUN_GREET *greet_prog;
  IPROG_FUN_FIGHT *fight_prog;
  IPROG_FUN_DEATH *death_prog;
  IPROG_FUN_SPEECH *speech_prog;
  IPROG_FUN_PULSE *pulse_prog;
  IPROG_FUN_INVOKE *invoke_prog;
  char *   		wear_name;
  char *   		remove_name;
  char *   		get_name;
  char *   		drop_name;
  char *   		sac_name;
  char *   		entry_name;
  char *   		give_name;
  char *   		greet_name;
  char *   		fight_name;
  char *   		death_name;
  char *   		speech_name;
  char *   		pulse_name;
  char *		invoke_name;
};

/* new mprog shit */

struct mprog_data
{
  MPROG_FUN_BRIBE * bribe_prog;
  MPROG_FUN_ENTRY * entry_prog;
  MPROG_FUN_GIVE *  give_prog;
  MPROG_FUN_GREET * greet_prog;
  MPROG_FUN_FIGHT * fight_prog;
  MPROG_FUN_DEATH * death_prog;
  MPROG_FUN_PULSE * pulse_prog;
  MPROG_FUN_SPEECH * speech_prog;
  MPROG_FUN_ATTACK *attack_prog;
  MPROG_FUN_MOVE *move_prog;
  MPROG_FUN_REQUEST *request_prog;
  char *		attack_name;
  char *   		bribe_name;
  char *   		entry_name;
  char *   		give_name;
  char *   		greet_name;
  char *   		fight_name;
  char *   		death_name;
  char *   		pulse_name;
  char *   		speech_name;
  char *		move_name;
  char *		request_name;
};

struct rprog_data
{
    RPROG_FUN_SPEECH	* speech_prog;
    RPROG_FUN_ENTRY	* entry_prog;
    char 		* speech_name;
    char		* entry_name;
};


/*
 * Connected state for a channel.
 */
#define CON_PLAYING			0
#define CON_GET_NAME			1
#define CON_GET_OLD_PASSWORD		2
#define CON_CONFIRM_NEW_NAME		3
#define CON_GET_NEW_PASSWORD		4
#define CON_CONFIRM_NEW_PASSWORD	5
#define CON_GET_NEW_RACE		6
#define CON_GET_NEW_SEX			7
#define CON_GET_NEW_CLASS		8
#define CON_ROLLING_STATS               9
#define CON_GET_ALIGNMENT               10
#define CON_GET_ETHOS                   11
#define CON_GET_CABAL                   12
#define CON_DEFAULT_CHOICE              13
#define CON_GEN_GROUPS                  14
#define CON_NEW_CHAR                    15
#define CON_READ_IMOTD                  16
#define CON_READ_MOTD                   17
#define CON_BREAK_CONNECT               18
#define CON_COPYOVER_RECOVER			19

/*
 * Descriptor (channel) structure.
 */
struct	descriptor_data
{
    DESCRIPTOR_DATA *	next;
    DESCRIPTOR_DATA *	snoop_by;
    CHAR_DATA *		character;
    CHAR_DATA *		original;
    bool		valid;
    char *		host;
    sh_int		descriptor;
    sh_int		connected;
    bool		fcommand;
    char		inbuf		[4 * MAX_INPUT_LENGTH];
    char		incomm		[MAX_INPUT_LENGTH];
    char		inlast		[MAX_INPUT_LENGTH];
    int			repeat;
    char *		outbuf;
    int			outsize;
    int			outtop;
    char *		showstr_head;
    char *		showstr_point;
    void *		pEdit;		/* OLC */
    char **		pString;	/* OLC */
    int			editor;		/* OLC */
    char *		ip;
};



/*
 * Attribute bonus structures.
 */
struct	str_app_type
{
    sh_int	tohit;
    sh_int	todam;
    sh_int	carry;
    sh_int	wield;
};

struct	int_app_type
{
    sh_int	learn;
};

struct	wis_app_type
{
    sh_int	practice;
};

struct	dex_app_type
{
    sh_int	defensive;
    sh_int	carry;
    sh_int	tohit;
    sh_int	armor;
};

struct	con_app_type
{
    sh_int	hitp;
    sh_int	shock;
};



/*
 * TO types for act.
 */
#define TO_ROOM		    0
#define TO_NOTVICT	    1
#define TO_VICT		    2
#define TO_CHAR		    3
#define TO_ALL		    4
#define TO_IMMINROOM        5


/*
 * Help table types.
 */
struct	help_data
{
    HELP_DATA *	next;
    HELP_DATA *	next_area;
    sh_int	level;
    char *	keyword;
    char *	text;
    char *	file;
    bool delete;
};

struct help_area_data
{
	HELP_AREA *	next;
	HELP_DATA *	first;
	HELP_DATA *	last;
	AREA_DATA *	area;
	char *		filename;
	bool		changed;
};

/*
 * Shop types.
 */
#define MAX_TRADE	 5

struct	shop_data
{
    SHOP_DATA *	next;			/* Next shop in list		*/
    sh_int	keeper;			/* Vnum of shop keeper mob	*/
    sh_int	buy_type [MAX_TRADE];	/* Item types shop will buy	*/
    sh_int	profit_buy;		/* Cost multiplier for buying	*/
    sh_int	profit_sell;		/* Cost multiplier for selling	*/
    sh_int	open_hour;		/* First opening hour		*/
    sh_int	close_hour;		/* First closing hour		*/
};



/*
 * Per-class stuff.
 */

#define MAX_GUILD 	8
#define MAX_STATS 	5
#define STAT_STR 	0
#define STAT_INT	1
#define STAT_WIS	2
#define STAT_DEX	3
#define STAT_CON	4
#define MAX_SPECS       8
#define MAX_SPEC_SKILLS 1
#define MAX_ZOMBIE	10

struct	class_type
{
    char *	name;			/* the full name of the class */
    char 	who_name	[4];	/* Three-letter name for 'who'	*/
    sh_int      align;                  /* see pc_race_type */
    sh_int      xpadd;                  /* see pc_race_type */
    sh_int	weapon;			/* First weapon			*/
    sh_int	guild[MAX_GUILD];	/* Vnum of guild rooms		*/
    sh_int	skill_adept;		/* Maximum skill level		*/
    sh_int	thac0_00;		/* Thac0 for level  0		*/
    sh_int	thac0_32;		/* Thac0 for level 32		*/
    sh_int	hp_min;			/* Min hp gained on leveling	*/
    sh_int	hp_max;			/* Max hp gained on leveling	*/
    bool	fMana;			/* Class gains mana on level	*/
    char *	base_group;		/* base skills gained		*/
    char *	default_group;		/* default skills gained	*/
    sh_int	ctype;
};

struct item_type
{
    int		type;
    char *	name;
};

struct weapon_type
{
    char *	name;
    long	vnum;
    sh_int	type;
    sh_int	*gsn;
};

struct wiznet_type
{
    char *	name;
    long 	flag;
    int		level;
};

struct attack_type
{
    char *	name;			/* name */
    char *	noun;			/* message */
    int   	damage;			/* damage class */
    int		modifier;
};

struct race_type
{
    char *	name;			/* call name of the race */
    bool	pc_race;		/* can be chosen by pcs */
    long	act;			/* act bits for the race */
    long	aff;			/* aff bits for the race */
    long	off;			/* off bits for the race */
    long	imm;			/* imm bits for the race */
    long        res;			/* res bits for the race */
    long	vuln;			/* vuln bits for the race */
    long	form;			/* default form flag for the race */
    long	parts;			/* default parts for the race */
};


struct pc_race_type  /* additional data for pc races */
{
    char *	name;			/* MUST be in race_type */
    char 	who_name[6];
    sh_int      align;                  /* alignments race can have: 0=gne 1=gn 2=ne 3=g 4=n 5=e */
    sh_int      xpadd;                  /* extra xp per lvl needed */
    char *	skills[5];		/* bonus skills for the race */
    sh_int 	classes[13]; 		/* class restricts for races */
    sh_int 	stats[MAX_STATS];	/* starting stat modifiers */
    sh_int	max_stats[MAX_STATS];	/* maximum stats */
    sh_int	size;			/* aff bits for the race */
    bool	set_race;
};

struct spec_type
{
    char * 	name;			/* special function name */
    SPEC_FUN *	function;		/* the function */
};

/*
 * An affect.
 */
struct	affect_data
{
    AFFECT_DATA *	next;

	//Very dangerous to use. if the char data is freed before the affect data (i.e. log off), this char data will almost certainly segfault
	//somewhere. -- Zornath
	//CHAR_DATA *	        owner;

	char *				owner_name;
    char *              name;
    bool		valid;
	sh_int		wait;
    sh_int		where;
    sh_int		type;
    sh_int		level;
    sh_int		duration;
    sh_int		location;
    sh_int		modifier;
    int			bitvector;
    int			aftype;
    char *		affect_list_msg;
    bool		dispel_at_death;
    bool		strippable;
    AFF_FUN *           end_fun;
};

/*
 * A room affect.
 */
struct	room_affect_data
{
    ROOM_AFFECT_DATA *	next;
	char *		owner_name;
    char *		name;
    bool		valid;
    sh_int		where;
    sh_int		type;
    sh_int		level;
    sh_int		duration;
    sh_int		location;
    sh_int		modifier;
    int			bitvector;
    int			aftype;
    RAFF_FUN *		tick_fun;
    RAFF_FUN *		end_fun;
};

/* affect types */
#define AFT_SPELL			0
#define AFT_SKILL			1
#define AFT_POWER			2
#define AFT_MALADY 			3
#define AFT_COMMUNE			4
#define AFT_INVIS			5

/* class types */
#define CLASS_CASTER	0
#define CLASS_COMMUNER	1
#define CLASS_SING	2
#define CLASS_FIGHTER	3

/* const.c skill table types */
#define CMD_NONE 	0
#define CMD_SPELL 	1
#define CMD_COMMUNE 	2
#define CMD_POWER 	3
#define CMD_BOTH 	4

/* wear definitions */
#define TO_AFFECTS	0
#define TO_OBJECT	1
#define TO_IMMUNE	2
#define TO_RESIST	3
#define TO_VULN		4
#define TO_WEAPON	5
#define TO_WPN_V3	6

/* where definitions for room */
#define TO_ROOM_AFFECTS 0
#define TO_ROOM_CONST   1
#define TO_ROOM_FLAGS   2

/* room applies */
#define APPLY_ROOM_NONE 	0
#define APPLY_ROOM_HEAL 	1
#define APPLY_ROOM_MANA 	2
#define APPLY_ROOM_SECT		3

/* Cabal definitions */
#define CABAL_ANCIENT    	1
#define CABAL_KNIGHT       	2
#define CABAL_ARCANA     	3
#define CABAL_RAGER   		4
#define CABAL_OUTLAW     	5
#define CABAL_EMPIRE     	6
#define CABAL_BOUNTY		7
#define CABAL_SYLVAN		8
#define CABAL_ENFORCER		9

/* Empire definitions */
#define EMPIRE_ANATH	-1
#define EMPIRE_NONE	0
#define EMPIRE_PBLOOD	1
#define EMPIRE_BLOOD	2
#define EMPIRE_CITIZEN	3
#define EMPIRE_SLEADER	4
#define EMPIRE_EMPEROR	5
#define EMPIRE_IMM	6

#define MAX_EMPIRE		8
#define CABAL_LEADER     	5
#define MAX_BOUNTY		4

/*
 * A kill structure (indexed by level).
 */
struct	kill_data
{
    sh_int		number;
    sh_int		killed;
};

/*
 * New bits.
 */

typedef struct bmlist BMlist;

struct bmlist
{
        BMlist *next;
        long set;
        long tar_mask;
};

typedef struct bitmask
{
        long bits; // number of bits set in total.
        long masks; // number of masks in all.
        BMlist *int_list;
} BITMASK;

int             remove_bit      args((BITMASK *mask, sh_int bit));
int             set_bit         args((BITMASK *mask, sh_int bit));
int             is_set          args((BITMASK *mask, sh_int bit));
BITMASK         init_bitmask    args((BITMASK *bm));
void            load_bitmask    args((BITMASK *pBmask, FILE *fp));
void            save_bitmask    args((BITMASK *pBmask, FILE *fp));
void            load_old_bits   args((BITMASK *pBmask, FILE *fp));
void            multi_set_bit   args((BITMASK *bm, int bits));
BITMASK         dup_bitmask     args((BITMASK *bm));
void 			free_bitmask	args((BITMASK *pBmask));

/*
 * End new bits.
 */



/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (Start of section ... start here)                     *
 *                                                                         *
 ***************************************************************************/

/*
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
#define MOB_VNUM_WARRIOR_MERCENARY	8805
#define MOB_VNUM_THIEF_MERCENARY      	8806
#define MOB_VNUM_ASSASSIN_MERCENARY     8807
#define MOB_VNUM_SHAMAN_MERCENARY     	8808
#define MOB_VNUM_SHAMAN_MERCENARY	8808

#define MOB_VNUM_FIDO		   	3090
#define MOB_VNUM_CITYGUARD	   	3060
#define MOB_VNUM_VAMPIRE	   	3404

/* skills.are mobs */
#define MOB_VNUM_ZOMBIE		   	2901
#define MOB_VNUM_FIRE_ELEMENTAL	   	2902
#define MOB_VNUM_WATER_ELEMENTAL   	2903
#define MOB_VNUM_AIR_ELEMENTAL	   	2904
#define MOB_VNUM_EARTH_ELEMENTAL   	2905
#define MOB_VNUM_FALCON		   	2906
#define MOB_VNUM_WOLF		   	2907
#define MOB_VNUM_BEAR		   	2908
#define MOB_VNUM_LION		   	2909
#define MOB_VNUM_L_GOLEM	   	2911
#define MOB_VNUM_UNICORN	   	2912
#define MOB_VNUM_DRAGON	   	   	2913
#define MOB_VNUM_SHADOW		   	2914
#define MOB_VNUM_G_GOLEM	   	2915
#define MOB_VNUM_DECOY		  	2916
#define MOB_VNUM_ARMOR		  	2917
#define MOB_VNUM_WEAPON		  	2918
#define MOB_VNUM_KEY		  	2919
#define MOB_VNUM_SLAYER		   	2920
#define MOB_VNUM_SKELETON	   	2921
#define MOB_VNUM_APRAT		   	2949
#define MOB_VNUM_MUMMY		   	2922
#define MOB_VNUM_NIGHTFALL	   	2923
#define MOB_VNUM_SLAVE             	2924
#define MOB_VNUM_STALKER           	2925
#define MOB_VNUM_BEAST             	2950
#define MOB_VNUM_PHANTOM	   	2929
#define MOB_VNUM_ENFORCER	   	4524

#define MOB_VNUM_ARCANA	   	  	5800
#define MOB_VNUM_ANCIENT	   	3800
#define MOB_VNUM_KNIGHT		   	4501
#define MOB_VNUM_RAGER   	   	5700
#define MOB_VNUM_BOUNTY            	8800
#define MOB_VNUM_OUTLAW		   	9902
#define MOB_VNUM_ENFORCER_G		4521
#define MOB_VNUM_EMPIRE            	8103
#define MOB_VNUM_SYLVAN			8903

#define MOB_VNUM_PATROLMAN	   	2106
#define GROUP_VNUM_TROLLS	   	2100
#define GROUP_VNUM_OGRES	   	2101
#define MOB_VNUM_HYDRA 			13724
#define MOB_VNUM_BONES			13725
#define MOB_VNUM_DRANNOR_LADY		13726
#define MOB_VNUM_DRANNOR_KNIGHT 	13730
#define MOB_VNUM_PLRMOB 		1201
#define MOB_VNUM_CENTURION      	8102
#define MOB_VNUM_POINTMASTER   		3802
#define MOB_VNUM_KNIGHT_SQUIRE		4502
#define MOB_VNUM_SPITTING_SNAKE		113

/* RT ASCII conversions -- used so we can have letters in this file */

#define A		  	1
#define B			2
#define C			4
#define D			8
#define E			16
#define F			32
#define G			64
#define H			128

#define I			256
#define J			512
#define K		        1024
#define L		 	2048
#define M			4096
#define N		 	8192
#define O			16384
#define P			32768

#define Q			65536
#define R			131072
#define S			262144
#define T			524288
#define U			1048576
#define V			2097152
#define W			4194304
#define X			8388608

#define Y			16777216
#define Z			33554432
#define aa			67108864 	/* doubled due to conflicts */
#define bb			134217728
#define cc			268435456
#define dd			536870912
#define ee			1073741824
#define ff 			2147483648U
#define gg 			4294967296
#define hh			8589934592
#define ii			17179869184
/*
 * ACT bits for mobs.
 * Used in #MOBILES.
 */
#define ACT_IS_NPC              (A)             /* Auto set for mobs     */
#define ACT_SENTINEL            (B)             /* Stays in one room     */
#define ACT_SCAVENGER           (C)             /* Picks up objects      */
#define ACT_INNER_GUARDIAN      (D)             /* Attacks non-cabal  PC's*/
#define ACT_AGGRESSIVE          (F)             /* Attacks PC's          */
#define ACT_STAY_AREA           (G)             /* Won't leave area      */
#define ACT_WIMPY		(H)
#define ACT_PET                 (I)             /* Auto set for pets     */
#define ACT_TRAIN               (J)             /* Can train PC's        */
#define ACT_PRACTICE            (K)             /* Can practice PC's     */
#define ACT_NO_TRACK		(L)
#define ACT_UNDEAD		(O)
#define ACT_CLERIC		(Q)
#define ACT_MAGE		(R)
#define ACT_THIEF		(S)
#define ACT_WARRIOR		(T)
#define ACT_NOALIGN		(U)
#define ACT_NOPURGE		(V)
#define ACT_OUTDOORS		(W)
#define ACT_INDOORS		(Y)
#define ACT_OUTER_GUARDIAN	(Z)
#define ACT_IS_HEALER		(aa)
#define ACT_GAIN		(bb)
#define ACT_UPDATE_ALWAYS	(cc)
#define ACT_IS_CHANGER		(dd)
#define ACT_BANKER		(ee)

/* damage classes */
#define DAM_NONE                0
#define DAM_BASH                1
#define DAM_PIERCE              2
#define DAM_SLASH               3
#define DAM_FIRE                4
#define DAM_COLD                5
#define DAM_LIGHTNING           6
#define DAM_ACID                7
#define DAM_POISON              8
#define DAM_NEGATIVE            9
#define DAM_HOLY                10
#define DAM_ENERGY              11
#define DAM_MENTAL              12
#define DAM_DISEASE             13
#define DAM_DROWNING            14
#define DAM_LIGHT		15
#define DAM_OTHER               16
#define DAM_HARM		17
#define DAM_CHARM		18
#define DAM_SOUND		19
#define DAM_TRUESTRIKE  	20
#define DAM_EARTH		21
#define DAM_WIND		22

/* OFF bits for mobiles */
#define OFF_AREA_ATTACK         (A)
#define OFF_BACKSTAB            (B)
#define OFF_BASH                (C)
#define OFF_BERSERK             (D)
#define OFF_DISARM              (E)
#define OFF_DODGE               (F)
#define OFF_FADE                (G)
#define OFF_FAST                (H)
#define OFF_KICK                (I)
#define OFF_KICK_DIRT           (J)
#define OFF_PARRY               (K)
#define OFF_RESCUE              (L)
#define OFF_TAIL                (M)
#define OFF_TRIP                (N)
#define OFF_CRUSH		(O)
#define ASSIST_ALL       	(P)
#define ASSIST_ALIGN	        (Q)
#define ASSIST_RACE    	     	(R)
#define ASSIST_PLAYERS      	(S)
#define ASSIST_GUARD        	(T)
#define ASSIST_VNUM		(U)
#define STATIC_TRACKING		(W)
#define SPAM_MURDER		(X)
#define OFF_UNDEAD_DRAIN	(Z)
#define OFF_ATTACK_EVIL		(aa)
#define OFF_ATTACK_GOOD		(bb)


#define OFF_CHEAPSHOT		(A)
#define OFF_PARTING_BLOW	(B)
#define OFF_ASSASSIN		(C)
#define OFF_SWORD_SPEC		(D)
#define OFF_AXE_SPEC		(E)
#define OFF_HTH_SPEC		(F)
#define OFF_DAGGER_SPEC		(G)
#define OFF_SPEAR_SPEC		(H)
#define OFF_MACE_SPEC		(I)
#define OFF_WHIP_SPEC		(J)
#define OFF_POLEARM_SPEC	(K)

#define GROUP_SWORD		25
#define GROUP_AXE		26
#define GROUP_HTH		27
#define GROUP_DAGGER		28
#define GROUP_SPEAR		29
#define GROUP_MACE		30
#define GROUP_WHIP		21
#define GROUP_POLEARM		32

/* paladin dedication skills */
#define GROUP_DED_TWOHAND	34
#define GROUP_DED_SHIELD	33

/* return values for check_imm */
#define IS_NORMAL		0
#define IS_IMMUNE		1
#define IS_RESISTANT		2
#define IS_VULNERABLE		3

/* IMM bits for mobs */
#define IMM_SUMMON              (A)
#define IMM_CHARM               (B)
#define IMM_MAGIC               (C)
#define IMM_WEAPON              (D)
#define IMM_BASH                (E)
#define IMM_PIERCE              (F)
#define IMM_SLASH               (G)
#define IMM_FIRE                (H)
#define IMM_COLD                (I)
#define IMM_LIGHTNING           (J)
#define IMM_ACID                (K)
#define IMM_POISON              (L)
#define IMM_NEGATIVE            (M)
#define IMM_HOLY                (N)
#define IMM_ENERGY              (O)
#define IMM_MENTAL              (P)
#define IMM_DISEASE             (Q)
#define IMM_DROWNING            (R)
#define IMM_LIGHT		(S)
#define IMM_SOUND		(T)
#define IMM_WOOD                (X)
#define IMM_SILVER              (Y)
#define IMM_IRON                (Z)
#define IMM_SLEEP		(aa)

/* RES bits for mobs */
#define RES_SUMMON		(A)
#define RES_CHARM		(B)
#define RES_MAGIC               (C)
#define RES_WEAPON              (D)
#define RES_BASH                (E)
#define RES_PIERCE              (F)
#define RES_SLASH               (G)
#define RES_FIRE                (H)
#define RES_COLD                (I)
#define RES_LIGHTNING           (J)
#define RES_ACID                (K)
#define RES_POISON              (L)
#define RES_NEGATIVE            (M)
#define RES_HOLY                (N)
#define RES_ENERGY              (O)
#define RES_MENTAL              (P)
#define RES_DISEASE             (Q)
#define RES_DROWNING            (R)
#define RES_LIGHT		(S)
#define RES_SOUND		(T)
#define RES_WOOD                (X)
#define RES_SILVER              (Y)
#define RES_IRON                (Z)

/* VULN bits for mobs */
#define VULN_SUMMON		(A)
#define VULN_CHARM		(B)
#define VULN_MAGIC              (C)
#define VULN_WEAPON             (D)
#define VULN_BASH               (E)
#define VULN_PIERCE             (F)
#define VULN_SLASH              (G)
#define VULN_FIRE               (H)
#define VULN_COLD               (I)
#define VULN_LIGHTNING          (J)
#define VULN_ACID               (K)
#define VULN_POISON             (L)
#define VULN_NEGATIVE           (M)
#define VULN_HOLY               (N)
#define VULN_ENERGY             (O)
#define VULN_MENTAL             (P)
#define VULN_DISEASE            (Q)
#define VULN_DROWNING           (R)
#define VULN_LIGHT		(S)
#define VULN_SOUND		(T)
#define VULN_WOOD               (X)
#define VULN_SILVER             (Y)
#define VULN_IRON		(Z)

/* body form */
#define FORM_EDIBLE             (A)
#define FORM_POISON             (B)
#define FORM_MAGICAL            (C)
#define FORM_INSTANT_DECAY      (D)
#define FORM_OTHER              (E)  /* defined by material bit */

/* actual form */
#define FORM_ANIMAL             (G)
#define FORM_SENTIENT           (H)
#define FORM_UNDEAD             (I)
#define FORM_CONSTRUCT          (J)
#define FORM_MIST               (K)
#define FORM_INTANGIBLE         (L)

#define FORM_BIPED              (M)
#define FORM_CENTAUR            (N)
#define FORM_INSECT             (O)
#define FORM_SPIDER             (P)
#define FORM_CRUSTACEAN         (Q)
#define FORM_WORM               (R)
#define FORM_BLOB		(S)

#define FORM_MAMMAL             (V)
#define FORM_BIRD               (W)
#define FORM_REPTILE            (X)
#define FORM_SNAKE              (Y)
#define FORM_DRAGON             (Z)
#define FORM_AMPHIBIAN          (aa)
#define FORM_FISH               (bb)
#define FORM_COLD_BLOOD		(cc)

/* body parts */
#define PART_HEAD               (A)
#define PART_ARMS               (B)
#define PART_LEGS               (C)
#define PART_HEART              (D)
#define PART_BRAINS             (E)
#define PART_GUTS               (F)
#define PART_HANDS              (G)
#define PART_FEET               (H)
#define PART_FINGERS            (I)
#define PART_EAR                (J)
#define PART_EYE		(K)
#define PART_LONG_TONGUE        (L)
#define PART_EYESTALKS          (M)
#define PART_TENTACLES          (N)
#define PART_FINS               (O)
#define PART_WINGS              (P)
#define PART_TAIL               (Q)
/* for combat */
#define PART_CLAWS              (U)
#define PART_FANGS              (V)
#define PART_HORNS              (W)
#define PART_SCALES             (X)
#define PART_TUSKS		(Y)

/*
 * Bits for 'affected_by'.
 * Used in #MOBILES.
 */
#define AFF_BLIND               (A)
#define AFF_INVISIBLE           (B)
#define AFF_DETECT_EVIL         (C)
#define AFF_DETECT_INVIS        (D)
#define AFF_DETECT_MAGIC        (E)
#define AFF_DETECT_HIDDEN       (F)
#define AFF_DETECT_GOOD         (G)
#define AFF_SANCTUARY           (H)
#define AFF_FAERIE_FIRE         (I)
#define AFF_INFRARED            (J)
#define AFF_CURSE               (K)
#define AFF_LOOKING_GLASS       (L)
#define AFF_POISON              (M)
#define AFF_PROTECT_EVIL        (N)
#define AFF_PROTECT_GOOD        (O)
#define AFF_SNEAK               (P)
#define AFF_HIDE                (Q)
#define AFF_SLEEP               (R)
#define AFF_CHARM               (S)
#define AFF_FLYING              (T)
#define AFF_PASS_DOOR           (U)
#define AFF_HASTE               (V)
#define AFF_CALM                (W)
#define AFF_PLAGUE              (X)
#define AFF_WEAKEN              (Y)
#define AFF_DARK_VISION         (Z)
#define AFF_BERSERK             (aa)
#define AFF_WATERBREATHING      (bb)
#define AFF_REGENERATION        (cc)
#define AFF_SLOW                (dd)
#define AFF_CAMOUFLAGE          (ee)



/*
 * *AFF* bits for rooms
 */
#define AFF_ROOM_RANDOMIZER             (A)
#define AFF_ROOM_CURSE                  (K)
#define AFF_ROOM_POISON                 (M)
#define AFF_ROOM_SLEEP                  (R)
#define AFF_ROOM_PLAGUE                 (X)
#define AFF_ROOM_SLOW                   (dd)
#define AFF_ROOM_BLIZZARD		(C)
#define AFF_ROOM_ELECTRIC_STORM		(D)
#define AFF_ROOM_ICE_SLICK		(E)
#define AFF_ROOM_VACUME			(F)
#define AFF_ROOM_WALL_THORNS		(G)
#define AFF_ROOM_TRIPWIRE		(H)

/*
 * Sex.
 * Used in #MOBILES.
 */
#define SEX_NEUTRAL		      0
#define SEX_MALE		      1
#define SEX_FEMALE		      2

/* AC types */
#define AC_PIERCE			0
#define AC_BASH				1
#define AC_SLASH			2
#define AC_EXOTIC			3

//Damage is decreased by 1% per -75 points of ac
#define AC_PER_ONE_PERCENT_DECREASE_DAMAGE -75.0

//Weight standard for materials. Relative values are taken with respect to this.
#define WEIGHT_STANDARD		30

/* dice */
#define DICE_NUMBER			0
#define DICE_TYPE			1
#define DICE_BONUS			2

/* size */
#define SIZE_TINY			0
#define SIZE_SMALL			1
#define SIZE_MEDIUM			2
#define SIZE_LARGE			3
#define SIZE_HUGE			4
#define SIZE_GIANT			5



/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */

#define OBJ_VNUM_OUTLAW		    9902
#define OBJ_VNUM_ENFORCER	    4521
#define OBJ_VNUM_RAGER	            5701
#define OBJ_VNUM_ARCANA	  	    5801
#define OBJ_VNUM_ANCIENT	    3801
#define OBJ_VNUM_BOUNTY          8800
#define OBJ_VNUM_KNIGHT		    4502
#define OBJ_VNUM_EMPIRE		    8101
#define OBJ_VNUM_SYLVAN		    8900

#define KEY_ANCIENT		3800
#define KEY_ARCANA		5802
#define KEY_RAGER		5700
#define KEY_KNIGHT		4500
#define KEY_ENFORCER		23602
#define KEY_OUTLAW		9900
#define KEY_BOUNTY           8800

#define OBJ_VNUM_GOLD_ONE	      2
#define OBJ_VNUM_GOLD_SOME	      3
#define OBJ_VNUM_COINS		      5
#define OBJ_VNUM_SACK		      2901
#define OBJ_VNUM_STEAK		      2902
#define OBJ_VNUM_POTION		      2903
#define OBJ_VNUM_RANGER_STAFF	      2905

#define OBJ_VNUM_RANGER_MAPLE         2963
#define OBJ_VNUM_RANGER_MAGIC         2964
#define OBJ_VNUM_RANGER_OAK           2965

#define OBJ_VNUM_AVENGER	      2906
#define OBJ_VNUM_PONCHO		      2907
#define OBJ_VNUM_PROTECTORATE	      2912
#define OBJ_VNUM_BADGE		      2913
#define OBJ_VNUM_SKELETON	      2925
#define OBJ_VNUM_CHAOS_BLADE          9903
#define OBJ_VNUM_ANCIENT_WHIP		  3810
#define OBJ_VNUM_ANCIENT_SWORD		  3811
#define OBJ_VNUM_ANCIENT_DAGGER		  3812
#define OBJ_VNUM_WOOD_MAPLE           2960
#define OBJ_VNUM_WOOD_MAGIC			  2961
#define OBJ_VNUM_WOOD_OAK			  2962

#define OBJ_VNUM_THIEF_PLANT_EXPLOSIVE	218

/* Lich elixirs and phylacteries */
#define OBJ_VNUM_CORRECT_LICH_ELIXIR		200
#define OBJ_VNUM_INCORRECT_LICH_ELIXIR		201
#define OBJ_VNUM_PHYLACTERY_BLACK_SAPPHIRE	2969
#define OBJ_VNUM_PHYLACTERY_FIRE_OPAL		2970
#define OBJ_VNUM_PHYLACTERY_BLACK_OPAL		2971
#define OBJ_VNUM_PHYLACTERY_SOAPSTONE		2972
#define OBJ_VNUM_PHYLACTERY_BLOODSTONE		2973
#define OBJ_VNUM_PHYLACTERY_BLACK_PEARL		2974
#define OBJ_VNUM_PHYLACTERY_DIAMOND		2975
#define OBJ_VNUM_PHYLACTERY_PALE_JADE		2976
#define OBJ_VNUM_PHYLACTERY_VIOLET_GARNET	2977
#define OBJ_VNUM_PHYLACTERY_SPHERICAL_MALACHITE	2978
#define OBJ_VNUM_PHYLACTERY_SUNSTONE		2979
#define OBJ_VNUM_PHYLACTERY_ZIRCON		2980
#define OBJ_VNUM_PHYLACTERY_STRIPED_GLASS	2981
#define OBJ_VNUM_PHYLACTERY_BASALT		2982
#define OBJ_VNUM_PHYLACTERY_STAR_DIOPSIDE	2983
#define OBJ_VNUM_PHYLACTERY_ROUNDED_JADE	2984
#define OBJ_VNUM_PHYLACTERY_MALACHITE_DISC	2985
#define OBJ_VNUM_PHYLACTERY_PALE_AQUAMARINE	2986
#define OBJ_VNUM_PHYLACTERY_GRANITE		2987
#define OBJ_VNUM_PHYLACTERY_AMETHYST		2988
#define OBJ_VNUM_PHYLACTERY_MILKY_QUARTZ	2989
#define OBJ_VNUM_PHYLACTERY_TAUROIDAL_BERYL	2990
#define OBJ_VNUM_PHYLACTERY_SOMETHING_OILY	2991
#define OBJ_VNUM_PHYLACTERY_ANGLED_JASPER	2992
#define OBJ_VNUM_PHYLACTERY_JADE		2993
#define OBJ_VNUM_PHYLACTERY_CATS_EYE		2994
#define OBJ_VNUM_PHYLACTERY_CATS_EYE_AGATE	2995
#define OBJ_VNUM_PHYLACTERY_FACETED_JASPER	2996
#define OBJ_VNUM_PHYLACTERY_RAINBOW_MOONSTONE	2997
#define OBJ_VNUM_PHYLACTERY_BLACK_DIAMOND	2998

/* Quest store objects */
#define OBJ_VNUM_QUEST_STORE_WARRIOR_BOOTS	2966
#define OBJ_VNUM_QUEST_STORE_WARRIOR_GAUNTLETS	2967
#define OBJ_VNUM_QUEST_STORE_WARRIOR_PLATE	2968
#define OBJ_VNUM_QUEST_STORE_WARRIOR_WEAPON	2999
#define OBJ_VNUM_QUEST_STORE_CHANNELER_NECK	212
#define OBJ_VNUM_QUEST_STORE_CHANNELER_ANKH	211
#define OBJ_VNUM_QUEST_STORE_CHANNELER_ROBE	210
#define OBJ_VNUM_QUEST_STORE_CHANNELER_HELM	209
#define OBJ_VNUM_QUEST_STORE_THIEF_BOOTS		214
#define OBJ_VNUM_QUEST_STORE_THIEF_ROBE		215
#define OBJ_VNUM_QUEST_STORE_THIEF_DAGGER		216
#define OBJ_VNUM_QUEST_STORE_THIEF_BELT		217
#define OBJ_VNUM_QUEST_STORE_PILL_RECALL		202
#define OBJ_VNUM_QUEST_STORE_PILL_CURE_POISON	203
#define OBJ_VNUM_QUEST_STORE_PILL_CURE_DISEASE	204
#define OBJ_VNUM_QUEST_STORE_PILL_REMOVE_CURSE	205
#define OBJ_VNUM_QUEST_STORE_PILL_TELEPORT	206
#define OBJ_VNUM_QUEST_STORE_PILL_FLYING	207
#define OBJ_VNUM_QUEST_STORE_PILL_PASS_DOOR	208
#define OBJ_VNUM_QUEST_STORE_HEALER_RING 246
#define OBJ_VNUM_QUEST_STORE_HEALER_SLEEVES 247
#define OBJ_VNUM_QUEST_STORE_HEALER_BRACELET 248
#define OBJ_VNUM_QUEST_STORE_HEALER_STATUE 249
#define OBJ_VNUM_QUEST_STORE_ELEMENTALIST_FLAME 253
#define OBJ_VNUM_QUEST_STORE_ELEMENTALIST_BELT 250
#define OBJ_VNUM_QUEST_STORE_ELEMENTALIST_NECK 252
#define OBJ_VNUM_QUEST_STORE_ELEMENTALIST_HELM 251




#define OBJ_VNUM_CORPSE_NPC	     10
#define OBJ_VNUM_CORPSE_PC	     11
#define OBJ_VNUM_SEVERED_HEAD	     12
#define OBJ_VNUM_TORN_HEART	     13
#define OBJ_VNUM_SLICED_ARM	     14
#define OBJ_VNUM_SLICED_LEG	     15
#define OBJ_VNUM_GUTS		     16
#define OBJ_VNUM_BRAINS		     17

#define OBJ_VNUM_MUSHROOM	     20
#define OBJ_VNUM_LIGHT_BALL	     21
#define OBJ_VNUM_SPRING		     22
#define OBJ_VNUM_DISC		     23
#define OBJ_VNUM_PORTAL		     25

#define OBJ_VNUM_ROSE		   1001
#define OBJ_VNUM_ARENA_TROPHY     26010
#define OBJ_VNUM_ANCIENT_SCALP	45

#define OBJ_VNUM_PIT		   3010
#define PIT_NEWTHALOS		   9603
#define PIT_OFCOL		   671
#define PIT_ARKHAM		   765

#define PIT_DEFAULT		   3010


#define OBJ_VNUM_SCHOOL_MACE	   3700
#define OBJ_VNUM_SCHOOL_DAGGER	   3701
#define OBJ_VNUM_SCHOOL_SWORD	   3702
#define OBJ_VNUM_SCHOOL_SPEAR	   3717
#define OBJ_VNUM_SCHOOL_STAFF	   3718
#define OBJ_VNUM_SCHOOL_AXE	   3719
#define OBJ_VNUM_SCHOOL_FLAIL	   3720
#define OBJ_VNUM_SCHOOL_WHIP	   3721
#define OBJ_VNUM_SCHOOL_POLEARM    3722

#define OBJ_VNUM_SCHOOL_VEST	   3703
#define OBJ_VNUM_SCHOOL_SHIELD	   3704
#define OBJ_VNUM_SCHOOL_BANNER     3716
#define OBJ_VNUM_MAP		   3162
#define OBJ_VNUM_MAP_THERA	   3163
#define OBJ_VNUM_WHISTLE	   2116

#define OBJ_VNUM_RING_EMPOWER 	   3810
#define OBJ_VNUM_ARCPOTION	    2920
#define OBJ_VNUM_ARCSCROLL	    2921
#define OBJ_VNUM_ARCWAND	    2922
#define OBJ_VNUM_ARCSTAFF	    2923
#define OBJ_VNUM_BERRY		   2924

#define OBJ_VNUM_HAIR_BRACELET	13725
#define OBJ_VNUM_BLOOD_BRACELET	13720
#define OBJ_VNUM_TIARA		23231
#define OBJ_VNUM_WIZARDRY_1	23236
#define OBJ_VNUM_WIZARDRY_2	23238
#define OBJ_VNUM_SWORD_PLANES	13714
#define OBJ_VNUM_HEAVENLY_SCEPTRE	23263
#define OBJ_VNUM_MACE_DISRUPTION	23261
#define OBJ_VNUM_ROD_LORDLY	13717
#define OBJ_VNUM_CROWN_BLOOD	13716
#define OBJ_VNUM_RED_IOUN		1
#define OBJ_VNUM_BLUE_IOUN		1
#define OBJ_VNUM_GREEN_IOUN		1
#define OBJ_VNUM_WINGS		13711
#define OBJ_VNUM_HYDRA_SLAYER	13738
#define OBJ_VNUM_HYDRA_HEAD	13737
#define OBJ_VNUM_BLACK_KEY	13736
#define OBJ_VNUM_HORN_VALERE	13739


#define OBJ_VNUM_DRAGONSWORD  4503
#define OBJ_VNUM_DRAGONPLATE  4506
#define OBJ_VNUM_DRAGON_WEAPON 9
#define OBJ_VNUM_EMPERORTHRONE 8102

#define OBJ_VNUM_SHIELD_ORDER	4523

/*
 * Item types.
 * Used in #OBJECTS.
 */
#define ITEM_LIGHT		      1
#define ITEM_SCROLL		      2
#define ITEM_WAND		      3
#define ITEM_STAFF		      4
#define ITEM_WEAPON		      5
#define ITEM_NULL6                    6
#define ITEM_NULL7                    7
#define ITEM_TREASURE		      8
#define ITEM_ARMOR		      9
#define ITEM_POTION		     10
#define ITEM_CLOTHING		     11
#define ITEM_FURNITURE		     12
#define ITEM_TRASH		     13
#define ITEM_CONTAINER		     15
#define ITEM_DRINK_CON		     17
#define ITEM_KEY		     18
#define ITEM_FOOD		     19
#define ITEM_MONEY		     20
#define ITEM_BOAT		     22
#define ITEM_CORPSE_NPC		     23
#define ITEM_CORPSE_PC		     24
#define ITEM_FOUNTAIN		     25
#define ITEM_PILL		     26
#define ITEM_PROTECT		     27
#define ITEM_MAP		     28
#define ITEM_PORTAL		     29
#define ITEM_WARP_STONE		     30
#define ITEM_ROOM_KEY		     31
#define ITEM_GEM		     32
#define ITEM_JEWELRY		     33
#define ITEM_JUKEBOX		     34
#define ITEM_CABAL_ITEM              35
#define ITEM_SKELETON	             36
#define ITEM_INSTRUMENT              37
#define ITEM_ARROW		     38
#define ITEM_BOW		     39

/*
 * Extra flags.
 * Used in #OBJECTS.
 */
#define ITEM_GLOW		(1)
#define ITEM_HUM		(2)
#define ITEM_DARK		(3)
#define ITEM_LOCK		(4)
#define ITEM_EVIL		(5)
#define ITEM_INVIS		(6)
#define ITEM_MAGIC		(7)
#define ITEM_NODROP		(8)
#define ITEM_BLESS		(9)
#define ITEM_ANTI_GOOD		(10)
#define ITEM_ANTI_EVIL		(11)
#define ITEM_ANTI_NEUTRAL	(12)
#define ITEM_NOREMOVE		(13)
#define ITEM_INVENTORY		(14)
#define ITEM_NOPURGE		(15)
#define ITEM_ROT_DEATH		(16)
#define ITEM_VIS_DEATH		(17)
#define ITEM_FIXED              (18)
#define ITEM_NODISARM		(19)
#define ITEM_NOLOCATE		(20)
#define ITEM_MELT_DROP		(21)
#define ITEM_HAD_TIMER		(22)
#define ITEM_SELL_EXTRACT	(23)
#define ITEM_METAL		(24)
#define ITEM_BURN_PROOF		(25)
#define ITEM_NOUNCURSE		(26)
#define ITEM_BRAND              (27)
#define CORPSE_NO_ANIMATE	(28)
#define ITEM_HIDDEN		(29)
#define ITEM_ANTI_NEUT		(30)
#define ITEM_ANTI_CHAOTIC	(31)
#define ITEM_MALEDICTED		(32)
/*
 * Wear flags.
 * Used in #OBJECTS.
 */
#define ITEM_TAKE		(A)
#define ITEM_WEAR_FINGER	(B)
#define ITEM_WEAR_NECK		(C)
#define ITEM_WEAR_BODY		(D)
#define ITEM_WEAR_HEAD		(E)
#define ITEM_WEAR_LEGS		(F)
#define ITEM_WEAR_FEET		(G)
#define ITEM_WEAR_HANDS		(H)
#define ITEM_WEAR_ARMS		(I)
#define ITEM_WEAR_SHIELD	(J)
#define ITEM_WEAR_ABOUT		(K)
#define ITEM_WEAR_WAIST		(L)
#define ITEM_WEAR_WRIST		(M)
#define ITEM_WIELD		(N)
#define ITEM_HOLD		(O)
#define ITEM_NO_SAC		(P)
#define ITEM_WEAR_BRAND         (R)
#define ITEM_WEAR_STRAPPED	(S)

/* object restrict_flags */
#define HUMAN_ONLY		(A)
#define DWARF_ONLY		(B)
#define ELF_ONLY		(C)
#define BARD_ONLY		(D)
#define DARK_ONLY		(E)
#define GIANT_ONLY		(F)
#define CENTAUR_ONLY		(G)
#define TROLL_ONLY		(H)
#define DRACONIAN_ONLY		(I)
#define GNOME_ONLY		(J)
#define WARRIOR_ONLY		(K)
#define THIEF_ONLY		(L)
#define ZEALOT_ONLY		(M)
#define PALADIN_ONLY		(N)
#define ANTI_PALADIN_ONLY	(O)
#define RANGER_ONLY		(P)
//#define (Q)
#define CHANNELER_ONLY		(R)
#define ASSASSIN_ONLY	        (S)
#define NECROMANCER_ONLY	(T)
#define ELEMENTALIST_ONLY	(U)
#define HEALER_ONLY		(Z)

#define ANCIENT_ONLY		(V)
#define ARCANA_ONLY		(W)
#define RAGER_ONLY		(X)
#define KNIGHT_ONLY		(Y)
//#define (Z)
#define OUTLAW_ONLY		(aa)
#define MAGE_ONLY		(bb)
#define NEWBIE_ONLY		(cc)
#define BOUNTY_ONLY          (dd)
#define SYLVAN_ONLY		(ee)

/* Specialization Warrior Skills swarrior */
#define STYLE_NONE              0
#define WSHAND           (A)
#define WSSWORD          (B)
#define WSDAGGER         (C)
#define WSSPEAR          (D)
#define WSMACE           (E)
#define WSAXE            (F)
#define WSFLAIL          (G)
#define WSWHIP           (H)
#define WSPOLEARM        (I)
#define WSSTAFF          (J)
#define MAX_STYLE               11

/* Paladin Dedications */
#define NO_DEDICATION		0
#define DED_TWOHAND		1
#define DED_SHIELD		2

/* Quest Store Definitions*/
/*Types of rewards*/
#define REWARD_ARMOR		0
#define REWARD_WEAPON		1
#define REWARD_BOOST		2
#define REWARD_MISCELLANEOUS	3
/*Set item or not*/
#define SET_ITEM		1
#define NOT_SET_ITEM		0
/*Which set?*/
#define SET_NONE		0
#define SET_WARRIOR		1
#define SET_CHANNELER		2
#define SET_THIEF			3
#define SET_HEALER		4
#define SET_ELEMENTALIST	5

/* weapon class */
#define WEAPON_EXOTIC		0
#define WEAPON_SWORD		1
#define WEAPON_DAGGER		2
#define WEAPON_SPEAR		3
#define WEAPON_MACE		4
#define WEAPON_AXE		5
#define WEAPON_FLAIL		6
#define WEAPON_WHIP		7
#define WEAPON_POLEARM		8
#define WEAPON_STAFF		9
#define WEAPON_HAND            10

/* weapon types */
#define WEAPON_FLAMING		(A)
#define WEAPON_FROST		(B)
#define WEAPON_VAMPIRIC		(C)
#define WEAPON_SHARP		(D)
#define WEAPON_VORPAL		(E)
#define WEAPON_TWO_HANDS	(F)
#define WEAPON_SHOCKING		(G)
#define WEAPON_POISON		(H)
#define WEAPON_AVENGER		(I)
#define WEAPON_SHADOWBANE	(J)
#define WEAPON_LIGHTBRINGER	(K)
#define WEAPON_UNHOLY		(L)
#define WEAPON_LEADER		(M)
#define WEAPON_UNHOLY_BLESS	(N)


/* gate flags */
#define GATE_NORMAL_EXIT	(A)
#define GATE_NOCURSE		(B)
#define GATE_GOWITH		(C)
#define GATE_BUGGY		(D)
#define GATE_RANDOM		(E)

/* furniture flags */
#define STAND_AT		(A)
#define STAND_ON		(B)
#define STAND_IN		(C)
#define SIT_AT			(D)
#define SIT_ON			(E)
#define SIT_IN			(F)
#define REST_AT			(G)
#define REST_ON			(H)
#define REST_IN			(I)
#define SLEEP_AT		(J)
#define SLEEP_ON		(K)
#define SLEEP_IN		(L)
#define PUT_AT			(M)
#define PUT_ON			(N)
#define PUT_IN			(O)
#define PUT_INSIDE		(P)




/*
 * Apply types (for affects).
 * Used in #OBJECTS.
 */
#define APPLY_NONE		      	0
#define APPLY_STR		      	1
#define APPLY_DEX		      	2
#define APPLY_INT		      	3
#define APPLY_WIS		      	4
#define APPLY_CON		      	5
#define APPLY_SEX		      	6
#define APPLY_CLASS		      	7
#define APPLY_LEVEL		      	8
#define APPLY_AGE		      	9
#define APPLY_HEIGHT		     	10
#define APPLY_WEIGHT		     	11
#define APPLY_MANA		     	12
#define APPLY_HIT		     	13
#define APPLY_MOVE		     	14
#define APPLY_GOLD		     	15
#define APPLY_EXP		     	16
#define APPLY_AC		     	17
#define APPLY_HITROLL		     	18
#define APPLY_DAMROLL		     	19
#define APPLY_SAVES		     	20
#define APPLY_SAVING_PARA	     	20
#define APPLY_SAVING_ROD	     	21
#define APPLY_SAVING_PETRI	     	22
#define APPLY_SAVING_BREATH	     	23
#define APPLY_SAVING_SPELL	     	24
#define APPLY_SPELL_AFFECT	     	25
#define APPLY_REGENERATION	     	27
#define APPLY_DAM_MOD			28
#define APPLY_ATTACK_MOD		29
#define APPLY_ENHANCED_MOD		30
#define APPLY_SIZE			31

/*
 * Object only apply types.
 */
#define APPLY_OBJ_V3			1

/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define CONT_CLOSEABLE		      1
#define CONT_PICKPROOF		      2
#define CONT_CLOSED		      4
#define CONT_LOCKED		      8
#define CONT_PUT_ON		     16

#define WIELD_ONE                     1
#define WIELD_TWO                     2
#define WIELD_PRIMARY                 3

#define HAS_DIED	8

/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
#define ROOM_VNUM_DEATH_GOOD		1212
#define ROOM_VNUM_DEATH_NEUTRAL		1212
#define ROOM_VNUM_DEATH_EVIL		1212

/* These are the cabal rooms where if you put another Cabal's item down
that cabal will lose it's powers. Tailor these for you cabals then make
cabal items for the cabal (these are made by setting the object with the
cabal item setting).
*/
#define ROOM_VNUM_OUTLAW	    9900
#define ROOM_VNUM_ENFORCER	    4523
#define ROOM_VNUM_RAGER	    5706
#define ROOM_VNUM_ARCANA	    5800
#define ROOM_VNUM_ANCIENT	    3804
#define ROOM_VNUM_KNIGHT		    4500
#define ROOM_VNUM_BOUNTY                   8805
#define ROOM_VNUM_SYLVAN		8907

#define ROOM_VNUM_NIGHTWALK	    2900
#define ROOM_VNUM_LIMBO		      2
#define ROOM_VNUM_CHAT		   1200
#define ROOM_VNUM_TEMPLE	   3001
#define ROOM_VNUM_ALTAR		   3054
#define ROOM_VNUM_SCHOOL	   3700
#define ROOM_VNUM_VENUEPORT	   9302
#define ROOM_FORCE_DUEL 	   4510
#define ROOM_VNUM_STATUE_SACRIFICE 1224

/* temple vnums for hometowns */
#define TEMPLE_MIDGAARD		3054
#define TEMPLE_NEW_THALOS	9609
#define TEMPLE_OFCOL		699
#define TEMPLE_ARKHAM		768
#define TEMPLE_NEWBIE		3058
/* recall vnums for cabal recalls */
#define ROOM_VNUM_OUTLAWT	    9904
#define ROOM_VNUM_ENFORCERT	    23603
#define ROOM_VNUM_RAGERT	    5706
#define ROOM_VNUM_ARCANAT	    5800
#define ROOM_VNUM_ANCIENTT	    3804
#define ROOM_VNUM_KNIGHTT	    4504


/* Set HOMETOWNS as the room in which you want the 'hometown' command to
function. This will be the only room in the game where players can change
their hometown after choosing it at creation.
*/
#define ROOM_VNUM_HOMETOWNS	3008

/*
 * Room flags.
 * Used in #ROOMS.
 */
#define ROOM_DARK		(A)
#define ROOM_NO_MOB		(C)
#define ROOM_NO_NEWBIE		(B)
#define ROOM_INDOORS		(D)
#define ROOM_CABAL              (E)
#define ROOM_PRIVATE		(J)
#define ROOM_SAFE		(K)
#define ROOM_SOLITARY		(L)
#define ROOM_PET_SHOP		(M)
#define ROOM_NO_RECALL		(N)
#define ROOM_IMP_ONLY		(O)
#define ROOM_GODS_ONLY		(P)
#define ROOM_HEROES_ONLY	(Q)
#define ROOM_NEWBIES_ONLY	(R)
#define ROOM_LAW		(S)
#define ROOM_NOWHERE		(T)
#define ROOM_NO_GATE		(U)
#define ROOM_CONSECRATED	(V)
#define ROOM_NO_SUMMON		(W)
#define ROOM_NO_CONSECRATE	(X)
#define ROOM_NO_ALARM		(Z)
#define ROOM_LOW_ONLY		(bb)
#define ROOM_NO_MAGIC		(cc)


/* Room extra_room_flags */
#define ROOM_GLOBE_DARKNESS	(A)

/*
 * Directions.
 * Used in #ROOMS.
 */
#define DIR_NORTH		      0
#define DIR_EAST		      1
#define DIR_SOUTH		      2
#define DIR_WEST		      3
#define DIR_UP			      4
#define DIR_DOWN		      5



/*
 * Exit flags.
 * Used in #ROOMS.
 */
#define EX_ISDOOR		      	(A)
#define EX_CLOSED		      	(B)
#define EX_LOCKED		      	(C)
#define EX_PICKPROOF		      	(F)
#define EX_NOPASS		      	(G)
#define EX_EASY			      	(H)
#define EX_HARD			      	(I)
#define EX_INFURIATING		      	(J)
#define EX_NOCLOSE		      	(K)
#define EX_NOLOCK		      	(L)
#define EX_NOBASH		      	(M)
#define EX_NONOBVIOUS		      	(N)
#define EX_CURRENT			(O)

/*
 * Sector types.
 * Used in #ROOMS.
 */
#define SECT_INSIDE		      	0
#define SECT_CITY		      	1
#define SECT_FIELD		      	2
#define SECT_FOREST		      	3
#define SECT_HILLS		      	4
#define SECT_MOUNTAIN		      	5
#define SECT_WATER_SWIM		      	6
#define SECT_WATER_NOSWIM	      	7
#define SECT_UNUSED		      	8
#define SECT_AIR		      	9
#define SECT_DESERT		     	10
#define SECT_ROAD			11
#define SECT_UNDERWATER			12
#define SECT_MAX		     	13

/* Alignment selections */
#define ALIGN_NONE	0
#define ALIGN_G		1
#define ALIGN_N		2
#define ALIGN_GN	3
#define ALIGN_E		4
#define ALIGN_GE        5
#define ALIGN_NE	6
#define ALIGN_ANY       7

/* Ethos selections */
#define ETHOS_NONE	0
#define ETHOS_L		1
#define ETHOS_N		2
#define ETHOS_LN	3
#define ETHOS_C		4
#define ETHOS_LC        5
#define ETHOS_NC	6
#define ETHOS_ANY	7

/* Class guild used in the room 'G'  flags */
#define GUILD_WARRIOR		1
#define GUILD_THIEF		2
#define GUILD_CLERIC		3
#define GUILD_PALADIN		4
#define GUILD_ANTI_PALADIN	5
#define GUILD_RANGER		6
#define GUILD_DRUID		7
#define GUILD_CHANNELER		8
#define GUILD_ASSASSIN		9
#define GUILD_NECROMANCER	10
#define GUILD_ELEMENTALIST	11

#define CLASS_WARRIOR 0
#define CLASS_THIEF 1
#define CLASS_ZEALOT 2
#define CLASS_PALADIN 3
#define CLASS_ANTI_PALADIN 4
#define CLASS_RANGER 5
#define CLASS_DRUID 6
#define CLASS_CHANNELER 7
#define CLASS_ASSASSIN 8
#define CLASS_NECROMANCER 9
#define CLASS_ELEMENTALIST 10
#define CLASS_BARD 11
#define CLASS_HEALER 12

/* Specialization types */
#define SPEC_NONE	0
#define SPEC_BARBARIAN	1
#define SPEC_PALADIN_AVENGER	2
#define SPEC_PALADIN_HUNTER	3
#define SPEC_KNIGHT_DEATH	4
#define SPEC_KNIGHT_FAMINE	5
#define SPEC_KNIGHT_PESTILENCE	6
#define SPEC_NECRO_ANATOMIST	7
#define SPEC_NECRO_CORRUPTOR	8

/*
 * Equpiment wear locations.
 * Used in #RESETS.
 */
#define WEAR_NONE			-1
#define WEAR_LIGHT		      	0
#define WEAR_FINGER_L		      	1
#define WEAR_FINGER_R		      	2
#define WEAR_NECK_1		      	3
#define WEAR_NECK_2		      	4
#define WEAR_BODY		      	5
#define WEAR_HEAD		      	6
#define WEAR_LEGS		      	7
#define WEAR_FEET		      	8
#define WEAR_HANDS		      	9
#define WEAR_ARMS		     	10
#define WEAR_SHIELD		     	11
#define WEAR_ABOUT		     	12
#define WEAR_WAIST		     	13
#define WEAR_WRIST_L		     	14
#define WEAR_WRIST_R		     	15
#define WEAR_WIELD		     	16
#define WEAR_HOLD		     	17
#define WEAR_DUAL_WIELD		    	18
#define WEAR_BRAND                   	19
#define WEAR_STRAPPED			20
#define MAX_WEAR                     	21

/* Imperial */
#define	IMPERIAL_OFFENSE		1
#define	IMPERIAL_NONE			0
#define IMPERIAL_DEFENSE		-1

/* New combat schtuff */
#define BLOCK_YES 			1
#define BLOCK_NO 			0
#define NORM 				1
#define HIT_UNBLOCKABLE 		0
#define HIT_BLOCKABLE 			1
#define HIT_NOSPECIALS 			0
#define HIT_SPECIALS 			1
#define HIT_NOMULT 			1
#define HIT_NOADD 			0
#define PLURAL				1
#define SINGULAR			0
#define DAM_POSSESSIVE			1
#define DAM_NOT_POSSESSIVE		0

#define DARKSHOUT_BELAL			1
#define DARKSHOUT_ISHMAEL		2
#define DARKSHOUT_AGAMEMNON		3
#define DARKSHOUT_MEPHISTOPHELES	4
#define DARKSHOUT_BAALZEBUB		5
#define DARKSHOUT_NECRUVIAN		6
#define DARKSHOUT_THANATOS		7
#define DARKSHOUT_BAAL			8

/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (End of this section ... stop here)                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Conditions.
 */
#define COND_DRUNK		      0
#define COND_FULL		      1
#define COND_THIRST		      2
#define COND_HUNGER		      3
#define COND_STARVING		      4
#define COND_DEHYDRATED		      5

/*
 * Positions.
 */
#define POS_DEAD		      0
#define POS_MORTAL		      1
#define POS_INCAP		      2
#define POS_STUNNED		      3
#define POS_SLEEPING		      4
#define POS_RESTING		      5
#define POS_SITTING		      6
#define POS_FIGHTING		      7
#define POS_STANDING		      8

#define MIN_PK_XP		9999 /* min xp players need to start PKing */
#define MIN_LEVEL_TO_PK		10 /* Minimum level for players to pk */

#define PK_KILLS		0
#define PK_GOOD			1
#define PK_NEUTRAL       	2
#define PK_EVIL			3

#define PK_KILLED		0
#define MOB_KILLED		1



/*
 * ACT bits for players.
 */
#define PLR_IS_NPC		(A)		/* Don't EVER set.	*/
#define PLR_SKIP_AUTODEL	(B)
/* RT auto flags */
//#define (B)
#define PLR_AUTOASSIST		(C)
#define PLR_AUTOEXIT		(D)
#define PLR_AUTOLOOT		(E)
#define PLR_AUTOSAC             (F)
#define PLR_AUTOGOLD		(G)
#define PLR_AUTOSPLIT		(H)
#define PLR_COLOR               (I)
#define PLR_EVALUATION		(J)
#define PLR_BETRAYER		(K)

/* RT personal flags */
#define PLR_HEROIMM		(M)
#define PLR_HOLYLIGHT		(N)
#define PLR_EMPOWERED		(O)
#define PLR_CANLOOT		(P)
#define PLR_NOSUMMON		(Q)
#define PLR_NOFOLLOW		(R)
/* 2 bits reserved, S-T */

#define PLR_NO_TRANSFER		(T)

/* penalty flags */
#define PLR_PERMIT		(U)
#define PLR_MORON		(V)
#define PLR_LOG			(W)
#define PLR_DENY		(X)
#define PLR_FREEZE		(Y)
#define PLR_THIEF		(Z)
#define PLR_KILLER		(aa)
#define PLR_CRIMINAL            (bb)

/* RT comm flags -- may be used on both mobs and chars */
#define COMM_QUIET              (A)
#define COMM_DEAF            	(B)
#define COMM_NOWIZ              (C)
#define COMM_NOAUCTION          (D)
#define COMM_NOSOCKET		(E)
#define COMM_NOQUESTION         (F)
#define COMM_NOCABAL            (H)
#define COMM_NOQUOTE		(I)
#define COMM_SHOUTSOFF		(J)
#define COMM_ANSI               (K)
#define COMM_NOGLOBAL        	(bb)

/* display flags */
#define COMM_COMPACT		(L)
#define COMM_BRIEF		(M)
#define COMM_PROMPT		(N)
#define COMM_COMBINE		(O)
#define COMM_TELNET_GA		(P)
#define COMM_SHOW_AFFECTS	(Q)
#define COMM_IMMORTAL		(S)

/* penalties */

#define COMM_NOEMOTE		(T)
#define COMM_NOSHOUT		(U)
#define COMM_NOTELL		(V)
#define COMM_NOCHANNELS		(W)
#define COMM_BUILDER		(X)
#define COMM_SNOOP_PROOF	(Y)
#define COMM_AFK		(Z)
#define COMM_ALL_CABALS         (aa)
//#define (bb)
//#define (cc)

/* WIZnet flags */
#define WIZ_ON			(A)
#define WIZ_TICKS		(B)
#define WIZ_LOGINS		(C)
#define WIZ_SITES		(D)
#define WIZ_LINKS		(E)
#define WIZ_DEATHS		(F)
#define WIZ_RESETS		(G)
#define WIZ_MOBDEATHS		(H)
#define WIZ_FLAGS		(I)
#define WIZ_PENALTIES		(J)
#define WIZ_SACCING		(K)
#define WIZ_LEVELS		(L)
#define WIZ_SECURE		(M)
#define WIZ_SWITCHES		(N)
#define WIZ_SNOOPS		(O)
#define WIZ_RESTORE		(P)
#define WIZ_LOAD		(Q)
#define WIZ_NEWBIE		(R)
#define WIZ_PREFIX		(S)
#define WIZ_SPAM		(T)
#define WIZ_CABAL		(U)
#define WIZ_LOOTING		(V)
#define WIZ_DEBUG		(W)
#define WIZ_MYSQL		(X)
#define WIZ_PERCENT		(Y)


#define ARE_NORMAL		0
#define ARE_CABAL		1
#define ARE_QUEST		2
#define ARE_CITY		3
#define ARE_UNOPENED		4

/*
 * Prototype for a mob.
 * This is the in-memory version of #MOBILES.
 */
struct	mob_index_data
{
    MOB_INDEX_DATA *	next;
    SPEC_FUN *		spec_fun;
    SHOP_DATA *		pShop;
    MPROG_DATA *        mprogs;
    AREA_DATA *		area;
    int                 progtypes;
    long		vnum;
    sh_int		group;
    bool		new_format;
    sh_int		count;
    sh_int		killed;
    char *		player_name;
    char *		short_descr;
    char *		long_descr;
    char *		description;
    long		act;
    long		affected_by;
    sh_int		alignment;
    sh_int		level;
    sh_int		hitroll;
    sh_int		hit[3];
    sh_int		mana[3];
    sh_int		damage[3];
    sh_int		ac[4];
    sh_int 		dam_type;
    long		off_flags;
    long                extended_flags;
    long		imm_flags;
    long		res_flags;
    long		vuln_flags;
    long                mprog_flags;
    sh_int		dam_mod;
    sh_int		start_pos;
    sh_int		default_pos;
    sh_int		sex;
    sh_int		race;
    long		wealth;
    long		quest_credit_reward;
    sh_int		numAttacks;
    float		enhancedDamMod;
	int			regen_rate;
    long		form;
    long		parts;
    sh_int		size;
    char *		material;
    sh_int		cabal;
};

/*
 * Color codes
 */
#define ANSI_NORMAL			"\x01B[m"
#define ANSI_BOLD			"\x01B[1m"
#define ANSI_BLINK			"\x01B[5m"
#define ANSI_REVERSE			"\x01B[7m"

/* memory settings */
#define MEM_CUSTOMER	A
#define MEM_SELLER	B
#define MEM_HOSTILE	C
#define MEM_AFRAID	D

/* memory for mobs */
struct mem_data
{
    MEM_DATA 	*next;
    bool	valid;
    int		id;
    int 	reaction;
    time_t 	when;
};


/*
 * One character (PC or NPC).
 */
struct	char_data
{
    CHAR_DATA *		next;
    CHAR_DATA *         owner;
    CHAR_DATA *		next_in_room;
    CHAR_DATA *		master;
    CHAR_DATA *		leader;
    CHAR_DATA *		fighting;
    CHAR_DATA *		reply;
    CHAR_DATA *         mprog_target;
    CHAR_DATA *		last_fought;
    time_t		last_fight_time;
    char *		last_fight_name;
    ROOM_INDEX_DATA *   dream; /* For dream stuff */
    CHAR_DATA *		defending;
    CHAR_DATA *		hunting;
    ROOM_INDEX_DATA *   alarm;
    MEM_DATA *		memory;
    SPEC_FUN *		spec_fun;
    MOB_INDEX_DATA *	pIndexData;
    DESCRIPTOR_DATA *	desc;
    AFFECT_DATA *	affected;
    OBJ_DATA *		carrying;
    OBJ_DATA *		on;
    ROOM_INDEX_DATA *	in_room;
    ROOM_INDEX_DATA *	was_in_room;
    AREA_DATA *		zone;
    PC_DATA *		pcdata;
    GEN_DATA *		gen_data;
    bool		valid;
    char *		name;
    char *		original_name;
    char *		unholy_name;
    long		id;
    sh_int		version;
    char *		short_descr;
    char *		long_descr;
    char *		description;
    char *		prompt;
    char *		prefix;
    int			temple;
    sh_int		group;
    sh_int              cabal;
    sh_int		sex;
    sh_int		class;
    sh_int		race;
    sh_int		level;
    sh_int		trust;
    int			played;
    int			lines;  /* for the pager */
    time_t		logon;
    sh_int		timer;
    sh_int		wait;
    sh_int		daze;
    sh_int		noregen_dam;	/* For troll regen...Ceran */
    sh_int		drain_level;
    sh_int		hit;
    sh_int		max_hit;
    sh_int		mana;
    sh_int		max_mana;
    sh_int		move;
    sh_int		max_move;
    long		gold;
    sh_int		numAttacks;
    sh_int		base_numAttacks;
    float		enhancedDamMod;
    float		base_enhancedDamMod;
	sh_int			regen_rate;
	sh_int			base_regen;
    long		quest_credits;
    long		gold_bank;
    int			exp;
    int 		exp_total;
    long		act;
    long		comm;   /* RT added to pad the vector */
    long		wiznet; /* wiz stuff */
    long		imm_flags;
    long		res_flags;
    long		vuln_flags;
    sh_int		invis_level;
    sh_int		incog_level;
    long		affected_by;
    sh_int		position;
    sh_int		practice;
    sh_int		train;
    sh_int		carry_weight;
    sh_int		carry_number;
    sh_int		saving_throw;
    sh_int		alignment;
    sh_int		hitroll;
    sh_int		damroll;
    sh_int		armor[4];
    sh_int		wimpy;
    /* stats */
    sh_int		perm_stat[MAX_STATS];
    sh_int		mod_stat[MAX_STATS];
    /* parts stuff */
    long		form;
    long		parts;
    sh_int		size;
    char*		material;
    /* mobile stuff */
    long		off_flags;
    long                extended_flags;
    sh_int		damage[3];
    sh_int		dam_type;
    sh_int		start_pos;
    sh_int		default_pos;
    sh_int              mprog_delay;
    sh_int		pause;
    sh_int		ghost;
    int                 status;
    int                 progtypes;
    sh_int		dam_mod;
    sh_int		base_dam_mod;
    int			hometown;
    sh_int		eavesdrop;
    bool		extracted;
    sh_int		cloaked;
    NOTE_DATA *		pnote;
    int			setItemsWorn[MAX_SET_TYPES];
};



/*
 * Data which only PC's have.
 */
struct	pc_data
{
    PC_DATA *		next;
    BUFFER * 		buffer;
    bool		valid;
    bool                newbie;
    char *		pwd;
    char *		bamfin;
    char *		bamfout;
    char *		title;
    char * 	        extitle;
    char *		imm_death;
    char *		smite;
    sh_int		perm_hit;
    sh_int		perm_mana;
    sh_int		perm_move;
    sh_int		true_sex;
    sh_int		age_mod;
    sh_int		death_timer;
    sh_int		death_status;
    sh_int              ethos;
    sh_int              oalign;
    sh_int              oethos;
    sh_int		sp;
    sh_int		special;
    sh_int		quest;
    sh_int		death_count;
    sh_int              induct;
    sh_int              empire;
    sh_int              lich;
    sh_int		killed[2]; /* 0 = PK_KILLED, 1 = MOB_KILLED */
    sh_int              kills[4];  /* 0,1 = PKs_KILL, 2,3 = MOB_KILL */
    sh_int		track_dir[40000];
    sh_int		old_room;
    int			last_level;
    sh_int		condition	[6];
    sh_int		learned		[MAX_SKILL];
    bool		group_known	[MAX_GROUP];
    bool              	confirm_delete;
    char *		alias[MAX_ALIAS];
    char * 		alias_sub[MAX_ALIAS];
    sh_int              style;
    long                styles;
    char *              temp_history_buffer;
    char *              history_buffer;
    char *              logon_time;
    char *		sitetrack;
    bool                entering_text;
    char *              entered_text;
    DO_FUN *            end_fun;
    sh_int		warrior_specs[MAX_SPECS];
    sh_int              empire_pay;
    sh_int              empire_prev;
    int			security;
    sh_int		forms[MAX_FORM];
    long		shifted;
    OLD_CHAR *		old;
    long		roll_time;
    char *		host;
    sh_int		helpid;
    sh_int		save_timer;
    char *		zombie_name[MAX_ZOMBIE];
    sh_int		zombie_level[MAX_ZOMBIE];
    sh_int		zombie_health[MAX_ZOMBIE];
    long		bounty;
    sh_int		bounty_credits;
    sh_int		dedication;
    time_t              last_note;
    time_t              last_idea;
    time_t              last_penalty;
    time_t              last_news;
    time_t              last_changes;
    sh_int		wanteds;
};

/* Data for generating characters -- only used during generation */
struct gen_data
{
    GEN_DATA	*next;
    bool	valid;
    bool	skill_chosen[MAX_SKILL];
    bool	group_chosen[MAX_GROUP];
    int		points_chosen;
};


struct old_char
{
    char *	name;
    char *	short_descr;
    char *	long_descr;
    char *	description;
    sh_int	perm_stats[MAX_STATS];
    sh_int	armor[4];
    sh_int	dam_mod;
    sh_int	numAttacks;
    float	enhancedDamMod;
	sh_int	regen_rate;
    sh_int	carry_weight;
    sh_int	carry_number;
    sh_int	saving_throw;
    long	affected_by;
    long	res_flags;
    long	vuln_flags;
    long	imm_flags;
    OLD_CHAR	*next;
    OBJ_DATA	*carrying;
};

/*
 * Data structure for notes.
 */

#define NOTE_NOTE	0
#define NOTE_IDEA	1
#define NOTE_PENALTY	2
#define NOTE_NEWS	3
#define NOTE_CHANGES	4

struct	note_data
{
    NOTE_DATA *	next;
    bool 	valid;
    sh_int	type;
    char *	sender;
    char *	date;
    char *	to_list;
    char *	subject;
    char *	text;
    time_t      date_stamp;
};

/*
 * Liquids.
 */
#define LIQ_WATER        0

struct	liq_type
{
    char *	liq_name;
    char *	liq_color;
    sh_int	liq_affect[5];
};



/*
 * Extra description data for a room or object.
 */
struct	extra_descr_data
{
    EXTRA_DESCR_DATA *next;	/* Next in list                     */
    bool valid;
    char *keyword;              /* Keyword in look/examine          */
    char *description;          /* What to see                      */
};

//Description for a material (i.e. iron/steel) - Zornath
struct mat_data {
	MAT_DATA *	next;
	char *	name;
	int		relative_protection_pierce;
	int		relative_protection_bash;
	int		relative_protection_slash;
	int		relative_protection_magic;
	int		relative_weight;
	//some other values
};

/*
 * Prototype for an object.
 */
struct	obj_index_data
{
    OBJ_INDEX_DATA *	next;
    EXTRA_DESCR_DATA *	extra_descr;
    AFFECT_DATA *	affected;
    AREA_DATA *		area;
    bool		new_format;
    char *		name;
    char *		short_descr;
    char *		description;
    long		vnum;
    sh_int		reset_num;
    sh_int              cabal;
	MAT_DATA *	material;
    sh_int		item_type;
    BITMASK		extra_flags;
    long		wear_flags;
    long		restrict_flags;
    sh_int		level;
    sh_int 		condition;
    sh_int		limtotal;
    sh_int		limcount;
    sh_int		count;
    sh_int		weight;
    int			cost;
    long		value[5];
    IPROG_DATA	       *iprogs;
    int                 progtypes;
};



/*
 * One object.
 */
struct	obj_data
{
    OBJ_DATA *		next;
    OBJ_DATA *		next_content;
    OBJ_DATA *		contains;
    OBJ_DATA *		in_obj;
    OBJ_DATA *		on;
    CHAR_DATA *		carried_by;
    EXTRA_DESCR_DATA *	extra_descr;
    AFFECT_DATA *	affected;
    OBJ_INDEX_DATA *	pIndexData;
    ROOM_INDEX_DATA *	in_room;
    int			owner_class;
    bool		valid;
    bool		enchanted;
    bool		unholy;
    char *		talked;
    char *	        owner;
    char *		name;
    char *		short_descr;
	char *		corpse_of; //Short descr for corpses --Zornath
    char *		description;
    sh_int		item_type;
    BITMASK		extra_flags;
    long		wear_flags;
    sh_int		wear_loc;
    sh_int		weight;
    int			cost;
    sh_int		level;
    sh_int 		condition;
	MAT_DATA *	material;
    sh_int		timer;
    long		value[5];
    long		restrict_flags;
    int			progtypes;
    int			ohp;
    bool		noPhylactery;
};



/*
 * Exit data.
 */
struct	exit_data
{
    union
    {
	ROOM_INDEX_DATA *	to_room;
	long			vnum;
    } u1;
    long		exit_info;
    long		key;
    char *		keyword;
    char *		description;
    EXIT_DATA *		next;
    int			rs_flags;
    long		orig_door;
};



/*
 * Reset commands:
 *   '*': comment
 *   'M': read a mobile
 *   'O': read an object
 *   'P': put object in object
 *   'G': give object to mobile
 *   'E': equip object to mobile
 *   'D': set state of door
 *   'R': randomize room exits
 *   'S': stop (end of list)
 */

/*
 * Area-reset definition.
 */
struct	reset_data
{
    RESET_DATA *	next;
    char		command;
long arg1;
long arg2;
long arg3;
long arg4;
};



/*
 * Area definition.
 */
struct	area_data
{
    AREA_DATA *		next;
    HELP_AREA *		helps;
    char *		file_name;
    char *		name;
    char *		credits;
    sh_int		age;
    sh_int		nplayer;
    sh_int		low_range;
    sh_int		high_range;
    long		min_vnum;
    long		max_vnum;
    bool		empty;
    char *		builders;	/* OLC */ /* Listing of */
    long		vnum;
    int			area_flags;	/* OLC */
    int			security;	/* OLC */ /* Value 1-9  */
    bool		explore;
};



/*
 * Room type.
 */
struct	room_index_data
{
    ROOM_INDEX_DATA *	next;
    ROOM_INDEX_DATA *   aff_next;
    CHAR_DATA *		people;
    OBJ_DATA *		contents;
    EXTRA_DESCR_DATA *	extra_descr;
    AREA_DATA *		area;
    EXIT_DATA *		exit	[6];
    EXIT_DATA * 	old_exit[6];
    RESET_DATA *	reset_first;
    RESET_DATA *	reset_last;
    char *		name;
    char *		description;
    char *		owner;
    long		vnum;
    int			room_flags;
    int			extra_room_flags;
    sh_int		light;
    sh_int		sector_type;
    sh_int		heal_rate;
    sh_int 		mana_rate;
    sh_int              cabal;
    sh_int		guild;
    ROOM_AFFECT_DATA *  affected;
    long                affected_by;
    bool		move_progs;
    int 		distance_from_source;
    ROOM_INDEX_DATA  *	shortest_from_room;
    ROOM_INDEX_DATA  *	shortest_next_room;
    RPROG_DATA *	rprogs;
    int                 progtypes;
};


/*
 * MOBprog definitions
 */
#define TRIG_ACT      (A)
#define TRIG_BRIBE    (B)
#define TRIG_DEATH    (C)
#define TRIG_ENTRY    (D)
#define TRIG_FIGHT    (E)
#define TRIG_GIVE     (F)
#define TRIG_GREET    (G)
#define TRIG_GRALL    (H)
#define TRIG_KILL     (I)
#define TRIG_HPCNT    (J)
#define TRIG_RANDOM   (K)
#define TRIG_SPEECH   (L)
#define TRIG_EXIT     (M)
#define TRIG_EXALL    (N)
#define TRIG_DELAY    (O)
#define TRIG_SURR     (P)

struct mprog_code
{
    long            vnum;
    char *            code;
    MPROG_CODE *      next;
};

/*
 * Types of attacks.
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 */
#define TYPE_NOCOMBAT		     -2
#define TYPE_UNDEFINED               -1
#define TYPE_HIT                     1000
#define TYPE_TRUESTRIKE		     -3



/*
 *  Target types.
 */
#define TAR_IGNORE		    0
#define TAR_CHAR_OFFENSIVE	    1
#define TAR_CHAR_DEFENSIVE	    2
#define TAR_CHAR_SELF		    3
#define TAR_OBJ_INV		    4
#define TAR_OBJ_CHAR_DEF	    5
#define TAR_OBJ_CHAR_OFF	    6

#define TARGET_CHAR		    0
#define TARGET_OBJ		    1
#define TARGET_ROOM		    2
#define TARGET_NONE		    3



/*
 * Skills include spells as a particular case.
 */
struct	skill_type
{
    char *	name;			/* Name of skill		*/
    sh_int	skill_level[MAX_CLASS];	/* Level needed by class	*/
    sh_int	rating[MAX_CLASS];	/* How hard it is to learn	*/
    SPELL_FUN *	spell_fun;		/* Spell pointer (for spells)	*/
    sh_int	target;			/* Legal targets		*/
    sh_int	minimum_position;	/* Position for caster / user	*/
    sh_int *	pgsn;			/* Pointer to associated gsn	*/
    sh_int	slot;			/* Slot for #OBJECT loading	*/
    sh_int	min_mana;		/* Minimum mana used		*/
    sh_int	beats;			/* Waiting time after use	*/
    char *	noun_damage;		/* Damage message		*/
    char *	msg_off;		/* Wear off message		*/
    char *	msg_obj;		/* Wear off message for obects	*/
    sh_int	ctype;
};

struct  group_type
{
    char *	name;
    sh_int	rating[MAX_CLASS];
    char *	spells[MAX_IN_GROUP];
};


extern sh_int cabal_members[MAX_CABAL];
extern sh_int cabal_max[MAX_CABAL];


extern sh_int obsidian_prince_timer;

/*
 * These are skill_lookup return values for common skills and spells.
 */
extern	sh_int	gsn_backstab;
extern	sh_int	gsn_plant_explosive;
extern	sh_int	gsn_advanced_tripwire;
extern	sh_int	gsn_shadowstep;
extern	sh_int	gsn_veil_of_the_opaque;
extern	sh_int	gsn_disguise;
extern	sh_int	gsn_improved_blackjack;
extern 	sh_int	gsn_coalesce_resistance;
extern 	sh_int	gsn_bulwark_of_blades;
extern 	sh_int	gsn_divine_transferrance;
extern 	sh_int	gsn_statue_sacrifice;
extern	sh_int	gsn_greater_regeneration;
extern	sh_int	gsn_resurrect;
extern 	sh_int	gsn_blade_mastery;
extern  sh_int  gsn_detect_hidden;
extern	sh_int	gsn_dodge;
extern  sh_int  gsn_envenom;
extern  sh_int  gsn_forge;
extern	sh_int	gsn_hide;
extern	sh_int	gsn_peek;
extern	sh_int	gsn_pick_lock;
extern	sh_int	gsn_sneak;
extern	sh_int	gsn_steal;
extern  sh_int  gsn_silence;
extern	sh_int	gsn_disarm;
extern	sh_int	gsn_enhanced_damage;
extern	sh_int	gsn_kick;
extern	sh_int	gsn_crescent_kick;
extern	sh_int	gsn_owaza;
extern	sh_int	gsn_shuto_strike;
extern	sh_int	gsn_seiken_strike;
extern	sh_int	gsn_sukuinage;
extern	sh_int	gsn_axe_kick;
extern	sh_int	gsn_heightened_awareness;
extern	sh_int	gsn_backfist;
extern	sh_int	gsn_unarmed_defense;
extern	sh_int	gsn_unarmed_offense;
extern	sh_int	gsn_bindwounds;
extern	sh_int	gsn_tigerclaw;
extern	sh_int	gsn_scissor_kick;
extern	sh_int	gsn_parry;
extern	sh_int	gsn_rescue;
extern	sh_int	gsn_second_attack;
extern	sh_int	gsn_third_attack;
extern  sh_int  gsn_fourth_attack;
extern  sh_int  gsn_icy_shards;
extern	sh_int	gsn_blindness;
extern	sh_int	gsn_charm_person;
extern	sh_int	gsn_curse;
extern	sh_int	gsn_invis;
extern	sh_int	gsn_mass_invis;
extern  sh_int  gsn_plague;
extern	sh_int	gsn_poison;
extern	sh_int	gsn_sleep;
extern  sh_int  gsn_fly;
extern  sh_int  gsn_sanctuary;
extern  sh_int  gsn_righteousness;
extern  sh_int  gsn_axe;
extern  sh_int  gsn_dagger;
extern  sh_int  gsn_flail;
extern  sh_int  gsn_mace;
extern  sh_int  gsn_polearm;
extern  sh_int  gsn_shield_block;
extern  sh_int  gsn_spear;
extern  sh_int  gsn_sword;
extern  sh_int  gsn_whip;
extern  sh_int  gsn_bash;
extern  sh_int  gsn_bearcharge;
extern  sh_int  gsn_berserk;
extern  sh_int  gsn_dirt;
extern  sh_int  gsn_hand_to_hand;
extern  sh_int  gsn_trip;
extern  sh_int  gsn_fast_healing;
extern  sh_int  gsn_haggle;
extern  sh_int  gsn_lore;
extern  sh_int  gsn_meditation;
extern  sh_int  gsn_scrolls;
extern  sh_int  gsn_staves;
extern  sh_int  gsn_wands;
extern  sh_int  gsn_recall;
extern  sh_int 	gsn_mimic;
extern  sh_int 	gsn_liquify;
extern 	sh_int 	gsn_crush;
extern 	sh_int	gsn_ground_control;
extern sh_int gsn_absorb;
extern sh_int gsn_camouflage;
extern sh_int gsn_acute_vision;
extern sh_int gsn_ambush;
extern sh_int gsn_laying_hands;
extern sh_int gsn_circle;
extern sh_int gsn_empower;
extern sh_int gsn_dual_backstab;
extern sh_int gsn_power_word_stun;
extern sh_int gsn_psi_blast;
extern sh_int gsn_power_word_fear;
extern sh_int gsn_skin;
extern sh_int gsn_camp;
extern sh_int gsn_resistance;
extern sh_int gsn_animate_dead;
extern sh_int gsn_barkskin;
extern sh_int gsn_animal_call;
extern sh_int gsn_shroud;
extern sh_int gsn_dark_shroud;
extern sh_int gsn_shadowgate;
extern sh_int gsn_eye_of_the_predator;
extern sh_int gsn_blackjack;
extern sh_int gsn_lunge;
/* extern sh_int gsn_vigilance; */
extern sh_int gsn_dual_wield;
extern sh_int gsn_trance;
extern sh_int gsn_wanted;
extern sh_int gsn_cleave;
extern sh_int gsn_herb;
extern sh_int gsn_bandage;
extern sh_int gsn_deathstrike;
extern sh_int gsn_protective_shield;
extern sh_int gsn_consecrate;
extern sh_int gsn_timestop;
extern sh_int gsn_timestop_done;
extern sh_int gsn_true_sight;
extern sh_int gsn_butcher;
extern sh_int gsn_shadowstrike;
extern sh_int gsn_fireshield;
extern sh_int gsn_iceshield;
extern sh_int gsn_vanish;
extern sh_int gsn_riot;
extern sh_int gsn_embalm;
extern sh_int gsn_dark_dream;
extern sh_int gsn_blizzard;
extern sh_int gsn_icy_wind;
extern sh_int gsn_electric_storm;
extern sh_int gsn_revolt;
extern sh_int gsn_acid_spit;
extern sh_int gsn_bear_call;
extern sh_int gsn_trophy;
extern sh_int gsn_tail;
extern sh_int gsn_spellbane;
extern sh_int gsn_request;
extern sh_int gsn_endure;
extern sh_int gsn_palm;
extern sh_int gsn_throw;
extern sh_int gsn_legbreak;
extern sh_int gsn_armbreak;
extern sh_int gsn_nerve;
extern sh_int gsn_follow_through;
extern sh_int gsn_poison_dust;
extern sh_int gsn_blindness_dust;
extern sh_int gsn_cblow;
extern sh_int gsn_roll;
extern sh_int gsn_block;
extern sh_int gsn_garrotte;
extern sh_int gsn_warcry;
extern sh_int gsn_chimera_lion;
extern sh_int gsn_chimera_goat;
extern sh_int gsn_strange_form;
extern sh_int gsn_downstrike;
extern sh_int gsn_enlist;
extern sh_int gsn_shadowplane;
extern sh_int gsn_blackjack_timer;
extern sh_int gsn_garrotte_timer;
extern sh_int gsn_tame;
extern sh_int gsn_track;
extern sh_int gsn_find_water;
extern sh_int gsn_shield_cleave;
extern sh_int gsn_spellcraft;
extern sh_int gsn_demand;
extern sh_int gsn_tertiary_wield;
extern sh_int gsn_breath_fire;
extern sh_int gsn_awareness;
extern sh_int gsn_forest_blending;
extern sh_int gsn_rear_kick;
extern sh_int gsn_shapeshift;
extern sh_int gsn_regeneration;
extern sh_int gsn_parrot;
extern sh_int gsn_door_bash;
extern sh_int gsn_wolverine_brand;
extern sh_int gsn_flame_scorch;
extern sh_int gsn_mutilated_left_arm;
extern sh_int gsn_mutilated_left_hand;
extern sh_int gsn_mutilated_left_leg;
extern sh_int gsn_mutilated_right_arm;
extern sh_int gsn_mutilated_right_hand;
extern sh_int gsn_mutilated_right_leg;
extern sh_int gsn_shattered_bone;
extern sh_int gsn_blood_tide;
extern sh_int gsn_flail_arms;
extern sh_int gsn_banshee_call;
extern sh_int gsn_soulbind;
extern sh_int gsn_earthfade;
extern sh_int gsn_forget;
extern sh_int gsn_earthbind;
extern sh_int gsn_divine_touch;
extern sh_int gsn_grounding;
extern sh_int gsn_shock_sphere;
extern sh_int gsn_forage;
extern sh_int gsn_fwood;
extern sh_int gsn_carve;
extern sh_int gsn_assassinate;
extern sh_int gsn_defend;
extern sh_int gsn_intimidate;
extern sh_int gsn_escape;
extern sh_int gsn_moving_ambush;
extern sh_int gsn_pugil;
extern sh_int gsn_staff;
extern sh_int gsn_evaluation;
extern sh_int gsn_enhanced_damage_two;
extern sh_int gsn_protection_heat_cold;
extern sh_int gsn_tactics;
extern sh_int gsn_lash;
extern sh_int gsn_prevent_healing;
extern sh_int gsn_regeneration;
extern sh_int gsn_undead_drain;
extern sh_int gsn_quiet_movement;
extern sh_int gsn_iron_resolve;
extern sh_int gsn_atrophy;
extern sh_int gsn_stoneskin;
extern sh_int gsn_shield;
extern sh_int gsn_spike;
extern sh_int gsn_starvation;
extern sh_int gsn_black_death;
extern sh_int gsn_black_scythe;
extern sh_int gsn_spore;
extern sh_int gsn_barbarian_strike;
extern sh_int gsn_barbarian_bone_tear;
extern sh_int gsn_dehydrated;	/* For starve/thirst */
extern sh_int gsn_hold_person;  /* Hold person -Detlef */
extern sh_int gsn_bloodthirst;  /* Bloodthirst for Rager -Detlef */
extern sh_int gsn_flurry;  /* Quest flurry -Dioxide */
extern sh_int gsn_drum;  /* Quest drum -Dioxide */
extern sh_int gsn_siton;  /* Quest siton -Dioxide */
extern sh_int gsn_knife;  /* Thief knife -Dioxide */
extern sh_int gsn_body_of_steel; /*tat affect -Orduno*/
extern sh_int gsn_body_of_iron; /*tat affect -Orduno*/
extern sh_int gsn_body_of_wood; /*tat affect -Orduno*/
extern sh_int gsn_body_of_clay; /*tat affect -Orduno*/
extern sh_int gsn_body_of_stone; /*tat affect -Orduno*/
extern sh_int gsn_body_of_diamond; /*tat affect -Orduno*/
extern sh_int gsn_dark_blessing; // Ceials real tat :P
extern sh_int gsn_rage; /*dhaytons tat -Orduno*/
extern sh_int gsn_curse_of_the_dagger; /*dhaytons tat Orduno*/
extern sh_int gsn_disperse;  /* Disperse for Outlaw -Dioxide */
extern sh_int gsn_decapitate;  /* Quest decapitate -Dioxide */
extern sh_int gsn_lightwalk;  /* Quest lightwalk -Dioxide */
extern sh_int gsn_bobbit;  /* Quest Bobbit -Dioxide */
extern sh_int gsn_garble;  /* Garble for Outlaw :) -Detlef */
extern sh_int gsn_armbreak; /*fun skill Deomanix*/
extern sh_int gsn_wraithform;
extern sh_int gsn_impale;  /* Quest Impale -Dioxide */
extern sh_int gsn_unholy_bless;  /* AP unholy bless -Dioxide */
extern sh_int gsn_defiance;
extern sh_int gsn_haunting;
extern sh_int gsn_executioner;
extern sh_int gsn_bind;
extern sh_int gsn_confuse; /* Confuse of Outlaw */
extern sh_int gsn_cheap_shot;
extern sh_int gsn_deafen; /* Deafen for AP -Detlef */
extern sh_int gsn_blitz;  /* Blitz for Rager -Dioxide */
extern sh_int gsn_questdodge;  /* Quest dodge -Dioxide */
extern sh_int gsn_divine_intervention;  /* divine intervention -Dioxide */
extern sh_int gsn_ranger_staff; /* ranger staff skill -Dioxide */
extern sh_int gsn_creep; /* creep skill -Dioxide */
extern sh_int gsn_dragonsword;
extern sh_int gsn_dragonplate;
extern sh_int gsn_unholy_timer;
extern sh_int gsn_honorable_combat;
extern sh_int gsn_chaos_blade;
extern sh_int gsn_darkforge;
extern sh_int gsn_darkforge_sword;
extern sh_int gsn_darkforge_dagger;
extern sh_int gsn_darkforge_whip;
extern sh_int gsn_aristaeia;
extern sh_int gsn_rage_of_dioxide;
extern sh_int gsn_phat_blunt;
extern sh_int gsn_merth;
extern sh_int gsn_merth2;
extern sh_int gsn_spin;
extern sh_int gsn_enhanced_damage_three;
extern sh_int gsn_pummel;
extern sh_int gsn_stun;
extern sh_int gsn_gouge;
extern sh_int gsn_stab;
extern sh_int gsn_stalker;
extern sh_int gsn_zealotism;
extern sh_int gsn_parting_blow;
extern sh_int gsn_dream_blossom;
extern sh_int gsn_randomizer;
extern sh_int gsn_shieldbash;
extern sh_int gsn_hacksaw;
extern sh_int gsn_boneshatter;
extern sh_int gsn_pincer;
extern sh_int gsn_spiderhands;
extern sh_int gsn_chargeset;
extern sh_int gsn_shove;
extern sh_int gsn_devour;
extern sh_int gsn_strip;
extern sh_int gsn_headclap;
extern sh_int gsn_noheadclap; // Damper for headclap -- Ceial
extern sh_int gsn_choke;
extern sh_int gsn_rot;
extern sh_int gsn_rot_recovery;
extern sh_int gsn_eagle_eyes;
extern sh_int gsn_cutoff;
extern sh_int gsn_incinerate;
extern sh_int gsn_integrate;
extern sh_int gsn_chameleon;
extern sh_int gsn_insect_swarm;
extern sh_int gsn_change_sex;
extern sh_int gsn_forest_friend;
extern sh_int gsn_slaves;
extern sh_int  gsn_phase;
extern sh_int gsn_flourintine;
extern sh_int gsn_ironhands;
extern sh_int gsn_overhead;
extern sh_int gsn_archery;
/*block of new elementalist spells*/
extern sh_int gsn_self_immolation;
extern sh_int gsn_shield_of_bubbles;
extern sh_int gsn_shield_of_dust;
extern sh_int gsn_shield_of_flames;
extern sh_int gsn_shield_of_frost;
extern sh_int gsn_shield_of_lightning;
extern sh_int gsn_shield_of_wind;
extern sh_int gsn_flames;
extern sh_int gsn_static_charge;
extern sh_int gsn_blizzard;
/*illusionist type stuff*/
extern sh_int gsn_displacement;
extern sh_int gsn_nullify_sound;
extern sh_int gsn_shackles;
extern sh_int gsn_bloody_shackles;
extern sh_int gsn_aura;
extern sh_int gsn_divine_aura;
extern sh_int gsn_artic_fury;
extern sh_int gsn_snare;
extern sh_int gsn_stance;
extern sh_int gsn_transform;
extern sh_int gsn_group_gate;
extern sh_int gsn_group_recall;
extern sh_int gsn_group_sanctuary;
extern sh_int gsn_valiant_wrath;
extern sh_int gsn_weaponmeld;
extern sh_int gsn_mana_shield;
extern sh_int gsn_march;
extern sh_int gsn_critical_strike;
extern sh_int gsn_lich_elixir_affect;
extern sh_int gsn_age_death;
extern sh_int gsn_soul_channel;
extern sh_int gsn_darkheart;
extern sh_int gsn_anti_magic_shell;
extern sh_int gsn_dark_essence;
extern sh_int gsn_ritual_of_abominations;
extern sh_int gsn_snaretimer;
extern sh_int gsn_whirl;
extern sh_int gsn_nightfist;
extern sh_int gsn_imperial_training;
extern sh_int gsn_centurion_call;
extern sh_int gsn_incandescense;
extern sh_int gsn_scorch;
extern sh_int gsn_counter;
extern sh_int gsn_asscounter;
extern sh_int gsn_weaponbreaker;
extern sh_int gsn_barrier;
extern sh_int gsn_crushing_blow;
extern sh_int gsn_hex;
extern sh_int gsn_maehslin;
extern sh_int gsn_volley;
extern sh_int gsn_soften;
extern sh_int gsn_duo;
extern sh_int gsn_protection;

extern sh_int gsn_wire_delay;
extern sh_int gsn_trip_wire;
extern sh_int gsn_balance;
extern sh_int gsn_gag;
extern sh_int gsn_healing_sleep;
extern sh_int gsn_sunray;
extern sh_int gsn_armor_of_thorns;
extern sh_int gsn_cranial;
extern sh_int gsn_entwine;
extern sh_int gsn_legsweep;
extern sh_int gsn_rising_pheonix_kick;
extern sh_int gsn_double_spin_kick;
extern sh_int gsn_charge;
extern sh_int gsn_dash;
extern sh_int gsn_restrike;
extern sh_int gsn_cross_slice;
extern sh_int gsn_icelance;
extern sh_int gsn_bleed;
extern sh_int gsn_blade_barrier;
extern sh_int gsn_detect_illusion;
extern sh_int gsn_counterspell;
extern sh_int gsn_kotegaeshi;
extern sh_int gsn_spire_of_flames;
extern sh_int gsn_worldbind;
extern sh_int gsn_hellfire;
extern sh_int gsn_chop;
extern sh_int gsn_weapontrip;
extern sh_int gsn_dblow;
extern sh_int gsn_spell_evasion;
extern sh_int gsn_bloodlust;
extern sh_int gsn_ward_timer;
extern sh_int gsn_ormthrak;
extern sh_int gsn_zornath;
extern sh_int gsn_paladin_tech_counter_pc;
extern sh_int gsn_paladin_tech_counter_npc;
extern sh_int gsn_swordplay;
extern sh_int gsn_drowning;
extern sh_int gsn_water_breathing;
extern sh_int gsn_hobble;
extern sh_int gsn_ward_diminution;
extern sh_int gsn_intrigue;
extern sh_int gsn_tracerah;
extern sh_int gsn_cloak_form;
extern sh_int gsn_vampiric_touch;
extern sh_int gsn_unholy_affinity;
extern sh_int gsn_darkshout;
extern sh_int gsn_credit_timer;
extern sh_int gsn_beast_timer;
extern sh_int gsn_pathfinding;
extern sh_int gsn_enlarge;
extern sh_int gsn_reduce;
extern sh_int gsn_shield_of_purity;
extern sh_int gsn_shield_of_redemption;
extern sh_int gsn_shield_of_justice;
extern sh_int gsn_shield_of_truth;
extern sh_int gsn_dragsnare;
extern sh_int gsn_wilderness_fam;
extern sh_int gsn_charge_weapon;
extern sh_int gsn_dent;
extern sh_int gsn_feint;
extern sh_int gsn_sigil_pain;
extern sh_int gsn_crescent;
extern sh_int gsn_intercept;
extern sh_int gsn_maelstrom;
extern sh_int gsn_bastion_of_elements;
extern sh_int gsn_elemental_affinity;
extern sh_int gsn_shatter_armor;
extern sh_int gsn_hurricane;
extern sh_int gsn_vorpal_bolt;
extern sh_int gsn_exude_health;
extern sh_int gsn_vitality;
extern sh_int gsn_dragon_might;

/*
 * Utility macros.
 */

#define IS_VALID(data)		((data) != NULL && (data)->valid)
#define VALIDATE(data)		((data)->valid = TRUE)
#define INVALIDATE(data)	((data)->valid = FALSE)
#define UMIN(a, b)		((a) < (b) ? (a) : (b))
#define UMAX(a, b)		((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)		((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define LOWER(c)		((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)		((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))
#define IS_SET(flag, bit)	((flag) & (bit))
#define SET_BIT(var, bit)	((var) |= (bit))
#define REMOVE_BIT(var, bit)	((var) &= ~(bit))
#define PERCENT(cur, max)       (max==0?0:((cur)*100)/(max))

#define IS_NULLSTR(str)		((str) == NULL || (str)[0] == '\0')
#define ENTRE(min,num,max)	( ((min) < (num)) && ((num) < (max)) )
#define CHECK_POS(a, b, c)	{							\
					(a) = (b);					\
					if ( (a) < 0 )					\
					bug( "CHECK_POS : " c " == %d < 0", a );	\
				}
#define IS_EXPLORE(room)	((room)->area->explore == TRUE ? TRUE : FALSE)
/*
 * Character macros.
 */
#define IS_NPC(ch)						(IS_SET((ch)->act, ACT_IS_NPC))
#define IS_IMMORTAL(ch)				(get_trust(ch) >= LEVEL_IMMORTAL)
#define IS_HERO(ch)						(get_trust(ch) >= LEVEL_HERO)
#define IS_HEROIMM(ch)				(IS_SET((ch)->act,PLR_HEROIMM))
#define IS_TRUSTED(ch,level)	(get_trust((ch)) >= (level))
#define IS_AFFECTED(ch, sn)		(IS_SET((ch)->affected_by, (sn)))
#define IS_ANSI(ch)						(!IS_NPC( ch ) && IS_SET((ch)->comm, COMM_ANSI))

#define GET_AGE(ch)		((int) (17 + ((ch)->played + current_time - (ch)->logon )/72000))

#define IS_CABAL_OUTER_GUARD(ch) 	(IS_NPC((ch)) && IS_SET((ch)->act, ACT_OUTER_GUARDIAN))
#define IS_CABAL_GUARD(ch)				(IS_NPC((ch)) && IS_SET((ch)->act, ACT_INNER_GUARDIAN))
#define HAS_TRIGGER(ch,trig)    	(IS_SET((ch)->pIndexData->mprog_flags,(trig)))
#define IS_GOOD(ch)             	(ch->alignment >= 1)
#define IS_EVIL(ch)             	(ch->alignment <= -1)
#define IS_NEUTRAL(ch)          	(!IS_GOOD(ch) && !IS_EVIL(ch))
#define IS_LAWFUL(ch)             (ch->pcdata->ethos >= 1)
#define IS_CHAOTIC(ch)            (ch->pcdata->ethos <= -1)
#define IS_ENEUTRAL(ch)           (!IS_LAWFUL(ch) && !IS_CHAOTIC(ch))


#define IS_AWAKE(ch)			(ch->position > POS_SLEEPING)
#define GET_AC(ch,type)		((ch)->armor[type] + ( IS_AWAKE(ch)			    \
														? dex_app[get_curr_stat(ch,STAT_DEX)].defensive : 0 ))

//#define GET_HITROLL(ch)	((ch)->hitroll+str_app[get_curr_stat(ch,STAT_STR)].tohit)
//#define GET_DAMROLL(ch) ((ch)->damroll+str_app[get_curr_stat(ch,STAT_STR)].todam)

#define GET_HITROLL(ch)		(ch->hitroll)
#define GET_DAMROLL(ch)		(ch->damroll)

#define IS_SHIFTED(ch)		(!IS_NPC(ch) ? (ch)->pcdata->shifted>-1 ? TRUE : FALSE: FALSE)

#define IS_OUTSIDE(ch)		(!IS_SET(				    						  \
											    (ch)->in_room->room_flags,		    \
											    ROOM_INDOORS))

#define DAZE_STATE(ch, npulse)  ((ch)->daze = UMAX((ch)->daze, (npulse)))
#define get_carry_weight(ch)    ((ch)->carry_weight + (ch)->gold/GOLD_PER_POUND)
#define GetName(ch)							(IS_NPC(ch) ? (ch)->short_descr : (ch)->name)

/* Room Macros */

#define IS_ROOM_AFFECTED(room, sn)  (IS_SET((room)->affected_by, (sn)))
#define IS_RAFFECTED(room, sn)  (IS_SET((room)->affected_by, (sn)))

/* Object macros. */
#define CAN_WEAR(obj, part)	(IS_SET((obj)->wear_flags,  (part)))
#define IS_OBJ_STAT(obj, stat)	(is_set(&(obj)->extra_flags, (stat)))
#define IS_WEAPON_STAT(obj,stat)(IS_SET((obj)->value[4],(stat)))
#define WEIGHT_MULT(obj)	((obj)->item_type == ITEM_CONTAINER ? (obj)->value[4] : 100)

#define IS_METAL(obj)      ((!str_cmp(obj->material->name,"iron"))      ||      \
                            (!str_cmp(obj->material->name,"metal"))     ||      \
                            (!str_cmp(obj->material->name,"tin"))       ||      \
                            (!str_cmp(obj->material->name,"titanium"))  ||      \
                            (!str_cmp(obj->material->name,"mithril"))   ||      \
                            (!str_cmp(obj->material->name,"copper"))    ||      \
                            (!str_cmp(obj->material->name,"silver"))    ||      \
                            (!str_cmp(obj->material->name,"steel"))     ||      \
                            (!str_cmp(obj->material->name,"bronze")))

/* Description macros. */
#define PERS(ch, looker)			( pers(ch, looker) )

/* Structure for a social in the socials table. */
struct	social_type {
    char      name[20];
    char *    char_no_arg;
    char *    others_no_arg;
    char *    char_found;
    char *    others_found;
    char *    vict_found;
    char *    char_not_found;
    char *    char_auto;
    char *    others_auto;
};

/* Global constants. */
extern	const	struct	str_app_type	str_app		[26];
extern	const	struct	int_app_type	int_app		[26];
extern	const	struct	wis_app_type	wis_app		[26];
extern	const	struct	dex_app_type	dex_app		[26];
extern	const	struct	con_app_type	con_app		[26];

extern	const			struct	class_type	 class_table	[MAX_CLASS];
extern	const			struct	weapon_type	 weapon_table	[];
extern  const   	struct  item_type		 item_table	[];
extern	const			struct	wiznet_type	 wiznet_table	[];
extern	const			struct	attack_type	 attack_table	[];
extern  const			struct  race_type		 race_table	[];
extern	const			struct	pc_race_type pc_race_table	[];
extern  const			struct	spec_type		 spec_table	[];
extern	const			struct	liq_type		 liq_table	[];
extern	const	  	struct	skill_type	 skill_table	[MAX_SKILL];
extern  const   	struct  group_type   group_table	[MAX_GROUP];
extern          	struct  social_type  social_table	[MAX_SOCIALS];
extern	char   *	const								 title_table	[MAX_CLASS] [MAX_LEVEL+1] [2];
extern  const   	struct  improg_type  iprog_table	[];
extern  const   	struct  improg_type	 mprog_table	[];
extern  const			struct	improg_type	 rprog_table	[];
/*
 * Global variables.
 */
extern		HELP_DATA	  		*	help_first;
extern		SHOP_DATA	  		*	shop_first;

extern		CHAR_DATA	  		*	char_list;
extern		DESCRIPTOR_DATA *	descriptor_list;
extern		OBJ_DATA	  		*	object_list;
extern		MAT_DATA	  		* mat_list;
extern		long						  gold_constant;

extern		char							bug_buf		[];
extern		time_t						current_time;
extern		bool							fLogAll;
extern    bool              MOBtrigger;
extern		FILE *						fpReserve;
extern		KILL_DATA					kill_table	[];
extern		char							log_buf		[];
extern		TIME_INFO_DATA		time_info;
extern		WEATHER_DATA			weather_info;
extern    ROOM_INDEX_DATA * top_affected_room;
extern 		sh_int						count_data	[40000];
extern    int               top_mprog_index;
extern		char							last_command[MSL];
extern		time_t						boot_time;

/*
 * OS-dependent declarations.
 * These are all very standard library functions,
 *   but some systems have incomplete or non-ansi header files.
 */

#if	defined(macintosh)
		#define NOCRYPT
		#if	defined(unix)
				void *  calloc          args( ( unsigned nelem, size_t size ) );
					#undef	unix
		#endif
#endif

#if	defined(MIPS_OS)
		char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(MSDOS)
		#define NOCRYPT
		#if	defined(unix)
				#undef	unix
		#endif
#endif

#if	defined(NeXT)
		char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(sequent)
		char *	crypt		args( ( const char *key, const char *salt ) );
		int			fclose	args( ( FILE *stream ) );
		int			fprintf	args( ( FILE *stream, const char *format, ... ) );
		int			fread		args( ( void *ptr, int size, int n, FILE *stream ) );
		int			fseek		args( ( FILE *stream, long offset, int ptrname ) );
		void		perror	args( ( const char *s ) );
		int			ungetc	args( ( int c, FILE *stream ) );
#endif

#if	defined(sun)
		char *	crypt		args( ( const char *key, const char *salt ) );
		int			fclose	args( ( FILE *stream ) );
		int			fprintf	args( ( FILE *stream, const char *format, ... ) );
		#if	defined(SYSV)
				siz_t	fread		args( ( void *ptr, size_t size, size_t n,FILE *stream) );
		#else
				int		fread		args( ( void *ptr, int size, int n, FILE *stream ) );
		#endif
		int		fseek		args( ( FILE *stream, long offset, int ptrname ) );
		void	perror	args( ( const char *s ) );
		int		ungetc	args( ( int c, FILE *stream ) );
#endif

#if	defined(ultrix)
		char *	crypt		args( ( const char *key, const char *salt ) );
#endif



/*
 * The crypt(3) function is not available on some operating systems.
 * In particular, the U.S. Government prohibits its export from the
 *   United States to foreign countries.
 * Turn on NOCRYPT to keep passwords in plain text.
 */
#if	defined(NOCRYPT)
		#define crypt(s1, s2)	(s1)
#endif

#define GOD_DIR         	"../gods/"
#define PLAYER_DIR		"../player/"
#define SYS_DIR			"../sys/"

#define AREA_LIST               "area.lst"
#define MATERIAL_LIST		"materials.lst"
#define NULL_FILE       	"/dev/null"

#define BUG_FILE                "../sys/bugs.txt"
#define TYPO_FILE               "../sys/typos.txt"
#define SHUTDOWN_FILE           "shutdown.txt"
#define BAN_FILE                "../sys/ban.txt"
#define DEBUG_LOG_FILE          "../sys/debug.txt"
#define BOUNTY_LOG_FILE         "../sys/bounties.txt"
#define TEMP_GREP_RESULTS 	"../sys/tempgrepresults.tmp"
#define LAST_COMMAND_FILE       "../sys/last_command.txt"
#define CRASH_FILE              "../sys/crash.txt"
#define CABAL_ITEMS_FILE	"../sys/citems.txt"
#define GOD_LOG_FILE    	"../sys/glog.txt"
#define LOGIN_SCREEN_FILE	"../sys/login.txt"
#define ITEM_FIX_FILE		"../sys/itemfix.txt"

#define PLAYER_LIST     	PLAYER_DIR "Player.lst"
#define TEMP_FILE       	PLAYER_DIR "romtmp"

#define PDEATH_LOG_FILE         "../players/dead_char/permdead.txt"


/*
 * Our function prototypes.
 * One big lump ... this is every function in Merc.
 */
#define CD	CHAR_DATA
#define MID	MOB_INDEX_DATA
#define OD	OBJ_DATA
#define OID	OBJ_INDEX_DATA
#define RID	ROOM_INDEX_DATA
#define SF	SPEC_FUN
#define AD	AFFECT_DATA
#define MPC     MPROG_CODE
#define RAD     ROOM_AFFECT_DATA

/* act_comm.c */
void do_quit_new args(( CHAR_DATA *ch, char *argument, bool autoq));
void report_cabal_items         (CHAR_DATA *ch, char *argument);
void 	perm_death_log( CHAR_DATA *ch, int deltype );
void  	check_sex	args( ( CHAR_DATA *ch) );
void	add_follower	args( ( CHAR_DATA *ch, CHAR_DATA *master ) );
void	stop_follower	args( ( CHAR_DATA *ch ) );
void 	nuke_pets	args( ( CHAR_DATA *ch ) );
void	die_follower	args( ( CHAR_DATA *ch ) );
bool	is_same_group	args( ( CHAR_DATA *ach, CHAR_DATA *bch ) );
void	announce_logout args((CHAR_DATA *ch));
void	get_age_mod	args((CHAR_DATA *ch));	/* meant to be comm.c but accident */
const char *lowstring(const char *i);
void	new_yell	args( ( CHAR_DATA *ch, char *argument) );
void temp_death_log( CHAR_DATA *killer, CHAR_DATA *dead);
bool can_join_cabal(CHAR_DATA *ch, int cabal);
char *get_ethos_restr_name(int iEthos);
char *get_align_restr_name(int iAlign);

/* act_enter.c */
RID  *get_random_room   args ( (CHAR_DATA *ch) );
AREA_DATA * area_lookup args((const char *));
ROOM_INDEX_DATA * area_begin args((AREA_DATA *));

/* act_info.c */
char	* pers				args( (CHAR_DATA *ch, CHAR_DATA *looker) );
char *  get_descr_form	args( (CHAR_DATA *ch,CHAR_DATA *looker, bool get_long) );
CHAR_DATA *get_cabal_guardian     args((int cabal));
const char *upstring(const char *i);
void	set_title	args( ( CHAR_DATA *ch, char *title ) );
void	debug_string	args( ( const char *str ) );

/* act_move.c */
bool isInWilderness args((CHAR_DATA *ch));
void	move_char	args( ( CHAR_DATA *ch, int door, bool follow ) );
void    un_hide         args( ( CHAR_DATA *ch, char *argument ) );
void    un_invis        args( ( CHAR_DATA *ch, char *argument ) );
void    un_earthfade       args( ( CHAR_DATA *ch, char *argument ) );
void    un_sneak        args( ( CHAR_DATA *ch, char *argument ) );
void 	un_camouflage	args( ( CHAR_DATA *ch, char *argument) );
void	un_blackjack	args( ( CHAR_DATA *ch, char *argument) );
void	un_garrotte	args( ( CHAR_DATA *ch, char *argument) );
void	un_shroud	args( (CHAR_DATA *ch, char *argument) );
void	un_forest_blend args( ( CHAR_DATA *ch));
void	un_duo		args( ( CHAR_DATA *ch, char *argument) );
void 	track_char	args( ( CHAR_DATA *ch, CHAR_DATA *tracking, int in_room) );
int	room_centurions(CHAR_DATA *ch);
bool	is_centurion(CHAR_DATA *ch);
bool 	isNewbie( CHAR_DATA *ch );
int reverse_d(int dir);

/* act_obj.c */
bool    is_owner                        args((CHAR_DATA *ch,OBJ_DATA *corpse));
bool  cabal_down_new args( (CHAR_DATA *ch,int cabal, bool show));
bool cant_carry		args( (CHAR_DATA *ch, OBJ_DATA *obj) );
bool is_restricted	args( (CHAR_DATA *ch, OBJ_DATA *obj) );
bool can_loot		args( (CHAR_DATA *ch, OBJ_DATA *obj) );
void    get_obj         args( ( CHAR_DATA *ch, OBJ_DATA *obj,
                            OBJ_DATA *container ) );
void  wear_obj          args( (CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace) );
void  cabal_shudder      args( (int cabal, bool itemloss) );
bool  cabal_down args( (CHAR_DATA *ch,int cabal) );
bool  hands_full	args( (CHAR_DATA *ch ) );
void wear_obj_girdle	args( (CHAR_DATA *ch, OBJ_DATA *obj) );
void wear_obj_bracers	args( (CHAR_DATA *ch, OBJ_DATA *obj) );
void remove_obj_girdle	args( (CHAR_DATA *ch, OBJ_DATA *obj) );
void remove_obj_bracers	args( (CHAR_DATA *ch, OBJ_DATA *obj) );
void reslot_weapon	args( (CHAR_DATA *ch) );
void report_weapon_skill	args( (CHAR_DATA *ch,OBJ_DATA *obj) );
bool can_wield_tertiary	args((CHAR_DATA *ch,OBJ_DATA *obj, bool fReplace));
void    affect_modify   args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd) );

/* act_wiz.c */
void	room_echo	args(( ROOM_INDEX_DATA *room, char *argument ));
void n_logf args((char * fmt, ...));
void add_history(CHAR_DATA *ch, CHAR_DATA *victim, char *string);
void show_history(CHAR_DATA *ch, CHAR_DATA *victim);
void show_temp_history(CHAR_DATA *ch, CHAR_DATA *victim);
void wiznet		args( (char *string, CHAR_DATA *ch, OBJ_DATA *obj,
			       long flag, long flag_skip, int min_level ) );
char *timestamp		args((void));
void copyover_recover args((void));
void    show_alist_free         args( (CHAR_DATA *ch, char *argument ) );

/* alias.c */
void 	substitute_alias args( (DESCRIPTOR_DATA *d, char *input) );

/* assassin.c */
void 	check_follow_through   	args( ( CHAR_DATA *ch, CHAR_DATA *victim,int dam) );
/* ban.c */
bool	check_ban	args( ( char *site, int type) );
int     unlink          (const char *pathname);

/* comm.c */
bool auto_check_multi(DESCRIPTOR_DATA *d_check, char *host);
void	show_string	args( ( struct descriptor_data *d, char *input) );
void	close_socket	args( ( DESCRIPTOR_DATA *dclose ) );
void	write_to_buffer	args( ( DESCRIPTOR_DATA *d, const char *txt,
			    int length ) );
void	send_to_char	args( ( const char *txt, CHAR_DATA *ch ) );
void    send_to_chars   args( ( const char *txt, CHAR_DATA *ch , int min , ... ) );
void page_to_char args( ( const char *txt, CHAR_DATA *ch ) );
void	act		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type ) );
void	act_new		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type,
			    int min_pos) );
void	printf_to_char	args( ( CHAR_DATA *, char *, ... ) );
void	bugf		args( ( char *, ... ) );

int     color           args( ( char type, CHAR_DATA *ch, char *string ) );
void    colorconv       args( ( char *buffer, const char *txt, CHAR_DATA *ch ) );
void    send_to_char_bw args( ( const char *txt, CHAR_DATA *ch ) );
void    page_to_char_bw args( ( const char *txt, CHAR_DATA *ch ) );
void    reverse_txt     args( (  char *txt, int length ) );
void    do_lockstat     args( ( CHAR_DATA *ch, char *argument ) );
void    sprintf2 args( ( CHAR_DATA *ch, char *fmt, ...) ) __attribute__ ((format(printf, 2,3)));
void    sendf args( ( CHAR_DATA *ch, char *fmt, ...) ) __attribute__ ((format(printf, 2,3)));


/* commune.c */
bool check_volley args((CHAR_DATA *ch, CHAR_DATA *victim));

void	reset_area	args( ( AREA_DATA * pArea ) );
void	reset_room	args( ( ROOM_INDEX_DATA *pRoom ) );
char *	print_flags	args( ( int flag ));
void	boot_db		args( ( void ) );
void	area_update	args( ( void ) );
MPC *   get_mprog_index args( ( long vnum ) );
CD *	create_mobile	args( ( MOB_INDEX_DATA *pMobIndex ) );
void	clone_mobile	args( ( CHAR_DATA *parent, CHAR_DATA *clone) );
OD *	create_object	args( ( OBJ_INDEX_DATA *pObjIndex, int level ) );
void	clone_object	args( ( OBJ_DATA *parent, OBJ_DATA *clone ) );
void	clear_char	args( ( CHAR_DATA *ch ) );
char *	get_extra_descr	args( ( const char *name, EXTRA_DESCR_DATA *ed ) );
MID *	get_mob_index	args( ( long vnum ) );
OID *	get_obj_index	args( ( long vnum ) );
RID *	get_room_index	args( ( long vnum ) );
char	fread_letter	args( ( FILE *fp ) );
int	fread_number	args( ( FILE *fp ) );
long 	fread_flag	args( ( FILE *fp ) );
char *	fread_string	args( ( FILE *fp ) );
char *  fread_string_eol args(( FILE *fp ) );
void	fread_to_eol	args( ( FILE *fp ) );
char *	fread_word	args( ( FILE *fp ) );
long	flag_convert	args( ( char letter) );
void *	alloc_mem	args( ( int sMem ) );
void *	alloc_perm	args( ( int sMem ) );
void	free_mem	args( ( void *pMem, int sMem ) );
char *	str_dup		args( ( const char *str ) );
void	free_string	args( ( char *pstr ) );
int	number_fuzzy	args( ( int number ) );
int	number_range	args( ( int from, int to ) );
int	number_percent	args( ( void ) );
int	number_door	args( ( void ) );
int	number_bits	args( ( int width ) );
long     number_mm       args( ( void ) );
int	dice		args( ( int number, int size ) );
int	interpolate	args( ( int level, int value_00, int value_32 ) );
void	smash_tilde	args( ( char *str ) );
bool	str_cmp		args( ( const char *astr, const char *bstr ) );
bool	str_prefix	args( ( const char *astr, const char *bstr ) );
bool	str_infix	args( ( const char *astr, const char *bstr ) );
bool	str_suffix	args( ( const char *astr, const char *bstr ) );
char *	capitalize	args( ( const char *str ) );
void	append_file	args( ( CHAR_DATA *ch, char *file, char *str ) );
void	bug		args( ( const char *str, int param ) );
void	log_string	args( ( const char *str ) );
void	tail_chain	args( ( void ) );

/* detextra.c */
int get_event_number args((char *name));
char * color_name_to_ascii args((char *name));
char * get_char_color args((CHAR_DATA *ch, char *event));
char * END_COLOR args((CHAR_DATA *ch));
char * get_color_name args((char *name));
bool is_wielded(CHAR_DATA *ch,int weapon,int type);
void chop(char *str);
void chomp(char *str);
void update_sitetrack(CHAR_DATA *ch, char *newsite);
char * get_where_name(int iWear);
int	count_carried(CHAR_DATA *ch, bool limited);
char * log_time args((void));
void delete_char args((char *name, bool save_pfile));

/* effect.c */
void	acid_effect	args( (void *vo, int level, int dam, int target) );
void	cold_effect	args( (void *vo, int level, int dam, int target) );
void	fire_effect	args( (void *vo, int level, int dam, int target) );
void	poison_effect	args( (void *vo, int level, int dam, int target) );
void	shock_effect	args( (void *vo, int level, int dam, int target) );


/* fight.c */
float calculate_ac_redux	args( (int ac) );
bool	spike_toed_boots	args( ( CHAR_DATA *ch ) );
void    check_downstrike        args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
void    check_cheap_shot        args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
void    one_hit                 args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ));
void    check_cheap_shot        args((CHAR_DATA *ch, CHAR_DATA *victim));
bool    check_chargeset args( ( CHAR_DATA *ch, CHAR_DATA *victim ));
void raw_kill	args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
void raw_kill_new   args( ( CHAR_DATA *ch, CHAR_DATA *victim, bool deathCry, bool fLook ) );
bool spellbaned 	args( (CHAR_DATA *chaster, CHAR_DATA *victim, int sn));
bool 	is_safe		args( (CHAR_DATA *ch, CHAR_DATA *victim ) );
bool 	is_safe_spell	args( (CHAR_DATA *ch, CHAR_DATA *victim, bool area ) );
void	violence_update	args( ( void ) );
void	multi_hit	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
bool	damage		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,
			        int dt, int class, bool show ) );
bool    damage_old      args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam,
                                int dt, int class, bool show ) );
bool check_evaluate args((CHAR_DATA *ch,CHAR_DATA *victim));
void	update_pos	args( ( CHAR_DATA *victim ) );
void	stop_fighting	args( ( CHAR_DATA *ch, bool fBoth ) );
void	check_killer	args( ( CHAR_DATA *ch, CHAR_DATA *victim) );
void	age_death	args((CHAR_DATA *ch));
char * check_evaluation_fight args((CHAR_DATA *ch,CHAR_DATA *victim));
bool is_wielded(CHAR_DATA *ch,int weapon,int type);
void update_pc_last_fight(CHAR_DATA *ch, CHAR_DATA *ch2);
bool    is_area_safe    args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void one_hit_new( CHAR_DATA *ch, CHAR_DATA *victim, int dt, bool specials, bool blockable, int addition, int multiplier, char *dnoun);
bool damage_new(CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show, bool blockable, int addition, int multiplier, char *dnoun);
bool damage_newer (CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type, bool show, bool blockable, int addition, int multiplier, char *dnoun, bool isPlural, bool showCharName, AFFECT_DATA *dealtByAffect, bool keep_invisible);
int damage_calculate(CHAR_DATA *ch, CHAR_DATA *victim, int dt, int dam_type, bool show, bool blockable, int addition, int multiplier);
float hitroll_multiplier(CHAR_DATA *ch, CHAR_DATA *victim, float max_multiplier);

/* handler.c */
bool    is_affected_obj args( ( OBJ_DATA *obj, int sn ) );
AFFECT_DATA *affect_find_obj    args( (OBJ_DATA *obj, int sn) );
bool is_prefix_name args((char *str, char *namelist));
ROOM_AFFECT_DATA *room_affect_find args ( ( ROOM_AFFECT_DATA *paf, int sn ));
AD  	*affect_find args( (AFFECT_DATA *paf, int sn));
void	affect_check	args( (CHAR_DATA *ch, int where, int vector) );
int	count_users	args( (OBJ_DATA *obj) );
void 	deduct_cost	args( (CHAR_DATA *ch, int cost) );
void	affect_enchant	args( (OBJ_DATA *obj) );
void  affect_unholy	args( (OBJ_DATA *obj) );
char *	color_value_string args( ( int color, bool bold, bool flash ) );
int 	strlen_color	args( ( char *argument ) );
int 	check_immune	args( (CHAR_DATA *ch, int dam_type) );
int 	material_lookup args( ( const char *name) );
int	weapon_lookup	args( ( const char *name) );
int	weapon_type	args( ( const char *name) );
char 	*weapon_name	args( ( int weapon_Type) );
char	*item_name	args( ( int item_type) );
int	attack_lookup	args( ( const char *name) );
long	wiznet_lookup	args( ( const char *name) );
int	class_lookup	args( ( const char *name) );
bool    is_cabal        args( (CHAR_DATA *ch) );
bool    is_same_cabal   args( (CHAR_DATA *ch, CHAR_DATA *victim));
bool	is_old_mob	args ( (CHAR_DATA *ch) );
int	get_skill	args( ( CHAR_DATA *ch, int sn ) );
int	get_weapon_sn	args( ( CHAR_DATA *ch ) );
int	get_weapon_skill args(( CHAR_DATA *ch, int sn ) );
int     get_age         args( ( CHAR_DATA *ch ) );
void	reset_char	args( ( CHAR_DATA *ch )  );
bool    can_pk        args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int	get_trust	args( ( CHAR_DATA *ch ) );
int	get_curr_stat	args( ( CHAR_DATA *ch, int stat ) );
int 	get_max_train	args( ( CHAR_DATA *ch, int stat ) );
int	can_carry_n	args( ( CHAR_DATA *ch ) );
int	can_carry_w	args( ( CHAR_DATA *ch ) );
bool	is_name		args( ( char *str, char *namelist ) );
bool	is_exact_name	args( ( char *str, char *namelist ) );
void	affect_to_char	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect_to_obj	args( ( OBJ_DATA *obj, AFFECT_DATA *paf ) );
void	affect_remove	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	affect_remove_obj args( (OBJ_DATA *obj, AFFECT_DATA *paf ) );
void	affect_strip	args( ( CHAR_DATA *ch, int sn ) );
bool	is_affected	args( ( CHAR_DATA *ch, int sn ) );
void    affect_to_room  args( ( ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf ) );
void    affect_remove_room  args( ( ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf ) );
void    affect_strip_room  args( ( ROOM_INDEX_DATA *ch, int sn ) );
bool    is_affected_room args( ( ROOM_INDEX_DATA *ch, int sn ) );
void    affect_join_room args( (ROOM_INDEX_DATA *ch, ROOM_AFFECT_DATA *paf )  );
void    new_affect_to_room( ROOM_INDEX_DATA *room, ROOM_AFFECT_DATA *paf );
void	affect_join	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void	char_from_room	args( ( CHAR_DATA *ch ) );
void	char_to_room	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
void	obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch ) );
void	obj_from_char	args( ( OBJ_DATA *obj ) );
int	apply_ac	args( ( OBJ_DATA *obj, int iWear, int type ) );
OD *	get_eq_char	args( ( CHAR_DATA *ch, int iWear ) );
void	equip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj, int iWear ) );
void	unequip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
int	count_obj_list	args( ( OBJ_INDEX_DATA *obj, OBJ_DATA *list ) );
void	obj_from_room	args( ( OBJ_DATA *obj ) );
void	obj_to_room	args( ( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex ) );
void	obj_to_obj	args( ( OBJ_DATA *obj, OBJ_DATA *obj_to ) );
void	obj_from_obj	args( ( OBJ_DATA *obj ) );
void	extract_obj	args( ( OBJ_DATA *obj ) );
void	extract_char	args( ( CHAR_DATA *ch, bool fPull ) );
CD *	get_char_room	args( ( CHAR_DATA *ch, char *argument ) );
CD *	get_char_world	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_type	args( ( OBJ_INDEX_DATA *pObjIndexData ) );
OD *	get_obj_list	args( ( CHAR_DATA *ch, char *argument,
			    OBJ_DATA *list ) );
OD *	get_obj_carry	args( ( CHAR_DATA *ch, char *argument,
			    CHAR_DATA *viewer ) );
OD *	get_obj_wear	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_here	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_world	args( ( CHAR_DATA *ch, char *argument ) );
OD *    create_money    args( ( int gold ) );
int	get_obj_number	args( ( OBJ_DATA *obj ) );
int	get_obj_weight	args( ( OBJ_DATA *obj ) );
int	get_true_weight	args( ( OBJ_DATA *obj ) );
bool	room_is_dark	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool	is_room_owner	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *room) );
bool	room_is_private	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool	can_see		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool	can_see_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool	can_see_room	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex) );
bool	can_drop_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
char *	affect_loc_name	args( ( int location ) );
char *  raffect_loc_name args( ( int location ) );
char *	affect_bit_name	args( ( int vector ) );
char *  raffect_bit_name args( ( int vector ) );
char *  restrict_bit_name args( (int restrict_flags) );
char *	extra_bit_name	args( ( int extra_flags ) );
char * 	wear_bit_name	args( ( int wear_flags ) );
char *	act_bit_name	args( ( int act_flags ) );
char *	off_bit_name	args( ( int off_flags ) );
char *  imm_bit_name	args( ( int imm_flags ) );
char * 	form_bit_name	args( ( int form_flags ) );
char *	part_bit_name	args( ( int part_flags ) );
char *	weapon_bit_name	args( ( int weapon_flags ) );
char *  comm_bit_name	args( ( int comm_flags ) );
char *	cont_bit_name	args( ( int cont_flags) );
char *  mprog_type_name args( ( int mprog_flags) );
bool	isCabalItem	args( ( OBJ_DATA *obj ) );
bool    is_safe_rspell  args( ( int level, CHAR_DATA *victim) );
void	init_affect	args( ( AFFECT_DATA *paf ) );
void	init_affect_room	args( ( ROOM_AFFECT_DATA *paf ) );
ROOM_AFFECT_DATA *new_affect_room(void);
void free_affect_room(ROOM_AFFECT_DATA *af);
bool    is_worn		args( ( OBJ_DATA *obj ) );
char *	sector_bit_name	args( ( int room_flags ) );


/* help.c */
void enter_text	args((CHAR_DATA *ch, DO_FUN *end_fun));
char * escape_string args((char *string));

/* interp.c */
void	interpret	args( ( CHAR_DATA *ch, char *argument ) );
//bool	is_number	args( ( char *arg ) );
bool	is_number	args( ( const char *arg ) );
int	number_argument	args( ( char *argument, char *arg ) );
int	mult_argument	args( ( char *argument, char *arg) );
char *	one_argument	args( ( char *argument, char *arg_first ) );

/* magic.c */
bool check_dispel_cancellation args((int dis_level, CHAR_DATA *victim, CHAR_DATA *ch, int sn,bool commune));
bool	check_absorb	args( (CHAR_DATA *ch,CHAR_DATA *victim,int dt) );
int	find_spell	args( ( CHAR_DATA *ch, const char *name) );
int 	mana_cost 	(CHAR_DATA *ch, int min_mana, int level);
int	skill_lookup	args( ( const char *name ) );
int	slot_lookup	args( ( int slot ) );
bool	saves_spell	args( ( int level, CHAR_DATA *victim, int dam_type ) );
void	obj_cast_spell	args( ( int sn, int level, CHAR_DATA *ch,
				    CHAR_DATA *victim, OBJ_DATA *obj ) );
int	average_ac(CHAR_DATA *ch);
bool	trusts		args((CHAR_DATA *ch, CHAR_DATA *victim));

/* morecabal.c */
void	set_extitle	(CHAR_DATA *ch, char *title);
void	set_fighting	(CHAR_DATA *ch, CHAR_DATA *victim);
void    check_bloodoath (CHAR_DATA *victim, char *argument);
int     empire_lookup   args( (const char *name) );

/* moremagic.c */
bool 	check_dispel		( int dis_level, CHAR_DATA *victim, int sn);
int	spellcraft_dam  	args( ( int num, int dice) );
bool    check_spellcraft 	args( (CHAR_DATA *ch, int sn) );
bool 	cleansed 		args( (CHAR_DATA *ch, CHAR_DATA *victim,int diffmodifier, int sn));
int 	get_affect_level	args( (CHAR_DATA *ch, int sn));
int     find_door       	args( ( CHAR_DATA *ch, char *arg ) );

/* save.c */
void	save_char_obj	args( ( CHAR_DATA *ch ) );
bool	load_char_obj	args( ( DESCRIPTOR_DATA *d, char *name ) );
int	form_lookup(char *argument);

/* skills.c */
bool	checkScrollsAcumen	args((CHAR_DATA *ch));
bool 	parse_gen_groups args( ( CHAR_DATA *ch,char *argument ) );
void 	list_group_costs args( ( CHAR_DATA *ch ) );
void    list_group_known args( ( CHAR_DATA *ch ) );
int     exp_per_level   args( ( CHAR_DATA *ch ) );
void 	check_improve	args( ( CHAR_DATA *ch, int sn, bool success,
				    int multiplier ) );
int 	group_lookup	args( (const char *name) );
void	gn_add		args( ( CHAR_DATA *ch, int gn) );
void 	gn_remove	args( ( CHAR_DATA *ch, int gn) );
void 	group_add	args( ( CHAR_DATA *ch, const char *name, bool deduct) );
void	group_remove	args( ( CHAR_DATA *ch, const char *name) );
int is_spec_skill args((int sn));
int is_mob_spec args((int sn));
bool is_specced args((CHAR_DATA *ch,int spec));
int	get_rand_skill	args((CHAR_DATA *ch, bool ignore_perfected));

/* special.c */
SF *	spec_lookup	args( ( const char *name ) );
char *	spec_name	args( ( SPEC_FUN *function ) );

/* string.c */
void	string_edit	args( ( CHAR_DATA *ch, char **pString ) );
void    string_append   args( ( CHAR_DATA *ch, char **pString ) );
char *	string_replace	args( ( char * orig, char * old, char * new ) );
void    string_add      args( ( CHAR_DATA *ch, char *argument ) );
char *  format_string   args( ( char *oldstring /*, bool fSpace */ ) );
char *  first_arg       args( ( char *argument, char *arg_first, bool fCase ) );
char *	string_unpad	args( ( char * argument ) );
char *	string_proper	args( ( char * argument ) );

/* olc.c */
bool	run_olc_editor	args( ( DESCRIPTOR_DATA *d ) );
char	*olc_ed_name	args( ( CHAR_DATA *ch ) );
char	*olc_ed_vnum	args( ( CHAR_DATA *ch ) );

/* lookup.c */
CHAR_DATA *find_char_by_name	args( (char *name) );
MAT_DATA *find_material_by_name	args( (char *name) );
int	race_lookup		args( ( const char *name) );
int	item_lookup		args( ( const char *name) );
int	liq_lookup		args( ( const char *name) );

int direction_lookup(char *dir);
void half_chop(const char *string, char *arg1, char *arg2);

/* teleport.c */
RID *	room_by_name	args( ( char *target, int level, bool error) );

/* update.c */
void	do_mob_spec args((CHAR_DATA *ch));
void	advance_level	args( ( CHAR_DATA *ch, bool hide ) );
void	gain_exp	args( ( CHAR_DATA *ch, int gain ) );
void	gain_quest_credits	args( ( CHAR_DATA *ch, int gain ) );
void	gain_condition	args( ( CHAR_DATA *ch, int iCond, int value ) );
void	update_handler	args( ( void ) );
void    spec_update     args( ( CHAR_DATA *ch ) );
int 	get_death_age	args((CHAR_DATA *ch));
char *	get_age_name	args((CHAR_DATA *ch));
void    room_update     args( ( void ) );
bool    IS_IMP          args( ( CHAR_DATA *ch ) );
int	get_age_new(int age, int racenumber);
char *	get_age_name_new(int age, int racenumber);
int	get_death_age_new(int racenumber, int con, int age_mod);
void	resurrect		args( (CHAR_DATA *ch, AFFECT_DATA *paf) );

#undef	CD
#undef	MID
#undef	OD
#undef	OID
#undef	RID
#undef	SF
#undef AD

/* tracerah.c */
bool 	is_protected_city	args((AREA_DATA *area));
bool 	can_live_in		(CHAR_DATA *ch,int hometown);
int 	hometown_lookup		(const char *name);
void 	ADD_WAIT_STATE		(CHAR_DATA *ch, int npulse);
void 	WAIT_STATE 		args((CHAR_DATA *ch, int npulse));
bool 	no_vanish_area 		args((CHAR_DATA *ch));
int	check_darkshout		args((CHAR_DATA *ch));
int	count_hands		args((CHAR_DATA *ch));

/* swarrior */
int check_posture(CHAR_DATA *ch);

/*  iprog.c */
void iprog_set(OBJ_INDEX_DATA *, const char *, const char *);

/* mprog.c */
void mprog_set(MOB_INDEX_DATA *, const char *, const char *);

/* rprog.c */
void rprog_set(ROOM_INDEX_DATA *, const char *, const char *);

/* bounty.c */
void pay_bounty args((CHAR_DATA *ch, CHAR_DATA *victim));
void bounty_cb args((char *string));
void bounty_log args ((char *string));
void bounty_credit args((CHAR_DATA *ch, int credit));
void record_bounty args((CHAR_DATA *ch, CHAR_DATA *victim, int amount));

/* queststore.c */
void quest_reward_warrior_boots args((CHAR_DATA *ch));
void quest_reward_warrior_gauntlets args((CHAR_DATA *ch));
void quest_reward_warrior_plate args((CHAR_DATA *ch));
void quest_reward_warrior_weapon args((CHAR_DATA *ch));
void quest_reward_channeler_helm args((CHAR_DATA *ch));
void quest_reward_channeler_ankh args((CHAR_DATA *ch));
void quest_reward_channeler_robe args((CHAR_DATA *ch));
void quest_reward_channeler_neck args((CHAR_DATA *ch));
void quest_reward_healer_ring args((CHAR_DATA *ch));
void quest_reward_healer_sleeves args((CHAR_DATA *ch));
void quest_reward_healer_bracelet args((CHAR_DATA *ch));
void quest_reward_healer_statue args((CHAR_DATA *ch));
void quest_reward_thief_boots args((CHAR_DATA *ch));
void quest_reward_thief_belt args((CHAR_DATA *ch));
void quest_reward_thief_dagger args((CHAR_DATA *ch));
void quest_reward_thief_robe args((CHAR_DATA *ch));
void quest_reward_elementalist_helm args((CHAR_DATA *ch));
void quest_reward_elementalist_belt args((CHAR_DATA *ch));
void quest_reward_elementalist_neck args((CHAR_DATA *ch));
void quest_reward_elementalist_flame args((CHAR_DATA *ch));
void quest_reward_hp_boost args((CHAR_DATA *ch));
void quest_reward_mana_boost args((CHAR_DATA *ch));
void quest_reward_move_boost args((CHAR_DATA *ch));
void quest_reward_attack_modifier args((CHAR_DATA *ch));
void quest_reward_damage_reduction args((CHAR_DATA *ch));
void quest_reward_damage_enhancer args((CHAR_DATA *ch));
void quest_reward_remove_vuln_fire args((CHAR_DATA *ch));
void quest_reward_remove_vuln_cold args((CHAR_DATA *ch));
void quest_reward_remove_vuln_lightning args((CHAR_DATA *ch));
void quest_reward_remove_vuln_holy args((CHAR_DATA *ch));
void quest_reward_remove_vuln_negative args((CHAR_DATA *ch));
void quest_reward_remove_vuln_mental args((CHAR_DATA *ch));
void quest_reward_remove_vuln_iron args((CHAR_DATA *ch));
void quest_reward_remove_vuln_drowning args((CHAR_DATA *ch));
void quest_reward_remove_vuln_light args((CHAR_DATA *ch));
void quest_reward_remove_vuln_bash args((CHAR_DATA *ch));
void quest_reward_add_resist_slash args((CHAR_DATA *ch));
void quest_reward_add_resist_pierce args((CHAR_DATA *ch));
void quest_reward_add_resist_bash args((CHAR_DATA *ch));
void quest_reward_pill_recall args((CHAR_DATA *ch));
void quest_reward_pill_antidote args((CHAR_DATA *ch));
void quest_reward_pill_purifier args((CHAR_DATA *ch));
void quest_reward_pill_holy_water args((CHAR_DATA *ch));
void quest_reward_pill_teleport args((CHAR_DATA *ch));
void quest_reward_pill_flying args((CHAR_DATA *ch));
void quest_reward_pill_pass_door args((CHAR_DATA *ch));
void check_set_item_warrior args((CHAR_DATA *ch, int setItemsWorn, bool increase));
void check_set_item_channeler args((CHAR_DATA *ch, int setItemsWorn, bool increase));
void check_set_item_thief args((CHAR_DATA *ch, int setItemsWorn, bool increase));
void check_set_item_healer args((CHAR_DATA *ch, int setItemsWorn, bool increase));
void check_set_item_elementalist args((CHAR_DATA *ch, int setItemsWorn, bool increase));

/*****************************************************************************
 *                                    OLC                                    *
 *****************************************************************************/

/*
 * Object defined in limbo.are
 * Used in save.c to load objects that don't exist.
 */
#define OBJ_VNUM_DUMMY	30

/*
 * Area flags.
 */
#define         AREA_NONE       0
#define         AREA_CHANGED    1	/* Area has been modified. */
#define         AREA_ADDED      2	/* Area has been added to. */
#define         AREA_LOADING    4	/* Used for counting in db.c */
#define		AREA_CLOSED	5	/* No Mortals allowed into closed area */

#define MAX_DIR	6
#define NO_FLAG -99	/* Must not be used in flags or stats. */

/*
 * Global Constants
 */
extern	char *	const	dir_name        [];
extern	const	sh_int	rev_dir         [];          /* sh_int - ROM OLC */
extern	const	struct	spec_type	spec_table	[];
extern  char *  const   he_she		[];
extern  char *  const   him_her		[];
extern  char *  const   his_her		[];

/*
 * Global variables
 */
extern		AREA_DATA *		area_first;
extern		AREA_DATA *		area_last;
extern		SHOP_DATA *		shop_last;

extern		int			top_affect;
extern		int			top_area;
extern		int			top_ed;
extern		long			top_exit;
extern		long			top_help;
extern		long			top_mob_index;
extern		long			top_obj_index;
extern		long			top_reset;
extern		long			top_room;
extern		long			top_shop;

extern		long			top_vnum_mob;
extern		long			top_vnum_obj;
extern		long			top_vnum_room;

extern		char			str_empty       [1];

extern		MOB_INDEX_DATA *	mob_index_hash  [MAX_KEY_HASH];
extern		OBJ_INDEX_DATA *	obj_index_hash  [MAX_KEY_HASH];
extern		ROOM_INDEX_DATA *	room_index_hash [MAX_KEY_HASH];

struct empire_type
{
	char	*name;
	char	*who_name;
};

extern	const	struct	empire_type	empire_table[MAX_EMPIRE];

struct bit_type
{
	const struct flag_type *	table;
	char *				help;
};

int get_spell_aftype(CHAR_DATA *ch);
