
// PhotoViewerMFCView.h : interface of the CPhotoViewerMFCView class
//

#pragma once


class CPhotoViewerMFCView : public CFormView
{
protected: // create from serialization only
	CPhotoViewerMFCView() noexcept;
	DECLARE_DYNCREATE(CPhotoViewerMFCView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_PHOTOVIEWERMFC_FORM };
#endif

// Attributes
public:
	CPhotoViewerMFCDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CPhotoViewerMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PhotoViewerMFCView.cpp
inline CPhotoViewerMFCDoc* CPhotoViewerMFCView::GetDocument() const
   { return reinterpret_cast<CPhotoViewerMFCDoc*>(m_pDocument); }
#endif
