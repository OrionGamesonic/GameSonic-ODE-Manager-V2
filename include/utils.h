#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <math.h>
#include "pad.h"

#include <sys/process.h>
#include <ppu-lv2.h>
#include <sys/stat.h>
#include <lv2/sysfs.h>

// for msgdialogs
#include <sysutil/sysutil.h>
#include <sysutil/msg.h>

#include <tiny3d.h>
#include <libfont.h>

#include "manager.h"

#define FONT_TTF -1
#define FONT_BUTTON  0

#define DT_DIR 1

//PARAM.SFO related
#define DATA_TABLE 			0xC
#define ENTRY_NUM 			0x10
#define START_DEF_TABLE 	0x14

#define OFF_FW_VER			0x42E
#define OFF_FW_VER_SPRX		0x27E



typedef struct {
    u32 flags;
    int splitted;
    char path_name[MAXPATHLEN];
	char title[64];
	char title_id[64];

} t_directories;

typedef struct {
    int index;
    u32 flags;
    char title[64];
    char title_id[64];
} entry_favourites;

typedef struct {
    u32 version;
    entry_favourites list[48];
} tfavourites;

#define MAX_DIRECTORIES 1024
#define MAX_CFGLINE_LEN 256

#define GAMELIST_FILTER 0xFFFF

#define GAMEBASE_MODE 0
#define HOMEBREW_MODE 2

extern int ndirectories;
extern t_directories directories[MAX_DIRECTORIES];

extern u32 fdevices;
extern u32 fdevices_old;
extern u32 forcedevices;

extern char hdd_folder[64];
extern char bluray_game[64];
extern float cache_need_free;

extern CobraManager manager;

void draw_cache_external();

void cls();
void cls2();

char * LoadFile(char *path, int *file_size);
int SaveFile(char *path, char *mem, int file_size);

void DrawDialogOK(char * str);
void DrawDialogOKTimer(char * str, float milliseconds);
void DrawDialogTimer(char * str, float milliseconds);

int DrawDialogYesNo(char * str);
int DrawDialogYesNo2(char * str);
int DrawDialogYesNoTimer(char * str, float milliseconds);
int DrawDialogYesNoTimer2(char * str, float milliseconds);


int parse_param_sfo(char * file, char *title_name);
int parse_ps3_disc(char *path, char * id);
int parse_param_sfo_id(char * file, char *title_id);
void utf8_to_ansi(char *utf8, char *ansi, int len);
void utf8_truncate(char *utf8, char *utf8_trunc, int len);

void sort_entries(t_directories *list, int *max);
void delete_entries(t_directories *list, int *max, u32 flag);
void fill_entries_from_device(char *path, t_directories *list, int *max, u32 flag, int sel);

void copy_from_selection(int game_sel);
void copy_from_bluray();
void delete_game(int game_sel);
void test_game(int game_sel);

void copy_usb_to_PS3ITA(char * path);

void DeleteDirectory(const char* path);

void FixDirectory(const char* path);

extern tfavourites favourites;
extern int havefavourites;

int getConfigMemValueInt (char* mem, int size, char* pchSection, char* pchKey, int iDefaultValue);
int getConfigMemValueString (char* mem, int size, char* pchSection, char* pchKey, char* pchValue, int iSize, char* pchDefaultValue);

void LoadFavourites(char * path, int mode);
void SaveFavourites(char * path, int mode);

void GetFavourites(int mode);
void SetFavourites(int mode);

void UpdateFavourites(t_directories *list, int nlist);
int TestFavouritesExits(char *id);
void AddFavourites(int indx, t_directories *list, int position_list);
int DeleteFavouritesIfExits(char *id);

int param_sfo_util(char * path, int patch_app);
int param_sfo_patch_category_to_cb(char * path_src, char *path_dst);

void reset_sys8_path_table();
void add_sys8_path_table(char * compare, char * replace);
void build_sys8_path_table();
void add_sys8_bdvd(char * bdvd, char * app_home);

u8 swap_half_word( u8 valore );
u16 swap_word( u16 valore );
u32 swap_double_word( u32 valore );
u16 find_table( u16 offset, const char *path );
int _parse_param_sfo_id( char *path_sfo, char* id );
int compare_fw_ver( char *self );
void patch_error_09( const char *path );
u64 string_to_ull( char *string );
int is_file_exist( char* path );

// console 

extern int con_x;
extern int con_y;

void initConsole();
void DbgHeader(char *str);
void DbgMess(char *str);
void DbgDraw();
void DPrintf(char *format, ...);

int unlink_secure(void *path);
int mkdir_secure(void *path);
int rmdir_secure(void *path);


#endif

