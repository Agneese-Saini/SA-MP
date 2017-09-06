#if defined lagcomp_included
	#endinput
#endif
#define lagcomp_included

// Lagcomp.inc - By Gammix - Last Updated: 5 Sept, 2017

enum
{
    WEAPON_UNARMED = 0,
    WEAPON_VEHICLE_M4 = 19,
    WEAPON_VEHICLE_MINIGUN = 20,
    WEAPON_PISTOLWHIP = 48,
    WEAPON_HELIBLADES = 50,
    WEAPON_EXPLOSION = 51,
    WEAPON_CARPARK = 52,
    WEAPON_UNKNOWN = 55
};

enum E_PLAYER_LAST_SHOT
{
    E_PLAYER_LAST_SHOT_TYPE,
    E_PLAYER_LAST_SHOT_HIT_ID,
    E_PLAYER_LAST_SHOT_RETURN
};

enum E_PLAYER_DEATH
{
    bool:E_PLAYER_DEATH_VALID,
    E_PLAYER_DEATH_KILLERID,
    E_PLAYER_DEATH_REASON
};

static playerLastShot[MAX_PLAYERS][E_PLAYER_LAST_SHOT];
static playerDeath[MAX_PLAYERS][E_PLAYER_DEATH];

static playerTeamID[MAX_PLAYERS];
static Float:playerWeaponDamage[MAX_PLAYERS][55];

static playerLastExplosive[MAX_PLAYERS];

static HasSameTeam(playerid, otherid)
{
	if (playerTeamID[playerid] == NO_TEAM || playerTeamID[otherid] == NO_TEAM)
	{
		return 0;
	}
	return (playerTeamID[playerid] == playerTeamID[otherid]);
}

/*
** Function originally by Slice - "weapon-config.inc"
*/
static ProcessDamage(&playerid, &issuerid, &Float:amount, &weaponid, &bodypart)
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
			 	{
			 	}

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

	if (issuerid == INVALID_PLAYER_ID || IsPlayerInAnyVehicle(issuerid))
	{
	    switch (weaponid)
	    {
	        case WEAPON_UNARMED..WEAPON_CANE,
			  	 WEAPON_COLT45..WEAPON_SNIPER,
				 WEAPON_MINIGUN,
		  		 WEAPON_PISTOLWHIP:
			{
			    return 0;
			}
	    }
	}

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
					 	{
					 	}

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
				{
					return 0;
				}
			}

			default:
			{
				if ((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP)
				{
					amount = playerWeaponDamage[issuerid][weaponid];
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
		    amount = playerWeaponDamage[issuerid][weaponid];
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
					amount *= playerWeaponDamage[issuerid][weaponid];
				}

				default:
				{
					amount = playerWeaponDamage[issuerid][weaponid];
				}
			}
	    }

		case WEAPON_GRENADE,
			 WEAPON_ROCKETLAUNCHER,
			 WEAPON_HEATSEEKER,
			 WEAPON_SATCHEL,
			 WEAPON_EXPLOSION:
		{
		    amount = (amount / 82.5) * playerWeaponDamage[issuerid][weaponid];
		}

		case WEAPON_MOLTOV,
			 WEAPON_FLAMETHROWER,
			 WEAPON_CARPARK,
			 WEAPON_VEHICLE,
			 WEAPON_COLT45..WEAPON_DEAGLE,
			 WEAPON_UZI..WEAPON_SNIPER,
			 WEAPON_MINIGUN,
			 WEAPON_SPRAYCAN,
			 WEAPON_FIREEXTINGUISHER,
			 WEAPON_HELIBLADES:
		{
		    amount = playerWeaponDamage[issuerid][weaponid];
		}

		case WEAPON_SHOTGSPA:
	 	{
	 	    new Float:bullets = amount / 4.950000286102294921875;
			if (8.0 - bullets < -0.05)
			{
   	 			bullets = 1.0;
			}

			new Float:f = floatfract(bullets);
			if (f > 0.01 && f < 0.99)
			{
				bullets = 1.0;
			}

	 	    amount = playerWeaponDamage[issuerid][weaponid] * bullets;
		}

		case WEAPON_SHOTGUN,
			 WEAPON_SAWEDOFF:
	 	{
	 	    new Float:bullets = amount / 3.30000019073486328125;
			if (15.0 - bullets < -0.05)
			{
				bullets = 1.0;
			}

			new Float:f = floatfract(bullets);
			if (f > 0.01 && f < 0.99)
			{
				bullets = 1.0;
			}

	 	    amount = playerWeaponDamage[issuerid][weaponid] * bullets;
		}

		default:
		{
			return 0;
		}
	}
	return 1;
}

public OnPlayerConnect(playerid)
{
	playerTeamID[playerid] = NO_TEAM;
	SetPlayerTeam(playerid, 655);

	playerWeaponDamage[playerid][0] = 1.0; // 0 - Fist
	playerWeaponDamage[playerid][1] = 1.0; // 1 - Brass knuckles
	playerWeaponDamage[playerid][2] = 1.0; // 2 - Golf club
	playerWeaponDamage[playerid][3] = 1.0; // 3 - Nitestick
	playerWeaponDamage[playerid][4] = 1.0; // 4 - Knife
	playerWeaponDamage[playerid][5] = 1.0; // 5 - Bat
	playerWeaponDamage[playerid][6] = 1.0; // 6 - Shovel
	playerWeaponDamage[playerid][7] = 1.0; // 7 - Pool cue
	playerWeaponDamage[playerid][8] = 1.0; // 8 - Katana
	playerWeaponDamage[playerid][9] = 1.0; // 9 - Chainsaw
	playerWeaponDamage[playerid][10] = 1.0; // 10 - Dildo
	playerWeaponDamage[playerid][11] = 1.0; // 11 - Dildo 2
	playerWeaponDamage[playerid][12] = 1.0; // 12 - Vibrator
	playerWeaponDamage[playerid][13] = 1.0; // 13 - Vibrator 2
	playerWeaponDamage[playerid][14] = 1.0; // 14 - Flowers
	playerWeaponDamage[playerid][15] = 1.0; // 15 - Cane
	playerWeaponDamage[playerid][16] = 82.5; // 16 - Grenade
	playerWeaponDamage[playerid][18] = 1.0; // 18 - Molotov
	playerWeaponDamage[playerid][19] = 9.9; // 19 - Vehicle M4 (custom)
	playerWeaponDamage[playerid][20] = 46.2; // 20 - Vehicle minigun (custom)
	playerWeaponDamage[playerid][22] = 8.25; // 22 - Colt 45
	playerWeaponDamage[playerid][23] = 13.2; // 23 - Silenced
	playerWeaponDamage[playerid][24] = 46.2; // 24 - Deagle
	playerWeaponDamage[playerid][25] = 3.3; // 25 - Shotgun
	playerWeaponDamage[playerid][26] = 3.3; // 26 - Sawed-off
	playerWeaponDamage[playerid][27] = 4.95; // 27 - Spas
	playerWeaponDamage[playerid][28] = 6.6; // 28 - UZI
	playerWeaponDamage[playerid][29] = 8.25; // 29 - MP5
	playerWeaponDamage[playerid][30] = 9.9; // 30 - AK47
	playerWeaponDamage[playerid][31] = 9.9; // 31 - M4
	playerWeaponDamage[playerid][32] = 6.6; // 32 - Tec9
	playerWeaponDamage[playerid][33] = 24.75; // 33 - Cuntgun
	playerWeaponDamage[playerid][34] = 41.25; // 34 - Sniper
	playerWeaponDamage[playerid][35] = 82.5; // 35 - Rocket launcher
	playerWeaponDamage[playerid][36] = 82.5; // 36 - Heatseeker
	playerWeaponDamage[playerid][37] = 1.0; // 37 - Flamethrower
	playerWeaponDamage[playerid][38] = 46.2; // 38 - Minigun
	playerWeaponDamage[playerid][39] = 82.5; // 39 - Satchel
	playerWeaponDamage[playerid][41] = 0.33; // 41 - Spraycan
	playerWeaponDamage[playerid][42] = 0.33; // 42 - Fire extinguisher
	playerWeaponDamage[playerid][48] = 2.64; // 48 - Pistol whip (custom)
	playerWeaponDamage[playerid][49] = 9.9; // 49 - Vehicle
	playerWeaponDamage[playerid][50] = 330.0; // 50 - Helicopter blades
	playerWeaponDamage[playerid][51] = 82.5; // 51 - Explosion
	playerWeaponDamage[playerid][52] = 1.0; // 52 - Car park (custom)

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
	{
		return 0;
	}

	if (hittype != BULLET_HIT_TYPE_NONE)
	{
	    new Float:maxrange = 0.0;
	    switch (weaponid)
	    {
	        case WEAPON_VEHICLE_M4: maxrange = 90.0; // 19 - Vehicle M4 (custom)
	        case WEAPON_VEHICLE_MINIGUN: maxrange = 75.0; // 20 - Vehicle minigun (custom)
	        case WEAPON_COLT45: maxrange = 35.0; // 22 - Colt 45
	        case WEAPON_SILENCED: maxrange = 35.0; // 23 - Silenced
			case WEAPON_DEAGLE: maxrange = 35.0; // 24 - Deagle
			case WEAPON_SHOTGUN: maxrange = 40.0; // 25 - Shotgun
			case WEAPON_SAWEDOFF: maxrange = 35.0; // 26 - Sawed-off
			case WEAPON_SHOTGSPA: maxrange = 40.0; // 27 - Spas
			case WEAPON_UZI: maxrange = 35.0; // 28 - UZI
			case WEAPON_MP5: maxrange = 45.0; // 29 - MP5
			case WEAPON_AK47: maxrange = 70.0; // 30 - AK47
			case WEAPON_M4: maxrange = 90.0; // 31 - M4
			case WEAPON_TEC9: maxrange = 35.0; // 32 - Tec9
			case WEAPON_RIFLE: maxrange = 100.0; // 33 - Cuntgun
			case WEAPON_SNIPER: maxrange = 320.0; // 34 - Sniper
			case WEAPON_MINIGUN: maxrange = 75.0; // 38 - Minigun
	    }

		new Float:ox, Float:oy, Float:oz, Float:hx, Float:hy, Float:hz;
		GetPlayerLastShotVectors(playerid, ox, oy, oz, hx, hy, hz);

		if (VectorSize((ox - hx), (oy - hy), (oz - hz)) > maxrange)
		{
			return 0;
		}
	}

	playerLastShot[playerid][E_PLAYER_LAST_SHOT_TYPE] = hittype;
	playerLastShot[playerid][E_PLAYER_LAST_SHOT_HIT_ID] = hitid;
	#if defined LC_OnPlayerWeaponShot
	    playerLastShot[playerid][E_PLAYER_LAST_SHOT_RETURN] = bool:LC_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	#else
	    playerLastShot[playerid][E_PLAYER_LAST_SHOT_RETURN] = true;
	#endif

	return playerLastShot[playerid][E_PLAYER_LAST_SHOT_RETURN];
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
	{
		return 1;
	}

	switch (weaponid)
 	{
 		case WEAPON_FLAMETHROWER,
			 WEAPON_SPRAYCAN,
			 WEAPON_FIREEXTINGUISHER,
			 WEAPON_VEHICLE,
			 WEAPON_CARPARK,
			 WEAPON_HELIBLADES,
			 WEAPON_EXPLOSION:
		{
    	    // process
    	    if (weaponid == WEAPON_FLAMETHROWER)
    	    {
    	        weaponid = playerLastExplosive[playerid];
    	    }
    	    
    	    if (weaponid == WEAPON_EXPLOSION)
    	    {
    	        weaponid = playerLastExplosive[playerid];
    	    }
		}

    	case WEAPON_DROWN:
    	{
    	    new Float:health;
			GetPlayerHealth(playerid, health);
			if ((health - amount) <= 0.0)
			{
			    playerDeath[playerid][E_PLAYER_DEATH_VALID] = true;
				playerDeath[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
				playerDeath[playerid][E_PLAYER_DEATH_REASON] = WEAPON_DROWN;
			}
    	}

    	case WEAPON_COLLISION:
    	{
    	    if (1061 <= GetPlayerAnimationIndex(playerid) <= 1067)
			{
			    TogglePlayerControllable(playerid, false);
				ClearAnimations(playerid);
			    TogglePlayerControllable(playerid, true);
				return 1;
			}

    	    new Float:health;
			GetPlayerHealth(playerid, health);
			if ((health - amount) <= 0.0)
			{
			    playerDeath[playerid][E_PLAYER_DEATH_VALID] = true;
				playerDeath[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
				playerDeath[playerid][E_PLAYER_DEATH_REASON] = WEAPON_DROWN;
			}
	 	}

    	default:
    	{
    	    // do not process
    	    return 1;
    	}
	}

	new Float:armour;
	GetPlayerArmour(playerid, armour);

    new Float:health;
	GetPlayerHealth(playerid, health);

	if (((armour + health) - amount) <= 0.0)
	{
	    playerDeath[playerid][E_PLAYER_DEATH_VALID] = true;
		playerDeath[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
		playerDeath[playerid][E_PLAYER_DEATH_REASON] = weaponid;
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

	if (HasSameTeam(playerid, damagedid))
	{
		return 1;
	}

	if ((WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) || weaponid == WEAPON_MINIGUN)
    {
        if (!(playerLastShot[playerid][E_PLAYER_LAST_SHOT_TYPE] == BULLET_HIT_TYPE_PLAYER && playerLastShot[playerid][E_PLAYER_LAST_SHOT_HIT_ID] == damagedid && playerLastShot[playerid][E_PLAYER_LAST_SHOT_RETURN]))
      	{
      	    // shot wasn't registeed

      	    playerLastShot[playerid][E_PLAYER_LAST_SHOT_TYPE] = BULLET_HIT_TYPE_NONE;
      	    playerLastShot[playerid][E_PLAYER_LAST_SHOT_HIT_ID] = INVALID_PLAYER_ID;
      	    playerLastShot[playerid][E_PLAYER_LAST_SHOT_RETURN] = false;
      	    return 0;
		}

		playerLastShot[playerid][E_PLAYER_LAST_SHOT_TYPE] = BULLET_HIT_TYPE_NONE;
  		playerLastShot[playerid][E_PLAYER_LAST_SHOT_HIT_ID] = INVALID_PLAYER_ID;
    	playerLastShot[playerid][E_PLAYER_LAST_SHOT_RETURN] = false;
    }

	if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
	{
	    if (weaponid == WEAPON_MINIGUN)
	    {
	        weaponid = WEAPON_VEHICLE_MINIGUN;
	    }
	    else if (GetVehicleModel(GetPlayerVehicleID(playerid)) == 447 && weaponid == WEAPON_M4)
	    {
	        weaponid = WEAPON_VEHICLE_M4;
	    }
	}

	if (!ProcessDamage(damagedid, playerid, amount, weaponid, bodypart))
	{
		return 1;
	}

    new Float:armour;
	GetPlayerArmour(damagedid, armour);

    new Float:health;
	GetPlayerHealth(damagedid, health);

	if (armour > 0.0)
	{
		armour -= amount;
	  	if (armour < 0.0)
	  	{
			SetPlayerArmour(damagedid, 0.0);
			health += armour;
			SetPlayerHealth(damagedid, health);
		}
		else
		{
		    SetPlayerArmour(damagedid, armour);
		}
	}
	else
	{
	    health -= amount;
		SetPlayerHealth(damagedid, health);
	}

    if (health <= 0.0)
	{
	    playerDeath[damagedid][E_PLAYER_DEATH_VALID] = true;
		playerDeath[damagedid][E_PLAYER_DEATH_KILLERID] = playerid;
		playerDeath[damagedid][E_PLAYER_DEATH_REASON] = weaponid;
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

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (newkeys & KEY_FIRE)
	{
		switch (GetPlayerWeapon(playerid))
		{
		    case WEAPON_GRENADE,
                 WEAPON_MOLTOV,
				 WEAPON_ROCKETLAUNCHER,
				 WEAPON_HEATSEEKER,
				 WEAPON_FLAMETHROWER,
				 WEAPON_SATCHEL,
				 WEAPON_BOMB:
	    	{
				playerLastExplosive[playerid] = GetPlayerWeapon(playerid);
			}
		}
	}
	
	#if defined LC_OnPlayerKeyStateChange
		return LC_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange LC_OnPlayerKeyStateChange
#if defined LC_OnPlayerKeyStateChange
	forward LC_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

public OnPlayerDeath(playerid, killerid, reason)
{
	if (!playerDeath[playerid][E_PLAYER_DEATH_VALID])
	{
	    return 0;
	}

	playerDeath[playerid][E_PLAYER_DEATH_VALID] = false;

	killerid = playerDeath[playerid][E_PLAYER_DEATH_KILLERID];
	reason = playerDeath[playerid][E_PLAYER_DEATH_REASON];

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
	{
	    return 0;
	}

	playerTeamID[playerid] = team;
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
	{
	    return 0;
	}

	return playerTeamID[playerid];
}
#if defined _ALS_GetPlayerTeam
    #undef GetPlayerTeam
#else
    #define _ALS_GetPlayerTeam
#endif
#define GetPlayerTeam LC_GetPlayerTeam

stock LC_SetPlayerHealth(playerid, Float:health)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (health < 0.0)
	{
        playerDeath[playerid][E_PLAYER_DEATH_VALID] = true;
		playerDeath[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
		playerDeath[playerid][E_PLAYER_DEATH_REASON] = WEAPON_UNKNOWN;
	}

	return SetPlayerHealth(playerid, health);
}
#if defined _ALS_SetPlayerHealth
    #undef SetPlayerHealth
#else
    #define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth LC_SetPlayerHealth

stock SetPlayerWeaponDamage(playerid, weaponid, Float:damage)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (weaponid < 0 || weaponid >= 53)
	{
	    return 0;
	}

    playerWeaponDamage[playerid][weaponid] = damage;
	return 1;
}

stock Float:GetPlayerWeaponDamage(playerid, weaponid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (weaponid < 0 || weaponid >= 53)
	{
	    return 0;
	}

    return playerWeaponDamage[playerid][weaponid];
}
