
#include <psp2/io/fcntl.h>

#include "sceIoapi.h"


#define SCE_ERROR_ERRNO_ENODEV 0x80010013

typedef struct {
	SceUID uid;
	int error;
	/*
	int mode;
	SceOff offset;
	void *filename;
	*/
} sceFILE;


static voidpf ZCALLBACK sce_fopen64_file_func( voidpf opaque, const void* filename, int mode )
{
	if( filename == NULL ) return NULL;

	SceUID uid = -1;
	int mode_fopen = 0;
	if( ( mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER ) == ZLIB_FILEFUNC_MODE_READ ) {
		mode_fopen = SCE_O_RDONLY;
	} else if( mode & ZLIB_FILEFUNC_MODE_EXISTING ) {
		mode_fopen = SCE_O_RDWR;
	} else if( mode & ZLIB_FILEFUNC_MODE_CREATE ) {
		mode_fopen = SCE_O_WRONLY | SCE_O_CREAT;
	}

	if( mode_fopen == 0 ) return NULL;

	uid = sceIoOpen( (char *)filename, mode_fopen, 0777 );
	if( uid < 0 ) return NULL;

	sceFILE *sf = (sceFILE *)malloc( sizeof( sceFILE ) );
	sf->uid = uid;
	sf->error = 0;
	/*
	sf->mode = mode_fopen;
	sf->offset = 0;
	int filename_len = strlen( filename ) + 1;
	sf->filename = (char *)malloc( filename_len * sizeof( char ) );
	strncpy( sf->filename, filename, filename_len );
	*/

	return (voidpf)sf;
}

static uLong ZCALLBACK sce_fread_file_func( voidpf opaque, voidpf stream, void* buf, uLong size )
{
	sceFILE *sf = (sceFILE *)stream;
	if( sf == NULL ) return -1;

	int ret = sceIoRead( sf->uid, buf, (SceSize)size );
	/*
	if( ret == SCE_ERROR_ERRNO_ENODEV ) {
		sf->uid = sceIoOpen( sf->filename, sf->mode, 0777 );
		if( sf->uid >= 0 ) {
			sceIoLseek( sf->uid, sf->offset, SCE_SEEK_SET );
			ret = sceIoRead( sf->uid, buf, (SceSize)size );
		}
	}
	if( ret > 0 ) sf->offset += ret;
	else sf->error = -1;
	*/
	if( ret <= 0 ) sf->error = -1;

	return (uLong)ret;
}

static uLong ZCALLBACK sce_fwrite_file_func( voidpf opaque, voidpf stream, const void* buf, uLong size )
{
	sceFILE *sf = (sceFILE *)stream;
	if( sf == NULL ) return -1;

	int ret = sceIoWrite( sf->uid, buf, (SceSize)size );
	/*
	if( ret == SCE_ERROR_ERRNO_ENODEV ) {
		sf->uid = sceIoOpen( sf->filename, sf->mode, 0777 );
		if( sf->uid >= 0 ) {
			sceIoLseek( sf->uid, sf->offset, SCE_SEEK_SET );
			ret = sceIoWrite( sf->uid, buf, (SceSize)size );
		}
	}*/
	/*
	if( ret > 0 ) sf->offset += ret;
	else sf->error = -1;
	*/
	if( ret <= 0 ) sf->error = -1;

	return (uLong)ret;
}

static ZPOS64_T ZCALLBACK sce_ftell64_file_func( voidpf opaque, voidpf stream )
{
	sceFILE *sf = (sceFILE *)stream;
	if( sf == NULL ) return -1;

	ZPOS64_T ret = (ZPOS64_T)sceIoLseek( sf->uid, 0, SCE_SEEK_CUR );
	/*
	if( ret == SCE_ERROR_ERRNO_ENODEV ) {
		sf->uid = sceIoOpen( sf->filename, sf->mode, 0777 );
		if( sf->uid >= 0 ) {
			sceIoLseek( sf->uid, sf->offset, SCE_SEEK_SET );
			ret = (ZPOS64_T)sceIoLseek( sf->uid, 0, SCE_SEEK_CUR );
		}
	}*/
	sf->error = (int)ret;
	return ret;
}

static long ZCALLBACK sce_fseek64_file_func( voidpf  opaque, voidpf stream, ZPOS64_T offset, int origin )
{
	sceFILE *sf = (sceFILE *)stream;
	if( sf == NULL ) return -1;

	int fseek_origin = 0;
	switch( origin ) {
	case ZLIB_FILEFUNC_SEEK_CUR:
		fseek_origin = SCE_SEEK_CUR;
		break;
	case ZLIB_FILEFUNC_SEEK_END:
		fseek_origin = SCE_SEEK_END;
		break;
	case ZLIB_FILEFUNC_SEEK_SET:
		fseek_origin = SCE_SEEK_SET;
		break;
	default: return -1;
	}

	SceOff sof = sceIoLseek( sf->uid, offset, fseek_origin );
	/*
	if( (int)sof == SCE_ERROR_ERRNO_ENODEV ) {
		sf->uid = sceIoOpen( sf->filename, sf->mode, 0777 );
		if( sf->uid >= 0 ) {
			sceIoLseek( sf->uid, sf->offset, SCE_SEEK_SET );
			sof = sceIoLseek( sf->uid, offset, fseek_origin );
		}
	}*/
	/*
	if( sof < 0 ) return -1;
	else sf->offset = sof;
	*/
	if( sof <= 0 ) sf->error = -1;

	return 0;
}

static int ZCALLBACK sce_fclose_file_func( voidpf opaque, voidpf stream )
{
	sceFILE *sf = (sceFILE *)stream;
	if( sf == NULL ) return -1;

//	if( sf->filename != NULL ) free( sf->filename );

	int ret = sceIoClose( sf->uid );
	/*
	if( ret == SCE_ERROR_ERRNO_ENODEV ) {
		sf->uid = sceIoOpen( sf->filename, sf->mode, 0777 );
		if( sf->uid >= 0 ) {
			sceIoLseek( sf->uid, sf->offset, SCE_SEEK_SET );
			ret = sceIoClose( sf->uid );
		}
	}*/
	free( sf );
	return ret;
}

static int ZCALLBACK sce_ferror_file_func( voidpf opaque, voidpf stream )
{
	sceFILE *sf = (sceFILE *)stream;
	if( sf == NULL ) return -1;
	if( sf->error < 0 ) return sf->error;
	return 0;
}



void sceInit_zlib_filefunc_def( zlib_filefunc64_def *pzlib_filefunc_def )
{
	pzlib_filefunc_def->zopen64_file = sce_fopen64_file_func;
	pzlib_filefunc_def->zread_file = sce_fread_file_func;
	pzlib_filefunc_def->zwrite_file = sce_fwrite_file_func;
	pzlib_filefunc_def->ztell64_file = sce_ftell64_file_func;
	pzlib_filefunc_def->zseek64_file = sce_fseek64_file_func;
	pzlib_filefunc_def->zclose_file = sce_fclose_file_func;
	pzlib_filefunc_def->zerror_file = sce_ferror_file_func;
	pzlib_filefunc_def->opaque = NULL;
}
