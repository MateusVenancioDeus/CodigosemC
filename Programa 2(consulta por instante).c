#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    time_t timestamp;
    char sensor_id[50];
    char value[256];
} SensorReading;

int compareTimestamp(const void *key, const void *elem) {
    time_t targetTimestamp = *((time_t *)key);
    SensorReading *reading = (SensorReading *)elem;
    if (targetTimestamp < reading->timestamp) return -1;
    if (targetTimestamp > reading->timestamp) return 1;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <ID_SENSOR> <TIMESTAMP_CONSULTA>\n", argv[0]);
        return 1;
    }

    char *sensorId = argv[1];
    time_t queryTimestamp = atol(argv[2]);

    char inputFileName[100];
    snprintf(inputFileName, sizeof(inputFileName), "%s.txt", sensorId);

    FILE *inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        perror("Erro ao abrir o arquivo do sensor");
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

    if (numReadings == 0) {
        printf("Nenhuma leitura encontrada para o sensor %s.\n", sensorId);
        free(allReadings);
        return 0;
    }

    SensorReading *foundReading = NULL;
    int low = 0;
    int high = numReadings - 1;
    int mid;

    while (low <= high) {
        mid = low + (high - low) / 2;
        if (allReadings[mid].timestamp == queryTimestamp) {
            foundReading = &allReadings[mid];
            break;
        } else if (allReadings[mid].timestamp < queryTimestamp) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (foundReading == NULL) {
        if (low >= numReadings) {
            foundReading = &allReadings[numReadings - 1];
        } else if (high < 0) {
            foundReading = &allReadings[0];
        } else {
            long diffLow = labs(allReadings[low].timestamp - queryTimestamp);
            long diffHigh = labs(allReadings[high].timestamp - queryTimestamp);
            if (diffLow < diffHigh) {
                foundReading = &allReadings[low];
            } else {
                foundReading = &allReadings[high];
            }
        }
    }

    if (foundReading != NULL) {
        printf("Leitura mais próxima para o sensor %s e timestamp %ld:\n", sensorId, queryTimestamp);
        printf("Timestamp: %ld, ID Sensor: %s, Valor: %s\n", foundReading->timestamp, foundReading->sensor_id, foundReading->value);
    } else {
        printf("Não foi possível encontrar uma leitura próxima.\n");
    }

    free(allReadings);

    return 0;
}

