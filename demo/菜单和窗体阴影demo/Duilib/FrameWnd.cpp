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
			frm_pop->Create(NULL, _T("QQ�����˵�"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
			frm_pop->ShowWindow();


			CPoint point(0, 0);
			GetCursorPos(&point);
			
			CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("menutest.xml"), point, &m_PaintManager, &m_MenuCheckInfo);

			//���򿪲˵�
			//CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("menutest.xml"), point, &m_PaintManager, &m_MenuCheckInfo, eMenuAlignment_Right );
			//���ϲ�򿪲˵�
			//CMenuWnd* pMenu = CMenuWnd::CreateMenu(NULL, _T("menutest.xml"), point, &m_PaintManager, &m_MenuCheckInfo, eMenuAlignment_Right | eMenuAlignment_Bottom);

			// �Ȼ�ȡ�����Ȼ��Ϳ���ʹ��rootMenu�嵽���˵��ڵ������Ӳ˵��Ȼ�������ɾ������
			CMenuUI* rootMenu = pMenu->GetMenuUI();
			if (rootMenu != NULL)
			{
				CMenuElementUI* pNew = new CMenuElementUI;
				pNew->SetName(_T("Menu_Dynamic"));
				pNew->SetText(_T("��̬һ���˵�"));
				pNew->SetShowExplandIcon(true);
				pNew->SetIcon(_T("WebSit.png"));
				pNew->SetIconSize(16,16);


				CMenuElementUI* pSubNew = new CMenuElementUI;
				pSubNew->SetText(_T("��̬�����˵�"));
				pSubNew->SetName(_T("Menu_Dynamic"));
				pSubNew->SetIcon(_T("Virus.png"));
				pSubNew->SetIconSize(16,16);
				pNew->Add(pSubNew);
				

				rootMenu->Add(pNew);


				CMenuElementUI* pNew2 = new CMenuElementUI;
				pNew2->SetName(_T("Menu_Dynamic"));
				pNew2->SetText(_T("��̬һ���˵�2"));
				rootMenu->AddAt(pNew2,2);
			}

			// ��̬��Ӻ��������ò˵��Ĵ�С
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
	 wsprintf(debug, L"��Ϣ:%d ����:%d \n", uMsg, wParam);
	 OutputDebugString(debug)*/;


	 if (uMsg == WM_MENUCLICK)
	 {
		 CDuiString strMenuName = CMenuWnd::GetClickedMenuName();
		 BOOL bChecked = (BOOL)lParam;		 

		 if (strMenuName == _T("Menu_Test1")) 
		 {
			 if (bChecked)
			 {
				 MessageBox(m_hWnd, L"��ѡ��Menu_Test1", L"", 0);
			 } 
			 else
			 {
				 MessageBox(m_hWnd, L"��ȡ��Menu_Test1", L"", 0);
			 }			 
		 }
		 else if (strMenuName == _T("Menu_Test2")) 
		 {
			MessageBox(m_hWnd, L"�㵥����Menu_Test2", L"", 0);		 
		 }
		 else if (strMenuName == _T("Menu_Test3")) 
		 {
			 if (bChecked)
			 {
				 MessageBox(m_hWnd, L"��ѡ��Menu_Test3", L"", 0);
			 } 
			 else
			 {
				 MessageBox(m_hWnd, L"��ȡ��Menu_Test3", L"", 0);
			 }			 
		 }
		 else if (strMenuName == _T("Menu_Dynamic")) 
		 {
			 MessageBox(m_hWnd, L"�㵥���˶�̬��Ӳ˵�", L"", 0);		 
		 }

	 }
	 else if (uMsg == 5 && wParam == 1)//��С��ʱ ����  ���� ���˺ܾ�
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
	 lstrcpy(m_trayIcon.szTip, _T("��������"));
	 m_trayIcon.uCallbackMessage = WM_SHOWTASK;
	 m_trayIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	 Shell_NotifyIcon(NIM_ADD, &m_trayIcon);
	 ShowWindow(SW_HIDE);
 }

 LRESULT CFrameWnd::OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
 {
	 //�����ͼ���е��������ԭ
	 if (lParam == WM_LBUTTONDOWN)
	 {
		 //ɾ������ͼ��
		 //Shell_NotifyIcon(NIM_DELETE, &m_trayIcon);    //������Ϊ�˴�Ӧע�͵�
		 //��ʾ������
		 ShowWindow(SW_SHOWNORMAL);
	 }
	 //�����ͼ���е����Ҽ��򵯳�����ʽ�˵�
	 if (lParam == WM_RBUTTONDOWN)
	 {
		 //��ȡ�������
		 POINT pt; GetCursorPos(&pt);
		 //�һ�����ؿ���������һ������Ĳ˵���
		 SetForegroundWindow(m_hWnd);
		 //���̲˵�    win32����ʹ�õ���HMENU�������MFC�������ʹ��CMenu
		 HMENU hMenu;
		 //�������̲˵�
		 hMenu = CreatePopupMenu();
		 //��Ӳ˵�,�ؼ��������õ�һ����ʶ��  WM_ONCLOSE �������õ�
		 AppendMenu(hMenu, MF_STRING, WM_CLOSE, _T("Exit"));
		 //�����˵�,�����û���ѡ�˵���ı�ʶ������
		 int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, m_hWnd, NULL);
		 //�����ʶ����WM_ONCLOSE��ر�
		 if (cmd == WM_CLOSE)
		 {
			 m_trayIcon.hIcon = NULL;
			 Shell_NotifyIcon(NIM_DELETE, &m_trayIcon);
			 //�˳�����
			 ::PostQuitMessage(0);
		 }
	 }
	 bHandled = true;
	 return 0;
 }


 bool CFrameWnd::OnExitConfirm()
 {
	 bool bResult = false;
	 //�������ļ��ж�ȡ�û�����
	 TCHAR cofigValue[MAX_PATH];
	 TCHAR moduleFileName[MAX_PATH];
	 GetModuleFileName(NULL, moduleFileName, MAX_PATH);
	 std::wstring workingFolder(moduleFileName);
	 size_t pos = workingFolder.find_last_of(L"\\");
	 workingFolder = workingFolder.substr(0, pos + 1);
	 wstring tszModule = workingFolder.data();
	 tszModule += L"\\config.ini";
	 ::GetPrivateProfileString(L"Setting", L"ExitDirect", L"Error", cofigValue, MAX_PATH, tszModule.data());

	 if (_tcsicmp(cofigValue, _T("Error")) == 0)//�����ļ�����û��
	 {
		 CCloseHintDialog closeHintBox;
		 closeHintBox.Create(this->GetHWND(), _T("CloseHintDialog"), UI_WNDSTYLE_EX_FRAME, WS_EX_WINDOWEDGE);
		 closeHintBox.CenterWindow();

		 if (closeHintBox.ShowModal() == IDOK)
		 {
			 if (closeHintBox.m_pchkOption->GetCheck())//��ѡ
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
			 else//����ѡ
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