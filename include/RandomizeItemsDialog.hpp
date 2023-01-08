#ifndef _RANDOMIZE_ITEMS_DIALOG_HPP_
#define _RANDOMIZE_ITEMS_DIALOG_HPP_

#include <wx/dialog.h>

class wxSpinCtrl;
class wxSpinCtrlDouble;


class RandomItemsDialog final : public wxDialog
{
public:
	RandomItemsDialog(wxWindow* parent = nullptr, wxWindowID id = wxID_ANY, const wxString& title = wxString::FromUTF8("Random create"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(210, 250), long style = wxDEFAULT_DIALOG_STYLE);
	~RandomItemsDialog();

	struct RandomData final
	{
		template<typename T>
		struct range final
		{
			T min = 0;
			T max = 1;
		};

		uint32_t items_count = 0u;
		range<uint32_t> count_range;
		range<float> prop_range;
	};

	RandomData getRandomData() const noexcept;

protected:
	void connectActions() noexcept;

private:
	wxPanel* _settings_panel = nullptr;
	wxSpinCtrl* _count_spin = nullptr;
	wxSpinCtrl* _count_min_spin = nullptr;
	wxSpinCtrl* _count_max_spin = nullptr;
	wxSpinCtrlDouble* _prop_min_spin = nullptr;
	wxSpinCtrlDouble* _prop_max_spin = nullptr;
	wxPanel* _btn_panel = nullptr;
	wxButton* _generate_button = nullptr;

};

#endif
