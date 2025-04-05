#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Parcel {
    int id;
    char sender[100];
    char receiver[100];
    char address[100];
    int priority;

    struct Parcel* next;
};


struct Parcel* front = NULL;
struct Parcel* rear = NULL;


struct Parcel* createParcel() {
    struct Parcel* newParcel = (struct Parcel*)malloc(sizeof(struct Parcel));

    printf("Enter Parcel ID: ");
    scanf("%d", &newParcel->id);
    getchar(); 

    printf("Enter Sender Name: ");
    fgets(newParcel->sender, 100, stdin);
    newParcel->sender[strcspn(newParcel->sender, "\n")] = '\0';

    printf("Enter Receiver Name: ");
    fgets(newParcel->receiver, 100, stdin);
    newParcel->receiver[strcspn(newParcel->receiver, "\n")] = '\0';

    printf("Enter Address: ");
    fgets(newParcel->address, 100, stdin);
    newParcel->address[strcspn(newParcel->address, "\n")] = '\0';

    printf("Enter Priority (1-5): ");
    scanf("%d", &newParcel->priority);

    newParcel->next = NULL;
    return newParcel;
}


void addParcel() {
    struct Parcel* newParcel = createParcel();

    if (rear == NULL) {
        
        front = rear = newParcel;
    } else {
        rear->next = newParcel;
        rear = newParcel;
    }

    printf("Parcel added to the queue successfully!\n");
}


void displayParcels() {
    if (front == NULL) {
        printf("No parcels in the queue.\n");
        return;
    }

    struct Parcel* temp = front;
    printf("\n--- Parcels in Queue ---\n");
    while (temp != NULL) {
        printf("ID: %d | Sender: %s | Receiver: %s | Address: %s | Priority: %d\n",
               temp->id, temp->sender, temp->receiver, temp->address, temp->priority);
        temp = temp->next;
    }
}


void removeParcel() {
    if (front == NULL) {
        printf("No parcel to remove. Queue is empty.\n");
        return;
    }

    struct Parcel* temp = front;
    front = front->next;

    if (front == NULL) {
        rear = NULL;  
    }

    printf("Parcel ID %d has been removed (delivered).\n", temp->id);
    free(temp);
}

int main() {
    int choice;

    while (1) {
        printf("\n--- Parcel Management System ---\n");
        printf("1. Add Parcel\n");
        printf("2. Display Parcels\n");
        printf("3. Remove Parcel\n");
        printf("4. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1:
                addParcel();
                break;
            case 2:
                displayParcels();
                break;
            case 3:
                removeParcel();
                break;
            case 4:
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}

