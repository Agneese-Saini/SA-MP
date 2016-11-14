#if defined dini2_included
	#endinput
#endif
#define dini2_included

/*
 * dini2.inc (IMPROVED)
 * Version: 2.3.3 (last updated on 14th November, 2016)
 * By Gammix
 *
 * FUNCTIONS:
   native dini_Exists(const file[]);
   native dini_Remove(const file[]);
   native dini_Create(const file[]);
   native dini_Set(const file[], const field[], const value[]);
   native dini_IntSet(const file[], const field[], value);
   native dini_FloatSet(const file[], const field[], Float:value);
   native dini_BoolSet(const file[], const field[], bool:value);
   native dini_Get(const file[], const field[]);
   native dini_Int(const file[], const field[]);
   native Float:dini_Float(const file[], const field[]);
   native bool:dini_Bool(const file[], const field[]);
   native dini_UnSet(const file[], const field[]);
   native dini_IsSet(const file[], const field[]);
   native DINI_fcopytextfile(const file[], const newname[]);
   native _
   native dini_Timeout(const file[]);
 *
 */

#if !defined INI_FILE_TIMEOUT
	#define INI_FILE_TIMEOUT (5)
#endif

#if !defined INI_MAX_INSTANCES
	#define INI_MAX_INSTANCES (18)
#endif

#if !defined INI_MAX_FILE_NAME
	#define INI_MAX_FILE_NAME (64)
#endif

#if !defined INI_MAX_FIELDS
	#define INI_MAX_FIELDS (64)
#endif

#if !defined INI_MAX_FIELD_NAME
	#define INI_MAX_FIELD_NAME (32)
#endif

#if !defined INI_MAX_FIELD_VALUE
	#define INI_MAX_FIELD_VALUE (256)
#endif

static g_INITimer;

enum E_FILE
{
			E_FILE_STATE,
			E_FILE_NAME[INI_MAX_FILE_NAME],
			E_FILE_TICK,
	        E_FILE_FIELDS_COUNT
};
static g_INIFile[INI_MAX_INSTANCES][E_FILE];

static g_INIFileFields[INI_MAX_INSTANCES][INI_MAX_FIELDS][INI_MAX_FIELD_NAME];
static g_INIFileValues[INI_MAX_INSTANCES][INI_MAX_FIELDS][INI_MAX_FIELD_VALUE];

#if ! defined FILTERSCRIPT
	public OnGameModeInit()
#else
	public OnFilterScriptInit()
#endif
{
	g_INITimer = SetTimer("INI_OnFileTimeout", (INI_FILE_TIMEOUT / INI_MAX_INSTANCES), true);

	#if ! defined FILTERSCRIPT
		#if defined DINI2_OnGameModeInit
			return DINI2_OnGameModeInit();
		#else
			return 1;
		#endif
	#else
		#if defined DINI2_OnFilterScriptInit
			return DINI2_OnFilterScriptInit();
		#else
			return 1;
		#endif
	#endif
}
#if ! defined FILTERSCRIPT
	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit DINI2_OnGameModeInit
	#if defined DINI2_OnGameModeInit
		forward DINI2_OnGameModeInit();
	#endif
#else
	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif
	#define OnFilterScriptInit DINI2_OnFilterScriptInit
	#if defined DINI2_OnFilterScriptInit
		forward DINI2_OnFilterScriptInit();
	#endif
#endif

#if ! defined FILTERSCRIPT
	public OnGameModeExit()
#else
	public OnFilterScriptExit()
#endif
{
	KillTimer(g_INITimer);

	#if ! defined FILTERSCRIPT
		#if defined DINI2_OnGameModeExit
			return DINI2_OnGameModeExit();
		#else
			return 1;
		#endif
	#else
		#if defined DINI2_OnFilterScriptExit
			return DINI2_OnFilterScriptExit();
		#else
			return 1;
		#endif
	#endif
}
#if ! defined FILTERSCRIPT
	#if defined _ALS_OnGameModeExit
		#undef OnGameModeExit
	#else
		#define _ALS_OnGameModeExit
	#endif
	#define OnGameModeExit DINI2_OnGameModeExit
	#if defined DINI2_OnGameModeExit
		forward DINI2_OnGameModeExit();
	#endif
#else
	#if defined _ALS_OnFilterScriptExit
		#undef OnFilterScriptExit
	#else
		#define _ALS_OnFilterScriptExit
	#endif
	#define OnFilterScriptExit DINI2_OnFilterScriptExit
	#if defined DINI2_OnFilterScriptExit
		forward DINI2_OnFilterScriptExit();
	#endif
#endif

forward INI_OnFileTimeout();
public  INI_OnFileTimeout()
{
	static data[INI_MAX_FIELDS * (INI_MAX_FIELD_NAME + INI_MAX_FIELD_VALUE + 1)];

	for (new i; i < INI_MAX_INSTANCES; i++)
	{
		switch (g_INIFile[i][E_FILE_STATE])
		{
		    case 1:
		    {
				if ((GetTickCount() - g_INIFile[i][E_FILE_TICK]) >= INI_FILE_TIMEOUT)
					g_INIFile[i][E_FILE_STATE] = 0;
			}

		    case 2:
		    {
				if ((GetTickCount() - g_INIFile[i][E_FILE_TICK]) >= INI_FILE_TIMEOUT)
				{
					new File:handle = fopen(g_INIFile[i][E_FILE_NAME], io_write);
					if (handle)
					{
	    				data[0] = EOS;
						for (new x, y = g_INIFile[i][E_FILE_FIELDS_COUNT]; x < y; x++)
							format(data, sizeof (data), "%s%s=%s\r\n", data, g_INIFileFields[i][x], g_INIFileValues[i][x]);

						fwrite(handle, data);
						fclose(handle);
					}

					g_INIFile[i][E_FILE_STATE] = 0;
				}
			}
		}
	}
}

stock static INI_StripLine(string[])
{
	new l = strlen(string);

	if (string[l - 2] == '\r')
		string[l - 2] = '\0';

	if (string[l - 1] == '\n')
		string[l - 1] = '\0';
}

stock static INI_OpenFile(const file[])
{
 	new id = -1;
	for (new i; i < INI_MAX_INSTANCES; i++)
	{
		if (g_INIFile[i][E_FILE_STATE] == 0)
		{
		    if (id == -1)
		    	id = i;
		}
  		else if (!strcmp(g_INIFile[i][E_FILE_NAME], file))
	    	return i;
	}

    new File:handle;
    static data[INI_MAX_FIELDS * (INI_MAX_FIELD_NAME + INI_MAX_FIELD_VALUE + 1)];
	if (id == -1)
	{
	    id = 0;
	    
	    handle = fopen(g_INIFile[id][E_FILE_NAME], io_write);
		if (handle)
		{
			data[0] = EOS;

			for (new x, y = g_INIFile[id][E_FILE_FIELDS_COUNT]; x < y; x++)
				format(data, sizeof (data), "%s%s=%s\r\n", data, g_INIFileFields[id][x], g_INIFileValues[id][x]);

			fwrite(handle, data);
			fclose(handle);
   	 	}

		g_INIFile[id][E_FILE_STATE] = 0;
	}

	handle = fopen(file, io_readwrite);
	if (!handle)
	    return -1;

    g_INIFile[id][E_FILE_FIELDS_COUNT] = 0;

	new pos;
	new count;
	new fieldname[INI_MAX_FIELD_NAME];
	while (count < INI_MAX_FIELDS && fread(handle, data))
	{
		INI_StripLine(data);

		pos = strfind(data, "=", false);
		if (pos == -1)
			continue;

		strmid(fieldname, data, 0, pos);
		format(g_INIFileFields[id][count], INI_MAX_FIELD_NAME, fieldname);
		format(g_INIFileValues[id][count], INI_MAX_FIELD_VALUE, data[pos + 1]);

		count++;
	}
	g_INIFile[id][E_FILE_FIELDS_COUNT] = count;

    format(g_INIFile[id][E_FILE_NAME], INI_MAX_FILE_NAME, file);
    g_INIFile[id][E_FILE_TICK] = GetTickCount();
    g_INIFile[id][E_FILE_STATE] = 1;

	fclose(handle);
	return id;
}

stock dini_Exists(const file[])
	return fexist(file);

stock dini_Remove(const file[])
	return fremove(file);

stock dini_Create(const file[])
	return ((INI_OpenFile(file) < 0) ? (0) : (1));

stock dini_Set(const file[], const field[], const value[])
{
	new id = INI_OpenFile(file);
	if (id == -1)
	    return 0;

	if (!field[0])
	    return 0;

	new count = g_INIFile[id][E_FILE_FIELDS_COUNT];
	for (new i; i < count; i++)
	{
	    if (!strcmp(g_INIFileFields[id][i], field))
    	{
			format(g_INIFileValues[id][i], INI_MAX_FIELD_VALUE, value);

			g_INIFile[id][E_FILE_STATE] = 2;
	    	return 1;
		}
	}

	if (count < INI_MAX_FIELDS)
	{
		format(g_INIFileFields[id][count], INI_MAX_FIELD_NAME, field);
		format(g_INIFileValues[id][count], INI_MAX_FIELD_VALUE, value);

		g_INIFile[id][E_FILE_FIELDS_COUNT]++;

		g_INIFile[id][E_FILE_STATE] = 2;
  		return 1;
	}

	return 0;
}

stock dini_IntSet(const file[], const field[], value)
{
	new id = INI_OpenFile(file);
	if (id == -1)
	    return 0;

	if (!field[0])
	    return 0;

	new count = g_INIFile[id][E_FILE_FIELDS_COUNT];
	for (new i; i < count; i++)
	{
	    if (!strcmp(g_INIFileFields[id][i], field))
    	{
			valstr(g_INIFileValues[id][i], value);

			g_INIFile[id][E_FILE_STATE] = 2;
	    	return 1;
		}
	}

	if (count < INI_MAX_FIELDS)
	{
		format(g_INIFileFields[id][count], INI_MAX_FIELD_NAME, field);
		valstr(g_INIFileValues[id][count], value);

		g_INIFile[id][E_FILE_FIELDS_COUNT]++;

		g_INIFile[id][E_FILE_STATE] = 2;
  		return 1;
	}

	return 0;
}

stock dini_FloatSet(const file[], const field[], Float:value)
{
	new id = INI_OpenFile(file);
	if (id == -1)
	    return 0;

	if (!field[0])
	    return 0;

	new count = g_INIFile[id][E_FILE_FIELDS_COUNT];
	for (new i; i < count; i++)
	{
	    if (!strcmp(g_INIFileFields[id][i], field))
    	{
			format(g_INIFileValues[id][i], INI_MAX_FIELD_VALUE, "%f", value);

			g_INIFile[id][E_FILE_STATE] = 2;
	    	return 1;
		}
	}

	if (count < INI_MAX_FIELDS)
	{
		format(g_INIFileFields[id][count], INI_MAX_FIELD_NAME, field);
		format(g_INIFileValues[id][count], INI_MAX_FIELD_VALUE, "%f", value);

		g_INIFile[id][E_FILE_FIELDS_COUNT]++;

		g_INIFile[id][E_FILE_STATE] = 2;
  		return 1;
	}

	return 0;
}

stock dini_BoolSet(const file[], const field[], bool:value)
{
	new id = INI_OpenFile(file);
	if (id == -1)
	    return 0;

	if (!field[0])
	    return 0;

	new count = g_INIFile[id][E_FILE_FIELDS_COUNT];
	for (new i; i < count; i++)
	{
	    if (!strcmp(g_INIFileFields[id][i], field))
    	{
			valstr(g_INIFileValues[id][i], _:value);

			g_INIFile[id][E_FILE_STATE] = 2;
	    	return 1;
		}
	}

	if (count < INI_MAX_FIELDS)
	{
		format(g_INIFileFields[id][count], INI_MAX_FIELD_NAME, field);
		valstr(g_INIFileValues[id][count], _:value);

		g_INIFile[id][E_FILE_FIELDS_COUNT]++;

		g_INIFile[id][E_FILE_STATE] = 2;
  		return 1;
	}

	return 0;
}

stock dini_Get(const file[], const field[])
{
	new ret[INI_MAX_FIELD_VALUE];

	new id = INI_OpenFile(file);
	if (id == -1)
	    return ret;

	if (!field[0])
	    return ret;

	for (new i, j = g_INIFile[id][E_FILE_FIELDS_COUNT]; i < j; i++)
	{
	    if (!strcmp(g_INIFileFields[id][i], field))
			return g_INIFileValues[id][i];
	}

	return ret;
}

stock dini_Int(const file[], const field[])
{
	new id = INI_OpenFile(file);
	if (id == -1)
	    return -1;

	if (!field[0])
	    return -1;

	for (new i, j = g_INIFile[id][E_FILE_FIELDS_COUNT]; i < j; i++)
	{
	    if (!strcmp(g_INIFileFields[id][i], field))
			return strval(g_INIFileValues[id][i]);
	}

	return -1;
}

stock Float:dini_Float(const file[], const field[])
{
	new id = INI_OpenFile(file);
	if (id == -1)
	    return -1.0;

	if (!field[0])
	    return -1.0;

	for (new i, j = g_INIFile[id][E_FILE_FIELDS_COUNT]; i < j; i++)
	{
	    if (!strcmp(g_INIFileFields[id][i], field))
			return floatstr(g_INIFileValues[id][i]);
	}

	return -1.0;
}

stock bool:dini_Bool(const file[], const field[])
{
	new id = INI_OpenFile(file);
	if (id == -1)
	    return false;

	if (!field[0])
	    return false;

	for (new i, j = g_INIFile[id][E_FILE_FIELDS_COUNT]; i < j; i++)
	{
	    if (!strcmp(g_INIFileFields[id][i], field))
			return bool:strval(g_INIFileValues[id][i]);
	}

	return false;
}

stock dini_UnSet(const file[], const field[])
{
	new id = INI_OpenFile(file);
	if (id == -1)
	    return 0;

	if (!field[0])
	    return 0;

	new bool:shift;
	for (new i, j = g_INIFile[id][E_FILE_FIELDS_COUNT]; i < j; i++)
	{
	    if (!strcmp(g_INIFileFields[id][i], field))
            shift = true;

		if (shift)
		{
		    if (i == (j - 1))
		    {
	            g_INIFileFields[id][i][0] = EOS;
	            g_INIFileValues[id][i][0] = EOS;

	            g_INIFile[id][E_FILE_FIELDS_COUNT]--;

	            g_INIFile[id][E_FILE_STATE] = 2;
		    }
		    else
		    {
	            g_INIFileFields[id][i] = g_INIFileFields[id][i + 1];
	            g_INIFileValues[id][i] = g_INIFileValues[id][i + 1];
			}
		}
	}

	return 0;
}

stock dini_IsSet(const file[], const field[])
{
	new id = INI_OpenFile(file);
	if (id == -1)
	    return 0;

	if (!field[0])
	    return 0;

	for (new i, j = g_INIFile[id][E_FILE_FIELDS_COUNT]; i < j; i++)
	{
	    if (!strcmp(g_INIFileFields[id][i], field))
			return 1;
	}

	return 0;
}

stock DINI_fcopytextfile(const file[], const newname[])
{
	new File:handles[2];

	handles[0] = fopen(file, io_read);
	if (!handles[0])
		return 0;

	handles[1] = fopen(newname, io_write);
	if (!handles[1])
	{
		fclose(handles[0]);
		return 0;
	}

	new data[INI_MAX_FIELD_NAME + INI_MAX_FIELD_VALUE];
	while (fread(handles[0], data))
	{
		INI_StripLine(data);
		strcat(data, "\r\n");
		fwrite(handles[1], data);
	}

	fclose(handles[0]);
	fclose(handles[1]);

	return 1;
}

stock dini_Timeout(const file[])
{
	for (new i; i < INI_MAX_INSTANCES; i++)
	{
		switch (g_INIFile[i][E_FILE_STATE])
		{
		    case 1:
		    {
				if (strcmp(g_INIFile[i][E_FILE_NAME], file))
		    	    continue;

		    	g_INIFile[i][E_FILE_STATE] = 0;
		    	return 1;
			}

		    case 2:
		    {
				if (strcmp(g_INIFile[i][E_FILE_NAME], file))
		    	    continue;

				new File:handle = fopen(g_INIFile[i][E_FILE_NAME], io_write);
				if (handle)
				{
				    static data[INI_MAX_FIELDS * (INI_MAX_FIELD_NAME + INI_MAX_FIELD_VALUE + 1)];
					data[0] = EOS;

					for (new x, y = g_INIFile[i][E_FILE_FIELDS_COUNT]; x < y; x++)
						format(data, sizeof (data), "%s%s=%s\r\n", data, g_INIFileFields[i][x], g_INIFileValues[i][x]);

					fwrite(handle, data);
					fclose(handle);
   			 	}

				g_INIFile[i][E_FILE_STATE] = 0;
		    	return 1;
			}
		}
	}

	return 0;
}
