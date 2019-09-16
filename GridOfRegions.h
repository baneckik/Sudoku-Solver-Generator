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

        GridOfRegions();
        GridOfRegions(int g_width, int g_hight, int n_regions);
        ~GridOfRegions();

        void operator=(GridOfRegions &gor);

        int getGridWidth(){ return GridWidth; }
        int getGridHight(){ return GridHight; }
        int getNumberOfRegions(){ return NumberOfRegions; }

        bool IsOverlapping();
        bool IsFilled();
        bool AreRegionsEqual();
        bool IsEquallyDivided();

        bool IsEmptyCellAround(int r, int c);
        bool IsLeafAround(int r, int c);

        void PrintToConsole();
        void PrintToConsole2();

        void UpdateGrid();
        void InsertRegion(int r, int c, int reg);
        void CreateRegions(int seed);
};

#endif

#ifndef GRIDOFREGIONS_METHODS
#define GRIDOFREGIONS_METHODS

void shuffle(int *array, size_t n){
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

GridOfRegions::GridOfRegions(){
    GridWidth = 9;
    GridHight = 9;
    NumberOfRegions = 9;
    Regions = new Region[NumberOfRegions];
    for( int i=0; i<9; i++ ){
        Regions[i].setWidth(9);
        Regions[i].setHight(9);
        Regions[i].Grid = new bool[GridWidth*GridHight];
        for( int j=0; j<9*9; j++ )
            Regions[i].Grid[j] = false;
    }

    Grid = new int[GridWidth*GridHight];
    for( int i=0; i<9*9; i++ ) Grid[i] = -1;
}

GridOfRegions::GridOfRegions(int g_width, int g_hight, int n_regions)
                                : GridWidth(g_width), GridHight(g_hight), NumberOfRegions(n_regions)
{
    //Region r1(g_width, g_hight);
    Regions = new Region[n_regions];
    for( int i=0; i<n_regions; i++ ){
        Regions[i].setWidth(g_width);
        Regions[i].setHight(g_hight);
        Regions[i].Grid = new bool[g_width*g_hight];
        for( int j=0; j<g_width*g_hight; j++ )
            Regions[i].Grid[j] = false;
    }

    Grid = new int[GridWidth*GridHight];
    for( int i=0; i<GridWidth*GridHight; i++ ) Grid[i] = -1;
}

GridOfRegions::~GridOfRegions(){
    
    delete[] Regions;
    delete[] Grid;
}

void GridOfRegions::operator=(GridOfRegions &gor){
    GridWidth = gor.getGridWidth();
    GridHight = gor.getGridHight();
    NumberOfRegions = gor.getNumberOfRegions();
    
    for( int reg=0; reg<NumberOfRegions; reg++ )
        Regions[reg] = gor.Regions[reg];

    for( int cell=0; cell<GridWidth*GridHight; cell++ )
        Grid[cell] = gor.Grid[cell];
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

bool GridOfRegions::IsEquallyDivided(){
    return IsFilled() && AreRegionsEqual() && !IsOverlapping();
}

bool GridOfRegions::IsEmptyCellAround(int r, int c){
    UpdateGrid();
    if( r>0 && Grid[(r-1)*GridWidth+c]==-1 )
        return true;
    if( r<GridHight-1 && Grid[(r+1)*GridWidth+c]==-1 )
        return true;
    if( c>0 && Grid[r*GridWidth+c-1]==-1 )
        return true;
    if( c<GridWidth-1 && Grid[r*GridWidth+c+1]==-1 )
        return true;
    return false;
}

bool GridOfRegions::IsLeafAround(int r, int c){
    UpdateGrid();
    if(     r>0 && 
            Grid[(r-1)*GridWidth+c]!=Grid[r*GridWidth+c] && 
            Regions[ Grid[(r-1)*GridWidth+c] ].IsLeaf(r-1,c) 
    )
        return true;
    if(     r<GridHight-1 && 
            Grid[(r+1)*GridWidth+c]!=Grid[r*GridWidth+c] &&
            Regions[ Grid[(r+1)*GridWidth+c] ].IsLeaf(r+1,c) 
    )
        return true;
    if(     c>0 && 
            Grid[r*GridWidth+c-1]!=Grid[r*GridWidth+c] &&
            Regions[ Grid[r*GridWidth+c-1] ].IsLeaf(r,c-1) 
    )
        return true;
    if(     c<GridWidth-1 && 
            Grid[r*GridWidth+c+1]!=Grid[r*GridWidth+c] &&
            Regions[ Grid[r*GridWidth+c+1] ].IsLeaf(r,c+1) 
    )
        return true;
    return false;
}

void GridOfRegions::PrintToConsole(){
    std::cout<<"\n";
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

void GridOfRegions::PrintToConsole2(){
    std::cout<<"\n";
    for( int r=0; r<GridHight; r++ ){
        for( int c=0; c<GridWidth; c++ ){
            if( Grid[r*GridWidth+c] == -1 )
                std::cout<<"-";
            else
                std::cout<<Grid[r*GridWidth+c];
        }
        std::cout<<"\n";
    }
}

void GridOfRegions::CreateRegions(int seed){
    if( (GridWidth*GridHight)%NumberOfRegions != 0 ) return;
    int DestinationSize = GridWidth*GridHight/NumberOfRegions;

    srand(seed);
    int iter=100;   // How many times function can start from the beggining
    int max_iter2=20;   // How many rounds function changes existing regions
    int iter3, max_iter3=100;   // How many times function is choosing starting cells for regions
    int r, c;
    for( int i=0; i<iter; i++ ){
        // Cleaning the existing regions
        delete[] Regions;
        Regions = new Region[NumberOfRegions];
        for( int i=0; i<NumberOfRegions; i++ ){
            Regions[i].setWidth(GridWidth);
            Regions[i].setHight(GridHight);
            Regions[i].Grid = new bool[GridWidth*GridHight];
            for( int j=0; j<GridWidth*GridHight; j++ )
                Regions[i].Grid[j] = false;
        }
        Grid = new int[GridWidth*GridHight];
        for( int i=0; i<GridWidth*GridHight; i++ ) Grid[i] = -1;

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
            if( iter3 < max_iter3){
                Regions[reg].Grid[r*GridWidth+c] = true;
                Grid[r*GridWidth+c] = reg;    
            }
            else
                return;    
        }

        // growing regions to full size
        bool WeAreStuck = false;
        for( int j=0; j<max_iter2; j++ ){
            if( WeAreStuck ) break;
            // j-th round for growing regions
            for( int reg=0; reg<NumberOfRegions; reg++ ){
                if( WeAreStuck ) break;
                // for every region
                for( int k=0; k<j+2-Regions[reg].RegionSize(); k++ ){
                    // for region reg we add "k" cells to make all regions equal
                    // Region size should equal min(j+2,9) after j-th round
                    
                    //PrintToConsole();

                    if( Regions[reg].RegionSize() == DestinationSize ) break;
                    // for every region we choose the cell "Cell" to grow region from
                    int progress = false;
                    
                    int Cell = rand()%Regions[reg].RegionSize(); // first candidate for grow cell
                    int Cell1 = -1, iter4=0;
                    for( int C=0; C<GridWidth*GridHight && iter4<Regions[reg].RegionSize(); C++ ){
                        if( Regions[reg].Grid[C] ) Cell1++;

                        if( Regions[reg].Grid[C] && Cell1 == Cell ){
                            if( Regions[reg].IsBoundary(C/GridWidth,C%GridWidth) && 
                                IsEmptyCellAround(C/GridWidth,C%GridWidth) ){
                                // We have found good cell to grow region from
                                Cell1 = C; // Cell1 is now index of our Cell in Grid
                                r = Cell1/GridWidth;
                                c = Cell1%GridWidth;
                                progress = true;
                                break;
                            }
                            else{
                                // We are looking for another good cell to grow region from
                                Cell++;
                                Cell = Cell%Regions[reg].RegionSize();
                                C=-1;
                                Cell1=-1;
                                iter4++;
                            }
                        }
                        
                    }

                    // if there is no cell touching any empty cell around
                    // we are looking for cells that are leaves of other regions
                    if( !progress ){
                        int Cell = rand()%Regions[reg].RegionSize(); // first candidate for grow cell
                        int Cell1 = -1, iter4=0;
                        for( int C=0; C<GridWidth*GridHight && iter4<Regions[reg].RegionSize(); C++ ){
                            if( Regions[reg].Grid[C] ) Cell1++;
                            
                            if( Regions[reg].Grid[C] && Cell1 == Cell ){
                                if( Regions[reg].IsBoundary(C/GridWidth,C%GridWidth) && 
                                    IsLeafAround(C/GridWidth,C%GridWidth) ){
                                    // We have found good cell to grow region from
                                    Cell1 = C; // Cell1 is now index of our Cell in Grid
                                    r = Cell1/GridWidth;
                                    c = Cell1%GridWidth;
                                    progress = true;
                                    break;
                                }
                                else{
                                    // We are looking for another good cell to grow region from
                                    Cell++;
                                    Cell = Cell%Regions[reg].RegionSize();
                                    C=0;
                                    Cell1=-1;
                                    iter4++;
                                }
                            }
                            
                        }
                    }
                    Cell1=r*GridWidth+c;
                    
                    if( !progress || Cell1<0 ){
                        // means if we haven't found a cell good to grow from
                        WeAreStuck = true;
                        break;
                    }else{
                        // if we have found a cell good to grow from
                        progress = false;
                        // if there is an empty cell around
                        if( IsEmptyCellAround(r,c) ){
                            std::vector<int> directories = {0,1,2,3};
                            std::random_device rd;
                            std::mt19937 g(rd());
                            std::shuffle(directories.begin(),directories.end(),g);
                            for( int l=0; l<4; l++ ){
                                // cell to the top is empty
                                if( directories[l] == 0 && r>0 && Grid[Cell1-GridWidth] == -1 ){
                                    InsertRegion(r-1,c,reg);
                                    progress = true;
                                    break;
                                }
                                // cell to the bottom is empty
                                if( directories[l] == 1 && r<GridHight-1 && Grid[Cell1+GridWidth] == -1 ){
                                    InsertRegion(r+1,c,reg);
                                    progress = true;
                                    break;
                                }
                                // cell to the left is empty
                                if( directories[l] == 2 && c>0 && Grid[Cell1-1] == -1 ){
                                    InsertRegion(r,c-1,reg);
                                    progress = true;
                                    break;
                                }
                                // cell to the right is empty
                                if( directories[l] == 3 && c<GridWidth-1 && Grid[Cell1+1] == -1 ){
                                    InsertRegion(r,c+1,reg);
                                    progress = true;
                                    break;
                                }
                            }
                        }

                        //if there is a leaf to take from another region around
                        if( !progress && IsLeafAround(r,c) ){
                            std::vector<int> directories = {0,1,2,3};
                            std::random_device rd;
                            std::mt19937 g(rd());
                            std::shuffle(directories.begin(),directories.end(),g);
                            for( int l=0; l<4; l++ ){
                                // cell to the top is other regions leaf
                                if( directories[l] == 0 && r>0 ){
                                    int reg2 = Grid[Cell1-GridWidth];
                                    if( reg2 != -1 && reg2 != reg && 
                                    Regions[reg2].RegionSize()>1 && Regions[reg2].IsLeaf(r-1,c) 
                                    ){
                                        InsertRegion(r-1,c,reg);
                                        progress = true;
                                        break;
                                    }
                                }
                                // cell to the bottom is other regions leaf
                                if( directories[l] == 1 && r<GridHight-1 ){
                                    int reg2 = Grid[Cell1+GridWidth];
                                    if( reg2 != -1 && reg2 != reg &&
                                    Regions[reg2].RegionSize()>1 && Regions[reg2].IsLeaf(r+1,c) 
                                    ){
                                        InsertRegion(r+1,c,reg);
                                        progress = true;
                                        break;
                                    }
                                }
                                // cell to the left is other regions leaf
                                if( directories[l] == 2 && c>0 ){
                                    int reg2 = Grid[Cell1-1];
                                    if( reg2 != -1 && reg2 != reg &&
                                    Regions[reg2].RegionSize()>1 && Regions[reg2].IsLeaf(r,c-1) 
                                    ){
                                        InsertRegion(r,c-1,reg);
                                        progress = true;
                                        break;
                                    }
                                }
                                // cell to the right is other regions leaf
                                if( directories[l] == 3 && c<GridWidth-1 ){
                                    int reg2 = Grid[Cell1+1];
                                    if( reg2 != -1 && reg2 != reg &&
                                    Regions[reg2].RegionSize()>1 && Regions[reg2].IsLeaf(r,c+1) 
                                    ){
                                        InsertRegion(r,c+1,reg);
                                        progress = true;
                                        break;
                                    }
                                }
                            }    
                        }
                    }

                    if( !progress ){
                        WeAreStuck = true;
                        break;
                    }
                }
            }
            if( IsEquallyDivided() ){ 
                UpdateGrid();
                return;
            }
        }
    }
}

void GridOfRegions::UpdateGrid(){
    for( int r=0; r<GridHight; r++ ){
        for( int c=0; c<GridWidth; c++ ){
            bool IsOccupied = false;
            for( int reg=0; reg<NumberOfRegions; reg++ ){
                if( Regions[reg].Grid[r*GridWidth+c] ){ 
                    Grid[r*GridWidth+c] = reg;
                    IsOccupied = true;
                    break;
                }
            }
            if( !IsOccupied ) 
                Grid[r*GridWidth+c] = -1;
        }
    }
}

#endif  //GRIDOFREGIONS_METHODS