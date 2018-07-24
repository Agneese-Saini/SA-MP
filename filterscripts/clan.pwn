// ---------------------------------------
// clan.pwn - v1.0 - Updated: 25 May, 2018 - MySQL based advanced clan system - By Gammix
// ---------------------------------------
#define FILTERSCRIPT

#include <a_samp>
#include <a_mysql>
#include <sscanf2>
#include <streamer>
#include <PreviewModelDialog>
#include <easyDialog>
#include <zcmd>
#include <foreach>

// ---------------------------------------
#define VERSION 		"v1.0"
#define DATE    		"25 May, 2018"
// ---------------------------------------

// ---------------------------------------
// MySQL SETTINGS
#define MYSQL_HOST		"localhost"
#define MYSQL_USER		"root"
#define MYSQL_PASS		""
#define MYSQL_DATABASE	"sa-mp"
// ---------------------------------------

// ---------------------------------------
// SOME CLAN SYSTEM SETTINGS
#define MAX_CLANS       			100 // maimum clans your server can have

#define MAX_CLAN_TAG_NAME        	7 // maxlength of a clan tag; exclude brackets '[' & ']' - also should be less than 24
#define MAX_CLAN_NAME 				64 // maxlength of a clan name
#define MAX_CLAN_MEMBERS			100 // maximum members a clan can ever have (this isn't any limit, its just used for setting string sizes in script)

#define MAX_CLAN_WEAPONS			3 // maximum weapons a clan vault can have (players are spawned with cault weapons)
#define CLAN_WEAPON_EXPIRE_INTERVAL 7 // maximum days a clan weapon bought can stay in vault inventory (so if a clan bought Sniper, they will have it for every clan member for "n" days)

#define MAX_CLAN_RANKS      		10 // maximum ranks a clan can have, if you change this make sure you modify "DEFAULT_CLAN_RANKS" array
#define MAX_CLAN_RANK_NAME			16 // maxlength of a clan rank name

#define CLAN_UPDATE_INTERVAL    	1 // after how many HOURS all clans' data is saved in MySQL database
#define CLAN_INFO_EXPIRE_INTERVAL   5 // for how many SECONDS notification textdraw is displayed (display when clan member gets exp points)

#define MAX_TEAM_NAME        		32 // maxlength of a team's name
// ---------------------------------------

// ---------------------------------------
// HEXA COLOR CODES
#define COLOR_WHITE			0xFFFFFFFF
#define COLOR_TOMATO		0xFF6347FF
#define COLOR_BIEGE			0xFFF8E5FF
#define COLOR_YELLOW		0xFFDD00FF
#define COLOR_GREEN			0x42F486FF
#define COLOR_DEFAULT		0xE5F8FFFF
#define COLOR_BLUE      	0x4286F4FF
#define COLOR_LIGHT_AQUA	0xBAFCFFFF

// EMBEDDED COLOR CODES
#define COL_WHITE 			"{FFFFFF}"
#define COL_TOMATO 			"{FF6347}"
#define COL_BIEGE			"{FFF8E5}"
#define COL_YELLOW			"{FFDD00}"
#define COL_GREEN			"{42F486}"
#define COL_DEFAULT			"{E5F8FF}"
#define COL_BLUE        	"{4286F4}"
#define COL_LIGHT_AQUA		"{BAFCFF}"
// ---------------------------------------

// ---------------------------------------
#define alpha(%1,%2) \
	((%1 & ~0xFF) | (clamp(%2, 0x00, 0xFF)))

#define foreach_clans(%0) \
	for (new _%0 = 0, %0 = sortedClansList[_%0][1]; _%0 < totalSortedClans; _%0++, %0 = sortedClansList[_%0][1])
// ---------------------------------------

// ---------------------------------------
// CLAN RANK DEFINITIONS
#define CLAN_RANK_OWNER		(MAX_CLAN_RANKS - 1)
#define CLAN_RANK_LEADER	(MAX_CLAN_RANKS - 2)
// ---------------------------------------

// ---------------------------------------
// ENUMERATORS
enum E_TEAM_DATA {
	TEAM_NAME[MAX_TEAM_NAME],
	TEAM_SKIN,
	TEAM_COLOR
};

enum E_CLAN_WEAPON_DATA {
	CLAN_WEAPON_MODEL,
	CLAN_WEAPON_ID,
	CLAN_WEAPON_AMMO,
	CLAN_WEAPON_NAME[32],
	CLAN_WEAPON_COST
};

enum E_CLAN_DATA {
	CLAN_SQLID,
	CLAN_TAG[MAX_CLAN_TAG_NAME],
	CLAN_NAME[MAX_CLAN_NAME],
	CLAN_SKIN,
	CLAN_TEAM,
	CLAN_OWNER[MAX_PLAYER_NAME],
	Float:CLAN_SPAWN_POS[4],
	CLAN_SPAWN_INTERIORID,
	CLAN_SPAWN_WORLDID,
	CLAN_VAULT_MONEY,
	CLAN_VAULT_WEAPONS[MAX_CLAN_WEAPONS],
	CLAN_VAULT_WEAPONS_TIMESTAMP[MAX_CLAN_WEAPONS],
	CLAN_TOTAL_EXP,
	CLAN_WAR_WINS,
	CLAN_WAR_TOTAL,
	Text3D:CLAN_3D_TEXT_LABEL,
	CLAN_PICKUPID
};
// ---------------------------------------

// ---------------------------------------
// CONSTANT ARRAYS

// DEFAULT CLAN RANK NAMES
new const DEFAULT_CLAN_RANKS[MAX_CLAN_RANKS][MAX_CLAN_RANK_NAME] = {
	"Recruit",
	"Trainer",
	"Commander",
	"Lead Commander",
	"Officer",
	"General",
	"Major General",
	"Manager",
	"Leader",
	"Owner"
};

// ENTER YOUR TEAM DATA IN THIS ARRAY
new const TEAMS[][E_TEAM_DATA] = {
	{"Soviet Union", 111, 0x00FF00FF},
	{"Australia", 127, 0xFFD700FF},
	{"Taliban", 175, 0x033999FF},
	{"Europe", 285, 0x00CED1FF},
	{"USA", 287, 0x8B008BFF}
};

// WEAPONS CLAN LEADER/OWNER CAN BUY FOR THEIR CLAN INVENTORY/VAULT
new const CLAN_WEAPONS[][E_CLAN_WEAPON_DATA] = {
    {351, WEAPON_SHOTGSPA, 300, "Spas12", 5000000},
    {350, WEAPON_SAWEDOFF, 200, "Sawnoff", 5000000},
    {348, WEAPON_DEAGLE, 200, "Deagle", 4000000},
    {352, WEAPON_UZI, 300, "UZI", 5000000},
    {356, WEAPON_M4, 200, "M4", 3000000},
    {355, WEAPON_AK47, 200, "AK-47", 3000000},
    {358, WEAPON_SNIPER, 150, "Sniper Rifle", 4500000},
    {359, WEAPON_ROCKETLAUNCHER, 2, "RPG", 5000000},
    {342, WEAPON_GRENADE, 5, "Grenade", 3000000},
    {363, WEAPON_SATCHEL, 5, "Satchel Charge", 3500000}
};
// ---------------------------------------

// ---------------------------------------
// VARIABLES
// MYSQL
new MySQL:database;

// CLAN DATA
new clanRankNames[MAX_CLANS][MAX_CLAN_RANKS][MAX_CLAN_RANK_NAME];
new clanData[MAX_CLANS][E_CLAN_DATA];
new totalSortedClans;
new sortedClansList[MAX_CLANS][2];

// PLAYER CLAN DATA
new playerClanID[MAX_PLAYERS];
new playerClanRank[MAX_PLAYERS];
new bool:playerClanWeaponsEnabled[MAX_PLAYERS];
new playerClanLastWithdrawTimestamp[MAX_PLAYERS];
new clanInviteFrom[MAX_PLAYERS];
new clanInviteTimer[MAX_PLAYERS];
new clanInviteCountDown[MAX_PLAYERS];
new Text3D:playerClan3DTextLabel[MAX_PLAYERS];

// TEXTDRAWS
new Text:clanHelpTD[13];
new Text:clanInfoTD[7];
new Text:clanAnnouncementTD[3];
new Text:clanInviteTD;
new PlayerText:clanInfoPTD[MAX_PLAYERS][4];
new PlayerText:clanNamePTD[MAX_PLAYERS];
new PlayerText:clanInvitePTD[MAX_PLAYERS][3];

// TIMERS
new databaseTimer;
new clanAnnouncementTimer;

// ---------------------------------------

// ---------------------------------------
// FUNCTIONS USED
QuickSortPair(array[][2], bool:desc, left, right) {
	new tempLeft = left,
		tempRight = right,
		pivot = array[(left + right) / 2][0],
		tempVar;

	while (tempLeft <= tempRight) {
	    if (desc) {
			while (array[tempLeft][0] > pivot) tempLeft++;
			while (array[tempRight][0] < pivot) tempRight--;
		}
	    else {
			while (array[tempLeft][0] < pivot) tempLeft++;
			while (array[tempRight][0] > pivot) tempRight--;
		}

		if (tempLeft <= tempRight) {
			tempVar = array[tempLeft][0];
		 	array[tempLeft][0] = array[tempRight][0];
		 	array[tempRight][0] = tempVar;

			tempVar = array[tempLeft][1];
			array[tempLeft][1] = array[tempRight][1];
			array[tempRight][1] = tempVar;

			tempLeft++;
			tempRight--;
		}
	}

	if (left < tempRight)
		QuickSortPair(array, desc, left, tempRight);

	if (tempLeft < right)
		QuickSortPair(array, desc, tempLeft, right);
}

FormatNumber(number) {
	new numOfPeriods = 0, tmp = number;
	new str[32];
	while(tmp > 1000) {
		tmp = floatround(tmp / 1000, floatround_floor), ++numOfPeriods;
	}
	valstr(str, number);
	new slen = strlen(str);
	for(new i = 1; i != numOfPeriods + 1; ++i) {
		strins(str, ",", slen - 3*i);
	}
	return str;
}

ReturnTimelapse(start, till) {
    new ret[32];
	new seconds = (till - start);

	const
		MINUTE = (60),
		HOUR = (60 * MINUTE),
		DAY = (24 * HOUR),
		MONTH = (30 * DAY);

	if (seconds == 1) {
		format(ret, sizeof(ret), "a second");
	} else if (seconds < (1 * MINUTE)) {
		format(ret, sizeof(ret), "%i seconds", seconds);
	} else if (seconds < (2 * MINUTE)) {
		format(ret, sizeof(ret), "a minute");
	} else if (seconds < (45 * MINUTE)) {
		format(ret, sizeof(ret), "%i minutes", (seconds / MINUTE));
	} else if (seconds < (90 * MINUTE)) {
		format(ret, sizeof(ret), "an hour");
	} else if (seconds < (24 * HOUR)) {
		format(ret, sizeof(ret), "%i hours", (seconds / HOUR));
	} else if (seconds < (48 * HOUR)) {
		format(ret, sizeof(ret), "a day");
	} else if (seconds < (30 * DAY)) {
		format(ret, sizeof(ret), "%i days", (seconds / DAY));
	} else if (seconds < (12 * MONTH)) {
		new months = floatround(seconds / DAY / 30);
      	if (months <= 1) {
			format(ret, sizeof(ret), "a month");
      	} else {
			format(ret, sizeof(ret), "%i months", months);
		}
	} else {
      	new years = floatround(seconds / DAY / 365);
      	if (years <= 1) {
			format(ret, sizeof(ret), "a year");
      	} else {
			format(ret, sizeof(ret), "%i years", years);
		}
	}

	return ret;
}
// ---------------------------------------

// ---------------------------------------
// DATABASE CONNECTION FUNCTIONS AND CALLBACKS TO LOAD DATA FOR CLANS
ConnectDatabase() {
    mysql_log(ALL);

    new MySQLOpt:option = mysql_init_options();
	mysql_set_option(option, AUTO_RECONNECT, true);
    mysql_set_option(option, MULTI_STATEMENTS, false);
    mysql_set_option(option, POOL_SIZE, 2);
    mysql_set_option(option, SERVER_PORT, 3306);

	database = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DATABASE, option);
	if (mysql_errno(database) != 0) {
		return 0;
	}

	new query[1024] =
		"CREATE TABLE IF NOT EXISTS clans (\
		id INT(11) NOT NULL AUTO_INCREMENT, \
		tag VARCHAR("#MAX_CLAN_TAG_NAME") DEFAULT NULL, \
		name VARCHAR("#MAX_CLAN_NAME") DEFAULT NULL, \
		skin INT DEFAULT NULL, \
		exp INT DEFAULT NULL, \
		team INT DEFAULT NULL, \
		clanwar_wins INT DEFAULT NULL, \
		clanwar_total INT DEFAULT NULL, "
	;
	strcat(query,
  		"spawn_x FLOAT DEFAULT NULL, \
		spawn_y FLOAT DEFAULT NULL, \
		spawn_z FLOAT DEFAULT NULL, \
		spawn_angle FLOAT DEFAULT NULL, \
		spawn_interiorid INT DEFAULT NULL, \
		spawn_worldid INT DEFAULT NULL, \
		vault_money INT DEFAULT NULL, \
		vault_weapons VARCHAR(32) DEFAULT NULL, \
		vault_weapons_timestamp VARCHAR(32) DEFAULT NULL, \
		PRIMARY KEY (id))"
	);
	mysql_tquery(database, query);

	mysql_tquery(database,
		"CREATE TABLE IF NOT EXISTS clan_members (\
			clan_name VARCHAR("#MAX_CLAN_NAME") DEFAULT NULL, \
			name VARCHAR(24) DEFAULT NULL, \
			rank INT DEFAULT NULL, \
			toggle_weapons INT NOT NULL, \
			last_withdraw_timestamp INT NOT NULL\
		)"
	);

	mysql_tquery(database,
		"CREATE TABLE IF NOT EXISTS clan_ranks (\
			clan_name VARCHAR("#MAX_CLAN_NAME") DEFAULT NULL, \
			name VARCHAR(24) DEFAULT NULL, \
			level INT DEFAULT NULL\
		)"
	);

	mysql_tquery(database, "SELECT * FROM clans LIMIT "#MAX_CLANS"", "OnClanDataLoad");

    databaseTimer = SetTimer("UpdateClansData", (CLAN_UPDATE_INTERVAL * 60 * 60 * 1000), true);
    return 1;
}

forward OnClanDataLoad();
public OnClanDataLoad() {
	new query[128];
	new text_label[32];
	new vault_weapons[32];
	new vault_weapons_timestamp[32];
	new weapons[MAX_CLAN_WEAPONS];
	new timestamps[MAX_CLAN_WEAPONS];
	for (new i = 0, j = cache_num_rows(); i < j; i++) {
		cache_get_value_int(i, "id", clanData[i][CLAN_SQLID]);
		cache_get_value(i, "tag", clanData[i][CLAN_TAG], MAX_CLAN_TAG_NAME);
		cache_get_value(i, "name", clanData[i][CLAN_NAME], MAX_CLAN_NAME);
		cache_get_value_int(i, "skin", clanData[i][CLAN_SKIN]);
		cache_get_value_int(i, "exp", clanData[i][CLAN_TOTAL_EXP]);
		cache_get_value_int(i, "team", clanData[i][CLAN_TEAM]);
		cache_get_value_int(i, "clanwar_wins", clanData[i][CLAN_WAR_WINS]);
		cache_get_value_int(i, "clanwar_total", clanData[i][CLAN_WAR_TOTAL]);
		cache_get_value_float(i, "spawn_x", clanData[i][CLAN_SPAWN_POS][0]);
		cache_get_value_float(i, "spawn_y", clanData[i][CLAN_SPAWN_POS][1]);
		cache_get_value_float(i, "spawn_z", clanData[i][CLAN_SPAWN_POS][2]);
		cache_get_value_float(i, "spawn_angle", clanData[i][CLAN_SPAWN_POS][3]);
		cache_get_value_int(i, "spawn_interiorid", clanData[i][CLAN_SPAWN_INTERIORID]);
		cache_get_value_int(i, "spawn_worldid", clanData[i][CLAN_SPAWN_WORLDID]);
		cache_get_value_int(i, "vault_money", clanData[i][CLAN_VAULT_MONEY]);
		cache_get_value(i, "vault_weapons", vault_weapons, sizeof(vault_weapons));
		cache_get_value(i, "vault_weapons_timestamp", vault_weapons_timestamp, sizeof(vault_weapons_timestamp));

		sscanf(vault_weapons, "a<i>["#MAX_CLAN_WEAPONS"]", weapons);
		sscanf(vault_weapons_timestamp, "a<i>["#MAX_CLAN_WEAPONS"]", timestamps);
		for (new x = 0; x < MAX_CLAN_WEAPONS; x++) {
            if ((gettime() - timestamps[x]) >= (CLAN_WEAPON_EXPIRE_INTERVAL * 24 * 60 * 60)) { // if weapon expired
	            weapons[x] = -1;
	            timestamps[x] = -1;
	        }

            clanData[i][CLAN_VAULT_WEAPONS][x] = weapons[x];
            clanData[i][CLAN_VAULT_WEAPONS_TIMESTAMP][x] = timestamps[x];
		}

		if (clanData[i][CLAN_SPAWN_POS][0] != 0.0 && clanData[i][CLAN_SPAWN_POS][1] != 0.0 && clanData[i][CLAN_SPAWN_POS][2] != 0.0) {
			format(text_label, sizeof(text_label), "[%s]\n%s\n"COL_DEFAULT"((SPAWN POINT))", clanData[i][CLAN_TAG], clanData[i][CLAN_NAME]);
			clanData[i][CLAN_3D_TEXT_LABEL] = CreateDynamic3DTextLabel(text_label, alpha(TEAMS[clanData[i][CLAN_TEAM]][TEAM_COLOR], 100), clanData[i][CLAN_SPAWN_POS][0], clanData[i][CLAN_SPAWN_POS][1], clanData[i][CLAN_SPAWN_POS][2], 50.0, _, _, 1, clanData[i][CLAN_SPAWN_WORLDID], clanData[i][CLAN_SPAWN_INTERIORID]);
			clanData[i][CLAN_PICKUPID] = CreateDynamicPickup(19306, 1, clanData[i][CLAN_SPAWN_POS][0], clanData[i][CLAN_SPAWN_POS][1], clanData[i][CLAN_SPAWN_POS][2], clanData[i][CLAN_SPAWN_WORLDID], clanData[i][CLAN_SPAWN_INTERIORID]);
		}
		else {
            clanData[i][CLAN_3D_TEXT_LABEL] = Text3D:INVALID_STREAMER_ID;
            clanData[i][CLAN_PICKUPID] = INVALID_STREAMER_ID;
		}

		mysql_format(database, query, sizeof(query),
			"SELECT * FROM clan_ranks WHERE clan_name = '%e' LIMIT "#MAX_CLAN_RANKS"",
			clanData[i][CLAN_NAME]
		);
        mysql_tquery(database, query, "OnClanRankDataLoad", "i", i);

        mysql_format(database, query, sizeof(query),
			"SELECT name FROM clan_members WHERE clan_name = '%e' AND rank = %i LIMIT 1",
			clanData[i][CLAN_NAME], CLAN_RANK_OWNER
		);
        mysql_tquery(database, query, "OnClanOwnerNameLoad", "i", i);

		printf("[Clan.pwn] Loaded clan [%s] %s", clanData[i][CLAN_TAG], clanData[i][CLAN_NAME]);
	}

	UpdateClansRank();
}

// LOAD RANK NAMES FOR A CLANID
forward OnClanRankDataLoad(clanid);
public OnClanRankDataLoad(clanid) {
	new level;

	for (new i = 0, j = cache_num_rows(); i < j; i++) {
		cache_get_value_int(i, "level", level);
		cache_get_value(i, "name", clanRankNames[clanid][level], MAX_CLAN_RANK_NAME);
	}
}

// LOAD THE OWNER NAME FOR A CLANID
forward OnClanOwnerNameLoad(clanid);
public OnClanOwnerNameLoad(clanid) {
	cache_get_value(0, "name", clanData[clanid][CLAN_OWNER], MAX_PLAYER_NAME);
}
// ---------------------------------------

// ---------------------------------------
// CLAN RANK UPDATES AFTER 2 HOURS OR WHATEVER VALUE YOU HAVE SET TO "CLAN_UPDATE_INTERVAL"
UpdateClansRank() {
	totalSortedClans = 0;
	for (new i = 0; i < MAX_CLANS; i++) {
		if (clanData[i][CLAN_NAME][0] != EOS) {
			++totalSortedClans;

			sortedClansList[i][0] = clanData[i][CLAN_TOTAL_EXP];
			sortedClansList[i][1] = i;
		}
	}

	QuickSortPair(sortedClansList, true, 0, totalSortedClans);
}

// UPDATING PLAYER CLAN STATS
UpdatePlayerClanData(playerid) {
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new query[128];
	mysql_format(database, query, sizeof(query),
	    "UPDATE clan_members SET rank = %i, toggle_weapons = %i, last_withdraw_timestamp = %i WHERE name = '%e'",
	    playerClanRank[playerid], playerClanWeaponsEnabled[playerid], playerClanLastWithdrawTimestamp[playerid], name
	);

	mysql_tquery(database, query);
}

UpdateClanData(clanid) {
	new query[1024];
	new vault_weapons[32];
	new vault_weapons_timestamp[32];

	for (new x = 0; x < MAX_CLAN_WEAPONS; x++) {
		if (clanData[clanid][CLAN_VAULT_WEAPONS][x] != -1) {
	        if ((gettime() - clanData[clanid][CLAN_VAULT_WEAPONS_TIMESTAMP][x]) >= (CLAN_WEAPON_EXPIRE_INTERVAL * 24 * 60 * 60)) { // if weapon is expired
	            clanData[clanid][CLAN_VAULT_WEAPONS][x] = -1;
	            clanData[clanid][CLAN_VAULT_WEAPONS_TIMESTAMP][x] = -1;
	        }
		}

		format(vault_weapons, sizeof(vault_weapons), "%s%i", vault_weapons, clanData[clanid][CLAN_VAULT_WEAPONS][x]);
		format(vault_weapons_timestamp, sizeof(vault_weapons_timestamp), "%s%i", vault_weapons_timestamp, clanData[clanid][CLAN_VAULT_WEAPONS_TIMESTAMP][x]);

		if (x != (MAX_CLAN_WEAPONS - 1)) {
			strcat(vault_weapons, " ");
			strcat(vault_weapons_timestamp, " ");
		}
	}

	mysql_format(database, query, sizeof(query),
	    "UPDATE clans SET \
			tag = '%e', \
			name = '%e', \
			skin = %i, \
			exp = %i, \
			team = %i, \
			clanwar_wins = %i, \
			clanwar_total = %i, \
			spawn_x = %f, spawn_y = %f, spawn_z = %f, spawn_angle = %f, \
			spawn_interiorid = %i, spawn_worldid = %i, \
			vault_money = %i, \
			vault_weapons = '%s', vault_weapons_timestamp = '%s' \
		WHERE id = %i",
	    clanData[clanid][CLAN_TAG],
	    clanData[clanid][CLAN_NAME],
	    clanData[clanid][CLAN_SKIN],
	    clanData[clanid][CLAN_TOTAL_EXP],
	    clanData[clanid][CLAN_TEAM],
	    clanData[clanid][CLAN_WAR_WINS],
	    clanData[clanid][CLAN_WAR_TOTAL],
	    clanData[clanid][CLAN_SPAWN_POS][0], clanData[clanid][CLAN_SPAWN_POS][1], clanData[clanid][CLAN_SPAWN_POS][2], clanData[clanid][CLAN_SPAWN_POS][3],
	    clanData[clanid][CLAN_SPAWN_INTERIORID], clanData[clanid][CLAN_SPAWN_WORLDID],
	    clanData[clanid][CLAN_VAULT_MONEY],
	    vault_weapons, vault_weapons_timestamp,
	    clanData[clanid][CLAN_SQLID]
	);
	mysql_tquery(database, query);

	for (new x = 0; x < MAX_CLAN_RANKS; x++) {
		mysql_format(database, query, sizeof(query),
		    "UPDATE clan_ranks SET name = '%e' WHERE level = %i AND clan_name = '%e'",
		    clanRankNames[clanid][x], x, clanData[clanid][CLAN_NAME]
		);
		mysql_tquery(database, query);
	}
}

// THIS IS THE CLAN UPDATE/SAVE TIMER
forward UpdateClansData();
public UpdateClansData() {
    UpdateClansRank();

	foreach(new i : Player) {
	    if (playerClanID[i] != -1) {
            UpdatePlayerClanData(i);
	    }
	}

	for (new i = 0; i < totalSortedClans; i++) {
		if (clanData[i][CLAN_NAME][0] != EOS) {
            UpdateClanData(i);
		}
	}
}
// ---------------------------------------

// ---------------------------------------
// DISCONNECT DATABASE
DisconnectDatabase() {
	KillTimer(databaseTimer);
    UpdateClansData();

    foreach_clans(i) {
		DestroyDynamic3DTextLabel(clanData[i][CLAN_3D_TEXT_LABEL]);
		DestroyDynamicPickup(clanData[i][CLAN_PICKUPID]);
	}

	mysql_close(database);
}
// ---------------------------------------

// ---------------------------------------
// CLAN RELATED FUNCTIONS
SendClanReward(playerid, const type[], reward_money, reward_score) {
    new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new reward_string[164];
	format(reward_string, sizeof(reward_string), "[CLAN-REWARD] You have received +$%s and +%i Score from %s's %s.", FormatNumber(reward_money), reward_score, name, type);

	foreach (new i : Player) {
		if (playerClanID[i] == playerClanID[playerid]) {
		    GivePlayerMoney(i, reward_money);
		    SetPlayerScore(i, GetPlayerScore(i) + reward_score);
		    if (i != playerid) {
		    	SendClientMessage(i, COLOR_BIEGE, reward_string);
			}
		}
	}
}

CountClanMembers(clanid, &total_members, &online_members) {
	new query[128];
    mysql_format(database, query, sizeof(query),
		"SELECT COUNT(*) FROM clan_members WHERE clan_name = '%e'",
		clanData[clanid][CLAN_NAME]
	);

 	new Cache:cache = mysql_query(database, query);
	total_members = cache_num_rows();
	cache_delete(cache);

	online_members = 0;
	foreach (new i : Player) {
		if (playerClanID[i] == clanid) {
			++online_members;
		}
	}
}

GetClanRank(clanid) {
	for (new i = 0; i < totalSortedClans; i++) {
		if (sortedClansList[i][1] == clanid) {
			return (i + 1);
		}
	}

	return -1;
}

GetClanDialogHeader(clanid, const add_text[]) {
	new ret[128];
	format(ret, sizeof(ret), COL_GREEN "[%s] Clan"COL_WHITE": %s", clanData[clanid][CLAN_TAG], add_text);
	return ret;
}
// ---------------------------------------

// ---------------------------------------
// CALLBACKS
public OnFilterScriptInit() {
	// CLAN HELP BOX GLOBAL TEXTDRAWS
	clanHelpTD[0] = TextDrawCreate(10.0000, 160.0000, "BOX");
	TextDrawFont(clanHelpTD[0], 1);
	TextDrawLetterSize(clanHelpTD[0], 0.0000, 21.7999);
	TextDrawColor(clanHelpTD[0], -1);
	TextDrawSetShadow(clanHelpTD[0], 0);
	TextDrawSetOutline(clanHelpTD[0], 0);
	TextDrawBackgroundColor(clanHelpTD[0], 255);
	TextDrawSetProportional(clanHelpTD[0], 1);
	TextDrawUseBox(clanHelpTD[0], 1);
	TextDrawBoxColor(clanHelpTD[0], 421075400);
	TextDrawTextSize(clanHelpTD[0], 190.0000, 50.0000);

	clanHelpTD[1] = TextDrawCreate(98.0000, 167.5000, "/CLANHELP");
	TextDrawFont(clanHelpTD[1], 2);
	TextDrawLetterSize(clanHelpTD[1], 0.2999, 2.0999);
	TextDrawAlignment(clanHelpTD[1], 2);
	TextDrawColor(clanHelpTD[1], -7601921);
	TextDrawSetShadow(clanHelpTD[1], 0);
	TextDrawSetOutline(clanHelpTD[1], 0);
	TextDrawBackgroundColor(clanHelpTD[1], 255);
	TextDrawSetProportional(clanHelpTD[1], 1);
	TextDrawTextSize(clanHelpTD[1], 640.0000, 0.0000);

	clanHelpTD[2] = TextDrawCreate(18.0000, 194.5000, "General:");
	TextDrawFont(clanHelpTD[2], 2);
	TextDrawLetterSize(clanHelpTD[2], 0.1899, 1.2000);
	TextDrawColor(clanHelpTD[2], -8433409);
	TextDrawSetShadow(clanHelpTD[2], 0);
	TextDrawSetOutline(clanHelpTD[2], 0);
	TextDrawBackgroundColor(clanHelpTD[2], 255);
	TextDrawSetProportional(clanHelpTD[2], 1);
	TextDrawTextSize(clanHelpTD[2], 640.0000, 640.0000);

	clanHelpTD[3] = TextDrawCreate(18.0000, 204.5000, "/clans, /onlineclans, /cmembers, /cinfo, /cjoin");
	TextDrawFont(clanHelpTD[3], 2);
	TextDrawLetterSize(clanHelpTD[3], 0.1499, 1.0000);
	TextDrawColor(clanHelpTD[3], -1);
	TextDrawSetShadow(clanHelpTD[3], 0);
	TextDrawSetOutline(clanHelpTD[3], 0);
	TextDrawBackgroundColor(clanHelpTD[3], 255);
	TextDrawSetProportional(clanHelpTD[3], 1);
	TextDrawTextSize(clanHelpTD[3], 182.0000, 640.0000);

	clanHelpTD[4] = TextDrawCreate(18.0000, 224.5000, "Clan Members:");
	TextDrawFont(clanHelpTD[4], 2);
	TextDrawLetterSize(clanHelpTD[4], 0.1899, 1.2000);
	TextDrawColor(clanHelpTD[4], -8433409);
	TextDrawSetShadow(clanHelpTD[4], 0);
	TextDrawSetOutline(clanHelpTD[4], 0);
	TextDrawBackgroundColor(clanHelpTD[4], 255);
	TextDrawSetProportional(clanHelpTD[4], 1);
	TextDrawTextSize(clanHelpTD[4], 640.0000, 640.0000);

	clanHelpTD[5] = TextDrawCreate(18.0000, 234.5000, "/cinvite, /cquit, /cwithdraw, /cdeposit, /cwithdraw, /cranks");
	TextDrawFont(clanHelpTD[5], 2);
	TextDrawLetterSize(clanHelpTD[5], 0.1499, 1.0000);
	TextDrawColor(clanHelpTD[5], -1);
	TextDrawSetShadow(clanHelpTD[5], 0);
	TextDrawSetOutline(clanHelpTD[5], 0);
	TextDrawBackgroundColor(clanHelpTD[5], 255);
	TextDrawSetProportional(clanHelpTD[5], 1);
	TextDrawTextSize(clanHelpTD[5], 182.0000, 640.0000);

	clanHelpTD[6] = TextDrawCreate(18.0000, 254.5000, "Clan Leaders:");
	TextDrawFont(clanHelpTD[6], 2);
	TextDrawLetterSize(clanHelpTD[6], 0.1899, 1.2000);
	TextDrawColor(clanHelpTD[6], -8433409);
	TextDrawSetShadow(clanHelpTD[6], 0);
	TextDrawSetOutline(clanHelpTD[6], 0);
	TextDrawBackgroundColor(clanHelpTD[6], 255);
	TextDrawSetProportional(clanHelpTD[6], 1);
	TextDrawTextSize(clanHelpTD[6], 640.0000, 640.0000);

	clanHelpTD[7] = TextDrawCreate(18.0000, 264.5000, "/cbuyskin, /cbuyweapon, /csetteam, /csetskin, /csetspawn, /cremovespawn, /cpromote, /ckick, /cranks");
	TextDrawFont(clanHelpTD[7], 2);
	TextDrawLetterSize(clanHelpTD[7], 0.1499, 1.0000);
	TextDrawColor(clanHelpTD[7], -1);
	TextDrawSetShadow(clanHelpTD[7], 0);
	TextDrawSetOutline(clanHelpTD[7], 0);
	TextDrawBackgroundColor(clanHelpTD[7], 255);
	TextDrawSetProportional(clanHelpTD[7], 1);
	TextDrawTextSize(clanHelpTD[7], 182.0000, 640.0000);

	clanHelpTD[8] = TextDrawCreate(18.0000, 294.5000, "Clan Owners:");
	TextDrawFont(clanHelpTD[8], 2);
	TextDrawLetterSize(clanHelpTD[8], 0.1899, 1.2000);
	TextDrawColor(clanHelpTD[8], -8433409);
	TextDrawSetShadow(clanHelpTD[8], 0);
	TextDrawSetOutline(clanHelpTD[8], 0);
	TextDrawBackgroundColor(clanHelpTD[8], 255);
	TextDrawSetProportional(clanHelpTD[8], 1);
	TextDrawTextSize(clanHelpTD[8], 640.0000, 640.0000);

	clanHelpTD[9] = TextDrawCreate(18.0000, 304.5000, "/cchangename, /cchangetag, /csavestats");
	TextDrawFont(clanHelpTD[9], 2);
	TextDrawLetterSize(clanHelpTD[9], 0.1499, 1.0000);
	TextDrawColor(clanHelpTD[9], -1);
	TextDrawSetShadow(clanHelpTD[9], 0);
	TextDrawSetOutline(clanHelpTD[9], 0);
	TextDrawBackgroundColor(clanHelpTD[9], 255);
	TextDrawSetProportional(clanHelpTD[9], 1);
	TextDrawTextSize(clanHelpTD[9], 182.0000, 640.0000);

	clanHelpTD[10] = TextDrawCreate(18.0000, 314.5000, "Admin Level 5:");
	TextDrawFont(clanHelpTD[10], 2);
	TextDrawLetterSize(clanHelpTD[10], 0.1899, 1.2000);
	TextDrawColor(clanHelpTD[10], -8433409);
	TextDrawSetShadow(clanHelpTD[10], 0);
	TextDrawSetOutline(clanHelpTD[10], 0);
	TextDrawBackgroundColor(clanHelpTD[10], 255);
	TextDrawSetProportional(clanHelpTD[10], 1);
	TextDrawTextSize(clanHelpTD[10], 640.0000, 640.0000);

	clanHelpTD[11] = TextDrawCreate(18.0000, 324.5000, "/ccreate, /cdelete, /cgivemoney, /cgiveexp");
	TextDrawFont(clanHelpTD[11], 2);
	TextDrawLetterSize(clanHelpTD[11], 0.1499, 1.0000);
	TextDrawColor(clanHelpTD[11], -1);
	TextDrawSetShadow(clanHelpTD[11], 0);
	TextDrawSetOutline(clanHelpTD[11], 0);
	TextDrawBackgroundColor(clanHelpTD[11], 255);
	TextDrawSetProportional(clanHelpTD[11], 1);
	TextDrawTextSize(clanHelpTD[11], 182.0000, 640.0000);

	clanHelpTD[12] = TextDrawCreate(188.5000, 348.0000, "Type /close to close box");
	TextDrawFont(clanHelpTD[12], 2);
	TextDrawLetterSize(clanHelpTD[12], 0.1099, 0.8000);
	TextDrawAlignment(clanHelpTD[12], 3);
	TextDrawColor(clanHelpTD[12], -1448498689);
	TextDrawSetShadow(clanHelpTD[12], 0);
	TextDrawSetOutline(clanHelpTD[12], 0);
	TextDrawBackgroundColor(clanHelpTD[12], 255);
	TextDrawSetProportional(clanHelpTD[12], 1);
	TextDrawTextSize(clanHelpTD[12], 640.0000, 640.0000);

	// CLAN INFO BOX GLOBAL TEXTDRAWS
	clanInfoTD[0] = TextDrawCreate(10.0000, 160.0000, "BOX");
	TextDrawFont(clanInfoTD[0], 1);
	TextDrawLetterSize(clanInfoTD[0], 0.0000, 18.7999);
	TextDrawColor(clanInfoTD[0], -1);
	TextDrawSetShadow(clanInfoTD[0], 0);
	TextDrawSetOutline(clanInfoTD[0], 0);
	TextDrawBackgroundColor(clanInfoTD[0], 255);
	TextDrawSetProportional(clanInfoTD[0], 1);
	TextDrawUseBox(clanInfoTD[0], 1);
	TextDrawBoxColor(clanInfoTD[0], 421075400);
	TextDrawTextSize(clanInfoTD[0], 190.0000, 50.0000);

	clanInfoTD[1] = TextDrawCreate(18.0000, 204.5000, "Name:~n~Owner:~n~Clan Rank:~n~Number Of Members:~n~Number Of ClanWars:~n~Clan EXP:~n~Clan Money:~n~Clan Weapons:");
	TextDrawFont(clanInfoTD[1], 2);
	TextDrawLetterSize(clanInfoTD[1], 0.1499, 1.0000);
	TextDrawColor(clanInfoTD[1], -1);
	TextDrawSetShadow(clanInfoTD[1], 0);
	TextDrawSetOutline(clanInfoTD[1], 0);
	TextDrawBackgroundColor(clanInfoTD[1], 255);
	TextDrawSetProportional(clanInfoTD[1], 1);
	TextDrawTextSize(clanInfoTD[1], 640.0000, 640.0000);

	clanInfoTD[2] = TextDrawCreate(124.0000, 280.5000, "LD_SPAC:WHITE");
	TextDrawFont(clanInfoTD[2], 4);
	TextDrawLetterSize(clanInfoTD[2], 0.1499, 1.0000);
	TextDrawColor(clanInfoTD[2], 842150655);
	TextDrawSetShadow(clanInfoTD[2], 0);
	TextDrawSetOutline(clanInfoTD[2], 0);
	TextDrawBackgroundColor(clanInfoTD[2], 255);
	TextDrawSetProportional(clanInfoTD[2], 1);
	TextDrawTextSize(clanInfoTD[2], 58.5000, 36.0000);

	clanInfoTD[3] = TextDrawCreate(125.0000, 281.5000, "LD_SPAC:WHITE");
	TextDrawFont(clanInfoTD[3], 4);
	TextDrawLetterSize(clanInfoTD[3], 0.1499, 1.0000);
	TextDrawColor(clanInfoTD[3], 421075455);
	TextDrawSetShadow(clanInfoTD[3], 0);
	TextDrawSetOutline(clanInfoTD[3], 0);
	TextDrawBackgroundColor(clanInfoTD[3], 255);
	TextDrawSetProportional(clanInfoTD[3], 1);
	TextDrawTextSize(clanInfoTD[3], 56.5000, 34.0000);

	clanInfoTD[4] = TextDrawCreate(134.5000, 283.0000, "MODEL");
	TextDrawFont(clanInfoTD[4], 5);
	TextDrawLetterSize(clanInfoTD[4], 0.1499, 1.0000);
	TextDrawColor(clanInfoTD[4], -1);
	TextDrawSetShadow(clanInfoTD[4], 0);
	TextDrawSetOutline(clanInfoTD[4], 0);
	TextDrawBackgroundColor(clanInfoTD[4], 0);
	TextDrawSetProportional(clanInfoTD[4], 1);
	TextDrawTextSize(clanInfoTD[4], 37.0000, 31.5000);
	TextDrawSetPreviewModel(clanInfoTD[4], 218);
	TextDrawSetPreviewRot(clanInfoTD[4], 0.0000, 0.0000, 0.0000, 1.0000);

	clanInfoTD[5] = TextDrawCreate(18.0000, 278.5000, "Clan Skin:");
	TextDrawFont(clanInfoTD[5], 2);
	TextDrawLetterSize(clanInfoTD[5], 0.1499, 1.0000);
	TextDrawColor(clanInfoTD[5], -1);
	TextDrawSetShadow(clanInfoTD[5], 0);
	TextDrawSetOutline(clanInfoTD[5], 0);
	TextDrawBackgroundColor(clanInfoTD[5], 255);
	TextDrawSetProportional(clanInfoTD[5], 1);
	TextDrawTextSize(clanInfoTD[5], 640.0000, 640.0000);

	clanInfoTD[6] = TextDrawCreate(188.5000, 322.0000, "Type /close to close box");
	TextDrawFont(clanInfoTD[6], 2);
	TextDrawLetterSize(clanInfoTD[6], 0.1099, 0.8000);
	TextDrawAlignment(clanInfoTD[6], 3);
	TextDrawColor(clanInfoTD[6], -1448498689);
	TextDrawSetShadow(clanInfoTD[6], 0);
	TextDrawSetOutline(clanInfoTD[6], 0);
	TextDrawBackgroundColor(clanInfoTD[6], 255);
	TextDrawSetProportional(clanInfoTD[6], 1);
	TextDrawTextSize(clanInfoTD[6], 640.0000, 640.0000);

	// CLAN HELP BOX GLOBAL TEXTDRAWS
	clanHelpTD[0] = TextDrawCreate(10.0000, 160.0000, "BOX");
	TextDrawFont(clanHelpTD[0], 1);
	TextDrawLetterSize(clanHelpTD[0], 0.0000, 21.7999);
	TextDrawColor(clanHelpTD[0], -1);
	TextDrawSetShadow(clanHelpTD[0], 0);
	TextDrawSetOutline(clanHelpTD[0], 0);
	TextDrawBackgroundColor(clanHelpTD[0], 255);
	TextDrawSetProportional(clanHelpTD[0], 1);
	TextDrawUseBox(clanHelpTD[0], 1);
	TextDrawBoxColor(clanHelpTD[0], 421075400);
	TextDrawTextSize(clanHelpTD[0], 190.0000, 50.0000);

	clanHelpTD[1] = TextDrawCreate(98.0000, 167.5000, "/CLANHELP");
	TextDrawFont(clanHelpTD[1], 2);
	TextDrawLetterSize(clanHelpTD[1], 0.2999, 2.0999);
	TextDrawAlignment(clanHelpTD[1], 2);
	TextDrawColor(clanHelpTD[1], -7601921);
	TextDrawSetShadow(clanHelpTD[1], 0);
	TextDrawSetOutline(clanHelpTD[1], 0);
	TextDrawBackgroundColor(clanHelpTD[1], 255);
	TextDrawSetProportional(clanHelpTD[1], 1);
	TextDrawTextSize(clanHelpTD[1], 640.0000, 0.0000);

	clanHelpTD[2] = TextDrawCreate(18.0000, 194.5000, "General:");
	TextDrawFont(clanHelpTD[2], 2);
	TextDrawLetterSize(clanHelpTD[2], 0.1899, 1.2000);
	TextDrawColor(clanHelpTD[2], -8433409);
	TextDrawSetShadow(clanHelpTD[2], 0);
	TextDrawSetOutline(clanHelpTD[2], 0);
	TextDrawBackgroundColor(clanHelpTD[2], 255);
	TextDrawSetProportional(clanHelpTD[2], 1);
	TextDrawTextSize(clanHelpTD[2], 640.0000, 640.0000);

	clanHelpTD[3] = TextDrawCreate(18.0000, 204.5000, "/clans, /onlineclans, /cmembers, /cinfo, /cjoin");
	TextDrawFont(clanHelpTD[3], 2);
	TextDrawLetterSize(clanHelpTD[3], 0.1499, 1.0000);
	TextDrawColor(clanHelpTD[3], -1);
	TextDrawSetShadow(clanHelpTD[3], 0);
	TextDrawSetOutline(clanHelpTD[3], 0);
	TextDrawBackgroundColor(clanHelpTD[3], 255);
	TextDrawSetProportional(clanHelpTD[3], 1);
	TextDrawTextSize(clanHelpTD[3], 182.0000, 640.0000);

	clanHelpTD[4] = TextDrawCreate(18.0000, 224.5000, "Clan Members:");
	TextDrawFont(clanHelpTD[4], 2);
	TextDrawLetterSize(clanHelpTD[4], 0.1899, 1.2000);
	TextDrawColor(clanHelpTD[4], -8433409);
	TextDrawSetShadow(clanHelpTD[4], 0);
	TextDrawSetOutline(clanHelpTD[4], 0);
	TextDrawBackgroundColor(clanHelpTD[4], 255);
	TextDrawSetProportional(clanHelpTD[4], 1);
	TextDrawTextSize(clanHelpTD[4], 640.0000, 640.0000);

	clanHelpTD[5] = TextDrawCreate(18.0000, 234.5000, "/cinvite, /cquit, /cwithdraw, /cdeposit, /cwithdraw, /cranks");
	TextDrawFont(clanHelpTD[5], 2);
	TextDrawLetterSize(clanHelpTD[5], 0.1499, 1.0000);
	TextDrawColor(clanHelpTD[5], -1);
	TextDrawSetShadow(clanHelpTD[5], 0);
	TextDrawSetOutline(clanHelpTD[5], 0);
	TextDrawBackgroundColor(clanHelpTD[5], 255);
	TextDrawSetProportional(clanHelpTD[5], 1);
	TextDrawTextSize(clanHelpTD[5], 182.0000, 640.0000);

	clanHelpTD[6] = TextDrawCreate(18.0000, 254.5000, "Clan Leaders:");
	TextDrawFont(clanHelpTD[6], 2);
	TextDrawLetterSize(clanHelpTD[6], 0.1899, 1.2000);
	TextDrawColor(clanHelpTD[6], -8433409);
	TextDrawSetShadow(clanHelpTD[6], 0);
	TextDrawSetOutline(clanHelpTD[6], 0);
	TextDrawBackgroundColor(clanHelpTD[6], 255);
	TextDrawSetProportional(clanHelpTD[6], 1);
	TextDrawTextSize(clanHelpTD[6], 640.0000, 640.0000);

	clanHelpTD[7] = TextDrawCreate(18.0000, 264.5000, "/cbuyskin, /cbuyweapon, /csetteam, /csetskin, /csetspawn, /cremovespawn, /cpromote, /ckick, /cranks");
	TextDrawFont(clanHelpTD[7], 2);
	TextDrawLetterSize(clanHelpTD[7], 0.1499, 1.0000);
	TextDrawColor(clanHelpTD[7], -1);
	TextDrawSetShadow(clanHelpTD[7], 0);
	TextDrawSetOutline(clanHelpTD[7], 0);
	TextDrawBackgroundColor(clanHelpTD[7], 255);
	TextDrawSetProportional(clanHelpTD[7], 1);
	TextDrawTextSize(clanHelpTD[7], 182.0000, 640.0000);

	clanHelpTD[8] = TextDrawCreate(18.0000, 294.5000, "Clan Owners:");
	TextDrawFont(clanHelpTD[8], 2);
	TextDrawLetterSize(clanHelpTD[8], 0.1899, 1.2000);
	TextDrawColor(clanHelpTD[8], -8433409);
	TextDrawSetShadow(clanHelpTD[8], 0);
	TextDrawSetOutline(clanHelpTD[8], 0);
	TextDrawBackgroundColor(clanHelpTD[8], 255);
	TextDrawSetProportional(clanHelpTD[8], 1);
	TextDrawTextSize(clanHelpTD[8], 640.0000, 640.0000);

	clanHelpTD[9] = TextDrawCreate(18.0000, 304.5000, "/cchangename, /cchangetag, /csavestats");
	TextDrawFont(clanHelpTD[9], 2);
	TextDrawLetterSize(clanHelpTD[9], 0.1499, 1.0000);
	TextDrawColor(clanHelpTD[9], -1);
	TextDrawSetShadow(clanHelpTD[9], 0);
	TextDrawSetOutline(clanHelpTD[9], 0);
	TextDrawBackgroundColor(clanHelpTD[9], 255);
	TextDrawSetProportional(clanHelpTD[9], 1);
	TextDrawTextSize(clanHelpTD[9], 182.0000, 640.0000);

	clanHelpTD[10] = TextDrawCreate(18.0000, 314.5000, "Admin Level 5:");
	TextDrawFont(clanHelpTD[10], 2);
	TextDrawLetterSize(clanHelpTD[10], 0.1899, 1.2000);
	TextDrawColor(clanHelpTD[10], -8433409);
	TextDrawSetShadow(clanHelpTD[10], 0);
	TextDrawSetOutline(clanHelpTD[10], 0);
	TextDrawBackgroundColor(clanHelpTD[10], 255);
	TextDrawSetProportional(clanHelpTD[10], 1);
	TextDrawTextSize(clanHelpTD[10], 640.0000, 640.0000);

	clanHelpTD[11] = TextDrawCreate(18.0000, 324.5000, "/ccreate, /cdelete, /cgivemoney, /cgiveexp");
	TextDrawFont(clanHelpTD[11], 2);
	TextDrawLetterSize(clanHelpTD[11], 0.1499, 1.0000);
	TextDrawColor(clanHelpTD[11], -1);
	TextDrawSetShadow(clanHelpTD[11], 0);
	TextDrawSetOutline(clanHelpTD[11], 0);
	TextDrawBackgroundColor(clanHelpTD[11], 255);
	TextDrawSetProportional(clanHelpTD[11], 1);
	TextDrawTextSize(clanHelpTD[11], 182.0000, 640.0000);

	clanHelpTD[12] = TextDrawCreate(188.5000, 348.0000, "Type /close to close box");
	TextDrawFont(clanHelpTD[12], 2);
	TextDrawLetterSize(clanHelpTD[12], 0.1099, 0.8000);
	TextDrawAlignment(clanHelpTD[12], 3);
	TextDrawColor(clanHelpTD[12], -1448498689);
	TextDrawSetShadow(clanHelpTD[12], 0);
	TextDrawSetOutline(clanHelpTD[12], 0);
	TextDrawBackgroundColor(clanHelpTD[12], 255);
	TextDrawSetProportional(clanHelpTD[12], 1);
	TextDrawTextSize(clanHelpTD[12], 640.0000, 640.0000);

	// CLAN ANNOUNCEMENT BOX GLOBAL TEXTDRAWS
	clanAnnouncementTD[0] = TextDrawCreate(220.0000, 349.0000, "BOX");
	TextDrawFont(clanAnnouncementTD[0], 1);
	TextDrawLetterSize(clanAnnouncementTD[0], 0.0000, 4.0000);
	TextDrawColor(clanAnnouncementTD[0], -1);
	TextDrawSetShadow(clanAnnouncementTD[0], 0);
	TextDrawSetOutline(clanAnnouncementTD[0], 0);
	TextDrawBackgroundColor(clanAnnouncementTD[0], 255);
	TextDrawSetProportional(clanAnnouncementTD[0], 1);
	TextDrawUseBox(clanAnnouncementTD[0], 1);
	TextDrawBoxColor(clanAnnouncementTD[0], 50);
	TextDrawTextSize(clanAnnouncementTD[0], 420.0000, 0.0000);

	clanAnnouncementTD[1] = TextDrawCreate(318.0000, 347.5000, "(DELTA) IS RECRUITING");
	TextDrawFont(clanAnnouncementTD[1], 2);
	TextDrawLetterSize(clanAnnouncementTD[1], 0.2099, 1.3999);
	TextDrawAlignment(clanAnnouncementTD[1], 2);
	TextDrawColor(clanAnnouncementTD[1], -16776961);
	TextDrawSetShadow(clanAnnouncementTD[1], 0);
	TextDrawSetOutline(clanAnnouncementTD[1], 0);
	TextDrawBackgroundColor(clanAnnouncementTD[1], 255);
	TextDrawSetProportional(clanAnnouncementTD[1], 1);
	TextDrawTextSize(clanAnnouncementTD[1], 640.0000, 480.0000);

	clanAnnouncementTD[2] = TextDrawCreate(320.0000, 362.5000, "~w~Clan ~r~~h~Delta Force Unit ~w~is now recruiting new memebers. To request an invite, contact ~r~~h~OwnerName ~w~(id: ~r~4~w~");
	TextDrawFont(clanAnnouncementTD[2], 2);
	TextDrawLetterSize(clanAnnouncementTD[2], 0.1299, 0.7999);
	TextDrawAlignment(clanAnnouncementTD[2], 2);
	TextDrawColor(clanAnnouncementTD[2], -1);
	TextDrawSetShadow(clanAnnouncementTD[2], 0);
	TextDrawSetOutline(clanAnnouncementTD[2], 0);
	TextDrawBackgroundColor(clanAnnouncementTD[2], 255);
	TextDrawSetProportional(clanAnnouncementTD[2], 1);
	TextDrawTextSize(clanAnnouncementTD[2], 640.0000, 200.0000);
	
	// CLAN INVITE BOX GLOBAL TEXTDRAW
	clanInviteTD = TextDrawCreate(10.0000, 426.0000, "BOX");
	TextDrawFont(clanInviteTD, 1);
	TextDrawLetterSize(clanInviteTD, 0.0000, 4.4000);
	TextDrawColor(clanInviteTD, -1);
	TextDrawSetShadow(clanInviteTD, 0);
	TextDrawSetOutline(clanInviteTD, 0);
	TextDrawBackgroundColor(clanInviteTD, 255);
	TextDrawSetProportional(clanInviteTD, 1);
	TextDrawUseBox(clanInviteTD, 1);
	TextDrawBoxColor(clanInviteTD, 421075400);
	TextDrawTextSize(clanInviteTD, 190.0000, 0.0000);

    if (ConnectDatabase() == 0) {
	    printf("\n==========================================\n");
		printf("[Clan.pwn] - Couldn't connect to MySQL database, closing filterscript.");
		printf("\n==========================================\n");

		return 0;
    }

    foreach (new i : Player) {
        OnPlayerConnect(i);
	}

    printf("\n==========================================\n");
	printf("[Clan.pwn] - Loaded "#VERSION" (updated: "#DATE") - By Gammix");
	printf("\n==========================================\n");

	return 1;
}

public OnFilterScriptExit() {
    DisconnectDatabase();

    KillTimer(clanAnnouncementTimer);

	for (new i = 0; i < sizeof(clanInfoTD); i++)
	    TextDrawDestroy(clanInfoTD[i]);

	for (new i = 0; i < sizeof(clanHelpTD); i++)
	    TextDrawDestroy(clanHelpTD[i]);

	for (new i = 0; i < sizeof(clanAnnouncementTD); i++)
	    TextDrawDestroy(clanAnnouncementTD[i]);

	TextDrawDestroy(clanInviteTD);

	return 1;
}

public OnPlayerConnect(playerid) {
	// CLAN INFO BOX PLAYER TEXTDRAWS
	clanInfoPTD[playerid][0] = CreatePlayerTextDraw(playerid, 98.0000, 167.5000, "(DELTA)");
	PlayerTextDrawFont(playerid, clanInfoPTD[playerid][0], 2);
	PlayerTextDrawLetterSize(playerid, clanInfoPTD[playerid][0], 0.2999, 2.0999);
	PlayerTextDrawAlignment(playerid, clanInfoPTD[playerid][0], 2);
	PlayerTextDrawColor(playerid, clanInfoPTD[playerid][0], -7601921);
	PlayerTextDrawSetShadow(playerid, clanInfoPTD[playerid][0], 0);
	PlayerTextDrawSetOutline(playerid, clanInfoPTD[playerid][0], 0);
	PlayerTextDrawBackgroundColor(playerid, clanInfoPTD[playerid][0], 255);
	PlayerTextDrawSetProportional(playerid, clanInfoPTD[playerid][0], 1);
	PlayerTextDrawTextSize(playerid, clanInfoPTD[playerid][0], 640.0000, 0.0000);

	clanInfoPTD[playerid][1] = CreatePlayerTextDraw(playerid, 98.0000, 183.0000, "DELTA FORCE");
	PlayerTextDrawFont(playerid, clanInfoPTD[playerid][1], 2);
	PlayerTextDrawLetterSize(playerid, clanInfoPTD[playerid][1], 0.2199, 1.5000);
	PlayerTextDrawAlignment(playerid, clanInfoPTD[playerid][1], 2);
	PlayerTextDrawColor(playerid, clanInfoPTD[playerid][1], -8433409);
	PlayerTextDrawSetShadow(playerid, clanInfoPTD[playerid][1], 0);
	PlayerTextDrawSetOutline(playerid, clanInfoPTD[playerid][1], 0);
	PlayerTextDrawBackgroundColor(playerid, clanInfoPTD[playerid][1], 255);
	PlayerTextDrawSetProportional(playerid, clanInfoPTD[playerid][1], 1);
	PlayerTextDrawTextSize(playerid, clanInfoPTD[playerid][1], 640.0000, 640.0000);

	clanInfoPTD[playerid][2] = CreatePlayerTextDraw(playerid, 180.5000, 204.5000, "~w~Delta Force 101~n~~y~(DELTA)Owner~n~~w~1/1009~n~15 (~g~~h~~h~3 online~w~)~n~15 wins, 20 total~n~5093~n~$18,003,129~n~Spas12");
	PlayerTextDrawFont(playerid, clanInfoPTD[playerid][2], 2);
	PlayerTextDrawLetterSize(playerid, clanInfoPTD[playerid][2], 0.1499, 1.0000);
	PlayerTextDrawAlignment(playerid, clanInfoPTD[playerid][2], 3);
	PlayerTextDrawColor(playerid, clanInfoPTD[playerid][2], -1);
	PlayerTextDrawSetShadow(playerid, clanInfoPTD[playerid][2], 0);
	PlayerTextDrawSetOutline(playerid, clanInfoPTD[playerid][2], 0);
	PlayerTextDrawBackgroundColor(playerid, clanInfoPTD[playerid][2], 255);
	PlayerTextDrawSetProportional(playerid, clanInfoPTD[playerid][2], 1);
	PlayerTextDrawTextSize(playerid, clanInfoPTD[playerid][2], 640.0000, 640.0000);

	clanInfoPTD[playerid][3] = CreatePlayerTextDraw(playerid, 125.5000, 280.5000, "~y~218");
	PlayerTextDrawFont(playerid, clanInfoPTD[playerid][3], 2);
	PlayerTextDrawLetterSize(playerid, clanInfoPTD[playerid][3], 0.1499, 1.0000);
	PlayerTextDrawColor(playerid, clanInfoPTD[playerid][3], -1);
	PlayerTextDrawSetShadow(playerid, clanInfoPTD[playerid][3], 0);
	PlayerTextDrawSetOutline(playerid, clanInfoPTD[playerid][3], 0);
	PlayerTextDrawBackgroundColor(playerid, clanInfoPTD[playerid][3], 255);
	PlayerTextDrawSetProportional(playerid, clanInfoPTD[playerid][3], 1);
	PlayerTextDrawTextSize(playerid, clanInfoPTD[playerid][3], 640.0000, 640.0000);

	// CLAN NAME PLAYER TEXTDRAW
    clanNamePTD[playerid] = CreatePlayerTextDraw(playerid, 609.5000, 101.0000, "Clan name here");
	PlayerTextDrawFont(playerid, clanNamePTD[playerid], 2);
	PlayerTextDrawLetterSize(playerid, clanNamePTD[playerid], 0.1999, 1.1000);
	PlayerTextDrawAlignment(playerid, clanNamePTD[playerid], 3);
	PlayerTextDrawColor(playerid, clanNamePTD[playerid], 2147418367);
	PlayerTextDrawSetShadow(playerid, clanNamePTD[playerid], 0);
	PlayerTextDrawSetOutline(playerid, clanNamePTD[playerid], 1);
	PlayerTextDrawBackgroundColor(playerid, clanNamePTD[playerid], 255);
	PlayerTextDrawSetProportional(playerid, clanNamePTD[playerid], 1);
	PlayerTextDrawTextSize(playerid, clanNamePTD[playerid], 640.0000, 0.0000);

	// CLAN INVITE BOX PLAYER TEXTDRAWS
	clanInvitePTD[playerid][0] = CreatePlayerTextDraw(playerid, 189.5000, 424.0000, "30");
	PlayerTextDrawFont(playerid, clanInvitePTD[playerid][0], 2);
	PlayerTextDrawLetterSize(playerid, clanInvitePTD[playerid][0], 0.1699, 1.2000);
	PlayerTextDrawAlignment(playerid, clanInvitePTD[playerid][0], 3);
	PlayerTextDrawColor(playerid, clanInvitePTD[playerid][0], -1768515841);
	PlayerTextDrawSetShadow(playerid, clanInvitePTD[playerid][0], 0);
	PlayerTextDrawSetOutline(playerid, clanInvitePTD[playerid][0], 0);
	PlayerTextDrawBackgroundColor(playerid, clanInvitePTD[playerid][0], 255);
	PlayerTextDrawSetProportional(playerid, clanInvitePTD[playerid][0], 1);
	PlayerTextDrawTextSize(playerid, clanInvitePTD[playerid][0], 640.0000, 640.0000);

	clanInvitePTD[playerid][1] = CreatePlayerTextDraw(playerid, 12.0000, 426.5000, "(DELTA)UserName is inviting you to join clan ~y~(DELTA) Delta Force");
	PlayerTextDrawFont(playerid, clanInvitePTD[playerid][1], 2);
	PlayerTextDrawLetterSize(playerid, clanInvitePTD[playerid][1], 0.1099, 0.8999);
	PlayerTextDrawColor(playerid, clanInvitePTD[playerid][1], -1);
	PlayerTextDrawSetShadow(playerid, clanInvitePTD[playerid][1], 0);
	PlayerTextDrawSetOutline(playerid, clanInvitePTD[playerid][1], 0);
	PlayerTextDrawBackgroundColor(playerid, clanInvitePTD[playerid][1], 255);
	PlayerTextDrawSetProportional(playerid, clanInvitePTD[playerid][1], 1);
	PlayerTextDrawTextSize(playerid, clanInvitePTD[playerid][1], 181.5000, 640.0000);

	clanInvitePTD[playerid][2] = CreatePlayerTextDraw(playerid, 12.0000, 440.5000, "Type /cjoin to accept invitation");
	PlayerTextDrawFont(playerid, clanInvitePTD[playerid][2], 2);
	PlayerTextDrawLetterSize(playerid, clanInvitePTD[playerid][2], 0.0899, 0.5999);
	PlayerTextDrawColor(playerid, clanInvitePTD[playerid][2], -1);
	PlayerTextDrawSetShadow(playerid, clanInvitePTD[playerid][2], 0);
	PlayerTextDrawSetOutline(playerid, clanInvitePTD[playerid][2], 0);
	PlayerTextDrawBackgroundColor(playerid, clanInvitePTD[playerid][2], 255);
	PlayerTextDrawSetProportional(playerid, clanInvitePTD[playerid][2], 1);
	PlayerTextDrawTextSize(playerid, clanInvitePTD[playerid][2], 181.5000, 640.0000);

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new query[128];
	mysql_format(database, query, sizeof(query),
	    "SELECT * FROM clan_members WHERE name = '%e' LIMIT 1",
	    name
	);

	mysql_tquery(database, query, "OnPlayerClanDataLoad", "i", playerid);
	return 1;
}

forward OnPlayerClanDataLoad(playerid);
public OnPlayerClanDataLoad(playerid) {
	playerClanID[playerid] = -1;

	if (cache_num_rows() == 0) {
		playerClanID[playerid] = -1;
		playerClanRank[playerid] = 0;
        playerClanWeaponsEnabled[playerid] = false;
        clanInviteFrom[playerid] = INVALID_PLAYER_ID;
		clanInviteTimer[playerid] = -1;
		clanInviteCountDown[playerid] = 0;
		playerClan3DTextLabel[playerid] = Text3D:INVALID_STREAMER_ID;

		SendClientMessage(playerid, COLOR_BIEGE, "Clan: To view a list of clans, use /clans or /onlineclans. For commands related to clans, use /chelp.");
	}
	else {
	    new clan_name[MAX_CLAN_NAME];
		cache_get_value(0, "clan_name", clan_name);
		foreach_clans(i) {
			if (!strcmp(clan_name, clanData[i][CLAN_NAME], true)) {
        		playerClanID[playerid] = i;
        		break;
			}
		}

		if (playerClanID[playerid] == -1) {
			SendClientMessage(playerid, COLOR_TOMATO, "Error: For some reason your clan wasn't found in database, either clan owner deleted it or its system error!");
			SendClientMessage(playerid, COLOR_TOMATO, "Error: To check if the owner deleted the clan, use /clans *clan name* to find your clan name there!");
			return 1;
		}

		cache_get_value_int(0, "rank", playerClanRank[playerid]);
		cache_get_value_int(0, "toggle_weapons", playerClanWeaponsEnabled[playerid]);
		cache_get_value_int(0, "last_withdraw_timestamp", playerClanLastWithdrawTimestamp[playerid]);
		
        clanInviteFrom[playerid] = INVALID_PLAYER_ID;
		clanInviteTimer[playerid] = -1;
		clanInviteCountDown[playerid] = 0;

		new string[164];
		format(string, sizeof(string), "[%s] %s", clanData[playerClanID[playerid]][CLAN_TAG], clanRankNames[playerClanID[playerid]][playerClanRank[playerid]]);
    	playerClan3DTextLabel[playerid] = CreateDynamic3DTextLabel(string, alpha(TEAMS[clanData[playerClanID[playerid]][CLAN_TEAM]][TEAM_COLOR], 150), 0.0, 0.0, 0.0, 25.0, playerid, .testlos = 1);

		format(string, sizeof(string),
			"Clan: Your clan data has been loaded, you're in clan "COL_DEFAULT"%s "COL_GREEN"(your rank: "COL_DEFAULT"[%i]%s"COL_GREEN").",
			clanData[playerClanID[playerid]][CLAN_NAME], playerClanRank[playerid] + 1, clanRankNames[playerClanID[playerid]][playerClanRank[playerid]]
		);
		SendClientMessage(playerid, COLOR_GREEN, string);
		SendClientMessage(playerid, COLOR_GREEN, "Clan: You can checkout all clan commands using /clanhelp.");
		
		new name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, name, sizeof(name));
		format(string, sizeof(string), "Clan %s, %s has joined!", clanRankNames[playerClanID[playerid]][playerClanRank[playerid]], name);
		
		foreach (new i : Player) {
			if (i != playerid && playerClanID[i] == playerClanID[playerid]) {
				SendClientMessage(playerid, COLOR_BIEGE, string);
			}
		}
	}

	return 1;
}

public OnPlayerDisconnect(playerid, reason) {
	new clanid = playerClanID[playerid];
	if (clanid != -1) {
        UpdatePlayerClanData(playerid);

		DestroyDynamic3DTextLabel(playerClan3DTextLabel[playerid]);

		new name[MAX_PLAYER_NAME];
		new string[164];
		GetPlayerName(playerid, name, sizeof(name));
		format(string, sizeof(string), "Clan %s, %s has left server!", clanRankNames[playerClanID[playerid]][playerClanRank[playerid]], name);

		foreach (new i : Player) {
			if (i != playerid && playerClanID[i] == playerClanID[playerid]) {
				SendClientMessage(playerid, COLOR_BIEGE, string);
			}
		}
	}
	else {
	    if (clanInviteFrom[playerid] != INVALID_PLAYER_ID) {
		    new name[MAX_PLAYER_NAME];
			new string[164];

		    GetPlayerName(playerid, name, sizeof(name));
			format(string, sizeof(string), "* Clan invitation to "COL_DEFAULT"%s "COL_BIEGE"has been canceled, player left server!", name);
			SendClientMessage(clanInviteFrom[playerid], COLOR_BIEGE, string);
	    	PlayerPlaySound(clanInviteFrom[playerid], 1057, 0.0, 0.0, 0.0);
	    	
			TextDrawHideForPlayer(playerid, clanInviteTD);

			KillTimer(clanInviteTimer[playerid]);
			clanInviteFrom[playerid] = INVALID_PLAYER_ID;
			clanInviteTimer[playerid] = -1;
			clanInviteCountDown[playerid] = 0;
			return 1;
		}
	}

	return 1;
}

forward OnPlayerCpatureZone(playerid, zoneid, previous_owner);
public OnPlayerCpatureZone(playerid, zoneid, previous_owner) {
	if (playerClanID[playerid] != -1) {
 		if (GetPlayerTeam(playerid) == clanData[playerClanID[playerid]][CLAN_TEAM]) {
			++clanData[playerClanID[playerid]][CLAN_TOTAL_EXP];

			SendClanReward(playerid, "capture", 150, 2);
		}
	}

	return 1;
}

public OnPlayerDeath(playerid, killerid, reason) {
	if (killerid != INVALID_PLAYER_ID) {
	    if (playerClanID[killerid] != -1) {
	        if (GetPlayerTeam(killerid) == clanData[playerClanID[killerid]][CLAN_TEAM]) {
				++clanData[playerClanID[killerid]][CLAN_TOTAL_EXP];

				SendClanReward(playerid, "kill", 150, 1);
			}
		}
	}

	return 1;
}

public OnPlayerSpawn(playerid) {
	new clanid = playerClanID[playerid];
    if (clanid != -1) {
		new string[164];
		format(string, sizeof(string), "~g~~h~~h~(%s) ~g~~h~%s ~w~(Rank: (%i)%s)", clanData[clanid][CLAN_TAG], clanData[clanid][CLAN_NAME], playerClanRank[playerid] + 1, clanRankNames[clanid][playerClanRank[playerid]]);
		PlayerTextDrawSetString(playerid, clanNamePTD[playerid], string);
		PlayerTextDrawShow(playerid, clanNamePTD[playerid]);

		// cusotm clan spawn
		if (clanData[clanid][CLAN_SPAWN_POS][0] != 0.0 && clanData[clanid][CLAN_SPAWN_POS][1] != 0.0 && clanData[clanid][CLAN_SPAWN_POS][2] != 0.0) {
		    SetPlayerPos(playerid, clanData[clanid][CLAN_SPAWN_POS][0], clanData[clanid][CLAN_SPAWN_POS][1], clanData[clanid][CLAN_SPAWN_POS][2]);
			SetPlayerFacingAngle(playerid, clanData[clanid][CLAN_SPAWN_POS][3]);
		 	SetPlayerInterior(playerid, clanData[clanid][CLAN_SPAWN_INTERIORID]);
	 		SetPlayerVirtualWorld(playerid, clanData[clanid][CLAN_SPAWN_WORLDID]);
		}
		//

		if (GetPlayerTeam(playerid) != clanData[clanid][CLAN_TEAM]) {
			format(string, sizeof(string), "~r~~h~You are breaking clan rules~n~~r~~h~please switch to~n~~r~~h~team ~r~%s!", TEAMS[clanData[clanid][CLAN_TEAM]][TEAM_NAME]);
			GameTextForPlayer(playerid, string, 7000, 3);
		}
		else {
		    // clan weapons
		    string = "Clan Weapons received: ";
			for (new i = 0; i < MAX_CLAN_WEAPONS; i++) {
				if (clanData[clanid][CLAN_VAULT_WEAPONS][i] != -1) {
				    if ((gettime() - clanData[clanid][CLAN_VAULT_WEAPONS_TIMESTAMP][i]) >= (CLAN_WEAPON_EXPIRE_INTERVAL * 24 * 60 * 60)) { // if weapon is expired
				        clanData[clanid][CLAN_VAULT_WEAPONS][i] = -1;
				        clanData[clanid][CLAN_VAULT_WEAPONS_TIMESTAMP][i] = -1;
				    }
				    else {
				        if (playerClanRank[playerid] >= CLAN_RANK_LEADER) {
					        if ((gettime() - clanData[clanid][CLAN_VAULT_WEAPONS_TIMESTAMP][i]) <= ((CLAN_WEAPON_EXPIRE_INTERVAL - 1) * 24 * 60 * 60)) { // if 1 day left for expire
								format(string, sizeof(string),
									"Your clan weapon "COL_DEFAULT"%s "COL_WHITE"is about to expire in "COL_DEFAULT"%s "COL_WHITE", you can renew it using "COL_DEFAULT"/cbuyweapon",
								    CLAN_WEAPONS[clanData[clanid][CLAN_VAULT_WEAPONS][i]][CLAN_WEAPON_NAME],
									ReturnTimelapse(clanData[clanid][CLAN_VAULT_WEAPONS_TIMESTAMP][i], gettime())
								);
								SendClientMessage(playerid, COLOR_WHITE, string);
							}
						}

						if (playerClanWeaponsEnabled[playerid]) {
							GivePlayerWeapon(playerid, CLAN_WEAPONS[clanData[clanid][CLAN_VAULT_WEAPONS][i]][CLAN_WEAPON_ID], CLAN_WEAPONS[clanData[clanid][CLAN_VAULT_WEAPONS][i]][CLAN_WEAPON_AMMO]);
						}
						
						format(string, sizeof(string),
							"%s"COL_GREEN"%s",
							string, CLAN_WEAPONS[clanData[clanid][CLAN_VAULT_WEAPONS][i]][CLAN_WEAPON_NAME]
						);
					}
				} else {
					strcat(string, COL_DEFAULT "empty");
				}

				if (i != (MAX_CLAN_WEAPONS - 1)) {
					strcat(string, COL_WHITE ", ");
				}
			}
			
			if (playerClanWeaponsEnabled[playerid]) {
				SendClientMessage(playerid, COLOR_WHITE, string);
			}
			//

		    // clan skin
			SetPlayerSkin(playerid, clanData[clanid][CLAN_SKIN]);
			//
		}
	}
	else {
		PlayerTextDrawHide(playerid, clanNamePTD[playerid]);
	}

	return 1;
}

public OnPlayerPickUpDynamicPickup(playerid, pickupid) {
    foreach_clans(i) {
		if (clanData[i][CLAN_PICKUPID] == pickupid) {
		    new string[MAX_CLAN_TAG_NAME + 24];
		    format(string, sizeof(string), "~g~~h~(%s) SPAWN POINT", clanData[i][CLAN_TAG]);
		    GameTextForPlayer(playerid, string, 5000, 3);
			break;
		}
	}

	return 1;
}

public OnPlayerText(playerid, text[]) { // CLAN CHAT
	if (text[0] == '@') {
        if (playerClanID[playerid] != -1) {
	        new name[MAX_PLAYER_NAME];
	        GetPlayerName(playerid, name, sizeof(name));

			new message[164];
			format(message, sizeof(message), "[CLAN CHAT] %s[%i]: %s", name, playerid, text[1]);

			foreach (new i : Player) {
				if (playerClanID[i] == playerClanID[playerid]) {
					SendClientMessage(playerid, COLOR_YELLOW, message);
				}
			}
	        return 0;
		}
    }

    return 1;
}
// ---------------------------------------

// ---------------------------------------
// COMMANDS
CMD:close(playerid) {
	switch (GetPVarInt(playerid, "ActiveBox")) {
	    case 1: { // /chelp box
	        for (new i = 0; i < sizeof(clanInfoTD); i++)
				TextDrawHideForPlayer(playerid, clanInfoTD[i]);

			for (new i = 0; i < sizeof(clanInfoPTD[]); i++)
				PlayerTextDrawHide(playerid, clanInfoPTD[playerid][i]);
				
			KillTimer(GetPVarInt(playerid, "ActiveBoxTimer"));
			DeletePVar(playerid, "ActiveBoxTimer");
		}

		case 2: { // /cstats or /cinfo box
			for (new i = 0; i < sizeof(clanHelpTD); i++)
				TextDrawHideForPlayer(playerid, clanHelpTD[i]);
		}
	}

	SetPVarInt(playerid, "ActiveBox", 0);
	return 1;
}

CMD:chelp(playerid) {
	if (GetPVarInt(playerid, "ActiveBox") != 2) {
	    if (cmd_close(playerid) == 0) {
			return 1;
		}
		SetPVarInt(playerid, "ActiveBox", 2);
	}

	for (new i = 0; i < sizeof(clanHelpTD); i++) {
		TextDrawShowForPlayer(playerid, clanHelpTD[i]);
	}
	return 1;
}
CMD:clanhelp(playerid) return cmd_chelp(playerid);

CMD:clans(playerid) {
	new bool:empty = true;
	new total_members, online_members;

    static string[MAX_CLANS * (MAX_CLAN_TAG_NAME + MAX_CLAN_NAME + MAX_PLAYER_NAME + 64)];
	string = "RANK (ranking based on exp)\tCLAN TAG/NAME\tOWNER\tNUMBER OF MEMBERS\n";

	for (new i = 0; i < totalSortedClans; i++) {
		if (clanData[sortedClansList[i][1]][CLAN_NAME][0] != EOS) {
		    empty = false;

		    CountClanMembers(sortedClansList[i][1], total_members, online_members);

			format(string, sizeof(string),
				"%s\
				{%06x}%i/%i (%i EXP)\t\
				{%06x}[%s] %s\t\
				{%06x}%s\t\
				{%06x}%i Members ("COL_GREEN"%i online{%06x})\n\
				",
				string,
				TEAMS[clanData[sortedClansList[i][1]][CLAN_TEAM]][TEAM_COLOR] >>> 8, (i + 1), totalSortedClans, sortedClansList[i][0],
				TEAMS[clanData[sortedClansList[i][1]][CLAN_TEAM]][TEAM_COLOR] >>> 8, clanData[sortedClansList[i][1]][CLAN_TAG], clanData[sortedClansList[i][1]][CLAN_NAME],
				TEAMS[clanData[sortedClansList[i][1]][CLAN_TEAM]][TEAM_COLOR] >>> 8, clanData[sortedClansList[i][1]][CLAN_OWNER],
				TEAMS[clanData[sortedClansList[i][1]][CLAN_TEAM]][TEAM_COLOR] >>> 8, total_members, online_members, TEAMS[clanData[sortedClansList[i][1]][CLAN_TEAM]][TEAM_COLOR] >>> 8
			);
		}
	}

	if (empty == true) {
		strcat(string, COL_TOMATO "n/a\t"COL_TOMATO"n/a\t"COL_TOMATO"n/a\t"COL_TOMATO"n/a");
	}

	return Dialog_Show(playerid, 0, DIALOG_STYLE_TABLIST_HEADERS, "CODE5 Resseruction clans list:", string, "CLOSE", "");
}

CMD:onlineclans(playerid) {
	new bool:empty = true;
	new total_members, online_members;

	static string[MAX_CLANS * (MAX_CLAN_TAG_NAME + MAX_CLAN_NAME + MAX_PLAYER_NAME + 64)];
	string = "RANK (ranking based on exp)\tCLAN TAG/NAME\tOWNER\tNUMBER OF MEMBERS\n";

	for (new i = 0; i < totalSortedClans; i++) {
		if (clanData[sortedClansList[i][1]][CLAN_NAME][0] != EOS) {
		    CountClanMembers(sortedClansList[i][1], total_members, online_members);

			if (online_members != 0) {
		    	empty = false;
		    	
				format(string, sizeof(string),
					"%s\
					{%06x}%i/%i (%i EXP)\t\
					{%06x}[%s] %s\t\
					{%06x}%s\t\
					{%06x}%i Members ("COL_GREEN"%i online{%06x})\n\
					",
					string,
					TEAMS[clanData[sortedClansList[i][1]][CLAN_TEAM]][TEAM_COLOR] >>> 8, (i + 1), totalSortedClans, sortedClansList[i][0],
					TEAMS[clanData[sortedClansList[i][1]][CLAN_TEAM]][TEAM_COLOR] >>> 8, clanData[sortedClansList[i][1]][CLAN_TAG], clanData[sortedClansList[i][1]][CLAN_NAME],
					TEAMS[clanData[sortedClansList[i][1]][CLAN_TEAM]][TEAM_COLOR] >>> 8, clanData[sortedClansList[i][1]][CLAN_OWNER],
					TEAMS[clanData[sortedClansList[i][1]][CLAN_TEAM]][TEAM_COLOR] >>> 8, total_members, online_members, TEAMS[clanData[sortedClansList[i][1]][CLAN_TEAM]][TEAM_COLOR] >>> 8
				);
			}
		}
	}

	if (empty == true) {
		strcat(string, COL_TOMATO "n/a\t"COL_TOMATO"n/a\t"COL_TOMATO"n/a\t"COL_TOMATO"n/a");
	}

	return Dialog_Show(playerid, 0, DIALOG_STYLE_TABLIST_HEADERS, "CODE5 Resseruction online clans list:", string, "CLOSE", "");
}

CMD:cmembers(playerid, params[]) {
	new clanid;
	if (sscanf(params, "k<clan>", clanid))
	    return SendClientMessage(playerid, COLOR_LIGHT_AQUA, "Usage: /cmembers [name/tag/id]");

	if (clanid == -1)
		return SendClientMessage(playerid, COLOR_TOMATO, "Error: Clan wasn't found, try /clans for a full list.");

	new query[128];
	mysql_format(database, query, sizeof(query),
		"SELECT name, rank FROM clan_members WHERE clan_name = '%e'",
		clanData[clanid][CLAN_NAME]
	);
    mysql_tquery(database, query, "OnClanMembersDataLoad", "ii", clanid, playerid);

	return 1;
}

forward OnClanMembersDataLoad(clanid, playerid);
public OnClanMembersDataLoad(clanid, playerid) {
    static string[100 * (MAX_PLAYER_NAME + MAX_CLAN_RANK_NAME + 32)];
	string = "S.NO.\tMEMBER NAME\tMEMBER'S CLAN RANKING\n";

	new mysql_name[MAX_PLAYER_NAME];
	new player_name[MAX_PLAYER_NAME];
	new rankid;
	new color;
	for (new i = 0, j = cache_num_rows(); i < j; i++) {
	    cache_get_value(i, "name", mysql_name, sizeof(mysql_name));
	    cache_get_value_int(i, "rank", rankid);

		color = COLOR_TOMATO;
		foreach (new x : Player) {
			if (playerClanID[x] == playerClanID[playerid]) {
			    GetPlayerName(x, player_name, sizeof(player_name));
				if (!strcmp(mysql_name, player_name)) {
				    color = COLOR_GREEN;
				    break;
				}
			}
		}
		
		format(string, sizeof(string),
			"%s\
			{%06x}%i\t\
			{%06x}%s\t\
			{%06x}%s (lvl: %i)\n\
			",
			string,
			color >>> 8, i + 1,
			color >>> 8, mysql_name,
			color >>> 8, clanRankNames[clanid][rankid], rankid + 1
		);
	}

	SendClientMessage(playerid, COLOR_WHITE, "["COL_GREEN"Green"COL_WHITE"] color is for members who are online and ["COL_TOMATO"Red"COL_WHITE"] for offline members.");

	return Dialog_Show(playerid, 0, DIALOG_STYLE_TABLIST_HEADERS, GetClanDialogHeader(playerClanID[playerid], "Clan members list"), string, "CLOSE", "");
}

CMD:cinfo(playerid, params[]) {
	new clanid;
	if (sscanf(params, "k<clan>", clanid)) {
	    if (playerClanID[playerid] == -1)
	        return SendClientMessage(playerid, COLOR_LIGHT_AQUA, "Usage: /cinfo [name/tag/id]");

	    clanid = playerClanID[playerid];
		SendClientMessage(playerid, COLOR_DEFAULT, "Tip: You can also view other clan's info via: /cinfo [name/tag/id]");
	}

	if (clanid == -1)
		return SendClientMessage(playerid, COLOR_TOMATO, "Error: Clan wasn't found, try /clans for a full list.");

	if (GetPVarInt(playerid, "ActiveBox") != 1) {
	    if (cmd_close(playerid) == 0) {
			return 1;
		}
		SetPVarInt(playerid, "ActiveBox", 1);
		SetPVarInt(playerid, "ActiveBoxTimer", SetTimerEx("ShowClanInfo", 5000, true, "ii", playerid, clanid));
	}

	ShowClanInfo(playerid, clanid);
	return 1;
}
CMD:cstats(playerid, params[]) return cmd_cinfo(playerid, params);

forward ShowClanInfo(playerid, clanid);
public ShowClanInfo(playerid, clanid) {
	TextDrawSetPreviewModel(clanInfoTD[3], clanData[clanid][CLAN_SKIN]);

	new string[256];
	new total_members, online_members;
	CountClanMembers(clanid, total_members, online_members);

	format(string, sizeof(string), "(%s)", clanData[clanid][CLAN_TAG]);
	PlayerTextDrawSetString(playerid, clanInfoPTD[playerid][0], string);
	PlayerTextDrawSetString(playerid, clanInfoPTD[playerid][1], clanData[clanid][CLAN_NAME]);

	new count;
	for (new i = 0; i < MAX_CLAN_WEAPONS; i++) {
		if (clanData[clanid][CLAN_VAULT_WEAPONS][i] != -1) {
		    ++count;
		}
	}

	new vault_weapons[MAX_CLAN_WEAPONS * 32];
	if (count == 0) {
	    vault_weapons = "none";
	}
	else {
		new count2;
		for (new i = 0; i < MAX_CLAN_WEAPONS; i++) {
			if (clanData[clanid][CLAN_VAULT_WEAPONS][i] != -1) {
			    strcat(vault_weapons, CLAN_WEAPONS[clanData[clanid][CLAN_VAULT_WEAPONS][i]][CLAN_WEAPON_NAME]);

			    if (++count2 != count) {
			        strcat(vault_weapons, ", ");
			    } else {
					break;
				}
			}
		}
	}

	format(string, sizeof(string),
		"~w~%s~n~\
		~y~%s~n~\
		~w~%i/%02i~n~\
		%i (~g~~h~~h~%i online~w~)~n~\
		%i wins, %i total~n~\
		%i points~n~\
		$%s~n~\
		%s~n~",
		clanData[clanid][CLAN_NAME],
		clanData[clanid][CLAN_OWNER],
		total_members, online_members,
		GetClanRank(clanid), totalSortedClans,
		clanData[clanid][CLAN_WAR_WINS],
		clanData[clanid][CLAN_WAR_TOTAL],
		clanData[clanid][CLAN_TOTAL_EXP],
		FormatNumber(clanData[clanid][CLAN_VAULT_MONEY]),
		vault_weapons
	);
	PlayerTextDrawSetString(playerid, clanInfoPTD[playerid][2], string);

	valstr(string, clanData[clanid][CLAN_SKIN]);
	PlayerTextDrawSetString(playerid, clanInfoPTD[playerid][3], string);

	TextDrawSetPreviewModel(clanInfoTD[4], clanData[clanid][CLAN_SKIN]);

	for (new i = 0; i < sizeof(clanInfoTD); i++) {
		TextDrawShowForPlayer(playerid, clanInfoTD[i]);
	}
	for (new i = 0; i < sizeof(clanInfoPTD[]); i++) {
		PlayerTextDrawShow(playerid, clanInfoPTD[playerid][i]);
	}
}

CMD:cinvite(playerid, params[]) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");

	new targetid;
	if (sscanf(params, "u", targetid))
	    return SendClientMessage(playerid, COLOR_LIGHT_AQUA, "Usage: /cinvite [id/name]");

	if (targetid == playerid)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Why would you invite yourself!");

	if (!IsPlayerConnected(targetid))
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: The player isn't connected.");

	if (playerClanID[targetid] == playerClanID[playerid])
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: The player is already in your clan.");

	if (playerClanID[targetid] != -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: The player is already in another clan.");

    if (clanInviteFrom[targetid] != INVALID_PLAYER_ID && clanInviteFrom[targetid] != playerid) {
	    new name[MAX_PLAYER_NAME];
		new string[164];

	    GetPlayerName(targetid, name, sizeof(name));
		format(string, sizeof(string), "* Clan invitation to "COL_DEFAULT"%s "COL_BIEGE"has been canceled, player recieved another invite!", name);
		SendClientMessage(clanInviteFrom[targetid], COLOR_BIEGE, string);
    	PlayerPlaySound(clanInviteFrom[targetid], 1057, 0.0, 0.0, 0.0);

		KillTimer(clanInviteTimer[targetid]);
		clanInviteFrom[targetid] = INVALID_PLAYER_ID;
		clanInviteTimer[targetid] = -1;
		clanInviteCountDown[targetid] = 0;
	}

	new string[164];
	new player_name[MAX_PLAYER_NAME];
    new target_name[MAX_PLAYER_NAME];
    
	GetPlayerName(playerid, player_name, sizeof(player_name));
	GetPlayerName(targetid, target_name, sizeof(target_name));

	// ------------
	// PLAYERID
	// ------------
	format(string, sizeof(string), "* "COL_DEFAULT"%s "COL_BIEGE"has invited "COL_DEFAULT"%s "COL_BIEGE"to our clan!", player_name, target_name);

 	foreach (new i : Player) {
		if (playerClanID[i] == playerClanID[playerid]) {
	    	SendClientMessage(i, COLOR_BIEGE, string);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	format(string, sizeof(string), "~n~~n~~n~~n~~n~~y~CLAN INVITATION SENT TO~n~~y~~h~%s", target_name);
	GameTextForPlayer(playerid, string, 5000, 3);

	// ------------
	// TARGETID
	// ------------
    PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);

	clanInviteFrom[targetid] = playerid;
	clanInviteTimer[targetid] = SetTimerEx("OnPlayerClanInviteUpdate", 1000, true, "i", targetid);
	clanInviteCountDown[targetid] = 60;

	format(string, sizeof(string), "~n~~n~~n~~n~~n~~y~CLAN INVITATION FROM ~w~~h~(%s)", clanData[playerClanID[playerid]][CLAN_TAG]);
	GameTextForPlayer(targetid, string, 5000, 3);

	format(string, sizeof(string), "* "COL_DEFAULT"%s "COL_BIEGE"has invited you to join clan: "COL_DEFAULT"[%s] %s", player_name, clanData[playerClanID[playerid]][CLAN_TAG], clanData[playerClanID[playerid]][CLAN_NAME]);
	SendClientMessage(targetid, COLOR_BIEGE, string);
	SendClientMessage(targetid, COLOR_BIEGE, "* Type /cjoin to accept invitation or ignore this message, the invitation will expire in 60 seconds!");
	SendClientMessage(targetid, COLOR_BIEGE, "");

	PlayerTextDrawSetString(targetid, clanInvitePTD[targetid][0], "60");
	format(string, sizeof(string), "%s is inviting you to join clan ~y~(%s) %s", player_name, clanData[playerClanID[playerid]][CLAN_TAG], clanData[playerClanID[playerid]][CLAN_NAME]);
	PlayerTextDrawSetString(targetid, clanInvitePTD[targetid][1], string);

	TextDrawShowForPlayer(targetid, clanInviteTD);
	for (new i = 0; i < sizeof(clanInvitePTD[]); i++) {
	    PlayerTextDrawShow(targetid, clanInvitePTD[targetid][i]);
	}
	return 1;
}

forward OnPlayerClanInviteUpdate(playerid);
public OnPlayerClanInviteUpdate(playerid) {
	--clanInviteCountDown[playerid];
	
	if (clanInviteCountDown[playerid] == 0) {
	    new name[MAX_PLAYER_NAME];
		new string[164];
		
	    GetPlayerName(playerid, name, sizeof(name));
		format(string, sizeof(string), "* Clan invitation to "COL_DEFAULT"%s "COL_BIEGE"has expired!", name);
		SendClientMessage(clanInviteFrom[playerid], COLOR_BIEGE, string);
    	PlayerPlaySound(clanInviteFrom[playerid], 1057, 0.0, 0.0, 0.0);
    	
	    GetPlayerName(clanInviteFrom[playerid], name, sizeof(name));
		format(string, sizeof(string), "* Clan invitation from "COL_DEFAULT"%s "COL_BIEGE"has expired!", name);
		SendClientMessage(playerid, COLOR_BIEGE, string);
    	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	
		TextDrawHideForPlayer(playerid, clanInviteTD);
		for (new i = 0; i < sizeof(clanInvitePTD[]); i++) {
		    PlayerTextDrawHide(playerid, clanInvitePTD[playerid][i]);
		}
		
		KillTimer(clanInviteTimer[playerid]);
		clanInviteFrom[playerid] = INVALID_PLAYER_ID;
		clanInviteTimer[playerid] = -1;
		clanInviteCountDown[playerid] = 0;
		return 1;
	}
	
	new string[11];
	valstr(string, clanInviteCountDown[playerid]);
	PlayerTextDrawSetString(playerid, clanInvitePTD[playerid][0], string);
	return 1;
}

CMD:cjoin(playerid) {
	if (GetPVarType(playerid, "ClanInviteFrom") == PLAYER_VARTYPE_NONE)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You have no clan invitation.");

	new fromid = clanInviteFrom[playerid];
	if (!IsPlayerConnected(fromid)) {
        KillTimer(clanInviteTimer[playerid]);
		clanInviteFrom[playerid] = INVALID_PLAYER_ID;
		clanInviteTimer[playerid] = -1;
		clanInviteCountDown[playerid] = 0;
		
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: The player who invited you, has disconnected.");
	}
	
	KillTimer(clanInviteTimer[playerid]);
	clanInviteFrom[playerid] = INVALID_PLAYER_ID;
	clanInviteTimer[playerid] = -1;
	clanInviteCountDown[playerid] = 0;

	TextDrawHideForPlayer(playerid, clanInviteTD);
	for (new i = 0; i < sizeof(clanInvitePTD[]); i++) {
	    PlayerTextDrawHide(playerid, clanInvitePTD[playerid][i]);
	}

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

    playerClanID[playerid] = playerClanID[fromid];
    playerClanRank[playerid] = 0;

	new string[256];
    mysql_format(database, string, sizeof(string),
        "INSERT INTO clan_members (\
            clan_name, name, rank\
		) VALUES (\
			'%e', '%e', %i\
		)",
		clanData[playerClanID[fromid]][CLAN_NAME], name, 0
	);
	mysql_tquery(database, string);

    new fromName[MAX_PLAYER_NAME];
	GetPlayerName(fromid, fromName, sizeof(fromName));

	format(string, sizeof(string), "* Please welcome "COL_DEFAULT"%s"COL_BIEGE", who have just joined "COL_DEFAULT"%s"COL_BIEGE"!", name, clanData[playerClanID[fromid]][CLAN_NAME]);

 	foreach (new i : Player) {
		if (playerClanID[i] == playerClanID[playerid]) {
	    	SendClientMessage(i, COLOR_BIEGE, string);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	format(string, sizeof(string), "* "COL_DEFAULT"%s "COL_BIEGE"accpeted your clan invitation and is now member of "COL_DEFAULT"%s", name, clanData[playerClanID[fromid]][CLAN_NAME]);
	SendClientMessage(fromid, COLOR_BIEGE, string);

    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
    PlayerPlaySound(fromid, 1057, 0.0, 0.0, 0.0);
    
	format(string, sizeof(string), "~n~~n~~n~~n~~n~~y~CLAN INVITATION ACCEPTED BY~n~~w~~h~%s", name);
	GameTextForPlayer(fromid, string, 5000, 3);

	format(string, sizeof(string), "~n~~n~~n~~n~~n~~y~CLAN INVITATION ACCEPTED~n~~y~WELCOME TO ~w~~h~(%s)", clanData[playerClanID[playerid]][CLAN_TAG]);
	GameTextForPlayer(playerid, string, 5000, 3);

	format(string, sizeof(string), "[%s] %s", clanData[playerClanID[playerid]][CLAN_TAG], clanRankNames[playerClanID[playerid]][playerClanRank[playerid]]);
 	playerClan3DTextLabel[playerid] = CreateDynamic3DTextLabel(string, alpha(TEAMS[clanData[playerClanID[playerid]][CLAN_TEAM]][TEAM_COLOR], 150), 0.0, 0.0, 0.0, 25.0, playerid, .testlos = 1);

	format(string, sizeof(string), "~g~~h~~h~(%s) ~g~~h~%s ~w~(Rank: (%i)%s)", clanData[playerClanID[playerid]][CLAN_TAG], clanData[playerClanID[playerid]][CLAN_NAME], playerClanRank[playerid] + 1, clanRankNames[playerClanID[playerid]][playerClanRank[playerid]]);
	PlayerTextDrawSetString(playerid, clanNamePTD[playerid], string);
	PlayerTextDrawShow(playerid, clanNamePTD[playerid]);
	return 1;
}

CMD:cquit(playerid, params[]) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan.");

	new answer[16];
	if (sscanf(params, "s[16]", answer)) {
	    new string[164];
	    format(string, sizeof(string), "Clan: Are you sure you want to quit "COL_DEFAULT"%s"COL_BIEGE"?", clanData[playerClanID[playerid]][CLAN_NAME]);
	    SendClientMessage(playerid, COLOR_BIEGE, string);
	    SendClientMessage(playerid, COLOR_BIEGE, "Clan: If yes, type "COL_DEFAULT"/cquit yes");
	    return 1;
	}

	if (!strcmp(answer, "yes", true)) {
		new name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, name, sizeof(name));

		if (playerClanRank[playerid] == CLAN_RANK_OWNER) { // if he's owner
		    new query[128];
		    mysql_format(database, query, sizeof(query),
		        "SELECT name, rank FROM clan_members WHERE name != '%e' AND clan_name = '%e' ORDER BY rank DESC LIMIT "#MAX_CLAN_MEMBERS"",
                name, clanData[playerClanID[playerid]][CLAN_NAME]
			);
		    mysql_tquery(database, query, "OnOwnerLeaveClan", "ii", playerid, playerClanID[playerid]);
		}

		new string[128];
		mysql_format(database, string, sizeof(string),
		    "DELETE FROM clan_members WHERE clan_name = '%e' AND name = '%e'",
		    clanData[playerClanID[playerid]][CLAN_NAME], name
		);
		mysql_tquery(database, string);

		format(string, sizeof(string), "* Clan %s "COL_DEFAULT"%s"COL_BIEGE", has left the clan!", clanRankNames[playerClanID[playerid]][playerClanRank[playerid]], name);

	 	foreach (new i : Player) {
			if (playerClanID[i] == playerClanID[playerid]) {
		    	SendClientMessage(i, COLOR_BIEGE, string);
				PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
			}
		}

		format(string, sizeof(string), "~n~~n~~n~~n~~n~~y~YOU LEFT (%s)!", clanData[playerClanID[playerid]][CLAN_TAG]);
		GameTextForPlayer(playerid, string, 5000, 3);

		playerClanID[playerid] = -1;
		playerClanRank[playerid] = -1;

		DestroyDynamic3DTextLabel(playerClan3DTextLabel[playerid]);
		playerClan3DTextLabel[playerid] = Text3D:INVALID_STREAMER_ID;
		
		PlayerTextDrawHide(playerid, clanNamePTD[playerid]);
	}

	return 1;
}

forward OnOwnerLeaveClan(playerid, clanid);
public OnOwnerLeaveClan(playerid, clanid) {
	if (cache_num_rows() == 0) {
        new string[128];
		mysql_format(database, string, sizeof(string),
		    "DELETE FROM clans WHERE name = '%e'",
		    clanData[clanid][CLAN_NAME]
		);
		mysql_tquery(database, string);

		mysql_format(database, string, sizeof(string),
		    "DELETE FROM clan_ranks WHERE clan_name = '%e'",
		    clanData[clanid][CLAN_NAME]
		);
		mysql_tquery(database, string);

		format(string, sizeof(string), "* Clan "COL_DEFAULT"[%s] %s "COL_BIEGE"has been disbanded by %s "COL_DEFAULT"%s"COL_BIEGE".", clanData[clanid][CLAN_TAG], clanData[clanid][CLAN_NAME], clanRankNames[clanid][MAX_CLAN_RANKS - 1], clanData[clanid][CLAN_OWNER]);
		SendClientMessageToAll(COLOR_BIEGE, string);

        DestroyDynamic3DTextLabel(clanData[clanid][CLAN_3D_TEXT_LABEL]);
		DestroyDynamicPickup(clanData[clanid][CLAN_PICKUPID]);
		clanData[clanid][CLAN_3D_TEXT_LABEL] = Text3D:INVALID_STREAMER_ID;
		clanData[clanid][CLAN_PICKUPID] = INVALID_STREAMER_ID;
		clanData[clanid][CLAN_NAME][0] = EOS;
		clanData[clanid][CLAN_TAG][0] = EOS;
		clanData[clanid][CLAN_OWNER][0] = EOS;

		UpdateClansRank();
	}
	else {
	    static string[MAX_CLAN_MEMBERS * (MAX_PLAYER_NAME + MAX_CLAN_RANK_NAME + 16)];

	    // temp set the first member as owner so if player crashes, the clan isn't fucked
	    new newOwner[MAX_PLAYER_NAME];
    	cache_get_value(0, "name", newOwner, sizeof(newOwner));

  		mysql_format(database, string, sizeof(string),
		    "UPDATE clan_members SET rank = %i WHERE name = '%e' AND clan_name = '%e'",
		    CLAN_RANK_OWNER, newOwner, clanData[clanid][CLAN_NAME]
		);
		mysql_tquery(database, string);

		format(clanData[clanid][CLAN_OWNER], MAX_PLAYER_NAME, newOwner);

		new name[MAX_PLAYER_NAME];
		foreach (new i : Player) {
			GetPlayerName(i, name, sizeof(name));
			if (!strcmp(name, newOwner)) {
				playerClanRank[i] = MAX_CLAN_RANKS - 1;
				break;
			}
		}
		//

	    new rankid;

	    string = "MEMBER NAME\tRANK (highest to lowest)\n";
	    for (new i = 0, j = cache_num_rows(); i < j; i++) {
	    	cache_get_value(i, "name", name, sizeof(name));
	    	cache_get_value_int(i, "rank", rankid);

			format(string, sizeof(string), "%s%s\t%s (lvl: %i)\n", string, name, clanRankNames[clanid][rankid], rankid + 1);
		}

		Dialog_Show(playerid, clan_disband, DIALOG_STYLE_TABLIST_HEADERS, GetClanDialogHeader(clanid, "Select new clan owner"), string, "Select", "Auto-Assign");

		SetPVarInt(playerid, "DisbandClanID", clanid);
	}
}

CMD:cwithdraw(playerid, params[]) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");

	if (playerClanRank[playerid] < 2) { // rank 3 and above can use
		new string[164];
		format(string, sizeof(string), "Error: You should be clan rank 3 (%s) or above to use this command.", clanRankNames[playerClanID[playerid]][2]);
		return SendClientMessage(playerid, COLOR_TOMATO, string);
	}

	if (playerClanRank[playerid] < CLAN_RANK_LEADER && (gettime() - playerClanLastWithdrawTimestamp[playerid]) < (60 * 60)) { // withdraw can be done once every 60 mins
		new string[164];
		format(string, sizeof(string), "Error: You havve to wait %i minutes to do another withdraw from clan vault.", (gettime() - playerClanLastWithdrawTimestamp[playerid]) / 60);
		return SendClientMessage(playerid, COLOR_TOMATO, string);
	}

	if (playerClanRank[playerid] < CLAN_RANK_LEADER && clanData[playerClanID[playerid]][CLAN_VAULT_MONEY] <= 10000)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Money withdrawl is closed due to low funds in vault!");

	new money;
	if (sscanf(params, "i", money))
	    return SendClientMessage(playerid, COLOR_LIGHT_AQUA, "Usage: /cwithdraw [amount of money]");

	if (money < 1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Money cannot be negative or 0!");

	if (clanData[playerClanID[playerid]][CLAN_VAULT_MONEY] <= 10000) {
		if (money > clanData[playerClanID[playerid]][CLAN_VAULT_MONEY]) {
			new string[164];
			format(string, sizeof(string), "Error: Maximum cash you can withdraw is $%s.", FormatNumber(clanData[playerClanID[playerid]][CLAN_VAULT_MONEY]));
			return SendClientMessage(playerid, COLOR_TOMATO, string);
		}
	}
	else {
	    new limit;
	    if (playerClanRank[playerid] < CLAN_RANK_LEADER) {
			limit = (clanData[playerClanID[playerid]][CLAN_VAULT_MONEY] * ((playerClanRank[playerid] + 1) / 75)); // player can only withdraw a percentage of money as per their rank level (so if i am rank 1, i can only withdraw 1% of the vault money)
		} else {
		    limit = clanData[playerClanID[playerid]][CLAN_VAULT_MONEY];
		}

		if (money > limit) {
			new string[164];
			format(string, sizeof(string), "Error: Maximum cash you can withdraw is $%s.", FormatNumber(limit));
			return SendClientMessage(playerid, COLOR_TOMATO, string);
		}
	}

    clanData[playerClanID[playerid]][CLAN_VAULT_MONEY] -= money;
    playerClanLastWithdrawTimestamp[playerid] = gettime();
	GivePlayerMoney(playerid, money);

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new string[164];
	format(string, sizeof(string),
		"* %s "COL_DEFAULT"%s "COL_BIEGE"have withdrawn "COL_DEFAULT"$%s"COL_BIEGE" from clan vault, new balance: "COL_DEFAULT"$%s"COL_BIEGE"!",
		clanRankNames[playerClanID[playerid]][playerClanRank[playerid]], name, FormatNumber(money), FormatNumber(clanData[playerClanID[playerid]][CLAN_VAULT_MONEY])
	);

 	foreach (new i : Player) {
		if (playerClanID[i] == playerClanID[playerid]) {
	    	SendClientMessage(i, COLOR_BIEGE, string);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	return 1;
}

CMD:cdeposit(playerid, params[]) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");

	if (playerClanRank[playerid] < 2) { // rank 3 and above can use
		new string[164];
		format(string, sizeof(string), "Error: You should be clan rank 3 (%s) or above to use this command.", clanRankNames[playerClanID[playerid]][2]);
		return SendClientMessage(playerid, COLOR_TOMATO, string);
	}

	new money;
	if (sscanf(params, "i", money))
	    return SendClientMessage(playerid, COLOR_LIGHT_AQUA, "Usage: /cdeposit [amount of money]");

	if (money < 1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Money cannot be negative or 0!");

	if (money > GetPlayerMoney(playerid))
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You don't have enough money.");

    clanData[playerClanID[playerid]][CLAN_VAULT_MONEY] += money;
    GivePlayerMoney(playerid, -money);

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new string[164];
	format(string, sizeof(string),
		"* %s "COL_DEFAULT"%s "COL_BIEGE"have deposited "COL_DEFAULT"$%s"COL_BIEGE" in clan vault, new balance: "COL_DEFAULT"$%s"COL_BIEGE"!",
		clanRankNames[playerClanID[playerid]][playerClanRank[playerid]], name, FormatNumber(money), FormatNumber(clanData[playerClanID[playerid]][CLAN_VAULT_MONEY])
	);

 	foreach (new i : Player) {
		if (playerClanID[i] == playerClanID[playerid]) {
	    	SendClientMessage(i, COLOR_BIEGE, string);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	return 1;
}

CMD:cranks(playerid) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");

	new clanid = playerClanID[playerid];

	new count[MAX_CLAN_RANKS];
	foreach (new i : Player) {
		if (playerClanID[i] == clanid) {
			++count[playerClanRank[playerid]];
		}
	}

	new string[MAX_CLAN_RANKS * (MAX_CLAN_RANK_NAME + 32)] =
	    "Rank Level\tRank Name\tMembers Under This Rank\n"
	;

	for (new i = 0; i < MAX_CLAN_RANKS; i++) {
	    if (i < CLAN_RANK_LEADER) {
			format(string, sizeof(string),
			    "%s\
				%i.\t\
			    %s\t\
			    %i players\n\
			    ",
			    string,
				i + 1,
				clanRankNames[clanid][i],
				count[i]
			);
		}
		else {
			format(string, sizeof(string),
			    "%s\
				"COL_TOMATO"%i.\t\
			    "COL_TOMATO"%s\t\
			    "COL_TOMATO"%i players\n\
			    ",
			    string,
				i + 1,
				clanRankNames[clanid][i],
				count[i]
			);
		}
	}

	if (playerClanRank[playerid] < CLAN_RANK_LEADER) {
		Dialog_Show(playerid, 0, DIALOG_STYLE_TABLIST_HEADERS, GetClanDialogHeader(playerClanID[playerid], "Ranks list"), string, "CLOSE", "");
	} else {
		Dialog_Show(playerid, clan_ranks, DIALOG_STYLE_TABLIST_HEADERS, GetClanDialogHeader(playerClanID[playerid], "Ranks list"), string, "Modify", "Close");
	}

	return 1;
}
CMD:clanranks(playerid) return cmd_cranks(playerid);

CMD:ctoggleweapons(playerid, params[]) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");
	    
 	if (playerClanWeaponsEnabled[playerid]) {
 	    playerClanWeaponsEnabled[playerid] = false;
	    SendClientMessage(playerid, COLOR_BIEGE, "Clan: You will no longer receive clan weapons on spawn!");
 	} else {
 	    playerClanWeaponsEnabled[playerid] = true;
	    SendClientMessage(playerid, COLOR_BIEGE, "Clan: You will no longer receive clan weapons on spawn!");
	}
	
    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}

CMD:cbuyskin(playerid) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");

	if (playerClanRank[playerid] < CLAN_RANK_LEADER) // leader or owner only use this command
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You should be clan leader/owner to use this command.");

	const MAX_SKINS = 312;
    static string[MAX_SKINS * 32];

    if (string[0] == EOS) {
        for (new i = 0; i < MAX_SKINS; i++) {
            format(string, sizeof(string), "%s%i\tID: %i~n~~r~~h~~h~Price: $%s\n", string, i, i, FormatNumber(15000000));
        }
    }

	for (new i = 0; i < sizeof(CLAN_WEAPONS); i++) {
		SetDialogPreviewRotation(playerid, i, 0.0, 0.0, 0.0, 1.0);
	}

	return Dialog_Show(playerid, clan_skins, DIALOG_STYLE_PREVIEW_MODEL, "Select clan skin", string, "Select", "Close");
}

CMD:cbuyweapon(playerid) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");

	if (playerClanRank[playerid] < CLAN_RANK_LEADER) // leader or owner only use this command
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You should be clan leader/owner to use this command.");

    static string[sizeof(CLAN_WEAPONS) * 64];
    if (string[0] == EOS) {
        for (new i = 0; i < sizeof(CLAN_WEAPONS); i++) {
       		format(string, sizeof string, "%s%i\t%s~n~~r~~h~~h~Price: $%s\n", string, CLAN_WEAPONS[i][CLAN_WEAPON_MODEL], CLAN_WEAPONS[i][CLAN_WEAPON_NAME], FormatNumber(CLAN_WEAPONS[i][CLAN_WEAPON_COST]));
       	}
    }

	for (new i = 0; i < sizeof(CLAN_WEAPONS); i++) {
		SetDialogPreviewRotation(playerid, i, 0.0, 0.0, -50.0, 1.5);
	}

	return Dialog_Show(playerid, clan_weapons, DIALOG_STYLE_PREVIEW_MODEL, "Select weapon for clan vault", string, "Select", "Close");
}

CMD:csetteam(playerid) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");

	if (playerClanRank[playerid] < CLAN_RANK_LEADER) // leader or owner only use this command
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You should be clan leader/owner to use this command.");

	new count[sizeof(TEAMS)];
	foreach_clans(i) {
		++count[clanData[i][CLAN_TEAM]];
	}

	new string[sizeof(TEAMS) * (MAX_TEAM_NAME + 32) + 64] =
	    "Team Name\tNumber of clans using this team\n"
	;

	for (new i = 0; i < sizeof(TEAMS); i++) {
		format(string, sizeof(string),
		    "%s{%06x}%s\t%i clans\n",
		    string, TEAMS[i][TEAM_COLOR] >>> 8, TEAMS[i][TEAM_NAME], count[i]
		);
	}

	return Dialog_Show(playerid, clan_teams, DIALOG_STYLE_TABLIST_HEADERS, GetClanDialogHeader(playerClanID[playerid], "Select clan team"), string, "Select", "Close");
}

CMD:csetspawn(playerid, params[]) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");

	if (playerClanRank[playerid] < CLAN_RANK_LEADER) // leader or owner only use this command
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You should be clan leader/owner to use this command.");

	if (clanData[playerClanID[playerid]][CLAN_TOTAL_EXP] < 500)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Your clan needs atleast 500 EXP to change clan spawn.");

	return Dialog_Show(playerid, clan_spawn, DIALOG_STYLE_MSGBOX, GetClanDialogHeader(playerClanID[playerid], "Set clan spawn"), COL_WHITE "Are you sure you want to set this as clan spawn?\n"COL_DEFAULT"This will cost clan vault "COL_TOMATO"-$10,000,000 "COL_DEFAULT"for setting a new spawn...", "Yes", "No");
}

CMD:cremovespawn(playerid, params[]) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");

	if (playerClanRank[playerid] < CLAN_RANK_LEADER) // leader or owner only use this command
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You should be clan leader/owner to use this command.");

	if (clanData[playerClanID[playerid]][CLAN_SPAWN_POS][0] == 0.0 && clanData[playerClanID[playerid]][CLAN_SPAWN_POS][1] == 0.0 && clanData[playerClanID[playerid]][CLAN_SPAWN_POS][2] == 0.0)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You don't have a custom clan spawn.");

	return Dialog_Show(playerid, clan_remove_spawn, DIALOG_STYLE_MSGBOX, GetClanDialogHeader(playerClanID[playerid], "Remove clan spawn"), COL_WHITE "Are you sure you want to remove your clan spawn?", "Yes", "No");
}

CMD:cpromote(playerid, params[]) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");

	if (playerClanRank[playerid] < CLAN_RANK_LEADER) // leader or owner only use this command
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You should be clan leader/owner to use this command.");

	new targetid;
	if (sscanf(params, "u", targetid))
	    return SendClientMessage(playerid, COLOR_LIGHT_AQUA, "Usage: /cpromote [id/name]");

	if (targetid == playerid)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You don't need promotion, you already have highest possible level!");

	if (!IsPlayerConnected(targetid))
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: The player isn't connected.");

	if (playerClanID[targetid] != playerClanID[playerid])
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: The player isn't in your clan.");

	if (playerClanRank[targetid] > playerClanRank[playerid])
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Can't use this command on high ranks.");

	SetPVarInt(playerid, "PromotionTargetPlayer", targetid);

	new string[MAX_CLAN_RANKS * (MAX_CLAN_RANK_NAME + 4)];

    for (new i = 0; i < CLAN_RANK_OWNER; i++) { // exclude "owner" rank
        if (i == CLAN_RANK_LEADER) { // leader
			format(string, sizeof(string), "%s"COL_TOMATO"lvl: "COL_DEFAULT"%i\t"COL_TOMATO"%s\n", string, (i + 1), clanRankNames[playerClanID[playerid]][i]);
		} else {
			format(string, sizeof(string), "%slvl: "COL_DEFAULT"%i\t%s\n", string, (i + 1), clanRankNames[playerClanID[playerid]][i]);
		}
	}

	return Dialog_Show(playerid, select_promotion, DIALOG_STYLE_TABLIST, GetClanDialogHeader(playerClanID[playerid], "Set player new rank"), string, "Select", "Cancel");
}

CMD:ckick(playerid, params[]) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");

	if (playerClanRank[playerid] < CLAN_RANK_LEADER) // leader or owner only use this command
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You should be clan leader/owner to use this command.");

	new targetid;
	if (sscanf(params, "u", targetid))
	    return SendClientMessage(playerid, COLOR_LIGHT_AQUA, "Usage: /ckick [id/name]");

	if (targetid == playerid)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You don't need to kick yourself, just /cquit to leave clan!");

	if (!IsPlayerConnected(targetid))
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: The player isn't connected.");

	if (playerClanID[targetid] != playerClanID[playerid])
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: The player isn't in your clan.");

	if (playerClanRank[targetid] > playerClanRank[playerid])
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Can't use this command on high ranks.");

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

    new playerName[MAX_PLAYER_NAME];
	GetPlayerName(targetid, playerName, sizeof(playerName));

	new string[164];
	mysql_format(database, string, sizeof(string),
		"DELETE FROM clan_members WHERE name = '%e'",
		name
	);
	mysql_tquery(database, string);

	format(string, sizeof(string), "* "COL_DEFAULT"%s "COL_BIEGE"has been kicked from clan by %s "COL_DEFAULT"%s"COL_BIEGE"!", playerName, clanRankNames[playerClanID[playerid]][playerClanRank[playerid]], name);

 	foreach (new i : Player) {
		if (playerClanID[i] == playerClanID[playerid]) {
	    	SendClientMessage(i, COLOR_BIEGE, string);
		}
	}

	playerClanID[playerid] = -1;
	playerClanRank[playerid] = -1;

	return GameTextForPlayer(targetid, "~n~~n~~n~~n~~n~~r~~h~KICKED OUT FROM CLAN!", 5000, 3);
}

CMD:cchangename(playerid, params[]) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");

	if (playerClanRank[playerid] < CLAN_RANK_OWNER) // owner only use this command
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You should be clan owner to use this command.");

	if (clanData[playerClanID[playerid]][CLAN_TOTAL_EXP] < 500) // price to change clan's name is -500 EXP
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Your clan needs atleast 500 EXP to change clan name.");

	new newName[MAX_CLAN_NAME];
	if (sscanf(params, "s["#MAX_CLAN_NAME"]", newName))
	    return SendClientMessage(playerid, COLOR_LIGHT_AQUA, "Usage: /cchangename [new-name]");

	foreach_clans(i) {
	    if (i != playerClanID[playerid] && !strcmp(clanData[i][CLAN_NAME], newName, true)) {
			return SendClientMessage(playerid, COLOR_TOMATO, "Error: The clan name already exist, try another one.");
		}
	}
	
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new string[164];
	format(string, sizeof(string), "* Clan name has been changed to "COL_DEFAULT"%s"COL_BIEGE", by %s "COL_DEFAULT"%s", newName, clanRankNames[playerClanID[playerid]][playerClanRank[playerid]], name);

 	foreach (new i : Player) {
		if (playerClanID[i] == playerClanID[playerid]) {
	    	SendClientMessage(i, COLOR_BIEGE, string);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	format(clanData[playerClanID[playerid]][CLAN_NAME], MAX_CLAN_NAME, newName);
	return 1;
}

CMD:cchangetag(playerid, params[]) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");

	if (playerClanRank[playerid] < CLAN_RANK_OWNER) // owner only use this command
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You should be clan owner to use this command.");

	if (clanData[playerClanID[playerid]][CLAN_TOTAL_EXP] < 500) // price to change clan's name is -500 EXP
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Your clan needs atleast 500 EXP to change clan name.");

	new newTag[MAX_CLAN_TAG_NAME];
	if (sscanf(params, "s["#MAX_CLAN_TAG_NAME"]", newTag))
	    return SendClientMessage(playerid, COLOR_LIGHT_AQUA, "Usage: /cchangetag [new-tag]");

	foreach_clans(i) {
	    if (i != playerClanID[playerid] && !strcmp(clanData[i][CLAN_TAG], newTag, true)) {
			return SendClientMessage(playerid, COLOR_TOMATO, "Error: The clan tag already exist, try another one.");
		}
	}
	
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new string[164];
	format(string, sizeof(string), "* Clan tag has been changed to "COL_DEFAULT"%s"COL_BIEGE", by %s "COL_DEFAULT"%s", newTag, clanRankNames[playerClanID[playerid]][playerClanRank[playerid]], name);

 	foreach (new i : Player) {
		if (playerClanID[i] == playerClanID[playerid]) {
	    	SendClientMessage(i, COLOR_BIEGE, string);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	format(clanData[playerClanID[playerid]][CLAN_TAG], MAX_CLAN_TAG_NAME, newTag);
	return 1;
}

CMD:csavestats(playerid) {
	if (playerClanID[playerid] == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You are not in any clan, therefore can't use this command.");

	if (playerClanRank[playerid] < CLAN_RANK_OWNER) // owner only use this command
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You should be clan owner to use this command.");

	new clanid = playerClanID[playerid];
    foreach(new i : Player) {
	    if (playerClanID[i] != clanid) {
            UpdatePlayerClanData(i);
	    }
	}

	UpdateClanData(clanid);

	new string[164];
	format(string, sizeof(string), ""COL_DEFAULT"[%s] "COL_GREEN"clan stats has been saved.", clanData[clanid][CLAN_TAG]);
	SendClientMessage(playerid, COLOR_GREEN, string);
	
    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}

CMD:ccreate(playerid, params[]) {
	if (!IsPlayerAdmin(playerid))
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Only admin level 5 and above can access to this command.");

	new clanName[MAX_CLAN_NAME], clanTag[MAX_CLAN_TAG_NAME], ownerid;
	if (sscanf(params, "s["#MAX_CLAN_NAME"]s["#MAX_CLAN_TAG_NAME"]u", clanName, clanTag, ownerid)) {
	    SendClientMessage(playerid, COLOR_LIGHT_AQUA, "Usage: /ccreate [name] [tag] [owner]");
	    SendClientMessage(playerid, COLOR_LIGHT_AQUA, "Usage: To add spaces in [name], you can use '_' and it will be auto detected as a white space!");
	    return 1;
	}

	foreach_clans(i) {
	    if (!strcmp(clanData[i][CLAN_NAME], clanName, true)) {
			return SendClientMessage(playerid, COLOR_TOMATO, "Error: The clan name already exist, try another one.");
		}
		
	    if (!strcmp(clanData[i][CLAN_TAG], clanTag, true)) {
			return SendClientMessage(playerid, COLOR_TOMATO, "Error: The clan tag already exist, try another one.");
		}
	}
	
	if (!IsPlayerConnected(ownerid))
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: The owner player isn't connected.");

	if (playerClanID[ownerid] != -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: The owner player is in a clan.");

	new index = -1;
	for (new i = 0; i < MAX_CLANS; i++) {
	    if (clanData[i][CLAN_NAME][0] == EOS) {
			index = i;
			break;
		}
	}
	
	if (index == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Looks like the server clans creation limit has been reached, please contact server developer about this.");

	for (new i = 0; clanName[i] != EOS; i++) {
	    if (clanName[i] == '_') {
            clanName[i] = ' ';
		}
	}

	new pos = strfind(clanTag, "[");
	if (pos != -1)
		strdel(clanTag, pos, pos + 1);

	pos = strfind(clanTag, "]");
	if (pos != -1)
		strdel(clanTag, pos, pos + 1);

 	format(clanData[index][CLAN_TAG], MAX_CLAN_TAG_NAME, clanTag);
 	format(clanData[index][CLAN_NAME], MAX_CLAN_NAME, clanName);
	clanData[index][CLAN_TEAM] = random(sizeof(TEAMS));
	clanData[index][CLAN_SKIN] = TEAMS[clanData[index][CLAN_TEAM]][TEAM_SKIN];
 	GetPlayerName(ownerid, clanData[index][CLAN_OWNER], MAX_PLAYER_NAME);
	clanData[index][CLAN_WAR_WINS] = 0;
	clanData[index][CLAN_WAR_TOTAL] = 0;
	clanData[index][CLAN_SPAWN_POS][0] = 0.0;
	clanData[index][CLAN_SPAWN_POS][1] = 0.0;
	clanData[index][CLAN_SPAWN_POS][2] = 0.0;
	clanData[index][CLAN_SPAWN_POS][3] = 0.0;
	clanData[index][CLAN_SPAWN_INTERIORID] = 0;
	clanData[index][CLAN_SPAWN_WORLDID] = 0;
	clanData[index][CLAN_VAULT_MONEY] = 0;
	for (new i = 0; i < MAX_CLAN_WEAPONS; i++) {
		clanData[index][CLAN_VAULT_WEAPONS][i] = -1;
		clanData[index][CLAN_VAULT_WEAPONS_TIMESTAMP][i] = -1;
	}
	clanData[index][CLAN_TOTAL_EXP] = 0;
	clanData[index][CLAN_3D_TEXT_LABEL] = Text3D:INVALID_STREAMER_ID;
	clanData[index][CLAN_PICKUPID] = INVALID_STREAMER_ID;

	playerClanID[ownerid] = index;
	playerClanRank[ownerid] = CLAN_RANK_OWNER;

	new vault_weapons[32];
	for (new i = 0; i < MAX_CLAN_WEAPONS; i++) {
		format(vault_weapons, sizeof(vault_weapons), "%s-1", vault_weapons);

		if (i != (MAX_CLAN_WEAPONS - 1)) {
			strcat(vault_weapons, " ");
		}
	}

	new query[512];
	mysql_format(database, query, sizeof(query),
	    "INSERT INTO clans (\
	        tag, name, skin, exp, team, clanwar_wins, clanwar_total, \
			spawn_x, spawn_y, spawn_z, spawn_angle, spawn_interiorid, spawn_worldid, \
			vault_money, vault_weapons, vault_weapons_timestamp\
		) VALUES (\
		    '%e', '%e', %i, 0, %i, 0, 0, \
		    0.0, 0.0, 0.0, 0.0, 0, 0, \
		    0, '%s', '%s'\
		)",
        clanTag, clanName, clanData[index][CLAN_SKIN], clanData[index][CLAN_TEAM],
        vault_weapons, vault_weapons
	);
	mysql_tquery(database, query);

    mysql_format(database, query, sizeof(query),
        "INSERT INTO clan_members (\
            clan_name, name, rank\
		) VALUES (\
			'%e', '%e', %i\
		)",
		clanName, clanData[index][CLAN_OWNER], CLAN_RANK_OWNER
	);
	mysql_tquery(database, query);

	for (new i = 0; i < MAX_CLAN_RANKS; i++) {
	    format(clanRankNames[index][i], MAX_CLAN_RANK_NAME, DEFAULT_CLAN_RANKS[i]);

	    mysql_format(database, query, sizeof(query),
	        "INSERT INTO clan_ranks (\
	            clan_name, name, level\
			) VALUES (\
				'%e', '%e', %i\
			)",
			clanName, DEFAULT_CLAN_RANKS[i], i
		);
		mysql_tquery(database, query);
	}
	
    mysql_format(database, query, sizeof(query),
		"SELECT id FROM clans WHERE clan_name = '%e'",
        clanName
	);
 	new Cache:cache = mysql_query(database, query);
 	cache_get_value_int(0, "id", clanData[index][CLAN_SQLID]);
	cache_delete(cache);
	
    UpdateClansRank();

 	new string[164];
	format(string, sizeof(string), "[%s] %s", clanData[index][CLAN_TAG], clanRankNames[index][MAX_CLAN_RANKS - 1]);
 	playerClan3DTextLabel[ownerid] = CreateDynamic3DTextLabel(string, alpha(TEAMS[clanData[index][CLAN_TEAM]][TEAM_COLOR], 150), 0.0, 0.0, 0.0, 25.0, ownerid, .testlos = 1);

    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));

	SendClientMessage(ownerid, COLOR_GREEN, "_________________________________________");
	SendClientMessage(ownerid, COLOR_GREEN, "");
	SendClientMessage(ownerid, COLOR_GREEN, "                           CLAN OWNERSHIP");
 	format(string, sizeof(string), "Admin "COL_DEFAULT"%s"COL_GREEN", have assgined you as owner of new clan: "COL_DEFAULT"%s", name, clanData[index][CLAN_NAME]);
	SendClientMessage(ownerid, COLOR_GREEN, string);
	SendClientMessage(ownerid, COLOR_GREEN, "Here are some tips to get a head start with your clan:");
	SendClientMessage(ownerid, COLOR_GREEN, "To start recruiting players to your clan, use "COL_DEFAULT"/cinvite");
	SendClientMessage(ownerid, COLOR_GREEN, "For list of clan commands that you have access to all of them, type "COL_DEFAULT"/chelp");
	SendClientMessage(ownerid, COLOR_GREEN, "_________________________________________");

 	format(string, sizeof(string), "Admin: You have suceessfully assigned a new clan, "COL_DEFAULT"%s "COL_BLUE"to player "COL_DEFAULT"%s", clanData[index][CLAN_NAME], clanData[index][CLAN_OWNER]);
	SendClientMessage(playerid, COLOR_BLUE, string);
	
    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
    PlayerPlaySound(ownerid, 1057, 0.0, 0.0, 0.0);

	format(string, sizeof(string), "(%s)", clanData[index][CLAN_TAG]);
	TextDrawSetString(clanAnnouncementTD[1], string);
	format(string, sizeof(string), "~w~Clan ~r~~h~%s ~w~is now recruiting new memebers. To request an invite, contact ~r~~h~%s ~w~(id: ~r~%i~w~)", clanData[index][CLAN_NAME], clanData[index][CLAN_OWNER], ownerid);
	TextDrawSetString(clanAnnouncementTD[2], string);

	foreach (new i : Player) {
	    if (GetPlayerState(i) != PLAYER_STATE_WASTED || GetPlayerState(i) != PLAYER_STATE_SPECTATING) {
			for (new x = 0; x < sizeof(clanAnnouncementTD); x++) {
				TextDrawShowForPlayer(i, clanAnnouncementTD[x]);
			}
		}
	}

	clanAnnouncementTimer = SetTimer("OnAnnouncementExpire", (30 * 1000), false);
	
	format(string, sizeof(string), "[%s] %s", clanData[index][CLAN_TAG], clanRankNames[index][CLAN_RANK_OWNER]);
 	playerClan3DTextLabel[playerid] = CreateDynamic3DTextLabel(string, alpha(TEAMS[clanData[index][CLAN_TEAM]][TEAM_COLOR], 150), 0.0, 0.0, 0.0, 25.0, playerid, .testlos = 1);

	format(string, sizeof(string), "~g~~h~~h~(%s) ~g~~h~%s ~w~(Rank: (%i)%s)", clanData[index][CLAN_TAG], clanData[index][CLAN_NAME], CLAN_RANK_OWNER + 1, clanRankNames[index][CLAN_RANK_OWNER]);
	PlayerTextDrawSetString(playerid, clanNamePTD[playerid], string);
	PlayerTextDrawShow(playerid, clanNamePTD[playerid]);

	return 1;
}

forward OnAnnouncementExpire();
public OnAnnouncementExpire() {
    for (new i = 0; i < sizeof(clanAnnouncementTD); i++) {
		TextDrawHideForAll(clanAnnouncementTD[i]);
	}
}

CMD:cdelete(playerid, params[]) {
	if (!IsPlayerAdmin(playerid))
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Only admin level 5 and above can access to this command.");

	new clanid;
	if (sscanf(params, "k<clan>", clanid))
	    return SendClientMessage(playerid, COLOR_LIGHT_AQUA, "Usage: /cdelete [name/tag/id]");

	if (clanid == -1)
		return SendClientMessage(playerid, COLOR_TOMATO, "Error: Clan wasn't found, try /clans for a full list.");

    new string[128];
	mysql_format(database, string, sizeof(string),
	    "DELETE FROM clans WHERE name = '%e'",
	    clanData[clanid][CLAN_NAME]
	);
	mysql_tquery(database, string);

	mysql_format(database, string, sizeof(string),
	    "DELETE FROM clan_members WHERE clan_name = '%e'",
	    clanData[clanid][CLAN_NAME]
	);
	mysql_tquery(database, string);

	mysql_format(database, string, sizeof(string),
	    "DELETE FROM clan_ranks WHERE clan_name = '%e'",
	    clanData[clanid][CLAN_NAME]
	);
	mysql_tquery(database, string);

	new adminname[MAX_PLAYER_NAME];
	GetPlayerName(playerid, adminname, sizeof(adminname));

	format(string, sizeof(string), "* Clan "COL_DEFAULT"[%s] %s "COL_BLUE"has been disbanded by Admin "COL_DEFAULT"%s"COL_BLUE".", clanData[clanid][CLAN_TAG], clanData[clanid][CLAN_NAME], adminname);
	SendClientMessageToAll(COLOR_BLUE, string);

	format(string, sizeof(string), "* Admin "COL_DEFAULT"%s "COL_BLUE"has disband your clan", adminname);

	foreach (new i : Player) {
		if (playerClanID[i] == clanid) {
            SendClientMessage(i, COLOR_BLUE, string);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);

			playerClanID[i] = -1;
			playerClanRank[i] = -1;

			DestroyDynamic3DTextLabel(playerClan3DTextLabel[i]);
			playerClan3DTextLabel[i] = Text3D:INVALID_STREAMER_ID;
			PlayerTextDrawHide(i, clanNamePTD[i]);
		}
	}

    DestroyDynamic3DTextLabel(clanData[clanid][CLAN_3D_TEXT_LABEL]);
	DestroyDynamicPickup(clanData[clanid][CLAN_PICKUPID]);
	clanData[clanid][CLAN_3D_TEXT_LABEL] = Text3D:INVALID_STREAMER_ID;
	clanData[clanid][CLAN_PICKUPID] = INVALID_STREAMER_ID;
	clanData[clanid][CLAN_NAME][0] = EOS;
	clanData[clanid][CLAN_TAG][0] = EOS;
	clanData[clanid][CLAN_OWNER][0] = EOS;

	UpdateClansRank();
	return 1;
}

CMD:cgivemoney(playerid, params[]) {
	if (!IsPlayerAdmin(playerid))
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Only admin level 5 and above can access to this command.");

	new clanid, money;
	if (sscanf(params, "k<clan>i", clanid, money))
	    return SendClientMessage(playerid, COLOR_LIGHT_AQUA, "Usage: /csetmoney [name/tag/id] [money]");

	if (clanid == -1)
		return SendClientMessage(playerid, COLOR_TOMATO, "Error: Clan wasn't found, try /clans for a full list.");

	if (money < 100 || money > 10000000)
		return SendClientMessage(playerid, COLOR_TOMATO, "Error: The amount of money should be between $100 - $10m.");

    new string[128];

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	format(string, sizeof(string), "* Clan "COL_DEFAULT"[%s] %s "COL_BLUE"has received "COL_GREEN"$%s"COL_BLUE" from Admin "COL_DEFAULT"%s"COL_BLUE"", clanData[clanid][CLAN_TAG], clanData[clanid][CLAN_NAME], FormatNumber(money), name);
	SendClientMessageToAll(COLOR_BLUE, string);

	format(string, sizeof(string), "Admin "COL_DEFAULT"%s "COL_BLUE"has given your clan "COL_GREEN"$%s"COL_BLUE"(vault money).", name, FormatNumber(money));
	foreach (new i : Player) {
		if (playerClanID[i] == clanid) {
            SendClientMessage(i, COLOR_BLUE, string);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	clanData[clanid][CLAN_VAULT_MONEY] += money;
	
    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}

CMD:cgiveexp(playerid, params[]) {
	if (!IsPlayerAdmin(playerid))
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Only admin level 5 and above can access to this command.");

	new clanid, exp;
	if (sscanf(params, "k<clan>i", clanid, exp))
	    return SendClientMessage(playerid, COLOR_LIGHT_AQUA, "Usage: /cgiveexp [name/tag/id] [exp]");

	if (clanid == -1)
		return SendClientMessage(playerid, COLOR_TOMATO, "Error: Clan wasn't found, try /clans for a full list.");

	if (exp < 1 || exp > 1000)
		return SendClientMessage(playerid, COLOR_TOMATO, "Error: The amount of exp should be between 1 - 1000 points.");

    new string[128];

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	format(string, sizeof(string), "* Clan "COL_DEFAULT"[%s] %s "COL_BLUE"has received "COL_GREEN"%s EXP"COL_BLUE" from Admin "COL_DEFAULT"%s"COL_BLUE"", clanData[clanid][CLAN_TAG], clanData[clanid][CLAN_NAME], FormatNumber(exp), name);
	SendClientMessageToAll(COLOR_BLUE, string);

	format(string, sizeof(string), "Admin "COL_DEFAULT"%s "COL_BLUE"has given your clan "COL_GREEN"%s EXP"COL_BLUE".", name, FormatNumber(exp));
	foreach (new i : Player) {
		if (playerClanID[i] == clanid) {
            SendClientMessage(i, COLOR_BLUE, string);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	clanData[clanid][CLAN_TOTAL_EXP] += exp;
	
    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}
// ---------------------------------------

// ---------------------------------------
// DIALOGS
Dialog:clan_disband(playerid, response, listitem, inputtext[]) {
    new clanid = GetPVarInt(playerid, "DisbandClanID");
    DeletePVar(playerid, "DisbandClanID");

	if (!response) {
	    new previousOwner[MAX_PLAYER_NAME];
	    GetPlayerName(playerid, previousOwner, sizeof(previousOwner));

		new string[150];
		format(string, sizeof(string), "* Clan %s of [%s] %s, "COL_DEFAULT"%s "COL_LIGHT_AQUA"has promoted "COL_DEFAULT"%s"COL_LIGHT_AQUA", and left the clan!", clanRankNames[clanid][MAX_CLAN_RANKS - 1], clanData[clanid][CLAN_TAG], clanData[clanid][CLAN_NAME], previousOwner, clanData[clanid][CLAN_OWNER]);
		SendClientMessageToAll(COLOR_LIGHT_AQUA, string);
	}
	else {
	    new previousOwner[MAX_PLAYER_NAME];
	    GetPlayerName(playerid, previousOwner, sizeof(previousOwner));

		new newOwner[MAX_PLAYER_NAME];
		format(newOwner, sizeof(newOwner), inputtext);

		new name[MAX_PLAYER_NAME];
		foreach (new i : Player) {
			GetPlayerName(i, name, sizeof(name));
			if (!strcmp(name, newOwner)) {
				playerClanRank[i] = MAX_CLAN_RANKS - 1;
				break;
			}
		}

		new string[150];
  		mysql_format(database, string, sizeof(string),
		    "UPDATE clan_members SET rank = %i WHERE name = '%e' AND clan_name = '%e'",
		    CLAN_RANK_OWNER, newOwner, clanData[clanid][CLAN_NAME]
		);
		mysql_tquery(database, string);

		format(clanData[clanid][CLAN_OWNER], MAX_PLAYER_NAME, newOwner);

		format(string, sizeof(string), "* Clan %s of [%s] %s, "COL_DEFAULT"%s "COL_LIGHT_AQUA"has promoted "COL_DEFAULT"%s"COL_LIGHT_AQUA", and left the clan!", clanRankNames[clanid][MAX_CLAN_RANKS - 1], clanData[clanid][CLAN_TAG], clanData[clanid][CLAN_NAME], previousOwner, newOwner);
		SendClientMessageToAll(COLOR_LIGHT_AQUA, string);
	}

	return 1;
}

Dialog:clan_skins(playerid, response, listitem, inputtext[]) {
	if (!response)
	    return 1;

	for (new i = 0; i < sizeof(TEAMS); i++) {
		if (i != clanData[playerClanID[playerid]][CLAN_TEAM]) {
		    if (TEAMS[i][TEAM_SKIN] == listitem) {
				return SendClientMessage(playerid, COLOR_TOMATO, "Error: The skin selected is used for a team and you cannot purchase a team skin!");
			}
		}
	}

	foreach_clans(i) {
		if (clanData[i][CLAN_SKIN] == listitem) {
			return SendClientMessage(playerid, COLOR_TOMATO, "Error: The skin selected is used by another clan and you cannot use the same skin!");
        }
	}

	if (clanData[playerClanID[playerid]][CLAN_TOTAL_EXP] < 500) // price to change clan's skin is -500 EXP
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Your clan needs atleast 500 EXP to change clan skin.");

	if (clanData[playerClanID[playerid]][CLAN_VAULT_MONEY] < 15000000) // 15 million to buy new skin
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Your clan needs atleast $15,000,000 in vault to buy new clan skin.");

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new string[164];
	format(string, sizeof(string), "* Clan skin has been changed to id: "COL_DEFAULT"%i "COL_BIEGE"worth "COL_TOMATO"-$%s"COL_BIEGE", by %s "COL_DEFAULT"%s", listitem, FormatNumber(15000000), clanRankNames[playerClanID[playerid]][playerClanRank[playerid]], name);

 	foreach (new i : Player) {
		if (playerClanID[i] == playerClanID[playerid]) {
	    	SendClientMessage(i, COLOR_BIEGE, string);
	    	SendClientMessage(i, COLOR_BIEGE, "* Your skin will be automatically changed if you are in the right assigned clan team");

			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);

			if (GetPlayerTeam(i) == clanData[playerClanID[playerid]][CLAN_TEAM]) {
				SetPlayerSkin(i, listitem);
			}
		}
	}

	clanData[playerClanID[playerid]][CLAN_SKIN] = listitem;
	clanData[playerClanID[playerid]][CLAN_VAULT_MONEY] -= 15000000;
	return 1;
}

Dialog:clan_weapons(playerid, response, listitem, inputtext[]) {
	if (!response)
	    return 1;

	new index = -1;
	for (new i = 0; i < MAX_CLAN_WEAPONS; i++) {
        if (clanData[playerClanID[playerid]][CLAN_VAULT_WEAPONS][i] == -1) {
            index = i;
            break;
        }
	}

	if (index == -1)
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: You cannot buy more weapons, your weapons vault is out of space.");

	if (clanData[playerClanID[playerid]][CLAN_TOTAL_EXP] < 500) // price to change clan's skin is -500 EXP
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Your clan needs atleast 500 EXP to buy clan vault weapon.");

	if (clanData[playerClanID[playerid]][CLAN_VAULT_MONEY] < CLAN_WEAPONS[listitem][CLAN_WEAPON_COST]) {
		new string[164];
		format(string, sizeof(string), "Error: Your clan needs atleast $%s in vault to buy %s.", FormatNumber(CLAN_WEAPONS[listitem][CLAN_WEAPON_COST]), CLAN_WEAPONS[listitem][CLAN_WEAPON_NAME]);
	    return SendClientMessage(playerid, COLOR_TOMATO, string);
	}

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new string[164];
	format(string, sizeof(string), "* Great news! "COL_DEFAULT"%s "COL_BIEGE"has bought new inventory weapon: "COL_DEFAULT"%s"COL_BIEGE", worth "COL_TOMATO"-$%s"COL_BIEGE"!", name, CLAN_WEAPONS[listitem][CLAN_WEAPON_NAME], FormatNumber(CLAN_WEAPONS[listitem][CLAN_WEAPON_COST]));

 	foreach (new i : Player) {
		if (playerClanID[i] == playerClanID[playerid]) {
	    	SendClientMessage(i, COLOR_BIEGE, string);
	    	SendClientMessage(i, COLOR_BIEGE, "* The weapon will be avialable for every clan member on spawn, but for only "COL_DEFAULT""#CLAN_WEAPON_EXPIRE_INTERVAL" days"COL_BIEGE"!");

			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	clanData[playerClanID[playerid]][CLAN_VAULT_WEAPONS][index] = listitem;
	clanData[playerClanID[playerid]][CLAN_VAULT_WEAPONS_TIMESTAMP][index] = gettime();
	clanData[playerClanID[playerid]][CLAN_VAULT_MONEY] -= CLAN_WEAPONS[listitem][CLAN_WEAPON_COST];
	return 1;
}

Dialog:clan_teams(playerid, response, listitem, inputtext[]) {
	if (!response)
		return 1;

	if (clanData[playerClanID[playerid]][CLAN_TEAM] == listitem) {
		new string[164];
		format(string, sizeof(string), "Error: Your clan team is already set to %s.", TEAMS[listitem][TEAM_NAME]);
	    return SendClientMessage(playerid, COLOR_TOMATO, string);
	}

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new string[164];
	format(string, sizeof(string), "* Clan team has been changed to "COL_DEFAULT"%s"COL_BIEGE", by %s "COL_DEFAULT"%s", TEAMS[listitem][TEAM_NAME], clanRankNames[playerClanID[playerid]][playerClanRank[playerid]], name);

 	foreach (new i : Player) {
		if (playerClanID[i] == playerClanID[playerid]) {
	    	SendClientMessage(i, COLOR_BIEGE, string);
	    	SendClientMessage(i, COLOR_BIEGE, "* Please press "COL_DEFAULT"F4 "COL_BIEGE"or "COL_DEFAULT"/st "COL_BIEGE"to switch to the new assigned team after next death");

			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	clanData[playerClanID[playerid]][CLAN_TEAM] = listitem;
	return 1;
}

Dialog:clan_ranks(playerid, response, listitem, inputtext[]) {
	if (!response)
		return 1;

	if (listitem >= CLAN_RANK_LEADER) // last 2 ranks are static, cant be changed
	    return cmd_cranks(playerid);

	SetPVarInt(playerid, "ModifyRankLevel", listitem);

	new string[256];
	format(string, sizeof(string),
	    COL_WHITE "Please write a new rank name for rank level "COL_DEFAULT"%i "COL_WHITE"("COL_DEFAULT"%s"COL_WHITE")",
	    (listitem + 1), clanRankNames[playerClanID[playerid]][listitem]
	);
	return Dialog_Show(playerid, modify_clan_rank, DIALOG_STYLE_INPUT, GetClanDialogHeader(playerClanID[playerid], "Set rank's new name"), string, "Set", "Back");
}

Dialog:modify_clan_rank(playerid, response, listitem, inputtext[]) {
	if (!response)
		return cmd_cranks(playerid);

	new nameName[MAX_CLAN_RANK_NAME];
	if (sscanf(inputtext, "s["#MAX_CLAN_RANK_NAME"]", nameName)) {
	    new string[256];
		format(string, sizeof(string),
		    COL_WHITE "Please write a new rank name for rank level "COL_DEFAULT"%i "COL_WHITE"("COL_DEFAULT"%s"COL_WHITE")\n\n\
			"COL_TOMATO"Error: No rank name was entered!",
		    (GetPVarInt(playerid, "ModifyRankLevel") + 1), clanRankNames[playerClanID[playerid]][GetPVarInt(playerid, "ModifyRankLevel")]
		);
		return Dialog_Show(playerid, modify_clan_rank, DIALOG_STYLE_INPUT, GetClanDialogHeader(playerClanID[playerid], "Set rank's new name"), string, "Set", "Back");
	}

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new string[164];
	format(string, sizeof(string), "* Clan rank level "COL_DEFAULT"%i"COL_BIEGE", name has been changed to "COL_DEFAULT"%s"COL_BIEGE", by %s "COL_DEFAULT"%s", (GetPVarInt(playerid, "ModifyRankLevel") + 1), nameName, clanRankNames[playerClanID[playerid]][playerClanRank[playerid]], name);

 	foreach (new i : Player) {
		if (playerClanID[i] == playerClanID[playerid]) {
	    	SendClientMessage(i, COLOR_BIEGE, string);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	format(clanRankNames[playerClanID[playerid]][GetPVarInt(playerid, "ModifyRankLevel")], MAX_CLAN_RANK_NAME, nameName);

	DeletePVar(playerid, "ModifyRankLevel");
	return 1;
}

Dialog:select_promotion(playerid, response, listitem, inputtext[]) {
	if (!response)
		return 1;

	playerClanRank[GetPVarInt(playerid, "PromotionTargetPlayer")] = listitem;

	if (playerClanRank[GetPVarInt(playerid, "PromotionTargetPlayer")] == listitem) {
		new string[MAX_CLAN_RANKS * (MAX_CLAN_RANK_NAME + 4)];
	    for (new i = 0; i < CLAN_RANK_OWNER; i++) { // exclude "owner" rank
			format(string, sizeof(string),
			    "%s%i. %s\n",
			    string, (i + 1), clanRankNames[playerClanID[playerid]][i]
			);
		}
		return Dialog_Show(playerid, select_promotion, DIALOG_STYLE_LIST, GetClanDialogHeader(playerClanID[playerid], "Set player new rank"), string, "Select", "Cancel");
	}

	new string[164];
	if (playerClanRank[GetPVarInt(playerid, "PromotionTargetPlayer")] < listitem) {
		format(string, sizeof(string), "~n~~n~~n~~n~~n~~g~~h~CLAN RANK PROMOTION~n~~y~YOUR NEW RANK: ~w~%s", clanRankNames[playerClanID[playerid]][listitem]);
	} else {
		format(string, sizeof(string), "~n~~n~~n~~n~~n~~r~~h~CLAN RANK DEMOTION~n~~y~YOUR NEW RANK: ~w~%s", clanRankNames[playerClanID[playerid]][listitem]);
	}

	GameTextForPlayer(GetPVarInt(playerid, "PromotionTargetPlayer"), string, 3, 5000);

	new leadername[MAX_PLAYER_NAME];
	GetPlayerName(playerid, leadername, sizeof(leadername));

	new name[MAX_PLAYER_NAME];
	GetPlayerName(GetPVarInt(playerid, "PromotionTargetPlayer"), name, sizeof(name));

    if (playerClanRank[GetPVarInt(playerid, "PromotionTargetPlayer")] < listitem) {
		format(string, sizeof(string), "* %s has been promoted to %s (lvl: %i) "COL_BIEGE"by %s "COL_DEFAULT"%s", name, clanRankNames[playerClanID[playerid]][listitem], (listitem + 1), clanRankNames[playerClanID[playerid]][playerClanRank[playerid]], leadername);
	} else {
		format(string, sizeof(string), "* %s has been demoted to %s (lvl: %i) "COL_BIEGE"by %s "COL_DEFAULT"%s", name, clanRankNames[playerClanID[playerid]][listitem], (listitem + 1), clanRankNames[playerClanID[playerid]][playerClanRank[playerid]], leadername);
	}

	foreach (new i : Player) {
		if (playerClanID[i] == playerClanID[playerid]) {
	    	SendClientMessage(i, COLOR_YELLOW, string);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	DeletePVar(playerid, "PromotionTargetPlayer");
	return 1;
}

Dialog:clan_spawn(playerid, response, listitem, inputtext[]) {
	if (!response)
	    return 1;

	new clanid = playerClanID[playerid];

	if (clanData[clanid][CLAN_VAULT_MONEY] < 10000000) // 10 million to set new spawn
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Your clan needs atleast $10,000,000 in vault to set new clan spawn.");

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new string[164];
	format(string, sizeof(string), "* Clan spawn has been changed by %s "COL_DEFAULT"%s "COL_BIEGE"(cost: "COL_TOMATO"-$%s"COL_BIEGE")", listitem, clanRankNames[clanid][playerClanRank[playerid]], name, FormatNumber(10000000));

 	foreach (new i : Player) {
		if (playerClanID[i] == clanid) {
	    	SendClientMessage(i, COLOR_BIEGE, string);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	clanData[clanid][CLAN_VAULT_MONEY] -= 10000000;

	GetPlayerPos(playerid, clanData[clanid][CLAN_SPAWN_POS][0], clanData[clanid][CLAN_SPAWN_POS][1], clanData[clanid][CLAN_SPAWN_POS][2]);
    GetPlayerFacingAngle(playerid, clanData[clanid][CLAN_SPAWN_POS][3]);
    clanData[clanid][CLAN_SPAWN_INTERIORID] = GetPlayerInterior(playerid);
    clanData[clanid][CLAN_SPAWN_WORLDID] = GetPlayerVirtualWorld(playerid);

	DestroyDynamic3DTextLabel(clanData[clanid][CLAN_3D_TEXT_LABEL]);
	DestroyDynamicPickup(clanData[clanid][CLAN_PICKUPID]);

	format(string, sizeof(string), "[%s]\n%s\n"COL_DEFAULT"((SPAWN POINT))", clanData[clanid][CLAN_TAG], clanData[clanid][CLAN_NAME]);
	clanData[clanid][CLAN_3D_TEXT_LABEL] = CreateDynamic3DTextLabel(string, alpha(TEAMS[clanData[clanid][CLAN_TEAM]][TEAM_COLOR], 150), clanData[clanid][CLAN_SPAWN_POS][0], clanData[clanid][CLAN_SPAWN_POS][1], clanData[clanid][CLAN_SPAWN_POS][2], 25.0, _, _, 1, clanData[clanid][CLAN_SPAWN_WORLDID], clanData[clanid][CLAN_SPAWN_INTERIORID]);
	clanData[clanid][CLAN_PICKUPID] = CreateDynamicPickup(19306, 1, clanData[clanid][CLAN_SPAWN_POS][0], clanData[clanid][CLAN_SPAWN_POS][1], clanData[clanid][CLAN_SPAWN_POS][2], clanData[clanid][CLAN_SPAWN_WORLDID], clanData[clanid][CLAN_SPAWN_INTERIORID]);
	return 1;
}

Dialog:clan_remove_spawn(playerid, response, listitem, inputtext[]) {
	if (!response)
	    return 1;

	new clanid = playerClanID[playerid];

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, sizeof(name));

	new string[164];
	format(string, sizeof(string), "* Clan spawn has been removed by %s "COL_DEFAULT"%s; "COL_BIEGE"(default spawn: base/zones)", listitem, clanRankNames[playerClanID[playerid]][playerClanRank[playerid]], name);

 	foreach (new i : Player) {
		if (playerClanID[i] == clanid) {
	    	SendClientMessage(i, COLOR_BIEGE, string);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	clanData[clanid][CLAN_SPAWN_POS][0] = 0.0;
	clanData[clanid][CLAN_SPAWN_POS][1] = 0.0;
	clanData[clanid][CLAN_SPAWN_POS][2] = 0.0;
    clanData[clanid][CLAN_SPAWN_POS][3] = 0.0;
    clanData[clanid][CLAN_SPAWN_INTERIORID] = 0;
    clanData[clanid][CLAN_SPAWN_WORLDID] = 0;

	DestroyDynamic3DTextLabel(clanData[clanid][CLAN_3D_TEXT_LABEL]);
	DestroyDynamicPickup(clanData[clanid][CLAN_PICKUPID]);
	clanData[clanid][CLAN_3D_TEXT_LABEL] = Text3D:INVALID_STREAMER_ID;
	clanData[clanid][CLAN_PICKUPID] = INVALID_STREAMER_ID;

	return 1;
}
// ---------------------------------------

// ---------------------------------------
// CUSTOM SSCANF SPECIFIER
SSCANF:clan(string[]) {
	new clanid = -1;

	new len = strlen(string);

	new bool:isNumeric = true;
	for (new i = 0; i < len; i++) {
	    if (string[i] == ' ')
	        break;

    	if (string[i] > '9' || string[i] < '0')  {
			isNumeric = false;
			break;
		}
	}

	if (isNumeric) {
		clanid = strval(string) - 1;

		return (clanid < 0 || clanid >= totalSortedClans) ? -1 : sortedClansList[clanid][1];
	}

	foreach_clans(i) {
 		if (strfind(clanData[i][CLAN_NAME], string, true) != -1) {
			return i;
		}
	}

	new pos = strfind(string, "[");
	if (pos != -1)
		strdel(string, pos, pos + 1);

	pos = strfind(string, "]");
	if (pos != -1)
		strdel(string, pos, pos + 1);

	foreach_clans(i) {
		if (strcmp(clanData[i][CLAN_TAG], string, true) == 0) {
			return i;
		}
	}

	return -1;
}
// ---------------------------------------
