
#include "Unity.h"
#include "Error.h"


namespace VpkPacker {

	Unity::Unity()
	{
		SetIDEnterButton( GetIDEnterButton() );
	}
	Unity::~Unity()
	{
	}
	void Unity::Draw()
	{
		Indicator::Draw();
		if( GetOperationCode() < 2 ) Directory::Draw();
	}
	bool Unity::CheckDraw()
	{
		bool bRet = Pad::Update();
		if( ( ! Error::Code ) && IsCompressing() ) {
			Compressing();
			bRet = true;
			if( GetSuccess() ) IncOperationCode();
		}

		if( ! bRet ) return bRet;

		if( Error::Code ) {
			switch( GetCurrentButton() ) {
			case SCE_CTRL_CROSS:
				ResetOperation();
				break;
			}
		} else if( GetOperationCode() < 2 ) {
			switch( GetCurrentButton() ) {
			case SCE_CTRL_UP:
				Directory::PosUp();
				break;
			case SCE_CTRL_DOWN:
				Directory::PosDown();
				break;
			case SCE_CTRL_LTRIGGER:
				Directory::PosPageUp();
				break;
			case SCE_CTRL_RTRIGGER:
				Directory::PosPageDown();
				break;
			case SCE_CTRL_CIRCLE:
				Enter();
				break;
			case SCE_CTRL_CROSS:
				Leave();
				break;
			case SCE_CTRL_TRIANGLE:
				SortChange( true );
				break;
			case SCE_CTRL_SQUARE:
				if( GetOperationCode() == 0 ) {
					IncOperationCode();
					SetSrcPath( SelectPath() );
					SetPath( DefaultDestPath );
				} else {
					IncOperationCode();
					SetDestPath( SelectPath() );
				}
				break;
			case SCE_CTRL_START:
				ResetOperation();
				break;
			}
		} else if( GetOperationCode() < 3 ) {
			switch( GetCurrentButton() ) {
			case SCE_CTRL_UP:
				Indicator::PosUp();
				break;
			case SCE_CTRL_DOWN:
				Indicator::PosDown();
				break;
			case SCE_CTRL_LTRIGGER:
				Indicator::PosPageUp();
				break;
			case SCE_CTRL_RTRIGGER:
				Indicator::PosPageDown();
				break;
			case SCE_CTRL_CIRCLE:
				IncOperationCode();
				SetLevel( Indicator::GetPos() );
				CompressStart();
				break;
			case SCE_CTRL_CROSS:
			case SCE_CTRL_START:
				ResetOperation();
				break;
			}
		} else if( IsCompressing() ) {
			switch( GetCurrentButton() ) {
			case SCE_CTRL_START:
				Kill( Error::NoneError );
				ResetOperation();
				break;
			}
		} else if( IsCompressSuccess() ) {
			switch( GetCurrentButton() ) {
			case SCE_CTRL_CROSS:
			case SCE_CTRL_CIRCLE:
				ResetOperation();
				break;
			}
		}

		return bRet;
	}

	void Unity::ResetOperation()
	{
		ResetOperationCode();
		Error::ResetCode();
		string path = GetSrcPath();
		SetSrcPath( "" );
		if( path != "" ) {
			path.erase( path.length() - 1 );
			size_t index = path.find_last_of( ":/" );

			if( index != string::npos ) {
				path.erase( index + 1 );
				SetPath( path );
			} else {
				SetRootPath();
			}			
		}
	}

}