#pragma once
#ifndef _MATH_H_
#define _MATH_H_


#include"glm\glm\glm.hpp"

glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, float factor);
glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, float factor);
glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, float factor);
glm::mat4 GetViewPortMatrix(int ox, int oy, int width, int height);
glm::mat4 GetViewPortMatrix(glm::vec3 pos,glm::vec3 front,glm::vec3 right,glm::vec3 up);
glm::mat4 GetPerspectiveMatrix(const float& fovy, const float& aspect, const float n, const float& f);
#endif // !_MATH_H_