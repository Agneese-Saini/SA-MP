//projectile.inc - By Gammix (originally by PeppeAC) - v1.0 - updated: 16 Mar,2017
#if defined projectile_included
	#endinput
#endif
#define projectile_included

#include <colandreas>

#if !defined MAX_PROJECTILES
	#define MAX_PROJECTILES \
		100
#endif

#if !defined PROJECTILE_TIMER_INTERVAL
	#define PROJECTILE_TIMER_INTERVAL \
		20
#endif

#if !defined FLOAT_INFINITY
	#define FLOAT_INFINITY \
		Float:0x7F800000
#endif

#if defined OnProjectileUpdate
	forward OnProjectileUpdate(projid);
#endif

#if defined OnProjectileStop
	forward OnProjectileStop(projid);
#endif

#if defined OnProjectileCollide
	forward OnProjectileCollide(projid, type);
#endif

/*
native Projectile(Float:x, Float:y, Float:z, Float:vx, Float:vy, Float:vz, Float:sphere_radius = 1.0, Float:acceleration = 0.0, Float:mass = 1.0, Float:friction = 10.0, Float:air_resistance = 0.5, Float:gravity = 4.1);
native StopProjectile(projid);
native GetProjectilePos(projid, &Float:x, &Float:y, &Float:z);
native GetProjectileVelocity(projid, &Float:vx, &Float:vy, &Float:vz);
*/

enum e_PROJECTILE
{
    Float:e_PROJECTILE_X,
    Float:e_PROJECTILE_Y,
	Float:e_PROJECTILE_Z,
	Float:e_PROJECTILE_VX,
	Float:e_PROJECTILE_VY,
	Float:e_PROJECTILE_VZ,
	Float:e_PROJECTILE_RADIUS,
	Float:e_PROJECTILE_ACCELERATION,
	Float:e_PROJECTILE_MASS,
	Float:e_PROJECTILE_FRICTION,
	Float:e_PROJECTILE_GRAVITY,
	Float:e_PROJECTILE_AIR_RESISTANCE,
	e_PROJECTILE_TIMER
};

static eProjectile[MAX_PROJECTILES][e_PROJECTILE];
static iProjectileCount;

stock Projectile(Float:x, Float:y, Float:z, Float:vx, Float:vy, Float:vz, Float:sphere_radius = 1.0, Float:acceleration = 0.0, Float:mass = 1.0, Float:friction = 10.0, Float:air_resistance = 0.5, Float:gravity = 4.1)
{
	if (iProjectileCount == MAX_PROJECTILES)
	    return -1;

	new i = iProjectileCount++;
    eProjectile[i][e_PROJECTILE_X] = x;
    eProjectile[i][e_PROJECTILE_Y] = y;
    eProjectile[i][e_PROJECTILE_Z] = z;
    eProjectile[i][e_PROJECTILE_VX] = vx;
    eProjectile[i][e_PROJECTILE_VY] = vy;
    eProjectile[i][e_PROJECTILE_VZ] = vz;
    eProjectile[i][e_PROJECTILE_RADIUS] = sphere_radius;
    eProjectile[i][e_PROJECTILE_ACCELERATION] = acceleration;
    eProjectile[i][e_PROJECTILE_MASS] = mass;
    eProjectile[i][e_PROJECTILE_FRICTION] = friction;
    eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] = air_resistance;
    eProjectile[i][e_PROJECTILE_GRAVITY] = gravity;
    eProjectile[i][e_PROJECTILE_TIMER] = SetTimerEx("Internal_OnProjectileUpdate", PROJECTILE_TIMER_INTERVAL, true, "i", i);
	return i;
}

stock StopProjectile(projid)
{
    if (projid < 0 || projid >= iProjectileCount)
        return 0;
        
	KillTimer(eProjectile[projid][e_PROJECTILE_TIMER]);
	
	for (new i = projid; i < (iProjectileCount - 1); i++)
	{
	    eProjectile[i][e_PROJECTILE_X] = eProjectile[i + 1][e_PROJECTILE_X];
	    eProjectile[i][e_PROJECTILE_Y] = eProjectile[i + 1][e_PROJECTILE_Y];
	    eProjectile[i][e_PROJECTILE_Z] = eProjectile[i + 1][e_PROJECTILE_Z];
	    eProjectile[i][e_PROJECTILE_VX] = eProjectile[i + 1][e_PROJECTILE_VX];
	    eProjectile[i][e_PROJECTILE_VY] = eProjectile[i + 1][e_PROJECTILE_VY];
	    eProjectile[i][e_PROJECTILE_VZ] = eProjectile[i + 1][e_PROJECTILE_VZ];
	    eProjectile[i][e_PROJECTILE_RADIUS] = eProjectile[i + 1][e_PROJECTILE_RADIUS];
	    eProjectile[i][e_PROJECTILE_ACCELERATION] = eProjectile[i + 1][e_PROJECTILE_ACCELERATION];
	    eProjectile[i][e_PROJECTILE_MASS] = eProjectile[i + 1][e_PROJECTILE_MASS];
	    eProjectile[i][e_PROJECTILE_FRICTION] = eProjectile[i + 1][e_PROJECTILE_FRICTION];
	    eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] = eProjectile[i + 1][e_PROJECTILE_AIR_RESISTANCE];
	    eProjectile[i][e_PROJECTILE_GRAVITY] = eProjectile[i + 1][e_PROJECTILE_GRAVITY];
	    eProjectile[i][e_PROJECTILE_TIMER] = eProjectile[i + 1][e_PROJECTILE_TIMER];
	}
	iProjectileCount--;
	return 1;
}

stock GetProjectilePos(projid, &Float:x, &Float:y, &Float:z)
{
    if (projid < 0 || projid >= iProjectileCount)
        return 0;

    x = eProjectile[projid][e_PROJECTILE_X];
    y = eProjectile[projid][e_PROJECTILE_Y];
    z = eProjectile[projid][e_PROJECTILE_Z];
	return 1;
}

stock GetProjectileVelocity(projid, &Float:vx, &Float:vy, &Float:vz)
{
    if (projid < 0 || projid >= iProjectileCount)
        return 0;

    vx = eProjectile[projid][e_PROJECTILE_VX];
    vy = eProjectile[projid][e_PROJECTILE_VY];
    vz = eProjectile[projid][e_PROJECTILE_VZ];
	return 1;
}

forward Internal_OnProjectileUpdate(i);
public Internal_OnProjectileUpdate(i)
{
	// new position the sphere will be at after this time interval
    new Float:new_x = eProjectile[i][e_PROJECTILE_X] + eProjectile[i][e_PROJECTILE_VX] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
	new Float:new_y = eProjectile[i][e_PROJECTILE_Y] + eProjectile[i][e_PROJECTILE_VY] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
	new Float:new_z = eProjectile[i][e_PROJECTILE_Z] + eProjectile[i][e_PROJECTILE_VZ] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
	
	// calculate max & min height the sphere can reach
	new Float:unused;
	new Float:min_height;
	CA_RayCastLineAngle(eProjectile[i][e_PROJECTILE_X], eProjectile[i][e_PROJECTILE_Y], eProjectile[i][e_PROJECTILE_Z], eProjectile[i][e_PROJECTILE_X], eProjectile[i][e_PROJECTILE_Y], eProjectile[i][e_PROJECTILE_Z] - 1000.0, unused, unused, min_height, unused, unused, unused);
	if (min_height != 0.0)
		min_height += eProjectile[i][e_PROJECTILE_RADIUS];

	new Float:max_height;
	CA_RayCastLineAngle(eProjectile[i][e_PROJECTILE_X], eProjectile[i][e_PROJECTILE_Y], eProjectile[i][e_PROJECTILE_Z], eProjectile[i][e_PROJECTILE_X], eProjectile[i][e_PROJECTILE_Y], eProjectile[i][e_PROJECTILE_Z] + 1000.0, unused, unused, max_height, unused, unused, unused);
	if (max_height > min_height)
		max_height -= eProjectile[i][e_PROJECTILE_RADIUS];
	else
		max_height = FLOAT_INFINITY;
	
	// check heights (z - component)
    if (new_z > max_height)
	{
		if (eProjectile[i][e_PROJECTILE_VZ] > 0)
		{
			eProjectile[i][e_PROJECTILE_VZ] = 0.0;
			#if defined OnProjectileCollide
				OnProjectileCollide(i, 1); // COLLIDE_HIGH_Z_BOUND
			#endif
		}
		new_z = max_height;
	}
	else if (new_z < min_height)
	{
		if (eProjectile[i][e_PROJECTILE_VZ] < 0)
		{
            eProjectile[i][e_PROJECTILE_VZ] = 0.0;
			#if defined OnProjectileCollide
				OnProjectileCollide(i, 2); // COLLIDE_LOW_Z_BOUND
			#endif
		}
		new_z = min_height;
	}

	if (eProjectile[i][e_PROJECTILE_VX] == 0.0 && eProjectile[i][e_PROJECTILE_VY] == 0.0 && eProjectile[i][e_PROJECTILE_VZ] == 0.0)
	{
	    StopProjectile(i);
	    #if defined OnProjectileStop
			OnProjectileStop(i);
		#endif
		return;
	}
	
	// apply gravitation force
	if (eProjectile[i][e_PROJECTILE_GRAVITY] != 0.0)
	{
	    if (eProjectile[i][e_PROJECTILE_VZ] > 0)
		{
			eProjectile[i][e_PROJECTILE_VZ] -= eProjectile[i][e_PROJECTILE_GRAVITY] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
			if (eProjectile[i][e_PROJECTILE_VZ] < 0)
				eProjectile[i][e_PROJECTILE_VZ] = 0;
		}
		else
			eProjectile[i][e_PROJECTILE_VZ] -= eProjectile[i][e_PROJECTILE_GRAVITY] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
	}
	
	// collision check with SA-World
	new Float:mod = floatsqroot((eProjectile[i][e_PROJECTILE_VX] * eProjectile[i][e_PROJECTILE_VX]) + (eProjectile[i][e_PROJECTILE_VY] * eProjectile[i][e_PROJECTILE_VY]));
	new Float:cx,
		Float:cy,
		Float:cz,
		Float:crx,
		Float:cry,
		Float:crz;
	CA_RayCastLineAngle(new_x, new_y, new_z, (new_x + (eProjectile[i][e_PROJECTILE_RADIUS] * (eProjectile[i][e_PROJECTILE_VX] / mod))), (new_y + (eProjectile[i][e_PROJECTILE_RADIUS] * (eProjectile[i][e_PROJECTILE_VY] / mod))), new_z, cx, cy, cz, crx, cry, crz);
	if ((cx != 0.0 || cy != 0.0 || cz != 0.0) && (crx != 0.0 || cry != 0.0))
	{
		new Float:angle = atan2(-cry, crx);
		new Float:new_vx = ((eProjectile[i][e_PROJECTILE_VX] * floatcos(angle, degrees)) - (eProjectile[i][e_PROJECTILE_VY] * floatsin(angle, degrees)));
		new Float:new_vy = -((eProjectile[i][e_PROJECTILE_VX] * floatsin(angle, degrees)) + (eProjectile[i][e_PROJECTILE_VY] * floatcos(angle, degrees)));

		angle *= -1;
		eProjectile[i][e_PROJECTILE_VX] = ((new_vx * floatcos(angle, degrees)) - (new_vy * floatsin(angle, degrees)));
		eProjectile[i][e_PROJECTILE_VY] = ((new_vx * floatsin(angle, degrees)) + (new_vy * floatcos(angle, degrees)));

		angle += ((new_vy > 0) ? (90.0) : (-90.0));
		new_x = (cx + ((eProjectile[i][e_PROJECTILE_RADIUS] + 0.001) * floatcos(angle, degrees)));
		new_y = (cy + ((eProjectile[i][e_PROJECTILE_RADIUS] + 0.001) * floatsin(angle, degrees)));

		#if defined OnProjectileCollide
			OnProjectileCollide(i, 3); // COLLIDE_SA_WORLD
		#endif
	}

	// apply friction
	new Float:moveangle = (atan2(eProjectile[i][e_PROJECTILE_VY], eProjectile[i][e_PROJECTILE_VX]) - 90.0);
	new Float:speed = floatsqroot((eProjectile[i][e_PROJECTILE_VX] * eProjectile[i][e_PROJECTILE_VX]) + (eProjectile[i][e_PROJECTILE_VY] * eProjectile[i][e_PROJECTILE_VY]));
	if (eProjectile[i][e_PROJECTILE_FRICTION] != 0.0 && new_z == min_height)
	{
		speed -= eProjectile[i][e_PROJECTILE_FRICTION] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
		if (speed < 0.001)
			speed = 0;

		eProjectile[i][e_PROJECTILE_VX] = speed * floatsin(-moveangle, degrees);
		eProjectile[i][e_PROJECTILE_VY] = speed * floatcos(-moveangle, degrees);
	}

	// apply air resistance
	if (eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] != 0.0)
	{
		eProjectile[i][e_PROJECTILE_VX] -= eProjectile[i][e_PROJECTILE_VX] * eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
		eProjectile[i][e_PROJECTILE_VY] -= eProjectile[i][e_PROJECTILE_VY] * eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
		eProjectile[i][e_PROJECTILE_VZ] -= eProjectile[i][e_PROJECTILE_VZ] * eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
	}

	// update position and velocity
	eProjectile[i][e_PROJECTILE_X] = new_x;
	eProjectile[i][e_PROJECTILE_Y] = new_y;
	eProjectile[i][e_PROJECTILE_Z] = new_z;

	eProjectile[i][e_PROJECTILE_VX] += eProjectile[i][e_PROJECTILE_ACCELERATION];
	eProjectile[i][e_PROJECTILE_VY] += eProjectile[i][e_PROJECTILE_ACCELERATION];
	eProjectile[i][e_PROJECTILE_VZ] += eProjectile[i][e_PROJECTILE_ACCELERATION];

	#if defined OnProjectileUpdate
		OnProjectileUpdate(i);
	#endif
}
