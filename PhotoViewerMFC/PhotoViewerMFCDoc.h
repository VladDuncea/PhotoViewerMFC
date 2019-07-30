
// PhotoViewerMFCDoc.h : interface of the CPhotoViewerMFCDoc class
//


#pragma once
#include <afxtempl.h>


class CPhotoViewerMFCDoc : public CDocument
{
protected: // create from serialization only
	CPhotoViewerMFCDoc() noexcept;
	DECLARE_DYNCREATE(CPhotoViewerMFCDoc)

// Attributes
public:
	
// Operations
public:
	bool rotateImage(void);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CPhotoViewerMFCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	Gdiplus::Bitmap* m_pBmp;
	CFile* m_pFile;
};
