
// MyNaviDlg.h: 头文件
//

#pragma once
#include"KitAllanDlg.h"
#include"ConverterDlg.h"
#include"BINDecodeE.h"
#include"CLooseCouplingManager.h"

// CMyNaviDlg 对话框
class CMyNaviDlg : public CDialogEx
{
// 构造
public:
	CMyNaviDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYNAVI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	void WriteConfig();
	void ReadConfig();
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 文件后缀
	//CString m_strFormat;
	// IMU文件路径
	CString m_strIMUFilePath;
	afx_msg void OnBnClickedButtonopenimufile();
	double m_dPosX;
	double m_dPosY;
	double m_dPosZ;
	double m_dAttX;
	double m_dAttY;
	double m_dAttZ;
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedButtonImu();
	int m_week;
	double m_dSow;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	CString m_strGnssFilepath;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	double m_dARW;
	double m_dVRW;
	double m_dLeverX;
	double m_dLeverY;
	double m_dLeverZ;
	double m_dRotAngleX;
	double m_dRotAngleY;
	double m_dRotAngleZ;
	afx_msg void OnBnClickedButton6();
};
