#if defined gangzone_included
	#endinput
#endif
#define gangzone_included

stock GZ_GangZoneCreate(Float:minx, Float:miny, Float:maxx, Float:maxy)
{
	if (minx > maxx)
	{
	    new Float:shift = maxx;
	    maxx = minx;
	    minx = shift;
	}

	if (miny > maxy)
	{
	    new Float:shift = maxy;
	    maxy = miny;
	    miny = shift;
	}
	
	return GangZoneCreate(floatsub(minx, floatfract(minx)), floatsub(miny, floatfract(miny)), floatsub(maxx, floatfract(maxx)), floatsub(maxy, floatfract(maxy)));
}
#if defined _ALS_GangZoneCreate
	#undef GangZoneCreate
#else
	#define _ALS_GangZoneCreate
#endif
#define GangZoneCreate GZ_GangZoneCreate
