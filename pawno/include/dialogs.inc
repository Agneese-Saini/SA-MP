#if defined _dialogs_included
	#endinput
#endif
#define _dialogs_included

/*
 * dialogs.inc
 * Version: v2.7.3 (last updated: 13 August, 2016)
 * By Gammix
 *
 * Functions:
   native ShowPlayerPreviewModelDialog(playerid, dialogid, style, caption[], models[], labels[][], button1[], button2[], limit = sizeof (models));
   native Dialog_SetModelRot(playerid, listitem, Float:x = 0.0, Float:y = 0.0, Float:z = 0.0, Float:zoom = 1.0, color1 = -1, color2 = -1);
 *
 */

enum
{
    DIALOG_STYLE_PREVMODEL = 6,
    DIALOG_STYLE_PREVMODEL_LIST
};

#define DIALOG_MAX_TEXT_DRAWS (100)

#define DIALOG_MAX_LISTITEM_SIZE (150)

#define Dialog_CountPages(%0,%1) \
	(((%0) - 1) / (%1) + 1)

#define Dialog_SetOpacity(%0,%1) \
	((%0 & ~0xFF) | clamp(%1, 0x00, 0xFF))

static PlayerText:p_DialogTextDraw[MAX_PLAYERS][DIALOG_MAX_TEXT_DRAWS];
static p_DialogTextDrawPool[MAX_PLAYERS];

enum E_PLAYER_DIALOG
{
		    E_PLAYER_DIALOG_ID,
		    E_PLAYER_DIALOG_STYLE,
    bool:	E_PLAYER_DIALOG_DUAL_BTN,
		    E_PLAYER_DIALOG_LISTITEMS,
		    E_PLAYER_DIALOG_LISTITEM_ID,
		    E_PLAYER_DIALOG_PAGE,
		    E_PLAYER_DIALOG_CLICK_TICKCOUNT,
    bool:	E_PLAYER_DIALOG_CANCELED
};
static p_Dialog[MAX_PLAYERS][E_PLAYER_DIALOG];

enum E_DIALOG_COMPONENT
{
	E_DIALOG_COMPONENT_SCROLL_UP,
	E_DIALOG_COMPONENT_SCROLL_DOWN,
	E_DIALOG_COMPONENT_BUTTON1,
	E_DIALOG_COMPONENT_BUTTON2,
	E_DIALOG_COMPONENT_BUTTONC,
	E_DIALOG_COMPONENT_MODEL[24],
	E_DIALOG_COMPONENT_LABEL[24],
	E_DIALOG_COMPONENT_BAR[24],
	E_DIALOG_COMPONENT_SCROLL
};
static p_DialogCompnent[MAX_PLAYERS][E_DIALOG_COMPONENT];

static Dialog_Create(playerid, Float:posX, Float:posY, string[], bgcolor, font, Float:lettersizeX, Float:lettersizeY, textcolor, allign = 0, outline = 1, proportional = 1, shadow = 1, usebox = 0, boxcolor = 0, Float:textsizeX = -1.0, Float:textsizeY = -1.0, model = 0, Float:rotX = 0.0, Float:rotY = 0.0, Float:rotZ = 0.0, Float:zoom = 1.0, selectable = 0, show = 0)
{
	p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]] = CreatePlayerTextDraw(playerid, posX, posY, string);

	PlayerTextDrawBackgroundColor(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]], bgcolor);
	PlayerTextDrawFont(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]], font);
	PlayerTextDrawLetterSize(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]], lettersizeX, lettersizeY);
	PlayerTextDrawColor(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]], textcolor);
	PlayerTextDrawAlignment(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]], allign);
	PlayerTextDrawSetOutline(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]], outline);
	PlayerTextDrawSetProportional(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]], proportional);
	PlayerTextDrawSetShadow(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]], shadow);
	PlayerTextDrawUseBox(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]], usebox);
	PlayerTextDrawSetSelectable(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]], selectable);

	if (usebox)
		PlayerTextDrawBoxColor(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]], boxcolor);

	if (textsizeX != -1.0 || textsizeY != -1.0)
		PlayerTextDrawTextSize(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]], textsizeX, textsizeY);

	if(font == 5)
	{
		PlayerTextDrawSetPreviewModel(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]], model);
		PlayerTextDrawSetPreviewRot(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]], rotX, rotY, rotZ, zoom);
	}

	if (show)
		PlayerTextDrawShow(playerid, p_DialogTextDraw[playerid][p_DialogTextDrawPool[playerid]]);

	return p_DialogTextDrawPool[playerid]++;
}

static Dialog_Init(playerid, style, caption[], button1[], button2[])
{
	for (new i; i < DIALOG_MAX_TEXT_DRAWS; i++)
		p_DialogTextDraw[playerid][i] = PlayerText:INVALID_TEXT_DRAW;

	Dialog_Create(playerid, 173.000000, 122.000000, "_", 50, 1, 0.000000, 0.599999, -1, 0, 0, 1, 1, 1, 200, 462.000000, 19.000000, .show = 1);
	Dialog_Create(playerid, 173.000000, 121.000000, caption, 50, 1, 0.129998, 0.799998, -36, 0, 0, 1, 1, 0, .show = 1);

	Dialog_Create(playerid, 173.000000, 131.000000, "_", 50, 1, 0.000000, 21.599998, -1, 0, 0, 1, 1, 1, 150, 462.000000, 19.000000, .show = 1);
	Dialog_Create(playerid, 177.000000, 135.000000, "_", 50, 1, 0.000000, 18.299997, -1, 0, 0, 1, 1, 1, 100, 458.000000, 19.000000, .show = 1);
	Dialog_Create(playerid, 177.000000, 134.000000, "_", 50, 1, 0.000000, 18.499998, -1, 0, 0, 1, 1, 1, -106, 173.000000, 19.000000, .show = 1);
	Dialog_Create(playerid, 454.000000, 135.000000, "_", 50, 1, 0.000000, 18.299997, -1, 0, 0, 1, 1, 1, -106, 452.000000, 19.000000, .show = 1);
	Dialog_Create(playerid, 461.000000, 135.000000, "_", 50, 1, 0.000000, 18.299997, -1, 0, 0, 1, 1, 1, -106, 459.000000, 19.000000, .show = 1);
	Dialog_Create(playerid, 462.000000, 134.000000, "_", 50, 1, 0.000000, -0.300001, -1, 0, 0, 1, 1, 1, -106, 174.000000, 19.000000, .show = 1);
	Dialog_Create(playerid, 462.000000, 304.250000, "_", 50, 1, 0.000000, -0.400001, -1, 0, 0, 1, 1, 1, -106, 173.000000, 19.000000, .show = 1);

	p_DialogCompnent[playerid][E_DIALOG_COMPONENT_SCROLL_UP] = Dialog_Create(playerid, 453.000000, 133.000000, "LD_POOL:ball", 50, 4, 0.500000, 1.000000, -106, 0, 0, 1, 1, 1, 255, 7.000000, 9.00000, .selectable = 1, .show = 1);
	Dialog_Create(playerid, 454.000000, 134.000000, "LD_BEAT:up", 255, 4, 0.500000, 1.000000, 255, 0, 0, 1, 1, 1, 255, 5.000000, 6.000000, .show = 1);

	p_DialogCompnent[playerid][E_DIALOG_COMPONENT_SCROLL_DOWN] = Dialog_Create(playerid, 453.000000, 293.000000, "LD_POOL:ball", 50, 4, 0.500000, 1.000000, -106, 0, 0, 1, 1, 1, 255, 7.000000, 9.00000, .selectable = 1, .show = 1);
	Dialog_Create(playerid, 454.000000, 295.000000, "LD_BEAT:down", 255, 4, 0.500000, 1.000000, 255, 0, 0, 1, 1, 1, 255, 5.000000, 6.000000, .show = 1);

	if (button2[0])
	{
		Dialog_Create(playerid, 302.000000, 310.000000, "_", 50, 1, 0.000000, -0.400001, -1, 0, 0, 1, 1, 1, -106, 253.000000, 19.000000, .show = 1);
		Dialog_Create(playerid, 302.000000, 324.000000, "_", 50, 1, 0.000000, -0.400001, -1, 0, 0, 1, 1, 1, -106, 253.000000, 19.000000, .show = 1);
		Dialog_Create(playerid, 253.000000, 304.000000, "(", 0, 1, 0.200000, 2.000000, -106, 0, 0, 1, 1, .show = 1);
		Dialog_Create(playerid, 299.000000, 304.000000, ")", 0, 1, 0.200000, 2.000000, -106, 0, 0, 1, 1, .show = 1);
		p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BUTTON1] = Dialog_Create(playerid, 254.000000, 309.000000, "ld_plan:tvbase", 50, 4, 0.000000, 0.899998, 255, 0, 0, 1, 1, 1, 255, 46.000000, 13.000000, .selectable = 1, .show = 1);
		Dialog_Create(playerid, 278.000000, 311.000000, button1, 50, 1, 0.129998, 0.799998, -36, 2, 0, 1, 1, 0, .show = 1);
		if (!button1[0])
			Dialog_Create(playerid, 278.000000, 311.000000, "button1", 50, 1, 0.129998, 0.799998, -36, 2, 0, 1, 1, 0, .show = 1);
		else
            Dialog_Create(playerid, 278.000000, 311.000000, button1, 50, 1, 0.129998, 0.799998, -36, 2, 0, 1, 1, 0, .show = 1);

		Dialog_Create(playerid, 372.000000, 310.000000, "_", 50, 1, 0.000000, -0.400001, -1, 0, 0, 1, 1, 1, -106, 323.000000, 19.000000, .show = 1);
		Dialog_Create(playerid, 372.000000, 324.000000, "_", 50, 1, 0.000000, -0.400001, -1, 0, 0, 1, 1, 1, -106, 323.000000, 19.000000, .show = 1);
		Dialog_Create(playerid, 322.000000, 304.000000, "(", 0, 1, 0.200000, 2.000000, -106, 0, 0, 1, 1, .show = 1);
		Dialog_Create(playerid, 369.000000, 304.000000, ")", 0, 1, 0.230000, 1.999999, -106, 0, 0, 1, 1, .show = 1);
		p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BUTTON2] = Dialog_Create(playerid, 324.000000, 309.000000, "ld_plan:tvbase", 50, 4, 0.000000, 0.899998, 255, 0, 0, 1, 1, 1, 255, 46.000000, 13.000000, .selectable = 1, .show = 1);
		Dialog_Create(playerid, 348.000000, 311.000000, button2, 50, 1, 0.140000, 0.899999, -36, 2, 0, 1, 1, 0, .show = 1);
	}
	else
	{
		Dialog_Create(playerid, 342.000000, 310.000000, "_", 50, 1, 0.000000, -0.400001, -1, 0, 0, 1, 1, 1, -106, 293.000000, 19.000000, .show = 1);
		Dialog_Create(playerid, 342.000000, 324.000000, "_", 50, 1, 0.000000, -0.400001, -1, 0, 0, 1, 1, 1, -106, 293.000000, 19.000000, .show = 1);
		Dialog_Create(playerid, 293.000000, 304.000000, "(", 0, 1, 0.200000, 2.000000, -106, 0, 0, 1, 1, .show = 1);
		Dialog_Create(playerid, 339.000000, 304.000000, ")", 0, 1, 0.219999, 2.000000, -106, 0, 0, 1, 1, .show = 1);
		p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BUTTONC] = Dialog_Create(playerid, 294.000000, 309.000000, "ld_plan:tvbase", 50, 4, 0.000000, 0.899998, 255, 0, 0, 1, 1, 1, 255, 46.000000, 13.000000, .selectable = 1, .show = 1);
	    if (!button1[0])
			Dialog_Create(playerid, 317.000000, 311.000000, "button1", 50, 1, 0.140000, 0.899999, -36, 2, 0, 1, 1, 0, .show = 1);
		else
			Dialog_Create(playerid, 317.000000, 311.000000, button1, 50, 1, 0.140000, 0.899999, -36, 2, 0, 1, 1, 0, .show = 1);
	}

	if (style == DIALOG_STYLE_PREVMODEL)
	{
		new count;
		new Float:base[2] = {179.000000, 134.000000};

		for (new i; i < 24; i++)
		{
			p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][i] = Dialog_Create(playerid, base[0], base[1], "_", Dialog_SetOpacity(0x3B3B3BFF, 200), 5, 0.129998, 0.799998, -1, 0, 0, 1, 1, 1, 0, 44.000000, 41.000000, .selectable = 1);
	        p_DialogCompnent[playerid][E_DIALOG_COMPONENT_LABEL][i] = Dialog_Create(playerid, base[0] + 2.000000, base[1] + 1.000000, "", 255, 1, 0.140000, 0.899999, -36, 0, 0, 1, 2, 0);

	        base[0] += 45.000000;

	        count++;
	        if (count == 6)
			{
				base[0] = 179.000000;
		        base[1] += 42.000000;

	            count = 0;
			}
		}
	}
	else
	{
		for (new i; i < (5); i++)
		{
			p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BAR][i] = Dialog_Create(playerid, 177.000000, (134.000000 + (33.0 * i)), "ld_plan:tvbase", 50, 4, 0.129997, 0.799997, Dialog_SetOpacity(0x788FB0FF, 200), 0, 0, 1, 1, 1, 0, 274.000000, 32.000000, .selectable = 1);
			p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][i] = Dialog_Create(playerid, 178.000000, (134.000000 + (33.0 * i)), "_", Dialog_SetOpacity(0x3B3B3BFF, 200), 5, 0.129997, 0.799997, -1, 0, 0, 1, 1, 1, 0, 34.000000, 30.000000);
			p_DialogCompnent[playerid][E_DIALOG_COMPONENT_LABEL][i] = Dialog_Create(playerid, 215.000000, (135.000000 + (33.0 * i)), "", 255, 1, 0.099999, 0.599997, -36, 0, 0, 1, 2, 0);
		}
	}

	p_DialogCompnent[playerid][E_DIALOG_COMPONENT_SCROLL] = Dialog_Create(playerid, 0.000000, 0.000000, "_", 0, 1, 0.0, 0.0, 0);
}

static Dialog_Exit(playerid)
{
	for (new i; i < p_DialogTextDrawPool[playerid]; i++)
	{
		PlayerTextDrawHide(playerid, p_DialogTextDraw[playerid][i]);
		PlayerTextDrawDestroy(playerid, p_DialogTextDraw[playerid][i]);

		p_DialogTextDraw[playerid][i] = PlayerText:INVALID_TEXT_DRAW;
  	}

  	p_DialogTextDrawPool[playerid] = 0;
}

static Dialog_GetListitem(playerid, listitem, &model, label[], &Float:x, &Float:y, &Float:z, &Float:zoom, &color1, &color2, maxlength = sizeof (label))
{
	static string[DIALOG_MAX_LISTITEM_SIZE];
	format(string, sizeof (string), "DialogListitem_%i", listitem);
	GetPVarString(playerid, string, string, DIALOG_MAX_LISTITEM_SIZE);

	static dest[(DIALOG_MAX_LISTITEM_SIZE / 2)];

	new type;
	new pos;
	for (new i; i < DIALOG_MAX_LISTITEM_SIZE; i++)
	{
	    if (string[i] == '|' || i == (DIALOG_MAX_LISTITEM_SIZE - 1))
	    {
	        strmid(dest, string, pos, i);
	        pos = (i + 1);

	        switch (type)
	        {
	            case 0: model = strval(dest);
	            case 1: format(label, maxlength, dest);
	            case 2: x = floatstr(dest);
	            case 3: y = floatstr(dest);
	            case 4: z = floatstr(dest);
	            case 5: zoom = floatstr(dest);
	            case 6: color1 = strval(dest);
	            case 7: color2 = strval(dest);
	        }
	        type++;
	    }
	}
}

static Dialog_SetListitem(playerid, listitem, model, label[], Float:x, Float:y, Float:z, Float:zoom, color1, color2)
{
	static string[DIALOG_MAX_LISTITEM_SIZE];
	format(string, DIALOG_MAX_LISTITEM_SIZE, "%i|%s|%f|%f|%f|%f|%i|%i", model, label, x, y, z, zoom, color1, color2);

	static varname[45];
	format(varname, sizeof (varname), "DialogListitem_%i", listitem);
	SetPVarString(playerid, varname, string);
}

static Dialog_SetPage(playerid, page)
{
	p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEM_ID] = 0;

	switch (p_Dialog[playerid][E_PLAYER_DIALOG_STYLE])
	{
		case DIALOG_STYLE_PREVMODEL:
		{
			PlayerTextDrawDestroy(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_SCROLL]]);
			p_DialogTextDrawPool[playerid]--;

			new pages = Dialog_CountPages(p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS], 24);
			if (pages > 1)
			{
				new Float:whiles = ((145.500000 + 16.200000) / pages);
				p_DialogCompnent[playerid][E_DIALOG_COMPONENT_SCROLL] = Dialog_Create(playerid, 455.000000, (144.000000 + (whiles * page)), "_", 0, 1, 0.000000, (whiles / 16.200000), 0, .usebox = 1, .boxcolor = Dialog_SetOpacity(0xD15454FF, 200), .textsizeX = 458.000000, .textsizeY = 0.000000, .show = 1);
	  		}

	  		new listitem, index, model, label[DIALOG_MAX_LISTITEM_SIZE / 2], Float:x, Float:y, Float:z, Float:zoom, color1, color2;
			for (new i; i < 24; i++)
			{
			    listitem = ((page * 24) + i);
			    if (listitem >= p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS])
			    {
					PlayerTextDrawHide(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]]);
					PlayerTextDrawHide(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_LABEL][index]]);

					index++;
				    continue;
				}

				Dialog_GetListitem(playerid, listitem, model, label, x, y, z, zoom, color1, color2);
				if (model < 0 || model > 20000)
				 	continue;

				PlayerTextDrawBackgroundColor(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]], Dialog_SetOpacity(0x3B3B3BFF, 150));
				PlayerTextDrawSetPreviewModel(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]], model);
				PlayerTextDrawSetPreviewRot(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]], x, y, z, zoom);
				PlayerTextDrawSetPreviewVehCol(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]], color1, color2);
			    PlayerTextDrawShow(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]]);

				if (label[0])
				{
					PlayerTextDrawSetString(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_LABEL][index]], label);
				    PlayerTextDrawShow(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_LABEL][index]]);
				}

				index++;
			}

			PlayerTextDrawBackgroundColor(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][0]], Dialog_SetOpacity(0xFF5959FF, 150));
			PlayerTextDrawShow(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][0]]);
		}

		case DIALOG_STYLE_PREVMODEL_LIST:
		{
			PlayerTextDrawDestroy(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_SCROLL]]);
			p_DialogTextDrawPool[playerid]--;

			new pages = Dialog_CountPages(p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS], 5);
			if (pages > 1)
			{
				new Float:whiles = ((145.500000 + 16.200000) / pages);
				p_DialogCompnent[playerid][E_DIALOG_COMPONENT_SCROLL] = Dialog_Create(playerid, 455.000000, (144.000000 + (whiles * page)), "_", 0, 1, 0.000000, (whiles / 16.200000), 0, .usebox = 1, .boxcolor = Dialog_SetOpacity(0xD15454FF, 200), .textsizeX = 458.000000, .textsizeY = 0.000000, .show = 1);
	  		}

	  		new listitem, index, model, label[DIALOG_MAX_LISTITEM_SIZE / 2], Float:x, Float:y, Float:z, Float:zoom, color1, color2;
			for (new i; i < 5; i++)
			{
			    listitem = ((page * 5) + i);
			    if (listitem >= p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS])
			    {
					PlayerTextDrawHide(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BAR][index]]);
					PlayerTextDrawHide(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]]);
					PlayerTextDrawHide(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_LABEL][index]]);

					index++;
				    continue;
				}

				Dialog_GetListitem(playerid, listitem, model, label, x, y, z, zoom, color1, color2);
				if (model < 0 || model > 20000)
				 	continue;

                PlayerTextDrawColor(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BAR][index]], Dialog_SetOpacity(0x788FB0FF, 200));
			    PlayerTextDrawShow(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BAR][index]]);

				PlayerTextDrawSetPreviewModel(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]], model);
				PlayerTextDrawSetPreviewRot(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]], x, y, z, zoom);
				PlayerTextDrawSetPreviewVehCol(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]], color1, color2);
			    PlayerTextDrawShow(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]]);

				if (label[0])
				{
					PlayerTextDrawSetString(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_LABEL][index]], label);
				    PlayerTextDrawShow(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_LABEL][index]]);
				}

				index++;
			}

			PlayerTextDrawColor(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BAR][0]], Dialog_SetOpacity(0xFF5959FF, 200));
			PlayerTextDrawShow(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BAR][0]]);
		}
  	}
}

forward Dialog_HideForPlayer(playerid);
public  Dialog_HideForPlayer(playerid)
{
	switch (p_Dialog[playerid][E_PLAYER_DIALOG_STYLE])
	{
		case DIALOG_STYLE_MSGBOX..DIALOG_STYLE_TABLIST_HEADERS:
		{
		    ShowPlayerDialog(playerid, -1, 0, "", "", "", "");
		}

		case DIALOG_STYLE_PREVMODEL,
			 DIALOG_STYLE_PREVMODEL_LIST:
		{
			Dialog_Exit(playerid);

			p_Dialog[playerid][E_PLAYER_DIALOG_CANCELED] = true;
			CancelSelectTextDraw(playerid);
		}
	}

	p_Dialog[playerid][E_PLAYER_DIALOG_ID] = -1;
	p_Dialog[playerid][E_PLAYER_DIALOG_STYLE] = -1;
	p_Dialog[playerid][E_PLAYER_DIALOG_DUAL_BTN] = false;
	p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS] = 0;
	p_Dialog[playerid][E_PLAYER_DIALOG_PAGE] = 0;
	p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEM_ID] = 0;
	p_Dialog[playerid][E_PLAYER_DIALOG_CLICK_TICKCOUNT] = 0;
}

stock Dialog_ShowToPlayer(playerid, dialogid, style, caption[], info[], button1[], button2[])
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	CallRemoteFunction("Dialog_HideForPlayer", "i", playerid);

	if (dialogid < 0)
	{
		p_Dialog[playerid][E_PLAYER_DIALOG_ID] = -1;
		p_Dialog[playerid][E_PLAYER_DIALOG_STYLE] = -1;
	    return 1;
	}
	
	p_Dialog[playerid][E_PLAYER_DIALOG_ID] = dialogid;
	p_Dialog[playerid][E_PLAYER_DIALOG_STYLE] = style;

	switch (style)
	{
		case DIALOG_STYLE_PREVMODEL,
			 DIALOG_STYLE_PREVMODEL_LIST:
		{
			Dialog_Init(playerid, style, caption, button1, button2);

			new pos;
			static str[5 + DIALOG_MAX_LISTITEM_SIZE];
			static str_model[5];
			static str_label[DIALOG_MAX_LISTITEM_SIZE / 2];

            for (new i, j = strlen(info); i < j; i++)
			{
			    if (info[i] == '\n')
			    {
					strmid(str, info, pos, i);
					pos = (i + 1);
				}
				else if (i == (j - 1))
				{
					strmid(str, info, pos, j);
				}
				else continue;

				for (new x, y = strlen(str); x < y; x++)
				{
				    if (str[x] == '\t')
				    {
				        strmid(str_model, str, 0, x);
     					strmid(str_label, str, (x + 1), y);
     					break;
				    }
				    else if (x == (y - 1))
				    {
				        strmid(str_model, str, 0, y);
				        str_label[0] = EOS;
     					break;
				    }
				}

				Dialog_SetListitem(playerid, (p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS]++), strval(str_model), str_label, 0.0, 0.0, 0.0, 1.0, -1, -1);
			}

			p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEM_ID] = 0;
			p_Dialog[playerid][E_PLAYER_DIALOG_PAGE] = 0;

            Dialog_SetPage(playerid, 0);
			SelectTextDraw(playerid, Dialog_SetOpacity(0xD15454FF, 200));
		}

		default:
			ShowPlayerDialog(playerid, dialogid, style, caption, info, button1, button2);
	}
	
	return 1;
}
#if defined _ALS_ShowPlayerDialog
	#undef ShowPlayerDialog
#else
	#define _ALS_ShowPlayerDialog
#endif
#define ShowPlayerDialog Dialog_ShowToPlayer

stock ShowPlayerPreviewModelDialog(playerid, dialogid, style, caption[], models[], labels[][], button1[], button2[], limit = sizeof (models), limit2 = sizeof (labels))
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	CallRemoteFunction("Dialog_HideForPlayer", "i", playerid);

	if (dialogid < 0)
		return 1;

	switch (style)
	{
		case DIALOG_STYLE_PREVMODEL,
			 DIALOG_STYLE_PREVMODEL_LIST:
		{
		}

		default:
			return 0;
	}

	p_Dialog[playerid][E_PLAYER_DIALOG_ID] = dialogid;
	p_Dialog[playerid][E_PLAYER_DIALOG_STYLE] = style;

	Dialog_Init(playerid, style, caption, button1, button2);

 	p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS]= limit;
 	for (new i; i < limit; i++)
 		Dialog_SetListitem(playerid, i, models[i], ((i >= limit2) ? ("") : (labels[i])), 0.0, 0.0, 0.0, 1.0, -1, -1);

	p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEM_ID] = 0;
	p_Dialog[playerid][E_PLAYER_DIALOG_PAGE] = 0;

	Dialog_SetPage(playerid, 0);
	SelectTextDraw(playerid, Dialog_SetOpacity(0xD15454FF, 200));

	return true;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if (! p_Dialog[playerid][E_PLAYER_DIALOG_CANCELED])
	{
		switch (p_Dialog[playerid][E_PLAYER_DIALOG_STYLE])
		{
		    case DIALOG_STYLE_PREVMODEL,
				 DIALOG_STYLE_PREVMODEL_LIST:
			{
				if (clickedid == Text:INVALID_TEXT_DRAW)
				{
					Dialog_Exit(playerid);

					new dialogid = p_Dialog[playerid][E_PLAYER_DIALOG_ID];
					new listitem = p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEM_ID];
					
					p_Dialog[playerid][E_PLAYER_DIALOG_ID] = -1;
					p_Dialog[playerid][E_PLAYER_DIALOG_STYLE] = -1;
					p_Dialog[playerid][E_PLAYER_DIALOG_DUAL_BTN] = false;
					p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS] = 0;
					p_Dialog[playerid][E_PLAYER_DIALOG_PAGE] = 0;
					p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEM_ID] = 0;
					p_Dialog[playerid][E_PLAYER_DIALOG_CLICK_TICKCOUNT] = 0;
					
	    			if (funcidx("OnDialogResponse") != -1)
						CallRemoteFunction("OnDialogResponse", "iiiis", playerid, dialogid, 0, listitem, "\1");
				}

				return 1;
			}
		}
	}

	p_Dialog[playerid][E_PLAYER_DIALOG_CANCELED] = false;

	#if defined Dialog_OnPlayerClickTextDraw
       	return Dialog_OnPlayerClickTextDraw(playerid, clickedid);
	#else
	   	return 0;
	#endif
}
#if defined _ALS_OnPlayerClickTextDraw
    #undef OnPlayerClickTextDraw
#else
    #define _ALS_OnPlayerClickTextDraw
#endif
#define OnPlayerClickTextDraw Dialog_OnPlayerClickTextDraw
#if defined Dialog_OnPlayerClickTextDraw
    forward Dialog_OnPlayerClickTextDraw(playerid, Text:clickedid);
#endif

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	switch (p_Dialog[playerid][E_PLAYER_DIALOG_STYLE])
	{
	    case DIALOG_STYLE_PREVMODEL,
			 DIALOG_STYLE_PREVMODEL_LIST:
		{
			if (playertextid == p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BUTTON2]])
			{
    			Dialog_Exit(playerid);
				p_Dialog[playerid][E_PLAYER_DIALOG_CANCELED] = true;
				CancelSelectTextDraw(playerid);

				new dialogid = p_Dialog[playerid][E_PLAYER_DIALOG_ID];
				new listitem = p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEM_ID];

				p_Dialog[playerid][E_PLAYER_DIALOG_ID] = -1;
				p_Dialog[playerid][E_PLAYER_DIALOG_STYLE] = -1;
				p_Dialog[playerid][E_PLAYER_DIALOG_DUAL_BTN] = false;
				p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS] = 0;
				p_Dialog[playerid][E_PLAYER_DIALOG_PAGE] = 0;
				p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEM_ID] = 0;
				p_Dialog[playerid][E_PLAYER_DIALOG_CLICK_TICKCOUNT] = 0;

	    		if (funcidx("OnDialogResponse") != -1)
					CallRemoteFunction("OnDialogResponse", "iiiis", playerid, dialogid, 0, listitem, "\1");
			}
			else if (playertextid == p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BUTTON1]] || playertextid == p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BUTTONC]])
			{
    			Dialog_Exit(playerid);
				p_Dialog[playerid][E_PLAYER_DIALOG_CANCELED] = true;
				CancelSelectTextDraw(playerid);

				new dialogid = p_Dialog[playerid][E_PLAYER_DIALOG_ID];
				new listitem = p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEM_ID];

				p_Dialog[playerid][E_PLAYER_DIALOG_ID] = -1;
				p_Dialog[playerid][E_PLAYER_DIALOG_STYLE] = -1;
				p_Dialog[playerid][E_PLAYER_DIALOG_DUAL_BTN] = false;
				p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS] = 0;
				p_Dialog[playerid][E_PLAYER_DIALOG_PAGE] = 0;
				p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEM_ID] = 0;
				p_Dialog[playerid][E_PLAYER_DIALOG_CLICK_TICKCOUNT] = 0;
				
				if (funcidx("OnDialogResponse") != -1)
					CallRemoteFunction("OnDialogResponse", "iiiis", playerid, dialogid, 1, listitem, "\1");
			}
			else if (playertextid == p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_SCROLL_UP]])
			{
				if ((p_Dialog[playerid][E_PLAYER_DIALOG_PAGE] - 1) < 0)
					PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);
				else
					Dialog_SetPage(playerid, (--p_Dialog[playerid][E_PLAYER_DIALOG_PAGE]));
			}
			else if (playertextid == p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_SCROLL_DOWN]])
			{
				if ((p_Dialog[playerid][E_PLAYER_DIALOG_PAGE] + 1) >= Dialog_CountPages(p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS], ((p_Dialog[playerid][E_PLAYER_DIALOG_STYLE] == DIALOG_STYLE_PREVMODEL) ? (24) : (5))))
					PlayerPlaySound(playerid, 1084, 0.0, 0.0, 0.0);
				else
					Dialog_SetPage(playerid, (++p_Dialog[playerid][E_PLAYER_DIALOG_PAGE]));
			}
	        else
	        {
    			new index = -1;
			    switch (p_Dialog[playerid][E_PLAYER_DIALOG_STYLE])
				{
				    case DIALOG_STYLE_PREVMODEL:
				    {
						new listitem;
						for (new i; i < 24; i++)
						{
						    listitem = ((p_Dialog[playerid][E_PLAYER_DIALOG_PAGE] * 24) + i);
							if (listitem >= p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS])
							    break;

							if (playertextid == p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][i]])
							{
							 	p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEM_ID] = listitem;
								index = i;
								break;
							}
						}
					}

				    case DIALOG_STYLE_PREVMODEL_LIST:
				    {
						new listitem;
						for (new i; i < 5; i++)
						{
						    listitem = ((p_Dialog[playerid][E_PLAYER_DIALOG_PAGE] * 5) + i);
							if (listitem >= p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS])
							    break;

							if (playertextid == p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BAR][i]])
							{
							 	p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEM_ID] = listitem;
								index = i;
								break;
							}
						}
					}
				}

				if (index != -1)
				{
					if (GetTickCount() - p_Dialog[playerid][E_PLAYER_DIALOG_CLICK_TICKCOUNT] <= 200)
					{
						Dialog_Exit(playerid);
						p_Dialog[playerid][E_PLAYER_DIALOG_CANCELED] = true;
						CancelSelectTextDraw(playerid);

						new dialogid = p_Dialog[playerid][E_PLAYER_DIALOG_ID];
						new listitem = p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEM_ID];

						p_Dialog[playerid][E_PLAYER_DIALOG_ID] = -1;
						p_Dialog[playerid][E_PLAYER_DIALOG_STYLE] = -1;
						p_Dialog[playerid][E_PLAYER_DIALOG_DUAL_BTN] = false;
						p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS] = 0;
						p_Dialog[playerid][E_PLAYER_DIALOG_PAGE] = 0;
						p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEM_ID] = 0;
						p_Dialog[playerid][E_PLAYER_DIALOG_CLICK_TICKCOUNT] = 0;

						if (funcidx("OnDialogResponse") != -1)
							CallRemoteFunction("OnDialogResponse", "iiiis", playerid, dialogid, 1, listitem, "\1");
					}
					else
					{
						switch (p_Dialog[playerid][E_PLAYER_DIALOG_STYLE])
						{
						    case DIALOG_STYLE_PREVMODEL:
						    {
							    for (new i; i < 24; i++)
								{
									if (((p_Dialog[playerid][E_PLAYER_DIALOG_PAGE] * 24) + i) >= p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS])
									    break;

		                            PlayerTextDrawBackgroundColor(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][i]], Dialog_SetOpacity(0x3B3B3BFF, 150));
		                            PlayerTextDrawShow(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][i]]);
								}
								PlayerTextDrawBackgroundColor(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]], Dialog_SetOpacity(0xFF5959FF, 150));
								PlayerTextDrawShow(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]]);
						    }

						    case DIALOG_STYLE_PREVMODEL_LIST:
						    {
							    for (new i; i < 5; i++)
								{
									if (((p_Dialog[playerid][E_PLAYER_DIALOG_PAGE] * 5) + i) >= p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS])
									    break;

		                            PlayerTextDrawColor(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BAR][i]], Dialog_SetOpacity(0x788FB0FF, 200));
		                            PlayerTextDrawShow(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BAR][i]]);
								}
								PlayerTextDrawColor(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BAR][index]], Dialog_SetOpacity(0xFF5959FF, 200));
								PlayerTextDrawShow(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_BAR][index]]);
						    }
						}

						p_Dialog[playerid][E_PLAYER_DIALOG_CLICK_TICKCOUNT] = GetTickCount();
					}
				}
			}

			return 1;
		}
	}

    #if defined Dialog_OnPlayerClickPlayerTD
       	return Dialog_OnPlayerClickPlayerTD(playerid, playertextid);
	#else
	   	return 0;
	#endif
}
#if defined _ALS_OnPlayerClickPlayerTD
    #undef OnPlayerClickPlayerTextDraw
#else
    #define _ALS_OnPlayerClickPlayerTD
#endif
#define OnPlayerClickPlayerTextDraw Dialog_OnPlayerClickPlayerTD
#if defined Dialog_OnPlayerClickPlayerTD
    forward Dialog_OnPlayerClickPlayerTD(playerid, PlayerText:playertextid);
#endif

stock Dialog_SetModelRot(playerid, listitem, Float:x = 0.0, Float:y = 0.0, Float:z = 0.0, Float:zoom = 1.0, color1 = -1, color2 = -1)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
		return 0;

	if (p_Dialog[playerid][E_PLAYER_DIALOG_STYLE] == -1)
		return 0;

  	new index;
    switch (p_Dialog[playerid][E_PLAYER_DIALOG_STYLE])
	{
		case DIALOG_STYLE_PREVMODEL,
	      	 DIALOG_STYLE_PREVMODEL_LIST:
		{
		    if (listitem >= p_Dialog[playerid][E_PLAYER_DIALOG_LISTITEMS])
		    	return 0;

			if (p_Dialog[playerid][E_PLAYER_DIALOG_STYLE] == DIALOG_STYLE_PREVMODEL)
				index = listitem - (floatround(Float:(listitem - 24), floatround_floor) * 24);
			else
				index = listitem - (floatround(Float:(listitem - 5), floatround_floor) * 5);

			new model, label[DIALOG_MAX_LISTITEM_SIZE], unused;
            Dialog_GetListitem(playerid, listitem, model, label, Float:unused, Float:unused, Float:unused, Float:unused, unused, unused);
            Dialog_SetListitem(playerid, listitem, model, label, x, y, z, zoom, color1, color2);

			PlayerTextDrawSetPreviewRot(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]], x, y, z, zoom);
			PlayerTextDrawSetPreviewVehCol(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]], color1, color2);
			PlayerTextDrawShow(playerid, p_DialogTextDraw[playerid][p_DialogCompnent[playerid][E_DIALOG_COMPONENT_MODEL][index]]);
		}

		default:
		    return 0;
	}

	return 1;
}
