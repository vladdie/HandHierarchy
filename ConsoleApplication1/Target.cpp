#include "Target.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <common/cylinder.h>
#include <GLFW/glfw3.h>
#include <common/control.hpp>
extern GLFWwindow* window;

float counter = 0;

glm::vec3 beginPoint = glm::vec3(-5,-3,6);//this is where the target start
glm::vec3 middlePoint1 = glm::vec3(-4,18,4); 
glm::vec3 middlePoint2 = glm::vec3(6,25,2); 
glm::vec3 EndPoint = glm::vec3(8,0,0); //target destination
glm::vec3 Position = glm::vec3(0,0,0); 
Cylinder* targetCube;

Target::Target(void)
{
}


Target::~Target(void)
{
}

 void Target::targetInit()
{
	//target is a cube, height 0.5, radius 0.5
	const vec4& colorTop = vec4 (1.0, 0.0, 0.0, 1.0);
	const vec4& colorBottom = vec4 (1.0, 0.0, 0.0, 1.0);
    targetCube = new Cylinder(0.5, 0.5, 0.5, colorTop, colorBottom, 4);
	targetCube->generateObjectBuffer();
}

 glm::vec3 Target::interpolateLine(float counter, glm::vec3 startpoint, glm::vec3 targetPoint)
{
	translateVector = startpoint*(1-counter)+counter*targetPoint;
	return translateVector; 
}

  glm::vec3 Target::interpolateCubicSpine(float counter, glm::vec3 startpoint, glm::vec3 point1,  glm::vec3 point2, glm::vec3 targetPoint)
{
	//here get the spine formula
	float countSpine = counter * counter;
	glm::vec3 tem1 = targetPoint - point2 - startpoint + point1;
	glm::vec3 tem2 = startpoint - point1 - tem1;
	glm::vec3 tem3 = point2 - startpoint;
  // translateVector = tem1 * counter * countSpine + tem2 * countSpine + tem3 * counter + point1;
	//bezier curve:
	translateVector = pow(1-counter,3)*startpoint+3*counter*pow(1-counter,2)*point1+3*pow(counter,2)*(1-counter)*point2+pow(counter,3)*targetPoint;
   return translateVector; 
}


  void Target::getTargetLinearMove(int ModelMatrixID)
{
	counter+=0.005;
	if(counter >=1)
	{
		counter = 0;
	}
	//glm::mat4 targetModelMatrix = glm::translate(glm::mat4(1.0), beginPoint);//put target in original position
	glm::mat4 targetModelMatrix = glm::mat4(1.0);
	glm::vec3 tempLine = interpolateLine(counter, beginPoint, EndPoint);
	targetModelMatrix = glm::translate(targetModelMatrix, tempLine);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &targetModelMatrix[0][0]);
	targetCube->draw();
	targetPosition = targetModelMatrix * glm::vec4(0, 0, 0, 1);
}

  void Target::getTargetSpineMove(int ModelMatrixID)
{
	counter+=0.005;
	if(counter >=1)
	{
		counter = 0;
	}
	//glm::mat4 targetModelMatrix = glm::translate(glm::mat4(1.0), beginPoint);//put target in original position
	glm::mat4 targetModelMatrix = glm::mat4(1.0);//put target in original position
	glm::vec3 tempLine = interpolateCubicSpine(counter, beginPoint, middlePoint1, middlePoint2, EndPoint);
	targetModelMatrix = glm::translate(targetModelMatrix, tempLine);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &targetModelMatrix[0][0]);
	targetCube->draw();
	targetPosition = targetModelMatrix * glm::vec4(0, 0, 0, 1);
}

void Target::putTarget(int ModelMatrixID, glm::vec3 putPosition)
{
	if(Position == glm::vec3(0,0,0))
	{
		Position = putPosition;
	}
	if (glfwGetKey( window, GLFW_KEY_X ) == GLFW_PRESS){
		Position[0] += 0.05;
	}
	if (glfwGetKey( window, GLFW_KEY_Y ) == GLFW_PRESS){
		Position[1] += 0.05;
	}
	if (glfwGetKey( window, GLFW_KEY_Z ) == GLFW_PRESS){
		Position[2] += 0.05;
	}
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		Position[0] -= 0.05;
	}
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		Position[1] -= 0.05;
	}
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		Position[2] -= 0.05;
	}
	glm::mat4 targetModelMatrix = glm::translate(glm::mat4(1.0), Position);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &targetModelMatrix[0][0]);
	targetCube->draw();
	targetPosition = targetModelMatrix * glm::vec4(0, 0, 0, 1);
}

  glm::vec4 Target::getTargetPosition()
{
	return targetPosition;
}
  
  
 