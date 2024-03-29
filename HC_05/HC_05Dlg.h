
// HC_05Dlg.h: 头文件
//

#pragma once

#include "CMSComm.h"
#include <vector>
#include "SetDlg.h"
#include "Order.h"

// CHC05Dlg 对话框
class CHC05Dlg : public CDialogEx
{
// 构造
public:
	CHC05Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HC_05_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCbnDropdownCombo1();
	afx_msg void OnCbnDropdownCombo2();
	afx_msg void OnBnClickedButtonOpencom();
	afx_msg void OnBnClickedButtonStartset();
	afx_msg void OnBnClickedButtonpswd();
	afx_msg void OnBnClickedButtonTip();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonOrgl();
	afx_msg void OnEnKillfocusEditpswd();
	afx_msg BOOL OnDeviceChange(UINT nEventType, DWORD dwData);
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:

	void OnCommMscomm1();
	void OnCommMscomm2();
	void CloseComm();

	CMSComm m_ctrlComm1;
	CMSComm m_ctrlComm2;
	CString m_text1;
	CString m_text2;
	bool m_setOk = false;
	CString _Addr;
	int _order1;
	int _order2;
	bool _flagset1 = 0;//串口1设置成功标志
	bool _flagset2 = 0;//串口2设置成功标志
	bool _flagreset = 0;//是否是复位设置标志
	int _error = 0;
	CComboBox m_rate;
	std::vector<CString> COPY_HC05_ORDERROLE0;
	std::vector<CString> COPY_HC05_ORDERROLE1;
	const std::vector<CString> _RATE =
	{
		_T("4800"),
		_T("9600"),
		_T("14400"),
		_T("38400"),
		_T("57600"),
		_T("115200"),
	};
	CString m_pswd;
	SetDlg _setdlg;
	
};
