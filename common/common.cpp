#include "stdafx.h"
#include "common.h"

#include <memory.h>
#include <ctype.h>

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

void WriteLogMsg(char chLogMsg[])
{

	char strFilePath[40] = ".\\Log\\";//如果是"\\Log\\"则到了当前盘符的根目录下了。
	char strTimeFileName[20];//将当前时间转换成字符串---声明字符串长度的时候，要比实际长度多1，作为结尾符号


	SYSTEMTIME sysTime; 
	GetLocalTime( &sysTime ); //得到系统时间
	sprintf(strTimeFileName,"%d-%d-%d",sysTime.wYear,sysTime.wMonth,sysTime.wDay);//"2010-09-21"

	strcat(strTimeFileName,".logFile");//加上扩展名--登录日志
	strcat(strFilePath,strTimeFileName);//得到完整的路径名


	FILE *fp;//文件指针

	if ((fp=fopen(strFilePath,"a"))==NULL)//以追加的形式往文件中写东西
	{
		//如果打开不成功，则一般表示没有Log目录
		//创建Log目录，然后再重新打开--一般情况下，如果目录存在的话，就不会创建成功的。
		if(!CreateDirectory(_T(".\\Log\\"),NULL)) 
		{ 
			return;
		}
		else 
		{
			if ((fp=fopen(strFilePath,"a"))==NULL)//以追加的形式往文本文件中写东西
			{			
				exit(0);
			}
		} 
	}

	char strTimeTag[30]; //将时间转成字符串
	sprintf(strTimeTag,"%d-%d-%d  %d:%d:%d  ",sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond);

	fputs(strTimeTag,fp);//写入时间标记
	fputs(" : ",fp);//分隔符号
	fputs(chLogMsg,fp);//写入消息日志
	fputs("\n",fp);//换行

	fclose(fp);

}