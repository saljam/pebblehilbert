#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "gen/resource_ids.auto.h"

#define MY_UUID { 0xAF, 0x99, 0x80, 0xFF, 0x41, 0xFA, 0x44, 0x43, 0x85, 0x9F, 0x99, 0xEE, 0xD9, 0x9E, 0x51, 0xA2 }
PBL_APP_INFO(MY_UUID,
	"Hello Hilbert", "0x65.net",
	1, 0,
	DEFAULT_MENU_ICON,
	APP_INFO_STANDARD_APP);

Window window;
Layer curve_layer;

#define WIDTH 144
#define HEIGHT 168

GPoint current;
void drawHilbert(GContext* ctx, int16_t x, int16_t y, int16_t xi, int16_t xj, int16_t yi, int16_t yj, int n)
{
	/* x and y are the coordinates of the bottom left corner */
	/* xi & xj are the i & j components of the unit x vector of the frame */
	/* similarly yi and yj */
	if (n == 0) {
		GPoint dest = {x + (xi + yi) / 2, y + (xj + yj) / 2};
		if (current.x != 0 && current.y != 0) graphics_draw_line(ctx, current, dest);
		current = dest;
	} else {
		drawHilbert(ctx, x, y, yi / 2, yj / 2, xi / 2, xj / 2, n - 1);
		drawHilbert(ctx, x + xi / 2, y + xj / 2, xi / 2, xj / 2, yi / 2, yj / 2, n - 1);
		drawHilbert(ctx, x + xi / 2 + yi / 2, y + xj / 2 + yj / 2, xi / 2, xj / 2, yi / 2, yj / 2, n - 1);
		drawHilbert(ctx, x + xi / 2 + yi, y + xj / 2 + yj, -yi / 2, -yj / 2, -xi / 2, -xj / 2, n - 1);
	}
}

void curve_update(Layer *me, GContext *ctx) {
	graphics_context_set_stroke_color(ctx, GColorWhite);
	
	GRect frame = layer_get_bounds(me);
	
	int n = 4;
	current.x = 0;
	current.y = 0;
	drawHilbert(ctx, 10, 10,
		WIDTH-20, 10,
		10, HEIGHT - 20, n);
}

void handle_init(AppContextRef ctx)
{
	window_init(&window, "Hilbert");
	window_stack_push(&window, true);
	window_set_background_color(&window, GColorBlack);
	
	GRect frame = window.layer.frame;

	layer_init(&curve_layer, GRect(0, 0, frame.size.w, frame.size.h));
	curve_layer.update_proc = curve_update;
	layer_add_child(&window.layer, &curve_layer);
	
}

void pbl_main(void *params)
{
	PebbleAppHandlers handlers = {
		.init_handler = &handle_init
	};
	app_event_loop(params, &handlers);
}
