//DatabaseManager.cpp
#include "DatabaseManager.h"
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>

// Constructor: Inicializa la conexión MySQL
DatabaseManager::DatabaseManager() : conn(nullptr) {
    loadProperties();
    host = this->properties["db.host"];
    user = this->properties["db.user"];
    pass = this->properties["db.pass"];
    db = this->properties["db.name"];
    std::string portStr = properties["db.port"];
    size_t idx;
    try {
        port = std::stoi(portStr, &idx);
        if (idx != portStr.size()) {
            throw std::invalid_argument("Invalid port number");
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: invalid port number in database.properties" << std::endl;
        // Maneja el error aquí, por ejemplo, estableciendo un valor por defecto para el puerto
        port = 3306; // valor por defecto
    }

    conn = mysql_init(nullptr);
    if (!conn) {
        std::cerr << "Error: mysql_init falló en DatabaseManager.\n";
    }
}

// Destructor: Cierra la conexión MySQL
DatabaseManager::~DatabaseManager() {
    if (conn) {
        mysql_close(conn);
        std::cout << "\nConexión a la base de datos cerrada por DatabaseManager.\n";
    }
}

// void DatabaseManager::loadProperties() {
//     // Comprobación de la ruta del archivo
//     if (!std::ifstream("../../database.properties").good()) {
//         std::cerr << "Error: no se pudo encontrar el archivo database.properties" << std::endl;
//         return;
//     }

//     // Comprobación de permisos de acceso
//     if (access("../../database.properties", R_OK) != 0) {
//         std::cerr << "Error: no se tienen permisos de acceso para leer el archivo database.properties" << std::endl;
//         return;
//     }

//     // Comprobación del formato del archivo
//     std::ifstream propertiesFile("../../database.properties");
//     if (!propertiesFile.is_open() || !propertiesFile.good()) {
//         std::cerr << "Error: no se pudo abrir o leer el archivo database.properties" << std::endl;
//         return;
//     }

//     // Comprobación de la estructura del archivo
//     std::string line;
//     while (std::getline(propertiesFile, line)) {
//         if (line.empty() || line.find('=') == std::string::npos) {
//             std::cerr << "Error: el archivo database.properties tiene un formato incorrecto" << std::endl;
//             return;
//         }
//         // ...
//     }

//     // Comprobación de la existencia de propiedades
//     if (properties.empty()) {
//         std::cerr << "Error: no se encontraron propiedades en el archivo database.properties" << std::endl;
//         return;
//     }
// }

void DatabaseManager::loadProperties() {
    std::ifstream propertiesFile("../../database.properties");
    if (propertiesFile.is_open()) {
        std::string line;
        while (std::getline(propertiesFile, line)) {
            size_t igual = line.find('=');
            if (igual != std::string::npos) {
                std::string clave = line.substr(0, igual);
                std::string valor = line.substr(igual + 1);
                properties[clave] = valor;
            }
        }
    } else {
        std::cerr << "Error al abrir el archivo de propiedades." << std::endl;
    }
}

// Conecta a la base de datos con los parámetros definidos en el header
bool DatabaseManager::connect() {
    if (!mysql_real_connect(conn, host.c_str(), user.c_str(), pass.c_str(), db.c_str(), port, NULL, 0)) {
        std::cerr << "Error al conectar a la base de datos: " << mysql_error(conn) << std::endl;
        return false;
    }
    std::cout << "Conexión exitosa a la base de datos '" << db << "'\n";
    return true;
}

// Ejecuta una consulta SQL y retorna true si no hay error
bool DatabaseManager::validateteQuery(const std::string& query) {
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Error al ejecutar la consulta: " << mysql_error(conn) << std::endl;
        return false;
    }
    return true;
}

// Inserta datos en la tabla indicada, usando columnas y valores dinámicos
// Uso: insertData("usuario", {"ID", "Nombre", "Apellido"}, {"123", "Juan", "Pérez"});
bool DatabaseManager::insertData(const std::string& tableName, 
                                const std::vector<std::string>& columns, 
                                const std::vector<std::string>& values) {
    if (columns.size() != values.size() || columns.empty()) {
        std::cerr << "Error: columnas y valores no coinciden o están vacíos.\n";
        return false;
    }
    std::string query = "INSERT INTO " + tableName + " (";
    for (size_t i = 0; i < columns.size(); ++i) {
        query += columns[i];
        if (i < columns.size() - 1) query += ", ";
    }
    query += ") VALUES (";
    for (size_t i = 0; i < values.size(); ++i) {
        query += "'" + values[i] + "'";
        if (i < values.size() - 1) query += ", ";
    }
    query += ")";
    std::cout << "Ejecutando: " << query << std::endl;
    if (validateteQuery(query)) {

        std::cout << "Datos insertados correctamente en " + tableName + ".\n";
        return true;
    } else {
        std::cerr << "Fallo al insertar datos.\n";
        return false;
    }
}

// Lee y muestra todos los datos de la tabla indicada
// Uso: readData("usuario");
void DatabaseManager::readData(const std::string& tableName) {
    std::string query = "SELECT * FROM " + tableName;
    if (validateteQuery(query)) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res) {
            MYSQL_ROW row;
            int num_fields = mysql_num_fields(res);
            MYSQL_FIELD *fields;
            fields = mysql_fetch_fields(res);

            std::cout << "\n--- Datos de la tabla '" << tableName << "' ---\n";
            
            while ((row = mysql_fetch_row(res))) {
                std::cout << "-------------------------------------\n";
                for (int i = 0; i < num_fields; i++) {                    
                    std::cout << fields[i].name << ": " << (row[i] ? row[i] : "NULL") << "\n";
                }
                std::cout << "\n";
            }
            std::cout << "--- Fin de datos de la tabla '" << tableName << "' ---\n";
            mysql_free_result(res);
        } else {
            std::cerr << "No se pudo obtener el resultado de la consulta: " << mysql_error(conn) << std::endl;
        }
    } else {
        std::cerr << "Fallo al leer datos.\n";
    }
}

// Ejecuta cualquier consulta SQL (SELECT, INSERT, UPDATE, etc.) y muestra resultados si los hay
// Uso: executeQuery("SELECT * FROM usuario WHERE ID='123'");
void DatabaseManager::executeQuery(const std::string& query) {
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Error al ejecutar la consulta SELECT: " << mysql_error(conn) << std::endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (res) {
        MYSQL_ROW row;
        int num_fields = mysql_num_fields(res);
        MYSQL_FIELD *fields = mysql_fetch_fields(res);

        std::cout << "\n--- Resultados de la Consulta Personalizada ---\n";
        if (mysql_num_rows(res) == 0) {
            std::cout << "No se encontraron resultados para la consulta.\n";
        } else {
            while ((row = mysql_fetch_row(res))) {
                std::cout << "-------------------------------------\n";
                for (int i = 0; i < num_fields; i++) {                    
                    std::cout << fields[i].name << ": " << (row[i] ? row[i] : "NULL") << "\n";
                }
                std::cout << "\n";
            }
        }
        std::cout << "--- Fin de resultados personalizados ---\n";
        mysql_free_result(res);
    } else if (mysql_field_count(conn) == 0) {                
        std::cout << "Consulta ejecutada con éxito (no es una consulta SELECT que devuelva resultados).\n";
    } else {
        std::cerr << "No se pudo obtener el resultado de la consulta: " << mysql_error(conn) << std::endl;
    }
}

std::string DatabaseManager::executeScalar(const std::string& query) {
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Error al ejecutar la consulta: " << mysql_error(conn) << std::endl;
        return "";
    }
    MYSQL_RES* res = mysql_store_result(conn);
    if (res) {
        MYSQL_ROW row = mysql_fetch_row(res);
        std::string result = (row && row[0]) ? row[0] : "";
        mysql_free_result(res);
        return result;
    }
    return "";
}


// Actualiza datos en la tabla indicada, usando columnas a modificar y condiciones dinámicas
// Uso: updateData("usuario", {{"Nombre", "Pedro"}}, {{"ID", "123"}});
bool DatabaseManager::updateData(const std::string& tableName, 
                                const std::map<std::string, std::string>& setColumns, 
                                const std::map<std::string, std::string>& whereConditions) {
    if (setColumns.empty() || whereConditions.empty()) {
        std::cerr << "Error: columnas a actualizar o condiciones vacías.\n";
        return false;
    }
    std::string query = "UPDATE " + tableName + " SET ";
    size_t count = 0;
    for (const auto& pair : setColumns) {
        query += pair.first + " = '" + pair.second + "'";
        if (++count < setColumns.size()) query += ", ";
    }
    query += " WHERE ";
    count = 0;
    for (const auto& pair : whereConditions) {
        query += pair.first + " = '" + pair.second + "'";
        if (++count < whereConditions.size()) query += " AND ";
    }
    std::cout << "Ejecutando: " << query << std::endl;
    if (validateteQuery(query)) {
        std::cout << "Datos actualizados correctamente en " + tableName + ".\n";
        return true;
    } else {
        std::cerr << "Fallo al actualizar datos.\n";
        return false;
    }
}

// Elimina datos de la tabla indicada según condiciones dinámicas
// Uso: deleteData("usuario", {{"ID", "123"}});
bool DatabaseManager::deleteData(const std::string& tableName, 
                                const std::map<std::string, std::string>& whereConditions) {
    if (whereConditions.empty()) {
        std::cerr << "Error: condiciones vacías para eliminar.\n";
        return false;
    }
    std::string query = "DELETE FROM " + tableName + " WHERE ";
    size_t count = 0;
    for (const auto& pair : whereConditions) {
        query += pair.first + " = '" + pair.second + "'";
        if (++count < whereConditions.size()) query += " AND ";
    }
    std::cout << "Ejecutando: " << query << std::endl;
    if (validateteQuery(query)) {
        std::cout << "Datos eliminados correctamente de " + tableName + ".\n";
        return true;
    } else {
        std::cerr << "Fallo al eliminar datos.\n";
        return false;
    }
}

// Verifica si un registro existe según una consulta dada
// Uso: existeRegistro("SELECT * FROM usuario WHERE ID='123'");
bool DatabaseManager::existsData(const std::string& query) {
    if (mysql_query(conn, query.c_str())) return false;
    MYSQL_RES* res = mysql_store_result(conn);
    bool existe = (res && mysql_num_rows(res) > 0);
    if (res) mysql_free_result(res);
    return existe;
}

std::vector<std::map<std::string, std::string>> DatabaseManager::fetchQuery(const std::string& query) {
    std::vector<std::map<std::string, std::string>> resultados;
    if (mysql_query(conn, query.c_str()) == 0) {
        MYSQL_RES* res = mysql_store_result(conn);
        if (res) {
            int num_fields = mysql_num_fields(res);
            MYSQL_FIELD* fields = mysql_fetch_fields(res);
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) {
                std::map<std::string, std::string> registro;
                for (int i = 0; i < num_fields; ++i) {
                    registro[fields[i].name] = row[i] ? row[i] : "";
                }
                resultados.push_back(registro);
            }
            mysql_free_result(res);
        }
    }
    return resultados;
}
