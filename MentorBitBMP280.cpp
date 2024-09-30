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
        Esta libreria esta especificamente diseñada para ser utilizada junto con 
        el modulo MentorBit BMP280
    Metodos principales:
        MentorBitBMP280 -> constructor de la clase
        begin -> inicializador, debe colocarse en el setup
        leerTemperatura -> lee el valor de temperatura del sensor
        leerPresion -> lee la presion atmosferica ddetectada por el modulo
        leerAltura -> lee la altura a la cual esta situado el modulo

*/

#include <MentorBitBMP280.h>

MentorBitBMP280::MentorBitBMP280(){

}

void MentorBitBMP280::begin(uint8_t i2c_addr = 0x76){
    _i2c_addr = i2c_addr;
     bmp.begin(_i2c_addr);
}

float MentorBitBMP280::leerTemperatura(){
    float value;
    value = bmp.readTemperature();
    return value;
}

float MentorBitBMP280::leerPresion(){
    float value;
    value = bmp.readPressure();
    return value;
}

float MentorBitBMP280::leerAltura(){
    float value;
    value = bmp.readAltitude();
    return value;
}