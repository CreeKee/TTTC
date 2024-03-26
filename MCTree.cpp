#include "MCTree.hpp"


#define SIM_DEPTH 15
#define RAND_MAX 100

#define PLACE_WEIGHT 0.25
#define PLOCK_WEIGHT 0.25
#define CLAIM_WEIGHT 0.25
#define BLOCK_WEIGHT 0.25

#define PLACE_CHANCE ((PLACE_WEIGHT                                             ) * RAND_MAX)
#define PLOCK_CHANCE ((PLACE_WEIGHT + PLOCK_WEIGHT                              ) * RAND_MAX)
#define CLAIM_CHANCE ((PLACE_WEIGHT + PLOCK_WEIGHT + CLAIM_WEIGHT               ) * RAND_MAX)
#define BLOCK_CHANCE ((PLACE_WEIGHT + PLOCK_WEIGHT + CLAIM_WEIGHT + BLOCK_WEIGHT) * RAND_MAX)




void MCNode::runSim(){

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> rando(1,RAND_MAX); // distribution in range [1, 6]

    uint32_t val;


    int32_t win = 0;

    //copy current board state

    for(uint32_t sim = 0; sim < SIM_DEPTH && win == 0; sim++){

        
        val = rando(rng);

        //get all empty tiles from board
        if(emptyTiles == 0) val *= (PLACE_WEIGHT+PLOCK_WEIGHT);
        else if(emptyTiles == 1) val *= (1-BLOCK_WEIGHT);

        

        //do random move
        if(val <= PLACE_CHANCE){
            //place random tile x3
        } 
        else if(val <= PLOCK_CHANCE){
            //place random tile then block random tile
        } 
        else if(val <= CLAIM_CHANCE){
            //claim random tile and check win/loss
        } 
        else if(val <= BLOCK_CHANCE){
            //block random tile x2
        }
        else{
            //uh oh
            fprintf(stderr, "someone cant do math\n");
            exit(-1);
        }


        backprop();
    }
}