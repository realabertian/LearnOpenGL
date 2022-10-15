/*
��ʾһ����������, ���Ҹ����ö���� ���߷���,�Լ� ��������
*/
#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>
#include <string>
#include "assimp/scene.h"
#include "glad/glad.h"


	struct Vertex {
		/* ��������� */
		glm::vec3 Position;

		/* ����ķ��� */
		glm::vec3 Normal;

		/* ������������� */
		glm::vec2 TexCoords;
	};

	struct Texture {
		/* ��OpenGL�е�����ID */
		GLuint id;
		/* �������� ������ ָ���˸������� ��������ͼ ���� �������ͼ */
		// TODO ����ʹ�� ö����������ʾ ���ܱȽ��ɿɶ����Լ�֧�־�̬������
		std::string type;

		aiString path; // �洢����ļ���·��,���ڱȽ������Ƿ���ͬ
	};


#endif