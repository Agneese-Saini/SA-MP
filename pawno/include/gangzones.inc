#if defined gangzones_included
	#endinput
#endif
#define gangzones_included

/**************************************/
// gangzones.inc - v2.2 - Last Updated: 28 April, 2018 - By Gammix & beijind
/***************************************/

#if !defined GANGZONE_DEFAULT_BORDER_SIZE
	#define GANGZONE_DEFAULT_BORDER_SIZE 2.0
#endif

#if !defined GANGZONE_DEFAULT_NUMBER_SIZE
	#define GANGZONE_DEFAULT_NUMBER_SIZE 1.0
#endif

#if !defined GANGZONE_DEFAULT_BORDER_COLOR
	#define GANGZONE_DEFAULT_BORDER_COLOR 0xAA
#endif

#if !defined GANGZONE_DEFAULT_NUMBER_COLOR
	#define GANGZONE_DEFAULT_NUMBER_COLOR 0xAA
#endif

static GangZone@MainZone[MAX_GANG_ZONES] = {-1, ...};
static GangZone@Border[MAX_GANG_ZONES][4];
static GangZone@Number1[MAX_GANG_ZONES][5];
static GangZone@Number2[MAX_GANG_ZONES][5];

static GangZone_CreateBorder(dest[], Float:minx, Float:miny, Float:maxx, Float:maxy, Float:bordersize) {
    dest[0] = GangZoneCreate(minx - bordersize, miny + bordersize, minx + bordersize, maxy - bordersize);
	dest[1] = GangZoneCreate(minx - bordersize, maxy - bordersize, maxx + bordersize, maxy + bordersize);
	dest[2] = GangZoneCreate(maxx - bordersize, miny + bordersize, maxx + bordersize, maxy - bordersize);
	dest[3] = GangZoneCreate(minx - bordersize, miny - bordersize, maxx + bordersize, miny + bordersize);

	for (new i = 0; i < 4; i++) {
		if (dest[i] == -1) {

			for (i = 0; i < 4; i++) {
				GangZoneDestroy(dest[i]);
				dest[i] = -1;
			}
			
		    return 0;
		}
	}

	return 4;
}

static GangZone_CreateNumber(dest[], number, Float:centpos_x, Float:centpos_y, Float:numbersize) {
    if (number < '0' || number > '9') {
        return 0;
	}

	static const Float:LETTER_STROKE = 3.5;
	static const Float:LETTER_XSQUASH = 0.7;

	new ret = 0;

	switch (number) {
	  	case '0': {
			new Float:stroke = (LETTER_STROKE * numbersize);
			new Float:distance_x = (20.0 * numbersize * LETTER_XSQUASH);
			new Float:distance_y = (20.0 * numbersize);

        	dest[0] = GangZoneCreate((centpos_x - distance_x - stroke), (centpos_y - distance_y - stroke), (centpos_x + distance_x + stroke), (centpos_y - distance_y + stroke)); // Down of 0
			dest[1] = GangZoneCreate((centpos_x - distance_x - stroke), (centpos_y + distance_y + stroke), (centpos_x + distance_x + stroke), (centpos_y + distance_y - stroke)); // Up of 0
       	 	dest[2] = GangZoneCreate((centpos_x - distance_x - stroke), (centpos_y - distance_y + stroke), (centpos_x - distance_x + stroke), (centpos_y + distance_y - stroke)); // Left of 0
        	dest[3] = GangZoneCreate((centpos_x + distance_x - stroke), (centpos_y - distance_y + stroke), (centpos_x + distance_x + stroke), (centpos_y + distance_y - stroke)); // Right of 0

			ret = 4;
	  	}

	  	case '1': {
			new Float:stroke = (LETTER_STROKE * numbersize);
			new Float:distance_y = (20.0 * numbersize);

			dest[0] = GangZoneCreate((centpos_x - stroke), (centpos_y - distance_y - stroke),(centpos_x + stroke), (centpos_y + distance_y + stroke));

			ret = 1;
	  	}

	  	case '2': {
			new Float:stroke = (LETTER_STROKE * numbersize);
			new Float:distance_y = (20.0 * numbersize);
			new Float:distance_x = (25.0 * numbersize * LETTER_XSQUASH);

			dest[0] = GangZoneCreate((centpos_x - distance_x), 				(centpos_y + distance_y - stroke), 	(centpos_x + distance_x),				(centpos_y + distance_y + stroke)); // Up
			dest[1] = GangZoneCreate((centpos_x + distance_x - (2 * stroke)),(centpos_y + stroke), 				(centpos_x + distance_x),				(centpos_y + distance_y - stroke)); // Right Stick
			dest[2] = GangZoneCreate((centpos_x - distance_x), 				(centpos_y - stroke), 				(centpos_x + distance_x), 				(centpos_y + stroke)); // Middle
			dest[3] = GangZoneCreate((centpos_x - distance_x), 				(centpos_y - distance_y + stroke),	(centpos_x - distance_x + (2 * stroke)),(centpos_y - stroke)); // Left Stick
	        dest[4] = GangZoneCreate((centpos_x - distance_x), 				(centpos_y - distance_y - stroke),	(centpos_x + distance_x),				(centpos_y - distance_y + stroke)); // Down

			ret = 5;
	  	}

	  	case '3': {
			new Float:stroke = (LETTER_STROKE * numbersize);
			new Float:distance_y = (20.0 * numbersize);
			new Float:distance_x = (25.0 * numbersize * LETTER_XSQUASH);

			dest[0] = GangZoneCreate((centpos_x - distance_x), (centpos_y + distance_y - stroke),	(centpos_x + distance_x),				(centpos_y + distance_y + stroke)); // Up
			dest[1] = GangZoneCreate((centpos_x + distance_x), (centpos_y - distance_y - stroke),	(centpos_x + distance_x + (2 * stroke)),(centpos_y + distance_y + stroke)); // Stick
			dest[2] = GangZoneCreate((centpos_x - distance_x), (centpos_y - stroke),				(centpos_x + distance_x),				(centpos_y + stroke)); // Middle
			dest[3] = GangZoneCreate((centpos_x - distance_x), (centpos_y - distance_y - stroke),	(centpos_x + distance_x),				(centpos_y - distance_y + stroke)); // Down

			ret = 4;
	  	}

	  	case '4': {
	  		new Float:stroke = (LETTER_STROKE * numbersize);
			new Float:distance_y = (20.0 * numbersize);
			new Float:distance_x = (25.0 * numbersize * LETTER_XSQUASH);

			dest[0] = GangZoneCreate((centpos_x - distance_x), (centpos_y + distance_y),	(centpos_x - distance_x + (2 * stroke)),	(centpos_y + stroke)); // Left
			dest[1] = GangZoneCreate((centpos_x - distance_x), (centpos_y - stroke),		(centpos_x + distance_x),					(centpos_y + stroke));// Middle
			dest[2] = GangZoneCreate((centpos_x + distance_x), (centpos_y - distance_y),	(centpos_x + distance_x + (2 * stroke)),	(centpos_y + distance_y)); // Stick

			ret = 3;
	  	}

	  	case '5': {
	  		new Float:stroke = (LETTER_STROKE * numbersize);
			new Float:distance_y = (20.0 * numbersize);
			new Float:distance_x = (25.0 * numbersize * LETTER_XSQUASH);

			dest[0] = GangZoneCreate((centpos_x - distance_x - stroke), (centpos_y + distance_y - stroke),	(centpos_x + distance_x + stroke), (centpos_y + distance_y + stroke)); // Up
			dest[1] = GangZoneCreate((centpos_x - distance_x - stroke), (centpos_y + stroke),				(centpos_x - distance_x + stroke), (centpos_y + distance_y - stroke)); // Left Stick
			dest[2] = GangZoneCreate((centpos_x - distance_x - stroke), (centpos_y - stroke),				(centpos_x + distance_x + stroke), (centpos_y + stroke)); // Middle
			dest[3] = GangZoneCreate((centpos_x + distance_x - stroke), (centpos_y - stroke),				(centpos_x + distance_x + stroke), (centpos_y - distance_y + stroke)); // Right Stick
			dest[4] = GangZoneCreate((centpos_x - distance_x - stroke), (centpos_y - distance_y - stroke),	(centpos_x + distance_x + stroke), (centpos_y - distance_y + stroke)); // Down

			ret = 5;
	  	}

	  	case '6': {
	  		new Float:stroke = (LETTER_STROKE * numbersize);
			new Float:distance_y = (20.0 * numbersize);
			new Float:distance_x = (25.0 * numbersize * LETTER_XSQUASH);

			dest[0] = GangZoneCreate((centpos_x - distance_x),					(centpos_y + distance_y - stroke),	(centpos_x + distance_x), (centpos_y + distance_y + stroke)); // Up
			dest[1] = GangZoneCreate((centpos_x - distance_x - (2 * stroke)),	(centpos_y + distance_y + stroke),	(centpos_x - distance_x), (centpos_y - distance_y - stroke)); // Main Left Stick
			dest[2] = GangZoneCreate((centpos_x - distance_x),					(centpos_y - stroke),				(centpos_x + distance_x), (centpos_y + stroke)); // Middle
			dest[3] = GangZoneCreate((centpos_x - distance_x),					(centpos_y - distance_y - stroke),	(centpos_x + distance_x), (centpos_y - distance_y + stroke)); // Down
			dest[4] = GangZoneCreate((centpos_x + distance_x - (2 * stroke)),	(centpos_y - distance_y + stroke),	(centpos_x + distance_x), (centpos_y - stroke)); // Right small stick

			ret = 5;
	  	}

	  	case '7': {
			new Float:stroke = (LETTER_STROKE * numbersize);
			new Float:distance_y = (20.0 * numbersize);
			new Float:distance_x = (15.0 * numbersize * LETTER_XSQUASH);

			dest[0] = GangZoneCreate((centpos_x + distance_x - stroke),		(centpos_y - distance_y - stroke), (centpos_x + distance_x + stroke), (centpos_y + distance_y - stroke)); // Stick of 7
			dest[1] = GangZoneCreate((centpos_x - distance_x - distance_x),	(centpos_y + distance_y - stroke), (centpos_x + distance_x + stroke), (centpos_y + distance_y + stroke)); // Up of 7

			ret = 2;
	  	}

	  	case '8': {
	  		new Float:stroke = (LETTER_STROKE * numbersize);
			new Float:distance_y = (20.0 * numbersize);
			new Float:distance_x = (25.0 * numbersize * LETTER_XSQUASH);

			dest[0] = GangZoneCreate((centpos_x - distance_x - stroke), (centpos_y + distance_y + stroke), 	(centpos_x - distance_x + stroke), (centpos_y - distance_y - stroke)); // Left stick
			dest[1] = GangZoneCreate((centpos_x + distance_x - stroke), (centpos_y + distance_y + stroke), 	(centpos_x + distance_x + stroke), (centpos_y - distance_y - stroke)); // Right stick
			dest[2] = GangZoneCreate((centpos_x - distance_x + stroke), (centpos_y + distance_y - stroke), 	(centpos_x + distance_x - stroke), (centpos_y + distance_y + stroke)); // Up
			dest[3] = GangZoneCreate((centpos_x - distance_x + stroke), (centpos_y - stroke),				(centpos_x + distance_x - stroke), (centpos_y + stroke)); // Center
			dest[4] = GangZoneCreate((centpos_x - distance_x + stroke), (centpos_y - distance_y - stroke), 	(centpos_x + distance_x - stroke), (centpos_y - distance_y + stroke)); // Down

			ret = 5;
	  	}

	  	case '9': {
	  		new Float:stroke = (LETTER_STROKE * numbersize);
			new Float:distance_y = (20.0 * numbersize);
			new Float:distance_x = (25.0 * numbersize * LETTER_XSQUASH);

			dest[0] = GangZoneCreate((centpos_x - distance_x - stroke),	centpos_y,							(centpos_x - distance_x + stroke),			(centpos_y + distance_y - stroke)); // Left Stick
			dest[1] = GangZoneCreate((centpos_x + distance_x),			(centpos_y - distance_y - stroke),	(centpos_x + distance_x + (2 * stroke)),	(centpos_y + distance_y + stroke)); // Right Stick
			dest[2] = GangZoneCreate((centpos_x - distance_x - stroke),	(centpos_y + distance_y - stroke),	(centpos_x + distance_x),					(centpos_y + distance_y + stroke)); // Up
			dest[3] = GangZoneCreate((centpos_x - distance_x + stroke),	centpos_y,							(centpos_x + distance_x),					(centpos_y + stroke + stroke)); // Middle
			dest[4] = GangZoneCreate((centpos_x - distance_x - stroke),	(centpos_y - distance_y - stroke),	(centpos_x + distance_x),					(centpos_y - distance_y + stroke)); // Down

			ret = 5;
	  	}
	}

	for (new i = 0; i < ret; i++) {
		if (dest[i] == -1) {
		
			for (i = 0; i < ret; i++) {
				GangZoneDestroy(dest[i]);
				dest[i] = -1;
			}
			
		    return 0;
		}
	}

	return ret;
}

stock GangZone_Create(Float:minx, Float:miny, Float:maxx, Float:maxy, number = -1, Float:bordersize = GANGZONE_DEFAULT_BORDER_SIZE, Float:numbersize = GANGZONE_DEFAULT_NUMBER_SIZE) {
	new index = -1;

	for (new i = 0; i < MAX_GANG_ZONES; i++) {
        if (GangZone@MainZone[i] == -1) {
            index = i;
			break;
		}
	}

	if (index == -1) {
		return -1;
	}

    GangZone@MainZone[index] = GangZoneCreate(minx, miny, maxx, maxy);
    if (GangZone@MainZone[index] == -1) {
		return -1;
    }

	GangZone@Border[index][0] = -1;
	GangZone@Border[index][1] = -1;
	GangZone@Border[index][2] = -1;
	GangZone@Border[index][3] = -1;

	GangZone@Number1[index][0] = -1;
	GangZone@Number1[index][1] = -1;
	GangZone@Number1[index][2] = -1;
   	GangZone@Number1[index][3] = -1;
   	GangZone@Number1[index][4] = -1;

    GangZone@Number2[index][0] = -1;
    GangZone@Number2[index][1] = -1;
    GangZone@Number2[index][2] = -1;
    GangZone@Number2[index][3] = -1;
    GangZone@Number2[index][4] = -1;

	if (bordersize != 0.0) {
		if (GangZone_CreateBorder(GangZone@Border[index], minx, miny, maxx, maxy, bordersize) == 0) {
      		GangZoneDestroy(GangZone@MainZone[index]);
      		GangZone@MainZone[index] = -1;
			return -1;
		}
	}

	if (numbersize != 0.0) {
		if (number == -1) {
			number = (index % 100);
		}

		if (number >= 0 && number <= 9) {
		    new Float:centpos_x = ((minx + maxx) / 2);
			new Float:centpos_y = ((miny + maxy) / 2);

	        new chars[2];
			format(chars, sizeof(chars), "%i", number);

			if (GangZone_CreateNumber(GangZone@Number1[index], chars[0], centpos_x, centpos_y, numbersize) == 0) {
	    		GangZoneDestroy(GangZone@MainZone[index]);
	    		GangZone@MainZone[index] = -1;

	    		GangZoneDestroy(GangZone@Border[index][0]);
	    		GangZoneDestroy(GangZone@Border[index][1]);
	    		GangZoneDestroy(GangZone@Border[index][2]);
	    		GangZoneDestroy(GangZone@Border[index][3]);
				return -1;
			}
		}
		else if (number >= 10 && number <= 99) {
		    const Float:NUMBER_DISTANCE = 25.0;

		    new Float:centpos_x = ((minx + maxx) / 2);
			new Float:centpos_y = ((miny + maxy) / 2);

	        new chars[3];
			format(chars, sizeof(chars), "%i", number);

			new numZones = GangZone_CreateNumber(GangZone@Number1[index], chars[0], (centpos_x - (NUMBER_DISTANCE * numbersize)), centpos_y, numbersize);
			if (numZones == 0) {
	    		GangZoneDestroy(GangZone@MainZone[index]);
	    		GangZone@MainZone[index] = -1;

	    		GangZoneDestroy(GangZone@Border[index][0]);
	    		GangZoneDestroy(GangZone@Border[index][1]);
	    		GangZoneDestroy(GangZone@Border[index][2]);
	    		GangZoneDestroy(GangZone@Border[index][3]);
				return -1;
			}

			if (GangZone_CreateNumber(GangZone@Number2[index], chars[1], (centpos_x + (NUMBER_DISTANCE * numbersize)), centpos_y, numbersize) == 0) {
	    		GangZoneDestroy(GangZone@MainZone[index]);
	    		GangZone@MainZone[index] = -1;

	    		GangZoneDestroy(GangZone@Border[index][0]);
	    		GangZoneDestroy(GangZone@Border[index][1]);
	    		GangZoneDestroy(GangZone@Border[index][2]);
	    		GangZoneDestroy(GangZone@Border[index][3]);

	    		for (new i = 0; i < numZones; i++) {
	    			GangZoneDestroy(GangZone@Number1[index][i]);
	    		}
				return -1;
			}
		}
		else {
			GangZoneDestroy(GangZone@MainZone[index]);
			GangZone@MainZone[index] = -1;

	   		GangZoneDestroy(GangZone@Border[index][0]);
	   		GangZoneDestroy(GangZone@Border[index][1]);
	   		GangZoneDestroy(GangZone@Border[index][2]);
	   		GangZoneDestroy(GangZone@Border[index][3]);
			return -1;
		}
	}

	return index;
}
#if defined _ALS_GangZoneCreate
    #undef GangZoneCreate
#else
    #define _ALS_GangZoneCreate
#endif
#define GangZoneCreate GangZone_Create

stock GangZone_Destroy(gangzone) {
	if (gangzone < 0 || gangzone >= MAX_GANG_ZONES) {
	    return 0;
	}

	if (GangZone@MainZone[gangzone] == -1) {
		return 0;
	}

  	GangZoneDestroy(GangZone@MainZone[gangzone]);
	GangZone@MainZone[gangzone] = -1;

  	GangZoneDestroy(GangZone@Border[gangzone][0]);
  	GangZoneDestroy(GangZone@Border[gangzone][1]);
  	GangZoneDestroy(GangZone@Border[gangzone][2]);
  	GangZoneDestroy(GangZone@Border[gangzone][3]);

  	GangZoneDestroy(GangZone@Number1[gangzone][0]);
  	GangZoneDestroy(GangZone@Number1[gangzone][1]);
  	GangZoneDestroy(GangZone@Number1[gangzone][2]);
  	GangZoneDestroy(GangZone@Number1[gangzone][3]);
  	GangZoneDestroy(GangZone@Number1[gangzone][4]);

  	GangZoneDestroy(GangZone@Number2[gangzone][0]);
  	GangZoneDestroy(GangZone@Number2[gangzone][1]);
  	GangZoneDestroy(GangZone@Number2[gangzone][2]);
  	GangZoneDestroy(GangZone@Number2[gangzone][3]);
  	GangZoneDestroy(GangZone@Number2[gangzone][4]);

  	return 1;
}
#if defined _ALS_GangZoneDestroy
    #undef GangZoneDestroy
#else
    #define _ALS_GangZoneDestroy
#endif
#define GangZoneDestroy GangZone_Destroy

stock GangZone_ShowForPlayer(playerid, gangzone, color, bordercolor = GANGZONE_DEFAULT_BORDER_COLOR, numbercolor = GANGZONE_DEFAULT_NUMBER_COLOR) {
	if (gangzone < 0 || gangzone >= MAX_GANG_ZONES) {
	    return 0;
	}

	if (GangZone@MainZone[gangzone] == -1) {
		return 0;
	}

  	GangZoneShowForPlayer(playerid, GangZone@MainZone[gangzone], color);

  	GangZoneShowForPlayer(playerid, GangZone@Border[gangzone][0], bordercolor);
  	GangZoneShowForPlayer(playerid, GangZone@Border[gangzone][1], bordercolor);
  	GangZoneShowForPlayer(playerid, GangZone@Border[gangzone][2], bordercolor);
  	GangZoneShowForPlayer(playerid, GangZone@Border[gangzone][3], bordercolor);

  	GangZoneShowForPlayer(playerid, GangZone@Number1[gangzone][0], numbercolor);
  	GangZoneShowForPlayer(playerid, GangZone@Number1[gangzone][1], numbercolor);
  	GangZoneShowForPlayer(playerid, GangZone@Number1[gangzone][2], numbercolor);
  	GangZoneShowForPlayer(playerid, GangZone@Number1[gangzone][3], numbercolor);
  	GangZoneShowForPlayer(playerid, GangZone@Number1[gangzone][4], numbercolor);

  	GangZoneShowForPlayer(playerid, GangZone@Number2[gangzone][0], numbercolor);
  	GangZoneShowForPlayer(playerid, GangZone@Number2[gangzone][1], numbercolor);
  	GangZoneShowForPlayer(playerid, GangZone@Number2[gangzone][2], numbercolor);
  	GangZoneShowForPlayer(playerid, GangZone@Number2[gangzone][3], numbercolor);
  	GangZoneShowForPlayer(playerid, GangZone@Number2[gangzone][4], numbercolor);

	return 1;
}
#if defined _ALS_GangZoneShowForPlayer
    #undef GangZoneShowForPlayer
#else
    #define _ALS_GangZoneShowForPlayer
#endif
#define GangZoneShowForPlayer GangZone_ShowForPlayer

stock GangZone_ShowForAll(gangzone, color, bordercolor = GANGZONE_DEFAULT_BORDER_COLOR, numbercolor = GANGZONE_DEFAULT_NUMBER_COLOR) {
	if (gangzone < 0 || gangzone >= MAX_GANG_ZONES) {
	    return 0;
	}

	if (GangZone@MainZone[gangzone] == -1) {
		return 0;
	}

  	GangZoneShowForAll(GangZone@MainZone[gangzone], color);

  	GangZoneShowForAll(GangZone@Border[gangzone][0], bordercolor);
  	GangZoneShowForAll(GangZone@Border[gangzone][1], bordercolor);
  	GangZoneShowForAll(GangZone@Border[gangzone][2], bordercolor);
  	GangZoneShowForAll(GangZone@Border[gangzone][3], bordercolor);

  	GangZoneShowForAll(GangZone@Number1[gangzone][0], numbercolor);
  	GangZoneShowForAll(GangZone@Number1[gangzone][1], numbercolor);
  	GangZoneShowForAll(GangZone@Number1[gangzone][2], numbercolor);
  	GangZoneShowForAll(GangZone@Number1[gangzone][3], numbercolor);
  	GangZoneShowForAll(GangZone@Number1[gangzone][4], numbercolor);

  	GangZoneShowForAll(GangZone@Number2[gangzone][0], numbercolor);
  	GangZoneShowForAll(GangZone@Number2[gangzone][1], numbercolor);
  	GangZoneShowForAll(GangZone@Number2[gangzone][2], numbercolor);
  	GangZoneShowForAll(GangZone@Number2[gangzone][3], numbercolor);
  	GangZoneShowForAll(GangZone@Number2[gangzone][4], numbercolor);

	return 1;
}
#if defined _ALS_GangZoneShowForAll
    #undef GangZoneShowForAll
#else
    #define _ALS_GangZoneShowForAll
#endif
#define GangZoneShowForAll GangZone_ShowForAll

stock GangZone_HideForPlayer(playerid, gangzone) {
	if (gangzone < 0 || gangzone >= MAX_GANG_ZONES) {
	    return 0;
	}

	if (GangZone@MainZone[gangzone] == -1) {
		return 0;
	}

  	GangZoneHideForPlayer(playerid, GangZone@MainZone[gangzone]);

  	GangZoneHideForPlayer(playerid, GangZone@Border[gangzone][0]);
  	GangZoneHideForPlayer(playerid, GangZone@Border[gangzone][1]);
  	GangZoneHideForPlayer(playerid, GangZone@Border[gangzone][2]);
  	GangZoneHideForPlayer(playerid, GangZone@Border[gangzone][3]);

  	GangZoneHideForPlayer(playerid, GangZone@Number1[gangzone][0]);
  	GangZoneHideForPlayer(playerid, GangZone@Number1[gangzone][1]);
  	GangZoneHideForPlayer(playerid, GangZone@Number1[gangzone][2]);
  	GangZoneHideForPlayer(playerid, GangZone@Number1[gangzone][3]);
  	GangZoneHideForPlayer(playerid, GangZone@Number1[gangzone][4]);

  	GangZoneHideForPlayer(playerid, GangZone@Number2[gangzone][0]);
  	GangZoneHideForPlayer(playerid, GangZone@Number2[gangzone][1]);
  	GangZoneHideForPlayer(playerid, GangZone@Number2[gangzone][2]);
  	GangZoneHideForPlayer(playerid, GangZone@Number2[gangzone][3]);
  	GangZoneHideForPlayer(playerid, GangZone@Number2[gangzone][4]);

	return 1;
}
#if defined _ALS_GangZoneHideForPlayer
    #undef GangZoneHideForPlayer
#else
    #define _ALS_GangZoneHideForPlayer
#endif
#define GangZoneHideForPlayer GangZone_HideForPlayer

stock GangZone_HideForAll(gangzone) {
	if (gangzone < 0 || gangzone >= MAX_GANG_ZONES) {
	    return 0;
	}

	if (GangZone@MainZone[gangzone] == -1) {
		return 0;
	}

  	GangZoneHideForAll(GangZone@MainZone[gangzone]);

  	GangZoneHideForAll(GangZone@Border[gangzone][0]);
  	GangZoneHideForAll(GangZone@Border[gangzone][1]);
  	GangZoneHideForAll(GangZone@Border[gangzone][2]);
  	GangZoneHideForAll(GangZone@Border[gangzone][3]);

  	GangZoneHideForAll(GangZone@Number1[gangzone][0]);
  	GangZoneHideForAll(GangZone@Number1[gangzone][1]);
  	GangZoneHideForAll(GangZone@Number1[gangzone][2]);
  	GangZoneHideForAll(GangZone@Number1[gangzone][3]);
  	GangZoneHideForAll(GangZone@Number1[gangzone][4]);

  	GangZoneHideForAll(GangZone@Number2[gangzone][0]);
  	GangZoneHideForAll(GangZone@Number2[gangzone][1]);
  	GangZoneHideForAll(GangZone@Number2[gangzone][2]);
  	GangZoneHideForAll(GangZone@Number2[gangzone][3]);
  	GangZoneHideForAll(GangZone@Number2[gangzone][4]);

	return 1;
}
#if defined _ALS_GangZoneHideForAll
    #undef GangZoneHideForAll
#else
    #define _ALS_GangZoneHideForAll
#endif
#define GangZoneHideForAll GangZone_HideForAll

stock GangZone_FlashForPlayer(playerid, gangzone, flashcolor, borderflashcolor = 0x00, numberflashcolor = 0x00) {
	if (gangzone < 0 || gangzone >= MAX_GANG_ZONES) {
	    return 0;
	}

	if (GangZone@MainZone[gangzone] == -1) {
		return 0;
	}

  	GangZoneFlashForPlayer(playerid, GangZone@MainZone[gangzone], flashcolor);

	if (borderflashcolor != 0x00) {
	  	GangZoneFlashForPlayer(playerid, GangZone@Border[gangzone][0], borderflashcolor);
	  	GangZoneFlashForPlayer(playerid, GangZone@Border[gangzone][1], borderflashcolor);
	  	GangZoneFlashForPlayer(playerid, GangZone@Border[gangzone][2], borderflashcolor);
	  	GangZoneFlashForPlayer(playerid, GangZone@Border[gangzone][3], borderflashcolor);
	}

	if (numberflashcolor != 0x00) {
	  	GangZoneFlashForPlayer(playerid, GangZone@Number1[gangzone][0], numberflashcolor);
	  	GangZoneFlashForPlayer(playerid, GangZone@Number1[gangzone][1], numberflashcolor);
	  	GangZoneFlashForPlayer(playerid, GangZone@Number1[gangzone][2], numberflashcolor);
	  	GangZoneFlashForPlayer(playerid, GangZone@Number1[gangzone][3], numberflashcolor);
	  	GangZoneFlashForPlayer(playerid, GangZone@Number1[gangzone][4], numberflashcolor);

	  	GangZoneFlashForPlayer(playerid, GangZone@Number2[gangzone][0], numberflashcolor);
	  	GangZoneFlashForPlayer(playerid, GangZone@Number2[gangzone][1], numberflashcolor);
	  	GangZoneFlashForPlayer(playerid, GangZone@Number2[gangzone][2], numberflashcolor);
	  	GangZoneFlashForPlayer(playerid, GangZone@Number2[gangzone][3], numberflashcolor);
	  	GangZoneFlashForPlayer(playerid, GangZone@Number2[gangzone][4], numberflashcolor);
	}

	return 1;
}
#if defined _ALS_GangZoneFlashForPlayer
    #undef GangZoneFlashForPlayer
#else
    #define _ALS_GangZoneFlashForPlayer
#endif
#define GangZoneFlashForPlayer GangZone_FlashForPlayer

stock GangZone_FlashForAll(gangzone, flashcolor, borderflashcolor = 0x00, numberflashcolor = 0x00) {
	if (gangzone < 0 || gangzone >= MAX_GANG_ZONES) {
	    return 0;
	}

	if (GangZone@MainZone[gangzone] == -1) {
		return 0;
	}

  	GangZoneFlashForAll(GangZone@MainZone[gangzone], flashcolor);

	if (borderflashcolor != 0x00) {
	  	GangZoneFlashForAll(GangZone@Border[gangzone][0], borderflashcolor);
	  	GangZoneFlashForAll(GangZone@Border[gangzone][1], borderflashcolor);
	  	GangZoneFlashForAll(GangZone@Border[gangzone][2], borderflashcolor);
	  	GangZoneFlashForAll(GangZone@Border[gangzone][3], borderflashcolor);
	}

	if (numberflashcolor != 0x00) {
	  	GangZoneFlashForAll(GangZone@Number1[gangzone][0], numberflashcolor);
	  	GangZoneFlashForAll(GangZone@Number1[gangzone][1], numberflashcolor);
	  	GangZoneFlashForAll(GangZone@Number1[gangzone][2], numberflashcolor);
	  	GangZoneFlashForAll(GangZone@Number1[gangzone][3], numberflashcolor);
	  	GangZoneFlashForAll(GangZone@Number1[gangzone][4], numberflashcolor);

	  	GangZoneFlashForAll(GangZone@Number2[gangzone][0], numberflashcolor);
	  	GangZoneFlashForAll(GangZone@Number2[gangzone][1], numberflashcolor);
	  	GangZoneFlashForAll(GangZone@Number2[gangzone][2], numberflashcolor);
	  	GangZoneFlashForAll(GangZone@Number2[gangzone][3], numberflashcolor);
	  	GangZoneFlashForAll(GangZone@Number2[gangzone][4], numberflashcolor);
	}

	return 1;
}
#if defined _ALS_GangZoneFlashForAll
    #undef GangZoneFlashForAll
#else
    #define _ALS_GangZoneFlashForAll
#endif
#define GangZoneFlashForAll GangZone_FlashForAll

stock GangZone_StopFlashForPlayer(playerid, gangzone, bool:mainzone = true, bool:border = true, bool:number = true) {
	if (gangzone < 0 || gangzone >= MAX_GANG_ZONES) {
	    return 0;
	}

	if (GangZone@MainZone[gangzone] == -1) {
		return 0;
	}

	if (mainzone) {
  		GangZoneStopFlashForPlayer(playerid, GangZone@MainZone[gangzone]);
	}

	if (border) {
	  	GangZoneStopFlashForPlayer(playerid, GangZone@Border[gangzone][0]);
	  	GangZoneStopFlashForPlayer(playerid, GangZone@Border[gangzone][1]);
	  	GangZoneStopFlashForPlayer(playerid, GangZone@Border[gangzone][2]);
	  	GangZoneStopFlashForPlayer(playerid, GangZone@Border[gangzone][3]);
	}

	if (number) {
	  	GangZoneStopFlashForPlayer(playerid, GangZone@Number1[gangzone][0]);
	  	GangZoneStopFlashForPlayer(playerid, GangZone@Number1[gangzone][1]);
	  	GangZoneStopFlashForPlayer(playerid, GangZone@Number1[gangzone][2]);
	  	GangZoneStopFlashForPlayer(playerid, GangZone@Number1[gangzone][3]);
	  	GangZoneStopFlashForPlayer(playerid, GangZone@Number1[gangzone][4]);

	  	GangZoneStopFlashForPlayer(playerid, GangZone@Number2[gangzone][0]);
	  	GangZoneStopFlashForPlayer(playerid, GangZone@Number2[gangzone][1]);
	  	GangZoneStopFlashForPlayer(playerid, GangZone@Number2[gangzone][2]);
	  	GangZoneStopFlashForPlayer(playerid, GangZone@Number2[gangzone][3]);
	  	GangZoneStopFlashForPlayer(playerid, GangZone@Number2[gangzone][4]);
	}

	return 1;
}
#if defined _ALS_GangZoneStopFlashForPlayer
    #undef GangZoneStopFlashForPlayer
#else
    #define _ALS_GangZoneStopFlashForPlayer
#endif
#define GangZoneStopFlashForPlayer GangZone_StopFlashForPlayer

stock GangZone_StopFlashForAll(gangzone, bool:mainzone = true, bool:border = true, bool:number = true) {
	if (gangzone < 0 || gangzone >= MAX_GANG_ZONES) {
	    return 0;
	}

	if (GangZone@MainZone[gangzone] == -1) {
		return 0;
	}

	if (mainzone) {
  		GangZoneStopFlashForAll(GangZone@MainZone[gangzone]);
	}

	if (border) {
	  	GangZoneStopFlashForAll(GangZone@Border[gangzone][0]);
	  	GangZoneStopFlashForAll(GangZone@Border[gangzone][1]);
	  	GangZoneStopFlashForAll(GangZone@Border[gangzone][2]);
	  	GangZoneStopFlashForAll(GangZone@Border[gangzone][3]);
	}

	if (number) {
	  	GangZoneStopFlashForAll(GangZone@Number1[gangzone][0]);
	  	GangZoneStopFlashForAll(GangZone@Number1[gangzone][1]);
	  	GangZoneStopFlashForAll(GangZone@Number1[gangzone][2]);
	  	GangZoneStopFlashForAll(GangZone@Number1[gangzone][3]);
	  	GangZoneStopFlashForAll(GangZone@Number1[gangzone][4]);

	  	GangZoneStopFlashForAll(GangZone@Number2[gangzone][0]);
	  	GangZoneStopFlashForAll(GangZone@Number2[gangzone][1]);
	  	GangZoneStopFlashForAll(GangZone@Number2[gangzone][2]);
	  	GangZoneStopFlashForAll(GangZone@Number2[gangzone][3]);
	  	GangZoneStopFlashForAll(GangZone@Number2[gangzone][4]);
	}

	return 1;
}
#if defined _ALS_GangZoneStopFlashForAll
    #undef GangZoneStopFlashForAll
#else
    #define _ALS_GangZoneStopFlashForAll
#endif
#define GangZoneStopFlashForAll GangZone_StopFlashForAll
