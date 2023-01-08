#include "SelectItemDialog.hpp"

#include <wx/sizer.h>
#include <wx/checklst.h>
#include <wx/button.h>
#include <wx/textdlg.h>


SelectItemDialog::SelectItemDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* common_sizer = new wxBoxSizer(wxHORIZONTAL);

	_list = new wxCheckListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, {}, wxLB_EXTENDED | wxLB_NEEDED_SB);
	_list->SetFont(wxFont(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));

	common_sizer->Add(_list, 1, wxEXPAND | wxALL, 4);

	_button_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(120, -1), wxBORDER_THEME | wxTAB_TRAVERSAL);
	_button_panel->SetMinSize(wxSize(120, -1));
	_button_panel->SetMaxSize(wxSize(120, -1));

	wxBoxSizer* button_sizer = new wxBoxSizer(wxVERTICAL);

	button_sizer->Add(0, 40, 0, wxEXPAND, 5);

	_add_btn = new wxButton(_button_panel, ADD_ID, wxT("Add"), wxDefaultPosition, wxDefaultSize, 0);
	button_sizer->Add(_add_btn, 0, wxALL | wxEXPAND, 5);

	_remove_btn = new wxButton(_button_panel, REMOVE_ID, wxT("Remove"), wxDefaultPosition, wxDefaultSize, 0);
	button_sizer->Add(_remove_btn, 0, wxALL | wxEXPAND, 5);

	_rename_btn = new wxButton(_button_panel, RENAME_ID, wxT("Rename"), wxDefaultPosition, wxDefaultSize, 0);
	button_sizer->Add(_rename_btn, 0, wxALL | wxEXPAND, 5);

	button_sizer->Add(0, 60, 0, wxEXPAND, 5);

	_accept_btn = new wxButton(_button_panel, ACCEPT_ID, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	button_sizer->Add(_accept_btn, 0, wxALL | wxEXPAND, 5);

	button_sizer->Add(0, 0, 1, wxEXPAND, 5);

	_button_panel->SetSizer(button_sizer);
	_button_panel->Layout();
	common_sizer->Add(_button_panel, 0, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 4);

	this->SetSizer(common_sizer);
	this->Layout();

	this->Centre(wxBOTH);

	////////////////////////////

	this->connectActions();

	////////////////////////////
}

SelectItemDialog::~SelectItemDialog()
{
}

void SelectItemDialog::connectActions()
{
	const auto RenameFunc = [this](const wxCommandEvent& event) -> void
	{
		const int selection_index = _list->GetSelection();

		if (selection_index != -1)
		{
			const auto& text = _list->GetString(selection_index);
			wxString new_string = wxGetTextFromUser("Rename item", wxEmptyString, text);

			new_string.Replace(" ", "_");

			if (!new_string.empty())
				_list->SetString(selection_index, new_string);
		}
	};

	_list->Bind(wxEVT_LISTBOX_DCLICK, RenameFunc);

	_add_btn->Bind(wxEVT_BUTTON, [this](const wxCommandEvent& event)
	{
		_list->Append("empty_item");
	});

	_remove_btn->Bind(wxEVT_BUTTON, [this](const wxCommandEvent& event)
	{
		if (const int index = event.GetSelection(); index != -1)
			_list->Delete(static_cast<uint32_t>(index));
	});

	_rename_btn->Bind(wxEVT_BUTTON, RenameFunc);

	_accept_btn->Bind(wxEVT_BUTTON, [this](const wxCommandEvent& event)
	{
		for (size_t i = 0; i < _list->GetCount(); ++i)
		{
			const auto& str = _list->GetString(i);

			_result_list[str] = _list->IsChecked(i);
		}

		this->EndModal(wxID_OK);
	});
}

void SelectItemDialog::fillList(const std::map<std::string, bool>& list)
{
	uint32_t counter = 0u;

	for (const auto& pair : list)
	{
		_list->Append(pair.first);
		_list->Check(counter, pair.second);

		++counter;
	}
}
