#include "hand.h"
#include "Node.h"
#include <common/cylinder.h>
#include <common/control.hpp>
#include "common/maths_funcs.h"
hand::hand()
{
    Palm = new Node("Palm");//root
	Palm->tagFinger = 0;
	Palm->tagLayer = 0;
	allNodes.push_back(Palm); 

	Node* thumb1 = new Node("thumb1", Palm);
	allNodes.push_back(thumb1); 
	thumb1->tagFinger = 5;
	thumb1->tagLayer = 4;
	Node* thumb2 = new Node("thumb2", thumb1);
	allNodes.push_back(thumb2); 
	thumb2->tagFinger = 5;
	thumb2->tagLayer = 4;
	
	Node* firstFinger1 = new Node("firstFinger1", Palm);
	allNodes.push_back(firstFinger1); 
	firstFinger1->tagFinger = 1;
	firstFinger1->tagLayer = 1;
	Node* secondFinger1 = new Node("secondFinger1", Palm);
	allNodes.push_back(secondFinger1);
	secondFinger1->tagFinger = 2;
	secondFinger1->tagLayer = 1;
	Node* thirdFinger1 = new Node("thirdFinger1", Palm);
	allNodes.push_back(thirdFinger1);
	thirdFinger1->tagFinger = 3;
	thirdFinger1->tagLayer = 1;
	Node* fourthFinger1 = new Node("fourthFinger1", Palm);
	allNodes.push_back(fourthFinger1);
	fourthFinger1->tagFinger = 4;
	fourthFinger1->tagLayer = 1;

	Node* firstFinger2 = new Node("firstFinger2", firstFinger1);
	allNodes.push_back(firstFinger2);
	firstFinger2->tagFinger = 1;
	firstFinger2->tagLayer = 2;
	Node* secondFinger2 = new Node("secondFinger2", secondFinger1);
	allNodes.push_back(secondFinger2 );
	secondFinger2->tagFinger = 2;
	secondFinger2->tagLayer = 2;
	Node* thirdFinger2 = new Node("thirdFinger2", thirdFinger1);
	allNodes.push_back(thirdFinger2);
	thirdFinger2->tagFinger = 3;
	thirdFinger2->tagLayer = 2;
	Node* fourthFinger2 = new Node("fourthFinger2", fourthFinger1);
	allNodes.push_back(fourthFinger2);
	fourthFinger2->tagFinger = 4;
	fourthFinger2->tagLayer = 2;

	Node* firstFinger3 = new Node("firstFinger3", firstFinger2);
	allNodes.push_back(firstFinger3);
	firstFinger3->tagFinger = 1;
	firstFinger3->tagLayer = 3;
	Node* secondFinger3 = new Node("secondFinger3", secondFinger2);
	allNodes.push_back(secondFinger3);
	secondFinger3->tagFinger = 2;
	secondFinger3->tagLayer = 3;
	Node* thirdFinger3 = new Node("thirdFinger3", thirdFinger2);
	allNodes.push_back(thirdFinger3);
	thirdFinger3->tagFinger = 3;
	thirdFinger3->tagLayer = 3;
	Node* fourthFinger3 = new Node("fourthFinger3", fourthFinger2);
	allNodes.push_back(fourthFinger3);
	fourthFinger3->tagFinger = 4;
	fourthFinger3->tagLayer = 3;
	
	
}


hand::~hand(void)
{
}



void hand::traverse(Node* root)
{
	std::list<Node*>::iterator it;
	if(!root->m_listChildren.empty())
	{
		for(it = root->m_listChildren.begin() ; it!=root->m_listChildren.end() ; it++)
     {
          traverse((*it));
	 }
	}
	return;
}

