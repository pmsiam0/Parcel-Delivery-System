
#include <stdio.h>
#include <stdlib.h>

#define MAX 100

struct Parcel {
    int id;
    char destination[50];
    int priority;
};

struct PriorityQueue {
    struct Parcel items[MAX];
    int size;
};

void initQueue(struct PriorityQueue* q) {
    q->size = 0;
}

void swap(struct Parcel* a, struct Parcel* b) {
    struct Parcel temp = *a;
    *a = *b;
    *b = temp;
}

void enqueue(struct PriorityQueue* q, struct Parcel p) {
    if (q->size >= MAX) {
        printf("Queue full\n");
        return;
    }

    q->items[q->size] = p;
    int now = q->size;
    q->size++;

    while (now > 0 && q->items[now].priority > q->items[(now-1)/2].priority) {
        swap(&q->items[now], &q->items[(now-1)/2]);
        now = (now-1)/2;
    }
}

struct Parcel dequeue(struct PriorityQueue* q) {
    if (q->size <= 0) {
        printf("Queue empty\n");
        struct Parcel empty = {0};
        return empty;
    }

    struct Parcel top = q->items[0];
    q->size--;
    q->items[0] = q->items[q->size];

    int now = 0;
    while (1) {
        int left = 2*now + 1;
        int right = 2*now + 2;
        int max = now;

        if (left < q->size && q->items[left].priority > q->items[max].priority)
            max = left;
        if (right < q->size && q->items[right].priority > q->items[max].priority)
            max = right;

        if (max != now) {
            swap(&q->items[now], &q->items[max]);
            now = max;
        } else {
            break;
        }
    }

    return top;
}

void showAll(struct PriorityQueue* q) {
    printf("\nAll parcels in queue:\n");
    printf("ID\tDestination\tPriority\n");
    for (int i = 0; i < q->size; i++) {
        printf("%d\t%s\t\t%d\n", q->items[i].id, q->items[i].destination, q->items[i].priority);
    }
}

int main() {
    struct PriorityQueue pq;
    initQueue(&pq);

    int choice;
    do {
        printf("\n1. Add parcel\n2. Deliver next\n3. Show all parcels\n4. Exit\n");
        printf("Enter your choiche: ");
        scanf("%d", &choice);

        if (choice == 1) {
            struct Parcel p;
            printf("Enter parcel ID: ");
            scanf("%d", &p.id);
            printf("Enter destination: ");
            scanf("%s", p.destination);
            printf("Priority (1-normal, 2-urgent, 3-very urgent): ");
            scanf("%d", &p.priority);
            enqueue(&pq, p);
        } else if (choice == 2) {
            struct Parcel p = dequeue(&pq);
            if (p.id != 0) {
                printf("\nDelivering: ID %d to %s (Priority %d)\n", p.id, p.destination, p.priority);
            }
        } else if (choice == 3) {
            showAll(&pq);
        }
    } while (choice != 4);

    return 0;
}
