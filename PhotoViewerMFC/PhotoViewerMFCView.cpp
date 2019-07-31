
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
	ON_COMMAND(ID_EDIT_ROTATE, &CPhotoViewerMFCView::OnEditRotate)
	ON_WM_ERASEBKGND()
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


void CPhotoViewerMFCView::OnUpdate(CView* pSender, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class

	///Added by me
	//CListBox* pList = reinterpret_cast<CListBox*>(GetDlgItem(IDC_LIST1));

	// get a pointer to the device context


	//call draw data
	///CRect rect;
	///pSender->GetClientRect(rect);
	drawData(nullptr);
}

void CPhotoViewerMFCView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	/*SetScrollRange(SB_VERT, 0, 1000);
	CFormView* cformview = reinterpret_cast<CFormView*>(GetDlgItem(IDD_PHOTOVIEWERMFC_FORM));
	int mapMode;
	SIZE s1, s2, s3;
	cformview->GetDeviceScrollSizes(mapMode, s1, s2, s3);
	CString ddebug;
	ddebug.Format("%ld %ld | %ld %ld | %ld %ld \n", s1.cx, s1.cy, s2.cx, s2.cy, s3.cx, s3.cy);*/
	OutputDebugString("Test\n");
}

void CPhotoViewerMFCView::OnSizing(UINT fwSide, LPRECT pRect)
{
	CFormView::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
	
}

void CPhotoViewerMFCView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CFormView::OnPaint() for painting messages

	//TODO: draw only if image is fully invalidated
	CRect rcUpdate = dc.m_ps.rcPaint;
	CRect rect;
	GetClientRect(rect);

	

	//Draw only on a full invalidation
	if (rcUpdate.TopLeft().x == 0 && rcUpdate.TopLeft().y == 0 && rcUpdate.BottomRight().x == rect.BottomRight().x && rcUpdate.BottomRight().y == rect.BottomRight().y)
	{
		drawData(NULL);
	}
	else
	{
		drawData(&rcUpdate);

	}
		
	
	

}

bool CPhotoViewerMFCView::drawData(const CRect *invalidRect)
{
	CDC* pDC = GetDC();
	CPhotoViewerMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//Check if a image is loaded
	if (pDoc->m_pBmp == NULL)
		return false;

	Gdiplus::Graphics graphics(*pDC);
	

	if (invalidRect)
	{
		//Get client rect
		CRect clientRect;
		GetClientRect(clientRect);

		//Background deletion is off so handle that here
		Gdiplus::SolidBrush background(Gdiplus::Color(255, 255, 255, 255));
		Gdiplus::Rect rects[2];
		if (pDoc->m_pBmp->GetWidth() > clientRect.Width())
			rects[1].X = pDoc->m_pBmp->GetWidth();

		graphics.FillRectangles(,);
		/*
		CString debug;
		//debug.Format("%ld %ld\n", invalidRect->TopLeft().x, invalidRect->TopLeft().y);
		//OutputDebugString(debug);

		//Get x and y of invalidated rect
		unsigned int x = invalidRect->TopLeft().x;
		unsigned int y = invalidRect->TopLeft().y;

		//Check if the invalidated rect is outside the image
		if (x > pDoc->m_pBmp->GetWidth() || y > pDoc->m_pBmp->GetHeight())
			return true;

		//Get clinet rect
		CRect clientRect;
		GetClientRect(clientRect);
		//If the picture is bigger than the view paint only the visible part
		unsigned int width = pDoc->m_pBmp->GetWidth() > clientRect.Width() ? clientRect.Width(): pDoc->m_pBmp->GetWidth();
		unsigned int height = pDoc->m_pBmp->GetHeight() > clientRect.Height()? clientRect.Height() : pDoc->m_pBmp->GetHeight();
		debug.Format("%d %d %d %d\n", x, y,width-x,height-y);
		OutputDebugString(debug);

		//Create a rect to use on copying the base image
		Gdiplus::Rect rect(x, y, width-x,height -y);
		//Clone the part that we need
		Gdiplus::Bitmap* partial = pDoc->m_pBmp->Clone(rect, PixelFormatDontCare);
		//Handle possible errors
		if (partial == NULL)
		{
			OutputDebugString("Error on bmp clone\n");
			return false;
		}
		//Draw the needed part
		//graphics.DrawImage(partial, invalidRect->TopLeft().x, invalidRect->TopLeft().y, partial->GetWidth(), partial->GetHeight());
		delete(partial);*/
	}
	else
	{
		graphics.DrawImage(pDoc->m_pBmp, 0, 0, pDoc->m_pBmp->GetWidth(), pDoc->m_pBmp->GetHeight());
	}
	
	return true;
}





void CPhotoViewerMFCView::OnEditRotate()
{
	CPhotoViewerMFCDoc* pDoc = GetDocument();

	pDoc->rotateImage();
	drawData(nullptr);
}


BOOL CPhotoViewerMFCView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	///Supress background deletion
	return CFormView::OnEraseBkgnd(pDC);
	return false;
}
