#include <psp2/kernel/processmgr.h>
#include <psp2/sysmodule.h>

#include "Header.h"

/*

SceVshBridge_SceVshBridge_vshNpDrmIsLooseAccountBind.o STORED 724

SceKernel_SceLibKernel_sceKernelLoadModule.o STORED 716
SceKernel_SceLibKernel_sceKernelLoadStartModule.o STORED 724
SceKernel_SceLibKernel_sceKernelUnloadModule.o STORED 720
SceKernel_SceLibKernel_sceKernelOpenModule.o STORED 716
SceKernel_SceLibKernel_sceKernelStartModule.o STORED 720
SceKernel_SceLibKernel_sceKernelStopModule.o STORED 716
SceKernel_SceLibKernel_sceKernelCloseModule.o STORED 720
SceKernel_SceLibKernel_sceKernelCallModuleExit.o STORED 720
SceKernel_SceLibKernel_sceKernelGetModuleInfoByAddr.o STORED 728
SceKernel_SceThreadmgr_sceKernelGetProcessId.o STORED 720
SceKernel_SceLibKernel_sceKernelGetOpenPsId.o STORED 720

SceKernel_SceModulemgr_sceKernelGetSystemSwVersion.o STORED 724

SceKernel_SceModulemgr_sceKernelGetModuleIdByAddr.o STORED 724
SceKernel_SceModulemgr_sceKernelGetModuleInfo.o STORED 720
SceKernel_SceModulemgr_sceKernelGetModuleList.o STORED 720
*/

int main( int argc, char *argv[] )
{
	vita2d_init();
	vita2d_set_clear_color( RGBA_BLACK );

	VpkPacker::Unity u;
	while( 1 ) {
		if(! u.CheckDraw() ) continue;
		sceKernelPowerTick( SCE_KERNEL_POWER_TICK_DISABLE_AUTO_SUSPEND );

		vita2d_start_drawing();
		vita2d_clear_screen();

		u.Draw();

		vita2d_end_drawing();
		vita2d_swap_buffers();
	}

	vita2d_fini();

	if( sceSysmoduleIsLoaded( SCE_SYSMODULE_PGF ) == SCE_SYSMODULE_LOADED ) sceSysmoduleUnloadModule( SCE_SYSMODULE_PGF );

	sceKernelExitProcess( 0 );
	return 0;
}
