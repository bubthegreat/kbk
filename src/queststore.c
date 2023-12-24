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

void do_redeeminfo(CHAR_DATA *ch, char *argument)
{
	int looper;
	bool found = FALSE;

	if (argument[0] == '\0')
		return send_to_char("Redeeminfo what?\n\r", ch);

	if (ch->in_room->vnum != ROOM_VNUM_ALTAR)
		return send_to_char("You must be at the altar in Midgaard to access the Quest Store.\n\r", ch);

	for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
	{
		if (!str_cmp(argument, quest_reward_table[looper].keyword))
		{
			int raceClassLooper, raceCount = 0, classCount = 0;
			char buf[MSL], buf2[MSL];

			sprintf(buf, "============================================================================\nKEYWORD: {B%-20s{xPRICE: {R%d{x quest credits\n============================================================================\n", quest_reward_table[looper].keyword,
					quest_reward_table[looper].price);
			strcat(buf, "CLASSES: \n");
			for (raceClassLooper = 0; raceClassLooper < MAX_CLASS; raceClassLooper++)
			{
				if (raceClassLooper != MAX_CLASS - 2)
				{
					sprintf(buf2, "%12s [%c]    ", capitalize(class_table[raceClassLooper].name), quest_reward_table[looper].acc_class[raceClassLooper] ? 'X' : ' ');
					classCount++;
					strcat(buf, buf2);
					if (classCount % 4 == 0)
						strcat(buf, "\n");
				}
			}

			strcat(buf, "\nRACES: \n");
			for (raceClassLooper = 1; raceClassLooper < MAX_PC_RACE; raceClassLooper++)
			{
				if (str_cmp(race_table[raceClassLooper].name, "ruugrah") && str_cmp(race_table[raceClassLooper].name, "vuuhu") && str_cmp(race_table[raceClassLooper].name, "malefisti") &&
					str_cmp(race_table[raceClassLooper].name, "changeling") && str_cmp(race_table[raceClassLooper].name, "angel"))
				{
					sprintf(buf2, "%12s [%c]    ", capitalize(race_table[raceClassLooper].name), quest_reward_table[looper].acc_race[raceClassLooper] ? 'X' : ' ');
					raceCount++;
					strcat(buf, buf2);
					if (raceCount % 4 == 0)
						strcat(buf, "\n");
				}
			}

			strcat(buf, "\nALIGNS: ");
			if (quest_reward_table[looper].acc_align == ALIGN_G)
				strcat(buf, "good");
			else if (quest_reward_table[looper].acc_align == ALIGN_N)
				strcat(buf, "neutral");
			else if (quest_reward_table[looper].acc_align == ALIGN_E)
				strcat(buf, "evil");
			else if (quest_reward_table[looper].acc_align == ALIGN_NE)
				strcat(buf, "neutral evil");
			else if (quest_reward_table[looper].acc_align == ALIGN_GN)
				strcat(buf, "good neutral");
			else if (quest_reward_table[looper].acc_align == ALIGN_GE)
				strcat(buf, "good evil");
			else
				strcat(buf, "any");
			strcat(buf, "\nETHOS: ");
			if (quest_reward_table[looper].acc_ethos == ETHOS_L)
				strcat(buf, "lawful");
			else if (quest_reward_table[looper].acc_ethos == ETHOS_N)
				strcat(buf, "neutral");
			else if (quest_reward_table[looper].acc_align == ETHOS_C)
				strcat(buf, "chaotic");
			else if (quest_reward_table[looper].acc_ethos == ETHOS_LN)
				strcat(buf, "lawful neutral");
			else if (quest_reward_table[looper].acc_ethos == ETHOS_NC)
				strcat(buf, "neutral chaotic");
			else if (quest_reward_table[looper].acc_align == ETHOS_LC)
				strcat(buf, "lawful chaotic");
			else
				strcat(buf, "any");

			if (quest_reward_table[looper].set != 0)
			{
				sprintf(buf2, "\n\n** This item is an integral piece of the %s set. **", set_type_table[quest_reward_table[looper].set].setName);
				strcat(buf, buf2);
			}
			sprintf(buf2, "\n\nREWARD DESCRIPTION:\n  %s\n\r",
					quest_reward_table[looper].description);
			strcat(buf, buf2);
			send_to_char(buf, ch);
			found = TRUE;
		}
	}

	if (!found)
		return send_to_char("That is not a valid redeemable reward.\n\r", ch);
	return;
}

void do_redeem(CHAR_DATA *ch, char *argument)
{
	if (argument[0] == '\0')
		return send_to_char("Redeem what?\n\r", ch);

	char buf[MSL], buf2[MSL];
	int looper;
	int armor = 0, weapon = 0, boost = 0, misc = 0;
	bool found = FALSE;

	if (ch->in_room->vnum != ROOM_VNUM_ALTAR)
		return send_to_char("You must be at the altar in Midgaard to access the Quest Store.\n\r", ch);

	if (!str_cmp(argument, "list"))
	{
		send_to_char("                      {R======================================{x\n", ch);
		send_to_char("                      {R={x          KBK Quest Store           {R={x\n", ch);
		send_to_char("                      {R={x           Full Item List           {R={x\n", ch);
		send_to_char("                      {R======================================{x\n", ch);
		sprintf(buf2, "                                     *ARMOR*\n--------------------------------------------------------------------------------\n");
		send_to_char(buf2, ch);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (quest_reward_table[looper].type == REWARD_ARMOR)
			{
				char name[MSL];
				sprintf(name, "\"%s\"", quest_reward_table[looper].keyword);
				int price = quest_reward_table[looper].price, priceLoop = 0, nextPriceLoop = 0, digits = 0;

				for (priceLoop = 1; priceLoop <= 1000000; priceLoop = nextPriceLoop)
				{
					nextPriceLoop = priceLoop * 10;
					if (price >= priceLoop && price < priceLoop * 10)
						digits++;
				}
				if (armor == 1)
				{
					sprintf(buf, "%-24s %s%5d credits\n", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					armor = 0;
				}
				else
				{
					sprintf(buf, "%-24s %s%5d credits    ", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					armor++;
				}
				send_to_char(buf, ch);
			}
		}

		sprintf(buf2, "\n\n       	              	            *WEAPONS*\n--------------------------------------------------------------------------------\n");
		send_to_char(buf2, ch);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (quest_reward_table[looper].type == REWARD_WEAPON)
			{
				char name[MSL];
				sprintf(name, "\"%s\"", quest_reward_table[looper].keyword);
				int price = quest_reward_table[looper].price, priceLoop = 0, nextPriceLoop = 0, digits = 0;

				for (priceLoop = 1; priceLoop <= 1000000; priceLoop = nextPriceLoop)
				{
					nextPriceLoop = priceLoop * 10;
					if (price >= priceLoop && price < priceLoop * 10)
						digits++;
				}

				if (weapon == 1)
				{
					sprintf(buf, "%-24s %s%5d credits\n", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					weapon = 0;
				}
				else
				{
					sprintf(buf, "%-24s %s%5d credits    ", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					weapon++;
				}
				send_to_char(buf, ch);
			}
		}

		sprintf(buf2, "\n\n       	              	            *BOOSTS*\n--------------------------------------------------------------------------------\n");
		send_to_char(buf2, ch);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (quest_reward_table[looper].type == REWARD_BOOST)
			{
				char name[MSL];
				sprintf(name, "\"%s\"", quest_reward_table[looper].keyword);
				int price = quest_reward_table[looper].price, priceLoop = 0, nextPriceLoop = 0, digits = 0;

				for (priceLoop = 1; priceLoop <= 1000000; priceLoop = nextPriceLoop)
				{
					nextPriceLoop = priceLoop * 10;
					if (price >= priceLoop && price < priceLoop * 10)
						digits++;
				}

				if (boost == 1)
				{
					sprintf(buf, "%-24s %s%5d credits\n", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					boost = 0;
				}
				else
				{
					sprintf(buf, "%-24s %s%5d credits    ", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					boost++;
				}
				send_to_char(buf, ch);
			}
		}

		sprintf(buf2, "\n\n       	              	         *MISCELLANEOUS*	            \n--------------------------------------------------------------------------------\n");
		send_to_char(buf2, ch);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (quest_reward_table[looper].type == REWARD_MISCELLANEOUS)
			{
				char name[MSL];
				sprintf(name, "\"%s\"", quest_reward_table[looper].keyword);
				int price = quest_reward_table[looper].price, priceLoop = 0, nextPriceLoop = 0, digits = 0;

				for (priceLoop = 1; priceLoop <= 1000000; priceLoop = nextPriceLoop)
				{
					nextPriceLoop = priceLoop * 10;
					if (price >= priceLoop && price < priceLoop * 10)
						digits++;
				}

				if (misc == 1)
				{
					sprintf(buf, "%-24s %s%5d credits\n", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					misc = 0;
				}
				else
				{
					sprintf(buf, "%-24s %s%5d credits    ", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					misc++;
				}
				send_to_char(buf, ch);
			}
		}
		send_to_char("\n\nSee 'redeeminfo <keyword>' for more information on an item.\n\r", ch);
		return;
	}

	if (!str_cmp(argument, "armorlist"))
	{
		send_to_char("                      {R======================================{x\n", ch);
		send_to_char("                      {R={x          KBK Quest Store           {R={x\n", ch);
		send_to_char("                      {R======================================{x\n", ch);
		sprintf(buf2, "       	              	             *ARMOR*\n--------------------------------------------------------------------------------\n");
		send_to_char(buf2, ch);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (quest_reward_table[looper].type == REWARD_ARMOR)
			{
				char name[MSL];
				sprintf(name, "\"%s\"", quest_reward_table[looper].keyword);
				int price = quest_reward_table[looper].price, priceLoop = 0, nextPriceLoop = 0, digits = 0;

				for (priceLoop = 1; priceLoop <= 1000000; priceLoop = nextPriceLoop)
				{
					nextPriceLoop = priceLoop * 10;
					if (price >= priceLoop && price < priceLoop * 10)
						digits++;
				}

				if (armor == 1)
				{
					sprintf(buf, "%-24s %s%5d credits\n", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					armor = 0;
				}
				else
				{
					sprintf(buf, "%-24s %s%5d credits    ", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					armor++;
				}
				send_to_char(buf, ch);
			}
		}
		send_to_char("\n\nSee 'redeeminfo <keyword>' for more information on an item.\n\r", ch);
		return;
	}

	if (!str_cmp(argument, "boostslist"))
	{
		send_to_char("                      {R======================================{x\n", ch);
		send_to_char("                      {R={x          KBK Quest Store           {R={x\n", ch);
		send_to_char("                      {R======================================{x\n", ch);
		sprintf(buf2, "       	              	            *BOOSTS*\n--------------------------------------------------------------------------------\n");
		send_to_char(buf2, ch);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (quest_reward_table[looper].type == REWARD_BOOST)
			{
				char name[MSL];
				sprintf(name, "\"%s\"", quest_reward_table[looper].keyword);
				int price = quest_reward_table[looper].price, priceLoop = 0, nextPriceLoop = 0, digits = 0;

				for (priceLoop = 1; priceLoop <= 1000000; priceLoop = nextPriceLoop)
				{
					nextPriceLoop = priceLoop * 10;
					if (price >= priceLoop && price < priceLoop * 10)
						digits++;
				}

				if (boost == 1)
				{
					sprintf(buf, "%-24s %s%5d credits\n", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					boost = 0;
				}
				else
				{
					sprintf(buf, "%-24s %s%5d credits    ", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					misc++;
				}
				send_to_char(buf, ch);
			}
		}
		send_to_char("\n\nSee 'redeeminfo <keyword>' for more information on an item.\n\r", ch);
		return;
	}

	if (!str_cmp(argument, "weaponslist"))
	{
		send_to_char("                      {R======================================{x\n", ch);
		send_to_char("                      {R={x          KBK Quest Store           {R={x\n", ch);
		send_to_char("                      {R======================================{x\n", ch);
		sprintf(buf2, "       	              	            *WEAPONS*\n--------------------------------------------------------------------------------\n");
		send_to_char(buf2, ch);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (quest_reward_table[looper].type == REWARD_WEAPON)
			{
				char name[MSL];
				sprintf(name, "\"%s\"", quest_reward_table[looper].keyword);
				int price = quest_reward_table[looper].price, priceLoop = 0, nextPriceLoop = 0, digits = 0;

				for (priceLoop = 1; priceLoop <= 1000000; priceLoop = nextPriceLoop)
				{
					nextPriceLoop = priceLoop * 10;
					if (price >= priceLoop && price < priceLoop * 10)
						digits++;
				}

				if (weapon == 1)
				{
					sprintf(buf, "%-24s %s%5d credits\n", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					weapon = 0;
				}
				else
				{
					sprintf(buf, "%-24s %s%5d credits    ", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					weapon++;
				}
				send_to_char(buf, ch);
			}
		}
		send_to_char("\n\nSee 'redeeminfo <keyword>' for more information on an item.\n\r", ch);
		return;
	}

	if (!str_cmp(argument, "misclist") || !str_cmp(argument, "miscellaneous"))
	{
		send_to_char("                      {R======================================{x\n", ch);
		send_to_char("                      {R={x          KBK Quest Store           {R={x\n", ch);
		send_to_char("                      {R======================================{x\n", ch);
		sprintf(buf2, "       	              	         *MISCELLANEOUS*	            \n--------------------------------------------------------------------------------\n");
		send_to_char(buf2, ch);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (quest_reward_table[looper].type == REWARD_MISCELLANEOUS)
			{
				char name[MSL];
				sprintf(name, "\"%s\"", quest_reward_table[looper].keyword);
				int price = quest_reward_table[looper].price, priceLoop = 0, nextPriceLoop = 0, digits = 0;

				for (priceLoop = 1; priceLoop <= 1000000; priceLoop = nextPriceLoop)
				{
					nextPriceLoop = priceLoop * 10;
					if (price >= priceLoop && price < priceLoop * 10)
						digits++;
				}

				if (misc == 1)
				{
					sprintf(buf, "%-24s %s%5d credits\n", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					misc = 0;
				}
				else
				{
					sprintf(buf, "%-24s %s%5d credits    ", name,
							digits == 7 ? "    " : digits > 5 ? "     "
											   : digits > 4	  ? "      "
											   : digits > 3	  ? "        "
											   : digits > 2	  ? "         "
											   : digits > 1	  ? "          "
															  : "",
							quest_reward_table[looper].price);
					misc++;
				}
				send_to_char(buf, ch);
			}
		}
		send_to_char("\n\nSee 'redeeminfo <keyword>' for more information on an item.\n\r", ch);
		return;
	}

	for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
	{
		if (!str_cmp(argument, quest_reward_table[looper].keyword))
		{
			if (ch->quest_credits >= quest_reward_table[looper].price)
			{
				if (!quest_reward_table[looper].acc_race[ch->race] && !IS_IMMORTAL(ch))
					return send_to_char("Your race cannot buy this.\n\r", ch);
				if (!quest_reward_table[looper].acc_class[ch->class] && !IS_IMMORTAL(ch))
					return send_to_char("Your class cannot buy this.\n\r", ch);
				int align, ethos;
				if (ch->alignment < 0)
					align = ALIGN_E;
				else if (ch->alignment == 0)
					align = ALIGN_N;
				else
					align = ALIGN_G;
				if (!(quest_reward_table[looper].acc_align & align) && !IS_IMMORTAL(ch))
				{
					switch (align)
					{
					case ALIGN_E:
						return send_to_char("Those of evil hearts cannot buy this.", ch);
					case ALIGN_G:
						return send_to_char("Those of good hearts cannot buy this.", ch);
					case ALIGN_N:
						return send_to_char("Those of grey hearts cannot buy this.", ch);
					}
				}

				if (ch->pcdata->ethos < 0)
					ethos = ETHOS_C;
				else if (ch->pcdata->ethos == 0)
					ethos = ETHOS_N;
				else
					ethos = ETHOS_L;
				if (!(quest_reward_table[looper].acc_ethos & ethos) && !IS_IMMORTAL(ch))
				{
					switch (ethos)
					{
					case ETHOS_C:
						return send_to_char("Those of chaotic hearts cannot buy this.", ch);
					case ETHOS_N:
						return send_to_char("Those of neutral hearts cannot buy this.", ch);
					case ETHOS_L:
						return send_to_char("Those of lawful hearts cannot buy this.", ch);
					}
				}
				ch->quest_credits -= quest_reward_table[looper].price;
				act("You cast your quest credits into the wind and pray for divine assistance!", ch, 0, 0, TO_CHAR);
				act("$n casts quest credits into the wind and prays for divine assistance!", ch, 0, 0, TO_ROOM);
				char log_buf[MSL];
				sprintf(log_buf, "%s has redeemed: %s.", ch->name, argument);
				log_string(log_buf);
				(*quest_reward_table[looper].function)(ch);
				found = TRUE;
			}
			else
				return send_to_char("You do not have enough quest credits to purchase that.\n\r", ch);
		}
	}

	if (!found)
		return send_to_char("That is not a valid redeemable reward.\n\r", ch);
	return;
}

bool check_already_has(CHAR_DATA *ch, int vnum, char *quest_buy_keyword)
{
	OBJ_DATA *dup, *dup_next;
	int looper;

	for (dup = ch->carrying; dup != NULL; dup = dup_next)
	{
		dup_next = dup->next;
		if (dup->pIndexData->vnum == vnum)
		{
			if (dup->owner != NULL)
			{
				if (!str_cmp(dup->owner, ch->original_name))
				{
					act("You can only carry one of those.", ch, 0, 0, TO_CHAR);
					for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
					{
						if (!str_cmp(quest_reward_table[looper].keyword, quest_buy_keyword))
						{
							act("You are refunded your quest credits.", ch, 0, 0, TO_CHAR);
							ch->quest_credits += quest_reward_table[looper].price;
							return TRUE;
						}
					}
				}
			}
		}
	}
	return FALSE;
}

void quest_reward_warrior_boots(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_WARRIOR_BOOTS, str_dup("warrior set boots")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_WARRIOR_BOOTS), 55);
	act("A pair of steel boots flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A pair of steel boots flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_warrior_gauntlets(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_WARRIOR_GAUNTLETS, str_dup("warrior set gauntlets")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_WARRIOR_GAUNTLETS), 55);
	act("A pair of mithril-bound gauntlets flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A pair of mithril-bound gauntlets flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_warrior_plate(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_WARRIOR_PLATE, str_dup("warrior set plate")))
		return;
	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_WARRIOR_PLATE), 55);
	act("A steel battle chest flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A steel battle chest flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_warrior_weapon(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_WARRIOR_WEAPON, str_dup("warrior set weapon")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_WARRIOR_WEAPON), 55);
	act("A weapon flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A weapon flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_channeler_helm(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_CHANNELER_HELM, str_dup("channeler set helm")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_CHANNELER_HELM), 55);
	act("A bejewelled crest flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A bejewelled crest flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_channeler_ankh(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_CHANNELER_ANKH, str_dup("channeler set ankh")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_CHANNELER_ANKH), 55);
	act("An ankh flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("An ankh crest flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_channeler_robe(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_CHANNELER_ROBE, str_dup("channeler set robe")))
		return;
	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_CHANNELER_ROBE), 55);
	act("An ethereal robe flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("An ethereal robe flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_channeler_neck(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_CHANNELER_NECK, str_dup("channeler set neck")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_CHANNELER_NECK), 55);
	act("A necklace flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A necklace flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_thief_boots(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_THIEF_BOOTS, str_dup("thief set boots")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_THIEF_BOOTS), 55);
	act("A pair of boots flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A pair of boots flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_thief_robe(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_THIEF_ROBE, str_dup("thief set robe")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_THIEF_ROBE), 55);
	act("A shroud flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A shroud flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_thief_dagger(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_THIEF_DAGGER, str_dup("thief set weapon")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_THIEF_DAGGER), 55);
	act("A skewer flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A skewer flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_thief_belt(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_THIEF_BELT, str_dup("thief set belt")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_THIEF_BELT), 55);
	act("A sash flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A sash flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_healer_ring(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_HEALER_RING, str_dup("healer set ring")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_HEALER_RING), 55);
	act("A ring flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A ring flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_healer_sleeves(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_HEALER_SLEEVES, str_dup("healer set sleeves")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_HEALER_SLEEVES), 55);
	act("A pair of sleeves flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A pair of sleeves flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_healer_bracelet(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_HEALER_BRACELET, str_dup("healer set bracelet")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_HEALER_BRACELET), 55);
	act("A bracelet flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A bracelet flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_healer_statue(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_HEALER_STATUE, str_dup("healer set statue")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_HEALER_STATUE), 55);
	act("A statuette flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A statuette flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_elementalist_helm(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_ELEMENTALIST_HELM, str_dup("elementalist set helm")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_ELEMENTALIST_HELM), 55);
	act("A halo of frozen flame flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A halo of frozen flame flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_elementalist_neck(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_ELEMENTALIST_NECK, str_dup("elementalist set neck")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_ELEMENTALIST_NECK), 55);
	act("A cloak of water flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A cloak of water flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_elementalist_belt(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_ELEMENTALIST_BELT, str_dup("elementalist set belt")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_ELEMENTALIST_BELT), 55);
	act("A belt of electricity flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A belt of electricity flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_elementalist_flame(CHAR_DATA *ch)
{
	if (check_already_has(ch, OBJ_VNUM_QUEST_STORE_ELEMENTALIST_FLAME, str_dup("elementalist set ankh")))
		return;

	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_ELEMENTALIST_FLAME), 55);
	act("A pulsating flame flashes into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A pulsating flame flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_hp_boost(CHAR_DATA *ch)
{
	act("As your prayers are answered, you feel infused with newfound vigor.", ch, 0, 0, TO_CHAR);
	act("$n appears infused with a newfound vigor.", ch, 0, 0, TO_ROOM);
	ch->max_hit += 10;
	return;
}

void quest_reward_mana_boost(CHAR_DATA *ch)
{
	act("As your prayers are answered, your mind feels stronger and sharper.", ch, 0, 0, TO_CHAR);
	ch->max_mana += 30;
	return;
}

void quest_reward_move_boost(CHAR_DATA *ch)
{
	act("As your prayers are answered, your legs swell slightly, the muscles growing larger.", ch, 0, 0, TO_CHAR);
	ch->max_move += 30;
	return;
}

void quest_reward_attack_modifier(CHAR_DATA *ch)
{
	act("As your prayers are answered, your arms feel infused with great agility.", ch, 0, 0, TO_CHAR);
	ch->numAttacks += 1;
	return;
}

void quest_reward_damage_reduction(CHAR_DATA *ch)
{
	act("As your prayers are answered, you feel your skin and bones become toughened.", ch, 0, 0, TO_CHAR);
	ch->dam_mod -= 1;
	return;
}

void quest_reward_damage_enhancer(CHAR_DATA *ch)
{
	act("As your prayers are answered, your body and mind swell with increased strength.", ch, 0, 0, TO_CHAR);
	ch->enhancedDamMod += 1;
	return;
}

void quest_reward_remove_vuln_fire(CHAR_DATA *ch)
{
	if (!IS_SET(ch->vuln_flags, VULN_FIRE))
	{
		int looper;
		act("You are already not vulnerable to fire.", ch, 0, 0, TO_CHAR);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (!str_cmp(quest_reward_table[looper].keyword, "remove_vuln_fire"))
			{
				act("You are refunded your quest credits.", ch, 0, 0, TO_CHAR);
				ch->quest_credits += quest_reward_table[looper].price;
				return;
			}
		}
	}

	act("As your prayers are answered, your body feels less vulnerable to fire.", ch, 0, 0, TO_CHAR);
	REMOVE_BIT(ch->vuln_flags, VULN_FIRE);
	return;
}

void quest_reward_remove_vuln_cold(CHAR_DATA *ch)
{
	if (!IS_SET(ch->vuln_flags, VULN_COLD))
	{
		int looper;
		act("You are already not vulnerable to cold.", ch, 0, 0, TO_CHAR);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (!str_cmp(quest_reward_table[looper].keyword, "remove_vuln_cold"))
			{
				act("You are refunded your quest credits.", ch, 0, 0, TO_CHAR);
				ch->quest_credits += quest_reward_table[looper].price;
				return;
			}
		}
	}

	act("As your prayers are answered, your body feels less vulnerable to cold.", ch, 0, 0, TO_CHAR);
	REMOVE_BIT(ch->vuln_flags, VULN_COLD);
	return;
}

void quest_reward_remove_vuln_lightning(CHAR_DATA *ch)
{
	if (!IS_SET(ch->vuln_flags, VULN_COLD))
	{
		int looper;
		act("You are already not vulnerable to lightning.", ch, 0, 0, TO_CHAR);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (!str_cmp(quest_reward_table[looper].keyword, "remove_vuln_lightning"))
			{
				act("You are refunded your quest credits.", ch, 0, 0, TO_CHAR);
				ch->quest_credits += quest_reward_table[looper].price;
				return;
			}
		}
	}

	act("As your prayers are answered, your body feels less vulnerable to lightning.", ch, 0, 0, TO_CHAR);
	REMOVE_BIT(ch->vuln_flags, VULN_LIGHTNING);
	return;
}

void quest_reward_remove_vuln_holy(CHAR_DATA *ch)
{
	if (!IS_SET(ch->vuln_flags, VULN_HOLY))
	{
		int looper;
		act("You are already not vulnerable to holy attacks.", ch, 0, 0, TO_CHAR);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (!str_cmp(quest_reward_table[looper].keyword, "remove_vuln_holy"))
			{
				act("You are refunded your quest credits.", ch, 0, 0, TO_CHAR);
				ch->quest_credits += quest_reward_table[looper].price;
				return;
			}
		}
	}

	act("As your prayers are answered, your body feels less vulnerable to holy attacks.", ch, 0, 0, TO_CHAR);
	REMOVE_BIT(ch->vuln_flags, VULN_HOLY);
	return;
}

void quest_reward_remove_vuln_negative(CHAR_DATA *ch)
{
	if (!IS_SET(ch->vuln_flags, VULN_NEGATIVE))
	{
		int looper;
		act("You are already not vulnerable to negative attacks.", ch, 0, 0, TO_CHAR);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (!str_cmp(quest_reward_table[looper].keyword, "remove_vuln_negative"))
			{
				act("You are refunded your quest credits.", ch, 0, 0, TO_CHAR);
				ch->quest_credits += quest_reward_table[looper].price;
				return;
			}
		}
	}

	act("As your prayers are answered, your body feels less vulnerable to negative attacks.", ch, 0, 0, TO_CHAR);
	REMOVE_BIT(ch->vuln_flags, VULN_NEGATIVE);
	return;
}

void quest_reward_remove_vuln_mental(CHAR_DATA *ch)
{
	if (!IS_SET(ch->vuln_flags, VULN_MENTAL))
	{
		int looper;
		act("You are already not vulnerable to lightning.", ch, 0, 0, TO_CHAR);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (!str_cmp(quest_reward_table[looper].keyword, "remove_vuln_mental"))
			{
				act("You are refunded your quest credits.", ch, 0, 0, TO_CHAR);
				ch->quest_credits += quest_reward_table[looper].price;
				return;
			}
		}
	}

	act("As your prayers are answered, your body feels less vulnerable to mental attacks.", ch, 0, 0, TO_CHAR);
	REMOVE_BIT(ch->vuln_flags, VULN_MENTAL);
	return;
}

void quest_reward_remove_vuln_bash(CHAR_DATA *ch)
{
	if (!IS_SET(ch->vuln_flags, VULN_BASH))
	{
		int looper;
		act("You are already not vulnerable to bashing.", ch, 0, 0, TO_CHAR);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (!str_cmp(quest_reward_table[looper].keyword, "remove_vuln_bash"))
			{
				act("You are refunded your quest credits.", ch, 0, 0, TO_CHAR);
				ch->quest_credits += quest_reward_table[looper].price;
				return;
			}
		}
	}

	act("As your prayers are answered, your body feels less vulnerable to bashing attacks.", ch, 0, 0, TO_CHAR);
	REMOVE_BIT(ch->vuln_flags, VULN_BASH);
	return;
}

void quest_reward_remove_vuln_drowning(CHAR_DATA *ch)
{
	if (!IS_SET(ch->vuln_flags, VULN_DROWNING))
	{
		int looper;
		act("You are already not vulnerable to drowning.", ch, 0, 0, TO_CHAR);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (!str_cmp(quest_reward_table[looper].keyword, "remove_vuln_drowning"))
			{
				act("You are refunded your quest credits.", ch, 0, 0, TO_CHAR);
				ch->quest_credits += quest_reward_table[looper].price;
				return;
			}
		}
	}

	act("As your prayers are answered, your body feels less vulnerable to drowning.", ch, 0, 0, TO_CHAR);
	REMOVE_BIT(ch->vuln_flags, VULN_DROWNING);
	return;
}

void quest_reward_remove_vuln_iron(CHAR_DATA *ch)
{
	if (!IS_SET(ch->vuln_flags, VULN_IRON))
	{
		int looper;
		act("You are already not vulnerable to iron.", ch, 0, 0, TO_CHAR);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (!str_cmp(quest_reward_table[looper].keyword, "remove_vuln_iron"))
			{
				act("You are refunded your quest credits.", ch, 0, 0, TO_CHAR);
				ch->quest_credits += quest_reward_table[looper].price;
				return;
			}
		}
	}

	act("As your prayers are answered, your body feels less vulnerable to iron-based attacks.", ch, 0, 0, TO_CHAR);
	REMOVE_BIT(ch->vuln_flags, VULN_IRON);
	return;
}

void quest_reward_remove_vuln_light(CHAR_DATA *ch)
{
	if (!IS_SET(ch->vuln_flags, VULN_LIGHT))
	{
		int looper;
		act("You are already not vulnerable to light.", ch, 0, 0, TO_CHAR);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (!str_cmp(quest_reward_table[looper].keyword, "remove_vuln_light"))
			{
				act("You are refunded your quest credits.", ch, 0, 0, TO_CHAR);
				ch->quest_credits += quest_reward_table[looper].price;
				return;
			}
		}
	}

	act("As your prayers are answered, your body feels less vulnerable to light.", ch, 0, 0, TO_CHAR);
	REMOVE_BIT(ch->vuln_flags, VULN_LIGHT);
	return;
}

void quest_reward_add_resist_slash(CHAR_DATA *ch)
{
	if (IS_SET(ch->res_flags, RES_SLASH))
	{
		int looper;
		act("You are already resistant to slashing attacks.", ch, 0, 0, TO_CHAR);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (!str_cmp(quest_reward_table[looper].keyword, "add_resist_slash"))
			{
				act("You are refunded your quest credits.", ch, 0, 0, TO_CHAR);
				ch->quest_credits += quest_reward_table[looper].price;
				return;
			}
		}
	}

	act("As your prayers are answered, your body feels resistant to slashing attacks.", ch, 0, 0, TO_CHAR);
	SET_BIT(ch->res_flags, RES_SLASH);
	return;
}

void quest_reward_add_resist_pierce(CHAR_DATA *ch)
{
	if (IS_SET(ch->res_flags, RES_PIERCE))
	{
		int looper;
		act("You are already resistant to piercing attacks.", ch, 0, 0, TO_CHAR);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (!str_cmp(quest_reward_table[looper].keyword, "add_resist_pierce"))
			{
				act("You are refunded your quest credits.", ch, 0, 0, TO_CHAR);
				ch->quest_credits += quest_reward_table[looper].price;
				return;
			}
		}
	}

	act("As your prayers are answered, your body feels resistant to piercing attacks.", ch, 0, 0, TO_CHAR);
	SET_BIT(ch->res_flags, RES_PIERCE);
	return;
}

void quest_reward_add_resist_bash(CHAR_DATA *ch)
{
	if (IS_SET(ch->res_flags, RES_BASH))
	{
		int looper;
		act("You are already resistant to bashing attacks.", ch, 0, 0, TO_CHAR);
		for (looper = 0; looper < MAX_QUEST_REWARDS; looper++)
		{
			if (!str_cmp(quest_reward_table[looper].keyword, "add_resist_bash"))
			{
				act("You are refunded your quest credits.", ch, 0, 0, TO_CHAR);
				ch->quest_credits += quest_reward_table[looper].price;
				return;
			}
		}
	}

	act("As your prayers are answered, your body feels resistant to bashing attacks.", ch, 0, 0, TO_CHAR);
	SET_BIT(ch->res_flags, RES_BASH);
	return;
}

void quest_reward_pill_recall(CHAR_DATA *ch)
{
	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_PILL_RECALL), 55);
	act("A pill into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A pill flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_pill_antidote(CHAR_DATA *ch)
{
	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_PILL_CURE_POISON), 55);
	act("A pill into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A pill flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_pill_purifier(CHAR_DATA *ch)
{
	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_PILL_CURE_DISEASE), 55);
	act("A pill into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A pill flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_pill_holy_water(CHAR_DATA *ch)
{
	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_PILL_REMOVE_CURSE), 55);
	act("A pill into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A pill flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_pill_teleport(CHAR_DATA *ch)
{
	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_PILL_TELEPORT), 55);
	act("A pill into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A pill flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_pill_flying(CHAR_DATA *ch)
{
	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_PILL_FLYING), 55);
	act("A pill into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A pill flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void quest_reward_pill_pass_door(CHAR_DATA *ch)
{
	OBJ_DATA *obj = create_object(get_obj_index(OBJ_VNUM_QUEST_STORE_PILL_PASS_DOOR), 55);
	act("A pill into your hands in a bolt of energy!", ch, 0, 0, TO_CHAR);
	act("A pill flashes into $n's hands in a bolt of energy!", ch, 0, 0, TO_ROOM);
	obj->owner = str_dup(ch->original_name);
	obj_to_char(obj, ch);
	return;
}

void check_set_item_warrior(CHAR_DATA *ch, int currNum, bool increase)
{
	if (increase)
	{
		if (currNum == 2)
		{
			ch->max_hit += 200;
			act("{yYou feel healthier.{x", ch, 0, 0, TO_CHAR);
		}

		else if (currNum == 3)
		{
			ch->pcdata->learned[skill_lookup("critical strike")] = 100;
			act("{yYou feel a rush of insight into Critical Strike!{x", ch, 0, 0, TO_CHAR);
		}
		else if (currNum == 4)
		{
			ch->numAttacks++;
			act("{yYour arms increase in speed.{x", ch, 0, 0, TO_CHAR);
		}
	}
	else
	{
		if (currNum == 3)
		{
			ch->numAttacks--;
			act("{yYour arms slow down.{x", ch, 0, 0, TO_CHAR);
		}

		else if (currNum == 2)
		{
			ch->pcdata->learned[skill_lookup("critical strike")] = -2;
			act("{yYou lose your ability to Critical Strike!{x", ch, 0, 0, TO_CHAR);
		}
		else if (currNum == 1)
		{
			ch->max_hit -= 200;
			act("{yYou feel more vulnerable.{x", ch, 0, 0, TO_CHAR);
		}
	}
	return;
}

void check_set_item_channeler(CHAR_DATA *ch, int currNum, bool increase)
{
	if (increase)
	{
		if (currNum == 2)
		{
			ch->pcdata->learned[skill_lookup("mana shield")] = 100;
			act("{yYou feel a rush of insight into Mana Shield!{x", ch, 0, 0, TO_CHAR);
		}

		else if (currNum == 3)
		{
			act("{yYou feel more mentally stable and healthier.{x", ch, 0, 0, TO_CHAR);
			ch->max_mana += 200;
			ch->max_hit += 100;
		}
		else if (currNum == 4)
		{
			act("{yYou feel a rush of insight into Doublecast!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("doublecast")] = 100;
		}
	}
	else
	{
		if (currNum == 3)
		{
			ch->pcdata->learned[skill_lookup("doublecast")] = -2;
			act("{yYou lose your Doublecast ability!{x", ch, 0, 0, TO_CHAR);
		}

		else if (currNum == 2)
		{
			act("{yYour mental and physical fortitude leaves you.{x", ch, 0, 0, TO_CHAR);
			ch->max_mana -= 200;
			ch->max_hit -= 100;
		}
		else if (currNum == 1)
		{
			ch->pcdata->learned[skill_lookup("mana shield")] = -2;
			act("{yYou lose your Mana Shield ability!{x", ch, 0, 0, TO_CHAR);
		}
	}
	return;
}

void check_set_item_healer(CHAR_DATA *ch, int currNum, bool increase)
{
	if (increase)
	{
		if (currNum == 2)
		{
			act("{yYour god grants you divine favor. An iron resolve grips you as your mind grows stronger.{x", ch, 0, 0, TO_CHAR);
			ch->max_mana += 450;
			act("{yYou feel a rush of insight into Coalesce Resistance!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("coalesce resistance")] = 100;
		}

		else if (currNum == 3)
		{
			act("{yYou feel a rush of insight into Group Recall!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("group recall")] = 100;
			act("{yYou feel a rush of insight into Group Sanctuary!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("group sanctuary")] = 100;
			act("{yYou feel a rush of insight into Group Gate!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("group gate")] = 100;
			act("{yYou feel a rush of insight into Bulwark of Blades!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("bulwark of blades")] = 100;
		}
		else if (currNum == 4)
		{
			act("{yYou feel a rush of insight into Blade Mastery!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("blade mastery")] = 100;
		}
	}
	else
	{
		if (currNum == 3)
		{
			ch->pcdata->learned[skill_lookup("blade mastery")] = -2;
			act("{yYou lose your Blade Mastery ability!{x", ch, 0, 0, TO_CHAR);
		}

		else if (currNum == 2)
		{
			ch->pcdata->learned[skill_lookup("bulwark of blades")] = -2;
			act("{yYou lose your Bulwark of Blades ability!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("group recall")] = -2;
			act("{yYou lose your Group Recall ability!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("group gate")] = -2;
			act("{yYou lose your Group Gate ability!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("group sanctuary")] = -2;
			act("{yYou lose your Group Sanctuary ability!{x", ch, 0, 0, TO_CHAR);
		}
		else if (currNum == 1)
		{
			act("{yYour god's holy favor fades away.{x", ch, 0, 0, TO_CHAR);
			ch->max_mana -= 450;
			act("{yYou lose your Coalesce Resistance ability!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("coalesce resistance")] = -2;
		}
	}
	return;
}

void check_set_item_thief(CHAR_DATA *ch, int currNum, bool increase)
{
	if (increase)
	{
		if (currNum == 2)
		{
			ch->max_hit += 150;
			act("{yYou feel a rush of vigor through your veins.{x", ch, 0, 0, TO_CHAR);
		}

		else if (currNum == 3)
		{
			act("{yYou feel a rush of insight into Improved Blackjack!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("improved blackjack")] = 100;
		}
		else if (currNum == 4)
		{
			act("{yYou feel a rush of insight into Disguise!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("disguise")] = 100;
		}
	}
	else
	{
		if (currNum == 3)
		{
			ch->pcdata->learned[skill_lookup("disguise")] = -2;
			act("{yYou lose your Disguise ability!{x", ch, 0, 0, TO_CHAR);
		}

		else if (currNum == 2)
		{
			ch->pcdata->learned[skill_lookup("improved blackjack")] = -2;
			act("{yYou lose your Improved Blackjack ability!{x", ch, 0, 0, TO_CHAR);
		}
		else if (currNum == 1)
		{
			ch->max_hit -= 150;
			act("{yThe vigor coursing through your veins fades away.{x", ch, 0, 0, TO_CHAR);
		}
	}
	return;
}

void check_set_item_elementalist(CHAR_DATA *ch, int currNum, bool increase)
{
	if (increase)
	{
		if (currNum == 2)
		{
			ch->max_hit += 150;
			ch->max_mana += 300;
			act("{yYou feel empowered by the elements.{x", ch, 0, 0, TO_CHAR);
		}

		else if (currNum == 3)
		{
			act("{yYou feel a rush of insight into Bastion of Elements!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("bastion of elements")] = 100;
			act("{yYou feel a rush of insight into Vorpal Bolt!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("vorpal bolt")] = 100;
		}
		else if (currNum == 4)
		{
			act("{yYou feel a rush of insight into Maelstrom!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("maelstrom")] = 100;
		}
	}
	else
	{
		if (currNum == 3)
		{
			ch->pcdata->learned[skill_lookup("maelstrom")] = -2;
			act("{yYou lose your Maelstrom ability!{x", ch, 0, 0, TO_CHAR);
		}

		else if (currNum == 2)
		{
			ch->pcdata->learned[skill_lookup("bastion of elements")] = -2;
			act("{yYou lose your Bastion of Elements ability!{x", ch, 0, 0, TO_CHAR);
			ch->pcdata->learned[skill_lookup("vorpal bolt")] = -2;
			act("{yYou lose your Vorpal Bolt ability!{x", ch, 0, 0, TO_CHAR);
		}
		else if (currNum == 1)
		{
			ch->max_hit -= 150;
			ch->max_mana -= 300;
			act("{yThe vigor coursing through your veins fades away.{x", ch, 0, 0, TO_CHAR);
		}
	}
	return;
}
