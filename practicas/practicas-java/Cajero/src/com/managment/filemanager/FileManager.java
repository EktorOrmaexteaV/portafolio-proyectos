package com.managment.filemanager;

import com.itextpdf.io.font.constants.StandardFonts;
import com.itextpdf.kernel.pdf.PdfWriter;
import com.itextpdf.kernel.pdf.PdfDocument;
import com.itextpdf.layout.Document;
import com.itextpdf.layout.element.Paragraph;
import com.itextpdf.kernel.font.*;


import com.managment.utils.InputUtils;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

/**
 * Archivo: FileManager.java
 * Clase que proporciona métodos para manejar archivos de texto y generar PDFs.
 * @author Ektor Ormaetxea V
 */
public class FileManager {
    /**
     * Crea un archivo de texto con el nombre especificado.
     * Si el archivo ya existe, lo sobrescribe.
     * @param nombreArchivo El nombre del archivo a crear.
     */    
    public static void createFile(String nombreArchivo) {
    	
        try {            
            try (FileWriter fileWriter = new FileWriter(nombreArchivo)) {
                //Se deja el bloque vacío para crear el archivo.
            }
            System.out.println("Archivo '" + nombreArchivo + "' creado exitosamente (o sobrescrito si existía).");
        } catch (IOException e) {
            System.err.println("Error al crear el archivo '" + nombreArchivo + "': " + e.getMessage());
        }
    }    

    /**
     * Escribe texto en el archivo especificado. El texto se añade al final del archivo.
     * @param nombreArchivo El nombre del archivo donde escribir.
     */
    public static void writeFile(String nombreArchivo) {
        try (FileWriter fileWriter = new FileWriter(nombreArchivo, true);
             BufferedWriter writer = new BufferedWriter(fileWriter)) {

            System.out.println("Escribe el texto que deseas guardar (termina con Enter):");
            String texto = InputUtils.getStringInput();
            writer.write(texto);
            writer.newLine();
            System.out.println("Texto escrito en el archivo.");

        } catch (IOException e) {
            System.err.println("No se pudo abrir el archivo para escribir o hubo un error de E/S: " + e.getMessage());
        }
    }

    public static void writeFile(String nombreArchivo, String contenido) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(nombreArchivo))) {
            writer.write(contenido);
        } catch (IOException e) {
            System.err.println("No se pudo abrir el archivo para escribir o hubo un error de E/S: " + e.getMessage());
        }
    }

    /**
     * Lee y muestra el contenido completo del archivo especificado.
     * @param nombreArchivo El nombre del archivo a leer.
     */
        
    public static void readFile(String nombreArchivo) {
        File file = new File(nombreArchivo);
        if (!file.exists()) {
            System.out.println("El archivo '" + nombreArchivo + "' no existe.");
            return;
        }

        try (FileReader fileReader = new FileReader(file);
             BufferedReader reader = new BufferedReader(fileReader)) {

            System.out.println("Contenido del archivo '" + nombreArchivo + "':");
            String linea;
            while ((linea = reader.readLine()) != null) {
                System.out.println(linea);
            }
        } catch (IOException e) {
            System.err.println("No se pudo abrir el archivo para leer o hubo un error de E/S: " + e.getMessage());
        }
    }

    /**
	 * Borra el archivo especificado.
	 * @param nombreArchivo El nombre del archivo a borrar.
	 */
    public static void eraseFile(String nombreArchivo){
        File file = new File(nombreArchivo);
        if (!file.exists()) {
            System.out.println("El archivo '" + nombreArchivo + "' no existe.");
            return;
        }
        file.delete();
        System.out.println("El archivo '" + nombreArchivo + "' ha sido borrado.");
    }
    
    /**
	 * Lee el contenido de un archivo y lo devuelve como una cadena.
	 * Si el archivo no existe, devuelve un mensaje de error.
	 * @param nombreArchivo El nombre del archivo a leer.
	 * @return El contenido del archivo como una cadena, o un mensaje de error si el archivo no existe.
	 */
    public static String FileContent(String nombreArchivo){
        File file = new File(nombreArchivo);        
        if (!file.exists()) {
            return "El archivo '" + nombreArchivo + "' no existe.";
        }

        StringBuilder contenido = new StringBuilder();

        try (BufferedReader reader = new BufferedReader(new FileReader(file))) {
            String linea;
            while ((linea = reader.readLine()) != null) {
                contenido.append(linea).append(System.lineSeparator());
            }
        } catch (IOException e) {
            return "Error al leer el archivo: " + e.getMessage();
        }

        return contenido.toString();
    }
    
    /**
	 * Guarda el contenido de un archivo de texto como un PDF.
	 * Si el archivo no existe, muestra un mensaje de error.
	 * @param nombreArchivo El nombre del archivo de texto a convertir a PDF.
	 */
    public static void saveAsPDF(String nombreArchivo) {
        String contenido=FileContent(nombreArchivo);
        nombreArchivo+=".pdf";
        File file = new File(nombreArchivo);
        if (!file.exists()) {
            System.out.println("El archivo '" + nombreArchivo + "' no existe.");
            return;
        }
        
        try {
            PdfWriter writer = new PdfWriter(nombreArchivo);
            PdfDocument pdf = new PdfDocument(writer);
            try (Document document = new Document(pdf)) {
                PdfFont font = PdfFontFactory.createFont(StandardFonts.COURIER);
                document.setFont(font);
                document.setFontSize(11);
                
                document.add(new Paragraph(contenido));
            }
            System.out.println("PDF generado correctamente: " + nombreArchivo);
        } catch (IOException e) {
            System.err.println("Error al generar el PDF:");
        }
    }
    
    /**
	 * Guarda el contenido proporcionado como un PDF con el nombre de archivo especificado.
	 * @param nombreArchivo El nombre del archivo PDF a crear.
	 * @param contenido El contenido que se escribirá en el PDF.
	 */
    public static void saveAsPDF(String nombreArchivo, String contenido) {
        try {
            PdfWriter writer = new PdfWriter(nombreArchivo);
            PdfDocument pdf = new PdfDocument(writer);
            try (Document document = new Document(pdf)) {
                PdfFont font = PdfFontFactory.createFont(StandardFonts.COURIER);
                document.setFont(font);
                document.setFontSize(11);
                
                document.add(new Paragraph(contenido));
            }
            System.out.println("PDF generado correctamente: " + nombreArchivo);
        } catch (IOException e) {
            System.err.println("Error al generar el PDF:");
        }
    }    
}
