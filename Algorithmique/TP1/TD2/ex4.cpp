

#include <iostream>


int PGCD_recur(int a, int b){
    if (b == 0) return a;
    return PGCD_recur(b, a%b);
}

int PGCD(int a, int b){
    int highest = std::max(a, b);
    int lowest = std::min(a, b);
    return PGCD_recur(highest, lowest);
}
int main(){
    std::cout<<PGCD(41, 13)<<std::endl;
    return 0;
}