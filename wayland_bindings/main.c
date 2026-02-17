#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"

#include <wayland-client.h>
#include "xdg-shell-client-protocol.h"

//create header soon
typedef struct wl_display wl_display;
typedef struct wl_registry wl_registry;
typedef struct wl_compositor wl_compositor;
typedef struct xdg_wm_base xdg_wm_base;
typedef struct wl_shm wl_shm;
typedef struct wl_surface wl_surface;
typedef struct xdg_surface xdg_surface;

struct state {
  //Globals
  wl_display *display;
  wl_registry *registry;
  
  wl_compositor *compositor;
  xdg_wm_base *wm_base;
  wl_shm *shm;

  //Objects
  wl_surface *surface;
  xdg_surface *xdg_surface;
};

// -------

/*
   --- WRAPPERS ---
*/
//Connect to display wrapper
wl_display
*display_connect(const char *name) {
  struct wl_display *display;
  display = wl_display_connect(name);
  if(!display) {
    fprintf(stderr, "Failed to connect to display\n");
    return NULL;
  }
  
  printf("Connected\n");
  return display;
}

//Retrieve registry wrapper
wl_registry
*registry_handler(wl_registry *display) {
  wl_registry *registry;
  registry = wl_display_get_registry(display);
  if(!registry) {
    fprintf(stderr, "Failed to get registry");
    return NULL;
  }
  
  printf("Registry retrieved successfully\n");
  return registry;
}

//binding wrapper
static void 
registry_global(void *data,
             wl_registry *registry,
             uint32_t name,
             const char *interface,
             uint32_t version) {
  
  struct state *app = data; 
  
  if(strcmp(interface, wl_compositor_interface.name) == 0) {
    app->compositor = wl_registry_bind(registry, name, &wl_compositor_interface, version);
  }
  else if(strcmp(interface, xdg_wm_base_interface.name) == 0 ) {
    app->wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, version);
  }
  else if(strcmp(interface, wl_shm_interface.name) == 0) {
    app->shm = wl_registry_bind(registry, name, &wl_shm_interface, version);
  }
  else {
    fprintf(stderr, "not handled yet !\n");
  }

  if(!app->compositor && !app->wm_base && !app->shm ) {
    fprintf(stderr, "failed to bind objects\n");
  }
  else {
    printf("Objects, binded successfully\n");
  }
}

static const struct wl_registry_listener wlr_listener = {
    .global = registry_global,
    .global_remove = NULL,
};

//main
int
main (int argc, char *argv[]){
  
  struct state *app = malloc(sizeof(struct state));

  //Display and registry
  app->display = display_connect(NULL);
  if(!app->display) {
    free(app);
    return EXIT_FAILURE; 
  }
  
  app->registry = registry_handler(app->display);
  if(!app->registry) {
    wl_display_disconnect(app->display);
    free(app);
    return EXIT_FAILURE;
  }
  // -----
  
  wl_registry_add_listener(app->registry, &wlr_listener, app);
  wl_display_roundtrip(app->display);

  //Surface
  app->surface = wl_compositor_create_surface(app->compositor);
  app->xdg_surface = xdg_wm_base_get_xdg_surface(app->wm_base, app->surface);
  
  wl_display_disconnect(app->display);
  free(app);
  return EXIT_SUCCESS;
}

// Road Map:
// surface listener ? 
// attach first buffer 
// to do: log file 
