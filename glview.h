/////////////////////////////////////////////////////////////////////////////
// Name:        glview.h
// Purpose:     GL View
// Author:      Mateusz Plociennik
// Modified by:
// Created:     16/06/2012
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GLVIEW_H_
#define _WX_GLVIEW_H_

#include "wx\glcanvas.h"
#include "wx\thread.h"

class GLThread :
	wxThread
{
public:
	GLThread(void);
	~GLThread(void);
};

// the rendering context used by all GL canvases
class TestGLContext : public wxGLContext
{
public:
    TestGLContext(wxGLCanvas *canvas);

    // render the cube showing it at given angles
    void Draw();

private:
    // textures for the cube faces
    GLuint m_textures[6];
	wxImage m_images[6];
};

class TestGLCanvas : public wxGLCanvas
{
public:
    TestGLCanvas(wxWindow *parent);

private:
    void OnPaint(wxPaintEvent& event);
    void Spin(float xSpin, float ySpin);
    void OnKeyDown(wxKeyEvent& event);
    void OnSpinTimer(wxTimerEvent& WXUNUSED(event));

    // angles of rotation around x- and y- axis
    float m_xangle,
          m_yangle;
	int m_time;

    wxTimer m_spinTimer;

    DECLARE_EVENT_TABLE()
};

#endif // _WX_GLVIEW_H_
