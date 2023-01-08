#include <iostream>
#include <wx/app.h>
#include <wx/image.h>
#include <wx/textctrl.h>
#include "Frame.hpp"


class Application : public wxApp
{
public:
	Application()
	{
	}

	~Application()
	{
		delete _redirect;
		_redirect = nullptr;
	}

	virtual bool OnInit() override;

private:
	wxStreamToTextRedirector* _redirect = nullptr;

};

#ifdef _CONSOLE
wxIMPLEMENT_APP_CONSOLE(Application);
#else
wxIMPLEMENT_APP(Application);
#endif

bool Application::OnInit()
{
	std::ios::sync_with_stdio(true);

	wxImage::AddHandler(new wxPNGHandler());

	if (!wxApp::OnInit())
		return false;

	auto frame = new SettingsFrame;

	_redirect = new wxStreamToTextRedirector(frame->getLog());

	frame->SetIcon(wxIcon("APP_ICON", wxBITMAP_TYPE_ICO_RESOURCE));

	frame->Show(true);

	return true;
}
