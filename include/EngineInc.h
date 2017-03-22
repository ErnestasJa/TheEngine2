#ifndef ENGINE_INC_H
#define ENGINE_INC_H

/*
*	This file is meant to be included into every .cpp file in engine.
*	That is done automatically by gcc compiler flag '-include <header_name>'
*/

/*Standard c/c++ headers*/
#include <array>
#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

/*Third party headers*/
#include "glm/glm.hpp"
#include "glm/ext.hpp"

/*Engine headers*/
#include "EngineForwardDeclares.h"
#include "core/Algorithm.hpp"
#include "core/Debug.h"
#include "core/POD.h"
#include "core/StringUtil.h"
#include "core/TypeUtils.h"

#endif
