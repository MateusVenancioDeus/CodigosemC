**Projeto: Manipulação de Dados de Sensores Industriais** \
Professor: Zoe Magalhães \
Aluno: Mateus Vitor Venâncio de Deus \

Objetivo: \
Este projeto apresenta três programas desenvolvidos em linguagem C para manipulação eficiente de dados provenientes de sensores industriais. Os programas permitem organizar, consultar e gerar arquivos de dados de sensores com diferentes tipos. 

**Sensores suportados** \
CONJ_Z: dados do tipo inteiro 

CONJ_Q: dados do tipo float 

TEXTO: dados do tipo string 

BINARIO: dados do tipo booleano (true/false)

Programas 
1. Organizador 
Responsável por organizar os dados brutos de sensores em arquivos separados para cada tipo de sensor. Os dados são ordenados em ordem decrescente de timestamp.

Como executar:\

bash
Copiar
Editar
./organizador <arquivo_entrada>
<arquivo_entrada>: arquivo que contém os dados brutos não organizados.

**2. Consulta por Instante**
Realiza a busca binária em arquivos organizados para encontrar a leitura do sensor mais próxima a um timestamp fornecido pelo usuário.

**Como executar:**

bash
Copiar
Editar
./consulta <SENSOR> <TIMESTAMP> \
<SENSOR>: nome do sensor (CONJ_Z, CONJ_Q, TEXTO ou BINARIO)

<TIMESTAMP>: instante em segundos desde 01/01/1970 (Epoch) 

O programa retorna o valor registrado mais próximo do timestamp informado.

3. Gerador de Arquivos \
Gera arquivos de dados simulados para sensores, com registros aleatórios entre duas datas e horários definidos, para os sensores e tipos especificados.

**Como executar:**

bash
Copiar
Editar
./gerador <data_hora_inicial> <data_hora_final> <sensor1> <tipo1> [<sensor2> <tipo2> ...] <arquivo_saida> \
<data_hora_inicial> e <data_hora_final>: no formato dd/mm/aaaa_HH:MM:SS

<sensorX>: nome do sensor (CONJ_Z, CONJ_Q, TEXTO ou BINARIO)

<tipoX>: tipo correspondente do sensor

<arquivo_saida>: nome do arquivo onde os dados gerados serão salvos 

O programa gera 2000 registros para cada sensor informado dentro do intervalo de datas.

**Observações importantes** \
Os arquivos gerados ou utilizados pelos programas devem estar no mesmo diretório dos executáveis ou informar o caminho completo. \

O timestamp representa a quantidade de segundos desde o início da Era Unix (1 de janeiro de 1970). \

Validações básicas são feitas para garantir formatos corretos e evitar erros comuns de entrada.

Para compilar os programas, utilize o GCC: /

bash
Copiar
Editar
gcc organizador.c -o organizador
gcc consulta.c -o consulta
gcc gerador.c -o gerador
