#ifndef _POPUPWND_H_
#define _POPUPWND_H_

#include "WndShadow.h"

#define WIN_VISITWIDTH    280
#define WIN_VISITHEIGHT    186

#define ID_VISITPOP_WINDOW 20
#define ID_TIMER_DISPLAY_DELAY 30
#define ID_TIMER_CLOSE_WINDOW 40

class CPopUpWnd :public WindowImplBase
{
public:
    CPopUpWnd();

    ~CPopUpWnd();

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

    void InitControl();

    void StartTrack();

    void MoveSelfWindow();

    LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled);

    LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnMouseEnter(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);




public:
    CWndShadow m_WndShadow;

    CLabelUI* m_pMousePos;

    UINT m_uTime;//�ƶ�����ʱ��

protected:
    bool m_bMouseTracking;   // �Ƿ����ڸ������
    bool m_bMouseWithin;     // ����Ƿ��ڴ�����
};

#endif//_POPUPWND_H