#if defined _spectate_included
	#endinput
#endif
#define _spectate_included

/*
 * Spectate.inc
 * Version: 2.0 (last updated on 24 July, 2016)
 * By Gammix
 *
 * Functions:
   native GetPlayerSpectateData(playerid, &spectatetype, &targetid, &mode);
   native UpdatePlayerSpectate(playerid, bool:advance = true);
 *
 */

static g_Players[MAX_PLAYERS];
static g_PlayersCount;

static g_Vehicles[MAX_VEHICLES];
static g_VehiclesCount;

#define SPECTATE_TYPE_NONE (0)
#define SPECTATE_TYPE_PLAYER (1)
#define SPECTATE_TYPE_VEHICLE (2)
#define SPECTATE_TYPE_FREE (3)

enum e_SPECTATE_DATA
{
	E_SPEC_DATA_ID,
	E_SPEC_DATA_TYPE,
	E_SPEC_DATA_MODE
};
static p_SpecData[MAX_PLAYERS][e_SPECTATE_DATA];
static p_SpecWorldid[MAX_PLAYERS];

stock Spec_CreateVehicle(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2, respawn_delay)
{
	new ret = CreateVehicle(modelid, spawn_x, spawn_y, spawn_z, z_angle, color1, color2, respawn_delay);
	if (ret != INVALID_VEHICLE_ID)
 		g_Vehicles[g_VehiclesCount++] = ret;
 	return ret;
}
#if defined _ALS_CreateVehicle
	#undef CreateVehicle
#else
	#define _ALS_CreateVehicle
#endif
#define CreateVehicle Spec_CreateVehicle

stock Spec_AddStaticVehicle(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2)
{
 	new ret = AddStaticVehicle(modelid, spawn_x, spawn_y, spawn_z, z_angle, color1, color2);
	if (ret != INVALID_VEHICLE_ID)
 		g_Vehicles[g_VehiclesCount++] = ret;
    return ret;
}
#if defined _ALS_AddStaticVehicle
	#undef AddStaticVehicle
#else
    #define _ALS_AddStaticVehicle
#endif
#define AddStaticVehicle Spec_AddStaticVehicle

stock Spec_AddStaticVehicleEx(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:z_angle, color1, color2, respawn_delay)
{
 	new ret = AddStaticVehicleEx(modelid, spawn_x, spawn_y, spawn_z, z_angle, color1, color2, respawn_delay);
	if (ret != INVALID_VEHICLE_ID)
 		g_Vehicles[g_VehiclesCount++] = ret;
   	return ret;
}
#if defined _ALS_AddStaticVehicleEx
 	#undef AddStaticVehicleEx
#else
    #define _ALS_AddStaticVehicleEx
#endif
#define AddStaticVehicleEx Spec_AddStaticVehicleEx

stock Spec_DestroyVehicle(vehicleid)
{
	new ret = DestroyVehicle(vehicleid);
	if (ret)
	{
		new bool:shift;
		for (new i; i < g_VehiclesCount; i++)
		{
		    if (!shift)
		    {
			    if (g_Vehicles[i] == vehicleid)
			    {
			        shift = true;
			        continue;
				}
			}
			else
			{
				g_Vehicles[i - 1] = g_Vehicles[i];
			}
		}
		g_VehiclesCount--;
	}
	return ret;
}
#if defined _ALS_DestroyVehicle
 	#undef DestroyVehicle
#else
 	#define _ALS_DestroyVehicle
#endif
#define DestroyVehicle Spec_DestroyVehicle

#if ! defined GetVehicleInterior
	static v_Interior[MAX_VEHICLES];

	stock Spec_LinkVehicleToInterior(vehicleid, interiorid)
	{
 		v_Interior[vehicleid] = interiorid;
 		return LinkVehicleToInterior(vehicleid, interiorid);
	}
	#if defined _ALS_LinkVehicleToInterior
 		#undef LinkVehicleToInterior
	#else
 		#define _ALS_LinkVehicleToInterior
	#endif
	#define LinkVehicleToInterior Spec_LinkVehicleToInterior

	stock GetVehicleInterior(vehicleid)
		return v_Interior[vehicleid];
#endif

#if ! defined IsValidVehicle
	native IsValidVehicle(vehicleid);
#endif

stock Spec_PlayerSpectatePlayer(playerid, targetplayerid, mode = SPECTATE_MODE_NORMAL)
{
	if (targetplayerid == playerid)
	    return 0;

	if (!IsPlayerConnected(playerid))
	    return 0;

	if (!IsPlayerConnected(targetplayerid))
	    return 0;
	    
	p_SpecData[playerid][E_SPEC_DATA_ID] = targetplayerid;
 	p_SpecData[playerid][E_SPEC_DATA_TYPE] = SPECTATE_TYPE_PLAYER;
  	p_SpecData[playerid][E_SPEC_DATA_MODE] = mode;

	SetPlayerInterior(playerid, GetPlayerInterior(targetplayerid));
	SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(targetplayerid));

	if (IsPlayerInAnyVehicle(targetplayerid))
		PlayerSpectateVehicle(playerid, GetPlayerVehicleID(targetplayerid), mode);
	else
	    PlayerSpectatePlayer(playerid, targetplayerid, mode);

	return 1;
}
#if defined _ALS_PlayerSpectatePlayer
    #undef PlayerSpectatePlayer
#else
    #define _ALS_PlayerSpectatePlayer
#endif
#define PlayerSpectatePlayer Spec_PlayerSpectatePlayer

stock Spec_PlayerSpectateVehicle(playerid, targetvehicleid, mode = SPECTATE_MODE_NORMAL)
{
	if (!IsPlayerConnected(playerid))
	    return 0;

	if (!IsValidVehicle(targetvehicleid))
	    return 0;
	    
	p_SpecData[playerid][E_SPEC_DATA_ID] = targetvehicleid;
 	p_SpecData[playerid][E_SPEC_DATA_TYPE] = SPECTATE_TYPE_VEHICLE;
 	p_SpecData[playerid][E_SPEC_DATA_MODE] = mode;

	SetPlayerInterior(playerid, GetVehicleInterior(targetvehicleid));
	SetPlayerVirtualWorld(playerid, GetVehicleVirtualWorld(targetvehicleid));

	return PlayerSpectateVehicle(playerid, targetvehicleid, mode);
}
#if defined _ALS_PlayerSpectateVehicle
    #undef PlayerSpectateVehicle
#else
    #define _ALS_PlayerSpectateVehicle
#endif
#define PlayerSpectateVehicle Spec_PlayerSpectateVehicle

stock Spec_TogglePlayerSpectating(playerid, bool:toggle)
{
	if (TogglePlayerSpectating(playerid, toggle))
	{
	    p_SpecData[playerid][E_SPEC_DATA_ID] = INVALID_PLAYER_ID;
	    p_SpecData[playerid][E_SPEC_DATA_TYPE] = SPECTATE_TYPE_FREE;
	    p_SpecData[playerid][E_SPEC_DATA_MODE] = 0;

		return 1;
	}
	return 0;
}
#if defined _ALS_TogglePlayerSpectating
    #undef TogglePlayerSpectating
#else
    #define _ALS_TogglePlayerSpectating
#endif
#define TogglePlayerSpectating Spec_TogglePlayerSpectating

stock GetPlayerSpectateData(playerid, &spectatetype, &targetid, &mode)
{
	if (playerid < 0 || player > MAX_PLAYERS)
		return 0;

    spectatetype = p_SpecData[playerid][E_SPEC_DATA_TYPE];
    targetid = p_SpecData[playerid][E_SPEC_DATA_ID];
    mode = p_SpecData[playerid][E_SPEC_DATA_MODE];
    
    return 1;
}

stock UpdatePlayerSpectate(playerid, bool:advance = true)
{
	if (playerid < 0 || playerid > MAX_PLAYERS)
		return -1;

	switch (p_SpecData[playerid][E_SPEC_DATA_TYPE])
	{
	    case SPECTATE_TYPE_PLAYER:
	    {
	        if (g_PlayersCount < 2)
	            return -1;
	    
			new targetid = -1;
			
			if (advance)
			{
   			SPEC_NEXT_PLAYER:
                for (new i = g_Players[0], j; j < g_PlayersCount; j++, i = g_Players[j])
                {
                    if (i == p_SpecData[playerid][E_SPEC_DATA_ID])
                    {
                        if (i >= (g_PlayersCount - 1))
                            goto SPEC_PERVIOUS_PLAYER;

                        targetid = g_Players[++j];
						break;
                	}
                }
                
                if (GetPlayerState(targetid) == PLAYER_STATE_SPECTATING || GetPlayerState(targetid) == PLAYER_STATE_WASTED)
                	goto SPEC_NEXT_PLAYER;
                else
                {
                    p_SpecData[playerid][E_SPEC_DATA_ID] = targetid;

					SetPlayerInterior(playerid, GetPlayerInterior(targetid));
					SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(targetid));

					if (IsPlayerInAnyVehicle(targetid))
						PlayerSpectateVehicle(playerid, GetPlayerVehicleID(targetid), p_SpecData[playerid][E_SPEC_DATA_MODE]);
					else
					    PlayerSpectatePlayer(playerid, targetid, p_SpecData[playerid][E_SPEC_DATA_MODE]);
                }
       			return targetid;
			}
			else
			{
			SPEC_PERVIOUS_PLAYER:
                for (new i = g_Players[0], j; j < g_PlayersCount; j++, i = g_Players[j])
                {
                    if (i == p_SpecData[playerid][E_SPEC_DATA_ID])
                    {
                        if (i <= 0)
                            goto SPEC_NEXT_PLAYER;

                        targetid = g_Players[--j];
						break;
                	}
                }

                if (GetPlayerState(targetid) == PLAYER_STATE_SPECTATING || GetPlayerState(targetid) == PLAYER_STATE_WASTED)
                	goto SPEC_NEXT_PLAYER;
                else
                {
                    p_SpecData[playerid][E_SPEC_DATA_ID] = targetid;

					SetPlayerInterior(playerid, GetPlayerInterior(targetid));
					SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(targetid));

					if (IsPlayerInAnyVehicle(targetid))
						PlayerSpectateVehicle(playerid, GetPlayerVehicleID(targetid), p_SpecData[playerid][E_SPEC_DATA_MODE]);
					else
					    PlayerSpectatePlayer(playerid, targetid, p_SpecData[playerid][E_SPEC_DATA_MODE]);
                }
       			return targetid;
			}
		}
		
		case SPECTATE_TYPE_VEHICLE:
	    {
	        if (g_VehiclesCount < 2)
	            return -1;

			new targetid = INVALID_PLAYER_ID;

			if (advance)
			{
   			SPEC_NEXT_VEHICLE:
                for (new i = g_Vehicles[0], j; j < g_VehiclesCount; j++, i = g_Vehicles[j])
                {
                    if (i == p_SpecData[playerid][E_SPEC_DATA_ID])
                    {
                        if (i >= (g_VehiclesCount - 1))
                            goto SPEC_PERVIOUS_VEHICLE;

                        targetid = g_Vehicles[++j];
						break;
                	}
                }

               	p_SpecData[playerid][E_SPEC_DATA_ID] = targetid;

				SetPlayerInterior(playerid, GetVehicleInterior(targetid));
				SetPlayerVirtualWorld(playerid, GetVehicleVirtualWorld(targetid));

				PlayerSpectateVehicle(playerid, targetid, p_SpecData[playerid][E_SPEC_DATA_MODE]);
       			return targetid;
			}
			else
			{
			SPEC_PERVIOUS_VEHICLE:
                for (new i = g_Vehicles[0], j; j < g_VehiclesCount; j++, i = g_Vehicles[j])
                {
                    if (i == p_SpecData[playerid][E_SPEC_DATA_ID])
                    {
                        if (i <= 0)
                            goto SPEC_NEXT_VEHICLE;

                        targetid = g_Vehicles[--j];
						break;
                	}
                }

                p_SpecData[playerid][E_SPEC_DATA_ID] = targetid;

				SetPlayerInterior(playerid, GetVehicleInterior(targetid));
				SetPlayerVirtualWorld(playerid, GetVehicleVirtualWorld(targetid));

				PlayerSpectateVehicle(playerid, targetid, p_SpecData[playerid][E_SPEC_DATA_MODE]);
       			return targetid;
			}
		}
	}

	return -1;
}

public OnVehicleDeath(vehicleid, killerid)
{
    new Float:pos[3];
	GetVehiclePos(vehicleid, pos[0], pos[1], pos[2]);
 	pos[2] += 5.0;

	for (new i = g_Players[0], j; j < g_PlayersCount; j++, i = g_Players[j])
	{
		if (p_SpecData[i][E_SPEC_DATA_TYPE] == SPECTATE_TYPE_VEHICLE && p_SpecData[i][E_SPEC_DATA_ID] == vehicleid)
		{
			SetPlayerCameraPos(i, pos[0], pos[1], pos[2]);
			SetPlayerCameraLookAt(i, pos[0], pos[1], pos[2]);
		}
 	}

	#if defined Spec_OnVehicleDeath
       	return Spec_OnVehicleDeath(vehicleid, killerid);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnVehicleDeath
    #undef OnVehicleDeath
#else
    #define _ALS_OnVehicleDeath
#endif
#define OnVehicleDeath Spec_OnVehicleDeath
#if defined Spec_OnVehicleDeath
    forward Spec_OnVehicleDeath(vehicleid, killerid);
#endif

public OnVehicleSpawn(vehicleid)
{
	for (new i = g_Players[0], j; j < g_PlayersCount; j++, i = g_Players[j])
	{
		if (p_SpecData[i][E_SPEC_DATA_TYPE] == SPECTATE_TYPE_VEHICLE && p_SpecData[i][E_SPEC_DATA_ID] == vehicleid)
			PlayerSpectateVehicle(i, vehicleid, p_SpecData[i][E_SPEC_DATA_MODE]);
 	}

	#if defined Spec_OnVehicleSpawn
       	return Spec_OnVehicleSpawn(vehicleid);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnVehicleSpawn
    #undef OnVehicleSpawn
#else
    #define _ALS_OnVehicleSpawn
#endif
#define OnVehicleSpawn Spec_OnVehicleSpawn
#if defined Spec_OnVehicleSpawn
    forward Spec_OnVehicleSpawn(vehicleid);
#endif

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (newstate == PLAYER_STATE_SPECTATING)
	{
		for (new i = g_Players[0], j; j < g_PlayersCount; j++, i = g_Players[j])
		{
			if (i != playerid && p_SpecData[i][E_SPEC_DATA_TYPE] == SPECTATE_TYPE_PLAYER && p_SpecData[i][E_SPEC_DATA_ID] == playerid)
	 			UpdatePlayerSpectate(i);
	  	}
  	}
	else if (((oldstate != PLAYER_STATE_DRIVER || oldstate != PLAYER_STATE_PASSENGER) && (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER)) ||
			((oldstate != PLAYER_STATE_ONFOOT || oldstate != PLAYER_STATE_SPAWNED) && (newstate == PLAYER_STATE_ONFOOT || newstate == PLAYER_STATE_SPAWNED)))
	{
		for (new i = g_Players[0], j; j < g_PlayersCount; j++, i = g_Players[j])
		{
			if (i != playerid && p_SpecData[i][E_SPEC_DATA_TYPE] == SPECTATE_TYPE_PLAYER && p_SpecData[i][E_SPEC_DATA_ID] == playerid)
	 			PlayerSpectatePlayer(i, playerid, p_SpecData[i][E_SPEC_DATA_MODE]);
	  	}
  	}

	#if defined Spec_OnPlayerStateChange
       	return Spec_OnPlayerStateChange(playerid, newstate, oldstate);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerStateChange
    #undef OnPlayerStateChange
#else
    #define _ALS_OnPlayerStateChange
#endif
#define OnPlayerStateChange Spec_OnPlayerStateChange
#if defined Spec_OnPlayerStateChange
    forward Spec_OnPlayerStateChange(playerid, newstate, oldstate);
#endif

public OnPlayerUpdate(playerid)
{
	new worldid = GetPlayerVirtualWorld(playerid);
	if (p_SpecWorldid[playerid] != worldid)
	{
 		p_SpecWorldid[playerid] = worldid;

	    for (new i = g_Players[0], j; j < g_PlayersCount; j++, i = g_Players[j])
		{
			if (i != playerid && p_SpecData[i][E_SPEC_DATA_TYPE] == SPECTATE_TYPE_PLAYER && p_SpecData[i][E_SPEC_DATA_ID] == playerid)
 				SetPlayerVirtualWorld(i, worldid);
	    }
	}

 	#if defined Spec_OnPlayerUpdate
 		return Spec_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate Spec_OnPlayerUpdate
#if defined Spec_OnPlayerUpdate
    forward Spec_OnPlayerUpdate(playerid);
#endif

public OnPlayerSpawn(playerid)
{
	for (new i = g_Players[0], j; j < g_PlayersCount; j++, i = g_Players[j])
	{
		if (i != playerid && p_SpecData[i][E_SPEC_DATA_TYPE] == SPECTATE_TYPE_PLAYER && p_SpecData[i][E_SPEC_DATA_ID] == playerid)
			PlayerSpectatePlayer(i, playerid, p_SpecData[i][E_SPEC_DATA_MODE]);
 	}

 	#if defined Spec_OnPlayerSpawn
 		return Spec_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
	#undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn Spec_OnPlayerSpawn
#if defined Spec_OnPlayerSpawn
    forward Spec_OnPlayerSpawn(playerid);
#endif

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	for (new i = g_Players[0], j; j < g_PlayersCount; j++, i = g_Players[j])
	{
		if (i != playerid && p_SpecData[i][E_SPEC_DATA_TYPE] == SPECTATE_TYPE_PLAYER && p_SpecData[i][E_SPEC_DATA_ID] == playerid)
 			SetPlayerInterior(i, newinteriorid);
  	}

	#if defined Spec_OnPlayerInteriorChange
	  	return Spec_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerInteriorChange
    #undef OnPlayerInteriorChange
#else
    #define _ALS_OnPlayerInteriorChange
#endif
#define OnPlayerInteriorChange Spec_OnPlayerInteriorChange
#if defined Spec_OnPlayerInteriorChange
    forward Spec_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
#endif

public OnPlayerDeath(playerid, killerid, reason)
{
	new Float:cam[3];
	GetPlayerCameraPos(playerid, cam[0], cam[1], cam[2]);

	new Float:fpos[3];
	GetPlayerPos(playerid, fpos[0], fpos[1], fpos[2]);

	if (! GetPlayerInterior(playerid))
	{
		cam[2] += 5.0;
 	}
 	else
 	{
 	    cam[2] += 0.5;
 	}

	for (new i = g_Players[0], j; j < g_PlayersCount; j++, i = g_Players[j])
	{
		if (i != playerid && p_SpecData[i][E_SPEC_DATA_TYPE] == SPECTATE_TYPE_PLAYER && p_SpecData[i][E_SPEC_DATA_ID] == playerid)
		{
			SetPlayerCameraPos(i, cam[0], cam[1], cam[2]);
			SetPlayerCameraLookAt(i, fpos[0], fpos[1], fpos[2]);
		}
 	}

	#if defined Spec_OnPlayerDeath
	  	return Spec_OnPlayerDeath(playerid, killerid, reason);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath Spec_OnPlayerDeath
#if defined Spec_OnPlayerDeath
    forward Spec_OnPlayerDeath(playerid, killerid, reason);
#endif

public OnPlayerConnect(playerid)
{
    g_Players[g_PlayersCount++] = playerid;
    
	#if defined Spec_OnPlayerConnect
	  	return Spec_OnPlayerConnect(playerid);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect Spec_OnPlayerConnect
#if defined Spec_OnPlayerConnect
    forward Spec_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
	new bool:shift;
	for (new i; i < g_PlayersCount; i++)
	{
	    if (!shift)
	    {
		    if (i == playerid)
		    {
		        shift = true;
		        continue;
			}
		}
		else
		{
			g_Players[i - 1] = g_Players[i];
		}
	}
	g_PlayersCount--;
    
	for (new i = g_Players[0], j; j < g_PlayersCount; j++, i = g_Players[j])
	{
		if (i != playerid && p_SpecData[i][E_SPEC_DATA_TYPE] == SPECTATE_TYPE_PLAYER && p_SpecData[i][E_SPEC_DATA_ID] == playerid)
			UpdatePlayerSpectate(i);
 	}

	#if defined Spec_OnPlayerDisconnect
	  	return Spec_OnPlayerDisconnect(playerid, reason);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect Spec_OnPlayerDisconnect
#if defined Spec_OnPlayerDisconnect
    forward Spec_OnPlayerDisconnect(playerid, reason);
#endif
