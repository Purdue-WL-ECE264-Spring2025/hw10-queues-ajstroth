#include "queue.h"

#include "tile_game.h"


#include <stdlib.h>


//max number of states for consistent in number of moves

#define MAX 1000000



//added here for easier reference

/*struct queue {

    struct linked_list data;

  };*/

  //helper function to check if two tile layour are equal
bool sameLayout(struct game_state *a, struct game_state *b)
{
    //loops trough layout to see if the layouts are the smae
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (a->tiles[i][j] != b->tiles[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

//serialate the layout helper function
uint64_t serializeLayout(struct game_state state)
{
    uint64_t stepsCopy = state.num_steps;
    state.num_steps = 0;
    uint64_t result = serialize(state);
    state.num_steps = stepsCopy;
    return result;
}

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

    return deserialize(remove);

}


//find the minum number of moves to solve

int number_of_moves(struct game_state start) 

{

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

    //iniitalize queue

    //queue q = new_queue();

    struct queue q = {0};

    //set the head of q to be null

    q.data.head = NULL;

    struct linked_list seenStates = {0};

    //alocate memory to track completes tiles

    //uint64_t *complete = calloc(MAX, sizeof(uint64_t));

    //track the number of completes tiles

    //size_t completeCnt = 0;

    //alocate memory to track step taken to solve

    //uint64_t *steps = calloc(MAX, sizeof(uint64_t));


    //if memory allocation failrs

    //if (!complete || !steps)

    //{

        //free memory

      //  free(complete);

     //   free(steps);

        //return -1 which indicates fail to solve

//        return -1;
//
  //  }


    //enqueue(&q, child);

    //add starting tile to queue

    enqueue(&q, start);

    insert_at_tail(&seenStates, serializeLayout(start));


    //serrialze the start tile

    //uint64_t startSer = serialize(start);

    //set the steps at this point to be 0

    //steps[0] = 0;

    //set the tile that startSer is at to be completed

    //complete[completeCnt++] = startSer;



    //while (!empty(s))

    //BfS loop

    while (q.data.head)

    {

        //node cur = dequeue(&q);

        //dequeue the tile to get the next tile

        struct game_state cur = dequeue(&q);

        //serilizate the current tile

        //uint64_t currentInt = serialize(cur);


        //set tseen to be fale

        //indicated wheter currentInt has been visited

        //bool solve = false;

        //create indicy of currentInt to be 0

        //size_t stepIdx = 0;


        //check is current tile has been seen begore

        /*for (size_t i = 0; i < completeCnt; i++)

        {

            //if that tile is equal to currentSer than it has been seen

            if (complete[i] == currentInt)

            {

                //seen seen to true

                seen = true;

                //the index of currentInt set to i where it has been seen incomplete

                stepIdx = i;

                //end loop

                break;

            }

        }


        //if the current tile hasnt been seen

        if (!seen)

        {

            //move on

            continue;

        }*/


        //set the current number of steps to be the number of steps at the stepIdz for currentInt

        //cur.num_steps = steps[stepIdx];


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

                    //if value is correct than move on

                    value++;

                }

            }

        }

        

        //if the tile puzzle haas been solved

        if (solved)

        {

            //free memory

            free_list(seenStates);

            //free(steps);

            //use free_list to free q.data

            free_list(q.data);

            //retunr the numver of moves made to get to solved

            return cur.num_steps;

        }



        //moves in 4 direction


        void (* moves[4])(struct game_state *) = {
            move_up, move_down, move_left, move_right
        };

        //attempt to move each direction

        for (int k = 0; k < 4; k++)

        {

            struct game_state nextCur = cur;

            moves[k](&nextCur);

            if (nextCur.empty_row == cur.empty_row && nextCur.empty_col == cur.empty_col)
            {
                continue;
            }

            uint64_t layout = serializeLayout(nextCur);


            //check to see if the file has already been seen

            bool completeAlready = false;

            for (struct list_node *node = seenStates.head; node != NULL; node = node->next)
            {
                if (node->value == layout)
                {
                    completeAlready = true;
                    break;
                }
            }

            if (completeAlready)
            {
                continue;
            }

            /*for (size_t i = 0; i < completeCnt; i++)

            {

                //if the next inteerger has been seen

                if(complete[i] == nextInt)

                {


                    //set to true 

                    completeAlready = true;

                    //end loop

                    break;

                }

            }


            //if the tile hasnt been seen

            if (!completeAlready)

            {

                //if the count is greater than the max

                if (completeCnt >= MAX)

                {

                    //move on

                    continue;

                }

            }*/


            //track the steps to get to this tile

            //steps[completeCnt] = nextCur.num_steps;

            //mark the start as seen

            //complete[completeCnt++] = nextInt;

            insert_at_tail(&seenStates, layout);

            //add to BFS queue

            //enqueue(&q, child);

            enqueue(&q, nextCur);

        }

    }

    

    //if no solution is found

    //free memory

    //free(complete);

    free_list(seenStates);

    //use free_list to free q.data memory

    free_list(q.data);


    //return -1 to insicate that the tile puzzle cannot be solved

    return -1;

    //return 0;

}
