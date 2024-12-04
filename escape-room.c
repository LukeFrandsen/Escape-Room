#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 10
#define MAX_ROOMS 5

typedef struct {
    char name[50];
    char description[200];
    int isUsable; // 1 if the item can be used, 0 otherwise
} Item;

typedef struct {
    char name[50];
    char description[200];
    Item roomItems[MAX_ITEMS];
    int itemCount;
    int isLocked; // 1 if the room is locked, 0 otherwise
    int isAccessible; // 1 if the room is accessible, 0 otherwise
} Room;

typedef struct {
    Room rooms[MAX_ROOMS];
    int currentRoom;
    Item inventory[MAX_ITEMS];
    int itemCount;
    int hasEscaped;
} Game;

void initializeGame(Game *game) {
    game->currentRoom = 0; // Start in Room 1
    game->itemCount = 0;
    game->hasEscaped = 0;

    // Room 1: Starter Room
    strcpy(game->rooms[0].name, "Room 1: Starter Room");
    strcpy(game->rooms[0].description, "A dimly lit room. A locked box is on the table.");
    game->rooms[0].itemCount = 1;
    game->rooms[0].isLocked = 0; // Always accessible
    game->rooms[0].isAccessible = 1; // Accessible at the start
    strcpy(game->rooms[0].roomItems[0].name, "Lockbox");
    strcpy(game->rooms[0].roomItems[0].description, "A small lockbox. You need to solve a riddle to open it.");
    game->rooms[0].roomItems[0].isUsable = 1;

    // Room 2: Library
    strcpy(game->rooms[1].name, "Room 2: Library");
    strcpy(game->rooms[1].description, "A quiet library filled with dusty books. A scroll lies on the desk.");
    game->rooms[1].itemCount = 1;
    game->rooms[1].isLocked = 0;
    game->rooms[1].isAccessible = 1; // Always accessible
    strcpy(game->rooms[1].roomItems[0].name, "Scroll");
    strcpy(game->rooms[1].roomItems[0].description, "An ancient scroll with a riddle.");
    game->rooms[1].roomItems[0].isUsable = 1;

    // Room 3: Exit Room
    strcpy(game->rooms[2].name, "Room 3: Exit Room");
    strcpy(game->rooms[2].description, "A long hallway with flickering lights. The exit is just ahead.");
    game->rooms[2].itemCount = 0;
    game->rooms[2].isLocked = 1; // Locked until Exit Key is found
    game->rooms[2].isAccessible = 0; // Locked until the Exit Key is found
}

void addItem(Game *game, Item item) {
    if (game->itemCount < MAX_ITEMS) {
        game->inventory[game->itemCount++] = item;
        printf("You picked up %s.\n", item.name);
    } else {
        printf("Inventory is full!\n");
    }
}

int hasItem(Game *game, const char *itemName) {
    for (int i = 0; i < game->itemCount; i++) {
        if (strcmp(game->inventory[i].name, itemName) == 0) {
            return 1;
        }
    }
    return 0;
}

void showInventory(Game *game) {
    printf("\nYour Inventory:\n");
    if (game->itemCount == 0) {
        printf(" - Your inventory is empty.\n");
        return;
    }
    for (int i = 0; i < game->itemCount; i++) {
        printf("- %s: %s\n", game->inventory[i].name, game->inventory[i].description);
    }
}

void showRoom(Game *game) {
    Room *room = &game->rooms[game->currentRoom];
    printf("\nYou are in %s\n", room->name);
    printf("%s\n", room->description);
    if (room->itemCount > 0) {
        printf("You see:\n");
        for (int i = 0; i < room->itemCount; i++) {
            printf("- %s\n", room->roomItems[i].name);
        }
    }
}

void useItem(Game *game, char *itemName) {
    for (int i = 0; i < game->itemCount; i++) {
        if (strcmp(game->inventory[i].name, itemName) == 0) {
            if (strcmp(itemName, "Scroll") == 0) {
                printf("\nYou read the scroll. It says:\n");
                printf("\"I speak without a mouth and hear without ears. I have no body, but I come alive with wind. What am I?\"\n");
            } else if (strcmp(itemName, "Lockbox") == 0) {
                printf("To open the lockbox, solve the riddle: \"I speak without a mouth and hear without ears. I have no body, but I come alive with wind. What am I?\"\n");
                printf("Enter your answer: ");
                char answer[50];
                scanf(" %[^\n]%*c", answer);

                if (strcasecmp(answer, "Echo") == 0) {
                    printf("Correct! You open the lockbox and find a key inside.\n");
                    Item key = {"Exit Key", "The key to unlock the exit door.", 1};
                    addItem(game, key);
                    game->rooms[2].isLocked = 0; // Unlock Room 3
                    game->rooms[2].isAccessible = 1;
                } else {
                    printf("Incorrect answer. The lockbox remains closed.\n");
                }
            }
            return;
        }
    }
    printf("You don't have that item in your inventory.\n");
}

void changeRoom(Game *game, int newRoom) {
    if (newRoom < 1 || newRoom > 3 || strlen(game->rooms[newRoom - 1].name) == 0) {
        printf("Invalid room number. Please enter a valid room number (1, 2, or 3).\n");
    } else if (!game->rooms[newRoom - 1].isAccessible) {
        printf("The door to %s is locked. You need the Exit Key to proceed.\n", game->rooms[newRoom - 1].name);
    } else {
        game->currentRoom = newRoom - 1;
        printf("You move to %s.\n", game->rooms[game->currentRoom].name);
        showRoom(game);
        if (newRoom == 3) {
            printf("Congratulations! You have escaped the room and won the game!\n");
            game->hasEscaped = 1;
        }
    }
}

void gameLoop(Game *game) {
    int choice;
    char itemName[50];

    while (!game->hasEscaped) {
        printf("\nYou are currently in %s.\n", game->rooms[game->currentRoom].name);
        printf("\nWhat would you like to do?\n");
        printf("1. Look around\n");
        printf("2. Pick up an item\n");
        printf("3. Check inventory\n");
        printf("4. Use an item\n");
        printf("5. Move to another room\n");
        printf("6. Quit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 6.\n");
            while (getchar() != '\n'); // Clear invalid input
            continue;
        }

        switch (choice) {
            case 1:
                showRoom(game);
                break;
            case 2:
                if (game->rooms[game->currentRoom].itemCount > 0) {
                    addItem(game, game->rooms[game->currentRoom].roomItems[--game->rooms[game->currentRoom].itemCount]);
                } else {
                    printf("There's nothing to pick up here.\n");
                }
                break;
            case 3:
                showInventory(game);
                break;
            case 4:
                printf("Which item would you like to use? ");
                scanf(" %[^\n]%*c", itemName);
                useItem(game, itemName);
                break;
            case 5:
                printf("Which room would you like to go to? (1, 2, or 3): ");
                int newRoom;
                if (scanf("%d", &newRoom) != 1) {
                    printf("Invalid input. Please enter a valid room number (1, 2, or 3).\n");
                    while (getchar() != '\n'); // Clear invalid input
                    continue;
                }
                changeRoom(game, newRoom);
                break;
            case 6:
                printf("Goodbye!\n");
                return;
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }
    }
}

int main() {
    Game game;
    initializeGame(&game);

    printf("Welcome to the Escape Room!\n");
    showRoom(&game);

    gameLoop(&game);

    return 0;
}
