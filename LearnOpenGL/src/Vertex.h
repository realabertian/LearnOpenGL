/*

*/
#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>
#include <string>
#include "assimp/scene.h"
#include "glad/glad.h"


	struct Vertex {
		/* 顶点位置 */
		glm::vec3 Position;

		/* 顶点的法线 */
		glm::vec3 Normal;

		/* 顶点的纹理坐标 */
		glm::vec2 TexCoords;
	};

	struct Texture {
		/* 纹理OpenGL ID */
		GLuint id;
		/* 纹理类型 */
		// TODO 
		std::string type;

		aiString path; // 纹理的加载路径,用来避免重复加载
	};


#endif