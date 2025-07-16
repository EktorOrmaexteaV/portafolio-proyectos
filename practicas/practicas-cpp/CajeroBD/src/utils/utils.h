//utils.h
#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include <iostream>
#include <limits>
#include <string>

// Limpia el buffer de entrada estándar (cin)
void clearInputBuffer();

// Valida y obtiene un número flotante desde la entrada estándar
float validateFloatInput();

// Verifica si una cadena contiene solo dígitos
bool validateDigits(const std::string &str);

// Solicita y valida un número entero desde la entrada estándar con un mensaje
int validateIntInput(const std::string &prompt);

// Solicita y valida un número entero, ejecutando un menú si es necesario
int validateIntInput(const std::string &prompt, void (*menu_ptr)(const std::string &prompt));

// Solicita una cadena de exactamente n dígitos, mostrando un mensaje
std::string validateDigitsInput(const std::string &message, int n);

// Solicita una cadena de exactamente n dígitos, con menú de error personalizado
std::string validateDigitsInput(const std::string &message, int n, void (*menu_ptr)(const std::string &prompt));

//Solicita una cadena de exactamente n caracteres
std::string validateStringInput(const std::string &message, int n);

// Solicita una cadena de exactamente n caracteres, con menú de error personalizado
std::string validateStringInput(const std::string &message, int n, void (*menu_ptr)(const std::string &prompt));

// Genera una cadena numérica aleatoria de n dígitos, el primer dígito puede ser 0 si "firstnumber" es 0
std::string generateNumberString(int n, int firstnumber);

// Limpia la pantalla de la terminal
void clearScreen();

// Pausa la ejecución esperando una tecla
void pause();

// Termina el programa de forma segura
void exitProgram();

// Hashea el PIN con un "salt" fijo, igual que en la función SQL hash_pin
std::string hashPin(const std::string& pin);

#endif /* UTILS_UTILS_H_ */
