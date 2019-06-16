
#ifndef __POJECT_MAKER_H_
#define __POJECT_MAKER_H_

#include <string>
#include <tchar.h>

/** 生成的类型: .exe .dll .lib
*/
enum CONFIGURATION_TYPE
{
    LIB = 0,
    DLL,
    EXE
};

/** 使用款字符还是多字符
*/
enum CHARACTER_SET
{
    UNICODE_TYPE = 0,
    MULTI_BYTE
};

/** 静态链接还是动态链接
@STATIC     二进制文件保存在lib中
@DYNAMIC    二进制文件保存在dll中，程序运行时依赖dll
*/
enum RUNTIME_LIBRARY_TYPE
{
    STATIC = 0,
    DYNAMIC
};

enum PROJECTMAKER_ERROR
{
	ERROR_DEFAULT = 0, //默认类型
};

/** 工程属性
*/
typedef struct ProjectProperty
{
    ProjectProperty() :
        m_projectName(_T("")),
        m_configurationType(EXE),
        m_characterSet(UNICODE_TYPE),
        m_runtimeLibraryType(STATIC)
    {

    }

    ProjectProperty(const std::wstring& name,
                    CONFIGURATION_TYPE configurationType,
                    CHARACTER_SET characterSet,
                    RUNTIME_LIBRARY_TYPE runtimeLibraryType) :
        m_projectName(name),
        m_configurationType(configurationType),
        m_characterSet(characterSet),
        m_runtimeLibraryType(runtimeLibraryType)
    {

    }

    /** 期望的工程名称
    */
    std::wstring m_projectName;

    /** 期望的生成文件
    */
    CONFIGURATION_TYPE m_configurationType;

    /** 使用款字符还是多字符
    */
    CHARACTER_SET m_characterSet;

    /** 静态链接还是动态链接
    */
    RUNTIME_LIBRARY_TYPE m_runtimeLibraryType;
}ProjectProperty;

/** 工程创造者
*/
class ProjectMaker
{
public:
    ProjectMaker() = delete;
    ProjectMaker(const ProjectProperty& m_projectProperty, const std::wstring rootDir, bool bRemoveDirIfExit);
    ~ProjectMaker();

public:
    /** 创造入口
    @return 是否成功
    */
    bool MakeProject();

private:
	/** 日子错误
	@param [in] error 错误类型
	*/
	void LogError(const PROJECTMAKER_ERROR& error);

    /** 创建目录前的准备处理：包括路径是否有效，文件夹存在的逻辑处理等
    @return 是否成功
    */
    bool PrepareMakeDir();

    /** 生成文件目录结构
    @return 是否成功
    */
    bool MakeDir();

    /** 生成.vcxproj文件
    @return 是否成功
    */
    bool MakeProjFile();

	/** 生成.vcxproj.filters
	@return 是否成功
	*/
	bool MakeFiltersFile();

	/** 生成 .h .cpp
	@return 是否成功
	*/
	bool MakeSrcFile();

private:
    /** 期望生成的工程属性
    */
    ProjectProperty m_projectProperty;

    /** 这个工程文件夹将要被放到哪里
    */
    std::wstring m_rootDir;

    /** 真正的项目文件夹路径
    */
    std::wstring m_projectDir;

    /** 如果文件夹m_rootDir/m_projectProperty.m_projectName 存在的话，是否移除他重新创建
    */
    bool m_bRemoveDirIfExit;

	/** 失败原因
	*/
	PROJECTMAKER_ERROR m_LastError;
};

#endif //__POJECT_MAKER_H_
