#include "user.h"
#include <string.h>
float User::FindByIDPrice(string ID, char *shop, MysqlDBAccess &mysql)
{
	char buf[150];
	/*根据商品ID查寻商品单价、名称*/
	sprintf(buf,"select sale_price, product_name from Shop where bar_code=\'%s\';",ID.c_str());
	if(mysql.ExcuteSQL(buf)==false) 
		return -1;

	mysql.result = mysql_store_result(mysql.mysql);
	mysql.row = mysql_fetch_row(mysql.result);
	if(mysql.row == NULL)
	{
		cout<<"NO this Shop"<<endl;
		return -1;
	}
	strcpy(shop, mysql.row[1]);			//得到商品名称
	mysql_free_result(mysql.result);
	return atof(mysql.row[0]);			//返回商品单价
}


void Management::DeleteShop(MysqlDBAccess &mysql)
{
	string name;
	cout<<"Input Name:";
	cin>>name;		//输入要删除的商品名称

	char buf[150];		
	sprintf(buf,"delete from Shop where product_name = \'%s\'",name.c_str());
	mysql.ExcuteSQL(buf);

	cout<<"\nshop list:"<<endl;		//自动输出所有用户信息
	DisplayShop(mysql);
}

void Management::AddShop(MysqlDBAccess &mysql)
{
	int bar_code;
	cout<<"Input staff_ID: ";	//输入商品编号
	cin>>bar_code;

	string product_name;
	cout<<"Input Name: ";		//输入商品名称
	cin>>product_name;

	float sale_price;		
	cout<<"Input price: ";		//输入商品价格
	cin>>sale_price;

	char buf[150];		
	sprintf(buf,"insert into Shop value(%d,\'%s\', %.4f)",bar_code,product_name.c_str(),sale_price);
	mysql.ExcuteSQL(buf);		//插入商品信息

	cout<<"\nshop list:"<<endl;		//自动输出所有用户信息
	DisplayShop(mysql);
}

void Management::DisplayShop(MysqlDBAccess &mysql)
{
	string sql("select * from Shop");
	mysql.SDelectRecord(sql);
}

void Management::DeleteUser(MysqlDBAccess &mysql)
{
	string name;
	cout<<"Input Name:";
	cin>>name;

	char buf[150];		
	sprintf(buf,"delete from Personnel where  product_name = \'%s\'",name.c_str());
	mysql.ExcuteSQL(buf);

	cout<<"\nproduct_name list:"<<endl;		//自动输出所有用户信息
	DisplayUser(mysql);
}

void Management::DisplayUser(MysqlDBAccess &mysql)
{
	string sql("select * from Personnel where staff_type=2");
	mysql.SDelectRecord(sql);
}

void Management::AddUser(MysqlDBAccess &mysql)
{
	string product_name;	
	cout<<"Input Name: ";
	cin>>product_name;

	char buf[150];	
	sprintf(buf,"insert into Personnel(product_name,staff_type) values(\'%s\', 2);",product_name.c_str());
	if(mysql.ExcuteSQL(buf)==false) 
		cout<<"Faile: "<<endl;
	else
		cout<<"Scuess! "<<endl;

	cout<<"\nproduct_name list:"<<endl;
	DisplayUser(mysql);
}


void Root::AddManagement(MysqlDBAccess &mysql)
{
	string product_name;	
	cout<<"Input Name: ";
	cin>>product_name;

	char buf[150];	
	sprintf(buf,"insert into Personnel(product_name,staff_type) values(\'%s\', 1);",product_name.c_str());
	if(mysql.ExcuteSQL(buf)==false) 
		cout<<"Faile: "<<endl;
	else
		cout<<"Scuess! "<<endl;

	cout<<"\nproduct_name list:"<<endl;
	DisplayManagement(mysql);
}


void Root::DisplayManagement(MysqlDBAccess &mysql)
{
	string sql("select * from Personnel where staff_type=1");
	mysql.SDelectRecord(sql);
}

void Root::DeleteManagement(MysqlDBAccess &mysql)
{
	string name;
	cout<<"Input Name:";
	cin>>name;

	char buf[150];		
	sprintf(buf,"delete from Personnel where  product_name = \'%s\'",name.c_str());
	mysql.ExcuteSQL(buf);

	cout<<"\nproduct_name list:"<<endl;		//自动输出所有用户信息
	DisplayManagement(mysql);
}

