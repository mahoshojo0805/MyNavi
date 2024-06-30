// KitAllanDlg.cpp: 实现文件
//

#include "pch.h"
#include "MyNavi.h"
#include "KitAllanDlg.h"
#include "afxdialogex.h"


// KitAllanDlg 对话框

IMPLEMENT_DYNAMIC(KitAllanDlg, CDialogEx)

KitAllanDlg::KitAllanDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_strFilepath(_T(""))
{

}

KitAllanDlg::~KitAllanDlg()
{
}

void KitAllanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strFilepath);
}


BEGIN_MESSAGE_MAP(KitAllanDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &KitAllanDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &KitAllanDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// KitAllanDlg 消息处理程序


void KitAllanDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog IMUFileDlg(true, NULL, NULL, 0, _T("ASC文件|*.asc"), this);
	if (IMUFileDlg.DoModal() == IDOK)
	{
		UpdateData(true);
		m_strFilepath = IMUFileDlg.GetPathName();
		m_strpath = IMUFileDlg.GetFolderPath();
		UpdateData(false);
	}
}


void KitAllanDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	string buf1 = CT2A(m_strFilepath.GetString());
	string buf2 = CT2A(m_strpath.GetString());
	reader.Openfile(buf1.c_str());

	FILE* gyox, *gyoy, *gyoz;
	FILE* accx, *accy, *accz;
	fopen_s(&gyox, (buf2 + "\\GYO_X.txt").c_str(), "w");
	fopen_s(&gyoy, (buf2 + "\\GYO_Y.txt").c_str(), "w");
	fopen_s(&gyoz, (buf2 + "\\GYO_Z.txt").c_str(), "w");
	fopen_s(&accx, (buf2 + "\\ACC_X.txt").c_str(), "w");
	fopen_s(&accy, (buf2 + "\\ACC_Y.txt").c_str(), "w");
	fopen_s(&accz, (buf2 + "\\ACC_Z.txt").c_str(), "w");
	FILE* time;
	fopen_s(&time, (buf2 + "\\time.txt").c_str(),"w");
	IMUData imudata;
	while (reader.Next(imudata))
	{
		if (imudata.time.sow == 0)break;
		fprintf(gyox, "%lf\n", rad2deg(imudata.wx));
		fprintf(gyoy, "%lf\n", rad2deg(imudata.wy));
		fprintf(gyoz, "%lf\n", rad2deg(imudata.wz));

		fprintf(accx, "%lf\n", imudata.fx);
		fprintf(accy, "%lf\n", imudata.fy);
		fprintf(accz, "%lf\n", imudata.fz);

		fprintf(time, "%lf\n", imudata.time.sow);
	}
	fclose(gyox); fclose(gyoy); fclose(gyoz);
	fclose(accx); fclose(accy); fclose(accz);
	fclose(time);
	MessageBox(_T("输出成功！"));
}
