#include <a_samp>
#include <zcmd>
#include <sscanf2>
#include <easydialog>
#include <kickban>

#define COLOR_WHITE (0xFFFFFFFF)
#define COL_WHITE "{FFFFFF}"

#define COLOR_TOMATO (0xFF6347FF)
#define COL_TOMATO "{FF6347}"

#define COLOR_YELLOW (0xFFDD00FF)
#define COL_YELLOW "{FFDD00}"

#define COLOR_GREEN (0x00FF00FF)
#define COL_GREEN "{00FF00}"

#define COLOR_DEFAULT (0xA9C4E4FF)
#define COL_DEFAULT "{A9C4E4}"

#define MAX_LOGIN_ATTEMPTS 3
#define MAX_ACCOUNT_LOCKTIME 2 // mins

#define MIN_PASSWORD_LENGTH 4
#define MAX_PASSWORD_LENGTH 45

//#define SECURE_PASSWORD_ONLY // this forces the user to have atleast 1 Lowercase, 1 Highercase and 1 Number in their password

#define MAX_SECURITY_QUESTION_SIZE 128

new DB:db;

new const SECURITY_QUESTIONS[][MAX_SECURITY_QUESTION_SIZE] =
{
	"What was your childhood nickname?",
	"What is the name of your favorite childhood friend?",
	"In what city or town did your mother and father meet?",
	"What is the middle name of your oldest child?",
	"What is your favorite team?",
	"What is your favorite movie?",
	"What is the first name of the boy or girl that you first kissed?",
	"What was the make and model of your first car?",
	"What was the name of the hospital where you were born?",
	"Who is your childhood sports hero?",
	"In what town was your first job?",
	"What was the name of the company where you had your first job?",
	"What school did you attend for sixth grade?",
	"What was the last name of your third grade teacher?"
};

enum e_USER
{
	e_USER_SQLID,
	e_USER_PASSWORD[64 + 1],
	e_USER_SALT[64 + 1],
	e_USER_KILLS,
	e_USER_DEATHS,
	e_USER_SCORE,
	e_USER_MONEY,
	e_USER_ADMIN_LEVEL,
	e_USER_VIP_LEVEL,
	e_USER_REGISTER_TIMESTAMP,
	e_USER_LASTLOGIN_TIMESTAMP,
	e_USER_SECURITY_QUESTION,
	e_USER_SECURITY_ANSWER[64 + 1]
};
new eUser[MAX_PLAYERS][e_USER];
new iLoginAttempts[MAX_PLAYERS];
new iAnswerAttempts[MAX_PLAYERS];

IpToLong(const ip[])
{
  	new len = strlen(ip);
	if (!(len > 0 && len < 17))
		return 0;

	new count,
		pos,
		dest[3],
		val[4];
	for (new i; i < len; i++)
	{
		if (ip[i] == '.' || i == len)
		{
			strmid(dest, ip, pos, i);
			pos = (i + 1);

		    val[count] = strval(dest);
		    if (!(1 <= val[count] <= 255))
		    	return 0;

			count++;
			if (count > 3)
				return 0;
		}
	}

	if (count != 3)
		return 0;
	return ((val[0] * 16777216) + (val[1] * 65536) + (val[2] * 256) + (val[3]));
}

ReturnTimelapse(start, till)
{
    new ret[32];
	new seconds = till - start;

	const
		MINUTE = 60,
		HOUR = 60 * MINUTE,
		DAY = 24 * HOUR,
		MONTH = 30 * DAY;

	if (seconds == 1)
		format(ret, sizeof(ret), "a second");
	if (seconds < (1 * MINUTE))
		format(ret, sizeof(ret), "%i seconds", seconds);
	else if (seconds < (2 * MINUTE))
		format(ret, sizeof(ret), "a minute");
	else if (seconds < (45 * MINUTE))
		format(ret, sizeof(ret), "%i minutes", (seconds / MINUTE));
	else if (seconds < (90 * MINUTE))
		format(ret, sizeof(ret), "an hour");
	else if (seconds < (24 * HOUR))
		format(ret, sizeof(ret), "%i hours", (seconds / HOUR));
	else if (seconds < (48 * HOUR))
		format(ret, sizeof(ret), "a day");
	else if (seconds < (30 * DAY))
		format(ret, sizeof(ret), "%i days", (seconds / DAY));
	else if (seconds < (12 * MONTH))
    {
		new months = floatround(seconds / DAY / 30);
      	if (months <= 1)
			format(ret, sizeof(ret), "a month");
      	else
			format(ret, sizeof(ret), "%i months", months);
	}
    else
    {
      	new years = floatround(seconds / DAY / 365);
      	if (years <= 1)
			format(ret, sizeof(ret), "a year");
      	else
			format(ret, sizeof(ret), "%i years", years);
	}
	return ret;
}

main()
{
}

public OnGameModeInit()
{
	db_debug_openresults();
    db = db_open("database.db");
	db_query(db, "PRAGMA synchronous = NORMAL");
 	db_query(db, "PRAGMA journal_mode = WAL");

	new string[1024];
	string = "CREATE TABLE IF NOT EXISTS `users`(\
		`id` INTEGER PRIMARY KEY, \
		`name` VARCHAR(24) NOT NULL DEFAULT '', \
		`ip` VARCHAR(18) NOT NULL DEFAULT '', \
		`longip` INT NOT NULL DEFAULT '0', \
		`password` VARCHAR(64) NOT NULL DEFAULT '', \
		`salt` VARCHAR(64) NOT NULL DEFAULT '', \
		`sec_question` INT NOT NULL DEFAULT '0', \
		`sec_answer` VARCHAR(64) NOT NULL DEFAULT '', ";
	strcat(string, "`register_timestamp` INT NOT NULL DEFAULT '0', \
		`lastlogin_timestamp` INT NOT NULL DEFAULT '0', \
		`kills` INT NOT NULL DEFAULT '0', \
		`deaths` INT NOT NULL DEFAULT '0', \
		`score` INT NOT NULL DEFAULT '0', \
		`money` INT NOT NULL DEFAULT '0', \
		`adminlevel` INT NOT NULL DEFAULT '0', \
		`viplevel` INT NOT NULL DEFAULT '0')");
	db_query(db, string);
	
	db_query(db, "CREATE TABLE IF NOT EXISTS `temp_blocked_users` (\
		`ip` VARCHAR(18) NOT NULL DEFAULT '', \
		`lock_timestamp` INT NOT NULL DEFAULT '0', \
		`user_id` INT NOT NULL DEFAULT '-1')");

    EnableVehicleFriendlyFire();
    DisableInteriorEnterExits();
	UsePlayerPedAnims();
	return 1;
}

public OnGameModeExit()
{
	db_close(db);
	return 1;
}

public OnPlayerConnect(playerid)
{
	return SetTimerEx("OnPlayerJoin", 0001, false, "i", playerid);
}

public OnPlayerDisconnect(playerid, reason)
{
	new string[1024],
		name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
    format(string, sizeof(string), "UPDATE `users` SET `name` = '%s', `password` = '%q', `salt` = '%q', `sec_question` = %i, `sec_answer` = '%q', `kills` = %i, `deaths` = %i, `score` = %i, `money` = %i, `adminlevel` = %i, `viplevel` = %i WHERE `id` = %i",
		name, eUser[playerid][e_USER_PASSWORD], eUser[playerid][e_USER_SALT], eUser[playerid][e_USER_SECURITY_QUESTION], eUser[playerid][e_USER_SECURITY_ANSWER],  eUser[playerid][e_USER_KILLS], eUser[playerid][e_USER_DEATHS], GetPlayerScore(playerid), GetPlayerMoney(playerid), eUser[playerid][e_USER_ADMIN_LEVEL], eUser[playerid][e_USER_VIP_LEVEL], eUser[playerid][e_USER_SQLID]);
	db_query(db, string);
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	if (!GetPVarInt(playerid, "LoggedIn"))
	{
	    SetPlayerCameraPos(playerid, -144.2838, 1244.2357, 35.6595);
		SetPlayerCameraLookAt(playerid, -144.2255, 1243.2335, 35.3393, CAMERA_MOVE);
	}
	else
	{
    	SetPlayerPos(playerid, -314.7314, 1052.8170, 20.3403);
     	SetPlayerFacingAngle(playerid, 357.8575);
     	SetPlayerCameraPos(playerid, -312.2127, 1055.5232, 20.5785);
		SetPlayerCameraLookAt(playerid, -313.0236, 1054.9427, 20.5334, CAMERA_MOVE);
	}
 	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	if (!GetPVarInt(playerid, "LoggedIn"))
	{
	    GameTextForPlayer(playerid, "~n~~n~~n~~n~~r~Login/Register first before spawning!", 3000, 3);
	    return 0;
	}
	return 1;
}

public OnPlayerSpawn(playerid)
{
    SetPlayerPos(playerid, -314.7314, 1052.8170, 20.3403);
   	SetPlayerFacingAngle(playerid, 357.8575);
	return 1;
}

forward OnPlayerJoin(playerid);
public OnPlayerJoin(playerid)
{
	for (new i; i < 100; i++)
	{
	    SendClientMessage(playerid, COLOR_WHITE, "");
	}
	SendClientMessage(playerid, COLOR_YELLOW, "You have connected to \"SA-MP 0.3.7 Server\"");
	PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);

	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);

	new string[150];
	format(string, sizeof(string), "SELECT * FROM `users` WHERE `name` = '%q' LIMIT 1", name);
	new DBResult:result = db_query(db, string);
	if (db_num_rows(result) == 0)
	{
	    eUser[playerid][e_USER_SQLID] = -1;
		Dialog_Show(playerid, REGISTER, DIALOG_STYLE_PASSWORD, "Account Registeration... [Step: 1/3]", COL_WHITE "Welcome to our server. We will take you through "COL_GREEN"3 simple steps "COL_WHITE"to register your account with a backup option in case you forgot your password!\nPlease enter a password, "COL_TOMATO"case sensitivity"COL_WHITE" is on.", "Continue", "Options");
		SendClientMessage(playerid, COLOR_WHITE, "[Step: 1/3] Enter your new account's password.");
	}
	else
	{
		eUser[playerid][e_USER_SQLID] = db_get_field_assoc_int(result, "id");
		
		format(string, sizeof(string), "SELECT `lock_timestamp` FROM `temp_blocked_users` WHERE `user_id` = %i LIMIT 1", eUser[playerid][e_USER_SQLID]);
		new DBResult:lock_result = db_query(db, string);
		if (db_num_rows(lock_result) == 1)
		{
			new lock_timestamp = db_get_field_int(lock_result, 0);
			if ((gettime() - lock_timestamp) < 0)
		    {
		        SendClientMessage(playerid, COLOR_TOMATO, "Sorry! The account is temporarily locked on your IP. due to "#MAX_LOGIN_ATTEMPTS"/"#MAX_LOGIN_ATTEMPTS" failed login attempts.");
		        format(string, sizeof(string), "You'll be able to try again in %s.", ReturnTimelapse(gettime(), lock_timestamp));
				SendClientMessage(playerid, COLOR_TOMATO, string);
				db_free_result(result);
				db_free_result(lock_result);
				return Kick(playerid);
		    }
		    else
		    {
		        new ip[18];
				GetPlayerIp(playerid, ip, 18);
		        format(string, sizeof(string), "DELETE FROM `temp_blocked_users` WHERE `user_id` = %i AND `ip` = '%s'", eUser[playerid][e_USER_SQLID], ip);
		        db_query(db, string);
		    }
		}
		db_free_result(lock_result);
		
		db_get_field_assoc(result, "password", eUser[playerid][e_USER_PASSWORD], 64);
		db_get_field_assoc(result, "salt", eUser[playerid][e_USER_SALT], 64);
		eUser[playerid][e_USER_SALT][0] = EOS;
		eUser[playerid][e_USER_KILLS] = db_get_field_assoc_int(result, "kills");
		eUser[playerid][e_USER_DEATHS] = db_get_field_assoc_int(result, "deaths");
		eUser[playerid][e_USER_SCORE] = db_get_field_assoc_int(result, "score");
		eUser[playerid][e_USER_MONEY] = db_get_field_assoc_int(result, "money");
		eUser[playerid][e_USER_ADMIN_LEVEL] = db_get_field_assoc_int(result, "adminlevel");
		eUser[playerid][e_USER_VIP_LEVEL] = db_get_field_assoc_int(result, "viplevel");
		eUser[playerid][e_USER_REGISTER_TIMESTAMP] = db_get_field_assoc_int(result, "register_timestamp");
		eUser[playerid][e_USER_LASTLOGIN_TIMESTAMP] = db_get_field_assoc_int(result, "lastlogin_timestamp");
		eUser[playerid][e_USER_SECURITY_QUESTION] = db_get_field_assoc_int(result, "sec_question");
		db_get_field_assoc(result, "sec_answer", eUser[playerid][e_USER_SECURITY_ANSWER], MAX_PASSWORD_LENGTH * 2);

		Dialog_Show(playerid, LOGIN, DIALOG_STYLE_PASSWORD, "Account Login...", COL_WHITE "Insert your secret password to access this account. If you failed in "COL_YELLOW""#MAX_LOGIN_ATTEMPTS" "COL_WHITE"attempts, account will be locked for "COL_YELLOW""#MAX_ACCOUNT_LOCKTIME" "COL_WHITE"minutes.", "Continue", "Options");
	}

	db_free_result(result);
	return 1;
}

Dialog:LOGIN(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
	    Dialog_Show(playerid, OPTIONS, DIALOG_STYLE_LIST, "Account Options...", "Forgot password\nForgot username\nExit to desktop", "Select", "Back");
	    return 1;
	}

	new string[256];

	new hash[64];
	SHA256_PassHash(inputtext, eUser[playerid][e_USER_SALT], hash, sizeof(hash));
	if (strcmp(hash, eUser[playerid][e_USER_PASSWORD]))
	{
		if (++iLoginAttempts[playerid] == MAX_LOGIN_ATTEMPTS)
		{
		    new lock_timestamp = gettime() + (MAX_ACCOUNT_LOCKTIME * 60);
		    new ip[18];
		    GetPlayerIp(playerid, ip, 18);
			format(string, sizeof(string), "INSERT INTO `temp_blocked_users` VALUES('%s', %i, %i)", ip, lock_timestamp, eUser[playerid][e_USER_SQLID]);
			db_query(db, string);

		    SendClientMessage(playerid, COLOR_TOMATO, "Sorry! The account has been temporarily locked on your IP. due to "#MAX_LOGIN_ATTEMPTS"/"#MAX_LOGIN_ATTEMPTS" failed login attempts.");
		    format(string, sizeof(string), "If you forgot your password/username, click on 'Options' in login window next time (you may retry in %s).", ReturnTimelapse(gettime(), lock_timestamp));
			SendClientMessage(playerid, COLOR_TOMATO, string);
		    return Kick(playerid);
		}

	    Dialog_Show(playerid, LOGIN, DIALOG_STYLE_INPUT, "Account Login...", COL_WHITE "Insert your secret password to access this account. If you failed in "COL_YELLOW""#MAX_LOGIN_ATTEMPTS" "COL_WHITE"attempts, account will be locked for "COL_YELLOW""#MAX_ACCOUNT_LOCKTIME" "COL_WHITE"minutes.", "Continue", "Options");
	    format(string, sizeof(string), "Incorrect password! Your login tries left: %i/"#MAX_LOGIN_ATTEMPTS" attempts.", iLoginAttempts[playerid]);
		SendClientMessage(playerid, COLOR_TOMATO, string);
	    return 1;
	}

	new name[MAX_PLAYER_NAME],
		ip[18];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	GetPlayerIp(playerid, ip, 18);
	format(string, sizeof(string), "UPDATE `users` SET `lastlogin_timestamp` = %i, `ip` = '%s', `longip` = %i WHERE `id` = %i", gettime(), ip, IpToLong(ip), eUser[playerid][e_USER_SQLID]);
	db_query(db, string);

	format(string, sizeof(string), "Successfully logged in! Welcome back to our server %s, we hope you enjoy your stay. [Last login: %s ago]", name, ReturnTimelapse(eUser[playerid][e_USER_LASTLOGIN_TIMESTAMP], gettime()));
	SendClientMessage(playerid, COLOR_GREEN, string);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	SetPVarInt(playerid, "LoggedIn", 1);
	OnPlayerRequestClass(playerid, 0);
	return 1;
}

Dialog:REGISTER(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
	    Dialog_Show(playerid, OPTIONS, DIALOG_STYLE_LIST, "Account Options...", "Forgot password\nForgot username\nExit to desktop", "Select", "Back");
	    return 1;
	}

	if (!(MIN_PASSWORD_LENGTH <= strlen(inputtext) <= MAX_PASSWORD_LENGTH))
	{
	    Dialog_Show(playerid, REGISTER, DIALOG_STYLE_PASSWORD, "Account Registeration... [Step: 1/3]", COL_WHITE "Welcome to our server. We will take you through "COL_GREEN"3 simple steps "COL_WHITE"to register your account with a backup option in case you forgot your password!\nPlease enter a password, "COL_TOMATO"case sensitivity"COL_WHITE" is on.", "Continue", "Options");
		SendClientMessage(playerid, COLOR_TOMATO, "Invalid password length, must be between "#MIN_PASSWORD_LENGTH" - "#MAX_PASSWORD_LENGTH" characters.");
	    return 1;
	}

	#if defined SECURE_PASSWORD_ONLY
		new bool:contain_number,
		    bool:contain_highercase,
		    bool:contain_lowercase;
		for (new i, j = strlen(inputtext); i < j; i++)
		{
		    switch (inputtext[i])
		    {
		        case '0'..'9':
		            contain_number = true;
				case 'A'..'Z':
				    contain_highercase = true;
				case 'a'..'z':
				    contain_lowercase = true;
		    }

		    if (contain_number && contain_highercase && contain_lowercase)
		        break;
		}

		if (!contain_number || !contain_highercase || !contain_lowercase)
		{
		    Dialog_Show(playerid, REGISTER, DIALOG_STYLE_INPUT, "Account Registeration... [Step: 1/3]", COL_WHITE "Welcome to our server. We will take you through "COL_GREEN"3 simple steps "COL_WHITE"to register your account with a backup option in case you forgot your password!\nPlease enter a password, "COL_TOMATO"case sensitivity"COL_WHITE" is on.", "Continue", "Options");
			SendClientMessage(playerid, COLOR_TOMATO, "Password must contain atleast a Highercase, a Lowercase and a Number.");
		    return 1;
		}
	#endif

	for (new i; i < 64; i++)
	{
		eUser[playerid][e_USER_SALT][i] = (random('z' - 'A') + 'A');
	}
	eUser[playerid][e_USER_SALT][64] = EOS;
	SHA256_PassHash(inputtext, eUser[playerid][e_USER_SALT], eUser[playerid][e_USER_PASSWORD], 64);

	new list[2 + (sizeof(SECURITY_QUESTIONS) * MAX_SECURITY_QUESTION_SIZE)];
	for (new i; i < sizeof(SECURITY_QUESTIONS); i++)
	{
	    strcat(list, SECURITY_QUESTIONS[i]);
	    strcat(list, "\n");
	}
	Dialog_Show(playerid, SEC_QUESTION, DIALOG_STYLE_LIST, "Account Registeration... [Step: 2/3]", list, "Continue", "Back");
	SendClientMessage(playerid, COLOR_WHITE, "[Step: 2/3] Select a security question. This will help you retrieve your password in case you forget it any time soon!");
	PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:SEC_QUESTION(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
	    Dialog_Show(playerid, REGISTER, DIALOG_STYLE_PASSWORD, "Account Registeration... [Step: 1/3]", COL_WHITE "Welcome to our server. We will take you through "COL_GREEN"3 simple steps "COL_WHITE"to register your account with a backup option in case you forgot your password!\nPlease enter a password, "COL_TOMATO"case sensitivity"COL_WHITE" is on.", "Continue", "Options");
		SendClientMessage(playerid, COLOR_WHITE, "[Step: 1/3] Enter your new account's password.");
		return 1;
	}

	eUser[playerid][e_USER_SECURITY_QUESTION] = listitem;

	new string[256];
	format(string, sizeof(string), COL_TOMATO "%s\n"COL_WHITE"Insert your answer below in the box. (don't worry about CAPS, answers are NOT case sensitive).", SECURITY_QUESTIONS[listitem]);
	Dialog_Show(playerid, SEC_ANSWER, DIALOG_STYLE_INPUT, "Account Registeration... [Step: 3/3]", string, "Confirm", "Back");
	SendClientMessage(playerid, COLOR_WHITE, "[Step: 3/3] Write the answer to your secuirty question and you'll be done :)");
	PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:SEC_ANSWER(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
	    new list[2 + (sizeof(SECURITY_QUESTIONS) * MAX_SECURITY_QUESTION_SIZE)];
		for (new i; i < sizeof(SECURITY_QUESTIONS); i++)
		{
		    strcat(list, SECURITY_QUESTIONS[i]);
		    strcat(list, "\n");
		}
		Dialog_Show(playerid, SEC_QUESTION, DIALOG_STYLE_LIST, "Account Registeration... [Step: 2/3]", list, "Continue", "Back");
		SendClientMessage(playerid, COLOR_WHITE, "[Step: 2/3] Select a security question. This will help you retrieve your password in case you forget it any time soon!");
		return 1;
	}

	new string[512];

	if (strlen(inputtext) < MIN_PASSWORD_LENGTH || inputtext[0] == ' ')
	{
	    format(string, sizeof(string), COL_TOMATO "%s\n"COL_WHITE"Insert your answer below in the box. (don't worry about CAPS, answers are NOT case sensitive).", SECURITY_QUESTIONS[eUser[playerid][e_USER_SECURITY_QUESTION]]);
		Dialog_Show(playerid, SEC_ANSWER, DIALOG_STYLE_INPUT, "Account Registeration... [Step: 3/3]", string, "Confirm", "Back");
		SendClientMessage(playerid, COLOR_TOMATO, "Security answer cannot be an less than "#MIN_PASSWORD_LENGTH" characters.");
		return 1;
	}

	for (new i, j = strlen(inputtext); i < j; i++)
	{
        inputtext[i] |= 0x20;
	}
	SHA256_PassHash(inputtext, eUser[playerid][e_USER_SALT], eUser[playerid][e_USER_SECURITY_ANSWER], 64);

	new name[MAX_PLAYER_NAME],
		ip[18];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	GetPlayerIp(playerid, ip, 18);
	format(string, sizeof(string), "INSERT INTO `users`(`name`, `ip`, `longip`, `password`, `salt`, `sec_question`, `sec_answer`, `register_timestamp`, `lastlogin_timestamp`) VALUES('%s', '%s', %i, '%q', '%q', %i, '%q', %i, %i)", name, ip, IpToLong(ip), eUser[playerid][e_USER_PASSWORD], eUser[playerid][e_USER_SALT], eUser[playerid][e_USER_SECURITY_QUESTION], eUser[playerid][e_USER_SECURITY_ANSWER], gettime(), gettime());
	db_query(db, string);

	format(string, sizeof(string), "SELECT `id` FROM `users` WHERE `name` = '%q' LIMIT 1", name);
	new DBResult:result = db_query(db, string);
    eUser[playerid][e_USER_SQLID] = db_get_field_int(result, 0);
	db_free_result(result);

	format(string, sizeof(string), "Successfully registered! Welcome to our server %s, we hope you enjoy your stay. [IP: %s]", name, ip);
	SendClientMessage(playerid, COLOR_GREEN, string);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	SetPVarInt(playerid, "LoggedIn", 1);
	OnPlayerRequestClass(playerid, 0);
	return 1;
}

Dialog:OPTIONS(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
		if (eUser[playerid][e_USER_SQLID] != -1)
			Dialog_Show(playerid, LOGIN, DIALOG_STYLE_PASSWORD, "Account Login...", COL_WHITE "Insert your secret password to access this account. If you failed in "COL_YELLOW""#MAX_LOGIN_ATTEMPTS" "COL_WHITE"attempts, account will be locked for "COL_YELLOW""#MAX_ACCOUNT_LOCKTIME" "COL_WHITE"minutes.", "Continue", "Options");
		else
			Dialog_Show(playerid, REGISTER, DIALOG_STYLE_PASSWORD, "Account Registeration... [Step: 1/3]", COL_WHITE "Welcome to our server. We will take you through "COL_GREEN"3 simple steps "COL_WHITE"to register your account with a backup option in case you forgot your password!\nPlease enter a password, "COL_TOMATO"case sensitivity"COL_WHITE" is on.", "Continue", "Options");
		return 1;
	}

	switch (listitem)
	{
	    case 0:
	    {
	        if (eUser[playerid][e_USER_SQLID] == -1)
	        {
	            SendClientMessage(playerid, COLOR_TOMATO, "This account isn't registered, try 'Forgot Username' or change your name and connect.");
	        	Dialog_Show(playerid, OPTIONS, DIALOG_STYLE_LIST, "Account Options...", "Forgot password\nForgot username\nExit to desktop", "Select", "Back");
	        	return 1;
	        }

			new string[64 + MAX_SECURITY_QUESTION_SIZE];
			format(string, sizeof(string), COL_WHITE "Answer your security question to reset password.\n\n"COL_TOMATO"%s", SECURITY_QUESTIONS[eUser[playerid][e_USER_SECURITY_QUESTION]]);
			Dialog_Show(playerid, FORGOT_PASSWORD, DIALOG_STYLE_INPUT, "Forgot Password:", string, "Next", "Cancel");
	    }
	    case 1:
	    {
	        const MASK = (-1 << (32 - 36));
			new string[256],
				ip[18];
			GetPlayerIp(playerid, ip, 18);
			format(string, sizeof(string), "SELECT `name`, `lastlogin_timestamp` FROM `users` WHERE ((`longip` & %i) = %i) LIMIT 1", MASK, (IpToLong(ip) & MASK));
			new DBResult:result = db_query(db, string);
			if (db_num_rows(result) == 0)
			{
			    SendClientMessage(playerid, COLOR_TOMATO, "There are no accounts realted to this ip, this seems to be your first join!");
		     	Dialog_Show(playerid, OPTIONS, DIALOG_STYLE_LIST, "Account Options...", "Forgot password\nForgot username\nExit to desktop", "Select", "Back");
			    return 1;
			}

			new list[25 * (MAX_PLAYER_NAME + 32)],
				name[MAX_PLAYER_NAME],
				lastlogin_timestamp,
				i,
				j = ((db_num_rows(result) > 10) ? (10) : (db_num_rows(result)));

			do
			{
			    db_get_field_assoc(result, "name", name, MAX_PLAYER_NAME);
				lastlogin_timestamp = db_get_field_assoc_int(result, "lastlogin_timestamp");
			    format(list, sizeof(list), "%s"COL_TOMATO"%s "COL_WHITE"|| Last login: %s ago\n", list, name, ReturnTimelapse(lastlogin_timestamp, gettime()));
			}
			while (db_next_row(result) && i > j);
			db_free_result(result);

			Dialog_Show(playerid, FORGOT_USERNAME, DIALOG_STYLE_LIST, "Your username history...", list, "Ok", "");
			PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
	    }
	    case 2:
	    {
	        return Kick(playerid);
	    }
	}
	return 1;
}

Dialog:FORGOT_PASSWORD(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
	    Dialog_Show(playerid, OPTIONS, DIALOG_STYLE_LIST, "Account Options...", "Forgot password\nForgot username\nExit to desktop", "Select", "Back");
	    return 1;
	}

	new string[256],
		hash[64];
	SHA256_PassHash(inputtext, eUser[playerid][e_USER_SALT], hash, sizeof(hash));
	if (strcmp(hash, eUser[playerid][e_USER_SECURITY_ANSWER]))
	{
		if (++iAnswerAttempts[playerid] == MAX_LOGIN_ATTEMPTS)
		{
		    new lock_timestamp = gettime() + (MAX_ACCOUNT_LOCKTIME * 60);
		    new ip[18];
		    GetPlayerIp(playerid, ip, 18);
            format(string, sizeof(string), "INSERT INTO `temp_blocked_users` VALUES('%s', %i, %i)", ip, lock_timestamp, eUser[playerid][e_USER_SQLID]);
			db_query(db, string);

		    SendClientMessage(playerid, COLOR_TOMATO, "Sorry! The account has been temporarily locked on your IP. due to "#MAX_LOGIN_ATTEMPTS"/"#MAX_LOGIN_ATTEMPTS" failed login attempts.");
		    format(string, sizeof(string), "If you forgot your password/username, click on 'Options' in login window next time (you may retry in %s).", ReturnTimelapse(gettime(), lock_timestamp));
			SendClientMessage(playerid, COLOR_TOMATO, string);
		    return Kick(playerid);
		}

	    format(string, sizeof(string), COL_WHITE "Answer your security question to reset password.\n\n"COL_TOMATO"%s", SECURITY_QUESTIONS[eUser[playerid][e_USER_SECURITY_QUESTION]]);
		Dialog_Show(playerid, FORGOT_PASSWORD, DIALOG_STYLE_INPUT, "Forgot Password:", string, "Next", "Cancel");
		format(string, sizeof(string), "Incorrect answer! Your tries left: %i/"#MAX_LOGIN_ATTEMPTS" attempts.", iAnswerAttempts[playerid]);
		SendClientMessage(playerid, COLOR_TOMATO, string);
	    return 1;
	}

	Dialog_Show(playerid, RESET_PASSWORD, DIALOG_STYLE_PASSWORD, "Reset Password:", COL_WHITE "Insert a new password for your account. Also in case you want to change security question for later, use /ucp.", "Confirm", "");
	SendClientMessage(playerid, COLOR_GREEN, "Successfully answered your security question! You shall now reset your password.");
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:RESET_PASSWORD(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
		Dialog_Show(playerid, RESET_PASSWORD, DIALOG_STYLE_PASSWORD, "Reset Password:", COL_WHITE "Insert a new password for your account. Also in case you want to change security question for later, use /ucp.", "Confirm", "");
		return 1;
	}

	new string[256];

	if (!(MIN_PASSWORD_LENGTH <= strlen(inputtext) <= MAX_PASSWORD_LENGTH))
	{
	    Dialog_Show(playerid, RESET_PASSWORD, DIALOG_STYLE_PASSWORD, "Reset Password:", COL_WHITE "Insert a new password for your account. Also in case you want to change security question for later, use /ucp.", "Confirm", "");
		SendClientMessage(playerid, COLOR_TOMATO, "Invalid password length, must be between "#MIN_PASSWORD_LENGTH" - "#MAX_PASSWORD_LENGTH" characters.");
	    return 1;
	}

	#if defined SECURE_PASSWORD_ONLY
		new bool:contain_number,
		    bool:contain_highercase,
		    bool:contain_lowercase;
		for (new i, j = strlen(inputtext); i < j; i++)
		{
		    switch (inputtext[i])
		    {
		        case '0'..'9':
		            contain_number = true;
				case 'A'..'Z':
				    contain_highercase = true;
				case 'a'..'z':
				    contain_lowercase = true;
		    }

		    if (contain_number && contain_highercase && contain_lowercase)
		        break;
		}

		if (!contain_number || !contain_highercase || !contain_lowercase)
		{
		    Dialog_Show(playerid, RESET_PASSWORD, DIALOG_STYLE_PASSWORD, "Reset Password:", COL_WHITE "Insert a new password for your account. Also in case you want to change security question for later, use /ucp.", "Confirm", "");
			SendClientMessage(playerid, COLOR_TOMATO, "Password must contain atleast a Highercase, a Lowercase and a Number.");
		    return 1;
		}
	#endif

	SHA256_PassHash(inputtext, eUser[playerid][e_USER_SALT], eUser[playerid][e_USER_PASSWORD], 64);

	new name[MAX_PLAYER_NAME],
		ip[18];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	GetPlayerIp(playerid, ip, 18);
	format(string, sizeof(string), "UPDATE `users` SET `password` = '%q', `ip` = '%s', `longip` = %i, `lastlogin_timestamp` = %i WHERE `id` = %i", eUser[playerid][e_USER_PASSWORD], ip, IpToLong(ip), gettime(), eUser[playerid][e_USER_SQLID]);
	db_query(db, string);

	format(string, sizeof(string), "Successfully logged in with new password! Welcome back to our server %s, we hope you enjoy your stay. [Last login: %s ago]", name, ReturnTimelapse(eUser[playerid][e_USER_LASTLOGIN_TIMESTAMP], gettime()));
	SendClientMessage(playerid, COLOR_GREEN, string);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	SetPVarInt(playerid, "LoggedIn", 1);
	OnPlayerRequestClass(playerid, 0);
	return 1;
}

Dialog:FORGOT_USERNAME(playerid, response, listitem, inputtext[])
{
	Dialog_Show(playerid, OPTIONS, DIALOG_STYLE_LIST, "Account Options...", "Forgot password\nForgot username\nExit to desktop", "Select", "Back");
	return 1;
}

CMD:changepass(playerid, params[])
{
	if (eUser[playerid][e_USER_SQLID] != 1)
	{
		SendClientMessage(playerid, COLOR_TOMATO, "Only registered users can use this command.");
		return 1;
	}

    Dialog_Show(playerid, CHANGE_PASSWORD, DIALOG_STYLE_PASSWORD, "Change account password...", COL_WHITE "Insert a new password for your account, Passwords are "COL_TOMATO"case sensitive"COL_WHITE".", "Confirm", "Cancel");
	SendClientMessage(playerid, COLOR_WHITE, "Enter your new password.");
	PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:CHANGE_PASSWORD(playerid, response, listitem, inputtext[])
{
	if (!response)
		return 1;

	if (!(MIN_PASSWORD_LENGTH <= strlen(inputtext) <= MAX_PASSWORD_LENGTH))
	{
	    Dialog_Show(playerid, CHANGE_PASSWORD, DIALOG_STYLE_PASSWORD, "Change account password...", COL_WHITE "Insert a new password for your account, Passwords are "COL_TOMATO"case sensitive"COL_WHITE".", "Confirm", "Cancel");
		SendClientMessage(playerid, COLOR_TOMATO, "Invalid password length, must be between "#MIN_PASSWORD_LENGTH" - "#MAX_PASSWORD_LENGTH" characters.");
	    return 1;
	}

	#if defined SECURE_PASSWORD_ONLY
		new bool:contain_number,
		    bool:contain_highercase,
		    bool:contain_lowercase;
		for (new i, j = strlen(inputtext); i < j; i++)
		{
		    switch (inputtext[i])
		    {
		        case '0'..'9':
		            contain_number = true;
				case 'A'..'Z':
				    contain_highercase = true;
				case 'a'..'z':
				    contain_lowercase = true;
		    }

		    if (contain_number && contain_highercase && contain_lowercase)
		        break;
		}

		if (!contain_number || !contain_highercase || !contain_lowercase)
		{
		    Dialog_Show(playerid, CHANGE_PASSWORD, DIALOG_STYLE_INPUT, "Change account password...", COL_WHITE "Insert a new password for your account, Passwords are "COL_TOMATO"case sensitive"COL_WHITE".", "Confirm", "Cancel");
			SendClientMessage(playerid, COLOR_TOMATO, "Password must contain atleast a Highercase, a Lowercase and a Number.");
		    return 1;
		}
	#endif

	SHA256_PassHash(inputtext, eUser[playerid][e_USER_SALT], eUser[playerid][e_USER_PASSWORD], 64);

	new string[256];
	for (new i, j = strlen(inputtext); i < j; i++)
	{
	    inputtext[i] = '*';
	}
	format(string, sizeof(string), "Successfully changed your password. [P: %s]", inputtext);
	SendClientMessage(playerid, COLOR_GREEN, string);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}

CMD:changeques(playerid, params[])
{
	if (eUser[playerid][e_USER_SQLID] != 1)
	{
		SendClientMessage(playerid, COLOR_TOMATO, "Only registered users can use this command.");
		return 1;
	}

    new list[2 + (sizeof(SECURITY_QUESTIONS) * MAX_SECURITY_QUESTION_SIZE)];
	for (new i; i < sizeof(SECURITY_QUESTIONS); i++)
	{
	    strcat(list, SECURITY_QUESTIONS[i]);
	    strcat(list, "\n");
	}
	Dialog_Show(playerid, CHANGE_SEC_QUESTION, DIALOG_STYLE_LIST, "Change account security question... [Step: 1/2]", list, "Continue", "Cancel");
	SendClientMessage(playerid, COLOR_WHITE, "[Step: 1/2] Select a security question. This will help you retrieve your password in case you forget it any time soon!");
	PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:CHANGE_SEC_QUESTION(playerid, response, listitem, inputext[])
{
	if (!response)
		return 1;

	SetPVarInt(playerid, "Question", listitem);

	new string[256];
	format(string, sizeof(string), COL_TOMATO "%s\n"COL_WHITE"Insert your answer below in the box. (don't worry about CAPS, answers are NOT case sensitive).", SECURITY_QUESTIONS[listitem]);
	Dialog_Show(playerid, CHANGE_SEC_ANSWER, DIALOG_STYLE_INPUT, "Change account security question... [Step: 2/2]", string, "Confirm", "Back");
	SendClientMessage(playerid, COLOR_WHITE, "[Step: 2/2] Write the answer to your secuirty question.");
	PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
	return 1;
}

Dialog:CHANGE_SEC_ANSWER(playerid, response, listitem, inputtext[])
{
	if (!response)
	{
		new list[2 + (sizeof(SECURITY_QUESTIONS) * MAX_SECURITY_QUESTION_SIZE)];
		for (new i; i < sizeof(SECURITY_QUESTIONS); i++)
		{
		    strcat(list, SECURITY_QUESTIONS[i]);
		    strcat(list, "\n");
		}
		Dialog_Show(playerid, CHANGE_SEC_QUESTION, DIALOG_STYLE_LIST, "Change account security question... [Step: 1/2]", list, "Continue", "Cancel");
		SendClientMessage(playerid, COLOR_WHITE, "[Step: 1/2] Select a security question. This will help you retrieve your password in case you forget it any time soon!");
		return 1;
	}

	new string[512];

	if (strlen(inputtext) < MIN_PASSWORD_LENGTH || inputtext[0] == ' ')
	{
	    format(string, sizeof(string), COL_TOMATO "%s\n"COL_WHITE"Insert your answer below in the box. (don't worry about CAPS, answers are NOT case sensitive).", SECURITY_QUESTIONS[listitem]);
		Dialog_Show(playerid, CHANGE_SEC_ANSWER, DIALOG_STYLE_INPUT, "Change account security question... [Step: 2/2]", string, "Confirm", "Back");
		SendClientMessage(playerid, COLOR_TOMATO, "Security answer cannot be an less than "#MIN_PASSWORD_LENGTH" characters.");
		return 1;
	}

	eUser[playerid][e_USER_SECURITY_QUESTION] = GetPVarInt(playerid, "Question");
	DeletePVar(playerid, "Question");

	for (new i, j = strlen(inputtext); i < j; i++)
	{
        inputtext[i] |= 0x20;
	}
	SHA256_PassHash(inputtext, eUser[playerid][e_USER_SALT], eUser[playerid][e_USER_SECURITY_ANSWER], 64);
	format(string, sizeof(string), "Successfully changed your security answer and question [Q: %s].", SECURITY_QUESTIONS[eUser[playerid][e_USER_SECURITY_QUESTION]]);
	SendClientMessage(playerid, COLOR_GREEN, string);
	PlayerPlaySound(playerid, 1057, 0.0, 0.0, 0.0);
	return 1;
}

CMD:stats(playerid, params[])
{
	new targetid;
	if (sscanf(params, "u", targetid))
	{
  		targetid = playerid;
		SendClientMessage(playerid, COLOR_DEFAULT, "Tip: You can also view other players stats by /stats [player]");
	}

	if (!IsPlayerConnected(targetid))
		return SendClientMessage(playerid, COLOR_TOMATO, "The player is no more connected.");

	new name[MAX_PLAYER_NAME];
	GetPlayerName(targetid, name, MAX_PLAYER_NAME);

	new string[150];
	SendClientMessage(playerid, COLOR_GREEN, "_______________________________________________");
	SendClientMessage(playerid, COLOR_GREEN, "");
	format(string, sizeof(string), "%s[%i]'s stats: (AccountId: %i)", name, targetid, eUser[targetid][e_USER_SQLID]);
	SendClientMessage(playerid, COLOR_GREEN, string);

	new Float:ratio = ((eUser[targetid][e_USER_DEATHS] < 0) ? (0.0) : (floatdiv(eUser[targetid][e_USER_KILLS], eUser[targetid][e_USER_DEATHS])));

	static levelname[6][25];
	if (!levelname[0][0])
	{
		levelname[0] = "Player";
		levelname[1] = "Operator";
		levelname[2] = "Moderator";
		levelname[3] = "Administrator";
		levelname[4] = "Manager";
		levelname[5] = "Owner/RCON";
	}

	format(string, sizeof (string), "Score: %i || Money: $%i || Kills: %i || Deaths: %i || Ratio: %0.2f || Admin Level: %i - %s || Vip Level: %i",
		GetPlayerScore(targetid), GetPlayerMoney(targetid), eUser[targetid][e_USER_KILLS], eUser[targetid][e_USER_DEATHS], ratio, eUser[targetid][e_USER_ADMIN_LEVEL], levelname[((eUser[targetid][e_USER_ADMIN_LEVEL] > 5) ? (5) : (eUser[targetid][e_USER_ADMIN_LEVEL]))], eUser[targetid][e_USER_VIP_LEVEL]);
	SendClientMessage(playerid, COLOR_GREEN, string);

	format(string, sizeof (string), "Registeration On: %s || Last Seen: %s",
	 	ReturnTimelapse(eUser[playerid][e_USER_REGISTER_TIMESTAMP], gettime()), ReturnTimelapse(eUser[playerid][e_USER_LASTLOGIN_TIMESTAMP], gettime()));
	SendClientMessage(playerid, COLOR_GREEN, string);

	SendClientMessage(playerid, COLOR_GREEN, "");
	SendClientMessage(playerid, COLOR_GREEN, "_______________________________________________");
	return 1;
}
