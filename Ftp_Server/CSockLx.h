#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
class CSockLx
{
protected:
	SOCKET  m_hSocket;//核心成员变量
public:
	BOOL Create(UINT nSocketPort = 0, int nSocketType = SOCK_STREAM, const char* lpszScoketAddress = NULL);//端口，（UDP和TCP）类型，IP地址
	int SendTo(const void *lpBuf, int nBufLen, UINT nHostPort, LPCSTR lpszScocketAddress = NULL, int Flags = 0);//发送
	int ReceiveFrom(void *lpBuf, int nBufLen, char *rScoketAddress, UINT &rScoketPort, int Flags = 0);//接收信息和对方IP，端口
	int Receive(void *lpBuf, int nBufLen, int nFlags = 0)//接受信息
	{
		return recv(m_hSocket, (char *)lpBuf, nBufLen, nFlags);
	}
	BOOL GetsockName(char *SocketAddress, UINT &SocketPort);//接收本机IP和端口信息
	BOOL GetpeerName(char *SocketAddress, UINT &SocketPort);//接收本机IP和端口信息（常用于TCP）
	BOOL Accept(CSockLx &rconnetedSocket, LPSTR szIP = NULL, UINT *nPort = NULL);
	BOOL Connect(LPCSTR lpszHostPort, UINT nHostPort);
	BOOL Listen(int nConnectionBacklog = 5)//侦听
	{
		return!listen(m_hSocket, nConnectionBacklog);
	}
	int Send(const void*lpBuf, int BufLen, int nFlags = 0)//发送
	{
		return send(m_hSocket, (LPCSTR)lpBuf, BufLen, nFlags);
	}
	void Close()//关闭
	{
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
	SOCKET GetSock()
	{
		return m_hSocket;
	}
	CSockLx();
	~CSockLx();
};

