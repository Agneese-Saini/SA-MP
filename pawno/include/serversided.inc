// Serversided.inc - by Gammix - v2.0.0 - last updated: 22 Jan, 2017
#if defined serversided_included
	#endinput
#endif
#define serversided_included

#tryinclude <streamer>

#define WEAPON_UNARMED (0)
#define WEAPON_VEHICLE_M4 (19)
#define WEAPON_VEHICLE_MINIGUN (20)
#define WEAPON_PISTOLWHIP (48)
#define WEAPON_HELIBLADES (50)
#define WEAPON_EXPLOSION (51)
#define WEAPON_CARPARK (52)
#define WEAPON_UNKNOWN (55)

#if defined OnPlayerUseVendingMachine
	forward OnPlayerUseVendingMachine(playerid, Float:oldhealth, Float:newhealth);
#endif

#if defined OnPlayerDie
	forward OnPlayerDie(playerid, killerid, reason);
#endif

#if !defined FAKE_HEALTH
	#define FAKE_HEALTH \
		1000.0
#endif

#define DISTANCE(%1,%2,%3,%4,%5,%6) \
    floatadd(floatadd(floatsqroot(floatpower(floatsub(%1, %4), 2)), floatsqroot(floatpower(floatsub(%2, %5), 2))), floatsqroot(floatpower(floatsub(%3, %6), 2)))

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

static bool:IS_GAMEMODE;

enum SS_e_VENDING_MACHINE
{
			SS_e_VENDING_MACHINE_MODEL,
			SS_e_VENDING_MACHINE_INTERIOR,
	Float:	SS_e_VENDING_MACHINE_X,
	Float:	SS_e_VENDING_MACHINE_Y,
	Float:	SS_e_VENDING_MACHINE_Z,
	Float:	SS_e_VENDING_MACHINE_RX,
	Float:	SS_e_VENDING_MACHINE_RY,
	Float:	SS_e_VENDING_MACHINE_RZ,
	Float:	SS_e_VENDING_MACHINE_FX,
	Float:	SS_e_VENDING_MACHINE_FY,
	        SS_e_VENDING_MACHINE_TIMER
};
static SS_eVendingMachine[][SS_e_VENDING_MACHINE] =
{
	{955, 	0, 		-862.82, 1536.60, 21.98, 	0.00, 0.00, 180.00, 	-862.84, 1537.60},
	{956, 	0, 		2271.72, -76.46, 25.96, 	0.00, 0.00, 0.00, 		2271.72, -77.46},
	{955, 	0, 		1277.83, 372.51, 18.95, 	0.00, 0.00, 64.00, 		1278.73, 372.07},
	{956, 	0, 		662.42, -552.16, 15.71, 	0.00, 0.00, 180.00, 	662.41, -551.16},
	{955, 	0, 		201.01, -107.61, 0.89, 		0.00, 0.00, 270.00, 	200.01, -107.63},
	{955, 	0, 		-253.74, 2597.95, 62.24,	0.00, 0.00, 90.00, 		-252.74, 2597.95},
	{956, 	0, 		-253.74, 2599.75, 62.24, 	0.00, 0.00, 90.00, 		-252.74, 2599.75},
	{956, 	0, 		-76.03, 1227.99, 19.12, 	0.00, 0.00, 90.00, 		-75.03, 1227.99},
	{955, 	0, 		-14.70, 1175.35, 18.95, 	0.00, 0.00, 180.00, 	-14.72, 1176.35},
	{1977, 	7, 		316.87, -140.35, 998.58, 	0.00, 0.00, 270.00, 	315.87, -140.36},
	{1775, 	17, 	373.82, -178.14, 1000.73, 	0.00, 0.00, 0.00, 		373.82, -179.14},
	{1776, 	17, 	379.03, -178.88, 1000.73, 	0.00, 0.00, 270.00, 	378.03, -178.90},
	{1775, 	17, 	495.96, -24.32, 1000.73, 	0.00, 0.00, 180.00, 	495.95, -23.32},
	{1776, 	17, 	500.56, -1.36, 1000.73, 	0.00, 0.00, 0.00, 		500.56, -2.36},
	{1775, 	17, 	501.82, -1.42, 1000.73, 	0.00, 0.00, 0.00, 		501.82, -2.42},
	{956, 	0, 		-1455.11, 2591.66, 55.23, 	0.00, 0.00, 180.00, 	-1455.13, 2592.66},
	{955, 	0, 		2352.17, -1357.15, 23.77, 	0.00, 0.00, 90.00, 		2353.17, -1357.15},
	{955, 	0, 		2325.97, -1645.13, 14.21, 	0.00, 0.00, 0.00, 		2325.97, -1646.13},
	{956, 	0, 		2139.51, -1161.48, 23.35, 	0.00, 0.00, 87.00, 		2140.51, -1161.53},
	{956, 	0, 		2153.23, -1016.14, 62.23, 	0.00, 0.00, 127.00, 	2154.03, -1015.54},
	{955, 	0, 		1928.73, -1772.44, 12.94, 	0.00, 0.00, 90.00, 		1929.73, -1772.44},
	{1776, 	1, 		2222.36, 1602.64, 1000.06, 	0.00, 0.00, 90.00, 		2223.36, 1602.64},
	{1775, 	1, 		2222.20, 1606.77, 1000.05, 	0.00, 0.00, 90.00, 		2223.20, 1606.77},
	{1775, 	1, 		2155.90, 1606.77, 1000.05, 	0.00, 0.00, 90.00, 		2156.90, 1606.77},
	{1775, 	1, 		2209.90, 1607.19, 1000.05, 	0.00, 0.00, 270.00, 	2208.90, 1607.17},
	{1776, 	1, 		2155.84, 1607.87, 1000.06, 	0.00, 0.00, 90.00, 		2156.84, 1607.87},
	{1776, 	1, 		2202.45, 1617.00, 1000.06,	0.00, 0.00, 180.00, 	2202.43, 1618.00},
	{1776, 	1, 		2209.24, 1621.21, 1000.06, 	0.00, 0.00, 0.00, 		2209.24, 1620.21},
	{1776, 	3, 		330.67, 178.50, 1020.07, 	0.00, 0.00, 0.00,	 	330.67, 177.50},
	{1776, 	3, 		331.92, 178.50, 1020.07, 	0.00, 0.00, 0.00, 		331.92, 177.50},
	{1776, 	3, 		350.90, 206.08, 1008.47, 	0.00, 0.00, 90.00, 		351.90, 206.08},
	{1776, 	3, 		361.56, 158.61, 1008.47, 	0.00, 0.00, 180.00, 	361.54, 159.61},
	{1776, 	3, 		371.59, 178.45, 1020.07, 	0.00, 0.00, 0.00, 		371.59, 177.45},
	{1776,	3, 		374.89, 188.97, 1008.47, 	0.00, 0.00, 0.00, 		374.89, 187.97},
	{1775,	2, 		2576.70, -1284.43, 1061.09, 0.00, 0.00, 270.00, 	2575.70, -1284.44},
	{1775, 	15, 	2225.20, -1153.42, 1025.90, 0.00, 0.00, 270.00, 	2224.20, -1153.43},
	{955, 	0, 		1154.72, -1460.89, 15.15, 	0.00, 0.00, 270.00, 	1153.72, -1460.90},
	{956, 	0, 		2480.85, -1959.27, 12.96, 	0.00, 0.00, 180.00, 	2480.84, -1958.27},
	{955, 	0, 		2060.11, -1897.64, 12.92, 	0.00, 0.00, 0.00, 		2060.11, -1898.64},
	{955, 	0, 		1729.78, -1943.04, 12.94, 	0.00, 0.00, 0.00, 		1729.78, -1944.04},
	{956, 	0, 		1634.10, -2237.53, 12.89, 	0.00, 0.00, 0.00, 		1634.10, -2238.53},
	{955, 	0, 		1789.21, -1369.26, 15.16, 	0.00, 0.00, 270.00, 	1788.21, -1369.28},
	{956, 	0, 		-2229.18, 286.41, 34.70, 	0.00, 0.00, 180.00, 	-2229.20, 287.41},
	{955,	256, 	-1980.78, 142.66, 27.07, 	0.00, 0.00, 270.00, 	-1981.78, 142.64},
	{955, 	256, 	-2118.96, -423.64, 34.72, 	0.00, 0.00, 255.00, 	-2119.93, -423.40},
	{955, 	256, 	-2118.61, -422.41, 34.72, 	0.00, 0.00, 255.00, 	-2119.58, -422.17},
	{955, 	256, 	-2097.27, -398.33, 34.72, 	0.00, 0.00, 180.00, 	-2097.29, -397.33},
	{955, 	256, 	-2092.08, -490.05, 34.72, 	0.00, 0.00, 0.00, 		-2092.08, -491.05},
	{955, 	256, 	-2063.27, -490.05, 34.72, 	0.00, 0.00, 0.00, 		-2063.27, -491.05},
	{955, 	256, 	-2005.64, -490.05, 34.72, 	0.00, 0.00, 0.00, 		-2005.64, -491.05},
	{955, 	256, 	-2034.46, -490.05, 34.72, 	0.00, 0.00, 0.00, 		-2034.46, -491.05},
	{955, 	256, 	-2068.56, -398.33, 34.72, 	0.00, 0.00, 180.00, 	-2068.58, -397.33},
	{955, 	256,	-2039.85, -398.33, 34.72, 	0.00, 0.00, 180.00, 	-2039.86, -397.33},
	{955, 	256, 	-2011.14, -398.33, 34.72, 	0.00, 0.00, 180.00, 	-2011.15, -397.33},
	{955, 	2048, 	-1350.11, 492.28, 10.58, 	0.00, 0.00, 90.00, 		-1349.11, 492.28},
	{956, 	2048, 	-1350.11, 493.85, 10.58, 	0.00, 0.00, 90.00,	 	-1349.11, 493.85},
	{955, 	0, 		2319.99, 2532.85, 10.21, 	0.00, 0.00, 0.00, 		2319.99, 2531.85},
	{956, 	0,		2845.72, 1295.04, 10.78, 	0.00, 0.00, 0.00, 		2845.72, 1294.04},
	{955, 	0, 		2503.14, 1243.69, 10.21, 	0.00, 0.00, 180.00, 	2503.12, 1244.69},
	{956, 	0, 		2647.69, 1129.66, 10.21, 	0.00, 0.00, 0.00, 		2647.69, 1128.66},
	{1209, 	0, 		-2420.21, 984.57, 44.29, 	0.00, 0.00, 90.00, 		-2419.21, 984.57},
	{1302, 	0, 		-2420.17, 985.94, 44.29, 	0.00, 0.00, 90.00, 		-2419.17, 985.94},
	{955, 	0,	 	2085.77, 2071.35, 10.45, 	0.00, 0.00, 90.00, 		2086.77, 2071.35},
	{956, 	0, 		1398.84, 2222.60, 10.42, 	0.00, 0.00, 180.00, 	1398.82, 2223.60},
	{956, 	0, 		1659.46, 1722.85, 10.21, 	0.00, 0.00, 0.00, 		1659.46, 1721.85},
	{955, 	0, 		1520.14, 1055.26, 10.00, 	0.00, 0.00, 270.00, 	1519.14, 1055.24},
	{1775, 	6,		-19.03, -57.83, 1003.63,	0.00, 0.00, 180.00, 	-19.05, -56.83},
	{1775, 	18, 	-16.11, -91.64, 1003.63, 	0.00, 0.00, 180.00, 	-16.13, -90.64},
	{1775, 	16, 	-15.10, -140.22, 1003.63, 	0.00, 0.00, 180.00, 	-15.11, -139.22},
	{1775, 	17, 	-32.44, -186.69, 1003.63, 	0.00, 0.00, 180.00, 	-32.46, -185.69},
	{1775, 	16, 	-35.72, -140.22, 1003.63, 	0.00, 0.00, 180.00, 	-35.74, -139.22},
	{1776, 	6, 		-36.14, -57.87, 1003.63, 	0.00, 0.00, 180.00, 	-36.16, -56.87},
	{1776, 	18, 	-17.54, -91.71, 1003.63, 	0.00, 0.00, 180.00, 	-17.56, -90.71},
	{1776, 	16, 	-16.53, -140.29, 1003.63, 	0.00, 0.00, 180.00, 	-16.54, -139.29},
	{1776, 	17, 	-33.87, -186.76, 1003.63, 	0.00, 0.00, 180.00, 	-33.89, -185.76},
	{1775, 	6, 		-19.03, -57.83, 1003.63, 	0.00, 0.00, 180.00, 	-19.05, -56.83},
	{1776, 	6, 		-36.14, -57.87, 1003.63, 	0.00, 0.00, 180.00, 	-36.16, -56.87},
	{1775, 	18, 	-16.11, -91.64, 1003.63, 	0.00, 0.00, 180.00, 	-16.13, -90.64},
	{1776, 	18, 	-17.54, -91.71, 1003.63, 	0.00, 0.00, 180.00, 	-17.56, -90.71},
	{1776, 	16, 	-16.53, -140.29, 1003.63, 	0.00, 0.00, 180.00,	 	-16.54, -139.29},
	{1775, 	16, 	-15.10, -140.22, 1003.63, 	0.00, 0.00, 180.00, 	-15.11, -139.22},
	{1776, 	17, 	-33.87, -186.76, 1003.63, 	0.00, 0.00, 180.00, 	-33.89, -185.76},
	{1775, 	17, 	-32.44, -186.69, 1003.63, 	0.00, 0.00, 180.00, 	-32.46, -185.69},
	{1775, 	16, 	-35.72, -140.22, 1003.63, 	0.00, 0.00, 180.00, 	-35.74, -139.22}
};
static SS_iPlayerVendingMachineObject[MAX_PLAYERS][sizeof(SS_eVendingMachine)];
static Float:SS_fPlayerVendingMachineAmount[MAX_PLAYERS][sizeof(SS_eVendingMachine)];

enum SS_e_WEAPON
{
			SS_e_WEAPON_NAME[35],
	Float:	SS_e_WEAPON_RANGE,
	Float:	SS_e_WEAPON_DAMAGE,
			SS_e_WEAPON_RATE_OF_FIRE
};
static SS_eWeapon[][SS_e_WEAPON] =
{
	{"Fist",			15.0,   1.0,	250},
	{"Brass knuckles",	15.0,   1.0,	250},
	{"Golf club",		15.0,   1.0,	250},
	{"Nitestick",		15.0,   1.0,	250},
	{"Knife",			15.0,   1.0,	250},
	{"Bat",				15.0,   1.0,	250},
	{"Shovel",			15.0,   1.0,	250},
	{"Pool cue",		15.0,   1.0,	250},
	{"Katana",			15.0,   1.0,	250},
	{"Chainsaw",		15.0,   1.0,	30},
	{"Dildo",			15.0,   1.0,	250},
	{"Pink dildo",		15.0,   1.0,	250},
	{"Vibrator",		15.0,   1.0,	250},
	{"Silver vibrator",	15.0,   1.0,	250},
	{"Flowers",			15.0,   1.0,	250},
	{"Cane",			15.0,   1.0,	250},
	{"Grenade",			0.0,   	82.5,	0},
	{"Teargas",			0.0,   	0.0,	0},
	{"Molotov",			0.0,   	1.0,	0},
	{"Vehicle M4",		90.0,   9.9,	20},
	{"Vehicle minigun",	75.0,   46.2,	20},
	{"",				0.0,   	0.0,	0},
	{"Colt 45",			35.0,   8.25,	160},
	{"Silenced",		35.0,   13.2,	120},
	{"Desert eagle",	35.0,   46.2,	120},
	{"Shotgun",			40.0,   3.3,	800},
	{"Sawnoff shotgun",	35.0,   3.3,	120},
	{"Spas12 shotgun",	40.0,   4.95,	120},
	{"UZI",				35.0,   6.6,	50},
	{"MP5",				45.0,   8.25,	90},
	{"AK-47",			70.0,   9.9,	90},
	{"M4",				90.0,   9.9,	90},
	{"Tec9",			35.0,   6.6,	70},
	{"Country rifle",	100.0,  24.75,	800},
	{"Sniper rifle",	320.0,  41.25,	900},
	{"Rocket launcher",	0.0,   	82.5,	0},
	{"Heatseeker launcher",0.0,	82.5,	0},
	{"Flamethrower",	0.0,   	1.0,	0},
	{"Minigun",			75.0,   46.2,	20},
	{"Satchel",			0.0,   	0.0,	0},
	{"Detonator",		0.0,   	0.0,	0},
	{"Spraycan",		15.0,   1.0,	10},
	{"Fire extinguisher",15.0,  1.0,	10},
	{"Camera",			0.0,   	0.0,	0},
	{"Night vision",	0.0,   	0.0,	0},
	{"Infrared",		0.0,   	0.0,	0},
	{"Parachute",		0.0,   	0.0,	0},
	{"Fake pistol",		0.0,   	0.0,	0},
	{"Pistol whip",		15.0,   2.64,	400},
	{"Vehicle",			0.0,   	9.9,	0},
	{"Helicopter blades",0.0,   330.0,	0},
	{"Explosion",		0.0,   	82.5,	0},
	{"Car park",		0.0,   	1.0,	0},
	{"Drowning",		0.0,   	1.0,	0},
	{"Splat",			0.0,   	165.0,	0}
};

static Float:SS_fPlayerWeaponDamage[MAX_PLAYERS][sizeof(SS_eWeapon)];

static SS_iPlayerTeam[MAX_PLAYERS];
static SS_iPlayerClass[MAX_PLAYERS];

static SS_iPlayerMoney[MAX_PLAYERS];

static Float:SS_fPlayerHealth[MAX_PLAYERS];
static Float:SS_fPlayerArmor[MAX_PLAYERS];

static SS_iPlayerWeaponData[MAX_PLAYERS][14][2];

enum SS_e_SPAWN_DATA
{
    SS_e_SPAWN_DATA_TEAM,
    SS_e_SPAWN_DATA_WEAPON1[2],
    SS_e_SPAWN_DATA_WEAPON2[2],
    SS_e_SPAWN_DATA_WEAPON3[2]
};
static SS_ePlayerSpawnData[MAX_PLAYERS][SS_e_SPAWN_DATA];
static SS_eClassSpawnData[MAX_CLASSES][SS_e_SPAWN_DATA];

static SS_iStaticPickup[MAX_PICKUPS];

static const SS_COMPONENT_PRICE[] =
{
	400, 550, 200, 250, 100, 150, 80, 500, 200, 500,
 	1000, 220, 250, 100, 400, 500, 200, 500, 350, 300,
    250, 200, 150, 350, 50, 1000, 480, 480, 770, 680,
    37, 370, 170, 120, 790, 150, 500, 690, 190, 390,
    500, 390, 1000, 500, 500, 510, 710, 670, 530, 810,
    620, 670, 530, 130, 210, 230, 520, 430, 620, 720,
    530, 180, 250, 430, 830, 850, 750, 250, 200, 550,
    450, 550, 450, 1000, 1030, 980, 1560, 1620, 1200, 1030,
    1000, 1230, 820, 1560, 1350, 770, 100, 1500, 150, 650,
    450, 100, 750, 350, 450, 350, 1000, 620, 1140, 1000,
    940, 780, 830, 3250, 1610, 1540, 780, 780, 780, 1610,
    1540, -1, -1, 3340, 3250, 2130, 2050, 2130, 780, 940,
    780, 940, 780, 860, 780, 1120, 3340, 3250, 3340, 1650,
    3380, 3290, 1590, 830, 800, 1500, 1000, 800, 580, 470,
    870, 980, 150, 150, 100, 100, 490, 600, 890, 1000,
    1090, 840, 910, 1200, 1030, 1030, 920, 930, 550, 1050,
    1050, 950, 650, 550, 450, 850, 950, 850, 950, 970,
    880, 990, 900, 950, 1000, 900, 1000, 900, 2050, 2150,
    2130, 2050, 2130, 2040, 2150, 2040, 2095, 2175, 2080, 2200,
    1200, 1040, 940, 1100
};

stock static SS_GetWeaponSlot(weaponid)
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

stock static SS_ProcessDamage(&playerid, &issuerid, &Float:amount, &weaponid, &bodypart) //Credits to Slice, Modified by me
{
	if (weaponid == WEAPON_COLLISION && 1061 <= GetPlayerAnimationIndex(playerid) <= 1067)
		return 0;

   	if (_:amount == _:1833.33154296875)
		return 0;

	if (issuerid != INVALID_PLAYER_ID)
    {
        switch (weaponid)
        {
            case WEAPON_COLT45..WEAPON_SNIPER,
				 WEAPON_SPRAYCAN,
				 WEAPON_FIREEXTINGUISHER:
            {
				if (_:amount == _:2.6400001049041748046875)
				{
					bodypart = 3;
					weaponid = WEAPON_PISTOLWHIP;
				}
			}

			case WEAPON_HELIBLADES:
			{
			    if (_:amount != _:330.0)
			    {
			        weaponid = WEAPON_CARPARK;
			    }
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
						case WEAPON_UZI, WEAPON_TEC9, WEAPON_CHAINSAW, WEAPON_SHOTGUN, WEAPON_SAWEDOFF:
					 	{
					 	}
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
			}

			default:
			{
			    if ((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP)
				{
				    amount = 1.32000005245208740234375;
				}
			}
		}
	}

	new Float:distance;
	if (issuerid != INVALID_PLAYER_ID)
	{
		if (SS_eWeapon[weaponid][SS_e_WEAPON_RANGE] > 0.0)
		{
			new Float:x, Float:y, Float:z;
			GetPlayerPos(issuerid, x, y, z);

			distance = GetPlayerDistanceFromPoint(playerid, x, y, z);
			if (distance > SS_eWeapon[weaponid][SS_e_WEAPON_RANGE] + 2.0)
		  		return 0;
		}
	}

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
				case WEAPON_VEHICLE, WEAPON_VEHICLE_M4, WEAPON_AK47, WEAPON_M4, WEAPON_SHOTGUN, WEAPON_SAWEDOFF, WEAPON_SHOTGSPA:
				{
				}
				default:
				{
					amount = 6.6000003814697265625;
				}
			}
		}
	}

	switch (weaponid)
	{
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
					amount *= SS_fPlayerWeaponDamage[issuerid][WEAPON_CHAINSAW];
				}

				default:
				{
					amount = SS_fPlayerWeaponDamage[issuerid][WEAPON_CHAINSAW];
				}
			}
	    }

	    case WEAPON_COLLISION:
	    {
			if (amount > 165.0)
				amount = 1.0;
			else
				amount /= 165.0;

	        amount *= SS_fPlayerWeaponDamage[issuerid][weaponid];
	    }

	    case WEAPON_DROWN:
	    {
	    	if (amount > 10.0)
				amount = 10.0;

	        amount *= SS_fPlayerWeaponDamage[issuerid][weaponid];
	    }

		case WEAPON_GRENADE,
			 WEAPON_ROCKETLAUNCHER,
			 WEAPON_HEATSEEKER,
			 WEAPON_SATCHEL,
			 WEAPON_EXPLOSION:
		{
			amount = (amount / 82.5) * SS_fPlayerWeaponDamage[issuerid][weaponid];
		}

		case WEAPON_MOLTOV,
			 WEAPON_COLT45..WEAPON_DEAGLE,
			 WEAPON_UZI..WEAPON_SNIPER,
			 WEAPON_MINIGUN,
			 WEAPON_FLAMETHROWER,
			 WEAPON_CARPARK,
			 WEAPON_SPRAYCAN,
			 WEAPON_FIREEXTINGUISHER,
			 WEAPON_PISTOLWHIP,
			 WEAPON_VEHICLE,
			 WEAPON_HELIBLADES:
		{
		    amount = SS_fPlayerWeaponDamage[issuerid][weaponid];
		}

		case WEAPON_SHOTGSPA:
	 	{
	 	    new Float:bullets = amount / 4.950000286102294921875;
			if (8.0 - bullets < -0.05)
   	 			return 0;

			new Float:f = floatfract(bullets);
			if (f > 0.01 && f < 0.99)
				return 0;

	 	    amount = SS_fPlayerWeaponDamage[issuerid][weaponid] * bullets;
		}

		case WEAPON_SHOTGUN,
			 WEAPON_SAWEDOFF:
	 	{
	 	    new Float:bullets = amount / 3.30000019073486328125;
			if (15.0 - bullets < -0.05)
				return 0;

			new Float:f = floatfract(bullets);
			if (f > 0.01 && f < 0.99)
				return 0;

	 	    amount = SS_fPlayerWeaponDamage[issuerid][weaponid] * bullets;
		}
	}

	return 1;
}

stock static SS_HasSameTeam(playerid, otherid)
{
	if (SS_iPlayerTeam[playerid] == NO_TEAM || SS_iPlayerTeam[otherid] == NO_TEAM)
		return 0;

	return (SS_iPlayerTeam[playerid] == SS_iPlayerTeam[otherid]);
}

stock static SS_InflictDamage(playerid, Float:amount, bool:only_health)
{
	if (only_health)
	{
	    if (amount >= SS_fPlayerHealth[playerid])
	    	SS_fPlayerHealth[playerid] = 0.0;
		else
	    	SS_fPlayerHealth[playerid] -= amount;
	}
	else
	{
		SS_fPlayerArmor[playerid] -= amount;
	    if (SS_fPlayerArmor[playerid] < 0.0)
			SS_fPlayerHealth[playerid] += SS_fPlayerArmor[playerid];

	    if (SS_fPlayerArmor[playerid] <= 0.0)
	     	SS_fPlayerArmor[playerid] = 0.0;
	    if (SS_fPlayerHealth[playerid] <= 0.0)
	     	SS_fPlayerHealth[playerid] = 0.0;
	}

	if (SS_fPlayerHealth[playerid] >= 100.0)
		SetPlayerHealth(playerid, FAKE_HEALTH);
	else
		SetPlayerHealth(playerid, SS_fPlayerHealth[playerid]);

	if (SS_fPlayerArmor[playerid] >= 100.0)
		SetPlayerArmour(playerid, FAKE_HEALTH);
	else
		SetPlayerArmour(playerid, SS_fPlayerArmor[playerid]);
}

stock static SS_GetModelWeaponID(weaponid)
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

public OnGameModeInit()
{
    IS_GAMEMODE = true;

	DisableInteriorEnterExits();
	EnableStuntBonusForAll(0);
	EnableVehicleFriendlyFire();

	for (new i; i < MAX_PLAYERS; i++)
	{
	  	for (new x, y = sizeof(SS_eWeapon); x < y; x++)
	   		ResetPlayerWeaponDamage(i, x);
	}

	#if defined SS_OnGameModeInit
		return SS_OnGameModeInit();
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit SS_OnGameModeInit
#if defined SS_OnGameModeInit
	forward SS_OnGameModeInit();
#endif

public OnPlayerConnect(playerid)
{
	SS_iPlayerTeam[playerid] = NO_TEAM;
	SS_iPlayerClass[playerid] = 0;

	SS_fPlayerHealth[playerid] = 100.0;
	SS_fPlayerArmor[playerid] = 0.0;

	for (new i; i < 14; i++)
	{
		SS_iPlayerWeaponData[playerid][i][0] = 0;
		SS_iPlayerWeaponData[playerid][i][1] = 0;
	}

	SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_TEAM] = NO_TEAM;
	SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON1][0] = 0;
	SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON1][1] = 0;
	SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON2][0] = 0;
	SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON2][1] = 0;
	SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON3][0] = 0;
	SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON3][1] = 0;

	if (IS_GAMEMODE)
	{
	    RemoveBuildingForPlayer(playerid, 955, 0.0, 0.0, 0.0, 20000.0);
		RemoveBuildingForPlayer(playerid, 956, 0.0, 0.0, 0.0, 20000.0);
		RemoveBuildingForPlayer(playerid, 1209, 0.0, 0.0, 0.0, 20000.0);
		RemoveBuildingForPlayer(playerid, 1302, 0.0, 0.0, 0.0, 20000.0);
		RemoveBuildingForPlayer(playerid, 1775, 0.0, 0.0, 0.0, 20000.0);
		RemoveBuildingForPlayer(playerid, 1776, 0.0, 0.0, 0.0, 20000.0);
		RemoveBuildingForPlayer(playerid, 1977, 0.0, 0.0, 0.0, 20000.0);
		for (new i, j = sizeof(SS_eVendingMachine); i < j; i++)
		{
			RemoveBuildingForPlayer(playerid, SS_eVendingMachine[i][SS_e_VENDING_MACHINE_MODEL], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_X], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_Y], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_Z], 1.0);

	        #if defined STREAMER_TYPE_OBJECT
				SS_iPlayerVendingMachineObject[playerid][i] = CreateDynamicObject(SS_eVendingMachine[i][SS_e_VENDING_MACHINE_MODEL], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_X], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_Y], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_Z], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_RX], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_RY], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_RZ], .interiorid = SS_eVendingMachine[i][SS_e_VENDING_MACHINE_INTERIOR], .playerid = playerid);
			#else
			 	SS_iPlayerVendingMachineObject[playerid][i] = CreatePlayerObject(playerid, SS_eVendingMachine[i][SS_e_VENDING_MACHINE_MODEL], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_X], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_Y], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_Z], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_RX], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_RY], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_RZ]);
			#endif
		}
	}

	#if defined SS_OnPlayerConnect
		return SS_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect SS_OnPlayerConnect
#if defined SS_OnPlayerConnect
	forward SS_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
    if (IS_GAMEMODE)
    {
	    for (new i, j = sizeof(SS_eVendingMachine); i < j; i++)
		{
		    #if defined STREAMER_TYPE_OBJECT
				DestroyDynamicObject(SS_iPlayerVendingMachineObject[playerid][i]);
			#else
			 	DestroyPlayerObject(playerid, SS_iPlayerVendingMachineObject[playerid][i]);
		 	#endif
		}
	}
	
    #if defined SS_OnPlayerDisconnect
       	return SS_OnPlayerDisconnect(playerid, reason);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect SS_OnPlayerDisconnect
#if defined SS_OnPlayerDisconnect
    forward SS_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerRequestClass(playerid, classid)
{
	SS_iPlayerClass[playerid] = classid;

	#if defined SS_OnPlayerRequestClass
		return SS_OnPlayerRequestClass(playerid, classid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerRequestClass
    #undef OnPlayerRequestClass
#else
    #define _ALS_OnPlayerRequestClass
#endif
#define OnPlayerRequestClass SS_OnPlayerRequestClass
#if defined SS_OnPlayerRequestClass
	forward SS_OnPlayerRequestClass(playerid, classid);
#endif

public OnPlayerRequestSpawn(playerid)
{
	new bool:ret = true;
    #if defined SS_OnPlayerRequestSpawn
		ret = bool:SS_OnPlayerRequestSpawn(playerid);
	#endif

    if (ret)
	{
	 	if (IS_GAMEMODE)
		 	ResetPlayerWeapons(playerid);
		 	
		for (new i; i < 14; i++)
		{
		    SS_iPlayerWeaponData[playerid][i][0] = 0;
		    SS_iPlayerWeaponData[playerid][i][1] = 0;
		}

		new slot = SS_GetWeaponSlot(SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON1][0]);
		if (slot != -1)
		{
			SS_iPlayerWeaponData[playerid][slot][0] = SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON1][0];
			SS_iPlayerWeaponData[playerid][slot][1] = SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON2][0];

			if (IS_GAMEMODE)
				GivePlayerWeapon(playerid, SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON1][0], SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON1][1]);
		}

		slot = SS_GetWeaponSlot(SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON2][0]);
		if (slot != -1)
		{
			SS_iPlayerWeaponData[playerid][slot][0] = SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON2][0];
			SS_iPlayerWeaponData[playerid][slot][1] = SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON2][0];

			if (IS_GAMEMODE)
				GivePlayerWeapon(playerid, SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON2][0], SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON2][1]);
		}

		slot = SS_GetWeaponSlot(SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON3][0]);
		if (slot != -1)
		{
			SS_iPlayerWeaponData[playerid][slot][0] = SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON3][0];
			SS_iPlayerWeaponData[playerid][slot][1] = SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON3][0];

			if (IS_GAMEMODE)
				GivePlayerWeapon(playerid, SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON3][0], SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_WEAPON3][1]);
		}

		if (SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_TEAM] != NO_TEAM + 1)
			SS_iPlayerTeam[playerid] = SS_eClassSpawnData[SS_iPlayerClass[playerid]][SS_e_SPAWN_DATA_TEAM];
	}

	return ret;
}
#if defined _ALS_OnPlayerRequestSpawn
    #undef OnPlayerRequestSpawn
#else
    #define _ALS_OnPlayerRequestSpawn
#endif
#define OnPlayerRequestSpawn SS_OnPlayerRequestSpawn
#if defined SS_OnPlayerRequestSpawn
	forward SS_OnPlayerRequestSpawn(playerid);
#endif

public OnPlayerSpawn(playerid)
{
    SS_fPlayerHealth[playerid] = 100.0;
    SetPlayerHealth(playerid, FAKE_HEALTH);

	SS_fPlayerArmor[playerid] = 0.0;
    SetPlayerArmour(playerid, 0.0);

	if (IS_GAMEMODE)
		ResetPlayerWeapons(playerid);
		
	for (new i; i < 14; i++)
	{
	    SS_iPlayerWeaponData[playerid][i][0] = 0;
	    SS_iPlayerWeaponData[playerid][i][1] = 0;
	}

	new slot = SS_GetWeaponSlot(SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON1][0]);
	if (slot != -1)
	{
		SS_iPlayerWeaponData[playerid][slot][0] = SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON1][0];
		SS_iPlayerWeaponData[playerid][slot][1] = SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON2][0];

		if (IS_GAMEMODE)
			GivePlayerWeapon(playerid, SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON1][0], SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON1][1]);
	}

	slot = SS_GetWeaponSlot(SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON2][0]);
	if (slot != -1)
	{
		SS_iPlayerWeaponData[playerid][slot][0] = SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON2][0];
		SS_iPlayerWeaponData[playerid][slot][1] = SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON2][0];

		if (IS_GAMEMODE)
			GivePlayerWeapon(playerid, SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON2][0], SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON2][1]);
	}

	slot = SS_GetWeaponSlot(SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON3][0]);
	if (slot != -1)
	{
		SS_iPlayerWeaponData[playerid][slot][0] = SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON3][0];
		SS_iPlayerWeaponData[playerid][slot][1] = SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON3][0];

		if (IS_GAMEMODE)
			GivePlayerWeapon(playerid, SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON3][0], SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON3][1]);
	}

	if (SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_TEAM] != NO_TEAM + 1)
		SS_iPlayerTeam[playerid] = SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_TEAM];

	if (IS_GAMEMODE)
		SetPlayerTeam(playerid, 0);

	#if defined SS_OnPlayerSpawn
		return SS_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn SS_OnPlayerSpawn
#if defined SS_OnPlayerSpawn
	forward SS_OnPlayerSpawn(playerid);
#endif

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (newkeys & KEY_FIRE)
 	{
		if (GetPlayerWeapon(playerid) == WEAPON_SATCHEL)
		{
	        new slot = SS_GetWeaponSlot(WEAPON_BOMB);
			if (slot != -1)
			{
				SS_iPlayerWeaponData[playerid][slot][0] = WEAPON_BOMB;
				SS_iPlayerWeaponData[playerid][slot][1] = 1;
		    }
		    
	        if (IS_GAMEMODE)
				GivePlayerWeapon(playerid, WEAPON_BOMB, 1);
		}
	}

	if (IS_GAMEMODE)
	{
	    switch (GetPlayerState(playerid))
		{
			case PLAYER_STATE_ONFOOT:
			{
			    if (PRESSED(KEY_SECONDARY_ATTACK))
			    {
					if (GetPlayerAnimationIndex(playerid) != 1660)
					{
			 			new interiorid = GetPlayerInterior(playerid);

						new Float:z;
	                   	GetPlayerPos(playerid, z, z, z);

						for (new i, j = sizeof(SS_eVendingMachine); i < j; i++)
						{
						    #if defined STREAMER_TYPE_OBJECT
							    if (IsValidDynamicObject(SS_iPlayerVendingMachineObject[playerid][i]))
							    	continue;
							#else
							    if (IsValidPlayerObject(playerid, SS_iPlayerVendingMachineObject[playerid][i]))
							    	continue;
							#endif

							if (SS_eVendingMachine[i][SS_e_VENDING_MACHINE_TIMER] != 0)
							    continue;

							if (interiorid != SS_eVendingMachine[i][SS_e_VENDING_MACHINE_INTERIOR])
			    				continue;

							if (!IsPlayerInRangeOfPoint(playerid, 0.5, SS_eVendingMachine[i][SS_e_VENDING_MACHINE_FX], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_FY], z))
								continue;

							if (floatabs(z - SS_eVendingMachine[i][SS_e_VENDING_MACHINE_Z]) > 1.5)
								continue;

						    if (SS_iPlayerMoney[playerid] <= 0)
						    {
						        PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
						        break;
						    }

							SetPlayerPos(playerid, SS_eVendingMachine[i][SS_e_VENDING_MACHINE_FX], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_FY], z);
	                    	SetPlayerFacingAngle(playerid, SS_eVendingMachine[i][SS_e_VENDING_MACHINE_Z]);

	                        ApplyAnimation(playerid, "VENDING", "VEND_USE", 4.1, 0, 0, 1, 0, 0, 1);

	                        PlayerPlaySound(playerid, 42600, 0.0, 0.0, 0.0);

							SS_eVendingMachine[i][SS_e_VENDING_MACHINE_TIMER] = SetTimerEx("__OnPlayerUseVendingMachine", 2500, false, "ii", playerid, i);
	   						SS_iPlayerMoney[playerid]--;

							break;
						}
					}
				}
			}
		}
	}

	#if defined SS_OnPlayerKeyStateChange
		return SS_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange SS_OnPlayerKeyStateChange
#if defined SS_OnPlayerKeyStateChange
	forward SS_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

forward __OnPlayerUseVendingMachine(playerid, id);
public  __OnPlayerUseVendingMachine(playerid, id)
{
	#if defined OnPlayerUseVendingMachine
		new Float:oldhealth = SS_fPlayerHealth[playerid];
	#endif
	new Float:newhealth = SS_fPlayerHealth[playerid];
	
	if (SS_fPlayerHealth[playerid] < 100.0)
	{
		newhealth = (100.0 - SS_fPlayerHealth[playerid] >= SS_fPlayerVendingMachineAmount[playerid][id]) ? (SS_fPlayerHealth[playerid] + SS_fPlayerVendingMachineAmount[playerid][id]) : (SS_fPlayerHealth[playerid] + (100.0 - SS_fPlayerHealth[playerid]));
		SetPlayerHealth(playerid, newhealth);
	}

	#if defined OnPlayerUseVendingMachine
    	OnPlayerUseVendingMachine(playerid, oldhealth, newhealth);
    #endif
    
	SS_eVendingMachine[id][SS_e_VENDING_MACHINE_TIMER] = 0;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if (!((WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) || weaponid == WEAPON_MINIGUN))
		return 0;

	new slot = SS_GetWeaponSlot(weaponid);
	if (SS_iPlayerWeaponData[playerid][slot][1] <= 0)
	    return 0;

	SS_iPlayerWeaponData[playerid][slot][1]--;
	if (!SS_iPlayerWeaponData[playerid][slot][1])
	{
		 SS_iPlayerWeaponData[playerid][slot][0] = 0;
		 SS_iPlayerWeaponData[playerid][slot][1] = 0;
	}

	#if defined SS_OnPlayerWeaponShot
	    if (SS_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ) <= 0)
 			return 0;
	#endif

	new Float:origin[3], Float:hit[3];
	GetPlayerLastShotVectors(playerid, origin[0], origin[1], origin[2], hit[0], hit[1], hit[2]);

	new Float:length = VectorSize(origin[0] - hit[0], origin[1] - hit[1], origin[2] - hit[2]);
	if (length > SS_eWeapon[weaponid][SS_e_WEAPON_RANGE])
		return 0;

	if (hittype == BULLET_HIT_TYPE_PLAYER)
		return 0;

	return 1;
}
#if defined _ALS_OnPlayerWeaponShot
    #undef OnPlayerWeaponShot
#else
    #define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot SS_OnPlayerWeaponShot
#if defined SS_OnPlayerWeaponShot
	forward SS_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

#if defined OnPlayerShotVehicle
	forward OnPlayerShotVehicle(playerid, vehicleid, weaponid, Float:damage);
#endif

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	if (!(WEAPON_COLT45 <= weaponid <= WEAPON_FIREEXTINGUISHER || weaponid == WEAPON_PARACHUTE))
	   	return 0;

	if (weaponid == WEAPON_KNIFE && _:amount > _:1.0)
	{
		if (SS_HasSameTeam(playerid, damagedid))
		{
			#if defined SS_OnPlayerGiveDamage
				return SS_OnPlayerGiveDamage(playerid, damagedid, amount, weaponid, bodypart);
			#else
				return 0;
			#endif
		}

		if (IS_GAMEMODE)
		{
		    new ret = 1;
			#if defined SS_OnPlayerGiveDamage
				ret = SS_OnPlayerGiveDamage(playerid, damagedid, FLOAT_INIFINITY, weaponid, bodypart);
			#endif

			SS_fPlayerHealth[playerid] = 0.0;
			SetPlayerHealth(playerid, 0.0);
			#if defined SS_OnPlayerTakeDamage
				SS_OnPlayerTakeDamage(damagedid, playerid, amount, weaponid, bodypart);
			#endif
			
            #if defined OnPlayerDie
	  		    OnPlayerDie(damagedid, playerid, weaponid);
			#endif
			return ret;
		}
	}
	else
	{
		if (!SS_ProcessDamage(damagedid, playerid, amount, weaponid, bodypart))
			return 0;

		if (SS_HasSameTeam(playerid, damagedid))
		{
			#if defined SS_OnPlayerGiveDamage
				return SS_OnPlayerGiveDamage(playerid, damagedid, amount, weaponid, bodypart);
			#else
				return 0;
			#endif
		}
	}

	new ret = 1;
	#if defined SS_OnPlayerGiveDamage
		ret = SS_OnPlayerGiveDamage(playerid, damagedid, amount, weaponid, bodypart);
	#endif

	SS_InflictDamage(damagedid, amount, false);
	#if defined SS_OnPlayerTakeDamage
		SS_OnPlayerTakeDamage(damagedid, playerid, amount, weaponid, bodypart);
	#endif
	
	if (SS_fPlayerHealth[damagedid] <= 0.0)
 	{
  		#if defined OnPlayerDie
  		    OnPlayerDie(damagedid, playerid, weaponid);
		#endif
	}
	return ret;
}
#if defined _ALS_OnPlayerGiveDamage
    #undef OnPlayerGiveDamage
#else
    #define _ALS_OnPlayerGiveDamage
#endif
#define OnPlayerGiveDamage SS_OnPlayerGiveDamage
#if defined SS_OnPlayerGiveDamage
	forward SS_OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart);
#endif

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	if (issuerid == INVALID_PLAYER_ID)
	{
	    if (!SS_ProcessDamage(playerid, issuerid, amount, weaponid, bodypart))
			return 0;

		SS_InflictDamage(playerid, amount, ((weaponid == WEAPON_COLLISION) ? (true) : (false)));
		new ret = 1;
   		#if defined SS_OnPlayerTakeDamage
			ret = SS_OnPlayerTakeDamage(playerid, issuerid, amount, weaponid, bodypart);
		#endif

		if (SS_fPlayerHealth[playerid] <= 0.0)
		{
	 	    #if defined OnPlayerDie
		  		OnPlayerDie(playerid, INVALID_PLAYER_ID, WEAPON_COLLISION);
			#endif
		}
		return ret;
	}
	
	return 0;
}
#if defined _ALS_OnPlayerTakeDamage
    #undef OnPlayerTakeDamage
#else
    #define _ALS_OnPlayerTakeDamage
#endif
#define OnPlayerTakeDamage SS_OnPlayerTakeDamage
#if defined SS_OnPlayerTakeDamage
	forward SS_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

public OnPlayerUpdate(playerid)
{
	if (IS_GAMEMODE)
	{
	    switch (GetPlayerState(playerid))
		{
			case PLAYER_STATE_WASTED, PLAYER_STATE_SPECTATING:
			{
			    #if defined SS_OnPlayerUpdate
					return SS_OnPlayerUpdate(playerid);
				#else
					return 1;
				#endif
			}
		}

		new Float:health;
		GetPlayerHealth(playerid, health);
		if (SS_fPlayerHealth[playerid] >= 100.0 && health != FAKE_HEALTH)
			SetPlayerHealth(playerid, FAKE_HEALTH);
		else if (health != SS_fPlayerHealth[playerid])
			SetPlayerHealth(playerid, SS_fPlayerHealth[playerid]);

		new Float:armour;
		GetPlayerArmour(playerid, armour);
		if (SS_fPlayerArmor[playerid] >= 100.0 && armour != FAKE_HEALTH)
			SetPlayerArmour(playerid, FAKE_HEALTH);
		else if (armour != SS_fPlayerArmor[playerid])
			SetPlayerArmour(playerid, SS_fPlayerArmor[playerid]);

		if (GetPlayerMoney(playerid) != SS_iPlayerMoney[playerid])
		{
		    ResetPlayerMoney(playerid);
			GivePlayerMoney(playerid, SS_iPlayerMoney[playerid]);
		}

		new weapon = GetPlayerWeapon(playerid);
		new slot = SS_GetWeaponSlot(weapon);
		if (SS_iPlayerWeaponData[playerid][slot][0] != weapon)
		{
			SetPlayerAmmo(playerid, weapon, 0);
			GivePlayerWeapon(playerid, SS_iPlayerWeaponData[playerid][slot][0], SS_iPlayerWeaponData[playerid][slot][1]);
		}
		else
		{
		    switch (weapon)
		    {
				case WEAPON_COLT45..WEAPON_SNIPER, WEAPON_MINIGUN, WEAPON_SATCHEL:
		    	{
					if (SS_iPlayerWeaponData[playerid][slot][1] != GetPlayerAmmo(playerid))
						SetPlayerAmmo(playerid, weapon, SS_iPlayerWeaponData[playerid][slot][1]);
				}

				default:
				{
					if (GetPlayerAmmo(playerid) > SS_iPlayerWeaponData[playerid][slot][1])
						SetPlayerAmmo(playerid, weapon, 0);
				}
			}
		}
	}
	
	#if defined SS_OnPlayerUpdate
		return SS_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerUpdate
    #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate SS_OnPlayerUpdate
#if defined SS_OnPlayerUpdate
	forward SS_OnPlayerUpdate(playerid);
#endif

public OnPlayerPickUpPickup(playerid, pickupid)
{
	switch (SS_iStaticPickup[pickupid])
 	{
  		case 1240:
	    {
     		if (SS_fPlayerHealth[playerid] < 100.0)
	        {
	            SS_fPlayerHealth[playerid] = 100.0;
	            
			  	if (IS_GAMEMODE)
	            	SetPlayerHealth(playerid, FAKE_HEALTH);
	        }

			#if defined OnPlayerPickUpStaticPickup
	        	return OnPlayerPickUpStaticPickup(playerid, SS_iStaticPickup[pickupid]);
			#else
				return 1;
			#endif
		}

	    case 1242:
	    {
     		if (SS_fPlayerArmor[playerid] < 100.0)
       		{
	            SS_fPlayerArmor[playerid] = 100.0;

			  	if (IS_GAMEMODE)
				  	SetPlayerArmour(playerid, FAKE_HEALTH);
           	}

			#if defined OnPlayerPickUpStaticPickup
	        	return OnPlayerPickUpStaticPickup(playerid, SS_iStaticPickup[pickupid]);
			#else
				return 1;
			#endif
   		}

     	case 321..369, 371, 372:
      	{
       		new weapon = SS_GetModelWeaponID(SS_iStaticPickup[pickupid]);
       		new slot = SS_GetWeaponSlot(weapon);

       		switch (weapon)
		    {
		        case WEAPON_BRASSKNUCKLE..WEAPON_CANE,
			 		 WEAPON_PARACHUTE:
				{
		       		SS_iPlayerWeaponData[playerid][slot][1] += 1;
		        }

				case WEAPON_GRENADE..WEAPON_MOLTOV:
		        	SS_iPlayerWeaponData[playerid][slot][1] += 8;

		        case WEAPON_COLT45:
		        	SS_iPlayerWeaponData[playerid][slot][1] += 30;

		        case WEAPON_SILENCED,
					 WEAPON_DEAGLE:
		        {
					SS_iPlayerWeaponData[playerid][slot][1] += 10;
		        }

				case WEAPON_SHOTGUN:
		        	SS_iPlayerWeaponData[playerid][slot][1] += 15;

		        case WEAPON_SAWEDOFF,
					 WEAPON_SHOTGSPA:
		        {
					SS_iPlayerWeaponData[playerid][slot][1] += 10;
		        }

				case WEAPON_UZI,
					 WEAPON_MP5,
					 WEAPON_TEC9:
     			{
				 	SS_iPlayerWeaponData[playerid][slot][1] += 60;
		        }

				case WEAPON_AK47,
					 WEAPON_M4:
     			{
				 	SS_iPlayerWeaponData[playerid][slot][1] += 80;
		        }

				case WEAPON_RIFLE:
		        	SS_iPlayerWeaponData[playerid][slot][1] += 20;

		        case WEAPON_SNIPER:
		        	SS_iPlayerWeaponData[playerid][slot][1] += 10;

		        case WEAPON_ROCKETLAUNCHER:
		        	SS_iPlayerWeaponData[playerid][slot][1] += 4;

		        case WEAPON_HEATSEEKER:
		        	SS_iPlayerWeaponData[playerid][slot][1] += 3;

		        case WEAPON_FLAMETHROWER:
		        	SS_iPlayerWeaponData[playerid][slot][1] += 100;

		        case WEAPON_MINIGUN:
		        	SS_iPlayerWeaponData[playerid][slot][1] += 500;

		        case WEAPON_SATCHEL:
		        	SS_iPlayerWeaponData[playerid][slot][1] += 5;

		        case WEAPON_SPRAYCAN:
		        	SS_iPlayerWeaponData[playerid][slot][1] += 500;

		        case WEAPON_FIREEXTINGUISHER:
		        	SS_iPlayerWeaponData[playerid][slot][1] += 100;

		        case WEAPON_CAMERA:
		        	SS_iPlayerWeaponData[playerid][slot][1] += 36;
		    }

         	if (weapon != -1)
          	{
			  	SS_iPlayerWeaponData[playerid][slot][0] = weapon;
			  	
			  	if (IS_GAMEMODE)
           			GivePlayerWeapon(playerid, weapon, SS_iPlayerWeaponData[playerid][slot][1]);
           	}

			#if defined OnPlayerPickUpStaticPickup
	        	return OnPlayerPickUpStaticPickup(playerid, SS_iStaticPickup[pickupid]);
			#else
				return 1;
			#endif
	    }
	}

	#if defined SS_OnPlayerPickUpPickup
		return SS_OnPlayerPickUpPickup(playerid, pickupid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerPickUpPickup
    #undef OnPlayerPickUpPickup
#else
    #define _ALS_OnPlayerPickUpPickup
#endif
#define OnPlayerPickUpPickup SS_OnPlayerPickUpPickup
#if defined SS_OnPlayerPickUpPickup
	forward SS_OnPlayerPickUpPickup(playerid, pickupid);
#endif

#if defined OnPlayerPickUpStaticPickup
	forward OnPlayerPickUpStaticPickup(playerid, modelid);
#endif

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
    SS_iPlayerMoney[playerid] -= 100;

	#if defined SS_OnVehicleRespray
		return SS_OnVehicleRespray(playerid, vehicleid, color1, color2);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleRespray
    #undef OnVehicleRespray
#else
    #define _ALS_OnVehicleRespray
#endif
#define OnVehicleRespray SS_OnVehicleRespray
#if defined SS_OnVehicleRespray
	forward SS_OnVehicleRespray(playerid, vehicleid, color1, color2);
#endif

public OnVehicleMod(playerid, vehicleid, componentid)
{
    new index = componentid - 1000;

    if (0 <= index < sizeof(SS_COMPONENT_PRICE))
       	SS_iPlayerMoney[playerid] -= SS_COMPONENT_PRICE[index];

	#if defined SS_OnVehicleMod
		return SS_OnVehicleMod(playerid, vehicleid, componentid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleMod
    #undef OnVehicleMod
#else
    #define _ALS_OnVehicleMod
#endif
#define OnVehicleMod SS_OnVehicleMod
#if defined SS_OnVehicleMod
	forward SS_OnVehicleMod(playerid, vehicleid, componentid);
#endif

stock SS_RemoveBuildingForPlayer(playerid, modelid, Float:fX, Float:fY, Float:fZ, Float:fRadius)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	for (new i, j = sizeof(SS_eVendingMachine); i < j; i++)
	{
	    if (modelid == SS_eVendingMachine[i][SS_e_VENDING_MACHINE_MODEL])
	    {
		    if (DISTANCE(fX, fY, 0.0, SS_eVendingMachine[i][SS_e_VENDING_MACHINE_X], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_Y], 0.0) <= fRadius)
		    {
		        #if defined STREAMER_TYPE_OBJECT
				    DestroyDynamicObject(SS_iPlayerVendingMachineObject[playerid][i]);
				#else
				    DestroyPlayerObject(playerid, SS_iPlayerVendingMachineObject[playerid][i]);
				#endif

				return 1;
		    }
 		}
	}

	return RemoveBuildingForPlayer(playerid, modelid, fX, fY, fZ, fRadius);
}
#if defined _ALS_RemoveBuildingForPlayer
    #undef RemoveBuildingForPlayer
#else
    #define _ALS_RemoveBuildingForPlayer
#endif
#define RemoveBuildingForPlayer SS_RemoveBuildingForPlayer

stock SS_GetWeaponName(weaponid, dest[], maxlen)
{
	if (weaponid < 0 || weaponid >= sizeof(SS_eWeapon))
	    return 0;

	strcat((dest[0] = EOS, dest), SS_eWeapon[weaponid][SS_e_WEAPON_NAME], maxlen);
	return 1;
}
#if defined _ALS_GetWeaponName
    #undef GetWeaponName
#else
    #define _ALS_GetWeaponName
#endif
#define GetWeaponName SS_GetWeaponName

forward __SetPlayerTeam(playerid, teamid);
public 	__SetPlayerTeam(playerid, teamid)
{
    SS_iPlayerTeam[playerid] = teamid;
}

stock SS_SetPlayerTeam(playerid, teamid)
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
#define SetPlayerTeam SS_SetPlayerTeam

stock SS_GetPlayerTeam(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return NO_TEAM;

	return SS_iPlayerTeam[playerid];
}
#if defined _ALS_GetPlayerTeam
    #undef GetPlayerTeam
#else
    #define _ALS_GetPlayerTeam
#endif
#define GetPlayerTeam SS_GetPlayerTeam

forward __SetPlayerHealth(playerid, Float:health);
public 	__SetPlayerHealth(playerid, Float:health)
{
	if (health < 0.0)
		health = 0.0;
    SS_fPlayerHealth[playerid] = health;
}

stock SS_SetPlayerHealth(playerid, Float:health)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	CallRemoteFunction("__SetPlayerHealth", "if", playerid, health);

	if (health >= 100.0)
		return SetPlayerHealth(playerid, FAKE_HEALTH);

	return SetPlayerHealth(playerid, health);
}
#if defined _ALS_SetPlayerHealth
    #undef SetPlayerHealth
#else
    #define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth SS_SetPlayerHealth

stock SS_GetPlayerHealth(playerid, &Float:health)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	health = SS_fPlayerHealth[playerid];
	return 1;
}
#if defined _ALS_GetPlayerHealth
    #undef GetPlayerHealth
#else
    #define _ALS_GetPlayerHealth
#endif
#define GetPlayerHealth SS_GetPlayerHealth

forward __SetPlayerArmour(playerid, Float:armour);
public 	__SetPlayerArmour(playerid, Float:armour)
{
	if (armour < 0.0)
		armour = 0.0;
    SS_fPlayerArmor[playerid] = armour;
}

stock SS_SetPlayerArmour(playerid, Float:armour)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
	return 0;

	CallRemoteFunction("__SetPlayerArmour", "if", playerid, armour);

	if (armour >= 100.0)
		return SetPlayerArmour(playerid, 200.0);

	return SetPlayerArmour(playerid, armour);
}
#if defined _ALS_SetPlayerArmour
    #undef SetPlayerArmour
#else
    #define _ALS_SetPlayerArmour
#endif
#define SetPlayerArmour SS_SetPlayerArmour

stock SS_GetPlayerArmour(playerid, &Float:armour)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	armour = SS_fPlayerArmor[playerid];
	return 1;
}
#if defined _ALS_GetPlayerArmour
    #undef GetPlayerArmour
#else
    #define _ALS_GetPlayerArmour
#endif
#define GetPlayerArmour SS_GetPlayerArmour

forward __SetPlayerMoney(playerid, money);
public 	__SetPlayerMoney(playerid, money)
{
    SS_iPlayerMoney[playerid] = money;
}

stock SS_GivePlayerMoney(playerid, money)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	CallRemoteFunction("__SetPlayerMoney", "ii", playerid, money + SS_iPlayerMoney[playerid]);
	return GivePlayerMoney(playerid, money);
}
#if defined _ALS_GivePlayerMoney
    #undef GivePlayerMoney
#else
    #define _ALS_GivePlayerMoney
#endif
#define GivePlayerMoney SS_GivePlayerMoney

stock SS_ResetPlayerMoney(playerid)
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
#define ResetPlayerMoney SS_ResetPlayerMoney

stock SS_GetPlayerMoney(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	return SS_iPlayerMoney[playerid];
}
#if defined _ALS_GetPlayerMoney
    #undef GetPlayerMoney
#else
    #define _ALS_GetPlayerMoney
#endif
#define GetPlayerMoney SS_GetPlayerMoney

forward __SetPlayerWeapon(playerid, slot, weaponid, ammo);
public 	__SetPlayerWeapon(playerid, slot, weaponid, ammo)
{
    SS_iPlayerWeaponData[playerid][slot][0] = weaponid;
    SS_iPlayerWeaponData[playerid][slot][1] = ammo;
}

stock SS_GivePlayerWeapon(playerid, weaponid, ammo)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	new slot = SS_GetWeaponSlot(weaponid);
	if (slot == -1)
		return 0;

	CallRemoteFunction("__SetPlayerWeapon", "iiii", playerid, slot, weaponid, ammo + SS_iPlayerWeaponData[playerid][slot][1]);
	return GivePlayerWeapon(playerid, weaponid, ammo);
}
#if defined _ALS_GivePlayerWeapon
    #undef GivePlayerWeapon
#else
    #define _ALS_GivePlayerWeapon
#endif
#define GivePlayerWeapon SS_GivePlayerWeapon

stock SS_ResetPlayerWeapons(playerid)
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
#define ResetPlayerWeapons SS_ResetPlayerWeapons

stock SS_GetPlayerWeaponData(playerid, slot, &weapons, &ammo)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (slot > 12 || slot < 0)
		return 0;

	weapons = SS_iPlayerWeaponData[playerid][slot][0];
	ammo = SS_iPlayerWeaponData[playerid][slot][1];
	return 1;
}
#if defined _ALS_GetPlayerWeaponData
    #undef GetPlayerWeaponData
#else
    #define _ALS_GetPlayerWeaponData
#endif
#define GetPlayerWeaponData SS_GetPlayerWeaponData

stock SS_SetPlayerAmmo(playerid, weaponid, ammo)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	CallRemoteFunction("__SetPlayerWeapon", "iiii", playerid, SS_GetWeaponSlot(weaponid), weaponid, ammo);
    return SetPlayerAmmo(playerid, weaponid, ammo);
}
#if defined _ALS_SetPlayerAmmo
    #undef SetPlayerAmmo
#else
    #define _ALS_SetPlayerAmmo
#endif
#define SetPlayerAmmo SS_SetPlayerAmmo

stock SS_GetPlayerAmmo(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	new slot = SS_GetWeaponSlot(GetPlayerWeapon(playerid));
	if (slot > 12 || slot < 0)
		return 0;

	return SS_iPlayerWeaponData[playerid][slot][1];
}
#if defined _ALS_GetPlayerAmmo
    #undef GetPlayerAmmo
#else
    #define _ALS_GetPlayerAmmo
#endif
#define GetPlayerAmmo SS_GetPlayerAmmo

stock SS_AddPlayerClass(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
	new ret = AddPlayerClass(modelid, spawn_x, spawn_y, spawn_z, z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);

	SS_eClassSpawnData[ret][SS_e_SPAWN_DATA_TEAM] = NO_TEAM;
	SS_eClassSpawnData[ret][SS_e_SPAWN_DATA_WEAPON1][0] = weapon1;
	SS_eClassSpawnData[ret][SS_e_SPAWN_DATA_WEAPON1][1] = weapon1_ammo;
	SS_eClassSpawnData[ret][SS_e_SPAWN_DATA_WEAPON2][0] = weapon2;
	SS_eClassSpawnData[ret][SS_e_SPAWN_DATA_WEAPON2][1] = weapon2_ammo;
	SS_eClassSpawnData[ret][SS_e_SPAWN_DATA_WEAPON3][0] = weapon3;
	SS_eClassSpawnData[ret][SS_e_SPAWN_DATA_WEAPON3][1] = weapon3_ammo;

	return ret;
}
#if defined _ALS_AddPlayerClass
    #undef AddPlayerClass
#else
    #define _ALS_AddPlayerClass
#endif
#define AddPlayerClass SS_AddPlayerClass

stock SS_AddPlayerClassEx(teamid, modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
	new ret = AddPlayerClassEx(teamid, modelid, spawn_x, spawn_y, spawn_z, z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);

	SS_eClassSpawnData[ret][SS_e_SPAWN_DATA_TEAM] = teamid;
	SS_eClassSpawnData[ret][SS_e_SPAWN_DATA_WEAPON1][0] = weapon1;
	SS_eClassSpawnData[ret][SS_e_SPAWN_DATA_WEAPON1][1] = weapon1_ammo;
	SS_eClassSpawnData[ret][SS_e_SPAWN_DATA_WEAPON2][0] = weapon2;
	SS_eClassSpawnData[ret][SS_e_SPAWN_DATA_WEAPON2][1] = weapon2_ammo;
	SS_eClassSpawnData[ret][SS_e_SPAWN_DATA_WEAPON3][0] = weapon3;
	SS_eClassSpawnData[ret][SS_e_SPAWN_DATA_WEAPON3][1] = weapon3_ammo;

	return ret;
}
#if defined _ALS_AddPlayerClassEx
    #undef AddPlayerClass
#else
    #define _ALS_AddPlayerClassEx
#endif
#define AddPlayerClassEx SS_AddPlayerClassEx

forward __SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo);
public  __SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
{
	SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_TEAM] = team;
	SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON1][0] = weapon1;
	SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON1][1] = weapon1_ammo;
	SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON2][0] = weapon2;
	SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON2][1] = weapon2_ammo;
	SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON3][0] = weapon3;
	SS_ePlayerSpawnData[playerid][SS_e_SPAWN_DATA_WEAPON3][1] = weapon3_ammo;
}

stock SS_SetSpawnInfo(playerid, team, skin, Float:x, Float:y, Float:z, Float:rotation, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo)
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
#define SetSpawnInfo SS_SetSpawnInfo

forward __SetStaticPickup(pickupid, model);
public  __SetStaticPickup(pickupid, model)
{
    SS_iStaticPickup[pickupid] = model;
}

stock SS_AddStaticPickup(model, type, Float:X, Float:Y, Float:Z, Virtualworld)
{
	switch (model)
	{
	    case 1240, 1242, 321..369, 371, 372:
		{}

     	default:
     		return AddStaticPickup(model, type, X, Y, Z, Virtualworld);
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
#define AddStaticPickup SS_AddStaticPickup

stock SS_CreatePickup(model, type, Float:X, Float:Y, Float:Z, Virtualworld)
{
    if (type == 1)
		return CreatePickup(model, type, X, Y, Z, Virtualworld);

	switch (model)
	{
	    case 1240, 1242, 321..369, 371, 372:
	    {}

     	default:
    	 	return CreatePickup(model, type, X, Y, Z, Virtualworld);
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
#define CreatePickup SS_CreatePickup

stock SS_DestroyPickup(pickupid)
{
	if (SS_iStaticPickup[pickupid])
		CallRemoteFunction("__SetStaticPickup", "ii", pickupid, 0);

	return DestroyPickup(pickupid);
}
#if defined _ALS_DestroyPickup
    #undef DestroyPickup
#else
    #define _ALS_DestroyPickup
#endif
#define DestroyPickup SS_DestroyPickup

stock SS_SendDeathMessage(playerid, killerid, reason)
{
	switch (reason)
	{
	    case WEAPON_UNKNOWN:
	    	reason = WEAPON_DROWN;
	    case WEAPON_CARPARK:
	    	reason = WEAPON_VEHICLE;
	    case WEAPON_PISTOLWHIP:
	    	reason = WEAPON_UNARMED;
	    case WEAPON_VEHICLE_M4:
	    	reason = WEAPON_M4;
	    case WEAPON_VEHICLE_MINIGUN:
	   		reason = WEAPON_MINIGUN;
	}

	return SendDeathMessage(playerid, killerid, reason);
}
#if defined _ALS_SendDeathMessage
    #undef SendDeathMessage
#else
    #define _ALS_SendDeathMessage
#endif
#define SendDeathMessage SS_SendDeathMessage

/*
native SetWeaponName(weaponid, const name[]);
*/
stock SetWeaponName(weaponid, const name[])
{
	if (weaponid < 0 || weaponid >= sizeof(SS_eWeapon))
	    return 0;

    format(SS_eWeapon[weaponid][SS_e_WEAPON_NAME], 35, name);
    return 1;
}

/*
native Float:GetPlayerWeaponDamage(playerid, weaponid);
*/
stock Float:GetPlayerWeaponDamage(playerid, weaponid)
{
	if (weaponid < 0 || weaponid >= sizeof(SS_eWeapon))
	    return 0.0;

    return SS_fPlayerWeaponDamage[playerid][weaponid];
}

/*
native SetPlayerWeaponDamage(playerid, weaponid, Float:damage);
*/
stock SetPlayerWeaponDamage(playerid, weaponid, Float:damage)
{
	if (weaponid < 0 || weaponid >= sizeof(SS_eWeapon))
	    return 0;

    SS_fPlayerWeaponDamage[playerid][weaponid] = damage;
    return 1;
}

/*
native ResetPlayerWeaponDamage(playerid, weaponid);
*/
stock ResetPlayerWeaponDamage(playerid, weaponid)
{
	if (weaponid < 0 || weaponid >= sizeof(SS_eWeapon))
	    return 0;

    SS_fPlayerWeaponDamage[playerid][weaponid] = SS_eWeapon[weaponid][SS_e_WEAPON_DAMAGE];
    return 1;
}

/*
native SetPlayerVendingMachine(playerid, Float:health = 35.0, modelid = -1, Float:fX = -1000.0, Float:fY = -1000.0, Float:fZ = -1000.0, Float:fRadius = -1000.0);
*/
stock SetPlayerVendingMachineAmount(playerid, Float:health = 35.0, modelid = -1, Float:fX = -1000.0, Float:fY = -1000.0, Float:fZ = -1000.0, Float:fRadius = -1000.0)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (fRadius == -1000.0)
	{
	    if (modelid == -1)
	    {
		    for (new i, j = sizeof(SS_eVendingMachine); i < j; i++)
				SS_fPlayerVendingMachineAmount[playerid][i] = health;
	    }
	    else
		{
			for (new i, j = sizeof(SS_eVendingMachine); i < j; i++)
			{
			    if (modelid == SS_eVendingMachine[i][SS_e_VENDING_MACHINE_MODEL])
		    	{
		    	    SS_fPlayerVendingMachineAmount[playerid][i] = health;
		    	}
			}
		}
	}
	else
	{
	    if (modelid == -1)
	    {
		    for (new i, j = sizeof(SS_eVendingMachine); i < j; i++)
		    {
		        if (DISTANCE(fX, fY, 0.0, SS_eVendingMachine[i][SS_e_VENDING_MACHINE_X], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_Y], 0.0) <= fRadius)
		    	{
					SS_fPlayerVendingMachineAmount[playerid][i] = health;
				}
			}
	    }
	    else
		{
			for (new i, j = sizeof(SS_eVendingMachine); i < j; i++)
			{
			    if (modelid == SS_eVendingMachine[i][SS_e_VENDING_MACHINE_MODEL])
			    {
				    if (DISTANCE(fX, fY, 0.0, SS_eVendingMachine[i][SS_e_VENDING_MACHINE_X], SS_eVendingMachine[i][SS_e_VENDING_MACHINE_Y], 0.0) <= fRadius)
				    {
				        SS_fPlayerVendingMachineAmount[playerid][i] = health;
				    }
		 		}
			}
		}
	}
    return 1;
}
