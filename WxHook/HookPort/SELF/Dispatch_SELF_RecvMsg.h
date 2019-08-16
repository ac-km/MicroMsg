#pragma once
#include"../InlineHook.h"

typedef VOID(WINAPI * __pfnWxRecvMsg)
(
);

extern __pfnWxRecvMsg pfnWxRecvMsg;
VOID WINAPI OnWxRecvMsg();

extern __pfnWxRecvMsg pfnWxRecvMsgForStream;
VOID WINAPI OnWxRecvMsgForStream();