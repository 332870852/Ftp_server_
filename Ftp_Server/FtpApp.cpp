#include "FtpApp.h"
#include<iostream>
#include<process.h>
#include"User.h"
using namespace std;


CFtpApp::CFtpApp()
{
}


CFtpApp::~CFtpApp()
{
}


int CFtpApp::Start()
{
	Load();//加载账户
	if (!m_sock.Create(21,1,"0.0.0.0"))  //10.34.9.79
	{
		cout << "端口创建失败:" << GetLastError() << endl;
	}
	cout << "ftp server open" << endl;
	m_sock.Listen();
	CUser* pUser = new CUser;
	const char* sWelc = "220 欢迎使用FTP服务器！\r\n";  //220协议，表示欢迎信息，欢迎之后才能访问
	char sIP[20];
	UINT sPort;
	while (m_sock.Accept(pUser->m_socka, sIP, &sPort))
	{
		cout << sIP << "-" << sPort << "登陆：" << endl;
		_beginthread(theProc, NULL, pUser);

		pUser = new CUser;
	}
	return 0;
}

void CFtpApp::Load()
{
	{
		SUser us[] =
		{
			{ "test","test","e:\\users" },//   //c:\\Users\\Administrator\\Desktop
			{ "admin","123456","c:\\Users" },//e:\\users          //c:\\Users
			{ "root","123456","/c:\\Users\\Administrator" } //e:\\users\\test    //c:\\Users\\Administrator
		};
		m_list.push_back(us[0]);
		m_list.push_back(us[1]);
		m_list.push_back(us[2]);
	}
}

bool CFtpApp::FindUser(const char* sUser)
{
	list<SUser>::const_iterator it = m_list.begin();
	_strlwr((char*)sUser);
	while (it != m_list.end())
	{
		if (!strcmp(it++->sUser, sUser))
			return true;
		
	}
	return false;
}

bool CFtpApp::Login(const char * sUser, const char * sPass, std::string & szPath)
{
	list<SUser>::const_iterator it = m_list.begin();
	
	while (it != m_list.end())
	{
		const SUser& u = *it;
		if (!strcmp(u.sUser, sUser))
		{
			if (sPass == NULL)
				return false;
			if (strcmp(u.sPass, sPass))
				return false;
			szPath = u.sPath;
			return true;
		}
		++it;
	}
	return false;
}
