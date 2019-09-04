#include "Region.h"

#ifndef GRID_OF_REGIONS
#define GRID_OF_REGIONS

class GridOfRegions{
    private:
        int GridWidth;
        int GridHight;
        int NumberOfRegions;
    public:
        Region *Regions;
        int *Grid;

        GridOfRegions(int g_width, int g_hight, int n_regions);
        ~GridOfRegions();

        void InsertRegion(int r, int c, int reg);
        bool IsOverlapping();
        bool IsFilled();
        bool AreRegionsEqual();
        void PrintToConsole();
        void CreateRegions(int seed);
};

#endif

GridOfRegions::GridOfRegions(int g_width, int g_hight, int n_regions)
                                : GridWidth(g_width), GridHight(g_hight), NumberOfRegions(n_regions)
{
    Region r1(g_width, g_hight);
    Region *reg = new Region[n_regions];
    for( int i=0; i<n_regions; i++ ){
        reg[i] = r1;
    }    
    Regions = reg;

    int *grid = new int[GridWidth*GridHight];
    for( int i=0; i<GridWidth*GridHight; i++ ) grid[i] = -1;
    Grid = grid;
}

GridOfRegions::~GridOfRegions(){
    delete[] Regions;
    delete[] Grid;
}

void GridOfRegions::InsertRegion(int r, int c, int reg){
    for( int reg2=0; reg2<NumberOfRegions; reg2++ ) 
        Regions[reg2].Grid[r*GridWidth+c] = false;
    Grid[r*GridWidth+c] = reg;
    Regions[reg].Grid[r*GridWidth+c] = true;
}

bool GridOfRegions::IsOverlapping(){
    for( int i=0; i<GridWidth*GridHight; i++ ){
        int sum = 0;
        for( int reg=0; reg<NumberOfRegions; reg++ ){
            if( Regions[reg].Grid[i] ) sum++;
            if( sum>1 ) return true;
        }
    }
    return false;
}

bool GridOfRegions::IsFilled(){
    for( int i=0; i<GridWidth*GridHight; i++ ){
        int sum = 0;
        for( int reg=0; reg<NumberOfRegions; reg++ ){
            if( Regions[reg].Grid[i] ) sum++;
        }
        if( sum == 0 ) return false;
    }
    return true;
}

bool GridOfRegions::AreRegionsEqual(){
    for( int reg=0; reg<NumberOfRegions-1; reg++ ){
        if( Regions[reg].RegionSize() != Regions[reg+1].RegionSize() )
            return false;
    }
    return true;
}

void GridOfRegions::PrintToConsole(){
    for( int r=0; r<GridHight; r++ ){
        for( int c=0; c<GridWidth; c++ ){
            bool IsOccupied = false;
            for( int reg=0; reg<NumberOfRegions; reg++ ){
                if( Regions[reg].Grid[r*GridWidth+c] ){ 
                    std::cout<<reg;
                    IsOccupied = true;
                    break;
                }
            }
            if( !IsOccupied ) std::cout<<"-";
        }
        std::cout<<"\n";
    }
}

void GridOfRegions::CreateRegions(int seed){
    if( (GridWidth*GridHight)%NumberOfRegions != 0 ) return;

    srand(seed);
    int iter=100, iter2=20, iter3, max_iter3=100, r, c;
    for( int i=0; i<iter; i++ ){
        // staring cells for regions
        for( int reg=0; reg<NumberOfRegions; reg++ ){
            r = rand()%GridHight;
            c = rand()%GridWidth;
            iter3 = 0;
            for( int reg2=0; reg2<reg && iter3<max_iter3; reg2++ ){
                if( Regions[reg2].Grid[r*GridWidth+c] ){
                    r = rand()%GridHight;
                    c = rand()%GridWidth;
                    reg2 = -1;
                }
                iter3++;
            }
            Regions[reg].Grid[r*GridWidth+c] = true;
        }
        // growing regions to full size
        for( int j=0; j<iter2; j++ ){
            for( int reg=0; reg<NumberOfRegions; reg++ ){
                // for every region we choose the cell to grow region from
                
                int Cell = rand()%Regions[reg].RegionSize();
                int C1 = 0;
                for( int C=0; C<GridWidth*GridHight; C++ ){
                    if( Regions[reg].Grid[C] && C1 == Cell ){
                        
                    }
                    if( Regions[reg].Grid[C] ) C1++;
                }
            }
            if( IsFilled() && AreRegionsEqual() ) return;
        }
    }
}