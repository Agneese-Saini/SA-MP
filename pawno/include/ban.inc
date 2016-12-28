// Ban.inc - v2.0.0 - By Gammix - Last updated on 22 Dec, 2016
#if defined ban_included
	#endinput
#endif
#define ban_included

#if !defined SQL_DATABASE
	#define SQL_DATABASE "sa-mp"
#endif

#if defined SQL_SERVER
	#include <a_mysql>

	#if !defined SQL_USERNAME
	    #define SQL_USERNAME "root"
	#endif

	#if !defined SQL_PASSWORD
	    #define SQL_PASSWORD ""
	#endif
#endif

forward _ban_anti_crash();
public	_ban_anti_crash()
{
	strcat("", "", 0);
}

const BAN_IP_MASK = (-1 << (32 - (/*this is the CIDR ip detection range [def: 26]*/26)));

#if defined SQL_SERVER
	static MySQL:db;
#else
	static DB:db;
#endif

static BAN_IpToLong(const ip[])
{
  	new len = strlen(ip);
	if (!(len > 0 && len < 17))
	{
    	return 0;
	}

	new count,
		pos,
		dest[3],
		val[4];
	for (new i; i < len; i++)
	{
		if (ip[i] == '.' || i == len)
		{
			strmid(dest, ip, pos, i);
			pos = (i + 1);

		    val[count] = strval(dest);
		    if (!(1 <= val[count] <= 255))
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

#if defined FILTERSCRIPT
	public OnFilterScriptInit()
#else
	public OnGameModeInit()
#endif
{
	#if defined SQL_SERVER
        mysql_log(ERROR | DEBUG);

	    new MySQLOpt: options = mysql_init_options();
		mysql_set_option(options, AUTO_RECONNECT, false);
		mysql_set_option(options, POOL_SIZE, 0);

		db = mysql_connect(SQL_SERVER, SQL_USERNAME, SQL_PASSWORD, SQL_DATABASE, options);
		if (mysql_errno() == 0)
		{
			mysql_tquery(db, "CREATE TABLE IF NOT EXISTS `bans` (`id` INT NOT NULL AUTO_INCREMENT, `name` VARCHAR(24) NOT NULL DEFAULT '', `ip` VARCHAR(18) NOT NULL DEFAULT '', `longip` INT NOT NULL DEFAULT '0', `expire` INT NOT NULL DEFAULT '0', PRIMARY KEY (`id`))");
	    	mysql_tquery(db, "CREATE TABLE IF NOT EXISTS `bans_data` (`bans_id` INT NOT NULL DEFAULT '0', `field` VARCHAR(128) NOT NULL DEFAULT '', `value` VARCHAR(512) NOT NULL DEFAULT '')");
		}
	#else
	    db = db_open(SQL_DATABASE ".db");
	    if (db)
	    {
		    db_query(db, "PRAGMA synchronous = NORMAL");
	       	db_query(db, "PRAGMA journal_mode = WAL");

			db_query(db, "CREATE TABLE IF NOT EXISTS `bans` (`id` INTEGER PRIMARY KEY, `name` VARCHAR(24) NOT NULL DEFAULT '', `ip` VARCHAR(18) NOT NULL DEFAULT '', `longip` INT NOT NULL DEFAULT '0', `expire` INT NOT NULL DEFAULT '0')");
			db_query(db, "CREATE TABLE IF NOT EXISTS `bans_data` (`bans_id` INT NOT NULL DEFAULT '0', `field` VARCHAR(128) NOT NULL DEFAULT '', `value` VARCHAR(512) NOT NULL DEFAULT '')");
		}
	#endif

	#if defined FILTERSCRIPT
		#if defined BAN_OnFilterScriptInit
			return BAN_OnFilterScriptInit();
		#else
		    return 1;
		#endif
	#else
		#if defined BAN_OnGameModeInit
			return BAN_OnGameModeInit();
		#else
		    return 1;
		#endif
	#endif
}
#if defined FILTERSCRIPT
	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif
	#define OnFilterScriptInit BAN_OnFilterScriptInit
	#if defined BAN_OnFilterScriptInit
		forward BAN_OnFilterScriptInit();
	#endif
#else
	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit BAN_OnGameModeInit
	#if defined BAN_OnGameModeInit
		forward BAN_OnGameModeInit();
	#endif
#endif

#if defined FILTERSCRIPT
	public OnFilterScriptExit()
#else
	public OnGameModeExit()
#endif
{
	#if defined SQL_SERVER
        mysql_close(db);
	#else
	    db_close(db);
	#endif

	#if defined FILTERSCRIPT
		#if defined BAN_OnFilterScriptExit
			return BAN_OnFilterScriptExit();
		#else
		    return 1;
		#endif
	#else
		#if defined BAN_OnGameModeExit
			return BAN_OnGameModeExit();
		#else
		    return 1;
		#endif
	#endif
}
#if defined FILTERSCRIPT
	#if defined _ALS_OnFilterScriptExit
		#undef OnFilterScriptExit
	#else
		#define _ALS_OnFilterScriptExit
	#endif
	#define OnFilterScriptExit BAN_OnFilterScriptExit
	#if defined BAN_OnFilterScriptExit
		forward BAN_OnFilterScriptExit();
	#endif
#else
	#if defined _ALS_OnGameModeExit
		#undef OnGameModeExit
	#else
		#define _ALS_OnGameModeExit
	#endif
	#define OnGameModeExit BAN_OnGameModeExit
	#if defined BAN_OnGameModeExit
		forward BAN_OnGameModeExit();
	#endif
#endif

public OnPlayerConnect(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);

	new ip[18];
	GetPlayerIp(playerid, ip, sizeof (ip));

	new banid = GetBanId(name, ip);
	if (banid != -1)
	{
		#if defined OnBannedPlayerConnect
	  		if (bool:OnBannedPlayerConnect(playerid, banid))
	   		{
		    	return 1;
			}
		#endif
		return Kick(playerid);
	}

	#if defined BAN_OnPlayerConnect
       	return BAN_OnPlayerConnect(playerid);
	#else
	   	return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect BAN_OnPlayerConnect
#if defined BAN_OnPlayerConnect
    forward BAN_OnPlayerConnect(playerid);
#endif

#if defined OnBannedPlayerConnect
	forward OnBannedPlayerConnect(playerid, banid);
#endif

/*
native Ban(playerid, const name[] = "", const ip[] = "", bool:range = false, expire = 0);
*/
stock BAN_Ban(playerid, name[MAX_PLAYER_NAME] = "", ip[18] = "", bool:range = false, expire = 0)
{
	if (IsPlayerConnected(playerid))
	{
	    GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	    GetPlayerIp(playerid, ip, 18);
	}
	else
	{
	    if (!name[0] && !ip[0])
	    {
	        return 0;
	    }
	}

	new string[256];
	#if defined SQL_SERVER
		mysql_format(db, string, sizeof(string), "SELECT 0 FROM `bans` WHERE `name` = '%s' OR `ip` = '%s' LIMIT 1", name, ip);
		new Cache:result = mysql_query(db, string);
		if (cache_num_rows() > 0)
		{
		    cache_delete(result);
		    return 0;
		}
		else
		{
		    mysql_format(db, string, sizeof(string), "INSERT INTO `bans`(`name`, `ip`, `longip`, `expire`) VALUES('%s', '%s', %i, %i)", name, ip, ((range) ? (BAN_IpToLong(ip)) : (0)), expire);
		    mysql_tquery(db, string);
		}
	#else
		format(string, sizeof(string), "SELECT 0 FROM `bans` WHERE `name` = '%s' OR `ip` = '%s' LIMIT 1", name, ip);
		new DBResult:result = db_query(db, string);
		if (db_num_rows(result) > 0)
		{
		    db_free_result(result);
      		return 0;
		}
		else
		{
		    format(string, sizeof(string), "INSERT INTO `bans`(`name`, `ip`, `longip`, `expire`) VALUES('%s', '%s', %i, %i)", name, ip, ((range) ? (BAN_IpToLong(ip)) : (0)), expire);
		    db_query(db, string);
		}
	#endif

	return Kick(playerid);
}
#if defined _ALS_Ban
	#undef Ban
#else
	#define _ALS_Ban
#endif
#define Ban BAN_Ban

/*
native BanEx(playerid, const reason[], const name[] = "", const ip[] = "", bool:range = false, expire = 0);
*/
stock BAN_BanEx(playerid, const reason[], name[MAX_PLAYER_NAME] = "", ip[18] = "", bool:range = false, expire = 0)
{
	if (IsPlayerConnected(playerid))
	{
	    GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	    GetPlayerIp(playerid, ip, 18);
	}
	else
	{
	    if (!name[0] && !ip[0])
	    {
	        return 0;
	    }
	}

	new string[256];
	#if defined SQL_SERVER
		mysql_format(db, string, sizeof(string), "SELECT 0 FROM `bans` WHERE `name` = '%s' OR `ip` = '%s' LIMIT 1", name, ip);
		new Cache:result = mysql_query(db, string);
		if (cache_num_rows() > 0)
		{
		    cache_delete(result);
		    return (IsPlayerConnected(playerid)) ? (Kick(playerid)) : (1);
		}
		else
		{
		    cache_delete(result);
		    mysql_format(db, string, sizeof(string), "INSERT INTO `bans`(`name`, `ip`, `longip`, `expire`) VALUES('%s', '%s', %i, %i)", name, ip, ((range) ? (BAN_IpToLong(ip)) : (0)), expire);
		    mysql_tquery(db, string);

		    mysql_format(db, string, sizeof(string), "SELECT `id` FROM `bans` WHERE `name` = '%s' AND `ip` = '%s' LIMIT 1", name, ip);
		    result = mysql_query(db, string);
		    new banid;
		    cache_get_value_index_int(0, 0, banid);
		    cache_delete(result);

		    mysql_format(db, string, sizeof(string), "INSERT INTO `bans_data` VALUES(%i, 'reason', '%e')", banid, reason, reason);
		    mysql_tquery(db, string);
		}
	#else
		format(string, sizeof(string), "SELECT 0 FROM `bans` WHERE `name` = '%s' OR `ip` = '%s' LIMIT 1", name, ip);
		new DBResult:result = db_query(db, string);
		if (db_num_rows(result) > 0)
		{
		    db_free_result(result);
      		return (IsPlayerConnected(playerid)) ? (Kick(playerid)) : (1);
		}
		else
		{
		    db_free_result(result);
		    format(string, sizeof(string), "INSERT INTO `bans`(`name`, `ip`, `longip`, `expire`) VALUES('%s', '%s', %i, %i)", name, ip, ((range) ? (BAN_IpToLong(ip)) : (0)), expire);
		    db_query(db, string);

		    format(string, sizeof(string), "SELECT `id` FROM `bans` WHERE `name` = '%s' AND `ip` = '%s' LIMIT 1", name, ip);
		    result = db_query(db, string);

		    format(string, sizeof(string), "INSERT INTO `bans_data` VALUES(%i, 'reason', '%q')", db_get_field_int(result, 0), reason);
		    db_query(db, string);

		    db_free_result(result);
		}
	#endif

	return Kick(playerid);
}
#if defined _ALS_BanEx
	#undef BanEx
#else
	#define _ALS_BanEx
#endif
#define BanEx BAN_BanEx

/*
native GetBanId(const name[], const ip[]);
*/
stock GetBanId(const name[], const ip[])
{
	if (!name[0] && !ip[0])
 	{
		return 0;
	}

    new string[256];
	#if defined SQL_SERVER
		mysql_format(db, string, sizeof(string), "SELECT `id`, `expire` FROM `bans` WHERE `name` = '%s' OR `ip` = '%s' LIMIT 1", name, ip);
		new Cache:result = mysql_query(db, string);
		if (cache_num_rows() > 0)
		{
		    new banid;
		    cache_get_value_index_int(0, 0, banid);
		    new expire;
		    cache_get_value_index_int(0, 1, expire);
			cache_delete(result);

			if (expire != 0 && (gettime() - expire) >= 0)
			{
			   	mysql_format(db, string, sizeof(string), "DELETE FROM `bans` WHERE `id` = %i", banid);
				mysql_tquery(db, string);
			   	mysql_format(db, string, sizeof(string), "DELETE FROM `bans_data` WHERE `bans_id` = %i", banid);
				mysql_tquery(db, string);
				return -1;
			}
			return banid;
		}
		cache_delete(result);
	#else
		format(string, sizeof(string), "SELECT `id`, `expire` FROM `bans` WHERE `name` = '%s' OR `ip` = '%s' LIMIT 1", name, ip);
		new DBResult:result = db_query(db, string);
		if (db_num_rows(result) > 0)
		{
		    new banid = db_get_field_int(result, 0);
		    new expire = db_get_field_int(result, 1);
            db_free_result(result);

			if (expire != 0 && (gettime() - expire) >= 0)
			{
			   	format(string, sizeof(string), "DELETE FROM `bans` WHERE `id` = %i", banid);
				db_query(db, string);
			   	format(string, sizeof(string), "DELETE FROM `bans_data` WHERE `bans_id` = %i", banid);
				db_query(db, string);
				return -1;
			}
			return banid;
		}
        db_free_result(result);
	#endif

	if (!ip[0])
	{
	    return -1;
	}

	#if defined SQL_SERVER
	    mysql_format(db, string, sizeof(string), "SELECT `id`, `expire` FROM `bans` WHERE (`longip` != 0) AND ((`longip` & %i) = %i) LIMIT 1", BAN_IP_MASK, (BAN_IpToLong(ip) & BAN_IP_MASK));
		result = mysql_query(db, string);
		if (cache_num_rows() > 0)
		{
  			new banid;
			cache_get_value_index_int(i, 0, banid);
		    new expire;
			cache_get_value_index_int(i, 1, expire);
            cache_delete(result);

		    if (expire != 0 && (gettime() - expire) >= 0)
			{
				mysql_format(db, string, sizeof(string), "DELETE FROM `bans` WHERE `id` = %i", banid);
				mysql_tquery(db, string);
   				mysql_format(db, string, sizeof(string), "DELETE FROM `bans_data` WHERE `bans_id` = %i", banid);
				mysql_tquery(db, string);
				return -1;
			}
            return banid;
		}
		cache_delete(result);
	#else
	    format(string, sizeof(string), "SELECT `id`, `expire` FROM `bans` WHERE (`longip` != 0) AND ((`longip` & %i) = %i) LIMIT 1", BAN_IP_MASK, (BAN_IpToLong(ip) & BAN_IP_MASK));
	    result = db_query(db, string);
		if (db_num_rows(result) > 0)
		{
			new banid = db_get_field_int(result, 0);
		    new expire = db_get_field_int(result, 1);
			db_free_result(result);

		    if (expire != 0 && (gettime() - expire) >= 0)
			{
				format(string, sizeof(string), "DELETE FROM `bans` WHERE `id` = %i", banid);
				db_query(db, string);
   				format(string, sizeof(string), "DELETE FROM `bans_data` WHERE `bans_id` = %i", banid);
				db_query(db, string);
				return -1;
			}
      		return banid;
		}
		db_free_result(result);
	#endif
	return -1;
}

/*
native UnBan(banid);
*/
stock UnBan(banid)
{
    new string[128];
	#if defined SQL_SERVER
	    mysql_format(db, string, sizeof(string), "DELETE FROM `bans` WHERE `id` = %i", banid);
		mysql_tquery(db, string);
		mysql_format(db, string, sizeof(string), "DELETE FROM `bans_data` WHERE `bans_id` = %i", banid);
		mysql_tquery(db, string);
	#else
	    format(string, sizeof(string), "DELETE FROM `bans` WHERE `id` = %i", banid);
		db_query(db, string);
 		format(string, sizeof(string), "DELETE FROM `bans_data` WHERE `bans_id` = %i", banid);
		db_query(db, string);
	#endif
	return 1;
}

/*
native SetBanString(banid, const field[], const value[]);
*/
stock SetBanString(banid, const field[], const value[])
{
	new string[256];
	#if defined SQL_SERVER
		mysql_format(db, string, sizeof(string), "SELECT 0 FROM `bans_data` WHERE `field` = '%s' AND `bans_id` = %i LIMIT 1", field, banid);
		new Cache:result = mysql_query(db, string);
		if (cache_num_rows() > 0)
		{
		    mysql_format(db, string, sizeof(string), "UPDATE `bans_data` SET `%e` = '%e' WHERE `bans_id` = %i", field, value, banid);
			mysql_tquery(db, string);
		}
		else
		{
		    mysql_format(db, string, sizeof(string), "INSERT INTO `bans_data` VALUES(%i, '%e', '%e')", banid, field, value);
			mysql_tquery(db, string);
		}
		cache_delete(result);
	#else
    	format(string, sizeof(string), "SELECT `value` FROM `bans_data` WHERE `field` = '%s' AND `bans_id` = %i LIMIT 1", field, banid);
		new DBResult:result = db_query(db, string);
		if (db_num_rows(result) > 0)
		{
		    format(string, sizeof(string), "UPDATE `bans_data` SET `%q` = '%q' WHERE `bans_id` = %i", field, value, banid);
			db_query(db, string);
		}
		else
		{
		    format(string, sizeof(string), "INSERT INTO `bans_data` VALUES(%i, '%q', '%q')", banid, field, value);
			db_query(db, string);
		}
		db_free_result(result);
	#endif
	return 1;
}

/*
native SetBanInt(banid, const field[], value);
*/
stock SetBanInt(banid, const field[], value)
{
	new string[256];
	#if defined SQL_SERVER
		mysql_format(db, string, sizeof(string), "SELECT 0 FROM `bans_data` WHERE `field` = '%s' AND `bans_id` = %i LIMIT 1", field, banid);
		new Cache:result = mysql_query(db, string);
		if (cache_num_rows() > 0)
		{
		    mysql_format(db, string, sizeof(string), "UPDATE `bans_data` SET `%e` = %i WHERE `bans_id` = %i", field, value, banid);
			mysql_tquery(db, string);
		}
		else
		{
		    mysql_format(db, string, sizeof(string), "INSERT INTO `bans_data` VALUES(%i, '%e', %i)", banid, field, value);
			mysql_tquery(db, string);
		}
		cache_delete(result);
	#else
    	format(string, sizeof(string), "SELECT `value` FROM `bans_data` WHERE `field` = '%s' AND `bans_id` = %i LIMIT 1", field, banid);
		new DBResult:result = db_query(db, string);
		if (db_num_rows(result) > 0)
		{
		    format(string, sizeof(string), "UPDATE `bans_data` SET `%q` = %i WHERE `bans_id` = %i", field, value, banid);
			db_query(db, string);
		}
		else
		{
		    format(string, sizeof(string), "INSERT INTO `bans_data` VALUES(%i, '%q', %i)", banid, field, value);
			db_query(db, string);
		}
	 	db_free_result(result);
	#endif
	return 1;
}

/*
native SetBanFloat(banid, const field[], Float:value);
*/
stock SetBanFloat(banid, const field[], Float:value)
{
	new string[256];
	#if defined SQL_SERVER
		mysql_format(db, string, sizeof(string), "SELECT 0 FROM `bans_data` WHERE `field` = '%s' AND `bans_id` = %i LIMIT 1", field, banid);
		new Cache:result = mysql_query(db, string);
		if (cache_num_rows() > 0)
		{
		    mysql_format(db, string, sizeof(string), "UPDATE `bans_data` SET `%e` = %f WHERE `bans_id` = %i", field, value, banid);
			mysql_tquery(db, string);
		}
		else
		{
		    mysql_format(db, string, sizeof(string), "INSERT INTO `bans_data` VALUES(%i, '%e', %f)", banid, field, value);
			mysql_tquery(db, string);
		}
		cache_delete(result);
	#else
    	format(string, sizeof(string), "SELECT `value` FROM `bans_data` WHERE `field` = '%s' AND `bans_id` = %i LIMIT 1", field, banid);
		new DBResult:result = db_query(db, string);
		if (db_num_rows(result) > 0)
		{
		    format(string, sizeof(string), "UPDATE `bans_data` SET `%q` = %f WHERE `bans_id` = %i", field, value, banid);
			db_query(db, string);
		}
		else
		{
		    format(string, sizeof(string), "INSERT INTO `bans_data` VALUES(%i, '%q', %f)", banid, field, value);
			db_query(db, string);
		}
	 	db_free_result(result);
	#endif
	return 1;
}

/*
native GetBanString(banid, const field[], dest[], maxlength = sizeof(dest));
*/
stock GetBanString(banid, const field[], dest[], maxlength = sizeof(dest))
{
	new string[256];
	#if defined SQL_SERVER
		mysql_format(db, string, sizeof(string), "SELECT `value` FROM `bans_data` WHERE `field` = '%s' AND `bans_id` = %i LIMIT 1", field, banid);
		new Cache:result = mysql_query(db, string);
		if (cache_num_rows() > 0)
		{
		    cache_get_value_index(0, 0, dest, maxlength);
			cache_delete(result);
		    return 1;
		}
		cache_delete(result);
	#else
    	format(string, sizeof(string), "SELECT `value` FROM `bans_data` WHERE `field` = '%s' AND `bans_id` = %i LIMIT 1", field, banid);
		new DBResult:result = db_query(db, string);
		if (db_num_rows(result) > 0)
		{
		    db_get_field(result, 0, dest, maxlength);
		    db_free_result(result);
		    return 1;
		}
		db_free_result(result);
	#endif
	return 0;
}

/*
native GetBanInt(banid, const field[]);
*/
stock GetBanInt(banid, const field[])
{
	new string[256];
	#if defined SQL_SERVER
		mysql_format(db, string, sizeof(string), "SELECT `value` FROM `bans_data` WHERE `field` = '%s' AND `bans_id` = %i LIMIT 1", field, banid);
		new Cache:result = mysql_query(db, string);
		if (cache_num_rows() > 0)
		{
		    new ret = cache_get_value_index_int(0, 0, ret);
		    cache_delete(result);
		    return ret;
		}
		cache_delete(result);
	#else
    	format(string, sizeof(string), "SELECT `value` FROM `bans_data` WHERE `field` = '%s' AND `bans_id` = %i LIMIT 1", field, banid);
		new DBResult:result = db_query(db, string);
		if (db_num_rows(result) > 0)
		{
		    new ret = db_get_field_int(result, 0);
		    db_free_result(result);
		    return ret;
		}
		db_free_result(result);
	#endif
	return 0;
}

/*
native Float:GetBanFloat(banid, const field[]);
*/
stock Float:GetBanFloat(banid, const field[])
{
	new string[256];
	#if defined SQL_SERVER
		mysql_format(db, string, sizeof(string), "SELECT `value` FROM `bans_data` WHERE `field` = '%s' AND `bans_id` = %i LIMIT 1", field, banid);
		new Cache:result = mysql_query(db, string);
		if (cache_num_rows() > 0)
		{
		    new ret = cache_get_value_index_float(0, 0, ret);
		    cache_delete(result);
		    return ret;
		}
		cache_delete(result);
	#else
    	format(string, sizeof(string), "SELECT `value` FROM `bans_data` WHERE `field` = '%s' AND `bans_id` = %i LIMIT 1", field, banid);
		new DBResult:result = db_query(db, string);
		if (db_num_rows(result) > 0)
		{
		    new ret = db_get_field_float(result, 0);
		    db_free_result(result);
		    return ret;
		}
		db_free_result(result);
	#endif
	return 0;
}
