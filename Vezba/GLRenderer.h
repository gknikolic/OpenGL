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


	void DrawAxes(double len);
	void Draw();

#pragma region 2015KOL2Zemlja
#pragma region code
	//PrepareScene, DrawScene i Reshape su iste kao one sto vec ima 
	UINT LoadTexture(char* fileName);

	void DrawPatch(double R, int n);
	void DrawEarth(double R, int tes);
	void DrawMoon(double R, int tes);
	void DrawSpace(double R, int tes);
	void inverseTSC(double x, double y, double& phi, double& theta);
	void SetLight();
	UINT T[6], S[6], M[6];

	float m_alpha = 0;
	float m_beta = 0;
	float m_dist = -10;
	float m_moonRot = 0;
	bool m_bLight = false;

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
