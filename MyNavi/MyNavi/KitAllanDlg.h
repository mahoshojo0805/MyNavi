#pragma once
#include"BaseIMUReader.h"

// KitAllanDlg 对话框

class KitAllanDlg : public CDialogEx
{
	DECLARE_DYNAMIC(KitAllanDlg)

public:
	KitAllanDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~KitAllanDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BaseIMUReader reader;
	CString m_strpath;
	CString m_strFilepath;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
