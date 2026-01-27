// ZWLookupAction.cpp
// 查寻动作实现

#include "stdafx.h"
#include "ZWLookupAction.h"
#include "ZWLookupParameter.h"

// 注册查寻动作类
ZCRX_CONS_DEFINE_MEMBERS(ZWLookupAction, ZcRxObject, 1);

// 注册查寻动作工厂类
ZCRX_CONS_DEFINE_MEMBERS(ZWLookupActionFactory, ZcRxObject, 1);

// 构造函数
ZWLookupAction::ZWLookupAction()
    : m_name(L"查寻动作")
    , m_parameterName(L"")
    , m_tableName(L"")
    , m_blockId(ZcDbObjectId::kNull)
    , m_parameterId(ZcDbObjectId::kNull)
    , m_actionId(ZcDbObjectId::kNull)
    , m_isPreviewActive(false)
{}

// 析构函数
ZWLookupAction::~ZWLookupAction()
{}

// 动作属性访问器
std::wstring ZWLookupAction::getName() const
{
    return m_name;
}

void ZWLookupAction::setName(const std::wstring& name)
{
    m_name = name;
}

std::wstring ZWLookupAction::getParameterName() const
{
    return m_parameterName;
}

void ZWLookupAction::setParameterName(const std::wstring& parameterName)
{
    m_parameterName = parameterName;
}

std::wstring ZWLookupAction::getTableName() const
{
    return m_tableName;
}

void ZWLookupAction::setTableName(const std::wstring& tableName)
{
    m_tableName = tableName;
}

// 参数和动作关联
ZcDbObjectId ZWLookupAction::getParameterId() const
{
    return m_parameterId;
}

void ZWLookupAction::setParameterId(const ZcDbObjectId& parameterId)
{
    m_parameterId = parameterId;
}

ZcDbObjectId ZWLookupAction::getActionId() const
{
    return m_actionId;
}

void ZWLookupAction::setActionId(const ZcDbObjectId& actionId)
{
    m_actionId = actionId;
}

ZcDbObjectId ZWLookupAction::getBlockId() const
{
    return m_blockId;
}

void ZWLookupAction::setBlockId(const ZcDbObjectId& blockId)
{
    m_blockId = blockId;
}

// 查寻功能
bool ZWLookupAction::execute(const std::vector<std::wstring>& inputValues)
{
    // 保存原始状态用于回滚
    if (!saveOriginalState())
    {
        return false;
    }

    // 查找匹配的条目
    ZWLookupParameter* param = nullptr;
    // 这里需要获取查寻参数对象
    // 暂时模拟查寻结果
    
    ZWLookupTableEntry entry;
    entry.inputValues = inputValues;
    entry.lookupValues = { L"100", L"200" };

    // 应用查寻结果
    bool result = applyResult(entry);
    if (!result)
    {
        rollback();
    }
    
    return result;
}

bool ZWLookupAction::executeReverse(const std::vector<std::wstring>& lookupValues)
{
    // 保存原始状态用于回滚
    if (!saveOriginalState())
    {
        return false;
    }

    // 反向查寻：根据查寻值查找输入值
    ZWLookupTableEntry entry;
    entry.inputValues = { L"选项1" };
    entry.lookupValues = lookupValues;

    // 应用查寻结果
    bool result = applyResult(entry);
    if (!result)
    {
        rollback();
    }
    
    return result;
}

// 查寻结果应用
bool ZWLookupAction::applyResult(const ZWLookupTableEntry& entry)
{
    // 更新块属性
    if (!updateBlockProperties(entry))
    {
        return false;
    }

    // 更新动态属性
    if (!updateDynamicProperties(entry))
    {
        return false;
    }

    return true;
}

// 预览功能
bool ZWLookupAction::preview(const std::vector<std::wstring>& inputValues)
{
    // 检查是否已经有活动的预览
    if (m_isPreviewActive)
    {
        cancelPreview();
    }

    m_isPreviewActive = true;

    // 保存原始状态
    if (!saveOriginalState())
    {
        m_isPreviewActive = false;
        return false;
    }

    // 查找匹配的条目
    ZWLookupTableEntry entry;
    entry.inputValues = inputValues;
    entry.lookupValues = { L"100", L"200" };

    // 应用预览结果
    bool result = applyResult(entry);
    if (!result)
    {
        cancelPreview();
    }
    
    return result;
}

void ZWLookupAction::cancelPreview()
{
    if (m_isPreviewActive)
    {
        // 恢复原始状态
        restoreOriginalState();
        m_isPreviewActive = false;
    }
}

// 回滚机制
bool ZWLookupAction::rollback()
{
    return restoreOriginalState();
}

// 验证功能
bool ZWLookupAction::validate() const
{
    m_validationErrors.clear();
    
    // 检查参数名称是否为空
    if (m_parameterName.empty())
    {
        m_validationErrors.push_back(L"查寻动作必须关联一个参数。");
    }
    
    // 检查查寻表名称是否为空
    if (m_tableName.empty())
    {
        m_validationErrors.push_back(L"查寻动作必须关联一个查寻表。");
    }
    
    // 检查块ID是否有效
    if (m_blockId.isNull())
    {
        m_validationErrors.push_back(L"查寻动作必须关联一个块。");
    }
    
    // 检查参数ID是否有效
    if (m_parameterId.isNull())
    {
        m_validationErrors.push_back(L"查寻动作必须关联一个参数对象。");
    }
    
    // 检查动作ID是否有效
    if (m_actionId.isNull())
    {
        m_validationErrors.push_back(L"查寻动作必须有一个有效的动作ID。");
    }
    
    return m_validationErrors.empty();
}

std::vector<std::wstring> ZWLookupAction::getValidationErrors() const
{
    return m_validationErrors;
}

// 动作上下文管理
void ZWLookupAction::setContext(ZcDbObjectId blockId, ZcDbObjectId parameterId, ZcDbObjectId actionId)
{
    m_blockId = blockId;
    m_parameterId = parameterId;
    m_actionId = actionId;
}

// 序列化
Zcad::ErrorStatus ZWLookupAction::dwgInFields(ZcDbDwgFiler* filer)
{
    // 实现DWG导入
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupAction::dwgOutFields(ZcDbDwgFiler* filer) const
{
    // 实现DWG导出
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupAction::dxfInFields(ZcDbDxfFiler* filer)
{
    // 实现DXF导入
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupAction::dxfOutFields(ZcDbDxfFiler* filer) const
{
    // 实现DXF导出
    // ...
    return Zcad::eOk;
}

// 内部方法
bool ZWLookupAction::updateBlockProperties(const ZWLookupTableEntry& entry)
{
    // 更新块的属性
    // ...
    return true;
}

bool ZWLookupAction::updateDynamicProperties(const ZWLookupTableEntry& entry)
{
    // 更新块的动态属性
    // ...
    return true;
}

bool ZWLookupAction::saveOriginalState()
{
    // 保存原始状态用于回滚
    m_modifiedObjects.clear();
    m_originalData.clear();
    
    // 这里需要实现保存原始状态的逻辑
    // ...
    
    return true;
}

bool ZWLookupAction::restoreOriginalState()
{
    // 恢复原始状态
    // ...
    
    m_modifiedObjects.clear();
    m_originalData.clear();
    
    return true;
}

// 查寻动作工厂类实现

// 构造函数
ZWLookupActionFactory::ZWLookupActionFactory()
{}

// 析构函数
ZWLookupActionFactory::~ZWLookupActionFactory()
{}

// 创建查寻动作
ZWLookupAction* ZWLookupActionFactory::createAction(const ZWLookupActionProps& props)
{
    ZWLookupAction* action = new ZWLookupAction();
    action->setName(props.name);
    action->setParameterName(props.parameterName);
    action->setTableName(props.tableName);
    return action;
}

ZWLookupAction* ZWLookupActionFactory::createAction(const std::wstring& name, const std::wstring& parameterName, const std::wstring& tableName)
{
    ZWLookupActionProps props;
    props.name = name;
    props.parameterName = parameterName;
    props.tableName = tableName;
    return createAction(props);
}

// 注册和注销工厂
void ZWLookupActionFactory::registerFactory()
{
    // 注册查寻动作工厂
    // ...
}

void ZWLookupActionFactory::unregisterFactory()
{
    // 注销查寻动作工厂
    // ...
}
