
#include <sstream>

#include "Indicator.h"
#include "Error.h"


namespace VpkPacker {

	const char Indicator::Version[] = "ver beta4";

	string Indicator::Strings[] = {
		"エラー",

		"作成元フォルダ選択",
		"作成元フォルダ",
		"作成先フォルダ選択",
		"作成先フォルダ",

		"圧縮方式選択",
		"圧縮方式",

		"vpk作成中...",
		"全体進行度",
		"ファイル中",
		"ファイル完了",
		"バイト中",
		"バイト完了",
		"現在処理ファイル",
		"現在進行度",
		"vpk作成完了",

		"操作方法",
		"○×：キャンセル",

		"×：キャンセル",
		"SELECT：ファイル削除切替",

		"↑↓←→ＬＲ：移動",
		"△：ソート",
		"○：選択",
		"×：キャンセル",

		"↑↓ＬＲ：移動",
		"×：選択　○：キャンセル",
		"○：選択　×：キャンセル",

		"○×：完了",

		"現在設定",
		"圧縮時に逐次ファイル削除：",
		"○",
		"×",
		"(エラーやキャンセルで元に戻せない)",
	};

	Indicator::Indicator() : OperationCode( 0 ), IDEnterButton( 0 )
	{
		pdt = new DrawText();
		SetMaxEntries( GetLevelStringVectorSize() - 1 );
		SetMaxPos( GetLevelStringVectorSize() - 1 );
	}
	Indicator::~Indicator()
	{
	}

	void Indicator::SetIDEnterButton( int ideb )
	{
		IDEnterButton = ideb;
	}

	int Indicator::GetOperationCode()
	{
		return OperationCode;
	}
	void Indicator::SetOperationCode( int code )
	{
		OperationCode = code;
	}
	void Indicator::ResetOperationCode()
	{
		OperationCode = 0;
	}
	void Indicator::IncOperationCode()
	{
		++ OperationCode;
	}
	int Indicator::IsCompressing()
	{
		return OperationCode == 3;
	}
	int Indicator::IsCompressSuccess()
	{
		return OperationCode == 4;
	}

	void Indicator::Draw()
	{
		string MainString = ( string )"VpkPacker " + Version + "\n\n";

		if( Error::Code ) {
			MainString += Strings[FoundError] + "\n\n" + Error::GetString() + "\n";
		} else if( OperationCode == 0 ) {
			MainString += Strings[SelectSourceFolder] + "\n";
		} else if( OperationCode > 0 ) {
			MainString += Strings[IsSourceFolder] + "\n" + GetSrcPath() + "\n\n";
			if( OperationCode == 1 ) {
				MainString += Strings[SelectDestinationFolder] + "\n";
			} else if( OperationCode >= 2 ) {
				MainString += Strings[IsDestinationFolder] + "\n" + GetDestPath() + "\n\n";
			}
			if( OperationCode == 2 ) {
				MainString += Strings[SelectCompressLevel] + "\n\n\n";
			} else if( OperationCode >= 3 ) {
				MainString += Strings[IsCompressLevel] + "\n" + GetCurModeString() + "\n\n";
			}
			if( IsCompressing() ) {
				MainString += Strings[CreatingVpk] + "\n\n";
			}
			if( IsCompressing() || IsCompressSuccess() ) {
				ostringstream sout;
				sout << Strings[TotalProgressPercentage] << ": " << GetTotalPercent() << " %" << endl
					<< GetFileNum() << " " << Strings[FileNumber] << " " << GetFinishFileNum() << " " << Strings[FinishFileNumber] << endl
					<< GetTotalFileSize() << " " << Strings[FilesSize] << " " << GetTotalWrittenSize() << " " << Strings[FinishFilesSize] << endl << endl
					<< Strings[CurrentFileName] << ": " << GetCurFileName() << endl
					<< Strings[CurrentProgressPercentage] << ": " << GetCurPercent() << " %" << endl
					<< GetCurFileSize() << " " << Strings[FilesSize] << " " << GetCurWrittenSize() << " " << Strings[FinishFilesSize] << endl;
				MainString += sout.str();
			}
			if( IsCompressSuccess() ) {
				MainString += "\n\n" + Strings[CompletedCreationVpk] + "\n";
			}
		}
		pdt->Draw( MainBaseX, MainBaseY, MainString.c_str() );

		string OperationString = Strings[OperationMethod] + "\n\n";

		if( Error::Code ) {
			OperationString += Strings[OpeErrorCancel] + "\n";
		} else if( IsCompressing() ) {
			OperationString += Strings[OpeCompressCancel] + "\n";
		} else {
			OperationString += Strings[OpeChangeDeleteFile] + "\n\n";

			if( OperationCode < 2 ) {
				OperationString += Strings[OpeDirectory_1] + "\n" + Strings[OpeDirectory_2] + "　"
					+ Strings[OpeDirectory_3] + "\n" + Strings[OpeDirectory_4] + "\n";
			} else if( OperationCode == 2 ) {
				OperationString += Strings[OpeCompressLevel_1] + "\n";
				OperationString += IDEnterButton ? Strings[OpeCompressLevel_2] + "\n" : Strings[OpeCompressLevel_3] + "\n";

				int y = CompressBaseY;
				for( int i = 0; i < GetLevelStringVectorSize(); ++ i ) {
					if( i != GetPos() ) y = pdt->DrawLF( MainBaseX, y, GetModeString( i ).c_str() );
					else y = pdt->DrawLF( MainBaseX, y, RGBA_GREEN, GetModeString( i ).c_str() );
				}
			} else if( IsCompressSuccess() ) {
				OperationString += Strings[OpeCompletedCreationVpk] + "\n";
			}
		}
		pdt->Draw( OperationBaseX, OperationBaseY, OperationString.c_str() );

		string SettingString = Strings[CurrentSetting] + "\n\n";
		SettingString += Strings[SetCompressAndDelete_1];
		SettingString += GetFileRemove() ? Strings[SetCompressAndDelete_2] : Strings[SetCompressAndDelete_3];
		SettingString += "\n" + Strings[SetCompressAndDelete_4];
		pdt->Draw( SettingBaseX, SettingBaseY, SettingString.c_str() );
	}

}
