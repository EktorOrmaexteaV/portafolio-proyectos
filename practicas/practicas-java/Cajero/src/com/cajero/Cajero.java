package com.cajero;

import com.managment.database.DatabaseManager;
import com.managment.filemanager.FileManager;
import com.managment.utils.*;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.logging.Logger;

import javax.swing.JOptionPane;

/**
 * Cajero.java
 * 
 * Clase principal del sistema de cajero automático.
 * Permite gestionar usuarios, cuentas y transacciones.
 * 
 * @author Ektor Ormaetxea V
 */
public class Cajero {
    private final DatabaseManager dbm;
    private static final Logger LOGGER = Logger.getLogger(DatabaseManager.class.getName());
    
    // Nombres de tablas
    private final String tablaUsuarios;		// Tabla de usuarios
    private final String tablaCuentas;      // Tabla de cuentas
    private final String tablaMovimientos;  // Tabla de transacciones/movimientos

    // Columnas de la tabla Usuarios
    private final String tuID;
    private final String tuNombre;
    private final String tuApellido;

    // Columnas de la tabla Cuentas
    private final String tcID;
    private final String tcUsuario;
    private final String tcPIN;
    private final String tcSaldo;    

    // Columnas de la tabla Movimientos
    private final String tmID;
    private final String tmCuenta;
    private final String tmUsuario;
    private final String tmDescripcion;    

    // Variables de sesión
    private String cuentaSesion;    		// ID de la cuenta con sesión iniciada
    private String IDusuarioSesion; 		// ID del usuario de la cuenta
    private String usuarioSesion;   		// Nombre y apellido del usuario de la cuenta
    private String pinSesion;       		// PIN de la cuenta con sesión iniciada
    private double saldoSesion;     		// Saldo actual (usar double por precisión en dinero)
    private boolean sesionActiva = false; 	// Indica si hay sesión activa
    
    // Constante para el monto máximo de depósito/retiro
    private static final double MONTO_MAXIMO = 1000.00;
    private static final int DIG_PIN = 4;
    private static final int DIG_CUENTA = 20;
    
    /**
     * Constructor de la clase Cajero.
     */
    public Cajero() {
        this.dbm = new DatabaseManager(); // Inicializa DatabaseManager aquí
        
        tablaUsuarios = "usuario";
        tuID = "ID";
        tuNombre = "Nombre";
        tuApellido = "Apellido";        

        tablaCuentas = "cuenta";
        tcID = "ID";
        tcUsuario = "ID_Usuario";
        tcPIN = "pin";
        tcSaldo = "monto";

        tablaMovimientos = "transaccion";
        tmID = "ID";
        tmCuenta = "ID_Cuenta";
        tmUsuario = "ID_Usuario";
        tmDescripcion = "descripcion";
        
        if (!dbm.connect()) {
            System.err.println("No se pudo conectar a la base de datos. Saliendo...");
            System.exit(1);
        }

    }
    
    // Getters para nombres de tablas
    public String getTablaUsuarios() { return tablaUsuarios; }
    public String getTablaCuentas() { return tablaCuentas; }
    public String getTablaMovimientos() { return tablaMovimientos; }

    // Getters para columnas de usuario
    public String getTuID() { return tuID; }
    public String getTuNombre() { return tuNombre; }
    public String getTuApellido() { return tuApellido; }    

    // Getters para columnas de cuenta
    public String getTcID() { return tcID; }
    public String getTcUsuario() { return tcUsuario; }
    public String getTcPIN() { return tcPIN; }
    public String getTcSaldo() { return tcSaldo; }    

    // Getters para columnas de movimientos
    public String getTmID() { return tmID; }
    public String getTmCuenta() { return tmCuenta; }
    public String getTmUsuario() { return tmUsuario; }
    public String getTmDescripcion() { return tmDescripcion; }

    // Getters para variables de sesión    
    /**
     * ID de la cuenta con sesión iniciada
     * @return ID de la cuenta con sesión iniciada
	 */	 
    public String getCuentaSesion() {
    	return cuentaSesion;
    }
    
    /**
     * ID del usuario de la cuenta con sesión iniciada
     * @return ID del usuario de la cuenta con sesión iniciada
     */
    public String getIDUsuarioSesion() {
    	return IDusuarioSesion;
    }
    
    /**
     * Nombre y apellido del usuario de la cuenta con sesión iniciada
     * @return Nombre y apellido del usuario de la cuenta con sesión iniciada
     */
    public String getUsuarioSesion() {
    	return usuarioSesion;
    }
    
    /**
     * PIN de la cuenta con sesión iniciada
     * @return PIN de la cuenta con sesión iniciada
     */
    public String getPinSesion() {
    	return pinSesion;
    }
    
    /**
     * Saldo actual de la cuenta con sesión iniciada
     * @return Saldo actual de la cuenta con sesión iniciada
     */
    public double getSaldoSesion() {
    	return saldoSesion;
    }
    
    /**
     * Indica si hay sesión activa.
     * @return true si hay sesión activa, false si no.
     */
    public boolean isSesionActiva() {
    	return sesionActiva;
    }
    
    /**
     * Obtiene el monto máximo permitido para depósitos y retiros.
     * @return El monto máximo permitido.
     */
    public double getMontoMaximo() {
		return MONTO_MAXIMO;
	}
    
    /**
     * Obtiene la cantidad de dígitos del PIN.
     * @return Cantidad de dígitos del PIN.
     */
    public int getDigitosPIN() {
    	return DIG_PIN;
    }
    
    /**
     * Obtiene la cantidad de dígitos de la cuenta.
     * @return Cantidad de dígitos de la cuenta.
     */
    public int getDigitosCuenta() {
		return DIG_CUENTA;
	}
    
    /**
	 * Cambia el PIN de la cuenta actual.
	 * 
	 * @param PINactual El PIN actual de la cuenta.
	 * @param cuenta El ID de la cuenta a la que se le cambiará el PIN.
	 * @param CuentaNueva Indica si es una cuenta nueva (0) o existente (1).
	 */
    public void cambiarPIN(String PINactual, String cuenta, int CuentaNueva) {
        String PIN;
        String nuevoPIN;

        // Validar que la cuenta existe
        if (CuentaNueva != 0) {
            PIN = InputUtils.getDigitsInput("Ingrese el PIN actual: ", DIG_PIN);
            while (!PIN.equals(PINactual)) {
                System.out.println("PIN incorrecto. Intente de nuevo.");
                PIN = InputUtils.getDigitsInput("Ingrese el PIN actual: ", DIG_PIN);
            }
        }
 
        nuevoPIN = InputUtils.getDigitsInput("Ingrese el nuevo PIN: ", DIG_PIN);
        String auxConfirmacion = InputUtils.getDigitsInput("Confirme el nuevo PIN: ", DIG_PIN);

        // Validar que el nuevo PIN no sea igual al actual
        while (nuevoPIN.equals(PINactual)) {
            System.out.println("El nuevo PIN no puede ser igual al actual. Intente de nuevo.");
            nuevoPIN = InputUtils.getDigitsInput("Ingrese el nuevo PIN: ", DIG_PIN);
            auxConfirmacion = InputUtils.getDigitsInput("Confirme el nuevo PIN: ", DIG_PIN);
        }

        // Validar que ambos PINs coincidan
        while (!nuevoPIN.equals(auxConfirmacion)) {
            System.out.println("PIN no coincide. Intente de nuevo.");
            nuevoPIN = InputUtils.getDigitsInput("Ingrese el nuevo PIN: ", DIG_PIN); // Volver a pedir ambos para evitar ciclos infinitos si el primero es erróneo
            auxConfirmacion = InputUtils.getDigitsInput("Confirme el nuevo PIN: ", DIG_PIN);
        }

        // Actualizar el PIN en la base de datos
        Map<String, String> setColumns = new HashMap<>();
        setColumns.put(getTcPIN(), nuevoPIN);
        Map<String, String> whereConditions = new HashMap<>();
        whereConditions.put(getTcID(), cuenta);

        // Si es una cuenta nueva, no se registra el evento de cambio de PIN
        if (dbm.updateData(getTablaCuentas(), setColumns, whereConditions)) {
            registrarEventos(Eventos.CAMBIO_PIN.name(), "Pin cambiado satisfactoriamente"); // Usando enum
            //reiniciarSesion(); // Llamar al método de instancia
            reiniciarSesion(cuenta, 0); // Reiniciar sesión con el nuevo PIN
        }
    }
    
    public void cambiarPIN(String nuevoPIN, String cuenta) {        // Actualizar el PIN en la base de datos
        Map<String, String> setColumns = new HashMap<>();
        setColumns.put(getTcPIN(), nuevoPIN);
        Map<String, String> whereConditions = new HashMap<>();
        whereConditions.put(getTcID(), cuenta);

        // Si es una cuenta nueva, no se registra el evento de cambio de PIN
        if (dbm.updateData(getTablaCuentas(), setColumns, whereConditions)) {
            registrarEventos(Eventos.CAMBIO_PIN.name(), "Pin cambiado satisfactoriamente"); // Usando enum            
        }
    }
 
    /**
     * Inicia sesión en el sistema.
     * @return true si la sesión se inicia correctamente, false si falla.
     */
    public boolean iniciarSesion() {
        String cuenta;       

        // Solicitar ID de cuenta al usuario
        cuenta = InputUtils.getDigitsInput("Ingrese ID de cuenta: ", DIG_CUENTA);     
        while (!confirmarCuenta(cuenta)) {
            System.out.println("Cuenta no encontrada. Intente nuevamente.");
            cuenta = InputUtils.getDigitsInput("Ingrese ID de cuenta: ", DIG_CUENTA);
        }

        // Si la cuenta existe, solicitar el PIN
        if (!confirmarPIN(cuenta, "Ingrese PIN: ")) { // Llamar al método de instancia
            return false; // Si el PIN es incorrecto, no se inicia sesión
        }
        sesionActiva = true;
        return true;
    }
    
     /**
      * Inicia sesión con una cuenta específica.
      * @param cuenta ID de la cuenta a iniciar sesión.
      * @return true si la sesión se inicia correctamente, false si falla.
      */
    public boolean iniciarSesion(String cuenta) {    
        if (!confirmarCuenta(cuenta)) {
            System.out.println("Cuenta no encontrada. No se puede iniciar sesión con la cuenta proporcionada.");
            return false;
        }

        // Si la cuenta existe, solicitar el PIN
        if (!confirmarPIN(cuenta, "Ingrese PIN: ")) { // Llamar al método de instancia
            System.out.println("PIN incorrecto.");
            return false; // Si el PIN es incorrecto, no se inicia sesión
        }
        sesionActiva = true;
        return true;
    }

    /**
     * Cierra la sesión actual del usuario.
     */
    public void cerrarSesion() {
        sesionActiva = false;
        cuentaSesion = null;
        IDusuarioSesion = null;
        saldoSesion = 0.0;
        pinSesion = null;
        usuarioSesion = null;
       
    }

    /**
	 * Consulta el saldo de la cuenta de sesión.
	 * Registra el evento de consulta de saldo.
	 * @param modo 0 para consola, 1 para mensaje en consola, 2 para ventana emergente.
	 */
    public void consultarSaldo(int modo) {
        double saldo = getSaldoSesion();
        String tipoEvento = Eventos.CONSULTA.name(); // Usando enum
        String detalle = "Consulta de saldo";
        registrarEventos(tipoEvento, saldo, detalle); // Usando la sobrecarga con monto

        DecimalFormat df = new DecimalFormat("0.00"); // Para formatear el saldo a dos decimales
        if (modo == 0) {
			System.out.println("El saldo de la cuenta es: €" + df.format(saldo));
			SystemUtils.pause();
		} else if (modo == 1) {
			LOGGER.info("Conuslta de saldo realizado");
		}else {
			JOptionPane.showMessageDialog(null, "El saldo de la cuenta es: €" + df.format(saldo), "Consulta de Saldo", JOptionPane.INFORMATION_MESSAGE);
		}
    }

    /**
     * Consulta el saldo de una cuenta específica.
     * @param cuenta ID de la cuenta a consultar.
     * @param modo 0 para consola, 1 para ventana emergente.
     */
    public void consultarSaldo(String cuenta, int modo) {
    	
    	// Verificar si la cuenta es la de sesión activa
        if (cuenta.equals(this.cuentaSesion) && this.sesionActiva) {
            consultarSaldo(modo); // Reutiliza el método que registra el evento
            return;
        }

        // Si no es la cuenta de sesión, consultar directamente en la base de datos
        String query = "SELECT " + tcSaldo + " FROM " + tablaCuentas + " WHERE " + tcID + " = '" + cuenta + "'";
        String saldoStr = dbm.executeScalar(query);
        try {
        	double saldo = Double.parseDouble(saldoStr);
            DecimalFormat df = new DecimalFormat("0.00");
            if(modo == 0) {            	
                System.out.println("El saldo de la cuenta es: €" + df.format(saldo));
                SystemUtils.pause();
            } else {
            	JOptionPane.showMessageDialog(null, "El saldo de la cuenta es: €" + df.format(saldo), "Consulta de Saldo", JOptionPane.INFORMATION_MESSAGE);
            }
        } catch (NumberFormatException e) {
            System.err.println("Error: Saldo no válido en la base de datos para cuenta " + cuenta);
        }
        registrarEventos(Eventos.CONSULTA.name(), 0, "Consulta de saldo de cuenta ");
    }

    /**
     * Realiza un depósito en una cuenta específica.
     * @param cuenta ID de la cuenta a la que se le realizará el depósito.
     * @param monto Monto a depositar en la cuenta.
     */
    public void realizarDeposito(String cuenta, double monto) {
    	double saldoActual = getSaldoSesion(); // Usar saldoSesion para el estado actual
    	if(monto <= 0) {
            System.err.println("El monto a depositar debe ser positivo.");
    	}
    	
    	if(monto > MONTO_MAXIMO) {
            System.err.println("El monto ingresado es mayor al monto máximo permitido: " + MONTO_MAXIMO + "€");			
            return;
        }
    	
    	double nuevoSaldo = saldoActual + monto;
        Map<String, String> setColumns = new HashMap<>();	
        setColumns.put(tcSaldo, String.valueOf(nuevoSaldo)); // Convertir double a String	
        Map<String, String> whereConditions = new HashMap<>();	
        whereConditions.put(tcID, cuenta);
        
        if (dbm.updateData(tablaCuentas, setColumns, whereConditions)) {
            // Actualizar saldo de sesión inmediatamente después de la operación exitosa en DB
            this.saldoSesion = nuevoSaldo;

            DecimalFormat df = new DecimalFormat("0.00");
            String montoString = df.format(monto);
            String detalle = "Depósito de €" + montoString;
            registrarEventos(Eventos.INGRESO.name(), monto, detalle);			
            //consultarSaldo(cuenta);
            consultarSaldo(cuenta, 1);
        } else {
            System.err.println("Error al realizar el depósito.");
            registrarEventos(Eventos.TRANSACCION_RECHAZADA.name(), monto, "Depósito fallido");			
        }
    }
    
    /**
	 * Realiza un depósito en la cuenta de sesión activa.
	 * Registra el evento de depósito.
	 * 
	 * @param cuenta ID de la cuenta a la que se le realizará el depósito.
	 */
    public void realizarDeposito(String cuenta) {
        double saldoActual = getSaldoSesion(); // Usar saldoSesion para el estado actual
        System.out.print("Ingrese monto a depositar: ");
        double monto = InputUtils.getFloatInput(""); // getFloatInput ya tiene prompt, pero puedes pasarlo vacío

        if (monto <= 0) {
            System.out.println("El monto a depositar debe ser positivo.");
            SystemUtils.pause();
            return;
        }

        if (monto > MONTO_MAXIMO) { // Usar la constante
            System.out.println("El monto ingresado es mayor al monto máximo permitido: " + MONTO_MAXIMO + "€");
            SystemUtils.pause();
            return;
        }

        double nuevoSaldo = saldoActual + monto;

        Map<String, String> setColumns = new HashMap<>();
        setColumns.put(tcSaldo, String.valueOf(nuevoSaldo)); // Convertir double a String
        Map<String, String> whereConditions = new HashMap<>();
        whereConditions.put(tcID, cuenta);

        if (dbm.updateData(tablaCuentas, setColumns, whereConditions)) {
            // Actualizar saldo de sesión inmediatamente después de la operación exitosa en DB
            this.saldoSesion = nuevoSaldo;

            DecimalFormat df = new DecimalFormat("0.00");
            String montoString = df.format(monto);
            String detalle = "Depósito de €" + montoString;
            registrarEventos(Eventos.INGRESO.name(), monto, detalle); // Usando enum
            SystemUtils.clearScreen();
            System.out.println("Depósito realizado.");
            //consultarSaldo(); // Muestra el saldo actualizado y registra la consulta
            consultarSaldo(0); // Muestra el saldo actualizado en ventana emergente
            //SystemUtils.pause();
        } else {
            System.err.println("Error al realizar el depósito.");
            registrarEventos(Eventos.TRANSACCION_RECHAZADA.name(), monto, "Depósito fallido");
            SystemUtils.pause();
        }
    }

    /**	 * Realiza un retiro de la cuenta de sesión activa.
	 * Registra el evento de retiro.
	 * 
	 * @param cuenta ID de la cuenta de la que se realizará el retiro.
	 */
    public void realizarRetiro(String cuenta) {
        double saldoActual = getSaldoSesion(); // Usar saldoSesion para el estado actual
        System.out.print("Ingrese monto a retirar: ");
        double monto = InputUtils.getFloatInput(""); // getFloatInput ya tiene prompt, pero puedes pasarlo vacío

        if (monto <= 0) {
            System.out.println("El monto a retirar debe ser positivo.");
            SystemUtils.pause();
            return;
        }

        if (monto > saldoActual) {
            SystemUtils.clearScreen();
            System.out.println("Saldo insuficiente");
            registrarEventos(Eventos.TRANSACCION_RECHAZADA.name(), monto, "Retiro fallido por saldo insuficiente");
            SystemUtils.pause();
        } else if (monto > MONTO_MAXIMO) {
            System.out.println("El monto ingresado es mayor al monto máximo permitido: " + MONTO_MAXIMO + "€");
            SystemUtils.pause();
        } else {
            double nuevoSaldo = saldoActual - monto;

            Map<String, String> setColumns = new HashMap<>();
            setColumns.put(tcSaldo, String.valueOf(nuevoSaldo)); // Convertir double a String
            Map<String, String> whereConditions = new HashMap<>();
            whereConditions.put(tcID, cuenta);

            if (dbm.updateData(tablaCuentas, setColumns, whereConditions)) {
                // Actualizar saldo de sesión inmediatamente después de la operación exitosa en DB
                this.saldoSesion = nuevoSaldo;

                DecimalFormat df = new DecimalFormat("0.00");
                String montoString = df.format(-monto); // Para que se muestre negativo en el detalle
                String detalle = "Retiro de €" + montoString;
                registrarEventos(Eventos.RETIRO.name(), -monto, detalle); // Monto negativo para el registro
                SystemUtils.clearScreen();
                System.out.println("Retiro realizado.");
                consultarSaldo(0); // Muestra el saldo actualizado y registra la consulta
                //SystemUtils.pause();
            } else {
                System.err.println("Error al realizar el retiro.");
                registrarEventos(Eventos.TRANSACCION_RECHAZADA.name(), monto, "Retiro fallido");
                SystemUtils.pause();
            }
        }
    }
    
    public void realizarRetiro(String cuenta, double monto) {
    	double saldoActual = getSaldoSesion(); // Usar saldoSesion para el estado actual
    	if(monto <= 0) {
    		System.err.println("El monto a retirar debe ser positivo.");
			return;
    	}
    	if(monto > MONTO_MAXIMO) {
    		System.err.println("El monto ingresado es mayor al monto máximo permitido: " + MONTO_MAXIMO + "€");
    	}
    	double nuevoSaldo = saldoActual - monto;
    	
    	Map<String, String> setColumns = new HashMap<>();
		setColumns.put(tcSaldo, String.valueOf(nuevoSaldo)); // Convertir double a String
		Map<String, String> whereConditions = new HashMap<>();
		whereConditions.put(tcID, cuenta);
		
		if (dbm.updateData(tablaCuentas, setColumns, whereConditions)) {
			// Actualizar saldo de sesión inmediatamente después de la operación exitosa en DB
			this.saldoSesion = nuevoSaldo;
			DecimalFormat df = new DecimalFormat("0.00");
			String montoString = df.format(-monto); // Para que se muestre negativo en el detalle
			String detalle = "Retiro de €" + montoString;
			registrarEventos(Eventos.RETIRO.name(), -monto, detalle); // Monto negativo para el registro
			consultarSaldo(cuenta, 1);
		} else {
			System.err.println("Error al realizar el retiro.");
			registrarEventos(Eventos.TRANSACCION_RECHAZADA.name(), monto, "Retiro fallido");
		}
		
    }
    
    /**
	 * Ejecuta una consulta SQL y devuelve los resultados.
	 * 
	 * @param query La consulta SQL a ejecutar.
	 * @return Una lista de mapas, donde cada mapa representa una fila de resultados.
	 */
    public List<Map<String, String>> ejecutarConsultaSQL(String query) {
        return dbm.fetchQuery(query);
    }

    /*
     * Genera un reporte de la cuenta especificada.
     * 
     * @param cuenta ID de la cuenta para la que se generará el reporte.
     */
    public void generarReporte(String cuenta) {
		StringBuilder reporte = new StringBuilder();

		// Query para obtener el saldo actual y el nombre del usuario
		String query1 = "SELECT " +
                "    CONCAT(u.Nombre, ' ', u.Apellido) AS usuario," +
                "    c.monto AS saldo_actual " +
                "FROM " +
                "    "+tablaCuentas+" c " +
                "JOIN " +
                "    "+tablaUsuarios+" u ON c."+tcUsuario+" = u."+tuID+" " +
                "WHERE " +
                "    c."+tcID+" = '" + cuenta + "';";

		// Query para obtener las transacciones INGRESO, RETIRO y CONSULTA de la cuenta
        String query2 = "SELECT " +
            "    t.fecha, " +
            "    t.monto AS monto_transaccion, " +
            "    t.descripcion " +
            "FROM " +
            "    cuenta c " +
            "JOIN " +
            "    usuario u ON c.ID_Usuario = u.ID " +
            "JOIN " +
            "    transaccion t ON t.ID_Cuenta = c.ID " +
            "WHERE " +
            "    c.ID = '" + cuenta + "' " +
            "    AND t.tipo_evento IN ('INGRESO', 'RETIRO', 'CONSULTA') " +
            "ORDER BY " +
            "    t.fecha;";

        // Ejecutar las consultas
    	List<Map<String, String>> resultados1 = ejecutarConsultaSQL(query1);
        List<Map<String, String>> resultados2 = ejecutarConsultaSQL(query2);

        // Verificar si hay resultados en la primera consulta
		if (!resultados1.isEmpty()) {
			for (Map<String, String> row : resultados1) {
				String usuario = row.get("usuario");
                                String c = "Cuenta: " + cuenta;
                                String saldoActual = row.get("saldo_actual");

                                reporte.append(String.format("\n%s \n\n",SystemUtils.getActualTime(1)));
                                
                                reporte.append(String.format("Nombre de cliente: %s \n\n",usuario));
                                
				int anchoCampo = 67;
				int espaciosTotales = anchoCampo - c.length();
				int espaciosIzquierda = espaciosTotales / 2;
				int espaciosDerecha = espaciosTotales - espaciosIzquierda;
				String cuentaCentrado = " ".repeat(Math.max(0, espaciosIzquierda)) + c + " ".repeat(Math.max(0, espaciosDerecha));
                                
				reporte.append("+---------------------------------------------------------------------+\n");
				reporte.append(String.format("| %s |\n", cuentaCentrado));
				reporte.append("+---------------------------------------------------------------------+\n");
				reporte.append(String.format("| Saldo actual: %-53s |\n", saldoActual));

                                
				int anchoFecha = 25;
				int espaciosTotalesF = anchoFecha - "Fecha".length();
				int espaciosIzquierdaF = espaciosTotalesF / 2;
				int espaciosDerechaF = espaciosTotalesF - espaciosIzquierdaF;
				String fechaCentrada = " ".repeat(Math.max(0, espaciosIzquierdaF)) + "Fecha" + " ".repeat(Math.max(0, espaciosDerechaF));

				int anchoMonto = 10;
				int espaciosTotalesM = anchoMonto - "Monto".length();
				int espaciosIzquierdaM = espaciosTotalesM / 2;
				int espaciosDerechaM = espaciosTotalesM - espaciosIzquierdaM;
				String montoCentrado = " ".repeat(Math.max(0, espaciosIzquierdaM)) + "Monto" + " ".repeat(Math.max(0, espaciosDerechaM));

				int anchoDescripcion = 30;
				int espaciosTotalesD = anchoDescripcion - "Descripción".length();
				int espaciosIzquierdaD = espaciosTotalesD / 2;
				int espaciosDerechaD = espaciosTotalesD - espaciosIzquierdaD;
				String descripcionCentrada = " ".repeat(Math.max(0, espaciosIzquierdaD)) + "Descripción" + " ".repeat(Math.max(0, espaciosDerechaD));
				
				reporte.append("+---------------------------------------------------------------------+\n");
				reporte.append(String.format("|%-26s|%11s|%-30s|\n", fechaCentrada, montoCentrado, descripcionCentrada));
				reporte.append("+---------------------------------------------------------------------+\n");
			}
		}

		// Verificar si hay resultados en la segunda consulta
        if (!resultados2.isEmpty()) {               
			for (Map<String, String> row2 : resultados2) {
				String fecha = row2.get("fecha");
				String montoTransaccion = row2.get("monto_transaccion");
				String descripcion = row2.get("descripcion");
				reporte.append(String.format("| %-25s|%10s | %-29s|\n", fecha, montoTransaccion, descripcion));				
			}									
		}else{
			System.out.println("No se encontraron resultados para la consulta.");
		}
        
        reporte.append("+---------------------------+----------+------------------------------+\n");            
		System.out.println(reporte.toString());                
		String nombreArchivo = "reporte_" + SystemUtils.getActualTime(0) + "_" + cuenta + ".pdf";                
		System.out.printf("Nombre de archivo: %s\n", nombreArchivo);                
		generarArchivoReporte(cuenta, nombreArchivo, reporte.toString());
    }
    
    /**
     * Genera un reporte de la cuenta especificada y devuelve el contenido como una cadena.
     * @param cuenta ID de la cuenta para la que se generará el reporte.
     * @return El contenido del reporte como una cadena.
     */
    public String generarReporteString(String cuenta) {
    	StringBuilder reporte = new StringBuilder();
    	
    	// Query para obtener el saldo actual y el nombre del usuario    	
		String query1 = "SELECT " +
                "    CONCAT(u.Nombre, ' ', u.Apellido) AS usuario," +
                "    c.monto AS saldo_actual " +
                "FROM " +
                "    "+tablaCuentas+" c " +
                "JOIN " +
                "    "+tablaUsuarios+" u ON c."+tcUsuario+" = u."+tuID+" " +
                "WHERE " +
                "    c."+tcID+" = '" + cuenta + "';";
		
		// Query para obtener las transacciones INGRESO, RETIRO y CONSULTA de la cuenta
		String query2 = "SELECT " +
	            "    t.fecha, " +
	            "    t.monto AS monto_transaccion, " +
	            "    t.descripcion " +
	            "FROM " +
	            "    cuenta c " +
	            "JOIN " +
	            "    usuario u ON c.ID_Usuario = u.ID " +
	            "JOIN " +
	            "    transaccion t ON t.ID_Cuenta = c.ID " +
	            "WHERE " +
	            "    c.ID = '" + cuenta + "' " +
	            "    AND t.tipo_evento IN ('INGRESO', 'RETIRO', 'CONSULTA') " +
	            "ORDER BY " +
	            "    t.fecha;";
		
		// Ejecutar las consultas
		List<Map<String, String>> resultados1 = ejecutarConsultaSQL(query1);
        List<Map<String, String>> resultados2 = ejecutarConsultaSQL(query2);
        
     // Verificar si hay resultados en la primera consulta
        if (!resultados1.isEmpty()) {
        	for (Map<String, String> row : resultados1) {
        		String usuario = row.get("usuario");
        		String c = "Cuenta: " + cuenta;
                String saldoActual = row.get("saldo_actual");
                reporte.append(String.format("\n%s \n\n",SystemUtils.getActualTime(1)));                
                reporte.append(String.format("Nombre de cliente: %s \n\n",usuario));

                int anchoCampo = 67;
				int espaciosTotales = anchoCampo - c.length();
				int espaciosIzquierda = espaciosTotales / 2;
				int espaciosDerecha = espaciosTotales - espaciosIzquierda;
				String cuentaCentrado = " ".repeat(Math.max(0, espaciosIzquierda)) + c + " ".repeat(Math.max(0, espaciosDerecha));
				
				reporte.append("+---------------------------------------------------------------------+\n");
				reporte.append(String.format("| %s |\n", cuentaCentrado));
				reporte.append("+---------------------------------------------------------------------+\n");
				reporte.append(String.format("| Saldo actual: %-53s |\n", saldoActual));

                                
				int anchoFecha = 25;
				int espaciosTotalesF = anchoFecha - "Fecha".length();
				int espaciosIzquierdaF = espaciosTotalesF / 2;
				int espaciosDerechaF = espaciosTotalesF - espaciosIzquierdaF;
				String fechaCentrada = " ".repeat(Math.max(0, espaciosIzquierdaF)) + "Fecha" + " ".repeat(Math.max(0, espaciosDerechaF));

				int anchoMonto = 10;
				int espaciosTotalesM = anchoMonto - "Monto".length();
				int espaciosIzquierdaM = espaciosTotalesM / 2;
				int espaciosDerechaM = espaciosTotalesM - espaciosIzquierdaM;
				String montoCentrado = " ".repeat(Math.max(0, espaciosIzquierdaM)) + "Monto" + " ".repeat(Math.max(0, espaciosDerechaM));

				int anchoDescripcion = 30;
				int espaciosTotalesD = anchoDescripcion - "Descripción".length();
				int espaciosIzquierdaD = espaciosTotalesD / 2;
				int espaciosDerechaD = espaciosTotalesD - espaciosIzquierdaD;
				String descripcionCentrada = " ".repeat(Math.max(0, espaciosIzquierdaD)) + "Descripción" + " ".repeat(Math.max(0, espaciosDerechaD));
				
				reporte.append("+---------------------------------------------------------------------+\n");
				reporte.append(String.format("|%-26s|%11s|%-30s|\n", fechaCentrada, montoCentrado, descripcionCentrada));
				reporte.append("+---------------------------------------------------------------------+\n");
        	}
        }
        
        // Verificar si hay resultados en la segunda consulta
        if (!resultados2.isEmpty()) {
        	for (Map<String, String> row2 : resultados2) {
        		String fecha = row2.get("fecha");
				String montoTransaccion = row2.get("monto_transaccion");
				String descripcion = row2.get("descripcion");
				reporte.append(String.format("| %-25s|%10s | %-29s|\n", fecha, montoTransaccion, descripcion));				
        	}
        } else {
        	System.out.println("No se encontraron resultados para la consulta.");
        }
        reporte.append("+---------------------------+----------+------------------------------+\n");            
        String nombreArchivo = "reporte_" + SystemUtils.getActualTime(0) + "_" + cuenta + ".pdf";
        generarArchivoReporte(cuenta, nombreArchivo, reporte.toString());
        
        return reporte.toString();        
    }
    
    

    /**
	 * Genera un archivo de reporte en formato PDF.
	 * 
	 * @param cuenta ID de la cuenta para la que se generará el reporte.
	 * @param nombreArchivo Nombre del archivo donde se guardará el reporte.
	 * @param reporte Contenido del reporte a guardar.
	 */
    public void generarArchivoReporte(String cuenta, String nombreArchivo, String reporte) {               
        FileManager.saveAsPDF(nombreArchivo, reporte);        
    }

    /**
     * Registra un movimiento de ingreso o retiro.
     * @param detalle Descripción del movimiento.
     * @param monto Monto del movimiento.
     * @param retiro 1 para retiro, 0 para ingreso.
     */
    public void registrarMovimiento(String detalle, double monto, int retiro) {
        String tipoEvento;
        if (retiro == 1) {
            tipoEvento = Eventos.RETIRO.name();
        } else {
            tipoEvento = Eventos.INGRESO.name();
        }
        registrarEventos(tipoEvento, monto, detalle);
    }

    /**
	 * Registra un evento en la tabla de movimientos sin monto.
	 * 
	 * @param tipoEvento Tipo de evento (INGRESO, RETIRO, CONSULTA, etc.).
	 * @param detalle Detalle del evento.
	 */
    public void registrarEventos(String tipoEvento, String detalle) {
        List<String> columns = new ArrayList<>();
        List<String> values = new ArrayList<>();

        columns.add(tmCuenta); values.add(cuentaSesion);
        columns.add(tmUsuario); values.add(IDusuarioSesion);
        columns.add("tipo_evento"); values.add(tipoEvento);
        columns.add(tmDescripcion); values.add(detalle);

        dbm.insertDataByList(tablaMovimientos, columns, values);
    }


    /**
     * Registra un evento en la tabla de movimientos con monto.
     * @param tipoEvento Tipo de evento (INGRESO, RETIRO, CONSULTA, etc.).
     * @param monto Monto del evento (puede ser positivo o negativo).
     * @param detalle Detalle del evento.
     */
    public void registrarEventos(String tipoEvento, double monto, String detalle) {
        List<String> columns = new ArrayList<>();
        List<String> values = new ArrayList<>();

        columns.add(tmCuenta); values.add(cuentaSesion);
        columns.add(tmUsuario); values.add(IDusuarioSesion);
        columns.add("tipo_evento"); values.add(tipoEvento);
        columns.add("monto"); values.add(String.valueOf(monto)); // Convertir double a String
        columns.add(tmDescripcion); values.add(detalle);

        dbm.insertDataByList(tablaMovimientos, columns, values);
    }

    /**
	 * Da de alta un nuevo cliente en el sistema.
	 * Si el cliente ya existe, pregunta si desea abrir una cuenta.
	 * 
	 * @param IDCliente ID del cliente a dar de alta.
	 */
    public void altaCliente(String IDCliente) {
        String query = "SELECT * FROM " + tablaUsuarios + " WHERE " + tuID + " = '" + IDCliente + "'";        
        if (!dbm.fetchQuery(query).isEmpty()) {
            System.out.println("El cliente ya existe");
            System.out.println("¿Desea abrir cuenta?\n1. Sí\n2. No");
            int opcion = InputUtils.getIntInput("> ");
            switch (opcion) {
                case 1 -> {       
                    String checkAccountQuery = "SELECT * FROM " + tablaCuentas + " WHERE " + tcUsuario + " = '" + IDCliente + "'";
                    if (dbm.fetchQuery(checkAccountQuery).isEmpty()) {
                        System.out.println("Cliente " + IDCliente + " no tiene ninguna cuenta.");
                        System.out.println("Creando cuenta...");
                        abrirCuenta(IDCliente);                        
                    } else {
                        System.out.println("Cliente " + IDCliente + " ya tiene una cuenta.");                    
                        SystemUtils.pause();
                    }                    
                }                
            }
        } else {
            System.out.print("Ingrese el/los nombre(s) del cliente: ");
            String nombre = InputUtils.getStringInput(); // Lee la línea completa
            System.out.print("Ingrese los apellidos del cliente: ");
            String apellidos = InputUtils.getStringInput(); // Lee la línea completa

            List<String> columns = new ArrayList<>();
            columns.add(tuID); columns.add(tuNombre); columns.add(tuApellido);
            List<String> values = new ArrayList<>();
            values.add(IDCliente); values.add(nombre); values.add(apellidos);

            if (dbm.insertDataByList(tablaUsuarios, columns, values)) {
                SystemUtils.clearScreen();
                System.out.println("Cliente dado de alta exitosamente.");
                SystemUtils.pause();
                abrirCuenta(IDCliente); // Después de dar de alta, abre una cuenta
            } else {
                SystemUtils.clearScreen();
                System.out.println("Error al dar de alta el cliente.");
                SystemUtils.pause();
            }
        }
    }
    
    /**
     * Da de alta un nuevo cliente en el sistema. Contiene mensajes de dialogo para la interfaz gráfica.
     * Si el cliente ya existe, pregunta si desea abrir una cuenta.  
     * @param IDCliente ID del cliente a dar de alta.
     * @param nombre Nombre del cliente.
     * @param apellidos Apellidos del cliente.
     * @param saldoInicial Saldo inicial de la cuenta del cliente.
     */
    public void altaCliente(String IDCliente, String nombre, String apellidos, String saldoInicial) {    
        // Query para verificar si el cliente ya existe
    	String query = "SELECT * FROM " + tablaUsuarios + " WHERE " + tuID + " = '" + IDCliente + "'";    	
        if (!dbm.fetchQuery(query).isEmpty()) {
            // Si el cliente ya existe, preguntar si desea abrir una cuenta
            int respuesta = JOptionPane.showConfirmDialog(
                    null,
                    "El cliente ya existe. ¿Desea abrir una cuenta?",
                    "Confirmación",
                    JOptionPane.YES_NO_OPTION
                    
            );
            
            if(respuesta == JOptionPane.YES_OPTION) {
                
                String checkAccountQuery = "SELECT * FROM " + tablaCuentas + " WHERE " + tcUsuario + " = '" + IDCliente + "'";		
                if (dbm.fetchQuery(checkAccountQuery).isEmpty()) {
                    JOptionPane.showMessageDialog(
                            null,
                            "Cliente " + IDCliente + " no tiene ninguna cuenta. Creando cuenta...",
                            "Información",
                            JOptionPane.INFORMATION_MESSAGE
                    );
                    abrirCuenta(IDCliente, saldoInicial); // Abre una cuenta para el cliente existente
                } else {
                    JOptionPane.showMessageDialog(null,
                            "Cliente " + IDCliente + 
                            " ya tiene una cuenta.",
                            "Información",
                            JOptionPane.INFORMATION_MESSAGE
                    );
                }
                
            } else {
                JOptionPane.showMessageDialog(null,
                        "Operación cancelada.",
                        "Información",
                        JOptionPane.INFORMATION_MESSAGE
                );
            }
        		
        } else {
            List<String> columns = new ArrayList<>();
            columns.add(tuID); columns.add(tuNombre); columns.add(tuApellido);
            List<String> values = new ArrayList<>();
            values.add(IDCliente); values.add(nombre); values.add(apellidos);

            if (dbm.insertDataByList(tablaUsuarios, columns, values)) {
                JOptionPane.showMessageDialog(
                        null,
                        "Cliente dado de alta exitosamente.",
                        "Éxito",
                        JOptionPane.INFORMATION_MESSAGE
                    );				
                    abrirCuenta(IDCliente, saldoInicial);
                    consultarSaldo(1);                    
            } else {
                JOptionPane.showMessageDialog(
                        null,
                        "Error al dar de alta el cliente.",
                        "Error",
                        JOptionPane.ERROR_MESSAGE
                    );
            }
        }
    }
    
    /**
     * Abre una nueva cuenta para un cliente con un saldo inicial. Adapado para la interfaz gráfica.
     * @param clienteID ID del cliente para el que se abrirá la cuenta.
     * @param saldoInicial Saldo inicial de la cuenta del cliente.
     */
    public void abrirCuenta(String clienteID, String saldoInicial) {
		String cuentaID = InputUtils.generateNumberString(DIG_CUENTA, 0);
		String pin = InputUtils.generateNumberString(DIG_PIN, 1);

		// Validar el saldo inicial
		double saldoInicialDouble;
		try {
			saldoInicialDouble = Double.parseDouble(saldoInicial);
			if (saldoInicialDouble > MONTO_MAXIMO) {
				JOptionPane.showMessageDialog(
						null,
						"Error: El saldo inicial no puede ser mayor que " + MONTO_MAXIMO + "€",
						"Error",
						JOptionPane.ERROR_MESSAGE
				);
				return;
			}
		} catch (NumberFormatException e) {
			JOptionPane.showMessageDialog(
					null,
					"Error: Saldo inicial no válido.",
					"Error",
					JOptionPane.ERROR_MESSAGE
			);
			return;
		}

		// Formatear el número con punto decimal
		String saldo = String.format(Locale.US, "%.2f", saldoInicialDouble);

		List<String> columns = Arrays.asList(tcID, tcUsuario, tcPIN, tcSaldo);
		List<String> values = Arrays.asList(cuentaID, clienteID, pin, saldo);

		if (dbm.insertDataByList(tablaCuentas, columns, values)) {
			SystemUtils.clearScreen();
			if (confirmarCuenta(cuentaID)) {
				JOptionPane.showMessageDialog(
						null,
						"Cuenta creada exitosamente.",
						"Éxito",
						JOptionPane.INFORMATION_MESSAGE
				);
			} else {
				JOptionPane.showMessageDialog(
						null,
						"Error al cargar la sesión.",
						"Error",
						JOptionPane.ERROR_MESSAGE
				);
			}
		} else {
			JOptionPane.showMessageDialog(
					null,
					"Error al crear la cuenta.",
					"Error",
					JOptionPane.ERROR_MESSAGE
			);
		}
	}

    /**
	 * Abre una nueva cuenta para un cliente.
	 * Genera un ID de cuenta y un PIN aleatorio.
	 * 
	 * @param clienteID ID del cliente para el que se abrirá la cuenta.
	 */
    public void abrirCuenta(String clienteID) {
        String cuentaID = InputUtils.generateNumberString(DIG_CUENTA, 0);        
        String pin = InputUtils.generateNumberString(DIG_PIN, 1);

        System.out.print("Ingrese saldo inicial: ");
        double saldoInicial = InputUtils.getDoubleInput("");
        
        if(saldoInicial > MONTO_MAXIMO) {
            System.out.printf("Error: El saldo inicial no puede ser mayor que %.2f%n", MONTO_MAXIMO);
            SystemUtils.pause();
            return;
        }

        // Formatear el número con punto decimal
        String saldo = String.format(Locale.US, "%.2f", saldoInicial);

        List<String> columns = Arrays.asList(tcID, tcUsuario, tcPIN, tcSaldo);
        List<String> values = Arrays.asList(cuentaID, clienteID, pin, saldo);

        if (dbm.insertDataByList(tablaCuentas, columns, values)) {
            SystemUtils.clearScreen();
            if (confirmarCuenta(cuentaID)) {
                System.out.println("Cuenta creada exitosamente.");
                SystemUtils.pause();
            } else {
                System.err.println("Error al cargar la sesión.");
                SystemUtils.pause();
            }
        } else {
            System.err.println("Error al crear la cuenta.");
            SystemUtils.pause();
        }
    }
    
    /**
	 * Confirma si una cuenta existe y carga la sesión.
	 * 
	 * @param cuenta ID de la cuenta a confirmar.
	 * @return true si la cuenta existe y se carga la sesión, false en caso contrario.
	 */
    public boolean confirmarCuenta(String cuenta) {
        String query = "SELECT c." + tcID + ", u." + tuID + ", u." + tuNombre + ", u." + tuApellido + ", c." + tcSaldo + ", c." + tcPIN + " "
                + "FROM " + tablaCuentas + " c " // Alias para tablaCuentas
                + "JOIN " + tablaUsuarios + " u ON u." + tuID + " = c." + tcUsuario + " " // Alias para tablaUsuarios
                + "WHERE c." + tcID + " = '" + cuenta + "'";

        List<Map<String, String>> resultados = dbm.fetchQuery(query);

        if (!resultados.isEmpty()) {
            Map<String, String> registro = resultados.get(0); // Obtener el primer resultado
            cuentaSesion = cuenta;
            IDusuarioSesion = registro.get(tuID); // Asumo que `tuID` es la columna ID en la tabla `usuario`
            usuarioSesion = registro.get(tuNombre) + " " + registro.get(tuApellido); // Nombre completo
            try {
                saldoSesion = Double.parseDouble(registro.get(tcSaldo)); // Convertir String a double
            } catch (NumberFormatException e) {
                System.err.println("Advertencia: Saldo no válido en la base de datos para la cuenta " + cuenta + ". Se usará 0.0.");
                saldoSesion = 0.0;
            }
            pinSesion = registro.get(tcPIN);
            sesionActiva = true;
            return true;
        } else {
            sesionActiva = false;
            return false;
        }
    }

    // Funciones de utilidad
    /**
	 * Reinicia la sesión actual cerrando la sesión y volviendo a iniciar con una cuenta específica.
	 * @param cuenta ID de la cuenta con la que se reiniciará la sesión.
	 * @param modo Modo de reinicio: 0 para consola, 1 para interfaz gráfica. 
	 */
    public void reiniciarSesion(String cuenta, int modo) {
        if (modo == 0) {
        	SystemUtils.clearScreen();
            System.out.println("Reiniciando sesión...");
            SystemUtils.pause();
            cerrarSesion(); // Cierra la sesión actual
            if (!iniciarSesion(cuenta)) { // Intenta iniciar sesión con la cuenta proporcionada
                System.out.println("Ha ocurrido un error al reiniciar la sesión.");
            }
        } else {
        	JOptionPane.showMessageDialog(null,"Reiniciando sesión...","Información",JOptionPane.INFORMATION_MESSAGE);
            cerrarSesion(); // Cierra la sesión actual
            if (!iniciarSesion(cuenta)) { // Intenta iniciar sesión con la cuenta proporcionada
            	JOptionPane.showMessageDialog(null,"Ha ocurrido un error al reiniciar la sesión.","Error",JOptionPane.ERROR_MESSAGE);
            }
        }
    }


    /**
	 * Confirma el PIN de una cuenta.
	 * Permite hasta 3 intentos para ingresar el PIN correcto.
	 * 
	 * @param cuenta ID de la cuenta a confirmar.
	 * @param mensaje Mensaje que se muestra al solicitar el PIN.
	 * @return true si el PIN es correcto, false si se superan los intentos.
	 */
    public boolean confirmarPIN(String cuenta, String mensaje) {
        for (int i = 0; i < 3; i++) {
            System.out.println("Intento " + (i + 1) + "/3:");
            String pin = InputUtils.getDigitsInput(mensaje, DIG_PIN);

            String query = "SELECT * FROM " + getTablaCuentas() +
                    " WHERE " + getTcID() + " = '" + cuenta +
                    "' AND " + getTcPIN() + " = '" + pin + "'";
            List<Map<String, String>> resultados = dbm.fetchQuery(query);

            if (!resultados.isEmpty()) {
                // PIN correcto
                return true;
            } else {
                System.out.println("PIN incorrecto. Intente de nuevo.");
            }
        }
        System.out.println("Ha superado el número máximo de intentos.");
        registrarEventos(Eventos.INTENTO_FALLIDO.name(), "PIN incorrecto en intento de acceso"); // Usando enum
        return false;
    }

    /**
     * Enum para representar los diferentes eventos del sistema.
     */
    public enum Eventos {        
        INICIO_SESION,
        CIERRE_SESION,
        INGRESO,
        RETIRO,
        CONSULTA,
        CAMBIO_PIN,
        INTENTO_FALLIDO,
        CAMBIO_PIN_FALLIDO,
        TRANSACCION_RECHAZADA
    }

}
