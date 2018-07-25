#include "server.h"
#include<iostream>
#include<string>
using std::cout;
using std::cin;
using std::endl;
int main()
{
	system("title 服务端");//设置cmd窗口标题
	system("color 0B");
	Server svr;
	cout << "***********************************************************" << endl;
	cout << "[帮助]：【D】深度【C】彩色【B】人体索引【BCD】此三种         " << endl;
	cout << "      【BD】深度+人体索引【CD】深度+彩色【BC】人体索引+彩色" << endl;
	cout << "***********************************************************" << endl;
	cout << "输入所需数据:";
	while (true) {
		std::cin >> svr.inData;
		if ((svr.inData == "D")|(svr.inData == "B")|(svr.inData == "C")|
			(svr.inData == "BCD")|(svr.inData == "BD")|(svr.inData == "CD")|
			(svr.inData == "BC"))
			break;
		else
			cout << "输入错误，请严格按照[帮助]中输入(区分大小写)" << endl;
	}
	svr.WaitForClient(svr.inData);
	system("pause");
	return 0;
}

