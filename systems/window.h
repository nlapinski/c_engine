
#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "../cimgui_extras.h"
#include "cimgui_impl.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>

#include "../components/world.h"
#include <cglm/cglm.h>   /* for inline */
#include <cglm/struct.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string.h>

#ifdef _MSC_VER
#include <windows.h>
#endif

#ifdef IMGUI_HAS_IMSTR
#define igBegin igBegin_Str
#define igSliderFloat igSliderFloat_Str
#define igCheckbox igCheckbox_Str
#define igColorEdit3 igColorEdit3_Str
#define igButton igButton_Str
#endif

extern int window_width;
extern int window_height;


typedef struct c_window_t
{
	int width, height;
	SDL_Window *sdl_win;
	SDL_GLContext gl_context;
	bool quit;
	float time;
	ImGuiIO* ioptr;
	bool showDemoWindow;
	bool showAnotherWindow;
	ImVec4 clearColor;
	SDL_Event e;


} c_window_t;

int c_window_resize(c_window_t *self,int w, int h);
void c_window_init(c_window_t *self,int width,int height);
c_window_t *c_window_new(int width, int height);
void c_window_exit(c_window_t *self);
int c_window_draw(c_window_t *self,world_t *world);
#endif /* !WINDOW_H */

