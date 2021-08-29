/* $Id: socket.c 1.1 1995/01/01 07:11:14 cthuang Exp $
 *
 * This module has been modified by Radim Kolar for OS/2 emx
 */

/***********************************************************************
  module:       socket.c
  program:      popclient
  SCCS ID:      @(#)socket.c    1.5  4/1/94
  programmer:   Virginia Tech Computing Center
  compiler:     DEC RISC C compiler (Ultrix 4.1)
  environment:  DEC Ultrix 4.3 
  description:  UNIX sockets code.
 ***********************************************************************/
#define	h_addr	h_addr_list[0] /* Address, for backward compatibility.*/

#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int fSocket(const char *host, int clientPort)
{
    int sock;
    unsigned long inaddr;//��������ʽ��ַ
    struct sockaddr_in ad;//��ŵ�ַ
    struct hostent *hp;
    
    memset(&ad, 0, sizeof(ad)); //�����ڴ�
    ad.sin_family = AF_INET;  //IPv4

    inaddr = inet_addr(host);//����ַ�ַ���ת��������ʽ�ĵ�ַ
    if (inaddr != INADDR_NONE)//INADDR_NONE����,�������Ƿ�IP�ַ���255 IP,������ָñ���
        memcpy(&ad.sin_addr, &inaddr, sizeof(inaddr));
    else
    {
        hp = gethostbyname(host);//���յ�exceptionΪINADDR_NONE��IP,���Խ�����IP
        if (hp == NULL)//�޷�����IP,֤��IP����
            return -1;
        memcpy(&ad.sin_addr, hp->h_addr, hp->h_length);//������DNS����IP,IP��ȷ
    }
    ad.sin_port = htons(clientPort);//��int��ת��Ϊ���紫���ֽ���
    
    sock = socket(AF_INET, SOCK_STREAM, 0);//����IPv4,����ʽ,TCP��������,��ȡ����FD
    if (sock < 0)
        return sock;//���ش������Ӻ�
    if (connect(sock, (struct sockaddr *)&ad, sizeof(ad)) < 0)//double check error exist
        return -1;
    return sock;
}

