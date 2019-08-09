#pragma once

#include <vector>
#include "stdafx.h"

#define _ADDR_ "ADDR?"

const std::vector<CString> CONST_HC05_ORDERROLE0 =
{
	_T("AT"),
	_T("AT+PSWD=\"pswd\""),//设置密码
	_T("AT+ROLE=0"),//设置主从
	_T("AT+ADDR?"),//地址
	_T("AT+UART=rate,0,0"),//设置波特率
};

const std::vector<CString> CONST_HC05_ORDERROLE1 =
{
	_T("AT"),
	_T("AT+PSWD=\"pswd\""),//设置密码
	_T("AT+ROLE=1"),//设置主从
	_T("AT+BIND=addr"),
	_T("AT+UART=rate,0,0"),
};