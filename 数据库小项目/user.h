#ifndef _USER_H_
#define _USER_H_

#include <iostream>
#include <string>
#include "mysqldb.h"
#include <stdlib.h>
#include <stdio.h>
using namespace std;


/*
 * 普通用户类
 * 数据成员：  
 * 函数成员： 收银模块 
					流程					函数名
				输入商品编号 返回商品单价  FindByIDPrice
				输入购买数量 返回应收金额			
				输入实收总额 返回应找金额	
			  
 *
 */
class User
{
public:
	float FindByIDPrice(string ID, char *shop, MysqlDBAccess &mysql);
private:
	
};


/*
 * Management类  
 *	管理普通用户和商品 包括增删改查
 *
 */

class Management
{
public:
	void AddUser(MysqlDBAccess &mysql);			//增加普通用户
	void DisplayUser(MysqlDBAccess &mysql);		//显示所在用户
	void DeleteUser(MysqlDBAccess &mysql);		//删除用户
	void AddShop(MysqlDBAccess &mysql);			//增加商品信息
	void DisplayShop(MysqlDBAccess &mysql);		//显示所有商品信息	
	void DeleteShop(MysqlDBAccess &mysql);		//删除商品信息
}; 


class Root
{
public:
	void AddManagement(MysqlDBAccess &mysql);			//增加普通用户
	void DisplayManagement(MysqlDBAccess &mysql);		//显示所在用户
	void DeleteManagement(MysqlDBAccess &mysql);		//删除用户 
};


#endif
