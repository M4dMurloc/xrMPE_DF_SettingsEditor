#ifndef _SELECT_ITEM_DIALOG_HPP_
#define _SELECT_ITEM_DIALOG_HPP_

#include <vector>
#include <map>
#include <string>
#include <wx/dialog.h>


class wxCheckListBox;
class wxPanel;
class wxButton;


class SelectItemDialog final : public wxDialog
{
public:
	SelectItemDialog(wxWindow* parent, wxWindowID id = wxID_ANY,
					 const wxString& title = wxEmptyString,
					 const wxPoint& pos = wxDefaultPosition,
					 const wxSize& size = wxSize(651, 543),
					 long style = wxDEFAULT_DIALOG_STYLE | wxMAXIMIZE_BOX | wxMINIMIZE_BOX | wxRESIZE_BORDER);
	~SelectItemDialog();

	void fillList(const std::map<std::string, bool>& list);

	const std::map<std::string, bool>& getResultList() const { return _result_list; }

private:
	enum Actions : unsigned int
	{
		ADD_ID = 2000,
		REMOVE_ID,
		RENAME_ID,
		ACCEPT_ID
	};

	wxCheckListBox* _list = nullptr;
	wxPanel* _button_panel = nullptr;
	wxButton* _add_btn = nullptr;
	wxButton* _remove_btn = nullptr;
	wxButton* _rename_btn = nullptr;
	wxButton* _accept_btn = nullptr;

	void connectActions();

	std::map<std::string, bool> _result_list;

};

#endif
