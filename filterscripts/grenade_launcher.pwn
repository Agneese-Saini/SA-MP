#include <a_samp>

#define FILTERSCRIPT

#include <streamer>
#include <projectile>

// Your grenade launcher configuration

#define GRENADE_SPEED \
	40.0

#define GRENADE_OBJECT \
    342

#define MAX_PLAYER_GRENADES \
	100

//

// Don't change these

#define MAX_GRENADES \
	(MAX_PLAYERS * MAX_PLAYER_GRENADES)

#define PRESSED(%0) \
	(((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))

//

new grenadesObject[MAX_GRENADES];
new grenadesCount;

new PlayerText:ptxtGrenades[MAX_PLAYERS];
new bool:playerHasM4[MAX_PLAYERS];
new playerGrenadesCount[MAX_PLAYERS];

public OnFilterScriptInit()
{
	CA_Init();
	return 1;
}

public OnPlayerConnect(playerid)
{
    ptxtGrenades[playerid] = CreatePlayerTextDraw(playerid, 629.000000, 421.000000, "Grenades left: 5");
	PlayerTextDrawAlignment(playerid, ptxtGrenades[playerid], 3);
	PlayerTextDrawBackgroundColor(playerid, ptxtGrenades[playerid], 255);
	PlayerTextDrawFont(playerid, ptxtGrenades[playerid], 1);
	PlayerTextDrawLetterSize(playerid, ptxtGrenades[playerid], 0.400000, 2.000000);
	PlayerTextDrawColor(playerid, ptxtGrenades[playerid], -1);
	PlayerTextDrawSetOutline(playerid, ptxtGrenades[playerid], 1);
	PlayerTextDrawSetProportional(playerid, ptxtGrenades[playerid], 1);
	PlayerTextDrawSetSelectable(playerid, ptxtGrenades[playerid], 0);
	return 1;
}

public OnPlayerUpdate(playerid)
{
	if (!playerHasM4[playerid])
	{
	    new w,
	        a;
	    GetPlayerWeaponData(playerid, 5, w, a);
	    if (w == 31)
	    {
	        playerHasM4[playerid] = true;
    		playerGrenadesCount[playerid] = MAX_PLAYER_GRENADES;

    		GameTextForPlayer(playerid, "~y~Press \"~k~~SNEAK_ABOUT~\" to shoot a grenade from launcher", 5000, 3);
	    }
	}

	if (GetPlayerWeapon(playerid) == 31)
	{
	    new string[64];
	    format(string, sizeof(string), "Grenades left: %i", playerGrenadesCount[playerid]);
	    PlayerTextDrawSetString(playerid, ptxtGrenades[playerid], string);
		PlayerTextDrawShow(playerid, ptxtGrenades[playerid]);
	}
	else
	{
		PlayerTextDrawHide(playerid, ptxtGrenades[playerid]);
	}
	return 1;
}

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if (weaponid == 31)
	{
	    if (GetPlayerAmmo(playerid) == 0)
     	{
	    	playerHasM4[playerid] = false;
	    }
	}
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if (PRESSED(KEY_WALK) && (newkeys & 128/*KEY_AIM*/))
    {
        if (GetPlayerWeapon(playerid) == 31)
        {
            if (playerGrenadesCount[playerid] == 0)
            {
            	GameTextForPlayer(playerid, "~r~No M4 Grenades left", 3000, 3);
                return 1;
            }

            if (grenadesCount == MAX_GRENADES)
            {
            	GameTextForPlayer(playerid, "~r~Couldn't launch grenade~n~~r~Try again, launcher might be stuck!", 5000, 3);
             	return 1;
			}

			new Float:fPX,
				Float:fPY,
				Float:fPZ;
	        GetPlayerCameraPos(playerid, fPX, fPY, fPZ);

	        new Float:fVX,
				Float:fVY,
				Float:fVZ;
	        GetPlayerCameraFrontVector(playerid, fVX, fVY, fVZ);

         	const Float:fScale = 5.0;
	        new Float:object_x = fPX + floatmul(fVX, fScale);
	        new Float:object_y = fPY + floatmul(fVY, fScale);
	        new Float:object_z = fPZ + floatmul(fVZ, fScale);

			new proj = CreateProjectile(object_x, object_y, object_z, GRENADE_SPEED * fVX, GRENADE_SPEED * fVY, (GRENADE_SPEED * fVZ) + 5.0, .air_resistance = 1.0, .spherecol_radius = 0.010, .gravity = 15.0);
            if (proj == INVALID_PROJECTILE_ID)
            {
				GameTextForPlayer(playerid, "~r~Couldn't launch grenade~n~~r~Try again, launcher might be stuck!", 5000, 3);
				return 1;
			}

			new obj = CreateDynamicObject(GRENADE_OBJECT, object_x, object_y, object_z + 0.5, 0, 0, 0);
            if (obj == INVALID_OBJECT_ID)
            {
				DestroyProjectile(proj);
            	GameTextForPlayer(playerid, "~r~Couldn't launch grenade~n~~r~Try again, launcher might be stuck!", 5000, 3);
                return 1;
			}

            grenadesObject[grenadesCount++] = obj;
            Streamer_SetIntData(STREAMER_TYPE_OBJECT, obj, E_STREAMER_EXTRA_ID, proj);
			for (new i, j = GetPlayerPoolSize(); i <= j; i++)
			{
				Streamer_UpdateEx(i, object_x, object_y, object_z, .type = STREAMER_TYPE_OBJECT);
			}

            playerGrenadesCount[playerid]--;
        }
    }
    return 1;
}

public OnProjectileUpdate(projid)
{
	new Float:x,
		Float:y,
		Float:z;
	for (new i; i < grenadesCount; i++)
	{
	    if (Streamer_GetIntData(STREAMER_TYPE_OBJECT, grenadesObject[i], E_STREAMER_EXTRA_ID) == projid)
	    {
			GetProjectilePos(projid, x, y, z);
			SetDynamicObjectPos(grenadesObject[i], x, y, z);

			GetProjectileRot(projid, x, y, z);
			SetDynamicObjectRot(grenadesObject[i], x, y, z);
	        break;
	    }
	}
	return 1;
}

public OnProjectileCollide(projid, type, Float:x, Float:y, Float:z, extraid)
{
	for (new i; i < grenadesCount; i++)
	{
	    if (Streamer_GetIntData(STREAMER_TYPE_OBJECT, grenadesObject[i], E_STREAMER_EXTRA_ID) == projid)
		{
		    if (type == PROJECTILE_COLLIDE_PLAYER)
		    {
		        GameTextForPlayer(extraid, "~r~You were hit by a grenade launcher!", 3000, 3);
		    }

	        CreateExplosion(x, y, z, 2, 10.0);
			DestroyProjectile(projid);
			DestroyDynamicObject(grenadesObject[i]);

			for (new a = i, b = --grenadesCount; a < b; a++)
			{
                grenadesObject[a] = grenadesObject[a + 1];
            	Streamer_SetIntData(STREAMER_TYPE_OBJECT, grenadesObject[a], E_STREAMER_EXTRA_ID, Streamer_GetIntData(STREAMER_TYPE_OBJECT, grenadesObject[a + 1], E_STREAMER_EXTRA_ID));
			}
		    break;
		}
	}
	return 1;
}
