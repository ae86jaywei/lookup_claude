// ZWLookupGripManager.cpp
// 夹点管理器实现

#include "stdafx.h"
#include "ZWLookupGripManager.h"
#include "ZWLookupParameter.h"

// 注册夹点管理器类
ZCRX_CONS_DEFINE_MEMBERS(ZWLookupGripManager, ZcRxObject, 1);

// 静态实例初始化
ZWLookupGripManager* ZWLookupGripManager::s_pInstance = nullptr;

// 构造函数
ZWLookupGripManager::ZWLookupGripManager()
    : m_clickCallback(nullptr)
    , m_clickCallbackUserData(nullptr)
    , m_hoverCallback(nullptr)
    , m_hoverCallbackUserData(nullptr)
    , m_optionSelectedCallback(nullptr)
    , m_optionSelectedCallbackUserData(nullptr)
    , m_menuItemSelectedCallback(nullptr)
    , m_menuItemSelectedCallbackUserData(nullptr)
    , m_gripSize(8.0)
    , m_labelFontName(L"Microsoft YaHei UI")
    , m_labelFontSize(9)
    , m_labelColor(RGB(0, 0, 0))
    , m_activeDropdownWindow(NULL)
    , m_activeContextMenuWindow(NULL)
    , m_activeTooltipWindow(NULL)
    , m_hoveredParameterId(ZcDbObjectId::kNull)
    , m_hoveredGripIndex(-1)
{
    // 初始化默认夹点颜色
    m_gripColors[ZWLookupGripState::Normal] = RGB(0, 0, 255);      // 蓝色
    m_gripColors[ZWLookupGripState::Hovered] = RGB(255, 165, 0);   // 橙色
    m_gripColors[ZWLookupGripState::Selected] = RGB(255, 0, 0);    // 红色
    m_gripColors[ZWLookupGripState::Disabled] = RGB(128, 128, 128); // 灰色
}

// 析构函数
ZWLookupGripManager::~ZWLookupGripManager()
{
    hideAllMenus();
    clearAllGrips();
}

// 单例访问
ZWLookupGripManager* ZWLookupGripManager::getInstance()
{
    if (s_pInstance == nullptr)
    {
        s_pInstance = new ZWLookupGripManager();
    }
    return s_pInstance;
}

void ZWLookupGripManager::destroyInstance()
{
    if (s_pInstance != nullptr)
    {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 生成夹点键值
std::wstring ZWLookupGripManager::makeGripKey(const ZcDbObjectId& parameterId, int gripIndex) const
{
    wchar_t buffer[64];
    swprintf_s(buffer, L"%lld_%d", parameterId.asOldId(), gripIndex);
    return std::wstring(buffer);
}

// 夹点注册和注销
bool ZWLookupGripManager::registerGrip(const ZWLookupGripData& gripData)
{
    std::wstring key = makeGripKey(gripData.parameterId, gripData.gripIndex);
    
    // 检查是否已存在
    if (m_grips.find(key) != m_grips.end())
    {
        return false;
    }
    
    m_grips[key] = gripData;
    return true;
}

bool ZWLookupGripManager::unregisterGrip(const ZcDbObjectId& parameterId, int gripIndex)
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    auto it = m_grips.find(key);
    if (it != m_grips.end())
    {
        m_grips.erase(it);
        m_dropdownMenuItems.erase(key);
        m_contextMenuItems.erase(key);
        m_tooltips.erase(key);
        return true;
    }
    return false;
}

bool ZWLookupGripManager::unregisterAllGrips(const ZcDbObjectId& parameterId)
{
    std::vector<std::wstring> keysToRemove;
    
    for (const auto& pair : m_grips)
    {
        if (pair.second.parameterId == parameterId)
        {
            keysToRemove.push_back(pair.first);
        }
    }
    
    for (const auto& key : keysToRemove)
    {
        m_grips.erase(key);
        m_dropdownMenuItems.erase(key);
        m_contextMenuItems.erase(key);
        m_tooltips.erase(key);
    }
    
    return !keysToRemove.empty();
}

void ZWLookupGripManager::clearAllGrips()
{
    m_grips.clear();
    m_dropdownMenuItems.clear();
    m_contextMenuItems.clear();
    m_tooltips.clear();
}

// 夹点查询
ZWLookupGripData* ZWLookupGripManager::getGripData(const ZcDbObjectId& parameterId, int gripIndex)
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    auto it = m_grips.find(key);
    if (it != m_grips.end())
    {
        return &(it->second);
    }
    return nullptr;
}

std::vector<ZWLookupGripData*> ZWLookupGripManager::getGripsForParameter(const ZcDbObjectId& parameterId)
{
    std::vector<ZWLookupGripData*> result;
    
    for (auto& pair : m_grips)
    {
        if (pair.second.parameterId == parameterId)
        {
            result.push_back(&(pair.second));
        }
    }
    
    return result;
}

std::vector<ZWLookupGripData*> ZWLookupGripManager::getAllGrips()
{
    std::vector<ZWLookupGripData*> result;
    
    for (auto& pair : m_grips)
    {
        result.push_back(&(pair.second));
    }
    
    return result;
}

int ZWLookupGripManager::getGripCount() const
{
    return static_cast<int>(m_grips.size());
}

int ZWLookupGripManager::getGripCountForParameter(const ZcDbObjectId& parameterId) const
{
    int count = 0;
    for (const auto& pair : m_grips)
    {
        if (pair.second.parameterId == parameterId)
        {
            count++;
        }
    }
    return count;
}

// 夹点状态管理
bool ZWLookupGripManager::setGripState(const ZcDbObjectId& parameterId, int gripIndex, ZWLookupGripState state)
{
    ZWLookupGripData* gripData = getGripData(parameterId, gripIndex);
    if (gripData)
    {
        gripData->state = state;
        return true;
    }
    return false;
}

ZWLookupGripState ZWLookupGripManager::getGripState(const ZcDbObjectId& parameterId, int gripIndex) const
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    auto it = m_grips.find(key);
    if (it != m_grips.end())
    {
        return it->second.state;
    }
    return ZWLookupGripState::Normal;
}

bool ZWLookupGripManager::setGripPosition(const ZcDbObjectId& parameterId, int gripIndex, const ZcGePoint3d& position)
{
    ZWLookupGripData* gripData = getGripData(parameterId, gripIndex);
    if (gripData)
    {
        gripData->position = position;
        return true;
    }
    return false;
}

ZcGePoint3d ZWLookupGripManager::getGripPosition(const ZcDbObjectId& parameterId, int gripIndex) const
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    auto it = m_grips.find(key);
    if (it != m_grips.end())
    {
        return it->second.position;
    }
    return ZcGePoint3d::kOrigin;
}

bool ZWLookupGripManager::setGripVisible(const ZcDbObjectId& parameterId, int gripIndex, bool visible)
{
    ZWLookupGripData* gripData = getGripData(parameterId, gripIndex);
    if (gripData)
    {
        gripData->isVisible = visible;
        return true;
    }
    return false;
}

bool ZWLookupGripManager::isGripVisible(const ZcDbObjectId& parameterId, int gripIndex) const
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    auto it = m_grips.find(key);
    if (it != m_grips.end())
    {
        return it->second.isVisible;
    }
    return false;
}

bool ZWLookupGripManager::setGripEnabled(const ZcDbObjectId& parameterId, int gripIndex, bool enabled)
{
    ZWLookupGripData* gripData = getGripData(parameterId, gripIndex);
    if (gripData)
    {
        gripData->isEnabled = enabled;
        return true;
    }
    return false;
}

bool ZWLookupGripManager::isGripEnabled(const ZcDbObjectId& parameterId, int gripIndex) const
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    auto it = m_grips.find(key);
    if (it != m_grips.end())
    {
        return it->second.isEnabled;
    }
    return false;
}

// 夹点选项管理
bool ZWLookupGripManager::setGripOptions(const ZcDbObjectId& parameterId, int gripIndex, const std::vector<std::wstring>& options)
{
    ZWLookupGripData* gripData = getGripData(parameterId, gripIndex);
    if (gripData)
    {
        gripData->options = options;
        if (gripData->selectedOptionIndex >= static_cast<int>(options.size()))
        {
            gripData->selectedOptionIndex = options.empty() ? -1 : 0;
        }
        return true;
    }
    return false;
}

std::vector<std::wstring> ZWLookupGripManager::getGripOptions(const ZcDbObjectId& parameterId, int gripIndex) const
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    auto it = m_grips.find(key);
    if (it != m_grips.end())
    {
        return it->second.options;
    }
    return std::vector<std::wstring>();
}

bool ZWLookupGripManager::setSelectedOption(const ZcDbObjectId& parameterId, int gripIndex, int optionIndex)
{
    ZWLookupGripData* gripData = getGripData(parameterId, gripIndex);
    if (gripData && optionIndex >= 0 && optionIndex < static_cast<int>(gripData->options.size()))
    {
        gripData->selectedOptionIndex = optionIndex;
        return true;
    }
    return false;
}

int ZWLookupGripManager::getSelectedOption(const ZcDbObjectId& parameterId, int gripIndex) const
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    auto it = m_grips.find(key);
    if (it != m_grips.end())
    {
        return it->second.selectedOptionIndex;
    }
    return -1;
}

std::wstring ZWLookupGripManager::getSelectedOptionText(const ZcDbObjectId& parameterId, int gripIndex) const
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    auto it = m_grips.find(key);
    if (it != m_grips.end())
    {
        int selectedIndex = it->second.selectedOptionIndex;
        if (selectedIndex >= 0 && selectedIndex < static_cast<int>(it->second.options.size()))
        {
            return it->second.options[selectedIndex];
        }
    }
    return L"";
}

// 夹点标签管理
bool ZWLookupGripManager::setGripLabel(const ZcDbObjectId& parameterId, int gripIndex, const std::wstring& label)
{
    ZWLookupGripData* gripData = getGripData(parameterId, gripIndex);
    if (gripData)
    {
        gripData->label = label;
        return true;
    }
    return false;
}

std::wstring ZWLookupGripManager::getGripLabel(const ZcDbObjectId& parameterId, int gripIndex) const
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    auto it = m_grips.find(key);
    if (it != m_grips.end())
    {
        return it->second.label;
    }
    return L"";
}

// 夹点命中测试
bool ZWLookupGripManager::hitTest(const ZcGePoint3d& pickPoint, double tolerance, ZcDbObjectId& outParameterId, int& outGripIndex)
{
    for (const auto& pair : m_grips)
    {
        const ZWLookupGripData& gripData = pair.second;
        if (!gripData.isVisible || !gripData.isEnabled)
        {
            continue;
        }
        
        double distance = pickPoint.distanceTo(gripData.position);
        if (distance <= tolerance)
        {
            outParameterId = gripData.parameterId;
            outGripIndex = gripData.gripIndex;
            return true;
        }
    }
    
    outParameterId = ZcDbObjectId::kNull;
    outGripIndex = -1;
    return false;
}

std::vector<std::pair<ZcDbObjectId, int>> ZWLookupGripManager::hitTestAll(const ZcGePoint3d& pickPoint, double tolerance)
{
    std::vector<std::pair<ZcDbObjectId, int>> result;
    
    for (const auto& pair : m_grips)
    {
        const ZWLookupGripData& gripData = pair.second;
        if (!gripData.isVisible || !gripData.isEnabled)
        {
            continue;
        }
        
        double distance = pickPoint.distanceTo(gripData.position);
        if (distance <= tolerance)
        {
            result.push_back(std::make_pair(gripData.parameterId, gripData.gripIndex));
        }
    }
    
    return result;
}

// 夹点绘制
void ZWLookupGripManager::drawGrip(ZcGiViewportDraw* pVd, const ZWLookupGripData& gripData)
{
    if (!gripData.isVisible)
    {
        return;
    }
    
    // 绘制夹点符号
    drawGripSymbol(pVd, gripData.position, gripData.state);
    
    // 绘制标签
    if (!gripData.label.empty())
    {
        drawGripLabel(pVd, gripData.position, gripData.label);
    }
    
    // 如果有选项，绘制下拉箭头
    if (!gripData.options.empty())
    {
        drawDropdownArrow(pVd, gripData.position);
    }
}

void ZWLookupGripManager::drawAllGrips(ZcGiViewportDraw* pVd)
{
    for (const auto& pair : m_grips)
    {
        drawGrip(pVd, pair.second);
    }
}

void ZWLookupGripManager::drawGripsForParameter(ZcGiViewportDraw* pVd, const ZcDbObjectId& parameterId)
{
    for (const auto& pair : m_grips)
    {
        if (pair.second.parameterId == parameterId)
        {
            drawGrip(pVd, pair.second);
        }
    }
}

void ZWLookupGripManager::drawGripSymbol(ZcGiViewportDraw* pVd, const ZcGePoint3d& position, ZWLookupGripState state)
{
    if (!pVd)
    {
        return;
    }
    
    // 获取夹点颜色
    COLORREF color = m_gripColors[state];
    
    // 设置颜色
    ZcCmColor cmColor;
    cmColor.setRGB(GetRValue(color), GetGValue(color), GetBValue(color));
    pVd->subEntityTraits().setColor(cmColor.colorIndex());
    
    // 计算夹点大小（屏幕像素转换为世界坐标）
    double halfSize = m_gripSize / 2.0;
    
    // 绘制正方形夹点
    ZcGePoint3d pts[5];
    pts[0] = ZcGePoint3d(position.x - halfSize, position.y - halfSize, position.z);
    pts[1] = ZcGePoint3d(position.x + halfSize, position.y - halfSize, position.z);
    pts[2] = ZcGePoint3d(position.x + halfSize, position.y + halfSize, position.z);
    pts[3] = ZcGePoint3d(position.x - halfSize, position.y + halfSize, position.z);
    pts[4] = pts[0];
    
    pVd->geometry().polyline(5, pts);
}

void ZWLookupGripManager::drawGripLabel(ZcGiViewportDraw* pVd, const ZcGePoint3d& position, const std::wstring& label)
{
    if (!pVd || label.empty())
    {
        return;
    }
    
    // 设置文本颜色
    ZcCmColor cmColor;
    cmColor.setRGB(GetRValue(m_labelColor), GetGValue(m_labelColor), GetBValue(m_labelColor));
    pVd->subEntityTraits().setColor(cmColor.colorIndex());
    
    // 计算文本位置（在夹点上方）
    ZcGePoint3d textPos(position.x, position.y + m_gripSize + 2.0, position.z);
    
    // 绘制文本
    pVd->geometry().text(textPos, ZcGeVector3d::kZAxis, ZcGeVector3d::kXAxis, 
                         m_labelFontSize, 1.0, 0.0, label.c_str());
}

void ZWLookupGripManager::drawDropdownArrow(ZcGiViewportDraw* pVd, const ZcGePoint3d& position)
{
    if (!pVd)
    {
        return;
    }
    
    // 设置箭头颜色
    ZcCmColor cmColor;
    cmColor.setRGB(0, 0, 0);
    pVd->subEntityTraits().setColor(cmColor.colorIndex());
    
    // 计算箭头位置（在夹点右侧）
    double arrowSize = m_gripSize / 3.0;
    ZcGePoint3d arrowPos(position.x + m_gripSize + 2.0, position.y, position.z);
    
    // 绘制向下的三角形箭头
    ZcGePoint3d pts[4];
    pts[0] = ZcGePoint3d(arrowPos.x - arrowSize, arrowPos.y + arrowSize / 2.0, arrowPos.z);
    pts[1] = ZcGePoint3d(arrowPos.x + arrowSize, arrowPos.y + arrowSize / 2.0, arrowPos.z);
    pts[2] = ZcGePoint3d(arrowPos.x, arrowPos.y - arrowSize / 2.0, arrowPos.z);
    pts[3] = pts[0];
    
    pVd->geometry().polyline(4, pts);
}

// 夹点交互
bool ZWLookupGripManager::showDropdownMenu(const ZcDbObjectId& parameterId, int gripIndex, const ZcGePoint3d& screenPoint)
{
    ZWLookupGripData* gripData = getGripData(parameterId, gripIndex);
    if (!gripData || gripData->options.empty())
    {
        return false;
    }
    
    // 隐藏现有菜单
    hideAllMenus();
    
    // 创建下拉菜单窗口
    m_activeDropdownWindow = createDropdownWindow(screenPoint, gripData->options);
    
    return m_activeDropdownWindow != NULL;
}

bool ZWLookupGripManager::showContextMenu(const ZcDbObjectId& parameterId, int gripIndex, const ZcGePoint3d& screenPoint)
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    auto it = m_contextMenuItems.find(key);
    if (it == m_contextMenuItems.end() || it->second.empty())
    {
        return false;
    }
    
    // 隐藏现有菜单
    hideAllMenus();
    
    // 创建上下文菜单窗口
    m_activeContextMenuWindow = createContextMenuWindow(screenPoint, it->second);
    
    return m_activeContextMenuWindow != NULL;
}

void ZWLookupGripManager::hideAllMenus()
{
    if (m_activeDropdownWindow != NULL)
    {
        DestroyWindow(m_activeDropdownWindow);
        m_activeDropdownWindow = NULL;
    }
    
    if (m_activeContextMenuWindow != NULL)
    {
        DestroyWindow(m_activeContextMenuWindow);
        m_activeContextMenuWindow = NULL;
    }
    
    if (m_activeTooltipWindow != NULL)
    {
        DestroyWindow(m_activeTooltipWindow);
        m_activeTooltipWindow = NULL;
    }
}

HWND ZWLookupGripManager::createDropdownWindow(const ZcGePoint3d& screenPoint, const std::vector<std::wstring>& options)
{
    // 创建弹出菜单
    HMENU hMenu = CreatePopupMenu();
    if (!hMenu)
    {
        return NULL;
    }
    
    // 添加菜单项
    for (size_t i = 0; i < options.size(); i++)
    {
        AppendMenuW(hMenu, MF_STRING, i + 1, options[i].c_str());
    }
    
    // 显示菜单
    CWnd* pMainWnd = AfxGetMainWnd();
    HWND hWnd = pMainWnd ? pMainWnd->GetSafeHwnd() : NULL;
    int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_TOPALIGN,
                            static_cast<int>(screenPoint.x), static_cast<int>(screenPoint.y),
                            0, hWnd, NULL);
    
    DestroyMenu(hMenu);
    
    // 处理选择
    if (cmd > 0)
    {
        // 触发选项选择回调
        // 这里需要知道当前的参数ID和夹点索引
        // 暂时返回NULL，实际实现需要更复杂的逻辑
    }
    
    return NULL;
}

HWND ZWLookupGripManager::createContextMenuWindow(const ZcGePoint3d& screenPoint, const std::vector<ZWLookupGripMenuItem>& items)
{
    // 创建弹出菜单
    HMENU hMenu = CreatePopupMenu();
    if (!hMenu)
    {
        return NULL;
    }
    
    // 添加菜单项
    for (const auto& item : items)
    {
        if (item.isSeparator)
        {
            AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
        }
        else
        {
            UINT flags = MF_STRING;
            if (!item.isEnabled)
            {
                flags |= MF_GRAYED;
            }
            if (item.isChecked)
            {
                flags |= MF_CHECKED;
            }
            AppendMenuW(hMenu, flags, item.id, item.text.c_str());
        }
    }
    
    // 显示菜单
    CWnd* pMainWnd = AfxGetMainWnd();
    HWND hWnd = pMainWnd ? pMainWnd->GetSafeHwnd() : NULL;
    int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_LEFTALIGN | TPM_TOPALIGN,
                            static_cast<int>(screenPoint.x), static_cast<int>(screenPoint.y),
                            0, hWnd, NULL);
    
    DestroyMenu(hMenu);
    
    return NULL;
}

// 下拉菜单管理
void ZWLookupGripManager::setDropdownMenuItems(const ZcDbObjectId& parameterId, int gripIndex, const std::vector<ZWLookupGripMenuItem>& items)
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    m_dropdownMenuItems[key] = items;
}

std::vector<ZWLookupGripMenuItem> ZWLookupGripManager::getDropdownMenuItems(const ZcDbObjectId& parameterId, int gripIndex) const
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    auto it = m_dropdownMenuItems.find(key);
    if (it != m_dropdownMenuItems.end())
    {
        return it->second;
    }
    return std::vector<ZWLookupGripMenuItem>();
}

// 上下文菜单管理
void ZWLookupGripManager::setContextMenuItems(const ZcDbObjectId& parameterId, int gripIndex, const std::vector<ZWLookupGripMenuItem>& items)
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    m_contextMenuItems[key] = items;
}

std::vector<ZWLookupGripMenuItem> ZWLookupGripManager::getContextMenuItems(const ZcDbObjectId& parameterId, int gripIndex) const
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    auto it = m_contextMenuItems.find(key);
    if (it != m_contextMenuItems.end())
    {
        return it->second;
    }
    return std::vector<ZWLookupGripMenuItem>();
}

// 夹点事件回调设置
void ZWLookupGripManager::setGripClickCallback(GripClickCallback callback, void* userData)
{
    m_clickCallback = callback;
    m_clickCallbackUserData = userData;
}

void ZWLookupGripManager::setGripHoverCallback(GripHoverCallback callback, void* userData)
{
    m_hoverCallback = callback;
    m_hoverCallbackUserData = userData;
}

void ZWLookupGripManager::setGripOptionSelectedCallback(GripOptionSelectedCallback callback, void* userData)
{
    m_optionSelectedCallback = callback;
    m_optionSelectedCallbackUserData = userData;
}

void ZWLookupGripManager::setGripMenuItemSelectedCallback(GripMenuItemSelectedCallback callback, void* userData)
{
    m_menuItemSelectedCallback = callback;
    m_menuItemSelectedCallbackUserData = userData;
}

// 夹点事件触发
void ZWLookupGripManager::onGripClicked(const ZcDbObjectId& parameterId, int gripIndex)
{
    if (m_clickCallback)
    {
        m_clickCallback(parameterId, gripIndex, m_clickCallbackUserData);
    }
}

void ZWLookupGripManager::onGripHovered(const ZcDbObjectId& parameterId, int gripIndex)
{
    // 更新悬停状态
    if (m_hoveredParameterId != parameterId || m_hoveredGripIndex != gripIndex)
    {
        // 恢复之前悬停夹点的状态
        if (!m_hoveredParameterId.isNull() && m_hoveredGripIndex >= 0)
        {
            setGripState(m_hoveredParameterId, m_hoveredGripIndex, ZWLookupGripState::Normal);
        }
        
        // 设置新悬停夹点的状态
        m_hoveredParameterId = parameterId;
        m_hoveredGripIndex = gripIndex;
        
        if (!parameterId.isNull() && gripIndex >= 0)
        {
            setGripState(parameterId, gripIndex, ZWLookupGripState::Hovered);
        }
    }
    
    if (m_hoverCallback)
    {
        m_hoverCallback(parameterId, gripIndex, m_hoverCallbackUserData);
    }
}

void ZWLookupGripManager::onGripOptionSelected(const ZcDbObjectId& parameterId, int gripIndex, int optionIndex)
{
    // 更新选中的选项
    setSelectedOption(parameterId, gripIndex, optionIndex);
    
    if (m_optionSelectedCallback)
    {
        m_optionSelectedCallback(parameterId, gripIndex, optionIndex, m_optionSelectedCallbackUserData);
    }
}

void ZWLookupGripManager::onGripMenuItemSelected(const ZcDbObjectId& parameterId, int gripIndex, int menuItemId)
{
    if (m_menuItemSelectedCallback)
    {
        m_menuItemSelectedCallback(parameterId, gripIndex, menuItemId, m_menuItemSelectedCallbackUserData);
    }
}

// 夹点外观设置
void ZWLookupGripManager::setGripSize(double size)
{
    m_gripSize = size;
}

double ZWLookupGripManager::getGripSize() const
{
    return m_gripSize;
}

void ZWLookupGripManager::setGripColor(ZWLookupGripState state, COLORREF color)
{
    m_gripColors[state] = color;
}

COLORREF ZWLookupGripManager::getGripColor(ZWLookupGripState state) const
{
    auto it = m_gripColors.find(state);
    if (it != m_gripColors.end())
    {
        return it->second;
    }
    return RGB(0, 0, 255);
}

void ZWLookupGripManager::setLabelFont(const std::wstring& fontName, int fontSize)
{
    m_labelFontName = fontName;
    m_labelFontSize = fontSize;
}

void ZWLookupGripManager::setLabelColor(COLORREF color)
{
    m_labelColor = color;
}

COLORREF ZWLookupGripManager::getLabelColor() const
{
    return m_labelColor;
}

// 夹点提示文本
void ZWLookupGripManager::setGripTooltip(const ZcDbObjectId& parameterId, int gripIndex, const std::wstring& tooltip)
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    m_tooltips[key] = tooltip;
}

std::wstring ZWLookupGripManager::getGripTooltip(const ZcDbObjectId& parameterId, int gripIndex) const
{
    std::wstring key = makeGripKey(parameterId, gripIndex);
    auto it = m_tooltips.find(key);
    if (it != m_tooltips.end())
    {
        return it->second;
    }
    return L"";
}

void ZWLookupGripManager::showTooltip(const ZcDbObjectId& parameterId, int gripIndex, const ZcGePoint3d& screenPoint)
{
    // 隐藏现有提示
    hideTooltip();
    
    std::wstring tooltip = getGripTooltip(parameterId, gripIndex);
    if (tooltip.empty())
    {
        return;
    }
    
    // 创建提示窗口
    // 这里使用简单的实现，实际可以使用更复杂的提示窗口
    CWnd* pMainWnd = AfxGetMainWnd();
    HWND hWnd = pMainWnd ? pMainWnd->GetSafeHwnd() : NULL;
    
    // 创建工具提示控件
    m_activeTooltipWindow = CreateWindowExW(
        WS_EX_TOPMOST,
        TOOLTIPS_CLASSW,
        NULL,
        WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        hWnd,
        NULL,
        NULL,
        NULL
    );
    
    if (m_activeTooltipWindow)
    {
        TOOLINFOW ti = { 0 };
        ti.cbSize = sizeof(TOOLINFOW);
        ti.uFlags = TTF_SUBCLASS;
        ti.hwnd = hWnd;
        ti.hinst = NULL;
        ti.lpszText = const_cast<LPWSTR>(tooltip.c_str());
        ti.rect.left = static_cast<LONG>(screenPoint.x) - 10;
        ti.rect.top = static_cast<LONG>(screenPoint.y) - 10;
        ti.rect.right = static_cast<LONG>(screenPoint.x) + 10;
        ti.rect.bottom = static_cast<LONG>(screenPoint.y) + 10;
        
        SendMessageW(m_activeTooltipWindow, TTM_ADDTOOLW, 0, (LPARAM)&ti);
        SendMessageW(m_activeTooltipWindow, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
        SendMessageW(m_activeTooltipWindow, TTM_TRACKPOSITION, 0, 
                    MAKELPARAM(static_cast<int>(screenPoint.x), static_cast<int>(screenPoint.y)));
    }
}

void ZWLookupGripManager::hideTooltip()
{
    if (m_activeTooltipWindow != NULL)
    {
        DestroyWindow(m_activeTooltipWindow);
        m_activeTooltipWindow = NULL;
    }
}

// 刷新和更新
void ZWLookupGripManager::refresh()
{
    // 请求重绘所有视图
    zcedUpdateDisplay();
}

void ZWLookupGripManager::refreshGrip(const ZcDbObjectId& parameterId, int gripIndex)
{
    // 请求重绘特定夹点区域
    ZWLookupGripData* gripData = getGripData(parameterId, gripIndex);
    if (gripData)
    {
        // 计算夹点边界框并请求重绘
        zcedUpdateDisplay();
    }
}

void ZWLookupGripManager::refreshGripsForParameter(const ZcDbObjectId& parameterId)
{
    // 请求重绘参数的所有夹点
    zcedUpdateDisplay();
}

// ============================================================================
// ZWLookupGripDrawer 实现
// ============================================================================

ZWLookupGripDrawer::ZWLookupGripDrawer(ZcGiViewportDraw* pVd)
    : m_pVd(pVd)
{
}

ZWLookupGripDrawer::~ZWLookupGripDrawer()
{
}

void ZWLookupGripDrawer::drawSquareGrip(const ZcGePoint3d& position, double size, COLORREF color)
{
    if (!m_pVd)
    {
        return;
    }
    
    ZcCmColor cmColor;
    cmColor.setRGB(GetRValue(color), GetGValue(color), GetBValue(color));
    m_pVd->subEntityTraits().setColor(cmColor.colorIndex());
    
    double halfSize = size / 2.0;
    ZcGePoint3d pts[5];
    pts[0] = ZcGePoint3d(position.x - halfSize, position.y - halfSize, position.z);
    pts[1] = ZcGePoint3d(position.x + halfSize, position.y - halfSize, position.z);
    pts[2] = ZcGePoint3d(position.x + halfSize, position.y + halfSize, position.z);
    pts[3] = ZcGePoint3d(position.x - halfSize, position.y + halfSize, position.z);
    pts[4] = pts[0];
    
    m_pVd->geometry().polyline(5, pts);
}

void ZWLookupGripDrawer::drawCircleGrip(const ZcGePoint3d& position, double radius, COLORREF color)
{
    if (!m_pVd)
    {
        return;
    }
    
    ZcCmColor cmColor;
    cmColor.setRGB(GetRValue(color), GetGValue(color), GetBValue(color));
    m_pVd->subEntityTraits().setColor(cmColor.colorIndex());
    
    // 绘制圆形（使用多边形近似）
    const int segments = 16;
    ZcGePoint3d pts[segments + 1];
    for (int i = 0; i <= segments; i++)
    {
        double angle = 2.0 * 3.14159265358979323846 * i / segments;
        pts[i] = ZcGePoint3d(
            position.x + radius * cos(angle),
            position.y + radius * sin(angle),
            position.z
        );
    }
    
    m_pVd->geometry().polyline(segments + 1, pts);
}

void ZWLookupGripDrawer::drawTriangleGrip(const ZcGePoint3d& position, double size, COLORREF color)
{
    if (!m_pVd)
    {
        return;
    }
    
    ZcCmColor cmColor;
    cmColor.setRGB(GetRValue(color), GetGValue(color), GetBValue(color));
    m_pVd->subEntityTraits().setColor(cmColor.colorIndex());
    
    double halfSize = size / 2.0;
    ZcGePoint3d pts[4];
    pts[0] = ZcGePoint3d(position.x, position.y + halfSize, position.z);
    pts[1] = ZcGePoint3d(position.x - halfSize, position.y - halfSize, position.z);
    pts[2] = ZcGePoint3d(position.x + halfSize, position.y - halfSize, position.z);
    pts[3] = pts[0];
    
    m_pVd->geometry().polyline(4, pts);
}

void ZWLookupGripDrawer::drawDiamondGrip(const ZcGePoint3d& position, double size, COLORREF color)
{
    if (!m_pVd)
    {
        return;
    }
    
    ZcCmColor cmColor;
    cmColor.setRGB(GetRValue(color), GetGValue(color), GetBValue(color));
    m_pVd->subEntityTraits().setColor(cmColor.colorIndex());
    
    double halfSize = size / 2.0;
    ZcGePoint3d pts[5];
    pts[0] = ZcGePoint3d(position.x, position.y + halfSize, position.z);
    pts[1] = ZcGePoint3d(position.x + halfSize, position.y, position.z);
    pts[2] = ZcGePoint3d(position.x, position.y - halfSize, position.z);
    pts[3] = ZcGePoint3d(position.x - halfSize, position.y, position.z);
    pts[4] = pts[0];
    
    m_pVd->geometry().polyline(5, pts);
}

void ZWLookupGripDrawer::drawDropdownGrip(const ZcGePoint3d& position, double size, COLORREF color)
{
    if (!m_pVd)
    {
        return;
    }
    
    // 绘制正方形
    drawSquareGrip(position, size, color);
    
    // 绘制下拉箭头
    ZcCmColor cmColor;
    cmColor.setRGB(GetRValue(color), GetGValue(color), GetBValue(color));
    m_pVd->subEntityTraits().setColor(cmColor.colorIndex());
    
    double arrowSize = size / 3.0;
    ZcGePoint3d arrowPos(position.x + size + 2.0, position.y, position.z);
    
    ZcGePoint3d pts[4];
    pts[0] = ZcGePoint3d(arrowPos.x - arrowSize, arrowPos.y + arrowSize / 2.0, arrowPos.z);
    pts[1] = ZcGePoint3d(arrowPos.x + arrowSize, arrowPos.y + arrowSize / 2.0, arrowPos.z);
    pts[2] = ZcGePoint3d(arrowPos.x, arrowPos.y - arrowSize / 2.0, arrowPos.z);
    pts[3] = pts[0];
    
    m_pVd->geometry().polyline(4, pts);
}

void ZWLookupGripDrawer::drawText(const ZcGePoint3d& position, const std::wstring& text, COLORREF color)
{
    if (!m_pVd || text.empty())
    {
        return;
    }
    
    ZcCmColor cmColor;
    cmColor.setRGB(GetRValue(color), GetGValue(color), GetBValue(color));
    m_pVd->subEntityTraits().setColor(cmColor.colorIndex());
    
    m_pVd->geometry().text(position, ZcGeVector3d::kZAxis, ZcGeVector3d::kXAxis, 
                          2.5, 1.0, 0.0, text.c_str());
}

void ZWLookupGripDrawer::drawLine(const ZcGePoint3d& start, const ZcGePoint3d& end, COLORREF color)
{
    if (!m_pVd)
    {
        return;
    }
    
    ZcCmColor cmColor;
    cmColor.setRGB(GetRValue(color), GetGValue(color), GetBValue(color));
    m_pVd->subEntityTraits().setColor(cmColor.colorIndex());
    
    ZcGePoint3d pts[2] = { start, end };
    m_pVd->geometry().polyline(2, pts);
}

void ZWLookupGripDrawer::drawArrow(const ZcGePoint3d& position, double size, bool pointDown, COLORREF color)
{
    if (!m_pVd)
    {
        return;
    }
    
    ZcCmColor cmColor;
    cmColor.setRGB(GetRValue(color), GetGValue(color), GetBValue(color));
    m_pVd->subEntityTraits().setColor(cmColor.colorIndex());
    
    double halfSize = size / 2.0;
    ZcGePoint3d pts[4];
    
    if (pointDown)
    {
        pts[0] = ZcGePoint3d(position.x - halfSize, position.y + halfSize / 2.0, position.z);
        pts[1] = ZcGePoint3d(position.x + halfSize, position.y + halfSize / 2.0, position.z);
        pts[2] = ZcGePoint3d(position.x, position.y - halfSize / 2.0, position.z);
    }
    else
    {
        pts[0] = ZcGePoint3d(position.x - halfSize, position.y - halfSize / 2.0, position.z);
        pts[1] = ZcGePoint3d(position.x + halfSize, position.y - halfSize / 2.0, position.z);
        pts[2] = ZcGePoint3d(position.x, position.y + halfSize / 2.0, position.z);
    }
    pts[3] = pts[0];
    
    m_pVd->geometry().polyline(4, pts);
}

// ============================================================================
// ZWLookupGripInteractor 实现
// ============================================================================

ZWLookupGripInteractor::ZWLookupGripInteractor()
    : m_isInteracting(false)
    , m_activeParameterId(ZcDbObjectId::kNull)
    , m_activeGripIndex(-1)
    , m_startPoint(ZcGePoint3d::kOrigin)
    , m_currentPoint(ZcGePoint3d::kOrigin)
{
}

ZWLookupGripInteractor::~ZWLookupGripInteractor()
{
}

bool ZWLookupGripInteractor::onMouseMove(const ZcGePoint3d& worldPoint, const ZcGePoint3d& screenPoint)
{
    ZWLookupGripManager* manager = ZWLookupGripManager::getInstance();
    
    // 进行命中测试
    ZcDbObjectId parameterId;
    int gripIndex;
    
    if (manager->hitTest(worldPoint, manager->getGripSize(), parameterId, gripIndex))
    {
        // 触发悬停事件
        manager->onGripHovered(parameterId, gripIndex);
        return true;
    }
    else
    {
        // 清除悬停状态
        manager->onGripHovered(ZcDbObjectId::kNull, -1);
    }
    
    if (m_isInteracting)
    {
        m_currentPoint = worldPoint;
        return true;
    }
    
    return false;
}

bool ZWLookupGripInteractor::onMouseDown(const ZcGePoint3d& worldPoint, const ZcGePoint3d& screenPoint, int button)
{
    ZWLookupGripManager* manager = ZWLookupGripManager::getInstance();
    
    // 进行命中测试
    ZcDbObjectId parameterId;
    int gripIndex;
    
    if (manager->hitTest(worldPoint, manager->getGripSize(), parameterId, gripIndex))
    {
        if (button == 0) // 左键
        {
            // 检查是否有下拉选项
            std::vector<std::wstring> options = manager->getGripOptions(parameterId, gripIndex);
            if (!options.empty())
            {
                // 显示下拉菜单
                manager->showDropdownMenu(parameterId, gripIndex, screenPoint);
            }
            else
            {
                // 触发点击事件
                manager->onGripClicked(parameterId, gripIndex);
            }
        }
        else if (button == 1) // 右键
        {
            // 显示上下文菜单
            manager->showContextMenu(parameterId, gripIndex, screenPoint);
        }
        
        return true;
    }
    
    return false;
}

bool ZWLookupGripInteractor::onMouseUp(const ZcGePoint3d& worldPoint, const ZcGePoint3d& screenPoint, int button)
{
    if (m_isInteracting)
    {
        endInteraction();
        return true;
    }
    return false;
}

bool ZWLookupGripInteractor::onMouseDoubleClick(const ZcGePoint3d& worldPoint, const ZcGePoint3d& screenPoint, int button)
{
    // 双击处理
    return false;
}

bool ZWLookupGripInteractor::onMouseWheel(const ZcGePoint3d& worldPoint, int delta)
{
    // 滚轮处理
    return false;
}

bool ZWLookupGripInteractor::onKeyDown(int keyCode)
{
    if (m_isInteracting && keyCode == VK_ESCAPE)
    {
        cancelInteraction();
        return true;
    }
    return false;
}

bool ZWLookupGripInteractor::onKeyUp(int keyCode)
{
    return false;
}

bool ZWLookupGripInteractor::isInteracting() const
{
    return m_isInteracting;
}

ZcDbObjectId ZWLookupGripInteractor::getActiveParameterId() const
{
    return m_activeParameterId;
}

int ZWLookupGripInteractor::getActiveGripIndex() const
{
    return m_activeGripIndex;
}

void ZWLookupGripInteractor::beginInteraction(const ZcDbObjectId& parameterId, int gripIndex)
{
    m_isInteracting = true;
    m_activeParameterId = parameterId;
    m_activeGripIndex = gripIndex;
    
    ZWLookupGripManager* manager = ZWLookupGripManager::getInstance();
    manager->setGripState(parameterId, gripIndex, ZWLookupGripState::Selected);
}

void ZWLookupGripInteractor::endInteraction()
{
    if (m_isInteracting)
    {
        ZWLookupGripManager* manager = ZWLookupGripManager::getInstance();
        manager->setGripState(m_activeParameterId, m_activeGripIndex, ZWLookupGripState::Normal);
        
        m_isInteracting = false;
        m_activeParameterId = ZcDbObjectId::kNull;
        m_activeGripIndex = -1;
    }
}

void ZWLookupGripInteractor::cancelInteraction()
{
    endInteraction();
}
