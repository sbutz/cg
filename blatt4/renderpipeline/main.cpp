#include "cmdline.h"
#include "scene.h"
#include "raster.h"

#include <iostream>
#include <functional>

// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace glm;

mat4 window_transform(int w, int h, float n, float f) {
	// Da wir das ganze Bild füllen ist der Offset jeweils 0.
	// ACHTUNG. Bei GLM (wie bei OpenGL generell) werden Column-Major Matrizen verwendet!
	// mat4(vec4(1,2,3,4),                         [ 1 5 9 3 ]
	//      vec4(5,6,7,8),    entspricht deshalb   [ 2 6 0 4 ]
	//      vec4(9,0,1,2),                         [ 3 7 1 5 ]
	//      vec4(3,4,5,6));                        [ 4 8 2 6 ]
	// Sie können die Matrizen mit
	//     cout << to_string(V) << endl;
	// ausgeben (beachten Sie dafür auch die beiden auskommentieren Präprozessorzeilen oben)
	//
	mat4 W = mat4(vec4(w/2,   0,       0, 0),
				  vec4(  0, h/2,       0, 0),
				  vec4(  0,   0, (f-n)/2, 0),
				  vec4(w/2, h/2, (f+n)/2, 1));
	return W;
}

mat4 perspective_projection_transform(float fovy, float aspect, float n, float f) {
	// Ein Beispiel finden Sie hier: 
	// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
	// Achtung, wie oben beschrieben verwenden wir Column-Major Matrizen.
	//
	fovy = fovy * M_PI/180;
	fovy = cos(fovy/2)/sin(fovy/2);
	mat4 P = mat4(vec4(fovy/aspect, 0, 0, 0),
				  vec4(0, fovy, 0, 0),
				  vec4(0, 0, (f+n)/(n-f), -1.0),
				  vec4(0, 0, (2*f*n)/(n-f), 0));
	return P;
}

mat4 viewing_transform(const vec3 &pos, const vec3 &dir, const vec3 &up) {
	// TODO Geben sie die View-Matrix an.
	// Die Transformationsrichtung ist von der Welt IN den Eye Space
	// Achtung, wie oben beschrieben verwenden wir Column-Major Matrizen.
	//
	vec3 w = normalize(dir * -1.0f);
	vec3 u = normalize(cross(up, w));
	vec3 v = normalize(cross(w, u));
	mat4 R = mat4(vec4(u, 0),
				  vec4(v, 0),
				  vec4(w, 0),
				  vec4(0, 0, 0, 1));
	mat4 T = mat4(vec4(1, 0, 0, 0),
				  vec4(0, 1, 0, 0),
				  vec4(0, 0, 1, 0),
				  vec4(pos, 1.0f));
	// global -> eye
	return inverse(T * R);
}

int main(int argc, char **argv)
{
	parse_cmdline(argc, argv);

	vector<triangle> triangles = scene_triangles();

	mat4 W = window_transform(cmdline.vp_w, cmdline.vp_h, cmdline.n, cmdline.f);
	mat4 P = perspective_projection_transform(cmdline.fovy,
											  float(cmdline.vp_w)/cmdline.vp_h,
											  cmdline.n,
											  cmdline.f);
	mat4 V = viewing_transform(cmdline.cam_pos, cmdline.view_dir, cmdline.world_up);

	auto W_transform = [&](const vec3 &v) {
		return W * vec4(v, 1.0f);
	};
	auto PW_transform = [&](const vec3 &v) {
		auto clip = P * vec4(v, 1.0f);
		auto ndc = clip/clip.w;
		return W * ndc;
	};
	auto VPW_transform = [&](const vec3 &v) {
		auto clip = P * V * vec4(v, 1.0f);
		auto ndc = clip/clip.w;
		return W * ndc;
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
