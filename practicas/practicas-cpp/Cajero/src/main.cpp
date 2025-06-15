/******************************************************************************
Proyecto:       Sistema de Cajero Automático
Autor:          Ektor Ormaetxea V
Archivos:       main.cpp, cajero.h, cajero.cpp, cuentas.txt
Lenguaje:       C++
Compilador:     g++
IDE:            Visual Studio Code
Descripción:    Este proyecto simula un sistema básico de cajero automático 
				que permite a los usuarios iniciar sesión, consultar su saldo, 
				depositar, retirar dinero, cambiar su PIN y crear nuevas cuentas. 
				Interactúa con un archivo 'cuentas.txt' para gestionar la 
				información de las cuentas y genera archivos de registro de 
				transacciones y actividades de sesión.
Versión:        v2.3.0
*******************************************************************************/
#include <cstdio>
#include <limits>
#include <string> 
#include <random>
#include <chrono>
//--------------------------------------------------------------------
#include <fstream>  
#include <sstream>
int cuentas();
long* CuentasRegistradas();
void CrearLogs(std::string cuenta,std::string mensaje);
void mostrarCuentas(long* cuentas, int n);
void crearArchivosLog();
//--------------------------------------------------------------------

#include <iostream>
#include "cajero.h"

std::string generarNumero(int n, int primerNum);
void menu(const std::string &opcion);
void menuUsuario(Cajero& c);
void menuPrincipal(const std::string &opcion);

void menuAdministrador(Cajero& c);
void menuAdministrador(const std::string &opcion);
void depurarMain(Cajero& c,long long int cuenta);

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


//-------------Menús-----------------------------------------------------------
//-------------Menu Usuarios---------------------------------------------------
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

void menuUsuario(Cajero& c){
	limpiarPantalla();
	int opc;	
	do{
			std::cout<<std::endl;
			std::string opcion="> ";
			std::cout<<"Menu principal"<<std::endl;
			menu(opcion);
            opc=validarNumero(opcion,menu);

			switch(opc){
				case 1:{//Iniciar sesion
					limpiarPantalla();
					std::string cuentaIngresada;
					std::string mensaje="Ingrese el numero de cuenta: ";
					cuentaIngresada=validarDigitos(mensaje,c.getDigCuenta(),menu);
					c.IniciarSesion(cuentaIngresada);
					break;
				}
				case 2:{//Crear cuenta
					limpiarPantalla();
					std::string nuevaCuenta=generarNumero(c.getDigCuenta(),1);
					std::string nuevoPin=generarNumero(c.getDigPin(),0);
					std::string nuevoNombre;
					float saldoInicial;
					std::cout<<"Ingrese su nombre y apellido: ";
					//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	                std::getline(std::cin, nuevoNombre);
					std::cout<<"Ingrese el saldo inicial: ";
					saldoInicial=validarSaldo(); 
                    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					c.CrearCuenta(nuevaCuenta,nuevoNombre,nuevoPin,saldoInicial);
					break;
				}	
                case 0:
					limpiarPantalla();
                    printf("Hasta luego!\n");
					pausar();
                    exit(0);
                    break;
                default:
					limpiarPantalla();
                    std::cout << "Opcion no valida. Intente de nuevo.\n";
					pausar();
                    break;
			}
		}while(opc!=0);	
}

//-------------Menu Administrador----------------------------------------------

void menuAdministrador(const std::string &opcion){
	std::cout<<std::endl;
    printf("Seleccione una opción\n");
    printf("1. Ingresar como administrador.\n");
	printf("2. Depuración.\n");
	printf("3. Ver usuarios registrados.\n");
	printf("4. Genrar archivos logs de todos los usuarios.\n");
    printf("0. Salir.\n");    
}

void menuAdministrador(Cajero& c){
	limpiarPantalla();
	int opc;
	do{
		std::cout<<std::endl;
		std::string opcion="> ";
		std::cout<<"Menu administrador"<<std::endl;
		menuAdministrador(opcion);
		opc=validarNumero(opcion);
		switch(opc){
			case 1:{//1. Ingresar como administrador
				limpiarPantalla();
				printf("EN CONSTRUCCION\n");				
				pausar();break;
			}
			case 2: {//2. Depuración
				limpiarPantalla();
				printf("Depuración\n");
				long long int cuenta=1111111111;
				depurarMain(c,cuenta);
				std::cout<<std::endl; break;
			}
			case 3: {//3. Ver usuarios registrados
				limpiarPantalla();
				printf("Usuarios registrados: %d\n",cuentas());
				mostrarCuentas(CuentasRegistradas(),cuentas());
				pausar(); break;
			}
			case 4: {//4. Genrar archivos logs de todos los usuarios
				limpiarPantalla();
				int n=cuentas();
				printf("Se van a generar %d archivos logs\n",n);
				printf("Desea continuar?\n");
				printf("1. Si\n");
				printf("2. No\n");
				int opc2=validarNumero(">");
				switch(opc2){
					case 1:crearArchivosLog();break;
					case 2:break;
					default:std::cout<<"Opcion no valida. Intente de nuevo.\n";
				}
				std::cout<<std::endl; break;
			}
			case 0://0. Salir
				limpiarPantalla();
				printf("Hasta luego!");
				pausar();
				exit(0);
				std::cout<<std::endl; break;

			default:
				limpiarPantalla();
				std::cout<<"Opcion no valida. Intente de nuevo.\n";
				pausar();
				break;
		}
	}while(opc!=0);
}
//-----------------------------------------------------------------------------

//-------------Funciones-------------------------------------------------------
void depurarMain(Cajero& c,long long int cuenta){	
	std::string cuentaIngresada=std::to_string(cuenta);
	if(c.getCuenta(cuentaIngresada)){
		std::string cuenta=c.getNumeroCuenta();
		std::string nombre=c.getNombre();
		std::string pin=c.getPin();
		float saldo=c.getSaldo();
		
		transacciones(c);
	}else{
		limpiarPantalla();
		printf("La cuenta %s no existe.\n",cuentaIngresada.c_str());	
		pausar();
		exit(0);
	}
}

std::string generarNumero(int n, int primerNum){ // Agregamos el segundo parámetro
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    
    std::string num_str;

    // Lógica para el primer dígito
    if (n > 0) { // Asegurarse de que n sea positivo
        if (primerNum == 0) { // Si primerNum es 0, permite que el primer dígito sea 0
            std::uniform_int_distribution<> distrib_first(0, 9);
            num_str += std::to_string(distrib_first(gen));
        } else { // Si primerNum es diferente de 0 (ej. 1), evita que el primer dígito sea 0
            std::uniform_int_distribution<> distrib_first(1, 9);
            num_str += std::to_string(distrib_first(gen));
        }
    }

    // Lógica para los dígitos restantes
    std::uniform_int_distribution<> distrib_rest(0, 9);
    for(int i = 1; i < n; ++i){ // Empieza desde el segundo dígito (índice 1)
        num_str += std::to_string(distrib_rest(gen));
    }
    return num_str;
}
//-----------------------------------------------------------------------------

//-------------Funciones para generar archivos logs----------------------------
int cuentas(){
    std::ifstream archivo("../cuentas.txt");
    int conteo = 0;
    std::string linea;
    while (getline(archivo, linea)) {
        conteo++;
    }
    archivo.close();
    return conteo;
}

long* CuentasRegistradas(){
    int n=cuentas();
    long* cuentas = new long[n];
    std::ifstream archivo("../cuentas.txt");
    int i=0;
    std::string linea;    
    while (getline(archivo, linea)) {
        std::istringstream ss(linea);
        std::string campo;
        getline(ss, campo, '|');
        cuentas[i++] = std::stol(campo);               
    }
    archivo.close();
    return cuentas;
}

void CrearLogs(std::string cuenta,std::string mensaje){
    Cajero c;
	std::cout<<mensaje<<std::endl;
    std::string nombreArchivo=cuenta;	 
    if(!c.existeArchivoLog(cuenta)){
		c.crearArchivoLog(cuenta);
	}
	c.logCuentaCreada(cuenta);
}

void mostrarCuentas(long* cuentas, int n){
	Cajero c;
	for(int i=0;i<n;i++){
		if(c.cuentaExiste(std::to_string(cuentas[i]))){
			if(c.getCuenta(std::to_string(cuentas[i]))){
				printf("El dueño de la cuenta %s es: %s\n",std::to_string(cuentas[i]).c_str(),c.getNombre().c_str());
				printf("El PIN de la cuenta %s es: %s\n", std::to_string(cuentas[i]).c_str(), c.getPin().c_str());
				printf("El saldo de la cuenta %s es: %.2f\n",std::to_string(cuentas[i]).c_str(),c.getSaldo());
			}			
		}
	}
	
}

void crearArchivosLog(){
	limpiarPantalla();
	int n=cuentas();
	long* cuentas=CuentasRegistradas();
	for(int i=0;i<n;i++){
		std::string mensaje="Se ha creado el log de la cuenta: "+std::to_string(i+1)+" "+std::to_string(cuentas[i]);
		CrearLogs(std::to_string(cuentas[i]),mensaje);
	}
	printf("Los logs de todas las cuentas han sido creados\n");
	pausar();
}

//-----------------------------------------------------------------------------
