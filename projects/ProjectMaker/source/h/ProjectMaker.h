
#ifndef __POJECT_MAKER_H_
#define __POJECT_MAKER_H_

#include <string>
#include <tchar.h>

/** ���ɵ�����: .exe .dll .lib
*/
enum CONFIGURATION_TYPE
{
    LIB = 0,
    DLL,
    EXE
};

/** ʹ�ÿ��ַ����Ƕ��ַ�
*/
enum CHARACTER_SET
{
    UNICODE_TYPE = 0,
    MULTI_BYTE
};

/** ��̬���ӻ��Ƕ�̬����
@STATIC     �������ļ�������lib��
@DYNAMIC    �������ļ�������dll�У���������ʱ����dll
*/
enum RUNTIME_LIBRARY_TYPE
{
    STATIC = 0,
    DYNAMIC
};

/** ��������
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

    /** �����Ĺ�������
    */
    std::wstring m_projectName;

    /** �����������ļ�
    */
    CONFIGURATION_TYPE m_configurationType;

    /** ʹ�ÿ��ַ����Ƕ��ַ�
    */
    CHARACTER_SET m_characterSet;

    /** ��̬���ӻ��Ƕ�̬����
    */
    RUNTIME_LIBRARY_TYPE m_runtimeLibraryType;
}ProjectProperty;

/** ���̴�����
*/
class ProjectMaker
{
public:
    ProjectMaker(const ProjectProperty& m_projectProperty, const std::wstring rootDir, bool bRemoveDirIfExit);
    ~ProjectMaker();

public:
    /** �������
    @return �Ƿ�ɹ�
    */
    bool MakeProject();

private:
    /** ����Ŀ¼ǰ��׼����������·���Ƿ���Ч���ļ��д��ڵ��߼������
    @return �Ƿ�ɹ�
    */
    bool PrepareMakeDir();

    /** �����ļ�Ŀ¼�ṹ
    @return �Ƿ�ɹ�
    */
    bool MakeDir();

    /** �����ļ�
    @return �Ƿ�ɹ�
    */
    bool MakeFile();

private:
    /** �������ɵĹ�������
    */
    ProjectProperty m_projectProperty;

    /** ��������ļ��н�Ҫ���ŵ�����
    */
    std::wstring m_rootDir;

    /** ��������Ŀ�ļ���·��
    */
    std::wstring m_projectDir;

    /** ����ļ���m_rootDir/m_projectProperty.m_projectName ���ڵĻ����Ƿ��Ƴ������´���
    */
    bool m_bRemoveDirIfExit;
};

#endif //__POJECT_MAKER_H_
