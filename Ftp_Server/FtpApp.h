#pragma once
#include"CSockLx.h"
#include"User.h"
#include<list>
struct SUser
{
	char sUser[20];
	char sPass[20];
	char sPath[256];
};
class CFtpApp
{
	CSockLx m_sock;
	static void theProc(void* p)
	{
		const char* sWelc = "220 欢迎使用FTP服务器！\r\n";  //220协议，表示欢迎信息，欢迎之后才能访问
		CUser* pUser = (CUser*)p;
		pUser->m_socka.Send(sWelc, strlen(sWelc));
		pUser->Work();
	}
	std::list<SUser>m_list;
public:
	CFtpApp();
	~CFtpApp();
	int Start();

	void Load();
	// 查找用户名是否存在
	bool FindUser(const char* sUser);
	bool Login(const char * sUser,const char* sPass, std::string& szPath);
};
extern CFtpApp theApp;

