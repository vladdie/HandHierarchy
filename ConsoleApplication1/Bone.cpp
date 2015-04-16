#include "Bone.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#define PI 3.14f

//Bone::Bone(string boneName)
Bone::Bone(string boneName)
{
    m_boneName = boneName;
	m_parent = nullptr;
	m_listChildren.empty();
	BoneLocalMatrix = glm::mat4(1.0);
	BoneInitLocalMatrix = glm::mat4(1.0);
	xangle = 0;
	yangle = 0;
	zangle = 0;
	/*xangleMax = 90;
	yangleMax = 90;
	zangleMax = 90;
	xangleMin = -10;
	yangleMin = -90;
	zangleMin = -10;*/
	jointPosition = getJoint();
	endPosition = getEnd();
	
}

//Bone::Bone(string boneName, Bone* parent)
Bone::Bone(string boneName, Bone* parent)
{
   m_boneName = boneName;
	m_parent = parent;
	m_listChildren.empty();
	m_parent->m_listChildren.push_front(this);
	BoneLocalMatrix = glm::mat4(1.0);
	BoneInitLocalMatrix = glm::mat4(1.0);
	xangle = 0;
	yangle = 0;
	zangle = 0;
	/*xangleMax = 70.0f;
	yangleMax = 5.0f;
	zangleMax = 5.0f;*/
	jointPosition = getJoint();
	endPosition = getEnd();
}
 


Bone::~Bone(void)
{
}

void Bone::setAngleLimit(float xmin, float ymin, float zmin,float xmax, float ymax, float zmax)
{
	xangleMax = xmax;
	yangleMax = ymax;
	zangleMax = zmax;
	xangleMin = xmin;
	yangleMin = ymin;
	zangleMin = zmin;
}

glm::vec3 Bone::setDOF(glm::vec3 angles)
{
	float x = angles[0];
	float y = angles[1];
	float z = angles[2];

	if (xangle == xangleMax)
	{
		x = 0;
	}
	if (yangle == yangleMax)
	{
		y = 0;
	}
	if (zangle == zangleMax)
	{
		z = 0;
	}
	if((x + xangle) > xangleMax )
	{
		x = xangleMax - xangle;
		xangle = xangleMax;
	} 
	if((y + yangle) > yangleMax )
	{
		y = yangleMax - yangle;
		yangle = yangleMax;
	} 
	if((z + zangle) > zangleMax )
	{
		z = zangleMax - zangle;
		zangle = zangleMax;
	} 
	angles[0] = glm::radians(x);
	angles[1] = glm::radians(y);
	angles[2] = glm::radians(z);
	
	return angles;
}

glm::mat4 Bone::getRotation(float x, float y, float z)
{
	//setDOF(glm::vec3(x,y,z));
	BoneLocalMatrix = glm::rotate(BoneLocalMatrix, xangle, glm::vec3(1, 0, 0));
	BoneLocalMatrix = glm::rotate(BoneLocalMatrix, yangle, glm::vec3(0, 1, 0));
	BoneLocalMatrix = glm::rotate(BoneLocalMatrix, zangle, glm::vec3(0, 0, 1));
	return BoneLocalMatrix;
}

glm::mat4 Bone::getTranslation(glm::vec3 translation)
{
   BoneLocalMatrix = glm::translate(BoneLocalMatrix,translation);
   BoneInitLocalMatrix = glm::translate(BoneInitLocalMatrix,translation);
   return BoneLocalMatrix;
}

glm::mat4 Bone::calculateLocalMatrix(float x, float y, float z)
{
	if(m_parent != nullptr)//not the root
	{
		BoneLocalMatrix = getRotation(x, y, z);
	}else //this is the root local matrix, only rotation, no translation
	{
		BoneLocalMatrix = getRotation(x, y, z);//root bone only can rotate
	}
	return BoneLocalMatrix;
}


glm::vec4 Bone::getJoint()
{
	jointPosition = glm::vec4(0,0,0,1);
	jointPosition = this->getGlobalTransform() * jointPosition;
	return jointPosition;
}

glm::vec4 Bone::getEnd()
{
	endPosition = glm::vec4(0,this->boneLength,0,1);
	endPosition = this->getGlobalTransform() * endPosition;
	return endPosition;
}

glm::mat4 Bone::getGlobalTransform()
{
	//if no parent node, Local matrix is the global matrix
	if(m_parent == nullptr)//root bone
	{
		BoneGlobalMatrix = BoneLocalMatrix;
		return BoneGlobalMatrix;
	}
	else{
	    BoneGlobalMatrix = m_parent->getGlobalTransform() *  BoneLocalMatrix;	
	    return BoneGlobalMatrix;
	}
}

void Bone::drawBone(Cylinder* cylinder)
{
		if(this->m_boneName == "upperArm")//root
	{
		glm::mat4 BoneGlobal = this->BoneLocalMatrix;
	    glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &BoneGlobal[0][0]); 
		cylinder->draw();
	}
	else
	{
		glm::mat4 BoneGlobal = this->getGlobalTransform();
	   glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &BoneGlobal[0][0]);
	   cylinder->draw();	 
	}
}

void Bone::getFingerAnimation(Bone* bone, int ModelMatrixID )
{
	bool timeFlag = true;  
	float thetatemp = 0;
	if(timeFlag)
	{
		thetatemp += 0.1;
		if (thetatemp > 1)
		{
			timeFlag = false;
			thetatemp = 0;
		}
	}
	if (timeFlag == false)
	{
		thetatemp += -0.1;
		if (thetatemp < -1)
		{
			timeFlag = true;
			thetatemp=0;
		}
	}
	glm::mat4 BoneGlobal =  glm::rotate(bone->BoneGlobalMatrix, thetatemp, glm::vec3(0,0,1));
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &BoneGlobal[0][0]);
}

glm::vec3 Bone::set3DOF(glm::vec3 angles )
{
	bool xModified = false;
	bool yModified = false;
	bool zModified = false; 
	if (angles.x > 0 && xangle == xangleMax)
	{
		angles.x = 0;
		xModified = true;
	}
	if (angles.y > 0 && yangle == yangleMax)
	{
		angles.y = 0;
		yModified = true;
	}
	if (angles.z > 0 && zangle == zangleMax)
	{
		angles.z = 0;
		zModified = true;
	}

	if (angles.x < 0 && xangle == xangleMin)
	{
		angles.x = 0;
		xModified = true;
	}
	if (angles.y < 0 && yangle == yangleMin)
	{
		angles.y = 0;
		yModified = true;
	}
	if (angles.z < 0 && zangle == zangleMin)
	{
		angles.z = 0;
		zModified = true;
	}

	if((angles.x + xangle) <= xangleMax && (angles.x + xangle) >= xangleMin && xModified == false)
	{
		xangle = xangle +  angles.x;
	}
	else
	{
		bool swicher = false;
		if ((angles.x + xangle) > xangleMax && xModified == false)
		{
			angles.x = xangleMax - xangle;
			xangle = xangleMax;
			xModified = true;
		}

		if ((angles.x + xangle) < xangleMin && xModified == false)
		{
			angles.x = xangleMin - xangle;
			xangle = xangleMin;
			xModified = true;
		}
	}

	if((angles.y + yangle) <= yangleMax && (angles.y + yangle) >= yangleMin && yModified == false)
	{
		yangle = yangle + angles.y;
	}
	else
	{
		if ((angles.y + yangle) > yangleMax && xModified == false)
		{
			angles.y = yangleMax - yangle;
			yangle = yangleMax;
			yModified = true;
		}


		if ((angles.y + yangle) < yangleMin && yModified == false)
		{
			angles.y = yangleMin - yangle;
			yangle = yangleMin;
		}
	}

	if((angles.z + zangle) <= zangleMax && (angles.z + zangle) >= zangleMin && zModified == false)
	{
		zangle = zangle + angles.z;
	}
	else
	{
		if ((angles.z + zangle) > zangleMax && xModified == false)
		{
			angles.z = zangleMax - zangle;
			zangle = zangleMax;
			zModified = true;
		}

		if ((angles.z + zangle) < zangleMin && zModified == false)
		{
			angles.z = zangleMin - zangle;
			zangle = zangleMin;
		}
	}
	return angles;
}