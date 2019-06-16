#ifndef _FILE_AND_DIR_UTIL_H__
#define _FILE_AND_DIR_UTIL_H__

#include <string>

namespace Util
{
    /** �Ƿ����ļ���
    @param [in] path ·��
    */
    bool IsDir(const std::wstring& path);

    /** �ж��ļ��Ƿ����
    @param [in] filePath �ļ�·��
    */
    bool IsFileExist(const std::wstring& filePath);

    /** �ж��ļ����Ƿ����
    @param [in] dirPath �ļ���·��
    */
    bool IsDirExist(const std::wstring& dirPath);

    /** �ж��ļ����ļ����Ƿ����
    @param [in] path ·��
    */
    bool IsPathExist(const std::wstring& path);

    /** ɾ���ļ���
    @param [in] dfilePath �ļ�·��
    */
    bool DeleteFile_(const std::wstring& dfilePath);

    /** ɾ���ļ��� (����������)
    @param [in] dirPath �ļ���·��
    */
    bool DeleteDir(const std::wstring& dirPath);

    /** �����ļ���
    @param [in] dirPath �ļ���·��
    */
    bool CreateDir(const std::wstring& dirPath);

    /** �����ļ�
    @param [in] filePath �ļ�·��
    */
    bool CreateFile_(const std::wstring& filePath);
}

#endif // _FILE_AND_DIR_UTIL_H__