/**************************************/
// strcalc.inc - v1.1 - Last Updated: 16 August, 2018 - By Gammix
/***************************************/

#if defined strcalc_included
	#endinput
#endif
#define strcalc_included

#if !defined FLOAT_INFINITY
	#define FLOAT_INFINITY Float:0x7F800000
#endif

static Float:STRCALC_number(const string[], start, end) {
	new tmp[16];
	strmid(tmp, string, start, end + 1);

	if (strfind(tmp, ".", true) != -1) {
	    return floatstr(tmp);
	}
	
    return float(strval(tmp));
}

static STRCALC_operation(opr) {
    switch (opr) {
	    case '+': return 1; // addition
	    case '-': return 1; // substraction
	    case '*': return 2; // multiplication
	    case '/': return 2; // division
    }

    return 0;
}

static Float:STRCALC_calculate(opr, Float:a, Float:b) {
    switch (opr) {
	    case '+': { // addition
			return a + b;
		}

		case '-': { // substraction
			return a - b;
		}

		case '*': { // multiplication
			return a * b;
		}

		case '/': { // division
			if (b == 0.0) {
				return FLOAT_INFINITY;
			}
			return a / b;
		}
	}
	
    return 0.0;
}

stock Float:strcalc(const string[], start = 0, end = -1) {
	new c, num_brackets, current_operation = -1, operation_index = -1;
	
	if (end == -1) {
		end = strlen(string) - 1;
	}
	
    for (new i = start; i <= end; i++) {
        c = string[i];

        if (c == '(') {
            ++num_brackets;
        }
		else if (c == ')') {
            if (num_brackets == 0) {
                return 0.0;
            }
            --num_brackets;
        }
		else {
			if (num_brackets == 0) {
	            current_operation = STRCALC_operation(c);

	            if (current_operation != 0 && (operation_index == -1 || current_operation < STRCALC_operation(string[operation_index]))) {
	                operation_index = i;
	            }
			}
        }
    }

    if (num_brackets > 0) {
        return 0.0;
    }

    if (operation_index != -1) {
    	new Float:a = strcalc(string, start, operation_index - 1);
		new Float:b = strcalc(string, operation_index + 1, end);
  		return STRCALC_calculate(string[operation_index], a, b);
    }

	if (string[start] == '(' && string[end] == ')') {
 		return strcalc(string, start + 1, end - 1);
   	}

	return STRCALC_number(string, start, end);
}
