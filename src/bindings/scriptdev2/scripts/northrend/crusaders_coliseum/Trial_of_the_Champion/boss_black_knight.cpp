/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_black_knight
SD%Complete: 92%
SDComment: missing yells. not sure about timers.
SDCategory: Trial Of the Champion
EndScriptData */

#include "precompiled.h"
#include "trial_of_the_champion.h"

enum
{
	//yells

	//undead
	SPELL_PLAGUE_STRIKE			= 67724,
	SPELL_PLAGUE_STRIKE_H		= 67884,
	SPELL_ICY_TOUCH				= 67718,
	SPELL_ICY_TOUCH_H			= 67881,
	SPELL_OBLITERATE			= 67725,
	SPELL_OBLITERATE_H			= 67883,
	SPELL_CHOKE					= 67745,
	SPELL_CHOKE_H				= 68306,
	//skeleton
	SPELL_ARMY					= 67761,			//replacing original one, since that one spawns millions of ghouls!!
	SPELL_DESECRATION			= 67781,
	SPELL_DESECRATION_H			= 67876,
	//ghost
	SPELL_DEATH 				= 67808,
	SPELL_DEATH_H				= 67875,
	SPELL_MARK					= 67823,

	//risen ghoul
	SPELL_CLAW					= 67879,
	SPELL_EXPLODE				= 67729,
	SPELL_EXPLODE_H				= 67886,
	SPELL_LEAP					= 67749,
	SPELL_LEAP_H				= 67880,

	//sword ID
	EQUIP_SWORD					= 40343
};

//Speak
#define SOUND_TOC_BLACKKNIGHT_Aggro01		16259
#define SOUND_TOC_BLACKKNIGHT_AreaEntry0    16257
#define SOUND_TOC_BLACKKNIGHT_AreaEntry1    16258
#define SOUND_TOC_BLACKKNIGHT_PH02			16262
#define SOUND_TOC_BLACKKNIGHT_PH03			16263
#define SOUND_TOC_BLACKKNIGHT_Slay01        16260
#define SOUND_TOC_BLACKKNIGHT_Slay02        16261
#define SOUND_TOC_BLACKKNIGHT_Death01       16264

#define SAY_TOC_BLACKKNIGHT_Aggro01		"This farce ends here!"
#define SAY_TOC_BLACKKNIGHT_AreaEntry0  "Did you honestly think that an agent of the Lich King would be bested on the field of your pathetic little tournament?"
#define SAY_TOC_BLACKKNIGHT_AreaEntry1  "I have come to finish my task."
#define SAY_TOC_BLACKKNIGHT_PH02		"My rotting flesh was just getting in the way!"
#define SAY_TOC_BLACKKNIGHT_PH03		"I have no need for bones to best you!"
#define SAY_TOC_BLACKKNIGHT_Slay01      "Pathetic."
#define SAY_TOC_BLACKKNIGHT_Slay02		"A waste of flesh."
#define SAY_TOC_BLACKKNIGHT_Death01     "No! I must not fail... again..."

// Risen Ghoul
struct MANGOS_DLL_DECL mob_toc5_risen_ghoulAI : public ScriptedAI
{
    mob_toc5_risen_ghoulAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
	}

	ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

	uint32 Attack;

    void Reset()
    {
		Attack = 2500;
    }

	void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (Attack < diff)
        {
			if (Creature* pTemp = ((Creature*)Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_BLACK_KNIGHT))))
				if (pTemp->isAlive())
					if ((pTemp->GetHealth()*100 / pTemp->GetMaxHealth()) < 25)
						DoCast(m_creature, m_bIsRegularMode ? SPELL_EXPLODE : SPELL_EXPLODE_H);
			if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 4))
			{
				DoCast(m_creature->getVictim(), SPELL_CLAW);
				if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,1))
					m_creature->AI()->AttackStart(target);
				Attack = 2500;
			}else
			if (m_creature->IsWithinDistInMap(m_creature->getVictim(), 30))
			{
				DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_LEAP : SPELL_LEAP_H);
				Attack = 2500;
			}
        }else Attack -= diff;

		if ((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 25)
			DoCast(m_creature, m_bIsRegularMode ? SPELL_EXPLODE : SPELL_EXPLODE_H);
		
		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_mob_toc5_risen_ghoul(Creature* pCreature)
{
    return new mob_toc5_risen_ghoulAI(pCreature);
}

// The Black Knight
struct MANGOS_DLL_DECL boss_black_knightAI : public ScriptedAI
{
    boss_black_knightAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		Reset();
		m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
	}

	ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

	uint32 Plague_Strike_Timer;
	uint32 Icy_Touch_Timer;
	uint32 Obliterate_Timer;
	uint32 Choke_Timer;
	uint32 Death_Timer;
	uint32 Mark_Timer;
	uint32 Phase_Delay;
	uint32 Summon_Ghoul;
	uint32 Event1_Timer;
	uint32 Event2_Timer;
	uint32 Desecration_Timer;

	bool phase1;
	bool phase2;
	bool phase3;
	bool ghoul;

	bool Eventchk2;

    void Reset()
    {
		m_creature->SetRespawnDelay(999999999);
		m_creature->SetDisplayId(29837);
		SetEquipmentSlots(false, EQUIP_SWORD, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
		Plague_Strike_Timer = 5000;
		Icy_Touch_Timer = 10000;
		Obliterate_Timer = 16000;
		Desecration_Timer = 13000;
		Choke_Timer = 15000;
		Summon_Ghoul = 4000;
		Event1_Timer = 1000;
		Event2_Timer = 9000;
		phase1 = true;
		phase2 = false;
		phase3 = false;
		ghoul = false;
    }

	void EnterEvadeMode()
	{
		m_creature->SetDisplayId(29837);
		Plague_Strike_Timer = 5000;
		Icy_Touch_Timer = 10000;
		Obliterate_Timer = 16000;
		Choke_Timer = 15000;
		Summon_Ghoul = 4000;
		Event1_Timer = 1000;
		Event2_Timer = 9000;
		phase1 = true;
		phase2 = false;
		phase3 = false;
		ghoul = false;
		Eventchk2 = false;
		m_creature->RemoveArenaAuras(true);
		m_creature->SendMonsterMove(746.864441f, 660.918762f, 411.695465f, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
		m_creature->GetMap()->CreatureRelocation(m_creature, 754.360779f, 660.816162f, 412.395996f, 4.698700f);
		m_creature->SetHealth(m_creature->GetMaxHealth());
	}

	void Aggro(Unit* pWho)
    {
		if (!m_pInstance)
			return;
		if (m_pInstance->GetData(TYPE_BLACK_KNIGHT) == DONE)
			m_creature->ForcedDespawn();
		else
			m_pInstance->SetData(TYPE_BLACK_KNIGHT, IN_PROGRESS);

		m_creature->MonsterYell(SAY_TOC_BLACKKNIGHT_Aggro01, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_TOC_BLACKKNIGHT_Aggro01);
    }

	void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
		if (uiDamage > m_creature->GetHealth() && !phase3){
			uiDamage = 0;
			if (phase2)
				StartPhase3();
			if (phase1)
				StartPhase2();
		}
    }

	void JustDied(Unit* pKiller)
    {
		if (!m_pInstance)
			return;
		if (phase3)
		{
			m_creature->MonsterYell(SAY_TOC_BLACKKNIGHT_Death01, LANG_UNIVERSAL, NULL);
			DoPlaySoundToSet(m_creature, SOUND_TOC_BLACKKNIGHT_Death01);
			m_pInstance->SetData(TYPE_BLACK_KNIGHT, DONE);
		}
		if (phase2)
			if (!m_creature->isAlive())
			{
				m_creature->Respawn();
				StartPhase3();
			}
		if (phase1)
			if (!m_creature->isAlive())
			{
				m_creature->Respawn();
				StartPhase2();
			}
	}

	void StartPhase2()
	{
		m_creature->MonsterYell(SAY_TOC_BLACKKNIGHT_PH02, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_TOC_BLACKKNIGHT_PH02);
		m_creature->SetHealth(m_creature->GetMaxHealth());
		m_creature->SetDisplayId(29846);
		phase1 = false;
		phase2 = true;
		phase3 = false;
		DoCast(m_creature, SPELL_ARMY);
		Plague_Strike_Timer = 14000;
		Icy_Touch_Timer = 12000;
		Obliterate_Timer = 18000;
	}

	void StartPhase3()
	{
		m_creature->MonsterYell(SAY_TOC_BLACKKNIGHT_PH03, LANG_UNIVERSAL, NULL);
        DoPlaySoundToSet(m_creature, SOUND_TOC_BLACKKNIGHT_PH03);
		m_creature->SetHealth(m_creature->GetMaxHealth());
		m_creature->SetDisplayId(14560);
		SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
		phase1 = false;
		phase2 = false;
		phase3 = true;
		Death_Timer = 3000;
		Mark_Timer = 9000;
	}

	void KilledUnit(Unit *Victim) 
    {
        switch (rand()%2)
        {  
            case 0: DoPlaySoundToSet(m_creature, SOUND_TOC_BLACKKNIGHT_Slay01); m_creature->MonsterYell(SAY_TOC_BLACKKNIGHT_Slay01, LANG_UNIVERSAL, NULL);break;
            case 1: DoPlaySoundToSet(m_creature, SOUND_TOC_BLACKKNIGHT_Slay02); m_creature->MonsterYell(SAY_TOC_BLACKKNIGHT_Slay02, LANG_UNIVERSAL, NULL);break;
        }
    }

	void UpdateAI(const uint32 diff)
    {
		if (!m_creature->getVictim() && Event1_Timer < diff && phase1)
		{
			m_creature->MonsterYell(SAY_TOC_BLACKKNIGHT_AreaEntry0, LANG_UNIVERSAL, NULL);
			DoPlaySoundToSet(m_creature, SOUND_TOC_BLACKKNIGHT_AreaEntry0);
			Eventchk2 = true;
			Event1_Timer = 600000;
		}else Event1_Timer -= diff;
		
		if (!m_creature->getVictim() && Eventchk2 == true && Event2_Timer < diff && phase1) 
        {
			m_creature->MonsterYell(SAY_TOC_BLACKKNIGHT_AreaEntry1, LANG_UNIVERSAL, NULL);
			DoPlaySoundToSet(m_creature, SOUND_TOC_BLACKKNIGHT_AreaEntry1);
			Eventchk2 = false;
			Event2_Timer = 600000;
        }else Event2_Timer -= diff;

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (Plague_Strike_Timer < diff && !phase3)
        {
			DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_PLAGUE_STRIKE : SPELL_PLAGUE_STRIKE_H);
            Plague_Strike_Timer = 10500;
        }else Plague_Strike_Timer -= diff;  

		if (Icy_Touch_Timer < diff && !phase3)
        {
			DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_ICY_TOUCH : SPELL_ICY_TOUCH_H);
            Icy_Touch_Timer = 10000;
        }else Icy_Touch_Timer -= diff;

		if (Obliterate_Timer < diff && !phase3)
        {
			DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_OBLITERATE : SPELL_OBLITERATE_H);
            Obliterate_Timer = 11000;
        }else Obliterate_Timer -= diff;

		if (Choke_Timer < diff && phase1)
        {
			if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
				DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_CHOKE : SPELL_CHOKE_H);
			Choke_Timer = 15000;
        }else Choke_Timer -= diff;

		if (Desecration_Timer < diff && phase2)
        {
			if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
				DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_DESECRATION : SPELL_DESECRATION_H);
			Desecration_Timer = urand(13000,17500);
        }else Desecration_Timer -= diff;

		if (Summon_Ghoul < diff && phase1 && !ghoul)
        {
			if (m_pInstance->GetData(DATA_TOC5_ANNOUNCER) == m_pInstance->GetData(DATA_JAEREN))
				 m_creature->SummonCreature(NPC_RISEN_JAEREN, 0.0f, 0.0f, 0.0f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
			else
				m_creature->SummonCreature(NPC_RISEN_ARELAS, 0.0f, 0.0f, 0.0f, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
			ghoul = true;
        }else Summon_Ghoul -= diff;

		if (Mark_Timer < diff && phase3)
        {
			if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
				DoCast(target, SPELL_MARK);
			Mark_Timer = 15000;
        }else Mark_Timer -= diff;

		if (Death_Timer < diff && phase3)
        {
			DoCast(m_creature, m_bIsRegularMode ? SPELL_DEATH : SPELL_DEATH_H);
			Death_Timer = 2000;
        }else Death_Timer -= diff;
		
		DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_black_knight(Creature* pCreature)
{
    return new boss_black_knightAI(pCreature);
}

void AddSC_boss_black_knight()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "mob_toc5_risen_ghoul";
	NewScript->GetAI = &GetAI_mob_toc5_risen_ghoul;
    NewScript->RegisterSelf();

	NewScript = new Script;
    NewScript->Name = "boss_black_knight";
	NewScript->GetAI = &GetAI_boss_black_knight;
    NewScript->RegisterSelf();
}