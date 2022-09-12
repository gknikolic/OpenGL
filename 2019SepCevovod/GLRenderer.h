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
	UINT LoadTexture(char* fileName);


	void DrawAxes(double len);
	void Draw();

#pragma region imeBlanketa
#pragma region code
	float alphaRot = 0;
	float betaRot = 0;
	float gamaRot = 0;

	void DrawRoller(float r, float h, int nPartR, int nPartH);
	void DrawTwoRollers(float r, float h, float p, int nPartR, int nPartH);
	UINT PrepareTextures(CString strText);
	void DrawFigure(float r, float d, float hp, float h1, float p1, float h2, float p2, float h3, float p3, float alpha, float beta, float gama, CString strTex);
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
