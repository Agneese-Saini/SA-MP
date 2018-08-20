/**************************************/
// strcalc.inc - v1.3 - Last Updated: 20 August, 2018 - By Gammix
/***************************************/

#if defined strcalc_included
	#endinput
#endif
#define strcalc_included

#if !defined STRCALC_MAX_STRING_SIZE
	#define STRCALC_MAX_STRING_SIZE 128
#endif

#if !defined FLOAT_INFINITY
	#define FLOAT_INFINITY Float:0x7F800000
#endif

#if !defined FLOAT_NAN
	#define FLOAT_NAN Float:(cellmin - 1)
#endif

static Float:STRCALC_number(const string[], start, end) {
	new tmp[16];
	strmid(tmp, string, start, end + 1);
	return (strfind(tmp, ".", true) != -1) ? floatstr(tmp) : float(strval(tmp));
}

static STRCALC_operation(opr) {
    switch (opr) {
	    case '+': return 1; // addition
	    case '-': return 1; // substraction
	    case '^': return 2; // power
	    case '%': return 2; // modulus
	    case '*': return 3; // multiplication
	    case '/': return 3; // division
    }

    return 0;
}

static Float:STRCALC_calculate(opr, Float:a, Float:b) {
    switch (opr) {
	    case '+': return floatadd(a, b); // addition
		case '-': return floatsub(a, b); // substraction
	    case '^': return floatpower(a, b); // power
	    case '%': return float(floatround(a) % floatround(b)); // modulus
		case '*': return floatmul(a, b); // multiplication
		case '/': return (b == 0.0) ? (FLOAT_INFINITY) : (floatdiv(a, b)); // division
	}

    return FLOAT_NAN;
}

stock Float:strcalc(const string[], start = 0, end = -1, begin = 1) {
	static copy_string[STRCALC_MAX_STRING_SIZE];
	
	new c, num_brackets, current_operation = -1, operation_index = -1;

	if (begin == 1) {
	    new pos = start;
	    new limit = (end == -1) ? (strlen(string) - 1) : (end);
	    
		strmid(copy_string, string, start, limit + 1);
	    
		for (;;) {
			if ((pos = strfind(copy_string, " ", true, pos)) == -1)
			    break;
			    
			if (pos >= limit)
				break;
			    
			strdel(copy_string, pos, pos + 1);
			--pos;
			--limit;
	 	}

		end = limit;
	}

    for (new i = start; i <= end; i++) {
        c = copy_string[i];

        if (c == '(') {
            ++num_brackets;
        }
		else if (c == ')') {
            if (num_brackets == 0) {
                return FLOAT_NAN;
            }
            --num_brackets;
        }
		else {
			if (num_brackets == 0) {
	            current_operation = STRCALC_operation(c);

	            if (current_operation != 0 && (operation_index == -1 || current_operation < STRCALC_operation(copy_string[operation_index]))) {
	                operation_index = i;
	            }
			}
        }
    }

    if (num_brackets > 0) {
        return FLOAT_NAN;
    }

    if (operation_index != -1) {
    	new Float:a = strcalc(copy_string, start, operation_index - 1, 0);
		new Float:b = strcalc(copy_string, operation_index + 1, end, 0);

		// printf("%0.1f %c %0.1f = %0.1f", a, copy_string[operation_index], b, STRCALC_calculate(copy_string[operation_index], a, b));

  		return STRCALC_calculate(copy_string[operation_index], a, b);
    }

	if (copy_string[start] == '(' && copy_string[end] == ')') {
 		return strcalc(copy_string, start + 1, end - 1, 0);
   	}

	return STRCALC_number(copy_string, start, end);
}
