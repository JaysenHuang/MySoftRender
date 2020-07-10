#include "SoftRender/draw.h"



void DrawMesh(const Mesh& mesh) {
	if (mesh.EBO.empty()) {
		return;
	}
	for (int i = 0; i < mesh.EBO.size(); i += 3) {
		Vertex p1, p2, p3;
		p1 = mesh.VBO[mesh.EBO[i]];
		p2 = mesh.VBO[mesh.EBO[i + 1]];
		p3 = mesh.VBO[mesh.EBO[i + 2]];

		V2F	v1, v2, v3;
		v1 = shader.VertexShader(p1);
		v2 = shader.VertexShader(p2);
		v3 = shader.VertexShader(p3);

		v1.windowPos = ViewPortMatrix * v1.windowPos;
		v2.windowPos = ViewPortMatrix * v2.windowPos;
		v3.windowPos = ViewPortMatrix * v3.windowPos;
		ScanLineTriangle(v1, v2, v3);
	}
}