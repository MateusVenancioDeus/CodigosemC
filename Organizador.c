#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long long timestamp;
    char sensor_id[50];
    char value[256];
} SensorReading;

int compareSensorReadingsDesc(const void *a, const void *b) {
    const SensorReading *readingA = (const SensorReading *)a;
    const SensorReading *readingB = (const SensorReading *)b;
    if (readingA->timestamp > readingB->timestamp) return -1;
    if (readingA->timestamp < readingB->timestamp) return 1;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "ERRO: Uso correto: %s <arquivo_entrada>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        fprintf(stderr, "ERRO: Não foi possível abrir o arquivo '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    SensorReading *allReadings = NULL;
    int numReadings = 0;
    int capacity = 0;
    char line[512];

    while (fgets(line, sizeof(line), inputFile)) {
        if (numReadings >= capacity) {
            capacity = (capacity == 0) ? 10 : capacity * 2;
            SensorReading *temp = realloc(allReadings, capacity * sizeof(SensorReading));
            if (!temp) {
                fprintf(stderr, "ERRO: Falha ao alocar memória.\n");
                free(allReadings);
                fclose(inputFile);
                return EXIT_FAILURE;
            }
            allReadings = temp;
        }

        int readItems = sscanf(line, "%lld %49s %255[^\n]", &allReadings[numReadings].timestamp,
                              allReadings[numReadings].sensor_id, allReadings[numReadings].value);

        if (readItems != 3) {
            fprintf(stderr, "ERRO: Linha mal formatada: %s", line);
            free(allReadings);
            fclose(inputFile);
            return EXIT_FAILURE;
        }

        numReadings++;
    }
    fclose(inputFile);

    if (numReadings == 0) {
        fprintf(stderr, "ERRO: Nenhuma leitura válida encontrada.\n");
        free(allReadings);
        return EXIT_FAILURE;
    }

    char **uniqueSensorIDs = NULL;
    int numUniqueSensors = 0;
    int sensorCapacity = 0;

    for (int i = 0; i < numReadings; i++) {
        int found = 0;
        for (int j = 0; j < numUniqueSensors; j++) {
            if (strcmp(allReadings[i].sensor_id, uniqueSensorIDs[j]) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            if (numUniqueSensors >= sensorCapacity) {
                sensorCapacity = (sensorCapacity == 0) ? 5 : sensorCapacity * 2;
                char **temp = realloc(uniqueSensorIDs, sensorCapacity * sizeof(char *));
                if (!temp) {
                    fprintf(stderr, "ERRO: Falha ao alocar memória para sensor IDs.\n");
                    for (int k = 0; k < numUniqueSensors; k++) free(uniqueSensorIDs[k]);
                    free(uniqueSensorIDs);
                    free(allReadings);
                    return EXIT_FAILURE;
                }
                uniqueSensorIDs = temp;
            }
            uniqueSensorIDs[numUniqueSensors] = strdup(allReadings[i].sensor_id);
            if (!uniqueSensorIDs[numUniqueSensors]) {
                fprintf(stderr, "ERRO: Falha ao duplicar sensor ID.\n");
                for (int k = 0; k < numUniqueSensors; k++) free(uniqueSensorIDs[k]);
                free(uniqueSensorIDs);
                free(allReadings);
                return EXIT_FAILURE;
            }
            numUniqueSensors++;
        }
    }

    for (int i = 0; i < numUniqueSensors; i++) {
        SensorReading *sensorReadings = NULL;
        int count = 0;
        int sensorCap = 0;

        for (int j = 0; j < numReadings; j++) {
            if (strcmp(allReadings[j].sensor_id, uniqueSensorIDs[i]) == 0) {
                if (count >= sensorCap) {
                    sensorCap = (sensorCap == 0) ? 10 : sensorCap * 2;
                    SensorReading *temp = realloc(sensorReadings, sensorCap * sizeof(SensorReading));
                    if (!temp) {
                        fprintf(stderr, "ERRO: Falha ao alocar memória para sensor '%s'.\n", uniqueSensorIDs[i]);
                        free(sensorReadings);
                        for (int k = 0; k < numUniqueSensors; k++) free(uniqueSensorIDs[k]);
                        free(uniqueSensorIDs);
                        free(allReadings);
                        return EXIT_FAILURE;
                    }
                    sensorReadings = temp;
                }
                sensorReadings[count++] = allReadings[j];
            }
        }

        if (count == 0) {
            fprintf(stderr, "ERRO: Nenhuma leitura para sensor '%s'.\n", uniqueSensorIDs[i]);
            free(sensorReadings);
            for (int k = 0; k < numUniqueSensors; k++) free(uniqueSensorIDs[k]);
            free(uniqueSensorIDs);
            free(allReadings);
            return EXIT_FAILURE;
        }

        qsort(sensorReadings, count, sizeof(SensorReading), compareSensorReadingsDesc);

        char outputFileName[100];
        snprintf(outputFileName, sizeof(outputFileName), "%s.txt", uniqueSensorIDs[i]);
        FILE *outputFile = fopen(outputFileName, "w");
        if (!outputFile) {
            fprintf(stderr, "ERRO: Não foi possível criar arquivo '%s'.\n", outputFileName);
            free(sensorReadings);
            for (int k = 0; k < numUniqueSensors; k++) free(uniqueSensorIDs[k]);
            free(uniqueSensorIDs);
            free(allReadings);
            return EXIT_FAILURE;
        }

        for (int j = 0; j < count; j++) {
            fprintf(outputFile, "%lld %s %s\n", sensorReadings[j].timestamp,
                    sensorReadings[j].sensor_id, sensorReadings[j].value);
        }

        fclose(outputFile);
        free(sensorReadings);
    }

    for (int i = 0; i < numUniqueSensors; i++) {
        free(uniqueSensorIDs[i]);
    }
    free(uniqueSensorIDs);
    free(allReadings);

    printf("Processamento finalizado com sucesso. Arquivos gerados para cada sensor.\n");
    return EXIT_SUCCESS;
}
