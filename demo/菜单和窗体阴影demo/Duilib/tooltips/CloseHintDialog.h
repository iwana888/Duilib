#ifndef _CLOSEHINTDIALOG_H_
#define _CLOSEHINTDIALOG_H_

class CCloseHintDialog :public WindowImplBase
{
public:
    CCloseHintDialog();

    ~CCloseHintDialog();

public:
    virtual LPCTSTR GetWindowClassName() const;

    virtual void OnFinalMessage(HWND hWnd);

    virtual CDuiString GetSkinFile();

    virtual CDuiString GetSkinFolder();

    virtual UILIB_RESOURCETYPE GetResourceType() const;

    virtual CControlUI* CreateControl(LPCTSTR pstrClassName);

    virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual void Notify(TNotifyUI& msg);

    virtual void OnClick(TNotifyUI& msg);

    virtual void InitWindow();

    void OnPrepare();

	

public:

    COptionUI  * m_pBtnYES;

    COptionUI * m_pBtnNO;

    COptionUI  * m_pExitbtn;

    COptionUI  * m_pMoveToTraybtn;

    CCheckBoxUI* m_pchkOption;
};
#endif//_CLOSEHINTDIALOG_H_