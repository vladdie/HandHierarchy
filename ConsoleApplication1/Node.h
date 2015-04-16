#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GL/glew.h>
//#include <common/cylinder.h>
#include <string> 
#include <list>
using namespace std;


class Node
{
public:
	Node(string m_nodeName);
	Node(string m_nodeName, Node* m_parent);
	~Node(void);

	string m_nodeName;
	Node* m_parent;
	list<Node*> m_listChildren;
	float count;
	glm::mat4 rotation;
	glm::mat4 nodeLocalMatrix;
	glm::mat4 nodeGlobalMatrix;
	glm::mat4 getGlobalTransform();
	glm::mat4 getLocalMatrix();
	float tagFinger;
	float tagLayer;
};

