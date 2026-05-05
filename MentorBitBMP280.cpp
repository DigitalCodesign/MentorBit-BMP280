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

#include <MentorBitBMP280.h>

constexpr uint8_t BMP280_REG_TEMP_XLSB  = 0xFC;
constexpr uint8_t BMP280_REG_TEMP_LSB   = 0xFB;
constexpr uint8_t BMP280_REG_TEMP_MSB   = 0xFA;
constexpr uint8_t BMP280_REG_PRESS_XLSB = 0xF9;
constexpr uint8_t BMP280_REG_PRESS_LSB  = 0xF8;
constexpr uint8_t BMP280_REG_PRESS_MSB  = 0xF7;
constexpr uint8_t BMP280_REG_CONFIG     = 0xF5;
constexpr uint8_t BMP280_REG_CTRL_MEAS  = 0xF4;
constexpr uint8_t BMP280_REG_STATUS     = 0xF3;
constexpr uint8_t BMP280_REG_RESET      = 0xE0;
constexpr uint8_t BMP280_REG_ID         = 0xD0;
constexpr uint8_t BMP280_REG_CALIB      = 0x88;

/**
 * @brief Constructor de la clase. Inicializa las variables internas y la dirección I2C por defecto.
 */
MentorBitBMP280::MentorBitBMP280() : _i2c_addr(0x76), _t_fine(0) {}

/**
 * @brief Inicializa la comunicación con el sensor BMP280.
 * Verifica la conexión, lee los coeficientes de calibración de fábrica y configura el sensor
 * con valores predeterminados (modo Normal, filtro IIR activado).
 * @param i2c_addr Dirección I2C del sensor (por defecto 0x76).
 * @return true si el sensor fue encontrado e inicializado correctamente, false en caso contrario.
 */
bool MentorBitBMP280::begin(uint8_t i2c_addr)
{

    _i2c_addr = i2c_addr;
    Wire.begin();
    // Verificamos si el sensor está conectado
    if (!estaConectado()) return false;
    // Leer la calibración única de fábrica
    _leerCoeficientes();
    // Configuración por defecto: Sobremuestreo Temp. x1, Sobremuestreo Pres. x4, Modo Normal
    _writeRegister(BMP280_REG_CTRL_MEAS, 0x27);
    // Configuración del filtro IIR (coeficiente 4) y Standby de 0.5ms
    _writeRegister(BMP280_REG_CONFIG, 0x08);
    return true;

}

/**
 * @brief Lee y calcula la temperatura actual utilizando el algoritmo de compensación de hardware.
 * @return Temperatura ambiente en grados Celsius (°C).
 */
bool MentorBitBMP280::estaConectado() const
{

    // Comprobamos que recibimos el ID del BMP280 que siempre es 0x58
    return _readRegister(BMP280_REG_ID) == 0x58;

}

/**
 * @brief Lee y calcula la presión atmosférica actual utilizando los coeficientes de fábrica.
 * Nota: Este método llama internamente a leerTemperatura() para ajustar la compensación térmica.
 * @return Presión atmosférica en Pascales (Pa).
 */
float MentorBitBMP280::leerTemperatura() const
{

    // Leemos los 24 bits crudos de temperatura
    int32_t adc_t = _readRegister24(BMP280_REG_TEMP_MSB);
    if (adc_t == 0x800000) return 0; // Sensor en sleep
    // Algoritmo de enteros de 32 bits de Bosch
    int32_t var1, var2;
    var1 = ((((adc_t >> 3) - ((int32_t)_calib.dig_T1 << 1))) * ((int32_t)_calib.dig_T2)) >> 11;
    var2 = (((((adc_t >> 4) - ((int32_t)_calib.dig_T1)) * ((adc_t >> 4) - ((int32_t)_calib.dig_T1))) >> 12) * ((int32_t)_calib.dig_T3)) >> 14;
    // Guardamos _t_fine porque la fórmula de la presión la necesita
    _t_fine = var1 + var2;
    // Hallamos la temperatura
    float temperature = (_t_fine * 5 + 128) >> 8;
    // Devuelve temperatura en ºC
    return temperature / 100.0f;

}

/**
 * @brief Estima la altitud actual asumiendo la presión estándar a nivel del mar (1013.25 hPa).
 * Útil para mediciones relativas o cuando no se necesita precisión milimétrica.
 * @return Altitud estimada en metros.
 */
float MentorBitBMP280::leerPresion() const
{

    // Primero leemos siempre la temperatura para obtener _t_fine actualizado
    leerTemperatura();
    // Leemos los 24 bits crudos de la presión
    int32_t adc_p = _readRegister24(BMP280_REG_PRESS_MSB);
    if (adc_p == 0x800000) return 0; // Sensor ten sleep
    // Algoritmo de enteros de 64 bits de Bosch
    int64_t var1, var2, p;
    var1 = ((int64_t)_t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)_calib.dig_P6;
    var2 = var2 + ((var1 * (int64_t)_calib.dig_P5) << 17);
    var2 = var2 + (((int64_t)_calib.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)_calib.dig_P3) >> 8) + ((var1 * (int64_t)_calib.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)_calib.dig_P1) >> 33;
    // Evitamos división por cero
    if (var1 == 0) return 0;
    // Hallamos la presión
    p = 1048576 - adc_p;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)_calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)_calib.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)_calib.dig_P7) << 4);
    // Devolvemos la presión en Pa
    return (float)p / 256.0f;

}

/**
 * @brief Calcula la altitud actual de forma precisa utilizando una presión base conocida.
 * @param presionNivelMar Presión actual a nivel del mar en Pascales (ej. 101325.0).
 * @return Altitud precisa en metros.
 */
float MentorBitBMP280::leerAltura() const
{

    // Presión estándar a nivel del mar (1013.25 hPa)
    return leerAltura(101325.0f);

}

/**
 * @brief Pone el sensor en modo reposo (Sleep Mode) para minimizar el consumo de energía.
 * En este modo, el sensor deja de tomar medidas pero mantiene su configuración.
 */
float MentorBitBMP280::leearAltura(float presion_nivel_mar) const
{

    float presion = leerPresion();
    // Fórmula barométrica internacional
    return 44330.0f * (1.0f - pow(presion / presion_nivel_mar, 0.1903f));

}

/**
 * @brief Reactiva el sensor poniéndolo en modo Normal.
 * El sensor vuelve a tomar mediciones continuas de temperatura y presión.
 */
void MentorBitBMP280::ponerEnReposo()
{

    // Leemos el estado actual
    uint8_t ctrl = _readRegister(BMP280_REG_CTRL_MEAS);
    // Limpiamos los últimos 2 bits para ponerlos en 00
    ctrl &= ~0x03;
    _writeRegister(BMP280_REG_CTRL_MEAS, ctrl);

}

/**
 * @brief Configura el filtro IIR (Infinite Impulse Response) interno del sensor.
 * Útil para suavizar las lecturas de presión frente a perturbaciones breves (como viento o puertas).
 * @param coeficiente Nivel del filtro: 0 (Apagado), 1 (x2), 2 (x4), 3 (x8), 4 (x16).
 */
void MentorBitBMP280::despertar()
{

    // Leemos el estado actual
    uint8_t ctrl = _readRegister(BMP280_REG_CTRL_MEAS);
    // Cambiamos a modo Normal (dos últimos bits a 11)
    ctrl |= 0x03;
    _writeRegister(BMP280_REG_CTRL_MEAS, ctrl);

}

/**
 * @brief Comprueba si el sensor responde en el bus I2C validando su registro de identificación (ID).
 * @return true si la comunicación es exitosa y el ID es correcto (0x58), false en caso contrario.
 */
void MentorBitBMP280::configurarFiltro(uint8_t coeficiente)
{

    // Valores válidos: 0 (Apagado), 1 (Coef. 2), 2 (Coef. 4), 3 (Coef.8), 4 (Coef. 16)
    if (coeficiente > 4) coeficiente = 4;
    // Leemos la configuración actual
    uint8_t config = _readRegister(BMP280_REG_CONFIG);
    // Limpiamos los bits 2, 3 y 4
    config &= ~0x1C;
    // Cambiamos el filtro al indicado
    config |= (coeficiente << 2);
    _writeRegister(BMP280_REG_CONFIG, config);

}

/**
 * @brief Ejecuta un reinicio por software (Soft Reset) en el sensor.
 * Restablece todos los registros a sus valores de fábrica como si se hubiera desconectado la alimentación.
 */
void MentorBitBMP280::resetear()
{

    // Enviamos comando de reseteo
    _writeRegister(BMP280_REG_RESET, 0xB6);
    // Damos tiempo al chip a resetearse
    delay(10);

}

/**
 * @brief Escribe un valor de 8 bits en un registro específico del sensor vía I2C.
 * @param reg Dirección de memoria del registro.
 * @param value Valor que se escribirá.
 */
void MentorBitBMP280::_writeRegister(uint8_t reg, uint8_t value) const
{

    Wire.beginTransmission(_i2c_addr);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();

}

/**
 * @brief Lee un valor de 8 bits de un registro específico del sensor vía I2C.
 * @param reg Dirección de memoria del registro a leer.
 * @return El byte contenido en el registro.
 */
uint8_t MentorBitBMP280::_readRegister(uint8_t reg) const
{

    Wire.beginTransmission(_i2c_addr);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2c_addr, (uint8_t)1);
    return Wire.available() ? Wire.read() : 0;

}

/**
 * @brief Lee 3 bytes (24 bits) contiguos desde un registro inicial vía I2C.
 * Utilizado para extraer los datos crudos de alta resolución de temperatura y presión.
 * @param reg Dirección de memoria del registro inicial (MSB).
 * @return Valor de 20 bits útil empaquetado en un entero de 32 bits.
 */
uint32_t MentorBitBMP280::_readRegister24(uint8_t reg) const
{

    Wire.beginTransmission(_i2c_addr);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2c_addr, (uint8_t)3);
    if (Wire.available() == 3) {
        uint32_t msb = Wire.read();
        uint32_t lsb = Wire.read();
        uint32_t xlsb = Wire.read();
        return (msb << 12) | (lsb << 4) | (xlsb >> 4);
    }
    return 0;

}

/**
 * @brief Extrae los 12 coeficientes de calibración únicos grabados en la memoria ROM del sensor.
 * Esta función es crítica y debe ejecutarse una sola vez durante el begin() para garantizar
 * la precisión de las fórmulas de compensación.
 */
void MentorBitBMP280::_leerCoeficientes()
{

    Wire.beginTransmission(_i2c_addr);
    Wire.write(BMP280_REG_CALIB);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2c_addr, (uint8_t)24);
    if (Wire.available() == 24) {
        _calib.dig_T1 = Wire.read() | (Wire.read() << 8);
        _calib.dig_T2 = (int16_t)(Wire.read() | (Wire.read() << 8));
        _calib.dig_T3 = (int16_t)(Wire.read() | (Wire.read() << 8));
        _calib.dig_P1 = Wire.read() | (Wire.read() << 8);
        _calib.dig_P2 = (int16_t)(Wire.read() | (Wire.read() << 8));
        _calib.dig_P3 = (int16_t)(Wire.read() | (Wire.read() << 8));
        _calib.dig_P4 = (int16_t)(Wire.read() | (Wire.read() << 8));
        _calib.dig_P5 = (int16_t)(Wire.read() | (Wire.read() << 8));
        _calib.dig_P6 = (int16_t)(Wire.read() | (Wire.read() << 8));
        _calib.dig_P7 = (int16_t)(Wire.read() | (Wire.read() << 8));
        _calib.dig_P8 = (int16_t)(Wire.read() | (Wire.read() << 8));
        _calib.dig_P9 = (int16_t)(Wire.read() | (Wire.read() << 8));
    }

}
