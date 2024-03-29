// SetDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "HC_05.h"
#include "SetDlg.h"
#include "afxdialogex.h"


// SetDlg 对话框

extern std::vector<CString> _HC05_ORDERROLE0;
extern std::vector<CString> _HC05_ORDERROLE1;

IMPLEMENT_DYNAMIC(SetDlg, CDialogEx)

SetDlg::SetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SET, pParent)
	, m_setorder0_1(_T(""))
	, m_setorder0_2(_T(""))
	, m_setorder0_3(_T(""))
	, m_setorder0_4(_T(""))
	, m_setorder0_5(_T(""))
	, m_setorder0_6(_T(""))
	, m_setorder0_7(_T(""))
	, m_setorder1_1(_T(""))
	, m_setorder1_2(_T(""))
	, m_setorder1_3(_T(""))
	, m_setorder1_4(_T(""))
	, m_setorder1_5(_T(""))
	, m_setorder1_6(_T(""))
	, m_setorder1_7(_T(""))
{

}

SetDlg::~SetDlg()
{
}

void SetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITSETORDER0_1, m_setorder0_1);
	DDX_Text(pDX, IDC_EDITSETORDER0_2, m_setorder0_2);
	DDX_Text(pDX, IDC_EDITSETORDER0_3, m_setorder0_3);
	DDX_Text(pDX, IDC_EDITSETORDER0_4, m_setorder0_4);
	DDX_Text(pDX, IDC_EDITSETORDER0_5, m_setorder0_5);
	DDX_Text(pDX, IDC_EDITSETORDER0_6, m_setorder0_6);
	DDX_Text(pDX, IDC_EDITSETORDER0_7, m_setorder0_7);
	DDX_Text(pDX, IDC_EDITSETORDER1_1, m_setorder1_1);
	DDX_Text(pDX, IDC_EDITSETORDER1_2, m_setorder1_2);
	DDX_Text(pDX, IDC_EDITSETORDER1_3, m_setorder1_3);
	DDX_Text(pDX, IDC_EDITSETORDER1_4, m_setorder1_4);
	DDX_Text(pDX, IDC_EDITSETORDER1_5, m_setorder1_5);
	DDX_Text(pDX, IDC_EDITSETORDER1_6, m_setorder1_6);
	DDX_Text(pDX, IDC_EDITSETORDER1_7, m_setorder1_7);
}


BEGIN_MESSAGE_MAP(SetDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &SetDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &SetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTONRESET, &SetDlg::OnBnClickedButtonreset)
END_MESSAGE_MAP()


// SetDlg 消息处理程序

//按下“取消”按键处理函数
//此时把 _HC05_ORDERROLE 数组重新赋值给 m_setorder ，以把文本框变回上一次保存的数据
void SetDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(FALSE);
	CDialogEx::OnCancel();
}

//按下“保存”按键处理函数
//此时把 m_setorder 数组赋值给 _HC05_ORDERROLE ，以保存文本框
void SetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int _flag = 0;
	for (unsigned int temp = 0; temp < m_setorder0.size(); ++temp)
		if (m_setorder0[temp]->Find(_T(_ADDR_)) != -1)
			_flag = 0;
	for (unsigned int temp = 0; temp < m_setorder1.size(); ++temp)
		if (m_setorder1[temp]->Find(_T(_ADDR_)) != -1)
			_flag = 1;
	//必须要先设置有ADDR指令的蓝牙
	_HC05_ORDERROLE0.clear();
	for (unsigned int temp = 0, index = 0; temp < m_setorder[_flag].size(); ++temp)
	{
		if (m_setorder[_flag][temp]->GetLength() > 0)
		{
			_HC05_ORDERROLE0.push_back(*m_setorder[_flag][temp]);
			++index;
		}
	}
	_HC05_ORDERROLE1.clear();
	for (unsigned int temp = 0, index = 0; temp < m_setorder[1 - _flag].size(); ++temp)
	{
		if (m_setorder[1 - _flag][temp]->GetLength() > 0)
		{
			_HC05_ORDERROLE1.push_back(*m_setorder[1 - _flag][temp]);
			++index;
		}
	}

	//把数据保存到文件中
	SaveCfgData();

	CDialogEx::OnOK();
}

//按下“重置”按键处理函数
//此时把 CONST_HC05_ORDERROLE 数组赋值给 m_setorder ，以保存文本框
void SetDlg::OnBnClickedButtonreset()
{
	// TODO: 在此添加控件通知处理程序代码
	unsigned int temp;
	_HC05_ORDERROLE0 = CONST_HC05_ORDERROLE0;
	_HC05_ORDERROLE1 = CONST_HC05_ORDERROLE1;
	for (temp = 0; temp < _HC05_ORDERROLE0.size() && temp < m_setorder0.size(); ++temp)
	{
		*(m_setorder0[temp]) = _HC05_ORDERROLE0[temp];
	}
	for (; temp < m_setorder0.size(); ++temp)
		*(m_setorder0[temp]) = _T("");
	for (temp = 0; temp < _HC05_ORDERROLE1.size() && temp < m_setorder1.size(); ++temp)
	{
		*(m_setorder1[temp]) = _HC05_ORDERROLE1[temp];
	}
	for (; temp < m_setorder1.size(); ++temp)
		*(m_setorder1[temp]) = _T("");

	//把数据保存到文件中
	SaveCfgData();

	UpdateData(FALSE);
}

void SetDlg::SaveCfgData(void)
{
	CFile cfg_file;
	cfg_file.Open(_T("cfg.txt"), CFile::modeWrite | CFile::modeCreate | CFile::typeBinary | CFile::shareDenyNone);
	for (unsigned int temp = 0; temp < m_setorder0.size(); ++temp)
	{
		char ch; CString str;
		str = *(m_setorder0[temp]) + _T("\r\n");
		for (int index = 0; index < str.GetLength(); ++index)
		{
			ch = (char)str[index];
			cfg_file.Write(&ch, 1);
		}
	}
	for (unsigned int temp = 0; temp < m_setorder1.size(); ++temp)
	{
		char ch; CString str;
		str = *(m_setorder1[temp]) + _T("\r\n");
		for (int index = 0; index < str.GetLength(); ++index)
		{
			ch = (char)str[index];
			cfg_file.Write(&ch, 1);
		}
	}
	cfg_file.Close();
}
