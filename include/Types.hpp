#ifndef _TYPES_HPP_
#define _TYPES_HPP_

#include <string>
#include <vector>
#include <map>


enum class SettingType : uint32_t
{
	PROVISION,
	NPC
};

struct VariantData final
{
	std::string desc;
	int spawn_number = 0;
	float spawn_prop = 1.0f;
	bool ignore_players_number = false;
};

struct Variant final
{
	std::string name;
	std::vector<VariantData> data;
};

struct Wave final
{
	std::string name;
	std::map<std::string, const Variant*> variants;
};

#endif
