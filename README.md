Códigos em C - Estrutura de Dados

Feito por: Mateus Vitor Venâncio de Deus

Professor: Zoé Roberto Magalhães Júnior

Estruturas de Dados em C - Organização, Gerador e consulta de sensores.
N1 AT2

Projeto Sistema de Monitoramento e Consulta de Sensores Industriais

O porque do projeto:
Organizar grandes volumes de dados de sensores industriais
Consultar rapidamente qualquer leitura por instante de tempo
Gerar arquivos de teste com milhares de medições simuladas

Visão Geral dos Programas

Programa - Objetivo
Organizador.c - Organiza os dados brutos por sensor e ordena por timestamp
Consulta.c - Permite buscar a leitura mais próxima de um timestamp específico
Gerador.c - Cria arquivos de teste com sensores e dados aleatórios

Como Funciona Cada Programa

Programa 1 - Organizador.c

O que ele faz:
Lê um arquivo contendo dados misturados de vários sensores e separa em arquivos individuais por sensor, ordenando por timestamp.

Formato de entrada:
<TIMESTAMP> <ID_SENSOR> <VALOR>

Exemplo de linha no arquivo:
1718429342 sensor1 100
1718429350 sensor2 true

Como compilar:
gcc Organizador.c -o Organizador

Como executar:
./Organizador dados_sensores.txt

Saída:
Arquivos como sensor1.txt, sensor2.txt, cada um contendo as leituras ordenadas por data.

Programa 2 - Consulta.c

O que ele faz:
Permite consultar a leitura mais próxima de um determinado timestamp para um sensor específico.

Como executar:
./Consulta sensor1 1718429342

Resultado esperado:
Exibe a leitura mais próxima ao timestamp informado dentro do arquivo sensor1.txt.

Importante:
Utiliza busca binária para garantir rapidez mesmo com grandes volumes de dados.

Programa 3 - Gerador.c

O que ele faz:
Gera um arquivo de teste com 2000 registros por sensor, com timestamps aleatórios dentro de um intervalo de tempo definido.

Tipos de dados suportados por sensor:
int - Número inteiro aleatório
bool - true ou false
float - Número decimal aleatório
string - Texto aleatório com até 16 caracteres

Como executar:
./Gerador <data_inicio> <data_fim> <sensor1> <tipo_dado1> [<sensor2> <tipo_dado2> ...] <arquivo_saida>

Exemplo de uso:
./Gerador 01/06/2025_00:00:00 10/06/2025_23:59:59 sensor1 int sensor2 bool sensor3 float teste.txt

Saída:
Gera um arquivo como teste.txt contendo linhas como:
1718429342 sensor1 512
1718429345 sensor2 true
1718429347 sensor3 15.75

Compilação dos programas:
gcc Organizador.c -o Organizador
gcc Consulta.c -o Consulta
gcc Gerador.c -o Gerador

Ordem de Execução:
Programa 3 - Gerador.c
Programa 1 - Organizador.c
Programa 2 - Consulta.c

Possíveis Problemas e Como Resolver:

Erro: Arquivo não encontrado
Causa: Caminho ou nome de arquivo incorreto
Solução: Verifique o nome e a localização do arquivo

Formato de data inválido
Causa: Formato de data/hora digitado errado
Solução: Use o formato dd/mm/aaaa_hh:mm:ss

Feito por: Mateus Vitor Venâncio de Deus
