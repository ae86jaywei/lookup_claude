// ZWLookupDialog.h
// 对话框实现

#pragma once

#include "stdafx.h"
#include "resource.h"
#include "ZWLookupDefs.h"
#include "ZWLookupTable.h"
#include "ZWLookupAction.h"

// 特性查寻表对话框类
class CZWLookupTableDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CZWLookupTableDialog)

public:
    CZWLookupTableDialog(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CZWLookupTableDialog();

    // 对话框数据
    enum { IDD = IDD_LOOKUP_TABLE_DIALOG };

    // 设置和获取查寻表数据
    void setLookupTable(const ZWLookupTable& table);
    ZWLookupTable getLookupTable() const;

    // 设置和获取动作名称
    void setActionName(const std::wstring& name);
    std::wstring getActionName() const;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()

    // 控件变量
    CEdit m_editActionName;
    CListCtrl m_listInputProperties;
    CListCtrl m_listQueryProperties;
    CEdit m_editQueryValue;

    // 数据变量
    ZWLookupTable m_lookupTable;
    std::wstring m_actionName;
    bool m_isInitialized;

    // 初始化函数
    virtual BOOL OnInitDialog() override;
    void initializeControls();

    // 控件事件处理
    afx_msg void OnBnClickedAddFeature();
    afx_msg void OnBnClickedCheck();
    afx_msg void OnBnClickedHelpButton();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
public:
    afx_msg void OnEnChangeActionName();
    afx_msg void OnBnClickedGroupQuery();
};

// 添加参数特性对话框类
class CZWAddPropertyDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CZWAddPropertyDialog)

public:
    CZWAddPropertyDialog(CWnd* pParent = nullptr);   // 标准构造函数
    virtual ~CZWAddPropertyDialog();

    // 对话框数据
    enum { IDD = IDD_ADD_PROPERTY_DIALOG };

    // 设置和获取参数列表
    void setParameters(const std::vector<std::wstring>& params);
    std::wstring getSelectedParameter() const;

    // 获取特性类型
    bool isInputFeature() const;
    bool isQueryFeature() const;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()

    // 控件变量
    CListCtrl m_listParameters;
    CButton m_radioInput;
    CButton m_radioQuery;

    // 数据变量
    std::vector<std::wstring> m_parameters;
    bool m_isInitialized;

    // 初始化函数
    virtual BOOL OnInitDialog() override;
    void initializeControls();

    // 控件事件处理
    afx_msg void OnBnClickedHelpButton();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
public:
    afx_msg void OnLvnItemchangedParamList(NMHDR* pNMHDR, LRESULT* pResult);
};

