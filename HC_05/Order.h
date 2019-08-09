#pragma once

#include <vector>
#include "stdafx.h"

#define _ADDR_ "ADDR?"

const std::vector<CString> CONST_HC05_ORDERROLE0 =
{
	_T("AT"),
	_T("AT+PSWD=\"pswd\""),//��������
	_T("AT+ROLE=0"),//��������
	_T("AT+ADDR?"),//��ַ
	_T("AT+UART=rate,0,0"),//���ò�����
};

const std::vector<CString> CONST_HC05_ORDERROLE1 =
{
	_T("AT"),
	_T("AT+PSWD=\"pswd\""),//��������
	_T("AT+ROLE=1"),//��������
	_T("AT+BIND=addr"),
	_T("AT+UART=rate,0,0"),
};