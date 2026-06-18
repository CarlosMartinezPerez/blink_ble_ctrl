# ESP32 BLE Blink Control

## Descrição

Este projeto demonstra o controle remoto do período de piscagem de um LED conectado a um ESP32 utilizando Bluetooth Low Energy (BLE) e a API Web Bluetooth disponível em navegadores modernos.

O ESP32 atua como um servidor BLE (GATT Server), expondo um serviço com uma única characteristic de escrita. Um cliente web desenvolvido em HTML e JavaScript conecta-se ao dispositivo e envia um valor numérico correspondente ao período de piscagem do LED em milissegundos.

O objetivo do projeto é apresentar uma solução simples e leve para comunicação BLE entre um ESP32 e uma aplicação web, sem a necessidade de aplicativos móveis, notificações, características adicionais ou protocolos complexos.

---

## Funcionalidades

* Servidor BLE executando no ESP32
* Advertising BLE com nome personalizado
* Serviço GATT com UUID próprio
* Characteristic de escrita para configuração do período
* Controle dinâmico da frequência de piscagem do LED
* Interface Web Bluetooth executada diretamente no navegador
* Sem necessidade de aplicativo Android ou iOS
* Comunicação direta entre navegador e ESP32

---

## Arquitetura do Sistema

```text
+---------------------+
| Navegador Web       |
| (Web Bluetooth)     |
+----------+----------+
           |
           | BLE Write
           |
+----------v----------+
| ESP32 BLE Server    |
| Serviço 0x00FF      |
| Char.   0xFF01      |
+----------+----------+
           |
           |
+----------v----------+
| Blink Task          |
| Controle do LED     |
+---------------------+
```

---

## Hardware Utilizado

* ESP32
* LED conectado a um GPIO configurado como saída
* Computador com navegador compatível com Web Bluetooth

---

## Requisitos de Software

### ESP32

* ESP-IDF
* FreeRTOS (incluído no ESP-IDF)

### Cliente Web

Navegadores com suporte à API Web Bluetooth:

* Google Chrome
* Microsoft Edge
* Brave

Observação:

A API Web Bluetooth normalmente exige acesso através de:

* HTTPS

ou

* localhost

Para testes locais, utilize um servidor HTTP simples.

---

## UUIDs BLE

### Serviço

```text
0x00FF
```

### Characteristic

```text
0xFF01
```

### Nome do dispositivo

```text
ESP32-BLINK
```

---

## Funcionamento

Após inicializado, o ESP32:

1. Inicializa a pilha BLE.
2. Configura o nome do dispositivo.
3. Cria um serviço BLE.
4. Cria uma characteristic de escrita.
5. Inicia o advertising.

Quando o usuário se conecta através da interface web:

1. O navegador localiza o dispositivo.
2. Estabelece conexão GATT.
3. Obtém o serviço BLE.
4. Obtém a characteristic.
5. Envia um valor numérico em formato texto.

Exemplo:

```text
100
```

O ESP32 recebe o valor, converte para inteiro e atualiza o período de piscagem do LED.

---

## Estrutura do Projeto

```text
.
├── main
│   ├── main.c
│   ├── ble_server.c
│   ├── ble_server.h
│   ├── blink.c
│   └── blink.h
│
├── CMakeLists.txt
├── sdkconfig
└── web
    └── index.html
```

---

## Compilação

Configurar o ambiente do ESP-IDF:

```bash
. $HOME/esp/esp-idf/export.sh
```

Compilar:

```bash
idf.py build
```

Gravar na placa:

```bash
idf.py flash
```

Monitor serial:

```bash
idf.py monitor
```

---

## Executando a Interface Web

Entre na pasta onde está o arquivo:

```text
index.html
```

Inicie um servidor HTTP simples:

```bash
python3 -m http.server 8000
```

Abra no navegador:

```text
http://localhost:8000
```

---

## Utilização

1. Ligue o ESP32.
2. Abra a interface web.
3. Clique em "Conectar".
4. Selecione o dispositivo "ESP32-BLINK".
5. Informe o período desejado em milissegundos.
6. Clique em "Enviar".

Exemplos:

```text
100
```

LED pisca rapidamente.

```text
500
```

LED pisca duas vezes por segundo.

```text
1000
```

LED alterna a cada segundo.

---

## Exemplo de Log no ESP32

```text
BLE inicializado com sucesso
Advertising iniciado

CLIENTE CONECTOU conn_id=0

WRITE conn_id=0 len=3

Recebido: 100

Novo periodo: 100 ms
```

---

## Limitações

Este projeto foi desenvolvido com foco em simplicidade e aprendizado.

Não foram implementados:

* Notificações BLE
* Indicações BLE
* Autenticação
* Criptografia
* Persistência de configuração
* Atualização OTA

---

## Possíveis Extensões

* Controle de múltiplos LEDs
* Controle de brilho via PWM
* Leitura de sensores
* Interface gráfica mais elaborada
* Armazenamento permanente das configurações
* Controle remoto de dispositivos IoT
* Integração com Progressive Web Apps (PWA)

---

## Autor

Carlos Martinez Perez

Projeto desenvolvido para estudo de Bluetooth Low Energy (BLE), ESP32 e Web Bluetooth.
