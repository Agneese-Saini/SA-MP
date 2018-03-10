#if defined dini2_included
	#endinput
#endif
#define dini2_included

/**********************************************/
// Dini2.inc | v3.1 | Updated: 10 March, 2018 | By Gammix
/**********************************************/

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

#define INVALID_DINI_FILE \
	Dini:-1

enum E_DINI {
	E_DINI_FILENAME[DINI_MAX_FILE_NAME],
	bool:E_DINI_FILE_MODIFIED,
	E_DINI_NUM_FIELDS,
	E_DINI_TIMER
};

static diniFile[Dini:DINI_MAX_FILES][E_DINI];
static diniFileFields[Dini:DINI_MAX_FILES][DINI_MAX_FIELDS][DINI_MAX_FIELD_NAME];
static diniFileValues[Dini:DINI_MAX_FILES][DINI_MAX_FIELDS][DINI_MAX_FIELD_VALUE];

static Dini:dini_ParseFile(const filename[], timeout = 150) {
	new Dini:file = INVALID_DINI_FILE;
	for (new Dini:i = Dini:0; i < Dini:DINI_MAX_FILES; i++) {
		if (diniFile[i][E_DINI_FILENAME][0]) {
			if (!strcmp(diniFile[i][E_DINI_FILENAME], filename)) {
				return i;
			}
		}
		else {
			if (file == INVALID_DINI_FILE) {
				file = i;
			}
		}
	}

	if (file == INVALID_DINI_FILE) {
		KillTimer(diniFile[Dini:0][E_DINI_TIMER]);
		OnDiniFileTimeout(Dini:0);
		file = Dini:0;
	}

	diniFile[file][E_DINI_NUM_FIELDS] = 0;

	if (fexist(filename)) {
		new File:h = fopen(filename, io_read);
		if (!h) {
			return INVALID_DINI_FILE;
		}

		new line[DINI_MAX_FIELD_NAME + 3 + DINI_MAX_FIELD_VALUE];
		new pos;
		new len;
		new fieldid;
		while (fread(h, line)) {
			pos = strfind(line, "=");
			if (pos == -1) {
				continue;
			}

			if (diniFile[file][E_DINI_NUM_FIELDS] == DINI_MAX_FIELDS) {
				break;
			}

			len = strlen(line);
	        if (len >= 2 && line[len - 2] == '\r') {
	            line[len - 2] = EOS;
	        }
	        else if (len >= 1 && line[len - 1] == '\n') {
	            line[len - 1] = EOS;
			}

			fieldid = diniFile[file][E_DINI_NUM_FIELDS]++;
			strmid(diniFileFields[file][fieldid], line, 0, pos, DINI_MAX_FIELD_NAME);
			format(diniFileValues[file][fieldid], DINI_MAX_FIELD_VALUE, line[pos + 1]);
		}

		fclose(h);
	}

	format(diniFile[file][E_DINI_FILENAME], DINI_MAX_FILE_NAME, filename);
	diniFile[file][E_DINI_FILE_MODIFIED] = false;

	if (timeout > 0) {
		diniFile[file][E_DINI_TIMER] = SetTimerEx("OnDiniFileTimeout", timeout, false, "i", _:file);
	}

	return file;
}

forward OnDiniFileTimeout(Dini:file);
public OnDiniFileTimeout(Dini:file) {
	if (diniFile[file][E_DINI_FILE_MODIFIED]) {
		new File:h = fopen(diniFile[file][E_DINI_FILENAME], io_write);
		if (h) {
	  		new line[DINI_MAX_FIELD_NAME + 3 + DINI_MAX_FIELD_VALUE];
	  		for (new i = 0; i < diniFile[file][E_DINI_NUM_FIELDS]; i++) {
				format(line, sizeof line, "%s=%s\r\n", diniFileFields[file][i], diniFileValues[file][i]);
				fwrite(h, line);
			}

			fclose(h);
		}
	}

	diniFile[file][E_DINI_FILENAME][0] = EOS;
}

#define dini_Exists \
	fexist

#define dini_Remove \
    fremove

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
	new Dini:file = dini_ParseFile(filename);
	if (file == INVALID_DINI_FILE) {
		return 0;
	}

	for (new i = 0; i < diniFile[file][E_DINI_NUM_FIELDS]; i++) {
		if (!strcmp(diniFileFields[file][i], key)) {
			format(diniFileValues[file][i], DINI_MAX_FIELD_VALUE, value);
			diniFile[file][E_DINI_FILE_MODIFIED] = true;
			return 1;
		}
	}

	if (diniFile[file][E_DINI_NUM_FIELDS] == DINI_MAX_FIELDS) {
		return 0;
	}

	new fieldid = diniFile[file][E_DINI_NUM_FIELDS]++;
	format(diniFileFields[file][fieldid], DINI_MAX_FIELD_NAME, key);
	format(diniFileValues[file][fieldid], DINI_MAX_FIELD_VALUE, value);
	diniFile[file][E_DINI_FILE_MODIFIED] = true;
	return 1;
}

stock dini_IntSet(const filename[], const key[], value) {
 	new dest[16];
	valstr(dest, value);
	return dini_Set(filename, key, dest);
}

stock dini_FloatSet(const filename[], const key[], Float:value) {
 	new dest[16];
	format(dest, sizeof dest, "%f", value);
	return dini_Set(filename, key, dest);
}

stock dini_BoolSet(const filename[], const key[], bool:value) {
 	new dest[2];
	valstr(dest, _:value);
	return dini_Set(filename, key, dest);
}

stock dini_Get(const filename[], const key[]) {
	static const NULL[DINI_MAX_FIELD_VALUE] = "\0";

	new Dini:file = INVALID_DINI_FILE;
	for (new Dini:i = Dini:0; i < Dini:DINI_MAX_FILES; i++) {
		if (diniFile[i][E_DINI_FILENAME][0] && !strcmp(diniFile[i][E_DINI_FILENAME], filename)) {
			file = i;
			break;
		}
	}

	if (file == INVALID_DINI_FILE) {
		if (!fexist(filename)) {
			return NULL;
		}

		file = dini_ParseFile(filename);
		if (file == INVALID_DINI_FILE) {
	 		return NULL;
		}
	}


	for (new i = 0; i < diniFile[file][E_DINI_NUM_FIELDS]; i++) {
		if (!strcmp(diniFileFields[file][i], key)) {
			return diniFileValues[file][i];
		}
	}

	return NULL;
}

stock dini_Int(const filename[], const key[]) {
	return strval(dini_Get(filename, key));
}

stock Float:dini_Float(const filename[], const key[]) {
	return floatstr(dini_Get(filename, key));
}

stock bool:dini_Bool(const filename[], const key[]) {
	return bool:strval(dini_Get(filename, key));
}

stock dini_Unset(const filename[], const key[]) {
	new Dini:file = INVALID_DINI_FILE;
	for (new Dini:i = Dini:0; i < Dini:DINI_MAX_FILES; i++) {
		if (diniFile[i][E_DINI_FILENAME][0] && !strcmp(diniFile[i][E_DINI_FILENAME], filename)) {
			file = i;
			break;
		}
	}

	if (file == INVALID_DINI_FILE) {
		if (!fexist(filename)) {
			return 0;
		}

		file = dini_ParseFile(filename);
		if (file == INVALID_DINI_FILE) {
	 		return 0;
		}
	}


	for (new i = 0; i < diniFile[file][E_DINI_NUM_FIELDS]; i++) {
		if (!strcmp(diniFileFields[file][i], key)) {
		    for (; i < (diniFile[file][E_DINI_NUM_FIELDS] - 1); i++) {
		        diniFileFields[file][i] = diniFileFields[file][i + 1];
		        diniFileValues[file][i] = diniFileValues[file][i + 1];
		    }
		    diniFile[file][E_DINI_NUM_FIELDS]--;
		    diniFile[file][E_DINI_FILE_MODIFIED] = true;
			return 1;
		}
	}

	return 0;
}

stock dini_Isset(const filename[], const key[]) {
	new Dini:file = INVALID_DINI_FILE;
	for (new Dini:i = Dini:0; i < Dini:DINI_MAX_FILES; i++) {
		if (diniFile[i][E_DINI_FILENAME][0] && !strcmp(diniFile[i][E_DINI_FILENAME], filename)) {
			file = i;
			break;
		}
	}

	if (file == INVALID_DINI_FILE) {
		if (!fexist(filename)) {
			return 0;
		}

		file = dini_ParseFile(filename);
		if (file == INVALID_DINI_FILE) {
	 		return 0;
		}
	}


	for (new i = 0; i < diniFile[file][E_DINI_NUM_FIELDS]; i++) {
		if (!strcmp(diniFileFields[file][i], key)) {
			return 1;
		}
	}

	return 0;
}

stock dini_Timeout(const filename[]) {
	for (new Dini:i = Dini:0; i < Dini:DINI_MAX_FILES; i++) {
        if (diniFile[i][E_DINI_FILENAME][0] && !strcmp(diniFile[i][E_DINI_FILENAME], filename)) {
			KillTimer(diniFile[i][E_DINI_TIMER]);
			OnDiniFileTimeout(i);
			return 1;
		}
	}

	return 0;
}

stock dini_NumKeys(const filename[]) {
	new Dini:file = INVALID_DINI_FILE;
	for (new Dini:i = Dini:0; i < Dini:DINI_MAX_FILES; i++) {
		if (diniFile[i][E_DINI_FILENAME][0] && !strcmp(diniFile[i][E_DINI_FILENAME], filename)) {
			file = i;
			break;
		}
	}

	if (file == INVALID_DINI_FILE) {
		if (!fexist(filename)) {
			return 0;
		}

		file = dini_ParseFile(filename);
		if (file == INVALID_DINI_FILE) {
	 		return 0;
		}
	}


	return diniFile[file][E_DINI_NUM_FIELDS];
}

stock dini_GetKeyName(const filename[], keyid) {
	static const NULL[DINI_MAX_FIELD_NAME] = "\0";

	new Dini:file = INVALID_DINI_FILE;
	for (new Dini:i = Dini:0; i < Dini:DINI_MAX_FILES; i++) {
		if (diniFile[i][E_DINI_FILENAME][0] && !strcmp(diniFile[i][E_DINI_FILENAME], filename)) {
			file = i;
			break;
		}
	}

	if (file == INVALID_DINI_FILE) {
		if (!fexist(filename)) {
			return NULL;
		}

		file = dini_ParseFile(filename);
		if (file == INVALID_DINI_FILE) {
	 		return NULL;
		}
	}

	if (keyid < 0 || keyid >= diniFile[file][E_DINI_NUM_FIELDS]) {
		return NULL;
	}

	return diniFileFields[file][keyid];
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
    new File:newFile = fopen(newfilename, io_write);
	if (!newFile) {
	    return 0;
	}

	new Dini:file = INVALID_DINI_FILE;
	for (new Dini:i = Dini:0; i < Dini:DINI_MAX_FILES; i++) {
		if (diniFile[i][E_DINI_FILENAME][0] && !strcmp(diniFile[i][E_DINI_FILENAME], filename)) {
			file = i;
			break;
		}
	}

	if (file != INVALID_DINI_FILE) {
	 	new line[DINI_MAX_FIELD_NAME + 3 + DINI_MAX_FIELD_VALUE];
	 	for (new i = 0; i < diniFile[file][E_DINI_NUM_FIELDS]; i++) {
			format(line, sizeof line, "%s=%s\r\n", diniFileFields[file][i], diniFileValues[file][i]);
			fwrite(newFile, line);
		}
	}
	else {
		new File:copyFile = fopen(filename, io_read);
		if (!copyFile) {
		    return 0;
		}

		new line[DINI_MAX_FIELD_NAME + 3 + DINI_MAX_FIELD_VALUE];
		while (fread(copyFile, line)) {
			fwrite(newFile, line);
		}

		fclose(copyFile);
	}

	fclose(newFile);
	return 1;
}

stock DINI_frenametextfile(const filename[], const newfilename[]) {
    new File:newFile = fopen(newfilename, io_write);
	if (!newFile) {
	    return 0;
	}

	new Dini:file = INVALID_DINI_FILE;
	for (new Dini:i = Dini:0; i < Dini:DINI_MAX_FILES; i++) {
		if (diniFile[i][E_DINI_FILENAME][0] && !strcmp(diniFile[i][E_DINI_FILENAME], filename)) {
			file = i;
			break;
		}
	}

	if (file != INVALID_DINI_FILE) {
	    format(diniFile[file][E_DINI_FILENAME], DINI_MAX_FILE_NAME, newfilename);
	}
	else {
		new File:copyFile = fopen(filename, io_read);
		if (!copyFile) {
		    return 0;
		}

		new line[DINI_MAX_FIELD_NAME + 3 + DINI_MAX_FIELD_VALUE];
		while (fread(copyFile, line)) {
			fwrite(newFile, line);
		}

		fclose(copyFile);
	}

	fclose(newFile);
	fremove(filename);
	return 1;
}
