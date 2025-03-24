# Net

```
#include <winsock2.h>
```

## Sever_socket


```
클라이언트의 연결을 대기 하는 역할
```

### 소켓 초기화

```
// 소켓 초기화.
WSADATA wsa;
if (WSAStartup((MAKEWORD(2, 2)), &wsa) != 0)    // 2 . 2 버전 실행.

WSACleanup(); -> 종료
```


### 소켓 생성

#### -socket()
```
SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// AF_INET : TCP / UDP //  SOCK_STREAM : TCP 
if (listen_sock == INVALID_SOCKET)      // INVALID_SOCKET
{
	printf(" 소켓 생성 실패\n");
	return -1;
}


closesocket(sock);	-> 소켓 종료료
```

```
soket
  매개변수
    1. AF_INET     // IPv4 사용
    2. SOCK_STREAM // TCP / UDP 사용
    3. ( TCP or UDP 사용 여부 ) IPPROTO_TCP : tcp // IPPROTO_UPD : upd
```
#### -SOCKADDR_IN

```
SOCKADDR_IN addr;		//IPv4 전용	SOCKADDR : 범용
memset(&addr, 0, sizeof(SOCKADDR_IN));	// 메모리 셋 
```

```
소켓 구조체
SOCKADDR_IN 
{
  1. ADDRESS_FAMILY sin_family;   // IP type  ( AF_INET : IPv4 / AF_INET6 : IPv6 ) 
  2. USHORT sin_port;             // 포트 번호
  3. IN_ADDR sin_addr;            // 주소값
  4. CHAR sin_zero[8];            // 빈공간
}
```
#### -bind

```
addr.sin_family = AF_INET;				          -> IPv4 선언
addr.sin_addr.s_addr = htonl(INADDR_ANY);	  -> 누구나 들어 올 수 있음
addr.sin_port = htons(SERVER_PROT);			    -> 포트 번호 입력.

int ret = bind(listen_sock, (SOCKADDR*)&addr, sizeof(addr));
if (ret == SOCKET_ERROR)
{
	printf(" bind  실패\n");
	return -1;
}
```

```
bind    반환값 :  -1(SOCKET_ERROR)  오류 
  매개변수
    1. 대기 소켓 주소
    2. 소켓 구조체 주소
    3. 사이즈값.
```


#### -Listen

```
ret = listen(listen_sock, SOMAXCONN);
if (ret == SOCKET_ERROR)
```

```
매개변수    반환값 :  -1(SOCKET_ERROR)  오류 
  1. 대기 소켓 주소
  2. 소켓 최대 수용인원 값.
```


### 연결 대기

#### -accept
```
SOCKADDR_IN client_addr;
memset(&client_addr, 0, sizeof(SOCKADDR_IN));  // 구조체 초기화.
int client_length = sizeof(SOCKADDR_IN);

static SOCKET client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &client_length);
	if (client_sock == INVALID_SOCKET)

```

```
accept
매개변수      반환 값 : 통신소켓 핸들값.
  1. 대기 소켓 핸들값
  2. 통신할 통신 소켓 구조체 주소
  3. 통신 소켓 사이즈 주소
```

### 메세지 수신


#### -recv

```
har buf[1024];
int recv_byte = recv(client_sock, buf, sizeof(buf), 0);


int recv_byte = recv(sock, buf, sizeof(buf), 0);
if (recv_byte == 0)
{
	상대방이 closesocket 으로 소켓을 종료함
}
else if (recv_byte == -1)
{
  -1 == SOCKET_ERROR
	대기 소켓이 꺼졌거나 , 상대방의 오류
}
else
{
	송수신 가능.
}
```

```
recv
매개변수
  1. 통신 소켓 핸들값
  2. 문자열 받을 버퍼
  3. 버퍼 크기
  4. flag (NULL )
```

### 메세지 송신

#### -send

```
char buf[1024];
printf(">>");	gets_s(buf, sizeof(buf));							// 보낼 값

int send_byte = send(client_sock, buf, (int)strlen(buf) + 1, 0);	
```

```
send
  매개변수
    1. 통신 소켓 핸들값
    2. 보낼 버퍼 주소
    3. 버퍼 사이즈
    4. flag
```


## client socket

### 초기화 / 생성  동일

### 소켓 연결

#### connect 
```
SOCKADDR_IN addr;
memset(&addr, 0, sizeof(SOCKADDR_IN));				
addr.sin_family = AF_INET;				// IP type				
addr.sin_addr.s_addr = inet_addr(SERVER_IP);		// 접속하고 싶은 SEVER IP 
addr.sin_port = htons(SERVER_PROT);			// 포트 번호

int ret = connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
if (ret == SOCKET_ERROR)
{	
	printf(" connect  실패\n");
	return -1;
}	
```

```
connect
   매개변수
	1. 
connect
  매개변수
    	1. 통신 소켓 핸들값
	2. 소켓 구조체 주소값
    	3. 사이즈	
```


### 메세지 송수신 동일


# 이모저모

## union

```
typedef struct in_addr {
        union {
                struct { UCHAR s_b1,s_b2,s_b3,s_b4; } S_un_b;
                struct { USHORT s_w1,s_w2; } S_un_w;
                ULONG S_addr;
        } S_un;
#define s_addr  S_un.S_addr /* can be used for most tcp & ip code */
#define s_host  S_un.S_un_b.s_b2    // host on imp
#define s_net   S_un.S_un_b.s_b1    // network
#define s_imp   S_un.S_un_w.s_w2    // imp
#define s_impno S_un.S_un_b.s_b4    // imp #
#define s_lh    S_un.S_un_b.s_b3    // logical host
} IN_ADDR, *PIN_ADDR, FAR *LPIN_ADDR;
```

```
- 유니온 끼리는 메모리를 공유함.
	-> 분할 / 조립 용도로 활용 가능

예시)
in_addr add; -> 구조체 선언

add.S_ddr =  ip 값 입력

add.S_un_b.s_b1 => 으로 1byte씩 출력가능
```

## inet_ntoa + inet_addr

```
inet_ntoa(addr.sin_addr);	
```

```
- IPv4 에서만 사용 가능

매개변수  : netwrok -> host 또는 host -> network 로 변환 할 때 정수 -> 문자열로 변환
```

```
inet_addr("1.2.3.4);
```

```
- IPv4 에서만 사용 가능

매개변수 : inet_ntoa 처럼 변환은 동일 / 문자열 -> 정수 변환.
```

## htons + ntohs

```
// 소켓에서 주로 사용
// host byte -> network byte	 ( 빅 엔디안 방식으로 전송 ) 
htons -> host to net short
htonl -> host to net long
```

```
// 프로그램에서  사용
// network byte -> host byte	( 빅 엔디안 , 리틀 엔디안 사용자 지정해서 전송 ) 
ntohs -> net to host short
ntohl -> net to host long
```





