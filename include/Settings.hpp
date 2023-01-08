#ifndef _SETTINGS_HPP_
#define _SETTINGS_HPP_

#include "Types.hpp"
#include <map>


class SettingsFrame;


class Settings final
{
public:
	Settings(SettingsFrame* frame) noexcept;
	~Settings() noexcept;

	void load(const std::string& path);
	void save(const std::string& path = {});

	void setSkins(const std::map<std::string, bool>& skins);

	void setVariant(const SettingType type, const Variant& variant);

	const std::map<std::string, bool>& getSkins() const { return _skins; }

	const std::vector<std::string>& getProvisionList() const { return _provisions; }
	const std::vector<std::string>& getMonstersList() const { return _monsters; }

	void addVariant(const SettingType type, const Variant& variant);
	void addWave(const SettingType type, const Wave& wave);

	const std::map<std::string, Variant>& getProvisionVariants() const { return _provision_variants; }
	const std::map<std::string, Variant>& getNPCVariants() const { return _npc_variants; }

	const std::map<std::string, Wave>& getProvisionWaves() const { return _provision_waves; }
	const std::map<std::string, Wave>& getNPCWaves() const { return _npc_waves; }

	Wave& getWave(const SettingType type, const std::string& name);

	void removeVariantFromWave(const SettingType type, const std::string& wave_name, const std::string& variant_name);
	void removeVariant(const SettingType type, const std::string& variant_name);
	void renameVariant(const SettingType type, const std::string& old_name, const std::string& new_name);

	void clearAll();

	const bool isHaveData() const;
	const bool isHaveFile() const { return !_current_file.empty(); }

	SettingsFrame* getFrame() const { return _frame; }

	std::vector<VariantData> getCurrentVariantList(const SettingType type) const;

protected:
	VariantData parseItemString(const std::string& str, const std::string& section, const std::string& key);

	void parsingError(const std::string& section, const std::string& key, const std::string& message) noexcept;

	void readConfigs();
	void setupUI();

	void getCommonSettingsFromUI();

	void readListsAndSetupUI();

private:
	SettingsFrame* _frame = nullptr;
	std::string _current_file;

	std::vector<std::string> _unexisted;

	std::map<std::string, Variant> _provision_variants;
	std::map<std::string, Variant> _npc_variants;

	std::map<std::string, Wave> _provision_waves;
	std::map<std::string, Wave> _npc_waves;

	int _spawn_delay = 0u;
	int _start_time = 0u;

	std::string _spawn_particles = "";
	std::string _spawn_sound = "";

	bool _surge = false;
	bool _phantoms = false;

	std::string _logo;
	int _messages_menu_faction = 0;

	// <name, used>
	std::map<std::string, bool> _skins;

	std::vector<std::string> _logos;
	std::vector<std::string> _provisions;
	std::vector<std::string> _monsters;

	static constexpr const char* _storage_cfg_path {"data/lists.ltx"};

	static constexpr const char* _storage_logos_section {"logos"};
	static constexpr const char* _storage_provision_section {"provision"};
	static constexpr const char* _storage_monsters_section {"monsters"};
	static constexpr const char* _storage_skins_section {"skins"};

	static constexpr const char* _data_section_section {"defence_gamedata"};
	static constexpr const char* _mod_section_section {"defence_modifiers"};
	static constexpr const char* _team_section_section {"defence_team"};

	static constexpr const char* _spawn_delay_key {"spawn_delay"};
	static constexpr const char* _start_time_key {"start_time"};

	static constexpr const char* _spawn_particles_key {"spawn_particles"};
	static constexpr const char* _spawn_sound_key {"spawn_sound"};

	static constexpr const char* _surge_key {"surge"};
	static constexpr const char* _phantoms_key {"phantoms"};

	static constexpr const char* _logo_key {"logo"};
	static constexpr const char* _messages_faction_key {"messages_menu_faction"};
	static constexpr const char* _skin_key {"skins"};

	static constexpr const char* _wave_provisions_section {"wave_provisions"};
	static constexpr const char* _wave_provisions_variants_section {"wave_provisions_variants"};
	static constexpr const char* _monster_waves_section {"monster_waves"};
	static constexpr const char* _monster_wave_variants_section {"monster_wave_variants"};

};

#endif
