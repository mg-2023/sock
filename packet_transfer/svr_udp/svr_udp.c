#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char** argv) {
    if(argc < 2) {
        fputs("포트번호 입력 필요\n", stderr);
        exit(1);
    }

    struct sockaddr_in serv_addr;
    int clnt_sock;
    char message[1024] = {0};

    clnt_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(clnt_sock == -1) {
        fputs("소켓 생성 실패\n", stderr);
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        fputs("소켓 바인딩 실패\n", stderr);
        exit(1);
    }
    printf("메시지 받는 중...\n");

    int serv_addr_len = sizeof(serv_addr);
    while(1) {
        if(recvfrom(clnt_sock, message, 1024, 0, (struct sockaddr*)&serv_addr, &serv_addr_len) == -1) {
            fputs("메시지 받기 실패\n", stderr);
            break;
        }
        printf("클라이언트로부터 받은 메시지: %s\n", message);

        if(sendto(clnt_sock, message, 1024, 0, (struct sockaddr*)&serv_addr, serv_addr_len) == -1) {
            fputs("메시지 보내기 실패\n", stderr);
            break;
        }
    }
    
    close(clnt_sock);

    return 0;
}
