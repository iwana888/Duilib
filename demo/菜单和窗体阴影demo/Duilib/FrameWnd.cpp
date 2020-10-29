#include "stdafx.h"
#include "FrameWnd.h"
#include "PopUpWnd.h"
#include "CloseHintDialog.h"


CFrameWnd::CFrameWnd( LPCTSTR pszXMLPath )
	:  m_strXMLPath(pszXMLPath)
{

}

LPCTSTR CFrameWnd::GetWindowClassName() const
{
	return _T("MainWnd");
}

CDuiString CFrameWnd::GetSkinFile()
{
	return m_strXMLPath;
}

void CFrameWnd::InitWindow()
{
	CenterWindow();
	ToTray();
}

void CFrameWnd::Notify( TNotifyUI& msg )
{

	if( msg.sType == _T("click") ) 
	{	
		if( msg.pSender->GetName() == _T("btnMenu") ) 
		{
			CPopUpWnd *frm_pop = new CPopUpWnd();
			frm_pop->Create(NULL, _T("QQ弹出菜单"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
			frm_pop->ShowWindow();


			CPoint point(0, 0);
			GetCursorPos(&point);
			
			CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("menutest.xml"), point, &m_PaintManager, &m_MenuCheckInfo);

			//左侧打开菜单
			//CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("menutest.xml"), point, &m_PaintManager, &m_MenuCheckInfo, eMenuAlignment_Right );
			//左上侧打开菜单
			//CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("menutest.xml"), point, &m_PaintManager, &m_MenuCheckInfo, eMenuAlignment_Right | eMenuAlignment_Bottom);

			// 先获取到根项，然后就可以使用rootMenu插到到菜单内的任意子菜单项，然后做添加删除操作
			CMenuUI* rootMenu = pMenu->GetMenuUI();
			if (rootMenu != NULL)
			{
				CMenuElementUI* pNew = new CMenuElementUI;
				pNew->SetName(_T("Menu_Dynamic"));
				pNew->SetText(_T("动态一级菜单"));
				pNew->SetShowExplandIcon(true);
				pNew->SetIcon(_T("WebSit.png"));
				pNew->SetIconSize(16,16);


				CMenuElementUI* pSubNew = new CMenuElementUI;
				pSubNew->SetText(_T("动态二级菜单"));
				pSubNew->SetName(_T("Menu_Dynamic"));
				pSubNew->SetIcon(_T("Virus.png"));
				pSubNew->SetIconSize(16,16);
				pNew->Add(pSubNew);
				

				rootMenu->Add(pNew);


				CMenuElementUI* pNew2 = new CMenuElementUI;
				pNew2->SetName(_T("Menu_Dynamic"));
				pNew2->SetText(_T("动态一级菜单2"));
				rootMenu->AddAt(pNew2,2);
			}

			// 动态添加后重新设置菜单的大小
			pMenu->ResizeMenu();
		}
		else if (msg.pSender->GetName() == _T("closebtn") )
		{
			if (!OnExitConfirm())
			{
				//ToTray();
				return;
			}
		}
		
	}

	__super::Notify(msg);
}

 LRESULT CFrameWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
 {
	 OnTrayIcon(uMsg, wParam, lParam, bHandled);
/*	 wchar_t debug[MAXBYTE];
	 wsprintf(debug, L"消息:%d 类型:%d \n", uMsg, wParam);
	 OutputDebugString(debug)*/;


	 if (uMsg == WM_MENUCLICK)
	 {
		 CDuiString strMenuName = CMenuWnd::GetClickedMenuName();
		 BOOL bChecked = (BOOL)lParam;		 

		 if (strMenuName == _T("Menu_Test1")) 
		 {
			 if (bChecked)
			 {
				 MessageBox(m_hWnd, L"你选中Menu_Test1", L"", 0);
			 } 
			 else
			 {
				 MessageBox(m_hWnd, L"你取消Menu_Test1", L"", 0);
			 }			 
		 }
		 else if (strMenuName == _T("Menu_Test2")) 
		 {
			MessageBox(m_hWnd, L"你单击了Menu_Test2", L"", 0);		 
		 }
		 else if (strMenuName == _T("Menu_Test3")) 
		 {
			 if (bChecked)
			 {
				 MessageBox(m_hWnd, L"你选中Menu_Test3", L"", 0);
			 } 
			 else
			 {
				 MessageBox(m_hWnd, L"你取消Menu_Test3", L"", 0);
			 }			 
		 }
		 else if (strMenuName == _T("Menu_Dynamic")) 
		 {
			 MessageBox(m_hWnd, L"你单击了动态添加菜单", L"", 0);		 
		 }

	 }
	 else if (uMsg == 5 && wParam == 1)//最小化时 隐藏  尼玛 找了很久
	 {

		 ShowWindow(SW_HIDE);

	 }
	 bHandled = false;
	 return 0;
 }


 void CFrameWnd::ToTray()
 {
	 memset(&m_trayIcon, 0, sizeof(NOTIFYICONDATA));
	 m_trayIcon.cbSize = sizeof(NOTIFYICONDATA);
	 m_trayIcon.hIcon = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	 m_trayIcon.hWnd = m_hWnd;
	 lstrcpy(m_trayIcon.szTip, _T("啊啊啊啊"));
	 m_trayIcon.uCallbackMessage = WM_SHOWTASK;
	 m_trayIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	 Shell_NotifyIcon(NIM_ADD, &m_trayIcon);
	 ShowWindow(SW_HIDE);
 }

 LRESULT CFrameWnd::OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
 {
	 //如果在图标中单击左键则还原
	 if (lParam == WM_LBUTTONDOWN)
	 {
		 //删除托盘图标
		 //Shell_NotifyIcon(NIM_DELETE, &m_trayIcon);    //博主认为此处应注释掉
		 //显示主窗口
		 ShowWindow(SW_SHOWNORMAL);
	 }
	 //如果在图标中单击右键则弹出声明式菜单
	 if (lParam == WM_RBUTTONDOWN)
	 {
		 //获取鼠标坐标
		 POINT pt; GetCursorPos(&pt);
		 //右击后点别地可以清除“右击出来的菜单”
		 SetForegroundWindow(m_hWnd);
		 //托盘菜单    win32程序使用的是HMENU，如果是MFC程序可以使用CMenu
		 HMENU hMenu;
		 //生成托盘菜单
		 hMenu = CreatePopupMenu();
		 //添加菜单,关键在于设置的一个标识符  WM_ONCLOSE 点击后会用到
		 AppendMenu(hMenu, MF_STRING, WM_CLOSE, _T("Exit"));
		 //弹出菜单,并把用户所选菜单项的标识符返回
		 int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, m_hWnd, NULL);
		 //如果标识符是WM_ONCLOSE则关闭
		 if (cmd == WM_CLOSE)
		 {
			 m_trayIcon.hIcon = NULL;
			 Shell_NotifyIcon(NIM_DELETE, &m_trayIcon);
			 //退出程序
			 ::PostQuitMessage(0);
		 }
	 }
	 bHandled = true;
	 return 0;
 }


 bool CFrameWnd::OnExitConfirm()
 {
	 bool bResult = false;
	 //从配置文件中读取用户设置
	 TCHAR cofigValue[MAX_PATH];
	 TCHAR moduleFileName[MAX_PATH];
	 GetModuleFileName(NULL, moduleFileName, MAX_PATH);
	 std::wstring workingFolder(moduleFileName);
	 size_t pos = workingFolder.find_last_of(L"\\");
	 workingFolder = workingFolder.substr(0, pos + 1);
	 wstring tszModule = workingFolder.data();
	 tszModule += L"\\config.ini";
	 ::GetPrivateProfileString(L"Setting", L"ExitDirect", L"Error", cofigValue, MAX_PATH, tszModule.data());

	 if (_tcsicmp(cofigValue, _T("Error")) == 0)//配置文件里面没有
	 {
		 CCloseHintDialog closeHintBox;
		 closeHintBox.Create(this->GetHWND(), _T("CloseHintDialog"), UI_WNDSTYLE_EX_FRAME, WS_EX_WINDOWEDGE);
		 closeHintBox.CenterWindow();

		 if (closeHintBox.ShowModal() == IDOK)
		 {
			 if (closeHintBox.m_pchkOption->GetCheck())//勾选
			 {
				 if (closeHintBox.m_pExitbtn->IsSelected())
				 {
					 ::WritePrivateProfileString(L"Setting", L"ExitDirect", L"1", tszModule.data());

					 Close(IDCLOSE);

					 bResult = true;

					 return bResult;

				 }
				 else if (closeHintBox.m_pMoveToTraybtn->IsSelected())
				 {

					 ::WritePrivateProfileString(L"Setting", L"ExitDirect", L"0", tszModule.data());

					 ::ShowWindow(this->GetHWND(), SW_HIDE);

					 return bResult;
				 }
			 }
			 else//不勾选
			 {
				 if (closeHintBox.m_pExitbtn->IsSelected())
				 {
					 Close(IDCLOSE);

					 bResult = true;

					 return bResult;
				 }
				 else if (closeHintBox.m_pMoveToTraybtn->IsSelected())
				 {
					 ::ShowWindow(this->GetHWND(), SW_HIDE);
					 return bResult;
				 }
			 }
		 }
		 else
		 {
			 return bResult;
		 }
	 }
	 else
	 {
		 if (_tcsicmp(cofigValue, _T("1")) == 0)
		 {
			 Close(IDCLOSE);

			 bResult = true;

			 return bResult;
		 }
		 else if (_tcsicmp(cofigValue, _T("0")) == 0)
		 {
			 ::ShowWindow(this->GetHWND(), SW_HIDE);
			 return bResult;
		 }
	 }

	 return bResult;
 }