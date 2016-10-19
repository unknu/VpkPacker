#pragma once

#include <string>

#include "DrawText.h"
#include "Selector.h"
#include "Compressor.h"



namespace VpkPacker {

	class Indicator : public Selector, public Compressor {
	private:
		DrawText *pdt;
		int OperationCode;
		int IDEnterButton;

		static const char Version[];

		static const unsigned int MainBaseX = 15;
		static const unsigned int MainBaseY = 15;

		static const unsigned int OperationBaseX = 700;

	public:
		Indicator();
		~Indicator();

		void SetIDEnterButton( int ideb );

		int GetOperationCode();
		void SetOperationCode( int code );
		void ResetOperationCode();
		void IncOperationCode();
		int IsCompressing();
		int IsCompressSuccess();

		void Draw();
	};
}
