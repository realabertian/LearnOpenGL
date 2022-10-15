/*
表示一个顶点数据, 并且附带该顶点的 法线防线,以及 纹理坐标
*/
#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>
#include <string>
#include "assimp/scene.h"
#include "glad/glad.h"


	struct Vertex {
		/* 顶点的坐标 */
		glm::vec3 Position;

		/* 顶点的法线 */
		glm::vec3 Normal;

		/* 顶点的纹理坐标 */
		glm::vec2 TexCoords;
	};

	struct Texture {
		/* 在OpenGL中的纹理ID */
		GLuint id;
		/* 问题类型 该类型 指明了该纹理是 漫反射贴图 还是 镜面光贴图 */
		// TODO 考虑使用 枚举类型来表示 可能比较由可读性以及支持静态编译检查
		std::string type;

		aiString path; // 存储纹理的加载路径,用于比较纹理是否相同
	};


#endif