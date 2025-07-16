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
    -- ID           : 9876543210
    -- Nombre       : Ana
    -- Apellidoss   : García Marquéz
    -- Cuenta       : 34456405644712625074
    -- Saldo        : € 200.00
    -- PIN          : 0356

-- Cliente 2: 
    -- ID           : 8673067128
    -- Nombre       : Sofía 
    -- Apellidos    : Rodríguez Díaz
    -- Cuenta       : 65075323172501000371
    -- Saldo        : € 500.00
    -- PIN          : 7507

-- Cliente 3:            
    -- ID           : 9876544351           
    -- Nombre       : Mateo 
    -- Apellidos    : Gómez Hernández
    -- Cuenta       : 33484820856455254835
    -- Saldo        : e 670.00
    -- PIN          : 4348

-- Cliente 4:
    -- ID           : 9142023433 
    -- Nombre       : Ana Lucía 
    -- Apellidos    : Martínez García
    -- Cuenta       : 02727334257163704360
    -- Saldo        : €90.00
    -- PIN          : 1272

-- Cliente 5:
    -- ID           : 0551234556 
    -- Nombre       : Julián 
    -- Apellidos    : Sánchez López
    -- Cuenta 1     : 34405743285246242252
    -- Saldo        : €450.00
    -- PIN          : 4440
    -- Cuenta 2     : 50407324433180720406
    -- Saldo        : €0.00
    -- PIN          : 6040


-- Cliente 6:
    -- ID           : 8765435210
    -- Nombre       : Rosa Valeria
    -- Apellidos    : Díaz Pérez
    -- Cuenta       : 85342174015348210212
    -- Saldo        : €300.00
    -- PIN          : 9534

-- Cliente 7:
    -- ID           : 3421253456
    -- Nombre       : Gabriel
    -- Apellidos    : Hernández Rodríguez
    -- Cuenta       : 31033718272847655736
    -- Saldo        : €120.00
    -- PIN          : 4103

-- Cliente 8:
    -- ID           : 6543211098
    -- Nombre       : Hubert 
    -- Apellidos    : Van der Veen
    -- Cuenta 1     : 54100351721835683722
    -- Saldo        : €30.50
    -- PIN          : 6410
    -- Cuenta 2     : 33388774372350184264
    -- Saldo        : €0.00
    -- PIN          : 4338

-- Cliente 9:
    -- ID           : 2199836543 
    -- Nombre       : Hiroyuki 
    -- Apellidos    : Hyuuga
    -- Cuenta       : 51230386007261288161
    -- Saldo        : €90.00
    -- PIN          : 6123

-- Cliente 10:
    -- ID           : 7654302109
    -- Nombre       : Jhonatan
    -- Apellidos    : Gutemberger
    -- Cuenta       : 71484004323412537231
    -- Saldo        : €0.00
    -- PIN          : 8148