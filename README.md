🌐 ESP32 IoT – Monitor Ambiental com DHT11 (Access Point)
Este projeto transforma o ESP32 em um dispositivo IoT autônomo, capaz de medir temperatura e umidade utilizando 
o sensor DHT11, exibir as informações em um display OLED e disponibilizar os dados em tempo real através de uma 
página web, sem necessidade de internet ou roteador.

O ESP32 opera em modo Access Point, criando sua própria rede Wi-Fi. Basta conectar o celular ou computador à rede 
do ESP32 e acessar o endereço IP informado no display para visualizar os dados diretamente no navegador.

🔧 Principais Funcionalidades

📡 Modo Access Point (AP)
O ESP32 cria sua própria rede Wi-Fi (ESP32-DHT11), permitindo acesso local sem infraestrutura externa.

🌡️ Leitura de Temperatura e Umidade
Coleta de dados ambientais usando o sensor DHT11 (compatível com DHT22).

🖥️ Display OLED 128x64
Exibe o IP da rede AP por 5 segundos no boot
Em seguida, mostra temperatura e umidade em tempo real

🌍 Interface Web (HTML)
Página acessível via navegador
Atualização automática a cada 5 segundos
Visual limpo e responsivo

📐 Formatação Profissional dos Dados
Valores exibidos com até 2 casas decimais
Tratamento de erro (nunca exibe NaN)

🎯 Objetivo do Projeto

Este projeto foi desenvolvido com foco didático, sendo ideal para:
Iniciantes em ESP32 e IoT
Estudantes e professores
Makers e entusiastas de eletrônica
Criação de conteúdos para YouTube, workshops e aulas práticas.
A arquitetura foi pensada para facilitar futuras expansões, como 
integração com dashboards, sensores mais precisos ou envio de dados para a nuvem.

🧩 Tecnologias Utilizadas

ESP32
Sensor DHT11
Display OLED SSD1306 (I2C)
WebServer nativo do ESP32
HTML + CSS
Arduino IDE
