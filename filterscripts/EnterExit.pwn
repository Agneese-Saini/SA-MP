#include <a_samp>

#define MAX_TEXT_DRAW_ANIMATIONS MAX_PLAYERS

#include <textanims>
#include <streamer>

#define MOVEMENT_SPEED 1.0
#define MOVEMENT_HEIGHT 1.0

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
	{"My Interior"/*enter exit name*/, {0.0, 0.0, 0.0}/*position*/, 0/*interior*/, 0/*virtal world*/, {1.0, 1.0, 1.0}/*teleport position*/, 0/*teleport interior*/, 0/*teleport virtual world*/}
};
// END

new Text:enterExitTextDraw;
new playerEnterExitID[MAX_PLAYERS];

// THIS METHOD/FUNCTION IS CALLED EVERYTIME A PLAYER ENTERS AN ENTER-EXIT PICKUP
// YOU CAN FIGURE ENTER-EXIT ID FROM THE ARRAY (MAYBE USE DEFINE STATEMENTS FOR EASE)
forward OnPlayerEnterExit(playerid, enxid);
public OnPlayerEnterExit(playerid, enxid) {
	if (enxid == 0) {
		// first enter exit code here!
	}
	
	return 1;
}
// END

public OnFilterScriptInit() {
    enterExitTextDraw = TextDrawCreate(0.0, 0.0, "_");
	TextDrawTextSize(enterExitTextDraw, 640.0, 480.0);
	TextDrawLetterSize(enterExitTextDraw, 0.0, 50.0);
	TextDrawUseBox(enterExitTextDraw, 1);

	for (new i = 0; i < sizeof(ENTER_EXITS); i++) {
		ENTER_EXITS[i][ENTER_EXIT_OBJECTID] = CreateDynamicObject(1559,
		    ENTER_EXITS[i][ENTER_EXIT_POSITION][0], ENTER_EXITS[i][ENTER_EXIT_POSITION][1], ENTER_EXITS[i][ENTER_EXIT_POSITION][2],
		    0.0, 0.0, 0.0, ENTER_EXITS[i][ENTER_EXIT_INTERIORID], ENTER_EXITS[i][ENTER_EXIT_WORLDID]);

		ENTER_EXITS[i][ENTER_EXIT_PICKUPID] = CreateDynamicPickup(1, 1,
		    ENTER_EXITS[i][ENTER_EXIT_POSITION][0], ENTER_EXITS[i][ENTER_EXIT_POSITION][1], ENTER_EXITS[i][ENTER_EXIT_POSITION][2],
		    ENTER_EXITS[i][ENTER_EXIT_INTERIORID], ENTER_EXITS[i][ENTER_EXIT_WORLDID]);

        MoveDynamicObject(ENTER_EXITS[i][ENTER_EXIT_OBJECTID],
			ENTER_EXITS[i][ENTER_EXIT_POSITION][0], ENTER_EXITS[i][ENTER_EXIT_POSITION][1], (ENTER_EXITS[i][ENTER_EXIT_POSITION][2] + MOVEMENT_HEIGHT),
			MOVEMENT_SPEED, 360.0, 360.0, 360.0);

        ENTER_EXITS[i][ENTER_EXIT_MOVEDUP] = false;
	}

	return 1;
}

public OnFilterScriptExit() {
	for (new i = 0; i < sizeof(ENTER_EXITS); i++) {
		DestroyDynamicObject(ENTER_EXITS[i][ENTER_EXIT_OBJECTID]);
		DestroyDynamicPickup(ENTER_EXITS[i][ENTER_EXIT_PICKUPID]);
	}

	return 1;
}

public OnPlayerPickUpDynamicPickup(playerid, pickupid) {
	for (new i = 0; i < sizeof(ENTER_EXITS); i++) {
	    if (pickupid == ENTER_EXITS[i][ENTER_EXIT_PICKUPID]) {
	        if (bool:(OnPlayerEnterExit(playerid, i)) == true) {
         		TextAnim_SetData(playerid, E_ANIMATION_TEXTDRAW, enterExitTextDraw);
			    TextAnim_SetData(playerid, E_ANIMATION_FROM_COLOR, 0x00);
			    TextAnim_SetData(playerid, E_ANIMATION_TO_COLOR, 0xFF);
			    TextAnim_SetData(playerid, E_ANIMATION_PLAYERID, playerid);
			    TextAnim_Play(playerid, 2, 50);

			    TogglePlayerControllable(playerid, false);

			    playerEnterExitID[playerid] = i;
	        }
			break;
	    }
	}

	return 1;
}

public OnDynamicObjectMoved(objectid) {
	for (new i = 0; i < sizeof(ENTER_EXITS); i++) {
	    if (objectid == ENTER_EXITS[i][ENTER_EXIT_OBJECTID]) {
	        if (ENTER_EXITS[i][ENTER_EXIT_MOVEDUP] == true) {
	            ENTER_EXITS[i][ENTER_EXIT_MOVEDUP] = false;

		        MoveDynamicObject(ENTER_EXITS[i][ENTER_EXIT_OBJECTID],
					ENTER_EXITS[i][ENTER_EXIT_POSITION][0], ENTER_EXITS[i][ENTER_EXIT_POSITION][1], (ENTER_EXITS[i][ENTER_EXIT_POSITION][2]),
					MOVEMENT_SPEED, 0.0, 0.0, 0.0);
	        }
	        else {
	            ENTER_EXITS[i][ENTER_EXIT_MOVEDUP] = true;

		        MoveDynamicObject(ENTER_EXITS[i][ENTER_EXIT_OBJECTID],
					ENTER_EXITS[i][ENTER_EXIT_POSITION][0], ENTER_EXITS[i][ENTER_EXIT_POSITION][1], (ENTER_EXITS[i][ENTER_EXIT_POSITION][2] + MOVEMENT_HEIGHT),
					MOVEMENT_SPEED, 360.0, 360.0, 360.0);
	        }
	    }
	}

	return 1;
}

public OnTextDrawAnimated(index) {
	new toColor;
	TextAnim_GetData(index, E_ANIMATION_TO_COLOR, toColor);

	if (toColor == 0xFF) {
	    new idx = playerEnterExitID[index];

	    SetPlayerPos(index, ENTER_EXITS[idx][ENTER_EXIT_TELEPORT][0], ENTER_EXITS[idx][ENTER_EXIT_TELEPORT][1], ENTER_EXITS[idx][ENTER_EXIT_TELEPORT][2]);
		SetPlayerInterior(index, ENTER_EXITS[idx][ENTER_EXIT_TELEPORT_INTERIORID]);
		SetPlayerVirtualWorld(index, ENTER_EXITS[idx][ENTER_EXIT_TELEPORT_WORLDID]);
		
		GameTextForPlayer(index, ENTER_EXITS[idx][ENTER_EXIT_NAME], 5000, 6);

		TextAnim_SetData(index, E_ANIMATION_TEXTDRAW, enterExitTextDraw);
	    TextAnim_SetData(index, E_ANIMATION_FROM_COLOR, 0xFF);
	    TextAnim_SetData(index, E_ANIMATION_TO_COLOR, 0x00);
	    TextAnim_SetData(index, E_ANIMATION_PLAYERID, index);
	    TextAnim_Play(index, 2, 50);
	}
	else if (toColor == 0x00) {
		TogglePlayerControllable(index, true);

	    TextDrawHideForPlayer(index, enterExitTextDraw);
	}

	return 1;
}
