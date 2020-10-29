#pragma once

#define WM_SHOWTASK (WM_USER +1)



class CFrameWnd: public WindowImplBase
{
public:
	explicit CFrameWnd(LPCTSTR pszXMLPath);

	LPCTSTR GetWindowClassName() const override;
	CDuiString GetSkinFile() override;

     void InitWindow();
     void Notify(TNotifyUI& msg);
	 LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;



private:
	CDuiString		m_strXMLPath;
	//��������ͼ�����
	 NOTIFYICONDATA m_trayIcon;
	 //�������ͼ��(��ʼ��)
    void ToTray();
    //��������ͼ���ϵ��¼�
    LRESULT OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	map<CDuiString,bool> m_MenuCheckInfo; //����˵��ĵ�ѡ��ѡ��Ϣ
	//�˳�ʱ����ȷ��
	bool OnExitConfirm();
};