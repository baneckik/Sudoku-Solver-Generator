#include <iostream>

#ifndef REGION
#define REGION

class Region{
    private:
        int GridWidth;
        int GridHight;
        
    public:
        bool *Grid;

        Region(int wid, int hig , bool *grid);
        Region(int wid, int hig);
        
        int getWidth(){ return GridWidth; }
        int getHight(){ return GridHight; }
        
        int RegionSize();
        bool IsConsistent();
        void PrintToConsole();
        bool IsLeaf(int r, int c);
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
    /* Help function for IsConsistent(). 
    Function modifies helpgrid to find current consistency component. */

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

bool AreOverlapping(Region r1, Region r2, int wid, int hig){
    for( int i=0; i<wid*hig; i++ )
        if( r1.Grid[i] && r2.Grid[i] ) return true;
    return false;
}

bool AreAdjacentBySide(Region r1, Region r2, int wid, int hig){
    if( AreOverlapping(r1,r2,wid,hig) ) return true;
     
    int w = r1.getWidth(), h = r1.getHight();
    for( int i=0; i<w*h; i++ ){
        if( i%w != w-1 ){
            if( (r1.Grid[i] && r2.Grid[i+1]) || (r2.Grid[i] && r1.Grid[i+1]) ) return true;
        }
        if( i/w != h-1 ){
            if( (r1.Grid[i] && r2.Grid[i+w]) || (r2.Grid[i] && r1.Grid[i+w]) ) return true;
        }
    }
    return false;
}

bool AreAdjacentByCorner(Region r1, Region r2, int wid, int hig){
    if( AreAdjacentBySide(r1,r2,wid,hig) ) return true;
    
    int w = r1.getWidth(), h = r1.getHight();
    for( int i=0; i<w*h; i++ ){
        if( i%w != w-1 && i/w != h-1 ){
            if( (r1.Grid[i] && r2.Grid[i+w+1]) || (r2.Grid[i] && r1.Grid[i+w+1]) ) return true;
        }
        if( i%w != 0 && i/w != h-1 ){
            if( (r1.Grid[i] && r2.Grid[i+w-1]) || (r2.Grid[i] && r1.Grid[i+w-1]) ) return true;
        }
    }
    return false;
}

bool Region::IsLeaf(int r, int c){
    /* Function checks if in case you take cell of coordinates (r,c) off the region 
    it stays consistent. In summary, function returns false if you cannot take given cell away 
    without making region inconsistent. */
    if( r<0 || c<0 || r>=GridWidth || c>= GridHight ) return true;
    Region reg(GridWidth, GridWidth);
    for( int i=0;i<GridWidth*GridHight; i++ ) reg.Grid[i] = Grid[i];
    reg.Grid[r*GridWidth+c] = false;
    return reg.IsConsistent();
}