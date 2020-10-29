#include "..\stdafx.h"
#include "CloseHintDialog.h"

CCloseHintDialog::CCloseHintDialog()
    :m_pBtnYES(NULL)
    ,m_pBtnNO(NULL)
    ,m_pExitbtn(NULL)
    ,m_pchkOption(NULL)
    ,m_pMoveToTraybtn(NULL)
{

}

CCloseHintDialog::~CCloseHintDialog()
{

}

LPCTSTR CCloseHintDialog::GetWindowClassName() const
{
    return TEXT("CloseHintDialog");
}

void CCloseHintDialog::OnFinalMessage(HWND hWnd)
{
    CWindowWnd::OnFinalMessage(hWnd);
}

CDuiString CCloseHintDialog::GetSkinFile()
{
    return TEXT("CloseHintDlg.xml");
}

CDuiString CCloseHintDialog::GetSkinFolder()
{
    return TEXT("");
}

LRESULT CCloseHintDialog::ResponseDefaultKeyEvent(WPARAM wParam)
{
    if (wParam == VK_RETURN)
    {
        return FALSE;
    }
    else if (wParam == VK_ESCAPE)
    {
        //Close();
        return TRUE;
    }
    return FALSE;
}

UILIB_RESOURCETYPE CCloseHintDialog::GetResourceType() const
{
    return UILIB_FILE;
}

CControlUI* CCloseHintDialog::CreateControl(LPCTSTR pstrClassName)
{
    return NULL;
}

void CCloseHintDialog::InitWindow()
{
    m_pBtnYES = static_cast<COptionUI *>(m_PaintManager.FindControl(TEXT("yesbtn")));

    m_pBtnNO = static_cast<COptionUI *>(m_PaintManager.FindControl(TEXT("nobtn")));

    m_pExitbtn = static_cast<COptionUI *>(m_PaintManager.FindControl(TEXT("ExitDirectbtn")));

    m_pMoveToTraybtn = static_cast<COptionUI *>(m_PaintManager.FindControl(TEXT("MoveInTraybtn")));

    m_pchkOption =  static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(TEXT("chkOption")));

}

void CCloseHintDialog::OnPrepare()
{

}

void CCloseHintDialog::Notify(TNotifyUI& msg)
{
    if( msg.sType == TEXT("windowinit") ) OnPrepare();

    if(msg.sType == TEXT("click"))
    {
		
        if(msg.pSender->GetName()==TEXT("closebtn"))
        {
			Close(IDCANCEL);
        }
        else if(msg.pSender->GetName() == TEXT("yesbtn"))
        {
            Close(IDOK);
        }
        else if (msg.pSender->GetName() == _T("nobtn"))
        {
            Close(IDCANCEL);
        }
    }

    WindowImplBase::Notify(msg);
}

void CCloseHintDialog::OnClick(TNotifyUI& msg)
{
	
    __super::OnClick(msg);
}

//禁用双击标题栏窗口最大化
LRESULT CCloseHintDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	
    default:
        bHandled = FALSE;
    }
    return WindowImplBase::HandleMessage(uMsg,wParam,lParam);


}


