#version 420 core

layout( location = 0 ) in vec3 i_vertexPosition_local;
layout( location = 1 ) in vec3 i_vertexNormal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
		vec4 vertexPosition_local = vec4( i_vertexPosition_local, 1.0 );
		vec4 vertexPosition_world = u_model * vertexPosition_local;
		vec4 vertexPosition_camera = u_view * vertexPosition_world;
		gl_Position = u_projection * vertexPosition_camera;
}