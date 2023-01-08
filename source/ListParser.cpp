#include "ListParser.hpp"
#include <wx/msgdlg.h>

#include <array>
#include <string_view>
#include <iostream>
#include <fstream>

// Don't change this
static constexpr std::string_view comment {"//"};
static constexpr std::array<std::string_view, 2u> multiline_comment {"/*", "*/"};

ListParser::ListParser(const std::string& filepath) : _filepath(filepath)
{
	try
	{
		this->readData();
	}
	catch (const std::exception& exception)
	{
		wxMessageBox("List parsing error! Reason: " + std::string(exception.what()), "Unhandled exception", wxICON_ERROR);
	}
}

ListParser::~ListParser()
{
}

const std::vector<std::string>& ListParser::getList(const std::string& section) const
{
	if (_data.find(section) != _data.cend())
	{
		return _data.at(section);
	}
	else
	{
		std::cout << "Cannot find section \"" << section << "\" inside file \"" << _filepath << "\"." << std::endl;
		return _data.cbegin()->second;
	}
}

const std::unordered_map<std::string, std::vector<std::string>>& ListParser::getData() const
{
	return _data;
}

const bool ListParser::isSectionExist(const std::string& section) const
{
	return (_data.find(section) != _data.cend());
}

void ListParser::readData()
{
	std::fstream file(_filepath, std::ios::in);

	if (file.good())
	{
		// Return empty string if get error
		const auto GetStructureName = [](const std::string& line) -> std::string
		{
			std::string name;

			if (line[0] == '[')
			{
				const size_t pos = line.find(']');

				if (pos != std::string::npos)
					name = (line.substr(1u, line.length() - 2u));
			}

			return name;
		};

		bool comment_process = false;
		bool deferred_comment_process = false;

		const auto RemoveSpacesAndComments = [&](std::string& str) -> void
		{
			for (auto iter = str.cbegin(); iter != str.cend(); ++iter)
			{
				const auto CheckCharPair = [&](const std::string_view& pair) -> const bool
				{
					const auto& next = std::next(iter);
					return (*iter == pair.front() && (next != str.cend() && *next == pair.back()));
				};

				if (CheckCharPair(comment))
				{
					str.erase(iter, str.cend());
					break;
				}
				else if (CheckCharPair(multiline_comment.front()))
				{
					// it's for single-line /* something */
					if (str.find(multiline_comment.back()) != std::string::npos)
					{
						str.erase(iter, str.cend());
						break;
					}

					str.erase(iter, str.cend());

					if (str.empty())
						comment_process = true;
					else
						deferred_comment_process = true;

					break;
				}
				else if (CheckCharPair(multiline_comment.back()))
				{
					comment_process = false;
					std::advance(iter, multiline_comment.back().size());
					str.erase(str.cbegin(), iter);

					iter = str.cbegin();

					if (str.empty())
						break;
				}
				else if (!comment_process && (*iter == ' ' || *iter == '\t'))
				{
					str.erase(iter);

					--iter;
				}
			}
		};

		std::string temp, section_name;
		bool list_process = false;

		while (!file.eof())
		{
			std::getline(file, temp);

			RemoveSpacesAndComments(temp);

			if (comment_process || temp.empty())
				continue;

			if (temp[0] == '{')
			{
				list_process = true;
			}
			else if (temp[0] == '}')
			{
				list_process = false;
			}
			else if (list_process)
			{
				_data[section_name].push_back(temp);
			}
			else if (const auto& section = GetStructureName(temp); !section.empty())
			{
				section_name = section;
			}

			if (deferred_comment_process)
			{
				comment_process = true;
				deferred_comment_process = false;
			}
		}
	}
	else
	{
		std::cout << "Cannot open list file \"" << _filepath << "\"!" << std::endl;
	}

	file.close();
}
