// arraylist.inc v1.0 - by Gammix - updated on 4 Dec, 2016
#if defined arraylist_included
	#endinput
#endif
#define arraylist_included

/*
native ArrayList_Add(ArrayList:name, {Float, _}:..);
native ArrayList_Remove(ArrayList:name, {Float, _}:..);
native ArrayList_RemoveIndex(ArrayList:name, index);
native ArrayList_sizeof(ArrayList:name);
native ArrayList_PoolSize(ArrayList:name);
native ArrayList_Set(ArrayList:name, index, {Float, _}:...);
native ArrayList_Get(ArrayList:name, index, {Float, _}:...);
native ArrayList_IndexOf(ArrayList:name, {Float, _}:...);
native ArrayList_Contains(ArrayList:name, {Float, _}:...);
native ArrayList_Random(ArrayList:name, start, end, {Float, _}:...);
*/

forward ArrayList_Fix();
public  ArrayList_Fix()
{
	strcat("", "");
}

#define INT 0
#define FLT 1
#define STR 2

#define ArrayList:: \
    ArrayList_

#define ArrayList_Integer:%1<%2> \
	%1_ARRAY_Con[%2][1], \
	%1_ARRAY_Type = INT, \
	%1_ARRAY_Size = %2, \
	%1_ARRAY_ConSize = 1, \
	%1_ARRAY_Pool

#define ArrayList_Float:%1<%2> \
	Float: %1_ARRAY_Con[%2][1], \
	%1_ARRAY_Type = FLT, \
	%1_ARRAY_Size = %2, \
	%1_ARRAY_ConSize = 1, \
	%1_ARRAY_Pool
	
#define ArrayList_String:%1<%2>[%3] \
	%1_ARRAY_Con[%2 + 1][%3], \
	%1_ARRAY_Type = STR, \
	%1_ARRAY_Size = %2, \
	%1_ARRAY_ConSize = %3, \
	%1_ARRAY_Pool

#define foreach(%1:%2) \
	for (new %2IDX; ArrayList_Get(%2, %2IDX, %1) != 0; %2IDX++)
	
#define ArrayList_Add(%1,%2) \
	ArrayList_InternalAdd(%1_ARRAY_Type, _: %1_ARRAY_Con, %1_ARRAY_Size, %1_ARRAY_ConSize, %1_ARRAY_Pool, %2)

stock static _InternalGetStringArg(dest[], arg, len = sizeof (dest)) // By Slice
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

stock ArrayList_InternalAdd(type, array[][], size, contentsize, &pool, {Float, _}:...)
{
	if (pool == size)
 	{
		return -1;
	}

	new i = pool++;
	switch (type)
	{
		case INT:
		{
			array[i][0] = getarg(5);
		}

		case FLT:
		{
			array[i][0] = _: (Float: getarg(5));
		}

		default:
		{
      		_InternalGetStringArg(array[i], 5, contentsize);
		}
	}
	return pool;
}

#define ArrayList_Remove(%1,%2) \
	ArrayList_InternalRemove(%1_ARRAY_Type, _: %1_ARRAY_Con, %1_ARRAY_Size, %1_ARRAY_ConSize, %1_ARRAY_Pool, %2)

stock ArrayList_InternalRemove(type, array[][], size, contentsize, &pool, {Float, _}:...)
{
	switch (type)
	{
		case INT:
		{
		    new bool: valfound;
		    for (new i, val = getarg(5); i < pool; i++)
		    {
				if (valfound)
				{
				    array[i - 1][0] = array[i][0];
				}
				else if (array[i][0] == val)
				{
				    valfound = true;
				}
		    }

		    if (!valfound)
		    {
		        return 0;
		    }

		    pool--;
		}

		case FLT:
		{
		    new bool: valfound;
		    for (new i, Float: val = Float: getarg(5); i < pool; i++)
		    {
				if (valfound)
				{
				    array[i - 1][0] = array[i][0];
				}
				else if (Float: array[i][0] == val)
				{
				    valfound = true;
				}
		    }

		    if (!valfound)
		    {
		        return 0;
		    }

		    pool--;
		}

		default:
		{
		    _InternalGetStringArg(array[size], 5, contentsize);
		    new bool: valfound;
		    for (new i; i < pool; i++)
		    {
				if (valfound)
				{
				    format(array[i - 1], contentsize, array[i]);
				}
				else if (!strcmp(array[i], array[size]))
				{
				    valfound = true;
				}
		    }

		    if (!valfound)
		    {
		        return 0;
		    }

		    pool--;
		}
	}
	
	return 1;
}

#define ArrayList_RemoveIndex(%1,%2) \
	ArrayList_InternalRemoveIndex(%1_ARRAY_Type, _: %1_ARRAY_Con, %1_ARRAY_Size, %1_ARRAY_ConSize, %1_ARRAY_Pool, %2)

stock ArrayList_InternalRemoveIndex(type, array[][], size, contentsize, &pool, index)
{
	if (index >= pool)
	{
		return 0;
	}

	pool--;
	return 1;
}

#define ArrayList_sizeof(%1) \
    %1_ARRAY_Size

#define ArrayList_PoolSize(%1) \
    (%1_ARRAY_Pool - 1)

#define ArrayList_Set(%1,%2,%3) \
	ArrayList_InternalSet(%1_ARRAY_Type, _: %1_ARRAY_Con, %1_ARRAY_ConSize, %1_ARRAY_Pool, %2, %3)

stock ArrayList_InternalSet(type, array[][], contentsize, pool, index, {Float, _}:...)
{
	if (index >= pool)
 	{
		return 0;
	}

	switch (type)
	{
		case INT:
		{
			array[index][0] = getarg(5);
		}

		case FLT:
		{
			array[index][0] = _: (Float: getarg(5));
		}

		default:
		{
      		_InternalGetStringArg(array[index], 5, contentsize);
		}
	}
	
	return 1;
}

#define ArrayList_Get(%1,%2,%3) \
	ArrayList_InternalGet(%1_ARRAY_Type, _: %1_ARRAY_Con, %1_ARRAY_ConSize, %1_ARRAY_Pool, %2, %3)

stock static _InternalSetStringArg(arg, const value[], maxlength = sizeof(value)) // By Y_Less
{
	new address;
    #emit LOAD.S.pri arg
    #emit SMUL.C     4
	#emit LOAD.S.alt 0
    #emit ADD
    #emit ADD.C      12
    #emit LOAD.I
	#emit STOR.S.PRI address
	#emit PUSH.S     maxlength
	#emit PUSH.S     value
	#emit CONST.pri  0
	#emit SREF.S.pri address
	#emit PUSH.S     address
	#emit PUSH.C     12
	#emit SYSREQ.C   strcat
	#emit STACK      16
}

stock ArrayList_InternalGet(type, array[][], contentsize, pool, index, {Float, _}:...)
{
	if (index >= pool)
 	{
		return 0;
	}

	switch (type)
	{
		case INT:
		{
			setarg(5, 0, array[index][0]);
		}

		case FLT:
		{
			setarg(5, 0, _: array[index][0]);
		}

		default:
		{
      		_InternalSetStringArg(5, array[index], contentsize);
		}
	}

	return 1;
}

#define ArrayList_IndexOf(%1,%2) \
	ArrayList_InternalIndexOf(%1_ARRAY_Type, _: %1_ARRAY_Con, %1_ARRAY_Size, %1_ARRAY_ConSize, %1_ARRAY_Pool, %2)

stock ArrayList_InternalIndexOf(type, array[][], size, contentsize, pool, {Float, _}:...)
{
	switch (type)
	{
		case INT:
		{
		    for (new i, val = getarg(5); i < pool; i++)
		    {
				if (array[i][0] == val)
				{
				    return i;
				}
		    }
		}

		case FLT:
		{
		    for (new i, Float: val = Float: getarg(5); i < pool; i++)
		    {
				if (Float: array[i][0] == val)
				{
				    return i;
				}
		    }
		}

		default:
		{
		    _InternalGetStringArg(array[size], 5, contentsize);
		    for (new i; i < pool; i++)
		    {
				if (!strcmp(array[i], array[size]))
				{
				    return i;
				}
		    }
		}
	}

	return -1;
}

#define ArrayList_Contains(%1,%2) \
	ArrayList_InternalContains(%1_ARRAY_Type, _: %1_ARRAY_Con, %1_ARRAY_Size, %1_ARRAY_ConSize, %1_ARRAY_Pool, %2)

stock ArrayList_InternalContains(type, array[][], size, contentsize, pool, {Float, _}:...)
{
	switch (type)
	{
		case INT:
		{
		    for (new i, val = getarg(5); i < pool; i++)
		    {
				if (array[i][0] == val)
				{
				    return 1;
				}
		    }
		}

		case FLT:
		{
		    for (new i, Float: val = Float: getarg(5); i < pool; i++)
		    {
				if (Float: array[i][0] == val)
				{
				    return 1;
				}
		    }
		}

		default:
		{
		    _InternalGetStringArg(array[size], 5, contentsize);
		    for (new i; i < pool; i++)
		    {
				if (!strcmp(array[i], array[size]))
				{
				    return 1;
				}
		    }
		}
	}

	return 0;
}

#define ArrayList_Random(%1,%2,%3,%4) \
	ArrayList_InternalRandom(%1_ARRAY_Type, _: %1_ARRAY_Con, %1_ARRAY_ConSize, %1_ARRAY_Pool, %2, %3, %4)
	
stock ArrayList_InternalRandom(type, array[][], contentsize, pool, start, end, {Float, _}:...)
{
	if (start < 0 || start >= pool)
	    return -1;

	if (end <= start || end >= pool)
	    return -1;

    new index = random((end - start) + 1) + start;
	switch (type)
	{
		case INT:
		{
			setarg(6, 0, array[index][0]);
		}

		case FLT:
		{
			setarg(6, 0, _: array[index][0]);
		}

		default:
		{
      		_InternalSetStringArg(6, array[index], contentsize);
		}
	}

	return index;
}
