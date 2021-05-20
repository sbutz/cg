#include "raster.h"
#include "cmdline.h"

#include <glm/gtc/random.hpp>

using namespace glm;
using namespace png;
using namespace std;

bool overlaps_pixel(const vec2 &p, const triangle &tri, render_target &target);

vector<rgb_pixel> color_palette;

vec3 barycentric_coord(const triangle &tri, vec2 p) {
	vec2 a = tri.a,
		 b = tri.b,
		 c = tri.c;
	vec2 u = b - a;
	vec2 v = c - a;
	float one_over_det = 1.0 / (u.x*v.y-u.y*v.x);
	vec3 res;
	vec2 to_p = p - a;
	res.y = one_over_det * ( v.y * to_p.x - v.x * to_p.y);
	res.z = one_over_det * (-u.y * to_p.x + u.x * to_p.y);
	res.x = 1.0 - res.y - res.z;
	return res;
}

render_target raster_triangles(const vector<triangle> &triangles)
{
	// set up color palette to visualize different triangles
	color_palette.resize(triangles.size());
	for (int i = 0; i < triangles.size(); ++i)
		color_palette[i] = rgb_pixel(linearRand(0,255), linearRand(0,255), linearRand(0,255));

	// set up render target and clear to black
	render_target target;
	float *depthbuffer = new float[cmdline.vp_w * cmdline.vp_h];
	target.framebuffer.resize(cmdline.vp_w, cmdline.vp_h);
	for (png::uint_32 y = 0; y < cmdline.vp_h; ++y)
		for (png::uint_32 x = 0; x < cmdline.vp_w; ++x) {
			target.framebuffer[y][x] = png::rgb_pixel(60,60,60);
			depthbuffer[y*cmdline.vp_w + x] = cmdline.f;
		}

	// go over all triangles
	for (int i = 0; i < triangles.size(); ++i) {
		const triangle &tri = triangles[i];
		// find bounding box
		vec3 bb_min, bb_max;
		bb_min = floor(min(tri.a, min(tri.b, tri.c)));
		bb_max = ceil(max(tri.a, max(tri.b, tri.c)));
		// clip bounding box
		bb_min.x = fmaxf(bb_min.x, 0.0f);
		bb_min.y = fmaxf(bb_min.y, 0.0f);
		bb_max.x = fminf(bb_max.x, cmdline.vp_w-1.0f);
		bb_max.y = fminf(bb_max.y, cmdline.vp_h-1.0f);
		bb_min.x = bb_min.y = 0;
		bb_max.x = cmdline.vp_w;
		bb_max.y = cmdline.vp_h;
		// go over all pixels in bounding box
		// NOTE: here, we assume to be in window coordinates, NOT NDC
		for (int y = int(bb_min.y); y < int(bb_max.y); ++y)
			for (int x = int(bb_min.x); x < int(bb_max.x); ++x) {
				vec2 p = vec2(x+0.5f, y+0.5f);
				if (overlaps_pixel(p, tri, target)) {
					vec3 bc = barycentric_coord(tri, p);
					float z = tri.a.z * bc.x + tri.b.z * bc.y + tri.c.z * bc.z;
					if (z < depthbuffer[y*cmdline.vp_w + x]) { 
						depthbuffer[y*cmdline.vp_w + x] = z;
						int inv_y = target.framebuffer.get_height()-1-y;
						target.framebuffer[inv_y][x] = color_palette[i];
					}
				}
			}
	}

	delete [] depthbuffer;
	return target;
}


vec2 line_normal(vec2 a, vec2 b)
{
	vec2 v = normalize(b-a);
	return vec2(-v.y, v.x);
}

bool overlaps_pixel(const vec2 &p, const triangle &tri, render_target &target)
{
	if (dot(line_normal(tri.a, tri.b), p - vec2(tri.a)) < 0) return false;
	if (dot(line_normal(tri.b, tri.c), p - vec2(tri.b)) < 0) return false;
	if (dot(line_normal(tri.c, tri.a), p - vec2(tri.c)) < 0) return false;

	if (tri.a.z < cmdline.n || tri.b.z < cmdline.n || tri.c.z < cmdline.n) return false;
 	if (tri.a.z > cmdline.f || tri.b.z > cmdline.f || tri.c.z > cmdline.f) return false;

	return true;
}
