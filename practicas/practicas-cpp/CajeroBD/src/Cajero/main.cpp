/******************************************************************************
Proyecto:       Sistema de Cajero Automático
Autor:          Ektor Ormaetxea V
Descripción:    Simula un sistema de cajero automático básico que permite 
                iniciar sesión, consultar saldo, depositar y retirar dinero
                usando una base de datos MySQL.
Lenguaje:       C++
******************************************************************************/
#include <iostream>
#include "Cajero.h"

void Depurar();
void menuUsuario(Cajero& c, std::string cuenta, std::string pin);
void cuentaNueva(Cajero& c);

int main(){
    // Depuración    
    //Depurar(); // Descomentar para activar la depuración
    Cajero c;
    int opc;

    do{
        clearScreen();
        printf("Bienvenido al Banco\n");
        printf("1. Iniciar Sesión\n");
        printf("2. Crear cuenta\n");
        printf("0. Salir\n");
        printf("Seleccione una opción:\n");
        opc=validateIntInput("> ");
        switch(opc){
            case 1:{//Iniciar sesion
                if(c.iniciarSesion()){                    
                    std::string cuenta=c.getCuentaSesion();
                    std::string pin=c.getPinSesion();
                    clearScreen();
                    printf("Sesión iniciada correctamente.\n");
                    printf("Bienvenido %s.\n",c.getUsuarioSesion().c_str());
                    pause();
                    clearScreen();
                    menuUsuario(c,cuenta,pin);                    
                }                
                break;
            }
            case 2://Crear cuenta
                clearScreen();                
                cuentaNueva(c);
                break;
            case 0:
                clearScreen();                
                break;
            default:
                printf("Opcion no valida\n");
                break;
        }
    }while(opc!=0);

    clearScreen();
    exitProgram();
    return 0;
}

void Depurar(){
    Cajero cajero;    
    printf("Depuración activada\n");    
    PruebaDB();
    std::string NumCuenta="11111111111111111111";    
    printf("Iniciando sesion a la cuenta %s\n", NumCuenta.c_str());
    if (!cajero.confirmarCuenta(NumCuenta)){
        printf("ERROR!!!: Cuenta %s no encontrada\n", NumCuenta.c_str());
        exitProgram();
    }
    pause();
    clearScreen();    
    
    printf("Cuenta %s encontrada\n", NumCuenta.c_str());
    printf("DNI/NIE del dueño de la cuenta: %s\n", cajero.getIDUsuarioSesion().c_str());
    printf("Nombre del dueño de la cuenta: %s\n", cajero.getUsuarioSesion().c_str());
    printf("Saldo de la cuenta: %.2f\n", cajero.getSaldoSesion());
    printf("PIN de la cuenta: %s\n", cajero.getPinSesion().c_str());
    if (!cajero.iniciarSesion(NumCuenta)){
        printf("No se pudo iniciar sesión. Saliendo...\n");
        exitProgram();
    }else {
        clearScreen();
        printf("Sesión iniciada correctamente.\n");
        pause();
        clearScreen();
        menuUsuario(cajero,NumCuenta,cajero.getPinSesion());
        exitProgram();
        printf("Bienvenido %s\n", cajero.getUsuarioSesion().c_str());
        printf("Cuenta: %s\n", cajero.getCuentaSesion().c_str());
        printf("Saldo: %.2f\n", cajero.getSaldoSesion());
        printf("PIN: %s\n", cajero.getPinSesion().c_str());
        pause();
        exitProgram();        
    }

}

void menuUsuario(Cajero& c, std::string cuenta, std::string pin){
    c.registrarEventos(eventos(0),"Sesion iniciada");
    int opc;
    do{

        clearScreen();
        printf("\nMenú de usuario: %s\n",c.getUsuarioSesion().c_str());
        printf("1. Consultar Saldo\n");
        printf("2. Realizar Depósito\n");
        printf("3. Realizar Retiro\n");
        printf("4. Cambiar PIN\n");
        printf("0. Salir\n");
        printf("Seleccione una opción:\n");
        opc=validateIntInput("> ");
        switch(opc){
            case 1:
                clearScreen();
                c.consultarSaldo(cuenta);
                break;
            case 2:
                clearScreen();
                c.realizarDeposito(cuenta);
                break;
            case 3:
                clearScreen();
                c.realizarRetiro(cuenta);
                break;
            case 4: {                
                std::string pinAnterior=c.getPinSesion();
                clearScreen();
                c.cambiarPIN(pinAnterior,cuenta,1);
                break;
            }
            case 0:
                c.cerrarSesion();
                clearScreen();
                printf("Cerrando sesión...\nSaliendo...\n");                                
                exitProgram();
                break;
            default: printf("Opción no válida\n"); break;
                
        }

    }while(opc!=0);
}

void cuentaNueva(Cajero& c){
    std::string id;
    std::cout<<"Ingrese los datos del cliente: "<<std::endl;
    id=validateStringInput("Ingrese el DNI/NIE: ",10);
    

    c.altaCliente(id);
    std::string cuenta=c.getCuentaSesion();
    std::cout<<"Gracias por su preferencia\n";
    std::cout<<"Señor(a)"<<c.getUsuarioSesion()<<", su cuenta ha sido dada de alta exitosamente\n";
    std::cout<<"Su cuenta es: "<<cuenta<<std::endl;
    std::cout<<"Su PIN de acceso es: "<<c.getPinSesion()<<" Cambielo de inmediato\n";
    std::cout<<"Usted ingresó el monto de: €"<<c.getSaldoSesion()<<std::endl;
    if(!c.iniciarSesion(cuenta)){
        std::cout<<"Ha ocurrido un error al iniciar sesión.\n";
    }
    pause();
    clearScreen();
    menuUsuario(c,c.getCuentaSesion(),c.getPinSesion());
}