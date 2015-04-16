
#include <GL/glew.h>
//#include <GL/freeglut.h>

#include <string> 
#include <fstream>
#include <iostream>
#include <sstream>

//include GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

// Include GLFW
#include <GLFW/glfw3.h>
#include "common/maths_funcs.h"
#include "common/cylinder.h"
#include <skeleton.h>
#include <Node.h>
GLFWwindow* window;

#include <common/control.hpp>
#include <common/cylinder.h>
#include <common/vboIndexer.hpp>

//using namespace glm;
using namespace std;

// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

//IDs of varibles mP, mV, mM in the shader
GLuint perspectiveMatrixID, viewMatrixID, modelMatrixID;
GLfloat theta;
GLuint shaderProgramID;
glm::mat4 Projection, View, Model;
glm::mat4 rotXMatrix, rotYMatrix, rotZMatrix;
GLuint vao;
Cylinder* cube;
Cylinder* cylinder;

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
	//// Have to enable this
	//glEnableVertexAttribArray(positionID);
	//// Tell it where to find the position data in the currently active buffer (at index positionID)
	////tell the variables where I can find the info it's looking for at the currently active
	//glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void initMatrices()// initialize camera's set (without interactive effect)
{
	Projection = glm::perspective(45.0f, 4.0f / 3.0f, 1.0f, 100.0f);//vertices closer than 1 and further than 100 won't be visible
	View = glm::lookAt(
	                   glm::vec3(0,0,-20),
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
	Model = glm::mat4(1.0);

	//////***********rotate by itself*****************************//
	//theta += 0.1f;
	////recover model matrix so that it won't rotate faster and faster
	//Model = glm::mat4(1.0);
	//Model=glm::rotate(Model, theta, glm::vec3(1,0,0));
	//Model=glm::rotate(Model, theta, glm::vec3(0,1,0));
	//Model=glm::rotate(Model, theta, glm::vec3(0,0,1));	
}


void init()
{
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	
	initMatrices();
	//set up base size
	const float height = 1.0;
	const float radiusTop = 2.0;
	const float radiusBottom = 2.0;
	const vec4& colorTop = vec4 (1.0, 0.0, 0.0, 1.0);
	const vec4& colorBottom = vec4 (1.0, 1.0, 0.0, 1.0);
	//const int segments = 4;
	cube = new Cylinder(height, radiusTop, radiusTop, colorTop, colorBottom, 4);
	cylinder = new Cylinder(2.0, 0.2, 0.2, colorTop, colorBottom, 16);
	
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

void firstVision()
{
	//rotate the base(in y-axis)->get the base Model Matrix
	theta += 0.4f;
	glm::mat4 baseModel = glm::rotate(Model, theta, glm::vec3(0,1,0)); 
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &baseModel[0][0]);
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(perspectiveMatrixID, 1, GL_FALSE, &Projection[0][0]);
	cube->draw();

	//rotate the child1, self rotation is z-axis, follows base rotation
	glm::mat4 child1Local = glm::mat4(1.0);
	//translate from its parent
	child1Local = glm::translate(child1Local, glm::vec3(0,1,0));//1: height of the parent(cube)
	child1Local = glm::rotate(child1Local, theta, glm::vec3(0,0,1));
	glm::mat4 child1Global = baseModel * child1Local; //baseModel is global transform of child1's parent
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &child1Global[0][0]);
	cylinder->draw();

	//rotate the child2, self rotation is z-axis, follows base&child1 rotation
	glm::mat4 child2Local = glm::translate(Model, glm::vec3(0,2,0));//2: height of the parent
	child2Local = glm::rotate(child2Local, theta, glm::vec3(0,0,1));
	glm::mat4 child2Global = child1Global * child2Local;
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &child2Global[0][0]);
	cylinder->draw();
	//rotate the child3, self rotation is y-axis, follows base&child1&child2 rotation
	//translate from its parent
	glm::mat4 child3Local = glm::translate(Model, glm::vec3(0,2,0));//2: height of the parent
	child3Local = glm::rotate(child3Local, theta, glm::vec3(1,0,0));
	glm::mat4 child3Global =child2Global * child3Local;
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &child3Global[0][0]);
	cylinder->draw();
}

void secondVision()
{
	string baseName, childName;
	Node* base = new Node(baseName, Model);
	Node* child1 = new Node(childName, base);
	skeleton* Skeleton = new skeleton();
	Skeleton->addBaseBone(base, cube, modelMatrixID, viewMatrixID, perspectiveMatrixID);
	

}

void display(){

	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//Pass matrices to shader variable
	//getInputMatrices();

	firstVision();
	//secondVision();

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
	init();
	
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	do
	{
		// Set up your objects and shaders
		display();
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)// Check if the ESC key was pressed or the window was closed
		{
			glfwSetWindowShouldClose(window,GL_TRUE);
		}
		

	}while(!glfwWindowShouldClose(window));//ESC check shouldn't be done here, it can only catch this thing when while is executed
	
	glfwTerminate();
	return 0;

}











