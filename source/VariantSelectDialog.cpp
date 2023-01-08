#include "VariantSelectDialog.hpp"

#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/listbox.h>
#include <wx/button.h>


VariantSelectDialog::VariantSelectDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* general_sizer = new wxBoxSizer(wxHORIZONTAL);

	_list = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, nullptr, wxLB_MULTIPLE);
	_list->SetFont(wxFont(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));

	general_sizer->Add(_list, 1, wxALL | wxEXPAND, 2);

	_button_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(92, -1), wxBORDER_THEME | wxTAB_TRAVERSAL);

	wxBoxSizer* panel_sizer = new wxBoxSizer(wxVERTICAL);

	panel_sizer->Add(0, 60, 0, wxEXPAND, 5);

	_accept_btn = new wxButton(_button_panel, wxID_ANY, wxT("OK"), wxDefaultPosition, wxSize(92, -1), 0);
	panel_sizer->Add(_accept_btn, 0, wxALL, 4);

	panel_sizer->Add(0, 0, 1, wxEXPAND, 5);

	_button_panel->SetSizer(panel_sizer);
	_button_panel->Layout();
	general_sizer->Add(_button_panel, 0, wxEXPAND | wxALL, 2);

	this->SetSizer(general_sizer);
	this->Layout();
	this->Centre(wxBOTH);

	//////////////////////////////////////////////////////////////

	_accept_btn->Bind(wxEVT_BUTTON, [this](const wxCommandEvent& event)
	{
		this->EndModal(wxID_OK);
	});
}

VariantSelectDialog::~VariantSelectDialog()
{
}

void VariantSelectDialog::fillList(const std::map<std::string, Variant>& variants)
{
	for (const auto& pair : variants)
		_list->Append(pair.first);
}

std::vector<std::string> VariantSelectDialog::getResultList() const
{
	wxArrayInt selections;
	std::vector<std::string> result;

	_list->GetSelections(selections);

	for (const auto& index : selections)
		result.push_back(_list->GetString(static_cast<unsigned int>(index)));

	return result;
}
