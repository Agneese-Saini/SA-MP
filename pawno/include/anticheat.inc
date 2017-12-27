#if defined anticheat_included
	#endinput
#endif
#define anticheat_included

/**********************************************/
// anticheat.inc | v1.0 | Updated: 27 Dec, 2017 | By Gammix
// Credits to Slice for "ProcessDamage" function, some code was take from his include used in OnPlayerGiveDamage.
/**********************************************/

#include <vending>

#define WEAPON_UNARMED 0
#define WEAPON_VEHICLE_M4 19
#define WEAPON_VEHICLE_MINIGUN 20
#define WEAPON_PISTOLWHIP 48
#define WEAPON_HELIBLADES 50
#define WEAPON_EXPLOSION 51
#define WEAPON_CARPARK 52
#define WEAPON_UNKNOWN 55

#define CHEAT_TYPE_HEALTH 0
#define CHEAT_TYPE_ARMOUR 1
#define CHEAT_TYPE_MONEY 2
#define CHEAT_TYPE_WEAPON 3
#define CHEAT_TYPE_WEAPON_AMMO 4
#define CHEAT_TYPE_WEAPON_RANGE 5
#define CHEAT_TYPE_WEAPON_DAMAGE 6

enum E_PLAYER_DEATH {
	E_PLAYER_DEATH_KILLERID,
    E_PLAYER_DEATH_REASON
};

enum E_PLAYER_WEAPON {
    E_PLAYER_WEAPON_WEAPONID,
    E_PLAYER_WEAPON_AMMO
};

static bool:IS_GAMEMODE = false;

static Float:playerHealth[MAX_PLAYERS];
static Float:playerArmour[MAX_PLAYERS];

static playerMoney[MAX_PLAYERS];

static playerTeamID[MAX_PLAYERS];

static playerDeathData[MAX_PLAYERS][E_PLAYER_DEATH];

static playerLastExplosive[MAX_PLAYERS];
static playerWeaponData[MAX_PLAYERS][13][E_PLAYER_WEAPON];
static Float:playerWeaponDamage[MAX_PLAYERS][55];

#if defined OnPlayerDamage
	forward OnPlayerDamage(playerid, issuerid, weaponid, Float:amount, bodypart, bool:death);
#endif

#if defined OnPlayerCheat
	forward OnPlayerCheat(playerid, type);
#endif

static HasSameTeam(playerid, targetid) {
	return ((playerTeamID[playerid] == NO_TEAM || playerTeamID[targetid] == NO_TEAM) ? (false) : (playerTeamID[playerid] == playerTeamID[targetid]));
}

static GetWeaponSlot(weaponid) {
	switch(weaponid) {
		case 0, 1: return 0; // No weapon
		case 2..9: return 1; // Melee
		case 22..24: return 2; // Handguns
		case 25..27: return 3; // Shotguns
		case 28, 29, 32: return 4; // Sub-Machineguns
		case 30, 31: return 5; // Machineguns
		case 33, 34: return 6; // Rifles
		case 35..38: return 7; // Heavy Weapons
		case 16, 18, 39: return 8; // Projectiles
		case 42, 43: return 9; // Special 1
		case 14: return 10; // Gifts
		case 44..46: return 11; // Special 2
		case 40: return 12; // Detonators
		default: return -1; // No slot
	}
	return -1;
}

public OnGameModeInit() {
	IS_GAMEMODE = true;

	#if defined LC_OnGameModeInit
		return LC_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit LC_OnGameModeInit
#if defined LC_OnGameModeInit
	forward LC_OnGameModeInit();
#endif

public OnPlayerConnect(playerid) {
	playerHealth[playerid] = 100.0;
	playerArmour[playerid] = 0.0;
	
	playerMoney[playerid] = 0;

	playerTeamID[playerid] = NO_TEAM;
	SetPlayerTeam(playerid, 655);

	playerDeathData[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
	playerDeathData[playerid][E_PLAYER_DEATH_REASON] = WEAPON_UNKNOWN;
	
	playerLastExplosive[playerid] = WEAPON_UNKNOWN;
	
	for (new i = 0; i < sizeof playerWeaponData[]; i++) {
        playerWeaponData[playerid][i][E_PLAYER_WEAPON_WEAPONID] = WEAPON_UNARMED;
        playerWeaponData[playerid][i][E_PLAYER_WEAPON_AMMO] = 0;
	}

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

public OnPlayerSpawn(playerid) {
    playerHealth[playerid] = 100.0;
    SetPlayerHealth(playerid, 100.0);

	playerArmour[playerid] = 0.0;
	SetPlayerArmour(playerid, 0.0);
	
	playerDeathData[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
	playerDeathData[playerid][E_PLAYER_DEATH_REASON] = WEAPON_UNKNOWN;

	playerLastExplosive[playerid] = WEAPON_UNKNOWN;

	for (new i = 0; i < sizeof playerWeaponData[]; i++) {
        playerWeaponData[playerid][i][E_PLAYER_WEAPON_WEAPONID] = WEAPON_UNARMED;
        playerWeaponData[playerid][i][E_PLAYER_WEAPON_AMMO] = 0;
	}
	
	#if defined LC_OnPlayerSpawn
		return LC_OnPlayerSpawn(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn LC_OnPlayerSpawn
#if defined LC_OnPlayerSpawn
	forward LC_OnPlayerSpawn(playerid);
#endif

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ) {
	if (hittype != BULLET_HIT_TYPE_NONE) {
	    new slot = GetWeaponSlot(weaponid);
	    if ((--playerWeaponData[playerid][slot][E_PLAYER_WEAPON_AMMO]) == 0) {
	   	 	playerWeaponData[playerid][slot][E_PLAYER_WEAPON_WEAPONID] = WEAPON_UNARMED;
            playerWeaponData[playerid][slot][E_PLAYER_WEAPON_AMMO] = 0;
		}
	
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
		    #if defined OnPlayerCheat
				OnPlayerCheat(playerid, CHEAT_TYPE_WEAPON_RANGE);
			#endif
			
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

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
	if (newkeys & KEY_FIRE) {
	    new weapon = GetPlayerWeapon(playerid);
	    switch (weapon) {
			case WEAPON_BOMB, WEAPON_SATCHEL: {
				playerLastExplosive[playerid] = WEAPON_SATCHEL;
			}
			
			case WEAPON_ROCKETLAUNCHER, WEAPON_HEATSEEKER, WEAPON_GRENADE: {
				playerLastExplosive[playerid] = weapon;
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

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart) {
	if (issuerid != INVALID_PLAYER_ID) {
		return 1;
	}

	new bool:death = false;
	switch (weaponid) {
		case WEAPON_FLAMETHROWER,
			 WEAPON_VEHICLE,
			 WEAPON_CARPARK,
			 WEAPON_EXPLOSION:
	 	{
	 	    if (weaponid == WEAPON_EXPLOSION) {
				weaponid = playerLastExplosive[playerid];
			}

			new Float:health = playerHealth[playerid];
			new Float:armour = playerArmour[playerid];

		 	death = (((armour + health) - amount) <= 0.0);
		 	
		 	armour -= amount;
		 	if (armour < 0.0) {
		 	    health += armour;
		 	    if (health < 0.0) {
					health = 0.0;
				}
				
				armour = 0.0;
			}
			
		 	playerHealth[playerid] = health;
		 	playerArmour[playerid] = armour;
		 	
			#if defined OnPlayerDamage
		 		OnPlayerDamage(playerid, INVALID_PLAYER_ID, weaponid, amount, bodypart, death);
			#endif
		}
		
		case WEAPON_DROWN,
			 WEAPON_COLLISION:
		{
		    new Float:health = playerHealth[playerid];

		 	death = ((health - amount) <= 0.0);

		 	health -= amount;
		 	if (health < 0.0) {
		 	    health = 0.0;
			}

		 	playerHealth[playerid] = health;

			#if defined OnPlayerDamage
		 		OnPlayerDamage(playerid, INVALID_PLAYER_ID, weaponid, amount, bodypart, death);
			#endif
		}

    	default: {
    	    return 1;
    	}
	}
	
	if (death) {
        playerDeathData[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
		playerDeathData[playerid][E_PLAYER_DEATH_REASON] = weaponid;
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
	#error OnPlayerTakeDamage isn't called anymore when using anticheat.inc, use OnPlayerDamage
#endif

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart) {
	if (HasSameTeam(playerid, damagedid)) {
		return 1;
	}
	
	if (IsPlayerInAnyVehicle(playerid)) {
	    if (GetPlayerVehicleSeat(playerid) == 0) {
	        if (weaponid == WEAPON_M4) {
	            weaponid = WEAPON_VEHICLE_M4;
	        }
	        else if (weaponid == WEAPON_MINIGUN) {
	            weaponid = WEAPON_MINIGUN;
	        }
	    }
	}
	else {
	    switch (weaponid) {
	        case WEAPON_UNARMED,
				 WEAPON_PISTOLWHIP,
				 WEAPON_BRASSKNUCKLE..WEAPON_CANE:
			 {
	            new Float:x, Float:y, Float:z, Float:dist;
				GetPlayerPos(playerid, x, y, z);
				dist = GetPlayerDistanceFromPoint(damagedid, x, y, z);

				if (dist > 15.0) {
					#if defined OnPlayerCheat
						OnPlayerCheat(playerid, CHEAT_TYPE_WEAPON_RANGE);
					#endif

					return 1;
				}
	        }
	    
			case WEAPON_COLT45..WEAPON_SNIPER,
 	 			 WEAPON_MINIGUN, WEAPON_SPRAYCAN, WEAPON_FIREEXTINGUISHER:
 			{
				if (_:amount == _:2.6400001049041748046875) {
					bodypart = weaponid;
					weaponid = WEAPON_PISTOLWHIP;
				}
			}
			
			case WEAPON_HELIBLADES: {
				if (_:amount != _:330.0) {
				    weaponid = WEAPON_CARPARK;
				}
			}

			case WEAPON_EXPLOSION: {
			    if (playerLastExplosive[playerid] != WEAPON_UNARMED) {
			    	weaponid = playerLastExplosive[playerid];
			    }
			}
		}
	}

	if (weaponid == WEAPON_CHAINSAW) {
		switch (amount) {
			case 6.6000003814697265625,
			     13.5300006866455078125,
			     16.1700000762939453125,
			     26.40000152587890625,
			     27.060001373291015625:
		 	{
		 	}

			default: {
				#if defined OnPlayerCheat
					OnPlayerCheat(playerid, CHEAT_TYPE_WEAPON_DAMAGE);
				#endif

				return 1;
			}
		}
	}
	else if (weaponid == WEAPON_DEAGLE) {
		switch (amount) {
			case 46.200000762939453125,
			     23.1000003814697265625:
		 	{
		 	}

			default: {
				#if defined OnPlayerCheat
					OnPlayerCheat(playerid, CHEAT_TYPE_WEAPON_DAMAGE);
				#endif

				return 1;
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
		#if defined OnPlayerCheat
			OnPlayerCheat(playerid, CHEAT_TYPE_WEAPON_DAMAGE);
		#endif

		return 1;
	}

	switch (weaponid) {
		case WEAPON_UNARMED,
			 WEAPON_PISTOLWHIP,
			 WEAPON_BRASSKNUCKLE..WEAPON_CANE:
		{
            amount *= playerWeaponDamage[playerid][weaponid];
		}

		case WEAPON_GRENADE,
			 WEAPON_ROCKETLAUNCHER,
			 WEAPON_HEATSEEKER,
			 WEAPON_SATCHEL,
			 WEAPON_EXPLOSION:
		{
		    amount /= 82.5;
		    amount *= playerWeaponDamage[playerid][weaponid];
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
		    amount = playerWeaponDamage[playerid][weaponid];
		}

		case WEAPON_SHOTGSPA: {
	 	    new Float:bullets = amount / 4.950000286102294921875;
			if (8.0 - bullets < -0.05) {
   	 			#if defined OnPlayerCheat
					OnPlayerCheat(playerid, CHEAT_TYPE_WEAPON_DAMAGE);
				#endif

				return 1;
			}

			new Float:f = floatfract(bullets);
			if (f > 0.01 && f < 0.99) {
   	 			#if defined OnPlayerCheat
					OnPlayerCheat(playerid, CHEAT_TYPE_WEAPON_DAMAGE);
				#endif

				return 1;
			}

	 	    amount = (playerWeaponDamage[playerid][weaponid] * bullets);
		}

		case WEAPON_SHOTGUN,
			 WEAPON_SAWEDOFF:
	 	{
	 	    new Float:bullets = amount / 3.30000019073486328125;
			if (15.0 - bullets < -0.05) {
   	 			#if defined OnPlayerCheat
					OnPlayerCheat(playerid, CHEAT_TYPE_WEAPON_DAMAGE);
				#endif

				return 1;
			}

			new Float:f = floatfract(bullets);
			if (f > 0.01 && f < 0.99) {
   	 			#if defined OnPlayerCheat
					OnPlayerCheat(playerid, CHEAT_TYPE_WEAPON_DAMAGE);
				#endif

				return 1;
			}

	 	    amount = (playerWeaponDamage[playerid][weaponid] * bullets);
		}
	}

	new Float:health = playerHealth[playerid];
	new Float:armour = playerArmour[playerid];

	new bool:death = (((armour + health) - amount) <= 0.0);

	armour -= amount;
	if (armour < 0.0) {
 		health += armour;
 	    if (health < 0.0) {
			health = 0.0;
		}

		armour = 0.0;
	}

    new bool:ret = false;
	#if defined OnPlayerDamage
 		ret = bool:OnPlayerDamage(damagedid, playerid, weaponid, amount, bodypart, death);
	#endif

	if (!ret) {
		return 1;
	}

 	playerHealth[playerid] = health;
 	SetPlayerHealth(playerid, health);

 	playerArmour[playerid] = armour;
 	SetPlayerArmour(playerid, armour);

    if (death) {
        playerDeathData[damagedid][E_PLAYER_DEATH_KILLERID] = playerid;
		playerDeathData[damagedid][E_PLAYER_DEATH_REASON] = weaponid;
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
	#error OnPlayerGiveDamage isn't called anymore when using anticheat.inc, use OnPlayerDamage
#endif

public OnPlayerUseVendingMachine(playerid, machineid) {
	if (GetPlayerMoney(playerid) < 1 || playerHealth[playerid] >= 100.0) {
	    #if defined LC_OnPlayerUseVendingMachine
			return LC_OnPlayerUseVendingMachine(playerid, machineid);
		#else
			return 0;
		#endif
	}
	
 	SetTimerEx("OnPlayerUsedVendingMachine", 2500, false, "i", playerid);

    #if defined LC_OnPlayerUseVendingMachine
		return LC_OnPlayerUseVendingMachine(playerid, machineid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerUseVendingMachine
	#undef OnPlayerUseVendingMachine
#else
	#define _ALS_OnPlayerUseVendingMachine
#endif

#define OnPlayerUseVendingMachine LC_OnPlayerUseVendingMachine
#if defined LC_OnPlayerUseVendingMachine
	forward LC_OnPlayerUseVendingMachine(playerid, machineid);
#endif

forward OnPlayerUsedVendingMachine(playerid);
public OnPlayerUsedVendingMachine(playerid) {
    playerHealth[playerid] += 35.0;
	if (playerHealth[playerid] > 100.0) {
		playerHealth[playerid] = 100.0;
	}

	SetPlayerHealth(playerid, playerHealth[playerid]);
}

public OnPlayerUpdate(playerid) {
	if (!IS_GAMEMODE) {
		#if defined LC_OnPlayerUpdate
			return LC_OnPlayerUpdate(playerid);
		#else
			return 1;
		#endif
	}

	new Float:health;
	GetPlayerHealth(playerid, health);
	if (health > playerHealth[playerid]) {
        #if defined OnPlayerCheat
        	if (!(bool:OnPlayerCheat(playerid, CHEAT_TYPE_HEALTH))) {
				return 1;
			}
		#endif

		SetPlayerHealth(playerid, playerHealth[playerid]);
	}

	new Float:armour;
	GetPlayerArmour(playerid, armour);
	if (armour > playerArmour[playerid]) {
        #if defined OnPlayerCheat
			if (!(bool:OnPlayerCheat(playerid, CHEAT_TYPE_ARMOUR))) {
				return 1;
			}
		#endif

		SetPlayerArmour(playerid, playerArmour[playerid]);
	}

	if (GetPlayerMoney(playerid) != playerMoney[playerid]) {
        #if defined OnPlayerCheat
			if (!(bool:OnPlayerCheat(playerid, CHEAT_TYPE_MONEY))) {
				return 1;
			}
		#endif
		
		ResetPlayerMoney(playerid);
		GivePlayerMoney(playerid, playerMoney[playerid]);
	}
	
	new weapon;
	new ammo;
	for (new i = 0; i < sizeof playerWeaponData[]; i++) {
		GetPlayerWeaponData(playerid, i, weapon, ammo);
		if (weapon == WEAPON_UNARMED) {
			continue;
		}
		
		if (weapon != playerWeaponData[playerid][i][E_PLAYER_WEAPON_WEAPONID]) {
            #if defined OnPlayerCheat
				if (!(bool:OnPlayerCheat(playerid, CHEAT_TYPE_WEAPON))) {
					return 1;
				}
			#endif

			ResetPlayerWeapons(playerid);

			for (i = 0; i < sizeof playerWeaponData[]; i++) {
			    GivePlayerWeapon(playerid, playerWeaponData[playerid][i][E_PLAYER_WEAPON_WEAPONID], playerWeaponData[playerid][i][E_PLAYER_WEAPON_AMMO]);
			}
			break;
		}
		
		if ((i >= 2 && i <= 6) || weapon == WEAPON_MINIGUN) {
			if (ammo != playerWeaponData[playerid][i][E_PLAYER_WEAPON_AMMO]) {
	            #if defined OnPlayerCheat
					if (!(bool:OnPlayerCheat(playerid, CHEAT_TYPE_WEAPON_AMMO))) {
						return 1;
					}
				#endif

				ResetPlayerWeapons(playerid);

				for (i = 0; i < sizeof playerWeaponData[]; i++) {
				    GivePlayerWeapon(playerid, playerWeaponData[playerid][i][E_PLAYER_WEAPON_WEAPONID], playerWeaponData[playerid][i][E_PLAYER_WEAPON_AMMO]);
				}
				break;
			}
		}
		else {
			if (ammo > playerWeaponData[playerid][i][E_PLAYER_WEAPON_AMMO]) {
	            #if defined OnPlayerCheat
					if (!(bool:OnPlayerCheat(playerid, CHEAT_TYPE_WEAPON_AMMO))) {
						return 1;
					}
				#endif

                playerWeaponData[playerid][i][E_PLAYER_WEAPON_WEAPONID] = WEAPON_UNARMED;
				playerWeaponData[playerid][i][E_PLAYER_WEAPON_AMMO] = 0;

				ResetPlayerWeapons(playerid);

				for (new x = 0; x < sizeof playerWeaponData[]; x++) {
				    if (x == i) {
						continue;
					}
					
				    GivePlayerWeapon(playerid, playerWeaponData[playerid][x][E_PLAYER_WEAPON_WEAPONID], playerWeaponData[playerid][x][E_PLAYER_WEAPON_AMMO]);
				}
				break;
			}
		}
	}

	#if defined LC_OnPlayerUpdate
		return LC_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerUpdate
    #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate LC_OnPlayerUpdate
#if defined LC_OnPlayerUpdate
	forward LC_OnPlayerUpdate(playerid);
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
	playerHealth[playerid] = 0.0;

	playerDeath[playerid][E_PLAYER_DEATH_KILLERID] = killerid;
	playerDeath[playerid][E_PLAYER_DEATH_REASON] = reason;
	return 1;
}

stock LC_ResetPlayerWeapons(playerid) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	for (new i = 0; i < sizeof playerWeaponData[]; i++) {
		playerWeaponData[playerid][i][E_PLAYER_WEAPON_WEAPONID] = WEAPON_UNARMED;
		playerWeaponData[playerid][i][E_PLAYER_WEAPON_AMMO] = 0;
	}
	return ResetPlayerWeapons(playerid);
}
#if defined _ALS_ResetPlayerWeapons
    #undef ResetPlayerWeapons
#else
    #define _ALS_ResetPlayerWeapons
#endif
#define ResetPlayerWeapons LC_ResetPlayerWeapons

stock LC_GivePlayerWeapon(playerid, weaponid, ammo) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}
	
	new slot = GetWeaponSlot(weaponid);
	if (slot == -1) {
		return 0;
	}

	playerWeaponData[playerid][slot][E_PLAYER_WEAPON_WEAPONID] = weaponid;
	playerWeaponData[playerid][slot][E_PLAYER_WEAPON_AMMO] += ammo;
	
	if (playerWeaponData[playerid][slot][E_PLAYER_WEAPON_AMMO] <= 0) {
        playerWeaponData[playerid][slot][E_PLAYER_WEAPON_WEAPONID] = WEAPON_UNARMED;
		playerWeaponData[playerid][slot][E_PLAYER_WEAPON_AMMO] = 0;
	}
	return GivePlayerWeapon(playerid, weaponid, ammo);
}
#if defined _ALS_GivePlayerWeapon
    #undef GivePlayerWeapon
#else
    #define _ALS_GivePlayerWeapon
#endif
#define GivePlayerWeapon LC_GivePlayerWeapon

stock LC_SetPlayerAmmo(playerid, weaponid, ammo) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	new slot = GetWeaponSlot(weaponid);
	if (slot == -1) {
		return 0;
	}

 	if (playerWeaponData[playerid][slot][E_PLAYER_WEAPON_WEAPONID] != weaponid) {
 	    return 0;
	}

	if (ammo < 0) {
        playerWeaponData[playerid][slot][E_PLAYER_WEAPON_WEAPONID] = WEAPON_UNARMED;
		playerWeaponData[playerid][slot][E_PLAYER_WEAPON_AMMO] = 0;
	}
	else {
		playerWeaponData[playerid][slot][E_PLAYER_WEAPON_AMMO] = ammo;
	}
	return SetPlayerAmmo(playerid, weapon, ammo);
}
#if defined _ALS_SetPlayerAmmo
    #undef SetPlayerAmmo
#else
    #define _ALS_SetPlayerAmmo
#endif
#define SetPlayerAmmo LC_SetPlayerAmmo

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

stock LC_ResetPlayerMoney(playerid) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	playerMoney[playerid] = 0;
	return ResetPlayerMoney(playerid);
}
#if defined _ALS_ResetPlayerMoney
    #undef ResetPlayerMoney
#else
    #define _ALS_ResetPlayerMoney
#endif
#define ResetPlayerMoney LC_ResetPlayerMoney

stock LC_GivePlayerMoney(playerid, money) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	playerMoney[playerid] += money;
	GivePlayerMoney(playerid, money);
	return 1;
}
#if defined _ALS_GivePlayerMoney
    #undef GivePlayerMoney
#else
    #define _ALS_GivePlayerMoney
#endif
#define GivePlayerMoney LC_GivePlayerMoney

stock LC_SetPlayerHealth(playerid, Float:health) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (health <= 0.0) {
	    health = 0.0;

		playerDeathData[playerid][E_PLAYER_DEATH_KILLERID] = INVALID_PLAYER_ID;
		playerDeathData[playerid][E_PLAYER_DEATH_REASON] = WEAPON_UNKNOWN;
	}

	playerHealth[playerid] = health;
	return SetPlayerHealth(playerid, health);
}
#if defined _ALS_SetPlayerHealth
    #undef SetPlayerHealth
#else
    #define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth LC_SetPlayerHealth

stock LC_GetPlayerHealth(playerid, &Float:health) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	health = playerHealth[playerid];
	return 1;
}
#if defined _ALS_GetPlayerHealth
    #undef GetPlayerHealth
#else
    #define _ALS_GetPlayerHealth
#endif
#define GetPlayerHealth LC_GetPlayerHealth

stock LC_SetPlayerArmour(playerid, Float:armour) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (armour <= 0.0) {
	    armour = 0.0;
	}

	playerArmour[playerid] = armour;
	return SetPlayerArmour(playerid, armour);
}
#if defined _ALS_SetPlayerHealth
    #undef SetPlayerHealth
#else
    #define _ALS_SetPlayerHealth
#endif
#define SetPlayerHealth LC_SetPlayerHealth

stock LC_GetPlayerArmour(playerid, &Float:armour) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	armour = playerArmour[playerid];
	return 1;
}
#if defined _ALS_GetPlayerArmour
    #undef GetPlayerArmour
#else
    #define _ALS_GetPlayerArmour
#endif
#define GetPlayerArmour LC_GetPlayerArmour
