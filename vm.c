// Ran Xu
// Spring 2020
// COP 3402 Systems Software
// Euripides Montagne
// HW 1, Virtual Machine

// library
#include <stdio.h>
#include <stdlib.h>
#define MAX_DATA_STACK_HEIGHT 40
#define MAX_CODE_LENGTH 200
#define MAX_LEXI_LEVELS 3
#define RF_SIZE 8

// struct variable to hold input instruction set
typedef struct {

  int op_code;
  int register_location;
  int lexi_level;
  int modifier;

} instruction;

// function declaration
int main(void);
int base(int l, int base, int data_stack[]);

// function
int main(void) {

  // PM/0 CPU registers to handle data and text segments
  int stack_pointer = 0;
  int base_pointer = 1;
  int program_counter = 0;
  instruction *instruction_register = NULL;

  // initial instruction register
  instruction_register = (instruction *) calloc(1, sizeof(instruction));

  // PM/0 CPU register files
  int register_file[RF_SIZE];

  // initial CPU register files
  for(int i=0;i<8;i++)
    register_file[i] = 0;

  // data stack
  int data_stack[MAX_DATA_STACK_HEIGHT + 1];

  // initial data stack values
  for(int i=0;i<MAX_DATA_STACK_HEIGHT + 1;i++)
    data_stack[i] = 0;
    
  // text stack
  instruction *text_stack = NULL;
  text_stack = (instruction *) calloc(MAX_CODE_LENGTH, sizeof(instruction));

  // set up input file and output file
  FILE *input_file = NULL;
  FILE *output_file = NULL;

  // input file and output file names
  input_file = fopen("input_vm.txt", "r");
  output_file = fopen("output_vm.txt", "w");

  // input file or output file error
  if (input_file == NULL || output_file == NULL) {

    printf("Error opening file.\n");
    return EXIT_FAILURE;

  }

  // printing initial to output and console
  fprintf(output_file, "Line\tOP\tR\tL\tM");
  printf("\t\tpc\tbp\tsp\tregisters\n");
  printf("Initial values\t%d\t%d\t%d\t", program_counter, base_pointer, stack_pointer);

  // printing initial registers
  for(int i = 0; i < RF_SIZE; i++)
    printf("%d ", register_file[i]);

  printf("\nStack: ");

  // printing initial data stack
  for(int i = 1; i < MAX_DATA_STACK_HEIGHT + 1; i++)
    printf("%d ", data_stack[i]);

  printf("\n");

  // keep track of lexi level
  int lexi_level = 0;

  // keep track of end of actavation record
  int lexi_array[MAX_LEXI_LEVELS + 1];

  // initial lexi array
  for(int i = 0; i < MAX_LEXI_LEVELS + 1; i++)
    lexi_array[i] = 0;
  
  // reading input file to text stack
  int line_num = 0;
  while(!feof(input_file)){

    // error, instructions over maximum
    if(line_num == MAX_CODE_LENGTH){
    
      printf("Error, number of instructions over the maximum\n");
      return EXIT_FAILURE;
    
    }
    
    // print line number
    fprintf(output_file, "\n%d\t", line_num);
    
    // read and print op code
    fscanf(input_file, "%d", &text_stack[line_num].op_code);
    
    if(text_stack[line_num].op_code == 1)
      fprintf(output_file, "lit\t");
      
    if(text_stack[line_num].op_code == 2)
      fprintf(output_file, "rtn\t");
            
    if(text_stack[line_num].op_code == 3)
      fprintf(output_file, "lod\t");
            
    if(text_stack[line_num].op_code == 4)
      fprintf(output_file, "sto\t");
            
    if(text_stack[line_num].op_code == 5)
      fprintf(output_file, "cal\t");
            
    if(text_stack[line_num].op_code == 6)
      fprintf(output_file, "inc\t");
            
    if(text_stack[line_num].op_code == 7)
      fprintf(output_file, "jmp\t");
            
    if(text_stack[line_num].op_code == 8)
      fprintf(output_file, "jpc\t");
            
    if(text_stack[line_num].op_code == 9)
      fprintf(output_file, "sio\t");
            
    if(text_stack[line_num].op_code == 10)
      fprintf(output_file, "sio\t");
            
    if(text_stack[line_num].op_code == 11)
      fprintf(output_file, "sio\t");
            
    if(text_stack[line_num].op_code == 12)
      fprintf(output_file, "neg\t");
            
    if(text_stack[line_num].op_code == 13)
      fprintf(output_file, "add\t");
            
    if(text_stack[line_num].op_code == 14)
      fprintf(output_file, "sub\t");
            
    if(text_stack[line_num].op_code == 15)
      fprintf(output_file, "mul\t");
            
    if(text_stack[line_num].op_code == 16)
      fprintf(output_file, "div\t");
            
    if(text_stack[line_num].op_code == 17)
      fprintf(output_file, "odd\t");
            
    if(text_stack[line_num].op_code == 18)
      fprintf(output_file, "mod\t");
            
    if(text_stack[line_num].op_code == 19)
      fprintf(output_file, "eql\t");
            
    if(text_stack[line_num].op_code == 20)
      fprintf(output_file, "neq\t");
            
    if(text_stack[line_num].op_code == 21)
      fprintf(output_file, "lss\t");
            
    if(text_stack[line_num].op_code == 22)
      fprintf(output_file, "leq\t");
            
    if(text_stack[line_num].op_code == 23)
      fprintf(output_file, "gtr\t");
            
    if(text_stack[line_num].op_code == 24)
      fprintf(output_file, "geq\t");
    
    // read and print register location
    fscanf(input_file, "%d", &text_stack[line_num].register_location);
    fprintf(output_file, "%d\t", text_stack[line_num].register_location);
    
    // read and print lexi level
    fscanf(input_file, "%d", &text_stack[line_num].lexi_level);
    fprintf(output_file, "%d\t", text_stack[line_num].lexi_level);
    
    // read and print modifier
    fscanf(input_file, "%d", &text_stack[line_num].modifier);
    fprintf(output_file, "%d", text_stack[line_num].modifier);
    
    // increase line
    line_num++;

  }

  fprintf(output_file, "\n");
  
  // fetch and execute cycle
  while(1){
  
    // fetch instruction cycle
    instruction_register[0].op_code = text_stack[program_counter].op_code;
    instruction_register[0].register_location = text_stack[program_counter].register_location;
    instruction_register[0].lexi_level = text_stack[program_counter].lexi_level;
    instruction_register[0].modifier = text_stack[program_counter].modifier;

    // printing instruction to be execute to console
    printf("\t\tpc\tbp\tsp\tregisters\n");
    printf("%d ", program_counter);
    
    if(instruction_register[0].op_code == 1)
      printf("lit ");
      
    if(instruction_register[0].op_code == 2)
      printf("rtn ");
            
    if(instruction_register[0].op_code == 3)
      printf("lod ");
            
    if(instruction_register[0].op_code == 4)
      printf("sto ");
            
    if(instruction_register[0].op_code == 5)
      printf("cal ");
            
    if(instruction_register[0].op_code == 6)
      printf("inc ");
            
    if(instruction_register[0].op_code == 7)
      printf("jmp ");
            
    if(instruction_register[0].op_code == 8)
      printf("jpc ");
            
    if(instruction_register[0].op_code == 9)
      printf("sio ");
            
    if(instruction_register[0].op_code == 10)
      printf("sio ");
            
    if(instruction_register[0].op_code == 11)
      printf("sio ");
            
    if(instruction_register[0].op_code == 12)
      printf("neg ");
            
    if(instruction_register[0].op_code == 13)
      printf("add ");
            
    if(instruction_register[0].op_code == 14)
      printf("sub ");
            
    if(instruction_register[0].op_code == 15)
      printf("mul ");
            
    if(instruction_register[0].op_code == 16)
      printf("div ");
            
    if(instruction_register[0].op_code == 17)
      printf("odd ");
            
    if(instruction_register[0].op_code == 18)
      printf("mod ");
            
    if(instruction_register[0].op_code == 19)
      printf("eql ");
            
    if(instruction_register[0].op_code == 20)
      printf("neq ");
            
    if(instruction_register[0].op_code == 21)
      printf("lss ");
            
    if(instruction_register[0].op_code == 22)
      printf("leq ");
            
    if(instruction_register[0].op_code == 23)
      printf("gtr ");
            
    if(instruction_register[0].op_code == 24)
      printf("geq ");
      
    // print R L M of the fetched instruction
    printf("%d %d %d\t",instruction_register[0].register_location,instruction_register[0].lexi_level,instruction_register[0].modifier);

    // increase program counter
    program_counter++;
    
    // execute cycle
    // lit, load constant M into R
    if(instruction_register[0].op_code == 1)
      register_file[instruction_register[0].register_location] = instruction_register[0].modifier;
    
    // rtn, restore caller environment
    if(instruction_register[0].op_code == 2){
      
      // erase the stack pointer
      lexi_array[lexi_level] = 0;

      // decrease the lexi level
      lexi_level--;
    
      stack_pointer = base_pointer - 1;
      base_pointer = data_stack[stack_pointer + 3];
      program_counter = data_stack[stack_pointer + 4];
    
    }
    
    // lod, load value to R at L lexi levels down with M offset
    if(instruction_register[0].op_code == 3)
      register_file[instruction_register[0].register_location] = data_stack[base(instruction_register[0].lexi_level, base_pointer, data_stack) + instruction_register[0].modifier];
    
    // sto, store value to R at L lexi levels down with M offset
    if(instruction_register[0].op_code == 4)
      data_stack[base(instruction_register[0].lexi_level, base_pointer, data_stack) + instruction_register[0].modifier] = register_file[instruction_register[0].register_location];
    
    // cal, call procedure at code index M
    if(instruction_register[0].op_code == 5){
    
      // space to return value
      data_stack[stack_pointer + 1] = 0;

      // static link
      data_stack[stack_pointer + 2] = base(instruction_register[0].lexi_level, base_pointer, data_stack);

      // dynamic link
      data_stack[stack_pointer + 3] = base_pointer;
      
      // return address
      data_stack[stack_pointer + 4] = program_counter;
      
      base_pointer = stack_pointer + 1;
      program_counter = instruction_register[0].modifier;
    
    }
    
    // inc, allocate M locals
    if(instruction_register[0].op_code == 6){
      
      // increase lexi level
      lexi_level++;
      
      // error, if lexi level is over the limit
      if(lexi_level > MAX_LEXI_LEVELS){

        printf("Error, lexi level is over the maximum\n");
        return EXIT_FAILURE;

      }

      // keep track of stack pointer of current activation record
      lexi_array[lexi_level] = stack_pointer;

      stack_pointer = stack_pointer + instruction_register[0].modifier;

      // error, stack pointer is over the limit
      if(stack_pointer > MAX_DATA_STACK_HEIGHT + 1){

        printf("Error, height of the data stack is over the maximum\n");
        return EXIT_FAILURE;

      }

    }
    
    // jmp, jump to instruction M
    if(instruction_register[0].op_code == 7)
      program_counter = instruction_register[0].modifier;
    
    // jpc, jump to instruction M if R is 0
    if(instruction_register[0].op_code == 8)
      if(register_file[instruction_register[0].register_location] == 0)
        program_counter = instruction_register[0].modifier;
    
    // sio, write R
    if(instruction_register[0].op_code == 9)
      printf("%d ",register_file[instruction_register[0].register_location]);

    // sio, read R
    if(instruction_register[0].op_code == 10)
      scanf("%d",&register_file[instruction_register[0].register_location]);

    // neg, R[i] = -R[i]
    if(instruction_register[0].op_code == 12)
      register_file[instruction_register[0].register_location] = -1 * register_file[instruction_register[0].register_location];

    // add, R[i] = R[j] + R[k]
    if(instruction_register[0].op_code == 13)
      register_file[instruction_register[0].register_location] = register_file[instruction_register[0].lexi_level] + register_file[instruction_register[0].modifier];
    
    // sub, R[i] = R[j] - R[k]
    if(instruction_register[0].op_code == 14)
      register_file[instruction_register[0].register_location] = register_file[instruction_register[0].lexi_level] - register_file[instruction_register[0].modifier];
    
    // mul, R[i] = R[j] * R[k]
    if(instruction_register[0].op_code == 15)
      register_file[instruction_register[0].register_location] = register_file[instruction_register[0].lexi_level] * register_file[instruction_register[0].modifier];
    
    // div, R[i] = R[j] / R[k]
    if(instruction_register[0].op_code == 16)
      register_file[instruction_register[0].register_location] = register_file[instruction_register[0].lexi_level] / register_file[instruction_register[0].modifier];
    
    // odd, R[i] = R[i] % 2
    if(instruction_register[0].op_code == 17)
      register_file[instruction_register[0].register_location] = register_file[instruction_register[0].register_location] % 2;
    
    // mod, R[i] = R[j] % R[k]
    if(instruction_register[0].op_code == 18)
      register_file[instruction_register[0].register_location] = register_file[instruction_register[0].lexi_level] % register_file[instruction_register[0].modifier];
    
    // eql, R[i] = R[j] == R[k]
    if(instruction_register[0].op_code == 19)
      if(register_file[instruction_register[0].lexi_level] == register_file[instruction_register[0].modifier])
        register_file[instruction_register[0].register_location] = 1;
      
      else
        register_file[instruction_register[0].register_location] = 0;

    // neq, R[i] = R[j] != R[k]
    if(instruction_register[0].op_code == 20)
      if(register_file[instruction_register[0].lexi_level] != register_file[instruction_register[0].modifier])
        register_file[instruction_register[0].register_location] = 1;
      
      else
        register_file[instruction_register[0].register_location] = 0;

    // lss, R[i] = R[j] < R[k]
    if(instruction_register[0].op_code == 21)
      if(register_file[instruction_register[0].lexi_level] < register_file[instruction_register[0].modifier])
        register_file[instruction_register[0].register_location] = 1;
      
      else
        register_file[instruction_register[0].register_location] = 0;
    
    // leq, R[i] = R[j] <= R[k]
    if(instruction_register[0].op_code == 22)
      if(register_file[instruction_register[0].lexi_level] <= register_file[instruction_register[0].modifier])
        register_file[instruction_register[0].register_location] = 1;
        
      else
        register_file[instruction_register[0].register_location] = 0;

    // gtr, R[i] = R[j] > R[k]
    if(instruction_register[0].op_code == 23)
      if(register_file[instruction_register[0].lexi_level] > register_file[instruction_register[0].modifier])
        register_file[instruction_register[0].register_location] = 1;
      
      else
        register_file[instruction_register[0].register_location] = 0;
    
    // geq, R[i] = R[j] >= R[k]
    if(instruction_register[0].op_code == 24)
      if(register_file[instruction_register[0].lexi_level] >= register_file[instruction_register[0].modifier])
        register_file[instruction_register[0].register_location] = 1;
      
      else
        register_file[instruction_register[0].register_location] = 0;
    
    // printing pc bp sp after execution
    printf("%d\t%d\t%d\t", program_counter, base_pointer, stack_pointer);
    
    // printing register file after execution
    for(int i = 0; i < RF_SIZE; i++)
     printf("%d ", register_file[i]);

    printf("\nStack: ");

    // printing stack after execution
    for(int i = 1; i <= stack_pointer; i++){

      printf("%d ", data_stack[i]);

      for(int j = 1; j <= MAX_LEXI_LEVELS; j++)
        if(i == lexi_array[j])
          printf(" | ");

    }
    
    printf("\n");

    // sio, end
    if(instruction_register[0].op_code == 11)
      break;
  
  }

  // free memory for text stack and set null pointer
  free(text_stack);
  text_stack = NULL;

  // free memory for instruction register and set null pointer
  free(instruction_register);
  instruction_register = NULL;

  // close file and set null pointer
  fclose(input_file);
  fclose(output_file);
  input_file = NULL;
  output_file = NULL;

  return EXIT_SUCCESS;

}

// find base L levels down
// l stand for L in the instruction format
int base(int l, int base, int data_stack[]) {

  // find base L levels down
  int b1;

  b1 = base;

  for(;l > 0;l--)
    b1 = data_stack[b1 + 1];

  return b1;
  
}