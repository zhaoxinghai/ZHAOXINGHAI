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
#include <sys/wait.h>
#include <arpa/inet.h>
#include "cpsw_vlan.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif /* __cplusplus */
#endif  /* __cplusplus */


#define VLAN_NONE "NONE"
static int get_ip_netmask(char* ipaddr,char* netmask,const char *if_name);
static int set_ipaddr(char *ipaddr,const char *if_name);
static int cpsw_vlan_add(const char * if_name, unsigned int vid,unsigned short priorty );
static int cpsw_vlan_del(const char * if_name );
static int get_ethernet_flags(const char *if_name);
static void *ethernet_moniter(void *param);
static int check_bonding_device(void);
static int vlan_bind_to_interface(const char *vlan_if_name);
static void vlan_set_ip_route_multpath_table(const char *ip, const char *if_name);
static void vlan_set_ip_route_default(const char *if_name);
static int set_ethernet_inactive(const char *if_name);
static int set_ethernet_active(const char *if_name);
static void update_ethernet_status(void);
static int set_netmask(char *netmask,const char *if_name);
static int cpsw_vlan_set_flag(const char * if_name, int flag,short qos );
static int open_sys_vlan(void);




static char host_ip_addr[30],host_netmask[30];
static char interface_name[30];
static unsigned int tmp_vid = 0;
static int cpsw_init_flag = 0;

//Only cpsw_set_flag > 0, ethernet_cb() will be called
static int cpsw_set_flag = 0;

static int monitor_thread_run = 0;
static int current_ethernet = 0;
static int last_ethernet = 0;
static int new_fd = 0;
static  FILE *proc_fp;

static char *save_eth0_ip,*save_eth1_ip,*save_vlan_netmask;
pthread_t pid_ethernet_monitor;

static int ip_table_num = 100;
static char vlan_eth0_name[IFNAMSIZ];
static char vlan_eth1_name[IFNAMSIZ];


ethernet_moniter_callback ethernet_cb;


int cpsw_vlan_config_init(const char *eth0_vlan_ip, const char *eth1_vlan_ip,
                                 const char *vlan_netmask,ethernet_moniter_callback cb)
{
    int ret;

    if(cpsw_init_flag > 0)
        {
            fprintf(stderr,"cpsw_vlan_config_init() has been inited \n");
            return 0;
        }
    
    if((eth0_vlan_ip ==NULL) || (eth1_vlan_ip==NULL))
        {
            fprintf(stderr,"cpsw_vlan_config_init param can not be NULL!!!\n");
            return -1;
        }

    //This is stupid for code check.
    if((eth0_vlan_ip !=NULL) && (eth1_vlan_ip !=NULL))
    {
        save_eth0_ip = strdup(eth0_vlan_ip);
        save_eth1_ip = strdup(eth1_vlan_ip);
        save_vlan_netmask = strdup(vlan_netmask);
    }
    ethernet_cb = cb;

    if(open_sys_vlan() != 0)
        return -1;

    last_ethernet = check_bonding_device();
    current_ethernet = last_ethernet;
    update_ethernet_status();

    monitor_thread_run = 1;
    pthread_create(&pid_ethernet_monitor,NULL,ethernet_moniter,NULL);

    cpsw_init_flag = 1;

    return 0;

}


int cpsw_vlan_config_deinit(void)
{
    if(cpsw_init_flag != 1)
        {
            fprintf(stderr,"%s:cpsw_vlan_config_init() has not been inited yet. \n",__FUNCTION__);
            return 0;
        }

    cpsw_init_flag = 0;
    monitor_thread_run = 0;    
    
    pthread_join(pid_ethernet_monitor,NULL);
    if(cpsw_set_flag > 0)
        {
            cpsw_vlan_del(vlan_eth0_name);
            cpsw_vlan_del(vlan_eth1_name);
        }

    if(save_eth0_ip != NULL)
    {
        free(save_eth0_ip);
        save_eth0_ip = NULL;
    }

    if(save_eth1_ip != NULL)
    {
        free(save_eth1_ip);
        save_eth1_ip = NULL;
    }

    if(save_vlan_netmask != NULL)
    {
        free(save_vlan_netmask);
        save_vlan_netmask = NULL;
    }

    fclose(proc_fp);    
    proc_fp = NULL;
    return 0;
}

                                    
static void update_ethernet_status(void)
{
    if(current_ethernet == ETH0_TYPE)
        {
            //set_ethernet_active("eth0");
            //set_ethernet_inactive("eth1");
            if(cpsw_set_flag > 0 )
                {
                    set_ethernet_inactive(vlan_eth1_name);
                    set_ethernet_active(vlan_eth0_name);
                }
        }
    else if(current_ethernet == ETH1_TYPE)
        {
            //set_ethernet_inactive("eth0");
            //set_ethernet_active("eth1");
            if(cpsw_set_flag > 0)
                {
                    set_ethernet_inactive(vlan_eth0_name);
                    set_ethernet_active(vlan_eth1_name);
                }
        }
    else
        {
            set_ethernet_active("eth0");
            set_ethernet_active("eth1");
        }
    fprintf(stderr,"update_ethernet_status:current_ethernet:%d \n",current_ethernet);

}
                                    
static void *ethernet_moniter(void *param)
{   char *vlan_if;
    
    while(monitor_thread_run)
        {
            current_ethernet = check_bonding_device();
            
            if(current_ethernet == NONE_TYPE)
                update_ethernet_status();
            
            //fprintf(stderr,"current_ethernet:%d last_ethernet=%d \n",current_ethernet,last_ethernet);
            
            if(current_ethernet != last_ethernet)
                {
                    update_ethernet_status();
                    last_ethernet = current_ethernet;

                    if(cpsw_set_flag)
                        {
                            if(last_ethernet == ETH0_TYPE)
                                {
                                    new_fd = vlan_bind_to_interface(vlan_eth0_name);
                                    vlan_if = vlan_eth0_name;
                                }
                            
                            else if (last_ethernet == ETH1_TYPE)
                                {
                                    new_fd= vlan_bind_to_interface(vlan_eth1_name);
                                    vlan_if = vlan_eth1_name;
                                }
                            
                            else
                                {
                                    new_fd =  -1;

                                    vlan_if = VLAN_NONE;
                                }

                            if(ethernet_cb != NULL)
                                (*ethernet_cb)(&new_fd,vlan_if);
                            
                            fprintf(stderr,"Ethernet has changed new soket:%d \n",new_fd);
                        }
                }
            
            sleep(1);
        } 
    return NULL;

}

static int open_sys_vlan(void)
{
    proc_fp = fopen(OPEN_BONDING_PORC_NAME,"r");
    if(proc_fp == NULL )
        {
            fprintf(stderr,"Failed to open proc file:%s \n",OPEN_BONDING_PORC_NAME);
            return -1;
        }
    	
    return 0;
}


static int check_bonding_device(void)
{
    int ethernet_type = NONE_TYPE;
    
    if (proc_fp == NULL)
        return ethernet_type;

    char buf[100];    
 
    memset(buf, 0, 100);
    fseek(proc_fp, 0, SEEK_SET);
    size_t iRead = fread(buf, sizeof(char), 100, proc_fp);
    if (iRead <= 0)
    {
        fprintf(stderr,"Failed to read  file:%s \n",OPEN_BONDING_PORC_NAME);
        fclose(proc_fp);
        proc_fp = NULL;
        open_sys_vlan();
        return  last_ethernet;
    }
    
    if (strstr(buf, "eth0") != NULL)
        {
            ethernet_type = ETH0_TYPE;
        }
    
    else if (strstr(buf, "eth1") != NULL)
        {
            ethernet_type = ETH1_TYPE;
        }
    
    else 
        {
            ethernet_type = NONE_TYPE;
        }
    
    fprintf(stderr,"Currently Active Slave: %d \n",ethernet_type);
    
    return ethernet_type;
}


int set_vlan_interface_stop(void)
{
    int ret = 0;
    int cur_if;
    
    if(cpsw_init_flag != 1)
        {
            fprintf(stderr,"cpsw_vlan_config_init() shoud be inited first. \n");
            return -1;
        }
        
    if(cpsw_set_flag)
    {
        cur_if = check_bonding_device();
        if(cur_if == ETH0_TYPE)
            {
                ret = set_ethernet_inactive(vlan_eth0_name);        
            }
        
        else if (cur_if == ETH1_TYPE)
            {
                ret = set_ethernet_inactive(vlan_eth1_name);
            }
    }

    return (ret);
}


int set_vlan_interface_restart(void)
{
    int ret = 0;
    int cur_if;
    
    if(cpsw_init_flag != 1)
        {
            fprintf(stderr,"cpsw_vlan_config_init() shoud be inited first. \n");
            return -1;
        }
        
    if(cpsw_set_flag)
    {
        cur_if = check_bonding_device();
        if(cur_if == ETH0_TYPE)
            {
                ret = set_ethernet_active(vlan_eth0_name);      
            }
        
        else if (cur_if == ETH1_TYPE)
            {
                ret = set_ethernet_active(vlan_eth1_name);
            }
    }

    return (ret);
}

char *get_current_vlan_interface(void)
{
    int cur_if;
    
    if(cpsw_init_flag != 1)
        {
            fprintf(stderr,"%s: cpsw_vlan_config_init() shoud be inited first. \n",__FUNCTION__);
            return NULL;
        }
        
    if(cpsw_set_flag)
    {
        cur_if = check_bonding_device();
        if(cur_if == ETH0_TYPE)
            {
                fprintf(stderr,"%s: %s vlan is active. \n",__FUNCTION__,vlan_eth0_name);
                return vlan_eth0_name;      
            }
        
        else if (cur_if == ETH1_TYPE)
            {
                fprintf(stderr,"%s: %s vlan is active. \n",__FUNCTION__,vlan_eth1_name);
                return vlan_eth1_name;
            }

        else 
            fprintf(stderr,"%s: None of vlans is active. \n",__FUNCTION__);
            return "NONE";
    }

    return NULL;
}


// 0       6      12      12/16 14/18           18/22
// +-------+-------+---------+----+---------------+
// | DMAC  | SMAC  |8100 VLAN|Type|Payload (4Bfix)|
// +-------+-------+---------+----+---------------+

int cpsw_vlan_set( unsigned int vid,unsigned short priorty )
{
    int fd;
    int ret = -1;
    int cur_if;
    struct sockaddr_in ser_addr;    

    if(cpsw_init_flag != 1)
        {
            fprintf(stderr,"cpsw_vlan_config_init() shoud be inited first. \n");
            return ret;
        }
        
    //host  ethernet interface  is :bond1
    ret = get_ip_netmask(host_ip_addr, host_netmask, TI_EVM_BOARD_ETHERNET_NAME);
    if(ret < 0)
    {
        fprintf(stderr,"Get ipaddr  fail!\n");
        return ret;
    }
    printf("Get host ethernet %s ip:%s netmask:%s \n",TI_EVM_BOARD_ETHERNET_NAME,host_ip_addr,host_netmask);


    //add vid vlan to eht0 and eht1
    ret = cpsw_vlan_add("eth0",vid, priorty);
    if(ret < 0)
    {
        fprintf(stderr,"CPSW vlan add  fail!\n");
        return ret;
    }    

    snprintf(vlan_eth0_name,sizeof(vlan_eth0_name),"%s.%d","eth0",vid);
    set_ipaddr(save_eth0_ip,vlan_eth0_name);
    set_netmask(save_vlan_netmask, vlan_eth0_name);

    ret = cpsw_vlan_add("eth1",vid, priorty);
    if(ret < 0)
    {
        fprintf(stderr,"CPSW vlan add  fail!\n");
        return ret;
    }

    snprintf(vlan_eth1_name,sizeof(vlan_eth1_name),"%s.%d","eth1",vid);
    set_ipaddr(save_eth1_ip,vlan_eth1_name);
    set_netmask(save_vlan_netmask, vlan_eth1_name);

   // cpsw_vlan_set_flag(vlan_eth0_name,1,0);
   // cpsw_vlan_set_flag(vlan_eth1_name,1,0);

    //set ip route multpath table
    vlan_set_ip_route_multpath_table(host_ip_addr,TI_EVM_BOARD_ETHERNET_NAME);
    
    vlan_set_ip_route_multpath_table(save_eth0_ip,vlan_eth0_name);
    
    vlan_set_ip_route_multpath_table(save_eth1_ip,vlan_eth1_name);

    vlan_set_ip_route_default(TI_EVM_BOARD_ETHERNET_NAME);  
    
    cur_if = check_bonding_device();

    //set vlan interface ip and running
    if(cur_if == ETH0_TYPE)
        {
            set_ethernet_active(vlan_eth0_name);        
            fd = vlan_bind_to_interface(vlan_eth0_name);
        }
    
    else if (cur_if == ETH1_TYPE)
        {
            set_ethernet_active(vlan_eth1_name);
            fd = vlan_bind_to_interface(vlan_eth1_name);
        }
    
    else
        fd =  ret;

    cpsw_set_flag = 1;
    tmp_vid = vid;

    return fd;  

}

static int run_command(const char *com, char *com_param[])
{	
	pid_t pid, child_pid;
	int ret;

	pid = fork();
	
	if(pid < 0 )
		{
			perror("fork() \n");
			return -1;
		}
	
	else if (pid == 0)
	 {	//child process 		 		
	 	if (execvp(com,com_param) < 0 )
	 	  { 		
	 	    perror("error on exec");			
	 	   // exit(-1);		
	 	    //return -1;
	 	  }	

	 	 else 
	 	 	return 2;
	 }
	 
	//parent process		
	do {
			child_pid = waitpid(pid,&ret,0);
			
			
		}while(child_pid == 0);
			
		return ret;
}


static void vlan_set_ip_route_default(const char *if_name)
{
	char ip_table_num_str[10];
	char *command= "ip";
	char *objet2 = "route";
	
	char *command_param2[12];
	command_param2[0] = command;
	command_param2[1] = objet2;
	command_param2[2] = "add";
	command_param2[3] = "default";
	command_param2[4] = "scope";
	command_param2[5] = "global";
	command_param2[6] = "nexthop";	
	command_param2[7] = "via";
	command_param2[8] = "192.168.1.1";
	command_param2[9] = "dev";
	command_param2[10] = if_name;
	command_param2[11] = NULL;
	run_command(command,command_param2);
}

static void vlan_set_ip_route_multpath_table(const char *ip, const char *if_name)
{
	char ip_table_num_str[10];
	char *command= "ip";
	char *objet1 = "rule";
	char *objet2 = "route";


	snprintf(ip_table_num_str,10,"%d",ip_table_num);

	char *command_param1[8];
	command_param1[0] = command;
	command_param1[1] = objet1;
	command_param1[2] = "add";
	command_param1[3] = "from";	
	command_param1[4] = ip;
	command_param1[5] = "table";
	command_param1[6] = ip_table_num_str;
	command_param1[7] = NULL;	
	run_command(command,command_param1);

	char *command_param2[11];
	command_param2[0] = command;
	command_param2[1] = objet2;
	command_param2[2] = "add";
	command_param2[3] = "192.168.1.0/24";
	command_param2[4] = "dev";
	command_param2[5] = if_name;
	command_param2[6] = "scope";	
	command_param2[7] = "link";
	command_param2[8] = "table";
	command_param2[9] = ip_table_num_str;
	command_param2[10] = NULL;
	run_command(command,command_param2);

	char *command_param3[11];
	command_param3[0] = command;
	command_param3[1] = objet2;
	command_param3[2] = "add";
	command_param3[3] = "default";
	command_param3[4] = "via";
	command_param3[5] = "192.168.1.1";
	command_param3[6] = "dev";	
	command_param3[7] = if_name;
	command_param3[8] = "table";
	command_param3[9] = ip_table_num_str;
	command_param3[10] = NULL;
	run_command(command,command_param3);

	ip_table_num ++;
}



static int vlan_bind_to_interface(const char *vlan_if_name)
{
    int fd;
    //bind the socket to vlan ethernet interface
    const char *opt = vlan_if_name;
    const int len = strnlen(opt,IFNAMSIZ);

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0)
    {
        fprintf(stderr,"Create socket fail!\n");
        return -1;
    }
    
    if(len == IFNAMSIZ)
        {
            fprintf(stderr,"Too long iface name");
            return -1;
        }

    if(setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, opt, len) <0)
        {
            fprintf(stderr,"Can not bind to device:%s\n",vlan_if_name);
            return -1;
        }

    return fd;
}

static int cpsw_vlan_set_flag(const char * if_name, int flag,short qos )
{
    int fd;
    struct vlan_ioctl_args if_request;
    char vlan_if_name[IFNAMSIZ];

    memset(&if_request, 0 , sizeof(struct vlan_ioctl_args));
    
    if((flag <0)||(flag>1)||(qos >7)||(qos <0))
        {
            fprintf(stderr,"cpsw_vlan_set_flag: error invalid param!\n");
            return -1;
        }
    
    if(if_name == NULL)
        {
            fprintf(stderr,"If_name can not be empty.\n");
            return -1;
        }

    const int len = strnlen(if_name,IFNAMSIZ);
    if(len == IFNAMSIZ)
        {
            fprintf(stderr,"Too long iface name\n");
            return -1;
        }
    
    if_request.u.flag = flag;
    if_request.vlan_qos = qos;
    strncpy(if_request.device1, if_name,IFNAMSIZ);
    
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        {
          fprintf(stderr, "FATAL:  Couldn't open a socket..!\n");
          return -1;
        }   

    if_request.cmd = SET_VLAN_FLAG_CMD;
    if (ioctl(fd, SIOCSIFVLAN, &if_request) < 0) 
        {
         fprintf(stderr,"ERROR: trying to set_flag %s  error: %s\n",if_name, strerror(errno));
         close(fd);
         return -1;
        }

    else 
        {
            fprintf(stdout,"Set %s: flag == %d to IF :%d\n", if_name,flag,qos);
     }

    close(fd);
    return 0;
    
}


static int cpsw_vlan_add(const char * if_name, unsigned int vid,unsigned short priorty )
{
    int fd;
    struct vlan_ioctl_args if_request;
    char vlan_if_name[IFNAMSIZ];

    memset(&if_request, 0 , sizeof(struct vlan_ioctl_args));
    
    if( (vid <= 2)||(vid >= 4095) )
        {
            fprintf(stderr,"Invalid vid !\n");
            return -1;
        }

    if(if_name == NULL)
        {
            fprintf(stderr,"If_name can not be empty.\n");
            return -1;
        }

    const int len = strnlen(if_name,IFNAMSIZ);
    if(len == IFNAMSIZ)
        {
            fprintf(stderr,"Too long iface name\n");
            return -1;
        }
    
    if_request.u.VID = vid;
    if_request.vlan_qos = priorty;
    strncpy(if_request.device1, if_name,IFNAMSIZ);
    
    strncpy(interface_name, if_name,IFNAMSIZ);
    tmp_vid = vid;

    //The vid vlan existed,del
    snprintf(vlan_if_name,sizeof(vlan_if_name),"%s.%d",interface_name,tmp_vid);
    //if(get_ethernet_flags(vlan_if_name) == 0)
        {
            //cpsw_vlan_del(vlan_if_name, vid);
        }

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        {
          fprintf(stderr, "FATAL:  Couldn't open a socket..!\n");
          return -1;
        }   

    if_request.cmd = ADD_VLAN_CMD;
    if (ioctl(fd, SIOCSIFVLAN, &if_request) < 0) 
        {
         fprintf(stderr,"ERROR: trying to add VLAN #%u to IF :%s:  error: %s\n",
                    vid, if_name, strerror(errno));
         close(fd);
         return -1;
        }

    else 
        {
         fprintf(stdout,"Added VLAN with VID == %u to IF :%s\n",vid, if_name);
     }

    close(fd);
    return 0;
    
}

static int cpsw_vlan_del(const char * if_name )
{
    int fd;
    struct vlan_ioctl_args if_request;

    memset(&if_request, 0 , sizeof(struct vlan_ioctl_args));

    if(if_name == NULL)
        {
            fprintf(stderr,"If_name can not be empty.\n");
            return -1;
        }

    strncpy(if_request.device1, if_name,IFNAMSIZ);

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
        {
          fprintf(stderr, "FATAL:  Couldn't open a socket..\n");
          return -1;
        }   

    if_request.cmd = DEL_VLAN_CMD;
    if (ioctl(fd, SIOCSIFVLAN, &if_request) < 0) 
        {
             fprintf(stderr,"ERROR: trying to remove VLAN :%s: error: %s\n",
                     if_name, strerror(errno));
             close(fd);
             return -1;
        }

    else 
        {
            fprintf(stdout,"Removed VLAN :%s\n", if_name);
        }

    close(fd);
    return 0;
    
}



static int get_ip_netmask(char* ipaddr,char* netmask,const char *if_name)
{   
    int fd;
    struct ifreq ifr;
    
    if( (ipaddr!=NULL) && (netmask!=NULL) && (if_name!=NULL))
        {
            if((fd = socket(AF_INET,SOCK_STREAM,0)) <0)
                {
                    fprintf(stderr, "FATAL:  Couldn't open a socket..for get ipaddr!\n");
                    return -1;                  
                }

            memset(&ifr,0,sizeof(ifr));
            ifr.ifr_addr.sa_family = AF_INET;
            strncpy(ifr.ifr_name, if_name, IFNAMSIZ);
            
            if(ioctl(fd,SIOCGIFADDR, &ifr) < 0)
                {
                    fprintf(stderr,"ERROR: trying to get %s IP addr -  error: %s\n",
                         if_name, strerror(errno));
                    close(fd);
                    return -1;
                }

            snprintf(ipaddr,50,"%s",inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

            if(ioctl(fd,SIOCGIFNETMASK, &ifr) < 0)
                {
                    fprintf(stderr,"ERROR: trying to get %s IP netmask -  error: %s\n",
                         if_name, strerror(errno));
                    
                    close(fd);
                    return -1;
                }

            snprintf(netmask,50,"%s",inet_ntoa(((struct sockaddr_in *)&ifr.ifr_netmask)->sin_addr));

            close(fd);

            return 0;
            
        }
    
    else
        return -1;
}


static int set_netmask(char *netmask,const char *if_name)
{
    struct ifreq ifr;
    struct sockaddr_in *s_addr=(struct sockaddr_in*)&ifr.ifr_addr;
    int fd;

    if((netmask != NULL) && (if_name !=NULL))
        {
            if((fd = socket(AF_INET,SOCK_STREAM,0)) <0)
                {
                    fprintf(stderr, "FATAL:  Couldn't open a socket..for set ipaddr!\n");
                    return -1;                  
                }

            memset(&ifr,0,sizeof(ifr));
            ifr.ifr_addr.sa_family = AF_INET;
            strncpy(ifr.ifr_name, if_name, IFNAMSIZ);

            inet_pton(AF_INET,netmask,&s_addr->sin_addr);           
            if(ioctl(fd,SIOCSIFNETMASK, &ifr) < 0)
                {
                    fprintf(stderr,"ERROR: trying to set %s netmask -  error: %s\n",
                         if_name, strerror(errno));
                    close(fd);
                    return -1;
                }           

            close(fd);

            return 0;
            
        }

    else
        return -1;  
    
}


static int get_ethernet_flags(const char *if_name)
{
    struct ifreq ifr;
    int fd;

    if(if_name != NULL)
        {
            if((fd = socket(AF_INET,SOCK_STREAM,0)) <0)
                {
                    fprintf(stderr, "FATAL:  Couldn't open a socket..for set ipaddr!\n");
                    return -1;                  
                }

            memset(&ifr,0,sizeof(ifr));
            ifr.ifr_addr.sa_family = AF_INET;
            strncpy(ifr.ifr_name, if_name, IFNAMSIZ);

            if(ioctl(fd,SIOCGIFFLAGS, &ifr) < 0)
                {
                    fprintf(stderr,"ERROR: trying to get %s SIOCGIFFLAGS -  error: %s\n",
                         if_name, strerror(errno));
                    
                    close(fd);
                    return -1;
                }
            
            close(fd);
            return 0;
            
        }

    else
        return -1;  
    
}


static int set_ipaddr(char *ipaddr,const char *if_name)
{
    struct ifreq ifr;
    struct sockaddr_in s_addr;
    int fd;

    if((ipaddr != NULL) && (if_name !=NULL))
        {
            if((fd = socket(AF_INET,SOCK_STREAM,0)) <0)
                {
                    fprintf(stderr, "FATAL:  Couldn't open a socket..for set ipaddr!\n");
                    return -1;                  
                }

            memset(&ifr,0,sizeof(ifr));
            ifr.ifr_addr.sa_family = AF_INET;
            strncpy(ifr.ifr_name, if_name, IFNAMSIZ);

            memset(&s_addr, 0 ,sizeof(struct sockaddr_in));
            s_addr.sin_family = AF_INET;
            s_addr.sin_port = 0;
            s_addr.sin_addr.s_addr = inet_addr(ipaddr);

            memcpy(&ifr.ifr_ifru.ifru_addr,&s_addr,sizeof(struct sockaddr));
            
            if(ioctl(fd,SIOCSIFADDR, &ifr) < 0)
                {
                    fprintf(stderr,"ERROR: trying to set %s IP addr -  error: %s\n",
                         if_name, strerror(errno));
                    close(fd);
                    return -1;
                }           

            close(fd);

            return 0;
            
        }

    else
        return -1;  
    
}


static int set_ethernet_active(const char *if_name)
{
    struct ifreq ifr;
    struct sockaddr_in s_addr;
    int fd;

    if( if_name!= NULL)
        {
            if((fd = socket(AF_INET,SOCK_STREAM,0)) <0)
                {
                    fprintf(stderr, "FATAL:  Couldn't open a socket..for set ipaddr!\n");
                    return -1;                  
                }

            memset(&ifr,0,sizeof(ifr));
            ifr.ifr_addr.sa_family = AF_INET;
            strncpy(ifr.ifr_name, if_name, IFNAMSIZ);   

            if(ioctl(fd,SIOCGIFFLAGS, &ifr) < 0)
                {
                    fprintf(stderr,"ERROR: trying to get %s SIOCGIFFLAGS -  error: %s\n",
                         if_name, strerror(errno));
                    
                    close(fd);
                    return -1;
                }

            ifr.ifr_flags = (unsigned )ifr.ifr_flags | (unsigned )IFF_UP | (unsigned )IFF_RUNNING;         
            if(ioctl(fd,SIOCSIFFLAGS, &ifr) < 0)
                {
                    fprintf(stderr,"ERROR: trying to SET %s SIOCSIFFLAGS -  error: %s\n",
                         if_name, strerror(errno));
                    
                    close(fd);
                    return -1;
                }

            close(fd);

            return 0;
            
        }

    else
        return -1;  
    
}


static int set_ethernet_inactive(const char *if_name)
{
    struct ifreq ifr;
    struct sockaddr_in s_addr;
    int fd;

    
    if( if_name != NULL)
        {
            if((fd = socket(AF_INET,SOCK_STREAM,0)) <0)
                {
                    fprintf(stderr, "FATAL:  Couldn't open a socket..for set ipaddr!\n");
                    return -1;                  
                }

            memset(&ifr,0,sizeof(ifr));
            ifr.ifr_addr.sa_family = AF_INET;
            strncpy(ifr.ifr_name, if_name, IFNAMSIZ);       

            if(ioctl(fd,SIOCGIFFLAGS, &ifr) < 0)
                {
                    fprintf(stderr,"ERROR: trying to get %s SIOCGIFFLAGS -  error: %s\n",
                         if_name, strerror(errno));
                    
                    close(fd);
                    return -1;
                }

            ifr.ifr_flags = (unsigned)ifr.ifr_flags & ~((unsigned)IFF_UP);
            if(ioctl(fd,SIOCSIFFLAGS, &ifr) < 0)
                {
                    fprintf(stderr,"ERROR: trying to SET %s SIOCSIFFLAGS -  error: %s\n",
                         if_name, strerror(errno));
                    
                    close(fd);
                    return -1;
                }

            close(fd);

            return 0;
            
        }

    else
        return -1;  
    
}



#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif  /* __cplusplus */
#endif


