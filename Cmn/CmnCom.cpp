#include "pch.h"
#include "CmnCom.h"

#include <iostream>
#include <functional>

#include <process.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// WinSocketを使用するためのlibファイル
#pragma comment(lib, "ws2_32.lib")

#define SHM_SIZE (sizeof(ShmData))
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

const TCHAR szName[] = TEXT("MyFileMappingObject");

/* for shared memory */
HANDLE CmnCom::m_hMapFile = NULL;

/* for socket */
SOCKET CmnCom::m_socket = INVALID_SOCKET;
EN_SOCKET_ROLE CmnCom::m_enRole = EN_SOCKET_ROLE::NONE;
CmnCom::CallbackFunc CmnCom::m_clbFunc_rcvMsg = NULL;

BOOL CmnCom::Initialize(CallbackFunc clbFunc_rcvMsg)
{
    /* Initialize shared memory */

    m_hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // use paging file
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
        SHM_SIZE,          // maximum object size (low-order DWORD)
        szName);                 // name of mapping object
    if (m_hMapFile == NULL)
    {
        return FALSE;
    }


    /* Initialize socket */

    int iResult;
    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        return FALSE;
    }

    if (NULL == clbFunc_rcvMsg)
    {
        return FALSE;
    }
    m_clbFunc_rcvMsg = clbFunc_rcvMsg;

    m_enRole = EN_SOCKET_ROLE::NONE;

    CreateThreadForRcvMsg();

    return TRUE;
}

BOOL CmnCom::StartCom(EN_SOCKET_ROLE enRole)
{
    if ((EN_SOCKET_ROLE::CLIENT == enRole) || (EN_SOCKET_ROLE::SERVER == enRole))
    {
        m_enRole = enRole;
        return TRUE;
    }
    
    return FALSE;
}

BOOL CmnCom::StartCom_Internal(EN_SOCKET_ROLE enRole)
{
    struct addrinfo* sockAdInfo = NULL;

    if (EN_SOCKET_ROLE::SERVER == enRole)
    {
        SOCKET ListenSocket = INVALID_SOCKET;
        m_socket = INVALID_SOCKET;

        struct addrinfo hints;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        int iResult;
        iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &sockAdInfo);
        if (iResult != 0) {
            WSACleanup();
            return FALSE;
        }

        // Create a SOCKET for the server to listen for client connections.
        ListenSocket = socket(sockAdInfo->ai_family, sockAdInfo->ai_socktype, sockAdInfo->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            freeaddrinfo(sockAdInfo);
            WSACleanup();
            return FALSE;
        }

        // Setup the TCP listening socket
        iResult = bind(ListenSocket, sockAdInfo->ai_addr, (int)sockAdInfo->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            freeaddrinfo(sockAdInfo);
            closesocket(ListenSocket);
            WSACleanup();
            return FALSE;
        }

        freeaddrinfo(sockAdInfo);

        iResult = listen(ListenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            closesocket(ListenSocket);
            WSACleanup();
            return FALSE;
        }

        // Accept a client socket
        m_socket = accept(ListenSocket, NULL, NULL);
        if (m_socket == INVALID_SOCKET) {
            closesocket(ListenSocket);
            WSACleanup();
            return FALSE;
        }

        // No longer need server socket
        closesocket(ListenSocket);

        return TRUE;
    }
    else if (EN_SOCKET_ROLE::CLIENT == enRole)
    {
        m_socket = INVALID_SOCKET;

        struct addrinfo hints;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        // Resolve the server address and port
        int iResult;
        struct addrinfo* result = NULL;
        iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &sockAdInfo);
        if (iResult != 0) {
            WSACleanup();
            return FALSE;
        }

        // Attempt to connect to an address until one succeeds
        for (struct addrinfo* ptr = sockAdInfo; ptr != NULL; ptr = ptr->ai_next) {

            // Create a SOCKET for connecting to server
            m_socket = socket(ptr->ai_family, ptr->ai_socktype,
                ptr->ai_protocol);
            if (m_socket == INVALID_SOCKET) {
                WSACleanup();
                return FALSE;
            }

            // Connect to server.
            iResult = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                int err = WSAGetLastError();
                closesocket(m_socket);
                m_socket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        freeaddrinfo(sockAdInfo);

        if (m_socket == INVALID_SOCKET) {
            WSACleanup();
            return FALSE;
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL CmnCom::Finalize(void)
{
    CloseHandle(m_hMapFile);

    closesocket(m_socket);
    WSACleanup();

    return TRUE;
}

void CmnCom::CreateThreadForRcvMsg(void)
{
    (void)_beginthreadex(NULL, 0,
        (_beginthreadex_proc_type)ThreadProc,
        0,0,NULL);

    return;
}

void CmnCom::ThreadProc(void)
{
    while (EN_SOCKET_ROLE::NONE == m_enRole)
    {
        NOP_FUNCTION;
    }
    StartCom_Internal(m_enRole);
    ReceiveMsg();
}


void CmnCom::ReceiveMsg(void)
{
    char recvbuf[DEFAULT_BUFLEN];
    memset(recvbuf, 0, DEFAULT_BUFLEN);

    while (1)
    {
        int n = recv(m_socket, recvbuf, sizeof(recvbuf), 0);
        if (n > 0)
        {
            std::cout << "receive message " << recvbuf << "\n";
            if (NULL == m_clbFunc_rcvMsg)
            {
                /* エラーログ残す */
                continue;
            }
            m_clbFunc_rcvMsg(recvbuf, n);
            memset(recvbuf, 0, DEFAULT_BUFLEN);
        }
    }
}

void CmnCom::ReadShm(ShmData* penShmDst)
{
    ShmData* penShm;

    penShm = (ShmData*)MapViewOfFile(m_hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        SHM_SIZE);

    if (penShm == NULL)
    {
        return;
    }

    memcpy(penShmDst, penShm, SHM_SIZE);

    UnmapViewOfFile(penShm);
}

void CmnCom::WriteShm(const ShmData* penShmSrc)
{
    ShmData* penShm;

    penShm = (ShmData*)MapViewOfFile(m_hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        SHM_SIZE);

    if (penShm == NULL)
    {
        return;
    }

    memcpy(penShm, penShmSrc, SHM_SIZE);

    UnmapViewOfFile(penShm);
}

void CmnCom::SendMsg(const char* cBuf, unsigned int unLen)
{
    send(m_socket, cBuf, unLen, 0);
}

void CmnCom::ConvMsgToCbuf(const OTHELLO_MSG* enMsg, char* pcBuf)
{
    pcBuf[0] = static_cast<char>((static_cast<unsigned int>(enMsg->enId) & 0x00ff));
    pcBuf[1] = static_cast<char>(enMsg->p1 & 0x00ff);
    pcBuf[2] = static_cast<char>((enMsg->p1 >> 8) & 0x00ff);
    pcBuf[3] = static_cast<char>(enMsg->p2 & 0x00ff);
    pcBuf[4] = static_cast<char>((enMsg->p2 >> 8) & 0x00ff);
    pcBuf[5] = static_cast<char>(enMsg->p3 & 0x00ff);
    pcBuf[6] = static_cast<char>((enMsg->p3 >> 8) & 0x00ff);
    pcBuf[7] = static_cast<char>(enMsg->p4 & 0x00ff);
    pcBuf[8] = static_cast<char>((enMsg->p4 >> 8) & 0x00ff);
}

void CmnCom::ConvCbufToMsg(const char* pcBuf, OTHELLO_MSG* enMsg)
{
    enMsg->enId = static_cast<OTHELLO_MSG_ID>(pcBuf[0]);

    unsigned int unTmp = (static_cast<unsigned int>(pcBuf[1]) & 0x00ff) | ((static_cast<unsigned int>(pcBuf[2]) << 8) & 0xff00);
    enMsg->p1 = static_cast<unsigned int>(unTmp);

    unTmp = (static_cast<unsigned int>(pcBuf[3]) & 0x00ff) | ((static_cast<unsigned int>(pcBuf[4]) << 8) & 0xff00);
    enMsg->p2 = static_cast<unsigned int>(unTmp);

    unTmp = (static_cast<unsigned int>(pcBuf[5]) & 0x00ff) | ((static_cast<unsigned int>(pcBuf[6]) << 8) & 0xff00);
    enMsg->p3 = static_cast<unsigned int>(unTmp);

    unTmp = (static_cast<unsigned int>(pcBuf[7]) & 0x00ff) | ((static_cast<unsigned int>(pcBuf[8]) << 8) & 0xff00);
    enMsg->p4 = static_cast<unsigned int>(unTmp);
}