# MentorBitBMP280

Librería para la lectura de datos de temperatura, presión y altitud utilizando el sensor BMP280 en módulos compatibles con MentorBit.

## Descripción

La librería `MentorBitBMP280` facilita la lectura de datos de temperatura, presión atmosférica y altitud desde el sensor BMP280 en módulos compatibles con MentorBit. Permite obtener mediciones precisas para aplicaciones meteorológicas, de navegación y monitoreo ambiental.

## Modo de Empleo

1.  **Instalación:**
    * Abre el IDE compatible con MentorBit.
    * Ve a "Herramientas" -> "Gestionar librerías..."
    * Busca "MentorBitBMP280" e instálala.
    * **Nota:** Esta librería depende de la librería `Adafruit_BMP280`. Asegúrate de que también esté instalada.

2.  **Ejemplo básico:**

    ```c++
    #include <MentorBitBMP280.h>

    MentorBitBMP280 bmp;

    void setup() {
      bmp.begin();
      Serial.begin(9600);
      Serial.println("Sensor BMP280 inicializado.");
    }

    void loop() {
      float temperatura = bmp.leerTemperatura();
      float presion = bmp.leerPresion();
      float altitud = bmp.leerAltura();

      Serial.print("Temperatura: ");
      Serial.print(temperatura);
      Serial.println(" °C");

      Serial.print("Presión: ");
      Serial.print(presion);
      Serial.println(" Pa");

      Serial.print("Altitud: ");
      Serial.print(altitud);
      Serial.println(" m");

      delay(1000);
    }
    ```

## Constructor y Métodos Públicos

### Constructor

* `MentorBitBMP280()`: Crea un objeto `MentorBitBMP280`.

### Métodos

* `void begin(uint8_t i2c_addr = 0x76)`: Inicializa el sensor BMP280.
    * `i2c_addr`: Dirección I2C del sensor BMP280. El valor predeterminado es `0x76`.
* `float leerTemperatura()`: Lee la temperatura en grados Celsius.
* `float leerPresion()`: Lee la presión atmosférica en pascales (Pa).
* `float leerAltura()`: Lee la altitud en metros.
