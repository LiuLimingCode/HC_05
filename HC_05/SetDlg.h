#pragma once

#include <vector>
#include "Order.h"

// SetDlg 对话框

class SetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetDlg)

public:
	SetDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SetDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CString m_setorder0_1;
	CString m_setorder0_2;
	CString m_setorder0_3;
	CString m_setorder0_4;
	CString m_setorder0_5;
	CString m_setorder0_6;
	CString m_setorder0_7;
	std::vector<CString *> m_setorder0 = { &m_setorder0_1 ,&m_setorder0_2 ,&m_setorder0_3 ,&m_setorder0_4 ,&m_setorder0_5 ,&m_setorder0_6 ,&m_setorder0_7 };
	CString m_setorder1_1;
	CString m_setorder1_2;
	CString m_setorder1_3;
	CString m_setorder1_4;
	CString m_setorder1_5;
	CString m_setorder1_6;
	CString m_setorder1_7;
	std::vector<CString *> m_setorder1 = { &m_setorder1_1 ,&m_setorder1_2 ,&m_setorder1_3 ,&m_setorder1_4 ,&m_setorder1_5 ,&m_setorder1_6 ,&m_setorder1_7 };
	std::vector<CString *> m_setorder[2] = { m_setorder0 ,m_setorder1 };
	afx_msg void OnBnClickedButtonreset();
	void SaveCfgData(void);
};
