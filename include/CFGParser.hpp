/**
	Copyright (c) 2021 Kazim Kamilov

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.
	
	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.
	
	3. This notice may not be removed or altered from any source distribution. 
*/

#ifndef _CFG_PARSER_HPP_
#define _CFG_PARSER_HPP_

#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>

#ifdef USE_GLM
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

using Vec2b = glm::bvec2;
using Vec3b = glm::bvec3;
using Vec4b = glm::bvec4;

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;

using Vec2d = glm::dvec2;
using Vec3d = glm::dvec3;
using Vec4d = glm::dvec4;

using Vec2i = glm::ivec2;
using Vec3i = glm::ivec3;
using Vec4i = glm::ivec4;

using Vec2u = glm::uvec2;
using Vec3u = glm::uvec3;
using Vec4u = glm::uvec4;
#else
#include "vector.hpp"
#endif


/**
	@brief class for parsing CFG configs in ini-style.

	Allow:
	- Boolean variables can be specified as "true" and "false", "on" and "off", "yes" and "no", "1" and "0".
	- Single-line strings with escape sequence characters.
	- Single-line commenting with the symbol ';'
	- Vector values are separated by commas(64, 128, 255).
	- Section inheritance is supported(single for now).
	- File inclde is supported.

	The syntax is simple:
	@code
	#include "path_of_the_file_to_include"

	[section_name]
	key = value
	key_string = "some text"
	key_vector = 53.74, 632.83, 146.013

	[section_name] : inherited_section_name
	key = value
	@endcode
*/
class CFGParser final
{
public:

	using CFGValueData = std::unordered_map<std::string, std::string>;

	struct SectionData final
	{
		std::string inherited;
		CFGValueData data;
	};

	using CFGData = std::unordered_map<std::string, SectionData>;

public:
	CFGParser(const std::string& filename);
	~CFGParser();

	void open(const std::string& filename) noexcept;

	const bool isHaveSection(const std::string& section) const noexcept;
	const bool isHaveSectionKey(const std::string& section, const std::string& key) const noexcept;

	const size_t getSectionsNum() const noexcept;
	const size_t getSectionValuesNum(const std::string& section) const noexcept;
	const CFGData& getData() const noexcept;

	const bool getBool(const std::string& section, const std::string& key, const bool default_value = false) const noexcept;
	const char getChar(const std::string& section, const std::string& key, const char default_value = 0x00) const noexcept;
	const unsigned char getUChar(const std::string& section, const std::string& key, const unsigned char default_value = 0x00u) const noexcept;
	const short getShort(const std::string& section, const std::string& key, const short default_value = 0x0000) const noexcept;
	const unsigned short getUShort(const std::string& section, const std::string& key, const unsigned short default_value = 0x0000u) const noexcept;
	const int getInt(const std::string& section, const std::string& key, const int default_value = 0) const noexcept;
	const unsigned int getUInt(const std::string& section, const std::string& key, const unsigned int default_value = 0u) const noexcept;
	const long getLong(const std::string& section, const std::string& key, const long& default_value = 0l) const noexcept;
	const unsigned long getULong(const std::string& section, const std::string& key, const unsigned long& default_value = 0ul) const noexcept;
	const unsigned long long getULongLong(const std::string& section, const std::string& key, const unsigned long long& default_value = 0ull) const noexcept;
	const float getFloat(const std::string& section, const std::string& key, const float& default_value = 0.0f) const noexcept;
	const double getDouble(const std::string& section, const std::string& key, const double& default_value = 0.0) const noexcept;
	const long double getLongDouble(const std::string& section, const std::string& key, const long double& default_value = 0.0l) const noexcept;
	const std::string& getString(const std::string& section, const std::string& key, const std::string& default_value = std::string()) const noexcept;

	const Vec2 getVec2(const std::string& section, const std::string& key, const Vec2& default_value = Vec2()) const noexcept;
	const Vec2b getVec2b(const std::string& section, const std::string& key, const Vec2b& default_value = Vec2b()) const noexcept;
	const Vec2d getVec2d(const std::string& section, const std::string& key, const Vec2d& default_value = Vec2d()) const noexcept;
	const Vec2i getVec2i(const std::string& section, const std::string& key, const Vec2i& default_value = Vec2i()) const noexcept;
	const Vec2u getVec2u(const std::string& section, const std::string& key, const Vec2u& default_value = Vec2u()) const noexcept;
	const Vec3 getVec3(const std::string& section, const std::string& key, const Vec3& default_value = Vec3()) const noexcept;
	const Vec3b getVec3b(const std::string& section, const std::string& key, const Vec3b& default_value = Vec3b()) const noexcept;
	const Vec3d getVec3d(const std::string& section, const std::string& key, const Vec3d& default_value = Vec3d()) const noexcept;
	const Vec3i getVec3i(const std::string& section, const std::string& key, const Vec3i& default_value = Vec3i()) const noexcept;
	const Vec3u getVec3u(const std::string& section, const std::string& key, const Vec3u& default_value = Vec3u()) const noexcept;
	const Vec4 getVec4(const std::string& section, const std::string& key, const Vec4& default_value = Vec4()) const noexcept;
	const Vec4b getVec4b(const std::string& section, const std::string& key, const Vec4b& default_value = Vec4b()) const noexcept;
	const Vec4d getVec4d(const std::string& section, const std::string& key, const Vec4d& default_value = Vec4d()) const noexcept;
	const Vec4i getVec4i(const std::string& section, const std::string& key, const Vec4i& default_value = Vec4i()) const noexcept;
	const Vec4u getVec4u(const std::string& section, const std::string& key, const Vec4u& default_value = Vec4u()) const noexcept;

	void setString(const std::string& section, const std::string& key, const std::string& value) noexcept;

	const CFGValueData& getSectionData(const std::string& section_name) const noexcept;
	const unsigned int getKeyLineInSection(const std::string& section, const std::string& key) const noexcept;

	template<typename T> const T get(const std::string& section, const std::string& key, const T& default_value = 0) const noexcept
	{
		const auto& desc = this->getString(section, key);

		if (desc.empty())
		{
			return default_value;
		}
		else
		{
			T value = 0;
			std::istringstream stream(desc);

			if (typeid(T) == typeid(bool))
				stream >> std::boolalpha >> value;
			else
				stream >> value;

			return value;
		}
	}

	template<typename T> std::vector<T> getArray(const std::string& section, const std::string& key) const noexcept
	{
		const auto& desc = this->getString(section, key);

		if (desc.empty())
		{
			return {};
		}
		else
		{
			std::vector<T> vec;
			std::string value_str;

			for (auto iter = desc.begin(); iter != desc.end(); ++iter)
			{
				const bool last_char = (iter + 1u) == desc.end();

				if ((*iter) == ',' || last_char)
				{
					if (last_char)
						value_str.push_back(*iter);

					T value = 0;
					std::istringstream stream(value_str);

					if (typeid(T) == typeid(bool))
						stream >> std::boolalpha >> value;
					else
						stream >> value;

					vec.push_back(value);

					value_str.clear();
				}
				else
				{
					value_str.push_back(*iter);
				}
			}

			return vec;
		}
	}

	std::vector<std::string> getStringArray(const std::string& section, const std::string& key) const noexcept
	{
		const auto& desc = this->getString(section, key);

		if (desc.empty())
		{
			return {};
		}
		else
		{
			std::vector<std::string> vec;
			std::string value_str;

			for (auto iter = desc.begin(); iter != desc.end(); ++iter)
			{
				const bool last_char = (iter + 1u) == desc.end();

				if ((*iter) == ',' || last_char)
				{
					if (last_char)
						value_str.push_back(*iter);

					vec.push_back(value_str);

					value_str.clear();
				}
				else
				{
					value_str.push_back(*iter);
				}
			}

			return vec;
		}
	}

	template<typename T> void set(const std::string& section, const std::string& key, const T& value) noexcept
	{
		std::ostringstream stream;

		if (typeid(T) == typeid(bool))
			stream << std::boolalpha << value;
		else
			stream << value;

		this->setString(section, key, stream.str());
	}

	template<typename T> void setArray(const std::string& section, const std::string& key, const std::vector<T>& value) noexcept
	{
		std::ostringstream stream;

		for (auto iter = value.begin(); iter != value.end(); ++iter)
		{
			if (typeid(T) == typeid(bool))
				stream << std::boolalpha << (*iter);
			else
				stream << (*iter);

			if ((iter + 1u) != value.end())
				stream << ", ";
		}

		this->setString(section, key, stream.str());
	}

protected:
	void clearData();
	void readData();
	void writeData();

	const CFGData::const_iterator sectionSearch(const std::string& section_name, const std::string& key) const noexcept;

private:
	CFGData _data;
	const std::string _filename;

};

#endif
