#pragma once
#include "..\SoftRender\graphic.h"
#include "..\SoftRender\Mesh.h"
#include "SoftRender\setting.h"

void setModelMatrix(const glm::mat4& model);
void setViewMatrix(const glm::mat4& view);
void setProjectMatrix(const glm::mat4& project) ;



void DrawMesh(const Mesh& mesh);
void Draw(SDL_Renderer* gRenderer, Shader shader, FrameBuffer FrontBuffer, const int SCREEN_WIDTH, const int SCREEN_HEIGHT);