#if defined easydialog_included
	#endinput
#endif
#define easydialog_included

// easydialog.inc - By Emmet_ - Last updated on 21 Sept, 2017

#define DIALOG:%0(%1) \
	forward dialog_%0(%1); public dialog_%0(%1)

#define Dialog:%0(%1) \
	forward dialog_%0(%1); public dialog_%0(%1)

#if !defined DIALOG_DEFAULT_ID
	#define DIALOG_DEFAULT_ID \
		32700
#endif

#if !defined DIALOG_PREFIX
	#define DIALOG_PREFIX \
	    "dialog_"
#endif

#if !defined DIALOG_PREFIX_LENGTH
	#define DIALOG_PREFIX_LENGTH \
	    7
#endif

static playerDialogID[MAX_PLAYERS][31 - DIALOG_PREFIX_LENGTH];

/*
native Dialog_Hide(playerid);
*/
#define Dialog_Hide(%0) \
	ShowPlayerDialog(playerid, -1, -1, "", "", "", "")

/*
native Dialog_GetID(playerid, dest[], maxlength);
*/
#define Dialog_GetID(%0,%1,%2) \
	format(%1, %2, playerDialogID[%0]);
	
stock Dialog_ShowPlayerDialog(playerid, function[], style, caption[], info[], button1[], button2[])
{
	new bool:ret = bool:ShowPlayerDialog(playerid, DIALOG_DEFAULT_ID, style, caption, info, button1, button2);
	if (ret)
	{
	    playerDialogID[playerid] = DIALOG_PREFIX;
	    strcat(playerDialogID[playerid], function);
	}
	return ret;
}
#if defined _ALS_ShowPlayerDialog
	#undef ShowPlayerDialog
#else
	#define _ALS_ShowPlayerDialog
#endif

#define ShowPlayerDialog(%0,%1, \
	Dialog_ShowPlayerDialog(%0, #%1,

/*
native Dialog_Show(playerid, Function<>, style, caption[], info[], button1[], button2[]);
*/
#define Dialog_Show(%0,%1, \
	Dialog_ShowPlayerDialog(%0, #%1,

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	for (new i = 0, l = strlen(inputtext); i < l; i ++)
	{
	    if (inputtext[i] == '%')
		{
			inputtext[i] = '#';
		}
	}

	if (dialogid == DIALOG_DEFAULT_ID && playerDialogID[playerid][0])
	{
		CallLocalFunction(playerDialogID[playerid], "ddds", playerid, response, listitem, ((!inputtext[0]) ? ("\1") : (inputtext)));
		playerDialogID[playerid][0] = EOS;
	}
	else return 0;

	#if defined Dialog_OnDialogResponse
	    return Dialog_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
	#else
	    return 1;
	#endif
}
#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif
#define OnDialogResponse Dialog_OnDialogResponse
#if defined Dialog_OnDialogResponse
	forward Dialog_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif
