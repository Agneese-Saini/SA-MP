#if defined gangzones_included
	#endinput
#endif
#define gangzones_included

/*
 * gangzones.inc
 * Version: 2.0 (last updated on 21 August, 2016)
 * By Gammix 
 *
 * Functions:
   native GangZoneCreate(Float:minx, Float:miny, Float:maxx, Float:maxy, color = -1, Float:bordersize = 1.0, bordercolor = 0x00000060, interior = -1, virtualworld = -1);
   native GangZoneDestory(zone);
   native GangZoneExist(zone);
   native GangZoneShowForPlayer(playerid, zone, color = -1, bordercolor = -1);
   native GangZoneHideForPlayer(playerid, zone);
   native GangZoneShowForAll(zone, color = -1, bordercolor = -1);
   native GangZoneHideForAll(zone);
   native GangZoneFlashForPlayer(playerid, zone, flashcolor = -1, flashbordercolor = -1);
   native GangZoneStopFlashForPlayer(playerid, zone);
   native GangZoneFlashForAll(zone, flashcolor = -1, flashbordercolor = -1);
   native GangZoneStopFlashForAll(zone);
   native GangZoneSetInterior(zone, interior);
   native GangZoneGetInterior(zone);
   native GangZoneSetVirtualWorld(zone, virtualworld);
   native GangZoneGetVirtualWorld(zone);
   native CountAllGangZones();
   native DestroyAllGangZones();
   native ShowAllGangZonesForPlayer(playerid, color = -1, bordercolor = -1);
   native ShowAllGangZonesForAll(color = -1, bordercolor = -1);
   native HideAllGangZonesForPlayer(playerid);
   native HideAllGangZonesForAll();
   native IsPlayerInAnyGangZone(playerid);
   native GetPlayerGangZone(playerid);
 *
 * Callbacks:
   forward OnPlayerEnterGangZone(playerid, zone);
   forward OnPlayerLeaveGangZone(playerid, zone);
 *
 */

#include <streamer>

#if defined MAX_GANGZONES
	#undef MAX_GANGZONES
#endif
#define MAX_GANGZONES (204)

enum GZ_e_DATA
{
    bool:	GZ_e_DATA_VALID,
            GZ_e_DATA_ZONEID,
            GZ_e_DATA_BORDERID[4],
            GZ_e_DATA_AREAID
};
static GZ_g_Data[MAX_GANGZONES][GZ_e_DATA];

static bool:GZ_p_Shown[MAX_PLAYERS][MAX_GANGZONES];
static GZ_p_ShownColor[MAX_PLAYERS][MAX_GANGZONES];
static GZ_p_ShownBorderColor[MAX_PLAYERS][MAX_GANGZONES];

static bool:GZ_p_Flashing[MAX_PLAYERS][MAX_GANGZONES];
static GZ_p_FlashColor[MAX_PLAYERS][MAX_GANGZONES];
static GZ_p_FlashBorderColor[MAX_PLAYERS][MAX_GANGZONES];

static GZ_p_Worldid[MAX_PLAYERS];

stock GZ_GangZoneCreate(Float:minx, Float:miny, Float:maxx, Float:maxy, color = -1, Float:bordersize = 1.0, bordercolor = 0x00000060, interior = -1, virtualworld = -1)
{
	new zone = -1;
	for (new i; i < MAX_GANGZONES; i++)
	{
	    if (!GZ_g_Data[i][GZ_e_DATA_VALID])
	    {
	        zone = i;
	        break;
		}
	}

	if (zone == -1)
	    return -1;

	new Float:SEPERATION = (2.0*bordersize);

    GZ_g_Data[zone][GZ_e_DATA_VALID] = true;
	GZ_g_Data[zone][GZ_e_DATA_ZONEID] = GangZoneCreate(minx, miny, maxx, maxy);
	GZ_g_Data[zone][GZ_e_DATA_BORDERID][0] = GangZoneCreate((minx - SEPERATION), miny, (minx + SEPERATION), maxy);
	GZ_g_Data[zone][GZ_e_DATA_BORDERID][1] = GangZoneCreate((minx - SEPERATION), (maxy - SEPERATION), maxx, (maxy + SEPERATION));
	GZ_g_Data[zone][GZ_e_DATA_BORDERID][2] = GangZoneCreate((maxx - SEPERATION), miny, (maxx + SEPERATION), maxy);
	GZ_g_Data[zone][GZ_e_DATA_BORDERID][3] = GangZoneCreate((minx - SEPERATION), (miny - SEPERATION), maxx, (miny + SEPERATION));
	GZ_g_Data[zone][GZ_e_DATA_AREAID] = CreateDynamicRectangle(minx, miny, maxx, maxy, virtualworld, interior, -1);

	for (new i; i < MAX_PLAYERS; i++)
 	{
        GZ_p_Shown[i][zone] = false;
 	    GZ_p_ShownColor[i][zone] = color;
        GZ_p_ShownBorderColor[i][zone] = bordercolor;

		GZ_p_Flashing[i][zone] = false;
		GZ_p_FlashColor[i][zone] = -1;
		GZ_p_FlashBorderColor[i][zone] = -1;
	}

	return zone;
}
#if defined _ALS_GangZoneCreate
    #undef GangZoneCreate
#else
    #define _ALS_GangZoneCreate
#endif
#define GangZoneCreate GZ_GangZoneCreate

stock GZ_GangZoneDestroy(zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_g_Data[zone][GZ_e_DATA_VALID])
	  	return 0;

    GZ_g_Data[zone][GZ_e_DATA_VALID] = false;
    GangZoneDestroy(GZ_g_Data[zone][GZ_e_DATA_ZONEID]);
    GangZoneDestroy(GZ_g_Data[zone][GZ_e_DATA_BORDERID][0]);
    GangZoneDestroy(GZ_g_Data[zone][GZ_e_DATA_BORDERID][1]);
    GangZoneDestroy(GZ_g_Data[zone][GZ_e_DATA_BORDERID][2]);
    GangZoneDestroy(GZ_g_Data[zone][GZ_e_DATA_BORDERID][3]);
	DestroyDynamicArea(GZ_g_Data[zone][GZ_e_DATA_AREAID]);

	for (new i; i < MAX_PLAYERS; i++)
 	{
        GZ_p_Shown[i][zone] = false;
 	    GZ_p_ShownColor[i][zone] = -1;
        GZ_p_ShownBorderColor[i][zone] = -1;

		GZ_p_Flashing[i][zone] = false;
		GZ_p_FlashColor[i][zone] = -1;
		GZ_p_FlashBorderColor[i][zone] = -1;
	}

  	return 1;
}
#if defined _ALS_GangZoneDestroy
    #undef GangZoneDestroy
#else
    #define _ALS_GangZoneDestroy
#endif
#define GangZoneDestroy GZ_GangZoneDestroy

stock GangZoneExist(zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

	return GZ_g_Data[zone][GZ_e_DATA_VALID];
}

stock GZ_GangZoneShowForPlayer(playerid, zone, color = -1, bordercolor = -1)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_g_Data[zone][GZ_e_DATA_VALID])
	  	return 0;

  	if (color != -1)
	  	GZ_p_ShownColor[playerid][zone] = color;

  	if (bordercolor != -1)
	  	GZ_p_ShownBorderColor[playerid][zone] = bordercolor;

	GangZoneShowForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_ZONEID], GZ_p_ShownColor[playerid][zone]);
	GangZoneShowForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][0], GZ_p_ShownBorderColor[playerid][zone]);
	GangZoneShowForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][1], GZ_p_ShownBorderColor[playerid][zone]);
	GangZoneShowForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][2], GZ_p_ShownBorderColor[playerid][zone]);
	GangZoneShowForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][3], GZ_p_ShownBorderColor[playerid][zone]);

	GZ_p_Shown[playerid][zone] = true;
	return 1;
}
#if defined _ALS_GangZoneShowForPlayer
    #undef GangZoneShowForPlayer
#else
    #define _ALS_GangZoneShowForPlayer
#endif
#define GangZoneShowForPlayer GZ_GangZoneShowForPlayer

stock GZ_GangZoneHideForPlayer(playerid, zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_g_Data[zone][GZ_e_DATA_VALID])
	  	return 0;

	GangZoneHideForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_ZONEID]);
	GangZoneHideForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][0]);
	GangZoneHideForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][1]);
	GangZoneHideForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][2]);
	GangZoneHideForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][3]);

	GZ_p_Shown[playerid][zone] = false;
	return 1;
}
#if defined _ALS_GangZoneHideForPlayer
    #undef GangZoneHideForPlayer
#else
    #define _ALS_GangZoneHideForPlayer
#endif
#define GangZoneHideForPlayer GZ_GangZoneHideForPlayer

stock GZ_GangZoneShowForAll(zone, color = -1, bordercolor = -1)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_g_Data[zone][GZ_e_DATA_VALID])
	  	return 0;

 	for (new i; i < MAX_PLAYERS; i++)
 	{
	  	if (color != -1)
		  	GZ_p_ShownColor[i][zone] = color;

	  	if (bordercolor != -1)
		  	GZ_p_ShownBorderColor[i][zone] = bordercolor;

		GangZoneShowForPlayer(i, GZ_g_Data[zone][GZ_e_DATA_ZONEID], GZ_p_ShownColor[i][zone]);
		GangZoneShowForPlayer(i, GZ_g_Data[zone][GZ_e_DATA_BORDERID][0], GZ_p_ShownBorderColor[i][zone]);
		GangZoneShowForPlayer(i, GZ_g_Data[zone][GZ_e_DATA_BORDERID][1], GZ_p_ShownBorderColor[i][zone]);
		GangZoneShowForPlayer(i, GZ_g_Data[zone][GZ_e_DATA_BORDERID][2], GZ_p_ShownBorderColor[i][zone]);
		GangZoneShowForPlayer(i, GZ_g_Data[zone][GZ_e_DATA_BORDERID][3], GZ_p_ShownBorderColor[i][zone]);

		GZ_p_Shown[i][zone] = true;
	}
	return 1;
}
#if defined _ALS_GangZoneShowForAll
    #undef GangZoneShowForAll
#else
    #define _ALS_GangZoneShowForAll
#endif
#define GangZoneShowForAll GZ_GangZoneShowForAll

stock GZ_GangZoneHideForAll(zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_g_Data[zone][GZ_e_DATA_VALID])
	  	return 0;

	GangZoneHideForAll(GZ_g_Data[zone][GZ_e_DATA_ZONEID]);
	GangZoneHideForAll(GZ_g_Data[zone][GZ_e_DATA_BORDERID][0]);
	GangZoneHideForAll(GZ_g_Data[zone][GZ_e_DATA_BORDERID][1]);
	GangZoneHideForAll(GZ_g_Data[zone][GZ_e_DATA_BORDERID][2]);
	GangZoneHideForAll(GZ_g_Data[zone][GZ_e_DATA_BORDERID][3]);

 	for (new i; i < MAX_PLAYERS; i++)
 		GZ_p_Shown[i][zone] = false;
	return 1;
}
#if defined _ALS_GangZoneHideForAll
    #undef GangZoneHideForAll
#else
    #define _ALS_GangZoneHideForAll
#endif
#define GangZoneHideForAll GZ_GangZoneHideForAll

stock GZ_GangZoneFlashForPlayer(playerid, zone, flashcolor = -1, flashbordercolor = -1)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_g_Data[zone][GZ_e_DATA_VALID])
	  	return 0;

  	if (flashcolor != -1)
  	{
	  	GZ_p_FlashColor[playerid][zone] = flashcolor;
		GangZoneFlashForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_ZONEID], GZ_p_FlashColor[playerid][zone]);
	}

  	if (flashbordercolor != -1)
	{
	  	GZ_p_FlashBorderColor[playerid][zone] = flashbordercolor;
		GangZoneShowForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][0], GZ_p_FlashBorderColor[playerid][zone]);
		GangZoneShowForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][1], GZ_p_FlashBorderColor[playerid][zone]);
		GangZoneShowForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][2], GZ_p_FlashBorderColor[playerid][zone]);
		GangZoneShowForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][3], GZ_p_FlashBorderColor[playerid][zone]);
	}

	GZ_p_Flashing[playerid][zone] = true;
	return 1;
}
#if defined _ALS_GangZoneFlashForPlayer
    #undef GangZoneFlashForPlayer
#else
    #define _ALS_GangZoneFlashForPlayer
#endif
#define GangZoneFlashForPlayer GZ_GangZoneFlashForPlayer

stock GZ_GangZoneStopFlashForPlayer(playerid, zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_g_Data[zone][GZ_e_DATA_VALID])
	  	return 0;

	GangZoneStopFlashForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_ZONEID]);
	GangZoneStopFlashForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][0]);
	GangZoneStopFlashForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][1]);
	GangZoneStopFlashForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][2]);
	GangZoneStopFlashForPlayer(playerid, GZ_g_Data[zone][GZ_e_DATA_BORDERID][3]);

	GZ_p_Flashing[playerid][zone] = false;
	return 1;
}
#if defined _ALS_GangZoneStopFlashForPlayer
    #undef GangZoneStopFlashForPlayer
#else
    #define _ALS_GangZoneStopFlashForPlayer
#endif
#define GangZoneStopFlashForPlayer GZ_GangZoneStopFlashForPlayer

stock GZ_GangZoneFlashForAll(zone, flashcolor = -1, flashbordercolor = -1)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_g_Data[zone][GZ_e_DATA_VALID])
	  	return 0;

	if (flashcolor != -1)
		GangZoneFlashForAll(GZ_g_Data[zone][GZ_e_DATA_ZONEID], flashcolor);

	if (flashbordercolor != -1)
	{
		GangZoneFlashForAll(GZ_g_Data[zone][GZ_e_DATA_BORDERID][0], flashbordercolor);
		GangZoneFlashForAll(GZ_g_Data[zone][GZ_e_DATA_BORDERID][1], flashbordercolor);
		GangZoneFlashForAll(GZ_g_Data[zone][GZ_e_DATA_BORDERID][2], flashbordercolor);
		GangZoneFlashForAll(GZ_g_Data[zone][GZ_e_DATA_BORDERID][3], flashbordercolor);
	}

	for (new i; i < MAX_PLAYERS; i++)
 	{
		GZ_p_Flashing[i][zone] = true;
	  	GZ_p_FlashColor[i][zone] = flashcolor;
		GZ_p_FlashBorderColor[i][zone] = flashbordercolor;
	}
	return 1;
}
#if defined _ALS_GangZoneFlashForAll
    #undef GangZoneFlashForAll
#else
    #define _ALS_GangZoneFlashForAll
#endif
#define GangZoneFlashForAll GZ_GangZoneFlashForAll

stock GZ_GangZoneStopFlashForAll(zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_g_Data[zone][GZ_e_DATA_VALID])
	  	return 0;

	GangZoneStopFlashForAll(GZ_g_Data[zone][GZ_e_DATA_ZONEID]);
	GangZoneStopFlashForAll(GZ_g_Data[zone][GZ_e_DATA_BORDERID][0]);
	GangZoneStopFlashForAll(GZ_g_Data[zone][GZ_e_DATA_BORDERID][1]);
	GangZoneStopFlashForAll(GZ_g_Data[zone][GZ_e_DATA_BORDERID][2]);
	GangZoneStopFlashForAll(GZ_g_Data[zone][GZ_e_DATA_BORDERID][3]);

	for (new i; i < MAX_PLAYERS; i++)
 		GZ_p_Flashing[i][zone] = false;
	return 1;
}
#if defined _ALS_GangZoneStopFlashForAll
    #undef GangZoneStopFlashForAll
#else
    #define _ALS_GangZoneStopFlashForAll
#endif
#define GangZoneStopFlashForAll GZ_GangZoneStopFlashForAll

stock GangZoneSetInterior(zone, interiorid)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_g_Data[zone][GZ_e_DATA_VALID])
	  	return 0;

    Streamer_SetIntData(STREAMER_TYPE_AREA, GZ_g_Data[zone][GZ_e_DATA_AREAID], E_STREAMER_INTERIOR_ID, interiorid);
	return 1;
}

stock GangZoneGetInterior(zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_g_Data[zone][GZ_e_DATA_VALID])
	  	return 0;

	return Streamer_GetIntData(STREAMER_TYPE_AREA, GZ_g_Data[zone][GZ_e_DATA_AREAID], E_STREAMER_INTERIOR_ID);
}

stock GangZoneSetVituralWorld(zone, virtualworldid)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_g_Data[zone][GZ_e_DATA_VALID])
	  	return 0;

    Streamer_SetIntData(STREAMER_TYPE_AREA, GZ_g_Data[zone][GZ_e_DATA_AREAID], E_STREAMER_WORLD_ID, virtualworldid);
    return 1;
}

stock GangZoneGetVirtualWorld(zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_g_Data[zone][GZ_e_DATA_VALID])
	  	return 0;

	return Streamer_GetIntData(STREAMER_TYPE_AREA, GZ_g_Data[zone][GZ_e_DATA_AREAID], E_STREAMER_WORLD_ID);
}

stock CountAllGangZones()
{
	new count = 0;
    for (new i; i < MAX_GANGZONES; i++)
	{
	    if (GZ_g_Data[i][GZ_e_DATA_VALID])
	    	count++;
	}
	return count;
}

stock DestroyAllGangZones()
{
    for (new i; i < MAX_GANGZONES; i++)
		GangZoneDestroy(i);
	return 1;
}

stock ShowAllGangZonesForPlayer(playerid, color = -1, bordercolor = -1)
{
	for (new i; i < MAX_GANGZONES; i++)
	{
	    if (GZ_g_Data[i][GZ_e_DATA_VALID])
	    	GangZoneShowForPlayer(playerid, zone, color, bordercolor);
	}
	return 1;
}

stock HideAllGangZonesForPlayer(playerid)
{
	for (new i; i < MAX_GANGZONES; i++)
	{
	    if (GZ_g_Data[i][GZ_e_DATA_VALID])
	    	GangZoneHideForPlayer(playerid, zone);
	}
	return 1;
}

stock ShowAllGangZonesForAll(color = -1, bordercolor = -1)
{
	for (new i; i < MAX_GANGZONES; i++)
	{
	    if (GZ_g_Data[i][GZ_e_DATA_VALID])
			GangZoneShowForAll(zone, color, bordercolor);
	}
	return 1;
}

stock HideAllGangZonesForAll()
{
	for (new i; i < MAX_GANGZONES; i++)
	{
	    if (GZ_g_Data[i][GZ_e_DATA_VALID])
			GangZoneHideForAll(zone);
	}
	return 1;
}

stock IsPlayerInAnyGangZone(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	    return 0;

	for (new i; i < MAX_GANGZONES; i++)
	{
	    if (GZ_g_Data[i][GZ_e_DATA_VALID])
	    {
	        if (IsPlayerInDynamicArea(playerid, GZ_g_Data[i][GZ_e_DATA_AREAID]))
	            return 1;
		}
	}
	return 0;
}

stock GetPlayerGangZone(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	    return 0;

	for (new i; i < MAX_GANGZONES; i++)
	{
	    if (GZ_g_Data[i][GZ_e_DATA_VALID])
	    {
	        if (IsPlayerInDynamicArea(playerid, GZ_g_Data[i][GZ_e_DATA_AREAID]))
	            return i;
		}
	}
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
    for (new i; i < MAX_GANGZONES; i++)
	{
        if (GZ_g_Data[i][GZ_e_DATA_VALID])
	    {
		    if (Streamer_GetIntData(STREAMER_TYPE_AREA, GZ_g_Data[i][GZ_e_DATA_AREAID], E_STREAMER_INTERIOR_ID) != -1)
		    {
		        if (GZ_p_Shown[playerid][i])
		        {
		            if (newinteriorid != Streamer_GetIntData(STREAMER_TYPE_AREA, GZ_g_Data[i][GZ_e_DATA_AREAID], E_STREAMER_INTERIOR_ID))
		            	GangZoneHideForPlayer(playerid, i);
		        	else
		        	{
		        	    GangZoneShowForPlayer(playerid, i);
		        	    if (GZ_p_Flashing[playerid][i])
		        	    	GangZoneFlashForPlayer(playerid, i, GZ_p_FlashColor[playerid][i], GZ_p_FlashBorderColor[playerid][i]);
		        	}
				}
			}
		}
	}

	#if defined GZ_OnPlayerInteriorChange
		GZ_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid);
	#endif
	return 1;
}
#if defined _ALS_OnPlayerInteriorChange
	#undef OnPlayerInteriorChange
#else
	#define _ALS_OnPlayerInteriorChange
#endif
#define OnPlayerInteriorChange GZ_OnPlayerInteriorChange
#if defined GZ_OnPlayerInteriorChange
	forward GZ_OnPlayerInteriorChange(playerid);
#endif

public OnPlayerUpdate(playerid)
{
	new worldid = GetPlayerVirtualWorld(playerid);
	if (GZ_p_Worldid[playerid] != worldid)
	{
	    GZ_p_Worldid[playerid] = worldid;

	    for (new i; i < MAX_GANGZONES; i++)
		{
	        if (GZ_g_Data[i][GZ_e_DATA_VALID])
		    {
			    if (Streamer_GetIntData(STREAMER_TYPE_AREA, GZ_g_Data[i][GZ_e_DATA_AREAID], E_STREAMER_WORLD_ID) != -1)
			    {
			        if (GZ_p_Shown[playerid][i])
			        {
			            if (worldid != Streamer_GetIntData(STREAMER_TYPE_AREA, GZ_g_Data[i][GZ_e_DATA_AREAID], E_STREAMER_WORLD_ID))
			            	GangZoneHideForPlayer(playerid, i);
			        	else
			        	{
			        	    GangZoneShowForPlayer(playerid, i);
			        	    if (GZ_p_Flashing[playerid][i])
			        	    	GangZoneFlashForPlayer(playerid, i, GZ_p_FlashColor[playerid][i], GZ_p_FlashBorderColor[playerid][i]);
			        	}
					}
				}
			}
		}
	}

    #if defined GZ_OnPlayerUpdate
		GZ_OnPlayerUpdate(playerid);
	#endif
	return 1;
}
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate GZ_OnPlayerUpdate
#if defined GZ_OnPlayerUpdate
	forward GZ_OnPlayerUpdate(playerid);
#endif

public OnPlayerEnterDynamicArea(playerid, areaid)
{
    for (new i; i < MAX_GANGZONES; i++)
	{
        if (GZ_g_Data[i][GZ_e_DATA_VALID])
	    {
			if (areaid == GZ_g_Data[i][GZ_e_DATA_AREAID])
	    	{
	    	    #if defined OnPlayerEnterGangZone
	    	    	OnPlayerEnterGangZone(playerid, i);
	    	    #endif
				break;
	    	}
	    }
	}

    #if defined GZ_OnPlayerEnterDynamicArea
		GZ_OnPlayerEnterDynamicArea(playerid, areaid);
	#endif
	return 1;
}
#if defined _ALS_OnPlayerEnterDynamicArea
	#undef OnPlayerEnterDynamicArea
#else
	#define _ALS_OnPlayerEnterDynamicArea
#endif
#define OnPlayerEnterDynamicArea GZ_OnPlayerEnterDynamicArea
#if defined GZ_OnPlayerEnterDynamicArea
	forward GZ_OnPlayerEnterDynamicArea(playerid, areaid);
#endif

public OnPlayerLeaveDynamicArea(playerid, areaid)
{
    for (new i; i < MAX_GANGZONES; i++)
	{
        if (GZ_g_Data[i][GZ_e_DATA_VALID])
	    {
			if (areaid == GZ_g_Data[i][GZ_e_DATA_AREAID])
	    	{
	    	    #if defined OnPlayerLeaveGangZone
	    	    	OnPlayerLeaveGangZone(playerid, i);
	    	    #endif
				break;
	    	}
	    }
	}

    #if defined GZ_OnPlayerLeaveDynamicArea
		GZ_OnPlayerLeaveDynamicArea(playerid, areaid);
	#endif
	return 1;
}
#if defined _ALS_OnPlayerLeaveDynamicArea
	#undef OnPlayerLeaveDynamicArea
#else
	#define _ALS_OnPlayerLeaveDynamicArea
#endif
#define OnPlayerLeaveDynamicArea GZ_OnPlayerLeaveDynamicArea
#if defined GZ_OnPlayerLeaveDynamicArea
	forward GZ_OnPlayerLeaveDynamicArea(playerid, areaid);
#endif

#if defined OnPlayerEnterGangZone
	forward OnPlayerEnterGangZone(playerid, zone);
#endif

#if defined OnPlayerLeaveGangZone
	forward OnPlayerLeaveGangZone(playerid, zone);
#endif
