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


#pragma region 2019OKTPrstenUPrstenu
#pragma region code
int CGLRenderer::PrepareTextures(CString strTex) {
    UINT texID;

    //1) DImage
    DImage img;
    img.Load(strTex);

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
void CGLRenderer::DrawRing(float h, float R1, float R2, int n, CString tex) {

#pragma region way1
//#pragma region code
//    float angle = 360 / n;
//
//    glColor3f(0, 0, 0);
//    float diff = R1 / R2;
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, tekstura1);
//    glColor3f(1, 1, 1);
//    glTranslatef(0,-h/2,0);
//    glBegin(GL_QUAD_STRIP);
//        for (int i = 0; i <= n; i++) {
//            glTexCoord2f(0.5 + diff * 0.5 * cos(angle * i * toRad), 0.5 + diff* 0.5 * sin(angle * i * toRad));
//            glVertex3f(R1 * cos(angle * i * toRad), 0, R1 * sin(angle * i * toRad));
//            glTexCoord2f(0.5 + 0.5 * cos(angle * i * toRad), 0.5 + 0.5 * sin(angle * i * toRad));
//            glVertex3f(R2 * cos(angle * i * toRad), 0, R2 * sin(angle * i * toRad));
//
//        }
//    glEnd();
//    glTranslatef(0, h, 0);
//    glBegin(GL_QUAD_STRIP);
//        for (int i = 0; i <= n; i++) {
//            glTexCoord2f(0.5 + diff * 0.5 * cos(angle * i * toRad), 0.5 + diff * 0.5 * sin(angle * i * toRad));
//            glVertex3f(R1 * cos(angle * i * toRad), 0, R1 * sin(angle * i * toRad));
//            glTexCoord2f(0.5 + 0.5 * cos(angle * i * toRad), 0.5 + 0.5 * sin(angle * i * toRad));
//            glVertex3f(R2 * cos(angle * i * toRad), 0, R2 * sin(angle * i * toRad));
//
//        }
//    glEnd();
//
//    glBindTexture(GL_TEXTURE_2D, tekstura2);
//
//    float razlika = 1 / (float)n;
//    glBegin(GL_QUAD_STRIP);
//    for (int i = 0; i <= n; i++) {
//        glTexCoord2f(1 - i* razlika, 0);
//        glVertex3f(R2 * cos(angle * i * toRad), 0, R2 * sin(angle * i * toRad));
//        glTexCoord2f(1 - i * razlika, 1);
//        glVertex3f(R2 * cos(angle * i * toRad), -h, R2 * sin(angle * i * toRad));
//
//    }
//    glEnd();
//
//    glBegin(GL_QUAD_STRIP);
//    for (int i = 0; i <= n; i++) {
//        glTexCoord2f(1 - i * razlika, 0);
//        glVertex3f(R1 * cos(angle * i * toRad), 0, R1 * sin(angle * i * toRad));
//        glTexCoord2f(1 - i * razlika, 1);
//        glVertex3f(R1 * cos(angle * i * toRad), -h, R1 * sin(angle * i * toRad));
//
//    }
//    glEnd();
//    glTranslatef(0, -h / 2, 0);
//    glDisable(GL_TEXTURE_2D);
//
//#pragma endregion
#pragma endregion

#pragma region way2
#pragma region code
    float angle = 360 / n;

    glColor3f(1, 1, 1);
    float diff = R1 / R2;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tekstura1);
    
    // donja stranica
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= n; i++) {
        glTexCoord2f(0.5 + diff * 0.5 * cos(angle * i * toRad), 0.5 + diff * 0.5 * sin(angle * i * toRad));
        glVertex3f(R1 * cos(angle * i * toRad), -h/2, R1 * sin(angle * i * toRad));
        glTexCoord2f(0.5 + 0.5 * cos(angle * i * toRad), 0.5 + 0.5 * sin(angle * i * toRad));
        glVertex3f(R2 * cos(angle * i * toRad), -h/2, R2 * sin(angle * i * toRad));

    }
    glEnd();

    // gornja stranica
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= n; i++) {
        glTexCoord2f(0.5 + diff * 0.5 * cos(angle * i * toRad), 0.5 + diff * 0.5 * sin(angle * i * toRad));
        glVertex3f(R1 * cos(angle * i * toRad), h/2, R1 * sin(angle * i * toRad));
        glTexCoord2f(0.5 + 0.5 * cos(angle * i * toRad), 0.5 + 0.5 * sin(angle * i * toRad));
        glVertex3f(R2 * cos(angle * i * toRad), h/2, R2 * sin(angle * i * toRad));

    }
    glEnd();

    glBindTexture(GL_TEXTURE_2D, tekstura2);

    // spoljanji omotac
    float razlika = 1 / (float)n;
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= n; i++) {
        glTexCoord2f(1 - i * razlika, 0);
        glVertex3f(R2 * cos(angle * i * toRad), -h/2, R2 * sin(angle * i * toRad));
        glTexCoord2f(1 - i * razlika, 1);
        glVertex3f(R2 * cos(angle * i * toRad), h/2, R2 * sin(angle * i * toRad));

    }
    glEnd();

    // unutrasnji omotac
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= n; i++) {
        glTexCoord2f(1 - i * razlika, 0);
        glVertex3f(R1 * cos(angle * i * toRad), -h/2, R1 * sin(angle * i * toRad));
        glTexCoord2f(1 - i * razlika, 1);
        glVertex3f(R1 * cos(angle * i * toRad), h/2, R1 * sin(angle * i * toRad));

    }
    glEnd();
    glDisable(GL_TEXTURE_2D);

#pragma endregion
#pragma endregion
}
void CGLRenderer::DrawRings(float h, float R1, float R2, int n, CString tex) {
    
#pragma region Kimi
//#pragma region code
//    glRotatef(-90, 1, 0, 0);
//    DrawRing(1, 2, 3, 10, CString("aaaa"));
//    glRotatef(90, 1, 0, 0);
//    glTranslatef((R1 + R2) / 2, 0, 0);
//    DrawRing(1, 2, 3, 10, CString("aaaa"));
//#pragma endregion
#pragma endregion

#pragma region Darjan
#pragma region code
    DrawRing(1, 2, 3, 10, CString("aaaa"));
    glTranslatef(-(R1 + R2) / 2, 0, 0);
    glRotatef(-90, 1, 0, 0);
    DrawRing(1, 2, 3, 10, CString("aaaa"));
    glRotatef(90, 1, 0, 0);
#pragma endregion
#pragma endregion
}

void CGLRenderer::Draw() {
    DrawRings(1, 2, 3, 10, CString("aaaa"));

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

#pragma region 2019OKTPrstenUPrstenu
    tekstura1 = PrepareTextures(CString("./Teksture//download.jpg"));
    tekstura2 = PrepareTextures(CString("./Teksture//download.jpg"));
#pragma endregion 

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
