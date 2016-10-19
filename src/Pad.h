#pragma once

#include <psp2/ctrl.h>

#include "Directory.h"
#include "Indicator.h"


namespace VpkPacker {

	class Pad {
	private:
		SceCtrlData cd;
		int IDEnterButton;

		unsigned int CurrentButtons;
		unsigned int CurrentButton;
		unsigned int PrevButtons;
	public:

		Pad();
		~Pad();

		void sceGetIDEnterButton();
		int GetIDEnterButton();
		unsigned int GetCurrentButton();
		bool Update();
	};

}
