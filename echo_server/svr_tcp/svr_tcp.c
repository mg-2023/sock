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
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    int clnt_sock;
    int serv_sock;
    char message[1024] = {0};

    serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(serv_sock == -1) {
        fputs("소켓 생성 실패\n", stderr);
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        fputs("소켓 바인딩 실패\n", stderr);
        exit(1);
    }

    if(listen(serv_sock, 5) == -1) {
        fputs("클라이언트 대기 중 오류\n", stderr);
        exit(1);
    }
    printf("클라이언트 대기열 열림\n");

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_sock == -1) {
        fputs("클라이언트 연결 실패\n", stderr);
        exit(1);
    }
    printf("클라이언트 연결됨\n");

    strcpy(message, "서버와 연결 성공\n");
    if(send(clnt_sock, message, 1024, 0) == -1) {
        fputs("메시지 보내기 실패\n", stderr);
        exit(1);
    }

    while(1) {
        if(recv(clnt_sock, message, 1024, 0) == -1) {
            fputs("메시지 받기 실패\n", stderr);
            break;
        }
        printf("클라이언트로부터 받은 메시지: %s\n", message);

        if(send(clnt_sock, message, 1024, 0) == -1) {
            fputs("메시지 보내기 실패\n", stderr);
            break;
        }
    }
    
    close(clnt_sock);
    close(serv_sock);

    return 0;
}
