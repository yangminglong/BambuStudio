#include "ACToolbar.hpp"
#include "wx/artprov.h"
#include "wx/aui/framemanager.h"
#include "I18N.hpp"
#include "GUI_App.hpp"
#include "GUI.hpp"
#include "wxExtensions.hpp"
#include "Plater.hpp"
#include "MainFrame.hpp"
#include "WebViewDialog.hpp"
#include "PartPlate.hpp"

#define TOPBAR_ICON_SIZE  18

using namespace Slic3r;

//enum CUSTOM_TOOL_ID
//{
//    ID_TOP_MENU_TOOL = 3100,
//    ID_LOGO,
//    ID_TOP_FILE_MENU,
//    ID_TOP_EDIT_MENU,
//    ID_TOP_VIEW_MENU,
//    ID_TOP_SETS_MENU,
//    ID_TOP_HELP_MENU,
//    //ID_TOP_DROPDOWN_MENU,
//    ID_TITLE,
//    ID_MODEL_STORE,
//    ID_PUBLISH,
//    ID_TOOL_BAR = 3200,
//    ID_AMS_NOTEBOOK,
//};

class ACToolBarArt : public wxAuiDefaultToolBarArt
{
public:
    //virtual void DrawLabel(wxDC& dc, wxWindow* wnd, const wxAuiToolBarItem& item, const wxRect& rect) wxOVERRIDE;
    virtual void DrawBackground(wxDC& dc, wxWindow* wnd, const wxRect& rect) wxOVERRIDE;
    virtual void DrawButton(wxDC& dc, wxWindow* wnd, const wxAuiToolBarItem& item, const wxRect& rect) wxOVERRIDE;
private:
    // button pen 175, 185, 204
};
//
//void ACToolBarArt::DrawLabel(wxDC& dc, wxWindow* wnd, const wxAuiToolBarItem& item, const wxRect& rect)
//{
//    dc.SetFont(m_font);
//#ifdef __WINDOWS__
//    dc.SetTextForeground(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));
//#else
//    dc.SetTextForeground(*wxWHITE);
//#endif
//
//    wxAuiToolBar* toolbar = (wxAuiToolBar*)wnd;
//    wxSize toolBarSize = toolbar->GetSize();
//
//    int textWidth = 0, textHeight = 0;
//    dc.GetTextExtent(item.GetLabel(), &textWidth, &textHeight);
//
//    wxRect clipRect = rect;
//    clipRect.width -= 1;
//    dc.SetClippingRegion(clipRect);
//
//    int textX, textY;
//    int leftSpace = max(0, toolBarSize.x/2-rect.x);
//
//    if (textWidth > leftSpace*2)  {
//        textX = rect.x + 1;
//    }
//    else {
//        textX = toolBarSize.GetWidth()/2- textWidth/2;
//    }
//
//    //if (textWidth < rect.GetWidth()) {
//    //    textX = rect.x + 1 + (rect.width - textWidth) / 2;
//    //}
//    //else {
//    //    textX = rect.x + 1;
//    //}
//
//    textY = rect.y + (rect.height - textHeight) / 2;
//    dc.DrawText(item.GetLabel(), textX, textY);
//    dc.DestroyClippingRegion();
//}

void ACToolBarArt::DrawBackground(wxDC& dc, wxWindow* wnd, const wxRect& rect)
{
    //dc.SetBrush(wxBrush(wxColour(38, 46, 48))); //
    dc.SetBrush(wxBrush(wxColour(255,255,255))); //55, 66, 84  255, 255, 255
    dc.SetPen(wxPen(wxColour(255, 255, 255))); //55, 66, 84
    wxRect clipRect = rect;
    clipRect.y -= 8;
    clipRect.height += 8;
    dc.SetClippingRegion(clipRect);
    dc.DrawRectangle(rect);
    dc.DestroyClippingRegion();
}

void ACToolBarArt::DrawButton(wxDC& dc, wxWindow* wnd, const wxAuiToolBarItem& item, const wxRect& rect)
{
    int textWidth = 0, textHeight = 0;

    if (m_flags & wxAUI_TB_TEXT)
    {
        dc.SetFont(m_font);
        int tx, ty;

        dc.GetTextExtent(wxT("ABCDHgj"), &tx, &textHeight);
        textWidth = 0;
        dc.GetTextExtent(item.GetLabel(), &textWidth, &ty);
    }

    int bmpX = 0, bmpY = 0;
    int textX = 0, textY = 0;

    const wxBitmap& bmp = item.GetState() & wxAUI_BUTTON_STATE_DISABLED
        ? item.GetDisabledBitmap()
        : item.GetBitmap();

    const wxSize bmpSize = bmp.IsOk() ? bmp.GetScaledSize() : wxSize(0, 0);

    if (m_textOrientation == wxAUI_TBTOOL_TEXT_BOTTOM)
    {
        bmpX = rect.x +
            (rect.width / 2) -
            (bmpSize.x / 2);

        bmpY = rect.y +
            ((rect.height - textHeight) / 2) -
            (bmpSize.y / 2);

        textX = rect.x + (rect.width / 2) - (textWidth / 2) + 1;
        textY = rect.y + rect.height - textHeight - 1;
    }
    else if (m_textOrientation == wxAUI_TBTOOL_TEXT_RIGHT)
    {
        bmpX = rect.x + wnd->FromDIP(3);

        bmpY = rect.y +
            (rect.height / 2) -
            (bmpSize.y / 2);

        textX = bmpX + wnd->FromDIP(3) + bmpSize.x;
        textY = rect.y +
            (rect.height / 2) -
            (textHeight / 2);
    }
    
    wxColour btBgPenColor = (int)wxID_ADD == item.GetId() ? wxColour(57, 134, 255) : wxColour(188, 196, 212, 0);
    wxColour btBgPenBrush = (int)wxID_ADD == item.GetId() ? wxColour(57, 134, 255) : wxColour(255, 255, 255, 0);

    dc.SetPen(wxPen(btBgPenColor));
    dc.SetBrush(wxBrush(btBgPenBrush));
    dc.DrawRoundedRectangle(rect, wnd->FromDIP(8));

    //bool isMenu = (int)ID_TOP_FILE_MENU == item.GetId() ||
    //              (int)ID_TOP_EDIT_MENU == item.GetId() ||
    //              (int)ID_TOP_VIEW_MENU == item.GetId() ||
    //              (int)ID_TOP_SETS_MENU == item.GetId() ||
    //              (int)ID_TOP_HELP_MENU == item.GetId();

    //double roundRadius = isMenu ? 0 : wnd->FromDIP(5);
    //if (!(item.GetState() & wxAUI_BUTTON_STATE_DISABLED))
    //{
    //    if (item.GetState() & wxAUI_BUTTON_STATE_PRESSED)
    //    {
    //        dc.SetPen(wxPen(m_highlightColour));
    //        dc.SetBrush(wxBrush(m_highlightColour.ChangeLightness(20)));
    //        dc.DrawRoundedRectangle(rect, roundRadius);
    //    }
    //    else if ((item.GetState() & wxAUI_BUTTON_STATE_HOVER) || item.IsSticky())
    //    {
    //        dc.SetPen(wxPen(m_highlightColour));
    //        dc.SetBrush(wxBrush(m_highlightColour.ChangeLightness(40)));

    //        // draw an even lighter background for checked item hovers (since
    //        // the hover background is the same color as the check background)
    //        if (item.GetState() & wxAUI_BUTTON_STATE_CHECKED)
    //            dc.SetBrush(wxBrush(m_highlightColour.ChangeLightness(50)));

    //        dc.DrawRoundedRectangle(rect, roundRadius);
    //    }
    //    else if (item.GetState() & wxAUI_BUTTON_STATE_CHECKED)
    //    {
    //        // it's important to put this code in an else statement after the
    //        // hover, otherwise hovers won't draw properly for checked items
    //        dc.SetPen(wxPen(m_highlightColour));
    //        dc.SetBrush(wxBrush(m_highlightColour.ChangeLightness(40)));
    //        dc.DrawRoundedRectangle(rect, roundRadius);
    //    }
    //    else if (!isMenu && item.GetState() == (int)wxAUI_BUTTON_STATE_NORMAL) 
    //    {
    //        dc.SetPen(wxPen(wxColour(70, 79, 96)));
    //        dc.SetBrush(wxBrush(wxColour(70, 79, 96)));
    //        dc.DrawRoundedRectangle(rect, roundRadius);
    //    }
    //}

    if (bmp.IsOk())
        dc.DrawBitmap(bmp, bmpX, bmpY, true);

    wxColour textFgColor = (int)wxID_ADD == item.GetId() ? wxColour(255, 255, 255) : wxColour(67, 87, 123);

    dc.SetTextForeground(textFgColor);
    // set the item's text color based on if it is disabled
//#ifdef __WINDOWS__
//    dc.SetTextForeground(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));
//#else
//    dc.SetTextForeground(*wxWHITE);
//#endif

    if (item.GetState() & wxAUI_BUTTON_STATE_DISABLED)
    {
        dc.SetTextForeground(wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT));
    }

    if ((m_flags & wxAUI_TB_TEXT) && !item.GetLabel().empty())
    {
        dc.DrawText(item.GetLabel(), textX, textY);
    }
}

//
//TitleBar::TitleBar(wxWindow* parent)
//    : wxControl(parent)
//{
//#ifdef __WINDOWS__
//    SetDoubleBuffered(true);
//#endif //__WINDOWS__
//
//    m_sizer = new wxBoxSizer(wxHORIZONTAL);
//    this->SetSizer(m_sizer);
//}
//
//void TitleBar::Rescale()
//{
//    m_menuBar.Rescale();
//}


ACToolBar::ACToolBar(wxFrame* parent) 
    : wxAuiToolBar(parent, ID_TOOL_BAR, wxDefaultPosition, wxDefaultSize, wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT)
{ 
    Init(parent);
}

ACToolBar::ACToolBar(wxWindow* pwin, wxFrame* parent)
    : wxAuiToolBar(pwin, ID_TOOL_BAR, wxDefaultPosition, wxDefaultSize, wxAUI_TB_TEXT | wxAUI_TB_HORZ_TEXT) 
{ 
    Init(parent);
}

void ACToolBar::Init(wxFrame* parent) 
{
    SetArtProvider(new ACToolBarArt());
    m_frame = parent;
    //m_skip_popup_file_menu = false;
    //m_skip_popup_edit_menu = false;
    //m_skip_popup_view_menu = false;
    //m_skip_popup_sets_menu = false;
    //m_skip_popup_help_menu = false;
    //m_skip_popup_dropdown_menu = false;

    //SetSizerAndFit();

    

    wxInitAllImageHandlers();

    //this->AddSpacer(FromDIP(5));

    //wxBitmap logo_bitmap = create_scaled_bitmap("topbar_logo", nullptr, TOPBAR_ICON_SIZE);
    //wxAuiToolBarItem* logo_item = this->AddTool(ID_LOGO, "", logo_bitmap);
    //logo_item->SetActive(false);
    //logo_item->SetHoverBitmap(logo_bitmap);

    //this->AddSpacer(FromDIP(5));

    /*wxBitmap logo_bitmap = create_scaled_bitmap("topbar_logo", nullptr, TOPBAR_ICON_SIZE);
    wxAuiToolBarItem* logo_item = this->AddTool(ID_LOGO, "", logo_bitmap);
    logo_item->SetHoverBitmap(logo_bitmap);
    logo_item->SetActive(false);*/

    // wxBitmap file_bitmap = create_scaled_bitmap("topbar_file", nullptr, TOPBAR_ICON_SIZE);
    //m_file_menu_item = this->AddTool(ID_TOP_FILE_MENU, _L("File"), wxBitmap(), wxEmptyString, wxITEM_NORMAL);
    //m_edit_menu_item = this->AddTool(ID_TOP_EDIT_MENU, _L("Edit"), wxBitmap(), wxEmptyString, wxITEM_NORMAL);
    //m_view_menu_item = this->AddTool(ID_TOP_VIEW_MENU, _L("View"), wxBitmap(), wxEmptyString, wxITEM_NORMAL);
    //m_sets_menu_item = this->AddTool(ID_TOP_SETS_MENU, _L("Settings"), wxBitmap(), wxEmptyString, wxITEM_NORMAL);
    //m_help_menu_item = this->AddTool(ID_TOP_HELP_MENU, _L("Help"), wxBitmap(), wxEmptyString, wxITEM_NORMAL);

    this->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));

    //this->AddSpacer(FromDIP(5));

    //wxBitmap dropdown_bitmap = create_scaled_bitmap("topbar_dropdown", nullptr, TOPBAR_ICON_SIZE);
    //m_dropdown_menu_item = this->AddTool(ID_TOP_DROPDOWN_MENU, "",
    //    dropdown_bitmap, wxEmptyString);

    //this->AddSpacer(FromDIP(5));
    //this->AddSeparator();
    //this->AddSpacer(FromDIP(5));

    //wxBitmap open_bitmap = create_scaled_bitmap("topbar_open", nullptr, TOPBAR_ICON_SIZE);
    //wxAuiToolBarItem* tool_item = this->AddTool(wxID_OPEN, "", open_bitmap);

    wxBitmap add_bitmap = create_scaled_bitmap("icon-add-nor", nullptr, TOPBAR_ICON_SIZE); //57, 134, 255
    wxAuiToolBarItem* addToPlate_btn = this->AddTool(wxID_ADD, "Import", add_bitmap);

    this->AddSpacer(FromDIP(10));
    this->AddSeparator();
    this->AddSpacer(FromDIP(10));

    wxBitmap save_bitmap = create_scaled_bitmap("icon-save-nor", nullptr, TOPBAR_ICON_SIZE);
    wxBitmap save_bitmap_inactive = create_scaled_bitmap("icon-save-inactive", nullptr, TOPBAR_ICON_SIZE);
    wxBitmap save_bitmap_hover = create_scaled_bitmap("icon-save-hover", nullptr, TOPBAR_ICON_SIZE);
    wxAuiToolBarItem* save_btn = this->AddTool(wxID_SAVE, "", save_bitmap);
    save_btn->SetDisabledBitmap(save_bitmap_inactive);
    save_btn->SetHoverBitmap(save_bitmap_hover);

    this->AddSpacer(FromDIP(10));

    wxBitmap undo_bitmap = create_scaled_bitmap("icon-undo-nor", nullptr, TOPBAR_ICON_SIZE);
    wxBitmap undo_bitmap_inactive = create_scaled_bitmap("icon-undo-inactive", nullptr, TOPBAR_ICON_SIZE);
    wxBitmap undo_bitmap_hover = create_scaled_bitmap("icon-undo-hover", nullptr, TOPBAR_ICON_SIZE);
    m_undo_item = this->AddTool(wxID_UNDO, "", undo_bitmap);
    m_undo_item->SetDisabledBitmap(undo_bitmap_inactive);
    m_undo_item->SetHoverBitmap(undo_bitmap_hover);

    this->AddSpacer(FromDIP(10));

    wxBitmap redo_bitmap = create_scaled_bitmap("icon-redo-nor", nullptr, TOPBAR_ICON_SIZE);
    wxBitmap redo_bitmap_inactive = create_scaled_bitmap("icon-redo-inactive", nullptr, TOPBAR_ICON_SIZE);
    wxBitmap redo_bitmap_hover = create_scaled_bitmap("icon-redo-hover", nullptr, TOPBAR_ICON_SIZE);
    m_redo_item = this->AddTool(wxID_REDO, "", redo_bitmap);
    m_redo_item->SetDisabledBitmap(redo_bitmap_inactive);
    m_redo_item->SetHoverBitmap(redo_bitmap_hover);

    //this->AddStretchSpacer(1);

    //m_title_item = this->AddLabel(ID_TITLE, "", FromDIP(TOPBAR_TITLE_WIDTH));
    //m_title_item->SetAlignment(wxALIGN_CENTRE);
    //m_title_item->SetProportion(1);

    this->AddStretchSpacer(1);

    wxBitmap config_bitmap = create_scaled_bitmap("icon-configuration manage-nor", nullptr, TOPBAR_ICON_SIZE);
    wxBitmap config_bitmap_inactive = create_scaled_bitmap("icon-configuration manage-inactive", nullptr, TOPBAR_ICON_SIZE);
    m_config_item = this->AddTool(wxID_CONFIGURATION, "Configuration Manage", config_bitmap);
    m_config_item->SetDisabledBitmap(config_bitmap_inactive);

#if !BBL_RELEASE_TO_PUBLIC
    /*wxBitmap m_publish_bitmap = create_scaled_bitmap("topbar_publish", nullptr, TOPBAR_ICON_SIZE);
    m_publish_item            = this->AddTool(ID_PUBLISH, "", m_publish_bitmap);
    wxBitmap m_publish_disable_bitmap = create_scaled_bitmap("topbar_publish_disable", nullptr, TOPBAR_ICON_SIZE);
    m_publish_item->SetDisabledBitmap(m_publish_disable_bitmap);
    this->AddSpacer(FromDIP(12));*/
#endif

    /*wxBitmap model_store_bitmap = create_scaled_bitmap("topbar_store", nullptr, TOPBAR_ICON_SIZE);
    m_model_store_item = this->AddTool(ID_MODEL_STORE, "", model_store_bitmap);
    this->AddSpacer(12);
    */

    //this->AddSeparator();
    //this->AddSpacer(FromDIP(6));

    //wxBitmap iconize_bitmap = create_scaled_bitmap("topbar_min", nullptr, TOPBAR_ICON_SIZE);
    //wxAuiToolBarItem* iconize_btn = this->AddTool(wxID_ICONIZE_FRAME, "", iconize_bitmap);

    //this->AddSpacer(FromDIP(6));

    //maximize_bitmap = create_scaled_bitmap("topbar_max", nullptr, TOPBAR_ICON_SIZE);
    //window_bitmap = create_scaled_bitmap("topbar_win", nullptr, TOPBAR_ICON_SIZE);
    //if (m_frame->IsMaximized()) {
    //    maximize_btn = this->AddTool(wxID_MAXIMIZE_FRAME, "", window_bitmap);
    //}
    //else {
    //    maximize_btn = this->AddTool(wxID_MAXIMIZE_FRAME, "", maximize_bitmap);
    //}

    //this->AddSpacer(FromDIP(6));

    //wxBitmap close_bitmap = create_scaled_bitmap("topbar_close", nullptr, TOPBAR_ICON_SIZE);
    //wxAuiToolBarItem* close_btn = this->AddTool(wxID_CLOSE_FRAME, "", close_bitmap);

    //this->AddSpacer(FromDIP(6));

    Realize();
    // m_toolbar_h = this->GetSize().GetHeight();
    //m_toolbar_h = FromDIP(30);

    int client_w = parent->GetClientSize().GetWidth();
    this->SetSize(client_w, m_toolbar_h);

    //this->Bind(wxEVT_MOTION, &ACToolBar::OnMouseMotion, this);
    //this->Bind(wxEVT_MOUSE_CAPTURE_LOST, &ACToolBar::OnMouseCaptureLost, this);
    //this->Bind(wxEVT_MENU_CLOSE, &ACToolBar::OnMenuClose, this);
    //this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnFileToolItem, this, ID_TOP_FILE_MENU);
    //this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnEditToolItem, this, ID_TOP_EDIT_MENU);
    //this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnViewToolItem, this, ID_TOP_VIEW_MENU);
    //this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnSetsToolItem, this, ID_TOP_SETS_MENU);
    //this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnHelpToolItem, this, ID_TOP_HELP_MENU);
    ////this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnDropdownToolItem, this, ID_TOP_DROPDOWN_MENU);
    //this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnIconize, this, wxID_ICONIZE_FRAME);
    //this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnFullScreen, this, wxID_MAXIMIZE_FRAME);
    //this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnCloseFrame, this, wxID_CLOSE_FRAME);
    //this->Bind(wxEVT_LEFT_DCLICK, &ACToolBar::OnMouseLeftDClock, this);
    //this->Bind(wxEVT_LEFT_DOWN, &ACToolBar::OnMouseLeftDown, this);
    //this->Bind(wxEVT_LEFT_UP, &ACToolBar::OnMouseLeftUp, this);
    //this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnOpenProject, this, wxID_OPEN);
    this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnAddToPlate, this, wxID_ADD);
    this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnSaveProject, this, wxID_SAVE);
    this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnRedo, this, wxID_REDO);
    this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnUndo, this, wxID_UNDO);
    this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnOpenConfigDialog, this, wxID_CONFIGURATION);
    //this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnModelStoreClicked, this, ID_MODEL_STORE);
    //this->Bind(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, &ACToolBar::OnPublishClicked, this, ID_PUBLISH);
}

ACToolBar::~ACToolBar()
{
    //m_file_menu_item = nullptr;
    //m_edit_menu_item = nullptr;
    //m_view_menu_item = nullptr;
    //m_sets_menu_item = nullptr;
    //m_help_menu_item = nullptr;
    ////m_dropdown_menu_item = nullptr;
    //m_file_menu = nullptr;
}
//
//void ACToolBar::OnOpenProject(wxAuiToolBarEvent& event)
//{
//    MainFrame* main_frame = dynamic_cast<MainFrame*>(m_frame);
//    Plater* plater = main_frame->plater();
//    plater->load_project();
//}

void ACToolBar::OnAddToPlate(wxAuiToolBarEvent& event)
{
    MainFrame* main_frame = dynamic_cast<MainFrame*>(m_frame);
    Plater* plater = main_frame->plater();
    plater->add_model();
}

void ACToolBar::OnSaveProject(wxAuiToolBarEvent& event)
{
    MainFrame* main_frame = dynamic_cast<MainFrame*>(m_frame);
    Plater* plater = main_frame->plater();
    plater->save_project();
}

void ACToolBar::OnUndo(wxAuiToolBarEvent& event)
{
    MainFrame* main_frame = dynamic_cast<MainFrame*>(m_frame);
    Plater* plater = main_frame->plater();
    plater->undo();
}

void ACToolBar::OnRedo(wxAuiToolBarEvent& event)
{
    MainFrame* main_frame = dynamic_cast<MainFrame*>(m_frame);
    Plater* plater = main_frame->plater();
    plater->redo();
}

void ACToolBar::OnOpenConfigDialog(wxAuiToolBarEvent& event)
{
    MainFrame* main_frame = dynamic_cast<MainFrame*>(m_frame);
    Plater* plater = main_frame->plater();
    //plater->openConfigDialog();
}

void ACToolBar::EnableUndoRedoItems()
{
    this->EnableTool(m_undo_item->GetId(), true);
    this->EnableTool(m_redo_item->GetId(), true);
    Refresh();
}

void ACToolBar::DisableUndoRedoItems()
{
    this->EnableTool(m_undo_item->GetId(), false);
    this->EnableTool(m_redo_item->GetId(), false);
    Refresh();
}
//
//void ACToolBar::SaveNormalRect()
//{
//    m_normalRect = m_frame->GetRect();
//}
//
//void ACToolBar::OnModelStoreClicked(wxAuiToolBarEvent& event)
//{
//    //GUI::wxGetApp().load_url(wxString(wxGetApp().app_config->get_web_host_url() + MODEL_STORE_URL));
//}
//
//void ACToolBar::OnPublishClicked(wxAuiToolBarEvent& event)
//{
//    if (GUI::wxGetApp().plater()->model().objects.empty()) return;
//
//    if (!wxGetApp().is_user_login()) return;
//
//    wxGetApp().plater()->show_publish_dialog();
//}
//
//void ACToolBar::SetFileMenu(wxMenu* file_menu)
//{
//    m_file_menu = file_menu;
//}
//
//void ACToolBar::SetEditMenu(wxMenu* edit_menu) 
//{ 
//    m_edit_menu = edit_menu;
//}
//
//void ACToolBar::SetViewMenu(wxMenu* view_menu) 
//{
//    m_view_menu = view_menu;
//}
//
//void ACToolBar::SetSetsMenu(wxMenu* sets_menu) 
//{
//    m_sets_menu = sets_menu;
//}
//
//void ACToolBar::SetHelpMenu(wxMenu* help_menu) 
//{ 
//    m_help_menu = help_menu;
//}

//void ACToolBar::AddDropDownSubMenu(wxMenu* sub_menu, const wxString& title)
//{
//    m_top_menu.AppendSubMenu(sub_menu, title);
//}
//
//void ACToolBar::AddDropDownMenuItem(wxMenuItem* menu_item)
//{
//    m_top_menu.Append(menu_item);
//}
//
//wxMenu* ACToolBar::GetTopMenu()
//{
//    return &m_top_menu;
//}
//
//void ACToolBar::SetTitle(wxString title)
//{
//    wxGCDC dc(this);
//    title = wxControl::Ellipsize(title, dc, wxELLIPSIZE_END, FromDIP(TOPBAR_TITLE_WIDTH));
//
//    m_title_item->SetLabel(title);
//    m_title_item->SetAlignment(wxALIGN_CENTRE);
//    this->Refresh();
//}
//
//void ACToolBar::SetMaximizedSize()
//{
//    maximize_btn->SetBitmap(maximize_bitmap);
//}
//
//void ACToolBar::SetWindowSize()
//{
//    maximize_btn->SetBitmap(window_bitmap);
//}

void ACToolBar::UpdateToolbarWidth(int width)
{
    this->SetSize(width, m_toolbar_h);
}

void ACToolBar::Rescale() {
    int em = em_unit(this);
    wxAuiToolBarItem* item;

    //item = this->FindTool(ID_LOGO);
    //item->SetBitmap(create_scaled_bitmap("topbar_logo", nullptr, TOPBAR_ICON_SIZE));

    //item = this->FindTool(ID_TOP_FILE_MENU); item->SetBitmap(create_scaled_bitmap("topbar_file", this, TOPBAR_ICON_SIZE));
    //item = this->FindTool(ID_TOP_EDIT_MENU); item->SetBitmap(create_scaled_bitmap("topbar_edit", this, TOPBAR_ICON_SIZE));
    //item = this->FindTool(ID_TOP_VIEW_MENU); item->SetBitmap(create_scaled_bitmap("topbar_view", this, TOPBAR_ICON_SIZE));
    //item = this->FindTool(ID_TOP_SETS_MENU); item->SetBitmap(create_scaled_bitmap("topbar_sets", this, TOPBAR_ICON_SIZE));
    //item = this->FindTool(ID_TOP_HELP_MENU); item->SetBitmap(create_scaled_bitmap("topbar_help", this, TOPBAR_ICON_SIZE));

    //item = this->FindTool(ID_TOP_DROPDOWN_MENU);
    //item->SetBitmap(create_scaled_bitmap("topbar_dropdown", this, TOPBAR_ICON_SIZE));

    //item = this->FindTool(wxID_OPEN);
    //item->SetBitmap(create_scaled_bitmap("topbar_open", nullptr, TOPBAR_ICON_SIZE));

  
    item = this->FindTool(wxID_ADD);
    item->SetBitmap(create_scaled_bitmap("icon-add-nor", this, TOPBAR_ICON_SIZE));
    

    item = this->FindTool(wxID_SAVE);
    item->SetBitmap(create_scaled_bitmap("icon-save-nor", this, TOPBAR_ICON_SIZE));
    item->SetHoverBitmap(create_scaled_bitmap("icon-save-hover", this, TOPBAR_ICON_SIZE));

    item = this->FindTool(wxID_UNDO);
    item->SetBitmap(create_scaled_bitmap("icon-undo-nor", this, TOPBAR_ICON_SIZE));
    item->SetDisabledBitmap(create_scaled_bitmap("icon-undo-inactive", nullptr, TOPBAR_ICON_SIZE));
    item->SetHoverBitmap(create_scaled_bitmap("icon-undo-hover", nullptr, TOPBAR_ICON_SIZE));

    item = this->FindTool(wxID_REDO);
    item->SetBitmap(create_scaled_bitmap("icon-redo-nor", this, TOPBAR_ICON_SIZE));
    item->SetDisabledBitmap(create_scaled_bitmap("icon-redo-inactive", nullptr, TOPBAR_ICON_SIZE));
    item->SetHoverBitmap(create_scaled_bitmap("icon-redo-hover", nullptr, TOPBAR_ICON_SIZE));

    item = this->FindTool(wxID_CONFIGURATION);
    item->SetBitmap(create_scaled_bitmap("icon-configuration manage-nor", this, TOPBAR_ICON_SIZE));
    item->SetDisabledBitmap(create_scaled_bitmap("icon-configuration manage-inactive", nullptr, TOPBAR_ICON_SIZE));

    //item = this->FindTool(ID_TITLE);

    /*item = this->FindTool(ID_PUBLISH);
    item->SetBitmap(create_scaled_bitmap("topbar_publish", this, TOPBAR_ICON_SIZE));
    item->SetDisabledBitmap(create_scaled_bitmap("topbar_publish_disable", nullptr, TOPBAR_ICON_SIZE));*/

    /*item = this->FindTool(ID_MODEL_STORE);
    item->SetBitmap(create_scaled_bitmap("topbar_store", this, TOPBAR_ICON_SIZE));
    */

    //item = this->FindTool(wxID_ICONIZE_FRAME);
    //item->SetBitmap(create_scaled_bitmap("topbar_min", this, TOPBAR_ICON_SIZE));

    //item = this->FindTool(wxID_MAXIMIZE_FRAME);
    //maximize_bitmap = create_scaled_bitmap("topbar_max", this, TOPBAR_ICON_SIZE);
    //window_bitmap   = create_scaled_bitmap("topbar_win", this, TOPBAR_ICON_SIZE);
    //if (m_frame->IsMaximized()) {
    //    item->SetBitmap(window_bitmap);
    //}
    //else {
    //    item->SetBitmap(maximize_bitmap);
    //}

    //item = this->FindTool(wxID_CLOSE_FRAME);
    //item->SetBitmap(create_scaled_bitmap("topbar_close", this, TOPBAR_ICON_SIZE));

    Realize();
}
//
//void ACToolBar::OnIconize(wxAuiToolBarEvent& event)
//{
//    m_frame->Iconize();
//}
//
//void ACToolBar::OnFullScreen(wxAuiToolBarEvent& event)
//{
//    if (m_frame->IsMaximized()) {
//        m_frame->Restore();
//    }
//    else {
//        wxDisplay display(this);
//        auto      size = display.GetClientArea().GetSize();
//        m_frame->SetMaxSize(size + wxSize{16, 16});
//        m_normalRect = m_frame->GetRect();
//        m_frame->Maximize();
//    }
//}
//
//void ACToolBar::OnCloseFrame(wxAuiToolBarEvent& event)
//{
//    m_frame->Close();
//}
//
//void ACToolBar::OnMouseLeftDClock(wxMouseEvent& mouse)
//{
//    wxPoint mouse_pos = ::wxGetMousePosition();
//    // check whether mouse is not on any tool item
//    if (this->FindToolByCurrentPosition() != NULL &&
//        this->FindToolByCurrentPosition() != m_title_item) {
//        mouse.Skip();
//        return;
//    }
//#ifdef __W1XMSW__
//    ::PostMessage((HWND) m_frame->GetHandle(), WM_NCLBUTTONDBLCLK, HTCAPTION, MAKELPARAM(mouse_pos.x, mouse_pos.y));
//    return;
//#endif //  __WXMSW__
//
//    if (m_frame->IsMaximized()) {
//        m_frame->Restore();
//    }
//    else {
//        wxDisplay display(this);
//        auto      size = display.GetClientArea().GetSize();
//        m_frame->SetMaxSize(size + wxSize{16, 16});
//        m_normalRect = m_frame->GetRect();
//        m_frame->Maximize();
//    }
//}
//
//void ACToolBar::OnFileToolItem(wxAuiToolBarEvent& evt)
//{
//    wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(evt.GetEventObject());
//
//    tb->SetToolSticky(evt.GetId(), true);
//
//    if (!m_skip_popup_file_menu) {
//        this->PopupMenu(m_file_menu, wxPoint(m_file_menu_item->GetSizerItem()->GetPosition().x, this->GetSize().GetHeight() - 2));
//    }
//    else {
//        m_skip_popup_file_menu = false;
//    }
//
//    // make sure the button is "un-stuck"
//    tb->SetToolSticky(evt.GetId(), false);
//}
//
//void ACToolBar::OnEditToolItem(wxAuiToolBarEvent& evt)
//{
//    wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(evt.GetEventObject());
//
//    tb->SetToolSticky(evt.GetId(), true);
//
//    if (!m_skip_popup_edit_menu) {
//        this->PopupMenu(m_edit_menu, wxPoint(m_edit_menu_item->GetSizerItem()->GetPosition().x, this->GetSize().GetHeight() - 2));
//    }
//    else {
//        m_skip_popup_edit_menu = false;
//    }
//
//    // make sure the button is "un-stuck"
//    tb->SetToolSticky(evt.GetId(), false);
//}
//
//void ACToolBar::OnViewToolItem(wxAuiToolBarEvent& evt)
//{
//    wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(evt.GetEventObject());
//
//    tb->SetToolSticky(evt.GetId(), true);
//
//    if (!m_skip_popup_view_menu) {
//        this->PopupMenu(m_view_menu, wxPoint(m_view_menu_item->GetSizerItem()->GetPosition().x, this->GetSize().GetHeight() - 2));
//    }
//    else {
//        m_skip_popup_view_menu = false;
//    }
//
//    // make sure the button is "un-stuck"
//    tb->SetToolSticky(evt.GetId(), false);
//}
//
//void ACToolBar::OnSetsToolItem(wxAuiToolBarEvent& evt)
//{
//    wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(evt.GetEventObject());
//
//    tb->SetToolSticky(evt.GetId(), true);
//
//    if (!m_skip_popup_sets_menu) {
//
//        this->PopupMenu(m_sets_menu, wxPoint(m_sets_menu_item->GetSizerItem()->GetPosition().x, this->GetSize().GetHeight() - 2));
//    }
//    else {
//        m_skip_popup_sets_menu = false;
//    }
//
//    // make sure the button is "un-stuck"
//    tb->SetToolSticky(evt.GetId(), false);
//}
//
//void ACToolBar::OnHelpToolItem(wxAuiToolBarEvent& evt)
//{
//    wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(evt.GetEventObject());
//
//    tb->SetToolSticky(evt.GetId(), true);
//
//    if (!m_skip_popup_help_menu) {
//        this->PopupMenu(m_help_menu, wxPoint(m_help_menu_item->GetSizerItem()->GetPosition().x, this->GetSize().GetHeight() - 2));
//    }
//    else {
//        m_skip_popup_help_menu = false;
//    }
//
//    // make sure the button is "un-stuck"
//    tb->SetToolSticky(evt.GetId(), false);
//}


//void ACToolBar::OnDropdownToolItem(wxAuiToolBarEvent& evt)
//{
//    wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(evt.GetEventObject());
//
//    tb->SetToolSticky(evt.GetId(), true);
//
//    if (!m_skip_popup_dropdown_menu) {
//        PopupMenu(&m_top_menu, wxPoint(0, this->GetSize().GetHeight() - 2));
//    }
//    else {
//        m_skip_popup_dropdown_menu = false;
//    }
//
//    // make sure the button is "un-stuck"
//    tb->SetToolSticky(evt.GetId(), false);
//}
//
//void ACToolBar::OnMouseLeftDown(wxMouseEvent& event)
//{
//    wxPoint mouse_pos = ::wxGetMousePosition();
//    wxPoint frame_pos = m_frame->GetScreenPosition();
//    m_delta = mouse_pos - frame_pos;
//
//    if (FindToolByCurrentPosition() == NULL 
//        || this->FindToolByCurrentPosition() == m_title_item)
//    {
//        CaptureMouse();
//#ifdef __WXMSW__
//        ReleaseMouse();
//        ::PostMessage((HWND) m_frame->GetHandle(), WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(mouse_pos.x, mouse_pos.y));
//        return;
//#endif //  __WXMSW__
//    }
//    
//    event.Skip();
//}
//
//void ACToolBar::OnMouseLeftUp(wxMouseEvent& event)
//{
//    wxPoint mouse_pos = ::wxGetMousePosition();
//    if (HasCapture())
//    {
//        ReleaseMouse();
//    }
//
//    event.Skip();
//}
//
//void ACToolBar::OnMouseMotion(wxMouseEvent& event)
//{
//    wxPoint mouse_pos = ::wxGetMousePosition();
//
//    if (!HasCapture()) {
//        //m_frame->OnMouseMotion(event);
//
//        event.Skip();
//        return;
//    }
//
//    if (event.Dragging() && event.LeftIsDown())
//    {
//        // leave max state and adjust position 
//        if (m_frame->IsMaximized()) {
//            wxRect rect = m_frame->GetRect();
//            // Filter unexcept mouse move
//            if (m_delta + rect.GetLeftTop() != mouse_pos) {
//                m_delta = mouse_pos - rect.GetLeftTop();
//                m_delta.x = m_delta.x * m_normalRect.width / rect.width;
//                m_delta.y = m_delta.y * m_normalRect.height / rect.height;
//                m_frame->Restore();
//            }
//        }
//        m_frame->Move(mouse_pos - m_delta);
//    }
//    event.Skip();
//}
//
//void ACToolBar::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
//{
//}
//
//void ACToolBar::OnMenuClose(wxMenuEvent& event)
//{
//    wxAuiToolBarItem* item = this->FindToolByCurrentPosition();
//
//    if (item == m_file_menu_item ) m_skip_popup_file_menu = true;
//    if (item == m_edit_menu_item ) m_skip_popup_edit_menu = true;
//    if (item == m_view_menu_item ) m_skip_popup_view_menu = true;
//    if (item == m_sets_menu_item ) m_skip_popup_sets_menu = true;
//    if (item == m_help_menu_item ) m_skip_popup_help_menu = true;
//
//    //else if (item == m_dropdown_menu_item) {
//    //    m_skip_popup_dropdown_menu = true;
//    //}
//}
//
//wxAuiToolBarItem* ACToolBar::FindToolByCurrentPosition()
//{
//    wxPoint mouse_pos = ::wxGetMousePosition();
//    wxPoint client_pos = this->ScreenToClient(mouse_pos);
//    return this->FindToolByPosition(client_pos.x, client_pos.y);
//}