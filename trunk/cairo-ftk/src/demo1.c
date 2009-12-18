#include <math.h>
#include "cairo-ftk-surface.h"

static cairo_t* cr = NULL;

static void paint_clip_image(int x, int y, int width, int height)
{
	int              w, h;
	cairo_surface_t *image;

	cairo_translate (cr, x, y);
	
	cairo_arc (cr, width/2, height/4, width/3, 0, 2*M_PI);
	cairo_clip (cr);
	cairo_new_path (cr); /* path not consumed by clip()*/

	image = cairo_image_surface_create_from_png ("pic.png");
	w = cairo_image_surface_get_width (image);
	h = cairo_image_surface_get_height (image);

//	cairo_scale (cr, 256.0/w, 256.0/h);

	cairo_set_source_surface (cr, image, 0, 0);
	cairo_paint (cr);

	cairo_surface_destroy (image);

	return;
}

static void paint_rect(int x, int y, int width, int height)
{
	/* a custom shape that could be wrapped in a function */
	double x0      = 25.6,   /* parameters like cairo_rectangle */
		   y0      = 25.6,
		   rect_width  = 204.8,
		   rect_height = 204.8,
		   radius = 102.4;   /* and an approximate curvature radius */

	double x1,y1;

	cairo_translate (cr, x, y);
	x1=x0+rect_width;
	y1=y0+rect_height;
	if (!rect_width || !rect_height)
		return;
	if (rect_width/2<radius) {
		if (rect_height/2<radius) {
			cairo_move_to  (cr, x0, (y0 + y1)/2);
			cairo_curve_to (cr, x0 ,y0, x0, y0, (x0 + x1)/2, y0);
			cairo_curve_to (cr, x1, y0, x1, y0, x1, (y0 + y1)/2);
			cairo_curve_to (cr, x1, y1, x1, y1, (x1 + x0)/2, y1);
			cairo_curve_to (cr, x0, y1, x0, y1, x0, (y0 + y1)/2);
		} else {
			cairo_move_to  (cr, x0, y0 + radius);
			cairo_curve_to (cr, x0 ,y0, x0, y0, (x0 + x1)/2, y0);
			cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
			cairo_line_to (cr, x1 , y1 - radius);
			cairo_curve_to (cr, x1, y1, x1, y1, (x1 + x0)/2, y1);
			cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);
		}
	} else {
		if (rect_height/2<radius) {
			cairo_move_to  (cr, x0, (y0 + y1)/2);
			cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
			cairo_line_to (cr, x1 - radius, y0);
			cairo_curve_to (cr, x1, y0, x1, y0, x1, (y0 + y1)/2);
			cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
			cairo_line_to (cr, x0 + radius, y1);
			cairo_curve_to (cr, x0, y1, x0, y1, x0, (y0 + y1)/2);
		} else {
			cairo_move_to  (cr, x0, y0 + radius);
			cairo_curve_to (cr, x0 , y0, x0 , y0, x0 + radius, y0);
			cairo_line_to (cr, x1 - radius, y0);
			cairo_curve_to (cr, x1, y0, x1, y0, x1, y0 + radius);
			cairo_line_to (cr, x1 , y1 - radius);
			cairo_curve_to (cr, x1, y1, x1, y1, x1 - radius, y1);
			cairo_line_to (cr, x0 + radius, y1);
			cairo_curve_to (cr, x0, y1, x0, y1, x0, y1- radius);
		}
	}
	cairo_close_path (cr);

	cairo_set_source_rgb (cr, 0.5, 0.5, 1);
	cairo_fill_preserve (cr);
	cairo_set_source_rgba (cr, 0.5, 0, 0, 0.5);
	cairo_set_line_width (cr, 10.0);
	cairo_stroke (cr);

	return;
}

static void paint_pic(int x, int y, int width, int height)
{
	int              w, h;
	cairo_surface_t *image;
	cairo_pattern_t *pattern;
	cairo_matrix_t   matrix;

	cairo_translate (cr, x, y);
	image = cairo_image_surface_create_from_png ("pic.png");
	w = cairo_image_surface_get_width (image);
	h = cairo_image_surface_get_height (image);

	pattern = cairo_pattern_create_for_surface (image);
	cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);

	cairo_translate (cr, 128.0, 128.0);
	cairo_rotate (cr, M_PI / 4);
	cairo_scale (cr, 1 / sqrt (2), 1 / sqrt (2));
	cairo_translate (cr, -128.0, -128.0);

	cairo_matrix_init_scale (&matrix, w/256.0 * 5.0, h/256.0 * 5.0);
	cairo_pattern_set_matrix (pattern, &matrix);

	cairo_set_source (cr, pattern);

	cairo_rectangle (cr, 0, 0, 256.0, 256.0);
	cairo_fill (cr);

	cairo_pattern_destroy (pattern);
	cairo_surface_destroy (image);

	return;
}

static void paint_arrow(int x, int y, int width, int height)
{
	cairo_translate (cr, x, y);
	cairo_set_line_width (cr, 40.96);
	cairo_move_to (cr, 76.8, 84.48);
	cairo_rel_line_to (cr, 51.2, -51.2);
	cairo_rel_line_to (cr, 51.2, 51.2);
	cairo_set_line_join (cr, CAIRO_LINE_JOIN_MITER); /* default */
	cairo_stroke (cr);

	cairo_move_to (cr, 76.8, 161.28);
	cairo_rel_line_to (cr, 51.2, -51.2);
	cairo_rel_line_to (cr, 51.2, 51.2);
	cairo_set_line_join (cr, CAIRO_LINE_JOIN_BEVEL);
	cairo_stroke (cr);

	cairo_move_to (cr, 76.8, 238.08);
	cairo_rel_line_to (cr, 51.2, -51.2);
	cairo_rel_line_to (cr, 51.2, 51.2);
	cairo_set_line_join (cr, CAIRO_LINE_JOIN_ROUND);
	cairo_stroke (cr);

	return;
}

static void paint_segments(int x, int y, int width, int height)
{
	cairo_translate (cr, x, y);
	cairo_move_to (cr, 50.0, 75.0);
	cairo_line_to (cr, 200.0, 75.0);

	cairo_move_to (cr, 50.0, 125.0);
	cairo_line_to (cr, 200.0, 125.0);

	cairo_move_to (cr, 50.0, 175.0);
	cairo_line_to (cr, 200.0, 175.0);

	cairo_set_line_width (cr, 30.0);
	cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
	cairo_stroke (cr);

	return;
}

static void paint_ball(int x, int y, int width, int height)
{
	cairo_pattern_t *pat;

	pat = cairo_pattern_create_linear (0.0, 0.0,  0.0, height);
	cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
	cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);
	cairo_rectangle (cr, x, y, width, height);
	cairo_set_source (cr, pat);
	cairo_fill (cr);
	cairo_pattern_destroy (pat);

	pat = cairo_pattern_create_radial (x+115.2, y+102.4, 25.6,
									   x+102.4,  y+102.4, 128.0);
	cairo_pattern_add_color_stop_rgba (pat, 0, 1, 1, 1, 1);
	cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
	cairo_set_source (cr, pat);
	cairo_arc (cr, x+width/2, y+height/4, 76.8, 0, 2 * M_PI);
	cairo_fill (cr);
	cairo_pattern_destroy (pat);

	return;
}
static void paint_helloworld(int x, int y, int width, int height)
{
	cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
								   CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 90.0);

	cairo_move_to (cr, x+10.0, y+135.0);
	cairo_show_text (cr, "Hello");

	cairo_move_to (cr, x+70.0, y+165.0);
	cairo_text_path (cr, "void");
	cairo_set_source_rgb (cr, 0.5, 0.5, 1);
	cairo_fill_preserve (cr);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_set_line_width (cr, 2.56);
	cairo_stroke (cr);

	/* draw helping lines */
	cairo_set_source_rgba (cr, 1, 0.2, 0.2, 0.6);
	cairo_arc (cr, x+10.0, y+135.0, 5.12, 0, 2*M_PI);
	cairo_close_path (cr);
	cairo_arc (cr, x+70.0, y+165.0, 5.12, 0, 2*M_PI);
	cairo_fill (cr);

	return;
}

static void paint_car(int x, int y, int width, int height)
{
	cairo_set_line_width (cr, 6);
	cairo_rectangle (cr, x+10, y+10, width-20, 70);
	cairo_new_sub_path (cr); cairo_arc (cr, x+64, y+64, 40, 0, 2*M_PI);
	cairo_new_sub_path (cr); cairo_arc_negative (cr, width-64, y+64, 40, 0, -2*M_PI);

	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_source_rgb (cr, 0, 0.7, 0); cairo_fill_preserve (cr);
	cairo_set_source_rgb (cr, 0, 0, 0); cairo_stroke (cr);

	cairo_translate (cr, x, height/2);
	cairo_rectangle (cr, 10, 10, width-20, 70);
	cairo_new_sub_path (cr); cairo_arc (cr, 64, 64, 40, 0, 2*M_PI);
	cairo_new_sub_path (cr); cairo_arc_negative (cr, width-64, 64, 40, 0, -2*M_PI);

	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
	cairo_set_source_rgb (cr, 0, 0, 0.9); cairo_fill_preserve (cr);
	cairo_set_source_rgb (cr, 0, 0, 0); cairo_stroke (cr);

	return;
}

static Ret on_paint(void* ctx, void* obj)
{
	FtkWidget* thiz = obj;
	FTK_BEGIN_PAINT(x, y, width, height, canvas);

	ftk_logd("%s:%d\n", __func__, __LINE__);
	if(cr == NULL)
	{
		cairo_surface_t* surface = cairo_ftk_surface_create(thiz);
		cr = cairo_create(surface);
		cairo_surface_destroy(surface);
	}
	
	cairo_save (cr);
	cairo_reset_clip (cr);
	cairo_identity_matrix (cr);
	cairo_new_path (cr);
	cairo_rectangle(cr, x, y, width, height);
	cairo_clip (cr);

	//paint_car(x, y, width, height);
	//paint_helloworld(x, y, width, height);
	//paint_ball(x, y, width, height);
	//paint_segments(x, y, width, height);
	//paint_arrow(x, y, width, height);
	//paint_pic(x, y, width, height);
	//paint_rect(x, y, width, height);
	paint_clip_image(x, y, width, height);

	cairo_restore (cr);
	
	FTK_END_PAINT();

	return RET_OK;
}

int main(int argc, char* argv[])
{
	int width = 0;
	int height = 0;
	FtkWidget* win = NULL;
	FtkWidget* button = NULL;
	FtkWidget* painter = NULL;
	ftk_init(argc, argv);

	win = ftk_app_window_create();
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);

	button = ftk_button_create(win, width/4, 10, width/2, 60);
	ftk_widget_set_text(button, "quit");
	painter = ftk_painter_create(win, 0, 70, width, height);
	ftk_painter_set_paint_listener(painter, on_paint, NULL);
	ftk_widget_show_all(win, 1);

	ftk_run();

	return 0;
}
