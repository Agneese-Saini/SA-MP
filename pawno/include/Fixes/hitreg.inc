#if defined hitreg_included
	#endinput
#endif
#define hitreg_included

/******************************************************/
// hitreg.inc | v1.0 | Last Updated: 21 Jan, 2018 | By Gammix
/******************************************************/

//*** DEFINITIONS

#define WEAPON_UNARMED 0
#define WEAPON_VEHICLE_M4 19
#define WEAPON_VEHICLE_MINIGUN 20
#define WEAPON_VEHICLE_ROCKETLAUNCHER 21
#define WEAPON_PISTOLWHIP 48
#define WEAPON_HELIBLADES 50
#define WEAPON_EXPLOSION 51
#define WEAPON_CARPARK 52
#define WEAPON_UNKNOWN 55

//*** ENUMERATORS

enum E_PLAYER_DEATH {
    bool:E_PLAYER_DEATH_VALID,
    E_PLAYER_DEATH_KILLERID,
    E_PLAYER_DEATH_REASON
};

//*** PRIVATE VARIABLES

static HitReg@playerTeamID[MAX_PLAYERS];

static HitReg@playerDeath[MAX_PLAYERS][E_PLAYER_DEATH];

static HitReg@playerLastExplosive[MAX_PLAYERS];

static Float:HitReg@playerWeaponDamage[MAX_PLAYERS][56];

//*** PUBLIC FUNCTION DECLERATIONS

SetPlayerWeaponDamage(playerid, weaponid, Float:damage);
Float:GetPlayerWeaponDamage(playerid, weaponid);

#if defined OnPlayerDamage
	forward OnPlayerDamage(playerid, issuerid, weaponid, Float:amount, bodypart, bool:death);
#endif

//*** INTERNAL FUNCTIONS

static bool:HitReg_ProcessDamage(playerid, issuerid, &Float:amount, &weaponid, &bodypart) { // All credits to "Slice" for this function
	if (amount < 0.0) {
		return false;
	}

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
		case WEAPON_FLAMETHROWER, WEAPON_DROWN, WEAPON_CARPARK,
		     WEAPON_SPRAYCAN, WEAPON_FIREEXTINGUISHER: {
			// Apply reasonable bounds
			if (weaponid == WEAPON_DROWN) {
				if (amount > 10.0) amount = 10.0;
			} else if (amount > 1.0) {
				amount = 1.0;
			}

			// Adjust the damage if the multiplier is not 1.0
			if (_:HitReg@playerWeaponDamage[issuerid][weaponid] != _:1.0) {
				amount *= HitReg@playerWeaponDamage[issuerid][weaponid];
			}

			// Make sure the distance is valid
			if (weaponid == WEAPON_SPRAYCAN || weaponid == WEAPON_FIREEXTINGUISHER || weaponid == WEAPON_CARPARK) {
				new Float:x, Float:y, Float:z, Float:dist;
				GetPlayerPos(issuerid, x, y, z);
				dist = GetPlayerDistanceFromPoint(playerid, x, y, z);

				if (dist > 15.0) {
					return false;
				}
			}

			return true;
		}
	}

	// Punching with a parachute
	if (weaponid == WEAPON_PARACHUTE) {
		weaponid = WEAPON_UNARMED;
	}

	// Collision damage should never be above 165
	if (weaponid == WEAPON_COLLISION) {
		if (amount > 165.0) {
			amount = 1.0;
		} else {
			amount /= 165.0;
		}
	}

	if (weaponid == WEAPON_EXPLOSION) {
		// Explosions do at most 82.5 damage. This will later be multipled by the damage value
		amount /= 82.5;

		if (IsPlayerInAnyVehicle(playerid) && GetPlayerVehicleSeat(playerid) == 0) { // detect hydra, hunter and tank rockets
		    weaponid = WEAPON_VEHICLE_ROCKETLAUNCHER;
		} else if (HitReg@playerLastExplosive[issuerid] != WEAPON_UNARMED) { // if not in vehicle, figure out what weapon was fired
			weaponid = HitReg@playerLastExplosive[issuerid];
		}
	}

	// detect vehicle weapons
	if (IsPlayerInAnyVehicle(playerid) && GetPlayerVehicleSeat(playerid) == 0) {
	    weaponid = (weaponid == WEAPON_M4) ? (WEAPON_VEHICLE_M4) : (WEAPON_VEHICLE_MINIGUN);
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

	new bool:melee = ((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP);

	// Can't punch from a vehicle
	if (melee && IsPlayerInAnyVehicle(issuerid)) {
		return false;
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
			     5.280000209808349609375: {
				// Damage is most likely from punching and switching weapon quickly
				if (!melee) {
					weaponid = WEAPON_UNARMED;
					melee = true;
				}
			}

			case 6.6000003814697265625: {
				if (!melee) {
					switch (weaponid) {
						case WEAPON_UZI, WEAPON_TEC9, WEAPON_CHAINSAW,
						     WEAPON_SHOTGUN, WEAPON_SAWEDOFF: {}

						default: {
							weaponid = WEAPON_UNARMED;
							melee = true;
						}
					}
				}
			}

			case 54.12000274658203125: {
				if (!melee) {
					melee = true;
					weaponid = WEAPON_UNARMED;
					amount = 1.32000005245208740234375;
				}

				// Be extra sure about this one
				if (GetPlayerFightingStyle(issuerid) != FIGHT_STYLE_KNEEHEAD) {
					return false;
				}
			}

			// Melee damage has been tampered with
			default: {
				if (melee) {
					return false;
				}
			}
		}
	}

	// check melee weapon range is valid
	if (melee) {
		new Float:x, Float:y, Float:z, Float:dist;
		GetPlayerPos(issuerid, x, y, z);
		dist = GetPlayerDistanceFromPoint(playerid, x, y, z);

		if (dist > 15.0) {
			return false;
		}
	}

	new Float:bullets;

	switch (weaponid) {
		// The spas shotguns shoot 8 bullets, each inflicting 4.95 damage
		case WEAPON_SHOTGSPA: {
			bullets = amount / 4.950000286102294921875;

			if (8.0 - bullets < -0.05) {
				return false;
			}
		}

		// Shotguns and sawed-off shotguns shoot 15 bullets, each inflicting 3.3 damage
		case WEAPON_SHOTGUN, WEAPON_SAWEDOFF: {
			bullets = amount / 3.30000019073486328125;

			if (15.0 - bullets < -0.05) {
				return false;
			}
		}
	}

	if (_:bullets) {
		new Float:f = floatfract(bullets);

		// The damage for each bullet has been tampered with
		if (f > 0.01 && f < 0.99) {
			return false;
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
				return false;
			}
		}
	}

	// Check deagle damage
	if (weaponid == WEAPON_DEAGLE) {
		switch (amount) {
			case 46.200000762939453125,
			     23.1000003814697265625: {}

			default: {
				return false;
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

	// damage is not equal to default damage
	if (_:def_amount && _:def_amount != _:amount) {
		return false;
	}

	// Adjust the damage
	switch (weaponid) {
		// melee weapons
		case WEAPON_UNARMED..WEAPON_KATANA,
		 	 WEAPON_DILDO..WEAPON_CANE,
 			 WEAPON_PISTOLWHIP: {
			amount *= HitReg@playerWeaponDamage[issuerid][weaponid];
		}

		// grenades (moltov/flamethrower is done on the top since its a high rate weapon)
		case WEAPON_GRENADE: {
			amount *= HitReg@playerWeaponDamage[issuerid][weaponid];
		}

		// vehicle weapons (hunter, rustler, sea sparrow & hydra)
		case WEAPON_VEHICLE_M4,
			 WEAPON_VEHICLE_MINIGUN: {
			amount = HitReg@playerWeaponDamage[issuerid][weaponid];
		}

		// bullet weapons
		case WEAPON_COLT45..WEAPON_SNIPER,
			 WEAPON_MINIGUN: {
			amount = HitReg@playerWeaponDamage[issuerid][weaponid];
		}

		// explosive weapons
		case WEAPON_ROCKETLAUNCHER,
			 WEAPON_HEATSEEKER,
			 WEAPON_VEHICLE_ROCKETLAUNCHER,
			 WEAPON_EXPLOSION: {
			amount *= HitReg@playerWeaponDamage[issuerid][weaponid];
		}

		// vehicle
		case WEAPON_VEHICLE: {
			amount = HitReg@playerWeaponDamage[issuerid][weaponid];
		}

		// heliblades
		case WEAPON_HELIBLADES: {
			amount = HitReg@playerWeaponDamage[issuerid][weaponid];
		}
	}

	return true;
}

//*** HOOKED METHODS/CALLBACKS

public OnPlayerConnect(playerid) {
	HitReg@playerTeamID[playerid] = NO_TEAM;
	SetPlayerTeam(playerid, 655);

    HitReg@playerDeath[playerid][E_PLAYER_DEATH_VALID] = false;
    HitReg@playerDeath[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
    HitReg@playerDeath[playerid][E_PLAYER_DEATH_REASON] = WEAPON_UNKNOWN;

    HitReg@playerLastExplosive[playerid] = WEAPON_UNARMED;

	HitReg@playerWeaponDamage[playerid][0] = 1.0; // 0 - Fist
	HitReg@playerWeaponDamage[playerid][1] = 1.0; // 1 - Brass knuckles
	HitReg@playerWeaponDamage[playerid][2] = 1.0; // 2 - Golf club
	HitReg@playerWeaponDamage[playerid][3] = 1.0; // 3 - Nitestick
	HitReg@playerWeaponDamage[playerid][4] = 1.0; // 4 - Knife
	HitReg@playerWeaponDamage[playerid][5] = 1.0; // 5 - Bat
	HitReg@playerWeaponDamage[playerid][6] = 1.0; // 6 - Shovel
	HitReg@playerWeaponDamage[playerid][7] = 1.0; // 7 - Pool cue
	HitReg@playerWeaponDamage[playerid][8] = 1.0; // 8 - Katana
	HitReg@playerWeaponDamage[playerid][9] = 1.0; // 9 - Chainsaw
	HitReg@playerWeaponDamage[playerid][10] = 1.0; // 10 - Dildo
	HitReg@playerWeaponDamage[playerid][11] = 1.0; // 11 - Dildo 2
	HitReg@playerWeaponDamage[playerid][12] = 1.0; // 12 - Vibrator
	HitReg@playerWeaponDamage[playerid][13] = 1.0; // 13 - Vibrator 2
	HitReg@playerWeaponDamage[playerid][14] = 1.0; // 14 - Flowers
	HitReg@playerWeaponDamage[playerid][15] = 1.0; // 15 - Cane
	HitReg@playerWeaponDamage[playerid][16] = 82.5; // 16 - Grenade
	HitReg@playerWeaponDamage[playerid][18] = 1.0; // 18 - Molotov
	HitReg@playerWeaponDamage[playerid][19] = 9.9; // 19 - Vehicle M4 (custom)
	HitReg@playerWeaponDamage[playerid][20] = 46.2; // 20 - Vehicle minigun (custom)
	HitReg@playerWeaponDamage[playerid][21] = 82.5; // 21 - Vehicle rocket (custom)
	HitReg@playerWeaponDamage[playerid][22] = 8.25; // 22 - Colt 45
	HitReg@playerWeaponDamage[playerid][23] = 13.2; // 23 - Silenced
	HitReg@playerWeaponDamage[playerid][24] = 46.2; // 24 - Deagle
	HitReg@playerWeaponDamage[playerid][25] = 3.3; // 25 - Shotgun
	HitReg@playerWeaponDamage[playerid][26] = 3.3; // 26 - Sawed-off
	HitReg@playerWeaponDamage[playerid][27] = 4.95; // 27 - Spas
	HitReg@playerWeaponDamage[playerid][28] = 6.6; // 28 - UZI
	HitReg@playerWeaponDamage[playerid][29] = 8.25; // 29 - MP5
	HitReg@playerWeaponDamage[playerid][30] = 9.9; // 30 - AK47
	HitReg@playerWeaponDamage[playerid][31] = 9.9; // 31 - M4
	HitReg@playerWeaponDamage[playerid][32] = 6.6; // 32 - Tec9
	HitReg@playerWeaponDamage[playerid][33] = 24.75; // 33 - Cuntgun
	HitReg@playerWeaponDamage[playerid][34] = 41.25; // 34 - Sniper
	HitReg@playerWeaponDamage[playerid][35] = 82.5; // 35 - Rocket launcher
	HitReg@playerWeaponDamage[playerid][36] = 82.5; // 36 - Heatseeker
	HitReg@playerWeaponDamage[playerid][37] = 1.0; // 37 - Flamethrower
	HitReg@playerWeaponDamage[playerid][38] = 46.2; // 38 - Minigun
	HitReg@playerWeaponDamage[playerid][39] = 82.5; // 39 - Satchel
	HitReg@playerWeaponDamage[playerid][41] = 0.33; // 41 - Spraycan
	HitReg@playerWeaponDamage[playerid][42] = 0.33; // 42 - Fire extinguisher
	HitReg@playerWeaponDamage[playerid][48] = 2.64; // 48 - Pistol whip (custom)
	HitReg@playerWeaponDamage[playerid][49] = 9.9; // 49 - Vehicle
	HitReg@playerWeaponDamage[playerid][50] = 330.0; // 50 - Helicopter blades
	HitReg@playerWeaponDamage[playerid][51] = 82.5; // 51 - Explosion
	HitReg@playerWeaponDamage[playerid][52] = 1.0; // 52 - Car park (custom)

	#if defined HitReg_OnPlayerConnect
		return HitReg_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect HitReg_OnPlayerConnect
#if defined HitReg_OnPlayerConnect
	forward HitReg_OnPlayerConnect(playerid);
#endif

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	// Detect player explosive weapon used
	if (newkeys & KEY_FIRE) {
	    new weaponid = GetPlayerWeapon(playerid);
		switch (weaponid) {
			case WEAPON_GRENADE,
				 WEAPON_ROCKETLAUNCHER,
				 WEAPON_HEATSEEKER,
				 WEAPON_SATCHEL,
				 WEAPON_BOMB: {
				HitReg@playerLastExplosive[playerid] = weaponid;
			}
		}
	}

	#if defined HitReg_OnPlayerKeyStateChange
		return HitReg_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange HitReg_OnPlayerKeyStateChange
#if defined HitReg_OnPlayerKeyStateChange
	forward HitReg_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
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
	    }

	    if (_:maxrange == _:0.0) { // Invalid weapon
			return 0;
		}

		// Weapon range check
		new Float:ox, Float:oy, Float:oz, Float:hx, Float:hy, Float:hz;
		GetPlayerLastShotVectors(playerid, ox, oy, oz, hx, hy, hz);

		if (VectorSize((ox - hx), (oy - hy), (oz - hz)) > maxrange) {
			return 0;
		}
	}

	#if defined HitReg_OnPlayerWeaponShot
	    return HitReg_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, fX, fY, fZ);
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnPlayerWeaponShot
    #undef OnPlayerWeaponShot
#else
    #define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot HitReg_OnPlayerWeaponShot
#if defined HitReg_OnPlayerWeaponShot
	forward HitReg_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart) {
	if (issuerid != INVALID_PLAYER_ID) { // Player damage player is handled in OnPlayerGiveDamage
		return 1;
	}

	// This calback will only handles self inflicted damage

	switch (weaponid) {
 		case WEAPON_FLAMETHROWER,
			 WEAPON_VEHICLE,
			 WEAPON_CARPARK,
			 WEAPON_EXPLOSION: {

			// Detect the original explosive weapon used
		    if (weaponid == WEAPON_EXPLOSION && HitReg@playerLastExplosive[issuerid] != WEAPON_UNARMED) {
				weaponid = HitReg@playerLastExplosive[issuerid];
			}

    	    new Float:health, Float:armour;
			GetPlayerHealth(playerid, health);
			GetPlayerArmour(playerid, armour);

    	    new bool:death = (((armour + health) - amount) <= 0.0);

			#if defined OnPlayerDamage
			    OnPlayerDamage(playerid, INVALID_PLAYER_ID, weaponid, amount, bodypart, death);
			#endif

			if (death) { // If player dies, set death to valid
			    HitReg@playerDeath[playerid][E_PLAYER_DEATH_VALID] = true;
				HitReg@playerDeath[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
				HitReg@playerDeath[playerid][E_PLAYER_DEATH_REASON] = weaponid;
			}

   		 	return 0;
		}

    	case WEAPON_DROWN,
			 WEAPON_COLLISION: { // This weapon/reason of damage only affects player health (not armour)

    	    new Float:health;
			GetPlayerHealth(playerid, health);

			new bool:death = ((health - amount) <= 0.0);

			#if defined OnPlayerDamage
			    OnPlayerDamage(playerid, INVALID_PLAYER_ID, weaponid, amount, bodypart, death);
			#endif

			if (death) { // If player dies, set death to valid
			    HitReg@playerDeath[playerid][E_PLAYER_DEATH_VALID] = true;
				HitReg@playerDeath[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
				HitReg@playerDeath[playerid][E_PLAYER_DEATH_REASON] = WEAPON_DROWN;
			}

   		 	return 0;
    	}
	}

    return 1;
}
#if defined _ALS_OnPlayerTakeDamage
    #undef OnPlayerTakeDamage
#else
    #define _ALS_OnPlayerTakeDamage
#endif
#define OnPlayerTakeDamage HitReg_OnPlayerTakeDamage
#if defined HitReg_OnPlayerTakeDamage
	forward HitReg_OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart);
#endif

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart) {
	// This calback will only handles player damage player
	if (playerid == INVALID_PLAYER_ID) {
		return 1;
	}

	// If "HitReg_ProcessDamage" returns "false", means something wrong in game (a bug) or a hack was detected; don't register hit
	if (!HitReg_ProcessDamage(damagedid, playerid, amount, weaponid, bodypart)) {
		return 1;
	}

    new Float:health, Float:armour;
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
		if (health < 0.0) health = 0.0;
	}

	new bool:death = (health <= 0.0);

	// Check what callback has to return for the hit taken
    new bool:ret = false;
	#if defined OnPlayerDamage
 		ret = bool:OnPlayerDamage(damagedid, playerid, weaponid, amount, bodypart, death);
	#endif

    // If the method/callback returns "false", don't inflict damage on player
	if (!ret) {
		return 1;
	}

	// If both players have same team, don't inflict damage
	if (HitReg@playerTeamID[playerid] != NO_TEAM && HitReg@playerTeamID[damagedid] != NO_TEAM && HitReg@playerTeamID[playerid] == HitReg@playerTeamID[damagedid]) {
	    return 1;
	}

	SetPlayerArmour(damagedid, armour);
	SetPlayerHealth(damagedid, health);

    if (death) { // If player dies, set death to valid
	    HitReg@playerDeath[damagedid][E_PLAYER_DEATH_VALID] = true;
		HitReg@playerDeath[damagedid][E_PLAYER_DEATH_KILLERID] = playerid;
		HitReg@playerDeath[damagedid][E_PLAYER_DEATH_REASON] = weaponid;
	}

	return 0;
}
#if defined _ALS_OnPlayerGiveDamage
    #undef OnPlayerGiveDamage
#else
    #define _ALS_OnPlayerGiveDamage
#endif
#define OnPlayerGiveDamage HitReg_OnPlayerGiveDamage
#if defined HitReg_OnPlayerGiveDamage
	forward HitReg_OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart);
#endif

public OnPlayerDeath(playerid, killerid, reason) {
	if (!HitReg@playerDeath[playerid][E_PLAYER_DEATH_VALID]) { // If death is not valid, return 0
	    return 0;
	}

	// Set values to include one's
	killerid = HitReg@playerDeath[playerid][E_PLAYER_DEATH_KILLERID];
	reason = HitReg@playerDeath[playerid][E_PLAYER_DEATH_REASON];

	// Reset death to invalid
	HitReg@playerDeath[playerid][E_PLAYER_DEATH_VALID] = false;

	#if defined HitReg_OnPlayerDeath
		return HitReg_OnPlayerDeath(playerid, killerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDeath
    #undef OnPlayerDeath
#else
    #define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath HitReg_OnPlayerDeath
#if defined HitReg_OnPlayerDeath
	forward HitReg_OnPlayerDeath(playerid, killerid, reason);
#endif

//*** PUBLIC FUNCTIONS

stock SetPlayerWeaponDamage(playerid, weaponid, Float:damage) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (weaponid < 0 || weaponid >= 53) {
	    return 0;
	}

    HitReg@playerWeaponDamage[playerid][weaponid] = damage;
	return 1;
}

stock Float:GetPlayerWeaponDamage(playerid, weaponid) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (weaponid < 0 || weaponid >= 53) {
	    return 0;
	}

    return HitReg@playerWeaponDamage[playerid][weaponid];
}

stock PlayerDeath(playerid, killerid, reason) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (killerid < 0 || killerid >= MAX_PLAYERS) {
	    return 0;
	}

	SetPlayerHealth(playerid, 0.0);

	HitReg@playerDeath[playerid][E_PLAYER_DEATH_VALID] = true;
	HitReg@playerDeath[playerid][E_PLAYER_DEATH_KILLERID] = killerid;
	HitReg@playerDeath[playerid][E_PLAYER_DEATH_REASON] = reason;
	return 1;
}

stock HitReg_SetPlayerTeam(playerid, team) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	HitReg@playerTeamID[playerid] = team;
	return 1;
}
#if defined _ALS_SetPlayerTeam
    #undef SetPlayerTeam
#else
    #define _ALS_SetPlayerTeam
#endif
#define SetPlayerTeam HitReg_SetPlayerTeam

stock HitReg_GetPlayerTeam(playerid) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	return HitReg@playerTeamID[playerid];
}
#if defined _ALS_GetPlayerTeam
    #undef GetPlayerTeam
#else
    #define _ALS_GetPlayerTeam
#endif
#define GetPlayerTeam HitReg_GetPlayerTeam

stock HitReg_SetPlayerHealth(playerid, Float:health) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (health <= 0.0) {
        HitReg@playerDeath[playerid][E_PLAYER_DEATH_VALID] = true;
		HitReg@playerDeath[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
		HitReg@playerDeath[playerid][E_PLAYER_DEATH_REASON] = WEAPON_UNKNOWN;
	}

	return SetPlayerHealth(playerid, health);
}
#if defined _ALS_SetPlayerHealth
    #undef SetPlayerHealth
#else
    #define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth HitReg_SetPlayerHealth
