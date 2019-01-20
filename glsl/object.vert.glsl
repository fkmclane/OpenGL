#version 150

// vertex information
in vec3 coords;
in vec2 texture_coords;
in vec3 normal;

// transformations
uniform mat4 obj_transform;
uniform mat4 world_transform;
uniform mat4 perspective;

// shared
out vec3 v_coords;
out vec2 v_texture_coords;
out vec3 v_normal;

void main() {
	// multiply transformations by coordinates to get position
	gl_Position = perspective * world_transform * obj_transform * vec4(coords, 1.0);

	// share information
	v_coords = coords;
	v_texture_coords = texture_coords;
	v_normal = normal;
}
