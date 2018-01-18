#if defined pause_included
	#endinput
#endif
#define pause_included

#if defined OnPlayerPause
	forward OnPlayerPause(playerid);
#endif

#if defined OnPlayerResume
	forward OnPlayerResume(playerid, pausedtime);
#endif

IsPlayerPaused(playerid);
GetPlayerPausedTime(playerid);

enum E_PLAYER_PAUSED {
	E_PLAYER_PAUSED_LAST_TICKCOUNT,
	E_PLAYER_PAUSED_TIMER,
	bool:E_PLAYER_PAUSED_TOGGLE
};

static playerPauseData[MAX_PLAYERS][E_PLAYER_PAUSED];

public OnPlayerConnect(playerid) {
	playerPauseData[playerid][E_PLAYER_PAUSED_LAST_TICKCOUNT] = GetTickCount();
    playerPauseData[playerid][E_PLAYER_PAUSED_TIMER] = SetTimerEx("__OnPlayerTimerUpdate", 250, true, "i", playerid);
	playerPauseData[playerid][E_PLAYER_PAUSED_TOGGLE] = false;
	
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

public OnPlayerUpdate(playerid) {
	if (playerPauseData[playerid][E_PLAYER_PAUSED_TOGGLE]) { // player finally updated after being paused
		#if defined OnPlayerResume
	        OnPlayerResume(playerid, (GetTickCount() - playerPauseData[playerid][E_PLAYER_PAUSED_LAST_TICKCOUNT]));
		#endif
		
	    playerPauseData[playerid][E_PLAYER_PAUSED_TOGGLE] = false;
	    playerPauseData[playerid][E_PLAYER_PAUSED_TIMER] = SetTimerEx("__OnPlayerTimerUpdate", 250, true, "i", playerid);
	}
	
    playerPauseData[playerid][E_PLAYER_PAUSED_LAST_TICKCOUNT] = GetTickCount();
    
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

forward __OnPlayerTimerUpdate(playerid);
public __OnPlayerTimerUpdate(playerid) {
	new tick = GetTickCount();

	if ((tick - playerPauseData[playerid][E_PLAYER_PAUSED_LAST_TICKCOUNT]) >= 2000) { // if player hasn't updated in 2 seconds
	    playerPauseData[playerid][E_PLAYER_PAUSED_TOGGLE] = true;

		#if defined OnPlayerPause
	        OnPlayerPause(playerid);
		#endif
		
		KillTimer(playerPauseData[playerid][E_PLAYER_PAUSED_TIMER]);
	}
}

stock IsPlayerPaused(playerid) {
	return (playerid < 0 || playerid >= MAX_PLAYERS) ? (1) : (playerPauseData[playerid][E_PLAYER_PAUSED_TOGGLE]);
}

stock GetPlayerPausedTime(playerid) {
	return ((playerid < 0 || playerid >= MAX_PLAYERS) || !playerPauseData[playerid][E_PLAYER_PAUSED_TOGGLE]) ? (0) : (GetTickCount() - playerPauseData[playerid][E_PLAYER_PAUSED_LAST_TICKCOUNT]);
}
