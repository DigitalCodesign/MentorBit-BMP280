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
        

@author Digital Codesign
@version 1.1.0
@date Fecha de creación: Septiembre de 2024
@date Fecha de versión: Abril de 2026
@see Repositorio: https://github.com/DigitalCodesign/MentorBit-BMP280
@brief Descripción: 
    Esta librería está específicamente diseñada para ser utilizada junto con 
    el módulo MentorBit BMP280. Ha sido reescrita para eliminar dependencias 
    de terceros, implementando el algoritmo de compensación de enteros nativo 
    de Bosch. Esto garantiza la máxima precisión y eficiencia, estando 
    especialmente optimizada para microcontroladores AVR como el ATmega2560.
    @details Métodos principales:
        - MentorBitBMP280  -> Constructor de la clase.
        - begin            -> Inicializador, lee la calibración. Debe colocarse en el setup().
        - leerTemperatura  -> Lee el valor de temperatura ambiente en grados Celsius (°C).
        - leerPresion      -> Lee la presión atmosférica detectada por el módulo en Pascales (Pa).
        - leerAltura       -> Lee la altura estimada a la cual está situado el módulo.
    * Métodos extendidos y de control:
        - leerAltura(presionNivelMar) -> Permite introducir una presión base para altitud precisa.
        - ponerEnReposo / despertar   -> Controlan el modo de bajo consumo del sensor.
        - configurarFiltro            -> Ajusta el filtro IIR de hardware para suavizar lecturas.
        - estaConectado               -> Comprueba si el módulo responde en el bus I2C.
        - resetear                    -> Ejecuta un reinicio por software (Soft Reset) del chip.

*/

#ifndef MentorBitBMP280_h
#define MentorBitBMP280_h

#include <Arduino.h>
#include <Wire.h>

class MentorBitBMP280
{

    public: 

        MentorBitBMP280();
        bool begin(uint8_t i2c_addr = 0x76);
        bool estaConectado() const;
        float leerTemperatura() const;
        float leerPresion() const;
        float leerAltura() const;
        float leearAltura(float presion_nivel_mar) const;
        void ponerEnReposo();
        void despertar();
        void configurarFiltro(uint8_t coeficiente);
        void resetear();

    private: 

        uint8_t _i2c_addr;
        mutable int32_t _t_fine; // Variable matemática interna para algoritmo de Bosch

        struct { // Estructura para los 12 coeficientes de calibración de fábrica
            uint16_t dig_T1; int16_t dig_T2; int16_t dig_T3;
            uint16_t dig_P1; int16_t dig_P2; int16_t dig_P3; int16_t dig_P4;
            int16_t  dig_P5; int16_t dig_P6; int16_t dig_P7; int16_t dig_P8; int16_t dig_P9;
        } _calib;

        void _writeRegister(uint8_t reg, uint8_t value) const;
        uint8_t _readRegister(uint8_t reg) const;
        uint32_t _readRegister24(uint8_t reg) const; // Los registros del BMP son de 24 bits
        void _leerCoeficientes();

};

#endif