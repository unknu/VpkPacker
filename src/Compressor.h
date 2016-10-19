#pragma once

#include <string>
#include <vector>

#include "Error.h"

#include "minizip/zip.h"

using namespace std;


namespace VpkPacker {

	class Compressor {
	private:
		int Level;
		string SrcPath;
		string DestPath;
		string DestFilePath;
		int TotalPercent;
		int CurPercent;
		vector<string> vFilePath;
		int FileNum;
		int FinishFileNum;
		SceOff TotalFileSize;
		SceOff TotalWrittenSize;
		SceUID CurFileId;
		SceOff CurFileSize;
		SceOff CurWrittenSize;
		const SceOff ReadBlockSize = 1024 * 256;
		char *ReadBuf;
		bool bSuccess;

		const vector<string> CompLevel = { "0:無圧縮", "1:最速", "2:", "3:", "4:", "5:", "6:標準", "7:", "8:", "9:最高圧縮", };

		zipFile z;
		int zMethod;

	public:
		Compressor();
		~Compressor();

		int GetLevel();
		void SetLevel( int l );

		int GetLevelStringVectorSize();
		string GetModeString( int num );
		string GetCurModeString();

		string GetSrcPath();
		void SetSrcPath( string path );
		string GetDestPath();
		void SetDestPath( string path );

		int GetTotalPercent();
		int GetCurPercent();
		int GetFileNum();
		int GetFinishFileNum();
		SceOff GetTotalFileSize();
		SceOff GetTotalWrittenSize();
		SceOff GetCurFileSize();
		SceOff GetCurWrittenSize();
		string GetCurFileName();
		bool GetSuccess();

		void Kill( Error::eCode ec );
		bool InitAddFiles();
		bool CheckFiles();
		void SearchFile( string TargetPath );
		bool CompressStart();
		bool AddFile();
		bool WriteFile();
		bool Compressing();

	};

}