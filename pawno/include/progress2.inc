#if defined _progress2_included
	#endinput
#endif
#define _progress2_included

#if !defined MAX_PLAYER_BARS
	#define MAX_PLAYER_BARS \
		(MAX_PLAYER_TEXT_DRAWS / 3)
#endif

#if !defined MAX_BARS
	#define MAX_BARS \
		(MAX_TEXT_DRAWS / 3)
#endif

#define INVALID_BAR_ID \
	(Bar:-1)

#define INVALID_PLAYER_BAR_ID \
	(PlayerBar:-1)

#define BAR_DIRECTION_RIGHT \
	0
#define BAR_DIRECTION_LEFT \
	1
#define BAR_DIRECTION_UP \
	2
#define BAR_DIRECTION_DOWN \
	3

#define BAR_TEXTDRAW_BACKGROUND \
	0
#define BAR_TEXTDRAW_FILL \
	1
#define BAR_TEXTDRAW_MAIN \
	2

enum e_BAR
{
	e_BAR_DIRECTION,
	Float:e_BAR_X,
	Float:e_BAR_Y,
	Float:e_BAR_WIDTH,
	Float:e_BAR_HEIGHT,
	e_BAR_COLOR,
	e_BAR_BACKGROUND_COLOR,
	Float:e_BAR_VALUE,
	Float:e_BAR_MAX_VALUE
};

static eBar[Bar:MAX_BARS][e_BAR];
static Text:txtBar[Bar:MAX_BARS][3];
static bool:bBarShownForPlayer[Bar:MAX_BARS][MAX_PLAYERS];
static iBarCount;

static ePlayerBar[MAX_PLAYERS][PlayerBar:MAX_PLAYER_BARS][e_BAR];
static PlayerText:ptxtBar[MAX_PLAYERS][PlayerBar:MAX_PLAYER_BARS][3];
static bool:bPlayerBarShown[MAX_PLAYERS][PlayerBar:MAX_PLAYER_BARS];
static iPlayerBarCount[MAX_PLAYERS];

static Float:PB_GetBarPercent(Float:x, Float:widthorheight, Float:max, Float:value, direction)
{
	new Float:ret;
	switch(direction)
	{
		case BAR_DIRECTION_RIGHT:
		{
			ret = ((x - 3.0) + (((((x - 2.0) + widthorheight) - x) / max) * value));
		}

		case BAR_DIRECTION_LEFT:
		{
			ret = ((x - 1.0) - (((((x + 2.0) - widthorheight) - x) / max) * -value)) - 4.0;
		}

		case BAR_DIRECTION_UP:
		{
			ret = -((((((widthorheight / 10.0) - 0.45) * 1.02) / max) * value) + 0.55);
		}

		case BAR_DIRECTION_DOWN:
		{
			ret = ((((((widthorheight / 10.0) - 0.45) * 1.02) / max) * value) - 0.55);
		}
	}
	return ret;
}

stock static PB_CreateBarBackground(Bar:barid)
{
	new direction = eBar[barid][e_BAR_DIRECTION],
		Float:x = eBar[barid][e_BAR_X],
	    Float:y = eBar[barid][e_BAR_Y],
	    Float:width = eBar[barid][e_BAR_WIDTH],
	    Float:height = eBar[barid][e_BAR_HEIGHT],
	    bgcolor = eBar[barid][e_BAR_BACKGROUND_COLOR];

    switch (direction)
	{
		case BAR_DIRECTION_RIGHT:
		{
			txtBar[barid][BAR_TEXTDRAW_BACKGROUND] = TextDrawCreate(x, y, "_");
			TextDrawUseBox(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1);
			TextDrawTextSize(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], ((x + width) - 4.0), 0.0);
			TextDrawLetterSize(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1.0, (height / 10));
			TextDrawBoxColor(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}

		case BAR_DIRECTION_LEFT:
		{
			txtBar[barid][BAR_TEXTDRAW_BACKGROUND] = TextDrawCreate(x, y, "_");
			TextDrawUseBox(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1);
			TextDrawTextSize(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], ((x - width) - 4.0), 0.0);
			TextDrawLetterSize(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1.0, (height / 10));
			TextDrawBoxColor(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}

		case BAR_DIRECTION_UP:
		{
			txtBar[barid][BAR_TEXTDRAW_BACKGROUND] = TextDrawCreate(x, y, "_");
			TextDrawUseBox(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1);
			TextDrawTextSize(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], ((x - width) - 4.0), 0.0);
			TextDrawLetterSize(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1.0, (-((height / 10) * 1.02) - 0.35));
			TextDrawBoxColor(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}

		case BAR_DIRECTION_DOWN:
		{
			txtBar[barid][BAR_TEXTDRAW_BACKGROUND] = TextDrawCreate(x, y, "_");
			TextDrawUseBox(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1);
			TextDrawTextSize(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], ((x - width) - 4.0), 0.0);
			TextDrawLetterSize(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1.0, ((height / 10) - 0.35));
			TextDrawBoxColor(txtBar[barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}
	}
}

stock static PB_CreateBarFill(Bar:barid)
{
	new direction = eBar[barid][e_BAR_DIRECTION],
		Float:x = eBar[barid][e_BAR_X],
	    Float:y = eBar[barid][e_BAR_Y],
	    Float:width = eBar[barid][e_BAR_WIDTH],
	    Float:height = eBar[barid][e_BAR_HEIGHT],
	    color = eBar[barid][e_BAR_COLOR];

    switch (direction)
	{
		case BAR_DIRECTION_RIGHT:
		{
			txtBar[barid][BAR_TEXTDRAW_FILL] = TextDrawCreate((x + 1.2), (y + 2.15), "_");
			TextDrawUseBox(txtBar[barid][BAR_TEXTDRAW_FILL], 1);
			TextDrawTextSize(txtBar[barid][BAR_TEXTDRAW_FILL], ((x + width) - 5.5), 0.0);
			TextDrawLetterSize(txtBar[barid][BAR_TEXTDRAW_FILL], 1.0, ((height / 10) - 0.35));
			TextDrawBoxColor(txtBar[barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}

		case BAR_DIRECTION_LEFT:
		{
			txtBar[barid][BAR_TEXTDRAW_FILL] = TextDrawCreate((x - 1.2), (y + 2.15), "_");
			TextDrawUseBox(txtBar[barid][BAR_TEXTDRAW_FILL], 1);
			TextDrawTextSize(txtBar[barid][BAR_TEXTDRAW_FILL], ((x - width) - 2.5), 0.0);
			TextDrawLetterSize(txtBar[barid][BAR_TEXTDRAW_FILL], 1.0, ((height / 10) - 0.35));
			TextDrawBoxColor(txtBar[barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}

		case BAR_DIRECTION_UP:
		{
			txtBar[barid][BAR_TEXTDRAW_FILL] = TextDrawCreate((x - 1.2), (y - 1.0), "_");
			TextDrawUseBox(txtBar[barid][BAR_TEXTDRAW_FILL], 1);
			TextDrawTextSize(txtBar[barid][BAR_TEXTDRAW_FILL], ((x - width) - 2.5), 0.0);
			TextDrawLetterSize(txtBar[barid][BAR_TEXTDRAW_FILL], 1.0, -((height / 10) - 1.02));
			TextDrawBoxColor(txtBar[barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}

		case BAR_DIRECTION_DOWN:
		{
			txtBar[barid][BAR_TEXTDRAW_FILL] = TextDrawCreate((x - 1.2), (y + 1.0), "_");
			TextDrawUseBox(txtBar[barid][BAR_TEXTDRAW_FILL], 1);
			TextDrawTextSize(txtBar[barid][BAR_TEXTDRAW_FILL], ((x - width) - 2.5), 0.0);
			TextDrawLetterSize(txtBar[barid][BAR_TEXTDRAW_FILL], 1.0, ((height / 10) - 0.55));
			TextDrawBoxColor(txtBar[barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}
	}
}

stock static PB_CreateBarMain(Bar:barid)
{
	new direction = eBar[barid][e_BAR_DIRECTION],
		Float:x = eBar[barid][e_BAR_X],
	    Float:y = eBar[barid][e_BAR_Y],
	    Float:width = eBar[barid][e_BAR_WIDTH],
	    Float:height = eBar[barid][e_BAR_HEIGHT],
	    Float:value = eBar[barid][e_BAR_VALUE],
	    Float:max_value = eBar[barid][e_BAR_MAX_VALUE],
	    color = eBar[barid][e_BAR_COLOR];

	switch (direction)
	{
		case BAR_DIRECTION_RIGHT:
		{
			txtBar[barid][BAR_TEXTDRAW_MAIN] = TextDrawCreate((x + 1.2), (y + 2.15), "_");
			TextDrawUseBox(txtBar[barid][BAR_TEXTDRAW_MAIN], 1);
			TextDrawTextSize(txtBar[barid][BAR_TEXTDRAW_MAIN], PB_GetBarPercent(x, width, max_value, value, direction), 0.0);
			TextDrawLetterSize(txtBar[barid][BAR_TEXTDRAW_MAIN], 1.0, ((height / 10) - 0.35));
			TextDrawBoxColor(txtBar[barid][BAR_TEXTDRAW_MAIN], color);
		}

		case BAR_DIRECTION_LEFT:
		{
			txtBar[barid][BAR_TEXTDRAW_MAIN] = TextDrawCreate((x - 1.2), (y + 2.15), "_");
			TextDrawUseBox(txtBar[barid][BAR_TEXTDRAW_MAIN], 1);
			TextDrawTextSize(txtBar[barid][BAR_TEXTDRAW_MAIN], PB_GetBarPercent(x, width, max_value, value, direction), 0.0);
			TextDrawLetterSize(txtBar[barid][BAR_TEXTDRAW_MAIN], 1.0, ((height / 10) - 0.35));
			TextDrawBoxColor(txtBar[barid][BAR_TEXTDRAW_MAIN], color);
		}

		case BAR_DIRECTION_UP:
		{
			txtBar[barid][BAR_TEXTDRAW_MAIN] = TextDrawCreate((x - 1.2), (y - 1.0), "_");
			TextDrawUseBox(txtBar[barid][BAR_TEXTDRAW_MAIN], 1);
			TextDrawTextSize(txtBar[barid][BAR_TEXTDRAW_MAIN], ((x - width) - 2.5), 0.0);
			TextDrawLetterSize(txtBar[barid][BAR_TEXTDRAW_MAIN], 1.0, PB_GetBarPercent(x, width, max_value, value, direction));
			TextDrawBoxColor(txtBar[barid][BAR_TEXTDRAW_MAIN], color);
		}

		case BAR_DIRECTION_DOWN:
		{
			txtBar[barid][BAR_TEXTDRAW_MAIN] = TextDrawCreate((x - 1.2), (y + 1.0), "_");
			TextDrawUseBox(txtBar[barid][BAR_TEXTDRAW_MAIN], 1);
			TextDrawTextSize(txtBar[barid][BAR_TEXTDRAW_MAIN], ((x - width) - 2.5), 0.0);
			TextDrawLetterSize(txtBar[barid][BAR_TEXTDRAW_MAIN], 1.0, PB_GetBarPercent(x, width, max_value, value, direction));
			TextDrawBoxColor(txtBar[barid][BAR_TEXTDRAW_MAIN], color);
		}
	}
}

stock static PB_CreatePlayerBarBackground(playerid, PlayerBar:barid)
{
	new direction = ePlayerBar[playerid][barid][e_BAR_DIRECTION],
		Float:x = ePlayerBar[playerid][barid][e_BAR_X],
	    Float:y = ePlayerBar[playerid][barid][e_BAR_Y],
	    Float:width = ePlayerBar[playerid][barid][e_BAR_WIDTH],
	    Float:height = ePlayerBar[playerid][barid][e_BAR_HEIGHT],
	    bgcolor = ePlayerBar[playerid][barid][e_BAR_BACKGROUND_COLOR];

    switch (direction)
	{
		case BAR_DIRECTION_RIGHT:
		{
			ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = CreatePlayerTextDraw(playerid, x, y, "_");
			PlayerTextDrawUseBox(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1);
			PlayerTextDrawTextSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], ((x + width) - 4.0), 0.0);
			PlayerTextDrawLetterSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1.0, (height / 10));
			PlayerTextDrawBoxColor(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}

		case BAR_DIRECTION_LEFT:
		{
			ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = CreatePlayerTextDraw(playerid, x, y, "_");
			PlayerTextDrawUseBox(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1);
			PlayerTextDrawTextSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], ((x - width) - 4.0), 0.0);
			PlayerTextDrawLetterSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1.0, (height / 10));
			PlayerTextDrawBoxColor(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}

		case BAR_DIRECTION_UP:
		{
			ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = CreatePlayerTextDraw(playerid, x, y, "_");
			PlayerTextDrawUseBox(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1);
			PlayerTextDrawTextSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], ((x - width) - 4.0), 0.0);
			PlayerTextDrawLetterSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1.0, (-((height / 10) * 1.02) - 0.35));
			PlayerTextDrawBoxColor(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}

		case BAR_DIRECTION_DOWN:
		{
			ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = CreatePlayerTextDraw(playerid, x, y, "_");
			PlayerTextDrawUseBox(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1);
			PlayerTextDrawTextSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], ((x - width) - 4.0), 0.0);
			PlayerTextDrawLetterSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1.0, ((height / 10) - 0.35));
			PlayerTextDrawBoxColor(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}
	}
}

stock static PB_CreatePlayerBarFill(playerid, PlayerBar:barid)
{
	new direction = ePlayerBar[playerid][barid][e_BAR_DIRECTION],
		Float:x = ePlayerBar[playerid][barid][e_BAR_X],
	    Float:y = ePlayerBar[playerid][barid][e_BAR_Y],
	    Float:width = ePlayerBar[playerid][barid][e_BAR_WIDTH],
	    Float:height = ePlayerBar[playerid][barid][e_BAR_HEIGHT],
	    color = ePlayerBar[playerid][barid][e_BAR_COLOR];

    switch (direction)
	{
		case BAR_DIRECTION_RIGHT:
		{
			ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL] = CreatePlayerTextDraw(playerid, (x + 1.2), (y + 2.15), "_");
			PlayerTextDrawUseBox(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1);
			PlayerTextDrawTextSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((x + width) - 5.5), 0.0);
			PlayerTextDrawLetterSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1.0, ((height / 10) - 0.35));
			PlayerTextDrawBoxColor(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}

		case BAR_DIRECTION_LEFT:
		{
			ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL] = CreatePlayerTextDraw(playerid, (x - 1.2), (y + 2.15), "_");
			PlayerTextDrawUseBox(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1);
			PlayerTextDrawTextSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((x - width) - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1.0, ((height / 10) - 0.35));
			PlayerTextDrawBoxColor(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}

		case BAR_DIRECTION_UP:
		{
			ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL] = CreatePlayerTextDraw(playerid, (x - 1.2), (y - 1.0), "_");
			PlayerTextDrawUseBox(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1);
			PlayerTextDrawTextSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((x - width) - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1.0, -((height / 10) - 1.02));
			PlayerTextDrawBoxColor(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}

		case BAR_DIRECTION_DOWN:
		{
			ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL] = CreatePlayerTextDraw(playerid, (x - 1.2), (y + 1.0), "_");
			PlayerTextDrawUseBox(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1);
			PlayerTextDrawTextSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((x - width) - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1.0, ((height / 10) - 0.55));
			PlayerTextDrawBoxColor(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}
	}
}

stock static PB_CreatePlayerBarMain(playerid, PlayerBar:barid)
{
	new direction = ePlayerBar[playerid][barid][e_BAR_DIRECTION],
		Float:x = ePlayerBar[playerid][barid][e_BAR_X],
	    Float:y = ePlayerBar[playerid][barid][e_BAR_Y],
	    Float:width = ePlayerBar[playerid][barid][e_BAR_WIDTH],
	    Float:height = ePlayerBar[playerid][barid][e_BAR_HEIGHT],
	    Float:value = ePlayerBar[playerid][barid][e_BAR_VALUE],
	    Float:max_value = ePlayerBar[playerid][barid][e_BAR_MAX_VALUE],
	    color = ePlayerBar[playerid][barid][e_BAR_COLOR];

	switch (direction)
	{
		case BAR_DIRECTION_RIGHT:
		{
			ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN] = CreatePlayerTextDraw(playerid, (x + 1.2), (y + 2.15), "_");
			PlayerTextDrawUseBox(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1);
			PlayerTextDrawTextSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], PB_GetBarPercent(x, width, max_value, value, direction), 0.0);
			PlayerTextDrawLetterSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1.0, ((height / 10) - 0.35));
			PlayerTextDrawBoxColor(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], color);
		}

		case BAR_DIRECTION_LEFT:
		{
			ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN] = CreatePlayerTextDraw(playerid, (x - 1.2), (y + 2.15), "_");
			PlayerTextDrawUseBox(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1);
			PlayerTextDrawTextSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], PB_GetBarPercent(x, width, max_value, value, direction), 0.0);
			PlayerTextDrawLetterSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1.0, ((height / 10) - 0.35));
			PlayerTextDrawBoxColor(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], color);
		}

		case BAR_DIRECTION_UP:
		{
			ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN] = CreatePlayerTextDraw(playerid, (x - 1.2), (y - 1.0), "_");
			PlayerTextDrawUseBox(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1);
			PlayerTextDrawTextSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], ((x - width) - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1.0, PB_GetBarPercent(x, width, max_value, value, direction));
			PlayerTextDrawBoxColor(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], color);
		}

		case BAR_DIRECTION_DOWN:
		{
			ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN] = CreatePlayerTextDraw(playerid, (x - 1.2), (y + 1.0), "_");
			PlayerTextDrawUseBox(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1);
			PlayerTextDrawTextSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], ((x - width) - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1.0, PB_GetBarPercent(x, width, max_value, value, direction));
			PlayerTextDrawBoxColor(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], color);
		}
	}
}

/*
native PlayerBar_Create(playerid, Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, color = 0xFF1C1CFF, Float:max = 100.0, direction = BAR_DIRECTION_RIGHT, bgcolor = 0x000000FF);
*/
stock PlayerBar:PlayerBar_Create(playerid, Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, color = 0xFF1C1CFF, Float:max = 100.0, direction = BAR_DIRECTION_RIGHT, bgcolor = 0x000000FF)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return INVALID_PLAYER_BAR_ID;
	}

	if (iPlayerBarCount[playerid] == MAX_BARS)
	{
	    return INVALID_PLAYER_BAR_ID;
	}

	new PlayerBar:barid = PlayerBar:iPlayerBarCount[playerid]++;

	ePlayerBar[playerid][barid][e_BAR_DIRECTION] = direction;
	ePlayerBar[playerid][barid][e_BAR_X] = x;
	ePlayerBar[playerid][barid][e_BAR_Y] = y;
	ePlayerBar[playerid][barid][e_BAR_WIDTH] = width;
	ePlayerBar[playerid][barid][e_BAR_HEIGHT] = height;
	ePlayerBar[playerid][barid][e_BAR_COLOR] = color;
	ePlayerBar[playerid][barid][e_BAR_BACKGROUND_COLOR] = bgcolor;
	ePlayerBar[playerid][barid][e_BAR_VALUE] = 0.0;
	ePlayerBar[playerid][barid][e_BAR_MAX_VALUE] = max;

    bPlayerBarShown[playerid][barid] = false;

	PB_CreatePlayerBarBackground(playerid, barid);
	PB_CreatePlayerBarFill(playerid, barid);
	PB_CreatePlayerBarMain(playerid, barid);
	return barid;
}

#define CreatePlayerProgressBar \
    PlayerBar_Create

/*
native PlayerBar_IsValid(playerid, PlayerBar:barid);
*/
stock PlayerBar_IsValid(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (barid < PlayerBar:0 || barid >= PlayerBar:iPlayerBarCount[playerid])
	{
	    return 0;
	}

	return 1;
}

#define IsValidPlayerProgressBar \
    PlayerBar_IsValid

/*
native PlayerBar_Destroy(playerid, PlayerBar:barid);
*/
stock PlayerBar_Destroy(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!PlayerBar_IsValid(playerid, barid))
	{
	    return 0;
	}

	PlayerTextDrawDestroy(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND]);
	PlayerTextDrawDestroy(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
	PlayerTextDrawDestroy(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);

	for (new PlayerBar:i = barid; i < (PlayerBar:(iPlayerBarCount[playerid] - 1)); i++)
	{
		ePlayerBar[playerid][i][e_BAR_DIRECTION] = ePlayerBar[playerid][(i + PlayerBar:1)][e_BAR_DIRECTION];
		ePlayerBar[playerid][i][e_BAR_X] = ePlayerBar[playerid][(i + PlayerBar:1)][e_BAR_X];
		ePlayerBar[playerid][i][e_BAR_Y] = ePlayerBar[playerid][(i + PlayerBar:1)][e_BAR_Y];
		ePlayerBar[playerid][i][e_BAR_WIDTH] = ePlayerBar[playerid][(i + PlayerBar:1)][e_BAR_WIDTH];
		ePlayerBar[playerid][i][e_BAR_HEIGHT] = ePlayerBar[playerid][(i + PlayerBar:1)][e_BAR_HEIGHT];
		ePlayerBar[playerid][i][e_BAR_COLOR] = ePlayerBar[playerid][(i + PlayerBar:1)][e_BAR_COLOR];
		ePlayerBar[playerid][i][e_BAR_BACKGROUND_COLOR] = ePlayerBar[playerid][(i + PlayerBar:1)][e_BAR_BACKGROUND_COLOR];
		ePlayerBar[playerid][i][e_BAR_VALUE] = ePlayerBar[playerid][(i + PlayerBar:1)][e_BAR_VALUE];
		ePlayerBar[playerid][i][e_BAR_MAX_VALUE] = ePlayerBar[playerid][(i + PlayerBar:1)][e_BAR_MAX_VALUE];

    	bPlayerBarShown[playerid][i] = bPlayerBarShown[playerid][(i + PlayerBar:1)];

		ptxtBar[playerid][i][BAR_TEXTDRAW_BACKGROUND] = ptxtBar[playerid][(i + PlayerBar:1)][BAR_TEXTDRAW_BACKGROUND];
		ptxtBar[playerid][i][BAR_TEXTDRAW_FILL] = ptxtBar[playerid][(i + PlayerBar:1)][BAR_TEXTDRAW_FILL];
		ptxtBar[playerid][i][BAR_TEXTDRAW_MAIN] = ptxtBar[playerid][(i + PlayerBar:1)][BAR_TEXTDRAW_MAIN];
	}

	iPlayerBarCount[playerid]--;
	return 1;
}

#define DestroyPlayerProgressBar \
    PlayerBar_Destroy

/*
native PlayerBar_Show(playerid, PlayerBar:barid);
*/
stock PlayerBar_Show(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!PlayerBar_IsValid(playerid, barid))
	{
	    return 0;
	}

	PlayerTextDrawShow(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND]);
	PlayerTextDrawShow(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
	PlayerTextDrawShow(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);

	bPlayerBarShown[playerid][barid] = true;
	return 1;
}

#define ShowPlayerProgressBar \
    PlayerBar_Show

/*
native PlayerBar_Hide(playerid, PlayerBar:barid);
*/
stock PlayerBar_Hide(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!PlayerBar_IsValid(playerid, barid))
	{
	    return 0;
	}

	PlayerTextDrawHide(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND]);
	PlayerTextDrawHide(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
	PlayerTextDrawHide(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);

	bPlayerBarShown[playerid][barid] = false;
	return 1;
}

#define HidePlayerProgressBar \
    PlayerBar_Hide

/*
native PlayerBar_GetColor(playerid, PlayerBar:barid);
*/
stock PlayerBar_GetColor(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!PlayerBar_IsValid(playerid, barid))
	{
	    return 0;
	}

	return ePlayerBar[playerid][barid][e_BAR_COLOR];
}

#define GetPlayerProgressBarColor \
    PlayerBar_GetColor

/*
native Float:PlayerBar_GetValue(playerid, PlayerBar:barid);
*/
stock Float:PlayerBar_GetValue(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0.0;
	}

	if (!PlayerBar_IsValid(playerid, barid))
	{
	    return 0.0;
	}

	return ePlayerBar[playerid][barid][e_BAR_VALUE];
}

#define GetPlayerProgressBarValue \
    PlayerBar_GetValue
    
/*
native PlayerBar_Update(playerid, PlayerBar:barid, Float:value, color);
*/
stock PlayerBar_Update(playerid, PlayerBar:barid, Float:value, color)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!PlayerBar_IsValid(playerid, barid))
	{
	    return 0;
	}

	if (value != ePlayerBar[playerid][barid][e_BAR_VALUE])
	{
		ePlayerBar[playerid][barid][e_BAR_VALUE] = ((value >= ePlayerBar[playerid][barid][e_BAR_MAX_VALUE]) ? (ePlayerBar[playerid][barid][e_BAR_MAX_VALUE]) : (value));

		PlayerTextDrawDestroy(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
		PB_CreatePlayerBarFill(playerid, barid);
		PlayerTextDrawDestroy(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);
		PB_CreatePlayerBarMain(playerid, barid);

		if (bPlayerBarShown[playerid][barid])
		{
			PlayerTextDrawShow(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
			PlayerTextDrawShow(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);
		}
		return 1;
	}

	if (color != ePlayerBar[playerid][barid][e_BAR_COLOR])
	{
		ePlayerBar[playerid][barid][e_BAR_COLOR] = color;

		PlayerTextDrawBoxColor(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		PlayerTextDrawBoxColor(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], color);

		if (bPlayerBarShown[playerid][barid])
		{
			PlayerTextDrawShow(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
			PlayerTextDrawShow(playerid, ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);
		}
	}
	return 1;
}

stock SetPlayerProgressBarColour(playerid, PlayerBar:barid, color)
{
	return PlayerBar_Update(playerid, barid, PlayerBar_GetValue(playerid, barid), color);
}

stock SetPlayerProgressBarValue(playerid, PlayerBar:barid, Float:value)
{
	return PlayerBar_Update(playerid, barid, value, PlayerBar_GetColor(playerid, barid));
}

/*
native PlayerBar_Count(playerid);
*/
stock PlayerBar_Count(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	return iBarCount;
}

/*
native Bar_Create(Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, color = 0xFF1C1CFF, Float:max = 100.0, direction = BAR_DIRECTION_RIGHT, bgcolor = 0x000000FF);
*/
stock Bar:Bar_Create(Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, color = 0xFF1C1CFF, Float:max = 100.0, direction = BAR_DIRECTION_RIGHT, bgcolor = 0x000000FF)
{
	if (iBarCount == MAX_BARS)
	{
	    return INVALID_BAR_ID;
	}

	new Bar:barid = Bar:iBarCount++;

	eBar[barid][e_BAR_DIRECTION] = direction;
	eBar[barid][e_BAR_X] = x;
	eBar[barid][e_BAR_Y] = y;
	eBar[barid][e_BAR_WIDTH] = width;
	eBar[barid][e_BAR_HEIGHT] = height;
	eBar[barid][e_BAR_COLOR] = color;
	eBar[barid][e_BAR_BACKGROUND_COLOR] = bgcolor;
	eBar[barid][e_BAR_VALUE] = 0.0;
	eBar[barid][e_BAR_MAX_VALUE] = max;

	for (new i; i < MAX_PLAYERS; i++)
	{
	    bBarShownForPlayer[barid][i] = false;
	}

	PB_CreateBarBackground(barid);
	PB_CreateBarFill(barid);
	PB_CreateBarMain(barid);
	return barid;
}

/*
native Bar_IsValid(Bar:barid);
*/
stock Bar_IsValid(Bar:barid)
{
	if (barid < Bar:0 || barid >= Bar:iBarCount)
	{
	    return 0;
	}

	return 1;
}

/*
native Bar_Destroy(Bar:barid);
*/
stock Bar_Destroy(Bar:barid)
{
	if (!Bar_IsValid(barid))
	{
	    return 0;
	}

	TextDrawDestroy(txtBar[barid][BAR_TEXTDRAW_BACKGROUND]);
	TextDrawDestroy(txtBar[barid][BAR_TEXTDRAW_FILL]);
	TextDrawDestroy(txtBar[barid][BAR_TEXTDRAW_MAIN]);

	for (new Bar:i = barid; i < Bar:(iBarCount - 1); i++)
	{
		eBar[i][e_BAR_DIRECTION] = eBar[(i + Bar:1)][e_BAR_DIRECTION];
		eBar[i][e_BAR_X] = eBar[(i + Bar:1)][e_BAR_X];
		eBar[i][e_BAR_Y] = eBar[(i + Bar:1)][e_BAR_Y];
		eBar[i][e_BAR_WIDTH] = eBar[(i + Bar:1)][e_BAR_WIDTH];
		eBar[i][e_BAR_HEIGHT] = eBar[(i + Bar:1)][e_BAR_HEIGHT];
		eBar[i][e_BAR_COLOR] = eBar[(i + Bar:1)][e_BAR_COLOR];
		eBar[i][e_BAR_BACKGROUND_COLOR] = eBar[(i + Bar:1)][e_BAR_BACKGROUND_COLOR];
		eBar[i][e_BAR_VALUE] = eBar[(i + Bar:1)][e_BAR_VALUE];
		eBar[i][e_BAR_MAX_VALUE] = eBar[(i + Bar:1)][e_BAR_MAX_VALUE];

		for (new x; x < MAX_PLAYERS; x++)
		{
		    bBarShownForPlayer[i][x] = bBarShownForPlayer[(i + Bar:1)][x];
		}

		txtBar[i][BAR_TEXTDRAW_BACKGROUND] = txtBar[(i + Bar:1)][BAR_TEXTDRAW_BACKGROUND];
		txtBar[i][BAR_TEXTDRAW_FILL] = txtBar[(i + Bar:1)][BAR_TEXTDRAW_FILL];
		txtBar[i][BAR_TEXTDRAW_MAIN] = txtBar[(i + Bar:1)][BAR_TEXTDRAW_MAIN];
	}

	iBarCount--;
	return 1;
}

/*
native Bar_Show(playerid, Bar:barid);
*/
stock Bar_Show(playerid, Bar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!Bar_IsValid(barid))
	{
	    return 0;
	}

	TextDrawShowForPlayer(playerid, txtBar[barid][BAR_TEXTDRAW_BACKGROUND]);
	TextDrawShowForPlayer(playerid, txtBar[barid][BAR_TEXTDRAW_FILL]);
	TextDrawShowForPlayer(playerid, txtBar[barid][BAR_TEXTDRAW_MAIN]);

	bBarShownForPlayer[barid][playerid] = true;
	return 1;
}

/*
native Bar_Hide(playerid, Bar:barid);
*/
stock Bar_Hide(playerid, Bar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!Bar_IsValid(barid))
	{
	    return 0;
	}

	TextDrawHideForPlayer(playerid, txtBar[barid][BAR_TEXTDRAW_BACKGROUND]);
	TextDrawHideForPlayer(playerid, txtBar[barid][BAR_TEXTDRAW_FILL]);
	TextDrawHideForPlayer(playerid, txtBar[barid][BAR_TEXTDRAW_MAIN]);

	bBarShownForPlayer[barid][playerid] = false;
	return 1;
}

/*
native Bar_GetColor(Bar:barid);
*/
stock Bar_GetColor(Bar:barid)
{
	if (!Bar_IsValid(barid))
	{
	    return 0;
	}

	return eBar[barid][e_BAR_COLOR];
}

#define GetProgressBarColor \
   	Bar_GetColor

/*
native Float:Bar_GetValue(Bar:barid);
*/
stock Float:Bar_GetValue(Bar:barid)
{
	if (!Bar_IsValid(barid))
	{
	    return 0.0;
	}

	return eBar[barid][e_BAR_VALUE];
}

#define GetProgressBarValue \
    Bar_GetValue
    
/*
native Bar_Update(Bar:barid, Float:value, color);
*/
stock Bar_Update(Bar:barid, Float:value, color)
{
	if (!Bar_IsValid(barid))
	{
	    return 0;
	}

	if (value != eBar[barid][e_BAR_VALUE])
	{
		eBar[barid][e_BAR_VALUE] = ((value >= eBar[barid][e_BAR_MAX_VALUE]) ? (eBar[barid][e_BAR_MAX_VALUE]) : (value));

		TextDrawDestroy(txtBar[barid][BAR_TEXTDRAW_FILL]);
		PB_CreateBarFill(barid);
		TextDrawDestroy(txtBar[barid][BAR_TEXTDRAW_MAIN]);
		PB_CreateBarMain(barid);

		for (new i, j = GetPlayerPoolSize(); i <= j; i++)
		{
		    if (IsPlayerConnected(i) && bBarShownForPlayer[barid][i])
			{
				TextDrawShowForPlayer(i, txtBar[barid][BAR_TEXTDRAW_FILL]);
				TextDrawShowForPlayer(i, txtBar[barid][BAR_TEXTDRAW_MAIN]);
			}
		}
		return 1;
	}

	if (color != eBar[barid][e_BAR_COLOR])
	{
		eBar[barid][e_BAR_COLOR] = color;

		TextDrawBoxColor(txtBar[barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		TextDrawBoxColor(txtBar[barid][BAR_TEXTDRAW_MAIN], color);

		for (new i, j = GetPlayerPoolSize(); i <= j; i++)
		{
		    if (IsPlayerConnected(i) && bBarShownForPlayer[barid][i])
			{
				TextDrawShowForPlayer(i, txtBar[barid][BAR_TEXTDRAW_FILL]);
				TextDrawShowForPlayer(i, txtBar[barid][BAR_TEXTDRAW_MAIN]);
			}
		}
	}
	return 1;
}

stock SetProgressBarColour(Bar:barid, color)
{
	return Bar_Update(barid, Bar_GetValue(barid), color);
}

stock SetProgressBarValue(Bar:barid, Float:value)
{
	return Bar_Update(barid, value, Bar_GetColor(barid));
}

/*
native Bar_Count();
*/
stock Bar_Count()
{
	return iBarCount;
}
