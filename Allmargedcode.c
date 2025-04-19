#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PARCELS 100
#define DATE_LENGTH 11
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

typedef struct {
    int id;
    char sender[50];
    char receiver[50];
    char address[100];
    int priority;
    char status[20];
    char expectedDelivery[DATE_LENGTH];
    char actualDelivery[DATE_LENGTH];
} Parcel;

typedef struct {
    Parcel items[MAX_PARCELS];
    int front, rear;
    int size;
} DeliveryQueue;

void initializeQueue(DeliveryQueue *q) {
    q->front = q->rear = -1;
    q->size = 0;
}

int isQueueEmpty(DeliveryQueue *q) {
    return q->size == 0;
}

int isQueueFull(DeliveryQueue *q) {
    return q->size == MAX_PARCELS;
}

void enqueue(DeliveryQueue *q, Parcel p) {
    if (isQueueFull(q)) {
        printf("\nQueue is full. Cannot add more parcels.\n");
        return;
    }

    if (isQueueEmpty(q)) {
        q->front = q->rear = 0;
    } else {
        q->rear = (q->rear + 1) % MAX_PARCELS;
    }

    q->items[q->rear] = p;
    q->size++;
    printf("\nParcel %d added to delivery queue.\n", p.id);
}

Parcel dequeue(DeliveryQueue *q) {
    if (isQueueEmpty(q)) {
        printf("\nQueue is empty.\n");
        Parcel empty = {0};
        return empty;
    }

    Parcel p = q->items[q->front];
    
    if (q->front == q->rear) {
        q->front = q->rear = -1;
    } else {
        q->front = (q->front + 1) % MAX_PARCELS;
    }
    
    q->size--;
    return p;
}

void updateStatus(DeliveryQueue *q, int id, char *newStatus) {
    for (int i = 0; i < q->size; i++) {
        int index = (q->front + i) % MAX_PARCELS;
        if (q->items[index].id == id) {
            strcpy(q->items[index].status, newStatus);
            if (strcmp(newStatus, "Delivered") == 0) {
                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                strftime(q->items[index].actualDelivery, DATE_LENGTH, "%Y-%m-%d", t);
                printf("\nParcel %d delivered on %s. Confirmation sent.\n", id, q->items[index].actualDelivery);
            }
            return;
        }
    }
    printf("\nParcel ID %d not found.\n", id);
}

void trackParcel(DeliveryQueue *q, int id) {
    if (isQueueEmpty(q)) {
        printf("\nQueue is empty. No parcels to track.\n");
        return;
    }

    for (int i = 0; i < q->size; i++) {
        int index = (q->front + i) % MAX_PARCELS;
        if (q->items[index].id == id) {
            printf("\n+------------------------------------------+\n");
            printf("|           PARCEL INFORMATION             |\n");
            printf("+------------------------------------------+\n");
            printf("| ID: %-34d |\n", q->items[index].id);
            printf("| Sender: %-30s |\n", q->items[index].sender);
            printf("| Receiver: %-28s |\n", q->items[index].receiver);
            printf("| Address: %-29s |\n", q->items[index].address);
            printf("| Status: %-30s |\n", q->items[index].status);
            printf("| Priority: %-28d |\n", q->items[index].priority);
            printf("| Expected Delivery: %-19s |\n", q->items[index].expectedDelivery);
            if (strcmp(q->items[index].status, "Delivered") == 0) {
                printf("| Actual Delivery: %-20s |\n", q->items[index].actualDelivery);
            } else {
                printf("| Actual Delivery: %-20s |\n", "Not delivered");
            }
            printf("+------------------------------------------+\n");
            return;
        }
    }

    printf("\nParcel ID %d not found in the delivery queue.\n", id);
}

void displayQueue(DeliveryQueue *q) {
    if (isQueueEmpty(q)) {
        printf("\nNo parcels in the queue.\n");
        return;
    }

    printf("\n+-----+----------+----------+-----------------+---------------+\n");
    printf("| ID  |  Status  | Priority | Expected Delivery|    Address    |\n");
    printf("+-----+----------+----------+-----------------+---------------+\n");

    for (int i = 0; i < q->size; i++) {
        int index = (q->front + i) % MAX_PARCELS;
        printf("| %-3d | %-8s |    %-4d | %-15s | %-13s |\n",
               q->items[index].id, q->items[index].status,
               q->items[index].priority, q->items[index].expectedDelivery,
               q->items[index].address);
    }
    printf("+-----+----------+----------+-----------------+---------------+\n");
}

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void displayMenu() {
    clearScreen();
    
    printf("========================================\n");
    printf("|          DELIVERY MANAGEMENT         |\n");
    printf("|--------------------------------------|\n");
    printf("|  1. [📦] Add New Parcel              |\n");
    printf("|  2. [🚚] Process Next Delivery       |\n");
    printf("|  3. [🔄] Update Parcel Status        |\n");
    printf("|  4. [🔍] Track Parcel                |\n");
    printf("|  5. [📋] View Delivery Queue         |\n");
    printf("|  6. [✅] Confirm Delivery            |\n");
    printf("|  7. [🚪] Exit System                 |\n");
    printf("========================================\n\n");
}
void flushInput() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int getMenuChoice() {
    int choice;
    int valid_input = 0;

    do {
        printf("Select an option (1-7): ");
        if (scanf("%d", &choice) != 1) {
            flushInput(); 
            printf("Invalid input. Please enter a number.\n");
        } else if (choice < 1 || choice > 7) {
            printf("Please enter a number between 1 and 7.\n");
        } else {
            flushInput(); 
            valid_input = 1;
        }
    } while (!valid_input);

    return choice;
}


void pressEnterToContinue() {
    printf("\nPress Enter to continue...");
    flushInput();
}


int main() {
    DeliveryQueue queue;
    initializeQueue(&queue);
    int choice;

    while (1) {
        displayMenu();
        choice = getMenuChoice();

        switch (choice) {
            case 1: {
                if (isQueueFull(&queue)) {
                    printf("\nQueue is full.\n");
                    pressEnterToContinue();
                    break;
                }
                Parcel p;
                printf("\nEnter Parcel ID: ");
                scanf("%d", &p.id);
                getchar();

                printf("Sender: ");
                fgets(p.sender, 50, stdin);
                p.sender[strcspn(p.sender, "\n")] = '\0';

                printf("Receiver: ");
                fgets(p.receiver, 50, stdin);
                p.receiver[strcspn(p.receiver, "\n")] = '\0';

                printf("Address: ");
                fgets(p.address, 100, stdin);
                p.address[strcspn(p.address, "\n")] = '\0';

                printf("Priority (1-5): ");
                scanf("%d", &p.priority);
                getchar();

                printf("Expected Delivery (YYYY-MM-DD): ");
                fgets(p.expectedDelivery, DATE_LENGTH, stdin);
                p.expectedDelivery[strcspn(p.expectedDelivery, "\n")] = '\0';

                strcpy(p.status, "Pending");
                strcpy(p.actualDelivery, "Not delivered");

                enqueue(&queue, p);
                pressEnterToContinue();
                break;
            }
            case 2: {
                if (isQueueEmpty(&queue)) {
                    printf("\nQueue is empty.\n");
                    pressEnterToContinue();
                    break;
                }
                Parcel p = queue.items[queue.front];
                printf("\nProcessing delivery for Parcel ID %d...\n", p.id);
                updateStatus(&queue, p.id, "In-Transit");
                pressEnterToContinue();
                break;
            }
            case 3: {
                int id;
                char status[20];
                printf("\nEnter Parcel ID: ");
                scanf("%d", &id);
                getchar();
                printf("New Status (Pending/In-Transit/Delivered): ");
                fgets(status, 20, stdin);
                status[strcspn(status, "\n")] = '\0';
                updateStatus(&queue, id, status);
                pressEnterToContinue();
                break;
            }
            case 4: {
    int id;
    printf("\nEnter Parcel ID to track: ");
    if (scanf("%d", &id) != 1) {
        flushInput();
        printf("Invalid input.\n");
        pressEnterToContinue();
        break;
    }
    flushInput(); 
    trackParcel(&queue, id);
    pressEnterToContinue();
    break;
}

            case 5:
                displayQueue(&queue);
                pressEnterToContinue();
                break;
            case 6: {
                if (isQueueEmpty(&queue)) {
                    printf("\nQueue is empty.\n");
                    pressEnterToContinue();
                    break;
                }
                Parcel p = dequeue(&queue);
                printf("\nConfirmed delivery for Parcel ID %d\n", p.id);
                pressEnterToContinue();
                break;
            }
            case 7:
                printf(COLOR_CYAN "\nThank you for using the Parcel Delivery System!\n");
                printf("Exiting...\n\n" COLOR_RESET);
                exit(0);
            default:
                printf("\nInvalid option. Please try again.\n");
                pressEnterToContinue();
        }
    }

    return 0;
}
