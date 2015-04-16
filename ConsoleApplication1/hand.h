#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <common/cylinder.h>
#include <string> 
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include <Node.h>

class hand
{
public:
	hand();
	~hand(void);

	Node* Palm;
	void hand::traverse(Node* bone);
	GLfloat theta;
	list<Node*> allNodes;
	//void hand::renderHand(Node* bone);
};

