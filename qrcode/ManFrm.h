#ifndef MANFRM_H_
#define MANFRM_H_

#include "QR_Encode.h"
#include <afxwin.h>
#include <afxext.h>

#define IDD_ABOUTBOX                    100
#define IDR_MAINFRAME                   128
#define IDD_RIGHTVIEW                   130
#define IDD_BOTTOMVIEW                  131
#define IDR_IMAGEMENU                   132
#define IDC_RHAND                       133
#define IDI_PSYTEC                      134
#define IDC_COMBOLEVEL                  1000
#define IDC_COMBOVERSION                1001
#define IDC_STATICVERSION               1002
#define IDC_CHECKAUTOEXTENT             1003
#define IDC_COMBOMASKINGNO              1004
#define IDC_STATICMASKINGNO             1005
#define IDC_EDITMODULESIZE              1006
#define IDC_SPINMODULESIZE              1007
#define IDC_STATICBMPSIZE               1008
#define IDC_BUTTONCOPY                  1009
#define IDC_BUTTONSAVE                  1010
#define IDC_EDITSOURCEDATA              1011
#define IDC_STATICURL                   1020
#define IDC_STATICMAILTO                1021


class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();
	~CMainFrame();
public:
	int  m_bDataEncoded;
	int m_nSymbleSize;
	CBitmap* m_pOldBitmap;
	CBitmap* m_pSymbleBitmap;
	CString m_strSavePath;
	BOOL m_bInitSplit;

	CDC* m_pSymbleDC;
public:
	//生成灰度化二维数组
	void ShowImage(char* source);//传入生成图片的内容
	//保存图片
	void SaveImage(char* bmpname);//图片名称/路径+文件名
};

#endif




















