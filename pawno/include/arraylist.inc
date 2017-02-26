// arraylist.inc v2.0 - by Gammix - updated on 26 Feb, 2017
#if defined arraylist_included
	#endinput
#endif
#define arraylist_included

#define ArrayList<%1> \
	ArrayList_%1

#define INT \
	0
#define FLT \
	1
#define STR \
	2

#define ArrayList_int:%1<%2>(%3) \
	%1ARRAY@Type = INT, \
	%1ARRAY@Count, \
	%1ARRAY@Content[%2][%3]

#define ArrayList_float:%1<%2>(%3) \
	%1ARRAY@Type = FLT, \
	%1ARRAY@Count, \
	Float:%1ARRAY@Content[%2][%3]

#define ArrayList_string:%1<%2>(%3) \
	%1ARRAY@Type = STR, \
	%1ARRAY@Count, \
	%1ARRAY@Content[%2][%3]

forward __arraylist_fix();
public __arraylist_fix()
{
	strcat("", "", 0);
}

stock static __GetStringArg(dest[], arg, len = sizeof (dest)) // By Slice
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

stock static __SetStringArg(arg, const value[], maxlength = sizeof(value)) // By Y_Less
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

/*
native ArrayList_Add(ArrayList:<>, {Float, _}:...);
*/
#define ArrayList_Add(%1,%2) \
	__ArrayList_Add(%1ARRAY@Type, %1ARRAY@Count, %1ARRAY@Content, sizeof(%1ARRAY@Content), sizeof(%1ARRAY@Content[]), %2)

stock __ArrayList_Add(type, &count, array[][], size1, size2, {Float, _}:...)
{
	if (count == size1)
	    return -1;

	new n,
	 	i;
	n = numargs();
	if (n == 7)
		i = getarg(6);
	else
	    i = 0;

	switch (type)
	{
	    case INT:
	        array[count++][i] = getarg(5);
	    case FLT:
	        array[count++][i] = _:float(getarg(5));
	    case STR:
	    {
	        if (n == 7)
	        	array[count++][i] = getarg(5);
			else
	        	__GetStringArg(array[count++], 5, size2);
		}
	}
	return (count - 1);
}

/*
native ArrayList_Remove(ArrayList:<>, index);
*/
#define ArrayList_Remove(%1,%2) \
	__ArrayList_Remove(%1ARRAY@Count, %1ARRAY@Content, sizeof(%1ARRAY@Content[]), %2)

stock __ArrayList_Remove(&count, array[][], size2, index)
{
	if (index < 0 || index >= count)
	    return 0;

	for (new i = index; i < (count - 1); i++)
	{
	    format(array[i], size2, array[i + 1]);
	}
	count--;
	return 1;
}

/*
native ArrayList_Set(ArrayList:<>, index, {Float, _}:...);
*/
#define ArrayList_Set(%1,%2,%3) \
	__ArrayList_Set(%1ARRAY@Type, %1ARRAY@Count, %1ARRAY@Content, sizeof(%1ARRAY@Content[]), %2, %3)

stock __ArrayList_Set(type, &count, array[][], size2, index, {Float, _}:...)
{
	if (index < 0 || index >= count)
	    return 0;

	new n,
	 	i;
	n = numargs();
	if (n == 7)
		i = getarg(6);
	else
	    i = 0;

	switch (type)
	{
	    case INT:
	        array[index][i] = getarg(5);
	    case FLT:
	        array[index][i] = _:float(getarg(5));
	    case STR:
	    {
	        if (n == 7)
	        	array[index][i] = getarg(5);
			else
	        	__GetStringArg(array[index], 5, size2);
		}
	}
	return 1;
}

/*
native ArrayList_Get(ArrayList:<>, index, {Float, _}:...);
*/
#define ArrayList_Get(%1,%2,%3) \
	__ArrayList_Get(%1ARRAY@Type, %1ARRAY@Count, %1ARRAY@Content, sizeof(%1ARRAY@Content[]), %2, %3)

stock __ArrayList_Get(type, &count, array[][], size2, index, {Float, _}:...)
{
	if (index < 0 || index >= count)
	    return 0;

	new n,
	 	i;
	n = numargs();
	if (n == 7)
		i = getarg(6);
	else
	    i = 0;

	switch (type)
	{
	    case INT:
	        setarg(5, .value = array[index][i]);
	    case FLT:
	        setarg(5, .value = _:array[index][i]);
	    case STR:
	    {
	        if (n == 7)
	        	setarg(5, .value = array[index][i]);
			else
	        	__SetStringArg(5, array[index], size2);
		}
	}
	return 1;
}

/*
native ArrayList_Size(ArrayList:<>);
*/
#define ArrayList_Size(%1) \
    sizeof(%1ARRAY@Content)

/*
native ArrayList_InternalSize(ArrayList:<>);
*/
#define ArrayList_InternalSize(%1) \
    sizeof(%1ARRAY@Content[])

/*
native ArrayList_PoolSize(ArrayList:<>);
*/
#define ArrayList_PoolSize(%1) \
    (%1ARRAY@Count - 1)

/*
native ArrayList_IndexOf(ArrayList:<>, internal_index, {Float, _}:...);
*/
#define ArrayList_IndexOf(%1,%2,%3) \
	__ArrayList_IndexOf(%1ARRAY@Type, %1ARRAY@Count, %1ARRAY@Content, %2, %3)

stock __ArrayList_IndexOf(type, count, array[][], internal_index, {Float, _}:...)
{
	switch (type)
	{
	    case INT:
	    {
	        new val = getarg(4);
	        for (new i; i < count; i++)
	        {
				if (array[i][internal_index] == val)
					return i;
	        }
	    }
		case FLT:
	    {
	        new Float:val = getarg(4);
	        for (new i; i < count; i++)
	        {
				if (array[i][internal_index] == val)
					return i;
	        }
	    }
	    case STR:
	    {
	       	new val[256];
		   	__GetStringArg(val, 4, sizeof(val));
	        for (new i; i < count; i++)
	        {
				if (!strcmp(val, array[i][internal_index]))
					return i;
	        }
		}
	}
	return -1;
}
