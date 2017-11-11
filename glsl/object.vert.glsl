#version 130

// vertex information
attribute vec3 coords;
attribute vec2 texture_coords;
attribute vec3 normal;

// transformations
uniform mat4 obj_transform;
uniform mat4 world_transform;
uniform mat4 perspective;

// shared
varying vec3 v_coords;
varying vec2 v_texture_coords;
varying vec3 v_normal;

void main() {
	// multiply transformations by coordinates to get position
	gl_Position = perspective * world_transform * obj_transform * vec4(coords, 1.0);

	// share information
	v_coords = coords;
	v_texture_coords = texture_coords;
	v_normal = normal;
}
