#ifndef ENGINE_INC_H
#define ENGINE_INC_H

/*
*	This file is meant to be included into every .cpp file in engine.
*	That is done automatically by gcc compiler flag '-include <header_name>'
*/

/*Standard c/c++ headers*/
#include <limits>
#include <cstdint>
#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <type_traits>

/*Third party headers*/

/*Engine headers*/
#include "EngineForwardDeclares.h"
#include "core/POD.h"
#include "core/TypeUtils.h"
#include "core/StringUtil.h"
#include "core/Algorithm.hpp"

#endif
