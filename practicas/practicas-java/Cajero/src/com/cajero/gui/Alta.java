package com.cajero.gui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

import com.cajero.Cajero;

/**
 * Alta.java
 * Ventana para dar de alta una nueva cuenta en el cajero automático.
 * Permite al usuario ingresar sus datos personales y saldo inicial.
 * El usuario debe de llenar todos los campos obligatorios 
 * y el saldo no puede ser negativo ni superar el máximo permitido.
 * 
 * @author Ektor Ormaetxea V
 */
public class Alta {
	Cajero cajero = new Cajero();

	private JFrame frmAlta;
	private JTextField txtID;
	private JTextField txtNombre;
	private JTextField txtApellido;
	private JTextField txtSaldo;
	
	private String cuenta;
    private String PIN;
    private final double MAX_SALDO = cajero.getMontoMaximo();
   /**
    * Constructor de la clase Alta.
    */
    public Alta() {iniciar();}
    
	/**
	 * Método para inicializar la ventana de alta.
	 */
    private void iniciar() {
		frmAlta = new JFrame();
		frmAlta.getContentPane().setBackground(new Color(60, 63, 65));
		frmAlta.getContentPane().setLayout(null);
		
		JPanel panel1 = new JPanel();
		panel1.setBackground(new Color(204, 204, 204));
		panel1.setBounds(50, 100, 500, 400);
		frmAlta.getContentPane().add(panel1);
		panel1.setLayout(null);
		
		JPanel panel = new JPanel();
		panel.setBackground(new Color(60, 63, 65));
		panel.setBounds(50, 75, 400, 250);
		panel1.add(panel);
		panel.setLayout(null);
		
		JLabel lblDninie = new JLabel("DNI/NIE:");
		lblDninie.setForeground(new Color(204, 204, 204));
		lblDninie.setFont(new Font("Dialog", Font.PLAIN, 15));
		lblDninie.setBounds(37, 61, 61, 17);
		panel.add(lblDninie);
		
		txtID = new JTextField();
		txtID.setBounds(109, 60, 279, 21);
		panel.add(txtID);
		txtID.setColumns(10);
		
		JLabel lblNombre = new JLabel("Nombre:");
		lblNombre.setForeground(UIManager.getColor("Button.disabledToolBarBorderBackground"));
		lblNombre.setFont(new Font("Dialog", Font.PLAIN, 15));
		lblNombre.setBounds(37, 94, 61, 17);
		panel.add(lblNombre);
		
		JLabel lblApellido = new JLabel("Apellidos:");
		lblApellido.setForeground(UIManager.getColor("Button.disabledToolBarBorderBackground"));
		lblApellido.setFont(new Font("Dialog", Font.PLAIN, 15));
		lblApellido.setBounds(26, 129, 70, 17);
		panel.add(lblApellido);
		
		JLabel lblSaldo = new JLabel("Saldo:   €");
		lblSaldo.setForeground(UIManager.getColor("Button.disabledToolBarBorderBackground"));
		lblSaldo.setFont(new Font("Dialog", Font.PLAIN, 15));
		lblSaldo.setBounds(37, 162, 70, 17);
		panel.add(lblSaldo);
		
		txtNombre = new JTextField();
		txtNombre.setBounds(109, 93, 279, 21);
		panel.add(txtNombre);
		txtNombre.setColumns(10);
		
		txtApellido = new JTextField();
		txtApellido.setBounds(109, 128, 279, 21);
		panel.add(txtApellido);
		txtApellido.setColumns(10);
		
		txtSaldo = new JTextField();
		txtSaldo.setBounds(109, 161, 142, 21);
		panel.add(txtSaldo);
		txtSaldo.setColumns(10);
		
		JButton btnSalir = new JButton("Salir");
		btnSalir.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				confirmarSalida();
			}
		});
		btnSalir.setForeground(Color.LIGHT_GRAY);
		btnSalir.setBackground(new Color(60, 63, 65));
		btnSalir.setBounds(576, 449, 100, 27);
		frmAlta.getContentPane().add(btnSalir);
		
		JButton btnAlta = new JButton("<html>\n  <div style='text-align: center;'>\n    Ingresar<br>datos\n  </div>\n</html>\n");
		btnAlta.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				darDeAlta();
				new Datos(cuenta, PIN);
				frmAlta.dispose();				
			}
			
		});
		btnAlta.setBounds(576, 264, 100, 50);
		frmAlta.getContentPane().add(btnAlta);
		frmAlta.setBounds(100, 100, 700, 600);
		frmAlta.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		frmAlta.setResizable(false);
		frmAlta.setVisible(true);
		frmAlta.setLocationRelativeTo(null);
		
		frmAlta.setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
		frmAlta.addWindowListener(new java.awt.event.WindowAdapter() {
		    @Override
		    public void windowClosing(java.awt.event.WindowEvent e) {
		        confirmarSalida();
		    }
		});
	
	}

	/**
	 * Método para dar de alta una nueva cuenta.
	 */
    private void darDeAlta() {
		String dni = txtID.getText().trim();
		String nombre = txtNombre.getText().trim();
		String apellidos = txtApellido.getText().trim();
		String saldoStr = txtSaldo.getText().trim();
		
		if (dni.isEmpty() || nombre.isEmpty() || apellidos.isEmpty() || saldoStr.isEmpty()) {
			JOptionPane.showMessageDialog(frmAlta, 
					"Todos los campos son obligatorios.",
                    "Datos incompletos",
					JOptionPane.ERROR_MESSAGE);
			return;
		}
		
		if (!dni.matches("\\d{10}")) {
			JOptionPane.showMessageDialog(frmAlta, 
					"DNI/NIE debe tener 10 dígitos.",
					"DNI/NIE inválido",
					JOptionPane.ERROR_MESSAGE);
			return;
		}
		
		try {			
			double saldo = Double.parseDouble(saldoStr);
			if (saldo < 0 || saldo > MAX_SALDO) {
				JOptionPane.showMessageDialog(frmAlta, 
						"El saldo debe ser un número positivo y no superar " + MAX_SALDO + " €.",
						"Saldo inválido",
						JOptionPane.ERROR_MESSAGE);
				return;				
			}
			JOptionPane.showMessageDialog(frmAlta, 
					"Cuenta creada con éxito.\nDNI/NIE: " + dni + "\nNombre: " + nombre + "\nApellidos: " + apellidos + "\nSaldo: " + saldo + " €",
					"Cuenta creada",
					JOptionPane.INFORMATION_MESSAGE);
			cajero.altaCliente(dni, nombre, apellidos, saldoStr);
			
			if (cajero.isSesionActiva()) {
				JOptionPane.showMessageDialog(frmAlta, 
						"Cuenta creada y sesión activa.",
						"Éxito",
						JOptionPane.INFORMATION_MESSAGE);
				cuenta = cajero.getCuentaSesion();
				PIN = cajero.getPinSesion();
			} else {
				JOptionPane.showMessageDialog(frmAlta, 
						"Cuenta creada pero no se pudo activar la sesión.",
						"Advertencia",
						JOptionPane.WARNING_MESSAGE);
			}
		} catch (NumberFormatException ex) {
			JOptionPane.showMessageDialog(frmAlta, 
					"Saldo debe ser un número válido.",
					"Saldo inválido",
					JOptionPane.ERROR_MESSAGE);
			return;
		}
		
	}

    	/**
	 * Método para confirmar la salida de la ventana de alta.
	 * Muestra un diálogo de confirmación antes de cerrar la ventana.
	 */
	private void confirmarSalida() {
		int respuesta = JOptionPane.showConfirmDialog(frmAlta, "¿"
				+ "Seguro que desea cancelar la operación y " 
				+ "volver al menú principal?", 
				"Confirmar salida",
				JOptionPane.YES_NO_OPTION);
		if (respuesta == JOptionPane.YES_OPTION) {
			frmAlta.dispose();
			new Main(); // Retorna a la ventana principal		
		}
	}
    
    // public static void main(String[] args) {
	// 	EventQueue.invokeLater(() -> {
	// 		try {
	// 			new Alta();				
	// 		} catch (Exception e) {
	// 			e.printStackTrace();
	// 		}
	// 	});
	// }
}