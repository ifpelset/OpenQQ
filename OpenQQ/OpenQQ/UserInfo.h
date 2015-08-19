#pragma once

#include "stdafx.h"

//用户信息结构
struct UserInfo
{
	UINT	headNum;//可以取1，2，3，4，5，6
	TCHAR	userName[20];
	TCHAR	passWord[20];
	BOOL	bOnLine;
	BOOL	accFlag;
	SOCKET	userSocket;
	SOCKET	friendSocket;
	BOOL	isFriendInfo;//来自服务器消息是好友的信息还是 真正我该接收的消息
	BOOL	isRegister;
};
