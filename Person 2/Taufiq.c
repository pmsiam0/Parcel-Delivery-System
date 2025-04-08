#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 50

typedef struct Parcel {
    int id;
    char sender[MAX_LENGTH];
    char receiver[MAX_LENGTH];
    char address[MAX_LENGTH];
    char status[MAX_LENGTH];
    struct Parcel* next;
} Parcel;

typedef struct {
    Parcel* front;
    Parcel* rear;
} ParcelQueue;

void initializeQueue(ParcelQueue* q) {
    q->front = q->rear = NULL;
}

void enqueue(ParcelQueue* q, int id, char sender[], char receiver[], char address[]) {
    Parcel* newParcel = (Parcel*)malloc(sizeof(Parcel));
    if (!newParcel) {
        printf("Memory allocation failed!\n");
        return;
    }
    newParcel->id = id;
    strcpy(newParcel->sender, sender);
    strcpy(newParcel->receiver, receiver);
    strcpy(newParcel->address, address);
    strcpy(newParcel->status, "Pending");
    newParcel->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = newParcel;
    } else {
        q->rear->next = newParcel;
        q->rear = newParcel;
    }
}

void updateStatus(ParcelQueue* q, int id, char newStatus[]) {
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

void dequeue(ParcelQueue* q) {
    if (q->front == NULL) {
        printf("No parcels to remove.\n");
        return;
    }
    
    Parcel* temp = q->front;
    if (strcmp(temp->status, "Delivered") == 0) {
        q->front = q->front->next;
        if (q->front == NULL) q->rear = NULL; 
        printf("Parcel ID %d removed from queue.\n", temp->id);
        free(temp);
    } else {
        printf("Only delivered parcels can be removed.\n");
    }
}

void displayParcels(ParcelQueue* q) {
    Parcel* temp = q->front;
    if (temp == NULL) {
        printf("No parcels in the queue.\n");
        return;
    }
    printf("\nParcel List:\n");
    while (temp != NULL) {
        printf("ID: %d, Sender: %s, Receiver: %s, Address: %s, Status: %s\n",
               temp->id, temp->sender, temp->receiver, temp->address, temp->status);
        temp = temp->next;
    }
}

void searchParcel(ParcelQueue* q, int id) {
    Parcel* temp = q->front;
    while (temp != NULL) {
        if (temp->id == id) {
            printf("Parcel Found - ID: %d, Sender: %s, Receiver: %s, Address: %s, Status: %s\n",
                   temp->id, temp->sender, temp->receiver, temp->address, temp->status);
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

int main() {
    ParcelQueue queue;
    initializeQueue(&queue);
    
    int choice, id;
    char sender[MAX_LENGTH], receiver[MAX_LENGTH], address[MAX_LENGTH], status[MAX_LENGTH];

    do {
        printf("\nParcel Delivery System Menu:\n");
        printf("1. Add Parcel\n");
        printf("2. Update Parcel Status\n");
        printf("3. Display All Parcels\n");
        printf("4. Search Parcel\n");
        printf("5. Remove Delivered Parcel\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // To consume newline after scanf
        
        switch (choice) {
            case 1:
                printf("Enter Parcel ID: ");
                scanf("%d", &id);
                getchar();
                printf("Enter Sender Name: ");
                fgets(sender, MAX_LENGTH, stdin);
                sender[strcspn(sender, "\n")] = '\0';
                printf("Enter Receiver Name: ");
                fgets(receiver, MAX_LENGTH, stdin);
                receiver[strcspn(receiver, "\n")] = '\0';
                printf("Enter Address: ");
                fgets(address, MAX_LENGTH, stdin);
                address[strcspn(address, "\n")] = '\0';
                
                enqueue(&queue, id, sender, receiver, address);
                printf("Parcel added successfully.\n");
                break;
            
            case 2:
                printf("Enter Parcel ID to update: ");
                scanf("%d", &id);
                getchar();
                printf("Enter New Status: ");
                fgets(status, MAX_LENGTH, stdin);
                status[strcspn(status, "\n")] = '\0';
                
                updateStatus(&queue, id, status);
                break;
            
            case 3:
                displayParcels(&queue);
                break;
            
            case 4:
                printf("Enter Parcel ID to search: ");
                scanf("%d", &id);
                searchParcel(&queue, id);
                break;
            
            case 5:
                dequeue(&queue);
                break;
            
            case 6:
                printf("Exiting the system...\n");
                freeQueue(&queue);
                break;
            
            default:
                printf("Invalid choice! Please enter a number between 1 and 6.\n");
        }
    } while (choice != 6);
    
    return 0;
}
