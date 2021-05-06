#include "cmdline.h"
#include "scene.h"
#include "raster.h"

#include <iostream>
#include <functional>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace glm;

mat4 window_transform(int w, int h, float n, float f) {
	// NOTE: initializers are column vectors
	mat4 W(vec4(cmdline.vp_w/2, 0, 0, 0),
		   vec4(0, cmdline.vp_h/2, 0, 0),
		   vec4(0, 0, (f-n)/2, 0),
		   vec4(cmdline.vp_w/2, cmdline.vp_h/2, (f+n)/2, 1));
	return W;
}

mat4 perspective_projection_transform(float fovy, float aspect, float n, float f) {
	// NOTE: initializers are column vectors
	// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
	float F = 1.0/tan(M_PI*fovy/2/180.0);
	mat4 P(vec4(F/aspect, 0, 0, 0),
		   vec4(0, F, 0, 0),
		   vec4(0, 0, (n+f)/(n-f), -1),
		   vec4(0, 0, 2*n*f/(n-f), 0));
	return P;
}

mat4 viewing_transform(const vec3 &pos, const vec3 &dir, const vec3 &up) {
	// NOTE: initializers are column vectors
	vec3 w = normalize(-dir);
	vec3 u = normalize(cross(up, w));
	vec3 v = cross(w, u);
	mat4 trans(vec4(1,0,0,0),
			   vec4(0,1,0,0),
			   vec4(0,0,1,0),
			   vec4(-pos.x,-pos.y,-pos.z,1));
	mat4 rot(vec4(u.x, v.x, w.x, 0),
			 vec4(u.y, v.y, w.y, 0),
			 vec4(u.z, v.z, w.z, 0),
			 vec4(0,0,0,1));
	mat4 V = rot*trans;
// 	cout << to_string(V) << endl;
	return V;
}

int main(int argc, char **argv)
{
	parse_cmdline(argc, argv);

	cout << to_string(cmdline.cam_pos) << endl;
	cout << to_string(cmdline.view_dir) << endl;
	cout << to_string(cmdline.world_up) << endl;
	cout << cmdline.n << " : " << cmdline.f << endl;

	vector<triangle> triangles = scene_triangles();

	mat4 W = window_transform(cmdline.vp_w, cmdline.vp_h, cmdline.n, cmdline.f);
	mat4 P = perspective_projection_transform(cmdline.fovy,
											  float(cmdline.vp_w)/cmdline.vp_h,
											  cmdline.n,
											  cmdline.f);
	mat4 V = viewing_transform(cmdline.cam_pos, cmdline.view_dir, cmdline.world_up);

	auto W_transform = [&](const vec3 &v) {
		return W*vec4(v,1);
	};
	auto PW_transform = [&](const vec3 &v) {
		auto clip = P*vec4(v,1);
		auto ndc = clip/clip.w;
		return W*ndc;
	};
	auto VPW_transform = [&](const vec3 &v) {
		auto clip = P*V*vec4(v,1);
		auto ndc = clip/clip.w;
		return W*ndc;
	};

	function<vec3(const vec3&)> vertex_trasnform;
	if (cmdline.pipeline == window_only)      vertex_trasnform = W_transform;
	if (cmdline.pipeline == proj_window)      vertex_trasnform = PW_transform;
	if (cmdline.pipeline == view_proj_window) vertex_trasnform = VPW_transform;
	
	for (auto &tri : triangles) {
		tri.a = vertex_trasnform(tri.a);
		tri.b = vertex_trasnform(tri.b);
		tri.c = vertex_trasnform(tri.c);
	}
	auto image = raster_triangles(triangles);
	image.framebuffer.write(cmdline.outfile);

	return 0;
}
