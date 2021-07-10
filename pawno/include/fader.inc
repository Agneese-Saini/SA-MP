//////////////////////////////////
//
// FADER.INC BY GAMMIX
// VERSION: 3.0 (14 Sep, 2020)
//
//////////////////////////////////

#if defined fader_included
	#endinput
#endif
#define fader_included

#if !defined TEXTDRAW_FADER_POOL_SIZE
	#define TEXTDRAW_FADER_POOL_SIZE (15)
#endif

#define TEXTDRAW_TAG (tagof(Text:))
#define PLAYER_TEXTDRAW_TAG (tagof(PlayerText:))

enum fade_type {
	fade_type_text,
	fade_type_box
};

#if defined fader_natives
	native TextDrawFadeForPlayer(playerid, Text: text, from_color, to_color, fade_type: type = fade_type_box, timer_interval = 50, update_rate = 5, from_outline_color = 255, to_outline_color = 0x00);
	native TextDrawStopFadeForPlayer(playerid, Text: text, fade_type: type = fade_type_box);

	native PlayerTextDrawFade(playerid, PlayerText: playertext, from_color, to_color, fade_type: type = fade_type_box, timer_interval = 50, update_rate = 5, from_outline_color = 255, to_outline_color = 0x00);
	native PlayerTextDrawStopFade(playerid, PlayerText: playertext, fade_type: type = fade_type_box);
#endif

#if defined OnTextDrawFaded
forward OnTextDrawFaded(playerid, Text: text, fade_type: type, from_color, to_color, from_outline_color, to_outline_color);
#endif

#if defined OnPlayerTextDrawFaded
forward OnPlayerTextDrawFaded(playerid, PlayerText: playertext, fade_type: type, from_color, to_color, from_outline_color, to_outline_color);
#endif

enum E_TEXTDRAW_FADER {
    bool: E_FADER_SLOT_USED,
    E_FADER_TEXTDRAW_TAG,
    E_FADER_TEXTDRAW_ID,
    fade_type: E_FADER_TYPE,
	E_FADER_FROM_COLOR,
    E_FADER_TO_COLOR,
	E_FADER_FROM_OUTLINE_COLOR,
    E_FADER_TO_OUTLINE_COLOR,
    E_FADER_TIMER
};

stock static _textDrawFader[MAX_PLAYERS][TEXTDRAW_FADER_POOL_SIZE][E_TEXTDRAW_FADER];

stock static RGBA(color, &rr, &gg, &bb, &aa) {

	rr = color >>> 24;
	gg = (color >>> 16) & 0xFF;
	bb = (color >>> 8) & 0xFF;
	aa = color & 0xFF;
}

stock static HEX(&color, rr, gg, bb, aa) {

	color = (aa & 0xFF) | ((bb & 0xFF) << 8) | ((gg & 0xFF) << 16) | (rr << 24);
}

stock static ConvertColor(from_color, to_color, update_rate) {

	new new_color[4];
	new final_color[4];

    RGBA(from_color, new_color[0], new_color[1], new_color[2], new_color[3]);
	RGBA(to_color, final_color[0], final_color[1], final_color[2], final_color[3]);

	for (new i; i < 4; i++) {
		if (new_color[i] > final_color[i]) {
		    new_color[i] -= update_rate;

		    if (new_color[i] <= final_color[i]) {
		    	new_color[i] = final_color[i];
		    }
		} else if (new_color[i] < final_color[i]) {
		    new_color[i] += update_rate;

		    if (new_color[i] >= final_color[i]) {
		    	new_color[i] = final_color[i];
		    }
		}
	}

	new ret;
	HEX(ret, new_color[0], new_color[1], new_color[2], new_color[3]);
	
	return ret;
}

stock TextDrawFadeForPlayer(playerid, Text: text, from_color, to_color, fade_type: type = fade_type_box, timer_interval = 50, update_rate = 5, from_outline_color = 255, to_outline_color = 0x00) {

	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (text < Text: 0 || text >= Text: MAX_TEXT_DRAWS) {
	    return 0;
	}

	new idx = -1;
	for (new i = 0; i < TEXTDRAW_FADER_POOL_SIZE; i++) {
		if (_textDrawFader[playerid][i][E_FADER_SLOT_USED] == true) {
			if (_textDrawFader[playerid][i][E_FADER_TEXTDRAW_TAG] == TEXTDRAW_TAG && _textDrawFader[playerid][i][E_FADER_TEXTDRAW_ID] == _: text && _textDrawFader[playerid][i][E_FADER_TYPE] == type) {
				TextDrawStopFadeForPlayer(playerid, text, type);
				idx = i;
				break;
			}
		} else {
			if (idx == -1) {
		    	idx = i;
		    }
		}
	}

	if (idx == -1) {
		return 0;
	}


	_textDrawFader[playerid][idx][E_FADER_SLOT_USED] = true;
	_textDrawFader[playerid][idx][E_FADER_TEXTDRAW_TAG] = TEXTDRAW_TAG;
	_textDrawFader[playerid][idx][E_FADER_TEXTDRAW_ID] = _: text;
	_textDrawFader[playerid][idx][E_FADER_TYPE] = type;
	_textDrawFader[playerid][idx][E_FADER_FROM_COLOR] = from_color;
	_textDrawFader[playerid][idx][E_FADER_TO_COLOR] = to_color;
	_textDrawFader[playerid][idx][E_FADER_FROM_OUTLINE_COLOR] = from_outline_color;
	_textDrawFader[playerid][idx][E_FADER_TO_OUTLINE_COLOR] = to_outline_color;
	_textDrawFader[playerid][idx][E_FADER_TIMER] = SetTimerEx("_OnTextDrawFade", timer_interval, true, "iiiii", playerid, idx, update_rate, from_color, from_outline_color);

	return 1;
}

stock TextDrawStopFadeForPlayer(playerid, Text: text, fade_type: type = fade_type_box) {

	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (text < Text: 0 || text >= Text: MAX_TEXT_DRAWS) {
	    return 0;
	}

	for (new i = 0; i < TEXTDRAW_FADER_POOL_SIZE; i++) {
		if (_textDrawFader[playerid][i][E_FADER_SLOT_USED] == true && _textDrawFader[playerid][i][E_FADER_TEXTDRAW_TAG] == TEXTDRAW_TAG && _textDrawFader[playerid][i][E_FADER_TEXTDRAW_ID] == _: text && _textDrawFader[playerid][i][E_FADER_TYPE] == type) {
  			KillTimer(_textDrawFader[playerid][i][E_FADER_TIMER]);
  			_textDrawFader[playerid][i][E_FADER_SLOT_USED] = false;

			return 1;
		}
	}

	return 0;
}

stock PlayerTextDrawFade(playerid, PlayerText: playertext, from_color, to_color, fade_type: type = fade_type_box, timer_interval = 50, update_rate = 5, from_outline_color = 255, to_outline_color = 0x00) {

	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (playertext < PlayerText: 0 || playertext >= PlayerText: MAX_TEXT_DRAWS) {
	    return 0;
	}

	new idx = -1;
	for (new i = 0; i < TEXTDRAW_FADER_POOL_SIZE; i++) {
		if (_textDrawFader[playerid][i][E_FADER_SLOT_USED] == true) {
			if (_textDrawFader[playerid][i][E_FADER_TEXTDRAW_TAG] == PLAYER_TEXTDRAW_TAG && _textDrawFader[playerid][i][E_FADER_TEXTDRAW_ID] == _: playertext && _textDrawFader[playerid][i][E_FADER_TYPE] == type) {
				PlayerTextDrawStopFade(playerid, playertext, type);
				idx = i;
				break;
			}
		} else {
			if (idx == -1) {
		    	idx = i;
		    }
		}
	}

	if (idx == -1) {
		return 0;
	}


	_textDrawFader[playerid][idx][E_FADER_SLOT_USED] = true;
	_textDrawFader[playerid][idx][E_FADER_TEXTDRAW_TAG] = PLAYER_TEXTDRAW_TAG;
	_textDrawFader[playerid][idx][E_FADER_TEXTDRAW_ID] = _: playertext;
	_textDrawFader[playerid][idx][E_FADER_TYPE] = type;
	_textDrawFader[playerid][idx][E_FADER_FROM_COLOR] = from_color;
	_textDrawFader[playerid][idx][E_FADER_TO_COLOR] = to_color;
	_textDrawFader[playerid][idx][E_FADER_FROM_OUTLINE_COLOR] = from_outline_color;
	_textDrawFader[playerid][idx][E_FADER_TO_OUTLINE_COLOR] = to_outline_color;
	_textDrawFader[playerid][idx][E_FADER_TIMER] = SetTimerEx("_OnTextDrawFade", timer_interval, true, "iiiii", playerid, idx, update_rate, from_color, from_outline_color);

	return 1;
}

stock PlayerTextDrawStopFade(playerid, PlayerText: playertext, fade_type: type = fade_type_box) {

	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
	}

	if (playertext < PlayerText: 0 || playertext >= PlayerText: MAX_PLAYER_TEXT_DRAWS) {
	    return 0;
	}

	for (new i = 0; i < TEXTDRAW_FADER_POOL_SIZE; i++) {
		if (_textDrawFader[playerid][i][E_FADER_SLOT_USED] == true && _textDrawFader[playerid][i][E_FADER_TEXTDRAW_TAG] == PLAYER_TEXTDRAW_TAG && _textDrawFader[playerid][i][E_FADER_TEXTDRAW_ID] == _: playertext && _textDrawFader[playerid][i][E_FADER_TYPE] == type) {
  			KillTimer(_textDrawFader[playerid][i][E_FADER_TIMER]);
  			_textDrawFader[playerid][i][E_FADER_SLOT_USED] = false;

			return 1;
		}
	}

	return 0;
}

forward _OnTextDrawFade(playerid, idx, update_rate, initial_color, initial_outline_color);
public	_OnTextDrawFade(playerid, idx, update_rate, initial_color, initial_outline_color) {

	_textDrawFader[playerid][idx][E_FADER_FROM_COLOR] = ConvertColor(_textDrawFader[playerid][idx][E_FADER_FROM_COLOR], _textDrawFader[playerid][idx][E_FADER_TO_COLOR], update_rate);

	if (_textDrawFader[playerid][idx][E_FADER_TEXTDRAW_TAG] == TEXTDRAW_TAG) {

		if (_textDrawFader[playerid][idx][E_FADER_TYPE] == fade_type_text) {
			TextDrawColor(Text: _textDrawFader[playerid][idx][E_FADER_TEXTDRAW_ID], _textDrawFader[playerid][idx][E_FADER_FROM_COLOR]);

			_textDrawFader[playerid][idx][E_FADER_FROM_OUTLINE_COLOR] = ConvertColor(_textDrawFader[playerid][idx][E_FADER_FROM_OUTLINE_COLOR], _textDrawFader[playerid][idx][E_FADER_TO_OUTLINE_COLOR], update_rate);
			TextDrawBackgroundColor(Text: _textDrawFader[playerid][idx][E_FADER_TEXTDRAW_ID], _textDrawFader[playerid][idx][E_FADER_FROM_OUTLINE_COLOR]);
		} else {
			TextDrawBoxColor(Text: _textDrawFader[playerid][idx][E_FADER_TEXTDRAW_ID], _textDrawFader[playerid][idx][E_FADER_FROM_COLOR]);
		}

		TextDrawShowForPlayer(playerid, Text: _textDrawFader[playerid][idx][E_FADER_TEXTDRAW_ID]);

		if (_textDrawFader[playerid][idx][E_FADER_FROM_COLOR] == _textDrawFader[playerid][idx][E_FADER_TO_COLOR]) {
		    if (_textDrawFader[playerid][idx][E_FADER_TYPE] == fade_type_box || (_textDrawFader[playerid][idx][E_FADER_TYPE] == fade_type_text && _textDrawFader[playerid][idx][E_FADER_FROM_OUTLINE_COLOR] == _textDrawFader[playerid][idx][E_FADER_TO_OUTLINE_COLOR])) {
				#if defined OnTextDrawFaded
		  			OnTextDrawFaded(playerid, Text: _textDrawFader[playerid][idx][E_FADER_TEXTDRAW_ID], _textDrawFader[playerid][idx][E_FADER_TYPE], initial_color, _textDrawFader[playerid][idx][E_FADER_FROM_COLOR], initial_outline_color, _textDrawFader[playerid][idx][E_FADER_FROM_OUTLINE_COLOR]);
				#endif
			
				KillTimer(_textDrawFader[playerid][idx][E_FADER_TIMER]);
	  			_textDrawFader[playerid][idx][E_FADER_SLOT_USED] = false;
			}
		}

	} else {

		if (_textDrawFader[playerid][idx][E_FADER_TYPE] == fade_type_text) {
			PlayerTextDrawColor(playerid, PlayerText: _textDrawFader[playerid][idx][E_FADER_TEXTDRAW_ID], _textDrawFader[playerid][idx][E_FADER_FROM_COLOR]);
			
			_textDrawFader[playerid][idx][E_FADER_FROM_OUTLINE_COLOR] = ConvertColor(_textDrawFader[playerid][idx][E_FADER_FROM_OUTLINE_COLOR], _textDrawFader[playerid][idx][E_FADER_TO_OUTLINE_COLOR], update_rate);
			PlayerTextDrawBackgroundColor(playerid, PlayerText: _textDrawFader[playerid][idx][E_FADER_TEXTDRAW_ID], _textDrawFader[playerid][idx][E_FADER_FROM_OUTLINE_COLOR]);
		} else {
			PlayerTextDrawBoxColor(playerid, PlayerText: _textDrawFader[playerid][idx][E_FADER_TEXTDRAW_ID], _textDrawFader[playerid][idx][E_FADER_FROM_COLOR]);
		}

		PlayerTextDrawShow(playerid, PlayerText: _textDrawFader[playerid][idx][E_FADER_TEXTDRAW_ID]);

		if (_textDrawFader[playerid][idx][E_FADER_FROM_COLOR] == _textDrawFader[playerid][idx][E_FADER_TO_COLOR] || (_textDrawFader[playerid][idx][E_FADER_TYPE] == fade_type_text && _textDrawFader[playerid][idx][E_FADER_FROM_OUTLINE_COLOR] == _textDrawFader[playerid][idx][E_FADER_TO_OUTLINE_COLOR])) {
            if (_textDrawFader[playerid][idx][E_FADER_TYPE] == fade_type_box || (_textDrawFader[playerid][idx][E_FADER_TYPE] == fade_type_text && _textDrawFader[playerid][idx][E_FADER_FROM_OUTLINE_COLOR] == _textDrawFader[playerid][idx][E_FADER_TO_OUTLINE_COLOR])) {
				#if defined OnPlayerTextDrawFaded
		  			OnPlayerTextDrawFaded(playerid, PlayerText: _textDrawFader[playerid][idx][E_FADER_TEXTDRAW_ID], _textDrawFader[playerid][idx][E_FADER_TYPE], initial_color, _textDrawFader[playerid][idx][E_FADER_FROM_COLOR], initial_outline_color, _textDrawFader[playerid][idx][E_FADER_FROM_OUTLINE_COLOR]);
				#endif

				KillTimer(_textDrawFader[playerid][idx][E_FADER_TIMER]);
	  			_textDrawFader[playerid][idx][E_FADER_SLOT_USED] = false;
			}
		}
	}
}
