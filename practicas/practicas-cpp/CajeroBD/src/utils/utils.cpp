//utils.cpp
#include "utils.h"
#include <string>
#include <algorithm>
#include <cctype>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

void clearInputBuffer(){
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
}

void validateInput(const std::string& prompt,std::string& input){
	std::cout<<prompt;
	std::getline(std::cin,input);
}

float validateFloatInput(){
	float valor;
    while(true){        
        if(std::cin>>valor) break;
        else{
            std::cout<<"Error: debe ingresar un valor numérico válido (float).\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    return valor;
}

int validateIntInput(const std::string &prompt){
	int valor;
    while(true){
        std::cout<<prompt;
        if(std::cin>>valor) break;
        else{
            std::cout<<"Error: debe ingresar un valor entero válido.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    return valor;
}

bool validateDigits(const std::string &str){
	return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

int validateIntInput(const std::string &prompt, void (*menu_ptr)(const std::string &prompt)){
	int valor;
    while(true){
        std::cout<<prompt;
        if(std::cin>>valor) break;
        else{
            std::cout<<"Error: debe ingresar un valor entero válido.\n";
            menu_ptr(prompt);
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return valor;
}

std::string validateDigitsInput(const std::string &message, int n){
    std::string numeroStr;
    while(true){
        //printf("%s", message.c_str());
        std::cout<<message;
        std::cin>>numeroStr;
        //std::getline(std::cin, numeroStr);

        if (!validateDigits(numeroStr)) {
            printf("Error: la entrada debe contener únicamente %d dígitos numéricos.\n", n);
            continue;
        }
        
        if(numeroStr.length()==static_cast<std::string::size_type>(n)){
            return numeroStr;
        } else {
            printf("Error: la longitud debe ser exactamente de %d dígitos.\n", n);
        }
    }
}

std::string validateDigitsInput(const std::string &message, int n, void (*menu_ptr)(const std::string &prompt)){
    std::string numeroStr;
    while(true){
        printf("%s", message.c_str());
        std::getline(std::cin, numeroStr);

        if (!validateDigits(numeroStr)) {
            printf("Error: la entrada debe contener únicamente %d dígitos numéricos.\n", n);
            menu_ptr(message);            
            continue;
        }
        
        if(numeroStr.length()==static_cast<std::string::size_type>(n)) return numeroStr;
        else {
            printf("Error: la longitud debe ser exactamente de %d dígitos.\n", n);
            menu_ptr(message);            
        }
    }
}

std::string validateStringInput(const std::string &message, int n) {
    std::string input;
    while (true) {
        printf("%s", message.c_str());
        std::getline(std::cin, input);
        if (input.length() == n) {
            return input;
        } else {
            printf("Error: la entrada debe tener exactamente %d caracteres.\n", n);
        }
    }
}

std::string validateStringInput(const std::string &message, int n, void (*menu_ptr)(const std::string &prompt)) {
    std::string input;
    while (true) {
        printf("%s", message.c_str());
        std::getline(std::cin, input);
        if (input.length() == n) {
            return input;
        } else {
            printf("Error: la entrada debe tener exactamente %d caracteres.\n", n);
            menu_ptr("¿Desea intentarlo de nuevo? (s/n)");
            char respuesta;
            std::cin >> respuesta;
            std::cin.ignore(); // Ignorar el salto de línea
            if (respuesta != 's') {
                break;
            }
        }
    }
    return "";
}

std::string generateNumberString(int n, int firstnumber){
    std::string num_str;
    int num=0;
    srand(time(NULL));
    if(n>0){
        if(firstnumber==0){
            num=0+rand()%9;
            num_str+=std::to_string(num);
        }else {
            num=1+rand()%9;
            num_str+=std::to_string(num);
        }
    }    
    for(int i=1;i<n;++i){
        num=0+rand()%9;
        num_str+=std::to_string(num);
    }
    return num_str;
}

void cleanString(std::string &str) {
	#ifdef _WIN32
        system("cls");   // Windows
    #else
        system("clear"); // Linux/macOS
    #endif
}

void pause(){
	std::cout<<"Presione enter para continuar...\n";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
}

void clearScreen(){
    #ifdef _WIN32
        system("cls");   // Windows
    #else
        system("clear"); // Linux/macOS
    #endif
}

void exitProgram(){
    std::cout<<"Saliendo del programa...\n";
    clearScreen();
    pause();
    std::cout<<"Hasta luego...\n";
    std::exit(0);
}

// Hashea el PIN con un "salt" fijo, igual que en la función SQL hash_pin
std::string hashPin(const std::string& pin) {
    std::string salted = "salt_" + pin;
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(salted.c_str()), salted.size(), hash);

    std::ostringstream oss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return oss.str();
}
