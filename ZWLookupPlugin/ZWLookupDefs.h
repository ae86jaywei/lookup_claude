// ZWLookupDefs.h
// 常量和类型定义

#pragma once

#include <string>
#include <vector>

// 常量定义
#define ZWLOOKUP_PLUGIN_NAME L"ZWLookupPlugin"
#define ZWLOOKUP_PLUGIN_VERSION L"1.0.0"
#define ZWLOOKUP_PLUGIN_DESCRIPTION L"中望CAD查寻功能插件"

// 命令名称
#define ZWLOOKUP_COMMAND_BPARAMSETLOOKUP L"ZWBPARAMSETLOOKUP"
#define ZWLOOKUP_COMMAND_BPARAMETER L"ZWBPARAMETER"
#define ZWLOOKUP_COMMAND_BACTIONTOOL L"ZWBACTIONTOOL"
#define ZWLOOKUP_COMMAND_BEDITLOOKUPPARAM L"ZWBEDITLOOKUPPARAM"
#define ZWLOOKUP_COMMAND_BEDITLOOKUPACTION L"ZWBEDITLOOKUPACTION"

// 错误代码
#define ZWLOOKUP_ERR_SUCCESS 0
#define ZWLOOKUP_ERR_INVALID_PARAMETER -1
#define ZWLOOKUP_ERR_INVALID_ACTION -2
#define ZWLOOKUP_ERR_INVALID_TABLE -3
#define ZWLOOKUP_ERR_INVALID_SET -4
#define ZWLOOKUP_ERR_NOT_FOUND -5
#define ZWLOOKUP_ERR_ALREADY_EXISTS -6

// 类型定义

// 查寻表条目类型
struct ZWLookupTableEntry
{
    std::vector<std::wstring> inputValues;
    std::vector<std::wstring> lookupValues;
};

// 查寻参数属性
struct ZWLookupParameterProps
{
    std::wstring name;
    std::wstring label;
    std::wstring description;
    bool showLabel;
    bool showGrip;
    int gripCount;
};

// 查寻动作属性
struct ZWLookupActionProps
{
    std::wstring name;
    std::wstring parameterName;
    std::wstring tableName;
};

// 查寻集属性
struct ZWLookupSetProps
{
    std::wstring name;
    std::vector<std::wstring> parameterNames;
};

// 特性类型枚举
enum class ZWLookupPropertyType
{
    Input,  // 输入特性
    Lookup  // 查寻特性
};

// 查寻方向枚举
enum class ZWLookupDirection
{
    Forward,  // 正向查寻
    Reverse   // 反向查寻
};

// 夹点状态枚举
enum class ZWLookupGripState
{
    Normal,   // 正常状态
    Hovered,  // 悬停状态
    Selected, // 选中状态
    Disabled  // 禁用状态
};

// 查寻表关联类型
enum class ZWLookupTableAssociation
{
    OneToOne,   // 一对一关联
    OneToMany,  // 一对多关联
    ManyToOne   // 多对一关联
};
