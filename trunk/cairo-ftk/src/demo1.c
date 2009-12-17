#include "cairo-ftk-surface.h"

static cairo_t* cr = NULL;
static Ret on_paint(void* ctx, void* obj)
{
	FtkWidget* painter = obj;

	if(cr == NULL)
	{
		cr = cairo_ftk_surface_create(painter);
	}
	cairo_set_line_width (cr, 6);

	cairo_rectangle (cr, 12, 12, 232, 70);
	cairo_new_sub_path (cr); cairo_arc (cr, 64, 64, 40, 0, 2*M_PI);
	cairo_new_sub_path (cr); cairo_arc_negative (cr, 192, 64, 40, 0, -2*M_PI);

	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_EVEN_ODD);
	cairo_set_source_rgb (cr, 0, 0.7, 0); cairo_fill_preserve (cr);
	cairo_set_source_rgb (cr, 0, 0, 0); cairo_stroke (cr);

	cairo_translate (cr, 0, 128);
	cairo_rectangle (cr, 12, 12, 232, 70);
	cairo_new_sub_path (cr); cairo_arc (cr, 64, 64, 40, 0, 2*M_PI);
	cairo_new_sub_path (cr); cairo_arc_negative (cr, 192, 64, 40, 0, -2*M_PI);

	cairo_set_fill_rule (cr, CAIRO_FILL_RULE_WINDING);
	cairo_set_source_rgb (cr, 0, 0, 0.9); cairo_fill_preserve (cr);
	cairo_set_source_rgb (cr, 0, 0, 0); cairo_stroke (cr);

	return RET_OK;
}

int main(int argc, char* argv[])
{
	int width = 0;
	int height = 0;
	FtkWidget* win = NULL;
	FtkWidget* painter = NULL;
	ftk_init(argc, argv);

	win = ftk_app_window_create();
	width = ftk_widget_width(win);
	height = ftk_widget_height(win);

	painter = ftk_painter_create(win, 0, 0, width, height);
	ftk_painter_set_paint_listener(painter, on_paint, NULL);
	ftk_widget_show_all(win, 1);

	ftk_run();

	return 0;
}
