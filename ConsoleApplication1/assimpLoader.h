#pragma once
#include <vector>
#include <glm.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/cimport.h> // C importer

#define MAX_BONES 32

std::vector<float> g_vp, g_vn, g_vt;
int g_point_count = 0;
int boneCount = 0;
//std::vector<glm::mat4> BoneOffset;
glm::mat4* BoneOffset;
std::vector<string> boneNames; 

//struct Skeleton_Node {
//	Skeleton_Node* children[MAX_BONES];
//	char name[64];
//	int num_children;
//	/* if this node corresponds to one of our weight-painted bones then we give
//	the index of that (the bone_ID) here, otherwrise it is set to -1 */
//	int bone_index;//The index to a bone in our array
//};
//
//glm::mat4 convertAssimpMatrix(aiMatrix4x4 from)
//{
//	glm::mat4 temp;
//
//	temp[0][0] = (GLfloat)from.a1; temp[1][0] = (GLfloat)from.a2;
//	temp[2][0] = (GLfloat)from.a3; temp[3][0] = (GLfloat)from.a4;
//	temp[0][1] = (GLfloat)from.b1; temp[1][1] = (GLfloat)from.b2;
//	temp[2][1] = (GLfloat)from.b3; temp[3][1] = (GLfloat)from.b4;
//	temp[0][2] = (GLfloat)from.c1; temp[1][2] = (GLfloat)from.c2;
//	temp[2][2] = (GLfloat)from.c3; temp[3][2] = (GLfloat)from.c4;
//	temp[0][3] = (GLfloat)from.d1; temp[1][3] = (GLfloat)from.d2;
//	temp[2][3] = (GLfloat)from.d3; temp[3][3] = (GLfloat)from.d4;
//
//	return temp;
//}
//
//
//void skeleton_animate (Skeleton_Node* node, glm::mat4 parent_mat, glm::mat4* bone_offset_mats,glm::mat4* bone_animation_mats) 
//{
//		assert (node);
//		/* the animation of a node after inheriting its parent's animation */
//		glm::mat4 our_mat = parent_mat;
//		/* the animation for a particular bone at this time */
//		glm::mat4 local_anim = glm::mat4(1.0f);
//
//		// if node has a weighted bone...
//		int bone_i = node->bone_index;
//		if (bone_i > -1) {
//			// ... then get offset matrices
//			glm::mat4 bone_offset = bone_offset_mats[bone_i];
//			glm::mat4 inv_bone_offset = glm::inverse(bone_offset);
//			// ... at the moment get the per-bone animation from keyboard input
//			glm::mat4 g_local_anims[MAX_BONES];
//			local_anim = g_local_anims[bone_i];
//
//			our_mat = parent_mat * inv_bone_offset * local_anim * bone_offset;
//			bone_animation_mats[bone_i] = our_mat;
//		}
//		for (int i = 0; i < node->num_children; i++) {
//			skeleton_animate (
//				node->children[i],
//				our_mat,
//				bone_offset_mats,
//				bone_animation_mats
//				);
//		}
//}
//
//
//bool import_skeleton_node (aiNode* assimp_node, Skeleton_Node** skeleton_node, int bone_count, std::vector<string> boneNames) 
//{
//	// allocate memory for node
//	//create a pointer to one of our skeleton nodes and allocate memory to it
//	Skeleton_Node* temp = (Skeleton_Node*)malloc (sizeof (Skeleton_Node));
//
//	// get node properties out of AssImp
//	strcpy_s (temp->name, assimp_node->mName.C_Str ());
//	printf ("-node name = %s\n", temp->name);
//	temp->num_children = 0;
//	printf ("node has %i children\n", (int)assimp_node->mNumChildren);
//	temp->bone_index = -1;//The index to a bone in our array. -1 means node doesn't directly correspond to a weighted bone
//	for (int i = 0; i < MAX_BONES; i++) {
//		temp->children[i] = NULL;
//	}
//
//	// look for matching bone name
//	bool has_bone = false;
//	for (int i = 0; i < bone_count; i++)
//	{
//		if (strcmp (boneNames[i], temp->name) == 0) 
//		{
//			printf ("node uses bone %i\n", i);
//			temp->bone_index = i;
//			has_bone = true;
//			break;
//		}
//	}
//	if (!has_bone) {
//		printf ("no bone found for node\n");
//	}
//
//	bool has_useful_child = false;
//	for (int i = 0; i < (int)assimp_node->mNumChildren; i++)
//	{
//		if (import_skeleton_node (assimp_node->mChildren[i], &temp->children[temp->num_children], bone_count, boneNames)) 
//		{
//			has_useful_child = true;
//			temp->num_children++;
//		} 
//		else
//		{
//			printf ("useless child culled\n");
//		}
//	}
//	if (has_useful_child || has_bone)
//	{
//		// point parameter to our allocated node
//		*skeleton_node = temp;
//		return true;
//	}
//	// no bone or good children - cull self
//	free (temp);
//	temp = NULL;
//	return false;
//}


  bool loadAssImp(
	const char * path, 
	std::vector<unsigned short> & indices,
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals
	//Skeleton_Node** root_node,
	//int pointCount, 
	//GLint* bone_ids
	)
{

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate/*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
		// const aiScene* scene = aiImportFile (file_name, aiProcess_Triangulate); // TRIANGLES!
		if( !scene) {
			fprintf( stderr, importer.GetErrorString());
			getchar();
			return false;
		}
		printf ("  %i animations\n", scene->mNumAnimations);
		printf ("  %i cameras\n", scene->mNumCameras);
		printf ("  %i lights\n", scene->mNumLights);
		printf ("  %i materials\n", scene->mNumMaterials);
		printf ("  %i meshes\n", scene->mNumMeshes);
		printf ("  %i textures\n", scene->mNumTextures);

		const aiMesh* mesh = scene->mMeshes[0]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)
		/*我们使用了aiMesh类的下列属性：
			mNumVertices - 顶点数量
			mVertices - 顶点位置向量mNumVertices
			mNormals - 顶点法向向量 mNormals
			mTextureCoords - 顶点纹理坐标向量 mTextureCoords ，注意一个顶点可能包含多个纹理坐标，所以该变量是一个二维数组。*/
		// Fill vertices positions
		if (mesh->HasPositions ())
		{
			vertices.reserve(mesh->mNumVertices);
			for(unsigned int i=0; i<mesh->mNumVertices; i++)
			{
				aiVector3D pos = mesh->mVertices[i];
				vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
			}

		}
		

		// Fill vertices texture coordinates
		if (mesh->HasTextureCoords (0))//I assume that there should only be 1 set of texture coordinates per vertex, so I give it "index 0"
		{
				uvs.reserve(mesh->mNumVertices);
				for(unsigned int i=0; i<mesh->mNumVertices; i++)
				{
					aiVector3D UVW = mesh->mTextureCoords[0][i]; // 只用了一个纹理坐标; AssImp supports 8 UV sets.
					uvs.push_back(glm::vec2(UVW.x, UVW.y));
	   			}
		 }
		

		// Fill vertices normals
		if (mesh->HasNormals ()) 
		{
			normals.reserve(mesh->mNumVertices);
			for(unsigned int i=0; i<mesh->mNumVertices; i++)
			{
				aiVector3D n = mesh->mNormals[i];
				normals.push_back(glm::vec3(n.x, n.y, n.z));
			}
		}

		//if (mesh->HasBones())
		//{
		//	boneCount = (int)mesh->mNumBones;
		//	pointCount = mesh->mNumVertices;
		//	bone_ids = (int*)malloc (pointCount * sizeof (int));
		//	for (int bone_i = 0; bone_i < boneCount; bone_i++)
		//	{
		//		const aiBone* bone = mesh->mBones[bone_i];
		//		boneNames[bone_i] = bone->mName.data;
		//		//strcpy(boneNames[bone_i], bone->mName.data);
		//		printf("Bone_name is ", boneNames[bone_i]);
		//		BoneOffset[bone_i] = convertAssimpMatrix(bone->mOffsetMatrix);
		//		int num_weights = (int)bone->mNumWeights;
		//		//an input attribute bone_weights[4], saying what factor each of the bone's matrices has on the vertex
		//		for (int w_i = 0; w_i < num_weights; w_i++)
		//		{
		//			aiVertexWeight weight = bone->mWeights[w_i];
		//			int vertex_id = (int)weight.mVertexId;
		//			// ignore weight if less than 0.5 factor
		//			if (weight.mWeight >= 0.5f) //work out a single bone ID for each vertex
		//			{
		//				bone_ids[vertex_id] = bone_i;
		//			}
		//		}
		//	}

		//	/* get the skeleton hierarchy from a separate AssImp data structure */
		//	// there should always be a 'root node', even if no skeleton exists
		//	aiNode* assimp_node = scene->mRootNode;
		//	//Skeleton_Node** root_node = NULL;
		//	if (!import_skeleton_node (assimp_node, root_node, boneCount, boneNames))
		//	{
		//			fprintf (stderr, "ERROR: could not import node tree from mesh\n");
		//	} // endif
		//}
		//
		if (mesh->HasTangentsAndBitangents ()) {
			// NB: could store/print tangents here
			//for normal mapping techniques
		}

		// Fill face indices
		//生成索引缓冲：aiMesh类的成员变量mNumFaces指定了每个mesh中包含多少个多边形(三角形)，mFaces成员变量包含具体的索引数据。
		indices.reserve(3*mesh->mNumFaces);
		for (unsigned int i=0; i<mesh->mNumFaces; i++){
			// Assume the model has only triangles.
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}
	printf ("mesh loaded\n");
	return true;
}

