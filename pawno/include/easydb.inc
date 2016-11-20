// easydb.inc v2.5 - by Gammix - updated on 20 Nov, 2016
#if defined easydb_included
	#endinput
#endif
#define easydb_included

/*
native DBIndex: DB_SELECT(const table[], bool: store_result = true, const sortfield[] = "", sorttype = SORT_ASCE, const condition[] = "", {Float, _}:...);
native DB_GetFieldsCount(DBIndex: idx = DBIndex: 0);
native DB_GetFieldName(fieldid, dest[], maxlength = sizeof (dest), DBIndex: idx = DBIndex: 0);
native DB_GetRowsCount(DBIndex: idx = DBIndex: 0);
native DB_GetRowInt(const field[], DBIndex: idx = DBIndex: 0);
native Float: DB_GetRowFloat(const field[], DBIndex: idx = DBIndex: 0);
native DB_GetRowString(const field[], dest[], maxlength = sizeof (dest), DBIndex: idx = DBIndex: 0);
native DB_GetRowId(DBIndex: idx = DBIndex: 0);
native DB_SetRowId(rowid, DBIndex: idx = DBIndex: 0);
native _
native DBIndex: DB_UPDATE(const table[], const condition[] = "", {Float, _}:...);
native DBIndex: DB_INSERT(const table[]);
native DB_SetRowInt(const field[], value, DBIndex: idx = DBIndex: 0);
native DB_SetRowFloat(const field[], Float: value, DBIndex: idx = DBIndex: 0);
native DB_SetRowString(const field[], value[], DBIndex: idx = DBIndex: 0);
native _
native DBIndex: DB_VERIFY_TABLE(const table[]);
native DB_IntField(const field[], defaultvalue = 0, DBIndex: idx = DBIndex: 0);
native DB_FloatField(const field[], Float: defaultvalue = 0.0, DBIndex: idx = DBIndex: 0);
native DB_StringField(const field[], defaultvalue[] = "", DBIndex: idx = DBIndex: 0);
native DB_PrimaryKey(const field[], DBIndex: idx = DBIndex: 0);
native DB_ForiegnKey(const field[], const assoctable[], const assocfield[], DBIndex: idx = DBIndex: 0);
native _
native DB_CLOSE(DBIndex: idx = DBIndex: 0);
native _
native DB_DELETE_TABLE(const table[]);
native _
native DB_DELETE_ROW(const table[], const condition[] = "", {Float, _}:...);
*/

#if !defined SQL_DATABASE
	#define SQL_DATABASE "database"
#endif

/*
User must define these themselves before including easydb in order to initialize MySQL connection.

#define SQL_SERVER ""
#define SQL_USERNAME ""
#define SQL_PASSWORD ""
*/

#if defined SQL_SERVER
	#if !defined SQL_USERNAME
	    #define SQL_USERNAME "root"
	#endif

	#if !defined SQL_PASSWORD
	    #define SQL_PASSWORD ""
	#endif

	#include <a_mysql>
#endif

static bool: DBInitiated;

#if defined SQL_SERVER
	static MySQL: DBId;
#else
	static DB: DBId;
#endif

enum
{
	STMT_NONE,
	STMT_SELECT,
	STMT_UPDATE,
	STMT_INSERT,
	STMT_VERIFY_TABLE
};

enum
{
	SORT_ASCE,
	SORT_DESC
};

#define DB_MAX_TABLE_NAME (32)
#define DB_MAX_CONDITION_SIZE (256)
#define DB_MAX_FIELDS (25)

enum e_DBINDEX
{
    e_DBINDEX_STATEMENT,

    // For "SELECT" statments
    #if defined SQL_SERVER
    	Cache: e_DBINDEX_RESULT,
	#else
	    DBResult: e_DBINDEX_RESULT,
		e_DBINDEX_LASTROWID,
	#endif
	e_DBINDEX_ROWID,

    // For "INSERT" & "UPDATE" statments
    e_DBINDEX_TABLE[DB_MAX_TABLE_NAME],
    e_DBINDEX_CONDITION[DB_MAX_CONDITION_SIZE],

    // For "CREATE TABLE" statements
    bool: e_DBINDEX_TABLE_CREATED,
    e_DBINDEX_PRIMARY_KEY
};

#define DB_MAX_INDEXES (12)
static eDBIndex[DBIndex: DB_MAX_INDEXES][e_DBINDEX];
static iDBIndexPoolSize;

#define DB_MAX_FIELD_NAME (DB_MAX_TABLE_NAME)
#define DB_MAX_FIELD_VALUE (512)
static sDBIndexFields[DBIndex: DB_MAX_INDEXES][DB_MAX_FIELDS][DB_MAX_FIELD_NAME];
static sDBIndexValues[DBIndex: DB_MAX_INDEXES][DB_MAX_FIELDS][DB_MAX_FIELD_VALUE];
static sDBIndexFieldTypes[DBIndex: DB_MAX_INDEXES][DB_MAX_FIELDS][DB_MAX_FIELD_VALUE];

static iDBIndexFieldsPoolSize[DBIndex: DB_MAX_INDEXES];

forward __DB_Fix();
public  __DB_Fix()
{
	strcat("", "");
	format("", 0, "");
}

stock static DB_ReturnStringArg(const fmat[], STATIC_ARGS) // By Y_Less
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

stock static DB_GetStringArg(dest[], arg, len = sizeof (dest)) // By Slice
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

stock static DB_Init()
{
	if (DBInitiated)
	    return 1;

	#if defined SQL_SERVER
		mysql_log(ERROR | DEBUG);

	    new MySQLOpt: options = mysql_init_options();
		mysql_set_option(options, AUTO_RECONNECT, false);
		mysql_set_option(options, POOL_SIZE, 0);

		DBId = mysql_connect(SQL_SERVER, SQL_USERNAME, SQL_PASSWORD, SQL_DATABASE, options);
		if (mysql_errno() != 0)
		    return false;
	#else
	    DBId = db_open(SQL_DATABASE ".db");
		if (!DBId)
		    return false;

        db_query(DBId, "PRAGMA synchronous = NORMAL");
        db_query(DBId, "PRAGMA journal_mode = WAL");

        db_debug_openresults();
	#endif

	DBInitiated = true;
	return 1;
}

stock DBIndex: DB_SELECT(const table[], bool: store_result = true, const sortfield[] = "", sorttype = SORT_ASCE, const condition[] = "", {Float, _}:...)
{
	if (!DB_Init())
	    return DBIndex: -1;

	if (iDBIndexPoolSize == DB_MAX_INDEXES)
	    return DBIndex: -1;

	new DBIndex: index = DBIndex: iDBIndexPoolSize;
	iDBIndexPoolSize++;

	eDBIndex[index][e_DBINDEX_STATEMENT] = STMT_SELECT;

	new string[50 + DB_MAX_CONDITION_SIZE];
	if (!condition[0])
		format(string, sizeof (string), "SELECT * FROM `%s`", table);
	else
		format(string, sizeof (string), "SELECT * FROM `%s` WHERE %s", table, DB_ReturnStringArg(condition, 5));

	if (sortfield[0])
		format(string, sizeof (string), "%s ORDER BY `%s` %s", string, sortfield, ((sorttype == SORT_ASCE) ? ("ASC") : ("DESC")));

	#if defined SQL_SERVER
		eDBIndex[index][e_DBINDEX_RESULT] = mysql_query(DBId, string, true);

		new rows;
		cache_get_row_count(rows);
		if (!eDBIndex[index][e_DBINDEX_RESULT] || rows < 0)
		{
		    cache_delete(eDBIndex[index][e_DBINDEX_RESULT]);

		    eDBIndex[index][e_DBINDEX_STATEMENT] = STMT_NONE;
		    iDBIndexPoolSize--;
		    return DBIndex: -1;
		}

		if (!store_result)
			return DBIndex: -2;
	#else
		eDBIndex[index][e_DBINDEX_RESULT] = db_query(DBId, string);
		eDBIndex[index][e_DBINDEX_LASTROWID] = 0;

		if (!eDBIndex[index][e_DBINDEX_RESULT] || db_num_rows(eDBIndex[index][e_DBINDEX_RESULT]) < 0)
		{
		    db_free_result(eDBIndex[index][e_DBINDEX_RESULT]);

		    eDBIndex[index][e_DBINDEX_STATEMENT] = STMT_NONE;
		    iDBIndexPoolSize--;
		    return DBIndex: -1;
		}
		
		if (!store_result)
			return DBIndex: -2;
	#endif

	eDBIndex[index][e_DBINDEX_ROWID] = 0;

	return index;
}

stock DB_GetFieldsCount(DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return 0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return 0;

    #if defined SQL_SERVER
    	cache_set_active(eDBIndex[idx][e_DBINDEX_RESULT]);

		new fields;
		cache_get_field_count(fields);
		return fields;
	#else
		return db_num_rows(eDBIndex[idx][e_DBINDEX_RESULT]);
	#endif
}

stock DB_GetFieldName(fieldid, dest[], maxlength = sizeof (dest), DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return 0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return 0;

    #if defined SQL_SERVER
    	cache_set_active(eDBIndex[idx][e_DBINDEX_RESULT]);
		return cache_get_field_name(feildid, dest, maxlength);
	#else
		return db_get_field_name(eDBIndex[idx][e_DBINDEX_RESULT], fieldid, dest, maxlength);
	#endif
}

stock DB_GetRowsCount(DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return 0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return 0;

    #if defined SQL_SERVER
    	cache_set_active(eDBIndex[idx][e_DBINDEX_RESULT]);

		new rows;
		cache_get_row_count(rows);
		return rows;
	#else
		return db_num_rows(eDBIndex[idx][e_DBINDEX_RESULT]);
	#endif
}

stock DB_GetRowInt(const field[], DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return -1;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return -1;

    #if defined SQL_SERVER
    	cache_set_active(eDBIndex[idx][e_DBINDEX_RESULT]);

		new get;
		cache_get_value_name_int(eDBIndex[idx][e_DBINDEX_ROWID], field, get);
		return get;
	#else
		return db_get_field_assoc_int(eDBIndex[idx][e_DBINDEX_RESULT], field);
	#endif
}

stock Float: DB_GetRowFloat(const field[], DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return -1.0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return -1.0;

    #if defined SQL_SERVER
    	cache_set_active(eDBIndex[idx][e_DBINDEX_RESULT]);

		new Float: get;
		cache_get_value_name_float(eDBIndex[idx][e_DBINDEX_ROWID], field, get);
		return get;
	#else
		return db_get_field_assoc_float(eDBIndex[idx][e_DBINDEX_RESULT], field);
	#endif
}

stock DB_GetRowString(const field[], dest[], maxlength = sizeof (dest), DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return 0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return 0;

    #if defined SQL_SERVER
    	cache_set_active(eDBIndex[idx][e_DBINDEX_RESULT]);

		cache_get_value_name(eDBIndex[idx][e_DBINDEX_ROWID], field, dest, maxlength);
		return 1;
	#else
		db_get_field_assoc(eDBIndex[idx][e_DBINDEX_RESULT], field, dest, maxlength);
		return 1;
	#endif
}

stock DB_GetRowId(DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return 0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return 0;

    return eDBIndex[idx][e_DBINDEX_ROWID];
}

stock DB_SetRowId(rowid, DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return 0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return 0;

    #if defined SQL_SERVER
   		new rows;
		cache_get_row_count(rows);

	    if (!(0 < rowid < rows))
	        return 0;

		eDBIndex[idx][e_DBINDEX_ROWID] = rowid;
	#else
	    if (!(0 < rowid < db_num_rows(eDBIndex[idx][e_DBINDEX_RESULT])))
	        return 0;

		eDBIndex[idx][e_DBINDEX_LASTROWID] = eDBIndex[idx][e_DBINDEX_ROWID];
		while (db_next_row(eDBIndex[idx][e_DBINDEX_RESULT]))
		{
		    eDBIndex[idx][e_DBINDEX_ROWID]++;
		    if (eDBIndex[idx][e_DBINDEX_ROWID] == rowid)
		        break;
		}

        if (rowid < eDBIndex[idx][e_DBINDEX_ROWID])
        {
        	print("[EasyDB] - Error: SQLite doesn't support switching to previous rows to collect data. You have to use DB_SELECT again to access rows from starting at 0.");
        	return 0;
		}
	#endif

	return 1;
}

stock DBIndex: DB_UPDATE(const table[], const condition[], {Float, _}:...)
{
	if (!DB_Init())
	    return DBIndex: -1;

	if (iDBIndexPoolSize == DB_MAX_INDEXES)
	    return DBIndex: -1;

	new DBIndex: index = DBIndex: iDBIndexPoolSize;
	iDBIndexPoolSize++;

	eDBIndex[index][e_DBINDEX_STATEMENT] = STMT_UPDATE;
	format(eDBIndex[index][e_DBINDEX_TABLE], DB_MAX_TABLE_NAME, table);
	format(eDBIndex[index][e_DBINDEX_CONDITION], DB_MAX_CONDITION_SIZE, DB_ReturnStringArg(condition, 2));
	iDBIndexFieldsPoolSize[index] = 0;

	return index;
}

stock DBIndex: DB_INSERT(const table[])
{
	if (!DB_Init())
	    return DBIndex: -1;

	if (iDBIndexPoolSize == DB_MAX_INDEXES)
	    return DBIndex: -1;

	new DBIndex: index = DBIndex: iDBIndexPoolSize;
	iDBIndexPoolSize++;

	eDBIndex[index][e_DBINDEX_STATEMENT] = STMT_INSERT;
	format(eDBIndex[index][e_DBINDEX_TABLE], DB_MAX_TABLE_NAME, table);
	iDBIndexFieldsPoolSize[index] = 0;

	return index;
}

stock DB_SetRowInt(const field[], value, DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return 0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return 0;

	for (new i; i < iDBIndexFieldsPoolSize[idx]; i++)
	{
	    if (!strcmp(field, sDBIndexFields[idx][i], true))
	    {
	        valstr(sDBIndexValues[idx][i], value);
	        return 1;
	    }
	}

	if (iDBIndexFieldsPoolSize[idx] == DB_MAX_FIELDS)
	    return 0;

	new index = iDBIndexFieldsPoolSize[idx];
	iDBIndexFieldsPoolSize[idx]++;

	format(sDBIndexFields[idx][index], DB_MAX_FIELD_NAME, field);
	valstr(sDBIndexValues[idx][index], value);
	return 1;
}

stock DB_SetRowFloat(const field[], Float: value, DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return 0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return 0;

	for (new i; i < iDBIndexFieldsPoolSize[idx]; i++)
	{
	    if (!strcmp(field, sDBIndexFields[idx][i], true))
	    {
	        format(sDBIndexValues[idx][i], DB_MAX_FIELD_VALUE, "%f", value);
	        return 1;
	    }
	}

	if (iDBIndexFieldsPoolSize[idx] == DB_MAX_FIELDS)
	    return 0;

	new index = iDBIndexFieldsPoolSize[idx];
	iDBIndexFieldsPoolSize[idx]++;

	format(sDBIndexFields[idx][index], DB_MAX_FIELD_NAME, field);
	format(sDBIndexValues[idx][index], DB_MAX_FIELD_VALUE, "%f", value);
	return 1;
}

stock DB_SetRowString(const field[], value[], DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return 0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return 0;

	for (new i; i < iDBIndexFieldsPoolSize[idx]; i++)
	{
	    if (!strcmp(field, sDBIndexFields[idx][i], true))
	    {
	        format(sDBIndexValues[idx][i], DB_MAX_FIELD_VALUE, value);
	        return 1;
	    }
	}

	if (iDBIndexFieldsPoolSize[idx] == DB_MAX_FIELDS)
	    return 0;

	new index = iDBIndexFieldsPoolSize[idx];
	iDBIndexFieldsPoolSize[idx]++;

	format(sDBIndexFields[idx][index], DB_MAX_FIELD_NAME, field);
	format(sDBIndexValues[idx][index], DB_MAX_FIELD_VALUE, value);
	return 1;
}

stock DBIndex: DB_VERIFY_TABLE(const table[])
{
	if (!DB_Init())
	    return DBIndex: -1;

	if (iDBIndexPoolSize == DB_MAX_INDEXES)
	    return DBIndex: -1;

	new DBIndex: index = DBIndex: iDBIndexPoolSize;
	iDBIndexPoolSize++;

	eDBIndex[index][e_DBINDEX_STATEMENT] = STMT_VERIFY_TABLE;
	format(eDBIndex[index][e_DBINDEX_TABLE], DB_MAX_TABLE_NAME, table);
	iDBIndexFieldsPoolSize[index] = 0;

	#if defined SQL_SERVER
	    new string[256];
		format(string, sizeof (string), "SELECT * FROM `INFORMATION_SCHEMA.COLUMNS` WHERE `TABLE_NAME` = '%s'", table);
	    new Cache: result = mysql_query(DBId, string);

		cache_get_row_count(iDBIndexFieldsPoolSize[idx]);
		if (iDBIndexFieldsPoolSize[idx] > 0)
		{
			eDBIndex[idx][e_DBINDEX_TABLE_CREATED] = true;

		    if (iDBIndexFieldsPoolSize[idx] > DB_MAX_FIELDS)
			    printf("[EasyDB] - Warning: There are more fields/columns in \"%s\" table than \"DB_MAX_FIELDS\" (%i).", iDBIndexFieldsPoolSize[idx], DB_MAX_FIELDS);

		    for (new i, j = ((iDBIndexFieldsPoolSize[idx] > DB_MAX_FIELDS) ? (DB_MAX_FIELDS) : (iDBIndexFieldsPoolSize[idx])); i < j; i++)
		    {
		    	cache_get_value_name(i, "COLUMN_NAME", sDBIndexFields[idx][i]);
		    	cache_get_value_name(i, "COLUMN_DEFAULT", sDBIndexValues[idx][i]);
		    	cache_get_value_name(i, "DATA_TYPE", sDBIndexFieldType[idx][i]);
			}
		}

		cache_delete(result);
	#else
	    new string[64];
		format(string, sizeof (string), "PRAGMA table_info('%s')", table);
	    new DBResult: result = db_query(DBId, string);

		iDBIndexFieldsPoolSize[index] = db_num_rows(result);
	    if (iDBIndexFieldsPoolSize[index] > 0)
		{
			eDBIndex[index][e_DBINDEX_TABLE_CREATED] = true;

			if (iDBIndexFieldsPoolSize[index] > DB_MAX_FIELDS)
			    printf("[EasyDB] - Warning: There are more fields/columns in \"%s\" table than \"DB_MAX_FIELDS\" (%i).", iDBIndexFieldsPoolSize[index], DB_MAX_FIELDS);

			new i;
		    do
		    {
		    	db_get_field_assoc(result, "name", sDBIndexFields[index][i], DB_MAX_FIELD_NAME);
		    	db_get_field_assoc(result, "dflt_value", sDBIndexValues[index][i], DB_MAX_FIELD_VALUE);
		    	db_get_field_assoc(result, "type", sDBIndexFieldTypes[index][i], DB_MAX_FIELD_NAME);

		    	i++;
			}
			while (db_next_row(result));
		}

		db_free_result(result);
	#endif

	return index;
}

stock DB_IntField(const field[], defaultvalue = 0, DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return 0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return 0;

	for (new i; i < iDBIndexFieldsPoolSize[idx]; i++)
	{
	    if (!strcmp(field, sDBIndexFields[idx][i], true))
	    {
			valstr(sDBIndexValues[idx][i], defaultvalue);
			sDBIndexFieldTypes[idx][i] = "INT";
	        return 1;
	    }
	}

	if (iDBIndexFieldsPoolSize[idx] == DB_MAX_FIELDS)
	    return 0;

	new index = iDBIndexFieldsPoolSize[idx];
	iDBIndexFieldsPoolSize[idx]++;

	format(sDBIndexFields[idx][index], DB_MAX_FIELD_NAME, field);
	valstr(sDBIndexValues[idx][index], defaultvalue);
	sDBIndexFieldTypes[idx][index] = "INT";
	return 1;
}

stock DB_FloatField(const field[], Float: defaultvalue = 0.0, DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return 0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return 0;

	for (new i; i < iDBIndexFieldsPoolSize[idx]; i++)
	{
	    if (!strcmp(field, sDBIndexFields[idx][i], true))
	    {
			format(sDBIndexValues[idx][i], DB_MAX_FIELD_VALUE, "%f", defaultvalue);
			sDBIndexFieldTypes[idx][i] = "FLOAT";
	        return 1;
	    }
	}

	if (iDBIndexFieldsPoolSize[idx] == DB_MAX_FIELDS)
	    return 0;

	new index = iDBIndexFieldsPoolSize[idx];
	iDBIndexFieldsPoolSize[idx]++;

	format(sDBIndexFields[idx][index], DB_MAX_FIELD_NAME, field);
	format(sDBIndexValues[idx][index], DB_MAX_FIELD_VALUE, "%f", defaultvalue);
	sDBIndexFieldTypes[idx][index] = "FLOAT";
	return 1;
}

stock DB_StringField(const field[], defaultvalue[] = "", DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return 0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return 0;

	for (new i; i < iDBIndexFieldsPoolSize[idx]; i++)
	{
	    if (!strcmp(field, sDBIndexFields[idx][i], true))
	    {
			format(sDBIndexValues[idx][i], DB_MAX_FIELD_VALUE, defaultvalue);
			format(sDBIndexFieldTypes[idx][i], DB_MAX_FIELD_NAME, "VARCHAR(%i)", DB_MAX_FIELD_VALUE);
	        return 1;
	    }
	}

	if (iDBIndexFieldsPoolSize[idx] == DB_MAX_FIELDS)
	    return 0;

	new index = iDBIndexFieldsPoolSize[idx];
	iDBIndexFieldsPoolSize[idx]++;

	format(sDBIndexFields[idx][index], DB_MAX_FIELD_NAME, field);
	format(sDBIndexValues[idx][index], DB_MAX_FIELD_VALUE, defaultvalue);
	format(sDBIndexFieldTypes[idx][index], DB_MAX_FIELD_NAME, "VARCHAR(%i)", DB_MAX_FIELD_VALUE);
	return 1;
}

stock DB_PrimaryKey(const field[], DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return 0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return 0;

	for (new i; i < iDBIndexFieldsPoolSize[idx]; i++)
	{
	    if (!strcmp(field, sDBIndexFields[idx][i], true))
	    {
			eDBIndex[idx][e_DBINDEX_PRIMARY_KEY] = i;
	        return 1;
	    }
	}

	return 0;
}

stock DB_ForiegnKey(const field[], const assoctable[], const assocfield[], DBIndex: idx = DBIndex: 0)
{
}

stock DB_CLOSE(DBIndex: idx = DBIndex: 0)
{
	if (!DB_Init())
	    return 0;

	if (!(DBIndex: 0 <= idx < DBIndex: iDBIndexPoolSize))
	    return 0;

	static string[64 + (DB_MAX_FIELDS * (DB_MAX_FIELD_NAME + DB_MAX_FIELD_VALUE + 5))];

	switch (eDBIndex[idx][e_DBINDEX_STATEMENT])
	{
	    case STMT_SELECT:
	    {
	        #if defined SQL_SERVER
				cache_delete(eDBIndex[idx][e_DBINDEX_RESULT]);
			#else
				db_free_result(eDBIndex[idx][e_DBINDEX_RESULT]);
			#endif
	    }

	    case STMT_INSERT:
	    {
			format(string, sizeof (string), "INSERT INTO `%s` (%s", eDBIndex[idx][e_DBINDEX_TABLE], sDBIndexFields[idx][0]);
			for (new i = 1; i < iDBIndexFieldsPoolSize[idx]; i++)
			{
			    strcat(string, ", ");
			    strcat(string, sDBIndexFields[idx][i]);
			}

			format(string, sizeof (string), "%s) VALUES('%q'", string, sDBIndexValues[idx][0]);
            for (new i = 1; i < iDBIndexFieldsPoolSize[idx]; i++)
				format(string, sizeof (string), "%s, '%q'", string, sDBIndexValues[idx][i]);

			strcat(string, ")");

	        #if defined SQL_SERVER
	            mysql_tquery(DBId, string);
			#else
				db_query(DBId, string);
			#endif
		}
		
	    case STMT_UPDATE:
	    {
			format(string, sizeof (string), "UPDATE `%s` SET `%s` = '%q'", eDBIndex[idx][e_DBINDEX_TABLE], sDBIndexFields[idx][0], sDBIndexValues[idx][0]);
			for (new i = 1; i < iDBIndexFieldsPoolSize[idx]; i++)
				format(string, sizeof (string), "%s, `%s` = '%q'", string, sDBIndexFields[idx][i], sDBIndexValues[idx][i]);

			format(string, sizeof (string), "%s WHERE %s", string, eDBIndex[idx][e_DBINDEX_CONDITION]);

			#if defined SQL_SERVER
	            mysql_tquery(DBId, string);
			#else
				db_query(DBId, string);
			#endif
	    }

	    case STMT_VERIFY_TABLE:
	    {
			if (eDBIndex[idx][e_DBINDEX_TABLE_CREATED])
			{
   				for (new i; i < iDBIndexFieldsPoolSize[idx]; i++)
   				{
   				    if (eDBIndex[idx][e_DBINDEX_PRIMARY_KEY] == i)
       					format(string, sizeof (string), "ALTER TABLE `%s` ADD COLUMN `%s` %s PRIMARY KEY NOT NULL DEFAULT '%s'", eDBIndex[idx][e_DBINDEX_TABLE], sDBIndexFields[idx][i], sDBIndexFieldTypes[idx][i], sDBIndexValues[idx][i]);
   				    else
       					format(string, sizeof (string), "ALTER TABLE `%s` ADD COLUMN `%s` %s NOT NULL DEFAULT '%s'", eDBIndex[idx][e_DBINDEX_TABLE], sDBIndexFields[idx][i], sDBIndexFieldTypes[idx][i], sDBIndexValues[idx][i]);
				}

			    #if defined SQL_SERVER
		            mysql_tquery(DBId, string);
				#else
					db_query(DBId, string);
				#endif
			}
			else
			{
		        #if defined SQL_SERVER
				    format(string, sizeof (string), "CREATE TABLE IF NOT EXISTS `%s` (", eDBIndex[idx][e_DBINDEX_TABLE]);
                    for (new i; i < iDBIndexFieldsPoolSize[idx]; i++)
                    {
                        format(string, sizeof (string), "%s%s %s NOT NULL DEFAULT '%s'", string, sDBIndexFields[idx][i], sDBIndexFieldTypes[idx][i], sDBIndexValues[idx][i]);

						if (i == (iDBIndexFieldsPoolSize[idx] - 1))
							format(string, sizeof (string), "%s, PRIMARY KEY (`%s`))", string, sDBIndexFields[idx][eDBIndex[idx][e_DBINDEX_PRIMARY_KEY]]);
						else
                            strcat(string, ", ");
                    }

		            mysql_tquery(DBId, string);
				#else
				    format(string, sizeof (string), "CREATE TABLE IF NOT EXISTS `%s` (", eDBIndex[idx][e_DBINDEX_TABLE]);
                    for (new i; i < iDBIndexFieldsPoolSize[idx]; i++)
                    {
                        if (eDBIndex[idx][e_DBINDEX_PRIMARY_KEY] == i)
                        	format(string, sizeof (string), "%s%s %s PRIMARY KEY NOT NULL DEFAULT '%s'", string, sDBIndexFields[idx][i], sDBIndexFieldTypes[idx][i], sDBIndexValues[idx][i]);
						else
                        	format(string, sizeof (string), "%s%s %s NOT NULL DEFAULT '%s'", string, sDBIndexFields[idx][i], sDBIndexFieldTypes[idx][i], sDBIndexValues[idx][i]);

						if (i == (iDBIndexFieldsPoolSize[idx] - 1))
                            strcat(string, ")");
						else
                            strcat(string, ", ");
                    }

					db_query(DBId, string);
				#endif
			}

	        #if defined SQL_SERVER
	            mysql_tquery(DBId, string);
			#else
				db_query(DBId, string);
			#endif
	    }
	}

	eDBIndex[idx][e_DBINDEX_STATEMENT] = STMT_NONE;

	if (DBIndex: (_:idx + 1) < DBIndex: iDBIndexPoolSize)
	{
	    for (new DBIndex: i = idx; i < DBIndex: (iDBIndexPoolSize - 1); i++)
		{
		    eDBIndex[i][e_DBINDEX_STATEMENT] = eDBIndex[DBIndex: (_:i + 1)][e_DBINDEX_STATEMENT];
		    eDBIndex[i][e_DBINDEX_RESULT] = eDBIndex[DBIndex: (_:i + 1)][e_DBINDEX_RESULT];
			#if !defined SQL_SERVER
		    	eDBIndex[i][e_DBINDEX_LASTROWID] = eDBIndex[DBIndex: (_:i + 1)][e_DBINDEX_LASTROWID];
		    #endif
			eDBIndex[i][e_DBINDEX_ROWID] = eDBIndex[DBIndex: (_:i + 1)][e_DBINDEX_ROWID];
		    format(eDBIndex[i][e_DBINDEX_TABLE], DB_MAX_TABLE_NAME, eDBIndex[DBIndex: (_:i + 1)][e_DBINDEX_TABLE]);
		    format(eDBIndex[i][e_DBINDEX_CONDITION], DB_MAX_CONDITION_SIZE, eDBIndex[DBIndex: (_:i + 1)][e_DBINDEX_CONDITION]);
		    eDBIndex[i][e_DBINDEX_TABLE_CREATED] = eDBIndex[DBIndex: (_:i + 1)][e_DBINDEX_TABLE_CREATED];
		    eDBIndex[i][e_DBINDEX_PRIMARY_KEY] = eDBIndex[DBIndex: (_:i + 1)][e_DBINDEX_PRIMARY_KEY];

			for (new x; x < DB_MAX_FIELDS; x++)
			{
				format(sDBIndexFields[i][x], DB_MAX_FIELD_NAME, sDBIndexFields[DBIndex: (_:i + 1)][x]);
				format(sDBIndexValues[i][x], DB_MAX_FIELD_VALUE, sDBIndexValues[DBIndex: (_:i + 1)][x]);
				format(sDBIndexFieldTypes[i][x], DB_MAX_FIELD_NAME, sDBIndexFieldTypes[DBIndex: (_:i + 1)][x]);
			}

			iDBIndexFieldsPoolSize[i] = iDBIndexFieldsPoolSize[DBIndex: (_:i + 1)];
		}
	}

	iDBIndexPoolSize--;
	return 1;
}

stock DB_DELETE_ROW(const table[], const sortfield[] = "", sorttype = SORT_ASCE, const condition[] = "", {Float, _}:...)
{
	if (!DB_Init())
	    return 0;

	if (!condition[0])
	{
		print("[EasyDB] - Error: To delete a whole table, use DB_DELETE_TABLE instead of DB_DELETE_ROW without any condition passed in it.");
 		return 0;
	}

	new string[64 + DB_MAX_CONDITION_SIZE];
	if (sortfield[0])
		format(string, sizeof (string), "DELETE FROM `%s` WHERE %s ORDER BY `%s` %s", table, DB_ReturnStringArg(condition, 4), sortfield, ((sorttype == SORT_ASCE) ? ("ASC") : ("DESC")));
	else
		format(string, sizeof (string), "DELETE FROM `%s` WHERE %s", table, DB_ReturnStringArg(condition, 4));

	#if defined SQL_SERVER
		mysql_tquery(DBId, string);
	#else
		db_query(DBId, string);
	#endif
	return 1;
}

stock DB_DELETE_TABLE(const table[])
{
	if (!DB_Init())
	    return 0;

	if (!condition[0])
	{
		print("[EasyDB] - Error: To delete a whole table, use DB_DELETE_TABLE instead of DB_DELETE_ROW without any condition passed in it.");
 		return 0;
	}

	new string[64];
	format(string, sizeof (string), "DROP TABLE `%s`", table);

	#if defined SQL_SERVER
		mysql_tquery(DBId, string);
	#else
		db_query(DBId, string);
	#endif
	return 1;
}
