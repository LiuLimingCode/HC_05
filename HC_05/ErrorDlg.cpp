// ErrorDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "HC_05.h"
#include "ErrorDlg.h"
#include "afxdialogex.h"


// ErrorDlg 对话框

IMPLEMENT_DYNAMIC(ErrorDlg, CDialogEx)

ErrorDlg::ErrorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ERROR, pParent)
{

}

ErrorDlg::~ErrorDlg()
{
}

void ErrorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ErrorDlg, CDialogEx)
END_MESSAGE_MAP()


// ErrorDlg 消息处理程序
