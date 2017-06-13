// dini2.inc - v2.6 - updated 12 June, 2017 - By Gammix
#if defined dini2_included
	#endinput
#endif
#define dini2_included

#if !defined DINI_MAX_HANDLES
	#define DINI_MAX_HANDLES (16)
#endif

#if !defined DINI_MAX_FILE_NAME
	#define DINI_MAX_FILE_NAME (128)
#endif

#if !defined DINI_TIMER_INTERVAL
	#define DINI_TIMER_INTERVAL (150)
#endif

#if !defined DINI_MAX_FIELDS
	#define DINI_MAX_FIELDS (64)
#endif

#if !defined DINI_MAX_FIELD_NAME
	#define DINI_MAX_FIELD_NAME (64)
#endif

#if !defined DINI_MAX_FIELD_VALUE
	#define DINI_MAX_FIELD_VALUE (512)
#endif

enum E_DINI_FILE
{
    E_DINI_FILE_NAME[DINI_MAX_FILE_NAME],
    E_DINI_FILE_TIMER,
	bool:E_DINI_FILE_MODIFIED,
	E_DINI_FILE_NUM_FIELDS
};

static diniFile[DINI_MAX_HANDLES][E_DINI_FILE];
static diniFileFields[DINI_MAX_HANDLES][DINI_MAX_FIELDS][DINI_MAX_FIELD_NAME];
static diniFileValues[DINI_MAX_HANDLES][DINI_MAX_FIELDS][DINI_MAX_FIELD_VALUE];
static diniFilePoolSize = -1;

stock dini_GetFileIndex(const file[])
{
	for (new i; i <= diniFilePoolSize; i++)
	{
	    if (diniFile[i][E_DINI_FILE_NAME][0] != EOS && !strcmp(diniFile[i][E_DINI_FILE_NAME], file))
	    {
	        return i;
	    }
	}

	new index = -1;
	for (new i; i < DINI_MAX_HANDLES; i++)
	{
	    if (diniFile[i][E_DINI_FILE_NAME][0] == EOS)
	    {
	        index = i;
	        break;
	    }
	}
	
	if (index == -1)
	{
        KillTimer(diniFile[0][E_DINI_FILE_TIMER]);
 		dini_OnFileTimeout(0);
 		index = 0;
	}

    diniFile[index][E_DINI_FILE_NUM_FIELDS] = 0;
	if (fexist(file))
	{
		new File:handle = fopen(file, io_read);
		if (!handle)
		{
		    return -1;
		}

		new string[DINI_MAX_FIELD_NAME/*field*/ + 1/*=*/ + DINI_MAX_FIELD_VALUE/*value*/];
		new pos;
		new len;
		new fieldIndex;
		
	    while (fread(handle, string))
	    {
	        pos = strfind(string, "=");
	        if (pos == -1)
	        {
	            continue;
	        }
	        
	        if (diniFile[index][E_DINI_FILE_NUM_FIELDS] == DINI_MAX_FIELDS)
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

			fieldIndex = diniFile[index][E_DINI_FILE_NUM_FIELDS]++;
			strmid(diniFileFields[index][fieldIndex], string, 0, pos, DINI_MAX_FIELD_NAME);
			format(diniFileValues[index][fieldIndex], DINI_MAX_FIELD_VALUE, string[pos + 1]);
	    }
	    
	    fclose(handle);
    }
    
    if (index > diniFilePoolSize)
    {
        diniFilePoolSize = index;
    }
    
    format(diniFile[index][E_DINI_FILE_NAME], DINI_MAX_FILE_NAME, file);
    diniFile[index][E_DINI_FILE_TIMER] = SetTimerEx("dini_OnFileTimeout", DINI_TIMER_INTERVAL, false, "i", index);
    diniFile[index][E_DINI_FILE_MODIFIED] = false;
	return index;
}

forward dini_OnFileTimeout(index);
public dini_OnFileTimeout(index)
{
	if (diniFile[index][E_DINI_FILE_MODIFIED])
	{
		new File:handle = fopen(diniFile[index][E_DINI_FILE_NAME], io_write);
		if (handle)
		{
			new string[DINI_MAX_FIELD_NAME/*field*/ + 1/*=*/ + DINI_MAX_FIELD_VALUE/*value*/];
		    for (new i; i < (diniFile[index][E_DINI_FILE_NUM_FIELDS] - 1); i++)
			{
			    format(string, sizeof string, "%s=%s\r\n", diniFileFields[index][i], diniFileValues[index][i]);
			    fwrite(handle, string);
			}
			format(string, sizeof string, "%s=%s", diniFileFields[index][(diniFile[index][E_DINI_FILE_NUM_FIELDS] - 1)], diniFileValues[index][(diniFile[index][E_DINI_FILE_NUM_FIELDS] - 1)]);
			fwrite(handle, string);
			
			fclose(handle);
		}
	}
	
	diniFile[index][E_DINI_FILE_NAME][0] = EOS;

	for (new i = diniFilePoolSize; i > -1; i--)
	{
	    if (diniFile[i][E_DINI_FILE_NAME][0] != EOS && diniFilePoolSize < i)
	    {
	        diniFilePoolSize = i;
	        break;
	    }
	}
}

/*
native dini_Create(const file[]);
*/
#define dini_Create \
    dini_GetFileIndex

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
native dini_Get(const file[], const field[]);
*/
stock dini_Get(const file[], const field[])
{
	static empty[DINI_MAX_FIELD_VALUE];
	if (!fexist(file))
	{
	    return empty;
	}

	new index = dini_GetFileIndex(file);
	if (index == -1)
	{
	    return empty;
	}

	for (new i; i < diniFile[index][E_DINI_FILE_NUM_FIELDS]; i++)
	{
 		if (!strcmp(field, diniFileFields[index][i]))
 		{
 		    return diniFileValues[index][i];
 		}
	}
	return empty;
}

/*
native dini_Int(const file[], const field[]);
*/
#define dini_Int(%1,%2) \
	strval(dini_Get(%1,%2))

/*
native Float:dini_Float(const file[], const field[]);
*/
#define dini_Float(%1,%2) \
	floatstr(dini_Get(%1,%2))

/*
native bool:dini_Bool(const file[], const field[]);
*/
#define dini_Bool(%1,%2) \
	bool:strval(dini_Get(%1,%2))

/*
native dini_Set(const file[], const field[], value[]);
*/
stock dini_Set(const file[], const field[], value[])
{
	new index = dini_GetFileIndex(file);
	if (index == -1)
	{
	    return 0;
	}

    for (new i; i < diniFile[index][E_DINI_FILE_NUM_FIELDS]; i++)
	{
 		if (!strcmp(field, diniFileFields[index][i]))
 		{
 		    format(diniFileValues[index][i], DINI_MAX_FIELD_VALUE, value);
    		diniFile[index][E_DINI_FILE_MODIFIED] = true;
 		    return 1;
 		}
	}

	if (diniFile[index][E_DINI_FILE_NUM_FIELDS] == DINI_MAX_FIELDS)
	{
	    printf("[dini2] - Error: Field \"%s\" couldn't be created in file \"%s\", increase the limit of \"DINI_MAX_FIELDS\" to have more fields.", field, file, (DINI_MAX_FIELDS + 1));
		return 0;
	}

	new i = diniFile[index][E_DINI_FILE_NUM_FIELDS]++;
	format(diniFileFields[index][i], DINI_MAX_FIELD_NAME, field);
	format(diniFileValues[index][i], DINI_MAX_FIELD_VALUE, value);
	diniFile[index][E_DINI_FILE_MODIFIED] = true;
	return 0;
}

/*
native dini_IntSet(const file[], const field[], value);
*/
stock dini_IntSet(const file[], const field[], value)
{
	new index = dini_GetFileIndex(file);
	if (index == -1)
	{
	    return 0;
	}

    for (new i; i < diniFile[index][E_DINI_FILE_NUM_FIELDS]; i++)
	{
 		if (!strcmp(field, diniFileFields[index][i]))
 		{
 		    valstr(diniFileValues[index][i], value);
    		diniFile[index][E_DINI_FILE_MODIFIED] = true;
 		    return 1;
 		}
	}

	if (diniFile[index][E_DINI_FILE_NUM_FIELDS] == DINI_MAX_FIELDS)
	{
	    printf("[dini2] - Error: Field \"%s\" couldn't be created in file \"%s\", increase the limit of \"DINI_MAX_FIELDS\" to have more fields.", field, file, (DINI_MAX_FIELDS + 1));
		return 0;
	}

	new i = diniFile[index][E_DINI_FILE_NUM_FIELDS]++;
	format(diniFileFields[index][i], DINI_MAX_FIELD_NAME, field);
	valstr(diniFileValues[index][i], value);
	diniFile[index][E_DINI_FILE_MODIFIED] = true;
	return 0;
}

/*
native dini_FloatSet(const file[], const field[], Float:value);
*/
stock dini_FloatSet(const file[], const field[], Float:value)
{
	new index = dini_GetFileIndex(file);
	if (index == -1)
	{
	    return 0;
	}

    for (new i; i < diniFile[index][E_DINI_FILE_NUM_FIELDS]; i++)
	{
 		if (!strcmp(field, diniFileFields[index][i]))
 		{
 		    format(diniFileValues[index][i], DINI_MAX_FIELD_VALUE, "%f", value);
    		diniFile[index][E_DINI_FILE_MODIFIED] = true;
 		    return 1;
 		}
	}

	if (diniFile[index][E_DINI_FILE_NUM_FIELDS] == DINI_MAX_FIELDS)
	{
	    printf("[dini2] - Error: Field \"%s\" couldn't be created in file \"%s\", increase the limit of \"DINI_MAX_FIELDS\" to have more fields.", field, file, (DINI_MAX_FIELDS + 1));
		return 0;
	}

	new i = diniFile[index][E_DINI_FILE_NUM_FIELDS]++;
	format(diniFileFields[index][i], DINI_MAX_FIELD_NAME, field);
	format(diniFileValues[index][i], DINI_MAX_FIELD_VALUE, "%f", value);
	diniFile[index][E_DINI_FILE_MODIFIED] = true;
	return 0;
}

/*
native dini_BoolSet(const file[], const field[], bool:value);
*/
#define dini_BoolSet(%1,%2,%3) \
    dini_IntSet(%1,%2,_:%3)

/*
native dini_UnSet(const file[], const field[]);
*/
stock dini_UnSet(const file[], const field[])
{
	new index = dini_GetFileIndex(file);
	if (index == -1)
	{
	    return 0;
	}

	for (new i; i < diniFile[index][E_DINI_FILE_NUM_FIELDS]; i++)
	{
 		if (!strcmp(field, diniFileFields[index][i]))
 		{
 		    for (; i < (diniFile[index][E_DINI_FILE_NUM_FIELDS] - 1); i++)
 		    {
 		        diniFileFields[index][i] = diniFileFields[index][i + 1];
 		        diniFileValues[index][i] = diniFileValues[index][i + 1];
 		    }
 		    
 		    diniFile[index][E_DINI_FILE_NUM_FIELDS]--;
    		diniFile[index][E_DINI_FILE_MODIFIED] = true;
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
	new index = dini_GetFileIndex(file);
	if (index == -1)
	{
	    return 0;
	}

	for (new i; i < diniFile[index][E_DINI_FILE_NUM_FIELDS]; i++)
	{
 		if (!strcmp(field, diniFileFields[index][i]))
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
	new File:handle1 = fopen(file, io_read);
	if (!handle1)
	{
	    return 0;
	}

	new File:handle2 = fopen(newfile, io_write);
	if (!handle2)
	{
	    fclose(handle1);
	    return 0;
	}

	new string[DINI_MAX_FIELD_NAME/*field*/ + 1/*=*/ + DINI_MAX_FIELD_VALUE/*value*/];
	while (fread(handle1, string))
	{
		fwrite(handle2, string);
	}

	fclose(handle1);
	fclose(handle2);
	
	return 1;
}

/*
native dini_Timeout(const file[]);
*/
stock dini_Timeout(const file[])
{
    for (new i; i <= diniFilePoolSize; i++)
	{
	    if (diniFile[i][E_DINI_FILE_NAME][0] != EOS && !strcmp(diniFile[i][E_DINI_FILE_NAME], file))
	    {
		    KillTimer(diniFile[i][E_DINI_FILE_TIMER]);
		    dini_OnFileTimeout(i);
		    return 1;
	    }
	}
	return 0;
}
