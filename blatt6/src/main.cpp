#include "mesh.h"
#include "shader.h"
#include "context.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

using namespace std;
using namespace glm;


mat4 perspective_projection_transform(float fovy, float aspect, float n, float f) {
	// TODO: Aufgabe 1.4
	// OpenGL Projektionsmatrix
	float F = 1.0/tan(M_PI*fovy/2/180.0);
	mat4 P(vec4(F/aspect, 0, 0, 0),
		   vec4(0, F, 0, 0),
		   vec4(0, 0, (n+f)/(n-f), -1),
		   vec4(0, 0, 2*n*f/(n-f), 0));
	return P;
}

mat4 viewing_transform(const vec3 &pos, const vec3 &dir, const vec3 &up) {
	// TODO: Aufgabe 1.5
	// OpenGL Viewingmatrix
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
	return V;
}


int main() {
	ContextParameters params;
	params.gl_major = 3;
	params.gl_minor = 3;
	params.title = "CG'20 mini renderer";
	Context::init(params);

	load_mesh();
	load_shader();

	while (Context::running()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2, 0.2, 0.2, 1);

		bind_shader();
		float aspect = float(Context::instance().vp_w) / Context::instance().vp_h;
		mat4 P = perspective_projection_transform(65, aspect, 1, 300);
		mat4 V = viewing_transform(vec3(180,0,50), vec3(-1, 0, 0), vec3(0,0,1));
		glUniformMatrix4fv(uniform_location("model"), 1, GL_FALSE, glm::value_ptr(mat4(1)));
		glUniformMatrix4fv(uniform_location("view"), 1, GL_FALSE, glm::value_ptr(V));
		glUniformMatrix4fv(uniform_location("proj"), 1, GL_FALSE, glm::value_ptr(P));
		draw_mesh();
		unbind_shader();

		Context::swap_buffers();
	}

	//TODO: shader hier aufraeumen (auch noch bei Fehlerbehandlung vorher)

	cout << "All seems to be fine :)" << endl;
	return 0;
}
