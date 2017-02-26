# SA-MP-Files
All my SA-MP libraries in one folder!
The libraries mentioned here are supported by me and i may update them in future if there's any problem/bugs. Depends on priorty of library's importance and number of users.

# Road Map
Click on the embedded links to directly goto the following's page.

* filterscripts
  * <b><a href="https://github.com/Agneese-Saini/SA-MP-Files/blob/master/filterscripts/attachments.pwn">attachments.pwn</a></b> - player attachment editor with preview model dialogs.
* gamemodes
  * base
     * <b><a href="https://github.com/Agneese-Saini/SA-MP-Files/blob/master/gamemodes/base/mysql.pwn">mysql.pwn</a></b> - an advance MYSQL R41+ base gamemode
     * <b><a href="https://github.com/Agneese-Saini/SA-MP-Files/blob/master/gamemodes/base/sqlite.pwn">sqlite.pwn</a></b> - an advance SQLite base gamemode
* includes
  * include
    * <b><a href="https://github.com/Agneese-Saini/SA-MP-Files/tree/master/pawno/include/Patch">Patch</a></b>
      * <b>anims.inc</b> - animation library fix (preload and crash free)
      * <b>attachments.inc</b> - player attachments auto remove while aiming with scope weapons (e.g. snipers)
      * <b>gametext.inc</b> - prevent crash from invalid tildes in gametexts
      * <b>gangzone.inc</b> - auto fixes gangzones flickering at corners
      * <b>kickban.inc</b> - adds a delay in kick and ban function so messages and dialogs can be shown before executing (freeze player sync when function is used though to not allow packets to other players)
      * <b>string.inc</b> - general string function fixes
      * <b>vehicle.inc</b> - invalid mods protection (currently)
      * <b>lagcomp.inc</b> - fixes SAMP lagcomp response
    * <b><a href="https://github.com/Agneese-Saini/SA-MP-Files/tree/master/pawno/include/serversided">Serversided</a></b>
      * <b>main.inc</b> - main file to include server sided library (#include <serversided\main>)
      * <b>player_stats.inc</b> - internally used file
      * <b>script_init.inc</b> - internally used file
      * <b>static_pickup.inc</b> - internally used file
      * <b>vehicle_mods.inc</b> - internally used file
      * <b>vending_machine.inc</b> - internally used file
      * <b>weapon_config.inc</b> - internally used file
    * <b><a href="https://github.com/Agneese-Saini/SA-MP-Files/blob/master/pawno/include/arraylist.inc">arraylist.inc</a></b> - Foreach with all data-type supports (int, float, string)
    * <b><a href="https://github.com/Agneese-Saini/SA-MP-Files/blob/master/pawno/include/ban.inc">ban.inc</a></b> - SQL based SA-MP ban system (hooks functions "Ban", "BanEx"). Supports both MYSQL and SQLite
    * <b><a href="https://github.com/Agneese-Saini/SA-MP-Files/blob/master/pawno/include/dialogs.inc">dialogs.inc</a></b> - Adds in new dialog styles "DIALOG_STYLE_PREVMODEL" and "DIALOG_STYLE_PREVMODEL_LIST"
    * <b><a href="https://github.com/Agneese-Saini/SA-MP-Files/blob/master/pawno/include/dini2.inc">dini2.inc</a></b> - Improved dini processor with multi file processing and fast! *you can use this really)
    * <b><a href="https://github.com/Agneese-Saini/SA-MP-Files/blob/master/pawno/include/easydialog.inc">easydialog.inc</a></b> - Emmet_'s easydialog improved version
    * <b><a href="https://github.com/Agneese-Saini/SA-MP-Files/blob/master/pawno/include/enterexit.inc">enterexit.inc</a></b> - GTA Enter Exit replica done (reached 70%) (video: https://www.youtube.com/watch?v=BezTR39JzWw)
    * <b><a href="https://github.com/Agneese-Saini/SA-MP-Files/blob/master/pawno/include/gangzones.inc">gangzones.inc</a></b> - GangZones with borders and interior & virtual world support
    * <b><a href="https://github.com/Agneese-Saini/SA-MP-Files/blob/master/pawno/include/gmenu.inc">gmenu.inc</a></b> - GTA 5 styled menus with response control, even switching to one item to next
    * <b><a href="https://github.com/Agneese-Saini/SA-MP-Files/blob/master/pawno/include/progress2.inc">progress2.inc</a></b> - SouthClaw's progress2 with global textdraw support and some minor changes for productivity!
