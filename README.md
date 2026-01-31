# Monitoramento de Bancada de Rolamentos (Temp & Carga)

![Badge Arduino](https://img.shields.io/badge/Hardware-Arduino-teal)
![Badge Sensors](https://img.shields.io/badge/Sensors-DS18B20%20%7C%20HX711-blue)
![Badge Display](https://img.shields.io/badge/Display-LCD%2016x2-yellow)
![License](https://img.shields.io/badge/License-MIT-lightgrey)

Este firmware foi desenvolvido para a instrumentação de uma bancada, destinada a estudos tribológicos em rolamentos. O sistema monitora em tempo real a temperatura de operação e a carga (peso/força) aplicada, exibindo os dados em um display LCD e enviando via Serial para aquisição de dados.

## Funcionalidades

* **Leitura de Temperatura:** Monitoramento preciso via sensor digital DS18B20.
* **Medição de Força/Peso:** Utiliza célula de carga com amplificador HX711 para medir carga axial/radial ou atrito.
* **Interface Local:** Exibição imediata dos dados em Display LCD 16x2.
* **Função Tara (Zero):** Zeragem remota da balança via comando Serial.
* **Multitarefa:** Leitura não-bloqueante da célula de carga para alta responsividade.

---

## Hardware Necessário

* **Microcontrolador:** Arduino Uno ou Nano.
* **Sensor de Temperatura:** DS18B20 (c/ resistor de 4.7kΩ pull-up).
* **Módulo de Carga:** Célula de Carga + Módulo Amplificador HX711.
* **Display:** LCD 16x2 (conexão paralela 4-bits).
* **Outros:** Potenciômetro 10kΩ (ajuste de contraste), Resistor 4.7kΩ, Jumpers.

### Esquema de Ligação (Pinout)

A pinagem está configurada conforme o código fonte:

| Componente | Pino do Componente | Pino Arduino | Observação |
| :--- | :--- | :--- | :--- |
| **DS18B20** | Dados (Data) | **D5** | *Requer resistor 4.7kΩ entre Data e VCC* |
| **HX711** | SCK (Clock) | **D6** | |
| **HX711** | DT (Data) | **D7** | |
| **LCD 16x2** | RS | **D8** | |
| **LCD 16x2** | Enable (E) | **D9** | |
| **LCD 16x2** | D4 | **D10** | |
| **LCD 16x2** | D5 | **D11** | |
| **LCD 16x2** | D6 | **D12** | |
| **LCD 16x2** | D7 | **D13** | |

---

## Instalação e Dependências

Para compilar este código, instale as seguintes bibliotecas através do **Gerenciador de Bibliotecas** da Arduino IDE (`Ctrl+Shift+I`):

1.  **DallasTemperature** (Autor: Miles Burton).
2.  **OneWire** (Autor: Paul Stoffregen).
3.  **HX711_ADC** (Autor: *Olav Kallhovd*).
    * *Nota: Certifique-se de usar a versão **_ADC**, pois a sintaxe difere da biblioteca HX711 genérica.*
4.  **LiquidCrystal** (Nativa do Arduino).

---

## Procedimento de Calibração

A célula de carga precisa ser calibrada antes do uso para garantir precisão. O valor `calibrationValue` no código deve ser ajustado para o seu hardware.

1.  Carregue o código no Arduino.
2.  Abra o **Monitor Serial** (Configure o Baud Rate para **57600**).
3.  Aguarde a inicialização (garanta que não há peso sobre a célula).
4.  Coloque um peso de massa conhecida (ex: 100g, 1kg) sobre a célula.
5.  Verifique o valor exibido no Serial.
    * Edite a linha no código: `float calibrationValue = 1720;`
    * Se o valor lido for **maior** que o real, **aumente** o valor de calibração.
    * Se o valor lido for **menor** que o real, **diminua** o valor.
6.  Reenvie o código até que o peso lido seja exato.

---

## Operação

### Monitoramento
Ao iniciar, o LCD exibe a mensagem de boas-vindas e começa a mostrar:
* **Linha 0:** Temperatura em °C.
* **Linha 1:** Peso em gramas (g).

### Comandos Serial
Você pode interagir com a bancada enviando caracteres pelo Monitor Serial:

* **`t`** : Realiza a **Tara (Zerar)** da célula de carga instantaneamente. Use isso para descontar o peso de ferramentas ou suportes antes do teste.

### Exemplo de Saída Serial
```text
Sistema Integrado - Temp, Peso e LCD
Iniciando HX711...
Inicio de Operacao Completo.
Temperatura: 24.50 C | Peso: 0.00 g
Temperatura: 24.60 C | Peso: 50.12 g
