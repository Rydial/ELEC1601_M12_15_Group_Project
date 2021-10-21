#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define OVERALL_WINDOW_WIDTH 640
#define OVERALL_WINDOW_HEIGHT 480

#define ROBOT_WIDTH 20
#define ROBOT_HEIGHT 20

#define WALL_WIDTH 10

#define DEFAULT_ANGLE_CHANGE 90
#define DEFAULT_SPEED_CHANGE 2
#define MAX_ROBOT_SPEED 25
#define PI 3.14159265
#define SENSOR_VISION 30


struct Wall {
    int x,y;
    int width, height;
};

struct Wall_collection {
    int key;
    struct Wall wall;
    struct Wall_collection *next;
};

typedef struct RobotMove {
    int x, y;
    int left, right;
} RM;

struct Robot {
    int x, y;
    double true_x, true_y;
    int direction;
    int angle;
    int currentSpeed;
    int width, height;
    int crashed;
    int auto_mode;

    /* First Algorithm */
    // int arr_size;
    // RM *history;

    /* Second Algorithm */
    int searchWall;
    int ridingWall;
};

#endif // STRUCTURES_H_INCLUDED
