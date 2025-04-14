
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PARCELS 100

// Structure for Parcel data
typedef struct {
    int parcelID;
    char sender[50];
    char receiver[50];
    char address[100];
    int priority;
    char status[20];  // Pending, In-Transit, Delivered
    char deliveryConfirmation[100]; // Delivery Confirmation details
} Parcel;

// Array to store parcels
Parcel parcels[MAX_PARCELS];
int parcelCount = 0;

// Function to generate unique Parcel ID
int generateParcelID() {
    return parcelCount + 1;
}

// Function to add a parcel
void addParcel() {
    if (parcelCount < MAX_PARCELS) {
        Parcel newParcel;

        // Generate unique Parcel ID
        newParcel.parcelID = generateParcelID();

        // Input parcel details
        printf("Enter sender name: ");
        getchar();  // Clear the input buffer
        fgets(newParcel.sender, 50, stdin);
        newParcel.sender[strcspn(newParcel.sender, "\n")] = '\0'; // Remove newline character

        printf("Enter receiver name: ");
        fgets(newParcel.receiver, 50, stdin);
        newParcel.receiver[strcspn(newParcel.receiver, "\n")] = '\0'; // Remove newline character

        printf("Enter address: ");
        fgets(newParcel.address, 100, stdin);
        newParcel.address[strcspn(newParcel.address, "\n")] = '\0'; // Remove newline character

        printf("Enter priority (1 - High, 2 - Medium, 3 - Low): ");
        scanf("%d", &newParcel.priority);

        // Set status as "Pending" initially
        strcpy(newParcel.status, "Pending");

        // Add the parcel to the array
        parcels[parcelCount] = newParcel;
        parcelCount++;

        printf("Parcel added successfully with ID: %d\n", newParcel.parcelID);
    } else {
        printf("Parcel storage is full.\n");
    }
}

// Function to display all parcels
void displayParcels() {
    if (parcelCount == 0) {
        printf("No parcels to display.\n");
        return;
    }

    printf("\nParcels in Queue:\n");
    for (int i = 0; i < parcelCount; i++) {
        printf("\nParcel ID: %d\n", parcels[i].parcelID);
        printf("Sender: %s\n", parcels[i].sender);
        printf("Receiver: %s\n", parcels[i].receiver);
        printf("Address: %s\n", parcels[i].address);
        printf("Priority: %d\n", parcels[i].priority);
        printf("Status: %s\n", parcels[i].status);
        printf("Delivery Confirmation: %s\n", parcels[i].deliveryConfirmation);
    }
}

// Function to mark parcel as delivered and add confirmation
void markAsDelivered(int parcelID) {
    for (int i = 0; i < parcelCount; i++) {
        if (parcels[i].parcelID == parcelID) {
            strcpy(parcels[i].status, "Delivered");
            printf("Enter delivery confirmation details: ");
            getchar();  // Clear the input buffer
            fgets(parcels[i].deliveryConfirmation, 100, stdin);
            parcels[i].deliveryConfirmation[strcspn(parcels[i].deliveryConfirmation, "\n")] = '\0'; // Remove newline character
            printf("Parcel ID %d has been marked as Delivered.\n", parcelID);
            return;
        }
    }
    printf("Parcel ID %d not found.\n", parcelID);
}

// Function to search parcel by ID
void searchParcelByID(int parcelID) {
    for (int i = 0; i < parcelCount; i++) {
        if (parcels[i].parcelID == parcelID) {
            printf("\nParcel Details for ID: %d\n", parcelID);
            printf("Sender: %s\n", parcels[i].sender);
            printf("Receiver: %s\n", parcels[i].receiver);
            printf("Address: %s\n", parcels[i].address);
            printf("Priority: %d\n", parcels[i].priority);
            printf("Status: %s\n", parcels[i].status);
            printf("Delivery Confirmation: %s\n", parcels[i].deliveryConfirmation);
            return;
        }
    }
    printf("Parcel ID %d not found.\n", parcelID);
}

int main() {
    int choice, parcelID;

    while (1) {
        printf("\nParcel Management System - Stage 5: Parcel Tracking and Confirmation\n");
        printf("1. Add Parcel\n");
        printf("2. Display All Parcels\n");
        printf("3. Mark Parcel as Delivered\n");
        printf("4. Search Parcel by ID\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addParcel();
                break;
            case 2:
                displayParcels();
                break;
            case 3:
                printf("Enter Parcel ID to mark as delivered: ");
                scanf("%d", &parcelID);
                markAsDelivered(parcelID);
                break;
            case 4:
                printf("Enter Parcel ID to search: ");
                scanf("%d", &parcelID);
                searchParcelByID(parcelID);
                break;
            case 5:
                printf("Exiting program.\n");
                exit(0);
            default:
                printf("Invalid choice, please try again.\n");
        }
    }

    return 0;
}
