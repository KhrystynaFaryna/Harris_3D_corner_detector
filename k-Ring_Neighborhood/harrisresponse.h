#ifndef HARRISRESPONSE_H
#define HARRISRESPONSE_H
#include<iostream>
#include<set>
#include <cstring>
#include <vector>


struct H_vertices{
    float x;
    float y;
    float z;
    std::set<int> adjacentpts;               // Container for Ring-1 Vertices Index
};

struct H_faces{
    int v1;
    int v2;
    int v3;
};

class HarrisResponse
{
    public:

    std::string Dir;                                // Directory of OFF file

    int nofaces;                                    // Total #faces in OFF Model
    int noverts;                                    // Total #vertices in OFF Model
    int norings;

    H_vertices* vertex;
    H_faces* face;
    HarrisResponse(std::string Dir);
    void getNeighborhood_prime();
    std::set<int> selected_neighborhood(int index);
    void display_ringhood(std::set<int> ringhood);
    std::set<int> getkRinghood(H_vertices vertex, int k);
    ~HarrisResponse();
};

#endif // HARRISRESPONSE_H





