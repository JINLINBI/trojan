#pragma once
class CmdServer
{
public:
	CmdServer(int = CMDPORT);
	~CmdServer();
	int listen();
private:
	int MasterPort = 0;
};

