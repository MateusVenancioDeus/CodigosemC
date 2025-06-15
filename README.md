# Codigos em C - Estrutura de Dados
Feito por: Mateus Vitor Venâncio de Deus
Professor: Zoé Roberto Magalhães Júnior

Estruturas de Dados em C - Organização, criação e consulta de sensores.
 N1 AT2

Projeto Sistema de Monitoramento e Consulta de Sensores Industriais

O porque do projeto:
Organizar grandes volumes de dados de sensores industriais
Consultar rapidamente qualquer leitura por instante de tempo
Gerar arquivos de teste com milhares de medições simuladas

Visão Geral dos Programas
Este projeto é composto por:

Programa	Objetivo
organiza_dados.c	Organiza os dados brutos por sensor e ordena por timestamp
consulta_instante.c	Permite buscar a leitura mais próxima de um timestamp específico
gera_arquivo_teste.c	Cria arquivos de teste com sensores e dados aleatórios

Como Funciona Cada Programa?
 1. Organização dos Dados – organiza_dados.c
O que ele faz:
Pega um arquivo gigante com dados de vários sensores misturados e separa tudo em arquivos individuais por sensor, já em ordem cronológica.

Formato de entrada:<TIMESTAMP> <ID_SENSOR> <VALOR>

Como compilar: gcc organiza_dados.c -o organiza_dados

Como executar:./organiza_dados dados_sensores.txt

Saída:sensor1.txt
sensor2.txt

Vários arquivos como:

Cada um contendo as leituras ordenadas por data.

2. Consulta por Instante – consulta_instante.c
3. O que ele faz:
Te permite descobrir qual foi a leitura mais próxima de uma determinada data/hora para um sensor específico.

Formato de execução:
./consulta_instante <ID_SENSOR> <TIMESTAMP>

Exemplo de uso:
./consulta_instante sensor1 1718429342

Resultado esperado:
O programa vai mostrar a leitura que tem o timestamp mais próximo de 1718429342 dentro do arquivo sensor1.txt.

Importante:
Este programa usa busca binária para ser super rápido, mesmo com arquivos gigantes e complexos.

3. Geração de Arquivo de Teste – gera_arquivo_teste.c
4. O que ele faz:
Gera um arquivo com 2000 registros por sensor, com timestamps aleatórios dentro de um intervalo de tempo que você escolher.

Tipos de dados suportados por sensor:

- int → Número inteiro aleatório

- bool → true ou false

- float → Número decimal

- string → Texto aleatório com até 16 letras

Como compilar: gcc gera_arquivo_teste.c -o gera_arquivo_teste

Como executar: ./gera_arquivo_teste <data_inicio> <data_fim> <sensor1> <tipo_dado1> [<sensor2> <tipo_dado2> ...] <arquivo_saida>

Exemplo real: ./gera_arquivo_teste 16/06/2025_11:19:12 10/06/2025_23:59:59 sensor1 int sensor2 bool sensor3 float teste.txt

Saída:
Um arquivo como teste.txt, contendo linhas como:

1718429342 sensor1 512
1718429345 sensor2 true
1718429347 sensor3 15.75

gcc organiza_dados.c -o organiza_dados
gcc consulta_instante.c -o consulta_instante
gcc gera_arquivo_teste.c -o gera_arquivo_teste

Ordem dos programas:
Para facilitar o entendimento e execução do código:
-Programa 3 (Gerador.c)
-Programa 1 (Organizador.c)
-Programa 2 (Consulta.c)

- Possíveis Problemas (e Como Resolver)
Problema, causa	e Solução:
Erro: Arquivo não encontrado	Caminho ou nome de arquivo errado	Verifique se o nome e a localização do arquivo estão corretos
Formato de data inválido	Digitou a data de forma incorreta	Use exatamente o formato dd/mm/aaaa_hh:mm:ss
Memória insuficiente	Muitos registros e pouca RAM	Tente rodar em uma máquina com mais memória
Linha mal formatada	Alguma linha do arquivo de entrada está errada	Corrija ou remova a linha com problema

  -Fluxo de Trabalho Recomendado
  (Opcional) Gere um arquivo de teste com o gera_arquivo_teste1.
  Organize o arquivo gerado com o organiza_dados.
  Faça consultas específicas com o consulta_instante.
