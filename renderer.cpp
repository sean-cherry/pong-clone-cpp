internal void render_background() {
	u32* pixel = (u32*) renderstate.memory;
	for (int i = 0; i < renderstate.height; i++)
	{
		for (int j = 0; j < renderstate.width; j++)
		{
			*pixel++ = i * j;
		}
	}
}
internal void clear_screen(u32 color) {
	u32* pixel = (u32*)renderstate.memory;
	for (int i = 0; i < renderstate.height; i++)
	{
		for (int j = 0; j < renderstate.width; j++)
		{
			*pixel++ = color;
		}
	}
}

// start at 15:00


internal void draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color) {
	
	x0 = clamp(0, x0, renderstate.width);
	x1 = clamp(0, x1, renderstate.width);
	y0 = clamp(0, y0, renderstate.height);
	y1 = clamp(0, y1, renderstate.height);
	for (int i = y0; i < y1; i++)
	{
		u32* pixel = (u32*)renderstate.memory + x0 + i*renderstate.width;
		for (int j = x0; j < x1; j++)
		{
			*pixel++ = color;
		}
	}
}

global_variable float render_scale = 0.01f;

internal void
draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color) {
	
	x *= renderstate.height*render_scale;
	y *= renderstate.height*render_scale;
	half_size_x *= renderstate.height*render_scale;
	half_size_y *= renderstate.height*render_scale;


	x += renderstate.width / 2.f;
	y += renderstate.height / 2.f;
	// change to pixels 
	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;
	draw_rect_in_pixels(x0, y0, x1, y1, color);
}