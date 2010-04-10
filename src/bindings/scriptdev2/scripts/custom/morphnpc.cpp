#include "precompiled.h"

bool GossipHello_npc_morph(Player* pPlayer, Creature* pCreature)
{
	pPlayer->ADD_GOSSIP_ITEM(0,"Illidan Morph",GOSSIP_SENDER_MAIN,1000);
	pPlayer->ADD_GOSSIP_ITEM(0,"Lich King Morph",GOSSIP_SENDER_MAIN,1001);
	pPlayer->ADD_GOSSIP_ITEM(0,"Lady Sylvanas Windrunner Morph",GOSSIP_SENDER_MAIN,1002);
	pPlayer->ADD_GOSSIP_ITEM(0,"Alexstrasza the Life-Binder Morph",GOSSIP_SENDER_MAIN,1003);
	pPlayer->ADD_GOSSIP_ITEM(0,"Kael'thas Sunstrider Morph",GOSSIP_SENDER_MAIN,1004);
	pPlayer->ADD_GOSSIP_ITEM(0,"High Inquisitor Whitemane Morph",GOSSIP_SENDER_MAIN,1005);
	pPlayer->ADD_GOSSIP_ITEM(0,"Cookie Morph",GOSSIP_SENDER_MAIN,1006);
	pPlayer->ADD_GOSSIP_ITEM(0,"Gorn One Eye Morph",GOSSIP_SENDER_MAIN,1007);
	pPlayer->ADD_GOSSIP_ITEM(0,"Halazzi Morph",GOSSIP_SENDER_MAIN,1008);
	pPlayer->ADD_GOSSIP_ITEM(0,"Zul'jin Morph",GOSSIP_SENDER_MAIN,1009);
	pPlayer->ADD_GOSSIP_ITEM(0,"Balnazzar Morph",GOSSIP_SENDER_MAIN,1010);
	pPlayer->ADD_GOSSIP_ITEM(0,"Kel'thuzad Morph",GOSSIP_SENDER_MAIN,1011);
	pPlayer->ADD_GOSSIP_ITEM(0,"Sneed's Shredder Morph",GOSSIP_SENDER_MAIN,1012);
	pPlayer->ADD_GOSSIP_ITEM(0,"Unstoppable Abomination Morph",GOSSIP_SENDER_MAIN,1013);
	pPlayer->ADD_GOSSIP_ITEM(0,"De Morph",GOSSIP_SENDER_MAIN,1014);

	pPlayer->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,pCreature->GetGUID());
	return true;
}

void SendDefaultMenu_npc_morph(Player* pPlayer, Creature* pCreature, uint32 uiAction)
{
	// Not allow in combat
    if (pPlayer->isInCombat())
    {
    pPlayer->CLOSE_GOSSIP_MENU();
    pCreature->MonsterSay("You are in combat", LANG_UNIVERSAL, NULL);
    return;
    }

	switch(uiAction)
	{
	case 1000: //Illidan
		pPlayer->SetDisplayId(21135);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X,0.40f);
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1001: //Lich King
		pPlayer->SetDisplayId(27481);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X,0.80f);
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1002: //Lady Sylvanas Windrunner
		pPlayer->SetDisplayId(28213);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X,1.50f);
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1003: //Alexstrasza the Life-Binder
		pPlayer->SetDisplayId(28227);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X,1.00f);
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1004: //Kael'thas Sunstrider
		pPlayer->SetDisplayId(20023);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X,0.60f);
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1005: //High Inquisitor Whitemane
		pPlayer->SetDisplayId(2043);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X,1.50f);
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1006: //Cookie
		pPlayer->SetDisplayId(1305);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X,2.00f);
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1007: //Gorn One Eye
		pPlayer->SetDisplayId(1012);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X,1.00f);
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1008: //Halazzi
		pPlayer->SetDisplayId(21632);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X,0.50f);
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1009: //Zul'jin
		pPlayer->SetDisplayId(21899);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X,0.50f);
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1010: //Balnazzar
		pPlayer->SetDisplayId(10691);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X,0.50f);
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1011: //Kel'thuzad	
		pPlayer->SetDisplayId(15945);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X,0.30f);
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1012: //Sneed's Shredder	
		pPlayer->SetDisplayId(1269);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X,1.00f);
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1013: //Unstoppable Abomination	
		pPlayer->SetDisplayId(12818);
		pPlayer->SetFloatValue(OBJECT_FIELD_SCALE_X,0.70f);
		pPlayer->CLOSE_GOSSIP_MENU();
		break;
	case 1014: //De Morph
		pPlayer->DeMorph();
		break;

	}

}

bool GossipSelect_npc_morph(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
	if (uiSender == GOSSIP_SENDER_MAIN)
	SendDefaultMenu_npc_morph(pPlayer, pCreature, uiAction);
	return true;
}

void AddSC_npc_morph()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_morph";
    newscript->pGossipHello =  &GossipHello_npc_morph;
    newscript->pGossipSelect = &GossipSelect_npc_morph;
    newscript->RegisterSelf();
}