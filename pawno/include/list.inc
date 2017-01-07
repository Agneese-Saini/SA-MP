#if defined list_included
  #endinput
#endif
#define list_included

#define List:%1[%2] \
	%1@LIST[%2] = {-1, ...}, \
	%1@POOLSIZE = 0, \
	%1@DEFAULT = -1

#define list:: list_

/*
native list::default(List: name, value);
*/
#define list_default(%1,%2) \
	internal_list_default(%1@LIST, sizeof(%1@LIST), %1@POOLSIZE, %1@DEFAULT, %2)
stock static internal_list_default(array[], size, poolsize, &def, value)
{
	for (new i = poolsize; i < size; i++)
	{
	    array[i] = value;
	}
	def = value;
	return 1;
}

/*
native list::add(List: name, value);
*/
#define list_add(%1,%2) \
	internal_list_add(%1@LIST, sizeof(%1@LIST), %1@POOLSIZE, %2)
stock static internal_list_add(array[], size, &poolsize, value)
{
	if (poolsize == size)
	{
	    return -1;
	}

	new i = poolsize++;
	array[i] = value;
	return i;
}

/*
native list::delete(List: name, value);
*/
#define list_delete(%1,%2) \
	internal_list_delete(%1@LIST, %1@POOLSIZE, %1@DEFAULT, %2)
stock static internal_list_delete(array[], &poolsize, def, value)
{
	for (new i; i < poolsize; i++)
	{
	    if (array[i] == value)
	    {
	        while (i < (poolsize - 1))
	        {
	            array[i] = array[++i];
	        }
	        array[--poolsize] = def;
         	return 1;
   		}
	}
	return 0;
}

/*
native list::sizeof(List: name);
*/
#define list_sizeof(%1) \
	%1@POOLSIZE

/*
native list::indexof(List: name, value);
*/
#define list_indexof(%1,%2) \
	internal_list_indexof(%1@LIST, %1@POOLSIZE, %2)
stock static internal_list_indexof(const array[], poolsize, value)
{
	for (new i; i < poolsize; i++)
	{
		if (array[i] == value)
		{
		    return i;
		}
	}
	return -1;
}

/*
native list::random(List: name);
*/
#define list_random(%1) \
	%1@LIST[random(%1@POOLSIZE)]

/*
native list::sort(List: name, type);
*/
#define SORT_ASC 0
#define SORT_DESC 1

#define list_sort(%1,%2) \
	internal_list_sort(%1@LIST, %1@POOLSIZE, %2)
stock static internal_list_sort(array[], poolsize, type)
{
	new swap;
	if (type == SORT_ASC)
	{
	    for (new i; i < (poolsize - 1); i++)
	    {
	        for (new x = (i + 1); x < poolsize; x++)
	        {
	            if (array[x] < array[i])
				{
					swap = array[i];
					array[i] = array[x];
					array[x] = swap;
	            }
	        }
	    }
	}
	else
	{
	    for (new i; i < (poolsize - 1); i++)
	    {
	        for (new x = (i + 1); x < poolsize; x++)
	        {
	            if (array[x] > array[i])
				{
					swap = array[i];
					array[i] = array[x];
					array[x] = swap;
	            }
	        }
	    }
	}
	return 1;
}

/*
native list::return(List: name, index);
*/
#define list_return(%1,%2) \
	%1@LIST[%2]

#define foreach(%1:%2) \
	for (new %1, %2@ID; (%1 = %2@LIST[%2@ID]) != %2@DEFAULT; %2@ID++)
