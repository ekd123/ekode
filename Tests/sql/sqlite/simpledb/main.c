#include <stdio.h>
#include <sqlite3.h>

static int callback (void *user_data, int argc ,char **argv, char **azColName)
{
    int i;
    for (i = 0; i<argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i]);
    }
    printf ("\n");
    return 0;
}

int main ()
{
    sqlite3 *db;
    sqlite3_open ("data.db", &db);
    sqlite3_exec (db, "CREATE TABLE users (id TEXT PRIMARY KEY, nick TEXT, salt CHAR(10), passwd CHAR(128));", NULL, NULL, NULL);
    sqlite3_exec (db, "SELECT * FROM users WHERE id=\"StarBrilliant\"", callback, NULL, NULL);
    sqlite3_close (db);
    return 0;
}

