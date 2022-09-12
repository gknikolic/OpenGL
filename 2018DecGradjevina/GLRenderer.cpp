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
void CGLRenderer::SetLight() {

    //postoji 8 izvora koja mogu da se postave, pise u zadatku da se postavi light0
    glEnable(GL_LIGHT0);

    //omogucavanje izvora
    glEnable(GL_LIGHTING);

    float position[] = { 0, 0, 1, 1 }; 
    //prva tri parametra su ose, 4 parametar ako je 0 onda je svetlo u beskonacnosti, ako je 1 onda je na fiksnoj poziciji
    
    glLightfv(GL_LIGHT0, GL_POSITION, position); //postavljanje izvora svetlosti
};  

void CGLRenderer::DrawBox(double a, double b, double c) {
    glTranslatef(0,-c/2, 0);

    //DONJA
    glNormal3f(0, -1, 0);
    glBegin(GL_QUADS);
    glVertex3f(-a / 2, 0, b / 2);
    glVertex3f(a / 2, 0, b / 2);
    glVertex3f(a / 2, 0, -b / 2);
    glVertex3f(-a / 2, 0, -b / 2);
    glEnd();

    glTranslatef(0, c , 0);

    //GORNJA
    glNormal3f(0, 1, 0);
    glBegin(GL_QUADS);
    glVertex3f(-a / 2, 0, b / 2);
    glVertex3f(a / 2, 0, b / 2);
    glVertex3f(a / 2, 0, -b / 2);
    glVertex3f(-a / 2, 0, -b / 2);
    glEnd();

    //OMOTAC
    glBegin(GL_QUAD_STRIP);
    glVertex3f(-a / 2, 0, b / 2);
    glVertex3f(-a / 2, -c, b / 2);

    glNormal3f(0, 0, 1);
    glVertex3f(a / 2, 0, b / 2);
    glVertex3f(a / 2, -c, b / 2);

    glNormal3f(1, 0, 0);
    glVertex3f(a / 2, 0, -b / 2);
    glVertex3f(a / 2, -c, -b / 2);

    glNormal3f(0, 0, -1);
    glVertex3f(-a / 2, 0, -b / 2);
    glVertex3f(-a / 2, -c, -b / 2);

    glNormal3f(-1, 0, 0);
    glVertex3f(-a / 2, 0, b / 2);
    glVertex3f(-a / 2, -c, b / 2);
    glEnd();
}

void CGLRenderer::DrawCylinder(double r, double h, int steps) {
    float angle = 360 / (float)steps;

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= steps; i++) {
        glNormal3f(cos(i * angle * toRad), 0, sin(i * angle * toRad));
        glVertex3f(r * cos(i * angle * toRad), 0, r * sin(i * angle * toRad));
        glVertex3f(r * cos(i * angle * toRad), h, r * sin(i * angle * toRad));
    }
    glEnd();
}
void CGLRenderer::DrawColumn(double r, double h, int steps) {
    DrawBox(4 * r, 4 * r, 0.3 * r);
    glTranslatef(0, 0.3*r/2, 0);
    glColor3f(0, 1, 0);
    DrawBox(3 * r, 3 * r, 0.3 * r);
    glColor3f(0, 0, 0);
    DrawCylinder(r, h, steps);
    glTranslatef(0, 0.5 * r / 2+h, 0);
    glColor3f(0.5, 0.5, 0.5);
    DrawBox(2.5 * r, 2.5 * r, 0.5 * r);

}
void CGLRenderer::DrawTEMPLE() {
    float A=14, a=12;
    float B = 26, b = 24;
    float C = 0.75, c = 0.5;

    DrawBox(A, B, C);
    glTranslatef(0, c/2, 0);
    glColor3f(1, 0, 0);
    DrawBox(a, b, c);
    
    float vecaKocka = 4 * 0.5;
    float razmakOdIvica = (12 - (3 * 3) - (2 * vecaKocka / 2)) / 2;
    
    float debljinaVeceKocke = 0.3 * 0.5;
    float visinaZaStub = debljinaVeceKocke / 2;
    float razmak = vecaKocka / 2;
   // glTranslatef(-(razmakOdIvica + razmak), 0, -(razmakOdIvica + razmak));
    glPushMatrix();
    glTranslatef(a / 2, 0, b / 2);
    glTranslatef(-(razmakOdIvica +razmak), 0, -(razmakOdIvica+razmak));
    glTranslatef(0, visinaZaStub, 0); //

    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 8; i++) {
            glPushMatrix();
            glColor3f(0, 0, 1);
            DrawColumn(0.5, 8, 360);
            glPopMatrix();
            glTranslatef(0, 0, -3);
        }
        glTranslatef(0, 0, 3);
        for (int i = 0; i < 2; i++) {
            glTranslatef(-3, 0, 0);
            glPushMatrix();
            DrawColumn(0.5, 8, 360);
            glPopMatrix();
        }
        glTranslatef(-3, 0, 0);
        glRotatef(180, 0, 1, 0);
        //glScalef(-1, 1, -1);
    }
    glPopMatrix();
    float debljinaGornjeKocke = 0.5 * 0.5; //r valja je 0.5
    glTranslatef(0, 2*debljinaVeceKocke+debljinaGornjeKocke+8+0.15, 0);
    glColor3f(0, 0, 0);
    DrawBox(12, 24, 0.3);
    glTranslatef(12 / 2, 24 / 12, 0);
    glRotatef(-60, 0, 0, 1);
    DrawBox(12, 24, 0.3);
}
void CGLRenderer::Draw() {
    //DrawBox(1.5, 1, 0.5);

    DrawTEMPLE();
}
#pragma endregion
#pragma endregion

void CGLRenderer::DrawScene(CDC* pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    //------------------------------------------



#pragma region Camera 
    //SVETLO SE POSTAVLJA U ODNOSU NA TRENUTNI POLOZAJ LOKALNOG KOORDINATNOG SISTEMA!
    
    //SetLight();
    //gluLookAt(20, 20, 20, 0.0, 0.0, 0.0, 0, 1, 0);
    //SetLight();
    //AKO JE SVETLO PRE gluLookAt, svetlo prati polozaj posmatraca
    //ako je svetlo posle gluLookAt, svetlo je na fiksnoj poziciji


    //ne pomera se posmatrac, vec koordinatni sistem u kome se iscrtava!
    //AKO JE SVETLO PRE transformacija, svetlo prati polozaj posmatraca
    //ako je svetlo posle transformacija, svetlo ne prati polozaj posmatraca
    SetLight();
    glTranslatef(0, 0, zoom);
    glRotatef(rotacijaX, 1, 0, 0);
    glRotatef(rotacijaY, 0, 1, 0);
    glRotatef(rotacijaZ, 0, 0, 1);
    //SetLight();


#pragma endregion
    DrawAxes(50);
    //------------------------------------------

  

    Draw();


    //------------------------------------------
    glFlush();
    SwapBuffers(pDC->m_hDC);
    wglMakeCurrent(NULL, NULL);
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
    
    glEnable(GL_DEPTH_TEST); // UKLJUCIVANJE Z BAFERA (da bi se videlo sta je iza, a sta ispred)
    glClearColor(1.0, 1.0, 1.0, 1.0); //(boji pozadinu)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //brisanje bafera pre iscrtavanja

    //3)
    gluPerspective(45, (double)w / (double)h, 0.1, 2000);

    //4)
    glMatrixMode(GL_MODELVIEW);

    //---------------------------------
    wglMakeCurrent(NULL, NULL);
}
#pragma endregion
