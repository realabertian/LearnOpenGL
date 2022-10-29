#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include "Shader.h"
#include "Mesh.h"
#include <assimp/scene.h>

GLuint TextureFromFile(const char* path, const std::string& directory, bool gamma);

class Model {
public:

	Model(const char* path);

	void Draw(Shader& shader);

private:
	// model data
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	void loadModel(const std::string& path);

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
#endif // !MODEL_H
