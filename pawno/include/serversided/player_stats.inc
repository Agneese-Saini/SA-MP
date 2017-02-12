#if defined ss@player_stats_included
	#endinput
#endif
#define ss@player_stats_included

/* FUNCTIONS:
native HasSameTeam(playerid, otherid);
native SetPlayerWeaponData(playerid, slot, weapon, ammo);
*/

#include <SKY>
#include "script_init"

#if !defined KEY_AIM
	#define KEY_AIM \
		128
#endif

#if !defined PRESSED
	#define PRESSED(%0) \
		(((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))
#endif

#if !defined MAX_CLASSES
	#define MAX_CLASSES \
		312
#endif

#if !defined MAX_HEALTH
	#define MAX_HEALTH \
		100.0
#endif

#if !defined MAX_ARMOUR
	#define MAX_ARMOUR \
		100.0
#endif

enum e_PLAYER_DATA
{
    e_PLAYER_DATA_TEAMID,
    e_PLAYER_DATA_CLASSID,
    e_PLAYER_DATA_MONEY,
    Float:e_PLAYER_DATA_HEALTH,
	Float:e_PLAYER_DATA_ARMOUR,
	e_PLAYER_DATA_RESPAWN_DELAY
};
static ePlayerData[MAX_PLAYERS][e_PLAYER_DATA];

static iPlayerWeaponData[MAX_PLAYERS][14][2];

enum e_SPAWN_DATA
{
    e_SPAWN_DATA_TEAMID,
    e_SPAWN_DATA_WEAPON1[2],
    e_SPAWN_DATA_WEAPON2[2],
    e_SPAWN_DATA_WEAPON3[2]
};
static ePlayerSpawnData[MAX_PLAYERS][e_SPAWN_DATA];
static eClassSpawnData[MAX_CLASSES][e_SPAWN_DATA];

static bool:bToggleSetSpawnInfo[MAX_PLAYERS];

static PL_UpdatePlayerHealth(playerid, Float:health)
{
    ePlayerData[playerid][e_PLAYER_DATA_HEALTH] = health;

	new set = floatround(ePlayerData[playerid][e_PLAYER_DATA_HEALTH] / MAX_HEALTH * 100.0, floatround_ceil);
	
	SetFakeHealth(playerid, set);
	SetPlayerHealth(playerid, ((float(set) <= 0.0) ? (0.1) : (float(set))));
}

static PL_UpdatePlayerArmour(playerid, Float:armour)
{
    ePlayerData[playerid][e_PLAYER_DATA_ARMOUR] = armour;

	new set = floatround(ePlayerData[playerid][e_PLAYER_DATA_ARMOUR] / MAX_ARMOUR * 100.0, floatround_ceil);
	
	SetFakeArmour(playerid, set);
	SetPlayerArmour(playerid, ((float(set) <= 0.0) ? (0.1) : (float(set))));
}

static PL_GetWeaponSlot(weaponid)
{
	switch (weaponid)
	{
		case 0, 1: return 0;
		case 2..9: return 1;
		case 10..15: return 10;
		case 16..18, 39: return 8;
		case 22..24: return 2;
		case 25..27: return 3;
		case 28, 29, 32: return 4;
		case 30, 31: return 5;
		case 33, 34: return 6;
		case 35..38: return 7;
		case 40: return 12;
		case 41..43: return 9;
		case 44..46: return 11;
	}
	return -1;
}

public OnScriptInit()
{
	for (new i; i < 312; i++)
	{
	    eClassSpawnData[i][e_SPAWN_DATA_TEAMID] = NO_TEAM;
	}

	#if defined PL_OnScriptInit
		return PL_OnScriptInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnScriptInit
	#undef OnScriptInit
#else
	#define _ALS_OnScriptInit
#endif
#define OnScriptInit PL_OnScriptInit
#if defined PL_OnScriptInit
	forward PL_OnScriptInit();
#endif

public OnPlayerConnect(playerid)
{
	ePlayerData[playerid][e_PLAYER_DATA_TEAMID] = NO_TEAM;
	ePlayerData[playerid][e_PLAYER_DATA_CLASSID] = 0;

	ePlayerData[playerid][e_PLAYER_DATA_HEALTH] = 100.0;
	ePlayerData[playerid][e_PLAYER_DATA_ARMOUR] = 0.0;

	for (new i; i < 14; i++)
	{
		iPlayerWeaponData[playerid][i][0] = 0;
		iPlayerWeaponData[playerid][i][1] = 0;
	}

	ePlayerSpawnData[playerid][e_SPAWN_DATA_TEAMID] = NO_TEAM;
	ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON1][0] = 0;
	ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON1][1] = 0;
	ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON2][0] = 0;
	ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON2][1] = 0;
	ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON3][0] = 0;
	ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON3][1] = 0;

	#if defined PL_OnPlayerConnect
		return PL_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect PL_OnPlayerConnect
#if defined PL_OnPlayerConnect
	forward PL_OnPlayerConnect(playerid);
#endif

public OnPlayerRequestClass(playerid, classid)
{
	ePlayerData[playerid][e_PLAYER_DATA_CLASSID] = classid;

	#if defined PL_OnPlayerRequestClass
		return PL_OnPlayerRequestClass(playerid, classid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerRequestClass
    #undef OnPlayerRequestClass
#else
    #define _ALS_OnPlayerRequestClass
#endif
#define OnPlayerRequestClass PL_OnPlayerRequestClass
#if defined PL_OnPlayerRequestClass
	forward PL_OnPlayerRequestClass(playerid, classid);
#endif

public OnPlayerSpawn(playerid)
{
    PL_UpdatePlayerHealth(playerid, 100.0);
    PL_UpdatePlayerArmour(playerid, 0.0);

	for (new i; i < 14; i++)
	{
	    iPlayerWeaponData[playerid][i][0] = 0;
	    iPlayerWeaponData[playerid][i][1] = 0;
	}
	ResetPlayerWeapons(playerid);

	new slot;
	if (!bToggleSetSpawnInfo[playerid])
	{
		slot = PL_GetWeaponSlot(eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON1][0]);
		if (slot != -1)
		{
			iPlayerWeaponData[playerid][slot][0] = eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON1][0];
			iPlayerWeaponData[playerid][slot][1] = eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON2][0];

			if (IS_GAMEMODE)
	 			GivePlayerWeapon(playerid, eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON1][0], eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON1][1]);
		}

		slot = PL_GetWeaponSlot(eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON2][0]);
		if (slot != -1)
		{
			iPlayerWeaponData[playerid][slot][0] = eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON2][0];
			iPlayerWeaponData[playerid][slot][1] = eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON2][0];

			if (IS_GAMEMODE)
				GivePlayerWeapon(playerid, eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON2][0], eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON2][1]);
		}

		slot = PL_GetWeaponSlot(eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON3][0]);
		if (slot != -1)
		{
			iPlayerWeaponData[playerid][slot][0] = eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON3][0];
			iPlayerWeaponData[playerid][slot][1] = eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON3][0];

			if (IS_GAMEMODE)
				GivePlayerWeapon(playerid, eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON3][0], eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_WEAPON3][1]);
		}

		ePlayerData[playerid][e_PLAYER_DATA_TEAMID] = eClassSpawnData[ePlayerData[playerid][e_PLAYER_DATA_CLASSID]][e_SPAWN_DATA_TEAMID];
	}
	else
	{
	    bToggleSetSpawnInfo[playerid] = false;

		slot = PL_GetWeaponSlot(ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON1][0]);
		if (slot != -1)
		{
			iPlayerWeaponData[playerid][slot][0] = ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON1][0];
			iPlayerWeaponData[playerid][slot][1] = ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON2][0];

			if (IS_GAMEMODE)
				GivePlayerWeapon(playerid, ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON1][0], ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON1][1]);
		}

		slot = PL_GetWeaponSlot(ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON2][0]);
		if (slot != -1)
		{
			iPlayerWeaponData[playerid][slot][0] = ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON2][0];
			iPlayerWeaponData[playerid][slot][1] = ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON2][0];

			if (IS_GAMEMODE)
				GivePlayerWeapon(playerid, ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON2][0], ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON2][1]);
		}

		slot = PL_GetWeaponSlot(ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON3][0]);
		if (slot != -1)
		{
			iPlayerWeaponData[playerid][slot][0] = ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON3][0];
			iPlayerWeaponData[playerid][slot][1] = ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON3][0];

			if (IS_GAMEMODE)
				GivePlayerWeapon(playerid, ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON3][0], ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON3][1]);
		}

		ePlayerData[playerid][e_PLAYER_DATA_TEAMID] = ePlayerSpawnData[playerid][e_SPAWN_DATA_TEAMID];
	}

	SetPlayerTeam(playerid, 0);

	#if defined PL_OnPlayerSpawn
		return PL_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn PL_OnPlayerSpawn
#if defined PL_OnPlayerSpawn
	forward PL_OnPlayerSpawn(playerid);
#endif

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (newkeys & KEY_FIRE)
 	{
		if (GetPlayerWeapon(playerid) == WEAPON_SATCHEL)
		{
	        new slot = PL_GetWeaponSlot(WEAPON_BOMB);
			if (slot != -1)
			{
				iPlayerWeaponData[playerid][slot][0] = WEAPON_BOMB;
				iPlayerWeaponData[playerid][slot][1] = 1;
		    }

	        if (IS_GAMEMODE)
				GivePlayerWeapon(playerid, WEAPON_BOMB, 1);
		}
	}

	#if defined PL_OnPlayerKeyStateChange
		return PL_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange PL_OnPlayerKeyStateChange
#if defined PL_OnPlayerKeyStateChange
	forward PL_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

public OnPlayerUpdate(playerid)
{
	if (IS_GAMEMODE)
	{
	    if (GetPlayerState(playerid) == PLAYER_STATE_SPAWNED)
		{
			if (GetPlayerMoney(playerid) != ePlayerData[playerid][e_PLAYER_DATA_MONEY])
			{
			    ResetPlayerMoney(playerid);
				GivePlayerMoney(playerid, ePlayerData[playerid][e_PLAYER_DATA_MONEY]);
			}

			new weapon = GetPlayerWeapon(playerid);
			new slot = PL_GetWeaponSlot(weapon);
			if (iPlayerWeaponData[playerid][slot][0] != weapon)
			{
				SetPlayerAmmo(playerid, weapon, 0);
				GivePlayerWeapon(playerid, iPlayerWeaponData[playerid][slot][0], iPlayerWeaponData[playerid][slot][1]);
			}
			else
			{
			    switch (weapon)
			    {
					case WEAPON_COLT45..WEAPON_SNIPER, WEAPON_MINIGUN, WEAPON_SATCHEL:
			    	{
						if (iPlayerWeaponData[playerid][slot][1] != GetPlayerAmmo(playerid))
							SetPlayerAmmo(playerid, weapon, iPlayerWeaponData[playerid][slot][1]);
					}

					default:
					{
						if (GetPlayerAmmo(playerid) > iPlayerWeaponData[playerid][slot][1])
							SetPlayerAmmo(playerid, weapon, 0);
					}
				}
			}
		}
	}

	#if defined PL_OnPlayerUpdate
		return PL_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerUpdate
    #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate PL_OnPlayerUpdate
#if defined PL_OnPlayerUpdate
	forward PL_OnPlayerUpdate(playerid);
#endif

forward __SetPlayerTeam(playerid, teamid);
public 	__SetPlayerTeam(playerid, teamid)
{
    ePlayerData[playerid][e_PLAYER_DATA_TEAMID] = teamid;
}

stock PL_SetPlayerTeam(playerid, teamid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	CallRemoteFunction("__SetPlayerTeam", "ii", playerid, teamid);
	return 1;
}
#if defined _ALS_SetPlayerTeam
    #undef SetPlayerTeam
#else
    #define _ALS_SetPlayerTeam
#endif
#define SetPlayerTeam PL_SetPlayerTeam

stock PL_GetPlayerTeam(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return NO_TEAM;

	return ePlayerData[playerid][e_PLAYER_DATA_TEAMID];
}
#if defined _ALS_GetPlayerTeam
    #undef GetPlayerTeam
#else
    #define _ALS_GetPlayerTeam
#endif
#define GetPlayerTeam PL_GetPlayerTeam

forward __SetPlayerHealth(playerid, Float:health);
public 	__SetPlayerHealth(playerid, Float:health)
{
	if (health < 0.0)
		health = 0.0;
		
    ePlayerData[playerid][e_PLAYER_DATA_HEALTH] = health;
}

stock PL_SetPlayerHealth(playerid, Float:health)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	CallRemoteFunction("__SetPlayerHealth", "if", playerid, health);
	PL_UpdatePlayerHealth(playerid, health);
	return 1;
}
#if defined _ALS_SetPlayerHealth
    #undef SetPlayerHealth
#else
    #define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth PL_SetPlayerHealth

stock PL_GetPlayerHealth(playerid, &Float:health)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	health = ePlayerData[playerid][e_PLAYER_DATA_HEALTH];
	return 1;
}
#if defined _ALS_GetPlayerHealth
    #undef GetPlayerHealth
#else
    #define _ALS_GetPlayerHealth
#endif
#define GetPlayerHealth PL_GetPlayerHealth

forward __SetPlayerArmour(playerid, Float:armour);
public 	__SetPlayerArmour(playerid, Float:armour)
{
	if (armour < 0.0)
		armour = 0.0;
		
    ePlayerData[playerid][e_PLAYER_DATA_ARMOUR] = armour;
}

stock PL_SetPlayerArmour(playerid, Float:armour)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
	return 0;

	CallRemoteFunction("__SetPlayerArmour", "if", playerid, armour);
	PL_UpdatePlayerArmour(playerid, armour);
	return 1;
}
#if defined _ALS_SetPlayerArmour
    #undef SetPlayerArmour
#else
    #define _ALS_SetPlayerArmour
#endif
#define SetPlayerArmour PL_SetPlayerArmour

stock PL_GetPlayerArmour(playerid, &Float:armour)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	armour = ePlayerData[playerid][e_PLAYER_DATA_ARMOUR];
	return 1;
}
#if defined _ALS_GetPlayerArmour
    #undef GetPlayerArmour
#else
    #define _ALS_GetPlayerArmour
#endif
#define GetPlayerArmour PL_GetPlayerArmour

forward __SetPlayerMoney(playerid, money);
public 	__SetPlayerMoney(playerid, money)
{
    ePlayerData[playerid][e_PLAYER_DATA_MONEY] = money;
}

stock PL_GivePlayerMoney(playerid, money)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	CallRemoteFunction("__SetPlayerMoney", "ii", playerid, money + ePlayerData[playerid][e_PLAYER_DATA_MONEY]);
	return GivePlayerMoney(playerid, money);
}
#if defined _ALS_GivePlayerMoney
    #undef GivePlayerMoney
#else
    #define _ALS_GivePlayerMoney
#endif
#define GivePlayerMoney PL_GivePlayerMoney

stock PL_ResetPlayerMoney(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	CallRemoteFunction("__SetPlayerMoney", "ii", playerid, 0);
    return ResetPlayerMoney(playerid);
}
#if defined _ALS_ResetPlayerMoney
    #undef ResetPlayerMoney
#else
    #define _ALS_ResetPlayerMoney
#endif
#define ResetPlayerMoney PL_ResetPlayerMoney

stock PL_GetPlayerMoney(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	return ePlayerData[playerid][e_PLAYER_DATA_MONEY];
}
#if defined _ALS_GetPlayerMoney
    #undef GetPlayerMoney
#else
    #define _ALS_GetPlayerMoney
#endif
#define GetPlayerMoney PL_GetPlayerMoney

forward __SetPlayerWeapon(playerid, slot, weaponid, ammo);
public 	__SetPlayerWeapon(playerid, slot, weaponid, ammo)
{
    iPlayerWeaponData[playerid][slot][0] = weaponid;
    iPlayerWeaponData[playerid][slot][1] = ammo;
}

stock PL_GivePlayerWeapon(playerid, weaponid, ammo)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	new slot = PL_GetWeaponSlot(weaponid);
	if (slot == -1)
		return 0;

	CallRemoteFunction("__SetPlayerWeapon", "iiii", playerid, slot, weaponid, ammo + iPlayerWeaponData[playerid][slot][1]);
	return GivePlayerWeapon(playerid, weaponid, ammo);
}
#if defined _ALS_GivePlayerWeapon
    #undef GivePlayerWeapon
#else
    #define _ALS_GivePlayerWeapon
#endif
#define GivePlayerWeapon PL_GivePlayerWeapon

stock PL_ResetPlayerWeapons(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	for (new i; i < 13; i++)
		CallRemoteFunction("__SetPlayerWeapon", "iiii", playerid, i, 0, 0);

	return ResetPlayerWeapons(playerid);
}
#if defined _ALS_ResetPlayerWeapons
    #undef ResetPlayerWeapons
#else
    #define _ALS_ResetPlayerWeapons
#endif
#define ResetPlayerWeapons PL_ResetPlayerWeapons

stock PL_GetPlayerWeaponData(playerid, slot, &weapons, &ammo)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (slot > 12 || slot < 0)
		return 0;

	weapons = iPlayerWeaponData[playerid][slot][0];
	ammo = iPlayerWeaponData[playerid][slot][1];
	return 1;
}
#if defined _ALS_GetPlayerWeaponData
    #undef GetPlayerWeaponData
#else
    #define _ALS_GetPlayerWeaponData
#endif
#define GetPlayerWeaponData PL_GetPlayerWeaponData

stock SetPlayerWeaponData(playerid, slot, weapon, ammo)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (slot > 12 || slot < 0)
		return 0;

	iPlayerWeaponData[playerid][slot][0] = weapon;
	iPlayerWeaponData[playerid][slot][1] = ammo;
	return 1;
}

stock PL_SetPlayerAmmo(playerid, weaponid, ammo)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (PL_iPlayerDeathTimer[playerid] != -1)
	    return 0;

	CallRemoteFunction("__SetPlayerWeapon", "iiii", playerid, PL_GetWeaponSlot(weaponid), weaponid, ammo);
    return SetPlayerAmmo(playerid, weaponid, ammo);
}
#if defined _ALS_SetPlayerAmmo
    #undef SetPlayerAmmo
#else
    #define _ALS_SetPlayerAmmo
#endif
#define SetPlayerAmmo PL_SetPlayerAmmo

stock PL_GetPlayerAmmo(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	new slot = PL_GetWeaponSlot(GetPlayerWeapon(playerid));
	if (slot > 12 || slot < 0)
		return 0;

	return iPlayerWeaponData[playerid][slot][1];
}
#if defined _ALS_GetPlayerAmmo
    #undef GetPlayerAmmo
#else
    #define _ALS_GetPlayerAmmo
#endif
#define GetPlayerAmmo PL_GetPlayerAmmo

stock PL_AddPlayerClass(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
	new ret = AddPlayerClass(modelid, spawn_x, spawn_y, spawn_z, z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
	eClassSpawnData[ret][e_SPAWN_DATA_TEAMID] = NO_TEAM;
	eClassSpawnData[ret][e_SPAWN_DATA_WEAPON1][0] = weapon1;
	eClassSpawnData[ret][e_SPAWN_DATA_WEAPON1][1] = weapon1_ammo;
	eClassSpawnData[ret][e_SPAWN_DATA_WEAPON2][0] = weapon2;
	eClassSpawnData[ret][e_SPAWN_DATA_WEAPON2][1] = weapon2_ammo;
	eClassSpawnData[ret][e_SPAWN_DATA_WEAPON3][0] = weapon3;
	eClassSpawnData[ret][e_SPAWN_DATA_WEAPON3][1] = weapon3_ammo;
	return ret;
}
#if defined _ALS_AddPlayerClass
    #undef AddPlayerClass
#else
    #define _ALS_AddPlayerClass
#endif
#define AddPlayerClass PL_AddPlayerClass

stock PL_AddPlayerClassEx(teamid, modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
	new ret = AddPlayerClassEx(teamid, modelid, spawn_x, spawn_y, spawn_z, z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
	eClassSpawnData[ret][e_SPAWN_DATA_TEAMID] = teamid;
	eClassSpawnData[ret][e_SPAWN_DATA_WEAPON1][0] = weapon1;
	eClassSpawnData[ret][e_SPAWN_DATA_WEAPON1][1] = weapon1_ammo;
	eClassSpawnData[ret][e_SPAWN_DATA_WEAPON2][0] = weapon2;
	eClassSpawnData[ret][e_SPAWN_DATA_WEAPON2][1] = weapon2_ammo;
	eClassSpawnData[ret][e_SPAWN_DATA_WEAPON3][0] = weapon3;
	eClassSpawnData[ret][e_SPAWN_DATA_WEAPON3][1] = weapon3_ammo;
	return ret;
}
#if defined _ALS_AddPlayerClassEx
    #undef AddPlayerClass
#else
    #define _ALS_AddPlayerClassEx
#endif
#define AddPlayerClassEx PL_AddPlayerClassEx

forward __SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
public  __SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
    bToggleSetSpawnInfo[playerid] = true;
	ePlayerSpawnData[playerid][e_SPAWN_DATA_TEAMID] = team;
	ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON1][0] = weapon1;
	ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON1][1] = weapon1_ammo;
	ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON2][0] = weapon2;
	ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON2][1] = weapon2_ammo;
	ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON3][0] = weapon3;
	ePlayerSpawnData[playerid][e_SPAWN_DATA_WEAPON3][1] = weapon3_ammo;
}

stock PL_SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	CallRemoteFunction("__SetSpawnInfo", "iiiffffiiiiii", playerid, team, skin, x, y, z, rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
	return SetSpawnInfo(playerid, team, skin, x, y, z, rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
}
#if defined _ALS_SetSpawnInfo
    #undef SetSpawnInfo
#else
    #define _ALS_SetSpawnInfo
#endif
#define SetSpawnInfo PL_SetSpawnInfo

stock HasSameTeam(playerid, otherid)
{
	if (ePlayerData[playerid][e_PLAYER_DATA_TEAMID] == NO_TEAM || ePlayerData[otherid][e_PLAYER_DATA_TEAMID] == NO_TEAM)
		return 0;

	return bool:(ePlayerData[playerid][e_PLAYER_DATA_TEAMID] == ePlayerData[otherid][e_PLAYER_DATA_TEAMID]);
}
