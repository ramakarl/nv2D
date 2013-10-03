
// NVIDIA
// R.Hoetzlein
// 

// nv2D 
// No other include or lib dependencies other than DX or GL.
// Does not use GLUT, but gives GLUT-style function interface
#ifdef USE_DX
	#include "app_directx.h"
#else
	#include "app_opengl.h"
#endif
#include "app_util.h"
#include "app_perf.h"

// Simple demo
int		numCircles = 100;
int		numRects = 100;
int		numLines = 100;
int		numText = 100;
float	frameTime;

// Main display loop
void display () 
{
	PERF_PUSH ( "frame" );		// instrument code (does CPU and GPU perf)
	
	glClearColor( 0.5, 0.5, 0.5, 1.0 );
	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Draw 2D funcs similar to immediate mode, but uses VBO buffers internally
	// Draw order maintained. Transparency allowed.
	// start2D() does dynamic redraw every frame. 
	// static2D() will redraw automatically, can be infrequently updated.
	// Commands:
	//    start2D, static2D, end2D, setview2D
	//    drawLine, drawRect, drawFill, drawText, setText
	//    drawCircle, drawCircleFill, drawCircleDash
	// Future goals (partially implemented):
	//    - Ability to change model/view matrix per start2D/end2D sets
	//    - Add/remove static2D sets

	start2D();			
	int x1, y1, x2, y2;
	float r, g, b;
	for (int n=0; n < numRects; n++ ) {
		x1 = n % 40; y1 = n / 40;
		drawFill ( x1*30+30-14, y1*30+200-14, x1*30+30+14, y1*30+200+14, 0, float(y1)/40.0, float(x1)/40.0, 1 );		
	}
	for (int n=0; n < numCircles; n++ ) {
		x1 = n % 40; y1 = n / 40;
		drawCircleFill ( x1*30+30, y1*30+200, 14, float(x1)/40.0, float(y1)/40.0, 1, 1 );
		drawCircleDash ( x1*30+30, y1*30+200, 14, 1, 1, 1, 1 );
	}
	srand ( 3142 );
	for (int n=0; n < numLines; n++ ) {
		x1 = (rand()*window_width)/RAND_MAX;	y1 = (rand()*window_height)/RAND_MAX;
		x2 = (rand()*window_width)/RAND_MAX;	y2 = (rand()*window_height)/RAND_MAX;
		r = float(rand())/RAND_MAX; g = float(rand())/RAND_MAX; b = float(rand())/RAND_MAX; 
		drawLine ( x1, y1+200, x2, y2+200, r, g, b, 1 );
	}
	end2D ();

	// multiple start/end blocks allowed per frame	
	start2D ();	
	char msg[128];
	for (int n=0; n < numText; n++ ) {		
		x1 = n % 40; y1 = n / 40;
		sprintf ( msg, "%d,%d", x1, y1 );
		drawText ( x1*30+30-12, y1*30+200, msg, 1, 1, 1, 1);
	}
	end2D();

	// DrawGui - required to draw GUI
	drawGui ();	

	// Draw2D - required to draw nv2D
	draw2D ();

	frameTime = PERF_POP ();  // frame

	SwapBuffers( g_hDC );  		
}

void reshape ( int width, int height ) 
{
	// set window height and width
	window_width  = (float) width;
	window_height = (float) height;
	glViewport( 0, 0, width, height );  
	setview2D ( window_width, window_height );
}

void keyboard_func ( unsigned char key, int x, int y )
{
	switch ( key ) {		
	case 27:
		exit ( 0 ); 
		break;
	};
}

void mouse_click_func ( int button, int state, int x, int y )
{
  if ( state==GLUT_DOWN && guiMouseDown ( x, y ) ) return;		// event handling for nv2D GUIs
  // any other event handling you want..
}

void mouse_move_func ( int x, int y )
{
}

void mouse_drag_func ( int x, int y )
{
	if ( guiMouseDrag ( x, y ) ) return;	// event handling for nv2D GUIs
	// any other event handling you want..
}

void idle_func ()
{
}

void initialize ()
{
	// GUI
	addGui (  20,  20, 200, 24, "Frame Time (ms)",		GUI_PRINT,  GUI_FLOAT, &frameTime, 0, 0 );	
	addGui (  20,  50, 200, 24, "Number of Rectangles",	GUI_SLIDER, GUI_INT, &numRects, 0, 1000 );
	addGui (  20,  80, 200, 24, "Number of Circles",	GUI_SLIDER, GUI_INT, &numCircles, 0, 1000 );
	addGui (  20, 110, 200, 24, "Number of Lines",		GUI_SLIDER, GUI_INT, &numLines, 0, 10000 );	
	addGui (  20, 140, 200, 24, "Number of Text",		GUI_SLIDER, GUI_INT, &numText, 0, 1000 );
	
	glViewport( 0, 0, window_width, window_height ); 

	// required init functions
	init2D ( "arial_24" );		// specify font file (.bin/tga)
	setText ( 0.5, -0.5 );		// scale by 0.5, kerning adjust -0.5 pixels
	setview2D ( window_width, window_height );
	setorder2D ( true, -.00001 );

	PERF_INIT ( false );				// Enable CPU perf (do not require nv-perfmarker DLL)
	PERF_SET ( true, 2, false, "" );	// Perf CPU on, Perf print depth, nv-perfmarkers off, no log file
}

void shutdown ()
{

}
