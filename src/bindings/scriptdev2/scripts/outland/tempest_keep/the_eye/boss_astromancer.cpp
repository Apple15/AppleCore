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
SDName: Boss_Astromancer
SD%Complete: 80
SDComment:
SDCategory: Tempest Keep, The Eye
EndScriptData */

#include "precompiled.h"
#include "the_eye.h"

enum
{
    SAY_AGGRO                           = -1550007,
    SAY_SUMMON1                         = -1550008,
    SAY_SUMMON2                         = -1550009,
    SAY_KILL1                           = -1550010,
    SAY_KILL2                           = -1550011,
    SAY_KILL3                           = -1550012,
    SAY_DEATH                           = -1550013,
    SAY_VOIDA                           = -1550014,
    SAY_VOIDB                           = -1550015,

    SPELL_ARCANE_MISSILES               = 33031,
    SPELL_WRATH_OF_THE_ASTROMANCER      = 42783,
    SPELL_BLINDING_LIGHT                = 33009,
    SPELL_FEAR                          = 34322,
    SPELL_VOID_BOLT                     = 39329,

    SPELL_SPOTLIGHT                     = 25824,
    NPC_ASTROMANCER_SOLARIAN_SPOTLIGHT  = 18928,

    NPC_SOLARIUM_AGENT                  = 18925,
    NPC_SOLARIUM_PRIEST                 = 18806,

    MODEL_HUMAN                         = 18239,
    MODEL_VOIDWALKER                    = 18988,

    SPELL_SOLARIUM_GREAT_HEAL           = 33387,
    SPELL_SOLARIUM_HOLY_SMITE           = 25054,
    SPELL_SOLARIUM_ARCANE_TORRENT       = 33390,

    WV_ARMOR                            = 31000
};

const float CENTER_X                    = 432.909f;
const float CENTER_Y                    = -373.424f;
const float CENTER_Z                    = 17.9608f;
const float CENTER_O                    = 1.06421f;
const float SMALL_PORTAL_RADIUS         = 12.6f;
const float LARGE_PORTAL_RADIUS         = 26.0f;
const float PORTAL_Z                    = 17.005f;

struct MANGOS_DLL_DECL boss_high_astromancer_solarianAI : public ScriptedAI
{
    boss_high_astromancer_solarianAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

        defaultarmor = m_creature->GetArmor();
        defaultsize = m_creature->GetFloatValue(OBJECT_FIELD_SCALE_X);
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 ArcaneMissiles_Timer;
    uint32 m_uiWrathOfTheAstromancer_Timer;
    uint32 BlindingLight_Timer;
    uint32 Fear_Timer;
    uint32 VoidBolt_Timer;
    uint32 Phase1_Timer;
    uint32 Phase2_Timer;
    uint32 Phase3_Timer;
    uint32 AppearDelay_Timer;
    uint32 defaultarmor;

    float defaultsize;

    bool AppearDelay;

    uint8 Phase;

    float Portals[3][3];

    void Reset()
    {
        ArcaneMissiles_Timer = 2000;
        m_uiWrathOfTheAstromancer_Timer = 15000;
        BlindingLight_Timer = 41000;
        Fear_Timer = 20000;
        VoidBolt_Timer = 10000;
        Phase1_Timer = 50000;
        Phase2_Timer = 10000;
        Phase3_Timer = 15000;
        AppearDelay_Timer = 2000;
        AppearDelay = false;
        Phase = 1;

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ASTROMANCER, NOT_STARTED);

        m_creature->SetArmor(defaultarmor);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetVisibility(VISIBILITY_ON);
        m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, defaultsize);
        m_creature->SetDisplayId(MODEL_HUMAN);
    }

    void KilledUnit(Unit *victim)
    {
        switch(urand(0, 2))
        {
            case 0: DoScriptText(SAY_KILL1, m_creature); break;
            case 1: DoScriptText(SAY_KILL2, m_creature); break;
            case 2: DoScriptText(SAY_KILL3, m_creature); break;
        }
    }

    void JustDied(Unit *victim)
    {
        m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, defaultsize);
        m_creature->SetDisplayId(MODEL_HUMAN);

        DoScriptText(SAY_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ASTROMANCER, DONE);
    }

    void Aggro(Unit *who)
    {
        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->SetInCombatWithZone();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_ASTROMANCER, IN_PROGRESS);
    }

    void SummonMinion(uint32 entry, float x, float y, float z)
    {
        Creature* Summoned = m_creature->SummonCreature(entry, x, y, z, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 5000);
        if (Summoned)
        {
            if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                Summoned->AI()->AttackStart(target);
        }
    }

    float Portal_X(float radius)
    {
        if (urand(0, 1))
            radius = -radius;

        return (radius * (float)(rand()%100)/100.0f + CENTER_X);
    }

    float Portal_Y(float x, float radius)
    {
        float z = 0.0f;

        switch(urand(0, 1))
        {
            case 0: z = 1; break;
            case 1: z = -1; break;
        }
        return (z*sqrt(radius*radius - (x - CENTER_X)*(x - CENTER_X)) + CENTER_Y);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (AppearDelay)
        {
            m_creature->StopMoving();
            m_creature->AttackStop();

            if (AppearDelay_Timer < diff)
            {
                AppearDelay = false;

                if (Phase == 2)
                {
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    m_creature->SetVisibility(VISIBILITY_OFF);
                }

                AppearDelay_Timer = 2000;
            }else AppearDelay_Timer -= diff;
        }

        if (Phase == 1)
        {
            //ArcaneMissiles_Timer
            if (ArcaneMissiles_Timer < diff)
            {
                //Solarian casts Arcane Missiles on on random targets in the raid.
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                {
                    if (!m_creature->HasInArc(2.5f, target))
                        target = m_creature->getVictim();

                    if (target)
                        DoCastSpellIfCan(target, SPELL_ARCANE_MISSILES);
                }

                ArcaneMissiles_Timer = 3000;
            }else ArcaneMissiles_Timer -= diff;

            //Wrath of the Astromancer targets a random player which will explode after 6 secondes
            if (m_uiWrathOfTheAstromancer_Timer < diff)
            {
                m_creature->InterruptNonMeleeSpells(false);

                //Target the tank ?
                if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 1))
                {
                    if (pTarget->GetTypeId() == TYPEID_PLAYER)
                    {
                        DoCastSpellIfCan(pTarget, SPELL_WRATH_OF_THE_ASTROMANCER);
                        m_uiWrathOfTheAstromancer_Timer = 25000;
                    }
                    else
                        m_uiWrathOfTheAstromancer_Timer = 1000;
                }
            }else m_uiWrathOfTheAstromancer_Timer -= diff;

            //BlindingLight_Timer
            if (BlindingLight_Timer < diff)
            {
                //She casts this spell every 45 seconds. It is a kind of Moonfire spell, which she strikes down on the whole raid simultaneously. It hits everyone in the raid for 2280 to 2520 arcane damage.
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_BLINDING_LIGHT);
                BlindingLight_Timer = 45000;
            }else BlindingLight_Timer -= diff;

            //Phase1_Timer
            if (Phase1_Timer < diff)
            {
                Phase = 2;
                Phase1_Timer = 50000;

                //After these 50 seconds she portals to the middle of the room and disappears, leaving 3 light portals behind.
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMap()->CreatureRelocation(m_creature, CENTER_X, CENTER_Y, CENTER_Z, CENTER_O);

                for(int i = 0; i <= 2; ++i)
                {
                    if (!i)
                    {
                        Portals[i][0] = Portal_X(SMALL_PORTAL_RADIUS);
                        Portals[i][1] = Portal_Y(Portals[i][0], SMALL_PORTAL_RADIUS);
                        Portals[i][2] = CENTER_Z;
                    }
                    else
                    {
                        Portals[i][0] = Portal_X(LARGE_PORTAL_RADIUS);
                        Portals[i][1] = Portal_Y(Portals[i][0], LARGE_PORTAL_RADIUS);
                        Portals[i][2] = PORTAL_Z;
                    }
                }

                if ((abs(int(Portals[2][0]) - int(Portals[1][0])) < 7) && (abs(int(Portals[2][1]) - int(Portals[1][1])) < 7))
                {
                    int i = 1;
                    if (abs(int(CENTER_X) + int(26.0f) - int(Portals[2][0])) < 7)
                        i = -1;

                    Portals[2][0] = Portals[2][0]+7*i;
                    Portals[2][1] = Portal_Y(Portals[2][0], LARGE_PORTAL_RADIUS);
                }

                for (int i = 0; i <= 2; ++i)
                {
                    if (Creature* Summoned = m_creature->SummonCreature(NPC_ASTROMANCER_SOLARIAN_SPOTLIGHT, Portals[i][0], Portals[i][1], Portals[i][2], CENTER_O, TEMPSUMMON_TIMED_DESPAWN, Phase2_Timer+Phase3_Timer+AppearDelay_Timer+1700))
                    {
                        Summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        Summoned->CastSpell(Summoned, SPELL_SPOTLIGHT, false);
                    }
                }

                AppearDelay = true;
            }else Phase1_Timer -= diff;
        }
        else if (Phase == 2)
        {
            m_creature->AttackStop();
            m_creature->StopMoving();

            //Check Phase2_Timer
            if (Phase2_Timer < diff)
            {
                //10 seconds after Solarian disappears, 12 mobs spawn out of the three portals.
                Phase = 3;
                for (int i = 0; i <= 2; ++i)
                {
                    for (int j = 1; j <= 4; ++j)
                        SummonMinion(NPC_SOLARIUM_AGENT, Portals[i][0], Portals[i][1], Portals[i][2]);
                }

                DoScriptText(SAY_SUMMON1, m_creature);

                Phase2_Timer = 10000;
            } else Phase2_Timer -= diff;
        }
        else if (Phase == 3)
        {
            m_creature->AttackStop();
            m_creature->StopMoving();

            //Check Phase3_Timer
            if (Phase3_Timer < diff)
            {
                Phase = 1;

                //15 seconds later Solarian reappears out of one of the 3 portals. Simultaneously, 2 healers appear in the two other portals.
                int i = irand(0, 2);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMap()->CreatureRelocation(m_creature, Portals[i][0], Portals[i][1], Portals[i][2], CENTER_O);

                for (int j = 0; j <= 2; ++j)
                    if (j != i)
                        SummonMinion(NPC_SOLARIUM_PRIEST, Portals[j][0], Portals[j][1], Portals[j][2]);

                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                m_creature->SetVisibility(VISIBILITY_ON);

                DoScriptText(SAY_SUMMON2, m_creature);

                AppearDelay = true;
                Phase3_Timer = 15000;
            }else Phase3_Timer -= diff;
        }
        else if (Phase == 4)
        {
            //Fear_Timer
            if (Fear_Timer < diff)
            {
                DoCastSpellIfCan(m_creature, SPELL_FEAR);
                Fear_Timer = 20000;
            }else Fear_Timer -= diff;

            //VoidBolt_Timer
            if (VoidBolt_Timer < diff)
            {
                DoCastSpellIfCan(m_creature->getVictim(), SPELL_VOID_BOLT);
                VoidBolt_Timer = 10000;
            }else VoidBolt_Timer -= diff;
        }

        //When Solarian reaches 20% she will transform into a huge void walker.
        if (Phase != 4 && m_creature->GetHealthPercent() < 20.0f)
        {
            Phase = 4;

            //To make sure she wont be invisible or not selecatble
            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_creature->SetVisibility(VISIBILITY_ON);

            DoScriptText(SAY_VOIDA, m_creature);
            DoScriptText(SAY_VOIDB, m_creature);

            m_creature->SetArmor(WV_ARMOR);
            m_creature->SetDisplayId(MODEL_VOIDWALKER);
            m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, defaultsize*2.5f);
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_solarium_priestAI : public ScriptedAI
{
    mob_solarium_priestAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint32 healTimer;
    uint32 holysmiteTimer;
    uint32 aoesilenceTimer;

    void Reset()
    {
        healTimer = 9000;
        holysmiteTimer = 1;
        aoesilenceTimer = 15000;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (healTimer < diff)
        {
            Unit* target = NULL;

            switch(urand(0, 1))
            {
                case 0:
                    if (m_pInstance)
                        target = Unit::GetUnit((*m_creature), m_pInstance->GetData64(DATA_ASTROMANCER));
                    break;
                case 1:
                    target = m_creature;
                    break;
            }

            if (target)
            {
                DoCastSpellIfCan(target,SPELL_SOLARIUM_GREAT_HEAL);
                healTimer = 9000;
            }
        } else healTimer -= diff;

        if (holysmiteTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SOLARIUM_HOLY_SMITE);
            holysmiteTimer = 4000;
        } else holysmiteTimer -= diff;

        if (aoesilenceTimer < diff)
        {
            DoCastSpellIfCan(m_creature->getVictim(), SPELL_SOLARIUM_ARCANE_TORRENT);
            aoesilenceTimer = 13000;
        } else aoesilenceTimer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_solarium_priest(Creature* pCreature)
{
    return new mob_solarium_priestAI(pCreature);
}

CreatureAI* GetAI_boss_high_astromancer_solarian(Creature* pCreature)
{
    return new boss_high_astromancer_solarianAI(pCreature);
}

void AddSC_boss_high_astromancer_solarian()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_high_astromancer_solarian";
    newscript->GetAI = &GetAI_boss_high_astromancer_solarian;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_solarium_priest";
    newscript->GetAI = &GetAI_mob_solarium_priest;
    newscript->RegisterSelf();
}