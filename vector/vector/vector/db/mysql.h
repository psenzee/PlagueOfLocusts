#pragma once

#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>

class Database
{
    typedef std::string string_type;
    //typedef sql::SQLString string_type;
    
    sql::mysql::MySQL_Driver *_driver;
    sql::Connection          *_con;

public:

    Database()
    {
        // Create a driver instance
        _driver = sql::mysql::get_mysql_driver_instance();
        // Establish a connection
        _con = _driver->connect("tcp://127.0.0.1:3308", "mend", "RWA*EyB3gcA6cbzT"); // dev
    }
    
    ~Database()
    {
        _con->close();
        delete _con;
        _con = nullptr;
    }
    
    void execute()
    {
        sql::Statement *stmt;
        sql::ResultSet *res;

        // Create a statement
        stmt = _con->createStatement();

        // Execute a SQL query
        //res = stmt->executeQuery("SELECT userId, orgId, chartId FROM core.usersOrgsInformation LIMIT 10");
        res = stmt->executeQuery("SELECT id, email FROM core.users LIMIT 10");

        // Process the result set
        while (res->next()) {
            // Access row data
            //int userId = res->getInt("userId"), orgId = res->getInt("orgId");
            int id = res->getInt("id");
            //std::string text(res->getString("chartId").c_str());
            //std::string
            string_type text(res->getString("email").c_str());
            //std::cout << "userId: " << userId << " orgId: " << orgId << " chartId: " << text << std::endl;
            std::cout << "id: " << id <<  " email: " << text << std::endl;
            // Process the data as needed
        }

        // Clean up resources
        delete res;
        delete stmt;
    }
};
