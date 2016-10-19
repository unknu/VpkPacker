#include <string.h>

#include <algorithm>
#include <functional>

#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>

#include "Directory.h"
#include "Error.h"

using namespace std;


namespace VpkPacker {

	Directory::Directory() : SortNum( 0 )
	{
		pdt = new DrawText();
		SetMaxEntries( MAX_ENTRIES );
		SetPath( "ux0:" );
	}
	Directory::~Directory()
	{
	}

	bool Directory::IsRoot()
	{
		return Path == "";
	}
	void Directory::Draw()
	{
		if( Error::Code != Error::NoneError ) return;

		bool bRoot = IsRoot();

		int y = BaseY;
		if( bRoot ) {
			y = pdt->DrawLF( BaseX, y, "root" );
		} else {
			y = pdt->DrawLF( BaseX, y, Path.c_str() );
		}
		y = pdt->LF( y );

		int PageThreshold = MAX_ENTRIES - 2;
		int FixNum = 0;
		if( GetPos() > PageThreshold ) {
			FixNum = GetPos() - PageThreshold;
			if( GetPos() == GetMaxPos() ) -- FixNum;
		}
		for( int i = 0; i < MAX_ENTRIES; ++ i ) {
			if( (int)vFolders.size() == i ) break;
			int ArrayNum = i + FixNum;
			if( ArrayNum != GetPos() ) y = pdt->DrawLF( BaseX, y, vFolders[ArrayNum].c_str() );
			else y = pdt->DrawLF( BaseX, y, RGBA_GREEN, vFolders[ArrayNum].c_str() );
		}
	}


	void Directory::SetRootPath()
	{
		Path = "";
		vFolders.clear();
		for( int i = 0; i < MAX_ROOT_DIRECTORIES; ++ i ) {
			vFolders.push_back( RootDirectories[i] );
		}
		SetMaxPos( vFolders.size() - 1 );
	}
	void Directory::SetPath( string path )
	{
		ResetPos();
		CreatePath( path );
		Path = path;
		SearchFolder();
	}
	void Directory::CreatePath( string path )
	{
		SceUID dfd = sceIoDopen( path.c_str() );
		if( dfd >= 0 ) {
			sceIoDclose( dfd );
			return;
		}
		size_t index = path.find_last_of( '/' );
		if( index == string::npos ) return;
		CreatePath( path.substr( 0, index ) );

		sceIoMkdir( path.c_str(), 0777 );
	}
	void Directory::Enter()
	{
		if( ! ( IsRoot() || GetPos() ) ) {
			Leave();
			return;
		}

		Path += vFolders[GetPos()];
		PushRecentPos();

		SearchFolder();
	}
	void Directory::Leave()
	{
		if( IsRoot() ) return;

		CheckPopRecentPos();

		Path.erase( Path.length() - 1 );
		size_t index = Path.find_last_of( ":/" );
		if( index != string::npos ) {
			Path.erase( index + 1 );
			SearchFolder();
		} else {
			SetRootPath();
		}
	}
	int Directory::SearchFolder()
	{
		vFolders.clear();
		vFolders.push_back( ".." );

		SceUID dfd = sceIoDopen( Path.c_str() );
		if( dfd < 0 ) {
			return 1;
		}

		int res = 0;
		SceIoDirent dir;
		memset( &dir, 0, sizeof( SceIoDirent ) );
		do {
			res = sceIoDread( dfd, &dir );
			if( res <= 0 ) continue;

			if( SCE_S_ISDIR( dir.d_stat.st_mode ) ) {
				string s = dir.d_name;
				vFolders.push_back( s + "/" );
			}
		} while( res > 0 );

		sceIoDclose( dfd );

		SortChange( false );

		SetMaxPos( vFolders.size() - 1 );

		return 0;
	}

	void Directory::SortChange( bool bInc )
	{
		if( bInc ) {
			if( ++ SortNum > MAX_SORT_NUM ) SortNum = 0;
		}
		if( SortNum ) {
			vector<string>::iterator begin = vFolders.begin();
			if( ! IsRoot() ) ++ begin;
			if( SortNum == 1 ) sort( begin, vFolders.end() );
			if( SortNum == 2 ) sort( begin, vFolders.end(), greater<string>() );
		} else if( bInc && ( ! IsRoot() ) ) {
			SearchFolder();
		}
	}

	string Directory::SelectPath()
	{
		string RetPath = Path;
		if( vFolders[GetPos()] != ".." ) RetPath += vFolders[GetPos()];
		return RetPath;
	}

	bool Directory::EbootSearch( string path )
	{
		SceUID dfd = sceIoDopen( path.c_str() );
		if( dfd < 0 ) return true;

		bool bEbootNotFound = true;
		int res = 0;
		SceIoDirent dir;
		memset( &dir, 0, sizeof( SceIoDirent ) );
		do {
			res = sceIoDread( dfd, &dir );
			if( res <= 0 ) continue;

			if( ! SCE_S_ISDIR( dir.d_stat.st_mode ) ) {
				string s = dir.d_name;
				if( s.find( "eboot.bin" ) == 0 ) {
					bEbootNotFound = false;
					break;
				}
			}
		} while( res > 0 );

		sceIoDclose( dfd );

		return bEbootNotFound;
	}

	int Directory::RemoveDirectory( string TargetDir )
	{
		SceUID dfd = sceIoDopen( TargetDir.c_str() );
		if( dfd < 0 ) {
			return 1;
		}

		int res = 0;
		SceIoDirent dir;
		memset( &dir, 0, sizeof( SceIoDirent ) );
		do {
			res = sceIoDread( dfd, &dir );
			if( res <= 0 ) continue;

			string FindPath = TargetDir + dir.d_name;
			if( SCE_S_ISDIR( dir.d_stat.st_mode ) ) {
				FindPath += '/';
				if( RemoveDirectory( FindPath ) ) return 1;
			} else if( sceIoRemove( FindPath.c_str() ) < 0 ) return 1;
		} while( res > 0 );

		sceIoDclose( dfd );
		if( sceIoRmdir( TargetDir.c_str() ) < 0 ) return 1;

		return 0;
	}

}
