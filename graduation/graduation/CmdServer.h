#pragma once
class CmdServer
{
public:
	CmdServer(int);
	~CmdServer();
	int listenAtPort();
private:
	int MasterPort = 2333;
};

