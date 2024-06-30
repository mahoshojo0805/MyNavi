
// MyNaviDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MyNavi.h"
#include "MyNaviDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyNaviDlg 对话框



CMyNaviDlg::CMyNaviDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYNAVI_DIALOG, pParent)
	, m_strIMUFilePath(_T(""))
	, m_dPosX(0)
	, m_dPosY(0)
	, m_dPosZ(0)
	, m_dAttX(0)
	, m_dAttY(0)
	, m_dAttZ(0)
	, m_week(0)
	, m_dSow(0)
	, m_strGnssFilepath(_T(""))
	, m_dARW(0)
	, m_dVRW(0)
	, m_dLeverX(0)
	, m_dLeverY(0)
	, m_dLeverZ(0)
	, m_dRotAngleX(0)
	, m_dRotAngleY(0)
	, m_dRotAngleZ(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyNaviDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strIMUFilePath);
	DDX_Text(pDX, IDC_EDIT2, m_dPosX);
	DDX_Text(pDX, IDC_EDIT3, m_dPosY);
	DDX_Text(pDX, IDC_EDIT4, m_dPosZ);
	DDX_Text(pDX, IDC_EDIT5, m_dAttX);
	DDX_Text(pDX, IDC_EDIT6, m_dAttY);
	DDX_Text(pDX, IDC_EDIT7, m_dAttZ);
	DDX_Text(pDX, IDC_EDIT8, m_week);
	DDX_Text(pDX, IDC_EDIT9, m_dSow);
	DDX_Text(pDX, IDC_EDIT13, m_strGnssFilepath);
	DDX_Text(pDX, IDC_EDIT14, m_dARW);
	DDX_Text(pDX, IDC_EDIT15, m_dVRW);
	DDX_Text(pDX, IDC_EDIT16, m_dLeverX);
	DDX_Text(pDX, IDC_EDIT17, m_dLeverY);
	DDX_Text(pDX, IDC_EDIT18, m_dLeverZ);
	DDX_Text(pDX, IDC_EDIT10, m_dRotAngleX);
	DDX_Text(pDX, IDC_EDIT11, m_dRotAngleY);
	DDX_Text(pDX, IDC_EDIT12, m_dRotAngleZ);
}

BEGIN_MESSAGE_MAP(CMyNaviDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTONOPENIMUFILE, &CMyNaviDlg::OnBnClickedButtonopenimufile)
	ON_BN_CLICKED(IDC_RADIO2, &CMyNaviDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CMyNaviDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON_IMU, &CMyNaviDlg::OnBnClickedButtonImu)
	ON_BN_CLICKED(IDC_CHECK2, &CMyNaviDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyNaviDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyNaviDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyNaviDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyNaviDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMyNaviDlg::OnBnClickedButton6)
END_MESSAGE_MAP()

// CMyNaviDlg 消息处理程序

BOOL CMyNaviDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);//默认选中自动初始对准
	ReadConfig();
	//启用控制台调试
	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyNaviDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyNaviDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyNaviDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyNaviDlg::OnBnClickedButtonopenimufile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog IMUFileDlg(true, NULL, NULL, 0, _T("ASC文件|*.asc"), this);
	if (IMUFileDlg.DoModal() == IDOK)
	{
		UpdateData(true);
		m_strIMUFilePath = IMUFileDlg.GetPathName();
		//m_strFormat = IMUFileDlg.GetFileExt();
		UpdateData(false);
	}
}


void CMyNaviDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit*)GetDlgItem(IDC_EDIT5))->EnableWindow(true);
	((CEdit*)GetDlgItem(IDC_EDIT6))->EnableWindow(true);
	((CEdit*)GetDlgItem(IDC_EDIT7))->EnableWindow(true);
	UpdateData(false);
}


void CMyNaviDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	((CEdit*)GetDlgItem(IDC_EDIT5))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT6))->EnableWindow(false);
	((CEdit*)GetDlgItem(IDC_EDIT7))->EnableWindow(false);
	UpdateData(false);
}

void CMyNaviDlg::WriteConfig()
{
	FILE* fcfg;
	fopen_s(&fcfg, "IMU.config", "w+");
	string buf1 = CT2A(m_strIMUFilePath.GetString());
	string buf2 = CT2A(m_strGnssFilepath.GetString());
	fprintf(fcfg, "IMU_FILEPATH$:%s\n", buf1.c_str());
	fprintf(fcfg, "GNSS_FILEPATH$:%s\n", buf2.c_str());
	fprintf(fcfg, "INIT_POS$:%lf,%lf,%lf\n", m_dPosX, m_dPosY, m_dPosZ);
	if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())
		fprintf(fcfg, "INIT_ATT$:AUTO,\n");
	else
		fprintf(fcfg, "INIT_ATT$:INPUT,%lf,%lf,%lf\n", m_dAttX, m_dAttY, m_dAttZ);
	if (!((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
		fprintf(fcfg, "INIT_TIME$:AUTO,\n");
	else
		fprintf(fcfg, "INIT_TIME$:INPUT,%d,%lf\n", m_week, m_dSow);
	fprintf(fcfg, "ARW_AND_VRW$:%lf,%lf\n", m_dARW, m_dVRW);
	fprintf(fcfg, "LEVER$:%lf,%lf,%lf\n", m_dLeverX, m_dLeverY, m_dLeverZ);
	fprintf(fcfg, "ROTATION_ANGLE$:%lf,%lf,%lf\n", m_dRotAngleX, m_dRotAngleY, m_dRotAngleZ);
	fclose(fcfg);
}
void CMyNaviDlg::ReadConfig()
{
	FILE* fcfg;
	fopen_s(&fcfg, "IMU.config", "r");
	if (fcfg != NULL) {
		char buffer[512];
		while (feof(fcfg) == 0) {
			fgets(buffer, 512, fcfg);
			string line = buffer;
			int idx_c = line.find("$:");
			if (idx_c == string::npos)continue;
			string head = line.substr(0, idx_c);
			string body = line.substr(idx_c + 2, line.size() - idx_c);
			if (head.compare("IMU_FILEPATH") == 0)
				m_strIMUFilePath = body.substr(0, body.size() - 1).c_str();
			else if (head.compare("GNSS_FILEPATH") == 0)
				m_strGnssFilepath = body.substr(0, body.size() - 1).c_str();
			else if (head.compare("INIT_POS") == 0)
			{
				int l = 0; int r = 0;
				string str;
				r = body.find(',', l);
				str = body.substr(l, r - l);
				m_dPosX = atof(str.c_str());

				l = r + 1;
				r = body.find(',', l);
				str = body.substr(l, r - l);
				m_dPosY = atof(str.c_str());

				l = r + 1;
				r = body.size() - 1;
				str = body.substr(l, r - l);
				m_dPosZ = atof(str.c_str());
			}
			else if (head.compare("INIT_ATT") == 0)
			{
				int l = 0; int r = 0;
				string str;
				r = body.find(',', l);
				str = body.substr(l, r - l);
				if (str.compare("AUTO") == 0)
					continue;
				else
				{
					l = r + 1;
					r = body.find(',', l);
					str = body.substr(l, r - l);
					m_dAttX = atof(str.c_str());

					l = r + 1;
					r = body.find(',', l);
					str = body.substr(l, r - l);
					m_dAttY = atof(str.c_str());

					l = r + 1;
					r = body.size();
					str = body.substr(l, r - l);
					m_dAttZ = atof(str.c_str());
				}
			}
			else if (head.compare("INIT_TIME") == 0)
			{
				int l = 0; int r = 0;
				string str;
				r = body.find(',', l);
				str = body.substr(l, r - l);
				if (str.compare("AUTO") == 0)
					continue;
				else
				{
					l = r + 1;
					r = body.find(',', l);
					str = body.substr(l, r - l);
					m_week = atoi(str.c_str());

					l = r + 1;
					r = body.find(',', l);
					str = body.substr(l, r - l);
					m_dSow = atof(str.c_str());
				}
			}
			else if (head.compare("ARW_AND_VRW") == 0) {
				int l = 0; int r = 0;
				string str;
				r = body.find(',', l);
				str = body.substr(l, r - l);
				m_dARW = atof(str.c_str());
				str = body.substr(r + 1, body.size() - r);
				m_dVRW = atof(str.c_str());
			}
			else if (head.compare("LEVER") == 0)
			{
				int l = 0; int r = 0;
				string str;
				r = body.find(',', l);
				str = body.substr(l, r - l);
				m_dLeverX = atof(str.c_str());

				l = r + 1;
				r = body.find(',', l);
				str = body.substr(l, r - l);
				m_dLeverY = atof(str.c_str());

				str = body.substr(r + 1, body.size() - r);
				m_dLeverZ = atof(str.c_str());
			}
			else if (head.compare("ROTATION_ANGLE") == 0) {
				int l = 0; int r = 0;
				string str;
				r = body.find(',', l);
				str = body.substr(l, r - l);
				m_dRotAngleX = atof(str.c_str());

				l = r + 1;
				r = body.find(',', l);
				str = body.substr(l, r - l);
				m_dRotAngleY = atof(str.c_str());

				str = body.substr(r + 1, body.size() - r);
				m_dRotAngleZ = atof(str.c_str());
			}
		}
		fclose(fcfg);
		UpdateData(false);
	}
}
void CMyNaviDlg::OnBnClickedButtonImu()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	//写入配置文件
	WriteConfig();
	MessageBox(_T("即将进行纯惯导解算...\n程序可能卡顿！"));
	//运行惯导解算
	CLooseCouplingManager LCManager;
	LCManager.SetImuFilePath(CT2A(m_strIMUFilePath.GetString()));
	LCManager.SetInitPos(m_dPosX, m_dPosY, m_dPosZ);
	if (!((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())
		LCManager.SetInitAtt(m_dAttX, m_dAttY, m_dAttZ);
	if (((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
		LCManager.SetStartTime(m_week, m_dSow);
	LCManager.RunPureIns();
	LCManager.WriterResultInFile("res.txt");
	LCManager.Clear();
	MessageBox(_T("解算完成"));
}


void CMyNaviDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	bool flag = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	(CButton*)GetDlgItem(IDC_EDIT8)->EnableWindow(flag);
	(CButton*)GetDlgItem(IDC_EDIT9)->EnableWindow(flag);
}


void CMyNaviDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	KitAllanDlg allandlg;
	allandlg.DoModal();
}


void CMyNaviDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	ConverterDlg dlg;
	dlg.DoModal();
}


void CMyNaviDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog GNSSFileDlg(true, NULL, NULL, 0, _T("POS文件|*.pos"), this);
	if (GNSSFileDlg.DoModal() == IDOK)
	{
		UpdateData(true);
		m_strGnssFilepath = GNSSFileDlg.GetPathName();
		//m_strFormat = IMUFileDlg.GetFileExt();
		UpdateData(false);
	}
}


void CMyNaviDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	//写入配置文件
	WriteConfig();
	MessageBox(_T("即将进行松组合解算...\n程序可能卡顿！"));
	CLooseCouplingManager LCManager;
	LCManager.SetImuFilePath(CT2A(m_strIMUFilePath.GetString()));
	LCManager.SetGnssFilePath(CT2A(m_strGnssFilepath.GetString()));
	LCManager.SetInitPos(m_dPosX, m_dPosY, m_dPosZ);
	LCManager.SetLever(m_dLeverX, m_dLeverY, m_dLeverZ);
	LCManager.SetRandomWalk(m_dARW, m_dVRW);
	double p[3]{ 100,100,100 }, v[3]{ 1,1,1 }, a[3]{ 0.01,0.01,0.01 };
	LCManager.SetStdData(p, v, a);
	if (!((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())
		LCManager.SetInitAtt(m_dAttX, m_dAttY, m_dAttZ);
	if (((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
		LCManager.SetStartTime(m_week, m_dSow);
	LCManager.RunLC();
	LCManager.WriterResultInFile("LCres.txt");
	LCManager.Clear();
	MessageBox(_T("解算完成！"));
}


void CMyNaviDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	BINDecodeE dlg;
	dlg.DoModal();
}
