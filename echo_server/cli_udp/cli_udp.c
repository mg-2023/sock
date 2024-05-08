#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char** argv) {
    if(argc < 3) {
        if(argc < 2) {
            fputs("IP주소와 ", stderr);
        }
        fputs("포트번호 입력 필요\n", stderr);
        exit(1);
    }

    char message[1024] = {0}; // 서버로부터 받을 메시지

    int clnt_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(clnt_sock == -1) {
        fputs("소켓 생성 실패\n", stderr);
        exit(1);
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // 서버주소를 1번째 인자로 받음
    serv_addr.sin_port = htons(atoi(argv[2])); // 연결포트를 2번째 인자로 받음

    if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) != 0) {
        fputs("연결 실패\n", stderr);
        exit(1);
    }
    printf("서버와 연결됨\n");

    int serv_addr_len = sizeof(serv_addr);
    while(1) {
        printf("메시지 입력: ");
        fgets(message, 1024, stdin);
        if(sendto(clnt_sock, message, 1024, 0, (struct sockaddr*)&serv_addr, serv_addr_len) == -1) {
            fputs("메시지 보내기 실패\n", stderr);
            break;
        }

        if(recvfrom(clnt_sock, message, 1024, 0, (struct sockaddr*)&serv_addr, &serv_addr_len) == -1) {
            fputs("메시지 받기 실패\n", stderr);
            break;
        }
        printf("서버로 갔다온 메시지: %s\n", message);
    }

    close(clnt_sock);

    return 0;
}
