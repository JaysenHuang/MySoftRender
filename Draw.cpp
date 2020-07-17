#include "SoftRender/draw.h"



void setModelMatrix(const glm::mat4& model) {
	shader.setModelMatrix(model);
}
void setViewMatrix(const glm::mat4& view) {
	shader.setViewMatrix(view);
}
void setProjectMatrix(const glm::mat4& project) {
	shader.setProjectMatrix(project);
}
bool FaceCulling(const glm::vec4 &v1,const glm::vec4 &v2,const glm::vec4 &v3) {
	glm::vec3 tmp1 = glm::vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
	glm::vec3 tmp2 = glm::vec3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
	glm::vec3 normal = glm::normalize(glm::cross(tmp1, tmp2));

	return glm::dot(normal, glm::vec3(0, 0, 1)) < 0;  //返回1则证明为正面
}

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

	
		//做透视除法 变换到NDC

		PerspectiveDivision(v1);
		PerspectiveDivision(v2);
		PerspectiveDivision(v3);


		if (!FaceCulling(v1.windowPos, v2.windowPos, v3.windowPos)) {
			continue;
		}
	
		v1.windowPos =  ViewPortMatrix * v1.windowPos;
		v2.windowPos =  ViewPortMatrix * v2.windowPos;
		v3.windowPos =  ViewPortMatrix * v3.windowPos;

		

		ScanLineTriangle(v1, v2, v3);


	}
}
void Draw(SDL_Renderer* gRenderer, Shader shader, FrameBuffer FrontBuffer, const int SCREEN_WIDTH,const int SCREEN_HEIGHT) {	
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		for (int k = 0; k < SCREEN_WIDTH; k++) {
			if (k < 0 || k > SCREEN_WIDTH || i < 0 || i > SCREEN_HEIGHT)
				return;

			
			int xy = i * SCREEN_WIDTH+k;	
			if (FrontBuffer.colorBuffer[xy * 4] == 75) {
				 
			}
			else {
				SDL_SetRenderDrawColor(gRenderer, FrontBuffer.colorBuffer[xy * 4], FrontBuffer.colorBuffer[xy * 4 + 1], FrontBuffer.colorBuffer[xy * 4 + 2], FrontBuffer.colorBuffer[xy * 4 + 3]);
				//SDL_SetRenderDrawColor(gRenderer, shader.FragmentShader(v).x, shader.FragmentShader(v).y, shader.FragmentShader(v).z, shader.FragmentShader(v).w);
				SDLDrawPixel(k, i);
			}
		
		
		}
	}
}