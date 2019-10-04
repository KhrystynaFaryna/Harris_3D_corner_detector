#include<fstream>
#include<iostream>
#include<set>
#include <functional>
#include <map>
#include<algorithm>
#include<vector>
#include "myglwidget.h"
#include "harrisresponse.h"

#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

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

void HarrisResponse::computeHarrisResponse(int rings, double H)
{
    cout<<"......Calculating the Harris Response for each point in the Mesh......"<<endl;
    for(int c = 0; c < noverts ; c++)
    {
        float x_center = 0, y_center = 0, z_center = 0;
        float x_center_trans = 0, y_center_trans = 0, z_center_trans = 0;

        int counter = 0;

        cout<<"Vertex: "<<c<<" of"<<noverts<<endl;
        // Calculating the neighborhood for the vertex
        std::set<int> temp_neighborhood = getkRinghood(vertex[c], rings);

        // new stuct variable for storing the neighborhood as a separate object
        harrisvertex = new H_vertices[temp_neighborhood.size()];

        //array to hold the indexes of the neighborhood vertices
        int neighborhood_array[temp_neighborhood.size()];

        //filling the array:
        for(std::set<int>::iterator it = temp_neighborhood.begin(); it != temp_neighborhood.end(); it++)
        {
            int val = *it;
            neighborhood_array[counter++] = val;
        }
        counter = 0;
        //populating the harrisvertex with the x,y,z of the neighborhood points
        for(int i = 0; i< temp_neighborhood.size(); i++)
        {
            harrisvertex[i].x = vertex[neighborhood_array[i]].x;
            harrisvertex[i].y = vertex[neighborhood_array[i]].y;
            harrisvertex[i].z = vertex[neighborhood_array[i]].z;

        }

        //Centroid calcualation for Original Neighbourhood:
        for(std::set<int>::iterator iti = temp_neighborhood.begin(); iti != temp_neighborhood.end(); iti++)
        {
            int j = *iti;

            x_center = x_center + vertex[j].x;
            y_center = y_center + vertex[j].y;
            z_center = z_center + vertex[j].z;

        }

        x_center = x_center/temp_neighborhood.size();
        y_center = y_center/temp_neighborhood.size();
        z_center = z_center/temp_neighborhood.size();
        //translating the harrisvertex points to origin:
        for(int i = 0; i < temp_neighborhood.size(); i++)
        {
            harrisvertex[i].x = harrisvertex[i].x - x_center ;
            harrisvertex[i].y = harrisvertex[i].y - y_center ;
            harrisvertex[i].z = harrisvertex[i].z - z_center ;
        }
        //testing the center of the translated points:
        for(int j = 0; j < temp_neighborhood.size(); j++)
        {
            x_center_trans = x_center_trans + harrisvertex[j].x;
            y_center_trans = y_center_trans + harrisvertex[j].y;
            z_center_trans = z_center_trans + harrisvertex[j].z;

        }
        x_center_trans = x_center_trans/temp_neighborhood.size();
        y_center_trans = y_center_trans/temp_neighborhood.size();
        z_center_trans = z_center_trans/temp_neighborhood.size();
        //Compute Covariance Matrix
        double A[9];
        for (register int i=0; i<9; i++)
            A[i]=0;
        for(register int i=0; i<temp_neighborhood.size(); i++)
        {
            float x = harrisvertex[i].x;
            float y = harrisvertex[i].y;
            float z = harrisvertex[i].z;

            A[0] += x*x;	   A[1] += x*y;	   A[2] += x*z;
            A[4] += y*y;	   A[5] += y*z;
            A[8] += z*z;
        }
        A[3]=A[1];	A[6]=A[2];	A[7]=A[5];
        // Calculate PCA using Covariance Martix (A)
        gsl_matrix_view m = gsl_matrix_view_array(A, 3, 3);
        gsl_vector* eval = gsl_vector_alloc(3);
        gsl_matrix* evec = gsl_matrix_alloc(3, 3);

        gsl_eigen_symmv_workspace* w = gsl_eigen_symmv_alloc(3);
        gsl_eigen_symmv(&m.matrix, eval, evec, w);
        gsl_eigen_symmv_free(w);
        // Order Eigenvectors decreasingly by Eigenvalues
        gsl_eigen_symmv_sort(eval, evec, GSL_EIGEN_SORT_VAL_DESC);
        // Export components of the New Coordinate System
        double x_1 = gsl_matrix_get(evec, 0, 0);	double x_2 = gsl_matrix_get(evec, 1, 0); double x_3 = gsl_matrix_get(evec, 2, 0);
        double y_1 = gsl_matrix_get(evec, 0, 1);	double y_2 = gsl_matrix_get(evec, 1, 1); double y_3 = gsl_matrix_get(evec, 2, 1);
        double z_1 = gsl_matrix_get(evec, 0, 2);	double z_2 = gsl_matrix_get(evec, 1, 2); double z_3 = gsl_matrix_get(evec, 2, 2);
        //Plane-Fitting
        double x2 = harrisvertex[0].x - x_center;
        double y2 = harrisvertex[0].y - y_center;
        double z2 = harrisvertex[0].z - z_center;
        if((z_1*x2 + z_2*y2 + z_3*z2) < 0){
            z_1 = -z_1;
            z_2 = -z_2;
            z_3 = -z_3;

            double aux_x1 = x_1;
            double aux_x2 = x_2;
            double aux_x3 = x_3;

            x_1 = y_1;
            x_2 = y_2;
            x_3 = y_3;
            y_1 = aux_x1;
            y_2 = aux_x2;
            y_3 = aux_x3;
        }

        // Rotation using New Coordinate System
        for(register int i=0; i<temp_neighborhood.size(); i++)
        {
            double x = harrisvertex[i].x;
            double y = harrisvertex[i].y;
            double z = harrisvertex[i].z;

            harrisvertex[i].x = x_1*x + x_2*y + x_3*z;
            harrisvertex[i].y = y_1*x + y_2*y + y_3*z;
            harrisvertex[i].z = z_1*x + z_2*y + z_3*z;
        }

        // Shift all the points so that the Point of Analysis is at the Origin
        // of the XY plane. We only move in the X and Y coordinates.

        double x = harrisvertex[0].x;
        double y = harrisvertex[0].y;

        for(register int i=0; i<temp_neighborhood.size(); i++)
        {
            harrisvertex[i].x = harrisvertex[i].x - x;
            harrisvertex[i].y = harrisvertex[i].y - y;
        }

        double C[36];
        for (register int i=0; i<36; i++)
            C[i]=0;
        double D[6];
        for (register int i=0; i<6; i++)
            D[i]=0;

        for(register int i=0; i<temp_neighborhood.size(); i++)
        {
            double x = harrisvertex[i].x;
            double y = harrisvertex[i].y;
            double z = harrisvertex[i].z;

            double x2 = x*x;
            double y2 = y*y;
            double x3 = x2*x;
            double y3 = y2*y;

            C[0]  += x*x3;	C[1]  += x3*y;	C[2]  += x2*y2;	C[3] += x3;		C[4] += x2*y;	C[5] += x2;
            C[7]  += x2*y2;	C[8]  += x*y3;	C[9]  += x2*y;	C[10] += x*y2;	C[11] += x*y;
            C[14] += y*y3;	C[15] += x*y2;	C[16] += y3;	C[17] += y2;
            C[21] += x2;	C[22] += x*y;	C[23] += x;
            C[28] += y2;	C[29] += y;
            D[0]  += z*x2;	D[1]  += z*x*y;	D[2]  += z*y2;	D[3] += z*x;	D[4] += z*y;	D[5] += z;
        }

        C[6]  = C[1];
        C[12] = C[2];	C[13] = C[8];
        C[18] = C[3];	C[19] = C[9];	C[20] = C[15];
        C[24] = C[4];	C[25] = C[10];	C[26] = C[16];	C[27] = C[22];
        C[30] = C[5];	C[31] = C[11];	C[32] = C[17];	C[33] = C[23];	C[34] = C[29];

        C[35] = (double)temp_neighborhood.size();

        //Solve Linear Systems using GSL
        gsl_matrix_view m1 = gsl_matrix_view_array(C, 6, 6);
        gsl_vector_view b1 = gsl_vector_view_array(D, 6);
        gsl_vector *x1 = gsl_vector_alloc(6);

        int s1;

        gsl_permutation *p11 = gsl_permutation_alloc(6);
        gsl_linalg_LU_decomp(&m1.matrix, p11, &s1);
        gsl_linalg_LU_solve(&m1.matrix, p11, &b1.vector, x1);

        //Extract Solution of Quadratic Surface
        double c20_2 = gsl_vector_get(x1, 0);
        double c11 = gsl_vector_get(x1, 1);
        double c02_2 = gsl_vector_get(x1, 2);
        double c10 = gsl_vector_get(x1, 3);
        double c01 = gsl_vector_get(x1, 4);
        double c0 = gsl_vector_get(x1, 5);

        double c20 = c20_2*2;
        double c02 = c02_2*2;

        double fx2 = c10*c10 + 2*c20*c20 + 2*c11*c11;   //A
        double fy2  = c10*c10 + 2*c11*c11 + 2*c02*c02;  //B
        double fxfy = c10*c01 + 2*c20*c11 + 2*c11*c02;  //C

        //Calculate Harris Response
        long double resp = fx2*fy2 - fxfy*fxfy - H*(fx2 + fy2)*(fx2 + fy2);

        vertex[c].response = resp;
        //std::cout<<"Res:"<<c<<": "<<resp<<endl;
        //  std::cout<<"Coordinates: "<<vertex[c].x<<" , "<<vertex[c].y<<" , "<<vertex[c].z<<endl;



        gsl_vector_free(x1);
        gsl_vector_free(eval);
        gsl_matrix_free(evec);
        gsl_permutation_free(p11);


        temp_neighborhood.clear();

        delete[] harrisvertex;

    }
}

void HarrisResponse::updateadjpts()
{
    for(int i =1; i<noverts;i++){

        for (std::set<int>::iterator it= vertex[i].adjacentpts.begin();it != vertex[i].adjacentpts.end(); it++) {
            int val = *it;
            if(val ==i)
            {
                vertex[i].adjacentpts.erase(it);
            }
        }
    }
}

double HarrisResponse::calcmax(float a[], int n )
{
    double res = 0.0;
    for(int i =0;i<n;i++)
    {
        if(*(a+i)>res)
        {
            res = *(a+i);
        }
    }
    return res;
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
void swap(float *xp, float *yp)
{
    float temp = *xp;
    *xp = *yp;
    *yp = temp;
}
void HarrisResponse::findcandidates(std::string filename, std::string filecan)
{
    updateadjpts();
    float fract ;
    if(noverts >9999)
    {
        fract = 0.25;
    }
    else
    {
        fract = 0.05;
    }
    cout<<"Calculating the candidates and selecting the interest points."<<endl;
    int tot_selected = round( fract* noverts) ;
    cout<<"The fraction will be: "<<fract<<" and the tot interest point detected will be: "<<tot_selected<<endl;

    std::vector<H_vertices>result;
    for(int i =0; i< noverts; i++)
    {
        float re1 = vertex[i].response;

        int c =0;
        float *compare = new float[vertex[i].adjacentpts.size()];
        for( std::set<int>::iterator it = vertex[i].adjacentpts.begin(); it!= vertex[i].adjacentpts.end();it++)
        {
            int j = *it;
            *(compare +c++)=vertex[j].response;

        }
        double k = calcmax(compare, vertex[i].adjacentpts.size());
        if(re1 >k)
        {
            //the dimension of the final array increases.
            // cand[conter] = i;
            //dim++;
            candidate_v.insert(i);
            //result.push_back(vertex[i]);
        }
        delete [] compare;
    }


          std::vector<int> tmp;
    std::set<int>::iterator xa;
    int pos =0;
    //creating 2 aray
    float *r = new float [candidate_v.size()];
    int *v = new int [candidate_v.size()];
    //populating
    for(xa = candidate_v.begin(); xa != candidate_v.end(); xa++)
    {
        int val = *xa;
        *(r+pos) = vertex[val].response;
        *(v+pos) = val;
        pos++;
    }
    //ordering
    for(int i =0; i<candidate_v.size(); i++)
    {
        for(int j =0; j<candidate_v.size()-i-1;j++)
        {
            if(*(r+j)< *(r+j+1))
            {
                swap(&r[j], &r[j+1]);
                swap(&v[j], &v[j+1]);
            }
        }
    }
    std::vector<int> final_pts;
    for(int i=0; i<tot_selected;i++)
    {
        int val = *(v+i);
        final_pts.push_back(val);
    }

    ofstream file;

    ofstream can;

    can.open(filecan);
    if(can)
    {
        cout<<"I opened the file: "<<filecan<<" To store the candidates"<<endl;



        for(int i=0; i<candidate_v.size(); i++)
        {
            int val = *(v+i);
            can << val<< "\n";
        }
    }
    else
    {
        cout<<"Error in Opening File."<<endl;


    }


    cout<<"I find them."<<endl;
    can.close();
    file.open(filename);
    if(file)
    {
        cout<<"I opened the file: "<<filename<<" to store the interest points."<<endl;

        for(int i =0; i<final_pts.size() && i<candidate_v.size();i++)
        {
            int val = final_pts.at(i);
            file<< val<< "\n";
        }
    }
    else
    {
        cout<<"Error in Opening File."<<endl;

    }


    cout<<"I find them."<<endl;
    file.close();

    delete [] r;
    delete [] v;
}

HarrisResponse::~HarrisResponse()
{

}
