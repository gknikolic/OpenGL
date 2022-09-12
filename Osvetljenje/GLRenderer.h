#pragma once

class CGLRenderer
{
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

public:
	float eyeX = -0.5;
	float eyeY = 2;
	float eyeZ = 7;

	float rotacijaY = 0;
	float rotacijaX = 0;
	float rotacijaZ = 0;
	float zoom = -10;
	float pi = atan(1) * 4;
	float toRad = pi / 180; //2 * pi / 360
	float toDeg = 180 / pi; //360 / 2 * pi 


	void DrawAxes(double len, double lenWidth);
	void Draw();


#pragma region Vezba
#pragma region code
	UINT woodTexture; //tekstura
	int cubeRotation = 0;

	void DrawCircle(float r);
	void DrawCircle(UINT texture, float r);
	void DrawSquare(float size);
	void DrawPlane(int rows, int cols, float size);

	UINT LoadTexture(char* fileName);
	void SetMaterial(float r, float g, float b);
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
