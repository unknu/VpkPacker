
#include <sstream>

#include "Indicator.h"
#include "Error.h"


namespace VpkPacker {

	const char Indicator::Version[] = "ver beta2";

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
		int y = MainBaseY;
		string name = "VpkPacker ";
		name += Version;
		y = pdt->DrawLF( MainBaseX, y, name.c_str() );
		y = pdt->LF( y );

		if( Error::Code ) {
			y = pdt->DrawLF( MainBaseX, y, "エラーだよ！" );
			y = pdt->LF( y );
			y = pdt->DrawLF( MainBaseX, y, Error::GetString().c_str() );
		} else if( OperationCode == 0 ) {
			y = pdt->DrawLF( MainBaseX, y, "vpkの作成元フォルダを選択してね！" );
		} else if( OperationCode > 0 ) {
			y = pdt->DrawLF( MainBaseX, y, "vpkの作成元フォルダだよ！" );
			y = pdt->DrawLF( MainBaseX, y, GetSrcPath().c_str() );
			y = pdt->LF( y );
			if( OperationCode == 1 ) {
				y = pdt->DrawLF( MainBaseX, y, "vpkの作成先フォルダを選択してね！" );
			} else if( OperationCode >= 2 ) {
				y = pdt->DrawLF( MainBaseX, y, "vpkの作成先フォルダだよ！" );
				y = pdt->DrawLF( MainBaseX, y, GetDestPath().c_str() );
				y = pdt->LF( y );
			}
			if( OperationCode == 2 ) {
				y = pdt->DrawLF( MainBaseX, y, "vpkの圧縮方式を選択してね！" );
				y = pdt->LF( y );
				y = pdt->LF( y );
			} else if( OperationCode >= 3 ) {
				y = pdt->DrawLF( MainBaseX, y, "vpkの圧縮方式だよ！" );
				y = pdt->DrawLF( MainBaseX, y, GetCurModeString().c_str() );
				y = pdt->LF( y );
			}
			if( IsCompressing() ) {
				y = pdt->DrawLF( MainBaseX, y, "vpkの作成中だよ！ちょっと待っててね！" );
				y = pdt->LF( y );
			}
			if( IsCompressing() || IsCompressSuccess() ) {
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
			}
			if( IsCompressSuccess() ) {
				y = pdt->LF( y );
				y = pdt->LF( y );
				y = pdt->DrawLF( MainBaseX, y, "vpk作成完了だよ！やったね！" );
			}
		}

		int oy = MainBaseY;
		oy = pdt->DrawLF( OperationBaseX, oy, "操作方法だよ！" );
		oy = pdt->LF( oy );
		if( Error::Code ) {
			string s = "×：キャンセル";
			if( IDEnterButton ) {
				s = "○：キャンセル";
			}
			oy = pdt->DrawLF( OperationBaseX, oy, s.c_str() );
		} else if( OperationCode < 2 ) {
			oy = pdt->DrawLF( OperationBaseX, oy, "↓↑○×ＬＲ：移動" );
			oy = pdt->DrawLF( OperationBaseX, oy, "△：ソート　□：選択" );
			oy = pdt->DrawLF( OperationBaseX, oy, "START：選択解除" );
		} else if( OperationCode == 2 ) {
			oy = pdt->DrawLF( OperationBaseX, oy, "↓↑ＬＲ：移動" );
			string s = "○：選択　×：キャンセル";
			if( IDEnterButton ) {
				s = "×：選択　○：キャンセル";
			}
			oy = pdt->DrawLF( OperationBaseX, oy, s.c_str() );

			for( int i = 0; i < GetLevelStringVectorSize(); ++ i ) {
				if( i != GetPos() ) y = pdt->DrawLF( MainBaseX, y, GetModeString( i ).c_str() );
				else y = pdt->DrawLF( MainBaseX, y, RGBA_GREEN, GetModeString( i ).c_str() );
			}
		} else if( IsCompressing() ) {
			oy = pdt->DrawLF( OperationBaseX, oy, "START：キャンセル" );
		} else if( IsCompressSuccess() ) {
			oy = pdt->DrawLF( OperationBaseX, oy, "○×：完了" );
		}
	}

}
