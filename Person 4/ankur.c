#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PARCELS 100
#define DATE_LENGTH 11

typedef struct {
    int parcelID;
    char sender[50];
    char receiver[50];
    char address[100];
    int priority;
    char status[20];
    char expectedDelivery[DATE_LENGTH];
    char actualDelivery[DATE_LENGTH];
} Parcel;

typedef struct {
    Parcel parcels[MAX_PARCELS];
    int front, rear;
} Queue;

void initializeQueue(Queue *q) {
    q->front = -1;
    q->rear = -1;
}

int isQueueEmpty(Queue *q) {
    return q->front == -1;
}

int isQueueFull(Queue *q) {
    return (q->rear + 1) % MAX_PARCELS == q->front;
}

void enqueue(Queue *q, Parcel p) {
    if (isQueueFull(q)) {
        printf("Queue is full. Cannot add more parcels.\n");
        return;
    }

    if (isQueueEmpty(q)) {
        q->front = q->rear = 0;
    } else {
        q->rear = (q->rear + 1) % MAX_PARCELS;
    }

    q->parcels[q->rear] = p;
}

Parcel dequeue(Queue *q) {
    if (isQueueEmpty(q)) {
        printf("Queue is empty.\n");
        Parcel emptyParcel = {0};
        return emptyParcel;
    }

    Parcel p = q->parcels[q->front];

    if (q->front == q->rear) {
        q->front = q->rear = -1;
    } else {
        q->front = (q->front + 1) % MAX_PARCELS;
    }

    return p;
}

void displayParcels(Queue *q) {
    if (isQueueEmpty(q)) {
        printf("No parcels in the queue.\n");
        return;
    }

    printf("\nCurrent Parcels in Queue:\n");
    printf("ID  | Sender       | Receiver     | Address              | Priority | Status      | Expected Delivery | Actual Delivery\n");
    printf("----+--------------+--------------+----------------------+----------+-------------+-------------------+----------------\n");

    int i = q->front;
    while (1) {
        Parcel p = q->parcels[i];
        printf("%-3d | %-12s | %-12s | %-20s | %-8d | %-11s | %-17s | %-15s\n",
               p.parcelID, p.sender, p.receiver, p.address, p.priority, p.status,
               p.expectedDelivery, p.actualDelivery);

        if (i == q->rear) break;
        i = (i + 1) % MAX_PARCELS;
    }
}

void addParcel(Queue *q) {
    if (isQueueFull(q)) {
        printf("Queue is full. Cannot add more parcels.\n");
        return;
    }

    Parcel newParcel;

    printf("\nEnter Parcel Details:\n");
    printf("Parcel ID: ");
    scanf("%d", &newParcel.parcelID);
    getchar();

    printf("Sender: ");
    fgets(newParcel.sender, 50, stdin);
    newParcel.sender[strcspn(newParcel.sender, "\n")] = '\0';

    printf("Receiver: ");
    fgets(newParcel.receiver, 50, stdin);
    newParcel.receiver[strcspn(newParcel.receiver, "\n")] = '\0';

    printf("Address: ");
    fgets(newParcel.address, 100, stdin);
    newParcel.address[strcspn(newParcel.address, "\n")] = '\0';

    printf("Priority (1-5, 5 being highest): ");
    scanf("%d", &newParcel.priority);
    getchar();

    strcpy(newParcel.status, "Pending");

    printf("Expected Delivery Date (YYYY-MM-DD): ");
    fgets(newParcel.expectedDelivery, DATE_LENGTH, stdin);
    newParcel.expectedDelivery[strcspn(newParcel.expectedDelivery, "\n")] = '\0';

    strcpy(newParcel.actualDelivery, "Not Delivered");

    enqueue(q, newParcel);
    printf("Parcel added successfully!\n");
}

void updateDeliveryStatus(Queue *q) {
    if (isQueueEmpty(q)) {
        printf("No parcels in the queue.\n");
        return;
    }

    int id;
    printf("Enter Parcel ID to update status: ");
    scanf("%d", &id);

    int found = 0;
    int i = q->front;
    while (1) {
        if (q->parcels[i].parcelID == id) {
            found = 1;
            break;
        }
        if (i == q->rear) break;
        i = (i + 1) % MAX_PARCELS;
    }

    if (!found) {
        printf("Parcel with ID %d not found.\n", id);
        return;
    }

    printf("Current status: %s\n", q->parcels[i].status);
    printf("Enter new status (Pending/In-Transit/Delivered): ");
    char newStatus[20];
    scanf("%s", newStatus);

    strcpy(q->parcels[i].status, newStatus);

    if (strcmp(newStatus, "Delivered") == 0) {
        printf("Enter actual delivery date (YYYY-MM-DD): ");
        scanf("%s", q->parcels[i].actualDelivery);

        struct tm expected = {0}, actual = {0};
        sscanf(q->parcels[i].expectedDelivery, "%d-%d-%d", &expected.tm_year, &expected.tm_mon, &expected.tm_mday);
expected.tm_year -= 1900;
expected.tm_mon -= 1;
expected.tm_hour = 0;
expected.tm_min = 0;
expected.tm_sec = 0;

sscanf(q->parcels[i].actualDelivery, "%d-%d-%d", &actual.tm_year, &actual.tm_mon, &actual.tm_mday);
actual.tm_year -= 1900;
actual.tm_mon -= 1;
actual.tm_hour = 0;
actual.tm_min = 0;
actual.tm_sec = 0;

        time_t expected_time = mktime(&expected);
        time_t actual_time = mktime(&actual);

        double diff = difftime(actual_time, expected_time) / (60 * 60 * 24);

        if (diff > 0) {
            printf("Warning: This parcel was delivered %.0f days late!\n", diff);
        } else if (diff == 0) {
            printf("Parcel delivered on time!\n");
        } else {
            printf("Parcel delivered %.0f days early!\n", -diff);
        }
    }

    printf("Status updated successfully!\n");
}

void displayDelayedParcels(Queue *q) {
    if (isQueueEmpty(q)) {
        printf("No parcels in the queue.\n");
        return;
    }

    printf("\nDelayed Parcels:\n");
    printf("ID  | Sender       | Receiver     | Expected Delivery | Actual Delivery | Delay (days)\n");
    printf("----+--------------+--------------+-------------------+-----------------+-------------\n");

    int i = q->front;
    int anyDelayed = 0;

    while (1) {
        Parcel p = q->parcels[i];

        if (strcmp(p.status, "Delivered") == 0 && strcmp(p.actualDelivery, "Not Delivered") != 0) {
            struct tm expected = {0}, actual = {0};
            sscanf(p.expectedDelivery, "%d-%d-%d", &expected.tm_year, &expected.tm_mon, &expected.tm_mday);
expected.tm_year -= 1900;
expected.tm_mon -= 1;
expected.tm_hour = 0;
expected.tm_min = 0;
expected.tm_sec = 0;

sscanf(p.actualDelivery, "%d-%d-%d", &actual.tm_year, &actual.tm_mon, &actual.tm_mday);
actual.tm_year -= 1900;
actual.tm_mon -= 1;
actual.tm_hour = 0;
actual.tm_min = 0;
actual.tm_sec = 0;

            time_t expected_time = mktime(&expected);
            time_t actual_time = mktime(&actual);

            double diff = difftime(actual_time, expected_time) / (60 * 60 * 24);

            if (diff > 0) {
                printf("%-3d | %-12s | %-12s | %-17s | %-15s | %.0f\n",
                       p.parcelID, p.sender, p.receiver,
                       p.expectedDelivery, p.actualDelivery, diff);
                anyDelayed = 1;
            }
        }

        if (i == q->rear) break;
        i = (i + 1) % MAX_PARCELS;
    }

    if (!anyDelayed) {
        printf("No delayed parcels found.\n");
    }
}

void sortByExpectedDelivery(Queue *q) {
    if (isQueueEmpty(q)) {
        printf("No parcels in the queue.\n");
        return;
    }


    int count = (q->rear >= q->front) ? (q->rear - q->front + 1) : (MAX_PARCELS - q->front + q->rear + 1);
    Parcel temp[MAX_PARCELS];

    int i = q->front;
    for (int j = 0; j < count; j++) {
        temp[j] = q->parcels[i];
        i = (i + 1) % MAX_PARCELS;
    }


    for (int j = 0; j < count - 1; j++) {
        for (int k = 0; k < count - j - 1; k++) {
            if (strcmp(temp[k].expectedDelivery, temp[k+1].expectedDelivery) > 0) {
                Parcel swap = temp[k];
                temp[k] = temp[k+1];
                temp[k+1] = swap;
            }
        }
    }

    printf("\nParcels Sorted by Expected Delivery Date:\n");
    printf("ID  | Sender       | Receiver     | Expected Delivery | Status\n");
    printf("----+--------------+--------------+-------------------+-------------\n");

    for (int j = 0; j < count; j++) {
        printf("%-3d | %-12s | %-12s | %-17s | %-11s\n",
               temp[j].parcelID, temp[j].sender, temp[j].receiver,
               temp[j].expectedDelivery, temp[j].status);
    }
}

int main() {
    Queue parcelQueue;
    initializeQueue(&parcelQueue);
    int choice;

    do {
        printf("\nParcel Management System - Delivery Time Tracking\n");
        printf("1. Add Parcel\n");
        printf("2. Display All Parcels\n");
        printf("3. Remove Parcel (Deliver)\n");
        printf("4. Update Delivery Status\n");
        printf("5. Display Delayed Parcels\n");
        printf("6. Sort Parcels by Expected Delivery Date\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                addParcel(&parcelQueue);
                break;
            case 2:
                displayParcels(&parcelQueue);
                break;
            case 3:
                if (!isQueueEmpty(&parcelQueue)) {
                    Parcel p = dequeue(&parcelQueue);
                    printf("Delivered Parcel: ID %d to %s\n", p.parcelID, p.receiver);
                } else {
                    printf("No parcels to deliver.\n");
                }
                break;
            case 4:
                updateDeliveryStatus(&parcelQueue);
                break;
            case 5:
                displayDelayedParcels(&parcelQueue);
                break;
            case 6:
                sortByExpectedDelivery(&parcelQueue);
                break;
            case 7:
                printf("Exiting system...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}
