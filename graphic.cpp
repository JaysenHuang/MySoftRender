#include "SoftRender\graphic.h"

//Class FrameBuffer



FrameBuffer::~FrameBuffer()=default;

FrameBuffer::FrameBuffer(const int& w = 1280, const int& h = 720) {
	Width = w;
	Height = h;
	//RGBA
	colorBuffer.resize(w * h * 4, 0);
	depthBuffer.resize(w * h, 1.0f);
	
}

void FrameBuffer::Resize(const int& w, const int& h) {
	Width = w;
	Height = h;
	colorBuffer.resize(w * h * 4, 0);
	depthBuffer.resize(w * h, 1.0f);
}

void FrameBuffer::ClearColorBuffer(const glm::vec4& color) {
	float* p = colorBuffer.data();
	for (int i = 0; i < Width * Height * 4; i += 4) {
		*(p + i) = color.r;
		*(p + i + 1) = color.g;
		*(p + i + 2) = color.b;
		*(p + i + 3) = color.a;
		
	}

	/*
	int length = Width * Height * 4;
	for (int i = 0; i < length; i+=4) {
		colorBuffer[i] = 255;
		colorBuffer[i + 1] = 0;
		colorBuffer[i + 2] = 0;
		colorBuffer[i + 3] = 0;
	}*/
}

void FrameBuffer::ClearDepthBuffer() {
	int size = Width * Height;
	float* p = depthBuffer.data();
	for (int i = 0; i < size; i++) {
		*(p + i) = 1.0f;
	}
}
void FrameBuffer::WritePoint(const int x, const int y, const glm::vec4 color, SDL_Renderer* gRenderer) {
	if (x < 0 || x >= Width || y < 0 || y >= Height)
		return;
	int xy = (y*Width +x);

	colorBuffer[xy * 4] = color.r;
	colorBuffer[xy * 4 + 1] = color.g;
	colorBuffer[xy * 4 + 2] = color.b;
	colorBuffer[xy * 4 + 3] = color.a;
	//SDL_SetRenderDrawColor(gRenderer, colorBuffer[xy * 4], colorBuffer[xy * 4 + 1], colorBuffer[xy * 4 + 2], colorBuffer[xy * 4 + 3]);
	//SDLDrawPixel(x,y);
}

float FrameBuffer::GetDepth(const int& x, const int& y){
	if (x < 0 || x >= Width || y < 0 || y >= Height)
		return 1.0f;
	
	return *(depthBuffer.data() + y* Width + x);
	
}

void FrameBuffer::WriteDepth(const int& x, const int& y, const float& depth){
	if (x < 0 || x >= Width || y < 0 || y >= Height)
		return;
	int pos = y * Width + x;
	
	float* p = depthBuffer.data();
	*(p + pos) = depth;
	
}

//Class Vertex

Vertex::Vertex() = default;

Vertex::~Vertex() = default;

Vertex::Vertex(
	const glm::vec4& _pos,
	const glm::vec4& _color,
	const glm::vec2& _tex,
	const glm::vec3& _normal
) :
	position(_pos), color(_color), texcoord(_tex), normal(_normal) {}

Vertex::Vertex(
	const glm::vec3& _pos,
	const glm::vec4& _color,
	const glm::vec2& _tex,
	const glm::vec3& _normal 
) :
	position(_pos, 1.0f), color(_color), texcoord(_tex), normal(_normal) {}

Vertex::Vertex(const Vertex& v):
	position(v.position), color(v.color), texcoord(v.texcoord), normal(v.normal) {}

//Class V2F

V2F::V2F() = default;
V2F::~V2F() = default;

V2F::V2F(
	const glm::vec4& _wPos,
	const glm::vec4& _pPos,
	const glm::vec4& _color,
	const glm::vec2& _tex,
	const glm::vec3& _normal
) :
	worldPos(_wPos), windowPos(_pPos), color(_color), texcoord(_tex), normal(_normal) {}

V2F::V2F(const V2F& v) :
	worldPos(v.worldPos), windowPos(v.windowPos), color(v.color), texcoord(v.texcoord), normal(v.normal),Z(v.Z) {}

V2F V2F::lerp(const V2F& v1, const V2F& v2, const float& factor) {
V2F result;
result.windowPos = Lerp(v1.windowPos, v2.windowPos, factor);
result.worldPos = Lerp(v1.worldPos, v2.worldPos, factor);
result.color = Lerp(v1.color, v2.color, factor);
result.normal = Lerp(v1.normal, v2.normal, factor);
result.texcoord = Lerp(v1.texcoord, v2.texcoord, factor);
result.Z = Lerp(v1.Z, v2.Z, factor);
return result;
	}

//Class Shader

Shader::Shader() {
	ModelMatrix = glm::mat4(1.0f);
	ViewMatrix = glm::mat4(1.0f);
	ProjectMatrix = glm::mat4(1.0f);
}

Shader::~Shader() = default;

V2F Shader::VertexShader(const Vertex& a2v) {
	V2F o;
	o.worldPos = ModelMatrix * a2v.position;
	// PVM*v
	o.windowPos = ProjectMatrix * ViewMatrix * o.worldPos;

	o.Z = 1/o.windowPos.w;
	o.worldPos =o.worldPos* o.Z;
	o.color = a2v.color*o.Z;
	o.normal = a2v.normal*o.Z;
	o.texcoord = a2v.texcoord*o.Z;
	return o;
}

/*glm::vec4 Shader::FragmentShader(const V2F& v) {
	glm::vec4 color;
	color = v.color;
	return color;
}*/

void Shader::setModelMatrix(const glm::mat4& model) {
	ModelMatrix = model;
}

void Shader::setViewMatrix(const glm::mat4& view) {
	ViewMatrix = view;
}

void Shader::setProjectMatrix(const glm::mat4& project) {
	ProjectMatrix = project;
}

glm::vec4 Shader::FragmentShader(const V2F& v,Texture texture) {
	glm::vec4 color = texture.Sample2D(v.texcoord);
	return color;
}

void PerspectiveDivision(V2F& v) {
	
	v.windowPos /= v.windowPos.w;
	v.windowPos.w = 1.0f;
	v.windowPos.z = (v.windowPos.z +1.0f ) /2;
}

