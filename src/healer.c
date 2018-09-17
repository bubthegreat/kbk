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

#include "include.h"

DECLARE_DO_FUN(do_say);

/* This is all from Rom except the restoration spell */
void do_heal(CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *mob;
    char arg[MAX_INPUT_LENGTH];
    int cost,sn;
    SPELL_FUN *spell;
    char *words;

    /* check for healer */
    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if ( IS_NPC(mob) && IS_SET(mob->act, ACT_IS_HEALER) )
            break;
    }

    if ( mob == NULL )
    {
        send_to_char( "You can't do that here.\n\r", ch );
        return;
    }

    if(mob->position==POS_SLEEPING) {
	send_to_char("The healer must be awake to heal you.\n\r",ch);
	return;
    }

    one_argument(argument,arg);

    if (mob->cabal != 0)
    {
    	if (mob->cabal != ch->cabal)
	{
    		do_say(mob,"I do not heal intruders in my cabal!");
		return;
	}
    }

    if (arg[0] == '\0')
    {
	if (mob->cabal != 0 && mob->cabal == ch->cabal)
	{
		do_say(mob, "Hail, friend. I offer you the following supplications to aid you in the cause: ");
        	send_to_char("  Light       :  Cure light wounds      25  gold\n\r",ch);
        	send_to_char("  Serious     :  Cure serious wounds    75  gold\n\r",ch);
        	send_to_char("  Critic      :  Cure critical wounds   125 gold\n\r",ch);
        	send_to_char("  Heal        :  Healing spell          250 gold\n\r",ch);
       		send_to_char("  Blind       :  Cure blindness         125 gold\n\r",ch);
        	send_to_char("  Disease     :  Cure disease           275 gold\n\r",ch);
        	send_to_char("  Poison      :  Cure poison            200 gold\n\r",ch);
        	send_to_char("  Uncurse     :  Remove curse           200 gold\n\r",ch);
        	send_to_char("  Restoration :  Restoration            350 gold\n\r",ch);
        	send_to_char("  Refresh     :  Restore movement       100 gold\n\r",ch);
        	send_to_char("  Mana        :  Restore mana           200 gold\n\r",ch);
        	send_to_char(" Type heal <type> to be healed.\n\r",ch);
	}
	else
	{
		do_say(mob, "I offer you the following supplications: ");
        	send_to_char("  Light       :  Cure light wounds      50  gold\n\r",ch);
        	send_to_char("  Serious     :  Cure serious wounds    150 gold\n\r",ch);
        	send_to_char("  Critic      :  Cure critical wounds   250 gold\n\r",ch);
        	send_to_char("  Heal        :  Healing spell          500 gold\n\r",ch);
       		send_to_char("  Blind       :  Cure blindness         250 gold\n\r",ch);
        	send_to_char("  Disease     :  Cure disease           550 gold\n\r",ch);
        	send_to_char("  Poison      :  Cure poison            400 gold\n\r",ch);
        	send_to_char("  Uncurse     :  Remove curse           400 gold\n\r",ch);
        	send_to_char("  Restoration :  Restoration            700 gold\n\r",ch);
        	send_to_char("  Refresh     :  Restore movement       200 gold\n\r",ch);
        	send_to_char("  Mana        :  Restore mana           400 gold\n\r",ch);
        	send_to_char(" Type heal <type> to be healed.\n\r",ch);
	}
		return;
    }

    if (!str_prefix(arg,"light"))
    {
        spell = spell_cure_light;
	sn    = skill_lookup("cure light");
        words = "cure light";
		cost = 50;
    }

    else if (!str_prefix(arg,"serious"))
    {
	spell = spell_cure_serious;
	sn    = skill_lookup("cure serious");
        words = "cure serious";
		cost = 150;
    }

    else if (!str_prefix(arg,"critical"))
    {
	spell = spell_cure_critical;
	sn    = skill_lookup("cure critical");
        words = "cure critical";
		cost = 250;
    }
    else if (!str_prefix(arg,"heal"))
    {
	spell = spell_heal;
	sn = skill_lookup("heal");
        words = "heal";
		cost = 500;
    }

    else if (!str_prefix(arg,"blindness"))
    {
	spell = spell_cure_blindness;
	sn    = skill_lookup("cure blindness");
        words = "cure blindness";
		cost = 250;
    }

    else if (!str_prefix(arg,"disease"))
    {
	spell = spell_cure_disease;
	sn    = skill_lookup("cure disease");
        words = "cure disease";
		cost = 550;
    }

    else if (!str_prefix(arg,"poison"))
    {
	spell = spell_cure_poison;
	sn    = skill_lookup("cure poison");
        words = "cure poison";
		cost = 400;
    }

    else if (!str_prefix(arg,"uncurse") || !str_prefix(arg,"curse"))
    {
	spell = spell_remove_curse;
	sn    = skill_lookup("remove curse");
        words = "remove curse";
		cost = 400;
    }

    else if (!str_prefix(arg,"restoration") || !str_prefix(arg,"restore"))
    {
	spell = spell_restoration;
	sn = skill_lookup("restoration");
	words = "restoration";
		cost = 700;
    }

    else if (!str_prefix(arg,"refresh") || !str_prefix(arg,"moves"))
    {
	spell =  spell_refresh;
	sn    = skill_lookup("refresh");
        words = "refresh";
		cost = 200;
    }

    else if (!str_prefix(arg,"mana") || !str_prefix(arg,"energize"))
    {
        spell = NULL;
        sn = -1;
        words = "energizer";
		cost = 400;
    }

    else
    {
	do_say(mob, "I don't offer that blessing. Type HEAL for a list of supplications.");
	return;
    }


    if (mob->cabal != 0 && mob->cabal == ch->cabal)
	    cost /= 2;

    if (mob->pIndexData->vnum == 2002) cost /= 4; // Training Grounds healer for newbies only

    if (cost > ch->gold)
    {
	do_say(mob, "I'm afraid you don't have enough gold for that.");
	return;
    }

    WAIT_STATE(ch,PULSE_VIOLENCE);


    deduct_cost(ch,cost);
    act("$n closes $s eyes for a moment and nods at $N.",mob,NULL,ch,TO_NOTVICT);
    act("$n closes $s eyes for a moment and nods at you.",mob,NULL,ch,TO_VICT);

    if (spell == NULL)
    {
    	ch->mana += dice(4,8) + mob->level / 3;
    	ch->mana = UMIN(ch->mana,ch->max_mana);
	send_to_char("A warm glow passes through you.\n\r",ch);
	return;
     }

     if (sn == -1)
	return;

     spell(sn,mob->level,mob,ch,TARGET_CHAR);
     do_visible(ch,"");
}
