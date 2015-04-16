
#include <GL/glew.h>
//#include <GL/freeglut.h>

#include <string> 
#include <fstream>
#include <iostream>
#include <sstream>

//include GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/matrix_cross_product.hpp>
// Include GLFW
#include <GLFW/glfw3.h>
#include "common/maths_funcs.h"
#include "common/cylinder.h"
//#include <skeleton.h>
#include <Node.h>
#include <hand.h>
#include <skeleton.h>
#include <Target.h>
#include <glm/gtx/rotate_vector.hpp> 
#include <computeIK.h>
GLFWwindow* window;

#include <common/control.hpp>
#include <common/cylinder.h>
#include <common/vboIndexer.hpp>
#include "assimpLoader.h"

//using namespace glm;
using namespace std;

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define MESH_NAME "hand.dae"

//IDs of varibles mP, mV, mM in the shader
GLuint perspectiveMatrixID, viewMatrixID, modelMatrixID;
GLfloat theta;
GLfloat theta1, theta2;
float phi;
GLuint shaderProgramID;
glm::mat4 Projection, View, Model;
glm::mat4 rotXMatrix, rotYMatrix, rotZMatrix;
GLuint vao;
Cylinder* cube;
//Cylinder* cubeLab3;
Cylinder* cylinder;
hand* Hand;
skeleton* Skeleton;
Target* target;
GLuint VertexArrayID;
GLuint TextureID;
GLuint LightID;
GLuint vertexbuffer_vbo, uvbuffer_vbo, normalbuffer_vbo, elementbuffer, boneId_vbo;
std::vector<unsigned short> indices;
//float counter = 0;

bool LoadFile(const std::string& fileName, std::string& outShader)
{
	std::ifstream file(fileName);
	if(!file.is_open())
	{
		std::cout << "Error Loading file: " << fileName << " - impossible to open file" << std::endl;
		return false;
	}

	if(file.fail())
	{
		std::cout << "Error Loading file: " << fileName << std::endl;
		return false;
	}

	std::stringstream stream;
	stream << file.rdbuf();
	file.close();

	outShader = stream.str();

	return true;
}

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	// create a shader object
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(0);
	}
	// Bind the source code to the shader, this happens before compilation
	glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderText, NULL);
	// compile the shader and check for errors
	glCompileShader(ShaderObj);
	GLint success;
	// check for shader related errors using glGetShaderiv
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}
	// Attach the compiled shader object to the program object
	glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders(const std::string& vsFilename, const std::string& psFilename)
{
	//Start the process of setting up our shaders by creating a program ID
	//Note: we will link all the shaders together into this ID
	shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	// Create two shader objects, one for the vertex, and one for the fragment shader
	std::string vs, ps;
	LoadFile(vsFilename, vs);
	AddShader(shaderProgramID, vs.c_str(), GL_VERTEX_SHADER);
	LoadFile(psFilename, ps);
	AddShader(shaderProgramID, ps.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	// After compiling all shader objects and attaching them to the program, we can finally link it
	glLinkProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	// program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
	glValidateProgram(shaderProgramID);
	// check for program related errors using glGetProgramiv
	glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}
	// Finally, use the linked shader program
	// Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
	glUseProgram(shaderProgramID);
	return shaderProgramID;
}

void linkCurrentBuffertoShader(GLuint shaderProgramID){
	
	// find the location of the variables that we will be using in the shader program
	//ID of the variables inside the shader
	//"vposition" one of the variables inside the shader program
	GLuint positionID = glGetAttribLocation(shaderProgramID, "vPosition");
	GLuint colorID = glGetAttribLocation(shaderProgramID, "vColor");
	viewMatrixID = glGetUniformLocation(shaderProgramID, "mV");
	modelMatrixID = glGetUniformLocation(shaderProgramID, "mM");
	perspectiveMatrixID = glGetUniformLocation(shaderProgramID, "mP");
   // TextureID  = glGetUniformLocation(shaderProgramID, "myTextureSampler");
}

void initMatrices()// initialize camera's set (without interactive effect)
{

	Projection = glm::perspective(45.0f, 4.0f / 3.0f, 1.0f, 100.0f);
	View = glm::lookAt(
	                   glm::vec3(0, 0, 10),
					   glm::vec3(0,0,0),
					   glm::vec3(0,1,0)
	              	);
	Model = glm::mat4(1.0f);
	
}

void getInputMatrices()//get keyboard and mouse to interact with the object
{
	
	//*************** Compute the MVP matrix from keyboard and mouse input
	computeMatricesFromInputs();
	Projection = getProjectionMatrix();
	View = getViewMatrix();
	//Model = glm::mat4(1.0);
}


void initLab2()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	initMatrices();
	const vec4& colorTop = vec4 (1.0, 0.0, 0.0, 1.0);
	const vec4& colorBottom = vec4 (1.0, 1.0, 0.0, 1.0);
	cube = new Cylinder(3, 2.5, 2.5, colorTop, colorBottom, 2);
	cylinder = new Cylinder(1, 0.5, 0.5, colorTop, colorBottom, 16);
	Hand = new hand();
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	// Set up the shaders
	shaderProgramID = CompileShaders("../diffuse.vs", "../diffuse.ps");
	//GLuint cube_VBO;
	cube->generateObjectBuffer();
	cylinder->generateObjectBuffer();
	// Link the current buffer to the shader
	linkCurrentBuffertoShader(shaderProgramID);		
}

void initLab3()
{
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	initMatrices();
	Skeleton = new skeleton(modelMatrixID);
	Skeleton->traverse(Skeleton->root);
	target = new Target();
	target->targetInit();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 
	shaderProgramID = CompileShaders("../diffuse.vs", "../diffuse.ps");
	linkCurrentBuffertoShader(shaderProgramID);	
}

void initLab4()
{
	initMatrices();
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, 1024/2, 768/2);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
//	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);//The Vertex Array Object (a.k.a VAO) is a special type of object that encapsulates all the data that is associated with the vertex processor. 
	glBindVertexArray(VertexArrayID);
	// Read our .obj file
	//std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	Skeleton_Node** root_node = NULL;
	GLint* bone_ids = NULL;// array of bone IDs,an input attribute bone_ids[4], saying which bones affect the vertex
	int pointCount;
	//bool res = loadAssImp("suzanne.obj", indices, indexed_vertices, indexed_uvs, indexed_normals);
	bool res = loadAssImp(MESH_NAME, indices, indexed_vertices, indexed_uvs, indexed_normals, root_node);
	//GLuint Texture = loadDDS("uvmap.DDS");

	shaderProgramID = CompileShaders("../diffuse.vs", "../diffuse.ps");
	viewMatrixID = glGetUniformLocation(shaderProgramID, "mV");
	modelMatrixID = glGetUniformLocation(shaderProgramID, "mM");
	perspectiveMatrixID = glGetUniformLocation(shaderProgramID, "mP");
	//TextureID  = glGetUniformLocation(shaderProgramID, "myTextureSampler");
	LightID = glGetUniformLocation(shaderProgramID, "LightPosition_worldspace");

	//GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_vbo);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	////GLuint uvbuffer;
	//glGenBuffers(1, &uvbuffer_vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_vbo);
	//glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	//GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_vbo);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	//GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

	/*glGenBuffers (1, &boneId_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, boneId_vbo);
	glBufferData (GL_ARRAY_BUFFER, pointCount * sizeof (GLint), bone_ids, GL_STATIC_DRAW);*/

//	linkCurrentBuffertoShader(shaderProgramID);		

}

void renderHand(Node* bone)
{
	bool timeFlag = true;    
		if(timeFlag)
	{
		theta += 0.01;
		if (theta > 1)
		{
			timeFlag = false;
			theta = 0;
		}
	}
	if (timeFlag == false)
	{
		theta += -0.01;
		if (theta < -1)
		{
			timeFlag = true;
			theta=0;
		}
	}
	
	if(bone->m_nodeName == "Palm")
	{
		glm::mat4 nodeGlobal = bone->getGlobalTransform();
		//nodeGlobal =  glm::rotate(glm::mat4(1.0), theta, glm::vec3(1,0,0)) * nodeGlobal;
		bone->nodeLocalMatrix = nodeGlobal;
	    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &nodeGlobal[0][0]); 
		cube->draw();
	}
	else
	{
		glm::mat4 nodeGlobal = bone->getGlobalTransform();
	   glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &nodeGlobal[0][0]);
	   cylinder->draw();	 
	}
}

void displayLab2(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	getInputMatrices();
	 glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(perspectiveMatrixID, 1, GL_FALSE, &Projection[0][0]);

	std::list<Node*>::iterator it;
     for(it = Hand->allNodes.begin() ; it!=Hand->allNodes.end() ; it++)
     {
          renderHand((*it));
	 }
}

void displayLab3(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//getInputMatrices(); 
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(perspectiveMatrixID, 1, GL_FALSE, &Projection[0][0]);
	//target->getTargetSpineMove(modelMatrixID);
	target->putTarget(modelMatrixID, glm::vec3(4,-6,0));
	//target->getTargetLinearMove(modelMatrixID);
	 computeCCDIK(Skeleton, target, modelMatrixID);
	 std::list<Bone*>::iterator it;
     for(it = Skeleton->allBones.begin() ; it!=Skeleton->allBones.end() ; it++)
     {
         Skeleton->renderBones((*it)); 
	 } 
	 /* list<Bone*> fingers = Skeleton->getFingers(Skeleton);
	 std::list<Bone*>::iterator it2;
	 for(it2 = fingers.begin() ; it2!=fingers.end() ; it++)
	 {
	 ((*it2))->getFingerAnimation((*it2), modelMatrixID);
	 } */
	 Skeleton->initBones(Skeleton);
}

void displayLab4()
{
	// Ensure we can capture the escape key being pressed below

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	getInputMatrices();

	if (glfwGetKey(window,GLFW_KEY_R) == GLFW_PRESS)
	{
		Model = glm::rotate(Model, (float)45.0, glm::vec3(0, 0, 1));
	}

	glUniformMatrix4fv(perspectiveMatrixID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &View[0][0]);


	//here is the light model
	glm::vec3 lightPos = glm::vec3(4,4,4);
	glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);


	//GL state machine has a number of "active" texture slots. using 1 texture->active slot 0, 
	//bind the texture that we want to use into it before rendering:
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, Texture);//glBindTexture()函数绑定纹理对象
	//// Set our "myTextureSampler" sampler to user Texture Unit 0
	//glUniform1i(TextureID, 0);

	/*GLuint location1, loc2, loc3;
	location1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(shaderProgramID, "vertexUV");
	loc3 = glGetAttribLocation(shaderProgramID, "vertexNormal_modelspace");
*/

	// 1rst attribute buffer : vertices
	//glEnableVertexAttribArray(location1);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_vbo);
	glVertexAttribPointer(
		0,                  // attribute,Specifies the index of the generic vertex attribute to be modified.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// 2nd attribute buffer : UVs
	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_vbo);
	//glVertexAttribPointer(//define an array of generic vertex attribute data
	//	1,                                // attribute
	//	2,                                // size
	//	GL_FLOAT,                         // type
	//	GL_FALSE,                         // normalized?
	//	0,                                // stride
	//	(void*)0                          // array buffer offset
	//);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_vbo);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glDrawElements(// Draw the triangles !
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0           // element array buffer offset
		);

	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

}

int main(int argc, char** argv){

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "HelloWorld", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	//initialize object, get their vertices	
	//initLab2();
	//initLab3();
	initLab4();

	do
	{
        //displayLab2();
		//displayLab3();
		displayLab4();
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)// Check if the ESC key was pressed or the window was closed
		{
			glfwSetWindowShouldClose(window,GL_TRUE);
		}
		
	}while(!glfwWindowShouldClose(window));//ESC check shouldn't be done here, it can only catch this thing when while is executed
	

	glDeleteBuffers(1, &vertexbuffer_vbo);
	//glDeleteBuffers(1, &uvbuffer_vbo);
	glDeleteBuffers(1, &normalbuffer_vbo);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteProgram(shaderProgramID);
	//glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();
	return 0;

}











