// Timestamp.inc by Gammix
// Version: 1.0 - 2018-12-26 20:47:11
// Special thanks: Crayder (his Time Conversion library was used as a reference)
#if defined _inc_timestamp_
	#endinput
#endif
#define _inc_timestamp_

#if defined _natives_timestamp_

// check if the year is leap year or not
bool:TimestampStatic.IsLeapYear(year);
// get number of days in entered month
TimestampStatic.GetMonthDays(month, year);
// get entered month name
TimestampStatic.GetMonthName(month);
// convert given unit to seconds
TimestampStatic.ConvertToSeconds(E_TIMEUNIT_TYPES:type, amount);
// convert seconds entered to given unit
TimestampStatic.ConvertFromSeconds(E_TIMEUNIT_TYPES:type, timestamp);
// convert date stamp to timestamp (returns timestamp)
TimestampStatic.DateToTime(year, month, day, hour, minute, second, HourGMT = 0, MinuteGMT = 0);
// convert timestamp to date stamp (stores in params)
TimestampStatic.TimeToDate(timestamp, &year, &month, &date, &hour, &minute, &second, HourGMT = 0, MinuteGMT = );


// create a Timestamp object from date parameters
Timestamp:TimestampFromDate(year, month, date, hour, minute, second);
// create a Timestamp object from timestamp supplied
Timestamp:TimestampFromTime(timestamp);
// create a Timestamp object for current time
Timestamp:Timestamp();
// check if timestamp object is valid
bool:Timestamp.IsValid(Timestamp:object);

// getters and setters
Timestamp.SetYear(Timestamp:object, year);
Timestamp.GetYear(Timestamp:object);
Timestamp.SetMonth(Timestamp:object, month);
Timestamp.GetMonth(Timestamp:object);
Timestamp.SetDate(Timestamp:object, date);
Timestamp.GetDate(Timestamp:object);
Timestamp.SetHour(Timestamp:object, hour);
Timestamp.GetHour(Timestamp:object);
Timestamp.SetMinute(Timestamp:object, minute);
Timestamp.GetMinute(Timestamp:object);
Timestamp.SetSecond(Timestamp:object, second);
Timestamp.GetSecond(Timestamp:object);

// returns timestamp value(seconds) in Timestamp object
Timestamp.Value(Timestamp:object);
// check if "object" is greater than "otherObject"
bool:Timestamp.IsGreater(Timestamp:object, Timestamp:otherObject);
// check if "object" is equal to "otherObject"
bool:Timestamp.IsEqual(Timestamp:object, Timestamp:otherObject);
// add time to Timestamp object (e.g. Timestamp.AddTime(object, .hour = 15); <-- adds 15 hours to object)
Timestamp.AddTime(Timestamp:object, year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0);
// subtract time from Timestamp object (e.g. Timestamp.SubtractTime(object, .hour = 15); <-- subtract 15 hours from object)
Timestamp.SubtractTime(Timestamp:object, year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0);
// return string format of timestamp (e.g. "2018-12-26 20:47:11")
Timestamp.ToString(Timestamp:object);
// return timestamp to some other units (e.g. Timestamp.ConvertToUnit(object, DAYS); <-- convert timestamp to number of days)
Timestamp.ConvertToUnit(Timestamp:object, E_TIMEUNIT_TYPES:type);
// destroy timestamp object
Timestamp.Destroy(Timestamp:object);

#endif

#if !defined MAX_TIMESTAMPS
	#define MAX_TIMESTAMPS 1000
#endif

#if !defined MAX_TIMESTAMP_STRING
	#define MAX_TIMESTAMP_STRING 64
#endif

#define INVALID_TIMESTAMP_ID Timestamp:-1

#define EPOCH_YEAR 1970

#define Timestamp. TS_
#define TimestampStatic. TSStatic_

enum E_TIMEUNIT_TYPES {
	YEAR,
	MONTH,
	DAY,
	HOUR,
	MINUTE,
	SECOND,
	MILLISECONDS
};

enum E_TIMESTAMP_DATA {
	bool:TIMESTAMP_VALID,
	TIMESTAMP_YEAR,
	TIMESTAMP_MONTH,
	TIMESTAMP_DATE,
	TIMESTAMP_HOUR,
	TIMESTAMP_MINUTE,
	TIMESTAMP_SECOND,
	TIMESTAMP_VALUE
};

static const MONTH_DAYS[12]	= {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static const MONTH_NAMES[12][] = {
	"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};

static timestampData[Timestamp:MAX_TIMESTAMPS][E_TIMESTAMP_DATA];

stock bool:TimestampStatic.IsLeapYear(year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

stock TimestampStatic.GetMonthDays(month, year) {
	if (month == 1 && TimestampStatic.IsLeapYear(year) == true) {
	    return 29;
	}

	return MONTH_DAYS[month - 1];
}

stock TimestampStatic.GetMonthName(month) {
	new ret[MAX_TIMESTAMP_STRING];
	format(ret, MAX_TIMESTAMP_STRING, MONTH_NAMES[month - 1]);
	return ret;
}

stock TimestampStatic.ConvertToSeconds(E_TIMEUNIT_TYPES:type, amount) {
    static const
		SECONDS_PER_MINUTE = 60,
		SECONDS_PER_HOUR = 3600,
		SECONDS_PER_DAY = 86400,
		SECONDS_PER_YEAR = 31556952;

	switch (type) {
		case YEAR:
			return SECONDS_PER_YEAR * amount;
		case MONTH:
		    return SECONDS_PER_DAY * (31 * amount);
		case DAY:
		    return SECONDS_PER_DAY * amount;
		case HOUR:
		    return SECONDS_PER_HOUR * amount;
		case MINUTE:
		    return SECONDS_PER_MINUTE * amount;
		case SECOND:
		    return amount;
	}

	return 0;
}

stock TimestampStatic.ConvertFromSeconds(E_TIMEUNIT_TYPES:type, timestamp) {
    static const
		SECONDS_PER_MINUTE = 60,
		SECONDS_PER_HOUR = 3600,
		SECONDS_PER_DAY = 86400,
		SECONDS_PER_YEAR = 31556952;

	switch (type) {
		case YEAR:
			return timestamp / SECONDS_PER_YEAR;
		case MONTH:
		    return timestamp / (SECONDS_PER_DAY * 31);
		case DAY:
		    return timestamp / SECONDS_PER_DAY;
		case HOUR:
		    return timestamp / SECONDS_PER_HOUR;
		case MINUTE:
		    return timestamp / SECONDS_PER_MINUTE;
		case SECOND:
		    return timestamp;
	}
	return 0;
}

stock TimestampStatic.DateToTime(year, month, day, hour, minute, second, HourGMT = 0, MinuteGMT = ) {
    static const
		SECONDS_PER_MINUTE = 60,
		SECONDS_PER_HOUR = 3600,
		SECONDS_PER_DAY = 86400;

	new timestamp = 0;

	for (new i = EPOCH_YEAR; i < year; i++)
	    day += (TimestampStatic.IsLeapYear(i) == true) ? 365 : 366;

	for (new i = 1; i < month; i++)
		day += MONTH_DAYS[i - 1];

	if (TimestampStatic.IsLeapYear(year) == true && month > 2)
	    day += 1;
	
    timestamp += ((day - 1) * SECONDS_PER_DAY);
    timestamp += ((hour + HourGMT) * SECONDS_PER_HOUR);
    timestamp += ((minute + MinuteGMT) * SECONDS_PER_MINUTE);
    timestamp += second;

	return timestamp;
}

stock TimestampStatic.TimeToDate(timestamp, &year, &month, &date, &hour, &minute, &second, HourGMT = 0, MinuteGMT = 0) {
    static const
		SECONDS_PER_MINUTE = 60,
		SECONDS_PER_HOUR = 3600,
		SECONDS_PER_DAY = 86400;

	new days = 0, seconds = 0;

	for (year = EPOCH_YEAR; ; year++) {
	    days = (TimestampStatic.IsLeapYear(year) == true) ? 365 : 366;
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

	for (date = 1; timestamp >= SECONDS_PER_DAY; date++)
		timestamp -= SECONDS_PER_DAY;

	for (hour = HourGMT; timestamp >= SECONDS_PER_HOUR; hour++)
		timestamp -= SECONDS_PER_HOUR;

	for (minute = MinuteGMT; timestamp >= SECONDS_PER_MINUTE; minute++)
		timestamp -= SECONDS_PER_MINUTE;

	second = timestamp;
}

stock Timestamp:TimestampFromDate(year, month, date, hour, minute, second) {
	new Timestamp:id = INVALID_TIMESTAMP_ID;

	for (new Timestamp:i = Timestamp:0; i < Timestamp:MAX_TIMESTAMPS; i++) {
		if (timestampData[i][TIMESTAMP_VALID] == false) {
			id = i;
			break;
		}
	}

	if (id == INVALID_TIMESTAMP_ID) {
		printf("[timestamp.inc] - Error occured while executing ['TimestampFromDate(year=%i,month=%i,date=%i,hour=%i,minute=%i,second=%i)'], you'll have to increase MAX_TIMESTAMPS(current=%i) limit to create more Timestamp instances.",
			year, month, date, hour, minute, second, MAX_TIMESTAMPS);

		return INVALID_TIMESTAMP_ID;
	}

	if (year < 1970) {
		printf("[timestamp.inc] - Error occured while executing ['TimestampFromDate(year=%i,month=%i,date=%i,hour=%i,minute=%i,second=%i)'], in parameter \"year\": cannot be less than 1970.",
			year, month, date, hour, minute, second);

		return INVALID_TIMESTAMP_ID;
 	}

	if (month < 1 || month > 12) {
		printf("[timestamp.inc] - Error occured while executing ['TimestampFromDate(year=%i,month=%i,date=%i,hour=%i,minute=%i,second=%i)'], in parameter \"month\": must be between 1-12.",
			year, month, date, hour, minute, second);

		return INVALID_TIMESTAMP_ID;
 	}

	if (date < 1 || date > TimestampStatic.GetMonthDays(month, year)) {
		printf("[timestamp.inc] - Error occured while executing ['TimestampFromDate(year=%i,month=%i,date=%i,hour=%i,minute=%i,second=%i)'], in parameter \"date\": must be between 1-%i (month=%s).",
			year, month, date, hour, minute, second, TimestampStatic.GetMonthDays(month, year), MONTH_NAMES[month - 1]);

		return INVALID_TIMESTAMP_ID;
 	}

	if (hour < 0 || hour > 24) {
		printf("[timestamp.inc] - Error occured while executing ['TimestampFromDate(year=%i,month=%i,date=%i,hour=%i,minute=%i,second=%i)'], in parameter \"hour\": must be between 1-24.",
			year, month, date, hour, minute, second);

		return INVALID_TIMESTAMP_ID;
 	}

	if (minute < 0 || minute > 59) {
		printf("[timestamp.inc] - Error occured while executing ['TimestampFromDate(year=%i,month=%i,date=%i,hour=%i,minute=%i,second=%i)'], in parameter \"minute\": must be between 1-59.",
			year, month, date, hour, minute, second);

		return INVALID_TIMESTAMP_ID;
 	}

	if (second < 0 || second > 59) {
		printf("[timestamp.inc] - Error occured while executing ['TimestampFromDate(year=%i,month=%i,date=%i,hour=%i,minute=%i,second=%i)'], in parameter \"second\": must be between 1-59.",
			year, month, date, hour, minute, second);

		return INVALID_TIMESTAMP_ID;
	}

	timestampData[id][TIMESTAMP_VALID] = true;
	timestampData[id][TIMESTAMP_YEAR] = year;
	timestampData[id][TIMESTAMP_MONTH] = month;
	timestampData[id][TIMESTAMP_DATE] = date;
	timestampData[id][TIMESTAMP_HOUR] = hour;
	timestampData[id][TIMESTAMP_MINUTE] = minute;
	timestampData[id][TIMESTAMP_SECOND] = second;
	timestampData[id][TIMESTAMP_VALUE] = TimestampStatic.DateToTime(year, month, date, hour, minute, second);

	return id;
}

stock Timestamp:TimestampFromTime(timestamp) {
	new Timestamp:id = INVALID_TIMESTAMP_ID;

	for (new Timestamp:i = Timestamp:0; i < Timestamp:MAX_TIMESTAMPS; i++) {
		if (timestampData[i][TIMESTAMP_VALID] == false) {
			id = i;
			break;
		}
	}

	if (id == INVALID_TIMESTAMP_ID) {
		printf("[timestamp.inc] - Error occured while executing ['TimestampFromTime(seconds1970=%i)'], you'll have to increase MAX_TIMESTAMPS(current=%i) limit to create more Timestamp instances.",
			timestamp, MAX_TIMESTAMPS);

		return INVALID_TIMESTAMP_ID;
	}

	TimestampStatic.TimeToDate(
		timestamp,
		timestampData[id][TIMESTAMP_YEAR], timestampData[id][TIMESTAMP_MONTH], timestampData[id][TIMESTAMP_DATE],
		timestampData[id][TIMESTAMP_HOUR], timestampData[id][TIMESTAMP_MINUTE], timestampData[id][TIMESTAMP_SECOND]);

	timestampData[id][TIMESTAMP_VALID] = true;
	timestampData[id][TIMESTAMP_VALUE] = timestamp;

	return id;
}

stock Timestamp:Timestamp() {
	new year, month, date, hour, minute, second;
	getdate(year, month, date);
	gettime(hour, minute, second);
	
	return TimestampFromDate(year, month, date, hour, minute, second);
}

stock bool:Timestamp.IsValid(Timestamp:object) {
	return (object < Timestamp:0 || object >= Timestamp:MAX_TIMESTAMPS) ? false : timestampData[object][TIMESTAMP_VALID];
}

stock Timestamp.SetYear(Timestamp:object, year) {
	if (year < 1970) {
	    printf("[timestamp.inc] - Error occured while executing ['Timestamp.SetYear(object=%i,year=%i)'], in parameter \"year\": cannot be less than 1970.",
			_:object, year);

		return;
	}

	timestampData[object][TIMESTAMP_YEAR] = year;

	timestampData[object][TIMESTAMP_VALUE] = TimestampStatic.DateToTime(year, month, date, hour, minute, second);
}

stock Timestamp.GetYear(Timestamp:object) {
	return timestampData[object][TIMESTAMP_YEAR];
}

stock Timestamp.SetMonth(Timestamp:object, month) {
	if (month < 1 || month > 12) {
	    printf("[timestamp.inc] - Error occured while executing ['Timestamp.SetMonth(object=%i,month=%i)'], in parameter \"month\": must be between 1-12.",
			_:object, month);

		return;
	}

	timestampData[object][TIMESTAMP_MONTH] = month - 1;

	timestampData[object][TIMESTAMP_VALUE] =
		TimestampStatic.DateToTime(timestampData[object][TIMESTAMP_YEAR], timestampData[object][TIMESTAMP_MONTH], timestampData[object][TIMESTAMP_DATE],
									timestampData[object][TIMESTAMP_HOUR], timestampData[object][TIMESTAMP_MINUTE], timestampData[object][TIMESTAMP_SECOND]);
}

stock Timestamp.GetMonth(Timestamp:object) {
	return timestampData[object][TIMESTAMP_MONTH] + 1;
}

stock Timestamp.SetDate(Timestamp:object, date) {
	if (date < 1 || date > MONTH_DAYS[timestampData[object][TIMESTAMP_MONTH]]) {
	    printf("[timestamp.inc] - Error occured while executing ['Timestamp.SetDate(object=%i,date=%i)'], in parameter \"date\": must be between 1-%i (month=%s).",
			_:object, date, MONTH_DAYS[timestampData[object][TIMESTAMP_MONTH]], MONTH_NAMES[timestampData[object][TIMESTAMP_MONTH]]);

		return;
	}

	timestampData[object][TIMESTAMP_DATE] = date;

	timestampData[object][TIMESTAMP_VALUE] =
		TimestampStatic.DateToTime(timestampData[object][TIMESTAMP_YEAR], timestampData[object][TIMESTAMP_MONTH], timestampData[object][TIMESTAMP_DATE],
									timestampData[object][TIMESTAMP_HOUR], timestampData[object][TIMESTAMP_MINUTE], timestampData[object][TIMESTAMP_SECOND]);
}

stock Timestamp.GetDate(Timestamp:object) {
	return timestampData[object][TIMESTAMP_DATE];
}

stock Timestamp.SetHour(Timestamp:object, hour) {
	if (hour < 0 || hour > 24) {
	    printf("[timestamp.inc] - Error occured while executing ['Timestamp.SetHour(object=%i,hour=%i)'], in parameter \"hour\": must be between 1-24.",
			_:object, hour);

		return;
	}

	timestampData[object][TIMESTAMP_HOUR] = hour;

	timestampData[object][TIMESTAMP_VALUE] =
		TimestampStatic.DateToTime(timestampData[object][TIMESTAMP_YEAR], timestampData[object][TIMESTAMP_MONTH], timestampData[object][TIMESTAMP_DATE],
									timestampData[object][TIMESTAMP_HOUR], timestampData[object][TIMESTAMP_MINUTE], timestampData[object][TIMESTAMP_SECOND]);
}

stock Timestamp.GetHour(Timestamp:object) {
	return timestampData[object][TIMESTAMP_HOUR];
}

stock Timestamp.SetMinute(Timestamp:object, minute) {
	if (minute < 0 || minute > 59) {
	    printf("[timestamp.inc] - Error occured while executing ['Timestamp.SetMinute(object=%i,minute=%i)'], in parameter \"minute\": must be between 1-59.",
			_:object, minute);

		return;
	}

	timestampData[object][TIMESTAMP_MINUTE] = minute;

	timestampData[object][TIMESTAMP_VALUE] =
		TimestampStatic.DateToTime(timestampData[object][TIMESTAMP_YEAR], timestampData[object][TIMESTAMP_MONTH], timestampData[object][TIMESTAMP_DATE],
									timestampData[object][TIMESTAMP_HOUR], timestampData[object][TIMESTAMP_MINUTE], timestampData[object][TIMESTAMP_SECOND]);
}

stock Timestamp.GetMinute(Timestamp:object) {
	return timestampData[object][TIMESTAMP_MINUTE];
}

stock Timestamp.SetSecond(Timestamp:object, second) {
	if (second < 0 || second > 59) {
	    printf("[timestamp.inc] - Error occured while executing ['Timestamp.SetMinute(object=%i,second=%i)'], in parameter \"second\": must be between 1-59.",
			_:object, second);

		return;
	}

	timestampData[object][TIMESTAMP_SECOND] = second;

	timestampData[object][TIMESTAMP_VALUE] =
		TimestampStatic.DateToTime(timestampData[object][TIMESTAMP_YEAR], timestampData[object][TIMESTAMP_MONTH], timestampData[object][TIMESTAMP_DATE],
									timestampData[object][TIMESTAMP_HOUR], timestampData[object][TIMESTAMP_MINUTE], timestampData[object][TIMESTAMP_SECOND]);
}

stock Timestamp.GetSecond(Timestamp:object) {
	return timestampData[object][TIMESTAMP_SECOND];
}

stock Timestamp.Value(Timestamp:object) {
	return timestampData[object][TIMESTAMP_VALUE];
}

stock bool:Timestamp.IsGreater(Timestamp:object, Timestamp:otherObject) {
	return (timestampData[object][TIMESTAMP_VALUE] > timestampData[otherObject][TIMESTAMP_VALUE]);
}

stock bool:Timestamp.IsEqual(Timestamp:object, Timestamp:otherObject) {
	return (timestampData[object][TIMESTAMP_VALUE] == timestampData[otherObject][TIMESTAMP_VALUE]);
}

stock Timestamp.AddTime(Timestamp:object, year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0) {
	timestampData[object][TIMESTAMP_VALUE] += TimestampStatic.ConvertToSeconds(YEAR, year)
	    + TimestampStatic.ConvertToSeconds(MONTH, month)
	    + TimestampStatic.ConvertToSeconds(DAY, day)
	    + TimestampStatic.ConvertToSeconds(HOUR, hour)
	    + TimestampStatic.ConvertToSeconds(MINUTE, minute)
	    + second;

	TimestampStatic.TimeToDate(
		timestampData[object][TIMESTAMP_VALUE],
		timestampData[object][TIMESTAMP_YEAR], timestampData[object][TIMESTAMP_MONTH], timestampData[object][TIMESTAMP_DATE],
		timestampData[object][TIMESTAMP_HOUR], timestampData[object][TIMESTAMP_MINUTE], timestampData[object][TIMESTAMP_SECOND]);

}

stock Timestamp.SubstractTime(Timestamp:object, year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0) {
	timestampData[object][TIMESTAMP_VALUE] -= TimestampStatic.ConvertToSeconds(YEAR, year)
	    + TimestampStatic.ConvertToSeconds(MONTH, month)
	    + TimestampStatic.ConvertToSeconds(DAY, day)
	    + TimestampStatic.ConvertToSeconds(HOUR, hour)
	    + TimestampStatic.ConvertToSeconds(MINUTE, minute)
	    + second;

	TimestampStatic.TimeToDate(
		timestampData[object][TIMESTAMP_VALUE],
		timestampData[object][TIMESTAMP_YEAR], timestampData[object][TIMESTAMP_MONTH], timestampData[object][TIMESTAMP_DATE],
		timestampData[object][TIMESTAMP_HOUR], timestampData[object][TIMESTAMP_MINUTE], timestampData[object][TIMESTAMP_SECOND]);
}

stock Timestamp.ConvertToUnit(Timestamp:object, E_TIMEUNIT_TYPES:type) {
	return TimestampStatic.ConvertFromSeconds(type, timestampData[object][TIMESTAMP_VALUE]);
}

stock Timestamp.ToString(Timestamp:object) {
	new ret[MAX_TIMESTAMP_STRING];
	format(ret, MAX_TIMESTAMP_STRING, "%02i-%02i-%02i %02i:%02i:%02i",
 		timestampData[object][TIMESTAMP_YEAR], timestampData[object][TIMESTAMP_MONTH], timestampData[object][TIMESTAMP_DATE],
   		timestampData[object][TIMESTAMP_HOUR], timestampData[object][TIMESTAMP_MINUTE], timestampData[object][TIMESTAMP_SECOND]);
	return ret;
}

stock Timestamp.Destroy(Timestamp:object) {
	timestampData[object][TIMESTAMP_VALID] = false;
}
