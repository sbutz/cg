pen B[] = { rgb("2e75b6"), rgb("5b9bd5"), rgb("9dc3e6") };
pen O[] = { rgb("f6bd97"), rgb("ed7d31"), rgb("f9cbad") };
pen G[] = { rgb("000000"), rgb("70ad47"), rgb("a9d18e") };
pen Y[] = { rgb("ffdf7f"), rgb("ffc000"), rgb("000000") };

unitsize(1cm);

// 
// Grundlegende Datentypen
// 

struct vec4 {
	real x, y, z, w;
	real val(int i) { if (i == 0) return x; if (i == 1) return y; if (i == 2) return z; if (i == 3) return w; return 1/0; }
	void set(int i, real v) { if (i == 0) x=v; else if (i == 1) y=v; else if (i == 2) z=v; else if (i == 3) w=v; else 1/0; }

};
void write(vec4 v) {
	write(v.x, v.y, v.z, v.w);
}
vec4 make_vec(real x, real y, real z, real w=1) {
	vec4 v;
	v.x = x;
	v.y = y;
	v.z = z;
	v.w = w;
	return v;
}
pair assume_2d(vec4 v) { return (v.x, v.y); }
struct mat4 {
	real a[] = new real[16];
	real val(int row, int col) {
		return a[row*4+col];
	}
	void set(int row, int col, real val) {
		a[row*4+col] = val;
	}
};
mat4 make_mat4() {
	mat4 m;
	for (int i = 0; i < 16; ++i)
		m.a[i] = 0;
	m.a[0] = m.a[5] = m.a[10] = m.a[15] = 1;
	return m;
}

real pi = 3.14159265359;

// 
// Matrizen
// 

mat4 make_proj(real fovy, real aspect, real n, real f) {
	mat4 p = make_mat4();
	real F = 1.0/tan(pi*fovy/2/180);
	p.set(0,0, F/aspect);
	p.set(1,1, F);
	p.set(2,2, (n+f)/(n-f));
	p.set(3,3, 0);
	p.set(2,3, 2*n*f/(n-f));
	p.set(3,2, -1);
	return p;
}

mat4 make_window(real w, real h, real n, real f) {
	mat4 m = make_mat4();
	m.set(0,0, w/2);
	m.set(1,1, h/2);
	m.set(2,2, (f-n)/2);
	m.set(0,3, w/2);
	m.set(1,3, h/2);
	m.set(2,3, (f+n)/2);
	return m;
}

vec4 mul(mat4 m, vec4 v) {
	vec4 res;
	for (int row = 0; row < 4; row = row + 1) {
		real acc = 0;
		for (int col = 0; col < 4; col = col + 1)
			acc += m.val(row, col) * v.val(col);
		res.set(row, acc);
	}
	return res;
}

// 
// Vertex Attributes:
// Alles was mit einem Dreicks-Eckpunkt mitkommt
// 
struct vertex_attributes {
	vec4 pos;
	pair tc;
	real rcp_z;
};

// 
// Dreiecke: Pro Eckpunkt einen Satz von Vertex Attributes
// 
struct tri {
	vertex_attributes a, b, c;
};
tri make_tri(vec4 a, pair tex_coord_a,
             vec4 b, pair tex_coord_b,
             vec4 c, pair tex_coord_c) {
	tri t;
	t.a.pos = a; t.a.tc = tex_coord_a;
	t.b.pos = b; t.b.tc = tex_coord_b;
	t.c.pos = c; t.c.tc = tex_coord_c;
	return t;
}

// 
// Füllen eines Pixel
// 
void draw_pixel(real x, real y, pen border = black+linewidth(2), pen fill = lightgray) {
	void draw_pixel_discrete(int x, int y) {
		filldraw((x,y)--(x+1,y)--(x+1,y+1)--(x,y+1)--cycle, fill, border);
	}
	draw_pixel_discrete((int)round(x), (int)round(y));
}

void draw_pixelgrid(int w, int h, pen lines = lightgray, pen dots = invisible) {
	for (int x = 0; x <= w; ++x)
		draw((x,0)--(x,h), lightgray);
	for (int y = 0; y <= h; ++y)
		draw((0,y)--(w,y), lightgray);
	for (int x = 0; x < w; ++x)
		for (int y = 0; y < h; ++y)
			dot((x+.5,y+.5), linewidth(4)+dots);
}

// 
// SD-Rasterisierung wie in der ersten Übung
// (auch: aufgeräumte Referenz für Teile von Blatt1)
// 
pair line_normal(vec4 A, vec4 B) {
	// suppose the vec4 is 2d
	pair a = assume_2d(A);
	pair b = assume_2d(B);
	pair v = b-a;
	v = v/sqrt(dot(v,v));
	return (-v.y,v.x);
}

// (Diese Funktion wird weiter unten definiert und soll von Ihnen vervollständigt werden)
pen pixel_color(real x, real y, tri triangle);

void raster_planes(tri tri) {
	pair n_a = line_normal(tri.a.pos, tri.b.pos);
	pair n_b = line_normal(tri.b.pos, tri.c.pos);
	pair n_c = line_normal(tri.c.pos, tri.a.pos);
	int x_min = (int)floor(min(tri.a.pos.x, tri.b.pos.x, tri.c.pos.x));
	int x_max = (int)ceil( max(tri.a.pos.x, tri.b.pos.x, tri.c.pos.x));
	int y_min = (int)floor(min(tri.a.pos.y, tri.b.pos.y, tri.c.pos.y));
	int y_max = (int)ceil( max(tri.a.pos.y, tri.b.pos.y, tri.c.pos.y));
	for (int y = y_min; y <= y_max; y += 1)
		for (int x = x_min; x <= x_max; x += 1) {
			bool hit = false;
			bool point_in_tri(real pixel_x, real pixel_y) {
				if (dot(n_a, (pixel_x, pixel_y) - assume_2d(tri.a.pos)) < 0) return false;
				if (dot(n_b, (pixel_x, pixel_y) - assume_2d(tri.b.pos)) < 0) return false;
				if (dot(n_c, (pixel_x, pixel_y) - assume_2d(tri.c.pos)) < 0) return false;
				return true;
			}
			real pixel_x = x + 0.5;
			real pixel_y = y + 0.5;
			hit = point_in_tri(pixel_x, pixel_y);
			if (hit) {
				pen col = pixel_color(pixel_x, pixel_y, tri);
				draw_pixel(x, y, border=col, fill=col);
			}
		} 
}

// 
// Die Texture
// 
pen checkerboard_texture(pair tex_coord) {
	pair tc = (abs(tex_coord.x) % 1, abs(tex_coord.y) % 1);
	if (tc.x > 0.5)
		if (tc.y > 0.5)
			return black;
		else
			return white;
	else
		if (tc.y > 0.5)
			return white;
		else
			return black;
}

// 
// Baryzentrische Koordinate, wie in VL02 besprochen.  Ein Objekt dieser Klasse
// enthält schon die Koordinaten und kann entsprechend verwendet werden um mit
// hilfe dieser Koordinaten an den Eckpunkten definierte Werte zu interpolieren.
// 
struct barycentric_coordinate {
	real alpha, beta, gamma;
	real interpolate(real a, real b, real c) {
		return alpha*a + beta*b + gamma*c;
	}
	pair interpolate(pair a, pair b, pair c) {
		return alpha*a + beta*b + gamma*c;
	}
};

// 
// Berechnung der baryzentrischen Koordinate eines Punkts p bezüglich eines Dreiecks.
// 
barycentric_coordinate compute_barycentric_coord(pair p, tri triangle) {
	pair a = assume_2d(triangle.a.pos);
	pair b = assume_2d(triangle.b.pos);
	pair c = assume_2d(triangle.c.pos);
	pair u = b-a;
	pair v = c-a;
	real d = u.x*v.y - u.y*v.x;
	barycentric_coordinate bc;
	bc.beta  = dot((v.y, -v.x), p-a) / d;
	bc.gamma = dot((-u.y, u.x), p-a) / d;
	bc.alpha = 1 - bc.beta - bc.gamma;
	return bc;
}


// 
// Ab hier können Sie Code anpassen
// 

bool PCI = true;

pen fragment_shader(real x, real y, tri triangle, pair tc) {
	return checkerboard_texture(tc);
}

pen fixed_function_fragment(real x, real y, tri triangle) {
	barycentric_coordinate bc = compute_barycentric_coord((x, y), triangle);
	pair tc = bc.interpolate(triangle.a.tc, triangle.b.tc, triangle.c.tc);
	return fragment_shader(x, y, triangle, tc);
}

pixel_color = new pen(real x, real y, tri triangle) {
	return fixed_function_fragment(x, y, triangle);
};

int vp_w = 150;
int vp_h = 150;
draw_pixelgrid(vp_w, vp_h);

mat4 W = make_window(vp_w, vp_h, 1, 100);
mat4 P = make_proj(37, vp_w/(real)vp_h, 1, 100);
vertex_attributes vertex_shader(vertex_attributes view) {
	vertex_attributes clip;
	clip.pos = mul(P, view.pos);
	clip.tc = view.tc;
	return clip;
}
void fixed_function_vertex(vertex_attributes vattr) {
	vattr.rcp_z  = 1 / vattr.pos.w;
	vec4 ndc     = make_vec(vattr.pos.x*vattr.rcp_z, vattr.pos.y*vattr.rcp_z, vattr.pos.z*vattr.rcp_z, 1); 
	vattr.pos    = mul(W, ndc);
}
tri pipe(tri in) {
	tri out;
	out.a = vertex_shader(in.a);
	out.b = vertex_shader(in.b);
	out.c = vertex_shader(in.c);
	fixed_function_vertex(out.a);
	fixed_function_vertex(out.b);
	fixed_function_vertex(out.c);
	return out;
}

// 
// Ab hier sollten Sie nur die Konfiguration via mode ändern.
// 

tri t1, t2;
string mode = "head-on";
// mode = "slant";
// mode = "SLANT";
real tc_max = 3;

if (mode == "head-on") {
	t1 = make_tri(make_vec(-3, -3, -10), (0,0),
				  make_vec( 3, -3, -10), (tc_max,0),
				  make_vec( 3,  3, -10), (tc_max,tc_max));

	t2 = make_tri(make_vec(-3, -3, -10), (0,0),
				  make_vec( 3,  3, -10), (tc_max,tc_max),
				  make_vec(-3,  3, -10), (0,tc_max));
}
else if (mode == "slant") {
	t1 = make_tri(make_vec(-3, -3, -10), (0,0),
				  make_vec( 3, -3, -15), (tc_max,0),
				  make_vec( 3,  3, -15), (tc_max,tc_max));

	t2 = make_tri(make_vec(-3, -3, -10), (0,0),
				  make_vec( 3,  3, -15), (tc_max,tc_max),
				  make_vec(-3,  3, -10), (0,tc_max));
}
else if (mode == "SLANT") {
	t1 = make_tri(make_vec(-3, -3, -10), (0,0),
				  make_vec( 3, -3, -25), (tc_max,0),
				  make_vec( 3,  3, -25), (tc_max,tc_max));

	t2 = make_tri(make_vec(-3, -3, -10), (0,0),
				  make_vec( 3,  3, -25), (tc_max,tc_max),
				  make_vec(-3,  3, -10), (0,tc_max));
}

tri ss1 = pipe(t1);
tri ss2 = pipe(t2);

raster_planes(ss1);
raster_planes(ss2);

