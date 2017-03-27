#if defined lagcomp_included
	#endinput
#endif
#define lagcomp_included

/*
native SetPlayerWeaponDamage(playerid, weaponid, Float:damage);
native Float:GetPlayerWeaponDamage(playerid, weaponid);
*/

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

static const Float:LC_WEAPON_DAMAGE[] =
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

enum LC_e_LAST_SHOT
{
    LC_e_LAST_SHOT_TYPE,
    LC_e_LAST_SHOT_HITID,
    bool:LC_e_LAST_SHOT_RETURN
};
static LC_ePlayerLastShot[MAX_PLAYERS][LC_e_LAST_SHOT];

enum LC_e_PLAYER_DEATHDATA
{
	bool:LC_e_PLAYER_DEATHDATA_VALID,
    LC_e_PLAYER_DEATHDATA_KILLERID,
    LC_e_PLAYER_DEATHDATA_WEAPONID
};
static LC_ePlayerDeathData[MAX_PLAYERS][LC_e_PLAYER_DEATHDATA];

static Float:LC_fPlayerWeaponDamage[MAX_PLAYERS][sizeof(LC_WEAPON_DAMAGE)];
static LC_iPlayerTeam[MAX_PLAYERS];

static LC_HasSameTeam(playerid, otherid)
{
	if (LC_iPlayerTeam[playerid] == NO_TEAM || LC_iPlayerTeam[otherid] == NO_TEAM)
		return 0;

	return bool:(LC_iPlayerTeam[playerid] == LC_iPlayerTeam[otherid]);
}

static LC_ProcessDamage(&playerid, &issuerid, &Float:amount, &weaponid, &bodypart) // Slice's weapon-config LC_ProcessDamage
{
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
					amount = LC_fPlayerWeaponDamage[playerid][weaponid];
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
		    amount = LC_fPlayerWeaponDamage[issuerid][weaponid];
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
					amount *= LC_fPlayerWeaponDamage[issuerid][weaponid];
				}

				default:
					amount = LC_fPlayerWeaponDamage[issuerid][weaponid];
			}
	    }

	    case WEAPON_COLLISION:
	    {
			if (amount > LC_WEAPON_DAMAGE[weaponid])
				amount = 1.0;
			else
				amount /= LC_WEAPON_DAMAGE[weaponid];

	        amount *= LC_fPlayerWeaponDamage[playerid][weaponid];
	    }

	    case WEAPON_DROWN:
	    {
	    	if (amount > 10.0)
				amount = 10.0;

	        amount *= LC_fPlayerWeaponDamage[playerid][weaponid];
	    }

		case WEAPON_GRENADE,
			 WEAPON_ROCKETLAUNCHER,
			 WEAPON_HEATSEEKER,
			 WEAPON_SATCHEL,
			 WEAPON_EXPLOSION:
		{
			amount = (amount / LC_WEAPON_DAMAGE[weaponid]) * LC_fPlayerWeaponDamage[issuerid][weaponid];
		}

		case WEAPON_MOLTOV,
			 WEAPON_FLAMETHROWER,
			 WEAPON_CARPARK,
			 WEAPON_VEHICLE:
		{
		    amount = LC_fPlayerWeaponDamage[issuerid][weaponid];
		}

		case WEAPON_COLT45..WEAPON_DEAGLE,
			 WEAPON_UZI..WEAPON_SNIPER,
			 WEAPON_MINIGUN,
			 WEAPON_SPRAYCAN,
			 WEAPON_FIREEXTINGUISHER,
			 WEAPON_HELIBLADES:
		{
		    amount = LC_fPlayerWeaponDamage[issuerid][weaponid];
		}

		case WEAPON_SHOTGSPA:
	 	{
	 	    new Float:bullets = amount / LC_WEAPON_DAMAGE[weaponid];
			if (8.0 - bullets < -0.05)
   	 			bullets = 1.0;

			new Float:f = floatfract(bullets);
			if (f > 0.01 && f < 0.99)
				bullets = 1.0;

	 	    amount = LC_fPlayerWeaponDamage[issuerid][weaponid] * bullets;
		}

		case WEAPON_SHOTGUN,
			 WEAPON_SAWEDOFF:
	 	{
	 	    new Float:bullets = amount / LC_WEAPON_DAMAGE[weaponid];
			if (15.0 - bullets < -0.05)
				bullets = 1.0;

			new Float:f = floatfract(bullets);
			if (f > 0.01 && f < 0.99)
				bullets = 1.0;

	 	    amount = LC_fPlayerWeaponDamage[issuerid][weaponid] * bullets;
		}
	}
	return 1;
}

static LC_DamagePlayer(playerid, Float:amount)
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

public OnPlayerConnect(playerid)
{
	LC_iPlayerTeam[playerid] = NO_TEAM;
	SetPlayerTeam(playerid, 655);
	
	for (new i; i < sizeof(LC_WEAPON_DAMAGE); i++)
		SetPlayerWeaponDamage(playerid, i, LC_WEAPON_DAMAGE[i]);

	#if defined LC_OnPlayerConnect
		return LC_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect LC_OnPlayerConnect
#if defined LC_OnPlayerConnect
	forward LC_OnPlayerConnect(playerid);
#endif

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if (!((WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) || weaponid == WEAPON_MINIGUN))
		return 0;

	LC_ePlayerLastShot[playerid][LC_e_LAST_SHOT_TYPE] = hittype;
	LC_ePlayerLastShot[playerid][LC_e_LAST_SHOT_HITID] = hitid;
	#if defined LC_OnPlayerWeaponShot
	    LC_ePlayerLastShot[playerid][LC_e_LAST_SHOT_RETURN] = bool:LC_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	#else
	    LC_ePlayerLastShot[playerid][LC_e_LAST_SHOT_RETURN] = true;
	#endif

	return LC_ePlayerLastShot[playerid][LC_e_LAST_SHOT_RETURN];
}
#if defined _ALS_OnPlayerWeaponShot
    #undef OnPlayerWeaponShot
#else
    #define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot LC_OnPlayerWeaponShot
#if defined LC_OnPlayerWeaponShot
	forward LC_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
	if (issuerid != INVALID_PLAYER_ID)
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

	new Float:health;
	GetPlayerHealth(playerid, health);
	if (health <= 0.0)
	{
	    LC_ePlayerDeathData[playerid][LC_e_PLAYER_DEATHDATA_VALID] = true;
		LC_ePlayerDeathData[playerid][LC_e_PLAYER_DEATHDATA_KILLERID] = INVALID_PLAYER_ID;
		LC_ePlayerDeathData[playerid][LC_e_PLAYER_DEATHDATA_WEAPONID] = weaponid;
	}

	#if defined LC_OnPlayerTakeDamage
		return LC_OnPlayerTakeDamage(playerid, issuerid, amount, weaponid, bodypart);
	#else
		return 0;
	#endif
}
#if defined _ALS_OnPlayerTakeDamage
    #undef OnPlayerTakeDamage
#else
    #define _ALS_OnPlayerTakeDamage
#endif
#define OnPlayerTakeDamage LC_OnPlayerTakeDamage
#if defined LC_OnPlayerTakeDamage
	forward LC_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
    #if defined LC_OnPlayerGiveDamage
		LC_OnPlayerGiveDamage(playerid, damagedid, amount, weaponid, bodypart);
	#endif

	if (LC_HasSameTeam(playerid, damagedid))
		return 1;

	if (!LC_ProcessDamage(damagedid, playerid, amount, weaponid, bodypart))
		return 1;

	if ((WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) || weaponid == WEAPON_MINIGUN)
    {
        if (LC_ePlayerLastShot[playerid][LC_e_LAST_SHOT_TYPE] != BULLET_HIT_TYPE_PLAYER || LC_ePlayerLastShot[playerid][LC_e_LAST_SHOT_HITID] != damagedid || !LC_ePlayerLastShot[playerid][LC_e_LAST_SHOT_RETURN])
      	{
      	    LC_ePlayerLastShot[playerid][LC_e_LAST_SHOT_TYPE] = BULLET_HIT_TYPE_NONE;
      	    LC_ePlayerLastShot[playerid][LC_e_LAST_SHOT_HITID] = INVALID_PLAYER_ID;
      	    LC_ePlayerLastShot[playerid][LC_e_LAST_SHOT_RETURN] = false;
	      	return 0;
		}

		LC_ePlayerLastShot[playerid][LC_e_LAST_SHOT_TYPE] = BULLET_HIT_TYPE_NONE;
  		LC_ePlayerLastShot[playerid][LC_e_LAST_SHOT_HITID] = INVALID_PLAYER_ID;
    	LC_ePlayerLastShot[playerid][LC_e_LAST_SHOT_RETURN] = false;
    }

    LC_DamagePlayer(damagedid, amount);

	new Float:health;
	GetPlayerHealth(damagedid, health);
	if (health <= 0.0)
	{
	    LC_ePlayerDeathData[damagedid][LC_e_PLAYER_DEATHDATA_VALID] = true;
		LC_ePlayerDeathData[damagedid][LC_e_PLAYER_DEATHDATA_KILLERID] = playerid;
		LC_ePlayerDeathData[damagedid][LC_e_PLAYER_DEATHDATA_WEAPONID] = weaponid;
	}

	#if defined LC_OnPlayerTakeDamage
		LC_OnPlayerTakeDamage(damagedid, playerid, amount, weaponid, bodypart);
	#endif
	return 0;
}
#if defined _ALS_OnPlayerGiveDamage
    #undef OnPlayerGiveDamage
#else
    #define _ALS_OnPlayerGiveDamage
#endif
#define OnPlayerGiveDamage LC_OnPlayerGiveDamage
#if defined LC_OnPlayerGiveDamage
	forward LC_OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart);
#endif

public OnPlayerDeath(playerid, killerid, reason)
{
	if (!LC_ePlayerDeathData[playerid][LC_e_PLAYER_DEATHDATA_VALID])
	{
	    return 0;
	}

	killerid = LC_ePlayerDeathData[playerid][LC_e_PLAYER_DEATHDATA_KILLERID];
	reason = LC_ePlayerDeathData[playerid][LC_e_PLAYER_DEATHDATA_WEAPONID];

	#if defined LC_OnPlayerDeath
		return LC_OnPlayerDeath(playerid, killerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath LC_OnPlayerDeath
#if defined LC_OnPlayerDeath
	forward LC_OnPlayerDeath(playerid, killerid, reason);
#endif

stock LC_SetPlayerTeam(playerid, team)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	    return 0;

	LC_iPlayerTeam[playerid] = team;
	return 1;
}
#if defined _ALS_SetPlayerTeam
    #undef SetPlayerTeam
#else
    #define _ALS_SetPlayerTeam
#endif
#define SetPlayerTeam LC_SetPlayerTeam

stock LC_GetPlayerTeam(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	    return 0;

	return LC_iPlayerTeam[playerid];
}
#if defined _ALS_GetPlayerTeam
    #undef GetPlayerTeam
#else
    #define _ALS_GetPlayerTeam
#endif
#define GetPlayerTeam LC_GetPlayerTeam

stock SetPlayerWeaponDamage(playerid, weaponid, Float:damage)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	    return 0;

	if (weaponid < 0 || weaponid >= sizeof(LC_WEAPON_DAMAGE))
	    return 0;

    LC_fPlayerWeaponDamage[playerid][weaponid] = damage;
	return 1;
}

stock Float:GetPlayerWeaponDamage(playerid, weaponid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	    return 0;

	if (weaponid < 0 || weaponid >= sizeof(LC_WEAPON_DAMAGE))
	    return 0;

    return LC_fPlayerWeaponDamage[playerid][weaponid];
}
