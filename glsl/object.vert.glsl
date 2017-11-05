#version 120

// coordinates
attribute vec3 coords;
attribute vec2 texture_coords;

// transformations
uniform mat4 obj_transform;
uniform mat4 world_transform;
uniform mat4 perspective;

// shared
varying vec2 v_texture_coords;

void main() {
	// multiply transformations by coordinates to get position
	gl_Position = perspective * world_transform * obj_transform * vec4(coords, 1.0);

	// share texture information
	v_texture_coords = texture_coords;
}
