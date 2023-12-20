#include "reader.h"


static struct termios old;

static struct termios current;

/*
 * Initialise IO settings 
 */
void init_term_io_settings()
{
    tcgetattr(0, &old); /* grab old terminal i/o settings */
    current = old; /* copy to modify attribute */
    current.c_lflag &= ~ICANON; /* disable the enter to get value */
    current.c_lflag &= ~ECHO; /* set no echo mode */
    tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/*
 * Reset to the original settings of the terminal I/O
 */
void reset_term_io_settings(void) 
{
  tcsetattr(0, TCSANOW, &old);
}


/*
 * Read a Line
 * allow the user to move into it command to modify using the cursor controlled by the arrow 
 * the user can charge old command from the history 
 * every command the user wrote is stock in a history
 * 
 */
int read_line(char* command)
{
    memset(command,0,4096);
    init_term_io_settings();
    int index = 0;
    int maxSize = 0;
    int endOfLine = 0;
    char c = 0;
    while(!endOfLine)
    {
        c = getchar();
        switch(c)
        {
            case BACKWARD:
            {

                if(index != 0){
                    index--;
                    for(int i = index ; i <= maxSize ; i++){
                        command[i] = command[i+1];
                    }
                    maxSize--;
                    printf("\033[%dD",1);
                    command[maxSize] = '\0';
                    for(int i = index ; i <= maxSize ; i++){
                        printf("%c",command[i]);
                    }
                    printf(" ");
                    printf("\033[%dD",(maxSize-(index)+1));



                }

                break;
            }
            case ENTER:
            {
                endOfLine = 1;
                printf("\n");
                break;
            }
            case ARROW:
            {
                getchar();
                c = getchar();
                switch (c)
                {
                    case RIGHT_ARROW:
                    {
                        if(maxSize != index){ // right deplacement
                            printf("\033[%dC",1);
                            index++;
                        }
                        break;

                    }
                    case LEFT_ARROW:
                    {
                        if(index > 0){ // left deplacement
                            printf("\033[%dD",1);
                            index--;


                        }
                        break;
                    }
                    
                            
        
                    
                    
                    default:
                        break;

                }
                break;
            }
            default:
            {
                for(int i = maxSize ; i > index ; i--){
                    command[i] = command[i-1];
                }
                command[index] = c;
                for(int i = index ; i <= maxSize ; i++){
                    printf("%c",command[i]);
                }
                maxSize++;
                command[maxSize] = '\0';
                index++;

                if(index == 1 && maxSize != 1){
                    printf("\033[%dD",(maxSize-(index)));
                }
                else if(index != maxSize && index != 0){
                    printf("\033[%dD",(maxSize-(index)));
                }
                else{
                    printf("\033[%dD",(maxSize-(index+1)));
                }
                break;
            }
            
        }


    }

    reset_term_io_settings();
    return 0;
}
