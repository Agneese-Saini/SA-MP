#if defined easydb_included
	#endinput
#endif
#define easydb_included

/*
 * EasyDB
 * Version: 2.4.8 (last updated on 9 August, 2016)
 * By Gammix
 *
 * <a_mysql> by BlueG and madinat0r
 *
 * Functions:
   native DB::Version();
   native DB::Init(database[], host[] = "", user[] = "", password[] = "", port = 3306 , autoreconnect = true, pool_size = 2);
   native DB::Exit();
   native DB::VerifyTable(const table[], const primary_key[] = "ID", bool:defaults = false, {Float, _}:...);
   native DB::DeleteTable(const table[]);
   native DB::CreateRow(const table[], {Float, _}:...);
   native DB::DeleteRow(const table[], ARG:id, limit);
   native DB::Update(const table[], ARGS:id, limit, {Float, _}:...);
   native DB::Fetch(const table[], limit = -1, const sortfield[] = "", fieldtype = STRING, const condition[] = "", {Float, _}:...);
   native fetch_string(const field[], dest[], maxlength = sizeof (dest), cache = 0);
   native fetch_int(const field[], cache = 0);
   native Float:fetch_float(const field[], cache = 0);
   native fetch_row_id(cache = 0);
   native bool:fetch_next_row(cache = 0);
   native fetch_rows_count(cache = 0);
   native fetch_field_name(field, dest[], maxlength = sizeof (dest), cache = 0);
   native fetch_fields_count(cache = 0);
   native fetcher_close(cache = 0);
 *
 * Notes:
 * In order to use MySQL version of this, add "#define CONNECTION_TYPE_MYSQL" before including <easydb>.
 *
 */

#if defined CONNECTION_TYPE_MYSQL
	#tryinclude <a_mysql>

	#if !defined mysql_included
	    #undef CONNECTION_TYPE_MYSQL
	#endif
#endif

forward __DB_anti_crash();
public	__DB_anti_crash()
{
	strcat("", "");
	format("", 0, "");
}

#define DB:: DB_

#define DB_Open DB_Init
#define DB_Close DB_Exit

static bool:g_Initalized;

#if !defined MAX_SQL_SELECTION_INSTANCES
	#define MAX_SQL_SELECTION_INSTANCES (50)
#endif

#if !defined CONNECTION_TYPE_MYSQL
	static DBResult:g_Result[MAX_SQL_SELECTION_INSTANCES];
#else
	static Cache:g_Result[MAX_SQL_SELECTION_INSTANCES];
#endif
static g_ResultLastRow[MAX_SQL_SELECTION_INSTANCES];
static g_ResultRowsCount[MAX_SQL_SELECTION_INSTANCES];

#if !defined MAX_FIELD_VALUE
	#define MAX_FIELD_VALUE	1024
#endif

#if !defined CONNECTION_TYPE_MYSQL
	static g_DatabaseName[35];
	static DB:g_Database;
	static g_DatabaseTimer;

	stock static bool:DB::OpenSynchronousDB()
	{
		if (g_DatabaseTimer != 0)
			return true;

	    g_Database = db_open(g_DatabaseName);
	    if (g_Database == DB:0)
	        return false;

	    db_query(g_Database, "PRAGMA synchronous = OFF");

	 	g_DatabaseTimer = SetTimer("__OnSQLiteDBTimeout", 2500, false);
	 	return true;
	}

	forward __OnSQLiteDBTimeout();
	public  __OnSQLiteDBTimeout()
	{
		for (new i; i < MAX_SQL_SELECTION_INSTANCES; i++)
		{
			#if !defined CONNECTION_TYPE_MYSQL
		    	if (g_Result[i] != DBResult:0)
			#else
		    	if (g_Result[i] != Cache:0)
			#endif
		    {
		        g_DatabaseTimer = SetTimer("__OnSQLiteDBTimeout", 2500, false);
		        return;
			}
		}

		db_close(g_Database);
		g_DatabaseTimer = 0;
	}
#else
	static g_MySQLConnection;
#endif

static g_EasyDBVersion[] = "v3.0";

stock DB::Version()
{
	return g_EasyDBVersion;
}

stock DB::Init(database[], host[] = "", user[] = "", password[] = "", port = 3306, bool:autoreconnect = true, pool_size = 2)
{
	if (g_Initalized)
	{
	    #if defined ENABLE_CONSOLE_MESSAGES
			print("[EasyDB] - Warning: Database already initalized, use 'DB::Exit' to open a new db/conn.");
		#endif
	    return 0;
	}

	#if !defined CONNECTION_TYPE_MYSQL
    	strcat((g_DatabaseName[0] = EOS, g_DatabaseName), database);

	    if (host[0])
	    {
            #if defined ENABLE_CONSOLE_MESSAGES
				print("[EasyDB] - Warning: You are missing the <a_mysql> plugin/include or you haven't defined 'CONNECTION_TYPE_MYSQL' before including <easydb>; Database type is SQLite.");
			#endif
	    }

		print("[EasyDB]: <SQLite> Initializing database...");

	    #pragma unused host
		#pragma unused user
		//#pragma unused database
		#pragma unused password
		#pragma unused port
		#pragma unused autoreconnect
		#pragma unused pool_size

		#if defined ENABLE_CONSOLE_MESSAGES
			if (!fexist(database))
			    printf("[EasyDB] - Notice: Database file '%s' doesn't exist, attempting to create.", database);
		#endif

		if (DB::OpenSynchronousDB())
			print("[EasyDB] - Notice: SQLite database opened. Include is now intialized and operational.");
		else
		{
			print("[EasyDB] - Error: SQLite database was not opened, easydb not initialized.");
		    return 0;
		}
	#else
		print("[EasyDB]: <MySQL> Connecting database...");

	    g_MySQLConnection = mysql_connect(host, user, database, password, port, autoreconnect, pool_size);
	    if (mysql_errno(g_MySQLConnection) == 0)
	    	print("[EasyDB] - Notice: MySQL connection established. Include is now intialized and operational.");
	    else
	    {
			print("[EasyDB] - Error: MySQL connection was not established, easydb not initialized.");
	        return 0;
		}
	#endif

	g_Initalized = true;

	return 1;
}

stock DB::Exit()
{
	if (!g_Initalized)
		return 0;

	#if !defined CONNECTION_TYPE_MYSQL
	    KillTimer(g_DatabaseTimer);
	    db_close(g_Database);

		#if defined ENABLE_CONSOLE_MESSAGES
			print("[EasyDB] - Notice: SQLite database closed. You can now open a new database/connection.");
		#endif
	#else
	    mysql_close(g_MySQLConnection);

		#if defined ENABLE_CONSOLE_MESSAGES
			print("[EasyDB] - Notice: MySQL connection closed. You can now open a new database/connection.");
		#endif
	#endif

	g_Initalized = false;

	return 1;
}

#define INTEGER (1)
#define NUMBER 	(INTEGER)

#define FLOAT   (2)
#define REAL 	(FLOAT)

#define STRING  (3)
#define TEXT 	(STRING)

stock static DB::GetStringArg(dest[], arg, len = sizeof (dest))
{
    dest[0] = EOS;

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

stock DB::VerifyTable(const table[], const primary_key[] = "ID", bool:defaults = false, {Float, _}:...)
{
	if (!g_Initalized)
	    return 0;

    #if !defined CONNECTION_TYPE_MYSQL
		if (!DB::OpenSynchronousDB())
		    return 0;
	#endif

	static query[1024];
	#if !defined CONNECTION_TYPE_MYSQL
		format(query, sizeof (query), "PRAGMA table_info('%s')", table);
	    new DBResult:result = db_query(g_Database, query);
	#else
		format(query, sizeof (query), "SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = '%s'", table);
	    new Cache:result = mysql_query(g_MySQLConnection, query);
	#endif

    static fieldnames[1024][35];
    new fields;
    
	#if !defined CONNECTION_TYPE_MYSQL
		if (db_num_rows(result) > 0)
		{
		    do
				db_get_field_assoc(result, "name", fieldnames[fields++], sizeof (fieldnames[]));
			while
				(db_next_row(result));
		}
	#else
	    fields = cache_get_row_count(g_MySQLConnection);
		if (fields > 0)
		{
		    for (new i; i < fields; i++)
		    	cache_get_field_content(i, "Field", fieldnames[i], g_MySQLConnection);
		}
	#endif

	static query2[2048];
    if (fields > 0)
    {
        #pragma unused query2
	}
	else
	{
		#if defined ENABLE_CONSOLE_MESSAGES
			printf("[EasyDB] - Notice: Table '%s' doesn't exists, attempting to create.", table);
		#endif

		#if !defined CONNECTION_TYPE_MYSQL
			format(query2, sizeof (query2), "CREATE TABLE IF NOT EXISTS `%s` (`%s` INTEGER PRIMARY KEY", table, primary_key);
		#else
			format(query2, sizeof (query2), "CREATE TABLE IF NOT EXISTS `%s` (`%s` INT NOT NULL AUTO_INCREMENT", table, primary_key);
		#endif
	}

	static name[35];
    new type;

	new status;

	for (new i = 3, j = numargs(); i < j; i++)
	{
	    switch (status)
	    {
	        case 0:
	        {
	            status = 1;

	            DB::GetStringArg(name, i);

				if (fields > 0)
	    		{
					for (new x; x < fields; x++)
					{
						if (!strcmp(fieldnames[x], name, true))
						{
							if (defaults)
							  	i += 2;
							else
							    i += 1;

				      	 	status = 0;
				      	    break;
						}
					}
				}
			}

			case 1:
			{
				type = getarg(i);
			    if (type > STRING)
			        type = STRING;

			    if (defaults)
	            	status = 2;
				else
				{
					#if defined ENABLE_CONSOLE_MESSAGES
						printf("[EasyDB] - Notice: Column '%s' doesn't exists in table '%s', attempting to create.", name, table);
					#endif

	            	status = 0;

	            	switch (type)
	            	{
					    case INTEGER:
					    {
							if (fields > 0)
							{
   								format(query, sizeof (query), "ALTER TABLE `%s` ADD COLUMN `%s` INT NOT NULL DEFAULT '0'", table, name);

								#if !defined CONNECTION_TYPE_MYSQL
									db_query(g_Database, query);
								#else
									mysql_tquery(g_MySQLConnection, query);
								#endif
							}
							else
								format(query2, sizeof (query2), "%s, `%s` INT NOT NULL DEFAULT '0'", query2, name);
						}

						case FLOAT:
					    {
							if (fields > 0)
							{
								format(query, sizeof (query), "ALTER TABLE `%s` ADD COLUMN `%s` FLOAT NOT NULL DEFAULT '0.0'", table, name);

								#if !defined CONNECTION_TYPE_MYSQL
									db_query(g_Database, query);
								#else
									mysql_tquery(g_MySQLConnection, query);
								#endif
							}
							else
								format(query2, sizeof (query2), "%s, `%s` FLOAT NOT NULL DEFAULT '0.0'", query2, name);
   						}

					    case STRING:
					    {
							if (fields > 0)
							{
   								format(query, sizeof (query), "ALTER TABLE `%s` ADD COLUMN `%s` VARCHAR("#MAX_FIELD_VALUE") NOT NULL DEFAULT ''", table, name);

								#if !defined CONNECTION_TYPE_MYSQL
									db_query(g_Database, query);
								#else
									mysql_tquery(g_MySQLConnection, query);
								#endif
							}
							else
								format(query2, sizeof (query2), "%s, `%s` VARCHAR("#MAX_FIELD_VALUE") NOT NULL DEFAULT ''", query2, name);
						}
					}
	            }
			}

			case 2:
			{
				#if defined ENABLE_CONSOLE_MESSAGES
					printf("[EasyDB] - Notice: Column '%s' doesn't exists in table '%s', attempting to create.", name, table);
				#endif

	            status = 0;

				switch (type)
				{
				    case INTEGER:
				    {
						if (fields > 0)
						{
							format(query, sizeof (query), "ALTER TABLE `%s` ADD COLUMN `%s` INT NOT NULL DEFAULT '%i'", table, name, getarg(i));

							#if !defined CONNECTION_TYPE_MYSQL
								db_query(g_Database, query);
							#else
								mysql_tquery(g_MySQLConnection, query);
							#endif
						}
						else
							format(query2, sizeof (query2), "%s, `%s` INT NOT NULL DEFAULT '%i'", query2, name, getarg(i));
					}

					case FLOAT:
				    {
						if (fields > 0)
						{
							format(query, sizeof (query), "ALTER TABLE `%s` ADD COLUMN `%s` FLOAT NOT NULL DEFAULT '%f'", table, name, Float:getarg(i));

							#if !defined CONNECTION_TYPE_MYSQL
								db_query(g_Database, query);
							#else
								mysql_tquery(g_MySQLConnection, query);
							#endif
						}
						else
							format(query2, sizeof (query2), "%s, `%s` FLOAT NOT NULL DEFAULT '%f'", query2, name, Float:getarg(i));
					}

				    case STRING:
				    {
				        static val[256];
	            		DB::GetStringArg(val, i);

						if (fields > 0)
						{
							format(query, sizeof (query), "ALTER TABLE `%s` ADD COLUMN `%s` VARCHAR("#MAX_FIELD_VALUE") NOT NULL DEFAULT '%q'", table, name, val);
							#if !defined CONNECTION_TYPE_MYSQL
								db_query(g_Database, query);
							#else
								mysql_tquery(g_MySQLConnection, query);
							#endif
						}
						else
							format(query2, sizeof (query2), "%s, `%s` VARCHAR("#MAX_FIELD_VALUE") NOT NULL DEFAULT '%q'", query2, name, val);
					}
				}
			}
		}
	}

	if (fields <= 0)
	{
		#if !defined CONNECTION_TYPE_MYSQL
			strcat(query2, ")");

			if (db_query(g_Database, query2))
			{
				#if defined ENABLE_CONSOLE_MESSAGES
					printf("[EasyDB] - Notice: Table '%s' successfully verified.", table);
				#endif
			}
			else
			{
				#if defined ENABLE_CONSOLE_MESSAGES
					printf("[EasyDB] - Error: Something went wrong while verifying table '%s', columns may be missing.", table);
				#endif
				return 0;
			}
		#else
			format(query2, sizeof (query2), "%s, PRIMARY KEY (`%s`))", query2, primary_key);

			mysql_tquery(g_MySQLConnection, query2);
			if (mysql_errno(g_MySQLConnection) == 0)
			{
				#if defined ENABLE_CONSOLE_MESSAGES
					printf("[EasyDB] - Notice: Table '%s' successfully verified.", table);
				#endif
			}
			else
			{
				#if defined ENABLE_CONSOLE_MESSAGES
					printf("[EasyDB] - Error: Something went wrong while verifying table '%s', columns may be missing.", table);
				#endif
				return 0;
			}
		#endif
	}

	#if !defined CONNECTION_TYPE_MYSQL
		db_free_result(result);
	#else
	    cache_delete(result, g_MySQLConnection);
	#endif

	return 1;
}

stock DB::DeleteTable(const table[])
{
	if (!g_Initalized)
	    return 0;

    #if !defined CONNECTION_TYPE_MYSQL
		if (!DB::OpenSynchronousDB())
		    return 0;
	#endif

	static query[128];
	format(query, sizeof (query), "DROP TABLE `%s`", table);

	#if !defined CONNECTION_TYPE_MYSQL
		if (db_query(g_Database, query))
		{
			#if defined ENABLE_CONSOLE_MESSAGES
				printf("[EasyDB] - Notice: Table '%s' was successfully deleted.", table);
			#endif
		}
		else
		{
			#if defined ENABLE_CONSOLE_MESSAGES
				printf("[EasyDB] - Error: Table '%s' is either missing or the table's name is incorrect.", table);
			#endif
		}
	#else
	    mysql_tquery(g_MySQLConnection, query);
	    if (mysql_errno(g_MySQLConnection) == 0)
		{
			#if defined ENABLE_CONSOLE_MESSAGES
				printf("[EasyDB] - Notice: Table '%s' was successfully deleted.", table);
			#endif
		}
		else
		{
			#if defined ENABLE_CONSOLE_MESSAGES
				printf("[EasyDB] - Error: Table '%s' is either missing or the table's name is incorrect.", table);
			#endif
		}
	#endif

	return 1;
}

stock DB::CreateRow(const table[], {Float, _}:...)
{
	if (!g_Initalized)
	    return -1;

    #if !defined CONNECTION_TYPE_MYSQL
		if (!DB::OpenSynchronousDB())
		    return -1;
	#endif

	new limit = ((numargs() - 1) / 3);

	static query[2048];
	format(query, sizeof (query), "INSERT INTO `%s` (", table);

	new arg = 1;
	static field[35];
	for (new i; i < limit; i++)
	{
		DB::GetStringArg(field, arg);
		if (i != (limit - 1))
    		format(query, sizeof (query), "%s`%s`, ", query, field);
		else
    		format(query, sizeof (query), "%s`%s`) VALUES (", query, field);

		arg += 3;
	}

	arg = 2;
	static value[512];
	for (new i; i < limit; i++)
	{
	    switch (getarg(arg))
	    {
	        case INTEGER:
	        {
				if (i != (limit - 1))
	        		format(query, sizeof (query), "%s%i, ", query, getarg(arg + 1));
				else
	        		format(query, sizeof (query), "%s%i)", query, getarg(arg + 1));
			}

	        case FLOAT:
	        {
				if (i != (limit - 1))
					format(query, sizeof (query), "%s%f, ", query, Float:getarg(arg + 1));
				else
					format(query, sizeof (query), "%s%f)", query, Float:getarg(arg + 1));
			}

	        case STRING:
	        {
				DB::GetStringArg(value, (arg + 1));
				if (i != (limit - 1))
					format(query, sizeof (query), "%s'%q', ", query, value);
				else
					format(query, sizeof (query), "%s'%q')", query, value);
			}
		}
		arg += 3;
	}

	#if !defined CONNECTION_TYPE_MYSQL
		if (db_query(g_Database, query))
		{
			#if defined ENABLE_CONSOLE_MESSAGES
				printf("[EasyDB] - Notice: A new row has been inserted to table '%s'.", table);
			#endif
		}
		else
		{
			#if defined ENABLE_CONSOLE_MESSAGES
				printf("[EasyDB] - Error: A new row couldn't be inserted to table '%s'.", table);
			#endif
			return 0;
		}
	#else
		mysql_tquery(g_MySQLConnection, query);
		if (mysql_errno(g_MySQLConnection) == 0)
		{
			#if defined ENABLE_CONSOLE_MESSAGES
				printf("[EasyDB] - Notice: A new row has been inserted to table '%s'.", table);
			#endif
		}
		else
		{
			#if defined ENABLE_CONSOLE_MESSAGES
				printf("[EasyDB] - Error: A new row couldn't be inserted to table '%s'.", table);
			#endif
			return 0;
		}
	#endif

	return 1;
}

stock DB::DeleteRow(const table[], {Float, _}:...)
{
	if (!g_Initalized)
	    return -1;

    #if !defined CONNECTION_TYPE_MYSQL
		if (!DB::OpenSynchronousDB())
		    return -1;
	#endif

	new limit = getarg(2);

	static query[512];
	if (limit > 1)
	{
		format(query, sizeof (query), "DELETE FROM `%s` WHERE `ID` IN (", table);
		for (new i; i < limit; i++)
		{
		    if (i != (limit - 1))
				format(query, sizeof (query), "%s%i,", query, getarg(1, i));
			else
				format(query, sizeof (query), "%s%i)", query, getarg(1, i));
		}
	}
	else
		format(query, sizeof (query), "DELETE FROM `%s` WHERE `ID` = %i", table, getarg(1));

	#if !defined CONNECTION_TYPE_MYSQL
		if (db_query(g_Database, query))
		{
			#if defined ENABLE_CONSOLE_MESSAGES
				printf("[EasyDB] - Notice: Row(s) form table '%s' were successfully deleted.", table);
			#endif
		}
		else
		{
			#if defined ENABLE_CONSOLE_MESSAGES
				printf("[EasyDB] - Warnings: Some row(s) form table '%s' were not deleted.", table);
			#endif
		}
	#else
	    mysql_tquery(g_MySQLConnection, query);
	    if (mysql_errno(g_MySQLConnection) == 0)
		{
			#if defined ENABLE_CONSOLE_MESSAGES
				printf("[EasyDB] - Notice: Row(s) form table '%s' were successfully deleted.", table);
			#endif
		}
		else
		{
			#if defined ENABLE_CONSOLE_MESSAGES
				printf("[EasyDB] - Warnings: Some row(s) form table '%s' were not deleted.", table);
			#endif
		}
	#endif

	return 1;
}

stock DB::Update({Float, _}:...)
{
	if (!g_Initalized)
	    return 0;

    #if !defined CONNECTION_TYPE_MYSQL
		if (!DB::OpenSynchronousDB())
		    return 0;
	#endif

	static table[35];
    DB::GetStringArg(table, 0);

	new limit = getarg(2);

	new limit2 = ((numargs() - 3) / 3);

	static query[2048];
	format(query, sizeof (query), "UPDATE `%s` SET ", table);

	new arg = 4;
	static field[35];
	static value[512];

	for (new i; i < limit2; i++)
	{
		switch (getarg(arg))
		{
		    case INTEGER:
		    {
		        DB::GetStringArg(field, (arg - 1));
		        if (i != (limit2 - 1))
		        	format(query, sizeof (query), "%s`%s` = %i, ", query, field, getarg(arg + 1));
				else
		        	format(query, sizeof (query), "%s`%s` = %i", query, field, getarg(arg + 1));
		    }

		    case FLOAT:
		    {
		        DB::GetStringArg(field, (arg - 1));
		        if (i != (limit2 - 1))
		        	format(query, sizeof (query), "%s`%s` = %f, ", query, field, Float:getarg(arg + 1));
				else
		        	format(query, sizeof (query), "%s`%s` = %f", query, field, getarg(arg + 1));
		    }

		    case STRING:
		    {
		        DB::GetStringArg(field, (arg - 1));
		        DB::GetStringArg(value, (arg + 1));
		        if (i != (limit2 - 1))
		        	format(query, sizeof (query), "%s`%s` = '%q', ", query, field, value);
				else
		        	format(query, sizeof (query), "%s`%s` = '%q'", query, field, value);
		    }
		}

		arg += 3;
	}

	if (limit > 1)
	{
		strcat(query, " WHERE `ID` IN (");
		for (new i; i < limit; i++)
		{
		    if (i != (limit - 1))
				format(query, sizeof (query), "%s%i,", query, getarg(1, i));
			else
				format(query, sizeof (query), "%s%i)", query, getarg(1, i));
		}
	}
	else
		format(query, sizeof (query), "%s WHERE `ID` = %i", query, getarg(1));

    #if !defined CONNECTION_TYPE_MYSQL
		if (!db_query(g_Database, query))
		{
			#if defined ENABLE_CONSOLE_MESSAGES
				print("[EasyDB] - Error: 'DB::Update' function didn't executed, might be invalid columns' names.");
			#endif
		}
	#else
		mysql_tquery(g_MySQLConnection, query);
		#if defined ENABLE_CONSOLE_MESSAGES
			if (mysql_errno(g_MySQLConnection) != 0)
			{
				print("[EasyDB] - Error: 'DB::Update' function didn't executed, might be invalid columns' names.");
			}
		#endif
	#endif

	return 1;
}

stock static DB::ReturnStringArg(const fmat[], STATIC_ARGS)
{
 	static out[512];
	static size = 512;

	new num_args, arg_start, arg_end;


	#emit LOAD.S.pri   0
	#emit ADD.C        8
	#emit MOVE.alt

	#emit LOAD.I
	#emit STOR.S.pri   num_args

	#emit ADD
	#emit STOR.S.pri   arg_end

	#emit LOAD.S.pri   STATIC_ARGS
	#emit SMUL.C       4
	#emit ADD
	#emit STOR.S.pri   arg_start

	while (arg_end != arg_start)
	{
		#emit MOVE.pri
		#emit LOAD.I
		#emit PUSH.pri
		#emit CONST.pri    4
		#emit SUB.alt
		#emit STOR.S.pri   arg_end
	}

	#emit PUSH.S       fmat
	#emit PUSH         size
	#emit PUSH.C       out

	#emit LOAD.S.pri   num_args
	#emit ADD.C        12
	#emit LOAD.S.alt   STATIC_ARGS
	#emit XCHG
	#emit SMUL.C       4
	#emit SUB.alt
	#emit PUSH.pri
	#emit MOVE.alt

	#emit SYSREQ.C     format

	#emit CONST.pri    4
	#emit ADD
	#emit MOVE.alt

	#emit LCTRL        4
	#emit ADD
	#emit SCTRL        4

	return out;
}

stock static DB::SetStringArg(arg, const value[], len = sizeof (value))
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

stock DB::Fetch(const table[], limit = -1, const sortfield[] = "", fieldtype = STRING, const condition[] = "", {Float, _}:...)
{
	if (!g_Initalized)
	    return -1;

    #if !defined CONNECTION_TYPE_MYSQL
		if (!DB::OpenSynchronousDB())
		    return -1;
	#endif

	static query[512];
	format(query, sizeof (query), "SELECT * FROM `%s`", table);

	if (condition[0])
		format(query, sizeof (query), "%s WHERE %s", query, DB::ReturnStringArg(condition, 5));

	if (sortfield[0])
	{
	    switch (fieldtype)
	    {
	        case STRING:
	        	format(query, sizeof (query), "%s ORDER BY LENGTH(`%s`) DESC", query, sortfield);

	        case INTEGER, FLOAT:
				format(query, sizeof (query), "%s ORDER BY `%s` DESC", query, sortfield);
	    }
	}

	if (limit > 0)
		format(query, sizeof (query), "%s LIMIT %i", query, limit);

	new id = -1;
	for (new i; i < MAX_SQL_SELECTION_INSTANCES; i++)
	{
		#if !defined CONNECTION_TYPE_MYSQL
	    	if (g_Result[i] == DBResult:0)
		#else
	    	if (g_Result[i] == Cache:0)
		#endif
	    {
	        id = i;
	        break;
	    }
	}

	if (id == -1)
	{
		#if !defined CONNECTION_TYPE_MYSQL
			db_free_result(g_Result[0]);
		#else
 			cache_delete(g_Result[0], g_MySQLConnection);
		#endif

		id = 0;
	}

	#if !defined CONNECTION_TYPE_MYSQL
		g_Result[id] = db_query(g_Database, query);
		if (!g_Result[id])
		{
		    #if defined ENABLE_CONSOLE_MESSAGES
				print("[EasyDB] - Error: 'DB::Fetch' function didn't executed, might be invalid condition or table name.");
			#endif
			return -1;
		}

		g_ResultRowsCount[id] = db_num_rows(g_Result[id]);
	#else
		g_Result[id] = mysql_query(g_MySQLConnection, query);
		g_ResultRowsCount[id] = cache_get_row_count(g_MySQLConnection);

		if (mysql_errno(g_MySQLConnection) != 0)
		{
		    #if defined ENABLE_CONSOLE_MESSAGES
				print("[EasyDB] - Error: 'DB::Fetch' function didn't executed, might be invalid condition or table name.");
			#endif
			return -1;
		}
	#endif

	g_ResultLastRow[id] = 0;

	return id;
}

#if defined CONNECTION_TYPE_MYSQL
	forward __OnMySQLSelect(id);
	public  __OnMySQLSelect(id)
	{
	    g_Result[id] = cache_save(g_MySQLConnection);
		g_ResultRowsCount[id] = cache_get_row_count(g_MySQLConnection);
	}
#endif

stock fetch_string(const field[], dest[], maxlength = sizeof (dest), cache = 0)
{
	if (cache < 0 || cache >= MAX_SQL_SELECTION_INSTANCES)
	    return 0;

	#if !defined CONNECTION_TYPE_MYSQL
		if (g_Result[cache] == DBResult:0)
	    return 0;
	#else
		if (g_Result[cache] == Cache:0)
	    return 0;
	#endif

	if (g_ResultRowsCount[cache] == 0)
	    return 0;

	#if !defined CONNECTION_TYPE_MYSQL
    	db_get_field_assoc(g_Result[cache], field, dest, maxlength);
	#else
	    cache_set_active(g_Result[cache], g_MySQLConnection);
	    cache_get_field_content(g_ResultLastRow[cache], field, dest, g_MySQLConnection, maxlength);
	#endif

	return 1;
}

stock fetch_int(const field[], cache = 0)
{
	if (cache < 0 || cache >= MAX_SQL_SELECTION_INSTANCES)
	    return -1;

	#if !defined CONNECTION_TYPE_MYSQL
		if (g_Result[cache] == DBResult:0)
	    return -1;
	#else
		if (g_Result[cache] == Cache:0)
	    return -1;
	#endif

	if (g_ResultRowsCount[cache] == 0)
	    return -1;

	#if !defined CONNECTION_TYPE_MYSQL
    	return db_get_field_assoc_int(g_Result[cache], field);
	#else
	    cache_set_active(g_Result[cache], g_MySQLConnection);
	    return cache_get_field_content_int(g_ResultLastRow[cache], field, g_MySQLConnection);
	#endif
}

stock Float:fetch_float(const field[], cache = 0)
{
	if (cache < 0 || cache >= MAX_SQL_SELECTION_INSTANCES)
	    return -1.0;

	#if !defined CONNECTION_TYPE_MYSQL
		if (g_Result[cache] == DBResult:0)
	    return -1.0;
	#else
		if (g_Result[cache] == Cache:0)
	    return -1.0;
	#endif

	if (g_ResultRowsCount[cache] == 0)
	    return -1.0;

	#if !defined CONNECTION_TYPE_MYSQL
    	return db_get_field_assoc_float(g_Result[cache], field);
	#else
	    cache_set_active(g_Result[cache], g_MySQLConnection);
	    return cache_get_field_content_float(g_ResultLastRow[cache], field, g_MySQLConnection);
	#endif
}

stock fetch_row_id(cache = 0)
{
	if (cache < 0 || cache >= MAX_SQL_SELECTION_INSTANCES)
	    return -1;

	#if !defined CONNECTION_TYPE_MYSQL
		if (g_Result[cache] == DBResult:0)
	    return -1;
	#else
		if (g_Result[cache] == Cache:0)
	    return -1;
	#endif

	if (g_ResultRowsCount[cache] == 0)
	    return 0;

	#if !defined CONNECTION_TYPE_MYSQL
    	return db_get_field_int(g_Result[cache], 0);
	#else
	    cache_set_active(g_Result[cache], g_MySQLConnection);
	    return cache_get_row_int(g_ResultLastRow[cache], 0, g_MySQLConnection);
	#endif
}

stock bool:fetch_next_row(cache = 0)
{
	if (cache < 0 || cache >= MAX_SQL_SELECTION_INSTANCES)
	    return false;

	#if !defined CONNECTION_TYPE_MYSQL
		if (g_Result[cache] == DBResult:0)
	    return false;
	#else
		if (g_Result[cache] == Cache:0)
	    return false;
	#endif

	#if !defined CONNECTION_TYPE_MYSQL
	    if ((g_ResultLastRow[cache] + 1) >= g_ResultRowsCount[cache])
	        return false;

		if (db_next_row(g_Result[cache]))
		{
            g_ResultLastRow[cache]++;
		    return true;
		}

		return false;
	#else
	    if ((g_ResultLastRow[cache] + 1) >= g_ResultRowsCount[cache])
	        return false;

        g_ResultLastRow[cache]++;
        return true;
	#endif
}

stock fetch_rows_count(cache = 0)
{
	if (cache < 0 || cache >= MAX_SQL_SELECTION_INSTANCES)
	    return 0;

	#if !defined CONNECTION_TYPE_MYSQL
		if (g_Result[cache] == DBResult:0)
	    return 0;
	#else
		if (g_Result[cache] == Cache:0)
	    return 0;
	#endif

	return g_ResultRowsCount[cache];
}

stock fetch_field_name(field, dest[], maxlength = sizeof (dest), cache = 0)
{
	if (cache < 0 || cache >= MAX_SQL_SELECTION_INSTANCES)
	    return 0;

	#if !defined CONNECTION_TYPE_MYSQL
		if (g_Result[cache] == DBResult:0)
	    return 0;
	#else
		if (g_Result[cache] == Cache:0)
	    return 0;
	#endif

    #if !defined CONNECTION_TYPE_MYSQL
    	return db_field_name(g_Result[cache], field, dest, maxlength);
	#else
	    return cache_get_field_name(field, dest, g_MySQLConnection, maxlength);
	#endif
}

stock fetch_fields_count(cache = 0)
{
	if (cache < 0 || cache >= MAX_SQL_SELECTION_INSTANCES)
	    return 0;

	#if !defined CONNECTION_TYPE_MYSQL
		if (g_Result[cache] == DBResult:0)
	    return 0;
	#else
		if (g_Result[cache] == Cache:0)
	    return 0;
	#endif

	#if !defined CONNECTION_TYPE_MYSQL
    	return db_num_fields(g_Result[cache]);
	#else
	    cache_set_active(g_Result[cache], g_MySQLConnection);
	    return cache_get_field_count(g_MySQLConnection);
	#endif
}

stock fetcher_close(cache = 0)
{
	if (cache < 0 || cache >= MAX_SQL_SELECTION_INSTANCES)
	    return 0;

	#if !defined CONNECTION_TYPE_MYSQL
		if (g_Result[cache] == DBResult:0)
	    return 0;
	#else
		if (g_Result[cache] == Cache:0)
	    return 0;
	#endif

	#if !defined CONNECTION_TYPE_MYSQL
		db_free_result(g_Result[cache]);
		g_Result[cache] = DBResult:0;
	#else
		cache_delete(g_Result[cache], g_MySQLConnection);
		g_Result[cache] = Cache:0;
	#endif

	return 1;
}
