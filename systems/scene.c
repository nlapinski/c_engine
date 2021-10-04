#include "scene.h"

scene_t *scene_new()
{
	scene_t *self = calloc(1, sizeof *self);
	printf("scene init \n");
	vector_setup(&self->objects,0, sizeof(model_t*));

	return self;
}


void scene_add_obj(scene_t *self,model_t *m)
{
	vector_push_back(&self->objects,&m);
	model_t* a= VECTOR_GET_AS(model_t* ,&self->objects,self->objects.size-1);
	printf("scene registerd -> object name -> %s \n", a->name);
	//free(a);
}