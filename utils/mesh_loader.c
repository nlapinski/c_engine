#include "mesh_loader.h"


// Vertex Attribute Locations
GLuint vertexLoc=0, normalLoc=1, texCoordLoc=2;

// Uniform Bindings Points
GLuint matricesUniLoc = 1, materialUniLoc = 2;

// The sampler uniform for textured models
// we are assuming a single texture so this will
//always be texture unit 0
GLuint texUnit = 0;

/* ---------------------------------------------------------------------------- */
void color4_to_float4(const C_STRUCT aiColor4D *c, float f[4])
{
  f[0] = c->r;
  f[1] = c->g;
  f[2] = c->b;
  f[3] = c->a;
}

/* ---------------------------------------------------------------------------- */
void set_float4(float f[4], float a, float b, float c, float d)
{
  f[0] = a;
  f[1] = b;
  f[2] = c;
  f[3] = d;
}


model_t* load_scene (const char* path, const char* obj_name)
{

	//load mesh/scene
	const C_STRUCT aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_Quality);
	if( !scene)
	{
		printf("asset load error \n");
	}
  // buffer for faces
  GLuint buffer;
  model_t *model_load;

  model_load = model_new();


  for (unsigned int n = 0; n < scene->mNumMeshes; ++n)
  {

    const C_STRUCT aiMesh* mesh = scene->mMeshes[n];
    //printf("loaded -> mesh group -> %s \n",mesh->mName.data);
    mesh_t mesh_load;
    //mesh_load = mesh_new();



    mesh_load.faceArray=  (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
    unsigned int *faceArray;
    faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
    unsigned int faceIndex = 0;

    for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
      const C_STRUCT aiFace* face = &mesh->mFaces[t];

      memcpy(&faceArray[faceIndex], face->mIndices,3 * sizeof(unsigned int));
      memcpy(&mesh_load.faceArray[faceIndex], face->mIndices,3 * sizeof(unsigned int));
      faceIndex += 3;
    }

    ///////////////////////////////////////////
    mesh_load.num_faces = (unsigned int)mesh->mNumFaces;

    //load the mesh to GPU

    // generate Vertex Array for mesh
    glGenVertexArrays(1,&(mesh_load.vao));
    glBindVertexArray(mesh_load.vao);


    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

    // buffer for vertex positions
    if (mesh->mVertices!=NULL) {
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);
    }


    // buffer for vertex normals
    if (mesh->mNormals != NULL){
      glGenBuffers(1, &buffer);
      glBindBuffer(GL_ARRAY_BUFFER, buffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
      glEnableVertexAttribArray(normalLoc);
      glVertexAttribPointer(normalLoc, 3, GL_FLOAT, 0, 0, 0);
    }

    // buffer for vertex texture coordinates
    if (mesh->mTextureCoords[0]!=NULL) {
      float *texCoords = (float *)malloc(sizeof(float)*2*mesh->mNumVertices);
      for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

        texCoords[k*2]   = mesh->mTextureCoords[0][k].x;
        texCoords[k*2+1] = mesh->mTextureCoords[0][k].y; 
        
      }
      glGenBuffers(1, &buffer);
      glBindBuffer(GL_ARRAY_BUFFER, buffer);
      glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
      glEnableVertexAttribArray(texCoordLoc);
      glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, 0, 0, 0);
    }

    // unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
  
    // create material uniform buffer
    C_STRUCT aiMaterial *mtl = scene->mMaterials[mesh->mMaterialIndex];
      
    C_STRUCT aiString texPath; //contains filename of texture
      
    //if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)){
    if(aiReturn_SUCCESS  == aiGetMaterialTexture(mtl,aiTextureType_DIFFUSE,0,&texPath,NULL,NULL,NULL,NULL,NULL,NULL)){
        //bind texture
        //unsigned int texId = textureIdMap[texPath.data];
        //aMesh.texIndex = texId;
        //aMat.texCount = 1;
      }
    else{

      mesh_load.mat.texCount = 0;
    }

    float c[4];
    set_float4(c, 0.5f, 0.5f, 0.5f, 1.0f);
    mesh_load.mat.texCount = 0;
    C_STRUCT aiColor4D diffuse;
    
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
      color4_to_float4(&diffuse, c);
    memcpy(mesh_load.mat.diffuse, c, sizeof(c));

    set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
    C_STRUCT aiColor4D ambient;
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
      color4_to_float4(&ambient, c);
    memcpy(mesh_load.mat.ambient, c, sizeof(c));

    set_float4(c, 0.5f, 0.5f, 0.5f, 1.0f);
    C_STRUCT aiColor4D specular;
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
      color4_to_float4(&specular, c);
    memcpy(mesh_load.mat.specular, c, sizeof(c));

    set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
    C_STRUCT aiColor4D emission;
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
      color4_to_float4(&emission, c);
    memcpy(mesh_load.mat.emissive, c, sizeof(c));


    float shininess = 1.0;
    unsigned int max;
    aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
    mesh_load.mat.shininess = shininess;

    glGenBuffers(1,&(mesh_load.uniformBlockIndex));
    glBindBuffer(GL_UNIFORM_BUFFER,mesh_load.uniformBlockIndex);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(mesh_load.mat), (void *)(&mesh_load.mat), GL_STATIC_DRAW);

    memcpy(mesh_load.name,mesh->mName.data,32);


    vector_push_back(&model_load->shapes,&mesh_load);




  }


  memcpy(model_load->name,obj_name,32);
  aiReleaseImport(scene);
  return model_load;


}

loader_t *loader_new(const char* file, const char* obj_name)
{
	loader_t *self = calloc(1, sizeof *self);

	self->load = load_scene(file,obj_name);
	return self;
}




