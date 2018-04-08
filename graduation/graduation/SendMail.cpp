#include "stdafx.h"
#include "SendMail.h"

#define WSWENS MAKEWORD(2,0)  
SendMail::SendMail()
{
	sockaddr_in sin;
	WSADATA wsadata;
	if (WSAStartup(WSWENS, &wsadata) != 0)
		cout << "startup failed" << endl;

	SOCKET s = socket(PF_INET, SOCK_STREAM, 0);
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(25);
	hostent* hptr = gethostbyname("smtp.sina.com");
	memcpy(&sin.sin_addr.S_un.S_addr, hptr->h_addr_list[0], hptr->h_length);
	printf("IP of smpt.sina.com is : %d:%d:%d:%d",
		sin.sin_addr.S_un.S_un_b.s_b1,
		sin.sin_addr.S_un.S_un_b.s_b2,
		sin.sin_addr.S_un.S_un_b.s_b3,
		sin.sin_addr.S_un.S_un_b.s_b4);

	cout << endl;
	if (connect(s, (sockaddr*)&sin, sizeof(sin)))
	{
		cout << "connect failed" << endl;
		exit(0);
	}
	else
	{
		cout << "connect success" << endl;
	}

	char bufferresv[10240];
	int len = recv(s, bufferresv, 10240, 0);
	bufferresv[len] = 0;
	printf("%s", bufferresv);
	cout << endl;
	// send "helo ehlo"  
	char bufferHello[] = "helo ehlo/r/n";
	cout << "helo ehlo..." << endl;
	send(s, bufferHello, strlen(bufferHello), 0);
	len = recv(s, bufferresv, 10240, 0);
	bufferresv[len] = 0;
	printf("%s", bufferresv);
	cout << endl;
	// send "auth login"  
	char bufferLogin[] = "auth login/r/n";
	cout << "auth login..." << endl;
	send(s, bufferLogin, strlen(bufferLogin), 0);
	len = recv(s, bufferresv, 10240, 0);

	bufferresv[len] = 0;
	printf("%s", bufferresv);

	cout << endl;
	// send "username" send "psw"  
	char bufferUserName[] = "c3VubnlrYWhv/r/n";
	char bufferpsw[] = "MjAwNzE2MTA0MDk=/r/n";

	cout << "input username...." << endl;
	send(s, bufferUserName, strlen(bufferUserName), 0);
	len = recv(s, bufferresv, 10240, 0);
	bufferresv[len] = 0;
	printf("%s", bufferresv);
	cout << endl;
	cout << "input password..." << endl;
	send(s, bufferpsw, strlen(bufferpsw), 0);
	len = recv(s, bufferresv, 10240, 0);
	bufferresv[len] = 0;
	printf("%s", bufferresv);
	cout << endl;
	// mail from:<sunnykaho@sina.com>  
	char bufferMailFrom[] = "mail from:<sunnykaho@sina.com>/r/n";
	cout << "mail from:<sunnykaho@sina.com>..." << endl;
	send(s, bufferMailFrom, strlen(bufferMailFrom), 0);
	len = recv(s, bufferresv, 10240, 0);
	bufferresv[len] = 0;
	printf("%s", bufferresv);
	cout << endl;
	// rcpt to:<kaholi@qq.com>  
	char bufferRcptTo[] = "rcpt to:<kaholi@qq.com>/r/n";
	cout << "rcpt to:<kaholi@qq.com>..." << endl;
	send(s, bufferRcptTo, strlen(bufferRcptTo), 0);
	len = recv(s, bufferresv, 10240, 0);
	bufferresv[len] = 0;
	printf("%s", bufferresv);
	cout << endl;

	// data  
	char bufferData[] = "data/r/n";
	cout << "data..." << endl;
	send(s, bufferData, strlen(bufferData), 0);
	len = recv(s, bufferresv, 10240, 0);
	bufferresv[len] = 0;
	printf("%s", bufferresv);
	cout << endl;
	//   
	char bufferTo[] = "to:kaholi@qq.com/r/n";
	char bufferFrom[] = "from:sunnykaho@sina.com/r/n";
	cout << "to:kaholi@qq.com... " << endl;
	send(s, bufferTo, strlen(bufferTo), 0);
	cout << "from:sunnykaho@sina.com..." << endl;
	send(s, bufferFrom, strlen(bufferFrom), 0);

	char bufferSubject[] = "subject:hello from kaho/r/n";
	cout << "subject:hello from kaho..." << endl;
	send(s, bufferSubject, strlen(bufferSubject), 0);
	char bufferPoint[] = "./r/n";
	send(s, bufferPoint, strlen(bufferPoint), 0);
	len = recv(s, bufferresv, 10240, 0);
	bufferresv[len] = 0;
	printf("%s", bufferresv);
	cout << endl;
}


SendMail::~SendMail()
{
}
