/*****************************************************/
/* Author : Joshua Eliser */
/*****************************************************/
#include "Ethernet.h"


#ifdef __cplusplus
extern "C" {
#endif


/****************************************/
int32_t ConfigBlockingState(Socket_t *mSock, int32_t mBlocking) {
/****************************************/
    /* Set the socket to be non-mBlocking */
    if(IOCTL(mSock->fd, FIONBIO, &mBlocking) < 0) {
        printf("ERR: Configuring socket to non-mBlocking  (%d)\n", mSock->fd);
        return(-1);
    }
    
    return(0);
}


/****************************************/
int32_t ConfigRecvSocketTimeout(Socket_t *mSock, int32_t msec) {
/****************************************/
    /* Variable Declaration */
    int32_t mRetVal = 0;
    int32_t mParamLen = sizeof(mRetVal);

    /* Set the socket to have a timeout */
    if(setsockopt(mSock->fd, SOL_SOCKET, SO_RCVTIMEO, (void*)&msec, mParamLen) < 0) {
        /*printf("ERR: Configuring recv socket for timeout  (%d)\n", WSAGetLastError());*/
        printf("ERR: Configuring recv socket for timeout\n");
        return(-1);
    }

    /* Make sure the buffer was set to proper size */
    getsockopt(mSock->fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&mRetVal, (socklen_t*)&mParamLen);
    if(mRetVal != msec) {
        printf("ERR: Configuring socket recv timeout to %d, expected %d\n", mRetVal, msec);
        return(-1);
    }
    
    return(0);
}


/****************************************/
int32_t ConfigXmitSocketTimeout(Socket_t *mSock, int msec) {
/****************************************/
    /* Variable Declaration */
    int32_t mRetVal = 0;
    int32_t mParamLen = sizeof(mRetVal);

    /* Set the socket to have a timeout */
    if(setsockopt(mSock->fd, SOL_SOCKET, SO_SNDTIMEO, (void*)&msec, mParamLen) < 0) {
        printf("ERR: Configuring xmit socket for timeout\n");
        return(-1);
    }

    /* Make sure the buffer was set to proper size */
    getsockopt(mSock->fd, SOL_SOCKET, SO_SNDTIMEO, (char*)&mRetVal, (socklen_t*)&mParamLen);
    if(mRetVal != msec) {
        printf("ERR: Configuring socket xmit timeout to %d, expected %d\n", mRetVal, msec);
        return(-1);
    }
    
    return(0);
}


/****************************************/
int ConfigRecvBuffSize(Socket_t *mSock, int mBuffSize) {
/****************************************/
    /* Variable Declaration */
    int32_t mRetVal = 0;
    int32_t mParamLen = sizeof(mRetVal);
    
    /* Sent the buffer size */
    if(setsockopt(mSock->fd, SOL_SOCKET, SO_RCVBUF, (char*)&mBuffSize, mParamLen) < 0) {
        printf("ERR: Configuring socket recv buffer size to %d\n", mBuffSize);
        return(-1);
    }

    /* Make sure the buffer was set to proper size */
    getsockopt(mSock->fd, SOL_SOCKET, SO_RCVBUF, (char*)&mRetVal, (socklen_t*)&mParamLen);
    if(mRetVal != mBuffSize) {
        printf("ERR: Configuring socket recv buffer size to %d, current size %d\n", mBuffSize, mRetVal);
        return(-1);
    }
    
    return(0);
}


/****************************************/
int ConfigXmitBuffSize(Socket_t *mSock, int mBuffSize) {
/****************************************/
    /* Variable Declaration */
    int32_t mRetVal = 0;
    int32_t mParamLen = sizeof(mRetVal);

    /* Sent the buffer size */
    if(setsockopt(mSock->fd, SOL_SOCKET, SO_SNDBUF, (char*)&mBuffSize, mParamLen) < 0) {
        printf("ERR: Configuring socket xmit buffer size to %d\n", mBuffSize);
        return(-1);
    }

    /* Make sure the buffer was set to proper size */
    getsockopt(mSock->fd, SOL_SOCKET, SO_SNDBUF, (char*)&mRetVal, (socklen_t*)&mParamLen);
    if(mRetVal != mBuffSize) {
        printf("ERR: Configuring socket xmit buffer size to %d, current size %d\n", mBuffSize, mRetVal);
        return(-1);
    }
    
    return(0);
}


/****************************************/
int CloseSocket(Socket_t *mSock) {
/****************************************/
    /* Close the socket */
    if(mSock->fd) {
        CLOSE_SOCKET(mSock->fd);
    }
    mSock->fd = 0;

    return(0);
}


/****************************************/
int32_t InitSocket(Socket_t *mSock, SOCKET_TYPE_e mSockType, char *mIpAddr, uint32_t mPortNum) {
/****************************************/
    /* Variable Declaration */
    int32_t mRetVal = 0;
    
    /* Sanity Check */
    if(mSock == NULL) {
        printf("ERR: BlockData is NULL\n");
        return(-1);
    }
    
    /* UDP_SERVER */
    if(mSockType == SOCKET_TYPE_UDP_SERVER) {
        /* Open the socket file descriptor */
        if((mSock->fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
            printf("ERR: Opening UDP server socket\n");
            return(-1);
        }

        /* Load the socket structure */
        memset(&mSock->mAddr, 0, sizeof(mSock->mAddr));
        mSock->mAddr.sin_family      = AF_INET;
        mSock->mAddr.sin_port        = htons(mPortNum);
        mSock->mAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        mSock->mAddrLen = sizeof(mSock->mAddr);
        
        /* Bind on the socket port */
        if((mRetVal = bind(mSock->fd, (struct sockaddr*)&mSock->mAddr, sizeof(mSock->mAddr))) < 0 ) {
            printf("ERR: Binding on port %d\n", mPortNum);
            CloseSocket(mSock);
            return(-1);
        }
    }
    /* UDP_CLIENT */
    else if(mSockType == SOCKET_TYPE_UDP_CLIENT) {
        /* Open the socket file descriptor */
        if((mSock->fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
            printf("ERR: Opening UDP client socket\n");
            return(-1);
        }
        
        /* Load the socket structure */
        memset(&mSock->mAddr, 0, sizeof(mSock->mAddr));
        mSock->mAddr.sin_family      = AF_INET;
        mSock->mAddr.sin_port        = htons(mPortNum);
        mSock->mAddr.sin_addr.s_addr = inet_mAddr(mIpAddr);
        mSock->mAddrLen = sizeof(mSock->mAddr);
    }
    /* TCP_SERVER */
    else if(mSockType == SOCKET_TYPE_TCP_SERVER) {
        /* Open the socket file descriptor */
        if((mSock->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ) {
            printf("ERR: Opening TCP server socket\n");
            return(-1);
        }
        
        /* Load the socket structure */
        memset(&mSock->mAddr, 0, sizeof(mSock->mAddr));
        mSock->mAddr.sin_family      = AF_INET;
        mSock->mAddr.sin_port        = htons(mPortNum);
        mSock->mAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        mSock->mAddrLen = sizeof(mSock->mAddr);

        /* Bind on the socket port */
        if((mRetVal = bind(mSock->fd, (struct sockaddr*)&mSock->mAddr, sizeof(mSock->mAddr))) < 0 ) {
            printf("ERR: Binding on port %d\n", mPortNum);
            CloseSocket(mSock);
            return(-1);
        }

        /* Listen on the port */
        if(listen(mSock->fd, 1) < 0) {
            printf("ERR: Listen on port %d\n", mPortNum);
            CloseSocket(mSock);
            return(-1);
        }
    }
    /* TCP_CLIENT */
    else if(mSockType == SOCKET_TYPE_TCP_CLIENT) {
        /* Open the socket file descriptor */
        if((mSock->fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ) {
            printf("ERR: Opening TCP client socket\n");
            return(-1);
        }
        
        /* Load the socket structure */
        memset(&mSock->mAddr, 0, sizeof(mSock->mAddr));
        mSock->mAddr.sin_family      = AF_INET;
        mSock->mAddr.sin_port        = htons(mPortNum);
        mSock->mAddr.sin_addr.s_addr = inet_mAddr(mIpAddr);
        mSock->mAddrLen = sizeof(mSock->mAddr);

        mSock->mConnected = FALSE;
        /* Connect the socket */
        if(connect(mSock->fd, (struct sockaddr*)&mSock->mAddr, sizeof(mSock->mAddr)) < 0) {
            printf("ERR: Connecting to remote TCP server\n");
            CloseSocket(mSock);
            return(-1);
        }
        mSock->mConnected = TRUE;
    }
    else {
        printf("ERR: Invalid socket type: %d\n", mSockType);
        return(-1);
    }

    return(0);
}


/****************************************/
int32_t RecvData(Socket_t *mSock, unsigned char *mBuff, int32_t mNumBytes) {
/****************************************/
    /* Variable Declaration */
    int32_t i = 0;
    int32_t mRetVal = 0;
    int32_t mRecvBytes = 0;
   
    /* Get the image from the socket. */
    for(i = 0, mRecvBytes = 0; (mRecvBytes < mNumBytes) && (i < MAX_TCP_READS); mRecvBytes += mRetVal, i++) {
        if((mRetVal = recvfrom(mSock->fd, &mBuff[mRecvBytes], (mNumBytes - mRecvBytes), 0, (struct sockaddr *)&mSock->mAddr, (socklen_t*)&mSock->mAddrLen)) < 0) {
            printf("ERR: Failed to recieve socket data: %d of %d\n", mRecvBytes, mNumBytes);
            return(-1);
        }
    }

    return(mRecvBytes);
}


/****************************************/
int32_t XmitData(Socket_t *mSock, unsigned char *mBuff, int32_t mNumBytes) {
/****************************************/
    /* Variable Declaration */
    int32_t mXmitBytes = 0;

    /* Send the data over the socket */
    if((mXmitBytes = sendto(mSock->fd, mBuff, mNumBytes, 0, (struct sockaddr *)&mSock->mAddr, (int)mSock->mAddrLen)) < 0) {
#ifdef _WIN32
        printf("ERR: Failed to xmit socket data: %d of %d  (%d)\n", mXmitBytes, mNumBytes, WSAGetLastError());
#else
        printf("ERR: Failed to xmit socket data: %d of %d  (%d)\n", mXmitBytes, mNumBytes, errno);
#endif
        mSock->mConnected = FALSE;
        return(-1);
    }
        
    return(mXmitBytes);
}


#ifdef __cplusplus
}
#endif



