#if defined gangzones_included
	#endinput
#endif
#define gangzones_included

/*
native GangZoneBorderCreate(zoneid[], Float:minx, Float:miny, Float:maxx, Float:maxy, Float:bordersize = 3.0);
*/
stock GangZoneBorderCreate(zoneid[], Float:minx, Float:miny, Float:maxx, Float:maxy, Float:bordersize = 3.0)
{
    zoneid[0] = GangZoneCreate(minx - bordersize, miny + bordersize, minx + bordersize, maxy - bordersize);
	zoneid[1] = GangZoneCreate(minx - bordersize, maxy - bordersize, maxx + bordersize, maxy + bordersize);
	zoneid[2] = GangZoneCreate(maxx - bordersize, miny + bordersize, maxx + bordersize, maxy - bordersize);
	zoneid[3] = GangZoneCreate(minx - bordersize, miny - bordersize, maxx + bordersize, miny + bordersize);
	return 4;
}

/*
native GangZoneNumberCreate(zoneid[], Float:minx, Float:miny, Float:maxx, Float:maxy, number, Float:numbersize = 0.7);
*/
stock GangZoneNumberCreate(zoneid[], Float:minx, Float:miny, Float:maxx, Float:maxy, number, Float:numbersize = 0.7)
{
	new Float:centposx = (minx + maxx) / 2;
	new Float:centposy = (miny + maxy) / 2;

	new num[3];
	format(num, sizeof num, "%d", number);
	
	if (number >= 0 && number <= 9)
	{
	    return Internal_GangZoneCreateNumber(zoneid, num[0], centposx, centposy, numbersize);
	}
	else if (number > 9 && number <= 99)
	{
	    #define NUMBER_DISTANCE \
			25.0

	    new temp[5];
		new ret = Internal_GangZoneCreateNumber(temp, num[0], (centposx - (NUMBER_DISTANCE * numbersize)), centposy, numbersize);
        for (new i; i < ret; i++)
        {
			zoneid[i] = temp[i];
        }

        new ret2 = Internal_GangZoneCreateNumber(temp, num[1], (centposx + (NUMBER_DISTANCE * numbersize)), centposy, numbersize);
        for (new i; i < ret2; i++)
        {
			zoneid[i + ret] = temp[i];
        }
        return (ret + ret2);
	}
	else return 0;
}

static Internal_GangZoneCreateNumber(zoneid[], number, Float:centposx, Float:centposy, Float:numbersize) // Credits for this function: "beijind"
{
	#define LETTER_STROKE \
		3.5
	#define LETTER_XSQUASH \
		0.7
	
	switch (number)
	{
	  	case '0':
	  	{
			new Float:gztk = LETTER_STROKE*numbersize;
			new Float:gsdisx = 20.0*numbersize*LETTER_XSQUASH;
			new Float:gsdisy = 20.0*numbersize;
        	zoneid[0] = GangZoneCreate(centposx-gsdisx-gztk,centposy-gsdisy-gztk,centposx+gsdisx+gztk,centposy-gsdisy+gztk); // Down of 0
			zoneid[1] = GangZoneCreate(centposx-gsdisx-gztk,centposy+gsdisy+gztk,centposx+gsdisx+gztk,centposy+gsdisy-gztk); // Up of 0
       	 	zoneid[2] = GangZoneCreate(centposx-gsdisx-gztk,centposy-gsdisy+gztk,centposx-gsdisx+gztk,centposy+gsdisy-gztk); // Left of 0
        	zoneid[3] = GangZoneCreate(centposx+gsdisx-gztk,centposy-gsdisy+gztk,centposx+gsdisx+gztk,centposy+gsdisy-gztk); // Right of 0
        	return 4;
	  	}
	  	
	  	case '1':
	  	{
			new Float:gztk = LETTER_STROKE*numbersize;
			new Float:gsdisy = 20.0*numbersize;
			zoneid[0] = GangZoneCreate(centposx-gztk,centposy-gsdisy-gztk,centposx+gztk,centposy+gsdisy+gztk);
        	return 1;
	  	}
	  	
	  	case '2':
	  	{
			new Float:gztk = LETTER_STROKE*numbersize;
			new Float:gsdisy = 20.0*numbersize;
			new Float:gsdisx = 25.0*numbersize*LETTER_XSQUASH;
			zoneid[0] = GangZoneCreate(centposx-gsdisx,centposy+gsdisy-gztk,centposx+gsdisx,centposy+gsdisy+gztk); // Up
			zoneid[1] = GangZoneCreate(centposx+gsdisx-gztk-gztk,centposy+gztk,centposx+gsdisx,centposy+gsdisy-gztk); // Right Stick
			zoneid[2] = GangZoneCreate(centposx-gsdisx,centposy-gztk,centposx+gsdisx,centposy+gztk); // Middle
			zoneid[3] = GangZoneCreate(centposx-gsdisx,centposy-gsdisy+gztk,centposx-gsdisx+gztk+gztk,centposy-gztk); // Left Stick
	        zoneid[4] = GangZoneCreate(centposx-gsdisx,centposy-gsdisy-gztk,centposx+gsdisx,centposy-gsdisy+gztk); // Down
        	return 5;
	  	}
	  	
	  	case '3':
	  	{
			new Float:gztk = LETTER_STROKE*numbersize;
			new Float:gsdisy = 20.0*numbersize;
			new Float:gsdisx = 25.0*numbersize*LETTER_XSQUASH;
			zoneid[0] = GangZoneCreate(centposx-gsdisx,centposy+gsdisy-gztk,centposx+gsdisx,centposy+gsdisy+gztk); // Up
			zoneid[1] = GangZoneCreate(centposx+gsdisx,centposy-gsdisy-gztk,centposx+gsdisx+gztk+gztk,centposy+gsdisy+gztk); // Stick
			zoneid[2] = GangZoneCreate(centposx-gsdisx,centposy-gztk,centposx+gsdisx,centposy+gztk); // Middle
			zoneid[3] = GangZoneCreate(centposx-gsdisx,centposy-gsdisy-gztk,centposx+gsdisx,centposy-gsdisy+gztk); // Down
        	return 4;
	  	}
	  	
	  	case '4':
	  	{
	  		new Float:gztk = LETTER_STROKE*numbersize;
			new Float:gsdisy = 20.0*numbersize;
			new Float:gsdisx = 25.0*numbersize*LETTER_XSQUASH;
			zoneid[0] = GangZoneCreate(centposx-gsdisx,centposy+gsdisy,centposx-gsdisx+gztk+gztk,centposy+gztk); // Left
			zoneid[1] = GangZoneCreate(centposx-gsdisx,centposy-gztk,centposx+gsdisx,centposy+gztk);// Middle
			zoneid[2] = GangZoneCreate(centposx+gsdisx,centposy-gsdisy,centposx+gsdisx+gztk+gztk,centposy+gsdisy); // Stick
        	return 3;
	  	}
	  	
	  	case '5':
	  	{
	  		new Float:gztk = LETTER_STROKE*numbersize;
			new Float:gsdisy = 20.0*numbersize;
			new Float:gsdisx = 25.0*numbersize*LETTER_XSQUASH;
			zoneid[0] = GangZoneCreate(centposx-gsdisx-gztk,centposy+gsdisy-gztk,centposx+gsdisx+gztk,centposy+gsdisy+gztk); // Up
			zoneid[1] = GangZoneCreate(centposx-gsdisx-gztk,centposy+gztk,centposx-gsdisx+gztk,centposy+gsdisy-gztk); // Left Stick
			zoneid[2] = GangZoneCreate(centposx-gsdisx-gztk,centposy-gztk,centposx+gsdisx+gztk,centposy+gztk); // Middle
			zoneid[3] = GangZoneCreate(centposx+gsdisx-gztk,centposy-gztk,centposx+gsdisx+gztk,centposy-gsdisy+gztk); // Right Stick
			zoneid[4] = GangZoneCreate(centposx-gsdisx-gztk,centposy-gsdisy-gztk,centposx+gsdisx+gztk,centposy-gsdisy+gztk); // Down
        	return 5;
	  	}
	  	
	  	case '6':
	  	{
	  		new Float:gztk = LETTER_STROKE*numbersize;
			new Float:gsdisy = 20.0*numbersize;
			new Float:gsdisx = 25.0*numbersize*LETTER_XSQUASH;
			zoneid[0] = GangZoneCreate(centposx-gsdisx,centposy+gsdisy-gztk,centposx+gsdisx,centposy+gsdisy+gztk); // Up
			zoneid[1] = GangZoneCreate(centposx-gsdisx-gztk-gztk,centposy+gsdisy+gztk,centposx-gsdisx,centposy-gsdisy-gztk); // Main Left Stick
			zoneid[2] = GangZoneCreate(centposx-gsdisx,centposy-gztk,centposx+gsdisx,centposy+gztk); // Middle
			zoneid[3] = GangZoneCreate(centposx-gsdisx,centposy-gsdisy-gztk,centposx+gsdisx,centposy-gsdisy+gztk); // Down
			zoneid[4] = GangZoneCreate(centposx+gsdisx-gztk-gztk,centposy-gsdisy+gztk,centposx+gsdisx,centposy-gztk); // Right small stick
        	return 5;
	  	}
	  	
	  	case '7':
	  	{
			new Float:gztk = LETTER_STROKE*numbersize;
			new Float:gsdisy = 20.0*numbersize;
			new Float:gsdisx = 15.0*numbersize*LETTER_XSQUASH;
			zoneid[0] = GangZoneCreate(centposx+gsdisx-gztk,centposy-gsdisy-gztk,centposx+gsdisx+gztk,centposy+gsdisy-gztk); // Stick of 7
			zoneid[1] = GangZoneCreate(centposx-gsdisx-gsdisx,centposy+gsdisy-gztk,centposx+gsdisx+gztk,centposy+gsdisy+gztk); // Up of 7
        	return 2;
	  	}
	  	
	  	case '8':
	  	{
	  		new Float:gztk = LETTER_STROKE*numbersize;
			new Float:gsdisy = 20.0*numbersize;
			new Float:gsdisx = 25.0*numbersize*LETTER_XSQUASH;
			zoneid[0] = GangZoneCreate(centposx-gsdisx-gztk,centposy+gsdisy+gztk,centposx-gsdisx+gztk,centposy-gsdisy-gztk); // Left stick
			zoneid[1] = GangZoneCreate(centposx+gsdisx-gztk,centposy+gsdisy+gztk,centposx+gsdisx+gztk,centposy-gsdisy-gztk); // Right stick
			zoneid[2] = GangZoneCreate(centposx-gsdisx+gztk,centposy+gsdisy-gztk,centposx+gsdisx-gztk,centposy+gsdisy+gztk); // Up
			zoneid[3] = GangZoneCreate(centposx-gsdisx+gztk,centposy-gztk,centposx+gsdisx-gztk,centposy+gztk); // Center
			zoneid[4] = GangZoneCreate(centposx-gsdisx+gztk,centposy-gsdisy-gztk,centposx+gsdisx-gztk,centposy-gsdisy+gztk); // Down
        	return 5;
	  	}
	  	
	  	case '9':
	  	{
	  		new Float:gztk = LETTER_STROKE*numbersize;
			new Float:gsdisy = 20.0*numbersize;
			new Float:gsdisx = 25.0*numbersize*LETTER_XSQUASH;
			zoneid[0] = GangZoneCreate(centposx-gsdisx-gztk,centposy,centposx-gsdisx+gztk,centposy+gsdisy-gztk); // Left Stick
			zoneid[1] = GangZoneCreate(centposx+gsdisx,centposy-gsdisy-gztk,centposx+gsdisx+gztk+gztk,centposy+gsdisy+gztk); // Right Stick
			zoneid[2] = GangZoneCreate(centposx-gsdisx-gztk,centposy+gsdisy-gztk,centposx+gsdisx,centposy+gsdisy+gztk); // Up
			zoneid[3] = GangZoneCreate(centposx-gsdisx+gztk,centposy,centposx+gsdisx,centposy+gztk+gztk); // Middle
			zoneid[4] = GangZoneCreate(centposx-gsdisx-gztk,centposy-gsdisy-gztk,centposx+gsdisx,centposy-gsdisy+gztk); // Down
        	return 5;
	  	}
	}
	
	return 0;
}
