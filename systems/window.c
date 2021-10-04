#include "window.h"
//#include "render_device.h"


int window_width = 1360;
int window_height = 766;

static void init_context(c_window_t *self){

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("failed to init: %s", SDL_GetError());
		//return -1;
	}
	SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
	SDL_Init(SDL_INIT_GAMECONTROLLER);
	

	int id=0;
	SDL_GameController *pad = SDL_GameControllerOpen( id );
	SDL_Joystick *joy = SDL_GameControllerGetJoystick( pad );
	int instanceID = SDL_JoystickInstanceID( joy );

	// Decide GL+GLSL versions
	#if __APPLE__
	    // GL 3.2 Core + GLSL 150
	    const char* glsl_version = "#version 150";
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	#else
	    // GL 3.0 + GLSL 130
	    const char* glsl_version = "#version 130";
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	#endif

	  // and prepare OpenGL stuff
	  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	  SDL_DisplayMode current;
	  SDL_GetCurrentDisplayMode(0, &current);



	  self->sdl_win = SDL_CreateWindow(
	      "Hello",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1024, 600,
	      SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	      );
	  if (self->sdl_win == NULL) {
	    SDL_Log("Failed to create window: %s", SDL_GetError());
	    //return -1;
	  }



	  self->gl_context = SDL_GL_CreateContext(self->sdl_win);
	  SDL_GL_SetSwapInterval(1);  // enable vsync

	  // Initialize OpenGL loader for cimgui_sdl
	  bool err = Do_gl3wInit() != 0;
	  if (err)
	  {
	    SDL_Log("Failed to initialize OpenGL loader for cimgui_sdl!");
	    //return 1;
	  }

	  // check opengl version sdl uses
	  SDL_Log("opengl version: %s", (char*)glGetString(GL_VERSION));

	  // setup imgui
	  
	  igCreateContext(NULL);


	self->ioptr = igGetIO();
	self->ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui_ImplSDL2_InitForOpenGL(self->sdl_win, self->gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	igStyleColorsDark(NULL);	  
	glewInit();
 

}

void c_window_exit(c_window_t *self)
{
	// clean up
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	igDestroyContext(NULL);

	SDL_GL_DeleteContext(self->gl_context);
	if (self->sdl_win != NULL)
	{
	SDL_DestroyWindow(self->sdl_win);
	self->sdl_win = NULL;
	}
	SDL_Quit();
	SDL_Log("shutting down");
}



void c_window_init(c_window_t *self,int width,int height)
{
	SDL_Log("init window");
	self->quit = false;
	self->width = width;
	self->height = height;

	self->showDemoWindow = false;
	self->showAnotherWindow = false;

	self->clearColor.x = 0.15f;
	self->clearColor.y = 0.15f;
	self->clearColor.z = 0.15f;
	self->clearColor.w = 1.00f;

}


int c_window_resize(c_window_t *self,int w, int h)
{
    glViewport(0, 0, w, h);
	//ratio = (1.0f * w) / h;
	//buildProjectionMatrix(fov, ratio, 0.1f, 1000.0f);
	return 1;
}


c_window_t *c_window_new(int width, int height)
{	
	c_window_t *self;
  	self = malloc(sizeof(c_window_t));
	c_window_init(self,width,height);
	init_context(self);
	return self;
}

int c_window_draw(c_window_t *self,world_t *world)
{


    // start imgui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(self->sdl_win);
    igNewFrame();

    if (self->showDemoWindow)
      igShowDemoWindow(&self->showDemoWindow);

    // show a simple window that we created ourselves.
    {
      static float f = 0.0f;
      static int counter = 0;

      igBegin("debug UI!", NULL, 0);
      igText("camera+view debug");
      igCheckbox("Demo window", &self->showDemoWindow);

      //igSliderFloat("player accel", &player_accel, 0.0f, 2.2f, "%.3f", 0);
     //igSliderFloat("cam lag", &delay, 0.35f,0.9f, "%.3f", 0);
      igSliderFloat("cam xa", &world->cx_angle, -3.0f, 3.0f, "%.3f", 0);
      igSliderFloat("cam ya", &world->cy_angle, -3.0f, 3.0f, "%.3f", 0);
      igSliderFloat("accel", &world->player_accel, 0.0f, 1.0f, "%.3f", 0);
      igSliderFloat3("cam pos", (float*)&world->cam_pos, -150.0f,150.0f, "%.3f", 0);
      igSliderFloat3("player pos", (float*)&world->player_pos, -150.0f,150.0f, "%.3f", 0);
	  igSliderFloat3("player dir", (float*)&world->player_dir, -1.0f,1.0f, "%.3f", 0);
      //igSliderFloat("field of view", &fov, 10.0f,100.0f, "%.3f", 0);
      igColorEdit3("clear color", (float*)&self->clearColor, 0);

      ImVec2 buttonSize;
      buttonSize.x = 0;
      buttonSize.y = 0;
      if (igButton("Button", buttonSize))
        counter++;
      igSameLine(0.0f, -1.0f);
      igText("counter = %d", counter);

      igText("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / igGetIO()->Framerate, igGetIO()->Framerate);
      igText("Delta Time -> %.3f ", igGetIO()->DeltaTime);
      //igText("player dir -> %.3f, %.3f, %.3f ", player_dir[0],player_dir[1],player_dir[2]);
      igEnd();
    }

    //delta
    self->time = igGetIO()->DeltaTime;
    // render

    //SDL_GL_MakeCurrent(window, gl_context);
    glClearColor(self->clearColor.x, self->clearColor.y, self->clearColor.z, self->clearColor.w);
    world->width = (int)self->ioptr->DisplaySize.x;
    world->height = (int)self->ioptr->DisplaySize.y;

    //c_window_resize(self,(int)self->ioptr->DisplaySize.x, (int)self->ioptr->DisplaySize.y);
    //display();
    igRender();

    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
    SDL_GL_SwapWindow(self->sdl_win);

	return 1;
}
