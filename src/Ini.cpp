
#include <stdlib.h>
#include <string.h>

#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>

#include "Ini.h"

using namespace std;


namespace VpkPacker {

	const char Ini::IniPath[] = "ux0:data/VpkPacker/setting.ini";


	SceUID Ini::OpenIniFile( const unsigned int flag )
	{
		SceUID fid = sceIoOpen( IniPath, flag, 0777 );
		if( fid < 0 ) {
			string tmp = IniPath;
			size_t index = tmp.find_last_of( '/' );
			Directory::CreatePath( tmp.substr( 0, index ) );
			fid = sceIoOpen( IniPath, flag, 0777 );
		}
		return fid;
	}
	bool Ini::ReadIniFileWithClose( const SceUID fid, string *ReadStr )
	{
		SceIoStat stat;
		memset( &stat, 0, sizeof( SceIoStat ) );
		sceIoGetstatByFd( fid, &stat );

		if( stat.st_size ) {
			char *buf = new char[stat.st_size];
			int BytesOfRead = sceIoRead( fid, buf, stat.st_size );
			if( BytesOfRead != stat.st_size ) {
				sceIoClose( fid );
				return true;
			}

			ReadStr->assign( buf, BytesOfRead );
			delete[] buf;
		}
		sceIoClose( fid );

		return false;
	}

	bool Ini::Get( string SectionName, vector<KeyAndValues> *pkv )
	{
		SceUID fid = OpenIniFile( SCE_O_RDONLY );
		if( fid < 0 ) return true;

		string rBuf = "";
		if( ReadIniFileWithClose( fid, &rBuf ) ) return true;

		string SecStr = "";
		SectionName = '[' + SectionName + ']';
		int secStartIndex = rBuf.find( SectionName );
		if( secStartIndex != -1 ) {
			secStartIndex += SectionName.length() + 1;
			int secEndIndex = rBuf.find_first_of( '[', secStartIndex );

			if( secEndIndex == -1 ) {
				SecStr = rBuf.substr( secStartIndex );
			} else {
				SecStr = rBuf.substr( secStartIndex, secEndIndex - secStartIndex );
			}
		} else {
			return true;
		}

		for( size_t i = 0; i < pkv->size(); ++ i ) {
			int index = SecStr.find( ( *pkv )[i].key );
			if( index != -1 ) {
				index += ( *pkv )[i].key.length() + 1;
				int index2 = SecStr.find_first_of( '\n', index );
				if( index2 != -1 ) {
					( *pkv )[i].value = SecStr.substr( index, index2 - index );
				} else {
					( *pkv )[i].value = SecStr.substr( index );
				}
			} else {
				( *pkv )[i].value = "";
			}
		}

		return false;
	}

	bool Ini::Set( string SectionName, const vector<KeyAndValues> kv )
	{
		SceUID fid = OpenIniFile( SCE_O_RDONLY | SCE_O_CREAT );
		if( fid < 0 ) return true;

		string rBuf = "";
		if( ReadIniFileWithClose( fid, &rBuf ) ) return true;

		string SecStr = "";
		SectionName = '[' + SectionName + ']';

		int secStartIndex = rBuf.find( SectionName );
		if( secStartIndex != -1 ) {
			secStartIndex += SectionName.length() + 1;
			int secEndIndex = rBuf.find_first_of( '[', secStartIndex );
			if( secEndIndex == -1 ) {
				SecStr = rBuf.substr( secStartIndex );
				rBuf = rBuf.substr( 0, secStartIndex );
			} else {
				SecStr = rBuf.substr( secStartIndex, secEndIndex - secStartIndex );
				rBuf = rBuf.substr( 0, secStartIndex ) + rBuf.substr( secEndIndex, rBuf.length() );
			}
		} else {
			rBuf.insert( rBuf.length(), SectionName + '\n' );
			secStartIndex = rBuf.length();
		}

		for( size_t i = 0; i < kv.size(); ++ i ) {
			int index = SecStr.length();
			if( SecStr != "" ) {
				index = SecStr.find( kv[i].key );

				if( index != -1 ) {
					int index2 = SecStr.find_first_of( '\n', index );
					if( index2 != -1 ) {
						SecStr = SecStr.substr( 0, index ) + SecStr.substr( index2 + 1 );
					} else {
						SecStr = SecStr.substr( 0, index );
					}
				} else {
					index = SecStr.length();
				}
			}
			SecStr.insert( index, kv[i].key + '=' + kv[i].value + '\n' );
		}
		rBuf.insert( secStartIndex, SecStr );

		fid = sceIoOpen( IniPath, SCE_O_WRONLY | SCE_O_TRUNC, 0777 );
		if( fid < 0 ) return true;
		sceIoWrite( fid, rBuf.c_str(), rBuf.length() );
		sceIoClose( fid );

		return false;
	}
}