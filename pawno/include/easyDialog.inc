/******************************************

// easyDialog.inc || v2.0 || Last Update: 19 Sept, 2018 || By Emmet_ || Reworked By Gammix

*******************************************

// Functions
native Dialog_Show(playerid, function[], style, caption[], info[], button1[], button2[]);
native Dialog_Hide(playerid);
native Dialog_IsActive(playerid);
native Dialog_GetID(playerid, dest[], maxlength = sizeof(dest));

*******************************************

// Callbacks
forward OnDialogRecieved(playerid, dialog[], response, listitem, inputtext[]);
forward OnDialogPerformed(playerid, dialog[], response, listitem, inputtext[], bool:success);

******************************************/

#if defined easydialog_included
	#endinput
#endif
#define easydialog_included

#if !defined easyDialog_ID
	#define easyDialog_ID (32700)
#endif

#if !defined MAX_DIALOG_INPUTTEXT
	#define MAX_DIALOG_INPUTTEXT (128)
#endif

#define Dialog:%0(%1) \
	forward dialog_%0(%1); public dialog_%0(%1)

#define DIALOG:%0(%1) \
	Dialog:%0(%1)

#define dialog(%0,%1) \
	Dialog:%0(%1)

#define Dialog_Show(%0,%1, \
	Dialog_Open(%0, #%1,

#define Dialog_Opened \
    Dialog_IsActive

#define Dialog_Close \
    Dialog_Hide

#if defined OnDialogRecieved
	forward OnDialogRecieved(playerid, dialog[], response, listitem, inputtext[]);
#endif

#if defined OnDialogPerformed
	forward OnDialogPerformed(playerid, dialog[], response, listitem, inputtext[], bool:success);
#endif

forward _dialog_format();
public _dialog_format() {
	format("", 0, "");
}

stock Dialog_Open(playerid, function[], style, caption[], info[], button1[], button2[]) {
	new ret = ShowPlayerDialog(playerid, easyDialog_ID, style, caption, info, button1, button2);
	if (ret != 0) {
	    SetPVarString(playerid, "dialogid", function);
	}

	return ret;
}
#if defined _ALS_ShowPlayerDialog
	#undef ShowPlayerDialog
#else
	#define _ALS_ShowPlayerDialog
#endif
#define ShowPlayerDialog(%0,%1,%2) Dialog_Open(%0, #%1, %2)

stock Dialog_IsActive(playerid) {
	return (GetPVarType(playerid, "dialogid") != PLAYER_VARTYPE_NONE);
}

stock Dialog_GetID(playerid, dest[], maxlength = sizeof(dest)) {
	if (GetPVarType(playerid, "dialogid") != PLAYER_VARTYPE_NONE)
	    return 0;
	    
	GetPVarString(playerid, "dialogid", dest, maxlength);
	return 1;
}

stock Dialog_Hide(playerid) {
	DeletePVar(playerid, "dialogid");

	return ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, " ", " ", " ", "");
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
	if (dialogid != easyDialog_ID) {
	    #if defined ED_OnDialogResponse
		    return ED_OnDialogResponse(playerid, dialogid, response, listitem, input);
		#else
		    return 0;
		#endif
	}
	
	new dialogstr[31];
	GetPVarString(playerid, "dialogid", dialogstr, sizeof(dialogstr));
	DeletePVar(playerid, "dialogid");
    	
	#if defined OnDialogRecieved
 		if (OnDialogRecieved(playerid, dialogstr, response, listitem, inputtext) == 0)
 		    return 1;
	#endif
	
    new input[MAX_DIALOG_INPUTTEXT * 2];
    new pos;

	strcat(input, inputtext);
	while ((pos = strfind(input, "%", true, pos), pos) != -1)
		strins(input, "%", pos + 1);

	new func[31] = "dialog_";
	strcat(func, dialogstr);

	#if defined OnDialogPerformed
	    OnDialogPerformed(playerid, dialogstr, response, listitem, input, (funcidx(func) != -1));
	#endif

	return 0;
}
#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif
#define OnDialogResponse ED_OnDialogResponse
#if defined ED_OnDialogResponse
	forward ED_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif
