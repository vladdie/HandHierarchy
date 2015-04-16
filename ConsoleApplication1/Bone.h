#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <common/cylinder.h>
#include <string> 
#include <list>
using namespace std;

class Bone
{
	public:
	//Bone(string m_boneName);
	Bone(string boneName);
	Bone::Bone(string boneName, Bone* m_parent);
	//Bone(string boneName, Bone* m_parent);
	~Bone(void);

	string m_boneName;
	float yangle, xangle, zangle;
	float xangleMax, yangleMax, zangleMax;
	float xangleMin, yangleMin, zangleMin;
	float boneLength;
	Bone* m_parent;
	list<Bone*> m_listChildren;
	glm::vec4 jointPosition;
	glm::vec4 endPosition;
	void setAngleLimit(float xmin, float ymin, float zmin,float xmax, float ymax, float zmax);

	glm::mat4 BoneLocalMatrix;
	glm::mat4 BoneGlobalMatrix;
	glm::mat4 BoneInitLocalMatrix;
	glm::mat4 getGlobalTransform();
	glm::mat4 calculateLocalMatrix(float x, float y, float z);
	glm::vec4 getJoint();
	glm::vec4 getEnd();
	void getFingerAnimation(Bone* bone, int ModelMatrixID );

    
	glm::mat4 getRotation(float x, float y, float z);
	glm::mat4 getTranslation(glm::vec3 translation);
	void drawBone(Cylinder* cylinder);
	int modelMatrixID;


	glm::vec3 setDOF(glm::vec3 angles);
	glm::vec3 set3DOF(glm::vec3 angles );
};

