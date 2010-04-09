#include "precompiled.h"

bool GossipHello_npc_custom_items(Player* pPlayer, Creature* pCreature)
{
    pPlayer->ADD_GOSSIP_ITEM( 5, "25 EOC for Custom Weapon", GOSSIP_SENDER_MAIN, 2000);
    /*if(pPlayer->getLevel() < 255)
    {
        pPlayer->ADD_GOSSIP_ITEM( 5, "40 BOJ Exchange 1 Level"           , GOSSIP_SENDER_MAIN, 3000);
    }
    pPlayer->ADD_GOSSIP_ITEM( 5, "20 BOJ Exchange 36 Slot Bag"     , GOSSIP_SENDER_MAIN, 4000);
	pPlayer->ADD_GOSSIP_ITEM(5,"5 BOJ Exchange 1 Buff Spell"		,GOSSIP_SENDER_MAIN,5000);
	pPlayer->ADD_GOSSIP_ITEM(5,"10 BOJ Exchange 1 Item Summon Trainer"	,GOSSIP_SENDER_MAIN,6000);
	pPlayer->ADD_GOSSIP_ITEM(5,"10 BOJ Exchange 1 Item Summon Professional" ,GOSSIP_SENDER_MAIN,7000);
	pPlayer->ADD_GOSSIP_ITEM(5,"10 BOJ Exchange 1 Item Summon Tele NPC"	,GOSSIP_SENDER_MAIN,8000);
	*/
    pPlayer->SEND_GOSSIP_MENU(3961,pCreature->GetGUID());
    return true;    
}

void Add_Item_to_player(Player* aPlayer,Creature* aCreature,uint32 aItem)
{
	uint32 itemEOC = 45624; //Emblem of Conquest
	int32 count = 0;
	int32 count2 = 1;
	uint32 noSpaceForCount = 0;
	ItemPosCountVec dest;
	ItemPosCountVec dest2;
	Item* item = NULL;
	uint8 msg = aPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemEOC, 0);
	uint8 msg2 = aPlayer->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest2, aItem, 1);

	if( msg != EQUIP_ERR_OK )
    count2 -= noSpaceForCount;

	if( count2 == 0 || dest.empty())
    {
		aPlayer->CLOSE_GOSSIP_MENU();
        aCreature->MonsterWhisper("Error: Item count set to 0. Please inform a GM about this error.", aPlayer->GetGUID());
    }

	item = aPlayer->StoreNewItem( dest2, aItem, true, Item::GenerateItemRandomPropertyId(aItem));

	if(count2 > 0 && item)
    {
       aPlayer->CLOSE_GOSSIP_MENU();
       aPlayer->SendNewItem(item,count2,false,true);
       aPlayer->DestroyItemCount(itemEOC, 25, true);
    }

	if(noSpaceForCount > 0)
    {
        aPlayer->CLOSE_GOSSIP_MENU();
        aCreature->MonsterWhisper("You are not Space for Store Item", aPlayer->GetGUID());
    }

}

void SendDefaultMenu_npc_custom_items(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{
    uint32 itemBoj = 29434; // Badge of Justic  
	uint32 itemEOC = 45624; //Emblem of Conquest
    uint32 itemId2 = 23162; // Forors Kiste der endlosen Widerstandsaustattung
	uint32 weapon_one_hand = 900000; //Bloodvenom Blade
    uint32 noSpaceForCount = 0;
    int32 count = 0;
    int32 count2 = 1;

    Item* item = NULL;
    
    ItemPosCountVec dest;
    ItemPosCountVec dest2;
    uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemEOC, 0);
    uint8 msg2 = pPlayer->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest2, 23162, 1);

    // Not allow in combat
    if (pPlayer->isInCombat())
    {
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterSay("You are in combat", LANG_UNIVERSAL, NULL);
    return;
    }
    
    switch(uiAction)
    {
        case 2000:
			if(pPlayer->HasItemCount(itemEOC, 25 , true))
            {
				pPlayer->ADD_GOSSIP_ITEM( 2, "Relentless Gladiator\'s Longblade"    , GOSSIP_SENDER_MAIN, 2001);
				pPlayer->ADD_GOSSIP_ITEM( 2, "Relentless Gladiator\'s Swiftblade"    , GOSSIP_SENDER_MAIN, 2002);
				pPlayer->ADD_GOSSIP_ITEM( 2, "Relentless Gladiator\'s Blade of Celerity"    , GOSSIP_SENDER_MAIN, 2003);
				pPlayer->ADD_GOSSIP_ITEM( 2, "Relentless Gladiator\'s Dirk"    , GOSSIP_SENDER_MAIN, 2004);
				pPlayer->ADD_GOSSIP_ITEM( 2, "Relentless Gladiator\'s Grasp"    , GOSSIP_SENDER_MAIN, 2005);
				pPlayer->ADD_GOSSIP_ITEM( 2, "Relentless Gladiator\'s Left Claw"    , GOSSIP_SENDER_MAIN, 2006);
				pPlayer->ADD_GOSSIP_ITEM( 2, "Relentless Gladiator\'s Handaxe"    , GOSSIP_SENDER_MAIN, 2007);
				pPlayer->ADD_GOSSIP_ITEM( 2, "Relentless Gladiator\'s Salvation"    , GOSSIP_SENDER_MAIN, 2008);
				pPlayer->ADD_GOSSIP_ITEM( 2, "Relentless Gladiator\'s Punisher"    , GOSSIP_SENDER_MAIN, 2009);
				pPlayer->ADD_GOSSIP_ITEM( 2, "Relentless Gladiator\'s Halberd"    , GOSSIP_SENDER_MAIN, 2010);
				pPlayer->ADD_GOSSIP_ITEM( 2, "Relentless Gladiator\'s Acute Staff"    , GOSSIP_SENDER_MAIN, 2011);
				pPlayer->ADD_GOSSIP_ITEM( 2, "Relentless Gladiator\'s Sunderer"    , GOSSIP_SENDER_MAIN, 2012);
				pPlayer->ADD_GOSSIP_ITEM( 2, "Relentless Gladiator\'s Crusher"    , GOSSIP_SENDER_MAIN, 2013);
				pPlayer->ADD_GOSSIP_ITEM( 2, "Relentless Gladiator\'s Claymore"    , GOSSIP_SENDER_MAIN, 2014);
				pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
            }
            else
            {
                pPlayer->CLOSE_GOSSIP_MENU();
                pCreature->MonsterSay("Your Curency not enough", LANG_UNIVERSAL, NULL);
            }
        break;
		case 2001:
				Add_Item_to_player(pPlayer,pCreature,900001);
			break;

		case 2002:
				Add_Item_to_player(pPlayer,pCreature,900002);
			break;

		case 2003:
				Add_Item_to_player(pPlayer,pCreature,900003);
			break;

		case 2004:
				Add_Item_to_player(pPlayer,pCreature,900004);
			break;

		case 2005:
				Add_Item_to_player(pPlayer,pCreature,900005);
			break;

		case 2006:
				Add_Item_to_player(pPlayer,pCreature,900006);
			break;

		case 2007:
				Add_Item_to_player(pPlayer,pCreature,900007);
			break;

		case 2008:
				Add_Item_to_player(pPlayer,pCreature,900008);
			break;

		case 2009:
				Add_Item_to_player(pPlayer,pCreature,900009);
			break;

		case 2010:
				Add_Item_to_player(pPlayer,pCreature,900010);
			break;

		case 2011:
				Add_Item_to_player(pPlayer,pCreature,900011);
			break;

		case 2012:
				Add_Item_to_player(pPlayer,pCreature,900012);
			break;

		case 2013:
				Add_Item_to_player(pPlayer,pCreature,900013);
			break;

		case 2014:
				Add_Item_to_player(pPlayer,pCreature,900014);
			break;
    }
}

bool GossipSelect_npc_custom_items(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    // Main menu
    if (uiSender == GOSSIP_SENDER_MAIN)
    SendDefaultMenu_npc_custom_items(pPlayer, pCreature, uiAction);

return true;
}

void AddSC_npc_custom_items()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_custom_items";
    newscript->pGossipHello =  &GossipHello_npc_custom_items;
    newscript->pGossipSelect = &GossipSelect_npc_custom_items;
    newscript->RegisterSelf();
}