#include <a_samp>
#include <streamer>
#define MAX_TEXT_DRAW_FADES 1
#include <fader>

#define MOVEMENT_SPEED 0.75
#define MOVEMENT_HEIGHT 0.65

#define FADER_UPDATE_RATE 10
#define FADER_TIMER_INTERVAL 50

enum E_ENTER_EXIT {
	ENTER_EXIT_NAME[64],
	Float:ENTER_EXIT_POSITION[3],
    ENTER_EXIT_INTERIORID,
    ENTER_EXIT_WORLDID,
    Float:ENTER_EXIT_TELEPORT[3],
    ENTER_EXIT_TELEPORT_INTERIORID,
    ENTER_EXIT_TELEPORT_WORLDID,
    ENTER_EXIT_OBJECTID,
    ENTER_EXIT_PICKUPID,
    bool:ENTER_EXIT_MOVEDUP
};

// ADD YOUR ENTER EXIT COORDINATES HERE
// THE FORMAT OF ADDING IS GIVEN IN ENUM ABOVE (ONLY ADD THE COORDINATES AND NAME(SHOWN WHEN PLAYER IS TELEPORTED)) - SAMPLE GIVEN BELOW
new const ENTER_EXITS[][E_ENTER_EXIT] = {
	{"My Interior"/*enter exit name*/, {0.0, 0.0, 0.0}/*pickup position*/, 0/*pickup interior*/, 0/*pickup virtal world*/, {1.0, 1.0, 1.0}/*teleport position*/, 0/*teleport interior*/, 0/*teleport virtual world*/}
};
// END

new Text:enterExitTextDraw;
new playerEnterExitID[MAX_PLAYERS];

// THIS METHOD/FUNCTION IS CALLED EVERYTIME A PLAYER ENTERS AN ENTER-EXIT PICKUP
// YOU CAN FIGURE ENTER-EXIT ID FROM THE ARRAY (MAYBE USE DEFINE STATEMENTS FOR EASE)
forward OnPlayerEnterExit(playerid, enxid);
public OnPlayerEnterExit(playerid, enxid) {
	switch (enxid) {
	    case 0: {
			// this is "My Interior"
		}
	}

	return 1; // returning 0 will not allow player to use/teleport from enter-exit pickup
}
// END

public OnFilterScriptInit() {
    enterExitTextDraw = TextDrawCreate(0.0, 0.0, "_");
	TextDrawTextSize(enterExitTextDraw, 640.0, 480.0);
	TextDrawLetterSize(enterExitTextDraw, 0.0, 50.0);
	TextDrawUseBox(enterExitTextDraw, 1);

	for (new i = 0; i < sizeof(ENTER_EXITS); i++) {
		ENTER_EXITS[i][ENTER_EXIT_OBJECTID] = CreateObject(1559,
		    ENTER_EXITS[i][ENTER_EXIT_POSITION][0], ENTER_EXITS[i][ENTER_EXIT_POSITION][1], ENTER_EXITS[i][ENTER_EXIT_POSITION][2],
		    0.0, 0.0, 0.0
		);

		ENTER_EXITS[i][ENTER_EXIT_PICKUPID] = CreateDynamicPickup(1, 1,
		    ENTER_EXITS[i][ENTER_EXIT_POSITION][0], ENTER_EXITS[i][ENTER_EXIT_POSITION][1], ENTER_EXITS[i][ENTER_EXIT_POSITION][2],
		    ENTER_EXITS[i][ENTER_EXIT_WORLDID], ENTER_EXITS[i][ENTER_EXIT_INTERIORID]
		);

        MoveObject(ENTER_EXITS[i][ENTER_EXIT_OBJECTID],
			ENTER_EXITS[i][ENTER_EXIT_POSITION][0], ENTER_EXITS[i][ENTER_EXIT_POSITION][1], (ENTER_EXITS[i][ENTER_EXIT_POSITION][2] + MOVEMENT_HEIGHT),
			MOVEMENT_SPEED, 360.0, 360.0, 360.0
		);

        ENTER_EXITS[i][ENTER_EXIT_MOVEDUP] = false;
	}

	return 1;
}

public OnFilterScriptExit() {
	for (new i = 0; i < sizeof(ENTER_EXITS); i++) {
		DestroyObject(ENTER_EXITS[i][ENTER_EXIT_OBJECTID]);
		DestroyPickup(ENTER_EXITS[i][ENTER_EXIT_PICKUPID]);
	}

	return 1;
}

public OnPlayerConnect(playerid) {
    playerEnterExitID[playerid] = -1;
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid) {
	if (playerEnterExitID[playerid] == -1) {
		for (new i = 0; i < sizeof(ENTER_EXITS); i++) {
		    if (pickupid == ENTER_EXITS[i][ENTER_EXIT_PICKUPID]) {
		        if (bool:(OnPlayerEnterExit(playerid, i)) == true) {
		            TextDrawBoxFadeForPlayer(playerid, enterExitTextDraw, 0x00000000, 0x000000FF, FADER_UPDATE_RATE, FADER_TIMER_INTERVAL);

				    TogglePlayerControllable(playerid, false);

				    playerEnterExitID[playerid] = i;
		        }
				break;
		    }
		}
	}
	
	return 1;
}

public OnObjectMoved(objectid) {
	for (new i = 0; i < sizeof(ENTER_EXITS); i++) {
	    if (objectid == ENTER_EXITS[i][ENTER_EXIT_OBJECTID]) {
	        if (ENTER_EXITS[i][ENTER_EXIT_MOVEDUP] == true) {
	            ENTER_EXITS[i][ENTER_EXIT_MOVEDUP] = false;

		        MoveObject(ENTER_EXITS[i][ENTER_EXIT_OBJECTID],
					ENTER_EXITS[i][ENTER_EXIT_POSITION][0], ENTER_EXITS[i][ENTER_EXIT_POSITION][1], (ENTER_EXITS[i][ENTER_EXIT_POSITION][2]),
					MOVEMENT_SPEED, 0.0, 0.0, 0.0);
	        }
	        else {
	            ENTER_EXITS[i][ENTER_EXIT_MOVEDUP] = true;

		        MoveObject(ENTER_EXITS[i][ENTER_EXIT_OBJECTID],
					ENTER_EXITS[i][ENTER_EXIT_POSITION][0], ENTER_EXITS[i][ENTER_EXIT_POSITION][1], (ENTER_EXITS[i][ENTER_EXIT_POSITION][2] + MOVEMENT_HEIGHT),
					MOVEMENT_SPEED, 360.0, 360.0, 360.0);
	        }
	    }
	}

	return 1;
}

public OnTextDrawFaded(playerid, Text:text, type, from_color, to_color) {
	if (type == TEXTDRAW_FADE_BOX) {
		if (to_color == 0x000000FF) {
		    new idx = playerEnterExitID[playerid];

		    SetPlayerPos(playerid, ENTER_EXITS[idx][ENTER_EXIT_TELEPORT][0], ENTER_EXITS[idx][ENTER_EXIT_TELEPORT][1], ENTER_EXITS[idx][ENTER_EXIT_TELEPORT][2]);
			SetPlayerInterior(playerid, ENTER_EXITS[idx][ENTER_EXIT_TELEPORT_INTERIORID]);
			SetPlayerVirtualWorld(playerid, ENTER_EXITS[idx][ENTER_EXIT_TELEPORT_WORLDID]);

			GameTextForPlayer(playerid, ENTER_EXITS[idx][ENTER_EXIT_NAME], 5000, 6);

			TextDrawBoxFadeForPlayer(playerid, enterExitTextDraw, 0x000000FF, 0x00000000, FADER_UPDATE_RATE, FADER_TIMER_INTERVAL);
		}
		else if (to_color == 0x00000000) {
			TogglePlayerControllable(playerid, true);

		    TextDrawHideForPlayer(playerid, enterExitTextDraw);

		    playerEnterExitID[playerid] = -1;
		}
	}
	
	return 1;
}
