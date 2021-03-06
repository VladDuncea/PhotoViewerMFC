
// PhotoViewerMFCDoc.cpp : implementation of the CPhotoViewerMFCDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PhotoViewerMFC.h"
#endif

#include "PhotoViewerMFCDoc.h"
#include "MainFrm.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPhotoViewerMFCDoc

IMPLEMENT_DYNCREATE(CPhotoViewerMFCDoc, CDocument)

BEGIN_MESSAGE_MAP(CPhotoViewerMFCDoc, CDocument)
END_MESSAGE_MAP()


// CPhotoViewerMFCDoc construction/destruction

CPhotoViewerMFCDoc::CPhotoViewerMFCDoc() noexcept
{
	// TODO: add one-time construction code here
	m_pBmp = NULL;
	m_pCBmp = NULL;
	m_pFile = NULL;
}

CPhotoViewerMFCDoc::~CPhotoViewerMFCDoc()
{
	delete(m_pBmp);
	delete(m_pCBmp);
}

bool CPhotoViewerMFCDoc::rotateImage(void)
{	
	if (m_pBmp == NULL)
	{
		OutputDebugString("Nu trebuia sa poata sa roteasca poza!");
		return false;
	}
	m_pBmp->RotateFlip(Gdiplus::RotateFlipType::Rotate90FlipNone);
	//TODO:error handlng
	return true;
}

bool CPhotoViewerMFCDoc::createCachedBmp(CDC* pDc)
{
	//Create a cached BMP
	Gdiplus::Graphics graphics(*pDc);
	//Clear old cached BMP
	if (m_pCBmp)
		delete(m_pCBmp);
	//Create the new cached BMP
	m_pCBmp = new Gdiplus::CachedBitmap(m_pBmp, &graphics);

	//TODO: error handling
	return true;
}

BOOL CPhotoViewerMFCDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here

	//TODO: is Cfile self deleting ?

	return TRUE;
}


// CPhotoViewerMFCDoc serialization

void CPhotoViewerMFCDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		//Get the file info
		m_pFile = ar.GetFile();
		OutputDebugString( m_pFile->GetFileName());

		//Delete old BMP
		if (m_pBmp)
			delete m_pBmp;
		//Open the new BMP
		m_pBmp = new Gdiplus::Bitmap((CStringW)(m_pFile->GetFilePath()));
	
		//Get main window 
		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		//Update status bar
		CString strStatus;
		pMain->ChangeStatusText(m_pFile->GetFileName());
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CPhotoViewerMFCDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CPhotoViewerMFCDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CPhotoViewerMFCDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CPhotoViewerMFCDoc diagnostics

#ifdef _DEBUG
void CPhotoViewerMFCDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPhotoViewerMFCDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG