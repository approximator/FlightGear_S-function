/*!
 * @file FgsTransport.h
 *
 * @brief Transport for  FlightGear <--> s-function
 * Matlab s-function implementation for interacting with FlightGear's FDM.
 *
 * @author Oleksii Aliakin (alex@nls.la)
 * @date Created Nov 14, 2014
 * @date Modified Dec 22, 2014
 */

#ifndef _FGSTRANSPORT_H_
#define _FGSTRANSPORT_H_

#ifdef __linux__
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
#else
    #define _WIN32_WINNT 0x0501
    #include <winsock2.h>
#endif

/*!
 * @brief FGS address context
 */
typedef struct 
{
    int sock;                /**< Socket descriptor */
    int port;                /**< Endpoint port     */
    struct sockaddr_in addr; /**< Addr              */
} FgsAddrContext;

/*!
 * @brief FGS transport context
 *
 * FgsTransportContext represents two endpoints:
 * - the \b S-Function endpoint - socket to read the FDM data from,
 * - the \b FlightGear endpoint - socket to send control data to.
 */
typedef struct
{
    FgsAddrContext flightGear; /**< FlightGear endpoint (write data to)  */
    FgsAddrContext server;     /**< S-Function endpoint (read data from) */
} FgsTransportContext;


/*!
 * @brief Create FGS transport context and initialize sockets
 *
 * @param [in] transportServerIpAddr - IP address of S-Function server (we will read data from here)
 * @param [in] serverPort - port of S-Function server
 * @param [in] flightgearIpAddr - IP address of FightGear instance (write data here)
 * @param [in] flightGearPort - port of FightGear instance
 *
 * @return FgsTransportContext structure if success, NULL otherwise
 *
 * @warning parameter transportServerIpAddr don't used yet, see corresponding todo
 * @todo Rename parameters to make them more intuitively understandable
 */
FgsTransportContext* fgsCreateTransportContext(const char* transportServerIpAddr, int serverPort,
                                               const char* flightgearIpAddr, int flightGearPort);

/*!
 * @brief Start fgsTransportStart (create sockets and start server)
 *
 * @param [in/out] transport - FgsTransportContext structure created with fgsCreateTransportContext
 * @return 0 if success, -1 otherwise
 * @return initialized sockets in transport context
 *
 * @see fgsCreateTransportContext
 */
int fgsTransportStart(FgsTransportContext* transport);

/*!
 * @brief Check is it any data to read from FlightGear instance
 *
 * @param [in] transport - valid FgsTransportContext structure
 * @param [in] timeOut - timeout to wait for data (sould be very small to not to slow down simulink)
 *
 * @return not 0 if it is any data to read, 0 otherwise
 *
 * @see FgsTransportContext
 * @see POSIX select
 */
int fgsTransportIsDataToRead(FgsTransportContext* transport, int timeOut);

/*!
 * @brief Read FDM data from FlightGear instance
 *
 * @param [in] transport - valid FgsTransportContext structure
 * @param [out] buf - buffer to read data to
 * @param [in] count - size of the buffer
 *
 * @return bytes read
 */
int fgsTransportReadData(FgsTransportContext* transport, void *buf, size_t count);

/*!
 * @brief Write control data to FlightGear instance
 *
 * @param [in] transport - valid FgsTransportContext structure
 * @param [in] buf - data to write
 * @param [in] len - size of data
 *
 * @return bytes written
 */
int fgsTransportWriteData(FgsTransportContext* transport, const void *buf, size_t len);

/*!
 * @brief Free all transport resources
 *
 * @param [in] transport - valid FgsTransportContext structure
 *
 * @return 0 if success
 */
int fgsTransportDestroy(FgsTransportContext* transport);

#endif /* _FGSTRANSPORT_H_ */
