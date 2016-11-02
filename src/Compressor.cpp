
#include <stdlib.h>
#include <string.h>

#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>

#include <psp2/kernel/threadmgr.h>

#include "Compressor.h"
#include "Directory.h"

#include "minizip/sceIoapi.h"


namespace VpkPacker {

	Compressor::Compressor() : Level( 0 ), SrcPath( "" ), DestPath( "" ), DestFilePath( "" ), TotalPercent( 0 ), CurPercent( 0 ),
		FileNum( 0 ), FinishFileNum( 0 ),
		TotalFileSize( 0 ), TotalWrittenSize( 0 ), CurFileId( -1 ), CurFileSize( 0 ), CurWrittenSize( 0 ), ReadBuf( NULL ),
		bSuccess( false ), bFileRemove( false ), bakThreadPriority( -1 ),
		z( NULL ), zMethod( 0 )
	{
	}
	Compressor::~Compressor()
	{
		if( z ) {
			zipClose( z, NULL );
			z = NULL;
		}
	}

	int Compressor::GetLevel()
	{
		return Level;
	}
	void Compressor::SetLevel( int l )
	{
		Level = l;
	}

	int Compressor::GetLevelStringVectorSize()
	{
		return CompLevel.size();
	}
	string Compressor::GetModeString( int num )
	{
		return CompLevel[num];
	}
	string Compressor::GetCurModeString()
	{
		return CompLevel[Level];
	}

	string Compressor::GetSrcPath()
	{
		return SrcPath;
	}
	void Compressor::SetSrcPath( string path )
	{
		SrcPath = path;
	}
	string Compressor::GetDestPath()
	{
		return DestPath;
	}
	void Compressor::SetDestPath( string path )
	{
		DestPath = path;
	}


	int Compressor::GetTotalPercent()
	{
		return TotalPercent;
	}
	int Compressor::GetCurPercent()
	{
		return CurPercent;
	}
	int Compressor::GetFileNum()
	{
		return FileNum;
	}
	int Compressor::GetFinishFileNum()
	{
		return FinishFileNum;
	}
	SceOff Compressor::GetTotalFileSize()
	{
		return TotalFileSize;
	}
	SceOff Compressor::GetTotalWrittenSize()
	{
		return TotalWrittenSize;
	}
	SceOff Compressor::GetCurFileSize()
	{
		return CurFileSize;
	}
	SceOff Compressor::GetCurWrittenSize()
	{
		return CurWrittenSize;
	}
	string Compressor::GetCurFileName()
	{
		int num = FinishFileNum;
		if( FinishFileNum == FileNum ) -- num;
		return vFilePath[num];
	}

	bool Compressor::GetSuccess()
	{
		return bSuccess;
	}
	bool Compressor::GetFileRemove()
	{
		return bFileRemove;
	}
	void Compressor::SetFileRemove( bool b )
	{
		bFileRemove = b;
	}


	void Compressor::Kill( Error::eCode ec )
	{
		if( bakThreadPriority != -1 ) {
			sceKernelChangeThreadPriority( sceKernelGetThreadId(), bakThreadPriority );
			bakThreadPriority = -1;
		}

		if( z ) {
			zipCloseFileInZip( z );
			zipClose( z, NULL );
			z = NULL;
			sceIoRemove( ( DestFilePath + ".tmp" ).c_str() );
		}
		if( ReadBuf ) {
			delete[] ReadBuf;
			ReadBuf = NULL;
		}

		DestFilePath = "";
		TotalPercent = CurPercent = 0;
		FileNum = FinishFileNum = 0;
		TotalFileSize = TotalWrittenSize = 0;

		if( CurFileId > 0 ) sceIoClose( CurFileId );
		CurFileId = -1;
		CurFileSize = CurWrittenSize = 0;
		vFilePath.clear();

		bSuccess = false;
		Error::Code = ec;
	}
	bool Compressor::InitAddFiles()
	{
		bakThreadPriority = sceKernelGetThreadCurrentPriority();
		sceKernelChangeThreadPriority( sceKernelGetThreadId(), 0 );

		Kill( Error::NoneError );
		SearchFile( "" );
		CheckFiles();
		if( FileNum == 0 ) {
			Kill( Error::NoneSrcFile );
			return true;
		}
		ReadBuf = new char[ReadBlockSize];
		return false;
	}
	bool Compressor::CheckFiles()
	{
		for( int i = vFilePath.size() - 1; i >= 0; -- i ) {
			string s = vFilePath[i];
			if( s.find( "sce_pfs/" ) == 0 ||
				s.find( "sce_sys/package/" ) == 0 ||
				s.find( "sce_sys/clearsign" ) == 0 ||
				s.find( "sce_sys/keystone" ) == 0 
			) {
				-- FileNum;

				SceIoStat stat;
				memset( &stat, 0, sizeof( SceIoStat ) );
				sceIoGetstat( ( SrcPath + vFilePath[i] ).c_str(), &stat );
				TotalFileSize -= stat.st_size;

				vFilePath.erase( vFilePath.begin() + i );
			}
		}
		return false;
	}
	void Compressor::SearchFile( string TargetPath )
	{
		SceUID dfd = sceIoDopen( ( SrcPath + TargetPath ).c_str() );
		if( dfd < 0 ) return;

		int res = 0;
		SceIoDirent dir;
		memset( &dir, 0, sizeof( SceIoDirent ) );
		do {
			res = sceIoDread( dfd, &dir );
			if( res <= 0 ) continue;

			if( SCE_S_ISDIR( dir.d_stat.st_mode ) ) {
				SearchFile( TargetPath + dir.d_name + "/" );
			} else {
				vFilePath.push_back( TargetPath + dir.d_name );
				++ FileNum;
				TotalFileSize += dir.d_stat.st_size;
			}
		} while( res > 0 );

		sceIoDclose( dfd );
	}
	bool Compressor::CompressStart()
	{
		if( InitAddFiles() ) {
			return true;
		}
		int len = SrcPath.length() - 1;
		string FileName = SrcPath.substr( 0, len );
		int index = FileName.find_last_of( "/:" ) + 1;
		if( index > 0 ) FileName = SrcPath.substr( index, len - index );
		DestFilePath = DestPath + FileName;
		string tmpFilePath = DestFilePath + ".tmp";

		zlib_filefunc64_def zff64;
		memset( &zff64, 0, sizeof( zff64 ) );
		sceInit_zlib_filefunc_def( &zff64 );

		if( ( z = zipOpen2_64( tmpFilePath.c_str(), APPEND_STATUS_CREATE, NULL, &zff64 ) ) == NULL ) {
			Kill( Error::FailedZipOpen );
			return true;
		}
		zMethod = Level ? Z_DEFLATED : 0;

		return AddFile();
	}

	bool Compressor::AddFile()
	{
		CurFileId = sceIoOpen( ( SrcPath + vFilePath[FinishFileNum] ).c_str(), SCE_O_RDONLY, 0777 );
		if( CurFileId < 0 ) {
			Kill( Error::FailedZipOpenSrcFile );
			return true;
		}
		SceIoStat stat;
		memset( &stat, 0, sizeof( SceIoStat ) );
		sceIoGetstatByFd( CurFileId, &stat );
		CurFileSize = stat.st_size;
		CurWrittenSize = 0;

		zip_fileinfo zi;
		memset( &zi, 0, sizeof( zi ) );
		zi.tmz_date.tm_year = stat.st_mtime.year;
		zi.tmz_date.tm_mon = stat.st_mtime.month;
		zi.tmz_date.tm_mday = stat.st_mtime.day;
		zi.tmz_date.tm_hour = stat.st_mtime.hour;
		zi.tmz_date.tm_min = stat.st_mtime.minute;
		zi.tmz_date.tm_sec = stat.st_mtime.second;
		if( zipOpenNewFileInZip64( z, vFilePath[FinishFileNum].c_str(), &zi, NULL, 0, NULL, 0, NULL, zMethod, Level, 1 ) != ZIP_OK ) {
			Kill( Error::FailedZipAddFile );
			return true;
		}

		return false;
	}
	bool Compressor::WriteFile()
	{
		SceOff ws = CurFileSize - CurWrittenSize;
		if( ws ) {
			if( ws > ReadBlockSize ) ws = ReadBlockSize;

			int BytesOfRead = sceIoRead( CurFileId, ReadBuf, ws );
			if( BytesOfRead != ws ) {
				Kill( Error::FailedZipSrcReadFile );
				return true;
			}

			if( zipWriteInFileInZip( z, ReadBuf, ws ) != ZIP_OK ) {
				Kill( Error::FailedZipWriteFile );
				return true;
			}

			CurWrittenSize += ws;
			CurPercent = (float)CurWrittenSize / (float)CurFileSize * 100;
			TotalWrittenSize += ws;
			TotalPercent = (float)TotalWrittenSize / (float)TotalFileSize * 100;
		} else {
			if( sceIoClose( CurFileId ) < 0 ) {
				Kill( Error::FailedZipCloseSrcFile );
				return true;
			}
			CurFileId = -1;

			if( bFileRemove ) sceIoRemove( ( SrcPath + vFilePath[FinishFileNum] ).c_str() );

			if( ++ FinishFileNum != FileNum ) {
				zipCloseFileInZip( z );
				CurPercent = 0;
				AddFile();
			} else {
				zipClose( z, NULL );
				z = NULL;
				if( ReadBuf ) {
					delete[] ReadBuf;
					ReadBuf = NULL;
				}

				sceIoRemove( ( DestFilePath + ".vpk" ).c_str() );
				if( sceIoRename( ( DestFilePath + ".tmp" ).c_str(), ( DestFilePath + ".vpk" ).c_str() ) < 0 ) {
					Kill( Error::FailedZipRenameDestFile );
					return true;
				}
				if( bFileRemove ) {
					if( Directory::RemoveDirectory( SrcPath ) ) {
						Kill( Error::FailedRemoveSrcFile );
						return true;
					}
				}
				if( bakThreadPriority != -1 ) {
					sceKernelChangeThreadPriority( sceKernelGetThreadId(), bakThreadPriority );
					bakThreadPriority = -1;
				}
				bSuccess = true;
			}
		}

		return false;
	}
	bool Compressor::Compressing()
	{
		if( CurFileId < 0 ) {
			if( AddFile() ) return true;
		}
		if( WriteFile() ) return true;

		return false;
	}

}