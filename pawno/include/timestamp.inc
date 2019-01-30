// Timestamp.inc by Gammix
// Version: 2.0 - 29 Jan, 2019 - 9:14:32 PM
// Special thanks: Crayder (his Time Conversion library was used as a reference)
#if defined _inc_timestamp_
	#endinput
#endif
#define _inc_timestamp_

#define EPOCH_YEAR 1970

#define MAX_TIMESTAMP_FORMAT_LENGTH 128

#if defined _inc_timestamp_natives_
native Timestamp(year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, hourGMT = 0, minuteGMT = 0);
native ParseTimestamp(timestamp, &year = 0, &month = 0, &day = 0, &hour = 0, &minute = 0, &second = 0, hourGMT = 0, minuteGMT = 0);

native FormatTimestamp(timestamp, const format[], hourGMT = 0, minuteGMT = 0);
native FormatTimeleft(startTimestamp, endTimestamp);

native ConvertFromSeconds(TimeUnit:type, seconds);
native ConvertToSeconds(TimeUnit:type, value);
#endif

enum TimeUnit {
	Year,
	Month,
	Day,
	Hour,
	Minute
};

static const MONTH_DAYS[12]	= {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static const MONTH_NAMES[12][] = {
	"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

static stock bool:IsLeapYear(year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static stock GetMonthDays(month, year) {
	if (month == 1 && IsLeapYear(year) == true) {
	    return 29;
	}

	return MONTH_DAYS[month - 1];
}

static stock ReplaceString(string[], start, end, const replacement[], size) {
	strdel(string, start, end);
	strins(string, replacement, start, size);
}

stock Timestamp(year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, hourGMT = 0, minuteGMT = 0) {
    static const
		SECONDS_PER_MINUTE = 60,
		SECONDS_PER_HOUR = 3600,
		SECONDS_PER_DAY = 86400;

	new timestamp = 0;

	if (year == 0 || month == 0 || day == 0) {
	    getdate(year, month, day);
	    gettime(hour, minute, second);
	}

	for (new i = EPOCH_YEAR; i < year; i++)
	    day += (IsLeapYear(i) == true) ? 365 : 366;

	for (new i = 1; i < month; i++)
		day += MONTH_DAYS[i - 1];

	if (IsLeapYear(year) == true && month > 2)
	    day += 1;

    timestamp += ((day - 1) * SECONDS_PER_DAY);
    timestamp += ((hour + hourGMT) * SECONDS_PER_HOUR);
    timestamp += ((minute + minuteGMT) * SECONDS_PER_MINUTE);
    timestamp += second;

	return timestamp;
}

stock ParseTimestamp(timestamp, &year = 0, &month = 0, &day = 0, &hour = 0, &minute = 0, &second = 0, hourGMT = 0, minuteGMT = 0) {
    static const
		SECONDS_PER_MINUTE = 60,
		SECONDS_PER_HOUR = 3600,
		SECONDS_PER_DAY = 86400;

	new days = 0, seconds = 0;

	for (year = EPOCH_YEAR; ; year++) {
	    days = (IsLeapYear(year) == true) ? 365 : 366;
		seconds = days * SECONDS_PER_DAY;

		if (seconds > timestamp)
			break;

		timestamp -= seconds;
	}

	for (month = 1; ; month++) {
		days = MONTH_DAYS[month - 1];
		seconds = days * SECONDS_PER_DAY;

		if (seconds > timestamp)
			break;

		timestamp -= seconds;
	}

	for (day = 1; timestamp >= SECONDS_PER_DAY; day++)
		timestamp -= SECONDS_PER_DAY;

	for (hour = hourGMT; timestamp >= SECONDS_PER_HOUR; hour++)
		timestamp -= SECONDS_PER_HOUR;

	for (minute = minuteGMT; timestamp >= SECONDS_PER_MINUTE; minute++)
		timestamp -= SECONDS_PER_MINUTE;

	second = timestamp;
}

stock FormatTimestamp(timestamp, const format[], hourGMT = 0, minuteGMT = 0) {
	new string[MAX_TIMESTAMP_FORMAT_LENGTH];
	strcat(string, format);
	
	new year, month, day, hour, minute, second;
	ParseTimestamp(timestamp, year, month, day, hour, minute, second, hourGMT, minuteGMT);

	new pos = -1;

	do {
	    // year abbreviated (last 2 chars)
		if ((pos = strfind(string, "%y")) != -1) {
			new replacement[4];
			valstr(replacement, year % 100);
			ReplaceString(string, pos, pos + 2, replacement, MAX_TIMESTAMP_FORMAT_LENGTH);
		}
		// full year
		else if ((pos = strfind(string, "%Y")) != -1) {
			new replacement[5];
			valstr(replacement, year);
			ReplaceString(string, pos, pos + 2, replacement, MAX_TIMESTAMP_FORMAT_LENGTH);
		}
		// month number
		else if ((pos = strfind(string, "%m")) != -1) {
			new replacement[4];
			valstr(replacement, month);
			ReplaceString(string, pos, pos + 2, replacement, MAX_TIMESTAMP_FORMAT_LENGTH);
		}
		// month name abbreviated
		else if ((pos = strfind(string, "%b")) != -1) {
		    new replacement[4];
		    strcat(replacement, MONTH_NAMES[month - 1]);
			ReplaceString(string, pos, pos + 2, replacement, MAX_TIMESTAMP_FORMAT_LENGTH);
		}
		// full month name
		else if ((pos = strfind(string, "%B")) != -1) {
			ReplaceString(string, pos, pos + 2, MONTH_NAMES[month - 1], MAX_TIMESTAMP_FORMAT_LENGTH);
		}
		// day
		else if ((pos = strfind(string, "%d")) != -1) {
			new replacement[4];
			valstr(replacement, day);
			ReplaceString(string, pos, pos + 2, replacement, MAX_TIMESTAMP_FORMAT_LENGTH);
		}
		// hour 24H
		else if ((pos = strfind(string, "%H")) != -1) {
			new replacement[4];
			valstr(replacement, hour);
			ReplaceString(string, pos, pos + 2, replacement, MAX_TIMESTAMP_FORMAT_LENGTH);
		}
		// hour 12H
		else if ((pos = strfind(string, "%I")) != -1) {
			new replacement[4];
			valstr(replacement, (hour > 12) ? (hour - 12) : (hour));
			ReplaceString(string, pos, pos + 2, replacement, MAX_TIMESTAMP_FORMAT_LENGTH);
		}
		// hour 12H AM or PM
		else if ((pos = strfind(string, "%p")) != -1) {
			ReplaceString(string, pos, pos + 2, (hour > 12) ? ("PM") : ("AM"), MAX_TIMESTAMP_FORMAT_LENGTH);
		}
		// minute
		else if ((pos = strfind(string, "%M")) != -1) {
			new replacement[4];
			valstr(replacement, minute);
			ReplaceString(string, pos, pos + 2, replacement, MAX_TIMESTAMP_FORMAT_LENGTH);
		}
		// second
		else if ((pos = strfind(string, "%S")) != -1) {
			new replacement[4];
			valstr(replacement, second);
			ReplaceString(string, pos, pos + 2, replacement, MAX_TIMESTAMP_FORMAT_LENGTH);
		}
	} while (pos != -1);
	
	return string;
}

stock FormatTimeleft(startTimestamp, endTimestamp) {
    static const
		SECONDS_PER_MINUTE = 60,
		SECONDS_PER_HOUR = 3600,
		SECONDS_PER_DAY = 86400,
		SECONDS_PER_MONTH = 2592000;

    new string[MAX_TIMESTAMP_FORMAT_LENGTH];

	new seconds = endTimestamp - startTimestamp;

	if (seconds == 1)
		format(string, sizeof(string), "a second");
	else if (seconds < SECONDS_PER_MINUTE)
		format(string, sizeof(string), "%i seconds", seconds);
	else if (seconds < (2 * SECONDS_PER_MINUTE))
		format(string, sizeof(string), "a minute");
	else if (seconds < (45 * SECONDS_PER_MINUTE))
		format(string, sizeof(string), "%i minutes", (seconds / SECONDS_PER_MINUTE));
	else if (seconds < (90 * SECONDS_PER_MINUTE))
		format(string, sizeof(string), "an hour");
	else if (seconds < (24 * SECONDS_PER_HOUR))
		format(string, sizeof(string), "%i hours", (seconds / SECONDS_PER_HOUR));
	else if (seconds < (48 * SECONDS_PER_HOUR))
		format(string, sizeof(string), "a day");
	else if (seconds < (30 * SECONDS_PER_DAY))
		format(string, sizeof(string), "%i days", (seconds / SECONDS_PER_DAY));
	else if (seconds < (12 * SECONDS_PER_MONTH)) {
		new months = floatround(seconds / SECONDS_PER_DAY / 30);
      	if (months <= 1)
			format(string, sizeof(string), "a month");
      	else
			format(string, sizeof(string), "%i months", months);
	}
    else {
      	new years = floatround(seconds / SECONDS_PER_DAY / 365);
      	if (years <= 1)
			format(string, sizeof(string), "a year");
      	else
			format(string, sizeof(string), "%i years", years);
	}

	return string;
}

stock ConvertFromSeconds(TimeUnit:type, seconds) {
    static const
		SECONDS_PER_MINUTE = 60,
		SECONDS_PER_HOUR = 3600,
		SECONDS_PER_DAY = 86400,
		SECONDS_PER_YEAR = 31556952;

	switch (type) {
		case Year:
			return seconds / SECONDS_PER_YEAR;
		case Month:
		    return seconds / (SECONDS_PER_DAY * 31);
		case Day:
		    return seconds / SECONDS_PER_DAY;
		case Hour:
		    return seconds / SECONDS_PER_HOUR;
		case Minute:
		    return seconds / SECONDS_PER_MINUTE;
	}

	return 0;
}

stock ConvertToSeconds(TimeUnit:type, value) {
    static const
		SECONDS_PER_MINUTE = 60,
		SECONDS_PER_HOUR = 3600,
		SECONDS_PER_DAY = 86400,
		SECONDS_PER_YEAR = 31556952;

	switch (type) {
		case Year:
			return SECONDS_PER_YEAR * value;
		case Month:
		    return SECONDS_PER_DAY * (31 * value);
		case Day:
		    return SECONDS_PER_DAY * value;
		case Hour:
		    return SECONDS_PER_HOUR * value;
		case Minute:
		    return SECONDS_PER_MINUTE * value;
	}

	return 0;
}
