// ZWLookupPlugin.h
// 查寻功能插件主头文件

#pragma once

#include "stdafx.h"
#include "ZWLookupDefs.h"

#ifdef ZWLOOKUPPLUGIN_EXPORTS
#define ZWLOOKUPPLUGIN_API __declspec(dllexport)
#else
#define ZWLOOKUPPLUGIN_API __declspec(dllimport)
#endif

// 插件管理类（单例模式）
class ZWLookupPlugin
{
public:
    // 单例访问器
    ZWLOOKUPPLUGIN_API static ZWLookupPlugin* GetInstance();
    ZWLOOKUPPLUGIN_API static void DestroyInstance();

    // 插件生命周期
    bool initialize();
    void terminate();

    // 获取插件信息
    const wchar_t* getName() const { return ZWLOOKUP_PLUGIN_NAME; }
    const wchar_t* getVersion() const { return ZWLOOKUP_PLUGIN_VERSION; }
    const wchar_t* getDescription() const { return ZWLOOKUP_PLUGIN_DESCRIPTION; }

private:
    // 私有构造函数（单例模式）
    ZWLookupPlugin();
    ~ZWLookupPlugin();

    // 禁止拷贝
    ZWLookupPlugin(const ZWLookupPlugin&) = delete;
    ZWLookupPlugin& operator=(const ZWLookupPlugin&) = delete;

    // 插件初始化函数
    void registerClasses();
    void registerCommands();
    void initializeData();

    // 插件清理函数
    void unregisterClasses();
    void unregisterCommands();
    void cleanupData();

    // 静态实例指针
    static ZWLookupPlugin* m_pInstance;
    
    // 初始化标志
    bool m_bInitialized;
};

// ZRX入口点声明
extern "C" ZWLOOKUPPLUGIN_API ZcRx::AppRetCode zcrxEntryPoint(ZcRx::AppMsgCode msg, void* appId);
