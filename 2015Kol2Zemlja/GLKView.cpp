
// GLKView.cpp : implementation of the CGLKView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GLK.h"
#endif

#include "GLKDoc.h"
#include "GLKView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGLKView

IMPLEMENT_DYNCREATE(CGLKView, CView)

BEGIN_MESSAGE_MAP(CGLKView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGLKView construction/destruction

CGLKView::CGLKView()
{
	// TODO: add construction code here

}

CGLKView::~CGLKView()
{
}

BOOL CGLKView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGLKView drawing

void CGLKView::OnDraw(CDC* pDC)
{
	CGLKDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_glRenderer.DrawScene(pDC);
}


// CGLKView printing

BOOL CGLKView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGLKView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGLKView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGLKView diagnostics

#ifdef _DEBUG
void CGLKView::AssertValid() const
{
	CView::AssertValid();
}

void CGLKView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGLKDoc* CGLKView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLKDoc)));
	return (CGLKDoc*)m_pDocument;
}
#endif //_DEBUG


// CGLKView message handlers


int CGLKView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDC* pDC = GetDC();
	m_glRenderer.CreateGLContext(pDC);
	ReleaseDC(pDC);

	return 0;
}


void CGLKView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CDC* pDC = GetDC();
	m_glRenderer.Reshape(pDC, cx, cy);
	ReleaseDC(pDC);
}


void CGLKView::OnDestroy()
{
	CView::OnDestroy();

	CDC* pDC = GetDC();
	m_glRenderer.DestroyScene(pDC);
	ReleaseDC(pDC);	
}


BOOL CGLKView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CGLKView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CDC* pDC = GetDC();
	m_glRenderer.PrepareScene(pDC);
	ReleaseDC(pDC);
}


void CGLKView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
#pragma region Vezba
//#pragma region code
//	if (nChar == '1') {
//		m_glRenderer.cubeRotation += 30;
//	}
//#pragma endregion
#pragma endregion

	if (nChar == VK_TAB)
	{
		exit(0);
	}
	if (nChar == VK_RIGHT) m_glRenderer.m_alpha -= 5;
	if (nChar == VK_LEFT) m_glRenderer.m_alpha += 5;
	if (nChar == VK_UP) m_glRenderer.m_beta += 5;
	if (nChar == VK_DOWN) m_glRenderer.m_beta -= 5;
	if (nChar == VK_ADD) m_glRenderer.m_dist /= 1.1;
	if (nChar == VK_SUBTRACT) m_glRenderer.m_dist *= 1.1;
	if (nChar == 'Q') m_glRenderer.m_moonRot -= 5;
	if (nChar == 'W') m_glRenderer.m_moonRot += 5;
	if (nChar == 'S') m_glRenderer.m_bLight = !m_glRenderer.m_bLight;

	/*if (nChar == VK_LEFT)
	{
		m_glRenderer.rotacijaY += 10;
	}
	if (nChar == VK_RIGHT)
	{
		m_glRenderer.rotacijaY -= 10;
	}
	if (nChar == VK_UP)
	{
		m_glRenderer.rotacijaX += 10;
	}
	if (nChar == VK_DOWN)
	{
		m_glRenderer.rotacijaX -= 10;
	}
	if (nChar == (char)'A')
	{
		m_glRenderer.rotacijaZ -= 10;
	}
	if (nChar == (char)'D')
	{
		m_glRenderer.rotacijaZ += 10;
	}
	if (nChar == VK_ADD)
	{
		m_glRenderer.zoom += 1;
	}
	if (nChar == VK_SUBTRACT)
	{
		m_glRenderer.zoom -= 1;
	}*/
    /*float step = 10;
    if (nChar == VK_RIGHT)
    {
        m_glRenderer.rotY += step;
    } else if (nChar == VK_LEFT)
    {
        m_glRenderer.rotY -= step;
    }
    else if (nChar == VK_UP)
    {
        m_glRenderer.rotX += step;
    }
    else if (nChar == VK_DOWN)
    {
        m_glRenderer.rotX -= step;
    }
    else if (nChar == VK_SUBTRACT)
    {
        m_glRenderer.dist *= 1.5;
        if (m_glRenderer.dist > 50)
            m_glRenderer.dist = 50;
    }
    else if (nChar == VK_ADD)
    {
        m_glRenderer.dist /= 1.5;
        if (m_glRenderer.dist < 8)
            m_glRenderer.dist = 8;
    }
    else if (nChar == '1')
    {
        m_glRenderer.rotCabine += step;
    }
    else if (nChar == '2')
    {
        m_glRenderer.rotCabine -= step;
    }
    else if (nChar == '3')
    {
        m_glRenderer.rotArm1 += step;
    }
    else if (nChar == '4')
    {
        m_glRenderer.rotArm1 -= step;
    }
    else if (nChar == '5')
    {
        m_glRenderer.rotArm2 += step;
    }
    else if (nChar == '6')
    {
        m_glRenderer.rotArm2 -= step;
    }
    else if (nChar == '7')
    {
        m_glRenderer.rotVilj += step;
    }
    else if (nChar == '8')
    {
        m_glRenderer.rotVilj -= step;
    }
    else
        return;
    */
    Invalidate();
    CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
