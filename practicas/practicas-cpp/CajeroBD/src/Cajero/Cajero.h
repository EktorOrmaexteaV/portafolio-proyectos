//cajero.h
#ifndef CAJERO_H
#define CAJERO_H

#include <string>
#include <iostream>
#include "../DatabaseManager/DatabaseManager.h"
#include "../utils/utils.h"

// Clase principal para la lógica del cajero automático
class Cajero {
    private:
    std::string tuNombre;
    std::string tuApellido;
    // Nombres de tablas        
    std::string tablaUsuarios;                          // Tabla de usuarios
    std::string tablaCuentas;                           // Tabla de cuentas
    std::string tablaMovimientos;                       // Tabla de transacciones/movimientos

    // Columnas de la tabla Usuarios
    std::string tuID;
        std::string tuFechaAlta;

        // Columnas de la tabla Cuentas
        std::string tcID;
        std::string tcUsuario;
        std::string tcPIN;
        std::string tcSaldo;
        std::string tcFechaCreacion;

        // Columnas de la tabla Movimientos
        std::string tmID;
        std::string tmCuenta;
        std::string tmUsuario;
        std::string tmDescripcion;
        std::string tmTimestamp;

        // Variables de sesión
        std::string cuentaSesion;    // ID de la cuenta con sesión iniciada
        std::string IDusuarioSesion; // ID del usuario de la cuenta
        std::string usuarioSesion;   // Nombre del usuario de la cuenta
        std::string pinSesion;       // PIN de la cuenta con sesión iniciada
        float saldoSesion;           // Saldo actual
        bool sesionActiva = false;   // Indica si hay sesión activa

    public:
        // Constructor y destructor
        Cajero();
        ~Cajero();

        // Métodos principales del cajero        
        bool iniciarSesion();                                   // Iniciar sesión de usuario
        bool iniciarSesion(const std::string& cuenta);          // Iniciar sesión de usuario, con cuenta y PIN
        //bool confirmarCliente(const std::string& cliente);      // Verifica si el cliente existe
        bool confirmarCuenta(const std::string& cuenta);        // Verifica si la cuenta existe
        void cerrarSesion();                                    // Cerrar sesión de usuario
        //bool altaClienteConCuenta();                            // Dar de alta un nuevo cliente con cuenta  
        //bool altaCliente(std::string IDCliente);                // Dar de alta un nuevo cliente
        

        void altaCliente(std::string& IDCliente);                       // Dar de alta un nuevo cliente
        void abrirCuenta(std::string cliente);                          // Crear una nueva cuenta

        void cambiarPIN(std::string PINactual, std::string cuenta, 
                        int CuentaNueva);                               // Cambiar PIN de una cuenta
        void consultarSaldo();                                          // Consultar saldo de una cuenta
        void consultarSaldo(std::string cuenta);                        // Consultar saldo de una cuenta específica                       
        //void realizarDeposito();                                      // Realizar un depósito
        void realizarDeposito(std::string cuenta);                      // Realizar un depósito en una cuenta particular
        //void realizarRetiro();                                        // Realizar un retiro
        void realizarRetiro(std::string cuenta);                        // Realizar un retiro en una cuenta particular        
        bool isSesionActiva() const;                                    // Verificar si hay sesión activa

        //Métodos para registrar movimientos
        void registrarMovimiento(const std::string& detalle, double monto = 0.0,int retiro=0);
        void registrarEventos(const std::string& tipoEvento, const std::string& detalle);
        void registrarEventos(const std::string& tipoEvento, double monto, const std::string& detalle);

        // Getters
        std::string getTablaUsuarios();
        std::string getTablaCuentas();
        std::string getTablaMovimientos();

        std::string getTuID();
        std::string getTuNombre();
        std::string getTuApellido();
        std::string getTuFechaAlta();

        std::string getTcID();
        std::string getTcUsuario();
        std::string getTcPIN();        
        std::string getTcSaldo();
        std::string getTcFechaCreacion();    

        std::string getTmID();        
        std::string getTmCuenta();
        std::string getTmUsuario();
        std::string getTmDescripcion();
        std::string getTmTimestamp();

        // Getters para variables de sesión
        std::string getCuentaSesion() const;
        std::string getIDUsuarioSesion() const;
        std::string getUsuarioSesion() const;
        std::string getPinSesion() const;
        float getSaldoSesion() const;
        
        // Metodo para generar reporte
        void generarReporte(std::string ususario);
};
void PruebaDB();

//Funciones de utilidad
void reiniciarSesion(Cajero& c);
//void cambiarPIN(std::string PINactual, std::string cuenta, int CuentaNueva);
bool confirmarPIN(Cajero& c, const std::string& cuenta, const std::string mensaje);

std::string eventos(int num);
#endif

