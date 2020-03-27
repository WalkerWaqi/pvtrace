# pvtrace
Add instrument.c in ttt.

pvtrace ttt

dot -Tjpg graph.dot -o graph.jpg

# cmake
```
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -finstrument-functions -fPIC -fpermissive")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -finstrument-functions -fPIC -fpermissive")
```
