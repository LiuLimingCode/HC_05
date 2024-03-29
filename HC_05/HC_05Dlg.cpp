// HC_05Dlg.cpp: 实现文件
//

#include "stdafx.h"
#include "HC_05.h"
#include "HC_05Dlg.h"
#include "afxdialogex.h"
#include <ctime>
#include <cstdlib>
#include "ErrorDlg.h"
#include "SetDlg.h"
#include "CMSComm.h"
#include <Dbt.h>

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


// CHC05Dlg 对话框

std::vector<CString> _HC05_ORDERROLE0;
std::vector<CString> _HC05_ORDERROLE1;


CHC05Dlg::CHC05Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HC_05_DIALOG, pParent)
	, m_text1(_T(""))
	, m_text2(_T(""))
	, m_pswd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHC05Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM1, m_ctrlComm1);
	DDX_Control(pDX, IDC_MSCOMM2, m_ctrlComm2);
	DDX_Text(pDX, IDC_EDITTEXT1, m_text1);
	DDX_Text(pDX, IDC_EDITTEXT2, m_text2);
	DDX_Control(pDX, IDC_COMBORATE, m_rate);
	DDX_Text(pDX, IDC_EDITPSWD, m_pswd);
}

BEGIN_MESSAGE_MAP(CHC05Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_DROPDOWN(IDC_COMBO1, &CHC05Dlg::OnCbnDropdownCombo1)
	ON_CBN_DROPDOWN(IDC_COMBO2, &CHC05Dlg::OnCbnDropdownCombo2)
	ON_BN_CLICKED(IDC_BUTTON_OPENCOM, &CHC05Dlg::OnBnClickedButtonOpencom)
	ON_BN_CLICKED(IDC_BUTTON_STARTSET, &CHC05Dlg::OnBnClickedButtonStartset)
	ON_BN_CLICKED(IDC_BUTTONPSWD, &CHC05Dlg::OnBnClickedButtonpswd)
	ON_BN_CLICKED(IDC_BUTTON_TIP, &CHC05Dlg::OnBnClickedButtonTip)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SET, &CHC05Dlg::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_ORGL, &CHC05Dlg::OnBnClickedButtonOrgl)
	ON_EN_KILLFOCUS(IDC_EDITPSWD, &CHC05Dlg::OnEnKillfocusEditpswd)
	ON_WM_DEVICECHANGE()
END_MESSAGE_MAP()


// CHC05Dlg 消息处理程序

BOOL CHC05Dlg::OnInitDialog()
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
	srand((unsigned int)time(NULL));
	for (unsigned int temp = 0; temp < _RATE.size(); ++temp)
		m_rate.AddString(_RATE[temp]);

	CFile cfg_file;
	if (cfg_file.Open(_T("cfg.txt"), CFile::modeRead | CFile::modeNoTruncate | CFile::typeBinary | CFile::shareDenyNone))
	{
		LONGLONG cfg_off = 0; int index = 0, flag = 0; char ch; CString str;
		cfg_file.Seek(cfg_off, CFile::begin);
		while (cfg_file.Read(&ch, 1))
		{
			if (ch == '\r')
				continue;
			else if (ch == '\n')
			{
				*(_setdlg.m_setorder[flag][index]) = str;
				str = _T("");
				index++;
				if (flag == 1 && index == _setdlg.m_setorder1.size())
					break;
				if (flag == 0 && index == _setdlg.m_setorder0.size())
				{ flag = 1; index = 0;}
				continue;
			}
			else
				str += ch;
		}
		cfg_file.Close();
		_HC05_ORDERROLE0.clear();
		for (unsigned int temp = 0, index = 0; temp < _setdlg.m_setorder[0].size(); ++temp)
		{
			if (_setdlg.m_setorder[0][temp]->GetLength() > 0)
			{
				_HC05_ORDERROLE0.push_back(*_setdlg.m_setorder[0][temp]);
				++index;
			}
		}
		_HC05_ORDERROLE1.clear();
		for (unsigned int temp = 0, index = 0; temp < _setdlg.m_setorder[1].size(); ++temp)
		{
			if (_setdlg.m_setorder[1][temp]->GetLength() > 0)
			{
				_HC05_ORDERROLE1.push_back(*_setdlg.m_setorder[1][temp]);
				++index;
			}
		}
	}
	else
	{
		_HC05_ORDERROLE0 = CONST_HC05_ORDERROLE0;
		_HC05_ORDERROLE1 = CONST_HC05_ORDERROLE1;

		for (unsigned int temp = 0; temp < _HC05_ORDERROLE0.size() && temp < _setdlg.m_setorder0.size(); ++temp)
		{
			*(_setdlg.m_setorder0[temp]) = _HC05_ORDERROLE0[temp];
		}
		for (unsigned int temp = 0; temp < _HC05_ORDERROLE1.size() && temp < _setdlg.m_setorder1.size(); ++temp)
		{
			*(_setdlg.m_setorder1[temp]) = _HC05_ORDERROLE1[temp];
		}
	}

	m_rate.SelectString(0, _T("115200"));
	m_pswd.Format(_T("%d%d%d%d"), rand() % 10, rand() % 10, rand() % 10, rand() % 10);

	m_text1 = _T("说明：\r\n    这是一个HC-05蓝牙配对上位机\r\n\r\n串口接线：\r\nGND<->GND\r\nRXD<->TXD\r\nTXD<->RXD\r\nVCC<->5V\r\nEN<->3V3\r\n");
	m_text2 = _T("注意！必须按住蓝牙上的按键再上电方可进入AT模式，才可以进行配对。\r\n\r\n手工配对教程网址：https://blog.csdn.net/m0_37182543/article/details/76383247 \r\n\r\n制作：\r\n    刘力铭  2018/10/24 \r\n    1104257233@qq.com");
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHC05Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHC05Dlg::OnPaint()
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
HCURSOR CHC05Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CHC05Dlg, CDialogEx)
	ON_EVENT(CHC05Dlg, IDC_MSCOMM1, 1, CHC05Dlg::OnCommMscomm1, VTS_NONE)
	ON_EVENT(CHC05Dlg, IDC_MSCOMM2, 1, CHC05Dlg::OnCommMscomm2, VTS_NONE)
END_EVENTSINK_MAP()
void CHC05Dlg::OnCommMscomm1()
{
	// TODO: 在此处添加消息处理程序代码
	VARIANT variant_inp;   //Variant 是一种特殊的数据类型，除了定长String数据及用户定义类型外，可以包含任何种类的数据。
	COleSafeArray safearray_inp;
	LONG len, k;
	BYTE rxdata[2048]; //设置BYTE数组 An 8-bit integer that is not signed.     
	CString strtemp;
	CString _RXData;
	if (m_ctrlComm1.get_CommEvent() == 2) //事件值为2表示接收缓冲区内有字符     
	{
		////////以下你可以根据自己的通信协议加入处理代码 
		_RXData = "";
		variant_inp = m_ctrlComm1.get_Input();		//读缓冲区  
		safearray_inp = variant_inp;				//VARIANT型变量转换为ColeSafeArray型变量
		len = safearray_inp.GetOneDimSize();		//得到有效数据长度        
		for (k = 0; k < len; k++)
			safearray_inp.GetElement(&k, rxdata + k);//转换为BYTE型数组         
		for (k = 0; k < len; k++)                    //将数组转换为Cstring型变量    
		{
			BYTE bt = *(char*)(rxdata + k);//字符型      
			strtemp.Format(_T("%c"), bt); //将字符送入临时变量strtemp存放   
			_RXData += strtemp; //加入接收编辑框对应字符串      
		}
	}
	if (_RXData == _T(""))
		return;

	m_text1 += _T("R:") + _RXData;

	if (_order1 >= 0)
	{
		if (_RXData.Find(_T("OK")) != -1 || _HC05_ORDERROLE0[_order1].Find(_T("?")) != -1)
		{
			_error = 0;
			KillTimer(1);
			SetTimer(1, 100000, NULL);
			_order1++;
			CString str;
			if (_order1 < (int)_HC05_ORDERROLE0.size())
			{
				if (_HC05_ORDERROLE0[_order1 - 1].Find(_T(_ADDR_)) != -1)
				{
					_Addr = _RXData;
					_Addr.Replace(':', ',');
					_Addr.Delete(0, 6);
					_Addr.Remove(_T('\r'));
					_Addr.Remove(_T('\n'));
					_Addr.Delete(_Addr.GetLength() - 2, 2);
					for (int index = 0, temp = 0; temp < (int)_HC05_ORDERROLE1.size(); ++temp)
					{
						if ((index = _HC05_ORDERROLE1[temp].Find(_T("addr"))) != -1)
						{
							_HC05_ORDERROLE1[temp] = _HC05_ORDERROLE1[temp].Left(index) +
								_Addr +
								_HC05_ORDERROLE1[temp].Right(_HC05_ORDERROLE1[temp].GetLength() - index - 4);
						}
					}
				}
				str = _HC05_ORDERROLE0[_order1] + _T("\r\n");
				m_ctrlComm1.put_Output(COleVariant(str));
				m_text1 += _T("T:") + str;
			}
			else
			{
				str = _HC05_ORDERROLE1[_order2] + _T("\r\n");
				m_text2 += _T("T:") + str;
				m_ctrlComm2.put_Output(COleVariant(str));
				_flagset1 = true;
			}
		}
	}
	else
	{
		if (_RXData.Find(_T("OK")) != -1)
		{
			_error = 0;
			KillTimer(1);
			_flagset1 = true;
			SetTimer(1, 100000, NULL);
			m_ctrlComm2.put_Output(COleVariant(_T("AT+ORGL\r\n")));
			m_text2 += _T("T:AT+ORGL\r\n");
		}
	}
	UpdateData(FALSE);
}

void CHC05Dlg::OnCommMscomm2()
{
	// TODO: 在此处添加消息处理程序代码
	VARIANT variant_inp;   //Variant 是一种特殊的数据类型，除了定长String数据及用户定义类型外，可以包含任何种类的数据。
	COleSafeArray safearray_inp;
	LONG len, k;
	BYTE rxdata[2048]; //设置BYTE数组 An 8-bit integer that is not signed.     
	CString strtemp;
	CString _RXData;
	if (m_ctrlComm2.get_CommEvent() == 2) //事件值为2表示接收缓冲区内有字符     
	{
		////////以下你可以根据自己的通信协议加入处理代码 
		_RXData = "";
		variant_inp = m_ctrlComm2.get_Input();		//读缓冲区  
		safearray_inp = variant_inp;				//VARIANT型变量转换为ColeSafeArray型变量
		len = safearray_inp.GetOneDimSize();		//得到有效数据长度        
		for (k = 0; k < len; k++)
			safearray_inp.GetElement(&k, rxdata + k);//转换为BYTE型数组         
		for (k = 0; k < len; k++)                    //将数组转换为Cstring型变量    
		{
			BYTE bt = *(char*)(rxdata + k);//字符型      
			strtemp.Format(_T("%c"), bt); //将字符送入临时变量strtemp存放   
			_RXData += strtemp; //加入接收编辑框对应字符串      
		}
	}
	if (_RXData == _T(""))
		return;

	m_text2 += _T("R:") + _RXData;

	if (_order1 >= 0)
	{
		if (_RXData.Find(_T("OK")) != -1)
		{
			_error = 0;
			KillTimer(1);
			SetTimer(1, 100000, NULL);
			_order2++;
			CString str;

			if (_order2 < (int)_HC05_ORDERROLE1.size())
			{
				str = _HC05_ORDERROLE1[_order2] + _T("\r\n");
				m_ctrlComm2.put_Output(COleVariant(str));
				m_text2 += _T("T:") + str;
			}
			else
			{
				KillTimer(1);
				_flagset2 = true;
				UpdateData(FALSE);
				MessageBox(_T("设置成功"));
			}
		}
	}
	else
	{
		if (_RXData.Find(_T("OK")) != -1)
		{
			_error = 0;
			KillTimer(1);
			_flagset2 = true;
			UpdateData(FALSE);
			MessageBox(_T("     复位成功\r\n请重新插拔蓝牙并进入AT模式后设置"));
		}
	}
	UpdateData(FALSE);
}

//按下“开始设置”按键处理函数
void CHC05Dlg::OnBnClickedButtonStartset()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ctrlComm1.put_Output(COleVariant(_HC05_ORDERROLE0[0] + _T("\r\n")));
	m_text1 = _T("T:") + _HC05_ORDERROLE0[0] + _T("\r\n");
	m_text2 = _T("");
	_error = 0;
	_order1 = 0; _order2 = 0;
	_flagset1 = false; _flagset2 = false; _flagreset = false;
	SetTimer(1, 100000, NULL);
	UpdateData(FALSE);
}

//按下“复位蓝牙”按键处理函数
void CHC05Dlg::OnBnClickedButtonOrgl()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ctrlComm1.put_Output(COleVariant(_T("AT+ORGL\r\n")));
	m_text1 = _T("T:AT+ORGL\r\n");
	m_text2 = _T("");
	_order1 = -1; _order2 = -1;
	_flagset1 = false; _flagset2 = false; _flagreset = true;
	SetTimer(1, 100000, NULL);
	UpdateData(FALSE);
}

//按下“打开串口”按键处理函数
void CHC05Dlg::OnBnClickedButtonOpencom()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel;
	CString strCBText;

	UpdateData(TRUE);
	while (m_pswd.GetLength() < 4) m_pswd = '0' + m_pswd;
	UpdateData(FALSE);

	if (((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel() == -1 || ((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel() == -1)
	{
		MessageBoxW(_T("请正确选择串口号！"));
		return;
	}

	if (m_setOk == FALSE)
	{
		nSel = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
		((CComboBox*)GetDlgItem(IDC_COMBO1))->GetLBText(nSel, strCBText);

		strCBText.Delete(0, 3);

		m_ctrlComm1.put_CommPort(_wtoi(strCBText));//选择串口号(这里因为列表框的索引号是从0开始，所以(nSel+1)对应的才是我们所选的串口号)
		m_ctrlComm1.put_PortOpen(TRUE);//打开串口
		m_ctrlComm1.put_RThreshold(2);//收到两个字节引发OnComm事件	
		m_ctrlComm1.put_InputMode(1);//输入模式选为二进制	
		m_ctrlComm1.put_Settings(_T("38400,n,8,1"));//设置串口参数，波特率，无奇偶校验，位停止位，位数据位
		m_ctrlComm1.put_InputMode(1);  // 以二进制方式检取数据 
		m_ctrlComm1.put_RThreshold(1); //参数1表示每当串口接收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件 
		m_ctrlComm1.put_InputLen(0); //设置当前接收区数据长度为0 
		m_ctrlComm1.get_Input();//先预读缓冲区以清除残留数据  

		/////////////////////////////////////////////////////////////////////

		nSel = ((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel();
		((CComboBox*)GetDlgItem(IDC_COMBO2))->GetLBText(nSel, strCBText);

		strCBText.Delete(0, 3);

		m_ctrlComm2.put_CommPort(_wtoi(strCBText));//选择串口号(这里因为列表框的索引号是从0开始，所以(nSel+1)对应的才是我们所选的串口号)
		m_ctrlComm2.put_PortOpen(TRUE);//打开串口
		m_ctrlComm2.put_RThreshold(2);//收到两个字节引发OnComm事件	
		m_ctrlComm2.put_InputMode(1);//输入模式选为二进制	
		m_ctrlComm2.put_Settings(_T("38400,n,8,1"));//设置串口参数，波特率，无奇偶校验，位停止位，位数据位
		m_ctrlComm2.put_InputMode(1);  // 以二进制方式检取数据 
		m_ctrlComm2.put_RThreshold(1); //参数1表示每当串口接收缓冲区中有多于或等于1个字符时将引发一个接收数据的OnComm事件 
		m_ctrlComm2.put_InputLen(0); //设置当前接收区数据长度为0 
		m_ctrlComm2.get_Input();//先预读缓冲区以清除残留数据  
	}
	else
	{
		m_ctrlComm1.put_PortOpen(FALSE);
		m_ctrlComm2.put_PortOpen(FALSE);
	}
	//如果打开串口成功
	if (m_ctrlComm1.get_PortOpen() && m_ctrlComm2.get_PortOpen())
	{
		m_setOk = true;
		COPY_HC05_ORDERROLE0 = _HC05_ORDERROLE0;
		COPY_HC05_ORDERROLE1 = _HC05_ORDERROLE1;
		GetDlgItem(IDC_EDITTEXT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDITTEXT2)->EnableWindow(TRUE); 
		GetDlgItem(IDC_EDITPSWD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTONPSWD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_STARTSET)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_ORGL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SET)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBORATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_OPENCOM)->SetWindowText(_T("关闭串口"));
		UpdateData(TRUE);
		for (int index = 0, temp = 0; temp < (int)_HC05_ORDERROLE0.size(); ++temp)
		{
			CString str;
			if ((index = _HC05_ORDERROLE0[temp].Find(_T("pswd"))) != -1)
			{
				_HC05_ORDERROLE0[temp] = _HC05_ORDERROLE0[temp].Left(index) +
					m_pswd +
					_HC05_ORDERROLE0[temp].Right(_HC05_ORDERROLE0[temp].GetLength() - index - 4);
			}
			if ((index = _HC05_ORDERROLE0[temp].Find(_T("rate"))) != -1)
			{
				m_rate.GetWindowTextW(str);
				_HC05_ORDERROLE0[temp] = _HC05_ORDERROLE0[temp].Left(index) +
					str +
					_HC05_ORDERROLE0[temp].Right(_HC05_ORDERROLE0[temp].GetLength() - index - 4);
			}
		}
		for (int index = 0, temp = 0; temp < (int)_HC05_ORDERROLE1.size(); ++temp)
		{
			CString str;
			if ((index = _HC05_ORDERROLE1[temp].Find(_T("pswd"))) != -1)
			{
				_HC05_ORDERROLE1[temp] = _HC05_ORDERROLE1[temp].Left(index) +
					m_pswd +
					_HC05_ORDERROLE1[temp].Right(_HC05_ORDERROLE1[temp].GetLength() - index - 4);
			}
			if ((index = _HC05_ORDERROLE1[temp].Find(_T("rate"))) != -1)
			{
				m_rate.GetWindowTextW(str);
				_HC05_ORDERROLE1[temp] = _HC05_ORDERROLE1[temp].Left(index) +
					str +
					_HC05_ORDERROLE1[temp].Right(_HC05_ORDERROLE1[temp].GetLength() - index - 4);
			}
		}
	}
	//如果打开串口失败
	else
	{
		CloseComm();
	}
}

void CHC05Dlg::CloseComm()
{
	m_setOk = false;
	if (m_ctrlComm1.get_PortOpen()) m_ctrlComm1.put_PortOpen(FALSE);
	if (m_ctrlComm2.get_PortOpen()) m_ctrlComm2.put_PortOpen(FALSE);
	_HC05_ORDERROLE0 = COPY_HC05_ORDERROLE0;
	_HC05_ORDERROLE1 = COPY_HC05_ORDERROLE1;
	GetDlgItem(IDC_EDITTEXT1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITTEXT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDITPSWD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTONPSWD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STARTSET)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ORGL)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SET)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO2)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBORATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OPENCOM)->SetWindowText(_T("打开串口"));
}

//按下串口号1下拉菜单的处理函数
//用于检测可以用的串口号
void CHC05Dlg::OnCbnDropdownCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	HANDLE hCom;
	CString str, _strothercomn, _strcomn;
	bool flag = false;

	// TODO: 在此添加控件通知处理程序代码
	((CComboBox *)GetDlgItem(IDC_MSCOMM1))->ResetContent();
	((CComboBox *)GetDlgItem(IDC_COMBO1))->ResetContent();
	((CComboBox *)GetDlgItem(IDC_COMBO2))->GetWindowTextW(_strothercomn);

	for (int comn = 1; comn < 32; comn++)
	{
		str.Format(_T("\\\\.\\COM%d"), comn);
		hCom = CreateFile(str, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE != hCom)
		{
			//能打开该串口，则添加该串口
			CloseHandle(hCom);
			_strcomn.Format(_T("COM%d"), comn);
			if (_strothercomn != _strcomn)
				((CComboBox *)GetDlgItem(IDC_COMBO1))->AddString(_strcomn);
		}
	}
}

//按下串口号2下拉菜单的处理函数
//用于检测可以用的串口号
void CHC05Dlg::OnCbnDropdownCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	HANDLE hCom;
	CString str, _strothercomn, _strcomn;
	bool flag = false;

	// TODO: 在此添加控件通知处理程序代码
	((CComboBox *)GetDlgItem(IDC_MSCOMM2))->ResetContent();
	((CComboBox *)GetDlgItem(IDC_COMBO2))->ResetContent();
	((CComboBox *)GetDlgItem(IDC_COMBO1))->GetWindowTextW(_strothercomn);

	for (int comn = 1; comn < 32; comn++)
	{
		str.Format(_T("\\\\.\\COM%d"), comn);
		hCom = CreateFile(str, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (INVALID_HANDLE_VALUE != hCom)
		{
			//能打开该串口，则添加该串口
			CloseHandle(hCom);
			_strcomn.Format(_T("COM%d"), comn);
			if (_strothercomn != _strcomn)
				((CComboBox *)GetDlgItem(IDC_COMBO2))->AddString(_strcomn);
		}
	}
}

//按下“随机密码”按键处理函数
void CHC05Dlg::OnBnClickedButtonpswd()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pswd.Format(_T("%d%d%d%d"), rand() % 10, rand() % 10, rand() % 10, rand() % 10);
	UpdateData(FALSE);
}

void CHC05Dlg::OnEnKillfocusEditpswd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_pswd = _T("0000") + m_pswd;
	while (m_pswd.GetLength() > 4) m_pswd.Delete(0);
	UpdateData(FALSE);
}

//按下“说明”按键的处理函数
//弹出 说明 界面
void CHC05Dlg::OnBnClickedButtonTip()
{
	// TODO: 在此添加控件通知处理程序代码
	m_text1 = _T("说明：\r\n    这是一个HC-05蓝牙配对上位机\r\n\r\n串口接线：\r\nGND<->GND\r\nRXD<->TXD\r\nTXD<->RXD\r\nVCC<->5V\r\nEN<->3V3\r\n");
	m_text2 = _T("注意！必须按住蓝牙上的按键再上电方可进入AT模式，才可以进行配对。\r\n\r\n手工配对教程网址：https://blog.csdn.net/m0_37182543/article/details/76383247 \r\n\r\n制作：\r\n    刘力铭  2018/10/24 \r\n    1104257233@qq.com");
	UpdateData(FALSE); 
}

//定时器处理函数
//用于蓝牙设置超时检测，出错数据重发
void CHC05Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	INT_PTR nRes;
	ErrorDlg errorDlg;
	CString str;

	switch (nIDEvent)
	{
	case 1:
		if (_error >= 4)
		{
			KillTimer(1);
			nRes = errorDlg.DoModal();
		}
		else
		{
			++_error;
			if (_flagreset == false && _flagset1 == false && _flagset2 == false)
			{
				str = _HC05_ORDERROLE0[_order1] + _T("\r\n");
				m_ctrlComm1.put_Output(COleVariant(str));
				m_text1 += _T("T:") + str;
			}
			else if (_flagreset == false && _flagset1 == true && _flagset2 == false)
			{
				str = _HC05_ORDERROLE1[_order2] + _T("\r\n");
				m_ctrlComm2.put_Output(COleVariant(str));
				m_text2 += _T("T:") + str;
			}
			else if (_flagreset == true && _flagset1 == false && _flagset2 == false)
			{
				m_ctrlComm1.put_Output(COleVariant(_T("AT+ORGL\r\n")));
				m_text1 += _T("T:AT+ORGL\r\n");
			}
			else if (_flagreset == true && _flagset1 == true && _flagset2 == false)
			{
				m_ctrlComm2.put_Output(COleVariant(_T("AT+ORGL\r\n")));
				m_text2 += _T("T:AT+ORGL\r\n");
			}
			SetTimer(1, 500000, NULL);
			UpdateData(FALSE);
		}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

//按下“设置”按键处理函数
//弹出 设置 界面
void CHC05Dlg::OnBnClickedButtonSet()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;
	nRes = _setdlg.DoModal();
	UpdateData(FALSE);
}


BOOL CHC05Dlg::OnDeviceChange(UINT nEventType, DWORD dwData)
{
	switch (nEventType)
	{
	case DBT_DEVICEREMOVECOMPLETE://移除设备
		if (m_setOk)
		{
			CloseComm();
		}
		OnCbnDropdownCombo1();
		OnCbnDropdownCombo2();
		break;

	case DBT_DEVICEARRIVAL://添加设备
		break;

	default:
		break;
	}
	return TRUE;
}
