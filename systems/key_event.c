#include "key_event.h"


/* This is indexed by SDL_GameControllerButton. */
static const struct { int x; int y; } button_positions[] = {
    {387, 167},  /* SDL_CONTROLLER_BUTTON_A */
    {431, 132},  /* SDL_CONTROLLER_BUTTON_B */
    {342, 132},  /* SDL_CONTROLLER_BUTTON_X */
    {389, 101},  /* SDL_CONTROLLER_BUTTON_Y */
    {174, 132},  /* SDL_CONTROLLER_BUTTON_BACK */
    {232, 128},  /* SDL_CONTROLLER_BUTTON_GUIDE */
    {289, 132},  /* SDL_CONTROLLER_BUTTON_START */
    {75,  154},  /* SDL_CONTROLLER_BUTTON_LEFTSTICK */
    {305, 230},  /* SDL_CONTROLLER_BUTTON_RIGHTSTICK */
    {77,  40},   /* SDL_CONTROLLER_BUTTON_LEFTSHOULDER */
    {396, 36},   /* SDL_CONTROLLER_BUTTON_RIGHTSHOULDER */
    {154, 188},  /* SDL_CONTROLLER_BUTTON_DPAD_UP */
    {154, 249},  /* SDL_CONTROLLER_BUTTON_DPAD_DOWN */
    {116, 217},  /* SDL_CONTROLLER_BUTTON_DPAD_LEFT */
    {186, 217},  /* SDL_CONTROLLER_BUTTON_DPAD_RIGHT */
    {232, 174},  /* SDL_CONTROLLER_BUTTON_MISC1 */
    {132, 135},  /* SDL_CONTROLLER_BUTTON_PADDLE1 */
    {330, 135},  /* SDL_CONTROLLER_BUTTON_PADDLE2 */
    {132, 175},  /* SDL_CONTROLLER_BUTTON_PADDLE3 */
    {330, 175},  /* SDL_CONTROLLER_BUTTON_PADDLE4 */
};
/* This is indexed by SDL_GameControllerAxis. */
static const struct { int x; int y; double angle; } axis_positions[] = {
    {74,  153, 270.0},  /* LEFTX */
    {74,  153,   0.0},  /* LEFTY */
    {306, 231, 270.0},  /* RIGHTX */
    {306, 231,   0.0},  /* RIGHTY */
    {91,  -20,   0.0},  /* TRIGGERLEFT */
    {375, -20,   0.0},  /* TRIGGERRIGHT */
};


void c_key_handler(c_window_t *self,world_t *world){
	// we need to call SDL_PollEvent to let window rendered, otherwise
	// no window will be shown
    //SDL_Log("starting key handler");
	bool fwd=false;
	bool back=false;
	bool left=false;
	bool right=false;

	vec3 dir;

		
	float accel=world->player_accel;
	//model_t* model= VECTOR_GET_AS(model_t* ,&scene->objects,0);

//const Uint8 *pad_state = SDL_GetGameControllerState(0);


const Uint8 *state = SDL_GetKeyboardState(NULL);
if (state[SDL_SCANCODE_LEFT]) {
    left=true;
    accel+=.15;
}
if (state[SDL_SCANCODE_RIGHT]) {
    right=true;
    accel+=.15;
}
if (state[SDL_SCANCODE_UP]) {
    fwd=true;
    accel+=.15;
}
if (state[SDL_SCANCODE_DOWN]) {
    back=true;
    accel+=.15;
}
if(accel>1.15f){
	accel=1.17f;
}
accel*=0.85f;

float axis_value;

	while (SDL_PollEvent(&self->e) != 0)
	{

		ImGui_ImplSDL2_ProcessEvent(&self->e);
		switch(self->e.type){

			case SDL_CONTROLLERAXISMOTION:
         	   
         	   	axis_value = (float)(self->e.caxis.value)/32767.0;

         	   //SDL_Log("Controller %d axis %s changed to %f\n", self->e.caxis.which, SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)self->e.caxis.axis),axis_value);

         	   //if(strcmp(SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)self->e.caxis.axis),"leftx")==0){
         	   //	world->l_x_axis=(float)(self->e.caxis.value)/32767.0;
         	   	//printf("LEFT\n");
         	   	//left x
         	  // }
         	  // if(strcmp(SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)self->e.caxis.axis),"lefty")==0){
         	   	//left y
         	   	//printf("LEFT\n");
         	  // 	world->l_y_axis=(float)(self->e.caxis.value)/32767.0;
	         //  }
        	 //  if(strcmp(SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)self->e.caxis.axis),"righty")==0){
         	   	//left y
         	   		//printf("RIGHT\n");
         	 //  	world->r_y_axis=((float)(self->e.caxis.value)/32767.0);
         	   	//printf("ang %f\n",world->cy_angle);
	         //  }
           	 //  if(strcmp(SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)self->e.caxis.axis),"rightx")==0){
         	 //  	//rx
         	   		//printf("RIGHT\n");
         	  // 	world->r_x_axis=((float)(self->e.caxis.value)/32767.0);
         	   	//printf("ang %f\n",world->cx_angle);
	          // }
	           break;

	        case SDL_CONTROLLERBUTTONDOWN:
	        	break;
	        case SDL_CONTROLLERBUTTONUP:
	            //if (&self->e.type == SDL_CONTROLLERBUTTONDOWN) {
	                //SetController(self->e.cbutton.which);
	           	// }
	            SDL_Log("Controller %d button %s %s\n", self->e.cbutton.which, SDL_GameControllerGetStringForButton((SDL_GameControllerButton)self->e.cbutton.button), self->e.cbutton.state ? "pressed" : "released");
	            break;

            case SDL_KEYDOWN:
		        if(self->e.key.keysym.sym == SDLK_ESCAPE){
		          self->quit=true;
		        }
		        break;
	            /* Fall through to signal quit */
            case SDL_WINDOWEVENT_CLOSE:
            	self->quit = true;
            	break;
        	case SDL_QUIT:
        		self->quit = true;
             	break;

		}
	}

    SDL_GameController *controller;
    controller = SDL_GameControllerOpen(0);


		float deadzone = 0.26;  /* !!! FIXME: real deadzone */
		const Sint16 lx = SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)(0));
		float left_axis_float_x = (float)(lx)/32767.0;

		const Sint16 ly = SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)(1));
		float left_axis_float_y = (float)(ly)/32767.0;

		//world->l_x_axis=left_axis_float_x;
		//world->l_y_axis=left_axis_float_y;

		const Sint16 rx = SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)(2));
		float right_axis_float_x = (float)(rx)/32767.0;

		const Sint16 ry = SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)(3));
		float right_axis_float_y = (float)(ry)/32767.0;

		
		
		glm_vec3_zero(dir);
		glm_vec3_copy(world->player_dir,dir);



//		printf("%f value \n",value_f);

		if (left_axis_float_x < -deadzone) {
			world->l_x_axis=left_axis_float_x;
			glm_vec3_add(dir,(vec3){world->l_x_axis*.50,0.0,0.0},dir);
			accel+=.07;

		} else if (left_axis_float_x > deadzone) {
			world->l_x_axis=left_axis_float_x;
			glm_vec3_add(dir,(vec3){world->l_x_axis*.50,0.0,0.0},dir);
			accel+=.07;
		}
		if (left_axis_float_y < -deadzone) {
			world->l_y_axis=left_axis_float_y;
			accel+=.07;
			glm_vec3_add(dir,(vec3){0.0,0.0,world->l_y_axis*.50},dir);

		} else if (left_axis_float_y > deadzone) {
			world->l_y_axis=left_axis_float_y;
			glm_vec3_add(dir,(vec3){0.0,0.0,world->l_y_axis*.50},dir);
			accel+=.07;
		}


		if (right_axis_float_x < -deadzone) {
			world->r_x_axis=right_axis_float_x;

		} else if (right_axis_float_x > deadzone) {
			world->r_x_axis=right_axis_float_x;
		}
		if (right_axis_float_y < -deadzone) {
			world->r_y_axis=right_axis_float_y;

		} else if (right_axis_float_y > deadzone) {
			world->r_y_axis=right_axis_float_y;
		}


	SDL_GameControllerClose(0);



	if(fwd==true){
		glm_vec3_add(dir,(vec3){0.0,0.0,-.1},dir);
	}
	if(back==true){
		glm_vec3_add(dir,(vec3){0.0,0.0,.1},dir);
	}
	if(left==true){
		glm_vec3_add(dir,(vec3){-.1,0.0,0.0},dir);
	}
	if(right==true){
		glm_vec3_add(dir,(vec3){.1,0.0,0.0},dir);
	}
	glm_vec3_clamp(dir,-1.0,1.0);

	glm_vec3_normalize(dir);
	//glm_vec3_scale(dir,.85,dir);
	//glm_vec3_clamp(dir,-1.0,1.0);

	//glm_vec3_normalize(dir);

	glm_vec3_copy(dir,world->player_dir);
	world->player_accel=accel*.7;
}