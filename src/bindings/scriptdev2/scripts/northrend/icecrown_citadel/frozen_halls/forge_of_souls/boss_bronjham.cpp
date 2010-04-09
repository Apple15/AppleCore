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
SDName: Bronjahm
SD%Complete: 0
SDComment: PH.
SDCategory: The Forge of Souls
EndScriptData */
#include "precompiled.h"
#include "the_forge_of_souls.h"

#define SAY_1						"Finally! A Captive Audience"
#define SOUND_1						16595
#define SAY_BRONJAHM_CORRUPT_SOUL	"I will sever your soul from your body"
#define SOUND_BRONJAHM_CORRUPT_SOUL 16600
#define SAY_BRONJAHM_SOULSTORM		"The vortex of the hovested calls for you!"
#define SOUND_BRONJAHM_SOULSTORM	16599
#define SAY_KILL1					"Another Soul to straight in the houst!"
#define SOUND_KILL1					16597
#define SAY_BRONJAHM_DEATH			"My soul for you. master"
#define SOUND_BRONJAHM_DEATH		16598

//define spells
#define SPELL_MAGIC_BANE	69050
#define SPELL_SHADOW_BOLT	70043
#define SPELL_CORRUPT_SOUL	68839
#define SPELL_CONSUME_SOUL	69047
#define SPELL_TELEPORT		68988
#define SPELL_SOULSTORM_x   68872
#define SPELL_SOULSTORM_1	68886
#define SPELL_SOULSTORM_2	68896
#define SPELL_SOULSTORM_3	68897
#define SPELL_SOULSTORM_4	68898
#define SPELL_SOULSTORM_5   68904
#define SPELL_SOULSTORM_6   68905
#define SPELL_SOULSTORM_7   68906
#define	SPELL_FEAR			68950	//with soulstorm v

struct MANGOS_DLL_DECL boss_bronjahmAI : public ScriptedAI
{
    boss_bronjahmAI(Creature *c) : ScriptedAI(c)
    {
        pInstance = (ScriptedInstance*)c->GetInstanceData();
        m_bIsRegularMode = c->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* pInstance;
	bool m_bIsRegularMode;

    uint32 Phase;
	uint32 Magic_Bane_Timer;
	uint32 Shadow_Bolt_Timer;
    uint32 Corrupt_Soul_Timer;
    uint32 Consume_Soul_Timer;
   	uint32 TELEPORT_Timer;
   	uint32 SOULSTORM_1_Timer;
	uint32 SOULSTORM_2_Timer;
	uint32 SOULSTORM_3_Timer;
	uint32 SOULSTORM_4_Timer;
	uint32 SOULSTORM_5_Timer;
	uint32 SOULSTORM_6_Timer;
	uint32 SOULSTORM_7_Timer;
	uint32 SOULSTORM_x_Timer;
   	uint32 FEAR_Timer;	
	bool InitialSpawn;
	bool Enraged;

	void Reset()
    {
        Phase = 1;

        Magic_Bane_Timer = 8000;
		Shadow_Bolt_Timer = urand(2000,5000);
        Corrupt_Soul_Timer = urand(10000,14000);
        Consume_Soul_Timer = 15000;
        TELEPORT_Timer = 1500;
        SOULSTORM_x_Timer = 1000;
        SOULSTORM_1_Timer = 1000;
		SOULSTORM_2_Timer = 1250;
		SOULSTORM_3_Timer = 1500;
		SOULSTORM_4_Timer = 1750;
		SOULSTORM_5_Timer = 2000;
		SOULSTORM_6_Timer = 2250;
		SOULSTORM_7_Timer = 2500;
        FEAR_Timer = urand (4000, 5000);
		
		Enraged = false;
		
        InitialSpawn = true;
    }

	 void Aggro(Unit *who)
    {
        if (pInstance)	
			DoPlaySoundToSet(m_creature, SOUND_1);
            m_creature->MonsterYell(SAY_1, LANG_UNIVERSAL,NULL);
    }

	 void KilledUnit(Unit *victim)
    {
		DoPlaySoundToSet(m_creature, SOUND_KILL1);
        m_creature->MonsterYell(SAY_KILL1, LANG_UNIVERSAL,NULL);        
    }

	 void JustDied(Unit *victim)
    {
		DoPlaySoundToSet(m_creature,SOUND_BRONJAHM_DEATH);
		m_creature->MonsterYell(SAY_BRONJAHM_DEATH, LANG_UNIVERSAL,NULL);
	 }

	void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 100) && (Phase == 1))
		{
			Phase = 2;
		}
		if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 75) && (Phase == 2))
		{
			Phase = 3;
		}
		if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 50) && (Phase == 3))
		{
			Phase = 4;
		}
		if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 40) && (Phase == 4))
		{
			Phase = 5;
			DoPlaySoundToSet(m_creature,SOUND_BRONJAHM_SOULSTORM);
            m_creature->MonsterYell(SAY_BRONJAHM_SOULSTORM, LANG_UNIVERSAL,NULL);
		}	
		if (((m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 29) && (Phase == 5))
		{
			Phase = 6;
		}

		if (Phase == 2)
		{
			if (Shadow_Bolt_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_SHADOW_BOLT);
                Shadow_Bolt_Timer = urand(2000,5000);
            } else Shadow_Bolt_Timer -=diff;
               
			if (Magic_Bane_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_MAGIC_BANE);
                Magic_Bane_Timer = urand(8000,10000);
            } else Magic_Bane_Timer -= diff;

            if (Corrupt_Soul_Timer < diff)
            {
				DoPlaySoundToSet(m_creature,SOUND_BRONJAHM_CORRUPT_SOUL);
				m_creature->MonsterYell(SAY_BRONJAHM_CORRUPT_SOUL, LANG_UNIVERSAL,NULL);
                DoCast(m_creature->getVictim(),SPELL_CORRUPT_SOUL);
                Corrupt_Soul_Timer = urand(12000,14000);
            } else Corrupt_Soul_Timer -=diff;
			 
        }

		/*if (Phase == 3)
		{
			if (TELEPORT_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_TELEPORT);
                TELEPORT_Timer = 2500;				   
            } else TELEPORT_Timer -=diff;
		}*/
		
		if (Phase == 4)
		{	
				
			if (SOULSTORM_1_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_SOULSTORM_1);
                SOULSTORM_1_Timer = 1000;				   
            } else SOULSTORM_1_Timer -=diff;
				
			if (SOULSTORM_2_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_SOULSTORM_2);
                SOULSTORM_2_Timer = 1250;				   
            } else SOULSTORM_2_Timer -=diff;
				
			if (SOULSTORM_3_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_SOULSTORM_3);
                SOULSTORM_3_Timer = 1500;				   
            } else SOULSTORM_3_Timer -=diff;
				
			if (SOULSTORM_4_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_SOULSTORM_4);
                SOULSTORM_4_Timer = 17500;				   
            } else SOULSTORM_4_Timer -=diff;
				
			if (SOULSTORM_5_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_SOULSTORM_5);
                SOULSTORM_5_Timer = 2000;				   
            } else SOULSTORM_5_Timer -=diff;
				
			if (SOULSTORM_6_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_SOULSTORM_6);
                SOULSTORM_6_Timer = 2250;				   
            } else SOULSTORM_6_Timer -=diff;
				
			if (SOULSTORM_7_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_SOULSTORM_7);
                SOULSTORM_7_Timer = 2500;				   
            } else SOULSTORM_7_Timer -=diff;
				
			if (SOULSTORM_x_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_SOULSTORM_x);
                SOULSTORM_x_Timer = 3000;				   
            } else SOULSTORM_x_Timer -=diff;
			
        }

			if (Phase == 5)
			{
				if (SOULSTORM_x_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_SOULSTORM_x);
                   SOULSTORM_x_Timer = 1000;				   
                } else SOULSTORM_x_Timer -=diff;
			}

			if (Phase == 6)
			{
				if (Shadow_Bolt_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),SPELL_SHADOW_BOLT);
                   Shadow_Bolt_Timer = urand(2000,5000);
                } else Shadow_Bolt_Timer -=diff;

				if (FEAR_Timer < diff)
				{
                   DoCast(m_creature->getVictim(),SPELL_FEAR);
                   FEAR_Timer = urand (4000, 5000);
                } else FEAR_Timer -= diff;
			}
			DoMeleeAttackIfReady();
	}
};

CreatureAI* GetAI_boss_bronjahm(Creature* pCreature)
{
    return new boss_bronjahmAI(pCreature);
}

void AddSC_boss_bronjahm()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_bronjahm";
    newscript->GetAI = &GetAI_boss_bronjahm;
    newscript->RegisterSelf();

}
/*UPDATE `creature_template` SET `ScriptName` = 'boss_bronjahm' WHERE `entry` =36497 LIMIT 1 ;*/