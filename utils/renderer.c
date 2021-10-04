#include "renderer.h"
#include "../vector/vector.h"
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flag

#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "../components/model.h"


GLuint matricesUniBuffer;
#define MatricesUniBufferSize sizeof(float) * 16 * 3
#define ProjMatrixOffset 0
#define ViewMatrixOffset sizeof(float) * 16
#define ModelMatrixOffset sizeof(float) * 16 * 2
#define MatrixSize sizeof(float) * 16



// Vertex Attribute Locations
GLuint vertexLoc_=0, normalLoc_=1, texCoordLoc_=2;

// Uniform Bindings Points
GLuint matricesUniLoc_ = 1, materialUniLoc_ = 2;

// The sampler uniform for textured models
// we are assuming a single texture so this will
//always be texture unit 0
GLuint texUnit_ = 0;



// Shader Names
char *vertexFileName = "vert.glsl";
char *fragmentFileName = "frag.glsl";



GLchar *load_shader_source(char *filename) {
  FILE *file = fopen(filename, "r");             // open 
  fseek(file, 0L, SEEK_END);                     // find the end
  size_t size = ftell(file);                     // get the size in bytes
  GLchar *shaderSource = calloc(1, size);        // allocate enough bytes
  rewind(file);                                  // go back to file beginning
  fread(shaderSource, size, sizeof(char), file); // read each char into ourblock
  fclose(file);                                  // close the stream
  return shaderSource;
}

GLuint setup_shaders(renderer_t *self) {

  char *vs = NULL,*fs = NULL,*fs2 = NULL;

  GLuint p,v,f;

  v = glCreateShader(GL_VERTEX_SHADER);
  f = glCreateShader(GL_FRAGMENT_SHADER);

  vs = load_shader_source(vertexFileName);
  fs = load_shader_source(fragmentFileName);

  const char * vv = vs;
  const char * ff = fs;

  glShaderSource(v, 1, &vv,NULL);
  glShaderSource(f, 1, &ff,NULL);

  free(vs);free(fs);

  glCompileShader(v);
  glCompileShader(f);

  //printShaderInfoLog(v);
  //printShaderInfoLog(f);

  p = glCreateProgram();
  glAttachShader(p,v);
  glAttachShader(p,f);

  glBindFragDataLocation(p, 0, "output");

  glBindAttribLocation(p,vertexLoc_,"position");
  glBindAttribLocation(p,normalLoc_,"normal");
  glBindAttribLocation(p,texCoordLoc_,"texCoord");

  glLinkProgram(p);
  glValidateProgram(p);
  //printProgramInfoLog(p);

  self->program = p;
  self->vertex_shader = v;
  self->fragment_shader = f;
  
  GLuint k = glGetUniformBlockIndex(p,"Matrices");
  glUniformBlockBinding(p, k, matricesUniLoc_);
  glUniformBlockBinding(p, glGetUniformBlockIndex(p,"Material"), materialUniLoc_);

  texUnit_ = glGetUniformLocation(p,"texUnit");

  return(p);
}

void c_resize(renderer_t *self, int w, int h) {

  printf("resize gl view\n");

  float ratio;
  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  if(h == 0)
    h = 1;

  // Set the viewport to be the entire window
  glViewport(0, 0, w, h);

  ratio = (1.0f * w) / h;
  self->aspect = ratio;
  glm_perspective_resize(ratio,self->glvars[0].projection);
}

void print_m4( mat4 m4 )
{

    for (int col = 0; col < 4; ++col) {
        printf("| ");
        for (int row = 0; row < 4; ++row) {
           printf("%f |",m4[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}


void c_set_projection(renderer_t *self) {


  glm_perspective(self->proj_fov,self->aspect,self->proj_near,self->proj_far,self->glvars[0].projection);
    //print_m4(self->glvars[0].projection);
  glBindBuffer(GL_UNIFORM_BUFFER,matricesUniBuffer);
  glBufferSubData(GL_UNIFORM_BUFFER, ProjMatrixOffset, MatrixSize, (float *)self->glvars[0].projection);
  glBindBuffer(GL_UNIFORM_BUFFER,0);

}


void c_set_camera(renderer_t *self) {
/*
vec4 quat;
glm_quat_identity(quat);

float rotation_angle = glm_rad(45.0f);

vec3 rotation_axis;

glm_vec3_copy((vec3){0,1,0},rotation_axis);

float x = rotation_axis[0] * sin(rotation_angle / 2);
float y = rotation_axis[1] * sin(rotation_angle / 2);
float z = rotation_axis[2]* sin(rotation_angle / 2);
float w = cos(rotation_angle / 2);

*/
arc_ball(&self->glvars[0]);

//quat look
//glm_quat_forp(vec3 from, vec3 to, vec3 up, versor dest)

//(vec3){0.0f,5.0f,-25.0f}

  //vec3 nd;
  //glm_vec3_normalize_to(dir,nd);

  //vec3 cam_pos;
  //glm_vec3_copy(self->glvars[0].pos,cam_pos);
  //glm_vec3_add(cam_pos,pos,cam_pos);


  glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer);
  glBufferSubData(GL_UNIFORM_BUFFER, ViewMatrixOffset, MatrixSize,  (float *)self->glvars[0].previous_view);
  //print_m4(self->glvars[0].previous_view);
  glBindBuffer(GL_UNIFORM_BUFFER,0);

}


void c_set_player_model_matrix(renderer_t *self, vec3 pos, vec3 dir){


  vec3 nd;

  //glm_vec3_rotate_m4(self->glvars[0].previous_view,dir,nd);

  nd[1]=0;

  dir[0]=-dir[0];


  glm_mat4_identity(self->glvars[0].model);

  mat4 look=GLM_MAT4_IDENTITY_INIT;
  glm_translate(self->glvars[0].model, pos);
  glm_look((vec3){0,0,0},dir, (vec3){0,1,0},look);
  glm_mul_rot(self->glvars[0].model,look,self->glvars[0].model);

  
  glBindBuffer(GL_UNIFORM_BUFFER,matricesUniBuffer);
  glBufferSubData(GL_UNIFORM_BUFFER, ModelMatrixOffset, MatrixSize,  (float *)self->glvars[0].model);
  //print_m4(self->glvars[0].model);
  glBindBuffer(GL_UNIFORM_BUFFER,0);

}

void c_set_model_matrix(renderer_t *self, vec3 pos){

  glm_mat4_identity(self->glvars[0].model);
  
  glm_rotate(self->glvars[0].model,0.0f,(vec3){0,1,0});
  glm_translate(self->glvars[0].model, pos);
  
  glBindBuffer(GL_UNIFORM_BUFFER,matricesUniBuffer);
  glBufferSubData(GL_UNIFORM_BUFFER, ModelMatrixOffset, MatrixSize,  (float *)self->glvars[0].model);
  //print_m4(self->glvars[0].model);
  glBindBuffer(GL_UNIFORM_BUFFER,0);

}

void c_recursive_render(Vector shapes){


    for(int j =0;j<shapes.size;j++){
      mesh_t current = VECTOR_GET_AS(mesh_t,&shapes,j);

      //printf("rendering -> mesh -> %s \n",current.name);
      //printf("num_faces -> %d \n",current.num_faces);
      glBindBufferRange(GL_UNIFORM_BUFFER, materialUniLoc_, current.uniformBlockIndex, 0, sizeof(struct material_t));
      //printf("current block Index -> %d \n",current.uniformBlockIndex);
      glBindTexture(GL_TEXTURE_2D, current.texIndex);
      //printf("current vao -> %d \n",current.vao);
      glBindVertexArray(current.vao);     
      glDrawElements(GL_TRIANGLES,current.num_faces*3,GL_UNSIGNED_INT,0);
      //printf("current faces -> %d \n",current.num_faces);

    }


}


void c_render(renderer_t *self,scene_t *scene,world_t *world)
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

// buildProjectionMatrix(45.0f, 1.0, .001, 1000); 
  c_set_projection(self);
  c_set_camera(self);

  glUseProgram(self->program);
  glUniform1i(texUnit_,0);

  if(self->width != world->width || self->height != world->height){
    self->width = world->width;
    self->height = world->height;
    c_resize(self, self->width,self->height);
  }
 


  for(int i=0;i<scene->objects.size;i++){

    model_t* model= VECTOR_GET_AS(model_t* ,&scene->objects,i);

    //printf("batched -> model -> %s \n", model->name);

    if(i==0){
      c_set_player_model_matrix(self,world->player_pos,world->player_view_dir);
    }
    else{
      c_set_model_matrix(self,model->pos);
    }

    c_recursive_render(model->shapes);
  }



}


renderer_t *renderer_new(float resolution)
{
  uint32_t f;
  renderer_t *self = calloc(1, sizeof(*self));

  for(f = 0; f < 6; f++)
  {
      glm_mat4_identity(self->relative_transform[f]);
      glm_mat4_identity(self->glvars[f].projection);
      glm_mat4_identity(self->glvars[f].inv_projection);
      glm_mat4_identity(self->glvars[f].previous_view);
      glm_mat4_identity(self->glvars[f].model);
  }

  self->proj_near = 0.01f;
  self->proj_far = 2000.0f;


  //aprox 80 def fov
  self->proj_fov = M_PI / 4.0f;

  self->resolution = resolution;
  glewInit();
  self->program = setup_shaders(self);

  // Uniform Block ??? camera ???
  glGenBuffers(1,&matricesUniBuffer);
  glBindBuffer(GL_UNIFORM_BUFFER, matricesUniBuffer);
  glBufferData(GL_UNIFORM_BUFFER, MatricesUniBufferSize,NULL,GL_DYNAMIC_DRAW);
  glBindBufferRange(GL_UNIFORM_BUFFER, matricesUniLoc_, matricesUniBuffer, 0, MatricesUniBufferSize);  //setUniforms();
  glBindBuffer(GL_UNIFORM_BUFFER,0);
  glEnable(GL_MULTISAMPLE);

  return self;
}
