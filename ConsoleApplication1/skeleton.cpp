#include "skeleton.h"
#include "hand.h"
#include "Node.h"
#include <common/cylinder.h>
#include <common/control.hpp>
#include "common/maths_funcs.h"
#include <Bone.h>

skeleton::skeleton(int ModelID)
{
	//rootPosition = glm::vec4(0, 0, 0, 1);
	const vec4& colorTop = vec4 (1.0, 1.0, 0.0, 1.0);
	const vec4& colorBottom = vec4 (1.0, 0.0, 0.0, 1.0);
	float armLength = 5;
	float palmLength = 2;
	float fingerlayerLength = 0.5;
	cylinderLab3 = new Cylinder(armLength, 0.5, 1, colorTop, colorBottom, 16);//for each part of arm, the length is 3
	cylinderLab3->generateObjectBuffer();
	cube = new Cylinder(palmLength, 1.5, 1.5, colorTop, colorBottom, 2);
	cube->generateObjectBuffer();
	body = new Cylinder(7, 3, 3, colorTop, colorBottom, 4);//for each part of arm, the length is 3
	body->generateObjectBuffer();
	cylinder = new Cylinder(fingerlayerLength, 0.25, 0.25, colorTop, colorBottom, 16);
	cylinder->generateObjectBuffer();
	skeletonLength = 0;

	ModelMatrixID = ModelID;
    root = new Bone("upperArm");//root
	root->boneLength = armLength;
	root->setAngleLimit(-45,-179,-179,10,179,179);
	allBones.push_back(root);
	skeletonLength = skeletonLength + root->boneLength;

	Bone* lowerArm = new Bone("lowerArm", root);
	setBonePosition(lowerArm, glm::vec3(0,lowerArm->m_parent->boneLength,0));
	lowerArm->boneLength = armLength;
	lowerArm->setAngleLimit(-45, -45,-45,45,45,45);
	allBones.push_back(lowerArm);
	skeletonLength = skeletonLength + lowerArm->boneLength;
	
	Bone* Palm = new Bone("Palm", lowerArm);
	setBonePosition(Palm, glm::vec3(0,Palm->m_parent->boneLength,0));
	allBones.push_back(Palm);
	Palm->boneLength = palmLength;
	Palm->setAngleLimit(0,0,-60,0,0,90);
	skeletonLength = skeletonLength + Palm->boneLength;

	Bone* firstFinger1 = new Bone("firstFinger1", Palm);
	setBonePosition(firstFinger1, glm::vec3(-1.15,2,0));
	allBones.push_back(firstFinger1); 
	firstFinger1->boneLength = fingerlayerLength;
	firstFinger1->setAngleLimit(0,0,-90,0,0,0);
	skeletonLength = skeletonLength + firstFinger1->boneLength;

	Bone* firstFinger2 = new Bone("firstFinger2", firstFinger1);
	setBonePosition(firstFinger2, glm::vec3(0, 0.7, 0));
	allBones.push_back(firstFinger2);
	firstFinger2->boneLength = fingerlayerLength;
	firstFinger2->setAngleLimit(0,0,-90,0,0,0);
	skeletonLength = skeletonLength + firstFinger2->boneLength;

	Bone* firstFinger3 = new Bone("firstFinger3", firstFinger2);
	setBonePosition(firstFinger3, glm::vec3(0, 0.7, 0));
	allBones.push_back(firstFinger3);
	firstFinger3->boneLength = fingerlayerLength;
	firstFinger3->setAngleLimit(0,0,-2,0,0,0);
	skeletonLength = skeletonLength + firstFinger3->boneLength;

	Bone* secondFinger1 = new Bone("secondFinger1", Palm);
	setBonePosition(secondFinger1, glm::vec3(-0.4,2,0));
	allBones.push_back(secondFinger1);
	secondFinger1->setAngleLimit(0,0,-90,0,0,0);
	secondFinger1->boneLength = fingerlayerLength;

	Bone* secondFinger2 = new Bone("secondFinger2", secondFinger1);
	setBonePosition(secondFinger2, glm::vec3(0, 0.7, 0));
	allBones.push_back(secondFinger2);
	secondFinger2->setAngleLimit(0,0,-90,0,0,0);
	secondFinger2->boneLength = fingerlayerLength;

	Bone* secondFinger3 = new Bone("secondFinger3", secondFinger2);
	setBonePosition(secondFinger3, glm::vec3(0, 0.7, 0));
	allBones.push_back(secondFinger3);
	secondFinger3->boneLength = fingerlayerLength;
	secondFinger3->setAngleLimit(0,0,-2,0,0,0);

	Bone* fourthFinger1 = new Bone("fourthFinger1", Palm);
	setBonePosition(fourthFinger1, glm::vec3(1.15,2,0));
	allBones.push_back(fourthFinger1);
	fourthFinger1->boneLength = fingerlayerLength;
	fourthFinger1->setAngleLimit(0,0,-90,0,0,0);

	Bone* fourthFinger2 = new Bone("fourthFinger2", fourthFinger1);
	setBonePosition(fourthFinger2, glm::vec3(0, 0.7, 0));
	allBones.push_back(fourthFinger2);
	fourthFinger2->boneLength = fingerlayerLength;
	fourthFinger2->setAngleLimit(0,0,-90,0,0,0);

	Bone* fourthFinger3 = new Bone("fourthFinger3", fourthFinger2);
	setBonePosition(fourthFinger3, glm::vec3(0, 0.7, 0));
	allBones.push_back(fourthFinger3);
	fourthFinger3->boneLength = fingerlayerLength;
	fourthFinger3->setAngleLimit(0,0,-2,0,0,0);

	Bone* thumb1 = new Bone("thumb1", Palm);
	setBonePosition(thumb1, glm::vec3(2,1,0));
	allBones.push_back(thumb1);
	thumb1->boneLength = fingerlayerLength;
	thumb1->setAngleLimit(-70,-160,0,0,10,90);

	Bone* thumb2 = new Bone("thumb2", thumb1);
	setBonePosition(thumb2, glm::vec3(0, 0.7, 0));
	allBones.push_back(thumb2);
	thumb2->boneLength = fingerlayerLength;
	thumb1->setAngleLimit(0,0,0,0,0,90);

	Bone* thirdFinger1 = new Bone("thirdFinger1", Palm);
	setBonePosition(thirdFinger1, glm::vec3(0.4,2,0));
	allBones.push_back(thirdFinger1);
	thirdFinger1->boneLength = fingerlayerLength;
	thirdFinger1->setAngleLimit(0,0,-90,0,0,0);

	Bone* thirdFinger2 = new Bone("thirdFinger2", thirdFinger1);
	setBonePosition(thirdFinger2, glm::vec3(0, 0.7, 0));
	allBones.push_back(thirdFinger2);
	thirdFinger2->boneLength = fingerlayerLength;
	thirdFinger2->setAngleLimit(0,0,-90,0,0,0);

	Bone* thirdFinger3 = new Bone("thirdFinger3", thirdFinger2);
	setBonePosition(thirdFinger3, glm::vec3(0, 0.7, 0));
	allBones.push_back(thirdFinger3);
	thirdFinger3->boneLength = fingerlayerLength;
	thirdFinger3->setAngleLimit(0,0,-2,0,0,0);

	/*Bone* lower2Arm = new Bone("lower2Arm", lowerArm);
	setBonePosition(lower2Arm, glm::vec3(0,lower2Arm->m_parent->boneLength,0));
	lower2Arm->boneLength = armLength;
	allBones.push_back(lower2Arm);
	skeletonLength = skeletonLength + lower2Arm->boneLength;
	lower2Arm->setAngleLimit(0,0,0,90,0,0);*/

	/*Bone* lower3Arm = new Bone("lower3Arm", lower2Arm);
	setBonePosition(lower3Arm, glm::vec3(0,3,0));
	allBones.push_back(lower3Arm);*/
}


skeleton::~skeleton(void)
{
}

void skeleton::traverse(Bone* bone)
{
	if(!bone->m_listChildren.empty())
	{
		std::list<Bone*>::iterator it;
		for(it = bone->m_listChildren.begin() ; it!=bone->m_listChildren.end() ; it++)
     {
          traverse((*it));
		  if ((*it)->m_boneName=="Palm")
		  {
			  leafBones.push_back(*it);
		  }
		//  leafBones.push_back(*it);
	 }
	}else
	{
		leafBones.push_back(bone);
	}
	return;
}

void skeleton::setBonePosition(Bone* bone, glm::vec3(BonePos))
{
	bone->getTranslation(BonePos);
}

void skeleton::renderBones(Bone* bone)
{
	if(bone->m_parent == nullptr)//root
	{
		glm::mat4 BoneGlobal = bone->getGlobalTransform();
		bone->BoneLocalMatrix = BoneGlobal;
	    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &BoneGlobal[0][0]); 
		cylinderLab3->draw();
		glm::mat4 temp = glm::rotate(glm::mat4(1.0), (float)45.0, glm::vec3(0,1,0));
		temp = glm::translate(temp, glm::vec3(-1.5,-6, -1.5));
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &temp[0][0]); 
		body->draw();
	}
	else
	{
		glm::mat4 BoneGlobal = bone->getGlobalTransform();
	   glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &BoneGlobal[0][0]);
	   if (bone->m_boneName == "Palm")
	   {
		   cube->draw();
	   }
	   else if (bone->m_boneName == "lowerArm" || bone->m_boneName == "upperArm")
	   {
		  cylinderLab3->draw();
	   }
	   else
	   {
		  cylinder->draw(); 
		  // cylinderLab3->draw();
	   }
	   	 
	}
}

list<Bone*> skeleton::getFingers(skeleton* Skeleton)
{
	list<Bone*> fingers;
	std::list<Bone*>::iterator it;
	for(it = Skeleton->allBones.begin() ; it!= Skeleton->allBones.end() ; it++)
	{  
		if ((*it)->m_parent->m_boneName == "Palm")
		{
			fingers.push_back((*it));
		}
	}
	return fingers;
}

void skeleton::initBones(skeleton* Skeleton)
{
	std::list<Bone*>::iterator it;
	for(it = Skeleton->allBones.begin() ; it!= Skeleton->allBones.end() ; it++)
	{  
		(*it)->BoneLocalMatrix = (*it)->BoneInitLocalMatrix;
		(*it)->BoneGlobalMatrix = (*it)->getGlobalTransform();
		(*it)->xangle = 0;
		(*it)->yangle = 0;
		(*it)->zangle = 0;

	}
}

