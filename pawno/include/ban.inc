#if defined ban_included
	#endinput
#endif
#define ban_included

/*
 * ban.inc
 * Version: 1.2.4 (last updated on 9 August, 2016)
 * By Gammix
 *
 * Functions:
   native Ban_Init(const database[], const host[] = "", const user[] = "", const password[] = "", port = 3306, bool:autoreconnect = true, pool_size = 2);
   native Ban_Exit();
   native Ban(playerid, const name[] = "", const ip[] = "", range = 0, expiredate = 0);
   native BanEx(playerid, const reason[], const name[] = "", const ip[] = "", range = 0, expiredate = 0);
   native GetBanId(const name[] = "", const ip[] = "");
   native UnBan(banid);
   native SetBanData(banid, const field[], type, {Float, _}:...);
   native GetBanData(banid, const field[], type, {Float, _}:...);
 *
 */

#if defined CONNECTION_TYPE_MYSQL
	#tryinclude <a_mysql>

	#if !defined mysql_included
		#undef CONNECTION_TYPE_MYSQL
	#endif
#endif

forward _ban_anti_crash();
public	_ban_anti_crash()
{
	strcat("", "", 0);
}

#if !defined CONNECTION_TYPE_MYSQL
	static DB:bandb;
#else
	static mysqlconn;
#endif

stock static BAN_ip2long(const ip[])
{
  	new len = strlen(ip);
	if(! (len > 0 && len < 17))
    {
        return 0;
    }

	new count = 0;
    for (new i; i < len; i++)
    {
     	if(ip[i] == '.')
		{
			count++;
		}
	}
	if (! (count == 3))
	{
	    return 0;
	}

 	new address = strval(ip) << 24;
    count = strfind(ip, ".", false, 0) + 1;

	address += strval(ip[count]) << 16;
	count = strfind(ip, ".", false, count) + 1;

	address += strval(ip[count]) << 8;
	count = strfind(ip, ".", false, count) + 1;

	address += strval(ip[count]);
	return address;
}

stock static BAN_ipmatch(const ip1[], const ip2[], rangetype = 26)
{
   	new ip = BAN_ip2long(ip1);
    new subnet = BAN_ip2long(ip2);

    new mask = -1 << (32 - rangetype);
    subnet &= mask;

    return bool:((ip & mask) == subnet);
}

stock Ban_Init(database[], const host[] = "", const user[] = "", const password[] = "", port = 3306, bool:autoreconnect = true, pool_size = 2)
{
	#if !defined CONNECTION_TYPE_MYSQL
		#pragma unused host
		#pragma unused user
		#pragma unused password
		#pragma unused port
		#pragma unused autoreconnect
		#pragma unused pool_size

		bandb = db_open(database);

		db_free_result(db_query(bandb, "CREATE TABLE IF NOT EXISTS `Bans` (`BanId` INTEGER PRIMARY KEY, `Name` TEXT, `Ip` TEXT, `RangeBan` INTEGER, `ExpireDate` INTEGER)"));
		db_free_result(db_query(bandb, "CREATE TABLE IF NOT EXISTS `BansData` (`BanId` INTEGER, `Field` TEXT, `Value` TEXT)"));
	#else
	    mysqlconn = mysql_connect(host, user, database, password, port, autoreconnect, pool_size);

	    mysql_tquery(mysqlconn, "CREATE TABLE IF NOT EXISTS `Bans` (`BanId` INT NOT NULL AUTO_INCREMENT, `Name` VARCHAR(24) NOT NULL DEFAULT '', `Ip` VARCHAR(18) NOT NULL DEFAULT '', `RangeBan` INT NOT NULL DEFAULT '0', `ExpireDate` INT NOT NULL DEFAULT '0', PRIMARY KEY (`BanId`))");
	    mysql_tquery(mysqlconn, "CREATE TABLE IF NOT EXISTS `BansData` (`BanId` INT, `Field` VARCHAR(32) NOT NULL DEFAULT '', `Value` VARCHAR(512) NOT NULL DEFAULT '')");
	#endif

	return 1;
}

stock Ban_Exit()
{
	#if !defined CONNECTION_TYPE_MYSQL
		db_close(bandb);
	#else
		mysql_close(mysqlconn);
	#endif

	return 1;
}

public OnPlayerConnect(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);

	new ip[18];
	GetPlayerIp(playerid, ip, sizeof (ip));

	new banid = GetBanId(name, ip);
	if (banid >= 0)
	{
	    #if defined OnBannedPlayerConnect
            if (!bool:OnBannedPlayerConnect(playerid, banid))
                return 1;
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

stock _Ban(playerid, const name[] = "", const ip[] = "", range = 0, expiredate = 0)
{
	new _name[MAX_PLAYER_NAME];
	new _ip[18];
	if (IsPlayerConnected(playerid))
	{
	    GetPlayerName(playerid, _name, MAX_PLAYER_NAME);
	    GetPlayerIp(playerid, _ip, sizeof (_ip));
	}
	else
	{
	    strcat(_name, name);
	    strcat(_ip, ip);
	}

	new query[512];
	format(query, sizeof (query), "SELECT * FROM `Bans` WHERE `Name` = '%q' AND `Ip` = '%q'", _name, _ip);

	#if !defined CONNECTION_TYPE_MYSQL
	    new DBResult:result;
	 	result = db_query(bandb, query);
	 	if (db_num_rows(result))
	    {
			db_free_result(result);
			return 0;
		}
		db_free_result(result);
	#else
	    new Cache:result;
	 	result = mysql_query(mysqlconn, query);
	 	if (cache_get_row_count(mysqlconn))
	    {
			cache_delete(result, mysqlconn);
			return 0;
		}
		cache_delete(result, mysqlconn);
	#endif

	format(query, sizeof (query), "INSERT INTO `Bans` (`Name`, `Ip`, `RangeBan`, `ExpireDate`) VALUES ('%q', '%q', %d, %d)", _name, _ip, bool:range, expiredate);

	#if !defined CONNECTION_TYPE_MYSQL
	    if (!db_query(bandb, query))
	        return 0;
	#else
	    mysql_tquery(mysqlconn, query);
	    if (mysql_errno(mysqlconn) != 0)
	        return 0;
	#endif

	Kick(playerid);
	return 1;
}
#if defined _ALS_Ban
	#undef Ban
#else
	#define _ALS_Ban
#endif
#define Ban _Ban

stock _BanEx(playerid, const reason[], const name[] = "", const ip[] = "", range = 0, expiredate = 0)
{
	new _name[MAX_PLAYER_NAME];
	new _ip[18];
	if (IsPlayerConnected(playerid))
	{
	    GetPlayerName(playerid, _name, MAX_PLAYER_NAME);
	    GetPlayerIp(playerid, _ip, sizeof (_ip));
	}
	else
	{
	    strcat(_name, name);
	    strcat(_ip, ip);
	}

	new query[512];
	format(query, sizeof (query), "SELECT * FROM `Bans` WHERE `Name` = '%q' AND `Ip` = '%q'", _name, _ip);

	#if !defined CONNECTION_TYPE_MYSQL
	    new DBResult:result;
	 	result = db_query(bandb, query);
	 	if (db_num_rows(result))
	    {
			db_free_result(result);
			return 0;
		}
		db_free_result(result);
	#else
	    new Cache:result;
	 	result = mysql_query(mysqlconn, query);
	 	if (cache_get_row_count(mysqlconn))
	    {
			cache_delete(result, mysqlconn);
			return 0;
		}
		cache_delete(result, mysqlconn);
	#endif

	format(query, sizeof (query), "INSERT INTO `Bans` (`Name`, `Ip`, `RangeBan`, `ExpireDate`) VALUES ('%q', '%q', %d, %d)", _name, _ip, bool:range, expiredate);

	#if !defined CONNECTION_TYPE_MYSQL
	    if (!db_query(bandb, query))
	        return 0;
	#else
	    mysql_tquery(mysqlconn, query);
	    if (mysql_errno(mysqlconn) != 0)
	        return 0;
	#endif

	new banid;
	#if !defined CONNECTION_TYPE_MYSQL
	    format(query, sizeof (query), "SELECT * FROM `Bans` WHERE `Name` = '%q', `Ip` = '%q'", _name, _ip);
	    result = db_query(bandb, query);
	    banid = db_get_field_assoc_int(result, "BanId");
	    db_free_result(result);
	#else
	    mysql_format(mysql_conn, query, sizeof (query), "SELECT * FROM `Bans` WHERE `Name` = '%q', `Ip` = '%q'", _name, _ip);
	    result = mysql_query(mysqlconn, query);
	    banid = cache_get_field_content_int(0, "BanId", mysqlconn);
	    cache_delete(result, mysqlconn);
	#endif

	format(query, sizeof (query), "INSERT INTO `BansData` (`BanId`, `Field`, `Value`) VALUES (%d, 'Reason', '%q')", banid, reason);

	#if !defined CONNECTION_TYPE_MYSQL
	    if (!db_query(bandb, query))
	        return 0;
	#else
	    mysql_tquery(mysqlconn, query);
	    if (mysql_errno(mysqlconn) != 0)
	        return 0;
	#endif

	Kick(playerid);
	return 1;
}
#if defined _ALS_BanEx
	#undef BanEx
#else
	#define _ALS_BanEx
#endif
#define BanEx _BanEx

stock GetBanId(const name[] = "", const ip[] = "")
{
	new query[512];
	format(query, sizeof (query), "SELECT `BanId`, `ExpireDate` FROM `Bans` WHERE `Name` = '%q' OR `Ip` = '%q'", name, ip);

	#if !defined CONNECTION_TYPE_MYSQL
	    new DBResult:result;
	 	result = db_query(bandb, query);
	    if (db_num_rows(result))
	    {
	    	new ret = db_get_field_int(result, 0);
	    	new expire = db_get_field_int(result, 1);
	        db_free_result(result);

	        if (expire >= 1 && expire < gettime())
	        {
	            format(query, sizeof (query), "DELETE FROM `Bans` WHERE `BanId` = %d", ret);
	    		db_query(bandb, query);
	            format(query, sizeof (query), "DELETE FROM `BansData` WHERE `BanId` = %d", ret);
	    		db_query(bandb, query);
	    		return -1;
	        }
			return ret;
		}
		db_free_result(result);
	#else
	    new Cache:result;
	 	result = mysql_query(mysqlconn, query);
	 	if (cache_get_row_count(mysqlconn))
	    {
	    	new ret = cache_get_row_int(0, 0, mysqlconn);
	    	new expire = cache_get_row_int(0, 1, mysqlconn);
	        cache_delete(result, mysqlconn);

	        if (expire >= 1 && expire < gettime())
	        {
	            format(query, sizeof (query), "DELETE FROM `Bans` WHERE `BanId` = %d", ret);
	    		mysql_tquery(mysqlconn, query);
	            format(query, sizeof (query), "DELETE FROM `BansData` WHERE `BanId` = %d", ret);
	    		mysql_tquery(mysqlconn, query);
	    		return -1;
	        }
			return ret;
		}
		cache_delete(result, mysqlconn);
	#endif

	#if !defined CONNECTION_TYPE_MYSQL
	 	result = db_query(bandb, "SELECT `BanId`, `Ip`, `ExpireDate` FROM `Bans` WHERE `RangeBan` = 1");
		if (db_num_rows(result) <= 0)
		{
	 		db_free_result(result);
			return -1;
		}
	#else
	 	result = mysql_query(mysqlconn, "SELECT `BanId`, `Ip`, `ExpireDate` FROM `Bans` WHERE `RangeBan` = 1");
	 	if (cache_get_row_count(mysqlconn) <= 0)
		{
	 		cache_delete(result, mysqlconn);
			return -1;
		}
	#endif

	new row_ip[18];

	#if !defined CONNECTION_TYPE_MYSQL
		do
		{
			db_get_field(result, 1, row_ip, sizeof (row_ip));
			if (BAN_ipmatch(row_ip, ip))
		    {
		    	new ret = db_get_field_int(result, 0);
		    	new expire = db_get_field_int(result, 2);
		        db_free_result(result);

		        if (expire >= 1 && expire < gettime())
		        {
		            format(query, sizeof (query), "DELETE FROM `Bans` WHERE `BanId` = %d", ret);
		    		db_free_result(db_query(bandb, query));
		            format(query, sizeof (query), "DELETE FROM `BansData` WHERE `BanId` = %d", ret);
		    		db_free_result(db_query(bandb, query));
		    		return -1;
		        }
				return ret;
		    }
		}
		while (db_next_row(result));

		db_free_result(result);
	#else
		for (new i, j = cache_get_row_count(mysqlconn); i < j; i++)
		{
			cache_get_row(i, 1, row_ip, mysqlconn);
			if (BAN_ipmatch(row_ip, ip))
		    {
		    	new ret = cache_get_row_int(0, 0, mysqlconn);
		    	new expire = cache_get_row_int(0, 2, mysqlconn);
	        	cache_delete(result, mysqlconn);

		        if (expire >= 1 && expire < gettime())
		        {
		            format(query, sizeof (query), "DELETE FROM `Bans` WHERE `BanId` = %d", ret);
		    		mysql_tquery(mysqlconn, query);
		            format(query, sizeof (query), "DELETE FROM `BansData` WHERE `BanId` = %d", ret);
		    		mysql_tquery(mysqlconn, query);
		    		return -1;
		        }
				return ret;
		    }
		}

		cache_delete(result, mysqlconn);
	#endif

	return -1;
}

stock UnBan(banid)
{
	new query[256];
    format(query, sizeof (query), "DELETE FROM `Bans` WHERE `BanId` = %d", banid);

	#if !defined CONNECTION_TYPE_MYSQL
	    if (!db_query(bandb, query))
	        return 0;
	#else
	    mysql_tquery(mysqlconn, query);
	    if (mysql_errno(mysqlconn) != 0)
	        return 0;
	#endif

	format(query, sizeof (query), "DELETE FROM `BansData` WHERE `BanId` = %d", banid);

	#if !defined CONNECTION_TYPE_MYSQL
	    if (!db_query(bandb, query))
	        return 0;
	#else
	    mysql_tquery(mysqlconn, query);
	    if (mysql_errno(mysqlconn) != 0)
	        return 0;
	#endif

	return 1;
}

#if !defined STRING
	#define STRING 	(0)
	#define TEXT 	(STRING)
	#define REAL 	(STRING)
#endif

#if !defined INTEGER
	#define INTEGER (1)
	#define NUMBER 	(INTEGER)
#endif

#if !defined FLOAT
	#define FLOAT 	(2)
#endif

stock static __getstringarg(dest[], arg, len = sizeof (dest))
{
    #emit LOAD.S.pri arg
    #emit SMUL.C     4
    #emit LOAD.S.alt 0
    #emit ADD
    #emit ADD.C      12
    #emit LOAD.I
    #emit PUSH.S     len
    #emit PUSH.pri
    #emit LOAD.S.alt dest
    #emit CONST.pri  0
    #emit STOR.I
    #emit PUSH.alt
    #emit PUSH.C     12
    #emit SYSREQ.C   strcat
    #emit STACK      16
}

stock SetBanData(banid, const field[], type, {Float, _}:...)
{
	new query[256];
    format(query, sizeof (query), "SELECT * FROM `BansData` WHERE `Field` = '%q' AND `BanId` = %d", field, banid);

	new value[512];
	switch (type)
	{
	    case STRING:
	        __getstringarg(value, 3);

		case INTEGER:
		    valstr(value, getarg(3));

		case FLOAT:
			format(value, sizeof (value), "%f", Float:getarg(3));
	}

	#if !defined CONNECTION_TYPE_MYSQL
	    new DBResult:result;
	 	result = db_query(bandb, query);
	    if (db_num_rows(result))
	    {
	        format(query, sizeof (query), "UPDATE `BansData` SET `Value` = '%q' WHERE `Field` = '%q' AND `BanId` = %d", value, field, banid);
			db_query(bandb, query);
        }
        else
        {
        	format(query, sizeof (query), "INSERT INTO `BansData` (`BanId`, `Field`, `Value`) VALUES (%d, '%q', '%q')", banid, field, value);
			db_query(bandb, query);
        }
		db_free_result(result);
	#else
	    new Cache:result;
	 	result = mysql_query(mysqlconn, query);
	 	if (cache_get_row_count(mysqlconn))
	    {
	        format(query, sizeof (query), "UPDATE `BansData` SET `Value` = '%q' WHERE `Field` = '%q' AND `BanId` = %d", value, field, banid);
			mysql_tquery(mysqlconn, query);
        }
        else
        {
        	format(query, sizeof (query), "INSERT INTO `BansData` (`BanId`, `Field`, `Value`) VALUES (%d, '%q', '%q')", banid, field, value);
			mysql_tquery(mysqlconn, query);
        }
		cache_delete(result, mysqlconn);
	#endif

	return 1;
}

stock static __setstringarg(arg, const value[], len = sizeof (value))
{
	new address;

    #emit LOAD.S.pri arg
    #emit SMUL.C     4
	#emit LOAD.S.alt 0
    #emit ADD
    #emit ADD.C      12
    #emit LOAD.I
	#emit STOR.S.PRI address
	#emit PUSH.S     len
	#emit PUSH.S     value
	#emit CONST.pri  0
	#emit SREF.S.pri address
	#emit PUSH.S     address
	#emit PUSH.C     12
	#emit SYSREQ.C   strcat
	#emit STACK      16
}

stock GetBanData(banid, const field[], type, {Float, _}:...)
{
	new query[256];
    format(query, sizeof (query), "SELECT `Value` FROM `BansData` WHERE `Field` = '%q' AND `BanId` = %d", field, banid);

	new value[512];

	#if !defined CONNECTION_TYPE_MYSQL
	    new DBResult:result;
	 	result = db_query(bandb, query);
	    if (db_num_rows(result))
	    {
			db_get_field(result, 0, value, sizeof (value));

	        switch (type)
			{
			    case STRING:
					__setstringarg(3, value);

			    case INTEGER, FLOAT:
			        setarg(3, 0, strval(value));
			}
	    }

		db_free_result(result);
	#else
	    new Cache:result;
	 	result = mysql_query(mysqlconn, query);
	 	if (cache_get_row_count(mysqlconn))
	    {
			cache_get_row(0, 0, value, mysqlconn);

	        switch (type)
			{
			    case STRING:
					__setstringarg(3, value);

			    case INTEGER, FLOAT:
			        setarg(3, 0, strval(value));
			}
	    }

		cache_delete(result, mysqlconn);
	#endif

	return 1;
}

#if defined OnBannedPlayerConnect
	forward OnBannedPlayerConnect(playerid, banid);
#endif
