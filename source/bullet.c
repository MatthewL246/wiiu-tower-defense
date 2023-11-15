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

        // Calculate the direction vector from the bullet to the target
        Point direction;
        direction.x = fromTower.targetPosition.x - newBullet.position.x;
        direction.y = fromTower.targetPosition.y - newBullet.position.y;

        // Normalize the direction vector to get a unit vector
        float length = sqrt(direction.x * direction.x + direction.y * direction.y);
        float directionX = direction.x / length;
        float directionY = direction.y / length;

        // Multiply the unit direction vector by the bullet's speed to get the velocity vector
        Point velocity;
        velocity.x = directionX * newBullet.speed;
        velocity.y = directionY * newBullet.speed;

        newBullet.velocity = velocity;

        bullets[bulletCount] = newBullet;
        bulletCount++;
    }
}

void MoveAllBullets()
{
    for (int i = 0; i < bulletCount; i++)
    {
        // Move the bullet
        bullets[i].position.x += bullets[i].velocity.x;
        bullets[i].position.y += bullets[i].velocity.y;

        if (bullets[i].position.x >= DRC_SCREEN_WIDTH || bullets[i].position.y >= DRC_SCREEN_HEIGHT ||
            bullets[i].position.x < 0 || bullets[i].position.y < 0)
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
