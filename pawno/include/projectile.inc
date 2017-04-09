//projectile.inc - By Gammix (originally by PeppeAC) - v1.3.5 - updated: 9 April,2017
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
	forward OnProjectileCollide(projid, type, Float:x, Float:y, Float:z, extraid);
#endif

/*
native Projectile(Float:x, Float:y, Float:z, Float:vx, Float:vy, Float:vz, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, Float:sphere_radius = 1.0, Float:ground_friction = 5.0, Float:collision_friction = 0.2, Float:air_resistance = 0.5, Float:gravity = 10.0, Float:playercol_radius = 0.8);
native IsValidProjectile(projid);
native StopProjectile(projid);
native PushProjectile(projid, Float:vx, Float:vy, Float:vz);
native GetProjectilePos(projid, &Float:x, &Float:y, &Float:z);
native GetProjectileRot(projid, &Float:rx, &Float:ry, &Float:rz);
native GetProjectileVelocity(projid, &Float:vx, &Float:vy, &Float:vz);
native GetProjectilePoolSize();
*/

static iInitializetimer;

enum e_PROJECTILE
{
    bool:e_PROJECTILE_VALID,
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
static iProjectilePoolSize;

stock Projectile(Float:x, Float:y, Float:z, Float:vx, Float:vy, Float:vz, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, Float:sphere_radius = 1.0, Float:ground_friction = 5.0, Float:collision_friction = 0.2, Float:air_resistance = 0.5, Float:gravity = 10.0, Float:playercol_radius = 0.8)
{
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
	
	new id = -1;
	for (new i; i < MAX_PROJECTILES; i++)
	{
	    if (!eProjectile[i][e_PROJECTILE_VALID])
	    {
	        id = i;
			break;
	    }
	}
	
	if (id == -1)
	{
	    printf("[projectile.inc] - Error: Cannot create more than \"%i\" active projectiles. Increase the value of \"MAX_PROJECTILES\" in your script.", MAX_PROJECTILES);
		return -1;
	}
	
	if (id > iProjectilePoolSize)
	{
	    iProjectilePoolSize = id;
	}
 	eProjectile[id][e_PROJECTILE_VALID] = true;
	eProjectile[id][e_PROJECTILE_X] = x;
 	eProjectile[id][e_PROJECTILE_Y] = y;
 	eProjectile[id][e_PROJECTILE_Z] = z;
  	eProjectile[id][e_PROJECTILE_RX] = rx;
    eProjectile[id][e_PROJECTILE_RY] = ry;
    eProjectile[id][e_PROJECTILE_RZ] = rz;
    eProjectile[id][e_PROJECTILE_VX] = vx;
    eProjectile[id][e_PROJECTILE_VY] = vy;
    eProjectile[id][e_PROJECTILE_VZ] = vz;
    eProjectile[id][e_PROJECTILE_RADIUS] = sphere_radius;
    eProjectile[id][e_PROJECTILE_GROUND_FRICTION] = ground_friction;
    eProjectile[id][e_PROJECTILE_COLLISION_FRICTION] = collision_friction;
    eProjectile[id][e_PROJECTILE_AIR_RESISTANCE] = air_resistance;
    eProjectile[id][e_PROJECTILE_GRAVITY] = gravity;
    eProjectile[id][e_PROJECTILE_PLAYERCOL_RADIUS] = playercol_radius;
	return id;
}

stock IsValidProjectile(projid)
{
    if (projid < 0 || projid > iProjectilePoolSize)
        return 0;

	return eProjectile[projid][e_PROJECTILE_VALID];
}

stock StopProjectile(projid)
{
    if (projid < 0 || projid > iProjectilePoolSize)
        return 0;

	if (!eProjectile[projid][e_PROJECTILE_VALID])
	    return 0;

 	eProjectile[projid][e_PROJECTILE_VALID] = false;
	if (projid == iProjectilePoolSize)
	{
	    for (new i = iProjectilePoolSize; i != -1; i--)
	    {
	        if (eProjectile[i][e_PROJECTILE_VALID])
	        {
	            iProjectilePoolSize = i;
	            break;
	        }
	    }
	}

	if (iProjectilePoolSize == 0)
	{
	    KillTimer(iInitializetimer);
	    iInitializetimer = 0;
	}
	return 1;
}

stock PushProjectile(projid, Float:vx, Float:vy, Float:vz)
{
    if (projid < 0 || projid > iProjectilePoolSize)
        return 0;

  	eProjectile[projid][e_PROJECTILE_VX] += vx;
    eProjectile[projid][e_PROJECTILE_VY] += vy;
    eProjectile[projid][e_PROJECTILE_VZ] += vz;
	return 1;
}

stock GetProjectilePos(projid, &Float:x, &Float:y, &Float:z)
{
    if (projid < 0 || projid > iProjectilePoolSize)
        return 0;

    x = eProjectile[projid][e_PROJECTILE_X];
    y = eProjectile[projid][e_PROJECTILE_Y];
    z = eProjectile[projid][e_PROJECTILE_Z];
	return 1;
}

stock GetProjectileRot(projid, &Float:rx, &Float:ry, &Float:rz)
{
    if (projid < 0 || projid > iProjectilePoolSize)
        return 0;

    rx = eProjectile[projid][e_PROJECTILE_RX];
    ry = eProjectile[projid][e_PROJECTILE_RY];
    rz = eProjectile[projid][e_PROJECTILE_RZ];
	return 1;
}

stock GetProjectileVelocity(projid, &Float:vx, &Float:vy, &Float:vz)
{
    if (projid < 0 || projid > iProjectilePoolSize)
        return 0;

    vx = eProjectile[projid][e_PROJECTILE_VX];
    vy = eProjectile[projid][e_PROJECTILE_VY];
    vz = eProjectile[projid][e_PROJECTILE_VZ];
	return 1;
}

stock GetProjectilePoolSize()
{
    return iProjectilePoolSize;
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
		extraid,
		Float:dx,
 		Float:dy,
	 	Float:moveangle,
	 	Float:new_vx,
	 	Float:new_vy,
		Float:speed,
		bool:collision;

	for (new i; i <= iProjectilePoolSize; i++)
	{
	    if (!eProjectile[i][e_PROJECTILE_VALID])
	        continue;
	        
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
			min_height = eProjectile[i][e_PROJECTILE_Z] - 1.0;
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
				eProjectile[i][e_PROJECTILE_VZ] = -eProjectile[i][e_PROJECTILE_VZ] * 0.8;

			#if defined OnProjectileCollide
				OnProjectileCollide(i, 1, new_x, new_y, new_z, 0); // COLLIDE_HIGH_Z_BOUND
			#endif
			new_z = max_height;
		}
		else if (new_z < min_height)
		{
			if (eProjectile[i][e_PROJECTILE_VZ] < 0)
				eProjectile[i][e_PROJECTILE_VZ] = -eProjectile[i][e_PROJECTILE_VZ] * 0.8;

			#if defined OnProjectileCollide
				OnProjectileCollide(i, 2, new_x, new_y, new_z, 0); // COLLIDE_LOW_Z_BOUND
			#endif
			new_z = min_height;
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
		cx = cy = cz = crx = cry = 0.0;
		if ((extraid = CA_RayCastLineAngle(eProjectile[i][e_PROJECTILE_X], eProjectile[i][e_PROJECTILE_Y], eProjectile[i][e_PROJECTILE_Z], new_x, new_y, new_z, cx, cy, cz, crx, cry, unused)) != 0)
		{
			moveangle = atan2(-cry, crx);
			new_vx = ((eProjectile[i][e_PROJECTILE_VX] * floatcos(moveangle, degrees)) - (eProjectile[i][e_PROJECTILE_VY] * floatsin(moveangle, degrees)));
			new_vy = -((eProjectile[i][e_PROJECTILE_VX] * floatsin(moveangle, degrees)) + (eProjectile[i][e_PROJECTILE_VY] * floatcos(moveangle, degrees)));

			moveangle *= -1;
			eProjectile[i][e_PROJECTILE_VX] = ((new_vx * floatcos(moveangle, degrees)) - (new_vy * floatsin(moveangle, degrees)));
			eProjectile[i][e_PROJECTILE_VY] = ((new_vx * floatsin(moveangle, degrees)) + (new_vy * floatcos(moveangle, degrees)));

			moveangle += ((new_vy > 0) ? (90.0) : (-90.0));
			new_x = (cx + (eProjectile[i][e_PROJECTILE_RADIUS] * floatcos(moveangle, degrees)));
			new_y = (cy + (eProjectile[i][e_PROJECTILE_RADIUS] * floatsin(moveangle, degrees)));

	        collision = true;

			#if defined OnProjectileCollide
				OnProjectileCollide(i, 3, cx, cy, cz, extraid); // COLLIDE_SA_WORLD
			#else
				#pragma unused extraid
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
                                moveangle = -atan2(dy, dx);
								new_vx = ((eProjectile[i][e_PROJECTILE_VX] * floatcos(moveangle, degrees)) - (eProjectile[i][e_PROJECTILE_VY] * floatsin(moveangle, degrees)));
								new_vy = ((eProjectile[i][e_PROJECTILE_VX] * floatsin(moveangle, degrees)) + (eProjectile[i][e_PROJECTILE_VY] * floatcos(moveangle, degrees)));

								moveangle *= -1;
								eProjectile[i][e_PROJECTILE_VX] = ((new_vx * floatcos(moveangle, degrees)) - (new_vy * floatsin(moveangle, degrees)));
								eProjectile[i][e_PROJECTILE_VY] = ((new_vx * floatsin(moveangle, degrees)) + (new_vy * floatcos(moveangle, degrees)));

								collision = true;

								#if defined OnProjectileCollide
									OnProjectileCollide(i, 4, cx, cy, cz, a); // COLLIDE_PLAYER
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
		speed = floatsqroot((eProjectile[i][e_PROJECTILE_VX] * eProjectile[i][e_PROJECTILE_VX]) + (eProjectile[i][e_PROJECTILE_VY] * eProjectile[i][e_PROJECTILE_VY]));
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
		#if defined OnProjectileUpdate
			OnProjectileUpdate(i);
		#endif

		// if velocity is 0, stop simulation (KillTimer)
		if (eProjectile[i][e_PROJECTILE_VX] == 0.0 && eProjectile[i][e_PROJECTILE_VY] == 0.0 && ((new_z == min_height && eProjectile[i][e_PROJECTILE_VZ] <= 0.0) || (new_z == max_height && eProjectile[i][e_PROJECTILE_VZ] >= 0.0)))
		{
		    #if defined OnProjectileStop
				OnProjectileStop(i);
			#endif
		    StopProjectile(i);
			return;
		}
	}
}
