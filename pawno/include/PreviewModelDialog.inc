#if defined previewmodel_dialog_included
	#endinput
#endif
#define previewmodel_dialog_included

/**************************************/
// PreviewModelDialog.inc - v4.1 - Last Updated: 26 Jan, 2018 - By Gammix
/***************************************/

// by BigETI
#include <memory.inc>

// definitions
#if !defined MAX_DIALOG_PREVIEW_MODELS
	#define MAX_DIALOG_PREVIEW_MODELS 350
#endif

#if !defined MAX_DIALOG_PREVIEW_TEXTSIZE
	#define MAX_DIALOG_PREVIEW_TEXTSIZE 128
#endif

#define DIALOG_STYLE_PREVIEW_MODEL 6 // new dialog id definition
#define DIALOG_STYLE_PREVMODEL DIALOG_STYLE_PREVIEW_MODEL

#define MAX_DIALOG_PREVIEW_ROWS 2
#define MAX_DIALOG_PREVIEW_COLUMNS 6

#define DIALOG_PREVIEW_HORIZONTAL_GAP 56.5
#define DIALOG_PREVIEW_VERTICAL_GAP 63.5

#define DIALOG_PREVIEW_SELECT_COLOR 0xFF0000FF
#define DIALOG_PREVIEW_UNSELECT_COLOR 0x464646FF

#define SCROLL_BAR_UP_TEXTDRAW frameTextDraw[4]
#define SCROLL_BAR_DOWN_TEXTDRAW frameTextDraw[6]

#define LEFT_BUTTON_TEXTDRAW leftButtonTextDraw[0]
#define RIGHT_BUTTON_TEXTDRAW rightButtonTextDraw[0]
#define CENTER_BUTTON_TEXTDRAW centerButtonTextDraw[0]

#define LISTITEM_TEXTDRAW<%0> previewModelTextDraw[%0][0]
#define LISTITEM_ROTATE_LEFT_TEXTDRAW<%0> previewModelTextDraw[%0][2]
#define LISTITEM_ROTATE_RIGHT_TEXTDRAW<%0> previewModelTextDraw[%0][3]

// variables
static Text:frameTextDraw[8];
static Text:leftButtonTextDraw[2];
static Text:rightButtonTextDraw[2];
static Text:centerButtonTextDraw[2];
static Text:previewModelTextDraw[MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS][4];

static PlayerText:headerPlayerTextDraw[MAX_PLAYERS];
static PlayerText:leftButtonPlayerTextDraw[MAX_PLAYERS];
static PlayerText:rightButtonPlayerTextDraw[MAX_PLAYERS];
static PlayerText:centerButtonPlayerTextDraw[MAX_PLAYERS];
static PlayerText:pageNumPlayerTextDraw[MAX_PLAYERS];
static PlayerText:scrollBarPlayerTextDraw[MAX_PLAYERS];
static PlayerText:previewModelPlayerTextDraw[MAX_PLAYERS][MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS][2];

static bool:cancelSelectTextDraw[MAX_PLAYERS];

static playerDialogID[MAX_PLAYERS];
static playerDialogListitem[MAX_PLAYERS];
static playerDialogNumListitems[MAX_PLAYERS];
static playerDialogPage[MAX_PLAYERS];
static playerDialogTickCount[MAX_PLAYERS];

static Pointer:playerModelsArray[MAX_PLAYERS];
static Pointer:playerModelsRotationsArray[MAX_PLAYERS][4];
static Pointer:playerModelsNamesArray[MAX_PLAYERS][MAX_DIALOG_PREVIEW_MODELS];

// public function decleration
ShowPreviewModelDialog(playerid, dialogid, caption[], models[], labels[][], button1[], button2[], size = sizeof models);
SetDialogPreviewRotation(playerid, listitem, Float:rx, Float:ry, Float:rz, Float:zoom);

// internal functions
static Prev@PlayerDialogPages(playerid) {
	return floatround((playerDialogNumListitems[playerid] / (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS)), floatround_tozero);
}

static Prev@CancelSelectTextDraw(playerid) {
    cancelSelectTextDraw[playerid] = true;
	return CancelSelectTextDraw(playerid);
}

static Prev@CreateTextDraws() {
	/*
	* FRAME
	*/
    frameTextDraw[0] = TextDrawCreate(144.0000, 154.0000, "CONTENT_BOX");
	TextDrawFont(frameTextDraw[0], 2);
	TextDrawLetterSize(frameTextDraw[0], 0.0000, 18.2000);
	TextDrawColor(frameTextDraw[0], -1);
	TextDrawSetShadow(frameTextDraw[0], 0);
	TextDrawSetOutline(frameTextDraw[0], 0);
	TextDrawBackgroundColor(frameTextDraw[0], 255);
	TextDrawSetProportional(frameTextDraw[0], 1);
	TextDrawUseBox(frameTextDraw[0], 1);
	TextDrawBoxColor(frameTextDraw[0], 150);
	TextDrawTextSize(frameTextDraw[0], 501.0000, 0.0000);

	frameTextDraw[1] = TextDrawCreate(146.5000, 155.5000, "LD_SPAC:WHITE");
	TextDrawFont(frameTextDraw[1], 4);
	TextDrawLetterSize(frameTextDraw[1], 0.0000, 18.2000);
	TextDrawColor(frameTextDraw[1], -1);
	TextDrawSetShadow(frameTextDraw[1], 0);
	TextDrawSetOutline(frameTextDraw[1], 0);
	TextDrawBackgroundColor(frameTextDraw[1], 255);
	TextDrawSetProportional(frameTextDraw[1], 1);
	TextDrawUseBox(frameTextDraw[1], 1);
	TextDrawBoxColor(frameTextDraw[1], 150);
	TextDrawTextSize(frameTextDraw[1], 351.5000, 130.0000);

	frameTextDraw[2] = TextDrawCreate(147.0000, 156.0000, "LD_SPAC:BLACK");
	TextDrawFont(frameTextDraw[2], 4);
	TextDrawLetterSize(frameTextDraw[2], 0.0000, 18.2000);
	TextDrawColor(frameTextDraw[2], -1);
	TextDrawSetShadow(frameTextDraw[2], 0);
	TextDrawSetOutline(frameTextDraw[2], 0);
	TextDrawBackgroundColor(frameTextDraw[2], 255);
	TextDrawSetProportional(frameTextDraw[2], 1);
	TextDrawUseBox(frameTextDraw[2], 1);
	TextDrawBoxColor(frameTextDraw[2], 150);
	TextDrawTextSize(frameTextDraw[2], 340.5000, 129.0000);

	frameTextDraw[3] = TextDrawCreate(488.0000, 156.0000, "LD_SPAC:BLACK");
	TextDrawFont(frameTextDraw[3], 4);
	TextDrawLetterSize(frameTextDraw[3], 0.0000, 18.2000);
	TextDrawColor(frameTextDraw[3], -1);
	TextDrawSetShadow(frameTextDraw[3], 0);
	TextDrawSetOutline(frameTextDraw[3], 0);
	TextDrawBackgroundColor(frameTextDraw[3], 255);
	TextDrawSetProportional(frameTextDraw[3], 1);
	TextDrawUseBox(frameTextDraw[3], 1);
	TextDrawBoxColor(frameTextDraw[3], 150);
	TextDrawTextSize(frameTextDraw[3], 9.5000, 129.0000);

	// button for scrolling up
	frameTextDraw[4] = TextDrawCreate(488.0000, 156.0000, "LD_POOL:BALL");
	TextDrawFont(frameTextDraw[4], 4);
	TextDrawLetterSize(frameTextDraw[4], 0.0000, 18.2000);
	TextDrawColor(frameTextDraw[4], -1);
	TextDrawSetShadow(frameTextDraw[4], 0);
	TextDrawSetOutline(frameTextDraw[4], 0);
	TextDrawBackgroundColor(frameTextDraw[4], 255);
	TextDrawSetProportional(frameTextDraw[4], 1);
	TextDrawUseBox(frameTextDraw[4], 1);
	TextDrawBoxColor(frameTextDraw[4], 150);
	TextDrawTextSize(frameTextDraw[4], 9.5000, 9.5000);
	TextDrawSetSelectable(frameTextDraw[4], 1);
	//

	frameTextDraw[5] = TextDrawCreate(489.0000, 156.5000, "LD_BEAT:UP");
	TextDrawFont(frameTextDraw[5], 4);
	TextDrawLetterSize(frameTextDraw[5], 0.0000, 18.2000);
	TextDrawColor(frameTextDraw[5], 255);
	TextDrawSetShadow(frameTextDraw[5], 0);
	TextDrawSetOutline(frameTextDraw[5], 0);
	TextDrawBackgroundColor(frameTextDraw[5], 255);
	TextDrawSetProportional(frameTextDraw[5], 1);
	TextDrawUseBox(frameTextDraw[5], 1);
	TextDrawBoxColor(frameTextDraw[5], 150);
	TextDrawTextSize(frameTextDraw[5], 7.5000, 7.5000);

	// button for scrolling down
	frameTextDraw[6] = TextDrawCreate(488.0000, 275.5000, "LD_POOL:BALL");
	TextDrawFont(frameTextDraw[6], 4);
	TextDrawLetterSize(frameTextDraw[6], 0.0000, 18.2000);
	TextDrawColor(frameTextDraw[6], -1);
	TextDrawSetShadow(frameTextDraw[6], 0);
	TextDrawSetOutline(frameTextDraw[6], 0);
	TextDrawBackgroundColor(frameTextDraw[6], 255);
	TextDrawSetProportional(frameTextDraw[6], 1);
	TextDrawUseBox(frameTextDraw[6], 1);
	TextDrawBoxColor(frameTextDraw[6], 150);
	TextDrawTextSize(frameTextDraw[6], 9.5000, 9.5000);
	TextDrawSetSelectable(frameTextDraw[6], 1);
	//

	frameTextDraw[7] = TextDrawCreate(489.0000, 277.0000, "LD_BEAT:DOWN");
	TextDrawFont(frameTextDraw[7], 4);
	TextDrawLetterSize(frameTextDraw[7], 0.0000, 18.2000);
	TextDrawColor(frameTextDraw[7], 255);
	TextDrawSetShadow(frameTextDraw[7], 0);
	TextDrawSetOutline(frameTextDraw[7], 0);
	TextDrawBackgroundColor(frameTextDraw[7], 255);
	TextDrawSetProportional(frameTextDraw[7], 1);
	TextDrawUseBox(frameTextDraw[7], 1);
	TextDrawBoxColor(frameTextDraw[7], 150);
	TextDrawTextSize(frameTextDraw[7], 7.5000, 7.5000);

	/*
	* LEFT BUTTON (button1)
	*/
	// left button for response type "1"
	leftButtonTextDraw[0] = TextDrawCreate(260.0000, 293.0000, "LD_SPAC:WHITE");
	TextDrawFont(leftButtonTextDraw[0], 4);
	TextDrawLetterSize(leftButtonTextDraw[0], 0.5000, 1.0000);
	TextDrawColor(leftButtonTextDraw[0], -1);
	TextDrawSetShadow(leftButtonTextDraw[0], 0);
	TextDrawSetOutline(leftButtonTextDraw[0], 0);
	TextDrawBackgroundColor(leftButtonTextDraw[0], 255);
	TextDrawSetProportional(leftButtonTextDraw[0], 1);
	TextDrawTextSize(leftButtonTextDraw[0], 55.0000, 18.0000);
	TextDrawSetSelectable(leftButtonTextDraw[0], 1);
	//

	leftButtonTextDraw[1] = TextDrawCreate(260.5000, 293.5000, "LD_SPAC:BLACK");
	TextDrawFont(leftButtonTextDraw[1], 4);
	TextDrawLetterSize(leftButtonTextDraw[1], 0.5000, 1.0000);
	TextDrawColor(leftButtonTextDraw[1], -1);
	TextDrawSetShadow(leftButtonTextDraw[1], 0);
	TextDrawSetOutline(leftButtonTextDraw[1], 0);
	TextDrawBackgroundColor(leftButtonTextDraw[1], 255);
	TextDrawSetProportional(leftButtonTextDraw[1], 1);
	TextDrawTextSize(leftButtonTextDraw[1], 54.0000, 17.0000);

	/*
	* RIGHT BUTTON (button2)
	*/
	// right button for response type "0"
	rightButtonTextDraw[0] = TextDrawCreate(325.0000, 293.0000, "LD_SPAC:WHITE");
	TextDrawFont(rightButtonTextDraw[0], 4);
	TextDrawLetterSize(rightButtonTextDraw[0], 0.5000, 1.0000);
	TextDrawColor(rightButtonTextDraw[0], -1);
	TextDrawSetShadow(rightButtonTextDraw[0], 0);
	TextDrawSetOutline(rightButtonTextDraw[0], 0);
	TextDrawBackgroundColor(rightButtonTextDraw[0], 255);
	TextDrawSetProportional(rightButtonTextDraw[0], 1);
	TextDrawTextSize(rightButtonTextDraw[0], 55.0000, 18.0000);
	TextDrawSetSelectable(rightButtonTextDraw[0], 1);
	//

	rightButtonTextDraw[1] = TextDrawCreate(325.5000, 293.5000, "LD_SPAC:BLACK");
	TextDrawFont(rightButtonTextDraw[1], 4);
	TextDrawLetterSize(rightButtonTextDraw[1], 0.5000, 1.0000);
	TextDrawColor(rightButtonTextDraw[1], -1);
	TextDrawSetShadow(rightButtonTextDraw[1], 0);
	TextDrawSetOutline(rightButtonTextDraw[1], 0);
	TextDrawBackgroundColor(rightButtonTextDraw[1], 255);
	TextDrawSetProportional(rightButtonTextDraw[1], 1);
	TextDrawTextSize(rightButtonTextDraw[1], 54.0000, 17.0000);

	/*
	* CENTER BUTTON (button3)
	*/
	// right button for response type "0"
	centerButtonTextDraw[0] = TextDrawCreate(290.5000, 293.0000, "LD_SPAC:WHITE");
	TextDrawFont(centerButtonTextDraw[0], 4);
	TextDrawLetterSize(centerButtonTextDraw[0], 0.5000, 1.0000);
	TextDrawColor(centerButtonTextDraw[0], -1);
	TextDrawSetShadow(centerButtonTextDraw[0], 0);
	TextDrawSetOutline(centerButtonTextDraw[0], 0);
	TextDrawBackgroundColor(centerButtonTextDraw[0], 255);
	TextDrawSetProportional(centerButtonTextDraw[0], 1);
	TextDrawTextSize(centerButtonTextDraw[0], 55.0000, 18.0000);
	TextDrawSetSelectable(centerButtonTextDraw[0], 1);
	//

	centerButtonTextDraw[1] = TextDrawCreate(291.0000, 293.5000, "LD_SPAC:BLACK");
	TextDrawFont(centerButtonTextDraw[1], 4);
	TextDrawLetterSize(centerButtonTextDraw[1], 0.5000, 1.0000);
	TextDrawColor(centerButtonTextDraw[1], -1);
	TextDrawSetShadow(centerButtonTextDraw[1], 0);
	TextDrawSetOutline(centerButtonTextDraw[1], 0);
	TextDrawBackgroundColor(centerButtonTextDraw[1], 255);
	TextDrawSetProportional(centerButtonTextDraw[1], 1);
	TextDrawTextSize(centerButtonTextDraw[1], 54.0000, 17.0000);

	/*
	* PREVIEW MODEL LISTITEMS
	*/
	new textdrawid;

	for (new a = 0; a < MAX_DIALOG_PREVIEW_ROWS; a++) {
		for (new b = 0; b < MAX_DIALOG_PREVIEW_COLUMNS; b++) {
		    textdrawid = ((a * MAX_DIALOG_PREVIEW_COLUMNS) + b);

		    // listitem border button to select item
			previewModelTextDraw[textdrawid][0] = TextDrawCreate((148.5000 + (b * DIALOG_PREVIEW_HORIZONTAL_GAP)), (157.5000 + (a * DIALOG_PREVIEW_VERTICAL_GAP)), "LD_SPAC:WHITE");
			TextDrawFont(previewModelTextDraw[textdrawid][0], 4);
			TextDrawLetterSize(previewModelTextDraw[textdrawid][0], 0.5000, 1.0000);
			TextDrawColor(previewModelTextDraw[textdrawid][0], -16776961);
			TextDrawSetShadow(previewModelTextDraw[textdrawid][0], 0);
			TextDrawSetOutline(previewModelTextDraw[textdrawid][0], 0);
			TextDrawBackgroundColor(previewModelTextDraw[textdrawid][0], 255);
			TextDrawSetProportional(previewModelTextDraw[textdrawid][0], 1);
			TextDrawTextSize(previewModelTextDraw[textdrawid][0], 55.0000, 62.0000);
			TextDrawSetSelectable(previewModelTextDraw[textdrawid][0], 1);
			//

			previewModelTextDraw[textdrawid][1] = TextDrawCreate((149.0000 + (b * DIALOG_PREVIEW_HORIZONTAL_GAP)), (158.0000 + (a * DIALOG_PREVIEW_VERTICAL_GAP)), "MODEL");
			TextDrawFont(previewModelTextDraw[textdrawid][1], 5);
			TextDrawLetterSize(previewModelTextDraw[textdrawid][1], 0.5000, 1.0000);
			TextDrawColor(previewModelTextDraw[textdrawid][1], -1);
			TextDrawSetShadow(previewModelTextDraw[textdrawid][1], 0);
			TextDrawSetOutline(previewModelTextDraw[textdrawid][1], 0);
			TextDrawBackgroundColor(previewModelTextDraw[textdrawid][1], 842150655);
			TextDrawSetProportional(previewModelTextDraw[textdrawid][1], 1);
			TextDrawTextSize(previewModelTextDraw[textdrawid][1], 54.0000, 61.0000);
			TextDrawSetPreviewModel(previewModelTextDraw[textdrawid][1], 132);
			TextDrawSetPreviewRot(previewModelTextDraw[textdrawid][1], 0.0000, 0.0000, -50.0000, 1.0000);

		    // listitem rotation button (left)
			previewModelTextDraw[textdrawid][2] = TextDrawCreate((151.5000 + (b * DIALOG_PREVIEW_HORIZONTAL_GAP)), (181.5000 + (a * DIALOG_PREVIEW_VERTICAL_GAP)), "LD_BEAT:LEFT");
			TextDrawFont(previewModelTextDraw[textdrawid][2], 4);
			TextDrawLetterSize(previewModelTextDraw[textdrawid][2], 0.5000, 1.0000);
			TextDrawColor(previewModelTextDraw[textdrawid][2], -156);
			TextDrawSetShadow(previewModelTextDraw[textdrawid][2], 0);
			TextDrawSetOutline(previewModelTextDraw[textdrawid][2], 0);
			TextDrawBackgroundColor(previewModelTextDraw[textdrawid][2], 255);
			TextDrawSetProportional(previewModelTextDraw[textdrawid][2], 1);
			TextDrawTextSize(previewModelTextDraw[textdrawid][2], 15.0000, 15.0000);
			TextDrawSetSelectable(previewModelTextDraw[textdrawid][2], 1);
			//

		    // listitem rotation button (right)
			previewModelTextDraw[textdrawid][3] = TextDrawCreate((185.0000 + (b * DIALOG_PREVIEW_HORIZONTAL_GAP)), (181.5000 + (a * DIALOG_PREVIEW_VERTICAL_GAP)), "LD_BEAT:RIGHT");
			TextDrawFont(previewModelTextDraw[textdrawid][3], 4);
			TextDrawLetterSize(previewModelTextDraw[textdrawid][3], 0.5000, 1.0000);
			TextDrawColor(previewModelTextDraw[textdrawid][3], -156);
			TextDrawSetShadow(previewModelTextDraw[textdrawid][3], 0);
			TextDrawSetOutline(previewModelTextDraw[textdrawid][3], 0);
			TextDrawBackgroundColor(previewModelTextDraw[textdrawid][3], 255);
			TextDrawSetProportional(previewModelTextDraw[textdrawid][3], 1);
			TextDrawTextSize(previewModelTextDraw[textdrawid][3], 15.0000, 15.0000);
			TextDrawSetSelectable(previewModelTextDraw[textdrawid][3], 1);
			//
		}
	}
}

static Prev@DestroyTextDraws() {
    /*
	* FRAME
	*/
	for (new i = 0; i < sizeof frameTextDraw; i++) {
   	 	TextDrawDestroy(frameTextDraw[i]);
	}

	/*
	* LEFT BUTTON (button1)
	*/
	TextDrawDestroy(leftButtonTextDraw[0]);
	TextDrawDestroy(leftButtonTextDraw[1]);

	/*
	* RIGHT BUTTON (button2)
	*/
	TextDrawDestroy(rightButtonTextDraw[0]);
	TextDrawDestroy(rightButtonTextDraw[1]);

	/*
	* CENTER BUTTON (button1)
	*/
	TextDrawDestroy(centerButtonTextDraw[0]);
	TextDrawDestroy(centerButtonTextDraw[1]);

	/*
	* PREVIEW MODEL LISTITEMS
	*/
	for (new i = 0; i < (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS); i++) {
	    TextDrawDestroy(previewModelTextDraw[i][0]);
	    TextDrawDestroy(previewModelTextDraw[i][1]);
	    TextDrawDestroy(previewModelTextDraw[i][2]);
	    TextDrawDestroy(previewModelTextDraw[i][3]);
	}
}

static Prev@CreatePlayerTextDraws(playerid, header[], button1[], button2[], button3[], totalItems) {
    /*
	* HEADER
	*/
    headerPlayerTextDraw[playerid] = CreatePlayerTextDraw(playerid, 144.0000, 143.0000, header);
	PlayerTextDrawFont(playerid, headerPlayerTextDraw[playerid], 2);
	PlayerTextDrawLetterSize(playerid, headerPlayerTextDraw[playerid], 0.1399, 0.8999);
	PlayerTextDrawColor(playerid, headerPlayerTextDraw[playerid], -1);
	PlayerTextDrawSetShadow(playerid, headerPlayerTextDraw[playerid], 0);
	PlayerTextDrawSetOutline(playerid, headerPlayerTextDraw[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, headerPlayerTextDraw[playerid], 255);
	PlayerTextDrawSetProportional(playerid, headerPlayerTextDraw[playerid], 1);
	PlayerTextDrawUseBox(playerid, headerPlayerTextDraw[playerid], 1);
	PlayerTextDrawBoxColor(playerid, headerPlayerTextDraw[playerid], 255);
	PlayerTextDrawTextSize(playerid, headerPlayerTextDraw[playerid], 501.0000, 0.0000);

	/*
	* LEFT BUTTON (button1)
	*/
	if (button1[0] != EOS) {
		leftButtonPlayerTextDraw[playerid] = CreatePlayerTextDraw(playerid, 287.5000, 297.0000, button1);
		PlayerTextDrawFont(playerid, leftButtonPlayerTextDraw[playerid], 2);
		PlayerTextDrawLetterSize(playerid, leftButtonPlayerTextDraw[playerid], 0.1399, 0.8999);
		PlayerTextDrawAlignment(playerid, leftButtonPlayerTextDraw[playerid], 2);
		PlayerTextDrawColor(playerid, leftButtonPlayerTextDraw[playerid], -1);
		PlayerTextDrawSetShadow(playerid, leftButtonPlayerTextDraw[playerid], 0);
		PlayerTextDrawSetOutline(playerid, leftButtonPlayerTextDraw[playerid], 0);
		PlayerTextDrawBackgroundColor(playerid, leftButtonPlayerTextDraw[playerid], 255);
		PlayerTextDrawSetProportional(playerid, leftButtonPlayerTextDraw[playerid], 1);
		PlayerTextDrawTextSize(playerid, leftButtonPlayerTextDraw[playerid], 0.0000, 480.0000);
	} else {
        leftButtonPlayerTextDraw[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}

	/*
	* RIGHT BUTTON (button2)
	*/
	if (button2[0] != EOS) {
		rightButtonPlayerTextDraw[playerid] = CreatePlayerTextDraw(playerid, 352.5000, 297.0000, button2);
		PlayerTextDrawFont(playerid, rightButtonPlayerTextDraw[playerid], 2);
		PlayerTextDrawLetterSize(playerid, rightButtonPlayerTextDraw[playerid], 0.1399, 0.8999);
		PlayerTextDrawAlignment(playerid, rightButtonPlayerTextDraw[playerid], 2);
		PlayerTextDrawColor(playerid, rightButtonPlayerTextDraw[playerid], -1);
		PlayerTextDrawSetShadow(playerid, rightButtonPlayerTextDraw[playerid], 0);
		PlayerTextDrawSetOutline(playerid, rightButtonPlayerTextDraw[playerid], 0);
		PlayerTextDrawBackgroundColor(playerid, rightButtonPlayerTextDraw[playerid], 255);
		PlayerTextDrawSetProportional(playerid, rightButtonPlayerTextDraw[playerid], 1);
		PlayerTextDrawTextSize(playerid, rightButtonPlayerTextDraw[playerid], 0.0000, 480.0000);
    } else {
        rightButtonPlayerTextDraw[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}

	/*
	* CENTER BUTTON (button3)
	*/
	if (button3[0] != EOS) {
		centerButtonPlayerTextDraw[playerid] = CreatePlayerTextDraw(playerid, 318.0000, 297.0000, button3);
		PlayerTextDrawFont(playerid, centerButtonPlayerTextDraw[playerid], 2);
		PlayerTextDrawLetterSize(playerid, centerButtonPlayerTextDraw[playerid], 0.1399, 0.8999);
		PlayerTextDrawAlignment(playerid, centerButtonPlayerTextDraw[playerid], 2);
		PlayerTextDrawColor(playerid, centerButtonPlayerTextDraw[playerid], -1);
		PlayerTextDrawSetShadow(playerid, centerButtonPlayerTextDraw[playerid], 0);
		PlayerTextDrawSetOutline(playerid, centerButtonPlayerTextDraw[playerid], 0);
		PlayerTextDrawBackgroundColor(playerid, centerButtonPlayerTextDraw[playerid], 255);
		PlayerTextDrawSetProportional(playerid, centerButtonPlayerTextDraw[playerid], 1);
		PlayerTextDrawTextSize(playerid, centerButtonPlayerTextDraw[playerid], 0.0000, 480.0000);
    } else {
        centerButtonPlayerTextDraw[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}

	/*
	* PAGE NUMBER
	*/
	if (totalItems > (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS)) { // if there are more than "12" listitems, create page textdraw
	    pageNumPlayerTextDraw[playerid] = CreatePlayerTextDraw(playerid, 501.5000, 143.0000, "Page: 1");
		PlayerTextDrawFont(playerid, pageNumPlayerTextDraw[playerid], 2);
		PlayerTextDrawLetterSize(playerid, pageNumPlayerTextDraw[playerid], 0.1399, 0.8999);
		PlayerTextDrawAlignment(playerid, pageNumPlayerTextDraw[playerid], 3);
		PlayerTextDrawColor(playerid, pageNumPlayerTextDraw[playerid], -1);
		PlayerTextDrawSetShadow(playerid, pageNumPlayerTextDraw[playerid], 0);
		PlayerTextDrawSetOutline(playerid, pageNumPlayerTextDraw[playerid], 0);
		PlayerTextDrawBackgroundColor(playerid, pageNumPlayerTextDraw[playerid], 255);
		PlayerTextDrawSetProportional(playerid, pageNumPlayerTextDraw[playerid], 1);
		PlayerTextDrawTextSize(playerid, pageNumPlayerTextDraw[playerid], 501.0000, 0.0000);
	} else {
        pageNumPlayerTextDraw[playerid] = PlayerText:INVALID_TEXT_DRAW;
	}

	/*
	* PREVIEW MODEL LISTITEMS
	*/
	new textdrawid;

	for (new a = 0; a < MAX_DIALOG_PREVIEW_ROWS; a++) {
		for (new b = 0; b < MAX_DIALOG_PREVIEW_COLUMNS; b++) {
		    textdrawid = ((a * MAX_DIALOG_PREVIEW_COLUMNS) + b);

			if (textdrawid >= totalItems) {
			    previewModelPlayerTextDraw[playerid][textdrawid][0] = PlayerText:INVALID_TEXT_DRAW;
			    previewModelPlayerTextDraw[playerid][textdrawid][1] = PlayerText:INVALID_TEXT_DRAW;
				continue;
			}

			previewModelPlayerTextDraw[playerid][textdrawid][0] = CreatePlayerTextDraw(playerid, (153.5000 + (b * DIALOG_PREVIEW_HORIZONTAL_GAP)), (161.5000 + (a * DIALOG_PREVIEW_VERTICAL_GAP)), "");
			PlayerTextDrawFont(playerid, previewModelPlayerTextDraw[playerid][textdrawid][0], 2);
			PlayerTextDrawLetterSize(playerid, previewModelPlayerTextDraw[playerid][textdrawid][0], 0.1399, 0.8999);
			PlayerTextDrawColor(playerid, previewModelPlayerTextDraw[playerid][textdrawid][0], -1);
			PlayerTextDrawSetShadow(playerid, previewModelPlayerTextDraw[playerid][textdrawid][0], 0);
			PlayerTextDrawSetOutline(playerid, previewModelPlayerTextDraw[playerid][textdrawid][0], 0);
			PlayerTextDrawBackgroundColor(playerid, previewModelPlayerTextDraw[playerid][textdrawid][0], 255);
			PlayerTextDrawSetProportional(playerid, previewModelPlayerTextDraw[playerid][textdrawid][0], 1);
			PlayerTextDrawTextSize(playerid, previewModelPlayerTextDraw[playerid][textdrawid][0], 640.0000, 480.0000);

			previewModelPlayerTextDraw[playerid][textdrawid][1] = CreatePlayerTextDraw(playerid, (176.5000 + (b * DIALOG_PREVIEW_HORIZONTAL_GAP)), (204.5000 + (a * DIALOG_PREVIEW_VERTICAL_GAP)), "0.0");
			PlayerTextDrawFont(playerid, previewModelPlayerTextDraw[playerid][textdrawid][1], 2);
			PlayerTextDrawLetterSize(playerid, previewModelPlayerTextDraw[playerid][textdrawid][1], 0.1399, 0.8999);
			PlayerTextDrawAlignment(playerid, previewModelPlayerTextDraw[playerid][textdrawid][1], 2);
			PlayerTextDrawColor(playerid, previewModelPlayerTextDraw[playerid][textdrawid][1], -156);
			PlayerTextDrawSetShadow(playerid, previewModelPlayerTextDraw[playerid][textdrawid][1], 0);
			PlayerTextDrawSetOutline(playerid, previewModelPlayerTextDraw[playerid][textdrawid][1], 0);
			PlayerTextDrawBackgroundColor(playerid, previewModelPlayerTextDraw[playerid][textdrawid][1], 255);
			PlayerTextDrawSetProportional(playerid, previewModelPlayerTextDraw[playerid][textdrawid][1], 1);
			PlayerTextDrawTextSize(playerid, previewModelPlayerTextDraw[playerid][textdrawid][1], 640.0000, 480.0000);
		}
	}

	/*
	* SCROLLBAR
	*/
	scrollBarPlayerTextDraw[playerid] = PlayerText:INVALID_TEXT_DRAW; // we will create this later if needed in dialog
}

static Prev@DestroyPlayerTextDraws(playerid) {
    /*
	* HEADER
	*/
    PlayerTextDrawDestroy(playerid, headerPlayerTextDraw[playerid]);

	/*
	* LEFT BUTTON (button1)
	*/
    PlayerTextDrawDestroy(playerid, leftButtonPlayerTextDraw[playerid]);

	/*
	* RIGHT BUTTON (button2)
	*/
    PlayerTextDrawDestroy(playerid, rightButtonPlayerTextDraw[playerid]);

	/*
	* CENTER BUTTON (button3)
	*/
    PlayerTextDrawDestroy(playerid, centerButtonPlayerTextDraw[playerid]);

	/*
	* PAGE NUMBER
	*/
    PlayerTextDrawDestroy(playerid, pageNumPlayerTextDraw[playerid]);

	/*
	* SCROLLBAR
	*/
    PlayerTextDrawDestroy(playerid, scrollBarPlayerTextDraw[playerid]);

	/*
	* PREVIEW MODEL LISTITEMS
	*/
	for (new i = 0; i < (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS); i++) {
		PlayerTextDrawDestroy(playerid, previewModelPlayerTextDraw[playerid][i][0]);
		PlayerTextDrawDestroy(playerid, previewModelPlayerTextDraw[playerid][i][1]);
	}
}

static Prev@UpdateScrollBar(playerid, page, totalPages) {
    PlayerTextDrawDestroy(playerid, scrollBarPlayerTextDraw[playerid]);

	const Float:MAX_HEIGHT = 108.0000;
	new Float:height = (MAX_HEIGHT / totalPages);
    new Float:y = 166.5000;
	y += (height * page);

    scrollBarPlayerTextDraw[playerid] = CreatePlayerTextDraw(playerid, 489.0000, y, "LD_SPAC:WHITE");
	PlayerTextDrawFont(playerid, scrollBarPlayerTextDraw[playerid], 4);
	PlayerTextDrawLetterSize(playerid, scrollBarPlayerTextDraw[playerid], 0.0000, 18.2000);
	PlayerTextDrawColor(playerid, scrollBarPlayerTextDraw[playerid], -16776961);
	PlayerTextDrawSetShadow(playerid, scrollBarPlayerTextDraw[playerid], 0);
	PlayerTextDrawSetOutline(playerid, scrollBarPlayerTextDraw[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, scrollBarPlayerTextDraw[playerid], 255);
	PlayerTextDrawSetProportional(playerid, scrollBarPlayerTextDraw[playerid], 1);
	PlayerTextDrawUseBox(playerid, scrollBarPlayerTextDraw[playerid], 1);
	PlayerTextDrawBoxColor(playerid, scrollBarPlayerTextDraw[playerid], 150);
	PlayerTextDrawTextSize(playerid, scrollBarPlayerTextDraw[playerid], 7.5000, height);

	PlayerTextDrawShow(playerid, scrollBarPlayerTextDraw[playerid]);
}

static Prev@UpdateListitems(playerid, page, selected, totalItems) {
    new textdrawid;
    new listitem;

    new name[MAX_DIALOG_PREVIEW_TEXTSIZE];

	for (new a = 0; a < MAX_DIALOG_PREVIEW_ROWS; a++) {
		for (new b = 0; b < MAX_DIALOG_PREVIEW_COLUMNS; b++) {
		    textdrawid = ((a * MAX_DIALOG_PREVIEW_COLUMNS) + b);
            listitem = ((page * (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS)) + textdrawid);

			if (listitem >= totalItems) {
			    TextDrawHideForPlayer(playerid, previewModelTextDraw[textdrawid][0]);
			    TextDrawHideForPlayer(playerid, previewModelTextDraw[textdrawid][1]);
			    TextDrawHideForPlayer(playerid, previewModelTextDraw[textdrawid][2]);
			    TextDrawHideForPlayer(playerid, previewModelTextDraw[textdrawid][3]);

			    PlayerTextDrawHide(playerid, previewModelPlayerTextDraw[playerid][textdrawid][0]);
			    PlayerTextDrawHide(playerid, previewModelPlayerTextDraw[playerid][textdrawid][1]);

				continue;
			}

			TextDrawSetPreviewModel(previewModelTextDraw[textdrawid][1], MEM_get_val(playerModelsArray[playerid], listitem));
			TextDrawSetPreviewRot(previewModelTextDraw[textdrawid][1], Float:MEM_get_val(playerModelsRotationsArray[playerid][0], listitem), Float:MEM_get_val(playerModelsRotationsArray[playerid][1], listitem), Float:MEM_get_val(playerModelsRotationsArray[playerid][2], listitem), Float:MEM_get_val(playerModelsRotationsArray[playerid][3], listitem));
			TextDrawShowForPlayer(playerid, previewModelTextDraw[textdrawid][1]);

			if (textdrawid == selected) {
				TextDrawColor(previewModelTextDraw[textdrawid][0], DIALOG_PREVIEW_SELECT_COLOR);
				TextDrawShowForPlayer(playerid, previewModelTextDraw[textdrawid][0]);

			    TextDrawShowForPlayer(playerid, previewModelTextDraw[textdrawid][2]);
			    TextDrawShowForPlayer(playerid, previewModelTextDraw[textdrawid][3]);

			    new string[16];
			    format(string, sizeof string, "%0.1f", Float:MEM_get_val(playerModelsRotationsArray[playerid][3], listitem));
			    PlayerTextDrawSetString(playerid, previewModelPlayerTextDraw[playerid][textdrawid][1], string);
			    PlayerTextDrawShow(playerid, previewModelPlayerTextDraw[playerid][textdrawid][1]);
			} else {
				TextDrawColor(previewModelTextDraw[textdrawid][0], DIALOG_PREVIEW_UNSELECT_COLOR);
				TextDrawShowForPlayer(playerid, previewModelTextDraw[textdrawid][0]);

			    TextDrawHideForPlayer(playerid, previewModelTextDraw[textdrawid][2]);
			    TextDrawHideForPlayer(playerid, previewModelTextDraw[textdrawid][3]);

			    PlayerTextDrawHide(playerid, previewModelPlayerTextDraw[playerid][textdrawid][1]);
			}

			if (MEM_get_arr(playerModelsNamesArray[playerid][listitem], 0, name, MEM_get_size(playerModelsNamesArray[playerid][listitem])) != MEM_NULLFEPTR) {
				PlayerTextDrawSetString(playerid, previewModelPlayerTextDraw[playerid][textdrawid][0], name);
				PlayerTextDrawShow(playerid, previewModelPlayerTextDraw[playerid][textdrawid][0]);
			} else {
				PlayerTextDrawHide(playerid, previewModelPlayerTextDraw[playerid][textdrawid][0]);
			}
		}
	}

	new string[16];
 	format(string, sizeof string, "PAGE: %i/%i", (page + 1), Prev@PlayerDialogPages(playerid));
	PlayerTextDrawSetString(playerid, pageNumPlayerTextDraw[playerid], string);
}

static Prev@HidePlayerDialog(playerid) {
    /*
	* FRAME
	*/
	for (new i = 0; i < sizeof frameTextDraw; i++) {
   	 	TextDrawHideForPlayer(playerid, frameTextDraw[i]);
	}

	/*
	* LEFT BUTTON
	*/
	TextDrawHideForPlayer(playerid, leftButtonTextDraw[0]);
	TextDrawHideForPlayer(playerid, leftButtonTextDraw[1]);

	/*
	* RIGHT BUTTON
	*/
	TextDrawHideForPlayer(playerid, rightButtonTextDraw[0]);
	TextDrawHideForPlayer(playerid, rightButtonTextDraw[1]);

	/*
	* CENTER BUTTON
	*/
	TextDrawHideForPlayer(playerid, centerButtonTextDraw[0]);
	TextDrawHideForPlayer(playerid, centerButtonTextDraw[1]);

	/*
	* PREVIEW MODEL LISTITEMS
	*/
	for (new i = 0; i < (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS); i++) {
	    TextDrawHideForPlayer(playerid, previewModelTextDraw[i][0]);
	    TextDrawHideForPlayer(playerid, previewModelTextDraw[i][1]);
	    TextDrawHideForPlayer(playerid, previewModelTextDraw[i][2]);
	    TextDrawHideForPlayer(playerid, previewModelTextDraw[i][3]);
	}

	/*
	* DESTROY DIALOG PLAYER TEXTDRAWS
	*/
	Prev@DestroyPlayerTextDraws(playerid);

	// reset player data
	MEM_delete(playerModelsArray[playerid]);

	MEM_delete(playerModelsRotationsArray[playerid][0]);
	MEM_delete(playerModelsRotationsArray[playerid][1]);
	MEM_delete(playerModelsRotationsArray[playerid][2]);
	MEM_delete(playerModelsRotationsArray[playerid][3]);

	for (new i = 0; i < playerDialogNumListitems[playerid]; i++) {
		MEM_delete(playerModelsNamesArray[playerid][i]);
	}

	playerDialogID[playerid] = -1;

	return Prev@CancelSelectTextDraw(playerid);
}

// hooked methods
public OnGameModeInit() {
    Prev@CreateTextDraws();

	#if defined PDialog_OnGameModeInit
		return PDialog_OnGameModeInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit PDialog_OnGameModeInit
#if defined PDialog_OnGameModeInit
	forward PDialog_OnGameModeInit();
#endif

public OnGameModeExit() {
    Prev@DestroyTextDraws();

	#if defined PDialog_OnGameModeExit
		return PDialog_OnGameModeExit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif
#define OnGameModeExit PDialog_OnGameModeExit
#if defined PDialog_OnGameModeExit
	forward PDialog_OnGameModeExit();
#endif

public OnFilterScriptInit() {
    Prev@CreateTextDraws();

	#if defined PDialog_OnFilterScriptInit
		return PDialog_OnFilterScriptInit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif
#define OnFilterScriptInit PDialog_OnFilterScriptInit
#if defined PDialog_OnFilterScriptInit
	forward PDialog_OnFilterScriptInit();
#endif

public OnFilterScriptExit() {
    Prev@DestroyTextDraws();

	#if defined PDialog_OnFilterScriptExit
		return PDialog_OnFilterScriptExit();
	#else
		return 1;
	#endif
}
#if defined _ALS_OnFilterScriptExit
	#undef OnFilterScriptExit
#else
	#define _ALS_OnFilterScriptExit
#endif
#define OnFilterScriptExit PDialog_OnFilterScriptExit
#if defined PDialog_OnFilterScriptExit
	forward PDialog_OnFilterScriptExit();
#endif

public OnPlayerConnect(playerid) {
    cancelSelectTextDraw[playerid] = false;

	playerDialogID[playerid] = -1;

	#if defined PDialog_OnPlayerConnect
		return PDialog_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect PDialog_OnPlayerConnect
#if defined PDialog_OnPlayerConnect
	forward PDialog_OnPlayerConnect(playerid);
#endif

public OnPlayerDisconnect(playerid, reason) {
    MEM_delete(playerModelsArray[playerid]);

	MEM_delete(playerModelsRotationsArray[playerid][0]);
	MEM_delete(playerModelsRotationsArray[playerid][1]);
	MEM_delete(playerModelsRotationsArray[playerid][2]);
	MEM_delete(playerModelsRotationsArray[playerid][3]);

	for (new i = 0; i < playerDialogNumListitems[playerid]; i++) {
		MEM_delete(playerModelsNamesArray[playerid][i]);
	}

	#if defined PDialog_OnPlayerDisconnect
		return PDialog_OnPlayerDisconnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect PDialog_OnPlayerDisconnect
#if defined PDialog_OnPlayerDisconnect
	forward PDialog_OnPlayerDisconnect(playerid, reason);
#endif

public OnPlayerClickTextDraw(playerid, Text:clickedid) {
	if (cancelSelectTextDraw[playerid]) {
        cancelSelectTextDraw[playerid] = false;

        #if defined PDialog_OnPlayerClickTextDraw
	       	return PDialog_OnPlayerClickTextDraw(playerid, Text:clickedid);
		#else
		   	return 1;
		#endif
	}

	if (playerDialogID[playerid] != -1) {
 		if (clickedid == Text:INVALID_TEXT_DRAW || clickedid == RIGHT_BUTTON_TEXTDRAW) {
		    new dialogid = playerDialogID[playerid];
		    new listitem = ((playerDialogPage[playerid] * (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS)) + playerDialogListitem[playerid]);
    		new inputtext[16];
		    valstr(inputtext, MEM_get_val(playerModelsArray[playerid], listitem));

			Prev@HidePlayerDialog(playerid);

			CallLocalFunction("OnDialogResponse", "iiiis", playerid, dialogid, 0, listitem, inputtext);

		} else if (clickedid == SCROLL_BAR_UP_TEXTDRAW) {
			if (playerDialogPage[playerid] > 0) {
			    Prev@UpdateListitems(playerid, --playerDialogPage[playerid], 0, playerDialogNumListitems[playerid]);
			    Prev@UpdateScrollBar(playerid, playerDialogPage[playerid], Prev@PlayerDialogPages(playerid));

			    playerDialogListitem[playerid] = 0;
			}

		} else if (clickedid == SCROLL_BAR_DOWN_TEXTDRAW) {
			if ((playerDialogPage[playerid] + 1) < Prev@PlayerDialogPages(playerid)) {
			    Prev@UpdateListitems(playerid, ++playerDialogPage[playerid], 0, playerDialogNumListitems[playerid]);
			    Prev@UpdateScrollBar(playerid, playerDialogPage[playerid], Prev@PlayerDialogPages(playerid));

			    playerDialogListitem[playerid] = 0;
			}

		} else if (clickedid == LEFT_BUTTON_TEXTDRAW || clickedid == CENTER_BUTTON_TEXTDRAW) {
		    new dialogid = playerDialogID[playerid];
		    new listitem = ((playerDialogPage[playerid] * (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS)) + playerDialogListitem[playerid]);
		    new inputtext[16];
		    valstr(inputtext, MEM_get_val(playerModelsArray[playerid], listitem));

			Prev@HidePlayerDialog(playerid);

			CallLocalFunction("OnDialogResponse", "iiiis", playerid, dialogid, 1, listitem, inputtext);

		} else if (clickedid == LISTITEM_ROTATE_LEFT_TEXTDRAW<playerDialogListitem[playerid]>) {
		    new listitem = ((playerDialogPage[playerid] * (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS)) + playerDialogListitem[playerid]);

			new Float:rx, Float:ry, Float:rz, Float:rzoom;
			rx = Float:MEM_get_val(playerModelsRotationsArray[playerid][0], listitem);
			ry = Float:MEM_get_val(playerModelsRotationsArray[playerid][1], listitem);
			rz = Float:MEM_get_val(playerModelsRotationsArray[playerid][2], listitem);
			rzoom = Float:MEM_get_val(playerModelsRotationsArray[playerid][3], listitem);

			rz -= 10.0;
			MEM_set_val(playerModelsRotationsArray[playerid][2], listitem, _:rz);

		    TextDrawSetPreviewRot(previewModelTextDraw[playerDialogListitem[playerid]][1], rx, ry, rz, rzoom);
			TextDrawShowForPlayer(playerid, previewModelTextDraw[playerDialogListitem[playerid]][1]);

            new string[16];
  			format(string, sizeof string, "%0.1f", rz);
	    	PlayerTextDrawSetString(playerid, previewModelPlayerTextDraw[playerid][playerDialogListitem[playerid]][1], string);
		    PlayerTextDrawShow(playerid, previewModelPlayerTextDraw[playerid][playerDialogListitem[playerid]][1]);

		} else if (clickedid == LISTITEM_ROTATE_RIGHT_TEXTDRAW<playerDialogListitem[playerid]>) {
		    new listitem = ((playerDialogPage[playerid] * (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS)) + playerDialogListitem[playerid]);

			new Float:rx, Float:ry, Float:rz, Float:rzoom;
			rx = Float:MEM_get_val(playerModelsRotationsArray[playerid][0], listitem);
			ry = Float:MEM_get_val(playerModelsRotationsArray[playerid][1], listitem);
			rz = Float:MEM_get_val(playerModelsRotationsArray[playerid][2], listitem);
			rzoom = Float:MEM_get_val(playerModelsRotationsArray[playerid][3], listitem);

			rz += 10.0;
			MEM_set_val(playerModelsRotationsArray[playerid][2], listitem, _:rz);

		    TextDrawSetPreviewRot(previewModelTextDraw[playerDialogListitem[playerid]][1], rx, ry, rz, rzoom);
			TextDrawShowForPlayer(playerid, previewModelTextDraw[playerDialogListitem[playerid]][1]);

            new string[16];
  			format(string, sizeof string, "%0.1f", rz);
	    	PlayerTextDrawSetString(playerid, previewModelPlayerTextDraw[playerid][playerDialogListitem[playerid]][1], string);
		    PlayerTextDrawShow(playerid, previewModelPlayerTextDraw[playerid][playerDialogListitem[playerid]][1]);

		} else {
			for (new i = 0; i < (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS); i++) {
				if (clickedid == LISTITEM_TEXTDRAW<i>) {

					if (playerDialogListitem[playerid] == i && (GetTickCount() - playerDialogTickCount[playerid]) <= 200) {
	                    new dialogid = playerDialogID[playerid];
		    			new listitem = ((playerDialogPage[playerid] * (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS)) + playerDialogListitem[playerid]);
					    new inputtext[16];
		    			valstr(inputtext, MEM_get_val(playerModelsArray[playerid], listitem));

						Prev@HidePlayerDialog(playerid);

						CallLocalFunction("OnDialogResponse", "iiiis", playerid, dialogid, 1, listitem, inputtext);

					} else {
						// reset previous listitem
						new previous = playerDialogListitem[playerid];

					 	TextDrawColor(previewModelTextDraw[previous][0], DIALOG_PREVIEW_UNSELECT_COLOR);
						TextDrawShowForPlayer(playerid, previewModelTextDraw[previous][0]);

					    TextDrawHideForPlayer(playerid, previewModelTextDraw[previous][2]);
					    TextDrawHideForPlayer(playerid, previewModelTextDraw[previous][3]);

					    PlayerTextDrawHide(playerid, previewModelPlayerTextDraw[playerid][previous][1]);

					    // update new selected listitem
					    TextDrawColor(previewModelTextDraw[i][0], DIALOG_PREVIEW_SELECT_COLOR);
						TextDrawShowForPlayer(playerid, previewModelTextDraw[i][0]);

					    TextDrawShowForPlayer(playerid, previewModelTextDraw[i][2]);
					    TextDrawShowForPlayer(playerid, previewModelTextDraw[i][3]);

					    new string[16];
					    format(string, sizeof string, "%0.1f", Float:MEM_get_val(playerModelsRotationsArray[playerid][2], ((playerDialogPage[playerid] * (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS)) + i)));
					    PlayerTextDrawSetString(playerid, previewModelPlayerTextDraw[playerid][i][1], string);
					    PlayerTextDrawShow(playerid, previewModelPlayerTextDraw[playerid][i][1]);

						playerDialogListitem[playerid] = i;
					    playerDialogTickCount[playerid] = GetTickCount();
					}

					break;
				}
			}
		}
	}

	#if defined PDialog_OnPlayerClickTextDraw
       	return PDialog_OnPlayerClickTextDraw(playerid, Text:clickedid);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerClickTextDraw
    #undef OnPlayerClickTextDraw
#else
    #define _ALS_OnPlayerClickTextDraw
#endif
#define OnPlayerClickTextDraw PDialog_OnPlayerClickTextDraw
#if defined PDialog_OnPlayerClickTextDraw
    forward PDialog_OnPlayerClickTextDraw(playerid, Text:clickedid);
#endif

stock Prev@ShowPlayerDialog(playerid, dialogid, style, caption[], info[], button1[], button2[]) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (playerDialogID[playerid] != -1) {
		Prev@HidePlayerDialog(playerid);
  	}

	if (style != DIALOG_STYLE_PREVIEW_MODEL) {
	    return ShowPlayerDialog(playerid, dialogid, style, caption, info, button1, button2);
	}

	if (dialogid < 0) {
	    if (style == DIALOG_STYLE_PREVIEW_MODEL) {
	        return 1;
	    }

	    return ShowPlayerDialog(playerid, dialogid, style, caption, info, button1, button2);
	}

	playerDialogNumListitems[playerid] = 0;

	new pos;
	new pos2;
	new listitem;
	new modelid[8];
	new string[sizeof modelid/*modelid*/ + 1/*\n*/ + MAX_DIALOG_PREVIEW_TEXTSIZE/*text*/];

	new models[MAX_DIALOG_PREVIEW_MODELS];

	new bool:validInput;

	for (new i = 0, len = strlen(info); i < len; i++) {
		if (info[i] == '\n') {
			strmid(string, info, pos, i);
			pos = i + 1;
		} else if (i == (len - 1)) {
			strmid(string, info, pos, len);
		} else {
		    continue;
		}

		if (playerDialogNumListitems[playerid] == MAX_DIALOG_PREVIEW_MODELS) {
    		printf("[PreviewModelDialog.inc] - Warning: You have more than \"MAX_DIALOG_PREVIEW_MODELS\" ("#MAX_DIALOG_PREVIEW_MODELS") listitems, only the items under the limit were loaded.");
			break;
   		}

   		listitem = playerDialogNumListitems[playerid];

		pos2 = strfind(string, "\t", true);

		if (pos2 == -1) {
		    modelid[0] = EOS;
		    strcat(modelid, string);
		} else {
			strmid(modelid, string, 0, pos2);
		}

		validInput = false;
		for (new x = 0; modelid[x] != EOS; x++) {
			if (modelid[x] >= '0' && modelid[x] <= '9') {
			    validInput = true;
				break;
			}
		}

		if (!validInput) {
			continue;
		}

	    models[listitem] = strval(modelid);

        if (pos2 != -1) {
			validInput = false;
			for (new x = (pos2 + 1); string[x] != EOS; x++) {
				if (modelid[x] != ' ') {
				    validInput = true;
					break;
				}
			}

			if (validInput) {
				playerModelsNamesArray[playerid][listitem] = MEM_new_arr(string[pos2 + 1], strlen(string[pos2 + 1]));
			}
		}

		playerDialogNumListitems[playerid]++;
	}

	if (playerDialogNumListitems[playerid] == 0) {
	    return 0;
	}

	// set player data
	playerDialogID[playerid] = dialogid;
	playerDialogListitem[playerid] = 0;
	playerDialogPage[playerid] = 0;
	playerDialogTickCount[playerid] = 0;

	playerModelsArray[playerid] = MEM_new_arr(models, playerDialogNumListitems[playerid]);

	new Float:rotation[MAX_DIALOG_PREVIEW_MODELS] = {0.0, ...};
	playerModelsRotationsArray[playerid][0] = MEM_new_arr(_:rotation, playerDialogNumListitems[playerid]);
	playerModelsRotationsArray[playerid][1] = MEM_new_arr(_:rotation, playerDialogNumListitems[playerid]);
	playerModelsRotationsArray[playerid][2] = MEM_new_arr(_:rotation, playerDialogNumListitems[playerid]);

	new Float:zoom[MAX_DIALOG_PREVIEW_MODELS] = {1.0, ...};
	playerModelsRotationsArray[playerid][3] = MEM_new_arr(_:zoom, playerDialogNumListitems[playerid]);

    /*
	* FRAME
	*/
	for (new i = 0; i < sizeof frameTextDraw; i++) {
   	 	TextDrawShowForPlayer(playerid, frameTextDraw[i]);
	}

	/*
	* BUTTONS AND PLAYER DIALOG TEXTDRAWS
	*/
	if (button2[0] == EOS) { // only one center button
		TextDrawHideForPlayer(playerid, leftButtonTextDraw[0]);
		TextDrawHideForPlayer(playerid, leftButtonTextDraw[1]);

		TextDrawHideForPlayer(playerid, rightButtonTextDraw[0]);
		TextDrawHideForPlayer(playerid, rightButtonTextDraw[1]);

		TextDrawShowForPlayer(playerid, centerButtonTextDraw[0]);
		TextDrawShowForPlayer(playerid, centerButtonTextDraw[1]);

		Prev@CreatePlayerTextDraws(playerid, caption, "", "", button1, playerDialogNumListitems[playerid]);
    	PlayerTextDrawShow(playerid, centerButtonPlayerTextDraw[playerid]);
	} else { // dual buttons
		TextDrawShowForPlayer(playerid, leftButtonTextDraw[0]);
		TextDrawShowForPlayer(playerid, leftButtonTextDraw[1]);

		TextDrawShowForPlayer(playerid, rightButtonTextDraw[0]);
		TextDrawShowForPlayer(playerid, rightButtonTextDraw[1]);

		TextDrawHideForPlayer(playerid, centerButtonTextDraw[0]);
		TextDrawHideForPlayer(playerid, centerButtonTextDraw[1]);

		Prev@CreatePlayerTextDraws(playerid, caption, button1, button2, "", playerDialogNumListitems[playerid]);
    	PlayerTextDrawShow(playerid, leftButtonPlayerTextDraw[playerid]);
    	PlayerTextDrawShow(playerid, rightButtonPlayerTextDraw[playerid]);
	}

	/*
	* HEADER
	*/
	PlayerTextDrawShow(playerid, headerPlayerTextDraw[playerid]);

	/*
	* PAGE NUMBER
	*/
    PlayerTextDrawShow(playerid, pageNumPlayerTextDraw[playerid]);

	/*
	* SCROLLBAR
	*/
 	scrollBarPlayerTextDraw[playerid] = PlayerText:INVALID_TEXT_DRAW;
	if (playerDialogNumListitems[playerid] > (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS)) {
        Prev@UpdateScrollBar(playerid, 0, Prev@PlayerDialogPages(playerid));
	}

	/*
	* PREVIEW MODEL LISTITEMS
	*/
	Prev@UpdateListitems(playerid, 0, 0, playerDialogNumListitems[playerid]);

	return SelectTextDraw(playerid, DIALOG_PREVIEW_SELECT_COLOR);
}
#if defined _ALS_ShowPlayerDialog
    #undef ShowPlayerDialog
#else
    #define _ALS_ShowPlayerDialog
#endif
#define ShowPlayerDialog Prev@ShowPlayerDialog

stock ShowPreviewModelDialog(playerid, dialogid, caption[], models[], labels[][], button1[], button2[], size = sizeof models) {
    if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (playerDialogID[playerid] != -1) {
		Prev@HidePlayerDialog(playerid);
  	}

	if (dialogid < 0 || size <= 0) {
	    return 0;
	}

	if (size >= MAX_DIALOG_PREVIEW_MODELS) {
	    size = MAX_DIALOG_PREVIEW_MODELS;
		printf("[PreviewModelDialog.inc] - Warning: You have more than \"MAX_DIALOG_PREVIEW_MODELS\" ("#MAX_DIALOG_PREVIEW_MODELS") listitems, only the items under the limit were loaded.");
	}

    playerModelsArray[playerid] = MEM_new_arr(models, size);

	new Float:rotation[MAX_DIALOG_PREVIEW_MODELS] = {0.0, ...};
	playerModelsRotationsArray[playerid][0] = MEM_new_arr(_:rotation, size);
	playerModelsRotationsArray[playerid][1] = MEM_new_arr(_:rotation, size);
	playerModelsRotationsArray[playerid][2] = MEM_new_arr(_:rotation, size);

	new Float:zoom[MAX_DIALOG_PREVIEW_MODELS] = {1.0, ...};
	playerModelsRotationsArray[playerid][3] = MEM_new_arr(_:zoom, size);

	for (new i = 0; i < size; i++) {
		playerModelsNamesArray[playerid][i] = MEM_new_arr(labels[i], strlen(labels[i]));
	}

    /*
	* FRAME
	*/
	for (new i = 0; i < sizeof frameTextDraw; i++) {
   	 	TextDrawShowForPlayer(playerid, frameTextDraw[i]);
	}

	// set player data
	playerDialogID[playerid] = dialogid;
	playerDialogListitem[playerid] = 0;
	playerDialogNumListitems[playerid] = size;
	playerDialogPage[playerid] = 0;
	playerDialogTickCount[playerid] = 0;

	/*
	* BUTTONS AND PLAYER DIALOG TEXTDRAWS
	*/
	if (button2[0] == EOS) { // only one center button
		TextDrawHideForPlayer(playerid, leftButtonTextDraw[0]);
		TextDrawHideForPlayer(playerid, leftButtonTextDraw[1]);

		TextDrawHideForPlayer(playerid, rightButtonTextDraw[0]);
		TextDrawHideForPlayer(playerid, rightButtonTextDraw[1]);

		TextDrawShowForPlayer(playerid, centerButtonTextDraw[0]);
		TextDrawShowForPlayer(playerid, centerButtonTextDraw[1]);

		Prev@CreatePlayerTextDraws(playerid, caption, "", "", button1, playerDialogNumListitems[playerid]);
    	PlayerTextDrawShow(playerid, centerButtonPlayerTextDraw[playerid]);
	} else { // dual buttons
		TextDrawShowForPlayer(playerid, leftButtonTextDraw[0]);
		TextDrawShowForPlayer(playerid, leftButtonTextDraw[1]);

		TextDrawShowForPlayer(playerid, rightButtonTextDraw[0]);
		TextDrawShowForPlayer(playerid, rightButtonTextDraw[1]);

		TextDrawHideForPlayer(playerid, centerButtonTextDraw[0]);
		TextDrawHideForPlayer(playerid, centerButtonTextDraw[1]);

		Prev@CreatePlayerTextDraws(playerid, caption, button1, button2, "", playerDialogNumListitems[playerid]);
    	PlayerTextDrawShow(playerid, leftButtonPlayerTextDraw[playerid]);
    	PlayerTextDrawShow(playerid, rightButtonPlayerTextDraw[playerid]);
	}

	/*
	* HEADER
	*/
	PlayerTextDrawShow(playerid, headerPlayerTextDraw[playerid]);

	/*
	* PAGE NUMBER
	*/
    PlayerTextDrawShow(playerid, pageNumPlayerTextDraw[playerid]);

	/*
	* SCROLLBAR
	*/
 	scrollBarPlayerTextDraw[playerid] = PlayerText:INVALID_TEXT_DRAW;
	if (playerDialogNumListitems[playerid] > (MAX_DIALOG_PREVIEW_ROWS * MAX_DIALOG_PREVIEW_COLUMNS)) {
        Prev@UpdateScrollBar(playerid, 0, Prev@PlayerDialogPages(playerid));
	}

	/*
	* PREVIEW MODEL LISTITEMS
	*/
	Prev@UpdateListitems(playerid, 0, 0, playerDialogNumListitems[playerid]);

	return SelectTextDraw(playerid, DIALOG_PREVIEW_SELECT_COLOR);
}

#define Dialog_ShowPreviewModelDialog \
    ShowPreviewModelDialog

#define ShowPlayerPreviewModelDialog \
    ShowPreviewModelDialog

stock SetDialogPreviewRotation(playerid, listitem, Float:rx, Float:ry, Float:rz, Float:zoom) {
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
	    return 0;
	}

	if (listitem < 0 || listitem >= MAX_DIALOG_PREVIEW_MODELS) {
	    return 0;
	}

    MEM_set_val(playerModelsRotationsArray[playerid][0], listitem, _:rx);
    MEM_set_val(playerModelsRotationsArray[playerid][1], listitem, _:ry);
    MEM_set_val(playerModelsRotationsArray[playerid][2], listitem, _:rz);
    MEM_set_val(playerModelsRotationsArray[playerid][3], listitem, _:zoom);
	return 1;
}

#define Dialog_SetListitemPreviewRot \
    SetDialogPreviewRotation

#define Dialog_SetModelRot \
    Dialog_SetListitemPreviewRot
