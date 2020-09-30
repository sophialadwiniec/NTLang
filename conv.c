/* conv.c - base conversion functions */

#include "ntcalc.h"

uint32_t conv_binstr_to_uint32(char *binstr) {
    uint32_t result = 0;
    int i = 0;

    while (binstr[i] != '\0') {
        result *= 2;
        result += binstr[i] - '0';
        i += 1;
        if (i >= 32) {
            printf("BINLIT too big\n");
            exit(-1);
        }     
    }
    return result;    
}

uint32_t conv_hexstr_to_uint32(char *hexstr) {
    uint32_t result = 0;
    int len = strlen(hexstr);
    int base = 1;  

    for (int i=len-1; i>=0; i--) 
    {    
        if (hexstr[i]>='0' && hexstr[i]<='9'){ 
            result += (hexstr[i] - 48)*base; 
        }else if (hexstr[i]>='a' && hexstr[i]<='f'){ 
            result += (hexstr[i] - 87)*base;   
        }else if (hexstr[i]>='A' && hexstr[i]<='F'){ 
            result += (hexstr[i] - 55)*base; 
        } 
        base = base*16; 
    } 
    return result; 
}

uint32_t conv_decstr_to_uint32(char *decstr){
    int result = 0; 
    int base = 1; 
    int len = strlen(decstr); 
    for(int i = len-1; i >=0; i--){
      result += (decstr[i] - 48)* base; 
      base*=10; 
    }
  return result; 
}

void conv_uint32_to_binstr(uint32_t v, char *binstr, int width) {
    int i, j, bit;
    binstr[0] = '0';
    binstr[1] = 'b';
    j = 2;

    for (i = width-1; i >= 0; i--) {
        bit = (v >> i) & 0b1;
        binstr[j] = '0' + bit;
        j += 1;         
    }
    binstr[j] = '\0';
}

void conv_uint32_to_hexstr(uint32_t v, char *hexstr, int width) {
    int i, j, nibble; 
    hexstr[0] = '0'; 
    hexstr[1] = 'x'; 
    j = 2; 
    width-=4; 

    for(i = width; i >=0; i-=4){
        nibble = (v >> i) & 0xF; 
        if(nibble >= 0 && nibble <= 9){
             hexstr[j] = '0' + nibble; 
        }else if(nibble>= 10 && nibble <= 15){
            hexstr[j] = 'A' + (nibble-10); 
        }
        j+=1; 
    }
    hexstr[j] = '\0';  
}

int set_mask(int width){
    switch(width){
        case 16:
            return 0xFFFF; 
        case 8: 
            return 0xFF; 
        case 4: 
            return 0xF; 
    }
}

int32_t power(int base, int exp){
    int result = 1; 
    for (exp; exp>0; exp--){
        result = result * base; 
    }
    return result; 
}

void conv_uint32_to_decstr(uint32_t v, char *decstr, bool u, int width) {
    int j = 0, v_length = 0, value = 0, i; 
    //if it's a signed value 
    if(!u) {
        int32_t v1 = (int32_t) v;
        //check if msb is 1, if it's 1 you know the value is negative 
        if(((v1 >> (width -1)) & 1) == 1){
            v1 = ((~v1 + 1));
            decstr[j] ='-'; 
            j+=1;
        }
        v = v1;
    }
    if(width < 32){
        v = v & set_mask(width); 
    } 
    for(i = 0; i < 10; i++){
        value = power(10, i); 
        if(value > v){
            v_length = i; 
            break; 
        }
    }    
    for(int i = v_length-1; i >= 0; i--){
        value = v / power(10,i); 
        value = value % 10; 
        decstr[j] = '0'+ value; 
        j+=1;
    }
    if (v_length == 0){
        decstr[j] = '0';
        j+=1; 
    }
    decstr[j] = '\0';   
}