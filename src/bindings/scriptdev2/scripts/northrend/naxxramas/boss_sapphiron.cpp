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
SDName: Boss_Sapphiron
SD%Complete: 0
SDComment: Place Holder
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

#define EMOTE_BREATH            -1533082
#define EMOTE_ENRAGE            -1533083

#define SPELL_ICEBOLT           28522
#define SPELL_FROST_BREATH      29318
#define SPELL_FROST_BREATH_H    28524
#define SPELL_FROST_AURA        28531
#define SPELL_LIFE_DRAIN        28542
#define SPELL_LIFE_DRAIN_H      55665
#define SPELL_BLIZZARD          28547
#define SPELL_BESERK            26662
#define SPELL_ICEBOLT_VISUAL    45776
#define SPELL_CLEAVE            19983
#define SPELL_TAIL_LASH         55697
#define SPELL_TAIL_LASH_H       55696

#define SAPPHIRON_X             3522.0f
#define SAPPHIRON_Y             -5236.0f
#define SAPPHIRON_Z             137.0f

struct MANGOS_DLL_DECL boss_sapphironAI : public ScriptedAI
{
    boss_sapphironAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    bool m_bIsRegularMode;

    uint32 Icebolt_Count;
    uint32 Icebolt_Timer;
    uint32 FrostBreath_Timer;
    uint32 FrostAura_Timer;
    uint32 LifeDrain_Timer;
    uint32 Blizzard_Timer;
    uint32 Fly_Timer;
    uint32 Beserk_Timer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiTailSweepTimer;
    uint32 phase;
    bool landoff;
    uint32 land_Timer;
    std::vector<Unit*> targets;

    void Reset()
    {
        FrostAura_Timer = 2000;
        FrostBreath_Timer = 2500;
        LifeDrain_Timer = 24000;
        Blizzard_Timer = 20000;
        Fly_Timer = 45000;
        Icebolt_Timer = 4000;
        land_Timer = 2000;
        Beserk_Timer = 900000;
        m_uiCleaveTimer = 7000;
        m_uiTailSweepTimer = 20000;
        phase = 1;
        Icebolt_Count = 0;
        landoff = false;

        targets.clear();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, NOT_STARTED);
        //m_creature->ApplySpellMod(SPELL_FROST_AURA, SPELLMOD_DURATION, -1);
    }

    void Aggro(Unit* who)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, IN_PROGRESS);
    }

    /*void MoveInLineOfSight(Unit* who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who->GetTypeId() == TYPEID_PLAYER)
        {
            if (m_pInstance)
            {
                if (m_pInstance->GetData(TYPE_SAPPHIRON) == NOT_STARTED)
                {
                    if (m_pInstance->GetData(TYPE_THADDIUS) == DONE && m_pInstance->GetData(TYPE_MAEXXNA) == DONE &&
                        m_pInstance->GetData(TYPE_FOUR_HORSEMEN) == DONE && m_pInstance->GetData(TYPE_LOATHEB) == DONE)
                    {
                        if (m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                            m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                    else
                    {
                        if (!m_creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
            }
        }
    }*/

    void SpellHitTarget(Unit *target, const SpellEntry *spell)
    {
        if(spell->Id == SPELL_ICEBOLT)
        {
                    if (target->isAlive() && target->HasAura(SPELL_ICEBOLT))
                    {
                        target->CastSpell(target, 62766, true);
                        target->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
                    }

/*            for(std::vector<Unit*>::iterator itr = targets.begin(); itr!= targets.end(); ++itr)
            {
                if (*itr)
                {
                    if (target->isAlive() && (*itr)->GetGUID() == target->GetGUID() && !((*itr)->HasAura(SPELL_ICEBOLT)))
                    {
                        target->CastSpell(target, 62766, true);
                        target->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
                    }
            	}
            }
*/
            return;
        }

        if(spell->Id == SPELL_FROST_BREATH_H)
        {
            if (target->GetTypeId() != TYPEID_PLAYER)
                return;

            if (target->HasAura(SPELL_ICEBOLT))
            {
                target->RemoveAurasDueToSpell(62766);
                target->RemoveAurasDueToSpell(SPELL_ICEBOLT);
                target->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, false);
                return;
            }
        }
    }

    void JustDied(Unit* pKiller)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SAPPHIRON, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (phase == 1)
        {
            if (FrostAura_Timer < diff)
            {
                DoCast(m_creature->getVictim(),SPELL_FROST_AURA);
                FrostAura_Timer = 5000;
            }else FrostAura_Timer -= diff;

            if (LifeDrain_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target, m_bIsRegularMode ? SPELL_LIFE_DRAIN_H : SPELL_LIFE_DRAIN);

                LifeDrain_Timer = 24000;
            }else LifeDrain_Timer -= diff;

            if (Blizzard_Timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    DoCast(target,SPELL_BLIZZARD);

                Blizzard_Timer = 20000;
            }else Blizzard_Timer -= diff;

            // Cleave
            if (m_uiCleaveTimer < diff)
            {
                DoCast(m_creature->getVictim(), SPELL_CLEAVE);
                m_uiCleaveTimer = 7000 + rand()%3000;
            }
            else
                m_uiCleaveTimer -= diff;

            // Tail Sweep
            if (m_uiTailSweepTimer < diff)
            {
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? SPELL_TAIL_LASH_H : SPELL_TAIL_LASH);
                m_uiTailSweepTimer = 15000 + rand()%5000;
            }
            else
                m_uiTailSweepTimer -= diff;

            if (m_creature->GetHealth()*100 / m_creature->GetMaxHealth() > 10)
            {
                if (Fly_Timer < diff)
                {
                    phase = 2;
                    m_creature->InterruptNonMeleeSpells(false);
                    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LIFTOFF);
                    m_creature->StopMoving();
                    m_creature->GetMotionMaster()->Clear(false);
                    m_creature->GetMotionMaster()->MoveIdle();
                    m_creature->GetMap()->CreatureRelocation(m_creature, SAPPHIRON_X, SAPPHIRON_Y, SAPPHIRON_Z + 2.0f, m_creature->GetOrientation());
					m_creature->SendMonsterMove(SAPPHIRON_X, SAPPHIRON_Y, SAPPHIRON_Z + 20, SPLINETYPE_NORMAL, m_creature->GetSplineFlags(), 1);
                    //DoCast(m_creature,11010);
                    m_creature->SetHover(true);
                    //DoCast(m_creature,18430);
                    Icebolt_Timer = 4000;
                    Icebolt_Count = 0;
                    landoff = false;
                }else Fly_Timer -= diff;
            }
        }

        if (phase == 2)
        {
            if (Icebolt_Timer < diff && Icebolt_Count < 5)
            {
                if (Icebolt_Count == 1 || Icebolt_Count == 3)
                    if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
                    {
                        DoCast(target, SPELL_ICEBOLT);
                        targets.push_back(target);
                    }

                ++Icebolt_Count;
                Icebolt_Timer = 4000;
            }else Icebolt_Timer -= diff;

            if (Icebolt_Count == 5 && !landoff)
            {
                if (FrostBreath_Timer < diff)
                {
                    // apply immune
                    std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
                    for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr != t_list.end(); ++itr)
                    {
                        Unit* pUnit = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
                        if (pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER))
                        {
                            if (!pUnit->HasAura(SPELL_ICEBOLT))
                            {
                                for(std::vector<Unit*>::iterator itr2 = targets.begin(); itr2 != targets.end(); ++itr2)
                                {
                                    if (*itr)
                                    {
                                        if (pUnit->GetDistance2d(*itr2) <= 10 && (*itr2)->HasAura(SPELL_ICEBOLT))
                                            pUnit->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, true);
                                    }
                                }
                            }
                        }
                    }

                    DoScriptText(EMOTE_BREATH, m_creature);
                    DoCast(m_creature->getVictim(),SPELL_FROST_BREATH_H);
                    land_Timer = 2000;
                    landoff = true;
                    FrostBreath_Timer = 6000;
                }else FrostBreath_Timer -= diff;
            }

            if (landoff)
            {
                if (land_Timer < diff)
                {
                    phase = 1;
                    m_creature->HandleEmoteCommand(EMOTE_ONESHOT_LAND);
                    m_creature->SetHover(false);
                    //m_creature->GetMotionMaster()->Clear(false);
                    //m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    DoStartMovement(m_creature->getVictim());

                    std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
                    for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr != t_list.end(); ++itr)
                    {
                        Unit* pUnit = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
                        if (pUnit && (pUnit->GetTypeId() == TYPEID_PLAYER))
                            pUnit->ApplySpellImmune(0, IMMUNITY_SCHOOL, SPELL_SCHOOL_MASK_FROST, false);
                    }

                    targets.clear();
                    Fly_Timer = 67000;
                }else land_Timer -= diff;
            }
        }

        if (Beserk_Timer < diff)
        {
            DoScriptText(EMOTE_ENRAGE, m_creature);
            DoCast(m_creature,SPELL_BESERK, 1);
            Beserk_Timer = 900000;
        }else Beserk_Timer -= diff;

        if (phase!=2)
            DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_sapphiron(Creature* pCreature)
{
    return new boss_sapphironAI(pCreature);
}

void AddSC_boss_sapphiron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_sapphiron";
    newscript->GetAI = &GetAI_boss_sapphiron;
    newscript->RegisterSelf();
}
