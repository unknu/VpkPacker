
#include "Selector.h"


namespace VpkPacker {

	Selector::Selector() : MaxEntries( 0 )
	{
		ResetPos();
	}
	Selector::~Selector()
	{
	}

	int Selector::GetPos()
	{
		return Pos;
	}
	void Selector::SetPos( int pos )
	{
		Pos = pos;
	}
	void Selector::ResetPos()
	{
		Pos = 0;
		RecentPos.clear();
	}

	int Selector::GetMaxEntries()
	{
		return MaxEntries;
	}
	void Selector::SetMaxEntries( int me )
	{
		MaxEntries = me;
	}
	int Selector::GetMaxPos()
	{
		return MaxPos;
	}
	void Selector::SetMaxPos( int mp )
	{
		MaxPos = mp;
	}

	vector<int> Selector::GetRecentPos()
	{
		return RecentPos;
	}

	void Selector::CheckPopRecentPos()
	{
		if( RecentPos.size() ) {
			PopRecentPos();
			if( Pos > MaxPos ) Pos = MaxPos;
		} else Pos = 0;
	}
	void Selector::PopRecentPos()
	{
		Pos = RecentPos.back();
		RecentPos.pop_back();
	}

	void Selector::PushRecentPos()
	{
		RecentPos.push_back( Pos );
		Pos = 0;
	}
	void Selector::PushRecentPos( int pos )
	{
		RecentPos.push_back( pos );
	}

	void Selector::ResetRecentPos()
	{
		RecentPos.clear();
	}

	void Selector::PosUp()
	{
		if( Pos > 0 ) -- Pos;
		else Pos = MaxPos;
	}
	void Selector::PosPageUp()
	{
		if( Pos <= 0 ) return;
		Pos -= MaxEntries;
		if( Pos < 0 ) Pos = 0;
	}
	void Selector::PosDown()
	{
		if( Pos < MaxPos ) ++ Pos;
		else Pos = 0;
	}
	void Selector::PosPageDown()
	{
		if( Pos >= MaxPos ) return;
		Pos += MaxEntries;
		if( Pos > MaxPos ) Pos = MaxPos;
	}

}
