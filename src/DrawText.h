#pragma once

#include <vita2d.h>

#define RGBA_WHITE RGBA8( 0xFF, 0xFF, 0xFF, 0xFF )
#define RGBA_BLACK RGBA8( 0x00, 0x00, 0x00, 0xFF )
#define RGBA_RED RGBA8( 0xFF, 0x00, 0x00, 0xFF )
#define RGBA_BLUE RGBA8( 0x00, 0x00, 0xFF, 0xFF )
#define RGBA_GREEN RGBA8( 0x00, 0xFF, 0x00, 0xFF )


namespace VpkPacker {

	class DrawText {
	private:
		vita2d_pgf *pgf;
	public:
		unsigned int color;
		float scale;
		static const unsigned int LineFeedY = 20;

		DrawText();
		DrawText( unsigned int color );
		DrawText( unsigned int color, float scale );
		~DrawText();

		int LF( int y );
		int DrawLF( int x, int y, const char *text );
		int DrawLF( int x, int y, unsigned int color, const char *text );
		void Draw( int x, int y, const char *text );
		void Draw( int x, int y, unsigned int color, const char *text );
		void Draw( int x, int y, unsigned int color, float scale, const char *text );
	};

}
