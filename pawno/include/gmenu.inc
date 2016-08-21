#if defined gmenu_included
	#endinput
#endif
#define gmenu_included

/*
 * gmenu.inc
 * Version: 2.0.1 (last updated on 21 August, 2016)
 * By Gammix
 *
 * Functions:
   native ShowPlayerMenu(playerid, menuid, caption[], info[], type[] = "Interaction Menu", selectioncolor = 0x3F7FBFFF, textcolor = 0xFFFFFFFF, caption_boxcolor = 0xC51A71FF, listitem_boxcolor = 0x000000FF);
 *
 * Callbacks:
   forward OnMenuResponse(playerid, menuid, response, listitem);
 *
 */

#if !defined MENU_SOUND_UP
	#define MENU_SOUND_UP (1054)
#endif

#if !defined MENU_SOUND_DOWN
	#define MENU_SOUND_DOWN (1054)
#endif

#if !defined MENU_SOUND_CLOSE
	#define MENU_SOUND_CLOSE (1084)
#endif

#if !defined MENU_SOUND_SELECT
	#define MENU_SOUND_SELECT (1083)
#endif

#define Menu_CountPages(%0,%1) \
	(((%0) - 1) / (%1) + 1)

#define Menu_IsEven(%0) \
	(((%0 % 2) > 0) ? (1) : (0))

#define Menu_SetOpacity(%0,%1) \
	((%0 & ~0xFF) | clamp(%1, 0x00, 0xFF))

#define INVALID_MENU_ID (-1)

enum
{
	MENU_RESPONSE_UP = 1,
	MENU_RESPONSE_DOWN,
	MENU_RESPONSE_SELECT,
	MENU_RESPONSE_CLOSE
};

enum E_PLAYER_MENU
{
	E_PLAYER_MENU_ID,
    E_PLAYER_MENU_PAGE,
    E_PLAYER_MENU_LISTITEM,
    E_PLAYER_MENU_TOTAL_LISTITEMS,
	E_PLAYER_MENU_SELECTION_COLOR,
	E_PLAYER_MENU_ITEMBOX_COLOR,
	E_PLAYER_MENU_TICKCOUNT
};
static p_Menu[MAX_PLAYERS][E_PLAYER_MENU];
static PlayerText:p_MenuTextDraw[MAX_PLAYERS][15];

static PlayerText:Menu_PlayerTextDrawCreate(playerid, Float:posX, Float:posY, string[], bgcolor, font, Float:lettersizeX, Float:lettersizeY, textcolor, allign = 0, outline = 1, proportional = 1, shadow = 1, usebox = 1, boxcolor = 0, Float:textsizeX = -1.0, Float:textsizeY = -1.0)
{
	static PlayerText:text;

    text = CreatePlayerTextDraw(playerid, posX, posY, string);
	PlayerTextDrawBackgroundColor(playerid, text, bgcolor);
	PlayerTextDrawFont(playerid, text, font);
	PlayerTextDrawLetterSize(playerid, text, lettersizeX, lettersizeY);
	PlayerTextDrawColor(playerid, text, textcolor);
 	PlayerTextDrawAlignment(playerid, text, allign);
	PlayerTextDrawSetOutline(playerid, text, outline);
	PlayerTextDrawSetProportional(playerid, text, proportional);
	PlayerTextDrawSetShadow(playerid, text, shadow);
	PlayerTextDrawUseBox(playerid, text, usebox);

	if(usebox)
		PlayerTextDrawBoxColor(playerid, text, boxcolor);

	if(textsizeX != -1.0 && textsizeY != -1.0)
		PlayerTextDrawTextSize(playerid, text, textsizeX, textsizeY);

	return text;
}

stock ShowPlayerMenu(playerid, menuid, caption[], info[], type[] = "Interaction Menu", selectioncolor = 0x3F7FBFFF, textcolor = 0xFFFFFFFF, caption_boxcolor = 0xC51A71FF, listitem_boxcolor = 0x000000FF)
{
    static varname[45];

	if (p_Menu[playerid][E_PLAYER_MENU_ID] >= 0)
	{
	    for(new i, j = (4 + ((p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS] >= 10) ? (10) : (p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]))); i < j; i++)
	    {
	        PlayerTextDrawDestroy(playerid, p_MenuTextDraw[playerid][i]);
	    }

	    for (new i, j = p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]; i < j; i++)
	    {
	        format(varname, sizeof (varname), "MenuListitem:%i", i);
			DeletePVar(playerid, varname);
	    }
	}

	p_Menu[playerid][E_PLAYER_MENU_ID] = menuid;
	p_Menu[playerid][E_PLAYER_MENU_PAGE] = 1;
	p_Menu[playerid][E_PLAYER_MENU_LISTITEM] = 1;
	p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS] = 0;
	p_Menu[playerid][E_PLAYER_MENU_SELECTION_COLOR] = selectioncolor;
	p_Menu[playerid][E_PLAYER_MENU_ITEMBOX_COLOR] = listitem_boxcolor;
	p_Menu[playerid][E_PLAYER_MENU_TICKCOUNT] = GetTickCount();

	if (menuid < 0)
	    return 1;

	if (!info[0])
	    return 1;

	static string[150];

	new pos;
	for (new i, j = strlen(info); i < j; i++)
	{
	    if (info[i] == '\n')
	    {
			strmid(string, info, pos, i);
			pos = (i + 1);
		}
		else if (i == (j - 1))
		{
			strmid(string, info, pos, j);
		}
		else continue;

	    if (!string[0])
	        continue;

		format(varname, sizeof (varname), "MenuListitem:%i", p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]);
		SetPVarString(playerid, varname, string);

		p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]++;
	}

    p_MenuTextDraw[playerid][0] = Menu_PlayerTextDrawCreate(playerid, 20.000000, 160.000000, "caption_box", 255, 1, 0.000000, 3.000000, 0xFFFFFFFF, 1, 0, 1, 1, 1, Menu_SetOpacity(caption_boxcolor, 150), 150.000000, 0.000000);
    PlayerTextDrawShow(playerid, p_MenuTextDraw[playerid][0]);

	p_MenuTextDraw[playerid][1] = Menu_PlayerTextDrawCreate(playerid, 83.000000, 164.000000, caption, 0, 1, 0.470000, 1.900000, textcolor, 2, 1, 1);
    PlayerTextDrawShow(playerid, p_MenuTextDraw[playerid][1]);

	p_MenuTextDraw[playerid][2] = Menu_PlayerTextDrawCreate(playerid, 20.000000, 191.000000, type, 255, 1, 0.139999, 0.799999, textcolor, 1, 0, 1, 1, 1, Menu_SetOpacity(listitem_boxcolor, 200), 150.000000, 0.000000);
	PlayerTextDrawShow(playerid, p_MenuTextDraw[playerid][2]);

	format(string, sizeof (string), "1/%i", p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]);
	p_MenuTextDraw[playerid][3] = Menu_PlayerTextDrawCreate(playerid, 149.000000, 189.000000, string, 255, 1, 0.139999, 0.799999, textcolor, 3, 0, 1);
	PlayerTextDrawShow(playerid, p_MenuTextDraw[playerid][3]);

	const Float:MENU_BASE_X = 20.000000;
	const Float:MENU_BASE_Y = 202.000000;

	for(new i, j = ((p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS] > 10) ? (10) : (p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS])); i < j; i++)
	{
		format(varname, sizeof (varname), "MenuListitem:%i", i);
		GetPVarString(playerid, varname, string, sizeof (string));

		if(Menu_IsEven(i))
			p_MenuTextDraw[playerid][(4 + i)] = Menu_PlayerTextDrawCreate(playerid, MENU_BASE_X, MENU_BASE_Y + (i * 10.0), string, 255, 1, 0.139999, 0.799999, textcolor, 1, 0, 1, 1, 1, Menu_SetOpacity(listitem_boxcolor, 150), 150.000000, 0.000000);
		else
			p_MenuTextDraw[playerid][(4 + i)] = Menu_PlayerTextDrawCreate(playerid, MENU_BASE_X, MENU_BASE_Y + (i * 10), string, 255, 1, 0.139999, 0.799999, textcolor, 1, 0, 1, 1, 1, Menu_SetOpacity(listitem_boxcolor, 100), 150.000000, 0.000000);

		PlayerTextDrawShow(playerid, p_MenuTextDraw[playerid][(4 + i)]);
	}

	PlayerTextDrawBoxColor(playerid, p_MenuTextDraw[playerid][4], Menu_SetOpacity(selectioncolor, 200));
    PlayerTextDrawShow(playerid, p_MenuTextDraw[playerid][4]);

	return 1;
}

public OnPlayerConnect(playerid)
{
	p_Menu[playerid][E_PLAYER_MENU_ID] = INVALID_MENU_ID;

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

public OnPlayerUpdate(playerid)
{
	if(p_Menu[playerid][E_PLAYER_MENU_ID] != INVALID_MENU_ID)
	{
	    static string[45];

	    new keys, updown, leftright;
		GetPlayerKeys(playerid, keys, updown, leftright);

		#pragma unused leftright

		if (updown == KEY_DOWN && (GetTickCount() - p_Menu[playerid][E_PLAYER_MENU_TICKCOUNT]) >= 200)
		{
    		p_Menu[playerid][E_PLAYER_MENU_TICKCOUNT] = GetTickCount();

    		if ((p_Menu[playerid][E_PLAYER_MENU_LISTITEM] + 1) > ((p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS] > 10) ? (10) : (p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS])))
    		{
    		    if ((p_Menu[playerid][E_PLAYER_MENU_PAGE] + 1) > Menu_CountPages(p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS], 10))
    		    {
    		        PlayerPlaySound(playerid, MENU_SOUND_UP, 0.0, 0.0, 0.0);

    		        #if defined Menu_OnPlayerUpdate
				       	return Menu_OnPlayerUpdate(playerid);
					#else
					   	return 1;
					#endif
    		    }

    		    p_Menu[playerid][E_PLAYER_MENU_LISTITEM] = 1;
    		    p_Menu[playerid][E_PLAYER_MENU_PAGE]++;
    		}
    		else
    		{
    		    p_Menu[playerid][E_PLAYER_MENU_LISTITEM]++;
    		}

			format(string, sizeof (string), "%i/%i", p_Menu[playerid][E_PLAYER_MENU_LISTITEM], p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]);
			PlayerTextDrawSetString(playerid, p_MenuTextDraw[playerid][3], string);

			for (new i; i < 10; i++)
			{
			    if ((i + ((p_Menu[playerid][E_PLAYER_MENU_PAGE] - 1) * 10)) >= p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS])
			        break;

			    if (i == (p_Menu[playerid][E_PLAYER_MENU_LISTITEM] - 1))
			    {
			        PlayerTextDrawBoxColor(playerid, p_MenuTextDraw[playerid][(4 + i)], Menu_SetOpacity(p_Menu[playerid][E_PLAYER_MENU_SELECTION_COLOR], 200));
			    }
			    else
			    {
				    if (Menu_IsEven(i))
				    	PlayerTextDrawBoxColor(playerid, p_MenuTextDraw[playerid][(4 + i)], Menu_SetOpacity(p_Menu[playerid][E_PLAYER_MENU_ITEMBOX_COLOR], 150));
				    else
				    	PlayerTextDrawBoxColor(playerid, p_MenuTextDraw[playerid][(4 + i)], Menu_SetOpacity(p_Menu[playerid][E_PLAYER_MENU_ITEMBOX_COLOR], 100));
				}

                PlayerTextDrawShow(playerid, p_MenuTextDraw[playerid][(4 + i)]);
			}

			#if defined OnMenuResponse
    			OnMenuResponse(playerid, p_Menu[playerid][E_PLAYER_MENU_ID], MENU_RESPONSE_DOWN, (p_Menu[playerid][E_PLAYER_MENU_LISTITEM] * p_Menu[playerid][E_PLAYER_MENU_PAGE]));
			#endif
		}
		else if (updown == KEY_UP && (GetTickCount() - p_Menu[playerid][E_PLAYER_MENU_TICKCOUNT]) >= 200)
		{
    		p_Menu[playerid][E_PLAYER_MENU_TICKCOUNT] = GetTickCount();

    		if ((p_Menu[playerid][E_PLAYER_MENU_LISTITEM] - 1) == 0)
    		{
    		    if ((p_Menu[playerid][E_PLAYER_MENU_PAGE] - 1) == 0)
    		    {
    		        PlayerPlaySound(playerid, MENU_SOUND_UP, 0.0, 0.0, 0.0);

    		        #if defined Menu_OnPlayerUpdate
				       	return Menu_OnPlayerUpdate(playerid);
					#else
					   	return 1;
					#endif
    		    }

    		    p_Menu[playerid][E_PLAYER_MENU_LISTITEM] = 1;
    		    p_Menu[playerid][E_PLAYER_MENU_PAGE]--;
    		}
    		else
    		{
    		    p_Menu[playerid][E_PLAYER_MENU_LISTITEM]--;
    		}

			format(string, sizeof (string), "%i/%i", p_Menu[playerid][E_PLAYER_MENU_LISTITEM], p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]);
			PlayerTextDrawSetString(playerid, p_MenuTextDraw[playerid][3], string);

			for (new i; i < 10; i++)
			{
			    if ((i + ((p_Menu[playerid][E_PLAYER_MENU_PAGE] - 1) * 10)) >= p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS])
			        break;

			    if (i == (p_Menu[playerid][E_PLAYER_MENU_LISTITEM] - 1))
			    {
			        PlayerTextDrawBoxColor(playerid, p_MenuTextDraw[playerid][(4 + i)], Menu_SetOpacity(p_Menu[playerid][E_PLAYER_MENU_SELECTION_COLOR], 200));
			    }
			    else
			    {
				    if (Menu_IsEven(i))
				    	PlayerTextDrawBoxColor(playerid, p_MenuTextDraw[playerid][(4 + i)], Menu_SetOpacity(p_Menu[playerid][E_PLAYER_MENU_ITEMBOX_COLOR], 150));
				    else
				    	PlayerTextDrawBoxColor(playerid, p_MenuTextDraw[playerid][(4 + i)], Menu_SetOpacity(p_Menu[playerid][E_PLAYER_MENU_ITEMBOX_COLOR], 100));
				}

                PlayerTextDrawShow(playerid, p_MenuTextDraw[playerid][(4 + i)]);
			}

			#if defined OnMenuResponse
    			OnMenuResponse(playerid, p_Menu[playerid][E_PLAYER_MENU_ID], MENU_RESPONSE_UP, (p_Menu[playerid][E_PLAYER_MENU_LISTITEM] * p_Menu[playerid][E_PLAYER_MENU_PAGE]));
			#endif
		}
		else if (keys == KEY_SECONDARY_ATTACK && (GetTickCount() - p_Menu[playerid][E_PLAYER_MENU_TICKCOUNT]) >= 200)
		{
			for (new i, j = (4 + ((p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS] >= 10) ? (10) : (p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]))); i < j; i++)
				PlayerTextDrawDestroy(playerid, p_MenuTextDraw[playerid][i]);

            for (new i, j = p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]; i < j; i++)
		    {
		        format(string, sizeof (string), "MenuListitem:%i", i);
				DeletePVar(playerid, string);
		    }

			PlayerPlaySound(playerid, MENU_SOUND_CLOSE, 0.0, 0.0, 0.0);

			new menuid = p_Menu[playerid][E_PLAYER_MENU_ID];
			p_Menu[playerid][E_PLAYER_MENU_ID] = INVALID_MENU_ID;
		    
		    #if defined OnMenuResponse
    			OnMenuResponse(playerid, menuid, MENU_RESPONSE_CLOSE, (p_Menu[playerid][E_PLAYER_MENU_LISTITEM] * p_Menu[playerid][E_PLAYER_MENU_PAGE]));
			#endif
		}
		else if (keys == KEY_SPRINT && (GetTickCount() - p_Menu[playerid][E_PLAYER_MENU_TICKCOUNT]) >= 200)
		{
			for (new i, j = (4 + ((p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS] >= 10) ? (10) : (p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]))); i < j; i++)
				PlayerTextDrawDestroy(playerid, p_MenuTextDraw[playerid][i]);

            for (new i, j = p_Menu[playerid][E_PLAYER_MENU_TOTAL_LISTITEMS]; i < j; i++)
		    {
		        format(string, sizeof (string), "MenuListitem:%i", i);
				DeletePVar(playerid, string);
		    }

			PlayerPlaySound(playerid, MENU_SOUND_SELECT, 0.0, 0.0, 0.0);

			new menuid = p_Menu[playerid][E_PLAYER_MENU_ID];
			p_Menu[playerid][E_PLAYER_MENU_ID] = INVALID_MENU_ID;
		    
		    #if defined OnMenuResponse
    			OnMenuResponse(playerid, menuid, MENU_RESPONSE_SELECT, (p_Menu[playerid][E_PLAYER_MENU_LISTITEM] * p_Menu[playerid][E_PLAYER_MENU_PAGE]));
			#endif
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

#if defined OnMenuResponse
	forward OnMenuResponse(playerid, menuid, response, listitem);
#endif
