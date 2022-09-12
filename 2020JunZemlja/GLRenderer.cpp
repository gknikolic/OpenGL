#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
#include <cmath>
//#pragma comment(lib, "GL\\glut32.lib")

void CGLRenderer::DrawAxes(double len)
{
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(len, 0.0, 0.0);

    glColor3f(0, 1, 0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, len, 0.0);

    glColor3f(0, 0, 1);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, len);

    glColor3f(1, 1, 1);

    glEnd();
}

#pragma region 2020JUNZemlja
#pragma region code
int CGLRenderer::PrepareTextures(char* fileName) {
    UINT texID;

    //1) DImage
    DImage img;
    img.Load(CString(fileName));

    //2) pixel store
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    //3) gen
    glGenTextures(1, &texID);

    //4) bind 2d
    glBindTexture(GL_TEXTURE_2D, texID);

    //5) Param x 4
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //6) env mode modulate
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    //7) build
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

    return texID;
}

void CGLRenderer::Draw() {

}

#pragma endregion
#pragma endregion



void CGLRenderer::DrawScene(CDC* pDC)
{
#pragma region Begin
    wglMakeCurrent(pDC->m_hDC, m_hrc);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glPushMatrix();
    //------------------------------------------
#pragma endregion
#pragma region Camera 
    //ne pomera se posmatrac, vec koordinatni sistem u kome se iscrtava!
    glTranslatef(0, 0, zoom);
    glRotatef(rotacijaX, 1, 0, 0);
    glRotatef(rotacijaY, 0, 1, 0);
    glRotatef(rotacijaZ, 0, 0, 1);
    //gluLookAt(0, 0, 20, 0.0, 0.0, 0.0, 0, 1, 0); // probaj prvo ovo, mora bar 1 vektor da bude up
    //gluLookAt(2, 2, 2, 0.0, 0.0, 0.0, 0, 0, 1);
  
#pragma endregion
    DrawAxes(10);
    //------------------------------------------


    Draw();


    //------------------------------------------
#pragma region End
    glPopMatrix();
    glFlush();
    SwapBuffers(pDC->m_hDC);
    wglMakeCurrent(NULL, NULL);
#pragma endregion
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);

#pragma region 2019 KOL2 BAGER
    //------------------------------------------


    //BRISANJE UCITANE TEKSTURE


    //------------------------------------------
#pragma endregion

    wglMakeCurrent(NULL, NULL);
    if (m_hrc)
    {
        wglDeleteContext(m_hrc);
        m_hrc = NULL;
    }
}

void CGLRenderer::PrepareScene(CDC* pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);
    //---------------------------------

    //1) color
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //2)depth test
    glEnable(GL_DEPTH_TEST);
    //------------------------------------------


    //UCITAVANJE TEKSTURE
    tekstura1 = PrepareTextures("download.jpg");
    tekstura2 = PrepareTextures("images.jpg");

    //------------------------------------------
    wglMakeCurrent(NULL, NULL);
}

#pragma region PROJECT FUNCTIONS
CGLRenderer::CGLRenderer(void)
{
}
CGLRenderer::~CGLRenderer(void)
{
}
bool CGLRenderer::CreateGLContext(CDC* pDC)
{
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);

    if (nPixelFormat == 0) return false;

    BOOL bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);

    if (!bResult) return false;

    m_hrc = wglCreateContext(pDC->m_hDC);

    if (!m_hrc) return false;

    return true;
}
void CGLRenderer::Reshape(CDC* pDC, int w, int h)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);
    //---------------------------------

    //1)
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    //2)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //3)
    gluPerspective(50, (double)w / (double)h, 0.1, 2000);

    //4)
    glMatrixMode(GL_MODELVIEW);

    //---------------------------------
    wglMakeCurrent(NULL, NULL);
}
#pragma endregion
