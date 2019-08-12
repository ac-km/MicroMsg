#pragma once
#include"../InlineHook.h"

typedef VOID(WINAPI * __pfnWxLogin)
(
);

extern __pfnWxLogin pfnWxLogin;

VOID
WINAPI
OnWxLogin();