#if defined lagcomp_included
	#endinput
#endif
#define lagcomp_included

/******************************************************/
// lagcomp.inc | v2.0 | Last Updated: 20 Dec, 2017 | By Gammix
/******************************************************/

#define HasSameTeam(%0,%1) \
	((playerTeamID[%0] == NO_TEAM || playerTeamID[%1] == NO_TEAM) ? (false) : (playerTeamID[%0] == playerTeamID[%1]))

enum {
    WEAPON_UNARMED = 0,
    WEAPON_VEHICLE_M4 = 19,
    WEAPON_VEHICLE_MINIGUN = 20,
    WEAPON_PISTOLWHIP = 48,
    WEAPON_HELIBLADES = 50,
    WEAPON_EXPLOSION = 51,
    WEAPON_CARPARK = 52,
    WEAPON_UNKNOWN = 55
};

enum E_WEAPON_DATA {
	E_WEAPON_DATA_WEAPON,
	E_WEAPON_DATA_TIMESTAMP
};

enum E_PLAYER_DEATH {
    bool:E_PLAYER_DEATH_VALID,
    E_PLAYER_DEATH_KILLERID,
    E_PLAYER_DEATH_REASON
};

static playerWeaponData[MAX_PLAYERS][15][E_WEAPON_DATA];
static playerWeaponDataArraySize[MAX_PLAYERS];
static playerDeath[MAX_PLAYERS][E_PLAYER_DEATH];
static playerTeamID[MAX_PLAYERS];
static Float:playerWeaponDamage[MAX_PLAYERS][55];

#if defined OnPlayerDamage
	forward OnPlayerDamage(playerid, issuerid, weaponid, Float:amount, bodypart, bool:death);
#endif

// Function originally by Slice - "weapon-config.inc"
static ProcessDamage(issuerid, &Float:amount, &weaponid, &bodypart) {
    switch (amount) {
		case 3.63000011444091796875,
		     5.940000057220458984375,
		     5.610000133514404296875:
		{
			amount = 2.6400001049041748046875;
		}

		case 3.30000019073486328125: {
			if (weaponid != WEAPON_SHOTGUN && weaponid != WEAPON_SAWEDOFF) {
				amount = 2.6400001049041748046875;
			}
		}

		case 4.950000286102294921875: {
			if ((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP) {
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

		case 9.90000057220458984375: {
			switch (weaponid) {
				case WEAPON_VEHICLE,
					 WEAPON_VEHICLE_M4,
					 WEAPON_AK47,
					 WEAPON_M4,
					 WEAPON_SHOTGUN,
					 WEAPON_SAWEDOFF,
					 WEAPON_SHOTGSPA:
			 	{
			 	}

				default: {
					amount = 6.6000003814697265625;
				}
			}
		}
	}

	if (weaponid == WEAPON_HELIBLADES && _:amount != _:330.0) {
		weaponid = WEAPON_CARPARK;
	}

	if (weaponid == WEAPON_PARACHUTE) {
		weaponid = WEAPON_UNARMED;
	}

    switch (weaponid) {
		case WEAPON_COLT45..WEAPON_SNIPER,
		     WEAPON_MINIGUN,
			 WEAPON_SPRAYCAN,
			 WEAPON_FIREEXTINGUISHER:
	 	{
			if (_:amount == _:2.6400001049041748046875) {
				bodypart = 4; // BODY_PART_TORSO
				weaponid = WEAPON_PISTOLWHIP;
			}
		}
	}

	if (IsPlayerInAnyVehicle(issuerid)) {
	    switch (weaponid) {
	        case WEAPON_UNARMED..WEAPON_CANE,
			  	 WEAPON_COLT45..WEAPON_SNIPER,
				 WEAPON_MINIGUN,
		  		 WEAPON_PISTOLWHIP:
			{
			    return 0;
			}
	    }
	}

    if (weaponid != WEAPON_PISTOLWHIP) {
		switch (amount) {
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
				if (!((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP)) {
					weaponid = WEAPON_UNARMED;
				}
			}

			case 6.6000003814697265625: {
				if (!((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP)) {
					switch (weaponid) {
						case WEAPON_UZI,
							 WEAPON_TEC9,
							 WEAPON_CHAINSAW,
						     WEAPON_SHOTGUN,
							 WEAPON_SAWEDOFF:
					 	{
					 	}

						default: {
							weaponid = WEAPON_UNARMED;
						}
					}
				}
			}

			case 54.12000274658203125: {
				if (!((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP)) {
					weaponid = WEAPON_UNARMED;
					amount = 1.32000005245208740234375;
				}

				if (GetPlayerFightingStyle(issuerid) != FIGHT_STYLE_KNEEHEAD) {
					return 0;
				}
			}
		}
	}

	switch (weaponid) {
		case WEAPON_UNARMED,
			 WEAPON_PISTOLWHIP,
			 WEAPON_BRASSKNUCKLE..WEAPON_KATANA,
             WEAPON_DILDO..WEAPON_CANE:
		{
		    amount = playerWeaponDamage[issuerid][weaponid];
		}

	    case WEAPON_CHAINSAW: {
	        switch (amount) {
				case 6.6000003814697265625,
				     13.5300006866455078125,
				     16.1700000762939453125,
				     26.40000152587890625,
				     27.060001373291015625:
			 	{
					amount *= playerWeaponDamage[issuerid][weaponid];
				}

				default: {
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

		case WEAPON_SHOTGSPA: {
	 	    new Float:bullets = amount / 4.950000286102294921875;
			if (8.0 - bullets < -0.05) {
   	 			bullets = 1.0;
			}

			new Float:f = floatfract(bullets);
			if (f > 0.01 && f < 0.99) {
				bullets = 1.0;
			}

	 	    amount = playerWeaponDamage[issuerid][weaponid] * bullets;
		}

		case WEAPON_SHOTGUN,
			 WEAPON_SAWEDOFF:
	 	{
	 	    new Float:bullets = amount / 3.30000019073486328125;
			if (15.0 - bullets < -0.05) {
				bullets = 1.0;
			}

			new Float:f = floatfract(bullets);
			if (f > 0.01 && f < 0.99) {
				bullets = 1.0;
			}

	 	    amount = playerWeaponDamage[issuerid][weaponid] * bullets;
		}

		default: {
			return 0;
		}
	}

	return 1;
}

public OnPlayerConnect(playerid) {
	for (new i = 0; i < sizeof playerWeaponData[]; i++) {
		playerWeaponData[playerid][i][E_WEAPON_DATA_WEAPON] = WEAPON_UNKNOWN;
		playerWeaponData[playerid][i][E_WEAPON_DATA_TIMESTAMP] = 0;
	}
	playerWeaponDataArraySize[playerid] = 0;
	
    playerDeath[playerid][E_PLAYER_DEATH_VALID] = false;
    playerDeath[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
    playerDeath[playerid][E_PLAYER_DEATH_REASON] = WEAPON_UNKNOWN;

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

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ) {
	if (hittype != BULLET_HIT_TYPE_NONE) {
	    new Float:maxrange = 0.0;
	    switch (weaponid) {
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

			default: { // Invalid weapon
				return 0;
			}
	    }

		new Float:ox, Float:oy, Float:oz, Float:hx, Float:hy, Float:hz;
		GetPlayerLastShotVectors(playerid, ox, oy, oz, hx, hy, hz);

		if (VectorSize((ox - hx), (oy - hy), (oz - hz)) > maxrange) {
			return 0;
		}
	}

	#if defined LC_OnPlayerWeaponShot
	    return LC_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	#else
	    return 1;
	#endif
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

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart) {
	if (issuerid != INVALID_PLAYER_ID) {
		return 1;
	}

	switch (weaponid) {
 		case WEAPON_FLAMETHROWER,
			 WEAPON_VEHICLE,
			 WEAPON_CARPARK,
			 WEAPON_EXPLOSION:
		{
    	    if (weaponid == WEAPON_EXPLOSION) {
    	        for (new i = (playerWeaponDataArraySize[playerid] - 1); i != -1; i--) {
					switch (playerWeaponData[playerid][i][E_WEAPON_DATA_WEAPON]) {
						case WEAPON_ROCKETLAUNCHER,
							 WEAPON_HEATSEEKER,
							 WEAPON_SATCHEL,
							 WEAPON_BOMB:
						{
						    if ((gettime() - playerWeaponData[playerid][i][E_WEAPON_DATA_TIMESTAMP]) <= 5000) {
						        weaponid = playerWeaponData[playerid][i][E_WEAPON_DATA_WEAPON];
						        break;
						    }
				        }
					}
				}
    	    }
    	    else if (weaponid == WEAPON_FLAMETHROWER) {
    	        new bool:hasFlamethrower = false;
    	        for (new i = (playerWeaponDataArraySize[playerid] - 1); i != -1; i--) {
					if (playerWeaponData[playerid][i][E_WEAPON_DATA_WEAPON] == WEAPON_FLAMETHROWER) {
					    hasFlamethrower = true;
						break;
					}
				}
				
				if (!hasFlamethrower) {
					weaponid = WEAPON_MOLTOV;
				}
				else {
	    	        for (new i = (playerWeaponDataArraySize[playerid] - 1); i != -1; i--) {
						if (playerWeaponData[playerid][i][E_WEAPON_DATA_WEAPON] == WEAPON_FLAMETHROWER) {
						    if ((gettime() - playerWeaponData[playerid][i][E_WEAPON_DATA_TIMESTAMP]) <= 10000) {
								weaponid = WEAPON_FLAMETHROWER;
								break;
					        }
						}
					}
				}
    	    }

    	    new Float:health;
			new Float:armour;
			GetPlayerHealth(playerid, health);
			GetPlayerArmour(playerid, armour);
    	    new bool:death = bool:(((armour + health) - amount) <= 0.0);

			#if defined OnPlayerDamage
			    OnPlayerDamage(playerid, INVALID_PLAYER_ID, weaponid, amount, bodypart, death);
			#endif

			if (death) {
			    playerDeath[playerid][E_PLAYER_DEATH_VALID] = true;
				playerDeath[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
				playerDeath[playerid][E_PLAYER_DEATH_REASON] = weaponid;
			}
		}

    	case WEAPON_DROWN,
			 WEAPON_COLLISION:
		{
    	    new Float:health;
			GetPlayerHealth(playerid, health);
			new bool:death = bool:((health - amount) <= 0.0);

			#if defined OnPlayerDamage
			    OnPlayerDamage(playerid, INVALID_PLAYER_ID, weaponid, amount, bodypart, death);
			#endif

			if (death) {
			    playerDeath[playerid][E_PLAYER_DEATH_VALID] = true;
				playerDeath[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
				playerDeath[playerid][E_PLAYER_DEATH_REASON] = WEAPON_DROWN;
			}
    	}

    	default: {
    	    return 1;
    	}
	}
    return 0;
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

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart) {
	if (HasSameTeam(playerid, damagedid)) {
		return 1;
	}

	if (IsPlayerInAnyVehicle(playerid) && GetPlayerVehicleSeat(playerid) == 0 && (weaponid == WEAPON_M4 || weaponid == WEAPON_MINIGUN)) {
	    weaponid = ((weaponid == WEAPON_MINIGUN) ? (WEAPON_VEHICLE_MINIGUN) : (WEAPON_VEHICLE_M4));
	}

	if (!ProcessDamage(playerid, amount, weaponid, bodypart)) {
		return 1;
	}

    new Float:health;
    new Float:armour;
	GetPlayerHealth(damagedid, health);
	GetPlayerArmour(damagedid, armour);

	if (armour > 0.0) {
		armour -= amount;

	  	if (armour < 0.0) {
	  	    armour = 0.0;
			health += armour;
  		}
	}
	else {
		health -= amount;
		if (health < 0.0) {
			health = 0.0;
		}
	}

	new bool:death = bool:(health <= 0.0);

    new bool:ret = false;
	#if defined OnPlayerDamage
 		ret = bool:OnPlayerDamage(damagedid, playerid, weaponid, amount, bodypart, death);
	#endif

	if (!ret) {
		return 1;
	}

	SetPlayerArmour(damagedid, armour);
	SetPlayerHealth(damagedid, health);

    if (death) {
	    playerDeath[damagedid][E_PLAYER_DEATH_VALID] = true;
		playerDeath[damagedid][E_PLAYER_DEATH_KILLERID] = playerid;
		playerDeath[damagedid][E_PLAYER_DEATH_REASON] = weaponid;
	}

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

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	if (newkeys & KEY_FIRE) {
	    new weaponid = GetPlayerWeapon(playerid);
		switch (weaponid) {
		    case WEAPON_MOLTOV,
				 WEAPON_FLAMETHROWER,
				 WEAPON_GRENADE,
				 WEAPON_ROCKETLAUNCHER,
				 WEAPON_HEATSEEKER,
				 WEAPON_SATCHEL,
				 WEAPON_BOMB:
	    	{
	    	    new index;
	    	    if (playerWeaponDataArraySize[playerid] == sizeof playerWeaponData[]) {
					for (new i = 0; i < (playerWeaponDataArraySize[playerid] - 1); i++) {
                        playerWeaponData[playerid][i] = playerWeaponData[playerid][i + 1];
					}

					index = (sizeof playerWeaponData[] - 1);
				}
				else {
	    	    	index = playerWeaponDataArraySize[playerid]++;
	    	   	}

				playerWeaponData[playerid][index][E_WEAPON_DATA_WEAPON] = weaponid;
				playerWeaponData[playerid][index][E_WEAPON_DATA_TIMESTAMP] = gettime();
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

public OnPlayerDeath(playerid, killerid, reason) {
	if (!playerDeath[playerid][E_PLAYER_DEATH_VALID]) {
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

stock SetPlayerWeaponDamage(playerid, weaponid, Float:damage) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (weaponid < 0 || weaponid >= 53) {
	    return 0;
	}

    playerWeaponDamage[playerid][weaponid] = damage;
	return 1;
}

stock Float:GetPlayerWeaponDamage(playerid, weaponid) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (weaponid < 0 || weaponid >= 53) {
	    return 0;
	}

    return playerWeaponDamage[playerid][weaponid];
}

stock PlayerDeath(playerid, killerid, reason) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (killerid != INVALID_PLAYER_ID || killerid < 0 || killerid >= MAX_PLAYERS) {
	    killerid = INVALID_PLAYER_ID;
	}

	SetPlayerHealth(playerid, 0.0);

	playerDeath[playerid][E_PLAYER_DEATH_VALID] = true;
	playerDeath[playerid][E_PLAYER_DEATH_KILLERID] = killerid;
	playerDeath[playerid][E_PLAYER_DEATH_REASON] = reason;
	return 1;
}

stock LC_SetPlayerTeam(playerid, team) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
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

stock LC_GetPlayerTeam(playerid) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
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

stock LC_SetPlayerHealth(playerid, Float:health) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (health <= 0.0) {
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
