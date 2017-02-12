#if defined ss@vehicle_mods_included
	#endinput
#endif
#define ss@vehicle_mods_included

static const COMPONENT_PRICE[] =
{
	400, 550, 200, 250, 100, 150, 80, 500, 200, 500,
 	1000, 220, 250, 100, 400, 500, 200, 500, 350, 300,
    250, 200, 150, 350, 50, 1000, 480, 480, 770, 680,
    37, 370, 170, 120, 790, 150, 500, 690, 190, 390,
    500, 390, 1000, 500, 500, 510, 710, 670, 530, 810,
    620, 670, 530, 130, 210, 230, 520, 430, 620, 720,
    530, 180, 250, 430, 830, 850, 750, 250, 200, 550,
    450, 550, 450, 1000, 1030, 980, 1560, 1620, 1200,
	1030, 1000, 1230, 820, 1560, 1350, 770, 100, 1500,
	150, 650, 450, 100, 750, 350, 450, 350, 1000, 620,
	1140, 1000, 940, 780, 830, 3250, 1610, 1540, 780,
	780, 780, 1610, 1540, -1, -1, 3340, 3250, 2130, 2050,
	2130, 780, 940, 780, 940, 780, 860, 780, 1120, 3340,
	3250, 3340, 1650, 3380, 3290, 1590, 830, 800, 1500,
	1000, 800, 580, 470, 870, 980, 150, 150, 100, 100,
	490, 600, 890, 1000, 1090, 840, 910, 1200, 1030, 1030,
	920, 930, 550, 1050, 1050, 950, 650, 550, 450, 850,
	950, 850, 950, 970, 880, 990, 900, 950, 1000, 900,
	1000, 900, 2050, 2150, 2130, 2050, 2130, 2040, 2150,
	2040, 2095, 2175, 2080, 2200, 1200, 1040, 940, 1100
};

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
    GivePlayerMoney(playerid, -100);

	#if defined SS_OnVehicleRespray
		return SS_OnVehicleRespray(playerid, vehicleid, color1, color2);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleRespray
    #undef OnVehicleRespray
#else
    #define _ALS_OnVehicleRespray
#endif
#define OnVehicleRespray SS_OnVehicleRespray
#if defined SS_OnVehicleRespray
	forward SS_OnVehicleRespray(playerid, vehicleid, color1, color2);
#endif

public OnVehicleMod(playerid, vehicleid, componentid)
{
    new index = (componentid - 1000);
    if (!(0 <= index < sizeof(COMPONENT_PRICE)))
        return 0;

	GivePlayerMoney(playerid, -COMPONENT_PRICE[index]);
	
	#if defined SS_OnVehicleMod
		return SS_OnVehicleMod(playerid, vehicleid, componentid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleMod
    #undef OnVehicleMod
#else
    #define _ALS_OnVehicleMod
#endif
#define OnVehicleMod SS_OnVehicleMod
#if defined SS_OnVehicleMod
	forward SS_OnVehicleMod(playerid, vehicleid, componentid);
#endif
