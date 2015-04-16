//#ifndef __CYLINDER_H__
//#define __CYLINDER_H__
//
//#include "maths_funcs.h"
//#include <GL/Glew.h>
//#include <vector>
//
//// Macro for indexing vertex buffer
//#define BUFFER_OFFSET(i) ((char *)NULL + (i))
//#define NumVertices 4
//
//class basic {
//
//public:
//	basic()
//	{
//		generateVertices();
//	};
//
//	
//	// generate a cylinder with the bottom base center at (0,0,0), up on the Y axis
//	void generateVertices()
//	{
//		
//		GLfloat vertices[] = { 
//			0.0f,-1.0f, 0.0f,1.0f,
//			0.5f,-1.0f, 0.0f,1.0f,
//			0.5f, -0.5f, 0.0f,1.0f,
//			0.0f, -0.5f, 0.0f,1.0f
//	};
//
//	GLfloat colors[] = { 
//		0.583f,  0.771f,  0.014f, 1.0f,
//		0.609f,  0.115f,  0.436f, 1.0f,
//		0.327f,  0.483f,  0.844f, 1.0f,
//		1.0f,0.0f,1.0f
//	};
//	    
//	}
//
//	void generateObjectBuffer() {
//		//Initialize VAO
//		glGenVertexArrays( 1, &vertex_array_object_id );
//		glBindVertexArray( vertex_array_object_id );
//
//		
//		//Initialize VBO
//		glGenBuffers( 1, &buffer_object_id );
//		glBindBuffer( GL_ARRAY_BUFFER, buffer_object_id );
//		
//		
//
//	// After binding, we now fill our object with data, everything in "Vertices" goes to the GPU
//	glBufferData(GL_ARRAY_BUFFER, NumVertices*7*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
//
//	// if you have more data besides vertices (e.g., vertex colours or normals), use glBufferSubData 
//	//to tell the buffer when the vertices array ends and when the colors start
//	glBufferSubData (GL_ARRAY_BUFFER, 0, NumVertices*3*sizeof(GLfloat), vertices);
//	glBufferSubData (GL_ARRAY_BUFFER, NumVertices*3*sizeof(GLfloat), NumVertices*4*sizeof(GLfloat), colors);
//
//		//Set up Vertex Arrays  
//		glEnableVertexAttribArray( 0 ); //SimpleShader attrib at position 0 = "vPosition"
//		glVertexAttribPointer( (GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
//		glEnableVertexAttribArray( 1 ); //SimpleShader attrib at position 1 = "vColor"
//		//*************************************why 0, why 1???????????????????????????????????????????????
//		glVertexAttribPointer( (GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertexArraySize));
//
//		//Unbind
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindVertexArray(0);
//
//		//Initialize IBO
//		glGenBuffers(1, &index_buffer_object_id);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_id);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices)*sizeof(GLuint), (const GLvoid*)(&vertex_indexes[0]), GL_STATIC_DRAW);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	}
//
//	void draw(){
//		glBindVertexArray(vertex_array_object_id);
//
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_id);
//		int size;  
//		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
//		size /= sizeof(GLuint);
//		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
//
//		glBindVertexArray(0);//******************************************************************WHY????
//	}
//
//private:
//
//	GLfloat vertices[];
//	GLfloat colors[];
//	GLuint vertexArraySize;
//	GLuint colorArraySize;
//
//	GLuint buffer_object_id;
//	GLuint vertex_array_object_id;
//	GLuint index_buffer_object_id;
//};
//
//#endif // __CYLINDER_H__