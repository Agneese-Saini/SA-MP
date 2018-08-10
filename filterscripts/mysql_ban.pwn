// mysql_ban.pwn by Gammix
// Commands: /ban, /findban, /unban

#include <a_samp>
#include <a_mysql>
#include <sscanf2>
#include <zcmd>
#include <timestamp>

// MySQL settings
#define MYSQL_HOST		"localhost"
#define MYSQL_USER		"root"
#define MYSQL_PASS		""
#define MYSQL_DATABASE	"sa-mp"

// General filterscript settings
#define MAX_BAN_REASON_LENGTH 64 // max string length of ban reason
#define KICK_TIMER_DELAY 150 // in miliseconds - a timer delay added to Kick(); function

#define CIDR_BAN_MASK \
	(-1<<(32-(26))) // 26 = this is the CIDR ip detection range

#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_TOMATO 0xFF6347FF
#define COLOR_GREEN 0x6AFF2AFF
#define COLOR_RED 0xFF0400FF
#define COLOR_GREY 0xC4C4C4FF

#define COL_WHITE "{FFFFFF}"
#define COL_TOMATO "{FF6347}"
#define COL_GREEN "{6AFF2A}"
#define COL_RED "{FF0400}"
#define COL_GREY "{c4c4c4}"

#define MAX_PLAYER_IP 18

enum E_BAN_DATA {
	BAN_CMD_TIMESTAMP,
	BAN_TARGET_ID,
	BAN_TARGET_NAME[MAX_PLAYER_NAME],
	BAN_TARGET_IP[MAX_PLAYER_IP],
	BAN_DAYS,
	BAN_REASON[MAX_BAN_REASON_LENGTH],
};

enum E_UNBAN_DATA {
	UNBAN_CMD_TIMESTAMP,
	UNBAN_TARGET_ID,
	UNBAN_TARGET_NAME[MAX_PLAYER_NAME]
};

new MySQL:database;
new Text:banTextDraw[2];
new adminBanData[MAX_PLAYERS][E_BAN_DATA];
new adminUnBanData[MAX_PLAYERS][E_UNBAN_DATA];

IpToLong(const address[]) {
	new parts[4];
	sscanf(address, "p<.>a<i>[4]", parts);
	return ((parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8) | parts[3]);
}

ReturnDate(timestamp) {
	static const MONTH_NAMES[12][] = {
		"January", "Feburary", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
	};

	new year, month, day, hour, minute, second;
	stamp2datetime(timestamp, year, month, day, hour, minute, second);

	new ret[32];
	format(ret, sizeof(ret),
		"%i %s, %i",
		day, MONTH_NAMES[month - 1], year
	);
	return ret;
}

ReturnTimelapse(start, till) {
    new ret[32];
	new seconds = (till - start);

	const
		MINUTE = (60),
		HOUR = (60 * MINUTE),
		DAY = (24 * HOUR),
		MONTH = (30 * DAY);

	if (seconds == 1) {
		format(ret, sizeof(ret), "a second");
	} else if (seconds < (1 * MINUTE)) {
		format(ret, sizeof(ret), "%i seconds", seconds);
	} else if (seconds < (2 * MINUTE)) {
		format(ret, sizeof(ret), "a minute");
	} else if (seconds < (45 * MINUTE)) {
		format(ret, sizeof(ret), "%i minutes", (seconds / MINUTE));
	} else if (seconds < (90 * MINUTE)) {
		format(ret, sizeof(ret), "an hour");
	} else if (seconds < (24 * HOUR)) {
		format(ret, sizeof(ret), "%i hours", (seconds / HOUR));
	} else if (seconds < (48 * HOUR)) {
		format(ret, sizeof(ret), "a day");
	} else if (seconds < (30 * DAY)) {
		format(ret, sizeof(ret), "%i days", (seconds / DAY));
	} else if (seconds < (12 * MONTH)) {
		new months = floatround(seconds / DAY / 30);
      	if (months <= 1) {
			format(ret, sizeof(ret), "a month");
      	} else {
			format(ret, sizeof(ret), "%i months", months);
		}
	} else {
      	new years = floatround(seconds / DAY / 365);
      	if (years <= 1) {
			format(ret, sizeof(ret), "a year");
      	} else {
			format(ret, sizeof(ret), "%i years", years);
		}
	}

	return ret;
}

ConnectMySQLDatabase() {
    mysql_log(ALL);

    new MySQLOpt:option = mysql_init_options();
	mysql_set_option(option, AUTO_RECONNECT, true);
    mysql_set_option(option, MULTI_STATEMENTS, false);
    mysql_set_option(option, POOL_SIZE, 2);
    mysql_set_option(option, SERVER_PORT, 3306);

	printf("[GBan.pwn] Connecting to MySQL server....");
	for (;;) {
		database = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DATABASE, option);

		if (mysql_errno(database) != 0) {
		    printf("[GBan.pwn] Connection error, retrying...");
		} else {
            printf("[GBan.pwn] Connection successfull!");
            break;
		}
	}

	mysql_tquery(database,
		"CREATE TABLE IF NOT EXISTS bans (\
			id INT(11) NOT NULL AUTO_INCREMENT, \
			name VARCHAR(24) DEFAULT NULL, \
			ip VARCHAR(24) DEFAULT NULL, \
			longip INT DEFAULT NULL, \
			ban_timestamp INT DEFAULT NULL, \
			ban_expire_timestamp INT DEFAULT NULL, \
			ban_admin VARCHAR(24) DEFAULT NULL, \
			ban_reason VARCHAR("#MAX_BAN_REASON_LENGTH") DEFAULT NULL, \
			PRIMARY KEY(id)\
		)"
	);
}

CreateBanTextDraws() {
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
}

forward DelayKick(playerid);
public DelayKick(playerid) {
    return Kick(playerid);
}

public OnFilterScriptInit() {
	ConnectMySQLDatabase();
    CreateBanTextDraws();

	return 1;
}

public OnFilterScriptExit() {
	mysql_close(database);

	for (new i = 0; i < sizeof(banTextDraw); i++) {
		TextDrawDestroy(banTextDraw[i]);
 	}

	return 1;
}

public OnPlayerConnect(playerid) {
	adminBanData[playerid][BAN_TARGET_ID] = INVALID_PLAYER_ID;
	adminUnBanData[playerid][UNBAN_TARGET_ID] = -1;

    new name[MAX_PLAYER_NAME];
    new ip[MAX_PLAYER_IP];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	GetPlayerIp(playerid, ip, MAX_PLAYER_IP);

	new string[MAX_PLAYER_NAME + 256];
	mysql_format(database, string, sizeof(string),
		"SELECT * FROM bans WHERE (name = '%e') OR (ip = '%e') OR (longip & %i = %i) LIMIT 1",
		name, ip, CIDR_BAN_MASK, (IpToLong(ip) & CIDR_BAN_MASK)
	);

	mysql_tquery(database, string, "OnUserBanDataLoad", "i", playerid);
	return 1;
}

forward OnUserBanDataLoad(playerid);
public OnUserBanDataLoad(playerid) {
	if (cache_num_rows() == 1) {
		new string[144];
		new ban_id;
		new ban_expire_timestamp;
   		cache_get_value_name_int(0, "id", ban_id);
   		cache_get_value_name_int(0, "ban_expire_timestamp", ban_expire_timestamp);

   		if (ban_expire_timestamp != 0 && gettime() >= ban_expire_timestamp) {
            mysql_format(database, string, sizeof(string),
                "DELETE FROM bans WHERE id = %i",
                ban_id
			);

			mysql_tquery(database, string);
			mysql_tquery(database, "ALTER TABLE bans AUTO_INCREMENT = 1");
			return;
		}

		new ban_timestamp;
		new ban_admin[MAX_PLAYER_NAME];
		new ban_reason[MAX_BAN_REASON_LENGTH];

   		cache_get_value_name_int(0, "ban_timestamp", ban_timestamp);
   		cache_get_value_name(0, "ban_admin", ban_admin, sizeof(ban_admin));
   		cache_get_value_name(0, "ban_reason", ban_reason, sizeof(ban_reason));

 		for (new i = 0; i < sizeof(banTextDraw); i++) {
			TextDrawShowForPlayer(playerid, banTextDraw[i]);
		}

	    for (new i = 0; i < 100; i++) {
			SendClientMessage(playerid, COLOR_TOMATO, "");
	    }

		format(string, sizeof(string), "This account is banned on this server! Banned on %s (%s ago) by admin %s!", ReturnDate(ban_timestamp), ReturnTimelapse(ban_timestamp, gettime()));
	    SendClientMessage(playerid, COLOR_TOMATO, string);
		format(string, sizeof(string), "Reason: %s", ban_reason);
	    SendClientMessage(playerid, COLOR_TOMATO, string);
	    if (ban_expire_timestamp != 0) {
			format(string, sizeof(string), "Your ban will be lifted on: %s (%s)", ReturnDate(ban_expire_timestamp), ReturnTimelapse(gettime(), ban_expire_timestamp));
		    SendClientMessage(playerid, COLOR_TOMATO, string);
		}

		SetTimerEx("DelayKick", KICK_TIMER_DELAY, 0, "i", playerid);
	}
	return;
}

CMD:ban(playerid, params[]) {
    if (!IsPlayerAdmin(playerid)) {
	    return SendClientMessage(playerid, COLOR_TOMATO, "You should be RCON Admin to use this command.");
	}

	if (!strcmp(params, "yes", true)) {
		if (adminBanData[playerid][BAN_TARGET_ID] != INVALID_PLAYER_ID) {
		    if (gettime() > (adminBanData[playerid][BAN_CMD_TIMESTAMP] + 60)) {
		        adminBanData[playerid][BAN_TARGET_ID] = INVALID_PLAYER_ID;
   				return SendClientMessage(playerid, COLOR_TOMATO, "Error: /ban command timeout, you have to respond to confirmation within a minute!");
			}

		    new name[MAX_PLAYER_NAME];
		    GetPlayerName(playerid, name, MAX_PLAYER_NAME);

			new ban_expire_timestamp = (adminBanData[playerid][BAN_DAYS] == 0) ? (0) : (gettime() + (adminBanData[playerid][BAN_DAYS] * 86400));

 			new string[1024];
			mysql_format(database, string, sizeof(string),
				"INSERT INTO bans(name, ip, longip, ban_timestamp, ban_expire_timestamp, ban_admin, ban_reason) \
					VALUES ('%e', '%e', %i, %i, %i, '%e', '%e')",
				adminBanData[playerid][BAN_TARGET_NAME], adminBanData[playerid][BAN_TARGET_IP], IpToLong(adminBanData[playerid][BAN_TARGET_IP]), gettime(), ban_expire_timestamp, name, adminBanData[playerid][BAN_REASON]
			);

			mysql_tquery(database, string);

			if (adminBanData[playerid][BAN_DAYS] != 0) {
				format(string, sizeof(string), "* Admin %s has banned %s for %i days (will be unbanned on %s) || Today's Date: %s || Reason: %s", name, adminBanData[playerid][BAN_TARGET_NAME], adminBanData[playerid][BAN_DAYS], ReturnDate(ban_expire_timestamp), ReturnDate(gettime()), adminBanData[playerid][BAN_REASON]);
			} else {
				format(string, sizeof(string), "* Admin %s has banned %s permanently || Today's Date: %s || Reason: %s", name, adminBanData[playerid][BAN_TARGET_NAME], ReturnDate(gettime()), adminBanData[playerid][BAN_REASON]);
			}
			SendClientMessageToAll(COLOR_TOMATO, string);

		    GetPlayerName(adminBanData[playerid][BAN_TARGET_ID], name, MAX_PLAYER_NAME);
		    if (!strcmp(name, adminBanData[playerid][BAN_TARGET_NAME])) {
				for (new i = 0; i < sizeof(banTextDraw); i++) {
					TextDrawShowForPlayer(adminBanData[playerid][BAN_TARGET_ID], banTextDraw[i]);
				}

			    for (new i = 0; i < 100; i++) {
					SendClientMessage(adminBanData[playerid][BAN_TARGET_ID], COLOR_TOMATO, "");
			    }

		    	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
		        format(string, sizeof(string), "Your account has been banned on this server, by admin %s! [Today's Date: %s]", name, ReturnDate(gettime()));
			    SendClientMessage(adminBanData[playerid][BAN_TARGET_ID], COLOR_TOMATO, string);
				format(string, sizeof(string), "Reason: %s", adminBanData[playerid][BAN_REASON]);
			    SendClientMessage(adminBanData[playerid][BAN_TARGET_ID], COLOR_TOMATO, string);
			    if (ban_expire_timestamp != 0) {
					format(string, sizeof(string), "Your ban will be lifted on: %s (%s later)", ReturnDate(ban_expire_timestamp), ReturnTimelapse(gettime(), ban_expire_timestamp));
				    SendClientMessage(adminBanData[playerid][BAN_TARGET_ID], COLOR_TOMATO, string);
				}

                SetTimerEx("DelayKick", KICK_TIMER_DELAY, 0, "i", adminBanData[playerid][BAN_TARGET_ID]);
			}
			else {
			    format(string, sizeof(string), "%s was banned, even he went offline but ban was successfull!", adminBanData[playerid][BAN_TARGET_NAME]);
			    SendClientMessage(playerid, COLOR_TOMATO, string);
			}

            adminBanData[playerid][BAN_TARGET_ID] = INVALID_PLAYER_ID;
		}
		return 1;
	}
	else if (!strcmp(params, "no", true)) {
		if (adminBanData[playerid][BAN_TARGET_ID] != INVALID_PLAYER_ID) {
		    if (gettime() > (adminBanData[playerid][BAN_CMD_TIMESTAMP] + 60)) {
		        adminBanData[playerid][BAN_TARGET_ID] = INVALID_PLAYER_ID;
   				return SendClientMessage(playerid, COLOR_TOMATO, "Error: /ban command timeout, you have to respond to confirmation within a minute!");
			}

		    adminBanData[playerid][BAN_TARGET_ID] = INVALID_PLAYER_ID;

		    SendClientMessage(playerid, COLOR_TOMATO, "Ban was canceled.");
		}
		return 1;
	}

	new targetid, reason[MAX_BAN_REASON_LENGTH], days;
	if (sscanf(params, "uis["#MAX_BAN_REASON_LENGTH"]", targetid, days, reason)) {
		SendClientMessage(playerid, COLOR_WHITE, "Usage: /ban [id/name] [days] [reason]");
	    SendClientMessage(playerid, COLOR_WHITE, "Note: 0 days means a permanent ban from server.");
	    return 1;
	}

	if (!IsPlayerConnected(targetid)) {
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Target player isn't online.");
	}

	if (days < 0 || days > 365) {
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Number of days cannot be negative or greater than 365 days! [0 = permanent ban]");
	}

	if (strlen(reason) < 4) {
	    return SendClientMessage(playerid, COLOR_TOMATO, "Error: Invalid reason entered.");
	}

	adminBanData[playerid][BAN_CMD_TIMESTAMP] = gettime();
	adminBanData[playerid][BAN_TARGET_ID] = targetid;
	adminBanData[playerid][BAN_DAYS] = days;
	GetPlayerName(targetid, adminBanData[playerid][BAN_TARGET_NAME], MAX_PLAYER_NAME);
	GetPlayerIp(targetid, adminBanData[playerid][BAN_TARGET_IP], 18);
	format(adminBanData[playerid][BAN_REASON], MAX_BAN_REASON_LENGTH, reason);

	new string[1024];
	if (days != 0) {
		format(string, sizeof(string),
		    "You are about to ban \"%s\" for %i days || Reason: %s || Today's date: %s",
		    adminBanData[playerid][BAN_TARGET_NAME], days, reason, ReturnDate(gettime())
		);
	} else {
		format(string, sizeof(string),
		    "You are about to ban \"%s\" permanently || Reason: %s || Today's date: %s",
		    adminBanData[playerid][BAN_TARGET_NAME], reason, ReturnDate(gettime())
		);
	}
	SendClientMessage(playerid, COLOR_TOMATO, "");
	SendClientMessage(playerid, COLOR_TOMATO, string);
	SendClientMessage(playerid, COLOR_TOMATO, "Please confirm by typing \"/ban yes\" to ban the player, or \"/ban no\" to cancel");
	return 1;
}

CMD:findban(playerid, params[]) {
    if (!IsPlayerAdmin(playerid)) {
	    return SendClientMessage(playerid, COLOR_TOMATO, "You should be RCON Admin to use this command.");
	}

	new match[32];
	if (sscanf(params, "s[32]", match)) {
		return SendClientMessage(playerid, COLOR_WHITE, "Usage: /unban [name/ip]");
	}

	new string[MAX_PLAYER_NAME + 256];
	mysql_format(database, string, sizeof(string),
		"SELECT * FROM bans WHERE (name = '%e') OR (ip = '%e') OR (longip & %i = %i) LIMIT 1",
		match, match, CIDR_BAN_MASK, (IpToLong(match) & CIDR_BAN_MASK)
	);

    return mysql_tquery(database, string, "OnFindBanSearchDataLoad", "is", playerid, match);
}

forward OnFindBanSearchDataLoad(playerid, const match[]);
public OnFindBanSearchDataLoad(playerid, const match[]) {
    if (cache_num_rows() == 0) {
	    SendClientMessage(playerid, COLOR_TOMATO, "Error: User not found in ban database!");
	    return;
	}

	new string[512];

	new ban_id;
	new ban_expire_timestamp;
	cache_get_value_name_int(0, "id", ban_id);
	cache_get_value_name_int(0, "ban_expire_timestamp", ban_expire_timestamp);

   	if (ban_expire_timestamp != 0 && gettime() >= ban_expire_timestamp) {
    	mysql_format(database, string, sizeof(string),
     		"DELETE FROM bans WHERE id = %i",
			 ban_id
		);

		mysql_tquery(database, string);
		mysql_tquery(database, "ALTER TABLE bans AUTO_INCREMENT = 1");

	    SendClientMessage(playerid, COLOR_TOMATO, "Error: User not found in ban database!");
	    return;
	}

	new name[MAX_PLAYER_NAME];
    new ip[MAX_PLAYER_IP];
    new date;
    new unban_date;
	new admin[MAX_PLAYER_NAME];
	new reason[MAX_BAN_REASON_LENGTH];

	cache_get_value_name(0, "name", name, MAX_PLAYER_NAME);
	cache_get_value_name(0, "ip", ip, MAX_PLAYER_IP);
   	cache_get_value_name_int(0, "ban_timestamp", date);
   	cache_get_value_name_int(0, "ban_expire_timestamp", unban_date);
	cache_get_value_name(0, "ban_admin", admin, MAX_PLAYER_NAME);
	cache_get_value_name(0, "ban_reason", reason, MAX_BAN_REASON_LENGTH);

	if (unban_date == 0) {
		format(string, sizeof(string),
			COL_WHITE "/findban search result for \"%s\":\n\n"COL_GREY"* UserName: "COL_TOMATO"%s\n"COL_GREY"* IP. Address: "COL_TOMATO"%s\n"COL_GREY"* Banned By Admin: "COL_WHITE"%s\n"COL_GREY"* Banned On Date: "COL_WHITE"%s (%s ago)\n"COL_GREY"* Ban Type: "COL_WHITE"Permanent\n"COL_GREY"* Reason: "COL_WHITE"%s\n\nToday's Date: %s!\nTo unban a player, type /unban <name/ip>!",
			match, name, ip, admin, ReturnDate(date), ReturnTimelapse(date, gettime()), reason, ReturnDate(gettime())
		);
	} else {
		format(string, sizeof(string),
			COL_WHITE "/findban search result for \"%s\":\n\n"COL_GREY"* UserName: "COL_TOMATO"%s\n"COL_GREY"* IP. Address: "COL_TOMATO"%s\n"COL_GREY"* Banned By Admin: "COL_WHITE"%s\n"COL_GREY"* Banned On Date: "COL_WHITE"%s (%s ago)\n"COL_GREY"* UnBan On: "COL_TOMATO"%s (%s)\n"COL_GREY"* Reason: "COL_WHITE"%s\n\nToday's Date: %s!\nTo unban a player, type /unban <name/ip>!",
			match, name, ip, admin, ReturnDate(date), ReturnTimelapse(date, gettime()), ReturnDate(unban_date), ReturnTimelapse(gettime(), unban_date), reason, ReturnDate(gettime())
		);
	}

	ShowPlayerDialog(playerid, 0, DIALOG_STYLE_MSGBOX, "Server ban info:", string, "Close", "");
}


CMD:unban(playerid, params[]) {
    if (!IsPlayerAdmin(playerid)) {
	    return SendClientMessage(playerid, COLOR_TOMATO, "You should be RCON Admin to use this command.");
	}

	if (!strcmp(params, "yes", true)) {
	    if (adminUnBanData[playerid][UNBAN_TARGET_ID] != -1) {
		    if (gettime() > (adminUnBanData[playerid][UNBAN_CMD_TIMESTAMP] + 60)) {
		        adminUnBanData[playerid][UNBAN_TARGET_ID] = -1;
   				return SendClientMessage(playerid, COLOR_TOMATO, "Error: /unban command timeout, you have to respond to confirmation within a minute!");
			}

			new string[144];

			new name[MAX_PLAYER_NAME];
		    GetPlayerName(playerid, name, MAX_PLAYER_NAME);

			mysql_format(database, string, sizeof(string),
	     		"DELETE FROM bans WHERE id = %i",
				 adminUnBanData[playerid][UNBAN_TARGET_ID]
			);

			mysql_tquery(database, string);
			mysql_tquery(database, "ALTER TABLE bans AUTO_INCREMENT = 1");

			format(string, sizeof(string), "* Admin %s has unbanned %s || Today's Date: %s", name, adminUnBanData[playerid][UNBAN_TARGET_NAME], ReturnDate(gettime()));
			SendClientMessageToAll(COLOR_TOMATO, string);

			adminUnBanData[playerid][UNBAN_TARGET_ID] = -1;
		}
  		return 1;
	}
	else if (!strcmp(params, "no", true)) {
	    if (adminUnBanData[playerid][UNBAN_TARGET_ID] != -1) {
		    if (gettime() > (adminUnBanData[playerid][UNBAN_CMD_TIMESTAMP] + 60)) {
		        adminUnBanData[playerid][UNBAN_TARGET_ID] = -1;
   				return SendClientMessage(playerid, COLOR_TOMATO, "Error: /unban command timeout, you have to respond to confirmation within a minute!");
			}

		    adminUnBanData[playerid][UNBAN_TARGET_ID] = -1;

		    SendClientMessage(playerid, COLOR_TOMATO, "UnBan was canceled.");
		}
		return 1;
	}

	new match[32];
	if (sscanf(params, "s[32]", match)) {
		return SendClientMessage(playerid, COLOR_WHITE, "Usage: /unban [name/ip]");
	}

	new string[MAX_PLAYER_NAME + 256];
	mysql_format(database, string, sizeof(string),
		"SELECT * FROM bans WHERE (name = '%e') OR (ip = '%e') OR (longip & %i = %i) LIMIT 1",
		match, match, CIDR_BAN_MASK, (IpToLong(match) & CIDR_BAN_MASK)
	);

    return mysql_tquery(database, string, "OnUnBanSearchDataLoad", "i", playerid);
}

forward OnUnBanSearchDataLoad(playerid);
public OnUnBanSearchDataLoad(playerid) {
    if (cache_num_rows() == 0) {
	    SendClientMessage(playerid, COLOR_TOMATO, "Error: User not found in ban database!");
	    return;
	}

	new string[144];

	new ban_expire_timestamp;
	cache_get_value_name_int(0, "id", adminUnBanData[playerid][UNBAN_TARGET_ID]);
	cache_get_value_name_int(0, "ban_expire_timestamp", ban_expire_timestamp);

   	if (ban_expire_timestamp != 0 && gettime() >= ban_expire_timestamp) {
    	mysql_format(database, string, sizeof(string),
     		"DELETE FROM bans WHERE id = %i",
			 adminUnBanData[playerid][UNBAN_TARGET_ID]
		);

		mysql_tquery(database, string);
		mysql_tquery(database, "ALTER TABLE bans AUTO_INCREMENT = 1");

	    SendClientMessage(playerid, COLOR_TOMATO, "Error: User not found in ban database!");
	    return;
	}

    new ip[MAX_PLAYER_IP];
    new date;
    new unban_date;
	new admin[MAX_PLAYER_NAME];
	new reason[MAX_BAN_REASON_LENGTH];

	cache_get_value_name(0, "name", adminUnBanData[playerid][UNBAN_TARGET_NAME], MAX_PLAYER_NAME);
	cache_get_value_name(0, "ip", ip, MAX_PLAYER_IP);
   	cache_get_value_name_int(0, "ban_timestamp", date);
   	cache_get_value_name_int(0, "ban_expire_timestamp", unban_date);
	cache_get_value_name(0, "ban_admin", admin, MAX_PLAYER_NAME);
	cache_get_value_name(0, "ban_reason", reason, MAX_BAN_REASON_LENGTH);

	SendClientMessage(playerid, COLOR_TOMATO, "");

	format(string, sizeof(string), "Are you sure you want to unban \"%s\" (ip: %s)?", adminUnBanData[playerid][UNBAN_TARGET_NAME], ip);
	SendClientMessage(playerid, COLOR_TOMATO, string);

	if (unban_date == 0) {
		format(string, sizeof(string),
			"%s was banned by admin %s, on %s (%s ago) || Unban On: %s || Reason: %s",
			adminUnBanData[playerid][UNBAN_TARGET_NAME], admin, ReturnDate(date), ReturnTimelapse(date, gettime()), ReturnDate(unban_date), reason
		);
	} else {
		format(string, sizeof(string),
			"%s was banned by admin %s, on %s (%s ago) || Ban Type: Permanent || Reason: %s",
			adminUnBanData[playerid][UNBAN_TARGET_NAME], admin, ReturnDate(date), ReturnTimelapse(date, gettime()), reason
		);
	}
	SendClientMessage(playerid, COLOR_TOMATO, string);

	SendClientMessage(playerid, COLOR_TOMATO, "Type \"/unban yes\" to unban this player, or type \"/unban no\" to cancel");

	adminUnBanData[playerid][UNBAN_CMD_TIMESTAMP] = gettime();
}
