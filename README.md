
nv2D
===============
NVIDIA (c) 2013

Contact:  Rama Hoetzlein, rhoetzlein@nvidia.com

This simple example demonstrates 2D drawing using an immediate-mode 
style API, but with high performance rendering using OpenGL 4.x core 
vertex buffer objects.

The primary work occurs in app_util.cpp. The nvDraw class provides functions
for drawing circles, lines, rectangles, and text. These do not render immediately,
but populate a VBO which is later rendered as a single buffer during 
the 2D draw phase. The illusion of 2D draw order is maintained using the Z-buffer.
Buffers may be draw in dynamic mode (cleared every frame), or in static mode,
in which buffers are saved for later frames.

The app_util.cpp also provides utility classes including meshes, images, 
2D drawing, and GUIs, for making a demo application without any external 
lib dependencies. This file includes:
 - String manipulation: strParse, strSplit, strReplace, strTrim, strExtract
 - nvMesh: Construct, load, and render meshes. PLY format supported
 - nvImg: Cosntruct, load, and render images. PNG and TGA format supported
 - nvDraw: A lightweight, efficient, 2D drawing API. Uses VBOs to render
    lines, circles, triangles, and text. Allows for both static and dynamic 
    groups (rewrite per frame), and immediate style (out-of-order) calling.
 - nvGui: A lightweight class for creating on-screen GUIs. Currently only checkboxes
     or sliders supported. Relies on nvDraw to render.
 - Vector/Matrix: Classes for vector and matrix math
 - Camera: Class for 3D camera projection, matching OpenGL matricies

Useage: 
   1. Main programs implement the functions required by app_opengl/directx.
   2. During display(), first do any rendering desired by your demo or application.
   3. Then call drawGui to render GUI items to the 2D layer.
   4. Then call draw2D to actually render all 2D objects (gui and user-specified)
   5. Finally call SwapBuffers 

Example main.cpp:
   #include "app_opengl.h"
   #include "app_util.h"
   void initialize() {}
   void display () {
       drawGui();		// drawGui asks that all GUIs are rendered in 2D
       draw2D();		// draw2D asks that all nvDraw items are rendered
       SwapBuffers ( g_hDC );	// g_hDC comes from app_opengl.h
   }
