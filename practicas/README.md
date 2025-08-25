# Mis Prácticas de Programación

Este repositorio está dedicado a recopilar y mostrar mis diversos proyectos y ejercicios de programación. Es un espacio para aplicar y reforzar conocimientos, explorar nuevas tecnologías y documentar mi proceso de aprendizaje.

---

## Sobre Este Repositorio

Aquí encontrarás una variedad de prácticas, organizadas por lenguaje de programación. Cada proyecto individual dentro de estas carpetas se enfoca en un concepto o tecnología específica, permitiéndome mejorar mis habilidades de codificación y resolver problemas.

---

## Estructura de Proyectos

La estructura de este repositorio se organiza primero por el lenguaje de programación, y luego por proyectos individuales dentro de esas carpetas.
```
portafolio-proyectos
└── practicas/    
│   ├── practicas-cpp/
│   │   └── Cajero/
│   │   │   ├── src/
│   │   │   │   ├── cajero.cpp
│   │   │   │   ├── cajero.h
│   │   │   │   └── main.cpp
│   │   │   ├── Logs/
│   │   │   ├── cuentas.txt
│   │   │   └── README.md
│   │   ├── CajeroBD/
│   │   │   ├── src/
│   │   │   │   ├── Cajero/
│   │   │   │   │   ├── Cajero.cpp
│   │   │   │   │   ├── Cajero.h
│   │   │   │   │   └── main.cpp
│   │   │   │   ├── DatabaseManager/
│   │   │   │   │   ├── DatabaseManager.cpp
│   │   │   │   │   └── DatabaseManager.h
│   │   │   │   └── Utils/
│   │   │   │   │   ├── utils.cpp
│   │   │   │   │   └── utils.h
│   │   │   ├── BD SQL/
│   │   │   │   ├── configuracion_bd_cajero.sql
│   │   │   │   ├── configuracion_cuenta_ejemplo.sql
│   │   │   │   └── otras_consultas.sql
│   │   │   ├── database.properties
│   │   │   └── README.md
│   │   └── README.md
│   ├── practicas-java/
│   │   └── Cajero/
│   │   │   ├── lib/
│   │   │   │   ├── io-7.1.4.jar
│   │   │   │   ├── kernel-7.1.4.jar
│   │   │   │   ├── layout-7.1.4.jar
│   │   │   │   ├── slf4j-api-2.0.17.jar
│   │   │   │   └── slf4j-simple-2.0.17.jar
│   │   │   ├── sql/
│   │   │   │   ├── configuracion_bd_cajero.sql
│   │   │   │   ├── configuracion_cuenta_ejemplo.sql
│   │   │   │   └── otras_consultas.sql
│   │   │   ├── src/
│   │   │   │   ├── com/
│   │   │   │   │   ├── cajero/
│   │   │   │   │   │   ├── Cajero.java
│   │   │   │   │   │   └── gui/
│   │   │   │   │   │       ├── Alta.java
│   │   │   │   │   │       ├── Datos.java
│   │   │   │   │   │       ├── Inicio.java
│   │   │   │   │   │       ├── Main.java
│   │   │   │   │   │       ├── PIN.java
│   │   │   │   │   │       └── Reporte.java
│   │   │   │   │   └── managment/
│   │   │   │   │       ├── database/
│   │   │   │   │       │   └── DatabaseManager.java
│   │   │   │   │       ├── filemanager/
│   │   │   │   │       │   └── FileManager.java
│   │   │   │   │       └── utils/
│   │   │   │   │           ├── InputUtils.java
│   │   │   │   │           └── SystemUtils.java
│   │   │   │   ├── database.properties
│   │   │   │   └── confg.properties
│   │   │   └── README.md
│   │   └── README.md
│   └── README.md
└── README.md
```

Cada carpeta de proyecto contendrá:

* **Código Fuente**: Los archivos `.cpp`, `.py`, `.js`, etc., del proyecto según el lenguaje que utilize.
* **`README.md`**: Un archivo `README.md` específico para ese proyecto, detallando su propósito, cómo compilarlo/ejecutarlo, sus funcionalidades y cualquier otra nota relevante.
* **Archivos de Datos/Configuración**: Cualquier archivo adicional necesario para el funcionamiento del proyecto (ej. `cuentas.txt` para el cajero).

---

## Proyectos por Lenguaje

A continuación, se listan los lenguajes de programación y algunos de los proyectos que he desarrollado en cada uno:

### C++

* **Cajero: [Simulador de Cajero Automático](https://github.com/EktorOrmaexteaV/portafolio-proyectos/tree/main/practicas/practicas-cpp/Cajero)**:
    * **Descripción**: Un sistema de cajero automático por consola con gestión de cuentas, transacciones básicas (depósito, retiro, consulta de saldo, cambio de PIN) y registro de logs de usuario. Desarrollado en C++ usando POO.
    * **Tecnologías**: C++, Programación Orientada a Objetos, Manejo de Archivos.
    * **Aprendizajes Clave**: Refuerzo de lógica de programación, estructuras de control, manejo de archivos y POO.
---

* **CajeroBD: [Simulador de Cajero Automático con Base de Datos MySQL](https://github.com/EktorOrmaexteaV/portafolio-proyectos/tree/main/practicas/practicas-cpp/CajeroBD)**:
    * **Descripción**: Un simulador de cajero automático por consola que integra una base de datos MySQL para una gestión eficiente de cuentas y transacciones.
    * **Tecnologías**: C++, Programación Orientada a Objetos, Manejo de Bases de Datos (MySQL), Estructuras de Control.
    * **Aprendizajes Clave**: Refuerzo de lógica de programación, estructuras de control, manejo de bases de datos y POO.
---

### Java
* **Cajero: [Simulador de Cajero Automático en Entorno GUI](https://github.com/EktorOrmaexteaV/portafolio-proyectos/tree/main/practicas/practicas-java/Cajero)**:
    * **Descripción**: Un simulador de cajero automático desarrollado en Java utilizando el entorno GUI de Eclipse para la programación orientada a objetos (POO).
    * **Tecnologías**: Java, POO, Entorno GUI y MySQL. 
    * **Aprendizajes Clave**: Recordando programción en Java, POO, manejo de entornos GUI y bases de datos.

## Herramientas y Tecnologías que Utilizo

* **Lenguajes**: C++, Java.
* **Entornos de Desarrollo (IDE/Editor)**: Visual Studio Code, Code::Blocks, Eclipse, NetBeans.
* **Control de Versiones**: Git & GitHub.
* **Sistemas Operativos**: (Linux).
* **Servidores**: MySQL.

---

## Mi Objetivo

Este repositorio es una muestra de mi compromiso con el aprendizaje continuo y mi pasión por el desarrollo de software. Busca reflejar mi crecimiento y mis habilidades a lo largo del tiempo.

---

## Autor
**[Ektor Ormaetxea V](https://github.com/EktorOrmaexteaV)**

---

### Licencia
Proyecto libre para fines educativos y personales.
