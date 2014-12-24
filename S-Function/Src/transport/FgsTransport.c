/*!
 * @file FgsTransport.c
 *
 * @brief Transport for  FlightGear <--> s-function
 * Matlab s-function implementation for interacting with FlightGear's FDM.
 *
 * @author Oleksii Aliakin (alex@nls.la)
 * @date Created Nov 14, 2014
 * @date Modified Dec 22, 2014
 */

#ifdef __linux__
#include <netdb.h>
#endif

#include "transport/FgsTransport.h"
#include "memory/OaMemory.h"
#include "log.h"

FgsTransportContext* fgsCreateTransportContext(const char* transportServerIpAddr, int serverPort,
                                               const char* flightgearIpAddr, int flightGearPort)
{
    FgsTransportContext* transport = NULL;

    FGS_TRACE("Creating transport...");

#ifndef __linux__
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(1,1), &wsa_data);
#endif

    transport = oa_malloc(sizeof(FgsTransportContext));
    if (transport == NULL)
    {
        FGS_TRACE("transport is NULL");
        return NULL;
    }

    transport->server.port = serverPort;
    transport->server.addr.sin_family = AF_INET;
    transport->server.addr.sin_port = htons(serverPort);

    /* if (inet_aton(SRV_IP, &si_other.sin_addr)==0)
           error("inet_aton:");

        struct hostent *host_out;
    */

    transport->server.addr.sin_addr.s_addr = INADDR_ANY; /*! @todo use transportServerIpAddr */
    memset(&(transport->server.addr.sin_zero), 0, 8);

    /* FlightGear */
    struct hostent *host_out;

    host_out = (struct hostent *) gethostbyname((char *)flightgearIpAddr);
    transport->flightGear.port = flightGearPort;
    transport->flightGear.addr.sin_family = AF_INET;
    transport->flightGear.addr.sin_port = htons(flightGearPort);
    transport->flightGear.addr.sin_addr = *((struct in_addr *)host_out->h_addr);
    memset(&(transport->flightGear.addr.sin_zero), 0, 8);

    return transport;
}

int fgsTransportStart(FgsTransportContext* transport)
{
    FGS_TRACE("Starting transport...");

    /* Create server socket and bind */
    FGS_TRACE("Creating transport server socket");
    if ((transport->server.sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        FGS_TRACE("Can't transport server create socket");
        return -1;
    }
    FGS_TRACE("Bind port %d", transport->server.port);
    if (bind(transport->server.sock, (struct sockaddr *)&transport->server.addr, sizeof(struct sockaddr)) == -1)
    {
        FGS_TRACE("ERROR Bind port %s", transport->server.port);
        return -1;
    }
    FGS_TRACE("UDPServer Waiting for client on port 5000");

    /* FlightGear socket */
    FGS_TRACE("Creating flightGear socket...");
    if ((transport->flightGear.sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        FGS_TRACE("ERROR flightGear socket");
        return -1;
    }

    return 0;
}

int fgsTransportIsDataToRead(FgsTransportContext* transport, int timeOut)
{
    /** @todo Check&optimize this */
    fd_set socketReadSet;
    FD_ZERO(&socketReadSet);
    FD_SET(transport->server.sock, &socketReadSet);
    struct timeval tv;
    if (timeOut)
    {
        tv.tv_sec  = timeOut / 1000;
        tv.tv_usec = (timeOut % 1000) * 1000;
    }
    else
    {
        tv.tv_sec  = 0;
        tv.tv_usec = 0;
    }

    if (select(transport->server.sock+1, &socketReadSet, 0, 0, &tv) == -1)
    {
        return 0;
    }

    return FD_ISSET(transport->server.sock, &socketReadSet) != 0;
}

int fgsTransportReadData(FgsTransportContext* transport, void *buf, size_t count)
{
    return recvfrom(transport->server.sock, buf, count, 0,
                    (struct sockaddr *)&transport->server.addr,
                    sizeof(struct sockaddr));
}

int fgsTransportWriteData(FgsTransportContext* transport, const void *buf, size_t len)
{
    return sendto(transport->flightGear.sock, buf, len, 0,
                  (struct sockaddr *)&transport->flightGear.addr,
                  sizeof(struct sockaddr));
}

int fgsTransportDestroy(FgsTransportContext* transport)
{
    /*! @todo: CHECK_PTR() */

#ifdef __linux__
    close(transport->flightGear.sock);
    close(transport->server.sock);
#else
    closesocket(transport->flightGear.sock);
    closesocket(transport->server.sock);
    WSACleanup();
#endif

    oa_free(transport);
    return 0;
}

