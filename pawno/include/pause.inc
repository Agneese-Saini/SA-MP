#if defined pause_included
	#endinput
#endif
#define pause_included

/**************************************

// pause.inc || v2.0 || Last Updated: 19 Sept, 2018 || By Gammix

***************************************

// List Of Functions

native IsPlayerPaused(playerid);
native GetPlayerPausedTime(playerid);

***************************************

// List Of Callbacks

forward OnPlayerPause(playerid);
OnPlayerResume(playerid, pausedtime);

**************************************/

enum E_PLAYER_PAUSED_DATA {
	PLAYER_PAUSED_LAST_CLASSID,
	PLAYER_PAUSED_LAST_UPDATE,
	PLAYER_PAUSED_TIMER,
	bool:PLAYER_PAUSED_VALID
}

static playerPauseData[MAX_PLAYERS][E_PLAYER_PAUSED_DATA];

#if defined OnPlayerPause
	forward OnPlayerPause(playerid);
#endif

#if defined OnPlayerResume
	forward OnPlayerResume(playerid, pausedtime);
#endif

public OnPlayerConnect(playerid) {
    playerPauseData[playerid][PLAYER_PAUSED_LAST_CLASSID] = -1;
	playerPauseData[playerid][PLAYER_PAUSED_LAST_UPDATE] = -1;
    playerPauseData[playerid][PLAYER_PAUSED_TIMER] = -1;
    playerPauseData[playerid][PLAYER_PAUSED_VALID] = false;
    
	#if defined Pause_OnPlayerConnect
       	return Pause_OnPlayerConnect(playerid);
	#else
	    return 1;
 	#endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect Pause_OnPlayerConnect
#if defined Pause_OnPlayerConnect
    forward Pause_OnPlayerConnect(playerid);
#endif

public OnPlayerRequestClass(playerid, classid) {
    if (playerPauseData[playerid][PLAYER_PAUSED_VALID] == true) {
		if (playerPauseData[playerid][PLAYER_PAUSED_LAST_CLASSID] != classid) {
			#if defined OnPlayerResume
		    	OnPlayerResume(playerid, (gettime() - playerPauseData[playerid][PLAYER_PAUSED_LAST_UPDATE]));
			#endif

			KillTimer(playerPauseData[playerid][PLAYER_PAUSED_TIMER]);

			playerPauseData[playerid][PLAYER_PAUSED_LAST_UPDATE] = gettime();
			playerPauseData[playerid][PLAYER_PAUSED_TIMER] = -1;
			playerPauseData[playerid][PLAYER_PAUSED_VALID] = false;
		}
	}
	
	playerPauseData[playerid][PLAYER_PAUSED_LAST_CLASSID] = classid;

	#if defined Pause_OnPlayerRequestClass
       	return Pause_OnPlayerRequestClass(playerid, classid);
	#else
	    return 1;
 	#endif
}
#if defined _ALS_OnPlayerRequestClass
    #undef OnPlayerRequestClass
#else
    #define _ALS_OnPlayerRequestClass
#endif
#define OnPlayerRequestClass Pause_OnPlayerRequestClass
#if defined Pause_OnPlayerRequestClass
    forward Pause_OnPlayerRequestClass(playerid, classid);
#endif

public OnPlayerRequestSpawn(playerid) {
	if (playerPauseData[playerid][PLAYER_PAUSED_VALID] == true) {
        #if defined OnPlayerResume
			OnPlayerResume(playerid, (gettime() - playerPauseData[playerid][PLAYER_PAUSED_LAST_UPDATE]));
		#endif

		playerPauseData[playerid][PLAYER_PAUSED_LAST_UPDATE] = gettime();
		playerPauseData[playerid][PLAYER_PAUSED_TIMER] = -1;
		playerPauseData[playerid][PLAYER_PAUSED_VALID] = false;
	}

	#if defined Pause_OnPlayerRequestSpawn
       	return Pause_OnPlayerRequestSpawn(playerid);
	#else
	    return 1;
 	#endif
}
#if defined _ALS_OnPlayerRequestSpawn
    #undef OnPlayerRequestSpawn
#else
    #define _ALS_OnPlayerRequestSpawn
#endif
#define OnPlayerRequestSpawn Pause_OnPlayerRequestSpawn
#if defined Pause_OnPlayerRequestSpawn
    forward Pause_OnPlayerRequestSpawn(playerid);
#endif

public OnPlayerSpawn(playerid) {
	if (playerPauseData[playerid][PLAYER_PAUSED_TIMER] == -1) {
    	playerPauseData[playerid][PLAYER_PAUSED_TIMER] = SetTimerEx("__OnPlayerPauseCheck", 1000, true, "i", playerid);
    }

	#if defined Pause_OnPlayerSpawn
       	return Pause_OnPlayerSpawn(playerid);
	#else
	    return 1;
 	#endif
}
#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn Pause_OnPlayerSpawn
#if defined Pause_OnPlayerSpawn
    forward Pause_OnPlayerSpawn(playerid);
#endif

public OnPlayerUpdate(playerid) {
	if (playerPauseData[playerid][PLAYER_PAUSED_VALID] == true) { // player finally updated after being paused
		#if defined OnPlayerResume
			OnPlayerResume(playerid, (gettime() - playerPauseData[playerid][PLAYER_PAUSED_LAST_UPDATE]));
		#endif

		playerPauseData[playerid][PLAYER_PAUSED_LAST_UPDATE] = gettime();
		playerPauseData[playerid][PLAYER_PAUSED_TIMER] = SetTimerEx("__OnPlayerPauseCheck", 1000, true, "i", playerid);
		playerPauseData[playerid][PLAYER_PAUSED_VALID] = false;
	}

    playerPauseData[playerid][PLAYER_PAUSED_LAST_UPDATE] = gettime();

	#if defined Pause_OnPlayerUpdate
       	return Pause_OnPlayerUpdate(playerid);
	#else
	    return 1;
 	#endif
}
#if defined _ALS_OnPlayerUpdate
    #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate Pause_OnPlayerUpdate
#if defined Pause_OnPlayerUpdate
    forward Pause_OnPlayerUpdate(playerid);
#endif

forward __OnPlayerPauseCheck(playerid);
public __OnPlayerPauseCheck(playerid) {
	new time = gettime();

	if ((time - playerPauseData[playerid][PLAYER_PAUSED_LAST_UPDATE]) >= 5) { // if player hasn't updated in 5 seconds
		KillTimer(playerPauseData[playerid][PLAYER_PAUSED_TIMER]);
		
		playerPauseData[playerid][PLAYER_PAUSED_TIMER] = -1;
	    playerPauseData[playerid][PLAYER_PAUSED_VALID] = true;

		#if defined OnPlayerPause
	        OnPlayerPause(playerid);
		#endif
	}
	
	return 1;
}

stock IsPlayerPaused(playerid) {
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	return playerPauseData[playerid][PLAYER_PAUSED_VALID];
}

stock GetPlayerPausedTime(playerid) {
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;
		
	if (playerPauseData[playerid][PLAYER_PAUSED_VALID] == false)
	    return 0;
	    
	return (gettime() - playerPauseData[playerid][PLAYER_PAUSED_LAST_UPDATE]);
}
