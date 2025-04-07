#include "Pinky.h"

Pinky::Pinky(int x, int y) : Ghost(x, y) {
    // Constructor của Pinky, kế thừa từ lớp Ghost
    // Xác định vị trí ban đầu cho Pinky
}

void Pinky::update(SDL_Rect pacman, int pacmanDirection) {
    if (!active) return;  // Nếu Pinky không được kích hoạt thì không làm gì

    int targetX = pacman.x;
    int targetY = pacman.y;
    int targetOffset = 4 * 20;  // Khoảng cách lên trước Pac-Man (4 ô)

    // Căn cứ vào hướng di chuyển của Pac-Man, tính toán vị trí Pinky cần di chuyển
    switch (pacmanDirection) {
        case 0:  // Pac-Man di chuyển lên
            targetY -= targetOffset;  // Pinky sẽ di chuyển lên trước Pac-Man
            break;
        case 1:  // Pac-Man di chuyển xuống
            targetY += targetOffset;  // Pinky sẽ di chuyển xuống trước Pac-Man
            break;
        case 2:  // Pac-Man di chuyển trái
            targetX -= targetOffset;  // Pinky sẽ di chuyển trái trước Pac-Man
            break;
        case 3:  // Pac-Man di chuyển phải
            targetX += targetOffset;  // Pinky sẽ di chuyển phải trước Pac-Man
            break;
    }

    // Di chuyển Pinky đến vị trí tính toán
    if (rect.x < targetX) rect.x += speed;
    else if (rect.x > targetX) rect.x -= speed;

    if (rect.y < targetY) rect.y += speed;
    else if (rect.y > targetY) rect.y -= speed;
}
