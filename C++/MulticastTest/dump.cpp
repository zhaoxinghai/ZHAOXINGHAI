/********************************************************
 * ���ܣ�����ץ������
 * ����: GCC-4.2.4 
 * ���ߣ�YSQ-NJUST,yushengqiangyu@163.com
 * ��ע�������������Ҫ����ѧϰ������������
 *******************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <errno.h>

/* ���ջ�������С */
#define RCV_BUF_SIZE     1024 * 5 

/* ���ջ����� */
static int g_iRecvBufSize = RCV_BUF_SIZE; 
static char g_acRecvBuf[RCV_BUF_SIZE] = {0};

/* ���������ӿ�,��Ҫ���ݾ�������޸� */
static const char *g_szIfName = "eth0";
 
/* ��̫��֡��װ��Э������ */
static const int      g_iEthProId[] = { ETHERTYPE_PUP, 
                                        ETHERTYPE_SPRITE, 
                                        ETHERTYPE_IP, 
                                        ETHERTYPE_ARP,
                                        ETHERTYPE_REVARP,
                                        ETHERTYPE_AT,
                                        ETHERTYPE_AARP,
                                        ETHERTYPE_VLAN,
                                        ETHERTYPE_IPX,
                                        ETHERTYPE_IPV6,
                                        ETHERTYPE_LOOPBACK
                                      };
static const char g_szProName[][24] = { "none", "xerox pup", "sprite", "ip", "arp", 
                                        "rarp", "apple-protocol", "apple-arp", 
                                        "802.1q", "ipx", "ipv6", "loopback" 
                                      };


/* ���MAC��ַ */
static void ethdump_showMac(const int iType, const char acHWAddr[]) 
{
    int i = 0;

    if (0 == iType)
    {
        printf("SMAC=[");
    }
    else
    {
        printf("DMAC=[");
    }

    for(i = 0; i < ETHER_ADDR_LEN - 1; i++)
    {
        printf("%02x:", *((unsigned char *)&(acHWAddr[i])));
    }
    printf("%02x] ", *((unsigned char *)&(acHWAddr[i])));
}

/* ������������ģʽ���Բ��� */
static int ethdump_setPromisc(const char *pcIfName, int fd, int iFlags)
{
    int iRet = -1;
    struct ifreq stIfr;

    /* ��ȡ�ӿ����Ա�־λ */
    strcpy(stIfr.ifr_name, pcIfName);
    iRet = ioctl(fd, SIOCGIFFLAGS, &stIfr);
    if (0 > iRet)
    {
        perror("[Error]Get Interface Flags");    
        return -1;
    }
    
    if (0 == iFlags)
    {
        /* ȡ������ģʽ */
        stIfr.ifr_flags &= ~IFF_PROMISC;
    }
    else
    {
        /* ����Ϊ����ģʽ */
        stIfr.ifr_flags |= IFF_PROMISC;
    }

    iRet = ioctl(fd, SIOCSIFFLAGS, &stIfr);
    if (0 > iRet)
    {
        perror("[Error]Set Interface Flags");
        return -1;
    }
    
    return 0;
}

/* ��ȡL2֡��װ��Э������ */
static char *ethdump_getProName(const int iProNum)
{
    int iIndex = 0; 
    
    for(iIndex = 0; iIndex < sizeof(g_iEthProId) / sizeof(g_iEthProId[0]); iIndex++)
    {
        if (iProNum == g_iEthProId[iIndex])
        {
            break;
        }
    }

    return (char *)(g_szProName[iIndex + 1]);
}

/* Init L2 Socket */
static int ethdump_initSocket()
{
    int iRet = -1;
    int fd = -1;
    struct ifreq stIf;
    struct sockaddr_ll stLocal = {0};
    
    /* ����SOCKET */
    fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (0 > fd)
    {
        perror("[Error]Initinate L2 raw socket");
        return -1;
    }
    
    /* ��������ģʽ���� */
    ethdump_setPromisc(g_szIfName, fd, 1);

    /* ����SOCKETѡ�� */
    iRet = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &g_iRecvBufSize,sizeof(int));
    if (0 > iRet)
    {
        perror("[Error]Set socket option");
        close(fd);
        return -1; 
    }
    
    /* ��ȡ���������ӿ����� */
    strcpy(stIf.ifr_name, g_szIfName);
    iRet = ioctl(fd, SIOCGIFINDEX, &stIf);
    if (0 > iRet)
    {
        perror("[Error]Ioctl operation");
        close(fd);
        return -1;
    }

    /* ���������� */
    stLocal.sll_family = PF_PACKET;
    stLocal.sll_ifindex = stIf.ifr_ifindex;
    stLocal.sll_protocol = htons(ETH_P_ALL);
    iRet = bind(fd, (struct sockaddr *)&stLocal, sizeof(stLocal)); 
    if (0 > iRet)
    {
        perror("[Error]Bind the interface");
        close(fd);
        return -1;
    }
    
    return fd;    
}

/* ����Ethernet֡�ײ� */
static int ethdump_parseEthHead(const struct ether_header *pstEthHead)
{
    unsigned short usEthPktType;

    if (NULL == pstEthHead)
    {
        return -1;
    }

    /* Э�����͡�ԴMAC��Ŀ��MAC */
    usEthPktType = ntohs(pstEthHead->ether_type);
    printf(">>> Eth-Pkt-Type:0x%04x(%s) ", usEthPktType, ethdump_getProName(usEthPktType));
    ethdump_showMac(0, (const char*)pstEthHead->ether_shost);
    ethdump_showMac(1, (const char*)pstEthHead->ether_dhost);
    
    return 0;    
}

/* ����IP���ݰ�ͷ */
static int ethdump_parseIpHead(const struct ip *pstIpHead)
{
    struct protoent *pstIpProto = NULL;

    if (NULL == pstIpHead)
    {
        return -1;
    }

    /* Э�����͡�ԴIP��ַ��Ŀ��IP��ַ */
    pstIpProto = getprotobynumber(pstIpHead->ip_p);
    if(NULL != pstIpProto)
    {
        printf(" IP-Pkt-Type:%d(%s) ", pstIpHead->ip_p, pstIpProto->p_name);
    }
    else
    {
        printf(" IP-Pkt-Type:%d(%s) ", pstIpHead->ip_p, "None");
    }
    printf("SAddr=[%s] ", inet_ntoa(pstIpHead->ip_src));
    printf("DAddr=[%s] ", inet_ntoa(pstIpHead->ip_dst));

    return 0;
}

/* ����֡�������� */
static int ethdump_parseFrame(const char *pcFrameData)
{
    int iRet = -1;

    struct ether_header *pstEthHead = NULL;
    struct ip *pstIpHead = NULL;

    /* Ethnet֡ͷ���� */
    pstEthHead = (struct ether_header*)g_acRecvBuf;
    iRet = ethdump_parseEthHead(pstEthHead);
    if (0 > iRet)
    {
        return iRet;
    }

    /* IP���ݰ����� */
    pstIpHead  = (struct ip *)(pstEthHead + 1);
    iRet = ethdump_parseIpHead(pstIpHead);
	printf("\n");
    return iRet;
}

/* ������������֡ */
static void ethdump_startCapture(const int fd)
{
    int iRet = -1;
    socklen_t stFromLen = 0;
    
    /* ѭ������ */
    while(1)
    {
        /* ��ս��ջ����� */
        memset(g_acRecvBuf, 0, RCV_BUF_SIZE);

        /* ��������֡ */
        iRet = recvfrom(fd, g_acRecvBuf, g_iRecvBufSize, 0, NULL, &stFromLen);
        if (0 > iRet)
        {
            continue;
        }
        
        /* ��������֡ */
        ethdump_parseFrame(g_acRecvBuf);
    }
}    

/* Main */
int main(int argc, char *argv[])
{
    int iRet = -1;
    int fd   = -1;
    
    /* ��ʼ��SOCKET */
    fd = ethdump_initSocket();
    if(0 > fd)
    {
        return -1;
    }
    
    /* �������ݰ� */
    ethdump_startCapture(fd);
    
    /* �ر�SOCKET */
    close(fd);

    return 0;
}
