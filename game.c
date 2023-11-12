typedef struct
{
    int x;
    int y;
} Point;

typedef struct {
    char r;
    char g;
    char b;
} Color;

typedef struct {
    Point position;
    Point velocity;
    int size;
    int speed;
    int damage;
    int health;
} Bullet;

typedef struct {
    Point position;
    Point targetPosition;
    Color color;
    int size;
    Bullet bulletsFired;
    int fireRate;
} Tower;

typedef struct {
    Point position;
    Color color;
    int size;
    int speed;
    int health;
} Enemy;

const int DRC_TOUCH_TOP = 3900;
const int DRC_TOUCH_BOTTOM = 180;
const int DRC_TOUCH_LEFT = 100;
const int DRC_TOUCH_RIGHT = 3960;

const int DRC_SCREEN_WIDTH = 854;
const int DRC_SCREEN_HEIGHT = 480;

Point MapTouchToDrcScreen(int touchX, int touchY)
{
    Point screenPoint;
    screenPoint.x = ((touchX - DRC_TOUCH_LEFT) / (float)(DRC_TOUCH_RIGHT - DRC_TOUCH_LEFT)) * DRC_SCREEN_WIDTH;
    screenPoint.y = DRC_SCREEN_HEIGHT - (((touchY - DRC_TOUCH_BOTTOM) / (float)(DRC_TOUCH_TOP - DRC_TOUCH_BOTTOM)) * DRC_SCREEN_HEIGHT);

    return screenPoint;
}

const int TV_WIDTH = 1280;
const int TV_HEIGHT = 720;

void DrawPoint(Point point, Color color, int size, bool drawOnBothScreens)
{
    int halfSize = size / 2;
    for (int dx = -halfSize; dx <= halfSize; dx++)
    {
        for (int dy = -halfSize; dy <= halfSize; dy++)
        {
            if (sqrt(dx * dx + dy * dy) <= halfSize) {
                OSScreenPutPixelEx(SCREEN_DRC, point.x + dx, point.y + dy, color.r << 24 | color.g << 16 | color.b << 8);
                if (drawOnBothScreens)
                {
                    float tvScaleX = (float)TV_WIDTH / DRC_SCREEN_WIDTH;
                    float tvScaleY = (float)TV_HEIGHT / DRC_SCREEN_HEIGHT;
                    int tvX = (point.x + dx) * tvScaleX;
                    int tvY = (point.y + dy) * tvScaleY;
                    for (int tvDx = -1; tvDx <= 1; tvDx++)
                    {
                        for (int tvDy = -1; tvDy <= 1; tvDy++)
                        {
                            OSScreenPutPixelEx(SCREEN_TV, tvX + tvDx, tvY + tvDy, color.r << 24 | color.g << 16 | color.b << 8);
                        }
                    }
                }
            }
        }
    }
}

#define MAX_TOWERS 100
Tower towers[MAX_TOWERS];
int numTowers = 0;

void AddTower(Point position)
{
    if (numTowers < MAX_TOWERS)
    {
        Tower newTower;
        newTower.position = position;
        newTower.targetPosition = (Point){-20, -20};
        newTower.color = (Color){0, 255, 0};
        newTower.size = 10;
        newTower.bulletsFired = (Bullet){(Point){0, 0}, (Point){0, 0}, 5, 5, 1, 1};
        newTower.fireRate = 5;

        towers[numTowers] = newTower;
        numTowers++;
    }
}

void SetTowerTarget(int towerIndex, Point targetPosition)
{
    towers[towerIndex].targetPosition = targetPosition;
}

#define MAX_BULLETS 1000
Bullet bullets[MAX_BULLETS];
int numBullets = 0;

void AddBullet(Tower fromTower)
{
    if (numBullets < MAX_BULLETS)
    {
        Bullet newBullet = fromTower.bulletsFired;
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

        bullets[numBullets] = newBullet;
        numBullets++;
    }
}

void MoveAllBullets()
{
    for (int i = 0; i < numBullets; i++)
    {
        // Move the bullet
        bullets[i].position.x += bullets[i].velocity.x;
        bullets[i].position.y += bullets[i].velocity.y;

        if (bullets[i].position.x >= DRC_SCREEN_WIDTH || bullets[i].position.y >= DRC_SCREEN_HEIGHT ||
            bullets[i].position.x < 0 || bullets[i].position.y < 0)
        {
            for (int j = i; j < numBullets - 1; j++)
            {
                bullets[j] = bullets[j + 1];
            }
            numBullets--;
            i--;
        }
    }
}
void FireAllTowers(unsigned gameLoopCounter)
{
    for (int i = 0; i < numTowers; i++)
    {
        if (towers[i].targetPosition.x != -20 && towers[i].targetPosition.y != -20)
        {
            if (gameLoopCounter % (100 / towers[i].fireRate) == 0)
            {
                AddBullet(towers[i]);
            }
        }
    }
}

#define MAX_ENEMIES 200
Enemy enemies[MAX_ENEMIES];
int numEnemies = 0;

void AddEnemy()
{
    if (numEnemies < MAX_ENEMIES)
    {
        Enemy newEnemy;
        newEnemy.position = (Point){0, DRC_SCREEN_HEIGHT / 2};
        newEnemy.color = (Color){255, 0, 0};
        newEnemy.size = 20;
        newEnemy.speed = 1;
        newEnemy.health = 1;

        enemies[numEnemies] = newEnemy;
        numEnemies++;
    }
}

void MoveAllEnemies()
{
    for (int i = 0; i < numEnemies; i++)
    {
        enemies[i].position.x += enemies[i].speed;

        if (enemies[i].position.x >= DRC_SCREEN_WIDTH)
        {
            for (int j = i; j < numEnemies - 1; j++)
            {
                enemies[j] = enemies[j + 1];
            }
            numEnemies--;
            i--;
        }
    }
}

void DrawAllTowers()
{
    for (int i = 0; i < numTowers; i++)
    {
        DrawPoint(towers[i].position, towers[i].color, towers[i].size, true);
        DrawPoint(towers[i].targetPosition, (Color){255, 155, 0}, 10, true);
    }
}

void DrawAllBullets()
{
    for (int i = 0; i < numBullets; i++)
    {
        DrawPoint(bullets[i].position, (Color){150, 150, 150}, bullets[i].size, true);
    }
}

void DrawAllEnemies()
{
    for (int i = 0; i < numEnemies; i++)
    {
        DrawPoint(enemies[i].position, enemies[i].color, enemies[i].size, true);
    }
}

void CheckBulletEnemyCollisions()
{
    for (int i = 0; i < numBullets; i++)
    {
        for (int j = 0; j < numEnemies; j++)
        {
            float dx = bullets[i].position.x - enemies[j].position.x;
            float dy = bullets[i].position.y - enemies[j].position.y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance <= bullets[i].size + enemies[j].size)
            {
                bullets[i].health -= 1;
                enemies[j].health -= bullets[i].damage;

                if (bullets[i].health <= 0)
                {
                    for (int k = i; k < numBullets - 1; k++)
                    {
                        bullets[k] = bullets[k + 1];
                    }
                    numBullets--;
                    i--;
                }
                if (enemies[j].health <= 0)
                {
                    for (int k = j; k < numEnemies - 1; k++)
                    {
                        enemies[k] = enemies[k + 1];
                    }
                    numEnemies--;
                    j--;
                }
            }
        }
    }
}

unsigned gameLoopCounter = 0;
unsigned touchTimer = 0;
const int touchDelay = 1;
bool targetMode = false;

int GameLoop(VPADStatus status)
{
    VPADTouchData touchData = status.tpNormal;
    if (touchData.touched != 0)
    {
        Point screenTouchPoint = MapTouchToDrcScreen(touchData.x, touchData.y);
        DrawPoint(screenTouchPoint, (Color){255, 255, 255}, 10, false);
        touchTimer++;
        if (touchTimer == touchDelay && !targetMode)
        {
            AddTower(screenTouchPoint);
            targetMode = true;
        }
        else if (touchTimer == touchDelay && targetMode)
        {
            SetTowerTarget(numTowers - 1, screenTouchPoint);
            targetMode = false;
        }
    }
    else
    {
        touchTimer = 0;
    }

    if (gameLoopCounter % 100 == 0)
    {
        AddEnemy();
    }

    FireAllTowers(gameLoopCounter);

    MoveAllEnemies();
    MoveAllBullets();
    CheckBulletEnemyCollisions();

    DrawAllTowers();
    DrawAllBullets();
    DrawAllEnemies();

    gameLoopCounter++;
    return 0;
}
