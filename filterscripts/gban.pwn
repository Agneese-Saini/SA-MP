#include <a_samp>
#include <kickbanfix>
#include <zcmd>
#include <easydialog>
#include <sscanf2>
#include <TimestampToDate>

#define COLOR_WHITE (0xFFFFFFFF)
#define COL_WHITE "{FFFFFF}"

#define COLOR_RED (0xFF0400FF)
#define COL_RED "{FF0400}"

#define COLOR_TOMATO (0xFF6347FF)
#define COL_TOMATO "{FF6347}"

#define COLOR_PINK (0xFF0090FF)
#define COL_PINK "{FF0090}"

#define COLOR_GREEN (0x6AFF2AFF)
#define COL_GREEN "{6AFF2A}"

const KICK_DELAY = 50; // in ms

const BAN_MASK = (-1 << (32 - (/*this is the CIDR ip detection range [def: 26]*/26)));

new DB:banDatabase;

new banTargetID[MAX_PLAYERS];
new banTargetDays[MAX_PLAYERS];
new banTargetReason[MAX_PLAYERS][64];

new unbanTargetID[MAX_PLAYERS];
new unbanTargetName[MAX_PLAYERS][MAX_PLAYER_NAME];
new unbanTargetIp[MAX_PLAYERS][18];

new Text:banTextDraw[2];

Ban_GetLongIP(const ip[])
{
  	new len = strlen(ip);
	if (!(len > 0 && len < 17))
	{
    	return 0;
	}

	new count;
	new pos;
	new dest[3];
	new val[4];
	for (new i; i < len; i++)
	{
		if (ip[i] == '.' || i == len)
		{
			strmid(dest, ip, pos, i);
			pos = (i + 1);

		    val[count] = strval(dest);
		    if (!(0 <= val[count] <= 255))
		    {
		        return 0;
			}

			count++;
			if (count > 3)
			{
				return 0;
			}
		}
	}

	if (count != 3)
	{
	    return 0;
	}
	return ((val[0] * 16777216) + (val[1] * 65536) + (val[2] * 256) + (val[3]));
}

ReturnDate(timestamp)
{
	new year, month, day, unused;
	TimestampToDate(timestamp, year, month, day, unused, unused, unused, 0);

	static monthname[15];
	switch (month)
	{
	    case 1: monthname = "January";
	    case 2: monthname = "February";
	    case 3: monthname = "March";
	    case 4: monthname = "April";
	    case 5: monthname = "May";
	    case 6: monthname = "June";
	    case 7: monthname = "July";
	    case 8: monthname = "August";
	    case 9: monthname = "September";
	    case 10: monthname = "October";
	    case 11: monthname = "November";
	    case 12: monthname = "December";
	}

	new date[30];
	format(date, sizeof (date), "%i %s, %i", day, monthname, year);
	return date;
}

ReturnTimelapse(start, till)
{
    new ret[32];
	new seconds = till - start;

	const
		MINUTE = 60,
		HOUR = 60 * MINUTE,
		DAY = 24 * HOUR,
		MONTH = 30 * DAY;

	if (seconds == 1)
		format(ret, sizeof(ret), "a second");
	if (seconds < (1 * MINUTE))
		format(ret, sizeof(ret), "%i seconds", seconds);
	else if (seconds < (2 * MINUTE))
		format(ret, sizeof(ret), "a minute");
	else if (seconds < (45 * MINUTE))
		format(ret, sizeof(ret), "%i minutes", (seconds / MINUTE));
	else if (seconds < (90 * MINUTE))
		format(ret, sizeof(ret), "an hour");
	else if (seconds < (24 * HOUR))
		format(ret, sizeof(ret), "%i hours", (seconds / HOUR));
	else if (seconds < (48 * HOUR))
		format(ret, sizeof(ret), "a day");
	else if (seconds < (30 * DAY))
		format(ret, sizeof(ret), "%i days", (seconds / DAY));
	else if (seconds < (12 * MONTH))
    {
		new months = floatround(seconds / DAY / 30);
      	if (months <= 1)
			format(ret, sizeof(ret), "a month");
      	else
			format(ret, sizeof(ret), "%i months", months);
	}
    else
    {
      	new years = floatround(seconds / DAY / 365);
      	if (years <= 1)
			format(ret, sizeof(ret), "a year");
      	else
			format(ret, sizeof(ret), "%i years", years);
	}
	return ret;
}

public OnFilterScriptInit()
{
	banDatabase = db_open("bans.db");

	db_query(banDatabase, "PRAGMA synchronous = NORMAL");
 	db_query(banDatabase, "PRAGMA journal_mode = WAL");

	db_query(banDatabase, "CREATE TABLE IF NOT EXISTS `users` \
		(`id` INTEGER PRIMARY KEY, \
		`name` VARCHAR(24), \
		`ip` VARCHAR(18), \
		`longip` INTEGER, \
		`expire_timestamp` INTEGER, \
		`register_timestamp` INTEGER, \
		`last_activity_timestamp` INTEGER, \
		`admin` VRACHAR(24), \
		`reason` VARCHAR(128)\
	)");

    banTextDraw[0] = TextDrawCreate(0.000000, 0.000000, "_");
	TextDrawBackgroundColor(banTextDraw[0], 255);
	TextDrawFont(banTextDraw[0], 1);
	TextDrawLetterSize(banTextDraw[0], 0.000000, 480.000000);
	TextDrawColor(banTextDraw[0], -1);
	TextDrawSetOutline(banTextDraw[0], 0);
	TextDrawSetProportional(banTextDraw[0], 1);
	TextDrawSetShadow(banTextDraw[0], 1);
	TextDrawUseBox(banTextDraw[0], 1);
	TextDrawBoxColor(banTextDraw[0], 255);
	TextDrawTextSize(banTextDraw[0], 640.000000, 0.000000);
	TextDrawSetSelectable(banTextDraw[0], 0);

	banTextDraw[1] = TextDrawCreate(310.000000, 200.000000, "~r~You are banned!");
	TextDrawAlignment(banTextDraw[1], 2);
	TextDrawBackgroundColor(banTextDraw[1], 255);
	TextDrawFont(banTextDraw[1], 1);
	TextDrawLetterSize(banTextDraw[1], 1.400000, 8.000000);
	TextDrawColor(banTextDraw[1], -1);
	TextDrawSetOutline(banTextDraw[1], 0);
	TextDrawSetProportional(banTextDraw[1], 1);
	TextDrawSetShadow(banTextDraw[1], 1);
	TextDrawSetSelectable(banTextDraw[1], 0);

	return 1;
}

public OnFilterScriptExit()
{
	db_close(banDatabase);

	return 1;
}

public OnPlayerConnect(playerid)
{
	new name[MAX_PLAYER_NAME];
	new ip[18];
	GetPlayerName(playerid, name, sizeof name);
	GetPlayerIp(playerid, ip, sizeof ip);
	
	new string[150];
	format(string, sizeof string, "SELECT * FROM `users` WHERE `name` = '%s' OR `ip` = '%s' OR (`longip` != 0 AND (`longip` & %i) = %i) LIMIT 1", name, ip, BAN_MASK, (Ban_GetLongIP(ip) & BAN_MASK));
	new DBResult:result = db_query(banDatabase, string);
	if (db_num_rows(result) != 0)
	{
	    if (db_get_field_assoc_int(result, "expire_timestamp") != 0 && db_get_field_assoc_int(result, "expire_timestamp") <= gettime())
	    {
	       	format(string, sizeof string, "DELETE FROM `users` WHERE `id` = %i", db_get_field_assoc_int(result, "id"));
			db_query(banDatabase, string);
			
			format(string, sizeof string, "Welcome back to server, its been %s since your ban was lifted.", ReturnTimelapse(db_get_field_assoc_int(result, "expire_timestamp"), gettime()));
	  		SendClientMessage(playerid, COLOR_GREEN, string);
		}
	    else
	    {
	        for (new i; i < 100; i++)
	        {
	            SendClientMessage(playerid, -1, "");
	        }
	        
	       	new admin[MAX_PLAYER_NAME];
	        db_get_field_assoc(result, "admin", admin, sizeof admin);
	       	new reason[128];
	       	db_get_field_assoc(result, "reason", reason, sizeof reason);

			if (db_get_field_assoc_int(result, "expire_timestamp") == 0)
			{
				format(string, sizeof string, "You are still banned from server | Admin who banned you: %s | Banned on: %s | Timeleft for unban: Never (permanent ban)", admin, ReturnDate(db_get_field_assoc_int(result, "register_timestamp")));
	 			SendClientMessageToAll(COLOR_RED, string);
			}
			else
			{
				format(string, sizeof string, "You are still banned from server | Admin who banned you: %s | Banned on: %s | Timeleft for unban: %s", admin, ReturnDate(db_get_field_assoc_int(result, "register_timestamp")), ReturnTimelapse(gettime(), db_get_field_assoc_int(result, "expire_timestamp")));
	 			SendClientMessageToAll(COLOR_RED, string);
			}
			format(string, sizeof string, "Reason for ban: %s", reason);
 			SendClientMessageToAll(COLOR_RED, string);
 		
			TextDrawShowForPlayer(playerid, banTextDraw[0]);
			TextDrawShowForPlayer(playerid, banTextDraw[1]);
			return Kick(banTargetID[playerid], KICK_DELAY);
		}
	}
	db_free_result(result);
	return 1;
}

CMD:ban(playerid, params[])
{
	new targetid;
	new reason[64];
	new days;
	if (sscanf(params, "uis[64]", targetid, days, reason))
	{
		SendClientMessage(playerid, COLOR_WHITE, "Usage: /ban [id/name] [days] [reason]");
	    return SendClientMessage(playerid, COLOR_WHITE, "Note: 0 days means a permanent ban from server.");
	}

	if (!IsPlayerConnected(targetid))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "Target player isn't online.");
	}

	if (days < 0 || days > 365)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "Number of days cannot be negative or greater than 365 days i.e. a year! [0 = permanent ban]");
	}

	if (strlen(reason) < 4)
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "Invalid reason entered.");
	}

    banTargetID[playerid] = targetid;
    banTargetDays[playerid] = days;
    format(banTargetReason[playerid], sizeof banTargetReason[], reason);

	new name[MAX_PLAYER_NAME];
	new ip[18];
	GetPlayerName(targetid, name, sizeof name);
	GetPlayerIp(targetid, ip, sizeof ip);

	new string[512] = ""COL_WHITE"You are about to ban a player! Please confirm the following\ncredentials and click \""COL_PINK"Ban"COL_WHITE"\" if they are correct, else click \""COL_TOMATO"Cancel"COL_WHITE"\".";
	if (days != 0)
	{
		format(string, sizeof string, "%s\n\n- "COL_PINK"Player: "COL_WHITE"%s (IP: %s)\n- "COL_PINK"Type: "COL_WHITE"Temporary - %i days\n- "COL_PINK"Reason: "COL_WHITE"%s",
			string, name, ip, days, reason);
	}
	else
	{
		format(string, sizeof string, "%s\n\n- "COL_PINK"Player: "COL_WHITE"%s (IP: %s)\n- "COL_PINK"Type: "COL_WHITE"Permanent\n- "COL_PINK"Reason: "COL_WHITE"%s",
			string, name, ip, reason);
	}
	strcat(string, "\n\n"COL_WHITE"Admin Note: If the cheat player used is challengable, make sure you have\nsome kind of evidence in case a ban appeal is posted.");

	return Dialog_Show(playerid, BAN_PLAYER, DIALOG_STYLE_MSGBOX, "Ban A Player...", string, "Ban", "Cancel");
}

Dialog:BAN_PLAYER(playerid, response, listitem, inputtext[])
{
	if (!IsPlayerConnected(banTargetID[playerid]))
	{
	    return SendClientMessage(playerid, COLOR_TOMATO, "The player you were trying to ban left the server recently.");
	}

    new string[512];
	if (!response)
	{
	    new name[MAX_PLAYER_NAME];
		GetPlayerName(banTargetID[playerid], name, sizeof name);
	    format(string, sizeof string, "Ban on %s was canceled by you!", name);
	    return SendClientMessage(playerid, COLOR_TOMATO, string);
	}

	new name[MAX_PLAYER_NAME];
	new ip[18];
	GetPlayerName(banTargetID[playerid], name, sizeof name);
	GetPlayerIp(banTargetID[playerid], ip, sizeof ip);
	
	new admin[MAX_PLAYER_NAME];
	GetPlayerName(playerid, admin, sizeof admin);

	for (new i; i < 100; i++)
 	{
  		SendClientMessage(banTargetID[playerid], -1, "");
    }
    
	if (banTargetDays[playerid] != 0)
	{
		format(string, sizeof string, "\"%s\" has been banned by admin \"%s\" for %i days; for reason: %s.", name, admin, banTargetDays[playerid], banTargetReason[playerid]);
 		SendClientMessageToAll(COLOR_RED, string);
	}
	else
	{
		format(string, sizeof string, "\"%s\" has been permanently banned by admin \"%s\"; for reason: %s.", name, admin, banTargetReason[playerid]);
 		SendClientMessageToAll(COLOR_RED, string);
	}

	format(string, sizeof string, "INSERT INTO `users`(`name`, `ip`, `longip`, `expire_timestamp`, `register_timestamp`, `last_activity_timestamp`, `admin`, `reason`) VALUES('%s', '%s', %i, %i, %i, %i, '%s', '%s')", name, ip, Ban_GetLongIP(ip), (banTargetDays[playerid] == 0) ? (0) : (gettime() + (banTargetDays[playerid] * 24 * 60 * 60)), gettime(), gettime(), name, banTargetReason[playerid]);
 	db_query(banDatabase, string);
    
	TextDrawShowForPlayer(playerid, banTextDraw[0]);
	TextDrawShowForPlayer(playerid, banTextDraw[1]);
	return Kick(banTargetID[playerid], KICK_DELAY);
}

CMD:searchban(playerid, params[])
{
	if (!params[0] || params[0] == ' ' || strlen(params) < 4)
	{
	    return SendClientMessage(playerid, COLOR_WHITE, "Usage: /searchban [name/ip]");
	}

	new bool:nameEntered;
	for (new i; params[i] != EOS; i++)
	{
	    if (params[i] == '.' || params[i] >= '0' && params[i] <= '9')
	    {
	        continue;
	    }

	    nameEntered = true;
	    break;
	}

	new string[512];
	if (nameEntered)
	{
		format(string, sizeof string, "SELECT * FROM `users` WHERE `name` = '%s' LIMIT 1", params);
	}
	else
	{
		if (!Ban_GetLongIP(params))
		{
		    return SendClientMessage(playerid, COLOR_TOMATO, "Invalid IP. Address entered.");
		}
		
		format(string, sizeof string, "SELECT * FROM `users` WHERE (`longip` != 0 AND (`longip` & %i) = %i) LIMIT 1", BAN_MASK, (Ban_GetLongIP(params) & BAN_MASK));
	}

	new DBResult:result = db_query(banDatabase, string);
	if (db_num_rows(result) == 0)
	{
	    if (nameEntered)
	    {
	    	SendClientMessage(playerid, COLOR_TOMATO, "The player you are looking for is not banned.");
		}
		else
	    {
	    	SendClientMessage(playerid, COLOR_TOMATO, "The IP. you entered is not banned.");
		}
	    return db_free_result(result);
	}

	if (db_get_field_assoc_int(result, "expire_timestamp") != 0 && db_get_field_assoc_int(result, "expire_timestamp") <= gettime())
 	{
		format(string, sizeof string, "DELETE FROM `users` WHERE `id` = %i", db_get_field_assoc_int(result, "id"));
		db_query(banDatabase, string);

		new name[MAX_PLAYER_NAME];
		db_get_field_assoc(result, "name", name, sizeof name);
		format(string, sizeof string, "\"%s\"'s ban was lifted %s ago.", name, ReturnTimelapse(db_get_field_assoc_int(result, "expire_timestamp"), gettime()));
		SendClientMessage(playerid, COLOR_GREEN, string);

	    return db_free_result(result);
	}

    unbanTargetID[playerid] = db_get_field_assoc_int(result, "id");

	db_get_field_assoc(result, "name", unbanTargetName[playerid], sizeof unbanTargetName[]);
	db_get_field_assoc(result, "ip", unbanTargetIp[playerid], sizeof unbanTargetIp[]);
	new reason[64];
	db_get_field_assoc(result, "reason", reason, sizeof reason);
	new admin[MAX_PLAYER_NAME];
	db_get_field_assoc(result, "admin", admin, sizeof admin);

	format(string, sizeof string, ""COL_WHITE"Showing ban result for your search \""COL_TOMATO"%s"COL_WHITE"\".\nIf you want to unban this player personally, click \""COL_PINK"Unban"COL_WHITE"\"; or click \""COL_TOMATO"Close"COL_WHITE"\" to close this dialog!", params);
	if (db_get_field_assoc_int(result, "expire_timestamp") != 0)
	{
		format(string, sizeof string, "%s\n\n- "COL_PINK"Player: "COL_WHITE"%s (IP: %s)\n- "COL_PINK"Dated: "COL_WHITE"%s (%s ago)\n- "COL_PINK"Timeleft For Unban: "COL_WHITE"%s\n- "COL_PINK"Admin Who Banned: "COL_WHITE"%s\n- "COL_PINK"Reason: "COL_WHITE"%s",
			string, unbanTargetName[playerid], unbanTargetIp[playerid], ReturnDate(db_get_field_assoc_int(result, "register_timestamp")), ReturnTimelapse(db_get_field_assoc_int(result, "register_timestamp"), gettime()), ReturnTimelapse(gettime(), db_get_field_assoc_int(result, "expire_timestamp")), admin, reason);
	}
	else
	{
		format(string, sizeof string, "%s\n\n- "COL_PINK"Player: "COL_WHITE"%s (IP: %s)\n- "COL_PINK"Dated: "COL_WHITE"%s (%s ago)\n- "COL_PINK"Timeleft For Unban: "COL_WHITE"Never (permanent ban)\n- "COL_PINK"Admin Who Banned: "COL_WHITE"%s\n- "COL_PINK"Reason: "COL_WHITE"%s",
			string, unbanTargetName[playerid], unbanTargetIp[playerid], ReturnDate(db_get_field_assoc_int(result, "register_timestamp")), ReturnTimelapse(db_get_field_assoc_int(result, "register_timestamp"), gettime()), admin, reason);
	}
	format(string, sizeof string, "%s\n\n"COL_WHITE"Player last active was on \"%s\" during ban time period!", string, ReturnDate(db_get_field_assoc_int(result, "last_activity_timestamp")));

	db_free_result(result);
	
	return Dialog_Show(playerid, SEARCH_BANNED_PLAYER, DIALOG_STYLE_MSGBOX, "Search result for banned Player / IP...", string, "Close", "UnBan");
}

Dialog:SEARCH_BANNED_PLAYER(playerid, response, listitem, inputtext[])
{
	if (response)
	{
		return 1;
	}

    new string[256];
	format(string, sizeof string, "DELETE FROM `users` WHERE `name` = '%s' OR `ip` = '%s' OR `longip` != 0 AND (`longip` & %i) = %i", unbanTargetName[playerid], unbanTargetIp[playerid], BAN_MASK, (Ban_GetLongIP(unbanTargetIp[playerid]) & BAN_MASK));
 	db_query(banDatabase, string);

	format(string, sizeof string, "You have unbanned player \"%s\" successfully!", unbanTargetName[playerid]);
	return SendClientMessage(playerid, COLOR_GREEN, string);
}
