// ZWLookupSet.cpp
// 查寻集实现

#include "stdafx.h"
#include "ZWLookupSet.h"

// 注册查寻集类
ZCRX_CONS_DEFINE_MEMBERS(ZWLookupSet, ZcRxObject, 1);

// 注册查寻集管理器类
ZCRX_CONS_DEFINE_MEMBERS(ZWLookupSetManager, ZcRxObject, 1);

// 构造函数
ZWLookupSet::ZWLookupSet()
    : m_name(L"查寻集1")
    , m_description(L"")
    , m_currentEntryIndex(0)
{}

// 析构函数
ZWLookupSet::~ZWLookupSet()
{}

// 查寻集属性访问器
std::wstring ZWLookupSet::getName() const
{
    return m_name;
}

void ZWLookupSet::setName(const std::wstring& name)
{
    m_name = name;
}

std::wstring ZWLookupSet::getDescription() const
{
    return m_description;
}

void ZWLookupSet::setDescription(const std::wstring& description)
{
    m_description = description;
}

// 参数关联管理
bool ZWLookupSet::addParameter(const std::wstring& parameterName)
{
    // 检查参数是否已存在
    for (const auto& param : m_parameterNames)
    {
        if (param == parameterName)
        {
            return false;
        }
    }

    m_parameterNames.push_back(parameterName);
    return true;
}

bool ZWLookupSet::removeParameter(const std::wstring& parameterName)
{
    auto it = std::find(m_parameterNames.begin(), m_parameterNames.end(), parameterName);
    if (it != m_parameterNames.end())
    {
        m_parameterNames.erase(it);
        return true;
    }
    return false;
}

bool ZWLookupSet::containsParameter(const std::wstring& parameterName) const
{
    return std::find(m_parameterNames.begin(), m_parameterNames.end(), parameterName) != m_parameterNames.end();
}

int ZWLookupSet::getParameterCount() const
{
    return (int)m_parameterNames.size();
}

std::vector<std::wstring> ZWLookupSet::getParameterNames() const
{
    return m_parameterNames;
}

// 参数ID关联
bool ZWLookupSet::addParameterId(const ZcDbObjectId& parameterId)
{
    // 检查参数ID是否已存在
    for (const auto& id : m_parameterIds)
    {
        if (id == parameterId)
        {
            return false;
        }
    }

    m_parameterIds.push_back(parameterId);
    return true;
}

bool ZWLookupSet::removeParameterId(const ZcDbObjectId& parameterId)
{
    auto it = std::find(m_parameterIds.begin(), m_parameterIds.end(), parameterId);
    if (it != m_parameterIds.end())
    {
        m_parameterIds.erase(it);
        return true;
    }
    return false;
}

bool ZWLookupSet::containsParameterId(const ZcDbObjectId& parameterId) const
{
    return std::find(m_parameterIds.begin(), m_parameterIds.end(), parameterId) != m_parameterIds.end();
}

std::vector<ZcDbObjectId> ZWLookupSet::getParameterIds() const
{
    return m_parameterIds;
}

// 查寻集条目管理
bool ZWLookupSet::addEntry(const ZWLookupTableEntry& entry)
{
    m_entries.push_back(entry);
    return true;
}

bool ZWLookupSet::removeEntry(int index)
{
    if (index < 0 || index >= (int)m_entries.size())
    {
        return false;
    }

    m_entries.erase(m_entries.begin() + index);
    
    // 如果删除的是当前条目，调整当前条目索引
    if (m_currentEntryIndex == index)
    {
        m_currentEntryIndex = std::max(0, std::min(m_currentEntryIndex, (int)m_entries.size() - 1));
    }
    else if (m_currentEntryIndex > index)
    {
        m_currentEntryIndex--;
    }
    
    return true;
}

bool ZWLookupSet::updateEntry(int index, const ZWLookupTableEntry& entry)
{
    if (index < 0 || index >= (int)m_entries.size())
    {
        return false;
    }

    m_entries[index] = entry;
    return true;
}

int ZWLookupSet::getEntryCount() const
{
    return (int)m_entries.size();
}

ZWLookupTableEntry ZWLookupSet::getEntry(int index) const
{
    if (index < 0 || index >= (int)m_entries.size())
    {
        return ZWLookupTableEntry();
    }

    return m_entries[index];
}

// 当前条目管理
int ZWLookupSet::getCurrentEntryIndex() const
{
    return m_currentEntryIndex;
}

void ZWLookupSet::setCurrentEntryIndex(int index)
{
    if (index >= 0 && index < (int)m_entries.size())
    {
        m_currentEntryIndex = index;
    }
}

ZWLookupTableEntry ZWLookupSet::getCurrentEntry() const
{
    return getEntry(m_currentEntryIndex);
}

bool ZWLookupSet::setCurrentEntry(const ZWLookupTableEntry& entry)
{
    // 查找匹配的条目
    for (size_t i = 0; i < m_entries.size(); i++)
    {
        const auto& existingEntry = m_entries[i];
        if (existingEntry.inputValues == entry.inputValues && existingEntry.lookupValues == entry.lookupValues)
        {
            m_currentEntryIndex = (int)i;
            return true;
        }
    }
    return false;
}

// 查寻功能
ZWLookupTableEntry ZWLookupSet::findEntry(const std::vector<std::wstring>& inputValues, ZWLookupDirection direction) const
{
    std::vector<ZWLookupTableEntry> entries = findEntries(inputValues, direction);
    if (!entries.empty())
    {
        return entries[0];
    }
    return ZWLookupTableEntry();
}

std::vector<ZWLookupTableEntry> ZWLookupSet::findEntries(const std::vector<std::wstring>& inputValues, ZWLookupDirection direction) const
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

// 查寻集验证
bool ZWLookupSet::validate() const
{
    m_validationErrors.clear();
    
    // 检查是否关联了参数
    if (m_parameterNames.empty() && m_parameterIds.empty())
    {
        m_validationErrors.push_back(L"查寻集必须至少关联一个参数。");
    }
    
    // 检查是否有条目
    if (m_entries.empty())
    {
        m_validationErrors.push_back(L"查寻集必须至少包含一个条目。");
    }
    
    // 检查当前条目索引是否有效
    if (m_currentEntryIndex < 0 || m_currentEntryIndex >= (int)m_entries.size())
    {
        m_validationErrors.push_back(L"当前条目索引无效。");
    }
    
    return m_validationErrors.empty();
}

std::vector<std::wstring> ZWLookupSet::getValidationErrors() const
{
    return m_validationErrors;
}

// 序列化
Zcad::ErrorStatus ZWLookupSet::dwgInFields(ZcDbDwgFiler* filer)
{
    // 实现DWG导入
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupSet::dwgOutFields(ZcDbDwgFiler* filer) const
{
    // 实现DWG导出
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupSet::dxfInFields(ZcDbDxfFiler* filer)
{
    // 实现DXF导入
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupSet::dxfOutFields(ZcDbDxfFiler* filer) const
{
    // 实现DXF导出
    // ...
    return Zcad::eOk;
}

// 查寻集管理器类实现

// 构造函数
ZWLookupSetManager::ZWLookupSetManager()
{}

// 析构函数
ZWLookupSetManager::~ZWLookupSetManager()
{}

// 查寻集管理
bool ZWLookupSetManager::addSet(const ZWLookupSet& set)
{
    // 检查查寻集是否已存在
    if (m_sets.find(set.getName()) != m_sets.end())
    {
        return false;
    }

    // 添加新查寻集
    m_sets[set.getName()] = std::make_unique<ZWLookupSet>(set);
    return true;
}

bool ZWLookupSetManager::removeSet(const std::wstring& name)
{
    auto it = m_sets.find(name);
    if (it != m_sets.end())
    {
        m_sets.erase(it);
        return true;
    }
    return false;
}

bool ZWLookupSetManager::updateSet(const ZWLookupSet& set)
{
    auto it = m_sets.find(set.getName());
    if (it != m_sets.end())
    {
        it->second = std::make_unique<ZWLookupSet>(set);
        return true;
    }
    return false;
}

ZWLookupSet* ZWLookupSetManager::getSet(const std::wstring& name) const
{
    auto it = m_sets.find(name);
    if (it != m_sets.end())
    {
        return it->second.get();
    }
    return nullptr;
}

int ZWLookupSetManager::getSetCount() const
{
    return (int)m_sets.size();
}

std::vector<std::wstring> ZWLookupSetManager::getSetNames() const
{
    std::vector<std::wstring> names;
    for (const auto& pair : m_sets)
    {
        names.push_back(pair.first);
    }
    return names;
}

// 序列化
Zcad::ErrorStatus ZWLookupSetManager::dwgInFields(ZcDbDwgFiler* filer)
{
    // 实现DWG导入
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupSetManager::dwgOutFields(ZcDbDwgFiler* filer) const
{
    // 实现DWG导出
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupSetManager::dxfInFields(ZcDbDxfFiler* filer)
{
    // 实现DXF导入
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupSetManager::dxfOutFields(ZcDbDxfFiler* filer) const
{
    // 实现DXF导出
    // ...
    return Zcad::eOk;
}
