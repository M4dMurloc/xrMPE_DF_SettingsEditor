#include "VariantPanel.hpp"
#include "Frame.hpp"
#include "Settings.hpp"

#include <random>

#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/scrolwin.h>
#include <wx/listbox.h>
#include <wx/checkbox.h>
#include <wx/choice.h>


VariantPanel::VariantPanel(wxScrolledWindow* parent, Settings* settings, SettingType type, wxListBox* list, const bool editable) :
	wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(500, 26), wxBORDER_THEME | wxNO_FULL_REPAINT_ON_RESIZE | wxCLIP_CHILDREN, wxEmptyString),
	_scroll(parent),
	_settings(settings),
	_type(type),
	_list(list),
	_editable(editable)
{
	wxBoxSizer* variant_sizer = new wxBoxSizer(wxHORIZONTAL);

	wxArrayString items_list;

	for (const auto& item : (this->isNPCType() ? _settings->getMonstersList() : _settings->getProvisionList()))
		items_list.push_back(item);

	_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, items_list, 0);
	_choice->SetSelection(-1);
	_choice->Enable(_editable);

	variant_sizer->Add(_choice, 1, wxEXPAND | wxRIGHT, 4);

	_count_spin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 1, 9999, 1);
	_count_spin->Enable(_editable);

	variant_sizer->Add(_count_spin, 0, wxRIGHT | wxLEFT, 4);

	_prop_spin = new wxSpinCtrlDouble(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, -1), wxSP_ARROW_KEYS, 0, 1, 1, 0.1);
	_prop_spin->SetDigits(2);
	_prop_spin->Enable(_editable);

	variant_sizer->Add(_prop_spin, 0, wxLEFT, 4);

	if (this->isNPCType())
	{
		_check = new wxCheckBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(18, -1), wxALIGN_RIGHT | wxCHK_2STATE);
		_check->SetToolTip("Ignore player number");
		_check->Enable(_editable);

		variant_sizer->Add(_check, 0, wxALL | wxEXPAND, 2);
	}

	if (_editable)
	{
		_staticline = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL | wxNO_FULL_REPAINT_ON_RESIZE);
		variant_sizer->Add(_staticline, 0, wxRIGHT | wxLEFT | wxEXPAND, 6);

		_del_button = new wxButton(this, wxID_ANY, "X", wxDefaultPosition, wxSize(22, 22), wxBORDER_NONE | wxNO_FULL_REPAINT_ON_RESIZE | wxCLIP_CHILDREN);
		_del_button->SetToolTip("Remove item");
		_del_button->SetMinSize(wxSize(22, 22));
		_del_button->SetMaxSize(wxSize(22, 22));

		variant_sizer->Add(_del_button, 0, wxALL, 0);
	}

	this->SetSizer(variant_sizer);
	this->Layout();

	///////////////////////////////////////
	// ACTIONS
	///////////////////////////////////////

	if (_editable)
	{
		// Little dirty removing)
		_del_button->Bind(wxEVT_BUTTON, [this](const wxCommandEvent& event)
		{
			auto sizer = this->GetParent()->GetSizer();

			auto scroll = _scroll;
			auto list = _list;
			const auto type = _type;
			auto settings = _settings;

			this->Destroy();

			if (scroll != nullptr)
				scroll->FitInside();

			{
				Variant variant;
				variant.data = settings->getCurrentVariantList(type);
				variant.name = list->GetStringSelection();

				settings->setVariant(type, variant);
			}

			sizer->Layout();
		});

		{
			const auto Changes = [this](const wxCommandEvent& event)
			{
				MakeChanges();
			};

			_choice->Bind(wxEVT_CHOICE, Changes);
			_count_spin->Bind(wxEVT_TEXT, Changes);
			_prop_spin->Bind(wxEVT_TEXT, Changes);

			if (this->isNPCType())
				_check->Bind(wxEVT_CHECKBOX, Changes);
		}
	}
}

VariantPanel::~VariantPanel()
{
}

void VariantPanel::MakeChanges() noexcept
{
	if (_choice->GetSelection() != -1)
	{
		Variant variant;
		variant.data = _settings->getCurrentVariantList(_type);
		variant.name = _list->GetStringSelection();

		_settings->setVariant(_type, variant);
	}
}

constexpr bool VariantPanel::isNPCType() const
{
	return (_type == SettingType::NPC);
}

void VariantPanel::setVisible(const bool enable)
{
	_choice->Show(enable);
	_count_spin->Show(enable);
	_prop_spin->Show(enable);

	if (_editable)
	{
		_staticline->Show(enable);
		_del_button->Show(enable);
	}
}

VariantData VariantPanel::getVariant() const noexcept
{
	VariantData data;

	data.desc = _choice->GetStringSelection();
	data.spawn_number = _count_spin->GetValue();
	data.spawn_prop = static_cast<float>(_prop_spin->GetValue());

	if (this->isNPCType())
		data.ignore_players_number = _check->IsChecked();

	return data;
}

void VariantPanel::setFromVariant(const VariantData& data) noexcept
{
	const int found_string = _choice->FindString(data.desc, true);

	_choice->SetSelection(found_string);
	_count_spin->SetValue(data.spawn_number);
	_prop_spin->SetValue(static_cast<double>(data.spawn_prop));

	if (this->isNPCType())
		_check->SetValue(data.ignore_players_number);
}

std::string VariantPanel::getRandomChoiceFomItemsList() const noexcept
{
	const auto item_count = _choice->GetCount();

	if (item_count > 0u)
	{
		std::random_device rd;
		std::mt19937_64 gen(rd());
		std::uniform_int_distribution<uint32_t> uid(0u, item_count);

		const auto random_index = uid(gen);

		return _choice->GetString(random_index);
	}
	else
	{
		return {};
	}
}
