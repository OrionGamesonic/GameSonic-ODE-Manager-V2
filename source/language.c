/*
 * Language IrisManager by D_Skywalk
 *
 * Copyright (c) 2011 David Colmenero Aka D_Skywalk
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *
 *   Simple code for play with languages
 *    for ps3 scene ;)
 *
**/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "language.h"
#include "utils.h"
#include "language_ini_en_bin.h"
#include "language_ini_sp_bin.h"
#include "language_ini_fr_bin.h"
#include "language_ini_it_bin.h"
#include "language_ini_nw_bin.h"
#include "language_ini_ps_bin.h"
#include "language_ini_chs_bin.h"
#include "language_ini_cht_bin.h"
#include "language_ini_gm_bin.h"
#include "language_ini_po_bin.h"

#define LANGFILE_VERSION 2

typedef struct lngstr
{
  u32 code;
  char * strname;  
  char * strdefault; 
} t_lngstr;

t_lngstr lang_strings[] = 
{
    //MAIN
    // VIDEO - ADJUST
    {VIDEOADJUST_POSITION, "VIDEOADJUST_POSITION"    , "Use LEFT (-X) / RIGHT (+X) / UP (-Y) / DOWN (+Y) to adjust the screen" },
    {VIDEOADJUST_SCALEINFO, "VIDEOADJUST_SCALEINFO"   , "Video Scale X: %i Y: %i" },
    {VIDEOADJUST_EXITINFO, "VIDEOADJUST_EXITINFO"    , "Press CROSS to exit" },
    {VIDEOADJUST_DEFAULTS, "VIDEOADJUST_DEFAULTS"    , "Press CIRCLE to default values" },

    //SELECT - GAME FOLDER
    {GAMEFOLDER_WANTUSE, "GAMEFOLDER_WANTUSE"      , "Want to use" },
    {GAMEFOLDER_TOINSTALLNTR, "GAMEFOLDER_TOINSTALLNTR" , "to install the game?" },
    {GAMEFOLDER_USING, "GAMEFOLDER_USING"        , "Using" },
    {GAMEFOLDER_TOINSTALL, "GAMEFOLDER_TOINSTALL"    , "to install the game" },
    
    //DRAW SCREEN1
    { DRAWSCREEN_FAVSWAP, "DRAWSCREEN_FAVSWAP"      , "Favourites Swap" },
    { DRAWSCREEN_FAVINSERT, "DRAWSCREEN_FAVINSERT"    , "Favourites Insert" },
    { DRAWSCREEN_FAVORITES, "DRAWSCREEN_FAVORITES"    , "Favourites" },
    { DRAWSCREEN_PAGE, "DRAWSCREEN_PAGE"         , "Page" },
    { DRAWSCREEN_GAMES,"DRAWSCREEN_GAMES"        , "Games" },
    { DRAWSCREEN_PLAY, "DRAWSCREEN_PLAY"         , "Play" },
    { DRAWSCREEN_SOPTIONS, "DRAWSCREEN_SOPTIONS"     , "SELECT: Game Options" },
    { DRAWSCREEN_SDELETE, "DRAWSCREEN_SDELETE"      , "SELECT: Delete Favourite" },
    { DRAWSCREEN_STGLOPT, "DRAWSCREEN_STGLOPT"      , "START: Global Options" },
    { DRAWSCREEN_EXITXMB, "DRAWSCREEN_EXITXMB"      , "Exit to XMB?" },
    { DRAWSCREEN_RESTART, "DRAWSCREEN_RESTART"      , "Restart the PS3 System?" },
    { DRAWSCREEN_CANRUNFAV, "DRAWSCREEN_CANRUNFAV"    , "Cannot run this favourite" },
    { DRAWSCREEN_MARKNOTEXEC, "DRAWSCREEN_MARKNOTEXEC"  , "Marked as non executable. Trying to install in HDD0 cache" },
    { DRAWSCREEN_MARKNOTEX4G, "DRAWSCREEN_MARKNOTEX4G"  ,"Marked as not executable - Contains splited files >= 4GB" },
    { DRAWSCREEN_GAMEINOFMNT, "DRAWSCREEN_GAMEINOFMNT"  , "I cannot find one folder to mount /dev_hdd0/game from USB" },
    { DRAWSCREEN_GAMEIASKDIR, "DRAWSCREEN_GAMEIASKDIR"  , "Want to create in /dev_usb00" },
    { DRAWSCREEN_GAMEICANTFD, "DRAWSCREEN_GAMEICANTFD"  , "I cannot find an USB device to mount /dev_hdd0/game from USB" },
    { DRAWSCREEN_GAMEIWLAUNCH, "DRAWSCREEN_GAMEIWLAUNCH" , "Want to launch the Game?" },
    { DRAWSCREEN_EXTEXENOTFND, "DRAWSCREEN_EXTEXENOTFND" , "external executable not found" },
    { DRAWSCREEN_EXTEXENOTCPY, "DRAWSCREEN_EXTEXENOTCPY" , "Use 'Copy EBOOT.BIN from USB' to import them." },
    { DRAWSCREEN_REQBR, "DRAWSCREEN_REQBR"        , "Required BR-Disc, Retry?" },
    { DRAWSCREEN_PARCONTROL, "DRAWSCREEN_PARCONTROL"        , "Locked by Parental Control" },
    { DRAWSCREEN_GAMESIZE,	"DRAWSCREEN_GAMESIZE"		, "Game Size" },
    { DRAWSCREEN_EJECTINFO,	"DRAWSCREEN_EJECTINFO"		, "Eject and reinsert the Blu-ray Disc.\nIf you have a ps3 series 4K instead open and close the cover." },
	{ DRAWSCREEN_MCUOLD,	"DRAWSCREEN_MCUOLD"		, "Auto eject is not supported from this cobra firmware\nEject and reinsert the Blu-ray Disc.\nIf you have a ps3 series 4K instead open and close the cover." },
	{ DRAWSCREEN_COPYGAME,	"DRAWSCREEN_COPYGAME"		, "SELECT: Copy Game" },

    //DRAW OPTIONS
    { DRAWGMOPT_OPTS, "DRAWGMOPT_OPTS"          , "Options" },
    { DRAWGMOPT_CFGGAME, "DRAWGMOPT_CFGGAME"       , "Config. Game" },
    { DRAWGMOPT_CPYGAME, "DRAWGMOPT_CPYGAME"       , "Copy Game" },
    { DRAWGMOPT_DELGAME, "DRAWGMOPT_DELGAME"       , "Delete Game" },
    { DRAWGMOPT_FIXGAME, "DRAWGMOPT_FIXGAME"       , "Fix File Permissions" },
    { DRAWGMOPT_TSTGAME, "DRAWGMOPT_TSTGAME"       , "Test Game" },
    { DRAWGMOPT_CPYEBOOTGAME, "DRAWGMOPT_CPYEBOOTGAME"  , "Copy EBOOT.BIN from USB" },
    { DRAWGMOPT_CPYTOFAV, "DRAWGMOPT_CPYTOFAV"      , "Copy to Favourites" },
    { DRAWGMOPT_DELFMFAV, "DRAWGMOPT_DELFMFAV"      , "Delete from Favourites" },

    { DRAWGMOPT_FIXCOMPLETE, "DRAWGMOPT_FIXCOMPLETE"   , "Fix Permissions Done!" },
    { DRAWGMOPT_CPYOK, "DRAWGMOPT_CPYOK"         , "copied successfully" },
    { DRAWGMOPT_CPYERR, "DRAWGMOPT_CPYERR"        , "Error copying" },
    { DRAWGMOPT_CPYNOTFND, "DRAWGMOPT_CPYNOTFND"     , "not found" },

	//DRAW SETUP VIDEO
    { DRAWSETVID_SETVIDEO, "DRAWSETVID_SETVIDEO"     , "Setup Video" },
    { DRAWSETVID_SCRADJUST, "DRAWSETVID_SCRADJUST"     , "Video Adjust" },
    { DRAWSETVID_CHANGEBCK, "DRAWSETVID_CHANGEBCK"     , "Change Background Color" },
    { DRAWSETVID_CHANGEBCKHERMES, "DRAWSETVID_CHANGEBCKHERMES"     , "Change Background Color (Hermes Style)" },
	{ DRAWSETVID_GRID8x6, "DRAWSETVID_GRID8x6"     , "Screen set: Grid 8x6" },	
    { DRAWSETVID_GRID6x4, "DRAWSETVID_GRID6x4"     , "Screen set: Grid 6x4" },
    { DRAWSETVID_GRID4x3, "DRAWSETVID_GRID4x3"     , "Screen set: Grid 4x3" },
	
    //DRAW_PSX
    { DRAWPSX_EMULATOR,  "DRAWPSX_EMULATOR"   , "Emulator" },
    { DRAWPSX_VIDEOPS,   "DRAWPSX_VIDEOPS"    , "PSX Video Options" },
    { DRAWPSX_SAVEASK,   "DRAWPSX_SAVEASK"    , "Save PSX options?" },
    { DRAWPSX_SAVED,     "DRAWPSX_SAVED"      , "PSX Options Saved" },
    { DRAWPSX_VIDEOTHER, "DRAWPSX_VIDEOTHER"  , "Video / Others" },
    { DRAWPSX_VIDEOMODE, "DRAWPSX_VIDEOMODE"  , "Video Mode" },
    { DRAWPSX_VIDEOASP,  "DRAWPSX_VIDEOASP"   , "Video Aspect (480/576)" },
    { DRAWPSX_FULLSCR,   "DRAWPSX_FULLSCR"    , "Full Screen" },
    { DRAWPSX_SMOOTH,    "DRAWPSX_SMOOTH"     , "Smooting" },
    { DRAWPSX_EXTROM,    "DRAWPSX_EXTROM"     , "External ROM" },
    { DRAWPSX_FORMAT,    "DRAWPSX_FORMAT"     , "Format Internal_MC" },
    { DRAWPSX_ASKFORMAT, "DRAWPSX_ASKFORMAT"  , "Want you format Internal_MC?\n\nYou LOSE the saves in this operation" },
    { DRAWPSX_ERRWRITING,"DRAWPSX_ERRWRITING" , "Error writing the file (Device full?)" },

    { DRAWPSX_BUILDISO,  "DRAWPSX_BUILDISO"   , "Building custom ISO..." },
    { DRAWPSX_ASKCHEATS, "DRAWPSX_ASKCHEATS"  , "PSX Cheat disc found\n\nWant you use it?" },
    { DRAWPSX_ERRCHEATS, "DRAWPSX_ERRCHEATS"  , "PSX Disc for Cheats cannot be launched\n\nwithout a PSX game" },
    { DRAWPSX_ERRSECSIZE,"DRAWPSX_ERRSECSIZE" , "Error: Different sector size in ISO files" },
    { DRAWPSX_ERRUNKSIZE,"DRAWPSX_ERRUNKSIZE" , "Error: Unknown Sector Size" },
    { DRAWPSX_DISCEJECT ,"DRAWPSX_DISCEJECT"  , "PSX CD Ejected" },
    { DRAWPSX_DISCORDER , "DRAWPSX_DISCORDER" , "Select Disc Order" },
    { DRAWPSX_PRESSOB   ,"DRAWPSX_PRESSOB"    , "Press CIRCLE to change the order" },
    { DRAWPSX_PRESSXB   ,"DRAWPSX_PRESSXB"    , "Press CROSS to launch de game" },
    { DRAWPSX_CHEATMAKE ,"DRAWPSX_CHEATMAKE"  , "PSX Cheat disc found, but different sector size\n\nWant you build one compatible?" },
    { DRAWPSX_COPYMC    ,"DRAWPSX_COPYMC"     , "Copying Memory Card to HDD0 device..." },
    { DRAWPSX_ERRCOPYMC ,"DRAWPSX_ERRCOPYMC"  , "Error copying the Memory Card to HDD0 device" },
    { DRAWPSX_PUTFNAME  ,"DRAWPSX_PUTFNAME"   , "Put a Folder Name:" },
    { DRAWPSX_FMUSTB    ,"DRAWPSX_FMUSTB"     , "Folder Name must be >=3 chars" },
    { DRAWPSX_PUTADISC  ,"DRAWPSX_PUTADISC"   , "Put a PSX disc and press YES to continue or NO to abort\n\nDisc to copy: " },
    { DRAWPSX_UNREC     ,"DRAWPSX_UNREC"      , "Unrecognized disc" },
    { DRAWPSX_ERROPENING,"DRAWPSX_ERROPENING" , "Error opening BDVD drive" },
    { DRAWPSX_ASKEFOLDER,"DRAWPSX_ASKEFOLDER" , "Folder Exits\n\nContinue?" },
    { DRAWPSX_ISOEXITS  ,"DRAWPSX_ISOEXITS"   , "exists\n\nSkip?" },

    //DRAW CONFIGS
    { DRAWGMCFG_CFGS, "DRAWGMCFG_CFGS"          , "Config. Game" },
    { DRAWGMCFG_DSK, "DRAWGMCFG_DSK"           , "Requires Disc" },
    { DRAWGMCFG_NO, "DRAWGMCFG_NO"            , "No" },
    { DRAWGMCFG_YES,"DRAWGMCFG_YES"           , "Yes" },
    { DRAWGMCFG_UPD, "DRAWGMCFG_UPD"           , "Online Updates" },
    { DRAWGMCFG_ON, "DRAWGMCFG_ON"            , "On" },
    { DRAWGMCFG_OFF, "DRAWGMCFG_OFF"           , "Off" },
    { DRAWGMCFG_EXTBOOT, "DRAWGMCFG_EXTBOOT"       , "Extern EBOOT.BIN" },
    { DRAWGMCFG_BDEMU, "DRAWGMCFG_BDEMU"         , "BD Emu (for USB)" },
    { DRAWGMCFG_EXTHDD0GAME, "DRAWGMCFG_EXTHDD0GAME"   , "Ext /dev_hdd0/game" },
    { DRAWGMCFG_SAVECFG, "DRAWGMCFG_SAVECFG"       , "Save Config" },

    //DRAW GLOBAL OPTIONS
    { DRAWGLOPT_OPTS, "DRAWGLOPT_OPTS"          , "Global Options" },
    
    { DRAWGLOPT_SCRADJUST, "DRAWGLOPT_SCRADJUST"     , "Video Adjust" },
    { DRAWGLOPT_CHANGEBCK, "DRAWGLOPT_CHANGEBCK"     , "Change Background Color" },
    { DRAWGLOPT_CHANGEBCKHERMES, "DRAWGLOPT_CHANGEBCKHERMES"     , "Change Background Color (Hermes Style)" },
	{ DRAWGLOPT_GRID8x6, "DRAWGLOPT_GRID8x6"     , "Screen set: Grid 8x6" },	
    { DRAWGLOPT_GRID6x4, "DRAWGLOPT_GRID6x4"     , "Screen set: Grid 6x4" },
    { DRAWGLOPT_GRID4x3, "DRAWGLOPT_GRID4x3"     , "Screen set: Grid 4x3" },
    
	{ DRAWGLOPT_SETVIDEO, "DRAWGLOPT_SETVIDEO"     , "Setup video" },
    { DRAWGLOPT_LANGUAGE_1, "DRAWGLOPT_LANGUAGE_1"     , "English" },
    { DRAWGLOPT_LANGUAGE_2, "DRAWGLOPT_LANGUAGE_2"     , "Español" },
    { DRAWGLOPT_LANGUAGE_3, "DRAWGLOPT_LANGUAGE_3"     , "Française" },
    { DRAWGLOPT_LANGUAGE_4, "DRAWGLOPT_LANGUAGE_4"     , "Italiano" },
    { DRAWGLOPT_LANGUAGE_5, "DRAWGLOPT_LANGUAGE_5"     , "Norsk" },
    { DRAWGLOPT_LANGUAGE_6, "DRAWGLOPT_LANGUAGE_6"     , "Deutsch" },
    { DRAWGLOPT_LANGUAGE_7, "DRAWGLOPT_LANGUAGE_7"     , "Português" },
    { DRAWGLOPT_LANGUAGE_8, "DRAWGLOPT_LANGUAGE_8"     , "(test) فارسی" },
    { DRAWGLOPT_LANGUAGE_9, "DRAWGLOPT_LANGUAGE_9"     , "Chinese Simplified"},
    { DRAWGLOPT_LANGUAGE_10, "DRAWGLOPT_LANGUAGE_10"     , "Chinese Traditional"},
    { DRAWGLOPT_LANGUAGE_11, "DRAWGLOPT_LANGUAGE_11"     , "Custom (from file)"},
    
    { DRAWGLOPT_INITFTP, "DRAWGLOPT_INITFTP"       , "Initialize FTP server" },
    { DRAWGLOPT_FTPINITED, "DRAWGLOPT_FTPINITED"     , "Server FTP initialized\nDo you want auto-enable FTP service on init?" },
    { DRAWGLOPT_FTPARINITED, "DRAWGLOPT_FTPARINITED"   , "Server FTP already initialized" },
    { DRAWGLOPT_FTPSTOPED, "DRAWGLOPT_FTPSTOPED"     , "Server FTP Stoped\nRemoved FTP service on init." },
    
    { DRAWGLOPT_CHANGEDIR, "DRAWGLOPT_CHANGEDIR"     , "Change Game Directory" },
    { DRAWGLOPT_SWMUSICOFF, "DRAWGLOPT_SWMUSICOFF"    , "Switch Music Off" },
    { DRAWGLOPT_SWMUSICON, "DRAWGLOPT_SWMUSICON"     , "Switch Music On" },
    
    { DRAWGLOPT_TOOLS, "DRAWGLOPT_TOOLS"         , "Tools" },
    { DRAWGLOPT_CREDITS, "DRAWGLOPT_CREDITS"       , "Credits" },
	{ DRAWGLOPT_BROWSER, "DRAWGLOPT_BROWSER"       , "Browser Internet" },
    
    { DRAWGLOPT_INSERTUSB, "DRAWGLOPT_INSERTUSB", "Please insert an USB device in the right port for save your settings" },
    
    //DRAW NETWORK TOOLS
    { DRAWNETOOLS_NETOOLS, "DRAWNETOOLS_NETOOLS"     , "Network Tools" },
    { DRAWNETOOLS_SETIDPS, "DRAWNETOOLS_SETIDPS"     , "Set new console id" },
    { DRAWNETOOLS_DEFIDPS, "DRAWNETOOLS_DEFIDPS"     , "Restore original console id (EID5)" },
    { DRAWNETOOLS_IDNOTVALID, "DRAWNETOOLS_IDNOTVALID"     , "The console id inserted is not valid!!" },
    { DRAWNETOOLS_INSERTID, "DRAWNETOOLS_INSERTID"     , "Enter the console id.." },
    { DRAWNETOOLS_IDPSPER, "DRAWNETOOLS_IDPSPER"     , "Do you want CORE (ps3itald.self) automatically set this console id when you boot PS3?" },
    { DRAWNETOOLS_IDPSOK, "DRAWNETOOLS_IDPSOK"     , "Done :)" },
    { DRAWNETOOLS_DEFSTILLSET, "DRAWNETOOLS_DEFSTILLSET"     , "Default console id is always set" },
    { DRAWNETOOLS_THISISYOURDEF, "DRAWNETOOLS_THISISYOURDEF"     , "You have entered default console id, uh.??" },
    
    //DRAW FTP ERRORS
    { DRAWFTP_ENETINITIALIZE, "DRAWFTP_ENETINITIALIZE"     , "Error in netInitialize()" },
    { DRAWFTP_ENETCTLINIT, "DRAWFTP_ENETCTLINIT"     , "Error in netCtlInit()" },
    { DRAWFTP_ENETCTLGETINFO, "DRAWFTP_ENETCTLGETINFO"     , "Error in netCtlGetInfo()" },
    { DRAWFTP_ECONNECTION, "DRAWFTP_ECONNECTION"     , "Net Disconnected or Connection not Established" },
    { DRAWFTP_EUNKNOWN, "DRAWFTP_EUNKNOWN"     , "FTP Unknown Error" },
    { DRAWFTP_EINTACTIVITY, "DRAWFTP_EINTACTIVITY"     , "FTP is working now\nDo you want interrupt the FTP activity?" },

    //DRAW TOOLS
    { DRAWTOOLS_TOOLS, "DRAWTOOLS_TOOLS"         , "Tools" },
    { DRAWTOOLS_DELCACHE, "DRAWTOOLS_DELCACHE"      , "Delete Cache Tool" },
   // { DRAWTOOLS_SECDISABLE, "DRAWTOOLS_SECDISABLE"    , "Press To Disable Syscall Security" },
   // { DRAWTOOLS_SECENABLE, "DRAWTOOLS_SECENABLE"     , "Press To Enable Syscall Security" },
    
    { DRAWTOOLS_COPYFROM, "DRAWTOOLS_COPYFROM"     , "Copy from /dev_usb/PS3ITA to PS3ITA folder"},
    { DRAWTOOLS_WITHBDVD, "DRAWTOOLS_WITHBDVD"     , "With BDVD Controller"},
    { DRAWTOOLS_NOBDVD,   "DRAWTOOLS_NOBDVD"         , "Without BDVD Device"},
	{ DRAWTOOLS_NOBDVD2,   "DRAWTOOLS_NOBDVD2"         , "Disc - Less payload"},
    
    { DRAWTOOLS_FIXFWVERPSNUPD,   "DRAWTOOLS_FIXFWVERPSNUPD"         , "Fix fw version in psn/update games"},
	{ DRAWTOOLS_FIXFWVERPSNUPDWAIT,   "DRAWTOOLS_FIXFWVERPSNUPDWAIT"         , "Wait for the next screen!!!\n\nProcessing in progress..."},
    { DRAWTOOLS_FIXFWVERPSNUPDOK,   "DRAWTOOLS_FIXFWVERPSNUPDOK"         , "All games have been fixed"},
    
    { DRAWTOOLS_PKGTOOLS, "DRAWTOOLS_PKGTOOLS"         , ".PKG Install" },
    { DRAWTOOLS_ARCHIVEMAN, "DRAWTOOLS_ARCHIVEMAN"         , "Archive Manager" },

    //MAIN - OTHERS
    { DRAWCACHE_CACHE, "DRAWCACHE_CACHE"         , "Delete Cache Tool" },
    { DRAWCACHE_ERRNEEDIT, "DRAWCACHE_ERRNEEDIT"     , "You need %1.2f GB free to install" },
    { DRAWCACHE_ASKTODEL, "DRAWCACHE_ASKTODEL"      , "Delete %s Cache?" },
    { PATCHBEMU_ERRNOUSB, "PATCHBEMU_ERRNOUSB"      , "BDEMU is only for USB devices" },
    { MOVEOBEMU_ERRSAVE, "MOVEOBEMU_ERRSAVE"       , "Error Saving:\n%s" },
    { MOVEOBEMU_ERRMOVE, "MOVEOBEMU_ERRMOVE"       , "Error Moving To:\n%s/PS3_GAME exists" },
    { MOVEOBEMU_MOUNTOK, "MOVEOBEMU_MOUNTOK"       , "BDEMU mounted in:\n%s/PS3_GAME" },
    { MOVETBEMU_ERRMOVE, "MOVETBEMU_ERRMOVE"       , "Error Moving To:\n%s exists" },

    //MAIN - GLOBAL
    { GLOBAL_RETURN, "GLOBAL_RETURN"           , "Return" },
    { GLOBAL_SAVED, "GLOBAL_SAVED"            , "File Saved" },

    //UTILS
    //FAST COPY ADD
    { FASTCPADD_FAILED, "FASTCPADD_FAILED"        , "Failed in fast_copy_process() ret" },
    { FASTCPADD_ERRTMFILES, "FASTCPADD_ERRTMFILES"    , "Too much files" },
    { FASTCPADD_FAILEDSTAT, "FASTCPADD_FAILEDSTAT"    , "Failed in stat()" },
    { FASTCPADD_ERROPEN, "FASTCPADD_ERROPEN"       , "Error Opening0 (write)" },
    { FASTCPADD_COPYING, "FASTCPADD_COPYING"       , "Copying" },
    { FASTCPADD_FAILFASTFILE, "FASTCPADD_FAILFASTFILE"  , "Failed in fast_files(fast_num_files).mem" },

    //FAST COPY PROCESS
    { FASTCPPRC_JOINFILE, "FASTCPPRC_JOINFILE"      , "Joining file" },
    { FASTCPPRC_COPYFILE, "FASTCPPRC_COPYFILE"      , "Copying. File" },
    { FASTCPPTC_OPENERROR, "FASTCPPTC_OPENERROR"     , "Error!!!!!!!!!!!!!!!!!!!!!!!!!\nFiles Opened %i\n Waiting 20 seconds to display fatal error" },

    //GAME TEST FILES
    { GAMETESTS_FOUNDINSTALL, "GAMETESTS_FOUNDINSTALL"  , "Found %s\n\nWant to install?" },
    { GAMETESTS_BIGFILE, "GAMETESTS_BIGFILE"       , "Big file" },
    { GAMETESTS_TESTFILE, "GAMETESTS_TESTFILE"      , "Test File" },
    { GAMETESTS_CHECKSIZE, "GAMETESTS_CHECKSIZE"     , "Checking Size of File" },

    //GAME DELETE FILES
    { GAMEDELFL_DELETED, "GAMEDELFL_DELETED"       , "Deleted" },
    { GAMEDELFL_DELETING, "GAMEDELFL_DELETING"      , "Deleting... File" },

    //GAME COPY
    { GAMECPYSL_GSIZEABCNTASK, "GAMECPYSL_GSIZEABCNTASK" , "Get Size: Aborted - Continue the copy?" },
    { GAMECPYSL_STARTED, "GAMECPYSL_STARTED"       , "Start copy dump on %s ?" },
    { GAMECPYSL_SPLITEDHDDNFO, "GAMECPYSL_SPLITEDHDDNFO" , "%s\n\nSplit game copied in HDD0 (non bootable)" },
    { GAMECPYSL_SPLITEDUSBNFO, "GAMECPYSL_SPLITEDUSBNFO" , "%s\n\nSplit game copied in USB00%c (non bootable)" },
    { GAMECPYSL_DONE, "GAMECPYSL_DONE"          , "Done! Files Copied" },
    { GAMECPYSL_FAILDELDUMP, "GAMECPYSL_FAILDELDUMP"   , "Delete failed dump in" },
    { GAMECPYSL_NOSPACEONUSB, "GAMECPYSL_NOSPACEONUSB"   , "Usb device not found or not enough free space on it.\nYou need %.3 GB (%d MB) of free space" },
    { GAMECPYSL_NOBDVD, "GAMECPYSL_NOBDVD"   , "No BDVD found" },
    

    //GAME CACHE COPY
    { GAMECHCPY_ISNEEDONEFILE, "GAMECHCPY_ISNEEDONEFILE" , "Sorry, but you needs to install at least a bigfile" },
    { GAMECHCPY_NEEDMORESPACE, "GAMECHCPY_NEEDMORESPACE" , "You have %.2fGB free and you needs %.2fGB\n\nPlease, delete Cache Entries" },
    { GAMECHCPY_NOSPACE, "GAMECHCPY_NOSPACE"       , "Sorry, you have %.2fGB free\n\nand you needs %.2fGB" },
    { GAMECHCPY_CACHENFOSTART, "GAMECHCPY_CACHENFOSTART" , "Cache Files: %.2fGB - Total Files: %.2fGB\n you save %.2fGB on HDD0 (%.2fGB Total)\n\nPress any button to Start" },
    { GAMECHCPY_FAILDELFROM, "GAMECHCPY_FAILDELFROM"   , "Delete Cache failed dump from" },

    //GAME DELETE
    { GAMEDELSL_WANTDELETE, "GAMEDELSL_WANTDELETE"    , "Want to delete from" },
    { GAMEDELSL_STARTED, "GAMEDELSL_STARTED"       , "Starting... \n delete" },
    { GAMEDELSL_DONE, "GAMEDELSL_DONE"          , "Done!  Files Deleted" },

    //GAME TEST
	// warning! don't translate GAMETSTSL_FINALNFO2 from english
    { GAMETSTSL_FINALNFO2, "GAMETSTSL_FINALNFO2"      , "Directories: %i Files: %i\nBig files: %i Split files: %i" },
    { GAMETSTSL_TESTED, "GAMETSTSL_TESTED"        , "Files Tested" },

    //GLOBAL UTILS
    { GLUTIL_SPLITFILE, "GLUTIL_SPLITFILE"        , "Split file" },
    { GLUTIL_WROTE, "GLUTIL_WROTE"            , "Wrote" },
    { GLUTIL_TIME, "GLUTIL_TIME"             , "Time" },
    { GLUTIL_TIMELEFT, "GLUTIL_TIMELEFT"         , "Time Left" },
    { GLUTIL_HOLDTRIANGLEAB, "GLUTIL_HOLDTRIANGLEAB"   , "Hold TRIANGLE to Abort" },
    { GLUTIL_HOLDTRIANGLESK, "GLUTIL_HOLDTRIANGLESK"   , "Hold TRIANGLE to Skip" },
    { GLUTIL_ABORTEDUSER, "GLUTIL_ABORTEDUSER"      , "Aborted by user" },
    { GLUTIL_ABORTED, "GLUTIL_ABORTED"          , "Aborted!!!" },
    { GLUTIL_XEXIT, "GLUTIL_XEXIT"            , "Press CROSS to Exit" },
    { GLUTIL_WANTCPYFROM, "GLUTIL_WANTCPYFROM"      , "Want to copy from" },
    { GLUTIL_WTO, "GLUTIL_WTO"              , "to" },
    
    // INSTALL .PKG
    { PKG_HEADER, "PKG_HEADER", "Install .PKG Utility -     Use CROSS to select and CIRCLE to exit"},
    { PKG_INSERTUSB, "PKG_INSERTUSB", "Insert the USB mass storage device"},
    { PKG_ERRTOBIG, "PKG_ERRTOBIG", ".PKG size too big or disk space small"},
    { PKG_WANTINSTALL, "PKG_WANTINSTALL", "Want to Install this .PKG file?"},
    { PKG_ERRALREADY, "ERRALREADY", "Error: .PKG already in the stack"},
    { PKG_ERRFULLSTACK, "PKG_ERRFULLSTACK", "Error: stack is full (max 16 entries)"},
    { PKG_ERRBUILD, "PKG_ERRBUILD", "Error Building .PKG process"},
    { PKG_COPYING, "PKG_COPYING", "Copying .PKG file to PS3ITA Manager folder..."},
    { PKG_ERROPENING, "PKG_ERROPENING", "Error Opening .PKG file"},
    { PKG_ERRCREATING, "PKG_ERRCREATING", "Error Creating .PKG file"},
    { PKG_ERRREADING, "PKG_ERRREADING", "Error Reading .PKG file"},
    { PKG_ERRLICON, "PKG_ERRLICON", "Error Loading ICON file"},
    { PKG_ERRMOVING, "PKG_ERRMOVING", "Error moving .PKG"},
   
     // generic
    { OUT_OFMEMORY, "OUT_OFMEMORY", "Out of Memory"},
    { OPERATION_DONE, "OPERATION_DONE"     , "Done!" },
    { PLUG_STORAGE1, "PLUG_STORAGE1" , 
        "Remember you to plug an USB storage mass device to create the fake disc event" },
    { PLUG_STORAGE2, "PLUG_STORAGE2" , "Fake Disc Inserted" },
    { PAYLOAD_MAPFAILED, "PAYLOAD_MAPFAILED" , "Error Loading Payload: map failed?!" },
    { PAYLOAD_FWUNSUPPORTED, "PAYLOAD_FWUNSUPPORTED" , "Error: Unsupported firmware!" },
    { WARN_EJECTLOADISC, "WARN_EJECTLOADISC" , "Warning! You must eject/load the disc" },
    { WARN_DISCFAILED, "WARN_DISCFAILED" , "Warning! Disc authentication failed" },
    { PATCH09_WAITGAME, "WARN_DISCFAILED" , "The fw version in the game should be fixed, Fix it!\nDo not turn off the PS3 or remove the usb storage mass device!\
		                                    \nThe process may take some minutes..Please Wait.." },
	{ PATCH09_WAITUPDATE, "PATCH09_WAITUPDATE" , "The fw version in the game update should be fixed, Fix it!\nDo not turn off the PS3!!!\
												 \nThe process may take some minutes..Please Wait.." },
	{ PATCH09_REBUILDGAME, "PATCH09_REBUILDGAME" , "This game is not bootable!\nRebuild game with a pc tool (example: trueancestor)" },
	{ PATCH09_REBUILDUPDATE, "PATCH09_REBUILDUPDATE" , "This game is not bootable!\nDelete game update or rebuild it with a pc tool (example: trueancestor)" },
  
    
    { LANGSTRINGS_COUNT, "", ""}
};

char * language[LANGSTRINGS_COUNT];

static int lang_inited = 0;

int reverse_language = 0;

int open_language (int lang, char * filename) 
{

    int n;//, version;
    struct stat s;

    int elements = sizeof(lang_strings)/sizeof(t_lngstr);
	
	char get_reverse[64]="";

    reverse_language = 0;

    for (n = 0; n < LANGSTRINGS_COUNT; n++)
    {
        if(!lang_inited) 
            language[n] = (char*) malloc(MAX_CFGLINE_LEN);
        strncpy(language[n], "***ERROR****", MAX_CFGLINE_LEN-1);
        
    }

    lang_inited = 1;

    char * file_external = NULL;
    int file_size = 0;

    if(lang>=10) { // test external filename
        if(!stat(filename, &s))
            file_external = LoadFile(filename, &file_size);
        
        if(!file_external) lang = 0;
    }

    for (n = 0; n < elements; n++)
    {

        if(lang_strings[n].code == LANGSTRINGS_COUNT) break;

        if(lang>=10)
        {

               getConfigMemValueString((char *) file_external, file_size, "Language",
                "REVERSE", get_reverse, 63, "OFF");

               if(!strcasecmp((const char *) get_reverse, "on")) reverse_language = 1;

                // from external file
           
                strncpy(language[lang_strings[n].code], lang_strings[n].strdefault, MAX_CFGLINE_LEN-1);
                getConfigMemValueString((char *) file_external, file_size, "Language",
                    lang_strings[n].strname, language[lang_strings[n].code], MAX_CFGLINE_LEN-1, lang_strings[n].strdefault);
         
        } else {

            char *file_bin = (char *) language_ini_en_bin;
            file_size = language_ini_en_bin_size;

            switch(lang) {
                case 1: // sp
                    file_bin = (char *) language_ini_sp_bin;
                    file_size = language_ini_sp_bin_size;
                    break;
                case 2: // fr
                    file_bin = (char *) language_ini_fr_bin;
                    file_size = language_ini_fr_bin_size;
                    break;
                case 3: // it
                    file_bin = (char *) language_ini_it_bin;
                    file_size = language_ini_it_bin_size;
                    break;
                case 4: // nw
                    file_bin = (char *) language_ini_nw_bin;
                    file_size = language_ini_nw_bin_size;
                    break;
                case 5: // gm
                    file_bin = (char *) language_ini_gm_bin;
                    file_size = language_ini_gm_bin_size;
                    break;
                case 6: // po (portoguese)
                    file_bin = (char *) language_ini_po_bin;
                    file_size = language_ini_po_bin_size;
                    break;
                case 7: // ps
                    file_bin = (char *) language_ini_ps_bin;
                    file_size = language_ini_ps_bin_size;
                    break;
                case 8: // chs
                    file_bin = (char *) language_ini_chs_bin;
                    file_size = language_ini_chs_bin_size;
                    break;
                case 9: // cht
                    file_bin = (char *) language_ini_cht_bin;
                    file_size = language_ini_cht_bin_size;
                    break;
            }

            getConfigMemValueString((char *) file_bin, file_size, "Language",
                "REVERSE", get_reverse, 63, "OFF");

            if(!strcasecmp((const char *) get_reverse, "on")) reverse_language = 1;

            getConfigMemValueString((char *) file_bin, file_size, "Language",
                lang_strings[n].strname, language[lang_strings[n].code], MAX_CFGLINE_LEN-1, lang_strings[n].strdefault);
        }
    }

    if(file_external) free(file_external);

    return 0;

}

u32 get_system_language( u16 *lang )
{
	s32 ret = - 1;
	int reg = -1;
	u32 val_lang = 0x66;

	ret = sysFsOpen( "/dev_flash2/etc/xRegistry.sys", SYS_O_RDONLY, &reg, NULL, 0);
	if( ret == 0 )
	{
		int entry_name = 0x10002;
	
		while( 1 == 1 )
		{
			u64 pos;
			sysFsLseek( reg, (s64)entry_name, 0, &pos );
		
		//Leggo offset stringa
			u16 off_string = 0;
			u64 read;
			sysFsRead( reg, &off_string, 2, &read );
		
		//Leggo lunghezza stringa
			u16 len_string = 0;
			sysFsLseek( reg, (s64)off_string + (s64)0x12, 0, &pos );
			sysFsRead( reg, &len_string, 2, &read );	
		
		//Avanzo di uno e leggo stringa
			char string[ 256 ];
			memset( string, 0, sizeof(string) );
			sysFsLseek( reg, (s64)off_string + (s64)0x15, 0, &pos );	
			sysFsRead( reg, string, len_string, &read );
		
		//Comparo stringa
			if( strcmp(string, "/setting/system/language") == 0 )
			{
				sysFsLseek( reg, (s64)entry_name + (s64)0x7, 0, &pos );
				sysFsRead( reg, &val_lang, 4, &read );
				break;
			}
		//Non è uguale, avanzo l'offset da entry_name di 4 e leggo lunghezza data
			u16 len_data = 0;
			entry_name += 4;
			sysFsLseek( reg, (s64)entry_name, 0, &pos );	
			sysFsRead( reg, &len_data, 2, &read );
		
		//Vado al prossimo offset stringa
			entry_name = entry_name + 6 + len_data;
		
			if( off_string == 0xCCDD )
				break;
		}
	}
	
	if( lang == NULL )
		return val_lang;
		
	switch( val_lang )
	{
		case 0x0:
		//	strcpy( lang, "ger_language.ini" );
			*lang = 5;
			break;
		case 0x1:
		//	strcpy( lang, "eng-us_language.ini" );
			*lang = 0;
			break;
		case 0x2:
		//	strcpy( lang, "spa_language.ini" );
			*lang = 1;
			break;
		case 0x3:
		//	strcpy( lang, "fre_language.ini" );
			*lang = 2;
			break;
		case 0x4:
		//	strcpy( lang, "ita_language.ini" );
			*lang = 3;
			break;
		case 0x5:
		//	strcpy( lang, "dut_language.ini" ); //Olandese
			*lang = 0; /** not exist (for now) **/
			break;
		case 0x6:
		//	strcpy( lang, "por-por_language.ini" );
			*lang = 6; /** not exist (for now) **/
			break;
		case 0x7:
		//	strcpy( lang, "rus_language.ini" );
			*lang = 0; /** not exist (for now) **/
			break;
		case 0x8:
		//	strcpy( lang, "jap_language.ini" );
			*lang = 0; /** not exist (for now) **/
			break;
		case 0x9:
		//	strcpy( lang, "kor_language.ini" );
			*lang = 0; /** not exist (for now) **/
			break;
		case 0xA:
		//	strcpy( lang, "chi-tra_language.ini" );
			*lang = 9;
			break;
		case 0xB:
		//	strcpy( lang, "chi-sim_language.ini" );
			*lang = 8;
			break;
		case 0xC:
		//	strcpy( lang, "fin_language.ini" );
			*lang = 0; /** not exist (for now) **/
			break;
		case 0xD:
		//	strcpy( lang, "swe_language.ini" );
			*lang = 0; /** not exist (for now) **/
			break;	
		case 0xE:
		//	strcpy( lang, "dan_language.ini" );
			*lang = 0; /** not exist (for now) **/
			break;		
		case 0xF:
		//	strcpy( lang, "nor_language.ini" );
			*lang = 4;
			break;
		case 0x10:
		//	strcpy( lang, "pol_language.ini" );
			*lang = 0; /** not exist (for now) **/
			break;	
		case 0x11:
		//	strcpy( lang, "por-bra_language.ini" );
			*lang = 0; /** not exist (for now) **/
			break;
		case 0x12:
		//	strcpy( lang, "eng-uk_language.ini" );
			*lang = 0;
			break;		
		default:
		//	strcpy( lang, "language.ini" );
			*lang = 0; 
			break;
	}
	sysFsClose( reg );
	
	return val_lang;
}


void close_language(void)
{
    int n;
    
    if(!lang_inited) return;

    //free memory
    for (n = 0; n < LANGSTRINGS_COUNT; n++)
        free(language[n]);
    lang_inited = 0;
}

