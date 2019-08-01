
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
//	ON_WM_SIZING()
	ON_COMMAND(ID_EDIT_ROTATE, &CPhotoViewerMFCView::OnEditRotate)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CPhotoViewerMFCView construction/destruction

CPhotoViewerMFCView::CPhotoViewerMFCView() noexcept
	: CFormView(IDD_PHOTOVIEWERMFC_FORM)
{
	// TODO: add construction code here
	m_pMemDC = NULL;
	m_gr = NULL;
	m_pbmpMemBitmap = NULL;
}

CPhotoViewerMFCView::~CPhotoViewerMFCView()
{
	if (m_pMemDC)
	{
		delete m_pMemDC; m_pMemDC = NULL;
	}
	if (m_pbmpMemBitmap)
	{
		delete m_pbmpMemBitmap; m_pbmpMemBitmap = NULL;
	}
	if (m_gr)
	{
		delete m_gr; m_gr = NULL;
	}
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
	///Added by me
	//CListBox* pList = reinterpret_cast<CListBox*>(GetDlgItem(IDC_LIST1));

	//Create a cached BMP
	CPhotoViewerMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->m_pBmp == NULL)
		return;

	CDC* pDesktopDC = GetDC();
	pDoc->createCachedBmp(pDesktopDC);
	CRect clientRect;
	GetClientRect(clientRect);

	///Using memory DC
	CreateScreenBuffer(clientRect.Size(), pDesktopDC);

	//Update Scroll Bars
	UpdateScrollSize(clientRect.Size().cx, clientRect.Size().cy);

}

void CPhotoViewerMFCView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	//Update Scroll Size
	UpdateScrollSize(cx, cy);

	//Update Screen Buffer
	CreateScreenBuffer({ cx,cy }, GetDC());
}

void CPhotoViewerMFCView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CFormView::OnPaint() for painting messages

	//TODO: draw only if image is fully invalidated
	CRect rcUpdate = dc.m_ps.rcPaint;

	CDC* pDC = GetDC();

	if (m_pMemDC == NULL)
		return;

	pDC->BitBlt(rcUpdate.left, rcUpdate.top, rcUpdate.Width(), rcUpdate.Height(),
		m_pMemDC, rcUpdate.left, rcUpdate.top, SRCCOPY);
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
		//graphics.DrawCachedBitmap(pDoc->m_pCBmp, 0, 0);

		//graphics.DrawImage(pDoc->m_pBmp, 0, 0, pDoc->m_pBmp->GetWidth(), pDoc->m_pBmp->GetHeight());
		/*CString debug;
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
		//graphics.DrawImage(pDoc->m_pBmp, 0, 0, pDoc->m_pBmp->GetWidth(), pDoc->m_pBmp->GetHeight());
		//graphics.DrawCachedBitmap(pDoc->m_pCBmp, 0, 0);
	}
	
	return true;
}

void CPhotoViewerMFCView::OnEditRotate()
{
	CPhotoViewerMFCDoc* pDoc = GetDocument();
	if (pDoc->m_pBmp == NULL)
	{
		MessageBox("Invalid option!", "Error",MB_OK);
		return;
	}
		
	pDoc->rotateImage();
	pDoc->createCachedBmp(GetDC());
	UpdateScreenBuffer();
	drawData(nullptr);
}

BOOL CPhotoViewerMFCView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	///Supress background deletion

	//return CFormView::OnEraseBkgnd(pDC);

	//If the window gets bigger and the picture is smaller than the window
	//Delete the background 
	//Get client rect
	CRect clientRect;
	GetClientRect(clientRect);

	//Background deletion is off so handle that here
	Gdiplus::SolidBrush background(Gdiplus::Color(255, 255, 255, 255));
	Gdiplus::Rect rects[2];
	/*if (pDoc->m_pBmp->GetWidth() > clientRect.Width())
		rects[1].X = pDoc->m_pBmp->GetWidth();

	graphics.FillRectangles(, );*/
	return false;
}

void CPhotoViewerMFCView::CreateScreenBuffer(const CSize szPanel, CDC* pDesktopDC)
{
	if (m_pMemDC)
	{
		delete m_gr;
		m_gr = NULL;
		delete m_pMemDC;
		m_pMemDC = NULL;
		delete m_pbmpMemBitmap;
		m_pbmpMemBitmap = NULL;
	}

	m_pMemDC = new CDC;
	m_pMemDC->CreateCompatibleDC(pDesktopDC);
	// Create a new bitmap
	m_pbmpMemBitmap = new CBitmap;
	// Create the new bitmap
	m_pbmpMemBitmap->CreateCompatibleBitmap(pDesktopDC, szPanel.cx, szPanel.cy);
	m_pbmpMemBitmap->SetBitmapDimension(szPanel.cx, szPanel.cy);
	// Select the new bitmap into the memory DC
	m_pMemDC->SelectObject(m_pbmpMemBitmap);
	// Then create a GDI+ Graphics object
	m_gr = Gdiplus::Graphics::FromHDC(m_pMemDC->m_hDC);
	if(m_gr==NULL)
		return;
	//Paint a white color
	Gdiplus::SolidBrush background(Gdiplus::Color(255, 255, 255, 255));
	m_gr->FillRectangle(&background, 0, 0, szPanel.cx, szPanel.cy);
	UpdateScreenBuffer();
}

void CPhotoViewerMFCView::UpdateScreenBuffer(CSize scrollOffset)
{
	//Verify to have a valid MemDC
	if (m_pMemDC == NULL)
		return;
	
	CPhotoViewerMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//Check for a valid Cached Bitmap
	if (pDoc->m_pCBmp == NULL)
	{
		return;
	}
	
	//Draw the cached Bitmap
	m_gr->DrawCachedBitmap(pDoc->m_pCBmp, -GetScrollPosition().x, -GetScrollPosition().y);

	//Invalidate the screen so it gets redrawn
	CRect crect;
	GetClientRect(crect);
	InvalidateRect(crect);
	OnPaint();
}

void CPhotoViewerMFCView::UpdateScrollSize(int cx, int cy)
{
	CPhotoViewerMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//If the document has an image
	if (pDoc->m_pBmp)
	{
		//Update the scroll-bars
		int width = pDoc->m_pBmp->GetWidth();
		int height = pDoc->m_pBmp->GetHeight();

		//Set scrollbars if needed
		if (cx < width || cy < height)
			SetScrollSizes(MM_TEXT, { width,height });
		else
			SetScrollSizes(MM_TEXT, { 0,0 });
	}
}

BOOL CPhotoViewerMFCView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	// TODO: Add your specialized code here and/or call the base class

	if (bDoScroll)
	{
		OutputDebugString("Scrolling !\n");
		UpdateScreenBuffer(sizeScroll);
	}
	

	return CFormView::OnScrollBy(sizeScroll, bDoScroll);
}
