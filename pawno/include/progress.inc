/****************************************************************
* progress.inc - Player and Global progress bars with all 4 styles - By Gammix (a modified version of Toribo's and Southclaw's version, so thanks to them for their contribution!)
* v1.1 - Last updated: 22 Aug, 2018
****************************************************************/
#if defined progress_included
	#endinput
#endif
#define progress_included

#if !defined DEFAULT_BAR_INITIAL_VALUE
#define DEFAULT_BAR_INITIAL_VALUE 0.0
#endif

#if !defined DEFAULT_BAR_MAX_VALUE
#define DEFAULT_BAR_MAX_VALUE 100.0
#endif

#if !defined DEFAULT_BAR_BORDER_COLOR
#define DEFAULT_BAR_BORDER_COLOR 0x383838ff
#endif

#if !defined DEFAULT_BAR_BACKGROUND_COLOR
#define DEFAULT_BAR_BACKGROUND_COLOR 0x87b4ffff
#endif

#if !defined DEFAULT_BAR_FILLER_COLOR
#define DEFAULT_BAR_FILLER_COLOR 0x3f87fcff
#endif

enum {
	BAR_DIRECTION_RIGHT,
	BAR_DIRECTION_LEFT,
	BAR_DIRECTION_UP,
	BAR_DIRECTION_DOWN
};

enum E_PROGRESS_BAR_DATA {
	bool:BAR_VALID,
	BAR_FILLER_COLOR,
	BAR_DIRECTION,
	Float:BAR_X,
	Float:BAR_Y,
	Float:BAR_WIDTH,
	Float:BAR_HEIGHT,
	Float:BAR_VALUE,
	Float:BAR_MAX_VALUE
};

forward Float:GetBarFill(Float:x, Float:widthorheight, Float:max, Float:value, direction);
static Float:GetBarFill(Float:x, Float:widthorheight, Float:max, Float:value, direction) {
	switch(direction) {
		case BAR_DIRECTION_RIGHT:
			return ((x - 3.0) + (((((x - 2.0) + widthorheight) - x) / max) * value));

		case BAR_DIRECTION_LEFT:
			return ((x - 1.0) - (((((x + 2.0) - widthorheight) - x) / max) * -value)) - 4.0;

		case BAR_DIRECTION_UP:
			return -((((((widthorheight / 10.0) - 0.45) * 1.02) / max) * value) + 0.55);

		case BAR_DIRECTION_DOWN:
			return ((((((widthorheight / 10.0) - 0.45) * 1.02) / max) * value) - 0.55);
	}

	return 0.0;
}

/****************************************************************
*
* GLOBAL PROGRESS BARS
*
****************************************************************/

// definitions
#if !defined MAX_GLOBAL_BARS
#define MAX_GLOBAL_BARS (MAX_TEXT_DRAWS / 3)
#endif

#define INVALID_BAR_ID (Bar:-1)

// variables
static Text:globalBarBorderTxd[Bar:MAX_GLOBAL_BARS];
static Text:globalBarBackgroundTxd[Bar:MAX_GLOBAL_BARS];
static Text:globalBarFillerTxd[Bar:MAX_GLOBAL_BARS];

static globalBarData[Bar:MAX_GLOBAL_BARS][E_PROGRESS_BAR_DATA];

static bool:globalBarShown[MAX_PLAYERS][Bar:MAX_GLOBAL_BARS];

// internal functions
Text:CreateGlobalBarBorder(Float:x, Float:y, Float:width, Float:height, border_color, direction);
Text:CreateGlobalBarBackground(Float:x, Float:y, Float:width, Float:height, background_color, direction);
Text:CreateGlobalBarFiller(Float:x, Float:y, Float:width, Float:height, fill_color, Float:value, Float:max_value, direction);

// public functions (API)
#if defined _progress_natives_
native Bar:ProgressBarCreate(Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, direction = BAR_DIRECTION_RIGHT);
native IsValidProgressBar(Bar:barid);
native ProgressBarDestroy(Bar:barid);
native ProgressBarColor(Bar:barid, color);
native ProgressBarBackgroundColor(Bar:barid, color);
native ProgressBarBorderColor(Bar:barid, color);
native ProgressBarSetValue(Bar:barid, Float:value);
native ProgressBarSetMaxValue(Bar:barid, Float:max_value);
native ProgressBarShowForPlayer(playerid, Bar:barid);
native ProgressBarHideForPlayer(playerid, Bar:barid);
native ProgressBarShowForAll(Bar:barid);
native ProgressBarHideForAll(Bar:barid);
native Float:GetProgressBarValue(Bar:barid);
#endif

// implementation...
static stock Text:CreateGlobalBarBorder(Float:x, Float:y, Float:width, Float:height, border_color, direction) {
	new Text:ret = Text:INVALID_TEXT_DRAW;

    switch (direction) {
		case BAR_DIRECTION_RIGHT: {
		    ret = TextDrawCreate(x, y, "_");
			TextDrawUseBox(ret, 1);
			TextDrawTextSize(ret, (x + width - 4.0), 0.0);
			TextDrawLetterSize(ret, 1.0, (height / 10));
			TextDrawBoxColor(ret, border_color);
		}

		case BAR_DIRECTION_LEFT: {
		    ret = TextDrawCreate(x, y, "_");
			TextDrawUseBox(ret, 1);
			TextDrawTextSize(ret, (x - width - 4.0), 0.0);
			TextDrawLetterSize(ret, 1.0, (height / 10));
			TextDrawBoxColor(ret, border_color);
		}

		case BAR_DIRECTION_UP: {
		    ret = TextDrawCreate(x, y, "_");
			TextDrawUseBox(ret, 1);
			TextDrawTextSize(ret, (x - width - 4.0), 0.0);
			TextDrawLetterSize(ret, 1.0, (-((height / 10) * 1.02) - 0.35));
			TextDrawBoxColor(ret, border_color);
		}

		case BAR_DIRECTION_DOWN: {
		    ret = TextDrawCreate(x, y, "_");
			TextDrawUseBox(ret, 1);
			TextDrawTextSize(ret, (x - width - 4.0), 0.0);
			TextDrawLetterSize(ret, 1.0, (((height / 10)) - 0.35));
			TextDrawBoxColor(ret, border_color);
		}
	}

	return ret;
}


static stock Text:CreateGlobalBarBackground(Float:x, Float:y, Float:width, Float:height, background_color, direction) {
	new Text:ret = Text:INVALID_TEXT_DRAW;

    switch (direction) {
		case BAR_DIRECTION_RIGHT: {
		    ret = TextDrawCreate((x + 1.2), (y + 2.15), "_");
			TextDrawUseBox(ret, 1);
			TextDrawTextSize(ret, (x + width - 5.5), 0.0);
			TextDrawLetterSize(ret, 1.0, ((height / 10) - 0.35));
			TextDrawBoxColor(ret, background_color);
		}

		case BAR_DIRECTION_LEFT: {
		    ret = TextDrawCreate((x - 1.2), (y + 2.15), "_");
			TextDrawUseBox(ret, 1);
			TextDrawTextSize(ret, (x - width - 2.5), 0.0);
			TextDrawLetterSize(ret, 1.0, ((height / 10) - 0.35));
			TextDrawBoxColor(ret, background_color);
		}

		case BAR_DIRECTION_UP: {
		    ret = TextDrawCreate((x - 1.2), (y - 1.0), "_");
			TextDrawUseBox(ret, 1);
			TextDrawTextSize(ret, (x - width - 2.5), 0.0);
			TextDrawLetterSize(ret, 1.0, (-(height / 10.0) * 1.02));
			TextDrawBoxColor(ret, background_color);
		}

		case BAR_DIRECTION_DOWN: {
		    ret = TextDrawCreate((x - 1.2), (y + 1.0), "_");
			TextDrawUseBox(ret, 1);
			TextDrawTextSize(ret, (x - width - 2.5), 0.0);
			TextDrawLetterSize(ret, 1.0, ((height / 10.0) - 0.55));
			TextDrawBoxColor(ret, background_color);
		}
	}

	return ret;
}

static stock Text:CreateGlobalBarFiller(Float:x, Float:y, Float:width, Float:height, fill_color, Float:value, Float:max_value, direction) {
	new Text:ret = Text:INVALID_TEXT_DRAW;

	switch (direction) {
		case BAR_DIRECTION_RIGHT: {
		    ret = TextDrawCreate((x + 1.2), (y + 2.15), "_");
			TextDrawUseBox(ret, 1);
			TextDrawTextSize(ret, GetBarFill(x, width, max_value, value, direction), 0.0);
			TextDrawLetterSize(ret, 1.0, ((height / 10) - 0.35));
			TextDrawBoxColor(ret, fill_color);
		}

		case BAR_DIRECTION_LEFT: {
		    ret = TextDrawCreate((x - 1.2), (y + 2.15), "_");
			TextDrawUseBox(ret, 1);
			TextDrawTextSize(ret, GetBarFill(x, width, max_value, value, direction), 0.0);
			TextDrawLetterSize(ret, 1.0, ((height / 10) - 0.35));
			TextDrawBoxColor(ret, fill_color);
		}

		case BAR_DIRECTION_UP: {
		    ret = TextDrawCreate((x - 1.2), (y - 1.0), "_");
			TextDrawUseBox(ret, 1);
			TextDrawTextSize(ret, (x - width - 2.5), 0.0);
			TextDrawLetterSize(ret, 1.0, GetBarFill(x, height, max_value, value, direction));
			TextDrawBoxColor(ret, fill_color);
		}

		case BAR_DIRECTION_DOWN: {
		    ret = TextDrawCreate((x - 1.2), (y + 1.0), "_");
			TextDrawUseBox(ret, 1);
			TextDrawTextSize(ret, (x - width - 2.5), 0.0);
			TextDrawLetterSize(ret, 1.0, GetBarFill(x, height, max_value, value, direction));
			TextDrawBoxColor(ret, fill_color);
		}
	}

	return ret;
}

stock Bar:ProgressBarCreate(Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, direction = BAR_DIRECTION_RIGHT) {
	if (height == 0.0 || width == 0.0) {
		return INVALID_BAR_ID;
	}

	new Bar:index = INVALID_BAR_ID;
	for (new Bar:i = Bar:0; i < Bar:MAX_GLOBAL_BARS; i++) {
		if (globalBarData[i][BAR_VALID] == false) {
			index = i;
			break;
		}
	}

	if (index == INVALID_BAR_ID) {
		return INVALID_BAR_ID;
	}

	globalBarBorderTxd[index] = CreateGlobalBarBorder(x, y, width, height, DEFAULT_BAR_BORDER_COLOR, direction);
	if (globalBarBorderTxd[index] == Text:INVALID_TEXT_DRAW) {
		return INVALID_BAR_ID;
	}

	globalBarBackgroundTxd[index] = CreateGlobalBarBackground(x, y, width, height, DEFAULT_BAR_BACKGROUND_COLOR, direction);
	if (globalBarBackgroundTxd[index] == Text:INVALID_TEXT_DRAW) {
	    TextDrawDestroy(globalBarBorderTxd[index]);
	   	globalBarBorderTxd[index] = Text:INVALID_TEXT_DRAW;

		return INVALID_BAR_ID;
	}

	globalBarFillerTxd[index] = CreateGlobalBarFiller(x, y, width, height, DEFAULT_BAR_FILLER_COLOR, DEFAULT_BAR_INITIAL_VALUE, DEFAULT_BAR_MAX_VALUE, direction);
	if (globalBarFillerTxd[index] == Text:INVALID_TEXT_DRAW) {
	    TextDrawDestroy(globalBarBorderTxd[index]);
	   	globalBarBorderTxd[index] = Text:INVALID_TEXT_DRAW;

	    TextDrawDestroy(globalBarBackgroundTxd[index]);
	    globalBarBackgroundTxd[index] = Text:INVALID_TEXT_DRAW;

		return INVALID_BAR_ID;
	}

	globalBarData[index][BAR_VALID] = true;
	globalBarData[index][BAR_DIRECTION] = direction;
	globalBarData[index][BAR_FILLER_COLOR] = DEFAULT_BAR_FILLER_COLOR;
	globalBarData[index][BAR_X] = x;
	globalBarData[index][BAR_Y] = y;
	globalBarData[index][BAR_WIDTH] = width;
	globalBarData[index][BAR_HEIGHT] = height;
	globalBarData[index][BAR_VALUE] = DEFAULT_BAR_INITIAL_VALUE;
	globalBarData[index][BAR_MAX_VALUE] = DEFAULT_BAR_MAX_VALUE;

	for (new i = 0; i < MAX_PLAYERS; i++) {
		globalBarShown[i][index] = false;
	}

	return index;
}

stock IsValidProgressBar(Bar:barid) {
	if (barid < Bar:0 || barid >= Bar:MAX_GLOBAL_BARS) {
 		return 0;
	}

	return globalBarData[barid][BAR_VALID];
}

stock ProgressBarDestroy(Bar:barid) {
	if (!IsValidProgressBar(barid)) {
		return 0;
	}

	TextDrawDestroy(globalBarBorderTxd[barid]);
	globalBarBorderTxd[barid] = Text:INVALID_TEXT_DRAW;

	TextDrawDestroy(globalBarBackgroundTxd[barid]);
	globalBarBackgroundTxd[barid] = Text:INVALID_TEXT_DRAW;

	TextDrawDestroy(globalBarFillerTxd[barid]);
	globalBarFillerTxd[barid] = Text:INVALID_TEXT_DRAW;

	globalBarData[barid][BAR_VALID] = false;
	return 1;
}

stock ProgressBarColor(Bar:barid, color) {
	if (!IsValidProgressBar(barid)) {
		return 0;
	}

	globalBarData[barid][BAR_FILLER_COLOR] = color;

	if ((color & 0xFF) == 0) {
		if (globalBarFillerTxd[barid] == Text:INVALID_TEXT_DRAW) {
		    return 1;
		} else {
			TextDrawDestroy(globalBarFillerTxd[barid]);
			globalBarFillerTxd[barid] = Text:INVALID_TEXT_DRAW;
		}
	}
	else {
		if (globalBarFillerTxd[barid] == Text:INVALID_TEXT_DRAW) {
			globalBarFillerTxd[barid] = CreateGlobalBarFiller(globalBarData[barid][BAR_X], globalBarData[barid][BAR_Y], globalBarData[barid][BAR_WIDTH], globalBarData[barid][BAR_HEIGHT], color, globalBarData[barid][BAR_VALUE], globalBarData[barid][BAR_MAX_VALUE], globalBarData[barid][BAR_DIRECTION]);
            if (globalBarFillerTxd[barid] == Text:INVALID_TEXT_DRAW) {
                return 0;
			}
		}
		else {
			TextDrawBoxColor(globalBarFillerTxd[barid], color);
		}
	}

	return 1;
}

stock ProgressBarBackgroundColor(Bar:barid, color) {
	if (!IsValidProgressBar(barid)) {
		return 0;
	}

	if ((color & 0xFF) == 0) {
		if (globalBarBackgroundTxd[barid] == Text:INVALID_TEXT_DRAW) {
		    return 1;
		} else {
			TextDrawDestroy(globalBarBackgroundTxd[barid]);
			globalBarBackgroundTxd[barid] = Text:INVALID_TEXT_DRAW;
		}
	}
	else {
		if (globalBarBackgroundTxd[barid] == Text:INVALID_TEXT_DRAW) {
			globalBarBackgroundTxd[barid] = CreateGlobalBarBackground(globalBarData[barid][BAR_X], globalBarData[barid][BAR_Y], globalBarData[barid][BAR_WIDTH], globalBarData[barid][BAR_HEIGHT], color, globalBarData[barid][BAR_DIRECTION]);
            if (globalBarBackgroundTxd[barid] == Text:INVALID_TEXT_DRAW) {
                return 0;
			}
		}
		else {
			TextDrawBoxColor(globalBarBackgroundTxd[barid], color);
		}
	}

	return 1;
}

stock ProgressBarBorderColor(Bar:barid, color) {
	if (!IsValidProgressBar(barid)) {
		return 0;
	}

	if ((color & 0xFF) == 0) {
		if (globalBarBorderTxd[barid] == Text:INVALID_TEXT_DRAW) {
		    return 1;
		} else {
			TextDrawDestroy(globalBarBorderTxd[barid]);
			globalBarBorderTxd[barid] = Text:INVALID_TEXT_DRAW;
		}
	}
	else {
		if (globalBarBorderTxd[barid] == Text:INVALID_TEXT_DRAW) {
			globalBarBorderTxd[barid] = CreateGlobalBarBorder(globalBarData[barid][BAR_X], globalBarData[barid][BAR_Y], globalBarData[barid][BAR_WIDTH], globalBarData[barid][BAR_HEIGHT], color, globalBarData[barid][BAR_DIRECTION]);
            if (globalBarBorderTxd[barid] == Text:INVALID_TEXT_DRAW) {
                return 0;
			}
		}
		else {
			TextDrawBoxColor(globalBarBorderTxd[barid], color);
		}
	}

	return 1;
}

stock ProgressBarSetValue(Bar:barid, Float:value) {
	if (!IsValidProgressBar(barid)) {
		return 0;
	}

	if (globalBarData[barid][BAR_VALUE] == value) {
		return 1;
	}

	globalBarData[barid][BAR_VALUE] = (value > globalBarData[barid][BAR_MAX_VALUE]) ? globalBarData[barid][BAR_MAX_VALUE] : value;
	
	if (globalBarFillerTxd[barid] == Text:INVALID_TEXT_DRAW) {
		return 1;
	}

	TextDrawDestroy(globalBarFillerTxd[barid]);
	globalBarFillerTxd[barid] = CreateGlobalBarFiller(globalBarData[barid][BAR_X], globalBarData[barid][BAR_Y], globalBarData[barid][BAR_WIDTH], globalBarData[barid][BAR_HEIGHT], globalBarData[barid][BAR_FILLER_COLOR], globalBarData[barid][BAR_VALUE], globalBarData[barid][BAR_MAX_VALUE], globalBarData[barid][BAR_DIRECTION]);
	if (globalBarFillerTxd[barid] == Text:INVALID_TEXT_DRAW) {
 		return 0;
	}

	#if defined foreach
	foreach(new i : Player) {
	#else
	for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) if (IsPlayerConnected(i)) {
	#endif
		if (globalBarShown[i][barid] == true) {
			TextDrawShowForPlayer(i, globalBarFillerTxd[barid]);
		}
	}

	return 1;
}

stock ProgressBarSetMaxValue(Bar:barid, Float:max_value) {
	if (!IsValidProgressBar(barid)) {
		return 0;
	}

	if (globalBarData[barid][BAR_MAX_VALUE] == max_value) {
		return 1;
	}

	if (max_value > globalBarData[barid][BAR_MAX_VALUE]) {
		return 0;
	}

	globalBarData[barid][BAR_MAX_VALUE] = max_value;
 	if (max_value < globalBarData[barid][BAR_VALUE]) {
  		globalBarData[barid][BAR_VALUE] = max_value;
	}

	if (globalBarFillerTxd[barid] == Text:INVALID_TEXT_DRAW) {
		return 1;
	}

	TextDrawDestroy(globalBarFillerTxd[barid]);
	globalBarFillerTxd[barid] = CreateGlobalBarFiller(globalBarData[barid][BAR_X], globalBarData[barid][BAR_Y], globalBarData[barid][BAR_WIDTH], globalBarData[barid][BAR_HEIGHT], globalBarData[barid][BAR_FILLER_COLOR], globalBarData[barid][BAR_VALUE], globalBarData[barid][BAR_MAX_VALUE], globalBarData[barid][BAR_DIRECTION]);
	if (globalBarFillerTxd[barid] == Text:INVALID_TEXT_DRAW) {
 		return 0;
	}

	#if defined foreach
	foreach(new i : Player) {
	#else
	for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) if (IsPlayerConnected(i)) {
	#endif
		if (globalBarShown[i][barid] == true) {
			TextDrawShowForPlayer(i, globalBarFillerTxd[barid]);
		}
	}

	return 1;
}

stock ProgressBarShowForPlayer(playerid, Bar:barid) {
    if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
    }

	if (barid < Bar:0 || barid >= Bar:MAX_GLOBAL_BARS) {
		return 0;
	}

	TextDrawShowForPlayer(playerid, globalBarBorderTxd[barid]);
	TextDrawShowForPlayer(playerid, globalBarBackgroundTxd[barid]);
	TextDrawShowForPlayer(playerid, globalBarFillerTxd[barid]);

	globalBarShown[playerid][barid] = true;
	return 1;
}

stock ProgressBarHideForPlayer(playerid, Bar:barid) {
    if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
    }

	if (barid < Bar:0 || barid >= Bar:MAX_GLOBAL_BARS) {
		return 0;
	}

	TextDrawHideForPlayer(playerid, globalBarBorderTxd[barid]);
	TextDrawHideForPlayer(playerid, globalBarBackgroundTxd[barid]);
	TextDrawHideForPlayer(playerid, globalBarFillerTxd[barid]);

	globalBarShown[playerid][barid] = false;
	return 1;
}

stock ProgressBarShowForAll(Bar:barid) {
	if (barid < Bar:0 || barid >= Bar:MAX_GLOBAL_BARS) {
		return 0;
	}

	TextDrawShowForAll(globalBarBorderTxd[barid]);
	TextDrawShowForAll(globalBarBackgroundTxd[barid]);
	TextDrawShowForAll(globalBarFillerTxd[barid]);

	#if defined foreach
	foreach(new i : Player) {
	#else
	for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) if (IsPlayerConnected(i)) {
	#endif
		globalBarShown[i][barid] = true;
	}
	return 1;
}

stock ProgressBarHideForAll(Bar:barid) {
	if (barid < Bar:0 || barid >= Bar:MAX_GLOBAL_BARS) {
		return 0;
	}

	TextDrawHideForAll(globalBarBorderTxd[barid]);
	TextDrawHideForAll(globalBarBackgroundTxd[barid]);
	TextDrawHideForAll(globalBarFillerTxd[barid]);

	#if defined foreach
	foreach(new i : Player) {
	#else
	for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) if (IsPlayerConnected(i)) {
	#endif
		globalBarShown[i][barid] = false;
	}
	return 1;
}

stock Float:GetProgressBarValue(Bar:barid) {
	if (barid < Bar:0 || barid >= Bar:MAX_GLOBAL_BARS) {
 		return 0.0;
	}

	return globalBarData[barid][BAR_VALUE];
}

/****************************************************************
*
* PLAYER PROGRESS BARS
*
****************************************************************/

// definitions
#if !defined MAX_PLAYER_BARS
#define MAX_PLAYER_BARS (MAX_PLAYER_TEXT_DRAWS / 3)
#endif

#define INVALID_PLAYER_BAR_ID (PlayerBar:-1)

// variables
static PlayerText:playerBarBorderTxd[MAX_PLAYERS][PlayerBar:MAX_PLAYER_BARS];
static PlayerText:playerBarBackgroundTxd[MAX_PLAYERS][PlayerBar:MAX_PLAYER_BARS];
static PlayerText:playerBarFillerTxd[MAX_PLAYERS][PlayerBar:MAX_PLAYER_BARS];

static playerBarData[MAX_PLAYERS][PlayerBar:MAX_PLAYER_BARS][E_PROGRESS_BAR_DATA];

static bool:playerBarShown[MAX_PLAYERS][PlayerBar:MAX_PLAYER_BARS];

// internal functions
PlayerText:CreatePlayerBarBorder(playerid, Float:x, Float:y, Float:width, Float:height, border_color, direction);
PlayerText:CreatePlayerBarFiller(playerid, Float:x, Float:y, Float:width, Float:height, fill_color, Float:value, Float:max_value, direction);
PlayerText:CreatePlayerBarBackground(playerid, Float:x, Float:y, Float:width, Float:height, background_color, direction);

// public functions (API)
#if defined _progress_natives_
native PlayerBar:PlayerProgressBarCreate(playerid, Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, direction = BAR_DIRECTION_RIGHT);
native IsValidPlayerProgressBar(playerid, PlayerBar:player_barid);
native PlayerProgressBarDestroy(playerid, PlayerBar:player_barid);
native PlayerProgressBarColor(playerid, PlayerBar:player_barid, color);
native PlayerProgressBarBGColor(playerid, PlayerBar:player_barid, color);
native PlayerProgressBarBorderColor(playerid, PlayerBar:player_barid, color);
native PlayerProgressBarSetValue(playerid, PlayerBar:player_barid, Float:value);
native PlayerProgressBarSetMaxValue(playerid, PlayerBar:player_barid, Float:max_value);
native PlayerProgressBarShow(playerid, PlayerBar:player_barid);
native PlayerProgressBarHide(playerid, PlayerBar:player_barid);
native Float:GetPlayerProgressBarValue(playerid, PlayerBar:player_barid);
#endif

// implementation...
static stock PlayerText:CreatePlayerBarBorder(playerid, Float:x, Float:y, Float:width, Float:height, border_color, direction) {
	new PlayerText:ret = PlayerText:INVALID_TEXT_DRAW;

    switch (direction) {
		case BAR_DIRECTION_RIGHT: {
		    ret = CreatePlayerTextDraw(playerid, x, y, "_");
			PlayerTextDrawUseBox(playerid, ret, 1);
			PlayerTextDrawTextSize(playerid, ret, (x + width - 4.0), 0.0);
			PlayerTextDrawLetterSize(playerid, ret, 1.0, (height / 10));
			PlayerTextDrawBoxColor(playerid, ret, border_color);
		}

		case BAR_DIRECTION_LEFT: {
		    ret = CreatePlayerTextDraw(playerid, x, y, "_");
			PlayerTextDrawUseBox(playerid, ret, 1);
			PlayerTextDrawTextSize(playerid, ret, (x - width - 4.0), 0.0);
			PlayerTextDrawLetterSize(playerid, ret, 1.0, (height / 10));
			PlayerTextDrawBoxColor(playerid, ret, border_color);
		}

		case BAR_DIRECTION_UP: {
		    ret = CreatePlayerTextDraw(playerid, x, y, "_");
			PlayerTextDrawUseBox(playerid, ret, 1);
			PlayerTextDrawTextSize(playerid, ret, (x - width - 4.0), 0.0);
			PlayerTextDrawLetterSize(playerid, ret, 1.0, (-((height / 10) * 1.02) - 0.35));
			PlayerTextDrawBoxColor(playerid, ret, border_color);
		}

		case BAR_DIRECTION_DOWN: {
		    ret = CreatePlayerTextDraw(playerid, x, y, "_");
			PlayerTextDrawUseBox(playerid, ret, 1);
			PlayerTextDrawTextSize(playerid, ret, (x - width - 4.0), 0.0);
			PlayerTextDrawLetterSize(playerid, ret, 1.0, (((height / 10)) - 0.35));
			PlayerTextDrawBoxColor(playerid, ret, border_color);
		}
	}

	return ret;
}

static stock PlayerText:CreatePlayerBarBackground(playerid, Float:x, Float:y, Float:width, Float:height, background_color, direction) {
	new PlayerText:ret = PlayerText:INVALID_TEXT_DRAW;

    switch (direction) {
		case BAR_DIRECTION_RIGHT: {
		    ret = CreatePlayerTextDraw(playerid, (x + 1.2), (y + 2.15), "_");
			PlayerTextDrawUseBox(playerid, ret, 1);
			PlayerTextDrawTextSize(playerid, ret, (x + width - 5.5), 0.0);
			PlayerTextDrawLetterSize(playerid, ret, 1.0, ((height / 10) - 0.35));
			PlayerTextDrawBoxColor(playerid, ret, background_color);
		}

		case BAR_DIRECTION_LEFT: {
		    ret = CreatePlayerTextDraw(playerid, (x - 1.2), (y + 2.15), "_");
			PlayerTextDrawUseBox(playerid, ret, 1);
			PlayerTextDrawTextSize(playerid, ret, (x - width - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, ret, 1.0, ((height / 10) - 0.35));
			PlayerTextDrawBoxColor(playerid, ret, background_color);
		}

		case BAR_DIRECTION_UP: {
		    ret = CreatePlayerTextDraw(playerid, (x - 1.2), (y - 1.0), "_");
			PlayerTextDrawUseBox(playerid, ret, 1);
			PlayerTextDrawTextSize(playerid, ret, (x - width - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, ret, 1.0, (-(height / 10.0) * 1.02));
			PlayerTextDrawBoxColor(playerid, ret, background_color);
		}

		case BAR_DIRECTION_DOWN: {
		    ret = CreatePlayerTextDraw(playerid, (x - 1.2), (y + 1.0), "_");
			PlayerTextDrawUseBox(playerid, ret, 1);
			PlayerTextDrawTextSize(playerid, ret, (x - width - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, ret, 1.0, ((height / 10.0) - 0.55));
			PlayerTextDrawBoxColor(playerid, ret, background_color);
		}
	}

	return ret;
}

static stock PlayerText:CreatePlayerBarFiller(playerid, Float:x, Float:y, Float:width, Float:height, fill_color, Float:value, Float:max_value, direction) {
	new PlayerText:ret = PlayerText:INVALID_TEXT_DRAW;

	switch (direction) {
		case BAR_DIRECTION_RIGHT: {
		    ret = CreatePlayerTextDraw(playerid, (x + 1.2), (y + 2.15), "_");
			PlayerTextDrawUseBox(playerid, ret, 1);
			PlayerTextDrawTextSize(playerid, ret, GetBarFill(x, width, max_value, value, direction), 0.0);
			PlayerTextDrawLetterSize(playerid, ret, 1.0, ((height / 10) - 0.35));
			PlayerTextDrawBoxColor(playerid, ret, fill_color);
		}

		case BAR_DIRECTION_LEFT: {
		    ret = CreatePlayerTextDraw(playerid, (x - 1.2), (y + 2.15), "_");
			PlayerTextDrawUseBox(playerid, ret, 1);
			PlayerTextDrawTextSize(playerid, ret, GetBarFill(x, width, max_value, value, direction), 0.0);
			PlayerTextDrawLetterSize(playerid, ret, 1.0, ((height / 10) - 0.35));
			PlayerTextDrawBoxColor(playerid, ret, fill_color);
		}

		case BAR_DIRECTION_UP: {
		    ret = CreatePlayerTextDraw(playerid, (x - 1.2), (y - 1.0), "_");
			PlayerTextDrawUseBox(playerid, ret, 1);
			PlayerTextDrawTextSize(playerid, ret, (x - width - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, ret, 1.0, GetBarFill(x, height, max_value, value, direction));
			PlayerTextDrawBoxColor(playerid, ret, fill_color);
		}

		case BAR_DIRECTION_DOWN: {
		    ret = CreatePlayerTextDraw(playerid, (x - 1.2), (y + 1.0), "_");
			PlayerTextDrawUseBox(playerid, ret, 1);
			PlayerTextDrawTextSize(playerid, ret, (x - width - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, ret, 1.0, GetBarFill(x, height, max_value, value, direction));
			PlayerTextDrawBoxColor(playerid, ret, fill_color);
		}
	}

	return ret;
}

stock PlayerBar:PlayerProgressBarCreate(playerid, Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, direction = BAR_DIRECTION_RIGHT) {
	if (height == 0.0 || width == 0.0) {
		return INVALID_PLAYER_BAR_ID;
	}

	new PlayerBar:index = INVALID_PLAYER_BAR_ID;
	for (new PlayerBar:i = PlayerBar:0; i < PlayerBar:MAX_PLAYER_BARS; i++) {
		if (playerBarData[playerid][i][BAR_VALID] == false) {
			index = i;
			break;
		}
	}

	if (index == INVALID_PLAYER_BAR_ID) {
		return INVALID_PLAYER_BAR_ID;
	}

	playerBarBorderTxd[playerid][index] = CreatePlayerBarBorder(playerid, x, y, width, height, DEFAULT_BAR_BORDER_COLOR, direction);
	if (playerBarBorderTxd[playerid][index] == PlayerText:INVALID_TEXT_DRAW) {
		return INVALID_PLAYER_BAR_ID;
	}

	playerBarBackgroundTxd[playerid][index] = CreatePlayerBarBackground(playerid, x, y, width, height, DEFAULT_BAR_BACKGROUND_COLOR, direction);
	if (playerBarBackgroundTxd[playerid][index] == PlayerText:INVALID_TEXT_DRAW) {
	    PlayerTextDrawDestroy(playerid, playerBarBorderTxd[playerid][index]);
	   	playerBarBorderTxd[playerid][index] = PlayerText:INVALID_TEXT_DRAW;

		return INVALID_PLAYER_BAR_ID;
	}

	playerBarFillerTxd[playerid][index] = CreatePlayerBarFiller(playerid, x, y, width, height, DEFAULT_BAR_FILLER_COLOR, DEFAULT_BAR_INITIAL_VALUE, DEFAULT_BAR_MAX_VALUE, direction);
	if (playerBarFillerTxd[playerid][index] == PlayerText:INVALID_TEXT_DRAW) {
	    PlayerTextDrawDestroy(playerid, playerBarBorderTxd[playerid][index]);
	   	playerBarBorderTxd[playerid][index] = PlayerText:INVALID_TEXT_DRAW;

	    PlayerTextDrawDestroy(playerid, playerBarBackgroundTxd[playerid][index]);
	    playerBarBackgroundTxd[playerid][index] = PlayerText:INVALID_TEXT_DRAW;

		return INVALID_PLAYER_BAR_ID;
	}

	playerBarData[playerid][index][BAR_VALID] = true;
	playerBarData[playerid][index][BAR_DIRECTION] = direction;
	playerBarData[playerid][index][BAR_FILLER_COLOR] = DEFAULT_BAR_FILLER_COLOR;
	playerBarData[playerid][index][BAR_X] = x;
	playerBarData[playerid][index][BAR_Y] = y;
	playerBarData[playerid][index][BAR_WIDTH] = width;
	playerBarData[playerid][index][BAR_HEIGHT] = height;
	playerBarData[playerid][index][BAR_VALUE] = DEFAULT_BAR_INITIAL_VALUE;
	playerBarData[playerid][index][BAR_MAX_VALUE] = DEFAULT_BAR_MAX_VALUE;

	for (new i = 0; i < MAX_PLAYERS; i++) {
		playerBarShown[i][index] = false;
	}

	return index;
}

stock IsValidPlayerProgressBar(playerid, PlayerBar:player_barid) {
	if (player_barid < PlayerBar:0 || player_barid >= PlayerBar:MAX_PLAYER_BARS) {
 		return 0;
	}

	return playerBarData[playerid][player_barid][BAR_VALID];
}

stock PlayerProgressBarDestroy(playerid, PlayerBar:player_barid) {
	if (!IsValidPlayerProgressBar(playerid, player_barid)) {
		return 0;
	}

	PlayerTextDrawDestroy(playerid, playerBarBorderTxd[playerid][player_barid]);
	playerBarBorderTxd[playerid][player_barid] = PlayerText:INVALID_TEXT_DRAW;

	PlayerTextDrawDestroy(playerid, playerBarBackgroundTxd[playerid][player_barid]);
	playerBarBackgroundTxd[playerid][player_barid] = PlayerText:INVALID_TEXT_DRAW;

	PlayerTextDrawDestroy(playerid, playerBarFillerTxd[playerid][player_barid]);
	playerBarFillerTxd[playerid][player_barid] = PlayerText:INVALID_TEXT_DRAW;

	playerBarData[playerid][player_barid][BAR_VALID] = false;
	return 1;
}

stock PlayerProgressBarColor(playerid, PlayerBar:player_barid, color) {
	if (!IsValidPlayerProgressBar(playerid, player_barid)) {
		return 0;
	}

	playerBarData[playerid][player_barid][BAR_FILLER_COLOR] = color;

	if ((color & 0xFF) == 0) {
		if (playerBarFillerTxd[playerid][player_barid] == PlayerText:INVALID_TEXT_DRAW) {
		    return 1;
		} else {
			PlayerTextDrawDestroy(playerid, playerBarFillerTxd[playerid][player_barid]);
			playerBarFillerTxd[playerid][player_barid] = PlayerText:INVALID_TEXT_DRAW;
		}
	}
	else {
		if (playerBarFillerTxd[playerid][player_barid] == PlayerText:INVALID_TEXT_DRAW) {
			playerBarFillerTxd[playerid][player_barid] = CreatePlayerBarFiller(playerid, playerBarData[playerid][player_barid][BAR_X], playerBarData[playerid][player_barid][BAR_Y], playerBarData[playerid][player_barid][BAR_WIDTH], playerBarData[playerid][player_barid][BAR_HEIGHT], color, playerBarData[playerid][player_barid][BAR_VALUE], playerBarData[playerid][player_barid][BAR_MAX_VALUE], playerBarData[playerid][player_barid][BAR_DIRECTION]);
            if (playerBarFillerTxd[playerid][player_barid] == PlayerText:INVALID_TEXT_DRAW) {
                return 0;
			}
		}
		else {
			PlayerTextDrawBoxColor(playerid, playerBarFillerTxd[playerid][player_barid], color);
		}
	}

	return 1;
}

stock PlayerProgressBarBGColor(playerid, PlayerBar:player_barid, color) {
	if (!IsValidPlayerProgressBar(playerid, player_barid)) {
		return 0;
	}

	if ((color & 0xFF) == 0) {
		if (playerBarBackgroundTxd[playerid][player_barid] == PlayerText:INVALID_TEXT_DRAW) {
		    return 1;
		} else {
			PlayerTextDrawDestroy(playerid, playerBarBackgroundTxd[playerid][player_barid]);
			playerBarBackgroundTxd[playerid][player_barid] = PlayerText:INVALID_TEXT_DRAW;
		}
	}
	else {
		if (playerBarBackgroundTxd[playerid][player_barid] == PlayerText:INVALID_TEXT_DRAW) {
			playerBarBackgroundTxd[playerid][player_barid] = CreatePlayerBarBackground(playerid, playerBarData[playerid][player_barid][BAR_X], playerBarData[playerid][player_barid][BAR_Y], playerBarData[playerid][player_barid][BAR_WIDTH], playerBarData[playerid][player_barid][BAR_HEIGHT], color, playerBarData[playerid][player_barid][BAR_DIRECTION]);
            if (playerBarBackgroundTxd[playerid][player_barid] == PlayerText:INVALID_TEXT_DRAW) {
                return 0;
			}
		}
		else {
			PlayerTextDrawBoxColor(playerid, playerBarBackgroundTxd[playerid][player_barid], color);
		}
	}

	return 1;
}

stock PlayerProgressBarBorderColor(playerid, PlayerBar:player_barid, color) {
	if (!IsValidPlayerProgressBar(playerid, player_barid)) {
		return 0;
	}

	if ((color & 0xFF) == 0) {
		if (playerBarBorderTxd[playerid][player_barid] == PlayerText:INVALID_TEXT_DRAW) {
		    return 1;
		} else {
			PlayerTextDrawDestroy(playerid, playerBarBorderTxd[playerid][player_barid]);
			playerBarBorderTxd[playerid][player_barid] = PlayerText:INVALID_TEXT_DRAW;
		}
	}
	else {
		if (playerBarBorderTxd[playerid][player_barid] == PlayerText:INVALID_TEXT_DRAW) {
			playerBarBorderTxd[playerid][player_barid] = CreatePlayerBarBorder(playerid, playerBarData[playerid][player_barid][BAR_X], playerBarData[playerid][player_barid][BAR_Y], playerBarData[playerid][player_barid][BAR_WIDTH], playerBarData[playerid][player_barid][BAR_HEIGHT], color, playerBarData[playerid][player_barid][BAR_DIRECTION]);
            if (playerBarBorderTxd[playerid][player_barid] == PlayerText:INVALID_TEXT_DRAW) {
                return 0;
			}
		}
		else {
			PlayerTextDrawBoxColor(playerid, playerBarBorderTxd[playerid][player_barid], color);
		}
	}

	return 1;
}

stock PlayerProgressBarSetValue(playerid, PlayerBar:player_barid, Float:value) {
	if (!IsValidPlayerProgressBar(playerid, player_barid)) {
		return 0;
	}

	if (playerBarData[playerid][player_barid][BAR_VALUE] == value) {
		return 1;
	}

	playerBarData[playerid][player_barid][BAR_VALUE] = (value > playerBarData[playerid][player_barid][BAR_MAX_VALUE]) ? playerBarData[playerid][player_barid][BAR_MAX_VALUE] : value;

	if (playerBarFillerTxd[playerid][player_barid] == PlayerText:INVALID_TEXT_DRAW) {
		return 1;
	}

	PlayerTextDrawDestroy(playerid, playerBarFillerTxd[playerid][player_barid]);
	playerBarFillerTxd[playerid][player_barid] = CreatePlayerBarFiller(playerid, playerBarData[playerid][player_barid][BAR_X], playerBarData[playerid][player_barid][BAR_Y], playerBarData[playerid][player_barid][BAR_WIDTH], playerBarData[playerid][player_barid][BAR_HEIGHT], playerBarData[playerid][player_barid][BAR_FILLER_COLOR], playerBarData[playerid][player_barid][BAR_VALUE], playerBarData[playerid][player_barid][BAR_MAX_VALUE], playerBarData[playerid][player_barid][BAR_DIRECTION]);
	if (playerBarFillerTxd[playerid][player_barid] == PlayerText:INVALID_TEXT_DRAW) {
 		return 0;
	}

	if (playerBarShown[playerid][player_barid] == true) {
		PlayerTextDrawShow(playerid, playerBarFillerTxd[playerid][player_barid]);
	}

	return 1;
}

stock PlayerProgressBarSetMaxValue(playerid, PlayerBar:player_barid, Float:max_value) {
	if (!IsValidPlayerProgressBar(playerid, player_barid)) {
		return 0;
	}

	if (playerBarData[playerid][player_barid][BAR_MAX_VALUE] == max_value) {
		return 1;
	}

	playerBarData[playerid][player_barid][BAR_MAX_VALUE] = max_value;
 	if (max_value < playerBarData[playerid][player_barid][BAR_VALUE]) {
  		playerBarData[playerid][player_barid][BAR_VALUE] = max_value;
	}

	if (playerBarFillerTxd[playerid][player_barid] == PlayerText:INVALID_TEXT_DRAW) {
		return 1;
	}

	PlayerTextDrawDestroy(playerid, playerBarFillerTxd[playerid][player_barid]);
	playerBarFillerTxd[playerid][player_barid] = CreatePlayerBarFiller(playerid, playerBarData[playerid][player_barid][BAR_X], playerBarData[playerid][player_barid][BAR_Y], playerBarData[playerid][player_barid][BAR_WIDTH], playerBarData[playerid][player_barid][BAR_HEIGHT], playerBarData[playerid][player_barid][BAR_FILLER_COLOR], playerBarData[playerid][player_barid][BAR_VALUE], playerBarData[playerid][player_barid][BAR_MAX_VALUE], playerBarData[playerid][player_barid][BAR_DIRECTION]);
	if (playerBarFillerTxd[playerid][player_barid] == PlayerText:INVALID_TEXT_DRAW) {
 		return 0;
	}

	if (playerBarShown[playerid][player_barid] == true) {
		PlayerTextDrawShow(playerid, playerBarFillerTxd[playerid][player_barid]);
	}

	return 1;
}

stock PlayerProgressBarShow(playerid, PlayerBar:player_barid) {
    if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
    }

	if (player_barid < PlayerBar:0 || player_barid >= PlayerBar:MAX_PLAYER_BARS) {
 		return 0;
	}

	PlayerTextDrawShow(playerid, playerBarBorderTxd[playerid][player_barid]);
	PlayerTextDrawShow(playerid, playerBarBackgroundTxd[playerid][player_barid]);
	PlayerTextDrawShow(playerid, playerBarFillerTxd[playerid][player_barid]);

	playerBarShown[playerid][player_barid] = true;
	return 1;
}

stock PlayerProgressBarHide(playerid, PlayerBar:player_barid) {
    if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
    }

	if (player_barid < PlayerBar:0 || player_barid >= PlayerBar:MAX_PLAYER_BARS) {
 		return 0;
	}

	PlayerTextDrawHide(playerid, playerBarBorderTxd[playerid][player_barid]);
	PlayerTextDrawHide(playerid, playerBarBackgroundTxd[playerid][player_barid]);
	PlayerTextDrawHide(playerid, playerBarFillerTxd[playerid][player_barid]);

	playerBarShown[playerid][player_barid] = false;
	return 1;
}

stock Float:GetPlayerProgressBarValue(playerid, PlayerBar:player_barid) {
    if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0.0;
    }

	if (player_barid < PlayerBar:0 || player_barid >= PlayerBar:MAX_PLAYER_BARS) {
 		return 0.0;
	}

	return playerBarData[playerid][player_barid][BAR_VALUE];
}
