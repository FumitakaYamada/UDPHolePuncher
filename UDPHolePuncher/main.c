//
//  main.c
//  UDPHolePuncher
//
//  Created by  on 7/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int receiveSomeThing(void);
int sendSomeThing(char *opponent, char *message);

int main (int argc, const char * argv[])
{
	
	printf("Hello, World!\n");
	
	const int strLen = 256;
	char str[strLen];
	
	printf("enter opponent IP addr\n");
	fgets(str, strLen, stdin);
	
	sendSomeThing(str, "Hello mojang");
	receiveSomeThing();
    return 0;
}

int receiveSomeThing(void)
{
	int sock;
	struct sockaddr_in addr;
	
	fd_set fds, readfds;
	int n;
	struct timeval tv;
	
	char buf[2048];
	
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = INADDR_ANY;
	
	bind(sock, (struct sockaddr *)&addr, sizeof(addr));
		
	FD_ZERO(&readfds);
	
	FD_SET(sock, &readfds);
	
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	
	int i = 0;
	
	/* このサンプルでは、10秒間データを受信しないとループを抜けます */
	while (1) {
		if (i == 0) {
			
			sendSomeThing("127.0.0.1", "Hello mojang");
			
			i = 1;
		}
		/*
		 読み込み用fd_setの初期化
		 selectが毎回内容を上書きしてしまうので、毎回初期化します
		 */
		memcpy(&fds, &readfds, sizeof(fd_set));
		
		/* fdsに設定されたソケットが読み込み可能になるまで待ちます */
		n = select(sock+1, &fds, NULL, NULL, &tv);
		
		/* タイムアウトの場合にselectは0を返します */
		if (n == 0) {
			/* ループから抜けます */
			printf("timeout\n");
			break;
		}
		
		/* sock1に読み込み可能データがある場合 */
		if (FD_ISSET(sock, &fds)) {
			/* sock1からデータを受信して表示します */
			memset(buf, 0, sizeof(buf));
			recv(sock, buf, sizeof(buf), 0);
			printf("%s\n", buf);
		}
	}
	
	close(sock);
	
	return 0;
}

int sendSomeThing(char *opponent, char *message)
{
	int sock;
	struct sockaddr_in addr;
	
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.s_addr = inet_addr(opponent);
	
	sendto(sock, message, sizeof(message), 256, (struct sockaddr *)&addr, sizeof(addr));
	
	close(sock);
	
	return 0;
}

