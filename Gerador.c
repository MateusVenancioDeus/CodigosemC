#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

time_t converter_para_timestamp(int dia, int mes, int ano, int hora, int min, int seg) {
    // Validações conforme pedido
    if (dia == 0 || mes == 0 || ano == 0) return -1;
    if (hora == 0 || hora > 24) return -1;
    if (min < 0 || min > 59 || seg < 0 || seg > 59) return -1;

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
        int len = 1 + rand() % 16;
        for (int i = 0; i < len; i++) buffer[i] = 'a' + (rand() % 26);
        buffer[len] = '\0';
    } else if (strcmp(tipo, "BINARIO") == 0) {
        sprintf(buffer, "%s", (rand() % 2 == 0) ? "true" : "false");
    } else {
        strcpy(buffer, "TIPO_INVALIDO");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 6 || (argc - 4) % 2 != 0) return 1;

    int dia_ini, mes_ini, ano_ini, hora_ini, min_ini, seg_ini;
    int dia_fim, mes_fim, ano_fim, hora_fim, min_fim, seg_fim;

    if (sscanf(argv[1], "%d/%d/%d_%d:%d:%d", &dia_ini, &mes_ini, &ano_ini, &hora_ini, &min_ini, &seg_ini) != 6) return 1;
    if (sscanf(argv[2], "%d/%d/%d_%d:%d:%d", &dia_fim, &mes_fim, &ano_fim, &hora_fim, &min_fim, &seg_fim) != 6) return 1;

    time_t timestamp_ini = converter_para_timestamp(dia_ini, mes_ini, ano_ini, hora_ini, min_ini, seg_ini);
    time_t timestamp_fim = converter_para_timestamp(dia_fim, mes_fim, ano_fim, hora_fim, min_fim, seg_fim);

    if (timestamp_ini == -1 || timestamp_fim == -1 || timestamp_ini > timestamp_fim) return 1;

    FILE *saida = fopen(argv[argc - 1], "w");
    if (saida == NULL) return 1;

    srand(time(NULL));

    for (int i = 3; i < argc - 1; i += 2) {
        char *sensor = argv[i];
        char *tipo = argv[i + 1];

        for (int j = 0; j < 2000; j++) {
            time_t ts = gerar_timestamp_aleatorio(timestamp_ini, timestamp_fim);
            char valor[256];
            gerar_valor(valor, tipo);
            fprintf(saida, "%ld %s %s\n", ts, sensor, valor);
        }
    }

    fclose(saida);
    return 0;
}
