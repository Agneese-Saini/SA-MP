// TDEditor.pwn by Gammix
// v1.2 - Last updated: 0 Nov, 2017
#define FILTERSCRIPT

#include <a_samp>
#include <zcmd>
#include <easydialog>
#include <timestamptodate>
#include <dini2>
#include <sscanf2>

#define PATH_PROJECT_FILES		"TDEditor/Projects/"
#define PATH_EXPORT_FILES		"TDEditor/Exports/"
#define PATH_RECORD_FILE        "TDEditor/Records.txt"
#define PATH_OBJECTS_FILE       "TDEditor/ObjectsData.db"

#define MAX_PROJECTS            32
#define MAX_PROJECT_NAME 		64

#define MAX_GROUPS 				32
#define MAX_GROUP_NAME 			16
#define MAX_GROUP_TEXTDRAWS 	25
#define MAX_GROUP_TEXTDRAW_TEXT 256

#define MAX_GROUP_TEXTDRAW_PREVIEW_CHARS 16

#define MESSAGE_COLOR           (0xfffd91ff)

#define COL_WHITE				"{ffffff}"
#define COL_GREEN 				"{93ffbc}"
#define COL_RED 				"{ff7f7f}"
#define COL_ORANGE 				"{ffa172}"
#define COL_BLUE 				"{96ceff}"
#define COL_YELLOW				"{f1ff96}"
#define COL_GREY				"{c4c4c4}"

enum {
	EDITING_NONE,
	EDITING_POS,
	EDITING_LETTER_SIZE,
	EDITING_TEXT_SIZE,
	EDITING_SHADOW_SIZE,
	EDITING_OUTLINE_SIZE,
	EDITING_PREVIEW_ROT,
	EDITING_BACKGROUND_COLOR,
	EDITING_TEXTDRAW_COLOR,
	EDITING_BOX_COLOR,
	EDITING_GROUP_POS
};

enum {
	COLOR_ELEMENT_TEXTDRAW,
	COLOR_ELEMENT_BOX,
	COLOR_ELEMENT_BACKGROUND
};

enum E_GROUP {
	E_GROUP_NAME[MAX_GROUP_NAME],
	E_GROUP_TEXTDRAWS_COUNT,
	bool:E_GROUP_VISIBLE
};

enum E_TEXTDRAW {
	E_TEXTDRAW_SQLID,
    Text:E_TEXTDRAW_ID,
    E_TEXTDRAW_TEXT[MAX_GROUP_TEXTDRAW_TEXT],
    Float:E_TEXTDRAW_X,
	Float:E_TEXTDRAW_Y,
	Float:E_TEXTDRAW_LETTERSIZE_X,
	Float:E_TEXTDRAW_LETTERSIZE_Y,
	Float:E_TEXTDRAW_TEXTSIZE_X,
	Float:E_TEXTDRAW_TEXTSIZE_Y,
	E_TEXTDRAW_ALIGNMENT,
	E_TEXTDRAW_COLOR,
	bool:E_TEXTDRAW_USE_BOX,
	E_TEXTDRAW_BOX_COLOR,
	E_TEXTDRAW_SHADOW,
	E_TEXTDRAW_OUTLINE,
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
	E_TEXTDRAW_PREVIEW_VEH_COLOR2,
	bool:E_TEXTDRAW_TYPE_PLAYER
};

enum E_COLOR {
	E_COLOR_NAME[32],
	E_COLOR_CODE
};

new const COLORS[][E_COLOR] = {
	{"AntiqueWhite",	0xFAEBD7FF},
	{"Aqua",			0x00FFFFFF},
	{"Aquamarine",		0x7FFFD4FF},
	{"Azure",			0xF0FFFFFF},
	{"Beige",			0xF5F5DCFF},
	{"Bisque",			0xFFE4C4FF},
	{"Black",			0x000000FF},
	{"BlanchedAlmond",	0xFFEBCDFF},
	{"Blue",			0x0000FFFF},
	{"BlueViolet",		0x8A2BE2FF},
	{"Brown",			0xA52A2AFF},
	{"BurlyWood",		0xDEB887FF},
	{"CadetBlue",		0x5F9EA0FF},
	{"Chartreuse",		0x7FFF00FF},
	{"Chocolate",		0xD2691EFF},
	{"Coral",			0xFF7F50FF},
	{"CornflowerBlue",	0x6495EDFF},
	{"Cornsilk",		0xFFF8DCFF},
	{"Crimson",			0xDC143CFF},
	{"Cyan",			0x00FFFFFF},
	{"DarkBlue",		0x00008BFF},
	{"DarkCyan",		0x008B8BFF},
	{"DarkGoldenRod",	0xB8860BFF},
	{"DarkGray",		0xA9A9A9FF},
	{"DarkGrey",		0xA9A9A9FF},
	{"DarkGreen",		0x006400FF},
	{"DarkKhaki",		0xBDB76BFF},
	{"DarkMagenta",		0x8B008BFF},
	{"DarkOliveGreen",	0x556B2FFF},
	{"DarkOrange",		0xFF8C00FF},
	{"DarkOrchid",		0x9932CCFF},
	{"DarkRed",			0x8B0000FF},
	{"DarkSalmon",		0xE9967AFF},
	{"DarkSeaGreen",	0x8FBC8FFF},
	{"DarkSlateBlue",	0x483D8BFF},
	{"DarkSlateGray",	0x2F4F4FFF},
	{"DarkSlateGrey",	0x2F4F4FFF},
	{"DarkTurquoise",	0x00CED1FF},
	{"DarkViolet",		0x9400D3FF},
	{"DeepPink",		0xFF1493FF},
	{"DeepSkyBlue",		0x00BFFFFF},
	{"DimGray",			0x696969FF},
	{"DimGrey",			0x696969FF},
	{"DodgerBlue",		0x1E90FFFF},
	{"FireBrick",		0xB22222FF},
	{"FloralWhite",		0xFFFAF0FF},
	{"ForestGreen",		0x228B22FF},
	{"Fuchsia",			0xFF00FFFF},
	{"Gainsboro",		0xDCDCDCFF},
	{"GhostWhite",		0xF8F8FFFF},
	{"Gold",			0xFFD700FF},
	{"GoldenRod",		0xDAA520FF},
	{"Gray",			0x808080FF},
	{"Grey",			0x808080FF},
	{"Green",			0x008000FF},
	{"GreenYellow",		0xADFF2FFF},
	{"HoneyDew",		0xF0FFF0FF},
	{"HotPink",			0xFF69B4FF},
	{"IndianRed ",		0xCD5C5CFF},
	{"Indigo ",			0x4B0082FF},
	{"Ivory",			0xFFFFF0FF},
	{"Khaki",			0xF0E68CFF},
	{"Lavender",		0xE6E6FAFF},
	{"LavenderBlush",	0xFFF0F5FF},
	{"LawnGreen",		0x7CFC00FF},
	{"LemonChiffon",	0xFFFACDFF},
	{"LightBlue",		0xADD8E6FF},
	{"LightCoral",		0xF08080FF},
	{"LightCyan",		0xE0FFFFFF},
	{"LightGoldenRodYellow", 0xFAFAD2FF},
	{"LightGray",		0xD3D3D3FF},
	{"LightGrey",		0xD3D3D3FF},
	{"LightGreen",		0x90EE90FF},
	{"LightPink",		0xFFB6C1FF},
	{"LightSalmon",		0xFFA07AFF},
	{"LightSeaGreen",	0x20B2AAFF},
	{"LightSkyBlue",	0x87CEFAFF},
	{"LightSlateGray",	0x778899FF},
	{"LightSlateGrey",	0x778899FF},
	{"LightSteelBlue",	0xB0C4DEFF},
	{"LightYellow",		0xFFFFE0FF},
	{"Lime",			0x00FF00FF},
	{"LimeGreen",		0x32CD32FF},
	{"Linen",			0xFAF0E6FF},
	{"Magenta",			0xFF00FFFF},
	{"Maroon",			0x800000FF},
	{"MediumAquaMarine",0x66CDAAFF},
	{"MediumBlue",		0x0000CDFF},
	{"MediumOrchid",	0xBA55D3FF},
	{"MediumPurple",	0x9370DBFF},
	{"MediumSeaGreen",	0x3CB371FF},
	{"MediumSlateBlue",	0x7B68EEFF},
	{"MediumSpringGreen",0x00FA9AFF},
	{"MediumTurquoise",	0x48D1CCFF},
	{"MediumVioletRed",	0xC71585FF},
	{"MidnightBlue",	0x191970FF},
	{"MintCream",		0xF5FFFAFF},
	{"MistyRose",		0xFFE4E1FF},
	{"Moccasin",		0xFFE4B5FF},
	{"NavajoWhite",		0xFFDEADFF},
	{"Navy",			0x000080FF},
	{"OldLace",			0xFDF5E6FF},
	{"Olive",			0x808000FF},
	{"OliveDrab",		0x6B8E23FF},
	{"Orange",			0xFFA500FF},
	{"OrangeRed",		0xFF4500FF},
	{"Orchid",			0xDA70D6FF},
	{"PaleGoldenRod",	0xEEE8AAFF},
	{"PaleGreen",		0x98FB98FF},
	{"PaleTurquoise",	0xAFEEEEFF},
	{"PaleVioletRed",	0xDB7093FF},
	{"PapayaWhip",		0xFFEFD5FF},
	{"PeachPuff",		0xFFDAB9FF},
	{"Peru",			0xCD853FFF},
	{"Pink",			0xFFC0CBFF},
	{"Plum",			0xDDA0DDFF},
	{"PowderBlue",		0xB0E0E6FF},
	{"Purple",			0x800080FF},
	{"RebeccaPurple",	0x663399FF},
	{"Red",				0xFF0000FF},
	{"RosyBrown",		0xBC8F8FFF},
	{"RoyalBlue",		0x4169E1FF},
	{"SaddleBrown",		0x8B4513FF},
	{"Salmon",			0xFA8072FF},
	{"SandyBrown",		0xF4A460FF},
	{"SeaGreen",		0x2E8B57FF},
	{"SeaShell",		0xFFF5EEFF},
	{"Sienna",			0xA0522DFF},
	{"Silver",			0xC0C0C0FF},
	{"SkyBlue",			0x87CEEBFF},
	{"SlateBlue",		0x6A5ACDFF},
	{"SlateGray",		0x708090FF},
	{"SlateGrey",		0x708090FF},
	{"Snow",			0xFFFAFAFF},
	{"SpringGreen",		0x00FF7FFF},
	{"SteelBlue",		0x4682B4FF},
	{"Tan",				0xD2B48CFF},
	{"Teal",			0x008080FF},
	{"Thistle",			0xD8BFD8FF},
	{"Tomato",			0xFF6347FF},
	{"Turquoise",		0x40E0D0FF},
	{"Violet",			0xEE82EEFF},
	{"Wheat",			0xF5DEB3FF},
	{"White",			0xFFFFFFFF},
	{"WhiteSmoke",		0xF5F5F5FF},
	{"Yellow",			0xFFFF00FF},
	{"YellowGreen",		0x9ACD32FF}
};

new projectName[MAX_PROJECT_NAME];
new DB:projectDB;
new bool:showTextDrawCmds;

new playerEditing[MAX_PLAYERS];
new playerEditingTimer[MAX_PLAYERS];
new PlayerText:playerEditingTextDraw[MAX_PLAYERS];
new playerCurrentGroup[MAX_PLAYERS];
new playerCurrentTextDraw[MAX_PLAYERS];

new groupData[MAX_GROUPS][E_GROUP];
new groupTextDrawData[MAX_GROUPS][MAX_GROUP_TEXTDRAWS][E_TEXTDRAW];
new groupsCount;

ReturnDate(timestamp) {
	new year, month, day, unused;
	TimestampToDate(timestamp, year, month, day, unused, unused, unused, 0);
	#pragma unused unused

	static const MONTH_NAMES[][] = {
		"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
	};

	new date[32];
	if (month < 1 || month > sizeof (MONTH_NAMES)) {
		format(date, sizeof (date), "%i/%i/%i", day, month, year);
	}
	else {
		format(date, sizeof (date), "%i %s, %i", day, MONTH_NAMES[(month - 1)], year);
	}
	return date;
}

ResetGroupData(groupid) {
	for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
        TextDrawDestroy(groupTextDrawData[groupid][i][E_TEXTDRAW_ID]);
        groupTextDrawData[groupid][i][E_TEXTDRAW_ID] = Text:INVALID_TEXT_DRAW;
	}

    groupData[groupid][E_GROUP_NAME][0] = EOS;
    groupData[groupid][E_GROUP_TEXTDRAWS_COUNT] = 0;
    groupData[groupid][E_GROUP_VISIBLE] = true;
}

CreateGroupTextDraw(groupid, textdrawid) {
	#define this(%0) \
	    groupTextDrawData[groupid][textdrawid][%0]

	if (this(E_TEXTDRAW_ID) != Text:INVALID_TEXT_DRAW) {
		TextDrawDestroy(this(E_TEXTDRAW_ID));
	}
	this(E_TEXTDRAW_ID) = TextDrawCreate(this(E_TEXTDRAW_X), this(E_TEXTDRAW_Y), this(E_TEXTDRAW_TEXT));

	TextDrawLetterSize(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_LETTERSIZE_X), this(E_TEXTDRAW_LETTERSIZE_Y));
	TextDrawTextSize(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_TEXTSIZE_X), this(E_TEXTDRAW_TEXTSIZE_Y));
	TextDrawAlignment(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_ALIGNMENT));
	TextDrawColor(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_COLOR));
	TextDrawUseBox(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_USE_BOX));
	TextDrawBoxColor(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_BOX_COLOR));
	TextDrawSetShadow(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_SHADOW));
	TextDrawSetOutline(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_OUTLINE));
	TextDrawBackgroundColor(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_BACKGROUND_COLOR));
	TextDrawFont(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_FONT));
	TextDrawSetProportional(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_PROPORTIONAL));
	TextDrawSetSelectable(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_SELECTABLE));
	TextDrawSetPreviewModel(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_PREVIEW_MODEL));
	TextDrawSetPreviewRot(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_PREVIEW_ROT_X), this(E_TEXTDRAW_PREVIEW_ROT_Y), this(E_TEXTDRAW_PREVIEW_ROT_Z), this(E_TEXTDRAW_PREVIEW_ROT_ZOOM));
	TextDrawSetPreviewVehCol(this(E_TEXTDRAW_ID), this(E_TEXTDRAW_PREVIEW_VEH_COLOR1), this(E_TEXTDRAW_PREVIEW_VEH_COLOR2));

	if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(this(E_TEXTDRAW_ID));
	}

	#undef this
}

RGBA(red, green, blue, alpha) {
	return ((red * 16777216) + (green * 65536) + (blue * 256) + alpha);
}

HexToInt(string[]) {
  	new cur = 1;
  	new res = 0;
	for (new i = strlen(string); i > 0; i--) {
    	if (string[i - 1] < 58) {
			res = res + cur * (string[i - 1] - 48);
		}
		else {
			res = res + cur * (string[i - 1] - 65 + 10);
		}

    	cur = cur * 16;
  	}
  	return res;
}

forward ShowPlayerGroupDialog(playerid, groupid);
public ShowPlayerGroupDialog(playerid, groupid) {
    static string[512 + (MAX_GROUP_TEXTDRAWS * (MAX_GROUP_TEXTDRAW_TEXT + 32))];
	string = ""COL_GREEN"Create New Textdraw\t"COL_GREY"Add a textdraw to group list (max textdraws you can add per group: "#MAX_GROUP_TEXTDRAWS")\n\
		"COL_YELLOW"Edit Group Position\t"COL_GREY"Modify all group textdraws position at the same time\n";

	if (groupData[groupid][E_GROUP_VISIBLE]) {
  		strcat(string, ""COL_YELLOW"Edit Group Visiblity\t"COL_GREY"Current: "COL_GREEN"ON\n");
	}
	else {
  		strcat(string, ""COL_YELLOW"Edit Group Visiblity\t"COL_GREY"Current: "COL_RED"OFF\n");
	}

	strcat(string, ""COL_YELLOW"Edit Group Name\t"COL_GREY"Current: '"COL_GREEN"");
	strcat(string, groupData[groupid][E_GROUP_NAME]);
	strcat(string, ""COL_GREY"'\n");

	strcat(string, ""COL_ORANGE"Duplicate Group\t"COL_GREY"Copy group properties to a new empty group\n\
		"COL_RED"Delete Group\t"COL_GREY"Delete the complete group with textdraws in it\n");

	new previewChars[MAX_GROUP_TEXTDRAW_PREVIEW_CHARS + 4];
	for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
		strmid(previewChars, groupTextDrawData[groupid][i][E_TEXTDRAW_TEXT], 0, MAX_GROUP_TEXTDRAW_PREVIEW_CHARS);
		if (strlen(groupTextDrawData[groupid][i][E_TEXTDRAW_TEXT]) > MAX_GROUP_TEXTDRAW_PREVIEW_CHARS) {
			strcat(previewChars, "...");
		}

		switch (groupTextDrawData[groupid][i][E_TEXTDRAW_FONT]) {
			case 0, 1, 2, 3: {
			    format(string, sizeof (string), "%sText #%i: '%s'\n", string, i, previewChars);
			}

			case 4: {
			    format(string, sizeof (string), "%sSprite #%i: '%s'\n", string, i, previewChars);
			}

			case 5: {
			    format(string, sizeof (string), "%sPreviewModel #%i: '%s'\n", string, i, previewChars);
			}
		}
	}

	playerCurrentGroup[playerid] = groupid;
	return Dialog_Show(playerid, GROUP_MENU, DIALOG_STYLE_TABLIST, "TDEditor: Group menu", string, "Select", "Back");
}

forward ShowPlayerTextDrawDialog(playerid, textdrawid);
public ShowPlayerTextDrawDialog(playerid, textdrawid) {
	new groupid = playerCurrentGroup[playerid];

	static string[1024];
	string = "Property\tValue\n";

    new previewChars[MAX_GROUP_TEXTDRAW_PREVIEW_CHARS + 4];
	strmid(previewChars, groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXT], 0, MAX_GROUP_TEXTDRAW_PREVIEW_CHARS);
	if (strlen(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXT]) > MAX_GROUP_TEXTDRAW_PREVIEW_CHARS) {
		strcat(previewChars, "...");
	}

	format(string, sizeof (string),
		"%s\
		Text & Position\t'%s'\n",
    string,
	previewChars,
	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_Y]);

    switch (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_FONT]) {
        case 0, 1, 2, 3: {
    		format(string, sizeof (string), "%sFont\t"COL_GREY"%i (Text)\n", string, groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_FONT]);
		}

		case 4: {
    		strcat(string, "Font\t"COL_GREY"4 (Sprite)\n");
		}

		case 5: {
    		strcat(string, "Font\t"COL_GREY"5 (PreviewModel)\n");
		}

		default: {
    		strcat(string, "Font\t"COL_GREY"Unknown\n");
		}
	}

	switch (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ALIGNMENT]) {
        case 1: {
    		strcat(string, "Alignment\t"COL_GREY"1 (Left)\n");
		}

		case 2: {
    		strcat(string, "Alignment\t"COL_GREY"2 (Center)\n");
		}

		case 3: {
    		strcat(string, "Alignment\t"COL_GREY"3 (Right)\n");
		}

		default: {
    		strcat(string, "Alignment\t"COL_GREY"Unknown\n");
		}
	}

	if (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PROPORTIONAL]) {
		strcat(string, "Proportional\t"COL_GREEN"ON\n");
	}
	else {
		strcat(string, "Proportional\t"COL_RED"OFF\n");
	}

	format(string, sizeof (string),
		"%s\
		Shadow\t"COL_GREY"Size: %i\n\
		Outline\t"COL_GREY"Size: %i\n\
		Outline/Shadow Color\t{%06x}Preview\n",
    string,
	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SHADOW],
	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_OUTLINE],
	(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BACKGROUND_COLOR] >>> 8));

	format(string, sizeof (string),
		"%s\
		Letter Size\t"COL_GREY"%0.4f, %0.4f\n\
		Letter/Textdraw Color\t{%06x}Preview\n",
    string,
	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y],
	(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_COLOR] >>> 8));

    if (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_USE_BOX]) {
		strcat(string, "Use Box\t"COL_GREEN"ON\n");
	}
	else {
		strcat(string, "Use Box\t"COL_RED"OFF\n");
	}

	format(string, sizeof (string),
		"%s\
		Box Size\t"COL_GREY"%0.4f, %0.4f\n\
		Box Color\t{%06x}Preview\n",
    string,
	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y],
	(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BOX_COLOR] >>> 8));

	if (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SELECTABLE]) {
		strcat(string, "Selectable\t"COL_GREEN"ON\n");
	}
	else {
		strcat(string, "Selectable\t"COL_RED"OFF\n");
	}

	strcat(string, "Preview Model Options\n");

	if (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TYPE_PLAYER]) {
		strcat(string, "TextDraw Type\t"COL_BLUE"PLAYER\n");
	}
	else {
		strcat(string, "TextDraw Type\t"COL_YELLOW"GLOBAL\n");
	}

	strcat(string,
		""COL_ORANGE"Duplicate TextDraw\n\
		"COL_RED"Delete TextDraw\n");

	playerCurrentTextDraw[playerid] = textdrawid;
	return Dialog_Show(playerid, TEXTDRAW_MENU, DIALOG_STYLE_TABLIST_HEADERS, "TDEditor: Textdraw menu", string, "Select", "Back");
}

ExportProject(const filename[]) {
	new File:h = fopen(filename, io_write);
	if (!h) {
	    return 0;
	}

	new string[1024];
	new globalTextdrawsCount;
	new playerTextdrawsCount;
	new bool:isThereAnyGlobalTextDraws;
	new bool:isThereAnyPlayerTextDraws;

	fwrite(h, "// TextDraw(s) developed using Gammix's TextDraw editor 1.0\r\n\r\n");
   	fwrite(h, "#include <a_samp>\r\n\r\n");

   	fwrite(h, "// Variable decleration on top of script\r\n");
    for (new i; i < groupsCount; i++) {
		format(string, sizeof (string), "// TextDraw Group: '%s'\r\n", groupData[i][E_GROUP_NAME]);
		fwrite(h, string);

	    globalTextdrawsCount = 0;
    	playerTextdrawsCount = 0;
	    for (new x; x < groupData[i][E_GROUP_TEXTDRAWS_COUNT]; x++) {
	        if (!groupTextDrawData[i][x][E_TEXTDRAW_TYPE_PLAYER]) {
   				globalTextdrawsCount++;

   				isThereAnyGlobalTextDraws = true;
			}
			else {
       			playerTextdrawsCount++;

   				isThereAnyPlayerTextDraws = true;
			}
	    }

	    if (globalTextdrawsCount == 1) {
	        format(string, sizeof (string), "new Text:%sTD;\r\n", groupData[i][E_GROUP_NAME]);
			fwrite(h, string);
		}
		else if (globalTextdrawsCount > 1) {
	        format(string, sizeof (string), "new Text:%sTD[%i];\r\n", groupData[i][E_GROUP_NAME], globalTextdrawsCount);
			fwrite(h, string);
		}

	    if (playerTextdrawsCount == 1) {
	        format(string, sizeof (string), "new PlayerText:%sPTD[MAX_PLAYERS];\r\n", groupData[i][E_GROUP_NAME]);
			fwrite(h, string);
		}
		else if (playerTextdrawsCount > 1) {
	        format(string, sizeof (string), "new PlayerText:%sPTD[MAX_PLAYERS][%i];\r\n", groupData[i][E_GROUP_NAME], playerTextdrawsCount);
			fwrite(h, string);
		}
	}

	fwrite(h, "\r\n");

	if (isThereAnyGlobalTextDraws) {
		fwrite(h, "// Creating global textdraw(s) under \"OnGameModeInit\" preferably\r\n");
		fwrite(h, "public OnGameModeInit()\r\n");
		fwrite(h, "{\r\n");
		
		for (new i; i < groupsCount; i++) {
		    globalTextdrawsCount = 0;
		    for (new x; x < groupData[i][E_GROUP_TEXTDRAWS_COUNT]; x++) {
		        if (!groupTextDrawData[i][x][E_TEXTDRAW_TYPE_PLAYER]) {
	   				globalTextdrawsCount++;
				}
		    }
		    
		    if (globalTextdrawsCount == 0) {
				continue;
			}

			format(string, sizeof (string), "\t// TextDraw Group: '%s'\r\n", groupData[i][E_GROUP_NAME]);
			fwrite(h, string);
			
		    if (globalTextdrawsCount == 1) {
		        format(string, sizeof (string), "\t%sTD = TextDrawCreate(%0.4f, %0.4f, \"%s\");\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_X], groupTextDrawData[i][0][E_TEXTDRAW_Y], groupTextDrawData[i][0][E_TEXTDRAW_TEXT]);
				fwrite(h, string);
				format(string, sizeof (string), "\tTextDrawFont(%sTD, %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_FONT]);
				fwrite(h, string);
		        format(string, sizeof (string), "\tTextDrawLetterSize(%sTD, %0.4f, %0.4f);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[i][0][E_TEXTDRAW_LETTERSIZE_Y]);
				fwrite(h, string);
				if (groupTextDrawData[i][0][E_TEXTDRAW_ALIGNMENT] != 1) {
			        format(string, sizeof (string), "\tTextDrawAlignment(%sTD, %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_ALIGNMENT]);
					fwrite(h, string);
				}
				format(string, sizeof (string), "\tTextDrawColor(%sTD, %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_COLOR]);
				fwrite(h, string);
				if (groupTextDrawData[i][0][E_TEXTDRAW_SHADOW] != 0) {
			        format(string, sizeof (string), "\tTextDrawSetShadow(%sTD, %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_SHADOW]);
					fwrite(h, string);
				}
				if (groupTextDrawData[i][0][E_TEXTDRAW_OUTLINE] != 0) {
			        format(string, sizeof (string), "\tTextDrawSetOutline(%sTD, %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_OUTLINE]);
					fwrite(h, string);
				}
				format(string, sizeof (string), "\tTextDrawBackgroundColor(%sTD, %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_BACKGROUND_COLOR]);
				fwrite(h, string);
				format(string, sizeof (string), "\tTextDrawSetProportional(%sTD, %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_PROPORTIONAL]);
				fwrite(h, string);
				format(string, sizeof (string), "\tTextDrawSetProportional(%sTD, %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_PROPORTIONAL]);
				fwrite(h, string);
				if (groupTextDrawData[i][0][E_TEXTDRAW_USE_BOX]) {
			        format(string, sizeof (string), "\tTextDrawUseBox(%sTD, 1);\r\n", groupData[i][E_GROUP_NAME]);
					fwrite(h, string);
			        format(string, sizeof (string), "\tTextDrawBoxColor(%sTD, %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_BOX_COLOR]);
					fwrite(h, string);
			        format(string, sizeof (string), "\tTextDrawTextSize(%sTD, %0.4f, %0.4f);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[i][0][E_TEXTDRAW_TEXTSIZE_Y]);
					fwrite(h, string);
				}
				if (groupTextDrawData[i][0][E_TEXTDRAW_FONT] == 5) {
			        format(string, sizeof (string), "\tTextDrawSetPreviewModel(%sTD, %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_PREVIEW_MODEL]);
					fwrite(h, string);
			        format(string, sizeof (string), "\tTextDrawSetPreviewRot(%sTD, %0.4f, %0.4f, %0.4f, %0.4f);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[i][0][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[i][0][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[i][0][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
					fwrite(h, string);
				}
				if (groupTextDrawData[i][0][E_TEXTDRAW_SELECTABLE]) {
			        format(string, sizeof (string), "\tTextDrawSetSelectable(%sTD, 1);\r\n", groupData[i][E_GROUP_NAME]);
					fwrite(h, string);
				}
				
				fwrite(h, "\r\n");
			}
			else if (globalTextdrawsCount > 1) {
			    for (new x; x < globalTextdrawsCount; x++) {
			        format(string, sizeof (string), "\t%sTD[%i] = TextDrawCreate(%0.4f, %0.4f, \"%s\");\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_X], groupTextDrawData[i][x][E_TEXTDRAW_Y], groupTextDrawData[i][x][E_TEXTDRAW_TEXT]);
					fwrite(h, string);
					format(string, sizeof (string), "\tTextDrawFont(%sTD[%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_FONT]);
					fwrite(h, string);
			        format(string, sizeof (string), "\tTextDrawLetterSize(%sTD[%i], %0.4f, %0.4f);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[i][x][E_TEXTDRAW_LETTERSIZE_Y]);
					fwrite(h, string);
					if (groupTextDrawData[i][0][E_TEXTDRAW_ALIGNMENT] != 1) {
				        format(string, sizeof (string), "\tTextDrawAlignment(%sTD[%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_ALIGNMENT]);
						fwrite(h, string);
					}
					format(string, sizeof (string), "\tTextDrawColor(%sTD[%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_COLOR]);
					fwrite(h, string);
					if (groupTextDrawData[i][0][E_TEXTDRAW_SHADOW] != 0) {
				        format(string, sizeof (string), "\tTextDrawSetShadow(%sTD[%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_SHADOW]);
						fwrite(h, string);
					}
					if (groupTextDrawData[i][0][E_TEXTDRAW_OUTLINE] != 0) {
				        format(string, sizeof (string), "\tTextDrawSetOutline(%sTD[%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_OUTLINE]);
						fwrite(h, string);
					}
					format(string, sizeof (string), "\tTextDrawBackgroundColor(%sTD[%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_BACKGROUND_COLOR]);
					fwrite(h, string);
					format(string, sizeof (string), "\tTextDrawSetProportional(%sTD[%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_PROPORTIONAL]);
					fwrite(h, string);
					format(string, sizeof (string), "\tTextDrawSetProportional(%sTD[%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_PROPORTIONAL]);
					fwrite(h, string);
					if (groupTextDrawData[i][0][E_TEXTDRAW_USE_BOX]) {
				        format(string, sizeof (string), "\tTextDrawUseBox(%sTD[%i], 1);\r\n", groupData[i][E_GROUP_NAME], x);
						fwrite(h, string);
				        format(string, sizeof (string), "\tTextDrawBoxColor(%sTD[%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_BOX_COLOR]);
						fwrite(h, string);
				        format(string, sizeof (string), "\tTextDrawTextSize(%sTD[%i], %0.4f, %0.4f);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[i][x][E_TEXTDRAW_TEXTSIZE_Y]);
						fwrite(h, string);
					}
					if (groupTextDrawData[i][0][E_TEXTDRAW_FONT] == 5) {
				        format(string, sizeof (string), "\tTextDrawSetPreviewModel(%sTD[%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_MODEL]);
						fwrite(h, string);
				        format(string, sizeof (string), "\tTextDrawSetPreviewRot(%sTD[%i], %0.4f, %0.4f, %0.4f, %0.4f);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
						fwrite(h, string);
					}
					if (groupTextDrawData[i][0][E_TEXTDRAW_SELECTABLE]) {
				        format(string, sizeof (string), "\tTextDrawSetSelectable(%sTD[%i], 1);\r\n", groupData[i][E_GROUP_NAME], x);
						fwrite(h, string);
					}
					
					fwrite(h, "\r\n");
			    }
			}
		}
		
		fwrite(h, "\treturn 1;\r\n");
		fwrite(h, "}\r\n\r\n");
	}
	
	if (isThereAnyPlayerTextDraws) {
		fwrite(h, "// Creating player textdraw(s) under \"OnPlayerConnect\" preferably\r\n");
		fwrite(h, "public OnPlayerConnect(playerid)\r\n");
		fwrite(h, "{\r\n");
		
		for (new i; i < groupsCount; i++) {
		    playerTextdrawsCount = 0;
		    for (new x; x < groupData[i][E_GROUP_TEXTDRAWS_COUNT]; x++) {
		        if (groupTextDrawData[i][x][E_TEXTDRAW_TYPE_PLAYER]) {
	       			playerTextdrawsCount++;
				}
		    }
		    
		    if (playerTextdrawsCount == 0) {
		        continue;
			}

			format(string, sizeof (string), "\t// TextDraw Group: '%s'\r\n", groupData[i][E_GROUP_NAME]);
			fwrite(h, string);

		    if (playerTextdrawsCount == 1) {
		        format(string, sizeof (string), "\t%sPTD[playerid] = CreatePlayerTextDraw(playerid, %0.4f, %0.4f, \"%s\");\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_X], groupTextDrawData[i][0][E_TEXTDRAW_Y], groupTextDrawData[i][0][E_TEXTDRAW_TEXT]);
				fwrite(h, string);
				format(string, sizeof (string), "\tPlayerTextDrawFont(playerid, %sPTD[playerid], %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_FONT]);
				fwrite(h, string);
		        format(string, sizeof (string), "\tPlayerTextDrawLetterSize(playerid, %sPTD[playerid], %0.4f, %0.4f);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[i][0][E_TEXTDRAW_LETTERSIZE_Y]);
				fwrite(h, string);
				if (groupTextDrawData[i][0][E_TEXTDRAW_ALIGNMENT] != 1) {
			        format(string, sizeof (string), "\tPlayerTextDrawAlignment(playerid, %sPTD[playerid], %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_ALIGNMENT]);
					fwrite(h, string);
				}
				format(string, sizeof (string), "\tPlayerTextDrawColor(playerid, %sPTD[playerid], %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_COLOR]);
				fwrite(h, string);
				if (groupTextDrawData[i][0][E_TEXTDRAW_SHADOW] != 0) {
			        format(string, sizeof (string), "\tPlayerTextDrawSetShadow(playerid, %sPTD[playerid], %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_SHADOW]);
					fwrite(h, string);
				}
				if (groupTextDrawData[i][0][E_TEXTDRAW_OUTLINE] != 0) {
			        format(string, sizeof (string), "\tPlayerTextDrawSetOutline(playerid, %sPTD[playerid], %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_OUTLINE]);
					fwrite(h, string);
				}
				format(string, sizeof (string), "\tPlayerTextDrawBackgroundColor(playerid, %sPTD[playerid], %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_BACKGROUND_COLOR]);
				fwrite(h, string);
				format(string, sizeof (string), "\tPlayerTextDrawSetProportional(playerid, %sPTD[playerid], %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_PROPORTIONAL]);
				fwrite(h, string);
				format(string, sizeof (string), "\tPlayerTextDrawSetProportional(playerid, %sPTD[playerid], %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_PROPORTIONAL]);
				fwrite(h, string);
				if (groupTextDrawData[i][0][E_TEXTDRAW_USE_BOX]) {
			        format(string, sizeof (string), "\tPlayerTextDrawUseBox(playerid, %sPTD[playerid], 1);\r\n", groupData[i][E_GROUP_NAME]);
					fwrite(h, string);
			        format(string, sizeof (string), "\tPlayerTextDrawBoxColor(playerid, %sPTD[playerid], %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_BOX_COLOR]);
					fwrite(h, string);
			        format(string, sizeof (string), "\tPlayerTextDrawTextSize(playerid, %sPTD[playerid], %0.4f, %0.4f);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[i][0][E_TEXTDRAW_TEXTSIZE_Y]);
					fwrite(h, string);
				}
				if (groupTextDrawData[i][0][E_TEXTDRAW_FONT] == 5) {
			        format(string, sizeof (string), "\tPlayerTextDrawSetPreviewModel(playerid, %sPTD[playerid], %i);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_PREVIEW_MODEL]);
					fwrite(h, string);
			        format(string, sizeof (string), "\tPlayerTextDrawSetPreviewRot(playerid, %sPTD[playerid], %0.4f, %0.4f, %0.4f, %0.4f);\r\n", groupData[i][E_GROUP_NAME], groupTextDrawData[i][0][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[i][0][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[i][0][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[i][0][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
					fwrite(h, string);
				}
				if (groupTextDrawData[i][0][E_TEXTDRAW_SELECTABLE]) {
			        format(string, sizeof (string), "\tPlayerTextDrawSetSelectable(playerid, %sPTD[playerid], 1);\r\n", groupData[i][E_GROUP_NAME]);
					fwrite(h, string);
				}
				
				fwrite(h, "\r\n");
			}
			else if (playerTextdrawsCount > 1) {
			    for (new x; x < playerTextdrawsCount; x++) {
			        format(string, sizeof (string), "\t%sPTD[playerid][%i] = CreatePlayerTextDraw(playerid, %0.4f, %0.4f, \"%s\");\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_X], groupTextDrawData[i][x][E_TEXTDRAW_Y], groupTextDrawData[i][x][E_TEXTDRAW_TEXT]);
					fwrite(h, string);
					format(string, sizeof (string), "\tPlayerTextDrawFont(playerid, %sPTD[playerid][%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][0][E_TEXTDRAW_FONT]);
					fwrite(h, string);
			        format(string, sizeof (string), "\tPlayerTextDrawLetterSize(playerid, %sPTD[playerid][%i], %0.4f, %0.4f);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[i][x][E_TEXTDRAW_LETTERSIZE_Y]);
					fwrite(h, string);
					if (groupTextDrawData[i][0][E_TEXTDRAW_ALIGNMENT] != 1) {
				        format(string, sizeof (string), "\tPlayerTextDrawAlignment(playerid, %sPTD[playerid][%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_ALIGNMENT]);
						fwrite(h, string);
					}
					format(string, sizeof (string), "\tPlayerTextDrawColor(playerid, %sPTD[playerid][%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_COLOR]);
					fwrite(h, string);
					if (groupTextDrawData[i][0][E_TEXTDRAW_SHADOW] != 0) {
				        format(string, sizeof (string), "\tPlayerTextDrawSetShadow(playerid, %sPTD[playerid][%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_SHADOW]);
						fwrite(h, string);
					}
					if (groupTextDrawData[i][0][E_TEXTDRAW_OUTLINE] != 0) {
				        format(string, sizeof (string), "\tPlayerTextDrawSetOutline(playerid, %sPTD[playerid][%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_OUTLINE]);
						fwrite(h, string);
					}
					format(string, sizeof (string), "\tPlayerTextDrawBackgroundColor(playerid, %sPTD[playerid][%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_BACKGROUND_COLOR]);
					fwrite(h, string);
					format(string, sizeof (string), "\tPlayerTextDrawSetProportional(playerid, %sPTD[playerid][%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_PROPORTIONAL]);
					fwrite(h, string);
					format(string, sizeof (string), "\tPlayerTextDrawSetProportional(playerid, %sPTD[playerid][%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_PROPORTIONAL]);
					fwrite(h, string);
					if (groupTextDrawData[i][0][E_TEXTDRAW_USE_BOX]) {
				        format(string, sizeof (string), "\tPlayerTextDrawUseBox(playerid, %sPTD[playerid][%i], 1);\r\n", groupData[i][E_GROUP_NAME], x);
						fwrite(h, string);
				        format(string, sizeof (string), "\tPlayerTextDrawBoxColor(playerid, %sPTD[playerid][%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_BOX_COLOR]);
						fwrite(h, string);
				        format(string, sizeof (string), "\tPlayerTextDrawTextSize(playerid, %sPTD[playerid][%i], %0.4f, %0.4f);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[i][x][E_TEXTDRAW_TEXTSIZE_Y]);
						fwrite(h, string);
					}
					if (groupTextDrawData[i][0][E_TEXTDRAW_FONT] == 5) {
				        format(string, sizeof (string), "\tPlayerTextDrawSetPreviewModel(playerid, %sPTD[playerid][%i], %i);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_MODEL]);
						fwrite(h, string);
				        format(string, sizeof (string), "\tPlayerTextDrawSetPreviewRot(playerid, %sPTD[playerid][%i], %0.4f, %0.4f, %0.4f, %0.4f);\r\n", groupData[i][E_GROUP_NAME], x, groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
						fwrite(h, string);
					}
					if (groupTextDrawData[i][0][E_TEXTDRAW_SELECTABLE]) {
				        format(string, sizeof (string), "\tPlayerTextDrawSetSelectable(playerid, %sPTD[playerid][%i], 1);\r\n", groupData[i][E_GROUP_NAME], x);
						fwrite(h, string);
					}
					
					fwrite(h, "\r\n");
			    }
			}
		}

		fwrite(h, "\treturn 1;\r\n");
		fwrite(h, "}\r\n\r\n");
	}

	fwrite(h, "// End of export!");
	fclose(h);
	return 1;
}

public OnFilterScriptInit() {
	printf("\n==========================================\n");
	
	if (!fexist(PATH_PROJECT_FILES)) {
		printf("[TDEditor.pwn] - Error: Path \"%s\" doesn't exists. Your projects won't be saved/loaded.", PATH_PROJECT_FILES);
	}
	
	if (!fexist(PATH_EXPORT_FILES)) {
		printf("[TDEditor.pwn] - Error: Path \"%s\" doesn't exists. Your projects won't be exported.", PATH_EXPORT_FILES);
	}

	if (!fexist(PATH_RECORD_FILE)) {
		new File:h = fopen(PATH_RECORD_FILE, io_write);
		if (!h) {
			printf("[TDEditor.pwn] - Error: Path \"%s\" doesn't exists. Your projects list won't save/load projects names as history.", PATH_RECORD_FILE);
		}
		else {
			fclose(h);
		}
	}

	if (!fexist(PATH_OBJECTS_FILE)) {
		printf("[TDEditor.pwn] - Warning: Path \"%s\" doesn't exists. You cannot search object models in Preview Model Option; only modelids accepted.", PATH_OBJECTS_FILE);
	}

	printf("[TDEditor.pwn] - Loaded v1.2 - By Gammix");
	
	printf("\n==========================================\n");

    projectDB = DB:0;
    showTextDrawCmds = true;

	for (new i; i < MAX_GROUPS; i++) {
	    for (new x; x < MAX_GROUP_TEXTDRAWS; x++) {
	        groupTextDrawData[i][x][E_TEXTDRAW_ID] = Text:INVALID_TEXT_DRAW;
		}

		ResetGroupData(i);
	}
	groupsCount = 0;

	for (new i; i < MAX_PLAYERS; i++) {
        playerEditing[i] = EDITING_NONE;
        playerEditingTimer[i] = -1;
	}
	return 1;
}

public OnFilterScriptExit() {
	if (projectDB) {
	    new string[1024];
	    for (new i; i < groupsCount; i++) {
		    for (new x; x < groupData[i][E_GROUP_TEXTDRAWS_COUNT]; x++) {
			    string = "UPDATE %s SET \
						text = '%q', \
						x = '%f', y = '%f', \
						letter_x = '%f', letter_y = '%f', \
						text_x = '%f', text_y = '%f', \
						alignment = '%i', \
						color = '%i', \
						usebox = '%i', \
						box_color = '%i', \
						shadow = '%i', \
						outline = '%i', \
						background_color = '%i', \
						font = '%i', \
						proportional = '%i', \
						selectable = '%i', \
						preview_model = '%i', \
						rot_x = '%f', rot_y = '%f', rot_z = '%f', rot_zoom = '%f', \
						veh_color1 = '%i', veh_color2 = '%i', \
						type_player = '%i' \
					WHERE id = '%i'";
				format(string, sizeof (string), string,
				    groupData[i][E_GROUP_NAME],
				    groupTextDrawData[i][x][E_TEXTDRAW_TEXT],
				    groupTextDrawData[i][x][E_TEXTDRAW_X], groupTextDrawData[i][x][E_TEXTDRAW_Y],
				    groupTextDrawData[i][x][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[i][x][E_TEXTDRAW_LETTERSIZE_Y],
				    groupTextDrawData[i][x][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[i][x][E_TEXTDRAW_TEXTSIZE_Y],
				    groupTextDrawData[i][x][E_TEXTDRAW_ALIGNMENT],
				    groupTextDrawData[i][x][E_TEXTDRAW_COLOR],
				    groupTextDrawData[i][x][E_TEXTDRAW_USE_BOX],
					groupTextDrawData[i][x][E_TEXTDRAW_BOX_COLOR],
					groupTextDrawData[i][x][E_TEXTDRAW_SHADOW],
					groupTextDrawData[i][x][E_TEXTDRAW_OUTLINE],
					groupTextDrawData[i][x][E_TEXTDRAW_BACKGROUND_COLOR],
					groupTextDrawData[i][x][E_TEXTDRAW_FONT],
					groupTextDrawData[i][x][E_TEXTDRAW_PROPORTIONAL],
					groupTextDrawData[i][x][E_TEXTDRAW_SELECTABLE],
					groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_MODEL],
					groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_ROT_ZOOM],
					groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_VEH_COLOR1], groupTextDrawData[i][x][E_TEXTDRAW_PREVIEW_VEH_COLOR2],
					groupTextDrawData[i][x][E_TEXTDRAW_TYPE_PLAYER],
					groupTextDrawData[i][x][E_TEXTDRAW_SQLID]);
				db_query(projectDB, string);
		    }
		}
		db_close(projectDB);
		projectDB = DB:0;
	}

    for (new i; i < groupsCount; i++) {
		ResetGroupData(i);
	}
	groupsCount = 0;
	
	for (new i; i < MAX_PLAYERS; i++) {
    	playerEditing[i] = EDITING_NONE;
    	playerEditingTimer[i] = -1;
	}
	return 1;
}

public OnPlayerConnect(playerid) {
    playerEditingTextDraw[playerid] = CreatePlayerTextDraw(playerid, 7.0, 170.0, "-");
	PlayerTextDrawBackgroundColor(playerid, playerEditingTextDraw[playerid], 255);
	PlayerTextDrawFont(playerid, playerEditingTextDraw[playerid], 1);
	PlayerTextDrawLetterSize(playerid, playerEditingTextDraw[playerid], 0.2, 1.0);
	PlayerTextDrawColor(playerid, playerEditingTextDraw[playerid], -1);
	PlayerTextDrawSetOutline(playerid, playerEditingTextDraw[playerid], 1);
	PlayerTextDrawSetProportional(playerid, playerEditingTextDraw[playerid], 1);
	PlayerTextDrawSetShadow(playerid, playerEditingTextDraw[playerid], 0);
	return 1;
}

public OnPlayerDisconnect(playerid, reason) {
    playerEditing[playerid] = EDITING_NONE;
    KillTimer(playerEditingTimer[playerid]);
    playerEditingTimer[playerid] = -1;
    
    if (projectDB) {
	    new string[1024];
	    new groupid = playerCurrentGroup[playerid];
	    
	    for (new x; x < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; x++) {
	    	string = "UPDATE %s SET \
				text = '%q', \
				x = '%f', y = '%f', \
				letter_x = '%f', letter_y = '%f', \
				text_x = '%f', text_y = '%f', \
				alignment = '%i', \
				color = '%i', \
				usebox = '%i', \
				box_color = '%i', \
				shadow = '%i', \
				outline = '%i', \
				background_color = '%i', \
				font = '%i', \
				proportional = '%i', \
				selectable = '%i', \
				preview_model = '%i', \
				rot_x = '%f', rot_y = '%f', rot_z = '%f', rot_zoom = '%f', \
				veh_color1 = '%i', veh_color2 = '%i', \
				type_player = '%i' \
			WHERE id = '%i'";
			format(string, sizeof (string), string,
   				groupData[groupid][E_GROUP_NAME],
			    groupTextDrawData[groupid][x][E_TEXTDRAW_TEXT],
			    groupTextDrawData[groupid][x][E_TEXTDRAW_X], groupTextDrawData[groupid][x][E_TEXTDRAW_Y],
			    groupTextDrawData[groupid][x][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[groupid][x][E_TEXTDRAW_LETTERSIZE_Y],
			    groupTextDrawData[groupid][x][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[groupid][x][E_TEXTDRAW_TEXTSIZE_Y],
			    groupTextDrawData[groupid][x][E_TEXTDRAW_ALIGNMENT],
			    groupTextDrawData[groupid][x][E_TEXTDRAW_COLOR],
			    groupTextDrawData[groupid][x][E_TEXTDRAW_USE_BOX],
				groupTextDrawData[groupid][x][E_TEXTDRAW_BOX_COLOR],
				groupTextDrawData[groupid][x][E_TEXTDRAW_SHADOW],
				groupTextDrawData[groupid][x][E_TEXTDRAW_OUTLINE],
				groupTextDrawData[groupid][x][E_TEXTDRAW_BACKGROUND_COLOR],
				groupTextDrawData[groupid][x][E_TEXTDRAW_FONT],
				groupTextDrawData[groupid][x][E_TEXTDRAW_PROPORTIONAL],
				groupTextDrawData[groupid][x][E_TEXTDRAW_SELECTABLE],
				groupTextDrawData[groupid][x][E_TEXTDRAW_PREVIEW_MODEL],
				groupTextDrawData[groupid][x][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][x][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][x][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][x][E_TEXTDRAW_PREVIEW_ROT_ZOOM],
				groupTextDrawData[groupid][x][E_TEXTDRAW_PREVIEW_VEH_COLOR1], groupTextDrawData[groupid][x][E_TEXTDRAW_PREVIEW_VEH_COLOR2],
				groupTextDrawData[groupid][x][E_TEXTDRAW_TYPE_PLAYER],
				groupTextDrawData[groupid][x][E_TEXTDRAW_SQLID]);
			db_query(projectDB, string);
		}
	}
	return 1;
}

public OnPlayerSpawn(playerid) {
	SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: Use \"/text\" to open TextDraw Edtior menu.");
	return 1;
}

forward OnPlayerTimerUpdate(playerid);
public OnPlayerTimerUpdate(playerid) {
    if (playerEditing[playerid] != EDITING_NONE) {
		new groupid = playerCurrentGroup[playerid];
		new textdrawid = playerCurrentTextDraw[playerid];
	
        new keys, updown, leftright;
		GetPlayerKeys(playerid, keys, updown, leftright);

		if (updown < 0) {
			switch (playerEditing[playerid]) {
				case EDITING_GROUP_POS: {
				    if (keys == KEY_SPRINT) {
				        for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
				  	  		groupTextDrawData[groupid][i][E_TEXTDRAW_Y] -= 10.0;
							CreateGroupTextDraw(groupid, i);
						}
					}
					else {
				        for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
				  	  		groupTextDrawData[groupid][i][E_TEXTDRAW_Y] -= 0.5;
							CreateGroupTextDraw(groupid, i);
						}
					}
				}
				
				case EDITING_POS: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_Y] -= 10.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_Y] -= 0.5;
					}
					CreateGroupTextDraw(groupid, textdrawid);
				}

				case EDITING_LETTER_SIZE: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y] -= 1.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y] -= 0.1;
					}

					TextDrawLetterSize(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}

				case EDITING_TEXT_SIZE: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y] -= 10.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y] -= 0.5;
					}

					TextDrawTextSize(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}

				case EDITING_SHADOW_SIZE: {
				    groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SHADOW] += 1;

					TextDrawSetShadow(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SHADOW]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}

				case EDITING_OUTLINE_SIZE: {
				    groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_OUTLINE] += 1;
	
					TextDrawSetOutline(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_OUTLINE]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}

				case EDITING_PREVIEW_ROT: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y] += 5.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y] += 1.0;
					}

					TextDrawSetPreviewRot(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}
			}
		}
		else if (updown > 0) {
			switch (playerEditing[playerid]) {
				case EDITING_GROUP_POS: {
				    if (keys == KEY_SPRINT) {
				        for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
				  	  		groupTextDrawData[groupid][i][E_TEXTDRAW_Y] += 10.0;
							CreateGroupTextDraw(groupid, i);
						}
					}
					else {
				        for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
				  	  		groupTextDrawData[groupid][i][E_TEXTDRAW_Y] += 0.5;
							CreateGroupTextDraw(groupid, i);
						}
					}
				}
				
				case EDITING_POS: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_Y] += 10.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_Y] += 0.5;
					}
					CreateGroupTextDraw(groupid, textdrawid);
				}

				case EDITING_LETTER_SIZE: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y] += 1.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y] += 0.1;
					}

					TextDrawLetterSize(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}

				case EDITING_TEXT_SIZE: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y] += 10.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y] += 0.5;
					}

					TextDrawTextSize(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}

				case EDITING_SHADOW_SIZE: {
				   	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SHADOW] -= 1;

					TextDrawSetShadow(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SHADOW]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}

				case EDITING_OUTLINE_SIZE: {
				    groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_OUTLINE] -= 1;

					TextDrawSetOutline(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_OUTLINE]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}

				case EDITING_PREVIEW_ROT: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y] -= 5.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y] -= 1.0;
					}

					TextDrawSetPreviewRot(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}
			}
		}

		if (leftright < 0) {
			switch (playerEditing[playerid]) {
				case EDITING_GROUP_POS: {
				    if (keys == KEY_SPRINT) {
				        for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
				  	  		groupTextDrawData[groupid][i][E_TEXTDRAW_X] -= 10.0;
							CreateGroupTextDraw(groupid, i);
						}
					}
					else {
				        for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
				  	  		groupTextDrawData[groupid][i][E_TEXTDRAW_X] -= 0.5;
							CreateGroupTextDraw(groupid, i);
						}
					}
				}
				
				case EDITING_POS: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_X] -= 10.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_X] -= 0.5;
					}
					CreateGroupTextDraw(groupid, textdrawid);
				}

				case EDITING_LETTER_SIZE: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X] -= 1.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X] -= 0.1;
					}

					TextDrawLetterSize(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}

				case EDITING_TEXT_SIZE: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X] -= 10.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X] -= 0.5;
					}

					TextDrawTextSize(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}

				case EDITING_PREVIEW_ROT: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X] -= 5.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X] -= 1.0;
					}

					TextDrawSetPreviewRot(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}
			}
		}
		else if (leftright > 0) {
			switch (playerEditing[playerid]) {
				case EDITING_GROUP_POS: {
				    if (keys == KEY_SPRINT) {
				        for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
				  	  		groupTextDrawData[groupid][i][E_TEXTDRAW_X] += 10.0;
							CreateGroupTextDraw(groupid, i);
						}
					}
					else {
				        for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
				  	  		groupTextDrawData[groupid][i][E_TEXTDRAW_X] += 0.5;
							CreateGroupTextDraw(groupid, i);
						}
					}
				}

				case EDITING_POS: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_X] += 10.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_X] += 0.5;
					}
					CreateGroupTextDraw(groupid, textdrawid);
				}

				case EDITING_LETTER_SIZE: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X] += 1.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X] += 0.1;
					}

					TextDrawLetterSize(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}

				case EDITING_TEXT_SIZE: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X] += 10.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X] += 0.5;
					}

					TextDrawTextSize(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}

				case EDITING_PREVIEW_ROT: {
				    if (keys == KEY_SPRINT) {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X] += 5.0;
					}
					else {
				  	  	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X] += 1.0;
					}

					TextDrawSetPreviewRot(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
                    if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}
				}
			}
		}
		
		if (showTextDrawCmds) {
			if (playerEditing[playerid] == EDITING_GROUP_POS) {
				PlayerTextDrawSetString(playerid, playerEditingTextDraw[playerid],
					"~w~EDITOR MODE COMMANDS: (/show)~n~\
					~w~/position~n~\
					~y~/x: ~b~~h~~h~?~n~\
					~y~/y: ~b~~h~~h~?");
				return 1;
			}

			new previewChars[MAX_GROUP_TEXTDRAW_PREVIEW_CHARS + 4];
			strmid(previewChars, groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXT], 0, MAX_GROUP_TEXTDRAW_PREVIEW_CHARS);
			if (strlen(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXT]) > MAX_GROUP_TEXTDRAW_PREVIEW_CHARS) {
				strcat(previewChars, "...");
			}

	  		new string[1024];
			switch (playerEditing[playerid]) {
			    case EDITING_POS, EDITING_GROUP_POS: {
					string = "~w~EDITOR MODE COMMANDS: (/show)~n~\
						~w~/string: \"%s\"~n~\
						~r~~h~~h~/position~n~\
						~r~~h~~h~/x: ~h~%0.4f~n~\
						~r~~h~~h~/y: ~h~%0.4f~n~\
						~w~/lettersize~n~\
						~w~/lx: %0.4f~n~\
						~w~/ly: %0.4f~n~\
						~w~/textsize~n~\
						~w~/tx: %0.4f~n~\
						~w~/ty: %0.4f~n~\
						~w~/rotation~n~\
						~w~/rx: %0.4f~n~\
						~w~/ry: %0.4f~n~\
						~w~/rz: %0.4f~n~\
						~w~/zoom: %0.4f~n~\
						~w~/shadow: %i~n~\
						~w~/outline: %i";
			    }
			    
			    case EDITING_LETTER_SIZE: {
					string = "~w~EDITOR MODE COMMANDS: (/show)~n~\
						~w~/string: \"%s\"~n~\
						~w~/position~n~\
						~w~/x: %0.4f~n~\
						~w~/y: %0.4f~n~\
						~r~~h~~h~/lettersize~n~\
						~r~~h~~h~/lx: ~h~%0.4f~n~\
						~r~~h~~h~/ly: ~h~%0.4f~n~\
						~w~/textsize~n~\
						~w~/tx: %0.4f~n~\
						~w~/ty: %0.4f~n~\
						~w~/rotation~n~\
						~w~/rx: %0.4f~n~\
						~w~/ry: %0.4f~n~\
						~w~/rz: %0.4f~n~\
						~w~/zoom: %0.4f~n~\
						~w~/shadow: %i~n~\
						~w~/outline: %i";
			    }
			    
			    case EDITING_TEXT_SIZE: {
					string = "~w~EDITOR MODE COMMANDS: (/show)~n~\
						~w~/string: \"%s\"~n~\
						~w~/position~n~\
						~w~/x: %0.4f~n~\
						~w~/y: %0.4f~n~\
						~w~/lettersize~n~\
						~w~/lx: %0.4f~n~\
						~w~/ly: %0.4f~n~\
						~r~~h~~h~/textsize~n~\
						~r~~h~~h~/tx: ~h~%0.4f~n~\
						~r~~h~~h~/ty: ~h~%0.4f~n~\
						~w~/rotation~n~\
						~w~/rx: %0.4f~n~\
						~w~/ry: %0.4f~n~\
						~w~/rz: %0.4f~n~\
						~w~/zoom: %0.4f~n~\
						~w~/shadow: %i~n~\
						~w~/outline: %i";
			    }
			    
			    case EDITING_PREVIEW_ROT: {
					string = "~w~EDITOR MODE COMMANDS: (/show)~n~\
						~w~/string: \"%s\"~n~\
						~w~/position~n~\
						~w~/x: %0.4f~n~\
						~w~/y: %0.4f~n~\
						~w~/lettersize~n~\
						~w~/lx: %0.4f~n~\
						~w~/ly: %0.4f~n~\
						~w~/textsize~n~\
						~w~/tx: %0.4f~n~\
						~w~/ty: %0.4f~n~\
						~r~~h~~h~/rotation~n~\
						~r~~h~~h~/rx: ~h~%0.4f~n~\
						~r~~h~~h~/ry: ~h~%0.4f~n~\
						~r~~h~~h~/rz: ~h~%0.4f~n~\
						~r~~h~~h~/zoom: ~h~%0.4f~n~\
						~w~/shadow: %i~n~\
						~w~/outline: %i";
			    }
			    
			    case EDITING_SHADOW_SIZE: {
					string = "~w~EDITOR MODE COMMANDS: (/show)~n~\
						~w~/string: \"%s\"~n~\
						~w~/position~n~\
						~w~/x: %0.4f~n~\
						~w~/y: %0.4f~n~\
						~w~/lettersize~n~\
						~w~/lx: %0.4f~n~\
						~w~/ly: %0.4f~n~\
						~w~/textsize~n~\
						~w~/tx: %0.4f~n~\
						~w~/ty: %0.4f~n~\
						~w~/rotation~n~\
						~w~/rx: %0.4f~n~\
						~w~/ry: %0.4f~n~\
						~w~/rz: %0.4f~n~\
						~w~/zoom: %0.4f~n~\
						~r~~h~~h~/shadow: ~h~%i~n~\
						~w~/outline: %i";
			    }
			    
			    case EDITING_OUTLINE_SIZE: {
					string = "~w~EDITOR MODE COMMANDS: (/show)~n~\
						~w~/string: \"%s\"~n~\
						~w~/position~n~\
						~w~/x: %0.4f~n~\
						~w~/y: %0.4f~n~\
						~w~/lettersize~n~\
						~w~/lx: %0.4f~n~\
						~w~/ly: %0.4f~n~\
						~w~/textsize~n~\
						~w~/tx: %0.4f~n~\
						~w~/ty: %0.4f~n~\
						~w~/rotation~n~\
						~w~/rx: %0.4f~n~\
						~w~/ry: %0.4f~n~\
						~w~/rz: %0.4f~n~\
						~w~/zoom: %0.4f~n~\
						~w~/shadow: %i~n~\
						~r~~h~~h~/outline: ~h~%i";
			    }
			}
			
			format(string, sizeof (string), string,
	        previewChars,
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_Y],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SHADOW],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_OUTLINE]);

			PlayerTextDrawSetString(playerid, playerEditingTextDraw[playerid], string);
		}
	}
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys) {
    if (playerEditing[playerid] != EDITING_NONE) {
		if (newkeys == KEY_SECONDARY_ATTACK) {
		    PlayerTextDrawHide(playerid, playerEditingTextDraw[playerid]);
			
		    TogglePlayerControllable(playerid, true);

            if (playerEditing[playerid] == EDITING_GROUP_POS) {
				SetTimerEx("ShowPlayerGroupDialog", 150, false, "ii", playerid, playerCurrentGroup[playerid]);
			}
			else {
				SetTimerEx("ShowPlayerTextDrawDialog", 150, false, "ii", playerid, playerCurrentTextDraw[playerid]);
			}

			playerEditing[playerid] = EDITING_NONE;
			KillTimer(playerEditingTimer[playerid]);
			playerEditingTimer[playerid] = -1;
		}
	}
	return 1;
}

CMD:x(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

    new Float:value;
	if (sscanf(params, "%f", value)) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "Usage: /x [value]");
	}

    new groupid = playerCurrentGroup[playerid];
	if (playerEditing[playerid] == EDITING_GROUP_POS) {
        for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
			groupTextDrawData[groupid][i][E_TEXTDRAW_X] = value;
			CreateGroupTextDraw(groupid, i);
		}
		return 1;
	}

	new textdrawid = playerCurrentTextDraw[playerid];
	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_X] = value;
	CreateGroupTextDraw(groupid, textdrawid);
	return 1;
}

CMD:y(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

    new Float:value;
	if (sscanf(params, "%f", value)) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "Usage: /y [value]");
	}

    new groupid = playerCurrentGroup[playerid];
	if (playerEditing[playerid] == EDITING_GROUP_POS) {
        for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
			groupTextDrawData[groupid][i][E_TEXTDRAW_Y] = value;
			CreateGroupTextDraw(groupid, i);
		}
		return 1;
	}

	new textdrawid = playerCurrentTextDraw[playerid];
	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_Y] = value;
	CreateGroupTextDraw(groupid, textdrawid);
	return 1;
}

CMD:lx(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

    new Float:value;
	if (sscanf(params, "%f", value)) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "Usage: /lx [value]");
	}

    new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];

	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X] = value;
	TextDrawLetterSize(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y]);
 	if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}
	return 1;
}

CMD:ly(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

    new Float:value;
	if (sscanf(params, "%f", value)) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "Usage: /ly [value]");
	}

    new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];

	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y] = value;
	TextDrawLetterSize(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y]);
 	if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}
	return 1;
}

CMD:tx(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

    new Float:value;
	if (sscanf(params, "%f", value)) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "Usage: /tx [value]");
	}

    new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];

	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X] = value;
	TextDrawTextSize(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y]);
 	if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}
	return 1;
}

CMD:ty(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

    new Float:value;
	if (sscanf(params, "%f", value)) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "Usage: /ty [value]");
	}

    new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];

	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y] = value;
	TextDrawTextSize(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y]);
 	if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}
	return 1;
}

CMD:rx(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

    new Float:value;
	if (sscanf(params, "%f", value)) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "Usage: /rx [value]");
	}

    new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];

	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X] = value;
	TextDrawSetPreviewRot(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
    if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}
	return 1;
}

CMD:ry(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

    new Float:value;
	if (sscanf(params, "%f", value)) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "Usage: /ry [value]");
	}

    new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];

	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y] = value;
	TextDrawSetPreviewRot(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
    if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}
	return 1;
}

CMD:rz(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

    new Float:value;
	if (sscanf(params, "%f", value)) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "Usage: /rz [value]");
	}

    new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];

	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z] = value;
	TextDrawSetPreviewRot(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
    if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}
	return 1;
}

CMD:zoom(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

    new Float:value;
	if (sscanf(params, "%f", value)) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "Usage: /zoom [value]");
	}

    new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];

	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM] = value;
	TextDrawSetPreviewRot(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
    if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}
	return 1;
}

CMD:position(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

	if (playerEditing[playerid] == EDITING_GROUP_POS) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: You are in Position Editing Mode already.");
	}

    playerEditing[playerid] = EDITING_POS;
	return 1;
}

CMD:lettersize(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

	if (playerEditing[playerid] == EDITING_GROUP_POS) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: You cannot use this command when in Group Position Editing Mode.");
	}

    playerEditing[playerid] = EDITING_LETTER_SIZE;
	return 1;
}

CMD:textsize(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

	if (playerEditing[playerid] == EDITING_GROUP_POS) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: You cannot use this command when in Group Position Editing Mode.");
	}

    playerEditing[playerid] = EDITING_TEXT_SIZE;
	return 1;
}

CMD:rotation(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

	if (playerEditing[playerid] == EDITING_GROUP_POS) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: You cannot use this command when in Group Position Editing Mode.");
	}

    playerEditing[playerid] = EDITING_PREVIEW_ROT;
	return 1;
}

CMD:shadow(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

	if (playerEditing[playerid] == EDITING_GROUP_POS) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: You cannot use this command when in Group Position Editing Mode.");
	}

    playerEditing[playerid] = EDITING_SHADOW_SIZE;
	return 1;
}

CMD:outline(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

	if (playerEditing[playerid] == EDITING_GROUP_POS) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: You cannot use this command when in Group Position Editing Mode.");
	}

    playerEditing[playerid] = EDITING_OUTLINE_SIZE;
	return 1;
}

CMD:string(playerid, params[]) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}

    new string[MAX_GROUP_TEXTDRAW_TEXT];
	if (sscanf(params, "%s["#MAX_GROUP_TEXTDRAW_TEXT"]", string)) {
	    return SendClientMessage(playerid, MESSAGE_COLOR, "Usage: /string [text]");
	}

    new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];
	
	format(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXT], MAX_GROUP_TEXTDRAW_TEXT, string);

	TextDrawSetString(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], string);
 	if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}
	return 1;
}

CMD:show(playerid) {
	if (playerEditing[playerid] == EDITING_NONE) {
	    return 1;
	}
	
	if (showTextDrawCmds) {
		PlayerTextDrawHide(playerid, playerEditingTextDraw[playerid]);
	}
	else {
		PlayerTextDrawShow(playerid, playerEditingTextDraw[playerid]);
	}
	showTextDrawCmds = !showTextDrawCmds;
	return 1;
}

CMD:text(playerid) {
	if (playerEditing[playerid] != EDITING_NONE) {
		return SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: You can't open editor menu while editing.");
	}
	
	if (!projectDB) {
		return Dialog_Show(playerid, MAIN_MENU, DIALOG_STYLE_TABLIST, "TDEditor: Main menu",
			""COL_GREEN"New Project\t"COL_GREY"Create an empty GUI project\n\
			"COL_YELLOW"Load Project\t"COL_GREY"Load a project from scriptfiles folder (Plug'n'Play)\n\
			"COL_YELLOW"Delete Project\t"COL_GREY"Delete a project from scriptfiles folder",
		"Select", "Close");
	}

	static string[512 + (MAX_GROUPS * (MAX_GROUP_NAME + 32))];
	string = ""COL_GREEN"Create New Group\t"COL_GREY"Add an empty group where you can add textdraws later (max groups you can add: "#MAX_GROUPS")\n\
		"COL_YELLOW"Export project\t"COL_GREY"You can export the whole project as a .pwn file or you can also choose to export certain groups\n\
		"COL_RED"Close Project\t"COL_GREY"Close the current project and go back to Main Menu\n";

	for (new i; i < groupsCount; i++) {
		format(string, sizeof (string), "%sGroup #%i: '%s'\t"COL_GREY"%i textdraws\n", string, i, groupData[i][E_GROUP_NAME], groupData[i][E_GROUP_TEXTDRAWS_COUNT]);
	}

	Dialog_Show(playerid, EDITOR_MENU, DIALOG_STYLE_TABLIST, "TDEditor: Editor menu", string, "Select", "Close");
	return 1;
}

Dialog:MAIN_MENU(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: Edtior closed.");
	}

	switch (listitem) {
		case 0: {
			Dialog_Show(playerid, NEW_PROJECT, DIALOG_STYLE_INPUT, "TDEditor: New project", ""COL_WHITE"Insert a "COL_GREEN"PROJECT-NAME"COL_WHITE" below to create.\n\n"COL_GREY"The project will be saved as a \".db\" file. Each project gets its\n"COL_GREY"own database file so its easy to manage and even share!", "Create", "Back");
		}

		case 1, 2: {
		    new Dini:h = dini_open(PATH_RECORD_FILE, 0);
		    new numFields = dini_num_fields(h);
		    
		    if (numFields == 0) {
		    	dini_close(h);
		    	
		        if (listitem == 1) {
					return Dialog_Show(playerid, LOAD_PROJECT, DIALOG_STYLE_TABLIST_HEADERS, "TDEditor: Load project", "File\tDate created\n"COL_RED"null\t"COL_RED"null", "Load", "Back");
				}
				else {
					return Dialog_Show(playerid, DELETE_PROJECT, DIALOG_STYLE_TABLIST_HEADERS, "TDEditor: Delete project", "File\tDate created\n"COL_RED"null\t"COL_RED"null", "Delete", "Back");
				}
			}

			new count;
			static string[MAX_PROJECTS * (MAX_PROJECT_NAME + 16 + 2)];
		    new field[DINI_MAX_FIELD_NAME];
			new value[DINI_MAX_FIELD_VALUE];
			
			string = "File\tDate Created\n";
		    for (new i; i < numFields; i++) {
				if (++count == MAX_PROJECTS) {
					SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: Maximum projects limit is set to "#MAX_PROJECTS", and the limit has reached. There might be more projects which are not listed in the dialog.");
					break;
				}

				dini_get_field_name(h, i, field);
				dini_get(h, i, value);
				format(string, sizeof (string), "%s%s\t%s\n", string, field, value);
			}

			dini_close(h);

			if (count == 0) {
		        if (listitem == 1) {
					return Dialog_Show(playerid, LOAD_PROJECT, DIALOG_STYLE_TABLIST_HEADERS, "TDEditor: Load project", "File\tDate created\n"COL_RED"null\t"COL_RED"null", "Load", "Back");
				}
				else {
					return Dialog_Show(playerid, DELETE_PROJECT, DIALOG_STYLE_TABLIST_HEADERS, "TDEditor: Delete project", "File\tDate created\n"COL_RED"null\t"COL_RED"null", "Delete", "Back");
				}
			}

			if (listitem == 1) {
				return Dialog_Show(playerid, LOAD_PROJECT, DIALOG_STYLE_TABLIST_HEADERS, "TDEditor: Load project", string, "Load", "Back");
			}
			else {
				return Dialog_Show(playerid, DELETE_PROJECT, DIALOG_STYLE_TABLIST_HEADERS, "TDEditor: Delete project", string, "Delete", "Back");
			}
		}
	}
	return 1;
}

Dialog:NEW_PROJECT(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return cmd_text(playerid);
	}

	new name[MAX_PROJECT_NAME + 3];
	if (sscanf(inputtext, "%s["#MAX_PROJECT_NAME"]", name)) {
		return Dialog_Show(playerid, NEW_PROJECT, DIALOG_STYLE_INPUT, "TDEditor: New project", ""COL_WHITE"Insert a "COL_GREEN"PROJECT-NAME"COL_WHITE" below to create.\n\n"COL_GREY"The project will be saved as a \".db\" file. Each project gets its\n"COL_GREY"own database file so its easy to manage and even share!\n\n"COL_RED"Error: "COL_GREY"The project name cannot be empty or spaces.", "Create", "Back");
	}

	new pos = strfind(name, ".db", true);
	if (pos == -1) {
		strcat(name, ".db");
	}
	
	new Dini:h = dini_open(PATH_RECORD_FILE, 0);
	
	if (dini_get_field_id(h, name) != -1 || fexist(name)) {
	    dini_close(h);
		return Dialog_Show(playerid, NEW_PROJECT, DIALOG_STYLE_INPUT, "TDEditor: New project", ""COL_WHITE"Insert a "COL_GREEN"PROJECT-NAME"COL_WHITE" below to create.\n\n"COL_GREY"The project will be saved as a \".db\" file. Each project gets its\n"COL_GREY"own database file so its easy to manage and even share!\n\n"COL_RED"Error: "COL_GREY"The project name already exists. Try something else or you can continue your work by loading that project instead!", "Create", "Back");
	}

	if (!strcmp(inputtext, "null", true)) {
	    dini_close(h);
		return Dialog_Show(playerid, NEW_PROJECT, DIALOG_STYLE_INPUT, "TDEditor: New project", ""COL_WHITE"Insert a "COL_GREEN"PROJECT-NAME"COL_WHITE" below to create.\n\n"COL_GREY"The project will be saved as a \".db\" file. Each project gets its\n"COL_GREY"own database file so its easy to manage and even share!\n\n"COL_RED"Error: "COL_GREY"The project name cannot be \"null\"!", "Create", "Back");
	}

	format(projectName, MAX_PROJECT_NAME, name);
	pos = strfind(projectName, ".db", true);
	if (pos != -1) {
		strdel(projectName, pos, (pos + strlen(".db")));
	}

	new string[150] = PATH_PROJECT_FILES;
	strcat(string, name);
	projectDB = db_open(string);
	if (!projectDB) {
		return Dialog_Show(playerid, NEW_PROJECT, DIALOG_STYLE_INPUT, "TDEditor: New project", ""COL_WHITE"Insert a "COL_GREEN"PROJECT-NAME"COL_WHITE" below to create.\n\n"COL_GREY"The project will be saved as a \".db\" file. Each project gets its\n"COL_GREY"own database file so its easy to manage and even share!\n\n"COL_RED"Error: "COL_GREY"Something went wrong! Try again or check your project name for invalid characters!", "Create", "Back");
	}
	db_query(projectDB, "PRAGMA synchronous = NORMAL");
 	db_query(projectDB, "PRAGMA journal_mode = WAL");

	format(string, sizeof (string), "TDEditor: A new project has been created \"%s\". Start editing!", name);
	SendClientMessage(playerid, MESSAGE_COLOR, string);

	if (!dini_set_assoc(h, name, ReturnDate(gettime()))) {
		SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: System couldn't open record file so your project name might not arrive on Loading dialog, but you project database is fine!");
	}

	dini_close(h);
	
	return cmd_text(playerid);
}

Dialog:LOAD_PROJECT(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return cmd_text(playerid);
	}

	if (!strcmp(inputtext, "null", true)) {
		return dialog_MAIN_MENU(playerid, 1, 1, "\1");
	}

	new string[256] = PATH_PROJECT_FILES;
	strcat(string, inputtext);
	projectDB = db_open(string);
	if (!projectDB) {
		SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: Couldn't load project file. Try again!");
		return dialog_MAIN_MENU(playerid, 1, 1, "\1");
	}
	db_query(projectDB, "PRAGMA synchronous = NORMAL");
 	db_query(projectDB, "PRAGMA journal_mode = WAL");

	format(projectName, MAX_PROJECT_NAME, inputtext);
	new pos = strfind(projectName, ".db", true);
	strdel(projectName, pos, (pos + strlen(".db")));
	
	new DBResult:result = db_query(projectDB, "SELECT name FROM sqlite_master WHERE type = 'table'");
	if (db_num_rows(result) > 0) {
		new DBResult:result2;
		new textdrawid;
		
		do {
		    if (groupsCount == MAX_GROUPS) {
		        SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: Only first \""#MAX_GROUPS"\" groups were loaded. If there are more than that in your database, you have to change the limit in script(MAX_GROUPS) and recompile.");
				break;
			}
			
		    db_get_field(result, 0, groupData[groupsCount][E_GROUP_NAME], MAX_GROUP_NAME);

			groupData[groupsCount][E_GROUP_TEXTDRAWS_COUNT] = 0;
			groupData[groupsCount][E_GROUP_VISIBLE] = true;
			
			format(string, sizeof (string), "SELECT * FROM %s", groupData[groupsCount][E_GROUP_NAME]);
			result2 = db_query(projectDB, string);
			if (result2) {
			    if (db_num_rows(result2) > 0) {
					do {
					    if (groupData[groupsCount][E_GROUP_TEXTDRAWS_COUNT] == MAX_GROUP_TEXTDRAWS) {
					        SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: Only first \""#MAX_GROUP_TEXTDRAWS"\" textdraws were loaded for group. If there are more than that in your database, you have to change the limit in script(MAX_GROUP_TEXTDRAWS) and recompile.");
							break;
						}

					    textdrawid = groupData[groupsCount][E_GROUP_TEXTDRAWS_COUNT]++;

	                    groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_SQLID] = db_get_field_assoc_int(result2, "id");
			            groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_ID] = Text:INVALID_TEXT_DRAW;
					    db_get_field_assoc(result2, "text", groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_TEXT], MAX_GROUP_TEXTDRAW_TEXT);
					    groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_X] = db_get_field_assoc_float(result2, "x");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_Y] = db_get_field_assoc_float(result2, "y");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_LETTERSIZE_X] = db_get_field_assoc_float(result2, "letter_x");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_LETTERSIZE_Y] = db_get_field_assoc_float(result2, "letter_y");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_TEXTSIZE_X] = db_get_field_assoc_float(result2, "text_x");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_TEXTSIZE_Y] = db_get_field_assoc_float(result2, "text_y");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_ALIGNMENT] = db_get_field_assoc_int(result2, "alignment");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_COLOR] = db_get_field_assoc_int(result2, "color");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_USE_BOX] = bool:db_get_field_assoc_int(result2, "usebox");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_BOX_COLOR] = db_get_field_assoc_int(result2, "box_color");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_SHADOW] = db_get_field_assoc_int(result2, "shadow");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_OUTLINE] = db_get_field_assoc_int(result2, "outline");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_BACKGROUND_COLOR] = db_get_field_assoc_int(result2, "background_color");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_FONT] = db_get_field_assoc_int(result2, "font");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_PROPORTIONAL] = bool:db_get_field_assoc_int(result2, "proportional");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_SELECTABLE] = bool:db_get_field_assoc_int(result2, "selectable");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_PREVIEW_MODEL] = db_get_field_assoc_int(result2, "preview_model");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X] = db_get_field_assoc_float(result2, "rot_x");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y] = db_get_field_assoc_float(result2, "rot_y");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z] = db_get_field_assoc_float(result2, "rot_z");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM] = db_get_field_assoc_float(result2, "rot_zoom");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_PREVIEW_VEH_COLOR1] = db_get_field_assoc_int(result2, "veh_color1");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_PREVIEW_VEH_COLOR2] = db_get_field_assoc_int(result2, "veh_color2");
						groupTextDrawData[groupsCount][textdrawid][E_TEXTDRAW_TYPE_PLAYER] = bool:db_get_field_assoc_int(result2, "type_player");

						CreateGroupTextDraw(groupsCount, textdrawid);
					}
					while (db_next_row(result2));
				}
				db_free_result(result2);
			}
			
			groupsCount++;
	    }
	    while (db_next_row(result));
	    db_free_result(result);
	}
	
	format(string, sizeof (string), "TDEditor: Loaded project \"%s\". Start editing!", inputtext);
	SendClientMessage(playerid, MESSAGE_COLOR, string);

	return cmd_text(playerid);
}

Dialog:DELETE_PROJECT(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return cmd_text(playerid);
	}

	if (!strcmp(inputtext, "null", true)) {
		return dialog_MAIN_MENU(playerid, 1, 1, "\1");
	}

	format(projectName, MAX_PROJECT_NAME, inputtext);

	new string[512];
	format(string, sizeof (string), ""COL_WHITE"Are you sure you want the delete this "COL_ORANGE"PROJECT"COL_WHITE"?\n\n"COL_RED"Project Name:\t"COL_GREY"%s\n\n"COL_GREY"Note: Deleting a project will erase the project permanently from scriptfiles folder. No recovey!", inputtext);
	return Dialog_Show(playerid, CONFIRM_DELETE_PROJECT, DIALOG_STYLE_MSGBOX, "TDEditor: Confirm delete project", string, "Yes", "No");
}

Dialog:CONFIRM_DELETE_PROJECT(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return dialog_MAIN_MENU(playerid, 1, 2, "\1");
	}

	fremove(projectName);
	
	new Dini:h = dini_open(PATH_RECORD_FILE, 0);
	if (!dini_remove_field_id(h, dini_get_field_id(h, projectName))) {
	    SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: System couldn't open record file so your project name might still be there in loading/deleteing dialog, but you project database has been deleted!");
	}
	dini_close(h);

	new string[150];
	format(string, sizeof (string), "TDEditor: Deleted project \"%s\".", projectName);
	SendClientMessage(playerid, MESSAGE_COLOR, string);

	return dialog_MAIN_MENU(playerid, 1, 2, "\1");
}

Dialog:EDITOR_MENU(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: Edtior closed.");
	}

	switch (listitem) {
		case 0: {
		    if (groupsCount == MAX_GROUPS) {
		        SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: You cannot add more than \""#MAX_GROUPS"\" groups. Change the limit in script(MAX_GROUPS) and recompile to be able to add more.");
				return cmd_text(playerid);
			}

		    return Dialog_Show(playerid, NEW_GROUP, DIALOG_STYLE_INPUT, "TDEditor: Create new group", ""COL_WHITE"Insert a textdraw "COL_GREEN"GROUP-NAME"COL_WHITE" below to create.\n\n"COL_GREY"What is a textdraw group?\n"COL_GREY"A textdraw group is a pack of textdraws made together to serve a purpose; e.g. a group with name\n'button' with textdraws made as a button in it. Later you can duplicate the group, move the whole textdraws in group, etc.", "Create", "Back");
		}

		case 1: {
		    new name[MAX_PROJECT_NAME * 2] = PATH_EXPORT_FILES;
			strcat(name, projectName);
			strcat(name, ".pwn");
			
   			if (!ExportProject(name)) {
				SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: Couldn't create project file. Try again!");
				return dialog_EDITOR_MENU(playerid, 1, 1, "\1");
	   		}

			new string[150];
			format(string, sizeof (string), "TDEditor: Project \"%s\" has been exported to file \"%s\".", projectName, name);
			SendClientMessage(playerid, MESSAGE_COLOR, string);
			
			return cmd_text(playerid);
		}

		case 2: {
			OnFilterScriptExit();
			SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: Project saved & closed.");
			return cmd_text(playerid);
		}

		default: {
		    return ShowPlayerGroupDialog(playerid, (listitem - 3));
		}
	}

	return 1;
}

Dialog:NEW_GROUP(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return cmd_text(playerid);
	}

	new name[MAX_GROUP_NAME];
	if (sscanf(inputtext, "%s["#MAX_GROUP_NAME"]", name)) {
		return Dialog_Show(playerid, NEW_GROUP, DIALOG_STYLE_INPUT, "TDEditor: Create new group",
			""COL_WHITE"Insert a textdraw "COL_GREEN"GROUP-NAME"COL_WHITE" below to create.\n\n\
			"COL_GREY"What is a textdraw group?\n\
			"COL_GREY"A textdraw group is a pack of textdraws made together to serve a purpose; e.g. a group with name\n\
			'button' with textdraws made as a button in it. Later you can duplicate the group, move the whole textdraws in group, etc.\n\n\
			"COL_RED"Error: "COL_GREY"The group name cannot be empty or spaces.",
			"Create", "Back");
	}

	format(groupData[groupsCount][E_GROUP_NAME], MAX_GROUP_NAME, name);
    groupData[groupsCount][E_GROUP_TEXTDRAWS_COUNT] = 0;
    groupData[groupsCount][E_GROUP_VISIBLE] = true;
    
	new string[1024] = "CREATE TABLE IF NOT EXISTS ";
	strcat(string, name);
	strcat(string, " (`id` INTEGER PRIMARY KEY, \
		`text` VARCHAR("#MAX_GROUP_TEXTDRAW_TEXT"), \
		`x` FLOAT, \
		`y` FLOAT, \
		`letter_x` FLOAT, \
		`letter_y` FLOAT, \
		`text_x` FLOAT, \
		`text_y` FLOAT, \
		`alignment` INTEGER, \
		`color` INTEGER, \
		`usebox` INTEGER, \
		`box_color` INTEGER, \
		`shadow` INTEGER, ");
	strcat(string, "`outline` INTEGER, \
		`background_color` INTEGER, \
		`font` INTEGER, \
		`proportional` INTEGER, \
		`selectable` INTEGER, \
		`preview_model` INTEGER, \
		`rot_x` FLOAT, \
		`rot_y` FLOAT, \
		`rot_z` FLOAT, \
		`rot_zoom` FLOAT, \
		`veh_color1` INTEGER, \
		`veh_color2` INTEGER, \
		`type_player` INTEGER)");
	db_query(projectDB, string);

	groupsCount++;

	format(string, sizeof (string), "TDEditor: A new textdraw group has been added: %s [Group #%i].", name, (groupsCount - 1));
	SendClientMessage(playerid, MESSAGE_COLOR, string);

	return cmd_text(playerid);
}

Dialog:GROUP_MENU(playerid, response, listitem, inputtext[]) {
	new groupid = playerCurrentGroup[playerid];
	
	if (!response) {
	    new string[1024];
	    for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
		    string = "UPDATE %s SET \
					text = '%q', \
					x = '%f', y = '%f', \
					letter_x = '%f', letter_y = '%f', \
					text_x = '%f', text_y = '%f', \
					alignment = '%i', \
					color = '%i', \
					usebox = '%i', \
					box_color = '%i', \
					shadow = '%i', \
					outline = '%i', \
					background_color = '%i', \
					font = '%i', \
					proportional = '%i', \
					selectable = '%i', \
					preview_model = '%i', \
					rot_x = '%f', rot_y = '%f', rot_z = '%f', rot_zoom = '%f', \
					veh_color1 = '%i', veh_color2 = '%i', \
					type_player = '%i' \
				WHERE id = '%i'";
			format(string, sizeof (string), string,
				groupData[groupid][E_GROUP_NAME],
			    groupTextDrawData[groupid][i][E_TEXTDRAW_TEXT],
			    groupTextDrawData[groupid][i][E_TEXTDRAW_X], groupTextDrawData[groupid][i][E_TEXTDRAW_Y],
			    groupTextDrawData[groupid][i][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[groupid][i][E_TEXTDRAW_LETTERSIZE_Y],
			    groupTextDrawData[groupid][i][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[groupid][i][E_TEXTDRAW_TEXTSIZE_Y],
			    groupTextDrawData[groupid][i][E_TEXTDRAW_ALIGNMENT],
			    groupTextDrawData[groupid][i][E_TEXTDRAW_COLOR],
			    groupTextDrawData[groupid][i][E_TEXTDRAW_USE_BOX],
				groupTextDrawData[groupid][i][E_TEXTDRAW_BOX_COLOR],
				groupTextDrawData[groupid][i][E_TEXTDRAW_SHADOW],
				groupTextDrawData[groupid][i][E_TEXTDRAW_OUTLINE],
				groupTextDrawData[groupid][i][E_TEXTDRAW_BACKGROUND_COLOR],
				groupTextDrawData[groupid][i][E_TEXTDRAW_FONT],
				groupTextDrawData[groupid][i][E_TEXTDRAW_PROPORTIONAL],
				groupTextDrawData[groupid][i][E_TEXTDRAW_SELECTABLE],
				groupTextDrawData[groupid][i][E_TEXTDRAW_PREVIEW_MODEL],
				groupTextDrawData[groupid][i][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][i][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][i][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][i][E_TEXTDRAW_PREVIEW_ROT_ZOOM],
				groupTextDrawData[groupid][i][E_TEXTDRAW_PREVIEW_VEH_COLOR1], groupTextDrawData[groupid][i][E_TEXTDRAW_PREVIEW_VEH_COLOR2],
				groupTextDrawData[groupid][i][E_TEXTDRAW_TYPE_PLAYER],
				groupTextDrawData[groupid][i][E_TEXTDRAW_SQLID]);
			db_query(projectDB, string);
	    }
		return cmd_text(playerid);
	}
	
	switch (listitem) {
        case 0: {
		    if (groupData[groupid][E_GROUP_TEXTDRAWS_COUNT] == MAX_GROUP_TEXTDRAWS) {
		        SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: You cannot add more than \""#MAX_GROUP_TEXTDRAWS"\" textdraws in a group. Change the limit in script(MAX_GROUP_TEXTDRAWS) and recompile to be able to add more.");
				return ShowPlayerGroupDialog(playerid, groupid);
			}

			new textdrawid = groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]++;

            groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID] = Text:INVALID_TEXT_DRAW;
		    format(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXT], MAX_GROUP_TEXTDRAW_TEXT, "New TextDraw");
		    groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_X] = 250.0;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_Y] = 10.0;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X] = 0.5;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y] = 1.0;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X] = 0.0;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y] = 0.0;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ALIGNMENT] = 1;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_COLOR] = 0xFFFFFFFF;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_USE_BOX] = false;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BOX_COLOR] = 0x000000FF;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SHADOW] = 0;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_OUTLINE] = 0;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BACKGROUND_COLOR] = 0x000000FF;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_FONT] = 1;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PROPORTIONAL] = true;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SELECTABLE] = false;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_MODEL] = 0;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X] = 0.0;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y] = 0.0;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z] = 0.0;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM] = 1.0;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_VEH_COLOR1] = -1;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_VEH_COLOR2] = -1;
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TYPE_PLAYER] = false;

			CreateGroupTextDraw(groupid, textdrawid);

		    new string[1024] = "INSERT INTO ";
			strcat(string, groupData[groupid][E_GROUP_NAME]);
			strcat(string, "(text, x, y, letter_x, letter_y, text_x, text_y, alignment, color, usebox, box_color, shadow, outline, background_color, font, proportional, selectable, preview_model, rot_x, rot_y, rot_z, rot_zoom, veh_color1, veh_color2, type_player)");
			strcat(string, "VALUES ('New TextDraw', '250.0', '10.0', '0.5', '1.0', '0.0', '0.0', '1', '-1', '0', '0xFF', '0', '0', '0xFF', '1', '1', '0', '0', '0.0', '0.0', '0.0', '1.0', '-1', '-1', '0')");
			db_query(projectDB, string);
			
			format(string, sizeof (string), "SELECT id FROM %s ORDER BY id DESC LIMIT 1", groupData[groupid][E_GROUP_NAME]);
			new DBResult:result = db_query(projectDB, string);
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SQLID] = db_get_field_int(result, 0);
			db_free_result(result);
			
			format(string, sizeof (string), "TDEditor: A new textdraw added: Text #%i [Group: %s].", textdrawid, groupData[groupid][E_GROUP_NAME]);
			SendClientMessage(playerid, MESSAGE_COLOR, string);

			return ShowPlayerGroupDialog(playerid, groupid);
		}

		case 1: {
		    if (groupData[groupid][E_GROUP_TEXTDRAWS_COUNT] == 0) {
		        SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: There are no textdraws in the group!");
				return ShowPlayerGroupDialog(playerid, groupid);
			}
		    
		    playerEditing[playerid] = EDITING_GROUP_POS;
		    playerEditingTimer[playerid] = SetTimerEx("OnPlayerTimerUpdate", 200, true, "i", playerid);
		    if (showTextDrawCmds) {
				PlayerTextDrawSetString(playerid, playerEditingTextDraw[playerid], "~w~Updating...");
				PlayerTextDrawShow(playerid, playerEditingTextDraw[playerid]);
			}
			
			TogglePlayerControllable(playerid, false);
		}

		case 2: {
		    if (groupData[groupid][E_GROUP_VISIBLE]) {
		        groupData[groupid][E_GROUP_VISIBLE] = false;
		        for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
		        	TextDrawHideForAll(groupTextDrawData[groupid][i][E_TEXTDRAW_ID]);
				}

				SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: Group textdraws visibility changed to OFF.");
		    }
		    else {
		        groupData[groupid][E_GROUP_VISIBLE] = true;
		        for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
		        	TextDrawShowForAll(groupTextDrawData[groupid][i][E_TEXTDRAW_ID]);
				}

				SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: Group textdraws visibility changed to ON.");
		    }

		    return ShowPlayerGroupDialog(playerid, groupid);
		}

		case 3: {
			new string[512];
			format(string, sizeof (string), ""COL_WHITE"Insert a new textdraw "COL_GREEN"GROUP-NAME"COL_WHITE" you want to call this group as!\n\n"COL_YELLOW"Current Name:\t"COL_WHITE"%s", groupData[groupid][E_GROUP_NAME]);
			return Dialog_Show(playerid, CHANGE_GROUP_NAME, DIALOG_STYLE_INPUT, "TDEditor: New project", string, "Change", "Back");
		}

		case 4: {
		    if (groupsCount == MAX_GROUPS) {
		        SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: You cannot add more than \""#MAX_GROUPS"\" groups. Change the limit in script(MAX_GROUPS) and recompile to be able to add more.");
				return ShowPlayerGroupDialog(playerid, groupid);
			}

		    groupData[groupsCount] = groupData[groupid];

			new bool:nameExist;
			new newName[MAX_GROUP_NAME];
			new groupName[MAX_GROUP_NAME];
			strmid(groupName, groupData[groupid][E_GROUP_NAME], 0, (MAX_GROUP_NAME - 3));

		    for (new x = 1; ; x++) {
				format(newName, MAX_GROUP_NAME, "%s_%i", groupName, x);
                nameExist = false;
                
				for (new i; i < groupsCount; i++) {
			        if (!strcmp(groupData[i][E_GROUP_NAME], newName, true)) {
			            nameExist = true;
						break;
					}
				}
				
				if (!nameExist) {
					break;
				}
		    }
		    
		    format(groupData[groupsCount][E_GROUP_NAME], MAX_GROUP_NAME, newName);
		    
			new string[1024] = "CREATE TABLE IF NOT EXISTS ";
			strcat(string, groupData[groupsCount][E_GROUP_NAME]);
			strcat(string, " (`id` INTEGER PRIMARY KEY, \
				`text` VARCHAR("#MAX_GROUP_TEXTDRAW_TEXT"), \
				`x` FLOAT, \
				`y` FLOAT, \
				`letter_x` FLOAT, \
				`letter_y` FLOAT, \
				`text_x` FLOAT, \
				`text_y` FLOAT, \
				`alignment` INTEGER, \
				`color` INTEGER, \
				`usebox` INTEGER, \
				`box_color` INTEGER, \
				`shadow` INTEGER, ");
			strcat(string, "`outline` INTEGER, \
				`background_color` INTEGER, \
				`font` INTEGER, \
				`proportional` INTEGER, \
				`selectable` INTEGER, \
				`preview_model` INTEGER, \
				`rot_x` FLOAT, \
				`rot_y` FLOAT, \
				`rot_z` FLOAT, \
				`rot_zoom` FLOAT, \
				`veh_color1` INTEGER, \
				`veh_color2` INTEGER, \
				`type_player` INTEGER)");
			db_query(projectDB, string);
		    
		    new DBResult:result;
		    for (new i; i < groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]; i++) {
		        for (new x; E_TEXTDRAW:x < E_TEXTDRAW; x++) {
		            if (E_TEXTDRAW:x == E_TEXTDRAW_ID || E_TEXTDRAW:x == E_TEXTDRAW_SQLID) {
		                continue;
					}
					
		    		groupTextDrawData[groupsCount][i][E_TEXTDRAW:x] = groupTextDrawData[groupid][i][E_TEXTDRAW:x];
				}
				
                string = "INSERT INTO ";
				strcat(string, groupData[groupsCount][E_GROUP_NAME]);
				strcat(string, "(text, x, y, letter_x, letter_y, text_x, text_y, alignment, color, usebox, box_color, shadow, outline, background_color, font, proportional, selectable, preview_model, rot_x, rot_y, rot_z, rot_zoom, veh_color1, veh_color2, type_player)");
				strcat(string, "VALUES ('%q', '%f', '%f', '%f', '%f', '%f', '%f', '%i', '%i', '%i', '%i', '%i', '%i', '%i', '%i', '%i', '%i', '%i', '%f', '%f', '%f', '%f', '%i', '%i', '%i')");
				format(string, sizeof (string), string,
				    groupTextDrawData[groupsCount][i][E_TEXTDRAW_TEXT],
				    groupTextDrawData[groupsCount][i][E_TEXTDRAW_X], groupTextDrawData[groupsCount][i][E_TEXTDRAW_Y],
				    groupTextDrawData[groupsCount][i][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[groupsCount][i][E_TEXTDRAW_LETTERSIZE_Y],
				    groupTextDrawData[groupsCount][i][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[groupsCount][i][E_TEXTDRAW_TEXTSIZE_Y],
				    groupTextDrawData[groupsCount][i][E_TEXTDRAW_ALIGNMENT],
				    groupTextDrawData[groupsCount][i][E_TEXTDRAW_COLOR],
				    groupTextDrawData[groupsCount][i][E_TEXTDRAW_USE_BOX],
					groupTextDrawData[groupsCount][i][E_TEXTDRAW_BOX_COLOR],
					groupTextDrawData[groupsCount][i][E_TEXTDRAW_SHADOW],
					groupTextDrawData[groupsCount][i][E_TEXTDRAW_OUTLINE],
					groupTextDrawData[groupsCount][i][E_TEXTDRAW_BACKGROUND_COLOR],
					groupTextDrawData[groupsCount][i][E_TEXTDRAW_FONT],
					groupTextDrawData[groupsCount][i][E_TEXTDRAW_PROPORTIONAL],
					groupTextDrawData[groupsCount][i][E_TEXTDRAW_SELECTABLE],
					groupTextDrawData[groupsCount][i][E_TEXTDRAW_PREVIEW_MODEL],
					groupTextDrawData[groupsCount][i][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupsCount][i][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupsCount][i][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupsCount][i][E_TEXTDRAW_PREVIEW_ROT_ZOOM],
					groupTextDrawData[groupsCount][i][E_TEXTDRAW_PREVIEW_VEH_COLOR1], groupTextDrawData[groupsCount][i][E_TEXTDRAW_PREVIEW_VEH_COLOR2],
					groupTextDrawData[groupsCount][i][E_TEXTDRAW_TYPE_PLAYER]);
				db_query(projectDB, string);
				
				format(string, sizeof (string), "SELECT id FROM %s ORDER BY id DESC LIMIT 1", groupData[groupsCount][E_GROUP_NAME]);
				result = db_query(projectDB, string);
				groupTextDrawData[groupsCount][i][E_TEXTDRAW_SQLID] = db_get_field_int(result, 0);
				db_free_result(result);
				
				groupTextDrawData[groupsCount][i][E_TEXTDRAW_ID] = Text:INVALID_TEXT_DRAW;
				CreateGroupTextDraw(groupsCount, i);
			}
			groupsCount++;
			

			format(string, sizeof (string), "TDEditor: Group '%s' duplicated to '%s_copy'.", groupData[groupid][E_GROUP_NAME], groupData[groupid][E_GROUP_NAME]);
			SendClientMessage(playerid, MESSAGE_COLOR, string);
			return cmd_text(playerid);
		}

		case 5: {
		    new string[512];
			format(string, sizeof (string), ""COL_WHITE"Are you sure you want the delete this "COL_ORANGE"TEXTDRAW-GROUP"COL_WHITE"?\n\n"COL_RED"Group Name:\t"COL_GREY"%s\n"COL_RED"Textdraws:\t"COL_GREY"%i items\n\n"COL_GREY"Note: Deleting a group will erase the group data/textdraws permanently from scriptfiles folder. No recovey!", groupData[groupid][E_GROUP_NAME], groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]);
			return Dialog_Show(playerid, CONFIRM_DELETE_GROUP, DIALOG_STYLE_MSGBOX, "TDEditor: Confirm delete group", string, "Yes", "No");
		}

		default: {
			return ShowPlayerTextDrawDialog(playerid, (listitem - 6));
		}
	}
	return 1;
}

Dialog:CONFIRM_DELETE_GROUP(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return ShowPlayerGroupDialog(playerid, playerCurrentGroup[playerid]);
	}

	new groupid = playerCurrentGroup[playerid];

	new string[150];
	format(string, sizeof (string), "DROP TABLE %s", groupData[groupid][E_GROUP_NAME]);
	db_query(projectDB, string);
	
	format(string, sizeof (string), "TDEditor: Deleted group \"%s\".", groupData[groupid][E_GROUP_NAME]);
	
	ResetGroupData(groupid);

	for (new i = groupid; i < (groupsCount - 1); i++) {
        for (new x; x < groupData[i + 1][E_GROUP_TEXTDRAWS_COUNT]; x++) {
			groupTextDrawData[i][x] = groupTextDrawData[i + 1][x];
		}
        groupData[i] = groupData[i + 1];
	}
	groupsCount--;

	SendClientMessage(playerid, MESSAGE_COLOR, string);
	return cmd_text(playerid);
}

Dialog:CHANGE_GROUP_NAME(playerid, response, listitem, inputtext[]) {
	if (response) {
		new name[MAX_GROUP_NAME];
		if (sscanf(inputtext, "%s["#MAX_GROUP_NAME"]", name)) {
		    new string[512];
			format(string, sizeof (string), ""COL_WHITE"Insert a new textdraw "COL_GREEN"GROUP-NAME"COL_WHITE" you want to call this group as!\n\n"COL_YELLOW"Current Name:\t"COL_WHITE"%s\n\n"COL_RED"Error: "COL_GREY"The group name cannot be empty or spaces.", groupData[playerCurrentGroup[playerid]][E_GROUP_NAME]);
			return Dialog_Show(playerid, CHANGE_GROUP_NAME, DIALOG_STYLE_INPUT, "TDEditor: New project", string, "Change", "Back");
		}
		
		for (new i; i < groupsCount; i++) {
		    if (!strcmp(name, groupData[i][E_GROUP_NAME], true)) {
		    	new string[512];
				format(string, sizeof (string), ""COL_WHITE"Insert a new textdraw "COL_GREEN"GROUP-NAME"COL_WHITE" you want to call this group as!\n\n"COL_YELLOW"Current Name:\t"COL_WHITE"%s\n\n"COL_RED"Error: "COL_GREY"The group name already exists, please choose a unique name which doesn't conflict.", groupData[playerCurrentGroup[playerid]][E_GROUP_NAME]);
				return Dialog_Show(playerid, CHANGE_GROUP_NAME, DIALOG_STYLE_INPUT, "TDEditor: New project", string, "Change", "Back");
			}
		}
		
		new groupid = playerCurrentGroup[playerid];

		new string[150];
		format(string, sizeof (string), "ALTER TABLE %s RENAME TO %s", groupData[groupid][E_GROUP_NAME], name);
		db_query(projectDB, string);
	
		format(string, sizeof (string), "TDEditor: Group name changed to \"%s\" from \"%s\".", name, groupData[groupid][E_GROUP_NAME]);
		SendClientMessage(playerid, MESSAGE_COLOR, string);

		format(groupData[groupid][E_GROUP_NAME], MAX_GROUP_NAME, name);
	}

	return ShowPlayerGroupDialog(playerid, playerCurrentGroup[playerid]);
}

Dialog:TEXTDRAW_MENU(playerid, response, listitem, inputtext[]) {
	new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];
	
	if (!response) {
	    new string[1024];
	    string = "UPDATE %s SET \
			text = '%q', \
			x = '%f', y = '%f', \
			letter_x = '%f', letter_y = '%f', \
			text_x = '%f', text_y = '%f', \
			alignment = '%i', \
			color = '%i', \
			usebox = '%i', \
			box_color = '%i', \
			shadow = '%i', \
			outline = '%i', \
			background_color = '%i', \
			font = '%i', \
			proportional = '%i', \
			selectable = '%i', \
			preview_model = '%i', \
			rot_x = '%f', rot_y = '%f', rot_z = '%f', rot_zoom = '%f', \
			veh_color1 = '%i', veh_color2 = '%i', \
			type_player = '%i' \
			WHERE id = '%i'";
		format(string, sizeof (string), string,
			groupData[groupid][E_GROUP_NAME],
		 	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXT],
		    groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_Y],
		    groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_LETTERSIZE_Y],
		    groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXTSIZE_Y],
		    groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ALIGNMENT],
		    groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_COLOR],
		    groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_USE_BOX],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BOX_COLOR],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SHADOW],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_OUTLINE],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BACKGROUND_COLOR],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_FONT],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PROPORTIONAL],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SELECTABLE],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_MODEL],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_VEH_COLOR1], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_VEH_COLOR2],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TYPE_PLAYER],
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SQLID]);
		db_query(projectDB, string);

		return ShowPlayerGroupDialog(playerid, playerCurrentGroup[playerid]);
	}
	
	switch (listitem) {
		case 0: {
		    return Dialog_Show(playerid, CHANGE_TEXT_OR_POSITION, DIALOG_STYLE_LIST, "TDEditor: Change text/position", "Change Text\nChange Position", "Select", "Back");
		}
		
		case 1: {
		    return Dialog_Show(playerid, CHANGE_FONT, DIALOG_STYLE_LIST, "TDEditor: Change font", "Font 0\nFont 1\nFont 2\nFont 3\nFont 4 (Sprite)\nFont 5 (Preview Model)", "Set", "Back");
		}

		case 2: {
		    return Dialog_Show(playerid, CHANGE_ALIGNMENT, DIALOG_STYLE_LIST, "TDEditor: Change alignment", "Alignment 1 (Left)\nAlignment 2 (Center)\nAlignment 3 (Right)", "Set", "Back");
		}

		case 3: {
			new string[150];

		    if (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PROPORTIONAL]) {
		        groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PROPORTIONAL] = false;

				format(string, sizeof (string), "TDEditor: Textdraw #%i Proportionality OFF.", textdrawid);
		    }
		    else {
		        groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PROPORTIONAL] = true;

				format(string, sizeof (string), "TDEditor: Textdraw #%i Proportionality ON.", textdrawid);
		    }

			TextDrawSetProportional(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PROPORTIONAL]);
			if (groupData[groupid][E_GROUP_VISIBLE]) {
				TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
			}

			SendClientMessage(playerid, MESSAGE_COLOR, string);

			return ShowPlayerTextDrawDialog(playerid, textdrawid);
		}

		case 4: {
		    playerEditing[playerid] = EDITING_SHADOW_SIZE;
		    playerEditingTimer[playerid] = SetTimerEx("OnPlayerTimerUpdate", 200, true, "i", playerid);
		    if (showTextDrawCmds) {
				PlayerTextDrawSetString(playerid, playerEditingTextDraw[playerid], "~w~Updating...");
				PlayerTextDrawShow(playerid, playerEditingTextDraw[playerid]);
			}

			TogglePlayerControllable(playerid, false);
		}

		case 5: {
		    playerEditing[playerid] = EDITING_OUTLINE_SIZE;
		    playerEditingTimer[playerid] = SetTimerEx("OnPlayerTimerUpdate", 200, true, "i", playerid);
		    if (showTextDrawCmds) {
				PlayerTextDrawSetString(playerid, playerEditingTextDraw[playerid], "~w~Updating...");
				PlayerTextDrawShow(playerid, playerEditingTextDraw[playerid]);
			}

			TogglePlayerControllable(playerid, false);
		}

		case 6: {
		    playerEditing[playerid] = EDITING_BACKGROUND_COLOR;
			Dialog_Show(playerid, CHANGE_COLOR, DIALOG_STYLE_LIST, "TDEditor: Change outline/shadow color", "Custom HEX code\nCustom RGBA input\nTDEditor's colors list", "Set", "Back");
		}

		case 7: {
		    playerEditing[playerid] = EDITING_LETTER_SIZE;
		    playerEditingTimer[playerid] = SetTimerEx("OnPlayerTimerUpdate", 200, true, "i", playerid);
		    if (showTextDrawCmds) {
				PlayerTextDrawSetString(playerid, playerEditingTextDraw[playerid], "~w~Updating...");
				PlayerTextDrawShow(playerid, playerEditingTextDraw[playerid]);
			}

			TogglePlayerControllable(playerid, false);
		}

		case 8: {
		    playerEditing[playerid] = EDITING_TEXTDRAW_COLOR;
			Dialog_Show(playerid, CHANGE_COLOR, DIALOG_STYLE_LIST, "TDEditor: Change textdraw color", "Custom HEX code\nCustom RGBA input\nTDEditor's colors list", "Set", "Back");
		}

		case 9: {
			new string[150];

		    if (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_USE_BOX]) {
		        groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_USE_BOX] = false;

				format(string, sizeof (string), "TDEditor: Textdraw #%i UseBox is set to OFF.", textdrawid);
		    }
		    else {
		        groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_USE_BOX] = true;

				format(string, sizeof (string), "TDEditor: Textdraw #%i UseBox is set to ON.", textdrawid);
		    }

			TextDrawUseBox(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_USE_BOX]);
			if (groupData[groupid][E_GROUP_VISIBLE]) {
				TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
			}

			SendClientMessage(playerid, MESSAGE_COLOR, string);

			return ShowPlayerTextDrawDialog(playerid, textdrawid);
		}

		case 10: {
		    playerEditing[playerid] = EDITING_TEXT_SIZE;
		    playerEditingTimer[playerid] = SetTimerEx("OnPlayerTimerUpdate", 200, true, "i", playerid);
		    if (showTextDrawCmds) {
				PlayerTextDrawSetString(playerid, playerEditingTextDraw[playerid], "~w~Updating...");
				PlayerTextDrawShow(playerid, playerEditingTextDraw[playerid]);
			}

			TogglePlayerControllable(playerid, false);
		}

		case 11: {
		    playerEditing[playerid] = EDITING_BOX_COLOR;
			Dialog_Show(playerid, CHANGE_COLOR, DIALOG_STYLE_LIST, "TDEditor: Change box color", "Custom HEX code\nCustom RGBA input\nTDEditor's colors list", "Set", "Back");
		}

		case 12: {
			new string[150];

		    if (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SELECTABLE]) {
		        groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SELECTABLE] = false;

				format(string, sizeof (string), "TDEditor: Textdraw #%i is Selectable.", textdrawid);
		    }
		    else {
		        groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SELECTABLE] = true;

				format(string, sizeof (string), "TDEditor: Textdraw #%i is not Selectable.", textdrawid);
		    }

			TextDrawSetSelectable(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SELECTABLE]);
			if (groupData[groupid][E_GROUP_VISIBLE]) {
				TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
			}

			SendClientMessage(playerid, MESSAGE_COLOR, string);

			return ShowPlayerTextDrawDialog(playerid, textdrawid);
		}
		
		case 13: {
			Dialog_Show(playerid, PREVIEW_MODEL_OPTIONS, DIALOG_STYLE_LIST, "TDEditor: Preview model options", "Modelid\nChange Rotation From Keyboard(X,Y only)\nInput Rotation (X,Y,Z,Zoom)", "Select", "Back");
		}

		case 14: {
		    new string[150];

		    if (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TYPE_PLAYER]) {
		        groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TYPE_PLAYER] = false;

				format(string, sizeof (string), "TDEditor: Textdraw #%i type changed to GLOBAL('Text:').", textdrawid);
		    }
		    else {
		        groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TYPE_PLAYER] = true;

				format(string, sizeof (string), "TDEditor: Textdraw #%i type changed to PLAYER('PlayerText:').", textdrawid);
		    }

			if (groupData[groupid][E_GROUP_VISIBLE]) {
				TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
			}

			SendClientMessage(playerid, MESSAGE_COLOR, string);

			return ShowPlayerTextDrawDialog(playerid, textdrawid);
		}

		case 15: {
		    if (groupData[groupid][E_GROUP_TEXTDRAWS_COUNT] == MAX_GROUP_TEXTDRAWS) {
		        SendClientMessage(playerid, MESSAGE_COLOR, "TDEditor: You cannot add more than \""#MAX_GROUP_TEXTDRAWS"\" textdraws in a group. Change the limit in script(MAX_GROUP_TEXTDRAWS) and recompile to be able to add more.");
				return ShowPlayerTextDrawDialog(playerid, textdrawid);
			}

		    new newTextdrawid = groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]++;
		    for (new i; E_TEXTDRAW:i < E_TEXTDRAW; i++) {
      			if (E_TEXTDRAW:i == E_TEXTDRAW_ID || E_TEXTDRAW:i == E_TEXTDRAW_SQLID) {
		          	continue;
				}

				groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW:i] = groupTextDrawData[groupid][textdrawid][E_TEXTDRAW:i];
			}

            new string[1024] = "INSERT INTO ";
			strcat(string, groupData[groupid][E_GROUP_NAME]);
			strcat(string, "(text, x, y, letter_x, letter_y, text_x, text_y, alignment, color, usebox, box_color, shadow, outline, background_color, font, proportional, selectable, preview_model, rot_x, rot_y, rot_z, rot_zoom, veh_color1, veh_color2, type_player)");
			strcat(string, "VALUES ('%q', '%f', '%f', '%f', '%f', '%f', '%f', '%i', '%i', '%i', '%i', '%i', '%i', '%i', '%i', '%i', '%i', '%i', '%f', '%f', '%f', '%f', '%i', '%i', '%i')");
			format(string, sizeof (string), string,
			    groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_TEXT],
			    groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_X], groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_Y],
			    groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_LETTERSIZE_X], groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_LETTERSIZE_Y],
			    groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_TEXTSIZE_X], groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_TEXTSIZE_Y],
			    groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_ALIGNMENT],
			    groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_COLOR],
			    groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_USE_BOX],
				groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_BOX_COLOR],
				groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_SHADOW],
				groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_OUTLINE],
				groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_BACKGROUND_COLOR],
				groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_FONT],
				groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_PROPORTIONAL],
				groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_SELECTABLE],
				groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_PREVIEW_MODEL],
				groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM],
				groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_PREVIEW_VEH_COLOR1], groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_PREVIEW_VEH_COLOR2],
				groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_TYPE_PLAYER]);
			db_query(projectDB, string);

			format(string, sizeof (string), "SELECT id FROM %s ORDER BY id DESC LIMIT 1", groupData[groupid][E_GROUP_NAME]);
			new DBResult:result = db_query(projectDB, string);
			groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_SQLID] = db_get_field_int(result, 0);
			db_free_result(result);

            groupTextDrawData[groupid][newTextdrawid][E_TEXTDRAW_ID] = Text:INVALID_TEXT_DRAW;
			CreateGroupTextDraw(groupid, newTextdrawid);

			format(string, sizeof (string), "TDEditor: A duplicate textdraw added: Text #%i [Group: %s].", newTextdrawid, groupData[groupid][E_GROUP_NAME]);
			SendClientMessage(playerid, MESSAGE_COLOR, string);

			return ShowPlayerGroupDialog(playerid, groupid);
		}

		case 16: {
			new previewChars[MAX_GROUP_TEXTDRAW_PREVIEW_CHARS + 4];
			strmid(previewChars, groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXT], 0, MAX_GROUP_TEXTDRAW_PREVIEW_CHARS);
			if (strlen(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXT]) > MAX_GROUP_TEXTDRAW_PREVIEW_CHARS) {
				strcat(previewChars, "...");
			}
			
			new fontName[16];
   			switch (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_FONT]) {
				case 0, 1, 2, 3: {
                    fontName = "Text";
				}
				
				case 4: {
                    fontName = "Sprite";
				}
				
				case 5: {
                    fontName = "Preview Model";
				}
			}
			
			new string[256];
			format(string, sizeof (string), ""COL_WHITE"Are you sure you want the delete "COL_ORANGE"TEXTDRAW: #%i"COL_WHITE"?\n\n"COL_RED"TextDraw Text:\t"COL_WHITE"'%s'\n"COL_RED"TextDraw Font:\t"COL_WHITE"%i (%s)", textdrawid, previewChars, groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_FONT], fontName);
			return Dialog_Show(playerid, CONFIRM_DELETE_TEXTDRAW, DIALOG_STYLE_MSGBOX, "TDEditor: Confirm delete group", string, "Yes", "No");
		}
	}
	return 1;
}

Dialog:CHANGE_TEXT_OR_POSITION(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return ShowPlayerTextDrawDialog(playerid, playerCurrentTextDraw[playerid]);
	}
	
	switch (listitem) {
	    case 0: {
			new groupid = playerCurrentGroup[playerid];
			new textdrawid = playerCurrentTextDraw[playerid];
			
	        new previewChars[MAX_GROUP_TEXTDRAW_PREVIEW_CHARS + 4];
			strmid(previewChars, groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXT], 0, MAX_GROUP_TEXTDRAW_PREVIEW_CHARS);
			if (strlen(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXT]) > MAX_GROUP_TEXTDRAW_PREVIEW_CHARS) {
				strcat(previewChars, "...");
			}
		
			new string[256];
		    format(string, sizeof(string), ""COL_WHITE"Insert a string to set as textxdraw "COL_GREEN"TEXT"COL_WHITE".\n\n"COL_YELLOW"Current Text: "COL_WHITE"'%s'", previewChars);
			Dialog_Show(playerid,  CHANGE_TEXT, DIALOG_STYLE_INPUT, "TDEditor: Change text", string, "Set", "Back");
		}

		case 1: {
			playerEditing[playerid] = EDITING_POS;
		    playerEditingTimer[playerid] = SetTimerEx("OnPlayerTimerUpdate", 200, true, "i", playerid);
		    if (showTextDrawCmds) {
				PlayerTextDrawSetString(playerid, playerEditingTextDraw[playerid], "~w~Updating...");
				PlayerTextDrawShow(playerid, playerEditingTextDraw[playerid]);
			}

			TogglePlayerControllable(playerid, false);
		}
	}
	return 1;
}

Dialog:CHANGE_TEXT(playerid, response, listitem, inputtext[]) {
	if (!response) {
	    return dialog_TEXTDRAW_MENU(playerid, 1, 0, "\1");
	}

	new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];
	
	new previewChars[MAX_GROUP_TEXTDRAW_PREVIEW_CHARS + 4];
	strmid(previewChars, groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXT], 0, MAX_GROUP_TEXTDRAW_PREVIEW_CHARS);
	if (strlen(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXT]) > MAX_GROUP_TEXTDRAW_PREVIEW_CHARS) {
		strcat(previewChars, "...");
	}
	
    new text[MAX_GROUP_TEXTDRAW_TEXT];
	if (sscanf(inputtext, "%s["#MAX_GROUP_TEXTDRAW_TEXT"]", text)) {
		new string[256];
		format(string, sizeof(string), ""COL_WHITE"Insert a string to set as textxdraw "COL_GREEN"TEXT"COL_WHITE".\n\n"COL_YELLOW"Current Text: "COL_WHITE"'%s'\n\n"COL_RED"Error: "COL_GREY"No text entered.", previewChars);
		return Dialog_Show(playerid,  CHANGE_TEXT, DIALOG_STYLE_INPUT, "TDEditor: Change text", string, "Set", "Back");
	}
	
    format(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_TEXT], MAX_GROUP_TEXTDRAW_TEXT, text);
	TextDrawSetString(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], text);
	if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}
	
	return dialog_TEXTDRAW_MENU(playerid, 1, 0, "\1");
}

Dialog:CHANGE_FONT(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return ShowPlayerTextDrawDialog(playerid, playerCurrentTextDraw[playerid]);
	}

	new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];

    groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_FONT] = listitem;
	TextDrawFont(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], listitem);
	if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}

	new string[150];
	format(string, sizeof (string), "TDEditor: Textdraw #%i Font changed to '%i'.", textdrawid, listitem);
	SendClientMessage(playerid, MESSAGE_COLOR, string);

	return ShowPlayerTextDrawDialog(playerid, textdrawid);
}

Dialog:CHANGE_ALIGNMENT(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return ShowPlayerTextDrawDialog(playerid, playerCurrentTextDraw[playerid]);
	}

	new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];

    groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ALIGNMENT] = (listitem + 1);
	TextDrawAlignment(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], (listitem + 1));
	if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}

	new string[150];
	format(string, sizeof (string), "TDEditor: Textdraw #%i Allignment changed to '%i'.", textdrawid, (listitem + 1));
	SendClientMessage(playerid, MESSAGE_COLOR, string);

	return ShowPlayerTextDrawDialog(playerid, textdrawid);
}

Dialog:CONFIRM_DELETE_TEXTDRAW(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return ShowPlayerTextDrawDialog(playerid, playerCurrentTextDraw[playerid]);
	}

	new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];

    new string[150];
	format(string, sizeof (string), "DELETE FROM %s WHERE id = %i", groupData[groupid][E_GROUP_NAME], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_SQLID]);
	db_query(projectDB, string);

	TextDrawDestroy(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
 	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID] = Text:INVALID_TEXT_DRAW;

	format(string, sizeof (string), "TDEditor: Deleted TextDraw #%i.", textdrawid);

	for (new i = textdrawid; i < (groupData[groupid][E_GROUP_TEXTDRAWS_COUNT] - 1); i++) {
        groupTextDrawData[groupid][i] = groupTextDrawData[groupid][i + 1];
	}
	groupData[groupid][E_GROUP_TEXTDRAWS_COUNT]--;

	SendClientMessage(playerid, MESSAGE_COLOR, string);
	return ShowPlayerGroupDialog(playerid, playerCurrentGroup[playerid]);
}

Dialog:CHANGE_COLOR(playerid, response, listitem, inputtext[]) {
	if (!response) {
  		return ShowPlayerTextDrawDialog(playerid, playerCurrentTextDraw[playerid]);
	}

	switch (listitem) {
	    case 0: {
	        return Dialog_Show(playerid, CUSTOM_HEX_COLOR, DIALOG_STYLE_INPUT, "TDEditor: Custom HEX color", ""COL_WHITE"Insert a "COL_GREEN"HEXA-DECIMAL"COL_WHITE" color code below.\n\n"COL_WHITE"(for example: '0xFFFFFFF' is color white)", "Set", "Back");
		}
		
		case 1: {
         	return Dialog_Show(playerid, CUSTOM_RGBA_COLOR, DIALOG_STYLE_INPUT, "TDEditor: Custom RGBA color", ""COL_WHITE"Insert color element value for "COL_RED"RED"COL_WHITE" below.\n\n"COL_WHITE"Maximum value of a color componenet can be 255.", "Next", "Back");
		}
		
		case 2: {
            static string[sizeof (COLORS) * ((32 * 2) + 1)];
            string[0] = EOS;
		    for (new i; i < sizeof (COLORS); i++) {
				format(string, sizeof (string), "%s{%06x}%s\n", string, (COLORS[i][E_COLOR_CODE] >>> 8), COLORS[i][E_COLOR_NAME]);
			}
			return Dialog_Show(playerid, COLOR_MENU, DIALOG_STYLE_LIST, "TDEditor: Colors menu", string, "Select", "Back");
		}
	}
	return 1;
}

Dialog:CUSTOM_HEX_COLOR(playerid, response, listitem, inputtext[]) {
	if (!response) {
	    switch (playerEditing[playerid]) {
			case EDITING_BACKGROUND_COLOR: {
			    return dialog_TEXTDRAW_MENU(playerid, 1, 6, "\1");
			}

			case EDITING_TEXTDRAW_COLOR: {
			    return dialog_TEXTDRAW_MENU(playerid, 1, 8, "\1");
			}

			case EDITING_BOX_COLOR: {
			    return dialog_TEXTDRAW_MENU(playerid, 1, 11, "\1");
			}
		}
	}

    new red[3], green[3], blue[3], alpha[3];

	if (inputtext[0] == '0' && inputtext[1] == 'x') { // He's using 0xFFFFFF format
		if (strlen(inputtext) != 8 && strlen(inputtext) != 10) {
		    return Dialog_Show(playerid, CUSTOM_HEX_COLOR, DIALOG_STYLE_INPUT, "TDEditor: Custom hex color", ""COL_WHITE"Insert a "COL_GREEN"HEXA-DECIMAL"COL_WHITE" color code below.\n\n"COL_WHITE"(for example: '0xFFFFFFF' is color white)\n\n"COL_RED"Error: "COL_GREY"Invalid hexa-decimal color code entered.", "Set", "Back");
		}

		format(red, sizeof (red), "%c%c", inputtext[2], inputtext[3]);
  		format(green, sizeof (green), "%c%c", inputtext[4], inputtext[5]);
  		format(blue, sizeof (blue), "%c%c", inputtext[6], inputtext[7]);
  		
   		if (inputtext[8] != '\0') {
			format(alpha, sizeof (alpha), "%c%c", inputtext[8], inputtext[9]);
		}
		else {
  			alpha = "FF";
		}
  	}
	else if (inputtext[0] == '#') { // He's using #FFFFFF format
		if (strlen(inputtext) != 7 && strlen(inputtext) != 9) {
		    return Dialog_Show(playerid, CUSTOM_HEX_COLOR, DIALOG_STYLE_INPUT, "TDEditor: Custom hex color", ""COL_WHITE"Insert a "COL_GREEN"HEXA-DECIMAL"COL_WHITE" color code below.\n\n"COL_WHITE"(for example: '0xFFFFFFF' is color white)\n\n"COL_RED"Error: "COL_GREY"Invalid hexa-decimal color code entered.", "Set", "Back");
		}

		format(red, sizeof (red), "%c%c", inputtext[1], inputtext[2]);
  		format(green, sizeof (green), "%c%c", inputtext[3], inputtext[4]);
  		format(blue, sizeof (blue), "%c%c", inputtext[5], inputtext[6]);

   		if (inputtext[7] != '\0') {
			format(alpha, sizeof (alpha), "%c%c", inputtext[7], inputtext[8]);
		}
		else {
  			alpha = "FF";
		}
  	}
  	else { // He's using FFFFFF format
		if (strlen(inputtext) != 6 && strlen(inputtext) != 8) {
		    return Dialog_Show(playerid, CUSTOM_HEX_COLOR, DIALOG_STYLE_INPUT, "TDEditor: Custom hex color", ""COL_WHITE"Insert a "COL_GREEN"HEXA-DECIMAL"COL_WHITE" color code below.\n\n"COL_WHITE"(for example: '0xFFFFFFF' is color white)\n\n"COL_RED"Error: "COL_GREY"Invalid hexa-decimal color code entered.", "Set", "Back");
		}

		format(red, sizeof (red), "%c%c", inputtext[0], inputtext[1]);
  		format(green, sizeof (green), "%c%c", inputtext[2], inputtext[3]);
  		format(blue, sizeof (blue), "%c%c", inputtext[4], inputtext[5]);

   		if (inputtext[7] != '\0') {
			format(alpha, sizeof (alpha), "%c%c", inputtext[6], inputtext[7]);
		}
		else {
  			alpha = "FF";
		}
	}
	
	new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];

	new string[150];
	switch (playerEditing[playerid]) {
		case EDITING_TEXTDRAW_COLOR: {
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_COLOR] = RGBA(HexToInt(red), HexToInt(green), HexToInt(blue), HexToInt(alpha));
			TextDrawColor(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_COLOR]);

			format(string, sizeof (string), "TDEditor: Textdraw #%i color changed to {%06x}Color Preview", textdrawid, (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_COLOR] >>> 8));
		}

		case EDITING_BOX_COLOR: {
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BOX_COLOR] = RGBA(HexToInt(red), HexToInt(green), HexToInt(blue), HexToInt(alpha));
			TextDrawBoxColor(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BOX_COLOR]);

			format(string, sizeof (string), "TDEditor: Textdraw #%i box color changed to {%06x}Color Preview", textdrawid, (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BOX_COLOR] >>> 8));
		}

		case EDITING_BACKGROUND_COLOR: {
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BACKGROUND_COLOR] = RGBA(HexToInt(red), HexToInt(green), HexToInt(blue), HexToInt(alpha));
			TextDrawBackgroundColor(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BACKGROUND_COLOR]);

			format(string, sizeof (string), "TDEditor: Textdraw #%i outline/shadow color changed to {%06x}Color Preview", textdrawid, (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BACKGROUND_COLOR] >>> 8));
		}
	}
	
	if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}

	SendClientMessage(playerid, MESSAGE_COLOR, string);

	return ShowPlayerTextDrawDialog(playerid, textdrawid);
}

Dialog:CUSTOM_RGBA_COLOR(playerid, response, listitem, inputtext[]) {
	if (!response) {
	    switch (GetPVarInt(playerid, "ColorElement")) {
			case 0: {
			    DeletePVar(playerid, "ColorElement");
			    DeletePVar(playerid, "ColorElement_R");
			    DeletePVar(playerid, "ColorElement_G");
			    DeletePVar(playerid, "ColorElement_B");
			    DeletePVar(playerid, "ColorElement_A");
			    
			    switch (playerEditing[playerid]) {
					case EDITING_BACKGROUND_COLOR: {
					    return dialog_TEXTDRAW_MENU(playerid, 1, 6, "\1");
					}

					case EDITING_TEXTDRAW_COLOR: {
					    return dialog_TEXTDRAW_MENU(playerid, 1, 8, "\1");
					}

					case EDITING_BOX_COLOR: {
					    return dialog_TEXTDRAW_MENU(playerid, 1, 11, "\1");
					}
				}
			}

			case 1: {
			    SetPVarInt(playerid, "ColorElement", 0);
				return Dialog_Show(playerid, CUSTOM_RGBA_COLOR, DIALOG_STYLE_INPUT, "TDEditor: Custom RGBA color", ""COL_WHITE"Insert color element value for "COL_RED"RED"COL_WHITE" below.\n\n"COL_WHITE"Maximum value of a color componenet can be 255.", "Next", "Back");
			}

			case 2: {
			    SetPVarInt(playerid, "ColorElement", 1);
				return Dialog_Show(playerid, CUSTOM_RGBA_COLOR, DIALOG_STYLE_INPUT, "TDEditor: Custom RGBA color", ""COL_WHITE"Insert color element value for "COL_GREEN"GREEN"COL_WHITE" below.\n\n"COL_WHITE"Maximum value of a color componenet can be 255.", "Next", "Back");
			}

			case 3: {
			    SetPVarInt(playerid, "ColorElement", 2);
				return Dialog_Show(playerid, CUSTOM_RGBA_COLOR, DIALOG_STYLE_INPUT, "TDEditor: Custom RGBA color", ""COL_WHITE"Insert color element value for "COL_BLUE"BLUE"COL_WHITE" below.\n\n"COL_WHITE"Maximum value of a color componenet can be 255.", "Next", "Back");
			}
		}
	}
	
	new val;
	if (!sscanf(inputtext, "%i", val)) {
		if (val >= 0 && val <= 255) {
		    switch (GetPVarInt(playerid, "ColorElement")) {
				case 0: {
				    SetPVarInt(playerid, "ColorElement", 1);
					SetPVarInt(playerid, "ColorElement_R", val);
					return Dialog_Show(playerid, CUSTOM_RGBA_COLOR, DIALOG_STYLE_INPUT, "TDEditor: Custom RGBA color", ""COL_WHITE"Insert color element value for "COL_GREEN"GREEN"COL_WHITE" below.\n\n"COL_WHITE"Maximum value of a color componenet can be 255.", "Next", "Back");
				}
				
				case 1: {
				    SetPVarInt(playerid, "ColorElement", 2);
					SetPVarInt(playerid, "ColorElement_G", val);
					return Dialog_Show(playerid, CUSTOM_RGBA_COLOR, DIALOG_STYLE_INPUT, "TDEditor: Custom RGBA color", ""COL_WHITE"Insert color element value for "COL_BLUE"BLUE"COL_WHITE" below.\n\n"COL_WHITE"Maximum value of a color componenet can be 255.", "Next", "Back");
				}
				
				case 2: {
				    SetPVarInt(playerid, "ColorElement", 3);
					SetPVarInt(playerid, "ColorElement_B", val);
					return Dialog_Show(playerid, CUSTOM_RGBA_COLOR, DIALOG_STYLE_INPUT, "TDEditor: Custom RGBA color", ""COL_WHITE"Insert color element value for "COL_GREY"ALPHA"COL_WHITE" below.\n\n"COL_WHITE"Maximum value of a color componenet can be 255.", "Next", "Back");
				}
				
				case 3: {
					new groupid = playerCurrentGroup[playerid];
					new textdrawid = playerCurrentTextDraw[playerid];
					
					new string[150];
					switch (playerEditing[playerid]) {
						case EDITING_TEXTDRAW_COLOR: {
							groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_COLOR] = RGBA(GetPVarInt(playerid, "ColorElement_R"), GetPVarInt(playerid, "ColorElement_G"), GetPVarInt(playerid, "ColorElement_B"), val);
							TextDrawColor(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_COLOR]);

							format(string, sizeof (string), "TDEditor: Textdraw #%i color changed to {%06x}Color Preview", textdrawid, (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_COLOR] >>> 8));
						}

						case EDITING_BOX_COLOR: {
							groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BOX_COLOR] = RGBA(GetPVarInt(playerid, "ColorElement_R"), GetPVarInt(playerid, "ColorElement_G"), GetPVarInt(playerid, "ColorElement_B"), val);
							TextDrawBoxColor(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BOX_COLOR]);

							format(string, sizeof (string), "TDEditor: Textdraw #%i box color changed to {%06x}Color Preview", textdrawid, (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BOX_COLOR] >>> 8));
						}

						case EDITING_BACKGROUND_COLOR: {
							groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BACKGROUND_COLOR] = RGBA(GetPVarInt(playerid, "ColorElement_R"), GetPVarInt(playerid, "ColorElement_G"), GetPVarInt(playerid, "ColorElement_B"), val);
							TextDrawBackgroundColor(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BACKGROUND_COLOR]);

							format(string, sizeof (string), "TDEditor: Textdraw #%i outline/shadow color changed to {%06x}Color Preview", textdrawid, (groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BACKGROUND_COLOR] >>> 8));
						}
					}

				    DeletePVar(playerid, "ColorElement");
				    DeletePVar(playerid, "ColorElement_R");
				    DeletePVar(playerid, "ColorElement_G");
				    DeletePVar(playerid, "ColorElement_B");
				    DeletePVar(playerid, "ColorElement_A");

					if (groupData[groupid][E_GROUP_VISIBLE]) {
						TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
					}

					SendClientMessage(playerid, MESSAGE_COLOR, string);

					return ShowPlayerTextDrawDialog(playerid, textdrawid);
				}
			}
 		}
	}
    return Dialog_Show(playerid, CUSTOM_RGBA_COLOR, DIALOG_STYLE_INPUT, "TDEditor: Custom RGBA color", ""COL_WHITE"Insert color element value for "COL_RED"RED"COL_WHITE" below.\n\n"COL_WHITE"Maximum value of a color componenet can be 255.", "Next", "Back");
}

Dialog:COLOR_MENU(playerid, response, listitem, inputtext[]) {
	if (!response) {
	    switch (playerEditing[playerid]) {
			case EDITING_BACKGROUND_COLOR: {
			    return dialog_TEXTDRAW_MENU(playerid, 1, 6, "\1");
			}
			
			case EDITING_TEXTDRAW_COLOR: {
			    return dialog_TEXTDRAW_MENU(playerid, 1, 8, "\1");
			}
			
			case EDITING_BOX_COLOR: {
			    return dialog_TEXTDRAW_MENU(playerid, 1, 11, "\1");
			}
		}
	}

	new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];

	new string[150];
	switch (playerEditing[playerid]) {
		case EDITING_TEXTDRAW_COLOR: {
			TextDrawColor(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], COLORS[listitem][E_COLOR_CODE]);
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_COLOR] = COLORS[listitem][E_COLOR_CODE];

			format(string, sizeof (string), "TDEditor: Textdraw #%i color changed to {%06x}%s", textdrawid, (COLORS[listitem][E_COLOR_CODE] >>> 8), COLORS[listitem][E_COLOR_NAME]);
		}

		case EDITING_BOX_COLOR: {
			TextDrawBoxColor(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], COLORS[listitem][E_COLOR_CODE]);
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BOX_COLOR] = COLORS[listitem][E_COLOR_CODE];

			format(string, sizeof (string), "TDEditor: Textdraw #%i box color changed to {%06x}%s", textdrawid, (COLORS[listitem][E_COLOR_CODE] >>> 8), COLORS[listitem][E_COLOR_NAME]);
		}

		case EDITING_BACKGROUND_COLOR: {
			TextDrawBackgroundColor(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], COLORS[listitem][E_COLOR_CODE]);
			groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_BACKGROUND_COLOR] = COLORS[listitem][E_COLOR_CODE];

			format(string, sizeof (string), "TDEditor: Textdraw #%i outline/shadow color changed to {%06x}%s", textdrawid, (COLORS[listitem][E_COLOR_CODE] >>> 8), COLORS[listitem][E_COLOR_NAME]);
		}
	}

	playerEditing[playerid] = EDITING_NONE;

	if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}

	SendClientMessage(playerid, MESSAGE_COLOR, string);

	return ShowPlayerTextDrawDialog(playerid, textdrawid);
}

Dialog:PREVIEW_MODEL_OPTIONS(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return ShowPlayerTextDrawDialog(playerid, playerCurrentTextDraw[playerid]);
	}
	
	switch (listitem) {
		case 0: {
		    Dialog_Show(playerid, SEARCH_PREVIEW_MODEL, DIALOG_STYLE_INPUT, "TDEditor: Change preview model", ""COL_WHITE"Insert exact "COL_GREEN"MODELID"COL_WHITE" or a "COL_GREEN"OBJECT NAME"COL_WHITE" or a hint so we can find all relative object models and give you a list!", "Search", "Back");
		}
		
		case 1: {
			playerEditing[playerid] = EDITING_PREVIEW_ROT;
		    playerEditingTimer[playerid] = SetTimerEx("OnPlayerTimerUpdate", 200, true, "i", playerid);
		    if (showTextDrawCmds) {
				PlayerTextDrawSetString(playerid, playerEditingTextDraw[playerid], "~w~Updating...");
				PlayerTextDrawShow(playerid, playerEditingTextDraw[playerid]);
			}

			TogglePlayerControllable(playerid, false);
		}

		case 2: {
			new groupid = playerCurrentGroup[playerid];
			new textdrawid = playerCurrentTextDraw[playerid];

		    new string[256];
		    format(string, sizeof(string), ""COL_WHITE"Insert a floating value for "COL_GREEN"X COORDINATE"COL_WHITE" of preview model rotation.\n\n"COL_YELLOW"Current Rot-X: "COL_WHITE"%0.2f", groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X]);
			Dialog_Show(playerid, INPUT_ROTATION, DIALOG_STYLE_INPUT, "TDEditor: Input X,Y,Z,Zoom manually", string, "Next", "Back");

			SetPVarInt(playerid, "RotationType", 0);
  		}
	}
	return 1;
}

Dialog:SEARCH_PREVIEW_MODEL(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return dialog_TEXTDRAW_MENU(playerid, 1, 13, "\1");
	}

	new name[32];
	if (sscanf(inputtext, "%s[32]", name)) {
		return Dialog_Show(playerid, SEARCH_PREVIEW_MODEL, DIALOG_STYLE_INPUT, "TDEditor: Search modelid", ""COL_WHITE"Insert exact "COL_GREEN"MODELID"COL_WHITE" or a "COL_GREEN"OBJECT NAME"COL_WHITE" or a hint so we can find all relative object models and give you a list!\n\n"COL_RED"Error: "COL_GREY"Model name not entered.", "Search", "Back");
	}
	
	if (!sscanf(name, "{i}")) {
	    new modelid = strval(name);
		if (modelid < 0 || modelid >= 44764) {
	   	 	return Dialog_Show(playerid, SEARCH_PREVIEW_MODEL, DIALOG_STYLE_INPUT, "TDEditor: Search modelid", ""COL_WHITE"Insert exact "COL_GREEN"MODELID"COL_WHITE" or a "COL_GREEN"OBJECT NAME"COL_WHITE" or a hint so we can find all relative object models and give you a list!\n\n"COL_RED"Error: "COL_GREY"Invalid object/modelid, must be between 0 to 44764.", "Search", "Back");
		}
		
		return dialog_CHOOSE_PREVIEW_MODEL(playerid, 1, 0, name);
	}
	else {
		new DB:db = db_open(PATH_OBJECTS_FILE);
		if (!db) {
			return Dialog_Show(playerid, SEARCH_PREVIEW_MODEL, DIALOG_STYLE_INPUT, "TDEditor: Search modelid", ""COL_WHITE"Insert exact "COL_GREEN"MODELID"COL_WHITE" or a "COL_GREEN"OBJECT NAME"COL_WHITE" or a hint so we can find all relative object models and give you a list!\n\n"COL_RED"Error: "COL_GREY"Database file \"allbuildings.db\" wasn't found in scriptfiles.", "Search", "Back");
		}

		new string[256];
		format(string, sizeof (string), "SELECT Model, Model_Name FROM buildings WHERE Model_Name LIKE '%s%q%s'", "%", name, "%");
		new DBResult:result = db_query(db, string);
		if (!result) {
		    db_close(db);
			return Dialog_Show(playerid, SEARCH_PREVIEW_MODEL, DIALOG_STYLE_INPUT, "TDEditor: Search modelid", ""COL_WHITE"Insert exact "COL_GREEN"MODELID"COL_WHITE" or a "COL_GREEN"OBJECT NAME"COL_WHITE" or a hint so we can find all relative object models and give you a list!\n\n"COL_RED"Error: "COL_GREY"Something went wrong while performing query, Please try again.", "Search", "Back");
		}

		if (db_num_rows(result) == 0) {
		    db_free_result(result);
		    db_close(db);
			return Dialog_Show(playerid, SEARCH_PREVIEW_MODEL, DIALOG_STYLE_INPUT, "TDEditor: Search modelid", ""COL_WHITE"Insert exact "COL_GREEN"MODELID"COL_WHITE" or a "COL_GREEN"OBJECT NAME"COL_WHITE" or a hint so we can find all relative object models and give you a list!\n\n"COL_RED"Error: "COL_GREY"No results found, try some other name.", "Search", "Back");
		}

		new count;
		static info[100 * (32 + 1)];
		info = "Model\tModel Name\n";

		do {
		    if (count++ == 100) {
				break;
			}

		    db_get_field(result, 1, name, sizeof (name));
		    format(info, sizeof (info), "%s%i\t%s\n", info, db_get_field_int(result, 0), name);
		}
		while (db_next_row(result));

		db_free_result(result);
		db_close(db);

		return Dialog_Show(playerid, CHOOSE_PREVIEW_MODEL, DIALOG_STYLE_TABLIST_HEADERS, "TDEditor: Search modelid", info, "Set", "Back");
	}
}

Dialog:CHOOSE_PREVIEW_MODEL(playerid, response, listitem, inputtext[]) {
	if (!response) {
		return dialog_PREVIEW_MODEL_OPTIONS(playerid, 1, 0, "\1");
	}

	new modelid = strval(inputtext);

	new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];

	TextDrawSetPreviewModel(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], modelid);
	groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_MODEL] = modelid;
	if (groupData[groupid][E_GROUP_VISIBLE]) {
		TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
	}

	new string[150];
	format(string, sizeof (string), "TDEditor: Textdraw #%i preview model changed to \"%i\"", textdrawid, modelid);
	SendClientMessage(playerid, MESSAGE_COLOR, string);
	return dialog_TEXTDRAW_MENU(playerid, 1, 13, "\1");
}

Dialog:INPUT_ROTATION(playerid, response, listitem, inputtext[]) {
	new groupid = playerCurrentGroup[playerid];
	new textdrawid = playerCurrentTextDraw[playerid];
	
	if (!response) {
		new string[256];
		switch (GetPVarInt(playerid, "RotationType")) {
		    case 0: {
		    	DeletePVar(playerid, "RotationType");
		    
		        return dialog_TEXTDRAW_MENU(playerid, 1, 13, "\1");
			}

		    case 1: {
    			SetPVarInt(playerid, "RotationType", 0);
    			
				format(string, sizeof(string), ""COL_WHITE"Insert a floating value for "COL_GREEN"X COORDINATE"COL_WHITE" of preview model rotation.\n\n"COL_YELLOW"Current Rot-X: "COL_WHITE"%0.2f", groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X]);
			}

		    case 2: {
    			SetPVarInt(playerid, "RotationType", 1);

				format(string, sizeof(string), ""COL_WHITE"Insert a floating value for "COL_GREEN"Y COORDINATE"COL_WHITE" of preview model rotation.\n\n"COL_YELLOW"Current Rot-Y: "COL_WHITE"%0.2f", groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y]);
			}

		    case 3: {
    			SetPVarInt(playerid, "RotationType", 2);

				format(string, sizeof(string), ""COL_WHITE"Insert a floating value for "COL_GREEN"Z COORDINATE"COL_WHITE" of preview model rotation.\n\n"COL_YELLOW"Current Rot-Z: "COL_WHITE"%0.2f", groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z]);
			}
		}
		
		return Dialog_Show(playerid, INPUT_ROTATION, DIALOG_STYLE_INPUT, "TDEditor: Input X,Y,Z,Zoom manuually", string, "Next", "Back");
	}
	
	new Float:val = -1.0;
	if (sscanf(inputtext, "%f", val) || ((GetPVarInt(playerid, "RotationType") == 3) ? (val < 0.0 || val > 100.0) : (val < -360.0 || val > 360.0))) {
		new string[256];
		switch (GetPVarInt(playerid, "RotationType")) {
		    case 0: {
				format(string, sizeof(string), ""COL_WHITE"Insert a floating value for "COL_GREEN"X COORDINATE"COL_WHITE" of preview model rotation.\n\n"COL_YELLOW"Current Rot-X: "COL_WHITE"%0.2f\n\n"COL_RED"Error: "COL_GREY"No value entered or out of bounds (must be between '-360' - '360')", groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X]);
			}
			
		    case 1: {
				format(string, sizeof(string), ""COL_WHITE"Insert a floating value for "COL_GREEN"Y COORDINATE"COL_WHITE" of preview model rotation.\n\n"COL_YELLOW"Current Rot-Y: "COL_WHITE"%0.2f\n\n"COL_RED"Error: "COL_GREY"No value entered or out of bounds (must be between '-360' - '360')", groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y]);
			}
			
		    case 2: {
				format(string, sizeof(string), ""COL_WHITE"Insert a floating value for "COL_GREEN"Z COORDINATE"COL_WHITE" of preview model rotation.\n\n"COL_YELLOW"Current Rot-Z: "COL_WHITE"%0.2f\n\n"COL_RED"Error: "COL_GREY"No value entered or out of bounds (must be between '-360' - '360')", groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z]);
			}
			
		    case 3: {
				format(string, sizeof(string), ""COL_WHITE"Insert a floating value for "COL_GREEN"ZOOM"COL_WHITE" of preview model rotation.\n\n"COL_YELLOW"Current Rot-Zoom: "COL_WHITE"%0.2f\n\n"COL_RED"Error: "COL_GREY"No value entered or out of bounds (must be between '0' - '100')", groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
				return Dialog_Show(playerid, INPUT_ROTATION, DIALOG_STYLE_INPUT, "TDEditor: Input X,Y,Z,Zoom manuually", string, "Finish", "Back");
			}
		}
		return Dialog_Show(playerid, INPUT_ROTATION, DIALOG_STYLE_INPUT, "TDEditor: Input X,Y,Z,Zoom manuually", string, "Next", "Back");
	}

	new string[256];
	switch (GetPVarInt(playerid, "RotationType")) {
	    case 0: {
	        groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X] = val;
	        TextDrawSetPreviewRot(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
			if (groupData[groupid][E_GROUP_VISIBLE]) {
				TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
			}
			
    		SetPVarInt(playerid, "RotationType", 1);

			format(string, sizeof(string), ""COL_WHITE"Insert a floating value for "COL_GREEN"Y COORDINATE"COL_WHITE" of preview model rotation.\n\n"COL_YELLOW"Current Rot-Y: "COL_WHITE"%0.2f", groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y]);
		}

	    case 1: {
	        groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y] = val;
	        TextDrawSetPreviewRot(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
			if (groupData[groupid][E_GROUP_VISIBLE]) {
				TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
			}

    		SetPVarInt(playerid, "RotationType", 2);

			format(string, sizeof(string), ""COL_WHITE"Insert a floating value for "COL_GREEN"Z COORDINATE"COL_WHITE" of preview model rotation.\n\n"COL_YELLOW"Current Rot-Z: "COL_WHITE"%0.2f", groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z]);
		}

	    case 2: {
	        groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z] = val;
	        TextDrawSetPreviewRot(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
			if (groupData[groupid][E_GROUP_VISIBLE]) {
				TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
			}

    		SetPVarInt(playerid, "RotationType", 3);

			format(string, sizeof(string), ""COL_WHITE"Insert a floating value for "COL_GREEN"ZOOM"COL_WHITE" of preview model rotation.\n\n"COL_YELLOW"Current Rot-Zoom: "COL_WHITE"%0.2f", groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
			return Dialog_Show(playerid, INPUT_ROTATION, DIALOG_STYLE_INPUT, "TDEditor: Input X,Y,Z,Zoom manuually", string, "Finish", "Back");
		}

	    case 3: {
	        groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM] = val;
	        TextDrawSetPreviewRot(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_X], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Y], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_Z], groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_PREVIEW_ROT_ZOOM]);
			if (groupData[groupid][E_GROUP_VISIBLE]) {
				TextDrawShowForAll(groupTextDrawData[groupid][textdrawid][E_TEXTDRAW_ID]);
			}

			DeletePVar(playerid, "RotationType");

			return dialog_TEXTDRAW_MENU(playerid, 1, 13, "\1");
		}
	}

	return Dialog_Show(playerid, INPUT_ROTATION, DIALOG_STYLE_INPUT, "TDEditor: Input X,Y,Z,Zoom manuually", string, "Next", "Back");
}
