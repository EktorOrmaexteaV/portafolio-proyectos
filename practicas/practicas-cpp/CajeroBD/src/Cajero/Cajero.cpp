//cajero.cpp
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include "../DatabaseManager/DatabaseManager.h"
#include "../utils/utils.h"
#include "Cajero.h"
#include <iomanip>

DatabaseManager dbm;
float MONTO_MAX=1000;

Cajero::Cajero(){
    tablaUsuarios = "usuario";
    tuID = "ID";
    tuNombre = "Nombre";
    tuApellido = "Apellido";
    tuFechaAlta = "fecha_alta";

    tablaCuentas = "cuenta";
    tcID = "ID";
    tcUsuario = "ID_Usuario";
    tcPIN = "pin";
    tcSaldo = "monto";
    tcFechaCreacion = "fecha_creacion";

    tablaMovimientos = "transaccion";
    tmID = "ID";
    tmCuenta = "ID_Cuenta";
    tmUsuario = "ID_Usuario";
    tmDescripcion = "descripcion";
    tmTimestamp = "fecha";

    if (!dbm.connect()) {
        std::cerr << "No se pudo conectar a la base de datos. Saliendo...\n";
        exit(EXIT_FAILURE);
    }
}

Cajero::~Cajero() {
    // Destructor vacío, la conexión se cierra en DatabaseManager
}

// Getters para nombres de tablas
std::string Cajero::getTablaUsuarios() { return tablaUsuarios; }
std::string Cajero::getTablaCuentas() { return tablaCuentas; }
std::string Cajero::getTablaMovimientos() { return tablaMovimientos; }

// Getters para columnas de usuario
std::string Cajero::getTuID() { return tuID; }
std::string Cajero::getTuNombre() { return tuNombre; }
std::string Cajero::getTuApellido() { return tuApellido; }
std::string Cajero::getTuFechaAlta() { return tuFechaAlta; }

// Getters para columnas de cuenta
std::string Cajero::getTcID() { return tcID; }
std::string Cajero::getTcUsuario() { return tcUsuario; }
std::string Cajero::getTcPIN() { return tcPIN; }
std::string Cajero::getTcSaldo() { return tcSaldo; }
std::string Cajero::getTcFechaCreacion() { return tcFechaCreacion; }

// Getters para columnas de movimientos
std::string Cajero::getTmID() { return tmID; }
std::string Cajero::getTmCuenta() { return tmCuenta; }
std::string Cajero::getTmUsuario() { return tmUsuario; }
std::string Cajero::getTmDescripcion() { return tmDescripcion; }
std::string Cajero::getTmTimestamp() { return tmTimestamp; }

// Getters para variables de sesión
std::string Cajero::getCuentaSesion() const { return cuentaSesion; }        //getter para la cuenta
std::string Cajero::getIDUsuarioSesion() const { return IDusuarioSesion; }  //getter para el DNI/NIE del usuario
std::string Cajero::getUsuarioSesion() const { return usuarioSesion; }      //getter para el nombre completo
std::string Cajero::getPinSesion() const { return pinSesion; }              //getter para el PIN
float Cajero::getSaldoSesion() const { return saldoSesion; }                //getter para el saldo
bool Cajero::isSesionActiva() const { return sesionActiva; }

//Cambiar PIN
//CuentaNueva=0 si se cambia el PIN de una cuenta nueva.
//CuentaNueva=1 si se cambia el PIN de una cuenta existente.
void Cajero::cambiarPIN(std::string PINactual, std::string cuenta, int CuentaNueva){    
    std::string PIN,aux,nuevoPIN;
    if(CuentaNueva!=0){        
        //std::cout<<"Ingrese el PIN actual: ";
        PIN=validateDigitsInput("Ingrese el PIN actual: ",4);
        while (PIN != PINactual) {
            std::cout << "PIN incorrecto. Intente de nuevo.\n";
            PIN = validateDigitsInput("Ingrese el PIN actual: ", 4);
        }
    }
    //std::cout<<"Ingrese el nuevo PIN: ";
    nuevoPIN=validateDigitsInput("Ingrese el nuevo PIN: ",4);
    aux=nuevoPIN;
    //std::cout<<"Confirme el nuevo PIN: ";
    nuevoPIN = validateDigitsInput("Confirme el nuevo PIN: ", 4);    

    while (nuevoPIN == PINactual) {
        std::cout << "El nuevo PIN no puede ser igual al actual. Intente de nuevo.\n";
        nuevoPIN = validateDigitsInput("Ingrese el nuevo PIN: ", 4);
    }

    aux=nuevoPIN;
    while(nuevoPIN!=aux){
        std::cout<<"PIN no coincide. Intente de nuevo.\n";
        nuevoPIN = validateDigitsInput("Confirme el nuevo PIN: ", 4);
    }

    if(dbm.updateData(getTablaCuentas(), {{getTcPIN(), nuevoPIN}}, {{getTcID(), cuenta}})){
        registrarEventos(eventos(5), "Pin cambiado satisfactoriamente");
        reiniciarSesion(*this);
    }    
}

// Método para iniciar sesión
bool Cajero::iniciarSesion() {
    std::string cuenta, pin;
    int digCuenta = 20; // Longitud del ID de cuenta
    int digPin = 4; // Longitud del PIN
    std::string mensaje="Ingrese ID de cuenta: ";    
    cuenta = validateDigitsInput(mensaje, digCuenta);

    std::string query="SELECT * FROM"+getTablaCuentas()+" WHERE "+getTcID()+" = '"+cuenta+"'";
    while(dbm.existsData(query)){
        std::cout << "Cuenta ya existe. Intente nuevamente.\n";
        cuenta = validateDigitsInput(mensaje, digCuenta);
    }   

    while (!confirmarCuenta(cuenta)) {
        std::cout << "Cuenta no encontrada. Intente nuevamente.\n";
        cuenta = validateDigitsInput("Ingrese ID de cuenta: ", digCuenta);
    }
    
    if(!confirmarPIN(*this, cuenta, "Ingrese PIN: ")){
        return false; // Si el PIN es incorrecto, no se inicia sesión
    }    
    sesionActiva = true;    
    return true;
}

//Metodo para iniciar sesion
bool Cajero::iniciarSesion(const std::string& cuenta) {
    while(!confirmarCuenta(cuenta)){
        std::cout<<"Error al intentar iniciar sesión. Intente nuevamente.\n";
    }
    
    if(!confirmarPIN(*this, cuenta, "Ingrese PIN: ")){
        std::cout << "PIN incorrecto.\n";        
        return false; // Si el PIN es incorrecto, no se inicia sesión
    }    
    sesionActiva = true;    
    return true;
}

// Método para cerrar sesión
void Cajero::cerrarSesion() {
    registrarEventos(eventos(1), "Sesion cerrada");
    sesionActiva = false;
    cuentaSesion.clear();
    IDusuarioSesion.clear();
    saldoSesion = 0.0;
    pinSesion.clear();
    usuarioSesion.clear();    
}

// Consultar saldo
void Cajero::consultarSaldo(){
    float saldo=getSaldoSesion();
    std::string cuenta=getCuentaSesion();
    std::string tipoEvento=eventos(4);//CONSULTA
    std::string detalle="Consulta de saldo";
    registrarEventos(tipoEvento,saldo,detalle);
    
}

void Cajero::consultarSaldo(std::string cuenta){
    std::string query = "SELECT " + tcSaldo + " FROM " + tablaCuentas + " WHERE " + tcID + " = '" + cuenta + "'";
    std::string saldo = dbm.executeScalar(query);
    std::cout<<"El saldo de la cuenta es: €"<<saldo<<std::endl;
    pause();
}

void Cajero::realizarDeposito(std::string cuenta){
    float saldo=getSaldoSesion();
    std::cout<<"Ingrese monto a depositar: ";
    float monto=validateFloatInput();
    if(monto>MONTO_MAX){
        std::cout<<"El monto ingresado es mayor al monto maximo permitido: "<<MONTO_MAX<<std::endl;
        return;
    }
    saldo+=monto;
    if(dbm.updateData(tablaCuentas, {{tcSaldo, std::to_string(saldo)}}, {{tcID, cuenta}})){
        if(confirmarCuenta(cuenta)){
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2) << monto;
            std::string montoString = oss.str();
            std::string detalle="Depósito de €"+montoString;            
            registrarEventos(eventos(2),monto,detalle);
            consultarSaldo();
            clearScreen();
            std::cout << "Depósito realizado.\n";            
            pause();
        }        
    }    
}

//Metodo para realizar un retiro
void Cajero::realizarRetiro(std::string cuenta){
    float saldo=getSaldoSesion();
    std::cout<<"Ingrese monto a retirar: ";
    float monto=validateFloatInput();
    if(monto>saldo){
        clearScreen();
        std::cout<<"Saldo insuficiente\n";
        pause();
    }else{
        saldo-=monto;
        if(dbm.updateData(tablaCuentas, {{tcSaldo, std::to_string(saldo)}}, {{tcID, cuenta}})){
            if(confirmarCuenta(cuenta)){
                monto=0-monto;
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(2) << monto;
                std::string montoString = oss.str();
                std::string detalle="Retiro de €"+montoString;                
                registrarEventos(eventos(3),monto,detalle);
                consultarSaldo();
                clearScreen();                
                std::cout << "Retiro realizado.\n";                
                pause();
            }
            
        }
    }    
}

// Registrar movimientos de montos (ingresos y retiros)
// Cambiar variable "retiro" a 1 si es un retiro, de lo contrario 0
void Cajero::registrarMovimiento(const std::string& detalle, double monto, int retiro) {
    std::string tipoEvento;
    if(retiro==1){
        //monto=-monto;
        tipoEvento=eventos(3);//RETIRO
    }else {tipoEvento=eventos(2);}//INGRESO
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << monto;
    std::string montoString = oss.str();
    registrarEventos(tipoEvento, monto, detalle);
}

// Generar reporte de movimientos
void Cajero::generarReporte(std::string usuario){    
    std::string query="SELECT * FROM "+tablaMovimientos+" WHERE "+tmUsuario+" = '"+usuario+"'";
    dbm.executeQuery(query);
}

// Método para registrar eventos
void Cajero::registrarEventos(const std::string& tipoEvento, const std::string& detalle){
    dbm.insertData(tablaMovimientos, {tmCuenta, tmUsuario, "tipo_evento", tmDescripcion}, {cuentaSesion, IDusuarioSesion, tipoEvento, detalle});   
}

// Método para registrar eventos con monto
void Cajero::registrarEventos(const std::string& tipoEvento, double monto, const std::string& detalle){
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << monto;
    std::string montoString = oss.str();
    dbm.insertData(tablaMovimientos, {tmCuenta, tmUsuario, "tipo_evento", "monto", tmDescripcion}, {cuentaSesion, IDusuarioSesion, tipoEvento, montoString, detalle});
}

// Método para dar de alta a un cliente
void Cajero::altaCliente(std::string& IDCliente){
    std::string query = "SELECT * FROM usuario WHERE ID = '" + IDCliente + "'";
    if(dbm.existsData(query)){
        std::cout<<"El cliente ya existe\n";
        std::cout<<"Desea abrir cuenta?\n1.Si\n2.No\n";
        int opcion=validateIntInput("> ");
        switch(opcion){
            case 1:
                if(!confirmarCuenta(IDCliente)){
                    std::cout<<"Cliente "<<IDCliente<<" no tiene ninguna cuenta\n"<<"\n";
                    std::cout<<"Creando cuenta...\n";
                    abrirCuenta(IDCliente);                    
                    return;
                }else{
                    std::cout<<"Cliente "<<IDCliente<<" tiene una cuenta\n"<<"\n";                    
                    return;
                }
                
                break;
            case 2:
                break;
            default:
                break;
        }
    }else{
        std::string nombre, apellidos;
        std::cout<<"Ingres el/los nombre(s) del cliente: ";
        std::getline(std::cin, nombre);
        std::cout<<"Ingrese los apellidos del cliente: ";
        std::getline(std::cin, apellidos);
        
        if(dbm.insertData(tablaUsuarios, {tuID, tuNombre, tuApellido}, {IDCliente, nombre, apellidos})){
            clearScreen();
            std::cout<<"Cliente dada de alta exitosamente\n";
            pause();
            abrirCuenta(IDCliente);
        }else{
            clearScreen();
            std::cout<<"Error al dar de alta el cliente\n";
            pause();
        }
    }
}

// Método para abrir una cuenta nueva
void Cajero::abrirCuenta(std::string cliente) {    
    float saldoFloat;
    std::string cuenta = generateNumberString(20, 0);
    std::string pin = generateNumberString(4, 1);

    std::cout << "Ingrese saldo inicial: ";
    saldoFloat = validateFloatInput();

    // Convertir saldo a string con dos decimales
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << saldoFloat;
    std::string saldo = oss.str();

    // Usar el parámetro cliente como ID_Usuario
    if (dbm.insertData(tablaCuentas, {tcID, tcUsuario, tcPIN, tcSaldo},
                       {cuenta, cliente, pin, saldo})) {
        clearScreen();
        if(confirmarCuenta(cuenta)){
            std::cout << "Cuenta creada exitosamente.\n";
            pause();
        }
    } else {
        std::cerr << "Error al crear la cuenta.\n";
    }    
}

// Confirma si una cuenta existe y guarda los datos en variables de sesión
bool Cajero::confirmarCuenta(const std::string& cuenta) {
    std::string query = "SELECT cuenta.ID, usuario.ID, usuario.Nombre, usuario.Apellido, cuenta.monto, cuenta.pin "
                        "FROM usuario "
                        "JOIN cuenta ON usuario.ID = cuenta.ID_Usuario "
                        "WHERE cuenta.ID = '" + cuenta + "'";
    auto resultados = dbm.fetchQuery(query);
    if (!resultados.empty()) {
        cuentaSesion = cuenta;
        IDusuarioSesion = resultados[0][tuID]; // usuario.ID
        std::string nombre=resultados[0][tuNombre] + " " + resultados[0][tuApellido]; // Nombre completo
        usuarioSesion = nombre;
        saldoSesion = std::stod(resultados[0][tcSaldo]);
        pinSesion = resultados[0][tcPIN];
        sesionActiva = true;
        return true;
    } else {
        sesionActiva = false;
        return false;
    }
}

//Funciones de utilidad

// Función para reiniciar la sesión
void reiniciarSesion(Cajero& c){
    std::string cuenta=c.getCuentaSesion();
    clearScreen();
    std::cout<<"Reiniciando sesión...\n";
    pause();
    c.cerrarSesion();
    if(!c.iniciarSesion(cuenta)){
        std::cout<<"Ha ocurrido un error al iniciar sesión.\n";
    }
}

// Función para confirmar el PIN
bool confirmarPIN(Cajero& c, const std::string& cuenta, const std::string mensaje){    
    for (int i = 0; i < 3; i++) {
        std::cout << "Intento " << i + 1 << "/3:\n";
        std::string pin = validateDigitsInput(mensaje, 4);

        // Consulta si el PIN es correcto
        std::string query = "SELECT * FROM " + c.getTablaCuentas() +
                            " WHERE " + c.getTcID() + " = '" + cuenta +
                            "' AND " + c.getTcPIN() + " = '" + pin + "'";
        auto resultados = dbm.fetchQuery(query);

        if (!resultados.empty()) {
            // PIN correcto
            return true;
        } else {
            std::cout << "PIN incorrecto. Intente de nuevo.\n";           
        }
    }
    std::cout << "Ha superado el número máximo de intentos.\n";
    c.registrarEventos(eventos(6), "PIN incorrecto en intento de acceso");    
    return false;
}

//Listado de eventos
//0 - INICIO_SESION
//1 - CIERRE_SESION
//2 - INGRESO
//3 - RETIRO
//4 - CONSULTA
//5 - CAMBIO_PIN
//6 - INTENTO_FALLIDO
//7 - CAMBIO_PIN_FALLIDO
//8 - TRANSACCION_RECHAZADA
std::string eventos(int num){
    std::vector<std::string> evento={
        "INICIO_SESION",
        "CIERRE_SESION",
        "INGRESO",
        "RETIRO",
        "CONSULTA",
        "CAMBIO_PIN",
        "INTENTO_FALLIDO",
        "CAMBIO_PIN_FALLIDO",
        "TRANSACCION_RECHAZADA"
    };
    return evento[num];
}

// Prueba de conexion a la base de datos
void PruebaDB(){    
    std::string query = "SHOW TABLES;";    
    std::cout<<"Monstrando las tablas de la base de datos: "<<std::endl;

    std::cout<<std::endl<<query<<std::endl;    
    std::vector<std::map<std::string, std::string>> tables = dbm.fetchQuery(query);

    if(tables.empty()){
        std::cout<<"No se encontraron tablas en la base de datos"<<std::endl;
        exitProgram();
    }else{
        std::cout<<"\n--- Tablas encontradas ---"<<std::endl;
        int c=1;
        for(const auto& table_row : tables){
            for(const auto& [column_name, table_name] : table_row){
                std::cout<<"Tabla: "<<c++<<": "<<table_name<<std::endl;
            }
        }        
        std::cout<<"--- Fin de tablas ---\n"<<std::endl;    
    }    
    std::cout<<std::endl;
}