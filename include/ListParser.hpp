#ifndef _LIST_PARSER_HPP_
#define _LIST_PARSER_HPP_

#include <string>
#include <vector>
#include <unordered_map>


class ListParser final
{
public:
	ListParser(const std::string& filepath);
	~ListParser();

	const std::vector<std::string>& getList(const std::string& section) const;
	const std::unordered_map<std::string, std::vector<std::string>>& getData() const;

	const bool isSectionExist(const std::string& section) const;

protected:
	void readData();

private:
	std::unordered_map<std::string, std::vector<std::string>> _data;
	std::string _filepath;

};

#endif
