/*


            ██████╗    ██╗    ██████╗    ██╗   ████████╗    █████╗    ██╗               
            ██╔══██╗   ██║   ██╔════╝    ██║   ╚══██╔══╝   ██╔══██╗   ██║               
            ██║  ██║   ██║   ██║  ███╗   ██║      ██║      ███████║   ██║               
            ██║  ██║   ██║   ██║   ██║   ██║      ██║      ██╔══██║   ██║               
            ██████╔╝   ██║   ╚██████╔╝   ██║      ██║      ██║  ██║   ███████╗          
            ╚═════╝    ╚═╝    ╚═════╝    ╚═╝      ╚═╝      ╚═╝  ╚═╝   ╚══════╝          
                                                                                        
     ██████╗    ██████╗    ██████╗    ███████╗   ███████╗   ██╗    ██████╗    ███╗   ██╗
    ██╔════╝   ██╔═══██╗   ██╔══██╗   ██╔════╝   ██╔════╝   ██║   ██╔════╝    ████╗  ██║
    ██║        ██║   ██║   ██║  ██║   █████╗     ███████╗   ██║   ██║  ███╗   ██╔██╗ ██║
    ██║        ██║   ██║   ██║  ██║   ██╔══╝     ╚════██║   ██║   ██║   ██║   ██║╚██╗██║
    ╚██████╗   ╚██████╔╝   ██████╔╝   ███████╗   ███████║   ██║   ╚██████╔╝   ██║ ╚████║
     ╚═════╝    ╚═════╝    ╚═════╝    ╚══════╝   ╚══════╝   ╚═╝    ╚═════╝    ╚═╝  ╚═══╝ 
        

    Autor: Digital Codesign
    Version: 1.0.0
    Fecha de creación: Septiembre de 2024
    Fecha de version: Septiembre de 2024
    Repositorio: https://github.com/DigitalCodesign/MentorBit-BMP280
    Descripcion: 
        Este sketch muestra un ejemplo de como utilizar el modulo BMP280 para obtener las
        medidas de temperatura, presion y altitud y mostrarlas por el monitor serial

*/

// Se incluye la libreria MentorBitBMP280
#include <MentorBitBMP280.h>

// Se crea el objeto bmp_280
MentorBitBMP280 bmp_280;

void setup(){
    // Se inicializa el sensor BMP280
    bmp_280.begin();
    // Se inicializa el bus serial para mostrar la información
    Serial.begin(9600);
}

void loop(){
    // Mostrar la temperatura
    Serial.print("Temperatura: ");
    Serial.print(bmp_280.leerTemperatura());
    Serial.println(" ºC");
    // Mostrar la altitud
    Serial.print("Altitud: ");
    Serial.print(bmp_280.leerAltura());
    Serial.println(" m");
    // Mostrar la presion atmosferica
    Serial.print("Presion: ");
    Serial.print(bmp_280.leerPresion());
    Serial.println(" Pa");
    // Separar los mensajes del bus serial y pequeño delay
    Serial.println(" ");
    delay(2000);
}