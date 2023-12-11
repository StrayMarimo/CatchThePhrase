#include "draw.h"

void DrawTextCenter(const char *text, int y, int font, Color color) {
    int textWidth = MeasureText(text, font);
    int x = (GetScreenWidth() - textWidth) / 2;
    DrawText(text, x, y, font, color);
}

void DrawTextCenterOfBoxWidth(const char *text, int y, int font, Color color, int boxWidth, int xOffset) {
    int textWidth = MeasureText(text, font);
    int x = (boxWidth - textWidth) / 2;
    DrawText(text, xOffset + x, y, font, color);
}


void DrawGameLayout(struct Player *player) {

    DrawRectangle(10, 10, (SCREEN_WIDTH - 30) / 2, 300, BLACK);
    DrawRectangle((SCREEN_WIDTH - 30) / 2 + 20, 10, (SCREEN_WIDTH - 30) / 2, 300, BLACK);
    DrawRectangle(10, 320, (SCREEN_WIDTH - 30) / 3 * 2, 200, BLACK);
    DrawRectangle((SCREEN_WIDTH - 30) / 3 * 2 + 20, 320, (SCREEN_WIDTH - 30) / 3, 200, BLACK);
    DrawTextCenterOfBoxWidth(player->turn, 400, 20, YELLOW, (SCREEN_WIDTH - 30) / 3, (SCREEN_WIDTH - 30) / 3 * 2 + 20);
    if (strcmp(player->turn, "") != 0)
        DrawTextCenterOfBoxWidth("TO GUESS", 430, 20, YELLOW, (SCREEN_WIDTH - 30) / 3, (SCREEN_WIDTH - 30) / 3 * 2 + 20);

    DrawTextCenterOfBoxWidth("YOU", 40, 25, YELLOW, (SCREEN_WIDTH - 30) / 2, 10);
    DrawTextCenterOfBoxWidth("OPPONENT", 40, 25, YELLOW, (SCREEN_WIDTH - 30) / 2, (SCREEN_WIDTH - 30) / 2 + 20);

    DrawText(system_message3, 30, 380, 10, WHITE);
    DrawText(system_message2, 30, 400, 10, WHITE);
    DrawText(system_message, 30, 420, 10, YELLOW);
    DrawHealthBar(player->score, 30, SCREEN_HEIGHT / 2);
    DrawHealthBar(player->opponent_score, (SCREEN_WIDTH - 30) / 2 + 40, SCREEN_HEIGHT / 2);
    DrawLettersPressed(player->letters_pressed, 80);
    DrawLettersPressed(player->opponent_letters_pressed, 550);

    DrawTextCenterOfBoxWidth(player->progress, 100, 20, WHITE, (SCREEN_WIDTH - 30) / 2, 10);
    DrawTextCenterOfBoxWidth(player->opponent_progress, 100, 20, WHITE, (SCREEN_WIDTH - 30) / 2, (SCREEN_WIDTH - 30) / 2 + 20);
    
}

void DrawLettersPressed(char *lettersPressed, int offset) {
    const float boxWidth = 20.0f;
    const float boxHeight = 20.0f;
    const float horizontalSpacing = 5.0f;
    const float verticalSpacing = 5.0f;
    const int charsPerRow = 13;

    for (int i = 0; i < 26; ++i) {
        int row = i / charsPerRow;
        int col = i % charsPerRow;

        float xPos = col * (boxWidth + horizontalSpacing) + offset;
        float yPos = row * (boxHeight + verticalSpacing) + 150;

        char charBuffer[2] = { lettersPressed[i], '\0' };
        char formattedText[26];
        sprintf(formattedText, "%s", charBuffer);

        float textX = xPos + (boxWidth - MeasureText(formattedText, 10)) / 2;
        float textY = yPos + (boxHeight - 10) / 2;

        if (lettersPressed[i] == '*')
            DrawRectangle(xPos, yPos, boxWidth, boxHeight, GRAY);
        else 
            DrawRectangle(xPos, yPos, boxWidth, boxHeight, WHITE);
        DrawText(formattedText, textX, textY, 10, BLACK);
    }
}

void DrawTopic(char *topic) {
    DrawText("TOPIC: ", 30, 480, 20, WHITE);
    DrawText(topic, 30 + MeasureText("TOPIC: ", 20), 480, 20, YELLOW);
}

void DrawHealthBar(int currentHealth, int x, int y) {

    DrawRectangle(x + 20, y - 15, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT, GRAY);

    float healthPercentage = (float)currentHealth / MAX_HEALTH;
    float currentHealthWidth = HEALTH_BAR_WIDTH * healthPercentage;

    DrawRectangle(x + 20, y - 15, currentHealthWidth, HEALTH_BAR_HEIGHT, GREEN);
    
    int textX = x + 25;
    int textY = y + (-10);
    DrawText("HP", textX, textY, 10, WHITE);
    
}

void DrawInputBox(char phraseBuffer[MAX_STRING_SIZE], Rectangle textBox, bool mouseOnText, int letterCount, int framesCounter) {
    DrawRectangleRec(textBox, LIGHTGRAY);
    if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, LIGHTGRAY);
    else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, WHITE);
    DrawText(phraseBuffer, 30, 450, 10, BLACK);
    if (mouseOnText) {
        if (letterCount < MAX_STRING_SIZE)
            if (((framesCounter/20)%2) == 0) 
                DrawText("_", (int)textBox.x + MeasureText(phraseBuffer, 10), (int)textBox.y + 10, 10, DARKGRAY);
    }
}