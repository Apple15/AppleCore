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
SDName: boss_lord_marrowgar
SD%Complete: 30%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"

#define SOUND_IC_Marrowgar_Aggro01     16941
#define SOUND_IC_Marrowgar_Slay01      16942
#define SOUND_IC_Marrowgar_Slay02      16943
#define SOUND_IC_Marrowgar_Death01     16944
#define SOUND_IC_Marrowgar_Berserk01   16945
#define SOUND_IC_Marrowgar_WW01        16946
#define SOUND_IC_Marrowgar_BS01        16947
#define SOUND_IC_Marrowgar_BS02        16948
#define SOUND_IC_Marrowgar_BS03        16949
#define SOUND_IC_Marrowgar_AreaEntry01 16950

#define SAY_IC_Marrowgar_Aggro01    "The Scourge will wash over this world as a swarm of death and destruction."
#define SAY_IC_Marrowgar_AreaEntry0 "This is the beginning and the end, mortals. None may enter the Master's sanctum."
#define SAY_IC_Marrowgar_Berserk0   "The Master's rage courses through me!"
#define SAY_IC_Marrowgar_BS01       "Bound by bones!"
#define SAY_IC_Marrowgar_BS02       "Stick around."
#define SAY_IC_Marrowgar_BS03       "Your only escape is death."
#define SAY_IC_Marrowgar_Death01    "I see ... only darkness."
#define SAY_IC_Marrowgar_Slay01     "More bones for the offering."
#define SAY_IC_Marrowgar_Slay02     "Languishing damnation."
#define SAY_IC_IC_Marrowgar_WW01    "Bone storm!"

enum
{
        //common
        SPELL_BERSERK                           = 47008,
        //yells
        //summons
        MOB_BONE_SPIKE                          = 38711,
        MOB_COLDFLAME                           = 36672,
        //Abilities
        SPELL_SABER_LASH_N                      = 71021,
        SPELL_COLD_FLAME_N                      = 69146,
        SPELL_BONE_STRIKE                       = 69057,
        SPELL_BONE_STORM                        = 69076,
        SPELL_BONE_STRIKE_IMPALE                = 69065,
        SPELL_BONE_STORM_STRIKE_N               = 69075,

        SPELL_SABER_LASH_H                      = 71021,
        SPELL_COLD_FLAME_H                      = 70824,
        SPELL_BONE_STORM_STRIKE_H               = 70835, //h25 - 70836
};

struct MANGOS_DLL_DECL boss_lord_marrowgarAI : public ScriptedAI
{
    boss_lord_marrowgarAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool Regular;
    ScriptedInstance *pInstance;
    //uint8 stage;
    uint32 m_uiSaberLash_Timer;
    uint32 m_uiColdFlame_Timer;
    uint32 m_uiBoneStrike_Timer;
	bool   m_bHasTaunted;
    //bool   m_uiBoneStorm;
    //uint32 m_uiBoneStorm_Timer;
    uint32 m_uiBerserk_Timer;
    uint32 m_uiBoneStormStrike_Timer;
    //uint8 health;

    void Reset()
    {
		m_uiSaberLash_Timer = 10000;
		m_uiColdFlame_Timer = 5000;
		m_uiBoneStrike_Timer = urand(17000,29000);
		//m_uiBoneStorm_Timer = 12000;
		//m_uiBoneStormStrike_Timer = 40000;
		m_uiBerserk_Timer = 600000;

		//m_uiBoneStorm = false;

		m_bHasTaunted = false;

        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, NOT_STARTED);
    }

    uint64 CallGuard(uint64 npctype,TempSummonType type, uint32 _summontime )
    {
        float fPosX, fPosY, fPosZ;
        m_creature->GetPosition(fPosX, fPosY, fPosZ);
        m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(0, 10), fPosX, fPosY, fPosZ);
        Creature* pSummon = m_creature->SummonCreature(npctype, fPosX, fPosY, fPosZ, 0, type, _summontime);
		/*if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
        {
			pSummon->AI()->AttackStart(pTarget);
        }*/
        return pSummon ? pSummon->GetGUID() : 0;
    }

    uint64 CallSpike(Unit* pPlayer, uint64 npctype,TempSummonType type, uint32 _summontime )
    {
        float fPosX, fPosY, fPosZ;
        pPlayer->GetPosition(fPosX, fPosY, fPosZ);
        Creature* pSummon = m_creature->SummonCreature(npctype, fPosX, fPosY, fPosZ, 0, type, _summontime);
        if (pSummon) pSummon->AddThreat(pPlayer);
        return pSummon ? pSummon->GetGUID() : 0;
    }
/*
    void JustSummoned(Creature* _summoned)
    {
        if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,1))
            _summoned->AddThreat(target);
    }
*/
    void Aggro(Unit *who) 
    {
		DoPlaySoundToSet(m_creature, SOUND_IC_Marrowgar_Aggro01);
		m_creature->MonsterYell(SAY_IC_Marrowgar_Aggro01, LANG_UNIVERSAL, 0);
        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, IN_PROGRESS);
    }

    void JustDied(Unit *killer)
    {
		DoPlaySoundToSet(m_creature, SOUND_IC_Marrowgar_Death01);
		m_creature->MonsterYell(SAY_IC_Marrowgar_Death01, LANG_UNIVERSAL, 0);
        if(pInstance) pInstance->SetData(TYPE_MARROWGAR, DONE);
    }

	void KilledUnit(Unit *victim)
    {
		switch(urand(0, 1)) // 50% chance to say something out of 2 texts
			{
				case 0: DoPlaySoundToSet(m_creature, SOUND_IC_Marrowgar_Slay01);
					m_creature->MonsterYell(SAY_IC_Marrowgar_Slay01, LANG_UNIVERSAL, 0);break;
				case 1: DoPlaySoundToSet(m_creature, SOUND_IC_Marrowgar_Slay02); 
					m_creature->MonsterYell(SAY_IC_Marrowgar_Slay02, LANG_UNIVERSAL, 0);break;
			}
    }

	void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bHasTaunted && m_creature->IsWithinDistInMap(pWho, 80.0f))
        {
            DoPlaySoundToSet(m_creature, SOUND_IC_Marrowgar_AreaEntry01);
			m_creature->MonsterYell(SAY_IC_Marrowgar_AreaEntry0, LANG_UNIVERSAL, 0);
            m_bHasTaunted = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (m_uiBoneStrike_Timer < diff)
        { 
			switch(urand(0, 3)) // 50% chance to say something out of 2 texts
			{
				case 0: DoPlaySoundToSet(m_creature, SOUND_IC_Marrowgar_BS01);
					m_creature->MonsterYell(SAY_IC_Marrowgar_BS01, LANG_UNIVERSAL, 0);break;
				case 1: DoPlaySoundToSet(m_creature, SOUND_IC_Marrowgar_BS02); 
					m_creature->MonsterYell(SAY_IC_Marrowgar_BS02, LANG_UNIVERSAL, 0);break;
				case 2: DoPlaySoundToSet(m_creature, SOUND_IC_Marrowgar_BS03); 
					m_creature->MonsterYell(SAY_IC_Marrowgar_BS03, LANG_UNIVERSAL, 0);break;
			}
			int i;
			for(i=0; i < 2;i++)
			{
				if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
				{
					DoCastSpellIfCan(pTarget, SPELL_BONE_STRIKE, 1);
					CallSpike(pTarget, MOB_BONE_SPIKE, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
				}
			}
			m_uiBoneStrike_Timer=urand(17000,29000);
        } 
		else m_uiBoneStrike_Timer -= diff;

        if (m_uiSaberLash_Timer < diff)
        {
			DoCastSpellIfCan(m_creature->getVictim(), Regular ? SPELL_SABER_LASH_N :SPELL_SABER_LASH_H);
			m_uiSaberLash_Timer=urand(8000,12000);
        } 
		else m_uiSaberLash_Timer -= diff;

        if (m_uiColdFlame_Timer < diff) 
		{
			CallGuard(MOB_COLDFLAME, TEMPSUMMON_TIMED_DESPAWN, 60000);
			m_uiColdFlame_Timer = 5000;
        } 
		else m_uiColdFlame_Timer -= diff;

        if (m_uiBerserk_Timer < diff)
        {
			DoPlaySoundToSet(m_creature, SOUND_IC_Marrowgar_Berserk01);
			m_creature->MonsterYell(SAY_IC_Marrowgar_Berserk0, LANG_UNIVERSAL, 0);
            DoCastSpellIfCan(m_creature, SPELL_BERSERK, 1);
			m_uiBerserk_Timer = 30000;
        }
        else  m_uiBerserk_Timer -= diff;

        DoMeleeAttackIfReady();

    }
};

struct sColdflameMove
{
    uint32 uiLocId;
    float fX, fY, fZ;
};

static sColdflameMove aMoveData[]=
{
    {0, -391.1690f, 2275.1176f, 42.0453f},
    {1, -391.4419f, 2234.3132f, 41.9925f},
    {2, -362.7435f, 2238.3676f, 41.761f},
    {3, -355.8132f, 2212.8525f, 42.3536f},
    {4, -379.6618f, 2211.5026f, 41.9930f},
    {5, -416.8332f, 2209.3630f, 41.9930f},
    {6, -405.1085f, 2186.9045f, 41.9930f},
    {7, -371.7454f, 2167.3820f, 41.7631f},
	{8, -386.6853f, 2151.6757f, 41.9152f},
    {9, -397.4873f, 2166.0642f, 41.9469f},
    {10, -414.6102f, 2191.3090f, 41.9927f},
    {11, -419.4127f, 2225.1459f, 41.9927f},
    {12, -375.8561f, 2259.4694f, 41.7609f},
    {13, -353.8366f, 2197.9206f, 42.4227f},
    {14, -374.4479f, 2166.5317f, 41.7627f},
    {15, -391.7147f, 2193.8784f, 41.9924f},
};

struct MANGOS_DLL_DECL mob_coldflameAI : public ScriptedAI
{
    mob_coldflameAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 m_uiRangeCheck_Timer;
    bool Regular;
    uint32 _diff;
    float fPosX, fPosY, fPosZ;

	uint32 m_uiMovePoint;
    uint32 m_uiMovementTimer;
    sColdflameMove* m_pPointData;

    void Reset()
    {
        m_uiRangeCheck_Timer = 1000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->GetPosition(fPosX, fPosY, fPosZ);
        m_creature->GetRandomPoint(fPosX, fPosY, fPosZ, urand(150, 200), fPosX, fPosY, fPosZ);
        m_creature->GetMotionMaster()->MovePoint(0, fPosX, fPosY, fPosZ);
        m_creature->SetSpeedRate(MOVE_RUN, 0.5f);

		m_uiMovePoint = urand(0, 15);
        m_uiMovementTimer = 1000;
        m_pPointData = GetMoveData();
    }

	sColdflameMove* GetMoveData()
    {
        uint32 uiMaxCount = sizeof(aMoveData)/sizeof(sColdflameMove);

        for (uint32 i = 0; i < uiMaxCount; ++i)
        {
            if (aMoveData[i].uiLocId == m_uiMovePoint)
                return &aMoveData[i];
        }

        return NULL;
    }

    void SetNextRandomPoint()
    {
        uint32 uiMaxCount = sizeof(aMoveData)/sizeof(sColdflameMove);

        uint32 iTemp = urand(0, uiMaxCount-1);

        if (iTemp >= m_uiMovePoint)
            ++iTemp;

        m_uiMovePoint = iTemp;
    }

	void Aggro(Unit* pWho)
    {
        SetCombatMovement(false);

		m_creature->GetMotionMaster()->Clear(false);
        m_creature->GetMotionMaster()->MoveIdle();
    }

	void JustSummoned(Creature* summoned)
    {
        SetCombatMovement(false);

		m_creature->GetMotionMaster()->Clear(false);
        m_creature->GetMotionMaster()->MoveIdle();
    }

    void UpdateAI(const uint32 uiDiff)
    {
      _diff = uiDiff;

        if (m_uiRangeCheck_Timer < uiDiff)
        {
			m_creature->CastSpell(m_creature, Regular ? SPELL_COLD_FLAME_N : SPELL_COLD_FLAME_H, false);
                    /*Map* pMap = m_creature->GetMap();
                    Map::PlayerList const &lPlayers = pMap->GetPlayers();
                    for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                    {
                        Unit* pPlayer = itr->getSource();
                        if (!pPlayer) continue;
                        if (pPlayer->isAlive() && pPlayer->IsWithinDistInMap(m_creature, 5.0f))
                            if (Creature* pMarrowgar = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(NPC_LORD_MARROWGAR))))
                                pMarrowgar->CastSpell(pPlayer, Regular ? SPELL_COLD_FLAME_N : SPELL_COLD_FLAME_H, true);
                     }*/
            m_uiRangeCheck_Timer = 1000;
        }
        else m_uiRangeCheck_Timer -= uiDiff;

		if (m_uiMovementTimer < uiDiff)
        {
                m_pPointData = GetMoveData();

                SetNextRandomPoint();

                m_uiMovementTimer = urand(5000,15000);

                if (!m_pPointData)
                    return;
 
                m_creature->GetMotionMaster()->MovePoint(m_pPointData->uiLocId, m_pPointData->fX, m_pPointData->fY, m_pPointData->fZ);
            
         }
         else m_uiMovementTimer -= uiDiff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
    }
};

struct MANGOS_DLL_DECL mob_bone_spikeAI : public Scripted_NoMovementAI
{
    mob_bone_spikeAI(Creature *pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 m_uiRangeCheck_Timer;
    bool Regular;
    uint32 _diff;
    uint64 m_uiVictimGUID;


    void Reset()
    {
        m_uiRangeCheck_Timer = 1000;
        m_creature->SetSpeedRate(MOVE_RUN, 5);
        _diff = 1000;
        m_creature->SetInCombatWithZone();
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWith(pWho);
        pWho->SetInCombatWith(m_creature);
        m_creature->GetMotionMaster()->MoveChase(pWho);
        DoCast(pWho, SPELL_BONE_STRIKE_IMPALE);
        m_uiVictimGUID = pWho->GetGUID();
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > m_creature->GetHealth())
        {
            if (m_uiVictimGUID)
            {
                if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
                    pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
            }
        }
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim) pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
    }

    void JustDied(Unit* Killer)
    {
        if (Unit* pVictim = Unit::GetUnit((*m_creature), m_uiVictimGUID))
            pVictim->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
        if (Killer)
            Killer->RemoveAurasDueToSpell(SPELL_BONE_STRIKE_IMPALE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiRangeCheck_Timer < uiDiff)
        {
        }
        else m_uiRangeCheck_Timer -= uiDiff;
    }

};

CreatureAI* GetAI_mob_bone_spike(Creature* pCreature)
{
    return new mob_bone_spikeAI(pCreature);
}

CreatureAI* GetAI_mob_coldflame(Creature* pCreature)
{
    return new mob_coldflameAI(pCreature);
}

CreatureAI* GetAI_boss_lord_marrowgar(Creature* pCreature)
{
    return new boss_lord_marrowgarAI(pCreature);
}

void AddSC_boss_lord_marrowgar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lord_marrowgar";
    newscript->GetAI = &GetAI_boss_lord_marrowgar;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_coldflame";
    newscript->GetAI = &GetAI_mob_coldflame;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_bone_spike";
    newscript->GetAI = &GetAI_mob_bone_spike;
    newscript->RegisterSelf();

}
