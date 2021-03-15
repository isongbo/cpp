#include "function.h"
#include "ManFrm.h"
#include "QR_Encode.h"
#include <STRING.H>

char* Utf8ToAnsi(const char* utf, char* ansi)
{
    int unicodeLen=MultiByteToWideChar(CP_UTF8,0,utf,strlen(utf),NULL,0);
    wchar_t* pUnicode=new wchar_t[unicodeLen+1];
    memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8,0,utf,strlen(utf),(LPWSTR)pUnicode,unicodeLen);
    WideCharToMultiByte(CP_ACP,0,(LPWSTR)pUnicode,unicodeLen,(char*)ansi,1024,NULL,NULL);
    delete pUnicode;
    return ansi;
}
char* AnsiToUtf8(const char* ansi, char* utf)
{
    int unicodeLen=MultiByteToWideChar(CP_ACP,0,ansi,strlen(ansi),NULL,0);
    wchar_t* pUnicode=new wchar_t[unicodeLen+1];
    memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));
    MultiByteToWideChar(CP_ACP,0,ansi,strlen(ansi),(LPWSTR)pUnicode,unicodeLen);
    WideCharToMultiByte(CP_UTF8,0,(LPWSTR)pUnicode,unicodeLen,(char*)utf,1024,NULL,NULL);
    delete pUnicode;
    return utf;
}
void encode(char * Content,char * route_picture_name)
{
	char buf1[1024]={0};
	char buf2[1024]={0};
	
	AnsiToUtf8(Content,buf1);
	strcpy(buf2,route_picture_name);
	CMainFrame * pmainframe = new CMainFrame;	
	pmainframe-> ShowImage(buf1);
	pmainframe-> SaveImage(buf2);
	delete pmainframe;

}