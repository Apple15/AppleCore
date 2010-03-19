/* Templates for our Zen Master and all of our class trainers */
REPLACE INTO `creature_template`(`entry`,`difficulty_entry_1`,`difficulty_entry_2`,`difficulty_entry_3`,`KillCredit1`,`KillCredit2`,`modelid_A`,`modelid_A2`,`modelid_H`,`modelid_H2`,`name`,`subname`,`IconName`,`gossip_menu_id`,`minlevel`,`maxlevel`,`minhealth`,`maxhealth`,`minmana`,`maxmana`,`armor`,`faction_A`,`faction_H`,`npcflag`,`speed`,`scale`,`rank`,`mindmg`,`maxdmg`,`dmgschool`,`attackpower`,`dmg_multiplier`,`baseattacktime`,`rangeattacktime`,`unit_class`,`unit_flags`,`dynamicflags`,`family`,`trainer_type`,`trainer_spell`,`trainer_class`,`trainer_race`,`minrangedmg`,`maxrangedmg`,`rangedattackpower`,`type`,`type_flags`,`lootid`,`pickpocketloot`,`skinloot`,`resistance1`,`resistance2`,`resistance3`,`resistance4`,`resistance5`,`resistance6`,`spell1`,`spell2`,`spell3`,`spell4`,`PetSpellDataId`,`mingold`,`maxgold`,`AIName`,`MovementType`,`InhabitType`,`unk16`,`unk17`,`RacialLeader`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`movementId`,`RegenHealth`,`equipment_id`,`mechanic_immune_mask`,`flags_extra`,`ScriptName`) values
(90010,0,0,0,0,0,15432,0,15432,0,'Miyagison','Shark-WOW Extreme','',0,85,85,126000,126000,0,0,10000,35,35,1,1,0.2,1,2750,2750,0,250,1,1000,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1,1,0,0,0,0,0,0,0,0,1,0,0,0,'ZenMaster'),
(92006,0,0,0,0,0,25468,0,25468,0,'Zumathed','Death Knight Trainer','',0,80,80,126000,126000,0,0,9729,35,35,51,1,0.6,1,422,586,0,642,4.6,2000,0,1,0,0,0,0,0,6,0,345,509,103,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,10,1,0,0,0,0,0,0,0,0,1,0,0,0,''),
(92011,0,0,0,0,0,18224,0,18224,0,'Minimirran','Druid Trainer','',0,80,80,126000,126000,0,0,5000,35,35,51,1.26,0.2,0,74,102,0,112,1,2000,0,8,512,0,0,0,0,11,0,61,90,25,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.35,1,0,0,0,0,0,0,0,0,1,1462,0,2,''),
(92003,0,0,0,0,0,17580,0,17580,0,'Wolf','Hunter Trainer','',0,80,80,126000,126000,0,0,9500,35,35,51,1.26,1,0,110,142,0,58,1,2000,0,1,512,0,0,0,0,3,0,74.448,102.366,100,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.35,1,0,0,0,0,0,0,0,0,1,117,0,2,''),
(92008,0,0,0,0,0,17870,0,17870,0,'Aethis','Mage Trainer','',0,80,80,126000,126000,9000,9000,9500,35,35,51,1,0.5,0,289,421,0,175,1,2000,0,8,512,0,0,0,0,8,0,271,403,45,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1,1,0,0,0,0,0,0,0,0,1,0,0,0,''),
(92002,0,0,0,0,0,28019,0,28019,0,'Myrio','Paladin Trainer','',0,80,80,126000,126000,0,0,9500,35,35,51,1.1,0.35,0,252,357,0,304,1,2000,0,1,32768,0,0,0,0,2,0,215,320,44,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,603,'',1,3,1,1,0,0,0,0,0,0,0,0,1,1407,0,2,''),
(92005,0,0,0,0,0,5233,0,5233,0,'Gabrielle','Priest Trainer','',0,80,80,126000,126000,9000,9000,9500,35,35,51,1.26,0.6,0,74,102,0,112,1,2000,0,8,4096,0,0,0,0,5,0,61,90,25,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.35,1,0,0,0,0,0,0,0,0,1,0,0,2,''),
(92004,0,0,0,0,0,14632,0,14632,0,'Grace','Rogue Trainer','',0,80,80,126000,126000,0,0,9500,35,35,51,1.26,1,0,104,138,0,252,1,2000,0,1,512,0,0,0,0,4,0,72,106,26,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,3,1.35,1,0,0,0,0,0,0,0,0,1,30,0,2,''),
(92007,0,0,0,0,0,22467,0,22467,0,'Ben','Shaman Trainer','',0,80,80,126000,126000,9000,9000,9500,35,35,51,1.125,1,0,234,331,0,286,1,2000,0,2,4096,0,0,0,0,7,0,198,295,33,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',1,3,1,1,0,0,0,0,0,0,0,0,1,0,0,2,''),
(92009,0,0,0,0,0,21135,0,21135,0,'Errntu','Warlock Trainer','',0,80,80,126000,126000,9000,9000,9500,35,35,51,1.1,0.5,0,74,102,0,112,1,2000,0,8,768,0,0,0,0,9,0,61,90,25,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',0,1,1,1,0,0,0,0,0,0,0,0,1,93,0,2,''),
(92001,0,0,0,0,0,28127,0,28127,0,'Zarnac','Warrior Trainer','',0,80,80,126000,126000,0,0,9500,35,35,51,1.125,1,0,65,99,0,38,1,2000,0,1,512,0,0,0,0,1,0,0,0,100,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'',1,3,1,1,0,0,0,0,0,0,0,0,1,0,0,2,'');

/* Give the trainers some spells by copying all the spells from existing trainers */
/* Death Knight */
REPLACE INTO `npc_trainer`(`entry`,`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel`)
SELECT '92006',`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel` FROM `npc_trainer` WHERE `entry`='28474';
/* Druid */
REPLACE INTO `npc_trainer`(`entry`,`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel`)
SELECT '92011',`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel` FROM `npc_trainer` WHERE `entry`='16721';
/* Hunter */
REPLACE INTO `npc_trainer`(`entry`,`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel`)
SELECT '92003',`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel` FROM `npc_trainer` WHERE `entry`='5515';
/* Mage */
REPLACE INTO `npc_trainer`(`entry`,`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel`)
SELECT '92008',`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel` FROM `npc_trainer` WHERE `entry`='28958';
/* Paladin */
REPLACE INTO `npc_trainer`(`entry`,`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel`)
SELECT '92002',`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel` FROM `npc_trainer` WHERE `entry`='23128';
/* Priest */
REPLACE INTO `npc_trainer`(`entry`,`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel`)
SELECT '92005',`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel` FROM `npc_trainer` WHERE `entry`='11401';
/* Rogue */
REPLACE INTO `npc_trainer`(`entry`,`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel`)
SELECT '92004',`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel` FROM `npc_trainer` WHERE `entry`='4214';
/* Shaman */
REPLACE INTO `npc_trainer`(`entry`,`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel`)
SELECT '92007',`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel` FROM `npc_trainer` WHERE `entry`='17204';
/* Warlock */
REPLACE INTO `npc_trainer`(`entry`,`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel`)
SELECT '92009',`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel` FROM `npc_trainer` WHERE `entry`='23534';
/* Warrior */
REPLACE INTO `npc_trainer`(`entry`,`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel`)
SELECT '92001',`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel` FROM `npc_trainer` WHERE `entry`='17120';