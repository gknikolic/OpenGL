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

#pragma region BaseShapes
#pragma region code
void CGLRenderer::DrawPlane(int rows, int cols, float size) {
    glPushMatrix();
    for (int i = 0; i < rows; i++) {
        glPushMatrix();
        for (int j = 0; j < cols; j++) {
            DrawSquare(size);
            glTranslatef(size, 0, 0);
        }
        glPopMatrix();
        glTranslatef(0, 0, size);
    }
    glPopMatrix();

}
void CGLRenderer::DrawSquare(float size) {
      glBegin(GL_LINE_STRIP);
      glVertex3f(0, 0, 0);
      glVertex3f(size, 0, 0);
      glVertex3f(size, 0, size);
      glVertex3f(0, 0, size);
      glVertex3f(0, 0, 0);
      glEnd();
}
void CGLRenderer::DrawGrid(int n, int m, float size) {
    glPushMatrix();

    DrawPlane(n, m, size);
    glRotatef(90, 0, 0, 1);
    DrawPlane(n, m, size);
    glRotatef(-90, 0, 0, 1);
    glRotatef(-90, 1, 0, 0);
    DrawPlane(n, m, size);

    glPopMatrix();
}
void CGLRenderer::DrawTriangle(UINT texture, float size) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glNormal3f(0, 1, 0);
    glColor3f(1, 1, 1);

    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.5, 0);
    glVertex3f(0, 0, 0);
    glTexCoord2f(1, 1);
    glVertex3f(size / 2, 0, size);
    glTexCoord2f(0, 1);
    glVertex3f(-size / 2, 0, size);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}
//void CGLRenderer::DrawSfera(float r) {
//    glBegin(GL_QUAD_STRIP);
//        
//    for (int j = -90; j <= 90; j++) {
//        for (int i = 0; i < 360; i++) {
//            glVertex3f(r * sin(i * toRad) * cos(j * toRad), r * sin(j * toRad), r * cos(i * toRad) * cos(j * toRad));
//            glVertex3f(r * sin(i * toRad) * cos((j+1) * toRad), r * sin((j+1) * toRad), r * cos(i * toRad) * cos((j+1) * toRad));
//        }
//    }
//
//    glEnd();
//}
void CGLRenderer::DrawSfera(float r) {
    glBegin(GL_QUAD_STRIP);
    for (int j = -90; j < 90; j++) {
        for (int i = 0; i <= 360; i++) {
            glNormal3f(sin(i * toRad) * cos(j * toRad), sin(j * toRad), cos(i * toRad) * cos(j * toRad));
            glVertex3f(r * sin(i * toRad) * cos(j * toRad), r * sin(j * toRad), r * cos(i * toRad) * cos(j * toRad));
            glNormal3f(sin(i*toRad)*cos((j+1)*toRad), sin((j+1)*toRad), cos(i*toRad)*cos((j+1)*toRad));
            glVertex3f(r * sin(i * toRad) * cos((j + 1) * toRad), r * sin(j + 1) * toRad, r * cos(i * toRad) * cos((j + 1) * toRad));
        }
    }
    glEnd();
}
void CGLRenderer::DrawCube(double dSize)
{
    double a = dSize / 2;

    glBegin(GL_QUADS);

    glNormal3f(0, 0, 1);
    glVertex3d(-a, a, a);
    glVertex3d(-a, -a, a);
    glVertex3d(a, -a, a);
    glVertex3d(a, a, a);

    glNormal3f(1, 0, 0);
    glVertex3d(a, a, a);
    glVertex3d(a, -a, a);
    glVertex3d(a, -a, -a);
    glVertex3d(a, a, -a);

    glNormal3f(0, 0, -1);
    glVertex3d(a, a, -a);
    glVertex3d(a, -a, -a);
    glVertex3d(-a, -a, -a);
    glVertex3d(-a, a, -a);

    glNormal3f(-1, 0, 0);
    glVertex3d(-a, a, -a);
    glVertex3d(-a, -a, -a);
    glVertex3d(-a, -a, a);
    glVertex3d(-a, a, a);

    glEnd();

    glBegin(GL_QUADS);

    glNormal3f(0, 1, 0);
    glVertex3d(-a, a, a);
    glVertex3d(a, a, a);
    glVertex3d(a, a, -a);
    glVertex3d(-a, a, -a);

    glNormal3f(0, -1, 0);
    glVertex3d(-a, -a, -a);
    glVertex3d(a, -a, -a);
    glVertex3d(a, -a, a);
    glVertex3d(-a, -a, a);

    glEnd();

}
void CGLRenderer::DrawCircle(float r) {
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    for (int i = 0; i <= 360; i++) {
        glVertex3f(r * cos(i * toRad), 0, r * sin(i * toRad));
    }
    glEnd();
}
void CGLRenderer::DrawCircle(UINT texture, float r) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.5, 0.5);
    glVertex3f(0, 0, 0);
    for (int i = 0; i <= 360; i++) {
        glTexCoord2f(0.5+0.5*cos(i*toRad), 0.5 + 0.5*sin(i*toRad));
        glVertex3f(r * cos(i * toRad), 0, r * sin(i * toRad));
    }

    glEnd();

    glDisable(GL_TEXTURE_2D);
}
void CGLRenderer::DrawCylinder(float r, float height) {
    glTranslatef(0, -height / 2, 0);
    glRotatef(180, 1, 0, 0);
    DrawCircle(r);
    glRotatef(-180, 1, 0, 0);

    glTranslatef(0, height, 0);
    DrawCircle(r);
    glTranslatef(0, -height / 2, 0);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 360; i++) {
        glNormal3f(cos(i * toRad), 0, sin(i * toRad));
        glVertex3f(r * cos(i * toRad), -height / 2, r * sin(i * toRad));
        glVertex3f(r*cos(i*toRad), height/2, r*sin(i*toRad));
    }
    glEnd();
}
void CGLRenderer::DrawCylinder(UINT texture, float r, float height) {
    glTranslatef(0, -height / 2, 0);
    glRotatef(180, 1, 0, 0);
    DrawCircle(texture, r);
    glRotatef(-180, 1, 0, 0);

    glTranslatef(0, height, 0);
    DrawCircle(texture, r);
    glTranslatef(0, -height / 2, 0);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUAD_STRIP);

    float part = 1. / 360;

    for (int i = 0; i <= 360; i++) {
        glNormal3f(cos(i * toRad), 0, sin(i * toRad));
        glTexCoord2f(i * part, 1);
        glVertex3f(r * cos(i * toRad), -height / 2, r * sin(i * toRad));

        glTexCoord2f(i*part, 0);
        glVertex3f(r * cos(i * toRad), height / 2, r * sin(i * toRad));
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
void CGLRenderer::DrawRing(float rMalo, float rVeliko) {
    glBegin(GL_QUAD_STRIP);
    glNormal3f(0, 1, 0);
    for (int i = 0; i <= 360; i++) {
        glVertex3f(rVeliko * cos(i * toRad), 0, rVeliko * sin(i * toRad));
        glVertex3f(rMalo *cos(i*toRad), 0, rMalo*sin(i*toRad));
    }
    glEnd();
}
void CGLRenderer::DrawRing(UINT texture, float rMalo, float rVeliko) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUAD_STRIP);
    glNormal3f(0, 1, 0);
    float diff = 0.5 * rMalo / rVeliko;
    for (int i = 0; i <= 360; i++) {
        glTexCoord2f(0.5 + 0.5 * cos(i * toRad), 0.5 + 0.5 * sin(i * toRad));
        glVertex3f(rVeliko * cos(i * toRad), 0, rVeliko * sin(i * toRad));
        
        glTexCoord2f(0.5+diff*cos(i*toRad), 0.5 + diff*sin(i*toRad));
        glVertex3f(rMalo * cos(i * toRad), 0, rMalo * sin(i * toRad));
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);


}
void CGLRenderer::DrawCone(float r, float h) {
    glRotatef(180, 1,0,0);
    DrawCircle(r);
    glRotatef(180, 1, 0, 0);

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0);
    glVertex3f(0, h, 0);
    for (int i = 0; i <= 360; i++) {
        glNormal3f(cos(i * toRad), 0, sin(i * toRad));
        glVertex3f(r * cos(i * toRad), 0, r * sin(i * toRad));
    }
    glEnd();
}
void CGLRenderer::DrawCone(UINT texture, float r, float h){
    glRotatef(180, 1, 0, 0);
    DrawCircle(texture, r);
    glRotatef(180, 1, 0, 0);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.5, 0.5);
    glVertex3f(0, h, 0);

    for (int i = 0; i <= 360; i++) {
        glNormal3f(cos(i * toRad), 0, sin(i * toRad));
        glTexCoord2f(0.5+0.5*cos(i*toRad), 0.5+0.5*sin(i*toRad));
        glVertex3f(r * cos(i * toRad), 0, r * sin(i * toRad));
    }

    glEnd();
    glDisable(GL_TEXTURE_2D);
}
void CGLRenderer::DrawPolygon(float size) {
    glBegin(GL_POLYGON);
    glTexCoord2f(0.5, 0);
    glVertex3f(0, 2, 0);
    glTexCoord2f(1, 1);
    glVertex3f(size / 2, 0, size);
    glTexCoord2f(0, 1);
    glVertex3f(-size / 2, 0, size);
    glEnd();
    glEnd();
}
void CGLRenderer::Draw() {
    //glColor3f(0, 0, 0);
    //glColor3f(1, 1, 1);
    //DrawCylinder(woodTexture, 2, 3);
    //DrawSfera(2);
    DrawPolygon(2);
}
void CGLRenderer::SetLight() {
    float ambient[] = { 1,1,1,1 };
    float diffuse[] = { 1,1,1,1 };
    float specular[] = { 1,1,1,1 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    float position[] = { 0,0,2,1 };
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHT0);

    //AKO JE SVETLO PRE gluLookAt, svetlo prati polozaj posmatraca
    //ako je svetlo posle gluLookAt, svetlo je na fiksnoj poziciji
};
UINT CGLRenderer::LoadTexture(char* fileName) {
    UINT textureID;

    DImage img;
    img.Load(CString(fileName));

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img.Width(), img.Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img.GetDIBBits());

    return textureID;
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
    //glDisable(GL_LIGHTING);

    glTranslatef(-0.01, -0.1, -1); //pomeramo lokalni koordinatni sistem u kome se iscrtavaju objekti u negativnom smeru z ose, kako bi objekti bili isped kamere (posmatraca)
    DrawAxes(1);
    glTranslatef(0.01, 0.1, 1); //vracamo lokalni koordinatni sistem u mesto gde se nalazi kamera


    //SetLight(); // svetlost je postavljenja na 0,0,1 u odnosu na trenutni lokalni koordinatni sistem
    glTranslatef(0, 0, zoom);
    glRotatef(rotacijaX, 1, 0, 0);
    glRotatef(rotacijaY, 0, 1, 0);
    glRotatef(rotacijaZ, 0, 0, 1);
    //glDisable(GL_LIGHTING);
    DrawAxes(10);
    SetLight(); // svetlost je postavljenja na 0,0,1 u odnosu na trenutni lokalni koordinatni sistem
    glEnable(GL_LIGHTING);

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

#pragma region 2019 KOL2 BAGER
    //------------------------------------------


    //BRISANJE UCITANE TEKSTURE
    //glDeleteTextures(1, &top);


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
    //woodTexture = LoadTexture("./Teksture//download.jpg");
    woodTexture = LoadTexture("./Teksture//images.jpg");


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
