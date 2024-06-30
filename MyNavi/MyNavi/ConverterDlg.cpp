// ConverterDlg.cpp: 实现文件
//

#include "pch.h"
#include "MyNavi.h"
#include "ConverterDlg.h"
#include "afxdialogex.h"


// ConverterDlg 对话框

IMPLEMENT_DYNAMIC(ConverterDlg, CDialogEx)

ConverterDlg::ConverterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_Converter, pParent)
	, m_strFilepath(_T(""))
	, m_strHeaderInfo(_T(""))
{

}

ConverterDlg::~ConverterDlg()
{
}

void ConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFilepath);
	DDX_Text(pDX, IDC_EDIT2, m_strHeaderInfo);
}


BEGIN_MESSAGE_MAP(ConverterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ConverterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ConverterDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT2, &ConverterDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// ConverterDlg 消息处理程序


void ConverterDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog IMUFileDlg(true, NULL, NULL, 0, _T("IMR文件|*.imr"), this);
	if (IMUFileDlg.DoModal() == IDOK)
	{
		UpdateData(true);
		m_strFilepath = IMUFileDlg.GetPathName();
		m_strFolderpath = IMUFileDlg.GetFolderPath();
		UpdateData(false);
	}
}


void ConverterDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CConverter cvter;
	string filepath = CT2A(m_strFilepath.GetString());
	string folderpath = CT2A(m_strFolderpath.GetString());
	string header_info;
	cvter.ConvertIMR(filepath, folderpath, &header_info);
	m_strHeaderInfo = header_info.c_str();
	UpdateData(false);
}


void ConverterDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
