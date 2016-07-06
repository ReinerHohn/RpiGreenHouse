#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>


MYSQL* sqlWrapOpen()
{
    MYSQL* connection;
    /* Initialize the connection object */
    connection = mysql_init(NULL);

    if (connection == NULL)
    {
        fprintf(stderr, "Failed to initialize: %s\n", mysql_error(connection));
        return 1;
    }

    return connection;
}

int sqlWrapConnect(MYSQL* connection, char* hostname, char* port, char* username, char* password, char* dbname)
{
    /* Connect to the server */
    if (!mysql_real_connect(connection, hostname, username, password, dbname, atoi(port), NULL, 0))
    {
        fprintf(stderr, "Failed to connect to server: %s\n", mysql_error(connection));
        return 1;
    }

    printf("\nYou're now connected to the server.\n");
}

MYSQL_RES* sqlWrapQuery(MYSQL* connection, char* query)
{
    MYSQL_RES* result;

    /* Execute the query */
    if (mysql_query(connection, query))
    {
        fprintf(stderr, "Error querying server: %s\n", mysql_error(connection));
    }

    /* Get the result set */
    result = mysql_use_result(connection);

    if (result == NULL)
    {
        printf("No result.\n");
    }

    return result;
}

int sqlParse(MYSQL* connection, MYSQL_RES* result)
{
    unsigned int num_fields;
    MYSQL_ROW row;
    /* Print all rows */
    num_fields = mysql_num_fields(result);

    while (row = mysql_fetch_row(result))
    {
        unsigned long* field_lengths = mysql_fetch_lengths(result);

        if (field_lengths == NULL)
        {
            fprintf(stderr, "Failed to get field lengths: %s\n", mysql_error(connection));
            return 1;
        }

        for (int i = 0; i < num_fields; i++)
        {
            printf("[%.*s] ", (int) field_lengths[i], row[i] ? row[i] : "NULL");
        }

        printf("\n");
    }

    /* Free result set */
    mysql_free_result(result);
}

void sqlWrapClose(MYSQL* connection)
{
    mysql_close(connection);
}
