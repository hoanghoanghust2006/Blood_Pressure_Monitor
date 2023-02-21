/*
 * Title : < Name of the file. >
 * Copyright : HCL
 * Author : < Name of the author. >
 * Creation Date : < Date in DD/MMM/YY format >
 * ------- ---------- --------
 */

/* System Include -----------------------------------------------------------------------*/

/* Local Include ------------------------------------------------------------------------*/
#include <stdio.h>
#include "common.h"
#include "trace.h"
#include "menu.h"
#include <string.h>
/* Private define constants -------------------------------------------------------------*/

/* Private macros -----------------------------------------------------------------------*/

/* Private type definitions  ------------------------------------------------------------*/

/* Private file-local global variables   ------------------------------------------------*/

/* Private function prototypes declarations   -------------------------------------------*/

/* Private functions definition   -------------------------------------------------------*/
void option_create(option* option_val, char* name, void (*DoWork)())
{
    strcpy(option_val->name, name);
    option_val->current_index  = 0;
    option_val->size           = 0;
    option_val->DoWork         = DoWork;
    option_val->parent         = NULL;
    option_val->option_list[0] = NULL;
}

void option_add_link(option* parent, option* child)
{
    child->parent                     = parent;
    parent->option_list[parent->size] = child;
    parent->size++;
}
/* Export functions definition   --------------------------------------------------------*/
