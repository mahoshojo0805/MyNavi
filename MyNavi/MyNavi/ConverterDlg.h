#pragma once
#include"CConverter.h"

// ConverterDlg 对话框

class ConverterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ConverterDlg)

public:
	ConverterDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ConverterDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_Converter };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strFolderpath;
	CString m_strFilepath;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnEnChangeEdit2();
	CString m_strHeaderInfo;
};
