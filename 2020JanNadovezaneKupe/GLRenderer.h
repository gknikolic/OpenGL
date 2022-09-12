#pragma once

class CGLRenderer
{
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

public:
	float rotacijaY = -45;
	float rotacijaX = 45;
	float rotacijaZ = 0;
	float zoom = -10;
	float pi = atan(1) * 4;
	float toRad = pi / 180; //2 * pi / 360
	float toDeg = 180 / pi; //360 / 2 * pi 
	UINT woodTexture; //tekstura

	void DrawAxes(double len);
	void Draw();

#pragma region 2020JanNadovezaneKupe
#pragma region code
	int PrepareTextures(CString strTex);
	void DrawCone(float h, float r, int nSeg);
	void DrawFigure(float h, float r, int nSeg, int n);
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
