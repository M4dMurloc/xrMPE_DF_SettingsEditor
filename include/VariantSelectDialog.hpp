#ifndef _VARIANT_SELECT_DIALOG_HPP_
#define _VARIANT_SELECT_DIALOG_HPP_

#include <map>
#include <wx/dialog.h>
#include "Types.hpp"


class wxListBox;
class wxPanel;
class wxButton;

class VariantSelectDialog final : public wxDialog
{
public:
	VariantSelectDialog(wxWindow* parent, wxWindowID id = wxID_ANY,
						const wxString& title = wxT("Select Variants"),
						const wxPoint& pos = wxDefaultPosition,
						const wxSize& size = wxSize(768, 640),
						long style = wxCAPTION | wxCLOSE_BOX | wxMAXIMIZE_BOX | wxMINIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU);
	~VariantSelectDialog();

	void fillList(const std::map<std::string, Variant>& variants);

	std::vector<std::string> getResultList() const;

private:
	wxListBox* _list = nullptr;
	wxPanel* _button_panel = nullptr;
	wxButton* _accept_btn = nullptr;

};

#endif
