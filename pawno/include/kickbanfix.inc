#if defined kickbanfix_included
	#endinput
#endif
#define kickbanfix_included

enum E_KICKBAN
{
	E_KICKBAN_TIMER,
	E_KICKBAN_TYPE,
	E_KICKBAN_REASON[150]
};
static playerKickbanDelay[MAX_PLAYERS][E_KICKBAN];

stock KB_Kick(playerid, delay = 50)
{
	if (! IsPlayerConnected(playerid) || playerKickbanDelay[playerid][E_KICKBAN_TIMER])
	{
		return 0;
	}

	CallRemoteFunction("OnPlayerDelayKickProcessed", "ii", playerid, SetTimerEx("OnPlayerDelayKicked", delay, false, "i", playerid));

	playerKickbanDelay[playerid][E_KICKBAN_TYPE] = 0;
	playerKickbanDelay[playerid][E_KICKBAN_REASON][0] = EOS;

	return 1;
}
#if defined _ALS_Kick
	#undef Kick
#else
	native BAD_Kick(playerid) = Kick;
	#define _ALS_Kick
#endif
#define Kick KB_Kick

stock KB_Ban(playerid, delay = 50)
{
	if (! IsPlayerConnected(playerid) || playerKickbanDelay[playerid][E_KICKBAN_TIMER])
	{
		return 0;
	}

	CallRemoteFunction("OnPlayerDelayKickProcessed", "ii", playerid, SetTimerEx("OnPlayerDelayKicked", delay, false, "i", playerid));

	playerKickbanDelay[playerid][E_KICKBAN_TYPE] = 1;
	playerKickbanDelay[playerid][E_KICKBAN_REASON][0] = EOS;

	return 1;
}
#if defined _ALS_Ban
	#undef Ban
#else
	native BAD_Ban(playerid) = Ban;
	#define _ALS_Ban
#endif
#define Ban KB_Ban

stock KB_BanEx(playerid, const reason[], delay = 50)
{
	if (! IsPlayerConnected(playerid) || playerKickbanDelay[playerid][E_KICKBAN_TIMER])
	{
		return 0;
	}

	CallRemoteFunction("OnPlayerDelayKickProcessed", "ii", playerid, SetTimerEx("OnPlayerDelayKicked", delay, false, "i", playerid));

	playerKickbanDelay[playerid][E_KICKBAN_TYPE] = 2;
	playerKickbanDelay[playerid][E_KICKBAN_REASON][0] = EOS;
	strcat(playerKickbanDelay[playerid][E_KICKBAN_REASON], reason);

	return 1;
}
#if defined _ALS_BanEx
	#undef BanEx
#else
	native BAD_BanEx(playerid, const reason[]) = BanEx;
	#define _ALS_BanEx
#endif
#define BanEx KB_BanEx

forward OnPlayerDelayKicked(playerid);
public	OnPlayerDelayKicked(playerid)
{
	CallRemoteFunction("OnPlayerDelayKickProcessed", "ii", playerid, 0);

	switch (playerKickbanDelay[playerid][E_KICKBAN_TYPE])
	{
		case 0:
		{
			BAD_Kick(playerid);
		}
		case 1:
		{
			BAD_Ban(playerid);
		}
		case 2:
		{
			BAD_BanEx(playerid, playerKickbanDelay[playerid][E_KICKBAN_REASON]);
		}
	}
}

forward OnPlayerDelayKickProcessed(playerid, i);
public  OnPlayerDelayKickProcessed(playerid, i)
{
    playerKickbanDelay[playerid][E_KICKBAN_TIMER] = i;
}

public OnPlayerConnect(playerid)
{
	playerKickbanDelay[playerid][E_KICKBAN_TIMER] = 0;

	#if defined KB_OnPlayerConnect
		return KB_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif

}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect KB_OnPlayerConnect
#if defined KB_OnPlayerConnect
	forward KB_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason)
{
	if (playerKickbanDelay[playerid][E_KICKBAN_TIMER])
	{
		KillTimer(playerKickbanDelay[playerid][E_KICKBAN_TIMER]);
		playerKickbanDelay[playerid][E_KICKBAN_TIMER] = 0;

		OnPlayerDelayKicked(playerid);
	}

	#if defined KB_OnPlayerDisconnect
		return KB_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect KB_OnPlayerDisconnect
#if defined KB_OnPlayerDisconnect
forward KB_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerUpdate(playerid)
{
	if (playerKickbanDelay[playerid][E_KICKBAN_TIMER])
	{
		return 0;
	}

	#if defined KB_OnPlayerUpdate
		return KB_OnPlayerUpdate(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerUpdate
	#undef OnPlayerUpdate
#else
	#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate KB_OnPlayerUpdate
#if defined KB_OnPlayerUpdate
	forward KB_OnPlayerUpdate(playerid);
#endif

public OnPlayerCommandText(playerid, cmdtext[])
{
	if (playerKickbanDelay[playerid][E_KICKBAN_TIMER])
	{
		return 0;
	}

	#if defined KB_OnPlayerCommandText
		return KB_OnPlayerCommandText(playerid, cmdtext);
	#else
		return 0;
	#endif
}
#if defined _ALS_OnPlayerCommandText
	#undef OnPlayerCommandText
#else
	#define _ALS_OnPlayerCommandText
#endif
#define OnPlayerCommandText KB_OnPlayerCommandText
#if defined KB_OnPlayerCommandText
	forward KB_OnPlayerCommandText(playerid, cmdtext[]);
#endif

public OnPlayerText(playerid, text[])
{
	if (playerKickbanDelay[playerid][E_KICKBAN_TIMER])
	{
		return 0;
	}

	#if defined KB_OnPlayerText
		return KB_OnPlayerText(playerid, text);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerText
	#undef OnPlayerText
#else
	#define _ALS_OnPlayerText
#endif
#define OnPlayerText KB_OnPlayerText
#if defined KB_OnPlayerText
	forward KB_OnPlayerText(playerid, text[]);
#endif

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if (playerKickbanDelay[playerid][E_KICKBAN_TIMER])
	{
		return 0;
	}

	#if defined KB_OnDialogResponse
		return KB_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif
#define OnDialogResponse KB_OnDialogResponse
#if defined KB_OnDialogResponse
	forward KB_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif

public OnPlayerDeath(playerid, killerid, reason)
{
	if (playerKickbanDelay[playerid][E_KICKBAN_TIMER])
	{
		return 0;
	}

	#if defined KB_OnPlayerDeath
		return KB_OnPlayerDeath(playerid, killerid, reason);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDeath
	#undef OnPlayerDeath
#else
	#define _ALS_OnPlayerDeath
#endif
#define OnPlayerDeath KB_OnPlayerDeath
#if defined KB_OnPlayerDeath
	forward KB_OnPlayerDeath(playerid, killerid, reason);
#endif

public OnVehicleMod(playerid, vehicleid, componentid)
{
	if (playerKickbanDelay[playerid][E_KICKBAN_TIMER])
	{
		return 0;
	}

	#if defined KB_OnVehicleMod
		return KB_OnVehicleMod(playerid, vehicleid, componentid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleMod
	#undef OnVehicleMod
#else
	#define _ALS_OnVehicleMod
#endif
#define OnVehicleMod KB_OnVehicleMod
#if defined KB_OnVehicleMod
	forward KB_OnVehicleMod(playerid, vehicleid, componentid);
#endif

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	if (playerKickbanDelay[playerid][E_KICKBAN_TIMER])
	{
		return 0;
	}

	#if defined KB_OnVehicleRespray
		return KB_OnVehicleRespray(playerid, vehicleid, color1, color2);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnVehicleRespray
	#undef OnVehicleRespray
#else
	#define _ALS_OnVehicleRespray
#endif
#define OnVehicleRespray KB_OnVehicleRespray
#if defined KB_OnVehicleRespray
	forward KB_OnVehicleRespray(playerid, vehicleid, color1, color2);
#endif
