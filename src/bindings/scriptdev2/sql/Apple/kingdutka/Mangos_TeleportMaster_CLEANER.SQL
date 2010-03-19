/* ------  Creature Templates  ------ */
delete from `creature_template` WHERE `entry` IN (100005, 100010, 100015, 100020);

/* ------  Creatures  ------ */
delete from `creature` WHERE `id` IN (100015, 100020, 100025);

/* ------  Objects  ------ */
delete from `gameobject` WHERE `guid` IN (900010,900011,900012,900013,900014,900015,900016,900017,900018,900019,900050,900051,900052,900053,900054,900055,900060,900061,900062,900063,900070,900071,100101,100098,100102,100099);

/* ------  Teleport Locations  ------ */
delete from `game_tele` WHERE `id` IN (1411,1410);

/* ------ Remove quests from OLD version ------ */
DELETE FROM `quest_template` WHERE `entry` IN (500021,500022,500023,500024,500031,500032,500033,500034,500035,500036,500037);