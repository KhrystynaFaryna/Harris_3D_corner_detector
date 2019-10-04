#include<fstream>
#include<iostream>
#include<set>
#include<algorithm>
#include<vector>
#include "myglwidget.h"
#include "harrisresponse.h"

using namespace std;

HarrisResponse::HarrisResponse(string offDir)
{
    ifstream OFF_file(offDir);

    float temp0,temp1,temp2,temp3;
    int mem_loc=0;

    int no_vert;
    int no_face;
    int no_edges;

    string ModelType;

    if(OFF_file>>ModelType>>no_vert>>no_face>>no_edges)

    {
        cout<<"The model type is: "<<ModelType<<endl;
        cout<<"The number of vertices is: "<<no_vert<<endl;
        cout<<"The number of faces is: "<<no_face<<endl;
        cout<<"The number of edges is: "<<no_edges<<endl<<endl;
    }

    vertex = new H_vertices[no_vert];
    face = new H_faces[no_face];

    this->noverts  = no_vert;
    this->nofaces = no_face;

    for (mem_loc=0; mem_loc<=no_vert-1; mem_loc++)
    {
        OFF_file>>temp1>>temp2>>temp3;
        vertex[mem_loc].x=temp1;
        vertex[mem_loc].y=temp2;
        vertex[mem_loc].z=temp3;
    }

    for (mem_loc=0; mem_loc<=no_face-1; mem_loc++)
    {
        OFF_file>>temp0>>temp1>>temp2>>temp3;
        face[mem_loc].v1=temp1;
        face[mem_loc].v2=temp2;
        face[mem_loc].v3=temp3;
    }


    this->vertex=vertex;
    this->face=face;

    OFF_file.close();


}

void HarrisResponse::getNeighborhood_prime()
{
    // Load Ring-1
    for (int i=0; i<noverts; i++)
    {
        for (int j=0; j<nofaces; j++)
        {
            if ((face[j].v1 == i) || (face[j].v2 == i) || (face[j].v3 == i))
            {
                vertex[i].adjacentpts.insert(face[j].v1);
                vertex[i].adjacentpts.insert(face[j].v2);
                vertex[i].adjacentpts.insert(face[j].v3);
            }
        }
    }



    // Display
    /* set<int>::iterator it;
    for (int i=0; i<noverts; i++)
    {
        cout<<"Ring #"<<1<<" for Vertex #"<<i<<":"<<endl;
        for (it =vertex[i].adjacentpts.begin(); it!=vertex[i].adjacentpts.end(); it++)

            cout<<*it<<" ";
        cout<<endl<<endl;
    }*/
}

std::set<int> HarrisResponse::selected_neighborhood(int index)
{
    set<int> ringhood ;
    for(int i=0; i<nofaces;i++)
    {

        if(face[i].v1 == index)
        {
            ringhood.insert(face[i].v2);
            ringhood.insert(face[i].v3);
        }
        else if(face[i].v2 == index)
        {
            ringhood.insert(face[i].v1);
            ringhood.insert(face[i].v3);
        }
        else if(face[i].v3 == index)
        {
            ringhood.insert(face[i].v1);
            ringhood.insert(face[i].v2);
        }
    }


    return ringhood;
}

void HarrisResponse::display_ringhood(std::set<int> ringhood)
{
    set<int>::iterator it;
    for(it = ringhood.begin(); it!=ringhood.end(); it++)
    {
        cout<<*it<<" ";
    }

}

std::set<int> HarrisResponse::getkRinghood(H_vertices vertex, int k)
{
    HarrisResponse x(Dir);
    set<int>::iterator ita,itx, itp;
    set<int> tmp, ringhood;
    vector<int> tmp2;
    // k = norings;
    set<int> k_ringhood, final_ring,k1_ringhood;
    set<int>::iterator it;

    if(k ==0){
        cout<<"Error. K has to be a non zero positive integer."<<endl;
        EXIT_FAILURE;

    }
    else if(k == 1)
    {
        return vertex.adjacentpts;
    }
    else if (k > 1){
        // number of ring = m+1
        for(int m = 1; m<k; m++) {
            if(m ==1 && k ==2){
                for(it = vertex.adjacentpts.begin(); it!=vertex.adjacentpts.end(); it++)
                {
                    int o = *it;
                    set<int>ringhood = selected_neighborhood(o);

                    for(set<int>::iterator iti = ringhood.begin(); iti != ringhood.end();iti++)
                    {
                        int j = *iti;

                        k_ringhood.insert(j);

                    }

                }
                return  k_ringhood;

            }
            if(m == 1)
            {
                for(it = vertex.adjacentpts.begin(); it!=vertex.adjacentpts.end(); it++)

                {
                    int o = *it;
                    set<int>ringhood = selected_neighborhood(o);
                    for(set<int>::iterator iti = ringhood.begin(); iti != ringhood.end();iti++)
                    {
                        int j = *iti;

                        k_ringhood.insert(j);

                    }

                }
            }
            else if(m>1)
            {

                final_ring = k_ringhood;
                for(ita = k_ringhood.begin(); ita!=k_ringhood.end(); ita++)

                {
                    int o = *ita;
                    tmp = selected_neighborhood(o);
                    for(itx = tmp.begin(); itx!=tmp.end(); itx++)
                    {
                        int l = *itx;
                        ringhood.insert(l);
                    }

                }

                for(set<int>::iterator iti = ringhood.begin(); iti != ringhood.end();iti++)
                {
                    int j = *iti;

                    final_ring.insert(j);

                }

                k_ringhood= final_ring;
            }

            if(m == k-1)
            {
                return final_ring;
            }
        }
    }



}



HarrisResponse::~HarrisResponse()
{

}
