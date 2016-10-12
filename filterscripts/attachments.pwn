#include <a_samp>

#include <easydb>//comment this if you want to disable saving attachments

#undef 	MAX_PLAYER_ATTACHED_OBJECTS
#define MAX_PLAYER_ATTACHED_OBJECTS 10 //maximum limit is 10, if you want less number of slots, set the value here

#include <dialogs>
#include <easydialog>
#include <izcmd>

#pragma dynamic (10000)

enum E_ATTACHMENTS
{
    E_ATTACHMENTS_MODEL,
    E_ATTACHMENTS_MODELNAME[25]
};

new const ATTACHMENTS[][E_ATTACHMENTS] =
{
	{18632, "FishingRod"},
	{18633, "GTASAWrench1"},
	{18634, "GTASACrowbar1"},
	{18635, "GTASAHammer1"},
	{18636, "PoliceCap1"},
	{18637, "PoliceShield1"},
	{18638, "HardHat1"},
	{18639, "BlackHat1"},
	{18640, "Hair1"},
	{18975, "Hair2"},
	{19136, "Hair4"},
	{19274, "Hair5"},
	{18641, "Flashlight1"},
	{18642, "Taser1"},
	{18643, "LaserPointer1"},
	{19080, "LaserPointer2"},
	{19081, "LaserPointer3"},
	{19082, "LaserPointer4"},
	{19083, "LaserPointer5"},
	{19084, "LaserPointer6"},
	{18644, "Screwdriver1"},
	{18645, "MotorcycleHelmet1"},
	{18865, "MobilePhone1"},
	{18866, "MobilePhone2"},
	{18867, "MobilePhone3"},
	{18868, "MobilePhone4"},
	{18869, "MobilePhone5"},
	{18870, "MobilePhone6"},
	{18871, "MobilePhone7"},
	{18872, "MobilePhone8"},
	{18873, "MobilePhone9"},
	{18874, "MobilePhone10"},
	{18875, "Pager1"},
	{18890, "Rake1"},
	{18891, "Bandana1"},
	{18892, "Bandana2"},
	{18893, "Bandana3"},
	{18894, "Bandana4"},
	{18895, "Bandana5"},
	{18896, "Bandana6"},
	{18897, "Bandana7"},
	{18898, "Bandana8"},
	{18899, "Bandana9"},
	{18900, "Bandana10"},
	{18901, "Bandana11"},
	{18902, "Bandana12"},
	{18903, "Bandana13"},
	{18904, "Bandana14"},
	{18905, "Bandana15"},
	{18906, "Bandana16"},
	{18907, "Bandana17"},
	{18908, "Bandana18"},
	{18909, "Bandana19"},
	{18910, "Bandana20"},
	{18911, "Mask1"},
	{18912, "Mask2"},
	{18913, "Mask3"},
	{18914, "Mask4"},
	{18915, "Mask5"},
	{18916, "Mask6"},
	{18917, "Mask7"},
	{18918, "Mask8"},
	{18919, "Mask9"},
	{18920, "Mask10"},
	{18921, "Beret1"},
	{18922, "Beret2"},
	{18923, "Beret3"},
	{18924, "Beret4"},
	{18925, "Beret5"},
	{18926, "Hat1"},
	{18927, "Hat2"},
	{18928, "Hat3"},
	{18929, "Hat4"},
	{18930, "Hat5"},
	{18931, "Hat6"},
	{18932, "Hat7"},
	{18933, "Hat8"},
	{18934, "Hat9"},
	{18935, "Hat10"},
	{18936, "Helmet1"},
	{18937, "Helmet2"},
	{18938, "Helmet3"},
	{18939, "CapBack1"},
	{18940, "CapBack2"},
	{18941, "CapBack3"},
	{18942, "CapBack4"},
	{18943, "CapBack5"},
	{18944, "HatBoater1"},
	{18945, "HatBoater2"},
	{18946, "HatBoater3"},
	{18947, "HatBowler1"},
	{18948, "HatBowler2"},
	{18949, "HatBowler3"},
	{18950, "HatBowler4"},
	{18951, "HatBowler5"},
	{18952, "BoxingHelmet1"},
	{18953, "CapKnit1"},
	{18954, "CapKnit2"},
	{18955, "CapOverEye1"},
	{18956, "CapOverEye2"},
	{18957, "CapOverEye3"},
	{18958, "CapOverEye4"},
	{18959, "CapOverEye5"},
	{18960, "CapRimUp1"},
	{18961, "CapTrucker1"},
	{18962, "CowboyHat2"},
	{18963, "CJElvisHead"},
	{18964, "SkullyCap1"},
	{18965, "SkullyCap2"},
	{18966, "SkullyCap3"},
	{18967, "HatMan1"},
	{18968, "HatMan2"},
	{18969, "HatMan3"},
	{18970, "HatTiger1"},
	{18971, "HatCool1"},
	{18972, "HatCool2"},
	{18973, "HatCool3"},
	{18974, "MaskZorro1"},
	{18976, "MotorcycleHelmet2"},
	{18977, "MotorcycleHelmet3"},
	{18978, "MotorcycleHelmet4"},
	{18979, "MotorcycleHelmet5"},
	{19006, "GlassesType1"},
	{19007, "GlassesType2"},
	{19008, "GlassesType3"},
	{19009, "GlassesType4"},
	{19010, "GlassesType5"},
	{19011, "GlassesType6"},
	{19012, "GlassesType7"},
	{19013, "GlassesType8"},
	{19014, "GlassesType9"},
	{19015, "GlassesType10"},
	{19016, "GlassesType11"},
	{19017, "GlassesType12"},
	{19018, "GlassesType13"},
	{19019, "GlassesType14"},
	{19020, "GlassesType15"},
	{19021, "GlassesType16"},
	{19022, "GlassesType17"},
	{19023, "GlassesType18"},
	{19024, "GlassesType19"},
	{19025, "GlassesType20"},
	{19026, "GlassesType21"},
	{19027, "GlassesType22"},
	{19028, "GlassesType23"},
	{19029, "GlassesType24"},
	{19030, "GlassesType25"},
	{19031, "GlassesType26"},
	{19032, "GlassesType27"},
	{19033, "GlassesType28"},
	{19034, "GlassesType29"},
	{19035, "GlassesType30"},
	{19036, "HockeyMask1"},
	{19037, "HockeyMask2"},
	{19038, "HockeyMask3"},
	{19039, "WatchType1"},
	{19040, "WatchType2"},
	{19041, "WatchType3"},
	{19042, "WatchType4"},
	{19043, "WatchType5"},
	{19044, "WatchType6"},
	{19045, "WatchType7"},
	{19046, "WatchType8"},
	{19047, "WatchType9"},
	{19048, "WatchType10"},
	{19049, "WatchType11"},
	{19050, "WatchType12"},
	{19051, "WatchType13"},
	{19052, "WatchType14"},
	{19053, "WatchType15"},
	{19085, "EyePatch1"},
	{19086, "ChainsawDildo1"},
	{19090, "PomPomBlue"},
	{19091, "PomPomRed"},
	{19092, "PomPomGreen"},
	{19093, "HardHat2"},
	{19094, "BurgerShotHat1"},
	{19095, "CowboyHat1"},
	{19096, "CowboyHat3"},
	{19097, "CowboyHat4"},
	{19098, "CowboyHat5"},
	{19099, "PoliceCap2"},
	{19100, "PoliceCap3"},
	{19101, "ArmyHelmet1"},
	{19102, "ArmyHelmet2"},
	{19103, "ArmyHelmet3"},
	{19104, "ArmyHelmet4"},
	{19105, "ArmyHelmet5"},
	{19106, "ArmyHelmet6"},
	{19107, "ArmyHelmet7"},
	{19108, "ArmyHelmet8"},
	{19109, "ArmyHelmet9"},
	{19110, "ArmyHelmet10"},
	{19111, "ArmyHelmet11"},
	{19112, "ArmyHelmet12"},
	{19113, "SillyHelmet1"},
	{19114, "SillyHelmet2"},
	{19115, "SillyHelmet3"},
	{19116, "PlainHelmet1"},
	{19117, "PlainHelmet2"},
	{19118, "PlainHelmet3"},
	{19119, "PlainHelmet4"},
	{19120, "PlainHelmet5"},
	{19137, "CluckinBellHat1"},
	{19138, "PoliceGlasses1"},
	{19139, "PoliceGlasses2"},
	{19140, "PoliceGlasses3"},
	{19141, "SWATHelmet1"},
	{19142, "SWATArmour1"},
	{19160, "HardHat3"},
	{19161, "PoliceHat1"},
	{19162, "PoliceHat2"},
	{19163, "GimpMask1"},
	{19317, "bassguitar01"},
	{19318, "flyingv01"},
	{19319, "warlock01"},
	{19330, "fire_hat01"},
	{19331, "fire_hat02"},
	{19346, "hotdog01"},
	{19347, "badge01"},
	{19348, "cane01"},
	{19349, "monocle01"},
	{19350, "moustache01"},
	{19351, "moustache02"},
	{19352, "tophat01"},
	{19487, "tophat02"},
	{19488, "HatBowler6"},
	{19513, "whitephone"},
	{19578, "Banana"},
	{19418, "HandCuff"},
	{321, "Dildo"},
	{322, "PurpleDildo"},
	{323, "Vibrator"},
    {324, "SilverVibrator"},
	{325, "Flowers"},
	{326, "Cane"},
	{330, "CellPhone"},
	{331, "BrassKnuckle"},
	{333, "GolfClub"},
    {334, "NiteStick"},
	{335, "Knife"},
	{336, "Bat"},
	{337, "Shovel"},
    {338, "PoolCue"},
	{339, "Katana"},
	{341, "Chainsaw"},
	{342, "Grenade"},
    {343, "Teargas"},
	{344, "Molotov"},
	{345, "Missile"},
	{346, "9mm"},
    {347, "Silenced-9mm"},
	{348, "DesertEagle"},
	{349, "Shotgun"},
	{350, "Sawnoff-Shotgun"},
    {351, "Combat-Shotgun"},
	{352, "UZI"},
	{353, "MP5"},
	{354, "Flare"},
    {355, "AK-47"},
	{356, "M4"},
	{357, "Rifle"},
	{358, "SniperRifle"},
    {359, "RPG"},
	{360, "HS-Rocket"},
	{361, "Flamethrower"},
	{362, "Minigun"},
    {363, "SatchelCharge"},
	{364, "Detonator"},
	{365, "Spraycan"},
	{366, "FireEstringuisher"},
    {367, "Camera"},
	{368, "Night-Vision-Goggles"},
	{369, "InfraRed-Vision-Goggles"}
};

enum E_PLAYER_ATTACHMENT
{
		    E_PLAYER_ATTACHMENT_SQLID,
		    E_PLAYER_ATTACHMENT_MODEL,
    		E_PLAYER_ATTACHMENT_BONE,
    Float:	E_PLAYER_ATTACHMENT_X,
    Float:	E_PLAYER_ATTACHMENT_Y,
    Float:	E_PLAYER_ATTACHMENT_Z,
    Float:	E_PLAYER_ATTACHMENT_RX,
    Float:	E_PLAYER_ATTACHMENT_RY,
    Float:	E_PLAYER_ATTACHMENT_RZ,
    Float:	E_PLAYER_ATTACHMENT_SX,
    Float:	E_PLAYER_ATTACHMENT_SY,
    Float:	E_PLAYER_ATTACHMENT_SZ,
		    E_PLAYER_ATTACHMENT_COLOR1,
		    E_PLAYER_ATTACHMENT_COLOR2
};
new p_Attachments[MAX_PLAYERS][MAX_PLAYER_ATTACHED_OBJECTS][E_PLAYER_ATTACHMENT];

RGB(red, green, blue, alpha)
{
	return (red * 16777216) + (green * 65536) + (blue * 256) + alpha;
}

HexToInt(string[])
{
  	if (! string[0])
	{
		return 0;
  	}

  	new
	  	cur = 1,
	  	res = 0
	;
  	for (new i = strlen(string); i > 0; i--)
  	{
    	if (string[i - 1] < 58)
		{
			res = res + cur * (string[i - 1] - 48);
		}
		else
		{
			res = res + cur * (string[i - 1] - 65 + 10);
    	}
		cur = cur * 16;
	}
	return res;
}

#if defined easydb_included
	public OnFilterScriptInit()
	{
	    DB::Init("Server.db");
	    DB::VerifyTable("PlayerAttachments", "ID", true,
	        "Username", STRING, "",
	        "Slot", INTEGER, 0,
	        "Modelid", INTEGER, 0,
	        "Boneid", INTEGER, 0,
	        "X", FLOAT, 0.0,
	        "Y", FLOAT, 0.0,
	        "Z", FLOAT, 0.0,
	        "RX", FLOAT, 0.0,
	        "RY", FLOAT, 0.0,
	        "RZ", FLOAT, 0.0,
	        "SX", FLOAT, 1.0,
	        "SY", FLOAT, 1.0,
	        "SZ", FLOAT, 1.0,
	        "Color1", INTEGER, -1,
	        "Color2", INTEGER, -1);
		return 1;
	}

	public OnFilterScriptExit()
	{
	    DB::Exit();
	    return 1;
	}

	public OnPlayerConnect(playerid)
	{
	    for (new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
	    	p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_SQLID] = -1;

	    new name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, name, MAX_PLAYER_NAME);

		new slot;
		DB::Fetch("PlayerAttachments", MAX_PLAYER_ATTACHED_OBJECTS, _, _, "`Username` = '%q'", name);
		do
		{
		    slot = fetch_int("Slot");
		    if (!(-1 < slot < MAX_PLAYER_ATTACHED_OBJECTS))
		        continue;

		    p_Attachments[playerid][slot][E_PLAYER_ATTACHMENT_SQLID] = fetch_row_id();
		    p_Attachments[playerid][slot][E_PLAYER_ATTACHMENT_MODEL] = fetch_int("Modelid");
		    p_Attachments[playerid][slot][E_PLAYER_ATTACHMENT_BONE] = fetch_int("Boneid");
		    p_Attachments[playerid][slot][E_PLAYER_ATTACHMENT_X] = fetch_float("X");
		    p_Attachments[playerid][slot][E_PLAYER_ATTACHMENT_Y] = fetch_float("Y");
		    p_Attachments[playerid][slot][E_PLAYER_ATTACHMENT_Z] = fetch_float("Z");
		    p_Attachments[playerid][slot][E_PLAYER_ATTACHMENT_RX] = fetch_float("RX");
		    p_Attachments[playerid][slot][E_PLAYER_ATTACHMENT_RY] = fetch_float("RY");
		    p_Attachments[playerid][slot][E_PLAYER_ATTACHMENT_RZ] = fetch_float("RZ");
		    p_Attachments[playerid][slot][E_PLAYER_ATTACHMENT_SX] = fetch_float("SX");
		    p_Attachments[playerid][slot][E_PLAYER_ATTACHMENT_SY] = fetch_float("SY");
		    p_Attachments[playerid][slot][E_PLAYER_ATTACHMENT_SZ] = fetch_float("SZ");
		    p_Attachments[playerid][slot][E_PLAYER_ATTACHMENT_COLOR1] = fetch_int("Color1");
		    p_Attachments[playerid][slot][E_PLAYER_ATTACHMENT_COLOR2] = fetch_int("Color2");
		}
		while (fetch_next_row());
		fetcher_close();
	    return 1;
	}

	public OnPlayerDisconnect(playerid, reason)
	{
	    for (new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
	    {
	        if (p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_SQLID] < 0)
	            continue;

		    DB::Update("PlayerAttachments", p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_SQLID], 1,
		        "Modelid", INTEGER, p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_MODEL],
		        "Boneid", INTEGER, p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_BONE],
		        "X", FLOAT, p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_X],
		        "Y", FLOAT, p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_Y],
		        "Z", FLOAT, p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_Z],
		        "RX", FLOAT, p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_RX],
		        "RY", FLOAT, p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_RY],
		        "RZ", FLOAT, p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_RZ],
		        "SX", FLOAT, p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_SX],
		        "SY", FLOAT, p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_SY],
		        "SZ", FLOAT, p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_SZ],
		        "Color1", INTEGER, p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_COLOR1],
		        "Color2", INTEGER, p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_COLOR2]);
	    }
	    return 1;
	}

	public OnPlayerSpawn(playerid)
	{
	    for (new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
	    {
	        if (p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_SQLID] < 0)
	            continue;

		    SetPlayerAttachedObject(playerid,
				i,
				p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_MODEL],
		        p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_BONE],
		        p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_X],
		        p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_Y],
		        p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_Z],
		        p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_RX],
		        p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_RY],
		        p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_RZ],
		        p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_SX],
		        p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_SY],
		        p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_SZ],
		        p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_COLOR1],
		        p_Attachments[playerid][i][E_PLAYER_ATTACHMENT_COLOR2]);
	    }
	    return 1;
	}
#endif

public OnPlayerEditAttachedObject(playerid, response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ)
{
	p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_MODEL] = modelid;
  	p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_BONE] = boneid;
  	p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_X] = fOffsetX;
  	p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_Y] = fOffsetY;
  	p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_Z] = fOffsetZ;
  	p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_RX] = fRotX;
  	p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_RY] = fRotY;
  	p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_RZ] = fRotZ;
  	p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_SX] = fScaleX;
  	p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_SY] = fScaleY;
  	p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_SZ] = fScaleZ;
  	p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_COLOR1] = -1;
    p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_COLOR2] = -1;

	#if defined easydb_included
		new name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, name, MAX_PLAYER_NAME);

		DB::Fetch("PlayerAttachments", MAX_PLAYER_ATTACHED_OBJECTS, _, _, "`Username` = '%q' AND `Slot` = %i", name, index);
		if (fetch_rows_count())
		{
		    p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_SQLID] = fetch_row_id();
	    	DB::Update("PlayerAttachments", p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_SQLID], 1,
		        "Modelid", INTEGER, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_MODEL],
		        "Boneid", INTEGER, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_BONE],
		        "X", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_X],
		        "Y", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_Y],
		        "Z", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_Z],
		        "RX", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_RX],
		        "RY", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_RY],
		        "RZ", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_RZ],
		        "SX", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_SX],
		        "SY", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_SY],
		        "SZ", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_SZ],
		        "Color1", INTEGER, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_COLOR1],
		        "Color2", INTEGER, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_COLOR2]);

			fetcher_close();
		}
		else
		{
			fetcher_close();

	    	DB::CreateRow("PlayerAttachments",
				"Username", STRING, name,
				"Slot", INTEGER, index,
		        "Modelid", INTEGER, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_MODEL],
		        "Boneid", INTEGER, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_BONE],
		        "X", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_X],
		        "Y", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_Y],
		        "Z", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_Z],
		        "RX", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_RX],
		        "RY", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_RY],
		        "RZ", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_RZ],
		        "SX", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_SX],
		        "SY", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_SY],
		        "SZ", FLOAT, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_SZ],
		        "Color1", INTEGER, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_COLOR1],
		        "Color2", INTEGER, p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_COLOR2]);

            DB::Fetch("PlayerAttachments", MAX_PLAYER_ATTACHED_OBJECTS, _, _, "`Username` = '%q' AND `Slot` = %i", name, index);
		    p_Attachments[playerid][index][E_PLAYER_ATTACHMENT_SQLID] = fetch_row_id();
		    fetcher_close();
		}
	#endif
	return 1;
}

CMD:att(playerid)
{
 	new info[25 * MAX_PLAYER_ATTACHED_OBJECTS];
	for (new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
		format(info, sizeof (info), "%sSlot %i%s", info, i, ((IsPlayerAttachedObjectSlotUsed(playerid, i)) ? ("{00FF00} (Used)\n") : ("\n")));

	Dialog_Show(playerid, DIALOG_ATTACHMENTS, DIALOG_STYLE_LIST, "Select an attachment slot to modify...", info, "Select", "Cancel");
	return 1;
}

Dialog:DIALOG_ATTACHMENTS(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	SetPVarInt(playerid, "AttachmentSlot", listitem);

	if (IsPlayerAttachedObjectSlotUsed(playerid, listitem))
	{
		new caption[100];
		format(caption, sizeof (caption), "Attachment already in use {00FF00}[slot %i]", listitem);

		Dialog_Show(playerid, DIALOG_MODIFY_ATTACHMENT, DIALOG_STYLE_LIST, caption, "Edit Object\nEdit Model & Bone\nEdit Material Color1\nEdit Material Color2\nDuplicate Object\n{FF0000}Delete Object", "Select", "Cancel");
	}
	else
	{
		new caption[100];
		format(caption, sizeof (caption), "[1/3] Select model for attachment {00FF00}[slot %i]", listitem);

		new string[sizeof (ATTACHMENTS) * 25];
		for (new i, j = sizeof (ATTACHMENTS); i < j; i++)
	    	format(string, sizeof (string), "%s%i\t%s\n", string, ATTACHMENTS[i][E_ATTACHMENTS_MODEL], ATTACHMENTS[i][E_ATTACHMENTS_MODELNAME]);

		Dialog_Show(playerid, DIALOG_SELECT_MODEL, DIALOG_STYLE_PREVMODEL, caption, string, "Continue", "Back");
	}
	return 1;
}

Dialog:DIALOG_MODIFY_ATTACHMENT(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return 1;

	switch (listitem)
	{
	    case 0:
	    {
			EditAttachedObject(playerid, GetPVarInt(playerid, "AttachmentSlot"));
			DeletePVar(playerid, "AttachmentSlot");

    		GameTextForPlayer(playerid, "~w~Use ~y~~k~~PED_SPRINT~ ~w~to look around while editing~n~~w~Press ~y~ESC ~w~to quick save and quit editing", 5000, 3);
	    }

	    case 1:
	    {
	        new caption[100];
			format(caption, sizeof (caption), "[Modify - 1/2] Select model for attachment {00FF00}[slot %i]", listitem);

			new string[sizeof (ATTACHMENTS) * 25];
			for (new i, j = sizeof (ATTACHMENTS); i < j; i++)
		    	format(string, sizeof (string), "%s%i\t%s\n", string, ATTACHMENTS[i][E_ATTACHMENTS_MODEL], ATTACHMENTS[i][E_ATTACHMENTS_MODELNAME]);

			Dialog_Show(playerid, DIALOG_MODIFY_MODEL, DIALOG_STYLE_PREVMODEL, caption, string, "Modify", "Back");
		}

	    case 2, 3:
	    {
	        new caption[100];
			format(caption, sizeof (caption), "[Modify] Select material color%s for attachment {00FF00}[slot %i]", ((listitem == 2) ? ("1") : ("2")), listitem);

			SetPVarInt(playerid, "AttachmentColorSlot", ((listitem == 2) ? (0) : (1)));

			new info[1024];
			info = "USE CUSOTM COLOR\n\
				{FFFFFF}White\n\
				{000000}Black\n\
				{808080}Grey\n\
				{008080}Teal\n\
				{003366}Navy blue\n\
				{3366CC}Sky blue\n\
				{000099}Dark blue\n\
				{3399FF}Light blue\n\
				{6600CC}Dark purple\n\
				{6600FF}Purple\n";

			strcat(info, "{6666FF}Light purple\n\
				{00FFFF}Cyan\n\
				{00FFCC}Aqua\n\
				{00CC99}Poision green\n\
				{006666}Lawn green\n\
				{00CC00}Green\n\
				{CC99FF}Pink\n\
				{FF99FF}Hot pink\n\
				{FFFF99}Light yellow\n\
				{FFFF66}Yellow\n");

			strcat(info, "{FF9933}Orange\n\
				{660033}Magenta\n\
				{800000}Marone\n\
				{CC0000}Dark red\n\
				{999966}Khaki\n\
				{993333}Coral\n\
				{CCFF99}Lime\n\
				{663300}Brown\n\
				{A9C4E4}SA-MP Blue");

		  	Dialog_Show(playerid, DIALOG_MODIFY_COLOR, DIALOG_STYLE_LIST, caption, info, "Modify", "Back");
	    }

	    case 4:
	    {
	        new caption[100];
			format(caption, sizeof (caption), "[Duplicate slot %i] Select the slot for attachment", listitem);

		 	new info[50 * MAX_PLAYER_ATTACHED_OBJECTS];
			for (new i; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
				format(info, sizeof (info), "%sDuplicate to slot %i%s", info, i, ((IsPlayerAttachedObjectSlotUsed(playerid, i)) ? ("{00FF00} (Used)\n") : ("\n")));

			Dialog_Show(playerid, DIALOG_DUPLICATE_ATT, DIALOG_STYLE_LIST, caption, info, "Select", "Cancel");
	    }

	    case 5:
	    {
	        new caption[100];
			format(caption, sizeof (caption), "[Delete] Confirm deletion of attachment {00FF00}[slot %i]", listitem);

			Dialog_Show(playerid, DIALOG_DELETE_ATT, DIALOG_STYLE_MSGBOX, caption, "{FFFFFF}Are you sure you want to delete the attachment?", "Yes", "No");
	    }
	}
	return 1;
}

Dialog:DIALOG_MODIFY_MODEL(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return dialog_DIALOG_ATTACHMENTS(playerid, 1, GetPVarInt(playerid, "AttachmentSlot"), "");

    SetPVarInt(playerid, "AttachmentModel", ATTACHMENTS[listitem][E_ATTACHMENTS_MODEL]);

	new caption[100];
	format(caption, sizeof (caption), "[Modify - 2/2] Select bone for attachment {00FF00}[slot %i]", GetPVarInt(playerid, "AttachmentSlot"));

	Dialog_Show(playerid, DIALOG_MODIFY_BONE, DIALOG_STYLE_TABLIST, caption, "Spine\nHead\nLeft upper arm\nRight upper arm\nLeft hand\nRight hand\nLeft thigh\nRight thigh\nLeft foot\nRight foot\nRight calf\nLeft calf\nLeft forearm\nRight forearm\nLeft clavicle\nRight clavicle\nNeck\nJaw", "Continue", "Back");
	return 1;
}

Dialog:DIALOG_MODIFY_BONE(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return dialog_DIALOG_MODIFY_ATTACHMENT(playerid, 1, 1, "");

    p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_MODEL] = GetPVarInt(playerid, "AttachmentModel");
    p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_BONE] = (listitem+1);

	SetPlayerAttachedObject(playerid,
		GetPVarInt(playerid, "AttachmentSlot"),
		p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_MODEL],
        p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_BONE],
        p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_X],
        p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_Y],
        p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_Z],
        p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_RX],
        p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_RY],
        p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_RZ],
        p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_SX],
        p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_SY],
        p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_SZ],
        p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_COLOR1],
        p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_COLOR2]);

	DeletePVar(playerid, "AttachmentSlot");
	DeletePVar(playerid, "AttachmentModel");

    GameTextForPlayer(playerid, "~w~Attachment model and bone modified", 5000, 3);
	return 1;
}

Dialog:DIALOG_MODIFY_COLOR(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return dialog_DIALOG_ATTACHMENTS(playerid, 1, GetPVarInt(playerid, "AttachmentSlot"), "");

	new newcolor;
	switch (listitem)
	{
		case 0:
		{
		    new caption[100];
			format(caption, sizeof (caption), "[Modify] Insert custom color%s for attachment {00FF00}[slot %i]", ((GetPVarInt(playerid, "AttachmentColorSlot") == 0) ? ("1") : ("2")), GetPVarInt(playerid, "AttachmentSlot"));

			Dialog_Show(playerid, DIALOG_MODIFY_CUSTOM_COL, DIALOG_STYLE_INPUT, caption, "{FFFFFF}Insert an hex color value to set it as your object's material color:", "Modify", "Back");
			return 1;
		}
		case 1: newcolor = 0xFFFFFFFF, GameTextForPlayer(playerid, "~w~Attachment color changed to White", 5000, 3);
		case 2: newcolor = 0x000000FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Black", 5000, 3);
		case 3: newcolor = 0x808080FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Grey", 5000, 3);
		case 4: newcolor = 0x008080FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Teal", 5000, 3);
		case 5: newcolor = 0x003366FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Navy blue", 5000, 3);
		case 6: newcolor = 0x3366CCFF, GameTextForPlayer(playerid, "~w~Attachment color changed to Sky blue", 5000, 3);
		case 7: newcolor = 0x000099FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Dark blue", 5000, 3);
		case 8: newcolor = 0x3399FFFF, GameTextForPlayer(playerid, "~w~Attachment color changed to Light blue", 5000, 3);
		case 9: newcolor = 0x6600CCFF, GameTextForPlayer(playerid, "~w~Attachment color changed to Dark purple", 5000, 3);
		case 10: newcolor = 0x6600FFFF, GameTextForPlayer(playerid, "~w~Attachment color changed to Purple", 5000, 3);
		case 11: newcolor = 0x6666FFFF, GameTextForPlayer(playerid, "~w~Attachment color changed to Light purple", 5000, 3);
		case 12: newcolor = 0x00FFFFFF, GameTextForPlayer(playerid, "~w~Attachment color changed to Cyan", 5000, 3);
		case 13: newcolor = 0x00FFCCFF, GameTextForPlayer(playerid, "~w~Attachment color changed to Aqua", 5000, 3);
		case 14: newcolor = 0x00CC99FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Poision green", 5000, 3);
		case 15: newcolor = 0x006666FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Lawn green", 5000, 3);
		case 16: newcolor = 0x00CC00FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Green", 5000, 3);
		case 17: newcolor = 0xCC99FFFF, GameTextForPlayer(playerid, "~w~Attachment color changed to Pink", 5000, 3);
		case 18: newcolor = 0xFF99FFFF, GameTextForPlayer(playerid, "~w~Attachment color changed to Hot pink", 5000, 3);
		case 19: newcolor = 0xFFFF99FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Light yellow", 5000, 3);
		case 20: newcolor = 0xFFFF66FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Yellow", 5000, 3);
		case 21: newcolor = 0xFF9933FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Orange", 5000, 3);
		case 22: newcolor = 0x660033FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Magenta", 5000, 3);
		case 23: newcolor = 0x800000FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Marone", 5000, 3);
		case 24: newcolor = 0xFF0000FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Red", 5000, 3);
		case 25: newcolor = 0xCC0000FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Dark red", 5000, 3);
		case 26: newcolor = 0x999966FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Khaki", 5000, 3);
		case 27: newcolor = 0x993333FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Coral", 5000, 3);
		case 28: newcolor = 0xCCFF99FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Lime", 5000, 3);
		case 29: newcolor = 0x663300FF, GameTextForPlayer(playerid, "~w~Attachment color changed to Brown", 5000, 3);
		case 30: newcolor = 0xA9C4E4FF, GameTextForPlayer(playerid, "~w~Attachment color changed to SAMP Blue", 5000, 3);
	}

	if (GetPVarInt(playerid, "AttachmentColorSlot") == 0)
		p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_COLOR1] = newcolor;
	else
		p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_COLOR2] = newcolor;

	DeletePVar(playerid, "AttachmentSlot");
	DeletePVar(playerid, "AttachmentColorSlot");
	return 1;
}

Dialog:DIALOG_MODIFY_CUSTOM_COL(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return dialog_DIALOG_MODIFY_ATTACHMENT(playerid, 1, ((GetPVarInt(playerid, "AttachmentColorSlot") == 0) ? (2) : (3)), "");

    new	red[3], green[3], blue[3], alpha[3];
  	if (inputtext[0] == '0' && inputtext[1] == 'x') //using 0xFFFFFF format
    {
        new len = strlen(inputtext);
       	if (len != 8 && len != 10)
		   return dialog_DIALOG_MODIFY_COLOR(playerid, 1, 0, "");

		format(red, sizeof (red), "%c%c", inputtext[2], inputtext[3]);
		format(green, sizeof (green), "%c%c", inputtext[4], inputtext[5]);
		format(blue, sizeof (blue), "%c%c", inputtext[6], inputtext[7]);

		if (inputtext[8] != '\0')
 			format(alpha, sizeof (alpha), "%c%c", inputtext[8], inputtext[9]);
		else
			alpha = "FF";
  	}
  	else if (inputtext[0] == '#') //using #FFFFFF format
	{
        new len = strlen(inputtext);
	  	if (len != 7 && len != 9)
			return dialog_DIALOG_MODIFY_COLOR(playerid, 1, 0, "");

		format(red, sizeof (red), "%c%c", inputtext[1], inputtext[2]);
 		format(green, sizeof (green), "%c%c", inputtext[3], inputtext[4]);
   		format(blue, sizeof (blue), "%c%c", inputtext[5], inputtext[6]);

		if (inputtext[7] != '\0')
 			format(alpha, sizeof (alpha), "%c%c", inputtext[7], inputtext[8]);
		else
			alpha = "FF";
	}
	else //using FFFFFF format
 	{
        new len = strlen(inputtext);
		if (len != 6 && len != 8)
			return dialog_DIALOG_MODIFY_COLOR(playerid, 1, 0, "");

   		format(red, sizeof (red), "%c%c", inputtext[0], inputtext[1]);
    	format(green, sizeof (green), "%c%c", inputtext[2], inputtext[3]);
	  	format(blue, sizeof (blue), "%c%c", inputtext[4], inputtext[5]);

		if (inputtext[6] != '\0')
  			format(alpha, sizeof (alpha), "%c%c", inputtext[6], inputtext[7]);
		else
			alpha = "FF";
	}

	new newcolor = RGB(HexToInt(red), HexToInt(green), HexToInt(blue), HexToInt(alpha));
	GameTextForPlayer(playerid, "~w~Attachment color changed (custom color)", 5000, 3);

	if (GetPVarInt(playerid, "AttachmentColorSlot") == 0)
		p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_COLOR1] = newcolor;
	else
		p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_COLOR2] = newcolor;

	DeletePVar(playerid, "AttachmentSlot");
	DeletePVar(playerid, "AttachmentColorSlot");
	return 1;
}

Dialog:DIALOG_DUPLICATE_ATT(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return dialog_DIALOG_ATTACHMENTS(playerid, 1, GetPVarInt(playerid, "AttachmentSlot"), "");

	if (listitem == GetPVarInt(playerid, "AttachmentSlot"))
		return GameTextForPlayer(playerid, "~w~Duplicating to the same slot makes ~r~no sense~w~!", 5000, 3);

	p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_MODEL] = p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_MODEL];
    p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_BONE] = p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_BONE];
    p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_X] = p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_X];
    p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_Y] = p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_Y];
    p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_Z] = p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_Z];
    p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_RX] = p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_RX];
    p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_RY] = p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_RY];
    p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_RZ] = p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_RZ];
    p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SX] = p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_SX];
    p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SY] = p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_SY];
    p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SZ] = p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_SZ];
    p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_COLOR1] = p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_COLOR1];
    p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_COLOR2] = p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_COLOR2];

    SetPlayerAttachedObject(playerid,
		listitem,
		p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_MODEL],
        p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_BONE],
        p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_X],
        p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_Y],
        p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_Z],
        p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_RX],
        p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_RY],
        p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_RZ],
        p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SX],
        p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SY],
        p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SZ],
        p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_COLOR1],
        p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_COLOR2]);
        
    #if defined easydb_included
		new name[MAX_PLAYER_NAME];
		GetPlayerName(playerid, name, MAX_PLAYER_NAME);

		DB::Fetch("PlayerAttachments", MAX_PLAYER_ATTACHED_OBJECTS, _, _, "`Username` = '%q' AND `Slot` = %i", name, listitem);
		if (fetch_rows_count())
		{
		    p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SQLID] = fetch_row_id();
	    	DB::Update("PlayerAttachments", p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SQLID], 1,
		        "Modelid", INTEGER, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_MODEL],
		        "Boneid", INTEGER, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_BONE],
		        "X", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_X],
		        "Y", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_Y],
		        "Z", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_Z],
		        "RX", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_RX],
		        "RY", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_RY],
		        "RZ", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_RZ],
		        "SX", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SX],
		        "SY", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SY],
		        "SZ", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SZ],
		        "Color1", INTEGER, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_COLOR1],
		        "Color2", INTEGER, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_COLOR2]);

			fetcher_close();
		}
		else
		{
			fetcher_close();

	    	DB::CreateRow("PlayerAttachments",
				"Username", STRING, name,
				"Slot", INTEGER, listitem,
		        "Modelid", INTEGER, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_MODEL],
		        "Boneid", INTEGER, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_BONE],
		        "X", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_X],
		        "Y", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_Y],
		        "Z", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_Z],
		        "RX", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_RX],
		        "RY", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_RY],
		        "RZ", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_RZ],
		        "SX", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SX],
		        "SY", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SY],
		        "SZ", FLOAT, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SZ],
		        "Color1", INTEGER, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_COLOR1],
		        "Color2", INTEGER, p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_COLOR2]);

            DB::Fetch("PlayerAttachments", MAX_PLAYER_ATTACHED_OBJECTS, _, _, "`Username` = '%q' AND `Slot` = %i", name, listitem);
		    p_Attachments[playerid][listitem][E_PLAYER_ATTACHMENT_SQLID] = fetch_row_id();
		    fetcher_close();
		}
	#endif

	DeletePVar(playerid, "AttachmentSlot");

    GameTextForPlayer(playerid, "~w~Attachment has been duplicated", 5000, 3);
	return 1;
}

Dialog:DIALOG_DELETE_ATT(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return dialog_DIALOG_ATTACHMENTS(playerid, 1, GetPVarInt(playerid, "AttachmentSlot"), "");

	#if defined easydb_included
	    DB::DeleteRow("PlayerAttachments", p_Attachments[playerid][GetPVarInt(playerid, "AttachmentSlot")][E_PLAYER_ATTACHMENT_SQLID]);
	#endif

  	RemovePlayerAttachedObject(playerid, GetPVarInt(playerid, "AttachmentSlot"));
	DeletePVar(playerid, "AttachmentSlot");

    GameTextForPlayer(playerid, "~w~Attachment has been duplicated", 5000, 3);
	return 1;
}

Dialog:DIALOG_SELECT_MODEL(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return cmd_att(playerid);

    SetPVarInt(playerid, "AttachmentModel", ATTACHMENTS[listitem][E_ATTACHMENTS_MODEL]);

	new caption[100];
	format(caption, sizeof (caption), "[2/3] Select bone for attachment {00FF00}[slot %i]", GetPVarInt(playerid, "AttachmentSlot"));

	Dialog_Show(playerid, DIALOG_SELECT_BONE, DIALOG_STYLE_TABLIST, caption, "Spine\nHead\nLeft upper arm\nRight upper arm\nLeft hand\nRight hand\nLeft thigh\nRight thigh\nLeft foot\nRight foot\nRight calf\nLeft calf\nLeft forearm\nRight forearm\nLeft clavicle\nRight clavicle\nNeck\nJaw", "Continue", "Back");
	return 1;
}

Dialog:DIALOG_SELECT_BONE(playerid, response, listitem, inputtext[])
{
	if (!response)
	    return dialog_DIALOG_ATTACHMENTS(playerid, 1, GetPVarInt(playerid, "AttachmentSlot"), "");

	SetPlayerAttachedObject(playerid, GetPVarInt(playerid, "AttachmentSlot"), GetPVarInt(playerid, "AttachmentModel"), (listitem+1));
	EditAttachedObject(playerid, GetPVarInt(playerid, "AttachmentSlot"));
	DeletePVar(playerid, "AttachmentSlot");
	DeletePVar(playerid, "AttachmentModel");

    GameTextForPlayer(playerid, "~w~Use ~y~~k~~PED_SPRINT~ ~w~to look around while editing~n~~w~Press ~y~ESC ~w~to quick save and quit editing", 5000, 3);
	return 1;
}
