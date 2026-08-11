

#include <core.h>

void print_bits(uint32_t value){

    for(int i = 31; i >= 0; i--){
        printf("PIN %d : %d\n", i, (value & BITS_L_SHITF(1, i)) >> i);
        /*
      if(i == 0){
        printf("PIN %d : %d\n", i, (value & BITS_L_SHITF(1, i)) >> i);
      }else{
        printf("%d\t", (value & BITS_L_SHITF(1, i)) >> i);
      }*/
    }
}