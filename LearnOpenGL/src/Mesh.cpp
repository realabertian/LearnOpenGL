#include "Mesh.h"
#include <string>



Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
		
	setupMesh();
}

// 渲染
	// 这里由一些困难点, 首先 对于这个Mesh数据来说,我们在调用glDrawElements进行绘制之前,需要先绑定纹理,但是实际上,我们并不知道
	// 究竟由多少纹理,以及这些纹理是什么类型的纹理, 所以我们怎么在着色器中来声明这些纹理变量,以及如何设置呢?
	// 为了解决这个问题,我们需要设定一个命名标准, 每个漫反射的纹理在着色器中被声明为textture_diffuseN, 每个镜面光纹理则被声明
	// 为texture_specularN, 其中字母N是范围为1到纹理采器最大允许设置的个数(一般情况下OpenGL运行最大16个, 可能会更高,并且opengl中
	// 需要绑定纹理变量名到纹理编号, 假设OpenGL允许最大16个纹理,那么纹理编号从0-15). 
	// 比如 我们对某个网格由3个漫反射纹理, 2个镜面光纹理, 那么在着色器中 应该会被声明为如下 变量
	// uniform sampler2D texture_diffuse1;
	// uniform sampler2D textrue_diifuse2;
	// uniform sampler2D texture_diffuse3;
	// uniform sampler2D texture_specular1;
	// uniform sampler2D texture_specular2;

// 依据这个标准 我们可以在着色器中定义任意数量的纹理采样器, 如果一个网格真的包含了这么多的纹理, 我们也能知道他们的名字是什么
void Mesh::Draw(Shader& shader) {
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;

		for (GLuint i = 0; i < textures.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);  // 激活纹理单元
			// 获得shader中对应的纹理标识
			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			// 设置纹理单元序号(0-15 至少)
			// 当前激活的纹理编号是 i, 所以将material.texture_diffusei 纹理变量
			// 绑定到纹理编号i上
			shader.setInt(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		// 绘制网格
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		// 恢复为默认激活状态, 起到清楚状态的效果, 避免当前的绘制状态 影响到其他的Mesh的绘制工作
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
		
		// 顶点位置
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// 顶点法线
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::Normal));

		// 纹理坐标
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::TexCoords));

		// 取消激活此 VAO
		glBindVertexArray(0);
	}
