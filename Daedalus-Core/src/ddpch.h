#pragma once

#ifdef DD_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
	#include <Windows.h>
#endif

#include <iostream>
#include <fstream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <format>
#include <chrono>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "core.h"
#include "utils/buffer.h"

#include "debugTools/logging/log.h"
#include "debugTools/profiling/instrumentor.h"