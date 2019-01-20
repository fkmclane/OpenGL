#version 150

// texture
uniform sampler2D tex;

// shared
out vec3 v_coords;
out vec2 v_texture_coords;
out vec3 v_normal;

// color output
out vec4 frag_data;

void main() {
	// update pixel with color from texture corresponding to this object coordinate
	frag_data.rgb = texture(tex, v_texture_coords).rgb + v_normal;
	frag_data.a = 1;
}
