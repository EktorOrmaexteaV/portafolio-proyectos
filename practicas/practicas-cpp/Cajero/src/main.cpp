/******************************************************************************
Proyecto:       Sistema de Cajero Automático
Autor:          Ektor Ormaetxea V
Archivos:       main.cpp, cajero.h, cajero.cpp, cuentas.txt
Lenguaje:       C++
Compilador:     g++ (o el que estés usando)
Descripción:    Este proyecto simula un sistema básico de cajero automático 
				que permite a los usuarios iniciar sesión, consultar su saldo, 
				depositar, retirar dinero, cambiar su PIN y crear nuevas cuentas. 
				Interactúa con un archivo 'cuentas.txt' para gestionar la 
				información de las cuentas y genera archivos de registro de 
				transacciones y actividades de sesión.
Versión:        v1.0.0
*******************************************************************************/
#include <cstdio>
#include <limits>
#include <string> 
#include <random>
#include <chrono>

#include <iostream>
#include "cajero.h"

std::string generarNumero(int n);
void menu(const std::string &opcion);
void menuUsuario(Cajero& c);
void menuAdministrador(Cajero& c);
void menuPrincipal(const std::string &opcion);

int main(){
    Cajero c;
    int opc;
   do{
	std::string opcion="> ";
	menuPrincipal(opcion);
	opc=validarNumero(opcion);
	switch(opc){
		case 1:menuUsuario(c); break;
		case 2:menuAdministrador(c); break;
		case 0:printf("Hasta luego!"); break;
		default:std::cout<<"Opcion no valida. Intente de nuevo.\n";
	}
	
   }while(opc!=0);
	
    return 0;
}

void menu(const std::string &opcion){
	std::cout<<std::endl;
    printf("Seleccione una opción\n");
    printf("1. Ingresar cuenta.\n");
    printf("2. Crear cuenta.\n");
    printf("0. Salir.\n");    
}

void menuPrincipal(const std::string &opcion){
	std::cout<<std::endl;
    printf("Eres un usuario o un administrador?\n");
    printf("1. Usuario.\n");
    printf("2. Administrador.\n");
    printf("0. Salir.\n");    
}

std::string generarNumero(int n){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 9);

    std::string num_str;

    for(int i=0; i<n; ++i){
        num_str += std::to_string(distrib(gen));
    }
    return num_str;
}

void menuUsuario(Cajero& c){
	int opc;	
	do{
			std::cout<<std::endl;
			std::string opcion="> ";
			std::cout<<"Menu principal"<<std::endl;
			menu(opcion);
            opc=validarNumero(opcion,menu);

			switch(opc){
				case 1:{
					std::string cuentaIngresada;
					std::string mensaje="Ingrese el numero de cuenta: ";
					cuentaIngresada=validarDigitos(mensaje,c.getDigCuenta(),menu);
					c.IniciarSesion(cuentaIngresada);
					break;
				}
				case 2:{
					std::string nuevaCuenta=generarNumero(c.getDigCuenta());
					std::string nuevoPin=generarNumero(c.getDigPin());
					std::string nuevoNombre;
					float saldoInicial;
					std::cout<<"Ingrese su nombre y apellido: ";
					//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	                std::getline(std::cin, nuevoNombre);
					std::cout<<"Ingrese el saldo inicial: ";
					std::cin>>saldoInicial; 
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					c.CrearCuenta(nuevaCuenta,nuevoNombre,nuevoPin,saldoInicial);
					break;
				}	
                case 0:
                    printf("Hasta luego!\n");
                    exit(0);
                    break;
                default:
                    std::cout << "Opcion no valida. Intente de nuevo.\n";
                    break;
			}
		}while(opc!=0);	
}
void menuAdministrador(Cajero& c){
	std::cout<<"\n\n	EN CONSTRUCCION\n\n";
}