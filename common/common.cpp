#include "StdAfx.h"
#include "common.h"

#include <Windows.h>
#include <memory.h>
#include <ctype.h>
#include <assert.h>



// unicode转ansi
string UnicodeToAnsi(wstring in)
{
    string s(in.length()*2+1,' ' );
    size_t len = ::WideCharToMultiByte(CP_ACP, 0,
        in.c_str(), in.length(),
        &s[0], s.length(),
        NULL, NULL);
    s.resize(len);
    return s;
}

// ansi转unicode
wstring AnsiToUnicode(string in)
{
    wstring s(in.length(), _T(' ' ));
    size_t len = ::MultiByteToWideChar(CP_ACP, 0,
        in.c_str(), in.length(),
        &s[0], s.length());
    s.resize(len);
    return s;
}

// 宽 字符转UTF8
string EncodeUtf8(wstring in)
{
    string s(in.length()*3+1,' ' );
    size_t len = ::WideCharToMultiByte(CP_UTF8, 0,
        in.c_str(), in.length(),
        &s[0], s.length(),
        NULL, NULL);
    s.resize(len);
    return s;
}

// UTF8 转宽字符
wstring DecodeUtf8(string in)
{
    wstring s(in.length(), _T(' ' ));
    size_t len = ::MultiByteToWideChar(CP_UTF8, 0,
        in.c_str(), in.length(),
        &s[0], s.length());
    s.resize(len);
    return s;
}

unsigned char ToHex(unsigned char x)
{
	return x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x)
{
	unsigned char y;
	if ( x >= 'A' && x <= 'Z')
	{
		y = x - 'A' + 10;
	}
	else if ( x >= 'a' && x <= 'z')
	{
		y = x - 'a' + 10;
	}
	else if (x >= '0' && x <= '9')
	{
		y = x - '0';  
	}
	else
	{
		assert(0);
	}
	return y;
}

string UrlEncode(const string& str)
{
	string strTemp = "";
	size_t length = str.length();
	for ( size_t i = 0;i < length;i++ )
	{
		if ( isalnum((unsigned char)str[i]) || (str[i] == '-') || (str[i] == '_') || (str[i] == '.') || (str[i] == '~') )
		{
			strTemp += str[i];
		}
		else if ( str[i] == ' ')
		{
			strTemp += "+";
		}
		else
		{
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}

string UrlDecode(const string& str)
{
	string strTemp = "";
	size_t length = str.length();
	for ( size_t i = 0;i < length;i++ )
	{
		if ( str[i] == '+' )
		{
			strTemp += ' ';
		}
		else if ( str[i] == '%' )
		{
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low  = FromHex((unsigned char)str[++i]);
			strTemp += high*16 + low;
		}
		else 
		{
			strTemp += str[i];
		}
	}
	return strTemp;
}



unsigned char* CharToBin(const unsigned char *pszSrc, int nSrcLen, unsigned char *pszDest, int *nDestLen)
{
	memset(pszDest, 0, *nDestLen);

	if (nSrcLen/2 > *nDestLen)
		return pszDest;

	unsigned char ch;
	int i=0;
	int j=0;

	for (i=0; i<nSrcLen; i++)
	{
		ch = pszSrc[i];

		if (isdigit(ch)>0)
		{
			//数字
			if (i%2 > 0) //i为奇数位  放至低位
			{
				pszDest[j] |= (ch & 0x0F);
				j++;
			}
			else //i为偶数位 放至高位
			{
				pszDest[j] |= (ch & 0x0F)<<4;
			}
		}
		else
		{
			//字母
			ch = toupper(ch);
			if (i%2 > 0)
			{
				pszDest[j] |= ((ch & 0x0F) + 9);
				j++;
			}
			else
			{
				pszDest[j] |= ((ch & 0x0F) + 9)<<4;
			}
		}
	}

	*nDestLen = j;

	return pszDest;
}


unsigned char* BinToChar(const unsigned char *pszSrc, int nSrcLen, unsigned char *pszDest, int *nDestLen)
{
	memset(pszDest, 0, *nDestLen);

	if (nSrcLen*2 > *nDestLen)
		return pszDest;

	unsigned char ch;
	int i=0;
	int j=0;

	for (i=0; i<nSrcLen; j++)
	{
		ch = pszSrc[i];
		if (j%2 == 0)
		{
			ch = (pszSrc[i]>>4) & 0x0F; //高位
			if ((ch>=0) && (ch <=9))
			{
				pszDest[j] = ch | 0x30 ;
			}
			else
			{
				pszDest[j] = (ch - 9) | 0x40;
			}
		}
		else
		{
			ch = pszSrc[i] & 0x0F; //低位
			if ((ch>=0) && (ch <=9))
			{
				pszDest[j] = ch | 0x30 ;
			}
			else
			{
				pszDest[j] = (ch - 9) | 0x40;
			}

			i++;
		}
	}

	*nDestLen = j;

	return pszDest;
}