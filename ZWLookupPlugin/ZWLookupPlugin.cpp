// ZWLookupPlugin.cpp
// 查寻功能插件入口

#include "stdafx.h"
#include "ZWLookupPlugin.h"
#include "ZWLookupCommands.h"
#include "ZWLookupParameter.h"
#include "ZWLookupAction.h"
#include "ZWLookupTable.h"
#include "ZWLookupSet.h"
#include "ZWLookupDialog.h"
#include "ZWLookupGripManager.h"

// 静态实例初始化
ZWLookupPlugin* ZWLookupPlugin::m_pInstance = nullptr;

// 构造函数
ZWLookupPlugin::ZWLookupPlugin()
    : m_bInitialized(false)
{
}

// 析构函数
ZWLookupPlugin::~ZWLookupPlugin()
{
}

// 单例访问器
ZWLookupPlugin* ZWLookupPlugin::GetInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new ZWLookupPlugin();
    }
    return m_pInstance;
}

// 销毁单例
void ZWLookupPlugin::DestroyInstance()
{
    if (m_pInstance != nullptr)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

// 插件初始化
bool ZWLookupPlugin::initialize()
{
    if (m_bInitialized)
    {
        return true;
    }

    try
    {
        // 初始化插件
        registerClasses();
        registerCommands();
        initializeData();

        // 输出加载成功信息
        zcutPrintf(_T("\n中望CAD查寻功能插件加载成功！"));
        zcutPrintf(_T("\n版本: %s"), ZWLOOKUP_PLUGIN_VERSION);
        zcutPrintf(_T("\n命令: BPARAMETER (查寻参数), BACTIONTOOL (查寻动作), BPARAMSETLOOKUP (查寻集)"));

        m_bInitialized = true;
        return true;
    }
    catch (...)
    {
        zcutPrintf(_T("\n中望CAD查寻功能插件初始化失败！"));
        return false;
    }
}

// 插件终止
void ZWLookupPlugin::terminate()
{
    if (!m_bInitialized)
    {
        return;
    }

    try
    {
        // 清理插件
        cleanupData();
        unregisterCommands();
        unregisterClasses();

        // 输出卸载成功信息
        zcutPrintf(_T("\n中望CAD查寻功能插件卸载成功！"));

        m_bInitialized = false;
    }
    catch (...)
    {
        zcutPrintf(_T("\n中望CAD查寻功能插件卸载时发生错误！"));
    }
}

// 插件初始化函数
void ZWLookupPlugin::registerClasses()
{
    // 注册查寻参数类
    ZWLookupParameter::rxInit();
    ZWLookupParameterFactory::rxInit();
    
    // 注册查寻动作类
    ZWLookupAction::rxInit();
    ZWLookupActionFactory::rxInit();
    
    // 注册查寻表类
    ZWLookupTable::rxInit();
    ZWLookupTableManager::rxInit();
    
    // 注册查寻集类
    ZWLookupSet::rxInit();
    ZWLookupSetManager::rxInit();
}

void ZWLookupPlugin::registerCommands()
{
    // 注册插件命令
    ZWLookupCommands::registerCommands();
}

void ZWLookupPlugin::initializeData()
{
    // 初始化夹点管理器
    ZWLookupGripManager::getInstance();
}

// 插件清理函数
void ZWLookupPlugin::unregisterClasses()
{
    // 注销查寻集类
    deleteZcRxClass(ZWLookupSetManager::desc());
    deleteZcRxClass(ZWLookupSet::desc());
    
    // 注销查寻表类
    deleteZcRxClass(ZWLookupTableManager::desc());
    deleteZcRxClass(ZWLookupTable::desc());
    
    // 注销查寻动作类
    deleteZcRxClass(ZWLookupActionFactory::desc());
    deleteZcRxClass(ZWLookupAction::desc());
    
    // 注销查寻参数类
    deleteZcRxClass(ZWLookupParameterFactory::desc());
    deleteZcRxClass(ZWLookupParameter::desc());
}

void ZWLookupPlugin::unregisterCommands()
{
    // 注销插件命令
    ZWLookupCommands::unregisterCommands();
}

void ZWLookupPlugin::cleanupData()
{
    // 清理夹点管理器
    ZWLookupGripManager::getInstance()->clearAllGrips();
}

// ZRX插件入口点函数
extern "C" ZcRx::AppRetCode zcrxEntryPoint(ZcRx::AppMsgCode msg, void* appId)
{
    switch (msg)
    {
    case ZcRx::kInitAppMsg:
        // 解锁应用程序
        zcrxDynamicLinker->unlockApplication(appId);
        // 注册MDI感知
        zcrxDynamicLinker->registerAppMDIAware(appId);
        // 初始化插件
        if (!ZWLookupPlugin::GetInstance()->initialize())
        {
            return ZcRx::kRetError;
        }
        break;

    case ZcRx::kUnloadAppMsg:
        // 终止插件
        ZWLookupPlugin::GetInstance()->terminate();
        // 销毁单例
        ZWLookupPlugin::DestroyInstance();
        break;

    case ZcRx::kLoadDwgMsg:
        // 文档加载消息
        break;

    case ZcRx::kUnloadDwgMsg:
        // 文档卸载消息
        break;

    case ZcRx::kInvkSubrMsg:
        // LISP调用消息
        break;

    default:
        break;
    }

    return ZcRx::kRetOK;
}
