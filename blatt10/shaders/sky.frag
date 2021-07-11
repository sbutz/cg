#version 150

in vec3 pos_wc;
uniform sampler2D tex;
out vec4 out_col;

#define PI 3.1415926535897932384626433832795

void main() {
	// TODO:
	// Bestimmen Sie die (normalisierte) Richtung in der sich das aktuelle
	// Fragment von der Kamera aus befindet
	vec3 dir = vec3(0,0,0);

	// TODO:
	// Bestimmen Sie die Texturkoordinaten wie in der VL beschrieben
	vec2 uv = vec2(0,
				   0);
	
	out_col = vec4(texture(tex, uv).rgb, 1);
	
	// Falls Sie die berechnete Textur-Koordinate ausgeben wollen:
	// out_col = vec4(uv,0,1);
}
