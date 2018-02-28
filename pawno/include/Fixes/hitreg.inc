#if defined hitreg_included
	#endinput
#endif
#define hitreg_included

/******************************************************/
// hitreg.inc | v2.0 | Last Updated: 11 Feb, 2018 | By Gammix
/******************************************************/

#define WEAPON_UNARMED 0
#define WEAPON_VEHICLE_M4 19
#define WEAPON_VEHICLE_MINIGUN 20
#define WEAPON_VEHICLE_ROCKETLAUNCHER 21
#define WEAPON_PISTOLWHIP 48
#define WEAPON_HELIBLADES 50
#define WEAPON_EXPLOSION 51
#define WEAPON_CARPARK 52
#define WEAPON_UNKNOWN 55

#if !defined MAX_FIRERATE_SAMPLES
	#define MAX_FIRERATE_SAMPLES 5
#endif

static const DEFAULT_WEAPON_FIRERATE[] = {
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
	0, // 21 - Vehicle rocket (custom)
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
	0  // 54 - Splat
};

static HitReg@lastExplosive[MAX_PLAYERS];
static Float:HitReg@weaponDamage[MAX_PLAYERS][56];

static HitReg@playerTeam[MAX_PLAYERS];
static HitReg@playerDeathData[MAX_PLAYERS][3];

static HitReg@knifeKillTimer[MAX_PLAYERS];

static HitReg@lastShotTickCount[MAX_PLAYERS][10];
static HitReg@lastShotWeapon[MAX_PLAYERS][sizeof(HitReg@lastShotTickCount[])];
static HitReg@lastShotID[MAX_PLAYERS];
static HitReg@numShotsFired[MAX_PLAYERS];

static HitReg@lastHitTickCount[MAX_PLAYERS][10];
static HitReg@lastHitWeapon[MAX_PLAYERS][sizeof(HitReg@lastHitTickCount[])];
static HitReg@lastHitID[MAX_PLAYERS];
static HitReg@numHitsIssued[MAX_PLAYERS];

#if defined OnPlayerDamage
	forward OnPlayerDamage(playerid, issuerid, weaponid, Float:amount, bodypart);
#endif

forward HitReg_OnPlayerKnifed(playerid, issuerid);

static HitReg_GetAverageFireRate(lastshot, ticks[], weapons[], size, &bool:multiple_weapons) {
	new total = 0;

	multiple_weapons = false;

	for (new i = (MAX_FIRERATE_SAMPLES - 2); i >= 0; i--) {
		new prev_idx = (lastshot - i - 1) % size;

		// JIT plugin fix
		if (prev_idx < 0)
			prev_idx += size;

		new this_idx = (lastshot - i) % size;

		// JIT plugin fix
		if (this_idx < 0)
			this_idx += size;

		multiple_weapons = (weapons[prev_idx] != weapons[this_idx]);

		total += (ticks[this_idx] - ticks[prev_idx]);
	}

	return total / (MAX_FIRERATE_SAMPLES - 1);
}

static bool:HitReg_IsPlayerSpawned(playerid) {
	if (HitReg@knifeKillTimer[playerid] != -1) {
		return false;
	}

	switch (GetPlayerState(playerid)) {
		case PLAYER_STATE_ONFOOT .. PLAYER_STATE_PASSENGER,
		     PLAYER_STATE_SPAWNED: {
			return true;
		}
	}

	return false;
}

static bool:HitReg_ProcessDamage(playerid, issuerid, &Float:amount, &weaponid, &bodypart) {
	if (amount < 0.0) {
		return false;
	}

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

    if (weaponid == WEAPON_M4 || weaponid == WEAPON_MINIGUN) {
		if (IsPlayerInAnyVehicle(playerid) && GetPlayerVehicleSeat(playerid) == 0) {
		    weaponid = (weaponid == WEAPON_M4) ? (WEAPON_VEHICLE_M4) : (WEAPON_VEHICLE_MINIGUN);
		}
	}

	switch (weaponid) {
		case WEAPON_FLAMETHROWER, WEAPON_DROWN, WEAPON_CARPARK,
		     WEAPON_SPRAYCAN, WEAPON_FIREEXTINGUISHER: {
			if (weaponid == WEAPON_DROWN) {
				if (amount > 10.0) amount = 10.0;
			}
			else if (amount > 1.0) {
				amount = 1.0;
			}

			if (_:HitReg@weaponDamage[issuerid][weaponid] != _:1.0) {
				amount *= HitReg@weaponDamage[issuerid][weaponid];
			}

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

	if (weaponid == WEAPON_PARACHUTE) {
		weaponid = WEAPON_UNARMED;
	}
	
	switch (weaponid) {
		case WEAPON_COLT45 .. WEAPON_SNIPER,
		     WEAPON_MINIGUN, WEAPON_SPRAYCAN, WEAPON_FIREEXTINGUISHER: {
			if (_:amount == _:2.6400001049041748046875) {
				bodypart = weaponid;
				weaponid = WEAPON_PISTOLWHIP;
			}
		}
	}

	new bool:melee = ((WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP);

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

				if (GetPlayerFightingStyle(issuerid) != FIGHT_STYLE_KNEEHEAD) {
					return false;
				}
			}

			default: {
				if (melee) {
					return false;
				}
			}
		}
	}

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
		case WEAPON_SHOTGSPA: {
			bullets = amount / 4.950000286102294921875;

			if (8.0 - bullets < -0.05) {
				return false;
			}
		}

		case WEAPON_SHOTGUN, WEAPON_SAWEDOFF: {
			bullets = amount / 3.30000019073486328125;

			if (15.0 - bullets < -0.05) {
				return false;
			}
		}
	}

	if (_:bullets) {
		new Float:f = floatfract(bullets);

		if (f > 0.01 && f < 0.99) {
			return false;
		}

		amount /= bullets;
	}

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

	if (weaponid == WEAPON_DEAGLE) {
		switch (amount) {
			case 46.200000762939453125,
			     23.1000003814697265625: {}

			default: {
				return false;
			}
		}
	}

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
		return false;
	}

	switch (weaponid) {
		case WEAPON_UNARMED..WEAPON_KATANA,
			 WEAPON_DILDO..WEAPON_CANE,
 			 WEAPON_PISTOLWHIP: {
			amount = HitReg@weaponDamage[issuerid][weaponid];
		}

		case WEAPON_CHAINSAW: {
			amount *= HitReg@weaponDamage[issuerid][weaponid];
		}

		case WEAPON_VEHICLE_M4,
			 WEAPON_VEHICLE_MINIGUN: {
			amount = HitReg@weaponDamage[issuerid][weaponid];
		}

		case WEAPON_SHOTGSPA, WEAPON_SHOTGUN, WEAPON_SAWEDOFF: {
			amount = (bullets * HitReg@weaponDamage[issuerid][weaponid]);
		}

		case WEAPON_COLT45..WEAPON_DEAGLE,
			 WEAPON_UZI..WEAPON_SNIPER,
			 WEAPON_MINIGUN: {
			amount = HitReg@weaponDamage[issuerid][weaponid];
		}
	}

	return true;
}

public OnPlayerConnect(playerid) {
	HitReg@lastExplosive[playerid] = WEAPON_UNARMED;

	HitReg@playerTeam[playerid] = NO_TEAM;
	HitReg@playerDeathData[playerid][0] = 0;
	HitReg@playerDeathData[playerid][1] = INVALID_PLAYER_ID;
	HitReg@playerDeathData[playerid][2] = WEAPON_UNKNOWN;

	HitReg@knifeKillTimer[playerid] = -1;

	for (new i = 0; i < sizeof(HitReg@lastShotTickCount[]); i++) {
		HitReg@lastShotTickCount[playerid][i] = 0;
		HitReg@lastShotWeapon[playerid][i] = WEAPON_UNARMED;
	}
	HitReg@lastShotID[playerid] = 0;
	HitReg@numShotsFired[playerid] = 0;

	for (new i = 0; i < sizeof(HitReg@lastHitTickCount[]); i++) {
		HitReg@lastHitTickCount[playerid][i] = 0;
		HitReg@lastHitWeapon[playerid][i] = WEAPON_UNARMED;
	}
	HitReg@lastHitID[playerid] = 0;
	HitReg@numHitsIssued[playerid] = 0;

	HitReg@weaponDamage[playerid][0] = 1.0; // 0 - Fist
	HitReg@weaponDamage[playerid][1] = 1.0; // 1 - Brass knuckles
	HitReg@weaponDamage[playerid][2] = 1.0; // 2 - Golf club
	HitReg@weaponDamage[playerid][3] = 1.0; // 3 - Nitestick
	HitReg@weaponDamage[playerid][4] = 1.0; // 4 - Knife
	HitReg@weaponDamage[playerid][5] = 1.0; // 5 - Bat
	HitReg@weaponDamage[playerid][6] = 1.0; // 6 - Shovel
	HitReg@weaponDamage[playerid][7] = 1.0; // 7 - Pool cue
	HitReg@weaponDamage[playerid][8] = 1.0; // 8 - Katana
	HitReg@weaponDamage[playerid][9] = 1.0; // 9 - Chainsaw
	HitReg@weaponDamage[playerid][10] = 1.0; // 10 - Dildo
	HitReg@weaponDamage[playerid][11] = 1.0; // 11 - Dildo 2
	HitReg@weaponDamage[playerid][12] = 1.0; // 12 - Vibrator
	HitReg@weaponDamage[playerid][13] = 1.0; // 13 - Vibrator 2
	HitReg@weaponDamage[playerid][14] = 1.0; // 14 - Flowers
	HitReg@weaponDamage[playerid][15] = 1.0; // 15 - Cane
	HitReg@weaponDamage[playerid][16] = 82.5; // 16 - Grenade
	HitReg@weaponDamage[playerid][18] = 1.0; // 18 - Molotov
	HitReg@weaponDamage[playerid][19] = 9.9; // 19 - Vehicle M4 (custom)
	HitReg@weaponDamage[playerid][20] = 46.2; // 20 - Vehicle minigun (custom)
	HitReg@weaponDamage[playerid][21] = 82.5; // 21 - Vehicle rocket (custom)
	HitReg@weaponDamage[playerid][22] = 8.25; // 22 - Colt 45
	HitReg@weaponDamage[playerid][23] = 13.2; // 23 - Silenced
	HitReg@weaponDamage[playerid][24] = 46.2; // 24 - Deagle
	HitReg@weaponDamage[playerid][25] = 3.3; // 25 - Shotgun
	HitReg@weaponDamage[playerid][26] = 3.3; // 26 - Sawed-off
	HitReg@weaponDamage[playerid][27] = 4.95; // 27 - Spas
	HitReg@weaponDamage[playerid][28] = 6.6; // 28 - UZI
	HitReg@weaponDamage[playerid][29] = 8.25; // 29 - MP5
	HitReg@weaponDamage[playerid][30] = 9.9; // 30 - AK47
	HitReg@weaponDamage[playerid][31] = 9.9; // 31 - M4
	HitReg@weaponDamage[playerid][32] = 6.6; // 32 - Tec9
	HitReg@weaponDamage[playerid][33] = 24.75; // 33 - Cuntgun
	HitReg@weaponDamage[playerid][34] = 41.25; // 34 - Sniper
	HitReg@weaponDamage[playerid][35] = 82.5; // 35 - Rocket launcher
	HitReg@weaponDamage[playerid][36] = 82.5; // 36 - Heatseeker
	HitReg@weaponDamage[playerid][37] = 1.0; // 37 - Flamethrower
	HitReg@weaponDamage[playerid][38] = 46.2; // 38 - Minigun
	HitReg@weaponDamage[playerid][39] = 82.5; // 39 - Satchel
	HitReg@weaponDamage[playerid][41] = 0.33; // 41 - Spraycan
	HitReg@weaponDamage[playerid][42] = 0.33; // 42 - Fire extinguisher
	HitReg@weaponDamage[playerid][48] = 2.64; // 48 - Pistol whip (custom)
	HitReg@weaponDamage[playerid][49] = 9.9; // 49 - Vehicle
	HitReg@weaponDamage[playerid][50] = 330.0; // 50 - Helicopter blades
	HitReg@weaponDamage[playerid][51] = 82.5; // 51 - Explosion
	HitReg@weaponDamage[playerid][52] = 1.0; // 52 - Car park (custom)
	HitReg@weaponDamage[playerid][53] = 0.0; // 52 - Drowning
	HitReg@weaponDamage[playerid][54] = 165.0; // 52 - Splat

	SetPlayerTeam(playerid, 0x100);
	
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

public OnPlayerSpawn(playerid) {
	HitReg@playerDeathData[playerid][0] = 0;
	HitReg@playerDeathData[playerid][1] = INVALID_PLAYER_ID;
	HitReg@playerDeathData[playerid][2] = WEAPON_UNKNOWN;
	
	KillTimer(HitReg@knifeKillTimer[playerid]);
	HitReg@knifeKillTimer[playerid] = -1;
	
	#if defined HitReg_OnPlayerSpawn
		return HitReg_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn HitReg_OnPlayerSpawn
#if defined HitReg_OnPlayerSpawn
	forward HitReg_OnPlayerSpawn(playerid);
#endif

public OnPlayerDeath(playerid, killerid, reason) {
	if (HitReg@playerDeathData[playerid][0] == 0) {
	    return 0;
	}
	
	KillTimer(HitReg@knifeKillTimer[playerid]);

	killerid = HitReg@playerDeathData[playerid][1];
	reason = HitReg@playerDeathData[playerid][2];
	HitReg@playerDeathData[playerid][0] = 0;

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

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	if (newkeys & KEY_FIRE) {
	    new weaponid = GetPlayerWeapon(playerid);
	    
		switch (weaponid) {
			case WEAPON_GRENADE,
				 WEAPON_ROCKETLAUNCHER,
				 WEAPON_HEATSEEKER,
				 WEAPON_SATCHEL,
				 WEAPON_BOMB: {
				HitReg@lastExplosive[playerid] = (weaponid == WEAPON_BOMB) ? WEAPON_SATCHEL : weaponid;
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
	if (!HitReg_IsPlayerSpawned(playerid)) {
		return 0;
	}

    new bool:multiple_weapons;
	new avg_rate = HitReg_GetAverageFireRate(HitReg@lastShotID[playerid], HitReg@lastShotTickCount[playerid], HitReg@lastShotWeapon[playerid], sizeof(HitReg@lastShotTickCount[]), multiple_weapons);

	if (avg_rate != -1) {
		if (multiple_weapons) {
			if (avg_rate < 100) {
				return 0;
			}
		}
		else if ((DEFAULT_WEAPON_FIRERATE[weaponid] - avg_rate) > 20) {
			return 0;
		}
	}

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

	    if (_:maxrange == _:0.0) {
			return 0;
		}

		new Float:ox,
			Float:oy,
			Float:oz,
			Float:hx,
			Float:hy,
			Float:hz;
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
	if (!HitReg_IsPlayerSpawned(playerid)) {
		return 1;
	}

    new bool:multiple_weapons;
	new avg_rate = HitReg_GetAverageFireRate(HitReg@lastHitID[playerid], HitReg@lastHitTickCount[playerid], HitReg@lastHitWeapon[playerid], sizeof(HitReg@lastHitTickCount[]), multiple_weapons);

	if (avg_rate != -1) {
		if (multiple_weapons) {
			if (avg_rate < 100) {
				return 0;
			}
		}
		else if ((DEFAULT_WEAPON_FIRERATE[weaponid] - avg_rate) > 20) {
			return 0;
		}
	}
	
	if (weaponid == WEAPON_HELIBLADES && _:amount != _:330.0) {
		weaponid = WEAPON_CARPARK;
	}
	
	
	switch (weaponid) {
 		case WEAPON_FLAMETHROWER,
			 WEAPON_VEHICLE,
			 WEAPON_CARPARK,
			 WEAPON_EXPLOSION,
			 WEAPON_DROWN,
		 	 WEAPON_COLLISION: {
		    if (weaponid == WEAPON_EXPLOSION) {
            	if (issuerid == INVALID_PLAYER_ID) {
			    	if (IsPlayerInAnyVehicle(playerid) && GetPlayerVehicleSeat(playerid) == 0) {
					    weaponid = WEAPON_VEHICLE_ROCKETLAUNCHER;
					}
					else {
						if (HitReg@lastExplosive[playerid] != WEAPON_UNARMED) {
							weaponid = HitReg@lastExplosive[playerid];
						}
					}
				}
				else {
			    	if (IsPlayerInAnyVehicle(issuerid) && GetPlayerVehicleSeat(issuerid) == 0) {
					    weaponid = WEAPON_VEHICLE_ROCKETLAUNCHER;
					}
					else {
						if (HitReg@lastExplosive[issuerid] != WEAPON_UNARMED) {
							weaponid = HitReg@lastExplosive[issuerid];
						}
					}
				}
			}
				
			#if defined OnPlayerDamage
			    OnPlayerDamage(playerid, INVALID_PLAYER_ID, weaponid, amount, bodypart);
			#endif

    	    new Float:health,
				Float:armour;
			GetPlayerHealth(playerid, health);
			GetPlayerArmour(playerid, armour);

			if (weaponid == WEAPON_DROWN || weaponid == WEAPON_COLLISION) {
				armour = 0.0;
			}

			if (((armour + health) - amount) <= 0.0) {
			    HitReg@playerDeathData[playerid][0] = 1;
			    HitReg@playerDeathData[playerid][1] = INVALID_PLAYER_ID;
			    HitReg@playerDeathData[playerid][2] = weaponid;
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
	if (damagedid == INVALID_PLAYER_ID) {
		return 1;
	}
	
	switch (weaponid) {
    	case WEAPON_UNARMED..WEAPON_KATANA,
			 WEAPON_DILDO..WEAPON_CANE,
 			 WEAPON_PISTOLWHIP,
 			 WEAPON_CHAINSAW,
		  	 WEAPON_VEHICLE_M4,
			 WEAPON_VEHICLE_MINIGUN,
			 WEAPON_SHOTGSPA,
			 WEAPON_SHOTGUN,
			 WEAPON_SAWEDOFF,
			 WEAPON_COLT45..WEAPON_DEAGLE,
			 WEAPON_UZI..WEAPON_SNIPER,
			 WEAPON_MINIGUN,
			 WEAPON_FLAMETHROWER,
			 WEAPON_DROWN,
			 WEAPON_CARPARK,
		     WEAPON_SPRAYCAN,
			 WEAPON_FIREEXTINGUISHER: {
 			new bool:knifed = (weaponid == WEAPON_KNIFE && _:amount == _:0.0);

			new Float:health,
				Float:armour;

			if (!knifed) {
				if (!HitReg_ProcessDamage(damagedid, playerid, amount, weaponid, bodypart)) {
					return 1;
				}

				GetPlayerHealth(damagedid, health);
				GetPlayerArmour(damagedid, armour);

				if (armour > 0.0) {
					armour -= amount;

					if (armour < 0.0) {
						health += armour;
			   			armour = 0.0;
			 		}
				}
				else {
					health -= amount;
					if (health < 0.0) {
						health = 0.0;
					}
				}
			}
			else {
				amount = (health + armour);
				health = 0.0;
				armour = 0.0;
			}

			new bool:ret = true;
			#if defined OnPlayerDamage
		 		ret = bool:OnPlayerDamage(damagedid, playerid, weaponid, amount, bodypart);
			#endif

			if (!ret) {
				return 1;
			}

			if (HitReg@playerTeam[playerid] != NO_TEAM && HitReg@playerTeam[damagedid] != NO_TEAM && HitReg@playerTeam[playerid] == HitReg@playerTeam[damagedid]) {
		 		return 1;
			}

			if (knifed) {
			    KillTimer(HitReg@knifeKillTimer[playerid]);
			    HitReg@knifeKillTimer[playerid] = SetTimerEx("HitReg_OnPlayerKnifed", 2500, false, "ii", damagedid, playerid);
			}
			else {
				SetPlayerArmour(damagedid, armour);
				SetPlayerHealth(damagedid, health);

				if (((armour + health) - amount) <= 0.0) {
			 		HitReg@playerDeathData[damagedid][0] = 1;
			   		HitReg@playerDeathData[damagedid][1] = playerid;
			    	HitReg@playerDeathData[damagedid][2] = weaponid;
				}
			}
			 
			return 0;
		}
	}
	
	return 1;
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

public HitReg_OnPlayerKnifed(playerid, issuerid) {
    SetPlayerArmour(playerid, 0.0);
	SetPlayerHealth(playerid, 0.0);

	HitReg@playerDeathData[playerid][0] = 1;
	HitReg@playerDeathData[playerid][1] = issuerid;
	HitReg@playerDeathData[playerid][2] = WEAPON_KNIFE;
}

stock SetPlayerWeaponDamage(playerid, weaponid, Float:damage) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (weaponid < 0 || weaponid >= 53) {
	    return 0;
	}

    HitReg@weaponDamage[playerid][weaponid] = damage;
	return 1;
}

stock Float:GetPlayerWeaponDamage(playerid, weaponid) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0.0;
	}

	if (weaponid < 0 || weaponid >= 53) {
	    return 0.0;
	}

    return HitReg@weaponDamage[playerid][weaponid];
}

stock PlayerDeath(playerid, killerid, reason) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (killerid < 0 || killerid >= MAX_PLAYERS) {
	    return 0;
	}

	SetPlayerHealth(playerid, 0.0);

	HitReg@playerDeathData[playerid][0] = 1;
	HitReg@playerDeathData[playerid][1] = killerid;
	HitReg@playerDeathData[playerid][2] = reason;
	return 1;
}

stock HitReg_SetPlayerTeam(playerid, team) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	HitReg@playerTeam[playerid] = team;
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

	return HitReg@playerTeam[playerid];
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
        HitReg@playerDeathData[playerid][0] = 1;
		HitReg@playerDeathData[playerid][1] = INVALID_PLAYER_ID;
		HitReg@playerDeathData[playerid][2] = WEAPON_UNKNOWN;
	}
	
	return SetPlayerHealth(playerid, health);
}
#if defined _ALS_SetPlayerHealth
    #undef SetPlayerHealth
#else
    #define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth HitReg_SetPlayerHealth

stock HitReg_SendDeathMessage(playerid, killerid, reason) {
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
#define SendDeathMessage HitReg_SendDeathMessage
