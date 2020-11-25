#ifndef ENGINE_INC_H
#define ENGINE_INC_H

/*
 *	This file is meant to be included into every .cpp file in engine.
 *	That is done automatically by gcc compiler flag '-include <header_name>'
 */

/*Standard c/c++ headers*/
#include <array>
#include <assert.h>
#include <cstdint>
#include <limits>
#include <memory>
#include <stack>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>
#include <queue>

/*Third party headers*/
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_float.hpp>

/*Engine headers*/
#include "Config.h"
#include "Macro.h"
#include "EngineForwardDeclares.h"
#include "core/Algorithm.hpp"
#include "core/POD.h"
#include "core/StringUtil.h"
#include "core/TypeUtils.h"
#include "core/memory/Memory.h"
#include "filesystem/FileSystemInc.h"
#include "log/LogInc.h"
#include "util/Assert.h"

#endif
