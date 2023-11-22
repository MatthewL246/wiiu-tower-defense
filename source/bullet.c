#include <math.h>

#include "bullet.h"
#include "drawing.h"

Bullet bullets[MAX_BULLETS];
int bulletCount = 0;

void AddBullet(Tower fromTower)
{
    if (bulletCount < MAX_BULLETS)
    {
        Bullet newBullet = *fromTower.bulletsFired;
        newBullet.position = fromTower.position;

        bullets[bulletCount] = newBullet;
        bulletCount++;
    }
}

void MoveAllBullets()
{
    for (int i = 0; i < bulletCount; i++)
    {
        Bullet* bullet = &bullets[i];

        bullet->position.x += roundf(bullet->direction.x * bullet->speed);
        bullet->position.y += roundf(bullet->direction.y * bullet->speed);

        if (bullet->position.x >= DRC_SCREEN_WIDTH || bullet->position.y >= DRC_SCREEN_HEIGHT ||
            bullet->position.x < 0 || bullet->position.y < 0)
        {
            for (int j = i; j < bulletCount - 1; j++)
            {
                bullets[j] = bullets[j + 1];
            }
            bulletCount--;
            i--;
        }
    }
}
