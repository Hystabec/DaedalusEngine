#pragma once

#include <filesystem>
#include <format>

// This is a custom format for std::filesystem
// as in C++20 it doesn't have a format

// This is untested with file that require wstrings

template<>
struct std::formatter<std::filesystem::path>
{

	constexpr auto parse(std::format_parse_context& ctx)
	{
		return ctx.begin(); 
	}

	auto format(const std::filesystem::path& name, std::format_context& ctx) const
	{
		return std::format_to(ctx.out(), "{}", name.generic_string());
	}

};