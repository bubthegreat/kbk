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


/* tiara & rings of wizardry....do several unseen things:
Rings:
 1. Raises spellcraft percent by 5.
 2. Lowers mana cost of all spells by 10% (20% for high ring)
 3. Reduces magic damage past 200 points by 25%. (After sanc)
Tiara:
 1. Regens mana at 1d3 mana per PULSE_VIOLENCE.
 2. Reduces magic damage past 150 points by 50%. (after sanc)

 Remove these progs if you don't want to use the Drannor areas.
*/

#include "include.h"

void save_cabal_items args( ( void ) );

DECLARE_IPROG_FUN_WEAR(	wear_prog_crystal_tiara 		);
DECLARE_IPROG_FUN_WEAR(	wear_prog_ring_wizardry 		);
DECLARE_IPROG_FUN_WEAR( wear_prog_gauntlets_striking 		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_gauntlets_striking		);
DECLARE_IPROG_FUN_REMOVE( remove_prog_crystal_tiara 		);
DECLARE_IPROG_FUN_REMOVE( remove_prog_ring_wizardry 		);
DECLARE_IPROG_FUN_REMOVE( remove_prog_gauntlets_striking 	);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_tattoo_detlef 		);
DECLARE_IPROG_FUN_PULSE( pulse_prog_potato 			);
DECLARE_IPROG_FUN_GIVE( give_prog_cabal_item 			);
DECLARE_IPROG_FUN_FIGHT( fight_prog_golden_weapon 		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_ranger_staff 		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_etched_pendant 		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_cure_critical 		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_training_pendant 		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_arcana_sword 		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_hair_bracelet 		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_blood_bracelet		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_shredded_wings		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_deathwalk_statue		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_seraphim_axe		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_orb_dominion		);
DECLARE_IPROG_FUN_FIGHT ( fight_prog_tattoo_kat			);
DECLARE_IPROG_FUN_INVOKE ( invoke_prog_tattoo_daed		);
DECLARE_IPROG_FUN_FIGHT(	fight_prog_tattoo_daed		);
DECLARE_IPROG_FUN_FIGHT(	fight_prog_earthsplitter	);
DECLARE_IPROG_FUN_FIGHT( fight_prog_dispel_shield		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_tattoo_ormthrak		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_tattoo_ormthrak		);
DECLARE_IPROG_FUN_SPEECH( speech_prog_jade_orb			);
DECLARE_IPROG_FUN_WEAR(	wear_prog_stone_skin			);
DECLARE_IPROG_FUN_REMOVE( remove_prog_stone_skin		);
DECLARE_IPROG_FUN_WEAR(	wear_prog_shield			);
DECLARE_IPROG_FUN_REMOVE( remove_prog_shield			);
DECLARE_IPROG_FUN_WEAR(	wear_prog_aura				);
DECLARE_IPROG_FUN_REMOVE( remove_prog_aura			);
DECLARE_IPROG_FUN_FIGHT( fight_prog_tattoo_zornath		);
DECLARE_IPROG_FUN_WEAR(	wear_prog_anti_rager			);
DECLARE_IPROG_FUN_WEAR( wear_prog_rune_dagger			);
DECLARE_IPROG_FUN_REMOVE( remove_prog_rune_dagger		);
DECLARE_IPROG_FUN_FIGHT(  fight_prog_cloak_embrace		);
DECLARE_IPROG_FUN_WEAR( wear_prog_cloak_displacement		);
DECLARE_IPROG_FUN_REMOVE( remove_prog_cloak_displacement	);
DECLARE_IPROG_FUN_FIGHT( fight_prog_tattoo_tracerah		);

// Quest Store progs
DECLARE_IPROG_FUN_WEAR( wear_prog_quest_reward_warrior_boots	);
DECLARE_IPROG_FUN_WEAR( wear_prog_quest_reward_warrior_weapon	);
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_warrior_boots);
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_warrior_weapon);
DECLARE_IPROG_FUN_PULSE( pulse_prog_quest_reward_warrior_plate);
DECLARE_IPROG_FUN_PULSE( pulse_prog_quest_reward_warrior_gauntlets);
DECLARE_IPROG_FUN_SPEECH( speech_prog_quest_reward_warrior_weapon);
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_warrior_gauntlets);
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_warrior_plate);
DECLARE_IPROG_FUN_WEAR( wear_prog_quest_reward_channeler_helm);
DECLARE_IPROG_FUN_WEAR( wear_prog_quest_reward_channeler_robe);
DECLARE_IPROG_FUN_WEAR( wear_prog_quest_reward_channeler_neck);
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_channeler_helm);
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_channeler_robe);
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_channeler_neck);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_quest_reward_channeler_ankh);
DECLARE_IPROG_FUN_WEAR( wear_prog_quest_reward_thief_belt);
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_thief_belt);
DECLARE_IPROG_FUN_WEAR( wear_prog_quest_reward_thief_boots);
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_thief_boots);
DECLARE_IPROG_FUN_WEAR( wear_prog_quest_reward_thief_robe);
DECLARE_IPROG_FUN_PULSE( pulse_prog_quest_reward_thief_robe);
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_thief_robe);
DECLARE_IPROG_FUN_WEAR( wear_prog_quest_reward_healer_ring );
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_healer_ring );
DECLARE_IPROG_FUN_WEAR( wear_prog_quest_reward_healer_sleeves );
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_healer_sleeves );
DECLARE_IPROG_FUN_WEAR( wear_prog_quest_reward_healer_bracelet );
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_healer_bracelet );
DECLARE_IPROG_FUN_INVOKE( invoke_prog_quest_reward_healer_statue );
DECLARE_IPROG_FUN_WEAR( wear_prog_quest_reward_elementalist_neck );
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_elementalist_neck );
DECLARE_IPROG_FUN_WEAR( wear_prog_quest_reward_elementalist_helm );
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_elementalist_helm );
DECLARE_IPROG_FUN_WEAR( wear_prog_quest_reward_elementalist_belt );
DECLARE_IPROG_FUN_REMOVE( remove_prog_quest_reward_elementalist_belt );

// Obsidian Palace
DECLARE_IPROG_FUN_GET( get_prog_palace_key_one			);
DECLARE_IPROG_FUN_GET( get_prog_palace_key_two			);
DECLARE_IPROG_FUN_GET( get_prog_palace_key_three		);
DECLARE_IPROG_FUN_GET( get_prog_palace_key_four			);
DECLARE_IPROG_FUN_GET( get_prog_palace_key_five			);
DECLARE_IPROG_FUN_GET( get_prog_palace_key_six			);
DECLARE_IPROG_FUN_FIGHT( fight_prog_sword_baalzebub 		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_sword_belal 		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_sword_prince 		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_crown_prince 		);
DECLARE_IPROG_FUN_SPEECH( speech_prog_sword_prince		);
DECLARE_IPROG_FUN_SPEECH( speech_prog_crown_prince		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_gloves_fortitude		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_crystal_talon		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_belt_fire_demon		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_bracer_diamond		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_devil_shield		);
DECLARE_IPROG_FUN_FIGHT( fight_prog_sword_infinity 		);
DECLARE_IPROG_FUN_DEATH( death_prog_sword_infinity		);
DECLARE_IPROG_FUN_PULSE( pulse_prog_sword_infinity		);
DECLARE_IPROG_FUN_SAC( sac_prog_sword_infinity			);
DECLARE_IPROG_FUN_WEAR( wear_prog_sword_infinity		);
DECLARE_IPROG_FUN_REMOVE( remove_prog_sword_infinity		);
DECLARE_IPROG_FUN_ENTRY( entry_prog_sword_infinity		);
DECLARE_IPROG_FUN_SPEECH( speech_prog_sword_infinity		);
DECLARE_IPROG_FUN_GET( get_prog_sword_infinity			);
// End Obsidian Palace

//Necromancer Lich Phylacteries
DECLARE_IPROG_FUN_INVOKE( invoke_prog_black_sapphire		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_black_opal		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_soapstone			);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_bloodstone		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_black_pearl		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_diamond			);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_pale_jade			);	
DECLARE_IPROG_FUN_INVOKE( invoke_prog_fire_opal			);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_violet_garnet		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_spherical_malachite	);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_sunstone			);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_zircon			);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_striped_glass		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_basalt			);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_star_diopside		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_rounded_jade		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_malachite_disc		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_pale_aquamarine		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_granite			);	
DECLARE_IPROG_FUN_INVOKE( invoke_prog_amethyst			);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_milky_quartz		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_tauroidal_beryl		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_something_oily		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_angled_jasper		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_jade			);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_cats_eye			);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_cats_eye_agate		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_faceted_jasper		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_rainbow_moonstone		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_black_diamond		);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_forest_green_emerald	);
//End Necromancer Phylacteries

DECLARE_IPROG_FUN_INVOKE( invoke_prog_charm_ivory_knowledge	);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_charm_earthen_vitality	);
DECLARE_IPROG_FUN_INVOKE( invoke_prog_charm_dragonbone_strength	);
const   struct  improg_type   iprog_table[] =
{   
    	{   	"remove_prog",  "remove_prog_crystal_tiara",      	remove_prog_crystal_tiara     	},
    	{   	"wear_prog",    "wear_prog_crystal_tiara",        	wear_prog_crystal_tiara       	},
    	{ 	"fight_prog",	"fight_prog_golden_weapon",	  	fight_prog_golden_weapon	},
    	{   	"remove_prog",  "remove_prog_ring_wizardry",      	remove_prog_ring_wizardry     	},
    	{   	"wear_prog",    "wear_prog_ring_wizardry",        	wear_prog_ring_wizardry       	},
    	{ 	"invoke_prog",	"invoke_prog_tattoo_detlef",	  	invoke_prog_tattoo_detlef	},
    	{   	"fight_prog",	"fight_prog_ranger_staff",	  	fight_prog_ranger_staff   	},
    	{   	"remove_prog",  "remove_prog_gauntlets_striking", 	remove_prog_gauntlets_striking	},
    	{   	"wear_prog",	"wear_prog_gauntlets_striking",   	wear_prog_gauntlets_striking  	},
	{	"fight_prog",	"fight_prog_gauntlets_striking",	fight_prog_gauntlets_striking	},
    	{	"fight_prog",	"fight_prog_etched_pendant",		fight_prog_etched_pendant	},
    	{   	"give_prog",    "give_prog_cabal_item",           	give_prog_cabal_item          	},
    	{	"fight_prog",	"fight_prog_cure_critical",		fight_prog_cure_critical	},
    	{	"fight_prog",	"fight_prog_training_pendant",		fight_prog_training_pendant	},
    	{	"fight_prog",	"fight_prog_arcana_sword",		fight_prog_arcana_sword		},
    	{   	"fight_prog",   "fight_prog_hair_bracelet",             fight_prog_hair_bracelet       	},
    	{   	"fight_prog",   "fight_prog_blood_bracelet",            fight_prog_blood_bracelet      	},
	{	"fight_prog",	"fight_prog_shredded_wings",		fight_prog_shredded_wings	},
	{	"invoke_prog",	"invoke_prog_deathwalk_statue",		invoke_prog_deathwalk_statue	},
	{	"fight_prog",	"fight_prog_seraphim_axe",		fight_prog_seraphim_axe		},
	{	"fight_prog",	"fight_prog_orb_dominion",		fight_prog_orb_dominion		},
	{	"fight_prog",	"fight_prog_tattoo_kat",		fight_prog_tattoo_kat		},
	{	"get_prog",	"get_prog_palace_key_one",		get_prog_palace_key_one		},
        {       "get_prog",     "get_prog_palace_key_two",              get_prog_palace_key_two         },
        {       "get_prog",     "get_prog_palace_key_three",		get_prog_palace_key_three	},
        {       "get_prog",     "get_prog_palace_key_four",		get_prog_palace_key_four	},
        {       "get_prog",     "get_prog_palace_key_five",		get_prog_palace_key_five	},
        {       "get_prog",     "get_prog_palace_key_six",              get_prog_palace_key_six         },
	{	"fight_prog",	"fight_prog_sword_baalzebub",		fight_prog_sword_baalzebub	},
	{	"fight_prog",	"fight_prog_sword_belal",		fight_prog_sword_belal		},
	{	"fight_prog",	"fight_prog_sword_prince",		fight_prog_sword_prince		},
	{	"fight_prog",	"fight_prog_crown_prince",		fight_prog_crown_prince		},
	{	"speech_prog",	"speech_prog_sword_prince",		speech_prog_sword_prince	},
	{	"speech_prog",	"speech_prog_crown_prince",		speech_prog_crown_prince	},
	{	"fight_prog",	"fight_prog_gloves_fortitude",		fight_prog_gloves_fortitude	},
	{	"fight_prog",	"fight_prog_crystal_talon",		fight_prog_crystal_talon	},
	{	"fight_prog",	"fight_prog_belt_fire_demon",		fight_prog_belt_fire_demon	},
	{	"fight_prog",	"fight_prog_bracer_diamond",		fight_prog_bracer_diamond	},
	{	"fight_prog",	"fight_prog_devil_shield",		fight_prog_devil_shield		},
	{	"fight_prog",	"fight_prog_sword_infinity",		fight_prog_sword_infinity	},
	{	"death_prog",	"death_prog_sword_infinity",		death_prog_sword_infinity	},
	{	"pulse_prog",	"pulse_prog_sword_infinity",		pulse_prog_sword_infinity	},
	{	"sac_prog",	"sac_prog_sword_infinity",		sac_prog_sword_infinity		},
	{	"wear_prog",	"wear_prog_sword_infinity",		wear_prog_sword_infinity	},
	{	"remove_prog",	"remove_prog_sword_infinity",		remove_prog_sword_infinity	},
	{	"entry_prog",	"entry_prog_sword_infinity",		entry_prog_sword_infinity	},
	{	"speech_prog",	"speech_prog_sword_infinity",		speech_prog_sword_infinity	},
 	{	"get_prog",	"get_prog_sword_infinity",		get_prog_sword_infinity		},
	{	"invoke_prog",	"invoke_prog_tattoo_daed",		invoke_prog_tattoo_daed		},
	{	"fight_prog",	"fight_prog_tattoo_daed",		fight_prog_tattoo_daed		},
	{	"fight_prog",	"fight_prog_earthsplitter",		fight_prog_earthsplitter	},
	{	"fight_prog",	"fight_prog_dispel_shield",		fight_prog_dispel_shield	},
	{	"invoke_prog",	"invoke_prog_tattoo_ormthrak",		invoke_prog_tattoo_ormthrak	},
	{	"fight_prog",	"fight_prog_tattoo_ormthrak",		fight_prog_tattoo_ormthrak	},
	{	"speech_prog",	"speech_prog_jade_orb",			speech_prog_jade_orb		},
	{	"wear_prog",	"wear_prog_stone_skin",			wear_prog_stone_skin		},
	{	"remove_prog",	"remove_prog_stone_skin",		remove_prog_stone_skin		},
	{	"wear_prog",	"wear_prog_shield",			wear_prog_shield		},
	{	"remove_prog",	"remove_prog_shield",			remove_prog_shield		},
	{	"wear_prog",	"wear_prog_aura",			wear_prog_aura			},
	{	"remove_prog",	"remove_prog_aura",			remove_prog_aura		},
	{	"fight_prog",	"fight_prog_tattoo_zornath",		fight_prog_tattoo_zornath	},
	{	"wear_prog",	"wear_prog_anti_rager",			wear_prog_anti_rager		},
	{	"invoke_prog",	"invoke_prog_black_sapphire",		invoke_prog_black_sapphire	},
	{	"invoke_prog",	"invoke_prog_black_opal",		invoke_prog_black_opal		},
	{	"invoke_prog",	"invoke_prog_soapstone",		invoke_prog_soapstone		},
	{	"invoke_prog",	"invoke_prog_bloodstone",		invoke_prog_bloodstone		},
	{	"invoke_prog",	"invoke_prog_black_pearl",		invoke_prog_black_pearl		},
	{	"invoke_prog",	"invoke_prog_diamond",			invoke_prog_diamond		},
	{	"invoke_prog",	"invoke_prog_pale_jade",		invoke_prog_pale_jade		},
	{	"invoke_prog",	"invoke_prog_fire_opal",		invoke_prog_fire_opal		},
	{	"invoke_prog",	"invoke_prog_violet_garnet",		invoke_prog_violet_garnet	},
	{	"invoke_prog",	"invoke_prog_spherical_malachite",	invoke_prog_spherical_malachite	},
	{	"invoke_prog",	"invoke_prog_sunstone",			invoke_prog_sunstone		},
	{	"invoke_prog",	"invoke_prog_zircon",			invoke_prog_zircon		},
	{	"invoke_prog",	"invoke_prog_striped_glass",		invoke_prog_striped_glass	},
	{	"invoke_prog",	"invoke_prog_basalt",			invoke_prog_basalt		},
	{	"invoke_prog",	"invoke_prog_star_diopside",		invoke_prog_star_diopside	},
	{	"invoke_prog",	"invoke_prog_rounded_jade",		invoke_prog_rounded_jade	},
	{	"invoke_prog",	"invoke_prog_malachite_disc",		invoke_prog_malachite_disc	},
	{	"invoke_prog",	"invoke_prog_pale_aquamarine",		invoke_prog_pale_aquamarine	},
	{	"invoke_prog",	"invoke_prog_granite",			invoke_prog_granite		},
	{	"invoke_prog",	"invoke_prog_amethyst",			invoke_prog_amethyst		},
	{	"invoke_prog",	"invoke_prog_milky_quartz",		invoke_prog_milky_quartz	},
	{	"invoke_prog",	"invoke_prog_tauroidal_beryl",		invoke_prog_tauroidal_beryl	},
	{	"invoke_prog",	"invoke_prog_something_oily",		invoke_prog_something_oily	},
	{	"invoke_prog",	"invoke_prog_angled_jasper",		invoke_prog_angled_jasper	},
	{	"invoke_prog",	"invoke_prog_jade",			invoke_prog_jade		},
	{	"invoke_prog",	"invoke_prog_cats_eye",			invoke_prog_cats_eye		},
	{	"invoke_prog", 	"invoke_prog_cats_eye_agate",		invoke_prog_cats_eye_agate	},
	{	"invoke_prog",	"invoke_prog_faceted_jasper",		invoke_prog_faceted_jasper	},
	{	"invoke_prog",	"invoke_prog_rainbow_moonstone",	invoke_prog_rainbow_moonstone	},
	{	"invoke_prog",	"invoke_prog_black_diamond",		invoke_prog_black_diamond	},
	{	"invoke_prog",	"invoke_prog_forest_green_emerald",	invoke_prog_forest_green_emerald},
	
	{	"wear_prog",	"wear_prog_quest_reward_warrior_boots", wear_prog_quest_reward_warrior_boots},
	{	"wear_prog",	"wear_prog_quest_reward_warrior_weapon", wear_prog_quest_reward_warrior_weapon},
	{	"remove_prog",	"remove_prog_quest_reward_warrior_boots", remove_prog_quest_reward_warrior_boots},
	{	"remove_prog",	"remove_prog_quest_reward_warrior_weapon", remove_prog_quest_reward_warrior_weapon},
	{	"remove_prog",	"remove_prog_quest_reward_warrior_plate", remove_prog_quest_reward_warrior_plate},
	{	"remove_prog",	"remove_prog_quest_reward_warrior_gauntlets", remove_prog_quest_reward_warrior_gauntlets},
	{	"speech_prog",	"speech_prog_quest_reward_warrior_weapon", speech_prog_quest_reward_warrior_weapon},
	{	"pulse_prog",	"pulse_prog_quest_reward_warrior_plate",pulse_prog_quest_reward_warrior_plate},
	{	"pulse_prog",	"pulse_prog_quest_reward_warrior_gauntlets",pulse_prog_quest_reward_warrior_gauntlets},
	
	{	"wear_prog",	"wear_prog_quest_reward_channeler_helm", wear_prog_quest_reward_channeler_helm},
	{	"wear_prog",	"wear_prog_quest_reward_channeler_robe",wear_prog_quest_reward_channeler_robe},
	{	"wear_prog",	"wear_prog_quest_reward_channeler_neck",wear_prog_quest_reward_channeler_neck},
	{	"remove_prog",	"remove_prog_quest_reward_channeler_helm",remove_prog_quest_reward_channeler_helm},
	{	"remove_prog",	"remove_prog_quest_reward_channeler_robe",remove_prog_quest_reward_channeler_robe},
	{	"remove_prog",	"remove_prog_quest_reward_channeler_neck",remove_prog_quest_reward_channeler_neck},
	{	"invoke_prog", "invoke_prog_quest_reward_channeler_ankh", invoke_prog_quest_reward_channeler_ankh},
	
	{	"wear_prog",	"wear_prog_quest_reward_thief_belt", wear_prog_quest_reward_thief_belt},
	{	"remove_prog",	"remove_prog_quest_reward_thief_belt", remove_prog_quest_reward_thief_belt},
	{	"wear_prog",	"wear_prog_quest_reward_thief_boots", wear_prog_quest_reward_thief_boots},
	{	"remove_prog",	"remove_prog_quest_reward_thief_boots", remove_prog_quest_reward_thief_boots},
	{	"wear_prog",	"wear_prog_quest_reward_thief_robe", wear_prog_quest_reward_thief_robe},
	{	"pulse_prog",	"pulse_prog_quest_reward_thief_robe",pulse_prog_quest_reward_thief_robe},
	{	"remove_prog",	"remove_prog_quest_reward_thief_robe", remove_prog_quest_reward_thief_robe},
	
	{	"wear_prog",	"wear_prog_quest_reward_healer_ring", wear_prog_quest_reward_healer_ring},
	{	"remove_prog",	"remove_prog_quest_reward_healer_ring", remove_prog_quest_reward_healer_ring},
	{	"wear_prog",	"wear_prog_quest_reward_healer_bracelet", wear_prog_quest_reward_healer_bracelet},
	{	"remove_prog",	"remove_prog_quest_reward_healer_bracelet", remove_prog_quest_reward_healer_bracelet},
	{	"wear_prog",	"wear_prog_quest_reward_healer_sleeves", wear_prog_quest_reward_healer_sleeves},
	{	"remove_prog",	"remove_prog_quest_reward_healer_sleeves", remove_prog_quest_reward_healer_sleeves},
	{   "invoke_prog",   "invoke_prog_quest_reward_healer_statue", invoke_prog_quest_reward_healer_statue},
	
	{	"wear_prog",	"wear_prog_quest_reward_elementalist_neck", wear_prog_quest_reward_elementalist_neck},
	{	"remove_prog",	"remove_prog_quest_reward_elementalist_neck", remove_prog_quest_reward_elementalist_neck},
	{	"wear_prog",	"wear_prog_quest_reward_elementalist_belt", wear_prog_quest_reward_elementalist_belt},
	{	"remove_prog",	"remove_prog_quest_reward_elementalist_belt", remove_prog_quest_reward_elementalist_belt},
	{	"wear_prog",	"wear_prog_quest_reward_elementalist_helm", wear_prog_quest_reward_elementalist_helm},
	{	"remove_prog",	"remove_prog_quest_reward_elementalist_helm", remove_prog_quest_reward_elementalist_helm},
	
	{	"wear_prog",	"wear_prog_rune_dagger",		wear_prog_rune_dagger		},
	{	"remove_prog",	"remove_prog_rune_dagger",		remove_prog_rune_dagger		},
	{	"fight_prog",	"fight_prog_cloak_embrace",		fight_prog_cloak_embrace	},
	{	"remove_prog",	"remove_prog_cloak_displacement",	remove_prog_cloak_displacement	},
	{	"wear_prog",	"wear_prog_cloak_displacement",		wear_prog_cloak_displacement	},
	{	"fight_prog",	"fight_prog_tattoo_tracerah",		fight_prog_tattoo_tracerah	},

	{	"invoke_prog",	"invoke_prog_charm_ivory_knowledge",	invoke_prog_charm_ivory_knowledge},
	{	"invoke_prog",	"invoke_prog_charm_earthen_vitality",	invoke_prog_charm_earthen_vitality},
	{	"invoke_prog",	"invoke_prog_charm_dragonbone_strength",	invoke_prog_charm_dragonbone_strength},
    	{   	NULL,		NULL,					NULL		        	},
};

void iprog_set(OBJ_INDEX_DATA *obj,const char *progtype, const char *name)
{
	int i;
	for(i = 0; iprog_table[i].name != NULL; i++)
		if(!str_cmp(iprog_table[i].name, name))
			break;
	if(!iprog_table[i].name)
		return bug("Load_improgs: 'I': Function not found for vnum %d",obj->vnum);

	if (!str_cmp(progtype, "wear_prog"))
	{
		obj->iprogs->wear_prog = iprog_table[i].function;
		free_string(obj->iprogs->wear_name);
		obj->iprogs->wear_name = str_dup(name);
		SET_BIT(obj->progtypes, IPROG_WEAR);
		return;
	}
	if (!str_cmp(progtype, "remove_prog"))
	{
		obj->iprogs->remove_prog = iprog_table[i].function;
		free_string(obj->iprogs->remove_name);
		obj->iprogs->remove_name = str_dup(name);
		SET_BIT(obj->progtypes, IPROG_REMOVE);
		return;
	}
	if (!str_cmp(progtype, "get_prog"))
	{
		obj->iprogs->get_prog = iprog_table[i].function;
		free_string(obj->iprogs->get_name);
		obj->iprogs->get_name = str_dup(name);
		SET_BIT(obj->progtypes, IPROG_GET);
		return;
	}
	if (!str_cmp(progtype, "drop_prog"))
	{
		obj->iprogs->drop_prog = iprog_table[i].function;
		free_string(obj->iprogs->drop_name);
		obj->iprogs->drop_name = str_dup(name);
		SET_BIT(obj->progtypes, IPROG_DROP);
		return;
	}
	if (!str_cmp(progtype, "sac_prog")) 
	{
		obj->iprogs->sac_prog = iprog_table[i].function;
		free_string(obj->iprogs->sac_name);
		obj->iprogs->sac_name = str_dup(name);
		SET_BIT(obj->progtypes, IPROG_SAC);
		return;
	}
	if (!str_cmp(progtype, "entry_prog"))
	{
		obj->iprogs->entry_prog = iprog_table[i].function;
		free_string(obj->iprogs->entry_name);
		obj->iprogs->entry_name = str_dup(name);
		SET_BIT(obj->progtypes, IPROG_ENTRY);
		return;
	}
	if (!str_cmp(progtype, "give_prog"))
	{
		obj->iprogs->give_prog = iprog_table[i].function;
		free_string(obj->iprogs->give_name);
		obj->iprogs->give_name = str_dup(name);
		SET_BIT(obj->progtypes, IPROG_GIVE);
		return;
	}
	if (!str_cmp(progtype, "greet_prog"))
	{
		obj->iprogs->greet_prog = iprog_table[i].function;
		free_string(obj->iprogs->greet_name);
		obj->iprogs->greet_name = str_dup(name);
		SET_BIT(obj->progtypes, IPROG_GREET);
		return;
	}
	if (!str_cmp(progtype, "fight_prog"))
	{
		obj->iprogs->fight_prog = iprog_table[i].function;
		free_string(obj->iprogs->fight_name);
		obj->iprogs->fight_name = str_dup(name);
		SET_BIT(obj->progtypes, IPROG_FIGHT);
		return;
	}
	if (!str_cmp(progtype, "death_prog")) /* returning TRUE prevents death */
	{
		obj->iprogs->death_prog = iprog_table[i].function;
		free_string(obj->iprogs->death_name);
		obj->iprogs->death_name = str_dup(name);
		SET_BIT(obj->progtypes, IPROG_DEATH);
		return;
	}
	if (!str_cmp(progtype, "speech_prog"))
	{
		obj->iprogs->speech_prog = iprog_table[i].function;
		free_string(obj->iprogs->speech_name);
		obj->iprogs->speech_name = str_dup(name);
		SET_BIT(obj->progtypes, IPROG_SPEECH);
		return;
	}
	if (!str_cmp(progtype, "pulse_prog"))
	{
		obj->iprogs->pulse_prog = iprog_table[i].function;
		free_string(obj->iprogs->pulse_name);
		obj->iprogs->pulse_name = str_dup(name);
		SET_BIT(obj->progtypes, IPROG_PULSE);
		return;
	}
	if (!str_cmp(progtype, "invoke_prog"))
	{
		obj->iprogs->invoke_prog = iprog_table[i].function;
		free_string(obj->iprogs->invoke_name);
		obj->iprogs->invoke_name = str_dup(name);
		SET_BIT(obj->progtypes, IPROG_INVOKE);
		return;
	}
	bug( "Load_improgs: 'I': invalid program type for vnum %d",obj->vnum);
	exit(1);
}

void do_invoke( CHAR_DATA *ch, char *argument ) 
{
	OBJ_DATA *obj;
	char arg[MAX_STRING_LENGTH];

	argument = one_argument(argument,arg);

	if (arg[0] == '\0')
	{
		send_to_char("Invoke what?\n\r",ch);
		return;
	}

	if ((obj = get_obj_wear(ch,arg)) == NULL)
	{
		send_to_char("You're not wearing that.\n\r",ch);
		return;
	}

	if (IS_SET(obj->progtypes,IPROG_INVOKE))
	{
		(obj->pIndexData->iprogs->invoke_prog) (obj,ch,argument);
	} else {
		send_to_char("You can't invoke that.\n\r",ch);
		return;
	}
}

bool give_prog_cabal_item(OBJ_DATA *obj, CHAR_DATA *from, CHAR_DATA *to)
{
        char buf[MSL];
	//OBJ_DATA *key;

        if (IS_IMMORTAL(from))
                return TRUE;
        if(!IS_CABAL_GUARD(to))
                return FALSE;
        if(from->cabal!=to->cabal && !IS_IMMORTAL(from))
        {
                do_say(to,"I want nothing from the likes of you.");
                return TRUE;
        }
        cabal_shudder(obj->pIndexData->cabal, TRUE);
        sprintf(buf,"%s has given me %s.",from->original_name,obj->short_descr);
        do_cb(to,buf);
        obj_from_char(obj);
        obj_to_char(obj, to);
	//key = create_object(get_obj_index(cabal_table[obj->pIndexData->cabal].key_vnum), 0 );
	//obj_to_char(key,to);
	//interpret(to, "unlock altar");
	//interpret(to, "open altar");
	//sprintf(buf, "put '%s' altar", obj->name);
	//interpret(to, buf);
	//interpret(to, "close altar");
	//interpret(to, "lock altar");
	//extract_obj(key);
        save_cabal_items();
        return TRUE;
}

void wear_prog_crystal_tiara(OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("$n is surrounded by a blue aura.",ch,0,0,TO_ROOM);
	send_to_char("You are surrounded by a blue aura.\n\r",ch);
}

void remove_prog_crystal_tiara(OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("The blue aura around $n fades.",ch,0,0,TO_ROOM);
	send_to_char("The blue aura around you fades.\n\r",ch);
}

void fight_prog_golden_weapon(OBJ_DATA *obj, CHAR_DATA *ch)
{
	int rnum;
	if (is_worn(obj) && number_percent() < 8)
	{
	act("$n's weapon glows brilliant gold.",ch,obj,0,TO_ROOM);
	act("Your body warms as your weapon heals you!",ch,obj,0,TO_CHAR);
	ch->hit = UMIN( ch->hit + 75, ch->max_hit );
	return;
	}
	if (!IS_NPC(ch) && ch->pcdata->induct == CABAL_LEADER && 
		(get_eq_char(ch,WEAR_WIELD) == obj || get_eq_char(ch,WEAR_DUAL_WIELD)==obj) && number_percent()<4)
	{
		act("$n's eyes blaze with the divine power granted to $m!",ch,0,ch->fighting,TO_NOTVICT);
		act("$n's eyes blaze with the divine power granted to $m!",ch,0,ch->fighting,TO_VICT);
		act("You feel a surge of power as your divine might pulses through your body!",ch,0,ch->fighting,TO_CHAR);
		rnum=number_range(0,2);
	    	if(rnum==0)
			obj_cast_spell(skill_lookup("wrath"),51,ch,ch->fighting,obj);
	    	if(rnum==1)
			obj_cast_spell(skill_lookup("ray of truth"),51,ch,ch->fighting,obj);
	    	if(rnum==2)
			obj_cast_spell(skill_lookup("divine aid"),40,ch,ch->fighting,obj);
	}
	return;
}

void wear_prog_ring_wizardry(OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("$n looks more attuned with to forces of magic.",ch,0,0,TO_ROOM);
        send_to_char("You attuned to the forces of magic.\n\r",ch);
}

void remove_prog_ring_wizardry(OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("$n looks less attuned with to forces of magic.",ch,0,0,TO_ROOM);
	send_to_char("You feel less attuned to the forces of magic.\n\r",ch);
}

void invoke_prog_tattoo_detlef(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;

	if(!is_worn(obj))
		return;

        if (is_affected(ch,gsn_phat_blunt))
            	return send_to_char("You cannot smoke phat blunt too much!\n\r",ch);

      	act("$n's $p glows blue.",ch,obj,0,TO_ROOM);
      	act("Your $p glows blue.",ch,obj,0,TO_CHAR);

	ch->hit += (ch->level*2) * 8;
      	ch->hit = UMIN(ch->hit,ch->max_hit);
      	send_to_char("You feel MUCH better.\n\r",ch);
      	act("$n looks much better.",ch,NULL,NULL,TO_ROOM);

	if (check_dispel(ch->level+3,ch,skill_lookup("blindness")))
		act("$n is no longer blinded.",ch,NULL,NULL,TO_ROOM);

	if (check_dispel(ch->level+3,ch,skill_lookup("curse")))
		act("$n is no longer cursed.",ch,NULL,NULL,TO_ROOM);

	if (check_dispel(ch->level+3,ch,skill_lookup("faerie fire")))
		act("$n's pink outline fades.",ch,NULL,NULL,TO_ROOM);

	if (check_dispel(ch->level+3,ch,skill_lookup("slow")))
		act("$n is no longer moving so slowly.",ch,NULL,NULL,TO_ROOM);

	if (check_dispel(ch->level+3,ch,skill_lookup("weaken")))
		act("$n looks stronger.",ch,NULL,NULL,TO_ROOM);

	if (check_dispel(ch->level+3,ch,skill_lookup("poison")))
		act("$n looks less ill.",ch,NULL,NULL,TO_ROOM);

	if (check_dispel(ch->level+3,ch,skill_lookup("plague")))
		act("$n's sores vanish.",ch,NULL,NULL,TO_ROOM);

	if (check_dispel(ch->level+3,ch,skill_lookup("windwall")))
		act("$n's is no longer blinded by dust and dirt.",ch,NULL,NULL,TO_ROOM);

      	send_to_char("Your pulse races fiercely as you burn with raging fury!\n\r",ch);
      	act("$n gets a furious look in $s eyes as $e burns with raging fury!",ch,NULL,NULL,TO_ROOM);

      	init_affect(&af);
      	af.where        = TO_AFFECTS;
	af.aftype       = AFT_SKILL;
	af.type		= gsn_phat_blunt;
      	af.level        = ch->level;
      	af.duration     = ch->level/4;
      	af.modifier     = ch->level/3;
      	af.bitvector    = 0;
      	af.location     = APPLY_HITROLL;
      	affect_to_char(ch,&af);
      	af.location     = APPLY_DAMROLL;
      	affect_to_char(ch,&af);
      	return;
}

void fight_prog_ranger_staff(OBJ_DATA *obj, CHAR_DATA *ch)
{
	if (get_eq_char(ch,WEAR_WIELD) == obj && number_percent() < 8)
	{
		send_to_char("Your ranger's staff glows blue!\n\r",ch);
		act("$n's ranger's staff glows blue!",ch,NULL,NULL,TO_ROOM);
		ch->hit = UMIN( ch->hit + 75, ch->max_hit );
	}
 	return;
}

void fight_prog_gauntlets_striking(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim;
	int numhits;
	int i;

	numhits = number_range(2,6);

	if ((victim = ch->fighting) == NULL)
	{
		return;
	}

	if (ch->cabal == CABAL_RAGER) return;

	if (number_percent() < 5)
	{
		send_to_char("Your hands seem to take on a life of their own unleashing a series of deadly blows!\n\r",ch);
		act("$n's hands seem to come to life, unleashing a series of deadly blows!",ch,obj,NULL,TO_ROOM);
		for ( i=0; i < numhits; i++ )
		{
			if (ch->in_room != victim->in_room) break;			damage_new(ch,victim,number_range(40,70),TYPE_UNDEFINED,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"brutal assault"); 
				continue;
		}
	}
	return;
}

void wear_prog_gauntlets_striking(OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("$n looks a lot meaner.",ch,0,0,TO_ROOM);
	send_to_char("You feel as if you can strike with far deadlier power than before!\n\r",ch);
}

void remove_prog_gauntlets_striking(OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("$n doesn't look quite so mean anymore.",ch,0,0,TO_ROOM);
	send_to_char("Your hands don't feel quite so powerful anymore.\n\r",ch);
}

void fight_prog_etched_pendant(OBJ_DATA *obj, CHAR_DATA *ch)
{
	if(!is_worn(obj))
		return;
	if (ch->cabal == CABAL_RAGER)
		return;
	if(number_percent()>94)
	{
		act("$p writhes upon your chest.",ch,obj,0,TO_CHAR);
		act("$p writhes upon $n's chest.",ch,obj,0,TO_ROOM);
		ch->hit = UMIN(ch->hit + 100, ch->max_hit);
	}
}

void fight_prog_cure_critical(OBJ_DATA *obj, CHAR_DATA *ch)
{
    if (ch->cabal == CABAL_RAGER)
	return;
    if (number_percent() < 8 && is_worn(obj))
        {   
        act("$n's $p glows bright blue!",ch,obj,0,TO_ROOM);
        act("$p glows bright blue!",ch,obj,0,TO_CHAR);
        obj_cast_spell(skill_lookup("cure critical"), obj->level, ch, ch, obj);
        }
}

void fight_prog_longsword_vengeance(OBJ_DATA *obj, CHAR_DATA *ch)
{

    if (is_worn(obj) && number_percent()<3 && ch->fighting!=NULL)
    {
        act("$n's $p shimmers with white light!",ch,obj,ch->fighting,TO_NOTVICT);
        act("You call down the wrath of the heavens upon $N!",ch,obj,ch->fighting,TO_CHAR);
        act("$n's $p shimmers with white light as it calls the wrath of the heavens upon you!",ch,obj,ch->fighting,TO_VICT);
        (*skill_table[skill_lookup("wrath")].spell_fun) ( skill_lookup("wrath"), 60, ch, ch->fighting, TAR_CHAR_OFFENSIVE );
    }
    return;
}

void fight_prog_arcana_sword(OBJ_DATA *obj, CHAR_DATA *ch)
{

    if (is_worn(obj) && number_percent()<7 && ch->fighting!=NULL)
    {
        (*skill_table[skill_lookup("faerie fire")].spell_fun) ( skill_lookup("faerie fire"), 56, ch, ch->fighting, TAR_CHAR_OFFENSIVE );
    }
    return;
}

void fight_prog_blood_bracelet(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim=ch->fighting;
	int sn_dispel;

	if (!is_worn(obj))
		return;

	if(!ch)
		return;

        if (IS_NPC(ch))
                return;

	if (!IS_GOOD(victim))
		return;

	if(number_percent() < 10) 
	{
		sn_dispel = skill_lookup("dispel good");

		act("The blood on $n's bracelet hisses and steams.",ch,0,0,TO_ROOM);
        	act("The blood on your bracelet hisses and steams.",ch,0,0,TO_CHAR);
		(*skill_table[sn_dispel].spell_fun) (sn_dispel, 60,ch,victim,TAR_CHAR_OFFENSIVE);

	}
}

void fight_prog_hair_bracelet(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim=ch->fighting;
	int sn_dispel;

	if (!is_worn(obj))
		return;

	if(!ch)
		return;

        if (IS_NPC(ch))
                return;

	if (!IS_EVIL(victim))
		return;

	if(number_percent() < 10) 
	{
		sn_dispel = skill_lookup("dispel evil");

        	act("The hair on $n's bracelet shimmers softly.",ch,0,0,TO_ROOM);
        	act("The hair on your bracelet shimmers softly.",ch,0,0,TO_CHAR);
		(*skill_table[sn_dispel].spell_fun) (sn_dispel, 60,ch,victim,TAR_CHAR_OFFENSIVE);

	}
}

void fight_prog_shredded_wings(OBJ_DATA *obj, CHAR_DATA *ch)
{
    int dam;
    CHAR_DATA *victim=ch->fighting;

    if((!ch) || (!victim))
	return;

    dam = dice(ch->level,8);

    if (ch->cabal == CABAL_RAGER)
	return;

    if (is_worn(obj) && number_percent()<8 && ch->fighting!=NULL)
    {
	act("The shredded wings on $n's back begin to beat with a brutal fury!",ch,0,0,TO_ROOM);
	act("The shredded wings on your back begin to beat with a brutal fury!",ch,0,0,TO_CHAR);
	damage_new(ch,victim,dam,TYPE_UNDEFINED,DAM_WIND,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"savage buffet");

	if (check_immune(victim, DAM_WIND) != IS_IMMUNE)
	    WAIT_STATE(victim, 1 * PULSE_VIOLENCE);
    }

    return;
}

void invoke_prog_deathwalk_statue(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
    AFFECT_DATA af;
	char buf[MSL];

    if (!is_worn(obj))
	return send_to_char("Nothing happens.\n\r", ch);

    if(str_cmp(argument,"protection") )
	return send_to_char("Nothing happens.\n\r", ch);

    if ( IS_AFFECTED(ch, AFF_SANCTUARY) )
	return send_to_char("Nothing happens.\n\r", ch);

	sprintf(buf,"reduces damage taken by %d%%",DAM_REDUX_SANC);
	
    init_affect(&af);
    af.where     = TO_AFFECTS;
    af.aftype    = AFT_COMMUNE;
    af.type      = gsn_sanctuary;
    af.level     = 51;
    af.duration  = 8;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.bitvector = AFF_SANCTUARY;
	af.affect_list_msg = str_dup(buf);
    affect_to_char( ch, &af );

    act( "$n is surrounded by a white aura.", ch, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a white aura.\n\r", ch );

    if (number_percent() < 25)
    {
	act( "$p explodes into fragments.", ch, obj, NULL, TO_ROOM );
	act( "$p explodes into fragments.", ch, obj, NULL, TO_CHAR );
	extract_obj(obj);
    }

    return;
}

void fight_prog_seraphim_axe(OBJ_DATA *obj, CHAR_DATA *ch)
{
    if (is_worn(obj) && number_percent()<7 && ch->fighting!=NULL)
    {
        (*skill_table[skill_lookup("concatenate")].spell_fun) ( skill_lookup("concatenate"), 60, ch, ch->fighting, TAR_CHAR_OFFENSIVE );
    }
    return;
}

void fight_prog_orb_dominion(OBJ_DATA *obj, CHAR_DATA *ch)
{
    if(!ch)
        return;

    if (number_percent() < 8 && is_worn(obj))
    {
        act("$n's $p glows bright blue!",ch,obj,0,TO_ROOM);
        act("$p glows bright blue!",ch,obj,0,TO_CHAR);
        obj_cast_spell(skill_lookup("cure critical"), obj->level, ch, ch, obj);
	return;
    }

    if (is_worn(obj) && number_percent()<30 && ch->fighting!=NULL)
    {
        act("$n bows $s head in agony and screams as $s weapon assaults $s mind!",ch,0,0,TO_ROOM);
        act("You bow your head in agony and scream as your weapon assaults your mind!",ch,0,0,TO_CHAR);
        damage_new(ch,ch,number_range(10,50),TYPE_UNDEFINED,DAM_TRUESTRIKE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"mental assault");
	WAIT_STATE(ch, PULSE_VIOLENCE);
    }

    return;
}

void invoke_prog_elemental_shard(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	char buf[MSL];

        if(!is_worn(obj))
                return;

	switch(number_range(0, 4))
	{
		case 0:
			sprintf(buf, "a shard of red crystal");
			obj->value[3] = attack_lookup("flamingbite");
			act( "$p becomes engulfed in fire!", ch, obj, NULL, TO_CHAR );
			break;
		case 1:
			sprintf(buf, "a shard of blue crystal");
			obj->value[3] = attack_lookup("freezingbite");
			act( "A frosty plume envelops $p!", ch, obj, NULL, TO_CHAR );
			break;	
		case 2:
			sprintf(buf, "a shard of yellow crystal");
			obj->value[3] = attack_lookup("shockingbite");
			act( "$p crackles with electricity!", ch, obj, NULL, TO_CHAR );
			break;	
		case 3:
			sprintf(buf, "a shard of orange crystal");
			obj->value[3] = attack_lookup("acidicbite");
			act( "An acidic rust forms around $p!", ch, obj, NULL, TO_CHAR );
			break;	
		case 4:
			sprintf(buf, "a shard of clear crystal");
			obj->value[3] = attack_lookup("rending");
			act( "A turbulent vortex surrounds $p!", ch, obj, NULL, TO_CHAR );
			break;	
		default: return;
	}

	free_string( obj->short_descr );
	obj->short_descr = str_dup( buf );
	WAIT_STATE(ch, PULSE_VIOLENCE * 6);
	return;
}
        
void fight_prog_tattoo_kat(OBJ_DATA *obj, CHAR_DATA *ch)
{        
	int dam;
	char buf[MSL];
	AFFECT_DATA af,af2,af3;
	CHAR_DATA *victim;
        if(!is_worn(obj) || !ch->fighting)
                return;
        victim = ch->fighting;
        if(number_percent() > 94)
	{
                do_yell(ch,"Maria!  Ayudame!");
		sprintf(buf,"{RYou feel a deep chill as %s calls to the dead.{x",ch->name);
		do_zecho(ch,buf);
		act("A pale banshee fades into existence and enters the body of $n.",ch,0,victim,TO_NOTVICT); 
		act("A pale banshee fades into existence and enters the body of $n.",ch,0,victim,TO_VICT);
		act("A pale banshee fades into existence and enters your body.",ch,0,victim,TO_CHAR);
		damage_new(ch,victim,number_range(100,150),TYPE_UNDEFINED,DAM_NEGATIVE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"death coil");
damage_new(ch,victim,number_range(100,150),TYPE_UNDEFINED,DAM_NEGATIVE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"death coil");
damage_new(ch,victim,number_range(100,150),TYPE_UNDEFINED,DAM_NEGATIVE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"death coil");
		act("$n's body seems more frail as the banshee leaves him.",ch,0,victim,TO_NOTVICT);
		act("$n's body seems more frail as the banshee leaves him.",ch,0,victim,TO_VICT);
		act("Your body feels more frail as the banshee leaves you.",ch,0,victim,TO_CHAR);
		ch->hit -= 50;
	}
                
        if(number_percent() > 96) {
                act("A wicked grin plays across $n's features as a knife appears in $s hand.",ch,0,0,TO_ROOM);
         	act("You feel a sinister force taking you over.",ch,0,0,TO_CHAR);
		dam = dice(number_range(ch->level,60),6);
		damage_new(ch,victim,dam,TYPE_UNDEFINED,DAM_PIERCE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"savage knifing");

		init_affect(&af);
		af.where = TO_AFFECTS;
		af.location = APPLY_STR;
		af.duration = ch->level/4;
		af.modifier = -7;
		af.bitvector = AFF_PLAGUE;
			af.type = gsn_plague;
		af.owner_name = str_dup(ch->original_name);
		af.level = ch->level;
		if(!is_affected(victim,gsn_plague))
			affect_to_char(victim,&af);
		init_affect(&af2);
		af2.where = TO_AFFECTS;
		af2.location = APPLY_STR;
		af2.duration = ch->level/4;
		af2.type = gsn_poison;
		af2.bitvector = AFF_POISON;
		af2.modifier = -5;
		af2.owner_name = str_dup(ch->original_name);
		af2.level = ch->level;
		if(!is_affected(victim,gsn_poison))
			affect_to_char(victim,&af2);
		init_affect(&af3);
		af3.where = TO_AFFECTS;
		af3.location = APPLY_SAVES;
		af3.modifier = ch->level/2;
		af3.owner_name = str_dup(ch->original_name);
		af3.duration = ch->level/4;
		af3.bitvector = AFF_CURSE;
		af3.type = gsn_haunting;
		af3.level = ch->level;
		if(!is_affected(victim,gsn_haunting))
			affect_to_char(victim,&af3);

		return send_to_char("You lurch forward and feel very sick.\n\r",victim);
	}

	return;
}

void get_prog_palace_key_one(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim;
	CHAR_DATA *v_next;
        if(ch->in_room->vnum==28009)
        {
		act("As Be'lal falls to the ground, his corpse a lifeless husk, another platform appears, beckoning you onwards.",ch,0,0,TO_ALL);
		for(victim = ch->in_room->people; victim!=NULL; victim=v_next)
		{
			v_next=victim->next_in_room;
			send_to_char("You scramble onto the platform, and are sped rapidly to...\n\r",victim);
			act("$n scrambles onto the platform.",victim,0,0,TO_ROOM);
			char_from_room(victim);
                  char_to_room(victim,get_room_index(28010));
                  do_look(victim,"auto");		
		}		
        }	
        extract_obj(obj);
}

void get_prog_palace_key_two(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim;
	CHAR_DATA *v_next;
        if(ch->in_room->vnum==28010)
        {
		act("As Ishmael falls to the ground, his corpse a lifeless husk, another platform appears, beckoning you onwards.",ch,0,0,TO_ALL);
		for(victim = ch->in_room->people; victim!=NULL; victim=v_next)
		{
			v_next=victim->next_in_room;
			send_to_char("You scramble onto the platform, and are sped rapidly to...\n\r",victim);
			act("$n scrambles onto the platform.",victim,0,0,TO_ROOM);
			char_from_room(victim);
                  char_to_room(victim,get_room_index(28011));
                  do_look(victim,"auto");		
		}		
        }	
        extract_obj(obj);
}

void get_prog_palace_key_three(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim;
	CHAR_DATA *v_next;
        if(ch->in_room->vnum==28011)
        {
		act("As Agamemnon falls to the ground, his corpse a lifeless husk, another platform appears, beckoning you onwards.",ch,0,0,TO_ALL);
		for(victim = ch->in_room->people; victim!=NULL; victim=v_next)
		{
			v_next=victim->next_in_room;
			send_to_char("You scramble onto the platform, and are sped rapidly to...\n\r",victim);
			act("$n scrambles onto the platform.",victim,0,0,TO_ROOM);
			char_from_room(victim);
                  char_to_room(victim,get_room_index(28012));
                  do_look(victim,"auto");		
		}		
        }	
        extract_obj(obj);
}

void get_prog_palace_key_four(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim;
	CHAR_DATA *v_next;
        if(ch->in_room->vnum==28012)
        {
		act("As Mephistopheles falls to the ground, his corpse a lifeless husk, another platform appears, beckoning you onwards.",ch,0,0,TO_ALL);
		for(victim = ch->in_room->people; victim!=NULL; victim=v_next)
		{
			v_next=victim->next_in_room;
			send_to_char("You scramble onto the platform, and are sped rapidly to...\n\r",victim);
			act("$n scrambles onto the platform.",victim,0,0,TO_ROOM);
			char_from_room(victim);
                  char_to_room(victim,get_room_index(28013));
                  do_look(victim,"auto");		
		}		
        }	
        extract_obj(obj);
}

void get_prog_palace_key_five(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim;
	CHAR_DATA *v_next;
        if(ch->in_room->vnum==28013)
        {
		act("As Baalzebub falls to the ground, his corpse a lifeless husk, you travel upwards, escaping this hell.",ch,0,0,TO_ALL);
		for(victim = ch->in_room->people; victim!=NULL; victim=v_next)
		{
			v_next=victim->next_in_room;
			send_to_char("You scramble onto the platform, and are sped rapidly to...\n\r",victim);
			act("$n scrambles onto the platform.",victim,0,0,TO_ROOM);
			char_from_room(victim);
                  char_to_room(victim,get_room_index(28014));
                  do_look(victim,"auto");		
		}		
        }	
	extract_obj(obj);
}


void get_prog_palace_key_six(OBJ_DATA *obj, CHAR_DATA *ch)
{
        CHAR_DATA *victim;
        CHAR_DATA *v_next;
        if(ch->in_room->vnum==28014)
        {
                act("As Necruvian falls to the ground, his corpse a lifeless husk, you step through the shimmering portal, downwards to...",ch,0,0,TO_ALL);
                for(victim = ch->in_room->people; victim!=NULL; victim=v_next)
                {
                        v_next=victim->next_in_room;
                        send_to_char("You scramble onto the platform, and are sped rapidly to...\n\r",victim);
                        act("$n scrambles onto the platform.",victim,0,0,TO_ROOM);
                        char_from_room(victim);
                  char_to_room(victim,get_room_index(28015));
                  do_look(victim,"auto");
                }
        }
        extract_obj(obj);
}

void fight_prog_sword_baalzebub(OBJ_DATA *obj, CHAR_DATA *ch)
{
	if (is_worn(obj) && number_percent()<16 && ch->fighting!=NULL)
	{
		act("The unholy sword of Baalzebub pulses with dark energy as it siphons the life from $N!",ch,0,ch->fighting,TO_NOTVICT);
		act("The unholy sword of Baalzebub pulses with dark energy as it siphons the life from $N!",ch,0,ch->fighting,TO_CHAR);
		act("The unholy sword of Baalzebub pulses with dark energy as it siphons the life from you!",ch,0,ch->fighting,TO_VICT);
		(*skill_table[skill_lookup("vampiric touch")].spell_fun) ( skill_lookup("vampiric touch"), 56, ch, ch->fighting, TAR_CHAR_OFFENSIVE );
	}
}

void fight_prog_sword_belal(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim = ch->fighting;
	if (is_worn(obj) && number_percent()<11 && victim)
	{
	 	act("$n plunges $p deep into your side!",ch,obj,victim,TO_VICT);
	 	act("$n plunges $p deep into $N's side!",ch,obj,victim,TO_NOTVICT);
		act("You plunge $p deep into $N's side!",ch,obj,victim,TO_CHAR);
		damage_new(ch,victim,number_range(80,140),TYPE_UNDEFINED,DAM_PIERCE,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"jab");
	}
}

void fight_prog_sword_prince(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim = ch->fighting;
	if (is_worn(obj) && number_percent()<9 && victim!=NULL)
	{
		act("$p glows with a crimson light and engulfs $N in a blaze of hellfire!",ch,obj,victim,TO_NOTVICT);
		act("$p glows with a crimson light and engulfs $N in a blaze of hellfire!",ch,obj,victim,TO_CHAR);
		act("$p glows with a crimson light and engulfs you in a blaze of hellfire!",ch,obj,victim,TO_VICT);
		(*skill_table[skill_lookup("incinerate")].spell_fun) (skill_lookup("incinerate"), 50, ch, victim, TAR_CHAR_OFFENSIVE);
	}
}


void fight_prog_crown_prince(OBJ_DATA *obj, CHAR_DATA *ch)
{
    	int infchance;
	CHAR_DATA *victim = ch->fighting;
	AFFECT_DATA af;
	if (is_worn(obj) && number_percent()<80 && victim!=NULL)
	{
		infchance=number_percent();
		if(infchance>90)
		{
			act("The obsidian gem in the center of the spiked crown of the Obsidian Prince begins to glow with an unholy black light!",ch,0,victim,TO_ROOM);
			act("The obsidian gem in the center of the spiked crown of the Obsidian Prince begins to glow with an unholy black light!",ch,0,victim,TO_CHAR);
    			if (!is_affected(victim,skill_lookup("hold person")) && !saves_spell(ch->level,victim,DAM_OTHER))
			{
				init_affect(&af);
    				af.where     	= TO_AFFECTS;
				af.aftype     	= AFT_SPELL;
				af.type		= gsn_hold_person;
				af.location  	= 0;
    				af.modifier  	= 0;
    				af.duration  	= 0;
    				af.bitvector 	= 0;
    				af.level     	= ch->level;
    				affect_to_char( victim, &af );
    				send_to_char("Your muscles stop responding as the terror of the Abyss strikes you!\n\r", victim);
    				act("$n suddenly stiffens up, $s face the only sign of the terrors $e faces.",victim,NULL,NULL,TO_ROOM);
			}
		}
		if(infchance<90 && infchance>60)
		{
			act("The right-hand emerald in the spiked crown of the Obsidian Prince begins to glow with a sickly green light!",ch,0,victim,TO_ROOM);
			act("The right-hand emerald in the spiked crown of the Obsidian Prince begins to glow with a sickly green light!",ch,0,victim,TO_CHAR);
			damage(ch,victim,number_range(75,190),skill_lookup("acid blast"),DAM_ACID,TRUE);
		}
		if(infchance<15)
		{
			act("The left-hand ruby in the spiked crown of the Obsidian Prince begins to glow with a blood red light!",ch,0,victim,TO_ROOM);
			act("The left-hand ruby in the spiked crown of the Obsidian Prince begins to glow with a blood red light!",ch,0,victim,TO_CHAR);
			act("$n looks better as $s unholy strength fortifies $m.",ch,0,victim,TO_ROOM);
			send_to_char("A warm feeling fills your body.\n",ch);
			ch->hit = UMIN(ch->hit + 125, ch->max_hit);
		}

	}
}

void speech_prog_crown_prince(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{
	//Cumulative prog
	OBJ_DATA *blade;
	CHAR_DATA *victim=ch->fighting;
	AFFECT_DATA af;
	if(!is_worn(obj))
		return;
	if(!(blade=get_eq_char(ch,WEAR_WIELD)) || blade->pIndexData->vnum!=28026)
		if(!(blade=get_eq_char(ch,WEAR_DUAL_WIELD)) || blade->pIndexData->vnum!=28026)
			return;
	if(!victim)
		return;
	if(!str_cmp(speech,"Suffer!") && !is_affected(victim,gsn_stun))
	{
		act("Your body stiffens up as something alien takes over, as you glare at $N with unholy might!",ch,0,victim,TO_CHAR);
		act("$n's glares at $N with unholy might!",ch,0,victim,TO_NOTVICT);
		act("$n's glares at you with unholy might!",ch,0,victim,TO_VICT);
		if(number_percent()>75)
		{
			act("$n seems to shudder for a moment.",victim,0,0,TO_ROOM);
			act("You reel under $n's assault, but it passes.",ch,0,victim,TO_VICT);
			WAIT_STATE(victim,PULSE_VIOLENCE);
			WAIT_STATE(ch,PULSE_VIOLENCE*2);
			return;
		}
		act("Your wrath crushes $N into submission!",ch,0,victim,TO_CHAR);
		act("$n's wrath crushes $N into submission!",ch,0,victim,TO_NOTVICT);
		act("$n's wrath crushes you into submission!",ch,0,victim,TO_VICT);
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.type		= gsn_stun;
		af.level	= obj->level;
		af.aftype	= AFT_MALADY;
		af.location	= APPLY_NONE;
		af.modifier	= 0;
		af.duration	= 2;
		af.bitvector	= AFF_SLEEP;
		stop_fighting(victim, TRUE);
		victim->position = POS_SLEEPING;
		affect_to_char(victim, &af);
		WAIT_STATE(ch,PULSE_VIOLENCE*3);
	}	
}

void speech_prog_sword_prince(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{
	char strone[MAX_STRING_LENGTH];
	char strtwo[MAX_STRING_LENGTH];
	char strthree[MAX_STRING_LENGTH];
	char *tname="wrath";
	int tval;
	if(strstr(speech,"My vengeance is inexorable") || strstr(speech,"my vengeance is inexorable"))
	{
		tval=obj->value[0]+1;
		if(tval>WEAPON_STAFF)
			tval=WEAPON_EXOTIC;
		if(tval==WEAPON_EXOTIC)
			tname="wrath";
		if(tval==WEAPON_SWORD)
			tname="blade";
		if(tval==WEAPON_DAGGER)
			tname="talon";
		if(tval==WEAPON_SPEAR)
			tname="spear";
		if(tval==WEAPON_MACE)
			tname="mace";
		if(tval==WEAPON_AXE)
			tname="battle-axe";
		if(tval==WEAPON_FLAIL)
			tname="scourge";
		if(tval==WEAPON_WHIP)
			tname="barbed whip";
		if(tval==WEAPON_POLEARM)
			tname="halberd";
		if(tval==WEAPON_STAFF)
			tname="staff";
		sprintf(strone,"$n's $p seems to twist and contort into a %s!",tval==WEAPON_EXOTIC ? "deadly weapon" : tname);
		sprintf(strtwo,"the flaming %s of the Obsidian Prince",tname);
		sprintf(strthree,"You feel a surge of power as your $p twists and contorts into a %s!",tval==WEAPON_EXOTIC ? "deadly weapon": tname);
		act(strone,ch,obj,0,TO_ROOM);
		act(strthree,ch,obj,0,TO_CHAR);
		free_string(obj->short_descr);
		obj->short_descr=str_dup(strtwo);
		obj->value[0]=tval;
	}
}

void fight_prog_gloves_fortitude(OBJ_DATA *obj, CHAR_DATA *ch)
{
	if(is_worn(obj) && number_percent()>90)
	{
		act("$n's $p glows a faint violet.",ch,obj,0,TO_ROOM);
		act("Your $p glows a faint violet, and you feel a surge of mental strength!",ch,obj,0,TO_CHAR);
		ch->mana = UMIN(ch->mana + number_range(50,80), ch->max_mana);		
	}
}

void fight_prog_crystal_talon(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim=ch->fighting;
	if(!victim)
		return;
	if(is_worn(obj) && number_percent()>92 && !is_affected(victim,gsn_deafen))
	{
		act("A brutally loud blast of sound rumbles from $p!",ch,obj,0,TO_ALL);
		obj_cast_spell(skill_lookup("deafen"),58,ch,victim,obj);
	}
}

void fight_prog_belt_fire_demon(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *vch, *vch_next, *victim;
	int dam;
	if(is_worn(obj) && number_percent()<6)
	{
		act("You hear mocking laughter as an inferno roars out of $p!",ch,obj,0,TO_ALL);
		victim=ch->fighting;
       		for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	       	{
            	vch_next = vch->next_in_room;
           	if (!is_same_group(vch,victim) && vch->fighting!=ch)
                	continue;
             	dam=number_range(150,350);
             	if (saves_spell(obj->level-4,vch,DAM_FIRE))
                	dam-=number_range(80,100);
             	act("$N is seared by the blistering heat of $n's hellfire.",ch,0,vch,TO_NOTVICT);
             	act("You are seared by the blistering heat of $n's hellfire.",ch,0,vch,TO_VICT);
             	act("$N is seared by the blistering heat of your hellfire.",ch,0,vch,TO_CHAR);
             	damage_old(ch,vch,dam,skill_lookup("nova"),DAM_FIRE,TRUE);
        	}
	}
}

void fight_prog_bracer_diamond(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim = ch->fighting;
	if(is_worn(obj) && number_percent()>93)
	{
		if(ch->in_room->sector_type==SECT_INSIDE)
			return;
		act("Sunlight strikes out from $p in a deadly torrent of light at $n!",victim,obj,0,TO_ROOM);
		act("Sunlight strikes out from $p in a deadly torrent of light at you!",victim,obj,0,TO_CHAR);
                obj_cast_spell(skill_lookup("sunray"),obj->level,ch,victim,obj);
	}
}

void fight_prog_devil_shield(OBJ_DATA *obj, CHAR_DATA *ch)
{
	OBJ_DATA *wield;
	CHAR_DATA *victim;
	if(!is_worn(obj))
		return;
	victim=ch->fighting;
	if(!(wield=get_eq_char(victim,WEAR_WIELD)))
		return;
	if(number_percent()<95)
		return;
	if(number_percent()>94)
	{
		act("As your weapon nears $p, it shifts into a gaping maw, poised to devour you AND your weapon!",ch,obj,victim,TO_VICT);
		act("The mouth of $p shifts into a gaping maw and DEVOURS $n!",victim,obj,ch,TO_ROOM);
		act("The world goes permanently black as the shield closes its maw around you.",victim,0,0,TO_CHAR);
		raw_kill(ch,victim);
		return;
	}
	if(IS_OBJ_STAT(wield,ITEM_NOREMOVE) && number_percent()<5 && wield->level < 54)
		return;
	if(IS_OBJ_STAT(wield,ITEM_NODISARM) && number_percent()<5 && wield->level < 54)
		return;
	if(number_percent() < 5 && wield->level < 54)
	{
		act("$p shifts into a gaping maw and devours $T!",ch,obj,wield->short_descr,TO_ALL);
		obj_from_char(wield);
		extract_obj(wield);
	}
}

void fight_prog_sword_infinity(OBJ_DATA *obj, CHAR_DATA *ch)
{
    	int infchance;
	CHAR_DATA *victim = ch->fighting;
	if (is_worn(obj))
	{

	if(is_affected(ch,gsn_hold_person))
	{
		send_to_char("A disdainful voice in your mind whispers 'Paralyse my wielder? I think not.'\n\r",ch);
		affect_strip(ch,gsn_hold_person);
		send_to_char("The stiffness in your body wears off.\n\r",ch);
	}
	if(number_percent()<5 && !IS_EVIL(victim))
	{
		send_to_char("A voice in your mind whispers '{rWouldn't you rather be fighting evil? That is what I was made for.{x'\n\r",ch);
		return;
	}
	if(number_percent()<40 && victim!=NULL)
	{
		infchance=number_percent();
		if(IS_EVIL(victim) && infchance>50)
		{
		act("The holy sword Infinity glows a blinding white as it bites into $N!",ch,0,victim,TO_NOTVICT);
		act("The holy sword Infinity glows a blinding white as it bites into $N!",ch,0,victim,TO_CHAR);
		act("The holy sword Infinity glows a blinding white as it bites into you!",ch,0,victim,TO_VICT);
		(*skill_table[skill_lookup("wrath")].spell_fun) (skill_lookup("wrath"), 58, ch, victim, TAR_CHAR_OFFENSIVE);
		}
		if(infchance<10)
		{
		act("The holy sword Infinity glows with a blinding light!",ch,0,victim,TO_ALL);
		act("$n looks healthier as $s righteous wrath is strengthened!",ch,0,0,TO_ROOM);
		send_to_char("A warm feeling fills your body.\n",ch);
		ch->hit = UMIN( ch->hit + 125, ch->max_hit );
		}
		if(infchance>20 && infchance<40)
		{
		act("The holy sword Infinity blazes brightly as it lays $N open to righteous vengeance!",ch,0,victim,TO_CHAR);
		act("The holy sword Infinity blazes brightly as it lays $N open to righteous vengeance!",ch,0,victim,TO_NOTVICT);
		act("The holy sword Infinity blazes brightly as it lays you open to righteous vengeance!",ch,0,victim,TO_VICT);
		(*skill_table[skill_lookup("dispel magic")].spell_fun) (skill_lookup("dispel magic"), 53, ch, victim, TAR_CHAR_OFFENSIVE);
		}
	}
	}
}

bool death_prog_sword_infinity(OBJ_DATA *obj, CHAR_DATA *ch)
{	
	ROOM_INDEX_DATA *location;
	if(!is_worn(obj))
		return FALSE;
	if(IS_NPC(ch))
		return FALSE;
	act("As $n's lifeless body falls to the ground, $p begins to glow with a soft white light that gradually envelops him.",ch,obj,0,TO_ROOM);
	act("As your body dies, $p begins to glow with a soft white light that gradually envelops you.",ch,obj,0,TO_CHAR);
	act("$n vanishes in a blinding flash of light, leaving only the ashes of $p remaining!",ch,obj,0,TO_ROOM);
	act("You feel yourself transported, as $p slips from your grasp.",ch,obj,0,TO_CHAR);
	char_from_room(ch);
    	if (ch->hometown)
            	location = get_room_index(hometown_table[ch->hometown].recall);
    	else
        		location = get_room_index(ROOM_VNUM_TEMPLE);
	stop_fighting(ch,FALSE);
 	char_to_room(ch,location);
	ch->hit=ch->max_hit/3;
	extract_obj(obj);	
	return TRUE;
}

void get_prog_sword_infinity(OBJ_DATA *obj, CHAR_DATA *ch)
{
	if(!IS_GOOD(ch))
	{
        	act("Sensing your impurity, a holy sword named 'Infinity' rejects you!",ch,0,0,TO_CHAR);
        	act("Sensing $n's impurity, a holy sword named 'Infinity' rejects $m!",ch,0,0,TO_ROOM);
        	damage_old(ch,ch,1000,gsn_zealotism,DAM_TRUESTRIKE,TRUE);
		obj_from_char(obj);
		obj_to_room(obj,ch->in_room);
	}
}

void pulse_prog_sword_infinity(OBJ_DATA *obj, bool isTick)
{
	CHAR_DATA *ch = obj->carried_by;
	if(!ch || IS_NPC(ch) || !is_worn(obj))
		return;
	if(ch->wait>PULSE_VIOLENCE && number_percent()<50)
		ch->wait=PULSE_VIOLENCE;
}


bool sac_prog_sword_infinity(OBJ_DATA *obj, CHAR_DATA *ch)
{
	send_to_char("YOU DARE?!?!?!\n\r",ch);
  	act("$p glows blinding white and unleashes its heavenly wrath upon you!",ch,obj,NULL,TO_CHAR);
        act("$p glows blinding white and unleashes its heavenly wrath upon $n!",ch,obj,NULL,TO_ROOM);
  	damage(ch,ch,1000,gsn_zealotism,DAM_TRUESTRIKE, TRUE); 
  	return TRUE; /* Not destroyed */
}

void wear_prog_sword_infinity(OBJ_DATA *obj, CHAR_DATA *ch)
{
  	//AFFECT_DATA af;

	act("$p glows blazing white as the memories of those who have fought evil for centuries surge through you!",ch,obj,0,TO_CHAR);
	act("$p begins to glow blazing white!",ch,obj,0,TO_ROOM);
//	if(is_affected(ch,gsn_divine_intervention) || ch->size==SIZE_IMMENSE)
		return;
	act("$p imbues its divine prowess upon you to aid you in vanquishing evil!",ch,obj,0,TO_CHAR);
	act("$n seems to grow larger and stronger as $p glows brighter!",ch,obj,0,TO_ROOM);
	/*
	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.aftype	= AFT_COMMUNE;
	af.type		= gsn_divine_intervention;
	af.location	= APPLY_SIZE;
	af.modifier	= 1;
	af.duration	= -1;
	af.level	= obj->level;
	af.bitvector	= 0;
	affect_to_char(ch,&af);
	af.location	= APPLY_STR;
	af.modifier	= 5;
	affect_to_char(ch,&af);
	*/
}

void remove_prog_sword_infinity(OBJ_DATA *obj, CHAR_DATA *ch)
{
	if(is_affected(ch,gsn_divine_intervention))
		affect_strip(ch,gsn_divine_intervention);
}

void entry_prog_sword_infinity(OBJ_DATA *obj)
{
	CHAR_DATA *ch = obj->carried_by, *rch;

	if (!is_worn(obj))
		return;

	for(rch=ch->in_room->people; rch!=NULL; rch=rch->next_in_room)
	{
		if(IS_EVIL(rch) && number_percent()>60 && !is_safe(ch,rch))
		{
		//WE DON'T LIKE EVIL!
			act("As you see $N, $p calls out to you to strike a blow against evil!",ch,obj,rch,TO_CHAR);
			act("As $n arrives, $p seems to strike out of its own accord against $N!",ch,obj,rch,TO_NOTVICT);
			act("As $n arrives, $p seems to strike out of its own accord against you!",ch,obj,rch,TO_VICT);
			multi_hit(ch,rch,TYPE_UNDEFINED);
			break;
		}
	}
}

void speech_prog_sword_infinity(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	if(!is_worn(obj))
		return;
	if(!str_cmp(argument,"Take me to the home of my ancestors!"))
	{
		act("$p glows blinding white, enveloping $n. When your eyes recover, $e is gone!",ch,obj,0,TO_ROOM);
		act("You feel yourself transported to the ancestral home of those who forged Infinity.",ch,obj,0,TO_CHAR);
		char_from_room(ch);
		char_to_room(ch,get_room_index(110));
		act("Your journey causes you to contemplate on the lives of your ancestors for some time.",ch,0,0,TO_CHAR);
		act("$n arrives in a blaze of light, $p burning brightly!",ch,obj,0,TO_ROOM);
		ch->hit/=2;
		ch->mana/=10;
		WAIT_STATE(ch,PULSE_VIOLENCE*8);
		do_look(ch,"auto");
	}
}

void invoke_prog_tattoo_daed(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;

	if(!is_worn(obj))
		return;
	
	if(IS_AFFECTED(ch,AFF_HASTE))
		return send_to_char("Even Daedelex can't make you any faster!\n\r",ch);

	init_affect(&af);
	af.where = TO_AFFECTS;
	af.type = skill_lookup("haste");
	af.aftype = AFT_POWER;
	af.location = APPLY_DEX;
	af.duration = 5;
	af.modifier = 15;
	af.bitvector = AFF_HASTE;
	affect_to_char(ch,&af);
	act("Your blood races as $p glows briefly...hey, keep that in your pants!\n\rYour blood shouldn't be racing there...",ch, obj,0,TO_CHAR);	
	act("$n's pants seem to move slightly as $p glows.",ch,obj,0,TO_ROOM);
	return;
}

void fight_prog_tattoo_daed(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim;
	AFFECT_DATA af;
	if(!is_worn(obj))
		return;
	if((victim = ch->fighting) == NULL)
		return;
	if(number_percent() < 95)
		return;
	if(IS_AFFECTED(victim,AFF_BLIND))	
		return;
	do_yell(ch,"Daedelex!  Your bitch is in need!");
	act("A giant pant zipper appears and quickly opens, revealing a tall dark-elf.",ch,0,0,TO_ALL);
	act("The drow quickly turns and drops his pants, revealing a bleached white ass!",ch,0,0,TO_ALL);
	send_to_char("You are blinded!\n\r",victim);
	act("$N appears to be blinded.",ch,0,victim,TO_CHAR);
	act("$N appears to be blinded.",ch,0,victim,TO_NOTVICT);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_MALADY;
	af.duration = ch->level/7;
	af.type = skill_lookup("blindness");
	af.modifier = -10;
	af.location = APPLY_HITROLL;
	af.bitvector = AFF_BLIND;
	af.owner_name = str_dup(ch->original_name);
	affect_to_char(victim,&af);

return;
}

	

void fight_prog_earthsplitter(OBJ_DATA *obj, CHAR_DATA *ch) 
{ 
     CHAR_DATA *victim = ch->fighting; 
 
     if (is_worn(obj) && number_percent()>94 && victim != NULL) 
     { 
          if (ch->alignment > -1) 
          { 
          	act("You feel your arm pulse with energy as $p begins shimmering with power!",ch,obj,victim,TO_CHAR); 
          	act("$n's $p begins shining with energy.",ch,obj,victim,TO_ROOM); 
          	act("The energy travels up your arm and into your body, taking control and overpowering it with strength!",ch,obj,victim,TO_CHAR); 
          	act("$n suddenly begins pulsing with newfound energy and strength.",ch,obj,victim,TO_ROOM); 
          	act("With a grunt, $n hurls $p powerfully into the ground at $N's feet, splitting the earth around $M!",ch,obj,victim,TO_NOTVICT); 
          	act("You hurl $p into the ground near $N with incredible force, splitting the earth around $M!",ch,obj,victim,TO_CHAR);      
          	act("You feel engulfed in pain as $n hurls $p into the ground at your feet, splitting the very earth beneath you with terrifying might!",ch,obj,victim,TO_VICT);               
damage_new(ch,victim,number_range(120,175),TYPE_UNDEFINED,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"earthshattering blow"); 
          	act("$p returns to your hand and the glow fades away.",ch,obj,victim,TO_CHAR); 
          	act("$p returns to $n's hand and the glow fades away.",ch,obj,victim,TO_ROOM); 
		return;
          } 
      
          if (ch->alignment < 0) 
          { 
          	act("You feel your arm pulse with energy as $p is wrapped in a black light!",ch,obj,victim,TO_CHAR); 
          	act("$n's $p becomes enveloped in a dark aura.",ch,obj,victim,TO_ROOM); 
         	act("The energy travels up your arm and into your body, taking control and overpowering it with dark energy!",ch,obj,victim,TO_CHAR); 
          	act("$n suddenly begins pulsing with newfound energy and strength.",ch,obj,victim,TO_ROOM); 
          	act("With a grunt, $n hurls $p powerfully into the ground at $N's feet, splitting the earth around $M!",ch,obj,victim,TO_NOTVICT); 
          	act("You hurl $p into the ground near $N with incredible force, splitting the earth around $M!",ch,obj,victim,TO_CHAR);      
          	act("You feel engulfed in pain as $n hurls $p into the ground at your feet, splitting the very earth beneath you with terrifying might!",ch,obj,victim,TO_VICT);                    
damage_new(ch,victim,number_range(125,170),TYPE_UNDEFINED,DAM_BASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,HIT_NOMULT,"earthshattering blow"); 
          	act("$p returns to your hand and the dark aura fades away.",ch,obj,victim,TO_CHAR); 
          	act("$p returns to $n's hand and the dark aura fades away.",ch,obj,victim,TO_ROOM); 
		return;
          } 
     } 

     return;
}

void fight_prog_dispel_shield(OBJ_DATA *obj, CHAR_DATA *ch)
{
     CHAR_DATA *victim = ch->fighting;
     int check = number_percent();

     if (is_worn(obj) && check>80 && victim != NULL)
     {
	act("$p quivers slightly in your arms, drawn towards $N.",ch,obj,victim,TO_CHAR);
	act("The world goes silent for a moment as the eyes on $p gaze towards $N.",ch,obj,victim,TO_NOTVICT);
	act("You feel a deathly chill grip you as the eyes on $p glare deep into your soul.",ch,obj,victim,TO_VICT);

	if (check>89)
	{
		act("Despair clouds your mind and soul, leaving you vulnerable!",ch,obj,victim,TO_VICT);
		act("$N's face is overcome with a look of painful despair.",ch,obj,victim,TO_NOTVICT);
		act("$N's face is overcome with a look of painful despair!",ch,obj,victim,TO_CHAR);
		obj_cast_spell(skill_lookup("dispel magic"),ch->level-2,ch,victim,obj);
	}
  	
	else
	{
		act("You shake off the biting chill, resisting the eyes' ominous gaze.",ch,obj,victim,TO_VICT);
		act("$N shakes off the biting chill, resisting the eyes' ominous gaze.",ch,obj,victim,TO_CHAR);
	}
     }

     return;
}


void invoke_prog_tattoo_ormthrak(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{

     CHAR_DATA *victim;
     AFFECT_DATA af;

	if (is_affected(ch,gsn_ormthrak))
		return send_to_char("You cannot call upon the power of your lord so soon.\n\r", ch);

 	if (!is_worn(obj))
		return send_to_char("Nothing happens.\n\r", ch);
	
	if (ch->fighting != NULL)
		return send_to_char("You cannot invoke that during combat.\n\r", ch);

	if (argument[0] == '\0')
		return send_to_char("Who do you wish to invoke upon?\n\r", ch);

	if ((victim = get_char_room(ch,argument)) == NULL)
		return send_to_char("They aren't here.\n\r",ch);	

	if (victim == ch)
		return send_to_char("You cannot invoke that upon yourself.\n\r", ch);

	if (is_safe(ch,victim))
		return send_to_char("You cannot strike them.\n\r",ch);
	
	act("You call the power of your lord, Ormthrak, upon $N!",ch,NULL,victim,TO_CHAR);
	do_whisper(ch,"The wind howls, beckoning darkness...\nSoon, the flame of oblivion will consume this earth...\nCultures, people, ideals,\nDecimated and wrought anew, their hearts,\nEternally blackened by the crimson flame...\nAnd at the center of the Inevitable, Lord Ormthrak.\nCome! Let his limitless power embrace your soul!");
	act("Shadowy arms fade into existence and grasp your body, restricting your movement!",ch,0,victim,TO_VICT);
	act("Shadowy arms fade into existence and grasp $N's body, restricting his movement!",ch,0,victim,TO_NOTVICT);
	act("Shadowy arms fade into existence and grasp $N's body, restricting his movement!",ch,0,victim,TO_CHAR);
	init_affect(&af);
	af.where 	= TO_AFFECTS;
	af.aftype 	= AFT_POWER;
	af.type 	= gsn_shadowstrike;
	af.name		= str_dup("dark embrace");
	af.duration	= 5;
	af.level	= ch->level;
	af.modifier     = 0;
      	af.bitvector    = 0;
	affect_to_char(victim, &af);

	
	init_affect(&af);
      	af.where        = TO_AFFECTS;
	af.aftype       = AFT_POWER;
	af.type		= gsn_ormthrak;
      	af.level        = ch->level;
      	af.duration     = 5;
      	af.modifier     = 0;
      	af.bitvector    = 0;
	affect_to_char(ch,&af);

	WAIT_STATE(ch,PULSE_VIOLENCE*2);
      	return;
}

void fight_prog_tattoo_ormthrak(OBJ_DATA *obj, CHAR_DATA *ch)
{

     CHAR_DATA *victim = ch->fighting;

     if (victim != NULL && number_percent() > 86 && is_worn(obj))
	{
		do_whisper(ch,"Feel your battered body falter under the fiery touch of Lord Ormthrak!");
		obj_cast_spell(skill_lookup("unholy fire"),ch->level+6,ch,victim,obj);
	}

     return;
}

void speech_prog_jade_orb(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *mobSummoned;
    MOB_INDEX_DATA *mobData;
    
	if(!is_worn(obj))
		return;
	if(!str_cmp(argument,"Come my silent killer"))
	{
		mobData = get_mob_index(MOB_VNUM_SPITTING_SNAKE);
		if (mobData->count > 0)
			return send_to_char("The silent killer shall only serve one soul.\n\r", ch);
    		mobSummoned = create_mobile(get_mob_index(MOB_VNUM_SPITTING_SNAKE));
    		char_to_room(mobSummoned,ch->in_room);
    		add_follower(mobSummoned,ch);
    		mobSummoned->leader = ch;
    		SET_BIT(mobSummoned->affected_by,AFF_CHARM);
    		act("A soft hissing fills the room as $N blurs into existence before $n.",ch,NULL,mobSummoned,TO_ROOM);
    		act("A soft hissing fills the room as $N blurs into existence before you.",ch,NULL,mobSummoned,TO_CHAR);
    		mobSummoned->hit = mobSummoned->max_hit;
	}
	return;
}

void wear_prog_stone_skin(OBJ_DATA *obj, CHAR_DATA *ch)
{
	AFFECT_DATA af;

	if (ch->cabal==cabal_lookup("rager"))
		return send_to_char("The stench of this magical relic disgusts you and you ignore its effects.\n\r",ch);

	act("As you hold $p, you feel your skin harden into pure stone.",ch,obj,0,TO_CHAR);
	init_affect(&af);
	af.aftype = AFT_SKILL;
	af.where = TO_AFFECTS;
	af.location = 0;
	af.type = gsn_stoneskin;
	af.duration = -1;
	af.owner_name = str_dup(ch->original_name);
	af.modifier = 0;
	affect_to_char(ch,&af);
	return;
}

void remove_prog_stone_skin(OBJ_DATA *obj, CHAR_DATA *ch)
{
	AFFECT_DATA *check, *checkLooper;

	for (check = ch->affected; check != NULL; check = checkLooper)
	{
		checkLooper = check->next;
		if (check->type == gsn_stoneskin && check->duration == -1)
		{
			act("As you remove $p, you feel your hardened skin soften and become more frail.",ch,obj,0,TO_CHAR);
			affect_remove(ch,check);
		}
	}
	return;
}

void wear_prog_shield(OBJ_DATA *obj, CHAR_DATA *ch)
{
	AFFECT_DATA af;

	if (ch->cabal==cabal_lookup("rager"))
		return send_to_char("The stench of this magical relic disgusts you and you ignore its effects.\n\r",ch);

	act("As you hold $p, a shield of energy surrounds you.",ch,obj,0,TO_CHAR);
	init_affect(&af);
	af.aftype = AFT_SKILL;
	af.where = TO_AFFECTS;
	af.location = 0;
	af.type = gsn_shield;
	af.duration = -1;
	af.owner_name = str_dup(ch->original_name);
	af.modifier = 0;
	affect_to_char(ch,&af);
	return;
}

void remove_prog_shield(OBJ_DATA *obj, CHAR_DATA *ch)
{
	AFFECT_DATA *check, *checkLooper;

	for (check = ch->affected; check != NULL; check = checkLooper)
	{
		checkLooper = check->next;
		if (check->type == gsn_shield && check->duration == -1)
		{
			act("As you remove $p, the shield around you dissipates.",ch,obj,0,TO_CHAR);
			affect_remove(ch,check);
		}
	}
	return;
}

void wear_prog_aura(OBJ_DATA *obj, CHAR_DATA *ch)
{
	AFFECT_DATA af;

	if (ch->cabal==cabal_lookup("rager"))
		return send_to_char("The stench of this magical relic disgusts you and you ignore its effects.\n\r",ch);

	act("As you hold $p, an aura of magic surrounds you.",ch,obj,0,TO_CHAR);
	init_affect(&af);
	af.where = TO_AFFECTS;
	af.aftype = AFT_SKILL;
	af.type = skill_lookup("aura");
	af.location = APPLY_AC;
	af.modifier = -40;
	af.duration = -1;
	af.owner_name = str_dup(ch->original_name);
	affect_to_char(ch,&af);
	af.aftype = AFT_INVIS;
	af.location = APPLY_DAM_MOD;
	af.modifier = -20;
	affect_to_char(ch,&af);
	return;
}

void remove_prog_aura(OBJ_DATA *obj, CHAR_DATA *ch)
{
	AFFECT_DATA *check, *checkLooper;

	for (check = ch->affected; check != NULL; check = checkLooper)
	{
		checkLooper = check->next;
		if (check->type == skill_lookup("aura") && check->duration == -1)
		{
			act("As you remove $p, the magical aura around you fades away.",ch,obj,0,TO_CHAR);
			affect_remove(ch,check);
			continue;
		}
	}
	return;
}

void fight_prog_tattoo_zornath(OBJ_DATA *obj, CHAR_DATA *ch)
{
	AFFECT_DATA *check, *checkLooper, newAffect;
	CHAR_DATA *victim = ch->fighting;
	int counter, chance = number_percent();

	if (!is_worn(obj))
		return;
	if (victim == NULL)
		return;

	if(!is_affected(ch,gsn_zornath))
	{
		act("$n's body begins shimmering softly as Zornath's power channels through $m.",ch,0,victim,TO_NOTVICT);
		act("$n's body begins shimmering softly as Zornath's power channels through $m.",ch,0,victim,TO_VICT);
		act("Your body begins shimmering softly as Zornath's power channels through it.",ch,0,victim,TO_CHAR);
		init_affect(&newAffect);
		newAffect.where = TO_AFFECTS;
		newAffect.level = 1;
		newAffect.duration = 1;
		newAffect.aftype = AFT_POWER;
		newAffect.name = str_dup("omnipotent wrath");
		newAffect.type = gsn_zornath;
		newAffect.affect_list_msg = str_dup("channeling Zornath's might");
		affect_to_char(ch,&newAffect);
		newAffect.aftype = AFT_INVIS;
		newAffect.location = APPLY_DAMROLL;
		newAffect.modifier = 4;
		affect_to_char(ch,&newAffect);	
		return;	
	}

	if(is_affected(ch,gsn_zornath))
	{
		for (check = ch->affected; check != NULL; check = checkLooper)
		{
			checkLooper = check->next;
			if (check->type == gsn_zornath)
			{
				counter = check->level;
				if (counter >= 4)
				{
					int counterMultiplier = 100 + (counter * 15);
					act("A bolt of energy explodes from within $n towards $N!",ch,0,victim,TO_NOTVICT);
					act("A bolt of energy explodes from within $n towards you!",ch,0,victim,TO_VICT);
					act("A bolt of energy explodes towards $N from within you!",ch,0,victim,TO_CHAR);
		damage_new(ch,victim,60,TYPE_UNDEFINED,DAM_SLASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,counterMultiplier,"crackling bolt");			
				}
				if (counter >= 7)
				{
					act("Zornath's energy heals $n!",ch,0,victim,TO_NOTVICT);
					act("Zornath's energy heals $n!",ch,0,victim,TO_VICT);
					act("You feel healed by Zornath's energy!",ch,0,victim,TO_CHAR);
					obj_cast_spell(skill_lookup("rejuvenate"),ch->level*2,ch,ch,obj);
				}
				if (counter == 10)
				{
					act("A mighty and fearsome force overtakes $n!",ch,0,victim,TO_NOTVICT);
					act("A mighty and fearsome force overtakes $n!",ch,0,victim,TO_VICT);
					act("A mighty and fearsome force overtakes you!",ch,0,victim,TO_CHAR);
					act("$n is lifted into the air, arms towards the sky.",ch,0,victim,TO_NOTVICT);
					act("$n is lifted into the air, arms towards the sky.",ch,0,victim,TO_VICT);
					act("You are lifted into the air, your arms spread towards the sky.",ch,0,victim,TO_CHAR);
					do_yell(ch,"Zornath! Grant me your divine assistance to crush he who would oppose you!");
					do_zecho(ch,"{BThe sky darkens as countless lightning bolts of immense strength begin to plummet to the earth!{x");
					act("Zornath's lightning bolts {robliterate{x $N completely!",ch,0,victim,TO_NOTVICT);
					act("You feel an immense pain and a charge of electricity, and everything goes black.",ch,0,victim,TO_VICT);
					act("Zornath's lightning bolts {robliterate{x $N completely!",ch,0,victim,TO_CHAR);
					if(!IS_NPC(victim))
						raw_kill(ch,victim);
					else
						damage_new(ch,victim,number_range(100,150),TYPE_UNDEFINED,DAM_SLASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,1500,"wrath of Zornath");				
					affect_strip(ch,gsn_zornath);
					return;
				}
				if (chance <= (100 - (counter * 5)))
				{
					if (counter > 0 && counter <= 3)
					{
						act("$n's body glows softly as Zornath's power channels through $m.",ch,0,victim,TO_NOTVICT);
						act("$n's body glows softly as Zornath's power channels through $m.",ch,0,victim,TO_VICT);
						act("Your body glows softly as Zornath's power channels through it.",ch,0,victim,TO_CHAR);
					}
					else if (counter > 3 && counter <= 5)
					{
						act("$n's body pulses brightly as Zornath's power channels through $m!",ch,0,victim,TO_NOTVICT);
						act("$n's body pulses brightly as Zornath's power channels through $m!",ch,0,victim,TO_VICT);
						act("Your body pulses brightly as Zornath's power channels through it!",ch,0,victim,TO_CHAR);
					}
					else if (counter > 5 && counter <= 10)
					{
						act("$n's body is surrounded by a flaming aura as Zornath's power channels through $m!",ch,0,victim,TO_NOTVICT);
						act("$n's body is surrounded by a flaming aura as Zornath's power channels through $m!",ch,0,victim,TO_VICT);
						act("Your body is surrounded by a flaming aura as Zornath's power channels through it!",ch,0,victim,TO_CHAR);
					}
						affect_strip(ch,gsn_zornath);
						init_affect(&newAffect);
						newAffect.where = TO_AFFECTS;
						newAffect.level = counter + 1;
						newAffect.duration = 1;
						newAffect.aftype = AFT_POWER;
						newAffect.name = "omnipotent wrath";
						newAffect.type = gsn_zornath;
						newAffect.affect_list_msg = str_dup("channeling Zornath's might");
						affect_to_char(ch,&newAffect);
						newAffect.aftype = AFT_INVIS;
						newAffect.location = APPLY_DAMROLL;
						newAffect.modifier = (counter + 1) * 4;
						affect_to_char(ch,&newAffect);	
						return;	
				}

				else
				{
					act("The energy stored within $n unleashes itself suddenly and rips through $N!",ch,0,victim,TO_NOTVICT);
					act("The energy stored within $n unleashes itself suddenly and rips through you!",ch,0,victim,TO_VICT);
					act("The energy stored within you unleashes itself suddenly and rips through $N!",ch,0,victim,TO_CHAR);
	damage_new(ch,victim,100,TYPE_UNDEFINED,DAM_SLASH,TRUE,HIT_UNBLOCKABLE,HIT_NOADD,(counter*40),"omnipotent wrath");				
					affect_strip(ch,gsn_zornath);
					return;
				}
			}
		}
	}
}


void wear_prog_anti_rager(OBJ_DATA *obj, CHAR_DATA *ch)
{
	if (ch->cabal == cabal_lookup("rager"))
	{
		act("$N zaps you and you drop it.",ch,NULL,obj,TO_CHAR);
		act("$N zaps $n and $e drops it.",ch,NULL,obj,TO_ROOM);
		obj_from_char(obj);
		obj_to_room(obj,ch->in_room);
	}
	return;
}

void invoke_prog_black_sapphire(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	DESCRIPTOR_DATA *d;

	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);

	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);

	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	for (d = descriptor_list; d; d = d->next)
        {
		if (d->connected == CON_PLAYING
		&&  d->character->in_room != NULL && ch->in_room != NULL
		&&  d->character->in_room->area == ch->in_room->area
		&& d->character != ch)
		{
	    		if (!IS_IMMORTAL(d->character) && d->character->ghost == 0 && !IS_AFFECTED(d->character,AFF_CURSE))
			{
				act("You feel your stomach twist suddenly as dark magic washes over you.",d->character,0,0,TO_CHAR);
				act("You feel unclean.",d->character,0,0,TO_CHAR);
				act("$n looks uncomfortable.",d->character,0,0,TO_ROOM);
	    			init_affect(&af);
				af.aftype = AFT_SPELL;
				af.where = TO_AFFECTS;
				af.type = skill_lookup("curse");
				af.level = ch->level;
				af.duration = ch->level/5;
				af.bitvector = AFF_CURSE;
				af.location = APPLY_SAVING_SPELL;
				af.modifier = ch->level/8.5;
				affect_to_char(d->character,&af);
				af.location = APPLY_HITROLL;
				af.modifier = (ch->level/8.5)*-1;
				affect_to_char(d->character,&af);
			}
		}
	}

	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_black_opal(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	DESCRIPTOR_DATA *d;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);

	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);

	for (d = descriptor_list; d; d = d->next)
        {
		if (d->connected == CON_PLAYING
		&&  d->character->in_room != NULL && ch->in_room != NULL
		&&  d->character->in_room->area == ch->in_room->area
		&& d->character != ch)
		{
	    		if (!IS_IMMORTAL(d->character) && d->character->ghost == 0 && !IS_AFFECTED(d->character,AFF_PLAGUE))
			{
				act("You feel your stomach twist suddenly as dark magic washes over you.",d->character,0,0,TO_CHAR);
				act("You scream in agony as plague sores erupt from your skin.",d->character,0,0,TO_CHAR);
				act("$n screams in agony as plague sores erupt from $s skin.",d->character,0,0,TO_ROOM);
	    			init_affect(&af);
				af.aftype = AFT_SPELL;
				af.where = TO_AFFECTS;
				af.type = skill_lookup("plague");
				af.level = ch->level - 15;
				af.duration = ch->level/5;
				af.bitvector = AFF_PLAGUE;
				af.location = APPLY_STR;
				af.modifier = ch->level/10;
				affect_to_char(d->character,&af);
			}
		}
	}
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_soapstone(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim = ch->fighting;
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);

	if (victim == NULL)
		return send_to_char("You must be fighting to invoke this phylactery's powers.\n\r",ch);

	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (!IS_AFFECTED(victim,AFF_SLOW))
	{
		act("You feel yourself slow down.",ch,0,victim,TO_VICT);
		act("$N is moving more slowly.",ch,0,victim,TO_CHAR);
		act("$N is moving more slowly.",ch,0,victim,TO_NOTVICT);
		init_affect(&af);
		af.aftype = AFT_SPELL;
		af.where = TO_AFFECTS;
		af.type = skill_lookup("slow");
		af.level = ch->level - 15;
		af.duration = ch->level/5;
		af.bitvector = AFF_SLOW;
		af.location = APPLY_DEX;
		af.modifier = ch->level/10;
		affect_to_char(victim,&af);
	}
	else
		send_to_char("They are already slowed.\n\r",ch);

	if (!is_affected(victim,skill_lookup("soften")))
	{
		act("You feel more frail.",ch,0,victim,TO_VICT);
		act("$N seems more frail.",ch,0,victim,TO_CHAR);
		act("$N seems more frail.",ch,0,victim,TO_NOTVICT);
		init_affect(&af);
		af.aftype = AFT_SPELL;
		af.where = TO_AFFECTS;
		af.type = skill_lookup("soften");
		af.level = ch->level - 15;
		af.duration = ch->level/5;
		affect_to_char(victim,&af);
	}
	else
		send_to_char("They are already frail.\n\r",ch);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_bloodstone(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *victim;
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	if (argument[0] == '\0')
		return send_to_char("Invoke it upon who?\n\r",ch);
	
	if ((victim = get_char_room(ch,argument)) == NULL)
		return send_to_char("They aren't here.\n\r",ch);

	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);

	if (!is_affected(victim,skill_lookup("rot")))
	{
		act("You feel your body begin to decompose!",ch,0,victim,TO_VICT);
		act("$N's body begins decomposing before him!",ch,0,victim,TO_CHAR);
		act("$N's body begins decomposing before him!",ch,0,victim,TO_NOTVICT);
		init_affect(&af);
		af.aftype = AFT_SPELL;
		af.where = TO_AFFECTS;
		af.type = skill_lookup("rot");
		af.level = ch->level - 15;
		af.duration = ch->level/3.5;
		affect_to_char(victim,&af);	
	}
	else
		send_to_char("They are already rotting away.\n\r",ch);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	multi_hit(victim,ch, TYPE_UNDEFINED);
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_black_pearl(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	send_to_char("Your heart fills with darkness.\n\r",ch);

	init_affect(&af);
    	af.where     	= TO_IMMUNE;
    	af.aftype  	= AFT_POWER;
    	af.type    	= skill_lookup("darkheart");
   	af.level     	= ch->level;
    	af.duration  	= ch->level;
    	af.bitvector 	= IMM_HOLY;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}
	
void invoke_prog_diamond(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	send_to_char("Your rotting flesh hardens into stone.\n\r",ch);
	act("$n's rotting flesh hardens into stone.",ch,0,0,TO_ROOM);

	if (!is_affected(ch,skill_lookup("stone skin")))
	{
		init_affect(&af);
    		af.where     	= TO_AFFECTS;
    		af.aftype  	= AFT_POWER;
    		af.type    	= skill_lookup("stone skin");
   		af.level     	= ch->level;
    		af.duration  	= -1;
		af.location	= APPLY_AC;
		af.modifier	= -40;
		affect_to_char(ch,&af);
	}
	else
		send_to_char("Your skin is already hardened.\n\r",ch);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_pale_jade(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	send_to_char("You feel aligned with darkness.\n\r",ch);

	if (!IS_AFFECTED(ch,AFF_PROTECT_GOOD))
	{
		init_affect(&af);
    		af.where     	= TO_AFFECTS;
    		af.aftype  	= AFT_POWER;
    		af.type    	= skill_lookup("protection good");
   		af.level     	= ch->level;
    		af.duration  	= ch->level;
		af.location	= APPLY_SAVING_SPELL;
		af.modifier	= -10;
		af.bitvector	= AFF_PROTECT_GOOD;
		affect_to_char(ch,&af);
	}
	else
		send_to_char("You are already aligned with darkness.\n\r",ch);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}


void invoke_prog_fire_opal(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (is_affected(ch,skill_lookup("transform")))
		return send_to_char("Your body is already transformed.\n\r",ch);

	send_to_char("Your bones and rotting flesh rearrange themselves, granting you increased vitality.\n\r",ch);
	act("$n grins as $s bones and rotting flesh rearrange themselves in a gruesome manner.",ch,0,0,TO_ROOM);

	init_affect(&af);
    	af.where     	= TO_AFFECTS;
    	af.aftype  	= AFT_POWER;
    	af.type    	= skill_lookup("transform");
   	af.level     	= ch->level;
    	af.duration  	= ch->level;
	af.location	= APPLY_HIT;
	af.modifier	= number_range(850,950);
	affect_to_char(ch,&af);

	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;	
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_violet_garnet(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	CHAR_DATA *gch, *gch_next;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);

	for (gch = ch->in_room->people; gch != NULL; gch = gch_next)
	{
		gch_next = gch->next;
		if (gch && (is_same_group(ch,gch) || gch == ch))
		{
			send_to_char("The elements gather about you, protecting you from harm.\n\r",gch);

			if (!is_affected(gch,skill_lookup("shield of flames")))
			{
				act("A shield of flames flares into existence around you.",gch,0,0,TO_CHAR);
				act("A shield of flames flares into existence around $n.",gch,0,0,TO_ROOM);
				init_affect(&af);
    				af.where     	= TO_AFFECTS;
    				af.aftype  	= AFT_POWER;
    				af.type    	= skill_lookup("shield of flames");
   				af.level     	= gch->level;
    				af.duration  	= gch->level/3;
				af.location	= APPLY_DAM_MOD;
				af.modifier	= -3;
				affect_to_char(gch,&af);
			}
			else
				send_to_char("You are already shielded in flames.\n\r",gch);
			if (!is_affected(gch,skill_lookup("shield of bubbles")))
			{
				act("A protective shield of bubbles envelops you.",gch,0,0,TO_CHAR);
				act("A protective shield of bubbles envelops $n.",gch,0,0,TO_ROOM);
				init_affect(&af);
    				af.where     	= TO_AFFECTS;
    				af.aftype  	= AFT_POWER;
    				af.type    	= skill_lookup("shield of bubbles");
   				af.level     	= gch->level;
    				af.duration  	= gch->level/3;
				af.location	= APPLY_DAM_MOD;
				af.modifier	= -3;
				affect_to_char(gch,&af);
			}
			else
				send_to_char("You are already shielded in bubbles.\n\r",gch);
			if (!is_affected(gch,skill_lookup("shield of lightning")))
			{
				act("A shield of lightning crackles into existence around you.",gch,0,0,TO_CHAR);
				act("A shield of lightning crackles into existence around $n.",gch,0,0,TO_ROOM);
				init_affect(&af);
    				af.where     	= TO_AFFECTS;
    				af.aftype  	= AFT_POWER;
    				af.type    	= skill_lookup("shield of lightning");
   				af.level     	= gch->level;
    				af.duration  	= gch->level/3;
				af.location	= APPLY_DAM_MOD;
				af.modifier	= -3;
				affect_to_char(gch,&af);
			}
			else
				send_to_char("You are already shielded in lightning.\n\r",gch);
			if (!is_affected(gch,skill_lookup("shield of frost")))
			{
				act("The temperature around you drops suddenly, forming a shield of frost about you.",gch,0,0,TO_CHAR);
				act("The temperature around $n drops suddenly, forming a shield of frost about $m.",gch,0,0,TO_ROOM);
				init_affect(&af);
    				af.where     	= TO_AFFECTS;
    				af.aftype  	= AFT_POWER;
    				af.type    	= skill_lookup("shield of frost");
   				af.level     	= gch->level;
    				af.duration  	= gch->level/3;
				af.location	= APPLY_DAM_MOD;
				af.modifier	= -3;
				affect_to_char(gch,&af);
			}
			else
				send_to_char("You are already shielded in frost.\n\r",gch);
			if (!is_affected(gch,skill_lookup("shield of wind")))
			{
				act("Strong winds coalesce about you in a shield of wind.",gch,0,0,TO_CHAR);
				act("Strong winds coalesce about $n in a shield of wind.",gch,0,0,TO_ROOM);
				init_affect(&af);
    				af.where     	= TO_AFFECTS;
    				af.aftype  	= AFT_POWER;
    				af.type    	= skill_lookup("shield of wind");
   				af.level     	= gch->level;
    				af.duration  	= gch->level/3;
				af.location	= APPLY_DAM_MOD;
				af.modifier	= -3;
				affect_to_char(gch,&af);
			}
			else
				send_to_char("You are already shielded in wind.\n\r",gch);
			if (!is_affected(gch,skill_lookup("shield of dust")))
			{
				act("The dust picks up from the earth, forming a shield of dust around you.",gch,0,0,TO_CHAR);
				act("The dust picks up from the earth, forming a shield of dust around $n.",gch,0,0,TO_ROOM);
				init_affect(&af);
    				af.where     	= TO_AFFECTS;
    				af.aftype  	= AFT_POWER;
    				af.type    	= skill_lookup("shield of dust");
   				af.level     	= gch->level;
    				af.duration  	= gch->level/3;
				af.location	= APPLY_DAM_MOD;
				af.modifier	= -3;
				affect_to_char(gch,&af);
			}
			else
				send_to_char("You are already shielded in dust.\n\r",gch);
		}
	}

	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_spherical_malachite(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);

	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (is_affected(ch,skill_lookup("anti-magic shell")))
		return send_to_char("You are already armored in an anti-magic shell.\n\r",ch);

	send_to_char("You feel immune to the effects of magic.\n\r",ch);

	init_affect(&af);
    	af.where     	= TO_IMMUNE;
    	af.aftype  	= AFT_POWER;
   	af.level     	= ch->level;
	af.type		= skill_lookup("anti-magic shell");
    	af.duration  	= ch->level/2;
    	af.bitvector 	= IMM_MAGIC;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_sunstone(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);

	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (is_affected(ch,skill_lookup("dark essence")))
		return send_to_char("You already resist the effects of the light.\n\r",ch);

	send_to_char("You build a natural resistance to all that is holy.\n\r",ch);

	init_affect(&af);
	af.where     	= TO_RESIST;
    	af.aftype  	= AFT_POWER;
   	af.level     	= ch->level;
	af.type		= skill_lookup("dark essence");
    	af.duration  	= ch->level/2;
    	af.bitvector 	= RES_HOLY;
	affect_to_char(ch,&af);
	af.bitvector	= RES_LIGHT;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_zircon(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);

	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (IS_SET(ch->res_flags,RES_COLD))
		return send_to_char("You already resist cold.\n\r",ch);

	send_to_char("The chill of the grave flows through you, protecting you from cold.\n\r",ch);

	init_affect(&af);
	af.where     	= TO_RESIST;
    	af.aftype  	= AFT_POWER;
   	af.level     	= ch->level;
	af.type		= skill_lookup("resist cold");
    	af.duration  	= ch->level/2;
    	af.bitvector 	= RES_COLD;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_striped_glass(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);

	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (IS_SET(ch->res_flags,RES_LIGHTNING))
		return send_to_char("You are already resistant to lightning.\n\r",ch);

	send_to_char("Your bones and rotting flesh develop a resistance to lightning.\n\r",ch);

	init_affect(&af);
	af.where     	= TO_RESIST;
    	af.aftype  	= AFT_POWER;
   	af.level     	= ch->level;
	af.type		= skill_lookup("resist lightning");
    	af.duration  	= ch->level/2;
    	af.bitvector 	= RES_LIGHTNING;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}


void invoke_prog_basalt(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);

	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (IS_SET(ch->res_flags,RES_FIRE))
		return send_to_char("You are already resistant to fire and heat.\n\r",ch);

	send_to_char("You build a natural resistance to fire and heat.\n\r",ch);

	init_affect(&af);
	af.where     	= TO_RESIST;
    	af.aftype  	= AFT_POWER;
   	af.level     	= ch->level;
	af.type		= skill_lookup("resist heat");
    	af.duration  	= ch->level/2;
    	af.bitvector 	= RES_FIRE;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_star_diopside(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	bool isHoly = FALSE, isNegative = FALSE;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);

	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (IS_SET(ch->res_flags,RES_HOLY))
	{
		isHoly = TRUE;
		send_to_char("You are already resistant to holy energy.\n\r",ch);
	}
	if (IS_SET(ch->res_flags,RES_NEGATIVE))
	{
		isNegative = TRUE;
		send_to_char("You are already resistant to negative energy.\n\r",ch);
	}

	if (isHoly && !isNegative)
		send_to_char("You build a natural resistance to negative energy.\n\r",ch);

	if (!isHoly && isNegative)
		send_to_char("You build a natural resistance to positive energy.\n\r",ch);

	if (!isHoly && !isNegative)
		send_to_char("You build a natural resistance to positive and negative energy.\n\r",ch);

	if (!(isHoly && isNegative))
	{
		if (!isHoly)
		{
			init_affect(&af);
			af.where     	= TO_RESIST;
    			af.aftype  	= AFT_POWER;
   			af.level     	= ch->level;
			af.type		= skill_lookup("resist positive");
    			af.duration  	= ch->level/2;
    			af.bitvector 	= RES_HOLY;
			affect_to_char(ch,&af);
		}
		if (!isNegative)
		{
			init_affect(&af);
			af.where     	= TO_RESIST;
    			af.aftype  	= AFT_POWER;
   			af.level     	= ch->level;
			af.type		= skill_lookup("resist negative");
    			af.duration  	= ch->level/2;
    			af.bitvector 	= RES_NEGATIVE;
			affect_to_char(ch,&af);
		}
	}
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_rounded_jade(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);

	if (ch->fighting == NULL)
		return send_to_char("You must be fighting to invoke this phylactery's powers.\n\r",ch);

	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (!is_affected(ch,skill_lookup("wraithform")))
	{
		act("You become briefly insubstantial as you pull yourself into the Prime Negative Plane.",ch,0,0,TO_CHAR);
		act("$n becomes briefly insubstantial as $e pulls $mself into the Prime Negative Plane.",ch,0,0,TO_ROOM);
		init_affect(&af);
		af.where = TO_AFFECTS;
		af.aftype = AFT_POWER;
		af.type  = skill_lookup("wraithform");
		af.level = ch->level;
		af.duration = ch->level/2;
		affect_to_char(ch,&af);
	}
	else
		send_to_char("You are already in the Prime Negative Plane.\n\r",ch);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}


void invoke_prog_malachite_disc(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);

	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (is_affected(ch,skill_lookup("anti-magic shell")))
		return send_to_char("You are already armored in an anti-magic shell.\n\r",ch);

	send_to_char("You feel resistant to the effects of magic.\n\r",ch);

	init_affect(&af);
    	af.where     	= TO_RESIST;
    	af.aftype  	= AFT_POWER;
   	af.level     	= ch->level;
	af.type		= skill_lookup("anti-magic shell");
    	af.duration  	= ch->level/1.5;
    	af.bitvector 	= RES_MAGIC;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_pale_aquamarine(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);

	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (IS_SET(ch->res_flags,RES_MENTAL))
		return send_to_char("You are already resistant to mental attacks.\n\r",ch);

	send_to_char("You build a natural resistance to mental attacks.\n\r",ch);

	init_affect(&af);
	af.where     	= TO_RESIST;
    	af.aftype  	= AFT_POWER;
   	af.level     	= ch->level;
	af.type		= skill_lookup("resist mental");
    	af.duration  	= ch->level/2;
    	af.bitvector 	= RES_MENTAL;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_granite(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (is_affected(ch,skill_lookup("body of stone")))
		return send_to_char("Your body is already stone.\n\r",ch);

	send_to_char("Your bones and rotting flesh harden into pure stone.\n\r",ch);
	act("$n's bones and rotting flesh harden into pure stone.",ch,0,0,TO_ROOM);

	init_affect(&af);
    	af.where     	= TO_AFFECTS;
    	af.aftype  	= AFT_POWER;
    	af.type    	= skill_lookup("body of stone");
   	af.level     	= ch->level;
    	af.duration  	= ch->level;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_amethyst(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);

	send_to_char("You feel the energy of the grave coursing through your bones, revitalizing you.\n\r",ch);
	act("The energy of the grave courses through $n's bones, revitalizing $m.",ch,0,0,TO_ROOM);

	ch->hit += number_range(850,925);
	if (ch->hit > ch->max_hit)
		ch->hit = ch->max_hit;
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_milky_quartz(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (is_affected(ch,skill_lookup("sneak")))
		return send_to_char("You are already sneaking.\n\r",ch);

	act("$p glows in your hand and you find yourself moving silently.",ch,obj,0,TO_CHAR);
	act("$n's $p glows in $s hand.",ch,obj,0,TO_ROOM);
	init_affect(&af);
    	af.where     	= TO_AFFECTS;
    	af.aftype  	= AFT_POWER;
    	af.type    	= skill_lookup("sneak");
   	af.level     	= ch->level;
    	af.duration  	= ch->level*3;
	af.bitvector	= AFF_SNEAK;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_tauroidal_beryl(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (is_affected(ch,skill_lookup("giant strength")))
		return send_to_char("You already have the strength of a giant.\n\r",ch);

	send_to_char("Your body is infused with the strength of the dead.\n\r",ch);
	act("$n is infused with the strength of the dead.",ch,0,0,TO_ROOM);

	init_affect(&af);
    	af.where     	= TO_AFFECTS;
    	af.aftype  	= AFT_POWER;
    	af.type    	= skill_lookup("giant strength");
   	af.level     	= ch->level;
	af.location	= APPLY_ENHANCED_MOD;
	af.modifier	= 4;
    	af.duration  	= ch->level*2;
	affect_to_char(ch,&af);
	af.location	= APPLY_STR;
	af.modifier	= 5;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_something_oily(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	ROOM_INDEX_DATA *teleportedRoom;
	
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);

	send_to_char("Your dark magic surrounds you, blocking out all light.\n\r",ch);
	act("A swirling black mist consumes $n and when it fades, $e is gone!",ch,0,0,TO_ROOM);
	send_to_char("As it fades, you find yourself somewhere else!\n\r",ch);

	teleportedRoom = get_random_room(ch);
	char_from_room(ch);
	char_to_room(ch,teleportedRoom);
	do_look(ch,"auto");
	act("A swirling black mist blasts into the room and disappears, leaving $n in its wake.",ch,0,0,TO_ROOM);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_angled_jasper(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (is_affected(ch,skill_lookup("detect hidden")))
		return send_to_char("You can already see that which is hidden.\n\r",ch);

	send_to_char("You become able to detect those who hide in shadows through your dark magic.\n\r",ch);

	init_affect(&af);
    	af.where     	= TO_AFFECTS;
    	af.aftype  	= AFT_POWER;
    	af.type    	= skill_lookup("detect hidden");
   	af.level     	= ch->level;
    	af.duration  	= ch->level;
	af.bitvector	= AFF_DETECT_HIDDEN;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_jade(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (is_affected(ch,skill_lookup("acute vision")))
		return send_to_char("You can already see that which is hidden in nature.\n\r",ch);

	send_to_char("You become able to detect those who hide in nature through your dark magic.\n\r",ch);

	init_affect(&af);
    	af.where     	= TO_AFFECTS;
    	af.aftype  	= AFT_POWER;
    	af.type    	= skill_lookup("acute vision");
   	af.level     	= ch->level;
    	af.duration  	= ch->level;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_cats_eye(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	if (is_affected(ch,skill_lookup("detect good")))
		return send_to_char("You can already see the hearts of good.\n\r",ch);

	send_to_char("You become able to detect the lighthearted through your dark magic.\n\r",ch);

	init_affect(&af);
    	af.where     	= TO_AFFECTS;
    	af.aftype  	= AFT_POWER;
    	af.type    	= skill_lookup("detect good");
   	af.level     	= ch->level;
    	af.duration  	= ch->level;
	af.bitvector	= AFF_DETECT_GOOD;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_cats_eye_agate(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *pet;
	char buf[MSL];
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);

	for(pet = char_list; pet!= NULL; pet=pet->next)
	{
		if(IS_NPC(pet) && IS_AFFECTED(pet,AFF_CHARM) && IS_SET(pet->act, ACT_UNDEAD) && pet->pIndexData->vnum== MOB_VNUM_ZOMBIE && pet->master == ch)
		{
			sprintf(buf,"%s kneels before $n as $s phylactery infuses it with power!",pet->short_descr);
			act(buf,ch,0,0,TO_ROOM);
			free_string(buf);
			sprintf(buf,"%s kneels before you as your phylactery infuses it with incredible power!",pet->short_descr);
			act(buf,ch,0,0,TO_CHAR);
			free_string(buf);
			sprintf(buf,"%s's body collapses into dust.",pet->short_descr);
			act(buf,ch,0,0,TO_ROOM);
			act(buf,ch,0,0,TO_CHAR);
			act("Moments later, a menacing ghoul appears in a dark mist to take its place!",ch,0,0,TO_ROOM);
			act("Moments later, a menacing ghoul appears in a dark mist to take its place!",ch,0,0,TO_CHAR);
			
			pet->level += 5;
			pet->max_hit *= 3;
			pet->damroll *= 1.5;
			pet->hitroll *= 3;
			pet->name = str_dup("ghoul");
			pet->hit = pet->max_hit;
			pet->short_descr = str_dup("a horrifying, decaying ghoul");
			pet->long_descr = str_dup("A menacing ghoul stands here with a grin on its rotting face.\n");
			pet->description = str_dup("This once-living being is no ordinary zombie; power echoes\n from within it, a dark power that threatens to consume\n the entire room.\n");
		}
	}
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}


void invoke_prog_faceted_jasper(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	bool found = FALSE;
	CHAR_DATA *pet;

	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);

	//Taken from necro.c's spell_black_circle function.
	for(pet = char_list; pet!= NULL; pet=pet->next)
	{
		if(IS_NPC(pet) && IS_AFFECTED(pet,AFF_CHARM) && (IS_SET(pet->act, ACT_UNDEAD) || (pet->pIndexData->vnum == MOB_VNUM_L_GOLEM || pet->pIndexData->vnum==MOB_VNUM_G_GOLEM)) && pet->master == ch)
			{
				stop_fighting(pet,TRUE);
				act("$n disappears suddenly.",pet,0,0,TO_ROOM);
				char_from_room(pet);
				char_to_room(pet, ch->in_room);
				act("$n arrives suddenly, kneeling before $N.",pet,0,ch,TO_ROOM);
				found = TRUE;
			}
	}
	if(!found)
		return send_to_char("Your summonings went unanswered.\n\r",ch);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_rainbow_moonstone(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	bool kill = FALSE;
	char buf[MSL];
	CHAR_DATA *victim;
	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	if (argument[0] == '\0')
		return send_to_char("Invoke it upon who?\n\r",ch);
	
	if ((victim = get_char_room(ch,argument)) == NULL)
		return send_to_char("They aren't here.\n\r",ch);

	if (!IS_NPC(victim))
		return send_to_char("This phylactery can only be invoked on servitors.\n\r",ch);

	if (victim->master == NULL)
		return send_to_char("This phylactery can only be invoked on servitors.\n\r",ch);

	if (victim->pIndexData->vnum == MOB_VNUM_FIRE_ELEMENTAL) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_WATER_ELEMENTAL) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_AIR_ELEMENTAL) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_EARTH_ELEMENTAL) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_ZOMBIE) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_BEAST) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_UNICORN) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_G_GOLEM) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_L_GOLEM) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_WOLF) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_BEAR) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_LION) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_FALCON) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_SPITTING_SNAKE) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_ARMOR) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_WEAPON) kill = TRUE;
	else if (victim->pIndexData->vnum == MOB_VNUM_KEY) kill = TRUE;

	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	sprintf(buf,"You call upon your dark magic to banish %s!",victim->short_descr);
	act(buf,ch,0,0,TO_CHAR);
	sprintf(buf,"$n calls upon $s dark magic to banish %s!",victim->short_descr);
	act(buf,ch,0,0,TO_ROOM);
	if (kill)
	{
		sprintf(buf,"With a blinding flash, %s is blown away into particles!",victim->short_descr);
		act(buf,ch,0,0,TO_CHAR);
		act(buf,ch,0,0,TO_ROOM);
		raw_kill_new(ch,victim,FALSE, TRUE);
	}
	else
	{
		sprintf(buf,"%s is unaffected by your dark magic.",victim->short_descr);
		act(buf,ch,0,0,TO_CHAR);
		free_string(buf);
		sprintf(buf,"%s is unaffected by $n's dark magic.",victim->short_descr);
		act(buf,ch,0,0,TO_ROOM);
	}
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}


void invoke_prog_black_diamond(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af, *affectCheck, *affectLooper;
	int level = 0;
	int damReduxLooper, damRedux = 20, previousDamRedux = 20;
	char buf[MSL];

	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);
	
	for (affectCheck = ch->affected; affectCheck != NULL; affectCheck = affectLooper)
	{
		affectLooper = affectCheck->next;
		if (affectCheck->type == skill_lookup("soul channel"))
			level = affectCheck->level;
	}

	if (level == 0)
		send_to_char("You channel your physical pain through your blackened soul, where it is lost eternally.\n\r",ch);
	else
	{
		send_to_char("You channel an even greater amount of your physical pain through your blackened soul.\n\r",ch);
		affect_strip(ch,skill_lookup("soul channel"));
	}

	if (level > 0)
		for (damReduxLooper = 1; damReduxLooper <= level; damReduxLooper++)
		{
			damRedux += previousDamRedux * .8;
			previousDamRedux *= .8;
		}

	sprintf(buf, "reduces damage taken by %d%%",damRedux);

	init_affect(&af);
    	af.where     	= TO_AFFECTS;
    	af.aftype  	= AFT_POWER;
    	af.type    	= skill_lookup("soul channel");
	af.affect_list_msg = str_dup(buf);
   	af.level     	= level+1;
    	af.duration  	= -1;
		af.location = APPLY_DAM_MOD;
		af.modifier = -1 * damRedux;
	affect_to_char(ch,&af);
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void invoke_prog_forest_green_emerald(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	CHAR_DATA *target, *targetLooper;
	int sn_fog, sn_fire;
	bool set = TRUE;

	if (ch->race != race_lookup("lich"))
		return send_to_char("The purpose of this phylactery escapes you.\n\r",ch);
	if (!is_worn(obj))
		return send_to_char("You must be holding it to invoke its powers!\n\r",ch);
	
	act( "You raise $p in a clenched fist!",ch,obj,0,TO_CHAR );
	act( "$n raises $p in a clenched fist!",ch,obj,0,TO_ROOM);

    	sn_fog = skill_lookup("faerie fog");
    	sn_fire = skill_lookup("faerie fire");
	send_to_char("You summon a cloak of dark magic to conceal your group within nature.\n\r",ch);
	
	for (target = ch->in_room->people; target != NULL; target = targetLooper)
	{
		targetLooper = target->next;
		if (is_same_group(ch,target))
		{	
    			if (is_affected(target,sn_fog) || is_affected(target,sn_fire) || is_affected(target, gsn_incandescense))
    			{
				send_to_char("You can't become camouflaged while revealed.\n\r",ch);
				set = FALSE;
   			}

    			if (IS_AFFECTED(ch, AFF_FAERIE_FIRE))
				return send_to_char("You can't become camouflaged while revealed.\n\r",ch);

    			if ( isInWilderness(target) )
    			{
				send_to_char("There is no cover here.\n\r", ch);
				set = FALSE;
    			}	

			if (set)
			{
				SET_BIT(target->affected_by,AFF_CAMOUFLAGE);
				act("A cloak of dark magic conceals you within the trees.",target,0,0,TO_CHAR);
			}
		}
	}
	if (obj->condition == 1)
	{
		act("$p crumbles into dust in your hands.",ch,obj,0,TO_CHAR);
		act("$p crumbles into dust in $n's hands.",ch,obj,0,TO_ROOM);
		extract_obj(obj);
	}
	else
		obj->condition--;
	WAIT_STATE(ch,PULSE_VIOLENCE*2);
	return;
}

void wear_prog_quest_reward_warrior_weapon( OBJ_DATA *obj, CHAR_DATA *ch)
{
	char buf[MSL];
	sprintf(buf, "As you grip %s, you feel empowered by the spirit of Victory.\n\r", obj->short_descr);
	send_to_char(buf,ch);
}

void wear_prog_quest_reward_warrior_boots( OBJ_DATA *obj, CHAR_DATA *ch)
{
	char buf[MSL];
	sprintf(buf, "{yYou feel a rush of insight into the March skill!{x\n\r");
	ch->pcdata->learned[skill_lookup("march")] = 100;
	send_to_char(buf,ch);
}

void remove_prog_quest_reward_warrior_weapon( OBJ_DATA *obj, CHAR_DATA *ch)
{
	char buf[MSL];
	sprintf(buf, "The spirit of Victory leaves you as you remove %s.\n\r", obj->short_descr);
	send_to_char(buf,ch);
}

void remove_prog_quest_reward_warrior_boots( OBJ_DATA *obj, CHAR_DATA *ch)
{
	char buf[MSL];
	sprintf(buf, "{yYou lose your ability to March!{x\n\r");
	ch->pcdata->learned[skill_lookup("march")] = -2;
	send_to_char(buf,ch);
}

void remove_prog_quest_reward_warrior_plate( OBJ_DATA *obj, CHAR_DATA *ch)
{
	if (is_affected(ch,skill_lookup("valiant wrath"))) {
		act("You lose your fervor as you remove $p.",ch,obj,NULL,TO_CHAR);
		act("$n looks less zealous as $e removes $p.",ch,obj,NULL,TO_ROOM);
		affect_strip(ch,skill_lookup("valiant wrath"));
	}
}

void remove_prog_quest_reward_warrior_gauntlets( OBJ_DATA *obj, CHAR_DATA *ch)
{
	if (is_affected(ch,skill_lookup("spiderhands"))) {
		AFFECT_DATA *af = affect_find(ch->affected,skill_lookup("spiderhands"));
		if (strcmp(af->name,"weaponmeld") == 0) {
			act("As you remove $p, your weapon is freed from their clasp.",ch,obj,NULL,TO_CHAR);
			act("$n's weapon is freed from $p's clasp as $e removes them.",ch,obj,NULL,TO_ROOM);
			affect_remove(ch,af);
		}
	}
}

void speech_prog_quest_reward_warrior_weapon(OBJ_DATA *obj, CHAR_DATA *ch, char *speech)
{
	if ((strstr(speech,"Victory to the sword!") || strstr(speech,"victory to the sword!")) && is_worn(obj) && obj->condition != 91)
	{
		act("$p glows and is encompassed in a bright light.",ch,obj,0,TO_CHAR);
		act("$n's $p glows and is encompassed in a bright light.",ch,obj,0,TO_ROOM);
		act("As the light fades, $p has taken the form of a sword!",ch,obj,0,TO_CHAR);
		act("As the light around it fades, $n's $p has taken the form of a sword!",ch,obj,0,TO_ROOM);
		obj->value[0] = WEAPON_SWORD;
		obj->short_descr = str_dup("a finely-honed longsword named 'Victory'");
		obj->name = str_dup("victory longsword sword");
		obj->description = str_dup("The Sword of Victory lies here. A meticulously inscribed message is carved into it.");
		obj->condition = 91;
		obj->weight = 9;
	}
	if ((strstr(speech,"Victory to the axe!") || strstr(speech,"victory to the axe!")) && is_worn(obj) && obj->condition != 92)
	{
		act("$p glows and is encompassed in a bright light.",ch,obj,0,TO_CHAR);
		act("$n's $p glows and is encompassed in a bright light.",ch,obj,0,TO_ROOM);
		act("As the light fades, $p has taken the form of an axe!",ch,obj,0,TO_CHAR);
		act("As the light around it fades, $n's $p has taken the form of an axe!",ch,obj,0,TO_ROOM);
		obj->value[0] = WEAPON_AXE;
		obj->short_descr = str_dup("a mithril-edged decapitator named 'Victory'");
		obj->description = str_dup("The Axe of Victory lies here. A meticulously inscribed message is carved into it.");
		obj->name = str_dup("victory decapitator axe");
		obj->condition = 92;
		obj->weight = 11;
	}
	if ((strstr(speech,"Victory to the dagger!") || strstr(speech,"victory to the dagger!")) && is_worn(obj) && obj->condition != 93)
	{
		act("$p glows and is encompassed in a bright light.",ch,obj,0,TO_CHAR);
		act("$n's $p glows and is encompassed in a bright light.",ch,obj,0,TO_ROOM);
		act("As the light fades, $p has taken the form of a dagger!",ch,obj,0,TO_CHAR);
		act("As the light around it fades, $n's $p has taken the form of a dagger!",ch,obj,0,TO_ROOM);
		obj->value[0] = WEAPON_DAGGER;
		obj->short_descr = str_dup("a serrated mithril knife named 'Victory'");
		obj->description = str_dup("The Knife of Victory lies here. A meticulously inscribed message is carved into it.");
		obj->name = str_dup("victory dagger knife");
		obj->condition = 93;
		obj->weight = 6;
	}
	if ((strstr(speech,"Victory to the flail!") || strstr(speech,"victory to the flail!")) && is_worn(obj) && obj->condition != 94)
	{
		act("$p glows and is encompassed in a bright light.",ch,obj,0,TO_CHAR);
		act("$n's $p glows and is encompassed in a bright light.",ch,obj,0,TO_ROOM);
		act("As the light fades, $p has taken the form of a flail!",ch,obj,0,TO_CHAR);
		act("As the light around it fades, $n's $p has taken the form of a flail!",ch,obj,0,TO_ROOM);
		obj->value[0] = WEAPON_FLAIL;
		obj->short_descr = str_dup("a mithril-chained morning-star named 'Victory'");
		obj->description = str_dup("The Flail of Victory lies here. A meticulously inscribed message is carved into it.");
		obj->name = str_dup("victory morning-star flail");
		obj->condition = 94;
		obj->weight = 13;
	}
	if ((strstr(speech,"Victory to the mace!") || strstr(speech,"victory to the mace!")) && is_worn(obj) && obj->condition != 95)
	{
		act("$p glows and is encompassed in a bright light.",ch,obj,0,TO_CHAR);
		act("$n's $p glows and is encompassed in a bright light.",ch,obj,0,TO_ROOM);
		act("As the light fades, $p has taken the form of a mace!",ch,obj,0,TO_CHAR);
		act("As the light around it fades, $n's $p has taken the form of a mace!",ch,obj,0,TO_ROOM);
		obj->value[0] = WEAPON_MACE;
		obj->short_descr = str_dup("a spiked mace of reinforced mithril named 'Victory'");
		obj->description = str_dup("The Mace of Victory lies here. A meticulously inscribed message is carved into it.");
		obj->name = str_dup("victory mace");
		obj->condition = 95;
		obj->weight = 10;
	}
	if ((strstr(speech,"Victory to the polearm!") || strstr(speech,"victory to the polearm!")) && is_worn(obj) && obj->condition != 96)
	{
		act("$p glows and is encompassed in a bright light.",ch,obj,0,TO_CHAR);
		act("$n's $p glows and is encompassed in a bright light.",ch,obj,0,TO_ROOM);
		act("As the light fades, $p has taken the form of a polearm!",ch,obj,0,TO_CHAR);
		act("As the light around it fades, $n's $p has taken the form of a polearm!",ch,obj,0,TO_ROOM);
		obj->value[0] = WEAPON_POLEARM;
		obj->short_descr = str_dup("a slender poleaxe of shining mithril named 'Victory'");
		obj->description = str_dup("The Polearm of Victory lies here. A meticulously inscribed message is carved into it.");
		obj->name = str_dup("victory poleaxe polearm");
		obj->condition = 96;
		obj->weight = 20;
	}
	if ((strstr(speech,"Victory to the spear!") || strstr(speech,"victory to the spear!")) && is_worn(obj) && obj->condition != 97)
	{
		act("$p glows and is encompassed in a bright light.",ch,obj,0,TO_CHAR);
		act("$n's $p glows and is encompassed in a bright light.",ch,obj,0,TO_ROOM);
		act("As the light fades, $p has taken the form of a spear!",ch,obj,0,TO_CHAR);
		act("As the light around it fades, $n's $p has taken the form of a spear!",ch,obj,0,TO_ROOM);
		obj->value[0] = WEAPON_SPEAR;
		obj->short_descr = str_dup("a mithril dragon-lance named 'Victory'");
		obj->description = str_dup("The Spear of Victory lies here. A meticulously inscribed message is carved into it.");
		obj->name = str_dup("victory dragonlance dragon-lance spear");
		obj->condition = 97;
		obj->weight = 17;
	}
	if ((strstr(speech,"Victory to the staff!") || strstr(speech,"victory to the staff!")) && is_worn(obj) && obj->condition != 98)
	{
		act("$p glows and is encompassed in a bright light.",ch,obj,0,TO_CHAR);
		act("$n's $p glows and is encompassed in a bright light.",ch,obj,0,TO_ROOM);
		act("As the light fades, $p has taken the form of a staff!",ch,obj,0,TO_CHAR);
		act("As the light around it fades, $n's $p has taken the form of a staff!",ch,obj,0,TO_ROOM);
		obj->value[0] = WEAPON_STAFF;
		obj->short_descr = str_dup("a mithril-laced staff of fine wood named 'Victory'");
		obj->description = str_dup("The Staff of Victory lies here. A meticulously inscribed message is carved into it.");
		obj->name = str_dup("victory staff");
		obj->condition = 98;
		obj->weight = 14;
	}	
	if ((strstr(speech,"Victory to the whip!") || strstr(speech,"victory to the whip!")) && is_worn(obj) && obj->condition != 99)
	{
		act("$p glows and is encompassed in a bright light.",ch,obj,0,TO_CHAR);
		act("$n's $p glows and is encompassed in a bright light.",ch,obj,0,TO_ROOM);
		act("As the light fades, $p has taken the form of a whip!",ch,obj,0,TO_CHAR);
		act("As the light around it fades, $n's $p has taken the form of a whip!",ch,obj,0,TO_ROOM);
		obj->value[0] = WEAPON_WHIP;
		obj->short_descr = str_dup("a lightweight dragonscale whip named 'Victory'");
		obj->description = str_dup("The Whip of Victory lies here. A meticulously inscribed message is carved into it.");
		obj->name = str_dup("victory whip");
		obj->condition = 99;
		obj->weight = 8;
	}
	if ((strstr(speech,"Victory to the fist!") || strstr(speech,"victory to the fist!")) && is_worn(obj) && obj->condition != 100)
	{
		act("$p glows and is encompassed in a bright light.",ch,obj,0,TO_CHAR);
		act("$n's $p glows and is encompassed in a bright light.",ch,obj,0,TO_ROOM);
		act("As the light fades, $p has taken an odd, fist-like shape!",ch,obj,0,TO_CHAR);
		act("As the light around it fades, $n's $p has taken the form of an odd, fist-like shape!",ch,obj,0,TO_ROOM);
		obj->value[0] = WEAPON_EXOTIC;
		obj->short_descr = str_dup("an oddly-shaped steel weapon named 'Fist of Victory'");
		obj->description = str_dup("The Fist of Victory lies here. A meticulously inscribed message is carved into it.");
		obj->name = str_dup("victory fist");
		obj->condition = 100;
		obj->weight = 8;
	}
	
	return;
}

void pulse_prog_quest_reward_warrior_plate(OBJ_DATA *obj, bool isTick)
{
	CHAR_DATA *ch = obj->carried_by;

	if(!ch || IS_NPC(ch) || !is_worn(obj) || !isTick)
		return;
	if(!is_affected(ch,skill_lookup("valiant wrath")) && number_percent() < 30)
	{
		act("$p glows a deep red and you are suddenly filled with fervor and zeal!",ch,obj,0,TO_CHAR);
		act("$n's $p glows a deep red.",ch,obj,0,TO_ROOM);
		AFFECT_DATA af;
		init_affect(&af);
		af.aftype = AFT_POWER;
		af.type = skill_lookup("valiant wrath");
		af.duration = 3;
		af.location = APPLY_DAMROLL;
		af.modifier = 10;
		af.affect_list_msg = str_dup("raises damroll by 10 and modifies your strikes to deal Sound-elemental damage");
		af.level = ch->level;
		affect_to_char(ch,&af);
	}
	return;
}

void pulse_prog_quest_reward_warrior_gauntlets(OBJ_DATA *obj, bool isTick)
{
	CHAR_DATA *ch = obj->carried_by;

	if(!ch || IS_NPC(ch) || !is_worn(obj) || !isTick)
		return;
	if(!is_affected(ch,skill_lookup("spiderhands")) && number_percent() < 30)
	{
		act("You feel a burning sensation as $p melds with your gauntlets.",ch,obj,0,TO_CHAR);
		act("$n's weapon melds with $s gauntlets!",ch,obj,0,TO_ROOM);
		AFFECT_DATA af;
		init_affect(&af);
		af.aftype = AFT_POWER;
		af.type = skill_lookup("spiderhands");
		af.name = str_dup("weaponmeld");
		af.duration = number_range(1,2);
		af.affect_list_msg = str_dup("grants immunity to disarm attempts");
		affect_to_char(ch,&af);
	}
	return;
}

void pulse_prog_quest_reward_thief_robe(OBJ_DATA *obj, bool isTick)
{
	CHAR_DATA *ch = obj->carried_by;
	AFFECT_DATA af;

	if(!ch || IS_NPC(ch) || !is_worn(obj) || !isTick)
		return;
	if((is_affected(ch,skill_lookup("faerie fire")) || is_affected(ch,skill_lookup("faerie fog"))) && number_percent() < 33)
	{
		act("The faerie dust on your body is slowly devoured by $p.",ch,obj,0,TO_CHAR);
		affect_strip(ch,skill_lookup("faerie fire"));
		affect_strip(ch,skill_lookup("faerie fog"));
	}
	if(IS_AFFECTED(ch, AFF_HIDE) && !is_affected(ch,skill_lookup("shadowfeed")) && number_percent() < 20) {
		act("$p feeds upon the darkness of the shadows, strengthening you.",ch, obj,0,TO_CHAR);
		init_affect(&af);
		af.aftype = AFT_POWER;
		af.type = skill_lookup("shadowfeed");
		af.where = TO_AFFECTS;
		af.duration = 3;
		af.location = APPLY_DAMROLL;
		af.modifier = 5;
		affect_to_char(ch,&af);
		af.location = APPLY_HITROLL;
		af.modifier = 35;
		affect_to_char(ch,&af);
		
		if (number_percent() < 50) {
			if (ch->hit < ch->max_hit) {
				send_to_char("You feel healed as the shadows wash over you.\n\r",ch);
				ch->hit += (ch->max_hit - ch->hit) / 4;
			}
		}
	}
	return;
}

void wear_prog_quest_reward_channeler_helm (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou feel a rush of insight into the Mana Conduit skill!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("mana conduit")] = 100;
	return;
}


void wear_prog_quest_reward_channeler_robe (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou feel a rush of insight into the Mental Clarity skill!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("mental clarity")] = 100;
	return;
}

void wear_prog_quest_reward_channeler_neck (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou feel a rush of insight into the Mana Disruption spell!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("mana disruption")] = 100;
	return;
}

void remove_prog_quest_reward_channeler_helm (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou lose your Mana Conduit ability!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("mana conduit")] = -2;
	return;
}

void remove_prog_quest_reward_channeler_robe (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou lose your Mental Clarity ability!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("mental clarity")] = -2;
	return;
}

void remove_prog_quest_reward_channeler_neck (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou lose your Mana Disruption ability!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("mana disruption")] = -2;
	return;
}

void invoke_prog_quest_reward_channeler_ankh (OBJ_DATA *obj, CHAR_DATA *ch, char* argument)
{	
	if (ch->fighting != NULL)
		return send_to_char("You are fighting, you can't invoke this now!\n\r",ch);
	if (is_affected(ch,skill_lookup("displacement")))
		return send_to_char("You are already masking your location.\n\r",ch);

	AFFECT_DATA af;
	init_affect(&af);
	af.aftype = AFT_POWER;
	af.where = TO_AFFECTS;
	af.type   = skill_lookup("displacement");
	af.duration = 10;
	af.affect_list_msg = str_dup("grants a chance to completely avoid attacks");
	af.level = 55;
	affect_to_char(ch,&af);
	act("You hold $p into the air and invoke its magic!",ch,obj,0,TO_CHAR);
	act("You feel a strange sensation as you pass between two dimensions, displacing yourself.",ch,0,0,TO_CHAR);
	act("$n holds $p into the air and invokes its magic!",ch,obj,0,TO_ROOM);
	act("$n fades slightly out of existence.",ch,0,0,TO_ROOM);
	return;
}

void wear_prog_quest_reward_thief_belt (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou feel a rush of insight into Plant Explosive!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("plant explosive")] = 100;
	return;
}

void remove_prog_quest_reward_thief_belt (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou lose your ability to Plant Explosives!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("plant explosive")] = -2;
	return;
}

void wear_prog_quest_reward_healer_ring (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou feel a rush of insight into Resurrect!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("resurrection")] = 100;
	return;
}

void remove_prog_quest_reward_healer_ring (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou lose your ability to Resurrect!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("resurrection")] = -2;
	return;
}

void wear_prog_quest_reward_healer_bracelet (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou feel a rush of insight into Greater Regeneration!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("greater regeneration")] = 100;
	return;
}

void remove_prog_quest_reward_healer_bracelet (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou lose your Greater Regeneration ability!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("greater regeneration")] = -2;
	return;
}

void wear_prog_quest_reward_healer_sleeves (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou feel a rush of insight into Divine Transferrance!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("divine transferrance")] = 100;
	return;
}

void remove_prog_quest_reward_healer_sleeves(OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou lose your Divine Transferrance ability!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("divine transferrance")] = -2;
	return;
}

void wear_prog_quest_reward_elementalist_neck (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou feel a rush of insight into Elemental Affinity!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("elemental affinity")] = 100;
	return;
}

void remove_prog_quest_reward_elementalist_neck(OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou lose your Elemental Affinity ability!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("elemental affinity")] = -2;
	return;
}

void wear_prog_quest_reward_elementalist_helm (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou feel a rush of insight into Shatter Armor!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("shatter armor")] = 100;
	return;
}

void remove_prog_quest_reward_elementalist_helm(OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou lose your Shatter Armor ability!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("shatter armor")] = -2;
	return;
}

void wear_prog_quest_reward_elementalist_belt (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou feel a rush of insight into Hurricane!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("hurricane")] = 100;
	return;
}

void remove_prog_quest_reward_elementalist_belt(OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou lose your Hurricane ability!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("hurricane")] = -2;
	return;
}

void invoke_prog_quest_reward_healer_statue(OBJ_DATA *obj, CHAR_DATA *ch, char* argument)
{
	AFFECT_DATA af;
	
	if (is_affected(ch, skill_lookup("sacrifice")))
		return send_to_char("The gods will not yet grant you shelter.\n\r",ch);
		
	act("You invoke the power contained in $p!",ch,obj,0,TO_CHAR);
	act("$n invokes the power contained in $p!",ch,obj,0,TO_ROOM);
	act("A bolt of lightning strikes you down!",ch,obj,0,TO_CHAR);
	act("$n is struck by a bolt of lightning and disappears in a puff of smoke!",ch,obj,0,TO_ROOM);
	
	if (ch->fighting != NULL)
		stop_fighting(ch, TRUE);
		
	char_from_room(ch);
	char_to_room(ch,get_room_index(ROOM_VNUM_STATUE_SACRIFICE));
	
	act("Your sacrifice delivers you into the sanctum of the gods, but has taken its toll.",ch,0,0,TO_CHAR);
	act("$n arrives and collapses in shock.",ch,0,0,TO_ROOM);
	
	init_affect(&af);
	af.aftype = AFT_POWER;
	af.where = TO_AFFECTS;
	af.type = skill_lookup("sacrifice");
	af.duration = 60;
	af.affect_list_msg = str_dup("unable to invoke the power of Sacrifice");
	affect_to_char(ch,&af);
	ch->hit = -2;
	ch->mana = -100;
	ch->move = -100;
	ch->position = POS_STUNNED;
	
	return;
}

void wear_prog_quest_reward_thief_boots (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou feel a rush of insight into Advanced Tripwire!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("advanced tripwire")] = 100;
	return;
}

void remove_prog_quest_reward_thief_boots (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou lose your Advanced Tripwire ability!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("advanced tripwire")] = -2;
	return;
}

void wear_prog_quest_reward_thief_robe (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou feel a rush of insight into Shadowstep!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("shadowstep")] = 100;
	return;
}

void remove_prog_quest_reward_thief_robe (OBJ_DATA *obj, CHAR_DATA *ch)
{
	act("{yYou lose your Shadowstep ability!{x",ch,0,0,TO_CHAR);
	ch->pcdata->learned[skill_lookup("shadowstep")] = -2;
	return;
}

void wear_prog_rune_dagger(OBJ_DATA *obj, CHAR_DATA *ch)
{
        send_to_char("You feel the power of your magic strengthen.\n\r",ch);
}

void remove_prog_rune_dagger(OBJ_DATA *obj, CHAR_DATA *ch)
{
        send_to_char("Your feeling of mental fortitude fades away.\n\r",ch);
}

void fight_prog_cloak_embrace(OBJ_DATA *obj, CHAR_DATA *ch)
{
        if(!is_worn(obj))
                return;

        if (ch->cabal == CABAL_RAGER)
                return;

        if(number_percent()>94)
        {
                act("$p glows with amber light.",ch,obj,0,TO_ALL);
                ch->hit = UMIN(ch->hit + 150, ch->max_hit);
        }
}

void wear_prog_cloak_displacement(OBJ_DATA *obj, CHAR_DATA *ch)
{
	AFFECT_DATA af;

	if (ch->cabal == cabal_lookup("rager"))
		return;

	if (is_affected(ch, gsn_displacement))
		return;

	act("Your bodily form begins to flicker.", ch, 0, 0, TO_CHAR);
	act("$n's bodily form begins to flicker.", ch, 0, 0, TO_ROOM);

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= gsn_displacement;
	af.aftype	= AFT_SPELL;
	af.duration	= -1;
	af.level 	= ch->level;
	af.modifier	= 0;
	af.bitvector	= 0;
	af.location	= 0;
	af.name		= str_dup("displacement");
	affect_to_char(ch, &af);
	return;
}

void remove_prog_cloak_displacement(OBJ_DATA *obj, CHAR_DATA *ch)
{
	if (!is_affected(ch, gsn_displacement))
		return;

	act("You feel more solid.", ch, 0, 0, TO_CHAR);
	act("$n looks more solid.", ch, 0, 0, TO_ROOM);
	affect_strip(ch, gsn_displacement);
	return;
}
	
void fight_prog_tattoo_tracerah(OBJ_DATA *obj, CHAR_DATA *ch)
{
	CHAR_DATA *victim = ch->fighting;
	AFFECT_DATA af;

        if(!is_worn(obj) || !victim)
                return;

	if (number_percent() > 96)
	{
		if (is_affected(victim, gsn_confuse))
			return;

		act("{MYour heart races and you begin to panic as an intense feeling of delirium overcomes you!{x", ch, 0, victim, TO_VICT);
		act("{MAn expression of panic overcomes $N.{x", ch, 0, victim, TO_NOTVICT);
		act("{MAn expression of panic overcomes $N.{x", ch, 0, victim, TO_CHAR);
		init_affect(&af);
		af.where	= TO_AFFECTS;
		af.type		= gsn_confuse;
		af.aftype	= AFT_MALADY;
		af.level	= ch->level;
		af.modifier	= 0;
		af.location	= 0;
		af.duration	= 12;
		af.bitvector	= AFF_LOOKING_GLASS;
		af.name		= str_dup("delirium");
		affect_to_char(victim, &af);
		if (!is_affected(victim,gsn_deafen))
			obj_cast_spell(skill_lookup("deafen"), 60, ch, victim, obj);
	} else if (number_percent() > 94) {
		obj_cast_spell(skill_lookup("chain lightning"), 60, ch, victim, obj);
	}
	return;
}

void invoke_prog_charm_ivory_knowledge(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	int lsn[5] = {-1};
	int i = 0;
	int j = 0;
	int sn = 0;
	int skills_known = 0;
	int gain = 0;
	char buf[MSL];
	bool found = FALSE;

	for (i=0; i<MAX_SKILL; i++) {
		if (ch->pcdata->learned[i] > 1 && ch->pcdata->learned[i] < 100)
			skills_known++;
	}

	// Make sure the character has at least 5 practiced, unperfected skills
	if (skills_known < 5)
		return send_to_char("You are already too knowledgeable enough in your craft to make use of this charm.\n\r", ch);

	i = 0;

	// In theory this (and get_random_skill) could cause an infinite loop
	while (i < 5) {
		sn = get_rand_skill(ch, TRUE);

		found = FALSE;

		for (j=0; j<=i; j++) {
			if (lsn[j] == sn) found = TRUE;
		}

		if (found) continue;

		lsn[i] = sn;
		i++;
	}

	// Reward time
	for (i=0; i<5; i++) {
		gain = number_range(1, 5);

		if (ch->pcdata->learned[lsn[i]] + gain > 100)
			gain = 100 - ch->pcdata->learned[lsn[i]];

		ch->pcdata->learned[lsn[i]] = UMIN(ch->pcdata->learned[lsn[i]] + gain, 100);
		sprintf(buf, "You have learned %d%% of your %s ability.\n\r", gain, skill_table[lsn[i]].name);
		send_to_char(buf, ch);
	}

	act("$p explodes into tiny fragments of ivory.", ch, obj, 0, TO_CHAR);
	extract_obj(obj);
	return;
}

void invoke_prog_charm_earthen_vitality(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= skill_lookup("vitality");
	af.aftype	= AFT_SKILL;
	af.location	= APPLY_HIT;
	af.duration	= -1;
	af.bitvector	= 0;
	af.modifier	= ch->max_hit * 0.15;
	af.level	= ch->level;
	affect_to_char(ch, &af);

	send_to_char("You will be more healthy until your next death.\n\r", ch);
	act("$p explodes into tiny fragments of clay.", ch, obj, 0, TO_CHAR);
	extract_obj(obj);
	return;
}

void invoke_prog_charm_dragonbone_strength(OBJ_DATA *obj, CHAR_DATA *ch, char *argument)
{
	AFFECT_DATA af;

	init_affect(&af);
	af.where	= TO_AFFECTS;
	af.type		= skill_lookup("dragon might");
	af.aftype	= AFT_SKILL;
	af.location	= APPLY_NONE;
	af.duration	= -1;
	af.bitvector	= 0;
	af.modifier	= 0;
	af.level	= ch->level;
	af.affect_list_msg = str_dup("grants invariable strength");
	affect_to_char(ch, &af);

	send_to_char("The might of the dragon-kind strengthens your arms.\n\r", ch);
	act("$p explodes into tiny fragments of dragonbone.", ch, obj, 0, TO_CHAR);
	extract_obj(obj);
	return;	
}

void fight_prog_training_pendant(OBJ_DATA *obj, CHAR_DATA *ch)
{
	if (IS_NPC(ch)) return;
	if (!is_worn(obj)) return;
	if (ch->pcdata->newbie != TRUE) return;
	if (ch->in_room->area == NULL || str_cmp(ch->in_room->area->name, "Training Grounds")) return;

	if (number_percent() < 20)
	{
		act("$n's $p glows bright blue!",ch,obj,0,TO_ROOM);
		act("$p glows bright blue!",ch,obj,0,TO_CHAR);
		obj_cast_spell(skill_lookup("heal"), obj->level, ch, ch, obj);
	}
}
