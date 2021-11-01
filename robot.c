#include "robot.h"

void setup_robot(struct Robot *robot){
    robot->x = OVERALL_WINDOW_WIDTH/2-10;
    robot->y = OVERALL_WINDOW_HEIGHT/2-10;
    robot->true_x = OVERALL_WINDOW_WIDTH/2-10;
    robot->true_y = OVERALL_WINDOW_HEIGHT/2-10;
    robot->width = ROBOT_WIDTH;
    robot->height = ROBOT_HEIGHT;
    robot->direction = 0;
    robot->angle = 90;
    robot->currentSpeed = 0;
    robot->crashed = 0;
    robot->auto_mode = 0;

    /* First Algorithm */
    // robot->history = malloc(0);
    // robot->arr_size = 0;

    /* Second Algorithm */
    robot->searchWall = 0;
    robot->ridingWall = 0;

    printf("Press arrow keys to move manually, or enter to move automatically\n\n");
}
int robot_off_screen(struct Robot * robot){
    if(robot->x < 0 || robot-> y < 0){
        return 0;
    }
    if(robot->x > OVERALL_WINDOW_WIDTH || robot->y > OVERALL_WINDOW_HEIGHT){
        return 0;
    }
    return 1;
}

int checkRobotHitWall(struct Robot * robot, struct Wall * wall) {

    int overlap = checkOverlap(robot->x,robot->width,robot->y,robot->height,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotHitWalls(struct Robot * robot, struct Wall_collection * head) {
   struct Wall_collection *ptr = head;
   int hit = 0;

   while(ptr != NULL) {
      hit = (hit || checkRobotHitWall(robot, &ptr->wall));
      ptr = ptr->next;
   }
   return hit;

}

int checkRobotReachedEnd(struct Robot * robot, int x, int y, int width, int height){

    int overlap = checkOverlap(robot->x,robot->width,robot->y,robot->height,
                 x,width,y,height);

    return overlap;
}

void robotCrash(struct Robot * robot) {
    robot->currentSpeed = 0;
    robot->direction = 0;
    if (!robot->crashed)
        printf("Ouchies!!!!!\n\nPress space to start again\n");
    robot->crashed = 1;
}

void robotSuccess(struct Robot * robot, int msec) {
    robot->currentSpeed = 0;
    if (!robot->crashed){
        printf("Success!!!!!\n\n");
        printf("Time taken %d seconds %d milliseconds \n", msec/1000, msec%1000);
        printf("Press space to start again\n");
    }
    robot->crashed = 1;
}

int checkRobotSensor(int x, int y, int width, int height, struct Wall * wall)  {
    //viewing_region of sensor is a square of 2 pixels * chosen length of sensitivity
    int overlap = checkOverlap(x,2,y,2,
                 wall->x,wall->width,wall->y, wall->height);

    return overlap;
}

int checkRobotSensorFrontRightAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;

    int sensorSensitivityLength =  floor(SENSOR_VISION/5);

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+cos((robot->angle+45)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle+45)*PI/180));
        yDir = round(robotCentreY-2+sin((robot->angle+45)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle+45)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            if (robot->angle == 90 || robot->angle == 270)
                hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, 2, &ptr->wall));
            else
                hit = (hit || checkRobotSensor(xTL, yTL, 2, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}


int checkRobotSensorFrontLeftAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int sensorSensitivityLength;

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;
    sensorSensitivityLength =  floor(SENSOR_VISION/5);

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+cos((robot->angle-45)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle-45)*PI/180));
        yDir = round(robotCentreY-2+sin((robot->angle-45)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle-45)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            if (robot->angle == 90 || robot->angle == 270)
                hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, 2, &ptr->wall));
            else
                hit = (hit || checkRobotSensor(xTL, yTL, 2, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorRightAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int sensorSensitivityLength;

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;
    sensorSensitivityLength =  floor(SENSOR_VISION/5);

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+cos((robot->angle+90)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle+90)*PI/180));
        yDir = round(robotCentreY+sin((robot->angle+90)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle+90)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            if (robot->angle == 90 || robot->angle == 270)
                hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, 2, &ptr->wall));
            else
                hit = (hit || checkRobotSensor(xTL, yTL, 2, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

int checkRobotSensorFrontMidAllWalls(struct Robot * robot, struct Wall_collection * head) {
    struct Wall_collection *ptr, *head_store;
    int i;
    double xDir, yDir;
    int robotCentreX, robotCentreY, xTL, yTL;
    int score, hit;
    int sensorSensitivityLength;

    head_store = head;
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;
    score = 0;
    sensorSensitivityLength =  floor(SENSOR_VISION/5);

    for (i = 0; i < 5; i++)
    {
        ptr = head_store;
        xDir = round(robotCentreX+cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;
        hit = 0;

        while(ptr != NULL) {
            if (robot->angle == 90 || robot->angle == 270)
                hit = (hit || checkRobotSensor(xTL, yTL, sensorSensitivityLength, 2, &ptr->wall));
            else
                hit = (hit || checkRobotSensor(xTL, yTL, 2, sensorSensitivityLength, &ptr->wall));
            ptr = ptr->next;
        }
        if (hit)
            score = i;
    }
    return score;
}

void robotUpdate(struct SDL_Renderer * renderer, struct Robot * robot){
    double xDir, yDir;

    int robotCentreX, robotCentreY, xTR, yTR, xTL, yTL, xBR, yBR, xBL, yBL;
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    /*
    //Other Display options:
    // The actual square which the robot is tested against (not so nice visually with turns, but easier
    // to test overlap
    SDL_Rect rect = {robot->x, robot->y, robot->height, robot->width};
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
    */
    /*
    //Center Line of Robot. Line shows the direction robot is facing
    xDir = -30 * sin(-robot->angle*PI/180);
    yDir = -30 * cos(-robot->angle*PI/180);
    xDirInt = robot->x+ROBOT_WIDTH/2+ (int) xDir;
    yDirInt = robot->y+ROBOT_HEIGHT/2+ (int) yDir;
    SDL_RenderDrawLine(renderer,robot->x+ROBOT_WIDTH/2, robot->y+ROBOT_HEIGHT/2, xDirInt, yDirInt);
    */

    //Rotating Square
    //Vector rotation to work out corners x2 = x1cos(angle)-y1sin(angle), y2 = x1sin(angle)+y1cos(angle)
    robotCentreX = robot->x+ROBOT_WIDTH/2;
    robotCentreY = robot->y+ROBOT_HEIGHT/2;

    xDir = round(robotCentreX+(ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xTR = (int) xDir;
    yTR = (int) yDir;

    xDir = round(robotCentreX+(ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xBR = (int) xDir;
    yBR = (int) yDir;

    xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xBL = (int) xDir;
    yBL = (int) yDir;

    xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2)*sin((robot->angle)*PI/180));
    yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2)*cos((robot->angle)*PI/180));
    xTL = (int) xDir;
    yTL = (int) yDir;

    SDL_RenderDrawLine(renderer,xTR, yTR, xBR, yBR);
    SDL_RenderDrawLine(renderer,xBR, yBR, xBL, yBL);
    SDL_RenderDrawLine(renderer,xBL, yBL, xTL, yTL);
    SDL_RenderDrawLine(renderer,xTL, yTL, xTR, yTR);


    //Front Right Sensor
    int sensorSensitivityLength =  floor(SENSOR_VISION/5);
    int i;

    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+cos((robot->angle+45)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle+45)*PI/180));
        yDir = round(robotCentreY-2+sin((robot->angle+45)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle+45)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensorSensitivityLength};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }



    //Front Left Sensor
    for (i = 0; i < 5; i++)
    {
        xDir = round(robotCentreX+cos((robot->angle-45)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle-45)*PI/180));
        yDir = round(robotCentreY-2+sin((robot->angle-45)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle-45)*PI/180));
        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, 2, sensorSensitivityLength};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }


    //Right Mid Sensor
    for (i = 0; i < 5; i++)
    {
        // xDir = round(robotCentreX+(-ROBOT_WIDTH/2)*cos((robot->angle+90)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*(i+1))*sin((robot->angle+90)*PI/180));
        // yDir = round(robotCentreY+(-ROBOT_WIDTH/2)*sin((robot->angle+90)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*(i+1))*cos((robot->angle+90)*PI/180));
        xDir = round(robotCentreX+cos((robot->angle+90)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle+90)*PI/180));
        yDir = round(robotCentreY+sin((robot->angle+90)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle+90)*PI/180));

        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, sensorSensitivityLength, 2};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }


    // Front Mid
    for (i = 0; i < 5; i++)
    {
        // xDir = round(robotCentreX+(ROBOT_WIDTH/2-2)*cos((robot->angle+90)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*(i+1))*sin((robot->angle+90)*PI/180));
        // yDir = round(robotCentreY+(ROBOT_WIDTH/2-2)*sin((robot->angle+90)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*(i+1))*cos((robot->angle+90)*PI/180));
        xDir = round(robotCentreX+cos((robot->angle)*PI/180)-(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*sin((robot->angle)*PI/180));
        yDir = round(robotCentreY+sin((robot->angle)*PI/180)+(-ROBOT_HEIGHT/2-SENSOR_VISION+sensorSensitivityLength*i)*cos((robot->angle)*PI/180));

        xTL = (int) xDir;
        yTL = (int) yDir;

        SDL_Rect rect = {xTL, yTL, sensorSensitivityLength, 2};
        SDL_SetRenderDrawColor(renderer, 80+(20*(5-i)), 80+(20*(5-i)), 80+(20*(5-i)), 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect);
    }

}

void robotMotorMove(struct Robot * robot) {
    double x_offset, y_offset;
    switch(robot->direction){
        case UP :
            robot->currentSpeed += DEFAULT_SPEED_CHANGE;
            if (robot->currentSpeed > MAX_ROBOT_SPEED)
                robot->currentSpeed = MAX_ROBOT_SPEED;
            break;
        case DOWN :
            robot->currentSpeed -= DEFAULT_SPEED_CHANGE;
            if (robot->currentSpeed < -MAX_ROBOT_SPEED)
                robot->currentSpeed = -MAX_ROBOT_SPEED;
            break;
        case LEFT :
            robot->angle = (robot->angle+360-DEFAULT_ANGLE_CHANGE)%360;
            break;
        case RIGHT :
            robot->angle = (robot->angle+DEFAULT_ANGLE_CHANGE)%360;
            break;
    }
    robot->direction = 0;
    x_offset = (-robot->currentSpeed * sin(-robot->angle*PI/180));
    y_offset = (-robot->currentSpeed * cos(-robot->angle*PI/180));

    robot->true_x += x_offset;
    robot->true_y += y_offset;

    x_offset = round(robot->true_x);
    y_offset = round(robot->true_y);

    robot->x = (int) x_offset;
    robot->y = (int) y_offset;

}



// void robotAutoMotorMove(struct Robot * robot, int front_left_sensor, int front_right_sensor) {

//     if ((front_left_sensor == 0) && (front_right_sensor == 0)) {
//         if (robot->currentSpeed<4){
//             robot->direction = UP;
//         }
//     } else if ((robot->currentSpeed>0) && ((front_left_sensor == 1) || (front_right_sensor == 1))) {
//         robot->direction = DOWN;
//     } else if ((robot->currentSpeed == 0) && ((front_left_sensor == 1) || (front_right_sensor == 1))) {

//         int exists = 0;
//         /* Check if current position exists in history */
//         for (int i = 0; i < robot->arr_size; i++) {
//             if (robot->history[i].x == robot->x && robot->history[i].y == robot->y) {
//                 robot->history[i].right = 1;
//                 robot->direction = RIGHT;
//                 exists = 1;
//                 break;
//             }
//         }
//         /* Add a new position if it doesn't exist in history */
//         if (!exists) {
//             robot->history = realloc(robot->history, sizeof(RM) * ++robot->arr_size);
//             robot->history[robot->arr_size - 1].x = robot->x;
//             robot->history[robot->arr_size - 1].y = robot->y;
//             robot->history[robot->arr_size - 1].left = 1;
//             robot->history[robot->arr_size - 1].right = 0;
//             robot->direction = LEFT;
//         }
//         /* Print */
//         printf("\nCurrent Array Size: %d\n", robot->arr_size);

//         for (int i = 0; i < robot->arr_size; i++) {
//             printf("Move %d: X is %d, Y is %d\n", i, robot->history[i].x, robot->history[i].y);
//             printf("Left: %s      Right: %s\n",
//                 robot->history[i].left ? "True" : "False", robot->history[i].right ? "True" : "False");
//         }

//     } else if ((robot->currentSpeed==0) && ((front_left_sensor == 1) || (front_right_sensor == 0)))
//         robot->direction = RIGHT;
//     else if ((robot->currentSpeed==0) && ((front_left_sensor == 0) || (front_right_sensor == 1)))
//         robot->direction = RIGHT;

// }

void robotAutoMotorMove(struct Robot * robot, int front_left_sensor, int front_right_sensor, int right_sensor, int front_mid_sensor) {

    printf("Front_Left: %d   Front_right: %d    Right: %d   Mid: %d\n", front_left_sensor, front_right_sensor, right_sensor, front_mid_sensor);
    printf("Current speed: %d       Riding: %d \n", robot->currentSpeed, robot->ridingWall);


    // initial search algorithm
    if (robot->ridingWall == 0) {
        if (robot->searchWall == 0) {
            robot->direction = RIGHT;
            robot->searchWall = 1;
        } else {
            if (robot->currentSpeed < 4)
                robot->direction = UP;
        }
    }
    // initial wall found on left or front
    if (robot->ridingWall == 0 && (front_left_sensor || front_mid_sensor))
        robot->direction = LEFT;
    // initial wall found on right
    if (front_right_sensor || right_sensor)
        robot->ridingWall = 1;


    // tracking right sensor
    if (robot->ridingWall) {
        if (right_sensor < 3 && front_right_sensor < 3) {
            if (robot->currentSpeed < 2)
                robot->direction = UP;
            else
                robot->direction = RIGHT;
        }
        if (right_sensor >= 3) {
            if (front_left_sensor < 3 && front_right_sensor < 4 && front_mid_sensor < 3) {
                if (robot->currentSpeed < 2)
                    robot->direction = UP;
            }
        }
    }

    // during navigation: other sensors maintain safety distance of 3 (except right sensor), emergency distance of 4
    if (front_left_sensor == 3 || front_right_sensor == 3 || front_mid_sensor == 3) {
        if (robot->currentSpeed < 2)
            robot->direction = UP;
        if (robot->currentSpeed > 2)
            robot->direction = DOWN;
    }

    if (front_left_sensor == 4 && front_right_sensor < 4)
        robot->direction = RIGHT;

    if (front_right_sensor == 3 && front_left_sensor < 1)
        robot->direction = LEFT;
    //if (right_sensor == 4 && front_right_sensor < 2 && front_left_sensor < 2) {
        //if (robot->currentSpeed > 0)
            //robot->direction = RIGHT;
    //}
    if (front_right_sensor == 4 || front_mid_sensor == 4)
        robot->direction = LEFT;

    if (front_left_sensor == 4 || front_right_sensor == 4 || front_mid_sensor == 4)
        if (robot->currentSpeed > 0)
            robot->direction = DOWN;


    printf("Direction: %d\n\n", robot->direction);



    /*

    /////////////////////////////////////////////////////////////////////////////////////////
    if (front_right_sensor < 3) {
        if (robot->ridingWall == 0) {
            if (robot->currentSpeed < 4)
                robot->direction = UP;
            else
                robot->direction = RIGHT;
        }

        if (robot->ridingWall == 1 && right_sensor == 0) {
                if (robot->currentSpeed > 2)
                    robot->direction = DOWN;
                else
                    robot->direction = RIGHT;
            }
    }

    if (right_sensor < 3 && robot->ridingWall == 1)
        robot->direction = RIGHT;

    if (right_sensor == 3 || front_right_sensor > 1) {
        if (right_sensor == 3)
            robot->ridingWall = 1;

        if (front_left_sensor == 0) {
            if (robot->currentSpeed < 4)
                robot->direction = UP;
        } else if (front_left_sensor > 2 && right_sensor < 1) { // front left sensor > 2
            if (robot->currentSpeed > 0)
                robot->direction = DOWN;
            else
                robot->direction = LEFT;
        } else {// front left sensor == 2
            if (robot->currentSpeed < 4)
                robot->direction = UP;
        }
    }
    if (front_right_sensor > 2) {
        robot->currentSpeed = 2;
        robot->direction = LEFT;

    }
    if (front_left_sensor > 1 && front_right_sensor > 1){
        if (front_left_sensor != 4 && front_right_sensor != 4)
            robot->currentSpeed = 2;
    }

    if (front_right_sensor == 4 || right_sensor == 4) {
        robot->currentSpeed = 0;
        robot->direction = LEFT;
    }

    if (front_left_sensor == 4){
        if (robot->currentSpeed > 0)
            robot->direction = DOWN;
        else
            robot->direction = RIGHT;
    }

    if (front_mid_sensor == 4) {
        robot->currentSpeed = 0;
        robot->direction = LEFT;
    }


    /*
    // turn on riding wall: right top or bottom sensor detects wall
    if (right_btm_sensor >= 3)
        robot->ridingWall = 1;
        robot->searchWall = 0;

    //turn on searching wall implementations
    if (right_top_sensor < 3 && right_btm_sensor < 3)
        robot->searchWall = 1;

    // initial search 1 0
    if (robot->searchWall == 1 && robot->ridingWall == 0) {
        if (robot->currentSpeed < 4)
            robot->direction = UP;
        else
            robot->direction = RIGHT;
    }

    // tracking search 1 1
    if (robot->searchWall == 1 && robot->ridingWall == 1) {
        if (right_top_sensor < 2)
            robot->direction = RIGHT;
        else
            robot->searchWall = 0;
    }
    // riding 0 1
    if (robot->searchWall == 0 && robot->ridingWall == 1) {
        // nothing in front
        if (front_left_sensor <= 1 && right_top_sensor >= 2){
            if (robot->currentSpeed < 4)
                robot->direction = UP;
        } else if (front_left_sensor >= 3 && right_top_sensor >= 2) { // left corner
            if (robot->currentSpeed > 0)
                robot->direction = DOWN;
            else
                robot->direction = LEFT;
        } else if (front_left_sensor < 2 && right_top_sensor == 0) {
            if (robot->currentSpeed > 0)
                robot->direction = DOWN;
            robot->searchWall = 1;
            robot->direction = RIGHT;
        }

    }

    if (right_top_sensor == 4)
        robot->direction = LEFT;
    if (front_left_sensor == 4)
        robot->direction = RIGHT;

    if (right_btm_sensor >= 3) {
        robot->ridingWall = 1;
        robot->searchWall = 0;
    }
    else if (right_btm_sensor < 3)
        robot->ridingWall = 0;
        if (front_left_sensor < 3 && right_top_sensor < 3)
            robot->searchWall = 1;

    if (robot->searchWall) {
        if (robot->currentSpeed < 4)
            robot->direction = UP;
        else
            robot->direction = RIGHT;
    }

    //no wall in sight -> turn right and search for wall
    if (front_left_sensor < 3 && right_top_sensor < 3 && !robot->ridingWall) {
        if (!robot->searchWall) {
            robot->searchWall = 1;
            robot->direction = RIGHT;
        } else if (robot->currentSpeed < 4)
            robot->direction = UP;
    }

    else if (front_left_sensor >= 3 || right_top_sensor >= 3) {
        if (robot->currentSpeed == 0) {
            robot->direction = LEFT;
        } else
            robot->direction = DOWN;
    }

    //riding wall on right -> left corner -> right corner -> straight
    if (robot->ridingWall) {
        //nothing in front -> go straight
        if (front_left_sensor == 0 && right_top_sensor < 3 && robot->currentSpeed < 4)
            robot->direction = UP;
        else if (front_left_sensor >= 3 || right_top_sensor>= 3) {
            if (robot->currentSpeed == 0) {
                robot->direction = LEFT;
            } else
                robot->direction = DOWN;
        }
    }



    /*
    /* Front Sensor Detection (Level >= 3)
    if (front_left_sensor >= 3 || front_right_sensor >= 3) {
        if (robot->currentSpeed > 0) {
            robot->direction = DOWN;
        } else /* robot->currentSpeed == 0  {
            if (!right_top_sensor && !right_btm_sensor && robot->ridingWall) {
                robot->ridingWall = 0;
                robot->searchWall = 1;
                robot->direction = RIGHT;
            } else {
                robot->ridingWall = 1;
                robot->searchWall = 0;
                robot->direction = LEFT;
            }
        }
    }
    */
     /* Right Sensor Detection (Level >= 3)

    else if (right_top_sensor >= 3 || right_btm_sensor >= 3) {
        if (robot->currentSpeed < 4)
            robot->direction = UP;

        robot->ridingWall = 1;
        robot->searchWall = 0;
    }  No Detection (Level <= 2)
    else {
        if (!robot->searchWall) {
            robot->searchWall = 1;
            robot->direction = RIGHT;
        } else if (robot->currentSpeed < 4)
            robot->direction = UP;
    }
    */

}
