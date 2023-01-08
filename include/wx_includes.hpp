#ifndef _WX_INCLUDES_HPP_
#define _WX_INCLUDES_HPP_

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/checklst.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/splitter.h>
#include <wx/aui/auibook.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/scrolwin.h>
#include <wx/textdlg.h>


#define utf8 wxString::FromUTF8

#define PNG_RC(ID) (wxBitmap(#ID, wxBITMAP_TYPE_PNG_RESOURCE))

#endif
