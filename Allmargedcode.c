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
} DeliveryQueue;

void initializeQueue(DeliveryQueue *q) {
    q->front = q->rear = -1;
}

int isQueueEmpty(DeliveryQueue *q) {
    return q->front == -1;
}

int isQueueFull(DeliveryQueue *q) {
    return (q->rear + 1) % MAX_PARCELS == q->front;
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

    return p;
}

void updateStatus(DeliveryQueue *q, int id, char *newStatus) {
    for (int i = q->front; i != (q->rear + 1) % MAX_PARCELS; i = (i + 1) % MAX_PARCELS) {
        if (q->items[i].id == id) {
            strcpy(q->items[i].status, newStatus);
            if (strcmp(newStatus, "Delivered") == 0) {
                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                strftime(q->items[i].actualDelivery, DATE_LENGTH, "%Y-%m-%d", t);
                printf("\nParcel %d delivered on %s. Confirmation sent.\n", id, q->items[i].actualDelivery);
            }
            return;
        }
    }
    printf("\nParcel ID %d not found.\n", id);
}

void trackParcel(DeliveryQueue *q, int id) {
    for (int i = q->front; i != (q->rear + 1) % MAX_PARCELS; i = (i + 1) % MAX_PARCELS) {
        if (q->items[i].id == id) {
            printf("\n╔══════════════════════════════════════╗\n");
            printf("║          PARCEL INFORMATION         ║\n");
            printf("╠══════════════════════════════════════╣\n");
            printf("║ ID: %-32d ║\n", q->items[i].id);
            printf("║ Sender: %-27s ║\n", q->items[i].sender);
            printf("║ Receiver: %-25s ║\n", q->items[i].receiver);
            printf("║ Status: %-28s ║\n", q->items[i].status);
            printf("║ Priority: %-26d ║\n", q->items[i].priority);
            printf("║ Expected Delivery: %-17s ║\n", q->items[i].expectedDelivery);
            if (strcmp(q->items[i].status, "Delivered") == 0) {
                printf("║ Actual Delivery: %-18s ║\n", q->items[i].actualDelivery);
            }
            printf("╚══════════════════════════════════════╝\n");
            return;
        }
    }
    printf("\nParcel ID %d not found.\n", id);
}

void displayQueue(DeliveryQueue *q) {
    if (isQueueEmpty(q)) {
        printf("\nNo parcels in the queue.\n");
        return;
    }

    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                     CURRENT DELIVERY QUEUE                   ║\n");
    printf("╠══════╦══════════╦══════════╦═════════════════╦═══════════════╣\n");
    printf("║  ID  ║  Status  ║ Priority ║ Expected Delivery║    Address    ║\n");
    printf("╠══════╬══════════╬══════════╬═════════════════╬═══════════════╣\n");

    int i = q->front;
    while (1) {
        printf("║ %-4d ║ %-8s ║    %-4d ║ %-15s ║ %-13s ║\n",
               q->items[i].id, q->items[i].status,
               q->items[i].priority, q->items[i].expectedDelivery,
               q->items[i].address);

        if (i == q->rear) break;
        i = (i + 1) % MAX_PARCELS;
    }
    printf("╚══════╩══════════╩══════════╩═════════════════╩═══════════════╝\n");
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


    printf("╔══════════════════════════════════════╗\n");
    printf("║          DELIVERY MANAGEMENT         ║\n");
    printf("╠══════════════════════════════════════╣\n");
    printf("║  1. 📦 Add New Parcel                ║\n");
    printf("║  2. 🚚 Process Next Delivery         ║\n");
    printf("║  3. 🔄 Update Parcel Status          ║\n");
    printf("║  4. 🔍 Track Parcel                  ║\n");
    printf("║  5. 📋 View Delivery Queue           ║\n");
    printf("║  6. 🚪 Exit System                   ║\n");
    printf("╚══════════════════════════════════════╝\n\n");
}

int getMenuChoice() {
    int choice;
    int valid_input = 0;
    
    do {
        printf("Select an option (1-6): ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
        } else if (choice < 1 || choice > 6) {
            printf("Please enter a number between 1 and 6.\n");
        } else {
            valid_input = 1;
        }
        getchar();
    } while (!valid_input);
    
    return choice;
}

void pressEnterToContinue() {
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
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
                Parcel p = dequeue(&queue);
                if (p.id != 0) {
                    printf("\nProcessing delivery for Parcel ID %d...\n", p.id);
                    updateStatus(&queue, p.id, "In-Transit");
                }
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
                scanf("%d", &id);
                trackParcel(&queue, id);
                pressEnterToContinue();
                break;
            }
            case 5:
                displayQueue(&queue);
                pressEnterToContinue();
                break;
            case 6:
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
