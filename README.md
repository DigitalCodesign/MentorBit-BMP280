# MentorBit-BMP280

## Descripción

Esta librería está específicamente diseñada para ser utilizada junto con el módulo **MentorBit BMP280**.

El **MentorBit BMP280** es un sensor barométrico utilizado para medir la temperatura, la presión atmosférica y la altitud. Esta librería facilita la interacción con el sensor, permitiendo obtener lecturas precisas de estos parámetros.

Puedes encontrar nuestro Módulo MentorBit y mucho más material de electrónica y robótica en nuestra tienda oficial:  [https://digitalcodesign.com/shop](https://digitalcodesign.com/shop)

## Métodos Principales

- `MentorBitBMP280` → Constructor de la clase.
- `begin` → Inicializador, debe colocarse en el `setup()`.
- `leerTemperatura` → Lee el valor de temperatura del sensor.
- `leerPresion` → Lee la presión atmosférica detectada por el módulo.
- `leerAltura` → Lee la altura a la cual está situado el módulo.

## Constructor

```cpp
MentorBitBMP280 miModuloBMP;
```

### Métodos

### `void begin(uint8_t i2c_addr = 0x76)`

Inicializa el sensor con la dirección I2C especificada. Debe llamarse en el `setup()` del programa.

#### Parámetros

- `i2c_addr`: Dirección I2C del sensor (por defecto es `0x76`).

### `float leerTemperatura()`

Lee y devuelve el valor de la temperatura detectada por el sensor BMP280.

### `float leerPresion()`

Lee y devuelve el valor de la presión atmosférica detectada por el sensor BMP280.

### `float leerAltura()`

Lee y devuelve el valor de la altitud calculada en base a la presión atmosférica detectada por el sensor BMP280.
