#include "model.h"


model_t *model_new()
{
	model_t *self=malloc(sizeof(*self));
	//self->name = malloc(64);
	glm_vec3_zero(self->pos);

	vector_setup(&self->shapes,0, sizeof(*self));
	return self;
}



mesh_t *mesh_new()
{
	mesh_t* self=malloc(sizeof(*self));
	//self->name = malloc(64);
	//mesh_t self;// = malloc(sizeof(mesh_t));
	return self;
}
