#pragma once

class CGLRenderer
{
public:
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

    float toDeg, toRad, Pi;
    float rotY;
    float rotX;
    float dist;

    float rotCabine;
    float rotArm1;
    float rotArm2;
    float rotVilj;

    void DrawAxes();
    UINT LoadTexture(char* fileName);
    void DrawPolygon(POINTF * points, POINTF * texCoords, int n);
    void DrawExtrudedPolygon(POINTF * points, POINTF * texCoords, int n, float zh, float r, float g, float b);
    void DrawBase();
    void DrawBody();
    void DrawArm(double zh);
    void DrawFork();
    void DrawExcavator();
    void DrawEnvCube(double a);

    UINT back, bot, front, exc, left, right, top;

    float unit = 1. / 16.;

    void DrawValjak(float r, float h);
    void DrawKupa(float r, float h);
    void DrawSfera(float r);


protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
