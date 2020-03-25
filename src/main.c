/*
 *
 * PlayStation(R)Vita SceShell Notice PoC
 *
 * Copyright (C) 2020 Princess of Sleeping
 *
 */

#include <psp2/kernel/modulemgr.h>
#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/kernel/clib.h>
#include <taihen.h>
#include "taihen_min.h"

int (* sceShellNoticeInit)(void *data);
int (* sceShellSetUtf8)(void *data, const char *text, SceSize len);
int (* sceShellSomeCpy)(void *a1, void *a2);
int (* sceShellNoticeClean)(void *data);

int SceShellNoticeTestThread(SceSize args, void *argp)
{
	sceKernelDelayThread(17 * 1000 * 1000);

	char data[0x100];
	int (* SceLsdb_315B9FD6)(void *a1, int a2);

	taiGetModuleExportFunc("SceLsdb", 0xFFFFFFFF, 0x315B9FD6, (uintptr_t *)&SceLsdb_315B9FD6);

	sceShellNoticeInit(data);

	sceShellSetUtf8(&data[0xC], "g00", sceClibStrnlen("g00", 0x10));
	sceShellSetUtf8(&data[0x0], "NPXS10015", sceClibStrnlen("NPXS10015", 0x10));

	data[0x2C] = 1;
	sceShellSetUtf8(&data[0xBC], "Hello :)", sceClibStrnlen("Hello :)", 0x10));

	if(0){
		sceShellSomeCpy(&data[0xCC], &data[0x0]);
	}else{
		sceShellSetUtf8(&data[0xCC], "VITASHELL", sceClibStrnlen("VITASHELL", 0x10));
		*(uint32_t *)(&data[0x28]) = 0x2; // enable app open
		// *(uint32_t *)(&data[0x28]) = 0x1; // enable app bound
	}

	sceShellSetUtf8(&data[0xD8], "arg", sceClibStrnlen("arg", 0x10));

	sceShellSetUtf8(&data[0x30], "ux0:app/VITASHELL/sce_sys/icon0.png", sceClibStrnlen("ux0:app/VITASHELL/sce_sys/icon0.png", 0xFFFF));
	*(uint32_t *)(&data[0xFC]) = 0x10;

	SceLsdb_315B9FD6(data, 1);

	sceShellNoticeClean(data);

	return sceKernelExitDeleteThread(0);
}

int sceKernelBootQueue(const char *name, const void *func)
{
	int res;

	SceUID thid = sceKernelCreateThread(name, func, 0x80, 0x8000, 0, 0, NULL);

	if(thid < 0)
		return thid;

	res = sceKernelStartThread(thid, 0, NULL);

	if(res < 0)
		sceKernelDeleteThread(thid);

	return res;
}

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args){

	tai_module_info_t info;
	info.size = sizeof(info);

	if(taiGetModuleInfo("SceShell", &info) < 0)
		return SCE_KERNEL_START_FAILED;

	switch(info.module_nid){
	case 0x0552F692: // 3.60 Retail
	module_get_offset(info.modid, 0, 0x42930C | 1, &sceShellNoticeInit);
	module_get_offset(info.modid, 0, 0x408E14 | 1, &sceShellSetUtf8);
	module_get_offset(info.modid, 0, 0x408EAC | 1, &sceShellSomeCpy);
	module_get_offset(info.modid, 0, 0x4163E8 | 1, &sceShellNoticeClean);
	break;
	case 0x6CB01295: // 3.60 Devkit
	module_get_offset(info.modid, 0, 0x41AA30 | 1, &sceShellNoticeInit);
	module_get_offset(info.modid, 0, 0x3FAD88 | 1, &sceShellSetUtf8);
	module_get_offset(info.modid, 0, 0x3FAE20 | 1, &sceShellSomeCpy);
	module_get_offset(info.modid, 0, 0x408298 | 1, &sceShellNoticeClean);
	break;
	case 0x5549BF1F: // 3.65 Retail
	module_get_offset(info.modid, 0, 0x429754 | 1, &sceShellNoticeInit);
	module_get_offset(info.modid, 0, 0x40925C | 1, &sceShellSetUtf8);
	module_get_offset(info.modid, 0, 0x4092F4 | 1, &sceShellSomeCpy);
	module_get_offset(info.modid, 0, 0x416830 | 1, &sceShellNoticeClean);
	break;
	default:
		return SCE_KERNEL_START_FAILED;
	}

	sceKernelBootQueue("SceShellNoticeTest", SceShellNoticeTestThread);

	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {

	return SCE_KERNEL_STOP_SUCCESS;
}
