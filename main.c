#include "stdio.h"
#include "stdlib.h"
#include "sdl.h"
#include "SDL2_gfxPrimitives.h"
#include "time.h"

#include "formulas.h"
#include "wall.h"
#include "robot.h"

int done = 0;


int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return 1;
    }

    window = SDL_CreateWindow("Robot Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    window = SDL_CreateWindow("Robot Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, 0);

    struct Robot robot;
    struct Wall_collection *head = NULL;
    int front_left_sensor=0, front_right_sensor=0, right_sensor=0, front_mid_sensor=0;
    clock_t start_time, end_time;
    int msec;

    // SETUP MAZE
    // You can create your own maze here. line of code is adding a wall.
    // You describe position of top left corner of wall (x, y), then width and height going down/to right
    // Relative positions are used (OVERALL_WINDOW_WIDTH and OVERALL_WINDOW_HEIGHT)
    //  But you can use absolute positions. 10 is used as the width, but you can change this.
    /*
    insertAndSetFirstWall(&head, 1,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2, 10, OVERALL_WINDOW_HEIGHT/2);
    insertAndSetFirstWall(&head, 2,  OVERALL_WINDOW_WIDTH/2-100, OVERALL_WINDOW_HEIGHT/2+100, 10, OVERALL_WINDOW_HEIGHT/2-100);
    insertAndSetFirstWall(&head, 3,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2+100, 150, 10);
    insertAndSetFirstWall(&head, 4,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2, 150, 10);
    insertAndSetFirstWall(&head, 5,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2-200, 10, 300);
    insertAndSetFirstWall(&head, 6,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2-100, 10, 100);
    insertAndSetFirstWall(&head, 7,  OVERALL_WINDOW_WIDTH/2-250, OVERALL_WINDOW_HEIGHT/2-200, 450, 10);
    insertAndSetFirstWall(&head, 8,  OVERALL_WINDOW_WIDTH/2-150, OVERALL_WINDOW_HEIGHT/2-100, 250, 10);
    insertAndSetFirstWall(&head, 9,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2-200, 10, 300);
    insertAndSetFirstWall(&head, 10,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2-100, 10, 300);
    insertAndSetFirstWall(&head, 11,  OVERALL_WINDOW_WIDTH/2+100, OVERALL_WINDOW_HEIGHT/2+200, OVERALL_WINDOW_WIDTH/2-100, 10);
    insertAndSetFirstWall(&head, 12,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2+100, OVERALL_WINDOW_WIDTH/2-100, 10);
    */
    int j=0; // Key value
    // Adding a circle with radius 50
    for (int i=-50; i<51; i+=1){
        // Leaving a gap
        if (-20>i || i>20) {
            insertAndSetFirstWall(&head, j,  OVERALL_WINDOW_WIDTH/2+i, OVERALL_WINDOW_HEIGHT/2+sqrt(2500-i*i), 5, 5);
            j+=1;
        }
        insertAndSetFirstWall(&head, j,  OVERALL_WINDOW_WIDTH/2+i, OVERALL_WINDOW_HEIGHT/2-sqrt(2500-i*i), 5, 5);
        j+=1;
    }
    // Adding a circle with radius 125
    for (int i=-125; i<126; i+=1){
        // Leaving a few gaps
        if ((70 > i || i > 100) && (-100 > i || i > -75)) {
            insertAndSetFirstWall(&head, j,  OVERALL_WINDOW_WIDTH/2+i, OVERALL_WINDOW_HEIGHT/2-sqrt(15625-i*i), 5, 5);
            j+=1;
        }
        // Leaving more gaps
        if ((-100 > i || i > -75) && (110 > i || i > 121)) {
            insertAndSetFirstWall(&head, j,  OVERALL_WINDOW_WIDTH/2+i, OVERALL_WINDOW_HEIGHT/2+sqrt(15625-i*i), 5, 5);
            j+=1;
        }
        // Creating a wall between the circles
        if (18<i && i<70) {
          insertAndSetFirstWall(&head, j,  OVERALL_WINDOW_WIDTH/2+19+i, OVERALL_WINDOW_HEIGHT/2+19+i, 5, 5);
          j+=1;
        }
        // Creating a wall between the circles
        if (20<i && i<85) {
          insertAndSetFirstWall(&head, j,  OVERALL_WINDOW_WIDTH/2+20+i, OVERALL_WINDOW_HEIGHT/2-15-i/1.5, 5, 5);
          j+=1;
        }
    }
    // Adding a circle with radius 200
    for (int i=-200; i<201; i+=1){
        // Leaving a gap
        if (198 > i) {
            insertAndSetFirstWall(&head, j,  OVERALL_WINDOW_WIDTH/2+i, OVERALL_WINDOW_HEIGHT/2+sqrt(40000-i*i), 5, 5);
            insertAndSetFirstWall(&head, j+1,  OVERALL_WINDOW_WIDTH/2+i, OVERALL_WINDOW_HEIGHT/2-sqrt(40000-i*i), 5, 5);
            j+=2;
        }
        // Creating two walls between the circles
        if (125<i && i<195) {
          insertAndSetFirstWall(&head, j,  OVERALL_WINDOW_WIDTH/2+i, (OVERALL_WINDOW_HEIGHT/2-2+(i/5)), 5, 5);
          j+=1;
        }
    }
    // Cleaining up the circles and adding the end corridoor
    insertAndSetFirstWall(&head, j,  OVERALL_WINDOW_WIDTH/2-200, OVERALL_WINDOW_HEIGHT/2-12.5, 5, 30);
    insertAndSetFirstWall(&head, j+1,  OVERALL_WINDOW_WIDTH/2-125, OVERALL_WINDOW_HEIGHT/2-10, 5, 25);
    insertAndSetFirstWall(&head, j+2,  OVERALL_WINDOW_WIDTH/2+125, OVERALL_WINDOW_HEIGHT/2-10, 5, 25);
    insertAndSetFirstWall(&head, j+3,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2+35, 150, 5);
    insertAndSetFirstWall(&head, j+4,  OVERALL_WINDOW_WIDTH/2+200, OVERALL_WINDOW_HEIGHT/2-35, 150, 5);
    // Creating more walls between the circles
    insertAndSetFirstWall(&head, j+5,  OVERALL_WINDOW_WIDTH/2, OVERALL_WINDOW_HEIGHT/2-200, 5, 75);
    insertAndSetFirstWall(&head, j+6,  OVERALL_WINDOW_WIDTH/2-200, OVERALL_WINDOW_HEIGHT/2, 75, 5);


    setup_robot(&robot);
    updateAllWalls(head, renderer);

    SDL_Event event;
    while(!done){
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);

        //Move robot based on user input commands/auto commands
        if (robot.auto_mode == 1)
            robotAutoMotorMove(&robot, front_left_sensor, front_right_sensor, right_sensor, front_mid_sensor);
        robotMotorMove(&robot);

        //Check if robot reaches endpoint. and check sensor values
        if (checkRobotReachedEnd(&robot, OVERALL_WINDOW_WIDTH-55, OVERALL_WINDOW_HEIGHT, 10, 0)){
            end_time = clock();
            msec = (end_time-start_time) * 1000 / CLOCKS_PER_SEC;
            robotSuccess(&robot, msec);
        }

        else if(checkRobotHitWalls(&robot, head))
            robotCrash(&robot);
        //Otherwise compute sensor information
        else {
            front_left_sensor = checkRobotSensorFrontLeftAllWalls(&robot, head);
            // if (front_left_sensor>0)
            //     printf("Getting close on the left. Score = %d\n", front_left_sensor);

            front_right_sensor = checkRobotSensorFrontRightAllWalls(&robot, head);
            // if (front_right_sensor>0)
            //     printf("Getting close on the right. Score = %d\n", front_right_sensor);

            right_sensor = checkRobotSensorRightAllWalls(&robot, head);

            front_mid_sensor = checkRobotSensorFrontMidAllWalls(&robot, head);
        }
        robotUpdate(renderer, &robot);
        updateAllWalls(head, renderer);

        // Check for user input
        SDL_RenderPresent(renderer);
        while(SDL_PollEvent(&event)){
            const Uint8 *state = SDL_GetKeyboardState(NULL);

            if(event.type == SDL_QUIT || state[SDL_SCANCODE_Q]) {
                /* First Algorithm */
                // free(robot.history);
                done = 1;
            }
            if(state[SDL_SCANCODE_UP] && robot.direction != DOWN){
                robot.direction = UP;
            }
            if(state[SDL_SCANCODE_DOWN] && robot.direction != UP){
                robot.direction = DOWN;
            }
            if(state[SDL_SCANCODE_LEFT] && robot.direction != RIGHT){
                robot.direction = LEFT;
            }
            if(state[SDL_SCANCODE_RIGHT] && robot.direction != LEFT){
                robot.direction = RIGHT;
            }
            if(state[SDL_SCANCODE_SPACE]){
                setup_robot(&robot);
            }
            if(state[SDL_SCANCODE_RETURN]){
                robot.auto_mode = 1;
                start_time = clock();
            }
        }

        SDL_Delay(120);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    printf("DEAD\n");
}
