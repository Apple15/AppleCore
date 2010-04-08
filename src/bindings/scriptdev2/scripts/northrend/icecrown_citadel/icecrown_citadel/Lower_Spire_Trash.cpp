#include "precompiled.h"
#include "def_spire.h"

/***************************************SPELLS*************************************/
//Nerub'ar Broodkeeper
#define SPELL_CRYPTSCARABS                70965
#define SPELL_DARKMENDING                 71020
#define SPELL_WEBWRAP                     70980

//The Damned
#define SPELL_BONEFLURRY                  70960
#define SPELL_SHATTEREDBONES              70961

//Servant of the Throne
#define SPELL_GLACIALBLAST                71029

//Ancient Skeletal Soldier
#define SPELL_SHIELDBASH                  70964

//Deathbound Ward
#define SPELL_DISRUPTINGSHOUT             70122
#define SPELL_SABERLASH                   70121

//Deathspeaker Attendant
#define SPELL_SHADOWBOLT                  69387
#define SPELL_SHADOWNOVA                  69355

//Deathspeaker Disciple
#define SPELL_DARKBLESSING				  69391
#define SPELL_SHADOWBOLT2                 69387
#define SPELL_SHADOWMEND                  69389
#define SPELL_SHADOWMEND_H				  71107

//Deathspeaker High Priest
#define SPELL_AURAOFDARKNESS              69491
#define SPELL_DARKRECKONING               69483

//Deathspeaker Servant
#define SPELL_CHAOSBOLT					  69576
#define SPELL_CHAOSBOLT_H				  71108
#define SPELL_CONSUMINGSHADOWS            69405
#define SPELL_CURSEOFAGONY                69404
#define SPELL_CURSEOFAGONY_H			  71112

//Deathspeaker Zealot
#define SPELL_SHADOWCLEAVE                69492

/*****************************MARROWGAR TRASH****************************/
struct MANGOS_DLL_DECL npc_NerubarBroodkeeperAI: public ScriptedAI
{
    npc_NerubarBroodkeeperAI(Creature *c) : ScriptedAI(c)
    {
		pInstance = (ScriptedInstance*)c->GetInstanceData();
        Regular = c->GetMap()->IsRegularDifficulty();
        Reset();
    }

	bool Regular;
    ScriptedInstance *pInstance;

	uint32 CRYPTSCARABS_TIMER;
	uint32 DARKMENDING_TIMER;
	uint32 WEBWRAP_TIMER;


    void Reset()
    {
        CRYPTSCARABS_TIMER = 10000;
		DARKMENDING_TIMER = 17800;
		WEBWRAP_TIMER = 12000;
    }

    void UpdateAI(const uint32 diff)
    {
         //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (CRYPTSCARABS_TIMER < diff)
		{
				if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
				{
                    DoCast(pTarget, SPELL_CRYPTSCARABS);
				}
				CRYPTSCARABS_TIMER = 12000;
		}
		else CRYPTSCARABS_TIMER -= diff;

		if (DARKMENDING_TIMER < diff)
		{
			DoCast(m_creature, SPELL_DARKMENDING);
			DARKMENDING_TIMER = 17800;
		}
		else DARKMENDING_TIMER -= diff;
             
        if (WEBWRAP_TIMER < diff)
		{
			if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
			{
				DoCast(pTarget, SPELL_WEBWRAP);
			}
			WEBWRAP_TIMER =15000;
		}
		else WEBWRAP_TIMER -= diff;    
        
        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_TheDamnedAI: public ScriptedAI
{
    npc_TheDamnedAI(Creature *c) : ScriptedAI(c)
    {
		pInstance = (ScriptedInstance*)c->GetInstanceData();
        Regular = c->GetMap()->IsRegularDifficulty();
        Reset();
    }
    
	bool Regular;
    ScriptedInstance *pInstance;

	uint32 BONEFLURRY_TIMER;

    void Reset()
    {
       BONEFLURRY_TIMER = 8000;
    }

	void JustDied(Unit* who)
	{
		DoCast(m_creature, SPELL_SHATTEREDBONES);
	}

    void UpdateAI(const uint32 diff)
    {
         //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (BONEFLURRY_TIMER < diff)
		{
			DoCast(m_creature, SPELL_BONEFLURRY, 1);
			BONEFLURRY_TIMER = 15000;
		}
		else BONEFLURRY_TIMER -= diff;
             
        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_ServantoftheThroneAI: public ScriptedAI
{
    npc_ServantoftheThroneAI(Creature *c) : ScriptedAI(c)
    {
		pInstance = (ScriptedInstance*)c->GetInstanceData();
        Regular = c->GetMap()->IsRegularDifficulty();
        Reset();
    }
    
	bool Regular;
    ScriptedInstance *pInstance;

	uint32 GLACIALBLAST_TIMER;

    void Reset()
    {
        GLACIALBLAST_TIMER = 13000;
    }

    void UpdateAI(const uint32 diff)
    {
         //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		if (GLACIALBLAST_TIMER < diff)
		{
			if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
			{
				DoCast(pTarget, SPELL_GLACIALBLAST);
			}
			GLACIALBLAST_TIMER = 8000;
		}
		else GLACIALBLAST_TIMER -= diff;
             
        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_AncientSkeletalSoldierAI: public ScriptedAI
{
    npc_AncientSkeletalSoldierAI(Creature *c) : ScriptedAI(c)
    {
		pInstance = (ScriptedInstance*)c->GetInstanceData();
        Regular = c->GetMap()->IsRegularDifficulty();
        Reset();
    }
    
	bool Regular;
    ScriptedInstance *pInstance;

	uint32 SHIELDBASH_TIMER;

    void Reset()
    {
		SHIELDBASH_TIMER = 8000;
    }

    void UpdateAI(const uint32 diff)
    {
         //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		
		if (SHIELDBASH_TIMER < diff)
		{
			DoCast(m_creature->getVictim(), SPELL_SHIELDBASH);
			SHIELDBASH_TIMER = 8000;
		}
		else SHIELDBASH_TIMER -= diff;
               
        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_DeathboundWardAI: public ScriptedAI
{
    npc_DeathboundWardAI(Creature *c) : ScriptedAI(c)
    {
		pInstance = (ScriptedInstance*)c->GetInstanceData();
        Regular = c->GetMap()->IsRegularDifficulty();
        Reset();
    }
    
	bool Regular;
    ScriptedInstance *pInstance;

	uint32 DISRUPTINGSHOUT_TIMER;
	uint32 SABERLASH_TIMER;

    void Reset()
    {
        DISRUPTINGSHOUT_TIMER = 8000;
		SABERLASH_TIMER = 8000;
    }

    void UpdateAI(const uint32 diff)
    {
         //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		
		if (DISRUPTINGSHOUT_TIMER < diff)
		{
			DoCast(m_creature, SPELL_DISRUPTINGSHOUT);
			DISRUPTINGSHOUT_TIMER = 8000;
		}
		else DISRUPTINGSHOUT_TIMER -= diff;
        
		if (SABERLASH_TIMER < diff)
		{
			DoCast(m_creature->getVictim(), SPELL_SABERLASH);
			SABERLASH_TIMER = 8000;
		}
		else SABERLASH_TIMER -= diff;
              
        DoMeleeAttackIfReady();
    }
};

/*****************************DEATHWHISPER TRASH****************************/
struct MANGOS_DLL_DECL npc_DeathspeakerAttedantAI: public ScriptedAI
{
    npc_DeathspeakerAttedantAI(Creature *c) : ScriptedAI(c)
    {
		pInstance = (ScriptedInstance*)c->GetInstanceData();
        Regular = c->GetMap()->IsRegularDifficulty();
        Reset();
    }
    
	bool Regular;
    ScriptedInstance *pInstance;

	uint32 SHADOWBOLT_TIMER;
	uint32 SHADOWNOVA_TIMER;

    void Reset()
    {
       SHADOWBOLT_TIMER = 8000;
	   SHADOWNOVA_TIMER = 23000;
    }

    void UpdateAI(const uint32 diff)
    {
         //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (SHADOWBOLT_TIMER < diff)
		{
			if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
			{
				DoCast(pTarget, SPELL_SHADOWBOLT);
			}
			SHADOWBOLT_TIMER = 800;
		}
		else SHADOWBOLT_TIMER -= diff;
        
		if (SHADOWNOVA_TIMER < diff)
		{
			DoCast(m_creature, SPELL_SHADOWNOVA);
			SHADOWNOVA_TIMER = 17000;
		}
		else SHADOWNOVA_TIMER -= diff;
              
        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_DeathspeakerDiscipleAI: public ScriptedAI
{
    npc_DeathspeakerDiscipleAI(Creature *c) : ScriptedAI(c)
    {
		pInstance = (ScriptedInstance*)c->GetInstanceData();
        Regular = c->GetMap()->IsRegularDifficulty();
        Reset();
    }
    
	bool Regular;
    ScriptedInstance *pInstance;

	uint32 SHADOWBOLT2_TIMER;
	uint32 DARKBLESSING_TIMER;
	uint32 SHADOWMEND_TIMER;

    void Reset()
    {
        SHADOWBOLT2_TIMER = 8000;
		DARKBLESSING_TIMER = 20000;
		SHADOWMEND_TIMER = 25000;
    }

    void UpdateAI(const uint32 diff)
    {
         //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

		if (DARKBLESSING_TIMER < diff)
		{
			std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
            for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr != t_list.end(); ++itr)
            {
               Unit* pTarget = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
               if (pTarget && (pTarget->GetTypeId() == TYPEID_UNIT))
			   {
					if(pTarget->GetHealth() == pTarget->GetHealth() * 100 / pTarget->GetMaxHealth() && pTarget->IsFriendlyTo(m_creature))
					{
						DoCast(pTarget, SPELL_DARKBLESSING);
								
					}
				}
			}
			DARKBLESSING_TIMER = 20000;
		}
		else DARKBLESSING_TIMER -= diff;
                
        if (SHADOWBOLT2_TIMER < diff)
		{
			if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
			{
		       DoCast(pTarget, SPELL_SHADOWBOLT2);
			}
			SHADOWBOLT2_TIMER = 8000;
		}
		else SHADOWBOLT2_TIMER -= diff;

		if (SHADOWMEND_TIMER < diff)
		{
			std::list<HostileReference *> t_list = m_creature->getThreatManager().getThreatList();
            for(std::list<HostileReference *>::iterator itr = t_list.begin(); itr != t_list.end(); ++itr)
            {
                Unit* pTarget = Unit::GetUnit(*m_creature, (*itr)->getUnitGuid());
                if (pTarget && (pTarget->GetTypeId() == TYPEID_UNIT))
				{
					if(pTarget->GetHealth() == pTarget->GetHealth() * 100 / pTarget->GetMaxHealth() && pTarget->IsFriendlyTo(m_creature))
					{
						DoCast(pTarget, Regular ? SPELL_SHADOWMEND : SPELL_SHADOWMEND_H);
					}
				}
			}
			SHADOWMEND_TIMER = 25000;
		}
		else SHADOWMEND_TIMER -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_DeathspeakerHighPriestAI: public ScriptedAI
{
    npc_DeathspeakerHighPriestAI(Creature *c) : ScriptedAI(c)
    {
		pInstance = (ScriptedInstance*)c->GetInstanceData();
        Regular = c->GetMap()->IsRegularDifficulty();
        Reset();
    }
    
	bool Regular;
    ScriptedInstance *pInstance;

	uint32 DARKRECKONING_TIMER;

    void Reset()
    {
		DARKRECKONING_TIMER = 10000;
    }

    void EnterCombat(Unit* who)
    {
		DoCast(m_creature, SPELL_AURAOFDARKNESS);
    }

    void UpdateAI(const uint32 diff)
    {
         //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		
		if (DARKRECKONING_TIMER < diff)
		{
			if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
			{
				DoCast(pTarget, SPELL_DARKRECKONING);
			}
			DARKRECKONING_TIMER = 20000;
		}
		else DARKRECKONING_TIMER -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_DeathspeakerServantAI: public ScriptedAI
{
    npc_DeathspeakerServantAI(Creature *c) : ScriptedAI(c)
    {
		pInstance = (ScriptedInstance*)c->GetInstanceData();
        Regular = c->GetMap()->IsRegularDifficulty();
        Reset();
    }
    
	bool Regular;
    ScriptedInstance *pInstance;

	uint32 CHAOSBOLT_TIMER;
	uint32 CONSUMINGSHADOWS_TIMER;
	uint32 CURSEOFAGONY_TIMER;

    void Reset()
    {
        CHAOSBOLT_TIMER = 15000;
		CONSUMINGSHADOWS_TIMER = 13000;
		CURSEOFAGONY_TIMER = 10000;
    }

    void UpdateAI(const uint32 diff)
    {
         //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		
		if (CHAOSBOLT_TIMER < diff)
		{
			if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
			{
				DoCast(pTarget, Regular ? SPELL_CHAOSBOLT : SPELL_CHAOSBOLT_H);
			}
			CHAOSBOLT_TIMER = 15000;
		}
		else CHAOSBOLT_TIMER -= diff;
        
		if (CONSUMINGSHADOWS_TIMER < diff)
		{
			if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
			{
				DoCast(pTarget, SPELL_CONSUMINGSHADOWS);
			}
			CONSUMINGSHADOWS_TIMER = 13000;
		}
		else CONSUMINGSHADOWS_TIMER -= diff;
        
		if (CURSEOFAGONY_TIMER < diff)
		{
			if (Unit *pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
			{
				DoCast(pTarget, Regular ? SPELL_CURSEOFAGONY : SPELL_CURSEOFAGONY_H);
			}
			CURSEOFAGONY_TIMER = 17000;
		}
		else CURSEOFAGONY_TIMER -= diff;
               
        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_DeathspeakerZealotAI: public ScriptedAI
{
    npc_DeathspeakerZealotAI(Creature *c) : ScriptedAI(c)
    {
		pInstance = (ScriptedInstance*)c->GetInstanceData();
        Regular = c->GetMap()->IsRegularDifficulty();
        Reset();
    }
    
	bool Regular;
    ScriptedInstance *pInstance;

	uint32 SHADOWCLEAVE_TIMER;

    void Reset()
    {
       SHADOWCLEAVE_TIMER = 8000;
    }

    void UpdateAI(const uint32 diff)
    {
         //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
		if (SHADOWCLEAVE_TIMER < diff)
		{
			DoCast(m_creature->getVictim(), SPELL_SHADOWCLEAVE);
			SHADOWCLEAVE_TIMER = 8000;
		}
		else SHADOWCLEAVE_TIMER -= diff;
                
        DoMeleeAttackIfReady();
    }
};

/*****************************MARROWGAR TRASH****************************/
CreatureAI* GetAI_npc_NerubarBroodkeeperAI(Creature* pCreature)
{
    return new npc_NerubarBroodkeeperAI (pCreature);
}

CreatureAI* GetAI_npc_TheDamnedAI(Creature* pCreature)
{
    return new npc_TheDamnedAI (pCreature);
}

CreatureAI* GetAI_npc_ServantoftheThroneAI(Creature* pCreature)
{
    return new npc_ServantoftheThroneAI (pCreature);
}

CreatureAI* GetAI_npc_AncientSkeletalSoldierAI(Creature* pCreature)
{
    return new npc_AncientSkeletalSoldierAI (pCreature);
}

CreatureAI* GetAI_npc_DeathboundWardAI(Creature* pCreature)
{
    return new npc_DeathboundWardAI (pCreature);
}

/*****************************DEATHWHISPER TRASH**************************/
CreatureAI* GetAI_npc_DeathspeakerAttedantAI(Creature* pCreature)
{
    return new npc_DeathspeakerAttedantAI (pCreature);
}

CreatureAI* GetAI_npc_DeathspeakerDiscipleAI(Creature* pCreature)
{
    return new npc_DeathspeakerDiscipleAI (pCreature);
}

CreatureAI* GetAI_npc_DeathspeakerHighPriestAI(Creature* pCreature)
{
    return new npc_DeathspeakerHighPriestAI (pCreature);
}

CreatureAI* GetAI_npc_DeathspeakerServantAI(Creature* pCreature)
{
    return new npc_DeathspeakerServantAI (pCreature);
}

CreatureAI* GetAI_npc_DeathspeakerZealotAI(Creature* pCreature)
{
    return new npc_DeathspeakerZealotAI (pCreature);
}

void AddSC_LowerSpireTrash()
{
/*****************************MARROWGAR TRASH****************************/
    Script *newscript;
    newscript = new Script;
    newscript->Name="npc_NerubarBroodkeeper";
    newscript->GetAI = &GetAI_npc_NerubarBroodkeeperAI;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name="npc_TheDamned";
    newscript->GetAI = &GetAI_npc_TheDamnedAI;
    newscript->RegisterSelf();

	newscript = new Script;
    newscript->Name="npc_ServantoftheThrone";
    newscript->GetAI = &GetAI_npc_ServantoftheThroneAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_AncientSkeletalSoldier";
    newscript->GetAI = &GetAI_npc_AncientSkeletalSoldierAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_DeathboundWard";
    newscript->GetAI = &GetAI_npc_DeathboundWardAI;
    newscript->RegisterSelf();

/*****************************DEATHWHISPER TRASH**************************/
    newscript = new Script;
    newscript->Name="npc_DeathspeakerAttedant";
    newscript->GetAI = &GetAI_npc_DeathspeakerAttedantAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_DeathspeakerDisciple";
    newscript->GetAI = &GetAI_npc_DeathspeakerDiscipleAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_DeathspeakerHighPriest";
    newscript->GetAI = &GetAI_npc_DeathspeakerHighPriestAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_DeathspeakerServant";
    newscript->GetAI = &GetAI_npc_DeathspeakerServantAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name="npc_DeathspeakerZealot";
    newscript->GetAI = &GetAI_npc_DeathspeakerZealotAI;
    newscript->RegisterSelf();
}