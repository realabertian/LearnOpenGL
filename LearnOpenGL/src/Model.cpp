#include "Model.h"
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <iostream>

Model::Model(const char* path)
{
	gammaCorrection = false;
	loadModel(path);
}

void Model::Draw(Shader& shader)
{
	for (GLuint i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

void Model::processNode(aiNode* node, const aiScene* scene) 
{
	// 处理节点的所有网格
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// 接下来对子节点进行处理
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) 
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		// 处理顶点位置, 法线, 纹理坐标
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		// Assimp 运行一个模型上的顶点最多包含8个不同的纹理坐标, 对与我们的程序来说我们不需要
		// 这里只简单的取第一组纹理坐标即可
		if (mesh->mTextureCoords[0]) // 是否有纹理坐标
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// 处理索引
	// Assimp 为每个网格都定义了一个面数组, 每个面代表了一个图元(对于本程序就是三角形 ,因为我们在加载已经指定了要进行三角形处理)
	// 索引也定义了绘制面的时的顶点顺序
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
		
	// 处理材质
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
		std::vector<Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for (GLuint j = 0; j < textures_loaded.size(); i++)
			{
				if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // 纹理时同样路径的已经被加载过了 我们跳过
					break;
				}
			}
			if (!skip)
			{
				// 纹理没有被加载过, 我们要加载
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), directory, gammaCorrection);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
			}
			
		}
		return textures;
	}

	// 实际通过Assimp工具导入模型文件
void Model::loadModel(const std::string& path) {
		Assimp::Importer importer;
		// 第一个参数是被导入的模型的文件路径, 第二个参数是 后期处理选项, Assimp允许我们设定一些选项来
		// 对模型的结果进行一些额外的计算或者操作
		// 这里的aiProcess_Triangulate 告之Assimp如果模型不是由三角形组成, 则将其转换为三角形图元
		// aiProcess_FlipUVs 则会在处理的时候翻转y轴纹理坐标(这是因为OpenGL中的大部分的图像的Y轴都是反的)
		// 还有一些 比如:
		// - aiProcess_GenNormals: 如果模型不包含法向量,就为每个顶点创建法线
		// - aiProcess_SplitLargeMeshes: 将比较大的网格分割成更小的子网格,如果你的渲染有最大顶点数限制,只能渲染
		//     较小的网格, 这个选项就会非常有用
		// - aiProcess_OptimizeMeshes: 和上个选项相反, 它会将多个小网格拼接为一个大网格, 减少绘制调用 从而进行优化
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		// 必须检查下 场景是否为空,以及 其根节点是否为null, 以及数据是否完整
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}
		int x = path.find_last_of('\\');
		directory = path.substr(0, x);

		processNode(scene->mRootNode, scene);
	}

GLuint TextureFromFile(const char* path, const std::string& directory, bool gamma = false)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}