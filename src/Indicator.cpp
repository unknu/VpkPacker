
#include <sstream>

#include "Indicator.h"
#include "Error.h"


namespace VpkPacker {

	const char Indicator::Version[] = "ver beta3";

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
		/*
		int y = MainBaseY;
		y = pdt->DrawLF( MainBaseX, y, name.c_str() );
		y = pdt->LF( y );
		*/
		string MainString = ( string )"VpkPacker " + Version + "\n\n";

		if( Error::Code ) {
			/*
			y = pdt->DrawLF( MainBaseX, y, "エラーだよ！" );
			y = pdt->LF( y );
			y = pdt->DrawLF( MainBaseX, y, Error::GetString().c_str() );
			*/
			MainString += "エラーだよ！\n\n" + Error::GetString() + "\n";
		} else if( OperationCode == 0 ) {
//			y = pdt->DrawLF( MainBaseX, y, "vpkの作成元フォルダを選択してね！" );
			MainString += "vpkの作成元フォルダを選択してね！\n";
		} else if( OperationCode > 0 ) {
			/*
			y = pdt->DrawLF( MainBaseX, y, "vpkの作成元フォルダだよ！" );
			y = pdt->DrawLF( MainBaseX, y, GetSrcPath().c_str() );
			y = pdt->LF( y );
			*/
			MainString += "vpkの作成元フォルダだよ！\n" + GetSrcPath() + "\n\n";
			if( OperationCode == 1 ) {
//				y = pdt->DrawLF( MainBaseX, y, "vpkの作成先フォルダを選択してね！" );
				MainString += "vpkの作成先フォルダを選択してね！\n";
			} else if( OperationCode >= 2 ) {
				/*
				y = pdt->DrawLF( MainBaseX, y, "vpkの作成先フォルダだよ！" );
				y = pdt->DrawLF( MainBaseX, y, GetDestPath().c_str() );
				y = pdt->LF( y );
				*/
				MainString += "vpkの作成先フォルダだよ！\n" + GetDestPath() + "\n\n";
			}
			if( OperationCode == 2 ) {
				/*
				y = pdt->DrawLF( MainBaseX, y, "vpkの圧縮方式を選択してね！" );
				y = pdt->LF( y );
				y = pdt->LF( y );
				*/
				MainString += "vpkの圧縮方式を選択してね！\n\n\n";
			} else if( OperationCode >= 3 ) {
				/*
				y = pdt->DrawLF( MainBaseX, y, "vpkの圧縮方式だよ！" );
				y = pdt->DrawLF( MainBaseX, y, GetCurModeString().c_str() );
				y = pdt->LF( y );
				*/
				MainString += "vpkの圧縮方式だよ！\n" + GetCurModeString() + "\n\n";
			}
			if( IsCompressing() ) {
				/*
				y = pdt->DrawLF( MainBaseX, y, "vpkの作成中だよ！ちょっと待っててね！" );
				y = pdt->LF( y );
				*/
				MainString += "vpkの作成中だよ！ちょっと待っててね！\n\n";
			}
			if( IsCompressing() || IsCompressSuccess() ) {
				/*
				ostringstream sout;
				sout << "全体進行度: " << GetTotalPercent() << " %";
				y = pdt->DrawLF( MainBaseX, y, sout.str().c_str() );
				ostringstream sout2;
				sout2 << GetFileNum() << " ファイル中 " << GetFinishFileNum() << " ファイル完了";
				y = pdt->DrawLF( MainBaseX, y, sout2.str().c_str() );
				ostringstream sout3;
				sout3 << GetTotalFileSize() << " バイト中 " << GetTotalWrittenSize() << " バイト完了";
				y = pdt->DrawLF( MainBaseX, y, sout3.str().c_str() );
				y = pdt->LF( y );
				ostringstream sout4;
				sout4 << "現在処理ファイル: " << GetCurFileName();
				y = pdt->DrawLF( MainBaseX, y, sout4.str().c_str() );
				ostringstream sout5;
				sout5 << "現在進行度: " << GetCurPercent() << " %";
				y = pdt->DrawLF( MainBaseX, y, sout5.str().c_str() );
				ostringstream sout6;
				sout6 << GetCurFileSize() << " バイト中 " << GetCurWrittenSize() << " バイト完了";
				y = pdt->DrawLF( MainBaseX, y, sout6.str().c_str() );
				*/
				ostringstream sout;
				sout << "全体進行度: " << GetTotalPercent() << " %" << endl
					 << GetFileNum() << " ファイル中 " << GetFinishFileNum() << " ファイル完了" << endl
					 << GetTotalFileSize() << " バイト中 " << GetTotalWrittenSize() << " バイト完了" << endl << endl
					 << "現在処理ファイル: " << GetCurFileName() << endl
					 << "現在進行度: " << GetCurPercent() << " %" << endl
					 << GetCurFileSize() << " バイト中 " << GetCurWrittenSize() << " バイト完了" << endl
					;
				MainString += sout.str();
			}
			if( IsCompressSuccess() ) {
				/*
				y = pdt->LF( y );
				y = pdt->LF( y );
				y = pdt->DrawLF( MainBaseX, y, "vpk作成完了だよ！やったね！" );
				*/
				MainString += "\n\nvpk作成完了だよ！やったね！\n";
			}
		}
		pdt->Draw( MainBaseX, MainBaseY, MainString.c_str() );

		/*
		int oy = MainBaseY;
		oy = pdt->DrawLF( OperationBaseX, oy, "操作方法だよ！" );
		oy = pdt->LF( oy );
		*/
		string OperationString = "操作方法だよ！\n\n";

		if( Error::Code ) {
			/*
			string s = "×：キャンセル";
			if( IDEnterButton ) {
				s = "○：キャンセル";
			}
			oy = pdt->DrawLF( OperationBaseX, oy, s.c_str() );
			*/
			OperationString += IDEnterButton ? "○：キャンセル\n" : "×：キャンセル\n";
		} else if( IsCompressing() ) {
//			oy = pdt->DrawLF( OperationBaseX, oy, "START：キャンセル" );
			OperationString += "START：キャンセル\n";
		} else {
			/*
			oy = pdt->DrawLF( OperationBaseX, oy, "SELECT：ファイル削除切替" );
			oy = pdt->LF( oy );
			*/
			OperationString += "SELECT：ファイル削除切替\n\n";

			if( OperationCode < 2 ) {
				/*
				oy = pdt->DrawLF( OperationBaseX, oy, "↓↑○×ＬＲ：移動" );
				oy = pdt->DrawLF( OperationBaseX, oy, "△：ソート　□：選択" );
				oy = pdt->DrawLF( OperationBaseX, oy, "START：選択解除" );
				*/
				OperationString += "↓↑○×ＬＲ：移動\n△：ソート　□：選択\nSTART：選択解除\n";
			} else if( OperationCode == 2 ) {
				/*
				oy = pdt->DrawLF( OperationBaseX, oy, "↓↑ＬＲ：移動" );
				string s = "○：選択　×：キャンセル";
				if( IDEnterButton ) {
					s = "×：選択　○：キャンセル";
				}
				oy = pdt->DrawLF( OperationBaseX, oy, s.c_str() );
				*/
				OperationString += "↓↑ＬＲ：移動\n";
				OperationString += IDEnterButton ? "×：選択　○：キャンセル\n" : "○：選択　×：キャンセル\n";

				int y = CompressBaseY;
				for( int i = 0; i < GetLevelStringVectorSize(); ++ i ) {
					if( i != GetPos() ) y = pdt->DrawLF( MainBaseX, y, GetModeString( i ).c_str() );
					else y = pdt->DrawLF( MainBaseX, y, RGBA_GREEN, GetModeString( i ).c_str() );
				}
			} else if( IsCompressSuccess() ) {
//				oy = pdt->DrawLF( OperationBaseX, oy, "○×：完了" );
				OperationString += "○×：完了\n";
			}
		}
		pdt->Draw( OperationBaseX, OperationBaseY, OperationString.c_str() );

		string SettingString = "現在設定だよ！\n\n圧縮時に逐次ファイル削除：";
		SettingString += GetFileRemove() ? "○" : "×";
		SettingString += "\n(エラーやキャンセルで元に戻せない)";
		pdt->Draw( SettingBaseX, SettingBaseY, SettingString.c_str() );
	}

}
