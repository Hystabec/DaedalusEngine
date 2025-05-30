#pragma once

#ifdef DD_PLATFORM_WINDOWS
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
#include <unordered_map>
#include <unordered_set>

#include "core.h"
#include "debugTools/logging/log.h"
#include "debugTools/profiling/instrumentor.h"