// ZWLookupDialog.cpp
// 对话框实现

#include "stdafx.h"
#include "ZWLookupDialog.h"

// 特性查寻表对话框实现

IMPLEMENT_DYNAMIC(CZWLookupTableDialog, CDialogEx)

// 构造函数
CZWLookupTableDialog::CZWLookupTableDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_LOOKUP_TABLE_DIALOG, pParent)
    , m_isInitialized(false)
{}

// 析构函数
CZWLookupTableDialog::~CZWLookupTableDialog()
{}

// 对话框数据交换
void CZWLookupTableDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ACTION_NAME, m_editActionName);
    DDX_Control(pDX, IDC_INPUT_LIST, m_listInputProperties);
    DDX_Control(pDX, IDC_QUERY_LIST, m_listQueryProperties);
    DDX_Control(pDX, IDC_QUERY_VALUE, m_editQueryValue);
}

BEGIN_MESSAGE_MAP(CZWLookupTableDialog, CDialogEx)
    ON_BN_CLICKED(IDC_ADD_FEATURE, &CZWLookupTableDialog::OnBnClickedAddFeature)
    ON_BN_CLICKED(IDC_CHECK, &CZWLookupTableDialog::OnBnClickedCheck)
    ON_BN_CLICKED(IDC_HELP_BUTTON, &CZWLookupTableDialog::OnBnClickedHelpButton)
    ON_BN_CLICKED(IDOK, &CZWLookupTableDialog::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CZWLookupTableDialog::OnBnClickedCancel)
    ON_EN_CHANGE(IDC_ACTION_NAME, &CZWLookupTableDialog::OnEnChangeActionName)
    ON_BN_CLICKED(IDC_GROUP_QUERY, &CZWLookupTableDialog::OnBnClickedGroupQuery)
END_MESSAGE_MAP()

// 对话框初始化
BOOL CZWLookupTableDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    initializeControls();
    m_isInitialized = true;
    return TRUE;
}

// 初始化控件
void CZWLookupTableDialog::initializeControls()
{
    // 设置动作名称
    m_editActionName.SetWindowText(m_actionName.c_str());
    
    // 初始化输入特性列表
    m_listInputProperties.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    m_listInputProperties.InsertColumn(0, L"特性名称", LVCFMT_LEFT, 150);
    m_listInputProperties.InsertColumn(1, L"值", LVCFMT_LEFT, 100);
    
    // 初始化查寻特性列表
    m_listQueryProperties.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    m_listQueryProperties.InsertColumn(0, L"特性名称", LVCFMT_LEFT, 150);
    m_listQueryProperties.InsertColumn(1, L"值", LVCFMT_LEFT, 100);
    
    // 初始化查寻值编辑框
    m_editQueryValue.SetWindowText(L"");
    m_editQueryValue.SetReadOnly(TRUE);
}

// 控件事件处理
void CZWLookupTableDialog::OnBnClickedAddFeature()
{
    // 添加特性
    AfxMessageBox(L"添加特性功能暂未实现。");
}

void CZWLookupTableDialog::OnBnClickedCheck()
{
    // 核查
    AfxMessageBox(L"核查功能暂未实现。");
}

void CZWLookupTableDialog::OnBnClickedHelpButton()
{
    // 帮助
    AfxMessageBox(L"帮助功能暂未实现。");
}

void CZWLookupTableDialog::OnBnClickedOk()
{
    // 获取动作名称
    CString actionName;
    m_editActionName.GetWindowText(actionName);
    m_actionName = actionName.GetString();
    
    CDialogEx::OnOK();
}

void CZWLookupTableDialog::OnBnClickedCancel()
{
    CDialogEx::OnCancel();
}

// 设置和获取查寻表数据
void CZWLookupTableDialog::setLookupTable(const ZWLookupTable& table)
{
    m_lookupTable = table;
}

ZWLookupTable CZWLookupTableDialog::getLookupTable() const
{
    return m_lookupTable;
}

// 设置和获取动作名称
void CZWLookupTableDialog::setActionName(const std::wstring& name)
{
    m_actionName = name;
    if (m_isInitialized)
    {
        m_editActionName.SetWindowText(name.c_str());
    }
}

std::wstring CZWLookupTableDialog::getActionName() const
{
    return m_actionName;
}

// 添加参数特性对话框实现

IMPLEMENT_DYNAMIC(CZWAddPropertyDialog, CDialogEx)

// 构造函数
CZWAddPropertyDialog::CZWAddPropertyDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_ADD_PROPERTY_DIALOG, pParent)
    , m_isInitialized(false)
{}

// 析构函数
CZWAddPropertyDialog::~CZWAddPropertyDialog()
{}

// 对话框数据交换
void CZWAddPropertyDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PARAM_LIST, m_listParameters);
    DDX_Control(pDX, IDC_RADIO_INPUT, m_radioInput);
    DDX_Control(pDX, IDC_RADIO_QUERY, m_radioQuery);
}

BEGIN_MESSAGE_MAP(CZWAddPropertyDialog, CDialogEx)
    ON_BN_CLICKED(IDC_HELP_BUTTON, &CZWAddPropertyDialog::OnBnClickedHelpButton)
    ON_BN_CLICKED(IDOK, &CZWAddPropertyDialog::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CZWAddPropertyDialog::OnBnClickedCancel)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_PARAM_LIST, &CZWAddPropertyDialog::OnLvnItemchangedParamList)
END_MESSAGE_MAP()

// 对话框初始化
BOOL CZWAddPropertyDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    initializeControls();
    m_isInitialized = true;
    return TRUE;
}

// 初始化控件
void CZWAddPropertyDialog::initializeControls()
{
    // 初始化参数列表
    m_listParameters.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
    m_listParameters.InsertColumn(0, L"参数名称", LVCFMT_LEFT, 250);
    
    // 添加参数到列表
    for (const auto& param : m_parameters)
    {
        m_listParameters.InsertItem(m_listParameters.GetItemCount(), param.c_str());
    }
    
    // 初始化单选按钮
    m_radioInput.SetCheck(TRUE);
    m_radioQuery.SetCheck(FALSE);
}

// 控件事件处理
void CZWAddPropertyDialog::OnBnClickedHelpButton()
{
    // 帮助
    AfxMessageBox(L"帮助功能暂未实现。");
}

void CZWAddPropertyDialog::OnBnClickedOk()
{
    CDialogEx::OnOK();
}

void CZWAddPropertyDialog::OnBnClickedCancel()
{
    CDialogEx::OnCancel();
}

// 设置和获取参数列表
void CZWAddPropertyDialog::setParameters(const std::vector<std::wstring>& params)
{
    m_parameters = params;
}

std::wstring CZWAddPropertyDialog::getSelectedParameter() const
{
    std::wstring selectedParam;
    int selectedIndex = m_listParameters.GetNextItem(-1, LVNI_SELECTED);
    if (selectedIndex != -1)
    {
        selectedParam = m_listParameters.GetItemText(selectedIndex, 0).GetString();
    }
    return selectedParam;
}

// 获取特性类型
bool CZWAddPropertyDialog::isInputFeature() const
{
    return m_radioInput.GetCheck() == TRUE;
}

bool CZWAddPropertyDialog::isQueryFeature() const
{
    return m_radioQuery.GetCheck() == TRUE;
}

// 对话框辅助函数
namespace ZWLookupDialogs
{
    bool showLookupTableDialog(ZWLookupTable& table, std::wstring& actionName)
    {
        CZWLookupTableDialog dlg;
        dlg.setLookupTable(table);
        dlg.setActionName(actionName);
        
        if (dlg.DoModal() == IDOK)
        {
            table = dlg.getLookupTable();
            actionName = dlg.getActionName();
            return true;
        }
        return false;
    }

    bool showAddPropertyDialog(const std::vector<std::wstring>& parameters, std::wstring& selectedParam, bool& isInputFeature)
    {
        CZWAddPropertyDialog dlg;
        dlg.setParameters(parameters);
        
        if (dlg.DoModal() == IDOK)
        {
            selectedParam = dlg.getSelectedParameter();
            isInputFeature = dlg.isInputFeature();
            return true;
        }
        return false;
    }
}


void CZWLookupTableDialog::OnEnChangeActionName()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
}

void CZWLookupTableDialog::OnBnClickedGroupQuery()
{
    // TODO: 在此添加控件通知处理程序代码
}

void CZWAddPropertyDialog::OnLvnItemchangedParamList(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
}
