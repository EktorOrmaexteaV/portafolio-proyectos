//cajero.h
#include <string> 

#ifndef CAJERO_H
#define CAJERO_H

class Cajero {
    private:
        std::string numeroCuenta; 
        std::string nombre;
        std::string pin;
        float saldo;

        std::string Archivo;
        int digPin;
        int digCuenta;

    public:
        Cajero();
        ~Cajero();
        
        std::string getNumeroCuenta() const;
        std::string getNombre() const;
        std::string getPin() const;
        float getSaldo() const;

        int getDigPin() const;
        int getDigCuenta() const;        
        bool getCuenta(int cuenta);
        
        void IniciarSesion(const std::string& cuentaIngresada);
        void RetirarDinero(const std::string& cuenta,float cantidad);
        void IngresarDinero(const std::string& cuenta,float cantidad);
        void CrearCuenta(const std::string& cuenta, const std::string& nombre, const std::string& pin, float saldoInicial); 
        bool cuentaExiste(const std::string& cuenta);
        void CambiarPin(const std::string& cuenta, const std::string& pin); 

        /*
        // Funciones de log
        void crearArchivoLog(const std::string& nombreArchivo);
        void generarLog(float monto);
        void escribirArchivoLog(const std::string& nombreArchivo,float monto);
        void logSesionIniciada(const std::string& nombreArchivo);
        void logSesionFinalizada(const std::string& nombreArchivo);
        void logSesionAdvertencia(const std::string& nombreArchivo);
        void logMensaje(const std::string& nombreArchivo, const std::string& mensaje);
        */
};

long long int getTimestamp();

void depurar(const Cajero& cajero);
void transacciones(Cajero& cajero);


void menu(const std::string &opcion);
void menu(const std::string &opcion,Cajero& cajero); 

/*
//Manejo de pantalla
void limpiarPantalla();
void pausar();
*/

bool esSoloDigitos(const std::string& str); 

std::string validarInputString(const std::string &mensaje); 

int validarNumero(const std::string &opcion);
int validarNumero(const std::string &opcion, void (*menu_ptr)(const std::string &opcion));

std::string validarDigitos(const std::string& mensaje, int n);
std::string validarDigitos(const std::string& mensaje, int n, void (*menu_ptr)(const std::string &opcion));

#endif // CAJERO_H