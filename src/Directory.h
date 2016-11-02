#pragma once

#include <string>
#include <vector>

#include "DrawText.h"
#include "Selector.h"

using namespace std;


namespace VpkPacker {

	typedef struct _PathAndSelector{
		string Path;
		Selector sel;
	} PathAndSelector;

	class Directory {
	private:
		DrawText *pdt;
		PathAndSelector Src;
		PathAndSelector Dest;
		PathAndSelector *pCurPathSel;
		vector<string> vFolders;
		int SortNum;

		const char DefaultDestPath[15] = "ux0:data/vpks/";

		static const int MAX_ROOT_DIRECTORIES = 9;
		string RootDirectories[MAX_ROOT_DIRECTORIES] = { "os0:", "pd0:", "sa0:", "tm0:", "ud0:", "ur0:", "ux0:", "vd0:", "vs0:" };
		static const int MAX_ENTRIES = 15;

		static const unsigned int BaseX = 15;
		static const unsigned int BaseY = 220;

		static const int MAX_SORT_NUM = 2;

		void SavePath( const char *sec, const string path, const vector<int> rpos );
	public:

		Directory();
		~Directory();

		bool IsRoot();
		void Draw();

		void SetRootPath();
		void CheckPath( PathAndSelector *ps );
		static void CreatePath( string path );
		void Enter();
		void Leave();
		int SearchFolder();
		void SortChange( bool bInc );

		string SelectPath();
		void ChangeCurPath( int sel );
		bool EbootSearch( string path );

		static int RemoveDirectory( string TargetDir );

		void CurPosUp();
		void CurPosPageUp();
		void CurPosDown();
		void CurPosPageDown();
		int CurGetPos();
		vector<int> CurGetRecentPos();
		void SavePath( const char *sec, const string path );
		void LoadPath( const char *sec, PathAndSelector *pas );
	};

}
