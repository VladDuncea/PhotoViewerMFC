
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
	ON_COMMAND(IDM_VIEW_ANIMATION, &CPhotoViewerMFCView::OnViewAnimation)
	ON_WM_TIMER()
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

	// get a pointer to the document class 
	CDC* pDC = GetDC();
	CPhotoViewerMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// make pens for solid lines of thickness 2 
	CPen RedPen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen BluePen(PS_SOLID, 2, RGB(0, 0, 255));

	//Change pen to red
	CPen* pOldPen = pDC->SelectObject(&RedPen);

	// get the total number of data points 
	int N = pDoc->m_BugDataArray.GetSize();

	// draw all of the connecting lines 
	for (int i = 0; i < pDoc->m_nBugPosition - 1; i++)
	{
		pDC->MoveTo(pDoc->m_BugDataArray[i].x, pDoc->m_BugDataArray[i].y);
		pDC->LineTo(pDoc->m_BugDataArray[i + 1].x, pDoc->m_BugDataArray[i + 1].y);
	}

	//Change pen
	pDC->SelectObject(&BluePen);

	// start drawing non animated tracks, but need to check for a 
	// valid starting postion 
	int start = pDoc->m_nBugPosition;
	if (start < 0) 
		start = 0;
	for (int i = start; i < N - 2; i++)
	{
		pDC->MoveTo(pDoc->m_BugDataArray[i].x, pDoc->m_BugDataArray[i].y);
		pDC->LineTo(pDoc->m_BugDataArray[i + 1].x, pDoc->m_BugDataArray[i + 1].y);
	}

	// deselect pens and delete them 
	pDC->SelectObject(pOldPen);
	RedPen.DeleteObject();
	BluePen.DeleteObject();

	// move to next position or quit animating 
	if (pDoc->m_nBugPosition != -1) pDoc->m_nBugPosition++;
	if (pDoc->m_nBugPosition >= N)
	{
		pDoc->m_nBugPosition = -1;

		// stop timer 1 
		KillTimer(1);

		// redraw and erase so all lines are in initial state (blue) 
		OnUpdate(NULL, NULL,NULL);
	}
}


void CPhotoViewerMFCView::OnViewAnimation()
{
	// TODO: Add your command handler code here
	// get the document 
	CPhotoViewerMFCDoc* pDoc = GetDocument();

	// set the position to the first data point 
	pDoc->m_nBugPosition = 0;

	// create a timer with id=1 and delay of 200 milliseconds 
	SetTimer(1, 200, NULL);
}


void CPhotoViewerMFCView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		// tell windows the view needs redrawn 
		// note: the last parameter is the erase flag. 
		// if it is TRUE, things will flicker like crazy. 
		OnUpdate(NULL,NULL,NULL);
	}
	CFormView::OnTimer(nIDEvent);
}
