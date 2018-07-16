#if defined fader_included
	#endinput
#endif
#define fader_included

#if ! defined MAX_TEXT_DRAW_FADES
	#define MAX_TEXT_DRAW_FADES (16) // max textdraws that can be faded once on a player's screen
#endif

#define RGBA(%0,%1,%2,%3,%4) \
	(((%1) = ((%0) >>> 24)),\
	((%2) = (((%0) >>> 16) & 0xFF)),\
	((%3) = (((%0) >>> 8) & 0xFF)),\
	((%4) = ((%0) & 0xFF)))

#define HEX(%0,%1,%2,%3,%4) \
	((%0) = ((%4) & 0xFF) | \
	(((%3) & 0xFF) << 8) | \
	(((%2) & 0xFF) << 16) | \
	((%1) << 24))

#define TEXTDRAW_FADE_TYPE_GLOBAL 0
#define TEXTDRAW_FADE_TYPE_PLAYER 1

#define TEXTDRAW_FADE_TEXT 0
#define TEXTDRAW_FADE_BOX 1

enum E_TEXTDRAW_FADE_DATA {
	bool:TEXTDRAW_FADE_VALID,
	TEXTDRAW_FADE_TEXTDRAW_ID,
	TEXTDRAW_FADE_TEXTDRAW_TYPE,
	TEXTDRAW_FADE_TIMER,
	TEXTDRAW_FADE_FROM_COLOR,
	TEXTDRAW_FADE_TO_COLOR,
	TEXTDRAW_FADE_TYPE
};

static textDrawFader[MAX_PLAYERS][MAX_TEXT_DRAW_FADES][E_TEXTDRAW_FADE_DATA];
static textDrawFaderCount[MAX_PLAYERS];

// Global textdraw functions
forward TextDrawFadeForPlayer(playerid, Text:text, from_color, to_color, update_rate = 2, timer_interval = 50);
forward TextDrawBoxFadeForPlayer(playerid, Text:text, from_color, to_color, update_rate = 2, timer_interval = 50);
forward TextDrawStopFadeForPlayer(playerid, Text:text);
forward TextDrawStopBoxFadeForPlayer(playerid, Text:text);
#if defined OnTextDrawFaded
	forward OnTextDrawFaded(playerid, Text:text, type);
#endif

// Player textdraw functions
forward PlayerTextDrawFade(playerid, PlayerText:text, from_color, to_color, update_rate = 2, timer_interval = 50);
forward PlayerTextDrawBoxFade(playerid, PlayerText:text, from_color, to_color, update_rate = 2, timer_interval = 50);
forward PlayerTextDrawStopFade(playerid, PlayerText:text);
forward PlayerTextDrawStopBoxFade(playerid, PlayerText:text);
#if defined OnPlayerTextDrawFaded
	forward OnPlayerTextDrawFaded(playerid, PlayerText:text, type);
#endif

static Internal_StopFade(playerid, idx) {
    KillTimer(textDrawFader[playerid][idx][TEXTDRAW_FADE_TIMER]);
   	textDrawFader[playerid][idx][TEXTDRAW_FADE_TIMER] = -1;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_VALID] = false;

	for (; idx < (textDrawFaderCount[playerid] - 1); idx++) {
	    textDrawFader[playerid][idx] = textDrawFader[playerid][idx + 1];
	}

	--textDrawFaderCount[playerid];
}

stock TextDrawFadeForPlayer(playerid, Text:text, from_color, to_color, update_rate = 2, timer_interval = 50) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (text < Text:0 || text >= Text:MAX_TEXT_DRAWS) {
	    return 0;
	}

	if (textDrawFaderCount[playerid] == MAX_TEXT_DRAW_FADES) {
		TextDrawStopFadeForPlayer(playerid, text);
	}

	new idx = textDrawFaderCount[playerid]++;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_VALID] = true;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_ID] = _:text;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_TYPE] = TEXTDRAW_FADE_TYPE_GLOBAL;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TIMER] = SetTimerEx("OnTextDrawFade", timer_interval, true, "iii", playerid, idx, update_rate);
	textDrawFader[playerid][idx][TEXTDRAW_FADE_FROM_COLOR] = from_color;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TO_COLOR] = to_color;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TYPE] = TEXTDRAW_FADE_TEXT;

	TextDrawColor(text, from_color);
	TextDrawShowForPlayer(playerid, text);
	return 1;
}

stock TextDrawBoxFadeForPlayer(playerid, Text:text, from_color, to_color, update_rate = 2, timer_interval = 50) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (text < Text:0 || text >= Text:MAX_TEXT_DRAWS) {
	    return 0;
	}

	if (textDrawFaderCount[playerid] == MAX_TEXT_DRAW_FADES) {
		TextDrawStopBoxFadeForPlayer(playerid, text);
	}

	new idx = textDrawFaderCount[playerid]++;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_VALID] = true;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_ID] = _:text;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_TYPE] = TEXTDRAW_FADE_TYPE_GLOBAL;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TIMER] = SetTimerEx("OnTextDrawFade", timer_interval, true, "iii", playerid, idx, update_rate);
	textDrawFader[playerid][idx][TEXTDRAW_FADE_FROM_COLOR] = from_color;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TO_COLOR] = to_color;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TYPE] = TEXTDRAW_FADE_BOX;

	TextDrawBoxColor(text, from_color);
	TextDrawShowForPlayer(playerid, text);
	return 1;
}

stock TextDrawStopFadeForPlayer(playerid, Text:text) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (text < Text:0 || text >= Text:MAX_TEXT_DRAWS) {
	    return 0;
	}

	for (new i = 0; i < textDrawFaderCount[playerid]; i++) {
		if (textDrawFader[playerid][i][TEXTDRAW_FADE_VALID] == true) {
			if (textDrawFader[playerid][i][TEXTDRAW_FADE_TEXTDRAW_ID] == _:text && textDrawFader[playerid][i][TEXTDRAW_FADE_TEXTDRAW_TYPE] == TEXTDRAW_FADE_TYPE_GLOBAL && textDrawFader[playerid][i][TEXTDRAW_FADE_TYPE] == TEXTDRAW_FADE_TEXT) {
				TextDrawHideForPlayer(playerid, Text:textDrawFader[playerid][i][TEXTDRAW_FADE_TEXTDRAW_ID]);
				Internal_StopFade(playerid, i);
				return 1;
			}
		}
	}

	return 0;
}

stock TextDrawStopBoxFadeForPlayer(playerid, Text:text) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (text < Text:0 || text >= Text:MAX_TEXT_DRAWS) {
	    return 0;
	}

	for (new i = 0; i < textDrawFaderCount[playerid]; i++) {
		if (textDrawFader[playerid][i][TEXTDRAW_FADE_VALID] == true) {
			if (textDrawFader[playerid][i][TEXTDRAW_FADE_TEXTDRAW_ID] == _:text && textDrawFader[playerid][i][TEXTDRAW_FADE_TEXTDRAW_TYPE] == TEXTDRAW_FADE_TYPE_GLOBAL && textDrawFader[playerid][i][TEXTDRAW_FADE_TYPE] == TEXTDRAW_FADE_BOX) {
				TextDrawHideForPlayer(playerid, Text:textDrawFader[playerid][i][TEXTDRAW_FADE_TEXTDRAW_ID]);
				Internal_StopFade(playerid, i);
				return 1;
			}
		}
	}

	return 0;
}

stock PlayerTextDrawFade(playerid, PlayerText:text, from_color, to_color, update_rate = 2, timer_interval = 50) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (text < PlayerText:0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS) {
	    return 0;
	}

	if (textDrawFaderCount[playerid] == MAX_TEXT_DRAW_FADES) {
		PlayerTextDrawStopFade(playerid, text);
	}

	new idx = textDrawFaderCount[playerid]++;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_VALID] = true;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_ID] = _:text;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_TYPE] = TEXTDRAW_FADE_TYPE_PLAYER;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TIMER] = SetTimerEx("OnTextDrawFade", timer_interval, true, "iii", playerid, idx, update_rate);
	textDrawFader[playerid][idx][TEXTDRAW_FADE_FROM_COLOR] = from_color;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TO_COLOR] = to_color;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TYPE] = TEXTDRAW_FADE_TEXT;

	PlayerTextDrawColor(playerid, text, from_color);
	PlayerTextDrawShow(playerid, text);
	return 1;
}

stock PlayerTextDrawBoxFade(playerid, PlayerText:text, from_color, to_color, update_rate = 2, timer_interval = 50) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (text < PlayerText:0 || text >= PlayerText:MAX_PLAYER_TEXT_DRAWS) {
	    return 0;
	}

	if (textDrawFaderCount[playerid] == MAX_TEXT_DRAW_FADES) {
		PlayerTextDrawStopBoxFade(playerid, text);
	}

	new idx = textDrawFaderCount[playerid]++;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_VALID] = true;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_ID] = _:text;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_TYPE] = TEXTDRAW_FADE_TYPE_PLAYER;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TIMER] = SetTimerEx("OnTextDrawFade", timer_interval, true, "iii", playerid, idx, update_rate);
	textDrawFader[playerid][idx][TEXTDRAW_FADE_FROM_COLOR] = from_color;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TO_COLOR] = to_color;
	textDrawFader[playerid][idx][TEXTDRAW_FADE_TYPE] = TEXTDRAW_FADE_BOX;

	PlayerTextDrawBoxColor(playerid, text, from_color);
	PlayerTextDrawShow(playerid, text);
	return 1;
}

stock PlayerTextDrawStopFade(playerid, PlayerText:text) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (text < PlayerText:0 || text >= PlayerText:MAX_TEXT_DRAWS) {
	    return 0;
	}

	for (new i = 0; i < textDrawFaderCount[playerid]; i++) {
		if (textDrawFader[playerid][i][TEXTDRAW_FADE_VALID] == true) {
			if (textDrawFader[playerid][i][TEXTDRAW_FADE_TEXTDRAW_ID] == _:text && textDrawFader[playerid][i][TEXTDRAW_FADE_TEXTDRAW_TYPE] == TEXTDRAW_FADE_TYPE_PLAYER && textDrawFader[playerid][i][TEXTDRAW_FADE_TYPE] == TEXTDRAW_FADE_TEXT) {
				PlayerTextDrawHide(playerid, PlayerText:textDrawFader[playerid][i][TEXTDRAW_FADE_TEXTDRAW_ID]);
				Internal_StopFade(playerid, i);
				return 1;
			}
		}
	}

	return 0;
}

stock PlayerTextDrawStopBoxFade(playerid, PlayerText:text) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (text < PlayerText:0 || text >= PlayerText:MAX_TEXT_DRAWS) {
	    return 0;
	}

	for (new i = 0; i < textDrawFaderCount[playerid]; i++) {
		if (textDrawFader[playerid][i][TEXTDRAW_FADE_VALID] == true) {
			if (textDrawFader[playerid][i][TEXTDRAW_FADE_TEXTDRAW_ID] == _:text && textDrawFader[playerid][i][TEXTDRAW_FADE_TEXTDRAW_TYPE] == TEXTDRAW_FADE_TYPE_PLAYER && textDrawFader[playerid][i][TEXTDRAW_FADE_TYPE] == TEXTDRAW_FADE_BOX) {
				PlayerTextDrawHide(playerid, PlayerText:textDrawFader[playerid][i][TEXTDRAW_FADE_TEXTDRAW_ID]);
				Internal_StopFade(playerid, i);
				return 1;
			}
		}
	}

	return 0;
}

forward OnTextDrawFade(playerid, idx, update_rate);
public OnTextDrawFade(playerid, idx, update_rate) {
	new newcolor[4];
	new finalcolor[4];
	
    RGBA(textDrawFader[playerid][idx][TEXTDRAW_FADE_FROM_COLOR], newcolor[0], newcolor[1], newcolor[2], newcolor[3]);
	RGBA(textDrawFader[playerid][idx][TEXTDRAW_FADE_TO_COLOR], finalcolor[0], finalcolor[1], finalcolor[2], finalcolor[3]);
	
	for (new i; i < sizeof newcolor; i++) {
		if (newcolor[i] > finalcolor[i]) {
		    newcolor[i] -= update_rate;

		    if (newcolor[i] <= finalcolor[i]) {
		    	newcolor[i] = finalcolor[i];
		    }
		}
		else if (newcolor[i] < finalcolor[i]) {
		    newcolor[i] += update_rate;

		    if (newcolor[i] >= finalcolor[i]) {
		    	newcolor[i] = finalcolor[i];
		    }
		}
	}
	
	HEX(textDrawFader[playerid][idx][TEXTDRAW_FADE_FROM_COLOR], newcolor[0], newcolor[1], newcolor[2], newcolor[3]);
	
	if (textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_TYPE] == TEXTDRAW_FADE_TYPE_GLOBAL) {
		if (textDrawFader[playerid][idx][TEXTDRAW_FADE_TYPE] == TEXTDRAW_FADE_TEXT) {
			TextDrawColor(Text:textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_ID], textDrawFader[playerid][idx][TEXTDRAW_FADE_FROM_COLOR]);
		} else {
			TextDrawBoxColor(Text:textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_ID], textDrawFader[playerid][idx][TEXTDRAW_FADE_FROM_COLOR]);
		}
		TextDrawShowForPlayer(playerid, Text:textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_ID]);
	}
	else {
		if (textDrawFader[playerid][idx][TEXTDRAW_FADE_TYPE] == TEXTDRAW_FADE_TEXT) {
			PlayerTextDrawColor(playerid, PlayerText:textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_ID], textDrawFader[playerid][idx][TEXTDRAW_FADE_FROM_COLOR]);
		} else {
			PlayerTextDrawBoxColor(playerid, PlayerText:textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_ID], textDrawFader[playerid][idx][TEXTDRAW_FADE_FROM_COLOR]);
		}
		PlayerTextDrawShow(playerid, PlayerText:textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_ID]);
	}


	if (textDrawFader[playerid][idx][TEXTDRAW_FADE_FROM_COLOR] == textDrawFader[playerid][idx][TEXTDRAW_FADE_TO_COLOR]) {
		if (textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_TYPE] == TEXTDRAW_FADE_TYPE_GLOBAL) {
			TextDrawHideForPlayer(playerid, Text:textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_ID]);

			#if defined OnTextDrawFaded
			    OnTextDrawFaded(playerid, Text:textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_ID], textDrawFader[playerid][idx][TEXTDRAW_FADE_TYPE]);
			#endif

			Internal_StopFade(playerid, idx);
		}
		else {
			PlayerTextDrawHide(playerid, PlayerText:textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_ID]);

			#if defined OnPlayerTextDrawFaded
			    OnPlayerTextDrawFaded(playerid, PlayerText:textDrawFader[playerid][idx][TEXTDRAW_FADE_TEXTDRAW_ID], textDrawFader[playerid][idx][TEXTDRAW_FADE_TYPE]);
			#endif
			
			Internal_StopFade(playerid, idx);
		}
	}
}
