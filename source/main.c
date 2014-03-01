/*	
	GameSonic Ode Manager (c) 2014 GameSonic Team <http://www.ps3ita.it>

    (c) 2011 Hermes/Estwald <www.elotrolado.net>
    IrisManager (HMANAGER port) (c) 2011 D_Skywalk <http://david.dantoine.org>

    HMANAGER4 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    HMANAGER4 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    apayloadlong with HMANAGER4.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include <lv2/process.h>
#include <sys/file.h>
#include <ppu-lv2.h>
#include <sys/stat.h>
#include <lv2/sysfs.h>

#include <sysmodule/sysmodule.h>
#include <pngdec/pngdec.h>
#include <jpgdec/jpgdec.h>

#include <io/pad.h>

#include <tiny3d.h>
#include "libfont2.h"
#include "language.h"
#include "credits.h"
#include "main.h"

#include "ttf_render.h"
#include "gfx.h"
#include "utils.h"
#include "pad.h"

#include "ftp.h"
#include "archive_manager.h"
#include "osk_input.h"

#include "manager.h"

// include fonts

// font 2: 224 chr from 32 to 255, 16 x 32 pix 2 bit depth
#include "font_b.h"

#include "ftp_png_bin.h"
#include "folder_png_bin.h"
#include "file_png_bin.h"
#include "pkg_png_bin.h"
#include "self_png_bin.h"

#include "event_threads.h"

#include <ft2build.h>
#include <freetype/freetype.h> 
#include <freetype/ftglyph.h>
#include "ttf_render.h"

CobraManager manager;

CobraDisc *disc;

s32 out;


int ftp_inited = 0;

void UTF8_to_Ansi(char *utf8, char *ansi, int len); // from osk_input

u16 * ttf_texture;
char sfo_title[MAX_DIRECTORIES][64];
char *title_name;


u64 frame_count = 0;

int refresh_game = 0;


#define ROT_INC(x ,y , z) {x++; if(x > y) x = z;}
#define ROT_DEC(x ,y , z) {x--; if(x < y) x = z;}

int menu_screen = 0;
int num_box = 24;

int ndirectories = 0;

int currentdir = 0;
int currentgamedir = 0;
int int_currentdir = 0;
int page = 0;

u8 * png_texture = NULL;
PngDatas Png_datas[50];
u32 Png_offset[50];
int Png_iscover[50];

PngDatas Png_res[24];
u32 Png_res_offset[24];

extern char * language[];
int lang = 99;
char self_path[MAXPATHLEN]= "/"__MKDEF_MANAGER_FULLDIR__;

char temp_buffer[8192];


int LoadPNG(PngDatas *png, const char *filename)
{
    int ret;
    pngData png2;

    if(filename) ret = pngLoadFromFile(filename, &png2);
    else ret= pngLoadFromBuffer((const void *) png->png_in, png->png_size, &png2);

    png->bmp_out = png2.bmp_out;
    png->wpitch  = png2.pitch;
    png->width   = png2.width;
    png->height  = png2.height;

    return ret;
}

int LoadJPG(JpgDatas *jpg, char *filename)
{
    int ret;

    jpgData jpg2;

    if(filename) ret = jpgLoadFromFile(filename, &jpg2);
    else ret= jpgLoadFromBuffer((const void *) jpg->jpg_in, jpg->jpg_size, &jpg2);

    jpg->bmp_out = jpg2.bmp_out;
    jpg->wpitch  = jpg2.pitch;
    jpg->width   = jpg2.width;
    jpg->height  = jpg2.height;

    return ret;
}

void Load_PNG_resources()
{
    int i;

    for(i = 0; i < 24; i++) Png_res[i].png_in = NULL;
    for(i = 0; i < 50; i++) Png_iscover[i] = 0;

    // datas for PNG from memory

    Png_res[4].png_in   = (void *) ftp_png_bin;
    Png_res[4].png_size = ftp_png_bin_size;
	
    Png_res[7].png_in   = (void *) folder_png_bin;
    Png_res[7].png_size = folder_png_bin_size;

    Png_res[8].png_in   = (void *) file_png_bin;
    Png_res[8].png_size = file_png_bin_size;

    Png_res[9].png_in   = (void *) pkg_png_bin;
    Png_res[9].png_size = pkg_png_bin_size;

    Png_res[10].png_in   = (void *) self_png_bin;
    Png_res[10].png_size = self_png_bin_size;

    // load PNG from memory

    for(i = 0; i < 16; i++)
        if(Png_res[i].png_in != NULL)
            LoadPNG(&Png_res[i], NULL);

}

int LoadTexturePNG(char * filename, int index)
{
    
    u32 * texture_pointer2 = (u32 *) (png_texture + index * 4096 * 1024); // 4 MB reserved for PNG index

    // here you can add more textures using 'texture_pointer'. It is returned aligned to 16 bytes
   
    memset(&Png_datas[index], 0, sizeof(PngDatas));
	if(LoadPNG(&Png_datas[index], filename) <0) memset(&Png_datas[index], 0, sizeof(PngDatas));
 
    Png_offset[index] = 0;
       
    if(Png_datas[index].bmp_out) {

        if(index < num_box && Png_datas[index].wpitch * Png_datas[index].height > 4096 * 1024) { //  too big!
            memset(texture_pointer2, 0, 64 * 64 * 4);
            Png_datas[index].wpitch = 64 * 4;
            Png_datas[index].height = Png_datas[index].width = 64;
        } else
            memcpy(texture_pointer2, Png_datas[index].bmp_out, Png_datas[index].wpitch * Png_datas[index].height);
        
        free(Png_datas[index].bmp_out);

        Png_datas[index].bmp_out= texture_pointer2;

        Png_offset[index] = tiny3d_TextureOffset(Png_datas[index].bmp_out);      // get the offset (RSX use offset instead address)

     return 0;
     } else {

         // fake PNG
        Png_datas[index].bmp_out= texture_pointer2;

        Png_offset[index] = tiny3d_TextureOffset(Png_datas[index].bmp_out);

        int n;
        u32 * text = texture_pointer2;

        Png_datas[index].width = Png_datas[index].height = 64;
        
        Png_datas[index].wpitch = Png_datas[index].width * 4;
       
        for (n = 0; n < Png_datas[index].width * Png_datas[index].height; n++) *text++ = 0xff000000;
       
     }

    return -1;
}


int LoadTextureJPG(char * filename, int index )
{
    
    u32 * texture_pointer2 = (u32 *) (png_texture + index * 4096 * 1024); // 4 MB reserved for PNG index

    // here you can add more textures using 'texture_pointer'. It is returned aligned to 16 bytes
   
    memset(&Png_datas[index], 0, sizeof(PngDatas));
	if(LoadJPG((JpgDatas *)&Png_datas[index], filename) <0) memset(&Png_datas[index], 0, sizeof(PngDatas));
 
    Png_offset[index] = 0;
       
    if(Png_datas[index].bmp_out) {
        
        if(index < num_box && Png_datas[index].wpitch * Png_datas[index].height > 4096 * 1024) { //  too big!
            memset(texture_pointer2, 0, 64 * 64 * 4);
            Png_datas[index].wpitch = 64 * 4;
            Png_datas[index].height = Png_datas[index].width = 64;
        } else
            memcpy(texture_pointer2, Png_datas[index].bmp_out, Png_datas[index].wpitch * Png_datas[index].height);
        
        free(Png_datas[index].bmp_out);

        Png_datas[index].bmp_out= texture_pointer2;

        Png_offset[index] = tiny3d_TextureOffset(Png_datas[index].bmp_out);      // get the offset (RSX use offset instead address)

     return 0;
     } else {

         // fake PNG
        Png_datas[index].bmp_out= texture_pointer2;

        Png_offset[index] = tiny3d_TextureOffset(Png_datas[index].bmp_out);

        int n;
        u32 * text = texture_pointer2;

        Png_datas[index].width = Png_datas[index].height = 64;
        
        Png_datas[index].wpitch = Png_datas[index].width * 4;
       
        for (n = 0; n < Png_datas[index].width * Png_datas[index].height; n++) *text++ = 0xff000000;
       
     }

    return -1;
}


void get_title()
{

	int i;
	
    for( i = 0; i < manager.num_iso && i < MAX_DIRECTORIES; i++ )


		parse_param_sfo( manager.iso[i].sfo_path, sfo_title[i] );

}

static volatile int break_get_games = 0;

void get_games_2(void *empty) 
{
    int n;

    if(int_currentdir < 0 ||  int_currentdir >= ndirectories) int_currentdir = 0;

    for(n = 0; n < num_box; n++) {
        if(break_get_games) return;
        if((int_currentdir + n) < ndirectories) {
            
            if(!Png_offset[n]) {

            		    if (manager.iso[int_currentdir + n].type == 1)

            			LoadTexturePNG("/dev_bdvd/PS3_GAME/USRDIR/PSX.PNG", n);

            		else

            			if (manager.iso[int_currentdir + n].type == 2)

            			 LoadTexturePNG("/dev_bdvd/PS3_GAME/USRDIR/PS2.PNG", n);
                	else

            			if (manager.iso[int_currentdir + n].type == 4)

            			 LoadTexturePNG("/dev_bdvd/PS3_GAME/USRDIR/BLURAY.PNG", n);
                	else
            			if (manager.iso[int_currentdir + n].type == 5)

            			 LoadTexturePNG("/dev_bdvd/PS3_GAME/USRDIR/DVD.PNG", n);
                	else
            			if (manager.iso[int_currentdir + n].type == 3)

            		    LoadTexturePNG(manager.iso[int_currentdir + n].png_path, n);

                else
					if(LoadTexturePNG(manager.iso[int_currentdir + n].png_path, n) < 0) ;
            }
            
       } else 
		   Png_offset[n] = 0;
    }
}

void get_games()
{
    int n;

    break_get_games = 1; // for short wait
    wait_event_thread(); // wait previous event thread function
    break_get_games = 0;
    int_currentdir = currentdir;

    // reset icon datas
    for(n = 0; n < 50; n++) Png_offset[n] = 0;
    // program new event thread function
    event_thread_send(0x555ULL, (u64) get_games_2, 0); 
}


void DrawCenteredBar2D(float y, float w, float h, u32 rgba)
{
    float x = (848.0f - w)/ 2.0f;

    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x    , y    , 1.0f);
    tiny3d_VertexColor(rgba);

    tiny3d_VertexPos(x + w, y    , 1.0f);

    tiny3d_VertexPos(x + w, y + h, 1.0f);

    tiny3d_VertexPos(x    , y + h, 1.0f);
    tiny3d_End();
}


void LoadTexture()
{
    int i;

    u32 * texture_mem = tiny3d_AllocTexture(100*1024*1024); // alloc 100MB of space for textures (this pointer can be global)    

    u32 * texture_pointer; // use to asign texture space without changes texture_mem

    if(!texture_mem) return; // fail!

    texture_pointer = texture_mem;

    ResetFont();

    //debug font
    texture_pointer = (u32 *) AddFontFromBitmapArray((u8 *) font_b, (u8 *) texture_pointer, 32, 255, 16, 32, 2, BIT0_FIRST_PIXEL);

/*
    TTFLoadFont(NULL, (void *) comfortaa_ttf_bin, comfortaa_ttf_bin_size);
    texture_pointer = (u32 *) AddFontFromTTF((u8 *) texture_pointer, 32, 255, 32, 32, TTF_to_Bitmap);
    texture_pointer = (u32 *) AddFontFromTTF((u8 *) texture_pointer, 32, 255, 20, 20, TTF_to_Bitmap);
    TTFUnloadFont();


    //new button font
    TTFLoadFont(NULL, (void *) comfortaa_bold_ttf_bin, comfortaa_bold_ttf_bin_size);
    texture_pointer = (u32 *) AddFontFromTTF((u8 *) texture_pointer, 32, 255, 24, 24, TTF_to_Bitmap);
    TTFUnloadFont();
*/
    
    {
        struct stat s;
        sprintf(temp_buffer, "%s/font.ttf", self_path);
        
        if(stat(temp_buffer, &s)<0 || TTFLoadFont(0, temp_buffer, NULL, 0)!=0) {//
            if(TTFLoadFont(0, "/dev_flash/data/font/SCE-PS3-SR-R-JPN.TTF", NULL, 0)!=0)
                if(TTFLoadFont(0, "/dev_flash/data/font/SCE-PS3-NR-R-JPN.TTF", NULL, 0)!=0)
                    exit(0);
                //TTFLoadFont(NULL, (void *) comfortaa_bold_ttf_bin, comfortaa_bold_ttf_bin_size);
        }
    

        TTFLoadFont(1, "/dev_flash/data/font/SCE-PS3-DH-R-CGB.TTF", NULL, 0);
        TTFLoadFont(2, "/dev_flash/data/font/SCE-PS3-SR-R-LATIN2.TTF", NULL, 0);
        TTFLoadFont(3, "/dev_flash/data/font/SCE-PS3-YG-R-KOR.TTF", NULL, 0);
    

    }
    
    Load_PNG_resources();

    for(i = 0; i < 24; i++) {
       
        if(Png_res[i].png_in == NULL) continue;

        Png_res_offset[i] = 0;
       
        if(Png_res[i].bmp_out) {

            memcpy(texture_pointer, Png_res[i].bmp_out, Png_res[i].wpitch * Png_res[i].height);
            
            free(Png_res[i].bmp_out); // free the PNG because i don't need this datas

            Png_res_offset[i] = tiny3d_TextureOffset(texture_pointer);      // get the offset (RSX use offset instead address)

            texture_pointer += ((Png_res[i].wpitch * Png_res[i].height + 15) & ~15) / 4; // aligned to 16 bytes (it is u32) and update the pointer
         }
    }


    ttf_texture = (u16 *) texture_pointer;

    texture_pointer += 1024 * 16;
	
    texture_pointer = (u32 *) init_ttf_table((u16 *) texture_pointer);

    png_texture = (u8 *) texture_pointer;
}


int background_sel = 0;
#define MAX_COL	16

u32 background_colors[ MAX_COL ] = {
	0xff6699FF, //azzurro
	0xff556B2F, //Oliva scuro
	0xffFE5DE5, //rosa
	0xff000033, //blu scuro
    0xff80804F,
    0xff10000F,
    0xff606060,
    0xff904F80,
	// new hermes colors
    0xff6699FF,	//azzurro 
    0xff808000, //Oliva
	0xffFE5DE5, //rosa
	0xff000000, //nero
	0xff80804F,
    0xff0040CF,
    0xff300060,
    0xff904f80
};

void cls()
{
    tiny3d_Clear(background_colors[background_sel], TINY3D_CLEAR_ALL);
        
		
    // Enable alpha Test
    tiny3d_AlphaTest(1, 0x10, TINY3D_ALPHA_FUNC_GEQUAL);

    // Enable alpha blending.
            tiny3d_BlendFunc(1, TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA | TINY3D_BLEND_FUNC_SRC_ALPHA_SRC_ALPHA,
                TINY3D_BLEND_FUNC_DST_RGB_ONE_MINUS_SRC_ALPHA | TINY3D_BLEND_FUNC_DST_ALPHA_ZERO,
                TINY3D_BLEND_RGB_FUNC_ADD | TINY3D_BLEND_ALPHA_FUNC_ADD);
    reset_ttf_frame();
}

void cls2()
{
    tiny3d_Clear(0xff000000, TINY3D_CLEAR_ALL);
        
    // Enable alpha Test
    tiny3d_AlphaTest(1, 0x10, TINY3D_ALPHA_FUNC_GEQUAL);

   // Enable alpha blending.
            tiny3d_BlendFunc(1, TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA | TINY3D_BLEND_FUNC_SRC_ALPHA_SRC_ALPHA,
                TINY3D_BLEND_FUNC_DST_RGB_ONE_MINUS_SRC_ALPHA | TINY3D_BLEND_FUNC_DST_ALPHA_ZERO,
                TINY3D_BLEND_RGB_FUNC_ADD | TINY3D_BLEND_ALPHA_FUNC_ADD);
    reset_ttf_frame();
}

/******************************************************************************************************************************************************/

// manager


int videoscale_x = 0;
int videoscale_y = 0;

int flash;

int select_px = 0;
int select_py = 0;

int select_option = 0;

static int exit_program = 0;

#define ROUND_UP12(x) ((((x)+11)/12)*12)
#define ROUND_UP24(x) ((((x)+23)/24)*24)
#define ROUND_UP48(x) ((((x)+47)/48)*48)

void draw_screen12(float x, float y);
void draw_screen24(float x, float y);
void draw_screen48(float x, float y);
void draw_gbloptions(float x, float y);
void draw_setup_video(float x, float y);


struct {
    int videoscale_x[4];
    int videoscale_y[4];
    int background_sel;
    char pad[156];
} manager_oldcfg;

struct {
    int videoscale_x[4];
    int videoscale_y[4];
    int background_sel;
    u16 language;
    char pad[150];
    u32 opt_flags;
    int num_box;
} manager_cfg;

int inited = 0;

#define INITED_SPU          2
#define INITED_GCM_SYS      8
#define INITED_IO          16
#define INITED_PNGDEC      32
#define INITED_FS          64
#define INITED_JPGDEC      128

int set_install_pkg = 0;

void fun_exit()
{
    // multiple calls? (Yes!)
    static int one = 1;
    if(!one) return;
    one = 0;
    
    cobra_manager_free(&manager);

    event_threads_finish();

    close_language();
    TTFUnloadFont();

    ftp_deinit();

    if(inited & INITED_GCM_SYS) sysModuleUnload(SYSMODULE_GCM_SYS);
    if(inited & INITED_IO)      sysModuleUnload(SYSMODULE_IO);
    if(inited & INITED_PNGDEC)  sysModuleUnload(SYSMODULE_PNGDEC);
    if(inited & INITED_JPGDEC)  sysModuleUnload(SYSMODULE_JPGDEC);
    if(inited & INITED_FS)      sysModuleUnload(SYSMODULE_FS);
    sysModuleUnload(SYSMODULE_SYSUTIL);
    
    inited = 0;
    
    ioPadEnd();
    
//    if(game_cfg.direct_boot == 555 )
//        sysProcessExitSpawn2("/dev_bdvd/PS3_GAME/USRDIR/EBOOT.BIN", NULL, NULL, NULL, 0, 3071, SYS_PROCESS_SPAWN_STACK_SIZE_1M);

}

void LoadManagerCfg()
{

    sprintf(temp_buffer, "%s/config/manager_setup.bin", self_path);

    int file_size;
    char *file = LoadFile(temp_buffer, &file_size);
    
    if(file)
    {
        if(file_size != sizeof(manager_cfg))
            file_size = sizeof(manager_oldcfg); // safe

        memcpy(&manager_cfg, file, file_size);
        free(file);
    }

    background_sel = manager_cfg.background_sel;
    
// grid
	num_box = manager_cfg.num_box;

    switch(Video_Resolution.height) {
        case 480:
            videoscale_x = manager_cfg.videoscale_x[0];
            videoscale_y = manager_cfg.videoscale_y[0];
            break;
       case 576:
            videoscale_x = manager_cfg.videoscale_x[1];
            videoscale_y = manager_cfg.videoscale_y[1];
            break;
       case 720:
            videoscale_x = manager_cfg.videoscale_x[2];
            videoscale_y = manager_cfg.videoscale_y[2];
            break;
       default:
            videoscale_x = manager_cfg.videoscale_x[3];
            videoscale_y = manager_cfg.videoscale_y[3];
            break;
    }

	lang = manager_cfg.language;
	if( lang == 99 )
		get_system_language( &manager_cfg.language );
		
    sprintf(temp_buffer, "%s/config/language.ini", self_path);
    open_language(manager_cfg.language, temp_buffer);
    
}

void auto_ftp(void)
{
    static int one = 1;
    if(!one) return;

    one= 0;
    if ( manager_cfg.opt_flags & OPTFLAGS_FTP) // maybe we need add an icon to user...
    {
		int r = ftp_init();
        if(r == 0)
        {
            ftp_inited = 1; //DrawDialogOK("FTP Service init on boot: OK");
            
        } 
        else 
        {
			if(r == -1) DrawDialogOKTimer(language[DRAWFTP_ENETINITIALIZE], 2000.0f);
			else if(r == -2) DrawDialogOKTimer(language[DRAWFTP_ENETCTLINIT], 2000.0f);
			else if(r == -3) DrawDialogOKTimer(language[DRAWFTP_ENETCTLGETINFO], 2000.0f);
			else if(r == -4) DrawDialogOKTimer(language[DRAWFTP_ECONNECTION], 2000.0f);
			else DrawDialogOK(language[DRAWFTP_EUNKNOWN]);
        }
    }
}

int SaveManagerCfg()
{
    sprintf(temp_buffer, "%s/config/manager_setup.bin", self_path);
    return SaveFile(temp_buffer, (char *) &manager_cfg, sizeof(manager_cfg));
}

void video_adjust()
{

    SetCurrentFont(FONT_TTF);
    while(1) {

        double sx = (double) Video_Resolution.width;
        double sy = (double) Video_Resolution.height;
        double psx = (double) (1000 + videoscale_x)/1000.0;
        double psy = (double) (1000 + videoscale_y)/1000.0;
        
        tiny3d_UserViewport(1, 
            (float) ((sx - sx * psx) / 2.0), // 2D position
            (float) ((sy - sy * psy) / 2.0), 
            (float) ((sx * psx) / 848.0),    // 2D scale
            (float) ((sy * psy) / 512.0),
            (float) ((sx / 1920.0) * psx),  // 3D scale
            (float) ((sy / 1080.0) * psy));

        cls();
        
        DrawAdjustBackground(0xffffffff) ; // light blue 

        update_twat();
        SetFontSize(16, 24);
        SetFontColor(0xffffffff, 0x0);

        SetFontAutoCenter(1);

        DrawFormatString(0, (512 - 24)/2 - 64, "%s", language[VIDEOADJUST_POSITION]);

        DrawFormatString(0, (512 - 24)/2, language[VIDEOADJUST_SCALEINFO], videoscale_x, videoscale_y);

        DrawFormatString(0, (512 - 24)/2 + 64, "%s", language[VIDEOADJUST_EXITINFO]);
       
        DrawFormatString(0, (512 - 24)/2 + 96, "%s", language[VIDEOADJUST_DEFAULTS]);

        // Warning!! don´t traslate this string!
        DrawFormatString(0, (512 - 24)/2 + 128, "%s", "Press [] to English language");

        SetFontAutoCenter(0);

        tiny3d_Flip();

        ps3pad_read();

        if(!(frame_count & 3)) {
            if(old_pad & BUTTON_UP) {if(videoscale_y > -179) videoscale_y--;}
            if(old_pad & BUTTON_DOWN) {if(videoscale_y < 10) videoscale_y++;}
            if(old_pad & BUTTON_LEFT) {if(videoscale_x > -199) videoscale_x--;}
            if(old_pad & BUTTON_RIGHT) {if(videoscale_x < 10) videoscale_x++;}
        }
        
        if(new_pad & BUTTON_SQUARE) {
            manager_cfg.language = 0;
            sprintf(temp_buffer, "%s/config/language.ini", self_path);
            open_language(manager_cfg.language, temp_buffer);
        }

        if(new_pad & BUTTON_CROSS) {

            switch(Video_Resolution.height) {
                case 480:
                    manager_cfg.videoscale_x[0] = videoscale_x;
                    manager_cfg.videoscale_y[0] = videoscale_y;
                    break;
               case 576:
                    manager_cfg.videoscale_x[1] = videoscale_x;
                    manager_cfg.videoscale_y[1] = videoscale_y;
                    break;
               case 720:
                    manager_cfg.videoscale_x[2] = videoscale_x;
                    manager_cfg.videoscale_y[2] = videoscale_y;
                    break;
               default:
                    manager_cfg.videoscale_x[3] = videoscale_x;
                    manager_cfg.videoscale_y[3] = videoscale_y;
                    break;
            }
            
      
            if(SaveManagerCfg() == 0) {
                sprintf(temp_buffer, "manager_setup.bin\n\n%s", language[GLOBAL_SAVED]);
                DrawDialogOK(temp_buffer);
            }

            break;
        }

        if(new_pad & BUTTON_CIRCLE) {videoscale_x = videoscale_y = -120;}

        frame_count++;
    }
	
}


int test_ftp_working()
{
    if(get_ftp_activity()) {
        if(DrawDialogYesNo(language[DRAWFTP_EINTACTIVITY]) == 1) {
            ftp_deinit();
            ftp_inited = 0;
            return 0;
            
        } else return 1;
    } return 0;
}


int get_console_id( u8 *buf )
{
	lv2syscall1( 870, (u64)buf );
	return_to_user_prog( int );
}


s32 main(s32 argc, const char* argv[])
{
    int n;

    event_threads_init();

    atexit(fun_exit);


    if(sysModuleLoad(SYSMODULE_FS) ==0)      inited|= INITED_FS;      else exit(0);
    if(sysModuleLoad(SYSMODULE_PNGDEC) ==0)  inited|= INITED_PNGDEC;  else exit(0);
    if(sysModuleLoad(SYSMODULE_JPGDEC) ==0)  inited|= INITED_JPGDEC;  else exit(0);
    if(sysModuleLoad(SYSMODULE_IO) ==0)      inited|= INITED_IO;      else exit(0);
    if(sysModuleLoad(SYSMODULE_GCM_SYS) ==0) inited|= INITED_GCM_SYS; else exit(0);
	
    sysModuleLoad(SYSMODULE_SYSUTIL);

	//Create APPID folder on /dev_usb000
	
    if(argc>0 && argv ) 
    {
    
        if(!strncmp(argv[0], "/dev_usb000/", 15)) {
            int n;

            strcpy(self_path, argv[0]);

            n= 15; while(self_path[n] != '/' && self_path[n] != 0) n++;

            if(self_path[n] == '/') {
                self_path[n] = 0;
            }
        }
		mkdir_secure(self_path);
    }
   

    // ADVERTENCIA: este codigo no debe ser removido de aqui para evitar pantalla negra por los parches (problemas de cache)

    tiny3d_Init(1024*1024);
	ioPadInit(7);
    usleep(250000);
    
	//Subfolders
	
	sprintf(temp_buffer, "%s/config", self_path);
    mkdir_secure(temp_buffer);

    sprintf(temp_buffer, "%s/self", self_path);
    mkdir_secure(temp_buffer);
		
	// Load texture

    LoadTexture();

    init_twat();

    // initialize manager conf
    
    memset(&manager_cfg, 0, sizeof(manager_cfg));
    
	for(n=0; n<4; n++) manager_cfg.videoscale_x[n] = 1024;

	manager_cfg.background_sel = 11;
	manager_cfg.num_box = 24;
	//load cfg, language strings
	LoadManagerCfg();
	
	if(videoscale_x >= 1024 && is_file_exist("/dev_usb000") == 1 )
	{
		videoscale_x = videoscale_y = 0;
		video_adjust();
	}
	else if( is_file_exist("/dev_usb000") == 0 )
		videoscale_x = videoscale_y = -120;

    double sx = (double) Video_Resolution.width;
    double sy = (double) Video_Resolution.height;
    double psx = (double) (1000 + videoscale_x)/1000.0;
    double psy = (double) (1000 + videoscale_y)/1000.0;
    
    tiny3d_UserViewport(1, 
        (float) ((sx - sx * psx) / 2.0), // 2D position
        (float) ((sy - sy * psy) / 2.0), 
        (float) ((sx * psx) / 848.0),    // 2D scale
        (float) ((sy * psy) / 512.0),
        (float) ((sx / 1920.0) * psx),  // 3D scale
        (float) ((sy / 1080.0) * psy));

    select_px = select_py = 0;

 //Init cobra_manager and fill the structure
    cobra_manager_init(&manager);
    ndirectories = manager.num_iso;
    get_title();
	get_games();


/** Mmm non abbiamo accesso in lettura alla dev_flash2 **/
    // read xRegitry datas
/*    read_from_registry();

    if(sys_parental_level !=0 && sys_parental_level < 9) options_locked = 1;
*/

	
    while(exit_program != 1 ) {

        float x = 0.0f, y = 0.0f;
    
        flash = (frame_count >> 5) & 1;
        frame_count++;
        
        if( refresh_game == 1 && menu_screen == 0 )
        {
			select_option = 0;
			select_px = select_py = 0;
			currentdir = 0;
			get_games();
			refresh_game = 0;
		}

        if(tiny3d_MenuActive()) frame_count = 32; // to avoid the access to hdd when menu is active
       
        cls();
        update_twat();
		
        x= (848 - 640) / 2; y=(512 - 360) / 2;
        x= 28; y= 0;

 /*       if((old_pad & (BUTTON_L2 | BUTTON_R2 | BUTTON_START)) == (BUTTON_L2 | BUTTON_R2 | BUTTON_START)) {
            
            videoscale_x = videoscale_y = 0;
            video_adjust();
        }
*/
        // paranoid checks
		if( num_box == 48 )
        {
			if(select_px < 0 || select_px > 7) select_px = 0;
			if(select_py < 0 || select_py > 5) select_py = 0;
		}
        else if( num_box == 24 )
        {
			if(select_px < 0 || select_px > 5) select_px = 0;
			if(select_py < 0 || select_py > 3) select_py = 0;
		}
		else
		{
			if(select_px < 0 || select_px > 3) select_px = 0;
			if(select_py < 0 || select_py > 2) select_py = 0;
		}
        
        if(currentdir < 0 || currentdir >= ndirectories) currentdir = 0;
        if(currentgamedir < 0 || currentgamedir >= ndirectories) currentgamedir = 0;
        
        

        switch(menu_screen) {
            case 0:
				if(num_box == 24)
					draw_screen24(x, y);
				else if(num_box == 48)
					draw_screen48(x, y);
				else
					draw_screen12(x, y);
				break;
            case 1:
				draw_gbloptions(x, y);
                break;
			case 2:
				draw_setup_video( x, y );
				break;
            default:
                menu_screen = 0;
                break;
        }

        auto_ftp(); // auto enable the ftp                                              
     }

	return 0;
}


inline int get_currentdir(int i)
{
    return (currentdir + i);
}

inline int get_int_currentdir(int i)
{
	return (int_currentdir + i);
}


void draw_screen12(float x, float y)
{
	
    int i, n, m;
    float x2;
    int selected = select_px + select_py * 4;

// TOP PANEL
    DrawBox(x, y, 0, 200 * 4 - 8, 18, 0x00000028);

// N° PAGE,GAMES..
	page = (ROUND_UP12(ndirectories)/12);

	SetCurrentFont(FONT_TTF);

   	if(background_sel == 11) SetFontColor(0xfff500ff, 0x00000000); //cobra style
    else SetFontColor(0xffffffff, 0x00000000);
    SetFontSize(16, 18);
    SetFontAutoCenter(0);

    DrawFormatString(x, y + 1, " %s %i/%i (%i %s)", language[DRAWSCREEN_PAGE], currentdir/12 + 1, page, ndirectories, language[DRAWSCREEN_GAMES] );
	
	m = selected;
	y += 24;

    i = 0;
    int flash2 = 0;
    #define MAX_FLASH 32
    if (frame_count & MAX_FLASH) flash2= (MAX_FLASH-1) - (frame_count & (MAX_FLASH-1)); else flash2= (frame_count & (MAX_FLASH-1));
    for(n = 0; n < 3; n++) 
        for(m = 0; m < 4; m++) 
        {
            int f = (select_px == m && select_py == n);
            float f2 = (int) f;
            
            //BACKGROUND HERMES STYLE 
            if(background_sel > 7) f2 = 3* ((float) (flash2 *(select_px == m && select_py == n)))/((float) MAX_FLASH);
            
            //DRAW EMPTY BOX
            if(background_sel == 11) DrawBox(x + 200 * m - 4 * f2, y + n * 150 - 4 * f2, 0, 192 + 8 * f2, 142 + 8 * f2, 0x4f4f4f28 - (flash2 * f) ); //cobra style
			else DrawBox(x + 200 * m - 4 * f2, y + n * 150 - 4 * f2, 0, 192 + 8 * f2, 142 + 8 * f2, 0x00000028 + (flash2 * f) );
            
            //DRAW ICON0.PNG
            if(Png_offset[i]) 
            {
				tiny3d_SetTextureWrap(0, Png_offset[i], Png_datas[i].width, 
					Png_datas[i].height, Png_datas[i].wpitch, 
						TINY3D_TEX_FORMAT_A8R8G8B8,  TEXTWRAP_CLAMP, TEXTWRAP_CLAMP,1);
             
               DrawTextBox(x + 200 * m - 4 * f2, y + n * 150 - 4 * f2, 0, 192 + 8 * f2, 142 + 8 * f2, (0xffffffff- (MAX_FLASH * 2 * f)) + (flash2 * 2 * f));
            }
			i++;   
        }
 
	i = selected;

    SetCurrentFont(FONT_TTF);
    SetFontSize(18, 20);
    SetFontAutoCenter(0);
	
// DRAW TEXT OPTION							
	if(background_sel == 11) SetFontColor(0x84c225ff, 0x00000000); //cobra style

	x2= DrawFormatString(1024, 0, " %s ", language[DRAWSCREEN_STGLOPT]);
	DrawFormatString(x + 4 * 200 - (x2 - 1024) - 5 , y + 3 * 150 + 11, " %s ", language[DRAWSCREEN_STGLOPT]);
			    
// DRAW TEXT PLAY
	
    if( background_sel == 11 && Png_offset[i] ) //cobra style
    {
        DrawBox(x + 200 * select_px - 8 + (200 - 24 * 8)/2, y + select_py * 150 - 4 + 150 - 40, 0, 200, 40, 0x000000a0);
		SetFontColor(0xfefefeff, 0x00000000); 
		SetCurrentFont(FONT_TTF);
		SetFontSize(24, 24);
		x2 = DrawFormatString(x + 200 * select_px - 4 + (200 - 24 * 8)/2, y + select_py * 150 - 4 + 150 - 40, "  %s", language[DRAWSCREEN_PLAY]);
    }
    else if(Png_offset[i])
    {
		DrawBox(x + 200 * select_px - 8 + (200 - 24 * 8)/2, y + select_py * 150 - 4 + 150 - 40, 0, 200, 40, 0x404040a0);
		SetFontColor(0xffffffff, 0x00000000); 
		SetCurrentFont(FONT_TTF);
		SetFontSize(24, 24);
		x2 = DrawFormatString(x + 200 * select_px - 4 + (200 - 24 * 8)/2, y + select_py * 150 - 4 + 150 - 40, "  %s", language[DRAWSCREEN_PLAY]);
	} 
    
    
// DRAW BOTTOM PANEL
    DrawBox(x, y + 3 * 150, 0, 200 * 4 - 8, 40, 0x00000028);

// DRAW GAME NAME
    if(Png_offset[i]) 
    {
		if(background_sel == 11) SetFontColor(0x84c225ff, 0x00000000); //cobra style
		else SetFontColor(0xffffffff, 0x00000000);
		SetFontSize(24, 26);
		SetFontAutoCenter(0);
		if (manager.iso[i].sfo_path <  manager.iso[i].filename)
		DrawFormatString(x, y + 3 * 150 + 8, " %s", manager.iso[i].filename);
		else
		DrawFormatString(x, y + 3 * 150 + 8, " %s", sfo_title[(currentdir + i)]);


    }

// DRAW GAME SIZE
	if ( (manager.info.major > 1 || manager.info.minor >= 3) && Png_offset[i] )
	{
		if(background_sel == 11) SetFontColor(0xfff500ff, 0x00000000); //cobra style
		else SetFontColor(0xffffffff, 0x00000000);
		SetFontSize(16, 18);
		SetFontAutoCenter(0);
		
		x2= DrawFormatString(1024, 0, "%s: %.d MB ", language[DRAWSCREEN_GAMESIZE], manager.iso[(currentdir + i)].size / 1048576 );
		DrawFormatString(x + 4 * 200 - (x2 - 1024) - 5 , y - 24 + 1, "%s: %d MB ", language[DRAWSCREEN_GAMESIZE], manager.iso[(currentdir + i)].size / 1048576 );
	}
	
// FTP ICON
  /*  if(flash && ftp_inited) {
        tiny3d_SetTextureWrap(0, Png_res_offset[4], Png_res[4].width, 
            Png_res[4].height, Png_res[4].wpitch, 
            TINY3D_TEX_FORMAT_A8R8G8B8,  TEXTWRAP_CLAMP, TEXTWRAP_CLAMP,1);

        DrawTextBox(200 * 4 -32, y - 30, 0, 64, 32, 0xffffffff); 
    }
     */

    //SetCurrentFont(FONT_DEFAULT);

    tiny3d_Flip();
    ps3pad_read();

// RETURN TO XMB
    if(new_pad & BUTTON_TRIANGLE) 
    {
		if(!test_ftp_working()) 
			if(DrawDialogYesNo(language[DRAWSCREEN_EXITXMB])==1) {exit_program = 1; return;}
    }

// RUN GAME
    if(new_pad & (BUTTON_CROSS | BUTTON_CIRCLE)) 
    {
        i = selected;
     
        if(test_ftp_working()) return;

		if(Png_offset[i])
		{ 
			cobra_iso_run(&manager.iso[currentdir + i]);
			if( cobra_manager_eject(&manager) == 0 )
			{
				if( manager.info.minor >= 6 )
					DrawDialogOK( language[DRAWSCREEN_EJECTINFO] );
				else
					DrawDialogOK( language[DRAWSCREEN_MCUOLD] );
			}
			
			exit_program = 1;
		}
    } 

// MENU' OPTIONS
    if(new_pad & BUTTON_START) 
    {
		select_option = 0;
        menu_screen = 1; 
        return;
    }

    static int auto_up = 0, auto_down = 0, auto_left = 0, auto_right = 0;

    AUTO_BUTTON_REP(auto_up, BUTTON_UP)
    AUTO_BUTTON_REP(auto_down, BUTTON_DOWN)
    AUTO_BUTTON_REP(auto_left, BUTTON_LEFT)
    AUTO_BUTTON_REP(auto_right, BUTTON_RIGHT)

    if(new_pad & BUTTON_UP) {
        select_py--;

        auto_up = 1;
        frame_count = 32;

        if(select_py < 0) {
            
            select_py = 2;
            
            if(currentdir >= 12) {currentdir -= 12; get_games();}
			else {currentdir = ROUND_UP12(ndirectories) - 12; get_games();}
        }
        return;
    }

    if(new_pad & BUTTON_DOWN) {
        
        select_py++;

        auto_down = 1;
        frame_count = 32;
        
        if(select_py > 2) {
           
            select_py = 0; 
            
            if(currentdir < (ROUND_UP12(ndirectories) - 12)) {currentdir += 12; get_games();}
            else {currentdir = 0; get_games();}
        }
        return;
    }

    if(new_pad & BUTTON_LEFT) {
        
        select_px--;

        auto_left = 1;
        frame_count = 32;

        if(select_px < 0) {
            
            select_px = 3;
            
            if(currentdir >= 12) {currentdir -= 12; get_games();}
            else {currentdir = ROUND_UP12(ndirectories) - 12; get_games();}
        }
        return;
    }

    if(new_pad & BUTTON_RIGHT) {
        
        select_px++; 

        auto_right = 1;
        frame_count = 32;

        if(select_px > 3) {
            
            select_px = 0; 
            
            if(currentdir < (ROUND_UP12(ndirectories) - 12)) { currentdir += 12; get_games();}
			else {currentdir = 0; get_games();}
        }
        return;
    }

    if(new_pad & BUTTON_L1) //change page
    {
        frame_count = 32;
            
        if(currentdir >= 12) {currentdir -= 12; get_games();}
        else {currentdir = ROUND_UP12(ndirectories) - 12; get_games();}

        return;
    }

    if(new_pad & BUTTON_R1) //change page
    {
        //maybe wait some seconds here...
        frame_count = 32;
            
        if(currentdir < (ROUND_UP12(ndirectories) - 12)) {currentdir += 12; get_games();}
        else {currentdir = 0; get_games();}

        return;
    }
}

void draw_screen24(float x, float y)
{
	
#define div		/1.5

    int i, n, m;
    float x2, y3, x3;
    int selected = select_px + select_py * 6;

    x3 = x + ((x * 1.5) / 2) - 9;
    x = x - 1;

// TOP PANEL
    DrawBox(x, y, 0, 200 * 4 - 6, 20, 0x00000028);

// N° PAGE,GAMES..
	page = (ROUND_UP24(ndirectories)/24);

	SetCurrentFont(FONT_TTF);

	if(background_sel == 11) SetFontColor(0xfff500ff, 0x00000000); //cobra style
    else SetFontColor(0xffffffff, 0x00000000);
    SetFontSize(16, 18);
    SetFontAutoCenter(0);

    DrawFormatString(x, y + 2, " %s %i/%i (%i %s)", language[DRAWSCREEN_PAGE], currentdir/24 + 1, page, ndirectories, language[DRAWSCREEN_GAMES] );
	
	m = selected;
	y += 23;
	y3 = (y + 50);
    
    i = 0;
    int flash2 = 0;
    #define MAX_FLASH 32
    if (frame_count & MAX_FLASH) flash2= (MAX_FLASH-1) - (frame_count & (MAX_FLASH-1)); else flash2= (frame_count & (MAX_FLASH-1));
    for(n = 0; n < 4; n++) 
        for(m = 0; m < 6; m++) 
        {
            int f = (select_px == m && select_py == n);
            float f2 = (int) f;
            
            //BACKGROUND HERMES STYLE 
            if(background_sel > 7) f2 = 3* ((float) (flash2 *(select_px == m && select_py == n)))/((float) MAX_FLASH);
            
            //DRAW EMPTY BOX
            if(background_sel == 11) DrawBox((x3 + 200 * m - 4 * f2) div, (y3 + n * 150 - 4 * f2) div, 0, (192 + 8 * f2) div, (142 + 8 * f2) div, 0x4f4f4f28 - (flash2 * f) ); //cobra style
            else DrawBox((x3 + 200 * m - 4 * f2) div, (y3 + n * 150 - 4 * f2) div, 0, (192 + 8 * f2) div, (142 + 8 * f2) div, 0x00000028 + (flash2 * f) );
            
            //DRAW ICON0.PNG
            if(Png_offset[i]) 
            {
				tiny3d_SetTextureWrap(0, Png_offset[i], Png_datas[i].width, 
					Png_datas[i].height, Png_datas[i].wpitch, 
						TINY3D_TEX_FORMAT_A8R8G8B8,  TEXTWRAP_CLAMP, TEXTWRAP_CLAMP,1);
             
                DrawTextBox((x3 + 200 * m - 4 * f2) div, (y3 + n * 150 - 4 * f2) div, 0, (192 + 8 * f2) div, (142 + 8 * f2) div, (0xffffffff- (MAX_FLASH * 2 * f)) + (flash2 * 2 * f));
            }
			i++;   
        }
 
	i = selected;

    SetCurrentFont(FONT_TTF);
    SetFontSize(18, 20);
    SetFontAutoCenter(0);

// DRAW TEXT OPTION
	if(background_sel == 11) SetFontColor(0x84c225ff, 0x00000000); //cobra style

	x2= DrawFormatString(1024, 0, " %s ", language[DRAWSCREEN_STGLOPT]);
	DrawFormatString(x + 4 * 200 - (x2 - 1024) - 5 , y + 3 * 150 + 11, " %s ", language[DRAWSCREEN_STGLOPT]);
			    
// DRAW TEXT PLAY
	if( background_sel == 11 && Png_offset[i] ) //cobra style
	{
		DrawBox((x3 + 200 * select_px - 8 + (200 - 24 * 8)/2) div, (y3 + select_py * 150 - 4 + 150 - 40) div, 0, 200 div, 40 div, 0x000000a0);
		SetFontColor(0xfefefeff, 0x00000000); //cobra style
		SetCurrentFont(FONT_TTF);
		SetFontSize(24 div, 24 div);
		x2 = DrawFormatString((x3 + 200 * select_px - 4 + (200 - 24 * 8)/2) div - 13, (y3 + select_py * 150 - 4 + 150 - 31) div, "  %s", language[DRAWSCREEN_PLAY]);
	}
	else if(Png_offset[i]) 
    {
		DrawBox((x3 + 200 * select_px - 8 + (200 - 24 * 8)/2) div, (y3 + select_py * 150 - 4 + 150 - 40) div, 0, 200 div, 40 div, 0x404040a0);
		SetFontColor(0xffffffff, 0x00000000);
		SetCurrentFont(FONT_TTF);
		SetFontSize(24 div, 24 div);
		x2 = DrawFormatString((x3 + 200 * select_px - 4 + (200 - 24 * 8)/2) div - 13, (y3 + select_py * 150 - 4 + 150 - 31) div, "  %s", language[DRAWSCREEN_PLAY]);
    }
    
// DRAW BOTTOM PANEL
    DrawBox(x, y + 3 * 150, 0, 200 * 4 - 6, 40, 0x00000028);


// DRAW GAME NAME
    if(Png_offset[i]) 
    {
		if(background_sel == 11) SetFontColor(0x84c225ff, 0x00000000); //cobra style
		else SetFontColor(0xffffffff, 0x00000000);
		SetFontSize(24, 26);
		SetFontAutoCenter(0);
		if (manager.iso[i].sfo_path <  manager.iso[i].filename)
		DrawFormatString(x, y + 3 * 150 + 8, " %s", manager.iso[i].filename);
		else
		DrawFormatString(x, y + 3 * 150 + 8, " %s", sfo_title[(currentdir + i)]);
	}
		
// DRAW GAME SIZE
	if ( (manager.info.major > 1 || manager.info.minor >= 3) && Png_offset[i] )
	{
		if(background_sel == 11) SetFontColor(0xfff500ff, 0x00000000); //cobra style
		else SetFontColor(0xffffffff, 0x00000000);
		SetFontSize(16, 18);
		SetFontAutoCenter(0);
		
		x2= DrawFormatString(1024, 0, "%s: %d MB ", language[DRAWSCREEN_GAMESIZE], (manager.iso[(currentdir + i)].size / 1048576) );
		DrawFormatString(x + 4 * 200 - (x2 - 1024) - 5 , y - 23 + 2, "%s: %d MB ", language[DRAWSCREEN_GAMESIZE], (manager.iso[(currentdir + i)].size / 1048576) );
	}
	
// FTP ICON
  /*  if(flash && ftp_inited) {
        tiny3d_SetTextureWrap(0, Png_res_offset[4], Png_res[4].width, 
            Png_res[4].height, Png_res[4].wpitch, 
            TINY3D_TEX_FORMAT_A8R8G8B8,  TEXTWRAP_CLAMP, TEXTWRAP_CLAMP,1);

        DrawTextBox(200 * 4 -32, y - 30, 0, 64, 32, 0xffffffff); 
    }
     */

    //SetCurrentFont(FONT_DEFAULT);

    tiny3d_Flip();
    ps3pad_read();

// RETURN TO XMB
    if(new_pad & BUTTON_TRIANGLE) 
    {
		if(!test_ftp_working()) 
			if(DrawDialogYesNo(language[DRAWSCREEN_EXITXMB])==1) {exit_program = 1; return;}
    }


// RUN GAME
    if(new_pad & (BUTTON_CROSS | BUTTON_CIRCLE)) 
    {
        i = selected;
 
        if(test_ftp_working()) return;

		if(Png_offset[i])
		{ 
			cobra_iso_run(&manager.iso[currentdir + i]);
			if( cobra_manager_eject(&manager) == 0 )
			{
				if( manager.info.minor >= 6 )
					DrawDialogOK( language[DRAWSCREEN_EJECTINFO] );
				else
					DrawDialogOK( language[DRAWSCREEN_MCUOLD] );
			}
			
			exit_program = 1;
		}
    } 

// MENU' OPTIONS
    if(new_pad & BUTTON_START) 
    {
		select_option = 0;
        menu_screen = 1; 
        return;
    }


    static int auto_up = 0, auto_down = 0, auto_left = 0, auto_right = 0;

    AUTO_BUTTON_REP(auto_up, BUTTON_UP)
    AUTO_BUTTON_REP(auto_down, BUTTON_DOWN)
    AUTO_BUTTON_REP(auto_left, BUTTON_LEFT)
    AUTO_BUTTON_REP(auto_right, BUTTON_RIGHT)

    if(new_pad & BUTTON_UP) {
        select_py--;

        auto_up = 1;
        frame_count = 32;

        if(select_py < 0) {
            
            select_py = 3;
            
            if(currentdir >= 24) {currentdir -= 24; get_games();}
			else {currentdir = ROUND_UP24(ndirectories) - 24; get_games();}
        }
        return;
    }

    if(new_pad & BUTTON_DOWN) {
        
        select_py++;

        auto_down = 1;
        frame_count = 32;
        
        if(select_py > 3) {
           
            select_py = 0; 
            
            if(currentdir < (ROUND_UP24(ndirectories) - 24)) {currentdir += 24; get_games();}
            else {currentdir = 0; get_games();}
        }
        return;
    }

    if(new_pad & BUTTON_LEFT) {
        
        select_px--;

        auto_left = 1;
        frame_count = 32;

        if(select_px < 0) {
            
            select_px = 5;
            
            if(currentdir >= 24) {currentdir -= 24; get_games();}
            else {currentdir = ROUND_UP24(ndirectories) - 24; get_games();}
        }
        return;
    }

    if(new_pad & BUTTON_RIGHT) {
        
        select_px++; 

        auto_right = 1;
        frame_count = 32;

        if(select_px > 5) {
            
            select_px = 0; 
            
            if(currentdir < (ROUND_UP24(ndirectories) - 24)) { currentdir += 24; get_games();}
			else {currentdir = 0; get_games();}
        }
        return;
    }

    if(new_pad & BUTTON_L1) //change page
    {
        frame_count = 32;
            
        if(currentdir >= 24) {currentdir -= 24; get_games();}
        else {currentdir = ROUND_UP24(ndirectories) - 24; get_games();}

        return;
    }
    
    if(new_pad & BUTTON_R1) //change page
    {
        //maybe wait some seconds here...
        frame_count = 32;
            
        if(currentdir < (ROUND_UP24(ndirectories) - 24)) {currentdir += 24; get_games();}
        else {currentdir = 0; get_games();}

        return;
    }
}


void draw_screen48(float x, float y)
{
	
#define div48		/2

    int i, n, m;
    float x2, y3, x3;
    int selected = select_px + select_py * 8;

    x3 = x + ((x * 2) / 2) - 4;
    x = x - 1;
    y = (y - 1);

// TOP PANEL
   DrawBox(x - 1, y, 0, 200 * 4 - 4, 20, 0x00000028);
   
// N° PAGE,GAMES..
	page = (ROUND_UP48(ndirectories)/48);

	SetCurrentFont(FONT_TTF);

   	if(background_sel == 11) SetFontColor(0xfff500ff, 0x00000000); //cobra style
    else SetFontColor(0xffffffff, 0x00000000);
    SetFontSize(16, 18);
    SetFontAutoCenter(0);

    DrawFormatString(x, y + 2, " %s %i/%i (%i %s)", language[DRAWSCREEN_PAGE], currentdir/48 + 1, page, ndirectories, language[DRAWSCREEN_GAMES] );
	
	m = selected;
	y += 23;
	y3 = (y + 26);
    
    i = 0;
    int flash2 = 0;
    #define MAX_FLASH 32
    if (frame_count & MAX_FLASH) flash2= (MAX_FLASH-1) - (frame_count & (MAX_FLASH-1)); else flash2= (frame_count & (MAX_FLASH-1));
    for(n = 0; n < 6; n++) 
        for(m = 0; m < 8; m++) 
        {
            int f = (select_px == m && select_py == n);
            float f2 = (int) f;
            
            //BACKGROUND HERMES STYLE 
            if(background_sel > 7) f2 = 3* ((float) (flash2 *(select_px == m && select_py == n)))/((float) MAX_FLASH);
            
            //DRAW EMPTY BOX
            if(background_sel == 11) DrawBox((x3 + 200 * m - 4 * f2) div48, (y3 + n * 150 - 4 * f2) div48, 0, (192 + 8 * f2) div48, (142 + 8 * f2) div48, 0x4f4f4f28 - (flash2 * f) );
            else DrawBox((x3 + 200 * m - 4 * f2) div48, (y3 + n * 150 - 4 * f2) div48, 0, (192 + 8 * f2) div48, (142 + 8 * f2) div48, 0x00000028 + (flash2 * f) );
            
            //DRAW ICON0.PNG
            if(Png_offset[i]) 
            {
				tiny3d_SetTextureWrap(0, Png_offset[i], Png_datas[i].width, 
					Png_datas[i].height, Png_datas[i].wpitch, 
						TINY3D_TEX_FORMAT_A8R8G8B8,  TEXTWRAP_CLAMP, TEXTWRAP_CLAMP,1);
             
                DrawTextBox((x3 + 200 * m - 4 * f2) div48, (y3 + n * 150 - 4 * f2) div48, 0, (192 + 8 * f2) div48, (142 + 8 * f2) div48, (0xffffffff- (MAX_FLASH * 2 * f)) + (flash2 * 2 * f));
            }
			i++;   
        }
 
	i = selected;

    SetCurrentFont(FONT_TTF);
    SetFontSize(18, 20);
    SetFontAutoCenter(0);

// DRAW TEXT OPTION	
	if(background_sel == 11) SetFontColor(0x84c225ff, 0x00000000); //cobra style

	x2= DrawFormatString(1024, 0, " %s ", language[DRAWSCREEN_STGLOPT]);
	DrawFormatString(x + 4 * 200 - (x2 - 1024) - 5 , y + 3 * 150 + 14, " %s ", language[DRAWSCREEN_STGLOPT]);
			    
// DRAW TEXT PLAY
	if(background_sel == 11 && Png_offset[i])
	{
		DrawBox((x3 + 200 * select_px - 8 + (200 - 24 * 8)/2) div48, (y3 + select_py * 150 - 4 + 150 - 40) div48, 0, 200 div48, 40 div48, 0x000000a0);
		SetFontColor(0xfefefeff, 0x00000000); //cobra style
        SetCurrentFont(FONT_TTF);
        SetFontSize(24 div48, 24 div48);
        x2 = DrawFormatString((x3 + 200 * select_px - 4 + (200 - 24 * 8)/2) div48 - 13, (y3 + select_py * 150 - 4 + 150 - 31) div48, "   %s", language[DRAWSCREEN_PLAY]);
	}
    else if(Png_offset[i]) 
    {
		DrawBox((x3 + 200 * select_px - 8 + (200 - 24 * 8)/2) div48, (y3 + select_py * 150 - 4 + 150 - 40) div48, 0, 200 div48, 40 div48, 0x404040a0);
		SetFontColor(0xffffffff, 0x00000000); //cobra style
        SetCurrentFont(FONT_TTF);
        SetFontSize(24 div48, 24 div48);
        x2 = DrawFormatString((x3 + 200 * select_px - 4 + (200 - 24 * 8)/2) div48 - 13, (y3 + select_py * 150 - 4 + 150 - 31) div48, "   %s", language[DRAWSCREEN_PLAY]);
    }
    
// DRAW BOTTOM PANEL
    DrawBox(x - 1, y + 3 * 150 + 2.5, 0, 200 * 4 - 4, 40, 0x00000028);


// DRAW GAME NAME
    if(Png_offset[i]) 
    {
		if(background_sel == 11) SetFontColor(0x84c225ff, 0x00000000); //cobra style
		else SetFontColor(0xffffffff, 0x00000000);
		SetFontSize(24, 26);
		SetFontAutoCenter(0);
		if (manager.iso[i].sfo_path <  manager.iso[i].filename)
		DrawFormatString(x, y + 3 * 150 + 8, " %s", manager.iso[i].filename);
		else
		DrawFormatString(x, y + 3 * 150 + 8, " %s", sfo_title[(currentdir + i)]);
	}
		
// DRAW GAME SIZE
	if ( (manager.info.major > 1 || manager.info.minor >= 3) && Png_offset[i] )
	{
		if(background_sel == 11) SetFontColor(0xfff500ff, 0x00000000); //cobra style
		else SetFontColor(0xffffffff, 0x00000000);
		SetFontSize(16, 18);
		SetFontAutoCenter(0);
		
		x2= DrawFormatString(1024, 0, "%s: %d MB ", language[DRAWSCREEN_GAMESIZE], (manager.iso[(currentdir + i)].size / 1048576) );
		DrawFormatString(x + 4 * 200 - (x2 - 1024) - 5 , y - 23 + 2, "%s: %d MB ", language[DRAWSCREEN_GAMESIZE], (manager.iso[(currentdir + i)].size / 1048576) );
	}
	
// FTP ICON
  /*  if(flash && ftp_inited) {
        tiny3d_SetTextureWrap(0, Png_res_offset[4], Png_res[4].width, 
            Png_res[4].height, Png_res[4].wpitch, 
            TINY3D_TEX_FORMAT_A8R8G8B8,  TEXTWRAP_CLAMP, TEXTWRAP_CLAMP,1);

        DrawTextBox(200 * 4 -32, y - 30, 0, 64, 32, 0xffffffff); 
    }
     */

    //SetCurrentFont(FONT_DEFAULT);

    tiny3d_Flip();
    ps3pad_read();

// RETURN TO XMB
    if(new_pad & BUTTON_TRIANGLE) 
    {
		if(!test_ftp_working()) 
			if(DrawDialogYesNo(language[DRAWSCREEN_EXITXMB])==1) {exit_program = 1; return;}
    }


// RUN GAME
    if(new_pad & (BUTTON_CROSS | BUTTON_CIRCLE)) 
    {
        i = selected;
    
        if(test_ftp_working()) return;

		if(Png_offset[i])
		{ 
			cobra_iso_run(&manager.iso[currentdir + i]);
			if( cobra_manager_eject(&manager) == 0 )
			{
				if( manager.info.minor >= 6 )
					DrawDialogOK( language[DRAWSCREEN_EJECTINFO] );
				else
					DrawDialogOK( language[DRAWSCREEN_MCUOLD] );
			}
			
			exit_program = 1;
		}
    } 

// MENU' OPTIONS
    if(new_pad & BUTTON_START) 
    {
		select_option = 0;
        menu_screen = 1; 
        return;
    }

    static int auto_up = 0, auto_down = 0, auto_left = 0, auto_right = 0;

    AUTO_BUTTON_REP(auto_up, BUTTON_UP)
    AUTO_BUTTON_REP(auto_down, BUTTON_DOWN)
    AUTO_BUTTON_REP(auto_left, BUTTON_LEFT)
    AUTO_BUTTON_REP(auto_right, BUTTON_RIGHT)

    if(new_pad & BUTTON_UP) {
        select_py--;

        auto_up = 1;
        frame_count = 32;

        if(select_py < 0) {
            
            select_py = 5;
            
            if(currentdir >= 48) {currentdir -= 48; get_games();}
			else {currentdir = ROUND_UP48(ndirectories) - 48; get_games();}
        }
        return;
    }

    if(new_pad & BUTTON_DOWN) {
        
        select_py++;

        auto_down = 1;
        frame_count = 32;
        
        if(select_py > 5) {
           
            select_py = 0; 
            
            if(currentdir < (ROUND_UP48(ndirectories) - 48)) {currentdir += 48; get_games();}
            else {currentdir = 0; get_games();}
        }
        return;
    }

    if(new_pad & BUTTON_LEFT) {
        
        select_px--;

        auto_left = 1;
        frame_count = 32;

        if(select_px < 0) {
            
            select_px = 7;
            
            if(currentdir >= 48) {currentdir -= 48; get_games();}
            else {currentdir = ROUND_UP48(ndirectories) - 48; get_games();}
        }
        return;
    }

    if(new_pad & BUTTON_RIGHT) {
        
        select_px++; 

        auto_right = 1;
        frame_count = 32;

        if(select_px > 7) {
            
            select_px = 0; 
            
            if(currentdir < (ROUND_UP48(ndirectories) - 48)) { currentdir += 48; get_games();}
			else {currentdir = 0; get_games();}
        }
        return;
    }

    if(new_pad & BUTTON_L1) //change page
    {
        frame_count = 32;
            
        if(currentdir >= 48) {currentdir -= 48; get_games();}
        else {currentdir = ROUND_UP48(ndirectories) - 48; get_games();}

        return;
    }
    
    if(new_pad & BUTTON_R1) //change page
    {
        //maybe wait some seconds here...
        frame_count = 32;
            
        if(currentdir < (ROUND_UP48(ndirectories) - 48)) {currentdir += 48; get_games();}
        else {currentdir = 0; get_games();}

        return;
    }
}

void draw_gbloptions(float x, float y )
{
    float y2, x2;
    u8 buf[16];

// TOP PANEL
    DrawBox(x, y, 0, 200 * 4 - 8, 20, 0x00000028);

	SetCurrentFont(FONT_TTF);
	
	if(background_sel == 11) SetFontColor(0xfff500ff, 0x00000000); //cobra style
    else SetFontColor(0xffffffff, 0x00000000);
    SetFontSize(16, 18);
    SetFontAutoCenter(0);
  
// TEXT OPTION
    DrawFormatString(x, y + 2, " %s", language[DRAWGLOPT_OPTS]);
	
	
    
// COBRA FW
	x2 = DrawFormatString(1024, 0, "Cobra Fw: v%d.%d ", manager.info.major, manager.info.minor);						
	DrawFormatString(x + 4 * 200 - (x2 - 1024) - 5 , y + 2, "Cobra Fw: v%d.%d ", manager.info.major, manager.info.minor);
    
    y += 24;

    DrawBox(x, y, 0, 200 * 4 - 8, 150 * 3 - 8, 0x00000028);

    x2 = x;
    y2 = y + 32;
    
    DrawButton1_UTF8((848 - 520) / 2, y2, 520, language[DRAWGLOPT_LANGUAGE_1 + (manager_cfg.language & 15)], (flash && select_option == 0));
    
    y2+= 48;

    DrawButton1_UTF8((848 - 520) / 2, y2, 520, language[DRAWGLOPT_SETVIDEO], (flash && select_option == 1));
    
    y2+= 48;

    DrawButton1_UTF8((848 - 520) / 2, y2, 520, (ftp_ip_str[0]) ? ftp_ip_str : language[DRAWGLOPT_INITFTP], (flash && select_option == 2));
    
    y2+= 48;

    DrawButton1_UTF8((848 - 520) / 2, y2, 520, language[DRAWTOOLS_ARCHIVEMAN], (flash && select_option == 3));
    
    y2+= 48;
	
    DrawButton1_UTF8((848 - 520) / 2, y2, 520, "Showtime Media Player", (flash && select_option == 4));
    
    y2+= 48;

    /*DrawButton1_UTF8((848 - 520) / 2, y2, 520, "Retro Ode Loader", (flash && select_option == 5));*/
    DrawButton1_UTF8((848 - 520) / 2, y2, 520, " User dumps disc ", (flash && select_option == 5));

    
    y2+= 48;
	
    DrawButton1_UTF8((848 - 520) / 2, y2, 520, language[DRAWGLOPT_CREDITS], (flash && select_option == 6));
    
    y2+= 48;
	
	DrawButton1_UTF8((848 - 520) / 2, y2, 520, language[DRAWGLOPT_BROWSER], (flash && select_option == 7));

    y2+= 48;
	
	DrawButton1_UTF8((848 - 520) / 2, y2, 520, language[GLOBAL_RETURN], (flash && select_option == 8));
   
    y2+= 48;

    DrawBox(x, y + 3 * 150, 0, 200 * 4 - 8, 40, 0x00000028);

//CONSOLE ID
	get_console_id( buf );
	if(background_sel == 11) SetFontColor(0xfefefeff, 0x00000000); //cobra style
    SetFontSize(18, 20);
    SetFontAutoCenter(1);
    DrawFormatString(0, y + 3 * 150 + 11, "IDPS: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", 
							buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10],buf[11],buf[12],buf[13],buf[14],buf[15]); //horrible..
    SetFontAutoCenter(0);

    tiny3d_Flip();

    ps3pad_read();


    if(new_pad & BUTTON_TRIANGLE) {
        menu_screen = 0; return;
    }

    if(new_pad & (BUTTON_CROSS | BUTTON_CIRCLE)) {
    
        switch(select_option) {
			
			case 0:
				manager_cfg.language++; if(manager_cfg.language > 10)  manager_cfg.language = 0;
                sprintf(temp_buffer, "%s/config/language.ini", self_path);
                open_language(manager_cfg.language, temp_buffer);
                SaveManagerCfg();
                break;
				
			case 1:
				select_option = 0;
				menu_screen = 2;
				return;

            case 2:
				if(test_ftp_working()) break;
				if((manager_cfg.opt_flags & OPTFLAGS_FTP) == 0)
                {
                    int r = ftp_init();
					if(r == 0)
                    {
                        ftp_inited = 1;
                        if(DrawDialogYesNo(language[DRAWGLOPT_FTPINITED]) != 1)
                            break;
                    }
                    else
                    {
                        if(r == -1) DrawDialogOK("Error in netInitialize()");
                        else if(r == -2) DrawDialogOK("Error in netCtlInit()");
                        else if(r == -3) DrawDialogOK("Error in netCtlGetInfo()");
                        else if(r == -4) DrawDialogOK("Net Disconnected or Connection not Established");
						else DrawDialogOK(language[DRAWGLOPT_FTPARINITED]);
                        break;
                    }
                }
                else
                {
					DrawDialogOK(language[DRAWGLOPT_FTPSTOPED]);
					ftp_deinit();
					ftp_inited = 0;
                }
                manager_cfg.opt_flags ^= OPTFLAGS_FTP;
                SaveManagerCfg();
                break;

            case 3:
				archive_manager();
				select_px = select_py = 0;
				select_option = 0;
				menu_screen = 0;
				return;

            case 4:
				sysProcessExitSpawn2("/dev_bdvd/PS3_GAME/USRDIR/showtime.self", NULL, NULL, NULL, 0, 3071, SYS_PROCESS_SPAWN_STACK_SIZE_1M);
				select_option = 0;
				menu_screen = 0;
				return;
            
			case 5:
				/*sysProcessExitSpawn2("/dev_bdvd/PS3_GAME/USRDIR/cores/vba_next_libretro_ps3.SELF", NULL, NULL, NULL, 0, 3071, SYS_PROCESS_SPAWN_STACK_SIZE_1M);*/
				  // User dumps disc

				  disc = cobra_disc_open(&manager);

				  sysFsOpen ("/dev_usb000/DISC.ISO", SYS_O_WRONLY, &out, NULL, 0);

				  while (1)
				  {
				    int bytes = cobra_disc_write (disc, out);
				    // Show progress
				    // ...
				    //////
				    if (bytes == 0)
				      break;
				  }
				  sysFsClose (out);
				  cobra_disc_close (disc);

				select_option = 0;
				menu_screen = 0;
				return;
			
			case 6:
				DrawDialogOK(credits_str1);
                DrawDialogOK(credits_str2);
                DrawDialogOK(credits_str3);
                DrawDialogOK(credits_str4);
                break;
			
			case 7:
				sysProcessExitSpawn2("/dev_bdvd/PS3_GAME/USRDIR/browser.self", NULL, NULL, NULL, 0, 3071, SYS_PROCESS_SPAWN_STACK_SIZE_1M);
				select_option = 0;
				menu_screen = 0;
				return;
			
			case 8:
                select_option = 0;
                menu_screen = 0; 
                return;

            default:
               break;
        }
    }


    if(new_pad & BUTTON_UP) {

        frame_count = 32;

        ROT_DEC(select_option, 0, 8)
    }

    if(new_pad & BUTTON_DOWN) {

        frame_count = 32;
        
        ROT_INC(select_option, 8, 0); 
    }
}

void draw_setup_video(float x, float y)
{
	
    float y2; /*, x2;*/

    SetCurrentFont(FONT_TTF);

    // header title

    DrawBox(x, y, 0, 200 * 4 - 8, 20, 0x00000028);

    SetFontColor(0xffffffff, 0x00000000);

    SetFontSize(18, 20);

    SetFontAutoCenter(0);
  
    DrawFormatString(x, y, " %s", language[DRAWSETVID_SETVIDEO]);

    y += 24;

    DrawBox(x, y, 0, 200 * 4 - 8, 150 * 3 - 8, 0x00000028);
	
    /*x2 = x;*/
    y2 = y + 32;
    
    DrawButton1_UTF8((848 - 520) / 2, y2, 520, language[DRAWSETVID_SCRADJUST], (flash && select_option == 0));
    
    y2+= 48;

    DrawButton1_UTF8((848 - 520) / 2, y2, 520, (background_sel > 7) ? language[DRAWSETVID_CHANGEBCKHERMES] : language[DRAWSETVID_CHANGEBCK], (flash && select_option == 1));
    
    y2+= 48;

    if(num_box==24)
	DrawButton1_UTF8((848 - 520) / 2, y2, 520, language[DRAWSETVID_GRID6x4] , (flash && select_option == 2));
	else if(num_box==48)
	DrawButton1_UTF8((848 - 520) / 2, y2, 520, language[DRAWSETVID_GRID8x6] , (flash && select_option == 2));
	else
	DrawButton1_UTF8((848 - 520) / 2, y2, 520, language[DRAWSETVID_GRID4x3] , (flash && select_option == 2));
    
    y2+= 48;
	
    DrawButton1_UTF8((848 - 520) / 2, y2, 520, language[GLOBAL_RETURN], (flash && select_option == 3));
    
    y2+= 48;
	
	
    SetCurrentFont(FONT_TTF);

    // draw game name

    DrawBox(x, y + 3 * 150, 0, 200 * 4 - 8, 40, 0x00000028);

    SetFontColor(0xffffffff, 0x00000000);


    tiny3d_Flip();

    ps3pad_read();

    if(new_pad & (BUTTON_CROSS | BUTTON_CIRCLE)) {
    
        switch(select_option) {
			
            case 0:
				video_adjust();
                select_option = 0;
                menu_screen = 0; 
                return;
                
            case 1:
				background_sel++;
				if(background_sel == MAX_COL) background_sel = 0;
                manager_cfg.background_sel = background_sel;
                SaveManagerCfg();
                break;

            case 2:
				if( num_box == 12 )
				{
					num_box = 24;
					manager_cfg.num_box = num_box;
				}
				else if( num_box == 24 )
				{
					num_box = 48;
					manager_cfg.num_box = num_box;
				}
				else
				{
					num_box = 12;
					manager_cfg.num_box = num_box;
				}
				SaveManagerCfg();
				refresh_game = 1;
                break;

            case 3:
                select_option = 0;
                menu_screen = 3; 
                return;

            default:
               break;
        }
    
    }

    if(new_pad & BUTTON_TRIANGLE) {
        menu_screen = 0; return;
    }
   

    if(new_pad & BUTTON_UP) {

        frame_count = 32;

        ROT_DEC(select_option, 0, 3);
        
    }

    if(new_pad & BUTTON_DOWN) {

        frame_count = 32;
        
        ROT_INC(select_option, 3, 0);
        
    }
    
}
