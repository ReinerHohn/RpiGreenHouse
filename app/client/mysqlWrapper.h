#ifndef MYSQL_WRAPPER_H
#define MYSQL_WRAPPER_H

#include <mysql.h>

MYSQL* sqlWrapOpen();

int sqlWrapConnect(MYSQL* connection, char* hostname, char* port, char* username, char* password, char* dbname);

MYSQL_RES* sqlWrapQuery(MYSQL* connection, char* query);

int sqlParse(MYSQL* connection, MYSQL_RES* result);
long sqlWrapgetRow(MYSQL* connection, MYSQL_RES* result);
void sqlWrapClose(MYSQL* connection);


#endif
