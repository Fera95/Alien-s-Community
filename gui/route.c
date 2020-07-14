#include "route.h"
#include "gui.h"

/**
 * ctx->map[0] = alfaexit;
    ctx->map[1] = alfaentry;
    ctx->map[2] = northavenueA;
    ctx->map[3] = northavenueB;
    ctx->map[4] = southavenueA;
    ctx->map[5] = southavenueB;
    ctx->map[6] = betaexit;
    ctx->map[7] = betaentry;
 * 
 */ 
ROUTE * create_route(BRIDGE *bridge, PATH **map, enum alienType type, float dx, float dy  )
{
    ROUTE *newRoute = malloc(sizeof(ROUTE));
    if(type == alfa)
    {
        newRoute->exit = map[0];
        newRoute->avenue1 = map[2];
        newRoute->bridge = bridge;
        
        newRoute->cross2avenue = map[5];
        newRoute->avenue2 = map[4];
        newRoute->cross2entry = map[5];
        newRoute->entry = map[7];
        newRoute->diry = up;
        newRoute->dirx = right;
    }
    else if (type == beta)
    {
        newRoute->exit = map[6];
        newRoute->avenue1 = map[5];
        
        newRoute->bridge = bridge;
        
        newRoute->cross2avenue = map[2];
        newRoute->avenue2 = map[3];
        newRoute->cross2entry = map[2];
        newRoute->entry = map[1];
        newRoute->diry = down;
        newRoute->dirx = left;
        
    }
    newRoute->current = newRoute->exit;
    newRoute->pos = 0;
    printf("FIRST POSITION of CURRENT type %d: X = %f, Y = %f\n", newRoute->type, newRoute->current->x, newRoute->current->y);
    newRoute->limit = EXIT_COUNT;
    newRoute->type = type;
    newRoute->finished = 0;
    return newRoute;
}



void next_move(float *next_x, float* next_y, ROUTE * alienRoute, float dx, float dy)
{
    
    int change_pos = 1;
    float tempx,tempy;

    if (alienRoute->diry-5 == down){
        tempy = *next_y + dy; 
        if(tempy < alienRoute->current[alienRoute->pos].y){
            *next_y = tempy;
            change_pos = 0;
        }
    }
    if (alienRoute->diry+5 == up){
        tempy = *next_y - dy; 
        if(tempy > alienRoute->current[alienRoute->pos].y){
            *next_y = tempy;
            change_pos = 0;
        }
    }
    
    if (alienRoute->dirx-5 == left){
        tempx = *next_x - dx; 
        if(tempx > alienRoute->current[alienRoute->pos].x){
            *next_x = tempx;
            change_pos = 0;
        }
    }
    if (alienRoute->dirx+5 == right){
        tempx = *next_x + dx; 
        if(tempx < alienRoute->current[alienRoute->pos].x){
            *next_x = tempx;
            change_pos = 0;
        }
    }

    // printf("Moving alien type: %d\n",alienRoute->type);
    // printf("Current: X = %f, Y = %f\n", *next_x, *next_y);
    // printf("Limit: X = %f, Y = %f\n", alienRoute->current[alienRoute->pos].x, alienRoute->current[alienRoute->pos].y);
    // printf("speed: dx = %f, dy = %f\n", dx, dy);
    
    if(change_pos){
        printf("POS: %d\n",alienRoute->pos);
        printf("LIMIT: %d\n",alienRoute->limit);
            
        int tempPos = alienRoute->pos;
        int tempLimit = alienRoute->limit;
        PATH *nextPath = alienRoute->current;
        // CASO PARTICULAR
        if(alienRoute->current == alienRoute->bridge->pass && alienRoute->type==beta)
            tempPos--;
        else
            tempPos++;

        if (tempPos == tempLimit)
        {
            printf("CALCULA INTERSECCION\n");
            // Salida del a -> Avenida
            if(alienRoute->current == alienRoute->exit){
                nextPath = alienRoute->avenue1;
                tempPos = 2;
                if(alienRoute->type == alfa)
                {
                    if(alienRoute->bridge->position == east)
                        tempLimit = 6;
                    else if(alienRoute->bridge->position == mid)
                        tempLimit = 10;
                    else if(alienRoute->bridge->position == west)
                        tempLimit = 14;
                }

                else if(alienRoute->type == beta)
                {
                    if(alienRoute->bridge->position == east)
                        tempLimit = 15;
                    else if(alienRoute->bridge->position == mid)
                        tempLimit = 11;
                    else if(alienRoute->bridge->position == west)
                        tempLimit = 6;
                }
            }
            // Avenida -> Cola
            else if(alienRoute->current == alienRoute->avenue1){
                tempLimit = alienRoute->bridge->queueSize;
                tempPos = 0;

                if(alienRoute->type == alfa){
                    nextPath = alienRoute->bridge->queueNorth;
                }
                else if(alienRoute->type == beta){
                    nextPath = alienRoute->bridge->queueSouth;

                }
                // LLAMAR A CALENDARIZADOR AQUI ORDENE LA COLA
            }

            // Cola -> Puente
            else if(alienRoute->current == alienRoute->bridge->queueNorth){
                nextPath = alienRoute->bridge->pass;
                tempLimit = alienRoute->bridge->length;
                tempPos = 0;
            }
            // Cola -> Puente
            else if(alienRoute->current == alienRoute->bridge->queueSouth){
                nextPath = alienRoute->bridge->pass;
                tempLimit = -1;
                tempPos = alienRoute->bridge->length-1;
            }
            // Puente -> salida
            else if(alienRoute->current == alienRoute->bridge->pass){
                tempLimit = alienRoute->bridge->queueSize;
                tempPos = 0;
                if(alienRoute->type == alfa){
                    nextPath = alienRoute->bridge->exitNorth;
                }
                else if(alienRoute->type == beta){
                    nextPath = alienRoute->bridge->exitSouth;
                }

            }
            // Salida -> Avenida (Cruzar avenida)
            else if(alienRoute->current == alienRoute->bridge->exitNorth){
                nextPath = alienRoute->cross2avenue;
                if(alienRoute->bridge->position == east){
                    tempLimit = AVENUE_COUNT;
                    tempPos = AVENUE_COUNT-1;
                }
                else if(alienRoute->bridge->position == mid){
                    tempLimit = 12;
                    tempPos = 11;
                }
                else if(alienRoute->bridge->position == west){
                    tempLimit = 7;
                    tempPos = 6;
                }
                
            }
            // Salida -> Avenida (Cruzar avenida)
            else if(alienRoute->current == alienRoute->bridge->exitSouth){
                nextPath = alienRoute->cross2avenue;
                if(alienRoute->bridge->position == east){
                    tempLimit = 6;
                    tempPos = 5;
                }
                else if(alienRoute->bridge->position == mid){
                    tempLimit = 11;
                    tempPos = 10;
                }
                else if(alienRoute->bridge->position == west){
                    tempLimit = AVENUE_COUNT;
                    tempPos = AVENUE_COUNT-1;
                }
                
            }
            // Avenida -> Avenida (Cruzar avenida)
            else if(alienRoute->current==alienRoute->cross2avenue&& tempPos!=1){
                nextPath = alienRoute->avenue2;
                tempLimit = AVENUE_COUNT;
                tempPos = AVENUE_COUNT - alienRoute->limit;
            }
            // Avenida -> Entrada
            else if(alienRoute->current == alienRoute->avenue2 ){
                nextPath = alienRoute->cross2entry;
                tempLimit = 1;
                tempPos= 0;
            }
            else if(alienRoute->current == alienRoute->cross2entry){
                nextPath = alienRoute->entry;
                tempLimit = ENTRY_COUNT;
                tempPos= 0;
            }
            // Final de trayecto, llegada a comunidad.
            else if(alienRoute->current == alienRoute->entry)
                alienRoute->finished = 1;

        }
        int available = !nextPath[tempPos].blocked;
        // LLAMAR AL SEMAFORO DEL PUENTE
        

        if(available){
            // Liberamos el actual
            alienRoute->current[alienRoute->pos].blocked = 0;
            // Asignar el nuevo espacio.
            alienRoute->current = nextPath;
            alienRoute->limit = tempLimit;
            alienRoute->pos = tempPos;
            printf("NEW POS: %d\n",alienRoute->pos);
            printf("NEW LIMIT: %d\n",alienRoute->limit);
          
            alienRoute->current[alienRoute->pos].blocked = 1;
            
            /**
             * Direction of speed
            */ 
            // OCUPO IR A LA DERECHA
            if( *next_x < alienRoute->current[tempPos].x ){
                printf("Derecha\n");
                alienRoute->dirx=right;

            }
            // OCUPO IR A LA IZQUIERDA
            else if(*next_x > alienRoute->current[tempPos].x){
                printf("Izquierda\n");
                alienRoute->dirx=left;

            }

            // OCUPO BAJAR
            if (*next_y < alienRoute->current[tempPos].y ){
                alienRoute->diry=down;
                printf("BAJANDO\n");
            }
                
            // OCUPO SUBIR
            else if (*next_y > alienRoute->current[tempPos].y ){
                alienRoute->diry=up;
                printf("Subiendo\n");


            }

        }
        // retornar al estado
        else{
            printf("CAMINO BLOQUEADO\n");
        }
    }
}

