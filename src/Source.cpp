#include <psp2/kernel/processmgr.h>
#include <psp2/sysmodule.h>

#include "Header.h"


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
