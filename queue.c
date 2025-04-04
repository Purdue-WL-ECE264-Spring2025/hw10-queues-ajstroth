#include "queue.h"
#include "tile_game.h"

#include <stdlib.h>

//max number of states for consistent in number of moves
#define MAX 1000000
#define SIZE (1ULL << 24)


//added here for easier reference
/*struct queue {
    struct linked_list data;
  };*/

//serialze on the tile layout
//inpmlemented to try and reduce timing out
uint64_t serialize_tiles(struct game_state state)
{
    //iniialze reuslt to hold serialzied value
    uint64_t result = 0;
    //loop through grid
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            //shift the result by 4 to make room for the next tile
            result <<= 4;
            //use bitwaise or to add tile balue
            //the & 0xF ensure that the valeuf stays within 0-15 or 4 bits
            result |= (uint64_t)(state.tiles[i][j] & 0xF);
        }
    }
    return result;
}

//set the bit at the index index in the puzzle
void set_complete(uint8_t *map, uint64_t index)
{
    //calculate which byte contains the bit
    //use bitwise or to set the bit at potion
    map[index / 8] |= (1 << (index % 8));
}

//check if the inedx index is already set
bool isComplete(uint8_t *map, uint64_t index)
{
    //check the bit at postion (index % 8) in the byta at (index /8)
    //does bitwise and if the result is not zero than the bit is set
    return map[index / 8] & (1 << (index % 8));
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

    //alocate memory to track completes tiles
    uint8_t *complete = calloc(SIZE / 8, sizeof(uint8_t));
    //alocate memory to track step taken to solve
    uint64_t *steps = calloc(SIZE, sizeof(uint64_t));
    //track the number of completes tiles
    //size_t completeCnt = 0;

    //if memory allocation failrs
    if (!complete || !steps)
    {
        free(steps);
        free(complete);
        //return -1 which indicates fail to solve
        return -1;
    }

    //serialze the starting state
    uint64_t startSer = serialize_tiles(start);
    uint64_t startIdx = startSer % SIZE;
    //mark it as complete
    set_complete(complete, startIdx);
    steps[startIdx] = 0;
    //initiaze BFS with starting start
    //steps[completeCnt++] = 0;

    //serrialze the start tile
    //uint64_t startSer = serialize_tiles(start);
    //set the steps at this point to be 0
    //steps[0] = 0;
    //set the tile that startSer is at to be completed
    //complete[completeCnt++] = startSer;

    //enqueue(&q, child);
    //add starting tile to queue
    enqueue(&q, start);

    //while (!empty(s))
    //BfS loop
    while (q.data.head)
    {
        //node cur = dequeue(&q);
        //dequeue the tile to get the next tile
        struct game_state cur = dequeue(&q);
        //serilizate the current tile
        uint64_t currentInt = serialize_tiles(cur) % SIZE;
        cur.num_steps = steps[currentInt];

        //should always be true sicnce it is already set
        //if (!isComplete(complete, currentInt))
        //{
          //  continue;
        //}

        //i think this is the loop causing erros, so remove
        /*//set tseen to be fale
        //indicated wheter currentInt has been visited
        bool seen = false;
        //create indicy of currentInt to be 0
        size_t stepIdx = 0;

        //check is current tile has been seen begore
        for (size_t i = 0; i < completeCnt; i++)
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
        //cur.num_steps = steps[completeCnt - 1];

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
            free(complete);
            free(steps);
            //use free_list to free q.data
            free_list(q.data);
            //retunr the numver of moves made to get to solved
            return cur.num_steps;
        }


        //moves in 4 direction
        int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        //iniialze rows and cols
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
                //move on to next k
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
            uint64_t nextSer = serialize_tiles(nextCur) % SIZE;
            uint64_t nextInt = nextSer % SIZE;
            //uint64_t nextIdx = nextInt % MAX;

            if (isComplete(complete, nextInt))
            {
                continue;
            }

            //if (completeCnt >= MAX)
            //{
             //   continue;
            //}
            //i think this loop could also be an issue
            //check to see if the file has already been seen
            /*bool completeAlready = false;
            for (size_t i = 0; i < completeCnt; i++)
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

            set_complete(complete, nextInt);
            //track the steps to get to this tile
            steps[nextInt] = cur.num_steps + 1;
            //add to BFS queue
            //enqueue(&q, child);
            enqueue(&q, nextCur);
        }
    }
    
    //if no solution is found
    //free memory
    free(complete);
    free(steps);
    //use free_list to free q.data memory
    free_list(q.data);

    //return -1 to insicate that the tile puzzle cannot be solved
    return -1;
    //return 0;
}

