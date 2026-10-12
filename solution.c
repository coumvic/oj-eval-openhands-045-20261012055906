#include <stdio.h>

#define MAX_N 500000

struct ElevatorSystem {
    int current_floor;
    int direction_up;  // 1 for up, 0 for down
    int up_heap[MAX_N];    // min-heap for floors above current
    int down_heap[MAX_N];  // max-heap for floors below current
    int up_size, down_size;
};

void swap_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void init_elevator(struct ElevatorSystem *e) {
    e->current_floor = 0;
    e->direction_up = 1;
    e->up_size = 0;
    e->down_size = 0;
}

// Heap operations for up_heap (min-heap)
void push_up(struct ElevatorSystem *e, int x) {
    e->up_heap[e->up_size++] = x;
    int i = e->up_size - 1;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (e->up_heap[parent] <= e->up_heap[i]) break;
        swap_int(&e->up_heap[parent], &e->up_heap[i]);
        i = parent;
    }
}

int pop_up(struct ElevatorSystem *e) {
    if (e->up_size == 0) return -1;
    int result = e->up_heap[0];
    e->up_heap[0] = e->up_heap[--e->up_size];
    
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        
        if (left < e->up_size && e->up_heap[left] < e->up_heap[smallest]) {
            smallest = left;
        }
        if (right < e->up_size && e->up_heap[right] < e->up_heap[smallest]) {
            smallest = right;
        }
        
        if (smallest == i) break;
        swap_int(&e->up_heap[i], &e->up_heap[smallest]);
        i = smallest;
    }
    return result;
}

// Heap operations for down_heap (max-heap)
void push_down(struct ElevatorSystem *e, int x) {
    e->down_heap[e->down_size++] = x;
    int i = e->down_size - 1;
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (e->down_heap[parent] >= e->down_heap[i]) break;
        swap_int(&e->down_heap[parent], &e->down_heap[i]);
        i = parent;
    }
}

int pop_down(struct ElevatorSystem *e) {
    if (e->down_size == 0) return -1;
    int result = e->down_heap[0];
    e->down_heap[0] = e->down_heap[--e->down_size];
    
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;
        
        if (left < e->down_size && e->down_heap[left] > e->down_heap[largest]) {
            largest = left;
        }
        if (right < e->down_size && e->down_heap[right] > e->down_heap[largest]) {
            largest = right;
        }
        
        if (largest == i) break;
        swap_int(&e->down_heap[i], &e->down_heap[largest]);
        i = largest;
    }
    return result;
}

// Find and remove a specific value from heap
int remove_from_up_heap(struct ElevatorSystem *e, int target) {
    int i;
    for (i = 0; i < e->up_size; i++) {
        if (e->up_heap[i] == target) {
            e->up_heap[i] = e->up_heap[--e->up_size];
            // Re-heapify from position i
            int current = i;
            while (1) {
                int left = 2 * current + 1;
                int right = 2 * current + 2;
                int smallest = current;
                
                if (left < e->up_size && e->up_heap[left] < e->up_heap[smallest]) {
                    smallest = left;
                }
                if (right < e->up_size && e->up_heap[right] < e->up_heap[smallest]) {
                    smallest = right;
                }
                
                if (smallest == current) break;
                swap_int(&e->up_heap[current], &e->up_heap[smallest]);
                current = smallest;
            }
            return 1;
        }
    }
    return 0;
}

int remove_from_down_heap(struct ElevatorSystem *e, int target) {
    int i;
    for (i = 0; i < e->down_size; i++) {
        if (e->down_heap[i] == target) {
            e->down_heap[i] = e->down_heap[--e->down_size];
            // Re-heapify from position i
            int current = i;
            while (1) {
                int left = 2 * current + 1;
                int right = 2 * current + 2;
                int largest = current;
                
                if (left < e->down_size && e->down_heap[left] > e->down_heap[largest]) {
                    largest = left;
                }
                if (right < e->down_size && e->down_heap[right] > e->down_heap[largest]) {
                    largest = right;
                }
                
                if (largest == current) break;
                swap_int(&e->down_heap[current], &e->down_heap[largest]);
                current = largest;
            }
            return 1;
        }
    }
    return 0;
}

void add_request(struct ElevatorSystem *e, int floor) {
    if (floor > e->current_floor) {
        push_up(e, floor);
    } else {
        push_down(e, floor);
    }
}

void cancel_request(struct ElevatorSystem *e, int floor) {
    if (floor > e->current_floor) {
        remove_from_up_heap(e, floor);
    } else {
        remove_from_down_heap(e, floor);
    }
}

void execute_action(struct ElevatorSystem *e) {
    if (e->up_size == 0 && e->down_size == 0) {
        return;  // No requests, stay still
    }

    int target_floor = -1;
    
    if (e->direction_up) {
        // Try to go up first
        if (e->up_size > 0) {
            target_floor = e->up_heap[0];
        } else {
            // No requests above, reverse direction
            e->direction_up = 0;
            if (e->down_size > 0) {
                target_floor = e->down_heap[0];
            }
        }
    } else {
        // Try to go down first
        if (e->down_size > 0) {
            target_floor = e->down_heap[0];
        } else {
            // No requests below, reverse direction
            e->direction_up = 1;
            if (e->up_size > 0) {
                target_floor = e->up_heap[0];
            }
        }
    }

    if (target_floor != -1) {
        // Remove from appropriate heap before updating current_floor
        if (target_floor > e->current_floor) {
            pop_up(e);
        } else {
            pop_down(e);
        }
        
        e->current_floor = target_floor;
        
        // Reorganize remaining requests based on new current floor
        int temp_up[MAX_N], temp_down[MAX_N];
        int temp_up_size = 0, temp_down_size = 0;
        
        // Move all remaining requests to temp arrays
        while (e->up_size > 0) {
            int floor = pop_up(e);
            if (floor > e->current_floor) {
                temp_up[temp_up_size++] = floor;
            } else {
                temp_down[temp_down_size++] = floor;
            }
        }
        
        while (e->down_size > 0) {
            int floor = pop_down(e);
            if (floor > e->current_floor) {
                temp_up[temp_up_size++] = floor;
            } else {
                temp_down[temp_down_size++] = floor;
            }
        }
        
        // Rebuild heaps
        int i;
        for (i = 0; i < temp_up_size; i++) {
            push_up(e, temp_up[i]);
        }
        for (i = 0; i < temp_down_size; i++) {
            push_down(e, temp_down[i]);
        }
    }
}

int locate(struct ElevatorSystem *e) {
    return e->current_floor;
}

int main() {
    int n;
    scanf("%d", &n);
    
    struct ElevatorSystem elevator;
    init_elevator(&elevator);
    
    int i;
    for (i = 0; i < n; i++) {
        char operation[20];
        scanf("%s", operation);
        
        if (operation[0] == 'a' && operation[1] == 'c') {
            execute_action(&elevator);
        } else if (operation[0] == 'a' && operation[1] == 'd') {
            int floor;
            scanf("%d", &floor);
            add_request(&elevator, floor);
        } else if (operation[0] == 'c') {
            int floor;
            scanf("%d", &floor);
            cancel_request(&elevator, floor);
        } else if (operation[0] == 'l') {
            printf("%d\n", locate(&elevator));
        }
    }
    
    return 0;
}