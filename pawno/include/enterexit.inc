#if defined enterexit_included
	#endinput
#endif
#define enterexit_included

/*
 * enterexit.inc
 * Version: 2.0 (last updated on 21 August, 2016)
 * By Gammix
 *
 * Functions:
   native CreateEnterExit(Float:x, Float:y, Float:z, interiorid, worldid, Float:enterx, Float:entery, Float:enterz, Float:entera, enterinteriorid, enterworldid, modelid = 1559, Float:streamdistance = 200.0, Float:drawdistance = 0.0);
   native IsValidEnterExit(id);
   native DestoryEnterExit(id);
 *
 * Callbacks:
   forward OnPlayerEnterExit(playerid, enterexitid);
 *
 */

#include <streamer>

#if !defined MAX_ENTER_EXITS
	#define MAX_ENTER_EXITS (100)
#endif

enum ENX_e_DATA
{
	bool:   ENX_e_DATA_VALID,
	        ENX_e_DATA_OBJECTID,
	        ENX_e_DATA_AREAID,
 	Float:	ENX_e_DATA_ENTER_POS[4],
	        ENX_e_DATA_ENTER_INTERIOR,
	        ENX_e_DATA_ENTER_WORLD,
	bool:   ENX_e_DATA_POSITION_DOWN
};
static ENX_g_Data[MAX_ENTER_EXITS][ENX_e_DATA];

enum ENX_e_PLAYER_FADER
{
    PlayerText:	ENX_e_PLAYER_FADER_TD,
                ENX_e_PLAYER_FADER_ALPHA,
                ENX_e_PLAYER_FADER_TIMERID
};
static ENX_p_Fader[MAX_PLAYERS][ENX_e_PLAYER_FADER];

stock CreateEnterExit(Float:x, Float:y, Float:z, interiorid, worldid, Float:enterx, Float:entery, Float:enterz, Float:entera, enterinteriorid, enterworldid, modelid = 1559, Float:streamdistance = 200.0, Float:drawdistance = 0.0)
{
	new id = -1;
	for (new i; i < MAX_ENTER_EXITS; i++)
	{
    	if (!ENX_g_Data[i][ENX_e_DATA_VALID])
		{
		    id = i;
		    break;
		}
	}
	
	if (id == -1)
	    return id;

    ENX_g_Data[id][ENX_e_DATA_VALID] = true;
    ENX_g_Data[id][ENX_e_DATA_OBJECTID] = CreateDynamicObject(modelid, x, y, z, 0.0, 0.0, 0.0, worldid, interiorid, -1, streamdistance, drawdistance);
   	ENX_g_Data[id][ENX_e_DATA_AREAID] = CreateDynamicCircle(x, y, 1.0, worldid, interiorid);
   	ENX_g_Data[id][ENX_e_DATA_ENTER_POS][0] = enterx;
   	ENX_g_Data[id][ENX_e_DATA_ENTER_POS][1] = entery;
   	ENX_g_Data[id][ENX_e_DATA_ENTER_POS][2] = enterz;
   	ENX_g_Data[id][ENX_e_DATA_ENTER_POS][3] = entera;
   	ENX_g_Data[id][ENX_e_DATA_ENTER_INTERIOR] = enterinteriorid;
   	ENX_g_Data[id][ENX_e_DATA_ENTER_WORLD] = enterworldid;
    ENX_g_Data[id][ENX_e_DATA_POSITION_DOWN] = false;

	MoveDynamicObject(ENX_g_Data[id][ENX_e_DATA_OBJECTID], x, y, (z+1.0), 1.0);
	return id;
}

stock IsValidEnterExit(id)
{
	if (id < 0 || id >= MAX_ENTER_EXITS)
		return 0;
		
	return ENX_g_Data[id][ENX_e_DATA_VALID];
}

stock DestroyEnterExit(id)
{
	if (id < 0 || id >= MAX_ENTER_EXITS)
		return 0;
		
	if (!ENX_g_Data[id][ENX_e_DATA_VALID])
		return 0;
		
	ENX_g_Data[id][ENX_e_DATA_VALID] = false;
    DestroyDynamicObject(ENX_g_Data[id][ENX_e_DATA_OBJECTID]);
   	DestroyDynamicArea(ENX_g_Data[id][ENX_e_DATA_AREAID]);
   	ENX_g_Data[id][ENX_e_DATA_ENTER_POS][0] = 0.0;
   	ENX_g_Data[id][ENX_e_DATA_ENTER_POS][1] = 0.0;
   	ENX_g_Data[id][ENX_e_DATA_ENTER_POS][2] = 0.0;
   	ENX_g_Data[id][ENX_e_DATA_ENTER_POS][3] = 0.0;
   	ENX_g_Data[id][ENX_e_DATA_ENTER_INTERIOR] = 0;
   	ENX_g_Data[id][ENX_e_DATA_ENTER_WORLD] = 0;
    ENX_g_Data[id][ENX_e_DATA_POSITION_DOWN] = true;
	return 1;
}

public OnPlayerEnterDynamicArea(playerid, areaid)
{
	for (new i; i < MAX_ENTER_EXITS; i++)
	{
    	if (ENX_g_Data[i][ENX_e_DATA_VALID])
		{
		    if (areaid == ENX_g_Data[i][ENX_e_DATA_AREAID])
			{
			    #if defined OnPlayerEnterExit
			    	if (OnPlayerEnterExit(playerid, i) <= 0)
			    	    return 1;
				#endif

		 		TogglePlayerControllable(playerid, false);

            	ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TD] = CreatePlayerTextDraw(playerid, 0.000000, 1.000000, "blankscreen");
				PlayerTextDrawBackgroundColor(playerid, ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TD], 255);
				PlayerTextDrawLetterSize(playerid, ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TD], 0.000000, 51.099998);
				PlayerTextDrawColor(playerid, ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TD], -1);
				PlayerTextDrawUseBox(playerid, ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TD], 1);
				PlayerTextDrawBoxColor(playerid, ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TD], 0);
				PlayerTextDrawTextSize(playerid, ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TD], 640.000000, 0.000000);
				PlayerTextDrawShow(playerid, ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TD]);
				
				ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_ALPHA] = 0;
				ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TIMERID] = SetTimerEx("ENX_OnFadeUpdate", 100, true, "iii", playerid, i, 0);

				return 1;
			}
		}
	}
	
 	#if defined ENX_OnPlayerPickUpDynamicP
		ENX_OnPlayerPickUpDynamicP(playerid, pickupid);
	#endif
	return 1;
}
#if defined _ALS_OnPlayerPickUpDynamicP
	#undef OnPlayerPickUpDynamicPickup
#else
	#define _ALS_OnPlayerPickUpDynamicP
#endif
#define OnPlayerPickUpDynamicPickup ENX_OnPlayerPickUpDynamicP
#if defined ENX_OnPlayerPickUpDynamicP
	forward ENX_OnPlayerPickUpDynamicP(playerid, pickupid);
#endif

forward ENX_OnFadeUpdate(playerid, id, type);
public 	ENX_OnFadeUpdate(playerid, id, type)
{
	if (type == 0)
	{
	    ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_ALPHA] += 10;
		PlayerTextDrawBoxColor(playerid, ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TD], ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_ALPHA]);
		PlayerTextDrawShow(playerid, ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TD]);
		
		if (ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_ALPHA] >= 255)
		{
		    KillTimer(ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TIMERID]);
			ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TIMERID] = SetTimerEx("ENX_OnFadeUpdate", 100, true, "ii", playerid, 1);

		    SetPlayerPos(playerid, ENX_g_Data[id][ENX_e_DATA_ENTER_POS][0], ENX_g_Data[id][ENX_e_DATA_ENTER_POS][1], ENX_g_Data[id][ENX_e_DATA_ENTER_POS][2]);
			SetPlayerFacingAngle(playerid, ENX_g_Data[id][ENX_e_DATA_ENTER_POS][3]);
			SetPlayerInterior(playerid, ENX_g_Data[id][ENX_e_DATA_ENTER_INTERIOR]);
			SetPlayerVirtualWorld(playerid, ENX_g_Data[id][ENX_e_DATA_ENTER_WORLD]);
		}
	}
	else
	{
	    ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_ALPHA] -= 10;
		PlayerTextDrawBoxColor(playerid, ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TD], ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_ALPHA]);
		PlayerTextDrawShow(playerid, ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TD]);

		if (ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_ALPHA] <= 0)
		{
			PlayerTextDrawDestroy(playerid, ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TD]);
		    KillTimer(ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TIMERID]);
		}
	}
}

public OnPlayerDisconnect(playerid, reason)
{
	KillTimer(ENX_p_Fader[playerid][ENX_e_PLAYER_FADER_TIMERID]);

	#if defined ENX_OnPlayerDisconnect
		ENX_OnPlayerDisconnect(playerid, reason);
	#endif
	return 1;
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect ENX_OnPlayerDisconnect
#if defined ENX_OnPlayerDisconnect
	forward ENX_OnPlayerDisconnect(playerid, reason);
#endif

public OnDynamicObjectMoved(objectid)
{
	for (new i; i < MAX_ENTER_EXITS; i++)
	{
    	if (ENX_g_Data[i][ENX_e_DATA_VALID])
		{
		    if (objectid == ENX_g_Data[i][ENX_e_DATA_OBJECTID])
			{
		        if (ENX_g_Data[i][ENX_e_DATA_POSITION_DOWN])
		        {
		            ENX_g_Data[i][ENX_e_DATA_POSITION_DOWN] = false;
		            
					new Float:x, Float:y, Float:z;
					Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_X, x);
					Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_Y, y);
					Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_Z, z);
					return MoveDynamicObject(objectid, x, y, (z+1.0), 1.0);
		        }
		        else
		        {
		            ENX_g_Data[i][ENX_e_DATA_POSITION_DOWN] = true;

					new Float:x, Float:y, Float:z;
					Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_X, x);
					Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_Y, y);
					Streamer_GetFloatData(STREAMER_TYPE_OBJECT, objectid, E_STREAMER_Z, z);
					return MoveDynamicObject(objectid, x, y, z, 1.0);
				}
		    }
		}
	}
	
	#if defined ENX_OnDynamicObjectMoved
		ENX_OnDynamicObjectMoved(objectid);
	#endif
	return 1;
}
#if defined _ALS_OnDynamicObjectMoved
	#undef OnDynamicObjectMoved
#else
	#define _ALS_OnDynamicObjectMoved
#endif
#define OnDynamicObjectMoved ENX_OnDynamicObjectMoved
#if defined ENX_OnDynamicObjectMoved
	forward ENX_OnDynamicObjectMoved(objectid);
#endif

#if defined OnPlayerEnterExit
	forward OnPlayerEnterExit(playerid, enterexitid);
#endif
