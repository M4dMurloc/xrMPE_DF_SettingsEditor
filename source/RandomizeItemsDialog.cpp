#include "RandomizeItemsDialog.hpp"

#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/spinctrl.h>

#define utf8(str) (wxString::FromUTF8(str))


RandomItemsDialog::RandomItemsDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxSize(210, 250), wxDefaultSize);

	wxBoxSizer* random_dialog_sizer = new wxBoxSizer(wxVERTICAL);

	_settings_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* settings_sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* _count_sizer = new wxStaticBoxSizer(new wxStaticBox(_settings_panel, wxID_ANY, utf8("Item count to generate:")), wxVERTICAL);

	_count_spin = new wxSpinCtrl(_count_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxALIGN_CENTER_HORIZONTAL | wxSP_ARROW_KEYS, 1, 50, 1);
	_count_sizer->Add(_count_spin, 0, wxALL, 5);

	settings_sizer->Add(_count_sizer, 0, wxEXPAND, 5);

	wxStaticBoxSizer* _count_range_sizer = new wxStaticBoxSizer(new wxStaticBox(_settings_panel, wxID_ANY, utf8("Item count range:")), wxHORIZONTAL);

	_count_min_spin = new wxSpinCtrl(_count_range_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxALIGN_CENTER_HORIZONTAL | wxSP_ARROW_KEYS, 1, 999, 1);
	_count_range_sizer->Add(_count_min_spin, 0, wxALL, 5);

	_count_max_spin = new wxSpinCtrl(_count_range_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxALIGN_CENTER_HORIZONTAL | wxSP_ARROW_KEYS, 1, 999, 10);
	_count_range_sizer->Add(_count_max_spin, 0, wxALL, 5);

	settings_sizer->Add(_count_range_sizer, 0, wxEXPAND, 5);

	wxStaticBoxSizer* _prop_sizer = new wxStaticBoxSizer(new wxStaticBox(_settings_panel, wxID_ANY, utf8("Item propatibility range:")), wxHORIZONTAL);

	_prop_min_spin = new wxSpinCtrlDouble(_prop_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxALIGN_CENTER_HORIZONTAL | wxSP_ARROW_KEYS, 0, 1, 0, 0.1);
	_prop_min_spin->SetDigits(2);
	_prop_sizer->Add(_prop_min_spin, 0, wxALL, 5);

	_prop_max_spin = new wxSpinCtrlDouble(_prop_sizer->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1), wxALIGN_CENTER_HORIZONTAL | wxSP_ARROW_KEYS, 0, 1, 1, 0.1);
	_prop_max_spin->SetDigits(2);
	_prop_sizer->Add(_prop_max_spin, 0, wxALL, 5);

	settings_sizer->Add(_prop_sizer, 0, wxEXPAND, 5);

	_settings_panel->SetSizer(settings_sizer);
	_settings_panel->Layout();
	settings_sizer->Fit(_settings_panel);
	random_dialog_sizer->Add(_settings_panel, 0, wxEXPAND | wxALL, 5);

	_btn_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* btn_sizer;
	btn_sizer = new wxBoxSizer(wxHORIZONTAL);

	btn_sizer->Add(0, 0, 1, wxEXPAND, 5);

	_generate_button = new wxButton(_btn_panel, wxID_ANY, utf8("Create"), wxDefaultPosition, wxDefaultSize, 0);
	btn_sizer->Add(_generate_button, 0, wxALL, 5);

	_btn_panel->SetSizer(btn_sizer);
	_btn_panel->Layout();
	btn_sizer->Fit(_btn_panel);
	random_dialog_sizer->Add(_btn_panel, 0, wxEXPAND | wxALL, 5);

	this->SetSizer(random_dialog_sizer);
	this->Layout();

	this->Centre(wxBOTH);

	///////////////////////////////////////
	// Connect actions
	///////////////////////////////////////

	this->connectActions();

	///////////////////////////////////////
}

RandomItemsDialog::~RandomItemsDialog()
{
}

RandomItemsDialog::RandomData RandomItemsDialog::getRandomData() const noexcept
{
	RandomData data;

	data.items_count = static_cast<uint32_t>(_count_spin->GetValue());
	data.count_range = {static_cast<uint32_t>(_count_min_spin->GetValue()), static_cast<uint32_t>(_count_max_spin->GetValue())};
	data.prop_range = {static_cast<float>(_prop_min_spin->GetValue()), static_cast<float>(_prop_max_spin->GetValue())};

	return data;
}

void RandomItemsDialog::connectActions() noexcept
{
	_generate_button->Bind(wxEVT_BUTTON, [this](const wxCommandEvent& event)
	{
		this->EndModal(wxID_OK);
	});
}
