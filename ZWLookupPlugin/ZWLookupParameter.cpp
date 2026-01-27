// ZWLookupParameter.cpp
// 查寻参数实现

#include "stdafx.h"
#include "ZWLookupParameter.h"

// 注册查寻参数类
ZCRX_CONS_DEFINE_MEMBERS(ZWLookupParameter, ZcRxObject, 1);

// 注册查寻参数工厂类
ZCRX_CONS_DEFINE_MEMBERS(ZWLookupParameterFactory, ZcRxObject, 1);

// 构造函数
ZWLookupParameter::ZWLookupParameter()
    : m_name(L"查寻1")
    , m_label(L"查寻1")
    , m_description(L"")
    , m_showLabel(true)
    , m_showGrip(true)
    , m_gripCount(1)
    , m_position(ZcGePoint3d::kOrigin)
    , m_tableName(L"")
    , m_setName(L"")
    , m_blockId(ZcDbObjectId::kNull)
    , m_parameterId(ZcDbObjectId::kNull)
    , m_actionId(ZcDbObjectId::kNull)
{}

// 析构函数
ZWLookupParameter::~ZWLookupParameter()
{}

// 参数属性访问器
std::wstring ZWLookupParameter::getName() const
{
    return m_name;
}

void ZWLookupParameter::setName(const std::wstring& name)
{
    m_name = name;
}

std::wstring ZWLookupParameter::getLabel() const
{
    return m_label;
}

void ZWLookupParameter::setLabel(const std::wstring& label)
{
    m_label = label;
}

std::wstring ZWLookupParameter::getDescription() const
{
    return m_description;
}

void ZWLookupParameter::setDescription(const std::wstring& description)
{
    m_description = description;
}

bool ZWLookupParameter::isLabelShown() const
{
    return m_showLabel;
}

void ZWLookupParameter::setLabelShown(bool show)
{
    m_showLabel = show;
}

bool ZWLookupParameter::isGripShown() const
{
    return m_showGrip;
}

void ZWLookupParameter::setGripShown(bool show)
{
    m_showGrip = show;
}

int ZWLookupParameter::getGripCount() const
{
    return m_gripCount;
}

void ZWLookupParameter::setGripCount(int count)
{
    m_gripCount = count;
}

// 参数位置访问器
ZcGePoint3d ZWLookupParameter::getPosition() const
{
    return m_position;
}

void ZWLookupParameter::setPosition(const ZcGePoint3d& position)
{
    m_position = position;
}

// 查寻表关联
std::wstring ZWLookupParameter::getTableName() const
{
    return m_tableName;
}

void ZWLookupParameter::setTableName(const std::wstring& tableName)
{
    m_tableName = tableName;
}

// 查寻集关联
std::wstring ZWLookupParameter::getSetName() const
{
    return m_setName;
}

void ZWLookupParameter::setSetName(const std::wstring& setName)
{
    m_setName = setName;
}

// 查寻功能
ZWLookupTableEntry ZWLookupParameter::findEntry(const std::vector<std::wstring>& inputValues, ZWLookupDirection direction) const
{
    // 如果有查寻表关联，使用查寻表进行查寻
    if (!m_tableName.empty())
    {
        // 这里需要获取查寻表管理器并进行查寻
        // 暂时使用内部条目进行查寻
    }

    // 使用内部条目进行查寻
    for (const auto& entry : m_entries)
    {
        bool match = true;
        if (direction == ZWLookupDirection::Forward)
        {
            // 正向查寻：根据输入值查找查寻值
            for (size_t i = 0; i < inputValues.size() && i < entry.inputValues.size(); i++)
            {
                if (inputValues[i] != entry.inputValues[i])
                {
                    match = false;
                    break;
                }
            }
        }
        else
        {
            // 反向查寻：根据查寻值查找输入值
            for (size_t i = 0; i < inputValues.size() && i < entry.lookupValues.size(); i++)
            {
                if (inputValues[i] != entry.lookupValues[i])
                {
                    match = false;
                    break;
                }
            }
        }
        if (match)
        {
            return entry;
        }
    }

    return ZWLookupTableEntry();
}

bool ZWLookupParameter::addEntry(const ZWLookupTableEntry& entry)
{
    m_entries.push_back(entry);
    return true;
}

bool ZWLookupParameter::removeEntry(int index)
{
    if (index < 0 || index >= (int)m_entries.size())
    {
        return false;
    }

    m_entries.erase(m_entries.begin() + index);
    return true;
}

bool ZWLookupParameter::updateEntry(int index, const ZWLookupTableEntry& entry)
{
    if (index < 0 || index >= (int)m_entries.size())
    {
        return false;
    }

    m_entries[index] = entry;
    return true;
}

int ZWLookupParameter::getEntryCount() const
{
    return (int)m_entries.size();
}

ZWLookupTableEntry ZWLookupParameter::getEntry(int index) const
{
    if (index < 0 || index >= (int)m_entries.size())
    {
        return ZWLookupTableEntry();
    }

    return m_entries[index];
}

// 夹点管理
void ZWLookupParameter::updateGrips()
{
    // 更新夹点位置和状态
    // 这里可以添加实际的夹点位置更新逻辑
}

void ZWLookupParameter::drawGrips(ZcGiViewportDraw* pVd) const
{
    // 绘制向下三角形夹点（类似于AutoCAD的夹点）
    if (!m_showGrip || m_gripCount == 0)
        return;

    // 使用实际的参数位置作为夹点位置
    ZcGePoint3d gripPoint = m_position;

    // 绘制向下三角形夹点
    // 1. 设置夹点大小（确保可见）
    double gripSize = 5.0; // 夹点大小

    // 2. 设置夹点颜色为浅蓝色（使用颜色索引）
    // 浅蓝色对应的颜色索引
    pVd->subEntityTraits().setColor((ZSoft::UInt16)170); // 浅蓝色颜色索引

    // 3. 绘制向下三角形
    ZcGePoint3dArray trianglePoints;
    trianglePoints.append(ZcGePoint3d(gripPoint.x, gripPoint.y, gripPoint.z));
    trianglePoints.append(ZcGePoint3d(gripPoint.x - gripSize, gripPoint.y - gripSize * 1.5, gripPoint.z));
    trianglePoints.append(ZcGePoint3d(gripPoint.x + gripSize, gripPoint.y - gripSize * 1.5, gripPoint.z));
    trianglePoints.append(ZcGePoint3d(gripPoint.x, gripPoint.y, gripPoint.z));

    pVd->geometry().polygon(trianglePoints.length(), trianglePoints.asArrayPtr());
}

bool ZWLookupParameter::hitTestGrips(const ZcGePoint3d& pickPoint, double tolerance)
{
    // 夹点命中测试
    if (!m_showGrip || m_gripCount == 0)
        return false;

    // 使用实际的参数位置作为夹点位置
    ZcGePoint3d gripPoint = m_position;

    // 检查拾取点是否在夹点范围内
    double distance = gripPoint.distanceTo(pickPoint);
    return distance <= tolerance;
}

// 序列化
Zcad::ErrorStatus ZWLookupParameter::dwgInFields(ZcDbDwgFiler* filer)
{
    // 实现DWG导入
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupParameter::dwgOutFields(ZcDbDwgFiler* filer) const
{
    // 实现DWG导出
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupParameter::dxfInFields(ZcDbDxfFiler* filer)
{
    // 实现DXF导入
    // ...
    return Zcad::eOk;
}

Zcad::ErrorStatus ZWLookupParameter::dxfOutFields(ZcDbDxfFiler* filer) const
{
    // 实现DXF导出
    // ...
    return Zcad::eOk;
}

// 动态块参数接口
void ZWLookupParameter::evaluate()
{
    // 评估参数值
    // ...
}

void ZWLookupParameter::updateReferences()
{
    // 更新参数引用
    // ...
}

void ZWLookupParameter::updateProperties()
{
    // 更新参数属性
    // ...
}

// 查寻参数工厂类实现

// 构造函数
ZWLookupParameterFactory::ZWLookupParameterFactory()
{}

// 析构函数
ZWLookupParameterFactory::~ZWLookupParameterFactory()
{}

// 创建查寻参数
ZWLookupParameter* ZWLookupParameterFactory::createParameter(const ZWLookupParameterProps& props)
{
    ZWLookupParameter* param = new ZWLookupParameter();
    param->setName(props.name);
    param->setLabel(props.label);
    param->setDescription(props.description);
    param->setLabelShown(props.showLabel);
    param->setGripShown(props.showGrip);
    param->setGripCount(props.gripCount);
    // 注意：参数位置需要在创建后单独设置
    return param;
}

ZWLookupParameter* ZWLookupParameterFactory::createParameter(const std::wstring& name, const std::wstring& label)
{
    ZWLookupParameterProps props;
    props.name = name;
    props.label = label;
    props.description = L"";
    props.showLabel = true;
    props.showGrip = true;
    props.gripCount = 1;
    return createParameter(props);
}

// 注册和注销工厂
void ZWLookupParameterFactory::registerFactory()
{
    // 注册查寻参数工厂
    // ...
}

void ZWLookupParameterFactory::unregisterFactory()
{
    // 注销查寻参数工厂
    // ...
}
