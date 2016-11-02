#pragma once

#include <string>
#include <vector>

#include "Directory.h"

using namespace std;


namespace VpkPacker {

	typedef struct {
		string key;
		string value;
	} KeyAndValues;

	class Ini {
	private:
		Ini() {}
		~Ini() {}


		static const char IniPath[];

		static SceUID OpenIniFile( const unsigned int flag );
		static bool ReadIniFileWithClose( const SceUID fid, string *ReadStr );
	public:
		static bool Get( string SectionName, vector<KeyAndValues> *pkv );
		static bool Set( string SectionName, const vector<KeyAndValues> kv );
	};

}