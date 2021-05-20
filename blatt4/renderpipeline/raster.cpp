#include "raster.h"

#include <iostream>
#include <glm/gtc/random.hpp>

using namespace glm;
using namespace png;
using namespace std;

bool overlaps_pixel(const vec2 &p, const triangle &tri, render_target &target);
vec3 barycentric(const triangle &tri, const vec2 &p);

vector<rgb_pixel> color_palette;

render_target raster_triangles(const vector<triangle> &triangles)
{
	// set up color palette to visualize different triangles
	color_palette.resize(triangles.size());
	for (int i = 0; i < triangles.size(); ++i)
		color_palette[i] = rgb_pixel(linearRand(0,255), linearRand(0,255), linearRand(0,255));

	// set up render target and clear to dark grey
	render_target target;
	target.framebuffer.resize(cmdline.vp_w, cmdline.vp_h);
	for (png::uint_32 y = 0; y < cmdline.vp_h; ++y)
		for (png::uint_32 x = 0; x < cmdline.vp_w; ++x)
			target.framebuffer[y][x] = png::rgb_pixel(60,60,60);

	// setup depth buffer
	float depth_buffer[cmdline.vp_h][cmdline.vp_w]{};
	for (int y = 0; y < cmdline.vp_h; ++y)
		for (int x = 0; x < cmdline.vp_w; ++x)
			depth_buffer[y][x] = cmdline.f;

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
		// go over all pixels in bounding box
		// NOTE: here, we assume to be in window coordinates, NOT NDC
		for (int y = int(bb_min.y); y < int(bb_max.y); ++y)
			for (int x = int(bb_min.x); x < int(bb_max.x); ++x) {
				vec2 p = vec2(x+0.5f, y+0.5f);
				if (overlaps_pixel(p, tri, target)) {
					int inv_y = target.framebuffer.get_height()-1-y;

					// interpolate z value in point p
					vec3 bc = barycentric(tri, p);
					float z = dot(bc, vec3(tri.a.z, tri.b.z, tri.c.z));

					// depth check
					if (z < depth_buffer[inv_y][x]) {
						depth_buffer[inv_y][x] = z;
						target.framebuffer[inv_y][x] = color_palette[i];
					}
				}
			}
	}

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

	if (cmdline.pipeline != window_only)
		if (tri.a.z < 0 || tri.b.z < 0 || tri.c.z < 0)
			return false;

	return true;
}

vec3 barycentric(const triangle &tri, const vec2 &p) {
		vec2 a = vec2(tri.a.x, tri.a.y);
		vec2 b = vec2(tri.b.x, tri.b.y);
		vec2 c = vec2(tri.c.x, tri.c.y);

		vec2 u = b - a;
		vec2 v = c - a;

		float d = u.x*v.y - u.y*v.x;
		float beta = 1.0f / d * dot(vec2(v.y, -v.x), p-a);
		float gamma = 1.0f / d * dot(vec2(-u.y, u.x), p-a);
		float alpha = 1.0f - beta - gamma;

		return vec3(alpha, beta, gamma);
}
