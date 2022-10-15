#ifndef MESH_H
#define MESH_H

#include <vector>
#include "Vertex.h"
#include "Shader.h"
#include "glad/glad.h"


class Mesh {
public:
	/* 网络数据 */
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	GLuint VAO;

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);

	/* 
		调用此方法进行会绘制此Mesh
		@param shader 是绘制此mesh所指定的渲染程序, 不可为空
	*/
	
	void Draw(Shader& shader);
private:
	/* 属于OpenGL 的关键渲染数据 */
	GLuint VBO, EBO;

	/* 绘制前需要调用此方法进行OpenGL数据配置 */
	void setupMesh();
};



#endif // MESH_H
