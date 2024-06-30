// BINDecodeE.cpp: 实现文件
//

#include "pch.h"
#include "MyNavi.h"
#include "BINDecodeE.h"
#include "afxdialogex.h"


// BINDecodeE 对话框

IMPLEMENT_DYNAMIC(BINDecodeE, CDialogEx)

BINDecodeE::BINDecodeE(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_strPath(_T(""))
{

}

BINDecodeE::~BINDecodeE()
{
}

void BINDecodeE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strPath);
}


BEGIN_MESSAGE_MAP(BINDecodeE, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &BINDecodeE::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &BINDecodeE::OnBnClickedButton1)
END_MESSAGE_MAP()


// BINDecodeE 消息处理程序


void BINDecodeE::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog IMUFileDlg(true, NULL, NULL, 0, _T("LOG文件|*.log|BIN文件|*.bin|DAT文件|*.dat"), this);
	if (IMUFileDlg.DoModal() == IDOK)
	{
		UpdateData(true);
		m_strPath = IMUFileDlg.GetPathName();
		m_strFolderPath = IMUFileDlg.GetFolderPath();
		UpdateData(false);
	}
}


void BINDecodeE::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CConverter cvter;
	string info;
	string path = CT2A(m_strPath.GetString());
	string folder = CT2A(m_strFolderPath.GetString());
	cvter.ConvertBIN(path, folder, &info);
}
