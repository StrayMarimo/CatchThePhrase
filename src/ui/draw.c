
#include "draw.h"

void DrawGameLayout() {
    DrawRectangle(10, 10, (SCREEN_WIDTH - 30) / 2, 300, BEIGE);
    DrawRectangle((SCREEN_WIDTH - 30) / 2 + 20, 10, (SCREEN_WIDTH - 30) / 2, 300, BEIGE);
    DrawRectangle(10, 320, (SCREEN_WIDTH - 30) / 3 * 2, 200, BEIGE);
    DrawRectangle((SCREEN_WIDTH - 30) / 3 * 2 + 20, 320, (SCREEN_WIDTH - 30) / 3, 150, BEIGE);
    DrawRectangle((SCREEN_WIDTH - 30) / 3 * 2 + 20, 480, (SCREEN_WIDTH - 30) / 3, 40, BEIGE);
    DrawLine(30, 460, (SCREEN_WIDTH - 30) / 3 * 2 - 10, 460, BLACK);
    DrawLine(30, 461, (SCREEN_WIDTH - 30) / 3 * 2 - 10, 461, BLACK);
}

// void DrawLettersPressed(char *lettersPressed) {
//     const float boxWidth = 40.0f;
//     const float boxHeight = 40.0f;
//     const float horizontalSpacing = 20.0f;
//     const float verticalSpacing = 20.0f;
//     const int charsPerRow = 9;

//     // Draw array in a box with 3 rows
//     for (int i = 0; i < 26; ++i) {
//         int row = i / charsPerRow;
//         int col = i % charsPerRow;

//         float xPos = col * (boxWidth + horizontalSpacing) + 50;
//         float yPos = row * (boxHeight + verticalSpacing) + 50;

//         DrawRectangle(xPos, yPos, boxWidth, boxHeight, LIGHTGRAY);
//         DrawText(&lettersPressed[i], (int)(xPos + 0.5 * (boxWidth - MeasureText(&lettersPressed[i], 20))), (int)(yPos + 0.5 * (boxHeight - 20)), 20, BLACK);
//     }
// }

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