#include "world.h"

world_t *world_new()
{
  world_t *self = calloc(1, sizeof(*self));
  glm_vec3_copy((vec3){0.0f,0.0f,0.0f},self->player_pos);
  glm_vec3_copy((vec3){0.0f,0.0f,1.0f},self->player_dir);
  glm_vec3_copy((vec3){0.0f,0.0f,12.0f},self->cam_pos);
  return self;
}


float map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


float InOutQuadBlend(float t)
{
    if(t <= 0.5f)
        return 2.0f * t * t;
    t -= 0.5f;
    return 2.0f * t * (1.0f - t) + 0.5f;
}

float BezierBlend(float t)
{
    return t * t * (3.0f - 2.0f * t);
}

float ParametricBlend(float t)
{
    float sqt = t * t;
    return sqt / (2.0f * (sqt - t) + 1.0f);
}

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

void update_world(world_t *world,gl_camera *camera){

	world->player_pos;

	vec3 c_view_dir;
	vec3 c_right_dir;

	get_view_dir(camera,c_view_dir);
	c_view_dir[1]=0;
	vec3 v;
	
	right_dir(camera,c_right_dir);
	c_right_dir[1]=0;

	glm_vec3_add(c_view_dir,world->player_heading,world->player_heading);
	glm_vec3_add(c_right_dir,world->player_heading,world->player_heading);
	//glm_vec3_normalize(world->player_heading);

	glm_vec3_copy(world->player_dir,v);
	//forward scaled by v
	glm_vec3_scale(c_view_dir,-v[2]*world->player_accel,c_view_dir);
	glm_vec3_scale(c_right_dir,v[0]*world->player_accel,c_right_dir);



	glm_vec3_add(c_view_dir,world->player_pos,world->player_pos);
	glm_vec3_add(c_right_dir,world->player_pos,world->player_pos);

	//glm_normalize(v);
	glm_vec3_add(c_right_dir,c_view_dir,world->player_view_dir);
	glm_normalize(world->player_view_dir);
	//glm_vec3_add(v,world->player_pos,world->player_pos);
	//glm_vec3_copy(v,world->player_view_dir);

	float dist = sqrt( (camera->eye[0]-world->player_pos[0]) * (camera->eye[0] - world->player_pos[0]) + ((camera->eye[2]+5) - world->player_pos[2]) * ((camera->eye[2]+5)- world->player_pos[2]) );
	float fitX = map(dist,10,40, 0, 1);
	float para_X = ParametricBlend(fitX*1.0);

	camera->pivot[0]=lerp(camera->pivot[0],world->player_pos[0],para_X);
	camera->pivot[1]=lerp(camera->pivot[1],world->player_pos[1],para_X);
	camera->pivot[2]=lerp(camera->pivot[2],world->player_pos[2],para_X);

	camera->look_at[0]=lerp(camera->look_at[0],world->player_pos[0],para_X);
	camera->look_at[1]=lerp(camera->look_at[1],world->player_pos[1],para_X);
	camera->look_at[2]=lerp(camera->look_at[2],world->player_pos[2],para_X);



	camera->dir[0]=lerp(camera->dir[0],world->player_heading[0],para_X);
	camera->dir[1]=lerp(camera->dir[1],world->player_heading[1],para_X);
	camera->dir[2]=lerp(camera->dir[2],world->player_heading[2],para_X);

  	//lm_vec3_copy(world->player_view_dir,camera->dir);
  	//glm_vec3_copy(world->player_pos,camera->pivot);
  	//glm_vec3_copy(world->player_pos,camera->look_at);
	//glm_vec3_copy(world->player_pos,camera->pos);
	//glm_vec3_copy(world->player_view_dir,camera->dir);

	glm_vec3_copy(world->cam_pos,camera->pos);
	//vec3 tmp;
 	//glm_vec3_copy(world->player_view_dir,tmp);
	//glm_vec3_normalize(tmp);
	//float mag = sqrt((world->player_view_dir[0]*world->player_view_dir[0]) + (world->player_view_dir[2]*world->player_view_dir[2]))/16;

  	camera->x_angle += world->r_x_axis*.05;
  	camera->y_angle += world->r_y_axis*.05*.5;

  	world->r_x_axis*=.8;
  	world->r_y_axis*=.8;

 	//camera->x_angle = camera->x_angle*(1-world->player_accel/4);
	//camera->y_angle = camera->y_angle*(1-world->player_accel/4);


	world->cx_angle=camera->x_angle;
	world->cy_angle=camera->y_angle;
}