#if defined ss@vending_machines_included
	#endinput
#endif
#define ss@vending_machines_included

/*
CALLBACKS:
forward OnPlayerUseVendingMachine(playerid, machineid);
forward OnPlayerUsedVendingMachine(playerid, machineid);
*/

/*
FUNCTIONS:
native CreateVendingMachine(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, price = 1);
native DestroyVendingMachine(machineid);
native IsValidVendingMachine(machineid);
native CountTotalVendingMachines();
native GetVendingMachineId(modelid, Float:fX, Float:fY, Float:fZ, Float:fRadius);
*/

#tryinclude <streamer>
#include "script_init"

forward OnPlayerUseVendingMachine(playerid, machineid);
forward OnPlayerUsedVendingMachine(playerid, machineid);

#if !defined MAX_VENDING_MACHINES
	#define MAX_VENDING_MACHINES 128
#endif

enum
{
	MACHINE_SNACK = 956,
	MACHINE_SNACK2 = 1776,
	MACHINE_SPRUNK = 955,
	MACHINE_SPRUNK2	= 1775,
	MACHINE_SODA = 1302,
	MACHINE_SODA2 = 1209,
	MACHINE_BLACK = 1977
};

enum
{
	Float:RADIUS_SODA = 1.2,
	Float:RADIUS_SPRUNK = 1.05,
	Float:RADIUS_SNACK = 1.05
};

enum e_DEFAULT_MACHINE
{
	e_DEFAULT_MACHINE_MODEL,
	e_DEFAULT_MACHINE_INTERIOR,
	Float:e_DEFAULT_MACHINE_X,
	Float:e_DEFAULT_MACHINE_Y,
	Float:e_DEFAULT_MACHINE_Z,
	Float:e_DEFAULT_MACHINE_RX,
	Float:e_DEFAULT_MACHINE_RY,
	Float:e_DEFAULT_MACHINE_RZ,
	Float:e_DEFAULT_MACHINE_FX,
	Float:e_DEFAULT_MACHINE_FY
};
static const Float:eDefaultMachine[][e_DEFAULT_MACHINE] =
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

enum e_NEW_MACHINE
{
	e_NEW_MACHINE_OBJECTID,
	Float:e_NEW_MACHINE_RADIUS,
	Float:e_NEW_MACHINE_X,
	Float:e_NEW_MACHINE_Y,
	Float:e_NEW_MACHINE_Z,
	Float:e_NEW_MACHINE_RX,
	Float:e_NEW_MACHINE_RY,
	Float:e_NEW_MACHINE_RZ,
	e_NEW_MACHINE_TYPE,
	e_NEW_MACHINE_PRICE,
	e_NEW_MACHINE_PLAYERID
}
static eNewMachine[MAX_VENDING_MACHINES][e_NEW_MACHINE];
static iTotalVendingMachines;

static iPlayerLastMachineId[MAX_PLAYERS] = 0;
static iPlayerLastMachineTimer[MAX_PLAYERS] = 0;

stock CreateVendingMachine(modelid, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, price = 1)
{
	if (iTotalVendingMachines == MAX_VENDING_MACHINES)
	{
		print("[vending_machine.inc] - ERROR: Limit of vending machines exceeded! Increase the limit or reduce the created machines.");
		return -1;
	}
	
	new machineid = iTotalVendingMachines++;

	#if defined STREAMER_TYPE_OBJECT
		eNewMachine[machineid][e_NEW_MACHINE_OBJECTID] = CreateDynamicObject(modelid, x, y, z, rx, ry, rz);
	#else
		eNewMachine[machineid][e_NEW_MACHINE_OBJECTID] = CreateObject(modelid, x, y, z, rx, ry, rz, 300.0);
	#endif
	
	switch(modelid)
	{
		case MACHINE_SPRUNK2,
			 MACHINE_SPRUNK,
			 MACHINE_BLACK:
		{
			eNewMachine[machineid][e_NEW_MACHINE_RADIUS] = RADIUS_SPRUNK;
		}

		case MACHINE_SNACK,
			 MACHINE_SNACK2:
		{
			eNewMachine[machineid][e_NEW_MACHINE_RADIUS] = RADIUS_SNACK;
		}

		case MACHINE_SODA,
			 MACHINE_SODA2:
		{
			eNewMachine[machineid][e_NEW_MACHINE_RADIUS] = RADIUS_SODA;
		}

		default:
		{
			print("[vending_machine.inc] - ERROR: Invalid vending machine modelid! (Used ID: %i | Valid IDs: 956, 1776, 955, 1775, 1302, 1209 and 1977.");
			return -1;
		}
	}
	
	eNewMachine[machineid][e_NEW_MACHINE_X] = x;
	eNewMachine[machineid][e_NEW_MACHINE_Y] = y;
	eNewMachine[machineid][e_NEW_MACHINE_Z] = z;

	eNewMachine[machineid][e_NEW_MACHINE_RX] = rx;
	eNewMachine[machineid][e_NEW_MACHINE_RY] = ry;
	eNewMachine[machineid][e_NEW_MACHINE_RZ] = rz;

	eNewMachine[machineid][e_NEW_MACHINE_TYPE] = modelid;
	eNewMachine[machineid][e_NEW_MACHINE_PRICE] = price;
	
	eNewMachine[machineid][e_NEW_MACHINE_PLAYERID] = INVALID_PLAYER_ID;
	return machineid;
}

stock DestroyVendingMachine(machineid)
{
	if (!IsValidVendingMachine(machineid))
		return 0;

	#if defined STREAMER_TYPE_OBJECT
		DestroyDynamicObject(eNewMachine[machineid][e_NEW_MACHINE_OBJECTID]);
	#else
		DestroyObject(eNewMachine[machineid][e_NEW_MACHINE_OBJECTID]);
	#endif
	
	eNewMachine[machineid][e_NEW_MACHINE_OBJECTID] = INVALID_OBJECT_ID;
	
	if (machineid == (iTotalVendingMachines - 1))
	    return 1;
	    
	eNewMachine[machineid][e_NEW_MACHINE_OBJECTID] = eNewMachine[(iTotalVendingMachines - 1)][e_NEW_MACHINE_OBJECTID];
    eNewMachine[machineid][e_NEW_MACHINE_RADIUS] = eNewMachine[(iTotalVendingMachines - 1)][e_NEW_MACHINE_RADIUS];

	eNewMachine[machineid][e_NEW_MACHINE_X] = eNewMachine[(iTotalVendingMachines - 1)][e_NEW_MACHINE_X];
	eNewMachine[machineid][e_NEW_MACHINE_Y] = eNewMachine[(iTotalVendingMachines - 1)][e_NEW_MACHINE_Y];
	eNewMachine[machineid][e_NEW_MACHINE_Z] = eNewMachine[(iTotalVendingMachines - 1)][e_NEW_MACHINE_Z];

	eNewMachine[machineid][e_NEW_MACHINE_RX] = eNewMachine[(iTotalVendingMachines - 1)][e_NEW_MACHINE_RX];
	eNewMachine[machineid][e_NEW_MACHINE_RY] = eNewMachine[(iTotalVendingMachines - 1)][e_NEW_MACHINE_RY];
	eNewMachine[machineid][e_NEW_MACHINE_RZ] = eNewMachine[(iTotalVendingMachines - 1)][e_NEW_MACHINE_RZ];

	eNewMachine[machineid][e_NEW_MACHINE_TYPE] = eNewMachine[(iTotalVendingMachines - 1)][e_NEW_MACHINE_TYPE];
	eNewMachine[machineid][e_NEW_MACHINE_PRICE] = eNewMachine[(iTotalVendingMachines - 1)][e_NEW_MACHINE_PRICE];
	
	eNewMachine[machineid][e_NEW_MACHINE_PLAYERID] = eNewMachine[(iTotalVendingMachines - 1)][e_NEW_MACHINE_PLAYERID];
	
	iTotalVendingMachines--;
	return 1;
}

stock IsValidVendingMachine(machineid)
{
	if (machineid < 0 || machineid >= iTotalVendingMachines)
	    return 0;
	return 1;
}

stock CountTotalVendingMachines()
{
	return iTotalVendingMachines;
}

static GetXYInFrontOfVending(machineid, &Float:x, &Float:y, Float:distance)
{
	eNewMachine[machineid][e_NEW_MACHINE_Z] += 180.0;
	if(eNewMachine[machineid][e_NEW_MACHINE_Z] > 359.0)
	{
		eNewMachine[machineid][e_NEW_MACHINE_Z] -= 359.0;
	}
	
	eNewMachine[machineid][e_NEW_MACHINE_X] += (distance * floatsin(-eNewMachine[machineid][e_NEW_MACHINE_Z], degrees));
	eNewMachine[machineid][e_NEW_MACHINE_Y] += (distance * floatcos(-eNewMachine[machineid][e_NEW_MACHINE_Z], degrees));
}

public OnPlayerConnect(playerid)
{
	if (IS_GAMEMODE)
	{
		for (new i = 0; i < sizeof(eDefaultMachine); i++)
		{
			RemoveBuildingForPlayer(playerid, eDefaultMachine[i][e_DEFAULT_MACHINE_MODEL], eDefaultMachine[i][e_DEFAULT_MACHINE_X], eDefaultMachine[i][e_DEFAULT_MACHINE_Y], eDefaultMachine[i][e_DEFAULT_MACHINE_Z], 0.25);
		}
	}	

	#if defined VM_OnPlayerConnect
		return VM_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect VM_OnPlayerConnect
#if defined VM_OnPlayerConnect
	forward VM_OnPlayerConnect(playerid);
#endif

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if ((newkeys & KEY_SECONDARY_ATTACK) && (GetPlayerAnimationIndex(playerid) != 1660))
	{
		new Float:x,
			Float:y,
		 	Float:z;
		for (new i = 0; i < iTotalVendingMachines; i++)
		{
			GetXYInFrontOfVending(i, x, y, 0.5);
			if (!IsPlayerInRangeOfPoint(playerid, eNewMachine[i][e_NEW_MACHINE_RADIUS], x, y, eNewMachine[i][e_NEW_MACHINE_Z]))
				continue;
			
			new bool:ret = bool:CallRemoteFunction("OnPlayerUseVendingMachine", "ii", playerid, i);
			if (ret == false)
			{
			    PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
			    break;
			}
			
			if (GetPlayerMoney(playerid) < eNewMachine[i][e_NEW_MACHINE_PRICE])
			{
			    PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
			    break;
			}
			
			ClearAnimations(playerid);
			GetPlayerPos(playerid, z, z, z);
			SetPlayerPos(playerid, x, y, z);
			SetPlayerFacingAngle(playerid, eNewMachine[i][e_NEW_MACHINE_RZ]);
			ApplyAnimation(playerid, "VENDING", "VEND_USE", 10.0, 0, 0, 0, 0, 0, 1);

			if(eNewMachine[i][e_NEW_MACHINE_TYPE] == MACHINE_SNACK)
			{
				PlayerPlaySound(playerid, 42601, 0.0, 0.0, 0.0);
			}
			else
			{
				PlayerPlaySound(playerid, 42600, 0.0, 0.0, 0.0);
			}

            eNewMachine[i][e_NEW_MACHINE_PLAYERID] = playerid;

            iPlayerLastMachineId[playerid] = i;
			iPlayerLastMachineTimer[playerid] = SetTimerEx("OnPlayerUsedVendingMachine", 2500, false, "i", playerid);
		}
	}

	#if defined VM_OnPlayerKeyStateChange
		return VM_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif

#define OnPlayerKeyStateChange VM_OnPlayerKeyStateChange
#if defined VM_OnPlayerKeyStateChange
	forward VM_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

public OnPlayerUsedVendingMachine(playerid)
{
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_DRINK_SPRUNK);
 	iPlayerLastMachineId[playerid] = -1;
	iPlayerLastMachineTimer[playerid] = -1;

	#if defined VM_OnPlayerUsedVendingMachine
		return VM_OnPlayerUsedVendingMachine(playerid, machineid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerUsedVendingMachine
	#undef OnPlayerUsedVendingMachine
#else
	#define _ALS_OnPlayerUsedVendingMachine
#endif

#define OnPlayerUsedVendingMachine VM_OnPlayerUsedVendingMachine
#if defined VM_OnPlayerUsedVendingMachine
	forward VM_OnPlayerUsedVendingMachine(playerid, machineid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
	if (iPlayerLastMachineId[playerid] != -1)
	{
		eNewMachine[iPlayerLastMachineId[playerid]][e_NEW_MACHINE_PLAYERID] = INVALID_PLAYER_ID;
		iPlayerLastMachineId[playerid] = -1;
		
		KillTimer(iPlayerLastMachineTimer[playerid]);
		iPlayerLastMachineTimer[playerid] = -1;
	}
	
	#if defined VM_OnPlayerDisconnect
		return VM_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect VM_OnPlayerDisconnect
#if defined VM_OnPlayerDisconnect
	forward VM_OnPlayerDisconnect(playerid, reason);
#endif

public OnScriptInit()
{
	if (IS_GAMEMODE)
	{
		for (new i = 0; i < sizeof(eDefaultMachine); i++)
		{
			CreateVendingMachine(eDefaultMachine[i][e_DEFAULT_MACHINE_MODEL], eDefaultMachine[i][e_DEFAULT_MACHINE_X], eDefaultMachine[i][e_DEFAULT_MACHINE_Y], eDefaultMachine[i][e_DEFAULT_MACHINE_Z], eDefaultMachine[i][e_DEFAULT_MACHINE_RX], eDefaultMachine[i][e_DEFAULT_MACHINE_RY], eDefaultMachine[i][e_DEFAULT_MACHINE_RZ]);
		}
	}
	
	#if defined VM_OnScriptInit
		return VM_OnScriptInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnScriptInit
	#undef OnScriptInit
#else
	#define _ALS_OnScriptInit
#endif
#define OnScriptInit VM_OnScriptInit
#if defined VM_OnScriptInit
	forward VM_OnScriptInit();
#endif

public OnScriptExit()
{
	if (IS_GAMEMODE)
	{
		for (new i = 0; i < iTotalVendingMachines; i++)
		{
			DestroyVendingMachine(i);
		}
	}
	
	#if defined VM_OnScriptExit
		return VM_OnScriptExit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnScriptExit
	#undef OnScriptExit
#else
	#define _ALS_OnScriptExit
#endif
#define OnScriptExit VM_OnScriptExit
#if defined VM_OnScriptExit
	forward VM_OnScriptExit();
#endif
