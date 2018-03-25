#if defined easydialog_included
	#endinput
#endif
#define easydialog_included

#if !defined easyDialog_ID
	#define easyDialog_ID (32700)
#endif

#define Dialog:%0(%1) \
	forward dialog_%0(%1); public dialog_%0(%1)

#define DIALOG:%0(%1) \
	Dialog:%0(%1)

#define dialog(%0,%1) \
	Dialog:%0(%1)

#define Dialog_Show(%0,%1, \
	Dialog_Open(%0, #%1,

#if defined previewmodel_dialog_included
	#define Dialog_ShowPreviewModel(%0,%1, \
		Dialog_OpenPreviewModel(%0, #%1,
#endif

#define Dialog_Opened \
    Dialog_IsActive

#define Dialog_Close \
    Dialog_Hide

#if defined OnDialogPerformed
	forward OnDialogPerformed(playerid, dialog[], response, success);
#endif

forward @dialog_format(); @dialog_format() {
	format("", 1, "");
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

#if defined previewmodel_dialog_included
	stock Dialog_OpenPreviewModel(playerid, function[], caption[], models[], texts[][], button1[], button2[], size = sizeof(models)) {
		new ret = ShowPreviewModelDialog(playerid, easyDialog_ID, caption, models, texts, button1, button2, size);

		if (ret != 0) {
		    SetPVarString(playerid, "dialogid", function);
		}

		return ret;
	}
#endif

stock Dialog_IsActive(playerid) {
	return (GetPVarType(playerid, "dialogid") != PLAYER_VARTYPE_NONE);
}

stock Dialog_Hide(playerid) {
	DeletePVar(playerid, "dialogid");

	return ShowPlayerDialog(playerid, -1, DIALOG_STYLE_MSGBOX, " ", " ", " ", "");
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]) {
	new input[128];
	strcat(input, inputtext);

	for (new i = 0; input[i] != EOS; i++) {
	    if (input[i] != '%') {
			continue;
		}

		input[i] = '#';
	}

	if (dialogid == easyDialog_ID) {
	    new _dialogid[31];
		GetPVarString(playerid, "dialogid", _dialogid, sizeof(_dialogid));
    	DeletePVar(playerid, "dialogid");

		new func[31] = "dialog_";
		strcat(func, _dialogid);

		new bool:ret = true;
		#if defined OnDialogPerformed
		    ret = bool:OnDialogPerformed(playerid, _dialogid, response, (funcidx(func) != -1)) == 0);
		#endif

		if (ret) {
			CallLocalFunction(func, "iiis", playerid, response, listitem, (!input[0]) ? ("\1") : (input));
		}
	}

	#if defined ED_OnDialogResponse
	    ED_OnDialogResponse(playerid, dialogid, response, listitem, input);
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
