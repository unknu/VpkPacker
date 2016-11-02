
#include "Unity.h"
#include "Error.h"

#include "Ini.h"


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
		bool bPadUpdate = Pad::Update();

		if( ( ! Error::Code ) && IsCompressing() ) {
			Compressing();
			if( GetSuccess() ) IncOperationCode();
			if( ! bPadUpdate ) return true;
		}

		if( Error::Code ) {
			switch( GetCurrentButton() ) {
			case SCE_CTRL_CROSS:
			case SCE_CTRL_CIRCLE:
				ResetOperation();
				break;
			}
		} else if( GetOperationCode() < 2 ) {
			switch( GetCurrentButton() ) {
			case SCE_CTRL_UP:
				CurPosUp();
				break;
			case SCE_CTRL_DOWN:
				CurPosDown();
				break;
			case SCE_CTRL_LTRIGGER:
				CurPosPageUp();
				break;
			case SCE_CTRL_RTRIGGER:
				CurPosPageDown();
				break;
			case SCE_CTRL_RIGHT:
				Enter();
				break;
			case SCE_CTRL_LEFT:
				Leave();
				break;
			case SCE_CTRL_TRIANGLE:
				SortChange( true );
				break;
			case SCE_CTRL_CIRCLE:
				if( GetOperationCode() == 0 ) {
					string path = SelectPath();
					if( EbootSearch( path ) ) {
						Kill( Error::NoneEbootFile );
						break;
					}
					IncOperationCode();
					SetSrcPath( path );
					SavePath( "Src", GetSrcPath() );
					ChangeCurPath( 1 );
				} else {
					IncOperationCode();
					SetDestPath( SelectPath() );
					SavePath( "Dest", GetDestPath() );
				}
				break;
			case SCE_CTRL_CROSS:
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
			case SCE_CTRL_CROSS:
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

		if( ! IsCompressing() ) {
			switch( GetCurrentButton() ) {
			case SCE_CTRL_SELECT:
				SetFileRemove( GetFileRemove() ^ true );
				break;
			}
		}

		return bPadUpdate;
	}

	void Unity::ResetOperation()
	{
		ResetOperationCode();
		Error::ResetCode();
		ChangeCurPath( 0 );
	}

}