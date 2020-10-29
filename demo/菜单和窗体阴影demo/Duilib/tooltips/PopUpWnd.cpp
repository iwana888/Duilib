#include "..\stdafx.h"
#include "PopUpWnd.h"
#pragma comment(lib, "comctl32.lib")

CPopUpWnd::CPopUpWnd()
    :m_uTime(0)
    ,m_bMouseTracking(false)
    ,m_bMouseWithin(false)
    ,m_pMousePos(NULL)
{

}

CPopUpWnd::~CPopUpWnd()
{
    if (m_uTime)
    {
        KillTimer(this->GetHWND(),ID_VISITPOP_WINDOW);
        m_uTime = 0;
    }
}

LPCTSTR CPopUpWnd::GetWindowClassName() const
{
    return _T("PopUpWnd");
}

void CPopUpWnd::OnFinalMessage(HWND hWnd)
{
    CWindowWnd::OnFinalMessage(hWnd);
}

CDuiString CPopUpWnd::GetSkinFile()
{
    return _T("PopUpDlg.xml");
}

CDuiString CPopUpWnd::GetSkinFolder()
{
    return _T("");
}

UILIB_RESOURCETYPE CPopUpWnd::GetResourceType() const
{
    return UILIB_FILE;
}

LRESULT CPopUpWnd::ResponseDefaultKeyEvent(WPARAM wParam)
{
    if (wParam == VK_RETURN)
    {
        return FALSE;
    }
    else if (wParam == VK_ESCAPE)
    {
        Close();
        return TRUE;
    }
    return FALSE;
}

CControlUI* CPopUpWnd::CreateControl(LPCTSTR pstrClassName)
{
    

    return NULL;
}

void CPopUpWnd::InitWindow()
{
    //窗体增加阴影
    CWndShadow::Initialize(m_PaintManager.GetInstance());
    m_WndShadow.Create(m_hWnd , true);
    m_WndShadow.SetSize(2);
    m_WndShadow.SetPosition(1, 1);

    //修改窗口风格
    //SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME );

    //加入WS_EX_LAYERED扩展属性
    LONG styleValue = ::GetWindowLong(m_hWnd, GWL_EXSTYLE); 

    styleValue &= ~(WS_EX_APPWINDOW);//当窗口可见时将一个顶层窗口放置在任务栏上
    styleValue |= WS_EX_TOOLWINDOW; //工具条窗口样式

    SetWindowLong(m_hWnd,GWL_EXSTYLE,styleValue);

    InitControl();

    //右下角慢慢弹出
    //m_uTime = SetTimer(m_hWnd,ID_VISITPOP_WINDOW,20,NULL);

   //在此调用MoveSelfWindow()方法  窗口不会显示在右下角,因为InitWindow在WM_CREATE 消息响应函数里调用，窗口还没创建出来
}

void CPopUpWnd::InitControl()
{
    m_pMousePos = static_cast<CLabelUI*>(m_PaintManager.FindControl(TEXT("mouse")));
}

void CPopUpWnd::OnPrepare()
{
    //直接显示在右下角
    MoveSelfWindow();

    //3秒后窗口自动关闭
    SetTimer(m_hWnd,ID_TIMER_DISPLAY_DELAY,3000,NULL);
}

void CPopUpWnd::Notify(TNotifyUI& msg)
{
    if( msg.sType == _T("windowinit") ) OnPrepare();

    if ( msg.sType == _T("click") )
    {
        
    }
    WindowImplBase::Notify(msg);
}

void CPopUpWnd::OnClick(TNotifyUI& msg)
{
    __super::OnClick(msg);
}

void CPopUpWnd::MoveSelfWindow()
{
    RECT rect;
    SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
    int y=rect.bottom-rect.top;
    int x=rect.right-rect.left;
    x=x-WIN_VISITWIDTH;
    y=y-WIN_VISITHEIGHT;

    MoveWindow(m_hWnd,x,y,WIN_VISITWIDTH,WIN_VISITHEIGHT,TRUE);
}

LRESULT CPopUpWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam,BOOL& bHandled)
{
    static int nHeight=0;
    int cy=GetSystemMetrics(SM_CYSCREEN);
    int cx=GetSystemMetrics(SM_CXSCREEN);
    RECT rect;
    SystemParametersInfo(SPI_GETWORKAREA,0,&rect,0);
    int y=rect.bottom-rect.top;
    int x=rect.right-rect.left;
    x=x-WIN_VISITWIDTH;

    switch(wParam)
    {//多个定时器
    case ID_VISITPOP_WINDOW:
        if(nHeight<=WIN_VISITHEIGHT)
        {
            ++nHeight;        
            MoveWindow(m_hWnd,x,y-WIN_VISITHEIGHT,WIN_VISITWIDTH,WIN_VISITHEIGHT,TRUE);
        }
        else
        {
            //nHeight = 0;
            KillTimer(m_hWnd,ID_VISITPOP_WINDOW);

            SetTimer(m_hWnd,ID_TIMER_DISPLAY_DELAY,3000,NULL);
        }
        break;
    case ID_TIMER_DISPLAY_DELAY:
        KillTimer(m_hWnd,ID_TIMER_DISPLAY_DELAY);
        Close(IDCLOSE);
        break;
    }
    return 0;
}


//LRESULT CPopUpWnd::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//    POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
//    //::ScreenToClient(m_hWnd,&pt);//转换成屏幕坐标
//    //POINT pt1 ;/*= { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };*/
//    //GetCursorPos(&pt1);
//    RECT rect;
//    ::GetWindowRect (m_hWnd,&rect);
//    //::ClientToScreen(m_hWnd,&pt);//转换成屏幕坐标
//    if (PtInRect(&rect,pt))//判断point是否在rect内部
//    {
//        KillTimer(m_hWnd,ID_TIMER_DISPLAY_DELAY);
//    }
//
//    return 0;
//}


void CPopUpWnd::StartTrack()
{
    TRACKMOUSEEVENT tme = {0};
    tme.cbSize = sizeof(tme);
    tme.hwndTrack = m_hWnd;
    tme.dwFlags = TME_HOVER | TME_LEAVE;
    tme.dwHoverTime = HOVER_DEFAULT;
    _TrackMouseEvent(&tme);
}

LRESULT CPopUpWnd::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    // 如果鼠标不在客户区内，就产生一个鼠标进入事件
    if(!m_bMouseWithin) {
        OnMouseEnter(uMsg /* WM_MOUSEMOVE */, wParam, lParam, bHandled);
    }

    // 开始跟踪鼠标停留/离开事件
    if(!m_bMouseTracking) {
        StartTrack();
        m_bMouseTracking = true;
        return 0;
    }
    

    TCHAR text[MAX_PATH];
    wsprintf(text,_T("鼠标移动: %d,%d"),GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	if (m_pMousePos != NULL)
    m_pMousePos->SetText(text);


    return 0;
}



LRESULT CPopUpWnd::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
     m_bMouseTracking = false;

     TCHAR text[MAX_PATH];
     wsprintf(text,_T("鼠标停留: %d,%d"),GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	 if (m_pMousePos != NULL)
     m_pMousePos->SetText(text);


     return 0;
}

LRESULT CPopUpWnd::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_bMouseTracking = false;
    m_bMouseWithin = false;

    SetTimer(m_hWnd,ID_TIMER_DISPLAY_DELAY,3000,NULL);


     TCHAR text[MAX_PATH];
     wsprintf(text,_T("鼠标离开: %d,%d"),GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	 if (m_pMousePos != NULL)
     m_pMousePos->SetText(text);

    return 0;
}

LRESULT CPopUpWnd::OnMouseEnter(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_bMouseWithin = true;

    
    KillTimer(m_hWnd,ID_TIMER_DISPLAY_DELAY);

    TCHAR text[MAX_PATH];
    wsprintf(text,_T("鼠标进入:%d,%d"),GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	if (m_pMousePos != NULL) 
        m_pMousePos->SetText(text);
    return 0;
}


LRESULT CPopUpWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = 0;
    BOOL bHandled = TRUE;

    switch( uMsg ) 
    {
    case WM_NCLBUTTONDBLCLK:
        //不做处理
        {
            return 0;
        }
        break;
    case WM_TIMER:
        lRes = OnTimer(uMsg,wParam,lParam,bHandled);
        break;
    case WM_MOUSELEAVE:
        lRes = OnMouseLeave(uMsg,wParam,lParam,bHandled);
        break;
    default:
        bHandled = FALSE;
    }
    return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}