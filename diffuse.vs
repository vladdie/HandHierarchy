#version 330

// Note: Input to this shader is the vertex positions that we specified for the triangle. 
// Note: gl_Position is a special built-in variable that is supposed to contain the vertex position (in X, Y, Z, W)
// Since our triangle vertices were specified as vec3, we just set W to 1.0.

//vPosition no longer has to be between -1 and 1 because we can translate/scale/rotate it!
in vec3 vPosition;


//Matrices
uniform mat4 mM;
uniform mat4 mV;
uniform mat4 mP;
//uniform mat4 MVP;

in vec4 vColor;
out vec4 color;

void main()
{
	vec4 Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
	
	//Matrix multiplication. From local, to World, to camera, to NDCs
	gl_Position = mP * mV * mM * Position;
	//gl_Position = MVP * Position;
	
	color = vColor;
}