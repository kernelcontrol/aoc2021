#include <stdio.h>

typedef struct {
    int x;
    int y;
    int vx;
    int vy;
} Probe;

typedef struct {
    int x1;
    int x2;
    int y1;
    int y2;
} Target;

Probe createProbe(int x, int y, int vx, int vy) {
    Probe probe;

    probe.x = x;
    probe.y = y;
    probe.vx = vx;
    probe.vy = vy;

    return probe;
}

Target createTarget(int x1, int x2, int y1, int y2) {
    Target target;

    target.x1 = x1;
    target.x2 = x2;
    target.y1 = y1;
    target.y2 = y2;

    return target;
}

void moveProbe(Probe* probe) {

    // Velocity
    probe->x = probe->x + probe->vx;
    probe->y = probe->y + probe->vy;

    // Drag
    if (probe->vx > 0)
        (probe->vx)--;
    else if (probe->vx < 0)
        (probe->vx)++;

    // Gravity
    (probe->vy)--;
}

void displayProbe(Probe probe) {
    printf("x=%d y=%d | vx=%d vy=%d", probe.x, probe.y, probe.vx, probe.vy);
}

void displayTarget(Target target) {
    printf("x1=%d x2=%d | y1=%d y2=%d", target.x1, target.x2, target.y1, target.y2);
}

/**
 * 
 * Returns :
 * 0 if the probe hit the target
 * 1 if the probe missed the target
 * 2 if the probe is still moving towards the target
 * 
 * */
int detectCollision(Probe probe, Target target) {
    if (probe.x >= target.x1 && probe.x <= target.x2 && probe.y >= target.y1 && probe.y <= target.y2)
        return 0;

    if (probe.x < target.x1 && probe.vx <= 0 || probe.x > target.x2 && probe.vx >= 0 || probe.y < target.y1)
        return 1;

    return 2;
}

int main(int argc, char const *argv[])
{
    const char* statuses[] = { "HIT!", "MISS!", "Moving"};
    Probe probe = createProbe(0, 0, 6, 3);
    Target target = createTarget(20, 30, -10, -5);
    int step = 1;
    int collision = 2;

    printf("Probe : ");
    displayProbe(probe);
    printf("\nTarget : ");
    displayTarget(target);
    printf("\n--------------------------------------\n");
    
    do {
        moveProbe(&probe);
        collision = detectCollision(probe, target);
        printf("Step %d [%s] : ", step, statuses[collision]);
        displayProbe(probe);
        printf("\n");
        step++;
    } while(collision == 2); 

    return 0;
}
