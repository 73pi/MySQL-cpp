#ifndef MYSQLDBACCESS_H
#define MYSQLDBACCESS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <mysql/mysql.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

typedef map<string, string>	Record;
typedef map<string, string>::iterator RecordIter;

typedef vector<Record>	Table;
typedef vector<Record>::iterator TableIter;

typedef vector<string>	Fields;
typedef vector<string>::iterator FieldsIter;

class User;

class MysqlDBAccess
{
public:
	MysqlDBAccess(){isSeverConnected = isDBConnected = false;}
	~MysqlDBAccess(){if(isSeverConnected) DisconnentSever();}
	bool ConnectSever(string severName, string userName, string pwd="917", int port=3306);	//连接服务器
	void DisconnentSever(void){mysql_close(mysql);}		
	bool ConnectDB(string dbName);						//连接数据库
	bool ExcuteSQL(string sql);						//执行语句
	int SDelectRecord(string selectSQL);				//执行查寻语句并显示查寻内容

	int ChrRecord(char *name,char *pwd,int &ID);	//用户名密码验证	
	void mysqldbinit(MysqlDBAccess &sql);			//数据库初始化
	int GetRecord(char *str,int ID);
	
	friend	class User;

protected:
	int GetFields(Fields & fields);
	void DisplayTable(Fields fd,Table dt);

private:
	MYSQL *mysql;
	MYSQL_RES *result;
    MYSQL_ROW row;//获取行
    MYSQL_FIELD *field;//获取列
    
	bool isSeverConnected;
	bool isDBConnected;
	string dbName;
};



#endif
