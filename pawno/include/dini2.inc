// Dini2.inc - v2.4.1 - By Gammix - Last updated on 18 Dec,2016
#if defined dini2_included
	#endinput
#endif
#define dini2_included

#if !defined INI_MAX_INSTANCES
	#define INI_MAX_INSTANCES (5)
#endif

#if !defined INI_MAX_FILE_NAME
	#define INI_MAX_FILE_NAME (32)
#endif

#if !defined INI_TIMER_INTERVAL
	#define INI_TIMER_INTERVAL (150)
#endif

#if !defined INI_MAX_FIELDS
	#define INI_MAX_FIELDS (64)
#endif

#if !defined INI_MAX_FIELD_NAME
	#define INI_MAX_FIELD_NAME (32)
#endif

#if !defined INI_MAX_FIELD_VALUE
	#define INI_MAX_FIELD_VALUE (512)
#endif

static INI_sFiles[INI_MAX_INSTANCES][INI_MAX_FILE_NAME];
static INI_iFilesTimer[INI_MAX_INSTANCES];
static bool:INI_bFilesChanged[INI_MAX_INSTANCES];
static INI_iFilesCount;

static INI_sFilesFields[INI_MAX_INSTANCES][INI_MAX_FIELDS][INI_MAX_FIELD_NAME];
static INI_sFilesValues[INI_MAX_INSTANCES][INI_MAX_FIELDS][INI_MAX_FIELD_VALUE];
static INI_iFilesFieldsCount[INI_MAX_INSTANCES];

static INI_OpenFileInstance(const file[])
{
	if (!file[0])
	{
	    print("[dini2] - Error: File name not specified while opening file.");
	    return -1;
	}

	for (new i; i < INI_iFilesCount; i++)
	{
	    if (!strcmp(INI_sFiles[i], file))
	    {
	        return i;
	    }
	}

	if (INI_iFilesCount == INI_MAX_INSTANCES)
	{
	    KillTimer(INI_iFilesTimer[0]);
	    OnFileTimeout(0);
	}

	new File:h = fopen(file, ((fexist(file)) ? (io_read) : (io_readwrite)));
	if (!h)
	{
	    return -1;
	}

	new i,
		x,
		pos,
		len,
		string[INI_MAX_FIELD_NAME + INI_MAX_FIELD_VALUE];

	i = INI_iFilesCount++;
	format(INI_sFiles[i], INI_MAX_FILE_NAME, file);
    INI_iFilesTimer[i] = SetTimerEx("OnFileTimeout", INI_TIMER_INTERVAL, false, "i", i);
    INI_bFilesChanged[i] = false;
    INI_iFilesFieldsCount[i] = 0;
    while (fread(h, string))
    {
        pos = strfind(string, "=");
        if (pos == -1)
        {
            continue;
        }

        if (x == INI_MAX_FIELDS)
        {
            break;
        }

        len = strlen(string);
        if (string[len - 2] == '\r')
        {
            string[len - 2] = EOS;
        }
        else if (string[len - 1] == '\n')
        {
            string[len - 1] = EOS;
		}

		x = INI_iFilesFieldsCount[i]++;
		strmid(INI_sFilesFields[i][x], string, 0, pos, INI_MAX_FIELD_NAME);
		format(INI_sFilesValues[i][x], INI_MAX_FIELD_VALUE, string[pos + 1]);
    }

    fclose(h);
    return i;
}

forward OnFileTimeout(instance);
public OnFileTimeout(instance)
{
	if (INI_bFilesChanged[instance])
	{
		new File:h = fopen(INI_sFiles[instance], io_write);
		new string[INI_MAX_FIELD_NAME + INI_MAX_FIELD_VALUE + 5];
	    for (new i; i < INI_iFilesFieldsCount[instance]; i++)
		{
		    format(string, sizeof (string), "%s=%s\r\n", INI_sFilesFields[instance][i], INI_sFilesValues[instance][i]);
		    fwrite(h, string);
		}
		fclose(h);
	}

	for (new i = instance; i < INI_iFilesCount; i++)
	{
	    format(INI_sFiles[i], INI_MAX_FILE_NAME, INI_sFiles[i + 1]);
	    INI_iFilesTimer[i] = INI_iFilesTimer[i + 1];
	    INI_bFilesChanged[i] = INI_bFilesChanged[i + 1];
	    INI_iFilesFieldsCount[i] = INI_iFilesFieldsCount[i + 1];
	    for (new x; x < INI_iFilesFieldsCount[i + 1]; x++)
	    {
	        format(INI_sFilesFields[i][x], INI_MAX_FIELD_NAME, INI_sFilesFields[i + 1][x]);
			format(INI_sFilesValues[i][x], INI_MAX_FIELD_VALUE, INI_sFilesValues[i + 1][x]);
		}
	}
	INI_iFilesCount--;
}

/*
native dini_Create(const file[]);
*/
stock dini_Create(const file[])
{
	return INI_OpenFileInstance(file);
}

/*
native dini_Remove(const file[]);
*/
#define dini_Remove fremove

/*
native dini_Exists(const file[]);
*/
#define dini_Exists fexist

/*
native dini_Get(const file[], const field[]);
*/
stock dini_Get(const file[], const field[])
{
	static dump[INI_MAX_FIELD_VALUE];

	if (!fexist(file))
	{
	    return dump;
	}

	new instance = INI_OpenFileInstance(file);
	if (instance == -1)
	{
	    return dump;
	}

	for (new i; i < INI_iFilesFieldsCount[instance]; i++)
	{
 		if (!strcmp(field, INI_sFilesFields[instance][i]))
 		{
 		    return INI_sFilesValues[instance][i];
 		}
	}

	return dump;
}

/*
native dini_Int(const file[], const field[]);
*/
stock dini_Int(const file[], const field[])
{
	return strval(dini_Get(file, field));
}

/*
native Float:dini_Float(const file[], const field[]);
*/
stock Float:dini_Float(const file[], const field[])
{
	return floatstr(dini_Get(file, field));
}

/*
native bool:dini_Bool(const file[], const field[]);
*/
stock bool:dini_Bool(const file[], const field[])
{
	return bool:strval(dini_Get(file, field));
}

/*
native dini_Set(const file[], const field[], value[]);
*/
stock dini_Set(const file[], const field[], value[])
{
	new instance = INI_OpenFileInstance(file);
	if (instance == -1)
	{
	    return 0;
	}

	for (new i; i < INI_iFilesFieldsCount[instance]; i++)
	{
 		if (!strcmp(field, INI_sFilesFields[instance][i]))
 		{
 		    format(INI_sFilesValues[instance][i], INI_MAX_FIELD_VALUE, value);
 		    return 1;
 		}
	}

	if (INI_iFilesFieldsCount[instance] == INI_MAX_FIELDS)
	{
	    printf("[dini2] - Error: Field \"%s\" couldn't be created in file \"%s\", increase \"INI_MAX_FIELDS\" value to %i.", field, file, (INI_MAX_FIELDS + 1));
		return 0;
	}

	new i = INI_iFilesFieldsCount[instance]++;
	format(INI_sFilesFields[instance][i], INI_MAX_FIELD_NAME, field);
	format(INI_sFilesValues[instance][i], INI_MAX_FIELD_VALUE, value);

    INI_bFilesChanged[instance] = true;
	return 0;
}

/*
native dini_IntSet(const file[], const field[], value);
*/
stock dini_IntSet(const file[], const field[], value)
{
	new instance = INI_OpenFileInstance(file);
	if (instance == -1)
	{
	    return 0;
	}

	for (new i; i < INI_iFilesFieldsCount[instance]; i++)
	{
 		if (!strcmp(field, INI_sFilesFields[instance][i]))
 		{
 		    valstr(INI_sFilesValues[instance][i], value);
 		    return 1;
 		}
	}

	if (INI_iFilesFieldsCount[instance] == INI_MAX_FIELDS)
	{
	    printf("[dini2] - Error: Field \"%s\" couldn't be created in file \"%s\", increase \"INI_MAX_FIELDS\" value to %i.", field, file, (INI_MAX_FIELDS + 1));
		return 0;
	}

	new i = INI_iFilesFieldsCount[instance]++;
	format(INI_sFilesFields[instance][i], INI_MAX_FIELD_NAME, field);
	valstr(INI_sFilesValues[instance][i], value);

    INI_bFilesChanged[instance] = true;
	return 1;
}

/*
native dini_FloatSet(const file[], const field[], Float:value);
*/
stock dini_FloatSet(const file[], const field[], Float:value)
{
	new instance = INI_OpenFileInstance(file);
	if (instance == -1)
	{
	    return 0;
	}

	for (new i; i < INI_iFilesFieldsCount[instance]; i++)
	{
 		if (!strcmp(field, INI_sFilesFields[instance][i]))
 		{
 		    format(INI_sFilesValues[instance][i], INI_MAX_FIELD_VALUE, "%f", value);
 		    return 1;
 		}
	}

	if (INI_iFilesFieldsCount[instance] == INI_MAX_FIELDS)
	{
	    printf("[dini2] - Error: Field \"%s\" couldn't be created in file \"%s\", increase \"INI_MAX_FIELDS\" value to %i.", field, file, (INI_MAX_FIELDS + 1));
		return 0;
	}

	new i = INI_iFilesFieldsCount[instance]++;
	format(INI_sFilesFields[instance][i], INI_MAX_FIELD_NAME, field);
	format(INI_sFilesValues[instance][i], INI_MAX_FIELD_VALUE, "%f", value);

    INI_bFilesChanged[instance] = true;
	return 1;
}

/*
native dini_BoolSet(const file[], const field[], bool:value);
*/
stock dini_BoolSet(const file[], const field[], bool:value)
{
	new instance = INI_OpenFileInstance(file);
	if (instance == -1)
	{
	    return 0;
	}

	for (new i; i < INI_iFilesFieldsCount[instance]; i++)
	{
 		if (!strcmp(field, INI_sFilesFields[instance][i]))
 		{
 		    valstr(INI_sFilesValues[instance][i], _:value);
 		    return 1;
 		}
	}

	if (INI_iFilesFieldsCount[instance] == INI_MAX_FIELDS)
	{
	    printf("[dini2] - Error: Field \"%s\" couldn't be created in file \"%s\", increase \"INI_MAX_FIELDS\" value to %i.", field, file, (INI_MAX_FIELDS + 1));
		return 0;
	}

	new i = INI_iFilesFieldsCount[instance]++;
	format(INI_sFilesFields[instance][i], INI_MAX_FIELD_NAME, field);
	valstr(INI_sFilesValues[instance][i], _:value);

    INI_bFilesChanged[instance] = true;
	return 1;
}

/*
native dini_UnSet(const file[], const field[]);
*/
stock dini_UnSet(const file[], const field[])
{
	new instance = INI_OpenFileInstance(file);
	if (instance == -1)
	{
	    return 0;
	}

	for (new i; i < INI_iFilesFieldsCount[instance]; i++)
	{
 		if (!strcmp(field, INI_sFilesFields[instance][i]))
 		{
 		    for (; i < INI_iFilesFieldsCount[instance]; i++)
 		    {
 		        format(INI_sFilesFields[instance][i], INI_MAX_FIELD_NAME, INI_sFilesFields[instance][i + 1]);
				format(INI_sFilesValues[instance][i], INI_MAX_FIELD_VALUE, INI_sFilesValues[instance][i + 1]);
 		    }
 		    INI_iFilesFieldsCount[instance]--;

    		INI_bFilesChanged[instance] = true;
 		    return 1;
 		}
	}

	return 0;
}

/*
native dini_IsSet(const file[], const field[]);
*/
stock dini_IsSet(const file[], const field[])
{
	new instance = INI_OpenFileInstance(file);
	if (instance == -1)
	{
	    return 0;
	}

	for (new i; i < INI_iFilesFieldsCount[instance]; i++)
	{
 		if (!strcmp(field, INI_sFilesFields[instance][i]))
 		{
 		    return 1;
 		}
	}

	return 0;
}

/*
native DINI_fcopytextfile(const file[], const newfile[]);
*/
stock DINI_fcopytextfile(const file[], const newfile[])
{
	new File:handles[2];

	handles[0] = fopen(file, io_read);
	if (!handles[0])
	{
		return 0;
	}

	handles[1] = fopen(newfile, io_write);
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

/*
native dini_Timeout(const file[]);
*/
stock dini_Timeout(const file[])
{
    for (new i; i < INI_iFilesCount; i++)
	{
	    if (!strcmp(INI_sFiles[i], file))
	    {
		    KillTimer(INI_iFilesTimer[i]);
		    OnFileTimeout(i);
		    return 1;
	    }
	}
	return 0;
}
