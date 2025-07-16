-- =====================================
-- Creacion de usuario y cuenta ejemplo
-- =====================================
-- Selecciona la base de datos
USE cajero;
-- Creación de usuario
-- 1. Alta de cliente Pedro Pérez
INSERT INTO usuario (ID, Nombre, Apellido) 
VALUES ('1234567890', 'Pedro', 'Pérez');

-- 2. Apertura de cuenta con €1000
INSERT INTO cuenta (ID, ID_Usuario, pin, monto)
VALUES ('11111111111111111111', '1234567890', '1111', 1000.00);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            

-- =====================================
-- Registros de transacciones
-- =====================================
-- Inicio sesión
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'INICIO_SESION', NULL, 'Sesion iniciada');

-- Ingreso de €50.00
-- Actualizar el saldo de la cuenta (aumentar)
UPDATE cuenta
SET monto = monto + 50.00
WHERE ID = '11111111111111111111';

-- Registrar el ingreso en la tabla de transacciones
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'INGRESO', 50.00, 'Depósito de €50.00');

-- Registrar la consulta en la tabla de transacciones (Nuevo saldo: 1000 + 50 = 1050)
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CONSULTA', 1050.00, 'Consulta saldo');


-- Retiro de €10.00
-- Actualizar el saldo de la cuenta (disminuir)
UPDATE cuenta
SET monto = monto - 10.00
WHERE ID = '11111111111111111111';

-- Registrar el retiro en la tabla de transacciones
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'RETIRO', -10.00, 'Retiro de €-10.00');

-- Registrar la consulta en la tabla de transacciones (Nuevo saldo: 1050 - 10 = 1040)
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CONSULTA', 1040.00, 'Consulta saldo');


-- Cierre de Sesión
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CIERRE_SESION', NULL, 'Sesion cerrada');


-- Inicio sesión
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'INICIO_SESION', NULL, 'Sesion iniciada');

-- Cambio de PIN
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CAMBIO_PIN', NULL, 'Pin cambiado satisfactoriamente');

-- Cierre de Sesión
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CIERRE_SESION', NULL, 'Sesion cerrada');


-- Inicio sesión
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'INICIO_SESION', NULL, 'Sesion iniciada');

-- Ingreso de €120.00
-- Actualizar el saldo de la cuenta (aumentar)
UPDATE cuenta
SET monto = monto + 120.00
WHERE ID = '11111111111111111111';

-- Registrar el ingreso en la tabla de transacciones
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'INGRESO', 120.00, 'Depósito de €120.00');

-- Registrar la consulta en la tabla de transacciones (Nuevo saldo: 1040 + 120 = 1160)
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CONSULTA', 1160.00, 'Consulta saldo');

-- Cambio de PIN
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CAMBIO_PIN', NULL, 'Pin cambiado satisfactoriamente');

-- Cierre de Sesión
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CIERRE_SESION', NULL, 'Sesion cerrada');


-- Inicio sesión
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'INICIO_SESION', NULL, 'Sesion iniciada');

-- Retiro de €75.00
-- Actualizar el saldo de la cuenta (disminuir)
UPDATE cuenta
SET monto = monto - 75.00
WHERE ID = '11111111111111111111';

-- Registrar el retiro en la tabla de transacciones
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'RETIRO', -75.00, 'Retiro de €-75.00');

-- Registrar la consulta en la tabla de transacciones (Nuevo saldo: 1160 - 75 = 1085)
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CONSULTA', 1085.00, 'Consulta saldo');


-- Retiro de €25.50
-- Actualizar el saldo de la cuenta (disminuir)
UPDATE cuenta
SET monto = monto - 25.50
WHERE ID = '11111111111111111111';

-- Registrar el retiro en la tabla de transacciones
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'RETIRO', -25.50, 'Retiro de €-25.50');

-- Registrar la consulta en la tabla de transacciones (Nuevo saldo: 1085 - 25.50 = 1059.50)
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CONSULTA', 1059.50, 'Consulta saldo');


-- Ingreso de €8.75
-- Actualizar el saldo de la cuenta (aumentar)
UPDATE cuenta
SET monto = monto + 8.75
WHERE ID = '11111111111111111111';

-- Registrar el ingreso en la tabla de transacciones
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'INGRESO', 8.75, 'Depósito de €8.75');

-- Registrar la consulta en la tabla de transacciones (Nuevo saldo: 1059.50 + 8.75 = 1068.25)
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CONSULTA', 1068.25, 'Consulta saldo');

-- Cambio de PIN
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CAMBIO_PIN', NULL, 'Pin cambiado satisfactoriamente');

-- Cierre de Sesión
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CIERRE_SESION', NULL, 'Sesion cerrada');


-- Inicio sesión
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'INICIO_SESION', NULL, 'Sesion iniciada');

-- Retiro de €3.20
-- Actualizar el saldo de la cuenta (disminuir)
UPDATE cuenta
SET monto = monto - 3.20
WHERE ID = '11111111111111111111';

-- Registrar el retiro en la tabla de transacciones
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'RETIRO', -3.20, 'Retiro de €-3.20');

-- Registrar la consulta en la tabla de transacciones (Nuevo saldo: 1068.25 - 3.20 = 1065.05)
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CONSULTA', 1065.05, 'Consulta saldo');

-- Cierre de Sesión
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CIERRE_SESION', NULL, 'Sesion cerrada');


-- Ingreso de €45.99
-- Actualizar el saldo de la cuenta (aumentar)
-- 1. Aumentar el saldo en la cuenta
UPDATE cuenta 
SET monto = monto + 45.99
WHERE ID = '11111111111111111111';

-- 2. Registrar el ingreso
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'INGRESO', 45.99, 'Depósito de €45.99');

-- 3. Obtener el saldo actualizado
-- (Esto se hace idealmente en el código backend, pero aquí asumimos el nuevo saldo manualmente)

-- 4. Registrar la consulta de saldo
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CONSULTA', 1111.04, 'Consulta saldo');


-- Retiro de €150.00
-- Actualizar el saldo de la cuenta (disminuir)
UPDATE cuenta
SET monto = monto - 150.00
WHERE ID = '11111111111111111111';

-- Registrar el retiro en la tabla de transacciones
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'RETIRO', -150.00, 'Retiro de €-150.00');

-- Registrar la consulta en la tabla de transacciones (Nuevo saldo: 1111.04 - 150.00 = 961.04)
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CONSULTA', 961.04, 'Consulta saldo');


-- Ingreso de €0.01
-- Actualizar el saldo de la cuenta (aumentar)
UPDATE cuenta
SET monto = monto + 0.01
WHERE ID = '11111111111111111111';

-- Registrar el ingreso en la tabla de transacciones
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'INGRESO', 0.01, 'Depósito de €0.01');

-- Registrar la consulta en la tabla de transacciones (Nuevo saldo: 961.04 + 0.01 = 961.05)
INSERT INTO transaccion (ID_Cuenta, ID_Usuario, tipo_evento, monto, descripcion)
VALUES ('11111111111111111111', '1234567890', 'CONSULTA', 961.05, 'Consulta saldo');