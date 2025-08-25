-- Script de creación de la base de datos y tablas
-- cajero_setup.sql

-- Elimina la base de datos
DROP DATABASE cajero;

-- Creación de la base de datos
CREATE DATABASE IF NOT EXISTS cajero;
USE cajero;

-- Tabla de Usuarios
CREATE TABLE usuario (
    ID VARCHAR(20) PRIMARY KEY,
    Nombre VARCHAR(50) NOT NULL,
    Apellido VARCHAR(50) NOT NULL,
    fecha_alta TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Tabla de Cuentas
CREATE TABLE cuenta (
    ID VARCHAR(50) PRIMARY KEY,
    ID_Usuario VARCHAR(20) NOT NULL,
    pin VARCHAR(10) NOT NULL,
    monto DECIMAL(12,2) NOT NULL DEFAULT 0.00 CHECK (monto <= 10000.99),
    fecha_creacion TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (ID_Usuario) REFERENCES usuario(ID) ON DELETE RESTRICT
);

-- Tabla de Transacciones/Eventos
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
