-- =====================================
-- Eliminar usuario y cuenta ejemplo
-- =====================================

-- Selecciona la base de datos
USE cajero;

-- 1. Eliminar todas las transacciones relacionadas con el usuario o la cuenta
DELETE FROM transaccion
WHERE ID_Usuario = '1234567890'
   OR ID_Cuenta = '11111111111111111111';

-- 2. Eliminar la cuenta específica
DELETE FROM cuenta
WHERE ID = '11111111111111111111';

-- 3. Eliminar el usuario (solo si ya no tiene cuentas asociadas)
DELETE FROM usuario
WHERE ID = '1234567890';


-- =====================================
-- Otras consultas
-- =====================================

-- Consulta final para verificar saldo
SELECT ID, ID_Usuario, monto 
FROM cuenta 
WHERE ID = '11111111111111111111';

-- Consulta de usuarios y sus cuentas
SELECT 
    u.ID AS Usuario_ID,
    CONCAT(u.Nombre, ' ', u.Apellido) AS Usuario_Nombre,
    c.ID AS Cuenta_ID,
    c.monto AS Saldo
FROM usuario u
LEFT JOIN cuenta c ON u.ID = c.ID_Usuario
ORDER BY u.ID, c.ID;        

-- Consulta del historial de movimientos de dinero
SELECT	
    CONCAT(u.Nombre, ' ', u.Apellido) AS nombre_completo,
    c.ID AS num_cuenta,
    t.monto
FROM
    transaccion t
JOIN 
    usuario u ON t.ID_Usuario = u.ID
JOIN 
    cuenta c ON c.ID_Usuario=u.ID
WHERE
    c.ID = '11111111111111111111'
    AND t.tipo_evento IN ('INGRESO', 'RETIRO', 'CONSULTA')
ORDER by
	t.fecha;

-- Consulta de transacciones por usuario
SELECT 
    u.ID AS Usuario_ID,
    CONCAT(u.Nombre, ' ', u.Apellido) AS Usuario_Nombre,        
    t.ID AS Transaccion_ID,
    t.ID_Cuenta AS Cuenta_ID,
    t.monto AS Monto,
    t.tipo_evento AS Tipo_Evento,
    t.fecha AS Fecha
FROM transaccion t
JOIN usuario u ON t.ID_Usuario = u.ID
ORDER BY u.ID, t.ID;


-- =====================================
-- Ejemplos de usuarios para pruebas
-- =====================================

-- Cliente 1:
    -- ID       : 9876543210
    -- Nombre   : Ana María
    -- Apellidos: García Marquéz
    -- Cuenta   : 34456405644712625074
    -- PIN      : 0356
    -- Saldo    : 200

-- Cliente 2: 
    -- ID: 123456789 
    -- Nombre: Sofía 
    -- Apellidos: Rodríguez Díaz

-- Cliente 3: 
    -- ID: 9876543 
    -- Nombre   : Mateo 
    -- Apellidos: Gómez Hernández
    

-- Cliente 4: 
    -- ID: 111222333 
    -- Nombre: Lucía 
    -- Apellidos: Martínez García

-- Cliente 5: 
    -- ID: 555123456 
    -- Nombre: Julián 
    -- Apellidos: Sánchez López

-- Cliente 6: 
    -- ID: 876543210 
    -- Nombre: Valeria 
    -- Apellidos: Díaz Pérez

-- Cliente 7: 
    -- ID: 342123456 
    -- Nombre: Gabriel 
    -- Apellidos: Hernández Rodríguez

-- Cliente 8: 
    -- ID: 654321098 
    -- Nombre: Isabella 
    -- Apellidos: Gómez Sánchez

-- Cliente 9: 
    -- ID: 219876543 
    -- Nombre: Alejandro 
    -- Apellidos: Martínez Díaz

-- Cliente 10: 
    -- ID: 765432109 
    -- Nombre: Daniela 
    -- Apellidos: López García