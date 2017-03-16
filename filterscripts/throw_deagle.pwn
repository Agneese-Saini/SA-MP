#include <a_samp>
#include <zcmd>
#include <projectile>

new iProjObject[MAX_PROJECTILES];

public OnFilterScriptInit()
{
	CA_Init();
	return 1;
}

CMD:test(playerid)
{
	new Float:x, Float:y, Float:z, Float:ang;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, ang);

	new i = Projectile(x, y - 0.5 * floatcos(-(ang + 90.0), degrees), z, 10.0 * floatsin(-ang, degrees), 10.0 * floatcos(-ang, degrees), 4.0, .sphere_radius = 0.10, .gravity = 13.0);
	if (i == -1)
	    return 0;
	DestroyObject(iProjObject[i]);
	iProjObject[i] = CreateObject(348, x, y - 0.5 * floatcos(-(ang + 90.0), degrees), z, 93.7, 120.0, ang + 60.0);
	
	ApplyAnimation(playerid,"GRENADE","WEAPON_throwu",3.0,0,0,0,0,0);
	return 1;
}

public OnProjectileUpdate(projid)
{
	new Float:x, Float:y, Float:z;
	GetProjectilePos(projid, x, y, z);
	SetObjectPos(iProjObject[projid], x, y, z);
	return 1;
}

public OnProjectileStop(projid)
{
	DestroyObject(iProjObject[projid]);
	return 1;
}
