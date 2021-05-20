#include "scene.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace std;
using namespace glm;

vector<triangle> scene_triangles()
{
	vector<triangle> tris;
	if (cmdline.scene == ndc_tri) {
		float z = 0;
		if (cmdline.user_z_defined)
			z = cmdline.user_z;
		tris.push_back({vec3(-0.3,-0.3,z),
					    vec3( 0.3,-0.3,z),
						vec3( 0.0, 0.4,z)});
	}
	// KAI make this part of the assignment?
	else if (cmdline.scene == es_tri) {
		float z = -(cmdline.n+cmdline.f)/16;
		if (cmdline.user_z_defined)
			z = cmdline.user_z;
		tris.push_back({vec3( 0 ,0,z),
					    vec3(10, 0,z),
						vec3( 0,10,z)});
	}
	else if (cmdline.scene == bunny) {
		vector<vec3> verts;
		auto add_vert = [&](float x, float y, float z, float, float, float) {
			verts.push_back(vec3(x,y,z));
		};
		auto add_tri = [&](int a, int b, int c) {
			tris.push_back({verts[a], verts[b], verts[c]});
		};
		#include "bunny.data"

		// Info about bounding box of the model
		vec3 min = tris.front().a, max = tris.front().a;
		for (auto &x : tris) {
			min = glm::min(glm::min(min, x.a),
						   glm::min(x.b, x.c));
			max = glm::max(glm::max(max, x.a),
						   glm::max(x.b, x.c));
		}
		cout << "Bounding box: " << to_string(min) << "  x  " << to_string(max) << endl;
	}
	return tris;
}
