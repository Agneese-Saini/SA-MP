#if defined dini2_included
	#endinput
#endif
#define dini2_included

/**********************************************/
// Dini2.inc | v2.8 | Updated: 6 Oct, 2017 | By Gammix
// New API code
/**********************************************/

// definitions
#if !defined DINI_MAX_FILES
	#define DINI_MAX_FILES \
		16
#endif

#if !defined DINI_MAX_FILE_NAME
	#define DINI_MAX_FILE_NAME \
		128
#endif

#if !defined DINI_MAX_FIELDS
	#define DINI_MAX_FIELDS \
		64
#endif

#if !defined DINI_MAX_FIELD_NAME
	#define DINI_MAX_FIELD_NAME \
		32
#endif

#if !defined DINI_MAX_FIELD_VALUE
	#define DINI_MAX_FIELD_VALUE \
		256
#endif

// variables decleration
enum E_DINI {
	E_DINI_FILE_NAME[DINI_MAX_FILE_NAME],
	E_DINI_TIMER,
	bool:E_DINI_SAVE_FILE,
	E_DINI_NUM_FIELDS
};

static diniFileData[Dini:DINI_MAX_FILES][E_DINI];
static diniFileFields[Dini:DINI_MAX_FILES][DINI_MAX_FIELDS][DINI_MAX_FIELD_NAME];
static diniFileValues[Dini:DINI_MAX_FILES][DINI_MAX_FIELDS][DINI_MAX_FIELD_VALUE];
static Dini:diniFilePoolSize = Dini:-1;

// functions decelration
Dini:dini_open(const filename[], timeout = 100);
dini_is_valid(Dini:index);
dini_close(Dini:index);
Dini:dini_get_file_poolsize();
dini_get_file_name(Dini:index, dest[], maxlength = sizeof(dest));
dini_num_fields(Dini:index);
dini_get_field_id(Dini:index, const field[]);
dini_remove_field_id(Dini:index, fieldid);
dini_get_field_name(Dini:index, fieldid, dest[], maxlength = sizeof(dest));
dini_set(Dini:index, fieldid, const value[]);
dini_get(Dini:index, fieldid, dest[], maxlength = sizeof(dest));
dini_set_assoc(Dini:index, const field[], const value[]);
dini_get_assoc(Dini:index, const field[], dest[], maxlength = sizeof(dest));

// callbacks decelrations
forward OnDiniFileTimeout(Dini:index);

// callbacks implementation
public OnDiniFileTimeout(Dini:index) {
	if (diniFileData[index][E_DINI_SAVE_FILE]) {
		new File:h = fopen(diniFileData[index][E_DINI_FILE_NAME], io_write);
		if (h) {
		    new line[DINI_MAX_FIELD_NAME + 1 + DINI_MAX_FIELD_VALUE];
		    for (new i; i < diniFileData[index][E_DINI_NUM_FIELDS]; i++) {
				format(line, sizeof(line), "%s=%s\r\n", diniFileFields[index][i], diniFileValues[index][i]);
				fwrite(h, line);
			}

			fclose(h);
		}
	}

	diniFileData[index][E_DINI_FILE_NAME][0] = EOS;

	if (diniFilePoolSize == index) {
		for (new Dini:i = diniFilePoolSize; i != Dini:-1; i--) {
			if (diniFileData[index][E_DINI_FILE_NAME][0]) {
                diniFilePoolSize = i;
				break;
			}
		}
	}

	if (diniFilePoolSize == index) {
        diniFilePoolSize = Dini:-1;
	}
}

// functions implementation
stock Dini:dini_open(const filename[], timeout = 100) {
	for (new Dini:i; i <= diniFilePoolSize; i++) {
		if (diniFileData[i][E_DINI_FILE_NAME][0] && !strcmp(diniFileData[i][E_DINI_FILE_NAME], filename)) {
			return i;
		}
	}

	new Dini:index = Dini:-1;
	for (new Dini:i; i < Dini:DINI_MAX_FILES; i++) {
		if (!diniFileData[i][E_DINI_FILE_NAME][0]) {
			index = i;
			break;
		}
	}

	if (index == Dini:-1) {
		return Dini:-1;
	}

	if (fexist(filename)) {
		new File:h = fopen(filename, io_read);
		if (!h) {
			return Dini:-2;
		}

		diniFileData[index][E_DINI_NUM_FIELDS] = 0;

		new line[DINI_MAX_FIELD_NAME + 1 + DINI_MAX_FIELD_VALUE];
		new pos;
		new len;
		new fieldIndex;
		while (fread(h, line)) {
			pos = strfind(line, "=");
			if (pos == -1) {
				continue;
			}

			if (diniFileData[index][E_DINI_NUM_FIELDS] == DINI_MAX_FIELDS) {
				break;
			}

			len = strlen(line);
	        if (len >= 2 && line[len - 2] == '\r') {
	            line[len - 2] = EOS;
	        }
	        else if (len >= 1 && line[len - 1] == '\n') {
	            line[len - 1] = EOS;
			}

			fieldIndex = diniFileData[index][E_DINI_NUM_FIELDS]++;
			strmid(diniFileFields[index][fieldIndex], line, 0, pos, DINI_MAX_FIELD_NAME);
			format(diniFileValues[index][fieldIndex], DINI_MAX_FIELD_VALUE, line[pos + 1]);
		}

		fclose(h);
	}
	else {
     	new File:h = fopen(filename, io_write);
		if (!h) {
			return Dini:-2;
		}

		diniFileData[index][E_DINI_NUM_FIELDS] = 0;

		fclose(h);
	}

	format(diniFileData[index][E_DINI_FILE_NAME], DINI_MAX_FILE_NAME, filename);
	if (timeout <= 0) {
	    diniFileData[index][E_DINI_TIMER] = -1;
	}
	else {
    	diniFileData[index][E_DINI_TIMER] = SetTimerEx("OnDiniFileTimeout", timeout, false, "i", _:index);
    }
	diniFileData[index][E_DINI_SAVE_FILE] = false;

    if (diniFilePoolSize < index) {
        diniFilePoolSize = index;
	}

	return index;
}

stock dini_is_valid(Dini:index) {
	if (index <= Dini:-1 || index > diniFilePoolSize) {
		return -1;
	}

	return (!diniFileData[index][E_DINI_FILE_NAME][0]) ? (0) : (1);
}

stock dini_close(Dini:index) {
	if (dini_is_valid(index) != 1) {
		return 0;
	}

	KillTimer(diniFileData[index][E_DINI_TIMER]);
 	OnDiniFileTimeout(index);
	return 1;
}

stock Dini:dini_get_file_poolsize() {
	return diniFilePoolSize;
}

stock dini_get_file_name(Dini:index, dest[], maxlength = sizeof(dest)) {
	if (dini_is_valid(index) != 1) {
		return 0;
	}

	format(dest, maxlength, diniFileData[index][E_DINI_FILE_NAME]);
	return 1;
}

stock dini_num_fields(Dini:index) {
	if (dini_is_valid(index) != 1) {
		return 0;
	}

	return diniFileData[index][E_DINI_NUM_FIELDS];
}

stock dini_get_field_id(Dini:index, const field[]) {
	if (dini_is_valid(index) != 1) {
		return -1;
	}

	for (new i; i < diniFileData[index][E_DINI_NUM_FIELDS]; i++) {
		if (!strcmp(diniFileFields[index][i], field)) {
			return i;
		}
	}
	return -1;
}

stock dini_remove_field_id(Dini:index, fieldid) {
	if (dini_is_valid(index) != 1) {
		return -1;
	}

	if (fieldid < 0 || fieldid >= diniFileData[index][E_DINI_NUM_FIELDS]) {
		return 0;
	}

	for (new i = fieldid; i < (diniFileData[index][E_DINI_NUM_FIELDS] - 1); i++) {
		format(diniFileFields[index][i], DINI_MAX_FIELD_NAME, diniFileFields[index][i + 1]);
		format(diniFileValues[index][i], DINI_MAX_FIELD_VALUE, diniFileValues[index][i + 1]);
	}

	diniFileData[index][E_DINI_SAVE_FILE] = true;
	return 1;
}

stock dini_get_field_name(Dini:index, fieldid, dest[], maxlength = sizeof(dest)) {
	if (dini_is_valid(index) != 1) {
		return -1;
	}

	if (fieldid < 0 || fieldid >= diniFileData[index][E_DINI_NUM_FIELDS]) {
		return 0;
	}

	format(dest, maxlength, diniFileFields[index][fieldid]);
	return 1;
}

stock dini_set(Dini:index, fieldid, const value[]) {
	if (dini_is_valid(index) != 1) {
		return -1;
	}

	if (fieldid < 0 || fieldid >= diniFileData[index][E_DINI_NUM_FIELDS]) {
		return 0;
	}

	format(diniFileValues[index][fieldid], DINI_MAX_FIELD_VALUE, value);

	diniFileData[index][E_DINI_SAVE_FILE] = true;
	return 1;
}

stock dini_get(Dini:index, fieldid, dest[], maxlength = sizeof(dest)) {
	if (dini_is_valid(index) != 1) {
		return -1;
	}

	if (fieldid < 0 || fieldid >= diniFileData[index][E_DINI_NUM_FIELDS]) {
		return 0;
	}

	format(dest, maxlength, diniFileValues[index][fieldid]);
	return 1;
}

stock dini_set_assoc(Dini:index, const field[], const value[]) {
	if (dini_is_valid(index) != 1) {
		return 0;
	}

	for (new i; i < diniFileData[index][E_DINI_NUM_FIELDS]; i++) {
		if (!strcmp(diniFileFields[index][i], field)) {
		    format(diniFileValues[index][i], DINI_MAX_FIELD_VALUE, value);
			return 1;
		}
	}

	if (diniFileData[index][E_DINI_NUM_FIELDS] == DINI_MAX_FIELDS) {
		return -1;
	}

	new fieldIndex = diniFileData[index][E_DINI_NUM_FIELDS]++;
	format(diniFileFields[index][fieldIndex], DINI_MAX_FIELD_NAME, field);
	format(diniFileValues[index][fieldIndex], DINI_MAX_FIELD_VALUE, value);

	diniFileData[index][E_DINI_SAVE_FILE] = true;
	return 1;
}

stock dini_get_assoc(Dini:index, const field[], dest[], maxlength = sizeof(dest)) {
	if (dini_is_valid(index) != 1) {
		return 0;
	}

	for (new i; i < diniFileData[index][E_DINI_NUM_FIELDS]; i++) {
		if (!strcmp(diniFileFields[index][i], field)) {
		    format(dest, maxlength, diniFileValues[index][i]);
			return 1;
		}
	}
	return -1;
}

/**********************************************/
// DINI's API with new code but same syntax
/**********************************************/

// functions decelration
dini_Exists(const filename[]);
dini_Remove(const filename[]);
dini_Create(const filename[]);
dini_Set(const filename[], const key[], const value[]);
dini_IntSet(const filename[], const key[], value);
dini_FloatSet(const filename[], const key[], Float:value);
dini_BoolSet(const filename[], const key[], bool:value);
dini_Get(const filename[], const key[]);
dini_Int(const filename[], const key[]);
Float:dini_Float(const filename[], const key[]);
bool:dini_Bool(const filename[], const key[]);
dini_Unset(const filename[], const key[]);
dini_Isset(const filename[], const key[]);
DINI_StripNewLine(const string[]);
DINI_fcopytextfile(const filename[], const newfilename[]);

// definitions
#define dini_Exists \
	fexist

#define dini_Remove \
    fremove

// functions implementation
static Dini:dini_InternalOpen(const filename[]) {
	new name[DINI_MAX_FILE_NAME];
	for (new Dini:i; i <= dini_get_file_poolsize(); i++) {
		dini_get_file_name(i, name, DINI_MAX_FILE_NAME);

		if (!strcmp(name, filename)) {
	        return i;
		}
	}

	new Dini:index = dini_open(filename);
	if (index == Dini:-1) {
		dini_close(Dini:0);
		
		index = dini_open(filename);
		if (index == Dini:0) {
			return Dini:0;
		}
	}
	return Dini:-1;
}

stock dini_Create(const filename[]) {
	if (fexist(filename)) {
		return 0;
	}

	new File:h = fopen(filename, io_write);
	if (!h) {
		return 0;
	}

	fclose(h);
	return 1;
}

stock dini_Set(const filename[], const key[], const value[]) {
	return dini_set_assoc(dini_InternalOpen(filename), key, value);
}

stock dini_IntSet(const filename[], const key[], value) {
 	new dest[16];
	valstr(dest, value);
	return dini_set_assoc(dini_InternalOpen(filename), key, dest);
}

stock dini_FloatSet(const filename[], const key[], Float:value) {
 	new dest[16];
	format(dest, sizeof(dest), "%f", value);
	return dini_set_assoc(dini_InternalOpen(filename), key, dest);
}

stock dini_BoolSet(const filename[], const key[], bool:value) {
 	new dest[2];
	valstr(dest, _:value);
	return dini_set_assoc(dini_InternalOpen(filename), key, dest);
}

stock dini_Get(const filename[], const key[]) {
	new value[DINI_MAX_FIELD_VALUE];
	dini_get_assoc(dini_InternalOpen(filename), key, value, DINI_MAX_FIELD_VALUE);
	return value;
}

stock dini_Int(const filename[], const key[]) {
	new value[16];
	dini_get_assoc(dini_InternalOpen(filename), key, value, sizeof(value));
	return strval(value);
}

stock Float:dini_Float(const filename[], const key[]) {
	new value[16];
	dini_get_assoc(dini_InternalOpen(filename), key, value, sizeof(value));
	return floatstr(value);
}

stock bool:dini_Bool(const filename[], const key[]) {
	new value[2];
	dini_get_assoc(dini_InternalOpen(filename), key, value, sizeof(value));
	return bool:strval(value);
}

stock dini_Unset(const filename[], const key[]) {
	new Dini:index = dini_InternalOpen(filename);
	return dini_remove_field_id(index, dini_get_field_id(index, key));
}

stock dini_Isset(const filename[], const key[]) {
	return (dini_get_field_id(dini_InternalOpen(filename), key) == -1) ? (0) : (1);
}

stock dini_Timeout(const filename[]) {
	new name[DINI_MAX_FILE_NAME];
	for (new Dini:i; i <= dini_get_file_poolsize(); i++) {
		dini_get_file_name(i, name, DINI_MAX_FILE_NAME);

		if (!strcmp(name, filename)) {
	        return dini_close(i);
		}
	}

	return 0;
}

stock DINI_StripNewLine(const string[]) {
    new len = strlen(string);
	if (len >= 2 && string[len - 2] == '\r') {
 		string[len - 2] = EOS;
 		return 1;
	}
 	else if (len >= 1 && string[len - 1] == '\n') {
  		string[len - 1] = EOS;
 		return 1;
	}
	return 0;
}

stock DINI_fcopytextfile(const filename[], const newfilename[]) {
	new File:h1 = fopen(filename, io_read);
	if (!h1) {
	    return 0;
	}

	new File:h2 = fopen(newfilename, io_write);
	if (!h2) {
	    fclose(h1);
	    return 0;
	}

	new string[DINI_MAX_FIELD_NAME + 1 + DINI_MAX_FIELD_VALUE];
	while (fread(h1, string)) {
		fwrite(h2, string);
	}

	fclose(h1);
	fclose(h2);
	return 1;
}
