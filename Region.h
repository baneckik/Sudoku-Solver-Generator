#include <iostream>

#ifndef REGION
#define REGION

class Region{
    private:
        int GridWidth;
        int GridHight;
        
    public:
        Region(int wid, int hig , bool *grid);
        Region(int wid, int hig);
        bool *Grid;

        int RegionSize();
        bool IsConsistent();
        void PrintToConsole();
};

#endif

Region::Region(int wid, int hig , bool *grid) : GridWidth(wid), GridHight(hig) {
    Grid = new bool[wid*hig];
    for( int i=0;i<wid*hig; i++ ) Grid[i] = grid[i];
};

Region::Region(int wid, int hig ) : GridWidth(wid), GridHight(hig) {
    Grid = new bool[wid*hig];
    for( int i=0;i<wid*hig; i++ ) Grid[i] = false;
};

int Region::RegionSize(){
    int sum = 0;
    for(int i=0; i<GridWidth*GridHight; i++ )
        if( Grid[i] == true ) sum++;
    return sum;
};

void SearchRegion(int r, int c, bool *helpgrid, bool *grid, int wid, int hig ){
    if( r<0 || c<0 || r>=hig || c>= wid ) return;
    if( grid[wid*r+c] == false ) return;
    if( helpgrid[wid*r+c] == true ) return;

    helpgrid[wid*r+c] = true;
    SearchRegion(r-1,c,helpgrid,grid,wid,hig);
    SearchRegion(r+1,c,helpgrid,grid,wid,hig);
    SearchRegion(r,c-1,helpgrid,grid,wid,hig);
    SearchRegion(r,c+1,helpgrid,grid,wid,hig);
}

bool Region::IsConsistent(){
    bool *helpgrid = new bool[GridWidth*GridHight];
    for( int i=0; i<GridWidth*GridHight; i++ ) helpgrid[i] = false;
    
    for( int i=0; i<GridWidth*GridHight; i++ ){
        if( Grid[i] == true ){
            SearchRegion(i/GridWidth,i%GridWidth,helpgrid,Grid,GridWidth,GridHight);
            for( int j=0; j<GridWidth*GridHight; j++ ){
                if( Grid[j] == true && helpgrid[j] == false ){ 
                    delete[] helpgrid;
                    return false;
                }
            }
            delete[] helpgrid;
            return true;
        }
    }
    delete[] helpgrid;
    return true;    //empty region is consistent
}

void Region::PrintToConsole(){
    for( int i=0; i<GridWidth*GridHight; i++ ){
        if( i%GridWidth == 0 ) std::cout<<"\n";
        if(Grid[i]) std::cout<<"1";
        else std::cout<<"0";
    }
    std::cout<<"\n";
}
