#include <Bone.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#define IK_POS_THRESH 0.01f
//if you want more accuracy, put this value larger, but don't make it too big when doing a animation/..
#define MAX_IK_TRIES 2
#define PI 3.14f


float AngleToDegree(float angle)
{
	float angleDegree = angle * 360 /( 2 * PI);
	return angleDegree;
}

float AngleToRadian(float angle)
{
	float angleRadian = PI/180 * angle;
	return angleRadian;
}

list<Bone*> getLink(Bone* bone)
{
	list<Bone*> BoneLink;
	BoneLink.push_back(bone);//bone is the first element in the link
	while(bone->m_parent!=nullptr)
	{
		BoneLink.push_back(bone->m_parent);
		bone = bone->m_parent;
	}
	return BoneLink;
}

void reachableDetect(skeleton* Skeleton, glm::vec4 targetPosition)
{
	Bone* rootBone = Skeleton->root;
	glm::vec4 jointPosition = rootBone->getJoint();
	float jointTargetDistance = glm::distance(jointPosition, targetPosition);
	//if(jointTargetDistance > Skeleton->allBones.size()*5)
	if(jointTargetDistance > Skeleton->skeletonLength)
	{
		//compute root angle
		//rootBone->BoneLocalMatrix = glm::mat4(1.0);
		glm::vec4 endPosition = rootBone->getEnd();
		glm::vec3 jointToEnd;
		jointToEnd[0] = endPosition[0] - jointPosition[0];//x
		jointToEnd[1] = endPosition[1] - jointPosition[1];//y
		jointToEnd[2] = endPosition[2] - jointPosition[2];//z
		jointToEnd = glm::normalize(jointToEnd);
		glm::vec3 targetToJoint;
		targetToJoint[0] = targetPosition[0] - jointPosition[0];//x
		targetToJoint[1] = targetPosition[1] - jointPosition[1];//y
		targetToJoint[2] = targetPosition[2] - jointPosition[2];//z
		targetToJoint = glm::normalize(targetToJoint);
		float cosAngle = glm::dot(jointToEnd, targetToJoint);
		if(cosAngle < 0.9999)//cosAngle!=1,no consideration about sinAngle = 0,angle=0,180,
		{
			glm::vec3 crossResult = glm::cross(jointToEnd, targetToJoint);
			crossResult = glm::normalize(crossResult);

			float angle = glm::acos(cosAngle);
			float angleDe = glm::degrees(angle);
			//glm::mat4 rot = glm::toMat4(glm::angleAxis(angleDe, crossResult));
			glm::quat quaternion = glm::angleAxis(angleDe, crossResult);
			glm::vec3 angles = glm::eulerAngles(quaternion);
			angles = rootBone->set3DOF(angles);
			glm::mat4 rot = glm::toMat4(glm::quat(glm::radians(angles)));
			rootBone->BoneLocalMatrix *= rot;
		}
		std::list<Bone*>::iterator it;
		for(it = Skeleton->allBones.begin() ; it!= Skeleton->allBones.end() ; it++)
		{  
			if((*it)!= rootBone)
			{
				(*it)->BoneLocalMatrix = (*it)->BoneInitLocalMatrix;
				(*it)->BoneGlobalMatrix = (*it)->getGlobalTransform();
			}

		}
	}
}

//void distanceDetect()
//{
//	float endTargetDistance = glm::distance(endPosition, targetPosition);
//}

void computeCCDIK(skeleton* Skeleton, Target* target, int modelMatrixID){
	int iterationCount = 0;
	float cosAngle, angle;
	Bone* lastBone = Skeleton->leafBones.back();
	glm::vec4 targetPosition = target->getTargetPosition();
	//get joint1 position, endEffect1 position
	glm::vec4 endPosition = lastBone->getEnd();
	//glm::vec4 endPosition;
	glm::vec4 jointPosition;
	//get joint2 position(root joint), endEffect2 position

	//distanceDetect(Skeleton,targetPosition);

	float endTargetDistance = glm::distance(endPosition, targetPosition);
	float TargetDistance = glm::distance(glm::vec4(0,0,0,1), targetPosition);

	while (iterationCount++ < MAX_IK_TRIES && endTargetDistance > IK_POS_THRESH)
	{
		list<Bone*> BoneLink = getLink(lastBone);
		while(!BoneLink.empty()){
			Bone* currentBone = BoneLink.front();
			jointPosition = currentBone->getJoint();
			glm::vec3 jointToEnd;
			jointToEnd[0] = endPosition[0] - jointPosition[0];//x
			jointToEnd[1] = endPosition[1] - jointPosition[1];//y
			jointToEnd[2] = endPosition[2] - jointPosition[2];//z
			jointToEnd = glm::normalize(jointToEnd);

			glm::vec3 targetToJoint;
			targetToJoint[0] = targetPosition[0] - jointPosition[0];//x
			targetToJoint[1] = targetPosition[1] - jointPosition[1];//y
			targetToJoint[2] = targetPosition[2] - jointPosition[2];//z
			targetToJoint = glm::normalize(targetToJoint);
		
			cosAngle = glm::dot(jointToEnd, targetToJoint);
			//This is 2D CCD
			//if(cosAngle < 0.9999)//cosAngle!=1,no consideration about sinAngle = 0,angle=0,180,
			//{
			//	glm::vec3 crossResult = glm::cross(jointToEnd, targetToJoint);
			//	if(crossResult[2] >= 0) //crossResult.z > 0, rotate clockwise
			//	{
			//		angle = AngleToDegree(acos(cosAngle)); //angle is a degree number
			//	{
			//		angle = AngleToDegree(acos(cosAngle));
			//			currentBone->BoneLocalMatrix = currentBone->calculateLocalMatrix(0, 0, -angle);
			//	}
			//	//update endPosition
			//	endPosition = lastBone->getEnd();
			//}
			if(cosAngle < 0.9999)//cosAngle!=1,no consideration about sinAngle = 0,angle=0,180,
			{
				glm::vec3 crossResult = glm::cross(jointToEnd, targetToJoint);
				crossResult = glm::normalize(crossResult);
			    angle = glm::acos(cosAngle);
				float angleDe = glm::degrees(angle);
				//glm::mat4 rot = glm::toMat4(glm::angleAxis(angleDe, crossResult));
				glm::quat quaternion = glm::angleAxis(angleDe, crossResult);
				glm::vec3 angles = glm::eulerAngles(quaternion);
				angles = currentBone->set3DOF(angles);
				glm::mat4 rot = glm::toMat4(glm::quat(glm::radians(angles)));
				currentBone->BoneLocalMatrix*= rot;
				endPosition = lastBone->getEnd();
			}
			BoneLink.pop_front();
		}
		endTargetDistance = glm::distance(endPosition, targetPosition);
	}
	reachableDetect(Skeleton,targetPosition);
}
