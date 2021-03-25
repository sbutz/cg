unitsize(1cm);

pen B[] = { rgb("2e75b6"), rgb("5b9bd5"), rgb("9dc3e6") };
pen O[] = { rgb("000000"), rgb("ed7d31"), rgb("f9cbad") };
pen G[] = { rgb("000000"), rgb("70ad47"), rgb("a9d18e") };

void draw_pixelgrid(int w, int h, pen lines = lightgray, pen dots = invisible) {
	for (int x = 0; x <= w; ++x)
		draw((x,0)--(x,h), lightgray);
	for (int y = 0; y <= h; ++y)
		draw((0,y)--(w,y), lightgray);
	for (int x = 0; x < w; ++x)
		for (int y = 0; y < h; ++y)
			dot((x+.5,y+.5), linewidth(4)+dots);
}

struct pixel_pos {
	int x, y;
	void operator init(int x, int y) {
		this.x = x;
		this.y = y;
	}
};

void draw_pixel(real x, real y, pen border = black+linewidth(2), pen fill = lightgray) {
	void draw_pixel_discrete(int x, int y) {
		filldraw((x,y)--(x+1,y)--(x+1,y+1)--(x,y+1)--cycle, fill, border);
	}
	draw_pixel_discrete((int)x, (int)y);
}

pair line_normal(pair a, pair b) {
	pair v = b-a;
	v = v/sqrt(dot(v,v));
	return (-v.y,v.x);
}

struct tri {
	pair a, b, c;
	void operator init(pair a, pair b, pair c) {
		this.a = a;
		this.b = b;
		this.c = c;
	}
	void draw(pen outline = black+linewidth(2), pen inner = invisible, bool labels=true) {
		filldraw(a--b--c--cycle, inner, outline);
		if (labels) {
			label("\huge$a$", a - .5*unit(line_normal(a,b)+line_normal(c,a)));
			label("\huge$b$", b - .5*unit(line_normal(a,b)+line_normal(b,c)));
			label("\huge$c$", c - .5*unit(line_normal(b,c)+line_normal(c,a)));
		}
	}
};

int res_x = 16, res_y = 9;

/*
 * Aufgabe 5 & 6.
 * Erweitern Sie diese Funktion wie beschrieben.
 *
 */
void raster_signed_distance(tri tri, bool draw_bb=false, bool use_bb=true) {
	int x_min = 0;
	int x_max = res_x;
	int y_min = 0;
	int y_max = res_y;
	if (use_bb) {
		x_min = min(round(tri.a.x), round(tri.b.x), round(tri.c.x));
		x_max = max(round(tri.a.x), round(tri.b.x), round(tri.c.x));
		y_min = min(round(tri.a.y), round(tri.b.y), round(tri.c.y));
		y_max = max(round(tri.a.y), round(tri.b.y), round(tri.c.y));
	}
	if (draw_bb) {
		draw((x_min,y_min)--(x_max,y_min)--(x_max,y_max)--(x_min,y_max)--cycle);
	}
	for (int y = y_min; y <= y_max; y += 1)
		for (int x = x_min; x <= x_max; x += 1) {
			/*
			 * Pruefe ob Punkt (x,y) im Dreieck liegt.
			 * Er liegt im Dreieck, falls:
			 *   <(p-a)|n(b-a)> >= 0 &&
			 *   <(p-b)|n(c-b)> >= 0 &&
			 *   <(p-c)|n(a-c)> >= 0
			 */
			pair p = (x+0.5,y+0.5);

			if (dot(p-tri.a, line_normal(tri.a, tri.b)) < 0)
				continue;

			if (dot(p-tri.b, line_normal(tri.b, tri.c)) < 0)
				continue;

			if (dot(p-tri.c, line_normal(tri.c, tri.a)) < 0)
				continue;

			draw_pixel(x, y, B[1], B[2]);
		}
}

/*
 * Aufgabe 5 & 6.
 * Demonstrieren Sie ihre Implementierung mit einzelnen Beispielen.
 * Gruppierung in {} der Übersichtlichkeit halber.
 *
 */

{
	tri tri = tri((10.2,8.3), (2.7,2.2), (14.3,1.1));

	draw_pixelgrid(res_x,res_y,dots=lightgray);
	tri.draw(B[1]+linewidth(2));

	raster_signed_distance(tri, draw_bb=false, use_bb=false);

	shipout("raster-tri-1.pdf"); erase();
}


