#pragma once

#include <string>
#include <vector>

#include "DrawText.h"
#include "Selector.h"

using namespace std;


namespace VpkPacker {

	class Directory : public Selector::Selector {
	private:
		DrawText *pdt;
		string Path;
		vector<string> vFolders;
		int SortNum;

		static const int MAX_ROOT_DIRECTORIES = 9;
		string RootDirectories[MAX_ROOT_DIRECTORIES] = { "os0:","pd0:","sa0:","tm0:","ud0:","ur0:","ux0:","vd0:","vs0:" };
		static const int MAX_ENTRIES = 15;

		static const unsigned int BaseX = 15;
		static const unsigned int BaseY = 220;

		static const int MAX_SORT_NUM = 2;
	public:

		Directory();
		~Directory();

		bool IsRoot();
		void Draw();

		void SetRootPath();
		void SetPath( string path );
		void CreatePath( string path );
		void Enter();
		void Leave();
		int SearchFolder();
		void SortChange( bool bInc );

		string SelectPath();
	};

}
