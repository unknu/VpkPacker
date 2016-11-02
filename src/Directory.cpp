#include <string.h>

#include <algorithm>
#include <functional>
#include <sstream>

#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>

#include "Directory.h"
#include "Error.h"
#include "Ini.h"

using namespace std;


namespace VpkPacker {

	Directory::Directory() : SortNum( 0 )
	{
		pdt = new DrawText();

		Src.sel.SetMaxEntries( MAX_ENTRIES );
		Src.Path = "";
		Src.sel.ResetPos();
		Src.sel.ResetRecentPos();
		LoadPath( "Src", &Src );

		Dest.sel.SetMaxEntries( MAX_ENTRIES );
		Dest.Path = DefaultDestPath;
		Dest.sel.ResetPos();
		Dest.sel.ResetRecentPos();
		LoadPath( "Dest", &Dest );

		CheckPath( &Src );
		CheckPath( &Dest );
		ChangeCurPath( 0 );
	}
	Directory::~Directory()
	{
	}

	bool Directory::IsRoot()
	{
		return pCurPathSel->Path == "";
	}
	void Directory::Draw()
	{
		if( Error::Code != Error::NoneError ) return;

		bool bRoot = IsRoot();

		int y = BaseY;
		if( bRoot ) {
			y = pdt->DrawLF( BaseX, y, "root" );
		} else {
			y = pdt->DrawLF( BaseX, y, pCurPathSel->Path.c_str() );
		}
		y = pdt->LF( y );

		int PageThreshold = MAX_ENTRIES - 2;
		int FixNum = 0;
		if( pCurPathSel->sel.GetPos() > PageThreshold ) {
			FixNum = pCurPathSel->sel.GetPos() - PageThreshold;
			if( pCurPathSel->sel.GetPos() == pCurPathSel->sel.GetMaxPos() ) -- FixNum;
		}
		for( int i = 0; i < MAX_ENTRIES; ++ i ) {
			if( (int)vFolders.size() == i ) break;
			int ArrayNum = i + FixNum;
			if( ArrayNum != pCurPathSel->sel.GetPos() ) y = pdt->DrawLF( BaseX, y, vFolders[ArrayNum].c_str() );
			else y = pdt->DrawLF( BaseX, y, RGBA_GREEN, vFolders[ArrayNum].c_str() );
		}
	}


	void Directory::SetRootPath()
	{
		pCurPathSel->Path = "";
		vFolders.clear();
		for( int i = 0; i < MAX_ROOT_DIRECTORIES; ++ i ) {
			vFolders.push_back( RootDirectories[i] );
		}
		pCurPathSel->sel.SetMaxPos( vFolders.size() - 1 );
	}
	void Directory::CheckPath( PathAndSelector *ps )
	{
		SceUID dfd = sceIoDopen( ps->Path.c_str() );
		if( dfd >= 0 ) {
			sceIoDclose( dfd );
		} else {
			ps->sel.ResetPos();
			ps->Path = "ux0:";
		}
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
		if( ! ( IsRoot() || pCurPathSel->sel.GetPos() ) ) {
			Leave();
			return;
		}

		pCurPathSel->Path += vFolders[pCurPathSel->sel.GetPos()];
		pCurPathSel->sel.PushRecentPos();

		SearchFolder();
	}
	void Directory::Leave()
	{
		if( IsRoot() ) return;

		pCurPathSel->Path.erase( pCurPathSel->Path.length() - 1 );
		size_t index = pCurPathSel->Path.find_last_of( ":/" );
		if( index != string::npos ) {
			pCurPathSel->Path.erase( index + 1 );
			SearchFolder();
		} else {
			SetRootPath();
		}

		pCurPathSel->sel.CheckPopRecentPos();
	}
	int Directory::SearchFolder()
	{
		vFolders.clear();
		vFolders.push_back( ".." );

		SceUID dfd = sceIoDopen( pCurPathSel->Path.c_str() );
		if( dfd < 0 ) {
			pCurPathSel->sel.SetMaxPos( 0 );
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

		pCurPathSel->sel.SetMaxPos( vFolders.size() - 1 );

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
		string RetPath = pCurPathSel->Path;
		if( vFolders[pCurPathSel->sel.GetPos()] != ".." ) RetPath += vFolders[pCurPathSel->sel.GetPos()];

		return RetPath;
	}
	void Directory::ChangeCurPath( int sel )
	{
		switch( sel ) {
		default:
		case 0:
			pCurPathSel = &Src;
			break;
		case 1:
			pCurPathSel = &Dest;
			break;
		}
		if( IsRoot() ) SetRootPath();
		else SearchFolder();
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

	void Directory::CurPosUp()
	{
		pCurPathSel->sel.PosUp();
	}
	void Directory::CurPosPageUp()
	{
		pCurPathSel->sel.PosPageUp();
	}
	void Directory::CurPosDown()
	{
		pCurPathSel->sel.PosDown();
	}
	void Directory::CurPosPageDown()
	{
		pCurPathSel->sel.PosPageDown();
	}
	int Directory::CurGetPos()
	{
		return pCurPathSel->sel.GetPos();
	}
	vector<int> Directory::CurGetRecentPos()
	{
		return pCurPathSel->sel.GetRecentPos();
	}

	void Directory::SavePath( const char *sec, const string path )
	{
		vector<int> rpos = pCurPathSel->sel.GetRecentPos();
		if( pCurPathSel->sel.GetPos() ) rpos.push_back( pCurPathSel->sel.GetPos() );
		SavePath( sec, path, rpos );
	}
	void Directory::SavePath( const char *sec, const string path, const vector<int> rpos )
	{
		ostringstream sout;
		if( rpos.size() ) {
			for( size_t i = 0; i < rpos.size(); ++ i ) {
				sout << rpos[i] << ",";
			}
		} else {
			sout << 0 << ",";
		}

		vector<KeyAndValues> kv = { { "Path", path },{ "RecentPos", sout.str() } };
		Ini::Set( sec, kv );
	}

	void Directory::LoadPath( const char *sec, PathAndSelector *pas )
	{
		vector<KeyAndValues> kv = { { "Path" },{ "RecentPos" } };
		Ini::Get( sec, &kv );

		if( kv[0].value != "" ) pas->Path = kv[0].value;

		stringstream ss( kv[1].value );
		string buffer;
		while( getline( ss, buffer, ',' ) ) {
			int tmp;
			istringstream( buffer ) >> tmp;
			pas->sel.PushRecentPos( tmp );
		}
	}

}
