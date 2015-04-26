/***************************************************************************
 * $Id: IACFleet.cpp, v1.0 2010/08/05 SethDart Exp $
 *
 * Project:  OpenCPN
 * Purpose:  IACFleet Plugin
 * Author:   Carsten Borchardt
 *
 ***************************************************************************
 *   Copyright (C) 2010 by Carsten Borchardt                               *
 *   Carsten@sy-fortytwo.de                                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif //precompiled headers

#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/mstream.h>
#include <wx/datetime.h>
#include <typeinfo>

#include "iacfleet_pi.h"
#include "iacfleet.h"
#include "iacfile.h"
IMPLEMENT_CLASS ( IACFleetUIDialog, wxDialog )

BEGIN_EVENT_TABLE ( IACFleetUIDialog, wxDialog )

    EVT_CLOSE ( IACFleetUIDialog::OnClose )
    EVT_BUTTON ( ID_OK, IACFleetUIDialog::OnIdOKClick )
    EVT_MOVE ( IACFleetUIDialog::OnMove )
    EVT_SIZE ( IACFleetUIDialog::OnSize )
    EVT_BUTTON ( ID_CHOOSEIACFLEETDIR, IACFleetUIDialog::OnChooseDirClick )
    EVT_LISTBOX ( ID_FILESELECTED,IACFleetUIDialog::OnFileSelect)
    EVT_TEXT(ID_RAWTEXT, IACFleetUIDialog::OnRawTextChanged)
    EVT_TIMER(ID_TIP_TIMER, IACFleetUIDialog::OnTipTimer)
END_EVENT_TABLE()

IACFleetUIDialog::IACFleetUIDialog(void):
    pParent(NULL),
    pPlugIn(NULL),
    m_pfolder_bitmap(NULL),
    m_lastViewPortValid(false),
    m_pitemCurrentDirectoryCtrl(NULL),
    m_pFileListCtrl(NULL),    
    m_pTextCtrl(NULL),
    m_pRawCtrl(NULL),
    m_pFileTime(NULL),
    m_pTipWindow(NULL)
{
    //      Init(); privat member of "wxDialog" in dialog.h
}

IACFleetUIDialog::~IACFleetUIDialog( void )
{
    m_bBrDownload->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IACFleetUIDialog::OnBrDownload ), NULL, this );
}

bool IACFleetUIDialog::Create ( wxWindow *parent, iacfleet_pi *ppi, wxWindowID id,
        const wxString& caption, const wxString initial_dir,
        const wxPoint& pos, const wxSize& size, long style )
{
    pParent = parent;
    pPlugIn = ppi;

    m_currentDir = initial_dir;
    long wstyle = wxDEFAULT_FRAME_STYLE;

    if( !wxDialog::Create( parent, id, caption, pos, size, wstyle ) )
        return false;
#include "folder.xpm"
    m_pfolder_bitmap = new wxBitmap ( folder );   // comes from XPM include

    CreateControls();
    SetMinSize(GetBestSize());
    return true;
}


// get called if the mouse is moved on the chart
void IACFleetUIDialog::SetCursorLatLon( double lat, double lon )
{
    m_cursorpos.x=lon;
    m_cursorpos.y=lat;
    // Display Tip Window with details of system near the cursor.
    // Do this by starting and restarting a timer.
    // If no movement of the cursor occures, the
    // timer triggers and a method is called
    // that can find and display information
    // about the object at the cursor position

    // start / restart the timer
    m_TooltipTimer.SetOwner(this, ID_TIP_TIMER);
    m_TooltipTimer.Start(200, true);
}

//
void IACFleetUIDialog::OnTipTimer( wxTimerEvent& event )
{
    wxString tip;
    // try to display the tooltip only if
    // - our vioewpoert is valid
    // - no "old" tooltip is displayed
    if( m_lastViewPortValid && NULL == m_pTipWindow )
    {
        IACSystem *pSystem;
        int sel_rad_pix = 8;
        double view_scale_ppm = m_lastViewPort.view_scale_ppm;
        double SelectRadius = sel_rad_pix/ ( view_scale_ppm * 1852 * 60 );  // Degrees, approximately
        if( NULL != (pSystem = m_iacfile.FindSystem(m_cursorpos,SelectRadius)) )
        {
            // found
            tip = pSystem->ToString(false); // get Detail-String of system near cursor
            m_pTipWindow = new wxTipWindow(m_parent,tip,150);
            if( NULL != m_pTipWindow )
            {
                // hurray - we have a tip window
                // define area beyond the tip gets deleted
                wxPoint pos;
                GetCanvasPixLL(&m_lastViewPort, &pos, m_cursorpos.y, m_cursorpos.x);
                wxRect bRect(pos.x - sel_rad_pix,
                        pos.y - sel_rad_pix,
                        pos.x + sel_rad_pix,
                        pos.y + sel_rad_pix);
                m_pTipWindow->SetBoundingRect(bRect);
                // tell tip window to NULL this pointer when it deletes itself
                // this means we can tell if the tip window is active or not
                m_pTipWindow->SetTipWindowPtr(&m_pTipWindow);
            }
        }
    }
}

void IACFleetUIDialog::OnClose ( wxCloseEvent& event )
{
    pPlugIn->SetDir(m_currentDir);
    RequestRefresh(pParent);
    Destroy();
    pPlugIn->OnDialogClose();
}

void IACFleetUIDialog::Invalidate( void )
{
    m_iacfile.Invalidate();
    m_pRawCtrl->Clear();
    m_pTextCtrl->Clear();
}

void IACFleetUIDialog::OnIdOKClick( wxCommandEvent& event )
{
    Close(); // this will call OnClose() later...
}

void IACFleetUIDialog::OnMove( wxMoveEvent& event )
{
    //    Record the dialog position
    wxPoint p = event.GetPosition();
    pPlugIn->SetDialogX(p.x);
    pPlugIn->SetDialogY(p.y);

    event.Skip();
}

void IACFleetUIDialog::OnSize( wxSizeEvent& event )
{
    wxSize p = event.GetSize();
    pPlugIn->SetDialogSizeX(p.x);
    pPlugIn->SetDialogSizeY(p.y);

    event.Skip();
}

void IACFleetUIDialog::OnChooseDirClick( wxCommandEvent& event )
{
    wxString new_dir  = ::wxDirSelector( _( "Select IACFleet Directory" ), m_currentDir );
    if( !new_dir.empty() )
    {
        m_currentDir = new_dir;
        m_pitemCurrentDirectoryCtrl->ChangeValue( m_currentDir );
        m_pitemCurrentDirectoryCtrl->SetInsertionPoint(0);
        updateFileList();

        Refresh();

        pPlugIn->SetDir( m_currentDir );

    }
}

void IACFleetUIDialog::CreateControls()
{

// A top-level sizer
    wxBoxSizer* topSizer = new wxBoxSizer( wxVERTICAL );
    SetSizer( topSizer );


//    The Fleetode directory
    wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox( this, wxID_ANY,_ ( "IACFleet File Directory" ) );
    wxStaticBoxSizer *itemStaticBoxSizer11 = new wxStaticBoxSizer ( itemStaticBoxSizer11Static, wxHORIZONTAL );
    topSizer->Add ( itemStaticBoxSizer11, 0, wxEXPAND );

    m_pitemCurrentDirectoryCtrl = new wxTextCtrl( this, -1, _T(""), wxDefaultPosition, wxSize(-1, -1), wxTE_READONLY );
    itemStaticBoxSizer11->Add( m_pitemCurrentDirectoryCtrl, 1, wxALIGN_LEFT);

    m_pitemCurrentDirectoryCtrl->AppendText( m_currentDir );

    wxButton* bChooseDir = new wxBitmapButton( this, ID_CHOOSEIACFLEETDIR, *m_pfolder_bitmap );
    itemStaticBoxSizer11->Add( bChooseDir, 0, wxALIGN_RIGHT|wxALL, 5 );

    // panels
    wxNotebook *itemNotebook = new wxNotebook( this, ID_NOTEBOOK, wxDefaultPosition,
            wxDefaultSize, wxNB_TOP );
    topSizer->Add(itemNotebook,1,wxGROW);
    itemNotebook->SetMinSize(wxSize(wxDefaultCoord,0));
    // file panel
    wxPanel *filepanel = new wxPanel( itemNotebook, wxID_ANY,
            wxDefaultPosition, wxDefaultSize);
    itemNotebook->AddPage(filepanel, _("Files"));

    wxBoxSizer* fpsizer = new wxBoxSizer(wxVERTICAL);
    filepanel->SetSizer(fpsizer);
    m_pFileListCtrl = new wxListBox(filepanel,ID_FILESELECTED,
            wxDefaultPosition,wxDefaultSize,0,
            NULL,
            wxLB_SINGLE|wxLB_HSCROLL|wxLB_NEEDED_SB,
            wxDefaultValidator,wxListBoxNameStr);

    fpsizer->Add(m_pFileListCtrl, 1, wxGROW);

    // Text panel
    wxPanel *textpanel = new wxPanel( itemNotebook, wxID_ANY,
            wxDefaultPosition, wxSize(-1, -1));
    itemNotebook->AddPage(textpanel, _("Text"));

    wxBoxSizer* tpsizer = new wxBoxSizer(wxVERTICAL);
    textpanel->SetSizer(tpsizer);

    m_pTextCtrl = new wxTextCtrl( textpanel,
            wxID_ANY, wxEmptyString,
            wxDefaultPosition,wxDefaultSize,
            wxTE_MULTILINE|wxTE_READONLY|wxHSCROLL
                                );
    tpsizer->Add(m_pTextCtrl, 1, wxGROW);

    // Raw panel
    wxPanel *rawpanel = new wxPanel( itemNotebook, wxID_ANY,
            wxDefaultPosition, wxSize(-1, -1));
    itemNotebook->AddPage(rawpanel, _("Raw"));

    wxBoxSizer* rpsizer = new wxBoxSizer(wxVERTICAL);
    rawpanel->SetSizer(rpsizer);

    m_pRawCtrl = new wxTextCtrl( rawpanel,
            ID_RAWTEXT, wxEmptyString,
            wxDefaultPosition,wxDefaultSize,
            wxTE_MULTILINE|wxHSCROLL
                               );
    rpsizer->Add(m_pRawCtrl, 1, wxGROW);
    
    // Downloads panel
    wxPanel *dnldpanel = new wxPanel( itemNotebook, wxID_ANY,
            wxDefaultPosition, wxSize(-1, -1));
    itemNotebook->AddPage(dnldpanel, _("Downloads"));
    //Generated mostly from wxFormBuilder
    wxBoxSizer* bSizerMain;
	bSizerMain = new wxBoxSizer( wxVERTICAL );
	dnldpanel->SetSizer(bSizerMain);
	
	wxStaticBoxSizer* sbSizerBrazil;
	sbSizerBrazil = new wxStaticBoxSizer( new wxStaticBox( dnldpanel, wxID_ANY, _("Brazil") ), wxVERTICAL );
	
	wxBoxSizer* bsBRazil;
	bsBRazil = new wxBoxSizer( wxHORIZONTAL );
	
	m_stDate = new wxStaticText( dnldpanel, wxID_ANY, _("Date"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stDate->Wrap( -1 );
	bsBRazil->Add( m_stDate, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_dpBrazil = new wxDatePickerCtrl( dnldpanel, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	bsBRazil->Add( m_dpBrazil, 0, wxALL, 5 );
	m_dpBrazil->SetValue(wxDateTime::Today());
	
	m_stHour = new wxStaticText( dnldpanel, wxID_ANY, _("Hour"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stHour->Wrap( -1 );
	bsBRazil->Add( m_stHour, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxString m_chHourChoices[] = { _("00"), _("12") };
	int m_chHourNChoices = sizeof( m_chHourChoices ) / sizeof( wxString );
	m_chHour = new wxChoice( dnldpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_chHourNChoices, m_chHourChoices, 0 );
	m_chHour->SetSelection( 0 );
	bsBRazil->Add( m_chHour, 0, wxALL, 5 );
	
	
	bsBRazil->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_bBrDownload = new wxButton( dnldpanel, wxID_ANY, _("Download"), wxDefaultPosition, wxDefaultSize, 0 );
	bsBRazil->Add( m_bBrDownload, 0, wxALL, 5 );
	
	
	sbSizerBrazil->Add( bsBRazil, 0, wxEXPAND, 5 );
	
	m_stBrDesc = new wxStaticText( dnldpanel, wxID_ANY, _("The data is available since 2006 and accesible manually from"), wxDefaultPosition, wxDefaultSize, 0 );
	m_stBrDesc->Wrap( -1 );
	sbSizerBrazil->Add( m_stBrDesc, 0, wxALL, 5 );
	
	m_hlBr = new wxHyperlinkCtrl( dnldpanel, wxID_ANY, _("http://www.mar.mil.br/dhn/chm/meteo/prev/iac/iac.htm"), wxT("http://www.mar.mil.br/dhn/chm/meteo/prev/iac/iac.htm"), wxDefaultPosition, wxDefaultSize, wxHL_DEFAULT_STYLE );
	sbSizerBrazil->Add( m_hlBr, 0, wxALL, 5 );
	
	
	bSizerMain->Add( sbSizerBrazil, 0, wxALL|wxEXPAND, 5 );
	
	// Connect Events
	m_bBrDownload->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( IACFleetUIDialog::OnBrDownload ), NULL, this );

    // the text control that shows the Date and Time
    // of the Fleetcode
    wxBoxSizer* tmsizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText *pIssueDate = new wxStaticText( this,
            wxID_ANY, _("IssueDate:"),
            wxDefaultPosition,wxDefaultSize,
            wxALIGN_LEFT
                                               );
    tmsizer->Add(pIssueDate,0);
    m_pFileTime = new wxStaticText( this,
            wxID_ANY, wxEmptyString,
            wxDefaultPosition,wxDefaultSize,
            wxALIGN_LEFT);
    tmsizer->Add(m_pFileTime,1,wxGROW);
    topSizer->Add(tmsizer,0,wxGROW);

    // A horizontal box sizer to contain OK
    wxBoxSizer* AckBox = new wxBoxSizer ( wxHORIZONTAL );
    topSizer->Add( AckBox, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );

    // The OK button
    wxButton* bOK = new wxButton( this, ID_OK, _( "&Close" ),
            wxDefaultPosition, wxDefaultSize, 0 );
    AckBox->Add( bOK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );

    updateFileList();
}

//---------------------------------------------------------
void IACFleetUIDialog::updateFileList( void )
{
    m_FilenameArray.Empty();
    wxDir::GetAllFiles(m_currentDir,
            &m_FilenameArray,
            wxEmptyString,
            wxDIR_FILES);
    for( int i = m_FilenameArray.GetCount() - 1; i >= 0; i-- )
    {
        // remove dirname from file
        wxFileName file(m_FilenameArray[i]);
        wxDateTime access;
        wxDateTime modified;
        wxDateTime created;
        file.GetTimes(&access,&modified,&created);
        wxString dates=modified.FormatISODate() + modified.FormatISOTime() + wxT(";");
        m_FilenameArray[i]=dates+file.GetFullName();
    }
    // sort the filenames according to dates
    m_FilenameArray.Sort(true);
    for( int i = m_FilenameArray.GetCount() - 1; i >= 0; i-- )
    {
        // throw away time info
        // Yes i know this is bad code!
        wxString str = m_FilenameArray[i].AfterFirst(';');
        m_FilenameArray[i] = str;
    }
    m_pFileListCtrl->Set(m_FilenameArray);
    m_currentFileName = wxEmptyString;
}

// a new file is selected, read and display this file
void IACFleetUIDialog::OnFileSelect( wxCommandEvent& event )
{
    wxArrayInt selectedFiles;
    int numberOfSelectedFiles = 0;
    numberOfSelectedFiles = m_pFileListCtrl->GetSelections(selectedFiles);
    if( numberOfSelectedFiles > 0 )
    {
        // use the one and only selected file
        int index = selectedFiles[0];
        wxFileName fn(m_currentDir, m_FilenameArray[index]);
        m_currentFileName = fn.GetFullPath();
    }
    else
    {
        m_currentFileName = wxEmptyString;
    }
    // read File and redisplay it
    updateIACFleet();
}

void IACFleetUIDialog::OnRawTextChanged( wxCommandEvent& event )
{
    if( m_pRawCtrl->IsModified() )
    {
        ::wxBeginBusyCursor();

        // the user has modified the text
        // use the modified text as input

        // first remove selection from filelist
        wxArrayInt selectedFiles;
        int numberOfSelectedFiles;

        numberOfSelectedFiles=m_pFileListCtrl->GetSelections(selectedFiles);
        if( numberOfSelectedFiles > 0 )
        {
            m_pFileListCtrl->Deselect(selectedFiles[0]);
        }
        // make an inputstream from the Raw data in the control
        wxString rawData = m_pRawCtrl->GetValue();
        wxMemoryOutputStream ostream;
        // is there an easier way than to loop throughall the chars in
        // the Control?
        for( size_t Index = 0; Index < rawData.Length(); Index++ )
        {
            int c = rawData[Index];
            ostream.PutC(c);
        }
        wxMemoryInputStream istream(ostream);
        if( m_iacfile.Read(istream) )
        {
            updateTextPanel();
        }
        RequestRefresh(pParent);
        ::wxEndBusyCursor();
    }
}

// if rawdata is not empty use it as input to the
// IACFleet code
// otherwise use m_currentFileName
void IACFleetUIDialog::updateIACFleet( void )
{
    bool ok = false;
    ::wxBeginBusyCursor();

    wxFileInputStream *pStream = m_iacfile.GetStream(m_currentFileName);
    if( NULL != pStream && pStream->IsOk() )
    {
        if( m_iacfile.Read(*pStream) )
        {
            updateTextPanel();
            updateRawPanel(m_iacfile.GetRawData());
            ok = true;
        }
    }
    if( !ok )
    {
        Invalidate();
    }
    RequestRefresh(pParent);
    delete(pStream);
    ::wxEndBusyCursor();
}

void IACFleetUIDialog::updateRawPanel( wxString &rawData )
{
    m_pRawCtrl->ChangeValue(rawData);
}

void IACFleetUIDialog::updateTextPanel( void )
{
    m_pTextCtrl->ChangeValue(m_iacfile.ToString());
    m_pFileTime->SetLabel(m_iacfile.GetIssueDate());
}

bool IACFleetUIDialog::RenderOverlay( wxDC *dc, PlugIn_ViewPort *vp )
{
    m_lastViewPortValid = true;
    m_lastViewPort = *vp;
    return m_iacfile.Draw(dc,vp);
}

void IACFleetUIDialog::OnBrDownload( wxCommandEvent& event )
{
    wxDateTime dt = m_dpBrazil->GetValue();
    if( dt > wxDateTime::Now() )
    {
        wxMessageBox(_("Sorry, you can't download data from the future - they don't exist..."));
        return;
    }
    else if( dt < wxDateTime( 1, 9, 2006 ) )
    {
        wxMessageBox(_("Sorry, the data is not available before September 1, 2006..."));
        return;
    }
    wxString filename = wxString::Format(_T("P%u%02u%02u%s.iac"), 
                                   dt.GetYear() - 2000, dt.GetMonth() + 1, dt.GetDay(), 
                                   m_chHour->GetString(m_chHour->GetSelection()).c_str() );
    wxString url = _T("http://www.mar.mil.br/dhn/chm/meteo/prev/iac/") + filename;
    
    wxFileName tfn = wxFileName::CreateTempFileName( _T("iacfleet") );
    wxFileName fn(m_currentDir, filename);
    wxFileOutputStream output( tfn.GetFullPath() );
    wxCurlDownloadDialog ddlg(url, &output, _("Downloading file"),
            _("Reading Headers: ") + url, wxNullBitmap, this,
            wxCTDS_ELAPSED_TIME|wxCTDS_ESTIMATED_TIME|wxCTDS_REMAINING_TIME|wxCTDS_SPEED|wxCTDS_SIZE|wxCTDS_URL|wxCTDS_CAN_PAUSE|wxCTDS_CAN_ABORT|wxCTDS_AUTO_CLOSE);
    ddlg.SetSize(this->GetSize().GetWidth(), ddlg.GetSize().GetHeight());
    wxCurlDialogReturnFlag ret = ddlg.RunModal();
    output.Close();
    switch( ret )
    {
        case wxCDRF_SUCCESS:
        {
            if ( wxCopyFile( tfn.GetFullPath(), fn.GetFullPath() ) )
            {
                updateFileList();
                m_pFileListCtrl->SetStringSelection(filename);
                m_currentFileName = fn.GetFullPath();
                updateIACFleet();
            }
            else
                wxMessageBox(wxString::Format( _("Failed to save: %s "), fn.GetFullPath().c_str() ),
                        _T("IACFleet"), wxOK | wxICON_ERROR);
            break;
        }
        case wxCDRF_FAILED:
        {
            wxMessageBox(wxString::Format( _("Failed to download: %s \nVerify there is a working Internet connection."), url.c_str() ),
                    _T("IACFleet"), wxOK | wxICON_ERROR);
            break;
        }
        case wxCDRF_USER_ABORTED:
        {
            break;
        }
        default:
            wxASSERT( false );  // This should never happen because we handle all possible cases of ret
    }
    wxRemoveFile ( tfn.GetFullPath() );
}
