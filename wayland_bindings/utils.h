#pragma once

typedef struct wl_display wl_display;
typedef struct wl_registry wl_registry;
typedef struct wl_compositor wl_compositor;
typedef struct xdg_wm_base xdg_wm_base;
typedef struct wl_shm wl_shm;
typedef struct wl_surface wl_surface;
typedef struct xdg_surface xdg_surface;

struct app_state {
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
