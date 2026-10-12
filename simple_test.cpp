#include <cstdio>

int main() {
    int n;
    scanf("%d", &n);
    
    int current_floor = 0;
    int direction_up = 1;  // 1 for up, 0 for down
    
    for (int i = 0; i < n; i++) {
        char operation[20];
        scanf("%s", operation);
        
        if (operation[0] == 'l') {
            printf("%d\n", current_floor);
        }
        // Simple placeholder for other operations
    }
    
    return 0;
}