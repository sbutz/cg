settings.tex = "pdflatex";

void draw_planet(transform t, real size, pen col) {
	fill(t*scale(size)*unitcircle, col);
	draw(t*scale(size)*unitcircle, col);
}

unitsize(0.1mm);

real sun_rad = 696;
real earth_rad = 6.3;
real earth_to_sun = 1500; // not to scale
real moon_rad = 1.7;
real moon_to_earth = 384;
real spaceelev_len = 35.8;

bool video_demo = false;
bool early_out = false;

if (video_demo) {
	sun_rad /= 10;
	earth_to_sun /= 10;
	moon_to_earth /= 10;
	spaceelev_len /= 3;
}

for (int d = 0; d < 365; d += 1) {

	for (int h = 0; h < 24; h+=2) {

		if (video_demo)
			filldraw(scale(400,400)*shift(-.5,-.5)*unitsquare, black, white+linewidth(1));
		else
			filldraw(scale(4000,4000)*shift(-.5,-.5)*unitsquare, black, white+linewidth(5));

		// This is the proper animation time
		real D = d + h/24;

		transform t_sun = identity;
		draw_planet(t_sun, sun_rad, orange+yellow);

		transform t_earth = rotate(360*D/365) * shift(earth_to_sun, 0)
			* t_sun;
		draw_planet(t_earth, earth_rad, blue+.4green);

		pair earth_center = t_earth * (0,0);
		transform t_elv = rotate(360*h/24, earth_center) * t_earth;
		draw(t_elv*((0,0)--(spaceelev_len,0)), white);

		transform t_moon = rotate(360*D/28, earth_center) *
			shift(moon_to_earth, 0) * t_earth;
		draw_planet(t_moon, moon_rad, lightgray);

		shipout("solar_" + format("%03d", d) + format("%02d", h) + ".png");//, bbox(p,Fill));
		erase();
		if (early_out)
			exit();
	}
	if (d%10==0) write(".");
}

