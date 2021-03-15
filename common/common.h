#ifndef _COMMON_H_20130423104102
#define _COMMON_H_20130423104102

#include <string>

using std::string;
using std::wstring;

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(p)		{ if((p) != NULL) { (p)->Release(); (p)=NULL; } }
#endif

#if !defined(SAFE_DELETE)
#define SAFE_DELETE(p)		{ if((p) != NULL) { delete (p);     (p)=NULL; } }
#endif

#if !defined(SAFE_DELETEARRAY)
#define SAFE_DELETEARRAY(p)	{ if((p) != NULL) { delete [](p);	(p) = NULL;} }
#endif

// unicode转ansi
string UnicodeToAnsi(wstring in);

// ansi转unicode
wstring AnsiToUnicode(string in);

// 宽 字符转UTF8
string EncodeUtf8(wstring in);

// UTF8 转宽字符
wstring DecodeUtf8(string in);

unsigned char ToHex(unsigned char x);

unsigned char FromHex(unsigned char x);

string UrlEncode(const string& str);

string UrlDecode(const string& str);


unsigned char* CharToBin(const unsigned char *pszSrc, int nSrcLen, unsigned char *pszDest, int *nDestLen);
unsigned char* BinToChar(const unsigned char *pszSrc, int nSrcLen, unsigned char *pszDest, int *nDestLen);


#endif