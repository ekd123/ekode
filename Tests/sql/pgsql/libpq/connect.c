#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>

static void
exit_nicely(PGconn *conn)
{
    PQfinish(conn);
    exit(1);
}

int main ()
{
    const char *conninfo;
    PGconn *conn;
    PGresult *res;
    int nFields;
    int i,j;
    
    conninfo = "dbname = mike";
    conn = PQconnectdb (conninfo);
    if (PQstatus (conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s",
                PQerrorMessage(conn));
        exit_nicely(conn);
    }
    PQfinish (conn);
    return 0;
}

