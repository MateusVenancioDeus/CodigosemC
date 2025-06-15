#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

time_t converter_para_timestap(int dia, int mes, int ano, int hora, int min, int seg) 
{
    struct tm t;
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;
    time_t timestamp = mktime(&t);
    if (timestamp == -1) {
        printf("Data inválida. Tente novamente.\n");
        return -1;
    } else {
        return timestamp;
    }
}

time_t gerar_timestamp_aleatorio(struct tm * inicial, struct tm * final) {
    time_t timestamp_inicial, timestamp_final;
    timestamp_inicial = mktime(inicial);
    if (timestamp_inicial == -1) {
        printf("Data inválida.\n");
        return -1;
    }
    timestamp_final = mktime(final);
    if (timestamp_final == -1) {
        printf("Data inválida.\n");
        return -1;
    }
    time_t timestamp_aleatorio = timestamp_inicial + rand() % (timestamp_final - timestamp_inicial + 1);
    return timestamp_aleatorio;
}

void generateRandomValue(char *buffer, const char *dataType) {
    if (strcmp(dataType, "int") == 0) {
        sprintf(buffer, "%d", rand() % 1000);
    } else if (strcmp(dataType, "bool") == 0) {
        sprintf(buffer, "%s", (rand() % 2 == 0) ? "true" : "false");
    } else if (strcmp(dataType, "float") == 0) {
        sprintf(buffer, "%.2f", (float)rand() / RAND_MAX * 100.0);
    } else if (strcmp(dataType, "string") == 0) {
        int len = 1 + rand() % 16;
        for (int i = 0; i < len; i++) {
            buffer[i] = 'a' + (rand() % 26);
        }
        buffer[len] = '\0';
    } else {
        strcpy(buffer, "UNKNOWN");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 6 || (argc - 4) % 2 != 0) {
        fprintf(stderr, "Uso: %s <data_inicio_dd/mm/aaaa_hh:mm:ss> <data_fim_dd/mm/aaaa_hh:mm:ss> <nome_sensor1> <tipo_dado1> [nome_sensor2 tipo_dado2 ...] <arquivo_saida>\n", argv[0]);
        return 1;
    }

    int start_day, start_month, start_year, start_hour, start_min, start_sec;
    int end_day, end_month, end_year, end_hour, end_min, end_sec;

    if (sscanf(argv[1], "%d/%d/%d_%d:%d:%d", &start_day, &start_month, &start_year, &start_hour, &start_min, &start_sec) != 6) {
        fprintf(stderr, "Formato de data/hora de início inválido. Use dd/mm/aaaa_hh:mm:ss\n");
        return 1;
    }
    time_t start_timestamp = converter_para_timestap(start_day, start_month, start_year, start_hour, start_min, start_sec);
    if (start_timestamp == -1) return 1;

    if (sscanf(argv[2], "%d/%d/%d_%d:%d:%d", &end_day, &end_month, &end_year, &end_hour, &end_min, &end_sec) != 6) {
        fprintf(stderr, "Formato de data/hora de fim inválido. Use dd/mm/aaaa_hh:mm:ss\n");
        return 1;
    }
    time_t end_timestamp = converter_para_timestap(end_day, end_month, end_year, end_hour, end_min, end_sec);
    if (end_timestamp == -1) return 1;

    if (start_timestamp > end_timestamp) {
        fprintf(stderr, "Intervalo de data/hora inválido: data de início é maior que a data de fim.\n");
        return 1;
    }

    char *outputFileName = argv[argc - 1];
    FILE *outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        perror("Erro ao criar arquivo de saída");
        return 1;
    }

    srand(time(NULL));

    struct tm tm_start_gen = *localtime(&start_timestamp);
    struct tm tm_end_gen = *localtime(&end_timestamp);

    for (int i = 3; i < argc - 1; i += 2) {
        char *sensorName = argv[i];
        char *dataType = argv[i+1];

        for (int j = 0; j < 2000; j++) {
            time_t randomTimestamp = gerar_timestamp_aleatorio(&tm_start_gen, &tm_end_gen);
            char valueBuffer[256];
            generateRandomValue(valueBuffer, dataType);
            fprintf(outputFile, "%ld %s %s\n", randomTimestamp, sensorName, valueBuffer);
        }
    }

    fclose(outputFile);
    printf("Arquivo teste '%s' gerado com sucesso.\n", outputFileName);

    return 0;
}

