#if defined gmenu_included
	#endinput
#endif
#define gmenu_included

// GMenu.inc - By Gammix - Last updated: 5 Sept, 2017

#if !defined MENU_SOUND_UPDOWN
	#define MENU_SOUND_UPDOWN \
		1054
#endif

#if !defined MENU_SOUND_CLOSE
	#define MENU_SOUND_CLOSE \
		1084
#endif

#if !defined MENU_SOUND_SELECT
	#define MENU_SOUND_SELECT \
		1083
#endif

#if !defined MENU_MAX_LISTITEMS
	#define MENU_MAX_LISTITEMS \
		24
#endif

#if !defined MENU_MAX_LISTITEM_SIZE
	#define MENU_MAX_LISTITEM_SIZE \
		128
#endif

#if !defined MENU_MAX_LISTITEMS_PERPAGE
	#define MENU_MAX_LISTITEMS_PERPAGE \
		8
#endif

#define MENU_COUNT_PAGES(%0,%1) \
	(((%0) - 1) / (%1) + 1)

#define MENU:%0(%1) \
	forward menu_%0(%1); public menu_%0(%1)

#define Menu:%0(%1) \
	MENU:%0(%1)

enum
{
	MENU_RESPONSE_UP,
	MENU_RESPONSE_DOWN,
	MENU_RESPONSE_SELECT,
	MENU_RESPONSE_CLOSE
};

enum E_PLAYER_MENU
{
	E_PLAYER_MENU_ID[32],
    E_PLAYER_MENU_PAGE,
    E_PLAYER_MENU_LISTITEM,
    E_PLAYER_MENU_TOTAL_LISTITEMS,
	E_PLAYER_MENU_TICKCOUNT
};
static playerMenu[MAX_PLAYERS][E_PLAYER_MENU];

enum E_MENU_TEXTDRAW
{
	E_MENU_TEXTDRAW_LISTITEM_COUNT,
	E_MENU_TEXTDRAW_LISTITEMS[MENU_MAX_LISTITEMS_PERPAGE],
	E_MENU_TEXTDRAW_INFO_BOX,
	E_MENU_TEXTDRAW_INFO_ICON,
	E_MENU_TEXTDRAW_INFO_TEXT,
};
static menuPlayerTextDrawsID[MAX_PLAYERS][E_MENU_TEXTDRAW];
static PlayerText:menuPlayerTextDraws[MAX_PLAYERS][50];
static menuPlayerTextDrawsCount[MAX_PLAYERS];

static playerMenuListitems[MAX_PLAYERS][MENU_MAX_LISTITEMS][MENU_MAX_LISTITEM_SIZE char];
static playerMenuListitemsInfo[MAX_PLAYERS][MENU_MAX_LISTITEMS][MENU_MAX_LISTITEM_SIZE char];

static Internal_UpdateListitemInfo(playerid)
{
	new listitemid = (playerMenu[playerid][E_PLAYER_MENU_LISTITEM] + (playerMenu[playerid][E_PLAYER_MENU_PAGE] * MENU_MAX_LISTITEMS_PERPAGE));
	if (playerMenuListitemsInfo[playerid][listitemid]{0})
	{
        PlayerTextDrawShow(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_INFO_BOX]]);
        PlayerTextDrawShow(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_INFO_ICON]]);

	    new string[MENU_MAX_LISTITEM_SIZE];
	    strunpack(string, playerMenuListitemsInfo[playerid][listitemid]);
	    PlayerTextDrawSetString(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_INFO_TEXT]], string);
        PlayerTextDrawShow(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_INFO_TEXT]]);
	}
	else
	{
        PlayerTextDrawHide(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_INFO_BOX]]);
        PlayerTextDrawHide(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_INFO_ICON]]);
		PlayerTextDrawHide(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_INFO_TEXT]]);
	}
}

static Internal_UpdateListitems(playerid)
{
	new string[MENU_MAX_LISTITEM_SIZE];
	new listitem = ((playerMenu[playerid][E_PLAYER_MENU_LISTITEM] + (playerMenu[playerid][E_PLAYER_MENU_PAGE] * MENU_MAX_LISTITEMS_PERPAGE)) + 1);
    format(string, sizeof string, "%i/%i", listitem, playerMenu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]);
	PlayerTextDrawSetString(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_LISTITEM_COUNT]], string);

	for (new i; i < MENU_MAX_LISTITEMS_PERPAGE; i++)
	{
	    listitem = (i + (playerMenu[playerid][E_PLAYER_MENU_PAGE] * MENU_MAX_LISTITEMS_PERPAGE));
 		if (listitem >= playerMenu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS])
		{
 			PlayerTextDrawHide(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_LISTITEMS][i]]);
	 		continue;
		}

		if (i == playerMenu[playerid][E_PLAYER_MENU_LISTITEM])
  		{
        	PlayerTextDrawColor(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_LISTITEMS][i]], 255);
        	PlayerTextDrawBoxColor(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_LISTITEMS][i]], -86);
		}
		else
		{
        	PlayerTextDrawColor(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_LISTITEMS][i]], -1);
        	PlayerTextDrawBoxColor(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_LISTITEMS][i]], 175);
		}

	    strunpack(string, playerMenuListitems[playerid][listitem]);
	    PlayerTextDrawSetString(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_LISTITEMS][i]], string);
        PlayerTextDrawShow(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_LISTITEMS][i]]);
	}

	Internal_UpdateListitemInfo(playerid);
}

/*
native ShowPlayerMenu(playerid, menuid[], caption[], type[] = "Interaction Menu", captionTextColor = -926365441, captionBoxColor = -1522452536, captionSprite[] = "loadsc12:loadsc12");
*/
#define ShowPlayerMenu(%0,%1, \
	Internal_ShowPlayerMenu(%0, #%1,

stock Internal_ShowPlayerMenu(playerid, menuid[], caption[], type[] = "INTERACTION MENU", captionTextColor = 0xf7f7f7ff, captionBoxColor = 0xdb0dd0bb, captionSprite[] = "loadsc12:loadsc12")
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	for (new i; i < menuPlayerTextDrawsCount[playerid]; i++)
	{
	    PlayerTextDrawDestroy(playerid, menuPlayerTextDraws[playerid][i]);
	}
    menuPlayerTextDrawsCount[playerid] = 0;

    menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]] = CreatePlayerTextDraw(playerid,12.000000, 141.000000, captionSprite);
	PlayerTextDrawBackgroundColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 255);
	PlayerTextDrawFont(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 4);
	PlayerTextDrawLetterSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0.000000, 4.000000);
	PlayerTextDrawColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], captionBoxColor);
	PlayerTextDrawSetOutline(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawSetProportional(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawSetShadow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawUseBox(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawBoxColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawTextSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 139.000000, 40.000000);
	PlayerTextDrawSetSelectable(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawShow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]++]);

	menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]] = CreatePlayerTextDraw(playerid,13.000000, 143.000000, "_box");
	PlayerTextDrawBackgroundColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 255);
	PlayerTextDrawFont(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawLetterSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0.000000, 4.000000);
	PlayerTextDrawColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], -1);
	PlayerTextDrawSetOutline(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawSetProportional(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawSetShadow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawUseBox(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawBoxColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], captionBoxColor);
	PlayerTextDrawTextSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 150.000000, 0.000000);
	PlayerTextDrawShow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]]);
	PlayerTextDrawSetSelectable(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]++], 0);

	menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]] = CreatePlayerTextDraw(playerid,16.000000, 151.000000, caption);
	PlayerTextDrawBackgroundColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawFont(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawLetterSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0.399998, 2.299998);
	PlayerTextDrawColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], captionTextColor);
	PlayerTextDrawSetOutline(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawSetProportional(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawSetShadow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawSetSelectable(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawShow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]++]);

	menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]] = CreatePlayerTextDraw(playerid,13.000000, 182.000000, "_box");
	PlayerTextDrawBackgroundColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawFont(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawLetterSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0.000000, 1.200000);
	PlayerTextDrawColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawSetOutline(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawSetProportional(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawSetShadow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawUseBox(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawBoxColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 421075400);
	PlayerTextDrawTextSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 150.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawShow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]++]);

	for (new i; type[i] != EOS; i++)
	{
	    toupper(type[i]);
	}
	menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]] = CreatePlayerTextDraw(playerid,14.000000, 183.000000, type);
	PlayerTextDrawBackgroundColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawFont(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawLetterSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0.189998, 1.000000);
	PlayerTextDrawColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0xf7f7f7ff);
	PlayerTextDrawSetOutline(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawSetProportional(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawSetShadow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawSetSelectable(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawShow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]++]);

	menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]] = CreatePlayerTextDraw(playerid,149.000000, 183.000000, "-/-");
	PlayerTextDrawAlignment(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 3);
	PlayerTextDrawBackgroundColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawFont(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawLetterSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0.189998, 1.000000);
	PlayerTextDrawColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0xf7f7f7ff);
	PlayerTextDrawSetOutline(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawSetProportional(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawSetShadow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawSetSelectable(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawShow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]]);
	menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_LISTITEM_COUNT] = menuPlayerTextDrawsCount[playerid]++;

	menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]] = CreatePlayerTextDraw(playerid,13.000000, 196.000000, "_box");
	PlayerTextDrawBackgroundColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawFont(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawLetterSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0.159999, (1.999999 + (1.6 * (MENU_MAX_LISTITEMS_PERPAGE - 1))));
	PlayerTextDrawColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawSetOutline(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawSetProportional(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawSetShadow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawUseBox(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawBoxColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 421075400);
	PlayerTextDrawTextSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 150.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawShow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]++]);

	for (new i; i < MENU_MAX_LISTITEMS_PERPAGE; i++)
	{
		menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]] = CreatePlayerTextDraw(playerid,13.000000, (196.000000 + (15 * i)), "_");
		PlayerTextDrawBackgroundColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
		PlayerTextDrawFont(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0.230000, 1.399999);
		PlayerTextDrawColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
		PlayerTextDrawSetProportional(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
		PlayerTextDrawSetShadow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 175);
		PlayerTextDrawTextSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 150.000000, 0.000000);
		PlayerTextDrawSetSelectable(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
		menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_LISTITEMS][i] = menuPlayerTextDrawsCount[playerid]++;
	}

	menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]] = CreatePlayerTextDraw(playerid,13.000000, 319.000000, "_box");
	PlayerTextDrawBackgroundColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawFont(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawLetterSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0.159999, 1.999999);
	PlayerTextDrawColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawSetOutline(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawSetProportional(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawSetShadow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawUseBox(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawBoxColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 421075400);
	PlayerTextDrawTextSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 150.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_INFO_BOX] = menuPlayerTextDrawsCount[playerid]++;

	menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]] = CreatePlayerTextDraw(playerid,13.000000, 319.000000, "_model");
	PlayerTextDrawBackgroundColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawFont(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 5);
	PlayerTextDrawLetterSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0.159999, 0.899999);
	PlayerTextDrawColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], -1);
	PlayerTextDrawSetOutline(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawSetProportional(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawSetShadow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawUseBox(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawBoxColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawTextSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 10.000000, 10.000000);
	PlayerTextDrawSetPreviewModel(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1239);
	PlayerTextDrawSetPreviewRot(playerid, menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0.000000, 0.000000, 0.000000, 1.000000);
	PlayerTextDrawSetSelectable(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_INFO_ICON] = menuPlayerTextDrawsCount[playerid]++;

	menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]] = CreatePlayerTextDraw(playerid,23.000000, 319.000000, "Write listitem info here");
	PlayerTextDrawBackgroundColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawFont(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawLetterSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0.159999, 0.899999);
	PlayerTextDrawColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], -1);
	PlayerTextDrawSetOutline(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawSetProportional(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawSetShadow(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawUseBox(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 1);
	PlayerTextDrawBoxColor(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	PlayerTextDrawTextSize(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 150.000000, 0.000000);
	PlayerTextDrawSetSelectable(playerid,menuPlayerTextDraws[playerid][menuPlayerTextDrawsCount[playerid]], 0);
	menuPlayerTextDrawsID[playerid][E_MENU_TEXTDRAW_INFO_TEXT] = menuPlayerTextDrawsCount[playerid]++;

	format(playerMenu[playerid][E_PLAYER_MENU_ID], 32, menuid);
	playerMenu[playerid][E_PLAYER_MENU_PAGE] = 0;
	playerMenu[playerid][E_PLAYER_MENU_LISTITEM] = 0;
	playerMenu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS] = 0;
	playerMenu[playerid][E_PLAYER_MENU_TICKCOUNT] = 0;
	return 1;
}

/*
native AddPlayerMenuItem(playerid, text[], info[] = "");
*/
stock AddPlayerMenuItem(playerid, text[], info[] = "")
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (!playerMenu[playerid][E_PLAYER_MENU_ID][0])
	{
	    return 0;
	}

	if (playerMenu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS] == MENU_MAX_LISTITEMS)
	{
	    return 0;
	}

	strpack(playerMenuListitems[playerid][playerMenu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]], text, MENU_MAX_LISTITEM_SIZE);
	strpack(playerMenuListitemsInfo[playerid][playerMenu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]], info, MENU_MAX_LISTITEM_SIZE);
    playerMenu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]++;

    Internal_UpdateListitems(playerid);
    return 1;
}

/*
native HidePlayerMenu(playerid);
*/
stock HidePlayerMenu(playerid)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

    for (new i; i < menuPlayerTextDrawsCount[playerid]; i++)
	{
	    PlayerTextDrawDestroy(playerid, menuPlayerTextDraws[playerid][i]);
	}
    menuPlayerTextDrawsCount[playerid] = 0;

    playerMenu[playerid][E_PLAYER_MENU_ID][0] = EOS;
	return 1;
}

public OnPlayerConnect(playerid)
{
    menuPlayerTextDrawsCount[playerid] = 0;
	playerMenu[playerid][E_PLAYER_MENU_ID][0] = EOS;

    #if defined Menu_OnPlayerConnect
       	return Menu_OnPlayerConnect(playerid);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect Menu_OnPlayerConnect
#if defined Menu_OnPlayerConnect
    forward Menu_OnPlayerConnect(playerid);
#endif

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(playerMenu[playerid][E_PLAYER_MENU_ID][0] != EOS)
	{
		if (newkeys == KEY_SECONDARY_ATTACK && (GetTickCount() - playerMenu[playerid][E_PLAYER_MENU_TICKCOUNT]) >= 200)
		{
    		playerMenu[playerid][E_PLAYER_MENU_TICKCOUNT] = GetTickCount();
			PlayerPlaySound(playerid, MENU_SOUND_CLOSE, 0.0, 0.0, 0.0);

			new menuid[32] = "menu_";
			strcat(menuid, playerMenu[playerid][E_PLAYER_MENU_ID]);

			for (new i; i < menuPlayerTextDrawsCount[playerid]; i++)
			{
			    PlayerTextDrawDestroy(playerid, menuPlayerTextDraws[playerid][i]);
			}
		    menuPlayerTextDrawsCount[playerid] = 0;
		    playerMenu[playerid][E_PLAYER_MENU_ID][0] = EOS;

			CallLocalFunction(menuid, "iii", playerid, MENU_RESPONSE_CLOSE, (playerMenu[playerid][E_PLAYER_MENU_LISTITEM] + (playerMenu[playerid][E_PLAYER_MENU_PAGE] * MENU_MAX_LISTITEMS_PERPAGE)));
		}
		else if (newkeys == KEY_SPRINT && (GetTickCount() - playerMenu[playerid][E_PLAYER_MENU_TICKCOUNT]) >= 200)
		{
    		playerMenu[playerid][E_PLAYER_MENU_TICKCOUNT] = GetTickCount();
			PlayerPlaySound(playerid, MENU_SOUND_SELECT, 0.0, 0.0, 0.0);

			new menuid[32] = "menu_";
			strcat(menuid, playerMenu[playerid][E_PLAYER_MENU_ID]);

			for (new i; i < menuPlayerTextDrawsCount[playerid]; i++)
			{
			    PlayerTextDrawDestroy(playerid, menuPlayerTextDraws[playerid][i]);
			}
		    menuPlayerTextDrawsCount[playerid] = 0;
		    playerMenu[playerid][E_PLAYER_MENU_ID][0] = EOS;

			CallLocalFunction(menuid, "iii", playerid, MENU_RESPONSE_SELECT, (playerMenu[playerid][E_PLAYER_MENU_LISTITEM] + (playerMenu[playerid][E_PLAYER_MENU_PAGE] * MENU_MAX_LISTITEMS_PERPAGE)));
		}
	}
	
	#if defined Menu_OnPlayerKeyStateChange
       	return Menu_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange Menu_OnPlayerKeyStateChange
#if defined Menu_OnPlayerKeyStateChange
    forward Menu_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif
		
public OnPlayerUpdate(playerid)
{
	if(playerMenu[playerid][E_PLAYER_MENU_ID][0] != EOS)
	{
	    new keys, updown, leftright;
		GetPlayerKeys(playerid, keys, updown, leftright);

		#pragma unused leftright
		#pragma unused keys

		if (updown == KEY_DOWN && (GetTickCount() - playerMenu[playerid][E_PLAYER_MENU_TICKCOUNT]) >= 200)
		{
		    new listitem = (playerMenu[playerid][E_PLAYER_MENU_LISTITEM] + (playerMenu[playerid][E_PLAYER_MENU_PAGE] * MENU_MAX_LISTITEMS_PERPAGE));

    		playerMenu[playerid][E_PLAYER_MENU_TICKCOUNT] = GetTickCount();
			PlayerPlaySound(playerid, MENU_SOUND_UPDOWN, 0.0, 0.0, 0.0);

			if ((listitem + 1) == playerMenu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS])
			{
				playerMenu[playerid][E_PLAYER_MENU_LISTITEM] = 0;
				playerMenu[playerid][E_PLAYER_MENU_PAGE] = 0;
			}
			else if ((playerMenu[playerid][E_PLAYER_MENU_LISTITEM] + 1) == ((playerMenu[playerid][E_PLAYER_MENU_PAGE] + 1) * MENU_MAX_LISTITEMS_PERPAGE))
			{
				playerMenu[playerid][E_PLAYER_MENU_LISTITEM] = 0;
				playerMenu[playerid][E_PLAYER_MENU_PAGE]++;
			}
			else
			{
				playerMenu[playerid][E_PLAYER_MENU_LISTITEM]++;
			}

			Internal_UpdateListitems(playerid);

			new menuid[32] = "menu_";
			strcat(menuid, playerMenu[playerid][E_PLAYER_MENU_ID]);
			CallLocalFunction(menuid, "iii", playerid, MENU_RESPONSE_DOWN, listitem);
		}
		else if (updown == KEY_UP && (GetTickCount() - playerMenu[playerid][E_PLAYER_MENU_TICKCOUNT]) >= 200)
		{
    		playerMenu[playerid][E_PLAYER_MENU_TICKCOUNT] = GetTickCount();
			PlayerPlaySound(playerid, MENU_SOUND_UPDOWN, 0.0, 0.0, 0.0);

    		if ((playerMenu[playerid][E_PLAYER_MENU_LISTITEM] - 1) == -1)
			{
			    if (playerMenu[playerid][E_PLAYER_MENU_PAGE] == 0)
			    {
					playerMenu[playerid][E_PLAYER_MENU_LISTITEM] = ((MENU_MAX_LISTITEMS_PERPAGE - ((MENU_COUNT_PAGES(playerMenu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS], MENU_MAX_LISTITEMS_PERPAGE) * MENU_MAX_LISTITEMS_PERPAGE) - playerMenu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS])) - 1);
					playerMenu[playerid][E_PLAYER_MENU_PAGE] = (MENU_COUNT_PAGES(playerMenu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS], MENU_MAX_LISTITEMS_PERPAGE) - 1);
				}
				else
				{
					playerMenu[playerid][E_PLAYER_MENU_LISTITEM] = (MENU_MAX_LISTITEMS_PERPAGE - 1);
					playerMenu[playerid][E_PLAYER_MENU_PAGE]--;
				}
			}
			else
			{
				playerMenu[playerid][E_PLAYER_MENU_LISTITEM]--;
			}

			Internal_UpdateListitems(playerid);

			new menuid[32] = "menu_";
			strcat(menuid, playerMenu[playerid][E_PLAYER_MENU_ID]);
			CallLocalFunction(menuid, "iii", playerid, MENU_RESPONSE_DOWN, (playerMenu[playerid][E_PLAYER_MENU_LISTITEM] + (playerMenu[playerid][E_PLAYER_MENU_PAGE] * MENU_MAX_LISTITEMS_PERPAGE)));
		}
	}

	#if defined Menu_OnPlayerUpdate
       	return Menu_OnPlayerUpdate(playerid);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerUpdate
    #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate Menu_OnPlayerUpdate
#if defined Menu_OnPlayerUpdate
    forward Menu_OnPlayerUpdate(playerid);
#endif
