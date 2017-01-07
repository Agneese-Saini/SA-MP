#if defined attachments_included
	#endinput
#endif
#define attachments_included

#if !defined KEY_AIM
	#define KEY_AIM 128
#endif

enum ATT_e
{
	bool:	ATT_e_USED,
 			ATT_e_MODEL,
    		ATT_e_BONE,
    Float:	ATT_e_X,
   	Float:	ATT_e_Y,
   	Float:	ATT_e_Z,
   	Float:  ATT_e_RX,
   	Float:  ATT_e_RY,
   	Float:  ATT_e_RZ,
   	Float:  ATT_e_SX,
   	Float:  ATT_e_SY,
   	Float:  ATT_e_SZ,
			ATT_e_COLOR1,
			ATT_e_COLOR2
};
static ATT_ePlayerData[MAX_PLAYERS][MAX_PLAYER_ATTACHED_OBJECTS][ATT_e];

public OnPlayerConnect(playerid)
{
    for (new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
    {
   	 	ATT_ePlayerData[playerid][i][ATT_e_USED] = false;
   	 	ATT_ePlayerData[playerid][i][ATT_e_MODEL] = 0;
   	 	ATT_ePlayerData[playerid][i][ATT_e_BONE] = 0;
   	 	ATT_ePlayerData[playerid][i][ATT_e_X] = 0.0;
   	 	ATT_ePlayerData[playerid][i][ATT_e_Y] = 0.0;
   	 	ATT_ePlayerData[playerid][i][ATT_e_Z] = 0.0;
   	 	ATT_ePlayerData[playerid][i][ATT_e_RX] = 0.0;
   	 	ATT_ePlayerData[playerid][i][ATT_e_RY] = 0.0;
   	 	ATT_ePlayerData[playerid][i][ATT_e_RZ] = 0.0;
   	 	ATT_ePlayerData[playerid][i][ATT_e_SX] = 1.0;
   	 	ATT_ePlayerData[playerid][i][ATT_e_SY] = 1.0;
   	 	ATT_ePlayerData[playerid][i][ATT_e_SZ] = 1.0;
   	 	ATT_ePlayerData[playerid][i][ATT_e_COLOR1] = 0;
   	 	ATT_ePlayerData[playerid][i][ATT_e_COLOR2] = 0;
	}
	
	#if defined ATT_OnPlayerConnect
		return ATT_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect ATT_OnPlayerConnect
#if defined ATT_OnPlayerConnect
	forward ATT_OnPlayerConnect(playerid);
#endif

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
	{
		if ((newkeys & KEY_AIM) == KEY_AIM)
		{
		    switch (GetPlayerWeapon(playerid))
			{
				case WEAPON_SNIPER, WEAPON_ROCKETLAUNCHER, WEAPON_HEATSEEKER, WEAPON_CAMERA:
				{
				    for (new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
					{
						if (IsPlayerAttachedObjectSlotUsed(playerid, i))
						{
					    	RemovePlayerAttachedObject(playerid, i);
						}
			        }
			    }
			}
		}
		else if ((newkeys & KEY_AIM) != KEY_AIM && (oldkeys & KEY_AIM) == KEY_AIM)
		{
		    for (new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
			{
				if (!IsPlayerAttachedObjectSlotUsed(playerid, i) && ATT_ePlayerData[playerid][i][ATT_e_USED])
			    {
					SetPlayerAttachedObject(playerid, i,
												ATT_ePlayerData[playerid][i][ATT_e_MODEL],
												ATT_ePlayerData[playerid][i][ATT_e_BONE],
												ATT_ePlayerData[playerid][i][ATT_e_X],
												ATT_ePlayerData[playerid][i][ATT_e_Y],
												ATT_ePlayerData[playerid][i][ATT_e_Z],
											 	ATT_ePlayerData[playerid][i][ATT_e_RX],
												ATT_ePlayerData[playerid][i][ATT_e_RY],
											 	ATT_ePlayerData[playerid][i][ATT_e_RZ],
											 	ATT_ePlayerData[playerid][i][ATT_e_SX],
											 	ATT_ePlayerData[playerid][i][ATT_e_SY],
											 	ATT_ePlayerData[playerid][i][ATT_e_SZ],
												ATT_ePlayerData[playerid][i][ATT_e_COLOR1],
												ATT_ePlayerData[playerid][i][ATT_e_COLOR2]);
				}
			}
		}
	}

	#if defined ATT_OnPlayerKeyStateChange
       	return ATT_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange ATT_OnPlayerKeyStateChange
#if defined ATT_OnPlayerKeyStateChange
    forward ATT_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif

stock ATT_SetPlayerAttachedObject(playerid, index, modelid, bone, Float:fOffsetX = 0.0, Float:fOffsetY = 0.0, Float:fOffsetZ = 0.0, Float:fRotX = 0.0, Float:fRotY = 0.0, Float:fRotZ = 0.0, Float:fScaleX = 1.0, Float:fScaleY = 1.0, Float:fScaleZ = 1.0, materialcolor1 = 0, materialcolor2 = 0)
{
    new ret = SetPlayerAttachedObject(playerid, index, modelid, bone, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ, materialcolor1, materialcolor2);
    if (ret)
	{
		ATT_ePlayerData[playerid][index][ATT_e_USED] = true;
		ATT_ePlayerData[playerid][index][ATT_e_MODEL] = modelid;
		ATT_ePlayerData[playerid][index][ATT_e_BONE] = bone;
		ATT_ePlayerData[playerid][index][ATT_e_X] = fOffsetX;
		ATT_ePlayerData[playerid][index][ATT_e_Y] = fOffsetY;
		ATT_ePlayerData[playerid][index][ATT_e_Z] = fOffsetZ;
		ATT_ePlayerData[playerid][index][ATT_e_RX] = fRotX;
		ATT_ePlayerData[playerid][index][ATT_e_RY] = fRotY;
		ATT_ePlayerData[playerid][index][ATT_e_RZ] = fRotZ;
		ATT_ePlayerData[playerid][index][ATT_e_SX] = fScaleX;
		ATT_ePlayerData[playerid][index][ATT_e_SY] = fScaleY;
		ATT_ePlayerData[playerid][index][ATT_e_SZ] = fScaleZ;
		ATT_ePlayerData[playerid][index][ATT_e_COLOR1] = materialcolor1;
		ATT_ePlayerData[playerid][index][ATT_e_COLOR2] = materialcolor2;
	}
	return ret;
}
#if defined _ALS_SetPlayerAttachedObject
    #undef SetPlayerAttachedObject
#else
	#define _ALS_SetPlayerAttachedObject
#endif
#define SetPlayerAttachedObject ATT_SetPlayerAttachedObject

stock ATT_RemovePlayerAttachedObject(playerid, index)
{
   	new ret = RemovePlayerAttachedObject(playerid, index);
   	if (ret)
	{
		ATT_ePlayerData[playerid][index][ATT_e_USED] = false;
	}
	return ret;
}
#if defined _ALS_RemovePlayerAttachedObject
    #undef RemovePlayerAttachedObject
#else
    #define _ALS_RemovePlayerAttachedObject
#endif
#define RemovePlayerAttachedObject ATT_RemovePlayerAttachedObject
