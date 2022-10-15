#include "Mesh.h"
#include <string>



Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
		
	setupMesh();
}

// ��Ⱦ
	// ������һЩ���ѵ�, ���� �������Mesh������˵,�����ڵ���glDrawElements���л���֮ǰ,��Ҫ�Ȱ�����,����ʵ����,���ǲ���֪��
	// �����ɶ�������,�Լ���Щ������ʲô���͵�����, ����������ô����ɫ������������Щ��������,�Լ����������?
	// Ϊ�˽���������,������Ҫ�趨һ��������׼, ÿ�����������������ɫ���б�����Ϊtextture_diffuseN, ÿ�����������������
	// Ϊtexture_specularN, ������ĸN�Ƿ�ΧΪ1��������������������õĸ���(һ�������OpenGL�������16��, ���ܻ����,����opengl��
	// ��Ҫ���������������������, ����OpenGL�������16������,��ô������Ŵ�0-15). 
	// ���� ���Ƕ�ĳ��������3������������, 2�����������, ��ô����ɫ���� Ӧ�ûᱻ����Ϊ���� ����
	// uniform sampler2D texture_diffuse1;
	// uniform sampler2D textrue_diifuse2;
	// uniform sampler2D texture_diffuse3;
	// uniform sampler2D texture_specular1;
	// uniform sampler2D texture_specular2;

// ���������׼ ���ǿ�������ɫ���ж�����������������������, ���һ��������İ�������ô�������, ����Ҳ��֪�����ǵ�������ʲô
void Mesh::Draw(Shader& shader) {
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;

		for (GLuint i = 0; i < textures.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);  // ����������Ԫ
			// ���shader�ж�Ӧ��������ʶ
			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			// ����������Ԫ���(0-15 ����)
			// ��ǰ�������������� i, ���Խ�material.texture_diffusei ��������
			// �󶨵��������i��
			shader.setInt(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		// ��������
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		// �ָ�ΪĬ�ϼ���״̬, �����״̬��Ч��, ���⵱ǰ�Ļ���״̬ Ӱ�쵽������Mesh�Ļ��ƹ���
		glBindVertexArray(0);
		glActiveTexture(0);
	}

void Mesh::setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// fill data into VBO
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
		
		// ����λ��
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// ���㷨��
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Normal));

		// ��������
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::TexCoords));

		// ȡ������� VAO
		glBindVertexArray(0);
	}