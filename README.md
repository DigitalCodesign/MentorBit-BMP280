
# MentorBit BMP280

Esta librería está específicamente diseñada para ser utilizada junto con el módulo **MentorBit BMP280**.

El **MentorBit BMP280** es un sensor barométrico utilizado para medir la temperatura, la presión atmosférica y la altitud. Esta librería facilita la interacción con el sensor, permitiendo obtener lecturas precisas de estos parámetros.

## Modo de Empleo

Una vez que tengas la librería instalada desde el Arduino IDE, inclúyela en tu proyecto con la siguiente línea:

```cpp
#include <MentorBitBMP280.h>
```

### Constructor

Una vez incluida la librería, usamos el constructor para crear el objeto del sensor BMP280:

```cpp
MentorBitBMP280 bmp280;
```

### Métodos Principales

#### `begin(uint8_t i2c_addr = 0x76)`

Inicializa el sensor con la dirección I2C especificada. Debe llamarse en el `setup()` del programa.

```cpp
bmp280.begin();
```

##### Parámetros

- `i2c_addr`: Dirección I2C del sensor (por defecto es `0x76`).

#### `leerTemperatura()`

Lee y devuelve el valor de la temperatura detectada por el sensor BMP280.

```cpp
float temperatura = bmp280.leerTemperatura();
```

#### `leerPresion()`

Lee y devuelve el valor de la presión atmosférica detectada por el sensor BMP280.

```cpp
float presion = bmp280.leerPresion();
```

#### `leerAltura()`

Lee y devuelve el valor de la altitud calculada en base a la presión atmosférica detectada por el sensor BMP280.

```cpp
float altura = bmp280.leerAltura();
```
