/////////////////////////////////////////////////////////////////////////////
// Name:        main.h
// Purpose:     gamma-view main
// Author:      Mateusz Plociennik
// Created:     04/01/98
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MAIN_H_
#define _WX_MAIN_H_

#include "glview.h"

/*
#include "block_base.h"
#include "block_data.h"
#include "block_usb.h"
#include "block_trans.h"
#include "block_fsave.h"
*/

// Define a new application type
class MyApp : public wxApp
{
public:
    MyApp() { m_glContext = NULL; }

    // Returns the shared context used by all frames and sets it as current for
    // the given canvas.
    TestGLContext& GetContext(wxGLCanvas *canvas);

    // virtual wxApp methods
    virtual bool OnInit();
    virtual int OnExit();

private:
    // the GL context we use for all our windows
    TestGLContext *m_glContext;
};

// Define a new frame type
class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnClose(wxCommandEvent& event);
	void OnHelpAbout(wxCommandEvent& event);
    void OnNewWindow(wxCommandEvent& event);
	void OnResizeWindow(wxCommandEvent& event);


    DECLARE_EVENT_TABLE()
};



#endif // _WX_MAIN_H_
