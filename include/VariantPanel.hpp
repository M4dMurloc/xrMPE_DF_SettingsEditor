#ifndef _VARIANT_PANEL_HPP_
#define _VARIANT_PANEL_HPP_

#include <wx/panel.h>
#include <Types.hpp>

class wxChoice;
class wxSpinCtrl;
class wxSpinCtrlDouble;
class wxCheckBox;
class wxStaticLine;
class wxButton;
class wxScrolledWindow;
class wxListBox;

class Settings;


class VariantPanel : public wxPanel
{
public:
	VariantPanel(wxScrolledWindow* parent, Settings* settings, SettingType type, wxListBox* list, const bool editable);
	~VariantPanel();

	VariantData getVariant() const noexcept;
	void setFromVariant(const VariantData& data) noexcept;

	const bool isEditable() const { return _editable; }

	void setVisible(const bool enable);

	std::string getRandomChoiceFomItemsList() const noexcept;

	void remember() noexcept
	{
		MakeChanges();
	}

protected:
	void MakeChanges() noexcept;

	constexpr bool isNPCType() const;

private:
	wxChoice* _choice = nullptr;
	wxSpinCtrl* _count_spin = nullptr;
	wxSpinCtrlDouble* _prop_spin = nullptr;
	wxCheckBox* _check = nullptr;
	wxStaticLine* _staticline = nullptr;
	wxButton* _del_button = nullptr;

	wxScrolledWindow* _scroll = nullptr;
	Settings* _settings = nullptr;

	wxListBox* _list = nullptr;
	SettingType _type;

	const bool _editable = false;

};

#endif
