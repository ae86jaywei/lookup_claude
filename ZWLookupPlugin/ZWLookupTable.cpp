// ZWLookupTable.cpp
// 查寻表实现

#include "stdafx.h"
#include "ZWLookupTable.h"

// 注册查寻表类
ZCRX_CONS_DEFINE_MEMBERS(ZWLookupTable, ZcRxObject, 1);

// 构造函数
ZWLookupTable::ZWLookupTable()
    : m_name(L"查寻表")
    , m_description(L"")
{}

// 析构函数
ZWLookupTable::~ZWLookupTable()
{}

// 表属性访问器
std::wstring ZWLookupTable::getName() const
{
    return m_name;
}

void ZWLookupTable::setName(const std::wstring& name)
{
    m_name = name;
}

std::wstring ZWLookupTable::getDescription() const
{
    return m_description;
}

void ZWLookupTable::setDescription(const std::wstring& description)
{
    m_description = description;
}

// 特性管理
bool ZWLookupTable::addProperty(const std::wstring& name, ZWLookupPropertyType type)
{
    // 检查特性是否已存在
    for (const auto& prop : m_properties)
    {
        if (prop.name == name)
        {
            return false;
        }
    }

    // 添加新特性
    PropertyInfo propInfo;
    propInfo.name = name;
    propInfo.type = type;
    m_properties.push_back(propInfo);
    return true;
}

bool ZWLookupTable::removeProperty(int index)
{
    if (index < 0 || index >= (int)m_properties.size())
    {
        return false;
    }

    m_properties.erase(m_properties.begin() + index);
    return true;
}

bool ZWLookupTable::updateProperty(int index, const std::wstring& name, ZWLookupPropertyType type)
{
    if (index < 0 || index >= (int)m_properties.size())
    {
        return false;
    }

    // 检查新名称是否与其他特性冲突
    for (size_t i = 0; i < m_properties.size(); i++)
    {
        if (i != (size_t)index && m_properties[i].name == name)
        {
            return false;
        }
    }

    m_properties[index].name = name;
    m_properties[index].type = type;
    return true;
}

int ZWLookupTable::getPropertyCount() const
{
    return (int)m_properties.size();
}

std::wstring ZWLookupTable::getPropertyName(int index) const
{
    if (index < 0 || index >= (int)m_properties.size())
    {
        return L"";
    }

    return m_properties[index].name;
}

ZWLookupPropertyType ZWLookupTable::getPropertyType(int index) const
{
    if (index < 0 || index >= (int)m_properties.size())
    {
        return ZWLookupPropertyType::Input;
    }

    return m_properties[index].type;
}

// 输入特性管理
int ZWLookupTable::getInputPropertyCount() const
{
    int count = 0;
    for (const auto& prop : m_properties)
    {
        if (prop.type == ZWLookupPropertyType::Input)
        {
            count++;
        }
    }
    return count;
}

std::wstring ZWLookupTable::getInputPropertyName(int index) const
{
    int inputIndex = 0;
    for (const auto& prop : m_properties)
    {
        if (prop.type == ZWLookupPropertyType::Input)
        {
            if (inputIndex == index)
            {
                return prop.name;
            }
            inputIndex++;
        }
    }
    return L"";
}

// 查寻特性管理
int ZWLookupTable::getLookupPropertyCount() const
{
    int count = 0;
    for (const auto& prop : m_properties)
    {
        if (prop.type == ZWLookupPropertyType::Lookup)
        {
            count++;
        }
    }
    return count;
}

std::wstring ZWLookupTable::getLookupPropertyName(int index) const
{
    int lookupIndex = 0;
    for (const auto& prop : m_properties)
    {
        if (prop.type == ZWLookupPropertyType::Lookup)
        {
            if (lookupIndex == index)
            {
                return prop.name;
            }
            lookupIndex++;
        }
    }
    return L"";
}

// 查寻表条目管理
bool ZWLookupTable::addEntry(const ZWLookupTableEntry& entry)
{
    // 检查条目格式是否与特性匹配
    int inputPropCount = getInputPropertyCount();
    int lookupPropCount = getLookupPropertyCount();
    
    if ((int)entry.inputValues.size() != inputPropCount || (int)entry.lookupValues.size() != lookupPropCount)
    {
        return false;
    }

    m_entries.push_back(entry);
    return true;
}

bool ZWLookupTable::removeEntry(int index)
{
    if (index < 0 || index >= (int)m_entries.size())
    {
        return false;
    }

    m_entries.erase(m_entries.begin() + index);
    return true;
}

bool ZWLookupTable::updateEntry(int index, const ZWLookupTableEntry& entry)
{
    if (index < 0 || index >= (int)m_entries.size())
    {
        return false;
    }

    // 检查条目格式是否与特性匹配
    int inputPropCount = getInputPropertyCount();
    int lookupPropCount = getLookupPropertyCount();
    
    if ((int)entry.inputValues.size() != inputPropCount || (int)entry.lookupValues.size() != lookupPropCount)
    {
        return false;
    }

    m_entries[index] = entry;
    return true;
}

int ZWLookupTable::getEntryCount() const
{
    return (int)m_entries.size();
}

ZWLookupTableEntry ZWLookupTable::getEntry(int index) const
{
    if (index < 0 || index >= (int)m_entries.size())
    {
        return ZWLookupTableEntry();
    }

    return m_entries[index];
}

// 查寻功能
ZWLookupTableEntry ZWLookupTable::findEntry(const std::vector<std::wstring>& inputValues, ZWLookupDirection direction) const
{
    std::vector<ZWLookupTableEntry> entries = findEntries(inputValues, direction);
    if (!entries.empty())
    {
        return entries[0];
    }
    return ZWLookupTableEntry();
}

std::vector<ZWLookupTableEntry> ZWLookupTable::findEntries(const std::vector<std::wstring>& inputValues, ZWLookupDirection direction) const
{
    std::vector<ZWLookupTableEntry> result;
    
    if (direction == ZWLookupDirection::Forward)
    {
        // 正向查寻：根据输入值查找查寻值
        for (const auto& entry : m_entries)
        {
            bool match = true;
            for (size_t i = 0; i < inputValues.size() && i < entry.inputValues.size(); i++)
            {
                if (inputValues[i] != entry.inputValues[i])
                {
                    match = false;
                    break;
                }
            }
            if (match)
            {
                result.push_back(entry);
            }
        }
    }
    else
    {
        // 反向查寻：根据查寻值查找输入值
        for (const auto& entry : m_entries)
        {
            bool match = true;
            for (size_t i = 0; i < inputValues.size() && i < entry.lookupValues.size(); i++)
            {
                if (inputValues[i] != entry.lookupValues[i])
                {
                    match = false;
                    break;
                }
            }
            if (match)
            {
                result.push_back(entry);
            }
        }
    }
    
    return result;
}

// 表验证
bool ZWLookupTable::validate() const
{
    m_validationErrors.clear();
    
    // 检查是否有特性
    if (m_properties.empty())
    {
        m_validationErrors.push_back(L"查寻表必须至少包含一个特性。");
    }
    
    // 检查是否有输入特性
    if (getInputPropertyCount() == 0)
    {
        m_validationErrors.push_back(L"查寻表必须至少包含一个输入特性。");
    }
    
    // 检查是否有查寻特性
    if (getLookupPropertyCount() == 0)
    {
        m_validationErrors.push_back(L"查寻表必须至少包含一个查寻特性。");
    }
    
    // 检查是否有条目
    if (m_entries.empty())
    {
        m_validationErrors.push_back(L"查寻表必须至少包含一个条目。");
    }
    
    // 检查条目格式是否正确
    int inputPropCount = getInputPropertyCount();
    int lookupPropCount = getLookupPropertyCount();
    
    for (size_t i = 0; i < m_entries.size(); i++)
    {
        const auto& entry = m_entries[i];
        if ((int)entry.inputValues.size() != inputPropCount || (int)entry.lookupValues.size() != lookupPropCount)
        {
            m_validationErrors.push_back(L"条目 " + std::to_wstring(i + 1) + L" 的格式与特性不匹配。");
        }
    }
    
    return m_validationErrors.empty();
}

std::vector<std::wstring> ZWLookupTable::getValidationErrors() const
{
    return m_validationErrors;
}

// 导入导出
bool ZWLookupTable::importFromCSV(const std::wstring& fileName)
{
    // 实现从CSV导入查寻表
    // ...
    return false;
}

bool ZWLookupTable::exportToCSV(const std::wstring& fileName) const
{
    // 实现导出查寻表到CSV
    // ...
    return false;
}

bool ZWLookupTable::importFromJSON(const std::wstring& fileName)
{
    // 实现从JSON导入查寻表
    // ...
    return false;
}

bool ZWLookupTable::exportToJSON(const std::wstring& fileName) const
{
    // 实现导出查寻表到JSON
    // ...
    return false;
}

// 序列化
Zcad::ErrorStatus ZWLookupTable::dwgInFields(ZcDbDwgFiler* filer)
{
    // 实现DWG导入
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupTable::dwgOutFields(ZcDbDwgFiler* filer) const
{
    // 实现DWG导出
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupTable::dxfInFields(ZcDbDxfFiler* filer)
{
    // 实现DXF导入
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupTable::dxfOutFields(ZcDbDxfFiler* filer) const
{
    // 实现DXF导出
    // ...
    return Zcad::eOk;
}

// 注册查寻表管理器类
ZCRX_CONS_DEFINE_MEMBERS(ZWLookupTableManager, ZcRxObject, 1);

// 构造函数
ZWLookupTableManager::ZWLookupTableManager()
{}

// 析构函数
ZWLookupTableManager::~ZWLookupTableManager()
{}

// 表管理
bool ZWLookupTableManager::addTable(const ZWLookupTable& table)
{
    // 检查表是否已存在
    if (m_tables.find(table.getName()) != m_tables.end())
    {
        return false;
    }

    // 添加新表
    m_tables[table.getName()] = std::make_unique<ZWLookupTable>(table);
    return true;
}

bool ZWLookupTableManager::removeTable(const std::wstring& name)
{
    auto it = m_tables.find(name);
    if (it != m_tables.end())
    {
        m_tables.erase(it);
        return true;
    }
    return false;
}

bool ZWLookupTableManager::updateTable(const ZWLookupTable& table)
{
    auto it = m_tables.find(table.getName());
    if (it != m_tables.end())
    {
        it->second = std::make_unique<ZWLookupTable>(table);
        return true;
    }
    return false;
}

ZWLookupTable* ZWLookupTableManager::getTable(const std::wstring& name) const
{
    auto it = m_tables.find(name);
    if (it != m_tables.end())
    {
        return it->second.get();
    }
    return nullptr;
}

int ZWLookupTableManager::getTableCount() const
{
    return (int)m_tables.size();
}

std::vector<std::wstring> ZWLookupTableManager::getTableNames() const
{
    std::vector<std::wstring> names;
    for (const auto& pair : m_tables)
    {
        names.push_back(pair.first);
    }
    return names;
}

// 序列化
Zcad::ErrorStatus ZWLookupTableManager::dwgInFields(ZcDbDwgFiler* filer)
{
    // 实现DWG导入
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupTableManager::dwgOutFields(ZcDbDwgFiler* filer) const
{
    // 实现DWG导出
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupTableManager::dxfInFields(ZcDbDxfFiler* filer)
{
    // 实现DXF导入
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupTableManager::dxfOutFields(ZcDbDxfFiler* filer) const
{
    // 实现DXF导出
    // ...
    return Zcad::eOk;
}
