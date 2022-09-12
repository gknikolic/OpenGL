#pragma once

class CGLRenderer
{
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

public:
	float rotacijaX = 45;
	float rotacijaY = -45;
	float rotacijaZ = 0;
	float zoom = -10;
	float pi = atan(1) * 4;
	float toRad = pi / 180; //2 * pi / 360
	float toDeg = 180 / pi; //360 / 2 * pi 


	void DrawAxes(double len);
	void Draw();


#pragma region BaseShapes
#pragma region code
	UINT woodTexture; //tekstura
	int cubeRotation = 0;

	void DrawSfera(float r);
	void DrawSquare(float size);
	void DrawPlane(int rows, int cols, float size);
	void DrawGrid(int n, int m, float size);
	void DrawTriangle(UINT texture, float size);
	void DrawCircle(float r);
	void DrawCircle(UINT texture, float r);
	void DrawCube(double dSize);
	void DrawCylinder(float r, float height);
	void DrawCylinder(UINT texture, float r, float height);
	void DrawRing(float rMalo, float rVeliko);
	void DrawRing(UINT texture, float rMalo, float rVeliko);
	void DrawCone(float r, float h);
	void DrawCone(UINT texture, float r, float h);

	void DrawPolygon(float r);

	UINT LoadTexture(char* fileName);
	void SetLight();
#pragma endregion
#pragma endregion


#pragma region PROJECT FUNCTIONS
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);   // kod koji treba se izvrsi svaki put kada se promeni velicina prozora ili pogleda
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
#pragma endregion 
};	
