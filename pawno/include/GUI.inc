#if defined GUI_included
	#endinput
#endif
#define GUI_included

/**********************************************/
// GUI.inc | v1.0 | Updated: 3 Jan, 2018 | By Gammix
/**********************************************/

#if !defined MAX_CANVAS
#define MAX_CANVAS 32
#endif

#if !defined MAX_CANVAS_NAME
#define MAX_CANVAS_NAME 64
#endif

#if !defined MAX_CANVAS_ITEMS
#define MAX_CANVAS_ITEMS 64
#endif

#if !defined MAX_TEXT_DRAW_STRING
#define MAX_TEXT_DRAW_STRING 64
#endif

#define INVALID_CANVAS_ID -1

enum E_CANVAS_ITEM {
    E_CANVAS_ITEM_ID,
    E_CANVAS_ITEM_TAG
};

enum E_CANVAS {
	Float:E_CANVAS_X,
	Float:E_CANVAS_Y,
	E_CANVAS_NUM_ITEMS,
	Canvas:E_CANVAS_PARENT_CANVAS
};

enum E_TEXTDRAW {
    E_TEXTDRAW_STRING[MAX_TEXT_DRAW_STRING],
    Float:E_TEXTDRAW_POS_X,
	Float:E_TEXTDRAW_POS_Y,
	Float:E_TEXTDRAW_LETTERSIZE_X,
	Float:E_TEXTDRAW_LETTERSIZE_Y,
	Float:E_TEXTDRAW_BOX_X,
	Float:E_TEXTDRAW_BOX_Y,
	E_TEXTDRAW_ALIGNMENT,
	E_TEXTDRAW_COLOR,
	bool:E_TEXTDRAW_USE_BOX,
	E_TEXTDRAW_BOX_COLOR,
	E_TEXTDRAW_OUTLINE,
	E_TEXTDRAW_SHADOW,
	E_TEXTDRAW_BACKGROUND_COLOR,
	E_TEXTDRAW_FONT,
	bool:E_TEXTDRAW_PROPORTIONAL,
	bool:E_TEXTDRAW_SELECTABLE,
	E_TEXTDRAW_PREVIEW_MODEL,
	Float:E_TEXTDRAW_PREVIEW_ROT_X,
	Float:E_TEXTDRAW_PREVIEW_ROT_Y,
	Float:E_TEXTDRAW_PREVIEW_ROT_Z,
	Float:E_TEXTDRAW_PREVIEW_ROT_ZOOM,
	E_TEXTDRAW_PREVIEW_VEH_COLOR1,
	E_TEXTDRAW_PREVIEW_VEH_COLOR2
};

static bool:canvasVisible[MAX_PLAYERS][Canvas:MAX_CANVAS];
static canvasItems[Canvas:MAX_CANVAS][MAX_CANVAS_ITEMS][E_CANVAS_ITEM];
static canvasData[Canvas:MAX_CANVAS][E_CANVAS];

static bool:textDrawVisible[MAX_PLAYERS][Text:MAX_TEXT_DRAWS];
static textDrawData[Text:MAX_TEXT_DRAWS][E_TEXTDRAW];
static bool:textDrawValid[Text:MAX_TEXT_DRAWS];
static Canvas:textDrawParentCanvas[Text:MAX_TEXT_DRAWS];

/***********************************************
*
* INTERNAL FUNCTIONS
*
***********************************************/

forward __gui();
public __gui() {
	strcat("", "", 0);
}

static stock GetStringArg(arg, dest[], len = sizeof (dest)) {
    // Get the address of the previous function's stack.  First get the index of
    // the argument required.
    #emit LOAD.S.pri arg
    // Then convert that number to bytes from cells.
    #emit SMUL.C     4
    // Get the previous function's frame.  Stored in variable 0 (in the current
    // frame).  Parameters are FRM+n+12, locals are FRM-n, previous frame is
    // FRM+0, return address is FRM+4, parameter count is FRM+8.  We could add
    // checks that "arg * 4 < *(*(FRM + 0) + 8)", for the previous frame parameter
    // count (in C pointer speak).
    #emit LOAD.S.alt 0
    // Add the frame pointer to the argument offset in bytes.
    #emit ADD
    // Add 12 to skip over the function header.
    #emit ADD.C      12
    // Load the address stored in the specified address.
    #emit LOAD.I
    // Push the length for "strcat".
    #emit PUSH.S     len
    // Push the address we just determined was the source.
    #emit PUSH.pri
    // Load the address of the destination.
    #emit LOAD.S.alt dest
    // Blank the first cell so "strcat" behaves like "strcpy".
    #emit CONST.pri  0
    // Store the loaded number 0 to the loaded address.
    #emit STOR.I
    // Push the loaded address.
    #emit PUSH.alt
    // Push the number of parameters passed (in bytes) to the function.
    #emit PUSH.C     12
    // Call the function.
    #emit SYSREQ.C   strcat
    // Restore the stack to its level before we called this native.
    #emit STACK      16
}

static stock SetStringArg(arg, const val[], len = sizeof(val)) {
	new adrr;

	// Get the address of the previous function's stack.  First get the index of
    // the argument required.
    #emit LOAD.S.pri arg
    // Then convert that number to bytes from cells.
    #emit SMUL.C     4
    // Get the previous function's frame.
	#emit LOAD.S.alt 0
	// Add the frame pointer to the argument offset in bytes.
    #emit ADD
    // Add 12 to skip over the function header.
    #emit ADD.C      12
    // Load the address stored in the specified address.
    #emit LOAD.I
	#emit STOR.S.PRI adrr
	// Push the length (last argument first)
	#emit PUSH.S     len
	// Push the new value (source) szValue
	#emit PUSH.S     val
	// Blank out the first cell of the argument
	#emit CONST.pri  0
	#emit SREF.S.pri adrr
	// Push the destination
	#emit PUSH.S     adrr
	// Push the number of parameters passed (in bytes) to the function.
	#emit PUSH.C     12
	// Call the function.
	#emit SYSREQ.C   strcat
	// Restore the stack to its level before we called this native.
	#emit STACK      16
}

static stock UpdateTextDrawPos(&Text:text, Float:x, Float:y) {
	TextDrawDestroy(text);

    textDrawData[text][E_TEXTDRAW_POS_X] = x;
	textDrawData[text][E_TEXTDRAW_POS_Y] = y;

	text = TextDrawCreate(x, y, textDrawData[text][E_TEXTDRAW_STRING]);
	TextDrawLetterSize(text, textDrawData[text][E_TEXTDRAW_LETTERSIZE_X], textDrawData[text][E_TEXTDRAW_LETTERSIZE_Y]);
	TextDrawTextSize(text, textDrawData[text][E_TEXTDRAW_BOX_X], textDrawData[text][E_TEXTDRAW_BOX_Y]);
	TextDrawAlignment(text, textDrawData[text][E_TEXTDRAW_ALIGNMENT]);
	TextDrawColor(text, textDrawData[text][E_TEXTDRAW_COLOR]);
	TextDrawUseBox(text, textDrawData[text][E_TEXTDRAW_USE_BOX]);
	TextDrawBoxColor(text, textDrawData[text][E_TEXTDRAW_BOX_COLOR]);
	TextDrawSetShadow(text, textDrawData[text][E_TEXTDRAW_SHADOW]);
	TextDrawSetOutline(text, textDrawData[text][E_TEXTDRAW_OUTLINE]);
	TextDrawBackgroundColor(text, textDrawData[text][E_TEXTDRAW_BACKGROUND_COLOR]);
	TextDrawFont(text, textDrawData[text][E_TEXTDRAW_FONT]);
	TextDrawSetProportional(text, textDrawData[text][E_TEXTDRAW_PROPORTIONAL]);
	TextDrawSetSelectable(text, textDrawData[text][E_TEXTDRAW_SELECTABLE]);
	TextDrawSetPreviewModel(text, textDrawData[text][E_TEXTDRAW_PREVIEW_MODEL]);
	TextDrawSetPreviewRot(text, textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_X], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Y], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Z], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
	TextDrawSetPreviewVehCol(text, textDrawData[text][E_TEXTDRAW_PREVIEW_VEH_COLOR1], textDrawData[text][E_TEXTDRAW_PREVIEW_VEH_COLOR2]);
}

/***********************************************
*
* TEXTDRAW FUNCTIONS (GLOBAL)
*
***********************************************/

stock Text:CreateTextDraw(Float:x = 0.0, Float:y = 0.0, string[] = "_", Text:copyTextDraw = Text:INVALID_TEXT_DRAW) {
	if ((copyTextDraw >= Text:0 || copyTextDraw < Text:MAX_TEXT_DRAWS) && textDrawValid[copyTextDraw]) {
		new Text:text = TextDrawCreate(textDrawData[copyTextDraw][E_TEXTDRAW_POS_X], textDrawData[copyTextDraw][E_TEXTDRAW_POS_Y], textDrawData[copyTextDraw][E_TEXTDRAW_STRING]);
		if (text == Text:INVALID_TEXT_DRAW) {
			return Text:INVALID_TEXT_DRAW;
		}

		textDrawData[text] = textDrawData[copyTextDraw];

		TextDrawLetterSize(text, textDrawData[text][E_TEXTDRAW_LETTERSIZE_X], textDrawData[text][E_TEXTDRAW_LETTERSIZE_Y]);
		TextDrawTextSize(text, textDrawData[text][E_TEXTDRAW_BOX_X], textDrawData[text][E_TEXTDRAW_BOX_Y]);
		TextDrawAlignment(text, textDrawData[text][E_TEXTDRAW_ALIGNMENT]);
		TextDrawColor(text, textDrawData[text][E_TEXTDRAW_COLOR]);
		TextDrawUseBox(text, textDrawData[text][E_TEXTDRAW_USE_BOX]);
		TextDrawBoxColor(text, textDrawData[text][E_TEXTDRAW_BOX_COLOR]);
		TextDrawSetShadow(text, textDrawData[text][E_TEXTDRAW_SHADOW]);
		TextDrawSetOutline(text, textDrawData[text][E_TEXTDRAW_OUTLINE]);
		TextDrawBackgroundColor(text, textDrawData[text][E_TEXTDRAW_BACKGROUND_COLOR]);
		TextDrawFont(text, textDrawData[text][E_TEXTDRAW_FONT]);
		TextDrawSetProportional(text, textDrawData[text][E_TEXTDRAW_PROPORTIONAL]);
		TextDrawSetSelectable(text, textDrawData[text][E_TEXTDRAW_SELECTABLE]);
		TextDrawSetPreviewModel(text, textDrawData[text][E_TEXTDRAW_PREVIEW_MODEL]);
		TextDrawSetPreviewRot(text, textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_X], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Y], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Z], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
		TextDrawSetPreviewVehCol(text, textDrawData[text][E_TEXTDRAW_PREVIEW_VEH_COLOR1], textDrawData[text][E_TEXTDRAW_PREVIEW_VEH_COLOR2]);

		return text;
	}

	new Text:text = TextDrawCreate(x, y, string);
	if (text == Text:INVALID_TEXT_DRAW) {
		return Text:INVALID_TEXT_DRAW;
	}

	textDrawValid[text] = true;
	textDrawParentCanvas[text] = Canvas:INVALID_CANVAS_ID;

	format(textDrawData[text][E_TEXTDRAW_STRING], MAX_TEXT_DRAW_STRING, string);
	textDrawData[text][E_TEXTDRAW_POS_X] = x;
	textDrawData[text][E_TEXTDRAW_POS_Y] = y;
	textDrawData[text][E_TEXTDRAW_LETTERSIZE_X] = 0.5;
	textDrawData[text][E_TEXTDRAW_LETTERSIZE_Y] = 1.0;
	textDrawData[text][E_TEXTDRAW_BOX_X] = 0.0;
	textDrawData[text][E_TEXTDRAW_BOX_Y] = 0.0;
	textDrawData[text][E_TEXTDRAW_ALIGNMENT] = 1;
	textDrawData[text][E_TEXTDRAW_COLOR] = 0xFFFFFFFF;
	textDrawData[text][E_TEXTDRAW_USE_BOX] = false;
	textDrawData[text][E_TEXTDRAW_BOX_COLOR] = 0x000000FF;
	textDrawData[text][E_TEXTDRAW_SHADOW] = 1;
	textDrawData[text][E_TEXTDRAW_OUTLINE] = 0;
	textDrawData[text][E_TEXTDRAW_BACKGROUND_COLOR] = 0x000000FF;
	textDrawData[text][E_TEXTDRAW_FONT] = 1;
	textDrawData[text][E_TEXTDRAW_PROPORTIONAL] = true;
	textDrawData[text][E_TEXTDRAW_SELECTABLE] = false;
	textDrawData[text][E_TEXTDRAW_PREVIEW_MODEL] = 0;
	textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_X] = 0.0;
	textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Y] = 0.0;
	textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Z] = 0.0;
	textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_ZOOM] = 1.0;
	textDrawData[text][E_TEXTDRAW_PREVIEW_VEH_COLOR1] = -1;
	textDrawData[text][E_TEXTDRAW_PREVIEW_VEH_COLOR2] = -1;

	TextDrawLetterSize(text, textDrawData[text][E_TEXTDRAW_LETTERSIZE_X], textDrawData[text][E_TEXTDRAW_LETTERSIZE_Y]);
	TextDrawTextSize(text, textDrawData[text][E_TEXTDRAW_BOX_X], textDrawData[text][E_TEXTDRAW_BOX_Y]);
	TextDrawAlignment(text, textDrawData[text][E_TEXTDRAW_ALIGNMENT]);
	TextDrawColor(text, textDrawData[text][E_TEXTDRAW_COLOR]);
	TextDrawUseBox(text, textDrawData[text][E_TEXTDRAW_USE_BOX]);
	TextDrawBoxColor(text, textDrawData[text][E_TEXTDRAW_BOX_COLOR]);
	TextDrawSetShadow(text, textDrawData[text][E_TEXTDRAW_SHADOW]);
	TextDrawSetOutline(text, textDrawData[text][E_TEXTDRAW_OUTLINE]);
	TextDrawBackgroundColor(text, textDrawData[text][E_TEXTDRAW_BACKGROUND_COLOR]);
	TextDrawFont(text, textDrawData[text][E_TEXTDRAW_FONT]);
	TextDrawSetProportional(text, textDrawData[text][E_TEXTDRAW_PROPORTIONAL]);
	TextDrawSetSelectable(text, textDrawData[text][E_TEXTDRAW_SELECTABLE]);
	TextDrawSetPreviewModel(text, textDrawData[text][E_TEXTDRAW_PREVIEW_MODEL]);
	TextDrawSetPreviewRot(text, textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_X], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Y], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Z], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
	TextDrawSetPreviewVehCol(text, textDrawData[text][E_TEXTDRAW_PREVIEW_VEH_COLOR1], textDrawData[text][E_TEXTDRAW_PREVIEW_VEH_COLOR2]);

	return text;
}

stock DestroyTextDraw(Text:text) {
	if (text < Text:0 || text >= Text:MAX_TEXT_DRAWS) {
		return 0;
	}

	if (!textDrawValid[text]) {
		return 0;
	}

	TextDrawDestroy(text);
	textDrawValid[text] = false;
	return 1;
}

stock SetTextDrawProperty(Text:text, E_TEXTDRAW:type, {Float, _}:...) {
	if (text < Text:0 || text >= Text:MAX_TEXT_DRAWS) {
		return 0;
	}

	if (!textDrawValid[text]) {
		return 0;
	}

	switch (type) {
		case E_TEXTDRAW_STRING: {
            GetStringArg(2, textDrawData[text][E_TEXTDRAW_STRING], MAX_TEXT_DRAW_STRING);
            TextDrawSetString(text, textDrawData[text][E_TEXTDRAW_STRING]);
		}

		case E_TEXTDRAW_POS_X: {
            textDrawData[text][E_TEXTDRAW_POS_X] = Float:getarg(2);
            UpdateTextDrawPos(text, textDrawData[text][E_TEXTDRAW_POS_X], textDrawData[text][E_TEXTDRAW_POS_Y]);
		}

		case E_TEXTDRAW_POS_Y: {
            textDrawData[text][E_TEXTDRAW_POS_Y] = Float:getarg(2);
            UpdateTextDrawPos(text, textDrawData[text][E_TEXTDRAW_POS_X], textDrawData[text][E_TEXTDRAW_POS_Y]);
		}

		case E_TEXTDRAW_LETTERSIZE_X: {
            textDrawData[text][E_TEXTDRAW_LETTERSIZE_X] = Float:getarg(2);
            TextDrawLetterSize(text, textDrawData[text][E_TEXTDRAW_LETTERSIZE_X], textDrawData[text][E_TEXTDRAW_LETTERSIZE_Y]);
		}

		case E_TEXTDRAW_LETTERSIZE_Y: {
            textDrawData[text][E_TEXTDRAW_LETTERSIZE_Y] = Float:getarg(2);
            TextDrawLetterSize(text, textDrawData[text][E_TEXTDRAW_LETTERSIZE_X], textDrawData[text][E_TEXTDRAW_LETTERSIZE_Y]);
		}

		case E_TEXTDRAW_BOX_X: {
            textDrawData[text][E_TEXTDRAW_BOX_X] = Float:getarg(2);
            TextDrawTextSize(text, textDrawData[text][E_TEXTDRAW_BOX_X], textDrawData[text][E_TEXTDRAW_BOX_Y]);
		}

		case E_TEXTDRAW_BOX_Y: {
            textDrawData[text][E_TEXTDRAW_BOX_Y] = Float:getarg(2);
            TextDrawTextSize(text, textDrawData[text][E_TEXTDRAW_BOX_X], textDrawData[text][E_TEXTDRAW_BOX_Y]);
		}

		case E_TEXTDRAW_ALIGNMENT: {
            textDrawData[text][E_TEXTDRAW_ALIGNMENT] = getarg(2);
            TextDrawAlignment(text, textDrawData[text][E_TEXTDRAW_ALIGNMENT]);
		}

		case E_TEXTDRAW_COLOR: {
            textDrawData[text][E_TEXTDRAW_COLOR] = getarg(2);
            TextDrawColor(text, textDrawData[text][E_TEXTDRAW_COLOR]);
		}

		case E_TEXTDRAW_USE_BOX: {
            textDrawData[text][E_TEXTDRAW_USE_BOX] = bool:getarg(2);
            TextDrawUseBox(text, textDrawData[text][E_TEXTDRAW_USE_BOX]);
		}

		case E_TEXTDRAW_BOX_COLOR: {
            textDrawData[text][E_TEXTDRAW_BOX_COLOR] = bool:getarg(2);
            TextDrawBoxColor(text, textDrawData[text][E_TEXTDRAW_BOX_COLOR]);
		}

		case E_TEXTDRAW_SHADOW: {
            textDrawData[text][E_TEXTDRAW_SHADOW] = getarg(2);
            TextDrawSetShadow(text, textDrawData[text][E_TEXTDRAW_SHADOW]);
		}

		case E_TEXTDRAW_OUTLINE: {
            textDrawData[text][E_TEXTDRAW_OUTLINE] = getarg(2);
            TextDrawSetOutline(text, textDrawData[text][E_TEXTDRAW_OUTLINE]);
		}

		case E_TEXTDRAW_BACKGROUND_COLOR: {
            textDrawData[text][E_TEXTDRAW_BACKGROUND_COLOR] = getarg(2);
            TextDrawBackgroundColor(text, textDrawData[text][E_TEXTDRAW_BACKGROUND_COLOR]);
		}

		case E_TEXTDRAW_FONT: {
            textDrawData[text][E_TEXTDRAW_FONT] = getarg(2);
            TextDrawFont(text, textDrawData[text][E_TEXTDRAW_FONT]);
		}

		case E_TEXTDRAW_PROPORTIONAL: {
            textDrawData[text][E_TEXTDRAW_PROPORTIONAL] = bool:getarg(2);
            TextDrawSetProportional(text, textDrawData[text][E_TEXTDRAW_PROPORTIONAL]);
		}

		case E_TEXTDRAW_SELECTABLE: {
            textDrawData[text][E_TEXTDRAW_SELECTABLE] = bool:getarg(2);
            TextDrawSetSelectable(text, textDrawData[text][E_TEXTDRAW_SELECTABLE]);
		}

		case E_TEXTDRAW_PREVIEW_MODEL: {
            textDrawData[text][E_TEXTDRAW_PREVIEW_MODEL] = getarg(2);
            TextDrawSetPreviewModel(text, textDrawData[text][E_TEXTDRAW_PREVIEW_MODEL]);
		}

		case E_TEXTDRAW_PREVIEW_ROT_X: {
            textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_X] = Float:getarg(2);
			TextDrawSetPreviewRot(text, textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_X], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Y], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Z], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
		}

		case E_TEXTDRAW_PREVIEW_ROT_Y: {
            textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Y] = Float:getarg(2);
			TextDrawSetPreviewRot(text, textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_X], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Y], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Z], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
		}

		case E_TEXTDRAW_PREVIEW_ROT_Z: {
            textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Z] = Float:getarg(2);
			TextDrawSetPreviewRot(text, textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_X], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Y], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Z], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
		}

		case E_TEXTDRAW_PREVIEW_ROT_ZOOM: {
            textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_ZOOM] = Float:getarg(2);
			TextDrawSetPreviewRot(text, textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_X], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Y], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_Z], textDrawData[text][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
		}

		case E_TEXTDRAW_PREVIEW_VEH_COLOR1: {
            textDrawData[text][E_TEXTDRAW_PREVIEW_VEH_COLOR1] = getarg(2);
			TextDrawSetPreviewVehCol(text, textDrawData[text][E_TEXTDRAW_PREVIEW_VEH_COLOR1], textDrawData[text][E_TEXTDRAW_PREVIEW_VEH_COLOR2]);
		}

		case E_TEXTDRAW_PREVIEW_VEH_COLOR2: {
            textDrawData[text][E_TEXTDRAW_PREVIEW_VEH_COLOR2] = getarg(2);
			TextDrawSetPreviewVehCol(text, textDrawData[text][E_TEXTDRAW_PREVIEW_VEH_COLOR1], textDrawData[text][E_TEXTDRAW_PREVIEW_VEH_COLOR2]);
		}
	}
	return 1;
}

stock GetTextDrawProperty(Text:text, E_TEXTDRAW:type, {Float, _}:...) {
	if (text < Text:0 || text >= Text:MAX_TEXT_DRAWS) {
		return 0;
	}

	if (!textDrawValid[text]) {
		return 0;
	}

	switch (type) {
		case E_TEXTDRAW_STRING: {
			SetStringArg(2, textDrawData[text][E_TEXTDRAW_STRING], MAX_TEXT_DRAW_STRING);
		}

		case E_TEXTDRAW_POS_X,
			 E_TEXTDRAW_POS_Y,
	 		 E_TEXTDRAW_LETTERSIZE_X,
			 E_TEXTDRAW_LETTERSIZE_Y,
			 E_TEXTDRAW_BOX_X,
			 E_TEXTDRAW_BOX_Y,
			 E_TEXTDRAW_PREVIEW_ROT_X,
			 E_TEXTDRAW_PREVIEW_ROT_Y,
			 E_TEXTDRAW_PREVIEW_ROT_Z,
			 E_TEXTDRAW_PREVIEW_ROT_ZOOM:
	 	{
		 	setarg(2, _, _:textDrawData[text][type]);
		}

		case E_TEXTDRAW_ALIGNMENT,
			 E_TEXTDRAW_COLOR,
			 E_TEXTDRAW_USE_BOX,
			 E_TEXTDRAW_BOX_COLOR,
			 E_TEXTDRAW_SHADOW,
			 E_TEXTDRAW_OUTLINE,
			 E_TEXTDRAW_BACKGROUND_COLOR,
			 E_TEXTDRAW_FONT,
			 E_TEXTDRAW_PROPORTIONAL,
			 E_TEXTDRAW_SELECTABLE,
			 E_TEXTDRAW_PREVIEW_MODEL,
			 E_TEXTDRAW_PREVIEW_VEH_COLOR1,
			 E_TEXTDRAW_PREVIEW_VEH_COLOR2:
	 	{
		 	setarg(2, _, _:textDrawData[text][type]);
		}
	}
	return 1;
}

stock UpdateTextDraw(Text:text, playerid = -1) {
	if (text < Text:0 || text >= Text:MAX_TEXT_DRAWS) {
		return 0;
	}

	if (!textDrawValid[text]) {
		return 0;
	}

	if (playerid != -1 && (playerid < 0 || playerid >= MAX_PLAYERS)) {
		return 0;
	}

	if (textDrawParentCanvas[text] != Canvas:INVALID_CANVAS_ID) {
	    new Canvas:canvas = textDrawParentCanvas[text];

		if (playerid >= 0 || playerid < MAX_PLAYERS) {
	 		if (canvasVisible[playerid][canvas]) {
    			if (textDrawVisible[playerid][text]) {
					TextDrawShowForPlayer(playerid, text);
				}
			}
		}
		else {
  			for (new i = 0; i < MAX_PLAYERS; i++) {
     			if (canvasVisible[i][canvas]) {
					if (textDrawVisible[i][text]) {
						TextDrawShowForPlayer(i, text);
					}
				}
			}
		}

		return 1;
	}

	if (playerid >= 0 || playerid < MAX_PLAYERS) {
 		if (textDrawVisible[playerid][text]) {
			TextDrawShowForPlayer(playerid, text);
		}
	}
	else {
 		for (new i = 0; i < MAX_PLAYERS; i++) {
   			if (textDrawVisible[i][text]) {
				TextDrawShowForPlayer(i, text);
			}
		}
	}

	return 0;
}

stock SetTextDrawVisibility(Text:text, bool:toggle, playerid = -1) {
	if (text < Text:0 || text >= Text:MAX_TEXT_DRAWS) {
		return 0;
	}

	if (!textDrawValid[text]) {
		return 0;
	}

	if (playerid != -1 && (playerid < 0 || playerid >= MAX_PLAYERS)) {
		return 0;
	}

	if (toggle) {
		if (textDrawParentCanvas[text] != Canvas:INVALID_CANVAS_ID) {
		    new Canvas:canvas = textDrawParentCanvas[text];

			if (playerid >= 0 || playerid < MAX_PLAYERS) {
       			textDrawVisible[playerid][text] = true;

			 	if (canvasVisible[playerid][canvas]) {
					TextDrawShowForPlayer(playerid, text);
				}
			}
			else {
	  			for (new i = 0; i < MAX_PLAYERS; i++) {
					textDrawVisible[i][text] = true;

	     			if (canvasVisible[i][canvas]) {
						TextDrawShowForPlayer(i, text);
					}
				}
			}

			return 1;
		}
	}

	if (playerid >= 0 || playerid < MAX_PLAYERS) {
		textDrawVisible[playerid][text] = toggle;

		if (toggle) {
			TextDrawShowForPlayer(playerid, text);
		}
		else {
			TextDrawHideForPlayer(playerid, text);
		}
	}
	else {
	    for (new i = 0; i < MAX_PLAYERS; i++) {
			textDrawVisible[i][text] = toggle;
		}

		if (toggle) {
			TextDrawShowForAll(text);
		}
		else {
			TextDrawHideForAll(text);
		}
	}

	return 1;
}

stock IsTextDrawVisible(Text:text, playerid) {
	if (text < Text:0 || text >= Text:MAX_TEXT_DRAWS) {
		return 0;
	}

	if (!textDrawValid[text]) {
		return 0;
	}

	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
  	}

	return textDrawVisible[playerid][text];
}

stock IsTextDrawVisibleOnScreen(Text:text, playerid) {
	if (text < Text:0 || text >= Text:MAX_TEXT_DRAWS) {
		return 0;
	}

	if (!textDrawValid[text]) {
		return 0;
	}

	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
  	}

    if (textDrawParentCanvas[text] != Canvas:INVALID_CANVAS_ID) {
		if (canvasVisible[playerid][textDrawParentCanvas[text]]) {
			return textDrawVisible[playerid][text];
		}
		else {
		    return 0;
		}
	}

	return textDrawVisible[playerid][text];
}

/***********************************************
*
* CANVAS FUNCTIONS
*
***********************************************/

stock Canvas:CreateCanvas(Canvas:copyCanvas = Canvas:INVALID_CANVAS_ID) {
	new Canvas:index = Canvas:INVALID_CANVAS_ID;
	
	for (new Canvas:i = Canvas:0; i < Canvas:MAX_CANVAS; i++) {
		if (canvasData[i][E_CANVAS_NUM_ITEMS] == 0) {
			index = i;
			break;
		}
	}
	
	if (index == Canvas:INVALID_CANVAS_ID) {
		return Canvas:INVALID_CANVAS_ID;
	}

	if ((copyCanvas >= Canvas:0 || copyCanvas < Canvas:MAX_CANVAS) && canvasData[copyCanvas][E_CANVAS_NUM_ITEMS] != 0) {
        for (new i = 0; i < canvasData[copyCanvas][E_CANVAS_NUM_ITEMS]; i++) {
            canvasItems[index][i][E_CANVAS_ITEM_TAG] = canvasItems[copyCanvas][i][E_CANVAS_ITEM_TAG];
            
			if (canvasItems[index][i][E_CANVAS_ITEM_TAG] == tagof(Canvas:)) {
                CreateCanvas(Canvas:canvasItems[index][i][E_CANVAS_ITEM_ID]);
			}
			else {
                CreateTextDraw(.copyTextDraw = Text:canvasItems[index][i][E_CANVAS_ITEM_ID]);
			}
		}
        
		canvasData[index] = canvasData[copyCanvas];

		for (new i = 0; i < MAX_PLAYERS; i++) {
			canvasVisible[i][index] = canvasVisible[i][copyCanvas];
		}
		return index;
	}

	canvasData[index][E_CANVAS_X] = 0.0;
	canvasData[index][E_CANVAS_Y] = 0.0;
	canvasData[index][E_CANVAS_NUM_ITEMS] = 0;
	canvasData[index][E_CANVAS_PARENT_CANVAS] = Canvas:INVALID_CANVAS_ID;
	
	for (new i = 0; i < MAX_PLAYERS; i++) {
		canvasVisible[i][index] = true;
	}
	return index;
}

stock AddItemToCanvas(Canvas:canvas, {Canvas, Text}:item, tag = tagof(item)) {
	if (canvas < Canvas:0 || canvas >= Canvas:MAX_CANVAS) {
		return -1;
	}

	if (canvasData[canvas][E_CANVAS_NUM_ITEMS] == MAX_CANVAS_ITEMS) {
		return -1;
	}

	new itemid = canvasData[canvas][E_CANVAS_NUM_ITEMS]++;
	canvasItems[canvas][itemid][E_CANVAS_ITEM_ID] = _:item;
	canvasItems[canvas][itemid][E_CANVAS_ITEM_TAG] = tag;

	if (tag == tagof(Canvas:)) {
	    canvasData[Canvas:item][E_CANVAS_PARENT_CANVAS] = canvas;

		for (new i = 0; i < MAX_PLAYERS; i++) {
			SetCanvasVisibility(Canvas:item, canvasVisible[i][canvas], i);
		}
	}
	else {
	    for (new i = 0; i < MAX_PLAYERS; i++) {
			SetTextDrawVisibility(Text:item, canvasVisible[i][canvas], i);
		}
	}
	return itemid;
}

stock RemoveItemFromCanvas(Canvas:canvas, {Canvas, Text}:item, tag = tagof(item)) {
	if (canvas < Canvas:0 || canvas >= Canvas:MAX_CANVAS) {
		return 0;
	}

	if (_:item < 0 || _:item >= canvasData[canvas][E_CANVAS_NUM_ITEMS]) {
		return 0;
	}

	for (new i = 0; i < canvasData[canvas][E_CANVAS_NUM_ITEMS]; i++) {
	    if (canvasItems[canvas][i][E_CANVAS_ITEM_TAG] == tag &&
			_:canvasItems[canvas][i][E_CANVAS_ITEM_ID] == _:item)
		{
		    if (tag == tagof(Canvas:)) {
                canvasData[Canvas:item][E_CANVAS_PARENT_CANVAS] = Canvas:INVALID_CANVAS_ID;
			}
			
			for (; i < (canvasData[canvas][E_CANVAS_NUM_ITEMS] - 1); i++) {
                canvasItems[canvas][i] = canvasItems[canvas][i + 1];
			}
			canvasData[canvas][E_CANVAS_NUM_ITEMS]--;
		    return 1;
	    }
	}

	return 0;
}

stock GetCanvasNumItems(Canvas:canvas) {
	if (canvas < Canvas:0 || canvas >= Canvas:MAX_CANVAS) {
		return -1;
	}

	return canvasData[canvas][E_CANVAS_NUM_ITEMS];
}

stock GetCanvasItem(Canvas:canvas, item, {Canvas, Text}:arg) {
	#pragma unused arg
	
	if (canvas < Canvas:0 || canvas >= Canvas:MAX_CANVAS) {
		return 0;
	}

	if (item < 0 || item >= canvasData[canvas][E_CANVAS_NUM_ITEMS]) {
		return 0;
	}

	return setarg(2, _, canvasItems[canvas][item][E_CANVAS_ITEM_ID]);
}

stock GetCanvasType(Canvas:canvas, item) {
	if (canvas < Canvas:0 || canvas >= Canvas:MAX_CANVAS) {
		return 0;
	}

	if (item < 0 || item >= canvasData[canvas][E_CANVAS_NUM_ITEMS]) {
		return 0;
	}

	return canvasItems[canvas][item][E_CANVAS_ITEM_TAG];
}

stock SetCanvasVisibility(Canvas:canvas, bool:toggle, playerid = -1) {
	if (canvas < Canvas:0 || canvas >= Canvas:MAX_CANVAS) {
		return 0;
	}

	if (canvasData[canvas][E_CANVAS_NUM_ITEMS] == 0) {
		return 0;
	}
	
	if (playerid != -1 && (playerid < 0 || playerid >= MAX_PLAYERS)) {
		return 0;
	}
    
    for (new i = 0; i < canvasData[canvas][E_CANVAS_NUM_ITEMS]; i++) {
		if (canvasItems[canvas][i][E_CANVAS_ITEM_TAG] == tagof(Canvas:)) {
		    if (playerid >= 0 || playerid < MAX_PLAYERS) {
    			canvasVisible[playerid][canvas] = toggle;
            }
            else if (playerid == -1) {
			    for (new x = 0; x < MAX_PLAYERS; x++) {
    				canvasVisible[x][canvas] = toggle;
				}
			}
			
		    SetCanvasVisibility(Canvas:canvasItems[canvas][i][E_CANVAS_ITEM_ID], toggle, playerid);
		}
		else {
		    SetTextDrawVisibility(Text:canvasItems[canvas][i][E_CANVAS_ITEM_ID], toggle, playerid);
		}
	}
	return 1;
}

stock IsCanvasVisible(Canvas:canvas, playerid) {
	if (canvas < Canvas:0 || canvas >= Canvas:MAX_CANVAS) {
		return 0;
	}
	
	if (canvasData[canvas][E_CANVAS_NUM_ITEMS] == 0) {
		return 0;
	}
	
	if (playerid < 0 || playerid >= MAX_PLAYERS) {
		return 0;
  	}

  	return canvasVisible[playerid][canvas];
}

stock SetCanvasPos(Canvas:canvas, Float:x, Float:y) {
	if (canvas < Canvas:0 || canvas >= Canvas:MAX_CANVAS) {
		return 0;
	}

	new Text:textdraw;
	new Float:dist;

    for (new i = 0; i < canvasData[canvas][E_CANVAS_NUM_ITEMS]; i++) {
		if (canvasItems[canvas][i][E_CANVAS_ITEM_TAG] == tagof(Canvas:)) {
		    SetCanvasPos(Canvas:canvasItems[canvas][i][E_CANVAS_ITEM_ID], x, y);
		}
		else {
			textdraw = Text:canvasItems[canvas][i][E_CANVAS_ITEM_ID];

			dist = (canvasData[canvas][E_CANVAS_X] + textDrawData[textdraw][E_TEXTDRAW_POS_X]);
			SetTextDrawProperty(textdraw, E_TEXTDRAW_POS_X, (x + dist));

			dist = (canvasData[canvas][E_CANVAS_Y] + textDrawData[textdraw][E_TEXTDRAW_POS_Y]);
			SetTextDrawProperty(textdraw, E_TEXTDRAW_POS_Y, (y + dist));

			if (textDrawData[textdraw][E_TEXTDRAW_FONT] >= 0 && textDrawData[textdraw][E_TEXTDRAW_FONT] <= 3) {
				//SetTextDrawProperty(textdraw, E_TEXTDRAW_BOX_X, (textDrawData[textdraw][E_TEXTDRAW_POS_X] - dist));
			}

			UpdateTextDraw(textdraw);
		}
	}

	canvasData[canvas][E_CANVAS_X] = x;
	canvasData[canvas][E_CANVAS_Y] = y;
	return 1;
}

stock GetCanvasPos(Canvas:canvas, &Float:x, &Float:y) {
	if (canvas < Canvas:0 || canvas >= Canvas:MAX_CANVAS) {
		return 0;
	}

	x = canvasData[canvas][E_CANVAS_X];
	y = canvasData[canvas][E_CANVAS_Y];
	return 1;
}
