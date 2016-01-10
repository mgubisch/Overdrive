#pragma once

#include "preprocessor.h"

// include openGL-related headers, in the prescribed order

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#if OVERDRIVE_PLATFORM == OVERDRIVE_PLATFORM_WINDOWS
	// MSVC pragma's to suppress GLM's warnings
	#pragma warning(push)
	#pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union
#endif

// consider all angles radians -- none of that 'degree' stuff
// use metaprogramming helpers (number of elements in a vec etc)
// GLM v0.9.7.2 (downloaded on 6th of januari 2016)
#define GLM_FORCE_RADIANS
#define GLM_META_PROG_HELPERS
	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <glm/gtc/quaternion.hpp>
	#include <glm/gtc/type_ptr.hpp>
	#include <glm/gtx/transform2.hpp>
#undef GLM_META_PROG_HELPERS
#undef GLM_FORCE_RADIANS

#if OVERDRIVE_PLATFORM == OVERDRIVE_PLATFORM_WINDOWS
	#pragma warning(pop)
#endif