/******************************************************/
// map_parser.inc | v1.1 [BETA] | Last Updated: 28 July, 2018 | By Southclaws - Gammix's verion
/******************************************************/

#if defined map_parser_included
	#endinput
#endif
#define map_parser_included

#include <streamer>
#include <sscanf2>

map_parse(const path[]);
map_close(const path[]);
map_num_items(const path[]);
map_get_item(const path[], idx, &item_id, &item_type);

#if !defined MAX_MAPS
	#define MAX_MAPS (20)
#endif

#if !defined MAX_MAP_PATH_LENGTH
	#define MAX_MAP_PATH_LENGTH (128)
#endif

#if !defined MAX_MAP_ITEMS
	#define MAX_MAP_ITEMS (5000)
#endif

#define MAX_REMOVE_BUILDINGS (1000)

enum E_ITEM_DATA {
	ITEM_TYPE,
	ITEM_ID
};

enum E_REMOVE_BUILDING_DATA {
    REMOVE_BUILDING_MODELID,
	Float:REMOVE_BUILDING_X,
	Float:REMOVE_BUILDING_Y,
	Float:REMOVE_BUILDING_Z,
	Float:REMOVE_BUILDING_RANGE
};

static MAP_paths[MAX_MAPS][MAX_MAP_PATH_LENGTH];
static MAP_pathsCount;

static MAP_items[MAX_MAPS][MAX_MAP_ITEMS][E_ITEM_DATA];
static MAP_itemsCount[MAX_MAPS];

static MAX_removeBuildings[MAX_REMOVE_BUILDINGS][E_REMOVE_BUILDING_DATA];
static MAX_removeBuildingsCount;

static MAP_internal_add_item(mapid, itemid, type) {
	if (MAP_itemsCount[mapid] == MAX_MAP_ITEMS) {
		return 0;
	}

	new idx = MAP_itemsCount[mapid]++;
	MAP_items[mapid][idx][ITEM_TYPE] = type;
	MAP_items[mapid][idx][ITEM_ID] = itemid;
	return 1;
}

static stock MAP_internal_find(const path[]) {
	for (new i = 0; i < MAP_pathsCount; i++) {
		if (!strcmp(MAP_paths[i], path, true)) {
			return i;
		}
	}

	return -1;
}

public OnPlayerConnect(playerid) {
	for (new i = 0; i < MAX_removeBuildingsCount; i++) {
		RemoveBuildingForPlayer(playerid, MAX_removeBuildings[i][REMOVE_BUILDING_MODELID], MAX_removeBuildings[i][REMOVE_BUILDING_X], MAX_removeBuildings[i][REMOVE_BUILDING_Y], MAX_removeBuildings[i][REMOVE_BUILDING_Z], MAX_removeBuildings[i][REMOVE_BUILDING_RANGE]);
	}
	
	#if defined MAP_OnPlayerConnect
		return MAP_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect MAP_OnPlayerConnect
#if defined MAP_OnPlayerConnect
	forward MAP_OnPlayerConnect(playerid);
#endif

stock map_parse(const path[]) {
	static const MATERIAL_SIZES[][] = {
		"32x32", "64x32", "64x64", "128x32", "128x64", "128x128", "256x32",
		"256x64", "256x128", "256x256", "512x64", "512x256","512x512"
	};

	if (!fexist(path)) {
		printf("[map_parser.inc] Error: Cannot open map location \"%s\"", path);
		return 0;
 	}

	if (MAP_pathsCount == MAX_MAPS) {
		printf("[map_parser.inc] Error: Exceeding MAX_MAPS limit, cannot open map \"%s\", increase the limit and restart!", path);
		return 0;
	}

	new File:file = fopen(path, io_read);
	if (file == File:0) {
		return 0;
 	}

	new mapid = MAP_pathsCount++;

 	new line[256],
		linenumber = 1,
		funcname[32],
		funcargs[156]
	;

	new modelid,
		Float:x,
		Float:y,
		Float:z,
		Float:rx,
		Float:ry,
		Float:rz,
		worldid,
		interiorid,
		playerid,
		Float:streamdistance,
		Float:drawdistance,
  		type,
		Float:size,
  		attachplayerid,
		attachvehicleid,
  		testlos,
    	idx,
		text[128],
		resname[32],
		res,
		font[32],
		fontsize,
		bold,
		fontcol,
		bgcol,
		align,
		txdname[32],
		texturename[32],
		matcol,
		objectid = -1
	;

	while (fread(file, line)) {
		if (line[0] < 65) {
			++linenumber;
			continue;
		}

	    if (sscanf(line, "p<(>s[32]p<)>s[156]{s[96]}", funcname, funcargs)) {
			++linenumber;
	        continue;
	    }

		// objects
		if (!strcmp(funcname, "CreateDynamicObject", true)) {
			if (!sscanf(funcargs, "p<,>dffffffD(-1)D(-1)D(-1)F("#STREAMER_OBJECT_SD")F("#STREAMER_OBJECT_DD")", modelid, x, y, z, rx, ry, rz, worldid, interiorid, playerid, streamdistance, drawdistance)) {
				objectid = CreateDynamicObject(modelid, x, y, z, rx, ry, rz, worldid, interiorid, playerid, streamdistance, drawdistance);

				if (!MAP_internal_add_item(mapid, objectid, STREAMER_TYPE_OBJECT)) {
					printf("[map_parser.inc] Error(line: %i): Exceeding MAX_MAP_ITEMS limit, cannot load more items in map \"%s\", increase the limit and restart!", linenumber, path);
					break;
				}
			}
		}

		if (!strcmp(funcname, "SetDynamicObjectMaterialText", true)) {
			if (!sscanf(funcargs, "p<,>{s[32]} d p<\">{s[2]}s[32]p<,>{s[2]} s[32] p<\">{s[2]}s[32]p<,>{s[2]} ddxxd", idx, text, resname, font, fontsize, bold, fontcol, bgcol, align)) {
				if (objectid == -1) {
					printf("[map_parser.inc] Warning(line: %i): There is no object before to apply texture on!", linenumber);
					continue;
				}

				new len = strlen(text);

				res = strval(resname[0]);
				if (res == 0) {
					for (new i, j = sizeof(MATERIAL_SIZES); i < j; i++) {
						if (strfind(resname, MATERIAL_SIZES[i]) != -1) {
							res = (i + 1) * 10;
						}
					}
				}

				for (new i; i < len; i++) {
					if (text[i] == '\\' && i != len - 1) {
						if (text[i + 1] == 'n') {
							strdel(text, i, i + 1);
							text[i] = '\n';
						}
					}
				}

				SetDynamicObjectMaterialText(objectid, idx, text, res, font, fontsize, bold, fontcol, bgcol, align);
			}
		}

		if (!strcmp(funcname, "SetDynamicObjectMaterial", true)) {
			if (!sscanf(funcargs, "p<,>{s[16]}dd p<\">{s[1]}s[32]p<,>{s[1]} p<\">{s[1]}s[32]p<,>{s[1]} x", idx, modelid, txdname, texturename, matcol)) {
				if (objectid == -1) {
					printf("[map_parser.inc] Warning(line: %i): There is no object before to apply texture on!", linenumber);
					continue;
				}

				SetDynamicObjectMaterial(objectid, idx, modelid, txdname, texturename, matcol);
			}
		}

		// pickups
		if (!strcmp(funcname, "CreateDynamicPickup", true)) {
			if (!sscanf(funcargs, "p<,>ddfffD(-1)D(-1)D(-1)F("#STREAMER_PICKUP_SD")", modelid, type, x, y, z, worldid, interiorid, playerid, streamdistance)) {
			    new pickupid = CreateDynamicPickup(modelid, type, x, y, z, worldid, interiorid, playerid, streamdistance);

				if (!MAP_internal_add_item(mapid, pickupid, STREAMER_TYPE_PICKUP)) {
					printf("[map_parser.inc] Error(line: %i): Exceeding MAX_MAP_ITEMS limit, cannot load more items in map \"%s\", increase the limit and restart!", linenumber, path);
					break;
				}
			}
		}

		// checkpoint
		if (!strcmp(funcname, "CreateDynamicCP", true)) {
			if (!sscanf(funcargs, "p<,>ffffD(-1)D(-1)D(-1)F("#STREAMER_CP_SD")", x, y, z, size, worldid, interiorid, playerid, streamdistance)) {
			    new checkpointid = CreateDynamicCP(x, y, z, size, worldid, interiorid, playerid, streamdistance);

				if (!MAP_internal_add_item(mapid, checkpointid, STREAMER_TYPE_CP)) {
					printf("[map_parser.inc] Error(line: %i): Exceeding MAX_MAP_ITEMS limit, cannot load more items in map \"%s\", increase the limit and restart!", linenumber, path);
					break;
				}
			}
		}

		// race checkpoint
		if (!strcmp(funcname, "CreateDynamicRaceCP", true)) {
			if (!sscanf(funcargs, "p<,>dfffffffD(-1)D(-1)D(-1)F("#STREAMER_RACE_CP_SD")", type, x, y, z, rx, ry, rz, size, worldid, interiorid, playerid, streamdistance)) {
			    new race_checkpointid = CreateDynamicRaceCP(type, x, y, z, rx, ry, rz, size, worldid, interiorid, playerid, streamdistance);

				if (!MAP_internal_add_item(mapid, race_checkpointid, STREAMER_TYPE_RACE_CP)) {
					printf("[map_parser.inc] Error(line: %i): Exceeding MAX_MAP_ITEMS limit, cannot load more items in map \"%s\", increase the limit and restart!", linenumber, path);
					break;
				}
			}
		}

		// mapicon
		if (!strcmp(funcname, "CreateDynamicMapIcon", true)) {
			if (!sscanf(funcargs, "p<,>fffddD(-1)D(-1)D(-1)F("#STREAMER_MAPICON_SD")D("#MAPICON_LOCAL")", x, y, z, type, fontcol, worldid, interiorid, playerid, streamdistance, res)) {
			    new mapiconid = CreateDynamicMapIcon(x, y, z, type, fontcol, worldid, interiorid, playerid, streamdistance, res);

				if (!MAP_internal_add_item(mapid, mapiconid, STREAMER_TYPE_MAP_ICON)) {
					printf("[map_parser.inc] Error(line: %i): Exceeding MAX_MAP_ITEMS limit, cannot load more items in map \"%s\", increase the limit and restart!", linenumber, path);
					break;
				}
			}
		}

		// 3d text label
		if (!strcmp(funcname, "CreateDynamic3DTextLabel", true)) {
			if (!sscanf(funcargs, "p<,>{s[32]} dffffD(-1)D(-1)D(0)D(-1)D(-1)D(-1)D(-1)F("#STREAMER_3D_TEXT_LABEL_SD")", text, fontcol, x, y, z, drawdistance, attachplayerid, attachvehicleid, testlos, worldid, interiorid, playerid, streamdistance)) {
			    new labelid = _:CreateDynamic3DTextLabel(text, fontcol, x, y, z, drawdistance, attachplayerid, attachvehicleid, testlos, worldid, interiorid, playerid, streamdistance);

				if (!MAP_internal_add_item(mapid, labelid, STREAMER_TYPE_3D_TEXT_LABEL)) {
					printf("[map_parser.inc] Error(line: %i): Exceeding MAX_MAP_ITEMS limit, cannot load more items in map \"%s\", increase the limit and restart!", linenumber, MAP_paths[mapid]);
					break;
				}
			}
		}

		// Remove building
		if (!strcmp(funcname, "RemoveBuildingForPlayer", true)) {
			if (!sscanf(funcargs, "p<,>ddffff", playerid, modelid, x, y, z, size)) {
			    if (MAX_removeBuildingsCount == MAX_REMOVE_BUILDINGS) {
					printf("[map_parser.inc] Warning(line: %i): Cannot remove more than %i buildings for player (map: \"%s\")!", linenumber, MAX_REMOVE_BUILDINGS, path);
					continue;
				}
			    
			    idx = MAX_removeBuildingsCount++;
				MAX_removeBuildings[idx][REMOVE_BUILDING_MODELID] = modelid;
				MAX_removeBuildings[idx][REMOVE_BUILDING_X] = x;
				MAX_removeBuildings[idx][REMOVE_BUILDING_Y] = y;
				MAX_removeBuildings[idx][REMOVE_BUILDING_Z] = z;
				MAX_removeBuildings[idx][REMOVE_BUILDING_RANGE] = size;
				
				for (new i = 0, j = GetPlayerPoolSize(); i <= j; i++) {
					if (IsPlayerConnected(i)) {
                        RemoveBuildingForPlayer(i, modelid, x, y, z, size);
					}
				}
			}
		}

		++linenumber;
	}

	fclose(file);

    MAP_paths[mapid][0] = EOS;
	strcat(MAP_paths[mapid], path);
	
	printf("[map_parser.inc] Success: Map \"%s\" loaded!", MAP_paths[mapid]);
	return 1;
}

stock map_close(const path[]) {
	new mapid = MAP_internal_find(path);
	if (mapid == -1) {
		return 0;
	}

 	for (new i = 0; i < MAP_itemsCount[mapid]; i++) {
 	    switch (MAP_items[mapid][i][ITEM_TYPE]) {
			case STREAMER_TYPE_OBJECT: {
 				DestroyDynamicObject(MAP_items[mapid][i][ITEM_ID]);
			}

			case STREAMER_TYPE_PICKUP: {
 				DestroyDynamicPickup(MAP_items[mapid][i][ITEM_ID]);
			}

			case STREAMER_TYPE_CP: {
 				DestroyDynamicCP(MAP_items[mapid][i][ITEM_ID]);
			}

			case STREAMER_TYPE_RACE_CP: {
 				DestroyDynamicRaceCP(MAP_items[mapid][i][ITEM_ID]);
			}

			case STREAMER_TYPE_MAP_ICON: {
 				DestroyDynamicMapIcon(MAP_items[mapid][i][ITEM_ID]);
			}

			case STREAMER_TYPE_3D_TEXT_LABEL: {
 				DestroyDynamic3DTextLabel(Text3D:MAP_items[mapid][i][ITEM_ID]);
			}
		}

 		MAP_items[mapid][i][ITEM_TYPE] = -1;
 		MAP_items[mapid][i][ITEM_ID] = -1;
 	}

	MAP_itemsCount[mapid] = 0;
	MAP_paths[mapid][0] = EOS;

	for (new i = mapid; i < (MAP_pathsCount - 1); i++) {
		format(MAP_paths[i], MAX_MAP_PATH_LENGTH, MAP_paths[i + 1]);

		for (new x = 0; x < MAP_itemsCount[i + 1]; x++) {
		    MAP_items[i][x] = MAP_items[i + 1][x];
		}
		MAP_itemsCount[i] = MAP_itemsCount[i + 1];
	}
	--MAP_pathsCount;

 	return 1;
}

stock map_num_items(const path[]) {
	new mapid = MAP_internal_find(path);
	if (mapid == -1) {
		return 0;
	}

 	return MAP_itemsCount[mapid];
}

stock map_get_item(const path[], idx, &item_id, &item_type) {
	new mapid = MAP_internal_find(path);
	if (mapid == -1) {
		return 0;
	}

	if (idx < 0 || idx >= MAP_itemsCount[mapid]) {
		return 0;
	}

	item_id = MAP_items[mapid][idx][ITEM_ID];
	item_type = MAP_items[mapid][idx][ITEM_TYPE];
 	return 1;
}
