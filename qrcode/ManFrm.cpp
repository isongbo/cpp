#include "ManFrm.h"
#include "QR_Encode.h"
#include "Stdafx.h"
#include <windows.h>  
#include <stdio.h>

#define CX_SPLITTER	7
#define CY_SPLITTER	7
#define CX_MARGIN	2
#define CY_MARGIN	2

extern char g_CurPath[MAX_PATH];
void WriteLog( char* sfmt ,...)
{
	//return ;
	char szMes [4096]={0};

	va_list ap ;
	va_start(ap ,sfmt);

	FILE *fp = fopen( "log\\prnt2.txt" ,"a");
	if (NULL != fp)
	{
		//输出时间
		char sztime [64]={0};
		SYSTEMTIME st ;
		GetLocalTime(&st );

		sprintf(sztime , "%02d %02d:%02d:%02d.%03d ",
			st.wDay , st. wHour, st .wMinute, st.wSecond , st. wMilliseconds);
		fprintf(fp , sztime);

		vsprintf (szMes , sfmt, ap);
		fprintf(fp , szMes);

		fprintf(fp , "\r\n");
		fclose(fp );
	}

	va_end(ap );
}
CMainFrame::CMainFrame()
{
	m_bInitSplit = FALSE;
	
	m_bDataEncoded = FALSE;
	m_pSymbleDC = NULL;
	
	m_strSavePath.Empty();
}
CMainFrame::~CMainFrame()
{
	if (m_pSymbleDC != NULL)
	{
		m_pSymbleDC->SelectObject(m_pOldBitmap);
		
		delete m_pSymbleBitmap;
		delete m_pSymbleDC;
	}
}
char szqrcodepath[MAX_PATH] = {0};
void CMainFrame::ShowImage(char* source)
{
	char level[10]={0};
	sprintf(szqrcodepath,".\\config\\qrcode.ini");
	WriteLog("szqrcodepath = %s",szqrcodepath);
	GetPrivateProfileStringA("code", "nlevel", "2", level, sizeof(level), szqrcodepath);
	char version[10]={0};
	GetPrivateProfileStringA("code", "nVersion", "1", version, sizeof(version),szqrcodepath);
	char masking[10]={0};
	GetPrivateProfileStringA("code", "nMaskingNo", "1", masking, sizeof(masking), szqrcodepath);
	int nlevel = atoi(level);
	int nVersion = atoi(version);
	int nMaskingNo =atoi(masking);
	int bAutoExtent =1;

	CQR_Encode * pCQR_Encode =new CQR_Encode;
	m_bDataEncoded = pCQR_Encode->EncodeData(nlevel,nVersion,bAutoExtent,nMaskingNo,source);
	if (m_bDataEncoded)
	{
		if (m_pSymbleDC != NULL)
		{
			m_pSymbleDC->SelectObject(m_pOldBitmap);
			delete m_pOldBitmap;
			delete m_pSymbleDC;
		}
		
		m_nSymbleSize = pCQR_Encode->m_nSymbleSize +(QR_MARGIN * 2);
		
		m_pSymbleBitmap = new CBitmap;
		m_pSymbleBitmap->CreateBitmap(m_nSymbleSize, m_nSymbleSize, 1, 1, NULL);

		m_pSymbleDC = new CDC;
		m_pSymbleDC->CreateCompatibleDC(NULL);
		m_pOldBitmap = m_pSymbleDC->SelectObject(m_pSymbleBitmap);
		m_pSymbleDC->PatBlt( 0 , 0 , m_nSymbleSize , m_nSymbleSize, WHITENESS);

		int i, j;
		for ( i = 0 ; i < pCQR_Encode->m_nSymbleSize ;  ++i)
		{
			for ( j = 0 ; j <pCQR_Encode->m_nSymbleSize ; ++j)
			{
				if (pCQR_Encode->m_byModuleData[i][j])
				{
					m_pSymbleDC->SetPixel(i + QR_MARGIN, j + QR_MARGIN, RGB(0,0,0));
				}
			}
		}
	}

	delete pCQR_Encode;
}
void CMainFrame::SaveImage(char * bmpname)
{
	char qusre[10]={0};
	GetPrivateProfileStringA("code", "SquareNum", "1", qusre, sizeof(qusre), szqrcodepath);
	int Square_Num = atoi(qusre);
	//WriteLog("Square_Num = %d",Square_Num);
	if (Square_Num > 20)
	{
		Square_Num = 20;
	}else if (Square_Num < 1)
	{
		Square_Num = 1;
	}
	int nBlankSize = 2/*m_nSymbleSize/11*/;
	//int nPixelSize = (Square_Num * m_nSymbleSize)/8 * 8;
	int nPixelSize = 224/*256*//*224*/;
	int nBmpSize = ((nPixelSize + 31-2*nBlankSize) / 32) * 32 / 8;
	nBmpSize *= nPixelSize;
	m_strSavePath = bmpname;
	HANDLE hFile = CreateFile(m_strSavePath, GENERIC_READ | GENERIC_WRITE, 0, NULL,
							  CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		m_strSavePath.Empty();
//		MessageBox("是否保存文件","QR Code保存?", MB_ICONSTOP);
		return;
	}
	HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE | SEC_COMMIT, 0, 
						  sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize, NULL);
	LPBYTE lpbyMapView = (LPBYTE)MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);
	ZeroMemory(lpbyMapView, sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize);
	//////////////////////////////////////////////////////////////////
	LPBITMAPFILEHEADER pbfh = (LPBITMAPFILEHEADER)lpbyMapView;
	LPBITMAPINFO       pbmi = (LPBITMAPINFO)(lpbyMapView + sizeof(BITMAPFILEHEADER));
	LPVOID             pbdt = lpbyMapView + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2;
	
	// 位图BITMAPFILEHEADER
	pbfh->bfType      = (WORD) (('M' << 8) | 'B'); // "BM"
	pbfh->bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2 + nBmpSize;
	pbfh->bfReserved1 = 0;
	pbfh->bfReserved2 = 0;
	pbfh->bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 2;

	// LPBITMAPINFO->BITMAPINFOHEADER 二维数组->灰度图
	pbmi->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth			= nPixelSize;
	pbmi->bmiHeader.biHeight		= nPixelSize;
	pbmi->bmiHeader.biPlanes		= 1;
	pbmi->bmiHeader.biBitCount		= 1;
	pbmi->bmiHeader.biCompression	= BI_RGB;
	pbmi->bmiHeader.biSizeImage		= nBmpSize;
	pbmi->bmiHeader.biXPelsPerMeter = 3780;
	pbmi->bmiHeader.biYPelsPerMeter = 3780;
	pbmi->bmiHeader.biClrUsed		= 0;
	pbmi->bmiHeader.biClrImportant	= 0;

	CDC* pWorkDC = new CDC;
	pWorkDC->CreateCompatibleDC(NULL);
	CBitmap* pWorkBitmap = new CBitmap;
	pWorkBitmap->CreateBitmap(nPixelSize, nPixelSize, 1, 1, NULL);
	CBitmap* pOldBitmap = pWorkDC->SelectObject(pWorkBitmap);
	//pWorkDC->StretchBlt(0, 0, nPixelSize, nPixelSize, m_pSymbleDC, 0, 0, m_nSymbleSize, m_nSymbleSize, SRCCOPY);
	//去白边
	pWorkDC->StretchBlt(0, 0, nPixelSize-nBlankSize+3, nPixelSize-nBlankSize+3, m_pSymbleDC, nBlankSize,nBlankSize, m_nSymbleSize-2*nBlankSize, m_nSymbleSize-2*nBlankSize, SRCCOPY);

	GetDIBits(pWorkDC->m_hDC, (HBITMAP)*pWorkBitmap, 0, nPixelSize, pbdt, pbmi, DIB_RGB_COLORS);
	pWorkDC->SelectObject(pOldBitmap);

	delete pWorkBitmap;
	delete pWorkDC;

	UnmapViewOfFile(lpbyMapView);
	CloseHandle(hFileMapping);
	CloseHandle(hFile);
	
}