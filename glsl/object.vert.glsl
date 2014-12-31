#version 120

//Coordinates
attribute vec3 coords;
attribute vec2 texture_coords;

//Transformations
uniform mat4 obj_transform;
uniform mat4 world_transform;
uniform mat4 perspective;

//Shared
varying vec2 v_texture_coords;

void main() {
	//Multiply transformtions by coordinates to get position
	gl_Position = perspective * world_transform * obj_transform * vec4(coords, 1.0);

	//Share texture information
	v_texture_coords = texture_coords;
}
