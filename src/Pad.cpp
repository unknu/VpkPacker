#include <string.h>

#include <psp2/ctrl.h>
#include <psp2/apputil.h>
#include <psp2/system_param.h>

#include "Pad.h"

namespace VpkPacker {

	Pad::Pad()
	{
		memset( &cd, 0, sizeof( cd ) );
		sceGetIDEnterButton();
	}
	Pad::~Pad()
	{
	}

	int Pad::GetIDEnterButton()
	{
		return IDEnterButton;
	}
	void Pad::sceGetIDEnterButton()
	{
		SceAppUtilInitParam init_param;
		SceAppUtilBootParam boot_param;
		memset( &init_param, 0, sizeof( SceAppUtilInitParam ) );
		memset( &boot_param, 0, sizeof( SceAppUtilBootParam ) );
		sceAppUtilInit( &init_param, &boot_param );

		sceAppUtilSystemParamGetInt( SCE_SYSTEM_PARAM_ID_ENTER_BUTTON, &IDEnterButton );

		sceAppUtilShutdown();
	}
	unsigned int Pad::GetCurrentButton()
	{
		return CurrentButton;
	}

	bool Pad::Update()
	{
		sceCtrlPeekBufferPositive( 0, &cd, 1 );

		CurrentButtons = cd.buttons;
		if( IDEnterButton != SCE_SYSTEM_PARAM_ENTER_BUTTON_CIRCLE ) {
			if( ( cd.buttons & ( SCE_CTRL_CIRCLE | SCE_CTRL_CROSS ) ) != ( SCE_CTRL_CIRCLE | SCE_CTRL_CROSS ) ) {
				if( cd.buttons & SCE_CTRL_CIRCLE ) {
					CurrentButtons = ( cd.buttons & ~SCE_CTRL_CIRCLE ) | SCE_CTRL_CROSS;
				} else if( cd.buttons & SCE_CTRL_CROSS ) {
					CurrentButtons = ( cd.buttons & ~SCE_CTRL_CROSS ) | SCE_CTRL_CIRCLE;
				}
			}
		}
		CurrentButton = CurrentButtons & ~PrevButtons;

		if( PrevButtons == CurrentButtons ) return false;

		PrevButtons = CurrentButtons;

		return true;
	}

}
