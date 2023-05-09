#include <utils_net.h>

int32_t util_net_create_server(util_net_server_context_t *ctx)
{
    struct sockaddr_un unet;
    int32_t            sockId;
    int32_t            sockFamily;
    int32_t            sockType;
    int32_t            protocol;
    int32_t            svrAddrSize;
    int32_t            status;

    if (NULL == ctx) {
        UTIL_NET_PRINT("param error \r\n");
        return -1;
    }
    
    status = UTIL_NET_RET_SUCCESS;
    sockId = -1;

    memset(&unet, 0, sizeof(struct sockaddr_un));

    sockFamily        = AF_UNIX;
    sockType          = SOCK_SEQPACKET;
    protocol          = 0;
    svrAddrSize       = sizeof(struct sockaddr_un);
    unet.sun_family  = AF_UNIX;

    strncpy(unet.sun_path, ctx->path_name, sizeof(unet.sun_path)-1);
    unlink(ctx->path_name);

    /* Open a socket. */
    sockId = socket(sockFamily, sockType, protocol);

    if (sockId < 0)
    {
        UTIL_NET_PRINT("socket() failed\n");
        perror(__FUNCTION__);
        status = UTIL_NET_SOCK_CREATE_ERROR;
    }

    /* Bind the socket. */
    if (status == UTIL_NET_RET_SUCCESS)
    {
        status = bind(sockId, (struct sockaddr*)&unet, svrAddrSize);

        if (status < 0)
        {
            UTIL_NET_PRINT("bind() failed\n");
            perror(__FUNCTION__);
            status = UTIL_NET_SOCK_BIND_ERROR;
        }
    }

    if (status == UTIL_NET_RET_SUCCESS)
    {
        status = listen(sockId, ctx->listen_cnt);

        if (status < 0)
        {
            UTIL_NET_PRINT("listen failed\n");
            status = UTIL_NET_SOCK_LISTEN_ERROR;
        }
    }

    if ((status != UTIL_NET_RET_SUCCESS) && (sockId >= 0))
    {
        close(sockId);
        sockId = -1;
    }

    ctx->svr_sock = sockId;

    return sockId;
}

int32_t util_net_destroy_server(util_net_server_context_t *ctx)
{
    if (NULL == ctx) {
        UTIL_NET_PRINT("param error \r\n");
        return -1;
    }
    
    ctx->exit_flag = 1;
    
    if (ctx->svr_sock >= 0) {
        // close(ctx->svr_sock);
        shutdown(ctx->svr_sock, SHUT_RDWR);
    }

    return 0;
}

int32_t util_net_process_server(util_net_server_context_t *ctx)
{
    if (NULL == ctx) {
        UTIL_NET_PRINT("param error \r\n");
        return -1;
    }
    
    while (ctx->exit_flag == 0)
    {
        struct sockaddr svrAddr;
        socklen_t       addrLen;
        int32_t         cliSock;

        addrLen = sizeof(struct sockaddr);

        cliSock = accept(ctx->svr_sock,
                         (struct sockaddr *)&svrAddr,
                         &addrLen);

        if (cliSock < 0)
        {
            UTIL_NET_PRINT("accept() failed\n");
            break;
        }

        /* Handle the incoming connection. */
        ctx->client_process(ctx->user_data, &cliSock);

        close(cliSock);
    }
    
    return 0;
}

int32_t util_net_connect_client(const char *path_name)
{
    struct sockaddr_un  svrAddr;
    int32_t             status;
    int32_t             sockId;

    if (NULL == path_name) {
        UTIL_NET_PRINT("param error \r\n");
        return -1;
    }
    
    status = UTIL_NET_RET_SUCCESS;
    sockId = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sockId < 0)
    {
        UTIL_NET_PRINT("socket() failed\n");
        perror(__FUNCTION__);
        status = UTIL_NET_SOCK_CREATE_ERROR;
    }

    if (status == 0)
    {
        memset(&svrAddr, 0, sizeof(struct sockaddr_un));
        svrAddr.sun_family = AF_UNIX;
        strncpy(svrAddr.sun_path, path_name, sizeof(svrAddr.sun_path)-1);

        status = connect(sockId,
                         (struct sockaddr *)&svrAddr,
                         sizeof(svrAddr));

        if (status < 0)
        {
            UTIL_NET_PRINT("connect() failed\n");
            perror(__FUNCTION__);
            status = UTIL_NET_SOCK_CONNECT_ERROR;
        }
    }

    if ((status != UTIL_NET_RET_SUCCESS) && (sockId >= 0))
    {
        close(sockId);
        sockId = -1;
    }

    return sockId;
}

int32_t util_net_write_msg(int32_t sock_fd, void *buff, int32_t size, int32_t fd[], uint32_t num)
{
    struct msghdr   msg;
    struct iovec    iov[1];
    char            buf[CMSG_SPACE(sizeof(int32_t)*UTIL_NET_MAX_NUM_FD)];
    int32_t         status = UTIL_NET_RET_SUCCESS;

    if (sock_fd < 0 || NULL == buff) {
        UTIL_NET_PRINT("param error \r\n");
        return -1;
    }

    iov[0].iov_base = buff;
    iov[0].iov_len  = size;
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov     = iov;
    msg.msg_iovlen  = 1;

    /* Check if any file descriptors need to be transferred, along with the
     * payload.
     */
    if (num)
    {
        struct cmsghdr *cmsg;
        int32_t        *fdPtr;

        msg.msg_control    = buf;
        msg.msg_controllen = CMSG_SPACE(sizeof(int32_t) * num);
        cmsg               = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_level   = SOL_SOCKET;
        cmsg->cmsg_type    = SCM_RIGHTS;
        cmsg->cmsg_len     = CMSG_LEN(sizeof(int32_t) * num);
        fdPtr              = (int32_t *)CMSG_DATA(cmsg);

        memcpy(fdPtr, fd, num * sizeof(int32_t));
    }
    else
    {
        msg.msg_control    = NULL;
        msg.msg_controllen = 0;
    }

    status = sendmsg(sock_fd, &msg, 0);

    if (status > 0)
    {
        status = UTIL_NET_RET_SUCCESS;
    }
    else
    {
        UTIL_NET_PRINT("sendmsg() failed\n");
        perror(__FUNCTION__);
    }

    return status;
}

int32_t util_net_read_msg(int32_t sock_fd, void *buff, uint32_t size, int32_t fd[], uint32_t *num)
{
    struct msghdr   msg;
    char            buf[CMSG_SPACE(sizeof(int32_t)*UTIL_NET_MAX_NUM_FD)];
    struct iovec    iov[1];
    int32_t         status;

    if (sock_fd < 0 || NULL == buff) {
        UTIL_NET_PRINT("param error \r\n");
        return -1;
    }
    
    msg.msg_name    = NULL;
    msg.msg_namelen = 0;

    iov[0].iov_base = buff;
    iov[0].iov_len  = size;

    msg.msg_iov    = iov;
    msg.msg_iovlen = 1;

    if (num)
    {
        msg.msg_control    = buf;
        msg.msg_controllen = CMSG_LEN(sizeof(buf));
    }
    else
    {
        msg.msg_control    = NULL;
        msg.msg_controllen = 0;
    }

    status = recvmsg(sock_fd, &msg, 0);

    if (status > 0)
    {
        /* Check if any file descriptors were transferred, along with the
         * payload.
         */
        if (num)
        {
            struct cmsghdr *cmsg;
            int32_t        *fdPtr;

            *num = 0;

            cmsg = CMSG_FIRSTHDR(&msg);

            if (cmsg != NULL)
            {
                int32_t     len;
                uint32_t    i;

                /* We do not know exactly how many descriptors are being sent.
                 * We can derive that from the length of the command buffer,
                 * taking care of any padding.
                 */
                /* Get the true length of the payload. */
                len = cmsg->cmsg_len -
                      CMSG_LEN(sizeof(int32_t)) +
                      sizeof(uint32_t);

                /* payloadLen/sizeof(int32_t) should give us the number of
                 * descriptors sent.
                 */
                *num = len/sizeof(int32_t);

                /* Extract the descriptor Ids. */
                fdPtr = (int32_t *)CMSG_DATA(cmsg);

                for (i = 0; i < *num; i++)
                {
                    fd[i] = fdPtr[i];
                }
            }
        }

        status = UTIL_NET_RET_SUCCESS;
    }
    else if (status == 0)
    {
        /* The peer has closed the connection. */
        status = UTIL_NET_SOCK_RCV_ERROR;
        UTIL_NET_PRINT("Peer closed the connection.\n");
    }
    else
    {
        status = UTIL_NET_SOCK_RCV_ERROR;
        UTIL_NET_PRINT("recvmsg() failed\n");
        perror(__FUNCTION__);
    }

    return status;
}

int32_t util_net_write_data(int32_t sock_fd, void *buff, int32_t size)
{
	int nWrittenBytes = 0;
	int nLeftBytes = 0;
	char *ptr = buff;

	if (sock_fd < 0 || buff == NULL)
	{
		UTIL_NET_PRINT("param error \r\n");
		return -1;
	}

	nLeftBytes = size;

	while(nLeftBytes > 0)
	{
		nWrittenBytes = send(sock_fd, buff, size, 0);
		if (nWrittenBytes <= 0)
		{
            UTIL_NET_PRINT("Send unKown socket %d error \r\n", nWrittenBytes);
            perror(__FUNCTION__);
            return -1;
		}
		nLeftBytes = nLeftBytes - nWrittenBytes;
		ptr+= nWrittenBytes;
	}

	return 0;
}

int32_t util_net_read_data(int32_t sock_fd, void *buff, uint32_t size)
{
    int32_t status;
    
	if (sock_fd < 0 || buff == NULL)
	{
		UTIL_NET_PRINT("param error \r\n");
		return -1;
	}
    
    status = recv(sock_fd, buff, size, 0);

    if (status == 0) {
        UTIL_NET_PRINT("Peer closed the connection.\n");
    }
    else {
        UTIL_NET_PRINT("recv() failed\n");
        perror(__FUNCTION__);
    }

    return status;
}
