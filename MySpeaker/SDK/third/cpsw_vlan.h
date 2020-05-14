/*****************************************************
Copyright (C), Honeywell
*******************************************************
File Name       :cpsw_vlan.h
Version         :Initial Draft
Author          :H361203
Created         :2019/5/20
Last Modified   :

Description     :ETCS project, base on ti am335x-evm board.
                 Send and receive udp packages with vlan tag.
                 Call the API steps:
                 1. cpsw_vlan_config_init();
                 2. cpsw_vlan_set();
                 3. cpsw_vlan_config_deinit().

********************************************************/

#ifndef _MSC_VER

#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <linux/if_vlan.h>
#include <linux/sockios.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h> 
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>
#include <arpa/inet.h>

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */

#define TI_EVM_BOARD_ETHERNET_NAME "bond1"
#define OPEN_BONDING_PORC_NAME "/sys/class/net/bond1/bonding/active_slave"


#define ETH0_TYPE 1
#define ETH1_TYPE 2
#define NONE_TYPE 3

/* Register ethernet monitor callback fuction */
typedef void  (*ethernet_moniter_callback)(int *new_vlan_socket,char *vlan_if);

/**
\brief ETCS project, vlan config init
\attention It should be invoked first before vlan confiure.
\param[in] eth0_vlan_ip: new vlan interface sush as eth0.5 ip address,its parent is eth0
           eth1_vlan_ip: new vlan interface sush as eth1.5 ip address,its parent is eth1
           vlan_netmask: vlan interface netmask
           ethernet_moniter_callback cb: due to eth0 and eth1 are bonding,works in (active-backup) 
           mode. When the network changs, cb will be invoked, and new vlan socket_fd and vlan interface's 
           name will be returned.
           
\retval 0:success, -1:fail.
**/
int cpsw_vlan_config_init(const char *eth0_vlan_ip, const char *eth1_vlan_ip,
                                 const char *vlan_netmask, ethernet_moniter_callback cb);

/**
\brief vlan properties configure
// 0       6      12      12/16 14/18           18/22
// +-------+-------+---------+----+---------------+
// | DMAC  | SMAC  |8100 VLAN|Type|Payload (4Bfix)|
// +-------+-------+---------+----+---------------+
\attention \n
None
\param[in] vid: 3-4095
           priorty:refer to vlan tags 
   
\retval -1:fail. >0: udp vlan socket_fd.  send or receive packags via the socket_fd.
**/
int cpsw_vlan_set(unsigned int vid,unsigned short priorty );

/**
\brief vlan interface restart
\attention \n
None
\param[in] none
   
\retval 0:success, -1:fail.
**/
int set_vlan_interface_restart(void);

/**
\brief vlan interface stop
\attention \n
None
\param[in] none
   
\retval 0:success, -1:fail.
**/
int set_vlan_interface_stop(void);

/**
\brief vlan interface deinit
\attention \n
None
\param[in] none
   
\retval 0:success, -1:fail.
**/
int cpsw_vlan_config_deinit(void);

/**
\brief get current vlan interface. 
\attention \n
None
\param[in] none
   
\retval     NONE:No vlan network works, NULL:fail.
**/
char *get_current_vlan_interface(void);


/**
\brief After cpsw_vlan_config_init() and cpsw_vlan_set() are called,
        you want to capture packages with vlan tag via pcap, or such like
        "tcpdump -i eth1.3" command. cpsw_vlan_set_flag() must be involed first,
        always the param flag set to 1 and qos set to 0.
\attention \n
None
\param[in] if_name: the vlan interface name, can get from get_current_vlan_interface()
           flag: 0 or 1
           qos: 0 to 7
   
\retval     0:success, -1:fail.
**/

//int cpsw_vlan_set_flag(const char * if_name, int flag,short qos );




#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif  /* __cplusplus */

#endif

