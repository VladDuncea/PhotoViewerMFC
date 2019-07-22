
// PhotoViewerMFCView.cpp : implementation of the CPhotoViewerMFCView class
//

#include "pch.h"
#include "framework.h"
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
