#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef struct {
    time_t timestamp;
    char sensor_id[10];
    char value[256];
} SensorReading;

int isValidSensor(const char *sensor) {
    return strcmp(sensor, "CONJ_Z") == 0 ||
           strcmp(sensor, "CONJ_Q") == 0 ||
           strcmp(sensor, "TEXTO") == 0 ||
           strcmp(sensor, "BINARIO") == 0;
}

int isNumber(const char *str) {
    if (!str || *str == '\0') return 0;
    for (int i = 0; str[i]; i++) {
        if (!isdigit((unsigned char)str[i])) return 0;
    }
    return 1;
}

int compareTimestamp(const void *a, const void *b) {
    SensorReading *sa = (SensorReading *)a;
    SensorReading *sb = (SensorReading *)b;
    if (sa->timestamp < sb->timestamp) return -1;
    if (sa->timestamp > sb->timestamp) return 1;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <TIPO_SENSOR> <TIMESTAMP>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *sensorId = argv[1];
    char *timestampStr = argv[2];

    if (!isValidSensor(sensorId)) {
        fprintf(stderr, "Sensor inválido. Use CONJ_Z, CONJ_Q, TEXTO ou BINARIO.\n");
        return EXIT_FAILURE;
    }

    if (!isNumber(timestampStr)) {
        fprintf(stderr, "Timestamp inválido. Deve conter somente números.\n");
        return EXIT_FAILURE;
    }

    time_t queryTimestamp = (time_t)atoll(timestampStr);
    if (queryTimestamp <= 0) {
        fprintf(stderr, "Timestamp deve ser um número positivo.\n");
        return EXIT_FAILURE;
    }

    char filename[20];
    snprintf(filename, sizeof(filename), "%s.txt", sensorId);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erro ao abrir arquivo do sensor");
        return EXIT_FAILURE;
    }

    SensorReading *readings = NULL;
    int capacity = 0;
    int count = 0;
    char line[512];
    int lineNum = 0;

    while (fgets(line, sizeof(line), file)) {
        lineNum++;
        line[strcspn(line, "\r\n")] = 0; 

        if (count >= capacity) {
            capacity = (capacity == 0) ? 10 : capacity * 2;
            SensorReading *tmp = realloc(readings, capacity * sizeof(SensorReading));
            if (!tmp) {
                fprintf(stderr, "Erro de memória ao ler linha %d\n", lineNum);
                free(readings);
                fclose(file);
                return EXIT_FAILURE;
            }
            readings = tmp;
        }

        long long ts;
        char sensorFromFile[10];
        char val[256];
        int fields = sscanf(line, "%lld %9s %255[^\n]", &ts, sensorFromFile, val);
        if (fields != 3) {
            fprintf(stderr, "Formato inválido na linha %d: '%s'\n", lineNum, line);
            continue; 
        }

        if (strcmp(sensorFromFile, sensorId) != 0) {
            fprintf(stderr, "Sensor diferente na linha %d: esperado %s, encontrado %s\n", lineNum, sensorId, sensorFromFile);
            continue; 
        }

        if (ts < 0) {
            fprintf(stderr, "Timestamp inválido na linha %d\n", lineNum);
            continue;
        }

        readings[count].timestamp = (time_t)ts;
        strncpy(readings[count].sensor_id, sensorFromFile, sizeof(readings[count].sensor_id) - 1);
        readings[count].sensor_id[sizeof(readings[count].sensor_id) - 1] = '\0';
        strncpy(readings[count].value, val, sizeof(readings[count].value) - 1);
        readings[count].value[sizeof(readings[count].value) - 1] = '\0';
        count++;
    }

    fclose(file);

    if (count == 0) {
        fprintf(stderr, "Nenhuma leitura válida encontrada no arquivo %s.\n", filename);
        free(readings);
        return EXIT_FAILURE;
    }

    qsort(readings, count, sizeof(SensorReading), compareTimestamp);


    int low = 0, high = count - 1, mid;
    SensorReading *found = NULL;

    while (low <= high) {
        mid = low + (high - low) / 2;
        if (readings[mid].timestamp == queryTimestamp) {
            found = &readings[mid];
            break;
        }
        else if (readings[mid].timestamp < queryTimestamp) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    if (!found) {
        if (low >= count) found = &readings[count - 1];
        else if (high < 0) found = &readings[0];
        else {
            long long diffLow = llabs((long long)readings[low].timestamp - (long long)queryTimestamp);
            long long diffHigh = llabs((long long)readings[high].timestamp - (long long)queryTimestamp);
            found = (diffLow < diffHigh) ? &readings[low] : &readings[high];
        }
    }

    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    char nowStr[30];
    strftime(nowStr, sizeof(nowStr), "%d/%m/%Y %H:%M:%S", tm_now);

    printf("\nBusca finalizada em %s\n\n", nowStr);
    printf("Resultado para sensor \"%s\" para timestamp solicitado %lld:\n", sensorId, (long long)queryTimestamp);
    printf("Timestamp encontrado: %lld\n", (long long)found->timestamp);

    if (strcmp(sensorId, "CONJ_Z") == 0) {
        int valInt = atoi(found->value);
        printf("Valor inteiro: %d\n", valInt);
    }
    else if (strcmp(sensorId, "CONJ_Q") == 0) {
        float valFloat = atof(found->value);
        printf("Valor float: %.6f\n", valFloat);
    }
    else if (strcmp(sensorId, "TEXTO") == 0) {
        printf("Texto: %s\n", found->value);
    }
    else if (strcmp(sensorId, "BINARIO") == 0) {
        if (strcmp(found->value, "0") == 0 || strcasecmp(found->value, "false") == 0)
            printf("Valor binário: falso (0)\n");
        else if (strcmp(found->value, "1") == 0 || strcasecmp(found->value, "true") == 0)
            printf("Valor binário: verdadeiro (1)\n");
        else
            printf("Valor binário inválido: %s\n", found->value);
    }

    free(readings);
    return EXIT_SUCCESS;
}
