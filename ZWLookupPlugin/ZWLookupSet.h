// ZWLookupSet.h
// 查寻集实现

#pragma once

#include "stdafx.h"
#include "ZWLookupDefs.h"

class ZWLookupSet : public ZcRxObject
{
public:
    ZCRX_DECLARE_MEMBERS(ZWLookupSet);

public:
    // 构造函数和析构函数
    ZWLookupSet();
    virtual ~ZWLookupSet();

    // 查寻集属性访问器
    std::wstring getName() const;
    void setName(const std::wstring& name);

    std::wstring getDescription() const;
    void setDescription(const std::wstring& description);

    // 参数关联管理
    bool addParameter(const std::wstring& parameterName);
    bool removeParameter(const std::wstring& parameterName);
    bool containsParameter(const std::wstring& parameterName) const;
    int getParameterCount() const;
    std::vector<std::wstring> getParameterNames() const;

    // 参数ID关联
    bool addParameterId(const ZcDbObjectId& parameterId);
    bool removeParameterId(const ZcDbObjectId& parameterId);
    bool containsParameterId(const ZcDbObjectId& parameterId) const;
    std::vector<ZcDbObjectId> getParameterIds() const;

    // 查寻集条目管理
    bool addEntry(const ZWLookupTableEntry& entry);
    bool removeEntry(int index);
    bool updateEntry(int index, const ZWLookupTableEntry& entry);
    int getEntryCount() const;
    ZWLookupTableEntry getEntry(int index) const;

    // 当前条目管理
    int getCurrentEntryIndex() const;
    void setCurrentEntryIndex(int index);
    ZWLookupTableEntry getCurrentEntry() const;
    bool setCurrentEntry(const ZWLookupTableEntry& entry);

    // 查寻功能
    ZWLookupTableEntry findEntry(const std::vector<std::wstring>& inputValues, ZWLookupDirection direction = ZWLookupDirection::Forward) const;
    std::vector<ZWLookupTableEntry> findEntries(const std::vector<std::wstring>& inputValues, ZWLookupDirection direction = ZWLookupDirection::Forward) const;

    // 查寻集验证
    bool validate() const;
    std::vector<std::wstring> getValidationErrors() const;

    // 序列化
    virtual Zcad::ErrorStatus dwgInFields(ZcDbDwgFiler* filer);
    virtual Zcad::ErrorStatus dwgOutFields(ZcDbDwgFiler* filer) const;
    virtual Zcad::ErrorStatus dxfInFields(ZcDbDxfFiler* filer);
    virtual Zcad::ErrorStatus dxfOutFields(ZcDbDxfFiler* filer) const;

protected:
    // 内部数据结构
    std::wstring m_name;
    std::wstring m_description;
    
    std::vector<std::wstring> m_parameterNames;
    std::vector<ZcDbObjectId> m_parameterIds;
    std::vector<ZWLookupTableEntry> m_entries;
    int m_currentEntryIndex;
    
    // 验证错误
    mutable std::vector<std::wstring> m_validationErrors;
};

// 查寻集管理器类
class ZWLookupSetManager : public ZcRxObject
{
public:
    ZCRX_DECLARE_MEMBERS(ZWLookupSetManager);

    // 构造函数和析构函数
    ZWLookupSetManager();
    virtual ~ZWLookupSetManager();

    // 禁用复制
    ZWLookupSetManager(const ZWLookupSetManager&) = delete;
    ZWLookupSetManager& operator=(const ZWLookupSetManager&) = delete;

    // 查寻集管理
    bool addSet(const ZWLookupSet& set);
    bool removeSet(const std::wstring& name);
    bool updateSet(const ZWLookupSet& set);
    ZWLookupSet* getSet(const std::wstring& name) const;
    int getSetCount() const;
    std::vector<std::wstring> getSetNames() const;

    // 序列化
    virtual Zcad::ErrorStatus dwgInFields(ZcDbDwgFiler* filer);
    virtual Zcad::ErrorStatus dwgOutFields(ZcDbDwgFiler* filer) const;
    virtual Zcad::ErrorStatus dxfInFields(ZcDbDxfFiler* filer);
    virtual Zcad::ErrorStatus dxfOutFields(ZcDbDxfFiler* filer) const;

protected:
    // 内部数据结构
    std::map<std::wstring, std::unique_ptr<ZWLookupSet>> m_sets;
};
