#include"SoftRender\Mesh.h"

Mesh::Mesh() = default;

Mesh::~Mesh() = default;

Mesh::Mesh(const int& vNum, const int& iNum) {
	VBO.resize(vNum);
	EBO.resize(iNum);
}

Mesh::Mesh(const Mesh& mesh):VBO(mesh.VBO),EBO(mesh.EBO){}

Mesh& Mesh::operator=(const Mesh& mesh) {
	if (&mesh == this)
		return *this;

	VBO = mesh.VBO;
	EBO = mesh.EBO;
	return *this;
}

Mesh& Mesh::operator +=(const Mesh& mesh) {
	AddMesh(mesh);
	return *this;
}


void Mesh::AddMesh(const Mesh& mesh) {
	int offset = VBO.size();
	VBO.insert(VBO.end(), mesh.VBO.begin(), mesh.VBO.end());
	EBO.reserve(EBO.size() + mesh.EBO.size());
	for (int i = 0; i < mesh.EBO.size(); i++) {
		EBO.push_back(mesh.EBO[i] + offset);
	}

}

void Mesh::AddTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
	int offset = VBO.size();
	VBO.push_back(v1);
	VBO.push_back(v2);
	VBO.push_back(v3);
	EBO.push_back(offset);
	EBO.push_back(offset + 1);
	EBO.push_back(offset + 2);
}