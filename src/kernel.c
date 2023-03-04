#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include "idt/idt.h"
#include "memory/heap/kheap.h"

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t get_char_to_write(char c, char color){
    return (color << 8) | c;
}


void write_char_to_terminal(char c, char color){
    if(c == '\n'){
        terminal_row += 1;
        terminal_col = 0;
        return;
    }

    video_mem[(terminal_row * VGA_WIDTH) + terminal_col] = get_char_to_write(c, color);

    terminal_col += 1;

    if(terminal_col >= VGA_WIDTH){
        terminal_row += 1;
        terminal_col = 0;
    }
}

void initalize_terminal(){
    video_mem = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;

    for(int y = 0; y < VGA_HEIGHT; y++){
        for(int x = 0; x < VGA_WIDTH; x++){
            video_mem[(y * VGA_WIDTH) + x] = get_char_to_write(' ', 0);
        }
    }
}

size_t get_str_len(const char* str){
    size_t len = 0;
    while (str[len])
    {
        len++;
    }

    return len;    
}

void print(const char* str){
    size_t len = get_str_len(str);

    for(int i = 0; i < len; i++){
        write_char_to_terminal(str[i], 15);
    }
}


void kernel_main(){
    initalize_terminal();
    
    // Initialize the heap
    print("Initalizing heap...\n");
    kheap_init();
    print("Initalizing interrupt discriptor table...\n");
    idt_init();
    print("All Set...\n");
    print("Howdy, techies\nWelcome!!!");
}

