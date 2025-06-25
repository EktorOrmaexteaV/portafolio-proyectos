# 🏧 Simulador de Cajero Automático en C++

Este proyecto es un **simulador de cajero automático por consola**, desarrollado en C++ utilizando programación orientada a objetos. Permite la creación y gestión de múltiples cuentas, operaciones bancarias básicas y registro de logs por usuario.

> Proyecto desarrollado como práctica para reforzar lógica de programación, estructuras de control, archivos y POO en C++.

---

## 🎯 Funcionalidades

- 🔐 Login con número de cuenta y PIN
- 💰 Consultar saldo
- ➕ Depositar dinero
- ➖ Retirar dinero con validación de fondos
- 🔄 Cambiar PIN
- 📄 Guardado automático de cuentas y logs en archivos de texto

---

## 🧱 Estructura del Proyecto

```
ProyectoCajero/
├── src/
│   ├── main.cpp          # Menú principal e interacción
│   ├── cajero.cpp        # Implementación de métodos de la clase Cuenta
│   └── cajero.h          # Declaración de la clase Cuenta
├── cuentas.txt           # Registro de todas las cuentas (nro|nombre|PIN|saldo)
├── Logs/
│   ├── LogCuenta1111111111.txt
│   ├── LogCuenta2387968051.txt
│   └── ...               # Un archivo por cada cuenta con su historial
├── main                  # Ejecutable compilado
└── ProyectoCajero.md     # Documento interno con anotaciones del proyecto
```

---

## ⚙️ Compilación y ejecución

### 🖥️ Requisitos

- Compilador de C++ (g++, clang, etc.)
- Sistema compatible con terminal (Linux, macOS o Windows con WSL/MinGW)
- (Opcional) Code::Blocks o VS Code

### 🔧 Compilación desde terminal

```bash
cd src
g++ main.cpp cajero.cpp -o ../main
```

### ▶️ Ejecución

```bash
cd ..
./main
```

---

## 💾 Formato de los archivos

### `cuentas.txt`
Cada línea representa una cuenta:
```
numeroCuenta|nombreTitular|PIN|saldo
```

### `Logs/LogCuentaXXXXXXXXXX.txt`
Historial de acciones por cuenta, incluyendo:
- Inicios y cierres de sesión
- Depósitos y retiros
- Cambios de PIN
- Intentos de errores (como montos negativos)

---

## 📌 Posibles mejoras
- [ ] Uso de bases de datos en vez de texto plano

---

## 👨‍💻 Autor

**Ektor Ormaetxea V**  
💡 Aprendiendo y creando con C++

---

### 🌐 Licencia
Proyecto libre para fines educativos y personales.
