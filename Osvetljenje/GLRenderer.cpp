#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
#include <cmath>
//#pragma comment(lib, "GL\\glut32.lib")

void CGLRenderer::DrawAxes(double len, double lenWidth)
{
    glLineWidth(lenWidth);
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

#pragma region Vezba
#pragma region code
void CGLRenderer::SetLight() {
    float position[] = { 0,0,1,1 };
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    //AKO JE SVETLO PRE gluLookAt, svetlo prati polozaj posmatraca
    //ako je svetlo posle gluLookAt, svetlo je na fiksnoj poziciji
};
void CGLRenderer::DrawCircle(float r) {
    glColor3f(0, 0, 0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);

    for (int i = 0; i <= 360; i++) {
        glVertex3f(r * cos(i * toRad), 0, r * sin(i * toRad));
    }

    glEnd();
}
void CGLRenderer::DrawCircle(UINT texture, float r) {
    float textureR = 0.5;
    glColor3f(0, 0, 0);
    glEnable(GL_TEXTURE_2D);
    
    //glBindTexture(GL_TEXTURE_2D, woodTexture);
    glBegin(GL_TRIANGLE_FAN);

    glNormal3f(0, 0, 1);
    //glTexCoord2f(0.5, 0.5);
    glVertex3f(0, 0, 0);
    for (int i = 0; i <= 360; i++) {
        //glTexCoord2f(0.5 + textureR * cos(i * toRad), 0.5 + textureR * sin(i * toRad));
        glVertex3f(r * cos(i * toRad), r * sin(i * toRad), 0);
    }
    glEnd();
    
    //glDisable(GL_TEXTURE_2D);
}
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
void CGLRenderer::SetMaterial(float r, float g, float b)
{
    float diffuse[] = { r,g,b };
    float specular[] = { r,g,b };
    float ambinet[] = { r * 0.2,g * 0.2,b * 0.2 };
    float shine = 64;
    //float emission[] = { r * 1,g * 1,b * 1 };

    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambinet);
    glMaterialf(GL_FRONT, GL_SHININESS, shine);
    //glMaterialfv(GL_FRONT, GL_EMISSION, emission);
}
void CGLRenderer::DrawPlane(int rows, int cols, float size) {
    glColor3f(0, 0, 0);
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
    //NAPOMENA: Krug sam ovde crtao uspravno (y!=0), nije kao u ostalim u kojima y jeste 0
    glDisable(GL_LIGHTING);
    
    glTranslatef(-0.01, -0.1, -1); //pomeramo lokalni koordinatni sistem u kome se iscrtavaju objekti u negativnom smeru z ose, kako bi objekti bili isped kamere (posmatraca)
    DrawAxes(1,2);
    glTranslatef(0.01, 0.1, 1); //vracamo lokalni koordinatni sistem u mesto gde se nalazi kamera
    
    gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, -9999999, 0, 1, 0); //kamera se ne pomera, staticna je!!!! Pomera se svet u odnosu na kameru za -eyeX, -eyeY, -eyeZ i utice na sva kasnija iscrtavanja, pogledaj: https://www.youtube.com/watch?v=ODKfUez08zE&t=539s&ab_channel=JamieKing
                                                              //kamera gleda u -beskonacno po z osi. Po default-u se kamera (posmatrac) nalazi u 0,0,0 i gleda u 0,0,-1: gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0); 

    //***********************************************************************************************************
    //ako se svetlo postavi pre gluLookAt, ono prati polozaj posmatraca jer se ceo svet pomera u odnosu na posmatraca, pa samim tim i u odnosu na svetlo
    //ako se svetlo postavi posle gluLookAt, ono ne prati polozaj posmatraca jer je svet vec pomeren       
    //***********************************************************************************************************
    DrawAxes(5, 2);       //ose i ravan se ne vide bez gluLookAt jer su pozicioni na isto mesto gde i posmatrac
    DrawPlane(10, 10, 1); //bez gluLookAt morao bi da se radi transliranje kao u primeru iznad za iscrtavanje osa

    //glTranslatef(0, 0, -5);
    //SetLight();                     //light se postavlja u odnosu na trenutni polozaj lokalnog koordinatnog sistema
    //glTranslatef(0, 0, 5);
   
    glTranslatef(0, 0, zoom);         //pomeramo lokalni koordinatni sistem u kome se iscrtavaju objekti u negativnom smeru z ose, kako bi objekti bili isped posmatraca 
    glRotatef(rotacijaX, 1, 0, 0);
    glRotatef(rotacijaY, 0, 1, 0);
    glRotatef(rotacijaZ, 0, 0, 1);
    //SetLight();                       //light se postavlja u odnosu na trenutni polozaj lokalnog koordinatnog sistema
                                        //na ovaj nacin svetlost ne prati posmatraca jer je ceo svet vec pomeren u odnosu na njega
                                        //svetlost prati objekat, ali ne i posmatraca

    DrawAxes(10,2);
    DrawCircle(woodTexture, 2);
   


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
