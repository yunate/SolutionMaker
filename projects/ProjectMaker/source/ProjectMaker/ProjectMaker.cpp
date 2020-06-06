#include "ProjectMaker.h"
#include "file/FileAndDirUtil.h"
#include "GUID/GUID.h"
#include "codecvt/code_cvt.h"
#include <iostream>
#include <vector>
#include <Windows.h>

template<typename T, unsigned int N>
char(&array_size_fake_func(T(&)[N]))[N];
#define MYARRAYSIZE(A) sizeof(array_size_fake_func(A))

ProjectMaker::ProjectMaker(const ProjectProperty& m_projectProperty,
                           const std::wstring rootDir,
                           bool bRemoveDirIfExit) :
    m_projectProperty(m_projectProperty),
    m_rootDir(rootDir),
    m_bRemoveDirIfExit(bRemoveDirIfExit)
{
}

ProjectMaker::~ProjectMaker()
{
}

bool ProjectMaker::MakeProject()
{
    codecvt::UTF8ToUTF16_Multi(Util::GenerateGuid(), m_ProjectGUIDStr);

    if (!PrepareMakeDir())
    {
        std::cout << "PrepareMakeDir() failure" <<std::endl;
        return false;
    }

    if (!MakeDir())
    {
        std::cout << "MakeDir() failure" << std::endl;
        return false;
    }

    if (!MakeProjFile())
    {
        std::cout << "MakeProjFile() failure" << std::endl;
        return false;
    }

    if (!MakeFiltersFile())
    {
        std::cout << "MakeFiltersFile() failure" << std::endl;
        return false;
    }

    if (!MakeSlnFile())
    {
        std::cout << "MakeSlnFile() failure" << std::endl;
        return false;
    }

    if (!MakeSrcFile())
    {
        std::cout << "MakeSrcFile() failure" << std::endl;
        return false;
    }

    std::cout << "success" << std::endl;
    return true;
}

bool ProjectMaker::PrepareMakeDir()
{
    if (!Util::IsDirExist(m_rootDir))
    {
        return false;
    }

    m_projectDir = m_rootDir + _T("\\");
    m_projectDir += m_projectProperty.m_projectName;
    m_projectDir += _T("\\");

    if (Util::IsDirExist(m_projectDir))
    {
        if (!m_bRemoveDirIfExit)
        {
            return false;
        }

        // 删除文件夹
        if (!Util::DeleteDir(m_projectDir))
        {
            return false;
        }
    }

    if (!Util::CreateDir(m_projectDir))
    {
        return false;
    }

    return true;
}

bool ProjectMaker::MakeDir()
{
    // m_projectDir 下
    {
        std::vector<std::wstring> subDirName { _T("build"), _T("doc"), _T("res"), _T("src"), _T("test"), _T("vendor") };

        for (int i = 0; i < subDirName.size(); ++i)
        {
            if (!Util::CreateDir(m_projectDir + subDirName[i] + _T("\\")))
            {
                return false;
            }
        }
    }

    // m_rootDir 下
    {
        std::vector<std::wstring> subDirName{ _T("bin") };

        for (int i = 0; i < subDirName.size(); ++i)
        {
            if (!Util::CreateDir(m_rootDir + subDirName[i] + _T("\\")))
            {
                return false;
            }
        }
    }

    return true;
}

std::wstring& replace_str(std::wstring& str, const std::wstring& to_replaced, const std::wstring& newchars)
{
    std::wstring::size_type pos(0);

    for ( ; ; )
    {
        pos = str.find(to_replaced, pos);

        if (pos != std::wstring::npos)
        {
            str.replace(pos, to_replaced.length(), newchars);
            pos += newchars.length();
        }
        else
        {
            break;
        }
    }

    return str;
}

bool ProjectMaker::MakeProjFile()
{
    std::wstring filePath = m_projectDir + _T("build\\") + m_projectProperty.m_projectName + _T(".vcxproj");

    if (!Util::CreateFile_(filePath))
    {
        return false;
    }

    std::wstring templateStr = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
        "<Project DefaultTargets=\"Build\" ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\r\n"
        "  <ItemGroup Label=\"ProjectConfigurations\">\r\n"
        "    <ProjectConfiguration Include=\"Debug|Win32\">\r\n"
        "      <Configuration>Debug</Configuration>\r\n"
        "      <Platform>Win32</Platform>\r\n"
        "    </ProjectConfiguration>\r\n"
        "    <ProjectConfiguration Include=\"Release|Win32\">\r\n"
        "      <Configuration>Release</Configuration>\r\n"
        "      <Platform>Win32</Platform>\r\n"
        "    </ProjectConfiguration>\r\n"
        "    <ProjectConfiguration Include=\"Debug|x64\">\r\n"
        "      <Configuration>Debug</Configuration>\r\n"
        "      <Platform>x64</Platform>\r\n"
        "    </ProjectConfiguration>\r\n"
        "    <ProjectConfiguration Include=\"Release|x64\">\r\n"
        "      <Configuration>Release</Configuration>\r\n"
        "      <Platform>x64</Platform>\r\n"
        "    </ProjectConfiguration>\r\n"
        "  </ItemGroup>\r\n"
        "  <PropertyGroup Label=\"Globals\">\r\n"
        "    <ProjectGuid>PROJECT_GUID</ProjectGuid>\r\n"
        "    <RootNamespace>PROJECTNAME1</RootNamespace>\r\n"
        "  </PropertyGroup>\r\n"
        "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />\r\n"
        "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"Configuration\">\r\n"
        "    <ConfigurationType>CONFIGURATION_TYPE</ConfigurationType>\r\n"
        "    <UseDebugLibraries>true</UseDebugLibraries>\r\n"
        "    <CharacterSet>CHARACTER_SET</CharacterSet>\r\n"
        "  </PropertyGroup>\r\n"
        "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"Configuration\">\r\n"
        "    <ConfigurationType>CONFIGURATION_TYPE</ConfigurationType>\r\n"
        "    <UseDebugLibraries>false</UseDebugLibraries>\r\n"
        "    <WholeProgramOptimization>true</WholeProgramOptimization>\r\n"
        "    <CharacterSet>CHARACTER_SET</CharacterSet>\r\n"
        "  </PropertyGroup>\r\n"
        "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\" Label=\"Configuration\">\r\n"
        "    <ConfigurationType>CONFIGURATION_TYPE</ConfigurationType>\r\n"
        "    <UseDebugLibraries>true</UseDebugLibraries>\r\n"
        "    <CharacterSet>CHARACTER_SET</CharacterSet>\r\n"
        "  </PropertyGroup>\r\n"
        "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\" Label=\"Configuration\">\r\n"
        "    <ConfigurationType>CONFIGURATION_TYPE</ConfigurationType>\r\n"
        "    <UseDebugLibraries>false</UseDebugLibraries>\r\n"
        "    <WholeProgramOptimization>true</WholeProgramOptimization>\r\n"
        "    <CharacterSet>CHARACTER_SET</CharacterSet>\r\n"
        "  </PropertyGroup>\r\n"
        "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\r\n"
        "  <ImportGroup Label=\"ExtensionSettings\">\r\n"
        "  </ImportGroup>\r\n"
        "  <ImportGroup Label=\"Shared\">\r\n"
        "    USER_PROPS"
        "  </ImportGroup>\r\n"
        "  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">\r\n"
        "    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\r\n"
        "  </ImportGroup>\r\n"
        "  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">\r\n"
        "    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\r\n"
        "  </ImportGroup>\r\n"
        "  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">\r\n"
        "    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\r\n"
        "  </ImportGroup>\r\n"
        "  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">\r\n"
        "    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />\r\n"
        "  </ImportGroup>\r\n"
        "  <PropertyGroup Label=\"UserMacros\" />\r\n"
        "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">\r\n"
        "    <OutDir>$(SolutionDir)bin\\$(Configuration)_$(PlatForm)\\</OutDir>\r\n"
        "    <IntDir>$(OutDir)tmp\\$(ProjectName)</IntDir>\r\n"
        "    <IncludePath>$(ProjectDir)..\\src\\;$(ProjectDir)..\\vendor\\;$(IncludePath)</IncludePath>\r\n"
        "    <LibraryPath>$(ProjectDir)..\\vendor\\;$(LibraryPath)</LibraryPath>\r\n"
        "  </PropertyGroup>\r\n"
        "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">\r\n"
        "    <OutDir>$(SolutionDir)bin\\$(Configuration)_$(PlatForm)\\</OutDir>\r\n"
        "    <IntDir>$(OutDir)tmp\\$(ProjectName)</IntDir>\r\n"
        "    <IncludePath>$(ProjectDir)..\\src\\;$(ProjectDir)..\\vendor\\;$(IncludePath)</IncludePath>\r\n"
        "    <LibraryPath>$(ProjectDir)..\\vendor\\;$(LibraryPath)</LibraryPath>\r\n"
        "  </PropertyGroup>\r\n"
        "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">\r\n"
        "    <OutDir>$(SolutionDir)bin\\$(Configuration)_$(PlatForm)\\</OutDir>\r\n"
        "    <IntDir>$(OutDir)tmp\\$(ProjectName)</IntDir>\r\n"
        "    <IncludePath>$(ProjectDir)..\\src\\;$(ProjectDir)..\\vendor\\;$(IncludePath)</IncludePath>\r\n"
        "    <LibraryPath>$(ProjectDir)..\\vendor\\;$(LibraryPath)</LibraryPath>\r\n"
        "  </PropertyGroup>\r\n"
        "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">\r\n"
        "    <OutDir>$(SolutionDir)bin\\$(Configuration)_$(PlatForm)\\</OutDir>\r\n"
        "    <IntDir>$(OutDir)tmp\\$(ProjectName)</IntDir>\r\n"
        "    <IncludePath>$(ProjectDir)..\\src\\;$(ProjectDir)..\\vendor\\;$(IncludePath)</IncludePath>\r\n"
        "    <LibraryPath>$(ProjectDir)..\\vendor\\;$(LibraryPath)</LibraryPath>\r\n"
        "  </PropertyGroup>\r\n"
        "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">\r\n"
        "    <ClCompile>\r\n"
        "      <WarningLevel>Level3</WarningLevel>\r\n"
        "      <Optimization>Disabled</Optimization>\r\n"
        "      <SDLCheck>true</SDLCheck>\r\n"
        "      <ConformanceMode>false</ConformanceMode>\r\n"
        "      <RuntimeLibrary>RUNTIME_LIBRARY_TYPE_DEBUG</RuntimeLibrary>\r\n"
        "    </ClCompile>\r\n"
        "  </ItemDefinitionGroup>\r\n"
        "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">\r\n"
        "    <ClCompile>\r\n"
        "      <WarningLevel>Level3</WarningLevel>\r\n"
        "      <Optimization>Disabled</Optimization>\r\n"
        "      <SDLCheck>true</SDLCheck>\r\n"
        "      <ConformanceMode>false</ConformanceMode>\r\n"
        "      <RuntimeLibrary>RUNTIME_LIBRARY_TYPE_DEBUG</RuntimeLibrary>\r\n"
        "    </ClCompile>\r\n"
        "  </ItemDefinitionGroup>\r\n"
        "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">\r\n"
        "    <ClCompile>\r\n"
        "      <WarningLevel>Level3</WarningLevel>\r\n"
        "      <Optimization>MaxSpeed</Optimization>\r\n"
        "      <FunctionLevelLinking>true</FunctionLevelLinking>\r\n"
        "      <IntrinsicFunctions>true</IntrinsicFunctions>\r\n"
        "      <SDLCheck>true</SDLCheck>\r\n"
        "      <ConformanceMode>false</ConformanceMode>\r\n"
        "      <RuntimeLibrary>RUNTIME_LIBRARY_TYPE_RELEASE</RuntimeLibrary>\r\n"
        "    </ClCompile>\r\n"
        "    <Link>\r\n"
        "      <EnableCOMDATFolding>true</EnableCOMDATFolding>\r\n"
        "      <OptimizeReferences>true</OptimizeReferences>\r\n"
        "    </Link>\r\n"
        "  </ItemDefinitionGroup>\r\n"
        "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">\r\n"
        "    <ClCompile>\r\n"
        "      <WarningLevel>Level3</WarningLevel>\r\n"
        "      <Optimization>MaxSpeed</Optimization>\r\n"
        "      <FunctionLevelLinking>true</FunctionLevelLinking>\r\n"
        "      <IntrinsicFunctions>true</IntrinsicFunctions>\r\n"
        "      <SDLCheck>true</SDLCheck>\r\n"
        "      <ConformanceMode>false</ConformanceMode>\r\n"
        "      <RuntimeLibrary>RUNTIME_LIBRARY_TYPE_RELEASE</RuntimeLibrary>\r\n"
        "    </ClCompile>\r\n"
        "    <Link>\r\n"
        "      <EnableCOMDATFolding>true</EnableCOMDATFolding>\r\n"
        "      <OptimizeReferences>true</OptimizeReferences>\r\n"
        "    </Link>\r\n"
        "  </ItemDefinitionGroup>\r\n"
        "  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.targets\" />\r\n"
        "  <ImportGroup Label=\"ExtensionTargets\">\r\n"
        "  </ImportGroup>\r\n"
        "</Project>");

    //
    std::wstring configurationStr = _T("");
    if (m_projectProperty.m_configurationType == DLL)
    {
        configurationStr = _T("DynamicLibrary");
    }
    else if (m_projectProperty.m_configurationType == EXE)
    {
        configurationStr = _T("Application");
    }
    else if (m_projectProperty.m_configurationType == LIB)
    {
        configurationStr = _T("StaticLibrary");
    }

    //
    std::wstring characterSetStr = _T("");
    if (m_projectProperty.m_characterSet == UNICODE_TYPE)
    {
        characterSetStr = _T("Unicode");
    }
    else if (m_projectProperty.m_characterSet == MULTI_BYTE)
    {
        characterSetStr = _T("MultiByte");
    }

    std::wstring runtimeLibraryDbgStr = _T("");
    std::wstring runtimeLibraryStr = _T("");
    if (m_projectProperty.m_runtimeLibraryType == STATIC)
    {
        runtimeLibraryDbgStr = _T("MultiThreadedDebug");
        runtimeLibraryStr = _T("MultiThreaded");
    }
    else if (m_projectProperty.m_runtimeLibraryType == DYNAMIC)
    {
        runtimeLibraryDbgStr = _T("MultiThreadedDebugDLL");
        runtimeLibraryStr = _T("MultiThreadedDLL");
    }

    std::wstring userProps = _T("");
    if (m_projectProperty.m_userProps != _T(""))
    {
        userProps = _T("<Import Project=\"$(SolutionDir)") + m_projectProperty.m_userProps + _T("\" />");
    }

    std::wstring projectNameStr = m_projectProperty.m_projectName;
    replace_str(templateStr, _T("CONFIGURATION_TYPE"), configurationStr);
    replace_str(templateStr, _T("CHARACTER_SET"), characterSetStr);
    replace_str(templateStr, _T("RUNTIME_LIBRARY_TYPE_DEBUG"), runtimeLibraryDbgStr);
    replace_str(templateStr, _T("RUNTIME_LIBRARY_TYPE_RELEASE"), runtimeLibraryStr);
    replace_str(templateStr, _T("PROJECTNAME"), projectNameStr);
    replace_str(templateStr, _T("PROJECT_GUID"), m_ProjectGUIDStr);
    replace_str(templateStr, _T("USER_PROPS"), userProps);
    std::string utf8Str = "";
    codecvt::UTF16ToUTF8_Multi(templateStr, utf8Str);

    FILE* pFile = NULL;
    ::_wfopen_s(&pFile, filePath.c_str(), _T("wb"));

    if (pFile == NULL)
    {
        return false;
    }

    char head[] = { (char)0xef, (char)0xbb, (char)0xbf };
    ::fwrite(head, 1, ARRAYSIZE(head), pFile);
    ::fwrite(utf8Str.c_str(), 1, utf8Str.length(), pFile);
    ::fclose(pFile);
    return true;
}

bool ProjectMaker::MakeFiltersFile()
{
    std::wstring filePath = m_projectDir + _T("build\\") + m_projectProperty.m_projectName + _T(".vcxproj.filters");

    if (!Util::CreateFile_(filePath))
    {
        return false;
    }

    std::wstring templateStr = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
        "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\r\n"
        "  <ItemGroup>\r\n"
        "    <Filter Include=\"src\">\r\n"
        "      <UniqueIdentifier>CODE_GUID</UniqueIdentifier>\r\n"
        "    </Filter>\r\n"
        "    <Filter Include=\"res\">\r\n"
        "      <UniqueIdentifier>RESOURCES_GUID</UniqueIdentifier>\r\n"
        "    </Filter>\r\n"
        "    <Filter Include=\"vendor\">\r\n"
        "      <UniqueIdentifier>VENDOR_GUID</UniqueIdentifier>\r\n"
        "    </Filter>\r\n"
        "  </ItemGroup>\r\n"
        "</Project>");

    // 替换
    std::wstring sCODE_GUID = _T("");
    std::wstring sRESOURCES_GUID = _T("");
    std::wstring sVENDOR_GUID = _T("");
    codecvt::UTF8ToUTF16_Multi(Util::GenerateGuid(), sCODE_GUID);
    codecvt::UTF8ToUTF16_Multi(Util::GenerateGuid(), sRESOURCES_GUID);
    codecvt::UTF8ToUTF16_Multi(Util::GenerateGuid(), sVENDOR_GUID);
    replace_str(templateStr, _T("CODE_GUID"), sCODE_GUID);
    replace_str(templateStr, _T("RESOURCES_GUID"), sRESOURCES_GUID);
    replace_str(templateStr, _T("VENDOR_GUID"), sVENDOR_GUID);
    replace_str(templateStr, _T("PROJECTNAME"), m_projectProperty.m_projectName);

    // 转码
    std::string utf8Str = "";
    codecvt::UTF16ToUTF8_Multi(templateStr, utf8Str);

    // 写文件
    FILE* pFile = NULL;
    ::_wfopen_s(&pFile, filePath.c_str(), _T("wb"));

    if (pFile == NULL)
    {
        return false;
    }

    char head[] = {(char)0xef, (char)0xbb, (char)0xbf };
    ::fwrite(head, 1, ARRAYSIZE(head), pFile);
    ::fwrite(utf8Str.c_str(), 1, utf8Str.length(), pFile);
    ::fclose(pFile);
    return true;
}

bool ProjectMaker::MakeSlnFile()
{
    char head[] = { (char)0xef, (char)0xbb, (char)0xbf };
    std::wstring filePath = m_rootDir + m_projectProperty.m_slnName + _T(".sln");
    std::wstring templateStr;

    if (!Util::IsFileExist(filePath)) 
    {
        if (!Util::CreateFile_(filePath))
        {
            return false;
        }

        templateStr = _T("Microsoft Visual Studio Solution File, Format Version 12.00\r\n"
                         "# Visual Studio Version 16\r\n"
                         "VisualStudioVersion = 16.0.29613.14\r\n"
                         "MinimumVisualStudioVersion = 10.0.40219.1\r\n"
                         "Global\r\n"
                         "  GlobalSection(SolutionProperties) = preSolution\r\n"
                         "    HideSolutionNode = FALSE\r\n"
                         "  EndGlobalSection\r\n"
                         "  GlobalSection(ExtensibilityGlobals) = postSolution\r\n"
                         "    SolutionGuid = SLN_GUID\r\n"
                         "  EndGlobalSection\r\n"
                         "EndGlobal\r\n");

        // 替换
        std::wstring sSLN_GUID = _T("");
        codecvt::UTF8ToUTF16_Multi(Util::GenerateGuid(), sSLN_GUID);
        replace_str(templateStr, _T("SLN_GUID"), sSLN_GUID);
    }
    else
    {
        // 读文件
        FILE* pFile = NULL;
        ::_wfopen_s(&pFile, filePath.c_str(), _T("rb"));

        if (pFile == NULL)
        {
            return false;
        }

        bool readRes = false;
        do 
        {
            ::fseek(pFile, 0, SEEK_END);
            int fileLen = ::ftell(pFile);

            if (fileLen < ARRAYSIZE(head))
            {
                break;
            }

            ::fseek(pFile, 0, SEEK_SET);
            std::vector<char> buff(fileLen);
            ::fread(buff.data(), 1, fileLen, pFile);

            for (int i = 0; i < ARRAYSIZE(head); ++i)
            {
                if (head[i] != buff[i])
                {
                    break;
                }
            }

            std::string tmp;
            tmp.assign(buff.begin() + ARRAYSIZE(head), buff.end());
            codecvt::UTF8ToUTF16_Multi(tmp, templateStr);
            readRes = true;
        } while (0);

        ::fclose(pFile);

        if (!readRes)
        {
            return false;
        }
    }

    // 构建.vcxproj字符
    std::wstring sSLN_PROJ_GUID = _T("");
    codecvt::UTF8ToUTF16_Multi(Util::GenerateGuid(), sSLN_PROJ_GUID);
    std::wstring proStr = _T("Project(\"SLN_PROJ_GUID\") = \"PROJECTNAME\", \"PROJEC_SUB_DIR\", \"PRO_GUID\"\r\nEndProject\r\n");
    replace_str(proStr, _T("SLN_PROJ_GUID"), sSLN_PROJ_GUID);
    replace_str(proStr, _T("PROJECTNAME"), m_projectProperty.m_projectName);
    std::wstring proSubDir = m_projectProperty.m_projectName + _T("\\build\\") + m_projectProperty.m_projectName + _T(".vcxproj");
    replace_str(proStr, _T("PROJEC_SUB_DIR"), proSubDir);
    replace_str(proStr, _T("PRO_GUID"), m_ProjectGUIDStr);

    // 将.vcxproj加到.sln中去
    size_t pos = templateStr.find(_T("Global"));
    if (pos == std::wstring::npos)
    {
        return false;
    }

    if (pos > templateStr.find(m_projectProperty.m_projectName))
    {
        std::wcout << _T("the ") << m_projectProperty.m_projectName << _T("has already exits") << std::endl;
        return false;
    }

    templateStr.insert(pos, proStr);

    // 写文件
    FILE* pFile = NULL;
    ::_wfopen_s(&pFile, filePath.c_str(), _T("wb"));

    if (pFile == NULL)
    {
        return false;
    }

    // 转码
    std::string utf8Str = "";
    codecvt::UTF16ToUTF8_Multi(templateStr, utf8Str);
    ::fwrite(head, 1, ARRAYSIZE(head), pFile);
    ::fwrite(utf8Str.c_str(), 1, utf8Str.length(), pFile);
    ::fclose(pFile);
    return true;
}

bool ProjectMaker::MakeSrcFile()
{
    MakeUserPropsFile();
    return true;
}

bool ProjectMaker::MakeUserPropsFile()
{
    if (m_projectProperty.m_userProps.length() == 0) 
    {
        return true;
    }

    std::wstring filePath = m_rootDir + m_projectProperty.m_userProps;

    if (Util::IsFileExist(filePath))
    {
        return true;
    }

    if (!Util::CreateFile_(filePath))
    {
        return false;
    }

    std::wstring templateStr = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
                                  "<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\r\n"
                                  "  <ImportGroup Label=\"PropertySheets\" />\r\n"
                                  "  <PropertyGroup Label=\"UserMacros\">\r\n"
                                  "    <AppName>SimpleApp</AppName>\r\n"
                                  "  </PropertyGroup>\r\n"
                                  "  <PropertyGroup />\r\n"
                                  "  <ItemDefinitionGroup />\r\n"
                                  "  <ItemGroup>\r\n"
                                  "    <BuildMacro Include=\"AppName\">\r\n"
                                  "      <Value>$(AppName)</Value>\r\n"
                                  "    </BuildMacro>\r\n"
                                  "  </ItemGroup>\r\n"
                                  "</Project>\r\n");

    // 替换
    std::wstring sCODE_GUID = _T("");
    std::wstring sRESOURCES_GUID = _T("");
    std::wstring sVENDOR_GUID = _T("");
    codecvt::UTF8ToUTF16_Multi(Util::GenerateGuid(), sCODE_GUID);
    codecvt::UTF8ToUTF16_Multi(Util::GenerateGuid(), sRESOURCES_GUID);
    codecvt::UTF8ToUTF16_Multi(Util::GenerateGuid(), sVENDOR_GUID);
    replace_str(templateStr, _T("CODE_GUID"), sCODE_GUID);
    replace_str(templateStr, _T("RESOURCES_GUID"), sRESOURCES_GUID);
    replace_str(templateStr, _T("VENDOR_GUID"), sVENDOR_GUID);
    replace_str(templateStr, _T("PROJECTNAME"), m_projectProperty.m_projectName);

    // 转码
    std::string utf8Str = "";
    codecvt::UTF16ToUTF8_Multi(templateStr, utf8Str);

    // 写文件
    FILE* pFile = NULL;
    ::_wfopen_s(&pFile, filePath.c_str(), _T("wb"));

    if (pFile == NULL)
    {
        return false;
    }

    char head[] = { (char)0xef, (char)0xbb, (char)0xbf };
    ::fwrite(head, 1, ARRAYSIZE(head), pFile);
    ::fwrite(utf8Str.c_str(), 1, utf8Str.length(), pFile);
    ::fclose(pFile);
    return true;
}

int main()
{
    ProjectProperty pro;
    pro.m_characterSet = UNICODE_TYPE;
    pro.m_configurationType = EXE;
    pro.m_projectName = L"Common";
    pro.m_slnName = L"SimpleApp";  // 添加到sln或者创建sln，不能为空
    pro.m_userProps = L"app.props";
    pro.m_runtimeLibraryType = STATIC;
    ProjectMaker maker(pro, L"D:\\workspaces\\C++_workspaces\\SimpleApp\\", true);
    bool b = maker.MakeProject();
    ::system("pause");
    return 1;
}
