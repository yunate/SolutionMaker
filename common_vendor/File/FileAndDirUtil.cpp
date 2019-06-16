#include "FileAndDirUtil.h"

#include <Windows.h>
#include <tchar.h>

bool Util::IsDir(const std::wstring & path)
{
    DWORD ftyp = ::GetFileAttributes(path.c_str());

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    {
        // ����һ���ļ���
        return true;
    }

    return false;
}

bool Util::IsFileExist(const std::wstring& filePath)
{
    DWORD dwAttrib = ::GetFileAttributes(filePath.c_str());
    return (INVALID_FILE_ATTRIBUTES != dwAttrib) && (0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool Util::IsDirExist(const std::wstring& dirPath)
{
    DWORD dwAttrib = ::GetFileAttributes(dirPath.c_str());
    return (INVALID_FILE_ATTRIBUTES != dwAttrib) && (0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool Util::IsPathExist(const std::wstring& path)
{
    DWORD dwAttrib = ::GetFileAttributes(path.c_str());
    return INVALID_FILE_ATTRIBUTES != dwAttrib;
}

bool Util::DeleteFile_(const std::wstring& dfilePath)
{
    return (::DeleteFile(dfilePath.c_str()) == TRUE);
}

bool Util::DeleteDir(const std::wstring & dirPath)
{
    if (!IsDirExist(dirPath))
    {
        return false;
    }

    HANDLE hFile = NULL;
    WIN32_FIND_DATA fileInfo;
    ::memset(&fileInfo, 0, sizeof(LPWIN32_FIND_DATA));

    std::wstring wsTemp(dirPath);
    if (_T('\\') != wsTemp[wsTemp.length() - 1] && _T('/') != wsTemp[wsTemp.length() - 1])
    {
        wsTemp.append(_T("\\"));
    }

    std::wstring rootPath(wsTemp);
    wsTemp.append(_T("*"));
    hFile = ::FindFirstFile(wsTemp.c_str(), &fileInfo);

    if (INVALID_HANDLE_VALUE == hFile)
    {
        return false;
    }

    do
    {
        //����ǵ�ǰĿ¼�������ϼ�Ŀ¼����ֱ�ӽ�����һ��ѭ��  
        if (wcscmp(fileInfo.cFileName, _T(".")) == 0 ||
			wcscmp(fileInfo.cFileName, _T("..")) == 0)
        {
            continue;
        }

        std::wstring subPath = rootPath + fileInfo.cFileName;
        if (IsDir(subPath))
        {
            if (!DeleteDir(subPath))
            {
                return false;
            }

			::RemoveDirectory(rootPath.c_str());
		}
        else
        {
            if (!DeleteFile_(subPath))
            {
                return false;
            }
        }

    } while (::FindNextFile(hFile, &fileInfo));

    ::FindClose(hFile);
    return true;
}

bool Util::CreateDir(const std::wstring& dirPath)
{
	if (IsDirExist(dirPath))
	{
		return true;
	}

    return ::CreateDirectory(dirPath.c_str(), NULL) == TRUE;
}

bool Util::CreateFile_(const std::wstring& filePath)
{
    DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
    DWORD dwShareMode = 0;
    LPSECURITY_ATTRIBUTES lpSecurityAttributes = 0;
    DWORD dwCreationDisposition = CREATE_NEW;
    DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE hTemplateFile = 0;
    HANDLE handle = CreateFile(filePath.c_str(),
                               dwDesiredAccess,
                               dwShareMode,
                               lpSecurityAttributes,
                               dwCreationDisposition,
                               dwFlagsAndAttributes,
                               hTemplateFile);

    if (handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    ::CloseHandle(handle);
    return true;
}
