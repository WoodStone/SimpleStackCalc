#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef enum { INT, PUSH, POP, ADD, SUB, MUL, DIV, OUT, END } token_t;
#define BUFSIZE 12
char buffer[BUFSIZE];

#define NSTATES 23
int16_t transtab[NSTATES][256];

void
setup_table ( void )
{
    memset ( transtab, 0, NSTATES*256*sizeof(int16_t) );

    transtab[0]['0'] = 1;
    transtab[0]['1'] = 1;
    transtab[0]['2'] = 1;
    transtab[0]['3'] = 1;
    transtab[0]['4'] = 1;
    transtab[0]['5'] = 1;
    transtab[0]['6'] = 1;
    transtab[0]['7'] = 1;
    transtab[0]['8'] = 1;
    transtab[0]['9'] = 1;

    transtab[1]['0'] = 1;
    transtab[1]['1'] = 1;
    transtab[1]['2'] = 1;
    transtab[1]['3'] = 1;
    transtab[1]['4'] = 1;
    transtab[1]['5'] = 1;
    transtab[1]['6'] = 1;
    transtab[1]['7'] = 1;
    transtab[1]['8'] = 1;
    transtab[1]['9'] = 1;
    transtab[1][0] = 1;

    transtab[0]['p'] = 2;
    transtab[2]['u'] = 3;
    transtab[3]['s'] = 4;
    transtab[4]['h'] = 5;
    transtab[5][0] = 1;

    transtab[2]['o'] = 6;
    transtab[6]['p'] = 7;
    transtab[7][0] = 1;

    transtab[0]['a'] = 8;
    transtab[8]['d'] = 9;
    transtab[9]['d'] = 10;
    transtab[10][0] = 1;

    transtab[0]['s'] = 11;
    transtab[11]['u'] = 12;
    transtab[12]['b'] = 13;
    transtab[13][0] = 1;

    transtab[0]['m'] = 14;
    transtab[14]['u'] = 15;
    transtab[15]['l'] = 16;
    transtab[16][0] = 1;

    transtab[0]['d'] = 17;
    transtab[17]['i'] = 18;
    transtab[18]['v'] = 19;
    transtab[19][0] = 1;

    transtab[0]['o'] = 20;
    transtab[20]['u'] = 21;
    transtab[21]['t'] = 22;
    transtab[22][0] = 1;
}


token_t
next ( FILE *input )
{
    int state = 0;
    int iteration = 0;
    memset(buffer, 0, BUFSIZE* sizeof(char));

    char current_char = getc(input);
    while (1) {
        int next_state = transtab[state][current_char];
        if (next_state == 0) {
            break;
        }
        state = next_state;
        buffer[iteration] = current_char;
        iteration = iteration + 1;

        current_char = getc(input);
        if (transtab[state][0] == 1 && current_char == '\n') {
            switch (state)
            {
                case 1:
                    return INT;
                case 5:
                    return PUSH;
                case 7:
                    return POP;
                case 10:
                    return ADD;
                case 13:
                    return SUB;
                case 16:
                    return MUL;
                case 19:
                    return DIV;
                case 22:
                    return OUT;
                default:
                    return END;
            }
        }
    }

    return END;
}

#define STACK_LIMIT 256
void
act ( token_t token, char *buffer )
{
    static int32_t
            stack[STACK_LIMIT],
            stack_index = 0,
            register_value = 0;
    switch ( token )
    {
        case INT:
            register_value = strtol(buffer, NULL, 10);
            break;
        case PUSH:
            stack_index += 1;
            stack[stack_index] = register_value;
            break;
        case POP:
            register_value = stack[stack_index];
            stack_index -= 1;
            break;
        case ADD:
            stack[stack_index] += register_value;
            break;
        case SUB:
            stack[stack_index] -= register_value;
            break;
        case MUL:
            stack[stack_index] *= register_value;
            break;
        case DIV:
            stack[stack_index] /= register_value;
            break;
        case OUT:
            printf ( "%d\n", register_value );
            break;
    }
}


int
main ()
{
    setup_table();
    token_t next_token;
    do {
        next_token = next(stdin);
        act ( next_token, buffer );
    } while ( next_token != END );
    exit ( EXIT_SUCCESS );
}
