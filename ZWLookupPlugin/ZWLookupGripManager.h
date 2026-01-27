// ZWLookupGripManager.h
// 夹点管理器实现

#pragma once

#include "stdafx.h"
#include "ZWLookupDefs.h"

// 前向声明
class ZWLookupParameter;

// 夹点数据结构
struct ZWLookupGripData
{
    ZcDbObjectId parameterId;           // 关联的参数ID
    ZcGePoint3d position;               // 夹点位置
    ZWLookupGripState state;            // 夹点状态
    int gripIndex;                      // 夹点索引
    std::wstring label;                 // 夹点标签
    std::vector<std::wstring> options;  // 下拉选项
    int selectedOptionIndex;            // 当前选中的选项索引
    bool isVisible;                     // 是否可见
    bool isEnabled;                     // 是否启用
};

// 夹点菜单项
struct ZWLookupGripMenuItem
{
    std::wstring text;                  // 菜单项文本
    int id;                             // 菜单项ID
    bool isEnabled;                     // 是否启用
    bool isSeparator;                   // 是否为分隔符
    bool isChecked;                     // 是否选中
};

// 夹点管理器类
class ZWLookupGripManager : public ZcRxObject
{
public:
    ZCRX_DECLARE_MEMBERS(ZWLookupGripManager);

public:
    // 构造函数和析构函数
    ZWLookupGripManager();
    virtual ~ZWLookupGripManager();

    // 单例访问
    static ZWLookupGripManager* getInstance();
    static void destroyInstance();

    // 夹点注册和注销
    bool registerGrip(const ZWLookupGripData& gripData);
    bool unregisterGrip(const ZcDbObjectId& parameterId, int gripIndex);
    bool unregisterAllGrips(const ZcDbObjectId& parameterId);
    void clearAllGrips();

    // 夹点查询
    ZWLookupGripData* getGripData(const ZcDbObjectId& parameterId, int gripIndex);
    std::vector<ZWLookupGripData*> getGripsForParameter(const ZcDbObjectId& parameterId);
    std::vector<ZWLookupGripData*> getAllGrips();
    int getGripCount() const;
    int getGripCountForParameter(const ZcDbObjectId& parameterId) const;

    // 夹点状态管理
    bool setGripState(const ZcDbObjectId& parameterId, int gripIndex, ZWLookupGripState state);
    ZWLookupGripState getGripState(const ZcDbObjectId& parameterId, int gripIndex) const;
    bool setGripPosition(const ZcDbObjectId& parameterId, int gripIndex, const ZcGePoint3d& position);
    ZcGePoint3d getGripPosition(const ZcDbObjectId& parameterId, int gripIndex) const;
    bool setGripVisible(const ZcDbObjectId& parameterId, int gripIndex, bool visible);
    bool isGripVisible(const ZcDbObjectId& parameterId, int gripIndex) const;
    bool setGripEnabled(const ZcDbObjectId& parameterId, int gripIndex, bool enabled);
    bool isGripEnabled(const ZcDbObjectId& parameterId, int gripIndex) const;

    // 夹点选项管理
    bool setGripOptions(const ZcDbObjectId& parameterId, int gripIndex, const std::vector<std::wstring>& options);
    std::vector<std::wstring> getGripOptions(const ZcDbObjectId& parameterId, int gripIndex) const;
    bool setSelectedOption(const ZcDbObjectId& parameterId, int gripIndex, int optionIndex);
    int getSelectedOption(const ZcDbObjectId& parameterId, int gripIndex) const;
    std::wstring getSelectedOptionText(const ZcDbObjectId& parameterId, int gripIndex) const;

    // 夹点标签管理
    bool setGripLabel(const ZcDbObjectId& parameterId, int gripIndex, const std::wstring& label);
    std::wstring getGripLabel(const ZcDbObjectId& parameterId, int gripIndex) const;

    // 夹点命中测试
    bool hitTest(const ZcGePoint3d& pickPoint, double tolerance, ZcDbObjectId& outParameterId, int& outGripIndex);
    std::vector<std::pair<ZcDbObjectId, int>> hitTestAll(const ZcGePoint3d& pickPoint, double tolerance);

    // 夹点绘制
    void drawGrip(ZcGiViewportDraw* pVd, const ZWLookupGripData& gripData);
    void drawAllGrips(ZcGiViewportDraw* pVd);
    void drawGripsForParameter(ZcGiViewportDraw* pVd, const ZcDbObjectId& parameterId);

    // 夹点交互
    bool showDropdownMenu(const ZcDbObjectId& parameterId, int gripIndex, const ZcGePoint3d& screenPoint);
    bool showContextMenu(const ZcDbObjectId& parameterId, int gripIndex, const ZcGePoint3d& screenPoint);
    void hideAllMenus();

    // 下拉菜单管理
    void setDropdownMenuItems(const ZcDbObjectId& parameterId, int gripIndex, const std::vector<ZWLookupGripMenuItem>& items);
    std::vector<ZWLookupGripMenuItem> getDropdownMenuItems(const ZcDbObjectId& parameterId, int gripIndex) const;

    // 上下文菜单管理
    void setContextMenuItems(const ZcDbObjectId& parameterId, int gripIndex, const std::vector<ZWLookupGripMenuItem>& items);
    std::vector<ZWLookupGripMenuItem> getContextMenuItems(const ZcDbObjectId& parameterId, int gripIndex) const;

    // 夹点事件回调
    typedef void (*GripClickCallback)(const ZcDbObjectId& parameterId, int gripIndex, void* userData);
    typedef void (*GripHoverCallback)(const ZcDbObjectId& parameterId, int gripIndex, void* userData);
    typedef void (*GripOptionSelectedCallback)(const ZcDbObjectId& parameterId, int gripIndex, int optionIndex, void* userData);
    typedef void (*GripMenuItemSelectedCallback)(const ZcDbObjectId& parameterId, int gripIndex, int menuItemId, void* userData);

    void setGripClickCallback(GripClickCallback callback, void* userData = nullptr);
    void setGripHoverCallback(GripHoverCallback callback, void* userData = nullptr);
    void setGripOptionSelectedCallback(GripOptionSelectedCallback callback, void* userData = nullptr);
    void setGripMenuItemSelectedCallback(GripMenuItemSelectedCallback callback, void* userData = nullptr);

    // 夹点事件触发
    void onGripClicked(const ZcDbObjectId& parameterId, int gripIndex);
    void onGripHovered(const ZcDbObjectId& parameterId, int gripIndex);
    void onGripOptionSelected(const ZcDbObjectId& parameterId, int gripIndex, int optionIndex);
    void onGripMenuItemSelected(const ZcDbObjectId& parameterId, int gripIndex, int menuItemId);

    // 夹点外观设置
    void setGripSize(double size);
    double getGripSize() const;
    void setGripColor(ZWLookupGripState state, COLORREF color);
    COLORREF getGripColor(ZWLookupGripState state) const;
    void setLabelFont(const std::wstring& fontName, int fontSize);
    void setLabelColor(COLORREF color);
    COLORREF getLabelColor() const;

    // 夹点提示文本
    void setGripTooltip(const ZcDbObjectId& parameterId, int gripIndex, const std::wstring& tooltip);
    std::wstring getGripTooltip(const ZcDbObjectId& parameterId, int gripIndex) const;
    void showTooltip(const ZcDbObjectId& parameterId, int gripIndex, const ZcGePoint3d& screenPoint);
    void hideTooltip();

    // 刷新和更新
    void refresh();
    void refreshGrip(const ZcDbObjectId& parameterId, int gripIndex);
    void refreshGripsForParameter(const ZcDbObjectId& parameterId);

protected:
    // 内部辅助方法
    std::wstring makeGripKey(const ZcDbObjectId& parameterId, int gripIndex) const;
    void drawGripSymbol(ZcGiViewportDraw* pVd, const ZcGePoint3d& position, ZWLookupGripState state);
    void drawGripLabel(ZcGiViewportDraw* pVd, const ZcGePoint3d& position, const std::wstring& label);
    void drawDropdownArrow(ZcGiViewportDraw* pVd, const ZcGePoint3d& position);
    HWND createDropdownWindow(const ZcGePoint3d& screenPoint, const std::vector<std::wstring>& options);
    HWND createContextMenuWindow(const ZcGePoint3d& screenPoint, const std::vector<ZWLookupGripMenuItem>& items);

private:
    // 单例实例
    static ZWLookupGripManager* s_pInstance;

    // 夹点数据存储
    std::map<std::wstring, ZWLookupGripData> m_grips;

    // 下拉菜单项存储
    std::map<std::wstring, std::vector<ZWLookupGripMenuItem>> m_dropdownMenuItems;

    // 上下文菜单项存储
    std::map<std::wstring, std::vector<ZWLookupGripMenuItem>> m_contextMenuItems;

    // 提示文本存储
    std::map<std::wstring, std::wstring> m_tooltips;

    // 回调函数
    GripClickCallback m_clickCallback;
    void* m_clickCallbackUserData;
    GripHoverCallback m_hoverCallback;
    void* m_hoverCallbackUserData;
    GripOptionSelectedCallback m_optionSelectedCallback;
    void* m_optionSelectedCallbackUserData;
    GripMenuItemSelectedCallback m_menuItemSelectedCallback;
    void* m_menuItemSelectedCallbackUserData;

    // 外观设置
    double m_gripSize;
    std::map<ZWLookupGripState, COLORREF> m_gripColors;
    std::wstring m_labelFontName;
    int m_labelFontSize;
    COLORREF m_labelColor;

    // 当前活动的菜单窗口
    HWND m_activeDropdownWindow;
    HWND m_activeContextMenuWindow;
    HWND m_activeTooltipWindow;

    // 当前悬停的夹点
    ZcDbObjectId m_hoveredParameterId;
    int m_hoveredGripIndex;
};

// 夹点绘制辅助类
class ZWLookupGripDrawer
{
public:
    ZWLookupGripDrawer(ZcGiViewportDraw* pVd);
    ~ZWLookupGripDrawer();

    // 绘制方法
    void drawSquareGrip(const ZcGePoint3d& position, double size, COLORREF color);
    void drawCircleGrip(const ZcGePoint3d& position, double radius, COLORREF color);
    void drawTriangleGrip(const ZcGePoint3d& position, double size, COLORREF color);
    void drawDiamondGrip(const ZcGePoint3d& position, double size, COLORREF color);
    void drawDropdownGrip(const ZcGePoint3d& position, double size, COLORREF color);
    void drawText(const ZcGePoint3d& position, const std::wstring& text, COLORREF color);
    void drawLine(const ZcGePoint3d& start, const ZcGePoint3d& end, COLORREF color);
    void drawArrow(const ZcGePoint3d& position, double size, bool pointDown, COLORREF color);

private:
    ZcGiViewportDraw* m_pVd;
};

// 夹点交互处理器
class ZWLookupGripInteractor
{
public:
    ZWLookupGripInteractor();
    ~ZWLookupGripInteractor();

    // 鼠标事件处理
    bool onMouseMove(const ZcGePoint3d& worldPoint, const ZcGePoint3d& screenPoint);
    bool onMouseDown(const ZcGePoint3d& worldPoint, const ZcGePoint3d& screenPoint, int button);
    bool onMouseUp(const ZcGePoint3d& worldPoint, const ZcGePoint3d& screenPoint, int button);
    bool onMouseDoubleClick(const ZcGePoint3d& worldPoint, const ZcGePoint3d& screenPoint, int button);
    bool onMouseWheel(const ZcGePoint3d& worldPoint, int delta);

    // 键盘事件处理
    bool onKeyDown(int keyCode);
    bool onKeyUp(int keyCode);

    // 状态查询
    bool isInteracting() const;
    ZcDbObjectId getActiveParameterId() const;
    int getActiveGripIndex() const;

    // 交互控制
    void beginInteraction(const ZcDbObjectId& parameterId, int gripIndex);
    void endInteraction();
    void cancelInteraction();

private:
    bool m_isInteracting;
    ZcDbObjectId m_activeParameterId;
    int m_activeGripIndex;
    ZcGePoint3d m_startPoint;
    ZcGePoint3d m_currentPoint;
};
