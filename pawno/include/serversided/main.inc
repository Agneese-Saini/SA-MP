#if defined ss@main_included
	#endinput
#endif
#define ss@main_included

#include "serversided\script_init"
#include "serversided\weapon_config"
#include "serversided\vehicle_mods"
#include "serversided\static_pickups"

public OnScriptInit()
{
	printf("===============================================\n");
	printf("\tServersided.inc Loaded.");
	printf("\t  v3.0 (11 Feb, 2017)");
	printf("\t      By Gammix");
	printf("\n===============================================\n");
	
	#if defined SS_OnScriptInit
		return SS_OnScriptInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnScriptInit
	#undef OnScriptInit
#else
	#define _ALS_OnScriptInit
#endif
#define OnScriptInit SS_OnScriptInit
#if defined SS_OnScriptInit
	forward SS_OnScriptInit();
#endif
