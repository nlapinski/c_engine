#include "../components/camera.h"



void update_view(gl_camera *self)
{

    glm_mat4_identity(self->previous_view);



    glm_lookat(self->eye, self->pivot, self->up, self->previous_view);

}

//cam forward z
void get_view_dir(gl_camera *self,vec3 view){
    mat4 to;

    glm_mat4_transpose_to(self->previous_view,to);
    glm_vec3_negate_to((vec3){to[2][0],to[2][1],to[2][2]}, view);
}

void right_dir(gl_camera *self,vec3 right){
    mat4 to;
    glm_mat4_transpose_to(self->previous_view,to);
    glm_vec3_copy((vec3){to[0][0],to[0][1],to[0][2]},right);

}


void arc_ball(gl_camera *self){


    float x_angle = self->x_angle;
    float y_angle = self->y_angle;

    glm_vec3_copy((vec3){0,1,0},self->up);

    //view dir + up (view dir typically down Z)
    //vec3 cos_angle = glm_vec3_dot(vec3{0,0,1},self->up);

   // if (cos_angle * sgn(delta_angle_y) > 0.99f){
   //     y_angle = 0;
  //  }
    //glm_vec3_copy((vec3){0.0f,-7.0f,-2.0f},self->pos);

  //  vec4 local_look;

  //  vec3 dir;



    //glm_vec3_copy(self->dir,dir);
   // glm_vec3_normalize(dir);
   // dir[0]=-dir[0];

    //glm_vec3_sub(self->pos,)

//    glm_quat_for(dir, self->up,local_look);

    // step 2: Rotate the camera around the pivot point on the first axis.
    vec3 temp_p;
    glm_vec3_copy(self->pos,temp_p);
  // glm_quat_rotatev(local_look, temp_p, temp_p);
    glm_vec3_rotate(temp_p,x_angle,self->up);
    vec3 right;
    right_dir(self,right);
    glm_vec3_rotate(temp_p,y_angle-.2,right);
    glm_vec3_add(temp_p,self->pivot,temp_p);
    glm_vec3_copy(temp_p,self->eye);
    update_view(self);

}


