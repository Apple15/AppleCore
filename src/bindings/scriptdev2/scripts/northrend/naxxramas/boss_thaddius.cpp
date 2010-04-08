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
SDName: Boss_Thaddius
SD%Complete: 0
SDComment: Placeholder. Includes Feugen & Stalagg.
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

#define SAY_AGGRO               -1533029
#define SAY_KILL1               -1533031
#define SAY_KILL2               -1533032
#define SAY_KILL3               -1533033
#define SAY_KILL4               -1533034
#define SAY_DEATH               -1533035
#define SAY_SCREAM1             -1533036
#define SAY_SCREAM2             -1533037
#define SAY_SCREAM3             -1533038
#define SAY_SCREAM4             -1533039

#define SPELL_BALL_LIGHTNING                28299

#define SPELL_POLARITY_SHIFT                28089
#define SPELL_CHARGE_POSITIVE_DMGBUFF       29659
#define SPELL_CHARGE_POSITIVE_NEARDMG       28059 // *
#define SPELL_CHARGE_NEGATIVE_DMGBUFF       29660
#define SPELL_CHARGE_NEGATIVE_NEARDMG       28084 // *

#define SPELL_CHAIN_LIGHTNING               28167
#define H_SPELL_CHAIN_LIGHTNING             54531

#define SPELL_BESERK                        26662

bool CheckStalaggAlive = true;
bool CheckFeugenAlive = true;

struct MANGOS_DLL_DECL boss_thaddiusAI : public ScriptedAI
{
    boss_thaddiusAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

	ScriptedInstance* m_pInstance;
	bool m_bIsRegularMode;

    uint32 random_timer;

	uint32 ChainLightning_Timer;
    uint32 PolarityShift_Timer;
    uint32 BallLightning_Timer;
    uint32 Enrage_Timer;

	void Reset()
    { 
        random_timer = 20000;

		ChainLightning_Timer = 15000;
        PolarityShift_Timer = 30000;
        BallLightning_Timer = 1000;
        Enrage_Timer = 300000;

		m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_STUNNED);

		m_pInstance->SetData(TYPE_THADDIUS, NOT_STARTED);
    }

	void AttackStart(Unit *Who)
    {
        if (!Who)
            return;
		
		 if (m_creature->Attack(Who, true))
        {
            m_creature->AddThreat(Who, 0.0f);
            m_creature->SetInCombatWith(Who);
            Who->SetInCombatWith(m_creature);
            DoStartMovement(Who);
        }
    }

	void KilledUnit(Unit *victim)
    {
        if(victim == m_creature)
            return;
        switch(rand()%4)
        {
            case 0: DoScriptText(SAY_KILL1, m_creature);break;
            case 1: DoScriptText(SAY_KILL2, m_creature);break;
            case 2: DoScriptText(SAY_KILL3, m_creature);break;
            case 3: DoScriptText(SAY_KILL4, m_creature);break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);

		if (m_pInstance)
            m_pInstance->SetData(TYPE_THADDIUS, DONE);
    }

    void Aggro(Unit *who)
    {
		DoScriptText(SAY_AGGRO, m_creature);

        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
            AttackStart(who);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_THADDIUS, IN_PROGRESS);
    }

	void UpdateAI(const uint32 uiDiff)
    {
       if (CheckStalaggAlive == false && CheckFeugenAlive == false) 
	   {
           m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_STUNNED);
       }
	   else if(random_timer < uiDiff)
       {
                switch (rand()%4)
                {
                case 0:
                    DoScriptText(SAY_SCREAM1, m_creature);
                    break;
                case 1:
                    DoScriptText(SAY_SCREAM2, m_creature);
                    break;
                case 2:
                    DoScriptText(SAY_SCREAM3, m_creature);
                    break;
                case 3: 
                    DoScriptText(SAY_SCREAM4, m_creature);
                    break;

                }
                random_timer = 20000;
         }else random_timer -=uiDiff;

	    if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
                return;
		
	   //Cast Ball Lightning
        if(BallLightning_Timer < uiDiff)
        {
            Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0);
            DoCast(target,SPELL_BALL_LIGHTNING);
            BallLightning_Timer = 20000;
        }
			else BallLightning_Timer -= uiDiff;
		
		// Cast ChainLightning
        if (ChainLightning_Timer < uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? H_SPELL_CHAIN_LIGHTNING : SPELL_CHAIN_LIGHTNING);
            ChainLightning_Timer = 12000+rand()%5000;
        }
			else ChainLightning_Timer -= uiDiff;

	    if(PolarityShift_Timer < uiDiff)
        {
      	    DoCast(m_creature, SPELL_POLARITY_SHIFT); // need core support
            PolarityShift_Timer = 30000;
        }
			else PolarityShift_Timer -= uiDiff;

	    if (Enrage_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_BESERK);
            Enrage_Timer = 300000;
        }
			else Enrage_Timer -= uiDiff;

		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_thaddius(Creature* pCreature)
{
    return new boss_thaddiusAI(pCreature);
}

//Feugen
#define SAY_FEUG_AGGRO          -1533026
#define SAY_FEUG_SLAY           -1533027
#define SAY_FEUG_DEATH          -1533028

#define SPELL_WARSTOMP			   28125
#define SPELL_STATICFIELD          28135
#define H_SPELL_STATICFIELD        54528

struct MANGOS_DLL_DECL mob_feugenAI : public ScriptedAI
{
    mob_feugenAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

	ScriptedInstance* m_pInstance;
	bool m_bIsRegularMode;

	uint32 WarStomp_Timer;
    uint32 StaticField_Timer;

	void Reset()
    {
        WarStomp_Timer = 8000+rand()%2000;
        StaticField_Timer = 10000+rand()%5000;

		CheckFeugenAlive = true;
    }

	void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_FEUG_AGGRO, m_creature);
    }

	void KilledUnit(Unit *victim)
    {
        if(victim == m_creature)
            return;
        DoScriptText(SAY_FEUG_SLAY, m_creature);
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_FEUG_DEATH, m_creature);
		CheckFeugenAlive = false;
    }

	void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (WarStomp_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_WARSTOMP);
            WarStomp_Timer = 8000+rand()%2000;
        }else WarStomp_Timer -= uiDiff;

        if (StaticField_Timer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? H_SPELL_STATICFIELD : SPELL_STATICFIELD);
            StaticField_Timer = 10000+rand()%5000;
        }else StaticField_Timer -= uiDiff;

		 DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_feugen(Creature* pCreature)
{
    return new mob_feugenAI(pCreature);
}

//Stalagg
#define SAY_STAL_AGGRO          -1533023
#define SAY_STAL_SLAY           -1533024
#define SAY_STAL_DEATH          -1533025

#define SPELL_WARSTOMP             28125
#define SPELL_POWERSURGE           28134
#define H_SPELL_POWERSURGE         54529

struct MANGOS_DLL_DECL mob_stalaggAI : public ScriptedAI
{
    mob_stalaggAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

	ScriptedInstance* m_pInstance;
	bool m_bIsRegularMode;

	uint32 WarStomp_Timer;
    uint32 PowerSurge_Timer;

	void Reset()
    {
        WarStomp_Timer = 8000+rand()%2000;
        PowerSurge_Timer = 10000+rand()%5000;

		CheckStalaggAlive = true;
    }

	void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_STAL_AGGRO, m_creature);
    }

	void KilledUnit(Unit *victim)
    {
        if(victim == m_creature)
            return;
        DoScriptText(SAY_STAL_SLAY, m_creature);
    }

    void JustDied(Unit* killer)
    {
        DoScriptText(SAY_STAL_DEATH, m_creature);
		CheckStalaggAlive = false;
    }

	void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (WarStomp_Timer < uiDiff)
        {
            DoCast(m_creature, SPELL_WARSTOMP);
            WarStomp_Timer = 8000+rand()%2000;
        }else WarStomp_Timer -= uiDiff;

        if (PowerSurge_Timer < uiDiff)
        {
            DoCast(m_creature, m_bIsRegularMode ? H_SPELL_POWERSURGE : SPELL_POWERSURGE);
            PowerSurge_Timer = 10000+rand()%5000;
        }else PowerSurge_Timer -= uiDiff;

		DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_stalagg(Creature* pCreature)
{
    return new mob_stalaggAI(pCreature);
}

void AddSC_boss_thaddius()
{
    Script *NewScript;
    NewScript = new Script;
    NewScript->Name = "boss_thaddius";
    NewScript->GetAI = &GetAI_boss_thaddius;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_stalagg";
    NewScript->GetAI = &GetAI_mob_stalagg;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_feugen";
    NewScript->GetAI = &GetAI_mob_feugen;
    NewScript->RegisterSelf();
}