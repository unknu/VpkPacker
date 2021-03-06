#pragma once

#include <vector>

using namespace std;


namespace VpkPacker {

	class Selector {
	private:
		vector<int> RecentPos;
		int Pos;
		int MaxPos;

		int MaxEntries;
	public:

		Selector();
		~Selector();

		int GetMaxEntries();
		void SetMaxEntries( int me );

		int GetPos();
		void SetPos( int pos );
		void ResetPos();

		int GetMaxPos();
		void SetMaxPos( int mp );

		vector<int> GetRecentPos();

		void CheckPopRecentPos();
		void PopRecentPos();
		void PushRecentPos();
		void PushRecentPos( int pos );
		void ResetRecentPos();

		void PosUp();
		void PosPageUp();
		void PosDown();
		void PosPageDown();

	};

}