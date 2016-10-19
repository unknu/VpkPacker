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

		static string Strings[];
		enum eString {
			FoundError,

			SelectSourceFolder,
			IsSourceFolder,
			SelectDestinationFolder,
			IsDestinationFolder,

			SelectCompressLevel,
			IsCompressLevel,

			CreatingVpk,
			TotalProgressPercentage,
			FileNumber,
			FinishFileNumber,
			FilesSize,
			FinishFilesSize,
			CurrentFileName,
			CurrentProgressPercentage,
			CompletedCreationVpk,

			OperationMethod,

			OpeErrorCancel,

			OpeCompressCancel,
			OpeChangeDeleteFile,

			OpeDirectory_1,
			OpeDirectory_2,
			OpeDirectory_3,
			OpeDirectory_4,

			OpeCompressLevel_1,
			OpeCompressLevel_2,
			OpeCompressLevel_3,

			OpeCompletedCreationVpk,

			CurrentSetting,
			SetCompressAndDelete_1,
			SetCompressAndDelete_2,
			SetCompressAndDelete_3,
			SetCompressAndDelete_4,
		};

		static const char Version[];

		static const unsigned int MainBaseX = 15;
		static const unsigned int MainBaseY = 15;

		static const unsigned int OperationBaseX = 600;
		static const unsigned int OperationBaseY = MainBaseY;

		static const unsigned int CompressBaseX = MainBaseX;
		static const unsigned int CompressBaseY = 200;

		static const unsigned int SettingBaseX = OperationBaseX;
		static const unsigned int SettingBaseY = 200;

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
