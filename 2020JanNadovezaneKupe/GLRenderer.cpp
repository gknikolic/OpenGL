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

    glColor3f(0, 0, 0);

    glEnd();
}

#pragma region 2020JanNadovezaneKupe
#pragma region code
int CGLRenderer::PrepareTextures(CString strTex) {
    UINT texID;

    DImage img;
    img.Load(strTex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

    return texID;

}
void CGLRenderer::DrawCone(float h, float r, int nSeg) {
    float angle = 360. / nSeg;
    glDisable(GL_TEXTURE_2D);
    glColor3f(1, 0.5, 0);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, -1, 0);
    glVertex3f(0, 0, 0);
    for (int i = 0; i <= nSeg; i++) {
        glVertex3f(r * cos(angle * i * toRad), 0, r * sin(angle * i * toRad));
    }
    glEnd();

    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, woodTexture);

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.5, 0.5);
    glVertex3f(0, h, 0);
    for (int i = 0; i <= nSeg; i++) {
        glNormal3f(cos(i * angle * toRad), 0, sin(i * angle * toRad));
        glTexCoord2f(0.5 + 0.5 * cos(i * angle * toRad), 0.5 + 0.5 * sin(i * angle * toRad));
        glVertex3f(r * cos(i * angle * toRad), 0, r * sin(i * angle * toRad));
    }
    glEnd();
}
void CGLRenderer::DrawFigure(float h, float r, int nSeg, int n) {
    DrawCone(2 * h, 2 * r, nSeg);
    glPushMatrix();
    float alpha = atan(r / h) * toDeg;
    float angle = 360 / n;
    glTranslatef(0, 2 * h, 0);

    glRotatef(90 - alpha, 0, 0, 1);

    for (int i = 0; i < n; i++) {
        DrawCone(h, r, nSeg);
        glTranslatef(0, h, 0);
        glRotatef(-angle, 0, 0, 1);
    }

    glPopMatrix();
}
void CGLRenderer::Draw() {
    glScalef(2, 2, 2);
    DrawFigure(1, 0.5, 6, 8);

}
#pragma endregion
#pragma endregion

void CGLRenderer::DrawScene(CDC* pDC)
{
#pragma region Begin
    wglMakeCurrent(pDC->m_hDC, m_hrc);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //------------------------------------------
#pragma endregion
#pragma region Camera 
    //ne pomera se posmatrac, vec koordinatni sistem u kome se iscrtava! Posmatrac (kamera) je sve vreme u centru koordinatnog pocetka
    //ako se koristi gluLookAt, ponovo se ne pomera kamera vec ceo koordniatni sistem u odnosu na kameru, a efekat je kao da se kamera pomerila, pogledaj: https://www.youtube.com/watch?v=ODKfUez08zE&ab_channel=JamieKing

    glTranslatef(-0.01, -0.1, -1); //pomeramo lokalni koordinatni sistem u kome se iscrtavaju objekti u negativnom smeru z ose, kako bi objekti bili isped kamere (posmatraca)
    DrawAxes(1);
    glTranslatef(0.01, 0.1, 1); //vracamo lokalni koordinatni sistem u mesto gde se nalazi kamera

    glTranslatef(0, 0, zoom);
    glRotatef(rotacijaX, 1, 0, 0);
    glRotatef(rotacijaY, 0, 1, 0);
    glRotatef(rotacijaZ, 0, 0, 1);
    DrawAxes(10);
#pragma endregion
    //------------------------------------------


    Draw();


    //------------------------------------------
#pragma region End
    glFlush();
    SwapBuffers(pDC->m_hDC);
    wglMakeCurrent(NULL, NULL);
#pragma endregion
}

void CGLRenderer::DestroyScene(CDC* pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);

    //------------------------------------------


    //BRISANJE UCITANE TEKSTURE
    //glDeleteTextures(1, &top);
    glDeleteTextures(1, &woodTexture);

    //------------------------------------------

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
    woodTexture = PrepareTextures(CString("Teksture/images.jpg"));

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
