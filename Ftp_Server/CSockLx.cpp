#define _CRT_SECURE_NO_WARNINGS
#include "CSockLx.h"

#define TRUE 1
#define FALSE 0

BOOL CSockLx::Create(UINT nSocketPort, int nSocketType, const char* lpszScocketAddress)
{//socket,&bind
	m_hSocket = socket(AF_INET, nSocketType, 0);
	if (m_hSocket == INVALID_SOCKET)
	{
		return FALSE;
	}
	sockaddr_in sa = { AF_INET,htons(nSocketPort) };
	if (lpszScocketAddress)
	{
		sa.sin_addr.S_un.S_addr = inet_addr(lpszScocketAddress);
		return !bind(m_hSocket, (sockaddr*)&sa, sizeof(sa));
	}
	return TRUE;
}

int CSockLx::SendTo(const void * lpBuf, int nBufLen, UINT nHostPort, LPCSTR lpszScocketAddress, int Flags)
{
	sockaddr_in sa = { AF_INET,htons(nHostPort) };
	sa.sin_addr.S_un.S_addr = inet_addr(lpszScocketAddress);
	sendto(m_hSocket, (const char*)lpBuf, nBufLen, 0, (sockaddr*)&sa, sizeof(sa));
	return 0;
}

int CSockLx::ReceiveFrom(void * lpBuf, int nBufLen, char * rScoketAddress, UINT & rScoketPort, int Flags)
{
	sockaddr_in sa = { AF_INET };
	int nLen = sizeof(sa);
	int nRet = recvfrom(m_hSocket, (char *)lpBuf, nBufLen, 0, (sockaddr*)&sa, &nLen);
	if (nRet >0)
	{
		rScoketPort = htons(sa.sin_port);
		strcpy(rScoketAddress, inet_ntoa(sa.sin_addr));
	}
	return nRet;
}

BOOL CSockLx::GetsockName(char * rSocketAddress, UINT & rSocketPort)
{
	sockaddr_in sa = { AF_INET };
	int nLen = sizeof(sa);
	if (getsockname(m_hSocket, (sockaddr*)&sa, &nLen)<0)
		return FALSE;
	strcpy(rSocketAddress, inet_ntoa(sa.sin_addr));
	rSocketPort = htons(sa.sin_port);
	return  TRUE;
}

BOOL CSockLx::GetpeerName(char * SocketAddress, UINT & SocketPort)
{
	sockaddr_in sa = { AF_INET };
	int nLen = sizeof(sa);
	if (getpeername(m_hSocket, (sockaddr*)&sa, &nLen) < 0)
		return FALSE;
	strcpy(SocketAddress, inet_ntoa(sa.sin_addr));
	SocketPort = htons(sa.sin_port);
	return  TRUE;
}

BOOL CSockLx::Accept(CSockLx & socka, LPSTR szIP, UINT * nPort)
{
	sockaddr_in sa = { AF_INET };
	int nLen = sizeof(sa);
	socka.m_hSocket = accept(this->m_hSocket, (sockaddr*)&sa, &nLen);
	if (INVALID_SOCKET == socka.m_hSocket)
	{
		return FALSE;
	}
	if (szIP)//szIP不为空，取出对方的IP赋值给szIP
		strcpy(szIP, inet_ntoa(sa.sin_addr));
	if (nPort)//nPort不为空，取出对方的端口赋值给nPOrt
		*nPort = htons(sa.sin_port);
	return TRUE;
}

BOOL CSockLx::Connect(LPCSTR lpszHostAddress, UINT nHostPort)
{
	sockaddr_in sa = { AF_INET,htons(nHostPort) };
	sa.sin_addr.S_un.S_addr = inet_addr(lpszHostAddress);
	return !connect(m_hSocket, (sockaddr*)&sa, sizeof(sa));
}

CSockLx::CSockLx()
{
	WSADATA wd;
	WSAStartup(0x0202, &wd);
	m_hSocket = INVALID_SOCKET;
}


CSockLx::~CSockLx()
{
	Close();
}
