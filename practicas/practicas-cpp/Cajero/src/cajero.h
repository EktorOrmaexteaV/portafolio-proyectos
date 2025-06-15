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
        std::string rutaLog;
        std::string ArchivoLog;

        int digPin;
        int digCuenta;

    public:
        int montoMaximo;
        Cajero();
        ~Cajero();
        
        std::string getNumeroCuenta() const;
        std::string getNombre() const;
        std::string getPin() const;
        float getSaldo() const;

        int getDigPin() const;
        int getDigCuenta() const;        
        bool getCuenta(const std::string& cuenta);

        std::string getArchivoLog() const;
        std::string getRutaLog() const;
        
        void IniciarSesion(const std::string& cuentaIngresada);
        void IngresarPin(const std::string& cuentaIngresada);
        void RetirarDinero(const std::string& cuenta,float cantidad);
        void IngresarDinero(const std::string& cuenta,float cantidad);
        void CrearCuenta(const std::string& cuenta, const std::string& nombre, const std::string& pin, float saldoInicial); 
        bool cuentaExiste(const std::string& cuenta);
        void CambiarPin(const std::string& cuenta, const std::string& pin);

        
        // Funciones de log
        void crearArchivoLog(const std::string& numeroCuenta);
        bool existeArchivoLog(const std::string& numeroCuenta);
        void logCuentaCreada(const std::string& nombreArchivo);
        void logGenerarMoivimentos(float monto);        
        void LogEscribirMovimientos(const std::string& nombreArchivo, float monto);
        
        void logGenerarMensajes(const std::string& numeroCuenta, const std::string& mensaje);
        void logSesionIniciada(const std::string& nombreArchivo);
        void logSesionFinalizada(const std::string& nombreArchivo);
        void logSesionFallido(const std::string& nombreArchivo);
        void logPinIncorrecto(const std::string& nombreArchivo);
        void logCambioPinExitoso(const std::string& nombreArchivo);
        void logCambioPinFallido(const std::string& nombreArchivo);
        void logIngresoFallido(const std::string& nombreArchivo, float monto);
        void logRetiroFallido(const std::string& nombreArchivo, float monto);
        void logConsultaSaldo(const std::string& nombreArchivo);

        // void generarLogs(float monto);
        // void escribirArchivoLog(const std::string& nombreArchivo, float monto);
};

long long int getTimestamp();

void depurar(const Cajero& cajero);
void transacciones(Cajero& cajero);


void menu(const std::string &opcion);
void menu(const std::string &opcion,Cajero& cajero); 


//Manejo de pantalla
void limpiarPantalla();
void pausar();


bool esSoloDigitos(const std::string& str); 

std::string validarInputString(const std::string &mensaje);

float validarSaldo();

int validarNumero(const std::string &opcion);
int validarNumero(const std::string &opcion, void (*menu_ptr)(const std::string &opcion));

std::string validarDigitos(const std::string& mensaje, int n);
std::string validarDigitos(const std::string& mensaje, int n, void (*menu_ptr)(const std::string &opcion));

#endif // CAJERO_H