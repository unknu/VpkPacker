
#include <stdlib.h>
#include <string.h>

#include <sstream>

#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>

#include "Ini.h"

using namespace std;


namespace VpkPacker {

	const char Ini::IniPath[] = "app0:setting.ini";

	void Ini::SetKey( vector<KeyAndValues> kv )
	{
		SceUID fid = sceIoOpen( IniPath, SCE_O_RDONLY | SCE_O_CREAT, 0777 );
		if( fid < 0 ) return;

		SceIoStat stat;
		memset( &stat, 0, sizeof( SceIoStat ) );
		sceIoGetstatByFd( fid, &stat );

		string tmp = "";
		if( stat.st_size ) {
			char *buf = new char[stat.st_size];
			int BytesOfRead = sceIoRead( fid, buf, stat.st_size );
			if( BytesOfRead != stat.st_size ) return;

			tmp = buf;
			delete [] buf;
			for( size_t i = 0; i < kv.size(); ++ i ) {
				int index = tmp.find( kv[i].key );
				if( index != -1 ) {
					int index2 = index = tmp.find( "\r\n", index );
					tmp = tmp.substr( 0, index ) + tmp.substr( index2 + 2 );
				}
				tmp.insert( 0, kv[i].key + kv[i].value );
			}
		} else {
			for( size_t i = 0; i < kv.size(); ++ i ) {
				tmp.insert( 0, kv[i].key + kv[i].value );
			}
		}
		sceIoClose( fid );

		fid = sceIoOpen( IniPath, SCE_O_WRONLY | SCE_O_TRUNC, 0777 );
		if( fid < 0 ) return;
		sceIoWrite( fid, tmp.c_str(), tmp.length() );
		sceIoClose( fid );
	}
	void Ini::SetPath( const char *keypath, string path, vector<int> rp )
	{
		ostringstream sout;
		if( rp.size() ) {
			for( size_t i = 0; i < rp.size(); ++ i ) {
				sout << i << ",";
			}
		} else {
			sout << 0 << ",";
		}
		vector<KeyAndValues> vkv = { { keypath, path }, { "RecentPos", sout.str() } };
		SetKey( vkv );
	}
	void Ini::SetSrcPath( string path, vector<int> rp )
	{
		SetPath( "SrcPath", path, rp );
		}
	void Ini::SetDestPath( string path, vector<int> rp )
	{
		SetPath( "DestPath", path, rp );
	}
}