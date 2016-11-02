#pragma once

#include "Directory.h"
#include "Indicator.h"
#include "Pad.h"

namespace VpkPacker {

	class Unity : public Directory, public Indicator, public Pad {
	private:

		void ResetOperation();
	public:
		Unity();
		~Unity();

		bool CheckDraw();
		void Draw();
	};

}