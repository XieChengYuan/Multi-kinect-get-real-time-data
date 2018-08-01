#include "server.h"

int main()
{
	system("title 服务端");//设置cmd窗口标题
	system("color 0B");
	Server svr;
	svr.WaitForClient();
	system("pause");
	return 0;
}

