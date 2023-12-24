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
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
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

char *target_name;

void do_supps(CHAR_DATA *ch, char *argument)
{
    BUFFER *buffer;
    char arg[MAX_INPUT_LENGTH];
    char spell_list[LEVEL_HERO + 1][MAX_STRING_LENGTH];
    char spell_columns[LEVEL_HERO + 1];
    int sn, level, min_lev = 1, max_lev = LEVEL_HERO, mana;
    bool fAll = FALSE, found = FALSE;
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
        return;

    if (class_table[ch->class].ctype != CLASS_COMMUNER && !IS_IMMORTAL(ch))
    {
        send_to_char("Your class knows no prayers.\n\r", ch);
        return;
    }

    if (argument[0] != '\0')
    {
        fAll = TRUE;

        if (str_prefix(argument, "all"))
        {
            argument = one_argument(argument, arg);
            if (!is_number(arg))
            {
                send_to_char("Arguments must be numerical or all.\n\r", ch);
                return;
            }
            max_lev = atoi(arg);

            if (max_lev < 1 || max_lev > LEVEL_HERO)
            {
                sprintf(buf, "Levels must be between 1 and %d.\n\r", LEVEL_HERO);
                send_to_char(buf, ch);
                return;
            }

            if (argument[0] != '\0')
            {
                argument = one_argument(argument, arg);
                if (!is_number(arg))
                {
                    send_to_char("Arguments must be numerical or all.\n\r", ch);
                    return;
                }
                min_lev = max_lev;
                max_lev = atoi(arg);

                if (max_lev < 1 || max_lev > LEVEL_HERO)
                {
                    sprintf(buf,
                            "Levels must be between 1 and %d.\n\r", LEVEL_HERO);
                    send_to_char(buf, ch);
                    return;
                }

                if (min_lev > max_lev)
                {
                    send_to_char("That would be silly.\n\r", ch);
                    return;
                }
            }
        }
    }

    /* initialize data */
    for (level = 0; level < LEVEL_HERO + 1; level++)
    {
        spell_columns[level] = 0;
        spell_list[level][0] = '\0';
    }

    for (sn = 0; sn < MAX_SKILL; sn++)
    {
        if (skill_table[sn].name == NULL)
            break;

        if ((level = skill_table[sn].skill_level[ch->class]) < LEVEL_HERO + 1 && level >= min_lev && level <= max_lev && skill_table[sn].spell_fun != spell_null && ch->pcdata->learned[sn] > 0 && (skill_table[sn].ctype == CMD_COMMUNE || skill_table[sn].ctype == CMD_BOTH))
        {
            found = TRUE;
            level = skill_table[sn].skill_level[ch->class];
            if (ch->level < level)
                sprintf(buf, "%-18s n/a      ", skill_table[sn].name);
            else
            {
                mana = UMAX(skill_table[sn].min_mana,
                            100 / (2 + ch->level - level));

                switch (get_curr_stat(ch, STAT_INT))
                {
                case 25:
                    mana = 43 * mana / 50;
                    break;
                case 24:
                    mana = 22 * mana / 25;
                    break;
                case 23:
                    mana = 45 * mana / 50;
                    break;
                case 22:
                    mana = 23 * mana / 25;
                    break;
                case 21:
                    mana = 47 * mana / 50;
                    break;
                case 20:
                    mana = 24 * mana / 25;
                    break;
                case 19:
                    mana = 49 * mana / 50;
                    break;
                }
                sprintf(buf, "%-18s  %3d mana  ", skill_table[sn].name, mana);
            }

            if (spell_list[level][0] == '\0')
                sprintf(spell_list[level], "\n\rLevel %2d: %s", level, buf);
            else /* append */
            {
                if (++spell_columns[level] % 2 == 0)
                    strcat(spell_list[level], "\n\r          ");
                strcat(spell_list[level], buf);
            }
        }
    }

    /* return results */

    if (!found)
    {
        send_to_char("No supplications found.\n\r", ch);
        return;
    }

    buffer = new_buf();
    for (level = 0; level < LEVEL_HERO + 1; level++)
        if (spell_list[level][0] != '\0')
            add_buf(buffer, spell_list[level]);
    add_buf(buffer, "\n\r");
    page_to_char(buf_string(buffer), ch);
    free_buf(buffer);
}
void do_commune(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    void *vo;
    int mana;
    int sn;
    AFFECT_DATA af;
    int target;
    if (IS_NPC(ch) && ch->desc == NULL)
        return;
    target_name = one_argument(argument, arg1);
    one_argument(target_name, arg2);

    if (class_table[ch->class].ctype != CLASS_COMMUNER && !IS_IMMORTAL(ch))
        return send_to_char("You aren't in touch well enough with the gods to commune prayers.\n\r", ch);

    if (IS_SET(ch->act, PLR_BETRAYER))
        return send_to_char("Your god has forsaken you!\n\r", ch);

    if (!IS_NPC(ch) && ch->pcdata->oalign == 3 && ch->alignment == -1000)
        return send_to_char("Your god has forsaken you!\n\r", ch);

    if (is_affected(ch, gsn_worldbind))
    {
        send_to_char("Your prayers go unanswered.\n\r", ch);
        return;
    }

    if (arg1[0] == '\0')
        return send_to_char("Commune which what where?\n\r", ch);

    if ((sn = find_spell(ch, arg1)) < 1 || skill_table[sn].spell_fun == spell_null || (!IS_NPC(ch) && get_skill(ch, sn) < 5) || (!IS_NPC(ch) && ch->pcdata->learned[sn] == 0))
        return send_to_char("You don't know any prayers of that name.\n\r", ch);

    //    if(skill_table[sn].skill_level[ch->class]>=51 && !IS_SET(ch->act,PLR_EMPOWERED) && !IS_IMMORTAL(ch))
    //	return send_to_char("The gods do not find you worthy of a prayer of that magnitude.\n\r",ch);

    if (ch->position < skill_table[sn].minimum_position)
        return send_to_char("You can't concentrate enough.\n\r", ch);

    if ((skill_table[sn].ctype == CMD_SPELL || skill_table[sn].ctype == CMD_POWER) && !IS_IMMORTAL(ch))
        return send_to_char("You can't commune that.\n\r", ch);

    if (ch->level + 2 == skill_table[sn].skill_level[ch->class])
        mana = 50;
    else
    {
        if (is_affected(ch, gsn_garble))
        {
            mana = UMAX(
                (skill_table[sn].min_mana * 2),
                100 / (2 + ch->level - skill_table[sn].skill_level[ch->class]));
            switch (get_curr_stat(ch, STAT_INT))
            {
            case 25:
                mana = 43 * mana / 50;
                break;
            case 24:
                mana = 22 * mana / 25;
                break;
            case 23:
                mana = 45 * mana / 50;
                break;
            case 22:
                mana = 23 * mana / 25;
                break;
            case 21:
                mana = 47 * mana / 50;
                break;
            case 20:
                mana = 24 * mana / 25;
                break;
            case 19:
                mana = 49 * mana / 50;
                break;
            }
        }
        else
        {
            mana = UMAX(
                skill_table[sn].min_mana,
                100 / (2 + ch->level - skill_table[sn].skill_level[ch->class]));
            switch (get_curr_stat(ch, STAT_INT))
            {
            case 25:
                mana = 43 * mana / 50;
                break;
            case 24:
                mana = 22 * mana / 25;
                break;
            case 23:
                mana = 45 * mana / 50;
                break;
            case 22:
                mana = 23 * mana / 25;
                break;
            case 21:
                mana = 47 * mana / 50;
                break;
            case 20:
                mana = 24 * mana / 25;
                break;
            case 19:
                mana = 49 * mana / 50;
                break;
            }
        }
    }
    /*
     * Locate targets.
     */
    victim = NULL;
    obj = NULL;
    vo = NULL;
    target = TARGET_NONE;
    af.aftype = AFT_COMMUNE;
    switch (skill_table[sn].target)
    {
    default:
        bug("Do_cast: bad target for sn %d.", sn);
        return;
    case TAR_IGNORE:
        break;
    case TAR_CHAR_OFFENSIVE:
        if (arg2[0] == '\0')
        {
            if ((victim = ch->fighting) == NULL)
            {
                send_to_char("Commune the prayer on whom?\n\r", ch);
                return;
            }
        }
        else
        {
            if ((victim = get_char_room(ch, target_name)) == NULL)
            {
                send_to_char("They aren't here.\n\r", ch);
                return;
            }
        }
        if (!IS_NPC(ch))
        {
            check_killer(ch, victim);
        }
        if (IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim)
        {
            send_to_char("You can't do that on your own master.\n\r",
                         ch);
            return;
        }
        vo = (void *)victim;
        target = TARGET_CHAR;
        break;
    case TAR_CHAR_DEFENSIVE:
        if (arg2[0] == '\0')
        {
            victim = ch;
        }
        else
        {
            if ((victim = get_char_room(ch, target_name)) == NULL)
            {
                send_to_char("They aren't here.\n\r", ch);
                return;
            }
        }

        vo = (void *)victim;
        target = TARGET_CHAR;
        break;
    case TAR_CHAR_SELF:
        if (arg2[0] != '\0' && !is_name(target_name, ch->name))
        {
            send_to_char("You cannot commune this prayer on another.\n\r", ch);
            return;
        }

        vo = (void *)ch;
        target = TARGET_CHAR;
        break;
    case TAR_OBJ_INV:
        if (arg2[0] == '\0')
        {
            send_to_char("What should the prayer be communed upon?\n\r", ch);
            return;
        }

        if ((obj = get_obj_carry(ch, target_name, ch)) == NULL)
        {
            send_to_char("You are not carrying that.\n\r", ch);
            return;
        }

        vo = (void *)obj;
        target = TARGET_OBJ;
        break;
    case TAR_OBJ_CHAR_OFF:
        if (arg2[0] == '\0')
        {
            if ((victim = ch->fighting) == NULL)
            {
                send_to_char("Commune the prayer on whom or what?\n\r", ch);
                return;
            }
            target = TARGET_CHAR;
        }
        else if ((victim = get_char_room(ch, target_name)) != NULL)
        {
            target = TARGET_CHAR;
        }
        if (target == TARGET_CHAR) /* check the sanity of the attack */
        {
            if (IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim)
                return send_to_char("You can't do that on your own follower.\n\r", ch);

            if (!IS_NPC(ch))
                check_killer(ch, victim);

            vo = (void *)victim;
        }
        else if ((obj = get_obj_here(ch, target_name)) != NULL)
        {
            vo = (void *)obj;
            target = TARGET_OBJ;
        }
        else
            return send_to_char("You don't see that here.\n\r", ch);

        break;
    case TAR_OBJ_CHAR_DEF:
        if (arg2[0] == '\0')
        {
            vo = (void *)ch;
            target = TARGET_CHAR;
        }
        else if ((victim = get_char_room(ch, target_name)) != NULL)
        {
            vo = (void *)victim;
            target = TARGET_CHAR;
        }
        else if ((obj = get_obj_carry(ch, target_name, ch)) != NULL)
        {
            vo = (void *)obj;
            target = TARGET_OBJ;
        }
        else
            return send_to_char("You don't see that here.\n\r", ch);

        break;
    }
    if (!IS_NPC(ch) && ch->mana < mana)
        return send_to_char("You don't have enough mana.\n\r", ch);

    WAIT_STATE(ch, skill_table[sn].beats);
    if (!IS_NPC(ch) && (number_percent() > get_skill(ch, sn)))
    {
        send_to_char("You lost your concentration.\n\r", ch);
        check_improve(ch, sn, FALSE, 1);
        ch->mana -= mana / 2;
    }
    else
    {
        ch->mana -= mana;
        if (skill_table[sn].target == TAR_CHAR_OFFENSIVE && is_safe(ch, victim))
            return;

        if (skill_table[sn].target == TAR_CHAR_OFFENSIVE)
        {
            if (!IS_NPC(ch) && !IS_NPC(victim) && (ch->fighting == NULL || victim->fighting == NULL))
            {
                switch (number_range(0, 2))
                {
                case (0):
                case (1):
                    sprintf(buf, "Die, %s you filthy dog!", PERS(ch, victim));
                    break;
                case (2):
                    sprintf(buf, "Help! %s is communing a supplication on me!", PERS(ch, victim));
                }
                if (victim != ch && !IS_NPC(ch))
                    do_myell(victim, buf);
            }
        }
        if (skill_table[sn].target == TAR_CHAR_OFFENSIVE && victim != ch)
        {
            act("You narrow your eyes and glare in $N's direction.", ch, 0, victim, TO_CHAR);
            act("$n narrows $s eyes and glares in $N's direction.", ch, 0, victim, TO_NOTVICT);
            act("$n narrows $s eyes and glares in your direction.", ch, 0, victim, TO_VICT);
        }
        if (skill_table[sn].target == TAR_CHAR_SELF || skill_table[sn].target == TAR_IGNORE ||
            skill_table[sn].target == TAR_OBJ_CHAR_DEF || skill_table[sn].target == TAR_OBJ_CHAR_OFF ||
            (skill_table[sn].target == TAR_CHAR_DEFENSIVE && victim == ch))
        {
            act("You close your eyes and concentrate for a moment.", ch, 0, 0, TO_CHAR);
            act("$n closes $s eyes with a look of concentration for a moment.", ch, 0, 0, TO_ROOM);
        }
        if (skill_table[sn].target == TAR_CHAR_DEFENSIVE && victim != ch)
        {
            act("You close your eyes for a moment and nod at $N.", ch, 0, victim, TO_CHAR);
            act("$n closes $s eyes for a moment and nods at $N.", ch, 0, victim, TO_NOTVICT);
            act("$n closes $s eyes for a moment and nods at you.", ch, 0, victim, TO_VICT);
        }
        if (skill_table[sn].target == TAR_OBJ_INV)
        {
            act("You furrow your brow as you look through your possessions.", ch, 0, 0, TO_CHAR);
            act("$n furrows $s brow as $e looks through $s possessions.", ch, 0, 0, TO_ROOM);
        }
        (*skill_table[sn].spell_fun)(sn, ch->level + GET_HITROLL(ch) / 20, ch, vo, target);
        check_improve(ch, sn, TRUE, 1);
    }
    if ((skill_table[sn].target == TAR_CHAR_OFFENSIVE || (skill_table[sn].target == TAR_OBJ_CHAR_OFF && target == TARGET_CHAR)) && victim != ch && victim->master != ch)
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
        for (vch = ch->in_room->people; vch; vch = vch_next)
        {
            vch_next = vch->next_in_room;
            if (victim == vch && victim->fighting == NULL)
            {
                check_killer(victim, ch);
                multi_hit(victim, ch, TYPE_UNDEFINED);
                break;
            }
        }
    }
    return;
}

void spell_healing_sleep(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *)vo;
    AFFECT_DATA af;

    if (victim == NULL)
        victim = ch;

    if (IS_NPC(victim))
        return send_to_char("You may only commune this supplication on real people.\n\r", ch);

    if (!is_same_group(victim, ch))
        return send_to_char("You may only commune this supplication on group members.\n\r", ch);

    if (victim->position == POS_FIGHTING)
        return send_to_char("You may not commune this supplication on somebody while they are fighting.\n\r", ch);

    if ((IS_AFFECTED(victim, AFF_SLEEP)) || !IS_AWAKE(victim))
    {
        act("$N is already asleep.", ch, 0, victim, TO_CHAR);
        return;
    }

    init_affect(&af);
    af.where = TO_AFFECTS;
    af.aftype = AFT_COMMUNE;
    af.type = gsn_healing_sleep;
    af.level = level;
    af.duration = 3;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = AFF_SLEEP;
    affect_to_char(victim, &af);

    act("You put $N into a deep healing sleep.", ch, 0, victim, TO_CHAR);
    act("$n puts you into a deep healing sleep.", ch, 0, victim, TO_VICT);
    victim->position = POS_SLEEPING;

    return;
}

void spell_righteousness(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    AFFECT_DATA af;
    CHAR_DATA *victim = (CHAR_DATA *)vo;

    if (ch != victim)
    {
        send_to_char("You cannot commune this supplication on another.\n\r", ch);
        return;
    }

    if (is_affected(ch, sn))
    {
        send_to_char("You're already feeling righteous.\n\r", ch);
        return;
    }

    init_affect(&af);
    af.where = TO_IMMUNE;
    af.aftype = AFT_COMMUNE;
    af.type = sn;
    af.level = level;
    af.duration = 8;
    af.location = APPLY_NONE;
    af.modifier = 0;
    af.bitvector = IMM_HOLY;
    affect_to_char(victim, &af);
    send_to_char("You feel righteous.\n\r", victim);
    return;
}

void spell_blade_barrier(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *victim = (CHAR_DATA *)vo;
    int dam, spins;
    int chance, i;

    chance = 100;

    if (get_skill(ch, skill_lookup("blade mastery")) < 0)
    {
        dam = dice(level, 2);
        spins = number_range(2, 3);
        dam *= 9;
        dam /= 10;
    }
    else
    {
        dam = dice(level, 3);
        spins = number_range(4, 5);
    }

    if (number_percent() > chance || victim == ch)
    {
        act("$n creates a whirlwind of spinning blades which turn and strike $m down!", ch, 0, 0, TO_ROOM);
        act("Your blade barrier turns and strikes you down!", ch, 0, 0, TO_CHAR);

        dam /= 2;
        damage_old(ch, ch, dam, sn, DAM_SLASH, TRUE);
        for (i = 0; i < spins; i++)
        {
            if (ch->ghost > 0)
                return;

            if (saves_spell(level, ch, DAM_SLASH))
                dam /= 2;

            dam *= 3;
            dam /= 2;
            act("The blades spin and slice away at $n.", ch, 0, 0, TO_ROOM);
            act("The blades spin and slice away at you.", ch, 0, 0, TO_CHAR);
            damage_old(ch, ch, dam, sn, DAM_SLASH, TRUE);
        }
    }
    else
    {
        act("You create a whirlwind of spinning blades to strike down $N!", ch, 0, victim, TO_CHAR);
        act("$n creates a deadly blade barrier that tears into $N!", ch, 0, victim, TO_NOTVICT);
        act("$n creates a deadly blade barrier that tears into you!", ch, 0, victim, TO_VICT);

        if (get_skill(ch, skill_lookup("blade mastery")) > 0)
        {
            act("Your blade mastery drives the blades to strike true!", ch, 0, victim, TO_CHAR);
            act("$n's blade mastery drives the blades to strike true!", ch, 0, victim, TO_NOTVICT);
            act("$n's blade mastery drives the blades to strike true!", ch, 0, victim, TO_VICT);
        }

        damage_old(ch, victim, dam, sn, DAM_SLASH, TRUE);

        for (i = 0; i < spins; i++)
        {
            if (victim->ghost > 0 || (victim->in_room != ch->in_room))
                return;

            if (saves_spell(level, victim, DAM_SLASH))
                dam /= 2;

            if (get_skill(ch, skill_lookup("blade mastery")) < 0)
            {
                dam *= 3;
                dam /= 4;
            }
            else
            {
                dam *= 17;
                dam /= 20;
            }
            act("The blades spin and slice away at $n.", victim, 0, 0, TO_ROOM);
            act("The blades spin and slice away at you.", victim, 0, 0, TO_CHAR);

            if (victim->ghost > 0)
                return;

            damage_old(ch, victim, dam, sn, DAM_SLASH, TRUE);
        }
    }

    act("The blade barrier fades away.", ch, 0, 0, TO_ROOM);
    act("Your blade barrier fades away.", ch, 0, 0, TO_CHAR);
    return;
}

void spell_holy_word(int sn, int level, CHAR_DATA *ch, void *vo, int target)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam, bless_num, curse_num, frenzy_num, sanc_num, wrath_num;

    if (IS_GOOD(ch))
        wrath_num = skill_lookup("wrath");
    else
        wrath_num = skill_lookup("dark wrath");

    sanc_num = skill_lookup("sanctuary");
    bless_num = skill_lookup("bless");
    curse_num = skill_lookup("curse");
    frenzy_num = skill_lookup("frenzy");

    if (IS_EVIL(ch))
    {
        act("$n utters a word of unholy power!", ch, NULL, NULL, TO_ROOM);
        send_to_char("You utter a word of unholy power.\n\r", ch);
    }
    else
    {
        act("$n utters a word of divine power!", ch, NULL, NULL, TO_ROOM);
        send_to_char("You utter a word of divine power.\n\r", ch);
    }

    for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
    {
        vch_next = vch->next_in_room;

        if (vch->invis_level > LEVEL_HERO)
            continue;

        if ((IS_GOOD(ch) && IS_GOOD(vch)) || (IS_EVIL(ch) && IS_EVIL(vch)) || ((IS_NEUTRAL(ch) && IS_NEUTRAL(vch)) && (ch == vch)))
        {
            send_to_char("You feel more powerful.\n\r", vch);
            spell_frenzy(frenzy_num, 60, ch, (void *)vch, TARGET_CHAR);
            spell_bless(bless_num, 60, ch, (void *)vch, TARGET_CHAR);
            spell_sanctuary(sanc_num, 60, ch, (void *)vch, TARGET_CHAR);
        }
        else if ((IS_GOOD(ch) && IS_EVIL(vch)) || (IS_EVIL(ch) && IS_GOOD(vch)))
        {
            if (!is_safe_spell(ch, vch, TRUE))
            {
                spell_curse(curse_num, level * 2, ch, (void *)vch, TARGET_CHAR);
                send_to_char("You are struck down!\n\r", vch);
                if (IS_GOOD(ch))
                    spell_wrath(wrath_num, level * 2, ch, (void *)vch, TARGET_CHAR);
                else
                    spell_dark_wrath(wrath_num, level * 2, ch, (void *)vch, TARGET_CHAR);
            }
        }
        else if (IS_NEUTRAL(ch))
        {
            if (!is_safe_spell(ch, vch, TRUE))
            {
                spell_curse(curse_num, level / 2, ch, (void *)vch, TARGET_CHAR);
                send_to_char("You are struck down!\n\r", vch);
                dam = dice(level, 8);
                damage_old(ch, vch, dam, sn, DAM_ENERGY, TRUE);
            }
        }
    }
    send_to_char("You feel drained.\n\r", ch);
    ch->move *= .9;
    ch->mana *= .9;
    ch->hit *= .9;
}
