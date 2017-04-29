#if defined script_init_included
	#endinput
#endif
#define script_init_included

#if defined OnScriptInit
	forward OnScriptInit();
#endif

#if defined OnScriptExit
	forward OnScriptExit();
#endif

new iScriptType;

#define IS_GAMEMODE \
    (iScriptType == 1)

#define IS_FILTERSCRIPT \
	(!IS_GAMEMODE)

public OnGameModeInit()
{
	if (iScriptType == 0)
	{
		iScriptType = 1;

		#if defined OnScriptInit
			OnScriptInit();
		#endif
	}

	#if defined SI_OnGameModeInit
	    return SI_OnGameModeInit();
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit SI_OnGameModeInit
#if defined SI_OnGameModeInit
	forward SI_OnGameModeInit();
#endif

public OnFilterScriptInit()
{
	if (iScriptType == 0)
	{
		iScriptType = 2;

		#if defined OnScriptInit
			OnScriptInit();
		#endif
	}

	#if defined SI_OnFilterScriptInit
	    return SI_OnFilterScriptInit();
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif
#define OnFilterScriptInit SI_OnFilterScriptInit
#if defined SI_OnFilterScriptInit
	forward SI_OnFilterScriptInit();
#endif

public OnGameModeExit()
{
	if (iScriptType == 1)
	{
		iScriptType = 0;

		#if defined OnScriptExit
			OnScriptExit();
		#endif
	}

	#if defined SI_OnGameModeExit
	    return SI_OnGameModeExit();
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif
#define OnGameModeExit SI_OnGameModeExit
#if defined SI_OnGameModeExit
	forward SI_OnGameModeExit();
#endif

public OnFilterScriptExit()
{
	if (iScriptType == 2)
	{
		iScriptType = 0;

		#if defined OnScriptExit
			OnScriptExit();
		#endif
	}

	#if defined SI_OnFilterScriptExit
	    return SI_OnFilterScriptExit();
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnFilterScriptExit
	#undef OnFilterScriptExit
#else
	#define _ALS_OnFilterScriptExit
#endif
#define OnFilterScriptExit SI_OnFilterScriptExit
#if defined SI_OnFilterScriptExit
	forward SI_OnFilterScriptExit();
#endif
