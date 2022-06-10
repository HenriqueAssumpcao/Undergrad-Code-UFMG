#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include "doctest.h"
#include "../include/buffer.h"


TEST_CASE("Buffer structure"){
    Buffer<int> buffer;
    Item<int> aux;
    for(int i = 0; i < 10; i++){
        buffer.push_back(i);
    }
    for(int i = 0; i < 10; i++){
        aux = buffer.pop_front();
        CHECK(aux.content == i);
    }
    CHECK(buffer.is_empty());
    buffer.clear();
    for(int i = 0; i < 10; i++){
        buffer.push_front(i);
    }
    for(int i = 9; i >= 0; i--){
        aux = buffer.pop_front();
        CHECK(aux.content == i);
    }
    CHECK(buffer.is_empty());
    buffer.clear();
    for(int i = 0; i < 10; i++){
        buffer.push_back(i);
    }
    aux = buffer.pop_pos(5);
    CHECK(aux.content == 5);
}
