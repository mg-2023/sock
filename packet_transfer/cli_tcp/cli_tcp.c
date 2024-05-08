#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SIZE 8192

long get_timediff(struct timeval* st, struct timeval* et) {
    long diff_sec = et->tv_sec - st->tv_sec;
    long diff_us = et->tv_usec - st->tv_usec;
    if(diff_us < 0) {
        diff_us += 1000000L;
        diff_sec -= 1;
    }

    return diff_sec * 1000000L + diff_us;
}

int main(int argc, char** argv) {
    if(argc < 3) {
        if(argc < 2) {
            fputs("IP주소와 ", stderr);
        }
        fputs("포트번호 입력 필요\n", stderr);
        exit(1);
    }

    char* message = (char*)malloc(sizeof(char) * SIZE);
    for(int i=0; i<SIZE; i++) {
        message[i] = (char)rand();
    }

    int clnt_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clnt_sock == -1) {
        fputs("소켓 생성 실패\n", stderr);
        exit(1);
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // 서버주소를 1번째 인자로 받음
    serv_addr.sin_port = htons(atoi(argv[2])); // 연결포트를 2번째 인자로 받음

    if(connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        fputs("연결 실패\n", stderr);
        exit(1);
    }
    printf("연결 성공\n");
    
    struct timeval st, et;
    gettimeofday(&st, NULL);
    if(send(clnt_sock, message, SIZE, 0) == -1) {
        fputs("데이터 보내기 실패\n", stderr);
        exit(1);
    }
    gettimeofday(&et, NULL);
    printf("Send Time: %ld us\n", get_timediff(&st, &et));

    free(message);
    close(clnt_sock);

    return 0;
}
