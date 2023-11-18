#include "parser/ast.h"
#include "shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "inc/commandHandler.h"
#include "inc/getUserInfo.h"
#include "inc/handlePipe.h"

#define MAX_BUFFER_SIZE 1024
static void ignore(){}

void initialize(void)
{
    /* This code will be called once at startup */
    if (prompt){
        char buffer[MAX_BUFFER_SIZE];
        getCommandLinePrompt(buffer);
        prompt = buffer;
    }
    
}

void run_command(node_t *node)
{
    signal(SIGINT, ignore);
    /* Print parsed input for testing - comment this when running the tests! */
    print_tree(node);

    if (node->type == NODE_COMMAND) {
    char *program = node->command.program;
    char **argv = node->command.argv;
    }

   switch (node->type) {
    case NODE_COMMAND:
        HandleCommand(node);
        break;
    case NODE_PIPE:
        HandlePipe(node);
        break;
    case NODE_SEQUENCE:
        HandleCommand(node->sequence.first);
        run_command(node->sequence.second);
        break;
    case NODE_REDIRECT:
        break;
    case NODE_DETACH:
        break;
    case NODE_SUBSHELL:
        break;
    default:
        break;
    }

    if (prompt){
        char buffer[MAX_BUFFER_SIZE];
        getCommandLinePrompt(buffer);
        prompt = buffer;
    }
}   
