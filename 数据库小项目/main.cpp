/*
 * 使用C++、Mysql简单模拟POS机部分功能
 *
 * 功能简介
	1登入模块
		输入账号和密码，连接数据库与验证账号和密码，按账号类型登入收银界面或管理界面

	2收银模块流程
		1．输入商品ID
			2．根据商品ID去查找数据中商品的价格，和名称
			3．打印商品信息
		4．输入商品数量
		5．打印应收总额

	3管理模块
		1商品管理
			3.1商品查询			
			3.2添加商品
				1．输入商品ID
				2．输入商品名称
				3．输入商品单价
			3.3修改商品
			3.4删除商品
				根据商品ID删除商品

		2账户管理
			1账户查询
			2添加账户
			3修改账户
			4删除账户
 
 */

#include <iostream>
#include <string>
#include "mysqldb.h"
#include "user.h"

using namespace std;

/*
 * 普通用户模式主程序
 * 输入商品ID
 * 根据商品ID去查找数据中商品的价格，和商品名称
 * 打印商品信息，输入商品数量，打印应收总额
 */
int user(MysqlDBAccess &mysql)
{
	User p;
	float price; 
	char name[30];
	int count;
	string buf;
	while(1)
	{
		cout<<"\nShop ID ";
		cin>>buf;
		price = p.FindByIDPrice(buf,name, mysql);	//根据商品ID，得到商品单价和名称
		if(price < 0)
			continue;
		cout<<"Shop Name:  "<<name<<endl;
		cout<<"Shop price: "<<price<<endl;
		cout<<"shop Count: ";
		cin>>count;		
		cout<<"all monery: "<<price*count<<endl<<endl;	
		
		cout<<" Quit(n) S> ";		//命令提示
		getchar();
		if(getchar() == 'n')
			break;
	}
	return 0;
}

/* 
 * 管理员模式主程序
 * 进入管理员界面，提供对商品和普通用户增加改查等管理权限
 */
void management(MysqlDBAccess &mysql)
{
	Management p;
	int choose;
	while(1)
	{
		cout<<"\n 1. Add User"<<endl;
		cout<<" 2. Delete User"<<endl;
		cout<<" 3. Display User"<<endl;
		cout<<" 4. Add Shop"<<endl;
		cout<<" 5. Delete Shop"<<endl;
		cout<<" 6. Display Shop"<<endl;
		cout<<" 0. quit"<<endl<<"\n M> ";
		cin>>choose;
		switch(choose)
		{
		case 1:	
			p.AddUser(mysql);
			break;
		case 2:
			p.DeleteUser(mysql);
			break;
		case 3:
			p.DisplayUser(mysql);
			break;
		case 4:	
			p.AddShop(mysql);
			break;
		case 5:
			p.DeleteShop(mysql);
			break;
		case 6:
			p.DisplayShop(mysql);
			break;
		case 0:
			return ;
		default :
			break;
		}
	}
}

/*
 * 超级管理员模式主程序
 * 进入超级管理员界面，提供对管理员增加改查等管理权限
 */
void root(MysqlDBAccess &mysql)
{
	Root p;
	int choose;
	while(1)
	{
		cout<<"\n 1. Add Management"<<endl;
		cout<<" 2. Delete Management"<<endl;
		cout<<" 3. Display Management"<<endl;		
		cout<<" 0. quit"<<endl<<"\n R> ";
		cin>>choose;
		switch(choose)
		{
		case 1:	
			p.AddManagement(mysql);
			break;
		case 2:
			p.DeleteManagement(mysql);
			break;
		case 3:
			p.DisplayManagement(mysql);
			break;		
		case 0:
			return ;
		default :
			break;
		}
	}
}


int main(void)
{
	char name[20], pwd[10];
	int staff_type,				//用户类型 0->超级用户 1->管理员 其它为普通用户
		staff_ID;				//编号

	MysqlDBAccess mysql;					//定义数据库对象
	mysql.mysqldbinit(mysql);				//初始化数据库
	
	while(1)
	{
		cout<<"\nname: ";	cin>>name;
		cout<<"pwd : ";	cin>>pwd;
		staff_type = mysql.ChrRecord(name, pwd, staff_ID);//用户名与密码验正
		if( staff_type <= -1)	//登陆失败
		{
			cout<<"faile"<<endl;
			continue;
		}
		else 
			break;
	}
	
	switch(staff_type)
	{
	case 0:
		cout<<"\nRoot: "<<staff_ID<<"->"<<name<<endl;		
		root(mysql);			//进入超级管理员模式
		break;

	case 1:
		cout<<"\nManagement: "<<staff_ID<<"->"<<name<<endl;		
		management(mysql);		//进入管理员模式
		break;

	default:
		cout<<"\nUser: "<<staff_ID<<"->"<<name<<endl;
		user(mysql);			//进入普通用户模式
		break;
	}
	
	cout<<"Bye\n"<<endl;
	return 0;
}
