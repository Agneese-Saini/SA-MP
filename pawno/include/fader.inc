#if defined fader_included
	#endinput
#endif
#define fader_included

#define RGBA(%0,%1,%2,%3,%4) \
	(((%1) = ((%0) >>> 24)),\
	((%2) = (((%0) >>> 16) & 0xFF)),\
	((%3) = (((%0) >>> 8) & 0xFF)),\
	((%4) = ((%0) & 0xFF)))
	
#define HEX(%0,%1,%2,%3,%4) \
	((%0) = ((%4) & 0xFF) | (((%3) & 0xFF) << 8) | (((%2) & 0xFF) << 16) | ((%1) << 24))

/**************************
*
* GLOBAL TEXTDRAW FADER
*
**************************/

#define MAX_TEXT_DRAW_FADES \
	(16)

enum E_TD_FADER
{
	E_TD_FADER_COLOR[4],
	E_TD_FADER_COLOR2[4],
	E_TD_FADER_TIMER,
	Text:E_TD_FADER_TEXT_ID
};
static textDrawFader[MAX_PLAYERS][MAX_TEXT_DRAW_FADES][E_TD_FADER];
static textDrawFaderCount[MAX_PLAYERS];

/*
native TextDrawFadeBoxForPlayer(playerid, Text:text, fromcolor, tocolor, updaterate = 2, fadeinterval = 50);
*/
stock TextDrawFadeBoxForPlayer(playerid, Text:text, fromcolor, tocolor, updaterate = 2, fadeinterval = 50)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (text < Text:0 || text >= Text:MAX_TEXT_DRAWS)
	{
	    return 0;
	}

	if (fromcolor == tocolor)
	{
	    return 0;
	}

	if (textDrawFaderCount[playerid] == MAX_TEXT_DRAW_FADES)
	{
	    return 0;
	}

	new index = textDrawFaderCount[playerid]++;

    RGBA(fromcolor, textDrawFader[playerid][index][E_TD_FADER_COLOR][0], textDrawFader[playerid][index][E_TD_FADER_COLOR][1], textDrawFader[playerid][index][E_TD_FADER_COLOR][2], textDrawFader[playerid][index][E_TD_FADER_COLOR][3]);
	RGBA(tocolor, textDrawFader[playerid][index][E_TD_FADER_COLOR2][0], textDrawFader[playerid][index][E_TD_FADER_COLOR2][1], textDrawFader[playerid][index][E_TD_FADER_COLOR2][2], textDrawFader[playerid][index][E_TD_FADER_COLOR2][3]);

	KillTimer(textDrawFader[playerid][index][E_TD_FADER_TIMER]);
    textDrawFader[playerid][index][E_TD_FADER_TIMER] = SetTimerEx("fader_OnTextDrawFadeForPlayer", fadeinterval, true, "iiiii", _:text, 1, updaterate, playerid, index);

	textDrawFader[playerid][index][E_TD_FADER_TEXT_ID] = text;

	TextDrawBoxColor(text, fromcolor);
	TextDrawShowForPlayer(playerid, text);
	return 1;
}

/*
native TextDrawFadeColorForPlayer(playerid, Text:text, fromcolor, tocolor, updaterate = 2, fadeinterval = 50);
*/
stock TextDrawFadeColorForPlayer(playerid, Text:text, fromcolor, tocolor, updaterate = 2, fadeinterval = 50)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (text < Text:0 || text >= Text:MAX_TEXT_DRAWS)
	{
	    return 0;
	}

	if (fromcolor == tocolor)
	{
	    return 0;
	}

	if (textDrawFaderCount[playerid] == MAX_TEXT_DRAW_FADES)
	{
	    return 0;
	}

	new index = textDrawFaderCount[playerid]++;

    RGBA(fromcolor, textDrawFader[playerid][index][E_TD_FADER_COLOR][0], textDrawFader[playerid][index][E_TD_FADER_COLOR][1], textDrawFader[playerid][index][E_TD_FADER_COLOR][2], textDrawFader[playerid][index][E_TD_FADER_COLOR][3]);
	RGBA(tocolor, textDrawFader[playerid][index][E_TD_FADER_COLOR2][0], textDrawFader[playerid][index][E_TD_FADER_COLOR2][1], textDrawFader[playerid][index][E_TD_FADER_COLOR2][2], textDrawFader[playerid][index][E_TD_FADER_COLOR2][3]);

	KillTimer(textDrawFader[playerid][index][E_TD_FADER_TIMER]);
    textDrawFader[playerid][index][E_TD_FADER_TIMER] = SetTimerEx("fader_OnTextDrawFoxFadeForPlayer", fadeinterval, true, "iiiii", _:text, 0, updaterate, playerid, index);

	textDrawFader[playerid][index][E_TD_FADER_TEXT_ID] = text;

	TextDrawColor(text, fromcolor);
	TextDrawShowForPlayer(playerid, text);
	return 1;
}

/*
native TextDrawStopFadeForPlayer(playerid, Text:text);
*/
stock TextDrawStopFadeForPlayer(playerid, Text:text)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (text < Text:0 || text >= Text:MAX_TEXT_DRAWS)
	{
	    return 0;
	}

	for (new i; i < textDrawFaderCount[playerid]; i++)
	{
		if (textDrawFader[playerid][i][E_TD_FADER_TEXT_ID] == text)
		{
		    KillTimer(textDrawFader[playerid][i][E_TD_FADER_TIMER]);

			for (new x = i; x < (textDrawFaderCount[playerid] - 1); x++)
			{
			    textDrawFader[playerid][x] = textDrawFader[playerid][x + 1];
			}
	        textDrawFaderCount[playerid]--;
		    return 1;
		}
	}

	return 0;
}

forward fader_OnTextDrawFadeForPlayer(Text:text, bool:box, updaterate, playerid, index);
public fader_OnTextDrawFadeForPlayer(Text:text, bool:box, updaterate, playerid, index)
{
	new faded;
	new color;
	for (new i; i < 4; i++)
	{
		if (textDrawFader[playerid][index][E_TD_FADER_COLOR][i] > textDrawFader[playerid][index][E_TD_FADER_COLOR2][i])
		{
		    textDrawFader[playerid][index][E_TD_FADER_COLOR][i] -= updaterate;
		    if (textDrawFader[playerid][index][E_TD_FADER_COLOR][i] < textDrawFader[playerid][index][E_TD_FADER_COLOR2][i])
		    {
		    	textDrawFader[playerid][index][E_TD_FADER_COLOR][i] = textDrawFader[playerid][index][E_TD_FADER_COLOR2][i];
		    	faded++;
		    }

		    HEX(color, textDrawFader[playerid][index][E_TD_FADER_COLOR][0], textDrawFader[playerid][index][E_TD_FADER_COLOR][1], textDrawFader[playerid][index][E_TD_FADER_COLOR][2], textDrawFader[playerid][index][E_TD_FADER_COLOR][3]);
		    if (box)
			{
				TextDrawBoxColor(text, color);
			}
			else
			{
				TextDrawColor(text, color);
			}
			TextDrawShowForPlayer(playerid, text);
		}
		else if (textDrawFader[playerid][index][E_TD_FADER_COLOR][i] < textDrawFader[playerid][index][E_TD_FADER_COLOR2][i])
		{
		    textDrawFader[playerid][index][E_TD_FADER_COLOR][i] += updaterate;
		    if (textDrawFader[playerid][index][E_TD_FADER_COLOR][i] > textDrawFader[playerid][index][E_TD_FADER_COLOR2][i])
		    {
		    	textDrawFader[playerid][index][E_TD_FADER_COLOR][i] = textDrawFader[playerid][index][E_TD_FADER_COLOR2][i];
		    	faded++;
		    }

			HEX(color, textDrawFader[playerid][index][E_TD_FADER_COLOR][0], textDrawFader[playerid][index][E_TD_FADER_COLOR][1], textDrawFader[playerid][index][E_TD_FADER_COLOR][2], textDrawFader[playerid][index][E_TD_FADER_COLOR][3]);
		    if (box)
			{
				TextDrawBoxColor(text, color);
			}
			else
			{
				TextDrawColor(text, color);
			}
			TextDrawShowForPlayer(playerid, text);
		}
		else
		{
		    faded++;
		}
	}

	if (faded == 4)
	{
	    KillTimer(textDrawFader[playerid][index][E_TD_FADER_TIMER]);

		for (new i = index; i < (textDrawFaderCount[playerid] - 1); i++)
		{
		    textDrawFader[playerid][i] = textDrawFader[playerid][i + 1];
		}
        textDrawFaderCount[playerid]--;

	    #if defined OnTextDrawFaded
	        OnTextDrawFaded(text, playerid);
		#endif
	}
}

#if defined OnTextDrawFaded
	forward OnTextDrawFaded(Text:text, forplayerid);
#endif

/**************************
*
* PLAYER TEXTDRAW FADER
*
**************************/

#define MAX_PLAYER_TEXT_DRAW_FADES \
	(16)
	
enum E_PLAYER_TD_FADER
{
	E_PLAYER_TD_FADER_COLOR[4],
	E_PLAYER_TD_FADER_COLOR2[4],
	E_PLAYER_TD_FADER_TIMER,
	PlayerText:E_PLAYER_TD_FADER_TEXT_ID
};
static playerTextDrawFader[MAX_PLAYERS][MAX_PLAYER_TEXT_DRAW_FADES][E_PLAYER_TD_FADER];
static playerTextDrawFaderCount[MAX_PLAYERS];

/*
native PlayerTextDrawFadeBox(playerid, PlayerText:playertext, fromcolor, tocolor, updaterate = 2, fadeinterval = 50);
*/
stock PlayerTextDrawFadeBox(playerid, PlayerText:playertext, fromcolor, tocolor, updaterate = 2, fadeinterval = 50)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (playertext < PlayerText:0 || playertext >= PlayerText:MAX_PLAYER_TEXT_DRAWS)
	{
	    return 0;
	}

	if (fromcolor == tocolor)
	{
	    return 0;
	}

	if (playerTextDrawFaderCount[playerid] == MAX_PLAYER_TEXT_DRAW_FADES)
	{
	    return 0;
	}

	new index = playerTextDrawFaderCount[playerid]++;

    RGBA(fromcolor, playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][0], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][1], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][2], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][3]);
	RGBA(tocolor, playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR2][0], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR2][1], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR2][2], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR2][3]);

	KillTimer(playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_TIMER]);
    playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_TIMER] = SetTimerEx("fader_OnPlayerTextDrawFade", fadeinterval, true, "iiiii", _:playertext, 1, updaterate, playerid, index);

	playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_TEXT_ID] = playertext;

	PlayerTextDrawBoxColor(playerid, playertext, fromcolor);
	PlayerTextDrawShow(playerid, playertext);
	return 1;
}

/*
native PlayerTextDrawFadeColor(playerid, PlayerText:playertext, fromcolor, tocolor, updaterate = 2, fadeinterval = 50);
*/
stock PlayerTextDrawFadeColor(playerid, PlayerText:playertext, fromcolor, tocolor, updaterate = 2, fadeinterval = 50)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (playertext < PlayerText:0 || playertext >= PlayerText:MAX_PLAYER_TEXT_DRAWS)
	{
	    return 0;
	}

	if (fromcolor == tocolor)
	{
	    return 0;
	}

	if (playerTextDrawFaderCount[playerid] == MAX_PLAYER_TEXT_DRAW_FADES)
	{
	    return 0;
	}

	new index = playerTextDrawFaderCount[playerid]++;

    RGBA(fromcolor, playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][0], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][1], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][2], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][3]);
	RGBA(tocolor, playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR2][0], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR2][1], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR2][2], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR2][3]);

	KillTimer(playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_TIMER]);
    playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_TIMER] = SetTimerEx("fader_OnPlayerTextDrawFade", fadeinterval, true, "iiiii", _:playertext, 0, updaterate, playerid, index);

	playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_TEXT_ID] = playertext;

	PlayerTextDrawColor(playerid, playertext, fromcolor);
	PlayerTextDrawShow(playerid, playertext);
	return 1;
}

/*
native PlayerTextDrawStopFade(playerid, PlayerText:playertext);
*/
stock PlayerTextDrawStopFade(playerid, PlayerText:playertext)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (playertext < PlayerText:0 || playertext >= PlayerText:MAX_PLAYER_TEXT_DRAWS)
	{
	    return 0;
	}

	for (new i; i < playerTextDrawFaderCount[playerid]; i++)
	{
		if (playerTextDrawFader[playerid][i][E_PLAYER_TD_FADER_TEXT_ID] == playertext)
		{
		    KillTimer(playerTextDrawFader[playerid][i][E_PLAYER_TD_FADER_TIMER]);

			for (new x = i; x < (playerTextDrawFaderCount[playerid] - 1); x++)
			{
			    playerTextDrawFader[playerid][x] = playerTextDrawFader[playerid][x + 1];
			}
	        playerTextDrawFaderCount[playerid]--;
		    return 1;
		}
	}

	return 0;
}

forward fader_OnPlayerTextDrawFade(PlayerText:playertext, bool:box, updaterate, playerid, index);
public fader_OnPlayerTextDrawFade(PlayerText:playertext, bool:box, updaterate, playerid, index)
{
	new faded;
	new color;
	for (new i; i < 4; i++)
	{
		if (playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][i] > playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR2][i])
		{
		    playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][i] -= updaterate;
		    if (playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][i] < playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR2][i])
		    {
		    	playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][i] = playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR2][i];
		    	faded++;
		    }

		    HEX(color, playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][0], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][1], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][2], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][3]);
		    if (box)
			{
				PlayerTextDrawBoxColor(playerid, playertext, color);
			}
			else
			{
				PlayerTextDrawColor(playerid, playertext, color);
			}
			PlayerTextDrawShow(playerid, playertext);
		}
		else if (playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][i] < playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR2][i])
		{
		    playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][i] += updaterate;
		    if (playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][i] > playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR2][i])
		    {
		    	playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][i] = playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR2][i];
		    	faded++;
		    }

			HEX(color, playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][0], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][1], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][2], playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_COLOR][3]);
		    if (box)
			{
				PlayerTextDrawBoxColor(playerid, playertext, color);
			}
			else
			{
				PlayerTextDrawColor(playerid, playertext, color);
			}
			PlayerTextDrawShow(playerid, playertext);
		}
		else
		{
		    faded++;
		}
	}

	if (faded == 4)
	{
	    KillTimer(playerTextDrawFader[playerid][index][E_PLAYER_TD_FADER_TIMER]);

		for (new i = index; i < (playerTextDrawFaderCount[playerid] - 1); i++)
		{
		    playerTextDrawFader[playerid][i] = playerTextDrawFader[playerid][i + 1];
		}
        playerTextDrawFaderCount[playerid]--;

	    #if defined OnPlayerTextDrawFaded
	        OnPlayerTextDrawFaded(playertext, playerid);
		#endif
	}
}

#if defined OnPlayerTextDrawFaded
	forward OnPlayerTextDrawFaded(PlayerText:playertext, forplayerid);
#endif
