#include "Stdafx.h"
#include "AfxDllx.h"
#include "SkinControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//静态变量
static AFX_EXTENSION_MODULE SkinControlDLL={NULL,NULL};

//////////////////////////////////////////////////////////////////////////////////

//声明钩子
DETOUR_TRAMPOLINE(INT   WINAPI GetScrollPosT(HWND, INT), GetScrollPos)
DETOUR_TRAMPOLINE(INT   WINAPI SetScrollPosT(HWND, INT, INT, BOOL), SetScrollPos)
DETOUR_TRAMPOLINE(BOOL  WINAPI GetScrollInfoT(HWND, INT, LPSCROLLINFO), GetScrollInfo)
DETOUR_TRAMPOLINE(INT   WINAPI SetScrollInfoT(HWND, INT, LPCSCROLLINFO, BOOL), SetScrollInfo)
DETOUR_TRAMPOLINE(BOOL  WINAPI GetScrollRangeT(HWND, INT, LPINT, LPINT), GetScrollRange)
DETOUR_TRAMPOLINE(BOOL  WINAPI SetScrollRangeT(HWND, INT, INT, INT, BOOL), SetScrollRange)
DETOUR_TRAMPOLINE(BOOL  WINAPI ShowScrollBarT(HWND, INT, BOOL), ShowScrollBar)
DETOUR_TRAMPOLINE(BOOL  WINAPI EnableScrollBarT(HWND, UINT, UINT), EnableScrollBar)

//////////////////////////////////////////////////////////////////////////////////

//设置滚动
INT WINAPI SetScrollInfoD(HWND hWnd, INT fnBar, LPCSCROLLINFO lpScrollInfo, BOOL bRedraw)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_SetScrollInfo(hWnd,fnBar,lpScrollInfo,bRedraw);
	}
	else
	{
		return SetScrollInfoT(hWnd,fnBar,lpScrollInfo,bRedraw);
	}
}

//获取滚动
BOOL WINAPI GetScrollInfoD(HWND hWnd, INT fnBar, LPSCROLLINFO lpScrollInfo)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_GetScrollInfo(hWnd, fnBar, lpScrollInfo);
	}
	else
	{
		return GetScrollInfoT(hWnd, fnBar, lpScrollInfo);
	}
}

//设置滚动
INT WINAPI SetScrollPosD(HWND hWnd, INT nBar, INT nPos, BOOL bRedraw)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_SetScrollPos(hWnd,nBar, nPos,bRedraw);
	}
	else
	{
		return SetScrollPosT(hWnd,nBar, nPos,bRedraw);
	}
}

//获取滚动
INT WINAPI GetScrollPosD(HWND hWnd, INT nBar)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_GetScrollPos(hWnd,nBar);
	}
	else
	{
		return GetScrollPosT(hWnd,nBar);
	}
}

//设置滚动
BOOL WINAPI SetScrollRangeD(HWND hWnd, INT nBar, INT nMinPos, INT nMaxPos, BOOL bRedraw)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_SetScrollRange(hWnd,nBar,nMinPos,nMaxPos,bRedraw);
	}
	else
	{
		return SetScrollRangeT(hWnd,nBar,nMinPos,nMaxPos,bRedraw);
	}
}

//获取滚动
BOOL WINAPI GetScrollRangeD(HWND hWnd, INT nBar, LPINT lpMinPos, LPINT lpMaxPos)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_GetScrollRange(hWnd,nBar,lpMinPos,lpMaxPos);
	}
	else
	{
		return GetScrollRangeT(hWnd,nBar,lpMinPos,lpMaxPos);
	}
}

//显示滚动
BOOL WINAPI ShowScrollBarD(HWND hWnd, INT nBar, BOOL bShow)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_ShowScrollBar(hWnd,nBar,bShow);
	}
	else
	{
		return ShowScrollBarT(hWnd,nBar,bShow);
	}
}

//启用滚动
BOOL WINAPI EnableScrollBarD(HWND hWnd, UINT wSBflags, UINT wArrows)
{
	if (SkinSB_IsValid(hWnd))
	{
		return SkinSB_EnableScrollBar(hWnd,wSBflags,wArrows);
	}
	else
	{
		return EnableScrollBarT(hWnd,wSBflags,wArrows);
	}
}

//////////////////////////////////////////////////////////////////////////////////

//导出函数
extern "C" INT APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason==DLL_PROCESS_ATTACH)
	{
		if (!AfxInitExtensionModule(SkinControlDLL, hInstance)) return 0;
		new CDynLinkLibrary(SkinControlDLL);

		//加载钩子
		DetourFunctionWithTrampoline((PBYTE)SetScrollPosT,(PBYTE)SetScrollPosD);
		DetourFunctionWithTrampoline((PBYTE)GetScrollPosT,(PBYTE)GetScrollPosD);
		DetourFunctionWithTrampoline((PBYTE)SetScrollInfoT,(PBYTE)SetScrollInfoD);
		DetourFunctionWithTrampoline((PBYTE)GetScrollInfoT,(PBYTE)GetScrollInfoD);
		DetourFunctionWithTrampoline((PBYTE)ShowScrollBarT,(PBYTE)ShowScrollBarD);
		DetourFunctionWithTrampoline((PBYTE)SetScrollRangeT,(PBYTE)SetScrollRangeD);
		DetourFunctionWithTrampoline((PBYTE)GetScrollRangeT,(PBYTE)GetScrollRangeD);
		DetourFunctionWithTrampoline((PBYTE)EnableScrollBarT,(PBYTE)EnableScrollBarD);
	}
	else if (dwReason==DLL_PROCESS_DETACH)
	{
		AfxTermExtensionModule(SkinControlDLL);

		//卸载钩子
		DetourRemove((PBYTE)SetScrollPosT,(PBYTE)SetScrollPosD);
		DetourRemove((PBYTE)GetScrollPosT,(PBYTE)GetScrollPosD);
		DetourRemove((PBYTE)SetScrollInfoT,(PBYTE)SetScrollInfoD);
		DetourRemove((PBYTE)GetScrollInfoT,(PBYTE)GetScrollInfoD);
		DetourRemove((PBYTE)ShowScrollBarT,(PBYTE)ShowScrollBarD);
		DetourRemove((PBYTE)SetScrollRangeT,(PBYTE)SetScrollRangeD);
		DetourRemove((PBYTE)GetScrollRangeT,(PBYTE)GetScrollRangeD);
		DetourRemove((PBYTE)EnableScrollBarT,(PBYTE)EnableScrollBarD);
	}

	return 1;
}

//////////////////////////////////////////////////////////////////////////////////
