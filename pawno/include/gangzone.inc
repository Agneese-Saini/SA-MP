// gangzone.inc - v2.0 - updated on 27 Feb, 2017 - by Gammix
#if defined gangzones_included
	#endinput
#endif
#define gangzones_included

/*
native GangZoneCreate(Float:minx, Float:miny, Float:maxx, Float:maxy, Float:bordersize = 1.0);
native GangZoneDestroy(zoneid);
native GangZoneShowForPlayer(playerid, zoneid, color, bordercolor = 0x000000FF);
native GangZoneHideForPlayer(playerid, zoneid);
native GangZoneShowForAll(zoneid, color, bordercolor = 0x000000FF);
native GangZoneHideForAll(zoneid);
native GangZoneFlashForPlayer(playerid, zoneid, flashcolor, flashbordercolor = 0);
native GangZoneStopFlashForPlayer(playerid, zoneid, bool:mainzone = true, bool:border = true);
native GangZoneFlashForAll(zoneid, flashcolor = 0, flashbordercolor = 0);
native GangZoneStopFlashForAll(zoneid, bool:mainzone = true, bool:border = true);
native IsValidGangZone(zoneid);
*/

#if defined MAX_GANG_ZONES
	#undef MAX_GANG_ZONES
#endif
#define MAX_GANG_ZONES (1024 / 5)

enum GZ_e_ZONE
{
    bool:GZ_e_ZONE_VALID,
    Float:GZ_e_ZONE_MINX,
    Float:GZ_e_ZONE_MINY,
    Float:GZ_e_ZONE_MAXX,
    Float:GZ_e_ZONE_MAXY,
    GZ_e_ZONE_ID[5]
};
static GZ_eZone[MAX_GANG_ZONES][GZ_e_ZONE];
static GZ_iCount;

#if defined OnPlayerEnterGangZone || defined OnPlayerLeaveGangZone
	static Float:GZ_fLastPos[MAX_PLAYERS][2];
	static GZ_iPlayerZoneid[MAX_PLAYERS];
#endif

stock GZ_GangZoneCreate(Float:minx, Float:miny, Float:maxx, Float:maxy, Float:bordersize = 1.0)
{
	if (GZ_iCount == MAX_GANG_ZONES)
	{
	    return -1;
	}
	
	new zoneid = GZ_iCount++;
    GZ_eZone[zoneid][GZ_e_ZONE_VALID] = true;

    GZ_eZone[zoneid][GZ_e_ZONE_MINX] = minx;
    GZ_eZone[zoneid][GZ_e_ZONE_MINY] = miny;
    GZ_eZone[zoneid][GZ_e_ZONE_MAXX] = maxx;
    GZ_eZone[zoneid][GZ_e_ZONE_MAXY] = maxy;
    
	new Float:SEPERATION = 2.0 * bordersize;
    GZ_eZone[zoneid][GZ_e_ZONE_ID][0] = GangZoneCreate(minx, miny, maxx, maxy);
	GZ_eZone[zoneid][GZ_e_ZONE_ID][1] = GangZoneCreate((minx - SEPERATION), miny, (minx + SEPERATION), maxy);
	GZ_eZone[zoneid][GZ_e_ZONE_ID][2] = GangZoneCreate((minx - SEPERATION), (maxy - SEPERATION), maxx, (maxy + SEPERATION));
	GZ_eZone[zoneid][GZ_e_ZONE_ID][3] = GangZoneCreate((maxx - SEPERATION), miny, (maxx + SEPERATION), maxy);
	GZ_eZone[zoneid][GZ_e_ZONE_ID][4] = GangZoneCreate((minx - SEPERATION), (miny - SEPERATION), maxx, (miny + SEPERATION));
	return zoneid;
}
#if defined _ALS_GangZoneCreate
    #undef GangZoneCreate
#else
    #define _ALS_GangZoneCreate
#endif
#define GangZoneCreate GZ_GangZoneCreate

stock GZ_GangZoneDestroy(zoneid)
{
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES)
	{
	    return 0;
	}
	
  	if (!GZ_eZone[zoneid][GZ_e_ZONE_VALID])
  	{
	  	return 0;
	}
	
	for (new i = zoneid; i < (GZ_iCount - 1); i++)
	{
	    GZ_eZone[i][GZ_e_ZONE_VALID] = GZ_eZone[(i + 1)][GZ_e_ZONE_VALID];

	    GZ_eZone[i][GZ_e_ZONE_MINX] = GZ_eZone[(i + 1)][GZ_e_ZONE_MINX];
	    GZ_eZone[i][GZ_e_ZONE_MINY] = GZ_eZone[(i + 1)][GZ_e_ZONE_MINY];
	    GZ_eZone[i][GZ_e_ZONE_MAXX] = GZ_eZone[(i + 1)][GZ_e_ZONE_MAXX];
	    GZ_eZone[i][GZ_e_ZONE_MAXY] = GZ_eZone[(i + 1)][GZ_e_ZONE_MAXY];

	    GZ_eZone[i][GZ_e_ZONE_ID][0] = GZ_eZone[(i + 1)][GZ_e_ZONE_ID][0];
		GZ_eZone[i][GZ_e_ZONE_ID][1] = GZ_eZone[(i + 1)][GZ_e_ZONE_ID][1];
		GZ_eZone[i][GZ_e_ZONE_ID][2] = GZ_eZone[(i + 1)][GZ_e_ZONE_ID][2];
		GZ_eZone[i][GZ_e_ZONE_ID][3] = GZ_eZone[(i + 1)][GZ_e_ZONE_ID][3];
		GZ_eZone[i][GZ_e_ZONE_ID][4] = GZ_eZone[(i + 1)][GZ_e_ZONE_ID][4];
	}
	GZ_iCount--;
	
    GZ_eZone[zoneid][GZ_e_ZONE_VALID] = false;
    GangZoneDestroy(GZ_eZone[zoneid][GZ_e_ZONE_ID][0]);
    GangZoneDestroy(GZ_eZone[zoneid][GZ_e_ZONE_ID][1]);
    GangZoneDestroy(GZ_eZone[zoneid][GZ_e_ZONE_ID][2]);
    GangZoneDestroy(GZ_eZone[zoneid][GZ_e_ZONE_ID][3]);
    GangZoneDestroy(GZ_eZone[zoneid][GZ_e_ZONE_ID][4]);
  	return 1;
}
#if defined _ALS_GangZoneDestroy
    #undef GangZoneDestroy
#else
    #define _ALS_GangZoneDestroy
#endif
#define GangZoneDestroy GZ_GangZoneDestroy

stock GZ_GangZoneShowForPlayer(playerid, zoneid, color, bordercolor = 0x000000FF)
{
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES)
	{
	    return 0;
	}

  	if (!GZ_eZone[zoneid][GZ_e_ZONE_VALID])
  	{
	  	return 0;
	}
	
	GangZoneShowForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][0], color);
	GangZoneShowForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][1], bordercolor);
	GangZoneShowForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][2], bordercolor);
	GangZoneShowForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][3], bordercolor);
	GangZoneShowForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][4], bordercolor);
	return 1;
}
#if defined _ALS_GangZoneShowForPlayer
    #undef GangZoneShowForPlayer
#else
    #define _ALS_GangZoneShowForPlayer
#endif
#define GangZoneShowForPlayer GZ_GangZoneShowForPlayer

stock GZ_GangZoneHideForPlayer(playerid, zoneid)
{
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES)
	{
	    return 0;
	}

  	if (!GZ_eZone[zoneid][GZ_e_ZONE_VALID])
  	{
	  	return 0;
	}

	GangZoneHideForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][0]);
	GangZoneHideForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][1]);
	GangZoneHideForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][2]);
	GangZoneHideForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][3]);
	GangZoneHideForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][4]);
	return 1;
}
#if defined _ALS_GangZoneHideForPlayer
    #undef GangZoneHideForPlayer
#else
    #define _ALS_GangZoneHideForPlayer
#endif
#define GangZoneHideForPlayer GZ_GangZoneHideForPlayer

stock GZ_GangZoneShowForAll(zoneid, color, bordercolor = 0x000000FF)
{
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES)
	{
	    return 0;
	}

  	if (!GZ_eZone[zoneid][GZ_e_ZONE_VALID])
  	{
	  	return 0;
	}

 	GangZoneShowForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][0], color);
	GangZoneShowForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][1], bordercolor);
	GangZoneShowForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][2], bordercolor);
	GangZoneShowForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][3], bordercolor);
	GangZoneShowForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][4], bordercolor);
	return 1;
}
#if defined _ALS_GangZoneShowForAll
    #undef GangZoneShowForAll
#else
    #define _ALS_GangZoneShowForAll
#endif
#define GangZoneShowForAll GZ_GangZoneShowForAll

stock GZ_GangZoneHideForAll(zoneid)
{
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES)
	{
	    return 0;
	}

  	if (!GZ_eZone[zoneid][GZ_e_ZONE_VALID])
  	{
	  	return 0;
	}

	GangZoneHideForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][0]);
	GangZoneHideForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][1]);
	GangZoneHideForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][2]);
	GangZoneHideForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][3]);
	GangZoneHideForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][4]);
	return 1;
}
#if defined _ALS_GangZoneHideForAll
    #undef GangZoneHideForAll
#else
    #define _ALS_GangZoneHideForAll
#endif
#define GangZoneHideForAll GZ_GangZoneHideForAll

stock GZ_GangZoneFlashForPlayer(playerid, zoneid, flashcolor, flashbordercolor = 0)
{
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES)
	{
	    return 0;
	}

  	if (!GZ_eZone[zoneid][GZ_e_ZONE_VALID])
  	{
	  	return 0;
	}

  	if (flashcolor != 0)
  	{
	  	GangZoneFlashForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][0], flashcolor);
	}

  	if (flashbordercolor != 0)
	{
		GangZoneShowForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][1], flashbordercolor);
		GangZoneShowForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][2], flashbordercolor);
		GangZoneShowForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][3], flashbordercolor);
		GangZoneShowForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][4], flashbordercolor);
	}
	return 1;
}
#if defined _ALS_GangZoneFlashForPlayer
    #undef GangZoneFlashForPlayer
#else
    #define _ALS_GangZoneFlashForPlayer
#endif
#define GangZoneFlashForPlayer GZ_GangZoneFlashForPlayer

stock GZ_GangZoneStopFlashForPlayer(playerid, zoneid, bool:mainzone = true, bool:border = true)
{
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES)
	{
	    return 0;
	}

  	if (!GZ_eZone[zoneid][GZ_e_ZONE_VALID])
  	{
	  	return 0;
	}

	if (mainzone)
	{
		GangZoneStopFlashForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][0]);
	}

	if (border)
	{
		GangZoneStopFlashForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][1]);
		GangZoneStopFlashForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][2]);
		GangZoneStopFlashForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][3]);
		GangZoneStopFlashForPlayer(playerid, GZ_eZone[zoneid][GZ_e_ZONE_ID][4]);
	}
	return 1;
}
#if defined _ALS_GangZoneStopFlashForPlayer
    #undef GangZoneStopFlashForPlayer
#else
    #define _ALS_GangZoneStopFlashForPlayer
#endif
#define GangZoneStopFlashForPlayer GZ_GangZoneStopFlashForPlayer

stock GZ_GangZoneFlashForAll(zoneid, flashcolor = 0, flashbordercolor = 0)
{
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES)
	{
	    return 0;
	}

  	if (!GZ_eZone[zoneid][GZ_e_ZONE_VALID])
  	{
	  	return 0;
	}

	if (flashcolor != 0)
	{
		GangZoneFlashForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][0], flashcolor);
	}
	
	if (flashbordercolor != 0)
	{
		GangZoneFlashForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][1], flashbordercolor);
		GangZoneFlashForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][2], flashbordercolor);
		GangZoneFlashForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][3], flashbordercolor);
		GangZoneFlashForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][4], flashbordercolor);
	}
	return 1;
}
#if defined _ALS_GangZoneFlashForAll
    #undef GangZoneFlashForAll
#else
    #define _ALS_GangZoneFlashForAll
#endif
#define GangZoneFlashForAll GZ_GangZoneFlashForAll

stock GZ_GangZoneStopFlashForAll(zoneid, bool:mainzone = true, bool:border = true)
{
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES)
	{
	    return 0;
	}

  	if (!GZ_eZone[zoneid][GZ_e_ZONE_VALID])
  	{
	  	return 0;
	}

	if (mainzone)
	{
		GangZoneStopFlashForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][0]);
	}

	if (border)
	{
		GangZoneStopFlashForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][1]);
		GangZoneStopFlashForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][2]);
		GangZoneStopFlashForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][3]);
		GangZoneStopFlashForAll(GZ_eZone[zoneid][GZ_e_ZONE_ID][4]);
	}
	return 1;
}
#if defined _ALS_GangZoneStopFlashForAll
    #undef GangZoneStopFlashForAll
#else
    #define _ALS_GangZoneStopFlashForAll
#endif
#define GangZoneStopFlashForAll GZ_GangZoneStopFlashForAll

stock IsValidGangZone(zoneid)
{
	if (zoneid < 0 || zoneid >= MAX_GANG_ZONES)
	{
	    return 0;
	}

  	return GZ_eZone[zoneid][GZ_e_ZONE_VALID];
}

#if defined OnPlayerEnterGangZone || defined OnPlayerLeaveGangZone
	public OnPlayerUpdate(playerid)
	{
		new Float:x,
		    Float:y,
		    Float:z;
		GetPlayerPos(playerid, x, y, z);
		#pragma unused z
		if (x != GZ_fLastPos[playerid][0] || y != GZ_fLastPos[playerid][1])
		{
		    GZ_fLastPos[playerid][0] = x;
		    GZ_fLastPos[playerid][1] = y;

		    new zoneid;
			if (GZ_iPlayerZoneid[playerid] != -1)
			{
				zoneid = GZ_iPlayerZoneid[playerid];
			    if (x >= GZ_eZone[zoneid][GZ_e_ZONE_MINX] && x <= GZ_eZone[zoneid][GZ_e_ZONE_MAXX] && y >= GZ_eZone[zoneid][GZ_e_ZONE_MINY] && y <= GZ_eZone[zoneid][GZ_e_ZONE_MAXY])
				{
				    #if defined GZ_OnPlayerUpdate
						return GZ_OnPlayerUpdate(playerid);
					#else
						return 1;
					#endif
				}
			}

			for (zoneid = 0; zoneid < GZ_iCount; zoneid++)
			{
				if (zoneid == GZ_iPlayerZoneid[playerid])
			  	{
	     			continue;
			   	}

	            if (x >= GZ_eZone[zoneid][GZ_e_ZONE_MINX] && x <= GZ_eZone[zoneid][GZ_e_ZONE_MAXX] && y >= GZ_eZone[zoneid][GZ_e_ZONE_MINY] && y <= GZ_eZone[zoneid][GZ_e_ZONE_MAXY])
	            {
	            	#if defined OnPlayerEnterGangZone
	            		OnPlayerEnterGangZone(playerid, zoneid);
					#endif

	            	#if defined OnPlayerLeaveGangZone
						if (GZ_iPlayerZoneid[playerid] != -1)
						{
		            		OnPlayerEnterGangZone(playerid, GZ_iPlayerZoneid[playerid]);
						}
					#endif

	                GZ_iPlayerZoneid[playerid] = zoneid;
	                break;
			    }
			}
		}

	    #if defined GZ_OnPlayerUpdate
			return GZ_OnPlayerUpdate(playerid);
		#else
			return 1;
		#endif
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
#endif
