#if defined vehicle_friendly_fire_included
	#endinput
#endif
#define vehicle_friendly_fire_included

enum E_VEHICLE_TIRES {
	E_VEHICLE_TIRES_STATUS,
    E_VEHICLE_TIRES_SHOTBY,
};

static vehicleTiresData[MAX_VEHICLES][E_VEHICLE_TIRES];
static bool:friendlyFireToggled = false;

#if defined OnPlayerFirendlyFireTires
	forward OnPlayerFirendlyFireTires(playerid, vehicleid, driverid);
#endif

public OnVehicleSpawn(vehicleid) {
	new panels, doors, lights, tires;
	GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);

	vehicleTiresData[vehicleid][E_VEHICLE_TIRES_STATUS] = tires;
	vehicleTiresData[vehicleid][E_VEHICLE_TIRES_SHOTBY] = INVALID_PLAYER_ID;

    #if defined VFFire_OnVehicleSpawn
        return VFFire_OnVehicleSpawn(vehicleid);
    #else
        return 1;
    #endif
}
#if defined VFFire_OnVehicleSpawn
    forward VFFire_OnVehicleSpawn(vehicleid);
#endif
#if defined _ALS_OnVehicleSpawn
    #undef OnVehicleSpawn
#else
    #define _ALS_OnVehicleSpawn
#endif
#define OnVehicleSpawn VFFire_OnVehicleSpawn

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ) {
    if (hittype == BULLET_HIT_TYPE_VEHICLE) {
        if (GetPlayerTeam(playerid) == NO_TEAM) { // if player is not in any team
            #if defined VFFire_OnPlayerWeaponShot
		        return VFFire_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
		    #else
		        return 1;
		    #endif
		}

        new driver = INVALID_PLAYER_ID;

        for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) {
			if (IsPlayerConnected(i)) {
			    if (GetPlayerVehicleID(i) == hitid && GetPlayerVehicleSeat(i) == 0) {
					driver = i;
					break;
				}
			}
		}

		if (driver == INVALID_PLAYER_ID || GetPlayerTeam(driver) == NO_TEAM || GetPlayerTeam(driver) != GetPlayerTeam(playerid)) { // if vehicle is not occupied by a teammate or not occupied at all; then don't continue
            #if defined VFFire_OnPlayerWeaponShot
		        return VFFire_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
		    #else
		        return 1;
		    #endif
		}
		
		new bool:shot = false;

    	new Float:offset[3];
		GetVehicleModelInfo(GetVehicleModel(hitid), VEHICLE_MODEL_INFO_WHEELSFRONT, offset[0], offset[1], offset[2]);

		/*
        ** FRONT LEFT WHEEL
        */
		if (fX >= (offset[0] - 2.4) && fX <= offset[0] &&
			fY >= (offset[1] - 0.4) && fY <= (offset[1] + 0.4) &&
			fZ >= (offset[2] - 0.4) && fZ <= (offset[2] + 0.4))
		{
		    shot = true;
		}
		/*
        ** FRONT RIGHT WHEEL
        */
		else if (fX >= offset[0] - 0.4 && fX <= offset[0] + 0.4 &&
				fY >= offset[1] - 0.4 && fY <= offset[1] + 0.4 &&
				fZ >= offset[2] - 0.4 && fZ <= offset[2] + 0.4)
		{
		    shot = true;
		}


		GetVehicleModelInfo(GetVehicleModel(hitid), VEHICLE_MODEL_INFO_WHEELSREAR, offset[0], offset[1], offset[2]);

		/*
        ** BACK LEFT WHEEL
        */
		if (fX >= offset[0] - 2.4 && fX <= offset[0] &&
			fY >= offset[1] - 0.4 && fY <= offset[1] + 0.4 &&
			fZ >= offset[2] - 0.4 && fZ <= offset[2] + 0.4)
		{
		    shot = true;
		}
		/*
        ** BACK RIGHT WHEEL
        */
		else if (fX >= offset[0] - 0.4 && fX <= offset[0] + 0.4 &&
				fY >= offset[1] - 0.4 && fY <= offset[1] + 0.4 &&
				fZ >= offset[2] - 0.4 && fZ <= offset[2] + 0.4)
		{
		    shot = true;
		}
		
		if (shot) { // if a tire was shot
		    new panels, doors, lights, tires;
			GetVehicleDamageStatus(hitid, panels, doors, lights, tires);

			vehicleTiresData[hitid][E_VEHICLE_TIRES_STATUS] = tires;
			vehicleTiresData[hitid][E_VEHICLE_TIRES_SHOTBY] = playerid;
		}
	}

	#if defined VFFire_OnPlayerWeaponShot
        return VFFire_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnPlayerWeaponShot
    #undef OnPlayerWeaponShot
#else
    #define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot VFFire_OnPlayerWeaponShot
#if defined VFFire_OnPlayerWeaponShot
    forward VFFire_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

public OnVehicleDamageStatusUpdate(vehicleid, playerid) {
	new panels, doors, lights, tires;
	GetVehicleDamageStatus(vehicleid, panels, doors, lights, tires);

	if (vehicleTiresData[vehicleid][E_VEHICLE_TIRES_SHOTBY] != INVALID_PLAYER_ID && tires != vehicleTiresData[vehicleid][E_VEHICLE_TIRES_STATUS]) {
		#if defined OnPlayerFirendlyFireTires
  			OnPlayerFirendlyFireTires(vehicleTiresData[vehicleid][E_VEHICLE_TIRES_SHOTBY], vehicleid, playerid);
		#endif

		if (friendlyFireToggled) { // reset tires to previous state if friendly fire is enabled
			tires = vehicleTiresData[vehicleid][E_VEHICLE_TIRES_STATUS];
			UpdateVehicleDamageStatus(vehicleid, panels, doors, lights, tires);
		}

		vehicleTiresData[vehicleid][E_VEHICLE_TIRES_SHOTBY] = INVALID_PLAYER_ID;
	}

	#if defined VFFire_OnVehicleDamageStatusUpd
        return VFFire_OnVehicleDamageStatusUpd(vehicleid, playerid);
    #else
        return 1;
    #endif
}
#if defined _ALS_OnVehicleDamageStatusUpd
    #undef OnVehicleDamageStatusUpdate
#else
    #define _ALS_OnVehicleDamageStatusUpd
#endif
#define OnVehicleDamageStatusUpdate VFFire_OnVehicleDamageStatusUpd
#if defined VFFire_OnVehicleDamageStatusUpd
    forward VFFire_OnVehicleDamageStatusUpd(vehicleid, playerid);
#endif
1y8
stock VFFire_EnableVFFire() {
    friendlyFireToggled = true;

	return EnableVehicleFriendlyFire();
}
#if defined _ALS_EnableVehicleFriendlyFire
    #undef EnableVehicleFriendlyFire
#else
    #define _ALS_EnableVehicleFriendlyFire
#endif
#define EnableVehicleFriendlyFire VFFire_EnableVFFire
