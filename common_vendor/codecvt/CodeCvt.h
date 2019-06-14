#ifndef __CODECVT_H_
#define __CODECVT_H_


#include <string>

namespace codecvt
{
	/*
		���ַ�ת���ַ�
		wsSrc �� Դ������һ��ΪUnicode
		sRes  �� ���������һ��ΪUnicode
	*/
	bool UTF16ToUTF8(const std::wstring & wsSrc, std::string & sRes);

	/*
		���ַ�ת���ַ�������һ��ΪUNICODE�룩
		sSrc   �� Դ������һ��ΪUnicode
		wsRes  �� ���������һ��ΪUnicode
	*/
	bool UTF8ToUTF16(const std::string  & sSrc, std::wstring & wsRes);

	/*
		���ַ�ת���ַ�������һ��ΪUNICODE�룩
		wsSrc   �� Դ������һ��ΪUnicode
		sRes    �� ���������ΪANSI windows���й�����һ��ΪGBK
	*/
	bool UTF16ToANSI(const std::wstring & wsSrc, std::string & sRes);

	/*
		���ַ�ת���ַ�������һ��ΪUNICODE�룩
		sSrc    �� Դ������ΪANSI windows���й�����һ��ΪGBK
		wsRes   �� ���������һ��ΪUnicode
	*/
	bool ANSIToUTF16(const std::string  & sSrc, std::wstring & wsRes);
}

#endif




