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
	//定义托盘图标对象
	 NOTIFYICONDATA m_trayIcon;
	 //添加托盘图标(初始化)
    void ToTray();
    //处理托盘图标上的事件
    LRESULT OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	map<CDuiString,bool> m_MenuCheckInfo; //保存菜单的单选复选信息
	//退出时弹出确认
	bool OnExitConfirm();
};