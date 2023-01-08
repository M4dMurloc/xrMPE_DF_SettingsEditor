#ifndef _SETTINGS_EDITOR_FRAME_HPP_
#define _SETTINGS_EDITOR_FRAME_HPP_

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "wx/frame.h"
#include "Types.hpp"

class wxMenuBar;
class wxPanel;
class wxAuiNotebook;
class wxSplitterWindow;
class wxTextCtrl;
class wxNotebook;
class wxStaticText;
class wxSpinCtrl;
class wxCheckBox;
class wxChoice;
class wxCheckListBox;
class wxBoxSizer;
class wxListBox;
class wxScrolledWindow;

class VariantPanel;
class Settings;


class SettingsFrame final : public wxFrame
{
public:

	SettingsFrame(wxWindow* parent = nullptr,
				  wxWindowID id = wxID_ANY,
				  const wxString& title = wxT("Settings Editor"),
				  const wxPoint& pos = wxDefaultPosition,
				  const wxSize& size = wxSize(1024, 720),
				  long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
	~SettingsFrame();

	wxTextCtrl* getLog() const noexcept { return _log; }

	void msg(const std::string& message) noexcept;

protected:
	void createForm();

	void connectActions();

	void clearUI();

	void addVariantItem(const SettingType type);

	std::vector<VariantData> getCurrentVariantList(const SettingType type) const;

	void fillCurrentVariantList(const SettingType type, wxListBox* variant_list);
	void fillCurrentWaveList(const SettingType type);

	void setShowVariantsSplitterWindow(const SettingType type, const bool visible);

	void _provision_splitterOnIdle(wxIdleEvent&);
	void _npc_splitterOnIdle(wxIdleEvent&);
	void _provision_common_splitterOnIdle(wxIdleEvent&);
	void _npc_common_splitterOnIdle(wxIdleEvent&);

private:
	enum Actions : uint32_t
	{
		FILE_OPEN_ID = 2000,
		FILE_SAVE_ID,
		FILE_SAVE_AS,
		FILE_EXIT_ID,
		VIEW_SHOW_LOG_ID,
		HELP_ABOUT_ID,
		PROVISION_VARIANTS_PAGE_ID,
		PROVISION_WAVES_PAGE_ID,
		NPC_VARIANTS_PAGE_ID,
		NPC_WAVES_PAGE_ID,

		POPUP_ADD_VARIANT,
		POPUP_REMOVE_VARIANT,
		POPUP_RENAME_VARIANT,

		POPUP_ADD_WAVE,
		POPUP_REMOVE_WAVE,
		POPUP_WAVE_MOVE_UP,
		POPUP_WAVE_MOVE_DOWN,

		POPUP_WAVE_SELECT_VARIANT,
		POPUP_WAVE_REMOVE_VARIANT,

		POPUP_ADD_ITEM,
		POPUP_RANDOM_CREATE_ITEMS
	};

	//////////////////////////////////////////////////////

	friend class Settings;

	static uint32_t provision_variant_index;
	static uint32_t npc_variant_index;

	std::unique_ptr<Settings> _settings;

	int _current_selection = -1;
	bool _editable_items = true;

	//////////////////////////////////////////////////////

	wxBoxSizer* _general_sizer = nullptr;
	wxBoxSizer* _provision_common_sizer = nullptr;
	wxBoxSizer* _npc_common_sizer = nullptr;
	wxBoxSizer* _prov_items_scroll_sizer = nullptr;
	wxBoxSizer* _npc_items_scroll_sizer = nullptr;

	wxMenuBar* _menubar = nullptr;
	wxMenu* _menu_file = nullptr;
	wxMenu* _menu_view = nullptr;
	wxMenu* _menu_help = nullptr;
	wxTextCtrl* _log = nullptr;
	wxPanel* _general_panel = nullptr;
	wxAuiNotebook* _notebook = nullptr;
	wxPanel* _common_page = nullptr;
	wxStaticText* _staticText1 = nullptr;
	wxSpinCtrl* _spawn_delay_spin = nullptr;
	wxStaticText* _staticText2 = nullptr;
	wxSpinCtrl* _start_time_spin = nullptr;
	wxStaticText* _staticText3 = nullptr;
	wxCheckBox* _surge_chk = nullptr;
	wxStaticText* _staticText4 = nullptr;
	wxCheckBox* _phantoms_chk = nullptr;
	wxStaticText* _staticText5 = nullptr;
	wxChoice* _logo_choice = nullptr;
	wxStaticText* _staticText6 = nullptr;
	wxSpinCtrl* _message_menu_spin = nullptr;
	wxStaticText* _staticText7 = nullptr;
	wxTextCtrl* _spawn_particles_ctrl = nullptr;
	wxStaticText* _staticText8 = nullptr;
	wxTextCtrl* _spawn_sound_ctrl = nullptr;
	wxStaticText* _staticText9 = nullptr;
	wxTextCtrl* _skins_ctrl = nullptr;
	wxPanel* _provision_page = nullptr;
	wxSplitterWindow* _provision_splitter = nullptr;
	wxPanel* _provision_tab_panel = nullptr;
	wxNotebook* _provision_type_notebook = nullptr;
	wxPanel* _provision_variants_page = nullptr;
	wxListBox* _provision_variants_list = nullptr;
	wxPanel* _provision_waves_page = nullptr;
	wxListBox* _provision_waves_list = nullptr;
	wxPanel* _provision_common_panel = nullptr;
	wxSplitterWindow* _provision_common_splitter = nullptr;
	wxScrolledWindow* _provision_variants_scroll = nullptr;
	wxListBox* _prov_waves_variants_list = nullptr;
	wxScrolledWindow* _provision_items_scroll = nullptr;
	wxPanel* _npc_page = nullptr;
	wxSplitterWindow* _npc_splitter = nullptr;
	wxPanel* _npc_tab_panel = nullptr;
	wxNotebook* _npc_type_notebook = nullptr;
	wxPanel* _npc_variants_page = nullptr;
	wxListBox* _npc_variants_list = nullptr;
	wxPanel* _npc_waves_page = nullptr;
	wxListBox* _npc_waves_list = nullptr;
	wxPanel* _npc_common_panel = nullptr;
	wxSplitterWindow* _npc_common_splitter = nullptr;
	wxScrolledWindow* _npc_variants_scroll = nullptr;
	wxListBox* _npc_waves_variants_list = nullptr;
	wxScrolledWindow* _npc_items_scroll = nullptr;

};

#endif
