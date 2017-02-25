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

enum PB_e_BAR
{
	PB_e_BAR_DIRECTION,
	Float:PB_e_BAR_X,
	Float:PB_e_BAR_Y,
	Float:PB_e_BAR_WIDTH,
	Float:PB_e_BAR_HEIGHT,
	PB_e_BAR_COLOR,
	PB_e_BAR_BACKGROUND_COLOR,
	Float:PB_e_BAR_VALUE,
	Float:PB_e_BAR_MAX_VALUE
};

static PB_eBar[Bar:MAX_BARS][PB_e_BAR];
static Text:PB_txtBar[Bar:MAX_BARS][3];
static bool:PB_bBarShownForPlayer[Bar:MAX_BARS][MAX_PLAYERS];
static PB_iBarCount;

static PB_ePlayerBar[MAX_PLAYERS][PlayerBar:MAX_PLAYER_BARS][PB_e_BAR];
static PlayerText:PB_ptxtBar[MAX_PLAYERS][PlayerBar:MAX_PLAYER_BARS][3];
static bool:PB_bPlayerBarShown[MAX_PLAYERS][PlayerBar:MAX_PLAYER_BARS];
static PB_iPlayerBarCount[MAX_PLAYERS];

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

stock static PB_CreatPB_eBarBackground(Bar:barid)
{
	new direction = PB_eBar[barid][PB_e_BAR_DIRECTION],
		Float:x = PB_eBar[barid][PB_e_BAR_X],
	    Float:y = PB_eBar[barid][PB_e_BAR_Y],
	    Float:width = PB_eBar[barid][PB_e_BAR_WIDTH],
	    Float:height = PB_eBar[barid][PB_e_BAR_HEIGHT],
	    bgcolor = PB_eBar[barid][PB_e_BAR_BACKGROUND_COLOR];

    switch (direction)
	{
		case BAR_DIRECTION_RIGHT:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND] = TextDrawCreate(x, y, "_");
			TextDrawUseBox(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1);
			TextDrawTextSize(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], ((x + width) - 4.0), 0.0);
			TextDrawLetterSize(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1.0, (height / 10));
			TextDrawBoxColor(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}

		case BAR_DIRECTION_LEFT:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND] = TextDrawCreate(x, y, "_");
			TextDrawUseBox(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1);
			TextDrawTextSize(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], ((x - width) - 4.0), 0.0);
			TextDrawLetterSize(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1.0, (height / 10));
			TextDrawBoxColor(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}

		case BAR_DIRECTION_UP:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND] = TextDrawCreate(x, y, "_");
			TextDrawUseBox(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1);
			TextDrawTextSize(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], ((x - width) - 4.0), 0.0);
			TextDrawLetterSize(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1.0, (-((height / 10) * 1.02) - 0.35));
			TextDrawBoxColor(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}

		case BAR_DIRECTION_DOWN:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND] = TextDrawCreate(x, y, "_");
			TextDrawUseBox(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1);
			TextDrawTextSize(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], ((x - width) - 4.0), 0.0);
			TextDrawLetterSize(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1.0, ((height / 10) - 0.35));
			TextDrawBoxColor(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}
	}
}

stock static PB_CreatPB_eBarFill(Bar:barid)
{
	new direction = PB_eBar[barid][PB_e_BAR_DIRECTION],
		Float:x = PB_eBar[barid][PB_e_BAR_X],
	    Float:y = PB_eBar[barid][PB_e_BAR_Y],
	    Float:width = PB_eBar[barid][PB_e_BAR_WIDTH],
	    Float:height = PB_eBar[barid][PB_e_BAR_HEIGHT],
	    color = PB_eBar[barid][PB_e_BAR_COLOR];

    switch (direction)
	{
		case BAR_DIRECTION_RIGHT:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_FILL] = TextDrawCreate((x + 1.2), (y + 2.15), "_");
			TextDrawUseBox(PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1);
			TextDrawTextSize(PB_txtBar[barid][BAR_TEXTDRAW_FILL], ((x + width) - 5.5), 0.0);
			TextDrawLetterSize(PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1.0, ((height / 10) - 0.35));
			TextDrawBoxColor(PB_txtBar[barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}

		case BAR_DIRECTION_LEFT:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_FILL] = TextDrawCreate((x - 1.2), (y + 2.15), "_");
			TextDrawUseBox(PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1);
			TextDrawTextSize(PB_txtBar[barid][BAR_TEXTDRAW_FILL], ((x - width) - 2.5), 0.0);
			TextDrawLetterSize(PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1.0, ((height / 10) - 0.35));
			TextDrawBoxColor(PB_txtBar[barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}

		case BAR_DIRECTION_UP:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_FILL] = TextDrawCreate((x - 1.2), (y - 1.0), "_");
			TextDrawUseBox(PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1);
			TextDrawTextSize(PB_txtBar[barid][BAR_TEXTDRAW_FILL], ((x - width) - 2.5), 0.0);
			TextDrawLetterSize(PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1.0, -((height / 10) - 1.02));
			TextDrawBoxColor(PB_txtBar[barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}

		case BAR_DIRECTION_DOWN:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_FILL] = TextDrawCreate((x - 1.2), (y + 1.0), "_");
			TextDrawUseBox(PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1);
			TextDrawTextSize(PB_txtBar[barid][BAR_TEXTDRAW_FILL], ((x - width) - 2.5), 0.0);
			TextDrawLetterSize(PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1.0, ((height / 10) - 0.55));
			TextDrawBoxColor(PB_txtBar[barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}
	}
}

stock static PB_CreatPB_eBarMain(Bar:barid)
{
	new direction = PB_eBar[barid][PB_e_BAR_DIRECTION],
		Float:x = PB_eBar[barid][PB_e_BAR_X],
	    Float:y = PB_eBar[barid][PB_e_BAR_Y],
	    Float:width = PB_eBar[barid][PB_e_BAR_WIDTH],
	    Float:height = PB_eBar[barid][PB_e_BAR_HEIGHT],
	    Float:value = PB_eBar[barid][PB_e_BAR_VALUE],
	    Float:max_value = PB_eBar[barid][PB_e_BAR_MAX_VALUE],
	    color = PB_eBar[barid][PB_e_BAR_COLOR];

	switch (direction)
	{
		case BAR_DIRECTION_RIGHT:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_MAIN] = TextDrawCreate((x + 1.2), (y + 2.15), "_");
			TextDrawUseBox(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1);
			TextDrawTextSize(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], PB_GetBarPercent(x, width, max_value, value, direction), 0.0);
			TextDrawLetterSize(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1.0, ((height / 10) - 0.35));
			TextDrawBoxColor(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], color);
		}

		case BAR_DIRECTION_LEFT:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_MAIN] = TextDrawCreate((x - 1.2), (y + 2.15), "_");
			TextDrawUseBox(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1);
			TextDrawTextSize(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], PB_GetBarPercent(x, width, max_value, value, direction), 0.0);
			TextDrawLetterSize(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1.0, ((height / 10) - 0.35));
			TextDrawBoxColor(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], color);
		}

		case BAR_DIRECTION_UP:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_MAIN] = TextDrawCreate((x - 1.2), (y - 1.0), "_");
			TextDrawUseBox(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1);
			TextDrawTextSize(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], ((x - width) - 2.5), 0.0);
			TextDrawLetterSize(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1.0, PB_GetBarPercent(x, width, max_value, value, direction));
			TextDrawBoxColor(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], color);
		}

		case BAR_DIRECTION_DOWN:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_MAIN] = TextDrawCreate((x - 1.2), (y + 1.0), "_");
			TextDrawUseBox(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1);
			TextDrawTextSize(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], ((x - width) - 2.5), 0.0);
			TextDrawLetterSize(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1.0, PB_GetBarPercent(x, width, max_value, value, direction));
			TextDrawBoxColor(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], color);
		}
	}
}

stock static PB_CreatPB_ePlayerBarBackground(playerid, PlayerBar:barid)
{
	new direction = PB_ePlayerBar[playerid][barid][PB_e_BAR_DIRECTION],
		Float:x = PB_ePlayerBar[playerid][barid][PB_e_BAR_X],
	    Float:y = PB_ePlayerBar[playerid][barid][PB_e_BAR_Y],
	    Float:width = PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH],
	    Float:height = PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT],
	    bgcolor = PB_ePlayerBar[playerid][barid][PB_e_BAR_BACKGROUND_COLOR];

    switch (direction)
	{
		case BAR_DIRECTION_RIGHT:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = CreatePlayerTextDraw(playerid, x, y, "_");
			PlayerTextDrawUseBox(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1);
			PlayerTextDrawTextSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], ((x + width) - 4.0), 0.0);
			PlayerTextDrawLetterSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1.0, (height / 10));
			PlayerTextDrawBoxColor(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}

		case BAR_DIRECTION_LEFT:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = CreatePlayerTextDraw(playerid, x, y, "_");
			PlayerTextDrawUseBox(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1);
			PlayerTextDrawTextSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], ((x - width) - 4.0), 0.0);
			PlayerTextDrawLetterSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1.0, (height / 10));
			PlayerTextDrawBoxColor(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}

		case BAR_DIRECTION_UP:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = CreatePlayerTextDraw(playerid, x, y, "_");
			PlayerTextDrawUseBox(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1);
			PlayerTextDrawTextSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], ((x - width) - 4.0), 0.0);
			PlayerTextDrawLetterSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1.0, (-((height / 10) * 1.02) - 0.35));
			PlayerTextDrawBoxColor(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}

		case BAR_DIRECTION_DOWN:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = CreatePlayerTextDraw(playerid, x, y, "_");
			PlayerTextDrawUseBox(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1);
			PlayerTextDrawTextSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], ((x - width) - 4.0), 0.0);
			PlayerTextDrawLetterSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1.0, ((height / 10) - 0.35));
			PlayerTextDrawBoxColor(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], bgcolor);
		}
	}
}

stock static PB_CreatPB_ePlayerBarFill(playerid, PlayerBar:barid)
{
	new direction = PB_ePlayerBar[playerid][barid][PB_e_BAR_DIRECTION],
		Float:x = PB_ePlayerBar[playerid][barid][PB_e_BAR_X],
	    Float:y = PB_ePlayerBar[playerid][barid][PB_e_BAR_Y],
	    Float:width = PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH],
	    Float:height = PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT],
	    color = PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR];

    switch (direction)
	{
		case BAR_DIRECTION_RIGHT:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL] = CreatePlayerTextDraw(playerid, (x + 1.2), (y + 2.15), "_");
			PlayerTextDrawUseBox(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1);
			PlayerTextDrawTextSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((x + width) - 5.5), 0.0);
			PlayerTextDrawLetterSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1.0, ((height / 10) - 0.35));
			PlayerTextDrawBoxColor(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}

		case BAR_DIRECTION_LEFT:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL] = CreatePlayerTextDraw(playerid, (x - 1.2), (y + 2.15), "_");
			PlayerTextDrawUseBox(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1);
			PlayerTextDrawTextSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((x - width) - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1.0, ((height / 10) - 0.35));
			PlayerTextDrawBoxColor(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}

		case BAR_DIRECTION_UP:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL] = CreatePlayerTextDraw(playerid, (x - 1.2), (y - 1.0), "_");
			PlayerTextDrawUseBox(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1);
			PlayerTextDrawTextSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((x - width) - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1.0, -((height / 10) - 1.02));
			PlayerTextDrawBoxColor(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}

		case BAR_DIRECTION_DOWN:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL] = CreatePlayerTextDraw(playerid, (x - 1.2), (y + 1.0), "_");
			PlayerTextDrawUseBox(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1);
			PlayerTextDrawTextSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((x - width) - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1.0, ((height / 10) - 0.55));
			PlayerTextDrawBoxColor(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		}
	}
}

stock static PB_CreatPB_ePlayerBarMain(playerid, PlayerBar:barid)
{
	new direction = PB_ePlayerBar[playerid][barid][PB_e_BAR_DIRECTION],
		Float:x = PB_ePlayerBar[playerid][barid][PB_e_BAR_X],
	    Float:y = PB_ePlayerBar[playerid][barid][PB_e_BAR_Y],
	    Float:width = PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH],
	    Float:height = PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT],
	    Float:value = PB_ePlayerBar[playerid][barid][PB_e_BAR_VALUE],
	    Float:max_value = PB_ePlayerBar[playerid][barid][PB_e_BAR_MAX_VALUE],
	    color = PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR];

	switch (direction)
	{
		case BAR_DIRECTION_RIGHT:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN] = CreatePlayerTextDraw(playerid, (x + 1.2), (y + 2.15), "_");
			PlayerTextDrawUseBox(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1);
			PlayerTextDrawTextSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], PB_GetBarPercent(x, width, max_value, value, direction), 0.0);
			PlayerTextDrawLetterSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1.0, ((height / 10) - 0.35));
			PlayerTextDrawBoxColor(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], color);
		}

		case BAR_DIRECTION_LEFT:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN] = CreatePlayerTextDraw(playerid, (x - 1.2), (y + 2.15), "_");
			PlayerTextDrawUseBox(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1);
			PlayerTextDrawTextSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], PB_GetBarPercent(x, width, max_value, value, direction), 0.0);
			PlayerTextDrawLetterSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1.0, ((height / 10) - 0.35));
			PlayerTextDrawBoxColor(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], color);
		}

		case BAR_DIRECTION_UP:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN] = CreatePlayerTextDraw(playerid, (x - 1.2), (y - 1.0), "_");
			PlayerTextDrawUseBox(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1);
			PlayerTextDrawTextSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], ((x - width) - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1.0, PB_GetBarPercent(x, width, max_value, value, direction));
			PlayerTextDrawBoxColor(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], color);
		}

		case BAR_DIRECTION_DOWN:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN] = CreatePlayerTextDraw(playerid, (x - 1.2), (y + 1.0), "_");
			PlayerTextDrawUseBox(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1);
			PlayerTextDrawTextSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], ((x - width) - 2.5), 0.0);
			PlayerTextDrawLetterSize(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1.0, PB_GetBarPercent(x, width, max_value, value, direction));
			PlayerTextDrawBoxColor(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], color);
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

	if (PB_iPlayerBarCount[playerid] == MAX_BARS)
	{
	    return INVALID_PLAYER_BAR_ID;
	}

	new PlayerBar:barid = PlayerBar:PB_iPlayerBarCount[playerid]++;

	PB_ePlayerBar[playerid][barid][PB_e_BAR_DIRECTION] = direction;
	PB_ePlayerBar[playerid][barid][PB_e_BAR_X] = x;
	PB_ePlayerBar[playerid][barid][PB_e_BAR_Y] = y;
	PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH] = width;
	PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT] = height;
	PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR] = color;
	PB_ePlayerBar[playerid][barid][PB_e_BAR_BACKGROUND_COLOR] = bgcolor;
	PB_ePlayerBar[playerid][barid][PB_e_BAR_VALUE] = 0.0;
	PB_ePlayerBar[playerid][barid][PB_e_BAR_MAX_VALUE] = max;

    PB_bPlayerBarShown[playerid][barid] = false;

	PB_CreatPB_ePlayerBarBackground(playerid, barid);
	PB_CreatPB_ePlayerBarFill(playerid, barid);
	PB_CreatPB_ePlayerBarMain(playerid, barid);
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

	if (barid < PlayerBar:0 || barid >= PlayerBar:PB_iPlayerBarCount[playerid])
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

	PlayerTextDrawDestroy(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND]);
	PlayerTextDrawDestroy(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
	PlayerTextDrawDestroy(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);

	for (new PlayerBar:i = barid; i < (PlayerBar:(PB_iPlayerBarCount[playerid] - 1)); i++)
	{
		PB_ePlayerBar[playerid][i][PB_e_BAR_DIRECTION] = PB_ePlayerBar[playerid][(i + PlayerBar:1)][PB_e_BAR_DIRECTION];
		PB_ePlayerBar[playerid][i][PB_e_BAR_X] = PB_ePlayerBar[playerid][(i + PlayerBar:1)][PB_e_BAR_X];
		PB_ePlayerBar[playerid][i][PB_e_BAR_Y] = PB_ePlayerBar[playerid][(i + PlayerBar:1)][PB_e_BAR_Y];
		PB_ePlayerBar[playerid][i][PB_e_BAR_WIDTH] = PB_ePlayerBar[playerid][(i + PlayerBar:1)][PB_e_BAR_WIDTH];
		PB_ePlayerBar[playerid][i][PB_e_BAR_HEIGHT] = PB_ePlayerBar[playerid][(i + PlayerBar:1)][PB_e_BAR_HEIGHT];
		PB_ePlayerBar[playerid][i][PB_e_BAR_COLOR] = PB_ePlayerBar[playerid][(i + PlayerBar:1)][PB_e_BAR_COLOR];
		PB_ePlayerBar[playerid][i][PB_e_BAR_BACKGROUND_COLOR] = PB_ePlayerBar[playerid][(i + PlayerBar:1)][PB_e_BAR_BACKGROUND_COLOR];
		PB_ePlayerBar[playerid][i][PB_e_BAR_VALUE] = PB_ePlayerBar[playerid][(i + PlayerBar:1)][PB_e_BAR_VALUE];
		PB_ePlayerBar[playerid][i][PB_e_BAR_MAX_VALUE] = PB_ePlayerBar[playerid][(i + PlayerBar:1)][PB_e_BAR_MAX_VALUE];

    	PB_bPlayerBarShown[playerid][i] = PB_bPlayerBarShown[playerid][(i + PlayerBar:1)];

		PB_ptxtBar[playerid][i][BAR_TEXTDRAW_BACKGROUND] = PB_ptxtBar[playerid][(i + PlayerBar:1)][BAR_TEXTDRAW_BACKGROUND];
		PB_ptxtBar[playerid][i][BAR_TEXTDRAW_FILL] = PB_ptxtBar[playerid][(i + PlayerBar:1)][BAR_TEXTDRAW_FILL];
		PB_ptxtBar[playerid][i][BAR_TEXTDRAW_MAIN] = PB_ptxtBar[playerid][(i + PlayerBar:1)][BAR_TEXTDRAW_MAIN];
	}

	PB_iPlayerBarCount[playerid]--;
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

	PlayerTextDrawShow(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND]);
	PlayerTextDrawShow(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
	PlayerTextDrawShow(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);

	PB_bPlayerBarShown[playerid][barid] = true;
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

	PlayerTextDrawHide(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND]);
	PlayerTextDrawHide(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
	PlayerTextDrawHide(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);

	PB_bPlayerBarShown[playerid][barid] = false;
	return 1;
}

#define HidePlayerProgressBar \
    PlayerBar_Hide

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

	if (value != PB_ePlayerBar[playerid][barid][PB_e_BAR_VALUE])
	{
		PB_ePlayerBar[playerid][barid][PB_e_BAR_VALUE] = ((value >= PB_ePlayerBar[playerid][barid][PB_e_BAR_MAX_VALUE]) ? (PB_ePlayerBar[playerid][barid][PB_e_BAR_MAX_VALUE]) : (value));

		PlayerTextDrawDestroy(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
		PB_CreatPB_ePlayerBarFill(playerid, barid);
		PlayerTextDrawDestroy(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);
		PB_CreatPB_ePlayerBarMain(playerid, barid);

		if (PB_bPlayerBarShown[playerid][barid])
		{
			PlayerTextDrawShow(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
			PlayerTextDrawShow(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);
		}
		return 1;
	}

	if (color != PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR])
	{
		PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR] = color;

		PlayerTextDrawBoxColor(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		PlayerTextDrawBoxColor(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], color);

		if (PB_bPlayerBarShown[playerid][barid])
		{
			PlayerTextDrawShow(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
			PlayerTextDrawShow(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);
		}
	}
	return 1;
}

#define SetPlayerProgressBarColour(%1,%2,%3) \
    PlayerBar_Update(%1, %2, PB_ePlayerBar[%1][%2][PB_e_BAR_VALUE], %3)

#define SetPlayerProgressBarValue(%1,%2,%3) \
    PlayerBar_Update(%1, %2, %3, PB_ePlayerBar[%1][%2][PB_e_BAR_COLOR])

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

	return PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR];
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

	return PB_ePlayerBar[playerid][barid][PB_e_BAR_VALUE];
}

#define GetPlayerProgressBarValue \
    PlayerBar_GetValue

/*
native PlayerBar_Count(playerid);
*/
stock PlayerBar_Count(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	return PB_iBarCount;
}

/*
native Bar_Create(Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, color = 0xFF1C1CFF, Float:max = 100.0, direction = BAR_DIRECTION_RIGHT, bgcolor = 0x000000FF);
*/
stock Bar:Bar_Create(Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, color = 0xFF1C1CFF, Float:max = 100.0, direction = BAR_DIRECTION_RIGHT, bgcolor = 0x000000FF)
{
	if (PB_iBarCount == MAX_BARS)
	{
	    return INVALID_BAR_ID;
	}

	new Bar:barid = Bar:PB_iBarCount++;

	PB_eBar[barid][PB_e_BAR_DIRECTION] = direction;
	PB_eBar[barid][PB_e_BAR_X] = x;
	PB_eBar[barid][PB_e_BAR_Y] = y;
	PB_eBar[barid][PB_e_BAR_WIDTH] = width;
	PB_eBar[barid][PB_e_BAR_HEIGHT] = height;
	PB_eBar[barid][PB_e_BAR_COLOR] = color;
	PB_eBar[barid][PB_e_BAR_BACKGROUND_COLOR] = bgcolor;
	PB_eBar[barid][PB_e_BAR_VALUE] = 0.0;
	PB_eBar[barid][PB_e_BAR_MAX_VALUE] = max;

	for (new i; i < MAX_PLAYERS; i++)
	{
	    PB_bBarShownForPlayer[barid][i] = false;
	}

	PB_CreatPB_eBarBackground(barid);
	PB_CreatPB_eBarFill(barid);
	PB_CreatPB_eBarMain(barid);
	return barid;
}

/*
native Bar_IsValid(Bar:barid);
*/
stock Bar_IsValid(Bar:barid)
{
	if (barid < Bar:0 || barid >= Bar:PB_iBarCount)
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

	TextDrawDestroy(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND]);
	TextDrawDestroy(PB_txtBar[barid][BAR_TEXTDRAW_FILL]);
	TextDrawDestroy(PB_txtBar[barid][BAR_TEXTDRAW_MAIN]);

	for (new Bar:i = barid; i < Bar:(PB_iBarCount - 1); i++)
	{
		PB_eBar[i][PB_e_BAR_DIRECTION] = PB_eBar[(i + Bar:1)][PB_e_BAR_DIRECTION];
		PB_eBar[i][PB_e_BAR_X] = PB_eBar[(i + Bar:1)][PB_e_BAR_X];
		PB_eBar[i][PB_e_BAR_Y] = PB_eBar[(i + Bar:1)][PB_e_BAR_Y];
		PB_eBar[i][PB_e_BAR_WIDTH] = PB_eBar[(i + Bar:1)][PB_e_BAR_WIDTH];
		PB_eBar[i][PB_e_BAR_HEIGHT] = PB_eBar[(i + Bar:1)][PB_e_BAR_HEIGHT];
		PB_eBar[i][PB_e_BAR_COLOR] = PB_eBar[(i + Bar:1)][PB_e_BAR_COLOR];
		PB_eBar[i][PB_e_BAR_BACKGROUND_COLOR] = PB_eBar[(i + Bar:1)][PB_e_BAR_BACKGROUND_COLOR];
		PB_eBar[i][PB_e_BAR_VALUE] = PB_eBar[(i + Bar:1)][PB_e_BAR_VALUE];
		PB_eBar[i][PB_e_BAR_MAX_VALUE] = PB_eBar[(i + Bar:1)][PB_e_BAR_MAX_VALUE];

		for (new x; x < MAX_PLAYERS; x++)
		{
		    PB_bBarShownForPlayer[i][x] = PB_bBarShownForPlayer[(i + Bar:1)][x];
		}

		PB_txtBar[i][BAR_TEXTDRAW_BACKGROUND] = PB_txtBar[(i + Bar:1)][BAR_TEXTDRAW_BACKGROUND];
		PB_txtBar[i][BAR_TEXTDRAW_FILL] = PB_txtBar[(i + Bar:1)][BAR_TEXTDRAW_FILL];
		PB_txtBar[i][BAR_TEXTDRAW_MAIN] = PB_txtBar[(i + Bar:1)][BAR_TEXTDRAW_MAIN];
	}

	PB_iBarCount--;
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

	TextDrawShowForPlayer(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND]);
	TextDrawShowForPlayer(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL]);
	TextDrawShowForPlayer(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN]);

	PB_bBarShownForPlayer[barid][playerid] = true;
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

	TextDrawHideForPlayer(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND]);
	TextDrawHideForPlayer(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL]);
	TextDrawHideForPlayer(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN]);

	PB_bBarShownForPlayer[barid][playerid] = false;
	return 1;
}

/*
native Bar_Update(Bar:barid, Float:value, color);
*/
stock Bar_Update(Bar:barid, Float:value, color)
{
	if (!Bar_IsValid(barid))
	{
	    return 0;
	}

	if (value != PB_eBar[barid][PB_e_BAR_VALUE])
	{
		PB_eBar[barid][PB_e_BAR_VALUE] = ((value >= PB_eBar[barid][PB_e_BAR_MAX_VALUE]) ? (PB_eBar[barid][PB_e_BAR_MAX_VALUE]) : (value));

		TextDrawDestroy(PB_txtBar[barid][BAR_TEXTDRAW_FILL]);
		PB_CreatPB_eBarFill(barid);
		TextDrawDestroy(PB_txtBar[barid][BAR_TEXTDRAW_MAIN]);
		PB_CreatPB_eBarMain(barid);

		for (new i, j = GetPlayerPoolSize(); i <= j; i++)
		{
		    if (IsPlayerConnected(i) && PB_bBarShownForPlayer[barid][i])
			{
				TextDrawShowForPlayer(i, PB_txtBar[barid][BAR_TEXTDRAW_FILL]);
				TextDrawShowForPlayer(i, PB_txtBar[barid][BAR_TEXTDRAW_MAIN]);
			}
		}
		return 1;
	}

	if (color != PB_eBar[barid][PB_e_BAR_COLOR])
	{
		PB_eBar[barid][PB_e_BAR_COLOR] = color;

		TextDrawBoxColor(PB_txtBar[barid][BAR_TEXTDRAW_FILL], ((color & 0xFFFFFF00) | (0x66 & ((color & 0x000000FF) / 2))));
		TextDrawBoxColor(PB_txtBar[barid][BAR_TEXTDRAW_MAIN], color);

		for (new i, j = GetPlayerPoolSize(); i <= j; i++)
		{
		    if (IsPlayerConnected(i) && PB_bBarShownForPlayer[barid][i])
			{
				TextDrawShowForPlayer(i, PB_txtBar[barid][BAR_TEXTDRAW_FILL]);
				TextDrawShowForPlayer(i, PB_txtBar[barid][BAR_TEXTDRAW_MAIN]);
			}
		}
	}
	return 1;
}

#define SetProgressBarColour(%1,%2) \
    Bar_Update(%1, PB_ePlayerBar[%1][%2][PB_e_BAR_VALUE], %2)

#define SetProgressBarValue(%1,%2) \
    PlayerBar_Update(%1, %2, PB_ePlayerBar[%1][%2][PB_e_BAR_COLOR])

/*
native Bar_GetColor(Bar:barid);
*/
stock Bar_GetColor(Bar:barid)
{
	if (!Bar_IsValid(barid))
	{
	    return 0;
	}

	return PB_eBar[barid][PB_e_BAR_COLOR];
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

	return PB_eBar[barid][PB_e_BAR_VALUE];
}

#define GetProgressBarValue \
    Bar_GetValue

/*
native Bar_Count();
*/
stock Bar_Count()
{
	return PB_iBarCount;
}
