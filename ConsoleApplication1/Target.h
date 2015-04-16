#pragma once
#include <glm/detail/type_vec3.hpp>
#include <glm.hpp>

class Target
{
public:
	Target(void);
	~Target(void);
	glm::vec4 targetPosition;
	void targetInit();
	//glm::vec3 interpolateLine(float counter, glm::vec3 startpoint, glm::vec3 targetPoint);
	glm::vec3 interpolateLine(float counter, glm::vec3 startpoint, glm::vec3 targetPoint);
	glm::vec3 interpolateCubicSpine(float counter, glm::vec3 startpoint,  glm::vec3 point1,  glm::vec3 point2, glm::vec3 targetPoint);
	void getTargetLinearMove(int ModelMatrixID);
	void getTargetSpineMove(int ModelMatrixID);
	void Target::putTarget(int ModelMatrixID, glm::vec3 putPosition);
	glm::vec4 getTargetPosition();

private:
	glm::vec3 translateVector;
};

