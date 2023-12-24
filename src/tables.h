/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *									   *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 *  Chastain, Michael Quan, and Mitchell Tse.				   *
 *									   *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc	   *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.						   *
 *									   *
 *  Much time and thought has gone into this software and you are	   *
 *  benefitting.  We hope that you share your changes too.  What goes	   *
 *  around, comes around.						   *
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

struct flag_type
{
    char *name;
    int bit;
    bool settable;
};

struct cabal_type
{
    char *name;
    char *who_name;
    char *long_name;
    sh_int hall;
    sh_int item_vnum;
    sh_int altar_vnum;
    sh_int key_vnum;
    sh_int max_members;
    bool independent; /* true for loners */
    bool status;
    bool induct;
};

struct cabal_restr_type
{
    bool acc_class[MAX_CLASS];
    bool acc_race[MAX_PC_RACE];
    int acc_align;
    int acc_ethos;
};

struct cabal_message
{
    char *login;
    char *logout;
    char *entrygreeting;
};

struct position_type
{
    char *name;
    char *short_name;
};

struct sex_type
{
    char *name;
};

struct size_type
{
    char *name;
};

/*struct empire_type
{
    char 	*name;
    char 	*who_name;
};
*/

struct style_type
{
    long bit;
    char *name;
};

struct color_type
{
    char *color_name;
    char *color_ascii;
};

struct color_event_type
{
    char *event_name;
    char *default_color;
    int min_level;
};

extern const struct flag_type aftype_table[];

struct display_type
{
    char *name;
    long bit;
    char *display;
};

struct hometown_type
{
    char *name;
    int recall;
    int pit;
    int align;
    int ethos;
    int restrict;
};

struct bounty_type
{
    char *rank_name;
    char *rank_extitle;
    int credits_required;
};

struct quest_reward
{
    char *keyword;
    int price;
    int type;
    int setItem;
    int set;
    int vnum;
    char description[MSL];
    bool acc_class[MAX_CLASS];
    bool acc_race[MAX_PC_RACE];
    int acc_align;
    int acc_ethos;
    FUNC *function;
};

struct set_type
{
    int set;
    char *setName;
    SET_ITEM_PROG *function;
};

struct sector_type
{
    char *name;
    sh_int flag;
    int move;
    int wait;
    char *desc;
};

/* game tables */
extern const struct cabal_type cabal_table[MAX_CABAL];
extern const struct cabal_restr_type cabal_restr_table[MAX_CABAL];
extern const struct position_type position_table[];
extern const struct sex_type sex_table[];
extern const struct size_type size_table[];
extern const struct empire_type empire_table[MAX_EMPIRE];
extern const struct color_type color_table[MAX_COLORS];
extern const struct color_event_type color_event[MAX_EVENTS];
extern const struct cabal_message cabal_messages[MAX_CABAL];
extern const struct hometown_type hometown_table[MAX_HOMETOWN];
extern const struct display_type apply_locations[];
extern const struct bounty_type bounty_table[MAX_BOUNTY];
extern const struct quest_reward quest_reward_table[MAX_QUEST_REWARDS];
extern const struct set_type set_type_table[MAX_SET_TYPES];
extern const struct sector_type sector_table[SECT_MAX];
/* swarrior */
extern const struct style_type style_table[];

/* flag tables */
extern const struct flag_type act_flags[];
extern const struct flag_type plr_flags[];
extern const struct flag_type affect_flags[];
extern const struct flag_type off_flags[];
extern const struct flag_type extended_flags[];
extern const struct flag_type imm_flags[];
extern const struct flag_type form_flags[];
extern const struct flag_type part_flags[];
extern const struct flag_type comm_flags[];
extern const struct flag_type extra_flags[];
extern const struct flag_type restrict_flags[];
extern const struct flag_type wear_flags[];
extern const struct flag_type weapon_flags[];
extern const struct flag_type container_flags[];
extern const struct flag_type portal_flags[];
extern const struct flag_type room_flags[];
extern const struct flag_type exit_flags[];
extern const struct flag_type mprog_flags[];
extern const struct flag_type area_flags[];
extern const struct flag_type sector_flags[];
extern const struct flag_type door_resets[];
extern const struct flag_type wear_loc_strings[];
extern const struct flag_type wear_loc_flags[];
extern const struct flag_type res_flags[];
extern const struct flag_type imm_flags[];
extern const struct flag_type vuln_flags[];
extern const struct flag_type type_flags[];
extern const struct flag_type apply_flags[];
extern const struct flag_type sex_flags[];
extern const struct flag_type furniture_flags[];
extern const struct flag_type weapon_class[];
extern const struct flag_type apply_types[];
extern const struct flag_type weapon_type2[];
extern const struct flag_type apply_types[];
extern const struct flag_type size_flags[];
extern const struct flag_type position_flags[];
extern const struct flag_type ac_type[];
extern const struct bit_type bitvector_type[];
extern const struct flag_type direction_table[];
extern const struct flag_type area_type_table[];

extern const struct bit_type bitvector_type[];
extern const struct flag_type room_flags[];
extern const struct flag_type area_flags[];
extern const struct flag_type sector_flags[];
extern const struct flag_type wear_loc_strings[];
extern const struct flag_type wear_loc_flags[];
extern const struct flag_type door_resets[];

extern const struct flag_type mprog_flags[];

char *bitmask_string args((BITMASK * bm, const struct flag_type *flags));
