//cajero.cpp
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>

#include <iostream>
#include <cstdio>
#include "cajero.h"


Cajero::Cajero(): numeroCuenta(""),nombre(""),pin(""),saldo(0),Archivo("../cuentas.txt"),digPin(0),digCuenta(0){
    digPin=4;
    digCuenta=10;
}

Cajero::~Cajero(){}

std::string Cajero::getNumeroCuenta() const{return numeroCuenta;}
std::string Cajero::getNombre() const{return nombre;}
std::string Cajero::getPin() const{return pin;}
float Cajero::getSaldo() const{return saldo;}

int Cajero::getDigPin() const {return digPin;}
int Cajero::getDigCuenta() const {return digCuenta;}

bool Cajero::getCuenta(int cuenta_int){
    std::string cuenta = std::to_string(cuenta_int);

    std::ifstream archivo(Archivo);
    std::string linea;
    bool encontrada=false;

    while(getline(archivo,linea)){
        std::istringstream ss(linea);
        std::string campo;
        int campoNum = 0;
        std::string cuentaArchivoStr;
        std::string nombreArchivo;
        std::string pinArchivoStr;
        float saldoArchivo = 0.0;
        while(getline(ss,campo,'|')){
            switch(campoNum){
                case 0: cuentaArchivoStr=campo; break;
                case 1: nombreArchivo=campo;break;
                case 2: pinArchivoStr=campo; break;
                case 3: saldoArchivo=std::stof(campo); break;
            }
            campoNum++;
        }
        if(cuentaArchivoStr==cuenta){
            numeroCuenta=cuentaArchivoStr;
            nombre=nombreArchivo;
            pin=pinArchivoStr;
            saldo=saldoArchivo;
            encontrada=true;
            break;
        }
    }
    if(!encontrada){
        printf("Número de cuenta no encontrada.\n");
        return false;
    }
    return true;
}
bool Cajero::cuentaExiste(const std::string& cuenta){
    std::ifstream archivo(Archivo);
    std::string linea;

    while(getline(archivo,linea)){
        std::istringstream ss(linea);
        std::string campo;
        getline(ss,campo,'|');
        std::string cuentaArchivoStr=campo;
        if(cuentaArchivoStr==cuenta) return true;
    }
    return false;
}

void Cajero::IniciarSesion(const std::string& cuentaIngresada){
    int intentos=0;
    int maxIntentos=3;
    int digitos=4;
    std::string pinIngresado;
    if (!cuentaExiste(cuentaIngresada)) {
        printf("Número de cuenta no encontrada.\n");
        return;
    }
    std::ifstream archivo_temp(Archivo);
    std::string linea_temp;
    while(getline(archivo_temp, linea_temp)){
        std::istringstream ss_temp(linea_temp);
        std::string campo_temp;
        int campoNum_temp = 0;
        std::string cuentaArchivoStr_temp;
        std::string nombreArchivo_temp;
        std::string pinArchivoStr_temp;
        float saldoArchivo_temp = 0.0;
        while(getline(ss_temp, campo_temp, '|')){
            switch(campoNum_temp){
                case 0: cuentaArchivoStr_temp = campo_temp; break;
                case 1: nombreArchivo_temp = campo_temp; break;
                case 2: pinArchivoStr_temp = campo_temp; break;
                case 3: saldoArchivo_temp = std::stof(campo_temp); break;
            }
            campoNum_temp++;
        }
        if(cuentaArchivoStr_temp == cuentaIngresada){
            numeroCuenta = cuentaArchivoStr_temp;
            nombre = nombreArchivo_temp;
            pin = pinArchivoStr_temp;
            saldo = saldoArchivo_temp;
            break;
        }
    }
    archivo_temp.close();


    //Depuración.
    //depurar(*this);// Comentar si no se quiere depurar

    std::cout<<"Bienvenido "<<nombre<<"\n\n";

    while(intentos<maxIntentos){
        std::ostringstream oss;
        oss <<"Por favor ingrese su PIN.\nIntento ["
            <<(intentos + 1)
            <<"] de "
            << maxIntentos
            << ": ";
        std::string mensaje=oss.str();
        printf("Porfavor ingrese su PIN.\n");
        pinIngresado = validarDigitos(mensaje,digitos); 

        if(pinIngresado==pin){
            std::cout<<"Inicio de sesión exitoso.\n";            
            transacciones(*this);
            return;
        }else{
            intentos++;
            printf("PIN incorrecto. Intentos restantes: %d\n",(maxIntentos-intentos));
        }
    }
    printf("Número máximo de intentos alcanzado. Acceso denegado.\n");    
}

void Cajero::CrearCuenta(const std::string& cuenta, const std::string& nombre, const std::string& pin, float saldoInicial){
    if(cuentaExiste(cuenta)){
        std::cout<<"La cuenta ya existe."<<std::endl;
        return;
    }

    if(saldoInicial<0){
        std::cout<<"El saldo inicial no puede ser negativo."<<std::endl;
        return;
    }

    std::ofstream archivo(Archivo,std::ios::app);
    if(!archivo){
        std::cout<<"Error al abrir el archivo de cuentas."<<std::endl;
        return;
    }

    std::ostringstream oss;
    oss <<cuenta<<"|"
        <<nombre<< "|"
        <<pin<<"|"
        <<std::fixed << std::setprecision(2)
        <<saldoInicial
        <<std::endl;
    archivo<<oss.str();;
    archivo.close();

    this->numeroCuenta=cuenta;
    this->nombre=nombre;
    this->pin=pin;
    this->saldo=saldoInicial;

    std::cout<<"Cuenta creada exitosamente."<<std::endl;
    printf("Su numero de cuenta es: %s\n",cuenta.c_str()); 
    printf("Su PIN asignado es: %s\n.Porfavor cambielo de inmediato!!!.\n",pin.c_str());
}

//----------------------Funciones----------------------------------------------
bool esSoloDigitos(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

std::string validarInputString(const std::string &mensaje){
    std::string valor;
    while(true){
        printf("%s", mensaje.c_str());
        std::getline(std::cin, valor); 

        if (valor.empty()) {
            printf("La entrada no puede estar vacía. Intente de nuevo.\n");
            continue;
        }        
        return valor;
    }
}

int validarNumero(const std::string &opcion){
    int valor;
    while(true){
        std::cout<<opcion;
        if(std::cin>>valor) break;
        else{
            std::cout<<"¡Error! Debe ingresar un número entero válido.\nIntente de nuevo."<<std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    return valor;
}

int validarNumero(const std::string &opcion,void (*menu_ptr)(const std::string &opcion)){
	int valor;
    while(true){
        std::cout<<opcion;
        if(std::cin>>valor) break;
        else{
            std::cout<<"¡Error! Debe ingresar un número entero válido.\nIntente de nuevo."<<std::endl;
            menu_ptr(opcion);
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return valor;
}

std::string validarDigitos(const std::string& mensaje,int n){
    std::string numeroStr;
    while(true){
        printf("%s", mensaje.c_str());
        std::getline(std::cin, numeroStr);

        if (!esSoloDigitos(numeroStr)) {
            printf("La entrada debe contener solo dígitos numéricos. Intente de nuevo.\n");
            continue;
        }
        
        if (numeroStr.length() == n) {
            return numeroStr;
        } else {
            printf("El número debe tener exactamente %d dígitos. Intente de nuevo.\n", n);
        }
    }
}

std::string validarDigitos(const std::string& mensaje,int n,void (*menu_ptr)(const std::string &opcion)){ // Renombre
    std::string numeroStr;
    while(true){
        printf("%s", mensaje.c_str());
        std::getline(std::cin, numeroStr);

        if (!esSoloDigitos(numeroStr)) {
            printf("La entrada debe contener solo dígitos numéricos. Intente de nuevo.\n");
            menu_ptr(mensaje);
            continue;
        }
        
        if (numeroStr.length()==n) return numeroStr;
        else {
            printf("El número debe tener exactamente %d dígitos. Intente de nuevo.\n", n);
            menu_ptr(mensaje);
        }
    }
}

void transacciones(Cajero& cajero){
    int opc; 
    do{
        std::string opcion="\nSeleccione una opción:\n>";
		menu(opcion,cajero);
        opc=validarNumero(opcion, menu);
		switch(opc){
			case 1:
				printf("Saldo actual: %.2f\n",cajero.getSaldo());				
				break;
			case 0:break;
			default:std::cout<<"Opcion no valida. Intente de nuevo.\n";
		}
    }while(opc!=0);
}

void menu(const std::string &opcion,Cajero& cajero){	
	printf("Bienvenido a su cuenta, %s\n",cajero.getNombre().c_str());
    printf("\nMenu de transacciones:\n");
    printf("1. Consultar saldo\n");
    printf("0. Salir\n");   
}


//Función de depuración
void depurar(const Cajero& cajero){
    printf("\nBienvenido %s\n", cajero.getNombre().c_str());
    printf("Numero de cuenta: %s\n", cajero.getNumeroCuenta().c_str()); // Imprimir como string
    printf("El pin de su cuenta es: %s\n", cajero.getPin().c_str()); // Imprimir como string
    printf("Saldo actual: €%.2f\n", cajero.getSaldo());
    //printf("Fecha de depuración: %lld\n", getTimestamp());
    printf("Depuración completa.\n");
    exit(0);
}
//-----------------------------------------------------------------------------