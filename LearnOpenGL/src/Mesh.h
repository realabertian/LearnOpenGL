#ifndef MESH_H
#define MESH_H

#include <vector>
#include "Vertex.h"
#include "Shader.h"
#include "glad/glad.h"


class Mesh {
public:
	/* �������� */
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	GLuint VAO;

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);

	/* 
		���ô˷������л���ƴ�Mesh
		@param shader �ǻ��ƴ�mesh��ָ������Ⱦ����, ����Ϊ��
	*/
	
	void Draw(Shader& shader);
private:
	/* ����OpenGL �Ĺؼ���Ⱦ���� */
	GLuint VBO, EBO;

	/* ����ǰ��Ҫ���ô˷�������OpenGL�������� */
	void setupMesh();
};



#endif // MESH_H
