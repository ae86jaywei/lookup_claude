// ZWLookupCommands.h
// 命令实现

#pragma once

#include "stdafx.h"
#include "ZWLookupDefs.h"
#include "ZWLookupParameter.h"
#include "ZWLookupAction.h"
#include "ZWLookupSet.h"
#include "ZWLookupDialog.h"

// 命令类
class ZWLookupCommands
{
public:
    // 注册命令
    static void registerCommands();
    static void unregisterCommands();

    // BPARAMETER命令扩展
    static void addLookupParameter();
    static void editLookupParameter();

    // BACTIONTOOL命令扩展
    static void addLookupAction();
    static void editLookupAction();

    // 查寻集命令
    static void manageLookupSets();
    static void createLookupSet();
    static void editLookupSet();
    static void deleteLookupSet();

    // 辅助函数
    static bool isInBlockEditor();
    static ZcDbObjectId getCurrentBlockId();
    static std::vector<std::wstring> getAvailableParameters();
    static void showErrorMessage(const std::wstring& message);
    static void showSuccessMessage(const std::wstring& message);
    static std::wstring promptForString(const std::wstring& prompt);
    static ZcGePoint3d promptForPoint(const std::wstring& prompt);
    static bool promptForBool(const std::wstring& prompt);
    static int promptForInteger(const std::wstring& prompt, int minValue = 0, int maxValue = INT_MAX);
};

// 命令函数声明
void ZWLOOKUP_BPARAMETER_LOOKUP();
void ZWLOOKUP_BACTIONTOOL_LOOKUP();
void ZWLOOKUP_BPARAMSETLOOKUP();
