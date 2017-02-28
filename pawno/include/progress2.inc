#if defined _progress2_included
	#endinput
#endif
#define _progress2_included

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

static Float:PB_BarPercent(Float:x, Float:widthorheight, Float:max, Float:value, direction)
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

/******************************
*
*
*   PLAYER PROGRESS BARS
*
*
******************************/

/*
native PlayerBar: CreatePlayerProgressBar(playerid, Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, color = 0xFF1C1CFF, Float:max = 100.0, direction = BAR_DIRECTION_RIGHT, bgcolor = 0x000000FF);
native IsValidPlayerProgressBar(playerid, PlayerBar:barid);
native DestroyPlayerProgressBar(playerid, PlayerBar:barid);
native ShowPlayerProgressBar(playerid, PlayerBar:barid);
native HidePlayerProgressBar(playerid, PlayerBar:barid);
native GetPlayerProgressBarDirection(playerid, PlayerBar:barid);
native GetPlayerProgressBarPos(playerid, PlayerBar:barid, &Float:x, &Float:y);
native Float:GetPlayerProgressBarHeight(playerid, PlayerBar:barid);
native Float:GetPlayerProgressBarWidth(playerid, PlayerBar:barid);
native GetPlayerProgressBarBGColor(playerid, PlayerBar:barid);
native GetPlayerProgressBarColor(playerid, PlayerBar:barid);
native Float:GetPlayerProgressBarValue(playerid, PlayerBar:barid);
native Float:GetPlayerProgressBarMaxValue(playerid, PlayerBar:barid);
native SetPlayerProgressBarDirection(playerid, PlayerBar:barid);
native SetPlayerProgressBarPos(playerid, PlayerBar:barid, &Float:x, &Float:y);
native SetPlayerProgressBarHeight(playerid, PlayerBar:barid, Float:height);
native SetPlayerProgressBarWidth(playerid, PlayerBar:barid, Float:width);
native SetPlayerProgressBarBGColor(playerid, PlayerBar:barid, bgcolor);
native SetPlayerProgressBarColor(playerid, PlayerBar:barid, color);
native SetPlayerProgressBarValue(playerid, PlayerBar:barid, Float:value);
native SetPlayerProgressBarMaxValue(playerid, PlayerBar:barid, Float:maxvalue);
native CountPlayerProgressBars(playerid);
*/

#if !defined MAX_PLAYER_BARS
	#define MAX_PLAYER_BARS \
		(MAX_PLAYER_TEXT_DRAWS / 3)
#endif

#define INVALID_PLAYER_BAR_ID \
	(PlayerBar:-1)

static PB_ePlayerBar[MAX_PLAYERS][PlayerBar:MAX_PLAYER_BARS][PB_e_BAR];
static PlayerText:PB_ptxtBar[MAX_PLAYERS][PlayerBar:MAX_PLAYER_BARS][3];
static bool:PB_bPlayerBarShown[MAX_PLAYERS][PlayerBar:MAX_PLAYER_BARS];
static PB_iPlayerBarCount[MAX_PLAYERS];

static stock PB_RenderPlayerBar(playerid, PlayerBar:barid)
{
	PlayerTextDrawDestroy(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND]);
	PlayerTextDrawDestroy(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
	PlayerTextDrawDestroy(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);

	switch (PB_ePlayerBar[playerid][barid][PB_e_BAR_DIRECTION])
	{
		case BAR_DIRECTION_RIGHT:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = CreatePlayerTextDraw(playerid, PB_ePlayerBar[playerid][barid][PB_e_BAR_X], PB_ePlayerBar[playerid][barid][PB_e_BAR_Y], "_");
			PlayerTextDrawUseBox		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1);
			PlayerTextDrawTextSize		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], PB_ePlayerBar[playerid][barid][PB_e_BAR_X] + PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH] - 4.0, 0.0);
			PlayerTextDrawLetterSize	(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1.0, PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT] / 10);
			PlayerTextDrawBoxColor		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], PB_ePlayerBar[playerid][barid][PB_e_BAR_BACKGROUND_COLOR]);

			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL] = CreatePlayerTextDraw(playerid, PB_ePlayerBar[playerid][barid][PB_e_BAR_X] + 1.2, PB_ePlayerBar[playerid][barid][PB_e_BAR_Y] + 2.15, "_");
			PlayerTextDrawUseBox		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1);
			PlayerTextDrawTextSize		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], PB_ePlayerBar[playerid][barid][PB_e_BAR_X] + PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH] - 5.5, 0.0);
			PlayerTextDrawLetterSize	(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1.0, PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT] / 10 - 0.35);
			PlayerTextDrawBoxColor		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], (PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR] & 0xFFFFFF00) | (0x66 & ((PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR] & 0x000000FF) / 2)));

			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN] = CreatePlayerTextDraw(playerid, PB_ePlayerBar[playerid][barid][PB_e_BAR_X] + 1.2, PB_ePlayerBar[playerid][barid][PB_e_BAR_Y] + 2.15, "_");
			PlayerTextDrawUseBox		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1);
			PlayerTextDrawTextSize		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], PB_BarPercent(PB_ePlayerBar[playerid][barid][PB_e_BAR_X], PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH], PB_ePlayerBar[playerid][barid][PB_e_BAR_MAX_VALUE], PB_ePlayerBar[playerid][barid][PB_e_BAR_VALUE], PB_ePlayerBar[playerid][barid][PB_e_BAR_DIRECTION]), 0.0);
			PlayerTextDrawLetterSize	(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1.0, PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT] / 10 - 0.35);
			PlayerTextDrawBoxColor		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR]);
		}

		case BAR_DIRECTION_LEFT:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = CreatePlayerTextDraw(playerid, PB_ePlayerBar[playerid][barid][PB_e_BAR_X], PB_ePlayerBar[playerid][barid][PB_e_BAR_Y], "_");
			PlayerTextDrawUseBox		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1);
			PlayerTextDrawTextSize		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], PB_ePlayerBar[playerid][barid][PB_e_BAR_X] - PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH] - 4.0, 0.0);
			PlayerTextDrawLetterSize	(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1.0, PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT] / 10);
			PlayerTextDrawBoxColor		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], PB_ePlayerBar[playerid][barid][PB_e_BAR_BACKGROUND_COLOR]);

			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL] = CreatePlayerTextDraw(playerid, PB_ePlayerBar[playerid][barid][PB_e_BAR_X] - 1.2, PB_ePlayerBar[playerid][barid][PB_e_BAR_Y] + 2.15, "_");
			PlayerTextDrawUseBox		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1);
			PlayerTextDrawTextSize		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], PB_ePlayerBar[playerid][barid][PB_e_BAR_X] - PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH] - 2.5, 0.0);
			PlayerTextDrawLetterSize	(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1.0, PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT] / 10 - 0.35);
			PlayerTextDrawBoxColor		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], (PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR] & 0xFFFFFF00) | (0x66 & ((PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR] & 0x000000FF) / 2)));

			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN] = CreatePlayerTextDraw(playerid, PB_ePlayerBar[playerid][barid][PB_e_BAR_X] - 1.2, PB_ePlayerBar[playerid][barid][PB_e_BAR_Y] + 2.15, "_");
			PlayerTextDrawUseBox		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1);
			PlayerTextDrawTextSize		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], PB_BarPercent(PB_ePlayerBar[playerid][barid][PB_e_BAR_X], PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH], PB_ePlayerBar[playerid][barid][PB_e_BAR_MAX_VALUE], PB_ePlayerBar[playerid][barid][PB_e_BAR_VALUE], PB_ePlayerBar[playerid][barid][PB_e_BAR_DIRECTION]), 0.0);
			PlayerTextDrawLetterSize	(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1.0, PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT] / 10 - 0.35);
			PlayerTextDrawBoxColor		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR]);
		}

		case BAR_DIRECTION_UP:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = CreatePlayerTextDraw(playerid, PB_ePlayerBar[playerid][barid][PB_e_BAR_X], PB_ePlayerBar[playerid][barid][PB_e_BAR_Y], "_");
			PlayerTextDrawUseBox		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1);
			PlayerTextDrawTextSize		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], PB_ePlayerBar[playerid][barid][PB_e_BAR_X] - PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH] - 4.0, 0.0);
			PlayerTextDrawLetterSize	(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1.0, -((PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT] / 10) * 1.02) -0.35);
			PlayerTextDrawBoxColor		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], PB_ePlayerBar[playerid][barid][PB_e_BAR_BACKGROUND_COLOR]);

			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL] = CreatePlayerTextDraw(playerid, PB_ePlayerBar[playerid][barid][PB_e_BAR_X] - 1.2, PB_ePlayerBar[playerid][barid][PB_e_BAR_Y] - 1.0, "_");
			PlayerTextDrawUseBox		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1);
			PlayerTextDrawTextSize		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], PB_ePlayerBar[playerid][barid][PB_e_BAR_X] - PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH] - 2.5, 0.0);
			PlayerTextDrawLetterSize	(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1.0, -(PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT] / 10.0) * 1.02);
			PlayerTextDrawBoxColor		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], (PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR] & 0xFFFFFF00) | (0x66 & ((PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR] & 0x000000FF) / 2)));

			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN] = CreatePlayerTextDraw(playerid, PB_ePlayerBar[playerid][barid][PB_e_BAR_X] - 1.2, PB_ePlayerBar[playerid][barid][PB_e_BAR_Y] - 1.0, "_");
			PlayerTextDrawUseBox		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1);
			PlayerTextDrawTextSize		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], PB_ePlayerBar[playerid][barid][PB_e_BAR_X] - PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH] - 2.5, 0.0);
			PlayerTextDrawLetterSize	(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1.0, PB_BarPercent(PB_ePlayerBar[playerid][barid][PB_e_BAR_X], PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT], PB_ePlayerBar[playerid][barid][PB_e_BAR_MAX_VALUE], PB_ePlayerBar[playerid][barid][PB_e_BAR_VALUE], PB_ePlayerBar[playerid][barid][PB_e_BAR_DIRECTION]));
			PlayerTextDrawBoxColor		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR]);
		}

		case BAR_DIRECTION_DOWN:
		{
			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = CreatePlayerTextDraw(playerid, PB_ePlayerBar[playerid][barid][PB_e_BAR_X], PB_ePlayerBar[playerid][barid][PB_e_BAR_Y], "_");
			PlayerTextDrawUseBox		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1);
			PlayerTextDrawTextSize		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], PB_ePlayerBar[playerid][barid][PB_e_BAR_X] - PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH] - 4.0, 0.0);
			PlayerTextDrawLetterSize	(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], 1.0, ((PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT] / 10)) -0.35);
			PlayerTextDrawBoxColor		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND], PB_ePlayerBar[playerid][barid][PB_e_BAR_BACKGROUND_COLOR]);

			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL] = CreatePlayerTextDraw(playerid, PB_ePlayerBar[playerid][barid][PB_e_BAR_X] - 1.2, PB_ePlayerBar[playerid][barid][PB_e_BAR_Y] + 1.0, "_");
			PlayerTextDrawUseBox		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1);
			PlayerTextDrawTextSize		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], PB_ePlayerBar[playerid][barid][PB_e_BAR_X] - PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH] - 2.5, 0.0);
			PlayerTextDrawLetterSize	(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], 1.0, (PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT] / 10.0) - 0.55);
			PlayerTextDrawBoxColor		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL], (PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR] & 0xFFFFFF00) | (0x66 & ((PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR] & 0x000000FF) / 2)));

			PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN] = CreatePlayerTextDraw(playerid, PB_ePlayerBar[playerid][barid][PB_e_BAR_X] - 1.2, PB_ePlayerBar[playerid][barid][PB_e_BAR_Y] + 1.0, "_");
			PlayerTextDrawUseBox		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1);
			PlayerTextDrawTextSize		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], PB_ePlayerBar[playerid][barid][PB_e_BAR_X] - PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH] - 2.5, 0.0);
			PlayerTextDrawLetterSize	(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], 1.0, PB_BarPercent(PB_ePlayerBar[playerid][barid][PB_e_BAR_X], PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT], PB_ePlayerBar[playerid][barid][PB_e_BAR_MAX_VALUE], PB_ePlayerBar[playerid][barid][PB_e_BAR_VALUE], PB_ePlayerBar[playerid][barid][PB_e_BAR_DIRECTION]));
			PlayerTextDrawBoxColor		(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN], PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR]);
		}
	}

	if (PB_bPlayerBarShown[playerid][barid])
	{
		PlayerTextDrawShow(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND]);
		PlayerTextDrawShow(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
		PlayerTextDrawShow(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);
	}
}

stock PlayerBar:CreatePlayerProgressBar(playerid, Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, color = 0xFF1C1CFF, Float:max = 100.0, direction = BAR_DIRECTION_RIGHT, bgcolor = 0x000000FF)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return INVALID_PLAYER_BAR_ID;
	}

	if (PB_iPlayerBarCount[playerid] == MAX_PLAYER_BARS)
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

	PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND] = PlayerText:INVALID_TEXT_DRAW;
	PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL] = PlayerText:INVALID_TEXT_DRAW;
	PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN] = PlayerText:INVALID_TEXT_DRAW;
	PB_RenderPlayerBar(playerid, barid);
	return barid;
}

stock IsValidPlayerProgressBar(playerid, PlayerBar:barid)
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

stock DestroyPlayerProgressBar(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
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

stock ShowPlayerProgressBar(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0;
	}

	PlayerTextDrawShow(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND]);
	PlayerTextDrawShow(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
	PlayerTextDrawShow(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);

	PB_bPlayerBarShown[playerid][barid] = true;
	return 1;
}

stock HidePlayerProgressBar(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0;
	}

	PlayerTextDrawHide(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_BACKGROUND]);
	PlayerTextDrawHide(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_FILL]);
	PlayerTextDrawHide(playerid, PB_ptxtBar[playerid][barid][BAR_TEXTDRAW_MAIN]);

	PB_bPlayerBarShown[playerid][barid] = false;
	return 1;
}

stock GetPlayerProgressBarDirection(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0;
	}

	return PB_ePlayerBar[playerid][barid][PB_e_BAR_DIRECTION];
}

stock GetPlayerProgressBarPos(playerid, PlayerBar:barid, &Float:x, &Float:y)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0;
	}

	x = PB_ePlayerBar[playerid][barid][PB_e_BAR_X];
	y = PB_ePlayerBar[playerid][barid][PB_e_BAR_X];
	return 1;
}

stock Float:GetPlayerProgressBarHeight(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0.0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0.0;
	}

	return PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT];
}

stock Float:GetPlayerProgressBarWidth(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0.0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0.0;
	}

	return PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH];
}

stock GetPlayerProgressBarBGColor(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0;
	}

	return PB_ePlayerBar[playerid][barid][PB_e_BAR_BACKGROUND_COLOR];
}

stock GetPlayerProgressBarColor(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0;
	}

	return PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR];
}

#define GetPlayerProgressBarColour \
    GetPlayerProgressBarColor

stock Float:GetPlayerProgressBarValue(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0.0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0.0;
	}

	return PB_ePlayerBar[playerid][barid][PB_e_BAR_VALUE];
}

stock Float:GetPlayerProgressBarMaxValue(playerid, PlayerBar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0.0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0.0;
	}

	return PB_ePlayerBar[playerid][barid][PB_e_BAR_MAX_VALUE];
}

stock SetPlayerProgressBarDirection(playerid, PlayerBar:barid, direction)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0;
	}

    PB_ePlayerBar[playerid][barid][PB_e_BAR_DIRECTION] = direction;
	PB_RenderPlayerBar(playerid, barid);
	return 1;
}

stock SetPlayerProgressBarPos(playerid, PlayerBar:barid, Float:x, Float:y)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0;
	}

	PB_ePlayerBar[playerid][barid][PB_e_BAR_X] = x;
	PB_ePlayerBar[playerid][barid][PB_e_BAR_X] = y;
	PB_RenderPlayerBar(playerid, barid);
	return 1;
}

stock SetPlayerProgressBarHeight(playerid, PlayerBar:barid, Float:height)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0;
	}

	PB_ePlayerBar[playerid][barid][PB_e_BAR_HEIGHT] = height;
	PB_RenderPlayerBar(playerid, barid);
	return 1;
}

stock SetPlayerProgressBarWidth(playerid, PlayerBar:barid, Float:width)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0;
	}

	PB_ePlayerBar[playerid][barid][PB_e_BAR_WIDTH] = width;
	PB_RenderPlayerBar(playerid, barid);
	return 1;
}

stock SetPlayerProgressBarBGColor(playerid, PlayerBar:barid, bgcolor)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0;
	}

    PB_ePlayerBar[playerid][barid][PB_e_BAR_BACKGROUND_COLOR] = bgcolor;
	PB_RenderPlayerBar(playerid, barid);
	return 1;
}

stock SetPlayerProgressBarColor(playerid, PlayerBar:barid, color)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0;
	}

    PB_ePlayerBar[playerid][barid][PB_e_BAR_COLOR] = color;
	PB_RenderPlayerBar(playerid, barid);
	return 1;
}

#define SetPlayerProgressBarColour \
    SetPlayerProgressBarColor

stock SetPlayerProgressBarValue(playerid, PlayerBar:barid, Float:value)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0;
	}

	PB_ePlayerBar[playerid][barid][PB_e_BAR_VALUE] = value;
	PB_RenderPlayerBar(playerid, barid);
	return 1;
}

stock SetPlayerProgressBarMaxValue(playerid, PlayerBar:barid, Float:maxvalue)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidPlayerProgressBar(playerid, barid))
	{
	    return 0;
	}

	PB_ePlayerBar[playerid][barid][PB_e_BAR_MAX_VALUE] = maxvalue;
	PB_RenderPlayerBar(playerid, barid);
	return 1;
}

stock CountPlayerProgressBars(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	return PB_iBarCount;
}

/******************************
*
*
*   GLOBAL PROGRESS BARS
*
*
******************************/

/*
native Bar: CreateProgressBar(Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, color = 0xFF1C1CFF, Float:max = 100.0, direction = BAR_DIRECTION_RIGHT, bgcolor = 0x000000FF);
native IsValidProgressBar(Bar:barid);
native DestroyProgressBar(Bar:barid);
native ShowProgressBarForPlayer(playerid, Bar:barid);
native HideProgressBarForPlayer(playerid, Bar:barid);
native ShowProgressBarForAll(Bar:barid);
native HideProgressBarForAll(Bar:barid);
native GetProgressBarDirection(Bar:barid);
native GetProgressBarPos(Bar:barid, &Float:x, &Float:y);
native Float:GetProgressBarHeight(Bar:barid);
native Float:GetProgressBarWidth(Bar:barid);
native GetProgressBarBGColor(Bar:barid);
native GetProgressBarColor(Bar:barid);
native Float:GetProgressBarValue(Bar:barid);
native Float:GetProgressBarMaxValue(Bar:barid);
native SetProgressBarDirection(Bar:barid);
native SetProgressBarPos(Bar:barid, Float:x, Float:y);
native SetProgressBarHeight(Bar:barid, Float:height);
native SetBarWidth(Bar:barid, Float:width);
native SetProgressBarBGColor(Bar:barid, bgcolor);
native SetProgressBarColor(Bar:barid, color);
native SetProgressBarValue(Bar:barid, Float:value);
native SetProgressBarMaxValue(Bar:barid, Float:maxvalue);
native CountProgressBars();
*/

#if !defined MAX_BARS
	#define MAX_BARS \
		(MAX_TEXT_DRAWS / 3)
#endif

#define INVALID_BAR_ID \
	(Bar:-1)

static PB_eBar[Bar:MAX_BARS][PB_e_BAR];
static Text:PB_txtBar[Bar:MAX_BARS][3];
static bool:PB_bBarShownForPlayer[Bar:MAX_BARS][MAX_PLAYERS];
static PB_iBarCount;

static stock PB_RenderBar(Bar:barid)
{
	TextDrawDestroy(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND]);
	TextDrawDestroy(PB_txtBar[barid][BAR_TEXTDRAW_FILL]);
	TextDrawDestroy(PB_txtBar[barid][BAR_TEXTDRAW_MAIN]);

	switch (PB_eBar[barid][PB_e_BAR_DIRECTION])
	{
		case BAR_DIRECTION_RIGHT:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND] = TextDrawCreate(playerid, PB_eBar[barid][PB_e_BAR_X], PB_eBar[barid][PB_e_BAR_Y], "_");
			TextDrawUseBox		(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1);
			TextDrawTextSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], PB_eBar[barid][PB_e_BAR_X] + PB_eBar[barid][PB_e_BAR_WIDTH] - 4.0, 0.0);
			TextDrawLetterSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1.0, PB_eBar[barid][PB_e_BAR_HEIGHT] / 10);
			TextDrawBoxColor	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], PB_eBar[barid][PB_e_BAR_BACKGROUND_COLOR]);

			PB_txtBar[barid][BAR_TEXTDRAW_FILL] = TextDrawCreate(playerid, PB_eBar[barid][PB_e_BAR_X] + 1.2, PB_eBar[barid][PB_e_BAR_Y] + 2.15, "_");
			TextDrawUseBox		(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1);
			TextDrawTextSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], PB_eBar[barid][PB_e_BAR_X] + PB_eBar[barid][PB_e_BAR_WIDTH] - 5.5, 0.0);
			TextDrawLetterSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1.0, PB_eBar[barid][PB_e_BAR_HEIGHT] / 10 - 0.35);
			TextDrawBoxColor	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], (PB_eBar[barid][PB_e_BAR_COLOR] & 0xFFFFFF00) | (0x66 & ((PB_eBar[barid][PB_e_BAR_COLOR] & 0x000000FF) / 2)));

			PB_txtBar[barid][BAR_TEXTDRAW_MAIN] = TextDrawCreate(playerid, PB_eBar[barid][PB_e_BAR_X] + 1.2, PB_eBar[barid][PB_e_BAR_Y] + 2.15, "_");
			TextDrawUseBox		(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1);
			TextDrawTextSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], PB_BarPercent(PB_eBar[barid][PB_e_BAR_X], PB_eBar[barid][PB_e_BAR_WIDTH], PB_eBar[barid][PB_e_BAR_MAX_VALUE], PB_eBar[barid][PB_e_BAR_VALUE], PB_eBar[barid][PB_e_BAR_DIRECTION]), 0.0);
			TextDrawLetterSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1.0, PB_eBar[barid][PB_e_BAR_HEIGHT] / 10 - 0.35);
			TextDrawBoxColor	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], PB_eBar[barid][PB_e_BAR_COLOR]);
		}

		case BAR_DIRECTION_LEFT:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND] = TextDrawCreate(playerid, PB_eBar[barid][PB_e_BAR_X], PB_eBar[barid][PB_e_BAR_Y], "_");
			TextDrawUseBox		(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1);
			TextDrawTextSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], PB_eBar[barid][PB_e_BAR_X] - PB_eBar[barid][PB_e_BAR_WIDTH] - 4.0, 0.0);
			TextDrawLetterSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1.0, PB_eBar[barid][PB_e_BAR_HEIGHT] / 10);
			TextDrawBoxColor	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], PB_eBar[barid][PB_e_BAR_BACKGROUND_COLOR]);

			PB_txtBar[barid][BAR_TEXTDRAW_FILL] = TextDrawCreate(playerid, PB_eBar[barid][PB_e_BAR_X] - 1.2, PB_eBar[barid][PB_e_BAR_Y] + 2.15, "_");
			TextDrawUseBox		(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1);
			TextDrawTextSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], PB_eBar[barid][PB_e_BAR_X] - PB_eBar[barid][PB_e_BAR_WIDTH] - 2.5, 0.0);
			TextDrawLetterSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1.0, PB_eBar[barid][PB_e_BAR_HEIGHT] / 10 - 0.35);
			TextDrawBoxColor	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], (PB_eBar[barid][PB_e_BAR_COLOR] & 0xFFFFFF00) | (0x66 & ((PB_eBar[barid][PB_e_BAR_COLOR] & 0x000000FF) / 2)));

			PB_txtBar[barid][BAR_TEXTDRAW_MAIN] = TextDrawCreate(playerid, PB_eBar[barid][PB_e_BAR_X] - 1.2, PB_eBar[barid][PB_e_BAR_Y] + 2.15, "_");
			TextDrawUseBox		(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1);
			TextDrawTextSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], PB_BarPercent(PB_eBar[barid][PB_e_BAR_X], PB_eBar[barid][PB_e_BAR_WIDTH], PB_eBar[barid][PB_e_BAR_MAX_VALUE], PB_eBar[barid][PB_e_BAR_VALUE], PB_eBar[barid][PB_e_BAR_DIRECTION]), 0.0);
			TextDrawLetterSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1.0, PB_eBar[barid][PB_e_BAR_HEIGHT] / 10 - 0.35);
			TextDrawBoxColor	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], PB_eBar[barid][PB_e_BAR_COLOR]);
		}

		case BAR_DIRECTION_UP:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND] = TextDrawCreate(playerid, PB_eBar[barid][PB_e_BAR_X], PB_eBar[barid][PB_e_BAR_Y], "_");
			TextDrawUseBox		(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1);
			TextDrawTextSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], PB_eBar[barid][PB_e_BAR_X] - PB_eBar[barid][PB_e_BAR_WIDTH] - 4.0, 0.0);
			TextDrawLetterSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1.0, -((PB_eBar[barid][PB_e_BAR_HEIGHT] / 10) * 1.02) -0.35);
			TextDrawBoxColor	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], PB_eBar[barid][PB_e_BAR_BACKGROUND_COLOR]);

			PB_txtBar[barid][BAR_TEXTDRAW_FILL] = TextDrawCreate(playerid, PB_eBar[barid][PB_e_BAR_X] - 1.2, PB_eBar[barid][PB_e_BAR_Y] - 1.0, "_");
			TextDrawUseBox		(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1);
			TextDrawTextSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], PB_eBar[barid][PB_e_BAR_X] - PB_eBar[barid][PB_e_BAR_WIDTH] - 2.5, 0.0);
			TextDrawLetterSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1.0, -(PB_eBar[barid][PB_e_BAR_HEIGHT] / 10.0) * 1.02);
			TextDrawBoxColor	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], (PB_eBar[barid][PB_e_BAR_COLOR] & 0xFFFFFF00) | (0x66 & ((PB_eBar[barid][PB_e_BAR_COLOR] & 0x000000FF) / 2)));

			PB_txtBar[barid][BAR_TEXTDRAW_MAIN] = TextDrawCreate(playerid, PB_eBar[barid][PB_e_BAR_X] - 1.2, PB_eBar[barid][PB_e_BAR_Y] - 1.0, "_");
			TextDrawUseBox		(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1);
			TextDrawTextSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], PB_eBar[barid][PB_e_BAR_X] - PB_eBar[barid][PB_e_BAR_WIDTH] - 2.5, 0.0);
			TextDrawLetterSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1.0, PB_BarPercent(PB_eBar[barid][PB_e_BAR_X], PB_eBar[barid][PB_e_BAR_HEIGHT], PB_eBar[barid][PB_e_BAR_MAX_VALUE], PB_eBar[barid][PB_e_BAR_VALUE], PB_eBar[barid][PB_e_BAR_DIRECTION]));
			TextDrawBoxColor	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], PB_eBar[barid][PB_e_BAR_COLOR]);
		}

		case BAR_DIRECTION_DOWN:
		{
			PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND] = TextDrawCreate(playerid, PB_eBar[barid][PB_e_BAR_X], PB_eBar[barid][PB_e_BAR_Y], "_");
			TextDrawUseBox		(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1);
			TextDrawTextSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], PB_eBar[barid][PB_e_BAR_X] - PB_eBar[barid][PB_e_BAR_WIDTH] - 4.0, 0.0);
			TextDrawLetterSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], 1.0, ((PB_eBar[barid][PB_e_BAR_HEIGHT] / 10)) -0.35);
			TextDrawBoxColor	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND], PB_eBar[barid][PB_e_BAR_BACKGROUND_COLOR]);

			PB_txtBar[barid][BAR_TEXTDRAW_FILL] = TextDrawCreate(playerid, PB_eBar[barid][PB_e_BAR_X] - 1.2, PB_eBar[barid][PB_e_BAR_Y] + 1.0, "_");
			TextDrawUseBox		(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1);
			TextDrawTextSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], PB_eBar[barid][PB_e_BAR_X] - PB_eBar[barid][PB_e_BAR_WIDTH] - 2.5, 0.0);
			TextDrawLetterSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], 1.0, (PB_eBar[barid][PB_e_BAR_HEIGHT] / 10.0) - 0.55);
			TextDrawBoxColor	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL], (PB_eBar[barid][PB_e_BAR_COLOR] & 0xFFFFFF00) | (0x66 & ((PB_eBar[barid][PB_e_BAR_COLOR] & 0x000000FF) / 2)));

			PB_txtBar[barid][BAR_TEXTDRAW_MAIN] = TextDrawCreate(playerid, PB_eBar[barid][PB_e_BAR_X] - 1.2, PB_eBar[barid][PB_e_BAR_Y] + 1.0, "_");
			TextDrawUseBox		(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1);
			TextDrawTextSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], PB_eBar[barid][PB_e_BAR_X] - PB_eBar[barid][PB_e_BAR_WIDTH] - 2.5, 0.0);
			TextDrawLetterSize	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], 1.0, PB_BarPercent(PB_eBar[barid][PB_e_BAR_X], PB_eBar[barid][PB_e_BAR_HEIGHT], PB_eBar[barid][PB_e_BAR_MAX_VALUE], PB_eBar[barid][PB_e_BAR_VALUE], PB_eBar[barid][PB_e_BAR_DIRECTION]));
			TextDrawBoxColor	(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN], PB_eBar[barid][PB_e_BAR_COLOR]);
		}
	}

	for (new i, j = GetPlayerPoolSize(); i <= j; i++)
	{
	    if (IsPlayerConnected(i))
	    {
		    if (PB_bBarShownForPlayer[barid][i])
		    {
				TextDrawShowForPlayer(i, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND]);
				TextDrawShowForPlayer(i, PB_txtBar[barid][BAR_TEXTDRAW_FILL]);
				TextDrawShowForPlayer(i, PB_txtBar[barid][BAR_TEXTDRAW_MAIN]);
			}
		}
	}
}

stock Bar:CreateProgressBar(Float:x, Float:y, Float:width = 55.5, Float:height = 3.2, color = 0xFF1C1CFF, Float:max = 100.0, direction = BAR_DIRECTION_RIGHT, bgcolor = 0x000000FF)
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

	PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND] = Text:INVALID_TEXT_DRAW;
	PB_txtBar[barid][BAR_TEXTDRAW_FILL] = Text:INVALID_TEXT_DRAW;
	PB_txtBar[barid][BAR_TEXTDRAW_MAIN] = Text:INVALID_TEXT_DRAW;
	PB_RenderBar(barid);
	return barid;
}

stock IsValidProgressBar(Bar:barid)
{
	if (barid < Bar:0 || barid >= Bar:PB_iBarCount)
	{
	    return 0;
	}

	return 1;
}

stock DestroyProgressBar(Bar:barid)
{
	if (!IsValidProgressBar(barid))
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

stock ShowProgressBarForPlayer(playerid, Bar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

	TextDrawShowForPlayer(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND]);
	TextDrawShowForPlayer(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL]);
	TextDrawShowForPlayer(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN]);

	PB_bBarShownForPlayer[barid][playerid] = true;
	return 1;
}

stock HideProgressBarForPlayer(playerid, Bar:barid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

	TextDrawHideForPlayer(playerid, PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND]);
	TextDrawHideForPlayer(playerid, PB_txtBar[barid][BAR_TEXTDRAW_FILL]);
	TextDrawHideForPlayer(playerid, PB_txtBar[barid][BAR_TEXTDRAW_MAIN]);

	PB_bBarShownForPlayer[barid][playerid] = false;
	return 1;
}

stock ShowProgressBarForAll(Bar:barid)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

	TextDrawShowForAll(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND]);
	TextDrawShowForAll(PB_txtBar[barid][BAR_TEXTDRAW_FILL]);
	TextDrawShowForAll(PB_txtBar[barid][BAR_TEXTDRAW_MAIN]);

	for (new i, j = GetPlayerPoolSize(); i <= j; i++)
	{
	    if (IsPlayerConnected(i))
	    {
			PB_bBarShownForPlayer[barid][i] = true;
		}
	}
	return 1;
}

stock HideProgressBarForAll(Bar:barid)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

	TextDrawHideForAll(PB_txtBar[barid][BAR_TEXTDRAW_BACKGROUND]);
	TextDrawHideForAll(PB_txtBar[barid][BAR_TEXTDRAW_FILL]);
	TextDrawHideForAll(PB_txtBar[barid][BAR_TEXTDRAW_MAIN]);

	for (new i, j = GetPlayerPoolSize(); i <= j; i++)
	{
	    if (IsPlayerConnected(i))
	    {
			PB_bBarShownForPlayer[barid][i] = false;
		}
	}
	return 1;
}

stock GetProgressBarDirection(Bar:barid)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

	return PB_eBar[barid][PB_e_BAR_DIRECTION];
}

stock GetProgressBarPos(Bar:barid, &Float:x, &Float:y)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

	x = PB_eBar[barid][PB_e_BAR_X];
	y = PB_eBar[barid][PB_e_BAR_X];
	return 1;
}

stock Float:GetProgressBarHeight(Bar:barid)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0.0;
	}

	return PB_eBar[barid][PB_e_BAR_HEIGHT];
}

stock Float:GetProgressBarWidth(Bar:barid)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0.0;
	}

	return PB_eBar[barid][PB_e_BAR_WIDTH];
}

stock GetProgressBarBGColor(Bar:barid)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

	return PB_eBar[barid][PB_e_BAR_BACKGROUND_COLOR];
}

stock GetProgressBarColor(Bar:barid)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

	return PB_eBar[barid][PB_e_BAR_COLOR];
}

#define GetProgressBarColour \
    GetProgressBarColor

stock Float:GetProgressBarValue(Bar:barid)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0.0;
	}

	return PB_eBar[barid][PB_e_BAR_VALUE];
}

stock Float:GetProgressBarMaxValue(Bar:barid)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0.0;
	}

	return PB_eBar[barid][PB_e_BAR_MAX_VALUE];
}

stock SetProgressBarDirection(Bar:barid, direction)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

    PB_eBar[barid][PB_e_BAR_DIRECTION] = direction;
	PB_RenderBar(barid);
	return 1;
}

stock SetProgressBarPos(Bar:barid, Float:x, Float:y)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

	PB_eBar[barid][PB_e_BAR_X] = x;
	PB_eBar[barid][PB_e_BAR_X] = y;
	PB_RenderBar(barid);
	return 1;
}

stock SetProgressBarHeight(Bar:barid, Float:height)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

	PB_eBar[barid][PB_e_BAR_HEIGHT] = height;
	PB_RenderBar(barid);
	return 1;
}

stock SetProgressBarWidth(Bar:barid, Float:width)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

	PB_eBar[barid][PB_e_BAR_WIDTH] = width;
	PB_RenderBar(barid);
	return 1;
}

stock SetProgressBarBGColor(Bar:barid, bgcolor)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

    PB_eBar[barid][PB_e_BAR_BACKGROUND_COLOR] = bgcolor;
	PB_RenderBar(barid);
	return 1;
}

stock SetProgressBarColor(Bar:barid, color)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

    PB_eBar[barid][PB_e_BAR_COLOR] = color;
	PB_RenderBar(barid);
	return 1;
}

#define SetProgressBarColour \
    SetProgressBarColor

stock SetProgressBarValue(Bar:barid, Float:value)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

	PB_eBar[barid][PB_e_BAR_VALUE] = value;
	PB_RenderBar(barid);
	return 1;
}

stock SetProgressBarMaxValue(Bar:barid, Float:maxvalue)
{
	if (!IsValidProgressBar(barid))
	{
	    return 0;
	}

	PB_eBar[barid][PB_e_BAR_MAX_VALUE] = maxvalue;
	PB_RenderBar(barid);
	return 1;
}

stock CountProgressBars()
{
	return PB_iBarCount;
}
