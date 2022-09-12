#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include "DImage.h"
#include <cmath>
//#pragma comment(lib, "GL\\glut32.lib")

CGLRenderer::CGLRenderer(void)
{
    Pi = atan(1.) * 4;
    toRad = 2 * Pi / 360;
    toDeg = 360. / 2 * Pi;

    rotY = 40;
    rotX = -10;
    dist = 17.5; 
    rotCabine = 0;
    rotArm1 = 90;
    rotArm2 = 90;
    rotVilj = 90;
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
    
    //1) color
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //2)depth test
    glEnable(GL_DEPTH_TEST);
    
    // teksture - load i enable
    back = LoadTexture("back.jpg");
    bot = LoadTexture("bot.jpg");
    front = LoadTexture("front.jpg");
    exc = LoadTexture("excavator.png"); 
    left = LoadTexture("left.jpg"); 
    right = LoadTexture("right.jpg");
    top = LoadTexture("top.jpg");

    glEnable(GL_TEXTURE_2D);


	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	

    //1)clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //2)ident
    glLoadIdentity();

    glPushMatrix();
    //------------------------------------------
    
    glTranslatef(0, -5, -dist);
    glRotatef(-rotX, 1, 0, 0);
    glRotatef(-rotY, 0, 1, 0);
 
    glTranslatef(0, 50, 0);
    //env cube
    DrawEnvCube(100.);
   
    glTranslatef(0, -50, 0);
    //axes
    DrawAxes();

    DrawExcavator();


    //dodatno
    glPopMatrix();
    glPushMatrix();

    glTranslatef(0, -5, -dist);
    glRotatef(-rotX, 1, 0, 0);
    glRotatef(-rotY, 0, 1, 0);

    //axes
    DrawAxes();
    
    glDisable(GL_TEXTURE_2D);

    glTranslatef(15, 5, 0);
    DrawValjak(2, 3);

    glTranslatef(5, 0, 0);
    DrawKupa(2, 3);

    glTranslatef(5, 0, 0);
    DrawSfera(2);

    glEnable(GL_TEXTURE_2D);

    //------------------------------------------
    glPopMatrix();

    //3) Flush
    glFlush();
    //4) swap
    SwapBuffers(pDC->m_hDC);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
    
    //1)
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    
    //2)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //3)
    gluPerspective(55, (double)w / (double)h, 0.1, 2000);

    //4)
    glMatrixMode(GL_MODELVIEW);

	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	// ... 
    glDeleteTextures(1, &top);
    //....

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawAxes()
{
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(10, 0, 0);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 10, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 10);

    glColor3f(1, 1, 1);
    glEnd();
}

UINT CGLRenderer::LoadTexture(char * fileName)
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

void CGLRenderer::DrawPolygon(POINTF* points, POINTF* texCoords, int n)
{
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < n; i++)
    {
        glTexCoord2f(texCoords[i].x, texCoords[i].y);
        glVertex2f(points[i].x, points[i].y);
    }
    glEnd();
}


void CGLRenderer::DrawExtrudedPolygon(POINTF* points,
    POINTF* texCoords, int n, float zh, float r, float g, float b)
{
    glNormal3f(0, 0, 1);
    DrawPolygon(points, texCoords, n);

    glDisable(GL_TEXTURE_2D);
    glBegin(GL_QUAD_STRIP);
    glColor3f(r, g, b);
    for (int i = 0; i < n; i++)
    {
        glVertex3f(points[i].x, points[i].y, 0.0);
        glVertex3f(points[i].x, points[i].y, zh);
    }
    glVertex3f(points[0].x, points[0].y, 0.0);
    glVertex3f(points[0].x, points[0].y, zh);
    glColor3f(1, 1, 1);
    glEnd();
    glEnable(GL_TEXTURE_2D);

    glTranslatef(0, 0, zh);
    glNormal3f(0, 0, -1);
    DrawPolygon(points, texCoords, n);
    glTranslatef(0, 0, -zh);
}

void CGLRenderer::DrawBase()
{
    POINTF points[] = {
        {0, 0},
        {-0.5, 0.5},
        {-0.5, 1.5},
        {0, 2},
        {7, 2},
        {7.5, 1.5},
        {7.5, 0.5},
        {7, 0}
    };
    POINTF texp[] = {
        {1 * unit, 16 * unit},
        {0 * unit, 15 * unit},
        {0 * unit, 13 * unit},
        {1 * unit, 12 * unit},
        {15 * unit, 12 * unit},
        {16 * unit, 13 * unit},
        {16 * unit, 15 * unit},
        {15 * unit, 16 * unit}

    };
    DrawExtrudedPolygon(points, texp, 8, 5.0, 0., 0., 0.);
}

void CGLRenderer::DrawBody()
{
    POINTF points[] = {
        {0, 0},
        {0, 4},
        {2, 4},
        {4, 2},
        {4, 0}
        
    };
    POINTF texp[] = {
        {8 * unit, 8 * unit},
        {8 * unit, 0 * unit},
        {12 * unit, 0 * unit},
        {16 * unit, 4 * unit},
        {16 * unit, 8 * unit}

    };
    DrawExtrudedPolygon(points, texp, 5, 4.0, 0.96, 0.5, 0.12);
}

void CGLRenderer::DrawArm(double zh)
{
    POINTF points[] = {
        {0, 0},
        {-0.5, 0.5},
        {-0.5, 1.5},
        {0, 2},
        {7.5, 1.5},
        {7.5, 0.5}

    };
    POINTF texp[] = {
        {1 * unit, 12 * unit},
        {0 * unit, 11 * unit},
        {0 * unit, 9 * unit},
        {1 * unit, 8 * unit},
        {16 * unit, 9 * unit},
        {16 * unit, 11 * unit}

    };
    DrawExtrudedPolygon(points, texp, 6, zh, 0.96, 0.5, 0.12);

}

void CGLRenderer::DrawFork()
{
    POINTF points[] = {
        {0, 0},
        {0., 2.5},
        {0.5, 3},
        {3.5, 3},
        {4, 2.5},
        {4, 0}
    };
    POINTF texp[] = {
        {0 * unit, 6 * unit},
        {0 * unit, 1 * unit},
        {1 * unit, 0 * unit},
        {7 * unit, 0 * unit},
        {8 * unit, 1 * unit},
        {8 * unit, 6 * unit}

    };
    DrawExtrudedPolygon(points, texp, 6, 0.5, 0.7, 0.7, 0.7);
}

void CGLRenderer::DrawExcavator()
{
    glBindTexture(GL_TEXTURE_2D, exc);

    glPushMatrix();
    glTranslatef(-4, 0, -2.5);
    //base
    DrawBase();

    glPopMatrix();
    glPushMatrix();

    glRotatef(rotCabine, 0, 1, 0);
    glTranslatef(-2.0, 2, -2);
    //body
    DrawBody();

    glTranslatef(2.0, -2, 2);

    glTranslatef(1.0, 3, -3.0);
    glRotatef(rotArm1, 0, 0, 1);
    glTranslatef(-0.5, -1, 0.5);
    //Arm1
    DrawArm(0.5);

    glTranslatef(7, 1, 0);
    glRotatef(-rotArm2, 0, 0, 1);
    glTranslatef(-0.5, -1, 0);
    //Arm2
    DrawArm(0.5);

    glTranslatef(8, 1, 0);
    glRotatef(-rotVilj, 0, 0, 1);
    glTranslatef(-1, -1, 0);
    //vilj
    DrawFork();

    glPopMatrix();
}

void CGLRenderer::DrawEnvCube(double a1)
{
    float a = a1 / 2;

    glPushMatrix();

    //omotac
    glPushMatrix();
    {
        glBindTexture(GL_TEXTURE_2D, front);
        glBegin(GL_QUADS);
        {
            glNormal3f(0, 0, 1);

            glTexCoord2f(0, 0);
            glVertex3f(-a, a, -a);

            glTexCoord2f(1, 0);
            glVertex3f(a, a, -a);

            glTexCoord2f(1, 1);
            glVertex3f(a, -a, -a);

            glTexCoord2f(0, 1);
            glVertex3f(-a, -a, -a);
        }
        glEnd();

        glRotatef(90, 0, 1, 0);

        //////

        glBindTexture(GL_TEXTURE_2D, left);
        glBegin(GL_QUADS);
        {
            glNormal3f(0, 0, 1);

            glTexCoord2f(0, 0);
            glVertex3f(-a, a, -a);

            glTexCoord2f(1, 0);
            glVertex3f(a, a, -a);

            glTexCoord2f(1, 1);
            glVertex3f(a, -a, -a);

            glTexCoord2f(0, 1);
            glVertex3f(-a, -a, -a);
        }
        glEnd();

        glRotatef(90, 0, 1, 0);

        //////

        glBindTexture(GL_TEXTURE_2D, back);
        glBegin(GL_QUADS);
        {
            glNormal3f(0, 0, 1);

            glTexCoord2f(0, 0);
            glVertex3f(-a, a, -a);

            glTexCoord2f(1, 0);
            glVertex3f(a, a, -a);

            glTexCoord2f(1, 1);
            glVertex3f(a, -a, -a);

            glTexCoord2f(0, 1);
            glVertex3f(-a, -a, -a);
        }
        glEnd();

        glRotatef(90, 0, 1, 0);

        //////

        glBindTexture(GL_TEXTURE_2D, right);
        glBegin(GL_QUADS);
        {
            glNormal3f(0, 0, 1);

            glTexCoord2f(0, 0);
            glVertex3f(-a, a, -a);

            glTexCoord2f(1, 0);
            glVertex3f(a, a, -a);

            glTexCoord2f(1, 1);
            glVertex3f(a, -a, -a);

            glTexCoord2f(0, 1);
            glVertex3f(-a, -a, -a);
        }
        glEnd();
    }
    glPopMatrix();

    //gornja
    glRotatef(90, 1, 0, 0);

    glBindTexture(GL_TEXTURE_2D, top);
    glBegin(GL_QUADS);
    {
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-a, a, -a);

        glTexCoord2f(1, 0);
        glVertex3f(a, a, -a);

        glTexCoord2f(1, 1);
        glVertex3f(a, -a, -a);

        glTexCoord2f(0, 1);
        glVertex3f(-a, -a, -a);
    }
    glEnd();

    glRotatef(-180, 1, 0, 0);

    //donja
    glBindTexture(GL_TEXTURE_2D, bot);
    glBegin(GL_QUADS);
    {
        glNormal3f(0, 0, 1);

        glTexCoord2f(0, 0);
        glVertex3f(-a, a, -a);

        glTexCoord2f(1, 0);
        glVertex3f(a, a, -a);

        glTexCoord2f(1, 1);
        glVertex3f(a, -a, -a);

        glTexCoord2f(0, 1);
        glVertex3f(-a, -a, -a);
    }
    glEnd();

    glPopMatrix();
}

void CGLRenderer::DrawValjak(float r, float h)
{
    //krug
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (int i = 0; i < 360; i++)
    {
        glVertex3f(r * sin(i * toRad), 0, r * cos(i * toRad));
    }
    glEnd();


    //omotac
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < 360; i++)
    {
        glVertex3f(r * sin(i * toRad), h, r * cos(i * toRad));
        glVertex3f(r * sin(i * toRad), 0, r * cos(i * toRad));
    }
    glEnd();



    //krug
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, h, 0);
    for (int i = 0; i < 360; i++)
    {
        glVertex3f(r * sin(i * toRad), h, r * cos(i * toRad));
    }
    glEnd();
}

void CGLRenderer::DrawKupa(float r, float h)
{
    //krug
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, 0, 0);
    for (int i = 0; i < 360; i++)
    {
        glVertex3f(r * sin(i * toRad), 0, r * cos(i * toRad));
    }
    glEnd();


    //omotac
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, h, 0);
    for (int i = 0; i < 360; i++)
    {
        glVertex3f(r * sin(i * toRad), 0, r * cos(i * toRad));
    }
    glEnd();
}

void CGLRenderer::DrawSfera(float r)
{
    glTranslatef(0, r, 0);
    glBegin(GL_QUAD_STRIP);
    for (int j = -90; j < 90; j++)
    {
        for (int i = 0; i < 360; i++)
        {
            glVertex3f(r * sin(i * toRad)*cos(j * toRad), r*sin(j * toRad), r * cos(i * toRad)*cos(j * toRad));
            glVertex3f(r * sin(i * toRad)*cos((j + 1) * toRad), r*sin((j+1) * toRad), r * cos(i * toRad)*cos((j + 1) * toRad));
        }
    }
    glEnd();
    glTranslatef(0, -r, 0);
}
