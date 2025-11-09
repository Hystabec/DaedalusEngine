#pragma once

#include "project.h"

namespace daedalus {

	class ProjectSerializer
	{
	public:
		ProjectSerializer(Shr_ptr<Project> project);

		bool serialize(const std::filesystem::path& path);
		bool deserialize(const std::filesystem::path& path);

	private:
		Shr_ptr<Project> m_project;
		static constexpr uint32_t m_currentFileFormatVersion = 1;
	};

}