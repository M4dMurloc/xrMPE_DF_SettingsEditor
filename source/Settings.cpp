#include "Settings.hpp"
#include "Frame.hpp"
#include "CFGParser.hpp"
#include "ListParser.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "wx_includes.hpp"



Settings::Settings(SettingsFrame* frame) noexcept : _frame(frame)
{
	this->readListsAndSetupUI();
}

Settings::~Settings() noexcept
{
}

void Settings::parsingError(const std::string& section, const std::string& key, const std::string& message) noexcept
{
	std::cout << "Parsing error! Section \"" << section << "\", key \"" << key << "\": " << message << std::endl;
}

void Settings::readConfigs()
{
	CFGParser cfg(_current_file);

	{
		_spawn_delay = cfg.getInt(_data_section_section, _spawn_delay_key);
		_start_time = cfg.getInt(_data_section_section, _start_time_key);

		_spawn_particles = cfg.getString(_data_section_section, _spawn_particles_key);
		_spawn_sound = cfg.getString(_data_section_section, _spawn_sound_key);

		_surge = cfg.getBool(_mod_section_section, _surge_key);
		_phantoms = cfg.getBool(_mod_section_section, _phantoms_key);

		_logo = cfg.getString(_team_section_section, _logo_key);
		_messages_menu_faction = cfg.getInt(_team_section_section, _messages_faction_key);

		for (const auto& skin : cfg.getStringArray(_team_section_section, _skin_key))
		{
			if (auto iter = _skins.find(skin); iter != _skins.cend())
				iter->second = true;
			else
				std::cout << "Skin \"" << skin << "\" doesn't exist inside skins list!" << std::endl;
		}
	}

	const auto ReadVariants = [&](std::map<std::string, Variant>& variants, const std::string& section) -> void
	{
		for (const auto& pair : cfg.getSectionData(section))
		{
			const auto& variants_array = cfg.getStringArray(section, pair.first);

			if (variants_array.empty())
			{
				std::cout << "\"" << section << "\" variant \"" << pair.first << "\" is empty!" << std::endl;
				continue;
			}

			variants[pair.first] = {};
			Variant& variant = variants[pair.first];

			variant.name = pair.first;

			for (const auto& var_str : variants_array)
			{
				VariantData& data = variant.data.emplace_back();

				data = this->parseItemString(var_str, section, pair.first);
			}
		}
	};

	const auto ReadWaves = [&](std::map<std::string, Wave>& waves, const std::map<std::string, Variant>& variants, const std::string& section) -> void
	{
		for (const auto& pair : cfg.getSectionData(section))
		{
			const auto& waves_array = cfg.getStringArray(section, pair.first);

			if (waves_array.empty())
			{
				std::cout << "\"" << section << "\" wave \"" << pair.first << "\" is empty!" << std::endl;
				continue;
			}

			waves[pair.first] = {};
			Wave& wave = waves[pair.first];

			wave.name = pair.first;

			for (const auto& variant_name : waves_array)
			{
				const auto iter = variants.find(variant_name);

				if (iter != variants.cend())
					wave.variants[variant_name] = (&iter->second);
				else
					_unexisted.push_back('[' + section + "] : " + wave.name + " - " + variant_name);
			}
		}
	};

	// Checking for valid file
	{
		const std::vector<std::string> all_sections
		{
			"defence_gamedata",
			"defence_modifiers",
			"defence_team",
			"wave_provisions",
			"wave_provisions_variants",
			"monster_waves",
			"monster_wave_variants"
		};

		bool is_valid = true;

		for (const auto& section : all_sections)
			is_valid = cfg.isHaveSection(section);

		if (!is_valid)
		{
			const std::string& msg = "Cannot open \"" + _current_file + "\" - file is not valid.";

			_frame->msg(msg);

			wxMessageBox(msg, "Hey!");

			return;
		}
	}

	try
	{
		ReadVariants(_provision_variants, _wave_provisions_variants_section);
		ReadVariants(_npc_variants, _monster_wave_variants_section);

		ReadWaves(_provision_waves, _provision_variants, _wave_provisions_section);
		ReadWaves(_npc_waves, _npc_variants, _monster_waves_section);
	}
	catch (const std::exception& exception)
	{
		std::cout << "Parsing error: " << exception.what() << std::endl;
	}
}

void Settings::setupUI()
{
	_frame->_spawn_delay_spin->SetValue(_spawn_delay);
	_frame->_start_time_spin->SetValue(_start_time);

	_frame->_surge_chk->SetValue(_surge);
	_frame->_phantoms_chk->SetValue(_phantoms);

	_frame->_spawn_particles_ctrl->SetValue(_spawn_particles);
	_frame->_spawn_sound_ctrl->SetValue(_spawn_sound);


	{
		const int current_index = _frame->_logo_choice->FindString(_logo, true);
		_frame->_logo_choice->SetSelection(current_index);
	}

	_frame->_message_menu_spin->SetValue(_messages_menu_faction);

	{
		uint32_t count = 0u;
		wxString str;

		for (const auto& skin_pair : _skins)
		{
			if (skin_pair.second)
			{
				if (count > 0)
					str += ", ";

				str += skin_pair.first;

				++count;
			}
		}

		_frame->_skins_ctrl->SetValue(str);
		_frame->_skins_ctrl->SetToolTip(str);
	}

	for (const auto& pair : _provision_variants)
		_frame->_provision_variants_list->Append(pair.first);

	for (const auto& pair : _npc_variants)
		_frame->_npc_variants_list->Append(pair.first);

	for (const auto& pair : _provision_waves)
		_frame->_provision_waves_list->Append(pair.first);

	for (const auto& pair : _npc_waves)
		_frame->_npc_waves_list->Append(pair.first);
}

void Settings::getCommonSettingsFromUI()
{
	_spawn_delay = _frame->_spawn_delay_spin->GetValue();
	_start_time = _frame->_start_time_spin->GetValue();

	_spawn_particles = _frame->_spawn_particles_ctrl->GetValue();
	_spawn_sound = _frame->_spawn_sound_ctrl->GetValue();

	_surge = _frame->_surge_chk->GetValue();
	_phantoms = _frame->_phantoms_chk->GetValue();

	_logo = _frame->_logo_choice->GetStringSelection();
	_messages_menu_faction = _frame->_message_menu_spin->GetValue();
}

void Settings::readListsAndSetupUI()
{
	_frame->msg("Reading of lists...");

	{
		if (!std::filesystem::is_directory("data/"))
			wxMessageBox("Directory \"data\" with \"list.ltx\" is not exist!\nThis directory must be placed inside folder with \"SettingsEditor.exe\".", "Hey!");

		ListParser list_cfg(_storage_cfg_path);

		if (list_cfg.isSectionExist(_storage_logos_section))
		{
			_logos = list_cfg.getList(_storage_logos_section);

			if (_logos.empty())
				_frame->msg("Logos list is empty!");
		}
		else
		{
			wxMessageBox("List \"" + std::string(_storage_logos_section) +
						 "\" not found inside\nlist config \"" + std::string(_storage_cfg_path) +
						 "\"!", "List not found!", wxICON_WARNING);
		}

		if (list_cfg.isSectionExist(_storage_provision_section))
		{
			_provisions = list_cfg.getList(_storage_provision_section);

			if (_provisions.empty())
				_frame->msg("Provision list is empty!");
		}
		else
		{
			wxMessageBox("List \"" + std::string(_storage_provision_section) +
						 "\" not found inside\nlist config \"" + std::string(_storage_cfg_path) +
						 "\"!", "List not found!", wxICON_WARNING);
		}

		if (list_cfg.isSectionExist(_storage_monsters_section))
		{
			_monsters = list_cfg.getList(_storage_monsters_section);

			if (_monsters.empty())
				_frame->msg("Provision list is empty!");
		}
		else
		{
			wxMessageBox("List \"" + std::string(_storage_monsters_section) +
						 "\" not found inside\nlist config \"" + std::string(_storage_cfg_path) +
						 "\"!", "List not found!", wxICON_WARNING);
		}

		if (list_cfg.isSectionExist(_storage_skins_section))
		{
			for (const auto& skin : list_cfg.getList(_storage_skins_section))
				_skins[skin] = false;

			if (_skins.empty())
				_frame->msg("Skins list is empty!");
		}
		else
		{
			wxMessageBox("List \"" + std::string(_storage_skins_section) +
						 "\" not found inside\nlist config \"" + std::string(_storage_cfg_path) +
						 "\"!", "List not found!", wxICON_WARNING);
		}
	}

	for (const auto& logo : _logos)
		_frame->_logo_choice->Insert(logo, _frame->_logo_choice->GetCount());

	_frame->msg("Lists reading done.");
}

VariantData Settings::parseItemString(const std::string& str, const std::string& section, const std::string& key)
{
	VariantData data;

	try
	{
		std::istringstream stream(str);
		std::vector<std::string> items;

		for (std::string temp; std::getline(stream, temp, ':');)
			items.push_back(temp);

		// Avoid of '::'
		[[maybe_unused]] const auto iter = std::remove_if(items.begin(), items.end(), [](const std::string& str)
		{
			return str.empty();
		});

		if(items.size() < 2)
			this->parsingError(section, key, "Value doesn't have a nessesary parameters!");

		data.desc = items[0u];
		data.spawn_number = std::stoi(items[1u]);

		// Optional
		if (items.size() > 2u)
			data.spawn_prop = std::stof(items[2u]);

		// Optional bool
		if (items.size() > 3u)
			data.ignore_players_number = (items[3u] == "true" || items[3u] == "1" || items[3u] == "yes") ? true : false;
	}
	catch (const std::exception& exception)
	{
		this->parsingError(section, key, exception.what());
	}

	return data;
}

void Settings::clearAll()
{
	_frame->clearUI();

	_provision_variants.clear();
	_npc_variants.clear();
	_provision_waves.clear();
	_npc_waves.clear();
}

const bool Settings::isHaveData() const
{
	return (!_provision_variants.empty() || !_npc_variants.empty() || !_provision_waves.empty() || !_npc_waves.empty());
}

std::vector<VariantData> Settings::getCurrentVariantList(const SettingType type) const
{
	return _frame->getCurrentVariantList(type);
}

void Settings::load(const std::string& path)
{
	_current_file = path;

	this->readConfigs();

	// Check
	if (!_unexisted.empty())
	{
		std::ostringstream stream;

		for (const auto& item : _unexisted)
			stream << item << '\n';

		wxMessageBox(stream.str(), "Some items is missed...", wxOK);

		_unexisted.clear();
	}

	this->setupUI();

	std::cout << "Config file \"" << _current_file << "\" was successfully readed." << std::endl;
}

template<typename T>
inline void write(std::fstream& file, const std::string& key, const T& value)
{
	file << key << " = " << value << std::endl;
}

inline void write(std::fstream& file, const std::string& key, const bool value)
{
	file << key << " = " << (value ? "true" : "false") << std::endl;
}

inline void write_section(std::fstream& file, const std::string& section)
{
	file << '[' << section << ']' << std::endl;
}

inline void big_separator(std::fstream& file, const std::string& desc, const bool first_endl = true)
{
	if (first_endl)
		file << std::endl;

	file << ';' << std::string(55u, '-') << std::endl;

	const size_t len = (55u - desc.length());
	const size_t actually_len = len / 2u;
	const size_t add = len % 2u;

	file << ';' << std::string((add > 0u) ? (actually_len + add) : actually_len, '-') << desc << std::string(actually_len, '-') << std::endl;

	file << ';' << std::string(55u, '-') << std::endl;
	file << std::endl;
}

inline void separator(std::fstream& file, const size_t number = 1u)
{
	file << std::string(number, '\n') << std::endl;
}

/**
	That's little wretch makes big work - writes whole container to cfg file with predicate help.
	FUNC must be:
	write_container(file, container, [](const container::value& val) -> std::string
	{
	});
*/
template<typename T, typename FUNC>
inline void write_container(std::fstream& file, const T& container, FUNC&& func)
{
	for (const auto& val : container)
		write(file, val.first, func(val.second));
}

/**
	FUNC must be:
	write_container(file, container, [](const container::value& val) -> std::string
	{
	});
*/
template<typename T, typename FUNC>
inline std::string make_str(const T& container, FUNC&& func)
{
	std::string str;

	for (const auto& val : container)
	{
		if (!str.empty())
			str += ", " + func(val);
		else
			str += func(val);
	}

	return str;
}

void Settings::save(const std::string& path)
{
	if (!path.empty())
		_current_file = path;

	if (_current_file.empty())
		return;

	std::fstream file(_current_file, std::ios::out | std::ios::trunc);

	if (!file.good())
	{
		wxMessageBox("Cannot open \"" + _current_file + "\" to save.", "Config write error");
		return;
	}

	{
		this->getCommonSettingsFromUI();

		big_separator(file, "GLOBAL SETTINGS", false);

		write_section(file, _data_section_section);
		{
			write(file, _spawn_delay_key, _spawn_delay);
			write(file, _start_time_key, _start_time);

			if (_spawn_particles.length() > 0)
			{
				write(file, _spawn_particles_key, _spawn_particles);
			}

			if (_spawn_sound.length() > 0)
			{
				write(file, _spawn_sound_key, _spawn_sound);
			}
		}

		separator(file);

		write_section(file, _mod_section_section);
		{
			write(file, _surge_key, _surge);
			write(file, _phantoms_key, _phantoms);
		}

		separator(file);

		write_section(file, _team_section_section);
		{
			write(file, _logo_key, _logo);
			write(file, _messages_faction_key, _messages_menu_faction);

			{
				std::string skin_str;

				for (const auto& pair : _skins)
				{
					if (pair.second)
					{
						if (!skin_str.empty())
							skin_str += ", " + pair.first;
						else
							skin_str += pair.first;
					}
				}

				write(file, _skin_key, skin_str);
			}
		}

		big_separator(file, "ITEM SPAWN");

		const auto write_waves = [&file](const std::string& section_name, const std::map<std::string, Wave>& waves)
		{
			write_section(file, section_name);
			{
				write_container(file, waves, [](const Wave& wave) -> std::string
				{
					return make_str(wave.variants, [](const std::pair<std::string, const Variant*>& pair) -> std::string
					{
						return pair.first;
					});
				});
			}
		};

		const auto write_variants = [&file](const std::string& section_name, const std::map<std::string, Variant>& variants)
		{
			write_section(file, section_name);
			{
				write_container(file, variants, [](const Variant& variant) -> std::string
				{
					return make_str(variant.data, [](const VariantData& data) -> std::string
					{
						std::ostringstream stream;
						stream << data.desc << ':' << data.spawn_number << ':' << data.spawn_prop;

						if (data.ignore_players_number)
							stream << ":true";

						return stream.str();
					});
				});
			}
		};

		write_waves(_wave_provisions_section, _provision_waves);

		separator(file);

		write_variants(_wave_provisions_variants_section, _provision_variants);

		big_separator(file, "MONSTERS SPAWN");

		write_waves(_monster_waves_section, _npc_waves);

		separator(file);

		write_variants(_monster_wave_variants_section, _npc_variants);
	}

	file.close();
}

void Settings::setSkins(const std::map<std::string, bool>& skins)
{
	_skins = skins;
}

void Settings::setVariant(const SettingType type, const Variant& variant)
{
	switch (type)
	{
		case SettingType::PROVISION:
		{
			const auto iter = std::find_if(_provision_variants.begin(), _provision_variants.end(), [&](const std::pair<std::string, Variant>& data)
			{
				return (data.first == variant.name);
			});

			if (iter != _provision_variants.end())
				iter->second.data = variant.data;
			else
				_provision_variants[variant.name] = variant;
		}
		break;

		case SettingType::NPC:
		{
			const auto iter = std::find_if(_npc_variants.begin(), _npc_variants.end(), [&](const std::pair<std::string, Variant>& data)
			{
				return (data.first == variant.name);
			});

			if (iter != _npc_variants.end())
				iter->second.data = variant.data;
			else
				_npc_variants[variant.name] = variant;
		}
		break;

		default:
			_frame->msg("WTF?");
		break;
	}
}

void Settings::addVariant(const SettingType type, const Variant& variant)
{
	switch (type)
	{
		case SettingType::PROVISION:
			_provision_variants[variant.name] = variant;
		break;

		case SettingType::NPC:
			_npc_variants[variant.name] = variant;
		break;
	}
}

void Settings::addWave(const SettingType type, const Wave& wave)
{
	switch (type)
	{
		case SettingType::PROVISION:
			_provision_waves[wave.name] = wave;
		break;

		case SettingType::NPC:
			_npc_waves[wave.name] = wave;
		break;
	}
}

Wave& Settings::getWave(const SettingType type, const std::string& name)
{
	switch (type)
	{
		case SettingType::PROVISION:
			return _provision_waves[name];
		break;

		case SettingType::NPC:
			return _npc_waves[name];
		break;

		default:
			throw std::exception("WTF?");
		break;
	}
}

void Settings::removeVariantFromWave(const SettingType type, const std::string& wave_name, const std::string& variant_name)
{
	switch (type)
	{
		case SettingType::PROVISION:
		{
			if (const auto wave_iter = _provision_waves.find(wave_name); wave_iter != _provision_waves.cend())
				wave_iter->second.variants.erase(variant_name);
		}
		break;

		case SettingType::NPC:
		{
			if (const auto wave_iter = _npc_waves.find(wave_name); wave_iter != _npc_waves.cend())
				wave_iter->second.variants.erase(variant_name);
		}
		break;

		default:
			throw std::exception("WTF?");
		break;
	}
}

void Settings::removeVariant(const SettingType type, const std::string& variant_name)
{
	switch (type)
	{
		case SettingType::PROVISION:
		{
			_provision_variants.erase(variant_name);

			for (auto& pair : _provision_waves)
				pair.second.variants.erase(variant_name);
		}
		break;

		case SettingType::NPC:
		{
			_npc_variants.erase(variant_name);

			for (auto& pair : _npc_waves)
				pair.second.variants.erase(variant_name);
		}
		break;

		default:
			throw std::exception("WTF?");
		break;
	}
}

void Settings::renameVariant(const SettingType type, const std::string& old_name, const std::string& new_name)
{
	switch (type)
	{
		case SettingType::PROVISION:
		{
			_provision_variants[new_name] = _provision_variants.at(old_name);
			_provision_variants.erase(old_name);

			for (auto& pair : _provision_waves)
			{
				auto& vars = pair.second.variants;

				vars[new_name] = &_provision_variants.at(new_name);
				vars.erase(old_name);
			}
		}
		break;

		case SettingType::NPC:
		{
			_npc_variants[new_name] = _npc_variants.at(old_name);
			_npc_variants.erase(old_name);

			for (auto& pair : _npc_waves)
			{
				auto& vars = pair.second.variants;

				vars[new_name] = &_npc_variants.at(new_name);
				vars.erase(old_name);
			}
		}
		break;

		default:
			throw std::exception("WTF?");
		break;
	}
}
