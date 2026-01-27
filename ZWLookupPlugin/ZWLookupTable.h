// ZWLookupTable.h
// 查寻表实现

#pragma once

#include "stdafx.h"
#include "ZWLookupDefs.h"

class ZWLookupTable : public ZcRxObject
{
public:
    ZCRX_DECLARE_MEMBERS(ZWLookupTable);

public:
    // 构造函数和析构函数
    ZWLookupTable();
    virtual ~ZWLookupTable();

    // 表属性访问器
    std::wstring getName() const;
    void setName(const std::wstring& name);

    std::wstring getDescription() const;
    void setDescription(const std::wstring& description);

    // 特性管理
    bool addProperty(const std::wstring& name, ZWLookupPropertyType type);
    bool removeProperty(int index);
    bool updateProperty(int index, const std::wstring& name, ZWLookupPropertyType type);
    int getPropertyCount() const;
    std::wstring getPropertyName(int index) const;
    ZWLookupPropertyType getPropertyType(int index) const;

    // 输入特性管理
    int getInputPropertyCount() const;
    std::wstring getInputPropertyName(int index) const;

    // 查寻特性管理
    int getLookupPropertyCount() const;
    std::wstring getLookupPropertyName(int index) const;

    // 查寻表条目管理
    bool addEntry(const ZWLookupTableEntry& entry);
    bool removeEntry(int index);
    bool updateEntry(int index, const ZWLookupTableEntry& entry);
    int getEntryCount() const;
    ZWLookupTableEntry getEntry(int index) const;

    // 查寻功能
    ZWLookupTableEntry findEntry(const std::vector<std::wstring>& inputValues, ZWLookupDirection direction = ZWLookupDirection::Forward) const;
    std::vector<ZWLookupTableEntry> findEntries(const std::vector<std::wstring>& inputValues, ZWLookupDirection direction = ZWLookupDirection::Forward) const;

    // 表验证
    bool validate() const;
    std::vector<std::wstring> getValidationErrors() const;

    // 导入导出
    bool importFromCSV(const std::wstring& fileName);
    bool exportToCSV(const std::wstring& fileName) const;
    bool importFromJSON(const std::wstring& fileName);
    bool exportToJSON(const std::wstring& fileName) const;

    // 序列化
    virtual Zcad::ErrorStatus dwgInFields(ZcDbDwgFiler* filer);
    virtual Zcad::ErrorStatus dwgOutFields(ZcDbDwgFiler* filer) const;
    virtual Zcad::ErrorStatus dxfInFields(ZcDbDxfFiler* filer);
    virtual Zcad::ErrorStatus dxfOutFields(ZcDbDxfFiler* filer) const;

protected:
    // 内部数据结构
    std::wstring m_name;
    std::wstring m_description;
    
    struct PropertyInfo
    {
        std::wstring name;
        ZWLookupPropertyType type;
    };

    std::vector<PropertyInfo> m_properties;
    std::vector<ZWLookupTableEntry> m_entries;
    mutable std::vector<std::wstring> m_validationErrors;
};

// 查寻表管理器类
class ZWLookupTableManager : public ZcRxObject
{
public:
    ZCRX_DECLARE_MEMBERS(ZWLookupTableManager);

    // 构造函数和析构函数
    ZWLookupTableManager();
    virtual ~ZWLookupTableManager();

    // 禁用复制
    ZWLookupTableManager(const ZWLookupTableManager&) = delete;
    ZWLookupTableManager& operator=(const ZWLookupTableManager&) = delete;

    // 表管理
    bool addTable(const ZWLookupTable& table);
    bool removeTable(const std::wstring& name);
    bool updateTable(const ZWLookupTable& table);
    ZWLookupTable* getTable(const std::wstring& name) const;
    int getTableCount() const;
    std::vector<std::wstring> getTableNames() const;

    // 序列化
    virtual Zcad::ErrorStatus dwgInFields(ZcDbDwgFiler* filer);
    virtual Zcad::ErrorStatus dwgOutFields(ZcDbDwgFiler* filer) const;
    virtual Zcad::ErrorStatus dxfInFields(ZcDbDxfFiler* filer);
    virtual Zcad::ErrorStatus dxfOutFields(ZcDbDxfFiler* filer) const;

protected:
    // 内部数据结构
    std::map<std::wstring, std::unique_ptr<ZWLookupTable>> m_tables;
};
