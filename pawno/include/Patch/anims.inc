#if defined anims_included
	#endinput
#endif
#define anims_included

static const ANIM_LIST[][] =
{
    "AIRPORT",      "Attractors",   "BAR",          "BASEBALL",     "BD_FIRE",
    "BEACH",        "benchpress",   "BF_injection", "BIKED",        "BIKEH",
    "BIKELEAP",     "BIKES",        "BIKEV",        "BIKE_DBZ",     "BLOWJOBZ",
    "BMX",          "BOMBER",       "BOX",          "BSKTBALL",     "BUDDY",
    "BUS",          "CAMERA",       "CAR",          "CARRY",        "CAR_CHAT",
    "CASINO",       "CHAINSAW",     "CHOPPA",       "CLOTHES",      "COACH",
    "COLT45",       "COP_AMBIENT",  "COP_DVBYZ",    "CRACK",        "CRIB",
    "DAM_JUMP",     "DANCING",      "DEALER",       "DILDO",        "DODGE",
    "DOZER",        "DRIVEBYS",     "FAT",          "FIGHT_B",      "FIGHT_C",
    "FIGHT_D",      "FIGHT_E",      "FINALE",       "FINALE2",      "FLAME",
    "Flowers",      "FOOD",         "Freeweights",  "GANGS",        "GHANDS",
    "GHETTO_DB",    "goggles",      "GRAFFITI",     "GRAVEYARD",    "GRENADE",
    "GYMNASIUM",    "HAIRCUTS",     "HEIST9",       "INT_HOUSE",    "INT_OFFICE",
    "INT_SHOP",     "JST_BUISNESS", "KART",         "KISSING",      "KNIFE",
    "LAPDAN1",      "LAPDAN2",      "LAPDAN3",      "LOWRIDER",     "MD_CHASE",
    "MD_END",       "MEDIC",        "MISC",         "MTB",          "MUSCULAR",
    "NEVADA",       "ON_LOOKERS",   "OTB",          "PARACHUTE",    "PARK",
    "PAULNMAC",     "PED",          "PLAYER_DVBYS", "PLAYIDLES",    "POLICE",
    "POOL",         "POOR",         "PYTHON",       "QUAD",         "QUAD_DBZ",
    "RAPPING",      "RIFLE",        "RIOT",         "ROB_BANK",     "ROCKET",
    "RUSTLER",      "RYDER",        "SCRATCHING",   "SHAMAL",       "SHOP",
    "SHOTGUN",      "SILENCED",     "SKATE",        "SMOKING",      "SNIPER",
    "SPRAYCAN",     "STRIP",        "SUNBATHE",     "SWAT",         "SWEET",
    "SWIM",         "SWORD",        "TANK",         "TATTOOS",      "TEC",
    "TRAIN",        "TRUCK",        "UZI",          "VAN",          "VENDING",
    "VORTEX",       "WAYFARER",     "WEAPONS",      "WUZI"
};
static ANIM_iTimer[MAX_PLAYERS][sizeof(ANIM_LIST)];

public OnPlayerConnect(playerid)
{
	for (new i; i < sizeof(ANIM_LIST); i++)
	{
    	ANIM_iTimer[playerid][i] = -1;
	}

	#if defined ANIM_OnPlayerConnect
		return ANIM_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect ANIM_OnPlayerConnect
#if defined ANIM_OnPlayerConnect
	forward ANIM_OnPlayerConnect(playerid);
#endif

stock ANIM_ClearAnimations(playerid, forcesync = 0)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
    	return 0;
	}
	
	if (IsPlayerInAnyVehicle(playerid))
	{
		return ApplyAnimation(playerid, "PED", "CAR_SIT", 4.0, 0, 0, 0, 0, 1, forcesync);
	}
	
	new ret = ClearAnimations(playerid, forcesync);
	ApplyAnimation(playerid, "PED", "IDLE_STANCE", 4.0, 0, 0, 0, 0, 1, forcesync);
	ApplyAnimation(playerid, "PED", "IDLE_CHAT", 4.0, 0, 0, 0, 0, 1, forcesync);
	ApplyAnimation(playerid, "PED", "WALK_PLAYER", 4.0, 0, 0, 0, 0, 1, forcesync);
	return ret;
}
#if defined _ALS_ClearAnimations
    #undef ClearAnimations
#else
    #define _ALS_ClearAnimations
#endif
#define ClearAnimations ANIM_ClearAnimations

stock ANIM_ApplyAnimation(playerid, animlib[], animname[], Float:fDelta = 4.1, loop = 0, lockx = 0, locky = 0, freeze = 0, time = 1, forcesync = 0, delay = 250)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
    {
    	return 0;
	}
	
	if (!animlib[0] || !animname[0])
	{
		return 0;
	}
	
	for (new i; i < sizeof(ANIM_LIST); i ++)
	{
		if (!strcmp(animlib, ANIM_LIST[i], true))
	    {
		    if (ANIM_iTimer[playerid][i] == -1)
			{
				ANIM_iTimer[playerid][i] = SetTimerEx("ANIM_OnPreload", delay, false, "issfiiiiii", playerid, animlib, animname, fDelta, loop, lockx, locky, freeze, time, forcesync);
				CallRemoteFunction("ANIM_OnProcessed", "iii", playerid, i, ANIM_iTimer[playerid][i]);
			}
	        return ApplyAnimation(playerid, animlib, animname, fDelta, loop, lockx, locky, freeze, time, forcesync);
	    }
    }
	return 0;
}
#if defined _ALS_ApplyAnimation
  	#undef ApplyAnimation
#else
  	#define _ALS_ApplyAnimation
#endif
#define ApplyAnimation ANIM_ApplyAnimation

forward ANIM_OnPreload(playerid, animlib[], animname[], Float:fDelta, loop, lockx, locky, freeze, time, forcesync);
public 	ANIM_OnPreload(playerid, animlib[], animname[], Float:fDelta, loop, lockx, locky, freeze, time, forcesync)
{
    ANIM_ApplyAnimation(playerid, animlib, animname, fDelta, loop, lockx, locky, freeze, time, forcesync);
}

forward ANIM_OnProcessed(playerid, i, set);
public	ANIM_OnProcessed(playerid, i, set)
{
    ANIM_iTimer[playerid][i] = set;
}
