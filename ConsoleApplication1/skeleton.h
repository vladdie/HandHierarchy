#pragma once
#include <Bone.h>
#include <common/cylinder.h>

class skeleton
{
public:
	skeleton(int ModelID);
	~skeleton(void);

	Bone* root;
	void skeleton::traverse(Bone* bone);
	void skeleton::renderBones(Bone* bone);
	list<Bone*> allBones;
	list<Bone*> leafBones;
	list<glm::vec4> jointLink;
	int ModelMatrixID;
	float skeletonLength;
	Cylinder* cylinderLab3;
	Cylinder* cube;
	Cylinder* body;
	Cylinder* cylinder;
	glm::vec4 rootPosition;
	list<Bone*> getFingers(skeleton* Skeleton);
	void initBones(skeleton* Skeleton);
	void setBonePosition(Bone* bone,  glm::vec3(BonePos));
};

