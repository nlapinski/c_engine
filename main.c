#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "systems/key_event.h"
#include "systems/render_device.h"
#include "systems/scene.h"
#include "utils/mesh_loader.h"
#include "utils/renderer.h"

#include "components/world.h"


#include <cglm/cglm.h>   /* for inline */
#include <cglm/struct.h>


static void main_loop_init(void)
{
  c_window_t *window;
  window = c_window_new(100,100);

  world_t *world;
  world = world_new();

  scene_t *scene;
  scene = scene_new();

  c_render_device_t *render_device;
  render_device = c_render_device_new();
  
  loader_t *test_scene;
  test_scene=loader_new("assets/cap.obj","player");
  scene_add_obj(scene, test_scene->load);

  test_scene=loader_new("assets/grid.obj","grid");
  scene_add_obj(scene, test_scene->load);

  test_scene=loader_new("assets/bench.obj","pig");
  scene_add_obj(scene, test_scene->load);
  free(test_scene);

  renderer_t *renderer;
  renderer = renderer_new(1.0f);
  
  while(!window->quit){
      c_key_handler(window,world);
      update_world(world,&renderer->glvars[0]);
      c_render(renderer,scene,world);
      c_window_draw(window,world);
      fflush(stdout);
      fflush(stderr);

  }
  c_window_exit(window);

}



int main(int argc, char* argv[])
{

  main_loop_init();

}