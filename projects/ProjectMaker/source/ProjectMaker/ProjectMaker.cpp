#include "ProjectMaker.h"
#include "file/FileAndDirUtil.h"
#include "GUID/GUID.h"
#include "codecvt/code_cvt.h"
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
    if (!PrepareMakeDir())
    {
        return false;
    }

    if (!MakeDir())
    {
        return false;
    }

    if (!MakeProjFile())
    {
        return false;
    }

    if (!MakeFiltersFile())
    {
        return false;
    }

    if (!MakeSrcFile())
    {
        return false;
    }

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
    // 生成文件夹
    std::wstring subDirName[] = {_T("bin"), _T("build"), _T("doc"), _T("res"), _T("src"), _T("test"), _T("vendor")};

    for (int i = 0; i < MYARRAYSIZE(subDirName); ++i)
    {
        if (!Util::CreateDir(m_projectDir + subDirName[i] + _T("\\")))
        {
            return false;
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
        "    <VCProjectVersion>15.0</VCProjectVersion>\r\n"
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
        "    <OutDir>$(ProjectDir)..\\bin\\$(Configuration)\\$(PlatForm)\\</OutDir>\r\n"
        "    <IntDir>$(OutDir)tmp\\</IntDir>\r\n"
        "    <IncludePath>$(ProjectDir)..\\src\\;$(ProjectDir)..\\vendor\\;$(IncludePath)</IncludePath>\r\n"
        "    <LibraryPath>$(ProjectDir)..\\vendor\\;$(LibraryPath)</LibraryPath>\r\n"
        "  </PropertyGroup>\r\n"
        "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">\r\n"
        "    <OutDir>$(ProjectDir)..\\bin\\$(Configuration)\\$(PlatForm)\\</OutDir>\r\n"
        "    <IntDir>$(OutDir)tmp\\</IntDir>\r\n"
        "    <IncludePath>$(ProjectDir)..\\src\\;$(ProjectDir)..\\vendor\\;$(IncludePath)</IncludePath>\r\n"
        "    <LibraryPath>$(ProjectDir)..\\vendor\\;$(LibraryPath)</LibraryPath>\r\n"
        "  </PropertyGroup>\r\n"
        "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">\r\n"
        "    <OutDir>$(ProjectDir)..\\bin\\$(Configuration)\\$(PlatForm)\\</OutDir>\r\n"
        "    <IntDir>$(OutDir)tmp\\</IntDir>\r\n"
        "    <IncludePath>$(ProjectDir)..\\src\\;$(ProjectDir)..\\vendor\\;$(IncludePath)</IncludePath>\r\n"
        "    <LibraryPath>$(ProjectDir)..\\vendor\\;$(LibraryPath)</LibraryPath>\r\n"
        "  </PropertyGroup>\r\n"
        "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">\r\n"
        "    <OutDir>$(ProjectDir)..\\bin\\$(Configuration)\\$(PlatForm)\\</OutDir>\r\n"
        "    <IntDir>$(OutDir)tmp\\</IntDir>\r\n"
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

    std::wstring projectNameStr = m_projectProperty.m_projectName;
    std::wstring GUIDStr = _T("");
    codecvt::UTF8ToUTF16_Multi(Util::GenerateGuid(), GUIDStr);
    replace_str(templateStr, _T("CONFIGURATION_TYPE"), configurationStr);
    replace_str(templateStr, _T("CHARACTER_SET"), characterSetStr);
    replace_str(templateStr, _T("RUNTIME_LIBRARY_TYPE_DEBUG"), runtimeLibraryDbgStr);
    replace_str(templateStr, _T("RUNTIME_LIBRARY_TYPE_RELEASE"), runtimeLibraryStr);
    replace_str(templateStr, _T("PROJECTNAME"), projectNameStr);
    replace_str(templateStr, _T("PROJECT_GUID"), GUIDStr);
    std::string utf8Str = "";
    codecvt::UTF16ToUTF8_Multi(templateStr, utf8Str);

    FILE* pFile = NULL;
    ::_wfopen_s(&pFile, filePath.c_str(), _T("wb"));

    if (pFile == NULL)
    {
        return false;
    }

    char head[] = { (char)0xef, (char)0xbb, (char)0xbf };
    ::fwrite(head, 1, 3, pFile);
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
    ::fwrite(head, 1, 3, pFile);
    ::fwrite(utf8Str.c_str(), 1, utf8Str.length(), pFile);
    ::fclose(pFile);
    return true;
}

bool ProjectMaker::MakeSrcFile()
{
    return true;
}

int main()
{
    ProjectProperty pro;
    pro.m_characterSet = UNICODE_TYPE;
    pro.m_configurationType = EXE;
    pro.m_projectName = L"Hosting";
    pro.m_runtimeLibraryType = STATIC;
    ProjectMaker maker(pro, L"D:\\workspaces\\C++_workspaces\\SimpleApp\\", true);
    bool b = maker.MakeProject();
    ::system("pause");
    return 1;
}
