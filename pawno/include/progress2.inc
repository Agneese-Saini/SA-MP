#if defined _progress2_included
	#endinput
#endif
#define _progress2_included

#define MAX_GLOBAL_BARS (MAX_TEXT_DRAWS / 3)
#define MAX_PLAYER_BARS (MAX_PLAYER_TEXT_DRAWS / 3)

#define INVALID_BAR_ID (Bar:-1)

#define DEFAULT_BAR_INITIAL_VALUE 0.0
#define DEFAULT_BAR_MAX_VALUE 100.0

#define DEFAULT_BAR_BORDER_COLOR 0x383838ff
#define DEFAULT_BAR_BACKGROUND_COLOR 0x87b4ffff
#define DEFAULT_BAR_FILLER_COLOR 0x3f87fcff

enum {
	BAR_DIRECTION_RIGHT,
	BAR_DIRECTION_LEFT,
	BAR_DIRECTION_UP,
	BAR_DIRECTION_DOWN
};

enum E_PROGRESS_BAR_DATA {
	bool:BAR_VALID,
	BAR_TEXTDRAW_BORDER,
	BAR_TEXTDRAW_BACKGROUND,
	BAR_TEXTDRAW_FILLER,
	BAR_FILLER_COLOR,
	BAR_DIRECTION,
	Float:BAR_X,
	Float:BAR_Y,
	Float:BAR_WIDTH,
	Float:BAR_HEIGHT,
	Float:BAR_VALUE,
	Float:BAR_MAX_VALUE
};

static globalProgressBarData[Bar:MAX_GLOBAL_BARS][E_PROGRESS_BAR_DATA];
static bool:globalProgressBarShown[MAX_PLAYERS][Bar:MAX_GLOBAL_BARS];

static playerProgressBarData[MAX_PLAYERS][Bar:MAX_PLAYER_BARS][E_PROGRESS_BAR_DATA];
static bool:playerProgressBarShown[MAX_PLAYERS][Bar:MAX_PLAYER_BARS];

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

static CreateBarBorderTextDraw(Float:x, Float:y, Float:width, Float:height, border_color, direction, playerid) {
	new border;

    switch (direction) {
		case BAR_DIRECTION_RIGHT: {
		    if (playerid == -1) {
				border = _:TextDrawCreate(x, y, "_");
				TextDrawUseBox(Text:border, 1);
				TextDrawTextSize(Text:border, (x + width - 4.0), 0.0);
				TextDrawLetterSize(Text:border, 1.0, (height / 10));
				TextDrawBoxColor(Text:border, border_color);
			}
			else  {
				border = _:CreatePlayerTextDraw(playerid, x, y, "_");
				PlayerTextDrawUseBox(playerid, PlayerText:border, 1);
				PlayerTextDrawTextSize(playerid, PlayerText:border, (x + width - 4.0), 0.0);
				PlayerTextDrawLetterSize(playerid, PlayerText:border, 1.0, (height / 10));
				PlayerTextDrawBoxColor(playerid, PlayerText:border, border_color);
			}
		}

		case BAR_DIRECTION_LEFT: {
		    if (playerid == -1) {
				border = _:TextDrawCreate(x, y, "_");
				TextDrawUseBox(Text:border, 1);
				TextDrawTextSize(Text:border, (x - width - 4.0), 0.0);
				TextDrawLetterSize(Text:border, 1.0, (height / 10));
				TextDrawBoxColor(Text:border, border_color);
			}
			else  {
				border = _:CreatePlayerTextDraw(playerid, x, y, "_");
				PlayerTextDrawUseBox(playerid, PlayerText:border, 1);
				PlayerTextDrawTextSize(playerid, PlayerText:border, (x - width - 4.0), 0.0);
				PlayerTextDrawLetterSize(playerid, PlayerText:border, 1.0, (height / 10));
				PlayerTextDrawBoxColor(playerid, PlayerText:border, border_color);
			}
		}

		case BAR_DIRECTION_UP: {
		    if (playerid == -1) {
				border = _:TextDrawCreate(x, y, "_");
				TextDrawUseBox(Text:border, 1);
				TextDrawTextSize(Text:border, (x - width - 4.0), 0.0);
				TextDrawLetterSize(Text:border, 1.0, (-((height / 10) * 1.02) - 0.35));
				TextDrawBoxColor(Text:border, border_color);
			}
			else  {
				border = _:CreatePlayerTextDraw(playerid, x, y, "_");
				PlayerTextDrawUseBox(playerid, PlayerText:border, 1);
				PlayerTextDrawTextSize(playerid, PlayerText:border, (x - width - 4.0), 0.0);
				PlayerTextDrawLetterSize(playerid, PlayerText:border, 1.0, (-((height / 10) * 1.02) - 0.35));
				PlayerTextDrawBoxColor(playerid, PlayerText:border, border_color);
			}
		}

		case BAR_DIRECTION_DOWN: {
		    if (playerid == -1) {
				border = _:TextDrawCreate(x, y, "_");
				TextDrawUseBox(Text:border, 1);
				TextDrawTextSize(Text:border, (x - width - 4.0), 0.0);
				TextDrawLetterSize(Text:border, 1.0, (((height / 10)) - 0.35));
				TextDrawBoxColor(Text:border, border_color);
			}
			else  {
				border = _:CreatePlayerTextDraw(playerid, x, y, "_");
				PlayerTextDrawUseBox(playerid, PlayerText:border, 1);
				PlayerTextDrawTextSize(playerid, PlayerText:border, (x - width - 4.0), 0.0);
				PlayerTextDrawLetterSize(playerid, PlayerText:border, 1.0, (((height / 10)) - 0.35));
				PlayerTextDrawBoxColor(playerid, PlayerText:border, border_color);
			}
		}
	}

	return border;
}


static CreateBarBackgroundTextDraw(Float:x, Float:y, Float:width, Float:height, background_color, direction, playerid) {
	new background;

    switch (direction) {
		case BAR_DIRECTION_RIGHT: {
		    if (playerid == -1) {
				background = _:TextDrawCreate((x + 1.2), (y + 2.15), "_");
				TextDrawUseBox(Text:background, 1);
				TextDrawTextSize(Text:background, (x + width - 5.5), 0.0);
				TextDrawLetterSize(Text:background, 1.0, ((height / 10) - 0.35));
				TextDrawBoxColor(Text:background, background_color);
			}
			else  {
				background = _:CreatePlayerTextDraw(playerid, (x + 1.2), (y + 2.15), "_");
				PlayerTextDrawUseBox(playerid, PlayerText:background, 1);
				PlayerTextDrawTextSize(playerid, PlayerText:background, (x + width - 5.5), 0.0);
				PlayerTextDrawLetterSize(playerid, PlayerText:background, 1.0, ((height / 10) - 0.35));
				PlayerTextDrawBoxColor(playerid, PlayerText:background, background_color);
			}
		}

		case BAR_DIRECTION_LEFT: {
		    if (playerid == -1) {
				background = _:TextDrawCreate((x - 1.2), (y + 2.15), "_");
				TextDrawUseBox(Text:background, 1);
				TextDrawTextSize(Text:background, (x - width - 2.5), 0.0);
				TextDrawLetterSize(Text:background, 1.0, ((height / 10) - 0.35));
				TextDrawBoxColor(Text:background, background_color);
			}
			else  {
				background = _:CreatePlayerTextDraw(playerid, (x - 1.2), (y + 2.15), "_");
				PlayerTextDrawUseBox(playerid, PlayerText:background, 1);
				PlayerTextDrawTextSize(playerid, PlayerText:background, (x - width - 2.5), 0.0);
				PlayerTextDrawLetterSize(playerid, PlayerText:background, 1.0, ((height / 10) - 0.35));
				PlayerTextDrawBoxColor(playerid, PlayerText:background, background_color);
			}
		}

		case BAR_DIRECTION_UP: {
		    if (playerid == -1) {
				background = _:TextDrawCreate((x - 1.2), (y - 1.0), "_");
				TextDrawUseBox(Text:background, 1);
				TextDrawTextSize(Text:background, (x - width - 2.5), 0.0);
				TextDrawLetterSize(Text:background, 1.0, (-(height / 10.0) * 1.02));
				TextDrawBoxColor(Text:background, background_color);
			}
			else  {
				background = _:CreatePlayerTextDraw(playerid, (x - 1.2), (y - 1.0), "_");
				PlayerTextDrawUseBox(playerid, PlayerText:background, 1);
				PlayerTextDrawTextSize(playerid, PlayerText:background, (x - width - 2.5), 0.0);
				PlayerTextDrawLetterSize(playerid, PlayerText:background, 1.0, (-(height / 10.0) * 1.02));
				PlayerTextDrawBoxColor(playerid, PlayerText:background, background_color);
			}
		}

		case BAR_DIRECTION_DOWN: {
		    if (playerid == -1) {
				background = _:TextDrawCreate((x - 1.2), (y + 1.0), "_");
				TextDrawUseBox(Text:background, 1);
				TextDrawTextSize(Text:background, (x - width - 2.5), 0.0);
				TextDrawLetterSize(Text:background, 1.0, ((height / 10.0) - 0.55));
				TextDrawBoxColor(Text:background, background_color);
			}
			else  {
				background = _:CreatePlayerTextDraw(playerid, (x - 1.2), (y + 1.0), "_");
				PlayerTextDrawUseBox(playerid, PlayerText:background, 1);
				PlayerTextDrawTextSize(playerid, PlayerText:background, (x - width - 2.5), 0.0);
				PlayerTextDrawLetterSize(playerid, PlayerText:background, 1.0, ((height / 10.0) - 0.55));
				PlayerTextDrawBoxColor(playerid, PlayerText:background, background_color);
			}
		}
	}

	return background;
}

static CreateBarFillerTextDraw(Float:x, Float:y, Float:width, Float:height, fill_color, Float:value, Float:max_value, direction, playerid) {
	new filler;

	switch (direction) {
		case BAR_DIRECTION_RIGHT: {
		    if (playerid == -1) {
				filler = _:TextDrawCreate((x + 1.2), (y + 2.15), "_");
				TextDrawUseBox(Text:filler, 1);
				TextDrawTextSize(Text:filler, GetBarFill(x, width, max_value, value, direction), 0.0);
				TextDrawLetterSize(Text:filler, 1.0, ((height / 10) - 0.35));
				TextDrawBoxColor(Text:filler, fill_color);
			}
			else  {
				filler = _:CreatePlayerTextDraw(playerid, (x + 1.2), (y + 2.15), "_");
				PlayerTextDrawUseBox(playerid, PlayerText:filler, 1);
				PlayerTextDrawTextSize(playerid, PlayerText:filler, GetBarFill(x, width, max_value, value, direction), 0.0);
				PlayerTextDrawLetterSize(playerid, PlayerText:filler, 1.0, ((height / 10) - 0.35));
				PlayerTextDrawBoxColor(playerid, PlayerText:filler, fill_color);
			}
		}

		case BAR_DIRECTION_LEFT: {
		    if (playerid == -1) {
				filler = _:TextDrawCreate((x - 1.2), (y + 2.15), "_");
				TextDrawUseBox(Text:filler, 1);
				TextDrawTextSize(Text:filler, GetBarFill(x, width, max_value, value, direction), 0.0);
				TextDrawLetterSize(Text:filler, 1.0, ((height / 10) - 0.35));
				TextDrawBoxColor(Text:filler, fill_color);
			}
			else  {
				filler = _:CreatePlayerTextDraw(playerid, (x - 1.2), (y + 2.15), "_");
				PlayerTextDrawUseBox(playerid, PlayerText:filler, 1);
				PlayerTextDrawTextSize(playerid, PlayerText:filler, GetBarFill(x, width, max_value, value, direction), 0.0);
				PlayerTextDrawLetterSize(playerid, PlayerText:filler, 1.0, ((height / 10) - 0.35));
				PlayerTextDrawBoxColor(playerid, PlayerText:filler, fill_color);
			}
		}

		case BAR_DIRECTION_UP: {
		    if (playerid == -1) {
				filler = _:TextDrawCreate((x - 1.2), (y - 1.0), "_");
				TextDrawUseBox(Text:filler, 1);
				TextDrawTextSize(Text:filler, (x - width - 2.5), 0.0);
				TextDrawLetterSize(Text:filler, 1.0, GetBarFill(x, height, max_value, value, direction));
				TextDrawBoxColor(Text:filler, fill_color);
			}
			else  {
				filler = _:CreatePlayerTextDraw(playerid, (x - 1.2), (y - 1.0), "_");
				PlayerTextDrawUseBox(playerid, PlayerText:filler, 1);
				PlayerTextDrawTextSize(playerid, PlayerText:filler, (x - width - 2.5), 0.0);
				PlayerTextDrawLetterSize(playerid, PlayerText:filler, 1.0, GetBarFill(x, height, max_value, value, direction));
				PlayerTextDrawBoxColor(playerid, PlayerText:filler, fill_color);
			}
		}

		case BAR_DIRECTION_DOWN: {
		    if (playerid == -1) {
				filler = _:TextDrawCreate((x - 1.2), (y + 1.0), "_");
				TextDrawUseBox(Text:filler, 1);
				TextDrawTextSize(Text:filler, (x - width - 2.5), 0.0);
				TextDrawLetterSize(Text:filler, 1.0, GetBarFill(x, height, max_value, value, direction));
				TextDrawBoxColor(Text:filler, fill_color);
			}
			else  {
				filler = _:CreatePlayerTextDraw(playerid, (x - 1.2), (y + 1.0), "_");
				PlayerTextDrawUseBox(playerid, PlayerText:filler, 1);
				PlayerTextDrawTextSize(playerid, PlayerText:filler, (x - width - 2.5), 0.0);
				PlayerTextDrawLetterSize(playerid, PlayerText:filler, 1.0, GetBarFill(x, height, max_value, value, direction));
				PlayerTextDrawBoxColor(playerid, PlayerText:filler, fill_color);
			}
		}
	}

	return filler;
}

stock Bar:ProgressBarCreate(Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, direction = BAR_DIRECTION_RIGHT, playerid = -1) {
	if (playerid != -1 && (playerid < 0 || playerid >= MAX_PLAYERS)) {
		return INVALID_BAR_ID;
	}

	if (height == 0.0 || width == 0.0) {
		return INVALID_BAR_ID;
	}

	new Bar:index = INVALID_BAR_ID;

	if (playerid == -1) {
		for (new Bar:i = Bar:0; i < Bar:MAX_GLOBAL_BARS; i++) {
            if (globalProgressBarData[i][BAR_VALID] == false) {
				index = i;
				break;
			}
		}

		if (index == INVALID_BAR_ID) {
			return INVALID_BAR_ID;
		}

		globalProgressBarData[index][BAR_TEXTDRAW_BORDER] = CreateBarBorderTextDraw(x, y, width, height, DEFAULT_BAR_BORDER_COLOR, direction, playerid);
		if (globalProgressBarData[index][BAR_TEXTDRAW_BORDER] == INVALID_TEXT_DRAW) {
			return INVALID_BAR_ID;
		}

		globalProgressBarData[index][BAR_TEXTDRAW_BACKGROUND] = CreateBarBackgroundTextDraw(x, y, width, height, DEFAULT_BAR_BACKGROUND_COLOR, direction, playerid);
		if (globalProgressBarData[index][BAR_TEXTDRAW_BACKGROUND] == INVALID_TEXT_DRAW) {

		    TextDrawDestroy(Text:globalProgressBarData[index][BAR_TEXTDRAW_BORDER]);
		    globalProgressBarData[index][BAR_TEXTDRAW_BORDER] = INVALID_TEXT_DRAW;

			return INVALID_BAR_ID;
		}

		globalProgressBarData[index][BAR_TEXTDRAW_FILLER] = CreateBarFillerTextDraw(x, y, width, height, DEFAULT_BAR_FILLER_COLOR, DEFAULT_BAR_INITIAL_VALUE, DEFAULT_BAR_MAX_VALUE, direction, playerid);
		if (globalProgressBarData[index][BAR_TEXTDRAW_FILLER] == INVALID_TEXT_DRAW) {

		    TextDrawDestroy(Text:globalProgressBarData[index][BAR_TEXTDRAW_BORDER]);
		    globalProgressBarData[index][BAR_TEXTDRAW_BORDER] = INVALID_TEXT_DRAW;

		    TextDrawDestroy(Text:globalProgressBarData[index][BAR_TEXTDRAW_BACKGROUND]);
		    globalProgressBarData[index][BAR_TEXTDRAW_BACKGROUND] = INVALID_TEXT_DRAW;

			return INVALID_BAR_ID;
		}

		globalProgressBarData[index][BAR_DIRECTION] = direction;
		globalProgressBarData[index][BAR_X] = x;
		globalProgressBarData[index][BAR_Y] = y;
		globalProgressBarData[index][BAR_WIDTH] = width;
		globalProgressBarData[index][BAR_HEIGHT] = height;
		globalProgressBarData[index][BAR_VALUE] = DEFAULT_BAR_INITIAL_VALUE;
		globalProgressBarData[index][BAR_MAX_VALUE] = DEFAULT_BAR_MAX_VALUE;

		globalProgressBarData[index][BAR_FILLER_COLOR] = DEFAULT_BAR_FILLER_COLOR;

		globalProgressBarData[index][BAR_VALID] = true;

		for (new i = 0; i < MAX_PLAYERS; i++) {
  			globalProgressBarShown[i][index] = false;
		}

		return Bar:index;
	}
	else {
		for (new Bar:i = Bar:0; i < Bar:MAX_PLAYER_BARS; i++) {
            if (playerProgressBarData[playerid][i][BAR_VALID] == false) {
				index = i;
				break;
			}
		}

		if (index == INVALID_BAR_ID) {
			return INVALID_BAR_ID;
		}

		playerProgressBarData[playerid][index][BAR_TEXTDRAW_BORDER] = CreateBarBorderTextDraw(x, y, width, height, DEFAULT_BAR_BORDER_COLOR, direction, playerid);
		if (playerProgressBarData[playerid][index][BAR_TEXTDRAW_BORDER] == INVALID_TEXT_DRAW) {
			return INVALID_BAR_ID;
		}

		playerProgressBarData[playerid][index][BAR_TEXTDRAW_BACKGROUND] = CreateBarBackgroundTextDraw(x, y, width, height, DEFAULT_BAR_BACKGROUND_COLOR, direction, playerid);
		if (playerProgressBarData[playerid][index][BAR_TEXTDRAW_BACKGROUND] == INVALID_TEXT_DRAW) {

		    PlayerTextDrawDestroy(playerid, PlayerText:playerProgressBarData[playerid][index][BAR_TEXTDRAW_BORDER]);
		    playerProgressBarData[playerid][index][BAR_TEXTDRAW_BORDER] = INVALID_TEXT_DRAW;

			return INVALID_BAR_ID;
		}

		playerProgressBarData[playerid][index][BAR_TEXTDRAW_FILLER] = CreateBarFillerTextDraw(x, y, width, height, DEFAULT_BAR_FILLER_COLOR, DEFAULT_BAR_INITIAL_VALUE, DEFAULT_BAR_MAX_VALUE, direction, playerid);
		if (playerProgressBarData[playerid][index][BAR_TEXTDRAW_FILLER] == INVALID_TEXT_DRAW) {

		    PlayerTextDrawDestroy(playerid, PlayerText:playerProgressBarData[playerid][index][BAR_TEXTDRAW_BORDER]);
		    playerProgressBarData[playerid][index][BAR_TEXTDRAW_BORDER] = INVALID_TEXT_DRAW;

		    PlayerTextDrawDestroy(playerid, PlayerText:playerProgressBarData[playerid][index][BAR_TEXTDRAW_BACKGROUND]);
		    playerProgressBarData[playerid][index][BAR_TEXTDRAW_BACKGROUND] = INVALID_TEXT_DRAW;

			return INVALID_BAR_ID;
		}

		playerProgressBarData[playerid][index][BAR_DIRECTION] = direction;
		playerProgressBarData[playerid][index][BAR_X] = x;
		playerProgressBarData[playerid][index][BAR_Y] = y;
		playerProgressBarData[playerid][index][BAR_WIDTH] = width;
		playerProgressBarData[playerid][index][BAR_HEIGHT] = height;
		playerProgressBarData[playerid][index][BAR_VALUE] = DEFAULT_BAR_INITIAL_VALUE;
		playerProgressBarData[playerid][index][BAR_MAX_VALUE] = DEFAULT_BAR_MAX_VALUE;

		playerProgressBarData[playerid][index][BAR_FILLER_COLOR] = DEFAULT_BAR_FILLER_COLOR;

		playerProgressBarData[playerid][index][BAR_VALID] = true;

		for (new i = 0; i < MAX_PLAYERS; i++) {
  			playerProgressBarShown[playerid][index] = false;
		}

		return (Bar:(MAX_PLAYERS) + index);
	}
}

stock IsValidProgressBar(Bar:barid, playerid = -1) {
	if (playerid == -1) {
		if (barid < Bar:0 || barid >= Bar:MAX_GLOBAL_BARS) {
		    return 0;
		}

		return globalProgressBarData[barid][BAR_VALID];
	}
	else if (playerid >= 0 && playerid < MAX_PLAYERS) {
		if (barid < Bar:MAX_PLAYERS || barid >= Bar:(MAX_PLAYERS + MAX_PLAYER_BARS)) {
		    return 0;
		}

		barid -= Bar:MAX_PLAYERS;
		return playerProgressBarData[playerid][barid][BAR_VALID];
	}

	return 0;
}

stock ProgressBarDestroy(Bar:barid, playerid = -1) {
	if (!IsValidProgressBar(barid, playerid)) {
		return 0;
	}

	if (playerid == -1) {
		TextDrawDestroy(Text:globalProgressBarData[barid][BAR_TEXTDRAW_BORDER]);
		globalProgressBarData[barid][BAR_TEXTDRAW_BORDER] = INVALID_TEXT_DRAW;

		TextDrawDestroy(Text:globalProgressBarData[barid][BAR_TEXTDRAW_BACKGROUND]);
		globalProgressBarData[barid][BAR_TEXTDRAW_BACKGROUND] = INVALID_TEXT_DRAW;

		TextDrawDestroy(Text:globalProgressBarData[barid][BAR_TEXTDRAW_FILLER]);
		globalProgressBarData[barid][BAR_TEXTDRAW_FILLER] = INVALID_TEXT_DRAW;

		globalProgressBarData[barid][BAR_VALID] = false;
	}
	else {
		barid -= Bar:MAX_PLAYERS;

		PlayerTextDrawDestroy(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BORDER]);
		playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BORDER] = INVALID_TEXT_DRAW;

		PlayerTextDrawDestroy(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BACKGROUND]);
		playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = INVALID_TEXT_DRAW;

		PlayerTextDrawDestroy(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER]);
		playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER] = INVALID_TEXT_DRAW;

		playerProgressBarData[playerid][barid][BAR_VALID] = false;
	}

	return 1;
}

stock ProgressBarColor(Bar:barid, color, playerid = -1) {
	if (!IsValidProgressBar(barid, playerid)) {
		return 0;
	}

	if (playerid == -1) {
		globalProgressBarData[barid][BAR_FILLER_COLOR] = color;

		if ((color & 0xFF) == 0) {
			if (globalProgressBarData[barid][BAR_TEXTDRAW_FILLER] == INVALID_TEXT_DRAW) {
			    return 1;
			}
			else {
				TextDrawDestroy(Text:globalProgressBarData[barid][BAR_TEXTDRAW_FILLER]);
				globalProgressBarData[barid][BAR_TEXTDRAW_FILLER] = INVALID_TEXT_DRAW;
			}
		}
		else {
			if (globalProgressBarData[barid][BAR_TEXTDRAW_FILLER] == INVALID_TEXT_DRAW) {
				globalProgressBarData[barid][BAR_TEXTDRAW_FILLER] = CreateBarFillerTextDraw(globalProgressBarData[barid][BAR_X],
																							globalProgressBarData[barid][BAR_Y],
																							globalProgressBarData[barid][BAR_WIDTH],
																							globalProgressBarData[barid][BAR_HEIGHT],
																							color,
																							globalProgressBarData[barid][BAR_VALUE],
																							globalProgressBarData[barid][BAR_MAX_VALUE],
																							globalProgressBarData[barid][BAR_DIRECTION],
																							playerid);
			}
			else {
				TextDrawBoxColor(Text:globalProgressBarData[barid][BAR_TEXTDRAW_FILLER], color);
			}
		}
	}
	else {
		barid -= Bar:MAX_PLAYERS;

		playerProgressBarData[playerid][barid][BAR_FILLER_COLOR] = color;

		if ((color & 0xFF) == 0) {
			if (playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER] == INVALID_TEXT_DRAW) {
			    return 1;
			}
			else {
				PlayerTextDrawDestroy(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER]);
				playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER] = INVALID_TEXT_DRAW;
			}
		}
		else {
			if (playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER] == INVALID_TEXT_DRAW) {
				playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER] = CreateBarFillerTextDraw(playerProgressBarData[playerid][barid][BAR_X],
																									  playerProgressBarData[playerid][barid][BAR_Y],
																									  playerProgressBarData[playerid][barid][BAR_WIDTH],
																									  playerProgressBarData[playerid][barid][BAR_HEIGHT],
																									  color,
																									  playerProgressBarData[playerid][barid][BAR_VALUE],
																									  playerProgressBarData[playerid][barid][BAR_MAX_VALUE],
																									  playerProgressBarData[playerid][barid][BAR_DIRECTION],
																									  playerid);
			}
			else {
				PlayerTextDrawBoxColor(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER], color);
			}
		}
	}

	return 1;
}

stock ProgressBarBackgroundColor(Bar:barid, color, playerid = -1) {
	if (!IsValidProgressBar(barid, playerid)) {
		return 0;
	}

	if (playerid == -1) {
		if ((color & 0xFF) == 0) {
			if (globalProgressBarData[barid][BAR_TEXTDRAW_BACKGROUND] == INVALID_TEXT_DRAW) {
			    return 1;
			}
			else {
				TextDrawDestroy(Text:globalProgressBarData[barid][BAR_TEXTDRAW_BACKGROUND]);
				globalProgressBarData[barid][BAR_TEXTDRAW_BACKGROUND] = INVALID_TEXT_DRAW;
			}
		}
		else {
			if (globalProgressBarData[barid][BAR_TEXTDRAW_BACKGROUND] == INVALID_TEXT_DRAW) {
				globalProgressBarData[barid][BAR_TEXTDRAW_BACKGROUND] = CreateBarBackgroundTextDraw(globalProgressBarData[barid][BAR_X],
																									globalProgressBarData[barid][BAR_Y],
																									globalProgressBarData[barid][BAR_WIDTH],
																									globalProgressBarData[barid][BAR_HEIGHT],
																									color,
																									globalProgressBarData[barid][BAR_DIRECTION],
																									playerid);
			}
			else {
				TextDrawBoxColor(Text:globalProgressBarData[barid][BAR_TEXTDRAW_BACKGROUND], color);
			}
		}
	}
	else {
		barid -= Bar:MAX_PLAYERS;

		if ((color & 0xFF) == 0) {
			if (playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BACKGROUND] == INVALID_TEXT_DRAW) {
			    return 1;
			}
			else {
				PlayerTextDrawDestroy(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BACKGROUND]);
				playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = INVALID_TEXT_DRAW;
			}
		}
		else {
			if (playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BACKGROUND] == INVALID_TEXT_DRAW) {
				playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = CreateBarBackgroundTextDraw(playerProgressBarData[playerid][barid][BAR_X],
																											  playerProgressBarData[playerid][barid][BAR_Y],
																											  playerProgressBarData[playerid][barid][BAR_WIDTH],
																											  playerProgressBarData[playerid][barid][BAR_HEIGHT],
																											  color,
																											  playerProgressBarData[playerid][barid][BAR_DIRECTION],
																											  playerid);
			}
			else {
				PlayerTextDrawBoxColor(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BACKGROUND], color);
			}
		}
	}

	return 1;
}

stock ProgressBarBorderColor(Bar:barid, color, playerid = -1) {
	if (!IsValidProgressBar(barid, playerid)) {
		return 0;
	}

	if (playerid == -1) {
		if ((color & 0xFF) == 0) {
			if (globalProgressBarData[barid][BAR_TEXTDRAW_BORDER] == INVALID_TEXT_DRAW) {
			    return 1;
			}
			else {
				TextDrawDestroy(Text:globalProgressBarData[barid][BAR_TEXTDRAW_BORDER]);
				globalProgressBarData[barid][BAR_TEXTDRAW_BORDER] = INVALID_TEXT_DRAW;
			}
		}
		else {
			if (globalProgressBarData[barid][BAR_TEXTDRAW_BORDER] == INVALID_TEXT_DRAW) {
				globalProgressBarData[barid][BAR_TEXTDRAW_BORDER] = CreateBarBorderTextDraw(globalProgressBarData[barid][BAR_X],
																							globalProgressBarData[barid][BAR_Y],
																							globalProgressBarData[barid][BAR_WIDTH],
																							globalProgressBarData[barid][BAR_HEIGHT],
																							color,
																							globalProgressBarData[barid][BAR_DIRECTION],
																							playerid);
			}
			else {
				TextDrawBoxColor(Text:globalProgressBarData[barid][BAR_TEXTDRAW_BORDER], color);
			}
		}
	}
	else {
		barid -= Bar:MAX_PLAYERS;

		if ((color & 0xFF) == 0) {
			if (playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BORDER] == INVALID_TEXT_DRAW) {
			    return 1;
			}
			else {
				PlayerTextDrawDestroy(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BORDER]);
				playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BORDER] = INVALID_TEXT_DRAW;
			}
		}
		else {
			if (playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BORDER] == INVALID_TEXT_DRAW) {
				playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BORDER] = CreateBarBorderTextDraw(playerProgressBarData[playerid][barid][BAR_X],
																									  playerProgressBarData[playerid][barid][BAR_Y],
																									  playerProgressBarData[playerid][barid][BAR_WIDTH],
																									  playerProgressBarData[playerid][barid][BAR_HEIGHT],
																									  color,
																									  playerProgressBarData[playerid][barid][BAR_DIRECTION],
																									  playerid);
			}
			else {
				PlayerTextDrawBoxColor(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BORDER], color);
			}
		}
	}

	return 1;
}

stock ProgressBarSetValue(Bar:barid, Float:value, playerid = -1) {
	if (!IsValidProgressBar(barid, playerid)) {
		return 0;
	}

	if (playerid == -1) {
	    if (globalProgressBarData[barid][BAR_VALUE] == value) {
			return 1;
		}

		if (value > globalProgressBarData[barid][BAR_MAX_VALUE]) {
			return 0;
		}

	    globalProgressBarData[barid][BAR_VALUE] = value;

		TextDrawDestroy(Text:globalProgressBarData[barid][BAR_TEXTDRAW_FILLER]);
		globalProgressBarData[barid][BAR_TEXTDRAW_FILLER] = CreateBarFillerTextDraw(globalProgressBarData[barid][BAR_X],
																					globalProgressBarData[barid][BAR_Y],
																					globalProgressBarData[barid][BAR_WIDTH],
																					globalProgressBarData[barid][BAR_HEIGHT],
																					globalProgressBarData[barid][BAR_FILLER_COLOR],
																					globalProgressBarData[barid][BAR_VALUE],
																					globalProgressBarData[barid][BAR_MAX_VALUE],
																					globalProgressBarData[barid][BAR_DIRECTION],
																					playerid);

		#if defined foreach
		foreach(new i : Player) {
		#else
		for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) {
      	if (IsPlayerConnected(i)) {
		#endif
			if (globalProgressBarShown[i][barid] == true) {
				TextDrawShowForPlayer(i, Text:globalProgressBarData[barid][BAR_TEXTDRAW_FILLER]);
			}
		#if !defined foreach
		}
		#endif
		}
	}
	else {
		barid -= Bar:MAX_PLAYERS;

	    if (playerProgressBarData[playerid][barid][BAR_VALUE] == value) {
			return 1;
		}

		if (value > playerProgressBarData[playerid][barid][BAR_MAX_VALUE]) {
			return 0;
		}

	    playerProgressBarData[playerid][barid][BAR_VALUE] = value;

	    PlayerTextDrawDestroy(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER]);
		playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER] = CreateBarFillerTextDraw(playerProgressBarData[playerid][barid][BAR_X],
																							  playerProgressBarData[playerid][barid][BAR_Y],
																							  playerProgressBarData[playerid][barid][BAR_WIDTH],
																							  playerProgressBarData[playerid][barid][BAR_HEIGHT],
																							  playerProgressBarData[playerid][barid][BAR_FILLER_COLOR],
																							  playerProgressBarData[playerid][barid][BAR_VALUE],
																							  playerProgressBarData[playerid][barid][BAR_MAX_VALUE],
																							  playerProgressBarData[playerid][barid][BAR_DIRECTION],
																							  playerid);

		if (playerProgressBarShown[playerid][barid] == true) {
			PlayerTextDrawShow(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER]);
		}
	}

	return 1;
}

stock ProgressBarSetMaxValue(Bar:barid, Float:max_value, playerid = -1) {
	if (!IsValidProgressBar(barid, playerid)) {
		return 0;
	}

	if (playerid == -1) {
	    if (globalProgressBarData[barid][BAR_MAX_VALUE] == max_value) {
			return 1;
		}

	    globalProgressBarData[barid][BAR_MAX_VALUE] = max_value;

	    if (max_value < globalProgressBarData[barid][BAR_VALUE]) {
            globalProgressBarData[barid][BAR_VALUE] = max_value;
		}

		TextDrawDestroy(Text:globalProgressBarData[barid][BAR_TEXTDRAW_FILLER]);
		globalProgressBarData[barid][BAR_TEXTDRAW_FILLER] = CreateBarFillerTextDraw(globalProgressBarData[barid][BAR_X],
																					globalProgressBarData[barid][BAR_Y],
																					globalProgressBarData[barid][BAR_WIDTH],
																					globalProgressBarData[barid][BAR_HEIGHT],
																					globalProgressBarData[barid][BAR_FILLER_COLOR],
																					globalProgressBarData[barid][BAR_VALUE],
																					globalProgressBarData[barid][BAR_MAX_VALUE],
																					globalProgressBarData[barid][BAR_DIRECTION],
																					playerid);

		#if defined foreach
		foreach(new i : Player) {
		#else
		for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) {
      	if (IsPlayerConnected(i)) {
		#endif
			if (globalProgressBarShown[i][barid] == true) {
				TextDrawShowForPlayer(i, Text:globalProgressBarData[barid][BAR_TEXTDRAW_FILLER]);
			}
		#if !defined foreach
		}
		#endif
		}
	}
	else {
		barid -= Bar:MAX_PLAYERS;

	    if (playerProgressBarData[playerid][barid][BAR_MAX_VALUE] == max_value) {
			return 1;
		}

	    playerProgressBarData[playerid][barid][BAR_MAX_VALUE] = max_value;

	    if (max_value < playerProgressBarData[playerid][barid][BAR_VALUE]) {
            playerProgressBarData[playerid][barid][BAR_VALUE] = max_value;
		}

	    PlayerTextDrawDestroy(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER]);
		playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER] = CreateBarFillerTextDraw(playerProgressBarData[playerid][barid][BAR_X],
																							  playerProgressBarData[playerid][barid][BAR_Y],
																							  playerProgressBarData[playerid][barid][BAR_WIDTH],
																							  playerProgressBarData[playerid][barid][BAR_HEIGHT],
																							  playerProgressBarData[playerid][barid][BAR_FILLER_COLOR],
																							  playerProgressBarData[playerid][barid][BAR_VALUE],
																							  playerProgressBarData[playerid][barid][BAR_MAX_VALUE],
																							  playerProgressBarData[playerid][barid][BAR_DIRECTION],
																							  playerid);

		if (playerProgressBarShown[playerid][barid] == true) {
			PlayerTextDrawShow(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER]);
		}
	}

	return 1;
}

stock ProgressBarShowForPlayer(playerid, Bar:barid) {
    if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
    }

	if (barid < Bar:MAX_PLAYERS) {
	   	if (barid < Bar:0 || barid >= Bar:MAX_GLOBAL_BARS) {
			return 0;
		}

		TextDrawShowForPlayer(playerid, Text:globalProgressBarData[barid][BAR_TEXTDRAW_BORDER]);
		TextDrawShowForPlayer(playerid, Text:globalProgressBarData[barid][BAR_TEXTDRAW_BACKGROUND]);
		TextDrawShowForPlayer(playerid, Text:globalProgressBarData[barid][BAR_TEXTDRAW_FILLER]);
		globalProgressBarShown[playerid][barid] = true;
	}
	else {
	   	if (barid < Bar:MAX_PLAYERS || barid >= Bar:(MAX_PLAYERS + MAX_PLAYER_BARS)) {
			return 0;
		}

		barid -= Bar:MAX_PLAYERS;

		PlayerTextDrawShow(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BORDER]);
		PlayerTextDrawShow(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BACKGROUND]);
		PlayerTextDrawShow(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER]);
		playerProgressBarShown[playerid][barid] = true;
	}

	return 1;
}

stock ProgressBarHideForPlayer(playerid, Bar:barid) {
    if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
    }

	if (barid < MAX_PLAYERS) {
	   	if (barid < Bar:0 || barid >= Bar:MAX_GLOBAL_BARS) {
			return 0;
		}

		TextDrawHideForPlayer(playerid, Text:globalProgressBarData[barid][BAR_TEXTDRAW_BORDER]);
		TextDrawHideForPlayer(playerid, Text:globalProgressBarData[barid][BAR_TEXTDRAW_BACKGROUND]);
		TextDrawHideForPlayer(playerid, Text:globalProgressBarData[barid][BAR_TEXTDRAW_FILLER]);
		globalProgressBarShown[playerid][barid] = false;
	}
	else {
	   	if (barid < Bar:MAX_PLAYERS || barid >= Bar:(MAX_PLAYERS + MAX_PLAYER_BARS)) {
			return 0;
		}

		barid -= Bar:MAX_PLAYERS;

		PlayerTextDrawHide(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BORDER]);
		PlayerTextDrawHide(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_BACKGROUND]);
		PlayerTextDrawHide(playerid, PlayerText:playerProgressBarData[playerid][barid][BAR_TEXTDRAW_FILLER]);
		playerProgressBarShown[playerid][barid] = false;
	}

	return 1;
}
