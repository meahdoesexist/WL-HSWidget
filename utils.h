#pragma once

typedef struct wl_display wl_display;
typedef struct wl_registry wl_registry;
typedef struct wl_compositor wl_compositor;
typedef struct xdg_wm_base xdg_wm_base;
typedef struct wl_shm wl_shm;

struct app_state {
  wl_display *display;
  wl_registry *registry;
  
  wl_compositor *compositor;
  xdg_wm_base *wm_base;
  wl_shm *shm;
};
