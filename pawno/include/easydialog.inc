// easydialog.inc - v2.0.0 - By Emmet_ & Gammix - Last updated on 7 Jan, 2017
#if defined easydialog_included
	#endinput
#endif
#define easydialog_included

#define DIALOG:%0(%1) \
	forward dialog_%0(%1); public dialog_%0(%1)
	
#define Dialog:%0(%1) \
	forward dialog_%0(%1); public dialog_%0(%1)

#if !defined DIALOG_DEFAULT_ID
	#define DIALOG_DEFAULT_ID (32700)
#endif

static DIALOG_sName[MAX_PLAYERS][32];

stock Dialog_Open(playerid, function[], style, caption[], info[], button1[], button2[])
{
	new ret = ShowPlayerDialog(playerid, DIALOG_DEFAULT_ID, style, caption, info, button1, button2);
	if (ret)
	{
		format(DIALOG_sName[playerid], sizeof(DIALOG_sName[]), function);
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
	
	if (dialogid == DIALOG_DEFAULT_ID && strlen(DIALOG_sName[playerid]) > 0)
	{
	  	new string[40]= "dialog_";
		strcat(string, DIALOG_sName[playerid]);
        DIALOG_sName[playerid][0] = EOS;

		CallLocalFunction(string, "ddds", playerid, response, listitem, (!inputtext[0]) ? ("\1") : (inputtext));
	}
	
	#if defined DIALOG_OnDialogResponse
	    return DIALOG_OnDialogResponse(playerid, dialogid, response, listitem, inputtext);
	#else
	    return 0;
	#endif
}
#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif
#define OnDialogResponse DIALOG_OnDialogResponse
#if defined DIALOG_OnDialogResponse
	forward DIALOG_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif
