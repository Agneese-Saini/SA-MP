// World war IV
// (c) copyright 2015 - Gammix

#include <a_samp>

#undef  MAX_PLAYERS
#define MAX_PLAYERS (200)

#define BUILD 8
#define BUILD_DATE "24/01/2017"

main()
{
	SetGameModeText("Build "#BUILD" - "BUILD_DATE"");
}

#pragma dynamic (10000)

#define KEY_AIM 128

// Plugins
#include <sscanf2>
#include <streamer>
#include <colandreas>
#include <regex>

// Database
#include <yoursql>

// Misc.
#include <colors>
#include <spectate>
#include <gangzones>
#include <izcmd>
#include <foreach>
#include <progress2>
#include <spikestrip>
#include <timestamptodate>

// Module
#include "maps.pwn"

#define IsValidEmail(%1) \
	regex_match(%1, "[a-zA-Z0-9_\\.]+@([a-zA-Z0-9\\-]+\\.)+[a-zA-Z]{2,4}")

#define IsValidIp(%1) \
	regex_match(%1, "(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.+){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)")

#define SET_ALPHA(%1,%2) \
	((%1 & ~0xFF) | (clamp(%2, 0x00, 0xFF)))

#define DIALOG_ID_REGISTER (1)
#define DIALOG_ID_LOGIN (2)
#define DIALOG_ID_FORGOT_PASSWORD (3)
#define DIALOG_ID_EMAIL (4)
#define DIALOG_ID_DISGUIZE (5)
#define DIALOG_ID_SHOP (6)
#define DIALOG_ID_CLASS (7)
#define DIALOG_ID_DUEL (8)
#define DIALOG_ID_SPAWN (9)
#define DIALOG_ID_BUILD (10)
#define DIALOG_ID_AIRSTRIKE (11)
#define DIALOG_ID_CAREPACK (12)
#define DIALOG_ID_MUSICBOX (13)

#define DIALOG_ID_REPORTS (100)
#define DIALOG_ID_REPORTS_PAGE (101)
#define DIALOG_ID_MUTE_LIST (102)
#define DIALOG_ID_UNMUTE (103)
#define DIALOG_ID_JAILED_LIST (104)
#define DIALOG_ID_UNJAIL (105)
#define DIALOG_ID_AUTO_LOGIN (106)

#define DIALOG_ID_EVENT (107)
#define DIALOG_ID_HELP (109)

#define MENU_ID_INVENTORY (1)
#define MENU_ID_WEAPONS (2)
#define MENU_ID_DUEL_WEAPON (3)
#define MENU_ID_PERSONAL_WEAPON (4)

#define MAX_WARNINGS (5)
#define MAX_ADMIN_LEVELS (6)

#if !defined FLOAT_INFINITY
    #define FLOAT_INFINITY (Float:0x7F800000)
#endif

#define MAX_TEAMS (7)

enum e_TEAM
{
	teamName[35],
	teamColor,
	teamSkin,
	Float:teamCam[3],
	Float:teamCam2[3],
	Float:teamCam3[4],
	Float:teamSpawn1[4],
	Float:teamSpawn2[4],
	Float:teamSpawn3[4],
	Float:teamBase[4],
	Float:teamProto[4],
	Float:teamProtoCP[3],
 	teamBaseId,
  	teamProtoId,
	teamProtoAttacker
};
new const gTeam[MAX_TEAMS][e_TEAM] =
{
	{
		"Germany",
		0x00CED1FF,
		285,
		{-252.8023, 2607.9700, 70.7897}, {-252.7830, 2606.9705, 70.7996}, {-252.6132, 2602.1384, 70.6012, 354.7943},
		{-249.8603,2599.3525,62.8582,357.0970}, {-272.0836,2674.8582,62.6376,273.4597}, {-170.0571,2705.0310,62.5153,93.3148},
		{-312.5,2617.1875,-117.1875,2777.34375},
		{-168.7945,2745.5662,62.4688,98.1378}, {-166.4540,2719.9355,62.0010}
	},
	{
		"Japan",
		0x8B008BFF,
		203,
		{1094.1646, 2284.7827, 17.9550}, {1095.1610, 2284.7002, 18.0902}, {1100.5251, 2284.4727, 18.1641, 84.7141},
		{1130.5125,2290.4436,10.8203,89.1754}, {1019.1215,2253.1057,10.8203,92.3088}, {973.6959,2246.0010,11.1672,161.8694},
		{892.578125,2173.828125,1128.90625,2453.125},
		{1023.7042,2376.6814,10.9630,176.9905}, {1023.5306,2354.5344,10.8203}
	},
	{
		"Africa",
		0xA0BCFBFF,
		102,
		{-737.9478, 1532.4700, 42.2281}, {-736.9523, 1532.5557, 42.3932}, {-732.5506, 1532.7202, 42.9766, 87.4691},
		{-798.5768,1557.1158,27.1172,96.1218}, {-820.6153,1602.2709,27.1244,178.2159}, {-796.5872,1590.1790,27.1172,176.9625},
		{-900.390625,1462.890625,-701.171875,1617.1875},
		{-776.7645,1522.2542,27.2548,359.1660}, {-824.1104,1543.1155,27.1172}
	},
	{
		"Soviet Union",
		0x00FF00FF,
		111,
		{-1488.6971, 2607.5005, 62.6617}, {-1488.0907, 2608.2949, 62.7616}, {-1485.6035, 2611.5359, 62.7813, 140.1490},
		{-1479.3788,2623.6807,58.7813,85.6585}, {-1464.6351,2586.3530,55.8403,1.3944}, {-1522.0659,2588.9683,55.8359,356.3810},
		{-1613.28125,2517.578125,-1378.90625,2714.84375},
		{-1463.1654,2561.2729,57.1816,91.3553}, {-1514.7504,2542.2246,55.6918}
	},
	{
		"Australia",
		0xFFD700FF,
		127,
  		{-89.1187, 1193.8519, 22.8610}, {-89.0952, 1192.8495, 22.9010}, {-89.1619,1189.9597,22.9675,359.2181},
		{-135.7707,1116.8470,20.1966,6.3144}, {-110.9746,1139.5753,19.7422,359.4211}, {-150.6350,1182.3279,19.9052,355.3711},
		{-269.53125,1015.625,82.03125,1220.703125},
		{-169.4736,1048.6688,19.7711,87.6834}, {-172.3002,1016.8591,19.7422}
	},
	{
		"Brazil",
		0x033999FF,
		175,
		{1708.1044, 929.3951, 18.0076}, {1708.0712, 930.3970, 18.0426}, {1707.9358, 934.7831, 17.6145, 177.0879},
		{1736.5856,1054.1406,10.8203,95.1389}, {1600.9348,1067.0264,10.8203,268.5994}, {1623.7683,997.9296,10.8203,267.0118},
		{1562.5,880.859375,1777.34375,1146.484375},
		{1657.7343,987.2290,10.8146,178.7493}, {1682.7275,988.4735,10.8203}
	},
	{
		"India",
		0xFF4500FF,
		206,
		{1480.6316, 2668.4268, 22.4025}, {1480.6359, 2667.4250, 22.4426}, {1480.8873,2663.3279,22.0299,354.9832},
		{1433.8374,2620.7874,11.3926,358.2195}, {1430.4445,2642.4910,11.3926,87.2070}, {1376.4081,2654.1792,10.8203,88.4604},
		{1296.875,2587.890625,1580.078125,2703.125},
		{1381.6289,2652.0388,10.9522,5.0166}, {1360.6295,2694.8547,10.8203}
	}
};

#define MAX_CLASSES (6)

enum e_CLASS
{
	className[35],
	classRank,
	classModel,
	classWeapon1[2],
	classWeapon2[2],
	classWeapon3[2],
	classWeapon4[2],
	classWeapon5[2]
};
new const gClass[MAX_CLASSES][e_CLASS] =
{
	{"Support", 0,	1314,	{33, 100}, {30, 300}, {24, 200}, {17, 3}, {0, 0}},
	{"Medic", 	1,	1240,	{25, 200}, {29, 300}, {23, 200}, {17, 5}, {5, 1}},
	{"Sniper", 	2,	358,	{34, 200}, {24, 200}, {29, 200}, {16, 3}, {4, 1}},
	{"Engineer",4,	564,	{30, 300}, {27, 200}, {24, 100}, {16, 3}, {3, 1}},
	{"Pilot", 	5,	520,	{25, 200}, {22, 150}, {35, 1}, {0, 0}, {0, 0}},
	{"Spy", 	8,	1275,	{31, 400}, {25, 200}, {29, 200}, {18, 5}, {4, 1}}
};

enum e_RANK
{
	rankName[35],
	rankScore,
	Float:rankHealth,
	Float:rankArmour
};
new const gRank[][e_RANK] =
{
	{"Private",				0,		100.0,	100.0},
	{"Private First Class",	50,		100.0,	100.0},
	{"Specialist",			100,	75.0,	0.0},
	{"Corporal",			500,	95.0,	5.0},
	{"Sergeant",			1000,	100.0,	5.0},
	{"Staff Sergeant",		1500,	100.0,	15.0},
	{"Master Sergeant",		2500,	100.0,	15.0},
	{"Sergeant Major",		4500,	100.0,	40.0},
	{"Officer",				6000,	100.0,	50.0},
	{"Warrant Officer",		7500,	100.0,	60.0},
	{"Chief Warrant Officer",9000,	100.0,	70.0},
	{"Lieutenant",			10000,	100.0,	70.0},
	{"First Lieutenant",	12500,	100.0,	85.0},
	{"Second Lieutenant",	15000,	100.0,	100.0},
	{"Captain",	 			20000,	100.0,	100.0},
	{"Major",				25000,	100.0,	100.0},
	{"Colonel",				30000,	100.0,	100.0},
	{"Marshall",			33000,	100.0,	100.0},
	{"Field Marshall",		37000,	100.0,	100.0},
	{"General",				40000,	100.0,	100.0},
	{"Brigadier General",	45000,	100.0,	100.0},
	{"Major General",		50000,	100.0,	100.0},
	{"Master Of War",		60000,	100.0,	100.0},
	{"God Of War",			9999999,100.0,	100.0}
};

enum e_SHOP
{
	Float:shopPos[3],
	shopTeam,
	shopAreaid
};
new const gShop[][e_SHOP] =
{
	{{-254.2268,2602.1331,62.8582}, 0},
	{{1118.0779,2259.7751,10.8203}, 1},
	{{-786.9837,1592.8396,27.6948},	2},
	{{-1478.1508,2641.2576,58.7879},3},
	{{-177.8801,1160.4963,19.7422}, 4},
	{{1699.8306,962.3998,10.8203}, 	5},
	{{1433.1210,2620.1143,11.3926}, 6}
};

#define CAPTURE_TIME 30
enum e_ZONE
{
	zoneName[35],
	Float:zonePos[4],
	Float:zoneCP[3],
	Float:zoneSpawn[4],
	zoneOwner,
	zoneAttacker,
	zoneTick,
	zoneId,
	zoneCPId,
	zoneTimer,
	zonePlayer,
	Text3D:zoneLabel
};
new const gZone[][e_ZONE] =
{
    {"Snake Farm", 		{-62.5000000000005,2318.359375, 23.4375,2390.625},{-36.5458, 2347.6426, 24.1406},	{0.0, 0.0, 0.0, 0.0},	0},
	{"Fishing Area", 	{210.2018, 2849.402, 303.6248, 2931.147}, 		{257.1011,2890.2222,11.3209},		{0.0, 0.0, 0.0, 0.0},	0},
	{"Area 51", 		{-46.875,1697.265625, 423.828125,2115.234375}, 	{254.4592,1802.8997,7.4285},		{0.0, 0.0, 0.0, 0.0},	0},
	{"Come-A-Lot", 		{-617.1875,2531.25, -455.078125,2658.203125}, 	{-551.6992,2593.0771,53.9348},		{0.0, 0.0, 0.0, 0.0},	1},
	{"Oil Canary", 		{95.703125,1339.84375, 287.109375,1484.375}, 	{221.0856,1422.6615,10.5859},		{0.0, 0.0, 0.0, 0.0},	1},
	{"Oil Factory", 	{529.296875,1205.078125, 636.71875,1267.578125},{558.9932,1221.8896,11.7188},		{0.0, 0.0, 0.0, 0.0},	1},
	{"Quarry", 			{439.453125,748.046875, 863.28125,992.1875}, 	{588.3246,875.7402,-42.4973},		{0.0, 0.0, 0.0, 0.0},	1},
	{"Army Restaurant", {-357.421875,1707.03125, -253.90625,1835.9375}, {-314.8433,1773.9176,43.6406},		{0.0, 0.0, 0.0, 0.0},	2},
	{"Big Ear", 		{-437.5,1513.671875, -244.140625,1636.71875}, 	{-311.0136,1542.9733,75.5625},		{0.0, 0.0, 0.0, 0.0},	2},
	{"Desert Airport",	{46.7115, 2358.931, 490.4708, 2604.166}, 		{406.1056,2456.0640,16.5000},		{0.0, 0.0, 0.0, 0.0},	2},
	{"The Hospital", 	{966.796875,972.65625, 1166.015625,1160.15625}, {1044.83008, 1013.94354, 10.19003},	{0.0, 0.0, 0.0, 0.0},	2},
	{"Airport", 		{1230.46875,1142.578125, 1640.625,1798.828125}, {1603.51587, 1178.88391, 13.41670},	{0.0, 0.0, 0.0, 0.0},	3},
	{"Ammu-Nation", 	{-351.5625,811.5234375, -284.1796875,884.765625},{-315.79111, 834.14001, 13.44070},	{0.0, 0.0, 0.0, 0.0},	3},
	{"Jay's Diner", 	{-1964.84375,2328.125, -1906.25,2402.34375}, 	{-1940.30542, 2380.05981, 48.99673},{0.0, 0.0, 0.0, 0.0},	3},
	{"Broken Bridge", 	{-1464.84375,679.6875, -1074.21875,1148.4375}, 	{-1341.66602, 885.54590, 35.29177},	{0.0, 0.0, 0.0, 0.0},	3},
	{"The Smuggler's ship",{-1511.71875,437.5, -1226.5625,527.34375}, 	{-1346.17224, 492.02451, 10.39430},	{0.0, 0.0, 0.0, 0.0},	4},
	{"Fuel Station", 	{-1500,1851.5625, -1468.75,1878.90625}, 		{-1471.26392, 1862.32068, 31.83293},{0.0, 0.0, 0.0, 0.0},	4},
	{"Cluckin Bell", 	{-1230.46875,1789.0625, -1175.78125,1839.84375},{-1212.40698, 1831.70020, 41.23455},{0.0, 0.0, 0.0, 0.0},	4},
	{"Missile Factory", {-476.5625,2195.3125, -351.5625,2277.34375}, 	{-427.48999, 2205.93652, 41.53221},	{0.0, 0.0, 0.0, 0.0},	4},
	{"Gas station", 	{542.96875 , 1632.8125, 687.5 , 1750.0}, 		{670.12445, 1703.94055, 6.38832},	{0.0, 0.0, 0.0, 0.0},	5},
	{"Abandoned Ship", 	{-1480.46875,1476.5625, -1332.03125,1519.53125},{-1435.59998, 1480.19714, 1.04882},	{0.0, 0.0, 0.0, 0.0},	5},
	{"The Villa", 		{-718.75,917.96875, -644.53125,976.5625}, 		{-688.13782, 936.66589, 13.04289},	{0.0, 0.0, 0.0, 0.0},	5},
	{"Cabin house", 	{-946.2890625,1415.0390625, -928.7109375,1435.546875},{-1052.63696, 1547.54651, 32.64770},{0.0, 0.0, 0.0, 0.0},5},
	{"Cargo Ship", 		{-2531.25,1526.3671875, -2288.0859375,1596.6796875},{-2474.13135, 1548.24231, 32.42630},{0.0, 0.0, 0.0, 0.0},6},
	{"Veen's Bait Shop",{-1371.09375,2050.78125, -1345.3125,2067.1875}, {-1353.93994, 2057.31470, 52.31471},{0.0, 0.0, 0.0, 0.0},	6},
	{"Abandoned City", 	{-1364.0625,2505.46875, -1256.25,2568.75}, 		{-1303.56567, 2533.47437, 87.04325},{0.0, 0.0, 0.0, 0.0},	6},
	{"Rocket site", 	{-843.75,2371.875, -728.90625,2453.90625}, 		{-797.59857, 2415.76099, 156.04553},{0.0, 0.0, 0.0, 0.0},	6}
};
new PlayerText:ptxtCapture[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};
new PlayerBar:pbarCapture[MAX_PLAYERS] = {INVALID_PLAYER_BAR_ID, ...};

new pSkipTimer[MAX_PLAYERS];
new bool:pInClass[MAX_PLAYERS];
new bool:pLogged[MAX_PLAYERS];

new Text:txtBase[4] = {Text:INVALID_TEXT_DRAW, ...};
new Text:txtConnect[4] = {Text:INVALID_TEXT_DRAW, ...};
new Text:txtTeam[22] = {Text:INVALID_TEXT_DRAW, ...};
new Text:txtClass[19] = {Text:INVALID_TEXT_DRAW, ...};

new pTeam[MAX_PLAYERS];
new pClass[MAX_PLAYERS];
new pRank[MAX_PLAYERS];
new pSpawn[MAX_PLAYERS];

new pActionTime[MAX_PLAYERS];
new pDisguizeKits[MAX_PLAYERS];
new pBuildMode[MAX_PLAYERS];

new pProtectTick[MAX_PLAYERS];
new Text3D:pProtectLabel[MAX_PLAYERS];

new pKiller[MAX_PLAYERS][2];

new bool:pHasHelmet[MAX_PLAYERS];
new bool:pHasMask[MAX_PLAYERS];

new pLastDamageTime[MAX_PLAYERS];

new pWeaponsSpree[MAX_PLAYERS][13];

new gServerTimer;

new Text3D:pRankLabel[MAX_PLAYERS];

new PlayerText:ptxtStats[MAX_PLAYERS] = {PlayerText:INVALID_TEXT_DRAW, ...};

enum e_DUEL
{
	bool:duelActive,
	duelPlayer,
	duelWeapon,
	duelBet
};
new pDuel[MAX_PLAYERS][e_DUEL];

new pIdx[MAX_PLAYERS];

new const gNotifications[][] =
{
	"New here, get started with ~y~/help ~w~~h~for frequently asked question and ~y~/cmds ~w~~h~for commands list.",
    "We also have our forums/website where you will find intersting discussions and game relate talks, visit ~b~~h~~h~~h~http://www.worldwargaming.com",
    "Type ~b~~h~/missions ~w~~h~or ~b~~h~/objective ~w~~h~to see what you can do for your team.",
	"If you saw a cheater or rulebreaker, report against him/her through ~r~/report (id) (reason) ~w~~h~only.",
	"We recommend everyone to go through ~r~/rules ~w~~h~if you haven't, please read them first because rulebreakers aren't tolerated.",
	"Unlock premium features by donating to this server, checkout a full list in ~p~/donate~w~~h~.",
	"~g~ADMIN APPLICATION OPEN~w~~h~, interested to be a staff memeber of the server, apply at the forums completing the requirements.",
	"Read ~g~/credits ~w~~h~to get server developer, owner, management information.",
	"You can privetly chat to your friends via ~y~/pm ~w~~h~and ~y~/r ~w~~h~for quickly replying the last PM.",
	"To check your stats/progress made this far, type ~g~/stats~w~~h~ and also watch other's stats by ~g~/stats (player)~w~~h~.",
	"Your stats are automatically saved at the instant you progress/deprogress, they can never be lost.",
	"To know your rank information, type ~b~/rank ~w~~h~and watch other's rank by ~b~/rank (player)~w~~h~.",
	"You can change your team via ~r~/st~w~~h~, class via ~r~/sc~w~~h~ and spawn via ~r~/ss~w~~h~.",
	"Each class have different ability and each require different rank/score, checkout the class related issues/info. in ~y~/chelp~w~~h~.",
	"Headshots can be prevented easily by buyig a ~b~Protection helmet ~w~~h~from the shop.",
	"You can checkout the bonus player in ~y~/bonusplayer ~w~~h~and bonus zone in ~y~/bonuszone~w~~h~."
};
new Text:txtNotify;
new gNotifyId;
new gNotifyGap;

new PlayerText:ptxtNotify[MAX_PLAYERS];
new pNotifyTimer[MAX_PLAYERS];

new pSync[MAX_PLAYERS];

new Text:txtMenu[(6 * 4) + 1] = {Text:INVALID_TEXT_DRAW, ...};
new PlayerText:ptxtMenu[(6 * 4) + 1] = PlayerText:INVALID_TEXT_DRAW;
new pMenu[MAX_PLAYERS];
new pMenuTick[MAX_PLAYERS];

new menuPersonalWeaponModels[] =
{
	335,
	341,
	342,
	344,
	346,
	347,
	348,
	349,
	350,
	351,
	352,
	353,
	355,
	356,
	372,
	357,
	358,
	359,
	361,
	362
};
new menuPersonalWeaponLabels[][] =
{
	"Knife",
	"Chainsaw",
	"Grenade",
	"Molotov Cocktail",
	"9mm",
	"Silenced 9mm",
	"Desert Eagle",
	"Shotgun",
	"Sawnoff Shotgun",
	"Combat Shotgun",
	"UZI",
	"MP5",
	"AK-47",
	"M4",
	"Tec-9",
	"Country Rifle",
	"Sniper Rifle",
	"RPG",
	"Flamethrower",
	"Minigun"
};

new menuDuelModels[] =
{
	335,
	341,
	342,
	344,
	346,
	347,
	348,
	349,
	350,
	351,
	352,
	353,
	355,
	356,
	372,
	357,
	358,
	359,
	361,
	362
};
new menuDuelLabels[][] =
{
	"Knife",
	"Chainsaw",
	"Grenade",
	"Molotov Cocktail",
	"9mm",
	"Silenced 9mm",
	"Desert Eagle",
	"Shotgun",
	"Sawnoff Shotgun",
	"Combat Shotgun",
	"UZI",
	"MP5",
	"AK-47",
	"M4",
	"Tec-9",
	"Country Rifle",
	"Sniper Rifle",
	"RPG",
	"Flamethrower",
	"Minigun"
};

new menuWeaponModels[] =
{
	335,
	339,
	341,
	342,
	343,
	344,
	346,
	347,
	348,
	349,
	350,
	351,
	352,
	353,
	355,
	356,
	372,
	357,
	358,
	359,
	360,
	361,
	362,
	363
};
new menuWeaponLabels[][] =
{
	"Knife~n~1 ammo~n~~r~~h~~h~-$1500",
	"Katana~n~1 ammo~n~~r~~h~~h~-$1500",
	"Chainsaw~n~1 ammo~n~~r~~h~~h~-$3000",
	"Grenade~n~1 ammo~n~~r~~h~~h~-$2500",
	"Tear Gas~n~1 ammo~n~~r~~h~~h~-$2500",
	"Molotov Cocktail~n~1 ammo~n~~r~~h~~h~-$3000",
	"9mm~n~100 ammo~n~~r~~h~~h~-$5000",
	"Silenced 9mm~n~100 ammo~n~~r~~h~~h~-$3500",
	"Desert Eagle~n~100 ammo~n~~r~~h~~h~-$6500",
	"Shotgun~n~100 ammo~n~~r~~h~~h~-$7000",
	"Sawnoff Shotgun~n~100 ammo~n~~r~~h~~h~-$10000",
	"Combat Shotgun~n~100 ammo~n~~r~~h~~h~-$9500",
	"UZI~n~200 ammo~n~~r~~h~~h~-$8500",
	"MP5~n~200 ammo~n~~r~~h~~h~-$7000",
	"AK-47~n~200 ammo~n~~r~~h~~h~-$7500",
	"M4~n~200 ammo~n~~r~~h~~h~-$8500",
	"Tec-9~n~200 ammo~n~~r~~h~~h~-$8500",
	"Country Rifle~n~100 ammo~n~~r~~h~~h~-$3500",
	"Sniper Rifle~n~100 ammo~n~~r~~h~~h~-$10000",
	"RPG~n~1 ammo~n~~r~~h~~h~-$3500",
	"HS Rocket~n~1 ammo~n~~r~~h~~h~-$4000",
	"Flamethrower~n~200 ammo~n~~r~~h~~h~-$4500",
	"Minigun~n~100 ammo~n~~r~~h~~h~-$10000",
	"Satchel Charge~n~1 ammo~n~~r~~h~~h~-$3500"
};

new menuInventoryModels[] =
{
	11738,
	2945,
	1654,
	2061,
	1279,
	647,
	2226,
	2892,
    19602,
    1242,
	19036
};
new menuInventoryLabels[][] =
{
	"Medickit~n~~r~~h~~h~~h~-$1500",
	"Nettrap~n~~r~~h~~h~~h~-$15000",
	"Dynamite~n~~r~~h~~h~~h~-$10000",
	"Ammunation~n~~r~~h~~h~~h~-$5000",
	"Drug Bundle~n~~r~~h~~h~~h~-$3500",
	"Camouflage~n~~r~~h~~h~~h~-$7500",
	"Musicbox~n~~r~~h~~h~~h~-$25000",
	"Spikestrip~n~~r~~h~~h~~h~-$10000",
	"Landmine~n~~r~~h~~h~~h~-$20000",
	"Protection Jacket~n~~r~~h~~h~~h~-$25000",
	"Protection Mask~n~~r~~h~~h~~h~-$15000"
};

new pInventory[MAX_PLAYERS][sizeof(menuInventoryModels)];

new pNetTrapObject[MAX_PLAYERS][2];
new pNetTrapArea[MAX_PLAYERS][2];
new Text3D:pNetTrapLabel[MAX_PLAYERS][2];
new pNetTrapTimer[MAX_PLAYERS][2];
new bool:pTrapped[MAX_PLAYERS];
new pTrappedTimer[MAX_PLAYERS];
new pTrappedObject[MAX_PLAYERS];

new pDynamiteObject[MAX_PLAYERS][3];
new Text3D:pDynamiteLabel[MAX_PLAYERS][3];

new pSpikeObject[MAX_PLAYERS][3];
new pSpikeTimer[MAX_PLAYERS][3];
new Text3D:pSpikeLabel[MAX_PLAYERS][3];

new pMusicBoxObject[MAX_PLAYERS];
new pMusicBoxAreaid[MAX_PLAYERS];
new Text3D:pMusicBoxLabel[MAX_PLAYERS];
new pMusicBoxURL[MAX_PLAYERS][150];

new pLandmineObject[MAX_PLAYERS][3];
new pLandmineAreaid[MAX_PLAYERS][3];
new Text3D:pLandmineLabel[MAX_PLAYERS][3];

new pUpdateTimer[MAX_PLAYERS];

new const gVehicleModelNames[212][] =
{
	{"Landstalker"},{"Bravura"},{"Buffalo"},{"Linerunner"},{"Perrenial"},{"Sentinel"},{"Dumper"},
	{"Firetruck"},{"Trashmaster"},{"Stretch"},{"Manana"},{"Infernus"},{"Voodoo"},{"Pony"},{"Mule"},
	{"Cheetah"},{"Ambulance"},{"Leviathan"},{"Moonbeam"},{"Esperanto"},{"Taxi"},{"Washington"},
	{"Bobcat"},{"Mr Whoopee"},{"BF Injection"},{"Hunter"},{"Premier"},{"Enforcer"},{"Securicar"},
	{"Banshee"},{"Predator"},{"Bus"},{"Rhino"},{"Barracks"},{"Hotknife"},{"Trailer 1"},{"Previon"},
	{"Coach"},{"Cabbie"},{"Stallion"},{"Rumpo"},{"RC Bandit"},{"Romero"},{"Packer"},{"Monster"},
	{"Admiral"},{"Squalo"},{"Seasparrow"},{"Pizzaboy"},{"Tram"},{"Trailer 2"},{"Turismo"},
	{"Speeder"},{"Reefer"},{"Tropic"},{"Flatbed"},{"Yankee"},{"Caddy"},{"Solair"},{"Berkley's RC Van"},
	{"Skimmer"},{"PCJ-600"},{"Faggio"},{"Freeway"},{"RC Baron"},{"RC Raider"},{"Glendale"},{"Oceanic"},
	{"Sanchez"},{"Sparrow"},{"Patriot"},{"Quad"},{"Coastguard"},{"Dinghy"},{"Hermes"},{"Sabre"},
	{"Rustler"},{"ZR-350"},{"Walton"},{"Regina"},{"Comet"},{"BMX"},{"Burrito"},{"Camper"},{"Marquis"},
	{"Baggage"},{"Dozer"},{"Maverick"},{"News Chopper"},{"Rancher"},{"FBI Rancher"},{"Virgo"},{"Greenwood"},
	{"Jetmax"},{"Hotring"},{"Sandking"},{"Blista Compact"},{"Police Maverick"},{"Boxville"},{"Benson"},
	{"Mesa"},{"RC Goblin"},{"Hotring Racer A"},{"Hotring Racer B"},{"Bloodring Banger"},{"Rancher"},
	{"Super GT"},{"Elegant"},{"Journey"},{"Bike"},{"Mountain Bike"},{"Beagle"},{"Cropdust"},{"Stunt"},
	{"Tanker"}, {"Roadtrain"},{"Nebula"},{"Majestic"},{"Buccaneer"},{"Shamal"},{"Hydra"},{"FCR-900"},
	{"NRG-500"},{"HPV1000"},{"Cement Truck"},{"Tow Truck"},{"Fortune"},{"Cadrona"},{"FBI Truck"},
	{"Willard"},{"Forklift"},{"Tractor"},{"Combine"},{"Feltzer"},{"Remington"},{"Slamvan"},
	{"Blade"},{"Freight"},{"Streak"},{"Vortex"},{"Vincent"},{"Bullet"},{"Clover"},{"Sadler"},
	{"Firetruck LA"},{"Hustler"},{"Intruder"},{"Primo"},{"Cargobob"},{"Tampa"},{"Sunrise"},{"Merit"},
	{"Utility"},{"Nevada"},{"Yosemite"},{"Windsor"},{"Monster A"},{"Monster B"},{"Uranus"},{"Jester"},
	{"Sultan"},{"Stratum"},{"Elegy"},{"Raindance"},{"RC Tiger"},{"Flash"},{"Tahoma"},{"Savanna"},
	{"Bandito"},{"Freight Flat"},{"Streak Carriage"},{"Kart"},{"Mower"},{"Duneride"},{"Sweeper"},
	{"Broadway"},{"Tornado"},{"AT-400"},{"DFT-30"},{"Huntley"},{"Stafford"},{"BF-400"},{"Newsvan"},
	{"Tug"},{"Trailer 3"},{"Emperor"},{"Wayfarer"},{"Euros"},{"Hotdog"},{"Club"},{"Freight Carriage"},
	{"Trailer 3"},{"Andromada"},{"Dodo"},{"RC Cam"},{"Launch"},{"Police Car (LSPD)"},{"Police Car (SFPD)"},
	{"Police Car (LVPD)"},{"Police Ranger"},{"Picador"},{"S.W.A.T. Van"},{"Alpha"},{"Phoenix"},{"Glendale"},
	{"Sadler"},{"Luggage Trailer A"},{"Luggage Trailer B"},{"Stair Trailer"},{"Boxville"},{"Farm Plow"},{"Utility Trailer"}
};

enum e_AIRSTRIKE
{
	asPlaneObject,
	asObject[3],
	asLastStrike,
	Float:asPosX,
	Float:asPosY,
	Float:asPosZ,
	bool:asCalled
};
new pAirstrike[MAX_PLAYERS][e_AIRSTRIKE];

enum e_CAREPACK
{
	cpPlaneObject,
	cpObject,
	cpLastDrop,
	Float:cpPosX,
	Float:cpPosY,
	Float:cpPosZ,
	Text3D:cpLabel,
	bool:cpCalled
};
new pCarepack[MAX_PLAYERS][e_CAREPACK];

enum e_STATS
{
	userAdmin,
	bool:userPremium,
	userWarnings,
	userKills,
	userDeaths,
	userZones,
	userHeadshots,
	userJailTime,
	userMuteTime,
	userVehicle,
	userLastPM,
	bool:userNoPM,
	userIdx,
	bool:userGod,
	bool:userGodCar,
	bool:userOnDuty
};
new pStats[MAX_PLAYERS][e_STATS];

#define MAX_REPORTS (10)
enum e_REPORT
{
	rAgainst[MAX_PLAYER_NAME],
	rAgainstId,
	rBy[MAX_PLAYER_NAME],
	rById,
	rReason[100],
	rTime[15],
	bool:rChecked
};
new gReport[MAX_REPORTS][e_REPORT];

new const Float:gAdminSpawn[][4] =
{
	{1435.8024,2662.3647,11.3926,1.1650}, //  Northern train station
	{1457.4762,2773.4868,10.8203,272.2754}, //  Northern golf club
	{2101.4192,2678.7874,10.8130,92.0607}, //  Northern near railway line
	{1951.1090,2660.3877,10.8203,180.8461}, //  Northern house 2
	{1666.6949,2604.9861,10.8203,179.8495}, //  Northern house 3
	{1860.9672,1030.2910,10.8203,271.6988}, //  Behind 4 Dragons
	{1673.2345,1316.1067,10.8203,177.7294}, //  Airport carpark
	{1412.6187,2000.0596,14.7396,271.3568} //  South baseball stadium houses
};

#define MAX_DROPS (1000)

new gDropTimer[MAX_DROPS];
new gDropObject[MAX_DROPS];
new gDropWeaponid[MAX_DROPS];
new gDropAmount[MAX_DROPS];
new gDropAreaid[MAX_DROPS];

new Text3D:pDonorLabel[MAX_PLAYERS];

new const gWeather[] =
{
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
	11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	23, 24, 25, 26, 27, 28, 29, 30,
	32, 34, 35, 37, 38, 40, 41, 42
};
new const gTime[] =
{
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
	11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 23
};
new gTimeGap;
new gTimeIdx;
new gServerTime;
new gServerWeather;

new bool:pPremiumSupply[MAX_PLAYERS];

public OnGameModeInit()
{
	CA_Init();

	SetWorldTime(7);
	SetWeather(10);

    EnableVehicleFriendlyFire();
    DisableInteriorEnterExits();
	UsePlayerPedAnims();
	EnableTirePopping(1);
	AllowInteriorWeapons(1);

	yoursql_open("Server.db");

	yoursql_verify_table(SQL:0, "users");
	yoursql_verify_column(SQL:0, "users/name", SQL_STRING);
	yoursql_verify_column(SQL:0, "users/password", SQL_STRING);
	yoursql_verify_column(SQL:0, "users/email", SQL_STRING);
	yoursql_verify_column(SQL:0, "users/ip", SQL_STRING);
	yoursql_verify_column(SQL:0, "users/register_on", SQL_STRING);
	yoursql_verify_column(SQL:0, "users/auto_login", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "users/kills", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "users/deaths", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "users/score", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "users/money", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "users/zones", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "users/headshots", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "users/admin", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "users/vip", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "users/weapon1", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "users/weapon2", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "users/weapon3", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "users/hours", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "users/minutes", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "users/seconds", SQL_NUMBER);

	yoursql_verify_table(SQL:0, "bans");
	yoursql_verify_column(SQL:0, "bans/name", SQL_STRING);
	yoursql_verify_column(SQL:0, "bans/ip", SQL_STRING);
	yoursql_verify_column(SQL:0, "bans/admin_name", SQL_STRING);
	yoursql_verify_column(SQL:0, "bans/reason", SQL_STRING);
	yoursql_verify_column(SQL:0, "bans/date", SQL_STRING);
	yoursql_verify_column(SQL:0, "bans/type", SQL_NUMBER);
	yoursql_verify_column(SQL:0, "bans/expire", SQL_NUMBER);

	gServerTimer = SetTimer("OnServerUpdate", 30000, true);

	txtNotify = TextDrawCreate(1.000000, 441.000000, gNotifications[0]);
	TextDrawBackgroundColor(txtNotify, 255);
	TextDrawFont(txtNotify, 2);
	TextDrawLetterSize(txtNotify, 0.160000, 0.699999);
	TextDrawColor(txtNotify, -1);
	TextDrawSetOutline(txtNotify, 1);
	TextDrawSetProportional(txtNotify, 1);
	TextDrawUseBox(txtNotify, 1);
	TextDrawBoxColor(txtNotify, 84215240);
	TextDrawTextSize(txtNotify, 650.000000, 0.000000);
	TextDrawSetSelectable(txtNotify, 0);

    txtBase[0] = TextDrawCreate(0.000000, 1.000000, "box");
	TextDrawBackgroundColor(txtBase[0], 255);
	TextDrawFont(txtBase[0], 1);
	TextDrawLetterSize(txtBase[0], 0.000000, 12.000000);
	TextDrawColor(txtBase[0], -1);
	TextDrawSetOutline(txtBase[0], 0);
	TextDrawSetProportional(txtBase[0], 1);
	TextDrawSetShadow(txtBase[0], 1);
	TextDrawUseBox(txtBase[0], 1);
	TextDrawBoxColor(txtBase[0], 168430335);
	TextDrawTextSize(txtBase[0], 660.000000, 0.000000);
	TextDrawSetSelectable(txtBase[0], 0);

	txtBase[1] = TextDrawCreate(0.000000, 112.000000, "line");
	TextDrawBackgroundColor(txtBase[1], 255);
	TextDrawFont(txtBase[1], 1);
	TextDrawLetterSize(txtBase[1], 0.000000, -0.200000);
	TextDrawColor(txtBase[1], -1);
	TextDrawSetOutline(txtBase[1], 0);
	TextDrawSetProportional(txtBase[1], 1);
	TextDrawSetShadow(txtBase[1], 1);
	TextDrawUseBox(txtBase[1], 1);
	TextDrawBoxColor(txtBase[1], 150);
	TextDrawTextSize(txtBase[1], 660.000000, 0.000000);
	TextDrawSetSelectable(txtBase[1], 0);

	txtBase[2] = TextDrawCreate(0.000000, 339.000000, "box");
	TextDrawBackgroundColor(txtBase[2], 255);
	TextDrawFont(txtBase[2], 1);
	TextDrawLetterSize(txtBase[2], 0.000000, 12.000000);
	TextDrawColor(txtBase[2], -1);
	TextDrawSetOutline(txtBase[2], 0);
	TextDrawSetProportional(txtBase[2], 1);
	TextDrawSetShadow(txtBase[2], 1);
	TextDrawUseBox(txtBase[2], 1);
	TextDrawBoxColor(txtBase[2], 168430335);
	TextDrawTextSize(txtBase[2], 660.000000, 0.000000);
	TextDrawSetSelectable(txtBase[2], 0);

	txtBase[3] = TextDrawCreate(0.000000, 337.000000, "line");
	TextDrawBackgroundColor(txtBase[3], 255);
	TextDrawFont(txtBase[3], 1);
	TextDrawLetterSize(txtBase[3], 0.000000, -0.200000);
	TextDrawColor(txtBase[3], -1);
	TextDrawSetOutline(txtBase[3], 0);
	TextDrawSetProportional(txtBase[3], 1);
	TextDrawSetShadow(txtBase[3], 1);
	TextDrawUseBox(txtBase[3], 1);
	TextDrawBoxColor(txtBase[3], 150);
	TextDrawTextSize(txtBase[3], 660.000000, 0.000000);
	TextDrawSetSelectable(txtBase[3], 0);

	txtConnect[0] = TextDrawCreate(312.000000, 302.000000, "WORLD WAR GAMING");
	TextDrawAlignment(txtConnect[0], 2);
	TextDrawBackgroundColor(txtConnect[0], 255);
	TextDrawFont(txtConnect[0], 1);
	TextDrawLetterSize(txtConnect[0], 0.259999, 1.599999);
	TextDrawColor(txtConnect[0], 1690894335);
	TextDrawSetOutline(txtConnect[0], 1);
	TextDrawSetProportional(txtConnect[0], 1);
	TextDrawSetSelectable(txtConnect[0], 0);

	txtConnect[1] = TextDrawCreate(362.000000, 307.000000, "v"#BUILD"");
	TextDrawBackgroundColor(txtConnect[1], 255);
	TextDrawFont(txtConnect[1], 1);
	TextDrawLetterSize(txtConnect[1], 0.170000, 0.899999);
	TextDrawColor(txtConnect[1], -16776961);
	TextDrawSetOutline(txtConnect[1], 1);
	TextDrawSetProportional(txtConnect[1], 1);
	TextDrawSetSelectable(txtConnect[1], 0);

	txtConnect[2] = TextDrawCreate(316.000000, 316.000000, "Last update: ~w~"BUILD_DATE"");
	TextDrawAlignment(txtConnect[2], 2);
	TextDrawBackgroundColor(txtConnect[2], 255);
	TextDrawFont(txtConnect[2], 1);
	TextDrawLetterSize(txtConnect[2], 0.160000, 0.899999);
	TextDrawColor(txtConnect[2], -1);
	TextDrawSetOutline(txtConnect[2], 1);
	TextDrawSetProportional(txtConnect[2], 1);
	TextDrawSetSelectable(txtConnect[2], 0);

	txtConnect[3] = TextDrawCreate(316.000000, 323.000000, "Teamdeathmatch/Deathmatch/Survival/War");
	TextDrawAlignment(txtConnect[3], 2);
	TextDrawBackgroundColor(txtConnect[3], 255);
	TextDrawFont(txtConnect[3], 1);
	TextDrawLetterSize(txtConnect[3], 0.160000, 0.899999);
	TextDrawColor(txtConnect[3], -1);
	TextDrawSetOutline(txtConnect[3], 1);
	TextDrawSetProportional(txtConnect[3], 1);
	TextDrawSetSelectable(txtConnect[3], 0);

	txtTeam[0] = TextDrawCreate(10.000000, 150.000000, "Select a team:");
	TextDrawBackgroundColor(txtTeam[0], 255);
	TextDrawFont(txtTeam[0], 1);
	TextDrawLetterSize(txtTeam[0], 0.170000, 1.000000);
	TextDrawColor(txtTeam[0], -1);
	TextDrawSetOutline(txtTeam[0], 1);
	TextDrawSetProportional(txtTeam[0], 1);
	TextDrawSetSelectable(txtTeam[0], 0);

	txtTeam[1] = TextDrawCreate(10.000000, 160.000000, "LD_PLAN:tvbase");
	TextDrawBackgroundColor(txtTeam[1], 255);
	TextDrawFont(txtTeam[1], 4);
	TextDrawLetterSize(txtTeam[1], 0.000000, 2.000000);
	TextDrawColor(txtTeam[1], -1768515946);
	TextDrawSetOutline(txtTeam[1], 1);
	TextDrawSetProportional(txtTeam[1], 1);
	TextDrawUseBox(txtTeam[1], 1);
	TextDrawBoxColor(txtTeam[1], 0);
	TextDrawTextSize(txtTeam[1], 51.000000, 22.000000);
	TextDrawSetSelectable(txtTeam[1], 1);

	txtTeam[2] = TextDrawCreate(11.000000, 161.000000, "team0");
	TextDrawBackgroundColor(txtTeam[2], SET_ALPHA(gTeam[0][teamColor], 150));
	TextDrawFont(txtTeam[2], 5);
	TextDrawLetterSize(txtTeam[2], 0.000000, 2.000000);
	TextDrawColor(txtTeam[2], -1);
	TextDrawSetOutline(txtTeam[2], 1);
	TextDrawSetProportional(txtTeam[2], 1);
	TextDrawUseBox(txtTeam[2], 1);
	TextDrawBoxColor(txtTeam[2], 0);
	TextDrawTextSize(txtTeam[2], 20.000000, 20.000000);
	TextDrawSetPreviewModel(txtTeam[2], gTeam[0][teamSkin]);
	TextDrawSetPreviewRot(txtTeam[2], 0.000000, 0.000000, 0.000000, 1.000000);
	TextDrawSetSelectable(txtTeam[2], 1);

	txtTeam[3] = TextDrawCreate(33.000000, 161.000000, "UNITED STATES~n~~y~~h~~h~Players: 15");
	TextDrawBackgroundColor(txtTeam[3], 0);
	TextDrawFont(txtTeam[3], 1);
	TextDrawLetterSize(txtTeam[3], 0.109999, 0.599999);
	TextDrawColor(txtTeam[3], -1);
	TextDrawSetOutline(txtTeam[3], 0);
	TextDrawSetProportional(txtTeam[3], 1);
	TextDrawSetShadow(txtTeam[3], 1);
	TextDrawSetSelectable(txtTeam[3], 0);

	txtTeam[4] = TextDrawCreate(62.000000, 160.000000, "LD_PLAN:tvbase");
	TextDrawBackgroundColor(txtTeam[4], 255);
	TextDrawFont(txtTeam[4], 4);
	TextDrawLetterSize(txtTeam[4], 0.000000, 2.000000);
	TextDrawColor(txtTeam[4], 84545430);
	TextDrawSetOutline(txtTeam[4], 1);
	TextDrawSetProportional(txtTeam[4], 1);
	TextDrawUseBox(txtTeam[4], 1);
	TextDrawBoxColor(txtTeam[4], 0);
	TextDrawTextSize(txtTeam[4], 51.000000, 22.000000);
	TextDrawSetSelectable(txtTeam[4], 1);

	txtTeam[5] = TextDrawCreate(63.000000, 161.000000, "team1");
	TextDrawBackgroundColor(txtTeam[5], SET_ALPHA(gTeam[1][teamColor], 150));
	TextDrawFont(txtTeam[5], 5);
	TextDrawLetterSize(txtTeam[5], 0.000000, 2.000000);
	TextDrawColor(txtTeam[5], -1);
	TextDrawSetOutline(txtTeam[5], 1);
	TextDrawSetProportional(txtTeam[5], 1);
	TextDrawUseBox(txtTeam[5], 1);
	TextDrawBoxColor(txtTeam[5], 0);
	TextDrawTextSize(txtTeam[5], 20.000000, 20.000000);
	TextDrawSetPreviewModel(txtTeam[5], gTeam[1][teamSkin]);
	TextDrawSetPreviewRot(txtTeam[5], 0.000000, 0.000000, 0.000000, 1.000000);
	TextDrawSetSelectable(txtTeam[5], 1);

	txtTeam[6] = TextDrawCreate(84.000000, 161.000000, "UNITED STATES~n~~y~~h~~h~Players: 15");
	TextDrawBackgroundColor(txtTeam[6], 0);
	TextDrawFont(txtTeam[6], 1);
	TextDrawLetterSize(txtTeam[6], 0.109999, 0.599999);
	TextDrawColor(txtTeam[6], -1);
	TextDrawSetOutline(txtTeam[6], 0);
	TextDrawSetProportional(txtTeam[6], 1);
	TextDrawSetShadow(txtTeam[6], 1);
	TextDrawSetSelectable(txtTeam[6], 0);

	txtTeam[7] = TextDrawCreate(114.000000, 160.000000, "LD_PLAN:tvbase");
	TextDrawBackgroundColor(txtTeam[7], 255);
	TextDrawFont(txtTeam[7], 4);
	TextDrawLetterSize(txtTeam[7], 0.000000, 2.000000);
	TextDrawColor(txtTeam[7], 84545430);
	TextDrawSetOutline(txtTeam[7], 1);
	TextDrawSetProportional(txtTeam[7], 1);
	TextDrawUseBox(txtTeam[7], 1);
	TextDrawBoxColor(txtTeam[7], 0);
	TextDrawTextSize(txtTeam[7], 51.000000, 22.000000);
	TextDrawSetSelectable(txtTeam[7], 1);

	txtTeam[8] = TextDrawCreate(115.000000, 161.000000, "team2");
	TextDrawBackgroundColor(txtTeam[8], SET_ALPHA(gTeam[2][teamColor], 150));
	TextDrawFont(txtTeam[8], 5);
	TextDrawLetterSize(txtTeam[8], 0.000000, 2.000000);
	TextDrawColor(txtTeam[8], -1);
	TextDrawSetOutline(txtTeam[8], 1);
	TextDrawSetProportional(txtTeam[8], 1);
	TextDrawUseBox(txtTeam[8], 1);
	TextDrawBoxColor(txtTeam[8], 0);
	TextDrawTextSize(txtTeam[8], 20.000000, 20.000000);
	TextDrawSetPreviewModel(txtTeam[8], gTeam[2][teamSkin]);
	TextDrawSetPreviewRot(txtTeam[8], 0.000000, 0.000000, 0.000000, 1.000000);
	TextDrawSetSelectable(txtTeam[8], 1);

	txtTeam[9] = TextDrawCreate(137.000000, 161.000000, "UNITED STATES~n~~y~~h~~h~Players: 15");
	TextDrawBackgroundColor(txtTeam[9], 0);
	TextDrawFont(txtTeam[9], 1);
	TextDrawLetterSize(txtTeam[9], 0.109999, 0.599999);
	TextDrawColor(txtTeam[9], -1);
	TextDrawSetOutline(txtTeam[9], 0);
	TextDrawSetProportional(txtTeam[9], 1);
	TextDrawSetShadow(txtTeam[9], 1);
	TextDrawSetSelectable(txtTeam[9], 0);

	txtTeam[10] = TextDrawCreate(166.000000, 160.000000, "LD_PLAN:tvbase");
	TextDrawBackgroundColor(txtTeam[10], 255);
	TextDrawFont(txtTeam[10], 4);
	TextDrawLetterSize(txtTeam[10], 0.000000, 2.000000);
	TextDrawColor(txtTeam[10], 84545430);
	TextDrawSetOutline(txtTeam[10], 1);
	TextDrawSetProportional(txtTeam[10], 1);
	TextDrawUseBox(txtTeam[10], 1);
	TextDrawBoxColor(txtTeam[10], 0);
	TextDrawTextSize(txtTeam[10], 51.000000, 22.000000);
	TextDrawSetSelectable(txtTeam[10], 1);

	txtTeam[11] = TextDrawCreate(167.000000, 161.000000, "team3");
	TextDrawBackgroundColor(txtTeam[11], SET_ALPHA(gTeam[3][teamColor], 150));
	TextDrawFont(txtTeam[11], 5);
	TextDrawLetterSize(txtTeam[11], 0.000000, 2.000000);
	TextDrawColor(txtTeam[11], -1);
	TextDrawSetOutline(txtTeam[11], 1);
	TextDrawSetProportional(txtTeam[11], 1);
	TextDrawUseBox(txtTeam[11], 1);
	TextDrawBoxColor(txtTeam[11], 0);
	TextDrawTextSize(txtTeam[11], 20.000000, 20.000000);
	TextDrawSetPreviewModel(txtTeam[11], gTeam[3][teamSkin]);
	TextDrawSetPreviewRot(txtTeam[11], 0.000000, 0.000000, 0.000000, 1.000000);
	TextDrawSetSelectable(txtTeam[11], 1);

	txtTeam[12] = TextDrawCreate(189.000000, 161.000000, "UNITED STATES~n~~y~~h~~h~Players: 15");
	TextDrawBackgroundColor(txtTeam[12], 0);
	TextDrawFont(txtTeam[12], 1);
	TextDrawLetterSize(txtTeam[12], 0.109999, 0.599999);
	TextDrawColor(txtTeam[12], -1);
	TextDrawSetOutline(txtTeam[12], 0);
	TextDrawSetProportional(txtTeam[12], 1);
	TextDrawSetShadow(txtTeam[12], 1);
	TextDrawSetSelectable(txtTeam[12], 0);

	txtTeam[13] = TextDrawCreate(10.000000, 183.000000, "LD_PLAN:tvbase");
	TextDrawBackgroundColor(txtTeam[13], 255);
	TextDrawFont(txtTeam[13], 4);
	TextDrawLetterSize(txtTeam[13], 0.000000, 2.000000);
	TextDrawColor(txtTeam[13], 84545430);
	TextDrawSetOutline(txtTeam[13], 1);
	TextDrawSetProportional(txtTeam[13], 1);
	TextDrawUseBox(txtTeam[13], 1);
	TextDrawBoxColor(txtTeam[13], 0);
	TextDrawTextSize(txtTeam[13], 51.000000, 22.000000);
	TextDrawSetSelectable(txtTeam[13], 1);

	txtTeam[14] = TextDrawCreate(11.000000, 184.000000, "team4");
	TextDrawBackgroundColor(txtTeam[14], SET_ALPHA(gTeam[4][teamColor], 150));
	TextDrawFont(txtTeam[14], 5);
	TextDrawLetterSize(txtTeam[14], 0.000000, 2.000000);
	TextDrawColor(txtTeam[14], -1);
	TextDrawSetOutline(txtTeam[14], 1);
	TextDrawSetProportional(txtTeam[14], 1);
	TextDrawUseBox(txtTeam[14], 1);
	TextDrawBoxColor(txtTeam[14], 0);
	TextDrawTextSize(txtTeam[14], 20.000000, 20.000000);
	TextDrawSetPreviewModel(txtTeam[14], gTeam[4][teamSkin]);
	TextDrawSetPreviewRot(txtTeam[14], 0.000000, 0.000000, 0.000000, 1.000000);
	TextDrawSetSelectable(txtTeam[14], 1);

	txtTeam[15] = TextDrawCreate(33.000000, 183.000000, "UNITED STATES~n~~y~~h~~h~Players: 15");
	TextDrawBackgroundColor(txtTeam[15], 0);
	TextDrawFont(txtTeam[15], 1);
	TextDrawLetterSize(txtTeam[15], 0.109999, 0.599999);
	TextDrawColor(txtTeam[15], -1);
	TextDrawSetOutline(txtTeam[15], 0);
	TextDrawSetProportional(txtTeam[15], 1);
	TextDrawSetShadow(txtTeam[15], 1);
	TextDrawSetSelectable(txtTeam[15], 0);

	txtTeam[16] = TextDrawCreate(62.000000, 183.000000, "LD_PLAN:tvbase");
	TextDrawBackgroundColor(txtTeam[16], 255);
	TextDrawFont(txtTeam[16], 4);
	TextDrawLetterSize(txtTeam[16], 0.000000, 2.000000);
	TextDrawColor(txtTeam[16], 84545430);
	TextDrawSetOutline(txtTeam[16], 1);
	TextDrawSetProportional(txtTeam[16], 1);
	TextDrawUseBox(txtTeam[16], 1);
	TextDrawBoxColor(txtTeam[16], 0);
	TextDrawTextSize(txtTeam[16], 51.000000, 22.000000);
	TextDrawSetSelectable(txtTeam[16], 1);

	txtTeam[17] = TextDrawCreate(63.000000, 184.000000, "team5");
	TextDrawBackgroundColor(txtTeam[17], SET_ALPHA(gTeam[5][teamColor], 150));
	TextDrawFont(txtTeam[17], 5);
	TextDrawLetterSize(txtTeam[17], 0.000000, 2.000000);
	TextDrawColor(txtTeam[17], -1);
	TextDrawSetOutline(txtTeam[17], 1);
	TextDrawSetProportional(txtTeam[17], 1);
	TextDrawUseBox(txtTeam[17], 1);
	TextDrawBoxColor(txtTeam[17], 0);
	TextDrawTextSize(txtTeam[17], 20.000000, 20.000000);
	TextDrawSetPreviewModel(txtTeam[17], gTeam[5][teamSkin]);
	TextDrawSetPreviewRot(txtTeam[17], 0.000000, 0.000000, 0.000000, 1.000000);
	TextDrawSetSelectable(txtTeam[17], 1);

	txtTeam[18] = TextDrawCreate(84.000000, 183.000000, "UNITED STATES~n~~y~~h~~h~Players: 15");
	TextDrawBackgroundColor(txtTeam[18], 0);
	TextDrawFont(txtTeam[18], 1);
	TextDrawLetterSize(txtTeam[18], 0.109999, 0.599999);
	TextDrawColor(txtTeam[18], -1);
	TextDrawSetOutline(txtTeam[18], 0);
	TextDrawSetProportional(txtTeam[18], 1);
	TextDrawSetShadow(txtTeam[18], 1);
	TextDrawSetSelectable(txtTeam[18], 0);

	txtTeam[19] = TextDrawCreate(114.000000, 183.000000, "LD_PLAN:tvbase");
	TextDrawBackgroundColor(txtTeam[19], 255);
	TextDrawFont(txtTeam[19], 4);
	TextDrawLetterSize(txtTeam[19], 0.000000, 2.000000);
	TextDrawColor(txtTeam[19], 84545430);
	TextDrawSetOutline(txtTeam[19], 1);
	TextDrawSetProportional(txtTeam[19], 1);
	TextDrawUseBox(txtTeam[19], 1);
	TextDrawBoxColor(txtTeam[19], 0);
	TextDrawTextSize(txtTeam[19], 51.000000, 22.000000);
	TextDrawSetSelectable(txtTeam[19], 1);

	txtTeam[20] = TextDrawCreate(115.000000, 184.000000, "team6");
	TextDrawBackgroundColor(txtTeam[20], SET_ALPHA(gTeam[6][teamColor], 150));
	TextDrawFont(txtTeam[20], 5);
	TextDrawLetterSize(txtTeam[20], 0.000000, 2.000000);
	TextDrawColor(txtTeam[20], -1);
	TextDrawSetOutline(txtTeam[20], 1);
	TextDrawSetProportional(txtTeam[20], 1);
	TextDrawUseBox(txtTeam[20], 1);
	TextDrawBoxColor(txtTeam[20], 0);
	TextDrawTextSize(txtTeam[20], 20.000000, 20.000000);
	TextDrawSetPreviewModel(txtTeam[20], gTeam[6][teamSkin]);
	TextDrawSetPreviewRot(txtTeam[20], 0.000000, 0.000000, 0.000000, 1.000000);
	TextDrawSetSelectable(txtTeam[20], 1);

	txtTeam[21] = TextDrawCreate(136.000000, 183.000000, "UNITED STATES~n~~y~~h~~h~Players: 15");
	TextDrawBackgroundColor(txtTeam[21], 0);
	TextDrawFont(txtTeam[21], 1);
	TextDrawLetterSize(txtTeam[21], 0.109999, 0.599999);
	TextDrawColor(txtTeam[21], -1);
	TextDrawSetOutline(txtTeam[21], 0);
	TextDrawSetProportional(txtTeam[21], 1);
	TextDrawSetShadow(txtTeam[21], 1);
	TextDrawSetSelectable(txtTeam[21], 0);

	txtClass[0] = TextDrawCreate(10.000000, 240.000000, "Select a class:");
	TextDrawBackgroundColor(txtClass[0], 255);
	TextDrawFont(txtClass[0], 1);
	TextDrawLetterSize(txtClass[0], 0.170000, 1.000000);
	TextDrawColor(txtClass[0], -1);
	TextDrawSetOutline(txtClass[0], 1);
	TextDrawSetProportional(txtClass[0], 1);
	TextDrawSetSelectable(txtClass[0], 0);

	txtClass[1] = TextDrawCreate(10.000000, 250.000000, "LD_PLAN:tvbase");
	TextDrawBackgroundColor(txtClass[1], 255);
	TextDrawFont(txtClass[1], 4);
	TextDrawLetterSize(txtClass[1], 0.000000, 2.000000);
	TextDrawColor(txtClass[1], -1768515946);
	TextDrawSetOutline(txtClass[1], 1);
	TextDrawSetProportional(txtClass[1], 1);
	TextDrawUseBox(txtClass[1], 1);
	TextDrawBoxColor(txtClass[1], 0);
	TextDrawTextSize(txtClass[1], 71.000000, 22.000000);
	TextDrawSetSelectable(txtClass[1], 1);

	txtClass[2] = TextDrawCreate(11.000000, 251.000000, "class0");
	TextDrawBackgroundColor(txtClass[2], 150);
	TextDrawFont(txtClass[2], 5);
	TextDrawLetterSize(txtClass[2], 0.000000, 2.000000);
	TextDrawColor(txtClass[2], -1);
	TextDrawSetOutline(txtClass[2], 1);
	TextDrawSetProportional(txtClass[2], 1);
	TextDrawUseBox(txtClass[2], 1);
	TextDrawBoxColor(txtClass[2], 0);
	TextDrawTextSize(txtClass[2], 20.000000, 20.000000);
	TextDrawSetPreviewModel(txtClass[2], gClass[0][classModel]);
	TextDrawSetPreviewRot(txtClass[2], -20.000000, 0.000000, -50.000000, 1.000000);
	TextDrawSetSelectable(txtClass[2], 1);

	new str[150];
	format(str, sizeof(str), "%s~n~Rank %i+~n~Ability: /armour", gClass[0][className], gClass[0][classRank]);
	txtClass[3] = TextDrawCreate(33.000000, 250.000000, str);
	TextDrawBackgroundColor(txtClass[3], 0);
	TextDrawFont(txtClass[3], 1);
	TextDrawLetterSize(txtClass[3], 0.109999, 0.599999);
	TextDrawColor(txtClass[3], -1);
	TextDrawSetOutline(txtClass[3], 0);
	TextDrawSetProportional(txtClass[3], 1);
	TextDrawSetShadow(txtClass[3], 1);
	TextDrawUseBox(txtClass[3], 1);
	TextDrawBoxColor(txtClass[3], 0);
	TextDrawTextSize(txtClass[3], 80.000000, 0.000000);
	TextDrawSetSelectable(txtClass[3], 0);

	txtClass[4] = TextDrawCreate(82.000000, 250.000000, "LD_PLAN:tvbase");
	TextDrawBackgroundColor(txtClass[4], 255);
	TextDrawFont(txtClass[4], 4);
	TextDrawLetterSize(txtClass[4], 0.000000, 2.000000);
	TextDrawColor(txtClass[4], -1768515946);
	TextDrawSetOutline(txtClass[4], 1);
	TextDrawSetProportional(txtClass[4], 1);
	TextDrawUseBox(txtClass[4], 1);
	TextDrawBoxColor(txtClass[4], 0);
	TextDrawTextSize(txtClass[4], 71.000000, 22.000000);
	TextDrawSetSelectable(txtClass[4], 1);

	txtClass[5] = TextDrawCreate(83.000000, 251.000000, "class1");
	TextDrawBackgroundColor(txtClass[5], 150);
	TextDrawFont(txtClass[5], 5);
	TextDrawLetterSize(txtClass[5], 0.000000, 2.000000);
	TextDrawColor(txtClass[5], -1);
	TextDrawSetOutline(txtClass[5], 1);
	TextDrawSetProportional(txtClass[5], 1);
	TextDrawUseBox(txtClass[5], 1);
	TextDrawBoxColor(txtClass[5], 0);
	TextDrawTextSize(txtClass[5], 20.000000, 20.000000);
	TextDrawSetPreviewModel(txtClass[5], gClass[1][classModel]);
	TextDrawSetPreviewRot(txtClass[5], -20.000000, 0.000000, -50.000000, 1.000000);
	TextDrawSetSelectable(txtClass[5], 1);

	format(str, sizeof(str), "%s~n~Rank %i+~n~Ability: /heal, 3 medickits", gClass[1][className], gClass[1][classRank]);
	txtClass[6] = TextDrawCreate(104.000000, 250.000000, str);
	TextDrawBackgroundColor(txtClass[6], 0);
	TextDrawFont(txtClass[6], 1);
	TextDrawLetterSize(txtClass[6], 0.109999, 0.599999);
	TextDrawColor(txtClass[6], -1);
	TextDrawSetOutline(txtClass[6], 0);
	TextDrawSetProportional(txtClass[6], 1);
	TextDrawSetShadow(txtClass[6], 1);
	TextDrawUseBox(txtClass[6], 1);
	TextDrawBoxColor(txtClass[6], 0);
	TextDrawTextSize(txtClass[6], 150.000000, 0.000000);
	TextDrawSetSelectable(txtClass[6], 0);

	txtClass[7] = TextDrawCreate(154.000000, 250.000000, "LD_PLAN:tvbase");
	TextDrawBackgroundColor(txtClass[7], 255);
	TextDrawFont(txtClass[7], 4);
	TextDrawLetterSize(txtClass[7], 0.000000, 2.000000);
	TextDrawColor(txtClass[7], -1768515946);
	TextDrawSetOutline(txtClass[7], 1);
	TextDrawSetProportional(txtClass[7], 1);
	TextDrawUseBox(txtClass[7], 1);
	TextDrawBoxColor(txtClass[7], 0);
	TextDrawTextSize(txtClass[7], 71.000000, 22.000000);
	TextDrawSetSelectable(txtClass[7], 1);

	txtClass[8] = TextDrawCreate(155.000000, 251.000000, "class2");
	TextDrawBackgroundColor(txtClass[8], 150);
	TextDrawFont(txtClass[8], 5);
	TextDrawLetterSize(txtClass[8], 0.000000, 2.000000);
	TextDrawColor(txtClass[8], -1);
	TextDrawSetOutline(txtClass[8], 1);
	TextDrawSetProportional(txtClass[8], 1);
	TextDrawUseBox(txtClass[8], 1);
	TextDrawBoxColor(txtClass[8], 0);
	TextDrawTextSize(txtClass[8], 20.000000, 20.000000);
	TextDrawSetPreviewModel(txtClass[8], gClass[2][classModel]);
	TextDrawSetPreviewRot(txtClass[8], -20.000000, 0.000000, -50.000000, 1.000000);
	TextDrawSetSelectable(txtClass[8], 1);

	format(str, sizeof(str), "%s~n~Rank %i+~n~Ability: Invisible on map", gClass[2][className], gClass[2][classRank]);
	txtClass[9] = TextDrawCreate(176.000000, 250.000000, str);
	TextDrawBackgroundColor(txtClass[9], 0);
	TextDrawFont(txtClass[9], 1);
	TextDrawLetterSize(txtClass[9], 0.109999, 0.599999);
	TextDrawColor(txtClass[9], -1);
	TextDrawSetOutline(txtClass[9], 0);
	TextDrawSetProportional(txtClass[9], 1);
	TextDrawSetShadow(txtClass[9], 1);
	TextDrawUseBox(txtClass[9], 1);
	TextDrawBoxColor(txtClass[9], 0);
	TextDrawTextSize(txtClass[9], 223.000000, 0.000000);
	TextDrawSetSelectable(txtClass[9], 0);

	txtClass[10] = TextDrawCreate(10.000000, 273.000000, "LD_PLAN:tvbase");
	TextDrawBackgroundColor(txtClass[10], 255);
	TextDrawFont(txtClass[10], 4);
	TextDrawLetterSize(txtClass[10], 0.000000, 2.000000);
	TextDrawColor(txtClass[10], -1768515946);
	TextDrawSetOutline(txtClass[10], 1);
	TextDrawSetProportional(txtClass[10], 1);
	TextDrawUseBox(txtClass[10], 1);
	TextDrawBoxColor(txtClass[10], 0);
	TextDrawTextSize(txtClass[10], 71.000000, 22.000000);
	TextDrawSetSelectable(txtClass[10], 1);

	txtClass[11] = TextDrawCreate(11.000000, 274.000000, "class3");
	TextDrawBackgroundColor(txtClass[11], 150);
	TextDrawFont(txtClass[11], 5);
	TextDrawLetterSize(txtClass[11], 0.000000, 2.000000);
	TextDrawColor(txtClass[11], -1);
	TextDrawSetOutline(txtClass[11], 1);
	TextDrawSetProportional(txtClass[11], 1);
	TextDrawUseBox(txtClass[11], 1);
	TextDrawBoxColor(txtClass[11], 0);
	TextDrawTextSize(txtClass[11], 20.000000, 20.000000);
	TextDrawSetPreviewModel(txtClass[11], gClass[3][classModel]);
	TextDrawSetPreviewRot(txtClass[11], -20.000000, 0.000000, -50.000000, 1.000000);
	TextDrawSetSelectable(txtClass[11], 1);

	format(str, sizeof(str), "%s~n~Rank %i+~n~Ability: Drive rhino, /fix, /build", gClass[3][className], gClass[3][classRank]);
	txtClass[12] = TextDrawCreate(33.000000, 273.000000, str);
	TextDrawBackgroundColor(txtClass[12], 0);
	TextDrawFont(txtClass[12], 1);
	TextDrawLetterSize(txtClass[12], 0.109999, 0.599999);
	TextDrawColor(txtClass[12], -1);
	TextDrawSetOutline(txtClass[12], 0);
	TextDrawSetProportional(txtClass[12], 1);
	TextDrawSetShadow(txtClass[12], 1);
	TextDrawUseBox(txtClass[12], 1);
	TextDrawBoxColor(txtClass[12], 0);
	TextDrawTextSize(txtClass[12], 80.000000, 0.000000);
	TextDrawSetSelectable(txtClass[12], 0);

	txtClass[13] = TextDrawCreate(82.000000, 273.000000, "LD_PLAN:tvbase");
	TextDrawBackgroundColor(txtClass[13], 255);
	TextDrawFont(txtClass[13], 4);
	TextDrawLetterSize(txtClass[13], 0.000000, 2.000000);
	TextDrawColor(txtClass[13], -1768515946);
	TextDrawSetOutline(txtClass[13], 1);
	TextDrawSetProportional(txtClass[13], 1);
	TextDrawUseBox(txtClass[13], 1);
	TextDrawBoxColor(txtClass[13], 0);
	TextDrawTextSize(txtClass[13], 71.000000, 22.000000);
	TextDrawSetSelectable(txtClass[13], 1);

	txtClass[14] = TextDrawCreate(83.000000, 274.000000, "class4");
	TextDrawBackgroundColor(txtClass[14], 150);
	TextDrawFont(txtClass[14], 5);
	TextDrawLetterSize(txtClass[14], 0.000000, 2.000000);
	TextDrawColor(txtClass[14], -1);
	TextDrawSetOutline(txtClass[14], 1);
	TextDrawSetProportional(txtClass[14], 1);
	TextDrawUseBox(txtClass[14], 1);
	TextDrawBoxColor(txtClass[14], 0);
	TextDrawTextSize(txtClass[14], 20.000000, 20.000000);
	TextDrawSetPreviewModel(txtClass[14], gClass[4][classModel]);
	TextDrawSetPreviewRot(txtClass[14], -20.000000, 0.000000, -50.000000, 1.000000);
	TextDrawSetSelectable(txtClass[14], 1);

	format(str, sizeof(str), "%s~n~Rank %i+~n~Ability: Drive hunter and hydra", gClass[4][className], gClass[4][classRank]);
	txtClass[15] = TextDrawCreate(104.000000, 273.000000, str);
	TextDrawBackgroundColor(txtClass[15], 0);
	TextDrawFont(txtClass[15], 1);
	TextDrawLetterSize(txtClass[15], 0.109999, 0.599999);
	TextDrawColor(txtClass[15], -1);
	TextDrawSetOutline(txtClass[15], 0);
	TextDrawSetProportional(txtClass[15], 1);
	TextDrawSetShadow(txtClass[15], 1);
	TextDrawUseBox(txtClass[15], 1);
	TextDrawBoxColor(txtClass[15], 0);
	TextDrawTextSize(txtClass[15], 150.000000, 0.000000);
	TextDrawSetSelectable(txtClass[15], 0);

	txtClass[16] = TextDrawCreate(154.000000, 273.000000, "LD_PLAN:tvbase");
	TextDrawBackgroundColor(txtClass[16], 255);
	TextDrawFont(txtClass[16], 4);
	TextDrawLetterSize(txtClass[16], 0.000000, 2.000000);
	TextDrawColor(txtClass[16], -1768515946);
	TextDrawSetOutline(txtClass[16], 1);
	TextDrawSetProportional(txtClass[16], 1);
	TextDrawUseBox(txtClass[16], 1);
	TextDrawBoxColor(txtClass[16], 0);
	TextDrawTextSize(txtClass[16], 71.000000, 22.000000);
	TextDrawSetSelectable(txtClass[16], 1);

	txtClass[17] = TextDrawCreate(155.000000, 274.000000, "class5");
	TextDrawBackgroundColor(txtClass[17], 150);
	TextDrawFont(txtClass[17], 5);
	TextDrawLetterSize(txtClass[17], 0.000000, 2.000000);
	TextDrawColor(txtClass[17], -1);
	TextDrawSetOutline(txtClass[17], 1);
	TextDrawSetProportional(txtClass[17], 1);
	TextDrawUseBox(txtClass[17], 1);
	TextDrawBoxColor(txtClass[17], 0);
	TextDrawTextSize(txtClass[17], 20.000000, 20.000000);
	TextDrawSetPreviewModel(txtClass[17], gClass[5][classModel]);
	TextDrawSetPreviewRot(txtClass[17], -20.000000, 0.000000, -50.000000, 1.000000);
	TextDrawSetSelectable(txtClass[17], 1);

	format(str, sizeof(str), "%s~n~Rank %i+~n~Ability: /dis, /undis, /rob, Press '~k~~TOGGLE_SUBMISSIONS~' to stab (in vehicle)", gClass[5][className], gClass[5][classRank]);
	txtClass[18] = TextDrawCreate(176.000000, 273.000000, str);
	TextDrawBackgroundColor(txtClass[18], 0);
	TextDrawFont(txtClass[18], 1);
	TextDrawLetterSize(txtClass[18], 0.109999, 0.599999);
	TextDrawColor(txtClass[18], -1);
	TextDrawSetOutline(txtClass[18], 0);
	TextDrawSetProportional(txtClass[18], 1);
	TextDrawSetShadow(txtClass[18], 1);
	TextDrawUseBox(txtClass[18], 1);
	TextDrawBoxColor(txtClass[18], 0);
	TextDrawTextSize(txtClass[18], 223.000000, 0.000000);
	TextDrawSetSelectable(txtClass[18], 0);

	txtMenu[0] = TextDrawCreate(160.000000, 131.000000, "box");
	TextDrawBackgroundColor(txtMenu[0], 255);
	TextDrawFont(txtMenu[0], 1);
	TextDrawLetterSize(txtMenu[0], 0.000000, 22.799999);
	TextDrawColor(txtMenu[0], -1);
	TextDrawSetOutline(txtMenu[0], 0);
	TextDrawSetProportional(txtMenu[0], 1);
	TextDrawSetShadow(txtMenu[0], 1);
	TextDrawUseBox(txtMenu[0], 1);
	TextDrawBoxColor(txtMenu[0], 252645320);
	TextDrawTextSize(txtMenu[0], 467.000000, 10.000000);
	TextDrawSetSelectable(txtMenu[0], 0);

	new count;
	new Float:base[2] = {161.000000, 131.000000};
	for (new i; i < 6 * 4; i++)
	{
		txtMenu[i + 1] = TextDrawCreate(base[0], base[1], "model");
		TextDrawBackgroundColor(txtMenu[i + 1], 338181220);
		TextDrawFont(txtMenu[i + 1], 5);
		TextDrawLetterSize(txtMenu[i + 1], 0.500000, 1.000000);
		TextDrawColor(txtMenu[i + 1], -1);
		TextDrawSetOutline(txtMenu[i + 1], 0);
		TextDrawSetProportional(txtMenu[i + 1], 1);
		TextDrawSetShadow(txtMenu[i + 1], 1);
		TextDrawUseBox(txtMenu[i + 1], 1);
		TextDrawBoxColor(txtMenu[i + 1], 0);
		TextDrawTextSize(txtMenu[i + 1], 50.000000, 47.000000);
		TextDrawSetPreviewModel(txtMenu[i + 1], 0);
		TextDrawSetPreviewRot(txtMenu[i + 1], 0.000000, 0.000000, -50.000000, 1.000000);
		TextDrawSetSelectable(txtMenu[i + 1], 1);

		base[0] += 51.000000;
        count += 1;
        if (count == 6)
		{
			base[0] = 161.000000;
	        base[1] += 48.000000;

            count = 0;
		}
	}

	new label[100];
	for (new i; i < MAX_TEAMS; i++)
	{
	    gTeam[i][teamBaseId] = GangZoneCreate(gTeam[i][teamBase][0], gTeam[i][teamBase][1], gTeam[i][teamBase][2], gTeam[i][teamBase][3], SET_ALPHA(gTeam[i][teamColor], 100), COLOR_RED, 2.0);
    	gTeam[i][teamProtoId] = AddStaticVehicleEx(428, gTeam[i][teamProto][0], gTeam[i][teamProto][1], gTeam[i][teamProto][2], gTeam[i][teamProto][3], gTeam[i][teamColor], gTeam[i][teamColor], 60);
		SetVehicleVirtualWorld(gTeam[i][teamProtoId], 0);
		gTeam[i][teamProtoAttacker] = INVALID_PLAYER_ID;

        label[0] = EOS;
		strcat(label, gTeam[i][teamName]);
		strcat(label, "'s Prototype\n"WHITE"(/PROTOTYPE)");
   		CreateDynamic3DTextLabel(label, gTeam[i][teamColor], 0.0, 0.0, 0.0, 50.0, _, gTeam[i][teamProtoId], _, 0);
	}

	for (new i, j = sizeof(gShop); i < j; i++)
	{
	    CreateDynamicPickup(1210, 1, gShop[i][shopPos][0], gShop[i][shopPos][1], gShop[i][shopPos][2], 0);
	    CreateDynamicMapIcon(gShop[i][shopPos][0], gShop[i][shopPos][1], gShop[i][shopPos][2], 18, 0, 0, .streamdistance = 700.0);
		gShop[i][shopAreaid] = CreateDynamicCircle(gShop[i][shopPos][0], gShop[i][shopPos][1], 3.0, 0, 0);

	    if (gShop[i][shopTeam] != NO_TEAM)
	    {
			label[0] = EOS;
		    strcat(label, gTeam[i][teamName]);
			strcat(label, "'s shop\n"WHITE"(/BUY)");
			CreateDynamic3DTextLabel(label, gTeam[i][teamColor], gShop[i][shopPos][0], gShop[i][shopPos][1], gShop[i][shopPos][2], 50.0, .worldid = 0);
		}
		else
		{
			CreateDynamic3DTextLabel("Zone's shop\n(/BUY)", COLOR_WHITE, gShop[i][shopPos][0], gShop[i][shopPos][1], gShop[i][shopPos][2], 50.0, .worldid = 0);
		}
	}

	for (new i, j = sizeof(gZone); i < j; i++)
	{
	    gZone[i][zoneId] = GangZoneCreate(gZone[i][zonePos][0], gZone[i][zonePos][1], gZone[i][zonePos][2], gZone[i][zonePos][3], SET_ALPHA(gTeam[gZone[i][zoneOwner]][teamColor], 100), .bordersize = 2.0);
		gZone[i][zoneCPId] = CreateDynamicCP(gZone[i][zoneCP][0], gZone[i][zoneCP][1], gZone[i][zoneCP][2], 5.0, 0, .streamdistance = 250.0);
	    CreateDynamicMapIcon(gZone[i][zoneCP][0], gZone[i][zoneCP][1], gZone[i][zoneCP][2], 19, 0, 0, .streamdistance = 700.0);
	    gZone[i][zoneAttacker] = INVALID_PLAYER_ID;

		label[0] = EOS;
	    strcat(label, gZone[i][zoneName]);
		strcat(label, "\n");
		strcat(label, ""WHITE"Controlled by ");
		strcat(label, gTeam[gZone[i][zoneOwner]][teamName]);
		gZone[i][zoneLabel] = CreateDynamic3DTextLabel(label, gTeam[gZone[i][zoneOwner]][teamColor], gZone[i][zoneCP][0], gZone[i][zoneCP][1], gZone[i][zoneCP][2], 50.0, .worldid = 0);
	}

	print("\n============================================");
	print("* World War IV");
	print("* Build: "#BUILD" ("BUILD_DATE")");
	print("* Author: Gammix");
	print("* ------------");
	print("* Gamemode file loaded...");
	print("============================================\n");

	print("\n============================================");
	printf("* Total teams: %i", MAX_TEAMS + 1);
	printf("* Total classes: %i", MAX_CLASSES + 1);
	printf("* Total shops: %i", sizeof(gShop));
	printf("* Total capture zones: %i", sizeof(gZone));
	printf("* Total ranks: %i", sizeof(gRank));
	print("* ------------");
	new SQLRow:keys[2], values[2];
	yoursql_sort_int(SQL:0, "users/ROW_ID", keys, values, .limit = 1);
	printf("* Total user accounts: %i", _:keys[0]);
	yoursql_sort_int(SQL:0, "bans/ROW_ID", keys, values, .limit = 1);
	printf("* Total banned accounts: %i", _:keys[0]);
	print("============================================\n");

	return 1;
}

forward OnServerUpdate();
public  OnServerUpdate()
{
	gNotifyGap++;
	if (gNotifyGap > 1)
	{
	    gNotifyGap = 0;
	    gNotifyId++;
	    if (gNotifyId >= sizeof(gNotifications))
	    {
            gNotifyId = 0;
	    }

	    TextDrawSetString(txtNotify, gNotifications[gNotifyId]);
	}

	gTimeGap++;
	if (gTimeGap > 3 * 60)
	{
	    gTimeGap = 0;

	    gTimeIdx++;
	    gServerTime = gTime[gTimeIdx];
	    gServerWeather = gWeather[gTimeIdx];

	    new buf[150];
	    format(buf, sizeof(buf), "Server time has been changed to %i and weather to %i.", gServerTime, gServerWeather);
	    SendClientMessageToAll(COLOR_WHITE, buf);

	    SetWorldTime(gServerTime);
	    SetWeather(gServerWeather);
	}
}

public OnGameModeExit()
{
	yoursql_close(SQL:0);

	KillTimer(gServerTimer);

	for (new i, j = sizeof(txtBase); i < j; i++)
	{
	    TextDrawHideForAll(txtBase[i]);
	    TextDrawDestroy(txtBase[i]);
	}
	for (new i, j = sizeof(txtConnect); i < j; i++)
	{
	    TextDrawHideForAll(txtConnect[i]);
	    TextDrawDestroy(txtConnect[i]);
	}
	for (new i, j = sizeof(txtTeam); i < j; i++)
	{
	    TextDrawHideForAll(txtTeam[i]);
	    TextDrawDestroy(txtTeam[i]);
	}
	for (new i, j = sizeof(txtClass); i < j; i++)
	{
	    TextDrawHideForAll(txtClass[i]);
	    TextDrawDestroy(txtClass[i]);
	}

	return 1;
}

ReturnPlayerName(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	return name;
}

ReturnPlayerIp(playerid)
{
	new ip[18];
	GetPlayerIp(playerid, ip, 18);
	return ip;
}

GetPlayerRank(playerid)
{
	for (new i = sizeof(gRank) - 1; i > -1; i--)
	{
	    if (GetPlayerScore(playerid) >= gRank[i][rankScore])
	    {
	        return i;
	    }
	}

	return 0;
}

ip2long(const ip[]) //(edited by me, originally by R@f)
{
  	new len = strlen(ip);
	if(! (len > 0 && len < 17))
    {
        return 0;
    }

	new count = 0;
    for (new i; i < len; i++)
    {
     	if(ip[i] == '.')
		{
			count++;
		}
	}
	if (! (count == 3))
	{
	    return 0;
	}

 	new address = strval(ip) << 24;
    count = strfind(ip, ".", false, 0) + 1;

	address += strval(ip[count]) << 16;
	count = strfind(ip, ".", false, count) + 1;

	address += strval(ip[count]) << 8;
	count = strfind(ip, ".", false, count) + 1;

	address += strval(ip[count]);
	return address;
}

ipmatch(ip1[], ip2[], rangetype = 26)
{
   	new ip = ip2long(ip1);
    new subnet = ip2long(ip2);

    new mask = -1 << (32 - rangetype);
    subnet &= mask;

    return bool:((ip & mask) == subnet);
}

public OnPlayerConnect(playerid)
{
	new pname[MAX_PLAYER_NAME];
	GetPlayerName(playerid, pname, MAX_PLAYER_NAME);

	new pip[18];
	GetPlayerIp(playerid, pip, 18);

	new SQLRow:rowid = yoursql_multiget_row(SQL:0, "bans", "ss", "name", ReturnPlayerName(playerid), "ip", ReturnPlayerIp(playerid));
	if (rowid != SQL_INVALID_ROW)
	{
	    if (yoursql_get_field_int(SQL:0, "bans/expire", rowid) != 0 && gettime() > yoursql_get_field_int(SQL:0, "bans/expire", rowid))
	    {
	        SendClientMessage(playerid, COLOR_GREEN, "You ban has been expired!");

	        yoursql_delete_row(SQL:0, "bans", rowid);
	    }
	    else
	    {
		    new buf[1000];
			strcat(buf, WHITE);

			strcat(buf, "You have been banned from the server.\n");
			strcat(buf, "If this was a mistake (from server/admin side), please report a BAN APPEAL on our forums.\n\n");

			strcat(buf, "Username: "PINK"");
		 	strcat(buf, pname);
			strcat(buf, "\n"WHITE"");

			strcat(buf, "Ip: "PINK"");
			strcat(buf, pip);
			strcat(buf, "\n"WHITE"");

		 	new value[100];

			strcat(buf, "Ban date: "PINK"");
			yoursql_get_field(SQL:0, "bans/date", rowid, value);
			strcat(buf, value);
			strcat(buf, "\n"WHITE"");

			strcat(buf, "Admin name: "PINK"");
			yoursql_get_field(SQL:0, "bans/admin_name", rowid, value);
			strcat(buf, value);
			strcat(buf, "\n"WHITE"");

			switch (yoursql_get_field_int(SQL:0, "bans/type", rowid))
			{
				case 0:
				{
					strcat(buf, "Ban type: "PINK"");
					strcat(buf, "PERMANENT");
					strcat(buf, "\n"WHITE"");
				}
				case 1:
				{
					strcat(buf, "Ban type: "PINK"");
					strcat(buf, "TEMPORARY (expire on: ");
					new year, month, day, hour, minute, second;
					TimestampToDate(yoursql_get_field_int(SQL:0, "bans/expire", rowid), year, month, day, hour, minute, second, 0);
					new month_name[15];
					switch (month)
					{
					    case 1: month_name = "January";
					    case 2: month_name = "Feburary";
					    case 3: month_name = "March";
					    case 4: month_name = "April";
					    case 5: month_name = "May";
					    case 6: month_name = "June";
					    case 7: month_name = "July";
					    case 8: month_name = "August";
					    case 9: month_name = "September";
					    case 10: month_name = "October";
					    case 11: month_name = "November";
					    case 12: month_name = "December";
					}
					format(buf, sizeof(buf), "%s%i %s, %i)", buf, day, month_name, year);
					strcat(buf, "\n"WHITE"");
				}
				case 2:
				{
					strcat(buf, "Ban type: "PINK"");
					strcat(buf, "RANGEBAN");
					strcat(buf, "\n"WHITE"");
				}
				case 3:
				{
					strcat(buf, "Ban type: "PINK"");
					strcat(buf, "TEMPORARY RANGEBAN (expire on: ");
					new year, month, day, hour, minute, second;
					TimestampToDate(yoursql_get_field_int(SQL:0, "bans/expire", rowid), year, month, day, hour, minute, second, 0);
					new month_name[15];
					switch (month)
					{
					    case 1: month_name = "January";
					    case 2: month_name = "Feburary";
					    case 3: month_name = "March";
					    case 4: month_name = "April";
					    case 5: month_name = "May";
					    case 6: month_name = "June";
					    case 7: month_name = "July";
					    case 8: month_name = "August";
					    case 9: month_name = "September";
					    case 10: month_name = "October";
					    case 11: month_name = "November";
					    case 12: month_name = "December";
					}
					format(buf, sizeof(buf), "%s%i %s, %i)", buf, day, month_name, year);
					strcat(buf, "\n"WHITE"");
				}
			}

			strcat(buf, "Reason: "RED"");
			yoursql_get_field(SQL:0, "bans/reason", rowid, value);
			strcat(buf, value);
			strcat(buf, "\n\n"WHITE"");

			strcat(buf, "Take a screenshot of this as a refrence for admins.");

			ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Account banned :(", buf, "Close", "");

			Kick(playerid);
			return 0;
		}
	}
	else
	{
		new SQLRow:keys[2], values[2];
	    yoursql_sort_int(SQL:0, "bans/ROW_ID", keys, values, .limit = 1);
		for (new i; i <= _:keys[0]; i++)
		{
		    new name[MAX_PLAYER_NAME];
		    yoursql_get_field(SQL:0, "bans/name", SQLRow:i, name, MAX_PLAYER_NAME);

		    new ip[MAX_PLAYER_NAME];
		    yoursql_get_field(SQL:0, "bans/ip", SQLRow:i, ip);

		    if (yoursql_get_field_int(SQL:0, "bans/type", SQLRow:i) >= 2)
		    {
		        if (ipmatch(pip, ip))
		        {
				    if (yoursql_get_field_int(SQL:0, "bans/expire", SQLRow:i) != 0 && gettime() > yoursql_get_field_int(SQL:0, "bans/expire", SQLRow:i))
				    {
				        SendClientMessage(playerid, COLOR_GREEN, "You rangeban has been expired!");

				        yoursql_delete_row(SQL:0, "bans", SQLRow:i);

				        break;
				    }
				    else
				    {
			            new buf[1000];
						strcat(buf, WHITE);

						strcat(buf, "You have been banned from the server.\n");
						strcat(buf, "If this was a mistake (from server/admin side), please report a BAN APPEAL on our forums.\n\n");

						strcat(buf, "Username: "PINK"");
						strcat(buf, name);
						strcat(buf, "\n"WHITE"");

						strcat(buf, "Ip: "PINK"");
						strcat(buf, ip);
						strcat(buf, "\n"WHITE"");

					 	new value[100];

						strcat(buf, "Ban date: "PINK"");
						yoursql_get_field(SQL:0, "bans/date", SQLRow:i, value);
						strcat(buf, value);
						strcat(buf, "\n"WHITE"");

						strcat(buf, "Admin name: "PINK"");
						yoursql_get_field(SQL:0, "bans/admin_name", SQLRow:i, value);
						strcat(buf, value);
						strcat(buf, "\n"WHITE"");

						switch (yoursql_get_field_int(SQL:0, "bans/type", rowid))
						{
							case 2:
							{
								strcat(buf, "Ban type: "PINK"");
								strcat(buf, "RANGEBAN");
								strcat(buf, "\n"WHITE"");
							}
							case 3:
							{
								strcat(buf, "Ban type: "PINK"");
								strcat(buf, "TEMPORARY RANGEBAN (expire on: ");
								new year, month, day, hour, minute, second;
								TimestampToDate(yoursql_get_field_int(SQL:0, "bans/expire", SQLRow:i), year, month, day, hour, minute, second, 0);
								new month_name[15];
								switch (month)
								{
								    case 1: month_name = "January";
								    case 2: month_name = "Feburary";
								    case 3: month_name = "March";
								    case 4: month_name = "April";
								    case 5: month_name = "May";
								    case 6: month_name = "June";
								    case 7: month_name = "July";
								    case 8: month_name = "August";
								    case 9: month_name = "September";
								    case 10: month_name = "October";
								    case 11: month_name = "November";
								    case 12: month_name = "December";
								}
								format(buf, sizeof(buf), "%s%i %s, %i)", buf, day, month_name, year);
								strcat(buf, "\n"WHITE"");
							}
						}

						strcat(buf, "Reason: "RED"");
						yoursql_get_field(SQL:0, "bans/reason", SQLRow:i, value);
						strcat(buf, value);
						strcat(buf, "\n\n"WHITE"");

						strcat(buf, "Take a screenshot of this as a refrence for admins.");

						ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Account banned :(", buf, "Close", "");

						Kick(playerid);
			            return 0;
      				}
		        }
		    }
		}
	}

    ptxtCapture[playerid] = CreatePlayerTextDraw(playerid,455.000000, 242.000000, "Big Ear (13)~n~~g~Owned by: ~w~Asia~n~~r~Attacked by: ~w~Russia~n~~n~~y~Zone Bonus:~n~~w~Allows your team to be protected");
	PlayerTextDrawBackgroundColor(playerid,ptxtCapture[playerid], 255);
	PlayerTextDrawFont(playerid,ptxtCapture[playerid], 1);
	PlayerTextDrawLetterSize(playerid,ptxtCapture[playerid], 0.129997, 0.699998);
	PlayerTextDrawColor(playerid,ptxtCapture[playerid], -1);
	PlayerTextDrawSetOutline(playerid,ptxtCapture[playerid], 1);
	PlayerTextDrawSetProportional(playerid,ptxtCapture[playerid], 1);
	PlayerTextDrawUseBox(playerid,ptxtCapture[playerid], 1);
	PlayerTextDrawBoxColor(playerid,ptxtCapture[playerid], 170);
	PlayerTextDrawTextSize(playerid,ptxtCapture[playerid], 510.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid,ptxtCapture[playerid], 0);

    pbarCapture[playerid] = CreatePlayerProgressBar(playerid, 518.000000, 291.000000, 4.000000, 56.200000, -1429936641, CAPTURE_TIME, 2);

	ptxtNotify[playerid] = CreatePlayerTextDraw(playerid,320.000000, 402.000000, "_");
	PlayerTextDrawAlignment(playerid,ptxtNotify[playerid], 2);
	PlayerTextDrawBackgroundColor(playerid,ptxtNotify[playerid], 255);
	PlayerTextDrawFont(playerid,ptxtNotify[playerid], 1);
	PlayerTextDrawLetterSize(playerid,ptxtNotify[playerid], 0.349999, 1.899999);
	PlayerTextDrawColor(playerid,ptxtNotify[playerid], -1);
	PlayerTextDrawSetOutline(playerid,ptxtNotify[playerid], 1);
	PlayerTextDrawSetProportional(playerid,ptxtNotify[playerid], 1);
	PlayerTextDrawSetSelectable(playerid,ptxtNotify[playerid], 0);

	ptxtMenu[0] = CreatePlayerTextDraw(playerid,160.000000, 120.000000, "Shop/Weapons list (/buy):");
	PlayerTextDrawBackgroundColor(playerid,ptxtMenu[0], 0);
	PlayerTextDrawFont(playerid,ptxtMenu[0], 1);
	PlayerTextDrawLetterSize(playerid,ptxtMenu[0], 0.140000, 0.799999);
	PlayerTextDrawColor(playerid,ptxtMenu[0], -1);
	PlayerTextDrawSetOutline(playerid,ptxtMenu[0], 0);
	PlayerTextDrawSetProportional(playerid,ptxtMenu[0], 1);
	PlayerTextDrawSetShadow(playerid,ptxtMenu[0], 1);
	PlayerTextDrawUseBox(playerid,ptxtMenu[0], 1);
	PlayerTextDrawBoxColor(playerid,ptxtMenu[0], 200);
	PlayerTextDrawTextSize(playerid,ptxtMenu[0], 467.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid,ptxtMenu[0], 0);

	new count;
	new Float:base[2] = {162.000000, 134.000000};
	for (new i; i < 6 * 4; i++)
	{
	    ptxtMenu[i + 1] = CreatePlayerTextDraw(playerid,base[0], base[1], "_");
		PlayerTextDrawBackgroundColor(playerid,ptxtMenu[i + 1], 255);
		PlayerTextDrawFont(playerid,ptxtMenu[i + 1], 1);
		PlayerTextDrawLetterSize(playerid,ptxtMenu[i + 1], 0.170000, 0.799999);
		PlayerTextDrawColor(playerid,ptxtMenu[i + 1], -1);
		PlayerTextDrawSetOutline(playerid,ptxtMenu[i + 1], 0);
		PlayerTextDrawSetProportional(playerid,ptxtMenu[i + 1], 1);
		PlayerTextDrawSetShadow(playerid,ptxtMenu[i + 1], 1);
		PlayerTextDrawSetSelectable(playerid,ptxtMenu[i + 1], 0);

		base[0] += 51.000000;
        count += 1;
        if (count == 6)
		{
			base[0] = 162.000000;
	        base[1] += 48.000000;

            count = 0;
		}
	}

	ptxtStats[playerid] = CreatePlayerTextDraw(playerid,1.000000, 430.000000, "~b~United States ~h~~h~Corporal ~h~Sniper ~w~- Kills: 3344 - Deaths: 833 - Score: 1488/2500 - ~y~~h~Use /inv to open inventory");
	PlayerTextDrawBackgroundColor(playerid,ptxtStats[playerid], 255);
	PlayerTextDrawFont(playerid,ptxtStats[playerid], 2);
	PlayerTextDrawLetterSize(playerid,ptxtStats[playerid], 0.190000, 0.999999);
	PlayerTextDrawColor(playerid,ptxtStats[playerid], -1);
	PlayerTextDrawSetOutline(playerid,ptxtStats[playerid], 1);
	PlayerTextDrawSetProportional(playerid,ptxtStats[playerid], 1);
	PlayerTextDrawUseBox(playerid,ptxtStats[playerid], 1);
	PlayerTextDrawBoxColor(playerid,ptxtStats[playerid], 84215210);
	PlayerTextDrawTextSize(playerid,ptxtStats[playerid], 650.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid,ptxtStats[playerid], 0);

	for (new i, j = sizeof(txtBase); i < j; i++)
	{
	    TextDrawShowForPlayer(playerid, txtBase[i]);
	}
	for (new i, j = sizeof(txtConnect); i < j; i++)
	{
	    TextDrawShowForPlayer(playerid, txtConnect[i]);
	}
	for (new i, j = sizeof(txtTeam); i < j; i++)
	{
	    TextDrawHideForPlayer(playerid, txtTeam[i]);
	}
	for (new i, j = sizeof(txtClass); i < j; i++)
	{
	    TextDrawHideForPlayer(playerid, txtClass[i]);
	}

	pLogged[playerid] = false;
	pInClass[playerid] = true;

	pStats[playerid][userJailTime] = -1;
	pStats[playerid][userMuteTime] = -1;

	pRank[playerid] = GetPlayerRank(playerid);

	pDuel[playerid][duelActive] = false;
 	pDuel[playerid][duelPlayer] = INVALID_PLAYER_ID;
	pDuel[playerid][duelWeapon] = 0;
	pDuel[playerid][duelBet] = 0;

    pSync[playerid] = false;

	for (new i; i < MAX_TEAMS; i++)
	{
	    GangZoneShowForPlayer(playerid, gTeam[i][teamBaseId]);
	}

	for(new i, j = sizeof(gZone); i < j; i++)
	{
		GangZoneShowForPlayer(playerid, gZone[i][zoneId]);
	}

	new text[150];
	format(text, sizeof(text), "%s(%i) has joined the server. [Total players: %i]", ReturnPlayerName(playerid), playerid, Iter_Count(Player));
	SendClientMessageToAll(COLOR_GREY, text);

	pUpdateTimer[playerid] = SetTimerEx("OnPlayerTimeUpdate", 1000, true, "i", playerid);

	pRankLabel[playerid] = CreateDynamic3DTextLabel("*", 0, 0.0, 0.0, 0.5, 35.0, playerid);

	pDonorLabel[playerid] = CreateDynamic3DTextLabel("*", 0, 0.0, 0.0, 0.7, 35.0, playerid);

	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	KillTimer(pUpdateTimer[playerid]);

    if (pDuel[playerid][duelActive])
	{
	    GivePlayerMoney(pDuel[playerid][duelPlayer], pDuel[playerid][duelBet]);

	    new weapon[35];
	    GetWeaponName(pDuel[playerid][duelWeapon], weapon, sizeof(weapon));
	    new string[144];
	    format(string, sizeof(string), "DUEL: %s(%i) have won the duel against opponent %s(%i) [weapon: %s, bet: $%i].", ReturnPlayerName(pDuel[playerid][duelPlayer]), pDuel[playerid][duelPlayer], ReturnPlayerName(playerid), playerid, weapon, pDuel[playerid][duelBet]);
	    SendClientMessageToAll(COLOR_YELLOW, string);
	    format(string, sizeof(string), "You won the duel against your opponent %s(%i) [weapon: %s, bet: $%i].", ReturnPlayerName(playerid), playerid, weapon, pDuel[playerid][duelBet]);
	    SendClientMessage(pDuel[playerid][duelPlayer], COLOR_GREEN, string);

		pDuel[pDuel[playerid][duelPlayer]][duelActive] = false;
	 	pDuel[pDuel[playerid][duelPlayer]][duelPlayer] = INVALID_PLAYER_ID;
		pDuel[pDuel[playerid][duelPlayer]][duelWeapon] = 0;
		pDuel[pDuel[playerid][duelPlayer]][duelBet] = 0;

		SpawnPlayer(pDuel[playerid][duelPlayer]);
	}

	new SQLRow:rowid = yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid));
	yoursql_set_field_int(SQL:0, "users/score", rowid, GetPlayerScore(playerid));
	yoursql_set_field_int(SQL:0, "users/money", rowid, GetPlayerMoney(playerid));
	yoursql_set_field(SQL:0, "users/ip", rowid, ReturnPlayerIp(playerid));
	yoursql_set_field_int(SQL:0, "users/kills", rowid, pStats[playerid][userKills]);
 	yoursql_set_field_int(SQL:0, "users/deaths", rowid, pStats[playerid][userDeaths]);
 	yoursql_set_field_int(SQL:0, "users/zones", rowid, pStats[playerid][userZones]);
	yoursql_set_field_int(SQL:0, "users/headshots", rowid, pStats[playerid][userHeadshots]);

	new hours, minutes, seconds;
 	GetPlayerConnectedTime(playerid, hours, minutes, seconds);
 	hours += yoursql_get_field_int(SQL:0, "users/hours", rowid);
 	minutes += yoursql_get_field_int(SQL:0, "users/minutes", rowid);
 	seconds += yoursql_get_field_int(SQL:0, "users/seconds", rowid);
	if (seconds >= 60)
	{
	    seconds = 0;
	    minutes++;
	    if (minutes >= 60)
	    {
	        minutes = 0;
	        hours++;
	    }
	}
 	yoursql_set_field_int(SQL:0, "users/hours", rowid, hours);
 	yoursql_set_field_int(SQL:0, "users/minutes", rowid, minutes);
 	yoursql_set_field_int(SQL:0, "users/seconds", rowid, seconds);

	for (new i; i < MAX_TEAMS; i++)
	{
	    GangZoneHideForPlayer(playerid, gTeam[i][teamBaseId]);
	}

	for(new i, j = sizeof(gZone); i < j; i++)
	{
		GangZoneHideForPlayer(playerid, gZone[i][zoneId]);
	}

	new text[150];
	switch (reason)
	{
		case 0:
		{
			format(text, sizeof(text), "%s(%i) have left the server. [Timeout/Crashed]", ReturnPlayerName(playerid), playerid);
		}
		case 1:
		{
			format(text, sizeof(text), "%s(%i) have left the server. [Quit]", ReturnPlayerName(playerid), playerid);
		}
		case 2:
		{
			format(text, sizeof(text), "%s(%i) have left the server. [Kicked/Banned]", ReturnPlayerName(playerid), playerid);
		}
	}
	SendClientMessageToAll(COLOR_GREY, text);

	return 1;
}

forward OnPlayerTimeUpdate(playerid);
public  OnPlayerTimeUpdate(playerid)
{
	if (pStats[playerid][userJailTime] > 0)
 	{
		pStats[playerid][userJailTime]--;

		new buf[150];
		format(buf, sizeof(buf), "~r~Unjail in %i seconds", pStats[playerid][userJailTime]);
		NotifyPlayer(playerid, buf, 0);

	   	if (pStats[playerid][userJailTime] == 0)
		{
			pStats[playerid][userJailTime] = -1;

			format(buf, sizeof(buf), "%s(%i) has been released from jail.", ReturnPlayerName(playerid), playerid);
			SendClientMessageToAll(COLOR_DODGER_BLUE, buf);

			NotifyPlayer(playerid, "~g~Unjailed!", 3000);

			SpawnPlayer(playerid);
			return;
	  	}
	}

	if (pStats[playerid][userMuteTime] > 0)
 	{
		pStats[playerid][userMuteTime]--;

		if (pStats[playerid][userMuteTime] == 0)
		{
			pStats[playerid][userMuteTime] = -1;

        	new buf[150];
			format(buf, sizeof(buf), "%s(%i) has been unmuted.", ReturnPlayerName(playerid), playerid);
			SendClientMessageToAll(COLOR_DODGER_BLUE, buf);

			NotifyPlayer(playerid, "~g~Unmuted!", 3000);
	  	}
	}

	if (pProtectTick[playerid])
	{
	    pProtectTick[playerid]--;

		if (! pProtectTick[playerid])
		{
			if (pStats[playerid][userPremium])
			{
			    SetPlayerHealth(playerid, 100.0);
			    SetPlayerArmour(playerid, 100.0);
			}
			else
			{
				new Float:get;
				SetPlayerHealth(playerid, get);
				if (get < gRank[pRank[playerid]][rankHealth])
				{
					SetPlayerHealth(playerid, gRank[pRank[playerid]][rankHealth]);
				}

				GetPlayerArmour(playerid, get);
				if (get < gRank[pRank[playerid]][rankArmour])
				{
					SetPlayerArmour(playerid, gRank[pRank[playerid]][rankArmour]);
				}
			}

            DestroyDynamic3DTextLabel(pProtectLabel[playerid]);

			NotifyPlayer(playerid, "Your spawn protection has ~r~ended!", 2000);
			SendClientMessage(playerid, COLOR_WHITE, "Anti-Spawnkill Protection: Ended (now you are on your own).");

			if (pStats[playerid][userPremium])
			{
			    SendClientMessage(playerid, COLOR_CYAN, "[VIP] Premium health and armour recieved!");
			}

			new buf[150];
			format(buf, sizeof(buf), "%s\n%s", gRank[pRank[playerid]][rankName], gClass[pClass[playerid]][className]);
			UpdateDynamic3DTextLabelText(pRankLabel[playerid], gTeam[pTeam[playerid]][teamColor], buf);
		}
		else
		{
		    new buf[150];
		    format(buf, sizeof(buf), "Your spawn protection will end in ~y~%i seconds", pProtectTick[playerid]);
            NotifyPlayer(playerid, buf, 0);

		    format(buf, sizeof(buf), "AntiSK for %i seconds", pProtectTick[playerid]);
            UpdateDynamic3DTextLabelText(pProtectLabel[playerid], COLOR_RED, buf);
		}
	}

	new buf[150];
	format(buf, sizeof(buf), "~b~%s's ~h~~h~%s ~h~%s ~w~- Kills: %02i - Deaths: %02i - Score: %02i/%02i - ~y~~h~Type /inv to check your inventory item", gTeam[pTeam[playerid]][teamName], gRank[pRank[playerid]][rankName], gClass[pClass[playerid]][className], pStats[playerid][userKills], pStats[playerid][userDeaths], GetPlayerScore(playerid), gRank[((pRank[playerid] + 1) >= sizeof(gRank)) ? (sizeof(gRank)) : (pRank[playerid] + 1)][rankScore]);
	PlayerTextDrawSetString(playerid, ptxtStats[playerid], buf);
}

public OnPlayerRequestClass(playerid, classid)
{
	SetPlayerVirtualWorld(playerid, playerid + 10);

	pInClass[playerid] = true;
	pSpawn[playerid] = sizeof(gZone);

    pStats[playerid][userGod] = false;
    pStats[playerid][userGodCar] = false;
    pStats[playerid][userOnDuty] = false;

	if (pLogged[playerid])
	{
		for (new i, j = sizeof(txtBase); i < j; i++)
		{
		    TextDrawShowForPlayer(playerid, txtBase[i]);
		}
		for (new i, j = sizeof(txtConnect); i < j; i++)
		{
		    TextDrawHideForPlayer(playerid, txtConnect[i]);
		}
		for (new i, j = sizeof(txtTeam); i < j; i++)
		{
		    TextDrawShowForPlayer(playerid, txtTeam[i]);
		}
		for (new i, j = sizeof(txtClass); i < j; i++)
		{
		    TextDrawShowForPlayer(playerid, txtClass[i]);
		}
    }
    else
    {
		for (new i, j = sizeof(txtBase); i < j; i++)
		{
		    TextDrawShowForPlayer(playerid, txtBase[i]);
		}
		for (new i, j = sizeof(txtConnect); i < j; i++)
		{
		    TextDrawShowForPlayer(playerid, txtConnect[i]);
		}
		for (new i, j = sizeof(txtTeam); i < j; i++)
		{
		    TextDrawHideForPlayer(playerid, txtTeam[i]);
		}
		for (new i, j = sizeof(txtClass); i < j; i++)
		{
		    TextDrawHideForPlayer(playerid, txtClass[i]);
		}
    }
	TextDrawHideForPlayer(playerid, txtNotify);

	PlayerTextDrawHide(playerid, ptxtStats[playerid]);

	for (new i; i < 50; i++)
	{
		SendClientMessage(playerid, COLOR_WHITE, " ");
	}

    KillTimer(pSkipTimer[playerid]);
    pSkipTimer[playerid] = SetTimerEx("OnPlayerSkipClass", 150, false, "i", playerid);

	return 1;
}

forward OnPlayerSkipClass(playerid);
public 	OnPlayerSkipClass(playerid)
{
	if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)
	{
		SpawnPlayer(playerid);
	}
}

SelectPlayerTeam(playerid, teamid)
{
    TextDrawColor(txtTeam[1], 84545430);
	TextDrawColor(txtTeam[4], 84545430);
 	TextDrawColor(txtTeam[7], 84545430);
	TextDrawColor(txtTeam[10], 84545430);
   	TextDrawColor(txtTeam[13], 84545430);
	TextDrawColor(txtTeam[16], 84545430);
	TextDrawColor(txtTeam[19], 84545430);

	switch (teamid)
	{
	    case 0:
	    {
			TextDrawColor(txtTeam[1], 0xFF0000FF);
	    }
	    case 1:
	    {
			TextDrawColor(txtTeam[4], 0xFF0000FF);
	    }
	    case 2:
	    {
			TextDrawColor(txtTeam[7], 0xFF0000FF);
	    }
	    case 3:
	    {
			TextDrawColor(txtTeam[10], 0xFF0000FF);
	    }
	    case 4:
	    {
			TextDrawColor(txtTeam[13], 0xFF0000FF);
	    }
	    case 5:
	    {
			TextDrawColor(txtTeam[16], 0xFF0000FF);
	    }
	    case 6:
	    {
			TextDrawColor(txtTeam[19], 0xFF0000FF);
	    }
	}

    TextDrawShowForPlayer(playerid, txtTeam[1]);
    TextDrawShowForPlayer(playerid, txtTeam[4]);
    TextDrawShowForPlayer(playerid, txtTeam[7]);
    TextDrawShowForPlayer(playerid, txtTeam[10]);
    TextDrawShowForPlayer(playerid, txtTeam[13]);
    TextDrawShowForPlayer(playerid, txtTeam[16]);
    TextDrawShowForPlayer(playerid, txtTeam[19]);
}

NotifyPlayer(playerid, text[], expiretime)
{
	PlayerTextDrawSetString(playerid, ptxtNotify[playerid], text);
	PlayerTextDrawShow(playerid, ptxtNotify[playerid]);

	KillTimer(pNotifyTimer[playerid]);
	if (expiretime)
	{
		pNotifyTimer[playerid] = SetTimerEx("OnPlayerNotified", expiretime, false, "i", playerid);
	}
}

forward OnPlayerNotified(playerid);
public  OnPlayerNotified(playerid)
{
    PlayerTextDrawHide(playerid, ptxtNotify[playerid]);
}

public OnPlayerSpawn(playerid)
{
	SetPlayerTime(playerid, gServerTime, 0);
	SetPlayerWeather(playerid, gServerWeather);

	if (pSync[playerid])
	{
		pSync[playerid] = false;
	    return 1;
	}

	pPremiumSupply[playerid] = false;

	pTrapped[playerid] = false;
	KillTimer(pTrappedTimer[playerid]);
	if (IsValidDynamicObject(pTrappedObject[playerid]))
	{
		DestroyDynamicObject(pTrappedObject[playerid]);
	}

	for (new i, j = sizeof(menuInventoryModels); i < j; i++)
	{
	    pInventory[playerid][i] = 0;
	}

	if (IsValidDynamicObject(pNetTrapObject[playerid][0]))
	{
		DestroyDynamicObject(pNetTrapObject[playerid][0]);
		DestroyDynamicArea(pNetTrapArea[playerid][0]);
		DestroyDynamic3DTextLabel(pNetTrapLabel[playerid][0]);
		KillTimer(pNetTrapTimer[playerid][0]);
	}
	if (IsValidDynamicObject(pNetTrapObject[playerid][1]))
	{
		DestroyDynamicObject(pNetTrapObject[playerid][1]);
		DestroyDynamicArea(pNetTrapArea[playerid][1]);
		DestroyDynamic3DTextLabel(pNetTrapLabel[playerid][1]);
		KillTimer(pNetTrapTimer[playerid][1]);
	}

	if (IsValidDynamicObject(pDynamiteObject[playerid][0]))
	{
		DestroyDynamicObject(pDynamiteObject[playerid][0]);
		DestroyDynamic3DTextLabel(pDynamiteLabel[playerid][0]);
	}
	if (IsValidDynamicObject(pDynamiteObject[playerid][1]))
	{
		DestroyDynamicObject(pDynamiteObject[playerid][1]);
		DestroyDynamic3DTextLabel(pDynamiteLabel[playerid][1]);
	}
	if (IsValidDynamicObject(pDynamiteObject[playerid][2]))
	{
		DestroyDynamicObject(pDynamiteObject[playerid][2]);
		DestroyDynamic3DTextLabel(pDynamiteLabel[playerid][2]);
	}

	if (IsValidDynamicObject(pLandmineObject[playerid][0]))
	{
		DestroyDynamicObject(pLandmineObject[playerid][0]);
		DestroyDynamicArea(pLandmineAreaid[playerid][0]);
		DestroyDynamic3DTextLabel(pLandmineLabel[playerid][0]);
	}
	if (IsValidDynamicObject(pLandmineObject[playerid][1]))
	{
		DestroyDynamicObject(pLandmineObject[playerid][1]);
		DestroyDynamicArea(pLandmineAreaid[playerid][1]);
		DestroyDynamic3DTextLabel(pLandmineLabel[playerid][1]);
	}
	if (IsValidDynamicObject(pLandmineObject[playerid][2]))
	{
		DestroyDynamicObject(pLandmineObject[playerid][2]);
		DestroyDynamicArea(pLandmineAreaid[playerid][2]);
		DestroyDynamic3DTextLabel(pLandmineLabel[playerid][2]);
	}

	if (IsValidDynamicObject(pMusicBoxObject[playerid]))
	{
	    DestroyDynamicObject(pMusicBoxObject[playerid]);
	    foreach (new i : Player)
	    {
	        if (IsPlayerInDynamicArea(i, pMusicBoxAreaid[playerid]))
	        {
	            StopAudioStreamForPlayer(i);
	        }
	    }
	    DestroyDynamicArea(pMusicBoxAreaid[playerid]);
	    DestroyDynamic3DTextLabel(pMusicBoxLabel[playerid]);
	}

	if (pSpikeTimer[playerid][0])
	{
		SpikeStrip_Delete(pSpikeObject[playerid][0]);
		KillTimer(pSpikeTimer[playerid][0]);
        pSpikeTimer[playerid][0] = 0;
		DestroyDynamic3DTextLabel(pSpikeLabel[playerid][0]);
	}
	if (pSpikeTimer[playerid][1])
	{
		SpikeStrip_Delete(pSpikeObject[playerid][1]);
		KillTimer(pSpikeTimer[playerid][1]);
        pSpikeTimer[playerid][1] = 0;
		DestroyDynamic3DTextLabel(pSpikeLabel[playerid][1]);
	}
	if (pSpikeTimer[playerid][2])
	{
		SpikeStrip_Delete(pSpikeObject[playerid][2]);
		KillTimer(pSpikeTimer[playerid][2]);
        pSpikeTimer[playerid][2] = 0;
		DestroyDynamic3DTextLabel(pSpikeLabel[playerid][2]);
	}

    pAirstrike[playerid][asLastStrike] = 0;
    pCarepack[playerid][cpLastDrop] = 0;

    pAirstrike[playerid][asCalled] = false;
    pCarepack[playerid][cpCalled] = false;

    if (pDuel[playerid][duelActive])
	{
	    GivePlayerMoney(pDuel[playerid][duelPlayer], pDuel[playerid][duelBet]);

	    new weapon[35];
	    GetWeaponName(pDuel[playerid][duelWeapon], weapon, sizeof(weapon));
	    new string[144];
	    format(string, sizeof(string), "DUEL: %s(%i) have won the duel against opponent %s(%i) [weapon: %s, bet: $%i].", ReturnPlayerName(pDuel[playerid][duelPlayer]), pDuel[playerid][duelPlayer], ReturnPlayerName(playerid), playerid, weapon, pDuel[playerid][duelBet]);
	    SendClientMessageToAll(COLOR_YELLOW, string);
	    format(string, sizeof(string), "You won the duel against your opponent %s(%i) [weapon: %s, bet: $%i].", ReturnPlayerName(playerid), playerid, weapon, pDuel[playerid][duelBet]);
	    SendClientMessage(pDuel[playerid][duelPlayer], COLOR_GREEN, string);
	    format(string, sizeof(string), "You lost the duel against your opponent %s(%i) [weapon: %s, bet: $%i].", ReturnPlayerName(pDuel[playerid][duelPlayer]), pDuel[playerid][duelPlayer], weapon, pDuel[playerid][duelBet]);
	    SendClientMessage(playerid, COLOR_TOMATO, string);

		NotifyPlayer(playerid, "You ~r~LOST ~w~~h~the duel!", 3000);
		NotifyPlayer(pDuel[playerid][duelPlayer], "You ~g~WON ~w~~h~the duel!", 3000);

		pDuel[pDuel[playerid][duelPlayer]][duelActive] = false;
	 	pDuel[pDuel[playerid][duelPlayer]][duelPlayer] = INVALID_PLAYER_ID;
		pDuel[pDuel[playerid][duelPlayer]][duelWeapon] = 0;
		pDuel[pDuel[playerid][duelPlayer]][duelBet] = 0;

		SpawnPlayer(pDuel[playerid][duelPlayer]);
	}
	pDuel[playerid][duelActive] = false;
 	pDuel[playerid][duelPlayer] = INVALID_PLAYER_ID;
	pDuel[playerid][duelWeapon] = 0;
	pDuel[playerid][duelBet] = 0;

	TogglePlayerControllable(playerid, false);

    pKiller[playerid][0] = INVALID_PLAYER_ID;
    pKiller[playerid][1] = 0;

    pHasHelmet[playerid] = false;
    pHasMask[playerid] = false;

    pLastDamageTime[playerid] = 0;

    for (new i; i < 13; i++)
    {
    	pWeaponsSpree[playerid][i] = 0;
	}

	new count[MAX_TEAMS];
	foreach (new i : Player)
	{
	    if (0 <= GetPlayerTeam(i) < MAX_TEAMS)
	    {
	        count[GetPlayerTeam(i)]++;
	    }
	}

	new buf[450];

	format(buf, sizeof(buf), "%s~n~~y~~h~~h~Players: %i", gTeam[0][teamName], count[0]);
	TextDrawSetString(txtTeam[3], buf);
	format(buf, sizeof(buf), "%s~n~~y~~h~~h~Players: %i", gTeam[1][teamName], count[1]);
	TextDrawSetString(txtTeam[6], buf);
	format(buf, sizeof(buf), "%s~n~~y~~h~~h~Players: %i", gTeam[2][teamName], count[2]);
	TextDrawSetString(txtTeam[9], buf);
	format(buf, sizeof(buf), "%s~n~~y~~h~~h~Players: %i", gTeam[3][teamName], count[3]);
	TextDrawSetString(txtTeam[12], buf);
	format(buf, sizeof(buf), "%s~n~~y~~h~~h~Players: %i", gTeam[4][teamName], count[4]);
	TextDrawSetString(txtTeam[15], buf);
	format(buf, sizeof(buf), "%s~n~~y~~h~~h~Players: %i", gTeam[5][teamName], count[5]);
	TextDrawSetString(txtTeam[18], buf);
	format(buf, sizeof(buf), "%s~n~~y~~h~~h~Players: %i", gTeam[6][teamName], count[6]);
	TextDrawSetString(txtTeam[21], buf);

	if (! pLogged[playerid])
	{
		SetPlayerCameraPos(playerid, 158.8506, 2263.6631, 129.6489);
		SetPlayerCameraLookAt(playerid, 159.1849, 2264.6108, 129.3190);

		for (new i, j = sizeof(txtBase); i < j; i++)
		{
		    TextDrawShowForPlayer(playerid, txtBase[i]);
		}
		for (new i, j = sizeof(txtConnect); i < j; i++)
		{
		    TextDrawShowForPlayer(playerid, txtConnect[i]);
		}
		for (new i, j = sizeof(txtTeam); i < j; i++)
		{
		    TextDrawHideForPlayer(playerid, txtTeam[i]);
		}
		for (new i, j = sizeof(txtClass); i < j; i++)
		{
		    TextDrawHideForPlayer(playerid, txtClass[i]);
		}

		new SQLRow:rowid = yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid));
		if (rowid == SQL_INVALID_ROW)
		{
		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		    SendClientMessage(playerid, COLOR_GREEN, "Welcome to World War IV, SAMP gaming community.");
		    SendClientMessage(playerid, COLOR_GREEN, "This a little formality that every new user should complete, please register and continue to play and have fun!");
		    SendClientMessage(playerid, COLOR_GREEN, "After registeration, you will get $50000 and 15 score as a regiseration achievement.");

		    new info[450];
			strcat(info, ""WHITE"Welcome "RED"");
			strcat(info, ReturnPlayerName(playerid));
			strcat(info, " "WHITE", you are new to the server!\n\n");
			strcat(info, "Before registering, please read the main rules:\n");
			strcat(info, ""RED"1. "WHITE"No cheats/hacks/invalid ways of playing.\n");
			strcat(info, ""RED"2. "WHITE"No insulting in main chat, respect all.\n");
			strcat(info, ""RED"3. "WHITE"Read all the rules in /rules.\n\n");
			strcat(info, "Now please insert a password and register this account!");

		    ShowPlayerDialog(playerid, DIALOG_ID_REGISTER, DIALOG_STYLE_PASSWORD, "Account registration", info, "Register", "Quit");
		}
		else
		{
		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		    SendClientMessage(playerid, COLOR_GREEN, "Welcome back to World War IV, SAMP gaming community.");

		    new ip[18];
			yoursql_get_field(SQL:0, "users/ip", rowid, ip);
	  		if (yoursql_get_field_int(SQL:0, "users/auto_login", rowid) && ! strcmp(ip, ReturnPlayerIp(playerid)))
	  		{
			  	SendClientMessage(playerid, COLOR_GREEN, "Login session has automatically completed, thanks for joining us back!");
				SendClientMessage(playerid, COLOR_GREEN, "If you want to change your account settings, type /settings.");

				ResetPlayerMoney(playerid);
				GivePlayerMoney(playerid, yoursql_get_field_int(SQL:0, "users/money", rowid));
				SetPlayerScore(playerid, yoursql_get_field_int(SQL:0, "users/score", rowid));

				pRank[playerid] = GetPlayerRank(playerid);

				pLogged[playerid] = true;
				SpawnPlayer(playerid);
			}
			else
			{
			    for (new i; i < 50; i++)
			    {
			        SendClientMessage(playerid, COLOR_WHITE, " ");
			    }
		    	SendClientMessage(playerid, COLOR_GREEN, "Welcome back to World War IV, SAMP gaming community.");
				SendClientMessage(playerid, COLOR_GREEN, "You are already registered here, complete the login session and enjoy your stay!");

			    new info[450];
				strcat(info, ""WHITE"Welcome back "RED"");
				strcat(info, ReturnPlayerName(playerid));
				strcat(info, " "WHITE", you are already registerd!\n\n");
				strcat(info, "If you any problem logging in this account, you can do the following:\n");
				strcat(info, ""RED"1. "WHITE"Press 'PROBLEM' and enter the email registered with this account.\n");
				strcat(info, ""RED"2. "WHITE"Press 'PROBLEM' and click 'QUIT' there if this is not your account.\n\n");
				strcat(info, "Else, please insert your password and login this account!");

			    ShowPlayerDialog(playerid, DIALOG_ID_LOGIN, DIALOG_STYLE_PASSWORD, "Account login required", info, "Login", "Problem?");
			}
		}

		return 1;
	}
	else if (pInClass[playerid])
	{
		for (new i, j = sizeof(txtBase); i < j; i++)
		{
		    TextDrawShowForPlayer(playerid, txtBase[i]);
		}
		for (new i, j = sizeof(txtConnect); i < j; i++)
		{
		    TextDrawHideForPlayer(playerid, txtConnect[i]);
		}
		for (new i, j = sizeof(txtTeam); i < j; i++)
		{
		    TextDrawShowForPlayer(playerid, txtTeam[i]);
		}
		for (new i, j = sizeof(txtClass); i < j; i++)
		{
		    TextDrawShowForPlayer(playerid, txtClass[i]);
		}
		TextDrawHideForPlayer(playerid, txtNotify);

		PlayerTextDrawHide(playerid, ptxtStats[playerid]);

		TextDrawColor(txtClass[1], -1768515946);
		TextDrawColor(txtClass[4], -1768515946);
	 	TextDrawColor(txtClass[7], -1768515946);
		TextDrawColor(txtClass[10], -1768515946);
	   	TextDrawColor(txtClass[13], -1768515946);
		TextDrawColor(txtClass[16], -1768515946);

	    TextDrawShowForPlayer(playerid, txtClass[1]);
	    TextDrawShowForPlayer(playerid, txtClass[4]);
	    TextDrawShowForPlayer(playerid, txtClass[7]);
	    TextDrawShowForPlayer(playerid, txtClass[10]);
	    TextDrawShowForPlayer(playerid, txtClass[13]);
	    TextDrawShowForPlayer(playerid, txtClass[16]);

		SelectPlayerTeam(playerid, pTeam[playerid]);
		SetPlayerSkin(playerid, gTeam[pTeam[playerid]][teamSkin]);
		SetPlayerColor(playerid, gTeam[pTeam[playerid]][teamColor]);

    	SetPlayerCameraPos(playerid, gTeam[pTeam[playerid]][teamCam][0], gTeam[pTeam[playerid]][teamCam][1], gTeam[pTeam[playerid]][teamCam][2]);
        SetPlayerCameraLookAt(playerid, gTeam[pTeam[playerid]][teamCam2][0], gTeam[pTeam[playerid]][teamCam2][1], gTeam[pTeam[playerid]][teamCam2][2], CAMERA_MOVE);
		SetPlayerPos(playerid, gTeam[pTeam[playerid]][teamCam3][0], gTeam[pTeam[playerid]][teamCam3][1], gTeam[pTeam[playerid]][teamCam3][2]);
		SetPlayerFacingAngle(playerid, gTeam[pTeam[playerid]][teamCam3][3]);

		PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);

		SelectPlayerTeam(playerid, pTeam[playerid]);
		SelectTextDraw(playerid, 0xFF0000FF);

		return 1;
	}

	TogglePlayerControllable(playerid, true);

	for (new i, j = sizeof(txtBase); i < j; i++)
	{
	    TextDrawHideForPlayer(playerid, txtBase[i]);
	}
	for (new i, j = sizeof(txtConnect); i < j; i++)
	{
	    TextDrawHideForPlayer(playerid, txtConnect[i]);
	}
	for (new i, j = sizeof(txtTeam); i < j; i++)
	{
	    TextDrawHideForPlayer(playerid, txtTeam[i]);
	}
	for (new i, j = sizeof(txtClass); i < j; i++)
	{
	    TextDrawHideForPlayer(playerid, txtClass[i]);
	}

	TextDrawShowForPlayer(playerid, txtNotify);

	PlayerTextDrawShow(playerid, ptxtStats[playerid]);

    if (pStats[playerid][userJailTime] > 0)
    {
		SetPlayerHealth(playerid, FLOAT_INFINITY);
		SetPlayerArmour(playerid, 0.0);
		SetPlayerInterior(playerid, 3);
		SetPlayerPos(playerid, 197.6661, 173.8179, 1003.0234);
		SetCameraBehindPlayer(playerid);

		format(buf, sizeof(buf), "You are still in jail for %i seconds.", pStats[playerid][userJailTime]);
	    SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
		return 1;
	}

	if (pStats[playerid][userOnDuty])
    {
        SendClientMessage(playerid, COLOR_WHITE, " ");
        SendClientMessage(playerid, COLOR_GREEN, "- You have spawned -");

        new i = random(sizeof(gAdminSpawn));
	    SetPlayerPos(playerid, gAdminSpawn[i][0], gAdminSpawn[i][1], gAdminSpawn[i][2]);
	    SetPlayerFacingAngle(playerid, gAdminSpawn[i][3]);

	    SetPlayerSkin(playerid, 217);
	    SetPlayerColor(playerid, COLOR_HOT_PINK);
	    SetPlayerTeam(playerid, 100);
	    ResetPlayerWeapons(playerid);
	    GivePlayerWeapon(playerid, 38, 999999);
	    if (! pStats[playerid][userGod])
	    {
	        pStats[playerid][userGod] = true;
	    }
	    if (! pStats[playerid][userGodCar])
	    {
    		pStats[playerid][userGodCar] = true;
    	}
	    SetPlayerHealth(playerid, FLOAT_INFINITY);
	    SetVehicleHealth(GetPlayerVehicleID(playerid), FLOAT_INFINITY);

	    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
        SendClientMessage(playerid, COLOR_WHITE, "You are currently "GREEN"ON Admin Duty"WHITE". To switch it off, type /offduty.");
        SendClientMessage(playerid, COLOR_WHITE, "For commands list for your respective level, type /acmds.");
        SendClientMessage(playerid, COLOR_WHITE, "Weapon recieved: Minigun (/aweaps for more weapons range)");

        SendClientMessage(playerid, COLOR_WHITE, " ");
	}
	else
	{
		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 0);

		SetPlayerSkin(playerid, gTeam[pTeam[playerid]][teamSkin]);
		SetPlayerColor(playerid, gTeam[pTeam[playerid]][teamColor]);
		SetPlayerTeam(playerid, pTeam[playerid]);

        SendClientMessage(playerid, COLOR_WHITE, " ");
        SendClientMessage(playerid, COLOR_GREEN, "- You have spawned -");

		format(buf, sizeof(buf), "You have spawned as {%06x}%s's %s %s"WHITE".", gTeam[pTeam[playerid]][teamColor] >>> 8, gTeam[pTeam[playerid]][teamName], gRank[pRank[playerid]][rankName], gClass[pClass[playerid]][className]);
		SendClientMessage(playerid, COLOR_WHITE, buf);

		switch (pClass[playerid])
		{
		    case 0:
		    {
	    		pActionTime[playerid] = 0;
		    }
		    case 1:
		    {
	    		pActionTime[playerid] = 0;
		    }
		    case 2:
		    {
		        SetPlayerColor(playerid, SET_ALPHA(gTeam[pTeam[playerid]][teamColor], 0));
		    }
		    case 3:
		    {
	    		pActionTime[playerid] = 0;
	    		if (IsValidVehicle(pBuildMode[playerid]))
	    		{
	    		    DestroyVehicle(pBuildMode[playerid]);
	    		}
	    		pBuildMode[playerid] = 0;
			}
		    case 4:
		    {
			}
		    case 5:
		    {
				pDisguizeKits[playerid] = 3;
			}
		}

	    ResetPlayerWeapons(playerid);
		GivePlayerWeapon(playerid, gClass[pClass[playerid]][classWeapon1][0], gClass[pClass[playerid]][classWeapon1][1] * ((pStats[playerid][userPremium]) ? (3) : (1)));
		GivePlayerWeapon(playerid, gClass[pClass[playerid]][classWeapon2][0], gClass[pClass[playerid]][classWeapon2][1] * ((pStats[playerid][userPremium]) ? (3) : (1)));
		GivePlayerWeapon(playerid, gClass[pClass[playerid]][classWeapon3][0], gClass[pClass[playerid]][classWeapon3][1] * ((pStats[playerid][userPremium]) ? (3) : (1)));
		GivePlayerWeapon(playerid, gClass[pClass[playerid]][classWeapon4][0], gClass[pClass[playerid]][classWeapon4][1] * ((pStats[playerid][userPremium]) ? (3) : (1)));
		GivePlayerWeapon(playerid, gClass[pClass[playerid]][classWeapon5][0], gClass[pClass[playerid]][classWeapon5][1] * ((pStats[playerid][userPremium]) ? (3) : (1)));

		buf[0] = EOS;
		strcat(buf, "Class Weapons: ");
		new weapon_name[35];
	 	strcat(buf, SAMP_BLUE);
		GetWeaponName(gClass[pClass[playerid]][classWeapon1][0], weapon_name, sizeof(weapon_name));
	 	strcat(buf, weapon_name);
	 	strcat(buf, ""WHITE", ");

	 	strcat(buf, SAMP_BLUE);
		GetWeaponName(gClass[pClass[playerid]][classWeapon2][0], weapon_name, sizeof(weapon_name));
	 	strcat(buf, weapon_name);
	 	strcat(buf, ""WHITE", ");

	 	strcat(buf, SAMP_BLUE);
		GetWeaponName(gClass[pClass[playerid]][classWeapon3][0], weapon_name, sizeof(weapon_name));
	 	strcat(buf, weapon_name);
	 	strcat(buf, ""WHITE", ");

	 	strcat(buf, SAMP_BLUE);
		GetWeaponName(gClass[pClass[playerid]][classWeapon4][0], weapon_name, sizeof(weapon_name));
	 	strcat(buf, weapon_name);
	 	strcat(buf, ""WHITE", ");

	 	strcat(buf, SAMP_BLUE);
		GetWeaponName(gClass[pClass[playerid]][classWeapon5][0], weapon_name, sizeof(weapon_name));
	 	strcat(buf, weapon_name);
		strcat(buf, ""WHITE".");
		SendClientMessage(playerid, COLOR_WHITE, buf);

		if (pStats[playerid][userPremium])
		{
		    SendClientMessage(playerid, COLOR_CYAN, "[VIP] Premium ammunation, 3x ammo for every weapon!");
		}

		new SQLRow:rowid = yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid));
		new weapon[5];
		weapon[0] = yoursql_get_field_int(SQL:0, "users/weapon1", rowid);
		weapon[1] = yoursql_get_field_int(SQL:0, "users/weapon2", rowid);
		weapon[2] = yoursql_get_field_int(SQL:0, "users/weapon3", rowid);

		for (new i; i < 3; i++)
		{
		    switch (weapon[i])
		    {
		        case 1..15:
		        {
		            GivePlayerWeapon(playerid, weapon[i], 1);
		        }
		        case 16..18, 39:
		        {
		            GivePlayerWeapon(playerid, weapon[i], 2);
		        }
		        case 22..24:
		        {
		            GivePlayerWeapon(playerid, weapon[i], 200);
		        }
		        case 25, 26, 27:
		        {
		            GivePlayerWeapon(playerid, weapon[i], 100);
		        }
		        case 28, 29, 32:
		        {
		            GivePlayerWeapon(playerid, weapon[i], 250);
		        }
		        case 30, 31:
		        {
		            GivePlayerWeapon(playerid, weapon[i], 300);
		        }
		        case 33, 34:
		        {
		            GivePlayerWeapon(playerid, weapon[i], 150);
		        }
		        case 35, 36:
		        {
		            GivePlayerWeapon(playerid, weapon[i], 2);
		        }
		        case 37:
		        {
		            GivePlayerWeapon(playerid, weapon[i], 500);
		        }
		        case 38:
		        {
		            GivePlayerWeapon(playerid, weapon[i], 100);
		        }
		    }
		}
		SendClientMessage(playerid, COLOR_GREY, "You can change your spawn by /ss, class by /sc and team by /st.");

		buf[0] = EOS;
		strcat(buf, "Personal weapons: ");
		if (weapon[0])
		{
			strcat(buf, SAMP_BLUE);
		    GetWeaponName(weapon[0], weapon_name, sizeof(weapon_name));
			strcat(buf, weapon_name);
			strcat(buf, ""WHITE", ");
		}
		else
		{
			strcat(buf, TOMATO);
			strcat(buf, "No Weapon");
			strcat(buf, ""WHITE", ");
		}

		if (weapon[1])
		{
			strcat(buf, SAMP_BLUE);
		    GetWeaponName(weapon[1], weapon_name, sizeof(weapon_name));
			strcat(buf, weapon_name);
			strcat(buf, ""WHITE", ");
		}
		else
		{
			strcat(buf, TOMATO);
			strcat(buf, "No Weapon");
			strcat(buf, ""WHITE", ");
		}

		if (weapon[2])
		{
			strcat(buf, SAMP_BLUE);
		    GetWeaponName(weapon[2], weapon_name, sizeof(weapon_name));
			strcat(buf, weapon_name);
		}
		else
		{
			strcat(buf, TOMATO);
			strcat(buf, "No Weapon");
		}
		strcat(buf, ""WHITE".");
		SendClientMessage(playerid, COLOR_WHITE, buf);
		SendClientMessage(playerid, COLOR_GREY, "You can change your personal weapon by /weapons. To modify your weapons or add extensions, type /extensions.");

		buf[0] = EOS;
		strcat(buf, "Spawn place: ");
		if (pSpawn[playerid] == sizeof(gZone))
		{
		    strcat(buf, ""SAMP_BLUE"Team base.");
		}
		else if (gZone[pSpawn[playerid]][zoneOwner] != pTeam[playerid])
		{
		    strcat(buf, ""TOMATO"Team base (the zone you were supposed to spawn in is no more under our control).");
		    pSpawn[playerid] = sizeof(gZone);
		}
		else if (gZone[pSpawn[playerid]][zoneAttacker] != INVALID_PLAYER_ID)
		{
		    strcat(buf, ""TOMATO"Team base (the zone you were supposed to spawn in is under attack).");
		    pSpawn[playerid] = sizeof(gZone);
		}
		else
		{
			strcat(buf, SAMP_BLUE);
			strcat(buf, gZone[pSpawn[playerid]][zoneName]);
			strcat(buf, ".");
		}
		SendClientMessage(playerid, COLOR_WHITE, buf);

        SendClientMessage(playerid, COLOR_WHITE, " ");

		if (pSpawn[playerid] == sizeof(gZone))
		{
		    switch (random(3))
			{
				case 0:
				{
					SetPlayerPos(playerid, gTeam[pTeam[playerid]][teamSpawn1][0], gTeam[pTeam[playerid]][teamSpawn1][1], gTeam[pTeam[playerid]][teamSpawn1][2]);
					SetPlayerFacingAngle(playerid, gTeam[pTeam[playerid]][teamSpawn1][3]);
				}
				case 1:
				{
					SetPlayerPos(playerid, gTeam[pTeam[playerid]][teamSpawn2][0], gTeam[pTeam[playerid]][teamSpawn2][1], gTeam[pTeam[playerid]][teamSpawn2][2]);
					SetPlayerFacingAngle(playerid, gTeam[pTeam[playerid]][teamSpawn2][3]);
				}
				case 2:
				{
					SetPlayerPos(playerid, gTeam[pTeam[playerid]][teamSpawn3][0], gTeam[pTeam[playerid]][teamSpawn3][1], gTeam[pTeam[playerid]][teamSpawn3][2]);
					SetPlayerFacingAngle(playerid, gTeam[pTeam[playerid]][teamSpawn3][3]);
				}
			}
		}
		else
		{
			SetPlayerPos(playerid, gZone[pSpawn[playerid]][zoneSpawn][0], gZone[pSpawn[playerid]][zoneSpawn][1], gZone[pSpawn[playerid]][zoneSpawn][2]);
			SetPlayerFacingAngle(playerid, gZone[pSpawn[playerid]][zoneSpawn][3]);
		}

 		if (pStats[playerid][userGod])
	    {
	        SetPlayerHealth(playerid, FLOAT_INFINITY);
	        SendClientMessage(playerid, COLOR_DODGER_BLUE, "Your godmode is active, type /god to deactivate.");
		}
		else
		{
		    if (pStats[playerid][userPremium])
			{
			    SetPlayerArmour(playerid, 100.0);
			}
			else
			{
				SetPlayerArmour(playerid, gRank[pRank[playerid]][rankArmour]);
			}
			SetPlayerHealth(playerid, FLOAT_INFINITY);

			pProtectTick[playerid] = 10;
			SendClientMessage(playerid, COLOR_WHITE, "Anti-Spawnkill Protection: 10 seconds. (if you shoot, protection will end instantly)");
			NotifyPlayer(playerid, "Your spawn protection will end in ~y~10 seconds", 0);

			pProtectLabel[playerid] = CreateDynamic3DTextLabel("AntiSK for 10 seconds", COLOR_RED, 0.0, 0.0, 0.0, 35.0, playerid);
			UpdateDynamic3DTextLabelText(pRankLabel[playerid], COLOR_WHITE, "*");
		}

 		if (pStats[playerid][userGodCar])
	    {
	        SetVehicleHealth(GetPlayerVehicleID(playerid), FLOAT_INFINITY);
	        SendClientMessage(playerid, COLOR_DODGER_BLUE, "Your godcar mode is active, type /godcar to deactivate.");
		}
	}
	for (new i, j = sizeof(gZone); i < j; i++)
	{
	    if (gZone[i][zoneAttacker] != INVALID_PLAYER_ID)
	    {
			GangZoneFlashForPlayer(playerid, gZone[i][zoneId], SET_ALPHA(gTeam[GetPlayerTeam(gZone[i][zoneAttacker])][teamColor], 100));
		}
	}

	return 1;
}

Menu_Show(playerid, menuid, header[], models[], labels[][], selection_color, size = sizeof(models))
{
	if (pMenu[playerid] != -1)
	{
	    Menu_Hide(playerid);
	}

	if (size > 6 * 4)
	{
	    size = 6 * 4;
	}

	TextDrawSetString(txtMenu[0], header);
	TextDrawShowForPlayer(playerid, txtMenu[0]);

	PlayerTextDrawSetString(playerid, ptxtMenu[0], header);
	PlayerTextDrawShow(playerid, ptxtMenu[0]);

	for (new i; i < size; i++)
	{
	    TextDrawSetPreviewModel(txtMenu[i + 1], models[i]);
	    TextDrawSetPreviewRot(txtMenu[i + 1], 0.000000, 0.000000, -50.000000, 1.000000);
	    TextDrawShowForPlayer(playerid, txtMenu[i + 1]);

	    PlayerTextDrawSetString(playerid, ptxtMenu[i + 1], labels[i]);
	    PlayerTextDrawShow(playerid, ptxtMenu[i + 1]);
	}

	SelectTextDraw(playerid, selection_color);

	pMenu[playerid] = menuid;
	pMenuTick[playerid] = GetTickCount();
}

Menu_EditRot(playerid, listitem, model, Float:x, Float:y, Float:z, Float:zoom)
{
	if (listitem > 6 * 4)
	{
	    return;
	}

	TextDrawSetPreviewModel(txtMenu[listitem + 1], model);
	TextDrawSetPreviewRot(txtMenu[listitem + 1], x, y, z, zoom);
	TextDrawShowForPlayer(playerid, txtMenu[listitem + 1]);
}

Menu_Hide(playerid)
{
	TextDrawHideForPlayer(playerid, txtMenu[0]);
	PlayerTextDrawHide(playerid, ptxtMenu[0]);

	for (new i; i < 6 * 4; i++)
	{
	    TextDrawHideForPlayer(playerid, txtMenu[i + 1]);
	    PlayerTextDrawHide(playerid, ptxtMenu[i + 1]);
	}

	pMenu[playerid] = -1;
	CancelSelectTextDraw(playerid);
}

QuickSort_Pair(array[][2], bool:desc, left, right)
{
	#define PAIR_FIST (0)
	#define PAIR_SECOND (1)

	new
		tempLeft = left,
		tempRight = right,
		pivot = array[(left + right) / 2][PAIR_FIST],
		tempVar
	;
	while (tempLeft <= tempRight)
	{
	    if (desc)
	    {
			while (array[tempLeft][PAIR_FIST] > pivot)
			{
				tempLeft++;
			}
			while (array[tempRight][PAIR_FIST] < pivot)
			{
				tempRight--;
			}
		}
	    else
	    {
			while (array[tempLeft][PAIR_FIST] < pivot)
			{
				tempLeft++;
			}
			while (array[tempRight][PAIR_FIST] > pivot)
			{
				tempRight--;
			}
		}

		if (tempLeft <= tempRight)
		{
			tempVar = array[tempLeft][PAIR_FIST];
		 	array[tempLeft][PAIR_FIST] = array[tempRight][PAIR_FIST];
		 	array[tempRight][PAIR_FIST] = tempVar;

			tempVar = array[tempLeft][PAIR_SECOND];
			array[tempLeft][PAIR_SECOND] = array[tempRight][PAIR_SECOND];
			array[tempRight][PAIR_SECOND] = tempVar;

			tempLeft++;
			tempRight--;
		}
	}
	if (left < tempRight)
	{
		QuickSort_Pair(array, desc, left, tempRight);
	}
	if (tempLeft < right)
	{
		QuickSort_Pair(array, desc, tempLeft, right);
	}

	#undef PAIR_FIST
	#undef PAIR_SECOND
}

IsTeamFull(teamid)
{
	new count[MAX_TEAMS][2];
	foreach (new i : Player)
	{
	    if (0 <= GetPlayerTeam(i) < MAX_TEAMS)
	    {
	        count[GetPlayerTeam(i)][0]++;
	        count[GetPlayerTeam(i)][1] = GetPlayerTeam(i);
	    }
	}

    QuickSort_Pair(count, true, 0, MAX_TEAMS - 1);

    if (count[0][0] < count[1][0] + 2 && count[1][0] == teamid)
    {
        return false;
    }
    else if (count[0][0] > count[1][0] + 2 && count[0][0] == teamid)
    {
        return true;
    }

	return false;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if (pMenu[playerid] != -1)
	{
		if (clickedid == Text:INVALID_TEXT_DRAW)
		{
		    OnPlayerMenuResponse(playerid, pMenu[playerid], 0, 0);
		    Menu_Hide(playerid);

		    return 1;
		}
		else if (GetTickCount() - pMenuTick[playerid] > 500)
		{
		    for (new i; i < 6 * 4; i++)
			{
			    if (clickedid == txtMenu[i + 1])
			    {
			        OnPlayerMenuResponse(playerid, pMenu[playerid], 1, i);
		    		Menu_Hide(playerid);

		    		return 1;
			    }
			}
		}
	}
	if (pInClass[playerid])
	{
	    new buf[150];

		if (clickedid == txtTeam[1] || clickedid == txtTeam[2])
		{
			if (IsTeamFull(0))
			{
			    return SendClientMessage(playerid, COLOR_TOMATO, "The team is full.");
			}

		    pTeam[playerid] = 0;
		    SelectPlayerTeam(playerid, 0);

		    PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);

		    SetPlayerSkin(playerid, gTeam[0][teamSkin]);
		    SetPlayerColor(playerid, gTeam[0][teamColor]);

	    	SetPlayerCameraPos(playerid, gTeam[0][teamCam][0], gTeam[0][teamCam][1], gTeam[0][teamCam][2]);
	        SetPlayerCameraLookAt(playerid, gTeam[0][teamCam2][0], gTeam[0][teamCam2][1], gTeam[0][teamCam2][2], CAMERA_MOVE);
			SetPlayerPos(playerid, gTeam[0][teamCam3][0], gTeam[0][teamCam3][1], gTeam[0][teamCam3][2]);
			SetPlayerFacingAngle(playerid, gTeam[0][teamCam3][3]);

		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		   	format(buf, sizeof(buf), "Team selected: %s", gTeam[0][teamName]);
		    SendClientMessage(playerid, COLOR_WHITE, buf);
		    SendClientMessage(playerid, COLOR_WHITE, "Select a class and spawn");
		}
		else if (clickedid == txtTeam[4] || clickedid == txtTeam[5])
		{
			if (IsTeamFull(1))
			{
			    return SendClientMessage(playerid, COLOR_TOMATO, "The team is full.");
			}

		    pTeam[playerid] = 1;
		    SelectPlayerTeam(playerid, 1);

		    PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);

		    SetPlayerSkin(playerid, gTeam[1][teamSkin]);
		    SetPlayerColor(playerid, gTeam[1][teamColor]);

	    	SetPlayerCameraPos(playerid, gTeam[1][teamCam][0], gTeam[1][teamCam][1], gTeam[1][teamCam][2]);
	        SetPlayerCameraLookAt(playerid, gTeam[1][teamCam2][0], gTeam[1][teamCam2][1], gTeam[1][teamCam2][2], CAMERA_MOVE);
			SetPlayerPos(playerid, gTeam[1][teamCam3][0], gTeam[1][teamCam3][1], gTeam[1][teamCam3][2]);
			SetPlayerFacingAngle(playerid, gTeam[1][teamCam3][3]);

		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		   	format(buf, sizeof(buf), "Team selected: %s", gTeam[1][teamName]);
		    SendClientMessage(playerid, COLOR_WHITE, buf);
		    SendClientMessage(playerid, COLOR_WHITE, "Select a class and spawn");
		}
		else if (clickedid == txtTeam[7] || clickedid == txtTeam[8])
		{
			if (IsTeamFull(2))
			{
			    return SendClientMessage(playerid, COLOR_TOMATO, "The team is full.");
			}

		    pTeam[playerid] = 2;
		    SelectPlayerTeam(playerid, 2);

		    PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);

		    SetPlayerSkin(playerid, gTeam[2][teamSkin]);
		    SetPlayerColor(playerid, gTeam[2][teamColor]);

	    	SetPlayerCameraPos(playerid, gTeam[2][teamCam][0], gTeam[2][teamCam][1], gTeam[2][teamCam][2]);
	        SetPlayerCameraLookAt(playerid, gTeam[2][teamCam2][0], gTeam[2][teamCam2][1], gTeam[2][teamCam2][2], CAMERA_MOVE);
			SetPlayerPos(playerid, gTeam[2][teamCam3][0], gTeam[2][teamCam3][1], gTeam[2][teamCam3][2]);
			SetPlayerFacingAngle(playerid, gTeam[2][teamCam3][3]);

		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		   	format(buf, sizeof(buf), "Team selected: %s", gTeam[2][teamName]);
		    SendClientMessage(playerid, COLOR_WHITE, buf);
		    SendClientMessage(playerid, COLOR_WHITE, "Select a class and spawn");
		}
		else if (clickedid == txtTeam[10] || clickedid == txtTeam[11])
		{
			if (IsTeamFull(3))
			{
			    return SendClientMessage(playerid, COLOR_TOMATO, "The team is full.");
			}

		    pTeam[playerid] = 3;
		    SelectPlayerTeam(playerid, 3);

		    PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);

		    SetPlayerSkin(playerid, gTeam[3][teamSkin]);
		    SetPlayerColor(playerid, gTeam[3][teamColor]);

	    	SetPlayerCameraPos(playerid, gTeam[3][teamCam][0], gTeam[3][teamCam][1], gTeam[3][teamCam][2]);
	        SetPlayerCameraLookAt(playerid, gTeam[3][teamCam2][0], gTeam[3][teamCam2][1], gTeam[3][teamCam2][2], CAMERA_MOVE);
			SetPlayerPos(playerid, gTeam[3][teamCam3][0], gTeam[3][teamCam3][1], gTeam[3][teamCam3][2]);
			SetPlayerFacingAngle(playerid, gTeam[3][teamCam3][3]);

		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		   	format(buf, sizeof(buf), "Team selected: %s", gTeam[3][teamName]);
		    SendClientMessage(playerid, COLOR_WHITE, buf);
		    SendClientMessage(playerid, COLOR_WHITE, "Select a class and spawn");
		}
		else if (clickedid == txtTeam[13] || clickedid == txtTeam[14])
		{
			if (IsTeamFull(4))
			{
			    return SendClientMessage(playerid, COLOR_TOMATO, "The team is full.");
			}

		    pTeam[playerid] = 4;
		    SelectPlayerTeam(playerid, 4);

		    PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);

		    SetPlayerSkin(playerid, gTeam[4][teamSkin]);
		    SetPlayerColor(playerid, gTeam[4][teamColor]);

	    	SetPlayerCameraPos(playerid, gTeam[4][teamCam][0], gTeam[4][teamCam][1], gTeam[4][teamCam][2]);
	        SetPlayerCameraLookAt(playerid, gTeam[4][teamCam2][0], gTeam[4][teamCam2][1], gTeam[4][teamCam2][2], CAMERA_MOVE);
			SetPlayerPos(playerid, gTeam[4][teamCam3][0], gTeam[4][teamCam3][1], gTeam[4][teamCam3][2]);
			SetPlayerFacingAngle(playerid, gTeam[4][teamCam3][3]);

		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		   	format(buf, sizeof(buf), "Team selected: %s", gTeam[4][teamName]);
		    SendClientMessage(playerid, COLOR_WHITE, buf);
		    SendClientMessage(playerid, COLOR_WHITE, "Select a class and spawn");
		}
		else if (clickedid == txtTeam[16] || clickedid == txtTeam[17])
		{
			if (IsTeamFull(5))
			{
			    return SendClientMessage(playerid, COLOR_TOMATO, "The team is full.");
			}

		    pTeam[playerid] = 5;
		    SelectPlayerTeam(playerid, 5);

		    PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);

		    SetPlayerSkin(playerid, gTeam[5][teamSkin]);
		    SetPlayerColor(playerid, gTeam[5][teamColor]);

	    	SetPlayerCameraPos(playerid, gTeam[5][teamCam][0], gTeam[5][teamCam][1], gTeam[5][teamCam][2]);
	        SetPlayerCameraLookAt(playerid, gTeam[5][teamCam2][0], gTeam[5][teamCam2][1], gTeam[5][teamCam2][2], CAMERA_MOVE);
			SetPlayerPos(playerid, gTeam[5][teamCam3][0], gTeam[5][teamCam3][1], gTeam[5][teamCam3][2]);
			SetPlayerFacingAngle(playerid, gTeam[5][teamCam3][3]);

		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		   	format(buf, sizeof(buf), "Team selected: %s", gTeam[5][teamName]);
		    SendClientMessage(playerid, COLOR_WHITE, buf);
		    SendClientMessage(playerid, COLOR_WHITE, "Select a class and spawn");
		}
		else if (clickedid == txtTeam[19] || clickedid == txtTeam[20])
		{
			if (IsTeamFull(6))
			{
			    return SendClientMessage(playerid, COLOR_TOMATO, "The team is full.");
			}

		    pTeam[playerid] = 6;
		    SelectPlayerTeam(playerid, 6);

		    PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);

		    SetPlayerSkin(playerid, gTeam[6][teamSkin]);
		    SetPlayerColor(playerid, gTeam[6][teamColor]);

	    	SetPlayerCameraPos(playerid, gTeam[6][teamCam][0], gTeam[6][teamCam][1], gTeam[6][teamCam][2]);
	        SetPlayerCameraLookAt(playerid, gTeam[6][teamCam2][0], gTeam[6][teamCam2][1], gTeam[6][teamCam2][2], CAMERA_MOVE);
			SetPlayerPos(playerid, gTeam[6][teamCam3][0], gTeam[6][teamCam3][1], gTeam[6][teamCam3][2]);
			SetPlayerFacingAngle(playerid, gTeam[6][teamCam3][3]);

		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		   	format(buf, sizeof(buf), "Team selected: %s", gTeam[6][teamName]);
		    SendClientMessage(playerid, COLOR_WHITE, buf);
		    SendClientMessage(playerid, COLOR_WHITE, "Select a class and spawn");
		}
		else if (clickedid == txtClass[1] || clickedid == txtClass[2])
		{
		    PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);

		    pClass[playerid] = 0;
		    pInClass[playerid] = false;
			SpawnPlayer(playerid);

			CancelSelectTextDraw(playerid);

		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		    format(buf, sizeof(buf), "Class selected: %s", gClass[0][className]);
		    SendClientMessage(playerid, COLOR_WHITE, buf);
		}
		else if (clickedid == txtClass[4] || clickedid == txtClass[5])
		{
		    if (pRank[playerid] < gClass[1][classRank])
		    {
				for (new i; i < 50; i++)
			    {
			        SendClientMessage(playerid, COLOR_WHITE, " ");
			    }
			    format(buf, sizeof(buf), "You must be atleast Rank %i (%i score) to become a %s.", gClass[1][classRank], gRank[1][rankScore], gClass[1][className]);
		    	SendClientMessage(playerid, COLOR_TOMATO, buf);

		    	return 1;
		    }

		    PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);

		    pClass[playerid] = 1;
		    pInClass[playerid] = false;
			SpawnPlayer(playerid);

			CancelSelectTextDraw(playerid);

		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		    format(buf, sizeof(buf), "Class selected: %s", gClass[1][className]);
		    SendClientMessage(playerid, COLOR_WHITE, buf);
		}
		else if (clickedid == txtClass[7] || clickedid == txtClass[8])
		{
		    if (pRank[playerid] < gClass[2][classRank])
		    {
				for (new i; i < 50; i++)
			    {
			        SendClientMessage(playerid, COLOR_WHITE, " ");
			    }
			    format(buf, sizeof(buf), "You must be atleast Rank %i (%i score) to become a %s.", gClass[2][classRank], gRank[2][rankScore], gClass[2][className]);
		    	SendClientMessage(playerid, COLOR_TOMATO, buf);

		    	return 1;
		    }

		    PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);

		    pClass[playerid] = 2;
		    pInClass[playerid] = false;
			SpawnPlayer(playerid);

			CancelSelectTextDraw(playerid);

		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		    format(buf, sizeof(buf), "Class selected: %s", gClass[2][className]);
		    SendClientMessage(playerid, COLOR_WHITE, buf);
		}
		else if (clickedid == txtClass[10] || clickedid == txtClass[11])
		{
		    if (pRank[playerid] < gClass[3][classRank])
		    {
				for (new i; i < 50; i++)
			    {
			        SendClientMessage(playerid, COLOR_WHITE, " ");
			    }
			    format(buf, sizeof(buf), "You must be atleast Rank %i (%i score) to become a %s.", gClass[3][classRank], gRank[3][rankScore], gClass[3][className]);
		    	SendClientMessage(playerid, COLOR_TOMATO, buf);

		    	return 1;
		    }

		    PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);

		    pClass[playerid] = 3;
		    pInClass[playerid] = false;
			SpawnPlayer(playerid);

			CancelSelectTextDraw(playerid);

		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		    format(buf, sizeof(buf), "Class selected: %s", gClass[3][className]);
		    SendClientMessage(playerid, COLOR_WHITE, buf);
		}
		else if (clickedid == txtClass[13] || clickedid == txtClass[14])
		{
		    if (pRank[playerid] < gClass[4][classRank])
		    {
				for (new i; i < 50; i++)
			    {
			        SendClientMessage(playerid, COLOR_WHITE, " ");
			    }
			    format(buf, sizeof(buf), "You must be atleast Rank %i (%i score) to become a %s.", gClass[4][classRank], gRank[5][rankScore], gClass[4][className]);
		    	SendClientMessage(playerid, COLOR_TOMATO, buf);

		    	return 1;
		    }

		    PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);

		    pClass[playerid] = 4;
		    pInClass[playerid] = false;
			SpawnPlayer(playerid);

			CancelSelectTextDraw(playerid);

		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		    format(buf, sizeof(buf), "Class selected: %s", gClass[4][className]);
		    SendClientMessage(playerid, COLOR_WHITE, buf);
		}
		else if (clickedid == txtClass[16] || clickedid == txtClass[17])
		{
		    if (pRank[playerid] < gClass[5][classRank])
		    {
				for (new i; i < 50; i++)
			    {
			        SendClientMessage(playerid, COLOR_WHITE, " ");
			    }
			    format(buf, sizeof(buf), "You must be atleast Rank %i (%i score) to become a %s.", gClass[5][classRank], gRank[8][rankScore], gClass[5][className]);
		    	SendClientMessage(playerid, COLOR_TOMATO, buf);

		    	return 1;
		    }

		    PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);

		    pClass[playerid] = 5;
		    pInClass[playerid] = false;
			SpawnPlayer(playerid);

			CancelSelectTextDraw(playerid);

		    for (new i; i < 50; i++)
		    {
		        SendClientMessage(playerid, COLOR_WHITE, " ");
		    }
		    format(buf, sizeof(buf), "Class selected: %s", gClass[5][className]);
		    SendClientMessage(playerid, COLOR_WHITE, buf);
		}
		else
		{
			SelectTextDraw(playerid, 0xFF0000FF);
		}
	}
	return 1;
}

GetModelWeaponID(weaponid)
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

forward OnPlayerMenuResponse(playerid, menuid, response, listitem);
public 	OnPlayerMenuResponse(playerid, menuid, response, listitem)
{
	switch (menuid)
	{
	    case MENU_ID_DUEL_WEAPON:
		{
		    if (response)
		    {
	            if (! IsPlayerConnected(pDuel[playerid][duelPlayer]))
				{
					return SendClientMessage(playerid, COLOR_TOMATO, "The opponent is not connected.");
				}
				else if (pInClass[pDuel[playerid][duelPlayer]])
				{
				    return SendClientMessage(playerid, COLOR_TOMATO, "The opponent isn't spawned.");
				}
				else if (GetPlayerState(pDuel[playerid][duelPlayer]) == PLAYER_STATE_WASTED)
				{
					return SendClientMessage(playerid, COLOR_TOMATO, "The opponent isn't spawned.");
				}
				else if (pDuel[pDuel[playerid][duelPlayer]][duelActive])
				{
					return SendClientMessage(playerid, COLOR_TOMATO, "The opponent is already in a duel.");
				}
				else if (pDuel[playerid][duelBet] > 0 && GetPlayerMoney(playerid) < pDuel[playerid][duelBet])
				{
					return SendClientMessage(playerid, COLOR_TOMATO, "You yourself don't have that much bet money.");
				}
				else if (pDuel[playerid][duelBet] > 0 && GetPlayerMoney(pDuel[playerid][duelPlayer]) < pDuel[playerid][duelBet])
				{
					return SendClientMessage(playerid, COLOR_TOMATO, "The opponent don't have that much bet money.");
				}

		        new weaponid = GetModelWeaponID(menuDuelModels[listitem]);
				pDuel[playerid][duelWeapon] = weaponid;
				pDuel[pDuel[playerid][duelPlayer]][duelWeapon] = weaponid;

				new weapon[35];
				GetWeaponName(pDuel[playerid][duelWeapon], weapon, sizeof(weapon));
				new string[150];
				format(string, sizeof(string), "You have sent a duel request to %s(%i) with weapon %s, bet $%i.", ReturnPlayerName(pDuel[playerid][duelPlayer]), pDuel[playerid][duelPlayer], weapon, pDuel[playerid][duelBet]);
			    SendClientMessage(playerid, COLOR_YELLOW, string);

		        format(string, sizeof(string), ""WHITE"You have recieved a duel request from "SAMP_BLUE"%s(%i)"WHITE".\nThe winner to kill with a "SAMP_BLUE"%s "WHITE"will get "SAMP_BLUE"$%i "WHITE"(as per duel request).\n\nClick 'ACCEPT' if you want to fight for the bet money or simply click 'CANCEL'.", ReturnPlayerName(playerid), playerid, weapon, pDuel[playerid][duelBet]);
		        ShowPlayerDialog(pDuel[playerid][duelPlayer], DIALOG_ID_DUEL, DIALOG_STYLE_MSGBOX, "Duel request!", string, "Accept", "Cancel");
		    }
		}
		case MENU_ID_PERSONAL_WEAPON:
	   	{
	   	    if (response)
	   	    {
				new weaponid = GetModelWeaponID(menuDuelModels[listitem]);

				new weapon[35];
				GetWeaponName(weaponid, weapon, sizeof(weapon));
				new buf[150];
				strcat(buf, "You have selected weapon ");
				strcat(buf, weapon);
				switch (pIdx[playerid])
				{
					case 1:
					{
						yoursql_set_field_int(SQL:0, "users/weapon1", yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid)), weaponid);

						strcat(buf, " as your Personal waepon 1.");
						SendClientMessage(playerid, COLOR_GREEN, buf);
		   			}
					case 2:
					{
						yoursql_set_field_int(SQL:0, "users/weapon2", yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid)), weaponid);

						strcat(buf, " as your Personal waepon 2.");
						SendClientMessage(playerid, COLOR_GREEN, buf);
		   			}
					case 3:
					{
						yoursql_set_field_int(SQL:0, "users/weapon3", yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid)), weaponid);

						strcat(buf, " as your Personal waepon 3.");
						SendClientMessage(playerid, COLOR_GREEN, buf);
		   			}
		   		}
		   	}
	   	}
	    case MENU_ID_INVENTORY:
	    {
	        if (response)
	        {
				switch (listitem)
				{
				    case 0:
				    {
				        if (pInventory[playerid][listitem] >= 7)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't carry more than 7 medickits.");
				        }

				        if (GetPlayerMoney(playerid) < 1500)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        pInventory[playerid][listitem] += 1;
				        GivePlayerMoney(playerid, -1500);

				        new buf[150];
				        format(buf, sizeof(buf), "You have bought a Medickit for -$1500 (Total %i medickits).", pInventory[playerid][listitem]);
				        SendClientMessage(playerid, COLOR_YELLOW, buf);
				        SendClientMessage(playerid, COLOR_YELLOW, "Use /med to get 35# health anywhere.");
				    }
				    case 1:
				    {
				        if (pInventory[playerid][listitem] >= 2)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't carry more than 2 nettraps.");
				        }

				        if (GetPlayerMoney(playerid) < 15000)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        pInventory[playerid][listitem] += 1;
				        GivePlayerMoney(playerid, -15000);

				        new buf[150];
				        format(buf, sizeof(buf), "You have bought a Nettrap for -$15000 (Total %i nettraps).", pInventory[playerid][listitem]);
				        SendClientMessage(playerid, COLOR_YELLOW, buf);
				        SendClientMessage(playerid, COLOR_YELLOW, "Use /trap to place a nettrap on the ground.");
				    }
				    case 2:
				    {
				        if (pInventory[playerid][listitem] >= 3)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't carry more than 3 dynamites.");
				        }

				        if (GetPlayerMoney(playerid) < 10000)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        pInventory[playerid][listitem] += 1;
				        GivePlayerMoney(playerid, -10000);

				        new buf[150];
				        format(buf, sizeof(buf), "You have bought a Dynamite for -$10000 (Total %i dynamites).", pInventory[playerid][listitem]);
				        SendClientMessage(playerid, COLOR_YELLOW, buf);
				        SendClientMessage(playerid, COLOR_YELLOW, "Use /dynamite to place a dynamite/bomb on the ground and '/det [id]' to detonate it.");
				    }
				    case 3:
				    {
				        if (pInventory[playerid][listitem])
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't carry more than 1 ammunation box.");
				        }

				        if (GetPlayerMoney(playerid) < 5000)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        pInventory[playerid][listitem] = 1;
				        GivePlayerMoney(playerid, -5000);

				        SendClientMessage(playerid, COLOR_YELLOW, "You have bought an Ammunation Box for -$5000.");
				        SendClientMessage(playerid, COLOR_YELLOW, "Use /ammo to retrieve ammunation for all your weapons.");
				    }
				    case 4:
				    {
				        if (pInventory[playerid][listitem] >= 3)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't carry more than 2 drug bundle.");
				        }

				        if (GetPlayerMoney(playerid) < 3500)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        pInventory[playerid][listitem] += 1;
				        GivePlayerMoney(playerid, -3500);

				        new buf[150];
				        format(buf, sizeof(buf), "You have bought a Drug Bundle for -$3500 (Total %i drug capsules).", pInventory[playerid][listitem]);
				        SendClientMessage(playerid, COLOR_YELLOW, buf);
				        SendClientMessage(playerid, COLOR_YELLOW, "Use /drug to use the drug capsule.");
				    }
				    case 5:
				    {
				        if (pInventory[playerid][listitem])
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't carry more than 1 camouflage.");
				        }

				        if (GetPlayerMoney(playerid) < 7500)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        pInventory[playerid][listitem] = 1;
				        GivePlayerMoney(playerid, -7500);

				        SendClientMessage(playerid, COLOR_YELLOW, "You have bought a Camouflage for -$7500.");
 						SendClientMessage(playerid, COLOR_YELLOW, "Use /camo to wear or unwear the camouflage.");
				    }
				    case 6:
				    {
				        if (pInventory[playerid][listitem])
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't carry more than 1 musicbox.");
				        }

				        if (GetPlayerMoney(playerid) < 25000)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        pInventory[playerid][listitem] = 1;
				        GivePlayerMoney(playerid, -25000);

				        SendClientMessage(playerid, COLOR_YELLOW, "You have bought a Musicbox for -$25000.");
 						SendClientMessage(playerid, COLOR_YELLOW, "Use /music to place a stereo on the ground and stream music.");
				    }
				    case 7:
				    {
				        if (pInventory[playerid][listitem] >= 3)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't carry more than 3 spikestrips.");
				        }

				        if (GetPlayerMoney(playerid) < 10000)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        pInventory[playerid][listitem] += 1;
				        GivePlayerMoney(playerid, -10000);

				        new buf[150];
				        format(buf, sizeof(buf), "You have bought a Spikestrips -$10000 (Total %i spikestrips).", pInventory[playerid][listitem]);
				        SendClientMessage(playerid, COLOR_YELLOW, buf);
				        SendClientMessage(playerid, COLOR_YELLOW, "Use /spike to plant spike strips on the ground.");
				    }
				    case 8:
				    {
				        if (pInventory[playerid][listitem] >= 3)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't carry more than 3 landmines.");
				        }

				        if (GetPlayerMoney(playerid) < 20000)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        pInventory[playerid][listitem] += 1;
				        GivePlayerMoney(playerid, -20000);

				        new buf[150];
				        format(buf, sizeof(buf), "You have bought a Landmine -$20000 (Total %i landmines).", pInventory[playerid][listitem]);
				        SendClientMessage(playerid, COLOR_YELLOW, buf);
				        SendClientMessage(playerid, COLOR_YELLOW, "Use /mine to plant landmines on the ground.");
				    }
				    case 9:
				    {
				        if (pInventory[playerid][listitem])
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't carry more than 1 protection jacket.");
				        }

				        if (GetPlayerMoney(playerid) < 25000)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        pInventory[playerid][listitem] = 1;
				        GivePlayerMoney(playerid, -25000);

				        SendClientMessage(playerid, COLOR_YELLOW, "You have bought a Protection Jacket -$25000.");
				        SendClientMessage(playerid, COLOR_YELLOW, "Use /jacket to wear or unwear the Protection Jacket. Drops the amount of damage taken at TORSO and CHEST bodyparts.");
				    }
				    case 10:
				    {
				        if (pInventory[playerid][listitem])
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't carry more than 1 Protection Mask.");
				        }

				        if (GetPlayerMoney(playerid) < 15000)
				        {
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        pInventory[playerid][listitem] = 1;
				        GivePlayerMoney(playerid, -15000);

				        SendClientMessage(playerid, COLOR_YELLOW, "You have bought a Protection Mask -$15000.");
				        SendClientMessage(playerid, COLOR_YELLOW, "Use /mask to wear or unwear the Protection Mask. Drops the amount of damage taken at HEAD bodypart.");
				    }
				}
	        }
	        else
	        {
	            cmd_buy(playerid);
	        }
	    }
	    case MENU_ID_WEAPONS:
	    {
	        if (response)
	        {
				new cost;
				switch (listitem)
				{
				    case 0: cost = 1500;
				    case 1: cost = 1500;
				    case 2: cost = 3000;
				    case 3: cost = 2500;
				    case 4: cost = 2500;
				    case 5: cost = 3000;
				    case 6: cost = 5000;
				    case 7: cost = 3500;
				    case 8: cost = 6500;
				    case 9: cost = 7000;
				    case 10: cost = 10000;
				    case 11: cost = 9500;
				    case 12: cost = 8500;
				    case 13: cost = 7000;
				    case 14: cost = 7500;
				    case 15: cost = 8500;
				    case 16: cost = 8500;
				    case 17: cost = 3500;
				    case 18: cost = 10000;
				    case 19: cost = 3500;
				    case 20: cost = 4000;
				    case 21: cost = 4500;
				    case 22: cost = 10000;
				    case 23: cost = 3500;
				}
				if (GetPlayerMoney(playerid) < cost)
				{
				    Menu_Show(playerid, MENU_ID_WEAPONS, "Shop/Weapons list (/buy):", menuWeaponModels, menuWeaponLabels, 0xFF0000FF);
			        Menu_EditRot(playerid, 0, menuWeaponModels[0], 0.0, 0.0, -50.0, 0.5);
					Menu_EditRot(playerid, 3, menuWeaponModels[3], 0.0, 0.0, 0.0, 0.5);
					Menu_EditRot(playerid, 4, menuWeaponModels[4], 0.0, 0.0, 0.0, 0.5);
					Menu_EditRot(playerid, 5, menuWeaponModels[5], 0.0, 0.0, 0.0, 0.5);
					Menu_EditRot(playerid, 6, menuWeaponModels[6], 0.0, 0.0, -50.0, 0.5);
					Menu_EditRot(playerid, 7, menuWeaponModels[7], 0.0, 0.0, -50.0, 0.5);
					Menu_EditRot(playerid, 8, menuWeaponModels[8], 0.0, 0.0, -50.0, 0.5);
					Menu_EditRot(playerid, 19, menuWeaponModels[19], 0.0, 0.0, -50.0, 1.3);
					Menu_EditRot(playerid, 20, menuWeaponModels[20], 0.0, 0.0, -50.0, 1.3);
					Menu_EditRot(playerid, 21, menuWeaponModels[21], 0.0, 0.0, -50.0, 1.3);
					Menu_EditRot(playerid, 22, menuWeaponModels[22], 0.0, 0.0, -50.0, 1.3);
					return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				}

				new ammo;
				switch (listitem)
				{
				    case 0: ammo = 1;
				    case 1: ammo = 1;
				    case 2: ammo = 1;
				    case 3: ammo = 1;
				    case 4: ammo = 1;
				    case 5: ammo = 1;
				    case 6: ammo = 100;
				    case 7: ammo = 100;
				    case 8: ammo = 100;
				    case 9: ammo = 100;
				    case 10: ammo = 100;
				    case 11: ammo = 100;
				    case 12: ammo = 200;
				    case 13: ammo = 200;
				    case 14: ammo = 200;
				    case 15: ammo = 200;
				    case 16: ammo = 200;
				    case 17: ammo = 100;
				    case 18: ammo = 100;
				    case 19: ammo = 1;
				    case 20: ammo = 1;
				    case 21: ammo = 200;
				    case 22: ammo = 100;
				    case 23: ammo = 1;
				}

				new weaponid = GetModelWeaponID(menuWeaponModels[listitem]);
				GivePlayerWeapon(playerid, weaponid, ammo);
				GivePlayerMoney(playerid, -cost);

				new weapon[35];
				GetWeaponName(weaponid, weapon, sizeof(weapon));
				new buf[150];
				format(buf, sizeof(buf), "You have bought %s with %i ammo for -$%i.", weapon, ammo, cost);
				SendClientMessage(playerid, COLOR_YELLOW, buf);
	        }
	        else
	        {
	            cmd_buy(playerid);
	        }
	    }
	}

	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	new info[700];
 	new SQLRow:rowid = yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid));

	switch (dialogid)
	{
	    case DIALOG_ID_REGISTER:
	    {
			if (response)
			{
			    if (strlen(inputtext) < 4 || strlen(inputtext) > 30)
				{
					info[0] = EOS;
					strcat(info, ""WHITE"Welcome "RED"");
					strcat(info, ReturnPlayerName(playerid));
					strcat(info, " "WHITE", you are new to the server!\n\n");
					strcat(info, "Before registering, please read the main rules:\n");
					strcat(info, ""RED"1. "WHITE"No cheats/hacks/invalid ways of playing.\n");
					strcat(info, ""RED"2. "WHITE"No insulting in main chat, respect all.\n");
					strcat(info, ""RED"3. "WHITE"Read all the rules in /rules.\n\n");
					strcat(info, "Now please insert a password and register this account!");

				    ShowPlayerDialog(playerid, DIALOG_ID_REGISTER, DIALOG_STYLE_PASSWORD, "Account registration", info, "Register", "Quit");

				    SendClientMessage(playerid, COLOR_TOMATO, "You have entered invalid password, the length must be betweem 4 - 30 characters.");

					return 1;
				}

				new hash[128];
				SHA256_PassHash(inputtext, "aafGEsq13", hash, sizeof(hash));

				yoursql_set_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid));
				rowid = yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid));
				yoursql_set_field(SQL:0, "users/ip", rowid, ReturnPlayerIp(playerid));
				yoursql_set_field(SQL:0, "users/password", rowid, hash);
			 	yoursql_set_field_int(SQL:0, "users/score", rowid, 15);
			 	yoursql_set_field_int(SQL:0, "users/money", rowid, 50000);

                new date[3];
				getdate(date[2], date[1], date[0]);

				new month[15];
				switch (date[1])
				{
				    case 1: month = "January";
				    case 2: month = "Feburary";
				    case 3: month = "March";
				    case 4: month = "April";
				    case 5: month = "May";
				    case 6: month = "June";
				    case 7: month = "July";
				    case 8: month = "August";
				    case 9: month = "September";
				    case 10: month = "October";
				    case 11: month = "November";
				    case 12: month = "December";
				}

				new register_on[25];
				format(register_on, sizeof(register_on), "%02d %s, %d", date[0], month, date[2]);
			 	yoursql_set_field(SQL:0, "users/register_on", rowid, register_on);

			 	pStats[playerid][userAdmin] = 0;
			 	pStats[playerid][userPremium] = false;
			 	pStats[playerid][userKills] = 0;
			 	pStats[playerid][userDeaths] = 0;

			    for (new i; i < 50; i++)
			    {
			        SendClientMessage(playerid, COLOR_WHITE, " ");
			    }
			    SendClientMessage(playerid, COLOR_GREEN, "Great job! now you are an official member World War IV community.");
			    SendClientMessage(playerid, COLOR_GREEN, "You have completeted your registration, you may start playing now or setup your account (/settings).");
			    SendClientMessage(playerid, COLOR_YELLOW, "+$50000 and +15 score");

			    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

			    GivePlayerMoney(playerid, 50000);
			    SetPlayerScore(playerid, 15);

				pRank[playerid] = GetPlayerRank(playerid);

			    info[0] = EOS;
			    strcat(info, ""WHITE"Email registration is an important part but "YELLOW"OPTIONAL"WHITE".\n");
			    strcat(info, "Email registration will help you to recover your password when you loose it, in a very safe way.\n\n");
			    strcat(info, "Please insert a valid "GREEN"Email Adress "WHITE"to register with this account.");
			    ShowPlayerDialog(playerid, DIALOG_ID_EMAIL, DIALOG_STYLE_INPUT, "Account registration/Register an email", info, "Done", "Cancel");
			}
			else
			{
			    Kick(playerid);
			}
	    }
	    case DIALOG_ID_LOGIN:
	    {
			if (response)
			{
			    new pass[128];
			    yoursql_get_field(SQL:0, "users/password", rowid, pass);
       			new hash[128];
			    SHA256_PassHash(inputtext, "aafGEsq13", hash, sizeof(hash));

			    if (hash[0] && strcmp(hash, pass))
				{
					info[0] = EOS;
					strcat(info, ""WHITE"Welcome back "RED"");
					strcat(info, ReturnPlayerName(playerid));
					strcat(info, " "WHITE", you are already registerd!\n\n");
					strcat(info, "If you any problem logging in this account, you can do the following:\n");
					strcat(info, ""RED"1. "WHITE"Press 'PROBLEM' and enter the email registered with this account.\n");
					strcat(info, ""RED"2. "WHITE"Press 'PROBLEM' and click 'QUIT' there if this is not your account.\n\n");
					strcat(info, "Else, please insert your password and login this account!");

				    ShowPlayerDialog(playerid, DIALOG_ID_LOGIN, DIALOG_STYLE_PASSWORD, "Account login required", info, "Login", "Problem?");

				    SendClientMessage(playerid, COLOR_TOMATO, "You have entered unmatching password, please try again or quit.");

					return 1;
				}

				yoursql_set_field(SQL:0, "users/ip", rowid, ReturnPlayerIp(playerid));

			    for (new i; i < 50; i++)
			    {
			        SendClientMessage(playerid, COLOR_WHITE, " ");
			    }
			    SendClientMessage(playerid, COLOR_GREEN, "Login session was successfully completed, thanks for joining us back!");
			    SendClientMessage(playerid, COLOR_GREEN, "If you want to change your account settings, type /settings.");

			    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

			 	pStats[playerid][userAdmin] = yoursql_get_field_int(SQL:0, "users/admin", rowid);
			 	pStats[playerid][userPremium] = bool:yoursql_get_field_int(SQL:0, "users/vip", rowid);
			 	pStats[playerid][userKills] = yoursql_get_field_int(SQL:0, "users/kills", rowid);
			 	pStats[playerid][userDeaths] = yoursql_get_field_int(SQL:0, "users/deaths", rowid);
			 	pStats[playerid][userZones] = yoursql_get_field_int(SQL:0, "users/zones", rowid);
			 	pStats[playerid][userHeadshots] = yoursql_get_field_int(SQL:0, "users/headshots", rowid);

				ResetPlayerMoney(playerid);
			    GivePlayerMoney(playerid, yoursql_get_field_int(SQL:0, "users/money", rowid));
			    SetPlayerScore(playerid, yoursql_get_field_int(SQL:0, "users/score", rowid));

				pRank[playerid] = GetPlayerRank(playerid);

				pLogged[playerid] = true;
				SpawnPlayer(playerid);
			}
			else
			{
			    ShowPlayerDialog(playerid, DIALOG_ID_FORGOT_PASSWORD, DIALOG_STYLE_INPUT, "Account login required/Forgot password", ""WHITE"Seems like you have lost your password, don't worry!\n\nIf this is your account, you must have registered an "RED"email"WHITE" while sign-up.\nEnter that email below and open it in your browser, retrieve the "RED"password reset key "WHITE" and follow the steps given.", "Confirm", "Quit");
			}
	    }
	    case DIALOG_ID_EMAIL:
	    {
	        if (response)
	        {
	            if (! IsValidEmail(inputtext))
	            {
	                info[0] = EOS;
				    strcat(info, ""WHITE"Email registration is an important part but "YELLOW"OPTIONAL"WHITE".\n");
				    strcat(info, "Email registration will help you to recover your password when you loose it, in a very safe way.\n\n");
				    strcat(info, "Please insert a valid "GREEN"Email Adress "WHITE"to register with this account.");
				    ShowPlayerDialog(playerid, DIALOG_ID_EMAIL, DIALOG_STYLE_INPUT, "Account registration/Register an email", info, "Done", "Cancel");

	                SendClientMessage(playerid, COLOR_TOMATO, "You have entered an invalid email address.");

					return 1;
	            }

				yoursql_set_field(SQL:0, "users/email", rowid, inputtext);

			    for (new i; i < 50; i++)
			    {
			        SendClientMessage(playerid, COLOR_WHITE, " ");
			    }
				info[0] = EOS;
				strcat(info, "You have successfully registered your account email "WHITE"");
				strcat(info, inputtext);
				strcat(info, " "GREEN".");
				SendClientMessage(playerid, COLOR_GREEN, info);
				SendClientMessage(playerid, COLOR_GREEN, "If you ever wish to change it, you can do so by /email.");

			    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

				pLogged[playerid] = true;
				SpawnPlayer(playerid);
	        }
	        else
	        {
				pLogged[playerid] = true;
				SpawnPlayer(playerid);

			    SendClientMessage(playerid, COLOR_YELLOW, "You have canceled email registration. If you want to add an email later, you can do so by /email.");
	        }
	    }
	    case DIALOG_ID_FORGOT_PASSWORD:
	    {
	        if (response)
	        {
	            new email[100];
				yoursql_get_field(SQL:0, "users/email", rowid, email);
				if (! IsValidEmail(inputtext) || strcmp(email, inputtext))
				{
			    	ShowPlayerDialog(playerid, DIALOG_ID_FORGOT_PASSWORD, DIALOG_STYLE_INPUT, "Account login required/Forgot password", ""WHITE"Seems like you have lost your password, do't worry!\n\nIf this is your account, you must have registered an "RED"email"WHITE" while sign-up.\nEnter that email below and open it in your browser, retrieve the "RED"password reset key "WHITE" and follow the steps given.", "Confirm", "Quit");

	                SendClientMessage(playerid, COLOR_TOMATO, "You have entered an unmatching email address.");

					return 1;
				}

			    for (new i; i < 50; i++)
			    {
			        SendClientMessage(playerid, COLOR_WHITE, " ");
			    }
				SendClientMessage(playerid, COLOR_GREEN, "You have entered the correct email address, You will be recieving one shortly (also check your Spam section).");
				SendClientMessage(playerid, COLOR_GREEN, "Once you have recieved the mail, use the reset password key and login with it (we have given steps in mail as well).");

			    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

			    Kick(playerid);
	        }
	        else
	        {
			    Kick(playerid);
	        }
	    }
	    case DIALOG_ID_DISGUIZE:
	    {
	        if (response)
	        {
				SetPlayerSkin(playerid, gTeam[listitem][teamSkin]);
				SetPlayerColor(playerid, gTeam[listitem][teamColor]);

				pDisguizeKits[playerid]--;

				new buf[150];
				format(buf, sizeof(buf), "You have disguized to team {%06x}%s"WHITE".", gTeam[listitem][teamColor] >>> 8, gTeam[listitem][teamName]);
				SendClientMessage(playerid, COLOR_WHITE, buf);
				format(buf, sizeof(buf), "You are now left with %i disguize kits.", pDisguizeKits[playerid]);
				SendClientMessage(playerid, COLOR_WHITE, buf);
	        }
	    }
	    case DIALOG_ID_SHOP:
	    {
	        if (response)
	        {
				switch (listitem)
				{
				    case 0:
				    {
				        if (GetPlayerMoney(playerid) < 5000)
				        {
							cmd_buy(playerid);
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        new Float:hp;
				        GetPlayerHealth(playerid, hp);
				        if (hp >= 100.0)
				        {
							cmd_buy(playerid);
							return SendClientMessage(playerid, COLOR_TOMATO, "You already have full health.");
				        }

				        SetPlayerHealth(playerid, 100.0);
				        GivePlayerMoney(playerid, -5000);

				        SendClientMessage(playerid, COLOR_YELLOW, "You have bought full health for -$5000.");
				    }
				    case 1:
				    {
				        if (GetPlayerMoney(playerid) < 6500)
				        {
							cmd_buy(playerid);
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        new Float:ar;
				        GetPlayerArmour(playerid, ar);
				        if (ar >= 100.0)
				        {
							cmd_buy(playerid);
				            return SendClientMessage(playerid, COLOR_TOMATO, "You already have full armour.");
				        }

				        SetPlayerArmour(playerid, 100.0);
				        GivePlayerMoney(playerid, -6500);

				        SendClientMessage(playerid, COLOR_YELLOW, "You have bought full armour for -$6500.");
				    }
				    case 2:
				    {
				        if (GetPlayerMoney(playerid) < 7500)
				        {
							cmd_buy(playerid);
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        if (pHasHelmet[playerid])
				        {
							cmd_buy(playerid);
				            return SendClientMessage(playerid, COLOR_TOMATO, "You already have a protection helmet.");
				        }

                        SetPlayerAttachedObject(playerid,0,18638,2,0.173000,0.024999,-0.003000,0.000000,0.000000,0.000000,1.000000,1.000000,1.000000); //skin 102
                    	pHasHelmet[playerid] = true;
				        GivePlayerMoney(playerid, -7500);

				        SendClientMessage(playerid, COLOR_YELLOW, "You have bought protection helmet for -$7500.");
				        SendClientMessage(playerid, COLOR_YELLOW, "You can notice a yellow helmet over your head, it will protect your from headshots.");
				    }
				    case 3:
				    {
				        if (GetPlayerMoney(playerid) < 5000)
				        {
							cmd_buy(playerid);
							return SendClientMessage(playerid, COLOR_TOMATO, "You can't afford this item.");
				        }

				        if (pHasMask[playerid])
				        {
							cmd_buy(playerid);
				            return SendClientMessage(playerid, COLOR_TOMATO, "You already have a gasmask.");
				        }

                    	SetPlayerAttachedObject(playerid, 1, 19472, 2, -0.022000, 0.137000, 0.018999, 3.899994, 85.999961, 92.999984, 0.923999, 1.141000, 1.026999);
                    	pHasMask[playerid] = true;
				        GivePlayerMoney(playerid, -5000);

				        SendClientMessage(playerid, COLOR_YELLOW, "You have bought gasmask for -$5000.");
				        SendClientMessage(playerid, COLOR_YELLOW, "You can notice a mask on your face, it will protect your from teargas.");
				    }
				    case 4:
				    {
						Menu_Show(playerid, MENU_ID_INVENTORY, "Shop/Inventory items (/buy):", menuInventoryModels, menuInventoryLabels, 0xFF0000FF);
					}
				    case 5:
				    {
						Menu_Show(playerid, MENU_ID_WEAPONS, "Shop/Weapons list (/buy):", menuWeaponModels, menuWeaponLabels, 0xFF0000FF);
				        Menu_EditRot(playerid, 0, menuWeaponModels[0], 0.0, 0.0, -50.0, 0.5);
						Menu_EditRot(playerid, 3, menuWeaponModels[3], 0.0, 0.0, 0.0, 0.5);
						Menu_EditRot(playerid, 4, menuWeaponModels[4], 0.0, 0.0, 0.0, 0.5);
						Menu_EditRot(playerid, 5, menuWeaponModels[5], 0.0, 0.0, 0.0, 0.5);
						Menu_EditRot(playerid, 6, menuWeaponModels[6], 0.0, 0.0, -50.0, 0.5);
						Menu_EditRot(playerid, 7, menuWeaponModels[7], 0.0, 0.0, -50.0, 0.5);
						Menu_EditRot(playerid, 8, menuWeaponModels[8], 0.0, 0.0, -50.0, 0.5);
						Menu_EditRot(playerid, 19, menuWeaponModels[19], 0.0, 0.0, -50.0, 1.3);
						Menu_EditRot(playerid, 20, menuWeaponModels[20], 0.0, 0.0, -50.0, 1.3);
						Menu_EditRot(playerid, 21, menuWeaponModels[21], 0.0, 0.0, -50.0, 1.3);
						Menu_EditRot(playerid, 22, menuWeaponModels[22], 0.0, 0.0, -50.0, 1.3);
				    }
				    case 6:
				    {
				        if (! yoursql_get_field_int(SQL:0, "users/vip", yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid))))
				        {
				            return SendClientMessage(playerid, COLOR_TOMATO, "Only a donator/premium user can access this room.");
				        }
				    }
				}
	        }
	    }
		case DIALOG_ID_CLASS:
	    {
	    	if (response)
	        {
	            if (pRank[playerid] < gClass[listitem][classRank])
	            {
	                info[0] = EOS;
	                format(info, sizeof(info), "You must be rank %d+ to be a %s.", gClass[listitem][classRank], gClass[listitem][className]);
	                return SendClientMessage(playerid, COLOR_TOMATO, info);
	            }

	            if (pClass[playerid] == listitem)
	            {
					return SendClientMessage(playerid, COLOR_TOMATO, "You already using that class.");
	            }

				info[0] = EOS;
    			format(info, sizeof(info), "Class seleted: %s", gClass[listitem][className]);
       			SendClientMessage(playerid, COLOR_WHITE, " ");
       			SendClientMessage(playerid, COLOR_WHITE, info);
       			SendClientMessage(playerid, COLOR_WHITE, " ");

       			pClass[playerid] = listitem;
       			SpawnPlayer(playerid);
			}
   		}
		case DIALOG_ID_DUEL:
		{
			if(! response)
		    {
		        SendClientMessage(pDuel[playerid][duelPlayer], COLOR_TOMATO, "The duel request was CANCELED by the opponenet.");

		        pDuel[pDuel[playerid][duelPlayer]][duelActive] = false;
		        pDuel[pDuel[playerid][duelPlayer]][duelPlayer] = INVALID_PLAYER_ID;
		        pDuel[pDuel[playerid][duelPlayer]][duelWeapon] = 0;
		        pDuel[pDuel[playerid][duelPlayer]][duelBet] = 0;

		        pDuel[playerid][duelActive] = false;
		        pDuel[playerid][duelPlayer] = INVALID_PLAYER_ID;
		        pDuel[playerid][duelWeapon] = 0;
		        pDuel[playerid][duelBet] = 0;
		    }
		    else
		    {
	            if (! IsPlayerConnected(pDuel[playerid][duelPlayer]))
				{
					return SendClientMessage(playerid, COLOR_TOMATO, "The opponent is not connected.");
				}
				else if (pInClass[pDuel[playerid][duelPlayer]])
				{
				    return SendClientMessage(playerid, COLOR_TOMATO, "The opponent isn't spawned.");
				}
				else if (GetPlayerState(pDuel[playerid][duelPlayer]) == PLAYER_STATE_WASTED)
				{
					return SendClientMessage(playerid, COLOR_TOMATO, "The opponent isn't spawned.");
				}
				else if (pDuel[pDuel[playerid][duelPlayer]][duelActive])
				{
					return SendClientMessage(playerid, COLOR_TOMATO, "The opponent is already in a duel.");
				}
				else if (pDuel[playerid][duelBet] > 0 && GetPlayerMoney(playerid) < pDuel[playerid][duelBet])
				{
					return SendClientMessage(playerid, COLOR_TOMATO, "You yourself don't have that much bet money.");
				}
				else if (pDuel[playerid][duelBet] > 0 && GetPlayerMoney(pDuel[playerid][duelPlayer]) < pDuel[playerid][duelBet])
				{
					return SendClientMessage(playerid, COLOR_TOMATO, "The opponent don't have that much bet money.");
				}

				ResetPlayerWeapons(playerid);
				GivePlayerWeapon(playerid, pDuel[playerid][duelWeapon], 99999);
				SetPlayerHealth(playerid, 100.0);
				SetPlayerArmour(playerid, 100.0);
				NotifyPlayer(playerid, "The duel has started!", 3500);
				SetPlayerTeam(playerid, NO_TEAM);
				SetPlayerSkin(playerid, random(312));
				SetPlayerColor(playerid, COLOR_WHITE);
				SetPlayerPos(playerid, 1353.407,2188.155,11.02344);
				SetPlayerInterior(playerid, 0);
				SetPlayerVirtualWorld(playerid, playerid);
				PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

				ResetPlayerWeapons(pDuel[playerid][duelPlayer]);
				GivePlayerWeapon(pDuel[playerid][duelPlayer], pDuel[playerid][duelWeapon], 99999);
				SetPlayerHealth(pDuel[playerid][duelPlayer], 100.0);
				SetPlayerArmour(pDuel[playerid][duelPlayer], 100.0);
				NotifyPlayer(pDuel[playerid][duelPlayer], "The duel has started!", 3500);
				SetPlayerTeam(pDuel[playerid][duelPlayer], NO_TEAM);
				SetPlayerSkin(pDuel[playerid][duelPlayer], random(312));
				SetPlayerColor(pDuel[playerid][duelPlayer], COLOR_WHITE);
				SetPlayerPos(pDuel[playerid][duelPlayer], 1346.255,2142.843,11.01563);
				SetPlayerInterior(pDuel[playerid][duelPlayer], 0);
				SetPlayerVirtualWorld(pDuel[playerid][duelPlayer], playerid);
				PlayerPlaySound(pDuel[playerid][duelPlayer], 1057, 0.0, 0.0, 0.0);

				new weapon[35];
				GetWeaponName(pDuel[playerid][duelWeapon], weapon, sizeof(weapon));
				new string[144];
				format(string, sizeof(string), "DUEL: A duel has begun b/w %s(%i) and %s(%i) [weapon: %s, bet: $%i]", ReturnPlayerName(playerid), playerid, ReturnPlayerName(pDuel[playerid][duelPlayer]), pDuel[playerid][duelPlayer], weapon, pDuel[playerid][duelBet]);
				SendClientMessageToAll(COLOR_ORANGE_RED, string);

		        pDuel[pDuel[playerid][duelPlayer]][duelActive] = true;
		        pDuel[playerid][duelActive] = true;
				return 1;
		    }
	   	}
	   	case DIALOG_ID_SPAWN:
	   	{
	   	    if (response)
	   	    {
		   	    if (listitem == 0)
		   	    {
					pSpawn[playerid] = sizeof(gZone);
		   	        return SendClientMessage(playerid, COLOR_GREEN, "You have changed your spawn to: Team base.");
	    		}
	    		else if (gZone[listitem - 1][zoneOwner] != GetPlayerTeam(playerid))
				{
				    return SendClientMessage(playerid, COLOR_TOMATO, "Your team doesn't own that zone.");
	  			}
	    		else if (gZone[listitem - 1][zoneAttacker] != INVALID_PLAYER_ID)
				{
				    return SendClientMessage(playerid, COLOR_TOMATO, "The zone is currently under attack.");
	  			}

	  			new buf[150];
				strcat(buf, "You have changed your spawn to: ");
				strcat(buf, gZone[listitem - 1][zoneName]);
				strcat(buf, ".");
				SendClientMessage(playerid, COLOR_GREEN, buf);

				pSpawn[playerid] = listitem;
			}
	   	}
	   	case DIALOG_ID_BUILD:
	   	{
	   	    if (response)
	   	    {
	   	        if (IsValidVehicle(pBuildMode[playerid]))
	    		{
	    		    DestroyVehicle(pBuildMode[playerid]);
	    		}

		   	    ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 0.5, 0, 1, 1, 1, 0, 1);
		   	    pBuildMode[playerid] = SetTimerEx("OnPlayerBuild", 5000, false, "ii", playerid, listitem);

		   	    new buf[150];
		   	    format(buf, sizeof(buf), "* %s %s(%i) is creating an RC vehicle.", gClass[3][className], ReturnPlayerName(playerid), playerid);

				new Float:x, Float:y, Float:z;
				GetPlayerPos(playerid, x, y, z);
				foreach (new i : Player)
		   	    {
		   	        if (IsPlayerInRangeOfPoint(i, 50.0, x, y, z))
		   	        {
		   	            SendClientMessage(i, COLOR_GREY, buf);
		   	        }
		   	    }
	   	    }
	   	}
	   	case DIALOG_ID_AIRSTRIKE:
	   	{
	   	    if (response)
	   	    {
		   	    if (GetPlayerMoney(playerid) < 15000)
				{
				    return SendClientMessage(playerid, COLOR_TOMATO, "You cannot afford an airstrike worth $15000.");
				}

				if (gettime() - pAirstrike[playerid][asLastStrike] < 60 * 5)
				{
				    return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 5 minutes after using an airstrike.");
				}

				new Float:x, Float:y, Float:z;
				GetPlayerPos(playerid, x, y, z);
				CA_FindZ_For2DCoord(x, y, z);

				CallAirstrike(playerid, x, y, z);
				pAirstrike[playerid][asLastStrike] = gettime();
				pAirstrike[playerid][asCalled] = true;

				GivePlayerMoney(playerid, -15000);

				SendClientMessage(playerid, COLOR_YELLOW, "You have requested an airstrike at your position, get cover.");
				SendClientMessage(playerid, COLOR_YELLOW, "The strike will happen in 5 seconds at the flare position.");
				SendClientMessage(playerid, COLOR_TOMATO, "The airstrike cost you -$15000.");

				new text[150];
				format(text, sizeof(text), "** (%i) %s have requested an airstrike.", playerid, ReturnPlayerName(playerid));

				foreach (new i : Player)
				{
					if (IsPlayerInRangeOfPoint(i, 10.0, x, y, z))
					{
					    SendClientMessage(i, COLOR_GREY, text);
					}
				}
	   		}
	   	}
	   	case DIALOG_ID_CAREPACK:
	   	{
	   	    if (response)
	   	    {
		   		if (GetPlayerMoney(playerid) < 20000)
				{
				    return SendClientMessage(playerid, COLOR_TOMATO, "You cannot afford a carepack worth $20000.");
				}

				if (gettime() - pCarepack[playerid][cpLastDrop] < 60 * 2)
				{
				    return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 2 minutes after calling a carepack.");
				}

				new Float:x, Float:y, Float:z;
				GetPlayerPos(playerid, x, y, z);
				CA_FindZ_For2DCoord(x, y, z);

				if (IsValidDynamicObject(pCarepack[playerid][cpPlaneObject]))
				{
				    DestroyDynamicObject(pCarepack[playerid][cpPlaneObject]);
				}
				if (IsValidDynamicObject(pCarepack[playerid][cpObject]))
				{
				    DestroyDynamicObject(pCarepack[playerid][cpObject]);
				}
				if (IsValidDynamic3DTextLabel(pCarepack[playerid][cpLabel]))
				{
				    DestroyDynamic3DTextLabel(pCarepack[playerid][cpLabel]);
				}

				CallCarepack(playerid, x, y, z);
				pCarepack[playerid][cpLastDrop] = gettime();
				pCarepack[playerid][cpCalled] = true;

				GivePlayerMoney(playerid, -20000);

				SendClientMessage(playerid, COLOR_YELLOW, "You have requested a carepack at your position, you shall type /lootpack to gather items from it.");
				SendClientMessage(playerid, COLOR_YELLOW, "The package will be dropped within 15 seconds at the flare position.");
				SendClientMessage(playerid, COLOR_TOMATO, "The carepack cost you -$20000.");

				new text[150];
				format(text, sizeof(text), "** (%i) %s have requested a carepack.", playerid, ReturnPlayerName(playerid));

				foreach (new i : Player)
				{
					if (IsPlayerInRangeOfPoint(i, 10.0, x, y, z))
					{
					    SendClientMessage(i, COLOR_GREY, text);
					}
				}
			}
		}
		case DIALOG_ID_REPORTS:
	    {
	        if (response)
	        {
	            if (listitem == 0)
				{
				    for (new i; i < MAX_REPORTS; i++)
				    {
					    gReport[i][rAgainst][0] = EOS;
				        gReport[i][rAgainstId] = INVALID_PLAYER_ID;
				        gReport[i][rBy][0] = EOS;
				        gReport[i][rById] = INVALID_PLAYER_ID;
				        gReport[i][rReason][0] = EOS;
				        gReport[i][rTime][0] = EOS;
				        gReport[i][rChecked] = false;
					}

					new buf[150];
					format(buf, sizeof(buf), "All reports were cleared by admin %s(%i).", ReturnPlayerName(playerid), playerid);
					foreach (new i : Player)
					{
					    if (pStats[i][userAdmin])
					    {
							SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
						}
					}
				}
				else
				{
				    new i = listitem + 1;

					if (! gReport[i][rChecked] && IsPlayerConnected(gReport[i][rById]))
					{
					    new buf[150];
						format(buf, sizeof(buf), "Admin %s(%i) is checking your report.", ReturnPlayerName(playerid), playerid);
						SendClientMessage(gReport[i][rById], COLOR_DODGER_BLUE, buf);
					}

					format(info, sizeof(info), ""WHITE"You are now checking the report of "GREEN"%s(%i)"WHITE".\n\n"TOMATO"Against: "WHITE"%s(%i)\n"TOMATO"Reason:\n"WHITE"%s\n"TOMATO"Time: "WHITE"%s", gReport[i][rBy], gReport[i][rById], gReport[i][rAgainst], gReport[i][rAgainstId], gReport[i][rReason], gReport[i][rTime]);
					ShowPlayerDialog(playerid, DIALOG_ID_REPORTS_PAGE, DIALOG_STYLE_MSGBOX, "Report info.:", info, "Back", "Close");
				}
	        }
	    }
	    case DIALOG_ID_REPORTS_PAGE:
	    {
	        if (response)
	        {
	            cmd_reports(playerid);
	        }
	    }
	    case DIALOG_ID_MUTE_LIST:
	    {
	        if (response)
	        {
	            new idx;
	            foreach (new i : Player)
				{
				    if (pStats[i][userMuteTime] != -1)
				    {
				        if (idx == listitem)
				        {
							format(info, sizeof(info), ""WHITE"Click "GREEN"UNMUTE "WHITE"to lift mute from player.\n\nPlayer selected: "TOMATO"%s(%i) "WHITE"(unmute will be auto lifted in %i seconds)", ReturnPlayerName(i), i, pStats[i][userMuteTime]);
							ShowPlayerDialog(playerid, DIALOG_ID_UNMUTE, DIALOG_STYLE_MSGBOX, "Unmute confirmation:", info, "Unmute", "Cancel");

							pStats[playerid][userIdx] = i;
				            break;
				        }
				        idx++;
				    }
				}
	        }
	    }
	    case DIALOG_ID_UNMUTE:
	    {
	        if (response)
	        {
	            new params[5];
	            valstr(params, pStats[playerid][userIdx]);
	            cmd_unmute(playerid, params);
	        }
	    }
	    case DIALOG_ID_JAILED_LIST:
	    {
	        if (response)
	        {
	            new idx;
	            foreach (new i : Player)
				{
				    if (pStats[i][userJailTime] != -1)
				    {
				        if (idx == listitem)
				        {
							format(info, sizeof(info), ""WHITE"Click "GREEN"UNJAIL "WHITE"to lift jail from player.\n\nPlayer selected: "TOMATO"%s(%i) "WHITE"(jail will be auto lifted in %i seconds)", ReturnPlayerName(i), i, pStats[i][userJailTime]);
							ShowPlayerDialog(playerid, DIALOG_ID_UNJAIL, DIALOG_STYLE_MSGBOX, "Unjail confirmation:", info, "Unjail", "Cancel");

							pStats[playerid][userIdx] = i;
				            break;
				        }
				        idx++;
				    }
				}
	        }
	    }
	    case DIALOG_ID_UNJAIL:
	    {
	        if (response)
	        {
	            new params[5];
	            valstr(params, pStats[playerid][userIdx]);
	            cmd_unjail(playerid, params);
	        }
	    }
	    case DIALOG_ID_AUTO_LOGIN:
	    {
	        if (response)
	        {
	            yoursql_set_field_int(SQL:0, "users/auto_login", yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid)), 1);
	            SendClientMessage(playerid, COLOR_GREEN, "AUTOLOGIN: You have enabled your auto login feature.");
	        }
	        else
	        {
	            yoursql_set_field_int(SQL:0, "users/auto_login", yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid)), 0);
	            SendClientMessage(playerid, COLOR_GREEN, "AUTOLOGIN: You have disabled your auto login feature.");
	        }
	    }
	    case DIALOG_ID_MUSICBOX:
	    {
			if (response)
			{
			    if (strlen(inputtext) < 7)
			    {
			        ShowPlayerDialog(playerid, DIALOG_ID_MUSICBOX, DIALOG_STYLE_INPUT, "Music box streamer:", ""WHITE"Insert a "LIME"URL. "WHITE"to start streaming.\n\nPress "RED"RANDOM "WHITE"to stream random radio station.", "Stream", "Random");
			        return SendClientMessage(playerid, COLOR_TOMATO, "The URL length must be grater than 6.");
			    }

			    pMusicBoxURL[playerid][0] = EOS;
				strcat(pMusicBoxURL[playerid], inputtext);

				new Float:x, Float:y, Float:z;
				GetDynamicObjectPos(pMusicBoxObject[playerid], x, y, z);

	        	format(info, sizeof(info), "[Music box] Streaming started, Hosted by %s(%i).", ReturnPlayerName(playerid), playerid);

				foreach (new i : Player)
				{
				    PlayAudioStreamForPlayer(i, inputtext, x, y, z);

				    if (IsPlayerInDynamicArea(i, pMusicBoxAreaid[playerid]))
				    {
						SendClientMessage(i, COLOR_ORANGE, info);
				    }
				}
			}
			else
			{
			    switch (random(5))
				{
					case 0: pMusicBoxURL[playerid] = "http://yp.shoutcast.com/sbin/tunein-station.xspf?id=389248";
					case 1: pMusicBoxURL[playerid] = "http://yp.shoutcast.com/sbin/tunein-station.pls?id=475565";
					case 2: pMusicBoxURL[playerid] = "http://yp.shoutcast.com/sbin/tunein-station.pls?id=17998";
					case 3: pMusicBoxURL[playerid] = "http://yp.shoutcast.com/sbin/tunein-station.pls?id=1009184";
					case 4: pMusicBoxURL[playerid] = "http://yp.shoutcast.com/sbin/tunein-station.pls?id=630803";
				}

				new Float:x, Float:y, Float:z;
				GetDynamicObjectPos(pMusicBoxObject[playerid], x, y, z);

	        	format(info, sizeof(info), "[Music box] Streaming started, Hosted by %s(%i).", ReturnPlayerName(playerid), playerid);

				foreach (new i : Player)
				{
				    PlayAudioStreamForPlayer(i, pMusicBoxURL[playerid], x, y, z);

				    if (IsPlayerInDynamicArea(i, pMusicBoxAreaid[playerid]))
				    {
						SendClientMessage(i, COLOR_ORANGE, info);
				    }
				}
			}
	    }
	    case DIALOG_ID_EVENT:
	    {
	        if (response)
	        {
	            SendClientMessage(playerid, COLOR_AQUA, "Event system is currently under development.");
	            /*switch (listitem)
	            {
	                case 0:
	                {
						new buf[150];
					 	format(buf, sizeof(buf), "Admin %s(%i) has organized a TDM. event - COPS VS. TERRORISTS.", ReturnPlayerName(playerid), playerid);
						SendClientMessageToAll(COLOR_AQUA, buf);
						SendClientMessageToAll(COLOR_AQUA, "Type /join to join the event.");

						SendRconCommand("loadfs event_tdm0");
	                }
	            }*/
	        }
	    }
	    case DIALOG_ID_HELP:
	    {
	        if (response)
		    {
				switch (listitem)
				{
				    case 0:
					{
						strcat(info, ""WHITE"Server main mode: "GREEN"Team deathmatch\n\n");
						strcat(info, ""WHITE"The server revolves around the LV. Desert where 7 teams batle each other to get more bonus.\n");
						strcat(info, "Bonus is mainly collected from "RED"Kills, Capture Zones, Zone Weapons/Benifits and Team work"WHITE".\n\n");
						strcat(info, "If in case you are tired of teaming, we also provide some minigames side by to let users participate.\n");
						strcat(info, "These minigames include different modes such like "SAMP_BLUE"Deathmatch, One In The Chamber etc"WHITE".");

						ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "About gamemode:", info, "Close", "");
					}
					case 1:
					{
						strcat(info, ""WHITE"Server Owner: "SAMP_BLUE"Gammix\n\n");
						strcat(info, "Please visit and register on our website "SAMP_BLUE"www.worldwargaming.com"WHITE".\n\n");
						strcat(info, ""LIME"Credits:\n");
						strcat(info, ""WHITE"- Scripter: "SAMP_BLUE"Gammix\n");
						strcat(info, ""WHITE"- Developer: "SAMP_BLUE"Gammix\n");
						strcat(info, ""WHITE"- Database: "DODGER_BLUE"YourSQL [SQLite]\n");
						strcat(info, ""WHITE"- iZCMD by "SAMP_BLUE"Zeex and Yashas\n");
						strcat(info, ""WHITE"- Sscanf2 & Foreach by "SAMP_BLUE"Y_Less\n");
						strcat(info, ""WHITE"- Streamer by "SAMP_BLUE"Incognito\n");
						strcat(info, ""WHITE"- ColAndreads by "SAMP_BLUE"Pottus, Slice & Chris\n");
						strcat(info, ""WHITE"- Regular Expression by "SAMP_BLUE"Fro1sha\n");
						strcat(info, ""WHITE"- Progress bar v2 by "SAMP_BLUE"Toribo & Southclaw\n");
						strcat(info, ""WHITE"- TimeStampToDate by "SAMP_BLUE"Jochemd\n");
						strcat(info, ""WHITE"- Spikestrips by "SAMP_BLUE"Stylock");

						ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "About server:", info, "Close", "");
					}
					case 2:
					{
						strcat(info, ""RED"PING "WHITE"is the biggest disturbing factor a user is offered while playing!\n\n");
						strcat(info, "Ping completely depends upon your internet or broadband connection, mostly.\n\n");
						strcat(info, ""SAMP_BLUE"Here are some steps you can do to reduce your ping:\n");
						strcat(info, ""WHITE"- Turn Off all the background applications using your bandwidth.\n");
						strcat(info, "- Turn Off all the background applications which can drop your FPS, even at a slight rate.\n");
						strcat(info, "- Disconnect all WiFi connections to your router (if any).\n");
						strcat(info, "- Restart your router if the ping is having an ordinary high value.\n");
						strcat(info, "- Make sure you get atleast 25-30 FPS while playing if your PC isn't that good!\n");
						strcat(info, "- "GREEN"Last"WHITE", if nothing works; buy a better internet connection!");

						ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "How to reduce ping?", info, "Close", "");
					}
					case 3:
					{
						strcat(info, ""WHITE"Unlocks require you to earn "GREEN"Score & Money"WHITE".\n\n");
						strcat(info, ""SAMP_BLUE"Here are some suggested ways to earn fast:\n");
						strcat(info, ""WHITE"- Capture most zones in a single spawn.\n");
						strcat(info, "- Buy inventory items to take down strong enemies.\n");
						strcat(info, "- Use sniper class (when unlocked) to capture zones, taking advantage of invisibility.\n");
						strcat(info, "- Making killing spree helps in boosting your score.\n");
						strcat(info, "- Use airstrikes (/as) when there are bulk of enemies on your way.\n");
						strcat(info, "- Sometimes zone weapons are very helpful and cheap to gather kill streaks.");

						ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "How to unlock features?", info, "Close", "");
					}
					case 4:
					{
					    cmd_donate(playerid);
					}
					case 5:
					{
						strcat(info, ""GREEN"STAFF APPLICATION OPEN\n\n");
						strcat(info, ""WHITE"Goto "SAMP_BLUE"www.worldwargaming.com "WHITE"and post your application in "SAMP_BLUE"Staff Application "WHITE"section.\n");
						strcat(info, ""WHITE"Do read the /rules before or after applying for staff.");

						ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "How to become an admin?", info, "Close", "");
					}
					case 6:
					{
						cmd_cmds(playerid);
					}
					case 7:
					{
					    cmd_rules(playerid);
					}
				}
		    }
	    }
	}

	return 1;
}

forward OnPlayerBuild(playerid, build);
public	OnPlayerBuild(playerid, build)
{
	ClearAnimations(playerid);

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	SetPlayerPos(playerid, 0.0, 0.0, 0.0);

	switch (build)
	{
		case 0:
		{
			pBuildMode[playerid] = CreateVehicle(464, x, y, z, 0.0, -1, -1, 0);
			SendClientMessage(playerid, COLOR_YELLOW, "Press FIRE button to shoot machine gun.");
		}
		case 1:
		{
			pBuildMode[playerid] = CreateVehicle(564, x, y, z, 0.0, -1, -1, 0);
			SendClientMessage(playerid, COLOR_YELLOW, "Press FIRE button to shoot rockets.");
		}
		case 2:
		{
			pBuildMode[playerid] = CreateVehicle(501, x, y, z, 0.0, -1, -1, 0);
			SetVehicleHealth(pBuildMode[playerid], 2000.0);
			SendClientMessage(playerid, COLOR_YELLOW, "Press FIRE button to drop bombs.");
		}
		case 3:
		{
			pBuildMode[playerid] = CreateVehicle(464, x, y, z, 0.0, -1, -1, 0);
			SendClientMessage(playerid, COLOR_YELLOW, "Press FIRE button to explode your vehicle (exploding near enemies will get you kills).");
		}
	}
	PutPlayerInVehicle(playerid, pBuildMode[playerid], 0);
}

GetWeaponSlot(weaponid)
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

SyncPlayer(playerid, Float:health = 0.0, Float:armour = 0.0)
{
   	new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    new Float:a;
	GetPlayerFacingAngle(playerid, a);

	new interior = GetPlayerInterior(playerid);
	new world = GetPlayerVirtualWorld(playerid);

	new weapon[13], ammo[13];
	for (new i; i < 13; i++)
	{
 		GetPlayerWeaponData(playerid, i, weapon[i], ammo[i]);
	}

	if (health == 0.0)
	{
		GetPlayerHealth(playerid, health);
	}
	if (armour == 0.0)
	{
		GetPlayerArmour(playerid, armour);
	}

	new skin = GetPlayerSkin(playerid);
	new color = GetPlayerColor(playerid);

	pSync[playerid] = true;
	SpawnPlayer(playerid);

   	SetPlayerPos(playerid, x, y, z);

	SetPlayerFacingAngle(playerid, a);

	SetPlayerInterior(playerid, interior);
	SetPlayerVirtualWorld(playerid, world);

	for (new i; i < 13; i++)
	{
	    if (weapon[i] && ammo[i])
	    {
 			GivePlayerWeapon(playerid, weapon[i], ammo[i]);
		}
	}

	SetPlayerHealth(playerid, health);
	SetPlayerArmour(playerid, armour);

	SetPlayerSkin(playerid, skin);
	SetPlayerColor(playerid, color);
}

GetWeaponModelID(weaponid)
{
	switch (weaponid)
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

stock CreateWeaponPickup(weaponid, ammo, Float:x, Float:y, Float:z, worldid = -1, interiorid = -1)
{
    for (new i; i < MAX_DROPS; i++)
    {
        if (! IsValidDynamicObject(gDropObject[i]))
        {
            gDropObject[i] = CreateDynamicObject(GetWeaponModelID(weaponid), x, y, z - 0.9, 80.0, 0.0, Float:random(180), worldid, interiorid);
		    gDropAreaid[i] = CreateDynamicCircle(x, y, 1.0, worldid, interiorid);

            gDropWeaponid[i] = weaponid;
		    gDropAmount[i] = ammo;

		    gDropTimer[i] = SetTimerEx("OnWeaponPickupDestroy", 3 * 60 * 1000, false, "i", i);

		    return;
	   	}
	}
}

forward OnWeaponPickupDestroy(i);
public  OnWeaponPickupDestroy(i)
{
	DestroyDynamicObject(gDropObject[i]);
	DestroyDynamicArea(gDropAreaid[i]);
	KillTimer(gDropTimer[i]);
}

public OnPlayerDeath(playerid, killerid, reason)
{
    pTrapped[playerid] = false;
    KillTimer(pTrappedTimer[playerid]);
	if (IsValidDynamicObject(pTrappedObject[playerid]))
	{
		DestroyDynamicObject(pTrappedObject[playerid]);
	}

	if (IsValidDynamicObject(pNetTrapObject[playerid][0]))
	{
		DestroyDynamicObject(pNetTrapObject[playerid][0]);
		DestroyDynamicArea(pNetTrapArea[playerid][0]);
		DestroyDynamic3DTextLabel(pNetTrapLabel[playerid][0]);
		KillTimer(pNetTrapTimer[playerid][0]);
	}
	if (IsValidDynamicObject(pNetTrapObject[playerid][1]))
	{
		DestroyDynamicObject(pNetTrapObject[playerid][1]);
		DestroyDynamicArea(pNetTrapArea[playerid][1]);
		DestroyDynamic3DTextLabel(pNetTrapLabel[playerid][1]);
		KillTimer(pNetTrapTimer[playerid][1]);
	}

	if (IsValidDynamicObject(pDynamiteObject[playerid][0]))
	{
		DestroyDynamicObject(pDynamiteObject[playerid][0]);
		DestroyDynamic3DTextLabel(pDynamiteLabel[playerid][0]);
	}
	if (IsValidDynamicObject(pDynamiteObject[playerid][1]))
	{
		DestroyDynamicObject(pDynamiteObject[playerid][1]);
		DestroyDynamic3DTextLabel(pDynamiteLabel[playerid][1]);
	}
	if (IsValidDynamicObject(pDynamiteObject[playerid][2]))
	{
		DestroyDynamicObject(pDynamiteObject[playerid][2]);
		DestroyDynamic3DTextLabel(pDynamiteLabel[playerid][2]);
	}

	if (IsValidDynamicObject(pLandmineObject[playerid][0]))
	{
		DestroyDynamicObject(pLandmineObject[playerid][0]);
		DestroyDynamicArea(pLandmineAreaid[playerid][0]);
		DestroyDynamic3DTextLabel(pLandmineLabel[playerid][0]);
	}
	if (IsValidDynamicObject(pLandmineObject[playerid][1]))
	{
		DestroyDynamicObject(pLandmineObject[playerid][1]);
		DestroyDynamicArea(pLandmineAreaid[playerid][1]);
		DestroyDynamic3DTextLabel(pLandmineLabel[playerid][1]);
	}
	if (IsValidDynamicObject(pLandmineObject[playerid][2]))
	{
		DestroyDynamicObject(pLandmineObject[playerid][2]);
		DestroyDynamicArea(pLandmineAreaid[playerid][2]);
		DestroyDynamic3DTextLabel(pLandmineLabel[playerid][2]);
	}

	if (IsValidDynamicObject(pMusicBoxObject[playerid]))
	{
	    DestroyDynamicObject(pMusicBoxObject[playerid]);
	    foreach (new i : Player)
	    {
	        if (IsPlayerInDynamicArea(i, pMusicBoxAreaid[playerid]))
	        {
	            StopAudioStreamForPlayer(i);
	        }
	    }
	    DestroyDynamicArea(pMusicBoxAreaid[playerid]);
	    DestroyDynamic3DTextLabel(pMusicBoxLabel[playerid]);
	}

	if (pSpikeTimer[playerid][0])
	{
		SpikeStrip_Delete(pSpikeObject[playerid][0]);
		KillTimer(pSpikeTimer[playerid][0]);
        pSpikeTimer[playerid][0] = 0;
		DestroyDynamic3DTextLabel(pSpikeLabel[playerid][0]);
	}
	if (pSpikeTimer[playerid][1])
	{
		SpikeStrip_Delete(pSpikeObject[playerid][1]);
		KillTimer(pSpikeTimer[playerid][1]);
        pSpikeTimer[playerid][1] = 0;
		DestroyDynamic3DTextLabel(pSpikeLabel[playerid][1]);
	}
	if (pSpikeTimer[playerid][2])
	{
		SpikeStrip_Delete(pSpikeObject[playerid][2]);
		KillTimer(pSpikeTimer[playerid][2]);
        pSpikeTimer[playerid][2] = 0;
		DestroyDynamic3DTextLabel(pSpikeLabel[playerid][2]);
	}

	if (IsValidDynamicObject(pCarepack[playerid][cpPlaneObject]))
	{
	    DestroyDynamicObject(pCarepack[playerid][cpPlaneObject]);
	}
	if (IsValidDynamicObject(pCarepack[playerid][cpObject]))
	{
	    DestroyDynamicObject(pCarepack[playerid][cpObject]);
	}
	if (IsValidDynamic3DTextLabel(pCarepack[playerid][cpLabel]))
	{
	    DestroyDynamic3DTextLabel(pCarepack[playerid][cpLabel]);
	}

    if (IsValidVehicle(pBuildMode[playerid]))
    {
        DestroyVehicle(pBuildMode[playerid]);
    }
    pBuildMode[playerid] = 0;

    pSync[playerid] = false;

    Menu_Hide(playerid);

    if (pDuel[playerid][duelActive])
	{
	    GivePlayerMoney(pDuel[playerid][duelPlayer], pDuel[playerid][duelBet]);

	    new weapon[35];
	    GetWeaponName(pDuel[playerid][duelWeapon], weapon, sizeof(weapon));
	    new string[144];
	    format(string, sizeof(string), "DUEL: %s(%i) have won the duel against opponent %s(%i) [weapon: %s, bet: $%i].", ReturnPlayerName(pDuel[playerid][duelPlayer]), pDuel[playerid][duelPlayer], ReturnPlayerName(playerid), playerid, weapon, pDuel[playerid][duelBet]);
	    SendClientMessageToAll(COLOR_YELLOW, string);
	    format(string, sizeof(string), "You won the duel against your opponent %s(%i) [weapon: %s, bet: $%i].", ReturnPlayerName(playerid), playerid, weapon, pDuel[playerid][duelBet]);
	    SendClientMessage(pDuel[playerid][duelPlayer], COLOR_GREEN, string);
	    format(string, sizeof(string), "You lost the duel against your opponent %s(%i) [weapon: %s, bet: $%i].", ReturnPlayerName(pDuel[playerid][duelPlayer]), pDuel[playerid][duelPlayer], weapon, pDuel[playerid][duelBet]);
	    SendClientMessage(playerid, COLOR_TOMATO, string);

		NotifyPlayer(playerid, "You ~r~LOST ~w~~h~the duel!", 3000);
		NotifyPlayer(pDuel[playerid][duelPlayer], "You ~g~WON ~w~~h~the duel!", 3000);

		pDuel[pDuel[playerid][duelPlayer]][duelActive] = false;
	 	pDuel[pDuel[playerid][duelPlayer]][duelPlayer] = INVALID_PLAYER_ID;
		pDuel[pDuel[playerid][duelPlayer]][duelWeapon] = 0;
		pDuel[pDuel[playerid][duelPlayer]][duelBet] = 0;

		SpawnPlayer(pDuel[playerid][duelPlayer]);

		pDuel[playerid][duelActive] = false;
	 	pDuel[playerid][duelPlayer] = INVALID_PLAYER_ID;
		pDuel[playerid][duelWeapon] = 0;
		pDuel[playerid][duelBet] = 0;

		return 1;
	}

	if (0 <= GetPlayerTeam(playerid) < MAX_TEAMS)
	{
	    switch (GetVehicleModel(GetPlayerVehicleID(killerid)))
	    {
	   		case 447, 520, 425, 432:
	   		{
				if (GetPlayerGangZone(playerid) == gTeam[GetPlayerTeam(playerid)][teamBaseId])
				{
				    SendClientMessage(playerid, COLOR_YELLOW, "You were killed in a base rape, your stats are recovered.");
				    SendClientMessage(playerid, COLOR_YELLOW, "You are now continuing on your same position.");
                    SyncPlayer(playerid, gRank[pRank[playerid]][rankHealth], gRank[pRank[playerid]][rankArmour]);

				    SendClientMessage(killerid, COLOR_TOMATO, "Base rape isn't allowed. You have been respawned as a punishment.");
				    SpawnPlayer(killerid);
					return 1;
				}
			}
		}

		new Float:x, Float:y, Float:z;
		GetPlayerPos(playerid, x, y, z);

		new w, a;
		for (new i; i < 13; i++)
		{
			GetPlayerWeaponData(playerid, i, w, a);

			switch (w)
			{
			    case 1..15:
			    {
					CreateWeaponPickup(w, a, x, y + random(4), z, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));
					GivePlayerWeapon(playerid, w, -1);
			    }
			    case 16..37, 39..46:
			    {
					CreateWeaponPickup(w, a, x, y + random(4), z, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));
					SetPlayerAmmo(playerid, w, 0);
			    }
			}
		}
	}

    pProtectTick[playerid] = 0;
    DestroyDynamic3DTextLabel(pProtectLabel[playerid]);

	new buf[150];
	format(buf, sizeof(buf), "%s\n%s", gRank[pRank[playerid]][rankName], gClass[pClass[playerid]][className]);
	UpdateDynamic3DTextLabelText(pRankLabel[playerid], gTeam[pTeam[playerid]][teamColor], buf);

	if (pKiller[playerid][0] != INVALID_PLAYER_ID)
	{
	    killerid = pKiller[playerid][0];
		pKiller[playerid][0] = INVALID_PLAYER_ID;

	    reason = pKiller[playerid][1];
		pKiller[playerid][1] = 0;
	}

	SendDeathMessage(killerid, playerid, reason);

	new money;
	money = 500 + random(500);
	GivePlayerMoney(playerid, -money);

	format(buf, sizeof(buf), "You have lost -$%i for the death.", money);
	SendClientMessage(playerid, COLOR_TOMATO, buf);

	if (killerid != INVALID_PLAYER_ID && reason)
	{
		if (pClass[killerid] == 5)
		{
			SendClientMessage(playerid, COLOR_TOMATO, "You were killed by a spy, beware next time.");
		}
		else
		{
			format(buf, sizeof(buf), "You were killed by %s(%i).", ReturnPlayerName(killerid), killerid);
			SendClientMessage(playerid, COLOR_TOMATO, buf);
		}

		money = 1000 + random(500);
		GivePlayerMoney(killerid, money);
		SetPlayerScore(killerid, GetPlayerScore(killerid) + 1);

		format(buf, sizeof(buf), "You killed %s(%i).", ReturnPlayerName(playerid), playerid);
		SendClientMessage(killerid, COLOR_GREEN, buf);
		format(buf, sizeof(buf), "You have gained +1 score and +$%i for the kill.", money);
	    SendClientMessage(killerid, COLOR_GREEN, buf);

	    if (pStats[killerid][userPremium])
	    {
			money = 500 + random(500);
			GivePlayerMoney(killerid, money);
			SetPlayerScore(killerid, GetPlayerScore(killerid) + 1);

			format(buf, sizeof(buf), "[VIP] Extra +1 score and +$%i as premium reward!", money);
			SendClientMessage(killerid, COLOR_CYAN, buf);
		}

		new weaponid = GetPlayerWeapon(killerid);
        pWeaponsSpree[killerid][GetWeaponSlot(weaponid)]++;
	    switch (pWeaponsSpree[killerid][GetWeaponSlot(weaponid)])
	    {
	    	case 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 100, 110, 120, 150, 175, 200, 250, 300..1000:
	    	{
	    	   	new gun[35];
	    	    GetWeaponName(weaponid, gun, sizeof(gun));
    			format(buf, sizeof(buf), "%s(%i) is on a %s spree of %i kills.", ReturnPlayerName(killerid), killerid, pWeaponsSpree[killerid][GetWeaponSlot(weaponid)]);
				SendClientMessageToAll(COLOR_ORANGE_RED, buf);
				SendClientMessage(killerid, COLOR_GREEN, "Extra +2 score and +$3500 for the spree.");

				GivePlayerMoney(killerid, 3500);
				SetPlayerScore(killerid, GetPlayerScore(killerid) + 2);
	    	}
		}

		pStats[killerid][userKills]++;
		pStats[playerid][userDeaths]++;
	}

	return 1;
}

public OnPlayerUpdate(playerid)
{
	new rank = GetPlayerRank(playerid);
	if (pRank[playerid] > rank)
	{
	    new buf[150];
	    format(buf, sizeof(buf), "You ranked down to ~r~%s(%i)", gRank[rank][rankName], rank);
	    NotifyPlayer(playerid, buf, 5000);

	    format(buf, sizeof(buf), "You have just ranked down to \"%s (%i)\" from \"%s (%i)\".", gRank[rank][rankName], rank, gRank[pRank[playerid]][rankName], pRank[playerid]);
	    SendClientMessage(playerid, COLOR_TOMATO, buf);

	    SetPlayerScore(playerid, GetPlayerScore(playerid) - 5);

	    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	    pRank[playerid] = rank;
	}
	else if (pRank[playerid] < rank)
	{
	    new buf[150];
	    format(buf, sizeof(buf), "You ranked up to ~g~%s(%i)", gRank[rank][rankName], rank);
	    NotifyPlayer(playerid, buf, 5000);

	    format(buf, sizeof(buf), "You have just ranked up to \"%s (%i)\" from \"%s (%i)\".", gRank[rank][rankName], rank, gRank[pRank[playerid]][rankName], pRank[playerid]);
	    SendClientMessage(playerid, COLOR_GREEN, buf);
	    SendClientMessage(playerid, COLOR_GREEN, "+$10000 and +10 Score as your reward.");

	    SetPlayerScore(playerid, GetPlayerScore(playerid) + 10);
	    GivePlayerMoney(playerid, 10000);

	    PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	    pRank[playerid] = rank;
	}

	return 1;
}

bool:HasSameTeam(playerid, otherid)
{
	if (GetPlayerTeam(playerid) == NO_TEAM || GetPlayerTeam(otherid) == NO_TEAM)
	{
		return false;
	}

	return bool:(GetPlayerTeam(playerid) == GetPlayerTeam(otherid));
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if (GetPlayerState(playerid) == PLAYER_STATE_SPECTATING && (pStats[playerid][userAdmin] || IsPlayerAdmin(playerid)))
 	{
		if (newkeys & KEY_LOOK_BEHIND)
	    {
	   		cmd_specoff(playerid, "");
	    }
	    if (newkeys & KEY_FIRE)
	    {
	        UpdatePlayerSpectate(playerid, true);
	    }
	    if (newkeys & KEY_AIM)
	    {
	        UpdatePlayerSpectate(playerid, false);
	    }
	    return 1;
	}

	new keys, updown, leftright;
	GetPlayerKeys(playerid, keys, updown, leftright);
	if (keys & KEY_LOOK_BEHIND && keys & KEY_AIM)
	{
	    if (pClass[playerid] == 0)
	    {
			if (gettime() - pActionTime[playerid] < 100)
			{
				return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 100 seconds before armouring players again.");
			}

		    new targetid = GetPlayerTargetPlayer(playerid);
		    if (! IsPlayerConnected(targetid) || targetid == INVALID_PLAYER_ID)
			{
				return SendClientMessage(playerid, COLOR_TOMATO, "Invalid target.");
			}

			if (! HasSameTeam(playerid, targetid))
			{
				return SendClientMessage(playerid, COLOR_TOMATO, "You cannot armour enemies.");
			}

			new Float:pos[3];
			GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
			if (! IsPlayerInRangeOfPoint(targetid, 10.0, pos[0], pos[1], pos[2]))
			{
				return SendClientMessage(playerid, COLOR_TOMATO, "The target player is not near you.");
			}

			new Float:ar;
		 	GetPlayerArmour(targetid, ar);
		 	if(ar + 50.0 >= 100.0)
			{
			 	SetPlayerArmour(targetid, 100.0);
		 	}
		 	else
		 	{
			 	SetPlayerArmour(targetid, ar + 50.0);
		 	}

		 	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
			GivePlayerMoney(playerid, 1000);
		 	PlayerPlaySound(targetid, 1133, 0.0, 0.0, 0.0);

		 	pActionTime[playerid] = gettime();

			new buf[150];
			format(buf, sizeof(buf), "You have armoured %s(%i) with 50# armour [+$1000].", ReturnPlayerName(targetid), targetid);
			SendClientMessage(playerid, COLOR_GREEN, buf);
			format(buf, sizeof(buf), "You armoured ~g~s(%i)", ReturnPlayerName(targetid), targetid);
			NotifyPlayer(playerid, buf, 5000);

			format(buf, sizeof(buf), "%s %s(%i) have armoured you with 50# armour.", gClass[0][className], ReturnPlayerName(playerid), playerid);
			SendClientMessage(targetid, COLOR_GREEN, buf);
			format(buf, sizeof(buf), "You got armoured by %s ~g~%s(%i)", ReturnPlayerName(playerid), playerid, gClass[0][className]);
			NotifyPlayer(targetid, buf, 5000);
		}
		else if (pClass[playerid] == 1)
	    {
			if (gettime() - pActionTime[playerid] < 100)
			{
				return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 100 seconds before healing players again.");
			}

		    new targetid = GetPlayerTargetPlayer(playerid);
		    if (! IsPlayerConnected(targetid) || targetid == INVALID_PLAYER_ID)
			{
				return SendClientMessage(playerid, COLOR_TOMATO, "Invalid target.");
			}

			if (! HasSameTeam(playerid, targetid))
			{
				return SendClientMessage(playerid, COLOR_TOMATO, "You cannot heal enemies.");
			}

			new Float:pos[3];
			GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
			if (! IsPlayerInRangeOfPoint(targetid, 10.0, pos[0], pos[1], pos[2]))
			{
				return SendClientMessage(playerid, COLOR_TOMATO, "The target player is not near you.");
			}

			new Float:hp;
		 	GetPlayerHealth(targetid, hp);
		 	if(hp + 50.0 >= 100.0)
			{
			 	SetPlayerHealth(targetid, 100.0);
		 	}
		 	else
		 	{
			 	SetPlayerHealth(targetid, hp + 50.0);
		 	}

		 	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
			GivePlayerMoney(playerid, 1000);
		 	PlayerPlaySound(targetid, 1133, 0.0, 0.0, 0.0);

		 	pActionTime[playerid] = gettime();

			new buf[150];
			format(buf, sizeof(buf), "You have healed %s(%i) with 50# health [+$1000].", ReturnPlayerName(targetid), targetid);
			SendClientMessage(playerid, COLOR_GREEN, buf);
			format(buf, sizeof(buf), "You healed ~g~%s(%i)", ReturnPlayerName(targetid), targetid);
			NotifyPlayer(playerid, buf, 5000);

			format(buf, sizeof(buf), "%s %s(%i) have healed you with 50# health.", gClass[1][className], ReturnPlayerName(playerid), playerid);
			SendClientMessage(targetid, COLOR_GREEN, buf);
			format(buf, sizeof(buf), "You got healed by %s ~g~%s(%i)", ReturnPlayerName(playerid), playerid, gClass[1][className]);
			NotifyPlayer(targetid, buf, 5000);
		}
  	}

	if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
	{
	  	if (newkeys == KEY_SUBMISSION)
		{
		    if (pClass[playerid] == 5)
			{
			    if (IsPlayerInAnyVehicle(playerid))
			    {
					foreach (new i : Player)
					{
					    if (! HasSameTeam(playerid, i) && GetPlayerVehicleID(i) == GetPlayerVehicleID(playerid) && GetPlayerState(i) == PLAYER_STATE_DRIVER)
					    {
					        new Float:hp;
					        GetPlayerHealth(i, hp);
					        SetPlayerHealth(i, hp - 5.0);

					        PlayerPlaySound(i, 1130, 0.0, 0.0, 0.0);
					        PlayerPlaySound(playerid, 1130, 0.0, 0.0, 0.0);

					        if(hp - 5.0 <= 0.0)
							{
							    pKiller[i][0] = playerid;
							    pKiller[i][1] = 4;
					        }

							break;
					    }
					}
				}
		    }
		}
	}

	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
	    if (newkeys & KEY_NO)
	    {
	        for (new i; i < MAX_DROPS; i++)
	        {
	            if (IsPlayerInDynamicArea(playerid, gDropAreaid[i]))
	            {
	                new weapon_name[35];
	                GetWeaponName(gDropWeaponid[i], weapon_name, sizeof(weapon_name));

	                new buf[150];
	                strcat(buf, "You picked up ");
	                strcat(buf, weapon_name);
	                strcat(buf, ".");
	                SendClientMessage(playerid, COLOR_GREEN, buf);

	                GivePlayerWeapon(playerid, gDropWeaponid[i], gDropAmount[playerid]);

					DestroyDynamicObject(gDropObject[i]);
					DestroyDynamicArea(gDropAreaid[i]);

					KillTimer(gDropTimer[i]);

	                break;
	            }
	        }
	    }
	    else if (newkeys & KEY_FIRE)
		{
	        new Float:x, Float:y, Float:z;
			GetPlayerPos(playerid, x, y, z);
			if (GetPlayerWeapon(playerid) == 17)
	 		{
	       		foreach (new i : Player)
	  			{
		     		if (IsPlayerInRangeOfPoint(i, 5.0, x, y, z) && i != playerid && GetPlayerState(i) == PLAYER_STATE_ONFOOT && ! HasSameTeam(playerid, i) && ! pHasMask[i])
	        		{
	        			ApplyAnimation(i, "ped", "gas_cwr", 1.0, 0, 0, 0, 0, 0);
			    	}
			    }
	    	}
	 	}
	}

	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	if (issuerid != INVALID_PLAYER_ID)
	{
    	pLastDamageTime[playerid] = gettime();

		if (pProtectTick[playerid])
		{
		    return GameTextForPlayer(issuerid, "~r~Player is Spawnkill Protected", 3000, 3);
		}

		if (bodypart == 3 || bodypart == 4)
		{
		    if (pInClass[playerid] || pDuel[playerid][duelActive])
		    {
		    }
	     	else if (! HasSameTeam(playerid, issuerid))
			{
			    if (IsPlayerAttachedObjectSlotUsed(playerid, 2))
			    {
			        new Float:hp;
			        GetPlayerHealth(playerid, hp);

			        amount -= (amount/100) * 15;
			        SetPlayerHealth(playerid, hp - amount);
			    }
			}
		}
		else if (bodypart == 9)
		{
		    if (pInClass[playerid] || pDuel[playerid][duelActive])
		    {
		    }
	     	else if (! HasSameTeam(playerid, issuerid))
			{
			    if (IsPlayerAttachedObjectSlotUsed(playerid, 2))
			    {
			        new Float:hp;
			        GetPlayerHealth(playerid, hp);

			        amount -= (amount/100) * 15;
			        SetPlayerHealth(playerid, hp - amount);
			    }

			    if (pHasHelmet[playerid])
			    {
			        return SendClientMessage(playerid, COLOR_WHITE, "The player is wearing a protection helmet, he/she won't die instantly.");
			    }

	            pStats[issuerid][userHeadshots]++;

			    NotifyPlayer(playerid, "You got ~r~Headshot", 5000);
			    NotifyPlayer(issuerid, "Perfect ~g~Headshot", 5000);

		    	SetPlayerHealth(playerid, 0.0);

				PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
				PlayerPlaySound(issuerid, 1055, 0.0, 0.0, 0.0);

				pKiller[playerid][0] = issuerid;
				pKiller[playerid][1] = weaponid;
		    }
	    }
	}

	return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float: amount, weaponid, bodypart)
{
    if (HasSameTeam(playerid, damagedid))
	{
    	NotifyPlayer(playerid, "~r~Same team!", 3000);
    	return 0;
    }

	return 1;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if (hittype == BULLET_HIT_TYPE_PLAYER)
	{
        if (! HasSameTeam(playerid, hitid))
        {
            if (pProtectTick[playerid])
            {
		    	pProtectTick[playerid] = 1;
			}
		}
	}
    else if (hittype == BULLET_HIT_TYPE_VEHICLE)
    {
        if (GetPlayerTeam(playerid) != NO_TEAM)
        {
	        foreach (new i : Player)
	        {
	            if (i != playerid && GetPlayerVehicleID(i) == hitid && GetPlayerVehicleSeat(i) == 0 && HasSameTeam(playerid, i))
	        	{
	                NotifyPlayer(playerid, "~r~Same team!", 3000);
	                return 0;
	            }
	        }
		}
    }

    return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	if(! ispassenger)
	{
	    if (pStats[playerid][userGodCar])
	    {
	   		SetVehicleHealth(vehicleid, FLOAT_INFINITY);
		}

		if (GetPlayerTeam(playerid) != NO_TEAM)
        {
	        foreach (new i : Player)
	        {
	            if (i != playerid && GetPlayerVehicleID(i) == vehicleid && GetPlayerVehicleSeat(i) == 0 && HasSameTeam(playerid, i))
	        	{
					new Float:x, Float:y, Float:z;
					GetPlayerPos(playerid, x, y, z);
					SetPlayerPos(playerid, x, y, z + 1);

					NotifyPlayer(playerid, "~r~No team jacking!", 3000);
					return 0;
	            }
	        }
		}
	}

    return 1;
}

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{
    if (pStats[playerid][userGodCar])
	{
    	UpdateVehicleDamageStatus(vehicleid, 0, 0, 0, 0);
	}

	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
    for (new i; i < MAX_TEAMS; i++)
    {
		if (vehicleid == gTeam[i][teamProtoId])
		{
			if(GetPlayerTeam(forplayerid) == i || GetPlayerTeam(forplayerid) == NO_TEAM)
			{
				SetVehicleParamsForPlayer(gTeam[i][teamProtoId], forplayerid, 1, 1);
			}
			else
			{
				SetVehicleParamsForPlayer(gTeam[i][teamProtoId], forplayerid, 1, 0);
			}
		}
	}

	return 1;
}

public OnVehicleSpawn(vehicleid)
{
    for (new i; i < MAX_TEAMS; i++)
	{
		if (vehicleid == gTeam[i][teamProtoId])
		{
		    if(gTeam[i][teamProtoAttacker] != INVALID_PLAYER_ID)
		    {
				new text[150];
				format(text, sizeof(text), "PROTOTYPE: %s(%d) failed to steel team %s's prototype vehicle.", ReturnPlayerName(gTeam[i][teamProtoAttacker]), gTeam[i][teamProtoAttacker], gTeam[i][teamName]);
				SendClientMessageToAll(COLOR_ORANGE_RED, text);

				DisablePlayerRaceCheckpoint(gTeam[i][teamProtoAttacker]);
	            gTeam[i][teamProtoAttacker] = INVALID_PLAYER_ID;
            }

			break;
	    }
    }

	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
    for (new i; i < MAX_TEAMS; i++)
	{
		if (vehicleid == gTeam[i][teamProtoId])
		{
      		if(gTeam[i][teamProtoAttacker] != INVALID_PLAYER_ID)
		    {
				new text[150];
				format(text, sizeof(text), "PROTOTYPE: %s(%d) failed to steel team %s's prototype vehicle.", ReturnPlayerName(gTeam[i][teamProtoAttacker]), gTeam[i][teamProtoAttacker], gTeam[i][teamName]);
				SendClientMessageToAll(COLOR_ORANGE_RED, text);

				DisablePlayerRaceCheckpoint(gTeam[i][teamProtoAttacker]);
                gTeam[i][teamProtoAttacker] = INVALID_PLAYER_ID;

				break;
	        }
	    }
    }

    if (killerid != INVALID_PLAYER_ID)
    {
	    foreach (new i : Player)
		{
			if (GetPlayerVehicleID(i) == vehicleid && GetPlayerVehicleSeat(i) == 0)
			{
		        new text[150];
				format(text, sizeof(text), "Good job! You destroyed enemy team's vehicle, +1 score and +$500. [Driver: %s(%d)]", ReturnPlayerName(i), i);
				SendClientMessage(killerid, COLOR_GREEN, text);

				GivePlayerMoney(killerid, 500);
				SetPlayerScore(killerid, GetPlayerScore(killerid) + 1);

	    		break;
			}
	    }
    }

	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (newstate == PLAYER_STATE_DRIVER)
	{
	    if (pStats[playerid][userGodCar])
		{
	    	UpdateVehicleDamageStatus(GetPlayerVehicleID(playerid), 0, 0, 0, 0);
	    	RepairVehicle(GetPlayerVehicleID(playerid));
	    	SetVehicleHealth(GetPlayerVehicleID(playerid), 1000.0);
		}

		new modelid = GetVehicleModel(GetPlayerVehicleID(playerid));
		if (modelid == 447)
		{
	     	if (0 <= GetPlayerTeam(playerid) < MAX_TEAMS && pClass[playerid] != 4)
		    {
		        new text[150];
		        format(text, sizeof(text), "You must be a %s to drive sea-sparrow.", gClass[4][className]);
		        SendClientMessage(playerid, COLOR_TOMATO, text);

		        new Float:x, Float:y, Float:z;
				GetPlayerPos(playerid, x, y, z);
				SetPlayerPos(playerid, x, y, z + 3.0);

		        return 1;
			}
		}
		else if (modelid == 520)
		{
	     	if (0 <= GetPlayerTeam(playerid) < MAX_TEAMS && pClass[playerid] != 4)
		    {
		        new text[150];
		        format(text, sizeof(text), "You must be a %s to drive hydra.", gClass[4][className]);
		        SendClientMessage(playerid, COLOR_TOMATO, text);

		        new Float:x, Float:y, Float:z;
				GetPlayerPos(playerid, x, y, z);
				SetPlayerPos(playerid, x, y, z + 3.0);

		        return 1;
			}
		}
		else if (modelid == 425)
		{
	     	if (0 <= GetPlayerTeam(playerid) < MAX_TEAMS && pClass[playerid] != 4 && pRank[playerid] < gClass[4][classRank] + 2)
		    {
		        new text[150];
		        format(text, sizeof(text), "You must be a %s rank %d+ to drive hunter.", gClass[4][className], gClass[4][classRank] + 2);
		        SendClientMessage(playerid, COLOR_TOMATO, text);

		        new Float:x, Float:y, Float:z;
				GetPlayerPos(playerid, x, y, z);
				SetPlayerPos(playerid, x, y, z + 3.0);

		        return 1;
			}
		}
		else if (modelid == 432)
		{
	     	if (0 <= GetPlayerTeam(playerid) < MAX_TEAMS && pClass[playerid] != 3)
		    {
		        new text[150];
		        format(text, sizeof(text), "You must be a %s to drive rhino.", gClass[3][className]);
		        SendClientMessage(playerid, COLOR_TOMATO, text);

		        new Float:x, Float:y, Float:z;
				GetPlayerPos(playerid, x, y, z);
				SetPlayerPos(playerid, x, y, z + 3.0);

		        return 1;
			}
		}
		else
		{
		    if (0 <= GetPlayerTeam(playerid) < MAX_TEAMS)
		    {
		    	for (new i; i < MAX_TEAMS; i++)
				{
				    if (GetPlayerVehicleID(playerid) == gTeam[i][teamProtoId])
					{
					  	if (gTeam[i][teamProtoAttacker] == INVALID_PLAYER_ID)
						{
						    if (GetPlayerTeam(playerid) != i)
						    {
								new string[156];
								format(string, sizeof(string), "PROTOTYPE: %s(%d) is trying to steel team %s's prototype vehicle.", ReturnPlayerName(playerid), playerid, gTeam[i][teamName]);
								SendClientMessageToAll(COLOR_ORANGE_RED, string);
							}
						}

						NotifyPlayer(playerid, "Take the van back to ~y~Base~w~~h~.", 5000);

						gTeam[i][teamProtoAttacker] = playerid;
						SendClientMessage(playerid, COLOR_YELLOW, "PROTOTYPE: You have the enemy prototype, take it to your base (marked as a red race checkpoint in your map).");
						SetPlayerRaceCheckpoint(playerid, 1, gTeam[GetPlayerTeam(playerid)][teamProtoCP][0], gTeam[GetPlayerTeam(playerid)][teamProtoCP][1], gTeam[GetPlayerTeam(playerid)][teamProtoCP][2], 0.0, 0.0, 0.0, 10.0);
						break;
				    }
	   	 		}
			}
		}
	}
	else
	{
		if (0 <= GetPlayerTeam(playerid) < MAX_TEAMS)
		{
	    	for (new i; i < MAX_TEAMS; i++)
			{
			    if (gTeam[i][teamProtoAttacker] == playerid)
				{
					NotifyPlayer(playerid, "The van will respawn in ~y~60 seconds ~w~~h~if you don't get in before.", 5000);

					SendClientMessage(playerid, COLOR_TOMATO, "PROTOTYPE: You have left the prototype vehicle, the vehicle will spawn back in 60 seconds unless you go back in.");
					DisablePlayerRaceCheckpoint(playerid);

					break;
				}
			}
		}
	}

	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
    if (0 <= GetPlayerTeam(playerid) < MAX_TEAMS)
	{
	    for (new i; i < MAX_TEAMS; i++)
		{
		    if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
		    {
		 		if (GetPlayerVehicleID(playerid) == gTeam[i][teamProtoId])
				{
				    if (playerid == gTeam[i][teamProtoAttacker])
				    {
						GivePlayerMoney(playerid, 5000);
						SetPlayerScore(playerid, GetPlayerScore(playerid) + 5);
						DisablePlayerRaceCheckpoint(playerid);

		      			gTeam[i][teamProtoAttacker] = INVALID_PLAYER_ID;
						SetVehicleToRespawn(gTeam[i][teamProtoId]);

						new text[150];
						format(text, sizeof(text), "PROTOTYPE: %s(%d) was successfull to steel %s's prototype vehicle.", ReturnPlayerName(playerid), playerid, gTeam[i][teamName]);
						SendClientMessageToAll(COLOR_ORANGE_RED, text);

						format(text, sizeof(text), "PROTOTYPE: You have successfully stolen %s's prototype vehicle, +5 score and +$5000.", gTeam[i][teamName]);
						SendClientMessage(playerid, COLOR_GREEN, text);

						break;
				    }
		   	 	}
			}
		}
	}

	return 1;
}

ReturnZoneBonus(zoneid)
{
	new bonus[100] = "-No bonus-";
	switch (zoneid)
	{
	    case 0: bonus = "Health supply after every 1 minute";
	}

	return bonus;
}

public OnPlayerEnterDynamicCP(playerid, checkpointid)
{
	new team = GetPlayerTeam(playerid);
	if (0 <= team < MAX_TEAMS)
	{
	    for (new i, j = sizeof(gZone); i < j; i++)
		{
		    if (gZone[i][zoneCPId] == checkpointid)
		    {
				new text[150];
		        if (gZone[i][zoneAttacker] != INVALID_PLAYER_ID)
		        {
			        if (team == gZone[i][zoneOwner])
					{
					    strcat(text, "The zone is under attack by ");
					    strcat(text, gTeam[GetPlayerTeam(gZone[i][zoneAttacker])][teamName]);
					    strcat(text, ".");
					    SendClientMessage(playerid, COLOR_YELLOW, text);
			    	}
			        else if (team == GetPlayerTeam(gZone[i][zoneAttacker]))
					{
					    if (IsPlayerInAnyVehicle(playerid))
						{
						    return SendClientMessage(playerid, COLOR_TOMATO, "You cannot capture the zone in a vehicle.");
						}

					    new buf[250];
					    format(buf, sizeof(buf), "%s (%i)~n~~g~Controlled by: ~w~~h~%s~n~~r~Attacked by: ~w~~h~%s~n~~n~~y~Zone bonus:~n~~w~~h~%s", gZone[i][zoneName], i, gTeam[gZone[i][zoneOwner]][teamName], gTeam[GetPlayerTeam(gZone[i][zoneAttacker])][teamName], ReturnZoneBonus(i));
					    PlayerTextDrawSetString(playerid, ptxtCapture[playerid], buf);
				  		PlayerTextDrawShow(playerid, ptxtCapture[playerid]);

				  		SetPlayerProgressBarValue(playerid, pbarCapture[playerid], gZone[i][zoneTick]);
						ShowPlayerProgressBar(playerid, pbarCapture[playerid]);

						gZone[i][zonePlayer]++;
						SendClientMessage(playerid, COLOR_YELLOW, "Stay in the checkpoint to assist your teammate in capturing the zone.");
			    	}
		    	}
		    	else
		    	{
		    		if (team != gZone[i][zoneOwner])
					{
					    if (IsPlayerInAnyVehicle(playerid))
						{
						    return SendClientMessage(playerid, COLOR_TOMATO, "You cannot capture the zone in a vehicle.");
						}

					    strcat(text, "The zone is controlled by team ");
					    strcat(text, gTeam[gZone[i][zoneOwner]][teamName]);
					    strcat(text, ".");
						SendClientMessage(playerid, COLOR_YELLOW, text);
						SendClientMessage(playerid, COLOR_YELLOW, "Stay in the checkpoint for "#CAPTURE_TIME" seconds to capture the zone.");

						NotifyPlayer(playerid, "Stay in the ~r~checkpoint ~w~~h~for ~r~"#CAPTURE_TIME" seconds ~w~~h~to capture!", 3000);

                        GangZoneFlashForAll(gZone[i][zoneId], SET_ALPHA(gTeam[team][teamColor], 100));

						gZone[i][zoneAttacker] = playerid;
						gZone[i][zonePlayer] = 1;
						gZone[i][zoneTick] = 0;

						KillTimer(gZone[i][zoneTimer]);
						gZone[i][zoneTimer] = SetTimerEx("OnZoneUpdate", 1000, true, "i", i);

                        text[0] = EOS;
						strcat(text, "ZONE: Team ");
						strcat(text, gTeam[team][teamName]);
						strcat(text, " is trying to capture zone ");
						strcat(text, gZone[i][zoneName]);
						strcat(text, " against team ");
						strcat(text, gTeam[gZone[i][zoneOwner]][teamName]);
						strcat(text, ".");
						SendClientMessageToAll(COLOR_ORANGE_RED, text);

						text[0] = EOS;
						strcat(text, gZone[i][zoneName]);
						strcat(text, "\n");
						strcat(text, ""WHITE"Controlled by ");
						strcat(text, gTeam[gZone[i][zoneOwner]][teamName]);
						UpdateDynamic3DTextLabelText(gZone[i][zoneLabel], gTeam[gZone[i][zoneOwner]][teamColor], text);

					    new buf[250];
					    format(buf, sizeof(buf), "%s (%i)~n~~g~Controlled by: ~w~~h~%s~n~~r~Attacked by: ~w~~h~%s~n~~n~~y~Zone bonus:~n~~w~~h~%s", gZone[i][zoneName], i, gTeam[gZone[i][zoneOwner]][teamName], gTeam[GetPlayerTeam(playerid)][teamName], ReturnZoneBonus(i));
					    PlayerTextDrawSetString(playerid, ptxtCapture[playerid], buf);
				  		PlayerTextDrawShow(playerid, ptxtCapture[playerid]);

				  		SetPlayerProgressBarValue(playerid, pbarCapture[playerid], gZone[i][zoneTick]);
						ShowPlayerProgressBar(playerid, pbarCapture[playerid]);
					}
					else if (team == gZone[i][zoneOwner])
					{
						SendClientMessage(playerid, COLOR_YELLOW, "The zone is under our team's control.");
					}
		    	}

		    	break;
		    }
		}
	}

	return 1;
}

forward OnZoneUpdate(zoneid);
public  OnZoneUpdate(zoneid)
{
    switch(gZone[zoneid][zonePlayer])
	{
 		case 1: gZone[zoneid][zoneTick] += 1;
 		case 2: gZone[zoneid][zoneTick] += 2;
		default: gZone[zoneid][zoneTick] += 3;
	}
	foreach (new p : Player)
	{
 		if (IsPlayerInDynamicCP(p, gZone[zoneid][zoneCPId]) && ! IsPlayerInAnyVehicle(p) && GetPlayerTeam(p) == GetPlayerTeam(gZone[zoneid][zoneAttacker]))
		{
			SetPlayerProgressBarValue(p, pbarCapture[p], gZone[zoneid][zoneTick]);
	    }
	}

	if (gZone[zoneid][zoneTick] > CAPTURE_TIME)
	{
		SendClientMessage(gZone[zoneid][zoneAttacker], COLOR_GREEN, "You have successfully captured the zone, +3 score and +$3000.");
		SetPlayerScore(gZone[zoneid][zoneAttacker], GetPlayerScore(gZone[zoneid][zoneAttacker]) + 3);
		GivePlayerMoney(gZone[zoneid][zoneAttacker], 3000);

		pStats[gZone[zoneid][zoneAttacker]][userZones]++;

		NotifyPlayer(gZone[zoneid][zoneAttacker], "Zone successfully ~g~captured ~w~~h~!", 3000);

		foreach (new p : Player)
		{
		    if (IsPlayerInDynamicCP(p, gZone[zoneid][zoneCPId]))
			{
				PlayerTextDrawHide(p, ptxtCapture[p]);
				HidePlayerProgressBar(p, pbarCapture[p]);

				if (p != gZone[zoneid][zoneAttacker] && GetPlayerTeam(p) == GetPlayerTeam(gZone[zoneid][zoneAttacker]) && ! IsPlayerInAnyVehicle(p))
				{
					SendClientMessage(p, COLOR_GREEN, "You have assisted your teammate to capture the zone, +2 score and +$1500.");
					SetPlayerScore(p, GetPlayerScore(p) + 2);
					GivePlayerMoney(p, 1500);
				}
			}
		}

	    GangZoneStopFlashForAll(gZone[zoneid][zoneId]);
	    GangZoneShowForAll(gZone[zoneid][zoneId], SET_ALPHA(gTeam[GetPlayerTeam(gZone[zoneid][zoneAttacker])][teamColor], 100));

		KillTimer(gZone[zoneid][zoneTimer]);

	    new text[150];
		strcat(text, "ZONE: Team ");
		strcat(text, gTeam[GetPlayerTeam(gZone[zoneid][zoneAttacker])][teamName]);
		strcat(text, " has successfully captured the zone ");
		strcat(text, gZone[zoneid][zoneName]);
		strcat(text, " against team ");
		strcat(text, gTeam[gZone[zoneid][zoneOwner]][teamName]);
		strcat(text, ".");
		SendClientMessageToAll(COLOR_ORANGE_RED, text);

		text[0] = EOS;
		strcat(text, gZone[zoneid][zoneName]);
		strcat(text, "\n");
		strcat(text, ""WHITE"Controlled by ");
		strcat(text, gTeam[GetPlayerTeam(gZone[zoneid][zoneAttacker])][teamName]);
		UpdateDynamic3DTextLabelText(gZone[zoneid][zoneLabel], gTeam[GetPlayerTeam(gZone[zoneid][zoneAttacker])][teamColor], text);

		gZone[zoneid][zoneOwner] = GetPlayerTeam(gZone[zoneid][zoneAttacker]);
		gZone[zoneid][zoneAttacker] = INVALID_PLAYER_ID;
	}
}

public OnPlayerLeaveDynamicCP(playerid, checkpointid)
{
	new team = GetPlayerTeam(playerid);
	if (0 <= team < MAX_TEAMS)
	{
	    for (new i, j = sizeof(gZone); i < j; i++)
		{
		    if (gZone[i][zoneCPId] == checkpointid)
		    {
		        if (gZone[i][zoneAttacker] != INVALID_PLAYER_ID)
		        {
				   	if (team == GetPlayerTeam(gZone[i][zoneAttacker]))
				   	{
                        gZone[i][zonePlayer]--;
                        if (! gZone[i][zonePlayer])
                        {
							SendClientMessage(playerid, COLOR_YELLOW, "You failed to capture the zone, there were no teammates left in your checkpoint.");

	                        GangZoneStopFlashForAll(gZone[i][zoneId]);

	                        new text[150];
							strcat(text, "ZONE: Team ");
							strcat(text, gTeam[team][teamName]);
							strcat(text, " failed to capture zone ");
							strcat(text, gZone[i][zoneName]);
							strcat(text, " against team ");
							strcat(text, gTeam[gZone[i][zoneOwner]][teamName]);
							strcat(text, ".");
							SendClientMessageToAll(COLOR_ORANGE_RED, text);

							text[0] = EOS;
							strcat(text, gZone[i][zoneName]);
							strcat(text, "\n");
							strcat(text, ""WHITE"Controlled by ");
							strcat(text, gTeam[gZone[i][zoneOwner]][teamName]);
							UpdateDynamic3DTextLabelText(gZone[i][zoneLabel], gTeam[gZone[i][zoneOwner]][teamColor], text);

							gZone[i][zoneAttacker] = INVALID_PLAYER_ID;
							KillTimer(gZone[i][zoneTimer]);
                        }
						else if (gZone[i][zoneAttacker] == playerid)
					   	{
					   	    foreach (new p : Player)
					   	    {
					   	        if (GetPlayerTeam(p) == team)
					   	        {
						   	        if (IsPlayerInDynamicCP(p, checkpointid))
						   	        {
					   	                gZone[i][zoneAttacker] = p;
					   	                break;
					   	            }
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

public OnPlayerEnterGangZone(playerid, zone)
{
	for (new i; i < MAX_TEAMS; i++)
	{
		if (gTeam[i][teamBaseId] == zone)
  		{
  		    new text[45];
  		    strcat(text, "~y~~h~");
  		    strcat(text, gTeam[i][teamName]);
  		    strcat(text, "'s base");
  		    GameTextForPlayer(playerid, text, 5000, 1);

		    break;
  		}
	}

	return 1;
}

public OnPlayerLeaveGangZone(playerid, zone)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
   	SetPlayerChatBubble(playerid, text, 0xEEEEEEFF, 35.0, 10000);

	new buf[150];
	if (text[0] == '!')
	{
	    if (pStats[playerid][userAdmin] >= 1)
		{
			format(buf, sizeof(buf), "[Admin Chat] %s(%i): %s", ReturnPlayerName(playerid), playerid, text[1]);
			foreach (new i : Player)
			{
			    if (pStats[i][userAdmin] >= 4)
			    {
					SendClientMessage(i, COLOR_PINK, buf);
				}
			}
		    return 0;
		}
	}
	else if (text[0] == '@')
	{
		if (pStats[playerid][userAdmin] >= 4)
		{
			format(buf, sizeof(buf), "[#4 Admin Chat] %s(%i): %s", ReturnPlayerName(playerid), playerid, text[1]);
			foreach (new i : Player)
			{
			    if (pStats[i][userAdmin] >= 4)
			    {
					SendClientMessage(i, COLOR_HOT_PINK, buf);
				}
			}
		    return 0;
		}
	}
	else if (text[0] == '#')
	{
		if (pStats[playerid][userAdmin] >= 5)
		{
			format(buf, sizeof(buf), "[#5 Admin Chat] %s(%i): %s", ReturnPlayerName(playerid), playerid, text[1]);
			foreach (new i : Player)
			{
			    if (pStats[i][userAdmin] >= 5)
			    {
					SendClientMessage(i, COLOR_DARK_PINK, buf);
				}
			}
		    return 0;
		}
	}

	if (pStats[playerid][userOnDuty])
	{
		format(buf, sizeof(buf), "Admin %s(%i): %s", ReturnPlayerName(playerid), playerid, text);
	}
	else if (pStats[playerid][userPremium])
	{
		format(buf, sizeof(buf), ""CYAN"[VIP] {%06x}(%i) %s: "WHITE"%s", gTeam[pTeam[playerid]][teamColor] >>> 8, playerid, ReturnPlayerName(playerid), text);
	}
	else
	{
		format(buf, sizeof(buf), "(%i) %s: "WHITE"%s", playerid, ReturnPlayerName(playerid), text);
	}
	SendClientMessageToAll(GetPlayerColor(playerid), buf);

	return 0;
}

CMD:heal(playerid, params[])
{
	if (GetPlayerTeam(playerid) < 0 || GetPlayerTeam(playerid) >= MAX_TEAMS)
	{
	    return 1;
	}

	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (pDuel[playerid][duelActive])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You can't perform this command while in a duel.");
	}

    if (pClass[playerid] != 1)
	{
		new text[150];
		format(text, sizeof(text), "You must be a %s to heal players.", gClass[1][className]);
	 	return SendClientMessage(playerid, COLOR_TOMATO, text);
	}

	if (gettime() - pActionTime[playerid] < 100)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 100 seconds before healing players again.");
	}

	new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /heal [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not connected.");
	}

	if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot heal yourself, use /med instead.");
	}

	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	if (! IsPlayerInRangeOfPoint(targetid, 10.0, pos[0], pos[1], pos[2]))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not near you.");
	}

	if (! HasSameTeam(playerid, targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot heal enemies.");
	}

	new Float:hp;
 	GetPlayerHealth(targetid, hp);
 	if(hp + 50.0 >= 100.0)
	{
	 	SetPlayerHealth(targetid, 100.0);
 	}
 	else
 	{
	 	SetPlayerHealth(targetid, hp + 50.0);
 	}

 	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	GivePlayerMoney(playerid, 1000);
 	PlayerPlaySound(targetid, 1133, 0.0, 0.0, 0.0);

 	pActionTime[playerid] = gettime();

	new buf[150];
	format(buf, sizeof(buf), "You have healed %s(%i) with 50# health [+$1000].", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_GREEN, buf);
	format(buf, sizeof(buf), "You got healed by %s ~g~%s(%i)", ReturnPlayerName(targetid), targetid, gClass[1][className]);
	NotifyPlayer(playerid, buf, 5000);

	format(buf, sizeof(buf), "%s %s(%i) have healed you with 50# health.", gClass[1][className], ReturnPlayerName(playerid), playerid);
	SendClientMessage(targetid, COLOR_GREEN, buf);
	format(buf, sizeof(buf), "You got healed by %s ~g~%s(%i)", ReturnPlayerName(playerid), playerid, gClass[1][className]);
	NotifyPlayer(targetid, buf, 5000);

	return 1;
}

CMD:armour(playerid, params[])
{
	if (GetPlayerTeam(playerid) < 0 || GetPlayerTeam(playerid) >= MAX_TEAMS)
	{
	    return 1;
	}

	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (pDuel[playerid][duelActive])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You can't perform this command while in a duel.");
	}

    if (pClass[playerid] != 0)
	{
		new text[150];
		format(text, sizeof(text), "You must be a %s to armour players.", gClass[0][className]);
	 	return SendClientMessage(playerid, COLOR_TOMATO, text);
	}

	if (gettime() - pActionTime[playerid] < 100)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 100 seconds before armouring players again.");
	}

	new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /armour [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not connected.");
	}

	if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot armour yourself, check your inventory for armour.");
	}

	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	if (! IsPlayerInRangeOfPoint(targetid, 10.0, pos[0], pos[1], pos[2]))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not near you.");
	}

	if (! HasSameTeam(playerid, targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot armour enemies.");
	}

	new Float:ar;
 	GetPlayerArmour(targetid, ar);
 	if(ar + 50.0 >= 100.0)
	{
	 	SetPlayerArmour(targetid, 100.0);
 	}
 	else
 	{
	 	SetPlayerArmour(targetid, ar + 50.0);
 	}

 	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
	GivePlayerMoney(playerid, 1000);
 	PlayerPlaySound(targetid, 1133, 0.0, 0.0, 0.0);

 	pActionTime[playerid] = gettime();

	new buf[150];
	format(buf, sizeof(buf), "You have armoured %s(%i) with 50# armour [+$1000].", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_GREEN, buf);
	format(buf, sizeof(buf), "You armoured ~g~%s(%i)", ReturnPlayerName(targetid), targetid);
	NotifyPlayer(playerid, buf, 5000);

	format(buf, sizeof(buf), "%s %s(%i) have armour you with 50# armour.", gClass[0][className], ReturnPlayerName(playerid), playerid);
	SendClientMessage(targetid, COLOR_GREEN, buf);
	format(buf, sizeof(buf), "You got armoured by %s ~g~%s(%i)", ReturnPlayerName(playerid), playerid, gClass[0][className]);
	NotifyPlayer(targetid, buf, 5000);

	return 1;
}

CMD:fix(playerid)
{
	if (GetPlayerTeam(playerid) < 0 || GetPlayerTeam(playerid) >= MAX_TEAMS)
	{
	    return 1;
	}

	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (pDuel[playerid][duelActive])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You can't perform this command while in a duel.");
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

    if (pClass[playerid] != 3)
	{
		new text[150];
		format(text, sizeof(text), "You must be a %s to fix vehicles.", gClass[3][className]);
	 	return SendClientMessage(playerid, COLOR_TOMATO, text);
	}

	if (gettime() - pActionTime[playerid] < 120)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 2 minutes before fixing vehicles again.");
	}

	new targetid = INVALID_VEHICLE_ID;
	new Float:x, Float:y, Float:z;
	for (new i = 1, j = GetVehiclePoolSize(); i <= j; i++)
	{
	    if (IsValidVehicle(i))
	    {
	        GetVehiclePos(i, x, y, z);
	        if (IsPlayerInRangeOfPoint(playerid, 7.0, x, y, z))
	        {
	            targetid = i;
	            break;
	        }
	    }
	}

	if (targetid == INVALID_VEHICLE_ID)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You aren't near any vehicle.");
	}

	foreach (new i : Player)
	{
	    if (i != playerid)
	    {
	        if (IsPlayerInAnyVehicle(i) && GetPlayerVehicleSeat(i) == 0)
	        {
	            if (! HasSameTeam(playerid, i))
	            {
	                return SendClientMessage(playerid, COLOR_TOMATO, "The vehicle is occupied by an enemy.");
	            }
	        }
	    }
	}

	ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 0.5, 0, 1, 1, 1, 0, 1);

	new vehicle[45];
	strcat(vehicle, gVehicleModelNames[GetVehicleModel(targetid) - 400]);

	RepairVehicle(targetid);
	SetVehicleHealth(targetid, 1000.0);

    pActionTime[playerid] = gettime();

	new buf[150];
	format(buf, sizeof(buf), "* %s %s(%i) have fixed vehicle %s (id: %i).", gClass[3][className], ReturnPlayerName(playerid), playerid, vehicle, targetid);

	GetPlayerPos(playerid, x, y, z);
	foreach (new i : Player)
	{
 		if (IsPlayerInRangeOfPoint(i, 50.0, x, y, z))
   		{
     		SendClientMessage(i, COLOR_GREY, buf);
		}
 	}

	format(buf, sizeof(buf), "You have fixed vehicle ~b~%s (id: %i)~w~~h~.", vehicle, targetid);
	NotifyPlayer(targetid, buf, 5000);

	PlayerPlaySound(playerid, 1134, 0.0, 0.0, 0.0);

	return 1;
}

CMD:build(playerid)
{
	if (GetPlayerTeam(playerid) < 0 || GetPlayerTeam(playerid) >= MAX_TEAMS)
	{
	    return 1;
	}

	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (pDuel[playerid][duelActive])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You can't perform this command while in a duel.");
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

    if (pClass[playerid] != 3)
	{
		new text[150];
		format(text, sizeof(text), "You must be a %s to build.", gClass[3][className]);
	 	return SendClientMessage(playerid, COLOR_TOMATO, text);
	}

	if (pBuildMode[playerid])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You are already in build mode, you can build again on next spawn.");
	}

	ShowPlayerDialog(playerid, DIALOG_ID_BUILD, DIALOG_STYLE_LIST, "Build options:", "RC Baron - Machinegun plane\nRC Tiger - Mini rocket tank\nRC Goblin - Armour system with bomb striker\nRC Bandit - Strike on ground explosive car", "Build", "Cancel");

	return 1;
}

CMD:dis(playerid)
{
	if (GetPlayerTeam(playerid) < 0 || GetPlayerTeam(playerid) >= MAX_TEAMS)
	{
	    return 1;
	}

	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (pDuel[playerid][duelActive])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You can't perform this command while in a duel.");
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

    if (pClass[playerid] != 5)
	{
		new text[150];
		format(text, sizeof(text), "You must be a %s to disguize.", gClass[5][className]);
	 	return SendClientMessage(playerid, COLOR_TOMATO, text);
	}

	if (pDisguizeKits[playerid] == 0)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You don't have any disguize kit left with you, buy one from shop.");
	}

	new buf[35 * MAX_TEAMS];
	for (new i; i < MAX_TEAMS; i++)
	{
		format(buf, sizeof(buf), "%s{%06x}%s\n", buf, gTeam[i][teamColor] >>> 8, gTeam[i][teamName]);
	}
	ShowPlayerDialog(playerid, DIALOG_ID_DISGUIZE, DIALOG_STYLE_LIST, "Disguize to team?", buf, "Select", "Cancel");

	return 1;
}

CMD:undis(playerid)
{
	if (GetPlayerTeam(playerid) < 0 || GetPlayerTeam(playerid) >= MAX_TEAMS)
	{
	    return 1;
	}

	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (pDuel[playerid][duelActive])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You can't perform this command while in a duel.");
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

    if (pClass[playerid] != 5)
	{
		new text[150];
		format(text, sizeof(text), "You must be a %s to undisguize.", gClass[5][className]);
	 	return SendClientMessage(playerid, COLOR_TOMATO, text);
	}

	SetPlayerSkin(playerid, gTeam[pTeam[playerid]][teamSkin]);
	SetPlayerColor(playerid, gTeam[pTeam[playerid]][teamColor]);

	new buf[150];
	format(buf, sizeof(buf), "You have undisguized to your team {%06x}%s"WHITE"%.", gTeam[pTeam[playerid]][teamName], gTeam[pTeam[playerid]][teamColor] >>> 8);
	SendClientMessage(playerid, COLOR_WHITE, buf);

	return 1;
}

CMD:rob(playerid, params[])
{
	if (GetPlayerTeam(playerid) < 0 || GetPlayerTeam(playerid) >= MAX_TEAMS)
	{
	    return 1;
	}

	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (pDuel[playerid][duelActive])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You can't perform this command while in a duel.");
	}

    if (pClass[playerid] != 5)
	{
		new text[150];
		format(text, sizeof(text), "You must be a %s to rob players.", gClass[5][className]);
	 	return SendClientMessage(playerid, COLOR_TOMATO, text);
	}

	new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /rob [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not connected.");
	}

	if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot rob yourself.");
	}

	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	if (! IsPlayerInRangeOfPoint(targetid, 3.0, pos[0], pos[1], pos[2]))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not near you.");
	}

	if (HasSameTeam(playerid, targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot rob your teammates.");
	}

	if (gettime() - pActionTime[playerid] < 100)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 100 seconds before robing players again.");
	}

	pActionTime[playerid] = gettime();

	switch (random(4))
	{
	    case 1:
	    {
		    SetPlayerSkin(playerid, gTeam[pTeam[playerid]][teamSkin]);
 			SetPlayerColor(playerid, gTeam[pTeam[playerid]][teamColor]);

 			SendClientMessage(playerid, COLOR_TOMATO, "You have been detected as a spy, run away or /dis to disguize again in a corner.");
	        NotifyPlayer(playerid, "You were detected as a ~r~spy~w~~h~!", 5000);
	    }
	    default:
	    {
	        new money = (1000 + random(500));
	        GivePlayerMoney(playerid, money);
	        GivePlayerMoney(targetid, -money);

			new buf[150];
			format(buf, sizeof(buf), "You have robbed %s(%i) with $%i.", ReturnPlayerName(targetid), targetid, money);
			SendClientMessage(playerid, COLOR_GREEN, buf);
			NotifyPlayer(playerid, "The player was ~g~robbed ~w~~h~successfully!", 5000);
	    }
	}

	return 1;
}

CMD:buy(playerid)
{
	if (GetPlayerTeam(playerid) < 0 || GetPlayerTeam(playerid) >= MAX_TEAMS)
	{
	    return 1;
	}

	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (pDuel[playerid][duelActive])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You can't perform this command while in a duel.");
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (gettime() - pLastDamageTime[playerid] < 15)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 15 seconds after geting shot by an enemy to open shop.");
	}

	new shop = -1;
	for (new i, j = sizeof(gShop); i < j; i++)
	{
		if (! IsPlayerInDynamicArea(playerid, gShop[i][shopAreaid]))
		{
			continue;
		}

		shop = i;
		break;
	}

	if (shop == -1)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You aren't near any shop.");
	}

	if (gShop[shop][shopTeam] != NO_TEAM && GetPlayerTeam(playerid) != gShop[shop][shopTeam])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot open enemy shops.");
	}

	new info[1000];
	strcat(info, "Health - "RED"Cost -$5000 for 100%\n");
	strcat(info, "Armour - "RED"Cost -$6500 for 100%\n");
	strcat(info, "Protection helmet - "ORANGE"-$7500, protect against headshots\n");
	strcat(info, "Gas mask - "ORANGE"-$5000, protect against teargas\n");
	strcat(info, "Inventory items - "GREY"Some extra items and inventory list\n");
	strcat(info, "Weapons list - "GREY"Buy weapons for a single spawn\n");
	strcat(info, "VIP. area - "GREEN"Only for donators");
	ShowPlayerDialog(playerid, DIALOG_ID_SHOP, DIALOG_STYLE_LIST, "Shop/Armoury (/buy)", info, "Select", "Cancel");

	return 1;
}

CMD:teams(playerid)
{
	new players[MAX_TEAMS];
	foreach (new i : Player)
	{
	    if (0 <= GetPlayerTeam(i) < MAX_TEAMS)
	    {
	        players[GetPlayerTeam(i)]++;
	    }
	}

	new zones[sizeof(gZone)];
	for (new i, j = sizeof(gZone); i < j; i++)
	{
	    zones[gZone[i][zoneOwner]]++;
	}

	new text[150];
	for (new i; i < MAX_TEAMS; i++)
	{
	    format(text, sizeof(text), "Team {%06x}%s(%i) "WHITE", currently owning "RED"%i/%i "WHITE"zones and "RED"%i "WHITE"players", gTeam[i][teamColor] >>> 8, gTeam[i][teamName], i + 1, zones[i], sizeof(gZone), players[i]);
	    SendClientMessage(playerid, COLOR_WHITE, text);
	}

	return 1;
}

CMD:zones(playerid)
{
	if (GetPlayerTeam(playerid) < 0 || GetPlayerTeam(playerid) >= MAX_TEAMS)
	{
	    return 1;
	}

	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	new buf[100];
	new info[(sizeof(gZone) - 1) * sizeof(buf)];
	new team = GetPlayerTeam(playerid);
	for (new i, j = sizeof(gZone); i < j; i++)
	{
	    if (team == gZone[i][zoneOwner])
	    {
		    if (gZone[i][zoneAttacker] == INVALID_PLAYER_ID)
		    {
				format(buf, sizeof(buf), ""GREEN"%s", gZone[i][zoneName]);
			}
			else
		    {
		    	format(buf, sizeof(buf), ""TOMATO"%s (under attack by %s)", gZone[i][zoneName], gTeam[GetPlayerTeam(gZone[i][zoneAttacker])][teamName]);
		    }
		    strcat(info, buf);
		    strcat(info, "\n");
	    }
	}

	if (! info[0])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "Your team doesn't own any zone.");
	}

	ShowPlayerDialog(playerid, 0, DIALOG_STYLE_LIST, "Zones under your team's control:", info, "Close", "");

	return 1;
}

CMD:rank(playerid)
{
	new buf[150];
	format(buf, sizeof(buf), "Your rank: %s (%i), Score: %i, Health: %f, Armour: %f.", gRank[pRank[playerid]][rankName], pRank[playerid], gRank[pRank[playerid]][rankScore], gRank[pRank[playerid]][rankHealth], gRank[pRank[playerid]][rankArmour]);
	SendClientMessage(playerid, COLOR_YELLOW, buf);

	return 1;
}

CMD:ranks(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	new buf[100];
	new info[(sizeof(gRank) - 1) * sizeof(buf)];
	for (new i, j = sizeof(gRank); i < j; i++)
	{
	    if (pRank[playerid] > i)
	    {
		    strcat(info, GREEN);
		}
	    else if (pRank[playerid] == i)
	    {
		    strcat(info, CYAN);
		}
		else
		{
		    strcat(info, TOMATO);
		}
		format(buf, sizeof(buf), "%i. %s (unlock at %i score)", i, gRank[i][rankName], gRank[i][rankScore]);
		strcat(info, buf);
		strcat(info, "\n");
	}
	ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Ranks info.", info, "Close", "");

	return 1;
}

CMD:chelp(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	new buf[150];
	strcat(buf, "- ");
	strcat(buf, gClass[pClass[playerid]][className]);
	strcat(buf, " class help -");
	SendClientMessage(playerid, COLOR_GREEN, buf);

	switch (pClass[playerid])
	{
	    case 0:
	    {
			SendClientMessage(playerid, COLOR_WHITE, "/armour or hold AIM and press MMB to distribute armour (50%) to teammates.");
	    }
	    case 1:
	    {
			SendClientMessage(playerid, COLOR_WHITE, "/heal or hold AIM and press MMB to distribute health (50%) to teammates.");
	    }
	    case 2:
	    {
			SendClientMessage(playerid, COLOR_WHITE, "Invisible on map.");
	    }
		case 3:
	    {
			SendClientMessage(playerid, COLOR_WHITE, "Ability to drive rhino; /fix to repair nearest team vehicle; /build to create RC vehicles.");
	    }
		case 4:
	    {
			SendClientMessage(playerid, COLOR_WHITE, "Ability to drive sea sparrow, hunter and hydra.");
	    }
		case 5:
	    {
			SendClientMessage(playerid, COLOR_WHITE, "/dis to disguize in enemy skins; /undis to undisguize to your team; /rob to rob cash from nearest enemy;");
			SendClientMessage(playerid, COLOR_WHITE, "Press SUBMISSION key (2) to stab enemy drivers in vehicle as a passenger.");
	    }
	}

	return 1;
}

CMD:unlocks(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	new buf[100];
	new info[sizeof(buf) * 7];
	format(buf, sizeof(buf), ""WHITE"Unlock class "GREEN"%s "WHITE"on rank "GREEN"%i "WHITE"(%s).\n", gClass[0][className], gClass[0][classRank], gRank[gClass[0][classRank]][rankName]);
	strcat(info, buf);
	format(buf, sizeof(buf), ""WHITE"Unlock class "GREEN"%s "WHITE"on rank "GREEN"%i "WHITE"(%s)\n", gClass[1][className], gClass[1][classRank], gRank[gClass[1][classRank]][rankName]);
	strcat(info, buf);
	format(buf, sizeof(buf), ""WHITE"Unlock class "GREEN"%s "WHITE"on rank "GREEN"%i "WHITE"(%s).\n", gClass[2][className], gClass[2][classRank], gRank[gClass[2][classRank]][rankName]);
	strcat(info, buf);
	format(buf, sizeof(buf), ""WHITE"Unlock class "GREEN"%s "WHITE"on rank "GREEN"%i "WHITE"(%s).\n", gClass[3][className], gClass[3][classRank], gRank[gClass[3][classRank]][rankName]);
	strcat(info, buf);
	format(buf, sizeof(buf), ""WHITE"Unlock class "GREEN"%s "WHITE"on rank "GREEN"%i "WHITE"(%s).\n", gClass[4][className], gClass[4][classRank], gRank[gClass[4][classRank]][rankName]);
	strcat(info, buf);
	format(buf, sizeof(buf), ""WHITE"Unlock access to "GREEN"Hunters "WHITE"on rank "GREEN"%i "WHITE"(%s).\n", gClass[4][classRank] + 2, gRank[gClass[4][classRank] + 2][rankName]);
	strcat(info, buf);
	format(buf, sizeof(buf), ""WHITE"Unlock class "GREEN"%s "WHITE"on rank "GREEN"%i "WHITE"(%s).", gClass[5][className], gClass[5][classRank], gRank[gClass[5][classRank]][rankName]);
	strcat(info, buf);

	ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Unlocks info.", info, "Close", "");

	return 1;
}

bool:IsPlayerNearAnyEnemy(playerid)
{
	new team = GetPlayerTeam(playerid);

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	foreach (new i : Player)
	{
	    if (i != playerid)
	    {
	        if (team != GetPlayerTeam(i))
	        {
	            if (IsPlayerInRangeOfPoint(i, 20.0, x, y, z))
	            {
	                return true;
	            }
	        }
	    }
	}

	return false;
}

CMD:st(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (pDuel[playerid][duelActive])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You can't perform this command while in a duel.");
	}

	if (gettime() - pLastDamageTime[playerid] < 60)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 60 seconds after geting shot by an enemy to change team.");
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (IsPlayerNearAnyEnemy(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You cannot perform this command near enemies.");
	}

	pInClass[playerid] = true;
	SpawnPlayer(playerid);

	return 1;
}

CMD:sc(playerid)
{
	if (GetPlayerTeam(playerid) < 0 || GetPlayerTeam(playerid) >= MAX_TEAMS)
	{
	    return 1;
	}

	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (pDuel[playerid][duelActive])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You can't perform this command while in a duel.");
	}

	if (gettime() - pLastDamageTime[playerid] < 60)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 60 seconds after geting shot by an enemy to change class.");
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (IsPlayerNearAnyEnemy(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You cannot perform this command near enemies.");
	}

	new info[MAX_CLASSES * 75];
	for (new i; i < MAX_CLASSES; i++)
	{
		format(info, sizeof(info), "%s%s%s - Rank %i+\n", info, (pRank[playerid] >= gClass[i][classRank]) ? (GREEN) : (RED), gClass[i][className], gClass[i][classRank]);
	}
	ShowPlayerDialog(playerid, DIALOG_ID_CLASS, DIALOG_STYLE_LIST, "Class selection dialog", info, "Spawn", "Cancel");

	return 1;
}

CMD:ss(playerid)
{
	if (GetPlayerTeam(playerid) < 0 || GetPlayerTeam(playerid) >= MAX_TEAMS)
	{
	    return 1;
	}

	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	new info[sizeof(gZone) * 45];
	strcat(info, "TEAM BASE\n");
	for (new i, j = sizeof(gZone); i < j; i++)
	{
	    if (gZone[i][zoneOwner] == GetPlayerTeam(playerid))
	    {
	    	strcat(info, GREEN);
	    }
	    else if (gZone[i][zoneAttacker] != INVALID_PLAYER_ID)
	    {
	    	strcat(info, YELLOW);
	    }
	    else
	    {
	        strcat(info, RED);
	    }
	    strcat(info, gZone[i][zoneName]);
	    strcat(info, "\n");
	}
	ShowPlayerDialog(playerid, DIALOG_ID_SPAWN, DIALOG_STYLE_LIST, "Spawn selection dialog", info, "Selet", "Cancel");

	return 1;
}

CMD:sync(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

    if (pProtectTick[playerid] || pTrapped[playerid])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You cannot perform this command right now.");
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (IsPlayerNearAnyEnemy(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You cannot perform this command near enemies.");
	}

    SendClientMessage(playerid, COLOR_GREEN, "Syncing...");
    SyncPlayer(playerid);
    SendClientMessage(playerid, COLOR_GREEN, "Synchronized (/stats restored)!");

    return 1;
}

CMD:kill(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (pDuel[playerid][duelActive])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You can't perform this command while in a duel.");
	}

	if (gettime() - pLastDamageTime[playerid] < 60)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 60 seconds after geting shot by an enemy to change class.");
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (IsPlayerNearAnyEnemy(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You cannot perform this command near enemies.");
	}

	SetPlayerHealth(playerid, 0.0);
	SendClientMessage(playerid, COLOR_TOMATO, "You commited sucide.");

	return 1;
}

CMD:duel(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (gettime() - pLastDamageTime[playerid] < 60)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 60 seconds after geting shot by an enemy to change class.");
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (GetPlayerState(playerid) == PLAYER_STATE_WASTED)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must be spawned to use this command.");
	}

	if (pDuel[playerid][duelActive])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You are already in a duel.");
	}

	new target, bet;
	if (sscanf(params, "ii", target, bet))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /duel [player] [bet]");
	}

	if (target == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You can't duel with yourself.");
	}

	if (! IsPlayerConnected(target))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not connected.");
	}

	if (bet < 0)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The bet money can't be negative.");
	}

	if (bet > 0 && GetPlayerMoney(playerid) < bet)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You yourself don't have that much bet money.");
	}

	if (pInClass[target])
	{
	    return SendClientMessage(playerid, COLOR_FIREBRICK, "The specified player is not spawned right now.");
	}

	if (gettime() - pLastDamageTime[target] < 60)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "The player is shot recently, you have to wait 60 seconds to send him a duel request.");
	}

	if (GetPlayerState(target) == PLAYER_STATE_WASTED)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not spawned right now.");
	}

	if (pDuel[target][duelActive])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The player is already in a duel.");
	}

	if (bet > 0 && GetPlayerMoney(target) < bet)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The opponent don't have that much bet money.");
	}

	pDuel[playerid][duelPlayer] = target;
	pDuel[target][duelPlayer] = playerid;

	SendClientMessage(playerid, COLOR_YELLOW, "Please select your duel weapon from the dialog.");

	Menu_Show(playerid, MENU_ID_DUEL_WEAPON, "Select your duel weapon:", menuDuelModels, menuDuelLabels, 0xFF0000FF);
	Menu_EditRot(playerid, 0, menuDuelModels[0], 0.0, 0.0, -50.0, 0.5);
	Menu_EditRot(playerid, 1, menuDuelModels[1], 0.0, 0.0, -50.0, 0.5);
	Menu_EditRot(playerid, 2, menuDuelModels[2], 0.0, 0.0, 0.0, 0.5);
	Menu_EditRot(playerid, 3, menuDuelModels[3], 0.0, 0.0, 0.0, 0.5);
	Menu_EditRot(playerid, 4, menuDuelModels[4], 0.0, 0.0, -50.0, 0.5);
	Menu_EditRot(playerid, 5, menuDuelModels[5], 0.0, 0.0, -50.0, 0.5);
	Menu_EditRot(playerid, 6, menuDuelModels[6], 0.0, 0.0, -50.0, 0.5);
	Menu_EditRot(playerid, 17, menuDuelModels[17], 0.0, 0.0, -50.0, 1.3);
	Menu_EditRot(playerid, 18, menuDuelModels[18], 0.0, 0.0, -50.0, 1.3);
	Menu_EditRot(playerid, 19, menuDuelModels[19], 0.0, 0.0, -50.0, 1.3);
	return 1;
}

CMD:weapons(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	new slot;
	if (sscanf(params, "i", slot))
	{
	    return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /weapons (1-3)");
	}

	switch (slot)
	{
	    case 1:
	    {
	        if (pRank[playerid] < 10)
	        {
	    		return SendClientMessage(playerid, COLOR_TOMATO, "You must be atleast rank 10+ to edit personal weapon 1.");
	        }
	    }
	    case 2:
	    {
	        if (pRank[playerid] < 15)
	        {
	    		return SendClientMessage(playerid, COLOR_TOMATO, "You must be atleast rank 15+ to edit personal weapon 2.");
	        }
	    }
	    case 3:
	    {
	        if (pRank[playerid] < 20)
	        {
	    		return SendClientMessage(playerid, COLOR_TOMATO, "You must be atleast rank 20+ to edit personal weapon 3.");
	        }
	    }
	    default:
	    {
	    	return SendClientMessage(playerid, COLOR_TOMATO, "The slot must be between 1-3.");
	    }
	}

	pIdx[playerid] = slot;
	Menu_Show(playerid, MENU_ID_PERSONAL_WEAPON, "Select your personal weapon:", menuPersonalWeaponModels, menuPersonalWeaponLabels, 0xFF0000FF);
	Menu_EditRot(playerid, 0, menuPersonalWeaponModels[0], 0.0, 0.0, -50.0, 0.5);
	Menu_EditRot(playerid, 1, menuPersonalWeaponModels[1], 0.0, 0.0, -50.0, 0.5);
	Menu_EditRot(playerid, 2, menuPersonalWeaponModels[2], 0.0, 0.0, 0.0, 0.5);
	Menu_EditRot(playerid, 3, menuPersonalWeaponModels[3], 0.0, 0.0, 0.0, 0.5);
	Menu_EditRot(playerid, 4, menuPersonalWeaponModels[4], 0.0, 0.0, -50.0, 0.5);
	Menu_EditRot(playerid, 5, menuPersonalWeaponModels[5], 0.0, 0.0, -50.0, 0.5);
	Menu_EditRot(playerid, 6, menuPersonalWeaponModels[6], 0.0, 0.0, -50.0, 0.5);
	Menu_EditRot(playerid, 17, menuPersonalWeaponModels[17], 0.0, 0.0, -50.0, 1.3);
	Menu_EditRot(playerid, 18, menuPersonalWeaponModels[18], 0.0, 0.0, -50.0, 1.3);
	Menu_EditRot(playerid, 19, menuPersonalWeaponModels[19], 0.0, 0.0, -50.0, 1.3);

	return 1;
}

CMD:resetweapon(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	new slot;
	if (sscanf(params, "i", slot))
	{
	    return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /weapons (1-3)");
	}

	switch (slot)
	{
	    case 1:
	    {
	        if (pRank[playerid] < 10)
	        {
	    		return SendClientMessage(playerid, COLOR_TOMATO, "You must be atleast rank 10+ to edit personal weapon 1.");
	        }

	        yoursql_set_field_int(SQL:0, "users/weapon1", yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid)), 0);

			SendClientMessage(playerid, COLOR_GREEN, "You have reset your personal weapon 1 to unarmed.");
	    }
	    case 2:
	    {
	        if (pRank[playerid] < 15)
	        {
	    		return SendClientMessage(playerid, COLOR_TOMATO, "You must be atleast rank 15+ to edit personal weapon 2.");
	        }

	        yoursql_set_field_int(SQL:0, "users/weapon2", yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid)), 0);

			SendClientMessage(playerid, COLOR_GREEN, "You have reset your personal weapon 2 to unarmed.");
	    }
	    case 3:
	    {
	        if (pRank[playerid] < 20)
	        {
	    		return SendClientMessage(playerid, COLOR_TOMATO, "You must be atleast rank 20+ to edit personal weapon 3.");
	        }

	        yoursql_set_field_int(SQL:0, "users/weapon3", yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(playerid)), 0);

			SendClientMessage(playerid, COLOR_GREEN, "You have reset your personal weapon 3 to unarmed.");
	    }
	    default:
	    {
	    	return SendClientMessage(playerid, COLOR_TOMATO, "The slot must be between 1-3.");
	    }
	}

	return 1;
}

CMD:r(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (pDuel[playerid][duelActive])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You can't perform this command while in a duel.");
	}

	if (GetPlayerTeam(playerid) < 0 || GetPlayerTeam(playerid) >= MAX_TEAMS)
	{
	    return 1;
	}

	new text[100];
	if (sscanf(params, "s[100]", text))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /r [message]");
	}

	new buf[150];
	format(buf, sizeof(buf), "** (RADIO) (%i) %s: %s", playerid, ReturnPlayerName(playerid), text);
	foreach (new i : Player)
	{
	    if (HasSameTeam(playerid, i))
	    {
			SendClientMessage(i, COLOR_LIME, buf);
	    }
	}

	SetPlayerChatBubble(playerid, text, COLOR_LIME, 20.0, 10000);

	return 1;
}

CMD:s(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	new text[100];
	if (sscanf(params, "s[100]", text))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /s [message]");
	}

	new buf[150];
	format(buf, sizeof(buf), "** (%i) %s says: %s", playerid, ReturnPlayerName(playerid), text);

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	foreach (new i : Player)
	{
	    if (IsPlayerInRangeOfPoint(i, 50.0, x, y, z))
	    {
			SendClientMessage(i, COLOR_WHITE, buf);
	    }
	}

	SetPlayerChatBubble(playerid, text, COLOR_WHITE, 20.0, 10000);

	return 1;
}

CMD:givegun(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	new target, amount;
	if (sscanf(params, "ui", target, amount))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /givegun [player] [amount]");
	}

	if(amount < 1)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The amount limit must be greater than 0.");
	}

	if (! IsPlayerConnected(target))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not connected.");
	}

	if (target == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You can't give money to yourself.");
	}

	if (GetPlayerState(target) == PLAYER_STATE_WASTED)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not spawned.");
	}

	new weapon = GetPlayerWeapon(playerid);
	if (weapon == 0)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot distribute your wrist to players!");
	}

	new ammo = GetPlayerAmmo(playerid);
	if (ammo < ammo)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You yourself don't have that much ammo.");
	}

	SetPlayerAmmo(playerid, weapon, -ammo);
	GivePlayerWeapon(target, weapon, ammo);

	new weapon_name[35];
	GetWeaponName(weapon, weapon_name, sizeof(weapon_name));

	new buf[150];
	format(buf, sizeof(buf), "You have recieved a %s with %i ammo from %s(%i).", weapon_name, amount, ReturnPlayerName(playerid), playerid);
	SendClientMessage(target, COLOR_GREEN, buf);
	format(buf, sizeof(buf), "You have given a %s with %i ammo to %s(%i).", weapon_name, amount, ReturnPlayerName(target), target);
	SendClientMessage(playerid, COLOR_GREEN, buf);

	return 1;
}

CMD:givemoney(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	new target, amount;
	if (sscanf(params, "ui", target, amount))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /givemoney [player] [amount]");
	}

	if(amount < 1)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The amount limit must be greater than 0.");
	}

	if (! IsPlayerConnected(target))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not connected.");
	}

	if (target == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You can't give money to yourself.");
	}

	if (GetPlayerState(target) == PLAYER_STATE_WASTED)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not spawned.");
	}

	if (GetPlayerMoney(playerid) < amount)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You yourself don't have that much money.");
	}

	GivePlayerMoney(playerid, -amount);
	GivePlayerMoney(target, amount);

	new buf[150];
	format(buf, sizeof(buf), "You have recieved $%i from %s(%i).", amount, ReturnPlayerName(playerid), playerid);
	SendClientMessage(target, COLOR_GREEN, buf);
	format(buf, sizeof(buf), "You have given $%i to %s(%i).", amount, ReturnPlayerName(target), target);
	SendClientMessage(playerid, COLOR_GREEN, buf);

	return 1;
}

CMD:help(playerid)
{
	ShowPlayerDialog(playerid, DIALOG_ID_HELP, DIALOG_STYLE_LIST, "Server helpline:",
		"About gamemode\n\
		 About server\n\
		 How to reduce ping?\n\
		 How to unlock new features/earn?\n\
		 How to become a Premium User?\n\
		 How to become an Administrator?\n\
		 Commands list </cmds>\n\
		 Server rules </rules>",
 	"Open", "Close");
	return 1;
}

CMD:rules(playerid)
{
    new info[1024];
	strcat(info, ""WHITE"1. Server don't permit us of hacks, cheats or any methods of "TOMATO"UNFAIR "WHITE"gameplay.\n");
	strcat(info, "2. Always respect every player, don't provoke people or insult them directly in main chat.\n");
	strcat(info, "3. Any sort of level boosting in an unfair method is strictly restricted (eg. score farming).\n");
	strcat(info, "4. Make use of "GREEN"/report "WHITE"against rulebreakers instead of announcing in main chat.\n");
	strcat(info, "5. Advertisements are not allowed, neither in chat nor in private.\n");
	strcat(info, "6. Don't misuse your admin or donor commands if provided.\n");
	strcat(info, "7. Don't ask for support for your admin application (if applied), because this will immediately let us close it.\n\n");
	strcat(info, ""TOMATO"NOTE: Violation of any of these rules will result a punishment against you.");

	ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Server rules:", info, "Close", "");

	return 1;
}

CMD:cmds(playerid)
{
    new info[1024];
	strcat(info, ""GREEN"General:\n");
	strcat(info, ""WHITE"/changename, /changepass, /autologin, /help, /rules, /site, /ask, /changelog,\n");
	strcat(info, "/stats, /admins, /vips, /time, /minigame, /richlist, /scorelist, /players, /id\n\n");
	strcat(info, ""GREEN"Chat related:\n");
	strcat(info, ""WHITE"/pm, /dnd, /reply, /s, /r, /ask, /report, /ignore, /unignore\n\n");
	strcat(info, ""GREEN"Combat related:\n");
	strcat(info, ""WHITE"/st, /sc, /ss, /zones, /missions, /objective, /chelp, /unlocks, /ranks, /mk, /as,\n");
	strcat(info, "/weapons, /resetweapon, /givegun, /givemoney, /carepack, /lootpack, /dropgun\n\n");
	strcat(info, ""GREEN"Inventory commands:\n");
	strcat(info, ""WHITE"/med, /trap, /destroytrap, /dynamite, /destroydynamite, /det, /ammo, /drug, /camo,\n");
	strcat(info, "/spike, /destroyspike, /music, /mine, /destroymine, /jacket, /mask\n\n");
	strcat(info, ""GREEN"Admin commands:\n");
	strcat(info, ""WHITE"/asay, /acmds, /adminarea, /aweaps, /spec\n\n");
	strcat(info, ""GREEN"Premium:\n");
	strcat(info, ""WHITE"/donate, /dcmds, /dlabel");

 	ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Server commands:", info, "Close", "");

	return 1;
}

CMD:dcmds(playerid)
{
    new info[500];
	strcat(info, ""CYAN"Premium user commands:\n");
	strcat(info, ""WHITE"/dlabel, /dskin, /dbike, /dcar, /dbmx, /dplane, /dnos, /dhyd, /dsupply, /dcolor\n\n");
	strcat(info, ""CYAN"Premium user features\n");
	strcat(info, ""WHITE"- Extra score and money on kills.\n");
	strcat(info, ""WHITE"- Full health and armour on every spawn.\n");
	strcat(info, ""WHITE"- 3x ammo for each weapon on every spawn.\n\n");
	strcat(info, ""GREY"Type /donate to see 'How to donate?'!\n");

 	ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Donor/Premium features & commands:", info, "Close", "");

	return 1;
}

CMD:donate(playerid)
{
    new info[750];
	strcat(info, ""CYAN"Premium account steps: (steps to activate premium account status)\n\n");
	strcat(info, ""WHITE"1. Register your Username on forums if you haven't done.\n");
	strcat(info, ""WHITE"2. You must verify your "GREEN"PayPal account"WHITE", because we support PayPal only for cash transfer, currently.\n");
	strcat(info, ""WHITE"3. Transfer an amount of "RED"CA$35 "WHITE"to the specified official game PayPal account.\n");
	strcat(info, ""WHITE"4. Once your transaction is recieved and verified, you will recieve a mail on both of your forum account and email.\n");
	strcat(info, ""WHITE"5. The email verifies your "CYAN"VIP "WHITE"status in game and you have the premium status for "CYAN"1 month"WHITE".\n\n");
	strcat(info, ""GREY"NOTE: CA$35 is for a single month premium achievement.");

 	ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Donor/Premium steps:", info, "Close", "");

	return 1;
}

CMD:mk(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	new text[150];
	format(text, sizeof(text), "** (RADIO) (%i) %s: I have marked my radar icon with YELLOW color.", playerid, ReturnPlayerName(playerid));

	new team = GetPlayerTeam(playerid);
	foreach (new i : Player)
	{
		if (GetPlayerTeam(i) == team)
		{
		    SetPlayerMarkerForPlayer(i, playerid, COLOR_YELLOW);

		    SendClientMessage(i, COLOR_LIME, text);
		    SendClientMessage(i, COLOR_LIME, "** The player will be visible on your radar if he/she is a sniper (/mk).");
		}
	}

	return 1;
}

CallAirstrike(playerid, Float:x, Float:y, Float:z)
{
	pAirstrike[playerid][asPosX] = x;
	pAirstrike[playerid][asPosY] = y;
	pAirstrike[playerid][asPosZ] = z;

    new flare = CreateDynamicObject(18728, x, y, z, 0.0, 0.0, 0.0);
	SetTimerEx("OnAirstrikeFlareExpire", 3500, false, "i", flare);

	pAirstrike[playerid][asPlaneObject] = CreateDynamicObject(14553, x - 55.0, y, z + 100.0, 90.0, 0.0, 0.0);
 	MoveDynamicObject(pAirstrike[playerid][asPlaneObject], x + 250.0, y, z + 100.0, 25.0);

  	SetTimerEx("OnAirstrikeBombDrop", 2000, false, "ifff", playerid, x, y, z);
}

forward OnAirstrikeFlareExpire(flare);
public  OnAirstrikeFlareExpire(flare)
{
	DestroyDynamicObject(flare);
}

forward OnAirstrikeBombDrop(playerid, Float:x, Float:y, Float:z);
public  OnAirstrikeBombDrop(playerid, Float:x, Float:y, Float:z)
{
	pAirstrike[playerid][asObject][0] = CreateDynamicObject(1636, x, y, z + 100.0, 0.0, 0.0, 0.0);
	pAirstrike[playerid][asObject][1] = CreateDynamicObject(1636, x, y + 4.0, z + 100.0, 0.0, 0.0, 0.0);
	pAirstrike[playerid][asObject][2] = CreateDynamicObject(1636, x, y + 8.0, z + 100.0, 0.0, 0.0, 0.0);
	MoveDynamicObject(pAirstrike[playerid][asObject][0], x, y, z, 50.0);
	MoveDynamicObject(pAirstrike[playerid][asObject][1], x, y + 2.0, z, 50.0);
	MoveDynamicObject(pAirstrike[playerid][asObject][2], x, y + 4.0, z, 50.0);
}

CMD:as(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (GetPlayerMoney(playerid) < 15000)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You cannot afford an airstrike worth $15000.");
	}

	if (gettime() - pAirstrike[playerid][asLastStrike] < 60 * 5)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 5 minutes after using an airstrike.");
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	new Float:CA_z;
	CA_FindZ_For2DCoord(x, y, CA_z);

	if (CA_z > z + 5.0 || CA_z < z - 5.0)
	{
	    return ShowPlayerDialog(playerid, DIALOG_ID_AIRSTRIKE, DIALOG_STYLE_MSGBOX, "Airstrike warning:", ""WHITE"Your airstrike request will be accomplished but the strike can't reach your current position.\nPress '"GREEN"ACCEPT"WHITE"' to continue and make an airstrike on possible ground (above you).\n\n"TOMATO"This dialog usually appears when you are inside a building with roof.", "Accept", "Cancel");
	}

	CallAirstrike(playerid, x, y, CA_z);
	pAirstrike[playerid][asLastStrike] = gettime();
	pAirstrike[playerid][asCalled] = true;

	GivePlayerMoney(playerid, -15000);

	SendClientMessage(playerid, COLOR_YELLOW, "You have requested an airstrike at your position, get cover.");
	SendClientMessage(playerid, COLOR_YELLOW, "The strike will happen in 5 seconds at the flare position.");
	SendClientMessage(playerid, COLOR_TOMATO, "The airstrike cost you -$15000.");

	new text[150];
	format(text, sizeof(text), "** (%i) %s have requested an airstrike.", playerid, ReturnPlayerName(playerid));

	foreach (new i : Player)
	{
		if (IsPlayerInRangeOfPoint(i, 10.0, x, y, z))
		{
		    SendClientMessage(i, COLOR_GREY, text);
		}
	}

	return 1;
}

CallCarepack(playerid, Float:x, Float:y, Float:z)
{
	pCarepack[playerid][cpPosX] = x;
	pCarepack[playerid][cpPosY] = y;
	pCarepack[playerid][cpPosZ] = z;

    new flare = CreateDynamicObject(18728, x, y, z, 0.0, 0.0, 0.0);
	SetTimerEx("OnCarepackFlareExpire", 5000, false, "i", flare);

	pCarepack[playerid][cpPlaneObject] = CreateDynamicObject(14553, x - 55.0, y, z + 100.0, 90.0, 0.0, 0.0);
 	MoveDynamicObject(pCarepack[playerid][cpPlaneObject], x + 250.0, y, z + 100.0, 25.0);

  	SetTimerEx("OnCarepackObjectDrop", 2000, false, "ifff", playerid, x, y, z);
}

forward OnCarepackFlareExpire(flare);
public  OnCarepackFlareExpire(flare)
{
	DestroyDynamicObject(flare);
}

forward OnCarepackObjectDrop(playerid, Float:x, Float:y, Float:z);
public  OnCarepackObjectDrop(playerid, Float:x, Float:y, Float:z)
{
	pCarepack[playerid][cpObject] = CreateDynamicObject(18849, x, y, z + 100.0, 0.0, 0.0, 0.0);
	MoveDynamicObject(pCarepack[playerid][cpObject], x, y, z + 7.5, 10.0);
}

CMD:carepack(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (GetPlayerMoney(playerid) < 20000)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You cannot afford a carepack worth $20000.");
	}

	if (gettime() - pCarepack[playerid][cpLastDrop] < 60 * 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must wait 2 minutes after calling a carepack.");
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	new Float:CA_z;
	CA_FindZ_For2DCoord(x, y, CA_z);

	if (CA_z > z + 5.0 || CA_z < z - 5.0)
	{
	    return ShowPlayerDialog(playerid, DIALOG_ID_CAREPACK, DIALOG_STYLE_MSGBOX, "Carepack warning:", ""WHITE"Your carepack request will be accomplished but the drop can't reach your current position.\nPress '"GREEN"ACCEPT"WHITE"' to continue and make the drop on possible ground (above you).\n\n"TOMATO"This dialog usually appears when you are inside a building with roof.", "Accept", "Cancel");
	}

	if (IsValidDynamicObject(pCarepack[playerid][cpPlaneObject]))
	{
	    DestroyDynamicObject(pCarepack[playerid][cpPlaneObject]);
	}
	if (IsValidDynamicObject(pCarepack[playerid][cpObject]))
	{
	    DestroyDynamicObject(pCarepack[playerid][cpObject]);
	}
	if (IsValidDynamic3DTextLabel(pCarepack[playerid][cpLabel]))
	{
	    DestroyDynamic3DTextLabel(pCarepack[playerid][cpLabel]);
	}

	CallCarepack(playerid, x, y, CA_z);
	pCarepack[playerid][cpLastDrop] = gettime();
	pCarepack[playerid][cpCalled] = true;

	GivePlayerMoney(playerid, -20000);

	SendClientMessage(playerid, COLOR_YELLOW, "You have requested a carepack at your position, you shall type /lootpack to gather items from it.");
	SendClientMessage(playerid, COLOR_YELLOW, "The package will be dropped within 15 seconds at the flare position.");
	SendClientMessage(playerid, COLOR_TOMATO, "The carepack cost you -$20000.");

	new text[150];
	format(text, sizeof(text), "** (%i) %s have requested a carepack.", playerid, ReturnPlayerName(playerid));

	foreach (new i : Player)
	{
		if (IsPlayerInRangeOfPoint(i, 10.0, x, y, z))
		{
		    SendClientMessage(i, COLOR_GREY, text);
		}
	}

	return 1;
}

CMD:lootpack(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	foreach (new i : Player)
	{
	    if (pCarepack[i][cpCalled])
		{
		    if (IsValidDynamicObject(pCarepack[i][cpObject]) && ! IsDynamicObjectMoving(pCarepack[i][cpObject]))
		    {
				if (IsPlayerInRangeOfPoint(playerid, 5.0, pCarepack[i][cpPosX], pCarepack[i][cpPosY], pCarepack[i][cpPosZ]))
				{
					if (i != playerid)
					{
					    return SendClientMessage(playerid, COLOR_TOMATO, "You can only loot the carepack called by you.");
					}
					else
					{
					    ApplyAnimation(playerid, "MISC", "pickup_box", 4.1, 0, 0, 0, 0, 0, 1);

					    SendClientMessage(playerid, COLOR_GREEN, "You have successfully looted your carepack.");
					    SendClientMessage(playerid, COLOR_GREEN, "Items recieved: MP5, Combat shotgun, Desert eagle, Health and Armour.");

					    SetPlayerHealth(playerid, 100.0);
					    SetPlayerArmour(playerid, 100.0);
					    GivePlayerWeapon(playerid, 29, 200);
					    GivePlayerWeapon(playerid, 27, 100);
					    GivePlayerWeapon(playerid, 24, 150);

					    DestroyDynamicObject(pCarepack[i][cpPlaneObject]);
						DestroyDynamicObject(pCarepack[i][cpObject]);
			 			DestroyDynamic3DTextLabel(pCarepack[i][cpLabel]);

			 			pCarepack[i][cpCalled] = false;
					    return 1;
					}
				}
			}
		}
	}

	SendClientMessage(playerid, COLOR_TOMATO, "You aren't near any carepack.");
	return 1;
}

public OnDynamicObjectMoved(objectid)
{
	foreach (new i : Player)
	{
    	if (pAirstrike[i][asCalled])
    	{
			if (objectid == pAirstrike[i][asPlaneObject])
			{
			   	DestroyDynamicObject(objectid);

				break;
			}
			else if (objectid == pAirstrike[i][asObject][0])
			{
			   	DestroyDynamicObject(objectid);
			   	DestroyDynamicObject(pAirstrike[i][asObject][1]);
			   	DestroyDynamicObject(pAirstrike[i][asObject][2]);

				new Float:x = pAirstrike[i][asPosX];
				new Float:y = pAirstrike[i][asPosY];
				new Float:z = pAirstrike[i][asPosZ];

			   	CreateExplosion(x, y, z, 6, 5);
			   	CreateExplosion(x, y + 2, z, 6, 5);
		     	CreateExplosion(x, y + 4, z, 6, 5);
		        CreateExplosion(x, y + 6, z, 6, 5);
		        CreateExplosion(x, y + 8, z, 6, 5);

				new text[150];
		        new team = GetPlayerTeam(i);
		        foreach (new p : Player)
		        {
		            if (team != GetPlayerTeam(p) && IsPlayerInRangeOfPoint(p, 5.0, x, y, z))
		            {
		                SendClientMessage(p, COLOR_TOMATO, "You were killed in the airstrike.");
					    NotifyPlayer(p, "You got ~r~Airstriked", 5000);

					    format(text, sizeof(text), "Your dynamite killed %s(%i), +$500.", ReturnPlayerName(p), p);
						SendClientMessage(i, COLOR_GREEN, text);
						GivePlayerMoney(i, 500);

				    	SetPlayerHealth(p, 0.0);

						PlayerPlaySound(p, 1055, 0.0, 0.0, 0.0);
						PlayerPlaySound(i, 1055, 0.0, 0.0, 0.0);

						pKiller[p][0] = i;
						pKiller[p][1] = 51;
		            }
		        }

		        pAirstrike[i][asCalled] = false;

				break;
			}
		}
		else if (pCarepack[i][cpCalled])
    	{
			if (objectid == pCarepack[i][cpPlaneObject])
			{
			   	DestroyDynamicObject(objectid);

				break;
			}
			else if (objectid == pCarepack[i][cpObject])
			{
			    new Float:x = pCarepack[i][cpPosX];
				new Float:y = pCarepack[i][cpPosY];
				new Float:z = pCarepack[i][cpPosZ];

			    new text[150];
			    format(text, sizeof(text), "Carepackage\n"WHITE"Called by %s(%i)\nType /lootpack to use it", ReturnPlayerName(i), i);
			    pCarepack[i][cpLabel] = CreateDynamic3DTextLabel(text, SET_ALPHA(COLOR_GREEN, 100), x, y, z, 5.0, .worldid = 0);

				break;
			}
		}
	}

	return 1;
}

GetVehicleModelIDFromName(name[])
{
	for (new i, j = sizeof(gVehicleModelNames); i < j; i++)
	{
		if (strfind(gVehicleModelNames[i], name, true) != -1)
		{
			return i + 400;
		}
	}
	return -1;
}

GetWeaponIDFromName(name[])
{
	for(new i; i <= 46; i++)
	{
		switch(i)
		{
			case 0, 19, 20, 21, 44, 45:
			{
				continue;
			}
			default:
			{
				new weapon_name[35];
				GetWeaponName(i, weapon_name, sizeof(weapon_name));
				if (strfind(name, weapon_name, true) != -1)
				{
					return i;
				}
			}
		}
	}
	return -1;
}

isnumeric(str[])
{
	new ch, i;
	while ((ch = str[i++])) if (!('0' <= ch <= '9'))
	{
		return false;
	}
	return true;
}

//Admin level 1+
CMD:acmds(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
        return SendClientMessage(playerid, COLOR_TOMATO, "You must be an admin to use this command.");
    }

	new info[3024];
	strcat(info, ""HOT_PINK"Moderator (Level 1):\n");
  	strcat(info, ""WHITE"/acmds, /onduty, /offduty, /spec, /specoff, /adminarea, /weaps, /reports, /repair, /addnos,\n");
  	strcat(info, "/warn, /resetwarns, /flip, /ip, /spawn, /goto, /setweather, /settime, /kick, /asay\n");
  	strcat(info, "Use `"GREEN"!"WHITE"' for admin chat [eg. ! hello].\n\n");

	if (pStats[playerid][userAdmin] >= 2 || IsPlayerAdmin(playerid))
	{
		strcat(info, ""HOT_PINK"Junior Administrator (Level 2):\n");
  		strcat(info, ""WHITE"/ann, /ann2, /jetpack, /aka, /aweaps, /text, /carhealth, /eject, /carpaint, /carcolor,\n");
  		strcat(info, "/givecar, /car, /akill, /jailed, /jail, /unjail, /muted, /mute, /unmute, /setskin, /cc, /aheal, /aarmour,\n");
  		strcat(info, "/setinterior, /setworld, /slap, /explode, /disarm, /ban, /ipban, /oban, /unban, /searchban\n\n");
	}
	if (pStats[playerid][userAdmin] >= 3 || IsPlayerAdmin(playerid))
	{
		strcat(info, ""HOT_PINK"Senior Administrator (Level 3):\n");
		strcat(info, ""WHITE"/get, /write, /force, /healall, /armourall, /fightstyle, /sethealth, /setarmour, /god, /godcar, /freeze,\n");
		strcat(info, "/unfreeze, /giveweapon, /setcolor, /setcash, /setscore, /givecash, /givescore, /spawncar, /destroycar, /spawncars,\n");
		strcat(info, "/removedrops, /setkills, /setdeaths, /giveallhelmet, /giveallmask\n\n");
	}
	if (pStats[playerid][userAdmin] >= 4 || IsPlayerAdmin(playerid))
	{
		strcat(info, ""HOT_PINK"Lead Administrator (Level 4):\n");
		strcat(info, ""WHITE"/fakedeath, /muteall, /unmuteall, /giveallscore, /giveallcash, /setalltime, /setallweather,\n");
		strcat(info, "/clearwindow, /giveallweapon, /object, /destroyobject, /editobject, /rban, /ripban, /roban, /event,\n");
		strcat(info, ""RED"Event Commands:\n");
		strcat(info, ""WHITE"/healteam, /armourteam, /disarmteam, /setteamhealth, /setteamarmour, /freezeteam, /unfreezeteam,\n");
		strcat(info, "/getteam, /spawnteam, /giveteamscore, /giveteamcash, /giveteamweapon, /giveteamhelmet, /giveteammask\n");
  		strcat(info, "Use `"GREEN"@"WHITE"' for admin level 4+ chat [eg. @ hello].\n\n");
	}
	if (pStats[playerid][userAdmin] >= 5 || IsPlayerAdmin(playerid))
	{
		strcat(info, ""HOT_PINK"Server Manager (Level 5):\n");
		strcat(info, ""WHITE"/gmx, /fakechat, /setlevel, /setpremium\n");
  		strcat(info, "Use `"GREEN"#"WHITE"' for admin level 5+ chat [eg. # hello].");
	}

	ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Administrative commands:", info, "Close", "");
	return 1;
}

CMD:onduty(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

	if (pStats[playerid][userOnDuty])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You are already on admin duty.");
	}

    new i = random(sizeof(gAdminSpawn));
    SetPlayerPos(playerid, gAdminSpawn[i][0], gAdminSpawn[i][1], gAdminSpawn[i][2]);
  	SetPlayerFacingAngle(playerid, gAdminSpawn[i][3]);

	SetPlayerSkin(playerid, 217);
	SetPlayerColor(playerid, COLOR_HOT_PINK);
 	SetPlayerTeam(playerid, 100);
 	ResetPlayerWeapons(playerid);
 	GivePlayerWeapon(playerid, 38, 999999);
 	if (! pStats[playerid][userGod])
  	{
   		pStats[playerid][userGod] = true;
    }
 	if (! pStats[playerid][userGodCar])
  	{
   		pStats[playerid][userGodCar] = true;
    }
    SetPlayerHealth(playerid, FLOAT_INFINITY);
    SetVehicleHealth(GetPlayerVehicleID(playerid), FLOAT_INFINITY);

    pStats[playerid][userOnDuty] = true;

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, "You are on admin duty, type /offduty to switch off duty.");
	return 1;
}

CMD:offduty(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

	if (! pStats[playerid][userOnDuty])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You are already off admin duty.");
	}

    pStats[playerid][userOnDuty] = false;
    pStats[playerid][userGod] = false;
    pStats[playerid][userGodCar] = false;

    SpawnPlayer(playerid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, "You are off admin duty.");
	return 1;
}

CMD:spec(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

    new targetid;
    if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /spec [player]");
	}

	if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You can't spectate to yourself.");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pInClass[targetid] && GetPlayerState(targetid) == PLAYER_STATE_SPECTATING)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not spawned.");
	}

	PlayerSpectatePlayer(playerid, targetid);

	new buf[150];
	format(buf, sizeof(buf), "You are now spectating %s(%i).", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, "You can type /specoff when you wish to stop spectating.");
    return 1;
}

CMD:specoff(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

    if (GetPlayerState(playerid) != PLAYER_STATE_SPECTATING)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You are not spectating.");
	}

	TogglePlayerSpectating(playerid, false);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, "You have stopped spectating now.");
    return 1;
}

CMD:adminarea(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

	GameTextForPlayer(playerid, "~b~Adminarea", 3000, 3);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, "You have teleported to admin area.");

	SetPlayerPos(playerid, 377, 170, 1008);
	SetPlayerFacingAngle(playerid, 90);
	SetPlayerInterior(playerid, 3);
	SetPlayerVirtualWorld(playerid, 0);
    return 1;
}

CMD:weaps(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

    new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /weaps [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	new buf[150];
	format(buf, sizeof(buf), "%s(%i)'s weapons:", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);

	new w, a;
	new count;
	new name[35];
	buf[0] = EOS;
	for (new i; i < 13; i++)
	{
		GetPlayerWeaponData(targetid, i, w, a);
		if (w && a)
		{
		    GetWeaponName(w, name, sizeof(name));
		    if (buf[0])
		    {
		    	format(buf, sizeof(buf), "%s, %s (%i)", buf, name, a);
			}
			else
			{
		    	format(buf, sizeof(buf), "%s (%i)", buf, name, a);
			}

		    count++;
			if (count >= 5)
			{
				SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);

			    count = 0;
				buf[0] = EOS;
			}
		}
	}
	return 1;
}

CMD:reports(playerid)
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

	new buf[100];
	new info[sizeof(buf) * (MAX_REPORTS + 1)];
	strcat(info, ""RED"CLEAR REPORTS\n");
	for (new i; i < MAX_REPORTS; i++)
	{
	    if (gReport[i][rAgainst][0])
	    {
		    if (gReport[i][rChecked])
		    {
		    	format(buf, sizeof(buf), ""GREEN"[Unread] "WHITE"%s(%i) report against %s(%i) - %s", gReport[i][rAgainst], gReport[i][rAgainstId], gReport[i][rBy], gReport[i][rById], gReport[i][rTime]);
		    }
		    else
		    {
		        format(buf, sizeof(buf), "%s(%i) report against %s(%i) - %s", gReport[i][rAgainst], gReport[i][rAgainstId], gReport[i][rBy], gReport[i][rById], gReport[i][rTime]);
		    }
		    strcat(info, buf);
		    strcat(info, "\n");
	    }
	}

	ShowPlayerDialog(playerid, DIALOG_ID_REPORTS, DIALOG_STYLE_MSGBOX, "Reports log:", info, "Open", "Close");
	return 1;
}

CMD:repair(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

    new targetid;
    if (! sscanf(params, "u", targetid))
    {
		if (! IsPlayerConnected(targetid))
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
		}

		if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
		}

		if (! IsPlayerInAnyVehicle(targetid))
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "Player is not in a vehicle.");
		}

		new vehicleid = GetPlayerVehicleID(targetid);
		RepairVehicle(vehicleid);
	  	SetVehicleHealth(vehicleid, 1000.0);

		GameTextForPlayer(targetid, "~b~Vehicle repaired", 5000, 3);
		PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
		PlayerPlaySound(targetid, 1133, 0.0, 0.0, 0.0);

		new buf[150];
		format(buf, sizeof(buf), "You have repaired %s(%i)'s vehicle.", ReturnPlayerName(targetid), targetid);
		SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
		format(buf, sizeof(buf), "Admin %s(%i) has repaired your vehicle.", ReturnPlayerName(playerid), playerid);
		SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
    }
    else
    {
		if (! IsPlayerInAnyVehicle(playerid))
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "You must be in a vehicle to use this command.");
		}

		new vehicleid = GetPlayerVehicleID(playerid);
		RepairVehicle(vehicleid);
	  	SetVehicleHealth(vehicleid, 1000.0);

		GameTextForPlayer(playerid, "~b~Vehicle repaired", 5000, 3);
		PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);

		SendClientMessage(playerid, COLOR_DODGER_BLUE, "You have repaired your vehicle.");
		SendClientMessage(playerid, COLOR_KHAKI, "TIP: You can repair other player's vehicle by /repair [player].");
    }
	return 1;
}

CMD:addnos(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

    new targetid;
    if (! sscanf(params, "u", targetid))
    {
		if (! IsPlayerConnected(targetid))
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
		}

		if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
		}

		if (! IsPlayerInAnyVehicle(targetid))
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "Player is not in a vehicle.");
		}

		new vehicleid = GetPlayerVehicleID(targetid);
		switch (GetVehicleModel(vehicleid))
		{
			case 448,461,462,463,468,471,509,510,521,522,523,581,586,449:
			{
				return SendClientMessage(playerid, COLOR_TOMATO, "You cannot add nitros to the current player's vehicle.");
			}
		}

		AddVehicleComponent(vehicleid, 1010);

		GameTextForPlayer(targetid, "~b~Nitros added", 5000, 3);
		PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
		PlayerPlaySound(targetid, 1133, 0.0, 0.0, 0.0);

		new buf[150];
		format(buf, sizeof(buf), "You have fliped %s(%i)'s vehicle.", ReturnPlayerName(targetid), targetid);
		SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
		format(buf, sizeof(buf), "Admin %s(%i) has flipped your vehicle.", ReturnPlayerName(playerid), playerid);
		SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
    }
    else
    {
		if (! IsPlayerInAnyVehicle(playerid))
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "You must be in a vehicle to use this command.");
		}

		new vehicleid = GetPlayerVehicleID(playerid);
		switch (GetVehicleModel(vehicleid))
		{
			case 448,461,462,463,468,471,509,510,521,522,523,581,586,449:
			{
				return SendClientMessage(playerid, COLOR_TOMATO, "You cannot add nitros to this vehicle.");
			}
		}

        AddVehicleComponent(vehicleid, 1010);

		GameTextForPlayer(playerid, "~b~Nitros added", 5000, 3);
		PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);

		SendClientMessage(playerid, COLOR_DODGER_BLUE, "You have added nitros to your vehicle.");
		SendClientMessage(playerid, COLOR_KHAKI, "TIP: You can add nos to other player's vehicle by /addnos [player].");
    }
	return 1;
}

CMD:warn(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

	new targetid, reason[128];
    if (sscanf(params, "uS(No reason specified)[128]", targetid, reason))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /warn [player] [*reason]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

    if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot warn yourself.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has warned %s(%i) [Reason: %s] (warnings: %i/%i)", ReturnPlayerName(playerid), playerid, ReturnPlayerName(targetid), targetid, reason, pStats[targetid][userWarnings], MAX_WARNINGS);
	SendClientMessageToAll(COLOR_YELLOW, buf);

	pStats[targetid][userWarnings] += 1;
	if (pStats[targetid][userWarnings] >= MAX_WARNINGS)
	{
		format(buf, sizeof(buf), "%s(%i) has been automatically kicked [Reason: Exceeded maximum warnings] (Warnings: %i/%i)", ReturnPlayerName(targetid), targetid, pStats[targetid][userWarnings], MAX_WARNINGS);
	    SendClientMessageToAll(COLOR_RED, buf);
		Kick(targetid);
		return 1;
	}

	format(buf, sizeof(buf), ""WHITE"You have been issued a "RED"WARNING from admin %s(%i).\n\n"TOMATO"Reason:\n"WHITE"%s\n"TOMATO"Warnings count:\n"WHITE"%i/%i", ReturnPlayerName(playerid), playerid, reason, pStats[targetid][userWarnings], MAX_WARNINGS);
  	ShowPlayerDialog(targetid, 0, DIALOG_STYLE_MSGBOX, "Warned by an admin:", buf, "Close", "");
	return 1;
}

CMD:resetwarns(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

    new targetid;
    if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /resetwarns [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	pStats[targetid][userWarnings] = 0;

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has remove your warning log.", ReturnPlayerName(playerid), playerid);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have removed %s(%i)'s warning log.", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:flip(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

    new targetid;
    if (! sscanf(params, "u", targetid))
    {
		if (! IsPlayerConnected(targetid))
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
		}

		if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
		}

		if (! IsPlayerInAnyVehicle(targetid))
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "Player is not in a vehicle.");
		}

		new vehicelid = GetPlayerVehicleID(targetid);
		new Float:angle;
		GetVehicleZAngle(vehicelid, angle);
		SetVehicleZAngle(vehicelid, angle);

		GameTextForPlayer(targetid, "~b~Vehicle fliped", 5000, 3);
		PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
		PlayerPlaySound(targetid, 1133, 0.0, 0.0, 0.0);

		new buf[150];
		format(buf, sizeof(buf), "You have fliped %s(%i)'s vehicle.", ReturnPlayerName(targetid), targetid);
		SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
		format(buf, sizeof(buf), "Admin %s(%i) has flipped your vehicle.", ReturnPlayerName(playerid), playerid);
		SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
    }
    else
    {
		if (! IsPlayerInAnyVehicle(playerid))
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "You must be in a vehicle to use this command.");
		}

		new vehicelid = GetPlayerVehicleID(targetid);
		new Float:angle;
		GetVehicleZAngle(vehicelid, angle);
		SetVehicleZAngle(vehicelid, angle);

		GameTextForPlayer(playerid, "~b~Vehicle fliped", 5000, 3);
		PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);

		SendClientMessage(playerid, COLOR_DODGER_BLUE, "You have fliped your vehicle.");
		SendClientMessage(playerid, COLOR_KHAKI, "TIP: You can flip other player's vehicle by /flip [player].");
    }
	return 1;
}

CMD:ip(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

	new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /ip [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	new buf[150];
	format(buf, sizeof(buf), "%s(%i)'s IP: %s", ReturnPlayerName(targetid), targetid, ReturnPlayerIp(playerid));
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:spawn(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

    new targetid;
    if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /spawn [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    TogglePlayerSpectating(targetid, false);
	}
	SpawnPlayer(targetid);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has respawned you.", ReturnPlayerName(playerid), playerid);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have respawned %s(%i).", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:goto(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

	new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /goto [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You can't teleport to yourself.");
	}

	if (GetPlayerState(targetid) == PLAYER_STATE_WASTED || GetPlayerState(targetid) == PLAYER_STATE_SPECTATING)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player isn't spawned yet.");
	}

	SetPlayerInterior(playerid, GetPlayerInterior(targetid));
	SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(targetid));

	new Float:x, Float:y, Float:z;
	GetPlayerPos(targetid, x, y, z);

	if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
	    new vehicleid = GetPlayerVehicleID(playerid);
		SetVehiclePos(vehicleid, x, y + 2.5, z);
		LinkVehicleToInterior(vehicleid, GetPlayerInterior(targetid));
		SetVehicleVirtualWorld(vehicleid, GetPlayerVirtualWorld(targetid));
	}
	else
	{
		SetPlayerPos(playerid, x, y + 2.0, z);
	}

	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "You have teleported to %s(%i)'s position.", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:setweather(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

	new targetid, id;
	if (sscanf(params, "ui", targetid, id))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setweather [player] [weatherid]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	SetPlayerWeather(targetid, id);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has changed your weather to %i.", ReturnPlayerName(playerid), playerid, id);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have cahnged %s(%i)'s weather to %i.", ReturnPlayerName(targetid), targetid, id);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:settime(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

	new targetid, id;
	if (sscanf(params, "ui", targetid, id))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /settime [player] [timeid]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	SetPlayerTime(targetid, id, 0);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has changed your time to %i.", ReturnPlayerName(playerid), playerid, id);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have cahnged %s(%i)'s time to %i.", ReturnPlayerName(targetid), targetid, id);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:kick(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

    new targetid, reason[45];
	if (sscanf(params, "us[128]", targetid, reason))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /kick [player] [reason]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You can't kick yourself.");
	}

	new buf[150];
	format(buf, sizeof(buf), "%s(%i) has been kicked by admin %s(%i) [Reason: %s]", ReturnPlayerName(targetid), targetid, ReturnPlayerName(playerid), playerid, reason);
	SendClientMessageToAll(COLOR_RED, buf);
	Kick(targetid);
	return 1;
}

CMD:asay(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 1)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 1+ to use this command.");
	}

	new message[135];
	if (sscanf(params, "s[135]", message))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /asay [message]");
	}

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i): %s", ReturnPlayerName(playerid), playerid, message);
    SendClientMessageToAll(COLOR_HOT_PINK, buf);
	return 1;
}

//Admin level 2+
CMD:ann(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new message[35];
	if (sscanf(params, "s[35]", message))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /ann [message]");
	}

	GameTextForAll(message, 5000, 3);
	return 1;
}

CMD:ann2(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new style, expiretime, message[35];
	if (sscanf(params, "iis[35]", style, expiretime, message))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /ann2 [style] [expiretime] [message]");
	}

	GameTextForAll(message, expiretime, style);
	return 1;
}

CMD:jetpack(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid;
	if (sscanf(params, "u", targetid) || ! sscanf(params, "u", targetid) && playerid == targetid)
	{
	    SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USEJETPACK);
		SendClientMessage(playerid, COLOR_DODGER_BLUE, "You have spawned a jetpack.");
		SendClientMessage(playerid, COLOR_KHAKI, "TIP: You can also give jetpack to other players by /jetpack [player].");
		PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
		return 1;
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	SetPlayerSpecialAction(targetid, SPECIAL_ACTION_USEJETPACK);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has given you a jetpack.", ReturnPlayerName(playerid), playerid);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have given %s(%i) a jetpack.", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:aka(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /aka [player]");
	}

	new SQLRow:keys[1], values[1];
	yoursql_sort_int(SQL:0, "users/ROW_ID", keys, values, .limit = 1);
	new ip[18];
	new aka_count;
	new aka[MAX_PLAYER_NAME * 5];
	for (new i; i <= _:keys[0]; i++)
	{
	    if (yoursql_get_field(SQL:0, "users/ip", SQLRow:i, ip, 18))
	    {
	        if (ipmatch(ip, ReturnPlayerIp(playerid)))
	        {
	            yoursql_get_field(SQL:0, "users/name", SQLRow:i, aka[aka_count], MAX_PLAYER_NAME);
	            aka_count++;

	            if (aka_count >= 5)
	            {
	                break;
	            }
	        }
	    }
	}

	if (aka_count == 1)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The user doesn't have any other account from the same ip.");
	}

	new buf[150];
	format(buf, sizeof(buf), "Search result for %s's AKA: [ip: %s]", ReturnPlayerName(targetid), ReturnPlayerIp(targetid));
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	for (new i = 0, j = aka_count; i < j; i++)
	{
	    strcat(buf, aka[i]);
	    if (j == aka_count - 1)
		{
			strcat(buf, ".");
		}
		else
		{
			strcat(buf, ", ");
		}
	}
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:aweaps(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

    GivePlayerWeapon(playerid, 9, 1);//chainsaw
    GivePlayerWeapon(playerid, 32, 999999);//tec-9
    GivePlayerWeapon(playerid, 16, 999999);//grenades
    GivePlayerWeapon(playerid, 24, 999999);//deagle
    GivePlayerWeapon(playerid, 26, 999999);//sawn off
    GivePlayerWeapon(playerid, 29, 999999);//mp5
    GivePlayerWeapon(playerid, 31, 999999);//m4
    GivePlayerWeapon(playerid, 34, 999999);//sniper
    GivePlayerWeapon(playerid, 38, 999999);//minigun

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	GameTextForPlayer(playerid, "~b~Admin weapons!", 5000, 3);
    return 1;
}

CMD:text(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid, message[35];
	if (sscanf(params, "us[35]", targetid, message))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /text [player] [message]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You can't text message to yourself.");
	}

	GameTextForPlayer(targetid, message, 5000, 3);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has sent you a screen message.", ReturnPlayerName(playerid), playerid);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have sent %s(%i) a scren message.", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:carhealth(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid, Float:amount;
	if (sscanf(params, "uf", targetid, amount))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /carhealth [player] [amount]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (! IsPlayerInAnyVehicle(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not in any vehicle.");
	}

	SetVehicleHealth(GetPlayerVehicleID(targetid), amount);
	PlayerPlaySound(targetid, 1133, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set your car's health to %0.2f.", ReturnPlayerName(playerid), playerid, amount);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have set %s(%i)'s car health to %.2f.", ReturnPlayerName(targetid), targetid, gVehicleModelNames[GetVehicleModel(GetPlayerVehicleID(targetid)) - 400], amount);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:eject(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid;
    if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /eject [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (! IsPlayerInAnyVehicle(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not in any vehicle.");
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(targetid, x, y, z);
	SetPlayerPos(targetid, x, y, z + 1.0);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has ejected you from your vehicle.", ReturnPlayerName(playerid), playerid);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have ejected %s(%i) from his vehicle.", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:carpaint(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid, paint;
	if (sscanf(params, "ui", targetid, paint))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /carpaint [player] [paintjob]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (! IsPlayerInAnyVehicle(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not in any vehicle.");
	}

	if (paint < 0 || paint > 3)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid paintjob id, must be b/w 0-3.");
	}

	ChangeVehiclePaintjob(GetPlayerVehicleID(targetid), paint);
	PlayerPlaySound(targetid, 1133, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set your vehicle's paintjob id to %i.", ReturnPlayerName(playerid), playerid, paint);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have set %s(%i)'s vehicle's paintjob id to %i.", ReturnPlayerName(targetid), targetid, paint);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:carcolor(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid, color1, color2;
	if (sscanf(params, "uiI(-1)", targetid, color1, color2))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /carcolor [player] [color1] [*color2]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (! IsPlayerInAnyVehicle(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not in any vehicle.");
	}

	ChangeVehicleColor(GetPlayerVehicleID(targetid), color1, color2);
	PlayerPlaySound(targetid, 1133, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);


	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set your vehicle's color to %i & %i.", ReturnPlayerName(playerid), playerid, color1, color2);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have set %s(%i)'s vehicle's paintjob to %i & %i.", ReturnPlayerName(targetid), targetid, color1, color2);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:givecar(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

    new targetid, vehicle[32], color1, color2;
	if (sscanf(params, "us[32]I(-1)I(-1)", targetid, vehicle, color1, color2))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /givecar [player] [vehicle] [*color1] [*color2]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	new model;
	if (isnumeric(vehicle))
	{
		model = strval(vehicle);
	}
	else
	{
		model = GetVehicleModelIDFromName(vehicle);
	}

	if (model < 400 || model > 611)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid vehicle model id/name.");
	}

	if (IsValidVehicle(pStats[playerid][userVehicle]))
	{
		DestroyVehicle(pStats[playerid][userVehicle]);
	}

	new Float:x, Float:y, Float:z, Float:a;
	GetPlayerPos(targetid, x, y, z);
    GetPlayerFacingAngle(targetid, a);

	if (IsPlayerInAnyVehicle(targetid))
	{
		SetPlayerPos(playerid, x, y, z + 1.0);
	}

	pStats[targetid][userVehicle] = CreateVehicle(model, x, y + 2.5, z, a, color1, color2, -1);
    SetVehicleVirtualWorld(pStats[targetid][userVehicle], GetPlayerVirtualWorld(playerid));
    LinkVehicleToInterior(pStats[targetid][userVehicle], GetPlayerInterior(playerid));
    PutPlayerInVehicle(playerid, pStats[targetid][userVehicle], 0);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has given you vehicle %s(model: %i | color1: %i | color2: %i).", ReturnPlayerName(playerid), playerid, gVehicleModelNames[model - 400], model, color1, color2);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have given %s(%i) vehicle %s(model: %i | color1: %i | color2: %i).", ReturnPlayerName(targetid), targetid, gVehicleModelNames[model - 400], model, color1, color2);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:car(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

    new vehicle[32], color1, color2;
	if (sscanf(params, "s[32]I(-1)I(-1)", vehicle, color1, color2))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /car [vehicle] [*color1] [*color2]");
	}

	new model;
	if (isnumeric(vehicle))
	{
		model = strval(vehicle);
	}
	else
	{
		model = GetVehicleModelIDFromName(vehicle);
	}

	if (model < 400 || model > 611)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid vehicle model id/name.");
	}

	if (IsValidVehicle(pStats[playerid][userVehicle]))
	{
		DestroyVehicle(pStats[playerid][userVehicle]);
	}

	new Float:x, Float:y, Float:z, Float:a;
	GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);

	if (IsPlayerInAnyVehicle(playerid))
	{
		SetPlayerPos(playerid, x, y, z + 1.0);
	}

	pStats[playerid][userVehicle] = CreateVehicle(model, x, y + 2.5, z, a, color1, color2, -1);
    SetVehicleVirtualWorld(pStats[playerid][userVehicle], GetPlayerVirtualWorld(playerid));
    LinkVehicleToInterior(pStats[playerid][userVehicle], GetPlayerInterior(playerid));
    PutPlayerInVehicle(playerid, pStats[playerid][userVehicle], 0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "You have spawned a vehicle %s(model: %i | color1: %i | color2: %i).", gVehicleModelNames[model - 400], model, color1, color2);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:akill(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid, reason[128];
    if (sscanf(params, "uS(No reason specified)[128]", targetid, reason))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /akill [player] [*reason]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

    SetPlayerHealth(targetid, 0.0);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "%s(%i) was killed by admin %s(%i) [Reason: %s]", ReturnPlayerName(targetid), targetid, ReturnPlayerName(playerid), playerid, reason);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:jailed(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new info[MAX_PLAYER_NAME * 100], buf[100];

	foreach (new i : Player)
	{
	    if (pStats[i][userJailTime] != -1)
	    {
	    	format(buf, sizeof(buf), "%s(%i) - %i seconds remaining for unjail", i, ReturnPlayerName(i), i, pStats[i][userJailTime]);
	        strcat(info, buf);
	    }
	}

	if (! info[0])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "No players are currently jailed.");
	}
	else
	{
	    ShowPlayerDialog(playerid, DIALOG_ID_JAILED_LIST, DIALOG_STYLE_LIST, "Jailed players:", info, "Close", "");
	}
	return 1;
}

CMD:jail(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid, time, reason[128];
	if (sscanf(params, "uI(60)S(No reason specified)[128]", targetid, time, reason))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /jail [player] [*seconds] [*reason]");
	}

	if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot jail yourself.");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		 return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (time > 5 * 60 || time < 10)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The jail time must be b/w 10 - 360(5 minutes) seconds.");
	}

	if (GetPlayerState(targetid) == PLAYER_STATE_WASTED || GetPlayerState(targetid) == PLAYER_STATE_SPECTATING)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player isn't spawned yet.");
	}

	if (pStats[targetid][userJailTime] != -1)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Player is already in jail.");
	}

    pStats[targetid][userJailTime] = time;

    SetPlayerInterior(targetid, 3);
	SetPlayerPos(targetid, 197.6661, 173.8179, 1003.0234);
	SetCameraBehindPlayer(targetid);

	new string[144];
	format(string, sizeof(string), "You are in jail for %i seconds.", time);
    SendClientMessage(playerid, COLOR_DODGER_BLUE, string);

	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "%s(%i) has been jailed by admin %s(%i) for %i seconds [Reason: %s]", ReturnPlayerName(targetid), targetid, ReturnPlayerName(playerid), playerid, time, reason);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "~r~Jailed for ~w~%i ~r~seconds", time);
	GameTextForPlayer(targetid, buf, 5000, 3);
	return 1;
}

CMD:unjail(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /unjail [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (pStats[targetid][userJailTime] == -1)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Player is not in jail.");
	}

	pStats[targetid][userJailTime] = -1;
	SpawnPlayer(targetid);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "%s(%i) has been unjailed by admin %s(%i).", ReturnPlayerName(targetid), targetid, ReturnPlayerName(playerid), playerid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	GameTextForPlayer(targetid, "~g~Unjailed!", 5000, 3);
	return 1;
}

CMD:muted(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new info[MAX_PLAYER_NAME * 100], buf[100];

	foreach (new i : Player)
	{
	    if (pStats[i][userMuteTime] != -1)
	    {
	    	format(buf, sizeof(buf), "%s(%i) - %i seconds remaining for unmute", i, ReturnPlayerName(i), i, pStats[i][userMuteTime]);
	        strcat(info, buf);
	    }
	}

	if (! info[0])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "No players are currently mute.");
	}
	else
	{
	    ShowPlayerDialog(playerid, DIALOG_ID_MUTE_LIST, DIALOG_STYLE_LIST, "Mute players:", info, "Close", "");
	}
	return 1;
}

CMD:mute(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid, time, reason[128];
	if (sscanf(params, "uI(60)S(No reason specified)[128]", targetid, time, reason))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /mute [player] [*seconds] [*reason]");
	}

	if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot jail yourself.");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		 return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (time > 5 * 60 || time < 10)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The mute time must be b/w 10 - 360(5 minutes) seconds.");
	}

	if (pStats[targetid][userMuteTime] != -1)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Player is already muted.");
	}

	pStats[targetid][userMuteTime] = time;
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "%s(%i) has been muted by admin %s(%i) for %i seconds [Reason: %s]", ReturnPlayerName(targetid), targetid, ReturnPlayerName(playerid), playerid, time, reason);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "~r~Muted for ~w~%i ~r~seconds", time);
	GameTextForPlayer(targetid, buf, 5000, 3);
	return 1;
}

CMD:unmute(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /unmute [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (pStats[targetid][userMuteTime] == -1)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Player is already muted.");
	}

	pStats[targetid][userMuteTime] = -1;
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "%s(%i) has been unmuted by admin %s(%i).", ReturnPlayerName(targetid), targetid, ReturnPlayerName(playerid), playerid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	GameTextForPlayer(targetid, "~g~Unmuted!", 5000, 3);
	return 1;
}

/*CMD:atele(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	ShowPlayerDialog(playerid, DIALOG_ID_TELEPORTS, DIALOG_STYLE_LIST, "Select City", "Los Santos\nSan Fierro\nLas Venturas", "Select", "Close");
	return 1;
}*/

CMD:setskin(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid, skin;
	if (sscanf(params, "ui", targetid, skin))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setskin [player] [skinid]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (GetPlayerState(targetid) == PLAYER_STATE_WASTED || GetPlayerState(targetid) == PLAYER_STATE_SPECTATING)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player isn't spawned yet.");
	}

	if (skin < 0 || skin == 74 || skin > 311)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid skin id, must be b/w 0 - 311 (except 74).");
	}

    SetPlayerSkin(targetid, skin);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set your skin id to %i.", ReturnPlayerName(playerid), playerid, skin);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have set %s(%i)'s skin id to %i.", ReturnPlayerName(targetid), targetid, skin);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:cc(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	for (new i; i < 250; i++)
	{
		SendClientMessageToAll(-1, " ");
	}
	foreach (new i : Player)
	{
		PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	}

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has cleared the chat.", ReturnPlayerName(playerid), playerid);
    SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:aheal(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid;
    if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /heal [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

    SetPlayerHealth(targetid, 100.0);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has healed you.", ReturnPlayerName(playerid), playerid);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have healed %s(%i).", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:aarmour(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid;
    if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /armour [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

    SetPlayerArmour(targetid, 100.0);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has armoured you.", ReturnPlayerName(playerid), playerid);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have armoured %s(%i).", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:setinterior(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid, id;
	if (sscanf(params, "ui", targetid, id))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setinterior [player] [interior]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	SetPlayerInterior(targetid, id);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set your interior id to %i.", ReturnPlayerName(playerid), playerid, id);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have set %s(%i)'s interior id to %i.", ReturnPlayerName(targetid), targetid, id);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:setworld(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid, id;
	if (sscanf(params, "ui", targetid, id))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setworld [player] [worldid]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	SetPlayerVirtualWorld(targetid, id);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set your virtual world id to %i.", ReturnPlayerName(playerid), playerid, id);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have set %s(%i)'s virtual world id to %i.", ReturnPlayerName(targetid), targetid, id);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:slap(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

    new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /slap [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(targetid, x, y, z);
	SetPlayerPos(targetid, x, y, z + 5.0);

    PlayerPlaySound(playerid, 1190, 0.0, 0.0, 0.0);
    PlayerPlaySound(targetid, 1190, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "You have slapped %s(%i).", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:explode(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /explode [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(targetid, x, y, z);
	CreateExplosion(x, y, z, 7, 1.00);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "You have made an explosion on %s(%i)'s position.", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:disarm(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /disarm [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	ResetPlayerWeapons(targetid);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has disarmed you.", ReturnPlayerName(playerid), playerid);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have disarmed %s(%i).", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:ban(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new targetid, reason[35], days;
	if (sscanf(params, "is[35]I(0)", targetid, reason, days))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /ban [player] [reason] [*days (default 0 permanent)]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

    if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You can't ban yourself.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (days < 0)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid days, must be greater than 0 for temp ban, or 0 for permanent ban.");
	}

	if (strlen(reason) < 3 || strlen(reason) > 35)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid reason length, must be b/w 0-35 characters.");
	}

	new bandate[18], date[3], time;
	getdate(date[0], date[1], date[2]);

	new month[15];
	switch (date[1])
	{
	    case 1: month = "January";
	    case 2: month = "Feburary";
	    case 3: month = "March";
	    case 4: month = "April";
	    case 5: month = "May";
	    case 6: month = "June";
	    case 7: month = "July";
	    case 8: month = "August";
	    case 9: month = "September";
	    case 10: month = "October";
	    case 11: month = "November";
	    case 12: month = "December";
	}

	format(bandate, sizeof(bandate), "%02i %s, %i", date[2], month, date[0]);

	if (days == 0)
	{
		time = 0;
	}
	else
	{
		time = ((days * 24 * 60 * 60) + gettime());
	}

	yoursql_multiset_row(SQL:0, "bans", "sssssii", "name", ReturnPlayerName(targetid), "ip", ReturnPlayerIp(targetid), "admin_name", ReturnPlayerName(playerid), "reason", reason, "date", bandate, "type", (! days) ? (1) : (0), "expire", time);

	new buf[150];
	if (! days)
	{
	    format(buf, sizeof(buf), "%s(%i) has been banned by admin %s(%i) [PERMANENT].", ReturnPlayerName(targetid), targetid, ReturnPlayerName(playerid), playerid);
		SendClientMessageToAll(COLOR_RED, buf);
	    format(buf, sizeof(buf), "Reason: %s", reason);
		SendClientMessageToAll(COLOR_RED, buf);
	}
	else
	{
	    format(buf, sizeof(buf), "%s(%i) has been banned by admin %s(%i) for %i days [TEMPERORARY].", ReturnPlayerName(targetid), targetid, ReturnPlayerName(playerid), playerid, days);
		SendClientMessageToAll(COLOR_RED, buf);
	    format(buf, sizeof(buf), "Reason: %s", reason);
		SendClientMessageToAll(COLOR_RED, buf);
	}

 	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	Kick(targetid);
	return 1;
}

CMD:ipban(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new ip[18], reason[35], days;
	if (sscanf(params, "s[18]s[35]I(0)", ip, reason, days))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /ipban [ip] [reason] [*days (default 0 permanent)]");
	}

	if (! IsValidIp(ip))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid Ip. specified.");
	}

    if (! strcmp(ip, ReturnPlayerIp(playerid)))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You can't ban yourself.");
	}

	if (pStats[playerid][userAdmin] < yoursql_get_field_int(SQL:0, "users/admin", yoursql_get_row(SQL:0, "users", "ip = %s", ip)))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (days < 0)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid days, must be greater than 0 for temp ban, or 0 for permanent ban.");
	}

	if (strlen(reason) < 3 || strlen(reason) > 35)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid reason length, must be b/w 0-35 characters.");
	}

	new bandate[18], date[3], time;
	getdate(date[0], date[1], date[2]);

	new month[15];
	switch (date[1])
	{
	    case 1: month = "January";
	    case 2: month = "Feburary";
	    case 3: month = "March";
	    case 4: month = "April";
	    case 5: month = "May";
	    case 6: month = "June";
	    case 7: month = "July";
	    case 8: month = "August";
	    case 9: month = "September";
	    case 10: month = "October";
	    case 11: month = "November";
	    case 12: month = "December";
	}

	format(bandate, sizeof(bandate), "%02i %s, %i", date[2], month, date[0]);

	if (days == 0)
	{
		time = 0;
	}
	else
	{
		time = ((days * 24 * 60 * 60) + gettime());
	}

	new name[MAX_PLAYER_NAME];
	yoursql_get_field(SQL:0, "users/name", yoursql_get_row(SQL:0, "users", "ip = %s", ip), name, MAX_PLAYER_NAME);
	yoursql_multiset_row(SQL:0, "bans", "sssssii", "name", name, "ip", ip, "admin_name", ReturnPlayerName(playerid), "reason", reason, "date", bandate, "type", (! days) ? (1) : (0), "expire", time);

	new id = -1;
	foreach (new i : Player)
	{
	    if (! strcmp(ip, ReturnPlayerIp(i)))
	    {
	        id = i;
	        break;
	    }
	}

	new buf[150];
	if (! days)
	{
	    format(buf, sizeof(buf), "%s(%i) has been banned by admin %s(%i) [PERMANENT].", name, id, ReturnPlayerName(playerid), playerid);
		SendClientMessageToAll(COLOR_RED, buf);
	    format(buf, sizeof(buf), "Reason: %s", reason);
		SendClientMessageToAll(COLOR_RED, buf);
	}
	else
	{
	    format(buf, sizeof(buf), "%s(%i) has been banned by admin %s(%i) for %i days [TEMPERORARY].", name, id, ReturnPlayerName(playerid), playerid, days);
		SendClientMessageToAll(COLOR_RED, buf);
	    format(buf, sizeof(buf), "Reason: %s", reason);
		SendClientMessageToAll(COLOR_RED, buf);
	}

 	PlayerPlaySound(id, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	Kick(id);
	return 1;
}

CMD:oban(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new name[MAX_PLAYER_NAME], reason[35], days;
	if (sscanf(params, "s[24]s[35]I(0)", name, reason, days))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /oban [name] [reason] [*days (default 0 permanent)]");
	}

	if (yoursql_get_row(SQL:0, "users", "name = %s", name) == SQL_INVALID_ROW)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified username isn't registered.");
	}

    if (! strcmp(name, ReturnPlayerName(playerid)))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You can't ban yourself.");
	}

	if (pStats[playerid][userAdmin] < yoursql_get_field_int(SQL:0, "users/admin", yoursql_get_row(SQL:0, "users", "name = %s", name)))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (days < 0)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid days, must be greater than 0 for temp ban, or 0 for permanent ban.");
	}

	if (strlen(reason) < 3 || strlen(reason) > 35)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid reason length, must be b/w 0-35 characters.");
	}

	new bandate[18], date[3], time;
	getdate(date[0], date[1], date[2]);

	new month[15];
	switch (date[1])
	{
	    case 1: month = "January";
	    case 2: month = "Feburary";
	    case 3: month = "March";
	    case 4: month = "April";
	    case 5: month = "May";
	    case 6: month = "June";
	    case 7: month = "July";
	    case 8: month = "August";
	    case 9: month = "September";
	    case 10: month = "October";
	    case 11: month = "November";
	    case 12: month = "December";
	}

	format(bandate, sizeof(bandate), "%02i %s, %i", date[2], month, date[0]);

	if (days == 0)
	{
		time = 0;
	}
	else
	{
		time = ((days * 24 * 60 * 60) + gettime());
	}

	new ip[18];
	yoursql_get_field(SQL:0, "users/ip", yoursql_get_row(SQL:0, "users", "name = %s", name), ip);
	yoursql_multiset_row(SQL:0, "bans", "sssssii", "name", name, "ip", ip, "admin_name", ReturnPlayerName(playerid), "reason", reason, "date", bandate, "type", (! days) ? (1) : (0), "expire", time);

	new buf[150];
	if (! days)
	{
	    format(buf, sizeof(buf), "%s has been offline banned by admin %s(%i) [PERMANENT].", name, ReturnPlayerName(playerid), playerid);
		SendClientMessageToAll(COLOR_RED, buf);
	    format(buf, sizeof(buf), "Reason: %s", reason);
		SendClientMessageToAll(COLOR_RED, buf);
	}
	else
	{
	    format(buf, sizeof(buf), "%s(%i) has been offline banned by admin %s(%i) for %i days [TEMPERORARY].", name, ReturnPlayerName(playerid), playerid, days);
		SendClientMessageToAll(COLOR_RED, buf);
	    format(buf, sizeof(buf), "Reason: %s", reason);
		SendClientMessageToAll(COLOR_RED, buf);
	}

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}

CMD:searchban(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new search[MAX_PLAYER_NAME];
	if (sscanf(params,"s[24]", search))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /searchban [name/ip]");
	}

	new SQLRow:rowid;
	if (IsValidIp(search))
	{
	    rowid = yoursql_get_row(SQL:0, "bans", "ip = %s", search);
		if (rowid == SQL_INVALID_ROW)
		{
		    return SendClientMessage(playerid, COLOR_TOMATO, "The specified ip isn't banned.");
		}
	}
	else
	{
	    rowid = yoursql_get_row(SQL:0, "bans", "name = %s", search);
		if (rowid == SQL_INVALID_ROW)
		{
		    return SendClientMessage(playerid, COLOR_TOMATO, "The specified name isn't banned.");
		}
	}

	new buf[1000];
	strcat(buf, WHITE);

	strcat(buf, "You have been banned from the server.\n");
	strcat(buf, "If this was a mistake (from server/admin side), please report a BAN APPEAL on our forums.\n\n");

	if (IsValidIp(search))
	{
		strcat(buf, "Username: "PINK"");
	    new name[MAX_PLAYER_NAME];
		yoursql_get_field(SQL:0, "bans/name", rowid, name, MAX_PLAYER_NAME);
		strcat(buf, name);
		strcat(buf, "\n"WHITE"");

		strcat(buf, "Ip: "PINK"");
		strcat(buf, search);
		strcat(buf, "\n"WHITE"");
 	}
 	else
 	{
		strcat(buf, "Username: "PINK"");
 	    strcat(buf, search);
		strcat(buf, "\n"WHITE"");

		strcat(buf, "Ip: "PINK"");
	    new ip[18];
		yoursql_get_field(SQL:0, "bans/ip", rowid, ip);
		strcat(buf, ip);
		strcat(buf, "\n"WHITE"");
 	}

 	new value[100];

	strcat(buf, "Ban date: "PINK"");
	yoursql_get_field(SQL:0, "bans/date", rowid, value);
	strcat(buf, value);
	strcat(buf, "\n"WHITE"");

	strcat(buf, "Admin name: "PINK"");
	yoursql_get_field(SQL:0, "bans/admin_name", rowid, value);
	strcat(buf, value);
	strcat(buf, "\n"WHITE"");

	switch (yoursql_get_field_int(SQL:0, "bans/type", rowid))
	{
		case 0:
		{
			strcat(buf, "Ban type: "PINK"");
			strcat(buf, "PERMANENT");
			strcat(buf, "\n"WHITE"");
		}
		case 1:
		{
			strcat(buf, "Ban type: "PINK"");
			strcat(buf, "TEMPORARY (expire on: ");
			new year, month, day, hour, minute, second;
			TimestampToDate(yoursql_get_field_int(SQL:0, "bans/expire", rowid), year, month, day, hour, minute, second, 0);
			new month_name[15];
			switch (month)
			{
			    case 1: month_name = "January";
			    case 2: month_name = "Feburary";
			    case 3: month_name = "March";
			    case 4: month_name = "April";
			    case 5: month_name = "May";
			    case 6: month_name = "June";
			    case 7: month_name = "July";
			    case 8: month_name = "August";
			    case 9: month_name = "September";
			    case 10: month_name = "October";
			    case 11: month_name = "November";
			    case 12: month_name = "December";
			}
			format(buf, sizeof(buf), "%s%i %s, %i)", buf, day, month_name, year);
			strcat(buf, "\n"WHITE"");
		}
		case 2:
		{
			strcat(buf, "Ban type: "PINK"");
			strcat(buf, "RANGEBAN");
			strcat(buf, "\n"WHITE"");
		}
		case 3:
		{
			strcat(buf, "Ban type: "PINK"");
			strcat(buf, "TEMPORARY RANGEBAN (expire on: ");
			new year, month, day, hour, minute, second;
			TimestampToDate(yoursql_get_field_int(SQL:0, "bans/expire", rowid), year, month, day, hour, minute, second, 0);
			new month_name[15];
			switch (month)
			{
			    case 1: month_name = "January";
			    case 2: month_name = "Feburary";
			    case 3: month_name = "March";
			    case 4: month_name = "April";
			    case 5: month_name = "May";
			    case 6: month_name = "June";
			    case 7: month_name = "July";
			    case 8: month_name = "August";
			    case 9: month_name = "September";
			    case 10: month_name = "October";
			    case 11: month_name = "November";
			    case 12: month_name = "December";
			}
			format(buf, sizeof(buf), "%s%i %s, %i)", buf, day, month_name, year);
			strcat(buf, "\n"WHITE"");
		}
	}

	strcat(buf, "Reason: "RED"");
	yoursql_get_field(SQL:0, "bans/reason", rowid, value);
	strcat(buf, value);
	strcat(buf, "\n\n"WHITE"");

	strcat(buf, "Take a screenshot of this as a refrence for admins.");

	ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Ban search result:", buf, "Close", "");

	return 1;
}

CMD:unban(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 2+ to use this command.");
	}

	new search[MAX_PLAYER_NAME];
	if (sscanf(params,"s[24]", search))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /unban [name/ip]");
	}

	new SQLRow:rowid;
	if (IsValidIp(search))
	{
	    rowid = yoursql_get_row(SQL:0, "bans", "ip = %s", search);
		if (rowid == SQL_INVALID_ROW)
		{
		    return SendClientMessage(playerid, COLOR_TOMATO, "The specified ip isn't banned.");
		}
	}
	else
	{
	    rowid = yoursql_get_row(SQL:0, "bans", "name = %s", search);
		if (rowid == SQL_INVALID_ROW)
		{
		    return SendClientMessage(playerid, COLOR_TOMATO, "The specified name isn't banned.");
		}
	}

	yoursql_delete_row(SQL:0, "bans", rowid);

	new buf[150];
	if (IsValidIp(search))
	{
		format(buf, sizeof(buf), "You have unbanned ip %s successfully.", search);
		SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);

		PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	}
	else
	{
		format(buf, sizeof(buf), "Admin %s(%i) have unbanned user %s.", ReturnPlayerName(playerid), playerid, search);
		SendClientMessageToAll(COLOR_DODGER_BLUE, buf);

		foreach (new i : Player)
		{
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}

	return 1;
}

//Admin level 3+
CMD:get(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /get [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot get yourself.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	if (GetPlayerState(targetid) == PLAYER_STATE_DRIVER)
	{
	    new vehicleid = GetPlayerVehicleID(targetid);
		SetVehiclePos(vehicleid, x, y + 2.5, z);
		LinkVehicleToInterior(vehicleid, GetPlayerInterior(playerid));
		SetVehicleVirtualWorld(vehicleid, GetPlayerVirtualWorld(playerid));
	}
	else
	{
		SetPlayerPos(targetid, x, y + 2.0, z);
	}
	SetPlayerInterior(targetid, GetPlayerInterior(playerid));
	SetPlayerVirtualWorld(targetid, GetPlayerVirtualWorld(playerid));
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has teleported you to his/her position.", ReturnPlayerName(playerid), playerid);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have teleport %s(%i) to your position.", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:write(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new text[144], color;
	if (sscanf(params, "s[144]I(1)", text, color))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /write [text] [*color]");
		SendClientMessage(playerid, COLOR_THISTLE, "COLOR: [0]Black, [1]White, [2]Red, [3]Orange, [4]Yellow, [5]Green, [6]Blue, [7]Purple, [8]Brown, [9]Pink");
		return 1;
	}

	if (color > 9 || color > 0)
	{
		SendClientMessage(playerid, COLOR_TOMATO, "Invalid color id, must be b/w 0-9.");
		SendClientMessage(playerid, COLOR_THISTLE, "COLOR: [0]Black, [1]White, [2]Red, [3]Orange, [4]Yellow, [5]Green, [6]Blue, [7]Purple, [8]Brown, [9]Pink");
		return 1;
	}

	switch(color)
	{
	    case 0: color = COLOR_BLACK;
	    case 1: color = COLOR_WHITE;
	    case 2: color = COLOR_RED;
	    case 3: color = COLOR_ORANGE;
	    case 4: color = COLOR_YELLOW;
	    case 5: color = COLOR_GREEN;
	    case 6: color = COLOR_BLUE;
	    case 7: color = COLOR_PURPLE;
	    case 8: color = COLOR_BROWN;
	    case 9: color = COLOR_PINK;
	}
	SendClientMessageToAll(color, text);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}

CMD:force(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /force [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	ForceClassSelection(targetid);
	SetPlayerHealth(targetid, 0.0);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has forced you to class selection.", ReturnPlayerName(playerid), playerid);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have forced %s(%i) to class selection.", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:healall(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	foreach (new i : Player)
	{
		SetPlayerHealth(i, 100.0);
		PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	}

    new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has healed all players.", ReturnPlayerName(playerid), playerid);
    SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:armourall(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	foreach (new i : Player)
	{
		SetPlayerArmour(i, 100.0);
		PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	}

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has armoured all players.", ReturnPlayerName(playerid), playerid);
    SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:fightstyle(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid, style;
    if (sscanf(params, "ui", targetid, style))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /fightstyle [player] [style]");
		SendClientMessage(playerid, COLOR_THISTLE, "STYLES: [0]Normal, [1]Boxing, [2]Kungfu, [3]Kneehead, [4]Grabkick, [5]Elbow");
		return 1;
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (style > 5 || style < 0)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Inavlid fighting style, must be b/w 0-5.");
	}
	new stylename[15];
	switch(style)
	{
	    case 0:
	    {
	        SetPlayerFightingStyle(targetid, 4);
	        stylename = "Normal";
	    }
	    case 1:
	    {
	        SetPlayerFightingStyle(targetid, 5);
	        stylename = "Boxing";
	    }
	    case 2:
	    {
	        SetPlayerFightingStyle(targetid, 6);
	        stylename = "Kung Fu";
	    }
	    case 3:
	    {
	        SetPlayerFightingStyle(targetid, 7);
	        stylename = "Kneehead";
	    }
	    case 4:
	    {
	        SetPlayerFightingStyle(targetid, 15);
	        stylename = "Grabkick";
	    }
	    case 5:
	    {
	        SetPlayerFightingStyle(targetid, 16);
	        stylename = "Elbow";
	    }
	}
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set your fighting style to (%i)%s.", ReturnPlayerName(playerid), playerid, stylename, style);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have set %s(%i)'s fighting style to (%i)%s.", ReturnPlayerName(targetid), targetid, stylename, style);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:sethealth(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid, Float:amount;
	if (sscanf(params, "uf", targetid, amount))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /sethealth [player] [amount]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	SetPlayerHealth(targetid, amount);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set your health to %0.2f.", ReturnPlayerName(playerid), playerid, amount);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have set %s(%i)'s health to %.2f.", ReturnPlayerName(targetid), targetid, amount);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:setarmour(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid, Float:amount;
	if (sscanf(params, "uf", targetid, amount))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setarmour [player] [amount]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	SetPlayerArmour(targetid, amount);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set your armour to %0.2f.", ReturnPlayerName(playerid), playerid, amount);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have set %s(%i)'s armour to %.2f.", ReturnPlayerName(targetid), targetid, amount);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:god(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	if (! pStats[playerid][userGod])
	{
	    SetPlayerHealth(playerid, FLOAT_INFINITY);
	    GameTextForPlayer(playerid, "~g~Godmode ON", 3000, 3);

	    pStats[playerid][userGod] = true;
	}
	else
	{
	    SetPlayerHealth(playerid, 100.0);
	    GameTextForPlayer(playerid, "~r~Godmode OFF", 3000, 3);

	    pStats[playerid][userGod] = false;
	}
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}

CMD:godcar(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	if (! pStats[playerid][userGodCar])
	{
	    SetVehicleHealth(GetPlayerVehicleID(playerid), FLOAT_INFINITY);
	    GameTextForPlayer(playerid, "~g~Godcarmode ON", 3000, 3);

	    pStats[playerid][userGodCar] = true;
	}
	else
	{
	    SetVehicleHealth(GetPlayerVehicleID(playerid), 1000.0);
	    GameTextForPlayer(playerid, "~r~Godcarmode OFF", 3000, 3);

	    pStats[playerid][userGodCar] = false;
	}
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}

CMD:freeze(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid, reason[35];
	if (sscanf(params, "uS(No reason specified)[35]", targetid, reason))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /freeze [playerid] [*reason]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	TogglePlayerControllable(targetid, false);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has freezed you.", ReturnPlayerName(playerid), playerid);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have freezed %s(%i).", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:unfreeze(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid;
	if (sscanf(params, "u", targetid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /unfreeze [playerid]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	TogglePlayerControllable(targetid, true);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has unfreezed you.", ReturnPlayerName(playerid), playerid);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have unfreezed %s(%i).", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:giveweapon(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid, weapon[32], ammo;
	if (sscanf(params, "us[32]I(250)", targetid, weapon, ammo))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /giveweapon [player] [weapon] [*ammo]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	new weaponid;
	if (isnumeric(weapon))
	{
		weaponid = strval(weapon);
	}
	else
	{
		weaponid = GetWeaponIDFromName(weapon);
	}

	if (1 > weaponid > 46)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid weapon id/name.");
	}

	GetWeaponName(weaponid, weapon, sizeof(weapon));
	GivePlayerWeapon(targetid, weaponid, ammo);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has given you a %s[id: %i] with %i ammo.", ReturnPlayerName(playerid), playerid, weapon, weaponid, ammo);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have given %s(%i) a %s[id: %i] with %i ammo.", ReturnPlayerName(targetid), targetid, weapon, weaponid, ammo);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:setcolor(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid, color;
	if (sscanf(params, "ui", targetid, color))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setcolor [player] [color]");
		SendClientMessage(playerid, COLOR_THISTLE, "COLOR: [0]Black, [1]White, [2]Red, [3]Orange, [4]Yellow, [5]Green, [6]Blue, [7]Purple, [8]Brown, [9]Pink");
		return 1;
	}

	if (color > 9 || color > 0)
	{
		SendClientMessage(playerid, COLOR_TOMATO, "Invalid color id, must be b/w 0-9.");
		SendClientMessage(playerid, COLOR_THISTLE, "COLOR: [0]Black, [1]White, [2]Red, [3]Orange, [4]Yellow, [5]Green, [6]Blue, [7]Purple, [8]Brown, [9]Pink");
		return 1;
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	new colorname[15];
	switch(color)
	{
	    case 0: color = COLOR_BLACK, colorname = "Black";
    	case 1: color = COLOR_WHITE, colorname = "White";
     	case 2: color = COLOR_RED, colorname = "Red";
      	case 3: color = COLOR_ORANGE, colorname = "Orange";
      	case 4: color = COLOR_YELLOW, colorname = "Yellow";
    	case 5: color = COLOR_GREEN, colorname = "Green";
       	case 6: color = COLOR_BLUE, colorname = "Blue";
      	case 7: color = COLOR_PURPLE, colorname = "Purple";
       	case 8: color = COLOR_BROWN, colorname = "Brown";
       	case 9: color = COLOR_PINK, colorname = "Pink";
	}

	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set your color to %s.", ReturnPlayerName(playerid), playerid, colorname);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have set %s[%i]'s color to %s.", ReturnPlayerName(targetid), targetid, colorname);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:setcash(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid, amount;
	if (sscanf(params, "ui", targetid, amount))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setcash [player] [amount]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	ResetPlayerMoney(targetid);
	GivePlayerMoney(targetid, amount);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set your money to $%i.", ReturnPlayerName(playerid), playerid, amount);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have set %s(%i)'s money to $%i.", ReturnPlayerName(targetid), targetid, amount);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:setscore(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid, amount;
	if (sscanf(params, "ui", targetid, amount))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setscore [player] [amount]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	SetPlayerScore(targetid, amount);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set your score to %i.", ReturnPlayerName(playerid), playerid, amount);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have set %s(%i)'s score to %i.", ReturnPlayerName(targetid), targetid, amount);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:givecash(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid, amount;
	if (sscanf(params, "ui", targetid, amount))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /givecash [player] [amount]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	GivePlayerMoney(targetid, amount);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has given you money $%i.", ReturnPlayerName(playerid), playerid, amount);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have given %s(%i)'s money $%i.", ReturnPlayerName(targetid), targetid, amount);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:givescore(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid, amount;
	if (sscanf(params, "ui", targetid, amount))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /givescore [player] [amount]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	SetPlayerScore(targetid, amount);
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has given you score to %i.", ReturnPlayerName(playerid), playerid, amount);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have given %s(%i)'s score %i.", ReturnPlayerName(targetid), targetid, amount);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:spawncar(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new vehicleid;
	if (sscanf(params, "i", vehicleid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /spawncar [vehicleid]");
	}

	if (! IsValidVehicle(vehicleid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified vehicle is not created.");
	}

	SetVehicleToRespawn(vehicleid);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "You have respawned vehicle id %i.", vehicleid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:destroycar(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new vehicleid;
	if (sscanf(params, "i", vehicleid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /destroycar [vehicleid]");
	}

	if (! IsValidVehicle(vehicleid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified vehicle is not created.");
	}

	SetVehicleToRespawn(vehicleid);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "You have destroyed vehicle id %i.", vehicleid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:setkills(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid, amount;
	if (sscanf(params, "ui", targetid, amount))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setkills [player] [amount]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	yoursql_set_field_int(SQL:0, "users/kills", yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(targetid)), amount);
	pStats[playerid][userKills] = amount;

	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set your kills to %i.", ReturnPlayerName(playerid), playerid, amount);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have set %s(%i)'s kills to %i.", ReturnPlayerName(targetid), targetid, amount);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:setdeaths(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new targetid, amount;
	if (sscanf(params, "ui", targetid, amount))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setdeaths [player] [amount]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	yoursql_set_field_int(SQL:0, "users/deaths", yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(targetid)), amount);
	pStats[playerid][userDeaths] = amount;

	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set your deaths to %i.", ReturnPlayerName(playerid), playerid, amount);
	SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have set %s(%i)'s deahs to %i.", ReturnPlayerName(targetid), targetid, amount);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:spawncars(playerid)
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	for (new i; i < MAX_VEHICLES; i++)
	{
	    foreach (new p : Player)
	    {
	        if (GetPlayerVehicleID(p) == i)
	        {
	            break;
			}
			else
			{
				SetVehicleToRespawn(i);
			}
        }
	}

	foreach (new i : Player)
	{
		PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	}

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) have respawned all unused vehicles.", ReturnPlayerName(playerid), playerid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:removedrops(playerid)
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	for (new i; i < MAX_DROPS; i++)
	{
        if (! IsValidDynamicObject(gDropObject[i]))
        {
	        DestroyDynamicObject(gDropObject[i]);
			DestroyDynamicArea(gDropAreaid[i]);
			KillTimer(gDropTimer[i]);
        }
	}

	foreach (new i : Player)
	{
		PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	}

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) have removed all ground weapons.", ReturnPlayerName(playerid), playerid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:giveallhelmet(playerid)
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	foreach (new i : Player)
	{
        PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
        if (! pHasHelmet[i])
        {
			SetPlayerAttachedObject(i,0,18638,2,0.173000,0.024999,-0.003000,0.000000,0.000000,0.000000,1.000000,1.000000,1.000000); //skin 102
			pHasHelmet[i] = true;
		}
	}

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has given all players a Protection Helmet.", ReturnPlayerName(playerid), playerid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:giveallmask(playerid)
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	foreach (new i : Player)
	{
        PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
        if (! pHasMask[i])
        {
			SetPlayerAttachedObject(i, 1, 19472, 2, -0.022000, 0.137000, 0.018999, 3.899994, 85.999961, 92.999984, 0.923999, 1.141000, 1.026999);
            pHasMask[i] = true;
		}
	}

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has given all players a Gas Mask.", ReturnPlayerName(playerid), playerid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

//Admin level 4+
CMD:rban(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new targetid, reason[35], days;
	if (sscanf(params, "is[35]I(0)", targetid, reason, days))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /rban [player] [reason] [*days (default 0 permanent)]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

    if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You can't ban yourself.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (days < 0)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid days, must be greater than 0 for temp ban, or 0 for permanent ban.");
	}

	if (strlen(reason) < 3 || strlen(reason) > 35)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid reason length, must be b/w 0-35 characters.");
	}

	new bandate[18], date[3], time;
	getdate(date[0], date[1], date[2]);

	new month[15];
	switch (date[1])
	{
	    case 1: month = "January";
	    case 2: month = "Feburary";
	    case 3: month = "March";
	    case 4: month = "April";
	    case 5: month = "May";
	    case 6: month = "June";
	    case 7: month = "July";
	    case 8: month = "August";
	    case 9: month = "September";
	    case 10: month = "October";
	    case 11: month = "November";
	    case 12: month = "December";
	}

	format(bandate, sizeof(bandate), "%02i %s, %i", date[2], month, date[0]);

	if (days == 0)
	{
		time = 0;
	}
	else
	{
		time = ((days * 24 * 60 * 60) + gettime());
	}

	yoursql_multiset_row(SQL:0, "bans", "sssssii", "name", ReturnPlayerName(targetid), "ip", ReturnPlayerIp(targetid), "admin_name", ReturnPlayerName(playerid), "reason", reason, "date", bandate, "type", (! days) ? (3) : (2), "expire", time);

	new buf[150];
	if (! days)
	{
	    format(buf, sizeof(buf), "%s(%i) has been banned by admin %s(%i) [RANGE PERMANENT].", ReturnPlayerName(targetid), targetid, ReturnPlayerName(playerid), playerid);
		SendClientMessageToAll(COLOR_RED, buf);
	    format(buf, sizeof(buf), "Reason: %s", reason);
		SendClientMessageToAll(COLOR_RED, buf);
	}
	else
	{
	    format(buf, sizeof(buf), "%s(%i) has been banned by admin %s(%i) for %i days [RANGE TEMPERORARY].", ReturnPlayerName(targetid), targetid, ReturnPlayerName(playerid), playerid, days);
		SendClientMessageToAll(COLOR_RED, buf);
	    format(buf, sizeof(buf), "Reason: %s", reason);
		SendClientMessageToAll(COLOR_RED, buf);
	}

 	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	Kick(targetid);
	return 1;
}

CMD:ripban(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new ip[18], reason[35], days;
	if (sscanf(params, "s[18]s[35]I(0)", ip, reason, days))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /ripban [ip] [reason] [*days (default 0 permanent)]");
	}

	if (! IsValidIp(ip))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid Ip. specified.");
	}

    if (! strcmp(ip, ReturnPlayerIp(playerid)))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You can't ban yourself.");
	}

	if (pStats[playerid][userAdmin] < yoursql_get_field_int(SQL:0, "users/admin", yoursql_get_row(SQL:0, "users", "ip = %s", ip)))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (days < 0)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid days, must be greater than 0 for temp ban, or 0 for permanent ban.");
	}

	if (strlen(reason) < 3 || strlen(reason) > 35)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid reason length, must be b/w 0-35 characters.");
	}

	new bandate[18], date[3], time;
	getdate(date[0], date[1], date[2]);

	new month[15];
	switch (date[1])
	{
	    case 1: month = "January";
	    case 2: month = "Feburary";
	    case 3: month = "March";
	    case 4: month = "April";
	    case 5: month = "May";
	    case 6: month = "June";
	    case 7: month = "July";
	    case 8: month = "August";
	    case 9: month = "September";
	    case 10: month = "October";
	    case 11: month = "November";
	    case 12: month = "December";
	}

	format(bandate, sizeof(bandate), "%02i %s, %i", date[2], month, date[0]);

	if (days == 0)
	{
		time = 0;
	}
	else
	{
		time = ((days * 24 * 60 * 60) + gettime());
	}

	new name[MAX_PLAYER_NAME];
	yoursql_get_field(SQL:0, "users/name", yoursql_get_row(SQL:0, "users", "ip = %s", ip), name, MAX_PLAYER_NAME);
	yoursql_multiset_row(SQL:0, "bans", "sssssii", "name", name, "ip", ip, "admin_name", ReturnPlayerName(playerid), "reason", reason, "date", bandate, "type", (! days) ? (3) : (2), "expire", time);

	new id = -1;
	foreach (new i : Player)
	{
	    if (! strcmp(ip, ReturnPlayerIp(i)))
	    {
	        id = i;
	        break;
	    }
	}

	new buf[150];
	if (! days)
	{
	    format(buf, sizeof(buf), "%s(%i) has been banned by admin %s(%i) [RANGE PERMANENT].", name, id, ReturnPlayerName(playerid), playerid);
		SendClientMessageToAll(COLOR_RED, buf);
	    format(buf, sizeof(buf), "Reason: %s", reason);
		SendClientMessageToAll(COLOR_RED, buf);
	}
	else
	{
	    format(buf, sizeof(buf), "%s(%i) has been banned by admin %s(%i) for %i days [RANGE TEMPERORARY].", name, id, ReturnPlayerName(playerid), playerid, days);
		SendClientMessageToAll(COLOR_RED, buf);
	    format(buf, sizeof(buf), "Reason: %s", reason);
		SendClientMessageToAll(COLOR_RED, buf);
	}

 	PlayerPlaySound(id, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	Kick(id);

	return 1;
}

CMD:roban(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new name[MAX_PLAYER_NAME], reason[35], days;
	if (sscanf(params, "s[24]s[35]I(0)", name, reason, days))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /roban [name] [reason] [*days (default 0 permanent)]");
	}

	if (yoursql_get_row(SQL:0, "users", "name = %s", name) == SQL_INVALID_ROW)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified username isn't registered.");
	}

    if (! strcmp(name, ReturnPlayerName(playerid)))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You can't ban yourself.");
	}

	if (pStats[playerid][userAdmin] < yoursql_get_field_int(SQL:0, "users/admin", yoursql_get_row(SQL:0, "users", "name = %s", name)))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (days < 0)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid days, must be greater than 0 for temp ban, or 0 for permanent ban.");
	}

	if (strlen(reason) < 3 || strlen(reason) > 35)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid reason length, must be b/w 0-35 characters.");
	}

	new bandate[18], date[3], time;
	getdate(date[0], date[1], date[2]);

	new month[15];
	switch (date[1])
	{
	    case 1: month = "January";
	    case 2: month = "Feburary";
	    case 3: month = "March";
	    case 4: month = "April";
	    case 5: month = "May";
	    case 6: month = "June";
	    case 7: month = "July";
	    case 8: month = "August";
	    case 9: month = "September";
	    case 10: month = "October";
	    case 11: month = "November";
	    case 12: month = "December";
	}

	format(bandate, sizeof(bandate), "%02i %s, %i", date[2], month, date[0]);

	if (days == 0)
	{
		time = 0;
	}
	else
	{
		time = ((days * 24 * 60 * 60) + gettime());
	}

	new ip[18];
	yoursql_get_field(SQL:0, "users/ip", yoursql_get_row(SQL:0, "users", "name = %s", name), ip);
	yoursql_multiset_row(SQL:0, "bans", "sssssii", "name", name, "ip", ip, "admin_name", ReturnPlayerName(playerid), "reason", reason, "date", bandate, "type", (! days) ? (3) : (2), "expire", time);

	new buf[150];
	if (! days)
	{
	    format(buf, sizeof(buf), "%s(%i) has been offline banned by admin %s(%i) [RANGE PERMANENT].", name, ReturnPlayerName(playerid), playerid);
		SendClientMessageToAll(COLOR_RED, buf);
	    format(buf, sizeof(buf), "Reason: %s", reason);
		SendClientMessageToAll(COLOR_RED, buf);
	}
	else
	{
	    format(buf, sizeof(buf), "%s(%i) has been offline banned by admin %s(%i) for %i days [RANGE TEMPERORARY].", name, ReturnPlayerName(playerid), playerid, days);
		SendClientMessageToAll(COLOR_RED, buf);
	    format(buf, sizeof(buf), "Reason: %s", reason);
		SendClientMessageToAll(COLOR_RED, buf);
	}

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}

CMD:fakedeath(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new targetid, killerid, weaponid;
	if (sscanf(params, "uui", targetid, killerid, weaponid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /fakedeath [player] [killer] [weapon]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (! IsPlayerConnected(killerid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified killer is not conected.");
	}

	if (0 > weaponid > 51)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid weapon id.");
	}

	new weaponname[35];
	GetWeaponName(weaponid, weaponname, sizeof(weaponname));
	SendDeathMessage(killerid, targetid, weaponid);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Fake death sent [Player: %s | Killer: %s | Weapon: %s]", ReturnPlayerName(targetid), ReturnPlayerName(killerid), weaponname);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:muteall(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	for (new i; i < MAX_PLAYERS; i++)
	{
        if (i != playerid)
        {
            if (pStats[playerid][userAdmin] < pStats[i][userAdmin])
            {
                pStats[i][userMuteTime] = 100000000;
            }
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		}
	}
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has muted all players.", ReturnPlayerName(playerid), playerid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:unmuteall(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	for (new i; i < MAX_PLAYERS; i++)
	{
        pStats[i][userMuteTime] = -1;
		PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	}
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has unmuted all players.", ReturnPlayerName(playerid), playerid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:setpass(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new name[MAX_PLAYER_NAME], newpass[35];
	if (sscanf(params, "s[24]s[35]", name, newpass))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setpass [name] [new password]");
	}

	new SQLRow:rowid = yoursql_get_row(SQL:0, "users", "name = %s", name);
	if (rowid == SQL_INVALID_ROW)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified username isn't registered.");
	}

	if (pStats[playerid][userAdmin] < yoursql_get_field_int(SQL:0, "users/admin", yoursql_get_row(SQL:0, "users", "name = %s", name)))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (strlen(newpass) < 4 || strlen(newpass) > 30)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid password length, must be b/w 4-30 characters.");
	}

	new hash[128];
	SHA256_PassHash(newpass, "aafGEsq13", hash, sizeof(hash));
	yoursql_set_field(SQL:0, "bans/password", rowid, hash);

	new buf[150];
 	format(buf, sizeof(buf), "You have reseted the password of '%s [A/C Id: %i]' to '%s'.", name, _:rowid, newpass);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	return 1;
}

CMD:giveallscore(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new amount;
	if (sscanf(params, "i", amount))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /giveallscore [amount]");
	}

	foreach (new i : Player)
	{
        PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		SetPlayerScore(i, GetPlayerScore(i) + amount);
	}

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has given all players %i score.", ReturnPlayerName(playerid), playerid, amount);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:giveallcash(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new amount;
	if (sscanf(params, "i", amount))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /giveallcash [amount]");
	}

	foreach (new i : Player)
	{
        PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		GivePlayerMoney(i, amount);
	}

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has given all players $%i.", ReturnPlayerName(playerid), playerid, amount);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:setalltime(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new id;
	if (sscanf(params, "i", id))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setalltime [id]");
	}

	foreach (new i : Player)
	{
        PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		SetPlayerTime(i, id, 0);
	}

	gTimeGap = 0;
	gServerTime = id;

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set all players time to %i.", ReturnPlayerName(playerid), playerid, id);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:setallweather(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new id;
	if (sscanf(params, "i", id))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setallweather [id]");
	}

	foreach (new i : Player)
	{
        PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
		SetPlayerWeather(i, id);
	}

	gTimeGap = 0;
	gServerWeather = id;

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has set all players weather to %i.", ReturnPlayerName(playerid), playerid, id);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:cleardwindow(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	for (new i; i < 10; i++)
	{
		SendDeathMessage(6000, 5005, 255);
	}

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has cleared all players death window.", ReturnPlayerName(playerid), playerid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:giveallweapon(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new weapon[32], ammo;
	if (sscanf(params, "s[32]I(250)", weapon, ammo))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /giveallweapon [weapon] [ammo]");
	}

	new weaponid;
	if (isnumeric(weapon))
	{
		weaponid = strval(weapon);
	}
	else
	{
		weaponid = GetWeaponIDFromName(weapon);
	}

	if (1 > weaponid > 46)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid weapon id/name.");
	}

	GetWeaponName(weaponid, weapon, sizeof(weapon));
   	foreach (new i : Player)
	{
		GivePlayerWeapon(i, weaponid, ammo);
		PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	}

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has given all players %s[id: %i] with %i ammo.", ReturnPlayerName(playerid), playerid, weapon, weaponid, ammo);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

GetXYInFrontOfPlayer(playerid, &Float:x, &Float:y, Float:distance)
{
	new Float:a;
	GetPlayerPos(playerid, x, y, a);
	GetPlayerFacingAngle(playerid, a);

	if (GetPlayerVehicleID(playerid))
	{
 		GetVehicleZAngle(GetPlayerVehicleID(playerid), a);
	}

	x += (distance * floatsin(-a, degrees));
	y += (distance * floatcos(-a, degrees));
}

CMD:object(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new model;
	if (sscanf(params, "i", model))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /object [model]");
	}

	if (0 > model > 20000)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified model is invalid.");
	}

	new Float:x, Float:y, Float:z, Float:a;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, a);

	GetXYInFrontOfPlayer(playerid, x, y, 5.0);

	new object = CreateObject(model, x, y, z + 5.0, 0, 0, a);

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "You have created a new object (model: %i, id: %i).", model, object);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, "You can edit the object via /editobject and destroy it via /destroyobject.");
	return 1;
}

CMD:destroyobject(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new object;
	if (sscanf(params, "i", object))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /destroyobject [object]");
	}

	if (! IsValidObject(object))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified object is invalid.");
	}

	DestroyObject(object);

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "You have destroyed the object id %i.", object);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:editobject(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new object;
	if (sscanf(params, "i", object))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /editobject [object]");
	}

	if (! IsValidObject(object))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified object is invalid.");
	}

	EditObject(playerid, object);

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "You are now editing the object id %i.", object);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, "Hold SPACE and use MOUSE to move camera.");
	return 1;
}

//Admin level 5+
CMD:gmx(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 5)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 5+ to use this command.");
	}

	new time;
	if (sscanf(params, "I(0)", time))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /gmx [*interval]");
	}

	if (time < 0 || time > 5 * 60)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid restart time, must be b/w 0-360 seconds.");
	}

	if (time > 0)
	{
	    SendClientMessageToAll(COLOR_ORANGE, "_______________________________________________");
	    SendClientMessageToAll(COLOR_ORANGE, " ");
		new buf[150];
		format(buf, sizeof(buf), "Admin %s(%i) has set the gamemode to reboot. The restart will occur in %i seconds.", ReturnPlayerName(playerid), playerid, time);
		SendClientMessageToAll(COLOR_ORANGE_RED, buf);
	    SendClientMessageToAll(COLOR_ORANGE, " ");
	    SendClientMessageToAll(COLOR_ORANGE, "_______________________________________________");

	    SetTimer("OnServerRequestRestart", time * 1000, false);
	}
	else
	{
	    SendClientMessageToAll(COLOR_ORANGE, "_______________________________________________");
	    SendClientMessageToAll(COLOR_ORANGE, " ");
		new buf[150];
		format(buf, sizeof(buf), "Admin %s(%i) has restarted the gamemode, please wait while the server startsup again.", ReturnPlayerName(playerid), playerid);
		SendClientMessageToAll(COLOR_ORANGE_RED, buf);
	    SendClientMessageToAll(COLOR_ORANGE, " ");
	    SendClientMessageToAll(COLOR_ORANGE, "_______________________________________________");

	    SendRconCommand("gmx");
	}
	return 1;
}

forward OnServerRequestRestart();
public 	OnServerRequestRestart()
{
	SendRconCommand("gmx");
}

CMD:fakechat(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 5)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 5+ to use this command.");
	}

	new targetid, text[129];
	if (sscanf(params, "us[129]", targetid, text))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /fakechat [player] [text]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

    new buf[150];
	format(buf, sizeof(buf), "(%i) %s: %s", targetid, ReturnPlayerName(targetid), text);
    SendClientMessageToAll(GetPlayerColor(targetid), buf);
	format(buf, sizeof(buf), "Fake chat sent [Player: %s(%i) | Text: %s]", ReturnPlayerName(targetid), targetid, text);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:setlevel(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 5)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 5+ to use this command.");
	}

	new targetid, level;
	if (sscanf(params, "ui", targetid, level))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setlevel [player] [level]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	new buf[150];
	if (level < 0 || level > MAX_ADMIN_LEVELS)
	{
		format(buf, sizeof(buf), "Invalid level, mus be b/w 0-%i.", MAX_ADMIN_LEVELS);
		return SendClientMessage(playerid, COLOR_TOMATO, buf);
	}

	if (level == pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Player is already of that level.");
	}

    if (pStats[playerid][userAdmin] < level)
    {
        GameTextForPlayer(targetid, "~g~~h~~h~~h~Promoted", 5000, 1);
		format(buf, sizeof(buf), "You have been promoted to admin level %i by %s(%i), Congratulation.", level, ReturnPlayerName(playerid), playerid);
		SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
		format(buf, sizeof(buf), "You have promoted %s(%i) to admin level %i.", ReturnPlayerName(targetid), targetid, level);
        SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
    }
    else if (pStats[playerid][userAdmin] > level)
    {
        GameTextForPlayer(targetid, "~r~~h~~h~~h~Demoted", 5000, 1);
		format(buf, sizeof(buf), "You have been demoted to admin level %i by %s(%i), Sorry.", level, ReturnPlayerName(playerid), playerid);
		SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
		format(buf, sizeof(buf), "You have demoted %s(%i) to admin level %i.", ReturnPlayerName(targetid), targetid, level);
        SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
    }
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	yoursql_set_field_int(SQL:0, "users/admin", yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(targetid)), level);

    pStats[targetid][userAdmin] = level;
	return 1;
}

CMD:setpremium(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 5)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 5+ to use this command.");
	}

	new targetid, set;
	if (sscanf(params, "ui", targetid, set))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setpremium [player] [1 - set/0 - remove]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	if (pStats[playerid][userAdmin] < pStats[targetid][userAdmin])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot use this command on higher level admin.");
	}

	if (0 > set > 1)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid 'set' value, use 0 to remove premium or 1 to set premium");
	}

	if (pStats[targetid][userPremium] && set)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Player is already a premium user.");
	}
	else if (! pStats[targetid][userPremium] && ! set)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Player is already non-premium user.");
	}

	new buf[150];
    if (! pStats[targetid][userPremium] && set)
    {
        GameTextForPlayer(targetid, "~g~~h~~h~~h~Premium", 5000, 1);
		format(buf, sizeof(buf), "You have been set as a premium user by admin %s(%i), Congratulation.", ReturnPlayerName(playerid), playerid);
		SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
		format(buf, sizeof(buf), "You have set %s(%i) to a premium user.", ReturnPlayerName(targetid), targetid);
        SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
    }
    else
    {
        GameTextForPlayer(targetid, "~r~~h~~h~~h~Premium removed", 5000, 1);
		format(buf, sizeof(buf), "Your premium has been removed by admin %s(%i).", ReturnPlayerName(playerid), playerid);
		SendClientMessage(targetid, COLOR_DODGER_BLUE, buf);
		format(buf, sizeof(buf), "You have removed %s(%i)'s premium eligablity.", ReturnPlayerName(targetid), targetid);
        SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
    }
	PlayerPlaySound(targetid, 1057, 0.0, 0.0, 0.0);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	yoursql_set_field_int(SQL:0, "users/vip", yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(targetid)), set);

    pStats[targetid][userPremium] = bool:set;
	return 1;
}

//Player commands
CMD:admins(playerid)
{
	SendClientMessage(playerid, COLOR_ORANGE_RED, " ");
	SendClientMessage(playerid, COLOR_GREEN, "- Online Administrators -");
	new color, status[10], rank[25], buf[150];
	foreach (new i : Player)
	{
	    if (pStats[i][userAdmin] || IsPlayerAdmin(i))
	    {
	        if (pStats[i][userOnDuty])
			{
			    color = COLOR_HOT_PINK;
				status = "On Duty";
	        }
			else
			{
			    color = COLOR_WHITE;
				status = "Playing";
			}

			if (IsPlayerAdmin(i))
			{
				rank = "RCON Administrator";
			}
			else
			{
			    switch (pStats[i][userAdmin])
			    {
			        case 1: rank = "Moderator";
			        case 2: rank = "Junior Administrator";
			        case 3: rank = "Senior Administrator";
			        case 4: rank = "Lead Administrator";
			        case 5: rank = "Server Manager";
					default: rank = "Server Owner";
			    }
   			}

	    	format(buf, sizeof(buf), "%s(%i) | Level: %i(%s) | Status: %s", ReturnPlayerName(i), i, pStats[i][userAdmin], rank, status);
	        SendClientMessage(playerid, color, buf);
	    }
	}
	if (! buf[0])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "No admin online.");
	}
	SendClientMessage(playerid, COLOR_ORANGE_RED, " ");
	return 1;
}

CMD:vips(playerid)
{
	SendClientMessage(playerid, COLOR_ORANGE_RED, " ");
	SendClientMessage(playerid, COLOR_GREEN, "- Online Premium Users -");
	new buf[150];
	foreach (new i : Player)
	{
	    if (pStats[i][userPremium])
	    {
		    format(buf, sizeof(buf), "%s(%i)", ReturnPlayerName(i), i);
		    SendClientMessage(playerid, COLOR_WHITE, buf);
		}
	}
	if (! buf[0])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "No vip/premium user online.");
	}
	SendClientMessage(playerid, COLOR_ORANGE_RED, " ");
	return 1;
}

CMD:report(playerid, params[])
{
	new targetid, reason[100];
	if (sscanf(params, "us[100]", targetid, reason))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /report [player] [reason]");
	}

	if (strlen(reason) < 1)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Report reason length must not be empty.");
	}

	if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot report yourself.");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	new hour, minute, second;
	gettime(hour, minute, second);

	new buf[145];
	format(buf, sizeof(buf), "REPORT: %s(%i) has reported against %s(%i), type /reports to check it.", ReturnPlayerName(playerid), playerid, ReturnPlayerName(targetid), targetid);
	foreach (new i : Player)
	{
		if (pStats[i][userAdmin] || IsPlayerAdmin(i))
		{
			SendClientMessage(i, COLOR_RED, buf);
		}
	}

	for (new i, j = sizeof(gReport) - 1; i < j; i++)
	{
	    gReport[i + 1][rAgainst] = gReport[i][rAgainst];
	    gReport[i + 1][rAgainstId] = gReport[i][rAgainstId];
	    gReport[i + 1][rBy] = gReport[i][rBy];
	    gReport[i + 1][rById] = gReport[i][rById];
	    gReport[i + 1][rReason] = gReport[i][rReason];
	    gReport[i + 1][rTime] = gReport[i][rTime];
	    gReport[i + 1][rChecked] = gReport[i][rChecked];
	}

	GetPlayerName(targetid, gReport[0][rAgainst], MAX_PLAYER_NAME);
	gReport[0][rAgainstId] = targetid;
	GetPlayerName(playerid, gReport[0][rBy], MAX_PLAYER_NAME);
	gReport[0][rById]= playerid;
 	format(gReport[0][rReason], 100, reason);
 	format(gReport[0][rTime], 15, "%i:%i", hour, minute);
 	gReport[0][rChecked] = false;

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	format(buf, sizeof(buf), "Your report against %s(%i) has been sent to online admins.", ReturnPlayerName(targetid), targetid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:changename(playerid, params[])
{
	new name[MAX_PLAYER_NAME];
    if (sscanf(params, "s[24]", name))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /changename [newname]");
	}

	if (strlen(name) < 4 || strlen(name) > MAX_PLAYER_NAME)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid user name length, must be b/w 4-24.");
	}

	if (yoursql_get_row(SQL:0, "user", "name = %s", name) != SQL_INVALID_ROW)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "That username is already registered, try another one!");
	}

    yoursql_set_field(SQL:0, "user/name", yoursql_get_row(SQL:0, "user", "name = %s", ReturnPlayerName(playerid)), name);

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "You have changed your username from '%s' to '%s'.", ReturnPlayerName(playerid), name);
	SendClientMessage(playerid, COLOR_GREEN, buf);
	GameTextForPlayer(playerid, "~w~Username changed", 5000, 3);

	SetPlayerName(playerid, name);
	return 1;
}

CMD:changepass(playerid, params[])
{
	new pass[30];
    if (sscanf(params, "s[30]", pass))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /changepass [newpass]");
	}

	if (strlen(pass) < 4 || strlen(pass) > MAX_PLAYER_NAME)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid password length, must be b/w 4-24.");
	}

	new hash[128];
	SHA256_PassHash(pass, "aafGEsq13", hash, sizeof(hash));
	yoursql_set_field(SQL:0, "user/password", yoursql_get_row(SQL:0, "user", "name = %s", ReturnPlayerName(playerid)), hash);

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	SendClientMessage(playerid, COLOR_GREEN, "You have successfully changed your account password.");
	GameTextForPlayer(playerid, "~w~Password changed", 5000, 3);
	return 1;
}

CMD:autologin(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	ShowPlayerDialog(playerid, DIALOG_ID_AUTO_LOGIN, DIALOG_STYLE_MSGBOX, "Autologin confirmation:", "Press "GREEN"ENABLE "WHITE"to switch auto login on or "RED"DISABLE "WHITE"to off.\n\nAutologin allows you to directly login without entering password when your ip is matching to the one registered with.", "Enable", "Disable");
	return 1;
}

CMD:nopm(playerid)
{
	if (! pStats[playerid][userNoPM])
	{
	    pStats[playerid][userNoPM] = true;

	    SendClientMessage(playerid, COLOR_TOMATO, "You are no longer accepting private messages (DND. On).");
	}
	else
	{
	    pStats[playerid][userNoPM] = false;

	    SendClientMessage(playerid, COLOR_GREEN, "You are now accepting private messages (DND. Off).");
	}
	return 1;
}
CMD:dnd(playerid)
{
	return cmd_nopm(playerid);
}

CMD:pm(playerid, params[])
{
	new targetid, text[128];
	if (sscanf(params, "us[128]", targetid, text))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /pm [player] [message]");
	}

	if (!IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not connected.");
	}

	if (targetid == playerid)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You cannot PM yourself.");
	}

	new buf[150];
	if (pStats[targetid][userNoPM])
	{
	    format(buf, sizeof(buf), "%s(%i) is not accepting private messages at the moment (DND).", ReturnPlayerName(targetid), targetid);
		return SendClientMessage(playerid, COLOR_TOMATO, buf);
	}

	PlayerPlaySound(playerid, 1085, 0.0, 0.0, 0.0);
	PlayerPlaySound(targetid, 1085, 0.0, 0.0, 0.0);

	format(buf, sizeof(buf), "PM to %s(%i): %s", ReturnPlayerName(targetid), targetid, text);
	SendClientMessage(playerid, COLOR_YELLOW, buf);
	format(buf, sizeof(buf), "PM from %s(%i): %s", ReturnPlayerName(playerid), playerid, text);
	SendClientMessage(targetid, COLOR_YELLOW, buf);

	format(buf, sizeof(buf), "[READPM] %s(%i) to %s(%i): %s", ReturnPlayerName(playerid), playerid, ReturnPlayerName(targetid), targetid, text);
	foreach (new i : Player)
	{
	    if (pStats[i][userAdmin] > 2)
	    {
	        SendClientMessage(i, COLOR_GREY, buf);
	    }
	}

	pStats[playerid][userLastPM] = targetid;
	return 1;
}

CMD:reply(playerid, params[])
{
	new text[128];
	if (sscanf(params, "s[128]", text))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /reply [message]");
	}

 	new targetid = pStats[playerid][userLastPM];
	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The player is not connected anymore.");
	}

	new buf[150];
	if (pStats[targetid][userNoPM])
	{
	    format(buf, sizeof(buf), "%s(%i) is not accepting private messages at the moment (DND).", ReturnPlayerName(targetid), targetid);
		return SendClientMessage(playerid, COLOR_TOMATO, buf);
	}

	format(buf, sizeof(buf), "PM to %s(%i): %s", ReturnPlayerName(targetid), targetid, text);
	SendClientMessage(playerid, COLOR_YELLOW, buf);
	format(buf, sizeof(buf), "PM from %s(%i): %s", ReturnPlayerName(playerid), playerid, text);
	SendClientMessage(targetid, COLOR_YELLOW, buf);

	format(buf, sizeof(buf), "[READPM] %s(%i) to %s(%i): %s", ReturnPlayerName(playerid), playerid, ReturnPlayerName(targetid), targetid, text);
	foreach (new i : Player)
	{
	    if (pStats[i][userAdmin] > 2)
	    {
	        SendClientMessage(i, COLOR_GREY, buf);
	    }
	}
	return 1;
}

CMD:time(playerid, params[])
{
	new time[3];
	gettime(time[0], time[1], time[2]);

	new buf[150];
	format(buf, sizeof(buf), "Server time: %i:%i:%i", time[0], time[1], time[2]);
	SendClientMessage(playerid, COLOR_WHITE, buf);

	format(buf, sizeof(buf), "~w~~h~%i:%i", time[0], time[1]);
	GameTextForPlayer(playerid, buf, 5000, 1);
	return 1;
}

CMD:id(playerid, params[])
{
	new name[MAX_PLAYER_NAME];
	if (sscanf(params, "s[24]", name))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /id [name]");
	}

	SendClientMessage(playerid, COLOR_DODGER_BLUE, " ");
	new buf[150];
	format(buf, sizeof(buf), "- Search result for '%s' -", name);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);

	new count;
	foreach (new i : Player)
	{
	    if (strfind(ReturnPlayerName(i), name, true) != -1)
	    {
	        count++;
			format(buf, sizeof(buf), "%i. %s(%i)", count, ReturnPlayerName(i), i);
			SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
		}
	}

	if (! count)
	{
		return SendClientMessage(playerid, COLOR_DODGER_BLUE, "No match found.");
	}
	return 1;
}
CMD:getid(playerid, params[])
{
	return cmd_id(playerid, params);
}

GetPlayerConnectedTime(playerid, &hours, &minutes, &seconds)
{
	new connected_time = NetStats_GetConnectedTime(playerid);
	seconds = (connected_time / 1000) % 60;
	minutes = (connected_time / (1000 * 60)) % 60;
	hours = (connected_time / (1000 * 60 * 60));
}

CMD:stats(playerid, params[])
{
	new targetid;
	if (sscanf(params, "u", targetid))
	{
  		targetid = playerid;
		SendClientMessage(playerid, COLOR_KHAKI, "TIP: You can also view other players stats by /stats [player]");
	}

	if (! IsPlayerConnected(targetid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The specified player is not conected.");
	}

	new SQLRow:rowid = yoursql_get_row(SQL:0, "users", "name = %s", ReturnPlayerName(targetid));

	new buf[150];
	format(buf, sizeof(buf), "%s(%i)'s stats: (AccountID: %i)", ReturnPlayerName(targetid), targetid, _:rowid);
	SendClientMessage(playerid, COLOR_GREEN, buf);

	new Float:ratio;
	if (pStats[targetid][userDeaths] <= 0)
	{
		ratio = 0.0;
	}
	else
	{
		ratio = floatdiv(pStats[targetid][userKills], pStats[targetid][userDeaths]);
	}

 	new team[35];
	if (0 <= pTeam[targetid] < MAX_TEAMS)
	{
		strcat(team, gTeam[pTeam[targetid]][teamName]);
	}
	else
	{
		team = "No Team";
	}

	format(buf, sizeof(buf), "Team: %s, Class: %s, Rank: %s(%i), Score: %i, Money: $%i, Kills: %i, Deaths: %i, Ratio: %0.2f", team, gClass[pClass[targetid]][className], gRank[pRank[targetid]][rankName], pRank[targetid], GetPlayerScore(targetid), GetPlayerMoney(targetid), pStats[targetid][userKills], pStats[targetid][userDeaths], ratio);
	SendClientMessage(playerid, COLOR_GREEN, buf);

	new admin_rank[25];
	if (IsPlayerAdmin(targetid))
	{
		admin_rank = "RCON Administrator";
	}
	else
	{
	    switch (pStats[targetid][userAdmin])
	    {
	        case 1: admin_rank = "Moderator";
	        case 2: admin_rank = "Junior Administrator";
	        case 3: admin_rank = "Senior Administrator";
	        case 4: admin_rank = "Lead Administrator";
		    case 5: admin_rank = "Server Manager";
			default: admin_rank = "Server Owner";
	    }
 	}

 	new premium[5];
 	if (pStats[targetid][userPremium])
 	{
	 	premium = "Yes";
	}
	else
	{
		premium = "No";
	}

 	new hours, minutes, seconds;
 	GetPlayerConnectedTime(targetid, hours, minutes, seconds);
 	hours += yoursql_get_field_int(SQL:0, "users/hours", rowid);
 	minutes += yoursql_get_field_int(SQL:0, "users/minutes", rowid);
 	seconds += yoursql_get_field_int(SQL:0, "users/seconds", rowid);
	if (seconds >= 60)
	{
	    seconds = 0;
	    minutes++;
	    if (minutes >= 60)
	    {
	        minutes = 0;
	        hours++;
	    }
	}

	format(buf, sizeof(buf), "Zones Captured: %i, Headshots: %i, Admin Level: %i (%s), Premium: %s, Time Played: %i hours, %i minutes, %i seconds", pStats[targetid][userZones], pStats[targetid][userHeadshots], pStats[targetid][userAdmin], admin_rank, premium, hours, minutes, seconds);
	SendClientMessage(playerid, COLOR_GREEN, buf);

	new register_on[25];
	yoursql_get_field(SQL:0, "users/register_on", rowid, register_on);

	new helmet[5];
 	if (pHasHelmet[targetid])
 	{
	 	helmet = "Yes";
	}
	else
	{
		helmet = "No";
	}

	new mask[5];
 	if (pHasMask[targetid])
 	{
	 	mask = "Yes";
	}
	else
	{
		mask = "No";
	}

	format(buf, sizeof(buf), "Registeration Date: %s, Helmet: %s, Gas Mask: %s", register_on, helmet, mask);
	SendClientMessage(playerid, COLOR_GREEN, buf);
	return 1;
}

CMD:richlist(playerid)
{
	new data[MAX_PLAYERS][2];
	foreach (new i : Player)
	{
	    data[i][0] = GetPlayerMoney(i);
	    data[i][1] = i;
	}

	QuickSort_Pair(data, true, 0, Iter_Count(Player));

	SendClientMessage(playerid, COLOR_DODGER_BLUE, "Top 5 rich players:");
	new buf[150];
	for (new i; i < 5; i++)
	{
	    if (data[i][0])
	    {
	        format(buf, sizeof(buf), "%i. %s(%i) - $%i", i + 1, ReturnPlayerName(data[i][1]), data[i][1], data[i][0]);
			SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
		}
	}
	return 1;
}

CMD:scorelist(playerid)
{
	new data[MAX_PLAYERS][2];
	foreach (new i : Player)
	{
	    data[i][0] = GetPlayerScore(i);
	    data[i][1] = i;
	}

	QuickSort_Pair(data, true, 0, Iter_Count(Player));

	SendClientMessage(playerid, COLOR_DODGER_BLUE, "Top 5 score players:");
	new buf[150];
	for (new i; i < 5; i++)
	{
	    if (data[i][0])
	    {
	        format(buf, sizeof(buf), "%i. %s(%i) - %i", i + 1, ReturnPlayerName(data[i][1]), data[i][1], data[i][0]);
			SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
		}
	}
	return 1;
}

CMD:med(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! pInventory[playerid][0])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You don't have any medickit with you.");
	}

    new Float:hp;
 	GetPlayerHealth(playerid, hp);
 	if (hp >= 100.0)
 	{
 	    return SendClientMessage(playerid, COLOR_TOMATO, "You already have full health (100%).");
 	}

 	if(hp + 35.0 >= 100.0)
	{
	 	SetPlayerHealth(playerid, 100.0);
 	}
 	else
 	{
	 	SetPlayerHealth(playerid, hp + 35.0);
 	}

    pInventory[playerid][0]--;

 	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);

	new text[150];
	format(text, sizeof(text), "You have used a Medickit (+35# health), %i remaining.", pInventory[playerid][0]);
	SendClientMessage(playerid, COLOR_GREEN, text);

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	format(text, sizeof(text), "** (%i) %s have used a medickit.", playerid, ReturnPlayerName(playerid));

	foreach (new i : Player)
	{
		if (IsPlayerInRangeOfPoint(i, 10.0, x, y, z))
		{
		    SendClientMessage(i, COLOR_GREY, text);
		}
	}

	return 1;
}

CMD:trap(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (! pInventory[playerid][1])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You don't have a nettrap with you.");
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	if (! IsValidDynamicObject(pNetTrapObject[playerid][0]))
	{
		pNetTrapObject[playerid][0] = CreateDynamicObject(2945, x, y, z - 1.0, 90.0, 0.0, 0.0, 0, GetPlayerInterior(playerid));
		pNetTrapArea[playerid][0] = CreateDynamicSphere(x, y, z - 1.0, 3.0, 0, GetPlayerInterior(playerid));
		pNetTrapLabel[playerid][0] = CreateDynamic3DTextLabel("Nettrap 1", COLOR_WHITE, x, y, z, 100.0, .worldid = 0, .interiorid = GetPlayerInterior(playerid), .playerid = playerid);
		pNetTrapTimer[playerid][0] = SetTimerEx("OnNetTrapExpire", 10 * 60 * 1000, false, "ii", playerid, 0);
	}
	else if (! IsValidDynamicObject(pNetTrapObject[playerid][1]))
	{
		pNetTrapObject[playerid][1] = CreateDynamicObject(2945, x, y, z - 1.0, 90.0, 0.0, 0.0, 0, GetPlayerInterior(playerid));
		pNetTrapArea[playerid][1] = CreateDynamicSphere(x, y, z - 1.0, 3.0, 0, GetPlayerInterior(playerid));
		pNetTrapLabel[playerid][1] = CreateDynamic3DTextLabel("Nettrap 2", COLOR_WHITE, x, y, z, 100.0, .worldid = 0, .interiorid = GetPlayerInterior(playerid), .playerid = playerid);
		pNetTrapTimer[playerid][1] = SetTimerEx("OnNetTrapExpire", 10 * 60 * 1000, false, "ii", playerid, 0);
	}
	else
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You have 2 active nettraps, you should destroy one before planting a new trap (/destroytrap).");
	}

	SendClientMessage(playerid, COLOR_GREEN, "You have placed a nettrap at your ground (it will auto destroy after 10 minutes if vaccent).");
    SendClientMessage(playerid, COLOR_GREEN, "You can also destroy the nettrap by '/destroytrap [id]'.");

	pInventory[playerid][1]--;

	return 1;
}

CMD:destroytrap(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

    new index;
    if (sscanf(params, "i", index))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /destroytrap [1/2]");
	}

	if (index <= 0 || index > 2)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "The trap id must be 1 or 2.");
	}

	if (IsValidDynamicObject(pNetTrapObject[playerid][index - 1]))
	{
		DestroyDynamicObject(pNetTrapObject[playerid][index - 1]);
		DestroyDynamicArea(pNetTrapArea[playerid][index - 1]);
		DestroyDynamic3DTextLabel(pNetTrapLabel[playerid][index - 1]);
		KillTimer(pNetTrapTimer[playerid][index - 1]);

		new buf[150];
		format(buf, sizeof(buf), "You have destroyed your nettrap %i.", index);
		SendClientMessage(playerid, COLOR_GREEN, buf);
	}
	else
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The nettrap id isn't active.");
	}

	return 1;
}

forward OnNetTrapExpire(playerid, i);
public 	OnNetTrapExpire(playerid, i)
{
	new buf[150];
	format(buf, sizeof(buf), "Your nettrap %i has been auto expired.", i);
	SendClientMessage(playerid, COLOR_YELLOW, buf);

	DestroyDynamicObject(pNetTrapObject[playerid][i]);
	DestroyDynamicArea(pNetTrapArea[playerid][i]);
	DestroyDynamic3DTextLabel(pNetTrapLabel[playerid][i]);
}

CMD:dynamite(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (! pInventory[playerid][2])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You don't have a dynamite with you.");
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	for (new i; i < 3; i++)
	{
		if (! IsValidDynamicObject(pDynamiteObject[playerid][i]))
		{
			pDynamiteObject[playerid][i] = CreateDynamicObject(1654, x, y, z - 0.9414, 338.00000, -91.00000, -127.00000, 0, GetPlayerInterior(playerid));

			new buf[150];
			format(buf, sizeof(buf), "Dynamite %i", i + 1);
			pDynamiteLabel[playerid][i] = CreateDynamic3DTextLabel(buf, COLOR_WHITE, x, y, z, 100.0, .worldid = 0, .interiorid = GetPlayerInterior(playerid), .playerid = playerid);

			pInventory[playerid][2]--;

			SendClientMessage(playerid, COLOR_GREEN, "You have placed a dynamite at your ground; type '/det [id]' to detonate it.");
			SendClientMessage(playerid, COLOR_GREEN, "You can also destroy the dynamite by '/destroydynamite [id]'.");

			return 1;
		}
 	}

	SendClientMessage(playerid, COLOR_TOMATO, "You have 3 active dynamites, you should destroy one before planting a new dynamite (/destroydynamite).");

	return 1;
}

CMD:destroydynamite(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

    new index;
    if (sscanf(params, "i", index))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /destroydynamite [1/2/3]");
	}

	if (index <= 0 || index > 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "The dynamite id must be between 1-3.");
	}

	if (IsValidDynamicObject(pDynamiteObject[playerid][index - 1]))
	{
		DestroyDynamicObject(pDynamiteObject[playerid][index - 1]);
		DestroyDynamic3DTextLabel(pDynamiteLabel[playerid][index - 1]);

		new buf[150];
		format(buf, sizeof(buf), "You have destroyed your dynamite %i.", index);
		SendClientMessage(playerid, COLOR_GREEN, buf);
	}
	else
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The dynamite id isn't active.");
	}

	return 1;
}

CMD:det(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

    new index;
    if (sscanf(params, "i", index))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /det [1/2/3]");
	}

	if (index <= 0 || index > 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "The dynamite id must be between 1-3.");
	}

	if (IsValidDynamicObject(pDynamiteObject[playerid][index - 1]))
	{
		new Float:x, Float:y, Float:z;
		GetDynamicObjectPos(pDynamiteObject[playerid][index - 1], x, y, z);

		if (! IsPlayerInRangeOfPoint(playerid, 100.0, x, y, z))
		{
		    return SendClientMessage(playerid, COLOR_TOMATO, "You are not close enough to detonate the specified dynamite id (Range: 100m).");
		}

		CreateExplosion(x, y, z, 6, 5);
		CreateExplosion(x, y + 2, z, 6, 5);

		new buf[150];
		format(buf, sizeof(buf), "You have detonated your dynamite %i.", index);
		SendClientMessage(playerid, COLOR_GREEN, buf);

		format(buf, sizeof(buf), "You were killed by the dynamite placed by %s(%i).", ReturnPlayerName(playerid), playerid);

		new team = GetPlayerTeam(playerid);
		foreach (new i : Player)
		{
  			if (team != GetPlayerTeam(i) && IsPlayerInRangeOfPoint(i, 5.0, x, y, z))
		    {
      			SendClientMessage(i, COLOR_TOMATO, buf);
				NotifyPlayer(i, "You got ~r~Dynamited!", 5000);

				format(buf, sizeof(buf), "Your dynamite killed %s(%i), +$500.", ReturnPlayerName(i), i);
				SendClientMessage(playerid, COLOR_GREEN, buf);
				GivePlayerMoney(playerid, 500);

				SetPlayerHealth(i, 0.0);

				PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
				PlayerPlaySound(i, 1055, 0.0, 0.0, 0.0);

				pKiller[i][0] = playerid;
				pKiller[i][1] = 51;
    		}
		}

		DestroyDynamicObject(pDynamiteObject[playerid][index - 1]);
		DestroyDynamic3DTextLabel(pDynamiteLabel[playerid][index - 1]);
	}
	else
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The dynamite id isn't active.");
	}

	return 1;
}

CMD:ammo(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! pInventory[playerid][3])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You don't have a ammunation refiller with you.");
	}

	pInventory[playerid][3] = 0;

	new wname[35];
	new buf[150];
	new weapon ,ammo;
	for (new i; i < 13; i++)
	{
		GetPlayerWeaponData(playerid, i, weapon ,ammo);
		switch(weapon)
		{
	 		case 16..18,35,36,39:
			{
			 	SetPlayerAmmo(playerid, weapon, ammo + 2);

			 	GetWeaponName(weapon, wname, sizeof(wname));
			 	format(buf, sizeof(buf), "You recieved +2 ammo for %s.", wname);
			 	SendClientMessage(playerid, COLOR_GREEN, buf);
		   	}
		   	case 22..34,38,41,42,43:
		   	{
			   	SetPlayerAmmo(playerid, weapon, ammo + 100);

			 	GetWeaponName(weapon, wname, sizeof(wname));
			 	format(buf, sizeof(buf), "You recieved +100 ammo for %s.", wname);
			 	SendClientMessage(playerid, COLOR_GREEN, buf);
		   	}
		   	case 37:
		   	{
			   	SetPlayerAmmo(playerid, weapon, ammo + 200);

			 	GetWeaponName(weapon, wname, sizeof(wname));
			 	format(buf, sizeof(buf), "You recieved +200 ammo for %s.", wname);
			 	SendClientMessage(playerid, COLOR_GREEN, buf);
			}
		}
	}

	NotifyPlayer(playerid, "~g~Weapons refilled!", 3000);
	SendClientMessage(playerid, COLOR_GREEN, "Your weapons have been refiled.");

	return 1;
}

CMD:drug(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (! pInventory[playerid][4])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You don't have a drug capsule with you.");
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	new Float:oldx = x;
	new Float:oldy = y;

	GetXYInFrontOfPlayer(playerid, x, y, 2.0);

	CreatePickup(1241, 4, x, y, z, 0);

	SetPlayerPos(playerid, oldx, oldy, z);

	pInventory[playerid][4]--;

	SendClientMessage(playerid, COLOR_GREEN, "You have created a drug bundle, pick it up for craziness!");

	return 1;
}

CMD:camo(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! pInventory[playerid][5])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You don't have a camouflage with you.");
	}

	if (! IsPlayerAttachedObjectSlotUsed(playerid, 2))
	{
	    SetPlayerAttachedObject(playerid, 2, 647, 1, -0.218999, 0.0, 0.104, 0.0, 0.0, 0.0, 0.472999, 0.300999, 0.376);
	    SendClientMessage(playerid, COLOR_GREEN, "You have enabled your camouflage.");
	}
	else
	{
	    RemovePlayerAttachedObject(playerid, 2);
	    SendClientMessage(playerid, COLOR_RED, "You have disabled your camouflage.");
	}

	return 1;
}

CMD:spike(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (! pInventory[playerid][7])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You don't have a spikestrip with you.");
	}

	new Float:x, Float:y, Float:z, Float:a;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, a);

	for (new i; i < 3; i++)
	{
		if (! pSpikeTimer[playerid][i])
		{
			pSpikeObject[playerid][i] = SpikeStrip_Create(2892, x, y, z - 0.9, a);

			pSpikeTimer[playerid][i] = SetTimerEx("OnPlayerSpikeStripExpire", 3 * 60 * 1000, false, "ii", playerid, i);

			new buf[150];
			format(buf, sizeof(buf), "Spikestrip %i", i + 1);
			pSpikeLabel[playerid][i] = CreateDynamic3DTextLabel(buf, COLOR_WHITE, x, y, z, 100.0, .worldid = 0, .interiorid = GetPlayerInterior(playerid), .playerid = playerid);

			pInventory[playerid][7]--;

		    SendClientMessage(playerid, COLOR_GREEN, "You have placed a spikestrip at your ground; Vehicles passing over it will get their tyres poped.");
		    SendClientMessage(playerid, COLOR_GREEN, "The planted spike will auto destroy after 3 minutes from now. You can also destroy the spikestrip by '/destroyspike [id]'.");

			return 1;
		}
 	}

	SendClientMessage(playerid, COLOR_TOMATO, "You have 3 active spike strips, you should destroy one before planting a new spike strips (/destroyspike).");

	return 1;
}

forward OnPlayerSpikeStripExpire(playerid, i);
public	OnPlayerSpikeStripExpire(playerid, i)
{
    SpikeStrip_Delete(pSpikeObject[playerid][i]);
	KillTimer(pSpikeTimer[playerid][i]);
 	pSpikeTimer[playerid][i] = 0;
	DestroyDynamic3DTextLabel(pSpikeLabel[playerid][i]);
}

CMD:destroyspike(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

    new index;
    if (sscanf(params, "i", index))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /destroyspike [1/2/3]");
	}

	if (index <= 0 || index > 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "The spikestrip id must be between 1-3.");
	}

	if (pSpikeTimer[playerid][index - 1])
	{
		SpikeStrip_Delete(pSpikeObject[playerid][index - 1]);
		KillTimer(pSpikeTimer[playerid][index - 1]);
        pSpikeTimer[playerid][index - 1] = 0;
		DestroyDynamic3DTextLabel(pSpikeLabel[playerid][index - 1]);

		new buf[150];
		format(buf, sizeof(buf), "You have destroyed your spikestrip %i.", index);
		SendClientMessage(playerid, COLOR_GREEN, buf);
	}
	else
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The spikestrip id isn't active.");
	}

	return 1;
}

CMD:music(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (! pInventory[playerid][6])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You don't have a musicbox with you.");
	}

	if (IsValidDynamicObject(pMusicBoxObject[playerid]))
	{
		if (! IsPlayerInDynamicArea(playerid, pMusicBoxAreaid[playerid]))
		{
		    return SendClientMessage(playerid, COLOR_TOMATO, "You must be near your musicbox.");
		}

	    DestroyDynamicObject(pMusicBoxObject[playerid]);
	    foreach (new i : Player)
	    {
	        if (IsPlayerInDynamicArea(i, pMusicBoxAreaid[playerid]))
	        {
	            StopAudioStreamForPlayer(i);
	        }
	    }
	    DestroyDynamicArea(pMusicBoxAreaid[playerid]);
	    DestroyDynamic3DTextLabel(pMusicBoxLabel[playerid]);

	    SendClientMessage(playerid, COLOR_RED, "You have picked up your musicbox. (use /music again to drop it)");
	}
	else
	{
	    new Float:x, Float:y, Float:z, Float:a;
	    GetPlayerPos(playerid, x, y, z);
	    GetPlayerFacingAngle(playerid, a);

		GetXYInFrontOfPlayer(playerid, x, y, 2.0);

	    pMusicBoxObject[playerid] = CreateDynamicObject(2226, x, y, z - 1.0, 0, 0, a + 180.0, 0, GetPlayerInterior(playerid));

        pMusicBoxAreaid[playerid] = CreateDynamicSphere(x, y, z, 50.0, 0, GetPlayerInterior(playerid));

        new text[100];
        format(text, sizeof(text), "%s(%i)'s musicbox", ReturnPlayerName(playerid), playerid);
        pMusicBoxLabel[playerid] = CreateDynamic3DTextLabel(text, COLOR_ORANGE, x, y, z, 50.0, .worldid = 0, .interiorid = GetPlayerInterior(playerid));

        pMusicBoxURL[playerid][0] = EOS;
		ShowPlayerDialog(playerid, DIALOG_ID_MUSICBOX, DIALOG_STYLE_INPUT, "Music box streamer:", ""WHITE"Insert a "LIME"URL. "WHITE"to start streaming.\n\nPress "RED"RANDOM "WHITE"to stream random radio station.", "Stream", "Random");

	    SendClientMessage(playerid, COLOR_GREEN, "You have placed your musicbox. (use /music again to pick it up)");
	}

	return 1;
}

CMD:mine(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (! pInventory[playerid][8])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You don't have a landmine with you.");
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	for (new i; i < 3; i++)
	{
		if (! IsValidDynamicObject(pLandmineObject[playerid][i]))
		{
			pLandmineObject[playerid][i] = CreateDynamicObject(1213, x, y, z - 0.9, 0.0, 0.0, 0.0, 0, GetPlayerInterior(playerid));

			new buf[150];
			format(buf, sizeof(buf), "Landmine %i", i + 1);
			pLandmineLabel[playerid][i] = CreateDynamic3DTextLabel(buf, COLOR_WHITE, x, y, z, 100.0, .worldid = 0, .interiorid = GetPlayerInterior(playerid), .playerid = playerid);

			pLandmineAreaid[playerid][i] = CreateDynamicSphere(x, y, z, 3.0, 0, GetPlayerInterior(playerid));

			pInventory[playerid][8]--;

			SendClientMessage(playerid, COLOR_GREEN, "You have placed a landmine at your ground.");
			SendClientMessage(playerid, COLOR_GREEN, "You can also destroy the landmine by '/destroymine [id]'.");

			return 1;
		}
 	}

	SendClientMessage(playerid, COLOR_TOMATO, "You have 3 active landmines, you should destroy one before planting a new landmine (/destroymine).");

	return 1;
}

CMD:destroymine(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

    new index;
    if (sscanf(params, "i", index))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /destroymine [1/2/3]");
	}

	if (index <= 0 || index > 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "The landmine id must be between 1-3.");
	}

	if (IsValidDynamicObject(pLandmineObject[playerid][index - 1]))
	{
		DestroyDynamicObject(pLandmineObject[playerid][index - 1]);
		DestroyDynamicArea(pLandmineAreaid[playerid][index - 1]);
		DestroyDynamic3DTextLabel(pLandmineLabel[playerid][index - 1]);

		new buf[150];
		format(buf, sizeof(buf), "You have destroyed your landmine %i.", index);
		SendClientMessage(playerid, COLOR_GREEN, buf);
	}
	else
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "The landmine id isn't active.");
	}

	return 1;
}

CMD:dropgun(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	new w = GetPlayerWeapon(playerid);

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);
	
	CreateWeaponPickup(w, GetPlayerAmmo(playerid), x, y + random(4), z, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));

	new weapon[14];
	new ammo[14];
	
	for (new i; i < 14; i++)
	{
	    if (weapon[i] != w)
		{
			GetPlayerWeaponData(playerid, i, weapon[i], ammo[i]);
		}
	}
	
	ResetPlayerWeapons(playerid);
	
	for (new i; i < 14; i++)
	{
		GivePlayerWeapon(playerid, weapon[i], ammo[i]);
	}

	new weapon_name[35];
	GetWeaponName(w, weapon_name, sizeof(weapon_name));

	new buf[150];
	strcat(buf, "You dropped weapon ");
	strcat(buf, weapon_name);
	strcat(buf, ".");
	SendClientMessage(playerid, COLOR_YELLOW, buf);

	return 1;
}

CMD:jacket(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (! pInventory[playerid][9])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You don't have a Protection Jacket with you.");
	}

	if (! IsPlayerAttachedObjectSlotUsed(playerid, 2))
	{
	    SetPlayerAttachedObject(playerid, 2, 1242, 1, 0.127000, 0.028000, 0.000999, 0.000000, 86.700004, -6.899999, 1.553000, 1.676998, 1.467000);

	    SendClientMessage(playerid, COLOR_GREEN, "You have wear your Protection Jacket, this will drop the amount of damgae taken at TORSO and CHEST bodyparts.");
	}
	else
	{
	    RemovePlayerAttachedObject(playerid, 2);

		SendClientMessage(playerid, COLOR_RED, "You have removed your Protection Jacket; type /jacket again to wear it.");
	}

	return 1;
}

CMD:mask(playerid)
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (IsPlayerInAnyVehicle(playerid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be on foot to use this command.");
	}

	if (! pInventory[playerid][9])
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You don't have a Protection Mask with you.");
	}

	if (! IsPlayerAttachedObjectSlotUsed(playerid, 3))
	{
        SetPlayerAttachedObject(playerid, 3, 19036, 2, 0.111999, 0.022999, 0.000000, 88.999977, 99.100006, 2.900001, 1.070999, 1.104999, 1.026000);

	    SendClientMessage(playerid, COLOR_GREEN, "You have wear your Protection Mask, this will drop the amount of damgae taken at HEAD bodypart.");
	}
	else
	{
	    RemovePlayerAttachedObject(playerid, 3);

		SendClientMessage(playerid, COLOR_RED, "You have removed your Protection Mask; type /mask again to wear it.");
	}

	return 1;
}

CMD:inv(playerid)
{
	new buf[150];
	new info[1024];
	strcat(info, ""WHITE"Protection helmet: ");
	if (pHasHelmet[playerid])
	{
		strcat(info, ""GREEN"YES (1)\n");
	}
	else
	{
		strcat(info, ""RED"NO (0)\n");
	}

	strcat(info, ""WHITE"Gas Mask: ");
	if (pHasMask[playerid])
	{
		strcat(info, ""GREEN"YES (1)\n");
	}
	else
	{
		strcat(info, ""RED"NO (0)\n");
	}

	strcat(info, ""WHITE"Medickit: ");
	if (pInventory[playerid][0])
	{
		strcat(info, GREEN);
		strcat(info, "(");
		valstr(buf, pInventory[playerid][0]);
		strcat(info, buf);
		strcat(info, ")\n");
		strcat(info, ""GREY"Use /med to gain health.\n\n");
	}
	else
	{
		strcat(info, ""RED"NO (0)\n\n");
	}

	strcat(info, ""WHITE"Nettrap: ");
	if (pInventory[playerid][1])
	{
		strcat(info, GREEN);
		strcat(info, "(");
		valstr(buf, pInventory[playerid][1]);
		strcat(info, buf);
		strcat(info, ")\n");
		strcat(info, ""GREY"Use /trap to place a trap on your ground.\n\n");
	}
	else
	{
		strcat(info, ""RED"NO (0)\n\n");
	}

	strcat(info, ""WHITE"Dynamite: ");
	if (pInventory[playerid][2])
	{
		strcat(info, GREEN);
		strcat(info, "(");
		valstr(buf, pInventory[playerid][2]);
		strcat(info, buf);
		strcat(info, ")\n");
		strcat(info, ""GREY"Use /dynamite to plant a dynamite explosive on your ground.\n\n");
	}
	else
	{
		strcat(info, ""RED"NO (0)\n\n");
	}

	strcat(info, ""WHITE"Ammunation: ");
	if (pInventory[playerid][3])
	{
		strcat(info, ""GREEN"YES (1)\n");
		strcat(info, ""GREY"Use /ammo to refill all your weapons' ammo.\n\n");
	}
	else
	{
		strcat(info, ""RED"NO (0)\n\n");
	}

	strcat(info, ""WHITE"Drug Bundle: ");
	if (pInventory[playerid][4])
	{
		strcat(info, GREEN);
		strcat(info, "(");
		valstr(buf, pInventory[playerid][4]);
		strcat(info, buf);
		strcat(info, ")\n");
		strcat(info, ""GREY"Use /drug to place a drug capsule pickup.\n\n");
	}
	else
	{
		strcat(info, ""RED"NO (0)\n\n");
	}

	strcat(info, ""WHITE"Camouflage: ");
	if (pInventory[playerid][5])
	{
		strcat(info, ""GREEN"YES (1)\n");
		strcat(info, ""GREY"Use /camo to wear or unwear camoflage.\n\n");
	}
	else
	{
		strcat(info, ""RED"NO (0)\n\n");
	}

	strcat(info, ""WHITE"Musicbox: ");
	if (pInventory[playerid][6])
	{
		strcat(info, ""GREEN"YES (1)\n");
		strcat(info, ""GREY"Use /music to place a bombbox and stream music.\n\n");
	}
	else
	{
		strcat(info, ""RED"NO (0)\n\n");
	}

	strcat(info, ""WHITE"Spikestrip: ");
	if (pInventory[playerid][7])
	{
		strcat(info, GREEN);
		strcat(info, "(");
		valstr(buf, pInventory[playerid][7]);
		strcat(info, buf);
		strcat(info, ")\n");
		strcat(info, ""GREY"Use /spike to place a spike strip.\n\n");
	}
	else
	{
		strcat(info, ""RED"NO (0)\n\n");
	}

	strcat(info, ""WHITE"Landmine: ");
	if (pInventory[playerid][8])
	{
		strcat(info, GREEN);
		strcat(info, "(");
		valstr(buf, pInventory[playerid][8]);
		strcat(info, buf);
		strcat(info, ")\n");
		strcat(info, ""GREY"Use /mine to place a landmine.\n\n");
	}
	else
	{
		strcat(info, ""RED"NO (0)\n\n");
	}

	strcat(info, ""WHITE"Protection Jacket: ");
	if (pInventory[playerid][9])
	{
		strcat(info, ""GREEN"YES (1)\n");
		strcat(info, ""GREY"Use /jacket to wear or unwear a protection jacket.\n\n");
	}
	else
	{
		strcat(info, ""RED"NO (0)\n\n");
	}

	strcat(info, ""WHITE"Protection Mask: ");
	if (pInventory[playerid][10])
	{
		strcat(info, ""GREEN"YES (1)\n");
		strcat(info, ""GREY"Use /mask to wear or unwear a protection mask.\n\n");
	}
	else
	{
		strcat(info, ""RED"NO (0)\n\n");
	}

	ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Inventory items list:", info, "Close", "");
	return 1;
}

CMD:event(playerid)
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new info[1000];
	strcat(info, "Cops Vs. Terrorists (event_tdm0.amx)\n");
	strcat(info, "Grove Vs. Ballas (event_tdm1.amx)\n");
	strcat(info, "Aztecas Vs. Vagos (event_tdm2.amx)\n");
	strcat(info, "Bikers Vs. Truckers (event_tdm3.amx)\n");
	strcat(info, "Boys Vs. Girls (event_tdm4.amx)\n");
	strcat(info, "Cowboys Vs. Police (event_tdm5.amx)\n");
	strcat(info, "Pilots Vs. Passengers (event_tdm7.amx)\n");
	strcat(info, "Prisoners Vs. Guards (event_tdm8.amx)\n");
	strcat(info, "Guests Vs. Employees (event_tdm9.amx)\n");
	strcat(info, "Gungame 1 (event_gun0.amx)\n");
	strcat(info, "Gungame 2 (event_gun1.amx)\n");
	strcat(info, "Gungame 3 (event_gun2.amx)\n");
	strcat(info, "Sharpshooter : Sniper (event_shp0.amx)\n");
	strcat(info, "Sharpshooter : Desert Eagle (event_shp1.amx)\n");
	strcat(info, "Sharpshooter : Shotgun (event_shp2.amx)\n");
	strcat(info, "One In The Chamber : Silenced Pistol (event_oic0.amx)\n");
	strcat(info, "One In The Chamber : MP5 (event_oic1.amx)\n");
	strcat(info, "One In The Chamber : Desert Eagle (event_oic2.amx)\n");
	strcat(info, "One In The Chamber : Chainsaw (event_oic3.amx)\n");
	strcat(info, "One In The Chamber : Knife (event_oic4.amx)\n");
	strcat(info, "Last Of Us 1 (event_lou0.amx)\n");
	strcat(info, "Last Of Us 2 (event_lou1.amx)\n");
	strcat(info, "Last Of Us 3 (event_lou2.amx)");
	ShowPlayerDialog(playerid, DIALOG_ID_EVENT, DIALOG_STYLE_LIST, "Select an event to organize:", info, "Select", "Cancel");

	return 1;
}

CMD:healteam(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new teamid;
	if (sscanf(params, "i", teamid))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /healteam [teamid]");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	if (teamid < 0 || teamid >= MAX_TEAMS)
	{
		SendClientMessage(playerid, COLOR_FIREBRICK, "ERROR: Invalid team id, must be b/w 0-6.");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	foreach (new i : Player)
	{
	    if (! pInClass[i] && GetPlayerTeam(i) == teamid)
	    {
			SetPlayerHealth(i, 100.0);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	    }
	}

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new string[144];
	format(string, sizeof(string), "You have healed team %s [%i].", gTeam[teamid][teamName], teamid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, string);
	format(string, sizeof(string), "Admin %s(%i) have healed team %s [%i].", ReturnPlayerName(playerid), playerid, gTeam[teamid][teamName], teamid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, string);
	return 1;
}

CMD:armourteam(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new teamid;
	if (sscanf(params, "i", teamid))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /armourteam [teamid]");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	if (teamid < 0 || teamid >= MAX_TEAMS)
	{
		SendClientMessage(playerid, COLOR_FIREBRICK, "ERROR: Invalid team id, must be b/w 0-6.");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	foreach (new i : Player)
	{
	    if (! pInClass[i] && GetPlayerTeam(i) == teamid)
	    {
			SetPlayerArmour(i, 100.0);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	    }
	}

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new string[144];
	format(string, sizeof(string), "You have armoured team %s [%i].", gTeam[teamid][teamName], teamid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, string);
	format(string, sizeof(string), "Admin %s(%i) have armoured team %s [%i].", ReturnPlayerName(playerid), playerid, gTeam[teamid][teamName], teamid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, string);
	return 1;
}

CMD:disarmteam(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new teamid;
	if (sscanf(params, "i", teamid))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /disarmteam [teamid]");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	if (teamid < 0 || teamid >= MAX_TEAMS)
	{
		SendClientMessage(playerid, COLOR_FIREBRICK, "ERROR: Invalid team id, must be b/w 0-6.");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	foreach (new i : Player)
	{
	    if (! pInClass[i] && GetPlayerTeam(i) == teamid)
	    {
			ResetPlayerWeapons(i);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	    }
	}

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new string[144];
	format(string, sizeof(string), "You have reseted team %s [%i]'s weapons.", gTeam[teamid][teamName], teamid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, string);
	format(string, sizeof(string), "Admin %s(%i) have reseted team %s [%i]'s weapons.", gTeam[teamid][teamName], teamid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, string);
	return 1;
}

CMD:setteamhealth(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new teamid, amount;
	if (sscanf(params, "ii", teamid, amount))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setteamhealth [teamid] [amount]");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	if (teamid < 0 || teamid >= MAX_TEAMS)
	{
		SendClientMessage(playerid, COLOR_FIREBRICK, "ERROR: Invalid team id, must be b/w 0-6.");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	foreach (new i : Player)
	{
	    if (! pInClass[i] && GetPlayerTeam(i) == teamid)
	    {
			SetPlayerHealth(i, amount);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	    }
	}

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new string[144];
	format(string, sizeof(string), "You have set team %s [%i] health to %i.", gTeam[teamid][teamName], teamid, amount);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, string);
	format(string, sizeof(string), "Admin %s(%i) have team %s [%i] health to %i.", ReturnPlayerName(playerid), playerid, gTeam[teamid][teamName], teamid, amount);
	SendClientMessageToAll(COLOR_DODGER_BLUE, string);
	return 1;
}

CMD:setteamarmour(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new teamid, amount;
	if (sscanf(params, "ii", teamid, amount))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /setteamarmour [teamid] [amount]");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	if (teamid < 0 || teamid >= MAX_TEAMS)
	{
		SendClientMessage(playerid, COLOR_FIREBRICK, "ERROR: Invalid team id, must be b/w 0-6.");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	foreach (new i : Player)
	{
	    if (! pInClass[i] && GetPlayerTeam(i) == teamid)
	    {
			SetPlayerArmour(i, amount);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	    }
	}

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new string[144];
	format(string, sizeof(string), "You have set team %s [%i] armour to %i.", gTeam[teamid][teamName], teamid, amount);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, string);
	format(string, sizeof(string), "Admin %s(%i) have team %s [%i] armour to %i.", ReturnPlayerName(playerid), playerid, gTeam[teamid][teamName], teamid, amount);
	SendClientMessageToAll(COLOR_DODGER_BLUE, string);
	return 1;
}

CMD:freezeteam(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new teamid;
	if (sscanf(params, "i", teamid))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /freezeteam [teamid]");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	if (teamid < 0 || teamid >= MAX_TEAMS)
	{
		SendClientMessage(playerid, COLOR_FIREBRICK, "ERROR: Invalid team id, must be b/w 0-6.");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	foreach (new i : Player)
	{
	    if (i != playerid && GetPlayerTeam(i) == teamid)
	    {
			TogglePlayerControllable(i, false);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	    }
	}

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new string[144];
	format(string, sizeof(string), "You have freezed team %s [%i].", gTeam[teamid][teamName], teamid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, string);
	format(string, sizeof(string), "Admin %s(%i) have freezed team %s [%i].", ReturnPlayerName(playerid), playerid, gTeam[teamid][teamName], teamid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, string);
	return 1;
}

CMD:unfreezeteam(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new teamid;
	if (sscanf(params, "i", teamid))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /unfreezeteam [teamid]");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	if (teamid < 0 || teamid >= MAX_TEAMS)
	{
		SendClientMessage(playerid, COLOR_FIREBRICK, "ERROR: Invalid team id, must be b/w 0-6.");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	foreach (new i : Player)
	{
	    if (i != playerid && GetPlayerTeam(i) == teamid)
	    {
			TogglePlayerControllable(i, true);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	    }
	}

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new string[144];
	format(string, sizeof(string), "You have unfreezed team %s [%i].", gTeam[teamid][teamName], teamid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, string);
	format(string, sizeof(string), "Admin %s(%i) have unfreezed team %s [%i].", ReturnPlayerName(playerid), playerid, gTeam[teamid][teamName], teamid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, string);
	return 1;
}

CMD:getteam(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new teamid;
	if (sscanf(params, "i", teamid))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /getteam [teamid]");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	if (teamid < 0 || teamid >= MAX_TEAMS)
	{
		SendClientMessage(playerid, COLOR_FIREBRICK, "ERROR: Invalid team id, must be b/w 0-6.");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	foreach (new i : Player)
	{
	    if (! pInClass[i] && i != playerid && GetPlayerTeam(i) == teamid)
	    {
			SetPlayerPos(i, pos[0], pos[1], pos[2]);
			SetPlayerInterior(i, GetPlayerInterior(playerid));
			SetPlayerVirtualWorld(i, GetPlayerVirtualWorld(playerid));
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	    }
	}

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new string[144];
	format(string, sizeof(string), "You have teleported team %s [%i].", gTeam[teamid][teamName], teamid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, string);
	format(string, sizeof(string), "Admin %s(%i) have teleported team %s [%i].", ReturnPlayerName(playerid), playerid, gTeam[teamid][teamName], teamid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, string);
	return 1;
}

CMD:spawnteam(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new teamid;
	if (sscanf(params, "i", teamid))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /spawnteam [teamid]");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	if (teamid < 0 || teamid >= MAX_TEAMS)
	{
		SendClientMessage(playerid, COLOR_FIREBRICK, "ERROR: Invalid team id, must be b/w 0-6.");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	foreach (new i : Player)
	{
	    if (! pInClass[i] && i != playerid && GetPlayerTeam(i) == teamid)
	    {
			SpawnPlayer(i);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	    }
	}

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new string[144];
	format(string, sizeof(string), "You have spawned team %s [%i].", gTeam[teamid][teamName], teamid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, string);
	format(string, sizeof(string), "Admin %s(%i) have spawned team %s [%i].", ReturnPlayerName(playerid), playerid, gTeam[teamid][teamName], teamid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, string);
	return 1;
}

CMD:giveteamscore(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new teamid, amount;
	if (sscanf(params, "ii", teamid, amount))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /giveteamscore [teamid] [amount]");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	if (teamid < 0 || teamid >= MAX_TEAMS)
	{
		SendClientMessage(playerid, COLOR_FIREBRICK, "ERROR: Invalid team id, must be b/w 0-6.");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	foreach (new i : Player)
	{
	    if (GetPlayerTeam(i) == teamid)
	    {
			SetPlayerScore(i, GetPlayerScore(i) + amount);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	    }
	}

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new string[144];
	format(string, sizeof(string), "You have given %i score to team %s [%i].", amount, gTeam[teamid][teamName], teamid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, string);
	format(string, sizeof(string), "Admin %s(%i) have given %i score to team %s [%i].", ReturnPlayerName(playerid), playerid, amount, gTeam[teamid][teamName], teamid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, string);
	return 1;
}

CMD:giveteamcash(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new teamid, amount;
	if (sscanf(params, "ii", teamid, amount))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /giveteamcash [teamid] [amount]");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	if (teamid < 0 || teamid >= MAX_TEAMS)
	{
		SendClientMessage(playerid, COLOR_FIREBRICK, "ERROR: Invalid team id, must be b/w 0-6.");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	foreach (new i : Player)
	{
	    if (GetPlayerTeam(i) == teamid)
	    {
			GivePlayerMoney(i, amount);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	    }
	}

	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new string[144];
	format(string, sizeof(string), "You have given $%i cash to team %s [%i].", amount, gTeam[teamid][teamName], teamid);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, string);
	format(string, sizeof(string), "Admin %s(%i) have given $%i cash to team %s [%i].", ReturnPlayerName(playerid), playerid, amount, gTeam[teamid][teamName], teamid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, string);
	return 1;
}

CMD:giveteamweapon(playerid, params[])
{
	if (pInClass[playerid] || GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
	{
	    return 1;
	}

	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 4+ to use this command.");
	}

	new teamid, weapon[32], ammo;
	if (sscanf(params, "is[32]I(250)", teamid, weapon, ammo))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /giveteamweapon [team] [weapon] [*ammo]");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	if (teamid < 0 || teamid >= MAX_TEAMS)
	{
		SendClientMessage(playerid, COLOR_FIREBRICK, "ERROR: Invalid team id, must be b/w 0-6.");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	new weaponid;
	if (isnumeric(weapon))
	{
		weaponid = strval(weapon);
	}
	else
	{
		weaponid = GetWeaponIDFromName(weapon);
	}

	if (1 > weaponid > 46)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid weapon id/name.");
	}

	foreach (new i : Player)
	{
	    if (GetPlayerTeam(i) == teamid)
	    {
			GivePlayerWeapon(i, weaponid, ammo);
			PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
	    }
	}

	GetWeaponName(weaponid, weapon, sizeof(weapon));
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has given team %s [%i] a %s[id: %i] with %i ammo.", ReturnPlayerName(playerid), playerid, gTeam[teamid][teamName], teamid, weapon, weaponid, ammo);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	format(buf, sizeof(buf), "You have given team %s [%i] a %s[id: %i] with %i ammo.", gTeam[teamid][teamName], teamid, weapon, weaponid, ammo);
	SendClientMessage(playerid, COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:giveteamhelmet(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new teamid;
	if (sscanf(params, "i", teamid))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /giveteammask [teamid]");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	if (teamid < 0 || teamid >= MAX_TEAMS)
	{
		SendClientMessage(playerid, COLOR_FIREBRICK, "ERROR: Invalid team id, must be b/w 0-6.");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	foreach (new i : Player)
	{
        PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
        if (GetPlayerTeam(i) == teamid)
        {
            if (! pHasHelmet[i])
            {
				SetPlayerAttachedObject(i,0,18638,2,0.173000,0.024999,-0.003000,0.000000,0.000000,0.000000,1.000000,1.000000,1.000000); //skin 102
				pHasHelmet[i] = true;
			}
		}
	}

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has given team %s [%i] a Protection Helmet.", ReturnPlayerName(playerid), playerid, gTeam[teamid][teamName], teamid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:giveteammask(playerid, params[])
{
	if (! IsPlayerAdmin(playerid) && pStats[playerid][userAdmin] < 3)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "You must be admin level 3+ to use this command.");
	}

	new teamid;
	if (sscanf(params, "i", teamid))
	{
		SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /giveteammask [teamid]");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	if (teamid < 0 || teamid >= MAX_TEAMS)
	{
		SendClientMessage(playerid, COLOR_FIREBRICK, "ERROR: Invalid team id, must be b/w 0-6.");
		new buf[150];
		for (new i; i < MAX_TEAMS; i++)
		{
		    if (i == MAX_TEAMS - 1)
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s", buf, i, gTeam[i][teamName]);
		    }
		    else
		    {
		    	format(buf, sizeof(buf), "%s[%i]%s, ", buf, i, gTeam[i][teamName]);
		    }
		}
		return SendClientMessage(playerid, COLOR_THISTLE, buf);
	}

	foreach (new i : Player)
	{
        PlayerPlaySound(i, 1057, 0.0, 0.0, 0.0);
        if (GetPlayerTeam(i) == teamid)
        {
            if (! pHasMask[i])
            {
				SetPlayerAttachedObject(i, 1, 19472, 2, -0.022000, 0.137000, 0.018999, 3.899994, 85.999961, 92.999984, 0.923999, 1.141000, 1.026999);
	            pHasMask[i] = true;
            }
		}
	}

	new buf[150];
	format(buf, sizeof(buf), "Admin %s(%i) has given team %s [%i] a Gas Mask.", ReturnPlayerName(playerid), playerid, gTeam[teamid][teamName], teamid);
	SendClientMessageToAll(COLOR_DODGER_BLUE, buf);
	return 1;
}

CMD:dlabel(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! pStats[playerid][userPremium])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must be a premium user to use this command.");
	}

	new text[25];
	if (sscanf(params, "s[25]", text))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /dlabel [text <25 length>]");
	}

	if (! text[0])
	{
    	UpdateDynamic3DTextLabelText(pDonorLabel[playerid], COLOR_CYAN, text);
    }

    new buf[150];
    format(buf, sizeof(buf), "[VIP] You have updated your donor label to %s.", text);
    SendClientMessage(playerid, COLOR_CYAN, buf);
	return 1;
}

CMD:dskin(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! pStats[playerid][userPremium])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must be a premium user to use this command.");
	}

	new skinid;
	if (sscanf(params, "i", skinid))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /dskin [skinid]");
	}

	if (skinid < 0 || skinid > 311)
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "Invalid skinid, must be b/w 0-311.");
	}

	SetPlayerSkin(playerid, skinid);

    new buf[150];
    format(buf, sizeof(buf), "[VIP] You have changed your skin to %i.", skinid);
    SendClientMessage(playerid, COLOR_CYAN, buf);
	return 1;
}

CMD:dbike(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! pStats[playerid][userPremium])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must be a premium user to use this command.");
	}

	new Float:x, Float:y, Float:z, Float:a;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, a);

	DestroyVehicle(pStats[playerid][userVehicle]);
	pStats[playerid][userVehicle] = CreateVehicle(522, x, y, z, a, -1, -1, -1);
	LinkVehicleToInterior(pStats[playerid][userVehicle], GetPlayerInterior(playerid));
	SetVehicleVirtualWorld(pStats[playerid][userVehicle], GetPlayerVirtualWorld(playerid));

	PutPlayerInVehicle(playerid, pStats[playerid][userVehicle], 0);
	GameTextForPlayer(playerid, "~b~~h~~h~VIP Bike Spawned!", 3000, 3);
    SendClientMessage(playerid, COLOR_CYAN, "[VIP] You have spawned a premium bike.");
	return 1;
}

CMD:dcar(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! pStats[playerid][userPremium])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must be a premium user to use this command.");
	}

	new Float:x, Float:y, Float:z, Float:a;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, a);

	DestroyVehicle(pStats[playerid][userVehicle]);
	pStats[playerid][userVehicle] = CreateVehicle(411, x, y, z, a, -1, -1, -1);
	LinkVehicleToInterior(pStats[playerid][userVehicle], GetPlayerInterior(playerid));
	SetVehicleVirtualWorld(pStats[playerid][userVehicle], GetPlayerVirtualWorld(playerid));

	PutPlayerInVehicle(playerid, pStats[playerid][userVehicle], 0);
	GameTextForPlayer(playerid, "~b~~h~~h~VIP Car Spawned!", 3000, 3);
    SendClientMessage(playerid, COLOR_CYAN, "[VIP] You have spawned a premium car.");
	return 1;
}

CMD:dbmx(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! pStats[playerid][userPremium])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must be a premium user to use this command.");
	}

	new Float:x, Float:y, Float:z, Float:a;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, a);

	DestroyVehicle(pStats[playerid][userVehicle]);
	pStats[playerid][userVehicle] = CreateVehicle(411, x, y, z, a, -1, -1, -1);
	LinkVehicleToInterior(pStats[playerid][userVehicle], GetPlayerInterior(playerid));
	SetVehicleVirtualWorld(pStats[playerid][userVehicle], GetPlayerVirtualWorld(playerid));

	PutPlayerInVehicle(playerid, pStats[playerid][userVehicle], 0);
	GameTextForPlayer(playerid, "~b~~h~~h~VIP BMX Spawned!", 3000, 3);
    SendClientMessage(playerid, COLOR_CYAN, "[VIP] You have spawned a premium BMX bicycle.");
	return 1;
}

CMD:dplane(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! pStats[playerid][userPremium])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must be a premium user to use this command.");
	}

	new Float:x, Float:y, Float:z, Float:a;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, a);

	DestroyVehicle(pStats[playerid][userVehicle]);
	pStats[playerid][userVehicle] = CreateVehicle(513, x, y, z, a, -1, -1, -1);
	LinkVehicleToInterior(pStats[playerid][userVehicle], GetPlayerInterior(playerid));
	SetVehicleVirtualWorld(pStats[playerid][userVehicle], GetPlayerVirtualWorld(playerid));

	PutPlayerInVehicle(playerid, pStats[playerid][userVehicle], 0);
	GameTextForPlayer(playerid, "~b~~h~~h~VIP Plane Spawned!", 3000, 3);
    SendClientMessage(playerid, COLOR_CYAN, "[VIP] You have spawned a premium plane.");
	return 1;
}

CMD:dnos(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! pStats[playerid][userPremium])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must be a premium user to use this command.");
	}

	if (! IsPlayerInAnyVehicle(playerid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must be in a vehicle to use this command.");
	}

	new vehicle = GetPlayerVehicleID(playerid);
	switch (GetVehicleModel(vehicle))
	{
		case 448,461,462,463,468,471,509,510,521,522,523,581,586,449:
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "You cannot add nitros to this vehicle.");
		}
	}
	AddVehicleComponent(vehicle, 1010);

	GameTextForPlayer(playerid, "~b~~h~~h~VIP Nitros Added!", 3000, 3);
    SendClientMessage(playerid, COLOR_CYAN, "[VIP] You have added nitros (10x) into your car.");
	return 1;
}

CMD:dhyd(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! pStats[playerid][userPremium])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must be a premium user to use this command.");
	}

	if (! IsPlayerInAnyVehicle(playerid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must be in a vehicle to use this command.");
	}

	new vehicle = GetPlayerVehicleID(playerid);
	switch (GetVehicleModel(vehicle))
	{
		case 448,461,462,463,468,471,509,510,521,522,523,581,586,449:
		{
			return SendClientMessage(playerid, COLOR_TOMATO, "You cannot add hydraulics to this vehicle.");
		}
	}
	AddVehicleComponent(vehicle, 1087);

	GameTextForPlayer(playerid, "~b~~h~~h~VIP Hydraulics Added!", 3000, 3);
    SendClientMessage(playerid, COLOR_CYAN, "[VIP] You have added hydraulics into your car.");
	return 1;
}

CMD:dsupply(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! pStats[playerid][userPremium])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must be a premium user to use this command.");
	}

	if (pPremiumSupply[playerid])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You can only call a premium supply once a spawn.");
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(playerid, x, y, z);

	new buf[150];
	format(buf, sizeof(buf),"** Premium supply from %s[%d]!", ReturnPlayerName(playerid), playerid);

  	foreach (new i : Player)
	{
		if (IsPlayerInRangeOfPoint(i, 10.0, x, y, z) && HasSameTeam(playerid, i))
		{
			GivePlayerWeapon(i, 30, 100);
			GivePlayerWeapon(i, 24, 50);
			GivePlayerWeapon(i, 25, 30);
			GivePlayerWeapon(i, 35, 2);

			new Float:val;
			GetPlayerArmour(i, val);
			if (val < 100.0)
			{
				SetPlayerArmour(playerid, (100.0 - val >= 25.0) ? (val + 25.0) : (val + (100.0 - val)));
			}

			GetPlayerHealth(i, val);
			if (val < 100.0)
			{
				SetPlayerHealth(playerid, (100.0 - val >= 25.0) ? (val + 25.0) : (val + (100.0 - val)));
			}

			SendClientMessage(i, COLOR_GREY, buf);
			SendClientMessage(i, COLOR_GREY, "** Health (25%), Armour (25%), Ak-47, Desert Eagle, Shotgun, Rocket launcher.");
		}
	}

    pPremiumSupply[playerid] = true;
	GivePlayerMoney(playerid, -10000);

	SendClientMessage(playerid, COLOR_CYAN, "[VIP] You have just ordered your premium supply. The supply was distributed among you and your teammates (only in range of 10m).");
	SendClientMessage(playerid, COLOR_CYAN, "[VIP] Supply package cost -$10000");
	return 1;
}

CMD:dcolor(playerid, params[])
{
	if (pInClass[playerid])
	{
	    return 1;
	}

	if (! pStats[playerid][userPremium])
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must be a premium user to use this command.");
	}

	if (! IsPlayerInAnyVehicle(playerid))
	{
		return SendClientMessage(playerid, COLOR_TOMATO, "You must be in a vehicle to use this command.");
	}

	new color1, color2;
	if (sscanf(params, "iI(-1)", color1, color2))
	{
		return SendClientMessage(playerid, COLOR_THISTLE, "USAGE: /dcolor [color1] [*color2]");
	}

	ChangeVehicleColor(GetPlayerVehicleID(playerid), color1, color2);
	PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);


	new buf[150];
	format(buf, sizeof(buf), "[VIP] You have changed you vehicle's color to %i & %i.", color1, color2);
	SendClientMessage(playerid, COLOR_CYAN, buf);
	return 1;
}

public OnPlayerEnterDynamicArea(playerid, areaid)
{
    for (new i; i < MAX_DROPS; i++)
    {
        if (IsValidDynamicObject(gDropObject[i]))
        {
            if (areaid == gDropAreaid[i])
           	{
                new weapon_name[35];
                GetWeaponName(gDropWeaponid[i], weapon_name, sizeof(weapon_name));

                new buf[150];
                strcat(buf, "Press ~b~~k~~CONVERSATION_NO~ ~w~~h~to pickup ~b~");
                strcat(buf, weapon_name);
                NotifyPlayer(playerid, buf, 5000);

				break;
			}
	   	}
	}

	new team = GetPlayerTeam(playerid);
	foreach (new i : Player)
	{
	    if (areaid == pMusicBoxAreaid[i])
	    {
	        if (pMusicBoxURL[i][0])
	        {
		        new Float:x, Float:y, Float:z;
		        PlayAudioStreamForPlayer(playerid, pMusicBoxURL[i], x, y, z);

		        new buf[150];
		        format(buf, sizeof(buf), "[Music box] Streaming started, Hosted by %s(%i).", ReturnPlayerName(i), i);
				SendClientMessage(playerid, COLOR_ORANGE, buf);

				return 1;
			}
	    }

		if (i != playerid)
	    {
	        if (! pTrapped[playerid] && ! pStats[playerid][userOnDuty])
	        {
		        for (new t; t < 3; t++)
		        {
			        if (areaid == pLandmineAreaid[i][t])
			        {
			            if (GetPlayerTeam(i) != team)
			   			{
			                new buf[150];
							format(buf, sizeof(buf), "You have been exploded by the landmine placed by %s(%i).", ReturnPlayerName(i), i);
							SendClientMessage(playerid, COLOR_YELLOW, buf);

							format(buf, sizeof(buf), "Player %s(%i) has been exploded in your landmine %i.", ReturnPlayerName(playerid), playerid, t);
							SendClientMessage(i, COLOR_YELLOW, buf);

							new Float:x, Float:y, Float:z;
							GetDynamicObjectPos(pLandmineObject[i][t], x, y, z);
							CreateExplosion(x, y, z, 6, 5);
							CreateExplosion(x, y + 2, z, 6, 5);

							SetPlayerHealth(playerid, 0.0);

							pKiller[playerid][0] = i;
							pKiller[playerid][1] = 51;

							DestroyDynamicObject(pLandmineObject[i][t]);
							DestroyDynamicArea(pLandmineAreaid[i][t]);
							DestroyDynamic3DTextLabel(pLandmineLabel[i][t]);

							return 1;
			   			}
			        }
		        }

		        if (! IsPlayerInAnyVehicle(i))
		        {
			        for (new t; t < 2; t++)
			        {
				        if (areaid == pNetTrapArea[i][t])
				        {
				            if (GetPlayerTeam(i) != team)
				   			{
				                new buf[150];
								format(buf, sizeof(buf), "You have been traped in the net placed by %s(%i).", ReturnPlayerName(i), i);
								SendClientMessage(playerid, COLOR_YELLOW, buf);

								format(buf, sizeof(buf), "Player %s(%i) has been traped in your nettrap %i.", ReturnPlayerName(playerid), playerid, t);
								SendClientMessage(i, COLOR_YELLOW, buf);

								ApplyAnimation(playerid, "CRACK", "crckdeth2", 4.0, 1, 0, 0, 0, 0);

								pTrapped[playerid] = true;
								pTrappedTimer[playerid] = SetTimerEx("OnPlayerUnTrapped", 2 * 60 * 1000, false, "ii", playerid, i);

								new Float:x, Float:y, Float:z;
								GetPlayerPos(playerid, x, y, z);
								pTrappedObject[playerid] = CreateDynamicObject(2068, x, y, z - 1.5, 180.0, 0.0, 0.0, 0, GetPlayerInterior(playerid));

								DestroyDynamicObject(pNetTrapObject[i][t]);
								DestroyDynamicArea(pNetTrapArea[i][t]);
								DestroyDynamic3DTextLabel(pNetTrapLabel[i][t]);
								KillTimer(pNetTrapTimer[i][t]);

								return 1;
				   			}
				        }
					}
       			}
      		}
		}
	}

	return 1;
}

public OnPlayerLeaveDynamicArea(playerid, areaid)
{
    for (new i; i < MAX_DROPS; i++)
    {
        if (IsValidDynamicObject(gDropObject[i]))
        {
            if (areaid == gDropAreaid[i])
           	{
                NotifyPlayer(playerid, "_", 0);

				break;
			}
	   	}
	}

	foreach (new i : Player)
	{
	    if (areaid == pMusicBoxAreaid[i])
	    {
	        StopAudioStreamForPlayer(playerid);

			return 1;
	    }
	}

	return 1;
}

forward OnPlayerUnTrapped(playerid, traperid);
public  OnPlayerUnTrapped(playerid, traperid)
{
	ClearAnimations(playerid);
	DestroyDynamicObject(pTrappedObject[playerid]);
	pTrapped[playerid] = false;

    new buf[150];
	format(buf, sizeof(buf), "Player %s(%i) has broken the trap.", ReturnPlayerName(playerid), playerid);
	SendClientMessage(traperid, COLOR_YELLOW, buf);
	SendClientMessage(traperid, COLOR_YELLOW, "You have broken the trap.");
}

public OnPlayerCommandReceived(playerid, cmdtext[])
{
	if (pStats[playerid][userMuteTime] > 0)
	{
	    SendClientMessage(playerid, COLOR_TOMATO, "You can only perform commands when your unmuted.");
	    return 0;
	}
	else if (pStats[playerid][userJailTime] > 0)
	{
	    SendClientMessage(playerid, COLOR_TOMATO, "You can only perform commands when your unjailed.");
	    return 0;
	}
	else if (pMenu[playerid] != -1)
	{
	    return 0;
	}
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid)
{
	new params[5];
	valstr(params, clickedplayerid);
    cmd_stats(playerid, params);
	return 1;
}
