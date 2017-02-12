#if defined ss@static_pickups_included
	#endinput
#endif
#define ss@static_pickups_included

/*
CALLBACKS:
forward OnPlayerPickUpStaticPickup(playerid, modelid);
*/

#include "weapon_config"
#include "player_stats"

#if defined OnPlayerPickUpStaticPickup
	forward OnPlayerPickUpStaticPickup(playerid, modelid);
#endif

static iStaticPickup[MAX_PICKUPS];

static PICK_GetModelWeaponID(weaponid)
{
	switch (weaponid)
	{
	    case 331: return 1;
	    case 333: return 2;
	    case 334: return 3;
	    case 335: return 4;
	    case 336: return 5;
	    case 337: return 6;
	    case 338: return 7;
	    case 339: return 8;
	    case 341: return 9;
	    case 321: return 10;
	    case 322: return 11;
	    case 323: return 12;
	    case 324: return 13;
	    case 325: return 14;
	    case 326: return 15;
	    case 342: return 16;
	    case 343: return 17;
	    case 344: return 18;
	    case 346: return 22;
	    case 347: return 23;
	    case 348: return 24;
	    case 349: return 25;
	    case 350: return 26;
	    case 351: return 27;
	    case 352: return 28;
	    case 353: return 29;
	    case 355: return 30;
	    case 356: return 31;
	    case 372: return 32;
	    case 357: return 33;
	    case 358: return 34;
	    case 359: return 35;
	    case 360: return 36;
	    case 361: return 37;
	    case 362: return 38;
	    case 363: return 39;
	    case 364: return 40;
	    case 365: return 41;
	    case 366: return 42;
	    case 367: return 43;
	    case 368: return 44;
	    case 369: return 45;
	    case 371: return 46;
	}
	return 0;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	switch (iStaticPickup[pickupid])
 	{
  		case 1240:
	    {
	        new Float:health;
	        GetPlayerHealth(playerid, health);
     		if (health < 100.0)
	        {
	            SetPlayerHealth(playerid, 100.0);
			}

			#if defined OnPlayerPickUpStaticPickup
	        	return OnPlayerPickUpStaticPickup(playerid, iStaticPickup[pickupid]);
			#else
				return 1;
			#endif
		}

	    case 1242:
	    {
	        new Float:armour;
	        GetPlayerArmour(playerid, armour);
     		if (armour < 100.0)
       		{
	            SetPlayerArmour(playerid, 100.0);
           	}

			#if defined OnPlayerPickUpStaticPickup
	        	return OnPlayerPickUpStaticPickup(playerid, iStaticPickup[pickupid]);
			#else
				return 1;
			#endif
   		}

     	case 321..369, 371, 372:
      	{
       		new weapon = PICK_GetModelWeaponID(iStaticPickup[pickupid]);
			new ammo;
			
       		switch (weapon)
		    {
		        case WEAPON_BRASSKNUCKLE..WEAPON_CANE,
			 		 WEAPON_PARACHUTE:
				{
		       		ammo = 1;
		        }

				case WEAPON_GRENADE..WEAPON_MOLTOV:
		       		ammo = 8;

		        case WEAPON_COLT45:
		       		ammo = 30;

		        case WEAPON_SILENCED,
					 WEAPON_DEAGLE:
		        {
		       		ammo = 10;
		        }

				case WEAPON_SHOTGUN:
		       		ammo = 15;

		        case WEAPON_SAWEDOFF,
					 WEAPON_SHOTGSPA:
		        {
		       		ammo = 10;
		        }

				case WEAPON_UZI,
					 WEAPON_MP5,
					 WEAPON_TEC9:
     			{
		       		ammo = 60;
		        }

				case WEAPON_AK47,
					 WEAPON_M4:
     			{
		       		ammo = 80;
		        }

				case WEAPON_RIFLE:
		       		ammo = 20;

		        case WEAPON_SNIPER:
		       		ammo = 10;

		        case WEAPON_ROCKETLAUNCHER:
		       		ammo = 4;

		        case WEAPON_HEATSEEKER:
		       		ammo = 3;

		        case WEAPON_FLAMETHROWER:
		       		ammo = 100;

		        case WEAPON_MINIGUN:
		       		ammo = 500;

		        case WEAPON_SATCHEL:
		       		ammo = 5;

		        case WEAPON_SPRAYCAN:
		       		ammo = 500;

		        case WEAPON_FIREEXTINGUISHER:
		       		ammo = 100;

		        case WEAPON_CAMERA:
		       		ammo = 36;
		    }

         	if (weapon != -1)
          		GivePlayerWeapon(playerid, weapon, ammo);

			#if defined OnPlayerPickUpStaticPickup
	        	return OnPlayerPickUpStaticPickup(playerid, iStaticPickup[pickupid]);
			#else
				return 1;
			#endif
	    }
	}

	#if defined PICK_OnPlayerPickUpPickup
		return PICK_OnPlayerPickUpPickup(playerid, pickupid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerPickUpPickup
    #undef OnPlayerPickUpPickup
#else
    #define _ALS_OnPlayerPickUpPickup
#endif
#define OnPlayerPickUpPickup PICK_OnPlayerPickUpPickup
#if defined PICK_OnPlayerPickUpPickup
	forward PICK_OnPlayerPickUpPickup(playerid, pickupid);
#endif

forward __SetStaticPickup(pickupid, model);
public  __SetStaticPickup(pickupid, model)
{
    iStaticPickup[pickupid] = model;
}

stock PICK_AddStaticPickup(model, type, Float:X, Float:Y, Float:Z, Virtualworld)
{
	switch (model)
	{
	    case 1240, 1242, 321..369, 371, 372:
		{}

     	default:
     		return AddStaticPickup(model, type, X, Y, Z, Virtualworld);
	}

	new ret = CreatePickup(model, type, X, Y, Z, Virtualworld);
	if (ret == -1)
		return 0;

	CallRemoteFunction("__SetStaticPickup", "ii", ret, model);
	return 1;
}
#if defined _ALS_AddStaticPickup
    #undef AddStaticPickup
#else
    #define _ALS_AddStaticPickup
#endif
#define AddStaticPickup PICK_AddStaticPickup

stock PICK_CreatePickup(model, type, Float:X, Float:Y, Float:Z, Virtualworld)
{
    if (type == 1)
		return CreatePickup(model, type, X, Y, Z, Virtualworld);

	switch (model)
	{
	    case 1240, 1242, 321..369, 371, 372:
	    {}

     	default:
    	 	return CreatePickup(model, type, X, Y, Z, Virtualworld);
	}

	new ret = CreatePickup(model, type, X, Y, Z, Virtualworld);
	if (ret == -1)
		return 0;

	CallRemoteFunction("__SetStaticPickup", "ii", ret, model);
	return 1;
}
#if defined _ALS_CreatePickup
    #undef CreatePickup
#else
    #define _ALS_CreatePickup
#endif
#define CreatePickup PICK_CreatePickup

stock PICK_DestroyPickup(pickupid)
{
	if (iStaticPickup[pickupid])
		CallRemoteFunction("__SetStaticPickup", "ii", pickupid, 0);

	return DestroyPickup(pickupid);
}
#if defined _ALS_DestroyPickup
    #undef DestroyPickup
#else
    #define _ALS_DestroyPickup
#endif
#define DestroyPickup PICK_DestroyPickup
