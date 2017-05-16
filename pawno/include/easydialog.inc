// easydialog.inc - By Emmet_ & Gammix - Last updated on 16 May, 2017
#if defined easydialog_included
	#endinput
#endif
#define easydialog_included

#define DIALOG:%0(%1) \
	forward dialog_%0(%1); public dialog_%0(%1)

#define Dialog:%0(%1) \
	forward dialog_%0(%1); public dialog_%0(%1)

#define DIALOG_DEFAULT_ID (32700)

static Dialog_playerDialogId[MAX_PLAYERS][31];

stock Dialog_Open(playerid, function[], style, caption[], info[], button1[], button2[])
{
	new ret = ShowPlayerDialog(playerid, DIALOG_DEFAULT_ID, style, caption, info, button1, button2);
	if (ret)
	{
	    Dialog_playerDialogId[playerid] = "dialog_";
	    strcat(Dialog_playerDialogId[playerid], function);
	}
	return ret;
}
#if defined _ALS_ShowPlayerDialog
	#undef ShowPlayerDialog
#else
	#define _ALS_ShowPlayerDialog
#endif

#define ShowPlayerDialog(%0,%1, \
	Dialog_Open(%0, #%1,

#define Dialog_Show(%0,%1, \
	Dialog_Open(%0, #%1,

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	for (new i = 0, l = strlen(inputtext); i < l; i ++)
	{
	    if (inputtext[i] == '%')
		{
			inputtext[i] = '#';
		}
	}

	if (dialogid == DIALOG_DEFAULT_ID && Dialog_playerDialogId[playerid][0])
	{
		CallLocalFunction(Dialog_playerDialogId[playerid], "ddds", playerid, response, listitem, (!inputtext[0]) ? ("\1") : (inputtext));
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
