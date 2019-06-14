#ifndef __CODECVT_H_
#define __CODECVT_H_


#include <string>

namespace codecvt
{
	/*
		宽字符转多字符
		wsSrc ： 源，编码一般为Unicode
		sRes  ： 结果，编码一般为Unicode
	*/
	bool UTF16ToUTF8(const std::wstring & wsSrc, std::string & sRes);

	/*
		多字符转宽字符（编码一般为UNICODE码）
		sSrc   ： 源，编码一般为Unicode
		wsRes  ： 结果，编码一般为Unicode
	*/
	bool UTF8ToUTF16(const std::string  & sSrc, std::wstring & wsRes);

	/*
		宽字符转多字符（编码一般为UNICODE码）
		wsSrc   ： 源，编码一般为Unicode
		sRes    ： 结果，编码为ANSI windows下中国编码一般为GBK
	*/
	bool UTF16ToANSI(const std::wstring & wsSrc, std::string & sRes);

	/*
		多字符转宽字符（编码一般为UNICODE码）
		sSrc    ： 源，编码为ANSI windows下中国编码一般为GBK
		wsRes   ： 结果，编码一般为Unicode
	*/
	bool ANSIToUTF16(const std::string  & sSrc, std::wstring & wsRes);
}

#endif




