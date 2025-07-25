# MentorBitBMP280
Esta librería te permite construir tu propia mini estación meteorológica con tu placa MentorBit y el módulo de sensor BMP-280.

El sensor BMP-280 es capaz de medir tres cosas importantes de la atmósfera que te rodea: la temperatura, la presión atmosférica y, a partir de ella, puede estimar la altitud a la que te encuentras.

Gracias al sistema MentorBit, solo tienes que conectar el módulo con un único cable y, con esta librería, podrás obtener datos precisos sin complicaciones. ¡Ideal para iniciarte en el monitoreo ambiental!

## Descripción

### ¿Qué es un sensor BMP-280?
Piensa en este sensor como un pequeño y potente meteorólogo digital. Es una herramienta muy popular en proyectos de electrónica porque mide con precisión variables clave del ambiente:

- **Temperatura:** Mide la temperatura ambiente en grados Celsius (°C).
- **Presión Atmosférica:** Mide el "peso" del aire que nos rodea. Se expresa en HectoPascals (hPa) y es fundamental para predecir cambios en el tiempo. Una bajada de presión suele indicar la llegada de mal tiempo.
- **Altitud:** Al saber la presión, el sensor puede estimar tu altitud sobre el nivel del mar. ¡Muy útil para proyectos con drones, cohetes o para saber a qué altura está tu casa!

### ¿Qué hace esta librería?
La librería MentorBitBMP280 actúa como un "traductor" inteligente entre el complejo sensor BMP-280 y tu placa MentorBit. Se encarga de toda la comunicación I2C y las fórmulas matemáticas para que tú solo tengas que pedirle los datos que necesitas con comandos muy sencillos.

### ¿Qué puedes construir con este sensor?
- Una estación meteorológica personal que muestre los datos en una pantalla.
- Un altímetro para un cohete de agua o un dron casero.
- Un sistema que te alerte de posibles cambios de tiempo si la presión baja bruscamente.
- Un dispositivo de monitoreo ambiental que registre los datos a lo largo del día.

Esta librería es el primer paso para integrar la monitorización de parámetros ambientales precisos en tus proyectos con MentorBit de una manera sencilla y profesional.

## Cómo Empezar

Usar la librería es muy sencillo. Solo sigue estos pasos para tener tu sensor midiendo luz en pocos minutos.

1. **Conexión del Módulo**
Antes que nada, conecta tu módulo de sensor BH1750 al puerto marcado como ***I2C*** dentro de la zona de ***Comunicaciones*** en tu placa MentorBit. ¡Y listo! No necesitas más cables.

2. **Instalación de las Librerías**
Para que tu placa MentorBit pueda comunicarse con el sensor, solo necesitas instalar una librería. El gestor del IDE de Arduino se encargará del resto.

- Abre tu entorno de programación IDE de Arduino.
- Ve al menú *Program -> Incluir Librería -> Administrar Bibliotecas...*
- En el buscador, escribe ***MentorBitBMP280*** y haz clic en "Instalar".
- El IDE detectará que esta librería necesita otras para funcionar. Haz clic en "Install all" o "Instalar todo" cuando te lo pregunte.

¡Y ya está! Con eso tienes todo lo necesario para empezar a programar.

## Ejemplo Básico: Tu Estación Meteorológica
El siguiente código lee los tres valores del sensor (temperatura, presión y altitud) y los muestra en tu ordenador cada dos segundos.

### Copia y pega este código en tu IDE:

```c++
// 1. Incluimos la librería que acabamos de instalar.
#include <MentorBitBMP280.h>

// 2. Creamos un objeto llamado 'miSensor' para poder controlar nuestro BMP-280.
MentorBit_BMP280 miSensor;

void setup() {
    // Inicializamos la comunicación con el ordenador.
    Serial.begin(9600);
    
    // 3. Inicializamos el sensor con su dirección I2C por defecto.
    miSensor.begin();
    
    Serial.println("Sensor BMP280 inicializado. Obteniendo datos...");
}

void loop() {
    // 4. Leemos cada uno de los valores usando las funciones de la librería.
    float temperatura = miSensor.leerTemperatura();
    float presion_Pa = miSensor.leerPresion(); // La lectura viene en Pascales (Pa)
    float altura = miSensor.leerAltura();

    // (Opcional) Convertimos los Pascales a HectoPascals (hPa), más común en meteorología.
    float presion_hPa = presion_Pa / 100.0;

    // 5. Mostramos los datos de forma ordenada en el monitor serie.
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");

    Serial.print("Presión: ");
    Serial.print(presion_hPa);
    Serial.println(" hPa");

    Serial.print("Altura estimada: ");
    Serial.print(altura);
    Serial.println(" m");

    Serial.println("--------------------");

    // Esperamos 2 segundos antes de la siguiente lectura.
    delay(2000);
}
```
### Para ver el resultado
1. Carga el código en tu placa MentorBit.
2. Abre el Monitor Serie del IDE de Arduino.
3. ¡Verás las lecturas del clima en tiempo real!

## Functiones Principales
Solo necesitas conocer estas sencillas funciones para controlar el sensor.

- <code>void begin(uint8_t i2c_addr = 0x76)</code>
   - **¿Qué hace?** Inicializa y configura el sensor para que esté listo para funcionar. Opcionalmente, puedes pasarle la dirección I2C de tu sensor si es diferente a la estándar (<code>0x76</code>).
   - **¿Cuándo se usa?** Debes llamarla siempre una vez dentro de la función <code>setup()</code> de tu programa.

- <code>float leerTemperatura()</code>
   - **¿Qué hace?** Le pide al sensor la temperatura ambiente y la devuelve como un número decimal (de tipo <code>float</code>) en grados **Celsius (°C)**.
   - **¿Cuándo se usa?** Cada vez que quieras saber la temperatura, normalmente dentro de la función <code>loop()</code> para obtener lecturas continuas.

- <code>float leerPresion()</code>
   - **¿Qué hace?** Le pide al sensor la presión atmosférica y la devuelve como un número decimal (de tipo <code>float</code>) en **Pascales (Pa)**.
   - **¿Cuándo se usa?** Cuando necesites una lectura de la presión, habitualmente dentro del <code>loop()<code>. Recuerda que puedes dividir el resultado entre 100 para obtener HectoPascals (hPa).

- <code>float leerAltura()</code>
   - **¿Qué hace?** Calcula la altitud estimada a partir de la presión y la devuelve como un número decimal (de tipo <code>float<code>) en **metros (m)**.
   - **¿Cuándo se usa?** Siempre que quieras obtener una estimación de la altitud, por lo general dentro de la función <code>loop()<code>.

## Recursos Adicionales
¿Quieres saber más sobre el módulo o necesitas comprar uno? Aquí tienes algunos enlaces que te serán de utilidad:

- [Web del fabricante](https://digitalcodesign.com/)
- [Tienda Online de Canarias](https://canarias.digitalcodesign.com/shop)
- [Tienda Online de Península](https://digitalcodesign.com/shop)
- [Web Oficial de MentorBit](https://digitalcodesign.com/mentorbit)
- [Web Oficial de MentorBit módulo de sensor BMP-280](https://canarias.digitalcodesign.com/shop/00038843-mentorbit-modulo-de-sensor-bmp-280-8116)
- [Manual de usuario del MentorBit módulo de sensor BMP-280](https://drive.google.com/file/d/1rPk9sRrEuS1h55T5Gu-Q_wcZwsAK9I8Y/view?usp=drive_link)
- [Modelo 3D de MentorBit módulo de sensor BMP-280 en formato .STEP](https://drive.google.com/file/d/1ahaK9Wh--3zj3Uh9wiCGhiTeSYJQ1jN1/view?usp=drive_link)
