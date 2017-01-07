#if defined string_included
	#endinput
#endif
#define string_included

stock STR_strins(string[], const substr[], pos, maxlength = sizeof(string))
{
	if (string[0] > 255)
	{
		new strlength = strlen(string);
		new sublength = strlen(substr);
		new m4 = maxlength * 4;

		if (strlength + sublength >= m4)
		{
			if (pos + sublength >= m4)
				return string{pos} = '\0', strcat(string, substr, maxlength);
			else
				string{maxlength - sublength - 1} = '\0';
		}
		return strins(string, substr, pos, maxlength);
	}
	else if (substr[0] > 255)
	{
		new strlength = strlen(string);
		new sublength = strlen(substr);

		if (strlength + sublength >= maxlength)
		{
			if (pos + sublength >= maxlength)
				return string[pos] = '\0', strcat(string, substr, maxlength);
			else
				string[maxlength - sublength - 1] = '\0';
		}

		return strins(string, substr, pos, maxlength);
	}
	else return format(string, maxlength, "%.*s%s%s", pos, string, substr, string[pos]);
}
#if defined _ALS_strins
	#undef strins
#else
	#define _ALS_strins
#endif
#define strins STR_strins

stock STR_valstr(dest[], value, bool:pack = false)
{
	static const __cellmin[] = !"-2147483648";

	if (value == cellmin)
		pack && strpack(dest, __cellmin, 12) || strunpack(dest, __cellmin, 12);
	else
		format(dest, 12, "%d", value), pack && strpack(dest, dest, 12);

	return 0;
}
#if defined _ALS_valstr
	#undef valstr
#else
	#define _ALS_valstr
#endif
#define valstr STR_valstr

stock STR_strcmp(const string1[], const string2[], bool:ignorecase = false, length = cellmax)
{
	if (string1[0])
		return (string2[0]) ? (strcmp(string1, string2, ignorecase, length)) : (1);
	else
		return (string2[0]) ? (-1) : (0);
}
#if defined _ALS_strcmp
	#undef strcmp
#else
	#define _ALS_strcmp
#endif
#define strcmp STR_strcmp

/*
native strsplit(dest[][], string[], const sub[], bool:ignorecase = false, pos = 0, limit = -1, len = sizeof (dest[]));
*/
stock strsplit(dest[][], string[], const sub[], bool:ignorecase = false, pos = 0, limit = -1, len = sizeof (dest[]))
{
	if (pos <= -1)
	    return 0;

	if (limit != -1 && limit <= 0)
	    return 0;

	new sublen = strlen(sub);
	new count;
	new last;

	while ((pos = strfind(string, sub, ignorecase, pos)) != -1)
	{
		strmid((dest[count][0] = EOS, dest[count]), string, last, pos, len);
	    last = ((pos + sublen) + 1);

		count++;
		if (limit > 0 && count >= limit)
		{
		    break;
		}
	}
	strmid((dest[count][0] = EOS, dest[count]), string, last, len);
	
	return count;
}

/*
native strcount(const string[], const sub[], bool:ignorecase = false, pos = 0);
*/
stock strcount(const string[], const sub[], bool:ignorecase = false, pos = 0)
{
	if (pos <= -1)
	    return 0;

	new sublen = strlen(sub);
	new count;

	while ((pos = strfind(string, sub, ignorecase, pos)) != -1)
	{
		count++;
	}
	
	return count;
}

/*
native strreplace(string[], const sub[], const replace[], bool:ignorecase = false, pos = 0, limit = -1, maxlength = sizeof (string));
*/
stock strreplace(string[], const sub[], const replace[], bool:ignorecase = false, pos = 0, limit = -1, maxlength = sizeof (string))
{
	if (pos <= -1)
	    return -1;

	if (limit != -1 && limit <= 0)
	    return -1;

	new sublen = strlen(sub);
	new count;

 	while ((pos = strfind(string, sub, ignorecase, pos)) != -1)
	{
	    strdel(string, pos, (pos + sublen));
		strins(string, replace, pos, maxlength);

		count++;
		if (limit > 0 && count >= limit)
		{
		    break;
		}
	}

    return pos;
}
