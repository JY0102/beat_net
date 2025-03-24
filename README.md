# Net

```
#include <winsock2.h>
```

## **Sever_socket**


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
SOCKADDR_IN addr;
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












