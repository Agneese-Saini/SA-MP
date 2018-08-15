# Gammix's SA-MP Package

Includes a full package of FilterScripts/GameModes/Includes written and supported by me(Gammix)!

Please read the [LICENSE.md](https://github.com/Agneese-Saini/SA-MP/blob/master/LICENSE.md) before re-distributing the code with your addons! 
___

## [FilterScripts](https://github.com/Agneese-Saini/SA-MP/tree/master/filterscripts)

| FileName		| Information	| Thread   |
| ------------- |:-------------:| --------:|
| [EnterExit.pwn](https://github.com/Agneese-Saini/SA-MP/blob/master/filterscripts/EnterExit.pwn) | GTA SA type moving arrow pickups, used to make enterance and exits | N/A |
| [TDEditor.pwn](https://github.com/Agneese-Saini/SA-MP/blob/master/filterscripts/TDEditor.pwn) | Keyboard based textdraw editor with modern features like having a Canvas panel allowing you to group textdraws and modify position together and lot more.. | http://forum.sa-mp.com/showthread.php?t=642981 |
| [clan.pwn](https://github.com/Agneese-Saini/SA-MP/blob/master/filterscripts/clan.pwn) | MySQL clan system, with nice textdraws. The system was build for a TeamDeathmatch type of gamemode | http://forum.sa-mp.com/showthread.php?t=656790 |
| [gban.pwn](https://github.com/Agneese-Saini/SA-MP/blob/master/filterscripts/gban.pwn) | SQLite ban system with IP Range ban support (player also see textdraw saying "You are banned" when banned) | http://forum.sa-mp.com/showthread.php?t=637472 |
| [grenade_launcher.pwn](https://github.com/Agneese-Saini/SA-MP/blob/master/filterscripts/grenade_launcher.pwn) | Example script of [projectile.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/projectile.inc). Basically your M4 rifle will get 5 grenades which you can shoot by pressing "N" (uses physics to calcualte path and also collides with SA world and players) | N/A |
| [inventory.pwn](https://github.com/Agneese-Saini/SA-MP/blob/master/filterscripts/inventory.pwn) | Basic textdraw menu inventory system for Survival servers | http://forum.sa-mp.com/showthread.php?t=636974 | 
| [mysql_ban.pwn](https://github.com/Agneese-Saini/SA-MP/blob/master/filterscripts/mysql_ban.pwn) | Similar to "GBan.pwn" but built with MySQL plugin and more optimized or up-to-date | N/A |
| [spectate.pwn](https://github.com/Agneese-Saini/SA-MP/blob/master/filterscripts/spectate.pwn) | Nice clickable textdraw spectate menu with auto transition for virtual-world/vehicle/death state changes | http://forum.sa-mp.com/showthread.php?t=652367 |
| [throw_deagle.pwn](https://github.com/Agneese-Saini/SA-MP/blob/master/filterscripts/throw_deagle.pwn) | Another example of [projectile.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/projectile.inc), this one is very basic, you can throw your deagle on ground, showing off physics in collision with SA world! | N/A |
| signature.pwn | Fully customizable Call of duty styled signature textdraws (display on deaths, from - who killed you) | http://forum.sa-mp.com/showthread.php?t=582950 |
| plabels.pwn | Player statistics label; similar to /dl, but for players! You can also have one label to concentrate on single player: /pl <playerid> | http://forum.sa-mp.com/showthread.php?t=573495 |
___

## [GameModes](https://github.com/Agneese-Saini/SA-MP/tree/master/gamemodes)

| FileName		| Information	| Thread   |
| ------------- |:-------------:| --------:|
| [base/mysql.pwn](https://github.com/Agneese-Saini/SA-MP/blob/master/gamemodes/base/mysql.pwn) | Starter MySQL login and register system with Security question addon for recovering/reseting password when user forgets! | http://forum.sa-mp.com/showthread.php?t=625195 |
| [base/sqlite.pwn](https://github.com/Agneese-Saini/SA-MP/blob/master/gamemodes/base/sqlite.pwn) | Similar to above but in SQLite | http://forum.sa-mp.com/showthread.php?t=625195 |
| [CopsAndTerrorists.pwn](https://github.com/Agneese-Saini/SA-MP/blob/master/gamemodes/CopsAndTerrorists.pwn) | A small los santos town is under war between Cops and Local Terrorists, there are 3 flags to capture for both teams! | N/A |
| Los Santos - Gangwars | 5 gangs fight over turfs in los santos teritory | http://forum.sa-mp.com/showthread.php?t=590991 |
| RedRex Freeroam | Fun freeroam gamemode with sub modes like: Team Deathmatch, Deathmatch, Race, Parkour and lot more... | http://forum.sa-mp.com/showthread.php?t=582050 |
| World War III | Team deathmatch set in desert of Las Venturas | http://forum.sa-mp.com/showthread.php?t=576348 |
| World War IV | Modern Team deathmatch set in desert of Las Venturas | http://forum.sa-mp.com/showthread.php?t=597799 |
___

## [Includes](https://github.com/Agneese-Saini/SA-MP/tree/master/pawno/include)

| FileName		| Information	| Thread   |
| ------------- |:-------------:| --------:|
| [EnableVehicleFriendlyFire.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/EnableVehicleFriendlyFire.inc) | A fix for annoying bug, when you have "EnableVehicleFriendlyFire()" toggled ON but your teammates can still pop your vehicle's tires! | http://forum.sa-mp.com/showthread.php?t=648320 |
| [PreviewModelDialog.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/PreviewModelDialog.inc) | Alternative to "mSelection.inc", more modern styled preview model textdraw menu with similar style to SA-MP dialogs | http://forum.sa-mp.com/showthread.php?t=570213 |
| [attachments.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/attachments.inc) | Fix: When you aim with scoped weapons (like sniper, RPG, etc.), attachments change bone automatically and comes infront of screen making it hard to view.<br><br>There is a better fix using Pawn-RakNet plugin, posted by Jelly on forums somewhere! | N/A |
| [cidr.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/cidr.inc) | Functions to check IP's range, can be used for range ban, AKA script! | http://forum.sa-mp.com/showpost.php?p=4037582&postcount=10 |
| [dini2.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/dini2.inc) | Hevaily improved version of original and old dini INI file processor! Gives a great speed boost! | http://forum.sa-mp.com/showthread.php?t=611399 |
| [easyDialog.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/easyDialog.inc) | (originally by Emmet_) Emmet's easy dialog mirror and with addon of support to my "PreviewModelDialog.inc", checkout function: "Dialog_OpenPreviewModel()" | http://forum.sa-mp.com/showthread.php?t=475838
| [fader.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/fader.inc) | Textdraw text color and box color fader | http://forum.sa-mp.com/showthread.php?t=644785 |
| [gangzones.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/gangzones.inc) | Adds borders and numbers to gangzones | http://forum.sa-mp.com/showthread.php?t=649230 |
| [gmenu.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/gmenu.inc) | Textdraw menu, alternative to GTA SA menus, works similarly but better functionality | http://forum.sa-mp.com/showthread.php?t=574271 |
| [map_parser.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/map_parser.inc) | (originally by "SouthClaws") You can load maps from text files now, plug'n'play features | N/A |
| [pause.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/pause.inc) | Allows you to detect players who are away from keyboard | N/A |
| [progress.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/progress.inc) | (originally by Toribio and SouthClaws for new styles) This have all progress2 features with ability to customize border/background/filler colors or even remove them (set color to 0x00).<br><br>And this include also adds support for Global Progress bars! | N/A |
| [projectile.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/projectile.inc) | (originally by Pepe) Physics simulator include. Uses ColAndreas plugin for collision detection with SA World too! | http://forum.sa-mp.com/showthread.php?t=630602 |
| [script_init.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/script_init.inc) | Adds two callbacks "OnScriptInit()" and "OnScriptExit()". So you no longer have to check if the script your include is being included in is a gamemode or a filterscript, just use these callbacks instead! | N/A |
| [vending.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/vending.inc) | Serversided vending machines, you can control how much helth to give, how much money to deduct etc. using the callback "OnPlayerUseVendingMachine()".<br><br>Also, to get a vending machine (default SA machines) data, use this function: "GetVendingMachineData()". | N/A |
| [weapon_damage.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/weapon_damage.inc) | Serversided weapons, the server will control weapon damage which also means, this works as an anti cheat!<br><br>You can prevent weapon damage cheat, weapon range cheat, fake kill, and this also uses OnPlayerGiveDamage which makes hit registeratio 10 times more accurate in terms of hitting player on your screen! | N/A |
| [strcalc.inc](https://github.com/Agneese-Saini/SA-MP/blob/master/pawno/include/strcalc.inc) | A simple string calculator, you provide a string with maths/numbers involved in it, and this function *strcalc()* will give you the result of it. | http://forum.sa-mp.com/showthread.php?t=657830 |
___

### Any Issues or Questions, post an issue: https://github.com/Agneese-Saini/SA-MP/issues/new
To post an issue about a library/mode, please include the filename so i know what you're talking about!
