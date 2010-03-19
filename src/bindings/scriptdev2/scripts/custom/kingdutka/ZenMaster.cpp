///////////////////////////////////////
//  ZenMaster v1.0.0
//  by Nicolai Dutka
//  http://nicolaidutka.archongames.com
///////////////////////////////////////

#include "precompiled.h"
#include "../../../shared/Config/Config.h"
#include "../config.h"
#include "Database/DatabaseEnv.h"

extern Config SD2Config;
extern std::string CopperToGold(uint32 copper);

void SummonTrainer_ZenMaster(Player* Player, Creature* Creature){
	// Make sure we can access the Config file
	if(!SD2Config.SetSource(_SCRIPTDEV2_CONFIG,true)){
		Player->CLOSE_GOSSIP_MENU();
		error_log("ZenMaster: Unable to open configuration file");
		Creature->MonsterWhisper("I'm sorry, we are having technical difficulties.  Please check back later.", Player->GetGUID());
		return;
	}
	// Not allow in combat
	if (Player->isInCombat()){
		Player->CLOSE_GOSSIP_MENU();
		Creature->MonsterSay("You are in combat!", LANG_UNIVERSAL, NULL);
		return;
	}
	if(Player->GetMoney()<SD2Config.GetFloatDefault("ZenMaster.Cost",0)){
		if(Creature>0) Creature->MonsterWhisper("You don't have enough money.",Player->GetGUID());
		return;
	}
	Player->ModifyMoney(-(SD2Config.GetFloatDefault("ZenMaster.Cost",0)));
	uint32 CreatureID = 0;
	bool A = false;
	if(Player->GetTeam()==ALLIANCE) A = true;
	switch(Player->getClass()){
		case CLASS_DEATH_KNIGHT: CreatureID = 92006; break;
		case CLASS_DRUID:		 CreatureID = 92011; break;
		case CLASS_HUNTER:		 CreatureID = 92003; break;
		case CLASS_MAGE:		 CreatureID = 92008; break;
		case CLASS_PALADIN:		 CreatureID = 92002; break;
		case CLASS_PRIEST:		 CreatureID = 92005; break;
		case CLASS_ROGUE:		 CreatureID = 92004; break;
		case CLASS_SHAMAN:		 CreatureID = 92007; break;
		case CLASS_WARLOCK:		 CreatureID = 92009; break;
		case CLASS_WARRIOR:		 CreatureID = 92001; break;
	}
	Player->SummonCreature(CreatureID, Player->GetPositionX(), Player->GetPositionY(), Player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 40000);
	Player->CLOSE_GOSSIP_MENU();
}

bool GossipHello_ZenMaster(Player* Player, Creature* Creature)
{
	error_log("ZenMaster: DK: \"%i\" DRUID: \"%i\" HUNTER: \"%i\" MAGE: \"%i\" PALADIN: \"%i\" PRIEST: \"%i\" ROGUE: \"%i\" SHAMAN: \"%i\" WARLOCK: \"%i\" WARRIOR: \"%i\"",CLASS_DEATH_KNIGHT,CLASS_DRUID,CLASS_HUNTER,CLASS_MAGE,CLASS_PALADIN,CLASS_PRIEST,CLASS_ROGUE,CLASS_SHAMAN,CLASS_WARLOCK,CLASS_WARRIOR);
	// Make sure we can access the Config file
	if(!SD2Config.SetSource(_SCRIPTDEV2_CONFIG,true)){
		Player->CLOSE_GOSSIP_MENU();
		error_log("ZenMaster: Unable to open configuration file");
		Creature->MonsterWhisper("I'm sorry, we are having technical difficulties.  Please check back later.", Player->GetGUID());
		return false;
	}
	if(SD2Config.GetBoolDefault("ZenMaster.OnlyGMs", false)) // If ZenMaster.OnlyGMs is enabled in scriptdev2.conf
		if (Player->GetSession()->GetSecurity() == SEC_PLAYER){
			Creature->MonsterWhisper("Sorry, I can only talk to game masters.", Player->GetGUID());
			return true;
		}
	std::stringstream MenuItem;
	MenuItem<<"Summon Class Trainer - "<<CopperToGold(SD2Config.GetFloatDefault("ZenMaster.Cost",0));
	Player->ADD_GOSSIP_ITEM( 5, MenuItem.str().c_str() , GOSSIP_SENDER_MAIN, 1000);
	Player->ADD_GOSSIP_ITEM( 7, "No Thanks...", GOSSIP_SENDER_MAIN, 0);
	Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,Creature->GetGUID());
return true;
}

void SendDefaultMenu_ZenMaster(Player* Player, Creature* Creature, uint32 Action)
{
	//Make sure player actually clicked the summon button
	if(Action==1000){
		outstring_log("Zen Master: Attempting to summon a Zen Master");
		SummonTrainer_ZenMaster(Player,Creature);
		return;
	}
	else{
		Player->CLOSE_GOSSIP_MENU();
	}
}

bool GossipSelect_ZenMaster(Player* Player, Creature* Creature, uint32 uiSender, uint32 uiAction)
{
	// Main menu
	if (uiSender == GOSSIP_SENDER_MAIN)
	SendDefaultMenu_ZenMaster(Player, Creature, uiAction);
	return true;
}

// Summon Stone
bool ItemUse_ZenMaster_SummonStone(Player *Player, Item* pItem, const SpellCastTargets &pTargets)
{
	bool EnableStone = SD2Config.GetBoolDefault("ZenMaster.EnableSummonStone", true);
	 if (Player->isInFlight() || Player->isDead() || Player->isInCombat() || !EnableStone)
        return true;
	 outstring_log("Zen Master Summon Stone: Attempting to summon a Zen Master");
	 Creature* ZenMaster = Player->SummonCreature(90010, Player->GetPositionX(), Player->GetPositionY(), Player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000);
	 return false;
}

void AddSC_ZenMaster()
{
    Script *newscript;

	newscript                  = new Script;
	newscript->Name            = "ZenMaster";
	newscript->pGossipHello    = &GossipHello_ZenMaster;
	newscript->pGossipSelect   = &GossipSelect_ZenMaster;
	newscript->RegisterSelf();

	newscript                  = new Script;
	newscript->Name            = "ZenMasterItem";
	newscript->pItemUse        = &ItemUse_ZenMaster_SummonStone;
	newscript->RegisterSelf();
}