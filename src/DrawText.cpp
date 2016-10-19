
#include "DrawText.h"

namespace VpkPacker {

	DrawText::DrawText() : color( RGBA_WHITE ), scale( 1.0f )
	{
		pgf = vita2d_load_default_pgf();
	}
	DrawText::DrawText( unsigned int color ) : color( color ), scale( 1.0f )
	{
		pgf = vita2d_load_default_pgf();
	}
	DrawText::DrawText( unsigned int color, float scale ) : color( color ), scale( scale )
	{
		pgf = vita2d_load_default_pgf();
	}
	DrawText::~DrawText()
	{
		vita2d_free_pgf( pgf );
	}

	int DrawText::LF( int y )
	{
		return y + LineFeedY * scale;
	}
	int DrawText::DrawLF( int x, int y, const char *text )
	{
		vita2d_pgf_draw_text( pgf, x, y, color, scale, text );
		return y + LineFeedY * scale;
	}
	int DrawText::DrawLF( int x, int y, unsigned int color, const char *text )
	{
		vita2d_pgf_draw_text( pgf, x, y, color, scale, text );
		return y + LineFeedY * scale;
	}
	void DrawText::Draw( int x, int y, const char *text )
	{
		vita2d_pgf_draw_text( pgf, x, y, color, scale, text );
	}
	void DrawText::Draw( int x, int y, unsigned int color, const char *text )
	{
		vita2d_pgf_draw_text( pgf, x, y, color, scale, text );
	}
	void DrawText::Draw( int x, int y, unsigned int color, float scale, const char *text )
	{
		vita2d_pgf_draw_text( pgf, x, y, color, scale, text );
	}

}
