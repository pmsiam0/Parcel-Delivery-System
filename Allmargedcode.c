#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PARCELS 100
#define MAX_LENGTH 100
#define DATE_LENGTH 11

typedef struct Parcel {
    int id;
    char sender[MAX_LENGTH];
    char receiver[MAX_LENGTH];
    char address[MAX_LENGTH];
    int priority;
    char status[MAX_LENGTH];
    char expectedDelivery[DATE_LENGTH];
    char actualDelivery[DATE_LENGTH];
    struct Parcel* next;
} Parcel;

typedef struct {
    Parcel* front;
    Parcel* rear;
} ParcelQueue;

typedef struct {
    Parcel items[MAX_PARCELS];
    int size;
} PriorityQueue;


void initializeQueue(ParcelQueue* q);
int isQueueEmpty(ParcelQueue* q);
Parcel* createParcel();
void addParcel(ParcelQueue* q);
void enqueue(ParcelQueue* q, Parcel* newParcel);
void displayParcels(ParcelQueue* q);
void removeParcel(ParcelQueue* q);
void updateStatus(ParcelQueue* q);
void searchParcel(ParcelQueue* q);
void freeQueue(ParcelQueue* q);
void initPriorityQueue(PriorityQueue* pq);
void enqueuePriority(PriorityQueue* pq, Parcel p);
Parcel dequeuePriority(PriorityQueue* pq);
void showPriorityQueue(PriorityQueue* pq);
void addParcelWithDelivery(ParcelQueue* q);
void updateDeliveryStatus(ParcelQueue* q);
void displayDelayedParcels(ParcelQueue* q);
void sortByExpectedDelivery(ParcelQueue* q);
void displayMenu();

int main() {
    ParcelQueue queue;
    PriorityQueue priorityQueue;
    initializeQueue(&queue);
    initPriorityQueue(&priorityQueue);
    
    int choice;

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                addParcel(&queue);
                break;
            case 2:
                addParcelWithDelivery(&queue);
                break;
            case 3:
                displayParcels(&queue);
                break;
            case 4:
                removeParcel(&queue);
                break;
            case 5:
                updateStatus(&queue);
                break;
            case 6:
                updateDeliveryStatus(&queue);
                break;
            case 7:
                searchParcel(&queue);
                break;
            case 8:
                {
                    Parcel p;
                    printf("Enter parcel ID: ");
                    scanf("%d", &p.id);
                    getchar();
                    printf("Enter sender: ");
                    fgets(p.sender, MAX_LENGTH, stdin);
                    p.sender[strcspn(p.sender, "\n")] = '\0';
                    printf("Enter receiver: ");
                    fgets(p.receiver, MAX_LENGTH, stdin);
                    p.receiver[strcspn(p.receiver, "\n")] = '\0';
                    printf("Enter address: ");
                    fgets(p.address, MAX_LENGTH, stdin);
                    p.address[strcspn(p.address, "\n")] = '\0';
                    printf("Priority (1-5, 5 being highest): ");
                    scanf("%d", &p.priority);
                    getchar();
                    strcpy(p.status, "Pending");
                    strcpy(p.expectedDelivery, "Not set");
                    strcpy(p.actualDelivery, "Not Delivered");
                    enqueuePriority(&priorityQueue, p);
                }
                break;
            case 9:
                {
                    Parcel p = dequeuePriority(&priorityQueue);
                    if (p.id != 0) {
                        printf("\nDelivering: ID %d to %s (Priority %d)\n", p.id, p.receiver, p.priority);
                    }
                }
                break;
            case 10:
                showPriorityQueue(&priorityQueue);
                break;
            case 11:
                displayDelayedParcels(&queue);
                break;
            case 12:
                sortByExpectedDelivery(&queue);
                break;
            case 13:
                printf("Exiting system...\n");
                freeQueue(&queue);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 13);

    return 0;
}

void displayMenu() {
    printf("\n=== Parcel Management System ===\n");
    printf("1. Add Basic Parcel\n");
    printf("2. Add Parcel with Delivery Date\n");
    printf("3. Display All Parcels\n");
    printf("4. Remove Parcel (Deliver)\n");
    printf("5. Update Parcel Status\n");
    printf("6. Update Delivery Status\n");
    printf("7. Search Parcel\n");
    printf("8. Add Priority Parcel\n");
    printf("9. Deliver Highest Priority Parcel\n");
    printf("10. Show Priority Queue\n");
    printf("11. Display Delayed Parcels\n");
    printf("12. Sort by Expected Delivery\n");
    printf("13. Exit\n");
}

void initializeQueue(ParcelQueue* q) {
    q->front = q->rear = NULL;
}

int isQueueEmpty(ParcelQueue* q) {
    return q->front == NULL;
}

Parcel* createParcel() {
    Parcel* newParcel = (Parcel*)malloc(sizeof(Parcel));

    printf("Enter Parcel ID: ");
    scanf("%d", &newParcel->id);
    getchar();

    printf("Enter Sender Name: ");
    fgets(newParcel->sender, MAX_LENGTH, stdin);
    newParcel->sender[strcspn(newParcel->sender, "\n")] = '\0';

    printf("Enter Receiver Name: ");
    fgets(newParcel->receiver, MAX_LENGTH, stdin);
    newParcel->receiver[strcspn(newParcel->receiver, "\n")] = '\0';

    printf("Enter Address: ");
    fgets(newParcel->address, MAX_LENGTH, stdin);
    newParcel->address[strcspn(newParcel->address, "\n")] = '\0';

    printf("Enter Priority (1-5): ");
    scanf("%d", &newParcel->priority);
    getchar();

    strcpy(newParcel->status, "Pending");
    strcpy(newParcel->expectedDelivery, "Not set");
    strcpy(newParcel->actualDelivery, "Not Delivered");

    newParcel->next = NULL;
    return newParcel;
}

void addParcel(ParcelQueue* q) {
    Parcel* newParcel = createParcel();
    enqueue(q, newParcel);
    printf("Parcel added to the queue successfully!\n");
}

void enqueue(ParcelQueue* q, Parcel* newParcel) {
    if (q->rear == NULL) {
        q->front = q->rear = newParcel;
    } else {
        q->rear->next = newParcel;
        q->rear = newParcel;
    }
}

void displayParcels(ParcelQueue* q) {
    if (isQueueEmpty(q)) {
        printf("No parcels in the queue.\n");
        return;
    }

    Parcel* temp = q->front;
    printf("\n--- Parcels in Queue ---\n");
    printf("ID  | Sender       | Receiver     | Address              | Priority | Status      | Expected Delivery | Actual Delivery\n");
    printf("----+--------------+--------------+----------------------+----------+-------------+-------------------+----------------\n");
    
    while (temp != NULL) {
        printf("%-3d | %-12s | %-12s | %-20s | %-8d | %-11s | %-17s | %-15s\n",
               temp->id, temp->sender, temp->receiver, temp->address, 
               temp->priority, temp->status, temp->expectedDelivery, 
               temp->actualDelivery);
        temp = temp->next;
    }
}

void removeParcel(ParcelQueue* q) {
    if (isQueueEmpty(q)) {
        printf("No parcel to remove. Queue is empty.\n");
        return;
    }

    Parcel* temp = q->front;
    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    printf("Parcel ID %d has been removed (delivered).\n", temp->id);
    free(temp);
}

void updateStatus(ParcelQueue* q) {
    if (isQueueEmpty(q)) {
        printf("No parcels in the queue.\n");
        return;
    }

    int id;
    printf("Enter Parcel ID to update status: ");
    scanf("%d", &id);
    getchar();

    char newStatus[MAX_LENGTH];
    printf("Enter new status (Pending/In-Transit/Delivered): ");
    fgets(newStatus, MAX_LENGTH, stdin);
    newStatus[strcspn(newStatus, "\n")] = '\0';

    Parcel* temp = q->front;
    while (temp != NULL) {
        if (temp->id == id) {
            strcpy(temp->status, newStatus);
            printf("Parcel ID %d status updated to %s.\n", id, newStatus);
            return;
        }
        temp = temp->next;
    }
    printf("Parcel ID %d not found.\n", id);
}

void searchParcel(ParcelQueue* q) {
    if (isQueueEmpty(q)) {
        printf("No parcels in the queue.\n");
        return;
    }

    int id;
    printf("Enter Parcel ID to search: ");
    scanf("%d", &id);
    getchar();

    Parcel* temp = q->front;
    while (temp != NULL) {
        if (temp->id == id) {
            printf("\nParcel Found:\n");
            printf("ID: %d\nSender: %s\nReceiver: %s\nAddress: %s\n", 
                   temp->id, temp->sender, temp->receiver, temp->address);
            printf("Priority: %d\nStatus: %s\n", temp->priority, temp->status);
            printf("Expected Delivery: %s\nActual Delivery: %s\n",
                   temp->expectedDelivery, temp->actualDelivery);
            return;
        }
        temp = temp->next;
    }
    printf("Parcel ID %d not found.\n", id);
}

void freeQueue(ParcelQueue* q) {
    Parcel* temp;
    while (q->front != NULL) {
        temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    q->rear = NULL;
}

void initPriorityQueue(PriorityQueue* pq) {
    pq->size = 0;
}

void swapParcels(Parcel* a, Parcel* b) {
    Parcel temp = *a;
    *a = *b;
    *b = temp;
}

void enqueuePriority(PriorityQueue* pq, Parcel p) {
    if (pq->size >= MAX_PARCELS) {
        printf("Priority queue full\n");
        return;
    }

    pq->items[pq->size] = p;
    int now = pq->size;
    pq->size++;

    while (now > 0 && pq->items[now].priority > pq->items[(now-1)/2].priority) {
        swapParcels(&pq->items[now], &pq->items[(now-1)/2]);
        now = (now-1)/2;
    }
}

Parcel dequeuePriority(PriorityQueue* pq) {
    if (pq->size <= 0) {
        printf("Priority queue empty\n");
        Parcel empty = {0};
        return empty;
    }

    Parcel top = pq->items[0];
    pq->size--;
    pq->items[0] = pq->items[pq->size];

    int now = 0;
    while (1) {
        int left = 2*now + 1;
        int right = 2*now + 2;
        int max = now;

        if (left < pq->size && pq->items[left].priority > pq->items[max].priority)
            max = left;
        if (right < pq->size && pq->items[right].priority > pq->items[max].priority)
            max = right;

        if (max != now) {
            swapParcels(&pq->items[now], &pq->items[max]);
            now = max;
        } else {
            break;
        }
    }

    return top;
}

void showPriorityQueue(PriorityQueue* pq) {
    printf("\nAll parcels in priority queue:\n");
    printf("ID\tSender\t\tReceiver\tPriority\n");
    for (int i = 0; i < pq->size; i++) {
        printf("%d\t%s\t%s\t%d\n", pq->items[i].id, pq->items[i].sender, 
               pq->items[i].receiver, pq->items[i].priority);
    }
}

void addParcelWithDelivery(ParcelQueue* q) {
    Parcel* newParcel = (Parcel*)malloc(sizeof(Parcel));

    printf("Enter Parcel ID: ");
    scanf("%d", &newParcel->id);
    getchar();

    printf("Enter Sender Name: ");
    fgets(newParcel->sender, MAX_LENGTH, stdin);
    newParcel->sender[strcspn(newParcel->sender, "\n")] = '\0';

    printf("Enter Receiver Name: ");
    fgets(newParcel->receiver, MAX_LENGTH, stdin);
    newParcel->receiver[strcspn(newParcel->receiver, "\n")] = '\0';

    printf("Enter Address: ");
    fgets(newParcel->address, MAX_LENGTH, stdin);
    newParcel->address[strcspn(newParcel->address, "\n")] = '\0';

    printf("Enter Priority (1-5): ");
    scanf("%d", &newParcel->priority);
    getchar();

    strcpy(newParcel->status, "Pending");

    printf("Enter Expected Delivery Date (YYYY-MM-DD): ");
    fgets(newParcel->expectedDelivery, DATE_LENGTH, stdin);
    newParcel->expectedDelivery[strcspn(newParcel->expectedDelivery, "\n")] = '\0';

    strcpy(newParcel->actualDelivery, "Not Delivered");

    newParcel->next = NULL;
    enqueue(q, newParcel);
    printf("Parcel added to the queue successfully!\n");
}

void updateDeliveryStatus(ParcelQueue* q) {
    if (isQueueEmpty(q)) {
        printf("No parcels in the queue.\n");
        return;
    }

    int id;
    printf("Enter Parcel ID to update status: ");
    scanf("%d", &id);
    getchar();

    Parcel* temp = q->front;
    while (temp != NULL) {
        if (temp->id == id) {
            printf("Current status: %s\n", temp->status);
            printf("Enter new status (Pending/In-Transit/Delivered): ");
            char newStatus[MAX_LENGTH];
            fgets(newStatus, MAX_LENGTH, stdin);
            newStatus[strcspn(newStatus, "\n")] = '\0';

            strcpy(temp->status, newStatus);

            if (strcmp(newStatus, "Delivered") == 0) {
                printf("Enter actual delivery date (YYYY-MM-DD): ");
                fgets(temp->actualDelivery, DATE_LENGTH, stdin);
                temp->actualDelivery[strcspn(temp->actualDelivery, "\n")] = '\0';

                struct tm expected = {0}, actual = {0};
                sscanf(temp->expectedDelivery, "%d-%d-%d", &expected.tm_year, &expected.tm_mon, &expected.tm_mday);
                expected.tm_year -= 1900;
                expected.tm_mon -= 1;
                expected.tm_hour = 0;
                expected.tm_min = 0;
                expected.tm_sec = 0;

                sscanf(temp->actualDelivery, "%d-%d-%d", &actual.tm_year, &actual.tm_mon, &actual.tm_mday);
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
            return;
        }
        temp = temp->next;
    }
    printf("Parcel ID %d not found.\n", id);
}

void displayDelayedParcels(ParcelQueue* q) {
    if (isQueueEmpty(q)) {
        printf("No parcels in the queue.\n");
        return;
    }

    printf("\nDelayed Parcels:\n");
    printf("ID  | Sender       | Receiver     | Expected Delivery | Actual Delivery | Delay (days)\n");
    printf("----+--------------+--------------+-------------------+-----------------+-------------\n");

    Parcel* temp = q->front;
    int anyDelayed = 0;

    while (temp != NULL) {
        if (strcmp(temp->status, "Delivered") == 0 && strcmp(temp->actualDelivery, "Not Delivered") != 0) {
            struct tm expected = {0}, actual = {0};
            sscanf(temp->expectedDelivery, "%d-%d-%d", &expected.tm_year, &expected.tm_mon, &expected.tm_mday);
            expected.tm_year -= 1900;
            expected.tm_mon -= 1;
            expected.tm_hour = 0;
            expected.tm_min = 0;
            expected.tm_sec = 0;

            sscanf(temp->actualDelivery, "%d-%d-%d", &actual.tm_year, &actual.tm_mon, &actual.tm_mday);
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
                       temp->id, temp->sender, temp->receiver,
                       temp->expectedDelivery, temp->actualDelivery, diff);
                anyDelayed = 1;
            }
        }
        temp = temp->next;
    }

    if (!anyDelayed) {
        printf("No delayed parcels found.\n");
    }
}

void sortByExpectedDelivery(ParcelQueue* q) {
    if (isQueueEmpty(q)) {
        printf("No parcels in the queue.\n");
        return;
    }


    int count = 0;
    Parcel* temp = q->front;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }


    Parcel* parcels = (Parcel*)malloc(count * sizeof(Parcel));
    temp = q->front;
    for (int i = 0; i < count; i++) {
        parcels[i] = *temp;
        parcels[i].next = NULL;
        temp = temp->next;
    }


    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(parcels[j].expectedDelivery, parcels[j+1].expectedDelivery) > 0) {
                Parcel swap = parcels[j];
                parcels[j] = parcels[j+1];
                parcels[j+1] = swap;
            }
        }
    }

    printf("\nParcels Sorted by Expected Delivery Date:\n");
    printf("ID  | Sender       | Receiver     | Expected Delivery | Status\n");
    printf("----+--------------+--------------+-------------------+-------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-3d | %-12s | %-12s | %-17s | %-11s\n",
               parcels[i].id, parcels[i].sender, parcels[i].receiver,
               parcels[i].expectedDelivery, parcels[i].status);
    }

    free(parcels);
}
