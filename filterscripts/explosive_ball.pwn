// Test script for v1.2

#include <a_samp>
#include <zcmd>
#include <projectile>

new iObject;

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

	#define SPEED \
	    50.0
	Projectile(x, y, z, SPEED * floatsin(-ang, degrees), SPEED * floatcos(-ang, degrees), 5.0, .sphere_radius = 0.1, .ground_friction = 100.0, .collision_friction = 9.7);
	iObject = CreateObject(2114, x, y, z, 90, 0, 0);

	ApplyAnimation(playerid,"GRENADE","WEAPON_throwu",3.0,0,0,0,0,0);
	return 1;
}

public OnProjectileUpdate(projid)
{
	new Float:x, Float:y, Float:z;
	GetProjectilePos(projid, x, y, z);
	SetObjectPos(iObject, x, y, z);
	GetProjectileRot(projid, x, y, z);
	SetObjectRot(iObject, x, y, z);
	return 1;
}

public OnProjectileCollide(projid, type)
{
	new Float:x, Float:y, Float:z;
	GetProjectilePos(projid, x, y, z);
	CreateExplosion(x, y, z, 2, 5);
	return 1;
}
