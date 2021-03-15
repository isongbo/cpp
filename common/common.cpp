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
            //����
            if (i%2 > 0) //iΪ����λ  ������λ
            {
                pszDest[j] |= (ch & 0x0F);
                j++;
            }
            else //iΪż��λ ������λ
            {
                pszDest[j] |= (ch & 0x0F)<<4;
            }
        }
        else
        {
            //��ĸ
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
            ch = (pszSrc[i]>>4) & 0x0F; //��λ
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
            ch = pszSrc[i] & 0x0F; //��λ
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

	char strFilePath[40] = ".\\Log\\";//�����"\\Log\\"���˵�ǰ�̷��ĸ�Ŀ¼���ˡ�
	char strTimeFileName[20];//����ǰʱ��ת�����ַ���---�����ַ������ȵ�ʱ��Ҫ��ʵ�ʳ��ȶ�1����Ϊ��β����


	SYSTEMTIME sysTime; 
	GetLocalTime( &sysTime ); //�õ�ϵͳʱ��
	sprintf(strTimeFileName,"%d-%d-%d",sysTime.wYear,sysTime.wMonth,sysTime.wDay);//"2010-09-21"

	strcat(strTimeFileName,".logFile");//������չ��--��¼��־
	strcat(strFilePath,strTimeFileName);//�õ�������·����


	FILE *fp;//�ļ�ָ��

	if ((fp=fopen(strFilePath,"a"))==NULL)//��׷�ӵ���ʽ���ļ���д����
	{
		//����򿪲��ɹ�����һ���ʾû��LogĿ¼
		//����LogĿ¼��Ȼ�������´�--һ������£����Ŀ¼���ڵĻ����Ͳ��ᴴ���ɹ��ġ�
		if(!CreateDirectory(_T(".\\Log\\"),NULL)) 
		{ 
			return;
		}
		else 
		{
			if ((fp=fopen(strFilePath,"a"))==NULL)//��׷�ӵ���ʽ���ı��ļ���д����
			{			
				exit(0);
			}
		} 
	}

	char strTimeTag[30]; //��ʱ��ת���ַ���
	sprintf(strTimeTag,"%d-%d-%d  %d:%d:%d  ",sysTime.wYear,sysTime.wMonth,sysTime.wDay,
		sysTime.wHour,sysTime.wMinute,sysTime.wSecond);

	fputs(strTimeTag,fp);//д��ʱ����
	fputs(" : ",fp);//�ָ�����
	fputs(chLogMsg,fp);//д����Ϣ��־
	fputs("\n",fp);//����

	fclose(fp);

}