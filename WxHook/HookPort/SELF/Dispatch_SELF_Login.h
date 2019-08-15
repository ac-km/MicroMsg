#pragma once
#include"../InlineHook.h"

typedef VOID(WINAPI * __pfnWxLogin)
(
);


extern __pfnWxLogin pfnWxLoginForHeadUrl;
VOID WINAPI OnWxLoginForHeadUrl();

extern __pfnWxLogin pfnWxLoginForWxid;
VOID WINAPI OnWxLoginForWxid();