
#include <sstream>

#include <psp2/io/fcntl.h>

#include "Error.h"


namespace VpkPacker {

	const string Error::String[] = {
		"ノーエラー！",
		"作成元のファイルがないよ！",
		"eboot.binファイルがないよ！",

		"vpk作成に失敗したよ！",
		"vpkへのファイル追加に失敗したよ！",
		"作成元のファイルのオープンに失敗したよ！",
		"作成元のファイルのクローズに失敗したよ！",

		"作成元のファイル読み込みに失敗したよ！",
		"vpkへのデータ書き込みに失敗したよ！",

		"vpkファイルのリネームに失敗したよ！",

		"作成元のフォルダの削除に失敗したよ！",
	};
	int Error::Code = 0;

	void Error::ResetCode()
	{
		Code = 0;
	}
	string Error::GetString()
	{
		return String[Code];
	}

	void Error::SetLog( string log )
	{
		SceUID fd = sceIoOpen( "ux0:vpkpacker_log.txt", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 0777 );
		if( fd >= 0 ) {
			string s = log + "\r\n";
			sceIoWrite( fd, s.c_str(), s.length() );
			sceIoClose( fd );
		}
	}
	void Error::SetLog( int i )
	{
		ostringstream sout;
		sout << i;
		SetLog( sout.str() );
	}
	void Error::SetLog( unsigned int ui )
	{
		ostringstream sout;
		sout << ui;
		SetLog( sout.str() );
	}
	void Error::SetLog( void * vp )
	{
		ostringstream sout;
		sout << vp;
		SetLog( sout.str() );
	}


	void Error::Dump( char *p, int len )
	{
		SceUID fd = sceIoOpen( "ux0:vpkpacker_log.txt", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 0777 );
		if( fd >= 0 ) {
			char *buf = new char[len];
			for( int i = 0; i < len; ++i ) {
				buf[i] = *(p + i);
			}
			sceIoWrite( fd, buf, len * sizeof( char ) );
			sceIoClose( fd );
			delete[] buf;
		}
	}
	void Error::Dump( void *p )
	{
		SceUID fd = sceIoOpen( "ux0:vpkpacker_log.txt", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 0777 );
		if( fd >= 0 ) {
			sceIoWrite( fd, (void *)p, sizeof( void * ) );
			sceIoClose( fd );
		}
	}

}
