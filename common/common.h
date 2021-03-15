#ifndef _COMMON_H_20130423104102
#define _COMMON_H_20130423104102

#include <string>
using namespace std;
    
unsigned char* CharToBin(const unsigned char *pszSrc, int nSrcLen, unsigned char *pszDest, int *nDestLen);
unsigned char* BinToChar(const unsigned char *pszSrc, int nSrcLen, unsigned char *pszDest, int *nDestLen);
string UnicodeToAnsi(wstring in);
void WriteLogMsg(char chLogMsg[]);


#endif