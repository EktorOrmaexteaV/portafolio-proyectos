//DatabaseManager.h
#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <mysql/mysql.h>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>


class DatabaseManager {
private:    
    MYSQL* conn;
    std::map<std::string, std::string> properties;

    std::string host;
    std::string user;
    std::string pass;
    std::string db;
    int port;

public:    
    DatabaseManager();    
    ~DatabaseManager();
    

    void loadProperties();
    
    bool connect();
    bool validateteQuery(const std::string& query);
    
    bool insertData(const std::string& tableName, 
                    const std::vector<std::string>& columns, 
                    const std::vector<std::string>& values);

    void readData(const std::string& tableName);
    bool updateData(const std::string& tableName, 
                    const std::map<std::string, std::string>& setColumns, 
                    const std::map<std::string, std::string>& whereConditions);

    bool deleteData(const std::string& tableName, 
                    const std::map<std::string, std::string>& whereConditions);

    void executeQuery(const std::string& query);

    std::string executeScalar(const std::string& query);

    bool existsData(const std::string& query);
    
    // Devuelve los resultados de una consulta SELECT como vector de mapas (columna->valor)
    std::vector<std::map<std::string, std::string>> fetchQuery(const std::string& query);
};

#endif
