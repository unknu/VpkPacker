#pragma once

#include <string>
#include <vector>

using namespace std;


namespace VpkPacker {

	class Ini {
	private:
		Ini() {}
		~Ini() {}

		typedef struct {
			string key;
			string value;
		} KeyAndValues;

		static const char IniPath[];

		static void SetKey( vector<KeyAndValues> kv );
	public:
		static void SetPath( const char *keypath, string path, vector<int> rp );
		static void SetSrcPath( string path, vector<int> rp );
		static void SetDestPath( string path, vector<int> rp );

	};

}