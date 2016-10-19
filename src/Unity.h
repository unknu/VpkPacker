#pragma once

#include "Directory.h"
#include "Indicator.h"
#include "Pad.h"

namespace VpkPacker {

	class Unity : public Directory, public Indicator, public Pad {
	private:
		const char DefaultDestPath[15] = "ux0:data/vpks/";

		void ResetOperation();
	public:
		Unity();
		~Unity();

		bool CheckDraw();
		void Draw();
	};

}