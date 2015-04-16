#include "Node.h"
//include GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <math.h>
//#include <common/cylinder.h>
#define PI 3.14159265
float delta;

Node::Node(string nodeName)
{
    m_nodeName = nodeName;
	m_parent = nullptr;
	m_listChildren.empty();
	nodeLocalMatrix = glm::mat4(1.0);
	//rotation = glm::mat4(1.0);
}
	
Node::Node(string nodeName, Node* parent)
{
    m_nodeName = nodeName;
	m_parent = parent;
	m_parent->m_listChildren.push_front(this);
	  nodeLocalMatrix = glm::mat4(1.0);
  
}

Node::~Node(void)
{
}

glm::mat4 Node::getLocalMatrix()
{
	delta+=0.001;
	
	if(m_parent->m_nodeName == "Palm" )//this is finer layer 1
		{
			nodeLocalMatrix = glm::mat4(1.0);
			
			if(sin(delta)>=0)
			{
				if(tagFinger==5)
				{
					nodeLocalMatrix = glm::rotate(nodeLocalMatrix,-sin(delta)*30, glm::vec3(0,1,0));
				}else
				{
					 nodeLocalMatrix = glm::rotate(nodeLocalMatrix,sin(delta)*30, glm::vec3(1,0,0));
				}
				 
			}else
			{
				if(tagFinger==5)
				{
					nodeLocalMatrix = glm::rotate(nodeLocalMatrix,sin(delta)*30, glm::vec3(0,1,0));
				}else
				{
					nodeLocalMatrix = glm::rotate(nodeLocalMatrix,-sin(delta)*30, glm::vec3(1,0,0));
				}
				
			}
			
			nodeLocalMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0,3.5,0)) * nodeLocalMatrix;	
		   if(tagFinger == 1)//first finger
		    {
			  nodeLocalMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-2,0,0))*nodeLocalMatrix;
		    }else if(tagFinger == 2) //second finger
	        {
			   nodeLocalMatrix = glm::translate(nodeLocalMatrix, glm::vec3(-0.7,0,0));
		    }else if(tagFinger == 3)  //third finger
		    {
		    	nodeLocalMatrix = glm::translate(nodeLocalMatrix, glm::vec3(0.7,0,0));
		    }else if(tagFinger == 5)  //thumb
		    {
		    	nodeLocalMatrix = glm::translate(nodeLocalMatrix, glm::vec3(3.5,-3.5,0));
		    }else  //last finger
		    {
			    nodeLocalMatrix = glm::translate(nodeLocalMatrix, glm::vec3(2,0,0));
		    }
        return nodeLocalMatrix;
	}else //finger layer 2 and 3
	{
		nodeLocalMatrix = glm::mat4(1.0);

		if(sin(delta)>=0)
			{
				if(tagFinger==5)
				{
					nodeLocalMatrix = glm::rotate(nodeLocalMatrix,-sin(delta)*30, glm::vec3(0,1,0));
				}else
				{
					nodeLocalMatrix = glm::rotate(nodeLocalMatrix,sin(delta)*30, glm::vec3(1,0,0));
				}
			
			}else
			{
				if(tagFinger==5)
				{
					nodeLocalMatrix = glm::rotate(nodeLocalMatrix,sin(delta)*30, glm::vec3(0,1,0));
				}
				else
				{
					nodeLocalMatrix = glm::rotate(nodeLocalMatrix,-sin(delta)*30, glm::vec3(1,0,0));
				}
				
			}
		nodeLocalMatrix = glm::translate(nodeLocalMatrix, glm::vec3(0,1.2,0));
	return nodeLocalMatrix;
	}
}

glm::mat4 Node::getGlobalTransform()
{
	if(m_nodeName == "Palm")
	{
		nodeGlobalMatrix = nodeLocalMatrix ;	
		return nodeGlobalMatrix;
	}
	else{
		nodeLocalMatrix = getLocalMatrix();
	    nodeGlobalMatrix = m_parent->nodeGlobalMatrix *  nodeLocalMatrix;	
	    return nodeGlobalMatrix;
	}
	
}

