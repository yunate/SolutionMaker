#ifndef __CODECVT_H_
#define __CODECVT_H_


#include <string>

namespace codecvt
{
    /** ���ַ�ת���ַ�,��std����ת����ע�����ת������ԭ���ԣ�Ҳ����˵���̼߳����ǲ��е�
    @param [in] wsSrc Դ������һ��ΪUnicode
    @param [in] sRes ���������һ��ΪUnicode
    @return �Ƿ�ɹ�
    */
    bool UTF16ToUTF8_STD(const std::wstring & wsSrc, std::string & sRes);

    /** ���ַ�ת���ַ�
    @param [in] wsSrc Դ������һ��ΪUnicode
    @param [in] sRes ���������һ��ΪUnicode
    @return �Ƿ�ɹ�
    */
    bool UTF16ToUTF8_Multi(const std::wstring & wsSrc, std::string & sRes);

    /** ���ַ�ת���ַ�������һ��ΪUNICODE�룩��ע�����ת������ԭ���ԣ�Ҳ����˵���̼߳����ǲ��е�
    @param [in] wsSrc Դ������һ��ΪUnicode
    @param [in] sRes ���������һ��ΪUnicode
    @return �Ƿ�ɹ�
    */
    bool UTF8ToUTF16_STD(const std::string  & sSrc, std::wstring & wsRes);

    /** ���ַ�ת���ַ�
    @param [in] wsSrc Դ������һ��ΪUnicode
    @param [in] sRes ���������һ��ΪUnicode
    @return �Ƿ�ɹ�
    */
    bool UTF8ToUTF16_Multi(const std::string  & sSrc, std::wstring & wsRes);

    /** ���ַ�תANSI������һ��ΪUNICODE�룩
    @param [in] wsSrc Դ������һ��ΪUnicode
    @param [in] sRes ���������ΪANSI windows���й�����һ��ΪGBK
    @return �Ƿ�ɹ�
    */
    bool UTF16ToANSI(const std::wstring & wsSrc, std::string & sRes);

    /** ANSIת���ַ�������һ��ΪUNICODE�룩
    @param [in] wsSrc Դ������ΪANSI windows���й�����һ��ΪGBK
    @param [in] sRes ���������һ��ΪUnicode
    @return �Ƿ�ɹ�
    */
    bool ANSIToUTF16(const std::string  & sSrc, std::wstring & wsRes);
}

#endif




