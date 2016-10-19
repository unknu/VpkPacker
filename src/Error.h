#pragma once

#include <string>
#include <vector>

using namespace std;


namespace VpkPacker {

	class Error {
	private:
		Error() {}
		~Error() {}
	public:
		static const string String[];
		enum eCode {
			NoneError,
			NoneSrcFile,
			NoneEbootFile,

			FailedZipOpen,
			FailedZipAddFile,
			FailedZipOpenSrcFile,
			FailedZipCloseSrcFile,

			FailedZipSrcReadFile,
			FailedZipWriteFile,

			FailedZipRenameDestFile,

			FailedRemoveSrcFile,
		};

		static int Code;

		static void ResetCode();
		static string GetString();

		static void SetLog( string log );
		static void SetLog( int i );
		static void SetLog( unsigned int ui );
		static void SetLog( void * vp );

		static void Dump( char *p, int len );
		static void Dump( void *p );
	};

}
