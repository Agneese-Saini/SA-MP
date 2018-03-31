// CopsAndTerrorists.pwn By Gammix
// An old gamemode of mine i hosted times ago!

#include <a_samp>

#undef MAX_PLAYERS
#define MAX_PLAYERS \
 	50

#pragma dynamic \
	10000

#if !defined KEY_AIM
	#define KEY_AIM \
		128
#endif

#if !defined IsValidVehicle
	native IsValidVehicle(vehicleid);
#endif

#define ZMSG_HYPHEN_END \
	""
#define ZMSG_HYPHEN_START \
	""

#include <zmessage>
#include <zcmd>
#include <sscanf2>
#include <streamer>
#include <PreviewModelDialog>
#include <easyDialog>
#include <attachmentfix>
#include <dini2>
#include <progress2>

#define COLOR_WHITE (0xFFFFFFFF)
#define COLOR_TOMATO (0xFF6347FF)
#define COLOR_YELLOW (0xFFDD00FF)
#define COLOR_GREEN (0x00FF00FF)
#define COLOR_DEFAULT (0xA9C4E4FF)

#define COL_WHITE "{FFFFFF}"
#define COL_TOMATO "{FF6347}"
#define COL_YELLOW "{FFDD00}"
#define COL_GREEN "{00FF00}"
#define COL_DEFAULT "{A9C4E4}"

#define DIRECTORY \
	""

#define MAX_SECURITY_QUESTIONS \
	25

#define MAX_SECURITY_QUESTION_SIZE \
	128

#define CAPTURE_TIME \
	60

#define SET_ALPHA(%1,%2) \
	((%1 & ~0xFF) | (clamp(%2, 0x00, 0xFF)))

main()
{
	SetGameModeText("TDM - v1.6");
}

new DB:db;

enum
{
	TEAM_COPS,
    TEAM_TERRORISTS
};

enum
{
	TEAM_COLOR_COPS = 0x095EE8FF,
    TEAM_COLOR_TERRORISTS = 0xE80909FF
};

enum e_USER
{
	e_USER_SQLID,
	e_USER_PASSWORD[64 + 1],
	e_USER_SALT[64 + 1],
	e_USER_KILLS,
	e_USER_DEATHS,
	e_USER_MONEY,
	e_USER_ADMIN_LEVEL,
	e_USER_VIP_LEVEL,
	e_USER_REGISTER_TIMESTAMP,
	e_USER_LASTLOGIN_TIMESTAMP,
	e_USER_SECURITY_QUESTION[MAX_SECURITY_QUESTION_SIZE],
	e_USER_SECURITY_ANSWER[64 + 1]
};
new eUser[MAX_PLAYERS][e_USER];
new iPlayerLoginAttempts[MAX_PLAYERS];
new iPlayerAnswerAttempts[MAX_PLAYERS];

enum e_STATIC_PICKUP
{
	e_STATIC_PICKUP_ID,
	e_STATIC_PICKUP_MODEL,
	Float:e_STATIC_PICKUP_AMOUNT,
	e_STATIC_PICKUP_TIMER
}
new eStaticPickup[MAX_PICKUPS][e_STATIC_PICKUP];
new iStaticPickupCount;

enum e_CAPTURE_ZONE
{
	e_CAPTURE_ZONE_NAME[35],
	Float:e_CAPTURE_ZONE_POS[4],
	Float:e_CAPTURE_ZONE_CPPOS[3],
	e_CAPTURE_ZONE_OWNER,
	e_CAPTURE_ZONE_ATTACKER,
	e_CAPTURE_ZONE_TICK,
	e_CAPTURE_ZONE_ID,
	e_CAPTURE_ZONE_CPID,
	e_CAPTURE_ZONE_TIMER,
	e_CAPTURE_ZONE_PLAYERS
};
new eCaptureZone[][e_CAPTURE_ZONE] =
{
	{"Cash Store", 	{607.3975, -523.7447, 676.2416, -476.0361}, 	{623.3041, -506.1986, 16.3525}, 	TEAM_TERRORISTS},
	{"Drug Den", 	{673.9339, -650.8413, 717.4572, -606.4443}, 	{705.4277, -639.7715, 16.3359}, 	TEAM_COPS},
	{"Houses", 		{726.5567, -598.6243, 836.5234, -477.0225}, 	{804.3663, -575.2430, 21.3363}, 	TEAM_TERRORISTS}
};
new PlayerText:ptxtCapture[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
new PlayerBar:pbarCapture[MAX_PLAYERS] = {INVALID_PLAYER_BAR_ID, ...};
new PlayerText:ptxtCaptureText[MAX_PLAYERS][2];
new tmrPlayerCaptureText[MAX_PLAYERS][2];
new iPlayerCaptureZone[MAX_PLAYERS];

enum e_WEAPON_SHOP
{
    e_WEAPON_SHOP_MODEL,
    e_WEAPON_SHOP_NAME[35],
    e_WEAPON_SHOP_PRICE,
    e_WEAPON_SHOP_AMMO
};
new const WEAPON_SHOP[][e_WEAPON_SHOP] =
{
	{335, "Knife", 750, 1},
	{341, "Chainsaw", 1500, 1},
	{342, "Grenade", 1545, 1},
	{343, "Moltove", 1745, 1},
	{347, "Silenced 9mm", 1500, 150},
	{348, "Desert Eagle", 3199, 150},
	{350, "Sawnoff Shotgun", 4999, 100},
	{351, "Combat Shotgun", 3870, 100},
	{352, "Micro-UZI", 3500, 300},
	{353, "MP5", 2999, 200},
	{372, "Tec-9", 3500, 300},
	{358, "Sniper Rifle", 4999, 50},
	{355, "Ak47", 2999, 200},
	{356, "M4", 3155, 200},
	{359, "RPG", 1999, 1},
	{361, "Flamethrower", 3500, 350},
	{362, "Minigun", 10000, 350},
	{363, "Satchel Charge", 1999, 2},
	{365, "Spray Can", 800, 200},
	{366, "Fire Extinguisher", 855, 200}
};

enum e_VEHICLE_SHOP
{
    e_VEHICLE_SHOP_MODEL,
    e_VEHICLE_SHOP_NAME[64],
    e_VEHICLE_SHOP_PRICE
};
new const VEHICLE_SHOP[][e_VEHICLE_SHOP] =
{
	{406, "Dumper", 15055},
	{409, "Stretch", 19999},
	{411, "Infernus", 26750},
	{425, "Hunter", 100000},
	{427, "Enforcer", 23599},
	{432, "Rhino", 55099},
	{434, "Hotknife", 17099},
	{437, "Coach", 12050},
	{444, "Monster", 19999},
	{447, "Seasparrow", 48900},
	{457, "Caddy", 8075},
	{461, "PCJ-600", 14699},
	{470, "Patriot", 10000},
	{481, "BMX", 7777},
	{494, "Hotring Racer", 25000},
	{532, "Combine Harvester", 15870}
};

new bool:bPlayerGambleStarted[MAX_PLAYERS];
new iPlayerGambleBet[MAX_PLAYERS];
new iPlayerGambleRightCard[MAX_PLAYERS];
new sPlayerGambleCards[MAX_PLAYERS][3][64];
new PlayerText:ptxtGamble[MAX_PLAYERS][6];

new CARD[] = "ld_card:cdback";
new RED_CARD[] = "ld_card:cd13h";
new BLACK_CARD[] = "ld_card:cd13s";
new OTHER_CARD[] = "ld_bum:bum2";

new const Float:COPS_SPAWN[][4] =
{
	{629.9655, -571.4874, 16.3359, 274.1459},
	{613.8113, -602.2937, 17.2266, 273.8325},
	{606.2744, -543.9374, 16.5985, 275.3991}
};

new const Float:TERRORISTS_SPAWN[][4] =
{
	{691.7385, -470.5683, 16.5363, 180.1450},
	{672.1321, -462.5963, 16.5363, 152.2346},
	{653.6218, -439.0352, 16.3359, 200.8018}
};

new const HELMETS[] =
{
	18936, 18937, 18938, 19101, 19102, 19103, 19104, 19105, 19106, 19107, 19108, 19109, 19110, 19111, 19112, 19113, 19114, 19115, 19116, 19117, 19118, 19119, 19120
};

enum
{
	NORMAL_DAY,
	CLOUDY_DAY,
	RAINY_DAY
};
new iDayType;
new iWeather;

new iPlayerClassid[MAX_PLAYERS];
new iPlayerSkills[MAX_PLAYERS][11];

new PlayerText:ptxtDeathText[MAX_PLAYERS][2];
new tmrPlayerDeathText[MAX_PLAYERS];

new PlayerText:ptxtNotification[MAX_PLAYERS];
new tmrPlayerNotification[MAX_PLAYERS];

new iPlayerHeadshotData[MAX_PLAYERS][2];

new bool:bPlayerHelmet[MAX_PLAYERS];
new PlayerText:ptxtHelmet[MAX_PLAYERS];

new iPlayerOffRadar[MAX_PLAYERS][2];
new PlayerText:ptxtOffRadar[MAX_PLAYERS][2];

new iPlayerVehicleid[MAX_PLAYERS][3];

new iPlayerProtection[MAX_PLAYERS][2];
new Text3D:iPlayerProtection3DText[MAX_PLAYERS];
new PlayerText:ptxtProtection[MAX_PLAYERS][2];

new iPlayerSpree[MAX_PLAYERS];

IpToLong(const address[])
{
	new parts[4];
	sscanf(address, "p<.>a<i>[4]", parts);
	return ((parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3]);
}

ReturnTimelapse(start, till)
{
    new ret[32];
	new seconds = till - start;

	const
		MINUTE = 60,
		HOUR = 60 * MINUTE,
		DAY = 24 * HOUR,
		MONTH = 30 * DAY;

	if (seconds == 1)
		format(ret, sizeof(ret), "a second");
	if (seconds < (1 * MINUTE))
		format(ret, sizeof(ret), "%i seconds", seconds);
	else if (seconds < (2 * MINUTE))
		format(ret, sizeof(ret), "a minute");
	else if (seconds < (45 * MINUTE))
		format(ret, sizeof(ret), "%i minutes", (seconds / MINUTE));
	else if (seconds < (90 * MINUTE))
		format(ret, sizeof(ret), "an hour");
	else if (seconds < (24 * HOUR))
		format(ret, sizeof(ret), "%i hours", (seconds / HOUR));
	else if (seconds < (48 * HOUR))
		format(ret, sizeof(ret), "a day");
	else if (seconds < (30 * DAY))
		format(ret, sizeof(ret), "%i days", (seconds / DAY));
	else if (seconds < (12 * MONTH))
    {
		new months = floatround(seconds / DAY / 30);
      	if (months <= 1)
			format(ret, sizeof(ret), "a month");
      	else
			format(ret, sizeof(ret), "%i months", months);
	}
    else
    {
      	new years = floatround(seconds / DAY / 365);
      	if (years <= 1)
			format(ret, sizeof(ret), "a year");
      	else
			format(ret, sizeof(ret), "%i years", years);
	}
	return ret;
}

bool:IsPasswordSecure(const password[])
{
    new bool:contain_number,
		bool:contain_highercase,
  		bool:contain_lowercase;
	for (new i, j = strlen(password); i < j; i++)
	{
 		switch (password[i])
   		{
     		case '0'..'9':
       			contain_number = true;
			case 'A'..'Z':
   				contain_highercase = true;
			case 'a'..'z':
   				contain_lowercase = true;
   		}

		if (contain_number && contain_highercase && contain_lowercase)
  			return true;
	}

	return false;
}

StaticPickup_Create(Float:x, Float:y, Float:z, model, Float:ammount, virtualworld = 0, expiretime = 0)
{
	new i;
	if (iStaticPickupCount == MAX_PICKUPS)
	{
	    StaticPickup_Destroy(0);
	}

	i = iStaticPickupCount++;
	eStaticPickup[i][e_STATIC_PICKUP_ID] = CreatePickup(model, 1, x, y, z, virtualworld);
	eStaticPickup[i][e_STATIC_PICKUP_MODEL] = model;
	eStaticPickup[i][e_STATIC_PICKUP_AMOUNT] = ammount;
    if (expiretime > 0)
		eStaticPickup[i][e_STATIC_PICKUP_TIMER] = SetTimerEx("StaticPickup_Destroy", expiretime, false, "i", i);
	else
		eStaticPickup[i][e_STATIC_PICKUP_TIMER] = -1;
    return i;
}

forward StaticPickup_Destroy(pickupid);
public StaticPickup_Destroy(pickupid)
{
	DestroyPickup(eStaticPickup[pickupid][e_STATIC_PICKUP_ID]);
    if (eStaticPickup[pickupid][e_STATIC_PICKUP_TIMER] != -1)
	{
		KillTimer(eStaticPickup[pickupid][e_STATIC_PICKUP_TIMER]);
		eStaticPickup[pickupid][e_STATIC_PICKUP_TIMER] = -1;
   	}

	if (pickupid < (iStaticPickupCount - 1))
	{
	    eStaticPickup[pickupid][e_STATIC_PICKUP_ID] = eStaticPickup[(iStaticPickupCount - 1)][e_STATIC_PICKUP_ID];
		eStaticPickup[pickupid][e_STATIC_PICKUP_MODEL] = eStaticPickup[(iStaticPickupCount - 1)][e_STATIC_PICKUP_MODEL];
		eStaticPickup[pickupid][e_STATIC_PICKUP_AMOUNT] = eStaticPickup[(iStaticPickupCount - 1)][e_STATIC_PICKUP_AMOUNT];
	    eStaticPickup[pickupid][e_STATIC_PICKUP_TIMER] = eStaticPickup[(iStaticPickupCount - 1)][e_STATIC_PICKUP_TIMER];
	}
}

GetModelWeaponID(weaponid)
{
	switch(weaponid)
	{
	    case 331: return 1;
	    case 333: return 2;
	    case 334: return 3;
	    case 335: return 4;
	    case 336: return 5;
	    case 337: return 6;
	    case 338: return 7;
	    case 339: return 8;
	    case 341: return 9;
	    case 321: return 10;
	    case 322: return 11;
	    case 323: return 12;
	    case 324: return 13;
	    case 325: return 14;
	    case 326: return 15;
	    case 342: return 16;
	    case 343: return 17;
	    case 344: return 18;
	    case 346: return 22;
	    case 347: return 23;
	    case 348: return 24;
	    case 349: return 25;
	    case 350: return 26;
	    case 351: return 27;
	    case 352: return 28;
	    case 353: return 29;
	    case 355: return 30;
	    case 356: return 31;
	    case 372: return 32;
	    case 357: return 33;
	    case 358: return 34;
	    case 359: return 35;
	    case 360: return 36;
	    case 361: return 37;
	    case 362: return 38;
	    case 363: return 39;
	    case 364: return 40;
	    case 365: return 41;
	    case 366: return 42;
	    case 367: return 43;
	    case 368: return 44;
	    case 369: return 45;
	    case 371: return 46;
	}
	return -1;
}

GetWeaponModelID(weaponid)
{
	switch(weaponid)
	{
	    case 1: return 331;
	    case 2: return 333;
	    case 3: return 334;
	    case 4: return 335;
	    case 5: return 336;
	    case 6: return 337;
	    case 7: return 338;
	    case 8: return 339;
	    case 9: return 341;
	    case 10: return 321;
	    case 11: return 322;
	    case 12: return 323;
	    case 13: return 324;
	    case 14: return 325;
	    case 15: return 326;
	    case 16: return 342;
	    case 17: return 343;
	    case 18: return 344;
	    case 22: return 346;
	    case 23: return 347;
	    case 24: return 348;
	    case 25: return 349;
	    case 26: return 350;
	    case 27: return 351;
	    case 28: return 352;
	    case 29: return 353;
	    case 30: return 355;
	    case 31: return 356;
	    case 32: return 372;
	    case 33: return 357;
	    case 34: return 358;
	    case 35: return 359;
	    case 36: return 360;
	    case 37: return 361;
	    case 38: return 362;
	    case 39: return 363;
	    case 40: return 364;
	    case 41: return 365;
	    case 42: return 366;
	    case 43: return 367;
	    case 44: return 368;
	    case 45: return 369;
	    case 46: return 371;
	}
	return -1;
}

ShowDeathText(playerid, killerid, expiretime, bool:headshot)
{
	if (tmrPlayerDeathText[playerid] != -1)
		KillTimer(tmrPlayerDeathText[playerid]);
	tmrPlayerDeathText[playerid] = SetTimerEx("OnDeathTextExpire", expiretime, false, "i", playerid);

	if (tmrPlayerDeathText[killerid] != -1)
		KillTimer(tmrPlayerDeathText[killerid]);
	tmrPlayerDeathText[killerid] = SetTimerEx("OnDeathTextExpire", expiretime, false, "i", killerid);

	new name[MAX_PLAYER_NAME],
		string[64];

	if (headshot)
		PlayerTextDrawSetString(playerid, ptxtDeathText[playerid][0], "~w~You got ~b~headshot ~w~by");
	else
		PlayerTextDrawSetString(playerid, ptxtDeathText[playerid][0], "~w~You were ~r~killed ~w~by");
	PlayerTextDrawShow(playerid, ptxtDeathText[playerid][0]);
	GetPlayerName(killerid, name, MAX_PLAYER_NAME);
	format(string, sizeof(string), "%s[%i]", name, killerid);
	PlayerTextDrawSetString(playerid, ptxtDeathText[playerid][1], string);
	PlayerTextDrawShow(playerid, ptxtDeathText[playerid][1]);

	if (headshot)
		PlayerTextDrawSetString(killerid, ptxtDeathText[killerid][0], "~w~You ~y~headshot");
	else
		PlayerTextDrawSetString(killerid, ptxtDeathText[killerid][0], "~w~You ~g~killed");
	PlayerTextDrawShow(killerid, ptxtDeathText[killerid][0]);
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	format(string, sizeof(string), "%s[%i]", name, playerid);
	PlayerTextDrawSetString(killerid, ptxtDeathText[killerid][1], string);
	PlayerTextDrawShow(killerid, ptxtDeathText[killerid][1]);
}

forward OnDeathTextExpire(playerid);
public OnDeathTextExpire(playerid)
{
    PlayerTextDrawHide(playerid, ptxtDeathText[playerid][0]);
    PlayerTextDrawHide(playerid, ptxtDeathText[playerid][1]);
    tmrPlayerDeathText[playerid] = -1;
}

ShowCaptureText(playerid, index, text[], expiretime)
{
	if (tmrPlayerCaptureText[playerid][index] != -1)
	    KillTimer(tmrPlayerCaptureText[playerid][index]);
	tmrPlayerCaptureText[playerid][index] = SetTimerEx("OnCaptureTextExpire", expiretime, false, "ii", playerid, index);

	PlayerTextDrawSetString(playerid, ptxtCaptureText[playerid][index], text);
	PlayerTextDrawShow(playerid, ptxtCaptureText[playerid][index]);
}

forward OnCaptureTextExpire(playerid, index);
public OnCaptureTextExpire(playerid, index)
{
    PlayerTextDrawHide(playerid, ptxtCaptureText[playerid][index]);
    tmrPlayerCaptureText[playerid][index] = -1;
}

ShowNotification(playerid, text[], expiretime)
{
	if (tmrPlayerNotification[playerid] != -1)
	    KillTimer(tmrPlayerNotification[playerid]);

	if (!text[0] || text[0] == ' ')
	{
	    OnNotificationExpire(playerid);
	    return;
	}

	if (expiretime > 0)
		tmrPlayerNotification[playerid] = SetTimerEx("OnNotificationExpire", expiretime, false, "i", playerid);

	PlayerTextDrawSetString(playerid, ptxtNotification[playerid], text);
	PlayerTextDrawShow(playerid, ptxtNotification[playerid]);
}

forward OnNotificationExpire(playerid);
public OnNotificationExpire(playerid)
{
    PlayerTextDrawHide(playerid, ptxtNotification[playerid]);
    tmrPlayerNotification[playerid] = -1;
}

IsPlayerSpawned(playerid)
{
	new Float:health;
	GetPlayerHealth(playerid, health);
	if (health <= 0.0)
	    return 0;
	else if (GetPlayerState(playerid) == PLAYER_STATE_WASTED)
		return 0;
	else if (GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
		return 0;
	else
		return 1;
}

public OnGameModeInit()
{
	if (strlen(DIRECTORY) > 1 && !dini_Exists(DIRECTORY))
	{
	    SendRconCommand("gmx");
		return 0;
	}

	new string[1024];

	if (!dini_Exists(DIRECTORY "config.ini"))
	{
	    dini_Create(DIRECTORY "config.ini");
	    dini_Set(DIRECTORY "config.ini", "database_name", "database.db");
	    dini_IntSet(DIRECTORY "config.ini", "max_warnings", 5);
     	dini_IntSet(DIRECTORY "config.ini", "max_login_attempts", 3);
     	dini_IntSet(DIRECTORY "config.ini", "max_answer_attempts", 3);
     	dini_IntSet(DIRECTORY "config.ini", "max_pint", 700);
     	dini_IntSet(DIRECTORY "config.ini", "max_accounts", 3);
     	dini_IntSet(DIRECTORY "config.ini", "max_admin_level", 5);
     	dini_IntSet(DIRECTORY "config.ini", "max_vip_level", 3);
     	dini_IntSet(DIRECTORY "config.ini", "min_password_length", 4);
     	dini_IntSet(DIRECTORY "config.ini", "max_password_length", 45);
     	dini_IntSet(DIRECTORY "config.ini", "account_lock_minutes", 2);
     	dini_BoolSet(DIRECTORY "config.ini", "toggle_spectate_feed", true);
     	dini_BoolSet(DIRECTORY "config.ini", "toggle_secure_password", true);
     	dini_BoolSet(DIRECTORY "config.ini", "toggle_anti_spam", true);
     	dini_BoolSet(DIRECTORY "config.ini", "toggle_anti_advert", true);
     	dini_BoolSet(DIRECTORY "config.ini", "toggle_anti_caps", false);
     	dini_BoolSet(DIRECTORY "config.ini", "toggle_blacklist", true);
     	dini_BoolSet(DIRECTORY "config.ini", "toggle_read_cmd", false);
     	dini_BoolSet(DIRECTORY "config.ini", "toggle_read_pm", true);
     	dini_BoolSet(DIRECTORY "config.ini", "toggle_aka", true);
     	dini_BoolSet(DIRECTORY "config.ini", "toggle_admins_cmd", true);
     	dini_BoolSet(DIRECTORY "config.ini", "toggle_guest_login", true);
	}

	if (!dini_Exists(DIRECTORY "adminspawns.ini"))
	{
	    string = "1435.8024,2662.3647,11.3926,1.1650\r\n";
		strcat(string, "1457.4762,2773.4868,10.8203,272.2754\r\n");
		strcat(string, "2101.4192,2678.7874,10.8130,92.0607\r\n");
		strcat(string, "1951.1090,2660.3877,10.8203,180.8461\r\n");
		strcat(string, "1666.6949,2604.9861,10.8203,179.8495\r\n");
		strcat(string, "1860.9672,1030.2910,10.8203,271.6988\r\n");
		strcat(string, "1673.2345,1316.1067,10.8203,177.7294\r\n");
		strcat(string, "1412.6187,2000.0596,14.7396,271.3568");

		new File:h = fopen(DIRECTORY "adminspawns.ini", io_write);
		fwrite(h, string);
		fclose(h);
	}

	if (!dini_Exists(DIRECTORY "questions.ini"))
	{
	    string = "What was your childhood nickname?\r\n";
		strcat(string, "What is the name of your favorite childhood friend?\r\n");
		strcat(string, "In what city or town did your mother and father meet?\r\n");
		strcat(string, "What is the middle name of your oldest child?\r\n");
		strcat(string, "What is your favorite team?\r\n");
		strcat(string, "What is your favorite movie?\r\n");
		strcat(string, "What is the first name of the boy or girl that you first kissed?\r\n");
		strcat(string, "What was the make and model of your first car?\r\n");
		strcat(string, "What was the name of the hospital where you were born?\r\n");
		strcat(string, "Who is your childhood sports hero?\r\n");
		strcat(string, "In what town was your first job?\r\n");
		strcat(string, "What was the name of the company where you had your first job?\r\n");
		strcat(string, "What school did you attend for sixth grade?\r\n");
		strcat(string, "What was the last name of your third grade teacher?");

		new File:h = fopen(DIRECTORY "questions.ini", io_write);
		fwrite(h, string);
		fclose(h);
	}

	db_debug_openresults();
	string = DIRECTORY;
	strcat(string, dini_Get(DIRECTORY "config.ini", "database_name"));
    db = db_open(string);
	db_query(db, "PRAGMA synchronous = NORMAL");
 	db_query(db, "PRAGMA journal_mode = WAL");

	string = "CREATE TABLE IF NOT EXISTS `users`(\
		`id` INTEGER PRIMARY KEY, \
		`name` VARCHAR(24) NOT NULL DEFAULT '', \
		`ip` VARCHAR(18) NOT NULL DEFAULT '', \
		`longip` INT NOT NULL DEFAULT '0', \
		`password` VARCHAR(64) NOT NULL DEFAULT '', \
		`salt` VARCHAR(64) NOT NULL DEFAULT '', \
		`sec_question` VARCHAR("#MAX_SECURITY_QUESTION_SIZE") NOT NULL DEFAULT '', \
		`sec_answer` VARCHAR(64) NOT NULL DEFAULT '', ";
	strcat(string, "`register_timestamp` INT NOT NULL DEFAULT '0', \
		`lastlogin_timestamp` INT NOT NULL DEFAULT '0', \
		`kills` INT NOT NULL DEFAULT '0', \
		`deaths` INT NOT NULL DEFAULT '0', \
		`money` INT NOT NULL DEFAULT '0', \
		`adminlevel` INT NOT NULL DEFAULT '0', \
		`viplevel` INT NOT NULL DEFAULT '0')");
	db_query(db, string);

    string = "CREATE TABLE IF NOT EXISTS `user_skills` (\
		`WEAPONSKILL_PISTOL` INT NOT NULL DEFAULT '0', \
		`WEAPONSKILL_PISTOL_SILENCED` INT NOT NULL DEFAULT '0', \
		`WEAPONSKILL_DESERT_EAGLE` INT NOT NULL DEFAULT '0', \
		`WEAPONSKILL_SHOTGUN` INT NOT NULL DEFAULT '0', \
		`WEAPONSKILL_SAWNOFF_SHOTGUN` INT NOT NULL DEFAULT '0', ";
	strcat(string, "`WEAPONSKILL_SPAS12_SHOTGUN` INT NOT NULL DEFAULT '0', \
		`WEAPONSKILL_MICRO_UZI` INT NOT NULL DEFAULT '0', \
		`WEAPONSKILL_MP5` INT NOT NULL DEFAULT '0', \
		`WEAPONSKILL_AK47` INT NOT NULL DEFAULT '0', \
		`WEAPONSKILL_M4` INT NOT NULL DEFAULT '0', \
		`WEAPONSKILL_SNIPERRIFLE` INT NOT NULL DEFAULT '0', \
		`user_id` INT NOT NULL DEFAULT '-1')");
	db_query(db, string);

	db_query(db, "CREATE TABLE IF NOT EXISTS `temp_blocked_users` (\
		`ip` VARCHAR(18) NOT NULL DEFAULT '', \
		`lock_timestamp` INT NOT NULL DEFAULT '0', \
		`user_id` INT NOT NULL DEFAULT '-1')");

	EnableVehicleFriendlyFire();
 	DisableInteriorEnterExits();
	UsePlayerPedAnims();
	SetNameTagDrawDistance(20.0);

	AddPlayerClass(4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 1
	AddPlayerClass(5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 2
	AddPlayerClass(9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 3
	AddPlayerClass(18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 5
	AddPlayerClass(28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 6
	AddPlayerClass(66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 7
	AddPlayerClass(80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 8
	AddPlayerClass(106, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 10
	AddPlayerClass(116, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 11
	AddPlayerClass(142, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 13
	AddPlayerClass(178, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 14
	AddPlayerClass(256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 15

	AddPlayerClass(280, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 1
	AddPlayerClass(281, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 2
	AddPlayerClass(282, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 3
	AddPlayerClass(283, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 4
	AddPlayerClass(283, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 5
	AddPlayerClass(286, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 8
	AddPlayerClass(288, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 9
	AddPlayerClass(300, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 10
	AddPlayerClass(302, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 11
	AddPlayerClass(310, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 12
	AddPlayerClass(311, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // 13

	for (new i; i < sizeof(eCaptureZone); i++)
	{
		eCaptureZone[i][e_CAPTURE_ZONE_ID] = GangZoneCreate(eCaptureZone[i][e_CAPTURE_ZONE_POS][0], eCaptureZone[i][e_CAPTURE_ZONE_POS][1], eCaptureZone[i][e_CAPTURE_ZONE_POS][2], eCaptureZone[i][e_CAPTURE_ZONE_POS][3]);
		eCaptureZone[i][e_CAPTURE_ZONE_CPID] = CreateDynamicCP(eCaptureZone[i][e_CAPTURE_ZONE_CPPOS][0], eCaptureZone[i][e_CAPTURE_ZONE_CPPOS][1], eCaptureZone[i][e_CAPTURE_ZONE_CPPOS][2], 5.0, 0, .streamdistance = 250.0);
		CreateDynamicMapIcon(eCaptureZone[i][e_CAPTURE_ZONE_CPPOS][0], eCaptureZone[i][e_CAPTURE_ZONE_CPPOS][1], eCaptureZone[i][e_CAPTURE_ZONE_CPPOS][2], 19, 0, 0, .streamdistance = 700.0);

		new label[65];
		format(label, sizeof(label), "ZONE #%i\n%s", i, eCaptureZone[i][e_CAPTURE_ZONE_NAME]);
		CreateDynamic3DTextLabel(label, SET_ALPHA(COLOR_TOMATO, 150), eCaptureZone[i][e_CAPTURE_ZONE_CPPOS][0], eCaptureZone[i][e_CAPTURE_ZONE_CPPOS][1], eCaptureZone[i][e_CAPTURE_ZONE_CPPOS][2], 50.0);

		eCaptureZone[i][e_CAPTURE_ZONE_ATTACKER] = INVALID_PLAYER_ID;
	}

	BeginDay();
	UpdateWeather();
	SetTimer("BeginDay", (24 * 60 * 1000), true);
	SetTimer("UpdateWeather", (30 * 60 * 1000), true);
	return 1;
}

forward BeginDay();
public BeginDay()
{
	#define PROBABILITY \
	    10

	iDayType = random(PROBABILITY);
	switch (iDayType)
	{
	    case 0..(PROBABILITY - 3):
	        iDayType = NORMAL_DAY;
		case (PROBABILITY - 2):
	        iDayType = CLOUDY_DAY;
		case (PROBABILITY - 1):
	        iDayType = RAINY_DAY;
	}
}

forward UpdateWeather();
public UpdateWeather()
{
	new h, x;
	gettime(h, x, x);
	#pragma unused x

	switch (h)
	{
	    case 0..5:
	        iWeather = 23;
		case 6..10:
		{
		    switch (iDayType)
		    {
		        case NORMAL_DAY:
		    		iWeather = 1;
		        case CLOUDY_DAY:
		    		iWeather = 15;
		        case RAINY_DAY:
		    		iWeather = 16;
		    }
		}
		case 11..14:
		{
		    switch (iDayType)
		    {
		        case NORMAL_DAY:
		    		iWeather = 3;
		        case CLOUDY_DAY:
		    		iWeather = 17;
		        case RAINY_DAY:
		    		iWeather = 9;
		    }
		}
		case 15..18:
		{
		    switch (iDayType)
		    {
		        case NORMAL_DAY:
		    		iWeather = 23;
		        case CLOUDY_DAY:
		    		iWeather = 29;
		        case RAINY_DAY:
		    		iWeather = 33;
		    }
		}
		case 19..24:
		{
		    switch (iDayType)
		    {
		        case NORMAL_DAY:
		    		iWeather = 2;
		        case CLOUDY_DAY:
		    		iWeather = 36;
		        case RAINY_DAY:
		    		iWeather = 36;
		    }
		}
	}
}

public OnGameModeExit()
{
	db_close(db);

	for (new i; i < iStaticPickupCount; i++)
		StaticPickup_Destroy(i);
	return 1;
}

public OnPlayerConnect(playerid)
{
	iPlayerLoginAttempts[playerid] = 0;
	iPlayerAnswerAttempts[playerid] = 0;
	tmrPlayerDeathText[playerid] = -1;
	tmrPlayerCaptureText[playerid][0] = -1;
	tmrPlayerCaptureText[playerid][1] = -1;
	for (new i; i < sizeof(iPlayerSkills[]); i++)
        iPlayerSkills[playerid][i] = 0;
    iPlayerCaptureZone[playerid] = -1;
    iPlayerOffRadar[playerid][0] = 0;
    iPlayerOffRadar[playerid][1] = -1;
    tmrPlayerNotification[playerid] = -1;

    ptxtDeathText[playerid][0] = CreatePlayerTextDraw(playerid, 321.000000, 328.000000, "~w~You were ~r~killed ~w~by");
	PlayerTextDrawAlignment(playerid, ptxtDeathText[playerid][0], 2);
	PlayerTextDrawBackgroundColor(playerid, ptxtDeathText[playerid][0], 255);
	PlayerTextDrawFont(playerid, ptxtDeathText[playerid][0], 2);
	PlayerTextDrawLetterSize(playerid, ptxtDeathText[playerid][0], 0.150000, 1.000000);
	PlayerTextDrawColor(playerid, ptxtDeathText[playerid][0], -1);
	PlayerTextDrawSetOutline(playerid, ptxtDeathText[playerid][0], 0);
	PlayerTextDrawSetProportional(playerid, ptxtDeathText[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, ptxtDeathText[playerid][0], 1);
	PlayerTextDrawSetSelectable(playerid, ptxtDeathText[playerid][0], 0);

	ptxtDeathText[playerid][1] = CreatePlayerTextDraw(playerid, 321.000000, 337.000000, "RydEr[4]");
	PlayerTextDrawAlignment(playerid, ptxtDeathText[playerid][1], 2);
	PlayerTextDrawBackgroundColor(playerid, ptxtDeathText[playerid][1], 255);
	PlayerTextDrawFont(playerid, ptxtDeathText[playerid][1], 1);
	PlayerTextDrawLetterSize(playerid, ptxtDeathText[playerid][1], 0.250000, 1.400000);
	PlayerTextDrawColor(playerid, ptxtDeathText[playerid][1], -1);
	PlayerTextDrawSetOutline(playerid, ptxtDeathText[playerid][1], 1);
	PlayerTextDrawSetProportional(playerid, ptxtDeathText[playerid][1], 1);
	PlayerTextDrawSetSelectable(playerid, ptxtDeathText[playerid][1], 0);

	ptxtNotification[playerid] = CreatePlayerTextDraw(playerid, 10.000000, 150.000000, "~w~You have chossen team ~b~Cops~w~. You mission is to eliminate ~r~Terrorists ~w~and capture all their terroteries!");
	PlayerTextDrawBackgroundColor(playerid, ptxtNotification[playerid], 0);
	PlayerTextDrawFont(playerid, ptxtNotification[playerid], 1);
	PlayerTextDrawLetterSize(playerid, ptxtNotification[playerid], 0.250000, 1.000000);
	PlayerTextDrawColor(playerid, ptxtNotification[playerid], -1);
	PlayerTextDrawSetOutline(playerid, ptxtNotification[playerid], 0);
	PlayerTextDrawSetProportional(playerid, ptxtNotification[playerid], 1);
	PlayerTextDrawSetShadow(playerid, ptxtNotification[playerid], 1);
	PlayerTextDrawUseBox(playerid, ptxtNotification[playerid], 1);
	PlayerTextDrawBoxColor(playerid, ptxtNotification[playerid], 150);
	PlayerTextDrawTextSize(playerid, ptxtNotification[playerid], 220.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid, ptxtNotification[playerid], 0);

	pbarCapture[playerid] = CreatePlayerProgressBar(playerid, 44.000000, 318.000000, 89.500000, 3.700000, -1429936641, CAPTURE_TIME, 0);

	ptxtCapture[playerid] = CreatePlayerTextDraw(playerid, 87.000000, 308.000000, "...");
	PlayerTextDrawBackgroundColor(playerid, ptxtCapture[playerid], 255);
	PlayerTextDrawFont(playerid, ptxtCapture[playerid], 1);
	PlayerTextDrawLetterSize(playerid, ptxtCapture[playerid], 0.180000, 0.799999);
	PlayerTextDrawColor(playerid, ptxtCapture[playerid], -1);
	PlayerTextDrawAlignment(playerid, ptxtCapture[playerid], 2);
	PlayerTextDrawSetOutline(playerid, ptxtCapture[playerid], 1);

	ptxtCaptureText[playerid][0] = CreatePlayerTextDraw(playerid, 317.000000, 348.000000, "~g~We have won the turfwar against the ~b~~h~~h~~h~Grove ~g~in ~b~~h~~h~~h~Gangton ~g~(/turfs)");
	PlayerTextDrawAlignment(playerid,  ptxtCaptureText[playerid][0], 2);
	PlayerTextDrawBackgroundColor(playerid, ptxtCaptureText[playerid][0], 255);
	PlayerTextDrawFont(playerid, ptxtCaptureText[playerid][0], 1);
	PlayerTextDrawLetterSize(playerid, ptxtCaptureText[playerid][0], 0.180000, 0.799999);
	PlayerTextDrawColor(playerid, ptxtCaptureText[playerid][0], -1);
	PlayerTextDrawSetOutline(playerid, ptxtCaptureText[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, ptxtCaptureText[playerid][0], 1);
	PlayerTextDrawSetSelectable(playerid, ptxtCaptureText[playerid][0], 0);

	ptxtCaptureText[playerid][1] = CreatePlayerTextDraw(playerid, 317.000000, 355.000000, "~r~We have lost the turfwar against the ~b~~h~~h~~h~Azetecas ~r~in ~b~~h~~h~~h~LS. Beach ~r~(/turfs)");
	PlayerTextDrawAlignment(playerid, ptxtCaptureText[playerid][1], 2);
	PlayerTextDrawBackgroundColor(playerid, ptxtCaptureText[playerid][1], 255);
	PlayerTextDrawFont(playerid, ptxtCaptureText[playerid][1], 1);
	PlayerTextDrawLetterSize(playerid, ptxtCaptureText[playerid][1], 0.180000, 0.799999);
	PlayerTextDrawColor(playerid, ptxtCaptureText[playerid][1], -1);
	PlayerTextDrawSetOutline(playerid, ptxtCaptureText[playerid][1], 1);
	PlayerTextDrawSetProportional(playerid, ptxtCaptureText[playerid][1], 1);
	PlayerTextDrawSetSelectable(playerid, ptxtCaptureText[playerid][1], 0);

	ptxtHelmet[playerid] = CreatePlayerTextDraw(playerid, 606.000000, 132.000000, "/Helmet: ~g~ON");
	PlayerTextDrawAlignment(playerid, ptxtHelmet[playerid], 3);
	PlayerTextDrawBackgroundColor(playerid, ptxtHelmet[playerid], 255);
	PlayerTextDrawFont(playerid, ptxtHelmet[playerid], 1);
	PlayerTextDrawLetterSize(playerid, ptxtHelmet[playerid], 0.239999, 1.199999);
	PlayerTextDrawColor(playerid, ptxtHelmet[playerid], -1);
	PlayerTextDrawSetOutline(playerid, ptxtHelmet[playerid], 1);
	PlayerTextDrawSetProportional(playerid, ptxtHelmet[playerid], 1);
	PlayerTextDrawSetPreviewModel(playerid, ptxtHelmet[playerid], 355);
	PlayerTextDrawSetPreviewRot(playerid, ptxtHelmet[playerid], 0.000000, 0.000000, -50.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid, ptxtHelmet[playerid], 0);

	ptxtOffRadar[playerid][0] = CreatePlayerTextDraw(playerid, 606.000000, 111.000000, "/Offradar: ~g~ON");
	PlayerTextDrawAlignment(playerid, ptxtOffRadar[playerid][0], 3);
	PlayerTextDrawBackgroundColor(playerid, ptxtOffRadar[playerid][0], 255);
	PlayerTextDrawFont(playerid, ptxtOffRadar[playerid][0], 1);
	PlayerTextDrawLetterSize(playerid, ptxtOffRadar[playerid][0], 0.239999, 1.199999);
	PlayerTextDrawColor(playerid, ptxtOffRadar[playerid][0], -1);
	PlayerTextDrawSetOutline(playerid, ptxtOffRadar[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, ptxtOffRadar[playerid][0], 1);
	PlayerTextDrawSetPreviewModel(playerid, ptxtOffRadar[playerid][0], 355);
	PlayerTextDrawSetPreviewRot(playerid, ptxtOffRadar[playerid][0], 0.000000, 0.000000, -50.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid, ptxtOffRadar[playerid][0], 0);

    ptxtOffRadar[playerid][1] = CreatePlayerTextDraw(playerid,606.000000, 122.000000, "You'll be visible back in ~y~2m 18s");
	PlayerTextDrawAlignment(playerid, ptxtOffRadar[playerid][1], 3);
	PlayerTextDrawBackgroundColor(playerid, ptxtOffRadar[playerid][1], 255);
	PlayerTextDrawFont(playerid, ptxtOffRadar[playerid][1], 1);
	PlayerTextDrawLetterSize(playerid, ptxtOffRadar[playerid][1], 0.149999, 0.799998);
	PlayerTextDrawColor(playerid, ptxtOffRadar[playerid][1], -1);
	PlayerTextDrawSetOutline(playerid, ptxtOffRadar[playerid][1], 1);
	PlayerTextDrawSetProportional(playerid, ptxtOffRadar[playerid][1], 1);
	PlayerTextDrawSetPreviewModel(playerid, ptxtOffRadar[playerid][1], 355);
	PlayerTextDrawSetPreviewRot(playerid, ptxtOffRadar[playerid][1], 0.000000, 0.000000, -50.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid, ptxtOffRadar[playerid][1], 0);

	ptxtProtection[playerid][0] = CreatePlayerTextDraw(playerid, 319.000000, 189.000000, "Anti-Spawnkill Protection");
	PlayerTextDrawAlignment(playerid, ptxtProtection[playerid][0], 2);
	PlayerTextDrawBackgroundColor(playerid, ptxtProtection[playerid][0], 255);
	PlayerTextDrawFont(playerid, ptxtProtection[playerid][0], 1);
	PlayerTextDrawLetterSize(playerid, ptxtProtection[playerid][0], 0.199999, 1.199999);
	PlayerTextDrawColor(playerid, ptxtProtection[playerid][0], -1);
	PlayerTextDrawSetOutline(playerid, ptxtProtection[playerid][0], 1);
	PlayerTextDrawSetProportional(playerid, ptxtProtection[playerid][0], 1);
	PlayerTextDrawSetPreviewModel(playerid, ptxtProtection[playerid][0], 355);
	PlayerTextDrawSetPreviewRot(playerid, ptxtProtection[playerid][0], 0.000000, 0.000000, -50.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid, ptxtProtection[playerid][0], 0);

	ptxtProtection[playerid][1] = CreatePlayerTextDraw(playerid, 320.000000, 197.000000, "10s");
	PlayerTextDrawAlignment(playerid, ptxtProtection[playerid][1], 2);
	PlayerTextDrawBackgroundColor(playerid, ptxtProtection[playerid][1], 255);
	PlayerTextDrawFont(playerid, ptxtProtection[playerid][1], 1);
	PlayerTextDrawLetterSize(playerid, ptxtProtection[playerid][1], 0.329999, 1.799999);
	PlayerTextDrawColor(playerid, ptxtProtection[playerid][1], -16776961);
	PlayerTextDrawSetOutline(playerid, ptxtProtection[playerid][1], 1);
	PlayerTextDrawSetProportional(playerid, ptxtProtection[playerid][1], 1);
	PlayerTextDrawSetPreviewModel(playerid, ptxtProtection[playerid][1], 355);
	PlayerTextDrawSetPreviewRot(playerid, ptxtProtection[playerid][1], 0.000000, 0.000000, -50.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid, ptxtProtection[playerid][1], 0);

	// Gambling
	ptxtGamble[playerid][0] = CreatePlayerTextDraw(playerid, 229.000000, 168.000000, "ld_spac:white");
	PlayerTextDrawBackgroundColor(playerid, ptxtGamble[playerid][0], 255);
	PlayerTextDrawFont(playerid, ptxtGamble[playerid][0], 4);
	PlayerTextDrawLetterSize(playerid, ptxtGamble[playerid][0], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, ptxtGamble[playerid][0], 255);
	PlayerTextDrawSetOutline(playerid, ptxtGamble[playerid][0], 0);
	PlayerTextDrawSetProportional(playerid, ptxtGamble[playerid][0], 1);
	PlayerTextDrawSetShadow(playerid, ptxtGamble[playerid][0], 1);
	PlayerTextDrawUseBox(playerid, ptxtGamble[playerid][0], 1);
	PlayerTextDrawBoxColor(playerid, ptxtGamble[playerid][0], 255);
	PlayerTextDrawTextSize(playerid, ptxtGamble[playerid][0], 54.000000, 74.000000);
	PlayerTextDrawSetSelectable(playerid, ptxtGamble[playerid][0], 0);

	ptxtGamble[playerid][1] = CreatePlayerTextDraw(playerid, 231.000000, 170.000000, "ld_card:cdback");
	PlayerTextDrawBackgroundColor(playerid, ptxtGamble[playerid][1], 255);
	PlayerTextDrawFont(playerid, ptxtGamble[playerid][1], 4);
	PlayerTextDrawLetterSize(playerid, ptxtGamble[playerid][1], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, ptxtGamble[playerid][1], -1);
	PlayerTextDrawSetOutline(playerid, ptxtGamble[playerid][1], 0);
	PlayerTextDrawSetProportional(playerid, ptxtGamble[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, ptxtGamble[playerid][1], 1);
	PlayerTextDrawUseBox(playerid, ptxtGamble[playerid][1], 1);
	PlayerTextDrawBoxColor(playerid, ptxtGamble[playerid][1], 255);
	PlayerTextDrawTextSize(playerid, ptxtGamble[playerid][1], 50.000000, 70.000000);
	PlayerTextDrawSetSelectable(playerid, ptxtGamble[playerid][1], 1);

	ptxtGamble[playerid][2] = CreatePlayerTextDraw(playerid, 289.000000, 168.000000, "ld_spac:white");
	PlayerTextDrawBackgroundColor(playerid, ptxtGamble[playerid][2], 255);
	PlayerTextDrawFont(playerid, ptxtGamble[playerid][2], 4);
	PlayerTextDrawLetterSize(playerid, ptxtGamble[playerid][2], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, ptxtGamble[playerid][2], 255);
	PlayerTextDrawSetOutline(playerid, ptxtGamble[playerid][2], 0);
	PlayerTextDrawSetProportional(playerid, ptxtGamble[playerid][2], 1);
	PlayerTextDrawSetShadow(playerid, ptxtGamble[playerid][2], 1);
	PlayerTextDrawUseBox(playerid, ptxtGamble[playerid][2], 1);
	PlayerTextDrawBoxColor(playerid, ptxtGamble[playerid][2], 255);
	PlayerTextDrawTextSize(playerid, ptxtGamble[playerid][2], 54.000000, 74.000000);
	PlayerTextDrawSetSelectable(playerid, ptxtGamble[playerid][2], 0);

	ptxtGamble[playerid][3] = CreatePlayerTextDraw(playerid, 291.000000, 170.000000, "ld_card:cdback");
	PlayerTextDrawBackgroundColor(playerid, ptxtGamble[playerid][3], 255);
	PlayerTextDrawFont(playerid, ptxtGamble[playerid][3], 4);
	PlayerTextDrawLetterSize(playerid, ptxtGamble[playerid][3], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, ptxtGamble[playerid][3], -1);
	PlayerTextDrawSetOutline(playerid, ptxtGamble[playerid][3], 0);
	PlayerTextDrawSetProportional(playerid, ptxtGamble[playerid][3], 1);
	PlayerTextDrawSetShadow(playerid, ptxtGamble[playerid][3], 1);
	PlayerTextDrawUseBox(playerid, ptxtGamble[playerid][3], 1);
	PlayerTextDrawBoxColor(playerid, ptxtGamble[playerid][3], 255);
	PlayerTextDrawTextSize(playerid, ptxtGamble[playerid][3], 50.000000, 70.000000);
	PlayerTextDrawSetSelectable(playerid, ptxtGamble[playerid][3], 1);

	ptxtGamble[playerid][4] = CreatePlayerTextDraw(playerid, 349.000000, 168.000000, "ld_spac:white");
	PlayerTextDrawBackgroundColor(playerid, ptxtGamble[playerid][4], 255);
	PlayerTextDrawFont(playerid, ptxtGamble[playerid][4], 4);
	PlayerTextDrawLetterSize(playerid, ptxtGamble[playerid][4], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, ptxtGamble[playerid][4], 255);
	PlayerTextDrawSetOutline(playerid, ptxtGamble[playerid][4], 0);
	PlayerTextDrawSetProportional(playerid, ptxtGamble[playerid][4], 1);
	PlayerTextDrawSetShadow(playerid, ptxtGamble[playerid][4], 1);
	PlayerTextDrawUseBox(playerid, ptxtGamble[playerid][4], 1);
	PlayerTextDrawBoxColor(playerid, ptxtGamble[playerid][4], 255);
	PlayerTextDrawTextSize(playerid, ptxtGamble[playerid][4], 54.000000, 74.000000);
	PlayerTextDrawSetSelectable(playerid, ptxtGamble[playerid][4], 0);

	ptxtGamble[playerid][5] = CreatePlayerTextDraw(playerid, 351.000000, 170.000000, "ld_card:cdback");
	PlayerTextDrawBackgroundColor(playerid, ptxtGamble[playerid][5], 255);
	PlayerTextDrawFont(playerid, ptxtGamble[playerid][5], 4);
	PlayerTextDrawLetterSize(playerid, ptxtGamble[playerid][5], 0.500000, 1.000000);
	PlayerTextDrawColor(playerid, ptxtGamble[playerid][5], -1);
	PlayerTextDrawSetOutline(playerid, ptxtGamble[playerid][5], 0);
	PlayerTextDrawSetProportional(playerid, ptxtGamble[playerid][5], 1);
	PlayerTextDrawSetShadow(playerid, ptxtGamble[playerid][5], 1);
	PlayerTextDrawUseBox(playerid, ptxtGamble[playerid][5], 1);
	PlayerTextDrawBoxColor(playerid, ptxtGamble[playerid][5], 255);
	PlayerTextDrawTextSize(playerid, ptxtGamble[playerid][5], 50.000000, 70.000000);
	PlayerTextDrawSetSelectable(playerid, ptxtGamble[playerid][5], 1);

	return SetTimerEx("OnPlayerJoin", 150, false, "i", playerid);
}

forward OnPlayerJoin(playerid);
public OnPlayerJoin(playerid)
{
	for (new i; i < 100; i++)
	{
	    SendClientMessage(playerid, COLOR_WHITE, "");
	}
	SendClientMessage(playerid, COLOR_YELLOW, "You have connected to \"SA-MP 0.3.7 Server\"");
	PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);

	new string[256];
	format(string, sizeof(string), "SELECT * FROM `users` WHERE `name` = '%q' LIMIT 1", name);
	new DBResult:result = db_query(db, string);
	if (db_num_rows(result) == 0)
	{
	    eUser[playerid][e_USER_SQLID] = -1;
	    eUser[playerid][e_USER_PASSWORD][0] = EOS;
	    eUser[playerid][e_USER_SALT][0] = EOS;
		eUser[playerid][e_USER_KILLS] = 0;
		eUser[playerid][e_USER_DEATHS] = 0;
		eUser[playerid][e_USER_MONEY] = 0;
		eUser[playerid][e_USER_ADMIN_LEVEL] = 0;
		eUser[playerid][e_USER_VIP_LEVEL] = 0;
		eUser[playerid][e_USER_REGISTER_TIMESTAMP] = 0;
		eUser[playerid][e_USER_LASTLOGIN_TIMESTAMP] = 0;
		eUser[playerid][e_USER_SECURITY_QUESTION][0] = EOS;
		eUser[playerid][e_USER_SECURITY_ANSWER][0] = EOS;

		Dialog_Show(playerid, REGISTER, DIALOG_STYLE_PASSWORD, "Account Registeration... [Step: 1/3]", COL_WHITE "Welcome to our server. We will take you through "COL_GREEN"3 simple steps "COL_WHITE"to register your account with a backup option in case you forgot your password!\nPlease enter a password, "COL_TOMATO"case sensitivity"COL_WHITE" is on.", "Continue", "Options");
		SendClientMessage(playerid, COLOR_WHITE, "[Step: 1/3] Enter your new account's password.");
	}
	else
	{
		iPlayerLoginAttempts[playerid] = 0;
		iPlayerAnswerAttempts[playerid] = 0;

		eUser[playerid][e_USER_SQLID] = db_get_field_assoc_int(result, "id");

		format(string, sizeof(string), "SELECT `lock_timestamp` FROM `temp_blocked_users` WHERE `user_id` = %i LIMIT 1", eUser[playerid][e_USER_SQLID]);
		new DBResult:lock_result = db_query(db, string);
		if (db_num_rows(lock_result) == 1)
		{
			new lock_timestamp = db_get_field_int(lock_result, 0);
			if ((gettime() - lock_timestamp) < 0)
		    {
		        format(string, sizeof(string), "Sorry!The account is temporarily locked on your IP. due to %i/%i failed login attempts.", dini_Int(DIRECTORY "config.ini", "max_login_attempts"), dini_Int(DIRECTORY "config.ini", "max_login_attempts"));
		        SendClientMessage(playerid, COLOR_TOMATO, string);
		        format(string, sizeof(string), "You'll be able to try again in %s.", ReturnTimelapse(gettime(), lock_timestamp));
				SendClientMessage(playerid, COLOR_TOMATO, string);
				db_free_result(result);
				db_free_result(lock_result);
				return Kick(playerid);
		    }
		    else
		    {
		        new ip[18];
				GetPlayerIp(playerid, ip, 18);
		        format(string, sizeof(string), "DELETE FROM `temp_blocked_users` WHERE `user_id` = %i AND `ip` = '%s'", eUser[playerid][e_USER_SQLID], ip);
		        db_query(db, string);
		    }
		}
		db_free_result(lock_result);

		db_get_field_assoc(result, "password", eUser[playerid][e_USER_PASSWORD], 64);
		db_get_field_assoc(result, "salt", eUser[playerid][e_USER_SALT], 64);
		eUser[playerid][e_USER_SALT][64] = EOS;
		eUser[playerid][e_USER_KILLS] = db_get_field_assoc_int(result, "kills");
		eUser[playerid][e_USER_DEATHS] = db_get_field_assoc_int(result, "deaths");
		eUser[playerid][e_USER_MONEY] = db_get_field_assoc_int(result, "money");
		eUser[playerid][e_USER_ADMIN_LEVEL] = db_get_field_assoc_int(result, "adminlevel");
		eUser[playerid][e_USER_VIP_LEVEL] = db_get_field_assoc_int(result, "viplevel");
		eUser[playerid][e_USER_REGISTER_TIMESTAMP] = db_get_field_assoc_int(result, "register_timestamp");
		eUser[playerid][e_USER_LASTLOGIN_TIMESTAMP] = db_get_field_assoc_int(result, "lastlogin_timestamp");
		db_get_field_assoc(result, "sec_question", eUser[playerid][e_USER_SECURITY_QUESTION], MAX_SECURITY_QUESTION_SIZE);
		db_get_field_assoc(result, "sec_answer", eUser[playerid][e_USER_SECURITY_ANSWER], 64);

		format(string, sizeof(string), "SELECT * FROM `user_skills` WHERE `user_id` = %i LIMIT 1", eUser[playerid][e_USER_SQLID]);
		new DBResult:result2 = db_query(db, string);
		if (db_num_rows(result2) == 1)
		{
		    for (new i; i < sizeof(iPlayerSkills[]); i++)
				iPlayerSkills[playerid][i] = db_get_field_int(result2, i);
		}
		db_free_result(result2);

		format(string, sizeof(string), COL_WHITE "Insert your secret password to access this account. If you failed in "COL_YELLOW"%i "COL_WHITE"attempts, account will be locked for "COL_YELLOW"%i "COL_WHITE"minutes.", dini_Int(DIRECTORY "config.ini", "max_login_attempts"), dini_Int(DIRECTORY "config.ini", "account_lock_minutes"));
		Dialog_Show(playerid, LOGIN, DIALOG_STYLE_PASSWORD, "Account Login...", string, "Login", "Options");
	}

	db_free_result(result);
	return 1;
}

Dialog:LOGIN(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
	    Dialog_Show(playerid, OPTIONS, DIALOG_STYLE_LIST, "Account Options...", "Forgot password\nForgot username\nClose", "Select", "Back");
	    return 1;
	}

	new string[256];

	new hash[64];
	SHA256_PassHash(inputtext, eUser[playerid][e_USER_SALT], hash, sizeof(hash));
	if (strcmp(hash, eUser[playerid][e_USER_PASSWORD]))
	{
		if (++iPlayerLoginAttempts[playerid] == dini_Int(DIRECTORY "config.ini", "max_login_attempts"))
		{
		    new lock_timestamp = (gettime() + (dini_Int(DIRECTORY "config.ini", "account_lock_minutes") * 60));
		    new ip[18];
		    GetPlayerIp(playerid, ip, 18);
			format(string, sizeof(string), "INSERT INTO `temp_blocked_users` VALUES('%s', %i, %i)", ip, lock_timestamp, eUser[playerid][e_USER_SQLID]);
			db_query(db, string);

		    format(string, sizeof(string), "Sorry!The account has been temporarily locked on your IP. due to %i/%i failed login attempts.", dini_Int(DIRECTORY "config.ini", "max_login_attempts"), dini_Int(DIRECTORY "config.ini", "max_login_attempts"));
		    SendClientMessage(playerid, COLOR_TOMATO, string);
		    format(string, sizeof(string), "If you forgot your password/username, click on 'Options' in login window next time (you may retry in %s).", ReturnTimelapse(gettime(), lock_timestamp));
			SendClientMessage(playerid, COLOR_TOMATO, string);
		    return Kick(playerid);
		}

	    format(string, sizeof(string), COL_WHITE "Insert your secret password to access this account. If you failed in "COL_YELLOW"%i "COL_WHITE"attempts, account will be locked for "COL_YELLOW"%i "COL_WHITE"minutes.", dini_Int(DIRECTORY "config.ini", "max_login_attempts"), dini_Int(DIRECTORY "config.ini", "account_lock_minutes"));
		Dialog_Show(playerid, LOGIN, DIALOG_STYLE_INPUT, "Account Login...", string, "Login", "Options");
	    format(string, sizeof(string), "Incorrect password!Your login tries left: %i/%i attempts.", iPlayerLoginAttempts[playerid], dini_Int(DIRECTORY "config.ini", "max_login_attempts"));
		SendClientMessage(playerid, COLOR_TOMATO, string);
	    return 1;
	}

	new name[MAX_PLAYER_NAME],
		ip[18];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	GetPlayerIp(playerid, ip, 18);
	format(string, sizeof(string), "UPDATE `users` SET `lastlogin_timestamp` = %i, `ip` = '%s', `longip` = %i WHERE `id` = %i", gettime(), ip, IpToLong(ip), eUser[playerid][e_USER_SQLID]);
	db_query(db, string);

	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid, eUser[playerid][e_USER_MONEY]);

	format(string, sizeof(string), "Successfully logged in!Welcome back to our server %s, we hope you enjoy your stay. [Last login: %s ago]", name, ReturnTimelapse(eUser[playerid][e_USER_LASTLOGIN_TIMESTAMP], gettime()));
	SendClientMessage(playerid, COLOR_GREEN, string);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	SetPVarInt(playerid, "LoggedIn", 1);
	CallRemoteFunction("OnPlayerLogin", "i", playerid);
	OnPlayerRequestClass(playerid, iPlayerClassid[playerid]);
	return 1;
}

Dialog:REGISTER(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
	    Dialog_Show(playerid, OPTIONS, DIALOG_STYLE_LIST, "Account Options...", "Forgot password\nForgot username\nClose", "Select", "Back");
	    return 1;
	}

	if (!(dini_Int(DIRECTORY "config.ini", "min_password_length") <= strlen(inputtext) <= dini_Int(DIRECTORY "config.ini", "max_password_length")))
	{
	    Dialog_Show(playerid, REGISTER, DIALOG_STYLE_PASSWORD, "Account Registeration... [Step: 1/3]", COL_WHITE "Welcome to our server. We will take you through "COL_GREEN"3 simple steps "COL_WHITE"to register your account with a backup option in case you forgot your password!\nPlease enter a password, "COL_TOMATO"case sensitivity"COL_WHITE" is on.", "Continue", "Options");

		new string[150];
		format(string, sizeof(string), "Invalid password length, must be between %i - %i characters.", dini_Int(DIRECTORY "config.ini", "min_password_length"), dini_Int(DIRECTORY "config.ini", "max_password_length"));
		SendClientMessage(playerid, COLOR_TOMATO, string);
	    return 1;
	}

	if (dini_Bool(DIRECTORY "config.ini", "toggle_secure_password"))
	{
        if (!IsPasswordSecure(inputtext))
		{
		    Dialog_Show(playerid, REGISTER, DIALOG_STYLE_INPUT, "Account Registeration... [Step: 1/3]", COL_WHITE "Welcome to our server. We will take you through "COL_GREEN"3 simple steps "COL_WHITE"to register your account with a backup option in case you forgot your password!\nPlease enter a password, "COL_TOMATO"case sensitivity"COL_WHITE" is on.", "Continue", "Options");
			SendClientMessage(playerid, COLOR_TOMATO, "Password must contain atleast a Highercase, a Lowercase and a Number.");
		    return 1;
		}
	}

	for (new i; i < 64; i++)
	{
		eUser[playerid][e_USER_SALT][i] = (random('z' - 'A') + 'A');
	}
	eUser[playerid][e_USER_SALT][64] = EOS;
	SHA256_PassHash(inputtext, eUser[playerid][e_USER_SALT], eUser[playerid][e_USER_PASSWORD], 64);

	new line[MAX_SECURITY_QUESTION_SIZE],
		info[MAX_SECURITY_QUESTIONS * MAX_SECURITY_QUESTION_SIZE],
		File:h,
		count;
	h = fopen(DIRECTORY "questions.ini", io_read);
	while (fread(h, line))
	{
	    strcat(info, line);
	    strcat(info, "\n");

	    if (++count >= MAX_SECURITY_QUESTIONS)
	        break;
	}
	fclose(h);

	Dialog_Show(playerid, SEC_QUESTION, DIALOG_STYLE_LIST, "Account Registeration... [Step: 2/3]", info, "Continue", "Back");
	SendClientMessage(playerid, COLOR_WHITE, "[Step: 2/3] Select a security question. This will help you retrieve your password in case you forget it any time soon!");
	PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:SEC_QUESTION(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
	    Dialog_Show(playerid, REGISTER, DIALOG_STYLE_PASSWORD, "Account Registeration... [Step: 1/3]", COL_WHITE "Welcome to our server. We will take you through "COL_GREEN"3 simple steps "COL_WHITE"to register your account with a backup option in case you forgot your password!\nPlease enter a password, "COL_TOMATO"case sensitivity"COL_WHITE" is on.", "Continue", "Options");
		SendClientMessage(playerid, COLOR_WHITE, "[Step: 1/3] Enter your new account's password.");
		return 1;
	}

	format(eUser[playerid][e_USER_SECURITY_QUESTION], MAX_SECURITY_QUESTION_SIZE, inputtext);

	new string[256];
	format(string, sizeof(string), COL_TOMATO "%s\n"COL_WHITE"Insert your answer below in the box. (don't worry about CAPS, answers are NOT case sensitive).", inputtext);
	Dialog_Show(playerid, SEC_ANSWER, DIALOG_STYLE_INPUT, "Account Registeration... [Step: 3/3]", string, "Confirm", "Back");
	SendClientMessage(playerid, COLOR_WHITE, "[Step: 3/3] Write the answer to your secuirty question and you'll be done :)");
	PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:SEC_ANSWER(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
	    new line[MAX_SECURITY_QUESTION_SIZE],
			info[MAX_SECURITY_QUESTIONS * MAX_SECURITY_QUESTION_SIZE],
			File:h,
			count;
		h = fopen(DIRECTORY "questions.ini", io_read);
		while (fread(h, line))
		{
		    strcat(info, line);
		    strcat(info, "\n");

		    if (++count >= MAX_SECURITY_QUESTIONS)
		        break;
		}
		fclose(h);

		Dialog_Show(playerid, SEC_QUESTION, DIALOG_STYLE_LIST, "Account Registeration... [Step: 2/3]", info, "Continue", "Back");
		SendClientMessage(playerid, COLOR_WHITE, "[Step: 2/3] Select a security question. This will help you retrieve your password in case you forget it any time soon!");
		return 1;
	}

	new string[512];

	if (strlen(inputtext) < dini_Int(DIRECTORY "config.ini", "min_password_length") || inputtext[0] == ' ')
	{
	    format(string, sizeof(string), COL_TOMATO "%s\n"COL_WHITE"Insert your answer below in the box. (don't worry about CAPS, answers are NOT case sensitive).", eUser[playerid][e_USER_SECURITY_QUESTION]);
		Dialog_Show(playerid, SEC_ANSWER, DIALOG_STYLE_INPUT, "Account Registeration... [Step: 3/3]", string, "Confirm", "Back");
		format(string, sizeof(string), "Security answer cannot be an less than %i characters.", dini_Int(DIRECTORY "config.ini", "min_password_length"));
		SendClientMessage(playerid, COLOR_TOMATO, string);
		return 1;
	}

	for (new i, j = strlen(inputtext); i < j; i++)
	{
        inputtext[i] = tolower(inputtext[i]);
	}
	SHA256_PassHash(inputtext, eUser[playerid][e_USER_SALT], eUser[playerid][e_USER_SECURITY_ANSWER], 64);

	new name[MAX_PLAYER_NAME],
		ip[18];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	GetPlayerIp(playerid, ip, 18);
	format(string, sizeof(string), "INSERT INTO `users`(`name`, `ip`, `longip`, `password`, `salt`, `sec_question`, `sec_answer`, `register_timestamp`, `lastlogin_timestamp`) VALUES('%s', '%s', %i, '%q', '%q', '%q', '%q', %i, %i)", name, ip, IpToLong(ip), eUser[playerid][e_USER_PASSWORD], eUser[playerid][e_USER_SALT], eUser[playerid][e_USER_SECURITY_QUESTION], eUser[playerid][e_USER_SECURITY_ANSWER], gettime(), gettime());
	db_query(db, string);

	format(string, sizeof(string), "SELECT `id` FROM `users` WHERE `name` = '%q' LIMIT 1", name);
	new DBResult:result = db_query(db, string);
    eUser[playerid][e_USER_SQLID] = db_get_field_int(result, 0);
	db_free_result(result);

	format(string, sizeof(string), "INSERT INTO `user_skills`(`user_id`) VALUES(%i)", eUser[playerid][e_USER_SQLID]);
	db_query(db, string);

	eUser[playerid][e_USER_REGISTER_TIMESTAMP] = gettime();
	eUser[playerid][e_USER_LASTLOGIN_TIMESTAMP] = gettime();

	format(string, sizeof(string), "Successfully registered!Welcome to our server %s, we hope you enjoy your stay. [IP: %s]", name, ip);
	SendClientMessage(playerid, COLOR_GREEN, string);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	SetPVarInt(playerid, "LoggedIn", 1);
	CallRemoteFunction("OnPlayerRegister", "i", playerid);
	CallRemoteFunction("OnPlayerLogin", "i", playerid);
	OnPlayerRequestClass(playerid, iPlayerClassid[playerid]);
	return 1;
}

Dialog:OPTIONS(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
		if (eUser[playerid][e_USER_SQLID] != -1)
		{
			new string[256];
		    format(string, sizeof(string), COL_WHITE "Insert your secret password to access this account. If you failed in "COL_YELLOW"%i "COL_WHITE"attempts, account will be locked for "COL_YELLOW"%i "COL_WHITE"minutes.", dini_Int(DIRECTORY "config.ini", "max_login_attempts"), dini_Int(DIRECTORY "config.ini", "account_lock_minutes"));
			Dialog_Show(playerid, LOGIN, DIALOG_STYLE_PASSWORD, "Account Login...", string, "Login", "Options");
		}
		else
			Dialog_Show(playerid, REGISTER, DIALOG_STYLE_PASSWORD, "Account Registeration... [Step: 1/3]", COL_WHITE "Welcome to our server. We will take you through "COL_GREEN"3 simple steps "COL_WHITE"to register your account with a backup option in case you forgot your password!\nPlease enter a password, "COL_TOMATO"case sensitivity"COL_WHITE" is on.", "Continue", "Options");
		return 1;
	}

	switch (listitem)
	{
	    case 0:
	    {
	        if (eUser[playerid][e_USER_SQLID] == -1)
	        {
	            SendClientMessage(playerid, COLOR_TOMATO, "This account isn't registered, try 'Forgot Username' or change your name and connect.");
	        	Dialog_Show(playerid, OPTIONS, DIALOG_STYLE_LIST, "Account Options...", "Forgot password\nForgot username\nClose", "Select", "Back");
	        	return 1;
	        }

			new string[64 + MAX_SECURITY_QUESTION_SIZE];
			format(string, sizeof(string), COL_WHITE "Answer your security question to reset password.\n\n"COL_TOMATO"%s", eUser[playerid][e_USER_SECURITY_QUESTION]);
			Dialog_Show(playerid, FORGOT_PASSWORD, DIALOG_STYLE_INPUT, "Forgot Password:", string, "Next", "Cancel");
	    }
	    case 1:
	    {
	        const MASK = (-1 << (32 - 36));
			new string[256],
				ip[18];
			GetPlayerIp(playerid, ip, 18);
			format(string, sizeof(string), "SELECT `name`, `lastlogin_timestamp` FROM `users` WHERE ((`longip` & %i) = %i) LIMIT 1", MASK, (IpToLong(ip) & MASK));
			new DBResult:result = db_query(db, string);
			if (db_num_rows(result) == 0)
			{
			    SendClientMessage(playerid, COLOR_TOMATO, "There are no accounts realted to this ip, this seems to be your first join!");
		     	Dialog_Show(playerid, OPTIONS, DIALOG_STYLE_LIST, "Account Options...", "Forgot password\nForgot username\nClose", "Select", "Back");
			    return 1;
			}

			new list[25 * (MAX_PLAYER_NAME + 32)],
				name[MAX_PLAYER_NAME],
				lastlogin_timestamp,
				i,
				j = ((db_num_rows(result) > 10) ? (10) : (db_num_rows(result)));

			do
			{
			    db_get_field_assoc(result, "name", name, MAX_PLAYER_NAME);
				lastlogin_timestamp = db_get_field_assoc_int(result, "lastlogin_timestamp");
			    format(list, sizeof(list), "%s"COL_TOMATO"%s "COL_WHITE"|| Last login: %s ago\n", list, name, ReturnTimelapse(lastlogin_timestamp, gettime()));
			}
			while (db_next_row(result) && i > j);
			db_free_result(result);

			Dialog_Show(playerid, FORGOT_USERNAME, DIALOG_STYLE_LIST, "Your username history...", list, "Ok", "");
			PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
	    }
	    case 2:
	    {
	        return Kick(playerid);
	    }
	}
	return 1;
}

Dialog:FORGOT_PASSWORD(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
	    Dialog_Show(playerid, OPTIONS, DIALOG_STYLE_LIST, "Account Options...", "Forgot password\nForgot username\nClose", "Select", "Back");
	    return 1;
	}

	new string[256],
		hash[64];
	SHA256_PassHash(inputtext, eUser[playerid][e_USER_SALT], hash, sizeof(hash));
	if (strcmp(hash, eUser[playerid][e_USER_SECURITY_ANSWER]))
	{
		if (++iPlayerAnswerAttempts[playerid] == dini_Int(DIRECTORY "config.ini", "max_login_attempts"))
		{
		    new lock_timestamp = gettime() + (dini_Int(DIRECTORY "config.ini", "account_lock_minutes") * 60);
		    new ip[18];
		    GetPlayerIp(playerid, ip, 18);
            format(string, sizeof(string), "INSERT INTO `temp_blocked_users` VALUES('%s', %i, %i)", ip, lock_timestamp, eUser[playerid][e_USER_SQLID]);
			db_query(db, string);

		    format(string, sizeof(string), "Sorry!The account has been temporarily locked on your IP. due to %i/%i failed login attempts.", dini_Int(DIRECTORY "config.ini", "max_login_attempts"), dini_Int(DIRECTORY "config.ini", "max_login_attempts"));
		    SendClientMessage(playerid, COLOR_TOMATO, string);
		    format(string, sizeof(string), "If you forgot your password/username, click on 'Options' in login window next time (you may retry in %s).", ReturnTimelapse(gettime(), lock_timestamp));
			SendClientMessage(playerid, COLOR_TOMATO, string);
		    return Kick(playerid);
		}

	    format(string, sizeof(string), COL_WHITE "Answer your security question to reset password.\n\n"COL_TOMATO"%s", eUser[playerid][e_USER_SECURITY_QUESTION]);
		Dialog_Show(playerid, FORGOT_PASSWORD, DIALOG_STYLE_INPUT, "Forgot Password:", string, "Next", "Cancel");
		format(string, sizeof(string), "Incorrect answer!Your tries left: %i/%i attempts.", iPlayerAnswerAttempts[playerid], dini_Int(DIRECTORY "config.ini", "max_login_attempts"));
		SendClientMessage(playerid, COLOR_TOMATO, string);
	    return 1;
	}

	Dialog_Show(playerid, RESET_PASSWORD, DIALOG_STYLE_PASSWORD, "Reset Password:", COL_WHITE "Insert a new password for your account. Also in case you want to change security question for later, use /ucp.", "Confirm", "");
	SendClientMessage(playerid, COLOR_GREEN, "Successfully answered your security question!You shall now reset your password.");
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:RESET_PASSWORD(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
		Dialog_Show(playerid, RESET_PASSWORD, DIALOG_STYLE_PASSWORD, "Reset Password:", COL_WHITE "Insert a new password for your account. Also in case you want to change security question for later, use /ucp.", "Confirm", "");
		return 1;
	}

	new string[256];

	if (!(dini_Int(DIRECTORY "config.ini", "min_password_length") <= strlen(inputtext) <= dini_Int(DIRECTORY "config.ini", "max_password_length")))
	{
	    Dialog_Show(playerid, RESET_PASSWORD, DIALOG_STYLE_PASSWORD, "Reset Password:", COL_WHITE "Insert a new password for your account. Also in case you want to change security question for later, use /ucp.", "Confirm", "");
		format(string, sizeof(string), "Invalid password length, must be between %i - %i characters.", dini_Int(DIRECTORY "config.ini", "min_password_length"), dini_Int(DIRECTORY "config.ini", "max_password_length"));
		SendClientMessage(playerid, COLOR_TOMATO, string);
	    return 1;
	}

    if (dini_Bool(DIRECTORY "config.ini", "toggle_secure_password"))
    {
		if (!IsPasswordSecure(inputtext))
		{
		    Dialog_Show(playerid, RESET_PASSWORD, DIALOG_STYLE_PASSWORD, "Reset Password:", COL_WHITE "Insert a new password for your account. Also in case you want to change security question for later, use /ucp.", "Confirm", "");
			SendClientMessage(playerid, COLOR_TOMATO, "Password must contain atleast a Highercase, a Lowercase and a Number.");
		    return 1;
		}
	}

	SHA256_PassHash(inputtext, eUser[playerid][e_USER_SALT], eUser[playerid][e_USER_PASSWORD], 64);

	new name[MAX_PLAYER_NAME],
		ip[18];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	GetPlayerIp(playerid, ip, 18);
	format(string, sizeof(string), "UPDATE `users` SET `password` = '%q', `ip` = '%s', `longip` = %i, `lastlogin_timestamp` = %i WHERE `id` = %i", eUser[playerid][e_USER_PASSWORD], ip, IpToLong(ip), gettime(), eUser[playerid][e_USER_SQLID]);
	db_query(db, string);

	format(string, sizeof(string), "Successfully logged in with new password!Welcome back to our server %s, we hope you enjoy your stay. [Last login: %s ago]", name, ReturnTimelapse(eUser[playerid][e_USER_LASTLOGIN_TIMESTAMP], gettime()));
	SendClientMessage(playerid, COLOR_GREEN, string);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	SetPVarInt(playerid, "LoggedIn", 1);
	CallRemoteFunction("OnPlayerLogin", "i", playerid);
	OnPlayerRequestClass(playerid, iPlayerClassid[playerid]);
	return 1;
}

Dialog:FORGOT_USERNAME(playerid, response, listitem, inputtext[])
{
	Dialog_Show(playerid, OPTIONS, DIALOG_STYLE_LIST, "Account Options...", "Forgot password\nForgot username\nClose", "Select", "Back");
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    if (iPlayerProtection[playerid][0] > 0)
	{
 		DestroyDynamic3DTextLabel(iPlayerProtection3DText[playerid]);
		KillTimer(iPlayerProtection[playerid][1]);
		PlayerTextDrawHide(playerid, ptxtProtection[playerid][0]);
		PlayerTextDrawHide(playerid, ptxtProtection[playerid][1]);
	}

    if (iPlayerOffRadar[playerid][0] > 0)
		KillTimer(iPlayerOffRadar[playerid][1]);

    if (GetPVarInt(playerid, "LoggedIn"))
    {
		new string[1024],
			name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	    format(string, sizeof(string), "UPDATE `users` SET `name` = '%s', `password` = '%q', `salt` = '%q', `sec_question` = '%q', `sec_answer` = '%q', `kills` = %i, `deaths` = %i, `money` = %i, `adminlevel` = %i, `viplevel` = %i WHERE `id` = %i",
			name, eUser[playerid][e_USER_PASSWORD], eUser[playerid][e_USER_SALT], eUser[playerid][e_USER_SECURITY_QUESTION], eUser[playerid][e_USER_SECURITY_ANSWER],  eUser[playerid][e_USER_KILLS], eUser[playerid][e_USER_DEATHS], GetPlayerMoney(playerid), eUser[playerid][e_USER_ADMIN_LEVEL], eUser[playerid][e_USER_VIP_LEVEL], eUser[playerid][e_USER_SQLID]);
		db_query(db, string);

		format(string, sizeof(string), "UPDATE `user_skills` SET `WEAPONSKILL_PISTOL` = %i, `WEAPONSKILL_PISTOL_SILENCED` = %i, `WEAPONSKILL_DESERT_EAGLE` = %i, `WEAPONSKILL_SHOTGUN` = %i, `WEAPONSKILL_SAWNOFF_SHOTGUN` = %i, `WEAPONSKILL_SPAS12_SHOTGUN` = %i, `WEAPONSKILL_MICRO_UZI` = %i, `WEAPONSKILL_MP5` = %i, `WEAPONSKILL_AK47` = %i, `WEAPONSKILL_M4` = %i, `WEAPONSKILL_SNIPERRIFLE` = %i WHERE `user_id` = %i",
			iPlayerSkills[playerid][WEAPONSKILL_PISTOL], iPlayerSkills[playerid][WEAPONSKILL_PISTOL_SILENCED], iPlayerSkills[playerid][WEAPONSKILL_DESERT_EAGLE], iPlayerSkills[playerid][WEAPONSKILL_SHOTGUN], iPlayerSkills[playerid][WEAPONSKILL_SAWNOFF_SHOTGUN], iPlayerSkills[playerid][WEAPONSKILL_SPAS12_SHOTGUN], iPlayerSkills[playerid][WEAPONSKILL_MICRO_UZI], iPlayerSkills[playerid][WEAPONSKILL_MP5],
			iPlayerSkills[playerid][WEAPONSKILL_AK47], iPlayerSkills[playerid][WEAPONSKILL_M4], iPlayerSkills[playerid][WEAPONSKILL_SNIPERRIFLE], eUser[playerid][e_USER_SQLID]);
		db_query(db, string);
	}
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    new h, m, s;
	gettime(h, m, s);
	#pragma unused s
	SetPlayerTime(playerid, h, m);
	SetPlayerWeather(playerid, iWeather);

	if (bPlayerGambleStarted[playerid])
	{
    	bPlayerGambleStarted[playerid] = false;

		PlayerTextDrawHide(playerid, ptxtGamble[playerid][0]);
	 	PlayerTextDrawHide(playerid, ptxtGamble[playerid][1]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][2]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][3]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][4]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][5]);
	}

    if (iPlayerProtection[playerid][0] > 0)
	{
 		DestroyDynamic3DTextLabel(iPlayerProtection3DText[playerid]);
		KillTimer(iPlayerProtection[playerid][1]);
		PlayerTextDrawHide(playerid, ptxtProtection[playerid][0]);
		PlayerTextDrawHide(playerid, ptxtProtection[playerid][1]);
	}

	if (iPlayerOffRadar[playerid][0] > 0)
		KillTimer(iPlayerOffRadar[playerid][1]);

	if (iPlayerCaptureZone[playerid] != -1)
	{
	    OnPlayerLeaveDynamicCP(playerid, eCaptureZone[iPlayerCaptureZone[playerid]][e_CAPTURE_ZONE_CPID]);
	    iPlayerCaptureZone[playerid] = -1;
	}

    iPlayerClassid[playerid] = classid;

	if (!GetPVarInt(playerid, "LoggedIn"))
	{
        SetPlayerCameraPos(playerid, 693.9114, -494.8807, 22.6305);
		SetPlayerCameraLookAt(playerid, 693.2760, -494.1037, 22.5604, CAMERA_MOVE);
	}
	else
	{
		switch (classid)
		{
		    case 0..14:
		    {
		        ShowNotification(playerid, "~w~You have chossen team ~r~Terrorists~w~. You mission is to eliminate ~b~Cops ~w~and capture all their terroteries!", 0);

		    	SetPlayerPos(playerid, 681.7634, -477.5455, 16.3359);
		     	SetPlayerFacingAngle(playerid, 179.5926);
		    	SetPlayerCameraPos(playerid, 681.3257, -489.8256, 19.6577);
				SetPlayerCameraLookAt(playerid, 681.3247, -488.8275, 19.5426, CAMERA_MOVE);

				SetPlayerTeam(playerid, TEAM_TERRORISTS);
		    }

		    default:
		    {
		        ShowNotification(playerid, "~w~You have chossen team ~b~Cops~w~. You mission is to eliminate ~r~Terrorists ~w~and capture all their terroteries!", 0);

		    	SetPlayerPos(playerid, 620.1630, -601.8186, 17.2330);
		     	SetPlayerFacingAngle(playerid, 270.1233);
		     	SetPlayerCameraPos(playerid, 634.4814, -602.4117, 21.7124);
				SetPlayerCameraLookAt(playerid, 633.4851, -602.4774, 21.5773, CAMERA_MOVE);

				SetPlayerTeam(playerid, TEAM_COPS);
		    }
		}
	}

	PlayerTextDrawHide(playerid, ptxtHelmet[playerid]);
	PlayerTextDrawHide(playerid, ptxtOffRadar[playerid][0]);
	PlayerTextDrawHide(playerid, ptxtOffRadar[playerid][1]);
 	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	if (!GetPVarInt(playerid, "LoggedIn"))
	{
	    GameTextForPlayer(playerid, "~n~~n~~n~~n~~r~Login/Register first, before spawning!", 3000, 3);
	    return 0;
	}

	new count[2];
	for (new i, j = GetPlayerPoolSize(); i <= j; i++)
	{
	    if (i != playerid && IsPlayerConnected(i))
	    {
			if (GetPlayerTeam(i) == TEAM_COPS)
			    count[0]++;
			else if (GetPlayerTeam(i) == TEAM_TERRORISTS)
			    count[1]++;
	    }
	}

	if ((GetPlayerTeam(playerid) == TEAM_COPS && count[0] > count[1]) || (GetPlayerTeam(playerid) == TEAM_TERRORISTS && count[1] > count[0]))
	{
	    GameTextForPlayer(playerid, "~n~~n~~n~~n~~r~Team Full!!", 2000, 3);
	    return 0;
	}

	SendClientMessage(playerid, COLOR_WHITE, "");
	SendClientMessage(playerid, COLOR_WHITE, "/kill - commit sucide");
	SendClientMessage(playerid, COLOR_WHITE, "/shop - open shop dialog");
	SendClientMessage(playerid, COLOR_WHITE, "/weapons - open weapons shop dialog");
	SendClientMessage(playerid, COLOR_WHITE, "/vehicles - open vehicles shop dialog");
	SendClientMessage(playerid, COLOR_WHITE, "/helmet - buy a headshot protection helmet");
	SendClientMessage(playerid, COLOR_WHITE, "/offradar - go invisible on radar/map for 2 minutes");
	SendClientMessage(playerid, COLOR_WHITE, "/gamble - make some money through betting");
	SendClientMessage(playerid, COLOR_WHITE, "/stats - player statistics");
	SendClientMessage(playerid, COLOR_WHITE, "/skills - player skills statistics");
	SendClientMessage(playerid, COLOR_WHITE, "/changeques - change your security question & answer");
	SendClientMessage(playerid, COLOR_WHITE, "/changepass - change your account password");
	return 1;
}

forward OnPlayerSpawnProtectonUpdate(playerid);
public OnPlayerSpawnProtectonUpdate(playerid)
{
	if (iPlayerProtection[playerid][0] == 0)
	{
	    DestroyDynamic3DTextLabel(iPlayerProtection3DText[playerid]);
		KillTimer(iPlayerProtection[playerid][1]);

	    iPlayerProtection[playerid][1] = SetTimerEx("OnPlayerSpawnProtectionEnd", 3000, false, "i", playerid);
		PlayerTextDrawSetString(playerid, ptxtProtection[playerid][0], "~r~Spawn protection has ended");
		PlayerTextDrawHide(playerid, ptxtProtection[playerid][1]);

 		SetPlayerHealth(playerid, 100.0);
 	   	SetPlayerArmour(playerid, 20.0);
		SendClientMessage(playerid, COLOR_TOMATO, "Spawn protection has ended!");

        iPlayerSpree[playerid] = 0;
		return;
	}

	new string[64];
	format(string, sizeof(string), "Spawn protected for 10 seconds...", iPlayerProtection[playerid][0]--);
	UpdateDynamic3DTextLabelText(iPlayerProtection3DText[playerid], COLOR_TOMATO, string);
	format(string, sizeof(string), "%is", iPlayerProtection[playerid][0]);
	PlayerTextDrawSetString(playerid, ptxtProtection[playerid][1], string);
}

forward OnPlayerSpawnProtectionEnd(playerid);
public OnPlayerSpawnProtectionEnd(playerid)
{
    iPlayerProtection[playerid][1] = -1;
	PlayerTextDrawHide(playerid, ptxtProtection[playerid][0]);
	PlayerTextDrawHide(playerid, ptxtProtection[playerid][1]);
}

public OnPlayerSpawn(playerid)
{
 	new h, m, s;
	gettime(h, m, s);
	#pragma unused s
	SetPlayerTime(playerid, h, m);
	SetPlayerWeather(playerid, iWeather);

	if (bPlayerGambleStarted[playerid])
	{
    	bPlayerGambleStarted[playerid] = false;

		PlayerTextDrawHide(playerid, ptxtGamble[playerid][0]);
	 	PlayerTextDrawHide(playerid, ptxtGamble[playerid][1]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][2]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][3]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][4]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][5]);
	}

	ShowNotification(playerid, "", 0);

    if (iPlayerProtection[playerid][0] > 0)
	{
        DestroyDynamic3DTextLabel(iPlayerProtection3DText[playerid]);
		KillTimer(iPlayerProtection[playerid][1]);
	}
    iPlayerProtection3DText[playerid] = CreateDynamic3DTextLabel("Spawn protected for 10 seconds...", COLOR_TOMATO, 0.0, 0.0, 0.5, 20.0, playerid, .testlos = 1);
    iPlayerProtection[playerid][0] = 10;
    iPlayerProtection[playerid][1] = SetTimerEx("OnPlayerSpawnProtectonUpdate", 1000, true, "i", playerid);

	PlayerTextDrawSetString(playerid, ptxtProtection[playerid][0], "Anti-Spawnkill Protection");
	PlayerTextDrawShow(playerid, ptxtProtection[playerid][0]);
	PlayerTextDrawSetString(playerid, ptxtProtection[playerid][1], "10s");
	PlayerTextDrawShow(playerid, ptxtProtection[playerid][1]);
	SetPlayerHealth(playerid, FLOAT_INFINITY);

	PlayerTextDrawShow(playerid, ptxtHelmet[playerid]);
	PlayerTextDrawShow(playerid, ptxtOffRadar[playerid][0]);
	PlayerTextDrawShow(playerid, ptxtOffRadar[playerid][1]);

	RemovePlayerAttachedObject(playerid, MAX_PLAYER_ATTACHED_OBJECTS - 1);
	bPlayerHelmet[playerid] = false;
	PlayerTextDrawSetString(playerid, ptxtHelmet[playerid], "/Helmet: ~r~OFF");

	if (iPlayerOffRadar[playerid][0] <= 0)
	{
		PlayerTextDrawSetString(playerid, ptxtOffRadar[playerid][0], "/Offradar: ~r~OFF");
		PlayerTextDrawSetString(playerid, ptxtOffRadar[playerid][1], "Invisibility on map for 2 minutes");
	}

    iPlayerHeadshotData[playerid][0] = INVALID_PLAYER_ID;
    iPlayerHeadshotData[playerid][1] = 0;

	ResetPlayerWeapons(playerid);
 	SetPlayerInterior(playerid, 0);
   	SetPlayerVirtualWorld(playerid, 0);
   	SetPlayerScore(playerid, floatround((eUser[playerid][e_USER_DEATHS] == 0) ? (0.0) : (floatdiv(eUser[playerid][e_USER_KILLS], eUser[playerid][e_USER_DEATHS]))));
	GivePlayerMoney(playerid, 100);

	new r;

	if (GetPlayerTeam(playerid) == TEAM_COPS)
	{
		r = random(sizeof(COPS_SPAWN));
	    SetPlayerPos(playerid, COPS_SPAWN[r][0], COPS_SPAWN[r][1], COPS_SPAWN[r][2]);
	   	SetPlayerFacingAngle(playerid, COPS_SPAWN[r][3]);

	   	SetPlayerColor(playerid, TEAM_COLOR_COPS);

		GivePlayerWeapon(playerid, 29, 200);
		GivePlayerWeapon(playerid, 24, 100);
		GivePlayerWeapon(playerid, 4, 1);

		SendClientMessage(playerid, COLOR_TOMATO, "___________________");
		SendClientMessage(playerid, COLOR_TOMATO, "");
		SendClientMessage(playerid, TEAM_COLOR_COPS, "TEAM: Cops");
		SendClientMessage(playerid, COLOR_TOMATO, "Your mission is to elimiate \"Terrorists\". Secondary objective is to capture all the three territories. When your team owns all 3 territories, you get cash advantages.");
		SendClientMessage(playerid, COLOR_TOMATO, "For list of commands, type /cmds.");
		SendClientMessage(playerid, COLOR_TOMATO, "");
		SendClientMessage(playerid, COLOR_TOMATO, "You have spawn protection for 10 seconds, if you shoot someone, it will end!");
		SendClientMessage(playerid, COLOR_TOMATO, "___________________");
 	}
 	else
	{
		r = random(sizeof(TERRORISTS_SPAWN));
	    SetPlayerPos(playerid, TERRORISTS_SPAWN[r][0], TERRORISTS_SPAWN[r][1], TERRORISTS_SPAWN[r][2]);
	   	SetPlayerFacingAngle(playerid, TERRORISTS_SPAWN[r][3]);

	   	SetPlayerColor(playerid, TEAM_COLOR_TERRORISTS);

		GivePlayerWeapon(playerid, 30, 200);
		GivePlayerWeapon(playerid, 22, 100);
		GivePlayerWeapon(playerid, 4, 1);

		SendClientMessage(playerid, COLOR_TOMATO, "___________________");
		SendClientMessage(playerid, COLOR_TOMATO, "");
		SendClientMessage(playerid, TEAM_COLOR_TERRORISTS, "TEAM: Terrorists");
		SendClientMessage(playerid, COLOR_TOMATO, "Your mission is to elimiate \"Cops\". Secondary objective is to capture all the three territories. When your team owns all 3 territories, you get cash advantages.");
		SendClientMessage(playerid, COLOR_TOMATO, "For list of commands, type /cmds.");
		SendClientMessage(playerid, COLOR_TOMATO, "");
		SendClientMessage(playerid, COLOR_TOMATO, "You have spawn protection for 10 seconds, if you shoot someone, it will end!");
		SendClientMessage(playerid, COLOR_TOMATO, "___________________");
 	}

    if (iPlayerOffRadar[playerid][0] > 0)
	{
	    GameTextForPlayer(playerid, "~w~You are still Off-Radar!", 3000, 3);
		SetPlayerColor(playerid, SET_ALPHA(((GetPlayerTeam(playerid) == TEAM_COPS) ? (TEAM_COLOR_COPS) : (TEAM_COLOR_TERRORISTS)), 0));
	}

	SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL, iPlayerSkills[playerid][WEAPONSKILL_PISTOL]);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_PISTOL_SILENCED, iPlayerSkills[playerid][WEAPONSKILL_PISTOL_SILENCED]);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_DESERT_EAGLE, iPlayerSkills[playerid][WEAPONSKILL_DESERT_EAGLE]);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_SHOTGUN, iPlayerSkills[playerid][WEAPONSKILL_SHOTGUN]);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, iPlayerSkills[playerid][WEAPONSKILL_SAWNOFF_SHOTGUN]);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_SPAS12_SHOTGUN, iPlayerSkills[playerid][WEAPONSKILL_SPAS12_SHOTGUN]);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_MICRO_UZI, iPlayerSkills[playerid][WEAPONSKILL_MICRO_UZI]);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_MP5, iPlayerSkills[playerid][WEAPONSKILL_MP5]);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_AK47, iPlayerSkills[playerid][WEAPONSKILL_AK47]);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_M4, iPlayerSkills[playerid][WEAPONSKILL_M4]);
	SetPlayerSkillLevel(playerid, WEAPONSKILL_SNIPERRIFLE, iPlayerSkills[playerid][WEAPONSKILL_SNIPERRIFLE]);

	for (new i; i < sizeof(eCaptureZone); i++)
	{
	    GangZoneShowForPlayer(playerid, eCaptureZone[i][e_CAPTURE_ZONE_ID], SET_ALPHA(((eCaptureZone[i][e_CAPTURE_ZONE_OWNER] == TEAM_COPS) ? (TEAM_COLOR_COPS) : (TEAM_COLOR_TERRORISTS)), 150));
	    if (eCaptureZone[i][e_CAPTURE_ZONE_ATTACKER] != INVALID_PLAYER_ID)
	    {
	        GangZoneFlashForPlayer(playerid, eCaptureZone[i][e_CAPTURE_ZONE_ID], SET_ALPHA(((GetPlayerTeam(playerid) == TEAM_COPS) ? (TEAM_COLOR_COPS) : (TEAM_COLOR_TERRORISTS)), 150));
	    }
	}
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	for (new i = 0; i < iStaticPickupCount; i++)
	{
	    if (pickupid == eStaticPickup[i][e_STATIC_PICKUP_ID])
	    {
			switch (eStaticPickup[i][e_STATIC_PICKUP_MODEL])
			{
			    case 331,
					 333..341,
					 321..326,
					 342..355,
					 372,
					 356..371:
				{
					GivePlayerWeapon(playerid, GetModelWeaponID(eStaticPickup[i][e_STATIC_PICKUP_MODEL]), _:eStaticPickup[i][e_STATIC_PICKUP_AMOUNT]);
				}

			    case 1242:
			    {
			        new Float:value;
			        GetPlayerArmour(playerid, value);
			        SetPlayerArmour(playerid, (((value + eStaticPickup[i][e_STATIC_PICKUP_AMOUNT]) >= 100.0) ? (100.0) : (value + eStaticPickup[i][e_STATIC_PICKUP_AMOUNT])));
			    }

			    case 1240:
			    {
			        new Float:value;
			        GetPlayerHealth(playerid, value);
			        SetPlayerHealth(playerid, (((value + eStaticPickup[i][e_STATIC_PICKUP_AMOUNT]) >= 100.0) ? (100.0) : (value + eStaticPickup[i][e_STATIC_PICKUP_AMOUNT])));
			    }

			    case 1212,
					 1550:
				{
					GivePlayerMoney(playerid, floatround(eStaticPickup[i][e_STATIC_PICKUP_AMOUNT]));
				}
			}

			StaticPickup_Destroy(i);
	    }
	}
	return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	if (GetPlayerTeam(playerid) != NO_TEAM && GetPlayerTeam(damagedid) != NO_TEAM && GetPlayerTeam(playerid) == GetPlayerTeam(damagedid))
	{
		GameTextForPlayer(playerid, "~r~Same Team!", 5000, 3);
	}
	else
	{
		if (iPlayerProtection[damagedid][0] > 0)
		{
		    GameTextForPlayer(playerid, "~r~Player under protection!", 5000, 3);
		    return 1;
		}

		if (iPlayerProtection[playerid][0] > 0)
		{
		    iPlayerProtection[playerid][0] = 0;
		    OnPlayerSpawnProtectonUpdate(playerid);
		}
	}
	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	if (issuerid != INVALID_PLAYER_ID)
	{
		PlayerPlaySound(issuerid, 17802, 0.0, 0.0, 0.0);

		if (bodypart == 9)
  		{
  		    if (weaponid == WEAPON_SILENCED || weaponid == WEAPON_MP5 || weaponid == WEAPON_AK47 || weaponid == WEAPON_M4 || weaponid == WEAPON_RIFLE || weaponid == WEAPON_SNIPER || weaponid == WEAPON_MINIGUN)
  		    {
				if (GetPlayerTeam(playerid) != NO_TEAM && GetPlayerTeam(issuerid) != NO_TEAM && GetPlayerTeam(playerid) != GetPlayerTeam(issuerid))
				{
					if (iPlayerProtection[playerid][0] > 0)
					{
					    GameTextForPlayer(issuerid, "~r~Player under protection!", 5000, 3);
					    return 1;
					}

				    if (bPlayerHelmet[playerid])
				    {
			        	return GameTextForPlayer(issuerid, "~n~~n~~n~~n~~n~~r~Player Has Helmet!", 5000, 3);
				    }

					PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
					PlayerPlaySound(issuerid, 1055, 0.0, 0.0, 0.0);
			        SetPlayerHealth(playerid, 0.0);
			        iPlayerHeadshotData[playerid][0] = issuerid;
	    			iPlayerHeadshotData[playerid][1] = weaponid;
			    }
			}
		}
	}
	return 1;
}

public OnPlayerText(playerid, text[])
{
	new string[150],
		name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	if (GetPlayerTeam(playerid) == TEAM_TERRORISTS)
	{
		format(string, sizeof(string), "sultan %s[%i]: "COL_WHITE"%s", name, playerid, text);
		SendClientMessageToAll(TEAM_COLOR_TERRORISTS, string);
	}
	else if (GetPlayerTeam(playerid) == TEAM_COPS)
	{
		format(string, sizeof(string), "officer %s[%i]: "COL_WHITE"%s", name, playerid, text);
		SendClientMessageToAll(TEAM_COLOR_COPS, string);
	}
	return 0;
}

public OnPlayerEnterDynamicCP(playerid, checkpointid)
{
	for (new i; i < sizeof(eCaptureZone); i++)
	{
		if (eCaptureZone[i][e_CAPTURE_ZONE_CPID] == checkpointid)
		{
			new string[150];
			if (eCaptureZone[i][e_CAPTURE_ZONE_ATTACKER] != INVALID_PLAYER_ID)
   			{
				if (GetPlayerTeam(playerid) == GetPlayerTeam(eCaptureZone[i][e_CAPTURE_ZONE_ATTACKER]))
				{
					if (IsPlayerInAnyVehicle(playerid))
						return SendClientMessage(playerid, COLOR_TOMATO, "You cannot capture the zone in a vehicle.");

					format(string, sizeof(string), "Capturing in %i/"#CAPTURE_TIME"...", eCaptureZone[i][e_CAPTURE_ZONE_TICK]);
					PlayerTextDrawSetString(playerid, ptxtCapture[playerid], string);
					PlayerTextDrawShow(playerid, ptxtCapture[playerid]);

				 	SetPlayerProgressBarValue(playerid, pbarCapture[playerid], eCaptureZone[i][e_CAPTURE_ZONE_TICK]);
					ShowPlayerProgressBar(playerid, pbarCapture[playerid]);

					eCaptureZone[i][e_CAPTURE_ZONE_PLAYERS]++;
					iPlayerCaptureZone[playerid] = i;

					SendClientMessage(playerid, COLOR_WHITE, "Stay in the checkpoint to assist your teammate in capturing the zone.");
				}
			}
			else
			{
 				if (GetPlayerTeam(playerid) != eCaptureZone[i][e_CAPTURE_ZONE_OWNER])
				{
					if (IsPlayerInAnyVehicle(playerid))
						return SendClientMessage(playerid, 0xFF0000FF, "ERROR: You cannot capture the zone in a vehicle.");

					format(string, sizeof(string), "The zone is controlled by team %s. Stay in the checkpoint for "#CAPTURE_TIME" seconds to capture the zone.", ((eCaptureZone[i][e_CAPTURE_ZONE_OWNER] == TEAM_COPS) ? ("Cops") : ("Terrorists")));
					SendClientMessage(playerid, COLOR_WHITE, string);

					GangZoneFlashForAll(eCaptureZone[i][e_CAPTURE_ZONE_ID], SET_ALPHA(((GetPlayerTeam(playerid) == TEAM_COPS) ? (TEAM_COLOR_COPS) : (TEAM_COLOR_TERRORISTS)), 150));

					iPlayerCaptureZone[playerid] = i;
					eCaptureZone[i][e_CAPTURE_ZONE_ATTACKER] = playerid;
					eCaptureZone[i][e_CAPTURE_ZONE_PLAYERS] = 1;
					eCaptureZone[i][e_CAPTURE_ZONE_TICK] = 0;

					KillTimer(eCaptureZone[i][e_CAPTURE_ZONE_TIMER]);
					eCaptureZone[i][e_CAPTURE_ZONE_TIMER] = SetTimerEx("OnZoneUpdate", 1000, true, "i", i);

					if (GetPlayerTeam(playerid) == TEAM_COPS)
					{
					    new copstr[150],
					        Terroriststr[150];
						format(copstr, sizeof(copstr), "~y~We have provoked ~r~Terrorists ~y~territory %s", eCaptureZone[i][e_CAPTURE_ZONE_NAME]);
						format(Terroriststr, sizeof(Terroriststr), "~b~Cops ~y~have provoked our territory %s", eCaptureZone[i][e_CAPTURE_ZONE_NAME]);

						for (new p, l = GetPlayerPoolSize(); p <= l; p++)
						{
						    if (GetPlayerTeam(p) == TEAM_COPS)
                    			ShowCaptureText(p, 1, copstr, 10000);
						    if (GetPlayerTeam(p) == TEAM_TERRORISTS)
                    			ShowCaptureText(p, 0, Terroriststr, 10000);
						}
					}
					else if (GetPlayerTeam(playerid) == TEAM_TERRORISTS)
					{
					    new copstr[150],
					        Terroriststr[150];
						format(copstr, sizeof(copstr), "~r~Terrorists ~y~have provoked our territory %s", eCaptureZone[i][e_CAPTURE_ZONE_NAME]);
						format(Terroriststr, sizeof(Terroriststr), "~y~We have provoked ~b~Cops ~y~territory %s", eCaptureZone[i][e_CAPTURE_ZONE_NAME]);

						for (new p, l = GetPlayerPoolSize(); p <= l; p++)
						{
						    if (GetPlayerTeam(p) == TEAM_COPS)
                    			ShowCaptureText(p, 0, copstr, 10000);
						    if (GetPlayerTeam(p) == TEAM_TERRORISTS)
                    			ShowCaptureText(p, 1, Terroriststr, 10000);
						}
					}

					PlayerTextDrawSetString(playerid, ptxtCapture[playerid], "Capturing in 1/"#CAPTURE_TIME"...");
					PlayerTextDrawShow(playerid, ptxtCapture[playerid]);

				 	SetPlayerProgressBarValue(playerid, pbarCapture[playerid], eCaptureZone[i][e_CAPTURE_ZONE_TICK]);
					ShowPlayerProgressBar(playerid, pbarCapture[playerid]);
				}
				else
				{
				    if (eCaptureZone[i][e_CAPTURE_ZONE_ATTACKER] != INVALID_PLAYER_ID)
				    	GameTextForPlayer(playerid, "~r~~h~~h~~h~Protect zone from enemies!", 2000, 1);
				    else
				    	GameTextForPlayer(playerid, "~r~~h~~h~~h~Zone secure!", 2000, 1);
				}
			}

			break;
		}
	}
	return 1;
}

forward OnZoneUpdate(zoneid);
public OnZoneUpdate(zoneid)
{
	switch(eCaptureZone[zoneid][e_CAPTURE_ZONE_PLAYERS])
	{
 		case 1:
			eCaptureZone[zoneid][e_CAPTURE_ZONE_TICK] += 1;
 		case 2:
			eCaptureZone[zoneid][e_CAPTURE_ZONE_TICK] += 2;
		default:
			eCaptureZone[zoneid][e_CAPTURE_ZONE_TICK] += 3;
	}

	new string[150];
	format(string, sizeof(string), "Capturing in %i/"#CAPTURE_TIME"...", eCaptureZone[zoneid][e_CAPTURE_ZONE_TICK]);
	for (new i, j = GetPlayerPoolSize(); i <= j; i++)
	{
 		if (IsPlayerInDynamicCP(i, eCaptureZone[zoneid][e_CAPTURE_ZONE_CPID]) && !IsPlayerInAnyVehicle(i) && GetPlayerTeam(i) == GetPlayerTeam(eCaptureZone[zoneid][e_CAPTURE_ZONE_ATTACKER]))
		{
			SetPlayerProgressBarValue(i, pbarCapture[i], eCaptureZone[zoneid][e_CAPTURE_ZONE_TICK]);
			PlayerTextDrawSetString(i, ptxtCapture[i], string);
  		}
	}

	if (eCaptureZone[zoneid][e_CAPTURE_ZONE_TICK] > CAPTURE_TIME)
	{
		SendClientMessage(eCaptureZone[zoneid][e_CAPTURE_ZONE_ATTACKER], COLOR_GREEN, "You have successfully captured the zone, +$1000.");
		GivePlayerMoney(eCaptureZone[zoneid][e_CAPTURE_ZONE_ATTACKER], 1000);

		for (new i, j = GetPlayerPoolSize(); i <= j; i++)
		{
			if (IsPlayerInDynamicCP(i, eCaptureZone[zoneid][e_CAPTURE_ZONE_CPID]))
			{
				PlayerTextDrawHide(i, ptxtCapture[i]);
				HidePlayerProgressBar(i, pbarCapture[i]);

				if (i != eCaptureZone[zoneid][e_CAPTURE_ZONE_ATTACKER] && GetPlayerTeam(i) == GetPlayerTeam(eCaptureZone[zoneid][e_CAPTURE_ZONE_ATTACKER]) && !IsPlayerInAnyVehicle(i))
				{
					SendClientMessage(i, COLOR_GREEN, "You have assisted your teammate to capture the zone, +$500.");
					GivePlayerMoney(i, 500);
				}
			}
		}

 		GangZoneStopFlashForAll(eCaptureZone[zoneid][e_CAPTURE_ZONE_ID]);
 		GangZoneShowForAll(eCaptureZone[zoneid][e_CAPTURE_ZONE_ID], SET_ALPHA(((GetPlayerTeam(eCaptureZone[zoneid][e_CAPTURE_ZONE_ATTACKER]) == TEAM_COPS) ? (TEAM_COLOR_COPS) : (TEAM_COLOR_TERRORISTS)), 150));

		KillTimer(eCaptureZone[zoneid][e_CAPTURE_ZONE_TIMER]);

		if (GetPlayerTeam(eCaptureZone[zoneid][e_CAPTURE_ZONE_ATTACKER]) == TEAM_COPS)
		{
		    new copstr[150],
      			Terroriststr[150];
			format(copstr, sizeof(copstr), "~g~We have taken over the territory %s from ~r~Terrorists", eCaptureZone[zoneid][e_CAPTURE_ZONE_NAME]);
			format(Terroriststr, sizeof(Terroriststr), "~b~Cops ~r~have taken over our territory %s", eCaptureZone[zoneid][e_CAPTURE_ZONE_NAME]);

			for (new i, j = GetPlayerPoolSize(); i <= j; i++)
			{
   				if (GetPlayerTeam(i) == TEAM_COPS)
                    ShowCaptureText(i, 0, copstr, 10000);
				if (GetPlayerTeam(i) == TEAM_TERRORISTS)
                    ShowCaptureText(i, 1, Terroriststr, 10000);
			}
		}
		else if (GetPlayerTeam(eCaptureZone[zoneid][e_CAPTURE_ZONE_ATTACKER]) == TEAM_TERRORISTS)
		{
		    new copstr[150],
      			Terroriststr[150];
			format(copstr, sizeof(copstr), "~r~Terrorists ~r~have taken over our territory %s", eCaptureZone[zoneid][e_CAPTURE_ZONE_NAME]);
			format(Terroriststr, sizeof(Terroriststr), "~g~We have taken over the territory %s from ~b~Cops", eCaptureZone[zoneid][e_CAPTURE_ZONE_NAME]);

			for (new i, j = GetPlayerPoolSize(); i <= j; i++)
			{
   				if (GetPlayerTeam(i) == TEAM_COPS)
                    ShowCaptureText(i, 1, copstr, 10000);
				if (GetPlayerTeam(i) == TEAM_TERRORISTS)
                    ShowCaptureText(i, 0, Terroriststr, 10000);
			}
		}

		eCaptureZone[zoneid][e_CAPTURE_ZONE_OWNER] = GetPlayerTeam(eCaptureZone[zoneid][e_CAPTURE_ZONE_ATTACKER]);
		eCaptureZone[zoneid][e_CAPTURE_ZONE_ATTACKER] = INVALID_PLAYER_ID;
	}
}

public OnPlayerLeaveDynamicCP(playerid, checkpointid)
{
	for (new i; i < sizeof(eCaptureZone); i++)
	{
		if (eCaptureZone[i][e_CAPTURE_ZONE_CPID] == checkpointid)
		{
			if (eCaptureZone[i][e_CAPTURE_ZONE_ATTACKER] != INVALID_PLAYER_ID)
   			{
				if (GetPlayerTeam(playerid) == GetPlayerTeam(eCaptureZone[i][e_CAPTURE_ZONE_ATTACKER]) && iPlayerCaptureZone[playerid] == i)
				{
				    iPlayerCaptureZone[playerid] = -1;
   					eCaptureZone[i][e_CAPTURE_ZONE_PLAYERS]--;

                    if (!eCaptureZone[i][e_CAPTURE_ZONE_PLAYERS])
                   	{
						SendClientMessage(playerid, COLOR_TOMATO, "You failed to capture the zone, there were no teammates left in your checkpoint.");

	                    GangZoneStopFlashForAll(eCaptureZone[i][e_CAPTURE_ZONE_ID]);

						if (GetPlayerTeam(eCaptureZone[i][e_CAPTURE_ZONE_ATTACKER]) == TEAM_COPS)
						{
						    new copstr[150],
				      			Terroriststr[150];
							format(copstr, sizeof(copstr), "~r~We failed to take over the territory %s from ~r~Terrorists", eCaptureZone[i][e_CAPTURE_ZONE_NAME]);
							format(Terroriststr, sizeof(Terroriststr), "~b~Cops ~g~failed to take over our territory %s", eCaptureZone[i][e_CAPTURE_ZONE_NAME]);

							for (new p, l = GetPlayerPoolSize(); p <= l; p++)
							{
				   				if (GetPlayerTeam(p) == TEAM_COPS)
                    				ShowCaptureText(p, 1, copstr, 10000);
								if (GetPlayerTeam(p) == TEAM_TERRORISTS)
                    				ShowCaptureText(p, 0, Terroriststr, 10000);
							}
						}
						else if (GetPlayerTeam(eCaptureZone[i][e_CAPTURE_ZONE_ATTACKER]) == TEAM_TERRORISTS)
						{
						    new copstr[150],
				      			Terroriststr[150];
							format(copstr, sizeof(copstr), "~r~Terrorists ~g~failed to take over our territory %s", eCaptureZone[i][e_CAPTURE_ZONE_NAME]);
							format(Terroriststr, sizeof(Terroriststr), "~r~We failed to take over the territory %s from ~b~Cops", eCaptureZone[i][e_CAPTURE_ZONE_NAME]);

							for (new p, l = GetPlayerPoolSize(); p <= l; p++)
							{
				   				if (GetPlayerTeam(p) == TEAM_COPS)
                    				ShowCaptureText(p, 0, copstr, 10000);
								if (GetPlayerTeam(p) == TEAM_TERRORISTS)
                    				ShowCaptureText(p, 1, Terroriststr, 10000);
							}
						}

						eCaptureZone[i][e_CAPTURE_ZONE_ATTACKER] = INVALID_PLAYER_ID;
						KillTimer(eCaptureZone[i][e_CAPTURE_ZONE_TIMER]);
    				}
					else if (eCaptureZone[i][e_CAPTURE_ZONE_ATTACKER] == playerid)
					{
						for (new p, l = GetPlayerPoolSize(); p <= l; p++)
						{
					   		if (GetPlayerTeam(p) == GetPlayerTeam(playerid))
					   	    {
						   		if (IsPlayerInDynamicCP(p, checkpointid))
						   	 	{
           							eCaptureZone[i][e_CAPTURE_ZONE_ATTACKER] = p;
					   	            break;
	   	            			}
					   		}
					 	}
					}
				}

	  			PlayerTextDrawHide(playerid, ptxtCapture[playerid]);
				HidePlayerProgressBar(playerid, pbarCapture[playerid]);
				break;
			}
		}
	}

	return 1;
}

public OnVehicleDeath(vehicleid)
{
	for (new i, j = GetPlayerPoolSize(); i <= j; i++)
	{
	    if (IsPlayerConnected(i))
	    {
			for (new x; x < sizeof(iPlayerVehicleid[]); x++)
			{
			    if (IsValidVehicle(iPlayerVehicleid[i][x]) && iPlayerVehicleid[i][x] == vehicleid)
			    {
			        DestroyVehicle(iPlayerVehicleid[i][x]);
			        break;
			    }
			}
		}
	}
	return 1;
}

CMD:changepass(playerid, params[])
{
	if (bPlayerGambleStarted[playerid])
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use commands while gambling.");

	if (eUser[playerid][e_USER_SQLID] != 1)
	{
		SendClientMessage(playerid, COLOR_TOMATO, "Only registered users can use this command.");
		return 1;
	}

    Dialog_Show(playerid, CHANGE_PASSWORD, DIALOG_STYLE_PASSWORD, "Change account password...", COL_WHITE "Insert a new password for your account, Passwords are "COL_YELLOW"case sensitive"COL_WHITE".", "Confirm", "Cancel");
	SendClientMessage(playerid, COLOR_WHITE, "Enter your new password.");
	PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:CHANGE_PASSWORD(playerid, response, listitem, inputtext[])
{
	if (!response)
		return 1;

	if (!(dini_Int(DIRECTORY "config.ini", "min_password_length") <= strlen(inputtext) <= dini_Int(DIRECTORY "config.ini", "max_password_length")))
	{
	    Dialog_Show(playerid, CHANGE_PASSWORD, DIALOG_STYLE_PASSWORD, "Change account password...", COL_WHITE "Insert a new password for your account, Passwords are "COL_YELLOW"case sensitive"COL_WHITE".", "Confirm", "Cancel");

		new string[150];
		format(string, sizeof(string), "Invalid password length, must be between %i - %i characters.", dini_Int(DIRECTORY "config.ini", "min_password_length"), dini_Int(DIRECTORY "config.ini", "max_password_length"));
		SendClientMessage(playerid, COLOR_TOMATO, string);
	    return 1;
	}

    if (dini_Bool(DIRECTORY "config.ini", "toggle_secure_password"))
    {
		if (!IsPasswordSecure(inputtext))
		{
		    Dialog_Show(playerid, CHANGE_PASSWORD, DIALOG_STYLE_INPUT, "Change account password...", COL_WHITE "Insert a new password for your account, Passwords are "COL_YELLOW"case sensitive"COL_WHITE".", "Confirm", "Cancel");
			SendClientMessage(playerid, COLOR_TOMATO, "Password must contain atleast a Highercase, a Lowercase and a Number.");
		    return 1;
		}
	}

	SHA256_PassHash(inputtext, eUser[playerid][e_USER_SALT], eUser[playerid][e_USER_PASSWORD], 64);

	new string[256];
	for (new i, j = strlen(inputtext); i < j; i++)
	{
	    inputtext[i] = '*';
	}
	format(string, sizeof(string), "Successfully changed your password. [P: %s]", inputtext);
	SendClientMessage(playerid, COLOR_GREEN, string);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}

CMD:changeques(playerid, params[])
{
	if (bPlayerGambleStarted[playerid])
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use commands while gambling.");

	if (eUser[playerid][e_USER_SQLID] != 1)
	{
		SendClientMessage(playerid, COLOR_TOMATO, "Only registered users can use this command.");
		return 1;
	}

    new line[MAX_SECURITY_QUESTION_SIZE],
		info[MAX_SECURITY_QUESTIONS * MAX_SECURITY_QUESTION_SIZE],
		File:h,
		count;
	h = fopen(DIRECTORY "questions.ini", io_read);
	while (fread(h, line))
	{
	    strcat(info, line);
	    strcat(info, "\n");

	    if (++count >= MAX_SECURITY_QUESTIONS)
	        break;
	}
	fclose(h);

	Dialog_Show(playerid, CHANGE_SEC_QUESTION, DIALOG_STYLE_LIST, "Change account security question... [Step: 1/2]", info, "Continue", "Cancel");
	SendClientMessage(playerid, COLOR_WHITE, "[Step: 1/2] Select a security question. This will help you retrieve your password in case you forget it any time soon!");
	PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:CHANGE_SEC_QUESTION(playerid, response, listitem, inputext[])
{
	if (!response)
		return 1;

	SetPVarString(playerid, "Question", inputext);

	new string[256];
	format(string, sizeof(string), COL_YELLOW "%s\n"COL_WHITE"Insert your answer below in the box. (don't worry about CAPS, answers are NOT case sensitive).", inputext);
	Dialog_Show(playerid, CHANGE_SEC_ANSWER, DIALOG_STYLE_INPUT, "Change account security question... [Step: 2/2]", string, "Confirm", "Back");
	SendClientMessage(playerid, COLOR_WHITE, "[Step: 2/2] Write the answer to your secuirty question.");
	PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:CHANGE_SEC_ANSWER(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
		new line[MAX_SECURITY_QUESTION_SIZE],
			info[MAX_SECURITY_QUESTIONS * MAX_SECURITY_QUESTION_SIZE],
			File:h,
			count;
		h = fopen(DIRECTORY "questions.ini", io_read);
		while (fread(h, line))
		{
		    strcat(info, line);
		    strcat(info, "\n");

		    if (++count >= MAX_SECURITY_QUESTIONS)
		        break;
		}
		fclose(h);

		Dialog_Show(playerid, CHANGE_SEC_QUESTION, DIALOG_STYLE_LIST, "Change account security question... [Step: 1/2]", info, "Continue", "Cancel");
		SendClientMessage(playerid, COLOR_WHITE, "[Step: 1/2] Select a security question. This will help you retrieve your password in case you forget it any time soon!");
		return 1;
	}

	new string[512],
	    question[MAX_SECURITY_QUESTION_SIZE];
    GetPVarString(playerid, "Question", question, MAX_SECURITY_QUESTION_SIZE);

	if (strlen(inputtext) < dini_Int(DIRECTORY "config.ini", "min_password_length") || inputtext[0] == ' ')
	{
	    format(string, sizeof(string), COL_YELLOW "%s\n"COL_WHITE"Insert your answer below in the box. (don't worry about CAPS, answers are NOT case sensitive).", question);
		Dialog_Show(playerid, CHANGE_SEC_ANSWER, DIALOG_STYLE_INPUT, "Change account security question... [Step: 2/2]", string, "Confirm", "Back");

		format(string, sizeof(string), "Security answer cannot be an less than %i characters.", dini_Int(DIRECTORY "config.ini", "min_password_length"));
		SendClientMessage(playerid, COLOR_TOMATO, string);
		return 1;
	}

	format(eUser[playerid][e_USER_SECURITY_QUESTION], MAX_SECURITY_QUESTION_SIZE, question);
	DeletePVar(playerid, "Question");

	for (new i, j = strlen(inputtext); i < j; i++)
	{
        inputtext[i] = tolower(inputtext[i]);
	}
	SHA256_PassHash(inputtext, eUser[playerid][e_USER_SALT], eUser[playerid][e_USER_SECURITY_ANSWER], 64);
	format(string, sizeof(string), "Successfully changed your security answer and question [Q: %s].", eUser[playerid][e_USER_SECURITY_QUESTION]);
	SendClientMessage(playerid, COLOR_GREEN, string);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}

CMD:stats(playerid, params[])
{
	new targetid,
		bool:showtip;
	if (sscanf(params, "u", targetid))
	{
  		targetid = playerid;
  		showtip = true;
	}

	if (!IsPlayerConnected(targetid))
		return SendClientMessage(playerid, COLOR_TOMATO, "The player is no more connected.");

	new name[MAX_PLAYER_NAME];
	GetPlayerName(targetid, name, MAX_PLAYER_NAME);

	new string[256];
	SendClientMessage(playerid, COLOR_GREEN, "");
	format(string, sizeof(string), "%s[%i]'s stats: (AccountId: %i)", name, targetid, eUser[targetid][e_USER_SQLID]);
	SendClientMessage(playerid, COLOR_GREEN, string);

	new Float:ratio = ((eUser[targetid][e_USER_DEATHS] == 0) ? (0.0) : (floatdiv(eUser[targetid][e_USER_KILLS], eUser[targetid][e_USER_DEATHS])));

	static levelname[6][25];
	if (!levelname[0][0])
	{
		levelname[0] = "Player";
		levelname[1] = "Operator";
		levelname[2] = "Moderator";
		levelname[3] = "Administrator";
		levelname[4] = "Manager";
		levelname[5] = "Owner/RCON";
	}

	format(string, sizeof(string), "Money: $%i || Kills: %i || Deaths: %i || Ratio: %0.2f || Admin Level: %i - %s || Vip Level: %i || Registered: %s ago || Last Seen: %s ago",
		GetPlayerMoney(targetid), eUser[targetid][e_USER_KILLS], eUser[targetid][e_USER_DEATHS], ratio, eUser[targetid][e_USER_ADMIN_LEVEL], levelname[((eUser[targetid][e_USER_ADMIN_LEVEL] > 5) ? (5) : (eUser[targetid][e_USER_ADMIN_LEVEL]))], eUser[targetid][e_USER_VIP_LEVEL], ReturnTimelapse(eUser[playerid][e_USER_REGISTER_TIMESTAMP], gettime()), ReturnTimelapse(eUser[playerid][e_USER_LASTLOGIN_TIMESTAMP], gettime()));
	SendClientMessage(playerid, COLOR_GREEN, string);

	if (showtip)
		SendClientMessage(playerid, COLOR_DEFAULT, "Tip: You can also view other players stats by /stats [player]");
	return 1;
}

CMD:skills(playerid, params[])
{
	new targetid,
		bool:showtip;
	if (sscanf(params, "u", targetid))
	{
  		targetid = playerid;
  		showtip = true;
	}

	if (!IsPlayerConnected(targetid))
		return SendClientMessage(playerid, COLOR_TOMATO, "The player is no more connected.");

	new name[MAX_PLAYER_NAME];
	GetPlayerName(targetid, name, MAX_PLAYER_NAME);

	new string[150];
	SendClientMessage(playerid, COLOR_GREEN, "");
	format(string, sizeof(string), "%s[%i]'s skills:", name, targetid);
	SendClientMessage(playerid, COLOR_GREEN, string);

	format(string, sizeof(string), "9mm: %i/1000 || Silenced 9mm: %i/1000 || Desert Eagle: %i/1000 || Shotgun: %i/1000 || Sawnoff Shotgun: %i/1000 || Spas12 Shotgun: %i/1000",
		iPlayerSkills[targetid][WEAPONSKILL_PISTOL], iPlayerSkills[targetid][WEAPONSKILL_PISTOL_SILENCED], iPlayerSkills[targetid][WEAPONSKILL_DESERT_EAGLE], iPlayerSkills[targetid][WEAPONSKILL_SHOTGUN], iPlayerSkills[targetid][WEAPONSKILL_SAWNOFF_SHOTGUN], iPlayerSkills[targetid][WEAPONSKILL_SPAS12_SHOTGUN]);
	SendClientMessage(playerid, COLOR_GREEN, string);

	format(string, sizeof(string), "Micro-UZI: %i/1000 || MP5: %i/1000 || Ak47: %i/1000 || M4: %i/1000 || Sniper Rifle: %i/1000",
		iPlayerSkills[targetid][WEAPONSKILL_MICRO_UZI], iPlayerSkills[targetid][WEAPONSKILL_MP5], iPlayerSkills[targetid][WEAPONSKILL_AK47], iPlayerSkills[targetid][WEAPONSKILL_M4], iPlayerSkills[targetid][WEAPONSKILL_SNIPERRIFLE]);
	SendClientMessage(playerid, COLOR_GREEN, string);

	if (showtip)
		SendClientMessage(playerid, COLOR_DEFAULT, "Tip: You can also view other players skills by /skills [player]");
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	if (bPlayerGambleStarted[playerid])
	{
    	bPlayerGambleStarted[playerid] = false;

		PlayerTextDrawHide(playerid, ptxtGamble[playerid][0]);
	 	PlayerTextDrawHide(playerid, ptxtGamble[playerid][1]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][2]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][3]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][4]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][5]);
	}

    if (iPlayerProtection[playerid][0] > 0)
	{
 		DestroyDynamic3DTextLabel(iPlayerProtection3DText[playerid]);
		KillTimer(iPlayerProtection[playerid][1]);
		PlayerTextDrawHide(playerid, ptxtProtection[playerid][0]);
		PlayerTextDrawHide(playerid, ptxtProtection[playerid][1]);
	}

	if (iPlayerHeadshotData[playerid][0] != INVALID_PLAYER_ID)
	{
	    killerid = iPlayerHeadshotData[playerid][0];
    	reason = iPlayerHeadshotData[playerid][1];
    }

	if (iPlayerCaptureZone[playerid] != -1)
	{
	    OnPlayerLeaveDynamicCP(playerid, eCaptureZone[iPlayerCaptureZone[playerid]][e_CAPTURE_ZONE_CPID]);
	    iPlayerCaptureZone[playerid] = -1;
	}

    eUser[playerid][e_USER_DEATHS]++;
	SetPlayerScore(playerid, floatround((eUser[playerid][e_USER_DEATHS] == 0) ? (0.0) : (floatdiv(eUser[playerid][e_USER_KILLS], eUser[playerid][e_USER_DEATHS]))));

	new const Float:DIF[] =
	{
		1.2, 1.3, 1.5, 1.7, 2.0, 2.1, 2.4, 2.5, 2.7, 3.0
	};

    new weapon,
		ammo,
		Float:x,
		Float:y,
		Float:z;
	GetPlayerPos(playerid, x, y, z);
	for (new i; i < 13; i++)
	{
		GetPlayerWeaponData(playerid, i, weapon, ammo);
		switch (weapon)
		{
		    case 1..37:
			{
				if (weapon != 0 && ammo != 0)
					StaticPickup_Create((x + DIF[random(sizeof(DIF))]), (y + DIF[random(sizeof(DIF))]), z, GetWeaponModelID(weapon), Float:ammo, GetPlayerVirtualWorld(playerid), (2 * 60 * 1000));
			}
		}
	}

	if (killerid != INVALID_PLAYER_ID)
    {
		ShowDeathText(playerid, killerid, 7000, ((iPlayerHeadshotData[playerid][0] == INVALID_PLAYER_ID) ? (false) : (true)));

        new money,
			string[150];
		if (GetPlayerMoney(playerid) > 500)
		{
		    new const Float:MONEYDIF[] =
		    {
		        1.7, 1.9, 2.0, 2.3, 2.5, 2.7, 3.0
		    };
			money = floatround(float(GetPlayerMoney(playerid)) / MONEYDIF[random(sizeof(MONEYDIF))]);
			StaticPickup_Create((x + DIF[random(sizeof(DIF))]), (y + DIF[random(sizeof(DIF))]), z, (money < 10000) ? (1212) : (1550), Float:money, GetPlayerVirtualWorld(playerid), (10 * 60 * 1000));
			GivePlayerMoney(playerid, -money);

			format(string, sizeof(string), "You lost $%i for dying!You maybe lucky if someone didn't picked your money yet!", money);
			SendClientMessage(playerid, COLOR_TOMATO, string);
		}

		switch (random(10))
		{
	 		case 0: StaticPickup_Create((x + DIF[random(sizeof(DIF))]), (y + DIF[random(sizeof(DIF))]), z, 1212, 50.0, GetPlayerVirtualWorld(playerid), (1 * 60 * 1000));
			case 1: StaticPickup_Create((x + DIF[random(sizeof(DIF))]), (y + DIF[random(sizeof(DIF))]), z, 1240, 50.0, GetPlayerVirtualWorld(playerid), (1 * 60 * 1000));
		}

    	eUser[killerid][e_USER_KILLS]++;
  		SetPlayerScore(killerid, floatround((eUser[killerid][e_USER_DEATHS] == 0) ? (0.0) : (floatdiv(eUser[killerid][e_USER_KILLS], eUser[killerid][e_USER_DEATHS]))));

		money = (random(700 - 500) + 500);
		GivePlayerMoney(killerid, money);

		new name[MAX_PLAYER_NAME],
			weapon_name[35];
		GetPlayerName(playerid, name, MAX_PLAYER_NAME);
		switch (reason)
		{
			case 22:
			{
				if (iPlayerSkills[killerid][WEAPONSKILL_PISTOL] != 1000)
				{
					iPlayerSkills[killerid][WEAPONSKILL_PISTOL] += 50;
					SetPlayerSkillLevel(killerid, WEAPONSKILL_PISTOL, iPlayerSkills[killerid][WEAPONSKILL_PISTOL]);
				}

				GetWeaponName(reason, weapon_name, sizeof(weapon_name));
				format(string, sizeof(string), "Good job! You killed %s[%i] and gained $%i. Your weapon skill for %s is now %i/1000.", name, playerid, money, weapon_name, iPlayerSkills[killerid][WEAPONSKILL_PISTOL]);
				SendClientMessage(killerid, COLOR_GREEN, string);
			}

			case 23:
			{
				if (iPlayerSkills[killerid][WEAPONSKILL_PISTOL_SILENCED] != 1000)
				{
					iPlayerSkills[killerid][WEAPONSKILL_PISTOL_SILENCED] += 50;
					SetPlayerSkillLevel(killerid, WEAPONSKILL_PISTOL_SILENCED, iPlayerSkills[killerid][WEAPONSKILL_PISTOL_SILENCED]);
				}

				GetWeaponName(reason, weapon_name, sizeof(weapon_name));
				format(string, sizeof(string), "Good job! You killed %s[%i] and gained $%i. Your weapon skill for %s is now %i/1000.", name, playerid, money, weapon_name, iPlayerSkills[killerid][WEAPONSKILL_PISTOL_SILENCED]);
				SendClientMessage(killerid, COLOR_GREEN, string);
			}

			case 24:
			{
				if (iPlayerSkills[killerid][WEAPONSKILL_DESERT_EAGLE] != 1000)
				{
					iPlayerSkills[killerid][WEAPONSKILL_DESERT_EAGLE] += 50;
					SetPlayerSkillLevel(killerid, WEAPONSKILL_DESERT_EAGLE, iPlayerSkills[killerid][WEAPONSKILL_DESERT_EAGLE]);
				}

				GetWeaponName(reason, weapon_name, sizeof(weapon_name));
				format(string, sizeof(string), "Good job! You killed %s[%i] and gained $%i. Your weapon skill for %s is now %i/1000.", name, playerid, money, weapon_name, iPlayerSkills[killerid][WEAPONSKILL_DESERT_EAGLE]);
				SendClientMessage(killerid, COLOR_GREEN, string);
			}

			case 25:
			{
				if (iPlayerSkills[killerid][WEAPONSKILL_SHOTGUN] != 1000)
				{
					iPlayerSkills[killerid][WEAPONSKILL_SHOTGUN] += 50;
					SetPlayerSkillLevel(killerid, WEAPONSKILL_SHOTGUN, iPlayerSkills[killerid][WEAPONSKILL_SHOTGUN]);
				}

				GetWeaponName(reason, weapon_name, sizeof(weapon_name));
				format(string, sizeof(string), "Good job! You killed %s[%i] and gained $%i. Your weapon skill for %s is now %i/1000.", name, playerid, money, weapon_name, iPlayerSkills[killerid][WEAPONSKILL_SHOTGUN]);
				SendClientMessage(killerid, COLOR_GREEN, string);
			}

			case 26:
			{
				if (iPlayerSkills[killerid][WEAPONSKILL_SAWNOFF_SHOTGUN] != 1000)
				{
					iPlayerSkills[killerid][WEAPONSKILL_SAWNOFF_SHOTGUN] += 50;
					SetPlayerSkillLevel(killerid, WEAPONSKILL_SAWNOFF_SHOTGUN, iPlayerSkills[killerid][WEAPONSKILL_SAWNOFF_SHOTGUN]);
				}

				GetWeaponName(reason, weapon_name, sizeof(weapon_name));
				format(string, sizeof(string), "Good job! You killed %s[%i] and gained $%i. Your weapon skill for %s is now %i/1000.", name, playerid, money, weapon_name, iPlayerSkills[killerid][WEAPONSKILL_SAWNOFF_SHOTGUN]);
				SendClientMessage(playerid, COLOR_GREEN, string);
			}

			case 27:
			{
				if (iPlayerSkills[killerid][WEAPONSKILL_SPAS12_SHOTGUN] != 1000)
				{
					iPlayerSkills[killerid][WEAPONSKILL_SPAS12_SHOTGUN] += 50;
					SetPlayerSkillLevel(killerid, WEAPONSKILL_SPAS12_SHOTGUN, iPlayerSkills[killerid][WEAPONSKILL_SPAS12_SHOTGUN]);
				}

				GetWeaponName(reason, weapon_name, sizeof(weapon_name));
				format(string, sizeof(string), "Good job! You killed %s[%i] and gained $%i. Your weapon skill for %s is now %i/1000.", name, playerid, money, weapon_name, iPlayerSkills[killerid][WEAPONSKILL_SPAS12_SHOTGUN]);
				SendClientMessage(killerid, COLOR_GREEN, string);
			}

			case 28, 32:
			{
				if (iPlayerSkills[killerid][WEAPONSKILL_MICRO_UZI] != 1000)
				{
					iPlayerSkills[killerid][WEAPONSKILL_MICRO_UZI] += 50;
					SetPlayerSkillLevel(killerid, WEAPONSKILL_MICRO_UZI, iPlayerSkills[killerid][WEAPONSKILL_MICRO_UZI]);
				}

				GetWeaponName(reason, weapon_name, sizeof(weapon_name));
				format(string, sizeof(string), "Good job! You killed %s[%i] and gained $%i. Your weapon skill for %s is now %i/1000.", name, playerid, money, weapon_name, iPlayerSkills[killerid][WEAPONSKILL_MICRO_UZI]);
				SendClientMessage(killerid, COLOR_GREEN, string);
			}

			case 29:
			{
				if (iPlayerSkills[killerid][WEAPONSKILL_MP5] != 1000)
				{
					iPlayerSkills[killerid][WEAPONSKILL_MP5] += 50;
					SetPlayerSkillLevel(killerid, WEAPONSKILL_MP5, iPlayerSkills[killerid][WEAPONSKILL_MP5]);
				}

				GetWeaponName(reason, weapon_name, sizeof(weapon_name));
				format(string, sizeof(string), "Good job! You killed %s[%i] and gained $%i. Your weapon skill for %s is now %i/1000.", name, playerid, money, weapon_name, iPlayerSkills[killerid][WEAPONSKILL_MP5]);
				SendClientMessage(killerid, COLOR_GREEN, string);
			}

			case 30:
			{
				if (iPlayerSkills[killerid][WEAPONSKILL_AK47] != 1000)
				{
					iPlayerSkills[killerid][WEAPONSKILL_AK47] += 50;
					SetPlayerSkillLevel(killerid, WEAPONSKILL_AK47, iPlayerSkills[killerid][WEAPONSKILL_AK47]);
				}

				GetWeaponName(reason, weapon_name, sizeof(weapon_name));
				format(string, sizeof(string), "Good job! You killed %s[%i] and gained $%i. Your weapon skill for %s is now %i/1000.", name, playerid, money, weapon_name, iPlayerSkills[killerid][WEAPONSKILL_AK47]);
				SendClientMessage(killerid, COLOR_GREEN, string);
			}

			case 31:
			{
				if (iPlayerSkills[killerid][WEAPONSKILL_M4] != 1000)
				{
					iPlayerSkills[killerid][WEAPONSKILL_M4] += 50;
					SetPlayerSkillLevel(killerid, WEAPONSKILL_M4, iPlayerSkills[killerid][WEAPONSKILL_M4]);
				}

				GetWeaponName(reason, weapon_name, sizeof(weapon_name));
				format(string, sizeof(string), "Good job! You killed %s[%i] and gained $%i. Your weapon skill for %s is now %i/1000.", name, playerid, money, weapon_name, iPlayerSkills[killerid][WEAPONSKILL_M4]);
				SendClientMessage(killerid, COLOR_GREEN, string);
			}

			case 34:
			{
				if (iPlayerSkills[killerid][WEAPONSKILL_SNIPERRIFLE] != 1000)
				{
					iPlayerSkills[killerid][WEAPONSKILL_SNIPERRIFLE] += 50;
					SetPlayerSkillLevel(killerid, WEAPONSKILL_SNIPERRIFLE, iPlayerSkills[killerid][WEAPONSKILL_SNIPERRIFLE]);
				}

				GetWeaponName(reason, weapon_name, sizeof(weapon_name));
				format(string, sizeof(string), "Good job! You killed %s[%i] and gained $%i. Your weapon skill for %s is now %i/1000.", name, playerid, money, weapon_name, iPlayerSkills[killerid][WEAPONSKILL_SNIPERRIFLE]);
				SendClientMessage(killerid, COLOR_GREEN, string);
			}
		}

	    iPlayerSpree[killerid]++;
	    GetPlayerName(killerid, name, MAX_PLAYER_NAME);
	    string[0] = EOS;
	    switch (iPlayerSpree[killerid])
	    {
	        case 5:
	            format(string, sizeof(string), "%s[%i] is on a killing spree of 5 kills!", name, killerid);
	        case 10:
	            format(string, sizeof(string), "%s[%i] is on insane killing spree 10 kills!", name, killerid);
	        case 15:
	            format(string, sizeof(string), "%s[%i] is godlike with a killing spree 15 kills!", name, killerid);
	        case 20:
	            format(string, sizeof(string), "%s[%i] is godlike with a killing spree 20 kills!", name, killerid);
	        case 25:
	            format(string, sizeof(string), "%s[%i] is PRO with a killing spree 25 kills!", name, killerid);
	        case 30:
	            format(string, sizeof(string), "%s[%i] is incredible with a killing spree 30 kills!", name, killerid);
	        case 35:
	            format(string, sizeof(string), "%s[%i] is wicked sick with a killing spree 35 kills!", name, killerid);
	        case 40:
	            format(string, sizeof(string), "%s[%i] is damn good with a killing spree 40 kills!", name, killerid);
	        case 50:
	            format(string, sizeof(string), "%s[%i] is god with a killing spree 50 kills!", name, killerid);
			default:
			{
				if (iPlayerSpree[killerid] > 50)
				{
				    if ((iPlayerSpree[killerid] % 5) == 0)
				    {
	            		format(string, sizeof(string), "%s[%i] is hacking! He/She's a killing spree %i kills!", name, killerid, iPlayerSpree[killerid]);
				    }
				}
			}
		}

		if (string[0])
 			SendClientMessageToAll(COLOR_DEFAULT, string);
	}

    iPlayerSpree[playerid] = 0;
	iPlayerHeadshotData[playerid][0] = INVALID_PLAYER_ID;
	return 1;
}

CMD:kill(playerid)
{
	if (bPlayerGambleStarted[playerid])
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use commands while gambling.");

	SetPlayerHealth(playerid, 0.0);
	return 1;
}

CMD:shop(playerid)
{
	if (!IsPlayerSpawned(playerid))
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command when you are not spawned or under spawn protection.");

	if (IsPlayerInAnyVehicle(playerid))
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command when you are in a vehicle.");

	if (bPlayerGambleStarted[playerid])
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use commands while gambling.");

	Dialog_Show(playerid, SHOP, DIALOG_STYLE_LIST, "Select a shop type you want to open...", "Weapons shop\nVehicles shop", "Open", "Cancel");
	return 1;
}

Dialog:SHOP(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	if (listitem == 0)
	{
	    SendClientMessage(playerid, COLOR_DEFAULT, "Tip: You can also open this shop directly by /weapons.");
		cmd_weapons(playerid);
	}
	else if (listitem == 1)
	{
	    SendClientMessage(playerid, COLOR_DEFAULT, "Tip: You can also open this shop directly by /vehicles.");
		cmd_vehicles(playerid);
	}
	return 1;
}

CMD:vehicles(playerid)
{
	if (!IsPlayerSpawned(playerid))
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command when you are not spawned or under spawn protection.");

	if (IsPlayerInAnyVehicle(playerid))
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command when you are in a vehicle.");

	if (bPlayerGambleStarted[playerid])
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use commands while gambling.");

	new string[sizeof(VEHICLE_SHOP) * 50];
	for (new i; i < sizeof(VEHICLE_SHOP); i++)
		format(string, sizeof(string), "%s%i\t%s~n~~r~-$%i\n", string, VEHICLE_SHOP[i][e_VEHICLE_SHOP_MODEL], VEHICLE_SHOP[i][e_VEHICLE_SHOP_NAME], VEHICLE_SHOP[i][e_VEHICLE_SHOP_PRICE]);

	for (new i; i < sizeof(VEHICLE_SHOP); i++)
		Dialog_SetModelRot(playerid, i, 0.0, 0.0, -50.0, 1.0);

	Dialog_Show(playerid, VEHICLES, DIALOG_STYLE_PREVMODEL, "Select a vehicle you want to buy:", string, "Buy", "Cancel");
	return 1;
}

Dialog:VEHICLES(playerid, response, listitem, inputtext[])
{
    if (!response)
	    return 1;

	new string[150];
	if (GetPlayerMoney(playerid) < VEHICLE_SHOP[listitem][e_VEHICLE_SHOP_PRICE])
	{
	    format(string, sizeof(string), "You need atleast $%i to buy a %s.", VEHICLE_SHOP[listitem][e_VEHICLE_SHOP_PRICE], VEHICLE_SHOP[listitem][e_VEHICLE_SHOP_NAME]);
		return SendClientMessage(playerid, COLOR_TOMATO, string);
	}

	new Float:x,
	    Float:y,
	    Float:z,
	    Float:ang;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, ang);
	x += (5.0 * floatsin(-ang, degrees));
	y += (5.0 * floatcos(-ang, degrees));

	new idx = -1;
	for (new i; i < sizeof(iPlayerVehicleid[]); i++)
	{
	    if (!IsValidVehicle(iPlayerVehicleid[playerid][i]))
		{
		    idx = i;
		    break;
		}
	}
	if (idx == -1)
	{
	    SetTimerEx("OnVehicleTimeout", (30 * 1000), false, "i", iPlayerVehicleid[playerid][0]);
	    idx = 0;
	}
	iPlayerVehicleid[playerid][idx] = AddStaticVehicle(VEHICLE_SHOP[listitem][e_VEHICLE_SHOP_MODEL], x, y, z, ang, random(10), random(10));

	GivePlayerMoney(playerid, -VEHICLE_SHOP[listitem][e_VEHICLE_SHOP_PRICE]);
	format(string, sizeof(string), "You have bought a %s for $%i.", VEHICLE_SHOP[listitem][e_VEHICLE_SHOP_NAME], VEHICLE_SHOP[listitem][e_VEHICLE_SHOP_PRICE]);
	SendClientMessage(playerid, COLOR_GREEN, string);
	return 1;
}

forward OnVehicleTimeout(vehicleid);
public OnVehicleTimeout(vehicleid)
{
    DestroyVehicle(vehicleid);
}

CMD:weapons(playerid)
{
	if (!IsPlayerSpawned(playerid))
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command when you are not spawned or under spawn protection.");

	if (IsPlayerInAnyVehicle(playerid))
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command when you are in a vehicle.");

	if (bPlayerGambleStarted[playerid])
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use commands while gambling.");

	new string[sizeof(WEAPON_SHOP) * 50];
	for (new i; i < sizeof(WEAPON_SHOP); i++)
		format(string, sizeof(string), "%s%i\t%s~n~~r~-$%i\n", string, WEAPON_SHOP[i][e_WEAPON_SHOP_MODEL], WEAPON_SHOP[i][e_WEAPON_SHOP_NAME], WEAPON_SHOP[i][e_WEAPON_SHOP_PRICE]);

	Dialog_Show(playerid, WEAPONS, DIALOG_STYLE_PREVMODEL, "Select a weapon you want to buy:", string, "Buy", "Cancel");

	for (new i; i < sizeof(WEAPON_SHOP); i++)
		Dialog_SetModelRot(playerid, i, 0.0, 0.0, -50.0, 1.5);
	return 1;
}

Dialog:WEAPONS(playerid, response, listitem, inputtext[])
{
    if (!response)
	    return 1;

	new string[150];
	if (GetPlayerMoney(playerid) < WEAPON_SHOP[listitem][e_WEAPON_SHOP_PRICE])
	{
	    format(string, sizeof(string), "You need atleast $%i to buy a %s.", WEAPON_SHOP[listitem][e_WEAPON_SHOP_PRICE], WEAPON_SHOP[listitem][e_WEAPON_SHOP_NAME]);
		return SendClientMessage(playerid, COLOR_TOMATO, string);
	}

	GivePlayerWeapon(playerid, GetModelWeaponID(WEAPON_SHOP[listitem][e_WEAPON_SHOP_MODEL]), WEAPON_SHOP[listitem][e_WEAPON_SHOP_AMMO]);
	GivePlayerMoney(playerid, -WEAPON_SHOP[listitem][e_WEAPON_SHOP_PRICE]);
	format(string, sizeof(string), "You have bought a %s with %i ammo for $%i.", WEAPON_SHOP[listitem][e_WEAPON_SHOP_NAME], WEAPON_SHOP[listitem][e_WEAPON_SHOP_AMMO], WEAPON_SHOP[listitem][e_WEAPON_SHOP_PRICE]);
	SendClientMessage(playerid, COLOR_GREEN, string);
	return 1;
}

CMD:helmet(playerid)
{
	if (!IsPlayerSpawned(playerid))
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command when you are not spawned or under spawn protection.");

	if (IsPlayerInAnyVehicle(playerid))
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command when you are in a vehicle.");

	if (bPlayerGambleStarted[playerid])
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use commands while gambling.");

	if (bPlayerHelmet[playerid])
	    return SendClientMessage(playerid, COLOR_TOMATO, "You already have a protection helmet and wearing it!");

	new string[sizeof(HELMETS) * 7];
	for (new i; i < sizeof(HELMETS); i++)
		format(string, sizeof(string), "%s%i\n", string, HELMETS[i]);
	Dialog_Show(playerid, HELMET, DIALOG_STYLE_PREVMODEL, "Select a helmet you would like to wear!", string, "-$3150", "Cacnel");

	for (new i; i < sizeof(HELMETS); i++)
		Dialog_SetModelRot(playerid, i, 0.0, 0.0, -50.0, 1.0);
    return 1;
}

Dialog:HELMET(playerid, response, listitem, inputext[])
{
	if (!response)
	    return 1;

	if (GetPlayerMoney(playerid) < 3150)
	    return SendClientMessage(playerid, COLOR_TOMATO, "You need atleast $3150 to buy a protection helmet.");

	GivePlayerMoney(playerid, -3150);
    SendClientMessage(playerid, COLOR_GREEN, "You have bought a protection helmet for $3150. You should no longer worry about headshots fam!");

    SetPlayerAttachedObject(playerid, MAX_PLAYER_ATTACHED_OBJECTS - 1, HELMETS[listitem], 2, 0.173000, 0.024999, -0.003000, 0.000000, 0.000000, 0.000000, 1.000000, 1.000000, 1.000000); //skin 102
	bPlayerHelmet[playerid] = true;
	PlayerTextDrawSetString(playerid, ptxtHelmet[playerid], "/Helmet: ~g~ON");
	return 1;
}

CMD:offradar(playerid)
{
	if (!IsPlayerSpawned(playerid))
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command when you are not spawned or under spawn protection.");

	if (bPlayerGambleStarted[playerid])
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use commands while gambling.");

	if (iPlayerOffRadar[playerid][0] > 0)
	    return SendClientMessage(playerid, COLOR_TOMATO, "You already off the radar till this time!");

	Dialog_Show(playerid, OFF_RADAR, DIALOG_STYLE_MSGBOX, "Go Off-Radar?", COL_WHITE "Would you like to go off the radar(inivisible on map) for 2 minutes at cost "COL_TOMATO"$3999"COL_WHITE"?", "-$3999", "Cacnel");
	return 1;
}

Dialog:OFF_RADAR(playerid, response, listitem, inputext[])
{
	if (!response)
	    return 1;

	if (GetPlayerMoney(playerid) < 3999)
	    return SendClientMessage(playerid, COLOR_TOMATO, "You need atleast $3999 to go off-radar.");

	GivePlayerMoney(playerid, -3999);
    SendClientMessage(playerid, COLOR_GREEN, "You are now off the radar, no one can see you on minimap for 2 minutes from now. (money deducted: -$3999)");

	GameTextForPlayer(playerid, "~w~You are now Off-Radar!", 3000, 3);
	SetPlayerColor(playerid, SET_ALPHA(((GetPlayerTeam(playerid) == TEAM_COPS) ? (TEAM_COLOR_COPS) : (TEAM_COLOR_TERRORISTS)), 0));

    iPlayerOffRadar[playerid][0] = (2 * 60);
    iPlayerOffRadar[playerid][1] = SetTimerEx("OnPlayerOffRadarUpdate", 1000, true, "i", playerid);
	PlayerTextDrawSetString(playerid, ptxtOffRadar[playerid][0], "/Offradar: ~g~ON");
	PlayerTextDrawSetString(playerid, ptxtOffRadar[playerid][1], "You will be visible back in ~g~1m 59s");
	return 1;
}

forward OnPlayerOffRadarUpdate(playerid);
public OnPlayerOffRadarUpdate(playerid)
{
	if (iPlayerOffRadar[playerid][0] == 0)
	{
	    GameTextForPlayer(playerid, "~w~You are now On-Radar!", 3000, 3);
		SetPlayerColor(playerid, SET_ALPHA(((GetPlayerTeam(playerid) == TEAM_COPS) ? (TEAM_COLOR_COPS) : (TEAM_COLOR_TERRORISTS)), 255));

	    KillTimer(iPlayerOffRadar[playerid][1]);
	    iPlayerOffRadar[playerid][1] = -1;

		PlayerTextDrawSetString(playerid, ptxtOffRadar[playerid][0], "/Offradar: ~r~OFF");
		PlayerTextDrawSetString(playerid, ptxtOffRadar[playerid][1], "Invisibility on map for 2 minutes");
	    return;
	}
    iPlayerOffRadar[playerid][0]--;

    new string[64];
    format(string, sizeof(string), "You will be visible back in ~g~%im %02is", (iPlayerOffRadar[playerid][0] / 60), (iPlayerOffRadar[playerid][0] % 60));
	PlayerTextDrawSetString(playerid, ptxtOffRadar[playerid][1], string);
}

CMD:cmds(playerid, params[])
{
	if (bPlayerGambleStarted[playerid])
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use commands while gambling.");

	if (!strcmp(params, "hide", true))
	{
	    ShowNotification(playerid, "", 0);
	    return 1;
	}

	new string[1024] = "~y~/kill ~w~- commit sucide~n~\
		~y~/shop ~w~- open shop dialog~n~\
		~y~/weapons ~w~- open weapons shop dialog~n~\
		~y~/vehicles ~w~- open vehicles shop dialog~n~\
		~y~/helmet ~w~- buy a headshot protection helmet~n~\
		~y~/offradar ~w~- go invisible on radar/map for 2 minutes~n~";
	strcat(string, "~y~/gamble ~w~- make some money through betting~n~\
		~y~/stats ~w~- player statistics~n~\
		~y~/skills ~w~- player skills statistics~n~\
		~y~/changeques ~w~- change your security question & answer~n~\
		~y~/changepass ~w~- change your account password~n~~n~\
		Type \"/cmds hide\" to hide this box.");
	ShowNotification(playerid, string, 0);
	return 1;
}

CMD:gamble(playerid, params[])
{
	if (!bPlayerGambleStarted[playerid])
	{
		ShowNotification(playerid, "~w~Welcome to mini-gambling arena!~n~~n~You can win alot of money at different stages but loose as well, all depends on luck!~n~~n~To start gambling, place a bet by typing ~y~/gamble [bet]~w~.~n~~n~To cancel, type \"/gamble exit\".", 0);
        bPlayerGambleStarted[playerid] = true;
        iPlayerGambleBet[playerid] = 0;
        return 1;
	}

	if (!strcmp(params, "exit", true))
	{
	    ShowNotification(playerid, "Gambling has been closed.", 3000);
	    bPlayerGambleStarted[playerid] = false;

		PlayerTextDrawHide(playerid, ptxtGamble[playerid][0]);
	 	PlayerTextDrawHide(playerid, ptxtGamble[playerid][1]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][2]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][3]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][4]);
	  	PlayerTextDrawHide(playerid, ptxtGamble[playerid][5]);
	    return 1;
	}

	if (iPlayerGambleBet[playerid] < 10)
	{
		if (!sscanf(params, "i", iPlayerGambleBet[playerid]))
		{
			if (iPlayerGambleBet[playerid] < 10)
			{
			  	SendClientMessage(playerid, COLOR_TOMATO, "Minimum bet for gambling is $10.");
			   	return 1;
			}
			else if (iPlayerGambleBet[playerid] > 1000)
			{
			 	SendClientMessage(playerid, COLOR_TOMATO, "Maximum bet for gambling is $1000.");
     			return 1;
			}
			else if (GetPlayerMoney(playerid) < iPlayerGambleBet[playerid])
			{
			 	SendClientMessage(playerid, COLOR_TOMATO, "You don't have that much money with you to bet.");
     			return 1;
			}
		}
		else return 1;
	}

	if (random(2) == 0)
 	{
    	iPlayerGambleRightCard[playerid] = random(3);
        new r = random(2);
   		if (iPlayerGambleRightCard[playerid] == 0)
   		{
        	format(sPlayerGambleCards[playerid][0], sizeof(sPlayerGambleCards[][]), RED_CARD);
   			format(sPlayerGambleCards[playerid][1], sizeof(sPlayerGambleCards[][]), ((r == 0) ? (BLACK_CARD) : (OTHER_CARD)));
   			format(sPlayerGambleCards[playerid][2], sizeof(sPlayerGambleCards[][]), ((r == 1) ? (BLACK_CARD) : (OTHER_CARD)));
   		}
   		else if (iPlayerGambleRightCard[playerid] == 1)
   		{
        	format(sPlayerGambleCards[playerid][1], sizeof(sPlayerGambleCards[][]), RED_CARD);
   			format(sPlayerGambleCards[playerid][2], sizeof(sPlayerGambleCards[][]), ((r == 0) ? (BLACK_CARD) : (OTHER_CARD)));
   			format(sPlayerGambleCards[playerid][0], sizeof(sPlayerGambleCards[][]), ((r == 1) ? (BLACK_CARD) : (OTHER_CARD)));
   		}
   		else if (iPlayerGambleRightCard[playerid] == 2)
   		{
        	format(sPlayerGambleCards[playerid][2], sizeof(sPlayerGambleCards[][]), RED_CARD);
   			format(sPlayerGambleCards[playerid][0], sizeof(sPlayerGambleCards[][]), ((r == 0) ? (BLACK_CARD) : (OTHER_CARD)));
   			format(sPlayerGambleCards[playerid][1], sizeof(sPlayerGambleCards[][]), ((r == 1) ? (BLACK_CARD) : (OTHER_CARD)));
   		}

		new string[512];
		format(string, sizeof(string), "~w~The game is simple and easy, you have to choose which card is of color \"RED\". Click on the card you want to choose.~n~~n~Bet Money: ~y~$%i~n~~n~~w~If you ~g~Won~w~, you'll get 2x of your bet money.~n~If you ~r~Loose~w~, all your money will be gone!", iPlayerGambleBet[playerid]);
       	ShowNotification(playerid, string, 0);
  	}
	else
    {
		iPlayerGambleRightCard[playerid] = random(3);
        new r = random(2);
   		if (iPlayerGambleRightCard[playerid] == 0)
   		{
        	format(sPlayerGambleCards[playerid][0], sizeof(sPlayerGambleCards[][]), BLACK_CARD);
   			format(sPlayerGambleCards[playerid][1], sizeof(sPlayerGambleCards[][]), ((r == 0) ? (RED_CARD) : (OTHER_CARD)));
   			format(sPlayerGambleCards[playerid][2], sizeof(sPlayerGambleCards[][]), ((r == 1) ? (RED_CARD) : (OTHER_CARD)));
   		}
   		else if (iPlayerGambleRightCard[playerid] == 1)
   		{
        	format(sPlayerGambleCards[playerid][1], sizeof(sPlayerGambleCards[][]), BLACK_CARD);
   			format(sPlayerGambleCards[playerid][2], sizeof(sPlayerGambleCards[][]), ((r == 0) ? (RED_CARD) : (OTHER_CARD)));
   			format(sPlayerGambleCards[playerid][0], sizeof(sPlayerGambleCards[][]), ((r == 1) ? (RED_CARD) : (OTHER_CARD)));
   		}
   		else if (iPlayerGambleRightCard[playerid] == 2)
   		{
        	format(sPlayerGambleCards[playerid][2], sizeof(sPlayerGambleCards[][]), BLACK_CARD);
   			format(sPlayerGambleCards[playerid][0], sizeof(sPlayerGambleCards[][]), ((r == 0) ? (RED_CARD) : (OTHER_CARD)));
   			format(sPlayerGambleCards[playerid][1], sizeof(sPlayerGambleCards[][]), ((r == 1) ? (RED_CARD) : (OTHER_CARD)));
   		}

		new string[512];
		format(string, sizeof(string), "~w~The game is simple and easy, you have to choose which card is of color \"BLACK\". Click on the card you want to choose.~n~~n~Bet Money: ~y~$%i~n~~n~~w~If you ~g~Won~w~, you'll get 2x of your bet money.~n~If you ~r~Loose~w~, all your money will be gone!", iPlayerGambleBet[playerid]);
       	ShowNotification(playerid, string, 0);
	}

	PlayerTextDrawColor(playerid, ptxtGamble[playerid][0], 0x000000FF);
	PlayerTextDrawShow(playerid, ptxtGamble[playerid][0]);
 	PlayerTextDrawSetString(playerid, ptxtGamble[playerid][1], CARD);
 	PlayerTextDrawShow(playerid, ptxtGamble[playerid][1]);
 	PlayerTextDrawColor(playerid, ptxtGamble[playerid][2], 0x000000FF);
  	PlayerTextDrawShow(playerid, ptxtGamble[playerid][2]);
  	PlayerTextDrawSetString(playerid, ptxtGamble[playerid][3], CARD);
  	PlayerTextDrawShow(playerid, ptxtGamble[playerid][3]);
 	PlayerTextDrawColor(playerid, ptxtGamble[playerid][4], 0x000000FF);
  	PlayerTextDrawShow(playerid, ptxtGamble[playerid][4]);
  	PlayerTextDrawSetString(playerid, ptxtGamble[playerid][5], CARD);
  	PlayerTextDrawShow(playerid, ptxtGamble[playerid][5]);
	SelectTextDraw(playerid, 0xFF0000BB);
	return 1;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if (bPlayerGambleStarted[playerid])
	{
	    new card;
		if (playertextid == ptxtGamble[playerid][1])
		{
			card = 0;
			PlayerTextDrawColor(playerid, ptxtGamble[playerid][0], 0xFF0000FF);
		 	PlayerTextDrawShow(playerid, ptxtGamble[playerid][0]);
		}
		else if (playertextid == ptxtGamble[playerid][3])
		{
			card = 1;
			PlayerTextDrawColor(playerid, ptxtGamble[playerid][2], 0xFF0000FF);
		 	PlayerTextDrawShow(playerid, ptxtGamble[playerid][2]);
		}
		else if (playertextid == ptxtGamble[playerid][5])
		{
			card = 2;
			PlayerTextDrawColor(playerid, ptxtGamble[playerid][4], 0xFF0000FF);
		 	PlayerTextDrawShow(playerid, ptxtGamble[playerid][4]);
		}
		else
			return 1;

		if (iPlayerGambleRightCard[playerid] == 0)
	    {
			PlayerTextDrawColor(playerid, ptxtGamble[playerid][0], 0x00FF00FF);
		 	PlayerTextDrawShow(playerid, ptxtGamble[playerid][0]);
	    }
	    else if (iPlayerGambleRightCard[playerid] == 1)
	    {
	 		PlayerTextDrawColor(playerid, ptxtGamble[playerid][2], 0x00FF00FF);
	 		PlayerTextDrawShow(playerid, ptxtGamble[playerid][2]);
	    }
	    else
	    {
	 		PlayerTextDrawColor(playerid, ptxtGamble[playerid][4], 0x00FF00FF);
	 		PlayerTextDrawShow(playerid, ptxtGamble[playerid][4]);
	    }

		if (card == iPlayerGambleRightCard[playerid])
		{
            GivePlayerMoney(playerid, (iPlayerGambleBet[playerid] * 2));

			new string[150];
			format(string, sizeof(string), "~w~Great job! You won ~g~$%i ~w~(your bet was ~g~$%i~w~).~n~~n~Type \"/gamble exit\" to quit playing or \"/gamble [bet]\" to play again!", (iPlayerGambleBet[playerid] * 2), iPlayerGambleBet[playerid]);
			ShowNotification(playerid, string, 0);
		}
		else
   		{
            GivePlayerMoney(playerid, -iPlayerGambleBet[playerid]);

			new string[150];
			format(string, sizeof(string), "Sorry! You lost this time (money lost: ~r~$%i~w~).~n~~n~Type \"/gamble exit\" to quit playing or \"/gamble [bet]\" to play again!", iPlayerGambleBet[playerid]);
			ShowNotification(playerid, string, 0);
		}

		iPlayerGambleBet[playerid] = 0;

	 	PlayerTextDrawSetString(playerid, ptxtGamble[playerid][1], sPlayerGambleCards[playerid][0]);
	 	PlayerTextDrawShow(playerid, ptxtGamble[playerid][1]);
	  	PlayerTextDrawSetString(playerid, ptxtGamble[playerid][3], sPlayerGambleCards[playerid][1]);
	  	PlayerTextDrawShow(playerid, ptxtGamble[playerid][3]);
	  	PlayerTextDrawSetString(playerid, ptxtGamble[playerid][5], sPlayerGambleCards[playerid][2]);
	  	PlayerTextDrawShow(playerid, ptxtGamble[playerid][5]);
		CancelSelectTextDraw(playerid);
	}
	return 1;
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
	if (!success)
	{
		SendClientMessage(playerid, COLOR_TOMATO, "Command not found! Type /cmds for list!");
	}
	return 1;
}
