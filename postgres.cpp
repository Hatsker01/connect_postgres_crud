#include <stdio.h>
#include <postgresql/libpq-fe.h>
#include <string>
#include <iostream>

using namespace std;

struct User
{
    string Id;
    string Name;
    string Phone_number;
    string Age;
};

int main()
{
    PGconn *conn;
    PGresult *res;

    int response_count;
    int row;

    // Connection to PostgreSQL database
    conn = PQconnectdb("dbname=your_db_name host=localhost user=your_db_user_name password=your_db_password");

    if (PQstatus(conn) == CONNECTION_BAD)
    {
        puts("We were unable to connect to the database");
        exit(0);
    }

    // Giving value to user struct
    User new_user;
    new_user.Id = "1";
    new_user.Name = "John";
    new_user.Phone_number = "+998901234567";
    new_user.Age = "15";

    char create_object[256];
    // Creating query to insert data to db
    sprintf(create_object, "INSERT INTO users(id, name, phone, age) VALUES('%s', '%s', '%s', '%s')", new_user.Id.c_str(), new_user.Name.c_str(), new_user.Phone_number.c_str(), new_user.Age.c_str());

    // Request to create
    res = PQexec(conn, create_object);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Insertion failed: %s", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return 1;
    }

    PQclear(res); // Free the result

    User update_user;
    update_user.Id = "1";
    update_user.Name = "Hatker";
    char update_object[300];

    
    sprintf(update_object, "UPDATE users SET name = '%s' WHERE id = '%s'", update_user.Name.c_str(), update_user.Id.c_str());

    res = PQexec(conn, update_object);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Update query failed: %s", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return 1;
    }

    PQclear(res);

    // Getting data from the database
    res = PQexec(conn, "SELECT * FROM users");
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        puts("Select query failed!");
        PQclear(res); // Free the result
        exit(0);
    }

    response_count = PQntuples(res);
    User all_user[response_count];

    for (row = 0; row < response_count; row++)
    {
        User single_user;

        single_user.Id = string(PQgetvalue(res, row, 0));
        single_user.Name = string(PQgetvalue(res, row, 1));
        single_user.Phone_number = string(PQgetvalue(res, row, 2));
        single_user.Age = string(PQgetvalue(res, row, 3));

        all_user[row] = single_user;
    }

    PQclear(res); // Free the result

    for (int i = 0; i < response_count; i++)
    {
        cout << all_user[i].Name << endl;
    }

    User delete_user;
    delete_user.Id = "2";
    char delete_user_query[400];
    sprintf(delete_user_query, "DELETE FROM users WHERE id = '%s'", delete_user.Id.c_str());

    res = PQexec(conn, delete_user_query);

    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        puts("Delete query failed!");
        PQclear(res); // Free the result
        exit(0);
    }
    PQclear(res);
    PQfinish(conn); // Close the database connection

    return 0;
}
