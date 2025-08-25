# Simulador de Cajero Automático en C++ con Base de Datos
## Versión mejorada usando MySQL

Este proyecto es una **versión mejorada** del [Cajero en C++ con archivos planos](../Cajero), donde se reemplazó el manejo de archivos de texto por una **base de datos MySQL** para un control más robusto y escalable.

> Proyecto desarrollado como práctica para reforzar conceptos de programación orientada a objetos, conexión a bases de datos y modularización de código en C++.

---

## Funcionalidades

- Inicio de sesión con número de cuenta y PIN.
- Consultar saldo.
- Depositar dinero.
- Retirar dinero (validación de fondos).
- Cambio de PIN.
- Registro de operaciones en base de datos y logs.
- Configuración externa vía archivo `database.properties`.

---

## Estructura del Proyecto

```
CajeroBD/
├── src/
│ ├── Cajero/
│ │ ├── Cajero.cpp
│ │ └── Cajero.h
│ ├── DatabaseManager/
│ │ ├── DatabaseManager.cpp # Manejo de conexión y consultas a MySQL
│ │ └── DatabaseManager.h
│ └── utils/
│ ├── utils.cpp
│ └── utils.h
├── BD SQL/
│ ├── configuracion_bd_cajero.sql # Script de creación de la BD
│ ├── configuracion_cuenta_ejemplo.sql # Script de datos iniciales
│ └── otras_consultas.sql # Ejemplos adicionales
├── database.properties # Archivo con parámetros de conexión
├── main # Ejecutable compilado
└── README.md
```

---

## Compilación y ejecución

### Requisitos

- Compilador de C++ (g++, clang, etc.)
- Sistema compatible con terminal (Linux, macOS o Windows con WSL/MinGW)
- Servidor MySQL
- (Opcional) Code::Blocks o VS Code

### Compilación desde terminal

```bash
cd src
g++ Cajero/*.cpp DatabaseManager/*.cpp Utils/*.cpp main.cpp -o ../main -lmysqlclient
```

### Ejecución

```bash
cd ..
./main
```

---

## Creación de la base de datos y tablas

```sql
-- Creación de la base de datos y selección
CREATE DATABASE IF NOT EXISTS cajero;
USE cajero;

-- Tabla `usuario`
CREATE TABLE usuario (
    ID VARCHAR(20) PRIMARY KEY,
    Nombre VARCHAR(50) NOT NULL,
    Apellido VARCHAR(50) NOT NULL,
    fecha_alta TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Tabla `cuenta`
CREATE TABLE cuenta (
    ID VARCHAR(50) PRIMARY KEY,
    ID_Usuario VARCHAR(20) NOT NULL,
    pin VARCHAR(10) NOT NULL,
    monto DECIMAL(12,2) NOT NULL DEFAULT 0.00 CHECK (monto <= 10000.99),
    fecha_creacion TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (ID_Usuario) REFERENCES usuario(ID) ON DELETE RESTRICT
);

-- Tabla `transaccion`
CREATE TABLE transaccion (
    ID INT AUTO_INCREMENT PRIMARY KEY,
    ID_Cuenta VARCHAR(50),
    ID_Usuario VARCHAR(20) NOT NULL,
    tipo_evento ENUM(
        'ALTA_USUARIO',
        'CREACION_CUENTA',
        'INICIO_SESION',        
        'CIERRE_SESION',
        'INGRESO',
        'RETIRO',
        'CONSULTA',
        'CAMBIO_PIN',
        'INTENTO_FALLIDO',
        'CAMBIO_PIN_FALLIDO',
        'TRANSACCION_RECHAZADA'
    ) NOT NULL,
    monto DECIMAL(12,2),
    descripcion TEXT,
    fecha TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (ID_Cuenta) REFERENCES cuenta(ID) ON DELETE SET NULL,
    FOREIGN KEY (ID_Usuario) REFERENCES usuario(ID) ON DELETE RESTRICT
);

-- Triggers
-- Trigger para registrar automáticamente el alta de usuario
DELIMITER //
CREATE TRIGGER after_usuario_insert
AFTER INSERT ON usuario
FOR EACH ROW
BEGIN
    INSERT INTO transaccion (ID_Usuario, tipo_evento, descripcion)
    VALUES (NEW.ID, 'ALTA_USUARIO', CONCAT('Alta de usuario: ', NEW.Nombre, ' ', NEW.Apellido));
END//
DELIMITER ;

-- Trigger para registrar la creación de cuentas
DELIMITER //
CREATE TRIGGER after_cuenta_insert
AFTER INSERT ON cuenta
FOR EACH ROW
BEGIN
    -- Registrar creación de cuenta
    INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
    VALUES (NEW.ID, NEW.ID_Usuario, 'CREACION_CUENTA', NEW.monto, 'Apertura de cuenta con saldo inicial');
END//
DELIMITER ;

-- Trigger para actualizar saldo en retiros
DELIMITER //
CREATE TRIGGER before_retiro_update
BEFORE UPDATE ON cuenta
FOR EACH ROW
BEGIN
    IF NEW.monto < 0 THEN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'El saldo no puede ser negativo';
    END IF;
END//
DELIMITER ;
```

---

## Mejoras respecto a la versión anterior
- [x] Sustitución de archivos de texto por MySQL
- [x] Gestión modular (clases separadas en carpetas).
- [x] Uso de triggers para registrar transacciones en la base de datos.
---

## Posibles mejoras
- [x] Uso de bases de datos en vez de texto plano
- [ ] Intepretación al lenguaje JAVA.
- [ ] Intepretación al lenguaje JAVA con entorno gráfico.
---

## Autor
**[Ektor Ormaetxea V](https://github.com/EktorOrmaexteaV)**

---

### Licencia
Proyecto libre para fines educativos y personales.
