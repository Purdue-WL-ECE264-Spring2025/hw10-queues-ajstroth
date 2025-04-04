#include "queue.h"
#include "tile_game.h"

#include <stdlib.h>

#define MAX 1000000


//added here for easier reference
/*struct queue {
    struct linked_list data;
  };*/

void enqueue(struct queue *q, struct game_state state) 
{
    //inset at tail or head can use either
    //use serialize found in tile_game to transfrom into a integer
    insert_at_tail(&q->data, (size_t)serialize(state));
}

struct game_state dequeue(struct queue *q) 
{ 
    //reomve tail or head use the one that is contrery to enqueue
    //thus will reomve head
    size_t remove = remove_from_head(&q->data);
    if (!q->data.head)
    {
        struct game_state invalid = {0};
        return invalid; 
    }

    //destialize the valye and then return it
    fprintf(stderr, "Dequeing value : %zu\n", remove);
    fflush(stderr);
    return deserialize(remove);
}

//find the minum number of moves to solve
int number_of_moves(struct game_state start) 
{
    fprintf(stderr, "straing number_of_moves\n");
    fflush(stderr);
    //want to follow this structr BFS
    /*
    node bfs(graph g, node start, node search)
    {
        queue q = new_queue();
        while (!empty(s))
        {
            node cur = dequeue(&q);
            if (equals(cur, search))
            {
                return cur;
            } else {
                for (node child in children(cur))
                {
                    enqueue(&q, child);
                }
            }
        }
    }
    */
    //start with an empty queue
    //queue q = new_queue();
    struct queue q = {0};

    q.data.head = NULL;

    //serialize the starting state 
    //uint64_t startSer = serialize(start);

    //initial tracking things
    //allocate memory for an array of bools to store if the tile has been moved
    //did 16^4
    bool *complete = calloc(MAX, sizeof(bool));
    uint64_t *steps = calloc(MAX, sizeof(uint64_t));

    //if memory allocation failrs
    if (!complete || !steps)
    {
        free(complete);
        free(steps);
        return -1;
    }

    //enqueue(&q, child);
    enqueue(&q, start);

    dequeue(&q);
    //fprintf(stderr, "enqueued start state\n");
    //fflush(stderr);
    /*
    uint64_t startSer = serialize(start);
    uint64_t startIdx = startSer % MAX;
    steps[startIdx] = 0;

    //initiales step to 0
    //steps[startSer] = 0;

    fprintf(stderr, "enqueued start state2\n");
    fflush(stderr);

    //while (!empty(s))
    while (q.data.head)
    {
        fprintf(stderr, "entered loop\n");
        fflush(stderr);
        //node cur = dequeue(&q);
        //dequeue the current state
        struct game_state cur = dequeue(&q);
        //serilizate the current state
        uint64_t currentSer = serialize(cur);
        uint64_t currentIdx = currentSer % MAX;

        fprintf(stderr, "entered loop2\n");
        fflush(stderr);
        //test to see if the tile had already been seen or meoved
        if (complete[currentIdx])
        {
            //if it has already been visited move to the next tile
            continue;
        }

        //mark the tile as being visited
        complete[currentIdx] = true;
        //update the numver of steps
        cur.num_steps = steps[currentIdx];

        //have to determine if the tile puzzle is solved
        //assume that the puzzle is solved
        bool solved = true;
        //expected value starts at 1
        int value = 1;

        //GOAL
        //1 2 3 4 
        //5 6 7 8
        //9 10 11 12
        //13 14 15 0


        //go through tile puzzle
        for (int i = 0; i < 4 && solved; i++)
        {
            for (int j = 0; j < 4 && solved; j++)
            {
                if (i == 3 && j == 3)
                {
                    //the bottom right corrent should be 0 and empty
                    if (cur.tiles[i][j] != 0)
                    {
                        //if it not zero then it is not solved
                        solved = false;
                    }
                }
                else
                {
                    //the other tiles should be in the correct order
                    if (cur.tiles[i][j] != value)
                    {
                        solved = false;
                    }
                    value++;
                }
            }
        }
        
        //if the tile puzzle haas been solved
        if (solved)
        {
            //free memory
            free(complete);
            free(steps);
            free_list(q.data);
            //retunr the numver of moves made to get to solved
            return cur.num_steps;
        }


        //moves in 4 direction
        int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        int row = cur.empty_row;
        int col = cur.empty_col;

        //attempt to move each direction
        for (int k = 0; k < 4; k++)
        {
            int newRow = row + dir[k][0];
            int newCol = col + dir[k][1];
            
            //if it is out of bounds
            if (newRow < 0 || newRow >= 4 || newCol < 0 || newCol >= 4)
            {
                continue;
            }
            
            //create next state to modify to aboid actually changing the current state
            struct game_state nextCur = cur;

            //swap the 0 with the neighboring tile
            nextCur.tiles[row][col] = nextCur.tiles[newRow][newCol];
            nextCur.tiles[newRow][newCol] = 0;

            //update the position of the blank tile o tile
            nextCur.empty_row = newRow;
            nextCur.empty_col = newCol;

            //seriable the next integer and set it to next
            uint64_t nextSer = serialize(nextCur);
            uint64_t nextIdx = nextSer % MAX;

            if (nextIdx < 65536 && !complete[nextIdx])
            {
                //incrmeent the num of moves taken to try to solve
                steps[nextIdx] = cur.num_steps + 1;
                enqueue(&q, nextCur);
            }
        }
    }
    
    //if no solution is found
    //free memory
    free(complete);
    free(steps);
    free_list(q.data);

    //return -1 to insicate that the tile puzzle cannot be solved
    return -1;*/
    return 0;
}
