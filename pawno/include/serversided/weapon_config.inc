#if defined ss@weapon_config_included
	#endinput
#endif
#define ss@weapon_config_included

/*
CALLBACKS:
forward OnPlayerKnife(playerid, issuerid);
*/

/*
FUNCTIONS:
native SetWeaponName(weaponid, const name[]);
native Float:GetWeaponRange(weaponid);
native GetWeaponRateOfFire(weaponid);
native SetPlayerDamage(playerid, weaponid, Float:damage);
native Float:GetPlayerDamage(playerid, weaponid);
native GetPlayerSkillLevel(playerid, skill);
native PlayerDeath(playerid, killerid, weaponid, bodypart, respawn_delay);
native SetPlayerRespawnDelay(playerid, delay);
native GetPlayerRespawnDelay(playerid);
*/

#include "script_init"
#include "vending_machines"
#include "player_stats"

#if defined OnPlayerKnife
	forward OnPlayerKnife(playerid, issuerid);
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

#define MAX_WEAPONS \
	57

#define MAX_SKILLS \
	11

static sWeaponName[MAX_WEAPONS][35] =
{
	"Fist", // 0
	"Brass knuckles", // 1
	"Golf club", // 2
	"Nightstick", // 3
	"Knife", // 4
	"Bat", // 5
	"Shovel", // 6
	"Pool cue", // 7
	"Katana", // 8
	"Chainsaw", // 9
	"Purple dildo", // 10
	"Dildo", // 11
	"Vibrator", // 12
	"Vibrator", // 13
	"Flowers", // 14
	"Cane", // 15
	"Grenade", // 16
	"Tear gas", // 17
	"Molotov", // 18
	"Vehicle M4", // 19
	"Vehicle Minigun", // 20
	"", // 21
	"Colt 45", // 22
	"Silenced pistol", // 23
	"Deagle", // 24
	"Shotgun", // 25
	"Sawn-off shotgun", // 26
	"Combat shotgun", // 27
	"Mac-10", // 28
	"MP5", // 29
	"AK-47", // 30
	"M4", // 31
	"Tec-9", // 32
	"Cuntgun", // 33
	"Sniper", // 34
	"Rocket launcher", // 35
	"Heat seeking RPG", // 36
	"Flamethrower", // 37
	"Minigun", // 38
	"Satchel", // 39
	"Detonator", // 40
	"Spraycan", // 41
	"Fire extinguisher", // 42
	"Camera", // 43
	"Night vision goggles", // 44
	"Infrared goggles", // 45
	"Parachute", // 46
	"Fake pistol", // 47
	"Pistol whip", // 48
	"Vehicle", // 49
	"Helicopter blades", // 50
	"Explosion", // 51
	"Car parking", // 52
	"Drowning", // 53
	"Collision", // 54
	"Unknown" // 55
};

static const Float:fWeaponDamage[MAX_WEAPONS] =
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

static const Float:fWeaponRange[MAX_WEAPONS] =
{
	15.0, // 0 - Fist
	15.0, // 1 - Brass knuckles
	15.0, // 2 - Golf club
	15.0, // 3 - Nitestick
	15.0, // 4 - Knife
	15.0, // 5 - Bat
	15.0, // 6 - Shovel
	15.0, // 7 - Pool cue
	15.0, // 8 - Katana
	15.0, // 9 - Chainsaw
	15.0, // 10 - Dildo
	15.0, // 11 - Dildo 2
	15.0, // 12 - Vibrator
	15.0, // 13 - Vibrator 2
	15.0, // 14 - Flowers
	15.0, // 15 - Cane
	0.0, // 16 - Grenade
	0.0, // 17 - Teargas
	0.0, // 18 - Molotov
	90.0, // 19 - Vehicle M4 (custom)
	75.0, // 20 - Vehicle minigun (custom)
	0.0, // 21
	35.0, // 22 - Colt 45
	35.0, // 23 - Silenced
	35.0, // 24 - Deagle
	40.0, // 25 - Shotgun
	35.0, // 26 - Sawed-off
	40.0, // 27 - Spas
	35.0, // 28 - UZI
	45.0, // 29 - MP5
	70.0, // 30 - AK47
	90.0, // 31 - M4
	35.0, // 32 - Tec9
	100.0, // 33 - Cuntgun
	320.0, // 34 - Sniper
	0.0, // 35 - Rocket launcher
	0.0, // 36 - Heatseeker
	0.0, // 37 - Flamethrower
	75.0,  // 38 - Minigun
	0.0, // 39 - Satchel
	0.0, // 40 - Detonator
	15.0, // 41 - Spraycan
	15.0, // 42 - Fire extinguisher
	0.0, // 43 - Camera
	0.0, // 44 - Night vision
	0.0, // 45 - Infrared
	0.0, // 46 - Parachute
	0.0, // 47 - Fake pistol
	15.0, // 48 - Pistol whip (custom)
	0.0, // 49 - Vehicle
	25.0, // 50 - Helicopter blades
	0.0, // 51 - Explosion
	25.0, // 52 - Car park (custom)
	0.0, // 53 - Drowning
	0.0, // 54 - Splat
	0.0 // 55 - Unknown
};

static const iWeaponFireRate[MAX_WEAPONS] =
{
	250, // 0 - Fist
	250, // 1 - Brass knuckles
	250, // 2 - Golf club
	250, // 3 - Nitestick
	250, // 4 - Knife
	250, // 5 - Bat
	250, // 6 - Shovel
	250, // 7 - Pool cue
	250, // 8 - Katana
	30, // 9 - Chainsaw
	250, // 10 - Dildo
	250, // 11 - Dildo 2
	250, // 12 - Vibrator
	250, // 13 - Vibrator 2
	250, // 14 - Flowers
	250, // 15 - Cane
	0, // 16 - Grenade
	0, // 17 - Teargas
	0, // 18 - Molotov
	20, // 19 - Vehicle M4 (custom)
	20, // 20 - Vehicle minigun (custom)
	0, // 21
	160, // 22 - Colt 45
	120, // 23 - Silenced
	120, // 24 - Deagle
	800, // 25 - Shotgun
	120, // 26 - Sawed-off
	120, // 27 - Spas
	50, // 28 - UZI
	90, // 29 - MP5
	90, // 30 - AK47
	90, // 31 - M4
	70, // 32 - Tec9
	800, // 33 - Cuntgun
	900, // 34 - Sniper
	0, // 35 - Rocket launcher
	0, // 36 - Heatseeker
	0, // 37 - Flamethrower
	20, // 38 - Minigun
	0, // 39 - Satchel
	0, // 40 - Detonator
	10, // 41 - Spraycan
	10, // 42 - Fire extinguisher
	0, // 43 - Camera
	0, // 44 - Night vision
	0, // 45 - Infrared
	0, // 46 - Parachute
	0, // 47 - Fake pistol
	400, // 48 - Pistol whip (custom)
	0, // 49 - Vehicle
	0, // 50 - Helicopter blades
	0, // 51 - Explosion
	0, // 52 - Car park (custom)
	0, // 53 - Drowning
	0, // 54 - Splat
	0 // 55 - Unknown
};

static Float:fPlayerWeaponDamage[MAX_PLAYERS][MAX_WEAPONS];
static iPlayerWeaponSkill[MAX_PLAYERS][MAX_SKILLS];

static iPlayerLastShotType[MAX_PLAYERS];
static iPlayerLastShotId[MAX_PLAYERS];
static bool:iPlayerLastShotReturn[MAX_PLAYERS];

static iPlayerDeathTimer[MAX_PLAYERS];
static iPlayerRespawnDelay[MAX_PLAYERS];

static Float:WC_AngleBetweenPoints(Float:x1, Float:y1, Float:x2, Float:y2)
{
	return -(90.0 - atan2(y1 - y2, x1 - x2));
}

static WC_SetPlayerFacePlayer(playerid, targetid, opposite = false, forcesync = true)
{
	new Float:x1, Float:y1, Float:z1;
	new Float:x2, Float:y2, Float:z2;

	GetPlayerPos(playerid, x1, y1, z1);
	GetPlayerPos(targetid, x2, y2, z2);
	new Float:angle = WC_AngleBetweenPoints(x2, y2, x1, y1);

	if (opposite)
	{
		angle += 180.0;
		if (angle > 360.0) angle -= 360.0;
	}

	if (angle < 0.0)
		angle += 360.0;
	if (angle > 360.0)
		angle -= 360.0;

	SetPlayerFacingAngle(playerid, angle);

	if (forcesync)
	{
		SetFakeFacingAngle(playerid, angle);
		WC_UpdateSyncData(playerid);
	}
}

static WC_IsPlayerBehindPlayer(playerid, targetid, Float:diff = 90.0)
{
	new Float:x1, Float:y1, Float:z1;
	new Float:x2, Float:y2, Float:z2;
	new Float:ang, Float:angdiff;

	GetPlayerPos(playerid, x1, y1, z1);
	GetPlayerPos(targetid, x2, y2, z2);
	GetPlayerFacingAngle(targetid, ang);

	angdiff = WC_AngleBetweenPoints(x1, y1, x2, y2);

	if (angdiff < 0.0)
		angdiff += 360.0;
	if (angdiff > 360.0)
		angdiff -= 360.0;

	ang = ang - angdiff;

	if (ang > 180.0)
		ang -= 360.0;
	if (ang < -180.0)
		ang += 360.0;

	return floatabs(ang) > diff;
}

static WC_ProcessDamage(&playerid, &issuerid, &Float:amount, &weaponid, &bodypart) // Slice's weapon-config ProcessDamage
{
	printf("'%i' damages '%i' with weapon '%i' at bodypart '%i' with amount of '%f'.", issuerid, playerid, weaponid, bodypart, amount);

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

	if (issuerid == INVALID_PLAYER_ID)
	{
	    switch (weaponid)
	    {
	        case WEAPON_COLT45..WEAPON_SNIPER,
				 WEAPON_MINIGUN,
 				 WEAPON_UNARMED..WEAPON_CANE,
		  		 WEAPON_PISTOLWHIP:
			{
			    return 0;
			}
	    }
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

	if (((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP) && IsPlayerInAnyVehicle(issuerid))
		return 0;

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
					amount = fPlayerWeaponDamage[playerid][weaponid];
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
			if (amount > fWeaponDamage[weaponid])
				amount = 1.0;
			else
				amount /= fWeaponDamage[weaponid];

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
			amount = (amount / fWeaponDamage[weaponid]) * fPlayerWeaponDamage[(issuerid == INVALID_PLAYER_ID) ? (playerid) : (issuerid)][weaponid];
		}

		case WEAPON_MOLTOV,
			 WEAPON_FLAMETHROWER,
			 WEAPON_CARPARK,
			 WEAPON_VEHICLE:
		{
		    amount = fPlayerWeaponDamage[(issuerid == INVALID_PLAYER_ID) ? (playerid) : (issuerid)][weaponid];
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
	 	    new Float:bullets = amount / fWeaponDamage[weaponid];
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
	 	    new Float:bullets = amount / fWeaponDamage[weaponid];
			if (15.0 - bullets < -0.05)
				bullets = 1.0;

			new Float:f = floatfract(bullets);
			if (f > 0.01 && f < 0.99)
				bullets = 1.0;

	 	    amount = fPlayerWeaponDamage[issuerid][weaponid] * bullets;
		}
	}
	return 1;
}

static WC_DamagePlayer(playerid, Float:amount, bool:ignore_armour)
{
	if (ignore_armour)
	{
		new Float:health;
		GetPlayerHealth(playerid, health);

	    if (amount >= health)
	    	health = 0.0;
		else
	    	health -= amount;

		SetPlayerHealth(playerid, health);
	}
	else
	{
		new Float:health,
			Float:armour;
		GetPlayerHealth(playerid, health);
		GetPlayerArmour(playerid, armour);

		armour -= amount;
	    if (armour < 0.0)
			health += armour;

	    if (armour <= 0.0)
	     	armour = 0.0;
	    if (health <= 0.0)
	     	health = 0.0;

		SetPlayerHealth(playerid, health);
		SetPlayerArmour(playerid, armour);
	}

	WC_UpdateSyncData(playerid);
}

static WC_UpdateSyncData(playerid)
{
	if (!IsPlayerConnected(playerid) || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT)
		return;

	for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
	{
		if (i != playerid && IsPlayerConnected(i) && IsPlayerStreamedIn(playerid, i))
			SendLastSyncData(playerid, i);
	}
}

public OnScriptInit()
{
	if (IS_GAMEMODE)
	{
		for (new i; i < MAX_WEAPONS; i++)
		{
		    for (new x; x < MAX_PLAYERS; x++)
		    {
				fPlayerWeaponDamage[x][i] = fWeaponDamage[i];
			}
		}
	}

	#if defined WC_OnScriptInit
		return WC_OnScriptInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnScriptInit
	#undef OnScriptInit
#else
	#define _ALS_OnScriptInit
#endif
#define OnScriptInit WC_OnScriptInit
#if defined WC_OnScriptInit
	forward WC_OnScriptInit();
#endif

public OnPlayerConnect(playerid)
{
    for (new i; i < MAX_SKILLS; i++)
	{
	    iPlayerWeaponSkill[playerid][i] = 1000;
	}

	iPlayerDeathTimer[playerid] = -1;
	iPlayerRespawnDelay[playerid] = 3500;

	FreezeSyncData(playerid, false);
	SetFakeFacingAngle(playerid, _);

	#if defined WC_OnPlayerConnect
		return WC_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect WC_OnPlayerConnect
#if defined WC_OnPlayerConnect
	forward WC_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
    if (iPlayerDeathTimer[playerid] != -1)
    {
        KillTimer(iPlayerDeathTimer[playerid]);
    }

    #if defined WC_OnPlayerDisconnect
       	return WC_OnPlayerDisconnect(playerid, reason);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect WC_OnPlayerDisconnect
#if defined WC_OnPlayerDisconnect
    forward WC_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerRequestClass(playerid, classid)
{
    if (iPlayerDeathTimer[playerid] != -1)
    {
        KillTimer(iPlayerDeathTimer[playerid]);
    }

    FreezeSyncData(playerid, false);

    #if defined WC_OnPlayerRequestClass
       	return WC_OnPlayerRequestClass(playerid, classid);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerRequestClass
    #undef OnPlayerRequestClass
#else
    #define _ALS_OnPlayerRequestClass
#endif
#define OnPlayerRequestClass WC_OnPlayerRequestClass
#if defined WC_OnPlayerRequestClass
    forward WC_OnPlayerRequestClass(playerid, classid);
#endif

public OnPlayerSpawn(playerid)
{
    if (iPlayerDeathTimer[playerid] != -1)
    {
        KillTimer(iPlayerDeathTimer[playerid]);
        iPlayerDeathTimer[playerid] = -1;
    }

	WC_UpdateSyncData(playerid);
	FreezeSyncData(playerid, false);
	SetFakeFacingAngle(playerid, _);

	#if defined WC_OnPlayerSpawn
		return WC_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn WC_OnPlayerSpawn
#if defined WC_OnPlayerSpawn
	forward WC_OnPlayerSpawn(playerid);
#endif

public OnPlayerUseVendingMachine(playerid, machineid)
{
	if (iPlayerDeathTimer[playerid] != -1)
    	return 0;

	#if defined WC_OnPlayerUseVendingMachine
		return WC_OnPlayerUseVendingMachine(playerid, machineid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerUseVendingMachine
    #undef OnPlayerUseVendingMachine
#else
    #define _ALS_OnPlayerUseVendingMachine
#endif
#define OnPlayerUseVendingMachine WC_OnPlayerUseVendingMachine
#if defined WC_OnPlayerUseVendingMachine
	forward WC_OnPlayerUseVendingMachine(playerid, machineid);
#endif

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if (!((WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) || weaponid == WEAPON_MINIGUN))
		return 0;

	iPlayerLastShotType[playerid] = hittype;
	iPlayerLastShotId[playerid] = hitid;
	#if defined WC_OnPlayerWeaponShot
	    iPlayerLastShotReturn[playerid] = bool:WC_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	#else
	    iPlayerLastShotReturn[playerid] = true;
	#endif

	if (hittype == BULLET_HIT_TYPE_PLAYER)
		return 0;

	return iPlayerLastShotReturn[playerid];
}
#if defined _ALS_OnPlayerWeaponShot
    #undef OnPlayerWeaponShot
#else
    #define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot WC_OnPlayerWeaponShot
#if defined WC_OnPlayerWeaponShot
	forward WC_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	if (issuerid != INVALID_PLAYER_ID)
		return 1;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 1;

	if (weaponid == WEAPON_COLLISION && (1061 <= GetPlayerAnimationIndex(playerid) <= 1067))
	{
	    TogglePlayerControllable(playerid, false);
		ClearAnimations(playerid);
	    TogglePlayerControllable(playerid, true);
		return 1;
	}

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
			 WEAPON_DROWN,
			 WEAPON_COLLISION:
    	{}

    	default:
    	{
    	    return 1;
    	}
	}

	if (!WC_ProcessDamage(playerid, issuerid, amount, weaponid, bodypart))
		return 1;

	switch (weaponid)
	{
	    case WEAPON_VEHICLE,
             WEAPON_CARPARK,
			 WEAPON_DROWN,
			 WEAPON_COLLISION:
		{
			WC_DamagePlayer(playerid, amount, true);
		}

		default:
		{
			WC_DamagePlayer(playerid, amount, false);
		}
	}

	new Float:health;
	GetPlayerHealth(playerid, health);
	if (health <= 0.0)
	{
		PlayerDeath(playerid, issuerid, weaponid, bodypart, GetPlayerRespawnDelay(playerid));
	}

	#if defined WC_OnPlayerTakeDamage
		return WC_OnPlayerTakeDamage(playerid, issuerid, amount, weaponid, bodypart);
	#else
		return 0;
	#endif
}
#if defined _ALS_OnPlayerTakeDamage
    #undef OnPlayerTakeDamage
#else
    #define _ALS_OnPlayerTakeDamage
#endif
#define OnPlayerTakeDamage WC_OnPlayerTakeDamage
#if defined WC_OnPlayerTakeDamage
	forward WC_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
    #if defined WC_OnPlayerGiveDamage
		WC_OnPlayerGiveDamage(playerid, damagedid, amount, weaponid, bodypart);
	#endif

    if ((WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) || weaponid == WEAPON_MINIGUN)
    {
		new Float:origin[3],
			Float:hit[3],
			Float:length;
		GetPlayerLastShotVectors(playerid, origin[0], origin[1], origin[2], hit[0], hit[1], hit[2]);
		length = VectorSize(origin[0] - hit[0], origin[1] - hit[1], origin[2] - hit[2]);
		if (length > fWeaponRange[weaponid])
			return 1;
	}
	else
	{
	    new Float:x,
			Float:y,
			Float:z,
			Float:length;
		GetPlayerPos(damagedid, x, y, z);
		length = GetPlayerDistanceFromPoint(playerid, x, y, z);
	    if (fWeaponRange[weaponid] != 0.0 && length > fWeaponRange[weaponid])
			return 1;
	}

	if (iPlayerDeathTimer[playerid] != -1 || iPlayerDeathTimer[damagedid] != -1)
	    return 1;

    if (weaponid == WEAPON_KNIFE && _:amount == _:0.0)
	{
	    new bool:ret = true;
		#if defined OnPlayerKnife
		    ret = bool:OnPlayerKnife(damagedid, playerid);
  		#endif

		if (ret && !HasSameTeam(playerid, damagedid))
		{
			SetPlayerHealth(damagedid, 0.0);

		   	ret = true;
			#if defined OnPlayerDeath
			    ret = bool:OnPlayerDeath(damagedid, playerid, weaponid);
			#endif

			PlayerDeath(damagedid, playerid, WEAPON_KNIFE, 9, ((ret) ? (GetPlayerRespawnDelay(playerid)) : (0)));
		}
	}

	if (HasSameTeam(playerid, damagedid))
		return 1;

	if (!WC_ProcessDamage(damagedid, playerid, amount, weaponid, bodypart))
		return 1;

	if ((WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) || weaponid == WEAPON_MINIGUN)
    {
        if (iPlayerLastShotType[playerid] != BULLET_HIT_TYPE_PLAYER || iPlayerLastShotId[playerid] != damagedid || !iPlayerLastShotReturn[playerid])
            return 0;
    }

    WC_DamagePlayer(damagedid, amount, false);

	new Float:health;
	GetPlayerHealth(damagedid, health);
	if (health <= 0.0)
	{
		PlayerDeath(damagedid, playerid, weaponid, bodypart, GetPlayerRespawnDelay(damagedid));
	}

	#if defined WC_OnPlayerTakeDamage
		WC_OnPlayerTakeDamage(damagedid, playerid, amount, weaponid, bodypart);
	#endif
	return 0;
}
#if defined _ALS_OnPlayerGiveDamage
    #undef OnPlayerGiveDamage
#else
    #define _ALS_OnPlayerGiveDamage
#endif
#define OnPlayerGiveDamage WC_OnPlayerGiveDamage
#if defined WC_OnPlayerGiveDamage
	forward WC_OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart);
#endif

public OnPlayerDeath(playerid, killerid, reason)
{
	if (killerid != INVALID_PLAYER_ID && !IsPlayerStreamedIn(killerid, playerid))
	{
		killerid = INVALID_PLAYER_ID;
	}

	if (iPlayerDeathTimer[playerid] != -1)
	{
		KillTimer(iPlayerDeathTimer[playerid]);
		return 1;
	}

	if (reason < 0 || reason > 55)
	{
		reason = 55;
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid)
	{
		reason = WEAPON_EXPLOSION;
		killerid = INVALID_PLAYER_ID;

		for (new i, j = GetPlayerPoolSize(); i <= j; i++)
		{
		    if (i != playerid && IsPlayerConnected(i))
		    {
				if (iPlayerLastShotType[i] == BULLET_HIT_TYPE_VEHICLE && iPlayerLastShotId[i] == vehicleid && iPlayerLastShotReturn[i])
				{
					killerid = i;
					break;
				}
			}
		}
	}

	new Float:amount = 0.0;
	new bodypart = 0;

	if (reason < 0 || reason > WEAPON_UNKNOWN)
		reason = WEAPON_UNKNOWN;
	else if (reason == WEAPON_PARACHUTE)
		reason = WEAPON_COLLISION;

  	new Float:health,
		Float:armour;
	GetPlayerHealth(playerid, health);
 	GetPlayerArmour(playerid, armour);

	if (amount <= 0.0)
		amount = health + armour;

	#if defined WC_OnPlayerTakeDamage
		WC_OnPlayerTakeDamage(playerid, killerid, amount, reason, bodypart);
	#else
	    #pragma unused bodypart
	#endif

	#if defined WC_OnPlayerDeath
		WC_OnPlayerDeath(playerid, killerid, reason);
	#endif

	WC_UpdateSyncData(playerid);
	return 1;
}
#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath WC_OnPlayerDeath
#if defined WC_OnPlayerDeath
	forward WC_OnPlayerDeath(playerid, killerid, reason);
#endif

public OnPlayerStreamIn(playerid, forplayerid)
{
	if (iPlayerDeathTimer[playerid] != -1)
	{
		SendLastSyncData(playerid, forplayerid, (0x2e040000 + 1150));
	}

	#if defined WC_OnPlayerStreamIn
		return WC_OnPlayerStreamIn(playerid, forplayerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerStreamIn
	#undef OnPlayerStreamIn
#else
	#define _ALS_OnPlayerStreamIn
#endif
#define OnPlayerStreamIn WC_OnPlayerStreamIn
#if defined WC_OnPlayerStreamIn
	forward WC_OnPlayerStreamIn(playerid, forplayerid);
#endif

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (iPlayerDeathTimer[playerid] != - 1 && (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER)) 
	{
		TogglePlayerControllable(playerid, false);
	}

	if (oldstate == PLAYER_STATE_DRIVER || oldstate == PLAYER_STATE_PASSENGER)
	{
		if (newstate == PLAYER_STATE_ONFOOT)
		{
			new Float:vx, Float:vy, Float:vz;
			GetPlayerVelocity(playerid, vx, vy, vz);

			if (((vx*vx) + (vy*vy) + (vz*vz)) <= 0.05)
			{
				for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++)
				{
					if (i != playerid && IsPlayerConnected(i) && IsPlayerStreamedIn(playerid, i))
					{
						SendLastSyncData(playerid, i);
						ClearAnimationsForPlayer(playerid, i);
					}
				}
			}
		}
	}
	
	#if defined WC_OnPlayerStateChange
		return WC_OnPlayerStateChange(playerid, newstate, oldstate);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerStateChange
	#undef OnPlayerStateChange
#else
	#define _ALS_OnPlayerStateChange
#endif
#define OnPlayerStateChange WC_OnPlayerStateChange
#if defined WC_OnPlayerStateChange
	forward WC_OnPlayerStateChange(playerid, newstate, oldstate);
#endif

stock SetWeaponName(weaponid, const name[])
{
	if (weaponid < 0 || weaponid >= MAX_WEAPONS)
	    return 0;

    format(sWeaponName[weaponid], sizeof(sWeaponName[]), name);
    return 1;
}

stock WC_GetWeaponName(weaponid, dest[], maxlen)
{
	if (weaponid < 0 || weaponid >= MAX_WEAPONS)
	    return 0;

	strcat((dest[0] = EOS, dest), sWeaponName[weaponid], maxlen);
	return 1;
}
#if defined _ALS_GetWeaponName
    #undef GetWeaponName
#else
    #define _ALS_GetWeaponName
#endif
#define GetWeaponName WC_GetWeaponName

stock Float:GetWeaponRange(weaponid)
{
	if (weaponid < 0 || weaponid >= MAX_WEAPONS)
	    return 0;

	return iWeaponRange[weaponid];
}

stock GetWeaponRateOfFire(weaponid)
{
	if (weaponid < 0 || weaponid >= MAX_WEAPONS)
	    return 0;

	return iWeaponFireRate[weaponid];
}

stock SetPlayerDamage(playerid, weaponid, Float:damage)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	    return 0;

	if (weaponid < 0 || weaponid >= MAX_WEAPONS)
	    return 0;

    fPlayerWeaponDamage[playerid][weaponid] = damage;
	return 1;
}

stock Float:GetPlayerDamage(playerid, weaponid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	    return 0;

	if (weaponid < 0 || weaponid >= MAX_WEAPONS)
	    return 0;

    return fPlayerWeaponDamage[playerid][weaponid];
}

stock WC_SetPlayerSkillLevel(playerid, skill, level)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	    return 0;

	if (skill < 0 || skill >= MAX_SKILLS)
	    return 0;

    iPlayerWeaponSkill[playerid][skill] = level;
	return SetPlayerSkillLevel(playerid, skill, level);
}
#if defined _ALS_SetPlayerSkillLevel
    #undef SetPlayerSkillLevel
#else
    #define _ALS_SetPlayerSkillLevel
#endif
#define SetPlayerSkillLevel WC_SetPlayerSkillLevel

stock GetPlayerSkillLevel(playerid, skill)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	    return 0;

	if (skill < 0 || skill >= MAX_SKILLS)
	    return 0;

    return iPlayerWeaponSkill[playerid][skill];
}

stock PlayerDeath(playerid, killerid, weaponid, bodypart, respawn_delay)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	    return;

	if (GetPlayerState(playerid) != PLAYER_STATE_SPAWNED)
	    return;

	if (iPlayerDeathTimer[playerid] != -1)
 		return;

	if (killerid != INVALID_PLAYER_ID && (killerid < 0 || killerid >= MAX_PLAYERS))
	    return;
	else
	{
		if (GetPlayerState(killerid) != PLAYER_STATE_SPAWNED)
		    return;

		if (iPlayerDeathTimer[killerid] != -1)
		    return;
	}

	if (weaponid < 0 || weaponid >= MAX_WEAPONS)
	{
	    weaponid = WEAPON_UNKNOWN;
	}

	new action = GetPlayerSpecialAction(playerid);
	if (action != SPECIAL_ACTION_DUCK)
	{
		if (action == SPECIAL_ACTION_USEJETPACK)
			ClearAnimations(playerid);

		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

		if (action == SPECIAL_ACTION_USEJETPACK)
		{
			new Float:vx, Float:vy, Float:vz;
			GetPlayerVelocity(playerid, vx, vy, vz);
			SetPlayerVelocity(playerid, vx, vy, vz);
		}
	}

    new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid)
	{
		new modelid = GetVehicleModel(vehicleid);
		new seat = GetPlayerVehicleSeat(playerid);

		TogglePlayerControllable(playerid, false);

		switch (modelid)
		{
			case 509, 481, 510, 462, 448, 581, 522,
			     461, 521, 523, 463, 586, 468, 471:
		 	{
				new Float:vx, Float:vy, Float:vz;
				GetVehicleVelocity(vehicleid, vx, vy, vz);
				ApplyAnimation(playerid, "PED", ((((vx*vx) + (vy*vy) + (vz*vz)) >= 0.4) ? ("BIKE_fallR") : ("BIKE_fall_off")), 4.1, 0, 0, 0, 1, 0, 1);
			}

			default:
			{
				ApplyAnimation(playerid, "PED", ((seat & 1) ? ("CAR_dead_LHS") : ("CAR_dead_RHS")), 4.1, 0, 0, 0, 1, 0, 1);
			}
		}
	}
	else if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_USEJETPACK)
	{
		ApplyAnimation(playerid, "PED", "KO_skid_back", 4.1, 0, 0, 0, 1, 0, 1);
	}
	else
	{
		new anim = GetPlayerAnimationIndex(playerid);
		if (anim == 1250 || (1538 <= anim <= 1544) || weaponid == WEAPON_DROWN)
		{
			ApplyAnimation(playerid, "PED", "Drown", 4.1, 0, 0, 0, 1, 0, 1);
		}
		else if (1195 <= anim <= 1198)
		{
			ApplyAnimation(playerid, "PED", "KO_skid_back", 4.1, 0, 0, 0, 1, 0, 1);
		}
		else if (WEAPON_SHOTGUN <= weaponid <= WEAPON_SHOTGSPA)
		{
		    if (killerid == INVALID_PLAYER_ID)
		    {
		        ApplyAnimation(playerid, "PED", "BIKE_fall_off", 4.1, 0, 0, 0, 1, 0, 1);
			}
			else
			{
				if (WC_IsPlayerBehindPlayer(killerid, playerid))
				{
					WC_SetPlayerFacePlayer(playerid, killerid, true);
					ApplyAnimation(playerid, "PED", "KO_shot_front", 4.1, 0, 0, 0, 1, 0, 1);
				}
				else
				{
					WC_SetPlayerFacePlayer(playerid, killerid);
					ApplyAnimation(playerid, "PED", "BIKE_fall_off", 4.1, 0, 0, 0, 1, 0, 1);
				}
			}
		}
		else if (WEAPON_RIFLE <= weaponid <= WEAPON_SNIPER)
		{
			if (bodypart == 9)
			{
				ApplyAnimation(playerid, "PED", "KO_shot_face", 4.1, 0, 0, 0, 1, 0, 1);
			}
			else
			{

			    if (killerid != INVALID_PLAYER_ID && WC_IsPlayerBehindPlayer(killerid, playerid))
			    {
					ApplyAnimation(playerid, "PED", "KO_shot_front", 4.1, 0, 0, 0, 1, 0, 1);
				}
				else
				{
					ApplyAnimation(playerid, "PED", "KO_shot_stom", 4.1, 0, 0, 0, 1, 0, 1);
				}
		    }
		}
		else if ((WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) || weaponid == WEAPON_MINIGUN)
		{
			if (bodypart == 9)
			{
				ApplyAnimation(playerid, "PED", "KO_shot_face", 4.1, 0, 0, 0, 1, 0, 1);
			}
			else
			{
				ApplyAnimation(playerid, "PED", "KO_shot_front", 4.1, 0, 0, 0, 1, 0, 1);
			}
		}
		else if (weaponid == WEAPON_KNIFE)
		{
		    if (bodypart == 9)
		    {
				new Float:a;
				GetPlayerFacingAngle(playerid, a);
				SetPlayerVelocity(playerid, 0.0, 0.0, 0.0);
		        ClearAnimations(playerid, 1);
				ApplyAnimation(playerid, "KNIFE", "KILL_Knife_Ped_Damage", 4.1, 0, 1, 1, 0, 5200);

				if (killerid != INVALID_PLAYER_ID)
				{
					SetPlayerFacingAngle(killerid, a);
					SetPlayerVelocity(killerid, 0.0, 0.0, 0.0);
					ApplyAnimation(killerid, "KNIFE", "KILL_Knife_Player", 4.1, 0, 1, 1, 0, 1800, ((747 < GetPlayerAnimationIndex(killerid) > 748) ? (1) : (2)));
				}
		    }
		    else
		    {
				ApplyAnimation(playerid, "PED", "KO_spin_R", 4.1, 0, 0, 0, 1, 0, 1);
			}
		}
		else if ((WEAPON_UNARMED <= weaponid <= WEAPON_NITESTICK) || (WEAPON_BAT <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP || weaponid == WEAPON_CARPARK)
		{
			ApplyAnimation(playerid, "PED", "KO_skid_front", 4.1, 0, 0, 0, 1, 0, 1);
		}
		else if ((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP || weaponid == WEAPON_CARPARK)
		{
			ApplyAnimation(playerid, "PED", "KO_skid_front", 4.1, 0, 0, 0, 1, 0, 1);
		}
		else if (weaponid == WEAPON_SPRAYCAN || weaponid == WEAPON_FIREEXTINGUISHER)
		{
			ApplyAnimation(playerid, "PED", "KILL_Knife_Ped_Die", 4.1, 0, 0, 0, 1, 0, 1);
		}
		else
		{
			ApplyAnimation(playerid, "PED", "KO_skid_back", 4.1, 0, 0, 0, 1, 0, 1);
		}
	}

	if (respawn_delay > 0)
	{
		iPlayerDeathTimer[playerid] = SetTimerEx("WC_OnPlayerDeathFinished", respawn_delay, false, "i", playerid);
	}

	WC_UpdateSyncData(playerid);
	FreezeSyncData(playerid, true);
}

forward WC_OnPlayerDeathFinished(playerid, killerid, reason);
public WC_OnPlayerDeathFinished(playerid, killerid, reason)
{
	KillTimer(iPlayerDeathTimer[playerid]);
	iPlayerDeathTimer[playerid] = -1;

	WC_UpdateSyncData(playerid);
	SetFakeFacingAngle(playerid, _);
	FreezeSyncData(playerid, false);
	SpawnPlayer(playerid);
}

stock SetPlayerRespawnDelay(playerid, delay)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

    iPlayerRespawnDelay[playerid] = delay;
	return 1;
}

stock GetPlayerRespawnDelay(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

    return iPlayerRespawnDelay[playerid];
}

stock WC_SendDeathMessage(playerid, killerid, reason)
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
#define SendDeathMessage WC_SendDeathMessage

stock WC_SpawnPlayer(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return SpawnPlayer(playerid);
}
#if defined _ALS_SpawnPlayer
    #undef SpawnPlayer
#else
    #define _ALS_SpawnPlayer
#endif
#define SpawnPlayer WC_SpawnPlayer

stock WC_GetPlayerState(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return PLAYER_STATE_WASTED;

	return GetPlayerState(playerid);
}
#if defined _ALS_GetPlayerState
    #undef GetPlayerState
#else
    #define _ALS_GetPlayerState
#endif
#define GetPlayerState WC_GetPlayerState

stock WC_ApplyAnimation(playerid, animlib[], animname[], Float:fDelta, loop, lockx, locky, freeze, time, forcesync = 0)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return ApplyAnimation(playerid, animlib, animname, fDelta, loop, lockx, locky, freeze, time, forcesync);
}
#if defined _ALS_ApplyAnimation
    #undef ApplyAnimation
#else
    #define _ALS_ApplyAnimation
#endif
#define ApplyAnimation WC_ApplyAnimation

stock WC_ClearAnimations(playerid, forcesync = 1)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return ClearAnimations(playerid, forcesync);
}
#if defined _ALS_ClearAnimations
    #undef ClearAnimations
#else
    #define _ALS_ClearAnimations
#endif
#define ClearAnimations WC_ClearAnimations

stock WC_TogglePlayerSpectating(playerid, toggle)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (bool:toggle)
	{
		if (iPlayerDeathTimer[playerid] != -1)
		{
			KillTimer(iPlayerDeathTimer[playerid]);
			iPlayerDeathTimer[playerid] = -1;
		}
	}

	return TogglePlayerSpectating(playerid, toggle);
}
#if defined _ALS_TogglePlayerSpectating
    #undef TogglePlayerSpectating
#else
    #define _ALS_TogglePlayerSpectating
#endif
#define TogglePlayerSpectating WC_TogglePlayerSpectating

stock WC_TogglePlayerControllable(playerid, toggle)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return TogglePlayerControllable(playerid, toggle);
}
#if defined _ALS_TogglePlayerControllable
    #undef TogglePlayerControllable
#else
    #define _ALS_TogglePlayerControllable
#endif
#define TogglePlayerControllable WC_TogglePlayerControllable

stock WC_SetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return SetPlayerPos(playerid, x, y, z);
}
#if defined _ALS_SetPlayerPos
    #undef SetPlayerPos
#else
    #define _ALS_SetPlayerPos
#endif
#define SetPlayerPos WC_SetPlayerPos

stock WC_SetPlayerPosFindZ(playerid, Float:x, Float:y, Float:z)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return SetPlayerPosFindZ(playerid, x, y, z);
}
#if defined _ALS_SetPlayerPosFindZ
    #undef SetPlayerPosFindZ
#else
    #define _ALS_SetPlayerPosFindZ
#endif
#define SetPlayerPosFindZ WC_SetPlayerPosFindZ

stock WC_SetPlayerFacingAngle(playerid, Float:ang)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return SetPlayerFacingAngle(playerid, ang);
}
#if defined _ALS_SetPlayerFacingAngle
    #undef SetPlayerFacingAngle
#else
    #define _ALS_SetPlayerFacingAngle
#endif
#define SetPlayerFacingAngle WC_SetPlayerFacingAngle

stock WC_SetPlayerVelocity(playerid, Float:X, Float:Y, Float:Z)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return SetPlayerVelocity(playerid, X, Y, Z);
}
#if defined _ALS_SetPlayerVelocity
    #undef SetPlayerVelocity
#else
    #define _ALS_SetPlayerVelocity
#endif
#define SetPlayerVelocity WC_SetPlayerVelocity

stock WC_PlayerSpectatePlayer(playerid, targetplayerid, mode = SPECTATE_MODE_NORMAL)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || targetplayerid < 0 || targetplayerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	{
		KillTimer(iPlayerDeathTimer[playerid]);
		iPlayerDeathTimer[playerid] = -1;
	}

	return PlayerSpectatePlayer(playerid, targetplayerid, mode);
}
#if defined _ALS_PlayerSpectatePlayer
    #undef PlayerSpectatePlayer
#else
    #define _ALS_PlayerSpectatePlayer
#endif
#define PlayerSpectatePlayer WC_PlayerSpectatePlayer

stock WC_PlayerSpectateVehicle(playerid, targetvehicleid, mode = SPECTATE_MODE_NORMAL)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS || targetvehicleid < 0 || targetvehicleid >= MAX_VEHICLES)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	{
		KillTimer(iPlayerDeathTimer[playerid]);
		iPlayerDeathTimer[playerid] = -1;
	}

	return PlayerSpectateVehicle(playerid, targetvehicleid, mode);
}
#if defined _ALS_PlayerSpectateVehicle
    #undef PlayerSpectateVehicle
#else
    #define _ALS_PlayerSpectateVehicle
#endif
#define PlayerSpectateVehicle WC_PlayerSpectateVehicle

stock WC_SetPlayerSpecialAction(playerid, actionid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return SetPlayerSpecialAction(playerid, actionid);
}
#if defined _ALS_SetPlayerSpecialAction
    #undef SetPlayerSpecialAction
#else
    #define _ALS_SetPlayerSpecialAction
#endif
#define SetPlayerSpecialAction WC_SetPlayerSpecialAction

stock WC_SetPlayerDrunkLevel(playerid, level)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return SetPlayerDrunkLevel(playerid, level);
}
#if defined _ALS_SetPlayerDrunkLevel
    #undef SetPlayerDrunkLevel
#else
    #define _ALS_SetPlayerDrunkLevel
#endif
#define SetPlayerDrunkLevel WC_SetPlayerDrunkLevel

stock WC_SetPlayerSkin(playerid, skin)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return SetPlayerSkin(playerid, skin);
}
#if defined _ALS_SetPlayerSkin
    #undef SetPlayerSkin
#else
    #define _ALS_SetPlayerSkin
#endif
#define SetPlayerSkin WC_SetPlayerSkin

stock WC_PutPlayerInVehicle(playerid, vehicleid, seat)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return PutPlayerInVehicle(playerid, vehicleid, seat);
}
#if defined _ALS_PutPlayerInVehicle
    #undef PutPlayerInVehicle
#else
    #define _ALS_PutPlayerInVehicle
#endif
#define PutPlayerInVehicle WC_PutPlayerInVehicle

stock WC_RemovePlayerFromVehicle(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return RemovePlayerFromVehicle(playerid);
}
#if defined _ALS_RemovePlayerFromVehicle
    #undef RemovePlayerFromVehicle
#else
    #define _ALS_RemovePlayerFromVehicle
#endif
#define RemovePlayerFromVehicle WC_RemovePlayerFromVehicle

stock WC_SetPlayerWorldBounds(playerid, Float:x_max, Float:x_min, Float:y_max, Float:y_min)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return SetPlayerWorldBounds(playerid, x_max, x_min, y_max, y_min);
}
#if defined _ALS_SetPlayerWorldBounds
    #undef SetPlayerWorldBounds
#else
    #define _ALS_SetPlayerWorldBounds
#endif
#define SetPlayerWorldBounds WC_SetPlayerWorldBounds

stock WC_GivePlayerWeapon(playerid, weaponid, ammo)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return GivePlayerWeapon(playerid, weaponid, ammo);
}
#if defined _ALS_GivePlayerWeapon
    #undef GivePlayerWeapon
#else
    #define _ALS_GivePlayerWeapon
#endif
#define GivePlayerWeapon WC_GivePlayerWeapon

stock WC_ResetPlayerWeapons(playerid)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

	return ResetPlayerWeapons(playerid);
}
#if defined _ALS_ResetPlayerWeapons
    #undef ResetPlayerWeapons
#else
    #define _ALS_ResetPlayerWeapons
#endif
#define ResetPlayerWeapons WC_ResetPlayerWeapons

stock WC_SetPlayerAmmo(playerid, weaponid, ammo)
{
    if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (iPlayerDeathTimer[playerid] != -1)
	    return 0;

    return SetPlayerAmmo(playerid, weaponid, ammo);
}
#if defined _ALS_SetPlayerAmmo
    #undef SetPlayerAmmo
#else
    #define _ALS_SetPlayerAmmo
#endif
#define SetPlayerAmmo WC_SetPlayerAmmo

stock WC_SetPlayerHealth(playerid, Float:health)
{
    if (SetPlayerHealth(playerid, health))
    {
		WC_UpdateSyncData(playerid);
		if (health <= 0.0)
		{
		    PlayerDeath(playerid, INVALID_PLAYER_ID, WEAPON_UNKNOWN, 0, GetPlayerRespawnDelay(playerid));
		}
        return 1;
    }
    return 0;
}
#if defined _ALS_SetPlayerHealth
    #undef SetPlayerHealth
#else
    #define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth WC_SetPlayerHealth

stock WC_SetPlayerArmour(playerid, Float:armour)
{
    if (SetPlayerArmour(playerid, armour))
    {
		WC_UpdateSyncData(playerid);
		return 1;
    }
    return 0;
}
#if defined _ALS_SetPlayerArmour
    #undef SetPlayerArmour
#else
    #define _ALS_SetPlayerArmour
#endif
#define SetPlayerArmour WC_SetPlayerArmour
