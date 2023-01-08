#include "Frame.hpp"

#include "wx_includes.hpp"

#include "Settings.hpp"
#include "SelectItemDialog.hpp"
#include "VariantPanel.hpp"
#include "VariantSelectDialog.hpp"
#include "RandomizeItemsDialog.hpp"

#include <iostream>
#include <sstream>
#include <random>

/////////////////////////////////////////////////

static constexpr int LIST_FONT_SIZE = 12;

wxFont CreateFont() { return wxFont(LIST_FONT_SIZE, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString); }

/////////////////////////////////////////////////

uint32_t SettingsFrame::provision_variant_index = 0u;
uint32_t SettingsFrame::npc_variant_index = 0u;


SettingsFrame::SettingsFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) :
	wxFrame(parent, id, title, pos, size, style)
{
	this->createForm();

	this->connectActions();

	_settings = std::make_unique<Settings>(this);
}

SettingsFrame::~SettingsFrame()
{
}

void SettingsFrame::createForm()
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	const auto& system_color = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
	this->SetBackgroundColour(system_color);

	_menubar = new wxMenuBar(0);
	_menu_file = new wxMenu();

	wxMenuItem* _menu_file_open = new wxMenuItem(_menu_file, FILE_OPEN_ID, wxString(utf8("Open")) + wxString('\t') + utf8("CTRL+O"), wxEmptyString, wxITEM_NORMAL);
	_menu_file->Append(_menu_file_open);

	_menu_file->AppendSeparator();

	wxMenuItem* _menu_file_save = new wxMenuItem(_menu_file, FILE_SAVE_ID, wxString(utf8("Save")) + wxString('\t') + utf8("CTRL+S"), wxEmptyString, wxITEM_NORMAL);
	_menu_file->Append(_menu_file_save);

	wxMenuItem* _menu_file_save_as = new wxMenuItem(_menu_file, FILE_SAVE_AS, wxString(utf8("Save As...")), wxEmptyString, wxITEM_NORMAL);
	_menu_file->Append(_menu_file_save_as);

	_menu_file->AppendSeparator();

	wxMenuItem* _menu_file_exit = new wxMenuItem(_menu_file, FILE_EXIT_ID, wxString(utf8("Exit")) + wxString('\t') + utf8("ALT+F4"), wxEmptyString, wxITEM_NORMAL);
	_menu_file->Append(_menu_file_exit);

	_menubar->Append(_menu_file, utf8("File"));

	_menu_view = new wxMenu();

	wxMenuItem* _menu_view_show_log = new wxMenuItem(_menu_view, VIEW_SHOW_LOG_ID, wxString(utf8("Show log")) + wxString('\t') + utf8("`"), wxEmptyString, wxITEM_NORMAL);
	_menu_view->Append(_menu_view_show_log);

	_menubar->Append(_menu_view, utf8("View"));

	_menu_help = new wxMenu();

	wxMenuItem* _menu_help_about = new wxMenuItem(_menu_help, HELP_ABOUT_ID, wxString(utf8("About...")), wxEmptyString, wxITEM_NORMAL);
	_menu_help->Append(_menu_help_about);

	_menubar->Append(_menu_help, utf8("Help"));

	this->SetMenuBar(_menubar);

	_general_sizer = new wxBoxSizer(wxHORIZONTAL);

	_log = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, -1), wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH | wxBORDER_THEME);
	_log->SetFont(CreateFont());
	_log->Hide();

	_general_sizer->Add(_log, 0, wxEXPAND | wxRIGHT, 2);

	_general_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* notebook_size = new wxBoxSizer(wxVERTICAL);

	_notebook = new wxAuiNotebook(_general_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_FIXED_WIDTH | wxAUI_NB_TOP | wxAUI_NB_WINDOWLIST_BUTTON);
	_common_page = new wxPanel(_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* common_page_sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* gamedata_sizer = new wxStaticBoxSizer(new wxStaticBox(_common_page, wxID_ANY, utf8("Defence gamedata")), wxVERTICAL);

	wxGridSizer* gamedata_group_sizer = new wxGridSizer(0, 2, 0, 0);

	_staticText1 = new wxStaticText(gamedata_sizer->GetStaticBox(), wxID_ANY, utf8("Spawn delay"), wxDefaultPosition, wxDefaultSize, 0);
	_staticText1->Wrap(-1);
	gamedata_group_sizer->Add(_staticText1, 0, wxALL, 5);

	_spawn_delay_spin = new wxSpinCtrl(gamedata_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120, -1), wxSP_ARROW_KEYS, 0, 99999999, 0);
	gamedata_group_sizer->Add(_spawn_delay_spin, 0, wxALL, 5);

	_staticText2 = new wxStaticText(gamedata_sizer->GetStaticBox(), wxID_ANY, utf8("Start time"), wxDefaultPosition, wxDefaultSize, 0);
	_staticText2->Wrap(-1);
	gamedata_group_sizer->Add(_staticText2, 0, wxALL, 5);

	_start_time_spin = new wxSpinCtrl(gamedata_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120, -1), wxSP_ARROW_KEYS, 0, 23, 0);
	gamedata_group_sizer->Add(_start_time_spin, 0, wxALL, 5);

	/*
	* 
	* 	_staticText7 = new wxStaticText(team_sizer->GetStaticBox(), wxID_ANY, utf8("Skins"), wxDefaultPosition, wxDefaultSize, 0);
	_staticText7->Wrap(-1);
	team_group_sizer->Add(_staticText7, 0, wxALL, 5);

	_skins_ctrl = new wxTextCtrl(team_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_RICH);
	team_group_sizer->Add(_skins_ctrl, 0, wxALL | wxEXPAND, 5);
	*/

	_staticText8 = new wxStaticText(gamedata_sizer->GetStaticBox(), wxID_ANY, utf8("Spawn Particles"), wxDefaultPosition, wxDefaultSize, 0);
	_staticText8->Wrap(-1);
	gamedata_group_sizer->Add(_staticText8, 0, wxALL, 5);

	_spawn_particles_ctrl = new wxTextCtrl(gamedata_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RICH);
	gamedata_group_sizer->Add(_spawn_particles_ctrl, 0, wxALL | wxEXPAND, 5);

	_staticText9 = new wxStaticText(gamedata_sizer->GetStaticBox(), wxID_ANY, utf8("Spawn Sound"), wxDefaultPosition, wxDefaultSize, 0);
	_staticText9->Wrap(-1);
	gamedata_group_sizer->Add(_staticText9, 0, wxALL, 5);

	_spawn_sound_ctrl = new wxTextCtrl(gamedata_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RICH);
	gamedata_group_sizer->Add(_spawn_sound_ctrl, 0, wxALL | wxEXPAND, 5);

	gamedata_sizer->Add(gamedata_group_sizer, 1, wxEXPAND, 5);

	common_page_sizer->Add(gamedata_sizer, 0, wxEXPAND, 4);

	wxStaticBoxSizer* modifiers_sizer = new wxStaticBoxSizer(new wxStaticBox(_common_page, wxID_ANY, utf8("Defence modifiers")), wxVERTICAL);

	wxGridSizer* modifiers_group_sizer = new wxGridSizer(0, 2, 0, 0);

	_staticText3 = new wxStaticText(modifiers_sizer->GetStaticBox(), wxID_ANY, utf8("Surge"), wxDefaultPosition, wxDefaultSize, 0);
	_staticText3->Wrap(-1);
	modifiers_group_sizer->Add(_staticText3, 0, wxALL, 5);

	_surge_chk = new wxCheckBox(modifiers_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
	modifiers_group_sizer->Add(_surge_chk, 0, wxALL, 5);

	_staticText4 = new wxStaticText(modifiers_sizer->GetStaticBox(), wxID_ANY, utf8("Phantoms"), wxDefaultPosition, wxDefaultSize, 0);
	_staticText4->Wrap(-1);
	modifiers_group_sizer->Add(_staticText4, 0, wxALL, 5);

	_phantoms_chk = new wxCheckBox(modifiers_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
	modifiers_group_sizer->Add(_phantoms_chk, 0, wxALL, 5);

	modifiers_sizer->Add(modifiers_group_sizer, 1, wxEXPAND, 5);

	common_page_sizer->Add(modifiers_sizer, 0, wxEXPAND, 5);

	wxStaticBoxSizer* team_sizer = new wxStaticBoxSizer(new wxStaticBox(_common_page, wxID_ANY, utf8("Defence team")), wxVERTICAL);

	wxGridSizer* team_group_sizer = new wxGridSizer(0, 2, 0, 0);

	_staticText5 = new wxStaticText(team_sizer->GetStaticBox(), wxID_ANY, utf8("Logo"), wxDefaultPosition, wxDefaultSize, 0);
	_staticText5->Wrap(-1);
	team_group_sizer->Add(_staticText5, 0, wxALL, 5);

	wxArrayString _logo_choiceChoices;
	_logo_choice = new wxChoice(team_sizer->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxSize(200, -1), _logo_choiceChoices, 0);
	_logo_choice->SetSelection(0);
	team_group_sizer->Add(_logo_choice, 0, wxALL, 5);

	_staticText6 = new wxStaticText(team_sizer->GetStaticBox(), wxID_ANY, utf8("Messages menu faction"), wxDefaultPosition, wxDefaultSize, 0);
	_staticText6->Wrap(-1);
	team_group_sizer->Add(_staticText6, 0, wxALL, 5);

	_message_menu_spin = new wxSpinCtrl(team_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(120, -1), wxSP_ARROW_KEYS, 1, 9, 1);
	team_group_sizer->Add(_message_menu_spin, 0, wxALL, 5);

	_staticText7 = new wxStaticText(team_sizer->GetStaticBox(), wxID_ANY, utf8("Skins"), wxDefaultPosition, wxDefaultSize, 0);
	_staticText7->Wrap(-1);
	team_group_sizer->Add(_staticText7, 0, wxALL, 5);

	_skins_ctrl = new wxTextCtrl(team_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_RICH);
	team_group_sizer->Add(_skins_ctrl, 0, wxALL | wxEXPAND, 5);

	team_sizer->Add(team_group_sizer, 1, wxEXPAND, 5);

	common_page_sizer->Add(team_sizer, 0, wxEXPAND, 4);

	_common_page->SetSizer(common_page_sizer);
	_common_page->Layout();
	common_page_sizer->Fit(_common_page);
	_notebook->AddPage(_common_page, utf8("Common settings"), true, wxNullBitmap);

	_provision_page = new wxPanel(_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* provision_page_sizer = new wxBoxSizer(wxHORIZONTAL);

	_provision_splitter = new wxSplitterWindow(_provision_page, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3DSASH | wxSP_BORDER | wxSP_LIVE_UPDATE);
	_provision_splitter->Connect(wxEVT_IDLE, wxIdleEventHandler(SettingsFrame::_provision_splitterOnIdle), NULL, this);
	_provision_splitter->SetMinimumPaneSize(300);

	_provision_tab_panel = new wxPanel(_provision_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* provision_tab_sizer = new wxBoxSizer(wxVERTICAL);

	_provision_type_notebook = new wxNotebook(_provision_tab_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_FIXEDWIDTH | wxBORDER_SIMPLE);
	_provision_type_notebook->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));

	_provision_variants_page = new wxPanel(_provision_type_notebook, PROVISION_VARIANTS_PAGE_ID, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* prov_var_sizer = new wxBoxSizer(wxVERTICAL);

	_provision_variants_list = new wxListBox(_provision_variants_page, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 | wxBORDER_NONE);
	_provision_variants_list->SetFont(CreateFont());

	prov_var_sizer->Add(_provision_variants_list, 1, wxALL | wxEXPAND, 2);

	_provision_variants_page->SetSizer(prov_var_sizer);
	_provision_variants_page->Layout();
	prov_var_sizer->Fit(_provision_variants_page);
	_provision_type_notebook->AddPage(_provision_variants_page, wxT("Variants"), true);
	_provision_waves_page = new wxPanel(_provision_type_notebook, PROVISION_WAVES_PAGE_ID, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* prov_waves_sizer = new wxBoxSizer(wxVERTICAL);

	_provision_waves_list = new wxListBox(_provision_waves_page, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 | wxBORDER_NONE);
	_provision_waves_list->SetFont(CreateFont());

	prov_waves_sizer->Add(_provision_waves_list, 1, wxALL | wxEXPAND, 2);

	_provision_waves_page->SetSizer(prov_waves_sizer);
	_provision_waves_page->Layout();
	prov_waves_sizer->Fit(_provision_waves_page);
	_provision_type_notebook->AddPage(_provision_waves_page, wxT("Waves"), false);

	provision_tab_sizer->Add(_provision_type_notebook, 1, wxEXPAND | wxALL, 0);

	_provision_tab_panel->SetSizer(provision_tab_sizer);
	_provision_tab_panel->Layout();
	provision_tab_sizer->Fit(_provision_tab_panel);
	_provision_common_panel = new wxPanel(_provision_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	_provision_common_sizer = new wxBoxSizer(wxHORIZONTAL);

	_provision_common_splitter = new wxSplitterWindow(_provision_common_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3DSASH | wxSP_BORDER | wxSP_LIVE_UPDATE | wxSP_PERMIT_UNSPLIT);
	_provision_common_splitter->Connect(wxEVT_IDLE, wxIdleEventHandler(SettingsFrame::_provision_common_splitterOnIdle), NULL, this);

	_provision_variants_scroll = new wxScrolledWindow(_provision_common_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME | wxHSCROLL | wxVSCROLL);
	_provision_variants_scroll->SetScrollRate(5, 10);
	_provision_variants_scroll->Hide();

	wxBoxSizer* prov_variants_scroll_sizer = new wxBoxSizer(wxVERTICAL);

	_prov_waves_variants_list = new wxListBox(_provision_variants_scroll, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 | wxBORDER_NONE);
	_prov_waves_variants_list->SetFont(CreateFont());

	prov_variants_scroll_sizer->Add(_prov_waves_variants_list, 1, wxALL | wxEXPAND, 0);

	_provision_variants_scroll->SetSizer(prov_variants_scroll_sizer);
	_provision_variants_scroll->Layout();
	prov_variants_scroll_sizer->Fit(_provision_variants_scroll);
	_provision_items_scroll = new wxScrolledWindow(_provision_common_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME | wxHSCROLL | wxVSCROLL);
	_provision_items_scroll->SetScrollRate(5, 10);

	_prov_items_scroll_sizer = new wxBoxSizer(wxVERTICAL);

	_provision_items_scroll->SetSizer(_prov_items_scroll_sizer);
	_provision_items_scroll->Layout();
	_prov_items_scroll_sizer->Fit(_provision_items_scroll);

	_provision_common_splitter->SplitVertically(_provision_variants_scroll, _provision_items_scroll, 0);
	_provision_common_sizer->Add(_provision_common_splitter, 1, wxEXPAND, 0);

	this->setShowVariantsSplitterWindow(SettingType::PROVISION, false);

	_provision_common_panel->SetSizer(_provision_common_sizer);
	_provision_common_panel->Layout();
	_provision_common_sizer->Fit(_provision_common_panel);
	_provision_splitter->SplitVertically(_provision_tab_panel, _provision_common_panel, 300);
	provision_page_sizer->Add(_provision_splitter, 1, wxEXPAND, 0);

	_provision_page->SetSizer(provision_page_sizer);
	_provision_page->Layout();
	provision_page_sizer->Fit(_provision_page);
	_notebook->AddPage(_provision_page, wxT("Provision spawn"), true, wxNullBitmap);
	_npc_page = new wxPanel(_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* npc_page_sizer = new wxBoxSizer(wxVERTICAL);

	_npc_splitter = new wxSplitterWindow(_npc_page, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3DSASH | wxSP_BORDER | wxSP_LIVE_UPDATE);
	_npc_splitter->Connect(wxEVT_IDLE, wxIdleEventHandler(SettingsFrame::_npc_splitterOnIdle), NULL, this);
	_npc_splitter->SetMinimumPaneSize(300);

	_npc_tab_panel = new wxPanel(_npc_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* npc_tab_sizer = new wxBoxSizer(wxVERTICAL);

	_npc_type_notebook = new wxNotebook(_npc_tab_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_FIXEDWIDTH | wxBORDER_SIMPLE);
	_npc_type_notebook->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));

	_npc_variants_page = new wxPanel(_npc_type_notebook, NPC_VARIANTS_PAGE_ID, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* npc_var_sizer = new wxBoxSizer(wxVERTICAL);

	_npc_variants_list = new wxListBox(_npc_variants_page, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 | wxBORDER_NONE);
	_npc_variants_list->SetFont(CreateFont());

	npc_var_sizer->Add(_npc_variants_list, 1, wxALL | wxEXPAND, 2);

	_npc_variants_page->SetSizer(npc_var_sizer);
	_npc_variants_page->Layout();
	npc_var_sizer->Fit(_npc_variants_page);
	_npc_type_notebook->AddPage(_npc_variants_page, wxT("Variants"), true);
	_npc_waves_page = new wxPanel(_npc_type_notebook, NPC_WAVES_PAGE_ID, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* npc_waves_sizer = new wxBoxSizer(wxVERTICAL);

	_npc_waves_list = new wxListBox(_npc_waves_page, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 | wxBORDER_NONE);
	_npc_waves_list->SetFont(CreateFont());

	npc_waves_sizer->Add(_npc_waves_list, 1, wxALL | wxEXPAND, 2);

	_npc_waves_page->SetSizer(npc_waves_sizer);
	_npc_waves_page->Layout();
	npc_waves_sizer->Fit(_npc_waves_page);
	_npc_type_notebook->AddPage(_npc_waves_page, wxT("Waves"), false);

	npc_tab_sizer->Add(_npc_type_notebook, 1, wxEXPAND | wxALL, 0);

	_npc_tab_panel->SetSizer(npc_tab_sizer);
	_npc_tab_panel->Layout();
	npc_tab_sizer->Fit(_npc_tab_panel);
	_npc_common_panel = new wxPanel(_npc_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	_npc_common_sizer = new wxBoxSizer(wxHORIZONTAL);

	_npc_common_splitter = new wxSplitterWindow(_npc_common_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3DSASH | wxSP_BORDER | wxSP_LIVE_UPDATE | wxSP_PERMIT_UNSPLIT);
	_npc_common_splitter->Connect(wxEVT_IDLE, wxIdleEventHandler(SettingsFrame::_npc_common_splitterOnIdle), NULL, this);

	_npc_variants_scroll = new wxScrolledWindow(_npc_common_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME | wxHSCROLL | wxVSCROLL);
	_npc_variants_scroll->SetScrollRate(5, 10);
	_npc_variants_scroll->Hide();

	wxBoxSizer* npc_variants_scroll_sizer = new wxBoxSizer(wxVERTICAL);

	_npc_waves_variants_list = new wxListBox(_npc_variants_scroll, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 | wxBORDER_NONE);
	_npc_waves_variants_list->SetFont(CreateFont());

	npc_variants_scroll_sizer->Add(_npc_waves_variants_list, 1, wxALL | wxEXPAND, 0);

	_npc_variants_scroll->SetSizer(npc_variants_scroll_sizer);
	_npc_variants_scroll->Layout();
	npc_variants_scroll_sizer->Fit(_npc_variants_scroll);
	_npc_items_scroll = new wxScrolledWindow(_npc_common_splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_THEME | wxHSCROLL | wxVSCROLL);
	_npc_items_scroll->SetScrollRate(5, 10);

	_npc_items_scroll_sizer = new wxBoxSizer(wxVERTICAL);

	_npc_items_scroll->SetSizer(_npc_items_scroll_sizer);
	_npc_items_scroll->Layout();
	_npc_items_scroll_sizer->Fit(_npc_items_scroll);

	_npc_common_splitter->SplitVertically(_npc_variants_scroll, _npc_items_scroll, 0);
	_npc_common_sizer->Add(_npc_common_splitter, 1, wxEXPAND, 0);

	this->setShowVariantsSplitterWindow(SettingType::NPC, false);

	_npc_common_panel->SetSizer(_npc_common_sizer);
	_npc_common_panel->Layout();
	_npc_common_sizer->Fit(_npc_common_panel);
	_npc_splitter->SplitVertically(_npc_tab_panel, _npc_common_panel, 300);
	npc_page_sizer->Add(_npc_splitter, 1, wxEXPAND, 0);

	_npc_page->SetSizer(npc_page_sizer);
	_npc_page->Layout();
	npc_page_sizer->Fit(_npc_page);
	_notebook->AddPage(_npc_page, wxT("NPC spawn"), false, wxNullBitmap);

	notebook_size->Add(_notebook, 1, wxEXPAND | wxALL, 0);

	_general_panel->SetSizer(notebook_size);
	_general_panel->Layout();
	notebook_size->Fit(_general_panel);
	_general_sizer->Add(_general_panel, 1, wxEXPAND | wxALL, 0);

	this->SetSizer(_general_sizer);
	this->Layout();

	this->Centre(wxBOTH);
}

void SettingsFrame::connectActions()
{
	/////////////////////////////////////////////////////////////////
	/// FILE MENU ACTIONS
	/////////////////////////////////////////////////////////////////

	_menubar->Bind(wxEVT_MENU, [this](const wxCommandEvent& event)
	{
		if (_settings->isHaveData())
		{
			if (wxMessageBox("You have some changes. Do you want to save this?", "Unsaved data", wxYES_NO | wxICON_QUESTION) == wxYES)
			{
				auto dialog = new wxFileDialog(nullptr, utf8("Choose config file to save..."), wxEmptyString, wxEmptyString, utf8("Config file (*.ltx)|*.ltx"), wxFD_SAVE);

				if (dialog->ShowModal() == wxID_OK)
				{
					const auto& path = dialog->GetPath();
					_settings->save(path);
				}

				delete dialog;
				dialog = nullptr;
			}

			_settings->clearAll();
		}

		auto dialog = new wxFileDialog(nullptr, utf8("Choose config file to open..."), wxEmptyString, wxEmptyString, utf8("Config file (*ltx)|*ltx"), wxFD_OPEN);

		if (dialog->ShowModal() == wxID_OK)
		{
			const auto& path = dialog->GetPath().ToStdString();

			_settings->load(path);
		}

		delete dialog;
		dialog = nullptr;

	}, Actions::FILE_OPEN_ID);

	_menubar->Bind(wxEVT_MENU, [this](const wxCommandEvent& event)
	{
		if (_settings->isHaveFile())
		{
			_settings->save();
		}
		else
		{
			auto dialog = new wxFileDialog(nullptr, utf8("Choose config file to save..."), wxEmptyString, wxEmptyString, utf8("Config file (*.ltx)|*.ltx"), wxFD_SAVE);

			if (dialog->ShowModal() == wxID_OK)
			{
				const auto& path = dialog->GetPath().ToStdString();

				_settings->save(path);
			}

			delete dialog;
			dialog = nullptr;
		}

	}, Actions::FILE_SAVE_ID);

	_menubar->Bind(wxEVT_MENU, [this](const wxCommandEvent& event)
	{
		auto dialog = new wxFileDialog(nullptr, utf8("Choose config file to save..."), wxEmptyString, wxEmptyString, utf8("Config file (*.ltx)|*.ltx"), wxFD_SAVE);

		if (dialog->ShowModal() == wxID_OK)
		{
			const auto& path = dialog->GetPath().ToStdString();

			_settings->save(path);
		}

		delete dialog;
		dialog = nullptr;

	}, Actions::FILE_SAVE_AS);

	_menubar->Bind(wxEVT_MENU, [this](const wxCommandEvent& event)
	{
		auto dialog = new wxMessageDialog(nullptr, utf8("Are you sure to close program?"), utf8("Try to close program"), wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

		if (dialog->ShowModal() == wxID_YES)
		{
			delete dialog;
			dialog = nullptr;

			this->Close();
		}
		else
		{
			delete dialog;
			dialog = nullptr;
		}

	}, Actions::FILE_EXIT_ID);

	//////////////////////////////////////////////////
	// VIEW MENU ACTIONS
	//////////////////////////////////////////////////

	_menubar->Bind(wxEVT_MENU, [this](const wxCommandEvent& event)
	{
		if (_log->IsShown())
		{
			_log->Hide();
			_general_sizer->Layout();
		}
		else
		{
			_log->Show();
			_general_sizer->Layout();
		}

	}, Actions::VIEW_SHOW_LOG_ID);

	//////////////////////////////////////////////////
	// HELP MENU ACTIONS
	//////////////////////////////////////////////////

	_menubar->Bind(wxEVT_MENU, [this](const wxCommandEvent& event)
	{
		std::ostringstream stream;
		stream << "Defence settings editor 0.9a\n\n" <<
			"This is part of xrMPE project\n\n" <<
			"Developer: Raziel\n\n" <<
			"June-july 2021";

		const auto& text = utf8(stream.str());

		auto dialog = new wxMessageDialog(nullptr, text, utf8("About application..."));

		dialog->ShowModal();

		delete dialog;
		dialog = nullptr;

	}, Actions::HELP_ABOUT_ID);

	//////////////////////////////////////////////////
	// SKINS TEXT ACTIONS
	//////////////////////////////////////////////////

	_skins_ctrl->Bind(wxEVT_LEFT_DCLICK, [this](const wxMouseEvent& event)
	{
		auto dialog = new SelectItemDialog(this);
		dialog->fillList(_settings->getSkins());

		if (dialog->ShowModal() == wxID_OK)
		{
			const auto& list = dialog->getResultList();

			_settings->setSkins(list);

			{
				wxString str;
				uint32_t counter = 0u;

				for (const auto& pair : list)
				{
					if (pair.second)
					{
						if (counter > 0u)
							str += ", ";

						str += pair.first;

						++counter;
					}
				}

				_skins_ctrl->SetValue(str);
				_skins_ctrl->SetToolTip(str);
			}
		}

		delete dialog;
		dialog = nullptr;
	});

	//////////////////////////////////////////////////
	// PAGES ACTIONS
	//////////////////////////////////////////////////

	_notebook->Bind(wxEVT_NOTEBOOK_PAGE_CHANGING, [this](const wxBookCtrlEvent& event)
	{
		_prov_items_scroll_sizer->Clear(true);
		_npc_items_scroll_sizer->Clear(true);
	});

	_provision_type_notebook->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, [this](const wxBookCtrlEvent& event)
	{
		if (const int selection = event.GetSelection(); selection != wxNOT_FOUND)
		{
			if (auto page = _provision_type_notebook->GetPage(static_cast<size_t>(selection)); page != nullptr)
			{
				_provision_variants_list->SetSelection(-1);
				_provision_waves_list->SetSelection(-1);

				_current_selection = -1;
				_prov_waves_variants_list->Clear();
				_prov_items_scroll_sizer->Clear(true);
				_npc_items_scroll_sizer->Clear(true);

				if (page == _provision_variants_page)
				{
					_editable_items = true;
					this->setShowVariantsSplitterWindow(SettingType::PROVISION, !_editable_items);
				}
				else if (page == _provision_waves_page)
				{
					_editable_items = false;
					this->setShowVariantsSplitterWindow(SettingType::PROVISION, !_editable_items);
				}
			}
		}
	});

	_npc_type_notebook->Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, [this](const wxBookCtrlEvent& event)
	{
		if (const int selection = event.GetSelection(); selection != wxNOT_FOUND)
		{
			if (auto page = _npc_type_notebook->GetPage(static_cast<size_t>(selection)); page != nullptr)
			{
				_npc_variants_list->SetSelection(-1);
				_npc_waves_list->SetSelection(-1);

				_current_selection = -1;
				_npc_waves_variants_list->Clear();
				_prov_items_scroll_sizer->Clear(true);
				_npc_items_scroll_sizer->Clear(true);

				if (page == _npc_variants_page)
				{
					_editable_items = true;
					this->setShowVariantsSplitterWindow(SettingType::NPC, !_editable_items);
				}
				else if (page == _npc_waves_page)
				{
					_editable_items = false;
					this->setShowVariantsSplitterWindow(SettingType::NPC, !_editable_items);
				}
			}
		}
	});

	//////////////////////////////////////////////////
	// PAGES ACTIONS
	//////////////////////////////////////////////////

	const auto ConnectVariantActions = [this](const SettingType type, wxListBox* list, int& list_selection, wxScrolledWindow* items_scroll)
	{
		// You can use dynamic memory allocation with wxWidgets everywhere - it's safe
		list->Bind(wxEVT_CONTEXT_MENU, [type, list, &list_selection, this](const wxContextMenuEvent& event)
		{
			auto context_menu = new wxMenu;

			auto add_variant = new wxMenuItem(context_menu, POPUP_ADD_VARIANT, utf8("Add variant"), wxEmptyString, wxITEM_NORMAL);
			context_menu->Append(add_variant);

			auto remove_variant = new wxMenuItem(context_menu, POPUP_REMOVE_VARIANT, utf8("Remove variant"), wxEmptyString, wxITEM_NORMAL);
			context_menu->Append(remove_variant);

			context_menu->AppendSeparator();

			auto rename_variant = new wxMenuItem(context_menu, POPUP_RENAME_VARIANT, utf8("Rename variant"), wxEmptyString, wxITEM_NORMAL);
			context_menu->Append(rename_variant);

			///////////////////////////////////////////////////////////////////////
			// ACTIONS
			///////////////////////////////////////////////////////////////////////

			context_menu->Bind(wxEVT_MENU, [type, list, &list_selection, this](const wxCommandEvent& event)
			{
				uint32_t index = 0;

				switch (type)
				{
					case SettingType::PROVISION:
					{
						++provision_variant_index;
						index = provision_variant_index;
						_prov_items_scroll_sizer->Clear(true);
					}
					break;

					case SettingType::NPC:
					{
						++npc_variant_index;
						index = npc_variant_index;
						_npc_items_scroll_sizer->Clear(true);
					}
					break;

					default:
					break;
				}

				const std::string& variant_name = "variant_" + std::to_string(index);

				_settings->setVariant(type, {variant_name, {}});

				list->Append(variant_name);

				const int selection = int(list->GetCount()) - 1;

				list->SetSelection(selection);
				list_selection = selection;

			}, POPUP_ADD_VARIANT);

			context_menu->Bind(wxEVT_MENU, [type, list, this](const wxCommandEvent& event)
			{
				const int selection_index = list->GetSelection();

				const auto& variant_name = list->GetString(static_cast<uint32_t>(selection_index));
				_settings->removeVariant(type, variant_name);

				if (selection_index != -1)
					list->Delete(static_cast<uint32_t>(selection_index));

			}, POPUP_REMOVE_VARIANT);

			context_menu->Bind(wxEVT_MENU, [type, list, this](const wxCommandEvent& event)
			{
				const int selection_index = list->GetSelection();

				if (selection_index != -1)
				{
					const auto& text = list->GetString(selection_index);
					wxString new_string = wxGetTextFromUser("Rename variant", wxEmptyString, text);

					new_string.Replace(" ", "_");

					if (!new_string.empty())
					{
						_settings->renameVariant(type, text, new_string);

						list->SetString(selection_index, new_string);
					}
				}

			}, POPUP_RENAME_VARIANT);

			///////////////////////////////////////////////////////////////////////

			this->PopupMenu(context_menu);
		});

		items_scroll->Bind(wxEVT_CONTEXT_MENU, [type, this](const wxContextMenuEvent& event)
		{
			if (_editable_items)
			{
				auto context_menu = new wxMenu;

				auto add_item = new wxMenuItem(context_menu, POPUP_ADD_ITEM, utf8("Add item"), wxEmptyString, wxITEM_NORMAL);
				context_menu->Append(add_item);

				auto random_add_item = new wxMenuItem(context_menu, POPUP_RANDOM_CREATE_ITEMS, utf8("Random create items..."), wxEmptyString, wxITEM_NORMAL);
				context_menu->Append(random_add_item);

				///////////////////////////////////////////////////////////////////////
				// ACTIONS
				///////////////////////////////////////////////////////////////////////

				context_menu->Bind(wxEVT_MENU, [type, this](const wxCommandEvent& event)
				{
					this->addVariantItem(type);

				}, POPUP_ADD_ITEM);

				context_menu->Bind(wxEVT_MENU, [type, this](const wxCommandEvent& event)
				{
					auto dialog = new RandomItemsDialog(this);

					if (dialog->ShowModal() == wxID_OK)
					{
						const RandomItemsDialog::RandomData& data = dialog->getRandomData();

						const auto random_uint = [](const uint32_t min, const uint32_t max) -> uint32_t
						{
							std::random_device rd;
							std::mt19937_64 gen(rd());
							std::uniform_int_distribution<uint32_t> uid(min, max);
							return uid(gen);
						};

						const auto random_float = [](const float min, const float max) -> float
						{
							std::random_device rd;
							std::mt19937_64 gen(rd());
							std::uniform_real_distribution<float> uid(min, max);
							return uid(gen);
						};

						const auto generate = [random_uint, random_float, data, settings = _settings.get()](const SettingType& type,
												wxScrolledWindow* parent,
												const int selection,
												wxBoxSizer* sizer,
												wxListBox* variant_list) -> void
						{
							if (const auto& variants_map = settings->getProvisionVariants();
								(selection < int(variants_map.size())))
							{
								for (uint32_t i = 0u; i < data.items_count; i++)
								{
									auto variant_panel = new VariantPanel(parent, settings, type, variant_list, true);

									VariantData variant_data;

									variant_data.spawn_number = static_cast<int>(random_uint(data.count_range.min, data.count_range.max));
									variant_data.spawn_prop = random_float(data.prop_range.min, data.prop_range.max);
									variant_data.desc = variant_panel->getRandomChoiceFomItemsList();

									variant_panel->setFromVariant(variant_data);

									sizer->Add(variant_panel, 0, wxEXPAND | wxALL, 2);
									sizer->Layout();

									variant_panel->remember();
								}
							}
						};

						switch (type)
						{
							case SettingType::PROVISION:
							{
								if (_current_selection < 0)
									return;

								generate(type, _provision_items_scroll, _current_selection, _prov_items_scroll_sizer, _provision_variants_list);
							}
							break;

							case SettingType::NPC:
							{
								if (_current_selection < 0)
									return;

								generate(type, _npc_items_scroll, _current_selection, _npc_items_scroll_sizer, _npc_variants_list);
							}
							break;

							default:
							break;
						}
					}

					delete dialog;
					dialog = nullptr;

				}, POPUP_RANDOM_CREATE_ITEMS);

				///////////////////////////////////////////////////////////////////////

				this->PopupMenu(context_menu);
			}
		});

		list->Bind(wxEVT_LISTBOX, [type, list, &list_selection, this](const wxCommandEvent& event)
		{
			list_selection = list->GetSelection();
			this->fillCurrentVariantList(type, list);
		});
	};

	const auto ConnectWaveActions = [this](const SettingType type, wxListBox* list, wxListBox* variant_list, int& list_selection)
	{
		list->Bind(wxEVT_CONTEXT_MENU, [type, list, &list_selection, this](const wxContextMenuEvent& event)
		{
			auto context_menu = new wxMenu;

			auto add_variant = new wxMenuItem(context_menu, POPUP_ADD_WAVE, utf8("Add wave"), wxEmptyString, wxITEM_NORMAL);
			context_menu->Append(add_variant);

			auto remove_variant = new wxMenuItem(context_menu, POPUP_REMOVE_WAVE, utf8("Remove wave"), wxEmptyString, wxITEM_NORMAL);
			context_menu->Append(remove_variant);

			context_menu->AppendSeparator();

			auto move_up = new wxMenuItem(context_menu, POPUP_WAVE_MOVE_UP, utf8("Move up"), wxEmptyString, wxITEM_NORMAL);
			context_menu->Append(move_up);

			auto move_down = new wxMenuItem(context_menu, POPUP_WAVE_MOVE_DOWN, utf8("Move down"), wxEmptyString, wxITEM_NORMAL);
			context_menu->Append(move_down);

			///////////////////////////////////////////////////////////////////////////////////////////////

			context_menu->Bind(wxEVT_MENU, [type, list, &list_selection, this](const wxCommandEvent& event)
			{
				const uint32_t index = list->GetCount() + 1u;

				list->Append("wave_" + std::to_string(index));

				const int selection = int(list->GetCount()) - 1;

				list->SetSelection(selection);
				list_selection = selection;

			}, POPUP_ADD_WAVE);

			context_menu->Bind(wxEVT_MENU, [type, list, this](const wxCommandEvent& event)
			{
				const int selection_index = list->GetSelection();

				if (selection_index != -1)
					list->Delete(static_cast<uint32_t>(selection_index));

				for (uint32_t i = 0u; i < list->GetCount(); ++i)
				{
					list->SetString(i, "wave_" + std::to_string(i + 1u));
				}

			}, POPUP_REMOVE_WAVE);

			context_menu->Bind(wxEVT_MENU, [type, list, this](const wxCommandEvent& event)
			{
				const int index = list->GetSelection();

				if (index > 0)
				{
					const auto& current_wave = list->GetString(static_cast<uint32_t>(index));
					const auto& prev_wave = list->GetString(static_cast<uint32_t>(index - 1));

					auto& current = _settings->getWave(type, current_wave);
					auto& prev = _settings->getWave(type, prev_wave);

					std::swap(current, prev);

					this->fillCurrentWaveList(type);
				}

			}, POPUP_WAVE_MOVE_UP);

			context_menu->Bind(wxEVT_MENU, [type, list, this](const wxCommandEvent& event)
			{
				const int index = list->GetSelection();

				if (index < static_cast<int>(list->GetCount() - 1u))
				{
					const auto& current_wave = list->GetString(static_cast<uint32_t>(index));
					const auto& next_wave = list->GetString(static_cast<uint32_t>(index + 1));

					auto& current = _settings->getWave(type, current_wave);
					auto& next = _settings->getWave(type, next_wave);

					std::swap(current, next);

					this->fillCurrentWaveList(type);
				}

			}, POPUP_WAVE_MOVE_DOWN);

			///////////////////////////////////////////////////////////////////////

			this->PopupMenu(context_menu);
		});

		list->Bind(wxEVT_LISTBOX, [type, list, &list_selection, this](const wxCommandEvent& event)
		{
			switch (type)
			{
				case SettingType::PROVISION:
					_prov_items_scroll_sizer->Clear(true);
				break;

				case SettingType::NPC:
					_npc_items_scroll_sizer->Clear(true);
				break;

				default:
				break;
			}

			_editable_items = false;

			// Fill list here
			list_selection = list->GetSelection();
			this->fillCurrentWaveList(type);
		});

		variant_list->Bind(wxEVT_CONTEXT_MENU, [type, list, &list_selection, this](const wxContextMenuEvent& event)
		{
			auto context_menu = new wxMenu;

			auto select_variants = new wxMenuItem(context_menu, POPUP_WAVE_SELECT_VARIANT, utf8("Select variants"), wxEmptyString, wxITEM_NORMAL);
			context_menu->Append(select_variants);

			auto remove_variant = new wxMenuItem(context_menu, POPUP_WAVE_REMOVE_VARIANT, utf8("Remove variant"), wxEmptyString, wxITEM_NORMAL);
			context_menu->Append(remove_variant);

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////

			context_menu->Bind(wxEVT_MENU, [type, list, this](const wxCommandEvent& event)
			{
				auto dialog = new VariantSelectDialog(this);

				switch (type)
				{
					case SettingType::PROVISION:
						dialog->fillList(_settings->getProvisionVariants());
					break;

					case SettingType::NPC:
						dialog->fillList(_settings->getNPCVariants());
					break;

					default:
					break;
				}

				if (dialog->ShowModal() == wxID_OK)
				{
					const auto& wave_name = list->GetStringSelection();

					Wave& wave = _settings->getWave(type, wave_name);

					const auto AddIfNotExist = [&](const std::string& item)
					{
						const auto iter = std::find_if(wave.variants.begin(), wave.variants.end(), [&](const std::pair<std::string, const Variant*>& pair)
						{
							return (pair.first == item);
						});

						if (iter == wave.variants.end())
						{
							switch (type)
							{
								case SettingType::PROVISION:
								{
									const auto& vars = _settings->getProvisionVariants();

									wave.variants[item] = &vars.at(item);
								}
								break;

								case SettingType::NPC:
								{
									const auto& vars = _settings->getNPCVariants();

									wave.variants[item] = &vars.at(item);
								}
								break;

								default:
								break;
							}
						}
					};

					for (const auto& var : dialog->getResultList())
						AddIfNotExist(var);

					// Fill list here
					this->fillCurrentWaveList(type);
				}

				delete dialog;
				dialog = nullptr;

			}, POPUP_WAVE_SELECT_VARIANT);

			context_menu->Bind(wxEVT_MENU, [type, list, this](const wxCommandEvent& event)
			{
				const auto& wave_name = list->GetStringSelection();
				const auto& variant_name = _prov_waves_variants_list->GetStringSelection();

				if (!wave_name.empty() && !variant_name.empty())
					_settings->removeVariantFromWave(type, wave_name, variant_name);

				this->fillCurrentWaveList(type);

			}, POPUP_WAVE_REMOVE_VARIANT);

			/// /////////////////////////////////////////////////////////////////////////////////////////////////////////

			this->PopupMenu(context_menu);
		});

		variant_list->Bind(wxEVT_LISTBOX, [type, variant_list, &list_selection, this](const wxCommandEvent& event)
		{
			_editable_items = false;

			list_selection = variant_list->GetSelection();

			this->fillCurrentVariantList(type, variant_list);

			list_selection = -1;
		});
	};

	// Connect by type
	ConnectVariantActions(SettingType::PROVISION, _provision_variants_list, _current_selection, _provision_items_scroll);
	ConnectVariantActions(SettingType::NPC, _npc_variants_list, _current_selection, _npc_items_scroll);

	ConnectWaveActions(SettingType::PROVISION, _provision_waves_list, _prov_waves_variants_list, _current_selection);
	ConnectWaveActions(SettingType::NPC, _npc_waves_list, _npc_waves_variants_list, _current_selection);
}

void SettingsFrame::clearUI()
{
	_provision_variants_list->Clear();
	_provision_waves_list->Clear();
	_prov_waves_variants_list->Clear();
	_prov_items_scroll_sizer->Clear(true);

	_npc_variants_list->Clear();
	_npc_waves_list->Clear();
	_npc_waves_variants_list->Clear();
	_npc_items_scroll_sizer->Clear(true);
}

void SettingsFrame::fillCurrentVariantList(const SettingType type, wxListBox* variant_list)
{
	switch (type)
	{
		case SettingType::PROVISION:
		{
			if (_current_selection < 0)
				return;

			_prov_items_scroll_sizer->Clear(true);

			if (const auto& variants_map = _settings->getProvisionVariants();
				!variants_map.empty() && (_current_selection < int(variants_map.size())))
			{
				const std::string& name = variant_list->GetString(_current_selection);

				const auto iter = variants_map.find(name);

				if (iter != variants_map.cend())
				{
					for (const auto& variant_data : iter->second.data)
					{
						auto variant_panel = new VariantPanel(_provision_items_scroll, _settings.get(), type, variant_list, _editable_items);

						variant_panel->setFromVariant(variant_data);

						_prov_items_scroll_sizer->Add(variant_panel, 0, wxEXPAND | wxALL, 2);
						_prov_items_scroll_sizer->Layout();
					}
				}
			}
		}
		break;

		case SettingType::NPC:
		{
			if (_current_selection < 0)
				return;

			_npc_items_scroll_sizer->Clear(true);

			if (const auto& variants_map = _settings->getNPCVariants();
				!variants_map.empty() && (_current_selection < int(variants_map.size())))
			{
				const std::string& name = variant_list->GetString(_current_selection);

				const auto iter = variants_map.find(name);

				if (iter != variants_map.cend())
				{
					for (const auto& variant_data : iter->second.data)
					{
						auto variant_panel = new VariantPanel(_npc_items_scroll, _settings.get(), type, variant_list, _editable_items);

						variant_panel->setFromVariant(variant_data);

						_npc_items_scroll_sizer->Add(variant_panel, 0, wxEXPAND | wxALL, 2);
						_npc_items_scroll_sizer->Layout();
					}
				}
			}
		}
		break;

		default:
			break;
	}
}

void SettingsFrame::fillCurrentWaveList(const SettingType type)
{
	switch (type)
	{
		case SettingType::PROVISION:
		{
			const auto& wave_name = _provision_waves_list->GetStringSelection();

			_prov_waves_variants_list->Clear();

			const auto& waves = _settings->getProvisionWaves();

			if (const auto iter = waves.find(wave_name); iter != waves.cend())
			{
				for (const auto& var : iter->second.variants)
					_prov_waves_variants_list->Append(var.first);
			}
		}
		break;

		case SettingType::NPC:
		{
			const auto& wave_name = _npc_waves_list->GetStringSelection();

			_npc_waves_variants_list->Clear();

			const auto& waves = _settings->getNPCWaves();

			if (const auto iter = waves.find(wave_name); iter != waves.cend())
			{
				for (const auto& var : iter->second.variants)
					_npc_waves_variants_list->Append(var.first);
			}
		}
		break;

		default:
		break;
	}
}

void SettingsFrame::setShowVariantsSplitterWindow(const SettingType type, const bool visible)
{
	switch (type)
	{
		case SettingType::PROVISION:
		{
			if (visible)
				_provision_common_splitter->SplitVertically(_provision_variants_scroll, _provision_items_scroll);
			else
				_provision_common_splitter->Unsplit(_provision_variants_scroll);
		}
		break;

		case SettingType::NPC:
		{
			if (visible)
				_npc_common_splitter->SplitVertically(_npc_variants_scroll, _npc_items_scroll);
			else
				_npc_common_splitter->Unsplit(_npc_variants_scroll);
		}
		break;

		default:
		break;
	}
}

void SettingsFrame::addVariantItem(const SettingType type)
{
	switch (type)
	{
		case SettingType::PROVISION:
		{
			if (_provision_variants_list->IsEmpty())
			{
				wxMessageBox("You cannot create item. Variant list is empty.\nYou must add at least one variant to add item.", "Cannot create item");
			}
			else
			{
				auto variant_panel = new VariantPanel(_provision_items_scroll, _settings.get(), type, _provision_variants_list, _editable_items);

				_prov_items_scroll_sizer->Add(variant_panel, 0, wxEXPAND | wxALL, 2);
				_prov_items_scroll_sizer->Layout();

				// it's actually recompute scroll window
				_provision_items_scroll->FitInside();
			}
		}
		break;

		case SettingType::NPC:
		{
			if (_npc_variants_list->IsEmpty())
			{
				wxMessageBox("You cannot create item. Variant list is empty.\nYou must add at least one variant to add item.", "Cannot create item");
			}
			else
			{
				auto variant_panel = new VariantPanel(_npc_items_scroll, _settings.get(), type, _npc_variants_list, _editable_items);

				_npc_items_scroll_sizer->Add(variant_panel, 0, wxEXPAND | wxALL, 2);
				_npc_items_scroll_sizer->Layout();

				// it's actually recompute scroll window
				_npc_items_scroll->FitInside();
			}
		}
		break;

		default:
			msg("WTF?");
		break;
	}
}

std::vector<VariantData> SettingsFrame::getCurrentVariantList(const SettingType type) const
{
	std::vector<VariantData> data;

	switch (type)
	{
		case SettingType::PROVISION:
		{
			for (const auto& item : _prov_items_scroll_sizer->GetChildren())
			{
				auto panel = dynamic_cast<VariantPanel*>(item->GetWindow());

				data.push_back(panel->getVariant());
			}
		}
		break;

		case SettingType::NPC:
		{
			for (const auto& item : _npc_items_scroll_sizer->GetChildren())
			{
				auto panel = dynamic_cast<VariantPanel*>(item->GetWindow());

				data.push_back(panel->getVariant());
			}
		}
		break;

		default:
		break;
	}

	return data;
}

void SettingsFrame::msg(const std::string& message) noexcept
{
	_log->AppendText(message + '\n');
}

void SettingsFrame::_provision_splitterOnIdle(wxIdleEvent&)
{
	_provision_splitter->SetSashPosition(300);
	_provision_splitter->Disconnect(wxEVT_IDLE, wxIdleEventHandler(SettingsFrame::_provision_splitterOnIdle), nullptr, this);
}

void SettingsFrame::_npc_splitterOnIdle(wxIdleEvent&)
{
	_npc_splitter->SetSashPosition(300);
	_npc_splitter->Disconnect(wxEVT_IDLE, wxIdleEventHandler(SettingsFrame::_npc_splitterOnIdle), nullptr, this);
}

void SettingsFrame::_provision_common_splitterOnIdle(wxIdleEvent&)
{
	_provision_common_splitter->SetSashPosition(0);
	_provision_common_splitter->Disconnect(wxEVT_IDLE, wxIdleEventHandler(SettingsFrame::_provision_common_splitterOnIdle), nullptr, this);
}

void SettingsFrame::_npc_common_splitterOnIdle(wxIdleEvent&)
{
	_npc_common_splitter->SetSashPosition(0);
	_npc_common_splitter->Disconnect(wxEVT_IDLE, wxIdleEventHandler(SettingsFrame::_npc_common_splitterOnIdle), nullptr, this);
}
