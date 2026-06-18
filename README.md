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
├── index.html
├── CMakeLists.txt
├── sdkconfig
└── README.md
```

---

## Executando a Interface Web

Na raiz do projeto execute:

```bash
python3 -m http.server 8000
```

Abra no navegador:

```text
http://localhost:8000
```

O arquivo `index.html` será carregado automaticamente pelo navegador.

---

## Arquivos Principais

### index.html

Interface Web Bluetooth responsável por:

* Localizar o dispositivo BLE
* Estabelecer conexão GATT
* Obter o serviço BLE
* Obter a characteristic de controle
* Enviar o período de piscagem para o ESP32

### ble_server.c

Implementa o servidor BLE do ESP32:

* Inicialização da pilha BLE
* Configuração do advertising
* Criação do serviço GATT
* Criação da characteristic de escrita
* Recepção dos comandos enviados pelo navegador

### blink.c

Implementa a lógica de controle do LED, permitindo a alteração dinâmica do período de piscagem.

### main.c

Ponto de entrada da aplicação, responsável pela inicialização dos módulos do sistema.

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

## Configuração do Navegador para Web Bluetooth

O projeto utiliza a API Web Bluetooth, que atualmente é suportada por navegadores baseados em Chromium, como:

* Google Chrome
* Chromium
* Microsoft Edge
* Brave

### Requisitos

* Bluetooth habilitado no computador
* ESP32 anunciando via BLE
* Navegador compatível com Web Bluetooth

---

### Linux

Verifique se o serviço Bluetooth está ativo:

```bash
systemctl status bluetooth
```

Caso necessário:

```bash
sudo systemctl start bluetooth
sudo systemctl enable bluetooth
```

Verifique se o adaptador Bluetooth está funcionando:

```bash
bluetoothctl list
```

---

### Executar através de um servidor HTTP

Por questões de segurança, a API Web Bluetooth não funciona abrindo o arquivo HTML diretamente pelo sistema de arquivos.

Não utilize:

```text
file:///home/usuario/projeto/index.html
```

Utilize um servidor HTTP local:

```bash
python3 -m http.server 8000
```

Depois abra:

```text
http://localhost:8000
```

---

### Permissões de Bluetooth

Na primeira conexão, o navegador exibirá uma janela solicitando acesso ao dispositivo BLE.

Selecione:

```text
ESP32-BLINK
```

e confirme a conexão.

---

### Verificação de Compatibilidade

Abra as ferramentas de desenvolvedor do navegador (F12) e execute:

```javascript
navigator.bluetooth
```

Se o navegador retornar um objeto Bluetooth, a API está disponível.

Exemplo:

```javascript
Bluetooth { ... }
```

---

### Problemas Comuns

#### O dispositivo não aparece na lista

Verifique:

* ESP32 energizado
* Advertising BLE ativo
* Nome configurado como:

```text
ESP32-BLINK
```

* Bluetooth do computador habilitado

---

#### Erro "Bluetooth not available"

Verifique:

* Adaptador Bluetooth reconhecido pelo sistema
* Serviço Bluetooth ativo
* Navegador compatível com Web Bluetooth

---

#### Erro "User cancelled the requestDevice chooser"

O usuário fechou a janela de seleção de dispositivos sem escolher um dispositivo BLE.

Basta tentar novamente.

---

#### Erro ao abrir o HTML diretamente

A API Web Bluetooth exige contexto seguro.

Utilize:

```text
http://localhost
```

ou

```text
https://
```

em vez de abrir o arquivo diretamente pelo navegador.

---

## Objetivo Educacional

Este projeto foi desenvolvido para demonstrar uma comunicação BLE direta entre um navegador web e um ESP32, sem a necessidade de aplicativos móveis ou bibliotecas JavaScript externas. O foco é mostrar como utilizar a API Web Bluetooth para controlar um dispositivo embarcado de forma simples e transparente.

---

## Autor

Carlos Martinez Perez

Projeto desenvolvido para estudo de Bluetooth Low Energy (BLE), ESP32 e Web Bluetooth.
