// Gangzones.inc - By Gammix - v2.0.1 - Last updated 21 January, 2017
#if defined gangzones_included
	#endinput
#endif
#define gangzones_included

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
static GZ_eData[MAX_GANGZONES][GZ_e_DATA];

static bool:GZ_bIsShown[MAX_PLAYERS][MAX_GANGZONES];
static GZ_iShownColor[MAX_PLAYERS][MAX_GANGZONES];
static GZ_iShownBorderColor[MAX_PLAYERS][MAX_GANGZONES];

static bool:GZ_bIsFlashing[MAX_PLAYERS][MAX_GANGZONES];
static GZ_iFlashColor[MAX_PLAYERS][MAX_GANGZONES];
static GZ_iFlashBorderColor[MAX_PLAYERS][MAX_GANGZONES];

static GZ_iPlayerWorldid[MAX_PLAYERS];

/*
native GangZoneCreate(Float:minx, Float:miny, Float:maxx, Float:maxy, color = -1, Float:bordersize = 1.0, bordercolor = 0x00000060, interior = -1, virtualworld = -1)
*/
stock GZ_GangZoneCreate(Float:minx, Float:miny, Float:maxx, Float:maxy, color = -1, Float:bordersize = 1.0, bordercolor = 0x00000060, interior = -1, virtualworld = -1)
{
	new zone = -1;
	for (new i; i < MAX_GANGZONES; i++)
	{
	    if (!GZ_eData[i][GZ_e_DATA_VALID])
	    {
	        zone = i;
	        break;
		}
	}

	if (zone == -1)
	    return -1;

	new Float:SEPERATION = (2.0*bordersize);

    GZ_eData[zone][GZ_e_DATA_VALID] = true;
	GZ_eData[zone][GZ_e_DATA_ZONEID] = GangZoneCreate(minx, miny, maxx, maxy);
	GZ_eData[zone][GZ_e_DATA_BORDERID][0] = GangZoneCreate((minx - SEPERATION), miny, (minx + SEPERATION), maxy);
	GZ_eData[zone][GZ_e_DATA_BORDERID][1] = GangZoneCreate((minx - SEPERATION), (maxy - SEPERATION), maxx, (maxy + SEPERATION));
	GZ_eData[zone][GZ_e_DATA_BORDERID][2] = GangZoneCreate((maxx - SEPERATION), miny, (maxx + SEPERATION), maxy);
	GZ_eData[zone][GZ_e_DATA_BORDERID][3] = GangZoneCreate((minx - SEPERATION), (miny - SEPERATION), maxx, (miny + SEPERATION));
	GZ_eData[zone][GZ_e_DATA_AREAID] = CreateDynamicRectangle(minx, miny, maxx, maxy, virtualworld, interior, -1);

	for (new i; i < MAX_PLAYERS; i++)
 	{
        GZ_bIsShown[i][zone] = false;
 	    GZ_iShownColor[i][zone] = color;
        GZ_iShownBorderColor[i][zone] = bordercolor;

		GZ_bIsFlashing[i][zone] = false;
		GZ_iFlashColor[i][zone] = -1;
		GZ_iFlashBorderColor[i][zone] = -1;
	}

	return zone;
}
#if defined _ALS_GangZoneCreate
    #undef GangZoneCreate
#else
    #define _ALS_GangZoneCreate
#endif
#define GangZoneCreate GZ_GangZoneCreate

/*
native GangZoneDestroy(zone);
*/
stock GZ_GangZoneDestroy(zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_eData[zone][GZ_e_DATA_VALID])
	  	return 0;

    GZ_eData[zone][GZ_e_DATA_VALID] = false;
    GangZoneDestroy(GZ_eData[zone][GZ_e_DATA_ZONEID]);
    GangZoneDestroy(GZ_eData[zone][GZ_e_DATA_BORDERID][0]);
    GangZoneDestroy(GZ_eData[zone][GZ_e_DATA_BORDERID][1]);
    GangZoneDestroy(GZ_eData[zone][GZ_e_DATA_BORDERID][2]);
    GangZoneDestroy(GZ_eData[zone][GZ_e_DATA_BORDERID][3]);
	DestroyDynamicArea(GZ_eData[zone][GZ_e_DATA_AREAID]);

	for (new i; i < MAX_PLAYERS; i++)
 	{
        GZ_bIsShown[i][zone] = false;
 	    GZ_iShownColor[i][zone] = -1;
        GZ_iShownBorderColor[i][zone] = -1;

		GZ_bIsFlashing[i][zone] = false;
		GZ_iFlashColor[i][zone] = -1;
		GZ_iFlashBorderColor[i][zone] = -1;
	}

  	return 1;
}
#if defined _ALS_GangZoneDestroy
    #undef GangZoneDestroy
#else
    #define _ALS_GangZoneDestroy
#endif
#define GangZoneDestroy GZ_GangZoneDestroy

/*
native GangZoneExist(zone);
*/
stock GangZoneExist(zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

	return GZ_eData[zone][GZ_e_DATA_VALID];
}

/*
native GangZoneShowForPlayer(playerid, zone, color = -1, bordercolor = -1);
*/
stock GZ_GangZoneShowForPlayer(playerid, zone, color = -1, bordercolor = -1)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_eData[zone][GZ_e_DATA_VALID])
	  	return 0;

  	if (color != -1)
	  	GZ_iShownColor[playerid][zone] = color;

  	if (bordercolor != -1)
	  	GZ_iShownBorderColor[playerid][zone] = bordercolor;

	GangZoneShowForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_ZONEID], GZ_iShownColor[playerid][zone]);
	GangZoneShowForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][0], GZ_iShownBorderColor[playerid][zone]);
	GangZoneShowForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][1], GZ_iShownBorderColor[playerid][zone]);
	GangZoneShowForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][2], GZ_iShownBorderColor[playerid][zone]);
	GangZoneShowForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][3], GZ_iShownBorderColor[playerid][zone]);

	GZ_bIsShown[playerid][zone] = true;
	return 1;
}
#if defined _ALS_GangZoneShowForPlayer
    #undef GangZoneShowForPlayer
#else
    #define _ALS_GangZoneShowForPlayer
#endif
#define GangZoneShowForPlayer GZ_GangZoneShowForPlayer

/*
native GangZoneHideForPlayer(playerid, zone);
*/
stock GZ_GangZoneHideForPlayer(playerid, zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_eData[zone][GZ_e_DATA_VALID])
	  	return 0;

	GangZoneHideForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_ZONEID]);
	GangZoneHideForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][0]);
	GangZoneHideForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][1]);
	GangZoneHideForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][2]);
	GangZoneHideForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][3]);

	GZ_bIsShown[playerid][zone] = false;
	return 1;
}
#if defined _ALS_GangZoneHideForPlayer
    #undef GangZoneHideForPlayer
#else
    #define _ALS_GangZoneHideForPlayer
#endif
#define GangZoneHideForPlayer GZ_GangZoneHideForPlayer

/*
native GangZoneShowForAll(zone, color = -1, bordercolor = -1);
*/
stock GZ_GangZoneShowForAll(zone, color = -1, bordercolor = -1)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_eData[zone][GZ_e_DATA_VALID])
	  	return 0;

 	for (new i; i < MAX_PLAYERS; i++)
 	{
	  	if (color != -1)
		  	GZ_iShownColor[i][zone] = color;

	  	if (bordercolor != -1)
		  	GZ_iShownBorderColor[i][zone] = bordercolor;

		GangZoneShowForPlayer(i, GZ_eData[zone][GZ_e_DATA_ZONEID], GZ_iShownColor[i][zone]);
		GangZoneShowForPlayer(i, GZ_eData[zone][GZ_e_DATA_BORDERID][0], GZ_iShownBorderColor[i][zone]);
		GangZoneShowForPlayer(i, GZ_eData[zone][GZ_e_DATA_BORDERID][1], GZ_iShownBorderColor[i][zone]);
		GangZoneShowForPlayer(i, GZ_eData[zone][GZ_e_DATA_BORDERID][2], GZ_iShownBorderColor[i][zone]);
		GangZoneShowForPlayer(i, GZ_eData[zone][GZ_e_DATA_BORDERID][3], GZ_iShownBorderColor[i][zone]);

		GZ_bIsShown[i][zone] = true;
	}
	return 1;
}
#if defined _ALS_GangZoneShowForAll
    #undef GangZoneShowForAll
#else
    #define _ALS_GangZoneShowForAll
#endif
#define GangZoneShowForAll GZ_GangZoneShowForAll

/*
native GangZoneHideForAll(zone);
*/
stock GZ_GangZoneHideForAll(zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_eData[zone][GZ_e_DATA_VALID])
	  	return 0;

	GangZoneHideForAll(GZ_eData[zone][GZ_e_DATA_ZONEID]);
	GangZoneHideForAll(GZ_eData[zone][GZ_e_DATA_BORDERID][0]);
	GangZoneHideForAll(GZ_eData[zone][GZ_e_DATA_BORDERID][1]);
	GangZoneHideForAll(GZ_eData[zone][GZ_e_DATA_BORDERID][2]);
	GangZoneHideForAll(GZ_eData[zone][GZ_e_DATA_BORDERID][3]);

 	for (new i; i < MAX_PLAYERS; i++)
 		GZ_bIsShown[i][zone] = false;
	return 1;
}
#if defined _ALS_GangZoneHideForAll
    #undef GangZoneHideForAll
#else
    #define _ALS_GangZoneHideForAll
#endif
#define GangZoneHideForAll GZ_GangZoneHideForAll

/*
native GangZoneFlashForPlayer(playerid, zone, flashcolor = -1, flashbordercolor = -1);
*/
stock GZ_GangZoneFlashForPlayer(playerid, zone, flashcolor = -1, flashbordercolor = -1)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_eData[zone][GZ_e_DATA_VALID])
	  	return 0;

  	if (flashcolor != -1)
  	{
	  	GZ_iFlashColor[playerid][zone] = flashcolor;
		GangZoneFlashForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_ZONEID], GZ_iFlashColor[playerid][zone]);
	}

  	if (flashbordercolor != -1)
	{
	  	GZ_iFlashBorderColor[playerid][zone] = flashbordercolor;
		GangZoneShowForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][0], GZ_iFlashBorderColor[playerid][zone]);
		GangZoneShowForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][1], GZ_iFlashBorderColor[playerid][zone]);
		GangZoneShowForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][2], GZ_iFlashBorderColor[playerid][zone]);
		GangZoneShowForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][3], GZ_iFlashBorderColor[playerid][zone]);
	}

	GZ_bIsFlashing[playerid][zone] = true;
	return 1;
}
#if defined _ALS_GangZoneFlashForPlayer
    #undef GangZoneFlashForPlayer
#else
    #define _ALS_GangZoneFlashForPlayer
#endif
#define GangZoneFlashForPlayer GZ_GangZoneFlashForPlayer

/*
native GangZoneStopFlashForPlayer(playerid, zone);
*/
stock GZ_GangZoneStopFlashForPlayer(playerid, zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_eData[zone][GZ_e_DATA_VALID])
	  	return 0;

	GangZoneStopFlashForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_ZONEID]);
	GangZoneStopFlashForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][0]);
	GangZoneStopFlashForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][1]);
	GangZoneStopFlashForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][2]);
	GangZoneStopFlashForPlayer(playerid, GZ_eData[zone][GZ_e_DATA_BORDERID][3]);

	GZ_bIsFlashing[playerid][zone] = false;
	return 1;
}
#if defined _ALS_GangZoneStopFlashForPlayer
    #undef GangZoneStopFlashForPlayer
#else
    #define _ALS_GangZoneStopFlashForPlayer
#endif
#define GangZoneStopFlashForPlayer GZ_GangZoneStopFlashForPlayer

/*
native GangZoneFlashForAll(zone, flashcolor = -1, flashbordercolor = -1);
*/
stock GZ_GangZoneFlashForAll(zone, flashcolor = -1, flashbordercolor = -1)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_eData[zone][GZ_e_DATA_VALID])
	  	return 0;

	if (flashcolor != -1)
		GangZoneFlashForAll(GZ_eData[zone][GZ_e_DATA_ZONEID], flashcolor);

	if (flashbordercolor != -1)
	{
		GangZoneFlashForAll(GZ_eData[zone][GZ_e_DATA_BORDERID][0], flashbordercolor);
		GangZoneFlashForAll(GZ_eData[zone][GZ_e_DATA_BORDERID][1], flashbordercolor);
		GangZoneFlashForAll(GZ_eData[zone][GZ_e_DATA_BORDERID][2], flashbordercolor);
		GangZoneFlashForAll(GZ_eData[zone][GZ_e_DATA_BORDERID][3], flashbordercolor);
	}

	for (new i; i < MAX_PLAYERS; i++)
 	{
		GZ_bIsFlashing[i][zone] = true;
	  	GZ_iFlashColor[i][zone] = flashcolor;
		GZ_iFlashBorderColor[i][zone] = flashbordercolor;
	}
	return 1;
}
#if defined _ALS_GangZoneFlashForAll
    #undef GangZoneFlashForAll
#else
    #define _ALS_GangZoneFlashForAll
#endif
#define GangZoneFlashForAll GZ_GangZoneFlashForAll

/*
native GangZoneStopFlashForAll(zone);
*/
stock GZ_GangZoneStopFlashForAll(zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_eData[zone][GZ_e_DATA_VALID])
	  	return 0;

	GangZoneStopFlashForAll(GZ_eData[zone][GZ_e_DATA_ZONEID]);
	GangZoneStopFlashForAll(GZ_eData[zone][GZ_e_DATA_BORDERID][0]);
	GangZoneStopFlashForAll(GZ_eData[zone][GZ_e_DATA_BORDERID][1]);
	GangZoneStopFlashForAll(GZ_eData[zone][GZ_e_DATA_BORDERID][2]);
	GangZoneStopFlashForAll(GZ_eData[zone][GZ_e_DATA_BORDERID][3]);

	for (new i; i < MAX_PLAYERS; i++)
 		GZ_bIsFlashing[i][zone] = false;
	return 1;
}
#if defined _ALS_GangZoneStopFlashForAll
    #undef GangZoneStopFlashForAll
#else
    #define _ALS_GangZoneStopFlashForAll
#endif
#define GangZoneStopFlashForAll GZ_GangZoneStopFlashForAll

/*
native GangZoneSetInterior(zone, interiorid);
*/
stock GangZoneSetInterior(zone, interiorid)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_eData[zone][GZ_e_DATA_VALID])
	  	return 0;

    Streamer_SetIntData(STREAMER_TYPE_AREA, GZ_eData[zone][GZ_e_DATA_AREAID], E_STREAMER_INTERIOR_ID, interiorid);
	return 1;
}

/*
native GangZoneGetInterior(zone);
*/
stock GangZoneGetInterior(zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_eData[zone][GZ_e_DATA_VALID])
	  	return 0;

	return Streamer_GetIntData(STREAMER_TYPE_AREA, GZ_eData[zone][GZ_e_DATA_AREAID], E_STREAMER_INTERIOR_ID);
}

/*
native GangZoneSetVituralWorld(zone, virtualworldid);
*/
stock GangZoneSetVituralWorld(zone, virtualworldid)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_eData[zone][GZ_e_DATA_VALID])
	  	return 0;

    Streamer_SetIntData(STREAMER_TYPE_AREA, GZ_eData[zone][GZ_e_DATA_AREAID], E_STREAMER_WORLD_ID, virtualworldid);
    return 1;
}

/*
native GangZoneGetVirtualWorld(zone);
*/
stock GangZoneGetVirtualWorld(zone)
{
	if (zone < 0 || zone >= MAX_GANGZONES)
	    return 0;

  	if (!GZ_eData[zone][GZ_e_DATA_VALID])
	  	return 0;

	return Streamer_GetIntData(STREAMER_TYPE_AREA, GZ_eData[zone][GZ_e_DATA_AREAID], E_STREAMER_WORLD_ID);
}

/*
native CountAllGangZones();
*/
stock CountAllGangZones()
{
	new count = 0;
    for (new i; i < MAX_GANGZONES; i++)
	{
	    if (GZ_eData[i][GZ_e_DATA_VALID])
	    	count++;
	}
	return count;
}

/*
native DestroyAllGangZones();
*/
stock DestroyAllGangZones()
{
    for (new i; i < MAX_GANGZONES; i++)
		GangZoneDestroy(i);
	return 1;
}

/*
native ShowAllGangZonesForPlayer(playerid, color = -1, bordercolor = -1);
*/
stock ShowAllGangZonesForPlayer(playerid, color = -1, bordercolor = -1)
{
	for (new i; i < MAX_GANGZONES; i++)
	{
	    if (GZ_eData[i][GZ_e_DATA_VALID])
	    	GangZoneShowForPlayer(playerid, i, color, bordercolor);
	}
	return 1;
}

/*
native HideAllGangZonesForPlayer(playerid);
*/
stock HideAllGangZonesForPlayer(playerid)
{
	for (new i; i < MAX_GANGZONES; i++)
	{
	    if (GZ_eData[i][GZ_e_DATA_VALID])
	    	GangZoneHideForPlayer(playerid, i);
	}
	return 1;
}

/*
native ShowAllGangZonesForAll(color = -1, bordercolor = -1);
*/
stock ShowAllGangZonesForAll(color = -1, bordercolor = -1)
{
	for (new i; i < MAX_GANGZONES; i++)
	{
	    if (GZ_eData[i][GZ_e_DATA_VALID])
			GangZoneShowForAll(i, color, bordercolor);
	}
	return 1;
}

/*
native HideAllGangZonesForAll();
*/
stock HideAllGangZonesForAll()
{
	for (new i; i < MAX_GANGZONES; i++)
	{
	    if (GZ_eData[i][GZ_e_DATA_VALID])
			GangZoneHideForAll(i);
	}
	return 1;
}

/*
native GetPlayerGangZone(playerid);
*/
stock GetPlayerGangZone(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	    return -1;

	for (new i; i < MAX_GANGZONES; i++)
	{
	    if (GZ_eData[i][GZ_e_DATA_VALID])
	    {
	        if (IsPlayerInDynamicArea(playerid, GZ_eData[i][GZ_e_DATA_AREAID]))
	            return i;
		}
	}
	return 1;
}

/*
native IsPlayerInGangZone(playerid, zone);
*/
#define IsPlayerInGangZone(%1,%2) \
    (GetPlayerGangZone(%1) == %2)

/*
native IsPlayerInAnyGangZone(playerid);
*/
#define IsPlayerInAnyGangZone(%1) \
    (GetPlayerGangZone(%1) != -1)

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
    for (new i; i < MAX_GANGZONES; i++)
	{
        if (GZ_eData[i][GZ_e_DATA_VALID])
	    {
		    if (Streamer_GetIntData(STREAMER_TYPE_AREA, GZ_eData[i][GZ_e_DATA_AREAID], E_STREAMER_INTERIOR_ID) != -1)
		    {
		        if (GZ_bIsShown[playerid][i])
		        {
		            if (newinteriorid != Streamer_GetIntData(STREAMER_TYPE_AREA, GZ_eData[i][GZ_e_DATA_AREAID], E_STREAMER_INTERIOR_ID))
		            	GangZoneHideForPlayer(playerid, i);
		        	else
		        	{
		        	    GangZoneShowForPlayer(playerid, i);
		        	    if (GZ_bIsFlashing[playerid][i])
		        	    	GangZoneFlashForPlayer(playerid, i, GZ_iFlashColor[playerid][i], GZ_iFlashBorderColor[playerid][i]);
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
	if (GZ_iPlayerWorldid[playerid] != worldid)
	{
	    GZ_iPlayerWorldid[playerid] = worldid;

	    for (new i; i < MAX_GANGZONES; i++)
		{
	        if (GZ_eData[i][GZ_e_DATA_VALID])
		    {
			    if (Streamer_GetIntData(STREAMER_TYPE_AREA, GZ_eData[i][GZ_e_DATA_AREAID], E_STREAMER_WORLD_ID) != -1)
			    {
			        if (GZ_bIsShown[playerid][i])
			        {
			            if (worldid != Streamer_GetIntData(STREAMER_TYPE_AREA, GZ_eData[i][GZ_e_DATA_AREAID], E_STREAMER_WORLD_ID))
			            	GangZoneHideForPlayer(playerid, i);
			        	else
			        	{
			        	    GangZoneShowForPlayer(playerid, i);
			        	    if (GZ_bIsFlashing[playerid][i])
			        	    	GangZoneFlashForPlayer(playerid, i, GZ_iFlashColor[playerid][i], GZ_iFlashBorderColor[playerid][i]);
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
        if (GZ_eData[i][GZ_e_DATA_VALID])
	    {
			if (areaid == GZ_eData[i][GZ_e_DATA_AREAID])
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
        if (GZ_eData[i][GZ_e_DATA_VALID])
	    {
			if (areaid == GZ_eData[i][GZ_e_DATA_AREAID])
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
