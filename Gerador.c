#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

time_t converter_para_timestamp(int dia, int mes, int ano, int hora, int min, int seg) {
    if (dia <= 0 || dia > 31 || mes <= 0 || mes > 12 || ano <= 0) return -1;
    if (hora < 0 || hora > 23 || min < 0 || min > 59 || seg < 0 || seg > 59) return -1;

    struct tm t;
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;

    return mktime(&t);
}

time_t gerar_timestamp_aleatorio(time_t inicio, time_t fim) {
    return inicio + rand() % (fim - inicio + 1);
}

void gerar_valor(char *buffer, const char *tipo) {
    if (strcmp(tipo, "CONJ_Z") == 0) {
        sprintf(buffer, "%d", rand() % 1000);
    } else if (strcmp(tipo, "CONJ_Q") == 0) {
        sprintf(buffer, "%.2f", ((float)rand() / RAND_MAX) * 100.0);
    } else if (strcmp(tipo, "TEXTO") == 0) {
        int tamanho = 1 + rand() % 16;
        for (int i = 0; i < tamanho; i++) {
            buffer[i] = 'a' + (rand() % 26);
        }
        buffer[tamanho] = '\0';
    } else if (strcmp(tipo, "BINARIO") == 0) {
        strcpy(buffer, (rand() % 2 == 0) ? "true" : "false");
    }
}

int tipo_sensor_valido(const char *tipo) {
    return (strcmp(tipo, "CONJ_Z") == 0 ||
            strcmp(tipo, "CONJ_Q") == 0 ||
            strcmp(tipo, "TEXTO") == 0 ||
            strcmp(tipo, "BINARIO") == 0);
}

int main(int argc, char *argv[]) {
    if (argc < 6 || (argc - 4) % 2 != 0) {
        printf("Erro: Quantidade de argumentos inválida.\n");
        return 1;
    }

    int dia_ini, mes_ini, ano_ini, hora_ini, min_ini, seg_ini;
    int dia_fim, mes_fim, ano_fim, hora_fim, min_fim, seg_fim;

    if (sscanf(argv[1], "%d/%d/%d_%d:%d:%d", &dia_ini, &mes_ini, &ano_ini, &hora_ini, &min_ini, &seg_ini) != 6) {
        printf("Erro: Formato da data/hora inicial inválido.\n");
        return 1;
    }

    if (sscanf(argv[2], "%d/%d/%d_%d:%d:%d", &dia_fim, &mes_fim, &ano_fim, &hora_fim, &min_fim, &seg_fim) != 6) {
        printf("Erro: Formato da data/hora final inválido.\n");
        return 1;
    }

    time_t timestamp_ini = converter_para_timestamp(dia_ini, mes_ini, ano_ini, hora_ini, min_ini, seg_ini);
    time_t timestamp_fim = converter_para_timestamp(dia_fim, mes_fim, ano_fim, hora_fim, min_fim, seg_fim);

    if (timestamp_ini == -1 || timestamp_fim == -1 || timestamp_ini > timestamp_fim) {
        printf("Erro: Intervalo de datas/hora inválido.\n");
        return 1;
    }

    for (int i = 3; i < argc - 1; i += 2) {
        if (!tipo_sensor_valido(argv[i + 1])) {
            printf("Erro: Tipo de sensor inválido: %s. Use apenas CONJ_Z, CONJ_Q, TEXTO ou BINARIO.\n", argv[i + 1]);
            return 1;
        }
    }

    FILE *saida = fopen(argv[argc - 1], "w");
    if (saida == NULL) {
        printf("Erro: Não foi possível criar o arquivo de saída.\n");
        return 1;
    }

    srand(time(NULL));

    for (int i = 3; i < argc - 1; i += 2) {
        char *tipo = argv[i + 1];
        for (int j = 0; j < 2000; j++) {
            time_t ts = gerar_timestamp_aleatorio(timestamp_ini, timestamp_fim);
            char valor[256];
            gerar_valor(valor, tipo);
            fprintf(saida, "%ld %s %s\n", ts, tipo, valor);
        }
    }

    fclose(saida);
    printf("Arquivo '%s' gerado com sucesso.\n", argv[argc - 1]);
    return 0;
}
