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

// Find a material by its name - Zornath
MAT_DATA *find_material_by_name(char *name)
{
    if (!name)
        return find_material_by_name("none");

    MAT_DATA *material;
    for (material = mat_list; material != NULL; material = material->next)
    {
        if (!str_cmp(material->name, name))
            return material;
    }
    return find_material_by_name("none");
}

CHAR_DATA *find_char_by_name(char *name)
{
    if (!name)
        return NULL;

    CHAR_DATA *vch, *vch_next;

    // Check if any characters logged in match the name
    for (vch = char_list; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next;

        // Sanity check?
        if (IS_NPC(vch))
            continue;

        if (!strcmp(vch->original_name, name))
            return vch;
    }

    // Not found
    return NULL;
}

int flag_lookup(const char *name, const struct flag_type *flag_table)
{
    int flag;

    for (flag = 0; flag_table[flag].name != NULL; flag++)
    {
        if (LOWER(name[0]) == LOWER(flag_table[flag].name[0]) && !str_prefix(name, flag_table[flag].name))
            return flag_table[flag].bit;
    }

    return 0;
}

char *flag_name_lookup(long bitv, const struct flag_type *flag_table)
{
    int flag;
    for (flag = 0; flag_table[flag].name != NULL; flag++)
        if (flag_table[flag].bit == bitv)
            return str_dup(flag_table[flag].name);
    return NULL;
}

int cabal_lookup(const char *name)
{
    int cabal;

    for (cabal = 0; cabal < MAX_CABAL; cabal++)
    {
        if (LOWER(name[0]) == LOWER(cabal_table[cabal].name[0]) && !str_prefix(name, cabal_table[cabal].name))
            return cabal;
    }

    return 0;
}

int position_lookup(const char *name)
{
    int pos;

    for (pos = 0; position_table[pos].name != NULL; pos++)
    {
        if (LOWER(name[0]) == LOWER(position_table[pos].name[0]) && !str_prefix(name, position_table[pos].name))
            return pos;
    }

    return -1;
}

int sex_lookup(const char *name)
{
    int sex;

    for (sex = 0; sex_table[sex].name != NULL; sex++)
    {
        if (LOWER(name[0]) == LOWER(sex_table[sex].name[0]) && !str_prefix(name, sex_table[sex].name))
            return sex;
    }

    return -1;
}

int size_lookup(const char *name)
{
    int size;

    for (size = 0; size_table[size].name != NULL; size++)
    {
        if (LOWER(name[0]) == LOWER(size_table[size].name[0]) && !str_prefix(name, size_table[size].name))
            return size;
    }

    return -1;
}

int empire_lookup(const char *name)
{
    int empire;

    for (empire = 0; empire < MAX_EMPIRE; empire++)
    {
        if (LOWER(name[0]) == LOWER(empire_table[empire].name[0]) && !str_prefix(name, empire_table[empire].name))
            return empire - 1;
    }

    return -2;
}

HELP_DATA *help_lookup(char *keyword)
{
    HELP_DATA *pHelp;
    char temp[MIL], argall[MIL];

    argall[0] = '\0';

    while (keyword[0] != '\0')
    {
        keyword = one_argument(keyword, temp);
        if (argall[0] != '\0')
            strcat(argall, " ");
        strcat(argall, temp);
    }

    for (pHelp = help_first; pHelp != NULL; pHelp = pHelp->next)
        if (is_name(argall, pHelp->keyword))
            return pHelp;

    return NULL;
}

HELP_AREA *had_lookup(char *arg)
{
    HELP_AREA *temp;
    extern HELP_AREA *had_list;

    for (temp = had_list; temp; temp = temp->next)
        if (!str_cmp(arg, temp->filename))
            return temp;

    return NULL;
}

int hometown_lookup(const char *name)
{
    int hometown;

    for (hometown = 0; hometown < MAX_HOMETOWN; hometown++)
    {
        if (LOWER(name[0]) == LOWER(hometown_table[hometown].name[0]) && !str_prefix(name, hometown_table[hometown].name))
            return hometown;
    }

    return 0;
}

int direction_lookup(char *dir)
{
    int num;

    if (!str_cmp(dir, "n") || !str_cmp(dir, "north"))
        num = 0;
    else if (!str_cmp(dir, "e") || !str_cmp(dir, "east"))
        num = 1;
    else if (!str_cmp(dir, "s") || !str_cmp(dir, "south"))
        num = 2;
    else if (!str_cmp(dir, "w") || !str_cmp(dir, "west"))
        num = 3;
    else if (!str_cmp(dir, "u") || !str_cmp(dir, "up"))
        num = 4;
    else if (!str_cmp(dir, "d") || !str_cmp(dir, "down"))
        num = 5;
    else
        return -1;

    return num;
}

void half_chop(const char *string, char *arg1, char *arg2)
{
    while (isspace(*string))
        string++;

    while (*string && !isspace(*string))
        *arg1++ = *string++;
    *arg1 = '\0';

    while (isspace(*string))
        string++;

    strcpy(arg2, string);
}

AREA_DATA *area_lookup(const char *arg)
{
    AREA_DATA *pArea;

    for (pArea = area_first; pArea; pArea = pArea->next)
    {
        if (is_number(arg) && atoi(arg) == pArea->vnum)
            return pArea;

        else if (tolower(arg[0]) == tolower(pArea->name[0]) && !str_prefix(arg, pArea->name))
            return pArea;

        else if (tolower(arg[0]) == tolower(pArea->file_name[0]) && !str_prefix(arg, pArea->file_name))
            return pArea;
    }
    return NULL;
}

ROOM_INDEX_DATA *area_begin(AREA_DATA *pArea)
{
    ROOM_INDEX_DATA *pRoom;
    int vnum;

    for (vnum = pArea->min_vnum; vnum <= pArea->max_vnum; vnum++)
        if (vnum > 0 && (pRoom = get_room_index(vnum)) != NULL)
            return pRoom;

    return NULL;
}
