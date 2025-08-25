# Simulador de Cajero Automático en Java (GUI + MySQL)

Este proyecto es la **versión gráfica en Java** del simulador de cajero automático. Se desarrolló usando **Java Swing** y un enfoque modular con POO, integrando MySQL como base de datos y generando reportes en PDF.

> Proyecto desarrollado para practicar Java, GUI, gestión de bases de datos y modularización avanzada.

---

## Funcionalidades

- Login con número de cuenta y PIN.
- Consultar saldo.
- Depósitos.
- Retiros con validación de fondos.
- Cambio de PIN.
- Reporte en PDF de transacciones.
- Manejo de configuración vía `config.properties`.
---

## Estructura del Proyecto

```
Cajero/
├── src/
│ ├── com/cajero/
│ │ ├── Cajero.java
│ │ └── gui/
│ │ ├── Alta.java
│ │ ├── Cuenta.java
│ │ ├── Datos.java
│ │ ├── Inicio.java
│ │ ├── Main.java
│ │ ├── PIN.java
│ │ └── Reporte.java
│ └── com/managment/
│ ├── database/DatabaseManager.java
│ ├── filemanager/FileManager.java
│ └── utils/
│ ├── InputUtils.java
│ └── SystemUtils.java
├── sql/
│ ├── configuracion_bd_cajero.sql
│ ├── configuracion_cuenta_ejemplo.sql
│ └── otras_consultas.sql
├── lib/ # Dependencias externas
│ ├── io-7.1.4.jar
│ ├── kernel-7.1.4.jar
│ ├── layout-7.1.4.jar
│ ├── slf4j-api-2.0.17.jar
│ └── slf4j-simple-2.0.17.jar
├── config.properties # Configuración de conexión a BD
├── .classpath / .project # Archivos de Eclipse
└── README.md
```

---

## Requisitos y Ejecución

### Requisitos
- Java 17+ (o superior)
- IDE compatible con Java y entornos GUI.
- MySQL Server
- Bibliotecas incluidas en `/lib`

### Bibliotecas incluidas

- **iText 7.1.4 (PDF)**  
  Necesaria para la generación de reportes en PDF.  
  Descarga los módulos (`io-7.1.4.jar`, `kernel-7.1.4.jar`, `layout-7.1.4.jar`) desde:  
  [iText 7.1.4 - SourceForge](https://sourceforge.net/projects/itext/files/7.1.4/iText7-Core-7.1.4.zip/download)

- **SLF4J 2.0.17 (Logging)**  
  Utilizado para manejo de logs y compatibilidad con iText.  
  Descarga los artefactos (`slf4j-api-2.0.17.jar`, `slf4j-simple-2.0.17.jar`) desde:  
  [SLF4J 2.0.17 - slf4j.org](https://www.slf4j.org/dist/slf4j-2.0.17.zip)

- **MySQL Connector/J (JDBC Driver)**  
  Necesario para la conexión del proyecto a la base de datos MySQL.  
  Descarga el `.jar` (`mysql-connector-j-9.1.0.jar` o la versión más reciente) desde:  
  [MySQL Connector/J - dev.mysql.com](https://dev.mysql.com/downloads/connector/j/)




### Pasos
1. Importar la base de datos:
   ```bash
   mysql -u root -p < sql/configuracion_bd_cajero.sql
   mysql -u root -p < sql/configuracion_cuenta_ejemplo.sql
   ```
2. Configurar el archivo config.properties:
   ```
    host=localhost # O la IP del servidor
    port=3306
    user=tu_usuario 
    password=tu_contraseña
    database=cajero_db # Nombre de la base de datos creada 
   ```
3. Importar el proyecto en el IDE.
4. Ejecutar el proyecto.

## Mejoras respecto a la versión anterior
- [x] Uso de bases de datos en vez de archivos de texto.
- [x] Intepretación al lenguaje JAVA.
- [x] Intepretación al lenguaje JAVA con entorno gráfico.

## Posibles mejoras
- [ ] Interpretación a otro lenguaje con entorno gráfico.
- [ ] Uso de aplicación web con entorno gráfico y bases de datos.


## Autor
**[Ektor Ormaetxea V](https://github.com/EktorOrmaexteaV)**

---

### Licencia
Proyecto libre para fines educativos y personales.