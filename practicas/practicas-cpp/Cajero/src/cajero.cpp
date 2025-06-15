//cajero.cpp
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <locale.h>

#include <iostream>
#include <cstdio>
#include "cajero.h"

Cajero::Cajero(): numeroCuenta(""),nombre(""),pin(""),saldo(0),Archivo("../cuentas.txt"),digPin(0),digCuenta(0),rutaLog("../Logs"),ArchivoLog(rutaLog+"/LogCuenta"+getNumeroCuenta()+".txt"){
    digPin=4;
    digCuenta=10;
    montoMaximo=10000;
}

Cajero::~Cajero(){}

std::string Cajero::getNumeroCuenta() const{return numeroCuenta;}
std::string Cajero::getNombre() const{return nombre;}
std::string Cajero::getPin() const{return pin;}
float Cajero::getSaldo() const{return saldo;}

int Cajero::getDigPin() const {return digPin;}
int Cajero::getDigCuenta() const {return digCuenta;}

std::string Cajero::getArchivoLog() const {return ArchivoLog;}
std::string Cajero::getRutaLog() const {return rutaLog;}

bool Cajero::getCuenta(const std::string& cuenta){
    //std::string cuenta = std::to_string(cuenta_int);

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
    IngresarPin(cuentaIngresada);
}

void Cajero::IngresarPin(const std::string& cuenta){
    int intentos=0;
    int maxIntentos=3;
    int digitos=4;
    std::string pinIngresado;

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
            //logSesionIniciada(cuenta);
            transacciones(*this);
            return;
        }else{
            intentos++;
            printf("PIN incorrecto. Intentos restantes: %d\n",(maxIntentos-intentos));            
        }
    }
    printf("Número máximo de intentos alcanzado. Acceso denegado.\n");
    logPinIncorrecto(cuenta);
}

void Cajero::IngresarDinero(const std::string& cuenta,float cantidad){
    if(cantidad<0){
        printf("La cantidad a ingresar debe ser mayor a 0.\n");
        return;        
    }
    
    if(cantidad>montoMaximo){
        printf("Transacción denegada.\n");
        printf("La cantidad a ingresar es mayor a €%'d.\n",montoMaximo);
        logIngresoFallido(cuenta,cantidad);
        return;
    }
    
    saldo+=cantidad;   

    //generarMontosLog(cantidad);
    //generarMontosLog(getSaldo());

    std::ifstream archivoLectura(Archivo);
    std::vector<std::string> lineas;
    std::string linea;

    while(getline(archivoLectura, linea)){
        std::istringstream ss(linea);
        std::string campo;
        getline(ss, campo,'|');
        std::string cuentaArchivoStr=campo;

        if(cuentaArchivoStr==cuenta){
            std::ostringstream ssNuevaLinea;
            ssNuevaLinea<<cuenta<<"|"<<nombre<<"|"<<pin<<"|"<<std::fixed<<std::setprecision(2)<<saldo;
            lineas.push_back(ssNuevaLinea.str());   
        }else lineas.push_back(linea);        
    }
    archivoLectura.close();

    std::ofstream archivoEscritura(Archivo);
    for(const std::string& linea : lineas){
        archivoEscritura<<linea<<"\n";
    }
    archivoEscritura.close();

    printf("Ingreso exitoso. Saldo actual: €%.2f\n",getSaldo());
    logGenerarMoivimentos(cantidad);
    logGenerarMoivimentos(getSaldo());
}

void Cajero::RetirarDinero(const std::string& cuenta,float cantidad){
    if(cantidad<0){
        printf("La cantidad a retirar debe ser mayor a €0.\n");
        logRetiroFallido(cuenta,cantidad);
        return;        
    }
    if(cantidad>saldo){
        printf("Transacción denegada.\n");
        printf("La cantidad a retirar es mayor al saldo disponible.\n");
        printf("Saldo disponible: €%.2f\n",saldo);
        logRetiroFallido(cuenta,cantidad);
        return;
    }
    saldo-=cantidad;
    logGenerarMoivimentos(-cantidad);
    logGenerarMoivimentos(getSaldo());

    std::ifstream archivoLectura(Archivo);
    std::vector<std::string> lineas;
    std::string linea;

    while(getline(archivoLectura, linea)){
        std::istringstream ss(linea);
        std::string campo;
        getline(ss, campo,'|');
        std::string cuentaArchivoStr=campo;

        if(cuentaArchivoStr==cuenta){
            std::ostringstream ssNuevaLinea;
            ssNuevaLinea<<cuenta<<"|"<<nombre<<"|"<<pin<<"|"<<std::fixed<<std::setprecision(2)<<saldo;
            lineas.push_back(ssNuevaLinea.str());   
        }else lineas.push_back(linea);        
    }
    archivoLectura.close();

    std::ofstream archivoEscritura(Archivo);
    for(const std::string& linea : lineas){
        archivoEscritura<<linea<<"\n";
    }
    archivoEscritura.close();

    printf("Retiro exitoso. Saldo actual: €%.2f\n",getSaldo());
}

void Cajero::CambiarPin(const std::string& cuentaIngresada, const std::string& pinAntiguoIngresado) {    
    if (!cuentaExiste(cuentaIngresada)) {
        printf("El número de cuenta ingresado no existe.\n");
        return;
    }
    
    std::string cuentaReal;
    std::string nombreReal;
    std::string pinReal;
    float saldoReal;

    std::ifstream inputFile(Archivo);
    std::string line;
    bool found = false;
    std::vector<std::string> lines;

    while (getline(inputFile, line)) {
        lines.push_back(line);
        std::istringstream ss(line);
        std::string campo;
        std::string currentAccountStr;
        std::string currentName;
        std::string currentPinStr;
        float currentSaldo;
        
        getline(ss, currentAccountStr, '|');
        getline(ss, currentName, '|');
        getline(ss, currentPinStr, '|');
        std::string saldoTemp;
        getline(ss, saldoTemp, '|');
        currentSaldo = std::stof(saldoTemp);

        if (currentAccountStr == cuentaIngresada) {
            cuentaReal = currentAccountStr;
            nombreReal = currentName;
            pinReal = currentPinStr;
            saldoReal = currentSaldo;
            found = true;
        }
    }
    inputFile.close();

    if (!found) {        
        printf("Error interno: Cuenta no encontrada después de la verificación inicial.\n");
        return;
    }
    
    if (pinAntiguoIngresado != pinReal) {
        printf("El PIN actual ingresado es incorrecto.\n");
        logCambioPinFallido(cuentaIngresada);
        return;
    }
    
    std::string nuevoPin1;
    std::string nuevoPin2;

    printf("Por favor, ingrese su nuevo PIN (de %d dígitos): \n",getDigPin());
    nuevoPin1 = validarDigitos("Nuevo PIN: ",getDigPin());
//PUTA CAGADA (Confiirma que no es el mismo pin)
    printf("Por favor, confirme su nuevo PIN: \n");
    nuevoPin2 = validarDigitos("Confirmar nuevo PIN: ",getDigPin());
    
    if (nuevoPin1 != nuevoPin2) {
        printf("Los nuevos PINs no coinciden. La operación ha sido cancelada.\n");
        logCambioPinFallido(cuentaIngresada);
        return;
    }
    
    std::ofstream outputFile(Archivo, std::ios::trunc);
    if (!outputFile) {
        printf("Error al abrir el archivo de cuentas para escritura.\n");
        return;
    }

    bool updated = false;
    for (const auto& l : lines) {
        std::istringstream ss(l);
        std::string currentAccountInLine;
        getline(ss, currentAccountInLine, '|');

        if (currentAccountInLine==cuentaReal){            
            std::ostringstream updatedLine;
            updatedLine << cuentaReal << "|"
                        << nombreReal << "|"
                        << nuevoPin1 << "|" 
                        << std::fixed << std::setprecision(2) << saldoReal;
            outputFile << updatedLine.str() << "\n";
            updated = true;
            printf("PIN cambiado exitosamente para la cuenta %s.\n", cuentaReal.c_str());            
            logCambioPinExitoso(cuentaReal);            
        } else {            
            outputFile << l << "\n";
        }
    }
    outputFile.close();

    if (!updated) {
        printf("Error: No se pudo encontrar y actualizar la cuenta. Esto no debería ocurrir.\n");
    }
    IniciarSesion(cuentaIngresada);
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

    logCuentaCreada(cuenta);
    std::cout<<"Cuenta creada exitosamente."<<std::endl;
    printf("Su numero de cuenta es: %s\n",cuenta.c_str()); 
    printf("Su PIN asignado es: %s\n.Porfavor cambielo de inmediato!!!.\n",pin.c_str());

    IngresarPin(cuenta);
}

//----------------------Funciones----------------------------------------------
bool esSoloDigitos(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

long long int getTimestamp() {
    time_t t = std::time(nullptr);
    tm* now = std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(now, "%Y%m%d%H%M%S");
    return std::stoll(ss.str());
}
void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");   // Windows
    #else
        system("clear"); // Linux/macOS
    #endif
}

void pausar() {
    std::cout << "\nPresiona Enter para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpia el buffer
    std::cin.get();
}

//----------------------------Logs---------------------------------------------
void Cajero::crearArchivoLog(const std::string& nombreArchivo) {
    std::string ruta = "../Logs";
    if(!std::filesystem::exists(ruta)) std::filesystem::create_directory(ruta);
    
    std::string nombreArchivoLog = ruta + "/LogCuenta" + nombreArchivo + ".txt";    
    std::ifstream archivoLog(nombreArchivoLog);
    if(archivoLog.good()) return;    
    archivoLog.close();

    std::ofstream archivoLogOut(nombreArchivoLog);
    if (!archivoLogOut) {
        std::cerr << "Error al crear el archivo de log." << std::endl;
        exit(1);
    }
    // Escribe el nombre en la primera línea
    if(getCuenta(nombreArchivo)){
        std::string nombre=getNombre();
        archivoLogOut<<"Dueño de la cuenta: "<<nombre<<std::endl;    
        archivoLogOut.close();
    }
    
}

bool Cajero::existeArchivoLog(const std::string& nombreArchivo) {
    std::string nombreArchivoLog = "../Logs/LogCuenta" + nombreArchivo + ".txt";
    if(!std::filesystem::exists(nombreArchivoLog)) {
        return false;
    }
    return true;
}

void Cajero::logCuentaCreada(const std::string& nombreArchivo){
    crearArchivoLog(nombreArchivo);
    std::string mensaje="------Cuenta creada en "+std::to_string(getTimestamp());
    logGenerarMensajes(nombreArchivo,mensaje);
}

void Cajero::logGenerarMoivimentos(float monto){  
    std::string nombreArchivo = getNumeroCuenta();
    crearArchivoLog(nombreArchivo);
    LogEscribirMovimientos(nombreArchivo,monto);
}

void Cajero::LogEscribirMovimientos(const std::string& nombreArchivo, float monto){
    std::string nombreArchivoLog = "../Logs/LogCuenta" + nombreArchivo + ".txt";
    std::ofstream archivoLogOut(nombreArchivoLog, std::ios::app);
    if (!archivoLogOut) {
        std::cerr << "Error al abrir el archivo de log." << std::endl;
        exit(1);
    }
    std::ostringstream oss;
    oss <<getTimestamp()<<"|\t€"<<std::fixed<<std::setprecision(2)
        <<monto<<std::endl;
    std::string mensaje=oss.str();
    logGenerarMensajes(nombreArchivo,mensaje);
}

//---------------Logs de mensajes--------------------------------------------
void Cajero::logGenerarMensajes(const std::string& nombreArchivo, const std::string& mensaje) {
    std::string nombreArchivoLog="../Logs/LogCuenta"+nombreArchivo+".txt";
    std::ofstream archivoLogOut(nombreArchivoLog, std::ios::app);
    if (!archivoLogOut) {
        std::cerr<<"Error al abrir el archivo de log." << std::endl;
        exit(1);
    }
    archivoLogOut<<mensaje;//<<std::endl;
    //archivoLogOut<<std::endl;
    archivoLogOut.close();
}

void Cajero::logSesionIniciada(const std::string& nombreArchivo){
    std::string mensaje;
    if(!existeArchivoLog(nombreArchivo)) logCuentaCreada(nombreArchivo);
    std::ostringstream oss;
    oss <<getTimestamp()<<"-> Sesion iniciada exitosamente"
        <<std::endl
        <<getTimestamp()<<"|\t€"<<std::fixed<<std::setprecision(2)
        <<getSaldo()<<std::endl;
    mensaje=oss.str();
    logGenerarMensajes(nombreArchivo,mensaje);
}

void Cajero::logSesionFinalizada(const std::string& nombreArchivo) {    
    std::string mensaje;
    if(!existeArchivoLog(nombreArchivo)) logCuentaCreada(nombreArchivo);
    std::ostringstream oss;
    oss <<getTimestamp()<<"-> Sesion finalizada"<<std::endl
        <<"-"<<std::endl;
    mensaje=oss.str();
    logGenerarMensajes(nombreArchivo,mensaje);
}

void Cajero::logSesionFallido(const std::string& nombreArchivo) {
    std::string mensaje;
    if(!existeArchivoLog(nombreArchivo)) logCuentaCreada(nombreArchivo);
    std::ostringstream oss;
    oss <<getTimestamp()<<"-> Sesion iniciada fallida"
        <<std::endl;
    mensaje=oss.str();
    logGenerarMensajes(nombreArchivo,mensaje);
}

void Cajero::logPinIncorrecto(const std::string& nombreArchivo) {
    std::string mensaje;
    if(!existeArchivoLog(nombreArchivo)) logCuentaCreada(nombreArchivo);
    std::ostringstream oss;
    oss <<getTimestamp()<<"-> PIN INCORRECTO. Intento de acceso fallido"
        <<std::endl;
    mensaje=oss.str();
    logGenerarMensajes(nombreArchivo,mensaje);
}

void Cajero::logCambioPinExitoso(const std::string& nombreArchivo) {
    std::string mensaje;
    if(!existeArchivoLog(nombreArchivo)) logCuentaCreada(nombreArchivo);
    std::ostringstream oss;
    oss <<getTimestamp()<<"-> Cambio de PIN exitoso"
        <<std::endl;
    mensaje=oss.str();
    logGenerarMensajes(nombreArchivo,mensaje);
}

void Cajero::logCambioPinFallido(const std::string& nombreArchivo) {
    std::string mensaje;
    if(!existeArchivoLog(nombreArchivo)) logCuentaCreada(nombreArchivo);
    std::ostringstream oss;
    oss <<getTimestamp()<<"-> Cambio de PIN fallido"
        <<std::endl;
    mensaje=oss.str();
    logGenerarMensajes(nombreArchivo,mensaje);
}

void Cajero::logIngresoFallido(const std::string& nombreArchivo, float monto){
    std::string mensaje;
    if(!existeArchivoLog(nombreArchivo)) logCuentaCreada(nombreArchivo);
    std::ostringstream oss;
     oss<<getTimestamp()<<"-> Transaccion denegada."<<std::endl
        <<"Ingreso fallido de €"
        <<std::fixed<<std::setprecision(2)<<monto
        <<std::endl;
    mensaje=oss.str();
    logGenerarMensajes(nombreArchivo,mensaje);
}

void Cajero::logRetiroFallido(const std::string& nombreArchivo, float monto){
    std::string mensaje;
    if(!existeArchivoLog(nombreArchivo)) logCuentaCreada(nombreArchivo);
    std::ostringstream oss;
    oss<<getTimestamp()<<"-> Transaccion denegada."<<std::endl
        <<"Retiro fallido de €"
        <<std::fixed<<std::setprecision(2)<<monto
        <<std::endl;
    mensaje=oss.str();
    logGenerarMensajes(nombreArchivo,mensaje);
}

void Cajero::logConsultaSaldo(const std::string& nombreArchivo){
    std::string mensaje;
    if(!existeArchivoLog(nombreArchivo)) logCuentaCreada(nombreArchivo);
    std::ostringstream oss;
    oss <<getTimestamp()<<"-> Conulta de saldo realizada"
        <<std::endl;
    mensaje=oss.str();
    logGenerarMensajes(nombreArchivo,mensaje);
}
//-----------------------------------------------------------------------------


float validarSaldo(){
    float valor;
    while(true){        
        if(std::cin>>valor) break;
        else{
            std::cout<<"¡Error! Debe ingresar un número entero valido.\nIntente de nuevo."<<std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    return valor;
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
        
        if(numeroStr.length()==static_cast<std::string::size_type>(n)){
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
        
        if(numeroStr.length()==static_cast<std::string::size_type>(n)) return numeroStr;
        else {
            printf("El número debe tener exactamente %d dígitos. Intente de nuevo.\n", n);
            menu_ptr(mensaje);
        }
    }
}

void transacciones(Cajero& cajero){
    cajero.logSesionIniciada(cajero.getNumeroCuenta());
    int opc; 
    do{
        std::string opcion="\nSeleccione una opción:\n>";
		menu(opcion,cajero);
        opc=validarNumero(opcion, menu);
        switch(opc){
            case 1:
                printf("Saldo actual: %.2f\n",cajero.getSaldo());
                cajero.logConsultaSaldo(cajero.getNumeroCuenta());
                break;
            case 2: {
                setlocale(LC_NUMERIC, "");
                int monto=cajero.montoMaximo;
                printf("NOTA!: No puede ingresar más de €%'d a la vez.\n",monto);
                printf("Ingrese la cantidad de dinero a ingresar: ");
                float cantidadIngresada=validarNumero("",menu);
                cajero.IngresarDinero(cajero.getNumeroCuenta(),cantidadIngresada);
                break;
            }
            case 3: {
                printf("Ingrese la cantidad de dinero a retirar: ");
                float cantidadRetirada=validarNumero("",menu);
                cajero.RetirarDinero(cajero.getNumeroCuenta(),cantidadRetirada);
                break;
            }
            case 4: {
                printf("Ingrese el PIN actual: ");
                std::string pinActual=validarDigitos("",cajero.getPin().length(),menu);
                if(pinActual!=cajero.getPin()){
                    std::cout<<"PIN incorrecto. Intente de nuevo.\n";
                    break;
                }
                // printf("Ingrese el nuevo PIN: ");
                // std::string nuevoPin=validarDigitos("",cajero.getPin().length(),menu);
                cajero.CambiarPin(cajero.getNumeroCuenta(),pinActual);
                break;
            }
            case 0:{
                cajero.logSesionFinalizada(cajero.getNumeroCuenta());
                printf("Hasta luego!");
                exit(0);
                break;
            }                
            default:std::cout<<"Opcion no valida. Intente de nuevo.\n";
        }
    }while(opc!=0);
}

void menu(const std::string &opcion,Cajero& cajero){	
	printf("Bienvenido a su cuenta, %s\n",cajero.getNombre().c_str());
    printf("\nMenu de transacciones:\n");
    printf("1. Consultar saldo\n");
    printf("2. Ingresar dinero\n");
    printf("3. Retirar dinero\n");
    printf("4. Cambiar PIN\n");
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