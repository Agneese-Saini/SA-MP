/********************************************

// projectile.inc || v1.4.0 || Last Updated: 27 Sept, 2018 || Orignally By: PeppeAC || Reworked By: Gammix

*********************************************

// Functions
native CreateProjectile(Float:x, Float:y, Float:z, Float:vx, Float:vy, Float:vz, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, Float:spherecol_radius = 1.0, Float:ground_friction = 5.0, Float:collision_friction = 0.2, Float:air_resistance = 0.5, Float:gravity = 10.0, Float:playercol_radius = 0.8, bool:collide_simulation = false, Float:mass = 1.0);
native IsValidProjectile(projid);
native DestroyProjectile(projid);
native GetProjectilePoolSize();
native GetProjectilePos(projid, &Float:x, &Float:y, &Float:z);
native GetProjectileRot(projid, &Float:rx, &Float:ry, &Float:rz);
native GetProjectileVel(projid, &Float:vx, &Float:vy, &Float:vz);
native UpdateProjectileVel(projid, Float:vx, Float:vy, Float:vz);

*********************************************

// Callbacks
forward OnProjectileUpdate(projid);
forward OnProjectileStop(projid, Float:x, Float:y, Float:z);
forward OnProjectileCollide(projid, type, Float:x, Float:y, Float:z, extraid);

********************************************/

#if defined projectile_included
	#endinput
#endif
#define projectile_included

#tryinclude <colandreas>

#if !defined MAX_PROJECTILES
	#define MAX_PROJECTILES \
		(100)
#endif

#if !defined PROJECTILE_TIMER_INTERVAL
	#define PROJECTILE_TIMER_INTERVAL \
		(20)
#endif

#define FLOAT_INFINITY (Float:0x7F800000)

#define INVALID_PROJECTILE_ID (-1)

#define PROJECTILE_COLLIDE_GROUND (0)
#define PROJECTILE_COLLIDE_CIELING (1)
#define PROJECTILE_COLLIDE_SIMULATION (2)
#define PROJECTILE_COLLIDE_OBJECT (3)
#define PROJECTILE_COLLIDE_PLAYER (4)

enum E_PROJECTILE_DATA {
    Float:PROJECTILE_X,
    Float:PROJECTILE_Y,
    Float:PROJECTILE_Z,
    Float:PROJECTILE_RX,
    Float:PROJECTILE_RY,
    Float:PROJECTILE_RZ,
	Float:PROJECTILE_VX,
	Float:PROJECTILE_VY,
	Float:PROJECTILE_VZ,
	Float:PROJECTILE_GRAVITY,
	Float:PROJECTILE_GROUND_FRICTION,
	Float:PROJECTILE_COLLISION_FRICTION,
	Float:PROJECTILE_AIR_RESISTANCE,
	Float:PROJECTILE_OBJECT_COL_RADIUS,
	Float:PROJECTILE_PLAYER_COL_RADIUS,
	Float:PROJECTILE_MASS,
	bool:PROJECTILE_SIMULATION_COLLISION,
    bool:PROJECTILE_IS_VALID
};

static projectileTimer;
static projectilePoolSize;
static projectileData[MAX_PROJECTILES][E_PROJECTILE_DATA];

#if defined OnProjectileUpdate
	forward OnProjectileUpdate(projid);
#endif

#if defined OnProjectileStop
	forward OnProjectileStop(projid, Float:x, Float:y, Float:z);
#endif

#if defined OnProjectileCollide
	forward OnProjectileCollide(projid, type, Float:x, Float:y, Float:z, extraid);
#endif

static Internal_VonU(Float:vx, Float:vy, Float:vz, Float:ux, Float:uy, Float:uz, &Float:x, &Float:y, &Float:z) {
	new Float:fac = ((vx * ux) + (vy * uy) + (vz * uz)) / ((ux * ux) + (uy * uy) + (uz * uz));
	x = ux * fac;
	y = uy * fac;
	z = uz * fac;
}

stock CreateProjectile(Float:x, Float:y, Float:z, Float:vx, Float:vy, Float:vz, Float:rx = 0.0, Float:ry = 0.0, Float:rz = 0.0, Float:spherecol_radius = 1.0, Float:ground_friction = 5.0, Float:collision_friction = 0.2, Float:air_resistance = 0.5, Float:gravity = 10.0, Float:playercol_radius = 0.8, bool:collide_simulation = false, Float:mass = 1.0)
{
	if (spherecol_radius <= 0.0)
	    return INVALID_PROJECTILE_ID;

	if (playercol_radius < 0.0)
	    return INVALID_PROJECTILE_ID;

	if (projectileTimer == 0) {
	    projectileTimer = SetTimer("Internal_OnProjectilesUpdate", PROJECTILE_TIMER_INTERVAL, true);
	}

	new projid = INVALID_PROJECTILE_ID;
	
	for (new i = 0; i < MAX_PROJECTILES; i++) {
	    if (projectileData[i][PROJECTILE_IS_VALID] == false) {
	        projid = i;
			break;
	    }
	}

	if (projid == INVALID_PROJECTILE_ID)
		return INVALID_PROJECTILE_ID;

	projectileData[projid][PROJECTILE_X] = x;
 	projectileData[projid][PROJECTILE_Y] = y;
 	projectileData[projid][PROJECTILE_Z] = z;
  	projectileData[projid][PROJECTILE_RX] = rx;
    projectileData[projid][PROJECTILE_RY] = ry;
    projectileData[projid][PROJECTILE_RZ] = rz;
    projectileData[projid][PROJECTILE_VX] = vx;
    projectileData[projid][PROJECTILE_VY] = vy;
    projectileData[projid][PROJECTILE_VZ] = vz;
    projectileData[projid][PROJECTILE_GRAVITY] = gravity;
    projectileData[projid][PROJECTILE_GROUND_FRICTION] = ground_friction;
    projectileData[projid][PROJECTILE_COLLISION_FRICTION] = collision_friction;
    projectileData[projid][PROJECTILE_AIR_RESISTANCE] = air_resistance;
    projectileData[projid][PROJECTILE_OBJECT_COL_RADIUS] = spherecol_radius;
    projectileData[projid][PROJECTILE_PLAYER_COL_RADIUS] = playercol_radius;
    projectileData[projid][PROJECTILE_MASS] = mass;
 	projectileData[projid][PROJECTILE_SIMULATION_COLLISION] = collide_simulation;
 	projectileData[projid][PROJECTILE_IS_VALID] = true;

	if (projid > projectilePoolSize) {
	    projectilePoolSize = projid;
 	}

	return projid;
}

stock IsValidProjectile(projid)
{
    if (projid < 0 || projid > projectilePoolSize)
        return 0;

	return _:projectileData[projid][PROJECTILE_IS_VALID];
}

stock DestroyProjectile(projid)
{
    if (IsValidProjectile(projid) == 0)
        return 0;

 	projectileData[projid][PROJECTILE_IS_VALID] = false;

	for (new i = projid - 1; i > -1; i--) {
		if (projectileData[i][PROJECTILE_IS_VALID] == true) {
            projectilePoolSize = i;
            break;
		}
	}

	if (projectilePoolSize == projid) {
	    KillTimer(projectileTimer);

        projectilePoolSize = -1;
        projectileTimer = 0;
	}

	return 1;
}

stock GetProjectilePoolSize()
{
    return projectilePoolSize;
}

stock GetProjectilePos(projid, &Float:x, &Float:y, &Float:z)
{
    if (IsValidProjectile(projid) == 0)
        return 0;

    x = projectileData[projid][PROJECTILE_X];
    y = projectileData[projid][PROJECTILE_Y];
    z = projectileData[projid][PROJECTILE_Z];

	return 1;
}

stock GetProjectileRot(projid, &Float:rx, &Float:ry, &Float:rz)
{
    if (IsValidProjectile(projid) == 0)
        return 0;

    rx = projectileData[projid][PROJECTILE_RX];
    ry = projectileData[projid][PROJECTILE_RY];
    rz = projectileData[projid][PROJECTILE_RZ];

	return 1;
}

stock GetProjectileVel(projid, &Float:vx, &Float:vy, &Float:vz)
{
    if (IsValidProjectile(projid) == 0)
        return 0;

    vx = projectileData[projid][PROJECTILE_VX];
    vy = projectileData[projid][PROJECTILE_VY];
    vz = projectileData[projid][PROJECTILE_VZ];

	return 1;
}

stock UpdateProjectileVel(projid, Float:vx, Float:vy, Float:vz)
{
    if (IsValidProjectile(projid) == 0)
        return 0;

    projectileData[projid][PROJECTILE_VX] = vx;
    projectileData[projid][PROJECTILE_VY] = vy;
    projectileData[projid][PROJECTILE_VZ] = vz;

	return 1;
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
 		Float:dz,
	 	Float:moveangle,
	 	Float:new_vx,
	 	Float:new_vy,
	 	Float:new_vz,
		Float:speed,
		bool:collision;

	for (new i = 0; i <= projectilePoolSize; i++)
	{
	    if (projectileData[i][PROJECTILE_IS_VALID] == false)
	        continue;

		collision = false;

		// calculate next position after this timer step
	    new_x = projectileData[i][PROJECTILE_X] + projectileData[i][PROJECTILE_VX] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
		new_y = projectileData[i][PROJECTILE_Y] + projectileData[i][PROJECTILE_VY] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
		new_z = projectileData[i][PROJECTILE_Z] + projectileData[i][PROJECTILE_VZ] * (PROJECTILE_TIMER_INTERVAL / 1000.0);

 		// calculate minimum height the sphere can reach
		if (CA_RayCastLine(projectileData[i][PROJECTILE_X], projectileData[i][PROJECTILE_Y], projectileData[i][PROJECTILE_Z], new_x, new_y, new_z - 1000.0, unused, unused, min_height) != 0)
			min_height += projectileData[i][PROJECTILE_OBJECT_COL_RADIUS];
		else
			min_height = projectileData[i][PROJECTILE_Z] - 1.0;

 		// calculate maximum height the sphere can reach
		if (CA_RayCastLine(projectileData[i][PROJECTILE_X], projectileData[i][PROJECTILE_Y], projectileData[i][PROJECTILE_Z], new_x, new_y, new_z + 1000.0, unused, unused, max_height) != 0) {
			if (max_height > min_height) {
				max_height -= projectileData[i][PROJECTILE_OBJECT_COL_RADIUS];
			} else {
				max_height = FLOAT_INFINITY;
			}
		}
		else {
		    max_height = FLOAT_INFINITY;
		}

		// collision check with max-height possible of a projectile (cieling)
	    if (new_z > max_height) {
			if (projectileData[i][PROJECTILE_VZ] > 0) {
				projectileData[i][PROJECTILE_VZ] = -projectileData[i][PROJECTILE_VZ] * 0.8;
			}

			#if defined OnProjectileCollide
				OnProjectileCollide(i, PROJECTILE_COLLIDE_CIELING, new_x, new_y, new_z, 0); // PROJECTILE_COLLIDE_CIELING
			#endif

			new_z = max_height;
		}
		// collision check with min-height possible of a projectile (ground)
		else if (new_z < min_height) {
			if (projectileData[i][PROJECTILE_VZ] < 0) {
				projectileData[i][PROJECTILE_VZ] = -projectileData[i][PROJECTILE_VZ] * 0.8;
			}

			#if defined OnProjectileCollide
				OnProjectileCollide(i, PROJECTILE_COLLIDE_GROUND, new_x, new_y, new_z, 0); // PROJECTILE_COLLIDE_GROUND
			#endif

			new_z = min_height;
		}

		// apply gravitation force (if gravity is set to non-zero)
		if (projectileData[i][PROJECTILE_GRAVITY] != 0.0) {
			if (projectileData[i][PROJECTILE_VZ] > 0) {
		    	projectileData[i][PROJECTILE_VZ] -= projectileData[i][PROJECTILE_GRAVITY] * (PROJECTILE_TIMER_INTERVAL / 1000.0);

				if (projectileData[i][PROJECTILE_VZ] < 0) {
		    	    projectileData[i][PROJECTILE_VZ] = 0;
				}
			} else {
		    	projectileData[i][PROJECTILE_VZ] -= projectileData[i][PROJECTILE_GRAVITY] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
			}
		}

		// collision check with another simulation/projectile
		if (projectileData[i][PROJECTILE_SIMULATION_COLLISION] == true) {
			for (new x = 0; x <= projectilePoolSize; x++) {
			    if (x != i && projectileData[x][PROJECTILE_IS_VALID] == true && projectileData[x][PROJECTILE_SIMULATION_COLLISION] == true) {
			        dx = projectileData[i][PROJECTILE_X] - projectileData[x][PROJECTILE_X];
			        dy = projectileData[i][PROJECTILE_Y] - projectileData[x][PROJECTILE_Y];
			        dz = projectileData[i][PROJECTILE_Z] - projectileData[x][PROJECTILE_Z];

			        if (((dx * dx) + (dy * dy) + (dz * dz)) < (projectileData[i][PROJECTILE_OBJECT_COL_RADIUS] * projectileData[x][PROJECTILE_OBJECT_COL_RADIUS])) {
			            // MAIN SIMULATION

						new_vx = projectileData[i][PROJECTILE_VX];
						new_vy = projectileData[i][PROJECTILE_VY];
						new_vz = projectileData[i][PROJECTILE_VZ];

						Internal_VonU(projectileData[i][PROJECTILE_VX], projectileData[i][PROJECTILE_VY], projectileData[i][PROJECTILE_VZ], dx, dy, dz, cx, cy, cz);
						new_vx -= cx;
						new_vy -= cy;
						new_vz -= cz;

						Internal_VonU(projectileData[x][PROJECTILE_VX], projectileData[x][PROJECTILE_VY], projectileData[x][PROJECTILE_VZ], -dx, -dy, -dz, cx, cy, cz);
		    			cx = ((projectileData[i][PROJECTILE_MASS] - projectileData[x][PROJECTILE_MASS]) * projectileData[i][PROJECTILE_VX] + 2 * projectileData[x][PROJECTILE_MASS] * cx) / (projectileData[i][PROJECTILE_MASS] + projectileData[x][PROJECTILE_MASS]);
						cy = ((projectileData[i][PROJECTILE_MASS] - projectileData[x][PROJECTILE_MASS]) * projectileData[i][PROJECTILE_VY] + 2 * projectileData[x][PROJECTILE_MASS] * cy) / (projectileData[i][PROJECTILE_MASS] + projectileData[x][PROJECTILE_MASS]);
						cz = ((projectileData[i][PROJECTILE_MASS] - projectileData[x][PROJECTILE_MASS]) * projectileData[i][PROJECTILE_VZ] + 2 * projectileData[x][PROJECTILE_MASS] * cz) / (projectileData[i][PROJECTILE_MASS] + projectileData[x][PROJECTILE_MASS]);
						new_vx += cx;
						new_vy += cy;
						new_vz += cz;

						projectileData[i][PROJECTILE_VX] = new_vx;
						projectileData[i][PROJECTILE_VY] = new_vy;
						projectileData[i][PROJECTILE_VZ] = new_vz;

						// OTHER SIMULATION IN LOOP

						Internal_VonU(projectileData[x][PROJECTILE_VX], projectileData[x][PROJECTILE_VY], projectileData[x][PROJECTILE_VZ], dx, dy, dz, cx, cy, cz);
						new_vx -= cx;
						new_vy -= cy;
						new_vz -= cz;

						Internal_VonU(projectileData[i][PROJECTILE_VX], projectileData[i][PROJECTILE_VY], projectileData[i][PROJECTILE_VZ], -dx, -dy, -dz, cx, cy, cz);
						cx = ((projectileData[x][PROJECTILE_MASS] - projectileData[i][PROJECTILE_MASS]) * projectileData[x][PROJECTILE_VX] + 2 * projectileData[i][PROJECTILE_MASS] * cx) / (projectileData[x][PROJECTILE_MASS] + projectileData[i][PROJECTILE_MASS]);
						cy = ((projectileData[x][PROJECTILE_MASS] - projectileData[i][PROJECTILE_MASS]) * projectileData[x][PROJECTILE_VY] + 2 * projectileData[i][PROJECTILE_MASS] * cy) / (projectileData[x][PROJECTILE_MASS] + projectileData[i][PROJECTILE_MASS]);
						cz = ((projectileData[x][PROJECTILE_MASS] - projectileData[i][PROJECTILE_MASS]) * projectileData[x][PROJECTILE_VZ] + 2 * projectileData[i][PROJECTILE_MASS] * cz) / (projectileData[x][PROJECTILE_MASS] + projectileData[i][PROJECTILE_MASS]);
						new_vx += cx;
						new_vy += cy;
						new_vz += cz;

						projectileData[x][PROJECTILE_VX] = new_vx;
						projectileData[x][PROJECTILE_VY] = new_vy;
						projectileData[x][PROJECTILE_VZ] = new_vz;

						collision = true;

						#if defined OnProjectileCollide
							OnProjectileCollide(i, PROJECTILE_COLLIDE_SIMULATION, dx, dy, dz, x); // PROJECTILE_COLLIDE_SIMULATION
						#endif
					}
			    }
			}
		}

		// collision check with objects
		if ((extraid = CA_RayCastLineAngle(projectileData[i][PROJECTILE_X], projectileData[i][PROJECTILE_Y], projectileData[i][PROJECTILE_Z], new_x, new_y, new_z, cx, cy, cz, crx, cry, unused)) != 0) {
			moveangle = atan2(-cry, crx);
			new_vx = ((projectileData[i][PROJECTILE_VX] * floatcos(moveangle, degrees)) - (projectileData[i][PROJECTILE_VY] * floatsin(moveangle, degrees)));
			new_vy = -((projectileData[i][PROJECTILE_VX] * floatsin(moveangle, degrees)) + (projectileData[i][PROJECTILE_VY] * floatcos(moveangle, degrees)));

			moveangle *= -1;
			projectileData[i][PROJECTILE_VX] = ((new_vx * floatcos(moveangle, degrees)) - (new_vy * floatsin(moveangle, degrees)));
			projectileData[i][PROJECTILE_VY] = ((new_vx * floatsin(moveangle, degrees)) + (new_vy * floatcos(moveangle, degrees)));

			moveangle += ((new_vy > 0) ? (90.0) : (-90.0));
			new_x = (cx + (projectileData[i][PROJECTILE_OBJECT_COL_RADIUS] * floatcos(moveangle, degrees)));
			new_y = (cy + (projectileData[i][PROJECTILE_OBJECT_COL_RADIUS] * floatsin(moveangle, degrees)));

	        collision = true;

			#if defined OnProjectileCollide
				OnProjectileCollide(i, PROJECTILE_COLLIDE_OBJECT, cx, cy, cz, extraid); // PROJECTILE_COLLIDE_OBJECT
			#else
			    #pragma unused extraid
			#endif
		}

		// collision check with players
		if (projectileData[i][PROJECTILE_PLAYER_COL_RADIUS] != 0.0) {
		    #if defined foreach
		        foreach(new a : Player) {
			#else
				for (new a = 0, b = GetPlayerPoolSize(); a <= b; a++) if (IsPlayerConnected(a)) {
			#endif
				GetPlayerPos(a, cx, cy, cz);

		        if ((cz - (min_height - projectileData[i][PROJECTILE_OBJECT_COL_RADIUS])) < new_z < (cz + (max_height + projectileData[i][PROJECTILE_OBJECT_COL_RADIUS]))) {
					dx = new_x - cx;
					dy = new_y - cy;

					if (((dx * dx) + (dy * dy)) < ((projectileData[i][PROJECTILE_OBJECT_COL_RADIUS] + projectileData[i][PROJECTILE_PLAYER_COL_RADIUS]) * (projectileData[i][PROJECTILE_OBJECT_COL_RADIUS] + projectileData[i][PROJECTILE_PLAYER_COL_RADIUS]))) {
						if (((projectileData[i][PROJECTILE_VX] * dx) + (projectileData[i][PROJECTILE_VY] * dy)) < 0.0) {
      						moveangle = -atan2(dy, dx);
							new_vx = ((projectileData[i][PROJECTILE_VX] * floatcos(moveangle, degrees)) - (projectileData[i][PROJECTILE_VY] * floatsin(moveangle, degrees)));
							new_vy = ((projectileData[i][PROJECTILE_VX] * floatsin(moveangle, degrees)) + (projectileData[i][PROJECTILE_VY] * floatcos(moveangle, degrees)));

							moveangle *= -1;
							projectileData[i][PROJECTILE_VX] = ((new_vx * floatcos(moveangle, degrees)) - (new_vy * floatsin(moveangle, degrees)));
							projectileData[i][PROJECTILE_VY] = ((new_vx * floatsin(moveangle, degrees)) + (new_vy * floatcos(moveangle, degrees)));

							collision = true;

							#if defined OnProjectileCollide
								OnProjectileCollide(i, PROJECTILE_COLLIDE_PLAYER, cx, cy, cz, a); // PROJECTILE_COLLIDE_PLAYER
							#endif

							break;
						}
					}
				}
		    }
		}

		// apply collision friction
		moveangle = (atan2(projectileData[i][PROJECTILE_VY], projectileData[i][PROJECTILE_VX]) - 90.0);
		speed = floatsqroot((projectileData[i][PROJECTILE_VX] * projectileData[i][PROJECTILE_VX]) + (projectileData[i][PROJECTILE_VY] * projectileData[i][PROJECTILE_VY]));

		if (projectileData[i][PROJECTILE_COLLISION_FRICTION] != 0.0 && speed > 0.0 && collision) {
			speed -= projectileData[i][PROJECTILE_COLLISION_FRICTION];
			if (speed < 0.001) {
				speed = 0;
			}

			projectileData[i][PROJECTILE_VX] = speed * floatsin(-moveangle, degrees);
			projectileData[i][PROJECTILE_VY] = speed * floatcos(-moveangle, degrees);
		}

		// apply ground friction
		if (projectileData[i][PROJECTILE_GROUND_FRICTION] != 0.0 && speed > 0.0 && new_z == min_height) {
			speed -= projectileData[i][PROJECTILE_GROUND_FRICTION] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
			if (speed < 0.001) {
				speed = 0;
			}

			projectileData[i][PROJECTILE_VX] = speed * floatsin(-moveangle, degrees);
			projectileData[i][PROJECTILE_VY] = speed * floatcos(-moveangle, degrees);
		}

		// apply air resistance
		if (projectileData[i][PROJECTILE_AIR_RESISTANCE] != 0.0) {
			if ((new_z == min_height && floatabs(projectileData[i][PROJECTILE_AIR_RESISTANCE]) > projectileData[i][PROJECTILE_GROUND_FRICTION]) || (collision && floatabs(projectileData[i][PROJECTILE_AIR_RESISTANCE]) > projectileData[i][PROJECTILE_COLLISION_FRICTION]) || new_z > min_height) {
				projectileData[i][PROJECTILE_VX] -= projectileData[i][PROJECTILE_VX] * projectileData[i][PROJECTILE_AIR_RESISTANCE] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
				projectileData[i][PROJECTILE_VY] -= projectileData[i][PROJECTILE_VY] * projectileData[i][PROJECTILE_AIR_RESISTANCE] * (PROJECTILE_TIMER_INTERVAL / 1000.0);
			}
	    }

		// update rotation
		speed = floatsqroot((projectileData[i][PROJECTILE_VX] * projectileData[i][PROJECTILE_VX]) + (projectileData[i][PROJECTILE_VY] * projectileData[i][PROJECTILE_VY]));
		if (speed > 0.0) {
		    projectileData[i][PROJECTILE_RX] -= ((speed * (PROJECTILE_TIMER_INTERVAL / 1000.0)) * ((180.0 / 3.14159) / projectileData[i][PROJECTILE_OBJECT_COL_RADIUS]));
			if (projectileData[i][PROJECTILE_RX] < 0.0) {
				projectileData[i][PROJECTILE_RX] += 360.0;
			}

			projectileData[i][PROJECTILE_RZ] = moveangle;
		}

		// update position
		projectileData[i][PROJECTILE_X] = new_x;
		projectileData[i][PROJECTILE_Y] = new_y;
		projectileData[i][PROJECTILE_Z] = new_z;

		#if defined OnProjectileUpdate
			OnProjectileUpdate(i);
		#endif

		// if velocity is 0, stop simulation (KillTimer)
		if (projectileData[i][PROJECTILE_VX] == 0.0 && projectileData[i][PROJECTILE_VY] == 0.0 && ((new_z == min_height && projectileData[i][PROJECTILE_VZ] <= 0.0) || (new_z == max_height && projectileData[i][PROJECTILE_VZ] >= 0.0))) {
		    #if defined OnProjectileStop
				OnProjectileStop(i, projectileData[i][PROJECTILE_X], projectileData[i][PROJECTILE_Y], projectileData[i][PROJECTILE_Z]);
			#endif

		    DestroyProjectile(i);
		}
	}

	return 1;
}