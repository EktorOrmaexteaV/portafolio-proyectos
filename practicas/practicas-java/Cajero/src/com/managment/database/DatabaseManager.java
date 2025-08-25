package com.managment.database;

import java.io.InputStream;
import java.sql.*;
import java.util.*;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * DatabaseManager.java
 * Clase para gestionar la conexión a la base de datos y realizar operaciones CRUD.
 * Adaptada para entorno GUI (sin impresión directa, devuelve datos como String o listas)
 * @autor Ektor Ormaetxea V
 */
public class DatabaseManager implements AutoCloseable {
    private static final Logger LOGGER = Logger.getLogger(DatabaseManager.class.getName());

    private Connection connection;

    private String DB_HOST;
    private String DB_USER;
    private String DB_PASS;
    private String DB_NAME;
    private int DB_PORT;
    private final String DB_URL;

    /**
     * Constructor que carga las propiedades de configuración y prepara la URL de conexión.
     */
    public DatabaseManager() {
        loadProperties();
        this.DB_URL = "jdbc:mysql://" + DB_HOST + ":" + DB_PORT + "/" + DB_NAME + "?useSSL=false&serverTimezone=UTC";
    }

    /**
     * Carga las propiedades desde config.properties.
     */
    private void loadProperties() {
        Properties prop = new Properties();
        try (InputStream input = getClass().getClassLoader().getResourceAsStream("config.properties")) {
            if (input == null) throw new RuntimeException("No se encontró el archivo de configuración.");
            prop.load(input);
            DB_HOST = prop.getProperty("db.host");
            DB_USER = prop.getProperty("db.user");
            DB_PASS = prop.getProperty("db.pass");
            DB_NAME = prop.getProperty("db.name");
            DB_PORT = Integer.parseInt(prop.getProperty("db.port"));
        } catch (Exception ex) {
            LOGGER.log(Level.SEVERE, "Error al cargar las propiedades: " + ex.getMessage(), ex);
            throw new RuntimeException("Error al cargar configuración", ex);
        }
    }

    /**
     * Establece conexión con la base de datos.
     * @return true si la conexión fue exitosa.
     */
    public boolean connect() {
        try {
            connection = DriverManager.getConnection(DB_URL, DB_USER, DB_PASS);
            LOGGER.info("Conexión exitosa a la base de datos");
            return true;
        } catch (SQLException e) {
            LOGGER.log(Level.SEVERE, "Error al conectar: " + e.getMessage(), e);
            return false;
        }
    }

    /**
     * Lee todos los datos de una tabla.
     * @param tableName Nombre de la tabla.
     * @return Lista de filas (mapas columna-valor).
     */
    public List<Map<String, String>> readData(String tableName) {
        return fetchQuery("SELECT * FROM " + tableName);
    }

    /**
     * Lee y devuelve los datos como texto.
     * @param tableName Nombre de la tabla.
     * @return Resultados como texto.
     */
    public String readDataAsString(String tableName) {
        return printQuery(readData(tableName));
    }

    /**
     * Convierte una lista de mapas en texto legible.
     * @param resultados Lista de resultados.
     * @return Cadena formateada.
     */
    public String printQuery(List<Map<String, String>> resultados) {
        if (resultados.isEmpty()) return "No hay resultados";

        StringBuilder result = new StringBuilder();
        for (Map<String, String> registro : resultados) {
            for (Map.Entry<String, String> par : registro.entrySet()) {
                result.append(par.getKey()).append(": ").append(par.getValue()).append("\n");
            }
            result.append("-------------------------\n");
        }
        return result.toString();
    }

    /**
     * Ejecuta una consulta SELECT y devuelve los resultados.
     * @param query Consulta SQL.
     * @return Lista de resultados.
     */
    public List<Map<String, String>> fetchQuery(String query) {
        List<Map<String, String>> resultados = new ArrayList<>();

        try (Statement stmt = connection.createStatement();
             ResultSet rs = stmt.executeQuery(query)) {

            ResultSetMetaData rsmd = rs.getMetaData();
            int numColumns = rsmd.getColumnCount();

            while (rs.next()) {
                Map<String, String> fila = new LinkedHashMap<>();
                for (int i = 1; i <= numColumns; i++) {
                    String nombreColumna = rsmd.getColumnLabel(i);
                    fila.put(nombreColumna, rs.getString(i));
                }
                resultados.add(fila);
            }
        } catch (SQLException e) {
            LOGGER.log(Level.SEVERE, "Error en fetchQuery: " + e.getMessage(), e);
        }

        return resultados;
    }

    /**
     * Inserta datos usando Map columna-valor.
     * @param tableName Tabla de destino.
     * @param columnValues Mapa columna-valor.
     * @return true si se insertó correctamente.
     */
    public boolean insertData(String tableName, Map<String, String> columnValues) {
        if (columnValues == null || columnValues.isEmpty()) return false;

        StringBuilder query = new StringBuilder("INSERT INTO ").append(tableName).append(" (");
        StringBuilder placeholders = new StringBuilder();

        List<String> keys = new ArrayList<>(columnValues.keySet());
        for (int i = 0; i < keys.size(); i++) {
            query.append(keys.get(i));
            placeholders.append("?");
            if (i < keys.size() - 1) {
                query.append(", ");
                placeholders.append(", ");
            }
        }
        query.append(") VALUES (").append(placeholders).append(")");

        LOGGER.info("Preparando: " + query.toString());

        try (PreparedStatement pstmt = connection.prepareStatement(query.toString())) {
            int index = 1;
            for (String key : keys) {
                pstmt.setString(index++, columnValues.get(key));
            }
            int rowsAffected = pstmt.executeUpdate();
            LOGGER.info("Datos insertados correctamente en " + tableName + ". Filas afectadas: " + rowsAffected);
            return rowsAffected > 0;
        } catch (SQLException e) {
            LOGGER.log(Level.SEVERE, "Fallo al insertar datos: " + e.getMessage(), e);
            return false;
        }
    }

    /**
     * Inserta datos usando listas columna-valor.
     * @param tableName Tabla destino.
     * @param columns Lista de columnas.
     * @param values Lista de valores.
     * @return true si se insertó correctamente.
     */
    public boolean insertDataByList(String tableName, List<String> columns, List<String> values) {
        if (columns.size() != values.size() || columns.isEmpty()) {
            LOGGER.severe("Error: columnas y valores no coinciden o están vacíos.");
            return false;
        }

        StringBuilder query = new StringBuilder("INSERT INTO ").append(tableName).append(" (");
        for (int i = 0; i < columns.size(); i++) {
            query.append(columns.get(i));
            if (i < columns.size() - 1) query.append(", ");
        }
        query.append(") VALUES (");
        for (int i = 0; i < values.size(); i++) {
            query.append("?");
            if (i < values.size() - 1) query.append(", ");
        }
        query.append(")");

        LOGGER.info("Preparando: " + query.toString());

        try (PreparedStatement pstmt = connection.prepareStatement(query.toString())) {
            for (int i = 0; i < values.size(); i++) {
                pstmt.setString(i + 1, values.get(i));
            }
            int rowsAffected = pstmt.executeUpdate();
            LOGGER.info("Datos insertados correctamente en " + tableName + ". Filas afectadas: " + rowsAffected);
            return rowsAffected > 0;
        } catch (SQLException e) {
            LOGGER.log(Level.SEVERE, "Fallo al insertar datos: " + e.getMessage(), e);
            return false;
        }
    }

    /**
     * Elimina datos según condiciones.
     * @param tableName Tabla objetivo.
     * @param whereConditions Condiciones WHERE.
     * @return true si se eliminó al menos una fila.
     */
    public boolean deleteData(String tableName, Map<String, String> whereConditions) {
        if (whereConditions == null || whereConditions.isEmpty()) return false;

        StringBuilder query = new StringBuilder("DELETE FROM ").append(tableName).append(" WHERE ");
        List<String> keys = new ArrayList<>(whereConditions.keySet());

        for (int i = 0; i < keys.size(); i++) {
            query.append(keys.get(i)).append(" = ?");
            if (i < keys.size() - 1) query.append(" AND ");
        }

        try (PreparedStatement pstmt = connection.prepareStatement(query.toString())) {
            int index = 1;
            for (String key : keys) {
                pstmt.setString(index++, whereConditions.get(key));
            }
            return pstmt.executeUpdate() > 0;
        } catch (SQLException e) {
            LOGGER.log(Level.SEVERE, "Error en deleteData: " + e.getMessage(), e);
            return false;
        }
    }

    /**
     * Actualiza datos con condiciones.
     * @param tableName Nombre de la tabla.
     * @param setColumns Nuevos valores.
     * @param whereConditions Condiciones WHERE.
     * @return true si se actualizó al menos una fila.
     */
    public boolean updateData(String tableName, Map<String, String> setColumns, Map<String, String> whereConditions) {
        if (setColumns == null || setColumns.isEmpty() || whereConditions == null || whereConditions.isEmpty()) {
            return false;
        }

        StringBuilder query = new StringBuilder("UPDATE ").append(tableName).append(" SET ");
        List<String> setKeys = new ArrayList<>(setColumns.keySet());
        for (int i = 0; i < setKeys.size(); i++) {
            query.append(setKeys.get(i)).append(" = ?");
            if (i < setKeys.size() - 1) query.append(", ");
        }

        query.append(" WHERE ");
        List<String> whereKeys = new ArrayList<>(whereConditions.keySet());
        for (int i = 0; i < whereKeys.size(); i++) {
            query.append(whereKeys.get(i)).append(" = ?");
            if (i < whereKeys.size() - 1) query.append(" AND ");
        }

        LOGGER.info("Ejecutando UPDATE: " + query.toString());

        try (PreparedStatement pstmt = connection.prepareStatement(query.toString())) {
            int index = 1;
            for (String key : setKeys) {
                pstmt.setString(index++, setColumns.get(key));
            }
            for (String key : whereKeys) {
                pstmt.setString(index++, whereConditions.get(key));
            }
            int rowsAffected = pstmt.executeUpdate();
            LOGGER.info("Filas actualizadas: " + rowsAffected);
            return rowsAffected > 0;
        } catch (SQLException e) {
            LOGGER.log(Level.SEVERE, "Error en updateData: " + e.getMessage(), e);
            return false;
        }
    }

    /**
     * Ejecuta cualquier consulta SQL y devuelve un resultado como String.
     * @param query Consulta a ejecutar.
     * @return Resultado o mensaje.
     */
    public String executeQuery(String query) {
        String trimmedQuery = query.trim().toUpperCase();
        boolean isSelect = trimmedQuery.startsWith("SELECT");

        if (isSelect) {
            List<Map<String, String>> resultados = fetchQuery(query);
            return printQuery(resultados);
        } else {
            try (Statement stmt = connection.createStatement()) {
                int rowsAffected = stmt.executeUpdate(query);
                return "Consulta ejecutada correctamente. Filas afectadas: " + rowsAffected;
            } catch (SQLException e) {
                LOGGER.log(Level.SEVERE, "Error al ejecutar la consulta: " + e.getMessage(), e);
                return "Error al ejecutar la consulta: " + e.getMessage();
            }
        }
    }

    /**
     * Ejecuta una consulta que devuelve solo un valor.
     * @param query Consulta tipo escalar.
     * @return Primer valor encontrado.
     */
    public String executeScalar(String query) {
        try (Statement stmt = connection.createStatement();
             ResultSet rs = stmt.executeQuery(query)) {
            if (rs.next()) {
                return rs.getString(1);
            }
        } catch (SQLException e) {
            LOGGER.log(Level.SEVERE, "Error al ejecutar la consulta escalar: " + e.getMessage(), e);
        }
        return "";
    }

    /**
     * Cierra la conexión con la base de datos.
     */
    @Override
    public void close() throws SQLException {
        if (connection != null && !connection.isClosed()) {
            connection.close();
            LOGGER.info("Conexión cerrada.");
        }
    }
}
