///////////////////////////////////////
//  TeleportMaster v4.0.0
//  by Nicolai Dutka
//  http://nicolaidutka.archongames.com
///////////////////////////////////////

#include "precompiled.h"
#include "../../../shared/Config/Config.h"
#include "../config.h"
#include "Database/DatabaseEnv.h"

extern DatabaseType SD2Database;
extern Config SD2Config;
extern std::string CopperToGold(uint32 copper);

void ProcessTeleport_TeleportMaster(Player *player, Creature *Creature, uint32 action){
	QueryResult* pResult = SD2Database.PQuery("SELECT ID,Cost,MapID,Xpos,Ypos,Zpos,Rpos,Name FROM teleportmaster_locations ORDER BY id");
		if(pResult){
			do{
				Field* pFields = pResult->Fetch();
				// Make sure this row is the teleport location we want
				if(pFields[0].GetInt32() == action){
					if(player->GetMoney() < pFields[1].GetUInt32()){
							std::stringstream ss;
							ss << "You do not have enough money to teleport here.  You need " << CopperToGold(pFields[1].GetUInt32()) <<".";
							Creature->MonsterWhisper(ss.str().c_str(), player->GetGUID());
					}
					else{
						// Player has enough money! Do the teleport!
						std::stringstream ss;
						ss << "You spent " << CopperToGold(pFields[1].GetUInt32()) <<" on your teleport to " << pFields[7].GetCppString() << ".";
						Creature->MonsterWhisper(ss.str().c_str(), player->GetGUID());
						player->ModifyMoney(-pFields[1].GetInt32());
						player->CLOSE_GOSSIP_MENU();
						player->TeleportTo(pFields[2].GetInt32(),pFields[3].GetFloat(),pFields[4].GetFloat(),pFields[5].GetFloat(),pFields[6].GetFloat());
					}
				}
			}while(pResult->NextRow());
		return;
		}
}

void ProcessMenu_TeleportMaster(Player *player, Creature *Creature, uint32 action, uint32 MoreLess, uint32 GroupID){
	error_log("TeleportMaster: Running ProcessMenu - Action: \"%i\" MoreLess: \"%i\" GroupID: \"%i\"",action,MoreLess,GroupID);
	
	//Initialize
	int32 ItemCount = 0;
	int32 locations = 0;
	int32 throttle  = 0;
	QueryResult* pResult;
	if(action==100000) action=0;
	if(action<=1000) throttle = 1000;
	if(action<=100)  throttle = 100;
	if(GroupID>0)    throttle = 1000;

	// Query the DB 
	if( action<=1000 ){
		pResult = SD2Database.PQuery("SELECT CategoryID,CategoryName,SubCatGroupID,SubCatName FROM teleportmaster_categories WHERE CategoryID >= \"%i\" AND CategoryID <= \"%i\" ORDER BY CategoryID", action, throttle);	
		//Not a SubCat, list out locations
		if( pResult->Fetch()[2].GetInt32()==0 && (action>100 || (action<=100 && action>0 && MoreLess==0)) ){
			pResult = SD2Database.PQuery("SELECT ID,CategoryID,Name,faction,ReqLevel,GuildID,Cost FROM teleportmaster_locations WHERE CategoryID = \"%i\" ORDER BY ID", action);
			locations = 1;
		}
		else if(action<=1000 && action>1){
			if(MoreLess>0 || action<=100){
				if(action<=100){
					pResult = SD2Database.PQuery("SELECT CategoryID,CategoryName,SubCatGroupID,SubCatName FROM teleportmaster_categories WHERE CategoryID >= \"%i\" AND CategoryID <= \"%i\" ORDER BY CategoryID", action, throttle);	
				}
				else{
					QueryResult* pResultGroup = SD2Database.PQuery("SELECT CategoryID,CategoryName,SubCatGroupID,SubCatName FROM teleportmaster_categories WHERE CategoryID = \"%i\"", action);	
					if(pResultGroup){
						GroupID = pResultGroup->Fetch()[2].GetInt32();
						pResult = SD2Database.PQuery("SELECT CategoryID,CategoryName,SubCatGroupID,SubCatName FROM teleportmaster_categories WHERE CategoryID >= \"%i\" AND SubCatGroupID = \"%i\" ORDER BY CategoryID", action, GroupID);	
					}
				}
			}
			else{
				//List what is INSIDE this sub-group
				pResult = SD2Database.PQuery("SELECT ID,CategoryID,Name,faction,ReqLevel,GuildID,Cost FROM teleportmaster_locations WHERE CategoryID = \"%i\" ORDER BY ID", action);
				locations = 1;
			}
		}
	}
	if(action>1000){
		if(MoreLess>0){
			QueryResult* pResultGroup = SD2Database.PQuery("SELECT ID,CategoryID FROM teleportmaster_locations WHERE ID = \"%i\"", action);
			if(pResultGroup){
				GroupID = pResultGroup->Fetch()[1].GetInt32();
				pResult = SD2Database.PQuery("SELECT ID,CategoryID,Name,faction,ReqLevel,GuildID,Cost FROM teleportmaster_locations WHERE CategoryID = \"%i\" AND ID>= \"%i\" ORDER BY ID", GroupID, action);
				locations = 1;
			}
		}
		else{
			ProcessTeleport_TeleportMaster( player, Creature, action);
			return;
		}
	}
	
	// Make sure the result is valid, add menu items
	if(pResult){
		error_log("TeleportMaster: ProcessMenu queried the DB and got results, processing results...");
		do{
			Field* pFields = pResult->Fetch();
			if(ItemCount==10){
				//Count is 10! We need a 'more' button!
				error_log("TeleportMaster: 10 Items on menu, adding 'more' button: \"%i\"", pFields[0].GetInt32() + 200000);
				player->ADD_GOSSIP_ITEM( 4, "More ->", GOSSIP_SENDER_MAIN,  pFields[0].GetInt32() + 200000);
				ItemCount++;
			}
			else{
				std::stringstream CatName;
				//MainCat Items
				if(action<101 && !locations && GroupID==0){
						CatName<<pFields[1].GetCppString()<<" ->";
						player->ADD_GOSSIP_ITEM( 5, CatName.str().c_str(), GOSSIP_SENDER_MAIN, pFields[0].GetInt32());
						ItemCount++;
				}
				//SubCat Items
				if(action<1001 && GroupID>0 && pFields[0].GetInt32()>100 && pFields[2].GetInt32()==GroupID && !locations){
						CatName<<pFields[3].GetCppString()<<" ->";
						player->ADD_GOSSIP_ITEM( 5, CatName.str().c_str(), GOSSIP_SENDER_MAIN, pFields[0].GetInt32());
						ItemCount++;
				}
				//Locations or Root SubCats
				if(locations){
					if(GroupID==0 || action>1000 || (action<=1000 && MoreLess==0) ){
						if( (pFields[3].GetInt32()==2 && player->GetTeam()==ALLIANCE) || (pFields[3].GetInt32()==1 && player->GetTeam()==HORDE) || (player->getLevel()<pFields[4].GetUInt32()) || (pFields[5].GetInt32()>0 && player->GetGuildId() != pFields[5].GetInt32()) ){
								//FAIL!!  Do nothing...
						}
						else{
							std::stringstream menuItem;
							menuItem << pFields[2].GetCppString() << " - " << CopperToGold(pFields[6].GetInt32());
							player->ADD_GOSSIP_ITEM( 2, menuItem.str().c_str(), GOSSIP_SENDER_MAIN, pFields[0].GetInt32());
							ItemCount++;
						}
					}
					else{
						CatName<<pFields[1].GetCppString()<<" ->";
						player->ADD_GOSSIP_ITEM( 5, CatName.str().c_str(), GOSSIP_SENDER_MAIN, pFields[0].GetInt32());
						ItemCount++;
					}
				}
			}
		}while(pResult->NextRow() && ItemCount<11);
	}
	
	//scan teleportmaster_locations for locations matching this groupID and add them if ItemCount<10
	if( GroupID>0 && !locations && ItemCount<10 ){
		pResult = SD2Database.PQuery("SELECT ID,CategoryID,faction,ReqLevel,GuildID,Name,cost FROM teleportmaster_locations WHERE CategoryID = \"%i\" ORDER BY ID",action); 
		if(pResult){
			error_log("TeleportMaster: Adding additional locations to a SubCat");
			do{
				Field* pFields = pResult->Fetch();
				if(ItemCount==10){
					//Count is 10! We need a 'more' button!
					error_log("TeleportMaster: 10 Items on menu, adding 'more' button: \"%i\"", pFields[0].GetInt32() + 200000);
					player->ADD_GOSSIP_ITEM( 4, "More ->", GOSSIP_SENDER_MAIN,  pFields[0].GetInt32() + 200000);
					ItemCount++;
				}
				if( (pFields[2].GetInt32()==2 && player->GetTeam()==ALLIANCE) || (pFields[2].GetInt32()==1 && player->GetTeam()==HORDE) || (player->getLevel()<pFields[3].GetUInt32()) || (pFields[4].GetInt32()>0 && player->GetGuildId() != pFields[4].GetInt32()) ){
					//FAIL!!  Do nothing...
				}
				else{
					std::stringstream menuItem;
					menuItem << pFields[5].GetCppString() << " - " << CopperToGold(pFields[6].GetInt32());
					player->ADD_GOSSIP_ITEM( 2, menuItem.str().c_str(), GOSSIP_SENDER_MAIN, pFields[0].GetInt32());
					ItemCount++;
				}
			}while(pResult->NextRow() && ItemCount<11);
		}
	}
		
	//Add 'Prev' and 'Main Menu' buttons as needed
	error_log("TeleportMaster: Adding 'Prev' and 'Main Menu' buttons for action: \"%i\" GroupId:\"%i\"",action,GroupID);
	if(MoreLess>0 || locations){
		if(action<2){
			if(MoreLess==0)
				player->ADD_GOSSIP_ITEM( 4, "<- Main Menu", GOSSIP_SENDER_MAIN,  100000 );
		}
		else{
			int32 prevAction=0;
			QueryResult* pResultPrev;
			if(action>=1000)
				pResultPrev = SD2Database.PQuery("SELECT ID,CategoryID,faction,ReqLevel,GuildID FROM teleportmaster_locations WHERE CategoryID = \"%i\" AND ID <= \"%i\" ORDER BY ID DESC",GroupID,action);	
			if(action>=100 && action<1000)
				pResultPrev = SD2Database.PQuery("SELECT CategoryID,SubCatGroupID FROM teleportmaster_categories WHERE SubCatGroupID = \"%i\" AND CategoryID <= \"%i\" ORDER BY CategoryID DESC",GroupID,action);
			if(action>1 && action<100)
				pResultPrev = SD2Database.PQuery("SELECT CategoryID,SubCatGroupID FROM teleportmaster_categories WHERE CategoryID <= \"%i\" ORDER BY CategoryID DESC",action);
			
			// Make sure the result is valid
			if(pResultPrev){
				int32 count=0;
				//Count backwards starting at our last item and only count valid locations
				do{
					Field* pFields = pResultPrev->Fetch();
					if(action>1000){
						if( (pFields[2].GetInt32()==2 && player->GetTeam()==ALLIANCE) || (pFields[2].GetInt32()==1 && player->GetTeam()==HORDE) || (player->getLevel()<pFields[3].GetUInt32()) || (pFields[4].GetInt32()>0 && player->GetGuildId() != pFields[4].GetInt32()) ){
							//FAIL!!  Do nothing...
						}
						else{
							count++;
						}
					}
					else{
						count++;
					}
					if(count==11)
						prevAction = pFields[0].GetInt32();
				}while(pResultPrev->NextRow());
				
			}
			if(prevAction>0){
				player->ADD_GOSSIP_ITEM( 4, "<- Prev", GOSSIP_SENDER_MAIN, prevAction + 400000 );
				error_log("TeleportMaster: Prev1: \"%i\"", prevAction + 400000);
			}
			player->ADD_GOSSIP_ITEM( 4, "<- Main Menu", GOSSIP_SENDER_MAIN,  100000 );
		}
	}
	else if(action<=1000 && action>0 && MoreLess==0){
		if( GroupID==0 || (action<100 && GroupID>0) )
			player->ADD_GOSSIP_ITEM( 4, "<- Main Menu", GOSSIP_SENDER_MAIN,  100000 );
	}
	player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,Creature->GetGUID());
	return;
}

bool GossipHello_TeleportMaster(Player *player, Creature *Creature)
{
	// Make sure we can access the Config file
	if(!SD2Config.SetSource(_SCRIPTDEV2_CONFIG,true)){
		player->CLOSE_GOSSIP_MENU();
		error_log("TeleportMaster: Unable to open configuration file");
		Creature->MonsterWhisper("I'm sorry, we are having technical difficulties.  Please check back later.", player->GetGUID());
		return false;
	}
	// Make sure player is not in combat
	if(!player->getAttackers().empty()){
		player->CLOSE_GOSSIP_MENU();
		Creature->MonsterWhisper("You are in combat!", player->GetGUID());
		return false;
	}
	// If enabled, make sure only GM can use TelePorts
	if(SD2Config.GetBoolDefault("TeleportMaster.OnlyGMs", false)){
		if(player->GetSession()->GetSecurity() == SEC_PLAYER){
			player->CLOSE_GOSSIP_MENU();
			Creature->MonsterWhisper("Sorry, I am only allowed to teleport game masters.", player->GetGUID());
			return false;
		}
	}
	ProcessMenu_TeleportMaster(player, Creature,0,0,0);
	return true;
}

void SendDefaultMenu_TeleportMaster(Player *player, Creature *Creature, uint32 action)
{
	int32 ItemCount = 0;			//Track how many items we have on the menu so far
	int32 MoreLess = 0;				//Track if we possibly just clicked a 'more' or 'prev' button
	int32 actionID = action;		//Track our initial action code when we got here

	if(action>400000)
		action-=100000;
	if(action>300000){
		action-=300000;
		MoreLess = 2;	//This was a 'prev' button click
	}
	if(action>200000)
		action-=100000;
	if(action>100000){
		action-=100000;
		MoreLess = 1;   //This was a 'more' button click
	}
	error_log("TeleportMaster: Processing Default Menu using 'action': \"%i\"  ActionID: \"%i\" MoreLess: \"%i\" ",action,actionID,MoreLess);

	// Main Menu Return
	if(action==100000){
	    ProcessMenu_TeleportMaster(player,Creature,action,0,0);
		return;
	}//End Main Menu Return

	if(MoreLess>0){
		ProcessMenu_TeleportMaster(player,Creature,action,MoreLess,0);
		return;
	}
	//Sub-Groups
	//Check for Group ID Number if we find a match, we have a sub-group
	int32 Group = 0;
	QueryResult* pResult = SD2Database.PQuery("SELECT CategoryID,SubCatGroupID FROM teleportmaster_categories ORDER BY CategoryID");	
	// Make sure the result is valid
	if(pResult){
		do{
			Field* pFields = pResult->Fetch();
			if(pFields[0].GetInt32()==action){  //Magic check
				Group = pFields[1].GetInt32();
			}
		}while(pResult->NextRow());
	}
	if(Group>0){
		ProcessMenu_TeleportMaster(player,Creature,action,MoreLess,Group);
	}
	else{
		ProcessMenu_TeleportMaster(player,Creature,action,MoreLess,0);
	}
}//End SendDefaultMenu_TeleportMaster

bool GossipSelect_TeleportMaster(Player *player, Creature *Creature, uint32 sender, uint32 action)
{
	// Main menu
	if (sender == GOSSIP_SENDER_MAIN)
	SendDefaultMenu_TeleportMaster( player, Creature, action);

return true;
}

// Summon Stone
bool ItemUse_TeleportMaster_SummonStone(Player *Player, Item* pItem, const SpellCastTargets &pTargets)
{
	 if (Player->isInFlight() || Player->isDead() || Player->isInCombat())
        return true;

	 outstring_log("Attempting to summon a Teleport Master");

	 Creature* TeleportMaster = Player->SummonCreature(90000, Player->GetPositionX(), Player->GetPositionY(), Player->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 20000);
	 return false;
}

void AddSC_TeleportMaster()
{
    Script *newscript;
	newscript                  = new Script;
	newscript->Name            = "TeleportMaster";
	newscript->pGossipHello    = &GossipHello_TeleportMaster;
	newscript->pGossipSelect   = &GossipSelect_TeleportMaster;
	newscript->RegisterSelf();
	
	newscript                  = new Script;
	newscript->Name            = "TeleportMasterItem";
	newscript->pItemUse        = &ItemUse_TeleportMaster_SummonStone;
	newscript->RegisterSelf();
}