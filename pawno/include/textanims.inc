#if defined fader_included
	#endinput
#endif
#define fader_included

#if !defined MAX_TEXT_DRAW_ANIMATIONS
	#define MAX_TEXT_DRAW_ANIMATIONS \
	    16
#endif

#define RGBA(%0,%1,%2,%3,%4) \
	(((%1) = ((%0) >>> 24)),\
	((%2) = (((%0) >>> 16) & 0xFF)),\
	((%3) = (((%0) >>> 8) & 0xFF)),\
	((%4) = ((%0) & 0xFF)))

#define HEX(%0,%1,%2,%3,%4) \
	((%0) = ((%4) & 0xFF) | \
	(((%3) & 0xFF) << 8) | \
	(((%2) & 0xFF) << 16) | \
	((%1) << 24))

#if defined OnTextDrawAnimate
	forward OnTextDrawAnimate(index, E_ANIMATION:type, &update_rate);
#endif

#if defined OnTextDrawAnimated
	forward OnTextDrawAnimated(index);
#endif

enum E_ANIMATION {
	E_ANIMATION_TEXTDRAW,
	E_ANIMATION_PLAYERID,

	Float:E_ANIMATION_FROM_LETTERSIZE_X,
	Float:E_ANIMATION_FROM_LETTERSIZE_Y,
	Float:E_ANIMATION_TO_LETTERSIZE_X,
	Float:E_ANIMATION_TO_LETTERSIZE_Y,

	Float:E_ANIMATION_FROM_BOXSIZE_X,
	Float:E_ANIMATION_FROM_BOXSIZE_Y,
	Float:E_ANIMATION_TO_BOXSIZE_X,
	Float:E_ANIMATION_TO_BOXSIZE_Y,
	
    E_ANIMATION_FROM_COLOR,
    E_ANIMATION_TO_COLOR,

    E_ANIMATION_FROM_BOXCOLOR,
    E_ANIMATION_TO_BOXCOLOR,

    E_ANIMATION_FROM_BGCOLOR,
    E_ANIMATION_TO_BGCOLOR,

    E_ANIMATION_FROM_OUTLINESIZE,
    E_ANIMATION_TO_OUTLINESIZE,

    E_ANIMATION_FROM_SHADOWSIZE,
    E_ANIMATION_TO_SHADOWSIZE,

	Float:E_ANIMATION_FROM_ROT_X,
	Float:E_ANIMATION_FROM_ROT_Y,
	Float:E_ANIMATION_FROM_ROT_Z,
	Float:E_ANIMATION_FROM_ROT_ZOOM,
	Float:E_ANIMATION_TO_ROT_X,
	Float:E_ANIMATION_TO_ROT_Y,
	Float:E_ANIMATION_TO_ROT_Z,
	Float:E_ANIMATION_TO_ROT_ZOOM
};

static textDrawAnimation[MAX_TEXT_DRAW_ANIMATIONS][E_ANIMATION];
static textDrawAnimationTimer[MAX_TEXT_DRAW_ANIMATIONS] = {
	-1, ...
};

stock TextAnim_SetData(index, E_ANIMATION:type, {Float, Text, _}:...) {
	if (index < 0 || index >= MAX_TEXT_DRAW_ANIMATIONS) {
		return 0;
	}
	
	if (textDrawAnimationTimer[index] != -1) {
		printf("[textdrawanims.inc] - Error: You cannot change textdraw animation settings while animation is being played!");
		return 0;
	}

	if (numargs() < 3) {
		return 0;
	}

	textDrawAnimation[index][type] = getarg(2);
	return 1;
}

stock TextAnim_GetData(index, E_ANIMATION:type, {Float, Text, _}:...) {
	if (index < 0 || index >= MAX_TEXT_DRAW_ANIMATIONS) {
		return 0;
	}

	if (numargs() < 3) {
		return 0;
	}

	setarg(2, _, textDrawAnimation[index][type]);
	return 1;
}

stock TextAnim_Play(index, update_rate, timer_interval) {
	if (index < 0 || index >= MAX_TEXT_DRAW_ANIMATIONS) {
		return 0;
	}

	if (update_rate <= 0) {
		return 0;
	}

	if (timer_interval <= 0) {
		return 0;
	}

	if (textDrawAnimation[index][E_ANIMATION_TEXTDRAW] < 0 || textDrawAnimation[index][E_ANIMATION_TEXTDRAW] >= MAX_TEXT_DRAWS) {
		return 0;
	}

	if (textDrawAnimation[index][E_ANIMATION_PLAYERID] != -1 && (textDrawAnimation[index][E_ANIMATION_PLAYERID] < 0 || textDrawAnimation[index][E_ANIMATION_PLAYERID] >= MAX_PLAYERS)) {
		return 0;
	}

	KillTimer(textDrawAnimationTimer[index]);
	textDrawAnimationTimer[index] = SetTimerEx("__OnTextDrawAnimate", timer_interval, true, "ii", index, update_rate);
	return 1;
}

stock TextAnim_Stop(index) {
	if (index < 0 || index >= MAX_TEXT_DRAW_ANIMATIONS) {
		return 0;
	}

	if (textDrawAnimation[index][E_ANIMATION_TEXTDRAW] < 0 || textDrawAnimation[index][E_ANIMATION_TEXTDRAW] >= MAX_TEXT_DRAWS) {
		return 0;
	}

	KillTimer(textDrawAnimationTimer[index]);
	textDrawAnimationTimer[index] = -1;
	
	for (new E_ANIMATION:i; i < E_ANIMATION; i++) {
        textDrawAnimation[index][i] = 0;
	}
	textDrawAnimation[index][E_ANIMATION_TEXTDRAW] = INVALID_TEXT_DRAW;
	textDrawAnimation[index][E_ANIMATION_PLAYERID] = -1;
	return 1;
}

forward __OnTextDrawAnimate(index, update_rate);
public __OnTextDrawAnimate(index, update_rate) {
	new Text:text = Text:textDrawAnimation[index][E_ANIMATION_TEXTDRAW];
	new Float:newsize[2];
	new Float:finalsize[2];
	new newcolor[4];
	new finalcolor[4];
	new Float:newrot[4];
	new Float:finalrot[4];
	new bool:savechanges;

	newsize[0] = textDrawAnimation[index][E_ANIMATION_FROM_LETTERSIZE_X];
	newsize[1] = textDrawAnimation[index][E_ANIMATION_FROM_LETTERSIZE_Y];
	finalsize[0] = textDrawAnimation[index][E_ANIMATION_TO_LETTERSIZE_X];
	finalsize[1] = textDrawAnimation[index][E_ANIMATION_TO_LETTERSIZE_Y];
	for (new i; i < sizeof newsize; i++) {
	    if (newsize[i] != finalsize[i]) {
	    	//
     		savechanges = true;
		   	//
		   
			#if defined OnTextDrawAnimate
			    OnTextDrawAnimate(index, ((i == 0) ? (E_ANIMATION_FROM_LETTERSIZE_X) : (E_ANIMATION_FROM_LETTERSIZE_Y)), update_rate);
			#endif

			if (newsize[i] < finalsize[i]) {
				newsize[i] += Float:update_rate;

				if (newsize[i] >= finalsize[i]) {
					newsize[i] = finalsize[i];
				}
			}
			else if (newsize[i] > finalsize[i]) {
				newsize[i] -= Float:update_rate;

				if (newsize[i] <= finalsize[i]) {
					newsize[i] = finalsize[i];
				}
			}

			TextDrawLetterSize(text, newsize[0], newsize[1]);
		}
	}
	textDrawAnimation[index][E_ANIMATION_FROM_LETTERSIZE_X] = newsize[0];
	textDrawAnimation[index][E_ANIMATION_FROM_LETTERSIZE_Y] = newsize[1];
	

	newsize[0] = textDrawAnimation[index][E_ANIMATION_FROM_BOXSIZE_X];
	newsize[1] = textDrawAnimation[index][E_ANIMATION_FROM_BOXSIZE_Y];
	finalsize[0] = textDrawAnimation[index][E_ANIMATION_TO_BOXSIZE_X];
	finalsize[1] = textDrawAnimation[index][E_ANIMATION_TO_BOXSIZE_Y];
	for (new i; i < sizeof newsize; i++) {
	    if (newsize[i] != finalsize[i]) {
	    	//
     		savechanges = true;
		   	//

			#if defined OnTextDrawAnimate
			    OnTextDrawAnimate(index, ((i == 0) ? (E_ANIMATION_FROM_BOXSIZE_X) : (E_ANIMATION_FROM_BOXSIZE_Y)), update_rate);
			#endif
	        
			if (newsize[i] < finalsize[i]) {
				newsize[i] += Float:update_rate;

				if (newsize[i] >= finalsize[i]) {
					newsize[i] = finalsize[i];
				}
			}
			else if (newsize[i] > finalsize[i]) {
				newsize[i] -= Float:update_rate;

				if (newsize[i] <= finalsize[i]) {
					newsize[i] = finalsize[i];
				}
			}

			TextDrawTextSize(text, newsize[0], newsize[1]);
		}
	}
	textDrawAnimation[index][E_ANIMATION_FROM_BOXSIZE_X] = newsize[0];
	textDrawAnimation[index][E_ANIMATION_FROM_BOXSIZE_Y] = newsize[1];
	
	if (textDrawAnimation[index][E_ANIMATION_FROM_COLOR] != textDrawAnimation[index][E_ANIMATION_TO_COLOR]) {
		//
		savechanges = true;
 		//
		   	
		RGBA(textDrawAnimation[index][E_ANIMATION_FROM_COLOR], newcolor[0], newcolor[1], newcolor[2], newcolor[3]);
		RGBA(textDrawAnimation[index][E_ANIMATION_TO_COLOR], finalcolor[0], finalcolor[1], finalcolor[2], finalcolor[3]);

		#if defined OnTextDrawAnimate
  			OnTextDrawAnimate(index, E_ANIMATION_FROM_COLOR, update_rate);
		#endif

		for (new i; i < sizeof newcolor; i++) {
			if (newcolor[i] > finalcolor[i]) {
			    newcolor[i] -= update_rate;

			    if (newcolor[i] <= finalcolor[i]) {
			    	newcolor[i] = finalcolor[i];
			    }
			}
			else if (newcolor[i] < finalcolor[i]) {
			    newcolor[i] += update_rate;

			    if (newcolor[i] >= finalcolor[i]) {
			    	newcolor[i] = finalcolor[i];
			    }
			}
		}

		HEX(textDrawAnimation[index][E_ANIMATION_FROM_COLOR], newcolor[0], newcolor[1], newcolor[2], newcolor[3]);
		TextDrawColor(text, textDrawAnimation[index][E_ANIMATION_FROM_COLOR]);
	}
	
	if (textDrawAnimation[index][E_ANIMATION_FROM_BOXCOLOR] != textDrawAnimation[index][E_ANIMATION_TO_BOXCOLOR]) {
		//
		savechanges = true;
 		//

		RGBA(textDrawAnimation[index][E_ANIMATION_FROM_BOXCOLOR], newcolor[0], newcolor[1], newcolor[2], newcolor[3]);
		RGBA(textDrawAnimation[index][E_ANIMATION_TO_BOXCOLOR], finalcolor[0], finalcolor[1], finalcolor[2], finalcolor[3]);

		#if defined OnTextDrawAnimate
  			OnTextDrawAnimate(index, E_ANIMATION_FROM_BOXCOLOR, update_rate);
		#endif
		
		for (new i; i < sizeof newcolor; i++) {
			if (newcolor[i] > finalcolor[i]) {
			    newcolor[i] -= update_rate;

			    if (newcolor[i] <= finalcolor[i]) {
			    	newcolor[i] = finalcolor[i];
			    }
			}
			else if (newcolor[i] < finalcolor[i]) {
			    newcolor[i] += update_rate;

			    if (newcolor[i] >= finalcolor[i]) {
			    	newcolor[i] = finalcolor[i];
			    }
			}
		}

		HEX(textDrawAnimation[index][E_ANIMATION_FROM_BOXCOLOR], newcolor[0], newcolor[1], newcolor[2], newcolor[3]);
		TextDrawBoxColor(text, textDrawAnimation[index][E_ANIMATION_FROM_BOXCOLOR]);
	}
	
	if (textDrawAnimation[index][E_ANIMATION_FROM_BGCOLOR] != textDrawAnimation[index][E_ANIMATION_TO_BGCOLOR]) {
		//
		savechanges = true;
 		//

		RGBA(textDrawAnimation[index][E_ANIMATION_FROM_BGCOLOR], newcolor[0], newcolor[1], newcolor[2], newcolor[3]);
		RGBA(textDrawAnimation[index][E_ANIMATION_TO_BGCOLOR], finalcolor[0], finalcolor[1], finalcolor[2], finalcolor[3]);

		#if defined OnTextDrawAnimate
  			OnTextDrawAnimate(index, E_ANIMATION_FROM_BGCOLOR, update_rate);
		#endif
		
		for (new i; i < sizeof newcolor; i++) {
			if (newcolor[i] > finalcolor[i]) {
			    newcolor[i] -= update_rate;

			    if (newcolor[i] <= finalcolor[i]) {
			    	newcolor[i] = finalcolor[i];
			    }
			}
			else if (newcolor[i] < finalcolor[i]) {
			    newcolor[i] += update_rate;

			    if (newcolor[i] >= finalcolor[i]) {
			    	newcolor[i] = finalcolor[i];
			    }
			}
		}

		HEX(textDrawAnimation[index][E_ANIMATION_FROM_BGCOLOR], newcolor[0], newcolor[1], newcolor[2], newcolor[3]);
		TextDrawBackgroundColor(text, textDrawAnimation[index][E_ANIMATION_FROM_BGCOLOR]);
	}

	if (textDrawAnimation[index][E_ANIMATION_FROM_OUTLINESIZE] != textDrawAnimation[index][E_ANIMATION_TO_OUTLINESIZE]) {
		//
		savechanges = true;
 		//

		#if defined OnTextDrawAnimate
  			OnTextDrawAnimate(index, E_ANIMATION_FROM_OUTLINESIZE, update_rate);
		#endif
		
	    if (textDrawAnimation[index][E_ANIMATION_FROM_OUTLINESIZE] < textDrawAnimation[index][E_ANIMATION_TO_OUTLINESIZE]) {
        	textDrawAnimation[index][E_ANIMATION_FROM_OUTLINESIZE] += update_rate;

			if (textDrawAnimation[index][E_ANIMATION_FROM_OUTLINESIZE] >= textDrawAnimation[index][E_ANIMATION_TO_OUTLINESIZE]) {
	        	textDrawAnimation[index][E_ANIMATION_FROM_OUTLINESIZE] = textDrawAnimation[index][E_ANIMATION_TO_OUTLINESIZE];
			}
		}

		TextDrawSetOutline(text, textDrawAnimation[index][E_ANIMATION_FROM_OUTLINESIZE]);
	}

	if (textDrawAnimation[index][E_ANIMATION_FROM_SHADOWSIZE] != textDrawAnimation[index][E_ANIMATION_TO_SHADOWSIZE]) {
		//
		savechanges = true;
 		//

	    #if defined OnTextDrawAnimate
  			OnTextDrawAnimate(index, E_ANIMATION_FROM_SHADOWSIZE, update_rate);
		#endif

		if (textDrawAnimation[index][E_ANIMATION_FROM_SHADOWSIZE] < textDrawAnimation[index][E_ANIMATION_TO_SHADOWSIZE]) {
        	textDrawAnimation[index][E_ANIMATION_FROM_SHADOWSIZE] += update_rate;

			if (textDrawAnimation[index][E_ANIMATION_FROM_SHADOWSIZE] >= textDrawAnimation[index][E_ANIMATION_TO_SHADOWSIZE]) {
	        	textDrawAnimation[index][E_ANIMATION_FROM_SHADOWSIZE] = textDrawAnimation[index][E_ANIMATION_TO_SHADOWSIZE];
			}
		}

		TextDrawSetShadow(text, textDrawAnimation[index][E_ANIMATION_FROM_SHADOWSIZE]);
	}

	if (textDrawAnimation[index][E_ANIMATION_FROM_SHADOWSIZE] != textDrawAnimation[index][E_ANIMATION_TO_SHADOWSIZE]) {
		//
		savechanges = true;
 		//

	    #if defined OnTextDrawAnimate
  			OnTextDrawAnimate(index, E_ANIMATION_FROM_SHADOWSIZE, update_rate);
		#endif

		if (textDrawAnimation[index][E_ANIMATION_FROM_SHADOWSIZE] < textDrawAnimation[index][E_ANIMATION_TO_SHADOWSIZE]) {
        	textDrawAnimation[index][E_ANIMATION_FROM_SHADOWSIZE] += update_rate;

			if (textDrawAnimation[index][E_ANIMATION_FROM_SHADOWSIZE] >= textDrawAnimation[index][E_ANIMATION_TO_SHADOWSIZE]) {
	        	textDrawAnimation[index][E_ANIMATION_FROM_SHADOWSIZE] = textDrawAnimation[index][E_ANIMATION_TO_SHADOWSIZE];
			}
		}

		TextDrawSetShadow(text, textDrawAnimation[index][E_ANIMATION_FROM_SHADOWSIZE]);
	}

	newrot[0] = textDrawAnimation[index][E_ANIMATION_FROM_ROT_X];
	newrot[1] = textDrawAnimation[index][E_ANIMATION_FROM_ROT_Y];
	newrot[2] = textDrawAnimation[index][E_ANIMATION_FROM_ROT_Z];
	newrot[3] = textDrawAnimation[index][E_ANIMATION_FROM_ROT_ZOOM];
	finalrot[0] = textDrawAnimation[index][E_ANIMATION_TO_ROT_X];
	finalrot[1] = textDrawAnimation[index][E_ANIMATION_TO_ROT_Y];
	finalrot[2] = textDrawAnimation[index][E_ANIMATION_TO_ROT_Z];
	finalrot[3] = textDrawAnimation[index][E_ANIMATION_TO_ROT_ZOOM];
	for (new i; i < sizeof newrot; i++) {
	    if (newrot[i] != finalrot[i]) {
			//
			savechanges = true;
	 		//

			switch (i) {
			    case 0: {
					#if defined OnTextDrawAnimate
					    OnTextDrawAnimate(index, E_ANIMATION_FROM_ROT_X, update_rate);
					#endif
				}

			    case 1: {
					#if defined OnTextDrawAnimate
					    OnTextDrawAnimate(index, E_ANIMATION_FROM_ROT_Y, update_rate);
					#endif
				}

			    case 2: {
					#if defined OnTextDrawAnimate
					    OnTextDrawAnimate(index, E_ANIMATION_FROM_ROT_Z, update_rate);
					#endif
				}

			    case 3: {
					#if defined OnTextDrawAnimate
					    OnTextDrawAnimate(index, E_ANIMATION_FROM_ROT_ZOOM, update_rate);
					#endif
				}
			}
			
			if (newrot[i] < finalrot[i]) {
				newrot[i] += Float:update_rate;

				if (newrot[i] >= finalrot[i]) {
					newrot[i] = finalrot[i];
				}
			}
			else if (newrot[i] > finalrot[i]) {
				newrot[i] -= Float:update_rate;

				if (newrot[i] <= finalrot[i]) {
					newrot[i] = finalrot[i];
				}
			}
			
			TextDrawSetPreviewRot(text, newrot[0], newrot[1], newrot[2], newrot[3]);
		}
	}
	textDrawAnimation[index][E_ANIMATION_FROM_ROT_X] = newrot[0];
	textDrawAnimation[index][E_ANIMATION_FROM_ROT_Y] = newrot[1];
	textDrawAnimation[index][E_ANIMATION_FROM_ROT_Z] = newrot[2];
	textDrawAnimation[index][E_ANIMATION_FROM_ROT_ZOOM] = newrot[3];
	
	if (!savechanges) {
	    #if defined OnTextDrawAnimated
        	OnTextDrawAnimated(index);
        #endif
        
        TextAnim_Stop(index);
        return;
	}
	
	if (textDrawAnimation[index][E_ANIMATION_PLAYERID] == -1) {
		TextDrawShowForAll(text);
	}
	else {
        TextDrawShowForPlayer(textDrawAnimation[index][E_ANIMATION_PLAYERID], text);
	}
}
