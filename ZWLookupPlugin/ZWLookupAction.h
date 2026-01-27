// ZWLookupAction.h
// 查寻动作实现

#pragma once

#include "stdafx.h"
#include "ZWLookupDefs.h"

class ZWLookupAction : public ZcRxObject
{
public:
    ZCRX_DECLARE_MEMBERS(ZWLookupAction);

public:
    // 构造函数和析构函数
    ZWLookupAction();
    virtual ~ZWLookupAction();

    // 动作属性访问器
    std::wstring getName() const;
    void setName(const std::wstring& name);

    std::wstring getParameterName() const;
    void setParameterName(const std::wstring& parameterName);

    std::wstring getTableName() const;
    void setTableName(const std::wstring& tableName);

    // 参数和动作关联
    ZcDbObjectId getParameterId() const;
    void setParameterId(const ZcDbObjectId& parameterId);

    ZcDbObjectId getActionId() const;
    void setActionId(const ZcDbObjectId& actionId);

    ZcDbObjectId getBlockId() const;
    void setBlockId(const ZcDbObjectId& blockId);

    // 查寻功能
    bool execute(const std::vector<std::wstring>& inputValues);
    bool executeReverse(const std::vector<std::wstring>& lookupValues);
    
    // 查寻结果应用
    bool applyResult(const ZWLookupTableEntry& entry);
    
    // 预览功能
    bool preview(const std::vector<std::wstring>& inputValues);
    void cancelPreview();
    
    // 回滚机制
    bool rollback();
    
    // 验证功能
    bool validate() const;
    std::vector<std::wstring> getValidationErrors() const;
    
    // 动作上下文管理
    void setContext(ZcDbObjectId blockId, ZcDbObjectId parameterId, ZcDbObjectId actionId);
    
    // 序列化
    virtual Zcad::ErrorStatus dwgInFields(ZcDbDwgFiler* filer);
    virtual Zcad::ErrorStatus dwgOutFields(ZcDbDwgFiler* filer) const;
    virtual Zcad::ErrorStatus dxfInFields(ZcDbDxfFiler* filer);
    virtual Zcad::ErrorStatus dxfOutFields(ZcDbDxfFiler* filer) const;

protected:
    // 内部数据结构
    std::wstring m_name;
    std::wstring m_parameterName;
    std::wstring m_tableName;
    
    ZcDbObjectId m_blockId;
    ZcDbObjectId m_parameterId;
    ZcDbObjectId m_actionId;
    
    // 预览和回滚数据
    bool m_isPreviewActive;
    std::vector<ZcDbObjectId> m_modifiedObjects;
    std::vector<std::pair<ZcDbObjectId, std::vector<std::byte>>> m_originalData;
    
    // 验证错误
    mutable std::vector<std::wstring> m_validationErrors;
    
    // 内部方法
    bool updateBlockProperties(const ZWLookupTableEntry& entry);
    bool updateDynamicProperties(const ZWLookupTableEntry& entry);
    bool saveOriginalState();
    bool restoreOriginalState();
};

// 查寻动作工厂类
class ZWLookupActionFactory : public ZcRxObject
{
public:
    ZCRX_DECLARE_MEMBERS(ZWLookupActionFactory);

public:
    // 构造函数和析构函数
    ZWLookupActionFactory();
    virtual ~ZWLookupActionFactory();
    
    // 创建查寻动作
    static ZWLookupAction* createAction(const ZWLookupActionProps& props);
    static ZWLookupAction* createAction(const std::wstring& name, const std::wstring& parameterName, const std::wstring& tableName);
    
    // 注册和注销工厂
    static void registerFactory();
    static void unregisterFactory();
};
