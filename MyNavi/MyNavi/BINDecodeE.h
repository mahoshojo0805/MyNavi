#pragma once
#include"CConverter.h"

// BINDecodeE 对话框

class BINDecodeE : public CDialogEx
{
	DECLARE_DYNAMIC(BINDecodeE)

public:
	BINDecodeE(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~BINDecodeE();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPath;
	CString m_strFolderPath;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
