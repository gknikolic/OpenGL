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

#pragma region imeBlanketa
#pragma region code
UINT CGLRenderer::LoadTexture(char* fileName)
{
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
UINT CGLRenderer::PrepareTextures(CString strText) {
    UINT texID;

    DImage img;
    img.Load(strText);

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
void CGLRenderer::DrawRoller(float r, float h, int nPartR, int nPartH) {

    float baseAngle = 360 / nPartR;

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.5, 0.5);
    glVertex3f(0, 0, 0);
    for (int i = 0; i <= nPartR; i++) {
        glTexCoord2f(0.5 + 0.5 * cos(i * baseAngle * toRad), 0.5 + 0.5 * sin(i * baseAngle * toRad));
        glVertex3f(r * cos(i * baseAngle * toRad), 0, r * sin(i * baseAngle * toRad));
    }
    glEnd();

    float hPart = h / nPartH;
    float texHPart = 1. / nPartH;
    float texRPart = 1. / nPartR;

    for (int j = 0; j < nPartH; j++) {
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= nPartR; i++) {
            glNormal3f(cos(i * baseAngle * toRad), 0, sin(i * baseAngle * toRad));
            glTexCoord2f(1 - texRPart * i, 1 - texHPart * j);
            glVertex3f(r * cos(i * baseAngle * toRad), j * hPart, r * sin(i * baseAngle * toRad));
            glTexCoord2f(1 - texRPart * i, 1 - texHPart * (j + 1));
            glVertex3f(r * cos(i * baseAngle * toRad), (j + 1) * hPart, r * sin(i * baseAngle * toRad));
        }
        glEnd();
    }

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, -1, 0);
    glTexCoord2f(0.5, 0.5);
    glVertex3f(0, h, 0);
    for (int i = 0; i <= nPartR; i++) {
        glTexCoord2f(0.5 + 0.5 * cos(i * baseAngle * toRad), 0.5 + 0.5 * sin(i * baseAngle * toRad));
        glVertex3f(r * cos(i * baseAngle * toRad), h, r * sin(i * baseAngle * toRad));
    }
    glEnd();
}
void CGLRenderer::DrawTwoRollers(float r, float h, float p, int nPartR, int nPartH) {
    float h1 = p * h / (1 + p);
    float h2 = h - h1;
    DrawRoller(r, h1, nPartR, nPartH);
    glTranslatef(0, h1, 0);
    DrawRoller(r, h2, nPartR, nPartH);
    glTranslatef(0, h2, 0);
}
void CGLRenderer::DrawFigure(float r, float d, float hp, float h1, float p1, float h2, float p2, float h3, float p3,
    float alpha, float beta, float gama, CString strTex) {

    float rZgloba = 1.5 * r;

    //POSTOLJE
    DrawRoller(4 * r, hp, 20, 50);
    glTranslatef(0, hp, 0);

    //CEV
    glRotatef(alpha + alphaRot, 0, 1, 0); //alphaRot je dodat da bi se omogucilo pomeranje na pritisak tastera
    DrawTwoRollers(r, h1, p1, 20, 50);

    //ZGLOB
    DrawRoller(rZgloba, d, 20, 50);
    glTranslatef(0, d / 2, 0);
    glRotatef(beta + betaRot, 0, 0, 1);
    glTranslatef(0, d / 2, 0);

    //CEV
    DrawTwoRollers(r, h2, p2, 20, 50);

    //ZGLOB
    DrawRoller(rZgloba, d, 20, 50);
    glTranslatef(0, d / 2, 0);
    glRotatef(gama + gamaRot, 0, 0, 1);
    glTranslatef(0, d / 2, 0);

    //CEV
    DrawTwoRollers(r, h2, p2, 20, 50);
}
void CGLRenderer::Draw() {
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    glColor3f(1, 1, 1);
    //DrawRoller(1, 3, 6, 4);
    DrawFigure(0.5, 1, 1.5, 2, 1, 3, 2, 2, 2, 30, -60, -60, CString("a?")); //CString("a?") - tekstura je ucitana u prepareScene zbog performansi
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

void CGLRenderer::Reshape(CDC* pDC, int w, int h) {
    wglMakeCurrent(pDC->m_hDC, m_hrc);

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40, (double)w / (double)h, 0.1, 2000);

    wglMakeCurrent(NULL, NULL);
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
    //woodTexture = LoadTexture("Teksture/download.jpg");
    woodTexture = LoadTexture("Teksture/images.jpg");
    glEnable(GL_TEXTURE_2D);

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
