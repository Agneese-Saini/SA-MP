#if defined _dialogs_included
	#endinput
#endif
#define _dialogs_included

// PreviewModelDialog.inc - v3.0.2
// By Gammix

#define DIALOG_STYLE_PREVIEW_MODEL \
	6
#define DIALOG_STYLE_PREVMODEL \
	DIALOG_STYLE_PREVIEW_MODEL
	
#define MAX_DIALOG_PLAYER_TEXT_DRAWS \
	100

#define MAX_DIALOG_LIST_ROWS \
	4

#define MAX_DIALOG_LIST_COLUMNS \
	8
	
#define MAX_SMALL_DIALOG_LIST_ROWS \
	3
	
#define MAX_SMALL_DIALOG_LIST_COLUMNS \
	4

#if !defined MAX_DIALOG_LIST_ITEMS
	#define MAX_DIALOG_LIST_ITEMS \
		256 // number of listitems a DIALOG_STYLE_PREVMODEL can have
#endif

#if !defined MAX_DIALOG_LIST_ITEM_SIZE
	#define MAX_DIALOG_LIST_ITEM_SIZE \
		64 // each listitem label string size (-6 bytes for model id and '\n')
#endif

#if !defined DIALOG_LIST_ITEM_COLOR
	#define DIALOG_LIST_ITEM_COLOR \
		5864548 // each preview model listitem background color (unselected)
#endif

#if !defined DIALOG_SELECTED_LIST_ITEM_COLOR
	#define DIALOG_SELECTED_LIST_ITEM_COLOR \
		5864548 // color of selected(clicked) preview model listitem background
#endif

#define NumOfPages(%0,%1) \
	(((%0) - 1) / (%1) + 1)

enum DIALOG_TEXTDRAW
{
    DIALOG_TEXTDRAW_SCROLL_BAR,
    DIALOG_TEXTDRAW_SCROLL_UP,
    DIALOG_TEXTDRAW_SCROLL_DOWN,
    DIALOG_TEXTDRAW_LEFT_BUTTON,
    DIALOG_TEXTDRAW_RIGHT_BUTTON,
    DIALOG_TEXTDRAW_MIDDLE_BUTTON,
    DIALOG_TEXTDRAW_MODEL[MAX_DIALOG_LIST_ROWS * MAX_DIALOG_LIST_COLUMNS],
    DIALOG_TEXTDRAW_TEXT[MAX_DIALOG_LIST_ROWS * MAX_DIALOG_LIST_COLUMNS]
};

static playerDialogTextDrawID[MAX_PLAYERS][DIALOG_TEXTDRAW];

static PlayerText:playerDialogTextDraw[MAX_PLAYERS][MAX_DIALOG_PLAYER_TEXT_DRAWS];
static playerDialogTextDrawCount[MAX_PLAYERS];

static playerDialogID[MAX_PLAYERS];
static playerDialogStyle[MAX_PLAYERS];
static playerDialogListitem[MAX_PLAYERS];
static playerDialogPage[MAX_PLAYERS];

static playerDialogTotalListitems[MAX_PLAYERS];
static playerDialogModel[MAX_PLAYERS][MAX_DIALOG_LIST_ITEMS];
static playerDialogText[MAX_PLAYERS][MAX_DIALOG_LIST_ITEMS][MAX_DIALOG_LIST_ITEM_SIZE char];
static playerDialogListitemModelRot[MAX_PLAYERS][MAX_DIALOG_LIST_ITEMS][6];

static playerDialogClickTickCount[MAX_PLAYERS][MAX_DIALOG_LIST_ROWS * MAX_DIALOG_LIST_COLUMNS];

static Dialog_ScrollBar(page, totalPages, &Float:y, Float:maxHeight, &Float:height)
{
	height = maxHeight / totalPages;
	y += (height * 7.4) * page;
}

static Dialog_CreatePlayerDialog(playerid, caption[], button1[], button2[], bool:smallVersion)
{
	if (smallVersion)
	{
	    // Header
	    playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,220.000000, 140.000000, caption);
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.159998, 1.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 421075455);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 410.000000, 0.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		// Background
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,220.000000, 151.000000, "_");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 20.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 150);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 410.000000, 0.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,223.000000, 156.000000, "_");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 16.400001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 407.000000, 0.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,224.000000, 157.000000, "_");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 16.200000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 406.000000, 0.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		// ScrollBar
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,401.000000, 156.000000, "_");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 16.400001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 399.000000, 0.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_SCROLL_UP] = playerDialogTextDrawCount[playerid];
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,400.000000, 155.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.500000, 1.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetShadow(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 7.000000, 9.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,401.000000, 156.000000, "LD_BEAT:up");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.500000, 1.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetShadow(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 5.000000, 6.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_SCROLL_DOWN] = playerDialogTextDrawCount[playerid];
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,400.000000, 296.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.500000, 1.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetShadow(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 7.000000, 9.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,401.000000, 298.000000, "LD_BEAT:down");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.500000, 1.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetShadow(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 5.000000, 6.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		// Listitems
		new text[MAX_DIALOG_LIST_ITEM_SIZE];
		for (new a; a < MAX_SMALL_DIALOG_LIST_ROWS; a++)
	    {
	        for (new b; b < MAX_SMALL_DIALOG_LIST_COLUMNS; b++)
	        {
	            if (((a * MAX_SMALL_DIALOG_LIST_COLUMNS) + b) >= playerDialogTotalListitems[playerid])
	            {
	                break;
	            }

				playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_MODEL][(a * MAX_SMALL_DIALOG_LIST_COLUMNS) + b] = playerDialogTextDrawCount[playerid];
				playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid, (225.000000 + (b * 43.000000)), (157.000000 + (a * 48.000000)), "_");
				PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], (((a * MAX_SMALL_DIALOG_LIST_COLUMNS) + b) == playerDialogListitem[playerid]) ? (DIALOG_SELECTED_LIST_ITEM_COLOR) : (DIALOG_LIST_ITEM_COLOR));
				PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 5);
				PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.500000, 1.000000);
				PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
				PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
				PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
				PlayerTextDrawSetShadow(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
				PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
				PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
				PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 42.000000, 47.000000);
				PlayerTextDrawSetPreviewModel(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], playerDialogModel[playerid][(a * MAX_SMALL_DIALOG_LIST_COLUMNS) + b]);
				PlayerTextDrawSetPreviewRot(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0, 0, 0, 1.0);
				PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
				PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

				if (playerDialogText[playerid][(a * MAX_SMALL_DIALOG_LIST_COLUMNS) + b]{0})
				{
					strunpack(text, playerDialogText[playerid][(a * MAX_SMALL_DIALOG_LIST_COLUMNS) + b]);
					playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_TEXT][(a * MAX_SMALL_DIALOG_LIST_COLUMNS) + b] = playerDialogTextDrawCount[playerid];
					playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid, (227.000000 + (b * 43.000000)), (160.000000 + (a * 48.000000)), text);
					PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
					PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
					PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.129997, 0.799997);
					PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
					PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
					PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
					PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
					PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
					PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], (265.000000 + (b * 43.000000)), 0.000000);
					PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
					PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);
				}
	        }
		}
	}
	else
	{
	    // Header
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,170.000000, 140.000000, caption);
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.159998, 1.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 421075455);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 470.000000, 0.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		// Background
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,170.000000, 151.000000, "_");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 20.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 150);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 470.000000, 0.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,173.000000, 156.000000, "_");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 16.400001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 467.000000, 0.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,174.000000, 157.000000, "_");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 16.200000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 466.000000, 0.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		// ScrollBar
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,461.000000, 156.000000, "_");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 16.400001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 459.000000, 0.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_SCROLL_BAR] = playerDialogTextDrawCount[playerid];
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,463.000000, 166.000000, "_");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 7.100001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1608903786);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 465.000000, 0.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++], 0);

		playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_SCROLL_UP] = playerDialogTextDrawCount[playerid];
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,460.000000, 155.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.500000, 1.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetShadow(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 7.000000, 9.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,461.000000, 156.000000, "LD_BEAT:up");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.500000, 1.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetShadow(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 5.000000, 6.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_SCROLL_DOWN] = playerDialogTextDrawCount[playerid];
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,460.000000, 296.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.500000, 1.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetShadow(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 7.000000, 9.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,461.000000, 298.000000, "LD_BEAT:down");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.500000, 1.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetShadow(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 5.000000, 6.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

        // Listitems
		for (new a; a < MAX_DIALOG_LIST_ROWS; a++)
	    {
	        for (new b; b < MAX_DIALOG_LIST_COLUMNS; b++)
	        {
				playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_MODEL][(a * MAX_DIALOG_LIST_COLUMNS) + b] = playerDialogTextDrawCount[playerid];
				playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid, (175.000000 + (b * 35.000000)), (156.000000 + (a * 37.000000)), "_");
				PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1608903836);
				PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 5);
				PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.500000, 1.000000);
				PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
				PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
				PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
				PlayerTextDrawSetShadow(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
				PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
				PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
				PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 34.000000, 36.000000);
				PlayerTextDrawSetPreviewModel(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
				PlayerTextDrawSetPreviewRot(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0, 0, 0, 1.0);
				PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++], 1);

				playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_TEXT][(a * MAX_DIALOG_LIST_COLUMNS) + b] = playerDialogTextDrawCount[playerid];
				playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid, (177.000000 + (b * 35.000000)), (160.000000 + (a * 37.000000)), "");
				PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
				PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
				PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.129997, 0.799997);
				PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
				PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
				PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
				PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
				PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
				PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], (207.000000 + (b * 35.000000)), 0.000000);
				PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++], 0);
	        }
		}
	}

	if (button2[0])
	{
		// Left button
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,249.000000, 309.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 15.000000, 18.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,250.000000, 310.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 13.000000, 16.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,297.000000, 309.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 15.000000, 18.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,298.000000, 310.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 13.000000, 16.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,257.000000, 309.250000, "LD_SPAC:white");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 48.000000, 17.250000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_LEFT_BUTTON] = playerDialogTextDrawCount[playerid];
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,257.000000, 310.000000, "LD_SPAC:white");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 48.000000, 16.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,281.000000, 313.000000, button1);
		PlayerTextDrawAlignment(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 2);
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.159997, 1.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		// Right Button
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,329.000000, 309.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 15.000000, 18.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,330.000000, 310.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 13.000000, 16.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,377.000000, 309.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 15.000000, 18.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,378.000000, 310.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 13.000000, 16.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,337.000000, 309.250000, "LD_SPAC:white");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 48.000000, 17.250000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_RIGHT_BUTTON] = playerDialogTextDrawCount[playerid];
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,337.000000, 310.000000, "LD_SPAC:white");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 48.000000, 16.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,361.000000, 313.000000, button2);
		PlayerTextDrawAlignment(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 2);
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.159997, 1.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);
	}
	else
	{
		// Middle button
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,289.000000, 309.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 15.000000, 18.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,290.000000, 310.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 13.000000, 16.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,337.000000, 309.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 15.000000, 18.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,338.000000, 310.000000, "LD_POOL:ball");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 13.000000, 16.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,297.000000, 309.250000, "LD_SPAC:white");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -926365496);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 48.000000, 17.250000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_MIDDLE_BUTTON] = playerDialogTextDrawCount[playerid];
		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,297.000000, 310.000000, "LD_SPAC:white");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 4);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.000000, 1.200001);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 255);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 48.000000, 16.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);

		playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]] = CreatePlayerTextDraw(playerid,321.000000, 313.000000, button1);
		PlayerTextDrawAlignment(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 2);
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0.159997, 1.000000);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 1);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]], 0);
		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawCount[playerid]++]);
	}
}

static Dialog_UpdatePlayerDialog(playerid)
{
	if (playerDialogTotalListitems[playerid] <= (MAX_SMALL_DIALOG_LIST_ROWS * MAX_SMALL_DIALOG_LIST_COLUMNS))
	{
	    return 0;
	}

	new index;
	new pages = NumOfPages(playerDialogTotalListitems[playerid], (MAX_DIALOG_LIST_ROWS * MAX_DIALOG_LIST_COLUMNS));
	if (pages > 1)
	{
	    index = playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_SCROLL_BAR];
	    PlayerTextDrawDestroy(playerid, playerDialogTextDraw[playerid][index]);

	    new Float:y = 166.000000;
	    new Float:height;
	    Dialog_ScrollBar(playerDialogPage[playerid], pages, y, 16.400001, height);

	    playerDialogTextDraw[playerid][index] = CreatePlayerTextDraw(playerid,463.000000, y, "_");
		PlayerTextDrawBackgroundColor(playerid,playerDialogTextDraw[playerid][index], 0);
		PlayerTextDrawFont(playerid,playerDialogTextDraw[playerid][index], 1);
		PlayerTextDrawLetterSize(playerid,playerDialogTextDraw[playerid][index], 0.000000, height);
		PlayerTextDrawColor(playerid,playerDialogTextDraw[playerid][index], -1);
		PlayerTextDrawSetOutline(playerid,playerDialogTextDraw[playerid][index], 1);
		PlayerTextDrawSetProportional(playerid,playerDialogTextDraw[playerid][index], 1);
		PlayerTextDrawUseBox(playerid,playerDialogTextDraw[playerid][index], 1);
		PlayerTextDrawBoxColor(playerid,playerDialogTextDraw[playerid][index], -1608903786);
		PlayerTextDrawTextSize(playerid,playerDialogTextDraw[playerid][index], 465.000000, 0.000000);
		PlayerTextDrawSetSelectable(playerid,playerDialogTextDraw[playerid][index], 0);

		PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][index]);
	}
	else
	{
	    index = playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_SCROLL_BAR];
	    PlayerTextDrawDestroy(playerid, playerDialogTextDraw[playerid][index]);
	}

	new itemid;
	new text[MAX_DIALOG_LIST_ITEM_SIZE];
    for (new a; a < MAX_DIALOG_LIST_ROWS; a++)
    {
    	for (new b; b < MAX_DIALOG_LIST_COLUMNS; b++)
     	{
			itemid = (((a * MAX_DIALOG_LIST_COLUMNS) + b) + (playerDialogPage[playerid] * (MAX_DIALOG_LIST_COLUMNS * MAX_DIALOG_LIST_ROWS)));

			index = playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_MODEL][(a * MAX_DIALOG_LIST_COLUMNS) + b];
			if (itemid >= playerDialogTotalListitems[playerid])
			{
			    PlayerTextDrawHide(playerid, playerDialogTextDraw[playerid][index]);
			}
			else
			{
				PlayerTextDrawSetPreviewModel(playerid, playerDialogTextDraw[playerid][index], playerDialogModel[playerid][itemid]);
				PlayerTextDrawSetPreviewRot(playerid, playerDialogTextDraw[playerid][index], Float:playerDialogListitemModelRot[playerid][itemid][0], Float:playerDialogListitemModelRot[playerid][itemid][1], Float:playerDialogListitemModelRot[playerid][itemid][2], Float:playerDialogListitemModelRot[playerid][itemid][3]);
				PlayerTextDrawSetPreviewVehCol(playerid, playerDialogTextDraw[playerid][index], playerDialogListitemModelRot[playerid][itemid][4], playerDialogListitemModelRot[playerid][itemid][5]);
				PlayerTextDrawBackgroundColor(playerid, playerDialogTextDraw[playerid][index], (playerDialogListitem[playerid] == ((a * MAX_DIALOG_LIST_COLUMNS) + b)) ? (DIALOG_SELECTED_LIST_ITEM_COLOR) : (DIALOG_LIST_ITEM_COLOR));
				PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][index]);
			}

			index = playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_TEXT][(a * MAX_DIALOG_LIST_COLUMNS) + b];
			if (itemid >= playerDialogTotalListitems[playerid])
			{
			    PlayerTextDrawHide(playerid, playerDialogTextDraw[playerid][index]);
			}
			else
			{
			    if (playerDialogText[playerid][itemid]{0})
			    {
					strunpack(text, playerDialogText[playerid][itemid]);
					PlayerTextDrawSetString(playerid, playerDialogTextDraw[playerid][index], text);
				    PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][index]);
				}
				else
				{
				    PlayerTextDrawHide(playerid, playerDialogTextDraw[playerid][index]);
				}
			}
		}
	}
	return 1;
}

static Dialog_HidePlayerDialog(playerid)
{
	for (new i; i < playerDialogTextDrawCount[playerid]; i++)
	{
	    PlayerTextDrawDestroy(playerid, playerDialogTextDraw[playerid][i]);
	}
	playerDialogTextDrawCount[playerid] = 0;

	playerDialogID[playerid] = -1;
	playerDialogStyle[playerid] = -1;
	playerDialogListitem[playerid] = 0;
	playerDialogPage[playerid] = 0;
	playerDialogTotalListitems[playerid] = 0;
	return 1;
}

stock Dialog_ShowPlayerDialog(playerid, dialogid, style, caption[], info[], button1[], button2[])
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (playerDialogID[playerid] >= 0 && playerDialogStyle[playerid] == DIALOG_STYLE_PREVIEW_MODEL)
	{
		Dialog_HidePlayerDialog(playerid);
  	}

	if (dialogid < 0)
	{
	    if (style == DIALOG_STYLE_PREVIEW_MODEL)
	    {
	        return 1;
	    }

	    return ShowPlayerDialog(playerid, dialogid, style, caption, info, button1, button2);
	}

	if (style != DIALOG_STYLE_PREVIEW_MODEL)
	{
	    return ShowPlayerDialog(playerid, dialogid, style, caption, info, button1, button2);
	}

	new pos;
	new pos2;
	new modelid[6];
	new index;
	new string[sizeof modelid/*modelid*/ + 1/*\n*/ + MAX_DIALOG_LIST_ITEM_SIZE/*text*/];
	for (new i, len = strlen(info); i < len; i++)
	{
		if (info[i] == '\n')
		{
			strmid(string, info, pos, i);
			pos = i + 1;
		}
		else if (i == (len - 1))
		{
			strmid(string, info, pos, len);
		}
		else
		{
		    continue;
		}

		if (playerDialogTotalListitems[playerid] == MAX_DIALOG_LIST_ITEMS)
  		{
    		printf("[PrevModelDialog.inc] - Warning: You have more than \"MAX_DIALOG_LIST_ITEMS\" (i.e. "#MAX_DIALOG_LIST_ITEMS" listitems), only the items under the limit were loaded.");
			break;
   		}
   		index = playerDialogTotalListitems[playerid]++;

		playerDialogText[playerid][index]{0} = EOS;
		pos2 = strfind(string, "\t", true);
		if (pos2 == -1)
		{
		    playerDialogModel[playerid][index] = strval(string);
		}
		else
		{
		    strmid(modelid, string, 0, pos2);
		    playerDialogModel[playerid][index] = strval(modelid);
		    strpack(playerDialogText[playerid][index], string[pos2 + 1], MAX_DIALOG_LIST_ITEM_SIZE);
		}
	}

	if (playerDialogTotalListitems[playerid] == 0)
	{
	    return 0;
	}

	Dialog_CreatePlayerDialog(playerid, caption, button1, button2, bool:(playerDialogTotalListitems[playerid] <= (MAX_SMALL_DIALOG_LIST_ROWS * MAX_SMALL_DIALOG_LIST_COLUMNS)));
	Dialog_UpdatePlayerDialog(playerid);

	playerDialogID[playerid] = dialogid;
	playerDialogStyle[playerid] = style;
	playerDialogListitem[playerid] = 0;
	playerDialogPage[playerid] = 0;

	return SelectTextDraw(playerid, DIALOG_SELECTED_LIST_ITEM_COLOR);
}
#if defined _ALS_ShowPlayerDialog
    #undef ShowPlayerDialog
#else
    #define _ALS_ShowPlayerDialog
#endif
#define ShowPlayerDialog Dialog_ShowPlayerDialog

/*
native Dialog_ShowPreviewModelDialog(playerid, dialogid, caption[], models[], labels[][], button1[], button2[]);
*/
stock Dialog_ShowPreviewModelDialog(playerid, dialogid, caption[], models[], labels[][], button1[], button2[], size = sizeof models)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (playerDialogID[playerid] >= 0 && playerDialogStyle[playerid] == DIALOG_STYLE_PREVIEW_MODEL)
	{
		Dialog_HidePlayerDialog(playerid);
  	}

	if (dialogid < 0)
	{
	    return 1;
	}

	new index;
	for (new i; i < size; i++)
	{
		if (playerDialogTotalListitems[playerid] == MAX_DIALOG_LIST_ITEMS)
  		{
    		printf("[PrevModelDialog.inc] - Warning: You have more than \"MAX_DIALOG_LIST_ITEMS\" (i.e. "#MAX_DIALOG_LIST_ITEMS" listitems), only the items under the limit were loaded.");
			break;
   		}
   		index = playerDialogTotalListitems[playerid]++;

		playerDialogModel[playerid][index] = models[i];
		strpack(playerDialogText[playerid][index], labels[i], MAX_DIALOG_LIST_ITEM_SIZE);
	}

	if (playerDialogTotalListitems[playerid] == 0)
	{
	    return 0;
	}

	Dialog_CreatePlayerDialog(playerid, caption, button1, button2, bool:(playerDialogTotalListitems[playerid] <= (MAX_SMALL_DIALOG_LIST_ROWS * MAX_SMALL_DIALOG_LIST_COLUMNS)));
    Dialog_UpdatePlayerDialog(playerid);

	playerDialogID[playerid] = dialogid;
	playerDialogStyle[playerid] = DIALOG_STYLE_PREVIEW_MODEL;
	playerDialogListitem[playerid] = 0;
	playerDialogPage[playerid] = 0;

	return SelectTextDraw(playerid, DIALOG_SELECTED_LIST_ITEM_COLOR);
}

#define ShowPlayerPreviewModelDialog \
    Dialog_ShowPreviewModelDialog

/*
native Dialog_SetListitemPreviewRot(playerid, listitem, Float:rx, Float:ry, Float:rz, Float:zoom, materialcolor1, materialcolor2);
*/
stock Dialog_SetListitemPreviewRot(playerid, listitem, Float:rx, Float:ry, Float:rz, Float:zoom, materialcolor1, materialcolor2)
{
	if (playerid < 0 || playerid >= MAX_PLAYERS)
	{
	    return 0;
	}

	if (listitem < 0 || listitem >= MAX_DIALOG_LIST_ITEMS)
	{
	    return 0;
	}

    playerDialogListitemModelRot[playerid][listitem][0] = _:rx;
    playerDialogListitemModelRot[playerid][listitem][1] = _:ry;
    playerDialogListitemModelRot[playerid][listitem][2] = _:rz;
    playerDialogListitemModelRot[playerid][listitem][3] = _:zoom;
    playerDialogListitemModelRot[playerid][listitem][4] = materialcolor1;
    playerDialogListitemModelRot[playerid][listitem][5] = materialcolor2;
	return 1;
}

#define Dialog_SetModelRot \
    Dialog_SetListitemPreviewRot

public OnPlayerConnect(playerid)
{
	for (new i; i < MAX_DIALOG_LIST_ITEMS; i++)
	{
		playerDialogListitemModelRot[playerid][i][0] = _:0.0;
		playerDialogListitemModelRot[playerid][i][1] = _:0.0;
		playerDialogListitemModelRot[playerid][i][2] = _:0.0;
		playerDialogListitemModelRot[playerid][i][3] = _:1.0;
	    playerDialogListitemModelRot[playerid][i][4] = random(255);
	    playerDialogListitemModelRot[playerid][i][5] = random(255);
    }

    #if defined GM_OnPlayerConnect
       	return GM_OnPlayerConnect(playerid);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect GM_OnPlayerConnect
#if defined GM_OnPlayerConnect
    forward GM_OnPlayerConnect(playerid);
#endif

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	if (playerDialogID[playerid] >= 0 && playerDialogStyle[playerid] == DIALOG_STYLE_PREVIEW_MODEL)
	{
	    if (clickedid == Text:INVALID_TEXT_DRAW)
	    {
			new dialogid = playerDialogID[playerid];

            new listitem;
			if (playerDialogTotalListitems[playerid] <= (MAX_SMALL_DIALOG_LIST_ROWS * MAX_SMALL_DIALOG_LIST_COLUMNS))
			{
			    listitem = playerDialogListitem[playerid];
			}
			else
			{
			    listitem = (playerDialogListitem[playerid] + (playerDialogPage[playerid] * (MAX_DIALOG_LIST_COLUMNS * MAX_DIALOG_LIST_ROWS)));
			}

			new inputtext[6];
			valstr(inputtext, playerDialogModel[playerid][listitem]);

            Dialog_HidePlayerDialog(playerid);
			CallLocalFunction("OnDialogResponse", "iiiis", playerid, dialogid, 0, listitem, inputtext);
	    	return 1;
	    }
	}

	#if defined GM_OnPlayerClickTextDraw
       	return GM_OnPlayerClickTextDraw(playerid, Text:clickedid);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerClickTextDraw
    #undef OnPlayerClickTextDraw
#else
    #define _ALS_OnPlayerClickTextDraw
#endif
#define OnPlayerClickTextDraw GM_OnPlayerClickTextDraw
#if defined GM_OnPlayerClickTextDraw
    forward GM_OnPlayerClickTextDraw(playerid, Text:clickedid);
#endif

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	if (playerDialogID[playerid] >= 0 && playerDialogStyle[playerid] == DIALOG_STYLE_PREVIEW_MODEL)
	{
	    if (playertextid == playerDialogTextDraw[playerid][playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_SCROLL_UP]])
	    {
			if (playerDialogTotalListitems[playerid] <= (MAX_SMALL_DIALOG_LIST_ROWS * MAX_SMALL_DIALOG_LIST_COLUMNS))
			{
			    return 1;
			}
			else
			{
			    if ((playerDialogPage[playerid] - 1) < 0)
			    {
			        return 1;
			    }
			}

			playerDialogPage[playerid]--;
			playerDialogListitem[playerid] = ((MAX_DIALOG_LIST_ROWS * MAX_DIALOG_LIST_COLUMNS) - 1);
			Dialog_UpdatePlayerDialog(playerid);
	    	return 1;
	    }

	    if (playertextid == playerDialogTextDraw[playerid][playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_SCROLL_DOWN]])
	    {
			if (playerDialogTotalListitems[playerid] <= (MAX_SMALL_DIALOG_LIST_ROWS * MAX_SMALL_DIALOG_LIST_COLUMNS))
			{
			    return 1;
			}
			else
			{
			    if ((playerDialogPage[playerid] + 1) >= NumOfPages(playerDialogTotalListitems[playerid], (MAX_DIALOG_LIST_ROWS * MAX_DIALOG_LIST_COLUMNS)))
			    {
			        return 1;
			    }
			}

			playerDialogPage[playerid]++;
			playerDialogListitem[playerid] = 0;
			Dialog_UpdatePlayerDialog(playerid);
	    	return 1;
	    }

	    if (playertextid == playerDialogTextDraw[playerid][playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_LEFT_BUTTON]] || playertextid == playerDialogTextDraw[playerid][playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_MIDDLE_BUTTON]])
	    {
			new dialogid = playerDialogID[playerid];

            new listitem;
			if (playerDialogTotalListitems[playerid] <= (MAX_SMALL_DIALOG_LIST_ROWS * MAX_SMALL_DIALOG_LIST_COLUMNS))
			{
			    listitem = playerDialogListitem[playerid];
			}
			else
			{
			    listitem = (playerDialogListitem[playerid] + (playerDialogPage[playerid] * (MAX_DIALOG_LIST_COLUMNS * MAX_DIALOG_LIST_ROWS)));
			}

			new inputtext[6];
			valstr(inputtext, playerDialogModel[playerid][listitem]);

            Dialog_HidePlayerDialog(playerid);
			CallLocalFunction("OnDialogResponse", "iiiis", playerid, dialogid, 1, listitem, inputtext);
	    	return CancelSelectTextDraw(playerid);
	    }

	    if (playertextid == playerDialogTextDraw[playerid][playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_RIGHT_BUTTON]])
	    {
			new dialogid = playerDialogID[playerid];

            new listitem;
			if (playerDialogTotalListitems[playerid] <= (MAX_SMALL_DIALOG_LIST_ROWS * MAX_SMALL_DIALOG_LIST_COLUMNS))
			{
			    listitem = playerDialogListitem[playerid];
			}
			else
			{
			    listitem = (playerDialogListitem[playerid] + (playerDialogPage[playerid] * (MAX_DIALOG_LIST_COLUMNS * MAX_DIALOG_LIST_ROWS)));
			}

			new inputtext[6];
			valstr(inputtext, playerDialogModel[playerid][listitem]);

            Dialog_HidePlayerDialog(playerid);
			CallLocalFunction("OnDialogResponse", "iiiis", playerid, dialogid, 0, listitem, inputtext);
	    	return CancelSelectTextDraw(playerid);
	    }

	    for (new i, j = (playerDialogTotalListitems[playerid] <= (MAX_SMALL_DIALOG_LIST_ROWS * MAX_SMALL_DIALOG_LIST_COLUMNS)) ? (MAX_SMALL_DIALOG_LIST_ROWS * MAX_SMALL_DIALOG_LIST_COLUMNS) : (MAX_DIALOG_LIST_ROWS * MAX_DIALOG_LIST_COLUMNS); i < j; i++)
	    {
	        if (playertextid == playerDialogTextDraw[playerid][playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_MODEL][i]])
	        {
	            if ((GetTickCount() - playerDialogClickTickCount[playerid][i]) <= 200)
	            {
	                return OnPlayerClickPlayerTextDraw(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_LEFT_BUTTON]]);
	            }
				playerDialogClickTickCount[playerid][i] = GetTickCount();

				PlayerTextDrawBackgroundColor(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_MODEL][playerDialogListitem[playerid]]], DIALOG_LIST_ITEM_COLOR);
				PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_MODEL][playerDialogListitem[playerid]]]);

				playerDialogListitem[playerid] = i;
				PlayerTextDrawBackgroundColor(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_MODEL][playerDialogListitem[playerid]]], DIALOG_SELECTED_LIST_ITEM_COLOR);
				PlayerTextDrawShow(playerid, playerDialogTextDraw[playerid][playerDialogTextDrawID[playerid][DIALOG_TEXTDRAW_MODEL][playerDialogListitem[playerid]]]);
				break;
			}
	    }
	}

	#if defined GM_OnPlayerClickPlayerTD
       	return GM_OnPlayerClickPlayerTD(playerid, PlayerText:playertextid);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerClickPlayerTD
    #undef OnPlayerClickPlayerTextDraw
#else
    #define _ALS_OnPlayerClickPlayerTD
#endif
#define OnPlayerClickPlayerTextDraw GM_OnPlayerClickPlayerTD
#if defined GM_OnPlayerClickPlayerTD
    forward GM_OnPlayerClickPlayerTD(playerid, PlayerText:playertextid);
#endif
