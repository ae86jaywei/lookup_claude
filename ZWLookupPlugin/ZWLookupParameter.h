// ZWLookupParameter.h
// 查寻参数实现

#pragma once

#include "stdafx.h"
#include "ZWLookupDefs.h"

class ZWLookupParameter : public ZcRxObject
{
public:
    ZCRX_DECLARE_MEMBERS(ZWLookupParameter);

    // 构造函数和析构函数
    ZWLookupParameter();
    virtual ~ZWLookupParameter();

    // 参数属性访问器
    std::wstring getName() const;
    void setName(const std::wstring& name);

    std::wstring getLabel() const;
    void setLabel(const std::wstring& label);

    std::wstring getDescription() const;
    void setDescription(const std::wstring& description);

    bool isLabelShown() const;
    void setLabelShown(bool show);

    bool isGripShown() const;
    void setGripShown(bool show);

    int getGripCount() const;
    void setGripCount(int count);

    // 参数位置
    ZcGePoint3d getPosition() const;
    void setPosition(const ZcGePoint3d& position);

    // 查寻表关联
    std::wstring getTableName() const;
    void setTableName(const std::wstring& tableName);

    // 查寻集关联
    std::wstring getSetName() const;
    void setSetName(const std::wstring& setName);

    // 查寻功能
    ZWLookupTableEntry findEntry(const std::vector<std::wstring>& inputValues, ZWLookupDirection direction = ZWLookupDirection::Forward) const;
    bool addEntry(const ZWLookupTableEntry& entry);
    bool removeEntry(int index);
    bool updateEntry(int index, const ZWLookupTableEntry& entry);
    int getEntryCount() const;
    ZWLookupTableEntry getEntry(int index) const;

    // 夹点管理
    void updateGrips();
    void drawGrips(ZcGiViewportDraw* pVd) const;
    bool hitTestGrips(const ZcGePoint3d& pickPoint, double tolerance);

    // 序列化
    virtual Zcad::ErrorStatus dwgInFields(ZcDbDwgFiler* filer);
    virtual Zcad::ErrorStatus dwgOutFields(ZcDbDwgFiler* filer) const;
    virtual Zcad::ErrorStatus dxfInFields(ZcDbDxfFiler* filer);
    virtual Zcad::ErrorStatus dxfOutFields(ZcDbDxfFiler* filer) const;

    // 动态块参数接口
    virtual void evaluate();
    virtual void updateReferences();
    virtual void updateProperties();

private:
    // 参数属性
    std::wstring m_name;
    std::wstring m_label;
    std::wstring m_description;
    bool m_showLabel;
    bool m_showGrip;
    int m_gripCount;
    ZcGePoint3d m_position;
    std::wstring m_tableName;
    std::wstring m_setName;
    std::vector<ZWLookupTableEntry> m_entries;
    ZcDbObjectId m_blockId;
    ZcDbObjectId m_parameterId;
    ZcDbObjectId m_actionId;
};

// 查寻参数工厂类
class ZWLookupParameterFactory : public ZcRxObject
{
public:
    ZCRX_DECLARE_MEMBERS(ZWLookupParameterFactory);

public:
    ZWLookupParameterFactory();
    virtual ~ZWLookupParameterFactory();

    // 创建查寻参数
    static ZWLookupParameter* createParameter(const ZWLookupParameterProps& props);
    static ZWLookupParameter* createParameter(const std::wstring& name, const std::wstring& label);

    // 注册和注销工厂
    static void registerFactory();
    static void unregisterFactory();
};
