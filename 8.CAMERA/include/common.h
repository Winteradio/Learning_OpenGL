#ifndef __COMMON_H__
#define __COMMON_H__
// define이 안되어 있으면, 안에 있는 것들을 붙인다.
// 즉, 중복이 생기지 않게 한다.

#include <memory> // unique_ptr, shared_ptr, weak_ptr 사용하기 위한 라이브러리
#include <optional>
#include <string>
#include <vector>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define CLASS_PTR(klassname)\
	class klassname;\
	using klassname ## UPtr = std::unique_ptr<klassname>;\
	using klassname ## Ptr = std::shared_ptr<klassname>;\
	using klassname ## WPtr = std::weak_ptr<klassname>;

std::optional<std::string> LoadTextFile(const std::string& filename);

#endif // __COMMON_H___
