/**
	Copyright (c) 2020 Kazim Kamilov

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

#include "CFGParser.hpp"
#include <iostream>
#include <fstream>


static constexpr const char ALLOWED_CHARS[] =
{
	"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-\\"
};

enum class ReadMode : uint8_t
{
	none = 0x00u,
	section = 0x01u,
	inheritance = 0x02u,
	comment = 0x03u,
	key = 0x04u,
	value = 0x05u,
	string = 0x06u,
	escape_sequense = 0x07u,
	error = 0x08u
};

CFGParser::CFGParser(const std::string& filename) : _filename(filename)
{
	this->readData();
}

CFGParser::~CFGParser()
{
	this->clearData();
}

const bool CFGParser::getBool(const std::string& section, const std::string& key, const bool default_value) const noexcept
{
	const auto& desc = this->getString(section, key);

	if (desc.empty())
		return default_value;
	else if(desc == "1" || desc == "on" || desc == "yes" || desc == "true")
		return true;
	else if (desc == "0" || desc == "off" || desc == "no" || desc == "false")
		return false;

	return default_value;
}

const char CFGParser::getChar(const std::string& section, const std::string& key, const char default_value) const noexcept
{
	const auto& desc = this->getString(section, key);

	if (desc.empty())
		return default_value;
	else
		return static_cast<char>(std::stoi(desc));
}

const unsigned char CFGParser::getUChar(const std::string& section, const std::string& key, const unsigned char default_value) const noexcept
{
	const auto& desc = this->getString(section, key);

	if (desc.empty())
		return default_value;
	else
		return static_cast<unsigned char>(std::stoi(desc));
}

const short CFGParser::getShort(const std::string& section, const std::string& key, const short default_value) const noexcept
{
	const auto& desc = this->getString(section, key);

	if (desc.empty())
		return default_value;
	else
		return static_cast<short>(std::stoi(desc));
}

const unsigned short CFGParser::getUShort(const std::string& section, const std::string& key, const unsigned short default_value) const noexcept
{
	const auto& desc = this->getString(section, key);

	if (desc.empty())
		return default_value;
	else
		return static_cast<unsigned short>(std::stoi(desc));
}

const int CFGParser::getInt(const std::string& section, const std::string& key, const int default_value) const noexcept
{
	const auto& desc = this->getString(section, key);

	if (desc.empty())
		return default_value;
	else
		return std::stoi(desc);
}

const unsigned int CFGParser::getUInt(const std::string& section, const std::string& key, const unsigned int default_value) const noexcept
{
	const auto& desc = this->getString(section, key);

	if (desc.empty())
		return default_value;
	else
		return static_cast<unsigned int>(std::stoul(desc));
}

const long CFGParser::getLong(const std::string& section, const std::string& key, const long& default_value) const noexcept
{
	const auto& desc = this->getString(section, key);

	if (desc.empty())
		return default_value;
	else
		return std::stol(desc);
}

const unsigned long CFGParser::getULong(const std::string& section, const std::string& key, const unsigned long& default_value) const noexcept
{
	const auto& desc = this->getString(section, key);

	if (desc.empty())
		return default_value;
	else
		return std::stoul(desc);
}

const unsigned long long CFGParser::getULongLong(const std::string& section, const std::string& key, const unsigned long long& default_value) const noexcept
{
	const auto& desc = this->getString(section, key);

	if (desc.empty())
		return default_value;
	else
		return std::stoull(desc);
}

const float CFGParser::getFloat(const std::string& section, const std::string& key, const float& default_value) const noexcept
{
	const auto& desc = this->getString(section, key);

	if (desc.empty())
		return default_value;
	else
		return std::stof(desc);
}

const double CFGParser::getDouble(const std::string& section, const std::string& key, const double& default_value) const noexcept
{
	const auto& desc = this->getString(section, key);

	if (desc.empty())
		return default_value;
	else
		return std::stod(desc);
}

const long double CFGParser::getLongDouble(const std::string& section, const std::string& key, const long double& default_value) const noexcept
{
	const auto& desc = this->getString(section, key);

	if (desc.empty())
		return default_value;
	else
		return std::stold(desc);
}

const std::string& CFGParser::getString(const std::string& section, const std::string& key, const std::string& default_value) const noexcept
{
	const auto& section_iter = this->sectionSearch(section, key);

	if (section_iter != _data.cend())
	{
		const auto& value_iter = section_iter->second.data.find(key);

		if (value_iter != section_iter->second.data.cend())
		{
			return value_iter->second;
		}
		else
		{
			std::cout << "CFG error: Config file \"" << _filename << "\" doesn't have key \"" << key << "\" inside section \"" << section << "\"! Default value will be return." << std::endl;
			return default_value;
		}
	}
	else
	{
		std::cout << "CFG error: Config file \"" << _filename << "\" doesn't have section \"" << section << "\"! Default value will be return." << std::endl;
		return default_value;
	}
}

const Vec2 CFGParser::getVec2(const std::string& section, const std::string& key, const Vec2& default_value) const noexcept
{
	const auto& array = this->getArray<float>(section, key);
	const auto size = array.size();

	if (size != 2u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec2{ array[0], array[1] };
	}
}

const Vec2b CFGParser::getVec2b(const std::string& section, const std::string& key, const Vec2b& default_value) const noexcept
{
	const auto& array = this->getArray<bool>(section, key);
	const auto size = array.size();

	if (size != 2u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec2b{ array[0], array[1] };
	}
}

const Vec2d CFGParser::getVec2d(const std::string& section, const std::string& key, const Vec2d& default_value) const noexcept
{
	const auto& array = this->getArray<double>(section, key);
	const auto size = array.size();

	if (size != 2u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec2d{ array[0], array[1] };
	}
}

const Vec2i CFGParser::getVec2i(const std::string& section, const std::string& key, const Vec2i& default_value) const noexcept
{
	const auto& array = this->getArray<int>(section, key);
	const auto size = array.size();

	if (size != 2u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec2i{ array[0], array[1] };
	}
}

const Vec2u CFGParser::getVec2u(const std::string& section, const std::string& key, const Vec2u& default_value) const noexcept
{
	const auto& array = this->getArray<uint32_t>(section, key);
	const auto size = array.size();

	if (size != 2u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec2u{ array[0], array[1] };
	}
}

const Vec3 CFGParser::getVec3(const std::string& section, const std::string& key, const Vec3& default_value) const noexcept
{
	const auto& array = this->getArray<float>(section, key);
	const auto size = array.size();

	if (size != 3u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec3{ array[0], array[1], array[2] };
	}
}

const Vec3b CFGParser::getVec3b(const std::string& section, const std::string& key, const Vec3b& default_value) const noexcept
{
	const auto& array = this->getArray<bool>(section, key);
	const auto size = array.size();

	if (size != 3u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec3b{ array[0], array[1], array[2] };
	}
}

const Vec3d CFGParser::getVec3d(const std::string& section, const std::string& key, const Vec3d& default_value) const noexcept
{
	const auto& array = this->getArray<double>(section, key);
	const auto size = array.size();

	if (size != 3u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec3d{ array[0], array[1], array[2] };
	}
}

const Vec3i CFGParser::getVec3i(const std::string& section, const std::string& key, const Vec3i& default_value) const noexcept
{
	const auto& array = this->getArray<int>(section, key);
	const auto size = array.size();

	if (size != 3u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec3i{ array[0], array[1], array[2] };
	}
}

const Vec3u CFGParser::getVec3u(const std::string& section, const std::string& key, const Vec3u& default_value) const noexcept
{
	const auto& array = this->getArray<uint32_t>(section, key);
	const auto size = array.size();

	if (size != 3u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec3u{ array[0], array[1], array[2] };
	}
}

const Vec4 CFGParser::getVec4(const std::string& section, const std::string& key, const Vec4& default_value) const noexcept
{
	const auto& array = this->getArray<float>(section, key);
	const auto size = array.size();

	if (size != 4u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec4{ array[0], array[1], array[2], array[3] };
	}
}

const Vec4b CFGParser::getVec4b(const std::string& section, const std::string& key, const Vec4b& default_value) const noexcept
{
	const auto& array = this->getArray<bool>(section, key);
	const auto size = array.size();

	if (size != 4u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec4b{ array[0], array[1], array[2], array[3] };
	}
}

const Vec4d CFGParser::getVec4d(const std::string& section, const std::string& key, const Vec4d& default_value) const noexcept
{
	const auto& array = this->getArray<double>(section, key);
	const auto size = array.size();

	if (size != 4u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec4d{ array[0], array[1], array[2], array[3] };
	}
}

const Vec4i CFGParser::getVec4i(const std::string& section, const std::string& key, const Vec4i& default_value) const noexcept
{
	const auto& array = this->getArray<int>(section, key);
	const auto size = array.size();

	if (size != 4u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec4i{ array[0], array[1], array[2], array[3] };
	}
}

const Vec4u CFGParser::getVec4u(const std::string& section, const std::string& key, const Vec4u& default_value) const noexcept
{
	const auto& array = this->getArray<uint32_t>(section, key);
	const auto size = array.size();

	if (size != 4u)
	{
		std::cout << "CFG error: [" << section << "] : " << key << " have " << size << " parameters." << std::endl;
		return default_value;
	}
	else
	{
		return Vec4u{ array[0], array[1], array[2], array[3] };
	}
}

void CFGParser::setString(const std::string& section, const std::string& key, const std::string& value) noexcept
{
	_data[section].data[key] = value;

	this->writeData();
}

void CFGParser::open(const std::string& filename) noexcept
{
	this->readData();
}

const bool CFGParser::isHaveSection(const std::string& section) const noexcept
{
	return (_data.find(section) != _data.cend());
}

const bool CFGParser::isHaveSectionKey(const std::string& section, const std::string& key) const noexcept
{
	const auto& iter = _data.find(section);

	if (iter != _data.cend())
	{
		const auto& key_iter = iter->second;

		return (key_iter.data.find(key) != key_iter.data.cend());
	}
	else
	{
		std::cout << "Config file \"" << _filename << "\" doesn't have section \"" << section << "\"." << std::endl;
		return false;
	}
}

const size_t CFGParser::getSectionsNum() const noexcept
{
	return _data.size();
}

const size_t CFGParser::getSectionValuesNum(const std::string& section) const noexcept
{
	if (this->isHaveSection(section))
	{
		return _data.at(section).data.size();
	}
	else
	{
		std::cout << "Config file \"" << _filename << "\" doesn't have section \"" << section << "\"." << std::endl;
		return 0u;
	}
}

const CFGParser::CFGData& CFGParser::getData() const noexcept
{
	return _data;
}

void CFGParser::clearData()
{
	for (auto& data : _data)
	{
		data.second.data.clear();
	}

	_data.clear();
}

void CFGParser::readData()
{
	std::fstream file(_filename, std::ios::in);

	if (file.good())
	{
		std::string section, inheritance, key, value, error;
		uint32_t line = 1u;

		ReadMode mode = ReadMode::none;

		const auto& allowed = std::string(ALLOWED_CHARS);

		const auto IsAllowed = [allowed](const char byte) noexcept -> bool
		{
			return std::find(allowed.cbegin(), allowed.cend(), byte) != allowed.cend();
		};

		while (!file.eof())
		{
			const char byte = static_cast<char>(file.get());

			if (byte != '\n' && mode == ReadMode::comment)
				continue;

			switch (byte)
			{
				case ';':
				{
					if (mode != ReadMode::string)
					{
						if (!key.empty() && !value.empty())
						{
							_data[section].data.insert({ key, value });
							key.clear();
							value.clear();
						}

						mode = ReadMode::comment;
					}
					else
					{
						value.push_back(byte);
					}
				}
				break;

				case '[':
				{
					if (mode == ReadMode::string)
					{
						value.push_back(byte);
					}
					else
					{
						mode = ReadMode::section;
						section.clear();
					}
				}
				break;

				case ']':
				{
					if (mode == ReadMode::string)
						value.push_back(byte);
					else
						_data[section] = {};
				}
				break;

				case ':':
				{
					if (mode == ReadMode::string)
					{
						value.push_back(byte);
					}
					else if (mode == ReadMode::section)
					{
						mode = ReadMode::inheritance;
						inheritance.clear();
					}
					else if (mode == ReadMode::value)
					{
						value.push_back(byte);
					}
				}
				break;

				case '=':
				{
					if (mode == ReadMode::string)
						value.push_back(byte);
					else
						mode = ReadMode::value;
				}
				break;

				case ',':
				{
					if (mode == ReadMode::string || mode == ReadMode::value)
					{
						value.push_back(byte);
					}
					else
					{
						mode = ReadMode::error;
						error += " Non-alowed character!";
					}
				}
				break;

				case '.':
				{
					if (mode == ReadMode::string || mode == ReadMode::value)
					{
						value.push_back(byte);
					}
					else
					{
						mode = ReadMode::error;
						error += " Non-alowed character!";
					}
				}
				break;

				case ' ':
				{
					if (mode == ReadMode::string)
						value.push_back(byte);
				}
				break;

				case '\"':
				{
					if (mode != ReadMode::string)
						mode = ReadMode::string;
					else
						mode = ReadMode::value;
				}
				break;

				case '\t':
				{
					if (mode == ReadMode::string)
						value.push_back(byte);
				}
				break;

				case '\n':
				{
					switch (mode)
					{
						case ReadMode::section:
							mode = ReadMode::key;
							key.clear();
						break;

						case ReadMode::inheritance:
							if (!this->isHaveSection(inheritance))
							{
								mode = ReadMode::error;
								error += " Section with name \"" + inheritance + "\" not exist!";
								_data[section].inherited = inheritance;
								key.clear();
							}
							else
							{
								_data[section].inherited = inheritance;
								mode = ReadMode::key;
								key.clear();
							}
						break;

						case ReadMode::comment:
							mode = ReadMode::key;
							key.clear();
						break;

						case ReadMode::key:
							if (byte != '\n')
							{
								mode = ReadMode::error;
								error += " Value are not specified!";
							}
						break;

						case ReadMode::value:
							_data[section].data.insert({ key, value });
							value.clear();
						break;

						case ReadMode::string:
							mode = ReadMode::error;
							error += " Missing of closure double brackets in string!";
						break;

						default:
						break;
					}

					if (mode == ReadMode::string)
					{
						value.push_back(byte);
					}
					else if (mode == ReadMode::error)
					{
						std::cout << "Config syntax error at line " << line << "! Reason:" << error << std::endl;
						error.clear();
						mode = ReadMode::key;
						key.clear();
					}
					else
					{
						mode = ReadMode::key;
						key.clear();
					}

					line++;
				}
				break;

				default:
				{
					switch (mode)
					{
						case ReadMode::section:
							if (IsAllowed(byte))
							{
								section.push_back(byte);
							}
							else
							{
								mode = ReadMode::error;
								error += " Non-alowed character in section name!";
							}
						break;

						case ReadMode::inheritance:
							if (IsAllowed(byte))
							{
								inheritance.push_back(byte);
							}
							else
							{
								mode = ReadMode::error;
								error += " Non-alowed character in inheritance!";
							}
						break;

						case ReadMode::comment:
						break;

						case ReadMode::key:
							if (IsAllowed(byte))
							{
								key.push_back(byte);
							}
							else
							{
								mode = ReadMode::error;
								error += " Non-alowed character in key!";
							}
						break;

						case ReadMode::value:
							if (IsAllowed(byte))
							{
								value.push_back(byte);
							}
							else
							{
								mode = ReadMode::error;
								error += " Non-alowed character!";
							}
						break;

						case ReadMode::string:
							value.push_back(byte);
						break;

						case ReadMode::error:
							std::cout << "Config syntax error at line " << line << "! Reason:" << error << std::endl;
							error.clear();
							mode = ReadMode::none;
						break;

						default:
						break;
					}
				}
				break;
			}

			// EOF fix
			if (file.eof() && (!key.empty() || !value.empty()))
			{
				_data[section].data.insert({ key, value });
				value.clear();
			}
		}
	}
	else
	{
		std::cout << "Cannot open cfg file\"" << _filename << "\". Make sure to file exist." << std::endl;
	}

	file.close();
}

void CFGParser::writeData()
{
	std::fstream file(_filename, std::ios::out | std::ios::trunc);

	if (file.good())
	{
		for (const auto& section_pair : _data)
		{
			std::ostringstream str;
			str << '[' << section_pair.first << ']';

			if (!section_pair.second.inherited.empty())
				str << " : " << section_pair.second.inherited << '\n';
			else
				str << '\n';

			for (const auto& value_pair : section_pair.second.data)
			{
				str << value_pair.first << " = " << value_pair.second << '\n';
			}

			file << str.str() << std::endl;
		}
	}
	else
	{
		std::cout << "Cannot open cfg file\"" << _filename << "\". Make sure to file exist." << std::endl;
	}

	file.close();
}

const CFGParser::CFGValueData& CFGParser::getSectionData(const std::string& section_name) const noexcept
{
	return _data.at(section_name).data;
}

const unsigned int CFGParser::getKeyLineInSection(const std::string& section, const std::string& key) const noexcept
{
	const auto& section_iter = this->sectionSearch(section, key);

	if (section_iter != _data.cend())
	{
		const auto& value_iter = section_iter->second.data.find(key);

		if (value_iter != section_iter->second.data.cend())
		{
			return static_cast<const unsigned int>(std::distance(section_iter->second.data.cbegin(), value_iter));
		}
		else
		{
			std::cout << "CFG error: Config file \"" << _filename << "\" doesn't have key \"" << key << "\" inside section \"" << section << "\"!" << std::endl;
			return 0u;
		}
	}
	else
	{
		std::cout << "CFG error: Config file \"" << _filename << "\" doesn't have section \"" << section << "\"!" << std::endl;
		return 0u;
	}
}

const CFGParser::CFGData::const_iterator CFGParser::sectionSearch(const std::string& section_name, const std::string& key) const noexcept
{
	const auto& section_iter = _data.find(section_name);
	const auto& inherited = section_iter->second.inherited;

	if (!inherited.empty() && !this->isHaveSection(inherited))
		std::cout << "CFG error: Config file \"" << _filename << "\" inherited section \"" << inherited << "\" not exist for section \"" << section_name << "\" inheritance!" << std::endl;

	if (section_iter != _data.cend())
	{
		const auto& value_iter = section_iter->second.data.find(key);

		if (value_iter != section_iter->second.data.cend())
		{
			return section_iter;
		}
		else
		{
			const auto& inherited = section_iter->second.inherited;

			if (!inherited.empty())
				return this->sectionSearch(inherited, key);
			else
				return _data.cend();
		}
	}
	else
	{
		return _data.cend();
	}
}
