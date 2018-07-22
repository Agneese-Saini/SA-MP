/******************************************************/
// weapon_damage.inc | v1.1 [ALPHA] | Last Updated: 22 July, 2018 | By Gammix
// credits: Gammix | special thanks: Slice (WD_ProcessDamage() function)
/******************************************************/

#if defined weapon_damage_included
	#endinput
#endif
#define weapon_damage_included

SetPlayerWeaponDamage(playerid, weaponid, Float:amount);
Float:GetPlayerWeaponDamage(playerid, weaponid);
PlayerDeath(playerid, killerid, reason);

#if defined OnPlayerDamage
	forward OnPlayerDamage(playerid, issuerid, &Float:amount, weapon, bodypart);
#endif

#define MAX_WEAPONS 55

#define WEAPON_UNARMED 0
#define WEAPON_VEHICLE_M4 19
#define WEAPON_VEHICLE_MINIGUN 20
#define WEAPON_VEHICLE_ROCKETLAUNCHER 21
#define WEAPON_PISTOLWHIP 48
#define WEAPON_HELIBLADES 50
#define WEAPON_EXPLOSION 51
#define WEAPON_CARPARK 52
#define WEAPON_UNKNOWN 55

enum E_DEATH_DATA {
	bool:DEATH_VALID,
	DEATH_KILLERID,
	DEATH_REASON
};

static Float:WD_weaponDamage[MAX_PLAYERS][MAX_WEAPONS];
static WD_deathData[MAX_PLAYERS][E_DEATH_DATA];
static WD_lastExplosive[MAX_PLAYERS];
static WD_teamID[MAX_PLAYERS];
static WD_knifeTimer[MAX_PLAYERS];

static WD_IsPlayerSpawned(playerid) {
	if (WD_knifeTimer[playerid] != -1) {
		return false;
	}

	switch (GetPlayerState(playerid)) {
		case PLAYER_STATE_ONFOOT..PLAYER_STATE_PASSENGER,
		     PLAYER_STATE_SPAWNED: {
			return true;
		}
	}

	return false;
}

static WD_IsMeleeWeapon(weaponid) { // by Slice
	return (WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP;
}

static WD_ProcessDamage(playerid, issuerid, &Float:amount, &weaponid, &bodypart) { // by Slice
	// Adjust invalid amounts caused by an animation bug
	switch (amount) {
		case 3.63000011444091796875,
		     5.940000057220458984375,
		     5.610000133514404296875: {
			amount = 2.6400001049041748046875;
		}

		case 3.30000019073486328125: {
			if (weaponid != WEAPON_SHOTGUN && weaponid != WEAPON_SAWEDOFF) {
				amount = 2.6400001049041748046875;
			}
		}

		case 4.950000286102294921875: {
			if (WD_IsMeleeWeapon(weaponid)) {
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
		     11.22000026702880859375: {
			amount = 2.6400001049041748046875;
		}

		case 9.90000057220458984375: {
			switch (weaponid) {
				case WEAPON_VEHICLE, WEAPON_VEHICLE_M4, WEAPON_AK47,
				     WEAPON_M4, WEAPON_SHOTGUN, WEAPON_SAWEDOFF, WEAPON_SHOTGSPA: {}

				default: {
					amount = 6.6000003814697265625;
				}
			}
		}
	}

	// Car parking
	if (weaponid == WEAPON_HELIBLADES && _:amount != _:330.0) {
		weaponid = WEAPON_CARPARK;
	}

	// Finish processing drown/fire/carpark quickly, since they are sent at very high rates
	switch (weaponid) {
		case WEAPON_FLAMETHROWER, WEAPON_SPRAYCAN, WEAPON_FIREEXTINGUISHER: {
			// Apply reasonable bounds
			if (amount > 1.0) {
				amount = 1.0;
			}

			// Adjust the damage
			amount *= WD_weaponDamage[playerid][weaponid];

  			// Make sure the distance and issuer is valid
			new Float:x, Float:y, Float:z, Float:dist;
			GetPlayerPos(issuerid, x, y, z);
			dist = GetPlayerDistanceFromPoint(playerid, x, y, z);

			if (dist > 15.0) {
				return 0;
			}

			return 1;
		}
	}

	// Punching with a parachute
	if (weaponid == WEAPON_PARACHUTE) {
		weaponid = WEAPON_UNARMED;
	}

	if (weaponid == WEAPON_EXPLOSION) {
		// Explosions do at most 82.5 damage. This will later be multipled by the damage value
		amount /= 82.5;

		// Figure out what caused the explosion
		weaponid = WD_lastExplosive[issuerid];
	}

	// Check for pistol whip
	switch (weaponid) {
		case WEAPON_COLT45 .. WEAPON_SNIPER,
		     WEAPON_MINIGUN, WEAPON_SPRAYCAN, WEAPON_FIREEXTINGUISHER: {
			// A pistol whip inflicts 2.64 damage
			if (_:amount == _:2.6400001049041748046875) {
				// Save the weapon in the bodypart argument (it's always BODY_PART_TORSO)
				bodypart = weaponid;
				weaponid = WEAPON_PISTOLWHIP;
			}
		}
	}

	if (!WD_IsMeleeWeapon(weaponid)) {
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
			     5.280000209808349609375: {
				// Damage is most likely from punching and switching weapon quickly
				weaponid = WEAPON_UNARMED;
			}

			case 6.6000003814697265625: {
				switch (weaponid) {
					case WEAPON_UZI, WEAPON_TEC9, WEAPON_CHAINSAW,
					     WEAPON_SHOTGUN, WEAPON_SAWEDOFF: {}

					default: {
						weaponid = WEAPON_UNARMED;
					}
				}
			}

			case 54.12000274658203125: {
				weaponid = WEAPON_UNARMED;
				amount = 1.32000005245208740234375;
			}
		}
	}

	if (WD_IsMeleeWeapon(weaponid)) {
		new Float:x, Float:y, Float:z, Float:dist;
		GetPlayerPos(issuerid, x, y, z);
		dist = GetPlayerDistanceFromPoint(playerid, x, y, z);

		if (dist > 15.0) {
			return 0;
		}
	}

	new Float:bullets = 0.0;

	switch (weaponid) {
		// The spas shotguns shoot 8 bullets, each inflicting 4.95 damage
		case WEAPON_SHOTGSPA: {
			bullets = amount / 4.950000286102294921875;

			if (8.0 - bullets < -0.05) {
				return 0;
			}
		}

		// Shotguns and sawed-off shotguns shoot 15 bullets, each inflicting 3.3 damage
		case WEAPON_SHOTGUN, WEAPON_SAWEDOFF: {
			bullets = amount / 3.30000019073486328125;

			if (15.0 - bullets < -0.05) {
				return 0;
			}
		}
	}

	if (_:bullets) {
		new Float:f = floatfract(bullets);

		// The damage for each bullet has been tampered with
		if (f > 0.01 && f < 0.99) {
			return 0;
		}

		// Divide the damage amount by the number of bullets
		amount /= bullets;
	}

	// Check chainsaw damage
	if (weaponid == WEAPON_CHAINSAW) {
		switch (amount) {
			case 6.6000003814697265625,
			     13.5300006866455078125,
			     16.1700000762939453125,
			     26.40000152587890625,
			     27.060001373291015625: {}

			default: {
				return 0;
			}
		}
	}

	// Check deagle damage
	if (weaponid == WEAPON_DEAGLE) {
		switch (amount) {
			case 46.200000762939453125,
			     23.1000003814697265625: {}

			default: {
				return 0;
			}
		}
	}

	// Check gun damage
	new Float:def_amount = 0.0;

	switch (weaponid) {
		case WEAPON_COLT45,
  		     WEAPON_MP5: def_amount = 8.25;
		case WEAPON_SILENCED: def_amount = 13.200000762939453125;
		case WEAPON_UZI,
		     WEAPON_TEC9: def_amount = 6.6000003814697265625;
		case WEAPON_AK47,
		     WEAPON_M4,
		     WEAPON_VEHICLE_M4: def_amount = 9.90000057220458984375;
		case WEAPON_RIFLE: def_amount = 24.7500019073486328125;
		case WEAPON_SNIPER: def_amount = 41.25;
		case WEAPON_MINIGUN,
		     WEAPON_VEHICLE_MINIGUN: def_amount = 46.200000762939453125;
		case WEAPON_VEHICLE: def_amount = 9.90000057220458984375;
	}

	if (_:def_amount && _:def_amount != _:amount) {
		return 0;
	}

	// Adjust the damage
	switch (weaponid) {
		// Melee weapons
		case WEAPON_UNARMED..WEAPON_KATANA,
			 WEAPON_DILDO..WEAPON_CANE,
 			 WEAPON_PISTOLWHIP: {
			amount = WD_weaponDamage[issuerid][weaponid];
		}

		// Chainsaw
		case WEAPON_CHAINSAW: {
			amount *= WD_weaponDamage[issuerid][weaponid];
		}

		// Vehicle weapons
		case WEAPON_VEHICLE_M4,
			 WEAPON_VEHICLE_MINIGUN: {
			amount = WD_weaponDamage[issuerid][weaponid];
		}

		// Shotguns
		case WEAPON_SHOTGSPA, WEAPON_SHOTGUN, WEAPON_SAWEDOFF: {
			amount = (bullets * WD_weaponDamage[issuerid][weaponid]);
		}

		// Bullet weapons
		case WEAPON_COLT45..WEAPON_DEAGLE,
			 WEAPON_UZI..WEAPON_SNIPER,
			 WEAPON_MINIGUN: {
			amount = WD_weaponDamage[issuerid][weaponid];
		}
	}

	return 1;
}

static WD_InflictDamage(playerid, Float:amount, bool:ignore_armour = false) {
	if (!WD_IsPlayerSpawned(playerid)) {
		return;
	}

	new Float:armour, Float:health;
	GetPlayerArmour(playerid, armour);
	GetPlayerHealth(playerid, health);

	if (ignore_armour == false) {
		armour -= amount;
		if (armour < 0.0) {
			health += armour;
		}
	}
	else {
	    health -= amount;
	}

	if (armour < 0.0) armour = 0.0;
	if (health < 0.0) health = 0.0;

	SetPlayerArmour(playerid, armour);
	SetPlayerHealth(playerid, health);
}


public OnPlayerConnect(playerid) {
	WD_weaponDamage[playerid][0] = 1.0; // 0 - Fist
	WD_weaponDamage[playerid][1] = 1.0; // 1 - Brass knuckles
	WD_weaponDamage[playerid][2] = 1.0; // 2 - Golf club
	WD_weaponDamage[playerid][3] = 1.0; // 3 - Nitestick
	WD_weaponDamage[playerid][4] = 1.0; // 4 - Knife
	WD_weaponDamage[playerid][5] = 1.0; // 5 - Bat
	WD_weaponDamage[playerid][6] = 1.0; // 6 - Shovel
	WD_weaponDamage[playerid][7] = 1.0; // 7 - Pool cue
	WD_weaponDamage[playerid][8] = 1.0; // 8 - Katana
	WD_weaponDamage[playerid][9] = 1.0; // 9 - Chainsaw
	WD_weaponDamage[playerid][10] = 1.0; // 10 - Dildo
	WD_weaponDamage[playerid][11] = 1.0; // 11 - Dildo 2
	WD_weaponDamage[playerid][12] = 1.0; // 12 - Vibrator
	WD_weaponDamage[playerid][13] = 1.0; // 13 - Vibrator 2
	WD_weaponDamage[playerid][14] = 1.0; // 14 - Flowers
	WD_weaponDamage[playerid][15] = 1.0; // 15 - Cane
	WD_weaponDamage[playerid][16] = 82.5; // 16 - Grenade
	WD_weaponDamage[playerid][18] = 1.0; // 18 - Molotov
	WD_weaponDamage[playerid][19] = 9.9; // 19 - Vehicle M4 (custom)
	WD_weaponDamage[playerid][20] = 46.2; // 20 - Vehicle minigun (custom)
	WD_weaponDamage[playerid][21] = 82.5; // 21 - Vehicle rocket (custom)
	WD_weaponDamage[playerid][22] = 8.25; // 22 - Colt 45
	WD_weaponDamage[playerid][23] = 13.2; // 23 - Silenced
	WD_weaponDamage[playerid][24] = 46.2; // 24 - Deagle
	WD_weaponDamage[playerid][25] = 3.3; // 25 - Shotgun
	WD_weaponDamage[playerid][26] = 3.3; // 26 - Sawed-off
	WD_weaponDamage[playerid][27] = 4.95; // 27 - Spas
	WD_weaponDamage[playerid][28] = 6.6; // 28 - UZI
	WD_weaponDamage[playerid][29] = 8.25; // 29 - MP5
	WD_weaponDamage[playerid][30] = 9.9; // 30 - AK47
	WD_weaponDamage[playerid][31] = 9.9; // 31 - M4
	WD_weaponDamage[playerid][32] = 6.6; // 32 - Tec9
	WD_weaponDamage[playerid][33] = 24.75; // 33 - Cuntgun
	WD_weaponDamage[playerid][34] = 41.25; // 34 - Sniper
	WD_weaponDamage[playerid][35] = 82.5; // 35 - Rocket launcher
	WD_weaponDamage[playerid][36] = 82.5; // 36 - Heatseeker
	WD_weaponDamage[playerid][37] = 1.0; // 37 - Flamethrower
	WD_weaponDamage[playerid][38] = 46.2; // 38 - Minigun
	WD_weaponDamage[playerid][39] = 82.5; // 39 - Satchel
	WD_weaponDamage[playerid][41] = 0.33; // 41 - Spraycan
	WD_weaponDamage[playerid][42] = 0.33; // 42 - Fire extinguisher
	WD_weaponDamage[playerid][48] = 2.64; // 48 - Pistol whip (custom)
	WD_weaponDamage[playerid][49] = 9.9; // 49 - Vehicle
	WD_weaponDamage[playerid][50] = 330.0; // 50 - Helicopter blades
	WD_weaponDamage[playerid][51] = 82.5; // 51 - Explosion
	WD_weaponDamage[playerid][52] = 1.0; // 52 - Car park (custom)
	WD_weaponDamage[playerid][53] = 0.0; // 52 - Drowning
	WD_weaponDamage[playerid][54] = 165.0; // 52 - Splat

	WD_deathData[playerid][DEATH_VALID] = false;
	WD_deathData[playerid][DEATH_KILLERID] = INVALID_PLAYER_ID;
	WD_deathData[playerid][DEATH_REASON] = WEAPON_UNARMED;

	WD_lastExplosive[playerid] = WEAPON_UNARMED;

	WD_knifeTimer[playerid] = -1;

	WD_teamID[playerid] = NO_TEAM;
	SetPlayerTeam(playerid, 0x255);

	#if defined WD_OnPlayerConnect
		return WD_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect WD_OnPlayerConnect
#if defined WD_OnPlayerConnect
	forward WD_OnPlayerConnect(playerid);
#endif

public OnPlayerSpawn(playerid) {
	WD_deathData[playerid][DEATH_VALID] = false;
	WD_deathData[playerid][DEATH_KILLERID] = INVALID_PLAYER_ID;
	WD_deathData[playerid][DEATH_REASON] = WEAPON_UNARMED;

	WD_lastExplosive[playerid] = WEAPON_UNARMED;

	KillTimer(WD_knifeTimer[playerid]);
	WD_knifeTimer[playerid] = -1;

	#if defined WD_OnPlayerSpawn
		return WD_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn WD_OnPlayerSpawn
#if defined WD_hOnPlayerSpawn
	forward WD_OnPlayerSpawn(playerid);
#endif

public OnPlayerDeath(playerid, killerid, reason) {
	if (WD_deathData[playerid][DEATH_VALID] == false) {
	    return 0;
	}

	KillTimer(WD_knifeTimer[playerid]);
	WD_knifeTimer[playerid] = -1;

	killerid = WD_deathData[playerid][DEATH_KILLERID];
	reason = WD_deathData[playerid][DEATH_REASON];

	WD_deathData[playerid][DEATH_VALID] = false;

	#if defined WD_OnPlayerDeath
		return WD_OnPlayerDeath(playerid, killerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath WD_OnPlayerDeath
#if defined WD_OnPlayerDeath
	forward WD_OnPlayerDeath(playerid, killerid, reason);
#endif

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if (newkeys & KEY_FIRE) {
		new weapon = GetPlayerWeapon(playerid);

		switch (weapon) {
			case WEAPON_BOMB, WEAPON_SATCHEL: {
				WD_lastExplosive[playerid] = WEAPON_SATCHEL;
			}

			case WEAPON_ROCKETLAUNCHER, WEAPON_HEATSEEKER, WEAPON_GRENADE: {
				WD_lastExplosive[playerid] = weapon;
			}
		}
	}

	#if defined WD_OnPlayerKeyStateChange
		return WD_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange WD_OnPlayerKeyStateChange
#if defined WD_OnPlayerKeyStateChange
	forward WD_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ) {
	if (!WD_IsPlayerSpawned(playerid)) {
		return 0;
	}

	if (hittype != BULLET_HIT_TYPE_NONE) {
	    new Float:max_range = 0.0;

	    switch (weaponid) {
	        case WEAPON_COLT45,
				 WEAPON_SILENCED,
				 WEAPON_DEAGLE: max_range = 35.0;
			case WEAPON_SHOTGUN,
			     WEAPON_SHOTGSPA: max_range = 40.0;
			case WEAPON_SAWEDOFF: max_range = 35.0;
			case WEAPON_UZI,
				 WEAPON_TEC9: max_range = 35.0;
			case WEAPON_MP5: max_range = 45.0;
			case WEAPON_AK47: max_range = 70.0;
			case WEAPON_M4,
				 WEAPON_VEHICLE_M4: max_range = 90.0;
			case WEAPON_RIFLE: max_range = 100.0;
			case WEAPON_SNIPER: max_range = 320.0;
			case WEAPON_VEHICLE_MINIGUN,
				 WEAPON_MINIGUN: max_range = 75.0;
	    }

		new Float:ox, Float:oy, Float:oz, Float:hx, Float:hy, Float:hz;
		GetPlayerLastShotVectors(playerid, ox, oy, oz, hx, hy, hz);

		if (_:max_range && VectorSize((ox - hx), (oy - hy), (oz - hz)) > max_range) {
			return 0;
		}
	}

	#if defined WD_OnPlayerWeaponShot
	    return WD_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnPlayerWeaponShot
    #undef OnPlayerWeaponShot
#else
    #define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot WD_OnPlayerWeaponShot
#if defined WD_OnPlayerWeaponShot
	forward WD_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart) {
	if (damagedid == INVALID_PLAYER_ID) {
		return 1;
	}

	if (!WD_IsPlayerSpawned(playerid) || !WD_IsPlayerSpawned(damagedid)) {
		return 1;
	}

	new team1 = GetPlayerTeam(playerid);
	new team2 = GetPlayerTeam(damagedid);
	if (team1 != NO_TEAM && team2 != NO_TEAM && team1 == team2) {
	    #if defined OnPlayerDamage
			OnPlayerDamage(playerid, issuerid, amount, weaponid, bodypart);
		#endif
		return 1;
	}

	new Float:armour, Float:health;
    if (weaponid == WEAPON_KNIFE) {
		if (_:amount == _:0.0) {
			GetPlayerArmour(damagedid, armour);
			GetPlayerHealth(damagedid, health);

			if (WD_knifeTimer[playerid] != -1) {
				KillTimer(WD_knifeTimer[playerid]);
			}

			#if defined OnPlayerDamage
				OnPlayerDamage(damagedid, playerid, amount, weaponid, bodypart);
			#endif

			WD_knifeTimer[playerid] = SetTimerEx("WD_OnPlayerKnifed", 2200, false, "ii", damagedid, playerid);
			return 1;
		}
	}

	if (!WD_ProcessDamage(damagedid, playerid, amount, weaponid, bodypart)) {
		return 1;
	}

	#if defined OnPlayerDamage
		if (OnPlayerDamage(damagedid, playerid, amount, weaponid, bodypart) == 0) {
		    return 0;
		}
	#endif

	WD_InflictDamage(damagedid, amount, false);

	GetPlayerHealth(damagedid, health);
	if (health == 0.0) {
	    WD_deathData[damagedid][DEATH_VALID] = true;
	    WD_deathData[damagedid][DEATH_KILLERID] = playerid;
	    WD_deathData[damagedid][DEATH_REASON] = weaponid;
	}

	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart) {
	switch (weaponid) {
		case WEAPON_FLAMETHROWER,
			 WEAPON_VEHICLE,
			 WEAPON_CARPARK,
			 WEAPON_EXPLOSION,
			 WEAPON_DROWN,
		 	 WEAPON_COLLISION: {
			new bool:explosive = false;
            if (weaponid == WEAPON_EXPLOSION) {
                explosive = true;
            
            	if (issuerid == INVALID_PLAYER_ID) {
			    	if (IsPlayerInAnyVehicle(playerid) && GetPlayerVehicleSeat(playerid) == 0) {
					    weaponid = WEAPON_VEHICLE_ROCKETLAUNCHER;
					} else {
						if (WD_lastExplosive[playerid] != WEAPON_UNARMED) {
							weaponid = WD_lastExplosive[playerid];
						}
					}
				}
				else {
			    	if (IsPlayerInAnyVehicle(issuerid) && GetPlayerVehicleSeat(issuerid) == 0) {
					    weaponid = WEAPON_VEHICLE_ROCKETLAUNCHER;
					} else {
						if (WD_lastExplosive[issuerid] != WEAPON_UNARMED) {
							weaponid = WD_lastExplosive[issuerid];
						}
					}
				}
			}

			#if defined OnPlayerDamage
				OnPlayerDamage(playerid, issuerid, amount, weaponid, bodypart);
			#endif

			new Float:armour, Float:health;
			GetPlayerArmour(playerid, armour);
			GetPlayerHealth(playerid, health);
			
			if (issuerid != INVALID_PLAYER_ID && (weaponid == WEAPON_FLAMETHROWER || weaponid == WEAPON_CARPARK || weaponid == WEAPON_VEHICLE || explosive == true)) {
                new team1 = GetPlayerTeam(playerid);
				new team2 = GetPlayerTeam(issuerid);
				if (team1 != NO_TEAM && team2 != NO_TEAM && team1 == team2) {
					return 1;
				}
				
                WD_InflictDamage(playerid, amount, false);
			}
			else {
				armour = 0.0;
				health -= amount;
				if (health < 0.0) health = 0.0;
			}

			GetPlayerHealth(playerid, health);
            if (health <= 0.0) {
			    WD_deathData[playerid][DEATH_VALID] = true;
			    WD_deathData[playerid][DEATH_KILLERID] = playerid;
			    WD_deathData[playerid][DEATH_REASON] = weaponid;
			}
		}
	}

	return 1;
}

forward WD_OnPlayerKnifed(playerid, issuerid);
public WD_OnPlayerKnifed(playerid, issuerid) {
    SetPlayerArmour(playerid, 0.0);
	SetPlayerHealth(playerid, 0.0);

	WD_deathData[playerid][DEATH_VALID] = true;
 	WD_deathData[playerid][DEATH_KILLERID] = issuerid;
 	WD_deathData[playerid][DEATH_REASON] = WEAPON_KNIFE;
}

stock WD_SetPlayerTeam(playerid, team) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	WD_teamID[playerid] = team;
	return 1;
}
#if defined _ALS_SetPlayerTeam
    #undef SetPlayerTeam
#else
    #define _ALS_SetPlayerTeam
#endif
#define SetPlayerTeam WD_SetPlayerTeam

stock WD_GetPlayerTeam(playerid) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	return WD_teamID[playerid];
}
#if defined _ALS_GetPlayerTeam
    #undef GetPlayerTeam
#else
    #define _ALS_GetPlayerTeam
#endif
#define GetPlayerTeam WD_GetPlayerTeam

stock WD_SetPlayerHealth(playerid, Float:health) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (health <= 0.0) {
		WD_deathData[playerid][DEATH_VALID] = true;
	 	WD_deathData[playerid][DEATH_KILLERID] = INVALID_PLAYER_ID;
	 	WD_deathData[playerid][DEATH_REASON] = WEAPON_UNKNOWN;
	}

	return SetPlayerHealth(playerid, health);
}
#if defined _ALS_SetPlayerHealth
    #undef SetPlayerHealth
#else
    #define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth WD_SetPlayerHealth

stock WD_SendDeathMessage(playerid, killerid, reason) {
	switch (reason) {
	    case WEAPON_UNKNOWN: reason = 47; // Fake pistol
	    case WEAPON_CARPARK: reason = WEAPON_VEHICLE;
	    case WEAPON_PISTOLWHIP: reason = WEAPON_UNARMED;
	    case WEAPON_VEHICLE_M4: reason = WEAPON_M4;
	    case WEAPON_VEHICLE_MINIGUN: reason = WEAPON_MINIGUN;
	    case WEAPON_VEHICLE_ROCKETLAUNCHER: reason = WEAPON_ROCKETLAUNCHER;
	}

	return SendDeathMessage(playerid, killerid, reason);
}
#if defined _ALS_SendDeathMessage
    #undef SendDeathMessage
#else
    #define _ALS_SendDeathMessage
#endif
#define SendDeathMessage WD_SendDeathMessage

stock SetPlayerWeaponDamage(playerid, weaponid, Float:amount) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (weaponid < 0 || weaponid >= 53) {
	    return 0;
	}

    WD_weaponDamage[playerid][weaponid] = amount;
	return 1;
}

stock Float:GetPlayerWeaponDamage(playerid, weaponid) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0.0;
	}

	if (weaponid < 0 || weaponid >= 53) {
	    return 0.0;
	}

    return WD_weaponDamage[playerid][weaponid];
}

stock PlayerDeath(playerid, killerid, reason) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (killerid < 0 || killerid >= MAX_PLAYERS) {
	    return 0;
	}

	SetPlayerHealth(playerid, 0.0);

	WD_deathData[playerid][DEATH_VALID] = true;
 	WD_deathData[playerid][DEATH_KILLERID] = killerid;
 	WD_deathData[playerid][DEATH_REASON] = reason;
	return 1;
}
