#if defined GAC_included
	#endinput
#endif
#define GAC_included

/*
native SetPlayerWeaponDamage(playerid, weaponid, Float:damage);
native Float:GetPlayerWeaponDamage(playerid, weaponid);
*/

#if defined OnPlayerFakeDeath
	forward OnPlayerFakeDeath(playerid, killerid, reason);
#endif

#if defined OnPlayerMoneyCheat
	forward OnPlayerMoneyCheat(playerid, oldmoney, newmoney);
#endif

#if defined OnPlayerWeaponCheat
	forward OnPlayerWeaponCheat(playerid, slot, oldweapon, newweapon);
#endif

#if defined OnPlayerAmmoCheat
	forward OnPlayerAmmoCheat(playerid, slot, oldammo, newammo);
#endif

#if defined OnPlayerHealthCheat
	forward OnPlayerHealthCheat(playerid, Float:oldhealth, Float:newhealth);
#endif

#if defined OnPlayerArmourCheat
	forward OnPlayerArmourCheat(playerid, Float:oldarmour, Float:newarmour);
#endif

#if !defined KEY_AIM
	#define KEY_AIM \
		128
#endif

#if !defined MAX_CLASSES
	#define MAX_CLASSES \
		312
#endif

#if !defined MAX_VENDING_MACHINES
	#define MAX_VENDING_MACHINES \
		128
#endif

#define WEAPON_UNARMED \
	0
#define WEAPON_VEHICLE_M4 \
	19
#define WEAPON_VEHICLE_MINIGUN \
	20
#define WEAPON_PISTOLWHIP \
	48
#define WEAPON_HELIBLADES \
	50
#define WEAPON_EXPLOSION \
	51
#define WEAPON_CARPARK \
	52
#define WEAPON_UNKNOWN \
	55

#define IS_GAMEMODE \
    iScriptType == 1

new iScriptType;

enum e_PLAYER_SHOT
{
    e_PLAYER_SHOT_TYPE,
    e_PLAYER_SHOT_HITID,
    bool:e_PLAYER_SHOT_RETURN
};
static ePlayerShot[MAX_PLAYERS][e_PLAYER_SHOT];

enum e_PLAYER_DEATH
{
	bool:e_PLAYER_DEATH_VALID,
    e_PLAYER_DEATH_KILLERID,
    e_PLAYER_DEATH_WEAPONID
};
static ePlayerDeath[MAX_PLAYERS][e_PLAYER_DEATH];

enum e_PLAYER_STATS
{
    e_PLAYER_STATS_TEAMID,
    e_PLAYER_STATS_CLASSID,
    e_PLAYER_STATS_MONEY,
    Float:e_PLAYER_STATS_HEALTH,
	Float:e_PLAYER_STATS_ARMOUR
};
static ePlayerStats[MAX_PLAYERS][e_PLAYER_STATS];

enum e_SPAWN
{
    e_SPAWN_TEAMID,
    e_SPAWN_WEAPON1[2],
    e_SPAWN_WEAPON2[2],
    e_SPAWN_WEAPON3[2]
};
static ePlayerSpawn[MAX_PLAYERS][e_SPAWN];
static eClassSpawn[MAX_CLASSES][e_SPAWN];
static bool:bToggleSetSpawnInfo[MAX_PLAYERS];

static const Float:WEAPON_DAMAGE[] =
{
	1.0, // 0 - Fist
	1.0, // 1 - Brass knuckles
	1.0, // 2 - Golf club
	1.0, // 3 - Nitestick
	1.0, // 4 - Knife
	1.0, // 5 - Bat
	1.0, // 6 - Shovel
	1.0, // 7 - Pool cue
	1.0, // 8 - Katana
	1.0, // 9 - Chainsaw
	1.0, // 10 - Dildo
	1.0, // 11 - Dildo 2
	1.0, // 12 - Vibrator
	1.0, // 13 - Vibrator 2
	1.0, // 14 - Flowers
	1.0, // 15 - Cane
	82.5, // 16 - Grenade
	0.0, // 17 - Teargas
	1.0, // 18 - Molotov
	9.9, // 19 - Vehicle M4 (custom)
	46.2, // 20 - Vehicle minigun (custom)
	0.0, // 21
	8.25, // 22 - Colt 45
	13.2, // 23 - Silenced
	46.2, // 24 - Deagle
	3.3, // 25 - Shotgun
	3.3, // 26 - Sawed-off
	4.95, // 27 - Spas
	6.6, // 28 - UZI
	8.25, // 29 - MP5
	9.9, // 30 - AK47
	9.9, // 31 - M4
	6.6, // 32 - Tec9
	24.75, // 33 - Cuntgun
	41.25, // 34 - Sniper
	82.5, // 35 - Rocket launcher
	82.5, // 36 - Heatseeker
	1.0, // 37 - Flamethrower
	46.2, // 38 - Minigun
	82.5, // 39 - Satchel
	0.0, // 40 - Detonator
	0.33, // 41 - Spraycan
	0.33, // 42 - Fire extinguisher
	0.0, // 43 - Camera
	0.0, // 44 - Night vision
	0.0, // 45 - Infrared
	0.0, // 46 - Parachute
	0.0, // 47 - Fake pistol
	2.64, // 48 - Pistol whip (custom)
	9.9, // 49 - Vehicle
	330.0, // 50 - Helicopter blades
	82.5, // 51 - Explosion
	1.0, // 52 - Car park (custom)
	1.0, // 53 - Drowning
	165.0, // 54 - Splat
	0.0, // 55 - Unknown
};

static iPlayerWeaponData[MAX_PLAYERS][14][2];
static Float:fPlayerWeaponDamage[MAX_PLAYERS][sizeof(WEAPON_DAMAGE)];

static iStaticPickup[MAX_PICKUPS];

static const Float:PAY_N_SPRAY[][] =
{
	{1025.05, -1024.23, 32.1}, // LS Temple
 	{487.68, -1740.87, 11.13}, // LS Santa Maria
  	{-1420.73, 2583.37, 55.56}, // El Quebrados
   	{-1904.39, 284.97, 40.75}, // Wang Cars
    {-2425.91, 1022.33, 50.10}, // Juniper Hill
    {1975.60, 2162.16, 10.77}, // LV Redsands
    {2065.38, -1831.51, 13.25}, // Idlewood
    {-99.55, 1118.36, 19.44}, // Fort Carson
    {721.07, -455.94, 16.04}, // Dillimore
    {2393.74, 1493.01, 10.52} // LV Unused (Pyramid)
};

static const COMPONENT_PRICE[] =
{
	400, 550, 200, 250, 100, 150, 80, 500, 200, 500,
 	1000, 220, 250, 100, 400, 500, 200, 500, 350, 300,
    250, 200, 150, 350, 50, 1000, 480, 480, 770, 680,
    37, 370, 170, 120, 790, 150, 500, 690, 190, 390,
    500, 390, 1000, 500, 500, 510, 710, 670, 530, 810,
    620, 670, 530, 130, 210, 230, 520, 430, 620, 720,
    530, 180, 250, 430, 830, 850, 750, 250, 200, 550,
    450, 550, 450, 1000, 1030, 980, 1560, 1620, 1200,
	1030, 1000, 1230, 820, 1560, 1350, 770, 100, 1500,
	150, 650, 450, 100, 750, 350, 450, 350, 1000, 620,
	1140, 1000, 940, 780, 830, 3250, 1610, 1540, 780,
	780, 780, 1610, 1540, -1, -1, 3340, 3250, 2130, 2050,
	2130, 780, 940, 780, 940, 780, 860, 780, 1120, 3340,
	3250, 3340, 1650, 3380, 3290, 1590, 830, 800, 1500,
	1000, 800, 580, 470, 870, 980, 150, 150, 100, 100,
	490, 600, 890, 1000, 1090, 840, 910, 1200, 1030, 1030,
	920, 930, 550, 1050, 1050, 950, 650, 550, 450, 850,
	950, 850, 950, 970, 880, 990, 900, 950, 1000, 900,
	1000, 900, 2050, 2150, 2130, 2050, 2130, 2040, 2150,
	2040, 2095, 2175, 2080, 2200, 1200, 1040, 940, 1100
};

enum e_VENDING_MACHINE
{
	e_VENDING_MACHINE_MODEL,
	e_VENDING_MACHINE_INTERIOR,
	Float:e_VENDING_MACHINE_X,
	Float:e_VENDING_MACHINE_Y,
	Float:e_VENDING_MACHINE_Z,
	Float:e_VENDING_MACHINE_RX,
	Float:e_VENDING_MACHINE_RY,
	Float:e_VENDING_MACHINE_RZ,
	Float:e_VENDING_MACHINE_FX,
	Float:e_VENDING_MACHINE_FY
};
static const Float:VENDING_MACHINE[][e_VENDING_MACHINE] =
{
	{955, 0, -862.82, 1536.60, 21.98, 0.00, 0.00, 180.00, -862.84, 1537.60},
	{956, 0, 2271.72, -76.46, 25.96, 0.00, 0.00, 0.00, 2271.72, -77.46},
	{955, 0, 1277.83, 372.51, 18.95, 0.00, 0.00, 64.00, 1278.73, 372.07},
	{956, 0, 662.42, -552.16, 15.71, 0.00, 0.00, 180.00, 662.41, -551.16},
	{955, 0, 201.01, -107.61, 0.89, 0.00, 0.00, 270.00, 200.01, -107.63},
	{955, 0, -253.74, 2597.95, 62.24, 0.00, 0.00, 90.00, -252.74, 2597.95},
	{956, 0, -253.74, 2599.75, 62.24, 0.00, 0.00, 90.00, -252.74, 2599.75},
	{956, 0, -76.03, 1227.99, 19.12, 0.00, 0.00, 90.00, -75.03, 1227.99},
	{955, 0, -14.70, 1175.35, 18.95, 0.00, 0.00, 180.00, -14.72, 1176.35},
	{1977, 7, 316.87, -140.35, 998.58, 0.00, 0.00, 270.00, 315.87, -140.36},
	{1775, 17, 373.82, -178.14, 1000.73, 0.00, 0.00, 0.00, 373.82, -179.14},
	{1776, 17, 379.03, -178.88, 1000.73, 0.00, 0.00, 270.00, 378.03, -178.90},
	{1775, 17, 495.96, -24.32, 1000.73, 0.00, 0.00, 180.00, 495.95, -23.32},
	{1776, 17, 500.56, -1.36, 1000.73, 0.00, 0.00, 0.00, 500.56, -2.36},
	{1775, 17, 501.82, -1.42, 1000.73, 0.00, 0.00, 0.00, 501.82, -2.42},
	{956, 0, -1455.11, 2591.66, 55.23, 0.00, 0.00, 180.00, -1455.13, 2592.66},
	{955, 0, 2352.17, -1357.15, 23.77, 0.00, 0.00, 90.00, 2353.17, -1357.15},
	{955, 0, 2325.97, -1645.13, 14.21, 0.00, 0.00, 0.00, 2325.97, -1646.13},
	{956, 0, 2139.51, -1161.48, 23.35, 0.00, 0.00, 87.00, 2140.51, -1161.53},
	{956, 0, 2153.23, -1016.14, 62.23, 0.00, 0.00, 127.00, 2154.03, -1015.54},
	{955, 0, 1928.73, -1772.44, 12.94, 0.00, 0.00, 90.00, 1929.73, -1772.44},
	{1776, 1, 2222.36, 1602.64, 1000.06, 0.00, 0.00, 90.00, 2223.36, 1602.64},
	{1775, 1, 2222.20, 1606.77, 1000.05, 0.00, 0.00, 90.00, 2223.20, 1606.77},
	{1775, 1, 2155.90, 1606.77, 1000.05, 0.00, 0.00, 90.00, 2156.90, 1606.77},
	{1775, 1, 2209.90, 1607.19, 1000.05, 0.00, 0.00, 270.00, 2208.90, 1607.17},
	{1776, 1, 2155.84, 1607.87, 1000.06, 0.00, 0.00, 90.00, 2156.84, 1607.87},
	{1776, 1, 2202.45, 1617.00, 1000.06, 0.00, 0.00, 180.00, 2202.43, 1618.00},
	{1776, 1, 2209.24, 1621.21, 1000.06, 0.00, 0.00, 0.00, 2209.24, 1620.21},
	{1776, 3, 330.67, 178.50, 1020.07, 0.00, 0.00, 0.00, 330.67, 177.50},
	{1776, 3, 331.92, 178.50, 1020.07, 0.00, 0.00, 0.00, 331.92, 177.50},
	{1776, 3, 350.90, 206.08, 1008.47, 0.00, 0.00, 90.00, 351.90, 206.08},
	{1776, 3, 361.56, 158.61, 1008.47, 0.00, 0.00, 180.00, 361.54, 159.61},
	{1776, 3, 371.59, 178.45, 1020.07, 0.00, 0.00, 0.00, 371.59, 177.45},
	{1776, 3, 374.89, 188.97, 1008.47, 0.00, 0.00, 0.00, 374.89, 187.97},
	{1775, 2, 2576.70, -1284.43, 1061.09, 0.00, 0.00, 270.00, 2575.70, -1284.44},
	{1775, 15, 2225.20, -1153.42, 1025.90, 0.00, 0.00, 270.00, 2224.20, -1153.43},
	{955, 0, 1154.72, -1460.89, 15.15, 0.00, 0.00, 270.00, 1153.72, -1460.90},
	{956, 0, 2480.85, -1959.27, 12.96, 0.00, 0.00, 180.00, 2480.84, -1958.27},
	{955, 0, 2060.11, -1897.64, 12.92, 0.00, 0.00, 0.00, 2060.11, -1898.64},
	{955, 0, 1729.78, -1943.04, 12.94, 0.00, 0.00, 0.00, 1729.78, -1944.04},
	{956, 0, 1634.10, -2237.53, 12.89, 0.00, 0.00, 0.00, 1634.10, -2238.53},
	{955, 0, 1789.21, -1369.26, 15.16, 0.00, 0.00, 270.00, 1788.21, -1369.28},
	{956, 0, -2229.18, 286.41, 34.70, 0.00, 0.00, 180.00, -2229.20, 287.41},
	{955, 256, -1980.78, 142.66, 27.07, 0.00, 0.00, 270.00, -1981.78, 142.64},
	{955, 256, -2118.96, -423.64, 34.72, 0.00, 0.00, 255.00, -2119.93, -423.40},
	{955, 256, -2118.61, -422.41, 34.72, 0.00, 0.00, 255.00, -2119.58, -422.17},
	{955, 256, -2097.27, -398.33, 34.72, 0.00, 0.00, 180.00, -2097.29, -397.33},
	{955, 256, -2092.08, -490.05, 34.72, 0.00, 0.00, 0.00, -2092.08, -491.05},
	{955, 256, -2063.27, -490.05, 34.72, 0.00, 0.00, 0.00, -2063.27, -491.05},
	{955, 256, -2005.64, -490.05, 34.72, 0.00, 0.00, 0.00, -2005.64, -491.05},
	{955, 256, -2034.46, -490.05, 34.72, 0.00, 0.00, 0.00, -2034.46, -491.05},
	{955, 256, -2068.56, -398.33, 34.72, 0.00, 0.00, 180.00, -2068.58, -397.33},
	{955, 256, -2039.85, -398.33, 34.72, 0.00, 0.00, 180.00, -2039.86, -397.33},
	{955, 256, -2011.14, -398.33, 34.72, 0.00, 0.00, 180.00, -2011.15, -397.33},
	{955, 2048, -1350.11, 492.28, 10.58, 0.00, 0.00, 90.00, -1349.11, 492.28},
	{956, 2048, -1350.11, 493.85, 10.58, 0.00, 0.00, 90.00, -1349.11, 493.85},
	{955, 0, 2319.99, 2532.85, 10.21, 0.00, 0.00, 0.00, 2319.99, 2531.85},
	{956, 0, 2845.72, 1295.04, 10.78, 0.00, 0.00, 0.00, 2845.72, 1294.04},
	{955, 0, 2503.14, 1243.69, 10.21, 0.00, 0.00, 180.00, 2503.12, 1244.69},
	{956, 0, 2647.69, 1129.66, 10.21, 0.00, 0.00, 0.00, 2647.69, 1128.66},
	{1209, 0, -2420.21, 984.57, 44.29, 0.00, 0.00, 90.00, -2419.21, 984.57},
	{1302, 0, -2420.17, 985.94, 44.29, 0.00, 0.00, 90.00, -2419.17, 985.94},
	{955, 0, 2085.77, 2071.35, 10.45, 0.00, 0.00, 90.00, 2086.77, 2071.35},
	{956, 0, 1398.84, 2222.60, 10.42, 0.00, 0.00, 180.00, 1398.82, 2223.60},
	{956, 0, 1659.46, 1722.85, 10.21, 0.00, 0.00, 0.00, 1659.46, 1721.85},
	{955, 0, 1520.14, 1055.26, 10.00, 0.00, 0.00, 270.00, 1519.14, 1055.24},
	{1775, 6, -19.03, -57.83, 1003.63, 0.00, 0.00, 180.00, -19.05, -56.83},
	{1775, 18, -16.11, -91.64, 1003.63, 0.00, 0.00, 180.00, -16.13, -90.64},
	{1775, 16, -15.10, -140.22, 1003.63, 0.00, 0.00, 180.00, -15.11, -139.22},
	{1775, 17, -32.44, -186.69, 1003.63, 0.00, 0.00, 180.00, -32.46, -185.69},
	{1775, 16, -35.72, -140.22, 1003.63, 0.00, 0.00, 180.00, -35.74, -139.22},
	{1776, 6, -36.14, -57.87, 1003.63, 0.00, 0.00, 180.00, -36.16, -56.87},
	{1776, 18, -17.54, -91.71, 1003.63, 0.00, 0.00, 180.00, -17.56, -90.71},
	{1776, 16, -16.53, -140.29, 1003.63, 0.00, 0.00, 180.00, -16.54, -139.29},
	{1776, 17, -33.87, -186.76, 1003.63, 0.00, 0.00, 180.00, -33.89, -185.76},
	{1775, 6, -19.03, -57.83, 1003.63, 0.00, 0.00, 180.00, -19.05, -56.83},
	{1776, 6, -36.14, -57.87, 1003.63, 0.00, 0.00, 180.00, -36.16, -56.87},
	{1775, 18, -16.11, -91.64, 1003.63, 0.00, 0.00, 180.00, -16.13, -90.64},
	{1776, 18, -17.54, -91.71, 1003.63, 0.00, 0.00, 180.00, -17.56, -90.71},
	{1776, 16, -16.53, -140.29, 1003.63, 0.00, 0.00, 180.00, -16.54, -139.29},
	{1775, 16, -15.10, -140.22, 1003.63, 0.00, 0.00, 180.00, -15.11, -139.22},
	{1776, 17, -33.87, -186.76, 1003.63, 0.00, 0.00, 180.00, -33.89, -185.76},
	{1775, 17, -32.44, -186.69, 1003.63, 0.00, 0.00, 180.00, -32.46, -185.69},
	{1775, 16, -35.72, -140.22, 1003.63, 0.00, 0.00, 180.00, -35.74, -139.22}
};

static iPlayerVendingTick[MAX_PLAYERS];

static AC_GetModelWeaponid(weaponid)
{
	switch (weaponid)
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
	return 0;
}

static AC_GetWeaponSlot(weaponid)
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

static AC_HasSameTeam(playerid, otherid)
{
	if (ePlayerStats[playerid][e_PLAYER_STATS_TEAMID] == NO_TEAM || ePlayerStats[otherid][e_PLAYER_STATS_TEAMID] == NO_TEAM)
	{
		return 0;
	}
	return (ePlayerStats[playerid][e_PLAYER_STATS_TEAMID] == ePlayerStats[otherid][e_PLAYER_STATS_TEAMID]);
}

static AC_ProcessDamage(&playerid, &issuerid, &Float:amount, &weaponid, &bodypart) // Slice's weapon-config "ProcessDamage"
{
    switch (amount)
	{
		case 3.63000011444091796875,
		     5.940000057220458984375,
		     5.610000133514404296875:
	 	{
			amount = 2.6400001049041748046875;
		}

		case 3.30000019073486328125:
		{
			if (weaponid != WEAPON_SHOTGUN && weaponid != WEAPON_SAWEDOFF)
			{
				amount = 2.6400001049041748046875;
			}
		}

		case 4.950000286102294921875:
		{
			if ((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP)
			{
				amount = 2.6400001049041748046875;
			}
		}

		case 6.270000457763671875,
		     6.93000030517578125,
		     7.2600002288818359375,
		     7.9200000762939453125,
		     8.5799999237060546875,
		     9.24000072479248046875,
		     11.88000011444091796875,
		     11.22000026702880859375:
	 	{
			amount = 2.6400001049041748046875;
		}

		case 9.90000057220458984375:
		{
			switch (weaponid)
			{
				case WEAPON_VEHICLE,
					 WEAPON_VEHICLE_M4,
					 WEAPON_AK47,
					 WEAPON_M4,
					 WEAPON_SHOTGUN,
					 WEAPON_SAWEDOFF,
					 WEAPON_SHOTGSPA:
			 	{}

				default:
				{
					amount = 6.6000003814697265625;
				}
			}
		}
	}

	if (weaponid == WEAPON_HELIBLADES && _:amount != _:330.0)
	{
		weaponid = WEAPON_CARPARK;
	}

	if (weaponid == WEAPON_PARACHUTE)
	{
		weaponid = WEAPON_UNARMED;
	}

    switch (weaponid)
	{
		case WEAPON_COLT45..WEAPON_SNIPER,
		     WEAPON_MINIGUN,
			 WEAPON_SPRAYCAN,
			 WEAPON_FIREEXTINGUISHER:
	 	{
			if (_:amount == _:2.6400001049041748046875)
			{
				bodypart = 4; // BODY_PART_TORSO
				weaponid = WEAPON_PISTOLWHIP;
			}
		}
	}

	if (issuerid == INVALID_PLAYER_ID || IsPlayerInAnyVehicle(issuerid))
	{
	    switch (weaponid)
	    {
	        case WEAPON_UNARMED..WEAPON_CANE,
			  	 WEAPON_COLT45..WEAPON_SNIPER,
				 WEAPON_MINIGUN,
		  		 WEAPON_PISTOLWHIP:
			{
			    return 0;
			}
	    }
	}

    if (weaponid != WEAPON_PISTOLWHIP)
	{
		switch (amount)
		{
			case 1.32000005245208740234375,
			     1.650000095367431640625,
			     1.980000019073486328125,
			     2.3100001811981201171875,
			     2.6400001049041748046875,
			     2.9700000286102294921875,
			     3.96000003814697265625,
			     4.28999996185302734375,
			     4.62000036239624023437,
			     5.280000209808349609375:
		 	{
				if (!((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP))
				{
					weaponid = WEAPON_UNARMED;
				}
			}

			case 6.6000003814697265625:
			{
				if (!((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP))
				{
					switch (weaponid)
					{
						case WEAPON_UZI,
							 WEAPON_TEC9,
							 WEAPON_CHAINSAW,
						     WEAPON_SHOTGUN,
							 WEAPON_SAWEDOFF:
					 	{}

						default:
						{
							weaponid = WEAPON_UNARMED;
						}
					}
				}
			}

			case 54.12000274658203125:
			{
				if (!((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP))
				{
					weaponid = WEAPON_UNARMED;
					amount = 1.32000005245208740234375;
				}

				if (GetPlayerFightingStyle(issuerid) != FIGHT_STYLE_KNEEHEAD)
					return 0;
			}

			default:
			{
				if ((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP)
				{
					amount = fPlayerWeaponDamage[issuerid][weaponid];
				}
			}
		}
	}

	switch (weaponid)
	{
		case WEAPON_UNARMED,
			 WEAPON_PISTOLWHIP,
			 WEAPON_BRASSKNUCKLE..WEAPON_KATANA,
             WEAPON_DILDO..WEAPON_CANE:
		{
		    amount = fPlayerWeaponDamage[issuerid][weaponid];
		}

	    case WEAPON_CHAINSAW:
	    {
	        switch (amount)
			{
				case 6.6000003814697265625,
				     13.5300006866455078125,
				     16.1700000762939453125,
				     26.40000152587890625,
				     27.060001373291015625:
			 	{
					amount *= fPlayerWeaponDamage[issuerid][weaponid];
				}

				default:
					amount = fPlayerWeaponDamage[issuerid][weaponid];
			}
	    }

	    case WEAPON_COLLISION:
	    {
			if (amount > WEAPON_DAMAGE[weaponid])
				amount = 1.0;
			else
				amount /= WEAPON_DAMAGE[weaponid];

	        amount *= fPlayerWeaponDamage[playerid][weaponid];
	    }

	    case WEAPON_DROWN:
	    {
	    	if (amount > 10.0)
				amount = 10.0;

	        amount *= fPlayerWeaponDamage[playerid][weaponid];
	    }

		case WEAPON_GRENADE,
			 WEAPON_ROCKETLAUNCHER,
			 WEAPON_HEATSEEKER,
			 WEAPON_SATCHEL,
			 WEAPON_EXPLOSION:
		{
		    if (issuerid == INVALID_PLAYER_ID)
				amount = (amount / fPlayerWeaponDamage[playerid][weaponid]) * fPlayerWeaponDamage[playerid][weaponid];
			else
				amount = (amount / fPlayerWeaponDamage[issuerid][weaponid]) * fPlayerWeaponDamage[issuerid][weaponid];
		}

		case WEAPON_MOLTOV,
			 WEAPON_FLAMETHROWER,
			 WEAPON_CARPARK,
			 WEAPON_VEHICLE:
		{
		    if (issuerid == INVALID_PLAYER_ID)
		   	 	amount = fPlayerWeaponDamage[playerid][weaponid];
			else
		   	 	amount = fPlayerWeaponDamage[issuerid][weaponid];
		}

		case WEAPON_COLT45..WEAPON_DEAGLE,
			 WEAPON_UZI..WEAPON_SNIPER,
			 WEAPON_MINIGUN,
			 WEAPON_SPRAYCAN,
			 WEAPON_FIREEXTINGUISHER,
			 WEAPON_HELIBLADES:
		{
		    amount = fPlayerWeaponDamage[issuerid][weaponid];
		}

		case WEAPON_SHOTGSPA:
	 	{
	 	    new Float:bullets = amount / WEAPON_DAMAGE[weaponid];
			if (8.0 - bullets < -0.05)
   	 			bullets = 1.0;

			new Float:f = floatfract(bullets);
			if (f > 0.01 && f < 0.99)
				bullets = 1.0;

	 	    amount = fPlayerWeaponDamage[issuerid][weaponid] * bullets;
		}

		case WEAPON_SHOTGUN,
			 WEAPON_SAWEDOFF:
	 	{
	 	    new Float:bullets = amount / WEAPON_DAMAGE[weaponid];
			if (15.0 - bullets < -0.05)
				bullets = 1.0;

			new Float:f = floatfract(bullets);
			if (f > 0.01 && f < 0.99)
				bullets = 1.0;

	 	    amount = fPlayerWeaponDamage[issuerid][weaponid] * bullets;
		}

		default:
			return 0;
	}
	return 1;
}

public OnGameModeInit()
{
	if (iScriptType == 0)
	{
		iScriptType = 1;

		#if defined OnScriptInit
			OnScriptInit();
		#endif
	}

	#if defined AC_OnGameModeInit
	    return AC_OnGameModeInit();
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit AC_OnGameModeInit
#if defined AC_OnGameModeInit
	forward AC_OnGameModeInit();
#endif

public OnFilterScriptInit()
{
	if (iScriptType == 0)
	{
		iScriptType = 2;

		#if defined OnScriptInit
			OnScriptInit();
		#endif
	}

	#if defined AC_OnFilterScriptInit
	    return AC_OnFilterScriptInit();
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif
#define OnFilterScriptInit AC_OnFilterScriptInit
#if defined AC_OnFilterScriptInit
	forward AC_OnFilterScriptInit();
#endif

public OnPlayerConnect(playerid)
{
	SetPlayerTeam(playerid, 655);
	ePlayerStats[playerid][e_PLAYER_STATS_TEAMID] = NO_TEAM;

	for (new i; i < sizeof(WEAPON_DAMAGE); i++)
	{
		fPlayerWeaponDamage[playerid][i] = WEAPON_DAMAGE[i];
	}
	
	ePlayerStats[playerid][e_PLAYER_STATS_CLASSID] = 0;

	ePlayerSpawn[playerid][e_SPAWN_TEAMID] = NO_TEAM;
	ePlayerSpawn[playerid][e_SPAWN_WEAPON1][0] = 0;
	ePlayerSpawn[playerid][e_SPAWN_WEAPON1][1] = 0;
	ePlayerSpawn[playerid][e_SPAWN_WEAPON2][0] = 0;
	ePlayerSpawn[playerid][e_SPAWN_WEAPON2][1] = 0;
	ePlayerSpawn[playerid][e_SPAWN_WEAPON3][0] = 0;
	ePlayerSpawn[playerid][e_SPAWN_WEAPON3][1] = 0;

	#if defined AC_OnPlayerConnect
		return AC_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect AC_OnPlayerConnect
#if defined AC_OnPlayerConnect
	forward AC_OnPlayerConnect(playerid);
#endif

public OnPlayerRequestClass(playerid, classid)
{
	ePlayerStats[playerid][e_PLAYER_STATS_CLASSID] = classid;

	#if defined AC_OnPlayerRequestClass
		return AC_OnPlayerRequestClass(playerid, classid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerRequestClass
    #undef OnPlayerRequestClass
#else
    #define _ALS_OnPlayerRequestClass
#endif
#define OnPlayerRequestClass AC_OnPlayerRequestClass
#if defined AC_OnPlayerRequestClass
	forward AC_OnPlayerRequestClass(playerid, classid);
#endif

public OnPlayerSpawn(playerid)
{
	ePlayerStats[playerid][e_PLAYER_STATS_HEALTH] = 100.0;
	SetPlayerHealth(playerid, 100.0);
	ePlayerStats[playerid][e_PLAYER_STATS_ARMOUR] = 0.0;
	SetPlayerArmour(playerid, 0.0);

	ResetPlayerWeapons(playerid);
	for (new i; i < sizeof (iPlayerWeaponData[]); i++)
	{
	    iPlayerWeaponData[playerid][i][0] = WEAPON_UNARMED;
	    iPlayerWeaponData[playerid][i][1] = 1;
	}

	new slot;
	if (!bToggleSetSpawnInfo[playerid])
	{
		slot = AC_GetWeaponSlot(eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON1][0]);
		if (slot != -1)
		{
			iPlayerWeaponData[playerid][slot][0] = eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON1][0];
			iPlayerWeaponData[playerid][slot][1] = eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON1][1];
			if (IS_GAMEMODE)
			{
				GivePlayerWeapon(playerid, eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON1][0], eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON1][1]);
			}
		}
	
		slot = AC_GetWeaponSlot(eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON2][0]);
		if (slot != -1)
		{
			iPlayerWeaponData[playerid][slot][0] = eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON2][0];
			iPlayerWeaponData[playerid][slot][1] = eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON2][1];
			if (IS_GAMEMODE)
			{
				GivePlayerWeapon(playerid, eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON2][0], eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON2][1]);
			}
		}
		
		slot = AC_GetWeaponSlot(eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON3][0]);
		if (slot != -1)
		{
			iPlayerWeaponData[playerid][slot][0] = eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON3][0];
			iPlayerWeaponData[playerid][slot][1] = eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON3][1];
			if (IS_GAMEMODE)
			{
				GivePlayerWeapon(playerid, eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON3][0], eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_WEAPON3][1]);
			}
		}
		
		ePlayerStats[playerid][e_PLAYER_STATS_TEAMID] = eClassSpawn[ePlayerStats[playerid][e_PLAYER_STATS_CLASSID]][e_SPAWN_TEAMID];
	}
	else
	{
	    bToggleSetSpawnInfo[playerid] = false;

		slot = AC_GetWeaponSlot(ePlayerSpawn[playerid][e_SPAWN_WEAPON1][0]);
		if (slot != -1)
		{
			iPlayerWeaponData[playerid][slot][0] = ePlayerSpawn[playerid][e_SPAWN_WEAPON1][0];
			iPlayerWeaponData[playerid][slot][1] = ePlayerSpawn[playerid][e_SPAWN_WEAPON1][1];
			if (IS_GAMEMODE)
			{
				GivePlayerWeapon(playerid, ePlayerSpawn[playerid][e_SPAWN_WEAPON1][0], ePlayerSpawn[playerid][e_SPAWN_WEAPON1][1]);
			}
		}
		
		slot = AC_GetWeaponSlot(ePlayerSpawn[playerid][e_SPAWN_WEAPON2][0]);
		if (slot != -1)
		{
			iPlayerWeaponData[playerid][slot][0] = ePlayerSpawn[playerid][e_SPAWN_WEAPON2][0];
			iPlayerWeaponData[playerid][slot][1] = ePlayerSpawn[playerid][e_SPAWN_WEAPON2][1];
			if (IS_GAMEMODE)
			{
				GivePlayerWeapon(playerid, ePlayerSpawn[playerid][e_SPAWN_WEAPON2][0], ePlayerSpawn[playerid][e_SPAWN_WEAPON2][1]);
			}
		}
		
		slot = AC_GetWeaponSlot(ePlayerSpawn[playerid][e_SPAWN_WEAPON3][0]);
		if (slot != -1)
		{
			iPlayerWeaponData[playerid][slot][0] = ePlayerSpawn[playerid][e_SPAWN_WEAPON3][0];
			iPlayerWeaponData[playerid][slot][1] = ePlayerSpawn[playerid][e_SPAWN_WEAPON3][1];
			if (IS_GAMEMODE)
			{
				GivePlayerWeapon(playerid, ePlayerSpawn[playerid][e_SPAWN_WEAPON3][0], ePlayerSpawn[playerid][e_SPAWN_WEAPON3][1]);
			}
		}
		
		ePlayerStats[playerid][e_PLAYER_STATS_TEAMID] = ePlayerSpawn[playerid][e_SPAWN_TEAMID];
	}

	#if defined AC_OnPlayerSpawn
		return AC_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn AC_OnPlayerSpawn
#if defined AC_OnPlayerSpawn
	forward AC_OnPlayerSpawn(playerid);
#endif

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (newkeys & KEY_FIRE)
 	{
		if (GetPlayerWeapon(playerid) == WEAPON_SATCHEL)
		{
	        new slot = AC_GetWeaponSlot(WEAPON_BOMB);
			if (slot != -1)
			{
				iPlayerWeaponData[playerid][slot][0] = WEAPON_BOMB;
				iPlayerWeaponData[playerid][slot][1] = 1;
		    }

			if (IS_GAMEMODE)
			{
				GivePlayerWeapon(playerid, WEAPON_BOMB, 1);
			}
		}
	}
	
	#if defined AC_OnPlayerKeyStateChange
		return AC_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange AC_OnPlayerKeyStateChange
#if defined AC_OnPlayerKeyStateChange
	forward AC_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if (!((WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) || weaponid == WEAPON_MINIGUN))
		return 0;

	ePlayerShot[playerid][e_PLAYER_SHOT_TYPE] = hittype;
	ePlayerShot[playerid][e_PLAYER_SHOT_HITID] = hitid;
	#if defined AC_OnPlayerWeaponShot
	    ePlayerShot[playerid][e_PLAYER_SHOT_RETURN] = bool:AC_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	#else
	    ePlayerShot[playerid][e_PLAYER_SHOT_RETURN] = true;
	#endif

	return ePlayerShot[playerid][e_PLAYER_SHOT_RETURN];
}
#if defined _ALS_OnPlayerWeaponShot
    #undef OnPlayerWeaponShot
#else
    #define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot AC_OnPlayerWeaponShot
#if defined AC_OnPlayerWeaponShot
	forward AC_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	if (issuerid != INVALID_PLAYER_ID)
		return 1;

	switch (weaponid)
 	{
 		case WEAPON_GRENADE,
			 WEAPON_MOLTOV,
			 WEAPON_ROCKETLAUNCHER,
			 WEAPON_HEATSEEKER,
			 WEAPON_SATCHEL,
			 WEAPON_FLAMETHROWER,
			 WEAPON_SPRAYCAN,
			 WEAPON_FIREEXTINGUISHER,
			 WEAPON_VEHICLE,
			 WEAPON_CARPARK,
			 WEAPON_HELIBLADES,
			 WEAPON_EXPLOSION,
			 WEAPON_DROWN:
    	{}

    	case WEAPON_COLLISION:
    	{
    	    if (1061 <= GetPlayerAnimationIndex(playerid) <= 1067)
			{
			    TogglePlayerControllable(playerid, false);
				ClearAnimations(playerid);
			    TogglePlayerControllable(playerid, true);
				return 1;
			}
	 	}

    	default:
    	{
    	    return 1;
    	}
	}

	if (!AC_ProcessDamage(playerid, issuerid, amount, weaponid, bodypart))
		return 1;

    if (IS_GAMEMODE)
    {
		if ((ePlayerStats[playerid][e_PLAYER_STATS_HEALTH] - amount) <= 0.0)
		{
		    ePlayerDeath[playerid][e_PLAYER_DEATH_VALID] = true;
			ePlayerDeath[playerid][e_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
			ePlayerDeath[playerid][e_PLAYER_DEATH_WEAPONID] = weaponid;
		}
	}
	
	#if defined AC_OnPlayerTakeDamage
		return AC_OnPlayerTakeDamage(playerid, issuerid, amount, weaponid, bodypart);
	#else
		return 0;
	#endif
}
#if defined _ALS_OnPlayerTakeDamage
    #undef OnPlayerTakeDamage
#else
    #define _ALS_OnPlayerTakeDamage
#endif
#define OnPlayerTakeDamage AC_OnPlayerTakeDamage
#if defined AC_OnPlayerTakeDamage
	forward AC_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
    #if defined AC_OnPlayerGiveDamage
		AC_OnPlayerGiveDamage(playerid, damagedid, amount, weaponid, bodypart);
	#endif

	if (AC_HasSameTeam(playerid, damagedid))
		return 1;

	if (!AC_ProcessDamage(damagedid, playerid, amount, weaponid, bodypart))
		return 1;

	if ((WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) || weaponid == WEAPON_MINIGUN)
    {
        if (ePlayerShot[playerid][e_PLAYER_SHOT_TYPE] == BULLET_HIT_TYPE_PLAYER && ePlayerShot[playerid][e_PLAYER_SHOT_HITID] == damagedid && !ePlayerShot[playerid][e_PLAYER_SHOT_RETURN])
      	{
      	    ePlayerShot[playerid][e_PLAYER_SHOT_TYPE] = BULLET_HIT_TYPE_NONE;
      	    ePlayerShot[playerid][e_PLAYER_SHOT_HITID] = INVALID_PLAYER_ID;
      	    ePlayerShot[playerid][e_PLAYER_SHOT_RETURN] = false;
	      	return 0;
		}

		ePlayerShot[playerid][e_PLAYER_SHOT_TYPE] = BULLET_HIT_TYPE_NONE;
  		ePlayerShot[playerid][e_PLAYER_SHOT_HITID] = INVALID_PLAYER_ID;
    	ePlayerShot[playerid][e_PLAYER_SHOT_RETURN] = false;
    }

    if (IS_GAMEMODE)
    {
		if (ePlayerStats[damagedid][e_PLAYER_STATS_ARMOUR] > 0.0)
		{
			ePlayerStats[damagedid][e_PLAYER_STATS_ARMOUR] -= amount;
		  	if (ePlayerStats[damagedid][e_PLAYER_STATS_ARMOUR] < 0.0)
		  	{
				SetPlayerArmour(damagedid, 0.0);
                ePlayerStats[damagedid][e_PLAYER_STATS_ARMOUR] = 0.0;
                
				SetPlayerHealth(damagedid, (ePlayerStats[damagedid][e_PLAYER_STATS_HEALTH] - ePlayerStats[damagedid][e_PLAYER_STATS_ARMOUR]));
				ePlayerStats[damagedid][e_PLAYER_STATS_HEALTH] -= ePlayerStats[damagedid][e_PLAYER_STATS_ARMOUR];
			}
			else
			{
			    SetPlayerArmour(damagedid, ePlayerStats[damagedid][e_PLAYER_STATS_ARMOUR]);
			}
		}
		else
		{
			SetPlayerHealth(damagedid, (ePlayerStats[damagedid][e_PLAYER_STATS_HEALTH] - amount));
		    ePlayerStats[damagedid][e_PLAYER_STATS_HEALTH] -= amount;
		}
	}
	
    if (ePlayerStats[damagedid][e_PLAYER_STATS_HEALTH] <= 0.0)
	{
	    ePlayerDeath[damagedid][e_PLAYER_DEATH_VALID] = true;
		ePlayerDeath[damagedid][e_PLAYER_DEATH_KILLERID] = playerid;
		ePlayerDeath[damagedid][e_PLAYER_DEATH_WEAPONID] = weaponid;
	}

	#if defined AC_OnPlayerTakeDamage
		AC_OnPlayerTakeDamage(damagedid, playerid, amount, weaponid, bodypart);
	#endif
	return 0;
}
#if defined _ALS_OnPlayerGiveDamage
    #undef OnPlayerGiveDamage
#else
    #define _ALS_OnPlayerGiveDamage
#endif
#define OnPlayerGiveDamage AC_OnPlayerGiveDamage
#if defined AC_OnPlayerGiveDamage
	forward AC_OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart);
#endif

public OnPlayerDeath(playerid, killerid, reason)
{
	if (!ePlayerDeath[playerid][e_PLAYER_DEATH_VALID])
	{
	    #if defined OnPlayerFakeDeath
            OnPlayerFakeDeath(playerid, killerid, reason);
		#endif
	    return 0;
	}

	ePlayerDeath[playerid][e_PLAYER_DEATH_VALID] = false;

	killerid = ePlayerDeath[playerid][e_PLAYER_DEATH_KILLERID];
	reason = ePlayerDeath[playerid][e_PLAYER_DEATH_WEAPONID];

	#if defined AC_OnPlayerDeath
		return AC_OnPlayerDeath(playerid, killerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath AC_OnPlayerDeath
#if defined AC_OnPlayerDeath
	forward AC_OnPlayerDeath(playerid, killerid, reason);
#endif

public OnPlayerUpdate(playerid)
{
    if (IS_GAMEMODE)
    {
		if (GetPlayerState(playerid) != PLAYER_STATE_NONE && GetPlayerState(playerid) != PLAYER_STATE_WASTED && GetPlayerState(playerid) != PLAYER_STATE_SPECTATING)
		{
			new weapon = GetPlayerWeapon(playerid);
			new slot = AC_GetWeaponSlot(weapon);

			if (iPlayerWeaponData[playerid][slot][0] != weapon)
			{
			    CallRemoteFunction("OnPlayerWeaponCheat", "iiii", playerid, slot, iPlayerWeaponData[playerid][slot][0], weapon);
			    
	            ResetPlayerWeapons(playerid);
				for (new i; i < sizeof(iPlayerWeaponData[]); i++)
				{
					GivePlayerWeapon(playerid, iPlayerWeaponData[playerid][i][0], iPlayerWeaponData[playerid][i][1]);
				}
			}
			else
			{
	  			switch (weapon)
		    	{
					case WEAPON_COLT45..WEAPON_SNIPER, WEAPON_MINIGUN, WEAPON_SATCHEL:
	   				{
	   				    if (GetPlayerAmmo(playerid) != iPlayerWeaponData[playerid][slot][1])
	   				    {
						    CallRemoteFunction("OnPlayerAmmoCheat", "iiii", playerid, slot, iPlayerWeaponData[playerid][slot][1], GetPlayerAmmo(playerid));
						    
				            SetPlayerAmmo(playerid, weapon, iPlayerWeaponData[playerid][slot][1]);
						}
					}

					default:
					{
	   				    if (GetPlayerAmmo(playerid) > iPlayerWeaponData[playerid][slot][1])
	   				    {
						    CallRemoteFunction("OnPlayerAmmoCheat", "iiii", playerid, slot, iPlayerWeaponData[playerid][slot][1], GetPlayerAmmo(playerid));

				            iPlayerWeaponData[playerid][slot][0] = WEAPON_UNARMED;
							iPlayerWeaponData[playerid][slot][1] = 1;

							ResetPlayerWeapons(playerid);
							for (new i; i < sizeof(iPlayerWeaponData[]); i++)
							{
								GivePlayerWeapon(playerid, iPlayerWeaponData[playerid][i][0], iPlayerWeaponData[playerid][i][1]);
							}
						}
					}
				}
			}

			new Float:health;
			GetPlayerHealth(playerid, health);
			if (floatround(ePlayerStats[playerid][e_PLAYER_STATS_HEALTH]) <= 255 && floatround(health) != floatround(ePlayerStats[playerid][e_PLAYER_STATS_HEALTH]))
			{
			    new bool:ignore;
			    if (floatabs(floatround(health) - floatround(ePlayerStats[playerid][e_PLAYER_STATS_HEALTH])) <= 35)
			    {
				    if ((GetTickCount() - iPlayerVendingTick[playerid]) <= 2000)
				    {
				        ignore = true;
				    }
					else if (GetPlayerMoney(playerid) > 1)
		            {
		                static const Float:RADIUS = 1.2;
					    new Float:x, Float:y, Float:z;
						for (new i; i < sizeof (VENDING_MACHINE); i++)
						{
							x = VENDING_MACHINE[i][e_VENDING_MACHINE_X];
							y = VENDING_MACHINE[i][e_VENDING_MACHINE_Y];
							z = VENDING_MACHINE[i][e_VENDING_MACHINE_Z];

							z += 180.0;
							if (z > 359.0)
							{
							    z -= 359.0;
							}
							x += (RADIUS * floatsin(-z, degrees));
							y += (RADIUS * floatcos(-z, degrees));

							if (IsPlayerInRangeOfPoint(playerid, RADIUS, x, y, VENDING_MACHINE[i][e_VENDING_MACHINE_Z]))
							{
							    ignore = true;
				                ePlayerStats[playerid][e_PLAYER_STATS_MONEY] -= 1;
								if (floatround(ePlayerStats[playerid][e_PLAYER_STATS_HEALTH]) < 100 && (100 - floatround(ePlayerStats[playerid][e_PLAYER_STATS_HEALTH])) >= 35)
				                {
				                	ePlayerStats[playerid][e_PLAYER_STATS_HEALTH] += 35;
								}
				               	iPlayerVendingTick[playerid] = GetTickCount();
							    break;
							}
						}
					}
				}

				if (!ignore)
				{
				    CallRemoteFunction("OnPlayerHealthCheat", "iff", playerid, ePlayerStats[playerid][e_PLAYER_STATS_HEALTH], health);

					SetPlayerHealth(playerid, ePlayerStats[playerid][e_PLAYER_STATS_HEALTH]);
				}
			}

			new Float:armour;
			GetPlayerArmour(playerid, armour);
			if (floatround(ePlayerStats[playerid][e_PLAYER_STATS_ARMOUR]) <= 255 && floatround(armour) != floatround(ePlayerStats[playerid][e_PLAYER_STATS_ARMOUR]))
			{
			    CallRemoteFunction("OnPlayerArmourCheat", "iff", playerid, ePlayerStats[playerid][e_PLAYER_STATS_ARMOUR], armour);

				SetPlayerArmour(playerid, ePlayerStats[playerid][e_PLAYER_STATS_ARMOUR]);
			}
		}

	    if (GetPlayerMoney(playerid) != ePlayerStats[playerid][e_PLAYER_STATS_MONEY])
		{
		    new bool:ignore;
		    if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
			{
			    if (floatabs(GetPlayerMoney(playerid) - ePlayerStats[playerid][e_PLAYER_STATS_MONEY]) <= 100)
			    {
				    for (new i; i < sizeof(PAY_N_SPRAY); i++)
		      		{
		        		if (IsPlayerInRangeOfPoint(playerid, 10.0, PAY_N_SPRAY[i][0], PAY_N_SPRAY[i][1], PAY_N_SPRAY[i][2]))
			            {
			                ignore = true;
			                ePlayerStats[playerid][e_PLAYER_STATS_MONEY] -= 100;
							break;
						}
					}
				}
			}

			if (!ignore)
			{
			    CallRemoteFunction("OnPlayerMoneyCheat", "iii", playerid, ePlayerStats[playerid][e_PLAYER_STATS_MONEY], GetPlayerMoney(playerid));

				ResetPlayerMoney(playerid);
				GivePlayerMoney(playerid, ePlayerStats[playerid][e_PLAYER_STATS_MONEY]);
			}
		}
	}
	
	#if defined AC_OnPlayerUpdate
		return AC_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerUpdate
    #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate AC_OnPlayerUpdate
#if defined AC_OnPlayerUpdate
	forward AC_OnPlayerUpdate(playerid);
#endif

public OnPlayerPickUpPickup(playerid, pickupid)
{
	switch (iStaticPickup[pickupid])
 	{
  		case 1240:
	    {
     		if (ePlayerStats[playerid][e_PLAYER_STATS_HEALTH] < 100)
	        {
	            ePlayerStats[playerid][e_PLAYER_STATS_HEALTH] = 100.0;
	            if (IS_GAMEMODE)
				{
					SetPlayerHealth(playerid, 100.0);
				}
			}
			return 1;
		}

	    case 1242:
	    {
     		if (ePlayerStats[playerid][e_PLAYER_STATS_ARMOUR] < 100)
	        {
	            ePlayerStats[playerid][e_PLAYER_STATS_ARMOUR] = 100.0;
	            if (IS_GAMEMODE)
				{
					SetPlayerArmour(playerid, 100.0);
				}
			}
			return 1;
   		}

     	case 321..369, 371, 372:
      	{
       		new weapon = AC_GetModelWeaponid(iStaticPickup[pickupid]);
			new ammo;

       		switch (weapon)
		    {
		        case WEAPON_BRASSKNUCKLE..WEAPON_CANE,
			 		 WEAPON_PARACHUTE:
				{
		       		ammo = 1;
		        }

				case WEAPON_GRENADE..WEAPON_MOLTOV:
		       		ammo = 8;

		        case WEAPON_COLT45:
		       		ammo = 30;

		        case WEAPON_SILENCED,
					 WEAPON_DEAGLE:
		        {
		       		ammo = 10;
		        }

				case WEAPON_SHOTGUN:
		       		ammo = 15;

		        case WEAPON_SAWEDOFF,
					 WEAPON_SHOTGSPA:
		        {
		       		ammo = 10;
		        }

				case WEAPON_UZI,
					 WEAPON_MP5,
					 WEAPON_TEC9:
     			{
		       		ammo = 60;
		        }

				case WEAPON_AK47,
					 WEAPON_M4:
     			{
		       		ammo = 80;
		        }

				case WEAPON_RIFLE:
		       		ammo = 20;

		        case WEAPON_SNIPER:
		       		ammo = 10;

		        case WEAPON_ROCKETLAUNCHER:
		       		ammo = 4;

		        case WEAPON_HEATSEEKER:
		       		ammo = 3;

		        case WEAPON_FLAMETHROWER:
		       		ammo = 100;

		        case WEAPON_MINIGUN:
		       		ammo = 500;

		        case WEAPON_SATCHEL:
		       		ammo = 5;

		        case WEAPON_SPRAYCAN:
		       		ammo = 500;

		        case WEAPON_FIREEXTINGUISHER:
		       		ammo = 100;

		        case WEAPON_CAMERA:
		       		ammo = 36;
		    }

         	if (weapon != 0)
         	{
          		if (IS_GAMEMODE)
				{
					GivePlayerWeapon(playerid, weapon, ammo);
				}
				
				new slot = AC_GetWeaponSlot(weapon);
				if (iPlayerWeaponData[playerid][slot][0] == weapon)
				{
					iPlayerWeaponData[playerid][slot][1] += ammo;
				}
				else
				{
					iPlayerWeaponData[playerid][slot][1] = ammo;
				}
				iPlayerWeaponData[playerid][slot][0] = weapon;
			}
			return 1;
	    }
	}

	#if defined AC_OnPlayerPickUpPickup
		return AC_OnPlayerPickUpPickup(playerid, pickupid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerPickUpPickup
    #undef OnPlayerPickUpPickup
#else
    #define _ALS_OnPlayerPickUpPickup
#endif
#define OnPlayerPickUpPickup AC_OnPlayerPickUpPickup
#if defined AC_OnPlayerPickUpPickup
	forward AC_OnPlayerPickUpPickup(playerid, pickupid);
#endif

forward __SetPlayerTeam(playerid, teamid);
public 	__SetPlayerTeam(playerid, teamid)
{
    ePlayerStats[playerid][e_PLAYER_STATS_TEAMID] = teamid;
}

stock AC_SetPlayerTeam(playerid, teamid)
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
#define SetPlayerTeam AC_SetPlayerTeam

stock AC_GetPlayerTeam(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return NO_TEAM;

	return ePlayerStats[playerid][e_PLAYER_STATS_TEAMID];
}
#if defined _ALS_GetPlayerTeam
    #undef GetPlayerTeam
#else
    #define _ALS_GetPlayerTeam
#endif
#define GetPlayerTeam AC_GetPlayerTeam

forward __SetPlayerHealth(playerid, Float:health);
public 	__SetPlayerHealth(playerid, Float:health)
{
	if (health < 0.0)
		health = 0.0;

    ePlayerStats[playerid][e_PLAYER_STATS_HEALTH] = health;
}

stock AC_SetPlayerHealth(playerid, Float:health)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	new ret = SetPlayerHealth(playerid, health);
	if (bool:ret)
	{
		CallRemoteFunction("__SetPlayerHealth", "if", playerid, health);
	}
	return ret;
}
#if defined _ALS_SetPlayerHealth
    #undef SetPlayerHealth
#else
    #define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth AC_SetPlayerHealth

stock AC_GetPlayerHealth(playerid, &Float:health)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	health = ePlayerStats[playerid][e_PLAYER_STATS_HEALTH];
	return 1;
}
#if defined _ALS_GetPlayerHealth
    #undef GetPlayerHealth
#else
    #define _ALS_GetPlayerHealth
#endif
#define GetPlayerHealth AC_GetPlayerHealth

forward __SetPlayerArmour(playerid, Float:armour);
public 	__SetPlayerArmour(playerid, Float:armour)
{
	if (armour < 0.0)
		armour = 0.0;

    ePlayerStats[playerid][e_PLAYER_STATS_ARMOUR] = armour;
}

stock AC_SetPlayerArmour(playerid, Float:armour)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
	return 0;

	new ret = SetPlayerArmour(playerid, armour);
	if (bool:ret)
	{
		CallRemoteFunction("__SetPlayerArmour", "if", playerid, armour);
	}
	return ret;
}
#if defined _ALS_SetPlayerArmour
    #undef SetPlayerArmour
#else
    #define _ALS_SetPlayerArmour
#endif
#define SetPlayerArmour AC_SetPlayerArmour

stock AC_GetPlayerArmour(playerid, &Float:armour)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	armour = ePlayerStats[playerid][e_PLAYER_STATS_ARMOUR];
	return 1;
}
#if defined _ALS_GetPlayerArmour
    #undef GetPlayerArmour
#else
    #define _ALS_GetPlayerArmour
#endif
#define GetPlayerArmour AC_GetPlayerArmour

forward __SetPlayerMoney(playerid, money);
public 	__SetPlayerMoney(playerid, money)
{
    ePlayerStats[playerid][e_PLAYER_STATS_MONEY] = money;
}

stock AC_GivePlayerMoney(playerid, money)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	new ret = GivePlayerMoney(playerid, money);
	if (bool:ret)
	{
		CallRemoteFunction("__SetPlayerMoney", "ii", playerid, money + ePlayerStats[playerid][e_PLAYER_STATS_MONEY]);
	}
	return ret;
}
#if defined _ALS_GivePlayerMoney
    #undef GivePlayerMoney
#else
    #define _ALS_GivePlayerMoney
#endif
#define GivePlayerMoney AC_GivePlayerMoney

stock AC_ResetPlayerMoney(playerid)
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
#define ResetPlayerMoney AC_ResetPlayerMoney

stock AC_GetPlayerMoney(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	return ePlayerStats[playerid][e_PLAYER_STATS_MONEY];
}
#if defined _ALS_GetPlayerMoney
    #undef GetPlayerMoney
#else
    #define _ALS_GetPlayerMoney
#endif
#define GetPlayerMoney AC_GetPlayerMoney

forward __SetPlayerWeapon(playerid, slot, weaponid, ammo);
public 	__SetPlayerWeapon(playerid, slot, weaponid, ammo)
{
    iPlayerWeaponData[playerid][slot][0] = weaponid;
    iPlayerWeaponData[playerid][slot][1] = ammo;
}

stock AC_GivePlayerWeapon(playerid, weaponid, ammo)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	new slot = AC_GetWeaponSlot(weaponid);
	if (slot == -1)
		return 0;
		
	new ret = GivePlayerWeapon(playerid, weaponid, ammo);
	if (bool:ret)
	{
		CallRemoteFunction("__SetPlayerWeapon", "iiii", playerid, slot, weaponid, ammo + iPlayerWeaponData[playerid][slot][1]);
	}
	return ret;
}
#if defined _ALS_GivePlayerWeapon
    #undef GivePlayerWeapon
#else
    #define _ALS_GivePlayerWeapon
#endif
#define GivePlayerWeapon AC_GivePlayerWeapon

stock AC_ResetPlayerWeapons(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	for (new i; i < sizeof(iPlayerWeaponData[]); i++)
	{
		CallRemoteFunction("__SetPlayerWeapon", "iiii", playerid, i, 0, 0);
	}
	return ResetPlayerWeapons(playerid);
}
#if defined _ALS_ResetPlayerWeapons
    #undef ResetPlayerWeapons
#else
    #define _ALS_ResetPlayerWeapons
#endif
#define ResetPlayerWeapons AC_ResetPlayerWeapons

stock AC_GetPlayerWeaponData(playerid, slot, &weapons, &ammo)
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
#define GetPlayerWeaponData AC_GetPlayerWeaponData

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

stock AC_SetPlayerAmmo(playerid, weaponid, ammo)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	new ret = SetPlayerAmmo(playerid, weaponid, ammo);
	if (bool:ret)
	{
		CallRemoteFunction("__SetPlayerWeapon", "iiii", playerid, AC_GetWeaponSlot(weaponid), weaponid, ammo);
    }
	return ret;
}
#if defined _ALS_SetPlayerAmmo
    #undef SetPlayerAmmo
#else
    #define _ALS_SetPlayerAmmo
#endif
#define SetPlayerAmmo AC_SetPlayerAmmo

stock AC_GetPlayerAmmo(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	new slot = AC_GetWeaponSlot(GetPlayerWeapon(playerid));
	if (slot > 12 || slot < 0)
		return 0;

	return iPlayerWeaponData[playerid][slot][1];
}
#if defined _ALS_GetPlayerAmmo
    #undef GetPlayerAmmo
#else
    #define _ALS_GetPlayerAmmo
#endif
#define GetPlayerAmmo AC_GetPlayerAmmo

stock AC_AddPlayerClass(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
	new ret = AddPlayerClass(modelid, spawn_x, spawn_y, spawn_z, z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
	eClassSpawn[ret][e_SPAWN_TEAMID] = NO_TEAM;
	eClassSpawn[ret][e_SPAWN_WEAPON1][0] = weapon1;
	eClassSpawn[ret][e_SPAWN_WEAPON1][1] = weapon1_ammo;
	eClassSpawn[ret][e_SPAWN_WEAPON2][0] = weapon2;
	eClassSpawn[ret][e_SPAWN_WEAPON2][1] = weapon2_ammo;
	eClassSpawn[ret][e_SPAWN_WEAPON3][0] = weapon3;
	eClassSpawn[ret][e_SPAWN_WEAPON3][1] = weapon3_ammo;
	return ret;
}
#if defined _ALS_AddPlayerClass
    #undef AddPlayerClass
#else
    #define _ALS_AddPlayerClass
#endif
#define AddPlayerClass AC_AddPlayerClass

stock AC_AddPlayerClassEx(teamid, modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
	new ret = AddPlayerClassEx(teamid, modelid, spawn_x, spawn_y, spawn_z, z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
	eClassSpawn[ret][e_SPAWN_TEAMID] = teamid;
	eClassSpawn[ret][e_SPAWN_WEAPON1][0] = weapon1;
	eClassSpawn[ret][e_SPAWN_WEAPON1][1] = weapon1_ammo;
	eClassSpawn[ret][e_SPAWN_WEAPON2][0] = weapon2;
	eClassSpawn[ret][e_SPAWN_WEAPON2][1] = weapon2_ammo;
	eClassSpawn[ret][e_SPAWN_WEAPON3][0] = weapon3;
	eClassSpawn[ret][e_SPAWN_WEAPON3][1] = weapon3_ammo;
	return ret;
}
#if defined _ALS_AddPlayerClassEx
    #undef AddPlayerClass
#else
    #define _ALS_AddPlayerClassEx
#endif
#define AddPlayerClassEx AC_AddPlayerClassEx

forward __SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
public  __SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
    bToggleSetSpawnInfo[playerid] = true;
	ePlayerSpawn[playerid][e_SPAWN_TEAMID] = team;
	ePlayerSpawn[playerid][e_SPAWN_WEAPON1][0] = weapon1;
	ePlayerSpawn[playerid][e_SPAWN_WEAPON1][1] = weapon1_ammo;
	ePlayerSpawn[playerid][e_SPAWN_WEAPON2][0] = weapon2;
	ePlayerSpawn[playerid][e_SPAWN_WEAPON2][1] = weapon2_ammo;
	ePlayerSpawn[playerid][e_SPAWN_WEAPON3][0] = weapon3;
	ePlayerSpawn[playerid][e_SPAWN_WEAPON3][1] = weapon3_ammo;
}

stock AC_SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
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
#define SetSpawnInfo AC_SetSpawnInfo

forward __SetStaticPickup(pickupid, model);
public  __SetStaticPickup(pickupid, model)
{
    iStaticPickup[pickupid] = model;
}

stock AC_AddStaticPickup(model, type, Float:X, Float:Y, Float:Z, Virtualworld)
{
	switch (model)
	{
	    case 1240, 1242, 321..369, 371, 372:
		{}

     	default:
     	{
     		return AddStaticPickup(model, type, X, Y, Z, Virtualworld);
		}
	}
	
	new ret = CreatePickup(model, type, X, Y, Z, Virtualworld);
	if (ret == -1)
		return 0;

	CallRemoteFunction("__SetStaticPickup", "ii", ret, model);
	return 1;
}
#if defined _ALS_AddStaticPickup
    #undef AddStaticPickup
#else
    #define _ALS_AddStaticPickup
#endif
#define AddStaticPickup AC_AddStaticPickup

stock AC_CreatePickup(model, type, Float:X, Float:Y, Float:Z, Virtualworld)
{
    if (type == 1)
		return CreatePickup(model, type, X, Y, Z, Virtualworld);

	switch (model)
	{
	    case 1240, 1242, 321..369, 371, 372:
	    {}

     	default:
     	{
    	 	return CreatePickup(model, type, X, Y, Z, Virtualworld);
		}
	}
	
	new ret = CreatePickup(model, type, X, Y, Z, Virtualworld);
	if (ret == -1)
		return 0;

	CallRemoteFunction("__SetStaticPickup", "ii", ret, model);
	return 1;
}
#if defined _ALS_CreatePickup
    #undef CreatePickup
#else
    #define _ALS_CreatePickup
#endif
#define CreatePickup AC_CreatePickup

stock AC_DestroyPickup(pickupid)
{
	if (iStaticPickup[pickupid])
	{
		CallRemoteFunction("__SetStaticPickup", "ii", pickupid, 0);
	}
	return DestroyPickup(pickupid);
}
#if defined _ALS_DestroyPickup
    #undef DestroyPickup
#else
    #define _ALS_DestroyPickup
#endif
#define DestroyPickup AC_DestroyPickup

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	ePlayerStats[playerid][e_PLAYER_STATS_MONEY] -= 100;
    GivePlayerMoney(playerid, -100);

	#if defined AC_OnVehicleRespray
		return AC_OnVehicleRespray(playerid, vehicleid, color1, color2);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleRespray
    #undef OnVehicleRespray
#else
    #define _ALS_OnVehicleRespray
#endif
#define OnVehicleRespray AC_OnVehicleRespray
#if defined AC_OnVehicleRespray
	forward AC_OnVehicleRespray(playerid, vehicleid, color1, color2);
#endif

public OnVehicleMod(playerid, vehicleid, componentid)
{
    new index = (componentid - 1000);
    if (!(0 <= index < sizeof(COMPONENT_PRICE)))
        return 0;

	ePlayerStats[playerid][e_PLAYER_STATS_MONEY] -= COMPONENT_PRICE[index];
	GivePlayerMoney(playerid, -COMPONENT_PRICE[index]);

	#if defined AC_OnVehicleMod
		return AC_OnVehicleMod(playerid, vehicleid, componentid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleMod
    #undef OnVehicleMod
#else
    #define _ALS_OnVehicleMod
#endif
#define OnVehicleMod AC_OnVehicleMod
#if defined AC_OnVehicleMod
	forward AC_OnVehicleMod(playerid, vehicleid, componentid);
#endif

stock SetPlayerWeaponDamage(playerid, weaponid, Float:damage)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	    return 0;

	if (weaponid < 0 || weaponid >= sizeof(WEAPON_DAMAGE))
	    return 0;

    fPlayerWeaponDamage[playerid][weaponid] = damage;
	return 1;
}

stock Float:GetPlayerWeaponDamage(playerid, weaponid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	    return 0;

	if (weaponid < 0 || weaponid >= sizeof(WEAPON_DAMAGE))
	    return 0;

    return fPlayerWeaponDamage[playerid][weaponid];
}
