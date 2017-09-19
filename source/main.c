/*
 * Copyright (C) 2017 FIX94
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#include <gccore.h>
#include <stdio.h>
#include <fat.h>
#include <string.h>
#include <unistd.h>
#include <ogc/lwp_threads.h>
#include "CommonConfig.h"

static NIN_CFG nincfg;
static u8 *EXECUTE_ADDR = (u8*)0x92000000;
static u8 *BOOTER_ADDR = (u8*)0x92F00000;
static void (*entry)() = (void*)0x92F00000;
static struct __argv *ARGS_ADDR = (struct __argv*)0x93300800;

extern u8 app_booter_bin[];
extern u32 app_booter_bin_size;

int main(int argc, char *argv[]) 
{
	void *xfb = NULL;
	GXRModeObj *rmode = NULL;
	VIDEO_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE)
		VIDEO_WaitVSync();
	int x = 24, y = 32, w, h;
	w = rmode->fbWidth - (32);
	h = rmode->xfbHeight - (48);
	CON_InitEx(rmode, x, y, w, h);
	VIDEO_ClearFrameBuffer(rmode, xfb, COLOR_BLACK);
	printf(" \n");
	const char *fPath = "/apps/nintendont/boot.dol";
	fatInitDefault();
	FILE *f = fopen(fPath,"rb");
	if(!f)
	{
		fPath = "/apps/Nintendont/boot.dol";
		f = fopen(fPath,"rb");
	}
	if(!f)
	{
		printf("boot.dol not found!\n");
		sleep(2);
		return -1;
	}
	fseek(f,0,SEEK_END);
	size_t fsize = ftell(f);
	fseek(f,0,SEEK_SET);
	fread(EXECUTE_ADDR,1,fsize,f);
	DCFlushRange(EXECUTE_ADDR,fsize);
	fclose(f);
	memcpy(BOOTER_ADDR,app_booter_bin,app_booter_bin_size);
	DCFlushRange(BOOTER_ADDR,app_booter_bin_size);
	ICInvalidateRange(BOOTER_ADDR,app_booter_bin_size);
	f = fopen("/nincfg.bin","rb");
	if(!f)
	{
		printf("nincfg.bin not found!\n");
		sleep(2);
		return -2;
	}
	fread(&nincfg,1,sizeof(NIN_CFG),f);
	fclose(f);
	memset(nincfg.GamePath,0,255);
	memset(nincfg.CheatPath,0,255);
	//this config can be modified with whatever settings you want for this game
	//by default it enables autoboot and sets it to boot from sd (wii vc default)
	nincfg.Config |= (NIN_CFG_AUTO_BOOT);
	nincfg.Config &= ~(NIN_CFG_USB);
	//for example this line would disable any widescreen bits set in the config
	//nincfg.Config &= ~(NIN_CFG_USB|NIN_CFG_WIIU_WIDE|NIN_CFG_FORCE_WIDE);
	strcpy(nincfg.GamePath,"di");
	char *CMD_ADDR = (char*)ARGS_ADDR + sizeof(struct __argv);
	size_t full_fPath_len = strlen(fPath)+1;
	size_t full_nincfg_len = sizeof(NIN_CFG);
	size_t full_args_len = sizeof(struct __argv)+full_fPath_len+full_nincfg_len;
	memset(ARGS_ADDR, 0, full_args_len);
	ARGS_ADDR->argvMagic = ARGV_MAGIC;
	ARGS_ADDR->commandLine = CMD_ADDR;
	ARGS_ADDR->length = full_args_len;
	ARGS_ADDR->argc = 2;
	memcpy(CMD_ADDR, fPath, full_fPath_len);
	memcpy(CMD_ADDR+full_fPath_len, &nincfg, full_nincfg_len);
	DCFlushRange(ARGS_ADDR, full_args_len);
	VIDEO_SetBlack(TRUE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE)
		VIDEO_WaitVSync();
	SYS_ResetSystem(SYS_SHUTDOWN,0,0);
	__lwp_thread_stopmultitasking(entry);

	return 0;
}
