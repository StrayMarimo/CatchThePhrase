#include "draw.h"

void DrawGameLayout(struct Player *player) {
    DrawRectangle(10, 10, (SCREEN_WIDTH - 30) / 2, 300, BEIGE);
    DrawRectangle((SCREEN_WIDTH - 30) / 2 + 20, 10, (SCREEN_WIDTH - 30) / 2, 300, BEIGE);
    DrawRectangle(10, 320, (SCREEN_WIDTH - 30) / 3 * 2, 200, BEIGE);
    DrawRectangle((SCREEN_WIDTH - 30) / 3 * 2 + 20, 320, (SCREEN_WIDTH - 30) / 3, 150, BEIGE);
    DrawRectangle((SCREEN_WIDTH - 30) / 3 * 2 + 20, 480, (SCREEN_WIDTH - 30) / 3, 40, BEIGE);
    DrawLine(30, 460, (SCREEN_WIDTH - 30) / 3 * 2 - 10, 460, BLACK);
    DrawLine(30, 461, (SCREEN_WIDTH - 30) / 3 * 2 - 10, 461, BLACK);

    DrawText(system_message3, 30, 380, 10, BLACK);
    DrawText(system_message2, 30, 400, 10, BLACK);
    DrawText(system_message, 30, 420, 10, MAROON);
    DrawHealthBar(player->score, 30, SCREEN_HEIGHT / 2);
    DrawHealthBar(player->opponent_score, (SCREEN_WIDTH - 30) / 2 + 40, SCREEN_HEIGHT / 2);
    DrawLettersPressed(player->letters_pressed, 50);
    DrawLettersPressed(player->opponent_letters_pressed, 520);
    DrawText(player->opponent_progress, (SCREEN_WIDTH - 30) / 2 + 50, 50, 20, DARKGRAY);
    DrawText(player->progress, 80, 50, 20, DARKGRAY);
    
}

void DrawLettersPressed(char *lettersPressed, int offset) {
    const float boxWidth = 35.0f;
    const float boxHeight = 25.0f;
    const float horizontalSpacing = 10.0f;
    const float verticalSpacing = 5.0f;
    const int charsPerRow = 9;

    for (int i = 0; i < 26; ++i) {
        int row = i / charsPerRow;
        int col = i % charsPerRow;

        float xPos = col * (boxWidth + horizontalSpacing) + offset;
        float yPos = row * (boxHeight + verticalSpacing) + 150;

        char charBuffer[2] = { lettersPressed[i], '\0' };
        char formattedText[26];
        sprintf(formattedText, "%s", charBuffer);
        DrawRectangle(xPos, yPos, boxWidth, boxHeight, LIGHTGRAY);
        DrawText(formattedText, xPos + 15, yPos + 5, 20, BLACK);
    }
}


void DrawTopic(char *topic) {
    DrawText(topic, 30, 480, 20, MAROON);
}

void DrawHealthBar(int currentHealth, int x, int y) {

    // Draw health bar background
    DrawRectangle(x, y, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT, GRAY);

    // Draw health bar
    float healthPercentage = (float)currentHealth / MAX_HEALTH;
    float currentHealthWidth = HEALTH_BAR_WIDTH * healthPercentage;

    DrawRectangle(x, y, currentHealthWidth, HEALTH_BAR_HEIGHT, GREEN);

}

void DrawInputBox(char phraseBuffer[MAX_STRING_SIZE], Rectangle textBox, bool mouseOnText, int letterCount, int framesCounter) {
    DrawRectangleRec(textBox, LIGHTGRAY);
    if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
    else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
    DrawText(phraseBuffer, 30, 440, 10, BLACK);
    if (mouseOnText) {
        if (letterCount < MAX_STRING_SIZE)
            if (((framesCounter/20)%2) == 0) 
                DrawText("_", (int)textBox.x + 7 + MeasureText(phraseBuffer, 10), (int)textBox.y, 10, MAROON);
                }
}