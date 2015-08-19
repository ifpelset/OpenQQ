#pragma once

#include "stdafx.h"

struct SendMsg
{
	TCHAR userName[20];//更新用
	TCHAR sendUser[20];
	TCHAR recvUser[20];
	TCHAR sendMsg[250];
	BOOL isSingleSend;
	BOOL isFriendInfo;
	BOOL isOnLine;
	
	//后面的在文件传输用
	BOOL isFile;
	BOOL isFinished;
	UINT transSize;
	UINT fileOffset;
	char sendFile[20480];
	TCHAR fileName[50];
	ULONGLONG fileLen;

	BOOL isWindowShake;//判断消息是不是窗口抖动

	//头像同步
	BOOL isHeadPic;
	UINT headNum;
};