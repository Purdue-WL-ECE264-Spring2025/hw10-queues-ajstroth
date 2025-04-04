#include "queue.h"
#include "tile_game.h"

//added here for easier reference
/*struct queue {
    struct linked_list data;
  };*/

void enqueue(struct queue *q, struct game_state state) 
{
    //inset at tail or head can use either
    //use serialize found in tile_game to transfrom into a integer
    insert_at_tail(&q->data, serialize(state));
}

struct game_state dequeue(struct queue *q) 
{ 
    //reomve tail or head use the one that is contrery to enqueue
    //thus will reomve head
    size_t remove = remove_from_head(&q->data);
    //destialize the valye and then return it
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
    //queue q = new_queue();
    struct queue q = {0};

    //enqueue the starting game structure
    //insert the start data ar the tail
    enqueue(&q, start);

    //allocate memory for an array of bools to store if the tile has been moved
    bool *complete = malloc(sizeof(bool));
    //check if allocation was successful
    if (complete != NULL)
    {
        //if successful inisiaze each element to 0
        //because ate the start nothing has been moved
        for (int i = 0; i < 100; i++)
        {
            complete[i] = 0;
        }
    }
    else
    {
        //if memory allocation fails
        return NULL;
    }

    //while (!empty(s))
    while (!q.data.head)
    {
        //node cur = dequeue(&q);
        //dequeue the current state
        struct game_state cur = dequeue(&q);
        //serilizate the current state
        size_t current = serialize(cur);

        //test to see if the tile had already been seen or meoved
        if (complete[current] == 0)
        {
            //if it has already been visited move to the next tile
            continue;
        }

        //mark the tile as being visited
        complete[current] = 1;

        //if (equals(cur, search))
        if (solved???))
        {
            //free memory
            free(complete);
            free(q.data);
            //return the number of moves to get to the current tile;
            return current.num_moves;
        }
        
        //max 4 surronding tiles
        struct game_state tile[4];
        //children(cur)
        //find a way to find the surronding tile and the empty way
        
        //for (node child in children(cur))
        //go through each tile surronding
        for (size_t i = 0; i < cnt; i++)
        {
            //turn the tile into a interger with serialize
            size_t tileInt = serialize(tile[i]);

            if(!complete[tileInt])
            {
                //increasae the number of moves 
                tile[i].num_moves = current.num_moves + 1;
                //enqueue(&q, child);
                //add to the tial and moves on 
                enqueue(&q, tile[i]);
            }

        }
    }
    
    //if no solution is found
    //free memory
    free(complete);
    free(q.data);

    //return -1 to insicate that the tile puzzle cannot be solved
    return -1;
}
