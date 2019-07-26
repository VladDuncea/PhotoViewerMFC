
// PhotoViewerMFCView.cpp : implementation of the CPhotoViewerMFCView class
//

#include "pch.h"
#include "framework.h"
#include "resource.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PhotoViewerMFC.h"
#endif

#include "PhotoViewerMFCDoc.h"
#include "PhotoViewerMFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPhotoViewerMFCView

IMPLEMENT_DYNCREATE(CPhotoViewerMFCView, CFormView)

BEGIN_MESSAGE_MAP(CPhotoViewerMFCView, CFormView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPhotoViewerMFCView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SIZING()
END_MESSAGE_MAP()

// CPhotoViewerMFCView construction/destruction

CPhotoViewerMFCView::CPhotoViewerMFCView() noexcept
	: CFormView(IDD_PHOTOVIEWERMFC_FORM)
{
	// TODO: add construction code here
}

CPhotoViewerMFCView::~CPhotoViewerMFCView()
{
}

void CPhotoViewerMFCView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CPhotoViewerMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CPhotoViewerMFCView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}

// CPhotoViewerMFCView printing


void CPhotoViewerMFCView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPhotoViewerMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPhotoViewerMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPhotoViewerMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CPhotoViewerMFCView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

void CPhotoViewerMFCView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPhotoViewerMFCView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPhotoViewerMFCView diagnostics

#ifdef _DEBUG
void CPhotoViewerMFCView::AssertValid() const
{
	CFormView::AssertValid();
}

void CPhotoViewerMFCView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CPhotoViewerMFCDoc* CPhotoViewerMFCView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPhotoViewerMFCDoc)));
	return (CPhotoViewerMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CPhotoViewerMFCView message handlers


void CPhotoViewerMFCView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class

	///Added by me
	//CListBox* pList = reinterpret_cast<CListBox*>(GetDlgItem(IDC_LIST1));

	// get a pointer to the device context
	

	//call draw data
	drawData();
}

//void CPhotoViewerMFCView::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: Add your message handler code here and/or call default
//	if (nIDEvent == 1)
//	{
//		// tell windows the view needs redrawn 
//		// note: the last parameter is the erase flag. 
//		// if it is TRUE, things will flicker like crazy. 
//		drawData();
//	}
//	CFormView::OnTimer(nIDEvent);
//}


void CPhotoViewerMFCView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	Invalidate(false);
}

void CPhotoViewerMFCView::OnSizing(UINT fwSide, LPRECT pRect)
{
	CFormView::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
	Invalidate(false);
}

void CPhotoViewerMFCView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CFormView::OnPaint() for painting messages

	//TODO: draw only if image is in invalidated rect
	CRect rcUpdate = dc.m_ps.rcPaint;
	if (rcUpdate.TopLeft().x > 500 || rcUpdate.TopLeft().y > 500)
		return;

	char test[100];
	sprintf_s(test, "%d %d", rcUpdate.TopLeft().x, rcUpdate.TopLeft().y);
	//MessageBox("Test", test, MB_OK);
	
	//drawData();
}

bool CPhotoViewerMFCView::drawData()
{
	CDC* pDC = GetDC();
	CPhotoViewerMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_pBmp == NULL)
		return false;
	Gdiplus::Graphics graphics(*pDC);

	//graphics.DrawImage(bmp,50, 50,bmp->GetWidth(),bmp->GetHeight());
	graphics.DrawImage(pDoc->m_pBmp, 0, 0, 500, 500);

	return true;
}



