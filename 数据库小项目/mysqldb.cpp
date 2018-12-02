#include "mysqldb.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



/*
 * 在表Personnel中按ID查找某一字段
 * 并把结果保存至str中
 */
int MysqlDBAccess::GetRecord(char *str,int ID)
{	
	char selectSQL[150];

	sprintf(selectSQL,"select %s from Personnel where staff_ID=%d;",str,ID);	
	if (ExcuteSQL(selectSQL)==false) 
		return false;

	result = mysql_store_result(mysql); //获取结果集<=>mysql_use_result(mysql).
	/*
	在使用mysql_query()进行一个查询后，一般要用这两个函数之一来把结果存到一个MYSQL_RES *变量中。
两者的主要区别是，mysql_use_result()的结果必须“一次性用完”，也就是说用它得到一个result后，必须反复用mysql_fetch_row()读取其结果直至该函数返回null为止，否则如果你再次进行mysql查询，会得到“Commands out of sync; you can't run this command now”的错误。
而mysql_store_result()得到result是存下来的，你无需把全部行结果读完，就可以进行另外的查询。比如你进行一个查询，得到一系列记录，再根据这些结果，用一个循环再进行数据库查询，就只能用mysql_store_result()。
	
	*/
	row = mysql_fetch_row(result);
	strcpy(str, row[0]);	
	mysql_free_result(result);	

	return true;	
}




/*
 * 执行查寻语句，并显示查寻结果
 */
int MysqlDBAccess::SDelectRecord(string selectSQL)
{
	int rowNum=-1;
	Table table;
	Fields fields;
	Record record;	

	if (isDBConnected==false) {
		cout<<"Not connect to any database"<<endl;
		return false;
	}
	if (ExcuteSQL(selectSQL)==false) 
		return false;

	result = mysql_store_result(mysql);
	rowNum = mysql_affected_rows(mysql); //mysql_affected_rows():成功，返回上一次操作中所影响的行数，否则为０．
	GetFields(fields);

	for (int i=1; i<=rowNum; i++) {
		row = mysql_fetch_row(result);
	
		for (int j = 0; j<fields.size(); j++) {
			if (row[j] == NULL) {
				record[fields[j]] = "";
			}
			else {
				record[fields[j]] = row[j];
			}
		}
		table.push_back(record);
		record.clear();
	}
	mysql_free_result(result);
	DisplayTable(fields, table);
	
	return true;	
}

/*显示查寻的内容*/
void MysqlDBAccess::DisplayTable(Fields fd,Table dt)
{
	TableIter iter;
	Record rt;
	string valStr;

	for (iter = dt.begin(); iter != dt.end();iter++) {
		rt = *iter;
		for (int i = 0; i < fd.size(); i++) {
			valStr = rt[fd[i]];
			if (valStr != "") {
				cout<<valStr<<"  ";
			}
			else {
				cout<<"---"<<" ";
			}
		}
		cout<<endl;
	}
}

int MysqlDBAccess::GetFields(Fields &fields)
{
	while (field = mysql_fetch_field(result)) //mysql_fetch_field(res):从结果集中获取一列，失败返回NULL
	{
		fields.push_back(field->name);
	}
	
	return 0;
}

/*执行mysql语句*/
bool MysqlDBAccess::ExcuteSQL(string sql)
{
	mysql_set_character_set(mysql, "gb2312");
	if (mysql_query(mysql, sql.c_str())) {
		cout<<cout<<"mysql_query: "<<mysql_error(mysql)<<endl;
		return false;
	}		
	return true;
}

//进入数据库
bool MysqlDBAccess::ConnectDB(string dbName)
{
//mysql_select_db(conn,“dbname”):设置活动的数据库,成功时返回０失败时返回非０
	if (mysql_select_db(mysql, dbName.c_str()))  
    {    
        cout<<cout<<"mysql_select_db: "<<mysql_error(mysql)<<endl;
		return false;
	}
	isDBConnected = true;
	this->dbName = dbName;
	return true;
}

bool MysqlDBAccess::ConnectSever(string severName, string userName, string pwd, int port)
{
	mysql = mysql_init(NULL);
	if(!mysql_real_connect(mysql, severName.c_str(), userName.c_str(), pwd.c_str(), NULL, port, NULL, 0))
	{
	//这是为了与c语言兼容，在c语言中没有string类型，故必须通过string类对象的成员函数c_str()把string 对象转换成c中的字符串样式。
		cout<<"mysql_real_connect: "<<mysql_error(mysql)<<endl;
		return false;
	}else
		isSeverConnected = true;
	return true;
}

/*
 * 验证用户名与密码并由参数列表返回姓名
 * 若用户名存在且密码正确则返回用户类型
 *	否则返回 -1 用户名输入有错
 *			 -2 密码有错
 */
int MysqlDBAccess::ChrRecord(char *name, char *pwd, int &ID)
{
	char selectSQL[150];

	sprintf(selectSQL,"select product_pwd,staff_type,staff_ID from Personnel where product_name=\'%s\';",name);
	ExcuteSQL(selectSQL);		

	result = mysql_store_result(mysql);	
	row = mysql_fetch_row(result);
	if(row == NULL)
		return -1;
	mysql_free_result(result);
	if(strcmp(pwd, row[0]) == 0)	
	{
		ID = atoi(row[2]);
		return atoi(row[1]);
	}
	else 
		return -2;	
}


/*
 * 数据库初始化函数
 * 连接服务器
 * 创建数据库和表
 */
void MysqlDBAccess::mysqldbinit(MysqlDBAccess &sql)
{
	sql.ConnectSever("localhost","root");			//连接服务器
	sql.ExcuteSQL("create database if not exists LzyPOS"); //创建数据库			
	sql.ConnectDB("LzyPOS");						//连接数据库
	
	/* 创建员工表 不存在则创建*/
	sql.ExcuteSQL("create table if not exists Personnel(\
				        staff_ID int auto_increment primary key,\
						product_name	varchar(15),	\
		                product_pwd		varchar(16) default '123456',	\
						staff_type   int,\
						remark	varchar(30))");

	/* 系统自动分配超级用户，用户名 root, 密码 12346*/
	const char *selectSQL = "select product_pwd from Personnel where product_name='root'";
	ExcuteSQL(selectSQL);		
	result = mysql_store_result(mysql);		
	row = mysql_fetch_row(result);		//查寻是否有root用户
	if(row == NULL)
		ExcuteSQL("insert into Personnel(product_name, staff_type, staff_ID) values('root',0, 0)");
	else
		mysql_free_result(result);
	

	sql.ExcuteSQL("create table if not exists Shop(	\
						bar_code varchar(8),			\
						product_name varchar(30),		\
						sale_price float)");

	sql.ExcuteSQL("create table if not exists Cell(	\
						sale_ID	 varchar(12),			\
						trans_ID varchar(4),			\
						staff_ID varchar(6),			\
						sale_date date,					\
						give_sum  float,				\
						real_sum  float,				\
						sale_money float,				\
						change_	  float,				\
						sale_state int);");	

	sql.ExcuteSQL("create table if not exists Detail(	\
						Detail_ID varchar(18),			\
						sale_ID  varchar(18),			\
						bar_code  varchar(8),			\
						count	  int,					\
						sale_price float,				\
						sale_state int,					\
						staff_ID   varchar(6))");
}

