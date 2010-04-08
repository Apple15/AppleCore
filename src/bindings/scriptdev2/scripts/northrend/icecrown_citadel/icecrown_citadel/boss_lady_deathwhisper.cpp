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
SDName: boss_lady_deathwhisper
SD%Complete: 20%
SDComment: by /dev/rsa
SDCategory: Icecrown Citadel
EndScriptData */

#include "precompiled.h"
#include "def_spire.h"

enum
{
        //common
        //SPELL_BERSERK                           = 47008,
        //yells
        //summons
        NPC_VENGEFUL_SHADE                      = 38222,
        NPC_FANATIC                             = 37890,
        NPC_REANIMATED_FANATIC                  = 38009,
        NPC_ADHERENT                            = 37949,
        NPC_REANIMATED_ADHERENT                 = 38010,
        //Abilities
        SPELL_MANA_BARRIER                      = 70842,
        SPELL_SHADOW_BOLT_N                     = 71254,
        SPELL_DEATH_AND_DECAY_N                 = 71001,
        SPELL_DARK_EMPOWERMENT_N                = 70901,
        SPELL_FROSTBOLT_N                       = 71420,
        SPELL_INSIGNIFICANCE_N                  = 71204,

        SPELL_SHADOW_BOLT_H                     = 72008,
        SPELL_FROSTBOLT_H                       = 72007,
        SPELL_DOMINATE_MIND_H                   = 71289,

        SPELL_VENGEFUL_BLAST_N                  = 72011,
        SPELL_VENGEFUL_BLAST_H                  = 72012,
};

// Lady Deathwhisper
#define SAY_IC_Deathwhisper_Aggro01   "What is this disturbance?! You dare trespass upon this hallowed ground? This shall be your final resting place."
#define SAY_IC_Deathwhisper_Death01   "All part of the masters plan! Your end is... inevitable!"
#define SAY_IC_Deathwhisper_Berserk01 "This charade has gone on long enough."
#define SAY_IC_Deathwhisper_P2        "Enough! I see I must take matters into my own hands!"
#define SAY_IC_Deathwhisper_Slay01    "Do you yet grasp of the futility of your actions?"
#define SAY_IC_Deathwhisper_Slay02    "Embrace the darkness... Darkness eternal!"
#define SAY_IC_Deathwhisper_SP01      "Take this blessing and show these intruders a taste of our master's power."
#define SAY_IC_Deathwhisper_SP02      "I release you from the curse of flesh!"
#define SAY_IC_Deathwhisper_SP03      "Arise and exalt in your pure form!"
#define SAY_IC_Deathwhisper_SP04      "You are weak, powerless to resist my will!"
#define SAY_IC_Deathwhisper_Event01   "Fix your eyes upon your crude hands! The sinew, the soft meat, the dark blood coarsing within! It is a weakness! A cripping flaw! A joke played by the creators upon their own creations! The sooner you come to accept your condition as a defect, the sooner you will find yourselves in a position to transcend it!"
#define SAY_IC_Deathwhisper_Event02   "Through our master all things are possible! His power is without limit, and his will unbending! Those who oppose him will be destroyed utterly! And those who serve, who serve wholly, unquestioningly, with utter devotion of mind and soul? Elevated! To heights beyond your ken!"
#define SAY_IC_Deathwhisper_Event03   "You have found your way here, because you are among the few gifted with true vision in a world cursed with blindness! You can see through the fog that hangs over this world like a shroud and grasp where true power lies!"

// Lady Deathwhisper Sounds
#define SOUND_IC_Deathwhisper_Aggro01   16868
#define SOUND_IC_Deathwhisper_Death01   16871
#define SOUND_IC_Deathwhisper_Berserk01 16872
#define SOUND_IC_Deathwhisper_P2        16877
#define SOUND_IC_Deathwhisper_Slay01    16869
#define SOUND_IC_Deathwhisper_Slay02    16870
#define SOUND_IC_Deathwhisper_SP01      16873  
#define SOUND_IC_Deathwhisper_SP02      16874  
#define SOUND_IC_Deathwhisper_SP03      16875    
#define SOUND_IC_Deathwhisper_SP04      16876 
#define SOUND_IC_Deathwhisper_Event01A  16878
#define SOUND_IC_Deathwhisper_Event01B  17268
#define SOUND_IC_Deathwhisper_Event01C  17269
#define SOUND_IC_Deathwhisper_Event02A  17270
#define SOUND_IC_Deathwhisper_Event02B  17271
#define SOUND_IC_Deathwhisper_Event03A  17272
#define SOUND_IC_Deathwhisper_Event03B  17273

// Phase 1
#define SPELL_MANA_BARRIER      70842
#define SPELL_DEATH_AND_DECAY   71001
#define SPELL_DEATH_AND_DECAY_H 72108
#define SPELL_SHADOW_BOLT       71254
#define SPELL_SHADOW_BOLT_H     72008
#define SPELL_CHANNEL           46757
#define SPELL_TERIGGERED		41637

// Phase 2
#define SPELL_FROSTBOLT          72007
#define SPELL_FROSTBOLT_VOLLEY   72905
#define SPELL_FROSTBOLT_VOLLEY_H 72906
#define SPELL_SUMMON_SPIRIT      71426
#define SPELL_TOUCH              71204
#define SPELL_BERSERK            26662

//Cult Adherant
#define SPELL_CURSEOFTRPOR                71237
#define SPELL_DARKEMPOWERMENT             70901
#define SPELL_DARKMARTYDOM                70903
#define SPELL_DARKMARTYDOM_H			  72498
#define SPELL_DEATHCHILLBLAST             70594
#define SPELL_DEATHCHILLBLAST_H			  72005
#define SPELL_DEATHCHILLBLAST_EM          70906
#define SPELL_DEATHCHILLBLAST_EM_H		  72485
#define SPELL_SHROUDOFTHEOCCULT           70768
#define COSMETIC_TELEPORT                 52096

//Cult Fanatic
#define SPELL_DARK_MARTYRDOM              71236
#define SPELL_DARK_MARTYRDOM_H			  72495
#define SPELL_DARKTRANSFORMATION          70900
#define SPELL_NECROTICSTRIKE              70659
#define SPELL_NECROTICSTRIKE_H			  72490
#define SPELL_SHADOWCLEAVE                70670
#define SPELL_SHADOWCLEAVE_H			  72006
#define SPELL_VAMPIRICMIGHT               70674

// MOB vengeful shade
#define SPELL_VENGEFUL_SHADE    72010
#define SPELL_VENGEFUL_SHADE_H  72012

#define ADD_1X -619.006f
#define ADD_1Y 2158.104f
#define ADD_1Z 50.848f

#define ADD_2X -598.697f
#define ADD_2Y 2157.767f
#define ADD_2Z 50.848f

#define ADD_3X -577.992f
#define ADD_3Y 2156.989f
#define ADD_3Z 50.848f

#define ADD_4X -618.748f
#define ADD_4Y 2266.648f
#define ADD_4Z 50.849f

#define ADD_5X -598.573f
#define ADD_5Y 2266.870f
#define ADD_5Z 50.849f

#define ADD_6X -578.360f
#define ADD_6Y 2267.210f
#define ADD_6Z 50.849f

#define NPC_CULT_FANATIC  37890
#define NPC_CULT_ADHERENT 37949

/*
#define SAY_AGGRO -1
#define SAY_SLAY -1
*/

struct MANGOS_DLL_DECL boss_lady_deathwhisperAI : public ScriptedAI
{
    boss_lady_deathwhisperAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
		m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    std::list<uint64> m_lCult_FanaticGUIDList;
    std::list<uint64> m_lCult_AdherentGUIDList;
    uint32 Range_Check_Timer;
	
	uint32 Phase;
    uint32 Shadow_Bolt_Timer;
	uint32 Touch_Timer;
    uint32 Death_and_Decay_Timer;
    uint32 Summon_left_Timer;
    uint32 Summon_right_Timer;
    uint32 Frostbolt_Volley_Timer;
	uint32 Frostbolt_Timer;
    uint32 Summon_Spirit_Timer;
	uint32 m_uiEmpower_Timer;
	uint32 m_uiIntroTimer;
	uint32 m_uiIntroCount;
	uint32 m_uiBerserk_Timer;
	uint64 m_uiMana;
    //bool berserk;
	bool isManaBarrier;
	bool m_bIsIntroNow;

    void Reset()
    {
        Shadow_Bolt_Timer = 5000;
        Death_and_Decay_Timer = urand(15000,17000);
		m_uiEmpower_Timer = 20000;
        Summon_left_Timer = 5000;
        Summon_right_Timer = 60000;
        Frostbolt_Volley_Timer = 2000;
		Frostbolt_Timer = urand(16000, 28000);
        Summon_Spirit_Timer = 10000;
		Touch_Timer = 7000;
		m_uiBerserk_Timer = 600000;
		m_uiIntroTimer = 1000;
		m_uiIntroCount = 0;
        Phase = 1;
        //berserk = false;
		isManaBarrier = false;
		m_bIsIntroNow = false;

        m_lCult_FanaticGUIDList.clear();
        m_lCult_AdherentGUIDList.clear();
        Range_Check_Timer = 1000;

		 if(m_pInstance) m_pInstance->SetData(TYPE_DEATHWHISPER, NOT_STARTED);
    }

    void KilledUnit(Unit *victim)
    {
        switch (urand(0,1))
        {
            case 0:
                DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_Slay01);
                m_creature->MonsterYell(SAY_IC_Deathwhisper_Slay01, LANG_UNIVERSAL, 0);
                break;
               case 1:
                DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_Slay02);
                m_creature->MonsterYell(SAY_IC_Deathwhisper_Slay02, LANG_UNIVERSAL, 0);
                break;
        }
    }

    void JustDied(Unit *victim)
    {
		DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_Death01);
        m_creature->MonsterYell(SAY_IC_Deathwhisper_Death01, LANG_UNIVERSAL, 0);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_DEATHWHISPER, DONE);
    }

    void Aggro(Unit* pWho)
    {
		SetCombatMovement(false);
        m_creature->GetMotionMaster()->Clear(false);
        m_creature->GetMotionMaster()->MoveIdle();
        DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_Aggro01);
        m_creature->MonsterYell(SAY_IC_Deathwhisper_Aggro01, LANG_UNIVERSAL, 0);
		DoCast(m_creature, SPELL_MANA_BARRIER);
        m_creature->SetInCombatWithZone();
		isManaBarrier = true;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_DEATHWHISPER, IN_PROGRESS);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_MARROWGAR, FAIL);
    }

	void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_creature || !m_creature->isAlive())
            return;
         if (isManaBarrier) 
		 {
            m_creature->SetPower(POWER_MANA,m_creature->GetPower(POWER_MANA)-uiDamage*4);
            uiDamage = 0;
            if(m_creature->GetHealth() <= m_creature->GetMaxHealth()) 
			{
                m_uiMana = m_creature->GetPower(POWER_MANA)-(m_creature->GetMaxHealth()- m_creature->GetHealth())*2;
                   if (m_uiMana <= 0)
				   {
						 m_uiMana =0;
                         m_creature->SetPower(POWER_MANA,m_uiMana);
                         m_creature->SetHealth(m_creature->GetMaxHealth());
				   }
            }
          } 
		  else return;
    }

    void JustSummoned(Creature* summoned)
    {
        if (Unit* target = SelectUnit(SELECT_TARGET_TOPAGGRO,0))
            summoned->AI()->AttackStart(target);
    }

	/*void EndIntro()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        Intro = false;
        IsIntro = false;
        error_log("End Intro");
    }*/

	void MoveInLineOfSight(Unit* pWho)
    {
        if (!m_bIsIntroNow && m_creature->IsWithinDistInMap(pWho, 250.0f))
        {
            m_bIsIntroNow = true;
        }

        ScriptedAI::MoveInLineOfSight(pWho);
    }

    void UpdateAI(const uint32 diff)
    {
		if (!m_creature->getVictim())
		{
			if(m_uiIntroTimer < diff)
			{
				if(m_bIsIntroNow)
				{
					switch(m_uiIntroCount)
					{
						case 0:
							DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_Event01A);
							m_creature->MonsterYell(SAY_IC_Deathwhisper_Event01, LANG_UNIVERSAL, 0);
							m_uiIntroTimer = 8400; 
							break;
						case 1:
							DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_Event01B);
							m_uiIntroTimer = 7500;
							break;
						case 2:
							DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_Event01C);
							m_uiIntroTimer = 9000;
							break;
						case 3:
							DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_Event02A);
							m_creature->MonsterYell(SAY_IC_Deathwhisper_Event02, LANG_UNIVERSAL, 0);
							m_uiIntroTimer = 10000; 
							break;
						case 4:
							DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_Event02B);
							m_uiIntroTimer = 17000;
							break;
						case 5:
							DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_Event03A);
							m_creature->MonsterYell(SAY_IC_Deathwhisper_Event03, LANG_UNIVERSAL, 0);
							m_uiIntroTimer = 10000;
							break;
						case 6:
							DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_Event03B);
							break;
					}
                ++m_uiIntroCount;
				}
			}else m_uiIntroTimer -= diff;
		}

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Range_Check_Timer < diff)
        {
            if (!m_lCult_FanaticGUIDList.empty())
            {
                for(std::list<uint64>::iterator itr = m_lCult_FanaticGUIDList.begin(); itr != m_lCult_FanaticGUIDList.end(); ++itr)
                    if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
                        if (pTemp->isAlive() && m_creature->IsWithinDistInMap(pTemp, ATTACK_DISTANCE))
                        {
                            m_creature->SetHealth(m_creature->GetHealth() + m_creature->GetMaxHealth() * 0.01);
                            pTemp->DealDamage(pTemp, pTemp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        }
            }
            if (!m_lCult_AdherentGUIDList.empty())
            {
                for(std::list<uint64>::iterator itr = m_lCult_AdherentGUIDList.begin(); itr != m_lCult_AdherentGUIDList.end(); ++itr)
                    if (Creature* pTemp = (Creature*)Unit::GetUnit(*m_creature, *itr))
                        if (pTemp->isAlive() && m_creature->IsWithinDistInMap(pTemp, ATTACK_DISTANCE))
                        {
                            pTemp->DealDamage(pTemp, pTemp->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        }
            }
            Range_Check_Timer = 1000;
        }else Range_Check_Timer -= diff;

        if ( Summon_left_Timer < diff)
        {
            Unit* pSummonedleft= NULL;

            pSummonedleft = m_creature->SummonCreature(NPC_CULT_FANATIC,ADD_1X,ADD_1Y,ADD_1Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,120000);
            pSummonedleft = m_creature->SummonCreature(NPC_CULT_ADHERENT,ADD_2X,ADD_2Y,ADD_2Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,120000);
            pSummonedleft = m_creature->SummonCreature(NPC_CULT_FANATIC,ADD_3X,ADD_3Y,ADD_3Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,120000);
            if (pSummonedleft)
            {
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                    pSummonedleft->AddThreat(pTarget);
            }

             Summon_left_Timer = 120000;
        }
        else
             Summon_left_Timer -= diff;

        if ( Summon_right_Timer < diff)
        {
            Unit* pSummonedright= NULL;

            pSummonedright = m_creature->SummonCreature(NPC_CULT_ADHERENT,ADD_4X,ADD_4Y,ADD_4Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,120000);
            pSummonedright = m_creature->SummonCreature(NPC_CULT_FANATIC,ADD_5X,ADD_5Y,ADD_5Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,120000);
            pSummonedright = m_creature->SummonCreature(NPC_CULT_ADHERENT,ADD_6X,ADD_6Y,ADD_6Z,0,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,120000);
            if (pSummonedright)
            {
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                    pSummonedright->AddThreat(pTarget);
            }

             Summon_right_Timer = 120000;
        }
        else
             Summon_right_Timer -= diff;

		if (Death_and_Decay_Timer < diff)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
			{
                DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_DEATH_AND_DECAY : SPELL_DEATH_AND_DECAY_H, 1);
			}
            Death_and_Decay_Timer = 27000;
        }
		else Death_and_Decay_Timer -= diff;

		if (m_uiBerserk_Timer < diff)
        {
			DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_Berserk01);
			m_creature->MonsterYell(SAY_IC_Deathwhisper_Berserk01, LANG_UNIVERSAL, 0);
            DoCastSpellIfCan(m_creature, SPELL_BERSERK, 1);
			m_uiBerserk_Timer = 30000;
        }
        else  m_uiBerserk_Timer -= diff;
		
		if (Phase == 1)
		{
			if (Shadow_Bolt_Timer < diff)
			{
				if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
				{
					DoCast(target, m_bIsRegularMode ? SPELL_SHADOW_BOLT : SPELL_SHADOW_BOLT_H);
				}
				Shadow_Bolt_Timer = 5000;
			}
			else Shadow_Bolt_Timer -= diff;

			if (m_uiEmpower_Timer < diff)
			{ 
				if(Creature *pGuard = GetClosestCreatureWithEntry(m_creature, NPC_CULT_FANATIC, urand(20.0f,100.0f)))
				{
					DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_SP02);
					m_creature->MonsterYell(SAY_IC_Deathwhisper_SP02, LANG_UNIVERSAL, 0);
					DoCastSpellIfCan(pGuard, SPELL_TERIGGERED, 1);
				}
				else if(Creature *pGuard = GetClosestCreatureWithEntry(m_creature, NPC_CULT_ADHERENT, urand(70.0f,100.0f)))
				{
					DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_SP01);
					m_creature->MonsterYell(SAY_IC_Deathwhisper_SP01, LANG_UNIVERSAL, 0);
					DoCastSpellIfCan(pGuard, SPELL_TERIGGERED, 1);
				}
				m_uiEmpower_Timer = urand(20000,25000);
             } 
			else m_uiEmpower_Timer -= diff;

			if (m_creature->GetMaxPower(POWER_MANA) && (m_creature->GetPower(POWER_MANA)*100 / m_creature->GetMaxPower(POWER_MANA)) < 5)
			{
				Phase = 2;
				isManaBarrier = false;
				//DoStartMovement(m_creature->getVictim());
				SetCombatMovement(true);
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
				DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_P2);
				m_creature->MonsterYell(SAY_IC_Deathwhisper_P2, LANG_UNIVERSAL, 0);
				m_creature->RemoveAllAuras();
			}
		}
		
		if (Phase == 2)
		{
			if (Frostbolt_Volley_Timer < diff)
			{
				if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
				{
					DoCast(target, m_bIsRegularMode ? SPELL_FROSTBOLT_VOLLEY : SPELL_FROSTBOLT_VOLLEY_H);
				}
				Frostbolt_Volley_Timer = urand(18000,20000);
			}
			else Frostbolt_Volley_Timer -= diff;

			if (Touch_Timer < diff)
			{
				DoCast(m_creature->getVictim(),SPELL_TOUCH);
				Touch_Timer = 35000;
			}
			else Touch_Timer -= diff;

                //Check for Frostbolt - needs a raid warning introduced in sql script!
			if (Frostbolt_Timer < diff)
			{
				DoCast(m_creature->getVictim(),SPELL_FROSTBOLT);
				Frostbolt_Timer = urand(16000, 28000);
			}
			else Frostbolt_Timer -= diff;

			if (Summon_Spirit_Timer < diff)
			{
				if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
				{
					DoCast(target, m_bIsRegularMode ? SPELL_SUMMON_SPIRIT : SPELL_SUMMON_SPIRIT);
				}
				Summon_Spirit_Timer = urand(20000,25000);
			}
			else Summon_Spirit_Timer -= diff;
		
			DoMeleeAttackIfReady();
		}
    }

};

struct MANGOS_DLL_DECL mob_vengeful_shadeAI : public ScriptedAI
{
    mob_vengeful_shadeAI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = ((ScriptedInstance*)pCreature->GetInstanceData());
        Regular = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    uint32 m_uiRangeCheck_Timer;
	uint32 m_deathCheck_Timer;
    bool Regular;
	bool deathcheck;


    void Reset()
    {
        m_uiRangeCheck_Timer = 1000;
		m_deathCheck_Timer = 900;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetInCombatWithZone();
		deathcheck = false;
        if (Unit* pTarget= SelectUnit(SELECT_TARGET_RANDOM, 0) ) 
		{
                m_creature->GetMotionMaster()->MoveChase(pTarget);
                m_creature->SetSpeedRate(MOVE_RUN, 0.8f);
        } 
		else if (Unit* pTarget = Unit::GetUnit((*m_creature),m_pInstance->GetData64(NPC_LADY_DEATHWHISPER))) 
		{
                m_creature->GetMotionMaster()->MoveChase(pTarget);
                m_creature->SetSpeedRate(MOVE_RUN, 0.8f);
        }
    }


    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiRangeCheck_Timer < uiDiff)
        {
            if (m_pInstance)
            {
                    if (m_creature->IsWithinDist(m_creature->getVictim(), 2.0f, false))
                    {
                        DoCast(m_creature->getVictim(), Regular ? SPELL_VENGEFUL_BLAST_N : SPELL_VENGEFUL_BLAST_H);
                        deathcheck = true;
                    }
            }
            m_uiRangeCheck_Timer = 1000;
            if (m_creature->getVictim()) 
			{
				m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                m_creature->SetSpeedRate(MOVE_RUN, 0.8f);
            }
        }
        else m_uiRangeCheck_Timer -= uiDiff;

		if (m_deathCheck_Timer < uiDiff)
		{
			if (deathcheck)
			{
				m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
			}
			m_deathCheck_Timer = 900;
		}
		else m_deathCheck_Timer -= uiDiff;
    }

};

struct MANGOS_DLL_DECL npc_CultAdherentAI: public ScriptedAI
{
    npc_CultAdherentAI(Creature *c) : ScriptedAI(c)
    {
		pInstance = (ScriptedInstance*)c->GetInstanceData();
        Regular = c->GetMap()->IsRegularDifficulty();
        Reset();
    }
 
	bool Regular;
    ScriptedInstance *pInstance;

	bool bReanimated;
	bool bEmpowered;

	uint32 CURSEOFTRPOR_TIMER;
	uint32 DARKEMPOWERMENT_TIMER;
	uint32 DEATHCHILLBLAST_TIMER;
	uint32 SHROUDOFTHEOCCULT_TIMER;

    void Reset()
    {
		CURSEOFTRPOR_TIMER = 10000;
		DARKEMPOWERMENT_TIMER = 30000; 
		DEATHCHILLBLAST_TIMER = 20000;
		SHROUDOFTHEOCCULT_TIMER = 15000;
		bEmpowered = false;
		//bReanimated = false;
    }

    void JustSummoned(Creature* summoned)
    {
        if (Unit* target = SelectUnit(SELECT_TARGET_TOPAGGRO,0))
            summoned->AI()->AttackStart(target);

        DoCast(m_creature, COSMETIC_TELEPORT);
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }

    void EnterCombat(Unit* who)
    {
		DoCast(m_creature, COSMETIC_TELEPORT);
    }

	void SpellHit(Unit *caster, const SpellEntry *spell)
    {
		if(!bEmpowered)
		{
			if (spell->Id == SPELL_TERIGGERED)
			{
				DoCastSpellIfCan(m_creature, SPELL_DARKEMPOWERMENT, 1);
				bEmpowered = true;
			}
		}
    }

    void UpdateAI(const uint32 diff)
    {
         //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (CURSEOFTRPOR_TIMER < diff)
		{
			if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
			{
				DoCast(pTarget, SPELL_CURSEOFTRPOR);
			}
			CURSEOFTRPOR_TIMER = 8000;
		}
		else CURSEOFTRPOR_TIMER -= diff;
        
		/*if (DARKEMPOWERMENT_TIMER < diff)
		{
			DoCast(m_creature, SPELL_DARKEMPOWERMENT);
			DARKEMPOWERMENT_TIMER = 9999999;
			bEmpowered = true;
		}
		else DARKEMPOWERMENT_TIMER -= diff;*/

		if (DEATHCHILLBLAST_TIMER < diff)
		{
			if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
			{
				if (bEmpowered = false)
				{
					DoCast(pTarget, Regular ? SPELL_DEATHCHILLBLAST : SPELL_DEATHCHILLBLAST_H);
				} 
				else 
				{
					DoCast(pTarget, Regular ? SPELL_DEATHCHILLBLAST_EM : SPELL_DEATHCHILLBLAST_EM_H);
				}
			}
			DEATHCHILLBLAST_TIMER = 8000;
		}
		else DEATHCHILLBLAST_TIMER -= diff;
                    
        if (SHROUDOFTHEOCCULT_TIMER < diff)
		{
			DoCast(m_creature, SPELL_SHROUDOFTHEOCCULT);
			SHROUDOFTHEOCCULT_TIMER = 8000;
		}
		else SHROUDOFTHEOCCULT_TIMER -= diff;
               
        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_CultFanaticAI: public ScriptedAI
{
    npc_CultFanaticAI(Creature *c) : ScriptedAI(c)
    {
		pInstance = (ScriptedInstance*)c->GetInstanceData();
        Regular = c->GetMap()->IsRegularDifficulty();
        Reset();
    }
   
	bool Regular;
    ScriptedInstance *pInstance;

	uint32 NECROTICSTRIKE_TIMER;
	uint32 SHADOWCLEAVE_TIMER;
	uint32 VAMPIRICMIGHT_TIMER;

	//bool Martyr;
	bool Transform;

    void Reset()
    {
        NECROTICSTRIKE_TIMER = 8000;
		SHADOWCLEAVE_TIMER = 8000;
		VAMPIRICMIGHT_TIMER = 8000;
		//Martyr = false;
		Transform = false;
    }

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
    }


    void EnterCombat(Unit* who)
    {
		DoCast(m_creature, COSMETIC_TELEPORT);
    }

    void JustSummoned(Creature* summoned)
    {
        if (Unit* target = SelectUnit(SELECT_TARGET_TOPAGGRO,0))
            summoned->AI()->AttackStart(target);

        DoCast(m_creature, COSMETIC_TELEPORT);
    }

	void SpellHit(Unit *caster, const SpellEntry *spell)
    {
		if (!Transform)
		{
			if (spell->Id == SPELL_TERIGGERED)
			{
                DoCastSpellIfCan(m_creature, SPELL_DARKTRANSFORMATION, 1);
				Transform = true;
			}
		}
    }

    void UpdateAI(const uint32 diff)
    {
         //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        
                /*case EVENT_DARKMARTYRDOM:
					if ((Martyr = false) && (Transform = false))
                    DoCast(me, RAND(SPELL_DARKMARTYRDOM, SPELL_DARKTRANSFORMATION));
                    events.RescheduleEvent(EVENT_DARKMARTYRDOM, 999999);
                    return;*/
		if (NECROTICSTRIKE_TIMER < diff)
		{
			DoCast(m_creature->getVictim(), Regular ? SPELL_NECROTICSTRIKE : SPELL_NECROTICSTRIKE_H);
			NECROTICSTRIKE_TIMER = 13000;
		}
		else NECROTICSTRIKE_TIMER -= diff;
               
		if (SHADOWCLEAVE_TIMER < diff)
		{
			DoCast(m_creature->getVictim(), Regular ? SPELL_SHADOWCLEAVE : SPELL_SHADOWCLEAVE_H);
			SHADOWCLEAVE_TIMER = 10000;
		}
		else SHADOWCLEAVE_TIMER -= diff;
        
		if (VAMPIRICMIGHT_TIMER < diff)
		{
			DoCast(m_creature, SPELL_VAMPIRICMIGHT);
			VAMPIRICMIGHT_TIMER = 15000;
		}
		else VAMPIRICMIGHT_TIMER -= diff;
              
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_vengeful_shade(Creature* pCreature)
{
    return new mob_vengeful_shadeAI(pCreature);
}


CreatureAI* GetAI_boss_lady_deathwhisper(Creature* pCreature)
{
    return new boss_lady_deathwhisperAI(pCreature);
}

CreatureAI* GetAI_npc_CultAdherentAI(Creature* pCreature)
{
    return new npc_CultAdherentAI (pCreature);
}

CreatureAI* GetAI_npc_CultFanaticAI(Creature* pCreature)
{
    return new npc_CultFanaticAI (pCreature);
}

void AddSC_boss_lady_deathwhisper()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lady_deathwhisper";
    newscript->GetAI = &GetAI_boss_lady_deathwhisper;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_vengeful_shade";
    newscript->GetAI = &GetAI_mob_vengeful_shade;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name = "npc_cultadherent";
    newscript->GetAI = &GetAI_npc_CultAdherentAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_cultfanatic";
    newscript->GetAI = &GetAI_npc_CultFanaticAI;
    newscript->RegisterSelf();
}

/*void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (!m_creature || !m_creature->isAlive())
            return;
         if (isManaBarrier) 
		 {
            m_creature->SetPower(POWER_MANA,m_creature->GetPower(POWER_MANA)-uiDamage*4);
            uiDamage = 0;
            if(m_creature->GetHealth() <= m_creature->GetMaxHealth()) 
			{
                m_uiMana = m_creature->GetPower(POWER_MANA)-(m_creature->GetMaxHealth()- m_creature->GetHealth())*2;
                   if (m_uiMana <= 0)
				   {
						 m_uiMana =0;
                         m_creature->SetPower(POWER_MANA,m_uiMana);
                         m_creature->SetHealth(m_creature->GetMaxHealth());
				   }
            };

            if(m_creature->GetPower(POWER_MANA) <= m_creature->GetMaxPower(POWER_MANA)/10 ) 
			{
				DoPlaySoundToSet(m_creature, SOUND_IC_Deathwhisper_P2);
				m_creature->MonsterYell(SAY_IC_Deathwhisper_P2, LANG_UNIVERSAL, 0);
                m_creature->RemoveAurasDueToSpell(SPELL_MANA_BARRIER);
                isManaBarrier = false;
            };
          } 
		  else return;
    }*/