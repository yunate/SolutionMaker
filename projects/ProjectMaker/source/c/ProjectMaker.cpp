#include "ProjectMaker.h"
#include "file/FileAndDirUtil.h"
#include "GUID/GUID.h"
#include "codecvt/CodeCvt.h"

#include <map>

template<typename T, unsigned int N>
char(&array_size_fake_func(T(&)[N]))[N];
#define ARRAYSIZE(A) sizeof(array_size_fake_func(A))

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

    if (!MakeFile())
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
    std::wstring subDirName[] = {_T("bin"), _T("build"), _T("doc"), _T("source"), _T("test"), _T("vendor"), };

    for (int i = 0; i < ARRAYSIZE(subDirName); ++i)
    {
        if (!Util::CreateDir(m_projectDir + subDirName[i]) + _T("\\"))
        {
            return false;
        }
    }

    std::wstring SourceDir = m_projectDir + _T("source") + _T("\\");
    std::wstring subSourceDirName[] = {_T("h"), _T("c"), _T("src"),};

    for (int i = 0; i < ARRAYSIZE(subSourceDirName); ++i)
    {
        if (!Util::CreateDir(m_projectDir + subSourceDirName[i]) + _T("\\"))
        {
            return false;
        }
    }

    return true;
}

bool ProjectMaker::MakeFile()
{
    std::wstring filePath = m_projectDir + _T("build\\") + m_projectProperty.m_projectName;

    if (!Util::CreateFile_(filePath))
    {
        return false;
    }

    std::wstring hPath = m_projectDir + _T("source\\h\\") + m_projectProperty.m_projectName + _T(".h");

    if (!Util::CreateFile_(hPath))
    {
        return false;
    }

    std::wstring cPath = m_projectDir + _T("source\\c\\") + m_projectProperty.m_projectName + _T(".cpp");

    if (!Util::CreateFile_(cPath))
    {
        return false;
    }

    std::wstring templateStr = _T("<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                  "<Project DefaultTargets=\"Build\" ToolsVersion=\"15.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">"
                                  "  <ItemGroup Label=\"ProjectConfigurations\">"
                                  "    <ProjectConfiguration Include=\"Debug|Win32\">"
                                  "      <Configuration>Debug</Configuration>"
                                  "      <Platform>Win32</Platform>"
                                  "    </ProjectConfiguration>"
                                  "    <ProjectConfiguration Include=\"Release|Win32\">"
                                  "      <Configuration>Release</Configuration>"
                                  "      <Platform>Win32</Platform>"
                                  "    </ProjectConfiguration>"
                                  "    <ProjectConfiguration Include=\"Debug|x64\">"
                                  "      <Configuration>Debug</Configuration>"
                                  "      <Platform>x64</Platform>"
                                  "    </ProjectConfiguration>"
                                  "    <ProjectConfiguration Include=\"Release|x64\">"
                                  "      <Configuration>Release</Configuration>"
                                  "      <Platform>x64</Platform>"
                                  "    </ProjectConfiguration>"
                                  "  </ItemGroup>"
                                  "  <ItemGroup>"
                                  "    <ClInclude Include=\"..\..\..\common_vendor\File\FileAndDirUtil.h\" />"
                                  "    <ClInclude Include=\"..\source\h\PROJECTNAME.h\" />"
                                  "  </ItemGroup>"
                                  "  <ItemGroup>"
                                  "    <ClCompile Include=\"..\..\..\common_vendor\File\FileAndDirUtil.cpp\" />"
                                  "    <ClCompile Include=\"..\source\c\PROJECTNAME.cpp\" />"
                                  "  </ItemGroup>"
                                  "  <PropertyGroup Label=\"Globals\">"
                                  "    <VCProjectVersion>15.0</VCProjectVersion>"
                                  "    <ProjectGuid>PROJECT_GUID</ProjectGuid>"
                                  "    <RootNamespace>PROJECTNAME1</RootNamespace>"
                                  "    <WindowsTargetPlatformVersion>7.0</WindowsTargetPlatformVersion>"
                                  "  </PropertyGroup>"
                                  "  <Import Project=\"$(VCTargetsPath)\Microsoft.Cpp.Default.props\" />"
                                  "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\" Label=\"Configuration\">"
                                  "    <ConfigurationType>CONFIGURATION_TYPE</ConfigurationType>"
                                  "    <UseDebugLibraries>true</UseDebugLibraries>"
                                  "    <PlatformToolset>v141_xp</PlatformToolset>"
                                  "    <CharacterSet>CHARACTER_SET</CharacterSet>"
                                  "  </PropertyGroup>"
                                  "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\" Label=\"Configuration\">"
                                  "    <ConfigurationType>CONFIGURATION_TYPE</ConfigurationType>"
                                  "    <UseDebugLibraries>false</UseDebugLibraries>"
                                  "    <PlatformToolset>v141_xp</PlatformToolset>"
                                  "    <WholeProgramOptimization>true</WholeProgramOptimization>"
                                  "    <CharacterSet>CHARACTER_SET</CharacterSet>"
                                  "  </PropertyGroup>"
                                  "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\" Label=\"Configuration\">"
                                  "    <ConfigurationType>CONFIGURATION_TYPE</ConfigurationType>"
                                  "    <UseDebugLibraries>true</UseDebugLibraries>"
                                  "    <PlatformToolset>v141_xp</PlatformToolset>"
                                  "    <CharacterSet>CHARACTER_SET</CharacterSet>"
                                  "  </PropertyGroup>"
                                  "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\" Label=\"Configuration\">"
                                  "    <ConfigurationType>CONFIGURATION_TYPE</ConfigurationType>"
                                  "    <UseDebugLibraries>false</UseDebugLibraries>"
                                  "    <PlatformToolset>v141_xp</PlatformToolset>"
                                  "    <WholeProgramOptimization>true</WholeProgramOptimization>"
                                  "    <CharacterSet>CHARACTER_SET</CharacterSet>"
                                  "  </PropertyGroup>"
                                  "  <Import Project=\"$(VCTargetsPath)\Microsoft.Cpp.props\" />"
                                  "  <ImportGroup Label=\"ExtensionSettings\">"
                                  "  </ImportGroup>"
                                  "  <ImportGroup Label=\"Shared\">"
                                  "  </ImportGroup>"
                                  "  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">"
                                  "    <Import Project=\"$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />"
                                  "  </ImportGroup>"
                                  "  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">"
                                  "    <Import Project=\"$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />"
                                  "  </ImportGroup>"
                                  "  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">"
                                  "    <Import Project=\"$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />"
                                  "  </ImportGroup>"
                                  "  <ImportGroup Label=\"PropertySheets\" Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">"
                                  "    <Import Project=\"$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />"
                                  "  </ImportGroup>"
                                  "  <PropertyGroup Label=\"UserMacros\" />"
                                  "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">"
                                  "    <OutDir>$(ProjectDir)..\bin\$(Configuration)\$(PlatForm)\</OutDir>"
                                  "    <IntDir>$(OutDir)tmp\</IntDir>"
                                  "    <IncludePath>$(ProjectDir)..\source\h\;$(SolutionDir)common_vendor\;$(IncludePath)</IncludePath>"
                                  "    <LibraryPath>$(ProjectDir)..\vendor\;$(SolutionDir)common_vendor\;$(LibraryPath)</LibraryPath>"
                                  "  </PropertyGroup>"
                                  "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">"
                                  "    <OutDir>$(ProjectDir)..\bin\$(Configuration)\$(PlatForm)\</OutDir>"
                                  "    <IntDir>$(OutDir)tmp\</IntDir>"
                                  "    <IncludePath>$(ProjectDir)..\source\h\;$(SolutionDir)common_vendor\;$(IncludePath)</IncludePath>"
                                  "    <LibraryPath>$(ProjectDir)..\vendor\;$(SolutionDir)common_vendor\;$(LibraryPath)</LibraryPath>"
                                  "  </PropertyGroup>"
                                  "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">"
                                  "    <OutDir>$(ProjectDir)..\bin\$(Configuration)\$(PlatForm)\</OutDir>"
                                  "    <IntDir>$(OutDir)tmp\</IntDir>"
                                  "    <IncludePath>$(ProjectDir)..\source\h\;$(SolutionDir)common_vendor\;$(IncludePath)</IncludePath>"
                                  "    <LibraryPath>$(ProjectDir)..\vendor\;$(SolutionDir)common_vendor\;$(LibraryPath)</LibraryPath>"
                                  "  </PropertyGroup>"
                                  "  <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">"
                                  "    <OutDir>$(ProjectDir)..\bin\$(Configuration)\$(PlatForm)\</OutDir>"
                                  "    <IntDir>$(OutDir)tmp\</IntDir>"
                                  "    <IncludePath>$(ProjectDir)..\source\h\;$(SolutionDir)common_vendor\;$(IncludePath)</IncludePath>"
                                  "    <LibraryPath>$(ProjectDir)..\vendor\;$(SolutionDir)common_vendor\;$(LibraryPath)</LibraryPath>"
                                  "  </PropertyGroup>"
                                  "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|Win32'\">"
                                  "    <ClCompile>"
                                  "      <WarningLevel>Level3</WarningLevel>"
                                  "      <Optimization>Disabled</Optimization>"
                                  "      <SDLCheck>true</SDLCheck>"
                                  "      <ConformanceMode>false</ConformanceMode>"
                                  "      <RuntimeLibrary>RUNTIME_LIBRARY_TYPE_DEBUG</RuntimeLibrary>"
                                  "    </ClCompile>"
                                  "  </ItemDefinitionGroup>"
                                  "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">"
                                  "    <ClCompile>"
                                  "      <WarningLevel>Level3</WarningLevel>"
                                  "      <Optimization>Disabled</Optimization>"
                                  "      <SDLCheck>true</SDLCheck>"
                                  "      <ConformanceMode>false</ConformanceMode>"
                                  "      <RuntimeLibrary>RUNTIME_LIBRARY_TYPE_DEBUG</RuntimeLibrary>"
                                  "    </ClCompile>"
                                  "  </ItemDefinitionGroup>"
                                  "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|Win32'\">"
                                  "    <ClCompile>"
                                  "      <WarningLevel>Level3</WarningLevel>"
                                  "      <Optimization>MaxSpeed</Optimization>"
                                  "      <FunctionLevelLinking>true</FunctionLevelLinking>"
                                  "      <IntrinsicFunctions>true</IntrinsicFunctions>"
                                  "      <SDLCheck>true</SDLCheck>"
                                  "      <ConformanceMode>false</ConformanceMode>"
                                  "      <RuntimeLibrary>RUNTIME_LIBRARY_TYPE_RELEASE</RuntimeLibrary>"
                                  "    </ClCompile>"
                                  "    <Link>"
                                  "      <EnableCOMDATFolding>true</EnableCOMDATFolding>"
                                  "      <OptimizeReferences>true</OptimizeReferences>"
                                  "    </Link>"
                                  "  </ItemDefinitionGroup>"
                                  "  <ItemDefinitionGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">"
                                  "    <ClCompile>"
                                  "      <WarningLevel>Level3</WarningLevel>"
                                  "      <Optimization>MaxSpeed</Optimization>"
                                  "      <FunctionLevelLinking>true</FunctionLevelLinking>"
                                  "      <IntrinsicFunctions>true</IntrinsicFunctions>"
                                  "      <SDLCheck>true</SDLCheck>"
                                  "      <ConformanceMode>false</ConformanceMode>"
                                  "      <RuntimeLibrary>RUNTIME_LIBRARY_TYPE_RELEASE</RuntimeLibrary>"
                                  "    </ClCompile>"
                                  "    <Link>"
                                  "      <EnableCOMDATFolding>true</EnableCOMDATFolding>"
                                  "      <OptimizeReferences>true</OptimizeReferences>"
                                  "    </Link>"
                                  "  </ItemDefinitionGroup>"
                                  "  <Import Project=\"$(VCTargetsPath)\Microsoft.Cpp.targets\" />"
                                  "  <ImportGroup Label=\"ExtensionTargets\">"
                                  "  </ImportGroup>"
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
    codecvt::UTF8ToUTF16(Util::GenerateGuid(), GUIDStr);

    std::map<std::wstring, std::wstring> replaceMap;
    replaceMap[_T("CONFIGURATION_TYPE")] = configurationStr;
    replaceMap[_T("CHARACTER_SET")] = characterSetStr;
    replaceMap[_T("RUNTIME_LIBRARY_TYPE_DEBUG")] = runtimeLibraryDbgStr;
    replaceMap[_T("RUNTIME_LIBRARY_TYPE_RELEASE")] = runtimeLibraryStr;
    replaceMap[_T("PROJECTNAME")] = projectNameStr;
    replaceMap[_T("PROJECT_GUID")] = GUIDStr;
    return true;
}

int main()
{
    return 1;
}
