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

#pragma region 2015KOL2Zemlja
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

void CGLRenderer::DrawPatch(double R, int n)
{
    double delta = 2. / (double)n;
    double y = 1.0;
    for (int i = 0; i < n; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        double x = -1.0;
        for (int j = 0; j < n + 1; j++) {
            double phi, theta;
            inverseTSC(x, y, phi, theta);

            double xd, yd, zd;
            xd = R * cos(theta) * sin(phi);
            yd = R * sin(theta);
            zd = R * cos(theta) * cos(phi);

            glNormal3f(xd / R, yd / R, zd / R);
            glTexCoord2f((x + 1.0) / 2.0, (-y + 1.0) / 2.0);

            glVertex3f(xd, yd, zd);

            inverseTSC(x, y - delta, phi, theta);
            xd = R * cos(theta) * sin(phi);
            yd = R * sin(theta);
            zd = R * cos(theta) * cos(phi);

            glNormal3f(xd / R, yd / R, zd / R);
            glTexCoord2f((x + 1.0) / 2.0, (-y + delta + 1.0) / 2.0);
            glVertex3f(xd, yd, zd);
            x += delta;
        }
        glEnd();
        y -= delta;
    }
}

void CGLRenderer::DrawEarth(double R, int tes) {
    glPushMatrix();
    for (int i = 0; i < 4; i++) {
        glBindTexture(GL_TEXTURE_2D, T[i]);
        DrawPatch(R, tes);
        glRotatef(90, 0, 1, 0);
    }
    glPopMatrix();
    glRotatef(-90, 1, 0, 0);
    glBindTexture(GL_TEXTURE_2D, T[4]);
    DrawPatch(R, tes);
    glRotatef(-180, 1, 0, 0);
    glBindTexture(GL_TEXTURE_2D, T[5]);
    DrawPatch(R, tes);
    glRotatef(-90, 1, 0, 0);

}

void CGLRenderer::DrawMoon(double R, int tes) {
    glPushMatrix();
    for (int i = 0; i < 4; i++) {
        glBindTexture(GL_TEXTURE_2D, M[i]);
        DrawPatch(R, tes);
        glRotatef(90, 0, 1, 0);
    }
    glPopMatrix();
    glRotatef(-90, 1, 0, 0);
    glBindTexture(GL_TEXTURE_2D, M[4]);
    DrawPatch(R, tes);
    glRotatef(-180, 1, 0, 0);
    glBindTexture(GL_TEXTURE_2D, M[5]);
    DrawPatch(R, tes);
    glRotatef(-90, 1, 0, 0);

}

void CGLRenderer::inverseTSC(double x, double y, double& phi, double& theta) {
    phi = atan(x);
    theta = atan(y * cos(phi));
}

void CGLRenderer::DrawSpace(double R, int tes) {
    glPushMatrix();
    for (int i = 0; i < 4; i++) {
        glBindTexture(GL_TEXTURE_2D, S[i]);
        DrawPatch(R, tes);
        glRotatef(90, 0, 1, 0);
    }
    glPopMatrix();
    glRotatef(-90, 1, 0, 0);
    glBindTexture(GL_TEXTURE_2D, S[4]);
    DrawPatch(R, tes);
    glRotatef(-180, 1, 0, 0);
    glBindTexture(GL_TEXTURE_2D, S[5]);
    DrawPatch(R, tes);
    glRotatef(-90, 1, 0, 0);

}

void CGLRenderer::SetLight() {
    float ambient[] = { 0,0,0,1 };
    float diffuse[] = { 1,1,1,1 };
    float specular[] = { 1,1,1,1 };

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

void CGLRenderer::Draw() {
    DrawSpace(100, 20);
    DrawEarth(3, 20);
    glTranslatef(10, 0, 10);
    DrawMoon(2, 20);
    glTranslatef(-10, 0, -10);

}

#pragma endregion
#pragma endregion

void CGLRenderer::DrawScene(CDC* pDC)
{
    wglMakeCurrent(pDC->m_hDC, m_hrc);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

   //DrawAxes(10);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glPushMatrix();
    glRotated(m_beta, 1, 0, 0);
    glRotated(m_alpha, 0, 1, 0);
    DrawSpace(1, 20);
    glPopMatrix();

    //------------------------------------------

    if (m_bLight) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }

    SetLight();
    glEnable(GL_DEPTH_TEST);

    GLfloat lightPosition[] = { 0,0,1,0 };
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); //svetlo prati polozaj posmatraca, jer se ceo svet pomera u odnosu na posmatraca, pa samim tim i u odnosu na svetlo


    glTranslatef(0, 0, m_dist);
    glRotated(m_beta, 1, 0, 0);
    glRotated(m_alpha, 0, 1, 0);


    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); //svetlo ne prati polozaj posmatraca
    DrawEarth(3, 20);
    glTranslatef(-50, 0, 0);
    glRotatef(m_moonRot, 0, 1, 0);
    DrawMoon(1, 20);


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
    glDeleteTextures(6, T);
    glDeleteTextures(6, M);
    glDeleteTextures(6, S);

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
    T[0] = LoadTexture("./Teksture//TSC0.jpg");
    T[1] = LoadTexture("./Teksture//TSC1.jpg");
    T[2] = LoadTexture("./Teksture//TSC2.jpg");
    T[3] = LoadTexture("./Teksture//TSC3.jpg");
    T[4] = LoadTexture("./Teksture//TSC4.jpg");
    T[5] = LoadTexture("./Teksture//TSC5.jpg");

    M[0] = LoadTexture("./Teksture//M0.jpg");
    M[1] = LoadTexture("./Teksture//M1.jpg");
    M[2] = LoadTexture("./Teksture//M2.jpg");
    M[3] = LoadTexture("./Teksture//M3.jpg");
    M[4] = LoadTexture("./Teksture//M4.jpg");
    M[5] = LoadTexture("./Teksture//M5.jpg");

    S[0] = LoadTexture("./Teksture//S0.jpg");
    S[1] = LoadTexture("./Teksture//S1.jpg");
    S[2] = LoadTexture("./Teksture//S2.jpg");
    S[3] = LoadTexture("./Teksture//S3.jpg");
    S[4] = LoadTexture("./Teksture//S4.jpg");
    S[5] = LoadTexture("./Teksture//S5.jpg");

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
    gluPerspective(40, (double)w / (double)h, 0.1, 2000); //FOV 40

    //4)
    glMatrixMode(GL_MODELVIEW);

    //---------------------------------
    wglMakeCurrent(NULL, NULL);
}
#pragma endregion
