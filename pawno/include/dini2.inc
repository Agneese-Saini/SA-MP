// Dini2.inc - v2.5.3 - By Gammix - Last updated "18 May, 2017"
#if defined dini2_included
	#endinput
#endif
#define dini2_included

#define DINI_MAX_INSTANCES  	32
#define DINI_MAX_FILE_NAME  	64
#define DINI_MAX_FIELDS 		64
#define DINI_MAX_FIELD_NAME 	32
#define DINI_MAX_FIELD_VALUE    512

enum E_DINI
{
	E_DINI_FILE_NAME[DINI_MAX_FILE_NAME],
	E_DINI_FILE_TIMER,
	bool:E_DINI_FILE_CHANGED,
	E_DINI_NUM_FIELDS
};

static diniFiles[DINI_MAX_INSTANCES][E_DINI];

static diniFields[DINI_MAX_INSTANCES][DINI_MAX_FIELDS][DINI_MAX_FIELD_NAME];
static diniValues[DINI_MAX_INSTANCES][DINI_MAX_FIELDS][DINI_MAX_FIELD_VALUE];

static diniFieldsList[DINI_MAX_INSTANCES][DINI_MAX_FIELDS];

static stock dini_sdbm(const string[])
{
    new hash;
    new i;
    new c;

    while ((c = string[i++]) != EOS)
    {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    
    return hash;
}

static stock bool:dini_Open(index, const file[])
{
	if (fexist(file))
	{
	    new File:handle = fopen(file, io_read);
	    if (!handle)
	    {
	        return false;
	    }
    
	    strcat(diniFiles[index][E_DINI_FILE_NAME], file, DINI_MAX_FILE_NAME);
	    diniFiles[index][E_DINI_FILE_TIMER] = SetTimerEx("dini_OnFileTimeout", 150, false, "i", index);
	    diniFiles[index][E_DINI_FILE_CHANGED] = false;
	    diniFiles[index][E_DINI_NUM_FIELDS] = 0;

	    new string[DINI_MAX_FIELD_NAME + DINI_MAX_FIELD_VALUE + 1];
		new pos;
		new len;
		new tempField[DINI_MAX_FIELD_NAME];
		new fieldIndex;

		while (fread(handle, string))
	    {
			pos = strfind(string, "=");

		 	len = strlen(string);
	        if (string[len - 2] == '\r')
	        {
	            string[len - 2] = EOS;
	        }
	        else if (string[len - 1] == '\n')
	        {
	            string[len - 1] = EOS;
			}

	        strmid(tempField, string, 0, pos);

	        fieldIndex = dini_sdbm(tempField);
	        fieldIndex %= DINI_MAX_FIELDS;

	        diniFields[index][fieldIndex] = tempField;
			strcat(diniValues[index][fieldIndex], string[pos + 1], DINI_MAX_FIELD_VALUE);

			diniFieldsList[index][diniFiles[index][E_DINI_NUM_FIELDS]++] = fieldIndex;
		}
		
		fclose(handle);
	}
	else
	{
	    new File:handle = fopen(file, io_readwrite);
	    if (!handle)
	    {
	        return false;
	    }

	    strcat(diniFiles[index][E_DINI_FILE_NAME], file, DINI_MAX_FILE_NAME);
	    diniFiles[index][E_DINI_FILE_TIMER] = SetTimerEx("dini_OnFileTimeout", 150, false, "i", index);
	    diniFiles[index][E_DINI_FILE_CHANGED] = false;
	    diniFiles[index][E_DINI_NUM_FIELDS] = 0;

		fclose(handle);
	}
	return true;
}

forward dini_OnFileTimeout(index);
public dini_OnFileTimeout(index)
{
    if (diniFiles[index][E_DINI_FILE_CHANGED] && fexist(diniFiles[index][E_DINI_FILE_NAME]))
    {
        new File:handle = fopen(diniFiles[index][E_DINI_FILE_NAME], io_write);
	    if (!handle)
	    {
    		diniFiles[index][E_DINI_FILE_NAME][0] = EOS;
	        return;
	    }

	    new string[DINI_MAX_FIELD_NAME + DINI_MAX_FIELD_VALUE + 1];
	    for (new i; i < diniFiles[index][E_DINI_NUM_FIELDS]; i++)
	    {
	   	 	format(string, sizeof string, "%s=%s\r\n", diniFields[index][diniFieldsList[index][i]], diniValues[index][diniFieldsList[index][i]]);
	   	 	diniFields[index][diniFieldsList[index][i]][0] = EOS;
	   	 	diniValues[index][diniFieldsList[index][i]][0] = EOS;
	   	 	
	   	 	fwrite(handle, string);
	    }
	    
	    fclose(handle);
    }
    
    diniFiles[index][E_DINI_FILE_NAME][0] = EOS;
}

/*
native dini_Remove(const file[]);
*/
#define dini_Remove \
	fremove

/*
native dini_Exists(const file[]);
*/
#define dini_Exists \
	fexist

/*
native dini_Create(const file[]);
*/
stock dini_Create(const file[])
{
	new fileIndex = dini_sdbm(file);
	fileIndex %= DINI_MAX_INSTANCES;
	if (!diniFiles[fileIndex][E_DINI_FILE_NAME][0])
	{
	    if (!dini_Open(fileIndex, file))
	    {
	        return 0;
	    }
	}
	else if (strcmp(diniFiles[fileIndex][E_DINI_FILE_NAME], file))
	{
	    KillTimer(diniFiles[fileIndex][E_DINI_FILE_TIMER]);
		dini_OnFileTimeout(fileIndex);
	    return dini_Create(file);
	}
	
	return 1;
}

/*
native dini_Get(const file[], const field[]);
*/
stock dini_Get(const file[], const field[])
{
	static empty[DINI_MAX_FIELD_VALUE];
	
	new fileIndex = dini_sdbm(file);
	fileIndex %= DINI_MAX_INSTANCES;
	if (!diniFiles[fileIndex][E_DINI_FILE_NAME][0])
	{
	    if (diniFiles[fileIndex][E_DINI_NUM_FIELDS] == 0)
	    {
			return empty;
	    }
	
	    if (!dini_Open(fileIndex, file))
	    {
	        return empty;
	    }
	}
	else if (strcmp(diniFiles[fileIndex][E_DINI_FILE_NAME], file))
	{
	    KillTimer(diniFiles[fileIndex][E_DINI_FILE_TIMER]);
		dini_OnFileTimeout(fileIndex);
	    return dini_Get(file, field);
	}

	new fieldIndex = dini_sdbm(field);
	fieldIndex %= DINI_MAX_FIELDS;
	if (!diniFields[fileIndex][fieldIndex][0])
	{
	    return empty;
	}
	else if (strcmp(diniFields[fileIndex][fieldIndex], field))
	{
	    return empty;
	}
	
	return diniValues[fileIndex][fieldIndex];
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
	new fileIndex = dini_sdbm(file);
	fileIndex %= DINI_MAX_INSTANCES;
	if (!diniFiles[fileIndex][E_DINI_FILE_NAME][0])
	{
	    if (!dini_Open(fileIndex, file))
	    {
	        return 0;
	    }
	}
	else if (strcmp(diniFiles[fileIndex][E_DINI_FILE_NAME], file))
	{
	    KillTimer(diniFiles[fileIndex][E_DINI_FILE_TIMER]);
		dini_OnFileTimeout(fileIndex);
	    return dini_Set(file, field, value);
	}

	new fieldIndex = dini_sdbm(field);
	fieldIndex %= DINI_MAX_FIELDS;
	if (!diniFields[fileIndex][fieldIndex][0])
	{
	    strcat(diniFields[fileIndex][fieldIndex], field, DINI_MAX_FIELD_NAME);
		strcat(diniValues[fileIndex][fieldIndex], value, DINI_MAX_FIELD_VALUE);

		diniFieldsList[fileIndex][diniFiles[fileIndex][E_DINI_NUM_FIELDS]++] = fieldIndex;

		diniFiles[fileIndex][E_DINI_FILE_CHANGED] = true;
	    return 2;
	}
	else if (strcmp(diniFields[fileIndex][fieldIndex], field))
	{
	    return 0;
	}

	diniValues[fileIndex][fieldIndex][0] = EOS;
	strcat(diniValues[fileIndex][fieldIndex], value, DINI_MAX_FIELD_VALUE);

	diniFiles[fileIndex][E_DINI_FILE_CHANGED] = true;
	return 1;
}

/*
native dini_IntSet(const file[], const field[], value);
*/
stock dini_IntSet(const file[], const field[], value)
{
	new string[16];
	valstr(string, value);
	return dini_Set(file, field, string);
}

/*
native dini_FloatSet(const file[], const field[], Float:value);
*/
stock dini_FloatSet(const file[], const field[], Float:value)
{
	new string[16];
	format(string, sizeof string, "%f", value);
	return dini_Set(file, field, string);
}

/*
native dini_BoolSet(const file[], const field[], bool:value);
*/
stock dini_BoolSet(const file[], const field[], bool:value)
{
	new string[16];
	valstr(string, _:value);
	return dini_Set(file, field, string);
}

/*
native dini_UnSet(const file[], const field[]);
*/
stock dini_UnSet(const file[], const field[])
{
	new fileIndex = dini_sdbm(file);
	fileIndex %= DINI_MAX_INSTANCES;
	if (!diniFiles[fileIndex][E_DINI_FILE_NAME][0])
	{
	    if (!dini_Open(fileIndex, file))
	    {
	        return 0;
	    }
	}
	else if (strcmp(diniFiles[fileIndex][E_DINI_FILE_NAME], file))
	{
	    KillTimer(diniFiles[fileIndex][E_DINI_FILE_TIMER]);
		dini_OnFileTimeout(fileIndex);
	    return dini_UnSet(file, field);
	}

	new fieldIndex = dini_sdbm(field);
	fieldIndex %= DINI_MAX_FIELDS;
	if (!diniFields[fileIndex][fieldIndex][0])
	{
	    return 1;
	}
	else if (strcmp(diniFields[fileIndex][fieldIndex], field))
	{
	    return 0;
	}

	diniFields[fileIndex][fieldIndex][0] = EOS;
	diniValues[fileIndex][fieldIndex][0] = EOS;
	
	for (new i; i < diniFiles[fileIndex][E_DINI_NUM_FIELDS]; i++)
	{
	    if (diniFieldsList[fileIndex][i] == fieldIndex)
	    {
	        for (new x = i; x < (diniFiles[fileIndex][E_DINI_NUM_FIELDS] - 1); x++)
	        {
	            diniFieldsList[fileIndex][x] = diniFieldsList[fileIndex][x + 1];
	        }
	        break;
	    }
	}
	diniFiles[fileIndex][E_DINI_NUM_FIELDS]--;

    diniFiles[fileIndex][E_DINI_FILE_CHANGED] = true;
	return 1;
}

/*
native dini_IsSet(const file[], const field[]);
*/
stock dini_IsSet(const file[], const field[])
{
	new fileIndex = dini_sdbm(file);
	fileIndex %= DINI_MAX_INSTANCES;
	if (!diniFiles[fileIndex][E_DINI_FILE_NAME][0])
	{
	    if (!dini_Open(fileIndex, file))
	    {
	        return 0;
	    }
	}
	else if (strcmp(diniFiles[fileIndex][E_DINI_FILE_NAME], file))
	{
	    KillTimer(diniFiles[fileIndex][E_DINI_FILE_TIMER]);
		dini_OnFileTimeout(fileIndex);
	    return dini_IsSet(file, field);
	}

	new fieldIndex = dini_sdbm(field);
	fieldIndex %= DINI_MAX_FIELDS;
	if (!diniFields[fileIndex][fieldIndex][0])
	{
	    return 0;
	}
	else if (strcmp(diniFields[fileIndex][fieldIndex], field))
	{
	    return 0;
	}
	
	return 1;
}

/*
native DINI_fcopytextfile(const file[], const newfile[]);
*/
stock DINI_fcopytextfile(const file[], const newfile[])
{
	new File:h1 = fopen(file, io_read);
	if (!h1)
	{
	    return 0;
	}
	
	new File:h2 = fopen(newfile, io_write);
	if (!h2)
	{
	    fclose(h1);
	    return 0;
	}
	
	new string[DINI_MAX_FIELD_NAME + DINI_MAX_FIELD_VALUE + 1];
	while (fread(h1, string))
	{
		fwrite(h2, string);
	}
	
	fclose(h1);
	fclose(h2);
	
	return 1;
}

/*
native dini_Timeout(const file[]);
*/
stock dini_Timeout(const file[])
{
	new fileIndex = dini_sdbm(file);
	fileIndex %= DINI_MAX_INSTANCES;
	if (!diniFiles[fileIndex][E_DINI_FILE_NAME][0] || strcmp(diniFiles[fileIndex][E_DINI_FILE_NAME], file))
	{
	    return 0;
	}
	
	KillTimer(diniFiles[fileIndex][E_DINI_FILE_TIMER]);
	dini_OnFileTimeout(fileIndex);
	return 0;
}
