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

#pragma region 2020DecRubikovaKocka
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
void CGLRenderer::SetMaterial(float r, float g, float b)
{
    float ambient[] = { 0.2 * r, 0.2 * g, 0.2 * b };
    float diffuse[] = { r, g, b };
    float specular[] = { r, g, b };
    float emission[] = { 0,0,0,1 }; //za emisionu su defaultne vrednosti 0,0,0,1 (iskljucena po defaultu)
                                    //da je trebalo iskljuciti npr. ambinetalnu, bilo bi ambient[] = {0,0,0,1}
    float shine = 64;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    glMaterialf(GL_FRONT, GL_SHININESS, shine);
}
void CGLRenderer::DrawCube(double a) {
    float d = a / 2;

    glBegin(GL_QUADS);

    glColor3f(1, 0, 0);
    glNormal3f(0, 0, 1);
    glVertex3f(-d, d, d); //1
    glVertex3f(-d, -d, d); //2
    glVertex3f(d, -d, d); //3
    glVertex3f(d, d, d); //4

    glColor3f(0, 0, 1);
    glNormal3f(1, 0, 0);
    glVertex3f(d, d, d); //4
    glVertex3f(d, -d, d); //3
    glVertex3f(d, -d, -d); //5
    glVertex3f(d, d, -d); //6

    glColor3f(1, 0.5, 0);
    glNormal3f(0, 0, -1);
    glVertex3f(d, d, -d); //6
    glVertex3f(d, -d, -d); //5
    glVertex3f(-d, -d, -d); //7
    glVertex3f(-d, d, -d); //8

    glColor3f(0, 1, 0);
    glNormal3f(-1, 0, 0);
    glVertex3f(-d, d, -d); //8
    glVertex3f(-d, -d, -d); //7
    glVertex3f(-d, -d, d); //2
    glVertex3f(-d, d, d); //1

    glColor3f(1, 1, 1);
    glNormal3f(0, 1, 0);
    glVertex3f(-d, d, d); //1
    glVertex3f(d, d, d); //4
    glVertex3f(d, d, -d); //6
    glVertex3f(-d, d, -d); //8

    glColor3f(1, 1, 0);
    glNormal3f(0, -1, 0);
    glVertex3f(-d, -d, d); //2  
    glVertex3f(d, -d, d); //3
    glVertex3f(d, -d, -d); //5
    glVertex3f(-d, -d, -d); //7

    glEnd();
}
void CGLRenderer::DrawRubikCube(double a, double* angle) {
    float stanicaKocke = a / 3;
    float pomeraj = stanicaKocke + 0.05 * stanicaKocke;
    float pomerajY;
    float pomerajZ;

    glPushMatrix();
    glTranslatef(pomeraj, 0, 0); // da bi srednja kompozicija po x bila u koord poc
    for (int x = 0; x < 3; x++) {
        glPushMatrix();
        glRotatef(angle[x], 1, 0, 0);
        for (int z = 0; z < 3; z++) {
            pomerajZ = pomeraj * z * pow(-1, z);
            glTranslatef(0, 0, pomerajZ);
            glPushMatrix();
            for (int y = 0; y < 3; y++) {
                pomerajY = pomeraj * y * pow(-1, y);
                glTranslatef(0, pomerajY, 0);
                DrawCube(stanicaKocke);
            }
            glPopMatrix();
        }
        glPopMatrix();
        glTranslatef(-pomeraj, 0, 0);
    }
    glPopMatrix();
}
void CGLRenderer::Draw() {
    double angles[] = { 0 , 20 ,30 };
    DrawRubikCube(3, angles);

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
