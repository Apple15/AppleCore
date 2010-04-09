/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Instance_Azjol-Nerub
SD%Complete: 50
SDComment:
SDCategory: Azjol-Nerub
EndScriptData */

#include "precompiled.h"
#include "the_forge_of_souls.h"

struct MANGOS_DLL_DECL instance_the_forge_of_souls : public ScriptedInstance
{
    instance_the_forge_of_souls(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint32 m_auiEncounter[MAX_ENCOUNTER];
    std::string strInstData;

	uint64 m_uiBronjhamGUID;
    uint64 m_uiDevourerofsoulsGUID;

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

		m_uiBronjhamGUID = 0;
		m_uiDevourerofsoulsGUID = 0;


    }

	void OnCreatureCreate(Creature* pCreature)
    {
        switch(pCreature->GetEntry())
        {
            case NPC_BRONJAHM: m_uiBronjhamGUID = pCreature->GetGUID(); break;
            case NPC_DEVOURER_OF_SOULS: m_uiDevourerofsoulsGUID = pCreature->GetGUID(); break;
            
        }
    }

    void SetData(uint32 uiType, uint32 uiData)
    {
        switch(uiType)
        {
            case TYPE_BRONJAHM:
                m_auiEncounter[0] = uiData;
                break;
            case TYPE_DEVOURER_OF_SOULS:
                m_auiEncounter[1] = uiData;
                break;
        }

        if (uiData == DONE)
        {
            OUT_SAVE_INST_DATA;

            std::ostringstream saveStream;
            saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1];

            strInstData = saveStream.str();

            SaveToDB();
            OUT_SAVE_INST_DATA_COMPLETE;
        }
    }

    const char* Save()
    {
        return strInstData.c_str();
    }

	uint64 GetData64(uint32 uiData)
    {
        switch(uiData)
        {
            case NPC_BRONJAHM:
                return m_uiBronjhamGUID;
            case NPC_DEVOURER_OF_SOULS:
                return m_uiDevourerofsoulsGUID;
        }
        return 0;
    }

	uint32 GetData(uint32 uiType)
    {
        switch(uiType)
        {
            case TYPE_BRONJAHM:
            case TYPE_DEVOURER_OF_SOULS:
                return m_auiEncounter[uiType];
        }

        return 0;
    }

    void Load(const char* in)
    {
        if (!in)
        {
            OUT_LOAD_INST_DATA_FAIL;
            return;
        }

        OUT_LOAD_INST_DATA(in);

        std::istringstream loadStream(in);
        loadStream >> m_auiEncounter[0] >> m_auiEncounter[1];

        for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
        {
            if (m_auiEncounter[i] == IN_PROGRESS)
                m_auiEncounter[i] = NOT_STARTED;
        }

        OUT_LOAD_INST_DATA_COMPLETE;
    }
};

InstanceData* GetInstanceData_instance_the_forge_of_souls(Map* pMap)
{
    return new instance_the_forge_of_souls(pMap);
}

void AddSC_instance_the_forge_of_souls()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_the_forge_of_souls";
    newscript->GetInstanceData = &GetInstanceData_instance_the_forge_of_souls;
    newscript->RegisterSelf();
}
