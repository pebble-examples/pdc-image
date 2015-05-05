/**
 * main.c - Sets up a Window and Layer that draws the GDrawCommandImage
 * in its LayerUpdateProc.
 */

#include <pebble.h>

static Window *s_main_window;
static Layer *s_canvas_layer;

static GDrawCommandImage *s_command_image;

static void update_proc(Layer *layer, GContext *ctx) {
  // Place image in the center of the Window
  GSize img_size = gdraw_command_image_get_bounds_size(s_command_image);
  GPoint origin = GPoint(72 - (img_size.w / 2), 84 - (img_size.h / 2));

  // If the image was loaded successfully...
  if (s_command_image) {
    // Draw it
    gdraw_command_image_draw(ctx, s_command_image, origin);
  }
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Load the image and check it was succcessful
  s_command_image = gdraw_command_image_create_with_resource(RESOURCE_ID_DRAW_COMMAND);
  if (!s_command_image) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Image is NULL!");
  }

  // Create canvas Layer and set up the update procedure
  s_canvas_layer = layer_create(bounds);
  layer_set_update_proc(s_canvas_layer, update_proc);
  layer_add_child(window_layer, s_canvas_layer);
}

static void window_unload(Window *window) {
  // Destroy canvas Layer
  layer_destroy(s_canvas_layer);

  // Destroy the image
  gdraw_command_image_destroy(s_command_image);
}

static void init() {
  // Set up main Window
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorJazzberryJam);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  // Destroy main Window
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
