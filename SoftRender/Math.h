#pragma once
#ifndef _MATH_H_
#define _MATH_H_


#include"glm\glm\glm.hpp"

glm::vec4 Lerp(const glm::vec4& v1, const glm::vec4& v2, float factor);
glm::vec3 Lerp(const glm::vec3& v1, const glm::vec3& v2, float factor);
glm::vec2 Lerp(const glm::vec2& v1, const glm::vec2& v2, float factor);
glm::mat4 GetViewPortMatrix(int ox, int oy, int width, int height);
#endif // !_MATH_H_