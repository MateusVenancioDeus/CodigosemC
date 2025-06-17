Documentação do Projeto: Manipulação de Dados de Sensores Industriais
Visão Geral
Este projeto contém três programas em C para organizar, consultar e gerar dados de sensores industriais. Os sensores são:

CONJ_Z (inteiro)

CONJ_Q (float)

TEXTO (string)

BINARIO (booleano)

Programa 1 – Organizador
Organiza dados brutos de sensores em arquivos separados por sensor, ordenados por timestamp em ordem decrescente.

Execução:

bash
Copiar
Editar
./organizador <arquivo_entrada>
Programa 2 – Consulta por Instante
Consulta leitura de sensor por timestamp usando busca binária, retornando o valor mais próximo.

Execução:

bash
Copiar
Editar
./consulta <SENSOR> <TIMESTAMP>
Programa 3 – Gerador de Arquivos
Gera arquivos de dados com registros aleatórios entre dois instantes para sensores especificados.

Execução:

bash
Copiar
Editar
./gerador <data_hora_inicial> <data_hora_final> <sensor1> <tipo1> [<sensor2> <tipo2> ...] <arquivo_saida>
Formato para datas e horas: dd/mm/aaaa_HH:MM:SS

Notas
Arquivos devem estar no mesmo diretório dos programas ou indicar caminho completo.

Timestamp é o número de segundos desde 01/01/1970 (Epoch).

Compilação padrão com GCC:

bash
Copiar
Editar
gcc organizador.c -o organizador
gcc consulta.c -o consulta
gcc gerador.c -o gerador
