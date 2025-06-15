#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    time_t timestamp;
    char sensor_id[50];
    char value[256];
} SensorReading;

int compareSensorReadings(const void *a, const void *b) {
    SensorReading *readingA = (SensorReading *)a;
    SensorReading *readingB = (SensorReading *)b;
    if (readingA->timestamp < readingB->timestamp) return -1;
    if (readingA->timestamp > readingB->timestamp) return 1;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return 1;
    }

    SensorReading *allReadings = NULL;
    int numReadings = 0;
    int capacity = 0;

    char line[512];
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        if (numReadings >= capacity) {
            capacity = (capacity == 0) ? 10 : capacity * 2;
            allReadings = (SensorReading *)realloc(allReadings, capacity * sizeof(SensorReading));
            if (allReadings == NULL) {
                perror("Erro de alocação de memória");
                fclose(inputFile);
                return 1;
            }
        }

        if (sscanf(line, "%ld %s %s", &allReadings[numReadings].timestamp, allReadings[numReadings].sensor_id, allReadings[numReadings].value) == 3) {
            numReadings++;
        } else {
            fprintf(stderr, "Aviso: Linha mal formatada ignorada: %s", line);
        }
    }
    fclose(inputFile);

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
                uniqueSensorIDs = (char **)realloc(uniqueSensorIDs, sensorCapacity * sizeof(char *));
                if (uniqueSensorIDs == NULL) {
                    perror("Erro de alocação de memória para IDs de sensor");
                    free(allReadings);
                    return 1;
                }
            }
            uniqueSensorIDs[numUniqueSensors] = strdup(allReadings[i].sensor_id);
            if (uniqueSensorIDs[numUniqueSensors] == NULL) {
                perror("Erro de alocação de memória para ID de sensor");
                for(int k=0; k<numUniqueSensors; k++) free(uniqueSensorIDs[k]);
                free(uniqueSensorIDs);
                free(allReadings);
                return 1;
            }
            numUniqueSensors++;
        }
    }

    for (int i = 0; i < numUniqueSensors; i++) {
        SensorReading *currentSensorReadings = NULL;
        int currentNumReadings = 0;
        int currentCapacity = 0;

        for (int j = 0; j < numReadings; j++) {
            if (strcmp(allReadings[j].sensor_id, uniqueSensorIDs[i]) == 0) {
                if (currentNumReadings >= currentCapacity) {
                    currentCapacity = (currentCapacity == 0) ? 10 : currentCapacity * 2;
                    currentSensorReadings = (SensorReading *)realloc(currentSensorReadings, currentCapacity * sizeof(SensorReading));
                    if (currentSensorReadings == NULL) {
                        perror("Erro de alocação de memória para leituras de sensor atual");
                        for(int k=0; k<numUniqueSensors; k++) free(uniqueSensorIDs[k]);
                        free(uniqueSensorIDs);
                        free(allReadings);
                        return 1;
                    }
                }
                currentSensorReadings[currentNumReadings] = allReadings[j];
                currentNumReadings++;
            }
        }

        qsort(currentSensorReadings, currentNumReadings, sizeof(SensorReading), compareSensorReadings);

        char outputFileName[100];
        snprintf(outputFileName, sizeof(outputFileName), "%s.txt", uniqueSensorIDs[i]);
        FILE *outputFile = fopen(outputFileName, "w");
        if (outputFile == NULL) {
            perror("Erro ao criar arquivo de saída");
            free(currentSensorReadings);
            for(int k=0; k<numUniqueSensors; k++) free(uniqueSensorIDs[k]);
            free(uniqueSensorIDs);
            free(allReadings);
            return 1;
        }

        for (int j = 0; j < currentNumReadings; j++) {
            fprintf(outputFile, "%ld %s %s\n", currentSensorReadings[j].timestamp, currentSensorReadings[j].sensor_id, currentSensorReadings[j].value);
        }
        fclose(outputFile);
        free(currentSensorReadings);
    }

    for(int i=0; i<numUniqueSensors; i++) free(uniqueSensorIDs[i]);
    free(uniqueSensorIDs);
    free(allReadings);

    printf("Processamento concluído. Arquivos de sensores gerados.\n");

    return 0;
}

