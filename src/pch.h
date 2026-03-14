#pragma once

#define NOMINMAX
#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <REX/REX/TOML.h>
#include <CLIBUtil/utils.hpp>

#ifdef DEBUG
#define GENERATE_CONFIG_FILE
#endif

namespace logs = SKSE::log;
using namespace std::literals;
using namespace clib_util;
