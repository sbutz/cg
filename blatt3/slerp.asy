import graph3;
import solids;
import quat;

currentlight=White;
defaultrender.merge=true;
currentlight=nolight;

size(10cm,0);

revolution sph=sphere((0,0,0.0),1.0);
draw(surface(sph),white*0.9+opacity(.8), meshpen=0.6*white+linewidth(1));


V dir0 = vector(-.1,.5,.9);
Q p0 = quaternion(scale(dir0, 1.0/length(dir0)), 0);

V dir1 = vector(0.9,1.0,-1);
//V dir1 = vector(1,1,1);
Q p1 = quaternion(scale(dir1, 1.0/length(dir1)), 0);

draw((0,0,0)---(1.0,0,0), dotted); draw((1,0,0)---(1.2,0,0), Arrow3); label("Z", (1.25,0,0));
draw((0,0,0)---(0,1.0,0), dotted); draw((0,1,0)---(0,1.2,0), Arrow3); label("X", (0,1.25,0));
draw((0,0,0)---(0,0,1.0), dotted); draw((0,0,1)---(0,0,1.2), Arrow3); label("Y", (0,0,1.25));

triple p = p0.v.t();
draw((0,0,0)--(p.x,0,0)--(p.x,p.y,0)--(p.x,p.y,p.z), dashed+blue);
draw((0,0,0)---p0.v.t(), Arrow3);

triple p = p1.v.t();
draw((0,0,0)--(p.x,0,0)--(p.x,p.y,0)--(p.x,p.y,p.z), dashed+red);
draw((0,0,0)---p1.v.t(), Arrow3);

triple lerp(triple q, triple r, real t) {
	triple interpol = q + t*(r-q);
	return interpol;
}
triple nlerp(triple q, triple r, real t) {
	triple interpol = lerp(q,r,t);
	return interpol / length(interpol);
}
Q slerp(Q q, Q r, real t) {
	real phi = acos(dot(q.v,r.v) + q.w*r.w); // rad to deg
	return add(scale(q, sin((1-t)*phi)/sin(phi)),
		   scale(r, sin(t*phi)    /sin(phi)));
	// TODO
	// Achtung, hier wird nicht wie in nlerp mit tripeln gerechnet, sondern mit
	// den in quat.asy implementierten Quaternionen.
	return q;
}

int N = 10;
path3 arc = p0.v.t();
int lerp = 0, nlerp = 1, slerp = 2;
int mode = slerp;

if (mode != slerp)
	for (int i = 1; i <= N; i = i + 1) {
		triple res;
		if (mode == lerp)
			res = lerp(p0.v.t(), p1.v.t(), i/(real)N);
		else if (mode == nlerp)
			res = nlerp(p0.v.t(), p1.v.t(), i/(real)N);
		arc = arc -- res;
	}
else
	for (int i = 1; i <= N; i = i + 1) {
		Q q = slerp(p0, p1, i/(real)N);
		arc = arc -- q.v.t();
	}

draw(arc, heavygreen+linewidth(1));
dot(arc, heavygreen+linewidth(2));

