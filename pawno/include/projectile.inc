//projectile.inc - By Gammix (originally by PeppeAC) - v1.3.2 - updated: 18 Mar,2017
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
	forward OnProjectileCollide(projid, type, Float:x, Float:y, Float:z, modelid);
#endif

/*
native Projectile(Float:x, Float:y, Float:z, Float:vx, Float:vy, Float:vz, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, Float:sphere_radius = 1.0, Float:ground_friction = 10.0, Float:collision_friction = 5.0, Float:air_resistance = 0.5, Float:gravity = 10.0, Float:playercol_radius = 0.8);
native StopProjectile(projid);
native PushProjectile(projid, Float:vx, Float:vy, Float:vz);
native GetProjectilePos(projid, &Float:x, &Float:y, &Float:z);
native GetProjectileRot(projid, &Float:rx, &Float:ry, &Float:rz);
native GetProjectileVelocity(projid, &Float:vx, &Float:vy, &Float:vz);
*/

static iInitializetimer;

enum e_PROJECTILE
{
    Float:e_PROJECTILE_X,
    Float:e_PROJECTILE_Y,
	Float:e_PROJECTILE_Z,
    Float:e_PROJECTILE_RX,
    Float:e_PROJECTILE_RY,
	Float:e_PROJECTILE_RZ,
	Float:e_PROJECTILE_VX,
	Float:e_PROJECTILE_VY,
	Float:e_PROJECTILE_VZ,
	Float:e_PROJECTILE_RADIUS,
	Float:e_PROJECTILE_MASS,
	Float:e_PROJECTILE_GROUND_FRICTION,
	Float:e_PROJECTILE_COLLISION_FRICTION,
	Float:e_PROJECTILE_GRAVITY,
	Float:e_PROJECTILE_AIR_RESISTANCE,
	Float:e_PROJECTILE_PLAYERCOL_RADIUS
};

static eProjectile[MAX_PROJECTILES][e_PROJECTILE];
static iProjectileCount;

stock Projectile(Float:x, Float:y, Float:z, Float:vx, Float:vy, Float:vz, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, Float:sphere_radius = 1.0, Float:ground_friction = 10.0, Float:collision_friction = 5.0, Float:air_resistance = 0.5, Float:gravity = 10.0, Float:playercol_radius = 0.8)
{
	if (iProjectileCount == MAX_PROJECTILES)
	{
	    printf("[projectile.inc] - Error: Cannot create more than \"%i\" active projectiles. Increase the value of \"MAX_PROJECTILES\" in your script.", MAX_PROJECTILES);
	    return -1;
	}

	if (sphere_radius <= 0.0)
	{
	    print("[projectile.inc] - Error: \"sphere_radius\" in function \"Projectile(..)\" cannot be negative or 0.");
	    return -1;
	}

	if (playercol_radius < 0.0)
	{
	    print("[projectile.inc] - Error: \"playercol_radius\" in function \"Projectile(..)\" cannot be negative; use 0.0 to disable player collision.");
	    return -1;
	}

	if (iInitializetimer == 0)
	{
	    iInitializetimer = SetTimer("Internal_OnProjectilesUpdate", PROJECTILE_TIMER_INTERVAL, true);
	}

	new i = iProjectileCount++;
    eProjectile[i][e_PROJECTILE_X] = x;
    eProjectile[i][e_PROJECTILE_Y] = y;
    eProjectile[i][e_PROJECTILE_Z] = z;
    eProjectile[i][e_PROJECTILE_RX] = rx;
    eProjectile[i][e_PROJECTILE_RY] = ry;
    eProjectile[i][e_PROJECTILE_RZ] = rz;
    eProjectile[i][e_PROJECTILE_VX] = vx;
    eProjectile[i][e_PROJECTILE_VY] = vy;
    eProjectile[i][e_PROJECTILE_VZ] = vz;
    eProjectile[i][e_PROJECTILE_RADIUS] = sphere_radius;
    eProjectile[i][e_PROJECTILE_GROUND_FRICTION] = ground_friction;
    eProjectile[i][e_PROJECTILE_COLLISION_FRICTION] = collision_friction;
    eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] = air_resistance;
    eProjectile[i][e_PROJECTILE_GRAVITY] = gravity;
    eProjectile[i][e_PROJECTILE_PLAYERCOL_RADIUS] = playercol_radius;
	return i;
}

stock StopProjectile(projid)
{
    if (projid < 0 || projid >= iProjectileCount)
        return 0;

	for (new i = projid, j = --iProjectileCount; i < j; i++)
	{
	    for (new e_PROJECTILE:x; x < e_PROJECTILE; x++)
	    	eProjectile[i][x] = eProjectile[i + 1][x];
	}
	
	if (iProjectileCount == 0)
	{
	    KillTimer(iInitializetimer);
	    iInitializetimer = 0;
	}
	return 1;
}

stock PushProjectile(projid, Float:vx, Float:vy, Float:vz)
{
    if (projid < 0 || projid >= iProjectileCount)
        return 0;

  	eProjectile[projid][e_PROJECTILE_VX] += vx;
    eProjectile[projid][e_PROJECTILE_VY] += vy;
    eProjectile[projid][e_PROJECTILE_VZ] += vz;
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

stock GetProjectileRot(projid, &Float:rx, &Float:ry, &Float:rz)
{
    if (projid < 0 || projid >= iProjectileCount)
        return 0;

    rx = eProjectile[projid][e_PROJECTILE_RX];
    ry = eProjectile[projid][e_PROJECTILE_RY];
    rz = eProjectile[projid][e_PROJECTILE_RZ];
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

static Internal_Collide(&Float:x, &Float:y, &Float:vx, &Float:vy, Float:sphere_radius, Float:cx, Float:cy, Float:crx, Float:cry)
{
	new Float:angle = atan2(-cry, crx);

	new Float:new_vx = ((vx * floatcos(angle, degrees)) - (vy * floatsin(angle, degrees)));
	new Float:new_vy = -((vx * floatsin(angle, degrees)) + (vy * floatcos(angle, degrees)));

	angle *= -1;
	vx = ((new_vx * floatcos(angle, degrees)) - (new_vy * floatsin(angle, degrees)));
	vy = ((new_vx * floatsin(angle, degrees)) + (new_vy * floatcos(angle, degrees)));
	
	angle += ((new_vy > 0) ? (90.0) : (-90.0));
	x = (cx + ((sphere_radius + 0.001) * floatcos(angle, degrees)));
	y = (cy + ((sphere_radius + 0.001) * floatsin(angle, degrees)));
}

forward Internal_OnProjectilesUpdate();
public Internal_OnProjectilesUpdate()
{
	new Float:unused,
		Float:new_x,
	    Float:new_y,
	    Float:new_z,
		Float:max_height,
	    Float:min_height,
		Float:cx,
		Float:cy,
		Float:cz,
		Float:crx,
		Float:cry,
		modelid,
		Float:dx,
 		Float:dy,
	 	Float:moveangle,
		Float:speed,
		bool:collision;
    
	for (new i; i < iProjectileCount; i++)
	{
		collision = false;

		// calculate next position at timestep
	    new_x = eProjectile[i][e_PROJECTILE_X] + eProjectile[i][e_PROJECTILE_VX] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
		new_y = eProjectile[i][e_PROJECTILE_Y] + eProjectile[i][e_PROJECTILE_VY] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
		new_z = eProjectile[i][e_PROJECTILE_Z] + eProjectile[i][e_PROJECTILE_VZ] * (PROJECTILE_TIMER_INTERVAL / 1000.0);

		// calculate max & min height the sphere can reach
		min_height = 0.0;
		if (CA_RayCastLine(eProjectile[i][e_PROJECTILE_X], eProjectile[i][e_PROJECTILE_Y], eProjectile[i][e_PROJECTILE_Z], new_x, new_y, new_z - 1000.0, unused, unused, min_height) != 0)
		{
			min_height += eProjectile[i][e_PROJECTILE_RADIUS];
		}
		else
		{
			min_height = eProjectile[i][e_PROJECTILE_Z];
		}

		max_height = 0.0;
		if (CA_RayCastLine(eProjectile[i][e_PROJECTILE_X], eProjectile[i][e_PROJECTILE_Y], eProjectile[i][e_PROJECTILE_Z], new_x, new_y, new_z + 1000.0, unused, unused, max_height) != 0)
		{
			if (max_height > min_height)
				max_height -= eProjectile[i][e_PROJECTILE_RADIUS];
			else
				max_height = FLOAT_INFINITY;
		}
		else
		{
		    max_height = FLOAT_INFINITY;
		}

	    if (new_z > max_height)
		{
			if (eProjectile[i][e_PROJECTILE_VZ] > 0)
			{
				eProjectile[i][e_PROJECTILE_VZ] = 0.0;
				#if defined OnProjectileCollide
					OnProjectileCollide(i, 1, new_x, new_y, new_z, 0); // COLLIDE_HIGH_Z_BOUND
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
					OnProjectileCollide(i, 2, new_x, new_y, new_z, 0); // COLLIDE_LOW_Z_BOUND
				#endif
			}
			new_z = min_height;
		}

		// collision check with SA-World
		if ((modelid = CA_RayCastLineAngle(eProjectile[i][e_PROJECTILE_X], eProjectile[i][e_PROJECTILE_Y], eProjectile[i][e_PROJECTILE_Z], new_x, new_y, new_z, cx, cy, cz, crx, cry, unused)) != 0)
		{
		    Internal_Collide(new_x, new_y, eProjectile[i][e_PROJECTILE_VX], eProjectile[i][e_PROJECTILE_VY], eProjectile[i][e_PROJECTILE_RADIUS], cx, cy, crx, cry);
	        collision = true;

			#if defined OnProjectileCollide
				OnProjectileCollide(i, 3, cx, cy, cz, modelid); // COLLIDE_SA_WORLD
			#endif
		}

		// collision check with players
		if (eProjectile[i][e_PROJECTILE_PLAYERCOL_RADIUS] != 0.0)
		{
			for (new a, b = GetPlayerPoolSize(); a <= b; a++)
			{
			    if (IsPlayerConnected(a))
			    {
			        GetPlayerPos(a, cx, cy, cz);
			        if ((cz - (min_height - eProjectile[i][e_PROJECTILE_RADIUS])) < new_z < (cz + (max_height + eProjectile[i][e_PROJECTILE_RADIUS])))
					{
						dx = new_x - cx;
						dy = new_y - cy;
						if (((dx * dx) + (dy * dy)) < ((eProjectile[i][e_PROJECTILE_RADIUS] + eProjectile[i][e_PROJECTILE_PLAYERCOL_RADIUS]) * (eProjectile[i][e_PROJECTILE_RADIUS] + eProjectile[i][e_PROJECTILE_PLAYERCOL_RADIUS])))
						{
							if (((eProjectile[i][e_PROJECTILE_VX] * dx) + (eProjectile[i][e_PROJECTILE_VY] * dy)) < 0.0)
							{
							    Internal_Collide(new_x, new_y, eProjectile[i][e_PROJECTILE_VX], eProjectile[i][e_PROJECTILE_VY], eProjectile[i][e_PROJECTILE_RADIUS], cx, cy, dx, dy);
								collision = true;

								#if defined OnProjectileCollide
									OnProjectileCollide(i, 4, cx, cy, cz, GetPlayerSkin(a)); // COLLIDE_PLAYER
								#endif
							}
						}
					}
			    }
			}
		}

		// apply collision friction
		moveangle = (atan2(eProjectile[i][e_PROJECTILE_VY], eProjectile[i][e_PROJECTILE_VX]) - 90.0);
		speed = floatsqroot((eProjectile[i][e_PROJECTILE_VX] * eProjectile[i][e_PROJECTILE_VX]) + (eProjectile[i][e_PROJECTILE_VY] * eProjectile[i][e_PROJECTILE_VY]));
		if (eProjectile[i][e_PROJECTILE_COLLISION_FRICTION] != 0.0 && speed > 0.0 && collision)
		{
			speed -= eProjectile[i][e_PROJECTILE_COLLISION_FRICTION];
			if (speed < 0.001)
				speed = 0;

			eProjectile[i][e_PROJECTILE_VX] = speed * floatsin(-moveangle, degrees);
			eProjectile[i][e_PROJECTILE_VY] = speed * floatcos(-moveangle, degrees);
		}

		// apply ground friction
		if (eProjectile[i][e_PROJECTILE_GROUND_FRICTION] != 0.0 && speed > 0.0 && new_z == min_height)
		{
			speed -= eProjectile[i][e_PROJECTILE_GROUND_FRICTION] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
			if (speed < 0.001)
				speed = 0;

			eProjectile[i][e_PROJECTILE_VX] = speed * floatsin(-moveangle, degrees);
			eProjectile[i][e_PROJECTILE_VY] = speed * floatcos(-moveangle, degrees);
		}

		// apply gravitation force
		if (eProjectile[i][e_PROJECTILE_GRAVITY] != 0.0)
		{
		    eProjectile[i][e_PROJECTILE_VZ] -= eProjectile[i][e_PROJECTILE_GRAVITY] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
		}

		// apply air resistance
		if (eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] != 0.0)
	    {
			if ((new_z == min_height && floatabs(eProjectile[i][e_PROJECTILE_AIR_RESISTANCE]) > eProjectile[i][e_PROJECTILE_GROUND_FRICTION]) || (collision && floatabs(eProjectile[i][e_PROJECTILE_AIR_RESISTANCE]) > eProjectile[i][e_PROJECTILE_COLLISION_FRICTION]) || new_z > min_height)
			{
				eProjectile[i][e_PROJECTILE_VX] -= eProjectile[i][e_PROJECTILE_VX] * eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
				eProjectile[i][e_PROJECTILE_VY] -= eProjectile[i][e_PROJECTILE_VY] * eProjectile[i][e_PROJECTILE_AIR_RESISTANCE] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
			}
	    }

		// update rotation
		if (speed > 0.0)
		{
		    eProjectile[i][e_PROJECTILE_RX] -= ((speed * (PROJECTILE_TIMER_INTERVAL / 1000.0)) * ((180.0 / 3.14159) / eProjectile[i][e_PROJECTILE_RADIUS]));
			if (eProjectile[i][e_PROJECTILE_RX] < 0.0)
			{
				eProjectile[i][e_PROJECTILE_RX] += 360.0;
			}
			eProjectile[i][e_PROJECTILE_RZ] = moveangle;
		}

		// update position
		eProjectile[i][e_PROJECTILE_X] = new_x;
		eProjectile[i][e_PROJECTILE_Y] = new_y;
		eProjectile[i][e_PROJECTILE_Z] = new_z;

		// if velocity is 0, stop simulation (KillTimer)
		if (eProjectile[i][e_PROJECTILE_VX] == 0.0 && eProjectile[i][e_PROJECTILE_VY] == 0.0 && new_z == min_height)
		{
		    #if defined OnProjectileStop
				OnProjectileStop(i);
			#endif
		    StopProjectile(i);
			return;
		}

		#if defined OnProjectileUpdate
			OnProjectileUpdate(i);
		#endif
	}
}
