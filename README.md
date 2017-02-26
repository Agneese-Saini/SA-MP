# SA-MP-Files
All my SA-MP libraries in one folder!
The libraries mentioned here are supported by me and i may update them in future if there's any problem/bugs. Depends on priorty of library's importance and number of users.

# Road Map
* filterscripts
  * <b>attachments.pwn</b> - player attachment editor with preview model dialogs.
* gamemodes
  * base
     * <b>mysql.pwn</b> - an advance MYSQL R41+ base gamemode
     * <b>sqlite.pwn</b> - an advance SQLite base gamemode
* includes
  * include
    * Patch
      * <b>anims.inc</b> - animation library fix (preload and crash free)
      * <b>attachments.inc</b> - player attachments auto remove while aiming with scope weapons (e.g. snipers)
      * <b>gametext.inc</b> - prevent crash from invalid tildes in gametexts
      * <b>gangzone.inc</b> - auto fixes gangzones flickering at corners
      * <b>kickban.inc</b> - adds a delay in kick and ban function so messages and dialogs can be shown before executing (freeze player sync when function is used though to not allow packets to other players)
      * <b>string</b> - general string function fixes
      * <b>vehicle</b> - invalid mods protection (currently)
    * Serversided
      * <b>main.inc</b> - main file to include server sided library (#include <serversided\main>)
      * <b>player_stats.inc</b> - internally used file
      * <b>script_init.inc</b> - internally used file
      * <b>static_pickup.inc</b> - internally used file
      * <b>vehicle_mods.inc</b> - internally used file
      * <b>vending_machine.inc</b> - internally used file
      * <b>weapon_config.inc</b> - internally used file
    * <b>arraylist.inc</b> - Foreach with all data-type supports (int, float, string)
    * <b>ban.inc</b> - SQL based SA-MP ban system (hooks functions "Ban", "BanEx"). Supports both MYSQL and SQLite
    * <b>dialogs.inc</b> - Adds in new dialog styles "DIALOG_STYLE_PREVMODEL" and "DIALOG_STYLE_PREVMODEL_LIST"
    * <b>dini2.inc</b> - Improved dini processor with multi file processing and fast! *you can use this really)
    * <b>easydialog.inc</b> - Emmet_'s easydialog improved version
    * <b>enterexit.inc</b> - GTA Enter Exit replica done (reached 70%) (video: https://www.youtube.com/watch?v=BezTR39JzWw)
    * <b>gangzones.inc</b> - GangZones with borders and interior & virtual world support
    * <b>gmenu.inc</b> - GTA 5 styled menus with response control, even switching to one item to next
    * <b>progress2.inc</b> - SouthClaw's progress2 with global textdraw support and some minor changes for productivity!
