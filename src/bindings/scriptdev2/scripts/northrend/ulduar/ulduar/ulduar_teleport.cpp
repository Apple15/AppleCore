
#include "precompiled.h"
#include "def_ulduar.h"

/*
The teleporter appears to be active and stable.

- Expedition Base Camp
- Formation Grounds
- Colossal Forge
- Scrapyard
- Antechamber of Ulduar
- Shattered Walkway
- Conservatory of Life
- Spark of Imagination
- Prison of Yogg-Saron
*/

#define BASE_CAMP    200
#define GROUNDS      201
#define FORGE        202
#define SCRAPYARD    203
#define ANTECHAMBER  204
#define WALKWAY      205
#define CONSERVATORY 206
#define SPARK        207
#define PRISON       208

bool GossipHello_ulduar_teleporter(Player *player, Creature *creature)
{

    ScriptedInstance *pInstance = (ScriptedInstance *) creature->GetInstanceData();

    if(!pInstance) return true;

    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to the Expedition Base Camp", GOSSIP_SENDER_MAIN, BASE_CAMP);
    if(pInstance->GetData(TYPE_LEVIATHAN_TP) == IN_PROGRESS)
    {
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to the Formation Grounds", GOSSIP_SENDER_MAIN, GROUNDS);
    };
    if(pInstance->GetData(TYPE_FLAME_LEVIATHAN) == DONE)
    {
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to the Colossal Forge", GOSSIP_SENDER_MAIN, FORGE);
    };
    if(pInstance->GetData(TYPE_XT002_TP) == IN_PROGRESS)
    {
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to the Scrapyard", GOSSIP_SENDER_MAIN, SCRAPYARD);
    };
    if(pInstance->GetData(TYPE_XT002) == DONE)
    {
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to the Antechamber of Ulduar", GOSSIP_SENDER_MAIN, ANTECHAMBER);
    };
    if(pInstance->GetData(TYPE_KOLOGARN) == DONE)
    {
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to the Shattered Walkway", GOSSIP_SENDER_MAIN, WALKWAY);
    };
    if(pInstance->GetData(TYPE_AURIAYA) == DONE)
    {
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to the Conservatory of Life", GOSSIP_SENDER_MAIN, CONSERVATORY);
    };
    if(pInstance->GetData(TYPE_MIMIRON_TP) == IN_PROGRESS)
    {
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to the Spark of Imagination", GOSSIP_SENDER_MAIN, SPARK);
    };
    if(pInstance->GetData(TYPE_VEZAX) == DONE)
    {
    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "Teleport to the Prison of Yogg-Saron", GOSSIP_SENDER_MAIN, PRISON);
    };
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
    return true;
}

bool GossipSelect_ulduar_teleporter(Player *player, Creature *creature, uint32 sender, uint32 action)
{
    if(sender != GOSSIP_SENDER_MAIN) return true;
    if(!player->getAttackers().empty()) return true;

    switch(action)
    {
    case BASE_CAMP:
        player->TeleportTo(603, -706.122f, -92.6024f, 429.876f, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    case GROUNDS:
        player->TeleportTo(603, 131.248f, -35.3802f, 409.804f, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    case FORGE:
        player->TeleportTo(603, 553.233f, -12.3247f, 409.679f, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    case SCRAPYARD:
        player->TeleportTo(603, 926.292f, -11.4635f, 418.595f, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    case ANTECHAMBER:
        player->TeleportTo(603, 1498.09f, -24.246f, 420.967f, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    case WALKWAY:
        player->TeleportTo(603, 1859.45f, -24.1f, 448.9f, 0); 
        player->CLOSE_GOSSIP_MENU(); break;
    case CONSERVATORY:
        player->TeleportTo(603, 2086.27f, -24.3134f, 421.239f, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    case SPARK:
        player->TeleportTo(603, 2536.87f, 2569.15f, 412.304f, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    case PRISON:
        player->TeleportTo(603, 1854.297f, -11.0173f, 334.4f, 0);
        player->CLOSE_GOSSIP_MENU(); break;
    }

    return true;
}

void AddSC_ulduar_teleport()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "ulduar_teleporter";
    newscript->pGossipHello = &GossipHello_ulduar_teleporter;
    newscript->pGossipSelect = &GossipSelect_ulduar_teleporter;
    newscript->RegisterSelf();
}
