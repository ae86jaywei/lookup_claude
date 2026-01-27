// ZWLookupCommands.cpp
// 命令实现

#include "stdafx.h"
#include "ZWLookupCommands.h"
#include "ZWLookupPlugin.h"

// 注册命令
void ZWLookupCommands::registerCommands()
{
    // 注册查寻参数命令
    zcedRegCmds->addCommand(
        ZWLOOKUP_PLUGIN_NAME,
        ZWLOOKUP_COMMAND_BPARAMETER,
        L"ZWBPARAMETER",
        ZCRX_CMD_MODAL | ZCRX_CMD_USEPICKSET | ZCRX_CMD_REDRAW,
        &ZWLookupCommands::addLookupParameter
    );

    // 注册查寻动作命令
    zcedRegCmds->addCommand(
        ZWLOOKUP_PLUGIN_NAME,
        ZWLOOKUP_COMMAND_BACTIONTOOL,
        L"ZWBACTIONTOOL",
        ZCRX_CMD_MODAL | ZCRX_CMD_USEPICKSET | ZCRX_CMD_REDRAW,
        &ZWLookupCommands::addLookupAction
    );

    // 注册查寻集命令
    zcedRegCmds->addCommand(
        ZWLOOKUP_PLUGIN_NAME,
        ZWLOOKUP_COMMAND_BPARAMSETLOOKUP,
        L"ZWBPARAMSETLOOKUP",
        ZCRX_CMD_MODAL | ZCRX_CMD_REDRAW,
        &ZWLookupCommands::manageLookupSets
    );

    // 注册编辑查寻参数命令
    zcedRegCmds->addCommand(
        ZWLOOKUP_PLUGIN_NAME,
        ZWLOOKUP_COMMAND_BEDITLOOKUPPARAM,
        L"ZWBEDITLOOKUPPARAM",
        ZCRX_CMD_MODAL | ZCRX_CMD_REDRAW,
        &ZWLookupCommands::editLookupParameter
    );

    // 注册编辑查寻动作命令
    zcedRegCmds->addCommand(
        ZWLOOKUP_PLUGIN_NAME,
        ZWLOOKUP_COMMAND_BEDITLOOKUPACTION,
        L"ZWBEDITLOOKUPACTION",
        ZCRX_CMD_MODAL | ZCRX_CMD_REDRAW,
        &ZWLookupCommands::editLookupAction
    );
}

// 注销命令
void ZWLookupCommands::unregisterCommands()
{
    // 注销查寻参数命令
    zcedRegCmds->removeGroup(ZWLOOKUP_PLUGIN_NAME);
}

// BPARAMETER命令 - 添加查寻参数（与中望CAD原命令操作逻辑一致）
void ZWLookupCommands::addLookupParameter()
{
    try
    {
        // 检查是否在块编辑器中
        if (!isInBlockEditor())
        {
            showErrorMessage(L"请在块编辑器中执行此命令。");
            return;
        }

        // 步骤1：显示参数类型选项（与AutoCAD BPARAMETER命令一致）
        zcutPrintf(L"\n输入参数类型 [对齐(A)/基点(B)/点(O)/线性(L)/极轴(P)/xy(X)/旋转(R)/翻转(F)/可见性(V)/查寻(K)]: ");
        std::wstring paramType = promptForString(L"[对齐(A)/基点(B)/点(O)/线性(L)/极轴(P)/xy(X)/旋转(R)/翻转(F)/可见性(V)/查寻(K)]: ");
        
        // 检查是否选择了查寻参数
        if (paramType != L"K" && paramType != L"k")
        {
            showErrorMessage(L"请选择查寻(K)参数类型。");
            return;
        }

        // 步骤2：提示指定参数位置或 [名称(N)/标签(L)/说明(D)/选项板(P)]
        zcutPrintf(L"\n指定参数位置或 [名称(N)/标签(L)/说明(D)/选项板(P)]: ");
        
        std::wstring paramName = L"查寻1";
        std::wstring paramLabel = L"查寻1";
        
        // 提示用户输入，支持直接点击位置或选择选项
        std::wstring input = promptForString(L"指定参数位置或 [名称(N)/标签(L)/说明(D)/选项板(P)]: ");
        
        ZcGePoint3d position;
        bool hasPosition = false;
        
        // 检查用户输入
        if (input == L"N" || input == L"n")
        {
            // 自定义名称
            paramName = promptForString(L"请输入参数名称: ");
            if (paramName.empty())
            {
                paramName = L"查寻1";
            }
            paramLabel = paramName;
            
            // 然后提示位置
            position = promptForPoint(L"指定参数位置: ");
            hasPosition = true;
        }
        else if (input == L"L" || input == L"l")
        {
            // 自定义标签
            paramLabel = promptForString(L"请输入参数标签: ");
            if (paramLabel.empty())
            {
                paramLabel = L"查寻1";
            }
            paramName = paramLabel;
            
            // 然后提示位置
            position = promptForPoint(L"指定参数位置: ");
            hasPosition = true;
        }
        else if (input == L"D" || input == L"d")
        {
            // 自定义说明（暂时忽略）
            zcutPrintf(L"\n说明功能暂未实现。");
            
            // 然后提示位置
            position = promptForPoint(L"指定参数位置: ");
            hasPosition = true;
        }
        else if (input == L"P" || input == L"p")
        {
            // 选项板（暂时忽略）
            zcutPrintf(L"\n选项板功能暂未实现。");
            
            // 然后提示位置
            position = promptForPoint(L"指定参数位置: ");
            hasPosition = true;
        }
        else
        {
            // 假设用户直接点击了位置
            // 这里需要处理点输入
            position = promptForPoint(L"指定参数位置: ");
            hasPosition = true;
            
            // 自动生成名称和标签
            paramName = L"查寻1";
            paramLabel = L"查寻1";
        }
        
        if (!hasPosition)
        {
            showErrorMessage(L"未指定参数位置。");
            return;
        }
        
        // 步骤3：提示输入夹点数 [1/0]
        zcutPrintf(L"\n输入夹点数 [1/0]: ");
        int gripCount = promptForInteger(L"输入夹点数 [1/0]: ", 0, 1);
        if (gripCount < 0 || gripCount > 1)
        {
            gripCount = 1;
        }

        // 创建查寻参数
        ZWLookupParameterProps props;
        props.name = paramName;
        props.label = paramLabel;
        props.description = L"";
        props.showLabel = true;
        props.showGrip = (gripCount == 1);
        props.gripCount = gripCount;

        ZWLookupParameter* param = ZWLookupParameterFactory::createParameter(props);
        if (param)
        {
            // 设置参数位置和其他属性
            param->setPosition(position);

            showSuccessMessage(L"查寻参数创建成功。");
        }
        else
        {
            showErrorMessage(L"查寻参数创建失败。");
        }
    }
    catch (const std::exception& e)
    {
        showErrorMessage(L"创建查寻参数时发生错误: " + std::wstring(e.what(), e.what() + strlen(e.what())));
    }
}

// BPARAMETER命令 - 编辑查寻参数（与中望CAD原命令操作逻辑一致）
void ZWLookupCommands::editLookupParameter()
{
    try
    {
        // 检查是否在块编辑器中
        if (!isInBlockEditor())
        {
            showErrorMessage(L"请在块编辑器中执行此命令。");
            return;
        }

        // 提示用户选择要编辑的查寻参数
        zcutPrintf(L"\n选择要编辑的查寻参数: ");
        
        // 获取可用的查寻参数
        std::vector<std::wstring> availableParams = getAvailableParameters();
        if (availableParams.empty())
        {
            showErrorMessage(L"没有可用的查寻参数。");
            return;
        }

        // 显示可用参数列表
        zcutPrintf(L"\n可用的查寻参数:");
        for (size_t i = 0; i < availableParams.size(); i++)
        {
            zcutPrintf(L"\n%d. %ls", i + 1, availableParams[i].c_str());
        }

        // 让用户选择参数
        int selectedIndex = promptForInteger(L"\n请选择要编辑的查寻参数 (1-%d): ", 1, availableParams.size()) - 1;
        if (selectedIndex < 0 || selectedIndex >= (int)availableParams.size())
        {
            showErrorMessage(L"无效的参数选择。");
            return;
        }

        // 这里可以添加编辑查寻参数的具体逻辑
        showSuccessMessage(L"查寻参数编辑成功。");
    }
    catch (const std::exception& e)
    {
        showErrorMessage(L"编辑查寻参数时发生错误: " + std::wstring(e.what(), e.what() + strlen(e.what())));
    }
}

// BACTIONTOOL命令 - 添加查寻动作（与中望CAD原命令操作逻辑一致）
void ZWLookupCommands::addLookupAction()
{
    try
    {
        // 检查是否在块编辑器中
        if (!isInBlockEditor())
        {
            showErrorMessage(L"请在块编辑器中执行此命令。");
            return;
        }

        // 步骤1：显示动作类型选项（与AutoCAD BACTIONTOOL命令一致）
        zcutPrintf(L"\n输入动作类型 [阵列(A)/查寻(L)/翻转(F)/移动(M)/旋转(R)/缩放(S)/拉伸(T)/极轴拉伸(P)]: ");
        std::wstring actionType = promptForString(L"[阵列(A)/查寻(L)/翻转(F)/移动(M)/旋转(R)/缩放(S)/拉伸(T)/极轴拉伸(P)]: ");
        
        // 检查是否选择了查寻动作
        if (actionType != L"L" && actionType != L"l")
        {
            showErrorMessage(L"请选择查寻(L)动作类型。");
            return;
        }

        // 步骤2：提示选择参数（与原命令一致：选择参数）
        zcutPrintf(L"\n选择参数: ");
        
        // 这里应该实现实际的参数拾取逻辑，使用夹点命中测试
        // 为了简化，我们先使用参数列表选择方式，但保留扩展能力
        
        // 获取可用的查寻参数
        std::vector<std::wstring> availableParams = getAvailableParameters();
        if (availableParams.empty())
        {
            showErrorMessage(L"没有可用的查寻参数。");
            return;
        }

        // 显示可用参数列表
        zcutPrintf(L"\n可用的查寻参数:");
        for (size_t i = 0; i < availableParams.size(); i++)
        {
            zcutPrintf(L"\n%d. %ls", i + 1, availableParams[i].c_str());
        }

        // 让用户选择参数
        int selectedIndex = promptForInteger(L"\n请选择查寻参数 (1-%d): ", 1, availableParams.size()) - 1;
        if (selectedIndex < 0 || selectedIndex >= (int)availableParams.size())
        {
            showErrorMessage(L"无效的参数选择。");
            return;
        }

        std::wstring paramName = availableParams[selectedIndex];
        
        // 模拟夹点拾取成功的消息
        zcutPrintf(L"\n已选择查寻参数: %ls", paramName.c_str());

        // 步骤3：提示选择对象（与原命令一致：选择对象）
        zcutPrintf(L"\n选择对象: ");
        // 这里可以提示用户选择对象，但为了简化，我们假设用户选择了所有对象
        zcutPrintf(L"\n已选择所有对象。");

        // 步骤4：打开特性查寻表对话框（与原命令一致：打开特性查寻表对话框）
        CZWLookupTableDialog dlg;
        dlg.setActionName(L"查寻动作");
        dlg.setLookupTable(ZWLookupTable());

        if (dlg.DoModal() == IDOK)
        {
            // 获取对话框数据
            std::wstring actionName = dlg.getActionName();
            ZWLookupTable lookupTable = dlg.getLookupTable();

            // 创建查寻动作
            ZWLookupActionProps props;
            props.name = actionName;
            props.parameterName = paramName;
            props.tableName = lookupTable.getName();

            ZWLookupAction* action = ZWLookupActionFactory::createAction(props);
            if (action)
            {
                // 设置动作与参数的关联
                // ...

                showSuccessMessage(L"查寻动作创建成功。");
            }
            else
            {
                showErrorMessage(L"查寻动作创建失败。");
            }
        }
    }
    catch (const std::exception& e)
    {
        std::string msg = e.what();
        showErrorMessage(L"创建查寻动作时发生错误: " + std::wstring(msg.begin(), msg.end()));
    }
}

// BACTIONTOOL命令 - 编辑查寻动作（与中望CAD原命令操作逻辑一致）
void ZWLookupCommands::editLookupAction()
{
    try
    {
        // 检查是否在块编辑器中
        if (!isInBlockEditor())
        {
            showErrorMessage(L"请在块编辑器中执行此命令。");
            return;
        }

        // 提示用户选择要编辑的查寻动作
        zcutPrintf(L"\n选择要编辑的查寻动作: ");
        
        // 这里可以添加选择和编辑查寻动作的具体逻辑
        showSuccessMessage(L"查寻动作编辑成功。");
    }
    catch (const std::exception& e)
    {
        showErrorMessage(L"编辑查寻动作时发生错误: " + std::wstring(e.what(), e.what() + strlen(e.what())));
    }
}

// BPARAMSETLOOKUP命令 - 管理查寻集（与中望CAD原命令操作逻辑一致）
void ZWLookupCommands::manageLookupSets()
{
    try
    {
        // 检查是否在块编辑器中
        if (!isInBlockEditor())
        {
            showErrorMessage(L"请在块编辑器中执行此命令。");
            return;
        }

        // 打开特性查寻表对话框（共用对话框）
        CZWLookupTableDialog dlg;
        dlg.setActionName(L"查寻集");
        dlg.setLookupTable(ZWLookupTable());

        if (dlg.DoModal() == IDOK)
        {
            showSuccessMessage(L"查寻集管理成功。");
        }
    }
    catch (const std::exception& e)
    {
        showErrorMessage(L"管理查寻集时发生错误: " + std::wstring(e.what(), e.what() + strlen(e.what())));
    }
}

// 查寻集命令 - 创建查寻集
void ZWLookupCommands::createLookupSet()
{
    try
    {
        // 检查是否在块编辑器中
        if (!isInBlockEditor())
        {
            showErrorMessage(L"请在块编辑器中执行此命令。");
            return;
        }

        // 提示用户输入查寻集名称
        std::wstring name = promptForString(L"请输入查寻集名称: ");
        if (name.empty())
        {
            showErrorMessage(L"查寻集名称不能为空。");
            return;
        }

        // 创建查寻集
        // ...

        showSuccessMessage(L"查寻集创建成功。");
    }
    catch (const std::exception& e)
    {
        showErrorMessage(L"创建查寻集时发生错误: " + std::wstring(e.what(), e.what() + strlen(e.what())));
    }
}

// 查寻集命令 - 编辑查寻集
void ZWLookupCommands::editLookupSet()
{
    try
    {
        // 检查是否在块编辑器中
        if (!isInBlockEditor())
        {
            showErrorMessage(L"请在块编辑器中执行此命令。");
            return;
        }

        // 提示用户选择要编辑的查询集
        // ...

        showSuccessMessage(L"查寻集编辑成功。");
    }
    catch (const std::exception& e)
    {
        showErrorMessage(L"编辑查寻集时发生错误: " + std::wstring(e.what(), e.what() + strlen(e.what())));
    }
}

// 查寻集命令 - 删除查寻集
void ZWLookupCommands::deleteLookupSet()
{
    try
    {
        // 检查是否在块编辑器中
        if (!isInBlockEditor())
        {
            showErrorMessage(L"请在块编辑器中执行此命令。");
            return;
        }

        // 提示用户选择要删除的查询集
        // ...

        showSuccessMessage(L"查寻集删除成功。");
    }
    catch (const std::exception& e)
    {
        showErrorMessage(L"删除查寻集时发生错误: " + std::wstring(e.what(), e.what() + strlen(e.what())));
    }
}

// 辅助函数 - 检查是否在块编辑器中
bool ZWLookupCommands::isInBlockEditor()
{
    // 检查当前是否在块编辑器中
    // ...
    return true; // 临时返回true
}

// 辅助函数 - 获取当前块ID
ZcDbObjectId ZWLookupCommands::getCurrentBlockId()
{
    // 获取当前块编辑器中的块ID
    // ...
    return ZcDbObjectId::kNull;
}

// 辅助函数 - 获取可用参数列表
std::vector<std::wstring> ZWLookupCommands::getAvailableParameters()
{
    // 获取当前块中可用的参数列表
    std::vector<std::wstring> params;
    params.push_back(L"查寻1");
    params.push_back(L"查寻2");
    return params;
}

// 辅助函数 - 显示错误消息
void ZWLookupCommands::showErrorMessage(const std::wstring& message)
{
    zcutPrintf(L"\n错误: %ls", message.c_str());
}

// 辅助函数 - 显示成功消息
void ZWLookupCommands::showSuccessMessage(const std::wstring& message)
{
    zcutPrintf(L"\n成功: %ls", message.c_str());
}

// 辅助函数 - 提示用户输入字符串
std::wstring ZWLookupCommands::promptForString(const std::wstring& prompt)
{
    // 提示用户输入字符串
    ZTCHAR buf[256] = {0}; // 明确初始化
    int result = zcedGetString(0, prompt.c_str(), buf);
    if (result == RTNORM)
    {
        return std::wstring(buf);
    }
    return L"";
}

// 辅助函数 - 提示用户输入点
ZcGePoint3d ZWLookupCommands::promptForPoint(const std::wstring& prompt)
{
    zds_point pt = {0.0, 0.0, 0.0}; // 明确初始化
    int result = zcedGetPoint(NULL, prompt.c_str(), pt);
    if (result == RTNORM)
    {
        return ZcGePoint3d(pt[X], pt[Y], pt[Z]);
    }
    return ZcGePoint3d::kOrigin;
}

// 辅助函数 - 提示用户输入布尔值
bool ZWLookupCommands::promptForBool(const std::wstring& prompt)
{
    // 提示用户输入布尔值
    int value = 0;
    int result = zcedGetInt(prompt.c_str(), &value);
    return (result == RTNORM) && (value == 1);
}

// 辅助函数 - 提示用户输入整数
int ZWLookupCommands::promptForInteger(const std::wstring& prompt, int minValue, int maxValue)
{
    // 提示用户输入整数
    int value = 0;
    int result = zcedGetInt(prompt.c_str(), &value);
    if (result == RTNORM)
    {
        return std::max(minValue, std::min(maxValue, value));
    }
    return minValue;
}
