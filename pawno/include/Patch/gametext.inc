#if defined gametext_included
	#endinput
#endif
#define gametext_included

static const FIX_Encoding[256] =
{
	0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
	16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
	32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
	48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
	64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
	80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
	96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
	112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
	128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
	144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
	160,  94, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
	124, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 175,
	128, 129, 130, 195, 131, 197, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141,
	208, 173, 142, 143, 144, 213, 145, 215, 216, 146, 147, 148, 149, 221, 222, 150,
	151, 152, 153, 227, 154, 229, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164,
	240, 174, 165, 166, 167, 245, 168, 247, 248, 169, 170, 171, 172, 253, 254, 255
};

static GT_IsSafeGameText(string[])
{
	new safetil = -5;
	for (new i, j = strlen(string); i < j; ++i)
	{
	    switch (string[i])
		{
			case '0'..'9', 'A'..'Z', 'a'..'z', '~', '`', '!', '#', '$', '%', '^', '&', '*', '(', ')', '-', '_', '=', '+', '[', ']', ';', ':', '"', '<', '>', '/', '?':
			{
		        if ((string[i] == 126) && (i > safetil))
		        {
		            if (i >= strlen(string) - 1)
		            {
						return 0;
					}
		            else if (string[i + 1] == 126)
		            {
						return 0;
					}
		            else if (string[i + 2] != 126)
		            {
						return 0;
					}
					else
					{
		            	safetil = i + 2;
		        	}
		        }
		        continue;
	        }
			default:
			{
			    new chr = string[i];
			    if (0 <= chr < 256)
				{
					string[i] = FIX_Encoding[chr];
				}
				else
				{
				    string[i] = '?';
				}
	        }
   		}
	}
	return 1;
}

stock GT_GameTextForPlayer(playerid, string[], time, style)
{
	if (string[0] == '\0' || (string[0] == '\1' && string[1] == '\0'))
	{
	    return GameTextForPlayer(playerid, "_", time, style);
	}
	
	if (!GT_IsSafeGameText(string))
	{
	    return -1;
	}
	
	return GameTextForPlayer(playerid, string, time, style);
}
#if defined _ALS_GameTextForPlayer
	#undef GameTextForPlayer
#else
	#define _ALS_GameTextForPlayer
#endif
#define GameTextForPlayer GT_GameTextForPlayer

stock GT_GameTextForAll(string[], time, style)
{
	if (string[0] == '\0' || (string[0] == '\1' && string[1] == '\0'))
	{
	    return GameTextForAll("_", time, style);
	}

	if (!GT_IsSafeGameText(string))
	{
	    return -1;
	}
	
	return GameTextForAll(string, time, style);
}
#if defined _ALS_GameTextForAll
	#undef GameTextForAll
#else
	#define _ALS_GameTextForAll
#endif
#define GameTextForAll GT_GameTextForAll
